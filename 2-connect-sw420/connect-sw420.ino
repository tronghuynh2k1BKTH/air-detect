/*
 * ESP32-C3 SW-420 Vibration Sensor
 * Chỉ gửi Serial khi phát hiện rung lắc
 */

const int sensorPin = 7;  // Chân kết nối SW-420 (OUT)
bool lastState = HIGH;    // Trạng thái trước đó
bool currentState;        // Trạng thái hiện tại
int countSensor = 0;

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT_PULLUP);
  delay(1000);  // Chờ ổn định
  Serial.println("ESP32-C3 SW-420 Ready");
  Serial.println("---------------------");
}

void loop() {
  currentState = digitalRead(sensorPin);

  // Chỉ in ra khi trạng thái thay đổi (từ KHÔNG RUNG sang RUNG)
  if (currentState != lastState) {
    if (currentState == LOW) {
      countSensor++;
      Serial.println("🛑 RUNG LẮC PHÁT HIỆN!: " + String(countSensor));
    }
    // else {
    //   Serial.println("✅ Đã ổn định (không rung)");
    // }
    lastState = currentState;  // Cập nhật trạng thái
  }

  delay(80);  // Giảm nhiễu
}