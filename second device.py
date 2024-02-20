#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define PPG_PIN 34       // Analog pin for PPG sensor

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Check the I2C address of your display
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

  pinMode(PPG_PIN, INPUT); // Initialize PPG sensor pin as input
}

void loop() {
  int ppgValue = analogRead(PPG_PIN); // Read the PPG sensor
  float voltage = ppgValue * (3.3 / 4095.0); // Convert to voltage if needed

  // Display PPG value on OLED
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0);     // Start at top-left corner
  display.print(F("PPG Value: "));
  display.println(ppgValue);
  display.print(F("Voltage: "));
  display.println(voltage);
  
  display.display(); // Show initial text
  delay(1000); // Update every second
}

