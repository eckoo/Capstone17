/**
 * Author: Rob Reid
 * Project: Rutgers ECE Capstone Group 17 Smart Pen
 * Components: Parallax Trackball, ESP32 WROOM DOIT Dev Kit V1, SSd1306 128x32 OLED display, MPU6050 Accelerometer and Gyroscope
 * 
 */
#include <ADNS2610.h>

// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   19
#define OLED_CLK   18
#define OLED_DC    5
#define OLED_CS    16
#define OLED_RESET 17
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

Adafruit_MPU6050 mpu;
const int FRAMESIZE = 30;
const int button_pin = 13;
int x = 0;
int y = 0; //I want to be in the center of the frame
double THRESHOLD = 2;

int upPin = 34; //OPA
int downPin = 32; //OPC
int rightPin = 33; //OPD
int leftPin = 35; //OPB

void setup(void) {
  SerialBT.begin("Smart Pen 0.0");
  Serial.begin(115200);
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  Serial.println("");
  pinMode(button_pin, INPUT);
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(1000); //delay 1 second on start up
  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("Bluetooth:OFF"));
  display.display();
  delay(100);
}



float accel_x, accel_y, accel_z;
float roll, pitch, yaw;
int temp_x = x;
int temp_y = y;
boolean o = false;
void loop() {
  
  if(!o){
    while(SerialBT.read() < 0){
    //if we dont receive anything
    }
    display.clearDisplay();
    display.setRotation(2);
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("Bluetooth:ON"));
    display.display();
    delay(100);
    o = true;
  }
  
  int val = 0;
  int orientation = getAccelerometerOrientation();
  //processAccelerometer(orientation);
  processTrackball();
  if(orientation ==2){//check for correct orientation
    x = temp_x;
    y = temp_y;
  }else{
    temp_x = x;
    temp_y = y;
  }
  Serial.println("x " + String(x) + " y " + String(y) + " orientation " + String(orientation));
  val = digitalRead(button_pin);
  if(val){
    SerialBT.print("peepee");
  }else{
    SerialBT.print(String(x) + "," + String(y));
  }
  delay(50);
}
int up, down, left, right = 0;
void processTrackball(){
  

  if(up != digitalRead(upPin)){
    temp_y++;
    up = digitalRead(upPin);
  }
  
  if(down != digitalRead(downPin)){
    temp_y--;
    down = digitalRead(downPin);
  }
  
  if(left != digitalRead(leftPin)){
    temp_x--;
    left = digitalRead(leftPin);
  }

  if(right != digitalRead(rightPin)){
    temp_x++;
    right = digitalRead(rightPin);
  }
  if(temp_x >= FRAMESIZE) temp_x = FRAMESIZE-1;
  if(temp_x < 0) temp_x = 0;
  if(temp_y >= FRAMESIZE) temp_y = FRAMESIZE-1;
  if(temp_y < 0) temp_y = 0;
}
void processAccelerometer(int orientation){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  accel_x = a.acceleration.x;
  accel_y = a.acceleration.y;
  accel_z = a.acceleration.z;
  double xVal = 0; 
  double yVal = 0;
  if(orientation == 1){//Z is up
     xVal = accel_x;
     yVal = accel_y;
  }else if(orientation == 2){//Y is up
    xVal= accel_x;
    yVal = accel_z;
  }else if(orientation == 3){//X is up
    xVal = accel_y;
    yVal = accel_x;
    
  }
  //Serial.println("X VAl " + String(xVal));
  //Serial.println("Y VAL " + String(yVal));
  if(xVal < 0 & xVal < (THRESHOLD *-1)){
     x-=1;
  }else if(xVal > 0 & xVal > (THRESHOLD)){
     x+=1;
  }
  if(yVal < 0 & yVal < (THRESHOLD *-1)){
    y-=1;
  }else if(yVal > 0 & yVal > (THRESHOLD)){
    y+=1;
  }
  if(x >= FRAMESIZE) x = FRAMESIZE-1;
  if(x < 0) x = 0;
  if(y >= FRAMESIZE) y = FRAMESIZE-1;
  if(y < 0) y = 0;
  
  delay(200);
}

int getAccelerometerOrientation(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  accel_x = a.acceleration.x;
  accel_y = a.acceleration.y;
  accel_z = a.acceleration.z;
  
  int orientation = 0; //Ill do states 1 = z is "up" 2 = y 3 = x
  float maxVal = max(max(abs(accel_x),abs(accel_y)), abs(accel_z));
  if(maxVal == abs(accel_x)){
      orientation = 3;
      //Serial.println("X is up");
  }else if(maxVal == abs(accel_y)){ 
      orientation = 2;
      //Serial.println("Y is up");
  }else if(maxVal == abs(accel_z)){ 
    orientation = 1;
    //Serial.println("Z is up");
  }
  return orientation; 
}
