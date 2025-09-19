#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h>
#include "Measurement.h"
#include "MeasurementService.h"
#include "Webserver.h"
#include "HeightManager.h"
#include "Container.h"
#define SECOND 1000.0
#define DELAY_MS 20.0
#define TIMER_FACTOR 50
#define THRESHOLD 1
#define g 9.81
#define SAMPLE_SIZE 50
#define ssid "HOEHENMESSER4"
#define password "12345678"

#define MINUTE_TO_SECOND 60


// Measurement class to representate a single Measurement over ~MM_TIME seconds

float slice = DELAY_MS/SECOND;





//software timer stuff flags to determine whether to measure or not;
//we don´t use a hardware timer because this shit is far more complicated and the board has the computing power to handle this
#define STATE_MEASURING true
#define STATE_IDLE false
bool STATE;
unsigned int TIMER;
unsigned int TIMER_THRESHOLD_MEASURE_START;
unsigned int TIMER_THRESHOLD_MEASURE_END;
bool STATE_CHANGED = false;

//Stuff for determining if currently going upwards or downwards
#define MOVING_UPWARDS true
#define MOVING_DOWNWARDS false

bool MOVEMENT_DIRECTION;
int different_movements = 0;




Container* hoehenmesser;

void setup(void) 
{
  //Inititalize Serial
  Serial.begin(9600);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  hoehenmesser = new Container();
  hoehenmesser->heightManager = new HeightManager();
  
  hoehenmesser->measurementService = new MeasurementService();
  hoehenmesser->webserver = new Webserver(hoehenmesser->heightManager, hoehenmesser->measurementService);

  TIMER = 0;
  MOVEMENT_DIRECTION = MOVING_DOWNWARDS;
  hoehenmesser->measurementService->refreshTimerThresholds();
}

void loop() {
  hoehenmesser->heightManager->captureHeight();
  updateTimer();
  if(STATE==STATE_MEASURING){

  }
  hoehenmesser->webserver->handleOpenClients();
  updateMovementDirection();
  delay(DELAY_MS);
}

void updateTimer(){
  TIMER++;
  if (STATE==STATE_IDLE) {
    if(TIMER == hoehenmesser->measurementService->TIMER_THRESHOLD_MEASURE_START){
      TIMER = 0;
      STATE = STATE_MEASURING;
      STATE_CHANGED = true;
    }
  }
  else if (STATE == STATE_MEASURING) {
    if(TIMER == hoehenmesser->measurementService->TIMER_THRESHOLD_MEASURE_END) {
      hoehenmesser->measurementService->print();
      hoehenmesser->measurementService->finishMeasurement();
      TIMER = 0;
      STATE = STATE_IDLE;
      STATE_CHANGED = true;
    }
  }
}

void updateMovementDirection(){
  int skips = 0;
  int array_pointer = SAMPLE_SIZE-1;
  float reference_value = (hoehenmesser->heightManager->height[array_pointer]);
  if(MOVEMENT_DIRECTION == MOVING_DOWNWARDS){
    for(int i = 1; i<= hoehenmesser->measurementService->MOVEMENT_CHANGE_THRESHOLD;i++){
      array_pointer--;
      if((hoehenmesser->heightManager->height[array_pointer])>reference_value){
        //early out because the measurements were not clearly continiously upwards
        skips++;
        if(skips > hoehenmesser->measurementService->ALLOWED_SKIPS){
          return;
        }
      }
      reference_value = hoehenmesser->heightManager->height[array_pointer];
    }
    //Serial.println("UPWARDS!");
    MOVEMENT_DIRECTION = MOVING_UPWARDS;
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    handleExtrema();
    return;
  }
  else if(MOVEMENT_DIRECTION == MOVING_UPWARDS){
    for(int i = 1; i<= hoehenmesser->measurementService->MOVEMENT_CHANGE_THRESHOLD;i++){
      Serial.println((hoehenmesser->heightManager->height[array_pointer]));
      array_pointer--;
      if((hoehenmesser->heightManager->height[array_pointer])<reference_value){
        //early out because the measurements were not clearly continiously downwards
        skips++;
        if(skips > hoehenmesser->measurementService->ALLOWED_SKIPS){
          return;
        }
      }
      reference_value = hoehenmesser->heightManager->height[array_pointer];
    }
    //Serial.println("DOWNWARDS");
    MOVEMENT_DIRECTION = MOVING_DOWNWARDS;
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, HIGH);
    handleExtrema();
  }
}

void handleExtrema(){
  if (MOVEMENT_DIRECTION == MOVING_DOWNWARDS){
    //We need to find a local maximum
    //The safe space to do so is to search in 2*MOVEMENT_CHANGE_THRESHOLD
    //ugly but works
    float max = -999.9;
    for(int i = 0; i<(2*hoehenmesser->measurementService->MOVEMENT_CHANGE_THRESHOLD);i++){
      if(hoehenmesser->heightManager->height[SAMPLE_SIZE-1-i]>max){
        max = hoehenmesser->heightManager->height[SAMPLE_SIZE-1-i];
      }
    }
    Serial.println("MAXIMUM");
    Serial.print(max); Serial.println(" cm");
    if(STATE == STATE_MEASURING){
      hoehenmesser->measurementService->addMaxima(max);
    }
  }
  if (MOVEMENT_DIRECTION == MOVING_UPWARDS){
    //We need to find a local minimum
    //The safe space to do so is to search in 2*MOVEMENT_CHANGE_THRESHOLD
    //ugly but works
    float min = 999.9;
    for(int i = 0; i<(2*hoehenmesser->measurementService->MOVEMENT_CHANGE_THRESHOLD);i++){
      if(hoehenmesser->heightManager->height[SAMPLE_SIZE-1-i]<min){
        min = hoehenmesser->heightManager->height[SAMPLE_SIZE-1-i];
      }
    }
    Serial.println("MINIMUM");
    Serial.print(min); Serial.println(" cm");
    if(STATE == STATE_MEASURING){
      hoehenmesser->measurementService->addMinima(min);
    }
  }
}

void storeMeasurement(int time,float height){

}

void react(String currentLine){
  
}


