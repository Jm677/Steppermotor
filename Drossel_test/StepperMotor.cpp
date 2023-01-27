#include "StepperMotor.h"

#define Umdrehung 200
#define MinDelay 50             //Microseconds
#define MaxDelay 1000           //Microseconds
#define DurchmesserRitzel 12.7  //mm
#define RotTimeLimit 7000
#define TransTimeLimit 10000


StepperMotor::StepperMotor(int dirPin, int stepPin, int enablePin, int msPin1, int msPin2, int endPin, float minPos, float maxPos, bool invert, bool invertHome) {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(msPin1, OUTPUT);
  pinMode(msPin2, OUTPUT);
  pinMode(endPin, INPUT);
  //digitalWrite(enablePin, HIGH);
  DirPin = dirPin;
  StepPin = stepPin;
  EnablePin = enablePin;
  MsPin1 = msPin1;
  MsPin2 = msPin2;
  EndPin = endPin;
  MaxPos = maxPos;
  MinPos = minPos;
  Invert=invert;
  InvertHome=invertHome;
}

void StepperMotor::Step(double Steps, int MS) {
  bool first = true, ausfuehren = true;
  //Serial.println("Steps in Steps: "+String(Steps));
  int Inv=1;
  if(Invert) Inv=-1;
  Steps=round(Steps*MS*Inv);
  double Delay = map(Speed, 100, 1, (1. / MinDelay) * 1000., (1. / MaxDelay) * 1000.) / 1000.;
  if (Steps < 0) {
    digitalWrite(DirPin, HIGH);
    Steps *= -1;
  } else if (Steps > 0) digitalWrite(DirPin, LOW);
  else {
    //Serial.println("Fehler bei Richtung");
    ausfuehren = false;
  }
  if (ausfuehren && MS != 0) {
    switch (MS) {
      case 8:
        digitalWrite(MsPin1, LOW);
        digitalWrite(MsPin2, LOW);
        break;
      case 16:
        digitalWrite(MsPin1, HIGH);
        digitalWrite(MsPin2, HIGH);
        break;
      case 32:
        digitalWrite(MsPin1, HIGH);
        digitalWrite(MsPin2, LOW);
        break;
      case 64:
        digitalWrite(MsPin1, LOW);
        digitalWrite(MsPin2, HIGH);
        break;
    }


    digitalWrite(EnablePin, LOW);
    for (int i = 0; i < Steps; i++) {
      digitalWrite(StepPin, HIGH);
      delayMicroseconds(1 / Delay);
      digitalWrite(StepPin, LOW);
      delayMicroseconds(1 / Delay);
    }
  }
}

void StepperMotor::MM(float MM, int MS) {
  double steps;
  if (MS != 0) steps = double((Umdrehung*MS / float(DurchmesserRitzel * PI)) * MM);
  // Serial.println("Steps from MM: "+String(steps)+" for "+ String(MM));
  Step(steps, MS);
}

void StepperMotor::Full(char Dir, int Umdrehungen, int MS) {
  for (int i = 0; i < Umdrehungen; i++) {
    Step(Umdrehung*MS, MS);
  }
}
/*
void StepperMotor::setSPEED(int SPEED) {

  if (SPEED < 1) SPEED = 1;
  else if (SPEED > 100) SPEED = 100;
  Speed = SPEED;
  //Serial.println("S: "+String(Speed));
}*/
void StepperMotor::Abs(float Soll, int MS) {
  if (Soll >= MaxPos) Soll = MaxPos;
  else if (Soll < MinPos) Soll = MinPos;
  //Serial.println("Distance to Drive: "+String(IstPos)+"mm");
  //Serial.println("Distance to Drive: "+String(Soll)+"mm");
  MM(Soll - IstPos, MS);
  IstPos = Soll;
}

void StepperMotor::Deg(float Soll, int MS) {
  if (Soll >= MaxPos) Soll = MaxPos;
  else if (Soll <= MinPos) Soll = MinPos;
  float sollSteps = (Soll - IstPos) / 360. * float(Umdrehung);
   Serial.println("Soll Steps: "+String(sollSteps*MS));
  Step(sollSteps, MS);
  IstPos += round(sollSteps*MS)/float(MS)/float(Umdrehung)*360.*Invert;
 //IstPos=Soll;
  Serial.println("Ist Pos: "+String(IstPos));
}
void StepperMotor::Spindel(float MM, int MS) {
  Abs(MM * -1, MS);
}
void StepperMotor::VLR(float Alpha, int MS) {
  // AN DEN ARDUINO GESENDET:
  // alpha_VLR = ...;
  // Geometrien Deklarieren
  float phi_0 = 45 * PI / 180;
  float l_h = 60;
  float l_hxmin = l_h * cos(phi_0);
  float l_hzmin = l_h * sin(phi_0);
  float a_Motorphi0_z = 103;
  float a_Motorphi0_x = 21;
  // Formeln
  // Winkel Deklarieren

  float phi_deg = 45 - Alpha;
  float phi = phi_deg * PI / 180;
  // Zwischenrechnung
  float a_z = l_hzmin - l_h * sin(phi);
  float delta_S_rt = l_h * cos(phi) - l_hxmin;
  // Rechnung

  float delta_S_M = -(sqrt((a_Motorphi0_z + a_z) * (a_Motorphi0_z + a_z) + (a_Motorphi0_x + delta_S_rt) * (a_Motorphi0_x + delta_S_rt) - a_Motorphi0_x * a_Motorphi0_x) - a_Motorphi0_z);
  //Serial.println(delta_S_M);
  Spindel(delta_S_M, MS);
}
void StepperMotor::Home(int MS) {

  int S = Speed;
  //setSPEED(30);
  int i;
  while (digitalRead(EndPin)) {
    int Inv=1;
    if(InvertHome)Inv=-1;
    Step(-1*Inv, MS);
    //if (i % 50 == 0) Serial.print(".");
    i++;
  }
  IstPos = MinPos;
  //setSPEED(S);
}
void StepperMotor:: disable()
{
  digitalWrite(EnablePin,HIGH);
}
