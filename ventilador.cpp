#include <Arduino.h>
#include <DHT.h>

const int dhtP = 2;
const int verde = 5;
const int amarillo = 6;
const int rojo = 7;
const int bz = 8;
const int fan = 9;

DHT dht(dhtP, DHT11);
float t, h;
unsigned long tLec = 0;
const unsigned long TL = 2000;

void setup(){
  Serial.begin(9600);
  dht.begin();
  pinMode(verde, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(bz, OUTPUT);
  pinMode(fan, OUTPUT);
}

void leds(){
  digitalWrite(verde, LOW);
  digitalWrite(amarillo, LOW);
  digitalWrite(rojo, LOW);
}

void loop(){
  if(millis() - tLec < TL) return;
  tLec = millis();
  t = dht.readTemperature();
  h = dht.readHumidity();
  if(isnan(t) || isnan(h)) return;
  leds();
  noTone(bz);
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C  Hum: ");
  Serial.print(h);
  Serial.print(" %  Estado: ");
  if(t < 25){
    digitalWrite(verde, HIGH);
    analogWrite(fan, 0);
    Serial.println("NORMAL");
  } else if(t <= 30){
    digitalWrite(amarillo, HIGH);
    analogWrite(fan, 128);
    Serial.println("VENTILACION");
  } else if(t <= 35){
    digitalWrite(rojo, HIGH);
    analogWrite(fan, 255);
    Serial.println("VENTILADOR MAX");
  } else {
    digitalWrite(rojo, HIGH);
    analogWrite(fan, 255);
    tone(bz, 1500);
    Serial.println("ALARMA");
  }
}