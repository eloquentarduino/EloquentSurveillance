//
// Created by Simone on 07/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_HASPIXFORMAT_H
#define ELOQUENTSURVEILLANCE_HASPIXFORMAT_H

#include <esp_camera.h>

namespace EloquentSurveillance {
    /**
     * Handle methods related to frame size
     */
    class HasFramesize {
    public:
        /**
         * Set QQVGA resolution
         */
        void qqvga() {
            _framesize = FRAMESIZE_QQVGA;
        }

        /**
         * Set QVGA resolution
         */
        void qvga() {
            _framesize = FRAMESIZE_QVGA;
        }

        /**
         * Set VGA resolution
         */
        void vga() {
            _framesize = FRAMESIZE_VGA;
        }

        /**
         * Get width of captured image
         * @return
         */
        uint16_t getWidth() {
            switch (_framesize) {
                case FRAMESIZE_QQVGA:
                    return 160;
                case FRAMESIZE_QVGA:
                    return 320;
                case FRAMESIZE_VGA:
                    return 640;
                default:
                    return 0;
            }
        }

        /**
         * Get height of captured image
         * @return
         */
        uint16_t getHeight() {
            switch (_framesize) {
                case FRAMESIZE_QQVGA:
                    return 120;
                case FRAMESIZE_QVGA:
                    return 240;
                case FRAMESIZE_VGA:
                    return 480;
                default:
                    return 0;
            }
        }

    protected:
        framesize_t _framesize;
    };
}

#endif //ELOQUENTSURVEILLANCE_HASPIXFORMAT_H
