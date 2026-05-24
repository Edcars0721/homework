#include <WiFi.h>
#include <WebServer.h>

// WiFi配置（请修改为你的WiFi信息）
const char* WIFI_SSID = "你的WiFi名称";
const char* WIFI_PASSWORD = "你的WiFi密码";

WebServer server(80);  // Web服务器监听80端口
const int ledPin = 2;

// 网页HTML代码
const char* HTML_PAGE = R"HTML(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP32无极调光</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body { text-align: center; padding-top: 50px; font-family: Arial; }
    #slider { width: 80%; max-width: 500px; height: 30px; }
    #value { font-size: 30px; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>LED亮度调节</h1>
  <input type="range" id="slider" min="0" max="255" value="0">
  <div id="value">当前亮度：0</div>

  <script>
    const slider = document.getElementById('slider');
    const valueText = document.getElementById('value');
    
    // 监听滑动条变化
    slider.addEventListener('input', () => {
      const brightness = slider.value;
      valueText.innerText = `当前亮度：${brightness}`;
      // 发送GET请求给ESP32，传递亮度值
      fetch(`/set?brightness=${brightness}`);
    });
  </script>
</body>
</html>
)HTML";

// 处理根路由：返回网页
void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

// 处理亮度设置请求
void handleSetBrightness() {
  if (server.hasArg("brightness")) {
    int brightness = server.arg("brightness").toInt();
    brightness = constrain(brightness, 0, 255);  // 限制范围0~255
    ledcWrite(0, brightness);
    Serial.print("设置亮度：");
    Serial.println(brightness);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  // PWM初始化
  ledcSetup(0, 5000, 8);
  ledcAttachPin(ledPin, 0);
  ledcWrite(0, 0);

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
  server.on("/set", handleSetBrightness);
  
  server.begin();
  Serial.println("Web服务器启动成功");
}

void loop() {
  server.handleClient();  // 处理Web请求
}