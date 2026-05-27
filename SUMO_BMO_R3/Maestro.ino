// ========================
// CONFIGURACIÓN DE PINES - ARDUINO UNO R3
// ========================
#include <Wire.h> //Librería esencial para I2C

// CONFIGURACIÓN I2C (Hardcodeada nativamente en A4 y A5 para UNO R3)
#define I2C_SDA A4 
#define I2C_SCL A5 
const byte DIR_ESCLAVO = 0x69; 
volatile int encoder1 = 0, encoder2 = 0;
int vel1 = 150, vel2 = 150;

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


// ========================
// Funciones Framework I2C (Eventos de comunicación binaria)
// ========================

// Se ejecuta automáticamente si este Arduino es ESCLAVO y el maestro le envía un código
void i2cRecibirDato(int cuantosBytes) {
  if (Wire.available() > 0) {
    byte instruccion = Wire.read(); // Lee la instrucción binaria hardcodeada
    
    // Aquí puedes meter un switch para reaccionar a los códigos binarios
    // Ejemplo: if(instruccion == 0x01) { ... }
  }
}

// Se ejecuta automáticamente si este Arduino es ESCLAVO y el maestro le pide datos
void i2cPeticionDato() {
  byte respuesta = 0xAA; // Código binario de respuesta de ejemplo
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
// SETUPEs la opción más directa para enviar texto o comandos sencillos entre dos placas.
// ========================
void setup() {


  Serial.begin(9600);

  // ------------------------
  // I2C
  // ------------------------
  Wire.begin();

  // ------------------------
  // ENCODERS
  // ------------------------
  pinMode(INT1, INPUT_PULLUP);
  pinMode(INT2, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(INT1),
    isr1,
    FALLING
  );

  attachInterrupt(
    digitalPinToInterrupt(INT2),
    isr2,
    FALLING
  );

  // ------------------------
  // PWM
  // ------------------------
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  // Motores apagados al inicio
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  analogWrite(PWM3, 0);
  analogWrite(PWM4, 0);

  // ------------------------
  // DIRECCIONES
  // ------------------------
  for (int i = 0; i < 8; i++) {

    pinMode(digitales[i], OUTPUT);
    digitalWrite(digitales[i], LOW);
  }

  // Todas adelante
  digitalWrite(digitales[0], HIGH);
  digitalWrite(digitales[2], HIGH);
  digitalWrite(digitales[4], HIGH);
  digitalWrite(digitales[6], HIGH);

  Serial.println("Setup listo - UNO R3 con I2C");
}

// ========================
// LOOP
// ========================
void loop() { // Comprobar si esta bien el loop

  // Aplicar PWM
  analogWrite(PWM1, vel1);
  analogWrite(PWM2, vel2);

  // Tiempo de muestreo
  delay(100);

  // Copia segura encoder
  noInterrupts();

  long e1 = encoder1;
  long e2 = encoder2;

  encoder1 = 0;
  encoder2 = 0;

  interrupts();

  // Error de velocidad
  int error = e1 - e2;

  // Corrección proporcional
  vel2 += error * 0.5;
  vel1 -= error * 0.5;

  // Limitar PWM
  vel1 = constrain(vel1, 0, 255);
  vel2 = constrain(vel2, 0, 255);

  // Debug
  Serial.print("E1: ");
  Serial.print(e1);

  Serial.print("E2: ");
  Serial.print(e2);

  Serial.print("PWM1: ");
  Serial.print(vel1);

  Serial.print("PWM2: ");
  Serial.println(vel2);
}