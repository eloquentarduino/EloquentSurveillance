//
// Created by Simone on 29/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_REGION_H
#define ELOQUENTSURVEILLANCE_REGION_H

namespace EloquentSurveillance {
    /**
     * Region of interest for motion detection
     */
    class Region {
    public:
        const uint16_t x;
        const uint16_t y;
        const uint16_t width;
        const uint16_t height;
        const float numChanges;
    };
}

#endif //ELOQUENTSURVEILLANCE_REGION_H
