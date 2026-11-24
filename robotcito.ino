#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"
#include <ESP32Servo.h>

#define ANCHO_PANTALLA 128 
#define ALTO_PANTALLA 64 

#define I2C2_SDA 4
#define I2C2_SCL 5

Servo miServo;
const int pinServo = 18;

String comando = "no";

BluetoothSerial SerialBT;

TwoWire Bus_I2C_1 = TwoWire(0);
TwoWire Bus_I2C_2 = TwoWire(1);

Adafruit_SSD1306 pantalla1(ANCHO_PANTALLA, ALTO_PANTALLA, &Bus_I2C_1, -1);
Adafruit_SSD1306 pantalla2(ANCHO_PANTALLA, ALTO_PANTALLA, &Bus_I2C_2, -1);

unsigned long timeprevio_blink = 0;
unsigned long blinktime = 0;

void setup() {
  miServo.attach(pinServo, 500, 2400);
  Serial.begin(115200);
  SerialBT.begin("ESP32_Test");

  // Inicializar bus 1 en los pines nativos D21 y D22
  Bus_I2C_1.begin(); 
  
  // Inicializar bus 2 en los pines disponibles D4 y D5
  Bus_I2C_2.begin(I2C2_SDA, I2C2_SCL, 400000); 
  pantalla1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pantalla2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pantalla1.clearDisplay();
  pantalla2.clearDisplay();
}
void derecha1() {
  pantalla1.clearDisplay();
  pantalla1.drawEllipse(64, 30, 35, 25,SSD1306_WHITE);
  pantalla1.fillRect(25, 20, 75, 75, SSD1306_BLACK);
  pantalla1.fillEllipse(64, 32, 25,20, SSD1306_WHITE);
  pantalla1.fillEllipse(74, 28, 12,12, SSD1306_BLACK);
  pantalla1.fillCircle(64, 25, 5, SSD1306_WHITE);
  pantalla1.display(); 
}
void derecha2() {
  pantalla2.clearDisplay();
  pantalla2.drawEllipse(64, 32, 35, 25,SSD1306_WHITE);
  pantalla2.fillRect(25, 20, 75, 75, SSD1306_BLACK);
  pantalla2.fillEllipse(64, 32, 25,20, SSD1306_WHITE);
  pantalla2.fillEllipse(74, 28, 12,12, SSD1306_BLACK);
  pantalla2.fillCircle(64, 25, 5, SSD1306_WHITE);
  pantalla2.display(); 
}
void izquierda1() {
  pantalla1.clearDisplay();
  pantalla1.drawEllipse(64, 30, 35, 25,SSD1306_WHITE);
  pantalla1.fillRect(25, 20, 75, 75, SSD1306_BLACK);
  pantalla1.fillEllipse(64, 32, 25,20, SSD1306_WHITE);
  pantalla1.fillEllipse(54, 28, 12,12, SSD1306_BLACK);
  pantalla1.fillCircle(64, 25, 5, SSD1306_WHITE);
  pantalla1.display(); 
}
void blink_face() {
  pantalla1.clearDisplay();
  pantalla1.drawEllipse(64, 30, 35, 25,SSD1306_WHITE);
  pantalla1.fillRect(25, 20, 75, 75, SSD1306_BLACK);
  for (int i = 22; i >17; i--){
    pantalla1.drawLine(40, 32, 88, i, SSD1306_WHITE);
  }
  for (int i = 42; i < 47; i++){
    pantalla1.drawLine(40, 32, 88, i, SSD1306_WHITE);
  }

  pantalla2.clearDisplay(); 
  pantalla2.drawEllipse(64, 32, 35, 25,SSD1306_WHITE);
  pantalla2.fillRect(25, 20, 75, 75, SSD1306_BLACK);
  for (int i = 22; i >17; i--){
    pantalla2.drawLine(88, 32, 40, i, SSD1306_WHITE);
  }
  for (int i = 42; i < 47; i++){
    pantalla2.drawLine(88, 32, 40, i, SSD1306_WHITE);
  }

  pantalla1.display(); 
  pantalla2.display(); 
  if (millis()- timeprevio_blink > 2800){
    timeprevio_blink = millis();
  }
}

void normal() {
  miServo.write(90); 
  if (millis()- timeprevio_blink > 2500) {
    blink_face();
  }
  if (millis()- timeprevio_blink < 2500) {
    izquierda1();
    derecha2();
  }
}

void derecha () {
  miServo.write(180); 
  if (millis()- timeprevio_blink > 2500) {
    blink_face();
  }
  if (millis()- timeprevio_blink < 2500) {
    // --- ACTUALIZACIÓN DE PANTALLA 2 ---
    derecha1();
    derecha2();
  }
}

void izquierda() {
  miServo.write(0); 

  if (millis()- timeprevio_blink > 2500) {
    blink_face();
  }
  if (millis()- timeprevio_blink < 2500) {
    izquierda1();
    // --- ACTUALIZACIÓN DE PANTALLA 2 ---
    pantalla2.clearDisplay();
    pantalla2.drawEllipse(64, 32, 35, 25,SSD1306_WHITE);
    pantalla2.fillRect(25, 20, 75, 75, SSD1306_BLACK);
    pantalla2.fillEllipse(64, 32, 25,20, SSD1306_WHITE);
    pantalla2.fillEllipse(54, 28, 12,12, SSD1306_BLACK);
    pantalla2.fillCircle(64, 25, 5, SSD1306_WHITE);

    pantalla2.display(); 
  }
}

void blinky() {
  if (millis()-blinktime<300) {
    izquierda1();
    pantalla2.clearDisplay(); 
    pantalla2.drawEllipse(64, 32, 35, 25,SSD1306_WHITE);
    pantalla2.fillRect(25, 20, 75, 75, SSD1306_BLACK);
    for (int i = 22; i >17; i--){
      pantalla2.drawLine(88, 32, 40, i, SSD1306_WHITE);
    }
    for (int i = 42; i < 47; i++){
      pantalla2.drawLine(88, 32, 40, i, SSD1306_WHITE);
    }
    pantalla2.display();
  }
  if (millis()-blinktime > 300) {
    normal();
    comando = "no";
  }
}

void bailecito() {
  blink_face();
  for (int i = 90; i > 30; i -=2){
    miServo.write(i);
  }
  for (int i = 90; i < 150; i +=2){
    miServo.write(i);
  }
}
void loop() {
  if (SerialBT.available()) {
    comando = SerialBT.readString();
    comando.trim();
    blinktime = millis();
  }

  if (comando == "iz") {
    izquierda();
  }
  else if (comando == "no") {
    normal();
  }
  else if (comando == "de") {
    derecha();
  }
  else if (comando == "blink"){
    blinky();
  }
  else if (comando == "baile"){
    bailecito();
  }
}