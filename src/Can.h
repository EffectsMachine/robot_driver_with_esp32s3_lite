#include <SPI.h>
#include <ESP32-TWAI-CAN.hpp>

// Default for ESP32
#define CAN_TX 14
#define CAN_RX 13

CanFrame rxFrame;

bool sendJsonAsCAN(const JsonDocument &docData) {
    // check json
    if (!docData["id"].is<uint32_t>() || !docData["data"].is<JsonArray>()) {
        msg("[ERROR] JSON missing id or data key");
        return false;
    }

    uint32_t canId = docData["id"].as<uint32_t>();
    bool isExtended = docData["ext"].is<bool>() ? docData["ext"].as<bool>() : false;

    JsonArrayConst dataArray = docData["data"];
    if (dataArray.isNull()) {
        msg("[ERROR] data error");
        return false;
    }

    // send
    twai_message_t frame = {};
    frame.identifier = canId;
    frame.extd = isExtended;
    frame.data_length_code = dataArray.size();
    if (frame.data_length_code > 8) {
        Serial.println("[ERROR] data over 8");
        return false;
    }

    for (uint8_t i = 0; i < frame.data_length_code; i++) {
        frame.data[i] = dataArray[i].as<uint8_t>();
    }

    // ESP-IDF API send
    esp_err_t result = twai_transmit(&frame, pdMS_TO_TICKS(100)); // overtime 100ms
    if (result != ESP_OK) {
        Serial.printf("[ERROR] CAN send failed, code: %d\n", result);
        return false;
    }

    Serial.print("[SEND] ID: 0x");
    Serial.print(canId, HEX);
    Serial.print(" Data: ");
    for (uint8_t i = 0; i < frame.data_length_code; i++) {
        Serial.print(frame.data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    return true;
}

bool receiveCANasJson() {
    twai_message_t rxFrame;
    // timeout = 0 ticks
    esp_err_t result = twai_receive(&rxFrame, 0); 
    if (result != ESP_OK) {
        return false;
    }

    // create JSON
    jsonFeedback.clear();
    jsonFeedback["T"] = CMD_CAN_SEND;
    jsonFeedback["id"] = rxFrame.identifier;
    jsonFeedback["ext"] = rxFrame.extd;

    JsonArray dataArray = jsonFeedback["data"].to<JsonArray>();
    for (uint8_t i = 0; i < rxFrame.data_length_code; i++) {
        dataArray.add(rxFrame.data[i]);
    }

    serializeJson(jsonFeedback, outputString);
    msg(outputString);
    ws.textAll(outputString);

    return true;
}

void CanStart() {
  // Set pins
  ESP32Can.setPins(CAN_TX, CAN_RX);

  // You can set custom size for the queues - those are default
  ESP32Can.setRxQueueSize(5);
  ESP32Can.setTxQueueSize(5);

  // .setSpeed() and .begin() functions require to use TwaiSpeed enum,
  // but you can easily convert it from numerical value using .convertSpeed()
  ESP32Can.setSpeed(ESP32Can.convertSpeed(500));

  // You can also just use .begin()..
  if (ESP32Can.begin()) {
    msg("CAN bus started!");
  } else {
    msg("CAN bus failed!");
  }

  // or override everything in one command;
  // It is also safe to use .begin() without .end() as it calls it internally
  if (ESP32Can.begin(ESP32Can.convertSpeed(500), CAN_TX, CAN_RX, 10, 10)) {
    msg("CAN bus started!");
  } else {
    msg("CAN bus failed!");
  }
}