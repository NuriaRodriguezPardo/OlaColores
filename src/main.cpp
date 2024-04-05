#include <Arduino.h>

const int numLEDs = 5; // Número de LEDs en línea recta
const int ledPins[] = {2, 3, 4, 5, 6}; // Pines a los que están conectados los LEDs
const int buttonPin = 7; // Pin al que está conectado el botón

void activarOla();

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(buttonPin, INPUT_PULLUP); // Establece el pin del botón como entrada con resistencia pull-up
}

void loop() {
  if (digitalRead(buttonPin) == LOW) { // Si se presiona el botón
    activarOla(); // Activa la animación de la ola
  }
}

void activarOla() {
  // Enciende los LEDs en secuencia ascendente
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
  }

  // Apaga los LEDs en secuencia descendente
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], LOW);
    delay(100);
  }
}
