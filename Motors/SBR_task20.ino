//The direction and rpm of the motor is controlled by
//tilt angle of the robot

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;

int accY, accZ, gyroX;
float accAngle = 0, gyroAngle = 0, botAngle = 0, previousAngle = 0;
float gyroRate = 0;

int val = 255;

void setup() {

  mpu.initialize();
  Serial.begin(9600);

  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);

  initTimer2();

}

void loop() {

  accZ = mpu.getAccelerationZ();
  accY = mpu.getAccelerationY();

  gyroX = mpu.getRotationX();

  accAngle = atan2(accZ, -accY) * RAD_TO_DEG;

  gyroRate = gyroX / 131;

  botAngle = 0.97 * (previousAngle + gyroAngle) + 0.03 * (accAngle);
  previousAngle = botAngle;

  if (botAngle > 0)
  {
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    val = map(botAngle, 0, 90, 30, 225);
    analogWrite(5, val);
    analogWrite(6, val);
  }

  if (botAngle < 0)
  {
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    val = map(botAngle, -90, 0,30, 225);
    analogWrite(5, val);
    analogWrite(6, val);

  }

  Serial.println(val);
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
