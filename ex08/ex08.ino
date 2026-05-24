#include <WiFi.h>
#include <WebServer.h>

// WiFi配置（请修改为你的WiFi信息）
const char* WIFI_SSID = "你的WiFi名称";
const char* WIFI_PASSWORD = "你的WiFi密码";

WebServer server(80);
const int touchPin = T0;
const int ledPin = 2;

// 系统状态变量
bool isArmed = false;     // 布防状态：false=撤防，true=布防
bool isAlarm = false;     // 报警状态：false=正常，true=报警
const int touchThreshold = 40;
unsigned long lastBlinkTime = 0;
const long blinkInterval = 100;  // 报警闪烁间隔100ms
bool blinkState = false;

// 网页HTML代码
const char* HTML_PAGE = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>安防报警器</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body { text-align: center; padding-top: 50px; font-family: Arial; }
    button { width: 200px; height: 60px; font-size: 20px; margin: 20px; border: none; border-radius: 10px; cursor: pointer; }
    #armBtn { background-color: #e74c3c; color: white; }
    #disarmBtn { background-color: #2ecc71; color: white; }
    #status { font-size: 24px; margin-top: 30px; }
  </style>
</head>
<body>
  <h1>安防报警系统</h1>
  <button id="armBtn">布防</button>
  <button id="disarmBtn">撤防</button>
  <div id="status">当前状态：已撤防</div>

  <script>
    const statusText = document.getElementById('status');
    
    document.getElementById('armBtn').addEventListener('click', () => {
      fetch('/arm');
      statusText.innerText = '当前状态：已布防';
    });

    document.getElementById('disarmBtn').addEventListener('click', () => {
      fetch('/disarm');
      statusText.innerText = '当前状态：已撤防';
    });
  </script>
</body>
</html>
)HTML";

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

// 布防处理
void handleArm() {
  isArmed = true;
  isAlarm = false;
  digitalWrite(ledPin, LOW);
  Serial.println("系统已布防");
  server.send(200, "text/plain", "布防成功");
}

// 撤防处理
void handleDisarm() {
  isArmed = false;
  isAlarm = false;
  digitalWrite(ledPin, LOW);
  Serial.println("系统已撤防，报警解除");
  server.send(200, "text/plain", "撤防成功");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // 连接WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！");
  Serial.print("ESP32 IP地址：");
  Serial.println(WiFi.localIP());

  // 注册路由
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long currentTime = millis();

  // 1. 布防状态下检测触摸触发报警
  if (isArmed && !isAlarm) {
    int touchValue = touchRead(touchPin);
    if (touchValue < touchThreshold) {
      isAlarm = true;
      Serial.println("触发报警！");
    }
  }

  // 2. 报警状态下LED高频闪烁（非阻塞，不影响Web服务）
  if (isAlarm) {
    if (currentTime - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentTime;
      blinkState = !blinkState;
      digitalWrite(ledPin, blinkState ? HIGH : LOW);
    }
  }
}