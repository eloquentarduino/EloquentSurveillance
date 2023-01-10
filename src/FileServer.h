//
// Created by Simone on 23/06/2022.
//

#ifndef ELOQUENTSURVEILLANCE_FILESERVER
#define ELOQUENTSURVEILLANCE_FILESERVER


#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
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
        FileServer(fs::FS &fs, uint16_t port = 80) :
            _fs(fs),
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
                _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
                _server.send(200, "text/html", "");
			    _server.sendContent(F("<html><head><meta charset=\"utf-8\" /><title>Carte SD</title></head>"));
			    _server.sendContent(F("<body>"));
			    _server.sendContent(F("<table border=\"1\"><thead><tr><th>Idx</th><th>Filename</th><th>Size</th></th>&nbsp;<th></tr></thead><tbody>"));

                uint16_t i = 1;
                uint16_t r = 0;
                File root = _fs.open("/");
                File file = root.openNextFile();

                while (file) {
                    // only list jpeg files
                    String filename = file.name();

                    if (filename.indexOf(".jpeg") || filename.indexOf(".jpg")) {
                    	if (file.size() == 0) {
                    		_fs.remove(file.name());
                    	} else {
		                    _server.sendContent(F("<tr"));
		                    if (++r >= 5) { _server.sendContent(F(" style=\"background-color: #CCC; \""));  r=0; }
		                    _server.sendContent(F(">"));
		                    _server.sendContent(F("<td>"));
		                    _server.sendContent(String(i++));
		                    _server.sendContent(F("</td><td><a href=\"/view"));
		                    _server.sendContent(filename);
		                    _server.sendContent(F("\" style=\"font-weight: bold;\" target=\"_blank\">"));
		                    _server.sendContent(filename.substring(1));
		                    _server.sendContent(F("</a></td><td>"));
		                    _server.sendContent(formatBytes(file.size()));
		                    _server.sendContent(F("</td><td>"));
		                    _server.sendContent(F("<a href=\"/delete"));
		                    _server.sendContent(filename);
		                    _server.sendContent(F("\" style=\"color: #600; font-weight: bold;\" >X</a>"));
		                    _server.sendContent(F("</td></tr>"));
                        }
                    }

                    if (i > _maxNumFiles)
                        break;

                    file = root.openNextFile();
                }

                _server.sendContent(F("</tbody></table>"));
                _server.sendContent(F("</body></html>"));
                _server.sendContent(F(""));
                _server.client().stop();

                return true;
            });

            _server.onNotFound([this]() {
                String uri = this->_server.uri();

                if (uri.indexOf("/view") == 0) {
                    String path = uri.substring(5);

                    _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
                    _server.send(200, "text/html", "");
                    _server.sendContent(F("<html><head><meta charset=\"utf-8\" /><title>Carte SD - view</title></head>"));
                    _server.sendContent(F("<body>"));
                    _server.sendContent(F("<button id=\"rotate\" style=\"position:absolute;top:10px;left:10px;z-index:1;\">Rotate</button><div id=\"frame\" style=\"display:flex\"><img id=\"image\" src=\"/raw"));
                    _server.sendContent(path);
                    _server.sendContent(F("\" style=\"margin-top:auto;\" /></div><script>"));
                    _server.sendContent(F("document.addEventListener(\"DOMContentLoaded\",function(){setTimeout(function(){var t=document.getElementById(\"frame\"),e=document.getElementById(\"image\"),n=document.getElementById(\"rotate\"),d=0;t.style.height=Math.max(e.clientHeight,e.naturalHeight,e.clientWidth,e.naturalWidth)+\"px\",n.addEventListener(\"click\",function(){d=(d+90)%360,e.style.transform=\"rotate(\"+d+\"deg)\"})},1e3)});"));
                    _server.sendContent(F("</script>"));
                    _server.sendContent(F("</body></html>"));
                    _server.client().stop();
                }
                if (uri.indexOf("/delete") == 0) {
                    String path = uri.substring(7);

                    _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
                    _server.send(200, "text/html", "");
                    _server.sendContent(F("<html><head><meta charset=\"utf-8\" /><title>Carte SD - delete</title></head>"));
                    _server.sendContent(F("<body>"));
                    _server.sendContent(F("<img id=\"image\" src=\"/raw"));
                    _server.sendContent(path);
                    _server.sendContent(F("\" style=\"margin-top:auto;\" />"));
                    _server.sendContent(F("<br clear=\"all\" />"));
                    _server.sendContent(F("<button id=\"supprimer\" "));
                    _server.sendContent(F("onClick=\"if (confirm(\'Etes-vous sur de vouloir supprimer ? \')) { document.location.href='/deleting/"));
                    _server.sendContent(path);
                    _server.sendContent(F("'; } else { document.location.href='/'; }\""));
                    _server.sendContent(F(">"));
                    _server.sendContent(F("Supprimer</button>"));
                    _server.sendContent(F("<br />"));
                    _server.sendContent(F("</body></html>"));
                    _server.client().stop();
                }
                if (uri.indexOf("/deleting") == 0) {
                    String path = uri.substring(10);
                    _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
                    _server.send(200, "text/html", "");
                    _server.sendContent(F("<html><head><meta charset=\"utf-8\" /><title>Carte SD - delete</title></head>"));
                    _server.sendContent(F("<body>"));
                    _server.sendContent(F("<script>"));
                    if (!_fs.remove(path) ) {
		                _server.sendContent(F("alert('Désolé! .... "));
		                _server.sendContent(path);
		                _server.sendContent(F(" .... n`a pu être supprimé.');"));
                    }
		            _server.sendContent(F("document.location.href='/';"));
                    _server.sendContent(F("</script>"));
                    _server.sendContent(F("<br />"));
                    _server.sendContent(F("</body></html>"));
                    _server.client().stop();
                }

                if (uri.indexOf("/raw") == 0) {
                    String path = uri.substring(4);
                    File file = _fs.open(path, "r");

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
        fs::FS _fs;
        uint16_t _port;
        uint16_t _maxNumFiles;
        WebServer _server;

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
