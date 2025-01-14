#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Crear una instancia del ADS1115
Adafruit_ADS1115 ads;

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600); // Cambiado a 9600 para evitar problemas comunes de baud rate
  Serial.println("Iniciando...");

  // Inicializar el ADS1115
  if (!ads.begin()) {
    Serial.println("Error al inicializar el ADS1115. Verifica las conexiones.");
    while (1);
  }

  Serial.println("ADS1115 inicializado correctamente.");

  // Configurar el rango de ganancia
  ads.setGain(GAIN_ONE); // Rango de +/- 4.096V
}

void loop() {
  // Leer el valor del canal A1
  int16_t valorLuz = ads.readADC_SingleEnded(3);

  // Convertir el valor a voltaje (basado en el rango de ganancia configurado)
  float voltaje = valorLuz * (4.096 / 32768.0); // Conversión correcta para GAIN_ONE

  // Imprimir el resultado
  Serial.print("Valor bruto: ");
  Serial.print(valorLuz);
  Serial.print("\tVoltaje: ");
  Serial.print(voltaje, 3);
  Serial.println(" V");

  // Esperar un poco antes de la próxima lectura
  delay(500);
}
