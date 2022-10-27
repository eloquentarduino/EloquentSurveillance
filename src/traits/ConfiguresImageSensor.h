//
// Created by Simone on 27/10/22.
//

#ifndef ELOQUENTSURVEILLANCE_CONFIGURESIMAGESENSOR_H
#define ELOQUENTSURVEILLANCE_CONFIGURESIMAGESENSOR_H


namespace EloquentSurveillance {
    class ConfiguresImageSensor {
    public:

        /**
         * Run function on image sensor for configuration
         *
         * @tparam Callback
         * @param callback
         */
        template<typename Callback>
        bool configureSensor(Callback callback) {
            if (_sensor != NULL) {
                callback(_sensor);

                return true;
            }

            return false;
        }

        /**
         * Vertical flip
         *
         * @param flip
         */
        bool vflip(bool flip = true) {
            return configureSensor([flip](sensor_t *sensor) {
                sensor->set_vflip(sensor, flip);
            });
        }

        /**
         * Horizontal mirror
         *
         * @param mirror
         */
        bool hmirror(bool mirror = true) {
            return configureSensor([mirror](sensor_t *sensor) {
                sensor->set_hmirror(sensor, mirror);
            });
        }

        /**
         * Alias for horizontal mirror
         *
         * @param flip
         */
        bool hflip(bool flip = true) {
            return hmirror(flip);
        }

        /**
         *
         * @return
         */
        bool lowBrightness() {
            return setBrightness(-1);
        }

        /**
         *
         * @return
         */
        bool lowestBrightness() {
            return setBrightness(-2);
        }

        /**
         *
         * @return
         */
        bool highBrightness() {
            return setBrightness(1);
        }

        /**
         *
         * @return
         */
        bool highestBrightness() {
            return setBrightness(2);
        }

        /**
         *
         * @param brightness
         */
        bool setBrightness(int8_t brightness) {
            return configureSensor([brightness](sensor_t *sensor) {
                sensor->set_brightness(sensor, brightness);
            });
        }

        /**
         *
         * @return
         */
        bool lowestSaturation() {
            return setSaturation(-2);
        }

        /**
         *
         * @return
         */
        bool lowSaturation() {
            return setSaturation(-1);
        }

        /**
         *
         * @return
         */
        bool highSaturation() {
            return setSaturation(1);
        }

        /**
         *
         * @return
         */
        bool highestSaturation() {
            return setSaturation(2);
        }

        /**
         *
         * @param saturation
         */
        bool setSaturation(int8_t saturation) {
            return configureSensor([saturation](sensor_t *sensor) {
                sensor->set_saturation(sensor, saturation);
            });
        }

        /**
         *
         * @return
         */
        bool enableAutomaticWhiteBalance() {
            return setAutomaticWhiteBalance(true);
        }

        /**
         *
         * @return
         */
        bool disableAutomaticWhiteBalance() {
            return setAutomaticWhiteBalance(false);
        }

        /**
         *
         * @return
         */
        bool setAutomaticWhiteBalance(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_whitebal(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableGainControl() {
            return setGainControl(true);
        }

        /**
         *
         * @return
         */
        bool disableGainControl() {
            return setGainControl(false);
        }

        /**
         *
         * @return
         */
        bool setGainControl(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_gain_ctrl(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableExposureControl() {
            setExposureControl(true);
        }

        /**
         *
         * @return
         */
        bool disableExposureControl() {
            return setExposureControl(false);
        }

        /**
         *
         * @return
         */
        bool setExposureControl(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_exposure_ctrl(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableAutomaticWhiteBalanceGain() {
            return setAutomaticWhiteBalanceGain(true);
        }

        /**
         *
         * @return
         */
        bool disableAutomaticWhiteBalanceGain() {
            return setAutomaticWhiteBalanceGain(false);
        }

        /**
         *
         * @return
         */
        bool setAutomaticWhiteBalanceGain(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_awb_gain(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableAutomaticGainControl() {
            return setAutomaticGainControl(true);
        }

        /**
         *
         * @return
         */
        bool disableAutomaticGainControl() {
            return setAutomaticGainControl(false);
        }

        /**
         *
         * @param enable
         * @return
         */
        bool setAutomaticGainControl(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_gain_ctrl(sensor, enable);
            });
        }

        /**
         *
         * @param gain 0 - 30
         * @return
         */
        bool setManualGain(uint8_t gain) {
            return configureSensor([gain](sensor_t *sensor) {
                sensor->set_agc_gain(sensor, gain);
            });
        }

        /**
         *
         * @param exposure 0 - 1200
         * @return
         */
        bool setManualExposure(uint16_t exposure) {
            return configureSensor([exposure](sensor_t *sensor) {
                sensor->set_aec_value(sensor, exposure);
            });
        }

        /**
         *
         * @return
         */
        bool enableAutomaticExposureControl() {
            return setAutomaticExposureControl(true);
        }

        /**
         *
         * @return
         */
        bool disableAutomaticExposureControl() {
            return setAutomaticExposureControl(false);
        }

        /**
         *
         * @param enable
         * @return
         */
        bool setAutomaticExposureControl(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_aec2(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableDCW() {
            return setDCW(true);
        }

        /**
         *
         * @return
         */
        bool disableDCW() {
            return setDCW(false);
        }

        /**
         *
         * @param enable
         * @return
         */
        bool setDCW(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_dcw(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableBPC() {
            return setBPC(true);
        }

        /**
         *
         * @return
         */
        bool disableBPC() {
            return setBPC(false);
        }

        /**
         *
         * @param enable
         * @return
         */
        bool setBPC(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_bpc(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableWPC() {
            return setWPC(true);
        }

        /**
         *
         * @return
         */
        bool disableWPC() {
            return setWPC(false);
        }

        /**
         *
         * @param enable
         * @return
         */
        bool setWPC(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_wpc(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableLensCorrection() {
            return setLensCorrection(true);
        }

        /**
         *
         * @return
         */
        bool disableLensCorrection() {
            return setLensCorrection(false);
        }

        /**
         *
         * @param enable
         * @return
         */
        bool setLensCorrection(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_lenc(sensor, enable);
            });
        }

        /**
         *
         * @return
         */
        bool enableRawGamma() {
            return setRawGamma(true);
        }

        /**
         *
         * @return
         */
        bool disableRawGamma() {
            return setRawGamma(false);
        }

        /**
         *
         * @param enable
         * @return
         */
        bool setRawGamma(bool enable) {
            return configureSensor([enable](sensor_t *sensor) {
                sensor->set_raw_gma(sensor, enable);
            });
        }

        /**
         *
         * @param ceiling 0 - 6
         * @return
         */
        bool setGainCeiling(uint8_t ceiling) {
            return configureSensor([ceiling](sensor_t *sensor) {
                sensor->set_gainceiling(sensor, (gainceiling_t) ceiling);
            });
        }

    protected:
        sensor_t *_sensor;
    };
}

#endif //ELOQUENTSURVEILLANCE_CONFIGURESIMAGESENSOR_H
