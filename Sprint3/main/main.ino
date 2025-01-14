2#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include "config.h"
#include "sensor_humedad.h"
#include "sensor_salinidad.h"
#include "sensor_temperatura.h"
#include "sensor_luminosidad.h"
#include "sensor_ph.h"

// Objeto ADS1115
//Adafruit_ADS1115 ads;

void setup() {
  Serial.begin(9600);
  connectWiFi();           // Conectar al Wi-Fi
  iniciarSensorHumedad();  // Inicializar el sensor de humedad
  iniciarSensorSalinidad();// Inicializar el sensor de salinidad
  iniciarSensorTemperatura(); // Inicializar el sensor de temperatura
  iniciarSensorLuminosidad(); // Inicializar el sensor de luminosidad
  iniciarSensorPH();       // Inicializar el sensor de pH
  ads.begin();             // Inicializar ADS1115
  ads.setGain(GAIN_ONE);
  Serial.println("Sistema inicializado");
}

void loop() {
  Serial.println("Selecciona el sensor a utilizar:");
  Serial.println("1. Sensor de Humedad");
  Serial.println("2. Sensor de Salinidad");
  Serial.println("3. Sensor de Temperatura");
  Serial.println("4. Sensor de Luminosidad");
  Serial.println("5. Sensor de pH");
  Serial.println("6. Leer y enviar todos los sensores");
  Serial.println("Escribe 's' para volver al menú de opciones.");

  while (Serial.available() == 0) {} // Esperar a que el usuario ingrese una opción
  String input = Serial.readStringUntil('\n');
  input.trim(); // Eliminar espacios en blanco

  if (input.equalsIgnoreCase("s")) {
    return; // Volver al menú de opciones
  }

  int opcion = input.toInt();

  while (true) {
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();
      if (command.equalsIgnoreCase("s")) {
        break; // Salir del bucle y volver al menú de opciones
      }
    }

    int humedadPorcentaje;
    float salinidad;
    float temperatura;
    float voltajeLuz;
    float pH;

    switch (opcion) {
      case 1:
        Serial.println("Leyendo sensor de humedad...");
        humedadPorcentaje = leerHumedad();
        Serial.println("Humedad: " + String(humedadPorcentaje) + "%");
        enviarDatosNube("humedad", humedadPorcentaje);
        break;
      case 2:
        Serial.println("Leyendo sensor de salinidad...");
        salinidad = leerSalinidad();
        Serial.println("Salinidad: " + String(salinidad) + " gramos");
        enviarDatosNube("salinidad", salinidad);
        break;
      case 3:
        Serial.println("Leyendo sensor de temperatura...");
        temperatura = leerTemperatura();
        Serial.println("Temperatura: " + String(temperatura) + " °C");
        enviarDatosNube("temperatura", temperatura);
        break;
      case 4:
        Serial.println("Leyendo sensor de luminosidad...");
        voltajeLuz = leerLuminosidad();
        Serial.println("Luminosidad: " + String(voltajeLuz) + " mV");
        enviarDatosNube("luminosidad", voltajeLuz);
        break;
      case 5:
        Serial.println("Leyendo sensor de pH...");
        pH = leerPH();
        Serial.println("pH: " + String(pH, 2));
        enviarDatosNube("ph", pH);
        break;
      case 6:
        Serial.println("Leyendo y enviando todos los sensores...");
        humedadPorcentaje = leerHumedad();
        salinidad = leerSalinidad();
        temperatura = leerTemperatura();
        voltajeLuz = leerLuminosidad();
        pH = leerPH();
        Serial.println("Humedad: " + String(humedadPorcentaje) + "%");
        Serial.println("Salinidad: " + String(salinidad) + " gramos");
        Serial.println("Temperatura: " + String(temperatura) + " °C");
        Serial.println("Luminosidad: " + String(voltajeLuz) + " mV");
        Serial.println("pH: " + String(pH, 2));
        enviarDatosNube("humedad", humedadPorcentaje);
        enviarDatosNube("salinidad", salinidad);
        enviarDatosNube("temperatura", temperatura);
        enviarDatosNube("luminosidad", voltajeLuz);
        enviarDatosNube("ph", pH);
        break;
      default:
        Serial.println("Opción no válida. Por favor, selecciona una opción del 1 al 6.");
        break;
    }

    delay(1000); // Espera 1 segundo antes de la próxima lectura
  }
}