// ========================
// CONFIGURACIÓN DE PINES
// ========================

// UART
#define UART_TX 17
#define UART_RX 16

// INTERRUPCIONES
#define INT1 34
#define INT2 35
#define INT3 32
#define INT4 33

// PWM
#define PWM1 25
#define PWM2 26
#define PWM3 27
#define PWM4 14

// SALIDAS DIGITALES
int digitales[] = {4, 5, 18, 19, 21, 22, 23, 13};

// ========================
// ISR (Interrupciones)
// ========================
void IRAM_ATTR isr1() {}
void IRAM_ATTR isr2() {}
void IRAM_ATTR isr3() {}
void IRAM_ATTR isr4() {}

// ========================
// SETUP
// ========================
void setup() {

  Serial.begin(115200);

  // UART2
  Serial2.begin(115200, SERIAL_8N1, UART_RX, UART_TX);

  // ------------------------
  // INTERRUPCIONES
  // ------------------------
  pinMode(INT1, INPUT);
  pinMode(INT2, INPUT);
  pinMode(INT3, INPUT);
  pinMode(INT4, INPUT);

  attachInterrupt(INT1, isr1, RISING);
  attachInterrupt(INT2, isr2, RISING);
  attachInterrupt(INT3, isr3, RISING);
  attachInterrupt(INT4, isr4, RISING);

  // ------------------------
  // PWM (LEDC)
  // ------------------------
  ledcAttach(PWM1, 5000, 8);
  ledcAttach(PWM2, 5000, 8);
  ledcAttach(PWM3, 5000, 8);
  ledcAttach(PWM4, 5000, 8);

  // ------------------------
  // SALIDAS DIGITALES
  // ------------------------
  for (int i = 0; i < 8; i++) {
    pinMode(digitales[i], OUTPUT);
    digitalWrite(digitales[i], LOW);
  }

  digitalWrite(digitales[0], HIGH);
  digitalWrite(digitales[2], HIGH);
}

// ========================
// LOOP
// ========================
void loop() {

  // Ejemplo PWM
  ledcWrite(0, 128);
  ledcWrite(1, 64);
  ledcWrite(2, 200);
  ledcWrite(3, 255);

  delay(1000);
}