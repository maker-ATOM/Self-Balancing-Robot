//include neccessary libraries
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;

int accY, accZ, gyroX;
float accAngle = 0, gyroAngle = 0, botAngle = 0, previousAngle = 0;
float gyroRate = 0;

void setup() 
{
  mpu.initialize();
  Serial.begin(9600);
  
  //function to setup Timer2
  initTimer2();
}

void loop() 
{
  //get raw data from mpu6050
  accZ = mpu.getAccelerationZ();
  accY = mpu.getAccelerationY();
  gyroX = mpu.getRotationX();

  //calculate angualar speed in deg/sec
  gyroRate = gyroX / 131;

  //calcualate tilt angle using accelerometer
  accAngle = atan2(accZ, -accY) * RAD_TO_DEG;

  //complementary filter
  botAngle = 0.97 * (previousAngle + gyroAngle) + 0.03 * (accAngle);
  previousAngle = botAngle;

  //print the calculated tilt angle
  Serial.println(botAngle);
}

ISR(TIMER2_COMPA_vect)
{
  gyroAngle = (float)gyroRate * 0.001;
}

void initTimer2()
{
  //reset timer2 control register A
  TCCR2A = 0;

  //set CTC mode
  TCCR2A |= (1 << WGM21);
  TCCR2A &= ~(1 << WGM20);
  TCCR2B &= ~(1 << WGM22);

  //prescaler of 1024
  TCCR2B &= ~(1 << CS21);
  TCCR2B |= ((1 << CS22) | (1 << CS20));

  //reset counter
  TCNT2 = 0;

  //set compare register
  OCR2A = 125;

  //enable timer1 compare match interrupt
  TIMSK2 |= (1 << OCIE2A);

  //enable global interrupt
  sei();
}
