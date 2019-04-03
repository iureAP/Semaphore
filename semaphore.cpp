#include "semaphore.hpp"

Semaphore::Semaphore(PinName green_led_pin, PinName yellow_led_pin, PinName red_led_pin)
     :green_led(green_led_pin), 
     yellow_led(yellow_led_pin),
     red_led(red_led_pin){}
  
void Semaphore::turnOnGreenLed(){
    green_led.write(1);     
}

void Semaphore::turnOnYellowLed(){
    yellow_led.write(1);   
}

void Semaphore::turnOnRedLed(){
    red_led.write(1); 
}

void Semaphore::turnOffGreenLed(){
    green_led.write(0); 
}

void Semaphore::turnOffYellowLed(){
    yellow_led.write(0);      
}

void Semaphore::turnOffRedLed(){
    red_led.write(0);    
}

void Semaphore::turnOffAllLeds(){
    turnOffGreenLed();
    turnOffYellowLed();
    turnOffRedLed();
}
/*
void Semaphore::checkLedTime(){
    float on_led_duration = timer.read();
    switch(whatIsLedOn()){
        case GREEN_LED_ON:
            if(on_led_duration >= GREEN_LED_TIME_ON){
                turnOffGreenLed();
            }
            break;
        case YELLOW_LED_ON:
            if(on_led_duration >= YELLOW_LED_TIME_ON){
                turnOffYellowLed();
            }
            break;
        case RED_LED_ON:
            if(on_led_duration >= RED_LED_TIME_ON){
                turnOffRedLed();
            }
            break; 
        default:
            break;
    }    
}
*/
int Semaphore::whatIsLedOn(){
    if(green_led.read())
        return GREEN_LED_ON;
    else{
        if(yellow_led.read())
            return YELLOW_LED_ON;
        else{
            if(red_led.read())
                return RED_LED_ON;
            return ALL_LEDS_OFF;
        } 
    }
}

/*void Semaphore::blinkYellowLed(){
    if(timer_yellow.read() > TOGGLE_TIME){
        if(yellow_led.read())
            turnOffYellowLed();
        else
            turnOnYellowLed();
        timer_yellow.stop();
        timer_yellow.reset();
    }
    
}

void Semaphore::enableAlertState(){
    timer_yellow.start();
}

void Semaphore::disableAlertState(){
    timer_yellow.stop();
    timer_yellow.reset();
}*/                
