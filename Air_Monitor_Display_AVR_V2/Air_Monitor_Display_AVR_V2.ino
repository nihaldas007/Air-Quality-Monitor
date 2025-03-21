#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define YELLOW 0xFFE0
#define ORANGE 0xFD20
#define MEGENTA 0xF81F

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;  // Adjust ID for compatibility
  tft.begin(ID);
  tft.setRotation(1);  // Landscape mode for 480x320
  drawUI();
}

void drawUI() {
  tft.fillScreen(BLACK);

  // White Dividing Lines
  tft.drawFastHLine(10, 150, 310, WHITE);  // Horizontal line below PM2.5
  tft.drawFastHLine(320, 85, 460, WHITE);
  tft.drawFastHLine(320, 170, 460, WHITE);
  // tft.drawFastHLine(10, 180, 460, WHITE); // Horizontal line below HCHO
  tft.drawFastVLine(320, 10, 240, WHITE);  // Vertical separator (right side)
  tft.drawFastHLine(10, 250, 480, WHITE);
  

  tft.setTextColor(VIOLET);
  tft.setTextSize(2);
  tft.setCursor(50, 10);
  tft.print("Air Quality Monitor");
  // PM2.5 Value
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 40);
  tft.print("PM2.5");

  tft.setTextColor(RED);
  tft.setTextSize(9);
  tft.setCursor(120, 60);
  tft.print("86");
  tft.setTextSize(2);
  tft.setCursor(240, 110);
  tft.print("ug/m3");

  // CO2 Value
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 160);
  tft.print("CO2");

  tft.setTextColor(CYAN);
  tft.setTextSize(4);
  tft.setCursor(80, 190);
  tft.print("100 mg/m3");

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(340, 20);
  tft.print("TEMP");

  tft.setTextSize(3);
  tft.setCursor(340, 50);
  tft.print("23");
  tft.print((char)247);
  tft.print("C");

  tft.setTextSize(2);
  tft.setCursor(340, 100);
  tft.print("RH");

  tft.setTextSize(3);
  tft.setCursor(340, 130);
  tft.print("49%");
  
  tft.setTextSize(2);
  tft.setCursor(340, 180);
  tft.print("GAS");

  tft.setTextSize(3);
  tft.setCursor(340, 210);
  tft.print("49%");
  // AQI Scale
  tft.setTextSize(2);
  tft.setCursor(20, 260);
  tft.print("AQI");

  // AQI Color Bar
  tft.fillRect(80, 260, 60, 20, GREEN);
  tft.fillRect(140, 260, 60, 20, YELLOW);
  tft.fillRect(200, 260, 60, 20, ORANGE);
  tft.fillRect(260, 260, 60, 20, RED);
  tft.fillRect(320, 260, 60, 20, 0xF81F);  // Purple

  // AQI Indicator
  tft.fillTriangle(110, 285, 100, 305, 120, 305, WHITE); //green
  // tft.fillTriangle(170, 285, 160, 305, 180, 305, WHITE); //Yellow
  // tft.fillTriangle(230, 285, 220, 305, 240, 305, WHITE); //light Yellow
  // tft.fillTriangle(290, 285, 280, 305, 300, 305, WHITE); //red
  // tft.fillTriangle(350, 285, 340, 305, 360, 305, WHITE); //pink
  tft.setCursor(140, 300);
  tft.setTextSize(2);
  // tft.print("Grade2");

  // Battery Icon
  tft.drawRect(410, 10, 40, 20, WHITE);
  tft.fillRect(450, 14, 5, 12, WHITE);
  tft.fillRect(413, 13, 34, 14, GREEN);
}

void loop() {
  // UI remains static unless updating values dynamically
}