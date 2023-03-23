#include <ArduinoQueue.h>

// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup(void) {
  Serial.begin(9600);
  SerialBT.begin("Smart Pen 0.0");


  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
}


float roll, pitch, yaw;
float elapsedTime, currentTime, previousTime;
double previousTheta, previousGamma, previousPhi;
double calcTheta, calcGamma, calcPhi;
const int numAverage = 5;
int averageCounter = 0;
ArduinoQueue<double> data_Theta(numAverage);
ArduinoQueue<double> data_Gamma(numAverage);
ArduinoQueue<double> data_Phi(numAverage);

double previousRoll, previousPitch, previousYaw;
void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000;
  /* Print out the values */
  //We know that the Gyro is in degrees per second 
  double ax, ay, az;
  previousTheta = calcTheta;
  previousGamma = calcGamma;
  previousPhi = calcPhi;
  previousRoll = roll;
  previousPitch = pitch;
  previousYaw = yaw;
  calcTheta = 0;
  calcGamma = 0;
  calcPhi = 0;
  ax = a.acceleration.x;
  ay = a.acceleration.y;
  az = a.acceleration.z;
  const int LIM = 20;
  for(int x = 0; x < LIM; x++){
    calcTheta += (ax)/(sqrt(ay*ay + az*az));
    calcGamma += (ay)/(sqrt(ax*ax + az*az));
    calcPhi += (az)/(sqrt(ay*ay + ax*ax));  
  }
  calcTheta = calcTheta/LIM;
  calcGamma = calcGamma/LIM;
  calcPhi = calcPhi/LIM;
  
  const int THRESHOLD = 3; //degrees
  
  roll = round(atan(calcTheta) * 180/3.14159265 );
  pitch = round( atan(calcGamma) * 180/3.14159265 );
  yaw = round( atan(calcPhi) * 180/3.14159265 );
  if((abs(previousRoll - roll) < THRESHOLD)){
     //if less than threshold do not overwrite previousTheta with new calcTheta. easiest way is to just replace 
     roll = previousRoll;
  }
  if((abs(previousPitch - pitch) < THRESHOLD)){
     pitch = previousPitch;
  }
  if((abs(previousYaw - yaw) < THRESHOLD)){
     yaw = previousYaw;
  }
  // Serial.print("roll : ");
  String line = String(roll) + " , " + String(pitch) + " , " + yaw;
  Serial.println(line);
  SerialBT.println(line);

  delay(10);
}
