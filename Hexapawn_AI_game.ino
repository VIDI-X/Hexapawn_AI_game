#include <Arduino.h>
#include "Hexapawn.h"

#define BAUD_RATE 115200

Hexapawn hexapawn;

void setup() {
  Serial.begin(BAUD_RATE);
  delay(1000);
  Serial.println("Hexapawn Pojacano Ucenje");
  hexapawn.train();
  Serial.println("Hexapawn treniranje zavrseno");
}

void loop() {
  // Igraj igru između dva agenta s naučenom strategijom
  int winner = hexapawn.playGame();
  if (winner == 1) {
    Serial.println("Bijeli agent je pobijedio!");
  } else if (winner == -1) {
    Serial.println("Crni agent je pobijedio!");
  } else {
    Serial.println("Nerijeseno!");
  }

  // Pauziraj između igara
  delay(5000);
}
