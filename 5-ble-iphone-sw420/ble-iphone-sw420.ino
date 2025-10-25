#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int counterMessage = 1;  // Thêm biến đếm

// sw420
const int sensorPin = 7;  // Chân kết nối SW-420 (OUT)
bool lastState = HIGH;    // Trạng thái trước đó
bool currentState;        // Trạng thái hiện tại
int countSensor = 0;

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcd1234-1234-1234-1234-abcdef123456"

// #define SERVICE_UUID        "0000THIEF-0000-1000-8000-00805F9B34FB"
// #define CHARACTERISTIC_UUID "0000MOVE-0000-1000-8000-00805F9B34FB"


class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    counterMessage = 1;  // Reset counter khi có kết nối mới
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  Serial.begin(115200);

  // sw420
  pinMode(sensorPin, INPUT_PULLUP);
  delay(1000);  // Chờ ổn định
  Serial.println("ESP32-C3 SW-420 Ready");
  Serial.println("---------------------");

  // BLE
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

// SW420
void loop() {
  currentState = digitalRead(sensorPin);

  // Chỉ in ra khi trạng thái thay đổi (từ KHÔNG RUNG sang RUNG)
  if (currentState != lastState) {
    if (currentState == LOW) {
      countSensor++;
      // Serial.println("RUNG LẮC PHÁT HIỆN!: " + String(countSensor));
      if (deviceConnected) {
        char message[50];
        // sprintf(message, "%d", counterMessage);  // Chuyển số thành chuỗi
        // sprintf(message, "Somebody touch your devices %d", counterMessage);
        snprintf(message, sizeof(message), "Somebody touch your devices %d", counterMessage);
        
        pCharacteristic->setValue(message);
        pCharacteristic->notify();
        Serial.print("Sent: ");
        Serial.println(message);
        
        counterMessage++;
      }
    }
    lastState = currentState;  // Cập nhật trạng thái
  }

  delay(50);  // Giảm nhiễu
}