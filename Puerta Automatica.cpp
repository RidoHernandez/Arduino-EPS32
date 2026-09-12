#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <DHT.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <avr/pgmspace.h>

Servo sv;
const int trig = 9;
const int echo = 10;
const int verde = 5;
const int rojo = 6;
const int umb = 50;
float dist;

void setup(){
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  sv.attach(11);
  sv.write(0);
  digitalWrite(rojo, HIGH);
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

void loop(){
  dist = medir();
  if(dist < 0){
    Serial.println("Sin lectura");
    delay(200);
    return;
  }
  Serial.print("Distancia: ");
  Serial.print(dist);
  Serial.print(" cm  Puerta: ");
  if(dist < umb){
    sv.write(90);
    digitalWrite(rojo, LOW);
    digitalWrite(verde, HIGH);
    Serial.println("ABIERTA");
  } else {
    sv.write(0);
    digitalWrite(verde, LOW);
    digitalWrite(rojo, HIGH);
    Serial.println("CERRADA");
  }
  delay(200);
}