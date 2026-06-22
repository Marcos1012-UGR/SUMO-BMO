// ========================
// SENSORES
// ========================

const int sensorIzquierdo = A2;
const int sensorDerecho   = A1;
const int sensorTrasero   = A3;

void setupSensor() {
  
  Serial.println("--- Lectura del Sensor CNY70 ---");
  Serial.println("Tiempo(ms) \t Valor Analogo");
  Serial.println("--------------------------------");
}

void testSensor() {
  Serial.println("Sensor izquierdo: " + String(getSensor(IZQUIERDO)));
  Serial.println("Sensor derecho: " + String(getSensor(DERECHO)));
  Serial.println("Sensor trasero: " + String(getSensor(TRASERO)));
  Serial.println("");

  delay(1000);
}

int getSensor(Sensor s) {
  switch (s) {
    case IZQUIERDO: return analogRead(sensorIzquierdo);
    case DERECHO:   return analogRead(sensorDerecho);
    case TRASERO:   return analogRead(sensorTrasero);
  }
  return 0;
}