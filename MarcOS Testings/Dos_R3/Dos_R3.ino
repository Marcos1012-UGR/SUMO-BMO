//#define enableTest

struct DatosVelocidades {
  byte v3;
  byte v4;
  bool dir3;
  bool dir4;
};

DatosVelocidades dv;

void setup() {
  setupI2C();
  setupMotor();
}

void loop() {
  Serial.println("Velocidades------------");
  Serial.println("Vel3: " + String(dv.v3));
  Serial.println("Vel4: " + String(dv.v4));
  Serial.println("FW3: " + String(dv.dir3));
  Serial.println("FW4: " + String(dv.dir4));
  Serial.println("");

  delay(500);
}
