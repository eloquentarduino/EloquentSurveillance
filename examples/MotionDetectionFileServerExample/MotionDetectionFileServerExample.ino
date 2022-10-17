/**
 * Save motion captures to SD and display on a web page
 */
#include <EloquentSurveillance.h>
#include <SD_MMC.h>
#include <SPI.h>

#define WIFI_SSID "Abc"
#define WIFI_PASS "12345678"


EloquentSurveillance::Motion motion;
/**
 * You can replace SD_MMC with SPIFFS or SD
 * 80 is the port to listen to
 * You can change it to whatever you want, 80 is the default for HTTP
 */
EloquentSurveillance::FileServer fileServer(SD_MMC, 80);


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
    camera.vga();
    camera.highQuality();

    /**
     * See MotionDetectionExample for more details
     */
    motion.setMinChanges(0.1);
    motion.setMinPixelDiff(8);
    motion.setMinSizeDiff(0.03);
    motion.debounce(10000L);

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
     * Initialize the filesystem
     * If something goes wrong, print an error message
     */
    while (!SD_MMC.begin() || SD_MMC.cardType() == CARD_NONE)
        debug("ERROR", "Cannot init SD card");

    /**
     * Initialize the file server
     * If something goes wrong, print an error message
     */
    while (!fileServer.begin())
        debug("ERROR", fileServer.getErrorMessage());

    /**
     * Display address of file server
     */
    debug("SUCCESS", fileServer.getWelcomeMessage());
}

/**
 *
 */
void loop() {
    /**
     * Handle file server requests, if any
     */
    fileServer.handle();

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
     * For this example, save the image to SPIFFS and print a debug message
     */
    if (!motion.update())
        return;

    if (motion.detect()) {
        String filename = motion.getNextFilename();

        if (camera.saveTo(SD_MMC, filename)) {
            debug("INFO", String("Frame saved to SPIFFS: ") + filename);
            debug("INFO", "Refresh the file server webpage to see the new file");
        }
    }
}
