//
// Created by Simone on 07/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_CAMERA_H
#define ELOQUENTSURVEILLANCE_CAMERA_H

#include <esp_camera.h>
#include <FS.h>
#include "./traits/HasFramesize.h"
#include "./traits/SetJpegQuality.h"
#include "./traits/ChooseModel.h"
#include "./traits/HasErrorMessage.h"
#include "./globals.h"


namespace EloquentSurveillance {
    /**
     * Esp32 cam interface
     */
    class Camera : public HasFramesize, public SetJpegQuality, public ChooseModel, public HasErrorMessage {
    public:
        /**
         *
         */
        Camera() {
            highQuality();

            _config.pin_d0 = 0;
            _config.pin_d1 = 0;
        }

        /**
         *
         */
        void aithinker() {
            _aithinker(&_config);
        }

        /**
         *
         */
        void m5() {
            _m5(&_config);
        }

        /**
         *
         */
        void m5wide() {
            _m5wide(&_config);
        }

        /**
         *
         */
        void eye() {
            _eye(&_config);
        }

        /**
         *
         */
        void wrover() {
            _wrover(&_config);
        }


        /**
         *
         * @return
         */
        bool begin() {
            _config.ledc_channel = LEDC_CHANNEL_0;
            _config.ledc_timer = LEDC_TIMER_0;
            _config.fb_count = 1;
            _config.pixel_format = PIXFORMAT_JPEG;
            _config.frame_size = _framesize;
            _config.jpeg_quality = _jpegQuality;

            if (!_config.xclk_freq_hz)
                _config.xclk_freq_hz = 20000000;

            if (!_config.pin_d0 && !_config.pin_d0)
                return setErrorMessage("You must set a model");

            if (esp_camera_init(&_config) != ESP_OK)
                return setErrorMessage("Init error");

            _sensor = esp_camera_sensor_get();
            _sensor->set_framesize(_sensor, _framesize);

            return true;
        }

        /**
         *
         * @return
         */
        uint16_t getFileSize() {
            return gIsFrame() ? gFrame->len : 0;
        }

        /**
         *
         * @return
         */
        camera_fb_t* getFrame() {
            return gIsFrame() ? gFrame : NULL;
        }

        /**
         *
         * @return
         */
        uint8_t* getBuffer() {
            return gIsFrame() ? gFrame->buf : NULL;
        }

        /**
         * Capture new frame
         */
        bool capture() {
            if (!gCapture())
                return setErrorMessage("Capture error");

            return true;
        }

        /**
         *
         * @param fs
         * @param filename
         * @return
         */
        bool saveTo(fs::FS &fs, String filename) {
            if (!gIsFrame())
                return setErrorMessage("Save error: frame not found");

            auto file = fs.open(filename, "wb");

            file.write(gFrame->buf, gFrame->len);
            file.close();

            return true;
        }


    protected:
        camera_config_t _config;
        sensor_t *_sensor;
    };
}


static EloquentSurveillance::Camera camera;

#endif //ELOQUENTSURVEILLANCE_CAMERA_H
