#include <Arduino.h>
#include "UAViant_client/UAViantSystemClient.h"
#include "Tasks/Task2.h"

// Класс для отладки с симулятором
static UAViantSystemClient *_UAViant_client;
// Класс - реализация задачи команды
static Task2_solution team;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5);
  // Не забываем инитить
  team.init(nullptr);
  _UAViant_client = new UAViantSystemClient(&Serial, &team);
}


void loop() {
  // put your main code here, to run repeatedly:
_UAViant_client->UAViant_serialEvent();
}
