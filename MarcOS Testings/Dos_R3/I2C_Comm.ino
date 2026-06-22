#include <Wire.h>

#define SLAVE_ADDR 67

// Variables volátiles porque cambian dentro de una interrupción
volatile bool nuevoMensaje = false;
String mensajeRecibido = "";

void loopI2C() {
  // El loop se encarga de imprimir de forma segura fuera de la interrupción
  if (nuevoMensaje) {
    Serial.print("Mensaje recibido: ");
    Serial.println(mensajeRecibido);
    nuevoMensaje = false; // Resetear bandera
  }
}

void setupI2C() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR); 
  Wire.onReceive(recibirEvento);
  Wire.onRequest(enviarEvento);
  Serial.println("Esclavo listo");
}

void recibirEvento(int bytes) {
  mensajeRecibido = "";
  while (Wire.available()) {
    char c = Wire.read();
    mensajeRecibido += c;
  }
  nuevoMensaje = true; // Avisamos al loop que hay trabajo que hacer
}

void enviarEvento() {
  // Aquí devolvemos los datos rápido sin usar Serial.print
  Wire.write("Hola A"); 
}