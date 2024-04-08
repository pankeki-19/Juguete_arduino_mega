#include <SPI.h>
#include <MFRC522.h>

// Pines de LED y botones
#define LED_PIN 4
#define RST_PIN 5
const int SS[3] = {53, 46, 42}; // Pines de selección para los lectores RFID
const int BUTTON_MAIN = 6; // Pin para el botón principal
const int BUTTON_PRIZE = 7; // Pin para el botón de premio
const int BUTTON_PANIC = 8; // Pin para el botón de pánico
const int LED_RGB[3] = {9, 10, 11}; // Pines para el LED RGB (R, G, B)
const int LED_PRIZE = 12; // Pin para el LED del botón de premio

MFRC522 mfrc[3] = {
  MFRC522(SS[0], RST_PIN),
  MFRC522(SS[1], RST_PIN),
  MFRC522(SS[2], RST_PIN)
};

void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_MAIN, INPUT_PULLUP);
  pinMode(BUTTON_PRIZE, INPUT_PULLUP);
  pinMode(BUTTON_PANIC, INPUT_PULLUP);
  pinMode(LED_PRIZE, OUTPUT);
  for(int i = 0; i < 3; i++) {
    pinMode(LED_RGB[i], OUTPUT);
    mfrc[i].PCD_Init();
  }
}

void checkForCard(MFRC522& mfrc522) {
  // La implementación se mantiene similar, se añadirá lógica adicional según necesidad
}

void loop() {
  for (int i = 0; i < 3; i++) { 
    checkForCard(mfrc[i]);
  }

  // Aquí se añadirá la lógica de manejo de botones y LED

  delay(250); // Este delay podría ajustarse según tus necesidades
}
