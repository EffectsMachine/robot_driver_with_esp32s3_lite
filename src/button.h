#include "espasyncbutton.hpp"

// using ESPButton::event_t;

#define BUTTON_UP  GPIO_NUM_10
#define BUTTON_DOWN  GPIO_NUM_11
#define BUTTON_LEFT  GPIO_NUM_12
#define BUTTON_RIGHT GPIO_NUM_9
#define BUTTON_OK    GPIO_NUM_2

// using ESPButton::event_t;

// GPIOButton<ESPEventPolicy> buttonUp(BUTTON_UP, HIGH);
// GPIOButton<ESPEventPolicy> buttonDown(BUTTON_DOWN, HIGH);
// GPIOButton<ESPEventPolicy> buttonLeft(BUTTON_LEFT, HIGH);
// GPIOButton<ESPEventPolicy> buttonRight(BUTTON_RIGHT, HIGH);
// GPIOButton<ESPEventPolicy> buttonOk(BUTTON_OK, HIGH);

AsyncEventButton buttonUp(BUTTON_UP, LOW);
AsyncEventButton buttonDown(BUTTON_DOWN, HIGH);
AsyncEventButton buttonLeft(BUTTON_LEFT, HIGH);
AsyncEventButton buttonRight(BUTTON_RIGHT, HIGH);
AsyncEventButton buttonOk(BUTTON_OK, HIGH);



// ButtonCallbackMenu menu;

// // an example function, pretend it could move a cursor via on-screeen menu
// void cursor_control(event_t e, const EventMsg* m);

// // an example function, pretend it could control sound volume for some device
// void volume_control(event_t e, const EventMsg* m);
// // an example function, it will just show click counts
// void counters(event_t e, const EventMsg* m);
// // this function will toggle menu levels based on special button events
// void menu_toggle();


// #ifndef BUTTON_H
// #define BUTTON_H

// #include <Arduino.h>

// // #define BUTTON_UP 10
// // #define BUTTON_DOWN 11
// // #define BUTTON_LEFT 12
// // #define BUTTON_RIGHT 9
// // #define BUTTON_OK 2

// enum Button {
//     BUTTON_NONE = 0,
//     BUTTON_UP = 10,
//     BUTTON_DOWN = 11,
//     BUTTON_LEFT = 12,
//     BUTTON_RIGHT = 9,
//     BUTTON_OK =2
// };

// typedef void (*ButtonCallback)(Button button);

// void initButtons();

// // Button checkButton();

// void registerButtonCallback(ButtonCallback callback);

// #endif