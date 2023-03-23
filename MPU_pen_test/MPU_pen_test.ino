// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

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
  delay(100);
}



float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float elapsedTime, currentTime, previousTime;
void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  /* Print out the values */
  accel_x = a.acceleration.x;
  accel_y = a.acceleration.y;
  accel_z = a.acceleration.z;
  gyro_x = g.gyro.x;
  gyro_y = g.gyro.y;
  gyro_z = g.gyro.z;
  
  int orientation = 0; //Ill do states 1 = z is "up" 2 = y 3 = x
  float maxVal = max(max(abs(accel_x),abs(accel_y)), abs(accel_z));
  if(maxVal == abs(accel_x)){
      orientation = 3;
      Serial.println("X is up");
  }else if(maxVal == abs(accel_y)){ 
      orientation = 2;
      Serial.println("Y is up");
  }else if(maxVal == abs(accel_z)){ 
    orientation = 1;
    Serial.println("Z is up");
  }
  
}
