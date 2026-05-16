// 定义LED引脚
const int ledPin = 2;  
// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

void setup() {
  Serial.begin(115200);
  // 新版用法：直接将引脚、频率和分辨率绑定
  ledcAttach(ledPin, freq, resolution);
}

void loop() {
  // 逐渐变亮
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(ledPin, dutyCycle);   
    delay(10);
  }
  // 逐渐变暗
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(ledPin, dutyCycle);   
    delay(10);
  }
  
  Serial.println("Breathing cycle completed");
}