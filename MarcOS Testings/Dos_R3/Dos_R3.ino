#include <Wire.h>

#define SLAVE_ADDR 8

String mensajeRecibido = "";

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR); // Esclavo

  Wire.onReceive(recibirEvento);
  Wire.onRequest(enviarEvento);

  Serial.println("Esclavo listo");
}

void loop() {
  // No hace falta nada aquí
}

void recibirEvento(int bytes) {
  mensajeRecibido = "";

  while (Wire.available()) {
    char c = Wire.read();
    mensajeRecibido += c;
  }

  Serial.print("Mensaje recibido: ");
  Serial.println(mensajeRecibido);
}

void enviarEvento() {
  String respuesta = "Hola A";

  Serial.println("Enviando respuesta...");

  Wire.write(respuesta.c_str());
}