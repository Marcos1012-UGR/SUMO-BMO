#include <Wire.h>

#define SLAVE_ADDR 8

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Maestro
  Serial.println("Maestro listo");
}

void loop() {
  Serial.println("Enviando mensaje a B...");

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write("Hola B");
  Wire.endTransmission();

  delay(500);

  Serial.println("Solicitando respuesta a B...");

  Wire.requestFrom(SLAVE_ADDR, 6);

  String respuesta = "";
  while (Wire.available()) {
    char c = Wire.read();
    respuesta += c;
  }

  Serial.print("Respuesta recibida: ");
  Serial.println(respuesta);

  Serial.println("----------------------");

  delay(2000);
}