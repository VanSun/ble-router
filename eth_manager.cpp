#include "eth_manager.h"
#include <lwip/sys.h>

static bool eth_initialized = false;
static sys_mutex_t tcpip_mutex;

void initEthernet() {
  if(eth_initialized) return;
  
  if(sys_mutex_new(&tcpip_mutex) != ERR_OK) {
    Serial.println("[ETH] Mutex init failed! Rebooting...");
    ESP.restart();
  }

  SPI.begin(ETH_SPI_CLK, ETH_SPI_MISO, ETH_SPI_MOSI, ETH_SPI_CS);
  delay(1000);

  sys_mutex_lock(&tcpip_mutex);
  bool eth_init_result = ETH.begin(
    ETH_PHY_W5500, 
    0, 
    ETH_SPI_CS, 
    ETH_INT, 
    ETH_RST, 
    SPI
  );
  sys_mutex_unlock(&tcpip_mutex);

  if(!eth_init_result) {
    Serial.println("[ETH] Initialization failed! Rebooting...");
    ESP.restart();
  }

  uint32_t timeout = millis() + 20000;
  while(!ETH.linkUp()) {
    if(millis() > timeout) {
      Serial.println("[ETH] Cable not connected! Rebooting...");
      ESP.restart();
    }
    delay(500);
  }

  WiFi.mode(WIFI_MODE_NULL);
  eth_initialized = true;
  Serial.println("[ETH] Initialization successful");
}

void safeSetupWatchdog() {
    static bool initialized = false;
    if(initialized) return;
    
    // Конфигурация Watchdog только для главного ядра
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = 10000,
        .idle_core_mask = 0, // Только CPU0
        .trigger_panic = true
    };
    
    esp_err_t ret = esp_task_wdt_init(&wdt_config);
    if(ret != ESP_OK) {
        Serial.printf("[WDT] Init error: 0x%x. Rebooting...\n", ret);
        ESP.restart();
    }

    ret = esp_task_wdt_add(NULL);
    if(ret != ESP_OK) {
        Serial.printf("[WDT] Add error: 0x%x. Rebooting...\n", ret);
        ESP.restart();
    }
    
    initialized = true;
    Serial.println("[WDT] Initialized");
}

void tcpipLock() {
  sys_mutex_lock(&tcpip_mutex);
}

void tcpipUnlock() {
  sys_mutex_unlock(&tcpip_mutex);
}