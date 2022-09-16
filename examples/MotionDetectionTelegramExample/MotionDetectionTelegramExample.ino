#define VERBOSE

#include "EloquentSurveillance.h"
#include "TelegramChat.h"

#define WIFI_SSID "Abc"
#define WIFI_PASS "12345678"
#define BOT_TOKEN "XXXXXXXXXX:yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"
#define CHAT_ID 1234567890

EloquentSurveillance::Motion motion;
EloquentSurveillance::TelegramChat chat(BOT_TOKEN, CHAT_ID);


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
    motion.setMinChanges(0.05);
    motion.setMinPixelDiff(7);
    motion.setMinSizeDiff(100);

    while (!camera.begin())
        debug("ERROR", camera.getErrorMessage());

    while (!connectToWiFi(WIFI_SSID, WIFI_PASS))
        debug("ERROR", "Cannot connect to WiFi");

    debug("SUCCESS", "Camera OK, WiFi connected");
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

        bool messageResponse = chat.sendMessage("Motion");
        debug("TELEGRAM MSG", messageResponse ? "OK" : "ERR");

        bool photoResponse = chat.sendPhoto();
        debug("TELEGRAM PHOTO", photoResponse ? "OK" : "ERR");
    }
}