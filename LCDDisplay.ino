#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//I2C LCD (16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

//DS18B20 Temperature Sensor
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

//ACS712 Current Sensor
#define ACS_PIN A0
int sensitivity = 100; // 100 mV/A for ACS712-20A
int offsetvoltage = 2500; // Sensor offset in mV

//Voltage Sensor
#define VOLT_PIN A1

void setup() {
  lcd.init();
  lcd.backlight();
  tempSensor.begin();

  lcd.setCursor(0,0);
  lcd.print("Solar Monitor");
  lcd.setCursor(0,1);
  lcd.print("Initializing");
  delay(2000);
  lcd.clear();
}
void loop() {
  //Read Voltage
  int vRaw = analogRead(VOLT_PIN);
  float vOut = (vRaw * 5.0) / 1023.0;
  float voltage = vOut * 5.0; // Voltage sensor divider factor

  //Read Current
  int cRaw = analogRead(ACS_PIN);
  float mV = (cRaw / 1023.0) * 5000;
  float current = (mV - offsetvoltage) / sensitivity;
  if (current < 0) current = 0;

  //Calculate Power
  float power = voltage * current;

  //Read Temperature
  tempSensor.requestTemperatures();
  float temperature = tempSensor.getTempCByIndex(0);

  //SCREEN 1: Voltage & Current
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("V:");
  lcd.print(voltage,1);
  lcd.print("V");

  lcd.setCursor(8,0);
  lcd.print("I:");
  lcd.print(current,2);
  lcd.print("A");

  delay(2000);

  //SCREEN 2: Power & Temperature
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("P:");
  lcd.print(power,1);
  lcd.print("W");

  lcd.setCursor(8,0);
  lcd.print("T:");
  lcd.print(temperature,1);
  lcd.print("C");

  delay(2000);
}
