#define VERBOSE
#include "EloquentSurveillance.h"


/**
 *
 */
void setup() {
    Serial.begin(115200);
    delay(3000);
    debug("INFO", "Init");

    camera.m5wide();
    camera.qvga();
    camera.highQuality();

    while (!camera.begin())
        debug("ERROR", camera.getErrorMessage());

    debug("SUCCESS", "Camera OK");
}

/**
 *
 */
void loop() {
    if (!camera.capture()) {
        debug("ERROR", camera.getErrorMessage());
        return;
    }

    debug("SUCCCESS", camera.getFileSize());
}
