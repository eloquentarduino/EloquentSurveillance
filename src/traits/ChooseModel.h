//
// Created by Simone on 07/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_CHOOSEMODEL_H
#define ELOQUENTSURVEILLANCE_CHOOSEMODEL_H

namespace EloquentSurveillance {
    /**
     * Configure pins based on camera model
     */
    class ChooseModel {
    protected:

        /**
         *
         */
        void _aithinker(camera_config_t* config) {
            config->pin_d0 = 5;
            config->pin_d1 = 18;
            config->pin_d2 = 19;
            config->pin_d3 = 21;
            config->pin_d4 = 36;
            config->pin_d5 = 39;
            config->pin_d6 = 34;
            config->pin_d7 = 35;
            config->pin_xclk = 0;
            config->pin_pclk = 22;
            config->pin_vsync = 25;
            config->pin_href = 23;
            config->pin_sscb_sda = 26;
            config->pin_sscb_scl = 27;
            config->pin_pwdn = 32;
            config->pin_reset = -1;
        }

        /**
         *
         */
        void _m5(camera_config_t* config) {
            config->pin_d0 = 32;
            config->pin_d1 = 35;
            config->pin_d2 = 34;
            config->pin_d3 = 5;
            config->pin_d4 = 39;
            config->pin_d5 = 18;
            config->pin_d6 = 36;
            config->pin_d7 = 19;
            config->pin_xclk = 27;
            config->pin_pclk = 21;
            config->pin_vsync = 22;
            config->pin_href = 26;
            config->pin_sscb_sda = 25;
            config->pin_sscb_scl = 23;
            config->pin_pwdn = -1;
            config->pin_reset = 15;
        }

        /**
         *
         */
        void _m5wide(camera_config_t* config) {
            config->pin_d0 = 32;
            config->pin_d1 = 35;
            config->pin_d2 = 34;
            config->pin_d3 = 5;
            config->pin_d4 = 39;
            config->pin_d5 = 18;
            config->pin_d6 = 36;
            config->pin_d7 = 19;
            config->pin_xclk = 27;
            config->pin_pclk = 21;
            config->pin_vsync = 25;
            config->pin_href = 26;
            config->pin_sscb_sda = 22;
            config->pin_sscb_scl = 23;
            config->pin_pwdn = -1;
            config->pin_reset = 15;
        }

        /**
         *
         */
        void _eye(camera_config_t* config) {
            config->pin_d0 = 34;
            config->pin_d1 = 13;
            config->pin_d2 = 14;
            config->pin_d3 = 35;
            config->pin_d4 = 39;
            config->pin_d5 = 38;
            config->pin_d6 = 37;
            config->pin_d7 = 36;
            config->pin_xclk = 4;
            config->pin_pclk = 25;
            config->pin_vsync = 5;
            config->pin_href = 27;
            config->pin_sscb_sda = 18;
            config->pin_sscb_scl = 23;
            config->pin_pwdn = -1;
            config->pin_reset = -1;
        }

        /**
         *
         */
        void _wrover(camera_config_t* config) {
            config->pin_d0 = 4;
            config->pin_d1 = 5;
            config->pin_d2 = 18;
            config->pin_d3 = 19;
            config->pin_d4 = 36;
            config->pin_d5 = 39;
            config->pin_d6 = 34;
            config->pin_d7 = 35;
            config->pin_xclk = 21;
            config->pin_pclk = 22;
            config->pin_vsync = 25;
            config->pin_href = 23;
            config->pin_sscb_sda = 26;
            config->pin_sscb_scl = 27;
            config->pin_pwdn = -1;
            config->pin_reset = -1;
        }
    };
}

#endif //ELOQUENTSURVEILLANCE_CHOOSEMODEL_H
