#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

BluetoothSerial BTSerial;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Buffer to store intervals
const int MAX_INTERVALS = 30;
int intervals[MAX_INTERVALS];
int intervalCount = 0;

void setup() {
  Serial.begin(115200);
  BTSerial.begin("PPG_Display_Device"); // Set the same name as in BTSerial.begin on the sensing device

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  if (BTSerial.available()) {
    String value = BTSerial.readStringUntil('\n');
    int interval = value.toInt();
    
    // Store intervals up to MAX_INTERVALS
    if (intervalCount < MAX_INTERVALS) {
      intervals[intervalCount++] = interval;
    }

    // If we have enough intervals, calculate RMSSD
    if (intervalCount == MAX_INTERVALS) {
      float rmssd = calculateRMSSD(intervals, intervalCount);
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("RMSSD: ");
      display.println(rmssd);

      // Display stress level based on RMSSD value (threshold is arbitrary, adjust based on your data)
      if (rmssd > 50) { // Example threshold
        display.println("Relaxed");
      } else {
        display.println("Stressed");
      }
      display.display();

      intervalCount = 0; // Reset for the next batch of intervals
    }
  }
}

float calculateRMSSD(int intervals[], int count) {
  long sumOfSquares = 0;
  for (int i = 1; i < count; i++) {
    int diff = intervals[i] - intervals[i - 1];
    sumOfSquares += diff * diff;
  }
  return sqrt(sumOfSquares / (float)(count - 1));
}
