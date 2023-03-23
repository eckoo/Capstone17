// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Wire.h>


#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

Adafruit_MPU6050 mpu;
const int buttonPin = 7;
const int FRAMESIZE = 13;
int FRAME[FRAMESIZE][FRAMESIZE];
int x = FRAMESIZE/2;
int y = FRAMESIZE/2; //I want to be in the center of the frame
double THRESHOLD = 2;

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
  pinMode(buttonPin, INPUT);
  clearFrame();
  delay(100);
}



float accel_x, accel_y, accel_z;
float roll, pitch, yaw;

void loop() {
  int val = 0;
  digitalWrite(13,HIGH);
  int orientation = getAccelerometerOrientation();
  processAccelerometer(orientation);
  //Serial.println("x " + String(x) + " y " + String(y));
  val = digitalRead(buttonPin);
  printFrame();
  if(val){
    clearFrame();
    Serial.println("Frame environment reset");
  }
  /*if(val == 1){
    printFrame();
    clearFrame();
    x = FRAMESIZE/2;
    y = FRAMESIZE /2;
    digitalWrite(13,LOW);
     - click to draw again");
    val = digitalRead(buttonPin);
    while(val == 0){
      //spin lock
      val = digitalRead(buttonPin);
      delay(100);
    }
  }*/
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
  FRAME[x][y] = 1;
  SerialBT.print(String(x) + "," + String(y));
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

void printFrame(){
  for(int i = 0; i < FRAMESIZE; i++){
    for(int j = 0; j < FRAMESIZE; j++){
      if(FRAME[i][j] == 0){
        Serial.print(" ");
      }else{
        if(i == x & j == y){
          Serial.print(">");
        }else{
          Serial.print("*");
        }
      }
    }
    Serial.println();
  }
  Serial.println("-------------------- Cursor Position X: " + String(x) + " Y: " +String(y)  );
}

void clearFrame(){
  x = FRAMESIZE/2;
  y = FRAMESIZE/2;
  for(int i = 0; i < FRAMESIZE; i++){
    for(int j = 0; j < FRAMESIZE; j++){
      FRAME[i][j] = 0;
    }
  }
}
