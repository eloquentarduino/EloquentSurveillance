//
// Created by Simone on 23/06/2022.
//

#ifndef ELOQUENTSURVEILLANCE_MOTIONSTREAMSERVER
#define ELOQUENTSURVEILLANCE_MOTIONSTREAMSERVER


#include <WiFi.h>
#include <esp_camera.h>
#include <esp_http_server.h>
#include "../src/globals.h"
#include "../src/traits/HasErrorMessage.h"
#include "../src/Motion.h"



namespace EloquentSurveillance {
    /**
     * ESP32 camera motion web server
     */
    class MotionStreamServer : public HasErrorMessage {
    public:

        /**
         *
         * @param port
         */
        MotionStreamServer() {
            setErrorMessage("");
            _configCtrl = HTTPD_DEFAULT_CONFIG();
            _configStream = HTTPD_DEFAULT_CONFIG();
            _configCtrl.server_port = 80;
            _configCtrl.ctrl_port = 80;
            _configStream.server_port = 81;
            _configStream.ctrl_port = 81;
        }

        /**
        *
        * @return
        */
        bool begin() {
            if (httpd_start(&_httpdCtrl, &_configCtrl) != ESP_OK)
                return setErrorMessage("Cannot start HTTP control server");

            if (httpd_start(&_httpdStream, &_configStream) != ESP_OK)
                return setErrorMessage("Cannot start HTTP stream server");

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
                                Serial.println("Cannot capture");
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

            httpd_uri_t index_uri = {
                    .uri       = "/",
                    .method    = HTTP_GET,
                    .handler   = [](httpd_req_t *req) {
                        const char html[] PROGMEM = R"(
                            <body>
                                <img id="stream" src=""></img>
                                <p id="motion"></p>
                                <script>
                                    const ctrl = new AbortController();
                                    const signal = ctrl.signal;
                                    document.getElementById('stream').setAttribute('src', 'http://' + window.location.hostname + ':81/stream')
                                    setInterval(() => {
                                        //ctrl.abort()
                                        fetch('/motion', {signal})
                                            .then(res => res.text())
                                            .then(res => document.getElementById('motion').textContent = res)
                                    }, 500)
                                </script>
                            </body>
                        )";
                        httpd_resp_sendstr(req, html);

                        return ESP_OK;
                    },
                    .user_ctx  = NULL
            };

            httpd_uri_t motion_uri = {
                    .uri       = "/motion",
                    .method    = HTTP_GET,
                    .handler   = [](httpd_req_t *req) {
                        if (motion.elapsed() < 500 || motion.forceUpdate())
                            httpd_resp_sendstr(req, "motion");
                        else
                            httpd_resp_sendstr(req, "no motion");

                        return ESP_OK;
                    },
                    .user_ctx  = NULL
            };

            httpd_register_uri_handler(_httpdCtrl, &index_uri);
            httpd_register_uri_handler(_httpdCtrl, &motion_uri);
            httpd_register_uri_handler(_httpdStream, &stream_uri);

            return true;
        }


        /**
         *
         * @return
         */
        String getWelcomeMessage() {
            String ip = wifi.getIP();

//            if (_port != 80) {
//                ip += ':';
//                ip += _port;
//            }

            return
                String(F("MotionStreamServer listening at http://"))
                + ip
                + String(F(".\nMJPEG stream is available at "))
                + ip
                + ":81/stream";
        }

    protected:
        uint16_t _port;
        httpd_config_t _configCtrl;
        httpd_config_t _configStream;
        httpd_handle_t _httpdCtrl;
        httpd_handle_t _httpdStream;
    };
}

#endif
