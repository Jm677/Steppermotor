#include <SoftwareSerial.h>
#include "StepperMotor.h"
SoftwareSerial Seriell(12, 13);

#define RohrRadius 59
#define PitotLaenge 47
#define KollisionsSicherheitsAbstand 10

#define TransDirPin 39
#define TransStepPin 38
#define TransMsPin1 201
#define TransMsPin2 200
#define TransEnablePin 40
#define TransEndtasterPin A5
int nullposition =98;
StepperMotor Rot(TransDirPin, TransStepPin, TransEnablePin, TransMsPin1, TransMsPin2, TransEndtasterPin, 0, nullposition, true,false);

void setup()
{
  Serial.begin(9600);
  Rot.Home(64);
}

void loop()
{
  if(Serial.available())
  {

    Rot.Deg(nullposition-Serial.parseFloat(),64);
    Serial.read();
  }
}
