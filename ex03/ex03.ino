const int ledPin = 2;

// 定义各种时间间隔（毫秒）
const int SHORT_ON = 200;    // 短亮时间
const int SHORT_OFF = 200;   // 短灭时间
const int LONG_ON = 600;     // 长亮时间
const int LONG_OFF = 200;    // 长灭时间
const int LETTER_GAP = 500;  // 字母间隔
const int WORD_GAP = 2000;   // 单词间隔（SOS结束后）

// 状态机定义（给枚举值加上STATE_前缀，避免与常量重名）
enum State {
  STATE_S_SHORT_ON,
  STATE_S_SHORT_OFF,
  STATE_S_LETTER_GAP,
  STATE_O_LONG_ON,
  STATE_O_LONG_OFF,
  STATE_O_LETTER_GAP,
  STATE_S2_SHORT_ON,
  STATE_S2_SHORT_OFF,
  STATE_WORD_GAP
};

State currentState = STATE_S_SHORT_ON;
unsigned long previousMillis = 0;
int count = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
  Serial.println("SOS signal started");
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentState) {
    case STATE_S_SHORT_ON:
      if (currentMillis - previousMillis >= SHORT_ON) {
        digitalWrite(ledPin, LOW);
        previousMillis = currentMillis;
        currentState = STATE_S_SHORT_OFF;
        count++;
      }
      break;

    case STATE_S_SHORT_OFF:
      if (currentMillis - previousMillis >= SHORT_OFF) {
        if (count < 3) {
          digitalWrite(ledPin, HIGH);
          previousMillis = currentMillis;
          currentState = STATE_S_SHORT_ON;
        } else {
          previousMillis = currentMillis;
          currentState = STATE_S_LETTER_GAP;
          count = 0;
        }
      }
      break;

    case STATE_S_LETTER_GAP:
      if (currentMillis - previousMillis >= LETTER_GAP) {
        digitalWrite(ledPin, HIGH);
        previousMillis = currentMillis;
        currentState = STATE_O_LONG_ON;
      }
      break;

    case STATE_O_LONG_ON:
      if (currentMillis - previousMillis >= LONG_ON) {
        digitalWrite(ledPin, LOW);
        previousMillis = currentMillis;
        currentState = STATE_O_LONG_OFF;
        count++;
      }
      break;

    case STATE_O_LONG_OFF:
      if (currentMillis - previousMillis >= LONG_OFF) {
        if (count < 3) {
          digitalWrite(ledPin, HIGH);
          previousMillis = currentMillis;
          currentState = STATE_O_LONG_ON;
        } else {
          previousMillis = currentMillis;
          currentState = STATE_O_LETTER_GAP;
          count = 0;
        }
      }
      break;

    case STATE_O_LETTER_GAP:
      if (currentMillis - previousMillis >= LETTER_GAP) {
        digitalWrite(ledPin, HIGH);
        previousMillis = currentMillis;
        currentState = STATE_S2_SHORT_ON;
      }
      break;

    case STATE_S2_SHORT_ON:
      if (currentMillis - previousMillis >= SHORT_ON) {
        digitalWrite(ledPin, LOW);
        previousMillis = currentMillis;
        currentState = STATE_S2_SHORT_OFF;
        count++;
      }
      break;

    case STATE_S2_SHORT_OFF:
      if (currentMillis - previousMillis >= SHORT_OFF) {
        if (count < 3) {
          digitalWrite(ledPin, HIGH);
          previousMillis = currentMillis;
          currentState = STATE_S2_SHORT_ON;
        } else {
          previousMillis = currentMillis;
          currentState = STATE_WORD_GAP;
          count = 0;
        }
      }
      break;

    case STATE_WORD_GAP:
      if (currentMillis - previousMillis >= WORD_GAP) {
        digitalWrite(ledPin, HIGH);
        previousMillis = currentMillis;
        currentState = STATE_S_SHORT_ON;
        Serial.println("SOS cycle completed");
      }
      break;
  }
}