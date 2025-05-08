#pragma once
#include "config.h"
#include <lwip/sys.h>

void initEthernet();
void safeSetupWatchdog();
void tcpipLock();
void tcpipUnlock();