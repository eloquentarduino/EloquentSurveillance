#include <SPIFFS.h>
#include "EloquentSurveillance.h"


#define WIFI_SSID "Abc"
#define WIFI_PASS "12345678"


EloquentSurveillance::Motion motion;
EloquentSurveillance::FileServer fileServer(80);


/**
 *
 */
void setup() {
    Serial.begin(115200);
    delay(3000);
    debug("INFO", "Init");

    // configure camera
    camera.m5wide();
    camera.vga();
    camera.highQuality();

    // configure motion detection
    // (see MotionDetectionExample for more info)
    motion.setMinChanges(0.1);
    motion.setMinPixelDiff(8);
    motion.setMinSizeDiff(0.03);

    while (!camera.begin())
        debug("ERROR", camera.getErrorMessage());

    while (!fileServer.beginAsClient(WIFI_SSID, WIFI_PASS))
        debug("ERROR", fileServer.getErrorMessage());

    while (!SPIFFS.begin(true))
        debug("ERROR", "Cannot init SPIFFS");

    debug("SUCCESS", fileServer.getWelcomeMessage());
}

/**
 *
 */
void loop() {
    fileServer.handle();

    if (!camera.capture()) {
        debug("ERROR", camera.getErrorMessage());
        return;
    }

    if (motion.update()) {
        String filename = String("/capture") + motion.getCount() + ".jpg";

        debug("INFO", String("Motion detected in ") + motion.getExecutionTimeInMicros() + " us");
        debug("INFO", String("Saving to file: SPIFFS") + filename);
        camera.saveTo(SPIFFS, filename);
        debug("INFO", "Refresh the file server to see the file");
    }
    else if (!motion.isOk()) {
        debug("ERROR", motion.getErrorMessage());
    }
}
