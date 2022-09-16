//
// Created by Simone on 23/06/2022.
//

#ifndef ELOQUENTSURVEILLANCE_WEBSERVER
#define ELOQUENTSURVEILLANCE_WEBSERVER


#include <WiFi.h>
#include <esp_camera.h>
#include <esp_http_server.h>
#include "./globals.h"
#include "./traits/HasErrorMessage.h"


namespace EloquentSurveillance {
    /**
     * ESP32 camera web server
     */
    class StreamServer : public HasErrorMessage {
    public:

        /**
         *
         * @param port
         */
        StreamServer(uint16_t port = 80) :
            _port(port) {
            setErrorMessage("");
            _config = HTTPD_DEFAULT_CONFIG();
            _config.server_port = port;
        }

        /**
         *
         * @param ssid
         * @param password
         * @param timeout
         */
        bool beginAsClient(const char* ssid, const char* password, uint16_t timeout = 10000) {
            setErrorMessage("");
            _isClient = true;

            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);

            time_t start = millis();

            while (millis() - start < timeout) {
                if (WiFi.status() == WL_CONNECTED) {
                    return begin();
                }
            }

            return setErrorMessage("Cannot connect to WiFi as client");
        }

        /**
         *
         * @tparam Callback
         * @param ssid
         * @param password
         * @param callback
         * @param timeout
         * @return
         */
        template<typename Callback>
        bool beginAsClient(const char* ssid, const char* password, Callback callback, uint16_t timeout = 10000) {
            setErrorMessage("");
            _isClient = true;

            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);

            time_t start = millis();

            while (millis() - start < timeout) {
                if (WiFi.status() == WL_CONNECTED)
                    return begin();

                callback(millis() - start);
                delay(100);
            }

            return setErrorMessage("Cannot connect to WiFi as client");
        }

        /**
        *
        * @return
        */
        bool begin() {
            if (httpd_start(&_httpd, &_config) != ESP_OK)
                return setErrorMessage("Cannot start HTTP server");

            httpd_uri_t stream_uri = {
                    .uri       = "/stream",
                    .method    = HTTP_GET,
                    .handler   = [](httpd_req_t *req) {
                        esp_err_t res = ESP_OK;
                        char *part[64];

                        if (httpd_resp_set_type(req, "multipart/x-mixed-replace;boundary=123456789000000000000987654321") != ESP_OK)
                            return ESP_FAIL;

                        httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

                        while (true) {
                            size_t contentTypeHeaderLength;

                            if (!gCapture()) {
                                delay(1);
                                continue;
                            }

                            if (!gIsFrame())
                                continue;

                            contentTypeHeaderLength = snprintf((char *) part, 64,
                                                               "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n",
                                                               gFrame->len);

                            if (httpd_resp_send_chunk(req, (const char *) part, contentTypeHeaderLength) != ESP_OK)
                                return ESP_FAIL;

                            if (httpd_resp_send_chunk(req, (const char *) gFrame->buf, gFrame->len) != ESP_OK) {
                                esp_camera_fb_return(gFrame);

                                return ESP_FAIL;
                            }

                            res = httpd_resp_send_chunk(req, "\r\n--123456789000000000000987654321\r\n", 37);
                        }

                        return ESP_OK;
                    },
                    .user_ctx  = NULL
            };

            httpd_register_uri_handler(_httpd, &stream_uri);

            return true;
        }


        /**
         *
         * @return
         */
        String getWelcomeMessage() {
            IPAddress ip = _isClient ? WiFi.localIP() : WiFi.softAPIP();

            return
                String(F("WebServer listening at http://"))
                + ip[0] + '.' + ip[1] + '.' + ip[2] + '.' + ip[3] + ':' + _port
                + String(F(". MJPEG stream is available at /stream"));
        }

    protected:
        bool _isClient;
        uint16_t _port;
        httpd_config_t _config;
        httpd_handle_t _httpd;
    };
}

#endif
