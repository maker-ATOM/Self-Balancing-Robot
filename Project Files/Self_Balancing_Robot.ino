//Installing necessary libraries
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <PID_v1.h>

//defined macros
#define LeftMotorDir 11
#define LeftMotorPower 5
#define RightMotorDir 10
#define RightMotorPower 6

#define LeftMotorEnable 8
#define RightMotorEnable 9

//the angle where the robot is stable
double Setpoint = -0.75;

double Input, Output;

//PID controllers
double Kp = 4.6;
double Kd = 0.04;
double Ki = 1;

//required variables
int accY, accZ, gyroX;
float accAngle = 0, gyroAngle = 0, previousAngle = 0;
float gyroRate = 0;
int val = 0;

//instance of class PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

//instance of class MPU6050
MPU6050 mpu;

/*.............................SETUP.................................*/
/*...................................................................*/

void setup() {

  //initializing MPU6050
  mpu.initialize();

  //setting the pinmodes
  pinMode(LeftMotorEnable, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
  pinMode(LeftMotorPower, OUTPUT);

  pinMode(RightMotorEnable, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(RightMotorPower, OUTPUT);

  //making enable pin high
  digitalWrite(LeftMotorEnable, HIGH);
  digitalWrite(RightMotorEnable, HIGH);

  //setting PID parameters
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255); // may change (50,255);
  myPID.SetSampleTime(5); //how often pid is evaluated in millisec
  myPID.SetControllerDirection(REVERSE);

  //initialize the timer
  initTimer2();

}

/*..............................LOOP.................................*/
/*...................................................................*/

void loop() {

  accZ = mpu.getAccelerationZ();
  accY = mpu.getAccelerationY();

  gyroX = mpu.getRotationX();

  accAngle = atan2(accZ, -accY) * RAD_TO_DEG;

  gyroRate = gyroX / 131;

  Input = 0.97 * (previousAngle + gyroAngle) + 0.03 * (accAngle);
  previousAngle = Input;

  myPID.Compute();

  if (Output > Setpoint)
  {
    digitalWrite(LeftMotorDir, LOW);
    digitalWrite(RightMotorDir, LOW);
    val = map(Output, 0, 255, 18, 255);
    analogWrite(LeftMotorPower, val);
    analogWrite(RightMotorPower, val);
  }

  if (Output < Setpoint)
  {
    digitalWrite(LeftMotorDir, HIGH);
    digitalWrite(RightMotorDir, HIGH);
    val = map(Output, -255, 0, 0, 237);
    analogWrite(LeftMotorPower, val);
    analogWrite(RightMotorPower, val);
  }
}


/*................... ........ISR_TIMER2.............................*/
/*...................................................................*/

ISR(TIMER2_COMPA_vect)
{
  gyroAngle = (float)gyroRate * 0.001;
}

/*...........................iniTimer2...............................*/
/*...................................................................*/

void initTimer2()
{
  //reset timer2 control register A
  TCCR2A = 0;

  //set CTC mode
  TCCR2A |= (1 << WGM21);
  TCCR2A &= ~(1 << WGM20);
  TCCR2B &= ~(1 << WGM22);

  //prescaler of 128
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
