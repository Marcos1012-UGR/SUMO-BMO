
// ================================================================================ //
// Planteamiento de codigo para resolver problema de limites de forma Deliberativa  //
// ================================================================================ //

// ========================
// CONFIGURACIÓN DE PINES - ARDUINO UNO R3
// ========================
#include <Arduino.h>
#include <Wire.h> // Librería esencial para I2C

// ESTRUCTURAS DE DATOS PARA I2C
struct DatosEncoders {
  long e3;
  long e4;
};

struct DatosVelocidades {
  byte v3;
  byte v4;
};

// CONFIGURACIÓN I2C
#define I2C_SDA A4 
#define I2C_SCL A5 
const byte DIR_ESCLAVO = 0x67; // Usaremos esta constante en todo el código

// Variables globales de control
volatile int encoder1 = 0, encoder2 = 0;
int vel1 = 150, vel2 = 150, vel3 = 150, vel4 = 150; 

// INTERRUPCIONES 
#define INT1 2
#define INT2 3

// PWM 
#define PWM1 5
#define PWM2 6
#define PWM3 9
#define PWM4 10

// SALIDAS DIGITALES 
int digitales[] = {4, 7, 8, 12, 13, A0, A1, A2};

// --- NUEVOS PINES PARA SENSORES DE SUELO MECANUM ---
const int sensorIzquierdo = 11; // Libre digital
const int sensorDerecho   = A3; // Libre analógico (usado como digital)
const int sensorTrasero   = 4;  // Sacamos el 4 del array 'digitales' para usarlo aquí


// --- VARIABLES DELIBERATIVAS (EL MAPA MENTAL) ---
unsigned long tiempoUltimaLinea = 0;
float peligroEstimado = 0.0;        // 0.0 = Centro (Seguro), 1.0 = Borde (Peligro)
int ultimaDireccionBorde = 0;       // 1 = Izquierda, 2 = Derecha, 3 = Atrás


// ========================
// LOOP DELIBERATIVO (MECANUM)
// ========================
void loop() { 
  // 1. PERCEPCIÓN: Leer sensores físicos
  int izq  = digitalRead(sensorIzquierdo);
  int der  = digitalRead(sensorDerecho);
  int tras = digitalRead(sensorTrasero);
  
  unsigned long tiempoActual = millis();

  // 2. ACTUALIZACIÓN DEL MAPA (Deliberación pasiva)
  if (izq == HIGH || der == HIGH || tras == HIGH) {
    // Si pisamos línea físicamente, reseteamos el mapa al 100% de peligro (Certeza absoluta)
    peligroEstimado = 1.0;
    tiempoUltimaLinea = tiempoActual;
    
    if (izq == HIGH)  ultimaDireccionBorde = 1;
    if (der == HIGH)  ultimaDireccionBorde = 2;
    if (tras == HIGH) ultimaDireccionBorde = 3;
  } 
  else {
    // Si no vemos línea, estimamos el riesgo por el tiempo que llevamos yendo de frente
    unsigned long tiempoCaminado = tiempoActual - tiempoUltimaLinea;
    
    // Si llevamos más de 1.2 segundos corriendo recto, asumimos deliberadamente 
    // que el borde del dojo está extremadamente cerca.
    if (tiempoCaminado > 1200) {
      peligroEstimado = 0.85; // Alerta alta preventiva (el suelo sigue negro)
    } else {
      // El peligro escala linealmente del 0% al 70% conforme pasa el tiempo
      peligroEstimado = (tiempoCaminado / 1200.0) * 0.7; 
    }
  }

  // 3. TOMA DE DECISIONES DELIBERATIVA (Planes de acción)
  pensarYActuar();
}

// --- ARQUITECTURA DE DECISIONES ---
void pensarYActuar() {
  
  // PLAN A: Emergencia reactiva (Prioridad Máxima - Pisamos línea)
  if (peligroEstimado >= 1.0) {
    if (ultimaDireccionBorde == 1) evadirDiagonalAtrasDerecha();
    if (ultimaDireccionBorde == 2) evadirDiagonalAtrasIzquierda();
    if (ultimaDireccionBorde == 3) evadirLateralIzquierda();
    delay(300); // Darle tiempo a los motores de ejecutar el escape
    peligroEstimado = 0.4; // Tras evadir con éxito, bajamos el nivel de peligro estimado
    return;
  }
  
  // PLAN B: Deliberación preventiva (¡Evitar morir por inercia!)
  // Los sensores ven negro, pero el mapa mental calcula que estamos al límite del dojo.
  // En lugar de seguir a ciegas, ejecutamos una maniobra segura bajando la velocidad.
  if (peligroEstimado > 0.80) {
    ejecutarManiobraSeguraMecanum();
    return;
  }
  
  // PLAN C: Modo Cazador (Zona central y segura del dojo)
  // Avanzar con máxima potencia de forma síncrona
  ejecutarPlanBuscarEnemigoSincrono();
}

// ========================================================
// PLANES DE ACCIÓN Y MOVIMIENTOS MECANUM
// ========================================================

void ejecutarManiobraSeguraMecanum() {
  // Configuración de motores hacia adelante
  digitalWrite(digitales[0], HIGH); digitalWrite(digitales[1], LOW); // F1
  digitalWrite(digitales[2], HIGH); digitalWrite(digitales[3], LOW); // F2
  digitalWrite(digitales[4], HIGH); digitalWrite(digitales[5], LOW); // F3
  digitalWrite(digitales[6], HIGH); digitalWrite(digitales[7], LOW); // F4

  // Bajamos bruscamente la velocidad de forma preventiva para que, si tocamos la línea,
  // el robot frene al instante y la inercia no lo lance fuera de la pista.
  vel1 = 110; 
  vel2 = 110;
  vel3 = 110;
  vel4 = 110;
  
  // Sincronizamos a baja velocidad para mantener estabilidad
  actualizarControlSincronizacionCompleta();
}

void ejecutarPlanBuscarEnemigoSincrono() {
  // Vamos seguros en el centro del dojo. Configuramos motores hacia adelante.
  digitalWrite(digitales[0], HIGH); digitalWrite(digitales[1], LOW); // F1
  digitalWrite(digitales[2], HIGH); digitalWrite(digitales[3], LOW); // F2
  digitalWrite(digitales[4], HIGH); digitalWrite(digitales[5], LOW); // F3
  digitalWrite(digitales[6], HIGH); digitalWrite(digitales[7], LOW); // F4

  // Ponemos velocidades base altas/agresivas
  vel1 = 200; 
  // Tu control por I2C (actualizarControlSincronizacionCompleta) regulará vel2, vel3 y vel4
  // de forma cooperativa usando los errores de los encoders.
  
  actualizarControlSincronizacionCompleta();
}

// =======================
// REACCIONES DE EVASIÓN
// =======================

void evadirDiagonalAtrasDerecha() {
  digitalWrite(digitales[0], LOW);  digitalWrite(digitales[1], HIGH); // R1
  digitalWrite(digitales[2], LOW);  digitalWrite(digitales[3], LOW);  // Stop 2
  digitalWrite(digitales[4], LOW);  digitalWrite(digitales[5], LOW);  // Stop 3
  digitalWrite(digitales[6], LOW);  digitalWrite(digitales[7], HIGH); // R4
  vel1 = 220; vel2 = 0; vel3 = 0; vel4 = 220;
  aplicarVelocidadesInmediatas();
}

void evadirDiagonalAtrasIzquierda() {
  digitalWrite(digitales[0], LOW);  digitalWrite(digitales[1], LOW);  // Stop 1
  digitalWrite(digitales[2], LOW);  digitalWrite(digitales[3], HIGH); // R2
  digitalWrite(digitales[4], LOW);  digitalWrite(digitales[5], HIGH); // R3
  digitalWrite(digitales[6], LOW);  digitalWrite(digitales[7], LOW);  // Stop 4
  vel1 = 0; vel2 = 220; vel3 = 220; vel4 = 0;
  aplicarVelocidadesInmediatas();
}

void evadirLateralIzquierda() {
  digitalWrite(digitales[0], LOW);  digitalWrite(digitales[1], HIGH); // R1
  digitalWrite(digitales[2], HIGH); digitalWrite(digitales[3], LOW);  // F2
  digitalWrite(digitales[4], HIGH); digitalWrite(digitales[5], LOW);  // F3
  digitalWrite(digitales[6], LOW);  digitalWrite(digitales[7], HIGH); // R4
  vel1 = 230; vel2 = 230; vel3 = 230; vel4 = 230;
  aplicarVelocidadesInmediatas();
}

void aplicarVelocidadesInmediatas() {
  analogWrite(PWM1, vel1);
  analogWrite(PWM2, vel2);
  DatosVelocidades vEsclavo = {(byte)vel3, (byte)vel4};
  Wire.beginTransmission(DIR_ESCLAVO);
  Wire.write((byte*)&vEsclavo, sizeof(DatosVelocidades));
  Wire.endTransmission();
}