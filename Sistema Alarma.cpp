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
bool arm = true;
unsigned long tB = 0;
bool st = false;

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

void leds(){
  digitalWrite(verde, LOW);
  digitalWrite(amarillo, LOW);
  digitalWrite(rojo, LOW);
}

void beep(unsigned long p){
  if(millis() - tB >= p){
    tB = millis();
    st = !st;
    if(st) tone(bz, 1800);
    else noTone(bz);
  }
}

void loop(){
  if(Serial.available()){
    char c = Serial.read();
    if(c == 'A' || c == 'a'){
      arm = true;
      Serial.println("ALARMA ACTIVADA");
    } else if(c == 'D' || c == 'd'){
      arm = false;
      Serial.println("ALARMA DESACTIVADA");
    }
  }
  dist = medir();
  if(dist < 0){
    Serial.println("Sin lectura");
    leds();
    noTone(bz);
    delay(200);
    return;
  }
  Serial.print("Distancia: ");
  Serial.print(dist);
  Serial.print(" cm  ");
  leds();
  noTone(bz);
  if(!arm){
    digitalWrite(verde, HIGH);
    Serial.println("SISTEMA OFF");
  } else if(dist > 100){
    digitalWrite(verde, HIGH);
    Serial.println("SISTEMA OK");
  } else if(dist > 30){
    digitalWrite(amarillo, HIGH);
    beep(400);
    Serial.println("ADVERTENCIA");
  } else {
    digitalWrite(rojo, HIGH);
    tone(bz, 2000);
    Serial.println("INTRUSO");
  }
  delay(100);
}