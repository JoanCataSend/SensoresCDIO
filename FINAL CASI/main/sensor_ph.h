#ifndef SENSOR_PH_H
#define SENSOR_PH_H

#include <Adafruit_ADS1X15.h>

#define channelValue 0
#define samplingInterval 20
#define ArrayLength 40

extern Adafruit_ADS1115 ads;

float pHArray[ArrayLength];
int pHArrayIndex = 0;

// Variables para calibración manual
float slope = 3.0;
float Offset = 2.3;

// Función para inicializar el sensor de pH
void iniciarSensorPH() {
  Serial.println("Sensor de pH inicializado.");
}

// Función para leer el pH
float leerPH() {
  static unsigned long samplingTime = millis();
  float pHValue = 0.0, voltage = 0.0;

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = ads.readADC_SingleEnded(channelValue);

    if (pHArrayIndex == ArrayLength) {
      pHArrayIndex = 0;
    }

    voltage = averageSample(ArrayLength, pHArray) * 4.096 / 32768.0;
    pHValue = calculatePH(voltage);
    samplingTime = millis();
  }

  Serial.print("Voltaje: ");
  Serial.println(voltage, 2);
  Serial.print("pH: ");
  Serial.println(pHValue, 2);

  return pHValue;
}

float averageSample(int length, float* samples) {
  float sum = 0.0;
  for (int i = 0; i < length; i++) {
    sum += samples[i];
  }
  return sum / length;
}

float calculatePH(float voltage) {
  return slope * voltage + Offset;
}

#endif