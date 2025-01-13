#include <Servo.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h> // Library for RTC module

// Pin assignments
const int sensorPin1 = A0;
const int sensorPin2 = A1;
const int servoPin = 13;
const int ledPin1 = 12;
const int ledPin2 = 4;
const int buzzerPin = 10;

// Variables
int sensorValue = 0;
int servoGrad = 90;
int tolerance = 40;
bool useDHT = true; // Flag for using DHT sensor
DateTime now;        // DateTime object to hold current time

Servo myservo;
RTC_DS3231 rtc; // RTC object

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT setup
#define DHTPIN 7        // Pin connected to the DHT sensor
#define DHTTYPE DHT11   // Type of DHT sensor
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

unsigned long previousMillisDHT = 0;
const long intervalDHT = 10000; // Interval to display DHT data (in milliseconds)

unsigned long previousMillisSensor = 0;
const long intervalSensor = 100; // Interval to read sensor and control servo (in milliseconds)

void setup()
{
  // Initialize pins
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Attach servo
  myservo.attach(servoPin);
  myservo.write(servoGrad);

  // Initialize serial communication
  Serial.begin(9600);

  // Start DHT sensor
  dht.begin();

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize RTC module
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Customize LCD layout
  lcd.setCursor(0, 0);
  lcd.print("   Welcome to");
  lcd.setCursor(0, 1);
  lcd.print(" Smart Soltrack");
  delay(2000); // Delay to display welcome message
}

void loop(){
    // Read sensor values
    sensorValue = analogRead(sensorPin1);
    int reading = analogRead(sensorPin2);

   unsigned long currentMillis = millis();

  // Read sensor values and control servo/photoresistor
  if (currentMillis - previousMillisSensor >= intervalSensor)
  {
    previousMillisSensor = currentMillis;
    sensorValue = analogRead(sensorPin1);

    if (sensorValue < (512 - tolerance))
    {
      if (myservo.read() < 180)
      {
        myservo.write(myservo.read() + 1);
      }
      digitalWrite(ledPin1, HIGH); // Turn on LED1 when light intensity is low
      digitalWrite(ledPin2, LOW);  // Turn off LED2 when light intensity is low
    }
    else if (sensorValue > (512 + tolerance))
    {
      if (myservo.read() > 0)
      {
        myservo.write(myservo.read() - 1);
      }
      digitalWrite(ledPin1, LOW);  // Turn off LED1 when light intensity is high
      digitalWrite(ledPin2, HIGH); // Turn on LED2 when light intensity is high
    }
    else
    {
      digitalWrite(ledPin1, LOW); // Turn off both LEDs when light intensity is within tolerance
      digitalWrite(ledPin2, LOW);
    }
    delay(100);
  }

  // Display DHT or RTC data
  if (currentMillis - previousMillisDHT >= intervalDHT)
  {
    previousMillisDHT = currentMillis;
    if (useDHT)
    {
      displayDHTData();
    }
    else
    {
      displayRTCData();
    }
    useDHT = !useDHT; // Switch between DHT and RTC display
  }
}

void displayDHTData() {
    // Read DHT sensor data
    float h = dht.readHumidity();       // Read humidity
    float t = dht.readTemperature();    // Read temperature in Celsius

    // Check if any reads failed and exit early (to try again)
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print temperature and humidity to Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %, Temp: ");
    Serial.print(t);
    Serial.println(" °C");

    // Update LCD display with temperature and humidity
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Temp: ");
    lcd.print(t);
    lcd.print("C  ");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(h);
    lcd.print("%  ");

    // Check temperature thresholds for alert sounds
    if (t > 40) {
        tone(buzzerPin, 1000); // Sound the buzzer for high temperature
    } else {
        noTone(buzzerPin); // Stop the sound if temperature is not high
    }

    // Check for temperature changes and sound a different tone
    static float lastTemperature = 0;
    if (abs(t - lastTemperature) > 1) {
        tone(buzzerPin, 2000); // Sound a different tone for temperature changes
        delay(100); // Ensure the tone is audible
        noTone(buzzerPin); // Stop the sound after a short delay
        lastTemperature = t; // Update the last temperature
    }
}

void displayRTCData() {
    // Get current time from RTC module
    now = rtc.now();

    // Update LCD display with date and time
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("");
    printDayOfWeek(now.dayOfTheWeek());
    lcd.print(", ");
    lcd.print(now.day());
    lcd.print(" ");
    printMonth(now.month());
    lcd.print(" ");
    lcd.print(now.year());

    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(formatHour(now.hour()));
    lcd.print(":");
    printTwoDigits(now.minute());
    lcd.print(":");
    printTwoDigits(now.second());
    lcd.print(" ");
    lcd.print(getTimePeriod(now.hour()));
}

// Function to print the day of the week
void printDayOfWeek(uint8_t day) {
    switch (day) {
        case 1: lcd.print("Sun"); break;
        case 2: lcd.print("Mon"); break;
        case 3: lcd.print("Tue"); break;
        case 4: lcd.print("Wed"); break;
        case 5: lcd.print("Thu"); break;
        case 6: lcd.print("Fri"); break;
        case 7: lcd.print("Sat"); break;
        default: lcd.print("Unknown"); break;
    }
}

// Function to print the month
void printMonth(uint8_t month) {
    switch (month) {
        case 1: lcd.print("January"); break;
        case 2: lcd.print("February"); break;
        case 3: lcd.print("March"); break;
        case 4: lcd.print("April"); break;
        case 5: lcd.print("May"); break;
        case 6: lcd.print("June"); break;
        case 7: lcd.print("July"); break;
        case 8: lcd.print("August"); break;
        case 9: lcd.print("September"); break;
        case 10: lcd.print("October"); break;
        case 11: lcd.print("November"); break;
        case 12: lcd.print("December"); break;
        default: lcd.print("Unknown"); break;
    }
}

// Function to format the hour in 12-hour format
int formatHour(uint8_t hour) {
    if (hour == 0) {
        return 12;
    } else if (hour > 12) {
        return hour - 12;
    } else {
        return hour;
    }
}

// Function to print two digits (leading zero if needed)
void printTwoDigits(uint8_t number) {
    if (number < 10) {
        lcd.print("0");
    }
    lcd.print(number);
}

// Function to get the time period (AM or PM)
String getTimePeriod(uint8_t hour) {
    if (hour >= 12) {
        return "PM";
    } else {
        return "AM";
    }
}