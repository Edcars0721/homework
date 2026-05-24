// 引脚定义
const int ledA_Pin = 14;  // 灯A引脚
const int ledB_Pin = 27;  // 灯B引脚

void setup() {
  Serial.begin(115200);
  // 初始化两个独立PWM通道
  ledcSetup(1, 5000, 8);  // 通道1：灯A
  ledcSetup(2, 5000, 8);  // 通道2：灯B
  // 引脚绑定PWM通道
  ledcAttachPin(ledA_Pin, 1);
  ledcAttachPin(ledB_Pin, 2);
}

void loop() {
  // 灯A从0→255变亮，同时灯B从255→0变暗（反相渐变）
  for (int i = 0; i <= 255; i++) {
    ledcWrite(1, i);       // 灯A占空比i
    ledcWrite(2, 255 - i); // 灯B占空比255-i
    delay(5);  // 调节这个数值可以改变渐变速度
  }

  // 灯A从255→0变暗，同时灯B从0→255变亮
  for (int i = 255; i >= 0; i--) {
    ledcWrite(1, i);
    ledcWrite(2, 255 - i);
    delay(5);
  }
}
