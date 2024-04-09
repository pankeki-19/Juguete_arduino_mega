#include <SPI.h>
#include <MFRC522.h>

// Pin del led
#define LED_PIN 4

// Pin para resetear todos los módulos
#define RST_PIN 5

// Pines del botón de pánico y el botón principal
#define PANIC_BUTTON_PIN 6
#define MAIN_BUTTON_PIN 7

// Pines para el LED RGB del botón principal
#define LED_RED_PIN 8
#define LED_GREEN_PIN 9
#define LED_BLUE_PIN 10

// Pin para la bocina de membrana
#define BUZZER_PIN 11

// Array de pines de Slave Select de los RFIDs por SPI
const int SS[3] = {53, 46, 42};

// Creamos un array de instancias MFRC522
MFRC522 mfrc[3] = {
  MFRC522(SS[0], RST_PIN),
  MFRC522(SS[1], RST_PIN),
  MFRC522(SS[2], RST_PIN)
};

bool panicMode = false;

void setup() {
  Serial.begin(115200);
  SPI.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(PANIC_BUTTON_PIN, INPUT_PULLUP); // Suponemos que el botón está conectado a GND cuando se presiona
  pinMode(MAIN_BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  for (int i=0; i<3; i++) { mfrc[i].PCD_Init(); }
}

void checkForCard(MFRC522& mfrc522) {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("UID:");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    mfrc522.PICC_HaltA();

    digitalWrite(LED_PIN, HIGH);
    delay(3000);
    digitalWrite(LED_PIN, LOW);
  }
}

void loop() {
  for (int i=0; i<3; i++) { 
    checkForCard(mfrc[i]);
  }

  if (digitalRead(PANIC_BUTTON_PIN) == LOW) {
    panicMode = !panicMode;
    while(digitalRead(PANIC_BUTTON_PIN) == LOW); // Espera a que se suelte el botón
  }

  if (panicMode) {
    // Parpadeo lento del LED azul y sonido de la bocina
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, HIGH); // Enciende solo el LED azul
    tone(BUZZER_PIN, 1000); // Emite un sonido
    delay(1000);
    digitalWrite(LED_BLUE_PIN, LOW);
    noTone(BUZZER_PIN); // Detiene el sonido
    delay(1000);
  } else {
    // Asegura que el LED y la bocina estén apagados cuando no esté en modo pánico
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  delay(250);
}
