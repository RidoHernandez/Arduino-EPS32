#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <DHT.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <avr/pgmspace.h>

const int ldr = A0;
const int led = 9;

int val;
int brillo;

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop(){
  val = analogRead(ldr);
  brillo = map(val, 0, 1023, 255, 0);
  brillo = constrain(brillo, 0, 255);
  analogWrite(led, brillo);
  Serial.print("LDR: ");
  Serial.print(val);
  Serial.print("  Brillo: ");
  Serial.print(map(brillo, 0, 255, 0, 100));
  Serial.println("%");
  delay(200);
}