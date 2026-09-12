#include <Arduino.h>
#include <Servo.h>          
#include <NewPing.h>     
#include <DHT.h>     
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#include <avr/pgmspace.h>

const int LED_ROJO = 8;
const int LED_AMARILLO = 9;
const int LED_VERDE = 10;
const int BOTON = 2;
const int BUZZER = 11;

const unsigned long T_VERDE = 5000;
const unsigned long T_AMARILLO = 2000;
const unsigned long T_ROJO = 5000;
const unsigned long T_CRUCE = 8000;

enum Estado {VERDE, AMARILLO, ROJO, CRUCE};
Estado estado = VERDE;

unsigned long tInicio = 0;
unsigned long tDebounce = 0;
bool peticion = false;
bool ultimoBoton = HIGH;

void setup(){
    pinMode(LED_ROJO, OUTPUT);
    pinMode(LED_AMARILLO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(BOTON, INPUT_PULLUP);
    digitalWrite(LED_VERDE, HIGH);
    tInicio = millis();
}

void loop(){
    bool lectura = digitalRead(BOTON);
    if(lectura == LOW && ultimoBoton == HIGH && millis() - tDebounce > 50){
        peticion = true;
        tDebounce = millis();
    }
    ultimoBoton = lectura;

    unsigned long ahora = millis();
    unsigned long transcurrido = ahora - tInicio;

    switch(estado){
        case VERDE:
            if(transcurrido >= T_VERDE){
                digitalWrite(LED_VERDE, LOW);
                digitalWrite(LED_AMARILLO, HIGH);
                estado = AMARILLO;
                tInicio = ahora;
            }
        break;

        case AMARILLO:
            if(transcurrido >= T_AMARILLO){
                digitalWrite(LED_AMARILLO, LOW);
                digitalWrite(LED_ROJO, HIGH);
                estado = ROJO;
                tInicio = ahora;
            }
        break;

        case ROJO:
            if(peticion){
                peticion = false;
                estado = CRUCE;
                tInicio = ahora;
            } else if (transcurrido >= T_ROJO){
                digitalWrite(LED_ROJO, LOW);
                digitalWrite(LED_VERDE, HIGH);
                estado = VERDE;
                tInicio = ahora;
            }
        break;

        case CRUCE:
            if(transcurrido < T_CRUCE){
                if((transcurrido / 300) % 2 == 0) tone(BUZZER, 1000);
                else noTone(BUZZER);
            } else {
                noTone(BUZZER);
                digitalWrite(LED_ROJO, LOW);
                digitalWrite(LED_VERDE, HIGH);
                estado = VERDE;
                tInicio = ahora;
            }
        break;
    }
}