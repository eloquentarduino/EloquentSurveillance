// turn on debug messages
#define VERBOSE
#include <SPIFFS.h>
#include "EloquentSurveillance.h"


/**
 * Instantiate motion detector
 */
EloquentSurveillance::Motion motion;


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

    /**
     * Initialize the filesystem
     * If something goes wrong, print an error message
     */
    while (!SPIFFS.begin(true))
        debug("ERROR", "Cannot init SPIFFS");

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
     * Look for motion.
     * In the `true` branch, you can handle a motion event.
     * In this case, we save the frame to disk
     */
    if (motion.update()) {
        /**
         * You can construct the filename as you prefer
         *  - motion.getCount() will return an incremental number *for the current run*
         *  - motion.getPersistenCount() will return an incremental number *that persists reboots*
         *  - motion.getNextFilename(String prefix) will construct a filename using the given prefix
         */
        //String filename = String("/capture_") + motion.getCount() + ".jpg";
        //String filename = String("/capture_") + motion.getPersistentCount() + ".jpg";
        String filename = motion.getNextFilename("/capture_");

        if (camera.saveTo(SPIFFS, filename)) {
            debug("SUCCESS", "Frame saved to disk");
            debug("SUCCESS", filename);
        }
        else {
            debug("ERROR", camera.getErrorMessage());
            debug("ERROR", filename);
        }

        delay(5000);
    }
}
