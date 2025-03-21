#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
MCUFRIEND_kbv tft;

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define BLUE    0x001F
#define YELLOW  0xFFE0

void setup(void) {
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.println("Air Quality Monitor");
    Serial.print("Found ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // Force ID if write-only display
    tft.begin(ID);
    tft.setRotation(0);
    tft.fillScreen(BLACK);
}

void loop(void) {
    // tft.fillScreen(BLACK);
    
    // Sample values for display
    int pm25 = 109;  // Replace with actual reading
    int co2 = 450;   // Replace with actual reading
    int temp = 24;   // Replace with actual reading
    int humidity = 60; // Replace with actual reading

    // Draw each circle with its value
    drawCircleWithValue(230, 120, 80, pm25, "PM2.5", GREEN);
    drawCircleWithValue(230, 280, 70, co2, " CO2 ", RED);
    drawCircleWithValue(80, 180, 70, temp, "Temp", BLUE);
    drawCircleWithValue(80, 330, 70, humidity, "Hum ", YELLOW);

    delay(5000); // Update every 5 seconds
}

void drawCircleWithValue(int x, int y, int radius, float value, const char* label, uint16_t color) {
    // Draw circle outline
    tft.drawCircle(x, y, radius, color);
    
    // Fill circle with a slightly transparent color
    tft.fillCircle(x, y, radius - 2, color);

    // Set text color and font for value
    tft.setTextColor(BLACK);
    tft.setFont(&FreeSevenSegNumFont);
    tft.setTextSize(1);
    
    // Convert value to string to calculate width
    String valueStr = String(value, 0);
    int valueWidth = getTextWidth(valueStr); // Get width of the value text

    // Center the value horizontally and position vertically
    tft.setCursor(x - valueWidth + 16, y + 40);
    tft.print(valueStr);
    
    // Set label font and position
    tft.setFont(&FreeSerif12pt7b);
    tft.setTextSize(1);
    int labelWidth = getTextWidth(label); // Get width of the label text

    tft.setCursor(x - (labelWidth / 2), y - 20); // Top the label below the value
    tft.print(label);
}

// Function to calculate text width based on current font size and characters
int getTextWidth(const String &text) {
    int width = 0;
    for (char c : text) {
        width += (c >= '0' && c <= '9') ? 24 : 12; // Approximate widths for numbers and letters
    }
    return width;
}
