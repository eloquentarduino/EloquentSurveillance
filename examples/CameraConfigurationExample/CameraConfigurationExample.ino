/**
 * This sketch showcases all the configuration functions
 * available for the camera sensor
 */
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
     * See CameraCaptureExample for more details
     */
    camera.m5wide();

    /**
     * Configure camera resolution
     * See CameraCaptureExample for more details
     */
    camera.qvga();

    /**
     * Configure JPEG quality
     * See CameraCaptureExample for more details
     */
    camera.highQuality();

    /**
     * Initialize the camera
     * If something goes wrong, print the error message
     */
    while (!camera.begin())
        debug("ERROR", camera.getErrorMessage());

    /**
     * Configure sensor
     * !!! AFTER camera.begin() !!!
     */
     // vertical flip
     camera.vflip();
     // horizontal flip
     camera.hflip();
     // adjust brightness
     camera.lowestBrightness();
     camera.lowBrightness();
     camera.highBrightness();
     camera.highestBrightness();
    // adjust saturation
    camera.lowestSaturation();
    camera.lowSaturation();
    camera.highSaturation();
    camera.highestSaturation();
    // adjust manual gain (0 - 30)
    camera.setManualGain(10);
    // adjust manual exposure (0 - 1200)
    camera.setManualExposure(300);
    // adjust gain ceiling (0 - 6)
    camera.setGainCeiling(3);
    // toggle white balance
    camera.enableAutomaticWhiteBalance();
    camera.disableAutomaticWhiteBalance();
    // toggle gain control
    camera.enableGainControl();
    camera.disableGainControl();
    // toggle exposure control
    camera.enableExposureControl();
    camera.disableExposureControl();
    // toggle automatic white balance gain
    camera.enableAutomaticWhiteBalanceGain();
    camera.disableAutomaticWhiteBalanceGain();
    // toggle automatic gain control
    camera.enableAutomaticGainControl();
    camera.disableAutomaticGainControl();
    // toggle automatic exposure control
    camera.enableAutomaticExposureControl();
    camera.disableAutomaticExposureControl();
    // toggle DCW
    camera.enableDCW();
    camera.disableDCW();
    // toggle BPC
    camera.enableBPC();
    camera.disableBPC();
    // toggle WPC
    camera.enableWPC();
    camera.disableWPC();
    // toggle lens correction
    camera.enableLensCorrection();
    camera.disableLensCorrection();
    // toggle raw gamma
    camera.enableRawGamma();
    camera.disableRawGamma();

    // you can also configure the sensor manually, if you prefer
    camera.configureSensor([](sensor_t *sensor) {
        // configure sensor as you wish
    });


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
