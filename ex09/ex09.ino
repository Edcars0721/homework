#include <WiFi.h>
#include <WebServer.h>

// WiFi配置（请修改为你的WiFi信息）
const char* WIFI_SSID = "你的WiFi名称";
const char* WIFI_PASSWORD = "你的WiFi密码";

WebServer server(80);
const int touchPin = T0;

// 网页HTML代码
const char* HTML_PAGE = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>触摸传感器仪表盘</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body { text-align: center; padding-top: 80px; font-family: Arial; background-color: #f0f0f0; }
    .dashboard { width: 300px; height: 200px; margin: 0 auto; background: white; border-radius: 20px; box-shadow: 0 0 20px rgba(0,0,0,0.1); padding-top: 40px; }
    #value { font-size: 80px; font-weight: bold; color: #2c3e50; }
    .label { font-size: 20px; color: #7f8c8d; margin-top: 10px; }
  </style>
</head>
<body>
  <div class="dashboard">
    <div id="value">0</div>
    <div class="label">触摸传感器数值</div>
  </div>

  <script>
    const valueText = document.getElementById('value');
    // 每200ms拉取一次最新数值
    function updateValue() {
      fetch('/getTouch')
        .then(res => res.text())
        .then(value => {
          valueText.innerText = value;
        });
    }
    // 定时更新
    setInterval(updateValue, 200);
    // 页面加载先执行一次
    updateValue();
  </script>
</body>
</html>
)HTML";

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

// 返回触摸传感器实时数值
void handleGetTouch() {
  int touchValue = touchRead(touchPin);
  server.send(200, "text/plain", String(touchValue));
}

void setup() {
  Serial.begin(115200);

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
  server.on("/getTouch", handleGetTouch);
  
  server.begin();
}

void loop() {
  server.handleClient();
}