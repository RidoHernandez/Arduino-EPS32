#include <Arduino.h>
#include <Servo.h>          
#include <NewPing.h>     
#include <DHT.h>     
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <avr/pgmspace.h>

const int trig = 9;
const int echo = 10;
const int verde = 5;
const int amarillo = 6;
const int rojo = 7;
const int bz = 8;

float dist;
unsigned long tB = 0;
bool st = false;

void leds(){
  digitalWrite(verde, LOW);
  digitalWrite(amarillo, LOW);
  digitalWrite(rojo, LOW);
}

void setup(){
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(verde, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(bz, OUTPUT);
}

float medir(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH, 30000);
  if(t == 0) return -1;
  return t * 0.0343 / 2.0;
}

void beep(unsigned long p){
  if(millis() - tB >= p){
    tB = millis();
    st = !st;
    if(st) tone(bz, 1500);
    else noTone(bz);
  }
}

void loop(){
  dist = medir();
  if(dist < 0){
    Serial.println("Sin lectura");
    leds();
    noTone(bz);
    delay(200);
    return;
  }
  leds();
  Serial.print("Distancia: ");
  Serial.print(dist);
  Serial.print(" cm  ");
  if(dist > 100){
    digitalWrite(verde, HIGH);
    noTone(bz);
    Serial.println("LIBRE");
  } else if(dist > 50){
    digitalWrite(amarillo, HIGH);
    beep(600);
    Serial.println("CERCA");
  } else if(dist > 20){
    digitalWrite(rojo, HIGH);
    beep(200);
    Serial.println("MUY CERCA");
  } else {
    digitalWrite(rojo, HIGH);
    tone(bz, 1500);
    Serial.println("PELIGRO");
  }
  delay(50);
}