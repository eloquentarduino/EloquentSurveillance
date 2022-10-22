//
// Created by Simone on 26/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_KEEPSCOUNT_H
#define ELOQUENTSURVEILLANCE_KEEPSCOUNT_H

#include <Preferences.h>


namespace EloquentSurveillance {
    /**
     * Keep a persistent count that survives resets
     */
    class KeepsCount {
    public:

        /**
         * Get global, persistent count
         *
         * @return
         */
        uint32_t getPersistentCount() {
            initializeCount();

            return _persistentCount;
        }

        /**
         * Get run count
         *
         * @return
         */
        size_t getCount() {
            return _runCount;
        }

        /**
         * Reset persistent count
         */
        void resetPersistenCount() {
            withCountStorage([](Preferences& preferences) {
                preferences.remove("count");
            });
        }

        /**
         * Reset counter when above given value
         *
         * @param upperThreshold
         */
        void resetWhenGreaterThan(uint32_t upperThreshold) {
            _upperThreshold = upperThreshold;
        }

    protected:
        bool _read = false;
        uint32_t _runCount = 0;
        uint32_t _persistentCount = 0;
        uint32_t _upperThreshold = 0;

        /**
         * Read stored count
         */
        void initializeCount() {
            if (_read)
                return;

            withCountStorage([this](Preferences& preferences) {
                this->_persistentCount = preferences.getFloat("count", 0);
                this->_read = true;
            });
        }

        /**
         * Increment current and global counter
         */
        void incrementCount() {
            initializeCount();

            // reset counter above threshold
            if (_upperThreshold > 0 && _runCount >= _upperThreshold)
                _runCount = 0;

            if (_upperThreshold > 0 && _persistentCount >= _upperThreshold)
                _persistentCount = 0;

            _runCount += 1;
            _persistentCount += 1;

            withCountStorage([this](Preferences& preferences) {
                preferences.putFloat("count", this->_persistentCount);
            });
        }

        /**
         * Run callback when preferences object is initialized
         *
         * @tparam Callback
         * @param callback
         */
        template<typename Callback>
        void withCountStorage(Callback callback) {
            Preferences preferences;

            preferences.begin("motion_count", false);
            callback(preferences);
            preferences.end();
        }
    };
}

#endif //ELOQUENTSURVEILLANCE_KEEPSCOUNT_H
