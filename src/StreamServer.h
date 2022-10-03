//
// Created by Simone on 23/06/2022.
//

#ifndef ELOQUENTSURVEILLANCE_STREAMSERVER
#define ELOQUENTSURVEILLANCE_STREAMSERVER


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
            String ip = wifi.getIP();

            if (_port != 80) {
                ip += ':';
                ip += _port;
            }

            return
                String(F("StreamServer listening at http://"))
                + ip
                + String(F(".\nMJPEG stream is available at "))
                + ip
                + "/stream";
        }

    protected:
        bool _isClient;
        uint16_t _port;
        httpd_config_t _config;
        httpd_handle_t _httpd;
    };
}

#endif
