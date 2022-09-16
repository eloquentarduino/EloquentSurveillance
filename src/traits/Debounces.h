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
            _debounce = (size_t) (value < 10000 ? 10000 : value);
        }

    protected:
        size_t _lastTick = 0;
        size_t _debounce = 10000;

        /**
         * Test if debounce time elapsed
         *
         * @return
         */
        bool debounced() {
            return millis() - _lastTick > _debounce;
        }

        /**
         * Update last tick
         */
        void touch() {
            _lastTick = millis();
        }
    };
}

#endif //ELOQUENTSURVEILLANCE_DEBOUNCES_H
