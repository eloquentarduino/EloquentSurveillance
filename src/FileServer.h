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
            _msgConnection("FileServer listening at http://"),
            _delQuestVeri1("Are you sure you want to delete  "),
            _delQuestVeri2(""),
            _delQuestResu1("Sorry!"),
            _delQuestResu2("could not be deleted"),
            _delQuestResu3("could not be renamed"),
            _tblTitName("File name"),
            _tblTitSize("File size"),
            _tblTitIndx("Idx"),
            _btnRename("Rename file"),
            _btnDelete("Delete file"),
            _btnBackToMain("Back to main page"),
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
			    _server.sendContent(F("<body color=\"#FFFFFF\" bgcolor=\"#000000\" text=\"#FFFFFF\" link=\"#FFFF00\" alink=\"#FF9900\" vlink=\"#CCCC00\" >"));
			    _server.sendContent(F("<table border=\"1\"><tr><th>"));
			    _server.sendContent(_tblTitIndx);
			    _server.sendContent(F("</th><th>"));
			    _server.sendContent(_tblTitName);
			    _server.sendContent(F("</th><th>"));
			    _server.sendContent(_tblTitSize);
			    _server.sendContent(F("</th></th>&nbsp;<th></tr>"));

                uint16_t i = 1;
                uint16_t r = 0;
                File root = _fs.open("/");
                File file = root.openNextFile();

                while (file) {
                    // only list jpeg files
                    String filename = file.name();

                    if (filename.indexOf(".jpeg") > 0 || filename.indexOf(".jpg") > 0) {
                    	if (file.size() == 0) {
                    		_fs.remove(file.name());
                    	} else {
		                    _server.sendContent(F("<tr"));
		                    if (++r >= 5) { _server.sendContent(F(" style=\"background-color: #333; \""));  r=0; }
		                    _server.sendContent(F(">"));
		                    _server.sendContent(F("<td>"));
		                    _server.sendContent(String(i++));
		                    _server.sendContent(F("</td><td><a href=\"/view"));
		                    _server.sendContent(filename);
		                    _server.sendContent(F("\" style=\"font-weight: bold;\" >"));
		                    _server.sendContent(filename.substring(1));
		                    _server.sendContent(F("</a></td><td>"));
		                    _server.sendContent(formatBytes(file.size()));
		                    _server.sendContent(F("</td><td>"));
		                    _server.sendContent(F("<a href=\"/delete"));
		                    _server.sendContent(filename);
		                    _server.sendContent(F("\" style=\"color: #F00; font-weight: bold;\" >X</a>"));
		                    _server.sendContent(F("</td></tr>"));
                        }
                    }

                    if (i > _maxNumFiles)
                        break;

                    file = root.openNextFile();
                }

                _server.sendContent(F("</table>"));
                _server.sendContent(F("</body></html>"));
                _server.sendContent(F(""));
                _server.client().stop();

                return true;
            });

            _server.onNotFound([this]() {
                String uri = this->_server.uri();

                if (uri.indexOf("/view") == 0) {
                    String path = uri.substring(5);
                	int posiEXT = path.indexOf(".");
                	String monEXT = path.substring(posiEXT);
                	String monFIL = path.substring(1, posiEXT);

                    _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
                    _server.send(200, "text/html", "");
                    _server.sendContent(F("<html><head><meta charset=\"utf-8\" /><title>Carte SD - view</title></head>"));
                    _server.sendContent(F("<body color=\"#FFFFFF\" bgcolor=\"#000000\" text=\"#FFFFFF\">"));
                    _server.sendContent(F("<script>"));
                    _server.sendContent(F("var monEXT = '"));
                    _server.sendContent(monEXT);
                    _server.sendContent(F("';"));
                    _server.sendContent(F("var monFIL = '"));
                    _server.sendContent(monFIL);
                    _server.sendContent(F("';"));
                    _server.sendContent(F("var angle = 0;"));
                    _server.sendContent(F("if(monFIL.indexOf('_rot90') > 0) { monFIL = monFIL.substr(0, monFIL.indexOf('_rot90'));  angle = 90;}"));
                    _server.sendContent(F("if(monFIL.indexOf('_rot180') > 0) {monFIL = monFIL.substr(0, monFIL.indexOf('_rot180')); angle = 180;}"));
                    _server.sendContent(F("if(monFIL.indexOf('_rot270') > 0) {monFIL = monFIL.substr(0, monFIL.indexOf('_rot270')); angle = 270;}"));
                    _server.sendContent(F("function tournons(newAngle) {"));
                    _server.sendContent(F("    angle = newAngle;"));
                    _server.sendContent(F("	   document.getElementById('image').style.transform = 'rotate(' + angle + 'deg)';"));
                    _server.sendContent(F("    if(angle == 0) { document.getElementById('input_newname').value = monFIL; } else { document.getElementById('input_newname').value = monFIL + '_rot' + angle; }"));
                    _server.sendContent(F("	   document.getElementById('input_rotation_' + angle).checked = 'checked';"));
                    _server.sendContent(F("}"));
                    _server.sendContent(F("function soumettre() {"));
                    _server.sendContent(F("    document.location.href = '/renaming/' + document.getElementById('input_renameFIL').value + '&' + document.getElementById('input_newname').value + '&' + document.getElementById('input_renameEXT').value;"));
                    _server.sendContent(F("}"));
                    _server.sendContent(F("</script>"));
                    _server.sendContent(F("<form>"));
                    _server.sendContent(F("<h2>"));
                    _server.sendContent(path);
                    _server.sendContent(F("</h2>"));
                    _server.sendContent(F("<table border=\"0\">"));
                    _server.sendContent(F("<tr><td>"));
                    _server.sendContent(F("0&deg<input name=\"rotation\" id=\"input_rotation_0\"   type=\"radio\" value=\"0\"   onclick=\"tournons(this.value);\" /><br />"));
                    _server.sendContent(F("90&deg;<input name=\"rotation\" id=\"input_rotation_90\"  type=\"radio\" value=\"90\"  onclick=\"tournons(this.value);\" /><br />"));
                    _server.sendContent(F("180&deg;<input name=\"rotation\" id=\"input_rotation_180\" type=\"radio\" value=\"180\" onclick=\"tournons(this.value);\" /><br />"));
                    _server.sendContent(F("270&deg;<input name=\"rotation\" id=\"input_rotation_270\" type=\"radio\" value=\"270\" onclick=\"tournons(this.value);\" /><br />"));
                    _server.sendContent(F("<br /><br />"));
                    _server.sendContent(F("<br />"));
                    _server.sendContent(F("<input name=\"newname\" id=\"input_newname\" value=\""));
                    _server.sendContent(monFIL);
                    _server.sendContent(F("\" type=\"text\" />"));
                    _server.sendContent(F("<br />"));
                    _server.sendContent(F("<input name=\"newname\" type=\"button\" id=\"input_newname\" value=\""));
                    _server.sendContent(_btnRename);
                    _server.sendContent(F("\" onclick=\"soumettre();\" /><br />"));
                    _server.sendContent(F("<input name=\"extensio\" id=\"input_renameEXT\" value=\""));
                    _server.sendContent(monEXT);
                    _server.sendContent(F("\"  type=\"hidden\" /><br />"));
                    _server.sendContent(F("<input name=\"prevname\" id=\"input_renameFIL\" value=\""));
                    _server.sendContent(monFIL);
                    _server.sendContent(F("\"  type=\"hidden\" /><br />"));
                    _server.sendContent(F("</td><td>"));
                    _server.sendContent(F("<img id=\"image\" src=\"/raw"));
                    _server.sendContent(path);
                    _server.sendContent(F("\" style=\"margin-top:auto;\" />"));
                    _server.sendContent(F("<br clear=\"all\" /><br /></td></tr>"));
                    _server.sendContent(F("</table"));
                    _server.sendContent(F("<p style=\"text-align:center;\">"));
                    _server.sendContent(F("<input type=\"button\" onclick=\"document.location.href='/';\" value=\""));
                    _server.sendContent(_btnBackToMain);
                    _server.sendContent(F("\" style=\"color: black; background-color: #FF0; font-size: 140%; \" />"));
                    _server.sendContent(F("</p>"));
                    _server.sendContent(F("</form>"));
                    _server.sendContent(F("<script>"));
                    _server.sendContent(F("tournons(angle);"));
                    _server.sendContent(F("</script>"));
                    _server.sendContent(F("</body></html>"));
                    _server.client().stop();

                }
                if (uri.indexOf("/delete") == 0) {
                    String path = uri.substring(7);

                    _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
                    _server.send(200, "text/html", "");
                    _server.sendContent(F("<html><head><meta charset=\"utf-8\" /><title>Carte SD - delete</title></head>"));
                    _server.sendContent(F("<body color=\"#FFFFFF\" bgcolor=\"#000000\" text=\"#FFFFFF\">"));
                    _server.sendContent(F("<h2>"));
                    _server.sendContent(path);
                    _server.sendContent(F("</h2>"));
                    _server.sendContent(F("<img id=\"image\" src=\"/raw"));
                    _server.sendContent(path);
                    _server.sendContent(F("\" style=\"margin-top:auto;\" />"));
                    _server.sendContent(F("<br clear=\"all\" />"));
                    _server.sendContent(F("<button id=\"supprimer\" "));
                    _server.sendContent(F("onClick=\"if (confirm('"));
                    _server.sendContent(_delQuestVeri1);
                    _server.sendContent(path);
                    _server.sendContent(F(" ?' )) { document.location.href='/deleting"));
                    _server.sendContent(path);
                    _server.sendContent(F("'; } else { document.location.href='/'; }\""));
                    _server.sendContent(F(" style=\"color: black; background-color: #F00; font-size: 140%; \">"));
                    _server.sendContent(_btnDelete);
                    _server.sendContent(F("</button>"));
                    _server.sendContent(F("&nbsp;&nbsp;&nbsp;&nbsp;"));
                    _server.sendContent(F("<p style=\"text-align:center;\">"));
                    _server.sendContent(F("<input type=\"button\" onclick=\"document.location.href='/';\" value=\""));
                    _server.sendContent(_btnBackToMain);
                    _server.sendContent(F("\" style=\"color: black; background-color: #FF0; font-size: 140%; \" />"));
                    _server.sendContent(F("</p>"));
                    _server.sendContent(F("</body></html>"));
                    _server.client().stop();
                }
                if (uri.indexOf("/deleting") == 0) {
                    String path = uri.substring(9);
                    
                    _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
                    _server.send(200, "text/html", "");
                    _server.sendContent(F("<html><head><meta charset=\"utf-8\" /><title>Carte SD - delete</title></head>"));
                    _server.sendContent(F("<body color=\"#FFFFFF\" bgcolor=\"#000000\" text=\"#FFFFFF\">"));
                    _server.sendContent(F("<script>"));
                    if (!_fs.remove(path) ) {
		                _server.sendContent(F("alert('"));
		                _server.sendContent(_delQuestResu1);
		                _server.sendContent(F(".... "));
		                _server.sendContent(path);
		                _server.sendContent(F(" .... "));
		                _server.sendContent(_delQuestResu2);
		                _server.sendContent(F(".');"));
                    }
		            _server.sendContent(F("document.location.href='/';"));
                    _server.sendContent(F("</script>"));
                    _server.sendContent(F("<br /><br />"));
                    _server.sendContent(F("<p style=\"text-align:center;\">"));
                    _server.sendContent(F("<input type=\"button\" onclick=\"document.location.href='/';\" value=\""));
                    _server.sendContent(_btnBackToMain);
                    _server.sendContent(F("\" style=\"color: black; background-color: #FF0; font-size: 140%; \" />"));
                    _server.sendContent(F("</p>"));
                    _server.sendContent(F("</body></html>"));
                    _server.client().stop();
                }
                if (uri.indexOf("/renaming") == 0) {
                	int posiAND = uri.indexOf("&");
                	int posiEXT = uri.indexOf("&", posiAND+1);
                    String pathFROM = "/" + uri.substring(10, posiAND);
                    String pathTO   = "/" + uri.substring(posiAND+1, posiEXT);
                    String pathEXT  = uri.substring(posiEXT+1);
                    pathFROM = pathFROM + pathEXT;
                    pathTO   = pathTO + pathEXT;
                    _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
                    _server.send(200, "text/html", "");
                    _server.sendContent(F("<html><head><meta charset=\"utf-8\" /><title>Carte SD - rename</title></head>"));
                    _server.sendContent(F("<body color=\"#FFFFFF\" bgcolor=\"#000000\" text=\"#FFFFFF\">"));
                    _server.sendContent(F("<script>"));
                    if (!_fs.rename(pathFROM, pathTO) ) {
		                _server.sendContent(F("alert('"));
		                _server.sendContent(_delQuestResu1);
		                _server.sendContent(F(".... "));
		                _server.sendContent(pathFROM);
		                _server.sendContent(F(" .... "));
		                _server.sendContent(_delQuestResu3);
		                _server.sendContent(pathTO);
		                _server.sendContent(F(".');"));
                    }
		            _server.sendContent(F("document.location.href='/';"));
                    _server.sendContent(F("</script>"));
                    _server.sendContent(F("<br /><br />"));
                    _server.sendContent(F("<p style=\"text-align:center;\">"));
                    _server.sendContent(F("<input type=\"button\" onclick=\"document.location.href='/';\" value=\""));
                    _server.sendContent(_btnBackToMain);
                    _server.sendContent(F("\" style=\"color: black; background-color: #FF0; font-size: 140%; \" />"));
                    _server.sendContent(F("</p>"));
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

            return String(_msgConnection) + ip;
        }

        /**
         * Set the welcome message
         *
         * @return
         */
        String setWelcomeMessage(String value) {
        	_msgConnection = value;
        }

        String setdelQuestVeri1(String value) {
        	_delQuestVeri1 = value;
        }

        String setdelQuestVeri2(String value) {
        	_delQuestVeri2 = value;
        }

        String setdelQuestResu1(String value) {
        	_delQuestResu1 = value;
        }

        String setdelQuestResu2(String value) {
        	_delQuestResu2 = value;
        }
        String setdelQuestResu3(String value) {
        	_delQuestResu3 = value;
        }
        
        String setTblTitName(String value) {
        	_tblTitName = value;
  		}
        String setTblTitSize(String value) {
        	_tblTitSize = value;
  		}
        String setTblTitIndx(String value) {
        	_tblTitIndx = value;
  		}
        String setBtnRename(String value) {
        	_btnRename = value;
  		}
        String setBtnDelete(String value) {
        	_btnDelete = value;
  		}
        String setBtnBackMain(String value) {
        	_btnBackToMain = value;
  		}


    protected:
        fs::FS _fs;
        uint16_t _port;
        uint16_t _maxNumFiles;
        WebServer _server;
		String _msgConnection;
        String _delQuestVeri1;
        String _delQuestVeri2;
        String _delQuestResu1;
        String _delQuestResu2;
        String _delQuestResu3;
        String _tblTitName;
        String _tblTitSize;
        String _tblTitIndx;
        String _btnRename;
        String _btnDelete;
        String _btnBackToMain;

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
