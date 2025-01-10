#ifndef FUNCIONES_EXTRAS
#define FUNCIONES_EXTRAS

#include <Wire.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#include <string>

class FuncionesExtras
{

  private:
    bool debug;
    int seg;

    const int direccionMemoriaDEBUG = 1;
    const int direccionMemoriaDEBUG2 = 2;

  public:

    //CONSTRUCTOR
    FuncionesExtras() {
      //DEVOLVEMOS LOS VALORES DE LA EEPROM A LAS VARIABLES DE ESTA CLASE
      this->debug = getDatos(direccionMemoriaDEBUG, direccionMemoriaDEBUG2);
    }


    //SETTES Y GETTERS
    bool getDebug() {
      return getDatos(direccionMemoriaDEBUG, direccionMemoriaDEBUG2);
    }

    void setDebug(bool debug) {
      setDatos(debug, direccionMemoriaDEBUG, direccionMemoriaDEBUG2);
      this->debug = debug;
    }

    double preguntarUsuario(String pregunta, String pordefecto = "Ningún valor recomendado") {

      double aux = 0;
      int respuesta = 0; //POR DEFECTO NO TIENE VALORES

      Serial.print(pregunta);
      Serial.print(" -> Valor recomendado: ");
      Serial.println(pordefecto);

      Serial.println("\n---> Escriba una respuesta... ");

      //ESPERAMOS HASTA QUE CONTESTE EL USUARIO
      while (Serial.available() == 0) {
        delay(1000);
      }
      //CUANDO CONTESTE QUE SUENE
      tone(0, 500, 30);

      aux = Serial.parseFloat(); //LO PASAMOS A INT

      Serial.print("\n ---> Has escrito: ");
      Serial.println(aux);
      Serial.println("\n");

      Serial.read(); //LO ELIMINADOS DEL BUFFER

      return aux;

    }

    String preguntarUsuarioTexto(String pregunta, String pordefecto = "Ningún valor recomendado") {

      String aux = "";
      int respuesta = 0; //POR DEFECTO NO TIENE VALORES

      Serial.print(pregunta);
      Serial.print(" -> Valor recomendado: ");
      Serial.println(pordefecto);

      Serial.println("\n---> Escriba una respuesta... ");

      //ESPERAMOS HASTA QUE CONTESTE EL USUARIO
      while (Serial.available() == 0) {
        delay(1000);
      }
      //CUANDO CONTESTE QUE SUENE
      tone(0, 500, 30);

      aux = Serial.readStringUntil('\n'); //LO PASAMOS A STRING

      Serial.print("\n ---> Has escrito: ");
      Serial.println(aux);
      Serial.println("\n");

      Serial.read(); //LO ELIMINADOS DEL BUFFER

      return aux;

    }


    void setDatos(int valor, int direccion, int direccion2) {

      int h = abs(valor / 256);
      int l = valor - h;

      EEPROM.write(direccion, h); //GUARDAMOS EL DATO 1
      EEPROM.write(direccion2, l); //GUARDAMOS EL DATO 2
      EEPROM.commit();   // GUARDAMOS CAMBIOS

    }


    double getDatos(int direccion, int direccion2) {

      int h = EEPROM.read(direccion);
      int l = EEPROM.read(direccion2);

      int valor = h * 256 + l;

      return valor;//RECUPERAMOS EL DATO
    }

    int devolverOpcionMenu(int segundos) {

      int respuesta = 0; //POR DEFECTO NO TIENE VALORES

      Serial.println("\n --------------- MENÚ PRINCIPAL ---------------");
      Serial.println("\n Selecciones una de las siguientes opciones: ");
      Serial.println("\t1. Ejecutar programa Arduino.");
      Serial.println("\t2. Calibrar y configurar sensor humedad.");
      Serial.println("\t3. Calibrar y configurar sensor temperatura.");
      Serial.println("\t4. Calibrar y configurar sensor salinidad.");
      Serial.println("\t5. Calibrar y configurar sensor iluminación.");

      if (getDebug()) {
        Serial.println("\t6. Desactivar modo debug.");
      } else {
        Serial.println("\t6. Activar modo debug.");
      }

      Serial.println("\t7. Configurar conexión WiFi.");

      Serial.print("\n Segundos restantes para elegir la opción por defecto: ");
      //ESPERAMOS HASTA QUE CONTESTE EL USUARIO
      /*for (int i = 0; i < segundos; i++) {

        if (Serial.available() == 0) {

          //POR CADA SEGUNDO QUE SUENE
          tone(4, 350, 30);
          delay(250);
          Serial.print(segundos - i);
          Serial.print(" ");
          delay(1000);

          if (i == (segundos - 1)) {
            //CUANDO NO CONTESTE QUE SUENE
            tone(4, 200, 30);
            delay(100);
            tone(4, 200, 30);
            return 1;//DEVOLVEMOS LA OPCION 1

          }

        }

      }*/

      //CUANDO CONTESTE QUE SUENE
      tone(0, 500, 30);

      respuesta = Serial.parseInt(); //LO PASAMOS A INT

      Serial.print("\n---> Has seleccionado la opción: ");
      Serial.println(respuesta);
      Serial.println("\n");

      Serial.read(); //LO ELIMINADOS DEL BUFFER

      return respuesta;

    }

};

#endif
