#define VERBOSE

#include "EloquentSurveillance.h"
#include "TelegramChat.h"

#define WIFI_SSID "Abc"
#define WIFI_PASS "12345678"
#define BOT_TOKEN "XXXXXXXXXX:yyyyyyyyyyyyyyyyyyyyyyyy-zzzzzzzzzz"
#define CHAT_ID 1234567890


EloquentSurveillance::Motion motion;
/**
 * Instantiate a Telegram chat to send messages and photos
 */
EloquentSurveillance::TelegramChat chat(BOT_TOKEN, CHAT_ID);


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
    motion.setMinChanges(0.05);
    motion.setMinPixelDiff(7);
    motion.setMinSizeDiff(100);
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

    debug("SUCCESS", "Camera OK, WiFi connected");
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
     * In this case, we send the photo to Telegram
     */
    if (motion.update()) {
        debug("INFO", String("Motion detected in ") + motion.getExecutionTimeInMicros() + " us");

        bool messageResponse = chat.sendMessage("Motion detected");
        debug("TELEGRAM MSG", messageResponse ? "OK" : "ERR");

        /**
         * @bug: this always returns false, even on success
         */
        bool photoResponse = chat.sendPhoto();
        debug("TELEGRAM PHOTO", photoResponse ? "OK" : "ERR");
    }
}