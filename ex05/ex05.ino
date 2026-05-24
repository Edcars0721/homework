// 引脚定义
const int touchPin = T0;   // 触摸引脚T0 = GPIO4
const int ledPin = 2;      // LED引脚

// 全局变量
int speedLevel = 1;        // 速度档位：1/2/3档，初始1档（最慢）
int lastTouchState = 0;
const int touchThreshold = 40;
const unsigned long debounceDelay = 50;
unsigned long lastTouchTime = 0;

// 不同档位对应的呼吸步长（档位越高，步长越大，呼吸越快）
const int step1 = 1;   // 1档：步长1
const int step2 = 3;   // 2档：步长3
const int step3 = 6;   // 3档：步长6

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  ledcSetup(0, 5000, 8);  // PWM通道0，频率5kHz，分辨率8位（0~255）
  ledcAttachPin(ledPin, 0);  // LED引脚绑定到PWM通道0
}

void loop() {
  // ========== 1. 触摸检测+档位切换 ==========
  int currentTouchValue = touchRead(touchPin);
  int currentTouchState = (currentTouchValue < touchThreshold) ? 1 : 0;
  unsigned long currentTime = millis();

  // 边缘检测+防抖，切换档位：1→2→3→1循环
  if (currentTouchState == 1 && lastTouchState == 0 && (currentTime - lastTouchTime) > debounceDelay) {
    speedLevel = speedLevel % 3 + 1;
    lastTouchTime = currentTime;
    Serial.print("切换到档位：");
    Serial.println(speedLevel);
  }
  lastTouchState = currentTouchState;

  // ========== 2. 呼吸灯逻辑 ==========
  int currentStep;
  // 根据档位选择步长
  switch(speedLevel) {
    case 1: currentStep = step1; break;
    case 2: currentStep = step2; break;
    case 3: currentStep = step3; break;
    default: currentStep = step1;
  }

  // 亮度从0→255递增
  for (int brightness = 0; brightness <= 255; brightness += currentStep) {
    ledcWrite(0, brightness);
    delay(10);
  }
  // 亮度从255→0递减
  for (int brightness = 255; brightness >= 0; brightness -= currentStep) {
    ledcWrite(0, brightness);
    delay(10);
  }
}
