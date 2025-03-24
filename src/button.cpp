#include "button.h"

volatile Button currentButton = BUTTON_NONE;
volatile unsigned long lastInterruptTime = 0; // Used for debouncing

// Pointer to the button callback function
ButtonCallback buttonCallback = nullptr;

// Interrupt handler function
void IRAM_ATTR handleButtonInterrupt(Button button) {
    unsigned long interruptTime = millis();
    // Debouncing, 200ms
        currentButton = button;
        if (interruptTime - lastInterruptTime > 600) { // Debouncing, 200ms
            lastInterruptTime = interruptTime;
        } else {
            return; // Ignore the interrupt if it's within the debounce period
        }
        // If a callback function is registered, invoke the callback function
        if (buttonCallback != nullptr) {
            buttonCallback(button);
        }
}

// Interrupt handler functions for each button
void IRAM_ATTR handleUp() { handleButtonInterrupt(static_cast<Button>(BUTTON_UP)); }
void IRAM_ATTR handleDown() { handleButtonInterrupt(static_cast<Button>(BUTTON_DOWN)); }
void IRAM_ATTR handleLeft() { handleButtonInterrupt(static_cast<Button>(BUTTON_LEFT)); }
void IRAM_ATTR handleRight() { handleButtonInterrupt(static_cast<Button>(BUTTON_RIGHT)); }
void IRAM_ATTR handleOk() { handleButtonInterrupt(static_cast<Button>(BUTTON_OK)); }

// Initialize buttons
void initButtons() {
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    pinMode(BUTTON_OK, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON_UP), handleUp, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_DOWN), handleDown, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_LEFT), handleLeft, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT), handleRight, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_OK), handleOk, FALLING);
}

// Register the callback function
void registerButtonCallback(ButtonCallback callback) {
    buttonCallback = callback;
}