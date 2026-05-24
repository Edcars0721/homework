// 引脚定义
const int touchPin = T0;   // 触摸引脚T0 = GPIO4
const int ledPin = 2;      // LED引脚

// 全局变量
bool ledState = false;     // LED状态：false=灭，true=亮
int lastTouchState = 0;    // 上一次触摸状态
const int touchThreshold = 40;  // 触摸触发阈值（低于这个值认为被触摸）
const unsigned long debounceDelay = 50;  // 防抖时间50ms
unsigned long lastTouchTime = 0;  // 上次触摸触发的时间

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // 初始状态LED熄灭
}

void loop() {
  // 1. 读取当前触摸值
  int currentTouchValue = touchRead(touchPin);
  int currentTouchState = (currentTouchValue < touchThreshold) ? 1 : 0;
  unsigned long currentTime = millis();

  // 2. 边缘检测+防抖：只有【上一次未触摸，当前触摸】，且距离上次触发超过防抖时间，才执行动作
  if (currentTouchState == 1 && lastTouchState == 0 && (currentTime - lastTouchTime) > debounceDelay) {
    // 翻转LED状态
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    // 更新触发时间
    lastTouchTime = currentTime;
    Serial.print("触摸触发，LED状态：");
    Serial.println(ledState ? "亮起" : "熄灭");
  }

  // 3. 更新上一次触摸状态
  lastTouchState = currentTouchState;
}