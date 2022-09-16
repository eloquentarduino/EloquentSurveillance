//
// Created by Simone on 09/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_HASERRORMESSAGE_H
#define ELOQUENTSURVEILLANCE_HASERRORMESSAGE_H


namespace EloquentSurveillance {
    class HasErrorMessage {
    public:

        /**
         *
         * @return
         */
        String getErrorMessage() {
            return _error;
        }

        /**
         *
         * @return
         */
        bool isOk() {
            return _error == "";
        }

    protected:
        String _error;

        /**
         *
         * @param error
         */
        bool setErrorMessage(String error) {
            _error = error;

            return isOk();
        }
    };
}

#endif //ELOQUENTSURVEILLANCE_HASERRORMESSAGE_H
