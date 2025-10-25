#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Tên dịch vụ và đặc tính BLE
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcd1234-1234-1234-1234-abcdef123456"

BLECharacteristic *pCharacteristic;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32C3_BLE");  // Tên hiển thị khi iPhone tìm

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->setValue("Hello from ESP32");
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  Serial.println("BLE started, waiting for iPhone to connect...");
}

void loop() {
  // Gửi dữ liệu mỗi 3 giây
  pCharacteristic->setValue("Hello from ESP32");
  pCharacteristic->notify();
  delay(3000);
}