#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void setup() {
  Serial.begin(9600);
  if (!ads.begin()) {
    Serial.println("No se encontró el ADS1115. Verifica la conexión.");
    while (1);
  }
  ads.setGain(GAIN_ONE);
  Serial.println("ADS1115 iniciado correctamente.");
}

void loop() {
  int16_t valorDigital = ads.readADC_SingleEnded(2);
  float Vmax = 4.096;
  int resolucionADC = 32768;
  float voltaje = (valorDigital * Vmax) / resolucionADC;
  Serial.print("Valor digital: ");
  Serial.print(valorDigital);
  Serial.print(" - Voltaje: ");
  Serial.println(voltaje, 6);
  delay(1000);
}