#include "prometheus_server.h"
#include "eth_manager.h"

String prometheusMetrics() {
    String output;
    tcpipLock();
    
    output += "# HELP beacon_rssi Last RSSI value of iBeacon\n";
    output += "# TYPE beacon_rssi gauge\n";
    
    if(xSemaphoreTake(dataMutex, pdMS_TO_TICKS(1000))) {
        for(int i = 0; i < MAX_DEVICES; i++) {
            if(beacons[i].last_seen > 0) {
                output += "beacon_rssi{uuid=\"" + String(beacons[i].uuid) 
                        + "\",major=" + String(beacons[i].major)
                        + ",minor=" + String(beacons[i].minor)
                        + "} " + String(beacons[i].rssi) + "\n";
            }
        }
        xSemaphoreGive(dataMutex);
    }
    
    output += "# HELP eth_connected Ethernet status\n";
    output += "eth_connected " + String(ETH.linkUp() ? "1" : "0") + "\n";
    
    output += "# HELP system_uptime System uptime\n";
    output += "system_uptime " + String(millis()/1000) + "\n";
    
    tcpipUnlock();
    return output;
}

void setupPrometheusServer() {
    server.on("/metrics", HTTP_GET, [](AsyncWebServerRequest *request){
        tcpipLock();
        if(!ETH.linkUp()) {
            tcpipUnlock();
            request->send(503, "text/plain", "Ethernet disconnected");
            return;
        }
        
        String metrics = prometheusMetrics();
        tcpipUnlock();
        
        request->send(200, "text/plain", metrics);
    });
    
    server.begin();
}