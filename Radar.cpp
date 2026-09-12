#include <Arduino.h>
#include <Servo.h>

Servo sv;
const int trig = 9;
const int echo = 10;
const int bz = 8;
const int umb = 30;
float dist;
int ang;

void setup(){
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(bz, OUTPUT);
  sv.attach(11);
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
  for(ang = 0; ang <= 180; ang += 10){
    sv.write(ang);
    delay(300);
    dist = medir();
    Serial.print("Angulo: ");
    Serial.print(ang);
    Serial.print("  Distancia: ");
    if(dist < 0) Serial.println("--");
    else {
      Serial.print(dist);
      Serial.println(" cm");
      if(dist < umb){
        Serial.println("OBJETO DETECTADO");
        Serial.print("Angulo: ");
        Serial.println(ang);
        Serial.print("Distancia: ");
        Serial.print(dist);
        Serial.println(" cm");
        tone(bz, 1200, 80);
      }
    }
  }
  for(ang = 180; ang >= 0; ang -= 10){
    sv.write(ang);
    delay(300);
    dist = medir();
    Serial.print("Angulo: ");
    Serial.print(ang);
    Serial.print("  Distancia: ");
    if(dist < 0) Serial.println("--");
    else{
      Serial.print(dist);
      Serial.println(" cm");
      if(dist < umb){
        Serial.println("OBJETO DETECTADO");
        Serial.print("Angulo: ");
        Serial.println(ang);
        Serial.print("Distancia: ");
        Serial.print(dist);
        Serial.println(" cm");
        tone(bz, 1200, 80);
      }
    }
  }
}