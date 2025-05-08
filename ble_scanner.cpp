#include "ble_scanner.h"

namespace {
  bool is_scanning = false; // Локальная переменная внутри анонимного пространства имен
}

void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {
  if(advertisedDevice.haveManufacturerData() && 
     advertisedDevice.getManufacturerData().length() >= 25) {
    
    String rawData = advertisedDevice.getManufacturerData();
    if(rawData.substring(0, 4) == "4C000215") {
      
      String uuid = rawData.substring(4, 36);
      uuid = uuid.substring(0,8) + "-" 
           + uuid.substring(8,12) + "-"
           + uuid.substring(12,16) + "-"
           + uuid.substring(16,20) + "-"
           + uuid.substring(20,32);

      uint16_t major = (rawData.charAt(36) << 8) | rawData.charAt(37);
      uint16_t minor = (rawData.charAt(38) << 8) | rawData.charAt(39);
      int rssi = advertisedDevice.getRSSI();

      xSemaphoreTake(dataMutex, portMAX_DELAY);
      // ... (логика обработки маяков)
      xSemaphoreGive(dataMutex);
    }
  }
}

void setupBLEScanner() {
  BLEDevice::init("Prometheus-Scanner");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  Serial.println("[BLE] Scanner initialized");
}

void handleBLEScan() {
  static unsigned long lastScan = 0;
  if(millis() - lastScan >= SCAN_INTERVAL * 1000) {
    if(!ETH.linkUp()) {
      Serial.println("[BLE] Scan skipped - Ethernet down");
      lastScan = millis();
      return;
    }
    
    if(is_scanning) {
      pBLEScan->stop();
      delay(100);
      is_scanning = false;
    }
    
    is_scanning = true;
    BLEScanResults* results = pBLEScan->start(SCAN_DURATION, false);
    
    if(results) {
      uint16_t count = results->getCount();
      Serial.printf("[BLE] Found %d devices\n", count);
      if(count > 0) pBLEScan->clearResults();
    }
    
    is_scanning = false;
  }
}