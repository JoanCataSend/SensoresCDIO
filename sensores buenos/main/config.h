#ifndef CONFIG_H
#define CONFIG_H

#include <ESP8266WiFi.h>

// Configuración Wi-Fi y servidor
const char WiFiSSID[] = "FabiVD";  
const char WiFiPSK[] = "Snoopy070506";      
const char Server_Host[] = "dweet.io";    
const int Server_HttpPort = 80;           
const String MyWriteAPIKey = "cdiocurso2024g06"; 

WiFiClient client;

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
void enviarDatosNube(int humedadPorcentaje, float salinidad, float temperatura) {
  if (client.connect(Server_Host, Server_HttpPort)) {
    String PostData = "GET /dweet/for/" + MyWriteAPIKey + "?humedad=" + String(humedadPorcentaje) + "&salinidad=" + String(salinidad) + "&temperatura=" + String(temperatura);
    client.print(PostData);
    client.println(" HTTP/1.1");
    client.println("Host: " + String(Server_Host));
    client.println("Connection: close");
    client.println();
    Serial.print("Servidor web: http://dweet.io/follow/");
    Serial.println(MyWriteAPIKey);
    client.stop();
  } else {
    Serial.println("Error conectando al servidor");
  }
}

#endif
