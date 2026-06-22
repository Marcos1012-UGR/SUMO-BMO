// ========================
// PWM MOTORES ESCLAVO
// ========================
#define PWM3 5
#define PWM4 6

// ========================
// ENCODERS ESCLAVO
// ========================
#define INT3 2
#define INT4 3

// ========================
// BOTÓN START
// ========================
#define BOTON_START 13

// ========================
// ESTADO
// ========================
bool robotActivo = false;
bool ultimoEstadoBoton = HIGH;

// ========================
// DIRECCIÓN MOTORES ESCLAVO
// ========================
#define DIR_M3_A 4
#define DIR_M3_B 7
#define DIR_M4_A 8
#define DIR_M4_B 12

// ========================
// VELOCIDADES
// ========================
int vel3 = 0;
int vel4 = 0;

// ========================
// ISR (Interrupciones)
// ========================
void isr3() {
  encoder3++;
}
void isr4() {
  encoder4++;
}

long getE3() {
  return encoder3;
}

long getE4() {
  return encoder4;
}

void resetE3() {
  encoder3 = 0;
}

void resetE4() {
  encoder4 = 0;
}














void test1() {
  analogWrite(PWM3, 255);
  analogWrite(PWM3, 255);

  // F1
  digitalWrite(DIR_M3_A, HIGH);
  digitalWrite(DIR_M3_B, LOW);
  delay(1000);

  // R1
  digitalWrite(DIR_M3_A, LOW);
  digitalWrite(DIR_M3_B, HIGH);
  delay(1000);

  // F2
  digitalWrite(DIR_M3_B, LOW);
  digitalWrite(DIR_M4_A, HIGH);
  digitalWrite(DIR_M4_B, LOW);
  delay(1000);

  // R2
  digitalWrite(DIR_M4_A, LOW);
  digitalWrite(DIR_M4_B, HIGH);
  delay(1000);

  // Parar todo
  digitalWrite(DIR_M3_A, LOW);
  digitalWrite(DIR_M3_B, LOW);
  digitalWrite(DIR_M4_A, LOW);
  digitalWrite(DIR_M4_B, LOW);

  delay(2000);

  // Todos adelante
  digitalWrite(DIR_M3_A, HIGH);
  digitalWrite(DIR_M3_B, LOW);

  digitalWrite(DIR_M4_A, HIGH);
  digitalWrite(DIR_M4_B, LOW);

  delay(1000);

  // Todos atrás
  digitalWrite(DIR_M3_A, LOW);
  digitalWrite(DIR_M3_B, HIGH);

  digitalWrite(DIR_M4_A, LOW);
  digitalWrite(DIR_M4_B, HIGH);

  delay(1000);

  // Parar
  digitalWrite(DIR_M3_A, LOW);
  digitalWrite(DIR_M3_B, LOW);
  digitalWrite(DIR_M4_A, LOW);
  digitalWrite(DIR_M4_B, LOW);
}

void setupMotor() {

  attachInterrupt(digitalPinToInterrupt(INT3), isr3, FALLING);
  attachInterrupt(digitalPinToInterrupt(INT4), isr4, FALLING);

  // PWM
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  // Motores apagados al inicio
  analogWrite(PWM3, 0);
  analogWrite(PWM4, 0);

  // DIRECCIONES
  pinMode(DIR_M3_A, OUTPUT);
  pinMode(DIR_M3_B, OUTPUT);
  pinMode(DIR_M4_A, OUTPUT);
  pinMode(DIR_M4_B, OUTPUT);

  digitalWrite(DIR_M3_A, LOW);
  digitalWrite(DIR_M3_B, LOW);
  digitalWrite(DIR_M4_A, LOW);
  digitalWrite(DIR_M4_B, LOW);
}

void setMotor(DatosVelocidades act) {
  analogWrite(PWM3, act.v3);
  analogWrite(PWM4, act.v4);

  if (act.dir3 == true) {
    digitalWrite(DIR_M3_A, HIGH);
    digitalWrite(DIR_M3_B, LOW);
  } else {
    digitalWrite(DIR_M3_A, LOW);
    digitalWrite(DIR_M3_B, HIGH);
  }

  if (act.dir4 == true) {
    digitalWrite(DIR_M4_A, HIGH);
    digitalWrite(DIR_M4_B, LOW);
  } else {
    digitalWrite(DIR_M4_A, LOW);
    digitalWrite(DIR_M4_B, HIGH);
  }
}