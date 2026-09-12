#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <DHT.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <Keypad.h>

const byte fil = 4;
const byte col = 4;
char tec[fil][col] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte fP[fil] = {9, 8, 7, 6};
byte cP[col] = {5, 4, 3, 2};
Keypad kp = Keypad(makeKeymap(tec), fP, cP, fil, col);

Servo sv;
const int bz = 11;
const int vd = 12;
const int rj = 13;
const char pin[5] = "1234";
char buf[5];
int idx = 0;
int intentos = 0;
bool bloq = false;

void setup(){
  Serial.begin(9600);
  sv.attach(10);
  sv.write(0);
  pinMode(bz, OUTPUT);
  pinMode(vd, OUTPUT);
  pinMode(rj, OUTPUT);
  digitalWrite(rj, HIGH);
  Serial.println("Ingrese PIN:");
}

void abrir(){
  digitalWrite(rj, LOW);
  digitalWrite(vd, HIGH);
  sv.write(90);
  tone(bz, 1500, 300);
  Serial.println("ACCESO PERMITIDO");
  delay(3000);
  sv.write(0);
  digitalWrite(vd, LOW);
  digitalWrite(rj, HIGH);
  intentos = 0;
  idx = 0;
}

void negar(){
  tone(bz, 400, 500);
  Serial.println("ACCESO DENEGADO");
  intentos++;
  idx = 0;
  if(intentos >= 3){
    bloq = true;
    Serial.println("SISTEMA BLOQUEADO");
    for(int i = 0; i < 5; i++){
      digitalWrite(rj, HIGH);
      tone(bz, 300, 200);
      delay(300);
      digitalWrite(rj, LOW);
      delay(200);
    }
    digitalWrite(rj, HIGH);
  }
}

void loop(){
  if(bloq) return;
  char c = kp.getKey();
  if(!c) return;
  if(c == '*'){
    idx = 0;
    Serial.println("Ingrese PIN:");
    return;
  }
  if(c == '#'){
    if(idx == 4){
      if(strcmp(buf, pin) == 0) abrir();
      else negar();
    } else {
      Serial.println("PIN incompleto");
      idx = 0;
    }
    return;
  }
  if(c >= '0' && c <= '9'){
    if(idx < 4){
      buf[idx++] = c;
      Serial.print("*");
      if(idx == 4) Serial.println();
    }
  }
}