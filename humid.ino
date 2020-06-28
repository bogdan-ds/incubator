#include "DHT.h"
#include "LiquidCrystal.h"

#define DHTPIN 8
#define DHTTYPE DHT22
#define HUMIDIFIER 7
#define HEAT 6
#define ON HIGH
#define OFF LOW

LiquidCrystal LCD(12, 11, 5, 4, 3, 2);

DHT dht(DHTPIN, DHTTYPE);

void switch_relay(int relay, int state) {
  digitalWrite(relay, state);
}

void operate_device(int ref, int lower, int higher, int device) {
  if (ref < lower & digitalRead(device) != ON) {
    switch_relay(device, ON);
    Serial.print("Switching on device on pin:\t");
    Serial.print(device);
  }
  if (ref > higher & digitalRead(device) != OFF) {
    switch_relay(device, OFF);
    Serial.print("Switching off device on pin: \t");
    Serial.print(device);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting incubator test!"));
  dht.begin();
  LCD.begin(16, 2);
  LCD.print("Booting...");
  pinMode(HUMIDIFIER, OUTPUT);
  pinMode(HEAT, OUTPUT);

}

void loop() {
  delay(5000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  Serial.print(F("Humidity: "));
  LCD.setCursor(0, 1);
  LCD.clear();
  LCD.print("Humidity: ");
  LCD.setCursor(10, 0);
  LCD.print(h);
  LCD.setCursor(15, 0);
  LCD.print('%');
  LCD.setCursor(0, 2);
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  LCD.print("Temp: ");
  LCD.setCursor(6, 2);
  LCD.print(t);
  LCD.setCursor(12, 2);
  LCD.print("C");
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print("\n");
}
