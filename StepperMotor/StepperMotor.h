#ifndef StepperMotor_h
#define StepperMotor_h
#include "Arduino.h"
class StepperMotor {

  private: int DirPin, StepPin, EnablePin, MsPin1, MsPin2, EndPin;
  public: float MaxPos, MinPos, IstPos;
  public: float Speed = 50;
  public:
    StepperMotor(int dirPin, int stepPin, int enablePin, int msPin1, int msPin2, int endPin, float minPos, float maxPos);

    void Step(double Steps, int MS);

    void MM(float MM, int MS);

    void Full(char Dir, int Umdrehungen, int MS);

    void Abs(float Soll, int MS);

    void Deg(float Soll, int MS);

    void Home(int MS);

    void Spindel(float MM, int MS);

    void VLR(float Alpha, int MS);

    void setSPEED(int SPEED);

    void disable();
};
#endif
