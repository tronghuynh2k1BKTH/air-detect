#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int counter = 1;  // Thêm biến đếm

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcd1234-1234-1234-1234-abcdef123456"

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    counter = 1;  // Reset counter khi có kết nối mới
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32BLE");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("BLE service started");
}

void loop() {
  if (deviceConnected) {
    char message[10];
    sprintf(message, "%d", counter);  // Chuyển số thành chuỗi
    
    pCharacteristic->setValue(message);
    pCharacteristic->notify();
    Serial.print("Sent: ");
    Serial.println(message);
    
    counter++;  // Tăng biến đếm
    delay(1000000);  // Gửi mỗi 1 giây thay vì 5 giây
  }
}