// ========================
// CONFIGURACIÓN DE PINES - ARDUINO UNO R3
// ========================

// INTERRUPCIONES (reservadas)
#define INT1 2
#define INT2 3

// PWM (sin conflictos)
#define PWM1 5
#define PWM2 6
#define PWM3 9
#define PWM4 10

// SALIDAS DIGITALES
int digitales[] = {4, 7, 8, 12, 13, A0, A1, A2};

// ========================
// ISR (Interrupciones)
// ========================
void isr1() {}
void isr2() {}

// ========================
// SETUP
// ========================
void setup() {

  Serial.begin(9600);

  // ------------------------
  // INTERRUPCIONES (preparadas)
  // ------------------------
  pinMode(INT1, INPUT);
  pinMode(INT2, INPUT);

  // NO activamos todavía, pero listo:
  // attachInterrupt(digitalPinToInterrupt(INT1), isr1, RISING);
  // attachInterrupt(digitalPinToInterrupt(INT2), isr2, RISING);

  // ------------------------
  // PWM
  // ------------------------
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  // ------------------------
  // SALIDAS DIGITALES
  // ------------------------
  for (int i = 0; i < 8; i++) {
    pinMode(digitales[i], OUTPUT);
    digitalWrite(digitales[i], LOW);
  }

  Serial.println("Setup listo - UNO R3");
}

// ========================
// LOOP
// ========================
void loop() {
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