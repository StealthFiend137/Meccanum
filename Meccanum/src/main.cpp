#include <Arduino.h>
#include <Wire.h>

// Motor control pins
const int motor1A = 2;
const int motor1B = 3;
const int motor2A = 4;
const int motor2B = 5;
const int motor3A = 6;
const int motor3B = 7;
const int motor4A = 8;
const int motor4B = 9;

// Variables to store velocity commands
int forwardVelocity = 0;
int sidewaysVelocity = 0;
int rotationalVelocity = 0;

void receiveEvent(int numBytes);
void setMotorSpeed(int pinA, int pinB, int speed);

void setup() {
  // Initialize I2C communication
  Wire.begin();
  Wire.onReceive(receiveEvent);

  // Initialize motor control pins
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(motor3A, OUTPUT);
  pinMode(motor3B, OUTPUT);
  pinMode(motor4A, OUTPUT);
  pinMode(motor4B, OUTPUT);
}

void loop() {
  // Calculate motor speeds based on velocity commands
  int motor1Speed = forwardVelocity + sidewaysVelocity + rotationalVelocity;
  int motor2Speed = forwardVelocity - sidewaysVelocity - rotationalVelocity;
  int motor3Speed = forwardVelocity - sidewaysVelocity + rotationalVelocity;
  int motor4Speed = forwardVelocity + sidewaysVelocity - rotationalVelocity;

  // Set motor directions and speeds
  setMotorSpeed(motor1A, motor1B, motor1Speed);
  setMotorSpeed(motor2A, motor2B, motor2Speed);
  setMotorSpeed(motor3A, motor3B, motor3Speed);
  setMotorSpeed(motor4A, motor4B, motor4Speed);

  delay(100); // Adjust as needed
}

void setMotorSpeed(int pinA, int pinB, int speed) {
  if (speed > 0) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    analogWrite(pinA, speed);
    analogWrite(pinB, 0);
  } else if (speed < 0) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    analogWrite(pinA, -speed);
    analogWrite(pinB, 0);
  } else {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    analogWrite(pinA, 0);
    analogWrite(pinB, 0);
  }
}

void receiveEvent(int numBytes) {
  if (numBytes >= 6) { // Assuming each velocity is sent as 2 bytes
    forwardVelocity = Wire.read() << 8 | Wire.read();
    sidewaysVelocity = Wire.read() << 8 | Wire.read();
    rotationalVelocity = Wire.read() << 8 | Wire.read();
  }
}