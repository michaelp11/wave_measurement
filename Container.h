#ifndef CONTAINER_H
#define CONTAINER_H

#include <Arduino.h>
#include "Measurement.h"
#include "MeasurementService.h"
#include "HeightManager.h"
#include "Webserver.h"

class Container{
  public:
    Container();
    Measurement currentMeasurement;
    MeasurementService* measurementService;
    HeightManager* heightManager;
    Webserver* webserver;

};
#endif