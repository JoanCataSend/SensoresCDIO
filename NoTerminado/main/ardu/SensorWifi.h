#include "FuncionesExtras.h"
#include <ESP8266WiFi.h>


class SensorWifi
{
    //ATRIBUTOS
  private:
    String wifi;
    String password;

    //FUNCIONES EXTRA
    FuncionesExtras func;
    const int direccionMemoriaW = 33;
    const int direccionMemoriaW2 = 34;

    const int direccionMemoriaP = 35;
    const int direccionMemoriaP2 = 36;

    //CONFIGURACION DEL SERVIDOR
    const String Rest_Host = "api.thingspeak.com";
    String MyWriteAPIKey = "4CDFJI2NMB86J7L2"; // API KEY
    const String Server_Host = "api.thingspeak.com";
    const int Server_HttpPort = 80;

    //CONFIGURACION DEL WIFI
    String WiFiSSID = "iPhone de Javi"; //WIFI
    String WiFiPSK = "jarabito"; //CONTRASEÑA
    const int LED_PIN = 5;

    //SERVIDOR REST
#define REST_SERVER_THINGSPEAK

    WiFiClient client;

  public:

    //CONSTRUCTOR
    SensorWifi() {
      //DEVOLVEMOS LOS VALORES DE LA EEPROM A LAS VARIABLES DE ESTA CLASE
      //this->WiFiSSID = this->func.getDatos(direccionMemoriaW, direccionMemoriaW2);
      //this->WiFiPSK = this->func.getDatos(direccionMemoriaP, direccionMemoriaP2);

      configuracionWifi();
    }

    //CONSTRUCTOR POR PARÁMETROS
    SensorWifi(String wifi, String password) {

      //GUARDAMOS LOS DATOS QUE NOS PASAN POR PARÁMETROS
      //this->func.setDatos(wifi, direccionMemoriaW, direccionMemoriaW2);
      //this->func.setDatos(password, direccionMemoriaP, direccionMemoriaP2);

      //ASIGNAMOS A LAS VARIABLES DE LA CLASE LOS VALORES DE LOS PARÁMETROS
      //this->WiFiSSID = wifi;
      //this->WiFiPSK = password;

      configuracionWifi();
    }

    void configuracionWifi() {

      connectWiFi();
      digitalWrite(LED_PIN, HIGH);

      Serial.print("Server_Host: ");
      Serial.println(Server_Host);
      Serial.print("Port: ");
      Serial.println(String( Server_HttpPort ));
      Serial.print("Server_Rest: ");
      Serial.println(Rest_Host);


    }


    void connectWiFi() {
      byte ledStatus = LOW;

      Serial.print("MAC: ");
      Serial.println(WiFi.macAddress());

      WiFi.begin(WiFiSSID, WiFiPSK);

      while (WiFi.status() != WL_CONNECTED) //SI NO ESTOY CONECTADO HACEMOS LA CONEXION
      {
        // Blink the LED
        digitalWrite(LED_PIN, ledStatus); // Write LED high/low
        ledStatus = (ledStatus == HIGH) ? LOW : HIGH;

        delay(500);
      }

      Serial.println( "WiFi Connected" );
      Serial.println(WiFi.localIP()); // Print the IP address

    }

    void enviarDatosServidor(double humedad, double salinidad, double temperatura, double iluminacion, double latitud, double longitud) {

      connectWiFi(); //CONECTARSE AL WIFI

      if (client.connect( Server_Host , Server_HttpPort )) {

        //CONCATENAR LATITUD Y LONGIUD
        String auxLatLong = "";
        auxLatLong = latitud;
        auxLatLong += ",";
        auxLatLong += longitud;

        String PostData = "api_key=" + MyWriteAPIKey ;
        PostData += "&field1=";
        PostData += humedad;
        PostData += "&field2=";
        PostData += salinidad;
        PostData += "&field3=";
        PostData += temperatura;
        PostData += "&field4=";
        PostData += iluminacion;
        PostData += "&field5=";
        PostData += auxLatLong;

        // POST data via HTTP
        //Serial.println( "Conectando con el servidor..." );

        client.println( "POST http://" + String(Rest_Host) + "/update HTTP/1.1" );
        client.println( "Host: " + String(Rest_Host) );
        client.println( "Connection: close" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );
        client.println( "Content-Length: " + String( PostData.length() ) );
        client.println();
        client.println( PostData );

        delay(500);
        Serial.print("------> Respuesta del servidor (ID):");

        while (client.available()) {
          String line = client.readStringUntil('\r');
          Serial.print(line);
          Serial.print(" ");
        }
        Serial.println();
      }

    }


    /*void HTTPPost(String fieldData[], int numFields) {

      if (client.connect( Server_Host , Server_HttpPort )) {

        String PostData = "api_key=" + MyWriteAPIKey ;
        for ( int field = 1; field < (numFields + 1); field++ ) {
          PostData += "&field" + String( field ) + "=" + fieldData[ field ];
        }

        // POST data via HTTP
        Serial.println( "Connecting to ThingSpeak for update..." );

        client.println( "POST http://" + String(Rest_Host) + "/update HTTP/1.1" );
        client.println( "Host: " + String(Rest_Host) );
        client.println( "Connection: close" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );
        client.println( "Content-Length: " + String( PostData.length() ) );
        client.println();
        client.println( PostData );

        delay(500);
        Serial.println("------> Respuesta del servidor:");
        while (client.available()) {
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }
        Serial.println();
        Serial.println();

      }
      }*/


};
