//LIBRERÍAS
#include <Wire.h>
#include <Adafruit_ADS1015.h>

//#include <TinyGPS++.h>  //Librería del GPS
//#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Arduino.h>


//SENSOR DE HUMEDAD
const int AirValue = 19473;  // VALOR NORMAL DEL SENSOR SIN METER EN AGUA
const int WaterValue = 10050;  // VALOR DEL SENSOR METIDO EN AGUA
const int PINHUMEDAD = 0;  // PIN ANALOGICO

//SENSOR DE SALINIDAD
const int PINDIGITALSALINIDAD = 5; // DIGITAL
const int PINANALOGICOSALINIDAD = 1; // ANALOGICO
const int maxSalinidad = 10000;  // MAXIMO DE SALINIDAD - 20166
const int minSalinidad = 5000;  // NADA DE SAL, ES DECIR, SOLO AGUA - 18300

//SENSOR DE TEMPERATURA
int PINTEMPERATURA = 2; //ANALOGICO

//TIEMPO PARA DORMIR AL MICROCONTROLADOR
int TIEMPO_DORMIR = 10;

//LIBRERIAS PROPIAS
#include "FuncionesExtras.h"
#include "SensorHumedad.h"
//#include "SensorGPS.h"
#include "SensorSalinidad.h"
#include "SensorTemperatura.h"
#include "SensorIluminacion.h"
#include "Speaker.h"
#include "SensorWifi.h"
#include "SensorAcelerometro.h"

//OBJETOS GLOBALES
FuncionesExtras func;
//TinyGPSPlus gps;
Adafruit_ADS1115 ads1(0x48);

void setup() {

  Serial.begin(9600);
  configurarSensor();//LLAMAMOS A LA FUNCION DE CONFIGURAR LOS SENSORES
  Serial.println("Setup completado."); //AVISAMOS POR PANTALLA QUE SE HA COMPLETADO LA CONFIGURACIÓN DEL SETUP

}


void loop() {

  //mostrarMenu();
ejecutarPrograma();
  //ESP.deepSleep(TIEMPO_DORMIR * 1000000); //PONEMOS A DORMIR EL MICROCONTROLADOR
  delay(10000); //ESPERAMOS 5 SEGUNDOS PARA QUE VUELVA A HACER EL LOOP

}

void ejecutarPrograma() {


  //GPS
  /*SensorGPS sensorGps(gps);
  double latitudAUX;
  double longAUX ;

  Serial.print("\n GPS -> Buscando Satélites\n ");
  /*int puntos = 0;
  do {

    Serial.print(".");
    puntos++;

    if(puntos%3 == 0){
      Serial.print(" ");
    }

    if(puntos%21 == 0){
      Serial.println("");
    }

    } while (!sensorGps.buscarSatelites()); //MIENTRAS ME DEVUELVA QUE NO HA ENCONTRADO SATELITES SIGUE BUSCANDO
 
  
  longAUX = sensorGps.getLongitud();
  latitudAUX = sensorGps.getLatitud();

  Serial.print("\n -> Latitud: ");
  Serial.println(latitudAUX);
  Serial.print(" -> Longitud: ");
  Serial.println(longAUX);
 */

 int latitudAUX = 1;
 int longAUX = 1;
  //HUMEDAD
  SensorHumedad humedad(ads1);
  Serial.print("\n Humedad -> ");
  double hume = humedad.getHumedad();
  Serial.print(hume);
  Serial.println("%");

  //SALINIDAD
  SensorSalinidad salinidad(ads1);
  Serial.print("\n Salinidad -> ");
  double sali = salinidad.getSalinidad();
  Serial.print(sali);
  Serial.println("%");

  //TEMPERATURA
  SensorTemperatura temperatura(ads1);
  Serial.print("\n Temperatura -> ");
  double temp = temperatura.getTemperatura();
  Serial.print(temp);
  Serial.println("º");

  //ILUMINAICON
  SensorIluminacion iluminacion(ads1);
  Serial.print("\n Iluminación -> ");
  double ilu = iluminacion.getIluminacion();
  Serial.print(ilu);
  Serial.println("%");

  //ENVIAR DATOS AL SERVIDOR
  SensorWifi wifi;
  wifi.enviarDatosServidor(hume, sali, temp, ilu, latitudAUX, longAUX);

}

void mostrarMenu() {


  int respuesta = func.devolverOpcionMenu(10);
  switch (respuesta) {
    case 1: {
        Serial.println("\n  Ejecutando el programa...");
        ejecutarPrograma();

        break;
      }
    case 2: {
        Serial.println("Calibrando el sensor de humedad...");

        int pin = func.preguntarUsuario("¿En que pin quieres utilizar el sensor de humedad?", "0");
        double airValue = func.preguntarUsuario("¿Qué cantidad de aire quieres seleccionar?", "19473");
        double waterValue = func.preguntarUsuario("¿Qué cantidad de agua quieres seleccionar?", "10050");

        SensorHumedad humedad(pin, waterValue, airValue, ads1);

        Serial.println("Sensor de humedad calibrado.");

        break;
      }
    case 3: {
        Serial.println("Calibrando el sensor de temperatura...");

        int pin = func.preguntarUsuario("¿En que pin quieres utilizar el sensor de temperatura?", "2");
        double minV = func.preguntarUsuario("¿Cuántos voltios da como mínimo el sensor de temperatura?", "348");
        double maxV = func.preguntarUsuario("¿Cuántos voltios da como máximo el sensor de temperatura?", "786");
        double margenError = func.preguntarUsuario("¿Qué margen de error tiene tu sensor de temperatura?", "31");

        SensorTemperatura temperatura(pin, minV, maxV, margenError, ads1);

        Serial.println("Sensor de temperatura calibrado.");

        break;
      }
    case 4: {
        Serial.println("Calibrando el sensor de salinidad...");

        int powerPin = func.preguntarUsuario("¿En que pin DIGITAL quieres utilizar el sensor de salinidad?", "5");
        int inputPin = func.preguntarUsuario("¿En que pin ANALÓGICO quieres utilizar el sensor de salinidad?", "1");
        int nave = func.preguntarUsuario("¿Cuántas vueltas quieres que de el bucle para medir la salinidad?", "10");
        int maxSalinidad = func.preguntarUsuario("¿Cúanto es el máximo de salinidad?", "10000");
        int minSalinidad = func.preguntarUsuario("¿Cuánto es el mínimo de salinidad?", "5000");

        SensorSalinidad salinidad(powerPin, inputPin, nave, maxSalinidad, minSalinidad, ads1);

        Serial.println("Sensor de salinidad calibrado.");

        break;
      }
    case 5: {
        Serial.println("Calibrando el sensor de iluminación...");

        int pin = func.preguntarUsuario("¿En que pin quieres utilizar el sensor de iluminación?", "3");
        double maxV = func.preguntarUsuario("¿Cuánto es el máximo de Voltios que da tu sensor?", "371");

        double minIlu = func.preguntarUsuario("¿Cuánto es el mínimo de iluminación que da tu sensor?", "50");
        double maxIlu = func.preguntarUsuario("¿Cuánto es el máximo de iluminación que da tu sensor?", "1070");

        SensorIluminacion iluminacion(pin, maxV, minIlu, maxIlu, ads1);

        Serial.println("Sensor de iluminación calibrado.");

        break;
      }
    case 6: {

        if (func.getDebug()) {
          func.setDebug(false);
          Serial.println("Has desactivado el modo debug.");
        } else {
          func.setDebug(true);
          Serial.println("Has activado el modo debug.");
        }

        break;
      }
    case 7: {


        /*String nameWifi = func.preguntarUsuarioTexto("¿Cúal es el nombre de la conexión WiFi?", "iPhone de Javi");
          String password = func.preguntarUsuarioTexto("¿Cúal es la contraseña?", "jarabito");

          SensorWifi wifi(nameWifi, password);*/

        Serial.println("Wifi configurado.");

        break;
      }
    default: {
        Serial.println("\tLo siento, esta opción no existe, vuelve a intentarlo.");

        break;
      }
  }


}


//CONFIGURAR SENSOR
void configurarSensor() {

  ads1.begin();
  ads1.setGain(GAIN_ONE);

  //SALINIDAD
  pinMode(PINDIGITALSALINIDAD, OUTPUT);

  //GPS
  pinMode(15, OUTPUT);

  //EPROM
  EEPROM.begin(512);

  //ACELEROMETRO
  configurarWakeOnMotion(); //CONFIGURAMOS EL WAKE ON MOTION

  Serial.println("\nConfiguración completada.");

  Speaker speaker(0);
  speaker.playSetup();

}
