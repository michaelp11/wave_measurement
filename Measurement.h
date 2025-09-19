#ifndef Measurement_h
#define Measurement_h
#include <Arduino.h>
class Measurement{

public:
  Measurement(){};
  String time_str;
  float maximum;
  float minimum;
  float average_height;
  float average_amplitude;
  float max_amplitude;
  float min_amplitude;
  float frequency;
  float max_frequency;
  float min_frequency;
  int waves;
  int time;
private: 
};
#endif
