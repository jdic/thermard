#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 2
#define BUTTON 14
#define ENA 15
#define IN1 16
#define IN2 17

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

bool buttonState = false;
bool lastButtonState = false;
bool isActive = false;

void showOnDisplay(String temp)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print(String(temp) + " F");
}

void awake()
{
  Serial.begin(9600);
  sensors.begin();

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void setup()
{
  lcd.init();
  lcd.backlight();
}

void loop()
{
  sensors.requestTemperatures();

  float fahrenheit = sensors.getTempFByIndex(0);

  showOnDisplay(String(fahrenheit));

  buttonState = digitalRead(BUTTON);

  if (buttonState != lastButtonState)
  {
    if (buttonState == LOW)
    {
      isActive = !isActive;

      if (isActive)
      {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(ENA, HIGH);
      }
      
      else
      {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(ENA, LOW);
      }

      delay(50);
    }
  }

  lastButtonState = buttonState;
  delay(1000);
}