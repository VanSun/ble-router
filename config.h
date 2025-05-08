#pragma once

#include <Arduino.h>
#include <ETH.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <esp_task_wdt.h>

#define PROMETHEUS_PORT 5959
#define MAX_DEVICES 20
#define SCAN_DURATION 5
#define SCAN_INTERVAL 10
#define WDT_TIMEOUT_SEC 5

#define ETH_SPI_CLK 13
#define ETH_SPI_MOSI 11
#define ETH_SPI_MISO 12
#define ETH_SPI_CS 14
#define ETH_RST 9
#define ETH_INT 10

extern AsyncWebServer server;

typedef struct {
  char uuid[37];
  uint16_t major;
  uint16_t minor;
  int rssi;
  unsigned long last_seen;
} BeaconData;

extern BeaconData beacons[MAX_DEVICES];
extern SemaphoreHandle_t dataMutex;
extern BLEScan* pBLEScan;

void printResetReason();