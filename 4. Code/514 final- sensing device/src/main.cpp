#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial BTSerial;

const int PPG_PIN = 34; // Assuming PPG sensor is connected to GPIO 34
unsigned long lastBeatTime = 0;

void setup() {
  Serial.begin(115200);
  BTSerial.begin("PPG_Sensing_Device"); // Bluetooth device name
  pinMode(PPG_PIN, INPUT);
}

void loop() {
  int sensorValue = analogRead(PPG_PIN);
  unsigned long currentTime = millis();

  // Dummy condition for heartbeat detection; replace with your algorithm
  if (sensorValue > 512 && (currentTime - lastBeatTime > 200)) { // Simple threshold and debounce logic
    unsigned long beatInterval = currentTime - lastBeatTime;
    lastBeatTime = currentTime;

    // Send interval to display device
    BTSerial.println(beatInterval);
  }

  delay(1); // Short delay to prevent spamming; adjust based on your needs
}
