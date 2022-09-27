//
// Created by Simone on 23/06/2022.
//

#ifndef ELOQUENTSURVEILLANCE_FILESERVER
#define ELOQUENTSURVEILLANCE_FILESERVER


#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#ifdef USE_SD
#include <SD.h>
#include <SPI.h>
#define ELOQUENTSURVEILLANCE_FILESERVER_FS SD
#else
#include <SPIFFS.h>
#define ELOQUENTSURVEILLANCE_FILESERVER_FS SPIFFS
#endif
#include "./traits/HasErrorMessage.h"


namespace EloquentSurveillance {
    /**
     * ESP32 camera file server
     */
    class FileServer : public HasErrorMessage {
    public:

        /**
         *
         * @param port
         */
        FileServer(uint16_t port = 81) :
            _port(port),
            _server(port),
            _maxNumFiles(100) {
            setErrorMessage("");
        }

        /**
        *
        * @return
        */
        bool begin() {
            _server.on("/", HTTP_GET, [this]() {
                String html = String(F("<table border=\"1\"><thead><tr><th>Idx</th><th>Filename</th><th>Size</th></tr></thead><tbody>"));

                uint16_t i = 1;
                File root = ELOQUENTSURVEILLANCE_FILESERVER_FS.open("/");
                File file = root.openNextFile();

                while (file) {
                    // only list jpeg files
                    String filename = file.name();

                    if (filename.indexOf(".jpeg") || filename.indexOf(".jpg")) {
                        html += F("<tr><td>");
                        html += (i++);
                        html += F("</td><td><a href=\"/view");
                        html += filename;
                        html += F("\" target=\"_blank\">");
                        html += filename.substring(1);
                        html += F("</a></td><td>");
                        html += formatBytes(file.size());
                        html += F("</td></tr>");
                    }

                    if (i > _maxNumFiles)
                        break;

                    file = root.openNextFile();
                }

                html += F("</tbody></table>");

                this->_server.send(200, "text/html", html);
                return true;
            });

            _server.onNotFound([this]() {
                String uri = this->_server.uri();

                if (uri.indexOf("/view/") == 0) {
                    String path = uri.substring(5);
                    File file = ELOQUENTSURVEILLANCE_FILESERVER_FS.open(path, "r");

                    verbose("View file = ", path);

                    if (file.size()) {
                        this->_server.streamFile(file, "image/jpeg");
                        file.close();

                        return true;
                    }
                }

                this->_server.send(404, "text/plain", String(F("URI not found: ")) + uri);
                return false;
            });

            _server.begin();

            return true;
        }

        /**
         *
         * @param value
         */
        void setMaxNumFiles(uint16_t value) {
            _maxNumFiles = value;
        }

        /**
         *
         */
        void handle() {
            _server.handleClient();
        }

        /**
         * Get address of server
         *
         * @return
         */
        String getWelcomeMessage() {
            String ip = wifi.getIP();

            if (_port != 80) {
                ip += ':';
                ip += _port;
            }

            return String(F("FileServer listening at http://")) + ip;
        }

    protected:
        uint16_t _port;
        uint16_t _maxNumFiles;
        WebServer _server;

        /**
         * Test is SD is attached
         * @return
         */
        bool hasSD() {
#ifdef USE_SD
            return SD.cardType() != CARD_NONE;
#else
            return false;
#endif
        }

        /**
         *
         * @param bytes
         * @return
         */
        String formatBytes(size_t bytes) {
            if (bytes < 1024) {
                return String(bytes) + "B";
            } else if (bytes < (1024 * 1024)) {
                return String(bytes / 1024.0) + "KB";
            } else if (bytes < (1024 * 1024 * 1024)) {
                return String(bytes / 1024.0 / 1024.0) + "MB";
            } else {
                return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
            }
        }
    };
}

#endif
