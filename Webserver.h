#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <WiFiNINA.h>
#include "HeightManager.h"
#include "MeasurementService.h"
class Webserver{
public:
  Webserver(HeightManager* heightManager, MeasurementService* measurement);
  void handleOpenClients();

private:
  void initWLAN();
  void createResponse();
  void handleInput(String input);
  void checkWLANStatus();
  void printMeasurements();
  int wlan_status;
  HeightManager* heightManager;
  MeasurementService* measurementService;
  WiFiClient client;
  bool connected;
  WiFiServer* server = new WiFiServer(80);
};
#endif 