#define VERBOSE
#include "EloquentSurveillance.h"

/**
 * Replace with your WiFi credentials
 */
#define WIFI_SSID "Abc"
#define WIFI_PASS "12345678"


EloquentSurveillance::StreamServer streamServer(80);


void setup() {
    Serial.begin(115200);
    delay(3000);
    debug("INFO", "Init");

    /**
     * See CameraCaptureExample for more details
     */
    camera.m5wide();
    camera.highQuality();
    camera.qvga();

    /**
     * Initialize the camera
     * If something goes wrong, print the error message
     */
    while (!camera.begin())
        debug("ERROR", camera.getErrorMessage());

    /**
     * Connect to WiFi
     * If something goes wrong, print the error message
     */
    while (!wifi.connect(WIFI_SSID, WIFI_PASS))
        debug("ERROR", wifi.getErrorMessage());

    /**
     * Initialize stream web server
     * If something goes wrong, print the error message
     */
    while (!streamServer.begin())
        debug("ERROR", streamServer.getErrorMessage());

    /**
     * Display address of stream server
     */
    debug("SUCCESS", streamServer.getWelcomeMessage());
}


void loop() {
}
