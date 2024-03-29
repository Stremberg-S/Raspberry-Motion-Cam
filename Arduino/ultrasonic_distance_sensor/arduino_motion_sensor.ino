#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int DISTANCE_THRESHOLD = 300; // cm
const int MAX_DISTANCE = 400; // cm
const int TRIGGER_PIN = 9;
const int ECHO_PIN = 10;
const int LED_PIN = 13;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

bool cooldown = false;
bool motionState = true;

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

void setup() {
    Serial.begin(9600);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    lcd.begin();
    lcd.backlight();
    lcd.clear();
}

void loop() {
    long duration;
    long distance; // cm

    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration / 59;

    lcd.setCursor(0, 0);
    displayDistance(distance);

    // NO MOTION:
    if (distance > DISTANCE_THRESHOLD ) {
        delay(3000);
        cooldown = false;
        digitalWrite(LED_PIN, LOW);
        if (motionState) {
            Serial.println('0');
            motionState = false;
        }
    }
    // MOTION DETECTED:
    else if (distance <= DISTANCE_THRESHOLD && !cooldown) {
        Serial.println('1');
        digitalWrite(LED_PIN, HIGH);
        cooldown = true;
        motionState = true;
    }
}

void displayDistance(long distance) {
    if (distance <= MAX_DISTANCE) {
        lcd.setCursor(0, 0);
        lcd.print("Distance: ");
        lcd.print(distance);
        lcd.print(" cm   ");
    } else {
        delay(3000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Out of range..");
    }
}
