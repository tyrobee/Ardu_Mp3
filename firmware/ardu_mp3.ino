#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

SoftwareSerial mySerial(10, 11);
DFRobotDFPlayerMini player;

// Buttons
#define BTN_UP 2
#define BTN_DOWN 3
#define BTN_SELECT 4
#define BTN_BACK 5

int volume = 20;
int eq = 3;

int menuIndex = 0;
int settingIndex = 0;

bool inSettings = false;

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  mySerial.begin(9600);

  player.begin(mySerial);
  player.volume(volume);
  player.EQ(eq);
  player.play(1);
}

void drawIcons() {
  display.drawRect(0, 0, 25, 64, WHITE);

  display.setTextSize(2);
  display.setCursor(5, 10);
  display.print("♪");

  display.setCursor(5, 35);
  display.print("S");
}

void drawSettings() {
  display.clearDisplay();

  drawIcons();

  display.setTextSize(1);
  display.setCursor(35, 5);
  display.print("Setting");

  display.setCursor(35, 25);
  display.print("Volume  ");
  display.print(volume);

  display.setCursor(35, 40);
  display.print("EQ      ");
  display.print(eq);

  if (settingIndex == 0)
    display.drawRect(33, 23, 90, 10, WHITE);

  if (settingIndex == 1)
    display.drawRect(33, 38, 90, 10, WHITE);

  display.display();
}

void drawMusic() {
  display.clearDisplay();

  drawIcons();

  display.setTextSize(1);
  display.setCursor(35, 20);
  display.print("Playing...");

  display.display();
}

void loop() {

  if (!inSettings) {
    drawMusic();
  } else {
    drawSettings();
  }

  if (digitalRead(BTN_UP) == LOW) {
    if (inSettings) {
      if (settingIndex == 0 && volume < 30) {
        volume++;
        player.volume(volume);
      }
      if (settingIndex == 1 && eq < 5) {
        eq++;
        player.EQ(eq);
      }
    }
    delay(200);
  }

  if (digitalRead(BTN_DOWN) == LOW) {
    if (inSettings) {
      if (settingIndex == 0 && volume > 0) {
        volume--;
        player.volume(volume);
      }
      if (settingIndex == 1 && eq > 0) {
        eq--;
        player.EQ(eq);
      }
    }
    delay(200);
  }

  if (digitalRead(BTN_SELECT) == LOW) {
    if (!inSettings) {
      inSettings = true;
    } else {
      settingIndex++;
      if (settingIndex > 1) settingIndex = 0;
    }
    delay(300);
  }

  if (digitalRead(BTN_BACK) == LOW) {
    inSettings = false;
    delay(300);
  }
}
