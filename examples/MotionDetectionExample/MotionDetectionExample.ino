// turn on debug messages
#define VERBOSE
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
     * Configure motion detection
     *
     * > setMinChanges() accepts a number from 0 to 1 (percent) or an integer
     *   At least the given number of pixels must change from one frame to the next
     *   to trigger the motion.
     *   The following line translates to "Trigger motion if at least 10% of the pixels
     *   in the image changed value"
     */
    motion.setMinChanges(0.1);

    /**
     * > setMinPixelDiff() accepts an integer
     *   Each pixel value must differ at least of the given amount from one frame to the next
     *   to be considered as different.
     *   The following line translates to "Consider a pixel as changed if its value increased
     *   or decreased by 10 (out of 255)"
     */
    motion.setMinPixelDiff(10);

    /**
     * > setMinSizeDiff() accepts a number from 0 to 1 (percent) or an integer
     *   To speed up the detection, you can exit early if the image size is almost the same.
     *   This is an heuristic that says: "If two consecutive frames have a similar size, they
     *   probably have the same contents". This is by no means guaranteed, but can dramatically
     *   reduce the computation cost.
     *   The following line translates to "Check for motion if the filesize of the current image
     *   differs by more than 5% from the previous".
     *
     *   If you don't feel like this heuristic works for you, delete this line.
     */
    motion.setMinSizeDiff(0.05);

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
     * Look for motion.
     * In the `true` branch, you can handle a motion event.
     * For the moment, just print the processing time for motion detection.
     */
    if (motion.update()) {
        debug("INFO", String("Motion detected in ") + motion.getExecutionTimeInMicros() + " us");
        delay(5000);
    }
    else if (!motion.isOk()) {
        /**
         * Something went wrong.
         * This part requires proper handling if you want to integrate it in your project
         * because you can reach this point for a number of reason.
         * For the moment, just print the error message
         */
        debug("ERROR", motion.getErrorMessage());
    }
}
