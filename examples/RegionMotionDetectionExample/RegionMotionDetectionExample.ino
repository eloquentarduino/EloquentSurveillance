// turn on debug messages
#define VERBOSE
#include "EloquentSurveillance.h"


/**
 * Instantiate motion detector
 */
EloquentSurveillance::Motion motion;

/**
 * Create region of interest
 */
EloquentSurveillance::Region region = {
        .x = 100,
        .y = 100,
        .width = 50,
        .height = 240,
        // inherit from motion configuration
        .numChanges = 0
};


/**
 *
 */
void setup() {
    Serial.begin(115200);
    delay(3000);
    debug("INFO", "Init");

    /**
     * See CameraCaptureExample for more details
     */
    camera.m5wide();
    camera.qvga();
    camera.highQuality();

    /**
     * See MotionDetectionExample for more details
     */
    motion.setMinChanges(0.1);
    motion.setMinPixelDiff(10);
    motion.setMinSizeDiff(0.05);
    motion.debounce(10000L);

    /**
     * Initialize the camera
     * If something goes wrong, print the error message
     */
    while (!camera.begin())
        debug("ERROR", camera.getErrorMessage());

    debug("SUCCESS", "Camera OK");
}

/**
 *
 */
void loop() {
    /**
     * Try to capture a frame
     * If something goes wrong, print the error message
     */
    if (!camera.capture()) {
        debug("ERROR", camera.getErrorMessage());
        return;
    }

    /**
     * Look for motion in Region of Interest.
     */
    if (!motion.update())
        return;

    if (motion.detect(region)) {
        debug("INFO", "Motion detected in ROI");
        delay(5000);
    }

    if (!motion.isOk()) {
        debug("ERROR", motion.getErrorMessage());
    }
}
