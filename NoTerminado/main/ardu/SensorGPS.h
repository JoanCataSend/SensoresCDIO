#include "FuncionesExtras.h"
//#include <TinyGPS++.h>  //Librería del GPS
#include <SoftwareSerial.h>


//SENSOR GPS
#define RX_PIN  12 // GPS RXI
#define TX_PIN  13 // GPS TX0
#define INIT_PIN 15 // Pin para  Inicializar el GPS
#define GPS_BAUD  4800
SoftwareSerial ss(RX_PIN, TX_PIN);



class SensorGPS
{

    //ATRIBUTOS
  private:
    //TinyGPSPlus gps;
    FuncionesExtras func;
    double latitud;
    double longitud;


  public:

    //CONSTRUCTOR
    //SensorGPS(TinyGPSPlus gps) {

      //ASIGNAMOS A LAS VARIABLES DE LA CLASE LOS VALORES DE LOS PARÁMETROS
      this->gps = gps;
      ss.begin(GPS_BAUD);
      switch_on_off();

    }

    //GETTERS
    double getLatitud() {
      if (this->gps.location.isValid()) {
        return this->latitud;

      }
      return -1;
    }

    double getLongitud() {
      if (this->gps.location.isValid()) {
        return this->longitud;

      }
      return -1;
    }

    void smartDelay(unsigned long ms)
    {
      unsigned long start = millis();
      do
      {
        while (ss.available())
        {
          this->gps.encode(ss.read());
        }
      } while (millis() - start < ms);
    }

    bool buscarSatelites() {
      delay(250);//ESPERAR PARA PODER BUSCAR GPS, SI NO DA ERROR

      bool encontrados = false;

      if (this->gps.location.isValid()) { // Si el GPS está recibiendo los mensajes NMEA

        encontrados = true;
        this->latitud = this->gps.location.lat();
        this->longitud = this->gps.location.lng();

      }

      smartDelay(1000);

      return encontrados;
    }

    // Función para encender/apagar mediante un pulso
    void switch_on_off()
    {
      digitalWrite(INIT_PIN, LOW);
      delay(200);
      digitalWrite(INIT_PIN, HIGH);
      delay(200);
      digitalWrite(INIT_PIN, LOW);
    }

};
