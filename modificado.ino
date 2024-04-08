#include <SPI.h>
#include <MFRC522.h>

// pin del led
#define LED_PIN 4

// Pin para resetear todos los modulos
#define RST_PIN 5

// array de pines de Slave Select de los RFIDs por SPI
const int SS[3] = {53, 46, 42};

// creamos un array de instancias, para no andar seleccionando variables diferentes
MFRC522 mfrc[3] = {
  // una para cada pin
  MFRC522(SS[0], RST_PIN),
  MFRC522(SS[1], RST_PIN),
  MFRC522(SS[2], RST_PIN)
};


void setup() {
  // Inicia puerto serial
  Serial.begin(115200);
  // Inicia comunicación por SPI
  SPI.begin();
  // configurar el led como salida
  pinMode(LED_PIN, OUTPUT);
  // arrancar todas las interfaces RFID
  for (int i=0; i<3; i++) { mfrc[i].PCD_Init(); }
}

void checkForCard(MFRC522& mfrc522) {
  // Verifica si hay una nueva tarjeta presente y la lee
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Imprime el UID de la tarjeta en formato hexadecimal
    Serial.print("UID:");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    // Desactiva la tarjeta
    mfrc522.PICC_HaltA();
    // parpadea un led
    digitalWrite(LED_PIN, HIGH);
    delay(3000);
    digitalWrite(LED_PIN, LOW);
  }
}

void loop() {
  // leer todos los RFIDs
  for (int i=0; i<3; i++) { 
    checkForCard(mfrc[i]);
  }

  delay(250); // Este delay podría ajustarse según tus necesidades
}