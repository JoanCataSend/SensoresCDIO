#ifndef SENSOR_PH_H
#define SENSOR_PH_H

#include <Adafruit_ADS1X15.h>

// Definir el canal del ADS1115 por el que leeremos la tensión del sensor de pH
#define channelValue 0  // Usamos el canal A0
#define samplingInterval 20  // Intervalo de muestreo (ms)
#define printInterval 800    // Intervalo para imprimir datos (ms)
#define ArrayLength 40       // Número de muestras para calcular el promedio

int pHArray[ArrayLength];    // Arreglo para almacenar las muestras
int pHArrayIndex = 0;        // Índice para recorrer el arreglo de muestras

// Crear el objeto ADS1115
extern Adafruit_ADS1115 ads;  // Usar el mismo objeto global

// Variables para calibración manual
float slope = 3.0;  // Pendiente ajustable
float Offset = 0.0;  // Offset ajustable

// Declaración de funciones
void iniciarSensorPH();
float leerPH();
float averageSample(int length, int* samples);
float calculatePH(float voltage);

// Implementación de funciones

void iniciarSensorPH() {
  Serial.println("Inicializando el medidor de pH...");
  ads.setGain(GAIN_ONE);  // Configurar la ganancia en 1 (±4.096V)
  Serial.println("Sensor de pH inicializado.");
}

float leerPH() {
  static unsigned long samplingTime = millis();
  static float pHValue = 0.0, voltage = 0.0;

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = ads.readADC_SingleEnded(channelValue);
    if (pHArrayIndex == ArrayLength) {
      pHArrayIndex = 0;
    }
    voltage = averageSample(ArrayLength, pHArray) * 4.096 / 32767.0;
    pHValue = calculatePH(voltage);
    samplingTime = millis();
  }

  Serial.print("Voltaje: ");
  Serial.print(voltage, 2);
  Serial.print(" V | pH: ");
  Serial.println(pHValue, 2);

  return pHValue;
}

float averageSample(int length, int* samples) {
  long sum = 0;
  for (int i = 0; i < length; i++) {
    sum += samples[i];
  }
  return (float)sum / length;
}

float calculatePH(float voltage) {
  return slope * voltage + Offset;
}

#endif