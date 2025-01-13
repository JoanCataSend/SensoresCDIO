#include <Wire.h>                       // Biblioteca para la comunicación I2C
#include <Adafruit_ADS1X15.h>           // Biblioteca para el convertidor ADC ADS1115
#include <ESP8266WiFi.h>                // Biblioteca para conectar a la nube
#include <cmath>                        // Biblioteca matemática para cálculos

//-------------------------------- Definiciones de red y servidor ------------------------------------
const char WiFiSSID[] = "iClone de Joan";  
const char WiFiPSK[] = "123qweasd";      
const char Server_Host[] = "dweet.io";    
const int Server_HttpPort = 80;           
const String MyWriteAPIKey = "cdiocurso2024g06"; 

WiFiClient client;

//------------------------------- Definiciones de pines y variables globales -----------------------------------
#define CANAL_HUMEDAD 2         // Canal del ADS1115 asignado al sensor de humedad
#define PIN_ENERGIA 5           // Pin para alimentar el sensor de salinidad
#define CANAL_TEMPERATURA 1     // Canal del ADS1115 asignado al sensor de temperatura
#define CANAL_LUMINOSIDAD 3     // Canal del ADS1115 asignado al sensor de luminosidad
#define CANAL_PH 0              // Canal del ADS1115 asignado al sensor de pH

Adafruit_ADS1115 ads;           // Crear objeto ADS1115 para manejar el ADC

// Calibración y valores de humedad
int humedadSeco = 20200;        // Valor de referencia cuando el sensor está seco
int humedadMojado = 9800;       // Valor de referencia cuando el sensor está mojado

// Variables de calibración de sensores
float pendienteTemperatura = 35e-3;    
float voltajeReferencia = 0.79;         
float rangoMaximoVoltaje = 4.096;      

// Variables para el sensor de pH
#define samplingInterval 20  // Intervalo de muestreo (ms)
#define ArrayLength 40       // Número de muestras para calcular el promedio

int pHArray[ArrayLength];    // Arreglo para almacenar las muestras
int pHArrayIndex = 0;        // Índice para recorrer el arreglo de muestras
float slope = 3.0;           // Pendiente ajustable
float Offset = 2.3;          // Offset ajustable

//------------------------------------------ Declaración de funciones --------------------------------------

// Función para conectar a la red Wi-Fi
void connectWiFi() {
  Serial.print("Conectando a WiFi");
  WiFi.begin(WiFiSSID, WiFiPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Función para enviar datos al servidor
void enviarDatosNube(float humedadPorcentaje, float salinidad, float luminosidad, float temperatura) {
  if (client.connect(Server_Host, Server_HttpPort)) {
    String PostData = "GET /dweet/for/" + MyWriteAPIKey + 
                      "?humedad=" + String(humedadPorcentaje) + 
                      "&salinidad=" + String(salinidad) + 
                      "&luminosidad=" + String(luminosidad) + 
                      "&temperatura=" + String(temperatura);
    client.print(PostData);
    client.println(" HTTP/1.1");
    client.println("Host: " + String(Server_Host));
    client.println("Connection: close");
    client.println();

    Serial.print("Datos enviados: ");
    Serial.println(PostData);
    Serial.print("Servidor web: http://dweet.io/follow/");
    Serial.println(MyWriteAPIKey);
    client.stop();
  } else {
    Serial.println("Error conectando al servidor");
  }
}

//----------------------------------- Lectura de sensores ---------------------------------------------
float obtenerHumedad() {
  int lecturaHumedad = ads.readADC_SingleEnded(CANAL_HUMEDAD); // Leer el valor del sensor de humedad
  float porcentajeHumedad = map(lecturaHumedad, humedadSeco, humedadMojado, 0, 100); // Escalar a porcentaje
  return porcentajeHumedad;
}

float obtenerTemperatura() {
  const int muestrasTotales = 30;              // Número de muestras para promediar
  double suma = 0;

  for (int i = 0; i < muestrasTotales; i++) {
    suma += ads.readADC_SingleEnded(CANAL_TEMPERATURA);
    delay(50);
  }
  double promedio = suma / muestrasTotales;

  float voltaje = (promedio * rangoMaximoVoltaje) / 32767; // Convertir el valor ADC a voltaje
  float temperatura = (voltaje - voltajeReferencia) / pendienteTemperatura; // Ecuación de temperatura

  return temperatura;
}

void iniciarSensorLuminosidad() {
  Serial.println("Sensor de luminosidad inicializado.");
}

float obtenerLuminosidad() {
  int luzRaw = ads.readADC_SingleEnded(CANAL_LUMINOSIDAD);
  float voltajeLuz = (luzRaw * 4.096) / 32768.0 * 1000;

  Serial.print("Luminosidad: ");
  Serial.print(voltajeLuz);
  Serial.println(" mV");

  if (voltajeLuz >= 140 && voltajeLuz <= 230) {
    Serial.println("Luz natural");
  } else if (voltajeLuz >= 250 && voltajeLuz <= 310) {
    Serial.println("Linterna");
  } else if (voltajeLuz >= 50 && voltajeLuz <= 110) {
    Serial.println("Sombra");
  } else if (voltajeLuz <= 50) {
    Serial.println("Oscuridad total");
  }

  Serial.println("---------------------------");
  return voltajeLuz;
}

float obtenerSalinidad() {
  digitalWrite(PIN_ENERGIA, HIGH);               // Energizar el sensor de salinidad
  delay(500);                                    // Esperar para estabilizar la lectura
  int lecturaSalinidad = ads.readADC_SingleEnded(3); // Canal 3 del ADS1115 para la salinidad
  float salinidad = map(lecturaSalinidad, 0, 32767, 0, 100); // Escalar a porcentaje
  digitalWrite(PIN_ENERGIA, LOW);                // Apagar el sensor para ahorrar energía
  return salinidad;
}

// Calcular el promedio de un arreglo de muestras
float averageSample(int length, int* samples) {
  long sum = 0;
  for (int i = 0; i < length; i++) {
    sum += samples[i];
  }
  return (float)sum / length;
}

// Calcular el pH a partir del voltaje
float calculatePH(float voltage) {
  return slope * voltage + Offset;
}

float obtenerPH() {
  static unsigned long samplingTime = millis();
  static float voltage, pHValue;

  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = ads.readADC_SingleEnded(CANAL_PH);
    if (pHArrayIndex == ArrayLength) {
      pHArrayIndex = 0;
    }
    voltage = averageSample(ArrayLength, pHArray) * 4.096 / 32767.0; // Conversión a voltaje
    pHValue = calculatePH(voltage);
    samplingTime = millis();
  }
  return pHValue;
}

// Funciones de menú
void mostrarMenu() {
  Serial.println("\n=== MENU DE SENSORES ===");
  Serial.println("1. Leer Humedad");
  Serial.println("2. Leer Salinidad");
  Serial.println("3. Leer Temperatura");
  Serial.println("4. Leer Luminosidad");
  Serial.println("5. Leer pH");
  Serial.println("6. Salir del menú");
  Serial.println("7. Subir datos de todos los sensores");
  Serial.print("Seleccione una opción: ");
}

void procesarOpcion(int opcion) {
  switch (opcion) {
    case 1: {
      float humedad = obtenerHumedad();
      Serial.print("Humedad: ");
      Serial.print(humedad);
      Serial.println(" %");
      break;
    }
    case 2: {
      float salinidad = obtenerSalinidad();
      Serial.print("Salinidad: ");
      Serial.print(salinidad);
      Serial.println(" %");
      break;
    }
    case 3: {
      float temperatura = obtenerTemperatura();
      Serial.print("Temperatura: ");
      Serial.print(temperatura);
      Serial.println(" ºC");
      break;
    }
    case 4: {
      float luminosidad = obtenerLuminosidad();
      Serial.print("Luminosidad: ");
      Serial.print(luminosidad);
      Serial.println(" mV");
      break;
    }
    case 5: {
      float pH = obtenerPH();
      Serial.print("pH: ");
      Serial.println(pH, 2);
      break;
    }
    case 6: {
      Serial.println("Saliendo del menú...");
      break;
    }
    case 7: {
      float humedad = obtenerHumedad();
      float salinidad = obtenerSalinidad();
      float temperatura = obtenerTemperatura();
      float luminosidad = obtenerLuminosidad();
      enviarDatosNube(humedad, salinidad, luminosidad, temperatura);
      break;
    }
    default: {
      Serial.println("Opción no válida. Intente de nuevo.");
      break;
    }
  }
}

//----------------------------------- Configuración inicial -------------------------------------------
void setup() {
  Serial.begin(9600);                      // Configurar comunicación serial
  ads.setGain(GAIN_ONE);                   // Configuración de ganancia (1x: -4.096V a +4.096V)
  ads.begin(0x48);                         // Inicializar el ADS1115 con dirección I2C 0x48
  pinMode(PIN_ENERGIA, OUTPUT);            // Configurar el pin para controlar el sensor de salinidad

  connectWiFi();                           // Llamar a la función para conectar a Wi-Fi
  iniciarSensorLuminosidad();              // Inicializar el sensor de luminosidad
  mostrarMenu();                           // Mostrar el menú inicial
}

//----------------------------------- Bucle principal -------------------------------------------------
void loop() {
  if (Serial.available() > 0) {            // Verificar si el usuario ingresó un valor
    int opcion = Serial.parseInt();        // Leer la opción ingresada por el usuario
    procesarOpcion(opcion);                // Procesar la opción seleccionada

    if (opcion != 6) {                     // Si no seleccionó salir, volver a mostrar el menú
      mostrarMenu();
    }
  }
}
