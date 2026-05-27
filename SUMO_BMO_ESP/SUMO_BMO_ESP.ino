// // ========================
// // CONFIGURACIÓN DE PINES
// // ========================

// // UART
// #define UART_TX 17
// #define UART_RX 16

// // INTERRUPCIONES
// #define INT1 34
// #define INT2 35
// #define INT3 32
// #define INT4 33

// // PWM
// #define PWM1 25
// #define PWM2 26
// #define PWM3 27
// #define PWM4 14

// // SALIDAS DIGITALES
// int digitales[] = {4, 5, 18, 19, 21, 22, 23, 13};

// // ========================
// // ISR (Interrupciones)
// // ========================
// void IRAM_ATTR isr1() {}
// void IRAM_ATTR isr2() {}
// void IRAM_ATTR isr3() {}
// void IRAM_ATTR isr4() {}

// // ========================
// // SETUP
// // ========================
// void setup() {

//   Serial.begin(115200);

//   // UART2
//   Serial2.begin(115200, SERIAL_8N1, UART_RX, UART_TX);

//   // ------------------------
//   // INTERRUPCIONES
//   // ------------------------
//   pinMode(INT1, INPUT);
//   pinMode(INT2, INPUT);
//   pinMode(INT3, INPUT);
//   pinMode(INT4, INPUT);

//   attachInterrupt(INT1, isr1, RISING);
//   attachInterrupt(INT2, isr2, RISING);
//   attachInterrupt(INT3, isr3, RISING);
//   attachInterrupt(INT4, isr4, RISING);

//   // ------------------------
//   // PWM (LEDC)
//   // ------------------------
//   ledcAttach(PWM1, 5000, 8);
//   ledcAttach(PWM2, 5000, 8);
//   ledcAttach(PWM3, 5000, 8);
//   ledcAttach(PWM4, 5000, 8);

//   // ------------------------
//   // SALIDAS DIGITALES
//   // ------------------------
//   for (int i = 0; i < 8; i++) {
//     pinMode(digitales[i], OUTPUT);
//     digitalWrite(digitales[i], LOW);
//   }

//   digitalWrite(digitales[0], HIGH);
//   digitalWrite(digitales[2], HIGH);
// }

// // ========================
// // LOOP
// // ========================
// void loop() {

//   // Ejemplo PWM
//   ledcWrite(0, 128);
//   ledcWrite(1, 64);
//   ledcWrite(2, 200);
//   ledcWrite(3, 255);

//   delay(1000);
// }

// ========================
// CONFIGURACIÓN DE PINES - ARDUINO UNO R3
// ========================

// UART - UNO R3 tiene solo Serial (USB)
// No hay UART2 disponible. Se usa Serial (0,1) para comunicación
#define UART_TX 1  // Pin TX (conectado a USB)
#define UART_RX 0  // Pin RX (conectado a USB)

// INTERRUPCIONES - Solo pines 2 y 3 en UNO R3
// INT0 = Pin 2, INT1 = Pin 3 (limitado a 2 interrupciones)
#define INT1 2
#define INT2 3
// INT3 y INT4 no están disponibles en UNO R3
// Alternativa: usar pin change interrupts en otros pines

// PWM - Pines PWM en UNO R3: 3, 5, 6, 9, 10, 11
#define PWM1 3   // Timer2 (conflicto con INT2)
#define PWM2 5   // Timer0
#define PWM3 6   // Timer0
#define PWM4 9   // Timer1

// SALIDAS DIGITALES - Pines disponibles
int digitales[] = {4, 7, 8, 12, 13, A0, A1, A2};  // A0-A2 como digitales

// ========================
// ISR (Interrupciones)
// ========================
void isr1() {
  // Función para interrupción INT0 (Pin 2)
}

void isr2() {
  // Función para interrupción INT1 (Pin 3)
}

// NOTA: Para más interrupciones, usar PCINTs (Pin Change Interrupts)
// Ver función setupPinChangeInterrupts() abajo

// ========================
// Pin Change Interrupt Setup (Alternativa para más interrupciones)
// ========================
void setupPinChangeInterrupts() {
  // Para agregar interrupciones en más pines, usar:
  // PCICR - Pin Change Interrupt Control Register
  // PCMSK0, PCMSK1, PCMSK2 - Pin Change Mask Registers
  
  // Ejemplo: Habilitar PCINT en puerto B (pins 8-13)
  PCICR |= (1 << PCIE0);  // Habilitar grupo 0 (PCINT0-7)
  PCMSK0 |= (1 << PCINT0); // Habilitar PCINT0 (Pin 8)
  
  // Función ISR necesaria:
  // ISR(PCINT0_vect) { /* código */ }
}

// ========================
// SETUP
// ========================
void setup() {

  // Serial - UNO R3 solo tiene Serial (USB)
  Serial.begin(9600);  // Velocidad estándar (115200 puede causar problemas)
  
  // Serial2 NO EXISTE en UNO R3
  // Si necesitas segunda comunicación serial, usa SoftwareSerial
  // #include <SoftwareSerial.h>
  // SoftwareSerial mySerial(10, 11); // RX, TX
  // mySerial.begin(9600);

  // ------------------------
  // INTERRUPCIONES
  // ------------------------
  pinMode(INT1, INPUT);
  pinMode(INT2, INPUT);
  
  // Solo 2 interrupciones disponibles en UNO R3
  attachInterrupt(digitalPinToInterrupt(INT1), isr1, RISING);
  attachInterrupt(digitalPinToInterrupt(INT2), isr2, RISING);
  
  // Para INT3 e INT4, usar Pin Change Interrupts
  // setupPinChangeInterrupts();

  // ------------------------
  // PWM (analogWrite)
  // ------------------------
  // UNO R3 usa analogWrite() en lugar de ledcAttach()
  // Pines PWM: 3, 5, 6, 9, 10, 11
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);
  
  // No es necesario "attach" PWM en UNO R3, solo usar analogWrite()

  // ------------------------
  // SALIDAS DIGITALES
  // ------------------------
  for (int i = 0; i < 8; i++) {
    pinMode(digitales[i], OUTPUT);
    digitalWrite(digitales[i], LOW);
  }

  digitalWrite(digitales[0], HIGH);
  digitalWrite(digitales[2], HIGH);
  
  Serial.println("Setup completado - Arduino UNO R3");
}

// ========================
// LOOP
// ========================
void loop() {

  // Ejemplo PWM - analogWrite(pin, valor 0-255)
  analogWrite(PWM1, 128);  // ~50% PWM
  analogWrite(PWM2, 64);   // ~25% PWM
  analogWrite(PWM3, 200);  // ~78% PWM
  analogWrite(PWM4, 255);  // 100% PWM

  delay(1000);
}

// ========================
// ISR para Pin Change Interrupts (si la necesitas)
// ========================
// ISR(PCINT0_vect) {
//   // Se ejecuta cuando cambia cualquier pin en PCINT0-7
// }