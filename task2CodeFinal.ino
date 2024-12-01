// Task 2 Code 
// Sondos Qasarwa - 1210259
// Sondos Hisham -120166
// Sarah Hassouneh - 1210068

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int motorIn1 = 7;  
const int motorIn2 = 8; 
const int motorPWM = 9;  

const int encoderA = 3;  

const int potPin = A0;  


volatile int encoderCount = 0;  
int motorSpeed = 0;             
String motorDirection = "STOP"; 
int potValue = 0;               

const int pulsesPerRevolution = 70;  // Calculated from encoderCounts
const int degreesPerPulse = 360 / pulsesPerRevolution;  

void setup() {

  Serial.begin(9600);

  encoderCount = 0;

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorPWM, OUTPUT);


 
  pinMode(encoderA, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderA), updateEncoder, CHANGE);

  motorSpeed = 150;  // Set default speed
  delay(1000);
}

void loop() {
  
  potValue = analogRead(potPin);

  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);

  
  int position =(encoderCount * degreesPerPulse) % 360;
  if (position < 0) {
  position += 360;  
}

  if (potValue < 490) {
    setMotorDirection("CCW"); 
    motorSpeed = map(potValue, 0, 490, 255, 0); // Motor speed is proportional to the potentiometer value 
  } else if (potValue > 520) {
    setMotorDirection("CW");
    motorSpeed = map(potValue, 520, 1023, 0, 255); // Motor speed is proportional to the potentiometer value 

  } else {
    setMotorDirection("STOP");
  }
  // Display direction and position on the LCD 
  lcd.setCursor(0, 0);
  lcd.print("Dir: " + motorDirection + "   "); 
  lcd.setCursor(0, 1);
  lcd.print("Pos: " + String(position) + "°   ");

  
  delay(200);
}

// Set motor direction and spped 

void setMotorDirection(String direction) {
  motorDirection = direction;
  if (direction == "CW") {
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);
    analogWrite(motorPWM, motorSpeed);
  } else if (direction == "CCW") {
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);
    analogWrite(motorPWM, motorSpeed);
  } else {
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    analogWrite(motorPWM, 0);
  }
}

// Interrupt service routine for encoder
void updateEncoder() {
  
  if (digitalRead(motorIn1) == HIGH && digitalRead(motorIn2) == LOW) {
    encoderCount++;  // CW direction
  } else if (digitalRead(motorIn1) == LOW && digitalRead(motorIn2) == HIGH) {
    encoderCount--;  // CCW direction
  }
}