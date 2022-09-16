//
// Created by Simone on 08/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_MOTION_H
#define ELOQUENTSURVEILLANCE_MOTION_H

#include <stdio.h>
#include "./picojpeg.h"
#include "./debug.h"
#include "./globals.h"
#include "./traits/HasErrorMessage.h"
#include "./traits/BenchmarksCode.h"
#include "./traits/Debounces.h"


/**
 * Callback for picojpeg decoding
 *
 * @param dest
 * @param chunkSize
 * @param read
 * @param data
 * @return
 */
unsigned char pjpeg_need_bytes_callback(unsigned char* dest, unsigned char chunkSize, unsigned char *read, void *data)
{
    if (!gIsFrame()) {
        *read = 0;
        return 0;
    }

    uint16_t n = gFrame->len - gOffset > chunkSize ? chunkSize : gFrame->len - gOffset;

    if (gOffset > gFrame->len)
        return PJPG_STREAM_READ_ERROR;

    memcpy(dest, gFrame->buf + gOffset, n);
    *read = n;
    gOffset += n;

    return 0;
}


namespace EloquentSurveillance {
    /**
     * Motion detection from JPEG image
     */
    class Motion : public HasErrorMessage, public BenchmarksCode, public Debounces {
    public:

        /**
         *
         */
        Motion() :
            _oldSize(0) {
            _run.i = 0;
            _run.numChanges = 0;
            _run.counter = 0;

            memset(_old, 0, sizeof (_old));
            setMinChanges(0.1);
            setMinPixelDiff(10);
            setMinSizeDiff(0.05);
        }

        /**
         *
         * @param value
         */
        void setMinPixelDiff(float value) {
            _config.diff = value;
        }

        /**
         *
         * @param value
         */
        void setMinChanges(float value) {
            _config.numChanges = value;
        }

        /**
         *
         * @param value
         */
        void setMinSizeDiff(float value) {
            _config.sizeThreshold = value;
        }

        /**
         * Detect motion
         *
         * @return
         */
        bool update() {
            int status = 0;
            uint16_t i = 0;

            _run.i = 0;
            _run.numChanges = 0;

            if (!gIsFrame())
                return setErrorMessage("No frame found");

            setErrorMessage("");
            startBenchmark();

            uint16_t frameSize = gFrame->len;

            // test file size
            // if it didn't changed much, probably neither the image contents changed
            if (_config.sizeThreshold) {
                uint16_t threshold = _config.sizeThreshold >= 1 ?
                     _config.sizeThreshold : _config.sizeThreshold * gFrame->len;

                verbose("old size = ", _oldSize, ", new size = ", gFrame->len, ", thresh = ", threshold, ", diff = ", absdiff(frameSize, _oldSize));

                if (absdiff(frameSize, _oldSize) < threshold) {
                    _oldSize = frameSize;
                    endBenchmark();

                    return false;
                }
            }

            _oldSize = frameSize;

            if (!gIsFrame())
                return setErrorMessage("No frame found");

            // decode jpeg
            if (pjpeg_decode_init(&_image, pjpeg_need_bytes_callback, NULL, 1))
                return setErrorMessage("JPEG init error");

            while ((status = pjpeg_decode_mcu()) != PJPG_NO_MORE_BLOCKS) {
                switch (_image.m_scanType) {
                    case PJPG_GRAYSCALE:
                        detectPixelChange(_image.m_pMCUBufR[0]);
                        break;
                    case PJPG_YH1V1:
                        detectPixelChange(_image.m_pMCUBufR[0]);
                        break;
                    case PJPG_YH1V2:
                        detectPixelChange(_image.m_pMCUBufR[0]);
                        detectPixelChange(_image.m_pMCUBufR[128]);
                        break;
                    case PJPG_YH2V1:
                        detectPixelChange(_image.m_pMCUBufR[0]);
                        detectPixelChange(_image.m_pMCUBufR[64]);
                        break;
                    case PJPG_YH2V2:
                        detectPixelChange(_image.m_pMCUBufR[0]);
                        detectPixelChange(_image.m_pMCUBufR[64]);
                        detectPixelChange(_image.m_pMCUBufR[128]);
                        detectPixelChange(_image.m_pMCUBufR[192]);
                        break;
                }
            }

            uint16_t changesThreshold = _config.numChanges >= 1 ? _config.numChanges : _config.numChanges * _run.i;

            endBenchmark();
            verbose("num changes = ", _run.numChanges, ", threshold = ", changesThreshold);

            if (!debounced()) {
                _oldSize = gFrame->len;

                return setErrorMessage("Too many updates");
            }

            if (_run.numChanges >= changesThreshold) {
                _run.counter += 1;
                touch();

                // skip first detection
                return _run.counter > 1;
            }

            return false;
        }

        /**
         *
         * @return
         */
        uint16_t getNumChanges() {
            return _run.numChanges;
        }

        /**
         *
         * @return
         */
        size_t getCount() {
            return _run.counter;
        }

    protected:
        struct {
            float numChanges;
            float diff;
            float sizeThreshold;
        } _config;
        struct {
            uint16_t i;
            uint16_t numChanges;
            size_t counter;
        } _run;
        uint16_t _oldSize;
        pjpeg_image_info_t _image;
        uint8_t _old[1600 * 1200 / 64];


        /**
         * Difference without overflow
         *
         * @param a
         * @param b
         * @return
         */
        uint16_t absdiff(uint16_t a, uint16_t b) {
            return a > b ? a - b : b - a;
        }

        /**
         *
         * @param p
         */
        void detectPixelChange(uint8_t p) {
            uint16_t thresh = _config.diff >= 1 ? _config.diff : _config.diff * p;

            if (absdiff(p, _old[_run.i++]) > thresh)
                _run.numChanges += 1;

            _old[_run.i - 1] = p;
        }
    };
}

#endif //ELOQUENTSURVEILLANCE_MOTION_H
