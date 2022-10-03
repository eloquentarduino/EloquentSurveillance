//
// Created by Simone on 11/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_DEBOUNCES_H
#define ELOQUENTSURVEILLANCE_DEBOUNCES_H


namespace EloquentSurveillance {
    class Debounces {
    public:

        /**
         * Set debounce interval
         *
         * @param value
         */
        void debounce(size_t value) {
            _debounce = (size_t) (value < 1000 ? 1000 : value);
        }

        /**
         * Get time elapsed since last event
         *
         * @return
         */
        inline size_t elapsed() {
            return millis() - _lastTick;
        }

    protected:
        size_t _lastTick = 0;
        size_t _debounce = 3000;

        /**
         * Test if debounce time elapsed
         *
         * @return
         */
        bool debounced() {
            return _lastTick == 0 || elapsed() > _debounce;
        }

        /**
         * Update last tick
         * @return true
         */
        bool touch() {
            _lastTick = millis();

            return true;
        }
    };
}

#endif //ELOQUENTSURVEILLANCE_DEBOUNCES_H
