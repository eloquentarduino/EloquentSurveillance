//
// Created by Simone on 09/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_NOTIFICATIONS_H
#define ELOQUENTSURVEILLANCE_NOTIFICATIONS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include "./traits/HasErrorMessage.h"
#include "./traits/Debounces.h"
#include "./Motion.h"
#include "./globals.h"


namespace EloquentSurveillance {

    /**
     * Enable / disable notification channels
     */
    enum class ChannelStatus {
        UNDEFINED,
        TURN_ON,
        TURN_OFF
    };

    /**
     * Notification center
     */
    class Dashboard : public HasErrorMessage, public Debounces {
    public:

        /**
         *
         */
        Dashboard(const char *userToken = "", const char *deviceToken = "") :
            _userToken(userToken),
            _deviceToken(deviceToken),
            _series(1),
            _motion(NULL) {

            _channels.email = ChannelStatus::UNDEFINED;
            _channels.telegram = ChannelStatus::UNDEFINED;
            _channels.webhook = ChannelStatus::UNDEFINED;
            _channels.slack = ChannelStatus::UNDEFINED;

        }

        /**
         *
         * @param userToken
         * @param deviceToken
         */
        void begin(String userToken, String deviceToken) {
            _userToken = userToken;
            _deviceToken = deviceToken;
        }

        /**
         * Bind motion object
         *
         * @param motion
         */
        void bind(Motion& motion) {
            _motion = &motion;
        }

        /**
         * Unbind motion object
         */
        void unbindMotion() {
            _motion = NULL;
        }

        /**
         * Enable Telegram notification
         */
        void enableTelegram() {
            _channels.telegram = ChannelStatus::TURN_ON;
        }

        /**
         * Disable Telegram notification
         */
        void disableTelegram() {
            _channels.telegram = ChannelStatus::TURN_ON;
        }

        /**
         * Enable email notification
         */
        void enableEmail() {
            _channels.email = ChannelStatus::TURN_ON;
        }

        /**
         * Disable email notification
         */
        void disableEmail() {
            _channels.email = ChannelStatus::TURN_ON;
        }

        /**
         * Send a single picture
         *
         * @return
         */
        bool send() {
            return sendMany(1);
        }

        /**
         * Send notifications
         * @return
         */
        bool sendMany(uint8_t howMany) {
            bool status = true;
            uint8_t succeded = 0;
            String batch = "";
            String httpError = "";

            if (!validate())
                return false;

            if (howMany > 1)
                batch = getBatchNumber();

            for (uint8_t i = 0; i < howMany; i++) {
                HTTPClient http;

                http.begin(
                    String(F("http://api.motion.eloquentarduino.com/api/devices/")) + _deviceToken + "/motion"
                );
                http.addHeader(F("Content-Type"), "image/jpeg");
                http.addHeader(F("X-User-Token"), _userToken);
                http.addHeader(F("X-Batch"), batch);
                http.addHeader(F("X-Telegram"), statusToString(_channels.telegram));
                http.addHeader(F("X-Email"), statusToString(_channels.email));
                http.addHeader(F("X-Webhook"), statusToString(_channels.webhook));
                http.addHeader(F("X-Slack"), statusToString(_channels.slack));

                int statusCode = http.POST(gFrame->buf, gFrame->len);

                verbose("Batch #", batch, ", Frame #", i + 1, " status code = ", statusCode);

                if (statusCode != 200) {
                    String response = http.getString();

                    httpError = String("Server error: (") + statusCode + ") " + response;
                    verbose("Server ERROR: ", response);
                    status = false;
                }
                else {
                    verbose("Server OK: ", http.getString());
                    succeded += 1;
                }

                http.end();

                // capture next frame
                if (i != howMany - 1) {
                    gCapture();

                    if (_motion != NULL)
                        _motion->forceUpdate();
                }
            }

            touch();
            verbose("Succeded notifications = ", succeded, ", Failed notifications = ", howMany - succeded);

            return status ? true : setErrorMessage(httpError);
        }

    protected:
        String _userToken;
        String _deviceToken;
        uint8_t _series;
        Motion *_motion;
        struct {
            ChannelStatus telegram;
            ChannelStatus email;
            ChannelStatus webhook;
            ChannelStatus slack;
        } _channels;


        /**
         * Convert status to string
         *
         * @param status
         * @return
         */
        String statusToString(ChannelStatus status) {
            switch (status) {
                case ChannelStatus::TURN_ON:
                    return "enabled";
                case ChannelStatus::TURN_OFF:
                    return "disabled";
                default:
                    return "undefined";
            }
        }

        /**
         * Generate random batch number as string
         *
         * @return
         */
        String getBatchNumber()
        {
            char buffer[9];
            const char alphabet[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

            for (int i = 0; i < 8; i += 2) {
                size_t seed = millis() + i;

                randomSeed(seed);
                buffer[i] = alphabet[random(61)];
                randomSeed(seed + abs(WiFi.RSSI()));
                buffer[i + 1] = alphabet[random(61)];
            }

            buffer[8] = '\0';

            return String(buffer);
        }

        /**
         * Validate request
         *
         * @return
         */
        bool validate() {
            if (!debounced())
                return setErrorMessage("Too many notifications");

            if (_userToken.length() != 32)
                return setErrorMessage("Invalid user token");

            if (_deviceToken.length() != 16)
                return setErrorMessage("Invalid device token");

            if (!gIsFrame())
                return setErrorMessage("No frame found");

            return setErrorMessage("");
        }
    };
}

#endif //ELOQUENTSURVEILLANCE_NOTIFICATIONS_H
