//
// Created by Simone on 08/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_GLOBALS_H
#define ELOQUENTSURVEILLANCE_GLOBALS_H

#include <esp_camera.h>
#include <WiFi.h>


uint16_t gOffset = 0;
camera_fb_t *gFrame;



/**
 * Test is there's a frame in memory
 * @return
 */
bool gIsFrame() {
    return gFrame != NULL && gFrame->len > 0;
}


/**
 * Capture frame
 */
bool gCapture() {
    // release old frame first
    if (gFrame) {
        esp_camera_fb_return(gFrame);
        gFrame = NULL;
    }

    gOffset = 0;
    gFrame = esp_camera_fb_get();

    return gIsFrame();
}

/**
 * Release frame
 */
void gFree() {
    if (gFrame) {
        esp_camera_fb_return(gFrame);
        gFrame = NULL;
    }
}


/**
 * Get remaining size to consume
 * @return
 */
uint16_t gRemaining() {
    return (uint16_t) gFrame->len - (uint16_t) gOffset;
}


#endif //ELOQUENTSURVEILLANCE_GLOBALS_H
