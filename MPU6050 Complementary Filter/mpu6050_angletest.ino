//measures angle using only accelerometer
//for self balancing robot with required orrentation

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

MPU6050 mpu;

int accY, accZ;
float accAngle;

void setup() {
  mpu.initialize();
  Serial.begin(9600);
}

void loop() {
  accZ = mpu.getAccelerationZ();
  accY = mpu.getAccelerationY();

  //RAD_TO_DEG = 180 / PI;
  accAngle = atan2(accZ, -accY) * RAD_TO_DEG;

  Serial.println(accAngle);
}
