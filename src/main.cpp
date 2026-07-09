#include <Arduino.h>
#include <NimBLEDevice.h>

#include <SPI.h>
//#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp

#include <OneWire.h>
#include <DallasTemperature.h>

//#include <BluetoothSerial.h>
//#include "dspdata.h"

//#include "measure.h"

#include "../../Bord-Monitor-100/src/dspdata.h"

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

#define DS18B20_BUS 0

OneWire oneWire(DS18B20_BUS);
DallasTemperature DS18B20(&oneWire);
// arrays to hold device addresses
DeviceAddress WaterThermometer, AirThermometer;

void setup() {
    Serial.begin(115200);

    DS18B20.begin();

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
    DataUnion data;
    float temp;
    static int counter = 0;

//   String value = "Counter: " + String(counter++);
//  pCharacteristic->setValue((value.c_str());
    
    DS18B20.requestTemperatures();
    temp= DS18B20.getTempCByIndex(0);   
    
    Serial.printf("MotorTemp: %5.1f\n", temp);

    data.Id= DATA_MOTOR_TEMP;
    data.Data.Float= temp;
    pCharacteristic->setValue((uint8_t*)&data, sizeof(DataUnion));
    pCharacteristic->notify();

    delay(2000);
}