#include <Servo.h>

Servo myServo;

int ldrPin = A0;
int servoPin = 9;

int btn1 = 6; // Toggle Auto/Manual
int btn2 = 11; // Increase angle (Manual)
int btn3 = 3;  // Decrease angle (Manual)

bool isManual = false; // Default: Auto mode
int servoAngle = 0;
int ldrValue = 0;
int threshold = 500;
bool lastBtn1State = HIGH; // For detecting btn1 press edge

void setup() {
  myServo.attach(servoPin);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  Serial.begin(9600);
  myServo.write(servoAngle); // Move to default angle
  Serial.println("Starting in AUTO mode...");
}

void loop() {
  // Read button states
  bool btn1State = digitalRead(btn1);
  bool btn2State = digitalRead(btn2);
  bool btn3State = digitalRead(btn3);
  ldrValue = analogRead(ldrPin);

  // Toggle Auto/Manual Mode on btn1 press (edge detection)
  if (btn1State == LOW && lastBtn1State == HIGH) {
    isManual = !isManual;
    Serial.print("Switched to ");
    Serial.println(isManual ? "MANUAL" : "AUTO");
    delay(300); // Debounce
  }
  lastBtn1State = btn1State;

  if (isManual) {
    // Manual Mode: adjust with btn2 and btn3
    if (btn2State == LOW && servoAngle < 180) {
      servoAngle++;
      myServo.write(servoAngle);
      delay(10);
    }

    if (btn3State == LOW && servoAngle > 0) {
      servoAngle--;
      myServo.write(servoAngle);
      delay(10);
    }
  } else {
    // Auto Mode: use LDR
    if (ldrValue > threshold) {
      servoAngle = 180;
    } else {
      servoAngle = 0;
    }
    myServo.write(servoAngle);
    delay(500); // Delay to reduce LDR polling frequency
  }

  // Print all values in one row
  Serial.print("BTN1: "); Serial.print(btn1State);
  Serial.print(" | BTN2: "); Serial.print(btn2State);
  Serial.print(" | BTN3: "); Serial.print(btn3State);
  Serial.print(" | LDR: "); Serial.print(ldrValue);
  Serial.print(" | Mode: "); Serial.print(isManual ? "MANUAL" : "AUTO");
  Serial.print(" | Servo Angle: "); Serial.println(servoAngle);
}
