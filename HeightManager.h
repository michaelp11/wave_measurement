#ifndef HEIGHTMANAGER_H
#define HEIGHTMANAGER_H

#include <Arduino.h>
#define SAMPLE_SIZE 50
class HeightManager{
  public:
    HeightManager();
    void setup();
    void captureHeight();
    float calculateHeight(float theta);
    float getLastMeasurement();

    float measurements[SAMPLE_SIZE];
    float height[SAMPLE_SIZE];

    float armEndHeight;                          // the height of the lower end of the tilting arm above the floor
    float thetaDegree;                           // the angle of the tilting arm in degrees
    float thetaRadian;                           // the angle of the tilting arm in radians
    int LEDPin = 9;                              // Assign LEDPin on the Nano to a pin number

    float current_height = 0.0;

    float ARM_LENGTH = 82;
    float PIVOT_HEIGHT = 100;
  private:

};
#endif