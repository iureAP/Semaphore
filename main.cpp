#include "mbed.h"
#include "semaphore.hpp"

DigitalIn button(PA_0);

enum Events{
    PRESS_BUTTON_LESS_OR_EQUAL_THREE_SEC,
    PRESS_BUTTON_BETWEEN_FOUR_AND_TEN_SEC,
    PRESS_BUTTON_LARGER_TEN_SEC,
    TIMER_VALUE_EQUAL_TWENTY_SEC,
    TIMER_VALUE_EQUAL_FOUR_SEC,
    TIMER_VALUE_EQUAL_TEN_SEC,   
};

enum States{
    MOVE_ON,
    ATENTION,
    STOP,
    OFF,
    ALERT,       
};

Events event;
States curent_state;
States next_state;
States previous_state;
bool occurred_event;
Ticker timer_event;
Semaphore semaphore(PD_12, PD_13, PD_14);

void timeout_green_isr(){
    occurred_event = true;
    event = TIMER_VALUE_EQUAL_TWENTY_SEC;
}
 
void timeout_yellow_isr(){
    occurred_event = true;
    event = TIMER_VALUE_EQUAL_FOUR_SEC;
}    

void timeout_red_isr(){
    occurred_event = true;
    event = TIMER_VALUE_EQUAL_TEN_SEC;
}

void alert_isr(){
    if(semaphore.whatIsLedOn() == YELLOW_LED_ON)
        semaphore.turnOffYellowLed();
    else
        semaphore.turnOnYellowLed();
}

int main(){
    semaphore.turnOffAllLeds();
    //semaphore.disableAlertState();
    curent_state = MOVE_ON;
    semaphore.turnOnGreenLed();
    next_state = curent_state;
    occurred_event = false;
    timer_event.attach(&timeout_green_isr, GREEN_LED_TIME_ON);
    Timer press_button;
    bool button_pressed = false;
    
    while(1){
        if(button){
            if(button_pressed == false){
                button_pressed = true;  
                press_button.start();
            }   
        }
        else{
            if(button_pressed){
                if(press_button.read() <= 3) {
                    event = PRESS_BUTTON_LESS_OR_EQUAL_THREE_SEC;
                }
                else{ 
                    if((press_button.read() > 3) && (press_button.read() < 10)) {
                        event = PRESS_BUTTON_BETWEEN_FOUR_AND_TEN_SEC;
                    }
                    else{
                        if(press_button.read() >= 10){
                            event = PRESS_BUTTON_LARGER_TEN_SEC;
                            semaphore.turnOnRedLed();
                        }    
                    }     
                }
                occurred_event = true;
                press_button.reset();
                press_button.stop();
                button_pressed = false;
            }            
        } 
        if(occurred_event){
            switch(curent_state){
                case MOVE_ON:
                    switch(event){
                        case PRESS_BUTTON_LESS_OR_EQUAL_THREE_SEC:
                            next_state = ATENTION;
                            break;
                        case PRESS_BUTTON_BETWEEN_FOUR_AND_TEN_SEC:
                            next_state = ALERT;
                            previous_state = curent_state;
                            break;
                        case PRESS_BUTTON_LARGER_TEN_SEC:
                            next_state = OFF;
                            previous_state = curent_state;
                            break;
                        case TIMER_VALUE_EQUAL_TWENTY_SEC:
                            next_state = ATENTION;
                            break;    
                     }
                     break;
                case ATENTION:
                    switch(event){
                        case TIMER_VALUE_EQUAL_FOUR_SEC:
                            next_state = STOP;
                            break;
                        case PRESS_BUTTON_BETWEEN_FOUR_AND_TEN_SEC:
                            next_state = ALERT;
                            previous_state = curent_state;
                            break;
                        case PRESS_BUTTON_LARGER_TEN_SEC:
                            next_state = OFF;
                            previous_state = curent_state;
                            break;
                    }
                    break;
                case STOP:
                    switch(event){
                        case TIMER_VALUE_EQUAL_TEN_SEC:
                            next_state = MOVE_ON;
                            break;
                        case PRESS_BUTTON_BETWEEN_FOUR_AND_TEN_SEC:
                            next_state = ALERT;
                            previous_state = curent_state;
                            break;
                        case PRESS_BUTTON_LARGER_TEN_SEC:
                            next_state = OFF;
                            previous_state = curent_state;
                            break;             
                    }
                    break;
                case OFF:
                    switch(event){
                        case PRESS_BUTTON_LARGER_TEN_SEC:
                            next_state = previous_state;
                            break;
                    }
                    break;
                case ALERT:
                    switch(event){
                        case PRESS_BUTTON_BETWEEN_FOUR_AND_TEN_SEC:
                            next_state = previous_state;
                            break;
                    }
                    break;
                default: break;    
            }
            switch(next_state){
                case MOVE_ON:
                    semaphore.turnOffAllLeds();
                    semaphore.turnOnGreenLed();
                    timer_event.attach(&timeout_green_isr, GREEN_LED_TIME_ON);
                    break;
                case ATENTION:
                    semaphore.turnOffAllLeds();
                    semaphore.turnOnYellowLed();
                    timer_event.attach(&timeout_yellow_isr, YELLOW_LED_TIME_ON);
                    break;
                case STOP:
                    semaphore.turnOffAllLeds();
                    semaphore.turnOnRedLed();
                    timer_event.attach(&timeout_red_isr, RED_LED_TIME_ON);
                    break;
                case OFF:
                    semaphore.turnOffAllLeds();
                    break;
                case ALERT:
                    semaphore.turnOffAllLeds();
                    semaphore.turnOnYellowLed();
                    timer_event.attach(&alert_isr, 1.0);
                    break;
            }
            curent_state = next_state;
            occurred_event = false;       
        }
        wait_ms(100);
    }
}
