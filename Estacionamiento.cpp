#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <DHT.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <avr/pgmspace.h>

const int s1 = 2;
const int s2 = 3;
const int s3 = 4;
const int v1 = 5;
const int r1 = 6;
const int v2 = 7;
const int r2 = 8;
const int v3 = 9;
const int r3 = 10;

bool o1, o2, o3;
int lib;

void setup(){
  Serial.begin(9600);
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(v1, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(v2, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(v3, OUTPUT);
  pinMode(r3, OUTPUT);
}

void lugar(int s, int v, int r, bool &o){
  o = digitalRead(s) == LOW;
  if(o){
    digitalWrite(v, LOW);
    digitalWrite(r, HIGH);
  } else {
    digitalWrite(v, HIGH);
    digitalWrite(r, LOW);
  }
}

void loop(){
  lugar(s1, v1, r1, o1);
  lugar(s2, v2, r2, o2);
  lugar(s3, v3, r3, o3);
  lib = 0;
  if(!o1) lib++;
  if(!o2) lib++;
  if(!o3) lib++;
  Serial.println("ESTACIONAMIENTO");
  Serial.println();
  Serial.print("Lugar 1: ");
  Serial.println(o1 ? "OCUPADO" : "LIBRE");
  Serial.print("Lugar 2: ");
  Serial.println(o2 ? "OCUPADO" : "LIBRE");
  Serial.print("Lugar 3: ");
  Serial.println(o3 ? "OCUPADO" : "LIBRE");
  Serial.println();
  Serial.print("Disponibles: ");
  Serial.println(lib);
  Serial.println("--------------------");
  delay(1000);
}