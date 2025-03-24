#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

// #define BUTTON_UP 10
// #define BUTTON_DOWN 11
// #define BUTTON_LEFT 12
// #define BUTTON_RIGHT 9
// #define BUTTON_OK 2

enum Button {
    BUTTON_NONE = 0,
    BUTTON_UP = 10,
    BUTTON_DOWN = 11,
    BUTTON_LEFT = 12,
    BUTTON_RIGHT = 9,
    BUTTON_OK =2
};

typedef void (*ButtonCallback)(Button button);

void initButtons();

// Button checkButton();

void registerButtonCallback(ButtonCallback callback);

#endif