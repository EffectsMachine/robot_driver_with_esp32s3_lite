#include "espasyncbutton.hpp"

#define BUTTON_UP    GPIO_NUM_10
#define BUTTON_DOWN  GPIO_NUM_11
#define BUTTON_LEFT  GPIO_NUM_12
#define BUTTON_RIGHT GPIO_NUM_9
#define BUTTON_OK    GPIO_NUM_2

using ESPButton::event_t;

GPIOButton<ESPEventPolicy> bUp(BUTTON_UP, LOW);
GPIOButton<ESPEventPolicy> bDown(BUTTON_DOWN, LOW);
GPIOButton<ESPEventPolicy> bLeft(BUTTON_LEFT, LOW);
GPIOButton<ESPEventPolicy> bRight(BUTTON_RIGHT, LOW);
GPIOButton<ESPEventPolicy> bOK(BUTTON_OK, LOW);

ButtonCallbackMenu menu;

void runMission(String missionName, int intervalTime, int loopTimes);
void buttonEventHandler(event_t e, const EventMsg* m){
    switch(e){
        // Use click event to move cursor Up and Down (just print message)
        case event_t::click :
            // we catch all 'click' events and check which gpio triggered it
            if (m->gpio == BUTTON_UP){
                buttonBuzzer();
                runMission("up", 0, 1);
            } else if (m->gpio == BUTTON_DOWN){
                buttonBuzzer();
                runMission("down", 0, 1);
            } else if (m->gpio == BUTTON_LEFT){
                buttonBuzzer();
                runMission("left", 0, 1);
            } else if (m->gpio == BUTTON_RIGHT){
                buttonBuzzer();
                runMission("right", 0, 1);
            } else if (m->gpio == BUTTON_OK){
                buttonBuzzer();
            }
            break;
    }
}



void menu_init_RD() {
    wifi_mode_t mode;
    esp_err_t err = esp_wifi_get_mode(&mode);

    if (err == ESP_ERR_WIFI_NOT_INIT) {
        Serial.println("WiFi not initialized. Creating default event loop...");
        Serial0.println("WiFi not initialized. Creating default event loop...");
        ESP_ERROR_CHECK(esp_event_loop_create_default());
    } else {
        Serial.println("WiFi already initialized. No need to create default event loop.");
        Serial0.println("WiFi already initialized. No need to create default event loop.");
    }

    ESP_ERROR_CHECK(esp_event_handler_instance_register(EBTN_EVENTS,
        ESP_EVENT_ANY_ID,
        // this lambda will simply translate loop events into btn_callback_t callback function
        [](void* handler_args, esp_event_base_t base, int32_t id, void* event_data){
            menu.handleEvent(ESPButton::int2event_t(id), reinterpret_cast<EventMsg*>(event_data));
        }, 
        NULL, NULL)
    );

    bUp.enableEvent(event_t::click);
    bDown.enableEvent(event_t::click);
    bLeft.enableEvent(event_t::click);
    bRight.enableEvent(event_t::click);
    bOK.enableEvent(event_t::click);

    menu.assign(BUTTON_UP, 0, buttonEventHandler);
    menu.assign(BUTTON_DOWN, 0, buttonEventHandler);
    menu.assign(BUTTON_LEFT, 0, buttonEventHandler);
    menu.assign(BUTTON_RIGHT, 0, buttonEventHandler);

    bUp.enable();
    bDown.enable();
    bLeft.enable();
    bRight.enable();
    bOK.enable();
}