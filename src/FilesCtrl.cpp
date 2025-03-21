#include "FilesCtrl.h"
#include <nvs_flash.h>
#include <esp_system.h>
#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

JsonDocument cmdJson;

void FilesCtrl::init() {
    if (!flashStatus) {
        if (nvs_flash_init() == ESP_OK) {
            Serial.println("nvs_flash_init() success");
            Serial0.println("nvs_flash_init() success");
        } else {
            Serial.println("nvs_flash_init() failed");
            Serial0.println("nvs_flash_init() failed");
        }
        if (LittleFS.begin()) {
            Serial.println("LittleFS.begin() success");
            Serial0.println("LittleFS.begin() success");
        } else {
            Serial.println("LittleFS.begin() failed");
            Serial0.println("LittleFS.begin() failed");
        }
        flashStatus = true;
    }
}

void FilesCtrl::flash() {
    if (flashStatus) {
        LittleFS.format();
        Serial.println("=== LittleFS formatted ===");
        Serial0.println("=== LittleFS formatted ===");
    }
}

void FilesCtrl::scan() {
    if (flashStatus) {
        File root = LittleFS.open("/");
        File file = root.openNextFile();
        Serial.println("=== Scaning Files ===");
        Serial0.println("=== Scaning Files ===");
        while (file) {
            Serial.print("file name: ");
            Serial.print(file.name());
            Serial.print(" file size: ");
            Serial.println(file.size());
            Serial0.print("file name: ");
            Serial0.print(file.name());
            Serial0.print(" file size: ");
            Serial0.println(file.size());
            file = root.openNextFile();
        }
    }
}

void FilesCtrl::createMission(String missionName, String contentInput) {
    if (flashStatus) {
        if (LittleFS.exists("/" + missionName + ".mission")) {
            Serial.println("file already exists");
            Serial0.println("file already exists");
            return;
        }
        File createFile = LittleFS.open("/" + missionName + ".mission", FILE_WRITE);
        createFile.close();
        File file = LittleFS.open("/" + missionName + ".mission", FILE_APPEND);
        if (file) {
            if(file.println(contentInput) == 0){
                Serial.println("file write failed");
                Serial0.println("file write failed");
            } else {
                Serial.println("file write succeed");
                Serial0.println("file write succeed");
            }
            file.close();
            Serial.println("file created");
            Serial0.println("file created");
        } else {
            Serial.println("file creation failed");
            Serial0.println("file creation failed");
        }
    }
}

void FilesCtrl::missionContent(String missionName) {
    File file = LittleFS.open("/" + missionName + ".mission", "r");
    if (!file) {
        Serial.println("file not found.");
        Serial0.println("file not found.");
        return;
    }
    String firstLine = file.readStringUntil('\n');
    Serial.println("=== File Content ===");
    Serial.print("File Name: ");
    Serial.println(missionName);
    Serial.print("Intro: ");
    Serial.println(firstLine);
    Serial0.println("=== File Content ===");
    Serial0.print("File Name: ");
    Serial0.println(missionName);
    Serial0.print("Intro: ");
    Serial0.println(firstLine);
    int _lineNum = 0;
    while (file.available()) {
		_lineNum++;
		String line = file.readStringUntil('\n');
		Serial.print("[StepNum: ");Serial.print(_lineNum);Serial.print(" ] - ");
		Serial.println(line);
        Serial0.print("[StepNum: ");Serial.print(_lineNum);Serial.print(" ] - ");
		Serial0.println(line);
    }
    file.close();
}

void FilesCtrl::appendStep(String missionName, String jsonCmd) {
    if (!LittleFS.exists("/" + missionName + ".mission")) {
        Serial.println("file not found.");
        Serial0.println("file not found.");
        return;
    }
    DeserializationError err = deserializeJson(cmdJson, jsonCmd);
	if (err == DeserializationError::Ok) {
		Serial.println("json parsing succeed.");
        Serial0.println("json parsing succeed.");
        File file = LittleFS.open("/" + missionName + ".mission", "a");
        if(!file){
            Serial.println("Error opening file for appending.");
            Serial0.println("Error opening file for appending.");
            return;
        }
        file.println(jsonCmd);
        file.close();
        Serial.println("=== Edited File ===");
        Serial0.println("=== Edited File ===");
        FilesCtrl::missionContent(missionName);
		cmdJson.clear();
	} else {
		cmdJson.clear();
		Serial.println("[deserializeJson err]");
        Serial0.println("[deserializeJson err]");
	}
}

void FilesCtrl::insertStep(String missionName, int stepNum, String jsonCmd) {
    if (!LittleFS.exists("/" + missionName + ".mission")) {
        Serial.println("file not found.");
        Serial0.println("file not found.");
        return;
    }
    DeserializationError err = deserializeJson(cmdJson, jsonCmd);
    if (err == DeserializationError::Ok) {
        Serial.println("json parsing succeed.");
        Serial0.println("json parsing succeed.");
        File file = LittleFS.open("/" + missionName + ".mission", "r");
        if(!file){
        Serial.println("Error opening file for inserting.");
        Serial0.println("Error opening file for inserting.");
        return;
        }
        String _content = "";
        _content += file.readStringUntil('\n') + "\n";
        for (int i = 0; i < stepNum - 1; i++) {
            _content += file.readStringUntil('\n') + "\n";
        }
        _content += jsonCmd + "\n";
        while (file.available()) {
            _content += file.readStringUntil('\n') + "\n";
        }
        file.close();
        file = LittleFS.open("/" + missionName + ".mission", "w");
        if(!file){
        Serial.println("Error opening file for inserting.");
        Serial0.println("Error opening file for inserting.");
        return;
        }
        file.print(_content);
        file.close();
        Serial.println("=== Edited File ===");
        Serial0.println("=== Edited File ===");
        FilesCtrl::missionContent(missionName);
        cmdJson.clear();
    } else {
        cmdJson.clear();
        Serial.println("[deserializeJson err]");
        Serial0.println("[deserializeJson err]");
    }
}

void FilesCtrl::replaceStep(String missionName, int stepNum, String jsonCmd) {
    if (!LittleFS.exists("/" + missionName + ".mission")) {
        Serial.println("file not found.");
        Serial0.println("file not found.");
        return;
    }
    if (stepNum < 1) {
        Serial.println("stepNum should be greater than 0.");
        Serial0.println("stepNum should be greater than 0.");
        return;
    }
    DeserializationError err = deserializeJson(cmdJson, jsonCmd);
    if (err == DeserializationError::Ok) {
        Serial.println("json parsing succeed.");
        Serial0.println("json parsing succeed.");
        File file = LittleFS.open("/" + missionName + ".mission", "r");
        if(!file){
        Serial.println("Error opening file for replacing.");
        Serial0.println("Error opening file for replacing.");
        return;
        }
        String _content = "";
        _content += file.readStringUntil('\n') + "\n";
        for (int i = 0; i < stepNum - 1; i++) {
            _content += file.readStringUntil('\n') + "\n";
        }
        _content += jsonCmd + "\n";
        file.readStringUntil('\n');
        while (file.available()) {
            _content += file.readStringUntil('\n') + "\n";
        }
        file.close();
        file = LittleFS.open("/" + missionName + ".mission", "w");
        if(!file){
        Serial.println("Error opening file for replacing.");
        Serial0.println("Error opening file for replacing.");
        return;
        }
        file.print(_content);
        file.close();
        Serial.println("=== Edited File ===");
        Serial0.println("=== Edited File ===");
        FilesCtrl::missionContent(missionName);
        cmdJson.clear();
    } else {
        cmdJson.clear();
        Serial.println("[deserializeJson err]");
        Serial0.println("[deserializeJson err]");
    }
}

void FilesCtrl::deleteStep(String missionName, int stepNum) {
    if (!LittleFS.exists("/" + missionName + ".mission")) {
        Serial.println("file not found.");
        Serial0.println("file not found.");
        return;
    }
    if (stepNum < 1) {
        Serial.println("stepNum should be greater than 0.");
        Serial0.println("stepNum should be greater than 0.");
        return;
    }
    File file = LittleFS.open("/" + missionName + ".mission", "r");
    if(!file){
        Serial.println("Error opening file for deleting.");
        Serial0.println("Error opening file for deleting.");
        return;
    }
    String _content = "";
    _content += file.readStringUntil('\n') + "\n";
    for (int i = 0; i < stepNum - 1; i++) {
        _content += file.readStringUntil('\n') + "\n";
    }
    file.readStringUntil('\n');
    while (file.available()) {
        _content += file.readStringUntil('\n') + "\n";
    }
    file.close();
    file = LittleFS.open("/" + missionName + ".mission", "w");
    if(!file){
        Serial.println("Error opening file for deleting.");
        Serial0.println("Error opening file for deleting.");
        return;
    }
    file.print(_content);
    file.close();
    Serial.println("=== Edited File ===");
    Serial0.println("=== Edited File ===");
    FilesCtrl::missionContent(missionName);
}

String FilesCtrl::readStep(String missionName, int stepNum) {
    if (!LittleFS.exists("/" + missionName + ".mission")) {
        Serial.println("file not found.");
        Serial0.println("file not found.");
        return "";
    }
    if (stepNum < 1) {
        Serial.println("stepNum should be greater than 0.");
        Serial0.println("stepNum should be greater than 0.");
        return "";
    }
    File file = LittleFS.open("/" + missionName + ".mission", "r");
    if(!file){
        Serial.println("Error opening file for reading.");
        Serial0.println("Error opening file for reading.");
        return "";
    }
    String _content = "";
    file.readStringUntil('\n');
    for (int i = 0; i < stepNum - 1; i++) {
        file.readStringUntil('\n');
    }
    _content += file.readStringUntil('\n');
    file.close();
    return _content;
}

void FilesCtrl::deleteMission(String missionName) {
    if (!LittleFS.exists("/" + missionName + ".mission")) {
        Serial.println("file not found.");
        Serial0.println("file not found.");
        return;
    }
    LittleFS.remove("/" + missionName + ".mission");
    Serial.println("file deleted.");
    Serial0.println("file deleted.");
}

bool FilesCtrl::checkMission(String missionName) {
    if (!LittleFS.exists("/" + missionName + ".mission")) {
        return false;
    }
    return true;
}