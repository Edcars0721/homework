const int ledPin = 2;

// 存储LED当前状态
bool ledState = LOW;
// 存储上次LED状态改变的时间
unsigned long previousMillis = 0;
// 闪烁间隔：1Hz意味着周期为1000ms，亮500ms，灭500ms
const long interval = 500;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // 获取当前系统运行时间
  unsigned long currentMillis = millis();

  // 检查是否达到了切换LED状态的时间
  if (currentMillis - previousMillis >= interval) {
    // 更新上次状态改变的时间
    previousMillis = currentMillis;

    // 切换LED状态
    ledState = !ledState;
    digitalWrite(ledPin, ledState);

    // 串口输出调试信息
    if (ledState == HIGH) {
      Serial.println("LED ON (millis)");
    } else {
      Serial.println("LED OFF (millis)");
    }
  }
}