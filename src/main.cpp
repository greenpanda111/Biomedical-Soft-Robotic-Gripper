#include "Arduino.h"
#include "NESControllerInterface.h"

#define OPEN_PIN 26

int PWM_PIN = 25;
int AIN1 = 18;
int AIN2 = 19;
int STBY = 5;

int freq = 5000;
int resolution = 8;

void maxOpen(void);
void Open(void);
void Close(void);
void stop(void);

// NESControllerInterface nes(16, 17, 5);

void setup()
{
  Serial.begin(9600);

  ledcSetup(0, freq, resolution);
  ledcAttachPin(PWM_PIN, 0);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(OPEN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(OPEN_PIN), maxOpen, RISING);

  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
}

void loop()
{

  Serial.println("----------------------------------");
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  ledcWrite(0, 127);

  Serial.print("PWM: ");
  Serial.print(ledcRead(0));  
  Serial.print(", STBY: ");
  Serial.print(digitalRead(STBY));
  Serial.print(", AIN1: ");
  Serial.print(digitalRead(AIN1));
  Serial.print(", AIN2: ");
  Serial.println(digitalRead(AIN2));



  delay(100);
  Serial.println("");
  /*
    NESInput input = nes.getNESInput();

  if (input.buttonLeft == true)
  {
    Open();
  }

  if (input.buttonRight == true)
  {
    Close();
  }
  */
}

void stop()
{
}

void Open()
{
}

void Close()
{
}

void maxOpen()
{
  stop();
}
