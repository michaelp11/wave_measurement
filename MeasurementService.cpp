#include "MeasurementService.h"
#include "Measurement.h"
#include <Arduino.h>
MeasurementService::MeasurementService(){
}

void MeasurementService::addMeasurement(Measurement* measurement){
  this->measurements.push_back(*measurement);
}

void MeasurementService::addMaxima(float value){
  int current_time = millis();
  if(this->at_least_measured_once){
      float difference = abs(this->last_extrema-value);

      if(difference > this->maximal_amplitude){
        this->maximal_amplitude = difference;
      }

      if(difference < this->minimal_amplitude){
        this->minimal_amplitude = difference;
      }
      float average = ((this->average_amplitude * float(this->measurements_so_far)) + difference) / (this->measurements_so_far+1);
      this->average_amplitude = average;
      if(value > this->total_max){
        this->total_max = value;
      }
      if(value < this->total_min){
        this->total_min = value;
      }
      float time_difference = current_time - this->last_millis;
      float frequency = (this->frequency * this->measurements_so_far + time_difference) / (this->measurements_so_far+1);
      this->frequency = frequency;
  }
  if(this->next_one_counts){
    this->measurements_so_far++;
  }
  this->at_least_measured_once = true;
  this->next_one_counts = !this->next_one_counts;
  this->last_millis = current_time;
  this->last_extrema = value;
  this->wave_parts++;
}

void MeasurementService::addMinima(float value){
  // seems confusing, but as maxima and minima are just alternating the logic behind both is actually identical. So just call the other function.
  this->addMaxima(value);
}

void MeasurementService::print(){
  //Serial.println(this->total_max);
  //Serial.println(this->total_min);
  //Serial.println(this->maximal_amplitude);
  //Serial.println(this->minimal_amplitude);
  //Serial.println(this->average_amplitude);
  //Serial.println(this->frequency);
  //Serial.println(this->measurements_so_far);
}

void MeasurementService::finishMeasurement(){
  Measurement* measurement = new Measurement();
  measurement->maximum = this->total_max;
  measurement->minimum = this->total_min;
  measurement->average_amplitude = this->average_amplitude;
  measurement->max_amplitude = this->maximal_amplitude;
  measurement->min_amplitude = this->minimal_amplitude;
  measurement->frequency = this->frequency;
  measurement->waves = this->measurements_so_far;
  measurement->time = millis();
  this->addMeasurement(measurement);

  this->wave_parts = 0;
  this->measurements_so_far = 0;
  this->at_least_measured_once = false;
  this->total_max = -9999;
  this->total_min = 9999;
  this->average_amplitude = 0;
  this->maximal_amplitude = 0;
  this->minimal_amplitude = 9999;
  this->frequency = 0;
}