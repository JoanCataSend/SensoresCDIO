#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>

Adafruit_ADS1115 ads;

// Pines para los LEDs
//const int ledLow = 5;   // LED para 0% - 33% (A1)
const int ledMid = 0;   // LED para 0% - 50% (A2)
const int ledHigh = 4;  // LED para 66% - 100% (A3)

// Variables globales para calibración
int16_t humedadCrudoSeco;   // Valor en seco
int16_t humedadCrudoMojado; // Valor en mojado
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//-----------------------------------CONEXION CON LA NUBE-----------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// Configuración Wi-Fi y servidor
const char WiFiSSID[] = "iClone de Joan";
const char WiFiPSK[] = "123qweasd";
const char Server_Host[] = "dweet.io";
const int Server_HttpPort = 80;
const String MyWriteAPIKey = "cdiocurso2024g06"; // Canal de Dweet.io
WiFiClient client;
void connectWiFi() {
  byte ledStatus = LOW;
  Serial.print("Conectando a WiFi");
  WiFi.begin(WiFiSSID, WiFiPSK);
  while (WiFi.status() != WL_CONNECTED) {
    // Mostrar progreso de conexión
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}
void enviarDatosNube(int humedadPorcentaje) {
  if (client.connect(Server_Host, Server_HttpPort)) {
    String PostData = "GET /dweet/for/" + MyWriteAPIKey + "?humedad=" + String(humedadPorcentaje);
    client.print(PostData);
    client.println(" HTTP/1.1");
    client.println("Host: " + String(Server_Host));
    client.println("Connection: close");
    client.println();
    Serial.println("Datos enviados a la nube:");
    Serial.println(PostData);
    // Leer respuesta del servidor
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    client.stop();
  } else {
    Serial.println("Error conectando al servidor");
  }
}
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------CONEXION CON EL SENSOR Y LOS LEDS---------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  // Configuración del ADS1115
  if (!ads.begin()) {
    Serial.println("No se encontró el ADS1115. Verifica la conexión.");
    while (1);
  }
  ads.setGain(GAIN_ONE); // Configuramos el rango de entrada a ±4.096V
  Serial.println("ADS1115 iniciado correctamente.");

  // Configurar pines de los LEDs como salida
//  pinMode(ledLow, OUTPUT);
  pinMode(ledMid, OUTPUT);
  pinMode(ledHigh, OUTPUT);

  // Asegurarnos de que los LEDs estén apagados al inicio
//  digitalWrite(ledLow, LOW);
  digitalWrite(ledMid, LOW);
  digitalWrite(ledHigh, LOW);

  // Conectar a Wi-Fi
  connectWiFi();

  // Calibración del sensor
  delay(5000);
  Serial.println("SECA EL SENSOR TOTALMENTE");
  delay(5000);
  Serial.println("LEYENDO VALOR EN SECO...");
  humedadCrudoSeco = ads.readADC_SingleEnded(0);
  Serial.print("Valor seco: ");
  Serial.println(humedadCrudoSeco);

  Serial.println("PON EL SENSOR EN EL AGUA HASTA LA LÍNEA");
  delay(5000);
  Serial.println("LEYENDO VALOR EN MOJADO...");
  humedadCrudoMojado = ads.readADC_SingleEnded(0);
  Serial.print("Valor mojado: ");
  Serial.println(humedadCrudoMojado);

  // Verificar si la calibración tiene sentido
  if (humedadCrudoSeco <= humedadCrudoMojado) {
    Serial.println("Error en la calibración. Asegúrate de que el sensor esté bien colocado.");
    while (1);
  }

  Serial.println("Calibración completada. Iniciando lectura...");
}

void loop() {
  // Leer el valor crudo del canal A0
  int16_t humedadCrudo = ads.readADC_SingleEnded(0);

  // Mapear el valor crudo al rango de 0-100 para el porcentaje de humedad
  int humedadPorcentaje = map(humedadCrudo, humedadCrudoMojado, humedadCrudoSeco, 100, 0);

  // Limitar el rango a 0-100%
  humedadPorcentaje = constrain(humedadPorcentaje, 0, 100);

  // Mostrar los valores en el monitor serie para depuración
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

  // Enviar los datos a la nube
  enviarDatosNube(humedadPorcentaje);

  delay(1000); // Esperar 1 segundos antes de la siguiente lectura
}
