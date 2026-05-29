// ========================
// CONFIGURACIÓN DE PINES - ARDUINO ESCLAVO
// ========================
#include <Arduino.h>
#include <Wire.h>

// ESTRUCTURAS DE DATOS (Deben ser idénticas a las del Maestro)
struct DatosEncoders {
  long e3;
  long e4;
};

struct DatosVelocidades {
  byte v3;
  byte v4;
};

// CONFIGURACIÓN I2C
const byte DIR_ESCLAVO = 0x69; // Su propia dirección

// Variables de control y encoders (Modificadas en ISR / I2C)
volatile long encoder3 = 0;
volatile long encoder4 = 0;
volatile byte vel3 = 0;
volatile byte vel4 = 0;

// INTERRUPCIONES (Encoders locales del esclavo)
#define INT3 2
#define INT4 3

// PWM (Motores del esclavo)
#define PWM3 5
#define PWM4 6

// SALIDAS DIGITALES (Dirección de motores del esclavo)
// Ajusta los pines según tu puente H (L298N, Shield, etc.)
#define DIR_M3_A 4
#define DIR_M3_B 7
#define DIR_M4_A 8
#define DIR_M4_B 12

// ========================
// ISR (Interrupciones de Encoders)
// ========================
void isr3() {
  encoder3++;
}
void isr4() {
  encoder4++;
}

// ========================
// EVENTOS I2C
// ========================

// 1. EL MAESTRO NOS ENVÍA LAS VELOCIDADES CALCULADAS
void i2cRecibirDato(int cuantosBytes) {
  // Verificamos que nos llegue el tamaño correcto de la estructura
  if (cuantosBytes == sizeof(DatosVelocidades)) {
    DatosVelocidades vRecibidas;
    
    // Leemos los bytes directamente a la estructura
    Wire.readBytes((char*)&vRecibidas, sizeof(DatosVelocidades));
    
    // Actualizamos las variables globales de velocidad
    vel3 = vRecibidas.v3;
    vel4 = vRecibidas.v4;
  }
}

// 2. EL MAESTRO NOS PIDE LOS ENCODERS ACTUALES
void i2cPeticionDato() {
  DatosEncoders encodersEnviar;

  // COPIA SEGURA Y RESET INMEDIATO
  // Desactivamos interrupciones un instante para que no varíen mientras copiamos
  noInterrupts(); 
  encodersEnviar.e3 = encoder3;
  encodersEnviar.e4 = encoder4;
  encoder3 = 0; // Reset para el siguiente ciclo de muestreo
  encoder4 = 0;
  interrupts();

  // Enviamos la estructura con los datos al maestro
  Wire.write((byte*)&encodersEnviar, sizeof(DatosEncoders));
}

// ========================
// SETUP
// ========================
void setup() {
  Serial.begin(9600);

  // Inicializar I2C como ESCLAVO con su dirección
  Wire.begin(DIR_ESCLAVO);
  
  // Registrar las funciones que van a reaccionar al bus I2C
  Wire.onReceive(i2cRecibirDato); // Cuando el maestro escribe
  Wire.onRequest(i2cPeticionDato); // Cuando el maestro pide

  // Configurar Encoders
  pinMode(INT3, INPUT_PULLUP);
  pinMode(INT4, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT3), isr3, FALLING);
  attachInterrupt(digitalPinToInterrupt(INT4), isr4, FALLING);

  // Configurar Pines de Motor
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);
  pinMode(DIR_M3_A, OUTPUT);
  pinMode(DIR_M3_B, OUTPUT);
  pinMode(DIR_M4_A, OUTPUT);
  pinMode(DIR_M4_B, OUTPUT);

  // Motores parados al iniciar
  analogWrite(PWM3, 0);
  analogWrite(PWM4, 0);

  // Direcciones por defecto (Hacia adelante)
  digitalWrite(DIR_M3_A, HIGH);
  digitalWrite(DIR_M3_B, LOW);
  digitalWrite(DIR_M4_A, HIGH);
  digitalWrite(DIR_M4_B, LOW);

  Serial.println("Esclavo listo y escuchando I2C...");
}

// ========================
// LOOP
// ========================
void loop() {
  // El loop del esclavo queda sumamente limpio.
  // Toda la lógica de comunicación ocurre por "interrupciones de software" de I2C.
  
  // Aquí aplicamos constantemente la última velocidad recibida por el maestro.
  analogWrite(PWM3, vel3);
  analogWrite(PWM4, vel4);
}