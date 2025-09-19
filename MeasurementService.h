#ifndef MEASUREMENTSERVICE_H
#define MEASUREMENTSERVICE_H

#include <Arduino.h>
#include "Measurement.h"
#include <vector>

#define TIMER_FACTOR 50
#define MINUTE_TO_SECOND 60
class MeasurementService{
  public:
    int MEASURE_EVERY_X_MINUTES = 1;
    int MEASURE_FOR_X_MINUTES = 1;
    int TIMER_THRESHOLD_MEASURE_START;
    int TIMER_THRESHOLD_MEASURE_END;
    int MOVEMENT_CHANGE_THRESHOLD =12;
    int ALLOWED_SKIPS =3;

    void refreshTimerThresholds(){
      TIMER_THRESHOLD_MEASURE_START = MEASURE_EVERY_X_MINUTES * MINUTE_TO_SECOND * TIMER_FACTOR;
      TIMER_THRESHOLD_MEASURE_END = MEASURE_FOR_X_MINUTES * MINUTE_TO_SECOND * TIMER_FACTOR;
      Serial.println(TIMER_THRESHOLD_MEASURE_START);
      Serial.println(TIMER_THRESHOLD_MEASURE_END);
    }
    MeasurementService();
    std::vector<Measurement> measurements;
    void addValue();
    void addMeasurement(Measurement* measurement);
    void addMaxima(float height);
    void addMinima(float height);
    void finishMeasurement();
    void print();

  private:
    int last_millis = 0;
    float last_extrema = -9999;
    bool at_least_measured_once = false;
    bool next_one_counts = false;
    int wave_parts = 0;
    int measurements_so_far = 0;
    float total_max = -9999;
    float total_min = 9999;
    float average_amplitude = 0;
    float maximal_amplitude = 0;
    float minimal_amplitude = 9999;
    float frequency = 0;
};
#endif
