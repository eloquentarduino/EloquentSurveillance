// turn on debug messages
#define VERBOSE
#include "EloquentSurveillance.h"


/**
 *
 */
void setup() {
    Serial.begin(115200);
    delay(3000);

    // debug(LEVEl, message)
    debug("INFO", "Init");

    /**
     * Configure camera model
     * You have access to the global variable `camera`
     * Allowed values are:
     *  - aithinker()
     *  - m5()
     *  - m5wide()
     *  - eye()
     *  - wrover()
     */
    camera.m5wide();

    /**
     * Configure camera resolution
     * Allowed values are:
     * - _96x96()
     * - qqvga()
     * - qcif()
     * - hqvga()
     * - _240x240()
     * - qvga()
     * - cif()
     * - hvga()
     * - vga()
     * - svga()
     * - xga()
     * - hd()
     * - sxga()
     * - uxga()
     * - fhd()
     * - p_hd()
     * - p_3mp()
     * - qxga()
     * - qhd()
     * - wqxga()
     * - p_fhd()
     * - qsxga()
     */
    camera.qvga();

    /**
     * Configure JPEG quality
     * Allowed values are:
     *  - lowQuality()
     *  - highQuality()
     *  - bestQuality()
     *  - setQuality(quality), ranging from 10 (best) to 64 (lowest)
     */
    camera.highQuality();

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

    debug("SUCCCESS", camera.getFileSize());

    /**
     * Do whatever you want with the captured frame.
     * Access it with `camera.getBuffer()` (it contains the JPEG frame as uint8_t*)
     */
}
