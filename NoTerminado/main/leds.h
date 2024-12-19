#ifndef LEDS_H
#define LEDS_H

const int ledMid = 0;   // LED indicador de 0% a 50%
const int ledHigh = 4;  // LED indicador de 50% a 100%

void iniciarLEDs() {
  pinMode(ledMid, OUTPUT);
  pinMode(ledHigh, OUTPUT);
  digitalWrite(ledMid, LOW);
  digitalWrite(ledHigh, LOW);
}

void actualizarLEDs(int humedadPorcentaje) {
  if (humedadPorcentaje <= 50) {
    digitalWrite(ledMid, HIGH);
    digitalWrite(ledHigh, LOW);
  } else {
    digitalWrite(ledMid, LOW);
    digitalWrite(ledHigh, HIGH);
  }
}

#endif