#include <Wire.h>


// ========================
// I2C
// ========================
#define I2C_SDA A4 
#define I2C_SCL A5 
#define DIR_ESCLAVO 67 

void setupI2C() {
  Wire.begin(); // Maestro
  Wire.onReceive(recvI2C);
}

void sendI2C(DatosVelocidades dv) {
  Wire.beginTransmission(DIR_ESCLAVO);
  Wire.write((byte*)&dv, sizeof(dv));
  Wire.endTransmission();
}

void recvI2C(int numBytes) {
  Wire.readBytes((byte*)&de, sizeof(de));
}

void testI2C() {
  Serial.println("Enviando mensaje a B...");

  Wire.beginTransmission(DIR_ESCLAVO);
  Wire.write("Hola B");
  Wire.endTransmission();

  delay(500);

  Serial.println("Solicitando respuesta a B...");

  Wire.requestFrom(DIR_ESCLAVO, 6);
  Wire.readBytes((byte*)&de, sizeof(de));

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