#include <Arduino.h>
#include <Servo.h>          
#include <NewPing.h>     
#include <string.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define AO 128
#define AL 64

Adafruit_SSD1306 oled(AO, AL, &Wire, -1);
DHT dht(2, DHT11);

const int N = 20;
float tH[N], hH[N];
int idx = 0, cnt = 0;
float tMax = -100, tMin = 200, hMax = -1, hMin = 200;
unsigned long tLec = 0;
const unsigned long TL = 2000;
int menu = 0;

void setup(){
  Serial.begin(9600);
  dht.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("ESTACION ARDUINO");
  oled.println("Menu: 1-5");
  oled.display();
}

void head(){
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("ESTACION ARDUINO");
  oled.drawLine(0, 10, 127, 10, SSD1306_WHITE);
}

void showM(){
  head();
  oled.setCursor(0, 18);
  oled.println("1 Temperatura");
  oled.println("2 Humedad");
  oled.println("3 Maximos");
  oled.println("4 Minimos");
  oled.println("5 Historial");
  oled.display();
}

void showT(float t, float h){
  head();
  oled.setTextSize(1);
  oled.setCursor(0, 20);
  oled.print("Temp: ");
  oled.print(t, 1);
  oled.println(" C");
  oled.print("Hum:  ");
  oled.print(h, 0);
  oled.println(" %");
  oled.println("Estado:");
  if(t > 30 || h > 80) oled.println("ALERTA");
  else if (t < 10 || h < 30) oled.println("SECO/FRIO");
  else oled.println("NORMAL");
  oled.display();
}

void showX(float a, float b, char *ti){
  head();
  oled.setCursor(0, 20);
  oled.println(ti);
  oled.setTextSize(2);
  oled.setCursor(0, 36);
  oled.print(a, 1);
  oled.println("C");
  oled.print(b, 0);
  oled.println("%");
  oled.display();
}

void showH(){
  head();
  oled.setCursor(0, 14);
  oled.println("Ultimas lecturas:");
  int m = cnt < 5 ? cnt : 5;
  int s = cnt < 5 ? 0 : idx - 5;
  if (s < 0) s += N;
  for (int i = 0; i < m; i++){
    int p = (s + i) % N;
    oled.print(tH[p], 1);
    oled.print("C ");
    oled.print(hH[p], 0);
    oled.println("%");
  }
  oled.display();
}

void loop(){
  if(Serial.available()){
    char c = Serial.read();
    if (c >= '1' && c <= '5') menu = c - '0';
    else if (c == '0') menu = 0;
  }
  if(millis() - tLec < TL) return;
  tLec = millis();
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if(isnan(t) || isnan(h)) return;
  tH[idx] = t;
  hH[idx] = h;
  idx = (idx + 1) % N;
  if(cnt < N) cnt++;
  if(t > tMax) tMax = t;
  if(t < tMin) tMin = t;
  if(h > hMax) hMax = h;
  if(h < hMin) hMin = h;
  Serial.print("T: ");
  Serial.print(t);
  Serial.print(" H: ");
  Serial.println(h);
  if (menu == 0) showT(t, h);
  else if (menu == 1) showT(t, h);
  else if (menu == 2) showX(t, h, "Actual");
  else if (menu == 3) showX(tMax, hMax, "Maximos");
  else if (menu == 4) showX(tMin, hMin, "Minimos");
  else if (menu == 5) showH();
}