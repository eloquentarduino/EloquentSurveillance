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
#include "./traits/KeepsCount.h"
#include "./Region.h"

#ifndef CAMERA_MAX_RESOLUTION
#define CAMERA_MAX_RESOLUTION 1600L * 1200L
#endif


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
    class Motion : public HasErrorMessage, public BenchmarksCode, public Debounces, public KeepsCount {
    public:
        uint8_t pixels[CAMERA_MAX_RESOLUTION / 64];

        /**
         *
         */
        Motion() :
            _oldSize(0) {
            _run.i = 0;
            _run.numChanges = 0;

            memset(pixels, 0, sizeof (pixels));
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
         * Get image width
         *
         * @return
         */
        inline uint16_t getWidth() {
            return _image.m_width;
        }

        /**
         * Get image height
         *
         * @return
         */
        inline uint16_t getHeight() {
            return _image.m_height;
        }

        /**
         * Detect motion
         *
         * @return
         */
        bool update(bool force = false) {
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
            if (!force && _config.sizeThreshold) {
                uint16_t threshold = _config.sizeThreshold >= 1 ?
                     _config.sizeThreshold : _config.sizeThreshold * gFrame->len;

                verbose("old size = ", _oldSize, ", new size = ", frameSize, ", thresh = ", threshold, ", diff = ", absdiff(frameSize, _oldSize));

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

            if (!force && !debounced()) {
                _oldSize = gFrame->len;
                endBenchmark();

                return setErrorMessage("Too many updates");
            }

            return true;
        }

        /**
         * Force update
         *
         * @return
         */
        bool forceUpdate() {
            return update(true);
        }

        /**
         * Detect if motion triggered on the whole image
         *
         * @return
         */
        bool detect() {
            Region region = {
                    .x = 0,
                    .y = 0,
                    .width = getWidth(),
                    .height = getHeight(),
                    .numChanges = _config.numChanges
            };

            return detect(region);
        }

        /**
         * Detect if motion triggered in ROI
         *
         * @return
         */
        bool detect(Region& region) {
            if (!getWidth() || !getHeight() || region.width * region.height == 0)
                return setErrorMessage("Bad image size");

            const float numChanges = region.numChanges > 0 ? region.numChanges : _config.numChanges;
            const float area = region.width * region.height;
            const uint16_t threshold = (numChanges >= 1 ? numChanges : numChanges * area) / 64;
            const uint16_t W = getWidth() / 8;
            const uint16_t H = getHeight() / 8;
            const uint16_t upperY = (region.y + region.height) / 8;
            const uint16_t upperX = (region.x + region.width) / 8;
            uint16_t changes = 0;

            for (uint16_t y = region.y / 8; y < min(upperY, H); y++) {
                const uint16_t offset = y * W;

                for (uint16_t x = region.x / 8; x < min(upperX, W); x++)
                    if (_changed[offset + x])
                        changes += 1;
            }

            verbose("ROI{.x=", region.x, ", .y=", region.y, ", .width=", region.width, ", .height=", region.height, "} changes = ", changes, ", threshold = ", threshold);

            if (changes >= threshold) {
                incrementCount();

                return touch() && getCount() > 1;
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
         * Generate filename for new image
         *
         * @param prefix
         * @return
         */
        String getNextFilename(String prefix = "/capture_") {
            return prefix + getPersistentCount() + ".jpg";
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
        } _run;
        uint16_t _oldSize;
        pjpeg_image_info_t _image;
        bool _changed[CAMERA_MAX_RESOLUTION / 64];


        /**
         * Difference without overflow
         *
         * @param a
         * @param b
         * @return
         */
        inline uint16_t absdiff(uint16_t a, uint16_t b) {
            return a > b ? a - b : b - a;
        }

        /**
         *
         * @param p
         */
        void detectPixelChange(uint8_t p) {
            uint16_t thresh = _config.diff >= 1 ? _config.diff : _config.diff * p;

            _changed[_run.i] = absdiff(p, pixels[_run.i]) > thresh;
            pixels[_run.i] = p;
            _run.i += 1;
        }
    };
}


#endif //ELOQUENTSURVEILLANCE_MOTION_H
