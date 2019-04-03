#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "mbed.h"

#define GREEN_LED_TIME_ON 20.0
#define YELLOW_LED_TIME_ON 4.0
#define RED_LED_TIME_ON 10.0

#define GREEN_LED_ON 0
#define YELLOW_LED_ON 1
#define RED_LED_ON 2
#define ALL_LEDS_OFF 3

class Semaphore{
 public:
    Semaphore(PinName green_led_pin, PinName yellow_led_pin, PinName red_led_pin);
    void turnOnGreenLed();
    void turnOnYellowLed();
    void turnOnRedLed();
    void turnOffGreenLed();
    void turnOffYellowLed();
    void turnOffRedLed();
    void turnOffAllLeds();
    void blinkYellowLed();
    void enableAlertState();
    void disableAlertState();
    int whatIsLedOn();

 private:
    DigitalOut green_led;
    DigitalOut yellow_led;
    DigitalOut red_led;
};
#endif
