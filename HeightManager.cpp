#include <Arduino.h>
#include "HeightManager.h"
#include <Arduino_LSM6DSOX.h>

#define SECOND 1000.0
#define DELAY_MS 40.0
#define TIMER_FACTOR 25
#define MINUTE_TO_SECOND 60

#define g 9.81

HeightManager::HeightManager(){
  this->setup();
}

void HeightManager::setup(){
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.print("Gyroscopic sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
}

void HeightManager::captureHeight(){
  float x, y, z;
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(x, y, z);

    float theta=-atan2(x/g,z/g)/2/3.141592654*360;
    float phi=-atan2(y/g,z/g)/2/3.141592654*360;

    int i = 0;
    while (i < SAMPLE_SIZE-1){
      this->measurements[i] = this->measurements[i+1];
      this->height[i]=this->height[i+1];
      i++;
    }
    this->measurements[SAMPLE_SIZE-1] = theta;
    

    this->current_height = this->calculateHeight(this->getLastMeasurement());
    this->height[SAMPLE_SIZE-1] = this->current_height;
  }
}

float HeightManager::calculateHeight(float theta){
  this->armEndHeight = PIVOT_HEIGHT - (sin(theta/57.2958) * ARM_LENGTH); 
  //Serial.println(theta);
  //Serial.print("Arm angle (degree): ");Serial.println(theta);
  //Serial.print("Height (cm): ");Serial.println(armEndHeight);
  return armEndHeight;
}

float HeightManager::getLastMeasurement(){
  int i = 0;
  float avg = 0;
  while (i < SAMPLE_SIZE){
    avg += this->measurements[i];
    i++;
  }
  avg = avg/SAMPLE_SIZE;
  return avg;
}