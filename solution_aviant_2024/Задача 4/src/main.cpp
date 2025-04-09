#include <Arduino.h>
#include "UAViant_client/UAViantSystemClient.h"
#include "Tasks/Task4.h"

// Класс для отладки с симулятором
static UAViantSystemClient *_UAViant_client;
// Класс - реализация задачи команды
static Task4_solution _team4;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5);
  // Не забываем инитить
  _team4.init(nullptr);
  _UAViant_client = new UAViantSystemClient(&Serial, &_team4);
}


void loop() {
  // put your main code here, to run repeatedly:
_UAViant_client->UAViant_serialEvent();
}

