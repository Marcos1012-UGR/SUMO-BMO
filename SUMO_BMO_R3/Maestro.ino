// ========================
// CONFIGURACIÓN DE PINES - ARDUINO UNO R3
// ========================
#include <Arduino.h>
#include <Wire.h> // Librería esencial para I2C

// ========================
// I2C
// ========================
#define I2C_SDA A4 
#define I2C_SCL A5 
#define DIR_ESCLAVO 67 

// ========================
// ESTRUCTURAS I2C
// ========================
struct DatosEncoders {
  long e3;
  long e4;
};

struct DatosVelocidades {
  byte v3;
  byte v4;
};

// ========================
// BOTÓN START
// ========================
#define BOTON_START A3

// ========================
// PWM MOTORES MAESTRO
// ========================
#define PWM1 5
#define PWM2 6

// ========================
// ENCODERS MAESTRO
// ========================
#define INT1 2
#define INT2 3

volatile int encoder1 = 0;
volatile int encoder2 = 0;

// ========================
// DIRECCIÓN MOTORES MAESTRO
// ========================
#define DIR_M1_A 4
#define DIR_M1_B 7
#define DIR_M2_A 8
#define DIR_M2_B 12

// ========================
// ESTADO
// ========================
bool robotActivo = false;
bool ultimoEstadoBoton = HIGH;

// ========================
// VELOCIDADES
// ========================
int vel1 = 150; 
int vel2 = 150; 
int vel3 = 150; 
int vel4 = 150; 

// ========================
// FACTORES DE CORRECCIÓN
// ========================
float k1 = 1.0;
float k2 = 1.0;
float k3 = 1.0;
float k4 = 1.0;

// ========================
// SENSORES
// ========================

const int sensorIzquierdo = 11; 
const int sensorDerecho   = A3; 
const int sensorTrasero   = 4;  

// ========================
// VARIABLES DELIBERATIVAS 
// ========================
unsigned long tiempoUltimaLinea = 0;

// 0.0 = Centro (Seguro) 
// 1.0 = Borde (Peligro)
float peligroEstimado = 0.0; 

// 1 = Izquierda, 2 = Derecha, 3 = Atrás
int ultimaDireccionBorde = 0;  

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
// Funciones - MAESTRO
// ========================
void test1() {
  analogWrite(PWM1, 255);
  analogWrite(PWM2, 255);

  delay(2000);

  // F1
  digitalWrite(DIR_M1_A, HIGH);
  digitalWrite(DIR_M1_B, LOW);
  delay(1000);

  // R1
  digitalWrite(DIR_M1_A, LOW);
  digitalWrite(DIR_M1_B, HIGH);
  delay(1000);

  // F2
  digitalWrite(DIR_M2_A, HIGH);
  digitalWrite(DIR_M2_B, LOW);
  delay(1000);

  // R2
  digitalWrite(DIR_M2_A, LOW);
  digitalWrite(DIR_M2_B, HIGH);
  delay(1000);

  // F3
  // enviar orden al esclavo
  delay(1000);

  // R3
  // enviar orden al esclavo
  delay(1000);

  // F4
  // enviar orden al esclavo
  delay(1000);

  // R4
  // enviar orden al esclavo
  delay(1000);

  // Parar todo
  digitalWrite(DIR_M1_A, LOW);
  digitalWrite(DIR_M1_B, LOW);
  digitalWrite(DIR_M2_A, LOW);
  digitalWrite(DIR_M2_B, LOW);

  delay(2000);

  // Todos adelante
  digitalWrite(DIR_M1_A, HIGH);
  digitalWrite(DIR_M1_B, LOW);

  digitalWrite(DIR_M2_A, HIGH);
  digitalWrite(DIR_M2_B, LOW);

  delay(1000);

  // Todos atrás
  digitalWrite(DIR_M1_A, LOW);
  digitalWrite(DIR_M1_B, HIGH);

  digitalWrite(DIR_M2_A, LOW);
  digitalWrite(DIR_M2_B, HIGH);

  delay(1000);

  // Parar
  digitalWrite(DIR_M1_A, LOW);
  digitalWrite(DIR_M1_B, LOW);
  digitalWrite(DIR_M2_A, LOW);
  digitalWrite(DIR_M2_B, LOW);

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
// SETUP
// ========================
void setup() {
  Serial.begin(9600);

  // Botón de START
  pinMode(BOTON_START, INPUT_PULLUP);

  // I2C
  Wire.begin(); // Se une al bus como Maestro

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
  pinMode(DIR_M1_A, OUTPUT);
  pinMode(DIR_M1_B, OUTPUT);
  pinMode(DIR_M2_A, OUTPUT);
  pinMode(DIR_M2_B, OUTPUT);

  digitalWrite(DIR_M1_A, LOW);
  digitalWrite(DIR_M1_B, LOW);
  digitalWrite(DIR_M2_A, LOW);
  digitalWrite(DIR_M2_B, LOW);

  // Todas adelante por defecto
  digitalWrite(DIR_M1_A, HIGH);
  digitalWrite(DIR_M1_B, LOW);

  digitalWrite(DIR_M2_A, HIGH);
  digitalWrite(DIR_M2_B, LOW);

  // ========================
  // CALIBRACIÓN INICIAL
  // ========================

  Serial.println("Calibrando motores...");

  noInterrupts();
  encoder1 = 0;
  encoder2 = 0;
  interrupts();

  // PWM idéntico para todos
  analogWrite(PWM1, 200);
  analogWrite(PWM2, 200);

  DatosVelocidades vCal = {200, 200};

  Wire.beginTransmission(DIR_ESCLAVO);
  Wire.write((byte*)&vCal, sizeof(vCal));
  Wire.endTransmission();

  // Tiempo de medida
  delay(3000);

  // Leer esclavo
  DatosEncoders encodersEsclavo = {0, 0};

  Wire.requestFrom(DIR_ESCLAVO, sizeof(DatosEncoders));

  if (Wire.available() == sizeof(DatosEncoders)) {
    Wire.readBytes((char*)&encodersEsclavo,
                   sizeof(DatosEncoders));
  }

  noInterrupts();

  long e1 = encoder1;
  long e2 = encoder2;

  interrupts();

  long e3 = encodersEsclavo.e3;
  long e4 = encodersEsclavo.e4;

  // Parar motores
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);

  vCal.v3 = 0;
  vCal.v4 = 0;

  Wire.beginTransmission(DIR_ESCLAVO);
  Wire.write((byte*)&vCal, sizeof(vCal));
  Wire.endTransmission();

  // Evitar división por cero
  if (e1 > 0) {

    if (e2 > 0)
      k2 = (float)e1 / e2;

    if (e3 > 0)
      k3 = (float)e1 / e3;

    if (e4 > 0)
      k4 = (float)e1 / e4;
  }

  Serial.println("Calibracion terminada");

  Serial.print("k1 = ");
  Serial.println(k1, 4);

  Serial.print("k2 = ");
  Serial.println(k2, 4);

  Serial.print("k3 = ");
  Serial.println(k3, 4);

  Serial.print("k4 = ");
  Serial.println(k4, 4);

  Serial.println("Setup listo");
}

// ========================
//  LOOP
// ========================
void loop()
{
    bool estadoBoton = digitalRead(BOTON_START);

    if (ultimoEstadoBoton == HIGH &&
        estadoBoton == LOW)
    {
        robotActivo = !robotActivo;

        if (robotActivo)
        {
            Serial.println("ROBOT ACTIVADO");
        }
        else
        {
            Serial.println("ROBOT DETENIDO");

            vel1 = 0;
            vel2 = 0;
            vel3 = 0;
            vel4 = 0;

            aplicarVelocidades();
        }

        delay(200);
    }

    ultimoEstadoBoton = estadoBoton;

    if (!robotActivo)
        return;

    // ======================
    // LOGICA DEL ROBOT
    // ======================

    vel1 = 180;
    vel2 = 180;
    vel3 = 180;
    vel4 = 180;

    aplicarVelocidades();
}

void aplicarVelocidadesMaestro() {
  // Aplica los PWM locales de inmediato
  analogWrite(PWM1, vel1 * k1);
  analogWrite(PWM2, vel2 * k2);

  // Envía los PWM al esclavo por I2C de inmediato
  DatosVelocidades vEsclavo = {(byte)(vel3 * k3), (byte)(vel4 * k4)};
  Wire.beginTransmission(DIR_ESCLAVO);
  Wire.write((byte*)&vEsclavo, sizeof(DatosVelocidades));
  Wire.endTransmission();
}

/// LOOP DELIBERATIVO ///
/*

// ¡¡¡Aun en desarrollo!!!

void loop()
{
    bool estadoBoton = digitalRead(BOTON_START);

    if (ultimoEstadoBoton == HIGH &&
        estadoBoton == LOW)
    {
        robotActivo = !robotActivo;

        if (robotActivo)
        {
            Serial.println("ROBOT ACTIVADO");
        }
        else
        {
            Serial.println("ROBOT DETENIDO");

            vel1 = 0;
            vel2 = 0;
            vel3 = 0;
            vel4 = 0;

            aplicarVelocidadesMaestro();
        }

        delay(200);
    }

    ultimoEstadoBoton = estadoBoton;

    if (!robotActivo)
        return;

    // ====================================
    // PERCEPCIÓN
    // ====================================

    int izq  = digitalRead(sensorIzquierdo);
    int der  = digitalRead(sensorDerecho);
    int tras = digitalRead(sensorTrasero);

    unsigned long tiempoActual = millis();

    // ====================================
    // ACTUALIZACIÓN DEL MAPA
    // ====================================

    if (izq == HIGH || der == HIGH || tras == HIGH)
    {
        peligroEstimado = 1.0;
        tiempoUltimaLinea = tiempoActual;

        if (izq == HIGH)
            ultimaDireccionBorde = 1;

        if (der == HIGH)
            ultimaDireccionBorde = 2;

        if (tras == HIGH)
            ultimaDireccionBorde = 3;
    }
    else
    {
        unsigned long tiempoCaminado =
            tiempoActual - tiempoUltimaLinea;

        if (tiempoCaminado > 1200)
        {
            peligroEstimado = 0.85;
        }
        else
        {
            peligroEstimado =
                (tiempoCaminado / 1200.0) * 0.7;
        }
    }

    // ====================================
    // TOMA DE DECISIONES
    // ====================================

    PensarYActuar();
}

void aplicarVelocidadesMaestro()
{
    analogWrite(PWM1, vel1 * k1);
    analogWrite(PWM2, vel2 * k2);

    DatosVelocidades vEsclavo =
    {
        (byte)(vel3 * k3),
        (byte)(vel4 * k4)
    };

    Wire.beginTransmission(DIR_ESCLAVO);
    Wire.write((byte*)&vEsclavo,
               sizeof(DatosVelocidades));
    Wire.endTransmission();
}

void PensarYActuar()
{
    // PLAN A: Emergencia
    if (peligroEstimado >= 1.0)
    {
        if (ultimaDireccionBorde == 1)
            evadirDiagonalAtrasDerecha();

        if (ultimaDireccionBorde == 2)
            evadirDiagonalAtrasIzquierda();

        if (ultimaDireccionBorde == 3)
            evadirLateralIzquierda();

        delay(300);

        peligroEstimado = 0.4;

        return;
    }

    // PLAN B: Preventivo
    if (peligroEstimado > 0.80)
    {
        ejecutarManiobraSeguraMecanum();
        return;
    }

    // PLAN C: Ataque
    ejecutarPlanBuscarEnemigoSincrono();
}

void ejecutarManiobraSeguraMecanum()
{
    // M1 adelante
    digitalWrite(DIR_M1_A, HIGH);
    digitalWrite(DIR_M1_B, LOW);

    // M2 adelante
    digitalWrite(DIR_M2_A, HIGH);
    digitalWrite(DIR_M2_B, LOW);

    vel1 = 110;
    vel2 = 110;
    vel3 = 110;
    vel4 = 110;

    aplicarVelocidadesMaestro();
}

void ejecutarPlanBuscarEnemigoSincrono()
{
    // M1 adelante
    digitalWrite(DIR_M1_A, HIGH);
    digitalWrite(DIR_M1_B, LOW);

    // M2 adelante
    digitalWrite(DIR_M2_A, HIGH);
    digitalWrite(DIR_M2_B, LOW);

    vel1 = 200;
    vel2 = 200;
    vel3 = 200;
    vel4 = 200;

    aplicarVelocidadesMaestro();
}

void evadirDiagonalAtrasDerecha()
{
    // M1 atrás
    digitalWrite(DIR_M1_A, LOW);
    digitalWrite(DIR_M1_B, HIGH);

    // M2 parado
    digitalWrite(DIR_M2_A, LOW);
    digitalWrite(DIR_M2_B, LOW);

    vel1 = 220;
    vel2 = 0;
    vel3 = 0;
    vel4 = 220;

    aplicarVelocidadesMaestro();
}

void evadirLateralIzquierda()
{
    // M1 atrás
    digitalWrite(DIR_M1_A, LOW);
    digitalWrite(DIR_M1_B, HIGH);

    // M2 adelante
    digitalWrite(DIR_M2_A, HIGH);
    digitalWrite(DIR_M2_B, LOW);

    vel1 = 230;
    vel2 = 230;
    vel3 = 230;
    vel4 = 230;

    aplicarVelocidadesMaestro();
}

*/