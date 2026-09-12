#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <DHT.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <avr/pgmspace.h>

const int sh = A0;
const int bomba = 8;
const int verde = 5;
const int rojo = 6;
const int bz = 7;
const int seco = 40;
const int mojado = 60;

int val;
int hum;

void setup(){
  Serial.begin(9600);
  pinMode(bomba, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(bz, OUTPUT);
}

void loop(){
  val = analogRead(sh);
  hum = map(val, 1023, 0, 0, 100);
  hum = constrain(hum, 0, 100);
  Serial.print("Humedad suelo: ");
  Serial.print(hum);
  Serial.println("%");
  if(hum < seco){
    digitalWrite(bomba, HIGH);
    digitalWrite(verde, LOW);
    digitalWrite(rojo, HIGH);
    tone(bz, 1000, 100);
    Serial.println("Estado: SUELO SECO");
    Serial.println("Riego: ACTIVADO");
  } else if(hum < mojado){
    digitalWrite(bomba, LOW);
    digitalWrite(verde, HIGH);
    digitalWrite(rojo, LOW);
    noTone(bz);
    Serial.println("Estado: HUMEDO");
    Serial.println("Riego: DESACTIVADO");
  } else {
    digitalWrite(bomba, LOW);
    digitalWrite(verde, HIGH);
    digitalWrite(rojo, LOW);
    noTone(bz);
    Serial.println("Estado: SUELO MOJADO");
    Serial.println("Riego: DESACTIVADO");
  }
  Serial.println("--------------------");
  delay(1000);
}