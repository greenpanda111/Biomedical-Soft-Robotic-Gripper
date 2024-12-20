#include "Arduino.h"
#include "NESControllerInterface.h"

// limit switch pins
#define OPEN_BUTTON 26

// motor pins
#define PWM_PIN 25
#define AIN1 18
#define AIN2 19
#define STBY 5

// encoder pins
#define ENCODER_PINA 2
#define ENCODER_PINB 15

// NES controller pins
#define DATA_PIN 4
#define LOAD_PIN 16
#define CLOCK_PIN 17

// ledc setup parameters
#define FREQUENCY 5000
#define RESOLUTION 8

// speed of motor (0-255)
#define MOTOR_SPEED 255

// function definitions
void maxOpen(void);
void CW(void);
void CCW(void);
void stop(void);
void countPulse(void);

volatile long int pulseCount;
bool isrFired;

// construct nes controller object
NESControllerInterface nes(DATA_PIN, LOAD_PIN, CLOCK_PIN);

void setup()
{
  Serial.begin(9600);

  // setup pwm output pin
  ledcSetup(0, FREQUENCY, RESOLUTION);
  ledcAttachPin(PWM_PIN, 0);

  // setup digital motorcontroller output pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  // setup Encoder pins and attach ISR
  pinMode(ENCODER_PINA, INPUT_PULLUP);
  pinMode(ENCODER_PINB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINA), countPulse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINB), countPulse, RISING);
  pulseCount = 0;

  // attach limit switch interrupt pins to ISR
  pinMode(OPEN_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(OPEN_BUTTON), maxOpen, RISING);

  // take motor controller out of standby mode
  digitalWrite(STBY, HIGH);
}

void loop()
{

  Serial.println("----------------------------------");

  Serial.print("EncoderCount: ");
  Serial.println(pulseCount);
  CW();
  // get current NES controller input
  NESInput input = nes.getNESInput();

  // if left button pressed
  if (input.buttonLeft == true)
  {
    // rotate clockwise for a short amount of time
    CCW();
    delay(100);
    stop();
  }

  // if right button pressed
  if (input.buttonRight == true)
  {
    // rotate counter-clockwise for a short period of time
    CCW();
    delay(100);
    stop();
  }

  Serial.println("");
}

// function for stopping the motor rotating
void stop()
{
  // both direction pins are set to 0
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);

  // pwm set to 0% duty cycle
  ledcWrite(0, 0);
}

// function to rotate the motor clockwise
void CW()
{
  // set direction pins to 10
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  // set pwm to set speed
  ledcWrite(0, MOTOR_SPEED);
}

// function to rotate the motor counter-clockwise
void CCW()
{
  // set direction pins to 01
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  // set pwm to set speed
  ledcWrite(0, MOTOR_SPEED);
}

// function to stop the motor when it reaches the end of the rack
void maxOpen()
{
  stop();
}

void countPulse()
{

  if ((ENCODER_PINA == HIGH) & (ENCODER_PINB == LOW))
  {
    pulseCount++;
  }
  else if ((ENCODER_PINA == LOW) & (ENCODER_PINB== HIGH))
  {
    pulseCount--;
  }
  else{

  }
  
}
