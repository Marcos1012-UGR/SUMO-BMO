
// ============================================================================ //
// Planteamiento de codigo para resolver problema de limites de forma Reactiva  //
// ============================================================================ //

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


// ========================
// LOOP MÁQUINA DE ESTADOS REACTIVA (MECANUM)
// ========================
void loop() { 
  // 1. Leer los sensores de suelo en cada iteración
  int izq  = digitalRead(sensorIzquierdo);
  int der  = digitalRead(sensorDerecho);
  int tras = digitalRead(sensorTrasero);

  // 2. Prioridad absoluta: Evasión de bordes con tracción omnidireccional
  if (izq == HIGH) {
    // Línea al frente-izquierda: Escapar en diagonal hacia atrás-derecha
    evadirDiagonalAtrasDerecha();
    delay(300); // Tiempo para asegurar que salimos de la línea blanca
  } 
  else if (der == HIGH) {
    // Línea al frente-derecha: Escapar en diagonal hacia atrás-izquierda
    evadirDiagonalAtrasIzquierda();
    delay(300);
  } 
  else if (tras == HIGH) {
    // Línea atrás: Alguien nos empuja. Desplazamiento lateral puro (Strafe) 
    // a la izquierda para esquivarlo lateralmente y hacerlo pasar de largo.
    evadirLateralIzquierda();
    delay(350);
  } 
  else {
    // 3. Si todo el suelo es NEGRO, el robot está a salvo. 
    // Mantiene las direcciones hacia adelante y sincroniza los motores.
    digitalWrite(digitales[0], HIGH); // F1
    digitalWrite(digitales[2], HIGH); // F2
    digitalWrite(digitales[4], HIGH); // F3
    digitalWrite(digitales[6], HIGH); // F4
    
    digitalWrite(digitales[1], LOW);  // R1
    digitalWrite(digitales[3], LOW);  // R2
    digitalWrite(digitales[5], LOW);  // R3
    digitalWrite(digitales[7], LOW);  // R4

    // Ejecuta tu control por I2C para ir recto y estable
    actualizarControlSincronizacionCompleta();
  }
}

// ========================================================
// FUNCIONES DE MOVIMIENTO OMNIDIRECCIONAL (EVASIÓN)
// ========================================================

void evadirDiagonalAtrasDerecha() {
  // Para diagonal atrás-derecha con Mecanum:
  // Motor 1: Reversa | Motor 2: Stop
  // Motor 3: Stop    | Motor 4: Reversa
  digitalWrite(digitales[0], LOW);  digitalWrite(digitales[1], HIGH); // R1
  digitalWrite(digitales[2], LOW);  digitalWrite(digitales[3], LOW);  // Stop 2
  digitalWrite(digitales[4], LOW);  digitalWrite(digitales[5], LOW);  // Stop 3
  digitalWrite(digitales[6], LOW);  digitalWrite(digitales[7], HIGH); // R4

  // Forzar velocidad de escape rápida sin esperar la sincronización lenta
  vel1 = 200; vel2 = 0; vel3 = 0; vel4 = 200;
  aplicarVelocidadesInmediatas();
}

void evadirDiagonalAtrasIzquierda() {
  // Para diagonal atrás-izquierda con Mecanum:
  // Motor 1: Stop    | Motor 2: Reversa
  // Motor 3: Reversa | Motor 4: Stop
  digitalWrite(digitales[0], LOW);  digitalWrite(digitales[1], LOW);  // Stop 1
  digitalWrite(digitales[2], LOW);  digitalWrite(digitales[3], HIGH); // R2
  digitalWrite(digitales[4], LOW);  digitalWrite(digitales[5], HIGH); // R3
  digitalWrite(digitales[6], LOW);  digitalWrite(digitales[7], LOW);  // Stop 4

  vel1 = 0; vel2 = 200; vel3 = 200; vel4 = 0;
  aplicarVelocidadesInmediatas();
}

void evadirLateralIzquierda() {
  // Desplazamiento lateral puro izquierda (Strafe) para esquivar empujones:
  // Motor 1: Reversa  | Motor 2: Adelante
  // Motor 3: Adelante | Motor 4: Reversa
  digitalWrite(digitales[0], LOW);  digitalWrite(digitales[1], HIGH); // R1
  digitalWrite(digitales[2], HIGH); digitalWrite(digitales[3], LOW);  // F2
  digitalWrite(digitales[4], HIGH); digitalWrite(digitales[5], LOW);  // F3
  digitalWrite(digitales[6], LOW);  digitalWrite(digitales[7], HIGH); // R4

  vel1 = 220; vel2 = 220; vel3 = 220; vel4 = 220;
  aplicarVelocidadesInmediatas();
}

void aplicarVelocidadesInmediatas() {
  // Aplica los PWM locales de inmediato
  analogWrite(PWM1, vel1);
  analogWrite(PWM2, vel2);

  // Envía los PWM al esclavo por I2C de inmediato
  DatosVelocidades vEsclavo = {(byte)vel3, (byte)vel4};
  Wire.beginTransmission(DIR_ESCLAVO);
  Wire.write((byte*)&vEsclavo, sizeof(DatosVelocidades));
  Wire.endTransmission();
}