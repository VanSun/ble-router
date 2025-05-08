#include "config.h"

BeaconData beacons[MAX_DEVICES];
SemaphoreHandle_t dataMutex;
BLEScan* pBLEScan;

void printResetReason() {
  esp_reset_reason_t reason = esp_reset_reason();
  Serial.print("Reset reason: ");
  switch(reason) {
    case ESP_RST_POWERON:    Serial.println("Power-on"); break;
    case ESP_RST_SW:         Serial.println("Software"); break;
    case ESP_RST_PANIC:      Serial.println("Exception/Panic"); break;
    case ESP_RST_INT_WDT:    Serial.println("Interrupt Watchdog"); break;
    case ESP_RST_TASK_WDT:   Serial.println("Task Watchdog"); break;
    case ESP_RST_WDT:        Serial.println("Other Watchdog"); break;
    case ESP_RST_DEEPSLEEP:  Serial.println("Deep Sleep"); break;
    case ESP_RST_BROWNOUT:   Serial.println("Brownout"); break;
    default:                 Serial.println("Unknown"); break;
  }
}