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

// ========================
// Funciones - ARDUINO UNO R3
// ========================
void test1(){
  analogWrite(PWM1, 255);
  analogWrite(PWM2, 255);
  analogWrite(PWM3, 255);
  analogWrite(PWM4, 255);

  delay(2000);
  //1 segundo F1
  digitalWrite(digitales[0], HIGH);
  delay(1000);

  //1 segundo R1
  digitalWrite(digitales[0], LOW);
  digitalWrite(digitales[1], HIGH);
  delay(1000);

  //1 segundo F2
  digitalWrite(digitales[1], LOW);
  digitalWrite(digitales[2], HIGH);
  delay(1000);

  //1 segundo R2
  digitalWrite(digitales[2], LOW);
  digitalWrite(digitales[3], HIGH);
  delay(1000);

  //1 segundo F3
  digitalWrite(digitales[3], LOW);
  digitalWrite(digitales[4], HIGH);
  delay(1000);

  //1 segundo R3
  digitalWrite(digitales[4], LOW);
  digitalWrite(digitales[5], HIGH);
  delay(1000);

  //1 segundo F4
  digitalWrite(digitales[5], LOW);
  digitalWrite(digitales[6], HIGH);
  delay(1000);

  //1 segundo R4
  digitalWrite(digitales[6], LOW);
  digitalWrite(digitales[7], HIGH);
  delay(1000);

  //1 segundo FALL
  digitalWrite(digitales[7], LOW);
  delay(2000);

  digitalWrite(digitales[0], HIGH);
  digitalWrite(digitales[2], HIGH);
  digitalWrite(digitales[4], HIGH);
  digitalWrite(digitales[6], HIGH);
  delay(1000);

  //1 segundo RALL
  digitalWrite(digitales[0], LOW);
  digitalWrite(digitales[2], LOW);
  digitalWrite(digitales[4], LOW);
  digitalWrite(digitales[6], LOW);
  digitalWrite(digitales[1], HIGH);
  digitalWrite(digitales[3], HIGH);
  digitalWrite(digitales[5], HIGH);
  digitalWrite(digitales[7], HIGH);
  delay(1000);
  
  digitalWrite(digitales[1], LOW);
  digitalWrite(digitales[3], LOW);
  digitalWrite(digitales[5], LOW);
  digitalWrite(digitales[7], LOW);
  delay(5000);
}

// --- FUNCIÓN DE CONTROL OPTIMIZADA Y SIN ERRORES ---
void actualizarControlSincronizacionCompleta() {
  
  // 1. TIEMPO DE MUESTREO (Esperamos a que los motores acumulen pasos)
  delay(100); 

  // 2. PEDIR ENCODERS AL ESCLAVO (Cambiado a DIR_ESCLAVO)
  DatosEncoders encodersEsclavo = {0, 0};
  
  Wire.requestFrom(DIR_ESCLAVO, sizeof(DatosEncoders));
  if (Wire.available() == sizeof(DatosEncoders)) {
    Wire.readBytes((char*)&encodersEsclavo, sizeof(DatosEncoders));
  }

  // 3. COPIA SEGURA ENCODERS LOCALES E INMEDIATO RESET
  noInterrupts();
  long e1 = encoder1;
  long e2 = encoder2;
  encoder1 = 0;
  encoder2 = 0;
  interrupts();

  long e3 = encodersEsclavo.e3;
  long e4 = encodersEsclavo.e4;

  // 4. ALGORITMO DE CONTROL PARA LOS 4 MOTORES
  int error2 = e1 - e2;
  int error3 = e1 - e3;
  int error4 = e1 - e4;

  // vel1 se mantiene como velocidad base fija para la sincronización cooperativa
  vel2 += error2 * 0.5;
  vel3 += error3 * 0.5;
  vel4 += error4 * 0.5;

  // Limitar todos los PWM
  vel1 = constrain(vel1, 0, 255);
  vel2 = constrain(vel2, 0, 255);
  vel3 = constrain(vel3, 0, 255);
  vel4 = constrain(vel4, 0, 255);

  // 5. APLICAR PWM LOCALES (1 y 2)
  analogWrite(PWM1, vel1);
  analogWrite(PWM2, vel2);

  // 6. ENVIAR VELOCIDADES AL ESCLAVO (3 y 4)
  DatosVelocidades vEsclavo = {(byte)vel3, (byte)vel4};
  
  Wire.beginTransmission(DIR_ESCLAVO);
  Wire.write((byte*)&vEsclavo, sizeof(DatosVelocidades));
  Wire.endTransmission();
}

// ========================
// Funciones Framework I2C
// ========================
void i2cRecibirDato(int cuantosBytes) {
  if (Wire.available() > 0) {
    byte instruccion = Wire.read();
  }
}

void i2cPeticionDato() {
  byte respuesta = 0xAA; 
  Wire.write(respuesta); 
}

// ========================
// ISR (Interrupciones)
// ========================
void isr1() {
  encoder1++;
}
void isr2() {
  encoder2++;
}

// ========================
// SETUP
// ========================
void setup() {
  Serial.begin(9600);

  // I2C
  Wire.begin(); // Se une al bus como Maestro

  // ENCODERS
  pinMode(INT1, INPUT_PULLUP);
  pinMode(INT2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(INT1), isr1, FALLING);
  attachInterrupt(digitalPinToInterrupt(INT2), isr2, FALLING);

  // PWM
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  // Motores apagados al inicio
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  analogWrite(PWM3, 0);
  analogWrite(PWM4, 0);

  // DIRECCIONES
  for (int i = 0; i < 8; i++) {
    pinMode(digitales[i], OUTPUT);
    digitalWrite(digitales[i], LOW);
  }

  // Todas adelante por defecto
  digitalWrite(digitales[0], HIGH);
  digitalWrite(digitales[2], HIGH);
  digitalWrite(digitales[4], HIGH);
  digitalWrite(digitales[6], HIGH);

  Serial.println("Setup listo - UNO R3 con I2C");
}

// ========================
// LOOP
// ========================
void loop() { 
  // Tu función de sincronización se ejecuta cíclicamente de forma correcta
  actualizarControlSincronizacionCompleta();
}