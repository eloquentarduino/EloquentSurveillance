#define VERBOSE
#include "EloquentSurveillance.h"


EloquentSurveillance::Motion motion;


/**
 *
 */
void setup() {
    Serial.begin(115200);
    delay(3000);
    debug("INFO", "Init");

    // configure camera
    camera.m5wide();
    camera.qvga();
    camera.highQuality();

    // configure motion detection
    //  > setMinChanges accepts a number from 0 to 1 (percent) or an integer
    //    At least the given number of pixels must change from one frame to the next
    //    to trigger the motion
    motion.setMinChanges(0.1);
    //  > setMinPixelDiff accepts an integer
    //    Each pixel value must differ at least of the given amount from one frame to the next
    //    to trigger the motion
    motion.setMinPixelDiff(10);
    //  > setMinSizeDiff accepts a number from 0 to 1 (percent) or an integer
    //    The filesize of one frame from the next must change of at least the given amount
    //    to check for motion
    motion.setMinSizeDiff(0.05);

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

    if (motion.update()) {
        debug("INFO", String("Motion detected in ") + motion.getExecutionTimeInMicros() + " us");
    }
    else if (!motion.isOk()) {
        debug("ERROR", motion.getErrorMessage());
    }
}
