#include <Arduino.h>
#include "UAViant_client/UAViantSystemClient.h"
#include "Tasks/Task3.h"


// Класс для отладки с симулятором
static UAViantSystemClient *_UAViant_client;
// Класс - реализация задачи команды
static Task3_Solution _team1;


void serialEvent(void){
_UAViant_client->UAViant_serialEvent();
}

void setup() {
  Serial.begin(115200);

  _team1.init_via_debug(&Serial2,&Serial);


  _UAViant_client = new UAViantSystemClient(&Serial,&_team1);

  // Настроим callbackи
  Serial.onReceive(serialEvent);

}


void loop() {
    // Обмен данными с стендом (Ardupilot)
    _team1.myloop();
}

