// ========================
// ESTRUCTURAS I2C
// ========================
struct DatosEncoders {
  long e3;
  long e4;
};

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

  // F1
  digitalWrite(DIR_M1_A, HIGH);
  digitalWrite(DIR_M1_B, LOW);
  delay(1000);

  // R1
  digitalWrite(DIR_M1_A, LOW);
  digitalWrite(DIR_M1_B, HIGH);
  delay(1000);

  // F2
  digitalWrite(DIR_M1_B, LOW);
  digitalWrite(DIR_M2_A, HIGH);
  digitalWrite(DIR_M2_B, LOW);
  delay(1000);

  // R2
  digitalWrite(DIR_M2_A, LOW);
  digitalWrite(DIR_M2_B, HIGH);
  delay(1000);

  /*
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

  */

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
}

void setupMotor() {
  

  // Botón de START
  pinMode(BOTON_START, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(INT1), isr1, FALLING);
  attachInterrupt(digitalPinToInterrupt(INT2), isr2, FALLING);

  // PWM
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);

  // Motores apagados al inicio
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);

  // DIRECCIONES
  pinMode(DIR_M1_A, OUTPUT);
  pinMode(DIR_M1_B, OUTPUT);
  pinMode(DIR_M2_A, OUTPUT);
  pinMode(DIR_M2_B, OUTPUT);

  digitalWrite(DIR_M1_A, LOW);
  digitalWrite(DIR_M1_B, LOW);
  digitalWrite(DIR_M2_A, LOW);
  digitalWrite(DIR_M2_B, LOW);
}