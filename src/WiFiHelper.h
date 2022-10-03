//
// Created by Simone on 24/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_WIFIHELPER_H
#define ELOQUENTSURVEILLANCE_WIFIHELPER_H

#include <WiFi.h>

namespace EloquentSurveillance {
    /**
     * Helper functions to manage WiFi
     */
    class WiFiHelper {
    public:
        /**
         * Connect to WiFi network as client
         *
         * @param ssid
         * @param password
         * @return
         */
        bool connect(const char *ssid, const char *password, uint16_t timeout = 10000) {
            _isClient = true;

            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);

            uint16_t start = millis();

            while (millis() - start < timeout) {
                if (WiFi.status() == WL_CONNECTED)
                    return (_isConnected = true);

                delay(100);
            }

            return false;
        }

        /**
         * Get error message, if any
         *
         * @return
         */
        String getErrorMessage() {
            return _isConnected ? "" : "Unknown error";
        }

        /**
         * Get IP address as string
         *
         * @return
         */
        String getIP() {
            IPAddress ip = _isClient ? WiFi.localIP() : WiFi.softAPIP();

            return String(ip[0]) + '.' + ip[1] + '.' + ip[2] + '.' + ip[3];
        }



    protected:
        bool _isClient = true;
        bool _isConnected = false;
    };
}

static EloquentSurveillance::WiFiHelper wifi;

#endif //ELOQUENTSURVEILLANCE_WIFIHELPER_H
