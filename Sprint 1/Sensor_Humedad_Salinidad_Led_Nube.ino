#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>
// Importa las librerías necesarias para el ADS1115 (sensor ADC) y para Wi-Fi en el ESP8266.

// Inicialización del ADC ADS1115
Adafruit_ADS1115 ads;

// Definición de los pines para los LEDs indicadores de humedad
const int ledMid = 0;   // LED indicador de 0% a 50%
const int ledHigh = 4;  // LED indicador de 50% a 100%

// Variables para almacenar valores de calibración de humedad
int16_t humedadCrudoSeco;   // Valor leído cuando el sensor está seco
int16_t humedadCrudoMojado; // Valor leído cuando el sensor está mojado

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//-----------------------------------CONEXION CON LA WIFI Y NUBE----------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// Configuración de red Wi-Fi y servidor
const char WiFiSSID[] = "iClone de Joan";  
const char WiFiPSK[] = "123qweasd";      
const char Server_Host[] = "dweet.io";    // Servidor al que se enviarán los datos
const int Server_HttpPort = 80;           // Puerto HTTP del servidor
const String MyWriteAPIKey = "cdiocurso2024g06"; // Canal 
WiFiClient client; 

// Definición del pin para encender el sensor de salinidad
#define power_pin 5

// Puntos de datos para el polinomio de Lagrange 
const int numPoints = 5; 
float valorDigital[numPoints] = {462, 840, 860, 900, 930}; // Valores ADC
float gramosSal[numPoints] = {0, 5, 10, 15, 20};           // Valores correspondientes en gramos de sal

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
  Serial.println(WiFi.localIP()); // Imprime la IP asignada
}
// Función para enviar datos al servidor dweet.io
void enviarDatosNube(int humedadPorcentaje, float salinidad) {
  if (client.connect(Server_Host, Server_HttpPort)) { // Verifica conexión con el servidor
    String PostData = "GET /dweet/for/" + MyWriteAPIKey + "?humedad=" + String(humedadPorcentaje) + "&salinidad=" + String(salinidad);
    client.print(PostData); // Envía los datos como una solicitud HTTP GET
    client.println(" HTTP/1.1");
    client.println("Host: " + String(Server_Host));
    client.println("Connection: close");
    client.println();
    Serial.println("Datos enviados a la nube:");
    Serial.println(PostData);
    while (client.available()) { // Imprime la respuesta del servidor
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    client.stop(); // Cierra la conexión
  } else {
    Serial.println("Error conectando al servidor");
  }
}
// Función para calibrar el sensor de humedad
void calibrarSensorHumedad() {
  Serial.println("SECA EL SENSOR TOTALMENTE");
  delay(5000);
  Serial.println("5");
  delay(1000);
  Serial.println("4");
  delay(1000);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
  Serial.println("LEYENDO VALOR EN SECO...");
  humedadCrudoSeco = ads.readADC_SingleEnded(0); // Lee el valor en seco
  Serial.print("Valor seco: ");
  Serial.println(humedadCrudoSeco);

  Serial.println("PON EL SENSOR EN EL AGUA HASTA LA LÍNEA");
  delay(5000);
  Serial.println("5");
  delay(1000);
  Serial.println("4");
  delay(1000);
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
  Serial.println("LEYENDO VALOR EN MOJADO...");
  humedadCrudoMojado = ads.readADC_SingleEnded(0); // Lee el valor en mojado
  Serial.print("Valor mojado: ");
  Serial.println(humedadCrudoMojado);

  if (humedadCrudoSeco <= humedadCrudoMojado) { // Verifica coherencia en la calibración
    Serial.println("Error en la calibración. Asegúrate de que el sensor esté bien colocado.");
    while (1); // Detiene el programa si hay un error
  }

  Serial.println("Calibración completada. Iniciando lectura...");
  delay(1000);
  Serial.println("Iniciando lectura...");
}

// Configuración inicial
void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 bps

  if (!ads.begin()) { // Verifica la conexión con el ADS1115
    Serial.println("No se encontró el ADS1115. Verifica la conexión.");
    while (1); // Detiene el programa si no se encuentra
  }
  ads.setGain(GAIN_ONE); // Configura la ganancia del ADS1115
  Serial.println("ADS1115 iniciado correctamente.");

  pinMode(ledMid, OUTPUT); // Configura el pin del LED como salida
  pinMode(ledHigh, OUTPUT);
  pinMode(power_pin, OUTPUT); // Configura el pin de encendido del sensor

  digitalWrite(ledMid, LOW); // Apaga los LEDs al inicio
  digitalWrite(ledHigh, LOW);

  connectWiFi(); // Conecta a Wi-Fi
  calibrarSensorHumedad(); // Realiza la calibración del sensor de humedad
}

// Función para calcular la salinidad usando el polinomio de Lagrange
float calcularSalinidad(int adc0) {
  float salinidad = 0.0;

  for (int i = 0; i < numPoints; i++) { // Itera por cada punto
    float Li = 1.0;
    for (int j = 0; j < numPoints; j++) {
      if (j != i) { // Calcula los términos de Lagrange
        Li *= (adc0 - valorDigital[j]) / (valorDigital[i] - valorDigital[j]);
      }
    }
    salinidad += gramosSal[i] * Li; // Suma la contribución del punto
  }

  if (adc0 < valorDigital[0]) { // Limita la salinidad a 0 si está fuera de rango
    salinidad = 0.0;
  }

  return salinidad;
}

// Bucle principal
void loop() {
  int16_t humedadCrudo = ads.readADC_SingleEnded(0); // Lee el valor del sensor
  int humedadPorcentaje = map(humedadCrudo, humedadCrudoMojado, humedadCrudoSeco, 100, 0); // Mapea a porcentaje
  humedadPorcentaje = constrain(humedadPorcentaje, 0, 100); // Limita el valor entre 0 y 100

  Serial.print("Valor crudo del ADC: ");
  Serial.print(humedadCrudo);
  Serial.print(" -> Humedad: ");
  Serial.print(humedadPorcentaje);
  Serial.println("%");

  // Control de LEDs según la humedad
  if (humedadPorcentaje <= 50) {
    digitalWrite(ledMid, HIGH);
    digitalWrite(ledHigh, LOW);
  } else {
    digitalWrite(ledMid, LOW);
    digitalWrite(ledHigh, HIGH);
  }

  int16_t adc0;
  digitalWrite(power_pin, HIGH); // Activa el sensor de salinidad
  delay(100); // Espera para estabilizar
  adc0 = analogRead(A0); // Lee el valor del sensor
  digitalWrite(power_pin, LOW); // Apaga el sensor
  delay(100);

  float salinidad = calcularSalinidad(adc0); // Calcula la salinidad

  Serial.print("Valor digital de salinidad = ");
  Serial.println(adc0);
  Serial.print("Salinidad en gramos = ");
  Serial.println(salinidad);

  enviarDatosNube(humedadPorcentaje, salinidad); // Envía los datos al servidor
  delay(1000); // Espera 1 segundo antes de la próxima iteración
}
