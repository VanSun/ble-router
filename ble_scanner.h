#pragma once
#include "config.h"

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
public:
  void onResult(BLEAdvertisedDevice advertisedDevice);
};

void setupBLEScanner();
void handleBLEScan();