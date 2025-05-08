#include "config.h"
#include "eth_manager.h"
#include "ble_scanner.h"
#include "prometheus_server.h"

AsyncWebServer server(PROMETHEUS_PORT);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  safeSetupWatchdog();
  initEthernet();
  
  dataMutex = xSemaphoreCreateMutex();
  setupBLEScanner();
  setupPrometheusServer();

  memset(beacons, 0, sizeof(beacons));
  ETH.setHostname("esp32-prometheus");
  
  Serial.println("\n\n=== System Ready ===");
}

void loop() {
  static unsigned long last_wdt_reset = 0;
  
  if(millis() - last_wdt_reset > 500) {
    esp_task_wdt_reset();
    last_wdt_reset = millis();
  }

  handleBLEScan();
  delay(10);
}
