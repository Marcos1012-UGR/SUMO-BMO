// ========================
// SENSORES
// ========================

const int sensorIzquierdo = A2;
const int sensorDerecho   = A1;
const int sensorTrasero   = A3;

void setupSensor() {
  // Mensaje inicial en el monitor serie
  Serial.println("--- Lectura del Sensor CNY70 ---");
  Serial.println("Tiempo(ms) \t Valor Analogo");
  Serial.println("--------------------------------");
}

void loopSensor() {

  // Leemos el valor del pin analógico (nos dará un valor entre 0 y 1023)
  int lectura0 = analogRead(sensorIzquierdo);
  int lectura1 = analogRead(sensorDerecho);
  int lectura2 = analogRead(sensorTrasero);

  String RES = "IZQUIERDO: " + String(lectura0) + "\n DERECHO: " + String(lectura1) + "\n TRASERO: " + String(lectura2) + "\n ------------";

  // Imprimimos los datos en el Monitor Serie
  Serial.print("------------\n");
  Serial.println(RES);
}