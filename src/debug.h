//
// Created by Simone on 07/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_DEBUG_H
#define ELOQUENTSURVEILLANCE_DEBUG_H


/**
 * Simple debugger
 *
 * @tparam T
 * @param stream
 * @param message
 */
template<typename T>
void debug(String stream, T message) {
    Serial.print('[');
    Serial.print(stream);
    Serial.print(']');
    Serial.print(' ');
    Serial.println(message);
}


/**
 * Verbose stop condition
 */
void verbose() {
#ifdef VERBOSE
    Serial.println();
    Serial.flush();
    delay(10);
#endif
}

/**
 * Simple debugger
 *
 * @tparam T
 * @param stream
 * @param message
 */
template<typename T, typename... Args>
void verbose(T message, Args... args) {
#ifdef VERBOSE
    Serial.print(message);
    verbose(args...);
#endif
}


#endif //ELOQUENTSURVEILLANCE_DEBUG_H
