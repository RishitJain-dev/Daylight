#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS    5
#define TFT_DC    4
#define TFT_RST   9
#define TFT_BL    6

#define ENCODER_A 2
#define ENCODER_B 3

#define BTN_SW1   0
#define BTN_SW2   1

#define BUZZER    7

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const char* items[] = {"SELECT ITEM 1", "SELECT ITEM 2", "SELECT ITEM 3", "PLAY TONE"};
const int totalItems = sizeof(items) / sizeof(items[0]);

int currentIndex = 0;
int lastAState;
bool backlightState = true;

void drawUI(const char* text) {
  tft.fillScreen(ST7789_BLACK);
  tft.setTextColor(ST7789_GREEN);
  tft.setTextSize(3);
  tft.setCursor(20, 60);
  tft.println(text);
}

void setup() {
  Serial.begin(115200);

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(BTN_SW1, INPUT_PULLUP);
  pinMode(BTN_SW2, INPUT_PULLUP);

  pinMode(TFT_BL, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(TFT_BL, HIGH);

  tft.init(170, 320);
  tft.setRotation(1);

  lastAState = digitalRead(ENCODER_A);
  
  drawUI(items[currentIndex]);
}

void loop() {
  int currentAState = digitalRead(ENCODER_A);
  if (currentAState != lastAState && currentAState == LOW) {
    if (digitalRead(ENCODER_B) != currentAState) {
      currentIndex = (currentIndex + 1) % totalItems;
    } else {
      currentIndex = (currentIndex - 1 + totalItems) % totalItems;
    }
    
    tone(BUZZER, 2000, 15); 
    drawUI(items[currentIndex]);
  }
  lastAState = currentAState;

  if (digitalRead(BTN_SW1) == LOW) {
    tone(BUZZER, 1000, 100);
    drawUI("SW1 PRESSED!");
    delay(250);
    drawUI(items[currentIndex]);
  }

  if (digitalRead(BTN_SW2) == LOW) {
    backlightState = !backlightState;
    digitalWrite(TFT_BL, backlightState ? HIGH : LOW);
    
    tone(BUZZER, 1500, 50);
    delay(250);
  }
}