#include "Webserver.h"
#include "MeasurementService.h"
#include <Arduino.h>
#include <WiFiNINA.h>
#define ssid "HOEHENMESSER2"
#define password "12345678" // ¯\_(ツ)_/¯
#define IP IPAddress(192,168,2,1)


Webserver::Webserver(HeightManager* heightManager, MeasurementService* measurementService){
  this->heightManager = heightManager;
  this->measurementService = measurementService;
  this->initWLAN();
}

void Webserver::initWLAN(){
  if (WiFi.status() == WL_NO_MODULE) {
    //Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  //Serial.print("Creating access point named: ");
 
  //Serial.println(ssid);
  wlan_status = WiFi.beginAP(ssid,password);
  WiFi.config(IP);
  server->begin();
}

void Webserver::handleOpenClients(){
  checkWLANStatus();
  if (this->client = this->server->available()) {
    String currentLine = "";
    while(this->client.available()){
        char c = this->client.read();
        currentLine += c;
        if(c=='\n'){
          break;
        }
    }
    this->handleInput(currentLine);
    this->createResponse();

    this->client.stop();
  }
}

void Webserver::handleInput(String currentLine){
  if (currentLine.startsWith("GET /set_length")) {
    currentLine.replace("GET /set_length?length=","");
    currentLine.replace(" HTTP/1.1","");
    this->heightManager->ARM_LENGTH = currentLine.toFloat();      
  }

  if (currentLine.startsWith("GET /set_heigth")) {
    currentLine.replace("GET /set_heigth?heigth=","");
    currentLine.replace(" HTTP/1.1","");
    this->heightManager->PIVOT_HEIGHT = currentLine.toFloat();      
  }

  if (currentLine.startsWith("GET /set_pause_time")) {
    currentLine.replace("GET /set_pause_time?pause_time=","");
    currentLine.replace(" HTTP/1.1","");
    this->measurementService->MEASURE_EVERY_X_MINUTES = currentLine.toFloat();      
    this->measurementService->refreshTimerThresholds();
  }

  if (currentLine.startsWith("GET /set_measurement_duration")) {
    currentLine.replace("GET /set_measurement_duration?measurement_duration=","");
    currentLine.replace(" HTTP/1.1","");
    this->measurementService->MEASURE_FOR_X_MINUTES = currentLine.toFloat();   
    this->measurementService->refreshTimerThresholds();   
  }
  if (currentLine.startsWith("GET /set_movement_threshold")) {
    currentLine.replace("GET /set_movement_threshold?movement_threshold=","");
    currentLine.replace(" HTTP/1.1","");
    this->measurementService->MOVEMENT_CHANGE_THRESHOLD = currentLine.toFloat();      
  }
  if (currentLine.startsWith("GET /set_allowed_skips")) {
    currentLine.replace("GET /set_allowed_skips?allowed_skips=","");
    currentLine.replace(" HTTP/1.1","");
    this->measurementService->ALLOWED_SKIPS = currentLine.toFloat();      
  }
}

// You know it's "not pretty", but it works and every idiot gets what happens here. So i guess i just print it line by line. Sry.
void Webserver::createResponse(){
  this->client.println("HTTP/1.1 200 OK");
  this->client.println("Content-type:text/html");
  this->client.println();
  // the content of the HTTP response follows the header:
  this->client.print("<style>");
  this->client.print(".container {margin: 0 auto; text-align: center; margin-top: 100px;}");
  this->client.print("button {color: white; width: 100px; height: 100px;");
  this->client.print("border-radius: 10%; margin: 20px; border: none; font-size: 20px; outline: none; transition: all 0.2s;}");
  this->client.print(".red{background-color: rgb(196, 39, 39);}");
  this->client.print(".green{background-color: rgb(39, 121, 39);}");
  this->client.print(".blue {background-color: rgb(5, 87, 180);}");
  this->client.print(".off{background-color: grey;}");
  this->client.print("button:hover{cursor: pointer; opacity: 0.7;}");
  this->client.print("</style>");
  this->client.print("<div class='container'>");
  this->client.print("<div class='centered'><table><thead>");
  this->client.print("<tr>");
  this->client.print("<td> Zeitpunkt </td>");
  this->client.print("<td> Höhe </td>");
  this->client.print("</tr>");
  this->client.print("</thead><tbody>");
  this->client.print("<tr>");
  this->client.print("<td> jetzt </td>");
  this->client.print("<td> ");
  this->client.print(this->heightManager->current_height);
  this->client.print(" cm </td>");
  this->client.print("</tr>");
  this->client.print("</tbody></table></div>");
  this->client.print("<input id='arm_length' type='text'/>");
  this->client.print("<button class='off' type='submit' onmousedown='location.href=\"/set_length?length=\"+document.getElementById(\"arm_length\").value'>Length</button><br>");
  this->client.print("<input id='pivot_height' type='text'/>");
  this->client.print("<button class='off' type='submit' onmousedown='location.href=\"/set_heigth?heigth=\"+document.getElementById(\"pivot_height\").value'>heigth</button><br>");
  this->client.print("<input id='pause_time' type='text'/>");
  this->client.print("<button class='off' type='submit' onmousedown='location.href=\"/set_pause_time?pause_time=\"+document.getElementById(\"pause_time\").value'>Pause Time</button><br>");
  this->client.print("<input id='measurement_duration' type='text'/>");
  this->client.print("<button class='off' type='submit' onmousedown='location.href=\"/set_measurement_duration?measurement_duration=\"+document.getElementById(\"measurement_duration\").value'>Measurement Duration</button><br>");
  this->client.print("<input id='movement_threshold' type='text'/>");
  this->client.print("<button class='off' type='submit' onmousedown='location.href=\"/set_movement_threshold?movement_threshold=\"+document.getElementById(\"movement_threshold\").value'>Movement change threshold</button><br>");
  this->client.print("<input id='allowed_skips' type='text'/>");
  this->client.print("<button class='off' type='submit' onmousedown='location.href=\"/set_allowed_skips?allowed_skips=\"+document.getElementById(\"allowed_skips\").value'>Allowed skips</button><br>");
  this->client.print("<div class='centered'><table><thead>");
  this->client.print("<tr>");
  this->client.print("<td> Length of the arm </td>");
  this->client.print("<td> Height of the pivot</td>");
  this->client.print("<td> Pause time</td>");
  this->client.print("<td> Measurement duration</td>");
  this->client.print("<td> Movement change threshold</td>");
  this->client.print("<td> Allowed skips</td>");
  this->client.print("</tr>");
  this->client.print("</thead><tbody>");
  this->client.print("<tr>");
  this->client.print("<td>");
  this->client.print(this->heightManager->ARM_LENGTH);
  this->client.print("</td>");
  this->client.print("<td> ");
  this->client.print(this->heightManager->PIVOT_HEIGHT);
  this->client.print(" cm </td>");
  this->client.print("<td> ");
  this->client.print(this->measurementService->MEASURE_EVERY_X_MINUTES);
  this->client.print(" min. </td>");
  this->client.print("<td> ");
  this->client.print(this->measurementService->MEASURE_FOR_X_MINUTES);
  this->client.print(" min. </td>");
    this->client.print("<td> ");
  this->client.print(this->measurementService->MOVEMENT_CHANGE_THRESHOLD);
  this->client.print("  </td>");
    this->client.print("<td> ");
  this->client.print(this->measurementService->ALLOWED_SKIPS);
  this->client.print("  </td>");
  this->client.print("</tr>");
  this->client.print("</tbody></table></div>");
  this->printMeasurements();
  this->client.print("</div>");

  this->client.println();
}

void Webserver::printMeasurements(){
  this->client.print("<div class='centered'><table><thead>");
  this->client.print("<tr>");
  this->client.print("<td> Maximum height</td>");
  this->client.print("<td> Minimum height</td>");
  this->client.print("<td> Maximum amplitude</td>");
  this->client.print("<td> Minimal amplitude</td>");
  this->client.print("<td> Average amplitude</td>");
  this->client.print("<td> 1/Frequency (half T) in ms</td>");
  this->client.print("<td> Waves</td>");
  this->client.print("<td> Time after start</td>");

  this->client.print("</tr>");
  this->client.print("</thead><tbody>");
  
  for ( auto i = this->measurementService->measurements.begin(); i != this->measurementService->measurements.end(); i++ ) {
    this->client.print("<tr>");
    this->client.print("<td>");
    this->client.print(i->maximum);
    this->client.print("</td>");
    this->client.print("<td> ");
    this->client.print(i->minimum);
    this->client.print(" cm </td>");
    this->client.print("<td> ");
    this->client.print(i->max_amplitude);
    this->client.print(" cm </td>");
    this->client.print("<td> ");
    this->client.print(i->min_amplitude);
    this->client.print(" cm </td>");
    this->client.print("<td> ");
    this->client.print(i->average_amplitude);
    this->client.print(" cm </td>");
    this->client.print("<td> ");
    this->client.print(i->frequency);
    this->client.print(" ms </td>");
    this->client.print("<td> ");
    this->client.print(i->waves);
    this->client.print(" </td>");
    this->client.print("<td> ");
    this->client.print(float(i->time)/1000/60/60);
    this->client.print(" h </td>");
    this->client.print("</tr>");
  }
  
  this->client.print("</tbody></table></div>");
}

void Webserver::checkWLANStatus(){
  if (this->wlan_status != WiFi.status()) {
    // it has changed update the variable
    this->wlan_status = WiFi.status();

    if (this->wlan_status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      //Serial.println("Device connected to AP");
      this->connected = true;
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      //Serial.println("Device disconnected from AP");
      client.stop();
      this->client = NULL;
    }
  }
}