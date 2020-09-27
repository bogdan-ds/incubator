#include "DHT.h"
#include "LiquidCrystal.h"

#define DHTPIN 8
#define DHTTYPE DHT22
#define HUMIDIFIER 7
#define HEAT 6
#define ON HIGH
#define OFF LOW

unsigned long startMillis;
unsigned long currentMillis;

LiquidCrystal LCD(12, 11, 5, 4, 3, 2);

DHT dht(DHTPIN, DHTTYPE);

boolean activated;

void switch_relay(int relay, int state) {
  digitalWrite(relay, state);
}

void operate_device(int sensor_reading, int lower_threshold, int upper_threshold, int device) {
  if (sensor_reading < lower_threshold & digitalRead(device) != ON) {
    switch_relay(device, ON);
    Serial.print("Switching on device on pin:\t");
    Serial.print(device);
  }
  if (sensor_reading > upper_threshold & digitalRead(device) != OFF) {
    switch_relay(device, OFF);
    Serial.print("Switching off device on pin: \t");
    Serial.print(device);
  }
}


bool periodic_action(int device, unsigned long period, unsigned long duration) {
  currentMillis = millis();
  if (activated == true && (currentMillis - startMillis >= duration)) {
    switch_relay(device, OFF);
    Serial.println(F("Switching device off"));
    activated = false;
  }
  else if (activated == false && (currentMillis - startMillis >= period)) {
    Serial.println(F("Switching device on"));
    switch_relay(device, ON);
    startMillis = currentMillis;
    activated = true;
  }
  return activated;
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Botting..."));
  dht.begin();
  LCD.begin(16, 2);
  LCD.print("Booting...");
  pinMode(HUMIDIFIER, OUTPUT);
  pinMode(HEAT, OUTPUT);
  startMillis = millis();
}

void loop() {
  delay(1000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    LCD.setCursor(0, 1);
    LCD.clear();
    LCD.print("Fail read sensor");
    return;
  }

  activated = periodic_action(HUMIDIFIER, 18000000, 180000);
  if (activated == false) {
    operate_device(humidity, 70, 90, HUMIDIFIER);
  }

  Serial.print(F("Humidity: "));
  LCD.setCursor(0, 1);
  LCD.clear();
  LCD.print("Humidity: ");
  LCD.setCursor(10, 0);
  LCD.print(humidity);
  LCD.setCursor(15, 0);
  LCD.print('%');
  LCD.setCursor(0, 2);
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  LCD.print("Temp: ");
  LCD.setCursor(6, 2);
  LCD.print(temperature);
  LCD.setCursor(12, 2);
  LCD.print("C");
  Serial.print(temperature);
  Serial.print(F("°C "));
  Serial.print("\n");
}
