#include "DHT.h"

#define DHTPIN 8
#define DHTTYPE DHT11
#define HUMIDIFIER 7
#define HEAT 6
#define ON HIGH
#define OFF LOW


DHT dht(DHTPIN, DHTTYPE);

void switch_relay(int relay, int state) {
  digitalWrite(relay, state);
}

void operate_device(int ref, int lower, int higher, int device) {
  if(ref < lower & digitalRead(device) != HIGH) {
    switch_relay(device, ON);
    Serial.print("Switching on device on pin:\t");
    Serial.print(device);
  }
  if(ref > higher & digitalRead(device) != LOW) {
    switch_relay(device, OFF);
    Serial.print("Switching off device on pin: \t");
    Serial.print(device);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting incubator test!"));

  dht.begin();
  pinMode(HUMIDIFIER, OUTPUT);
}

void loop() {
  delay(5000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  operate_device(h, 80, 91, HUMIDIFIER);
  operate_device(t, 22, 25, HEAT);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print("\n");
}
