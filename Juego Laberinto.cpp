#include <Arduino.h>
#include <Servo.h>          
#include <NewPing.h>     
#include <DHT.h>     
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <avr/pgmspace.h>

const int azul = 8;
const int verde = 9;
const int rojo = 10;
const int amarillo = 11;
const int bz = 12;
const int FW = 14;
const int FH = 9;

const char mp[4][FH][FW + 1] = {
  {"##############","#P           #","# #######    #","#            #","#    ######  #","#            #","#  ########  #","#           X#","##############"},
  {"##############","#P #         #","#  #  #####  #","#  #  #   #  #","#  #  # X #  #","#  #  #   #  #","#  #  #####  #","#            #","##############"},
  {"##############","#P  #        #","# # # #####  #","# # # #   #  #","# # # # # #  #","# #   # # #  #","# ##### # #  #","#       #   X#","##############"},
  {"##############","#P #       # #","#  # ##### # #","#  # #   # # #","#  # # # # # #","#  #   #   # #","#  ######### #","#           X#","##############"}
};

char w[FH][FW + 1];
int nv, px, py;

void setup(){
  Serial.begin(9600);
  pinMode(azul, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(bz, OUTPUT);
  delay(500);
  carga();
}

void carga(){
  for(int y = 0; y < FH; y++){
    for(int x = 0; x < FW; x++){
      w[y][x] = mp[nv][y][x];
      if (w[y][x] == 'P') { py = y; px = x; }
    }
    w[y][FW] = 0;
  }
  dib();
}

void dib(){
  Serial.print("\nMAPA ");
  Serial.println(nv + 1);
  for(int y = 0; y < FH; y++) Serial.println(w[y]);
}

void leds() {
  digitalWrite(azul, LOW);
  digitalWrite(verde, LOW);
  digitalWrite(rojo, LOW);
  digitalWrite(amarillo, LOW);
}

void mover(int dx, int dy, int led){
  int nx = px + dx, ny = py + dy;
  if(nx < 0 || nx >= FW || ny < 0 || ny >= FH || w[ny][nx] == '#'){tone(bz, 800, 100); return;}
  leds();
  digitalWrite(led, HIGH);
  char c = w[ny][nx];
  w[py][px] = ' ';
  w[ny][nx] = 'P';
  px = nx; py = ny;
  if(c == 'X'){
    Serial.print("\nGANASTE EL MAPA ");
    Serial.println(nv + 1);
    nv++;
    if(nv >= 4){Serial.println("JUEGO COMPLETO"); nv = 0;}
    Serial.print("Cargando mapa ");
    Serial.print(nv + 1);
    Serial.println("...");
    delay(1500);
    carga();
    return;
  }
  dib();
}

void loop(){
  if(!Serial.available()) return;
  char c = Serial.read();
  if(c == 'w' || c == 'W') mover(0, -1, azul);
  else if(c == 'a' || c == 'A') mover(-1, 0, verde);
  else if(c == 's' || c == 'S') mover(0, 1, rojo);
  else if(c == 'd' || c == 'D') mover(1, 0, amarillo);
}