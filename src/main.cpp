#include <Arduino.h>
#include <NimBLEDevice.h>


//
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcdefab-1234-1234-1234-abcdefabcdef"

NimBLECharacteristic* pCharacteristic;

class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
        Serial.println("Client verbunden");
    }

    void onDisconnect(NimBLEServer* pServer) {
        Serial.println("Client getrennt");
        NimBLEDevice::startAdvertising();
    }
};

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        Serial.print("Empfangen: ");
        Serial.println(value.c_str());
    }
};

void setup() {
    Serial.begin(115200);

    NimBLEDevice::init("ESP32-BLE-SERVER");

    NimBLEServer* pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    NimBLEService* pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ |
        NIMBLE_PROPERTY::WRITE |
        NIMBLE_PROPERTY::NOTIFY
    );

    pCharacteristic->setValue("Initial Value");
    pCharacteristic->setCallbacks(new CharacteristicCallbacks());

    pService->start();

    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("\nBLE Server gestartet");
}

void loop() {
    static int counter = 0;

    String value = "Counter: " + String(counter++);
    pCharacteristic->setValue(value.c_str());
    pCharacteristic->notify();

    delay(2000);
    Serial.print('.');
}