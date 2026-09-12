#include "BluetoothSerial.h"
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

BluetoothSerial SerialBT;

#define ENA 5
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22
#define ENB 23

#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 25
#define OLED_SCL 26
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char comando;

const int freq = 1000;
const int channelA = 0;
const int channelB = 1;
const int resolution = 8;

int velocidad = 180;

float temperaturas[128];
int indexGrafica = 0;

void detener();
void derecha();
void izquierda();
void adelante();
void atras();

void setup(){
  Serial.begin(115200);

  SerialBT.begin("ESP32_CAR");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  
  ledcSetup(channelA, freq, resolution);
  ledcAttachPin(ENA, channelA);

  ledcSetup(channelB, freq, resolution);
  ledcAttachPin(ENB, channelB);

  
  ledcWrite(channelA, velocidad);
  ledcWrite(channelB, velocidad);

  detener();

  
  dht.begin();

  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    Serial.println("Error OLED");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  for(int i = 0; i < 128; i++)
    temperaturas[i] = 0;

  Serial.println("Bluetooth listo");
}

void adelante(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void atras(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void izquierda(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void derecha(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void detener(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void mostrarGraficaTemperatura(){
  float t = dht.readTemperature();
  
  if(!isnan(t)){
    temperaturas[indexGrafica] = t;
    indexGrafica = (indexGrafica + 1) % 128;
  }
  

  display.clearDisplay();
  
  display.setCursor(0, 0);
  display.print("Temperatura:");
  display.setCursor(90, 0);
  display.print(t, 1);
  display.print("C");
  

  display.drawLine(10, 15, 10, 60, SSD1306_WHITE);
  display.drawLine(10, 60, 125, 60, SSD1306_WHITE);
  
  for(int i = 0; i < 127; i++){
    int x1 = 10 + i;
    int x2 = 10 + i + 1;
    
    int y1 = 60 - (int)((temperaturas[(indexGrafica + i) % 128] / 50.0) * 45);
    int y2 = 60 - (int)((temperaturas[(indexGrafica + i + 1) % 128] / 50.0) * 45);
    
    if(y1 < 15) y1 = 15;
    if(y1 > 60) y1 = 60;
    if(y2 < 15) y2 = 15;
    if(y2 > 60) y2 = 60;
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }
  display.setCursor(0, 15);
  display.print("50");
  display.setCursor(0, 55);
  display.print("0");
  display.display();
}

void loop(){ 
  mostrarGraficaTemperatura();
  if (SerialBT.available()){ 
    comando = SerialBT.read(); 
    Serial.println(comando); 
    switch (comando){ 
      case 'F': adelante(); break; 
      case 'B': atras(); break; 
      case 'L': izquierda(); break; 
      case 'R': derecha(); break; 
      case 'S': detener(); break; 
    } 
  }  
  delay(50);
}