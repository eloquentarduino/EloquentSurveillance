#define VERBOSE
#include "EloquentSurveillance.h"


#define WIFI_SSID "Abc"
#define WIFI_PASS "12345678"


EloquentSurveillance::WebServer webServer(80);


void setup() {
    Serial.begin(115200);
    delay(3000);
    debug("INFO", "Init");

    camera.m5wide();
    camera.highQuality();
    camera.qvga();

    while (!camera.begin())
        debug("ERROR", camera.getErrorMessage());

    while (!webServer.beginAsClient(WIFI_SSID, WIFI_PASS))
        debug("ERROR", webServer.getErrorMessage());

    debug("SUCCESS", webServer.getWelcomeMessage());
}


void loop() {
}
