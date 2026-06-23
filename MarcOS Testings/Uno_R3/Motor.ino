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
#define DIR_M1_A 7
#define DIR_M1_B 4
#define DIR_M2_A 12
#define DIR_M2_B 8

// ========================
// ESTADO
// ========================
bool robotActivo = false;
bool ultimoEstadoBoton = HIGH;

bool pressed = false;
bool longPressHandled = false;
unsigned long pressed_on = 0;

unsigned long ultimoCambio = 0;
const int debounceTime = 50;


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

  initCal();
}

void setMotor(DatosVelocidadesMaestro act) {
  analogWrite(PWM1, act.v1);
  analogWrite(PWM2, act.v2);

  if (act.dir1 == true) {
    digitalWrite(DIR_M1_A, HIGH);
    digitalWrite(DIR_M1_B, LOW);
  } else {
    digitalWrite(DIR_M1_A, LOW);
    digitalWrite(DIR_M1_B, HIGH);
  }

  if (act.dir2 == true) {
    digitalWrite(DIR_M2_A, HIGH);
    digitalWrite(DIR_M2_B, LOW);
  } else {
    digitalWrite(DIR_M2_A, LOW);
    digitalWrite(DIR_M2_B, HIGH);
  }
}

void aplicarVelocidades(bool FW1, bool FW2, bool FW3, bool FW4) {
  if (FW1) {
    digitalWrite(DIR_M1_A, HIGH);
    digitalWrite(DIR_M1_B, LOW);
  } else {
    digitalWrite(DIR_M1_A, LOW);
    digitalWrite(DIR_M1_B, HIGH);
  }

  if (FW2) {
    digitalWrite(DIR_M2_A, HIGH);
    digitalWrite(DIR_M2_B, LOW);
  } else {
    digitalWrite(DIR_M2_A, LOW);
    digitalWrite(DIR_M2_B, HIGH);
  }

  analogWrite(PWM1, vel1 * k1);
  analogWrite(PWM2, vel2 * k2);

  DatosVelocidades dvSend;
  dvSend.v3 = vel3 * k3;
  dvSend.v4 = vel4 * k4;
  dvSend.dir3 = FW3;
  dvSend.dir4 = FW4;
  sendI2C(dvSend);
}

void initCal() {
  noInterrupts();
  encoder1 = 0;
  encoder2 = 0;
  interrupts();


  dvm.v1 = 200;
  dvm.v2 = 200;
  dvm.dir1 = true;
  dvm.dir2 = true;
  setMotor(dvm);

  DatosVelocidades vCal = {200, 200, true, true};

  sendI2C(vCal);

  delay(3000);

  DatosEncoders de = {12, 12};
  Wire.requestFrom(DIR_ESCLAVO, sizeof(DatosEncoders));
  if (Wire.available() == sizeof(DatosEncoders)) {
    Wire.readBytes((byte*)&de, sizeof(de));
  }

  noInterrupts();

  long e1 = encoder1;
  long e2 = encoder2;

  interrupts();

  long e3 = de.e3;
  long e4 = de.e4;

  dvm.v1 = 0;
  dvm.v2 = 0;
  setMotor(dvm);

  vCal.v3 = 0;
  vCal.v4 = 0;
  sendI2C(vCal);

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

  Serial.print("e1 = ");
  Serial.println(e1);

  Serial.print("e2 = ");
  Serial.println(e2);

  Serial.print("e3 = ");
  Serial.println(e3);

  Serial.print("e4 = ");
  Serial.println(e4);

  Serial.println("Setup listo");
}

void test2() {
  bool estadoBoton = digitalRead(BOTON_START);

  // ======================
  // DETECCIÓN DE CAMBIO (debounce)
  // ======================
  if (estadoBoton != ultimoEstadoBoton) {
    if (millis() - ultimoCambio > debounceTime) {

      // FLANCO DE BAJADA (pulsación)
      if (estadoBoton == LOW) {
        pressed = true;
        longPressHandled = false;
        pressed_on = millis();
      }

      // FLANCO DE SUBIDA (soltar botón)
      if (estadoBoton == HIGH && pressed) {

        // Si NO fue long press → toggle normal
        if (!longPressHandled) {
          robotActivo = !robotActivo;

          if (robotActivo) {
            Serial.println("ROBOT ACTIVADO");
            delay(3000);
          }
          else
            Serial.println("ROBOT DETENIDO");
        }

        pressed = false;
      }

      ultimoCambio = millis();
    }
  }

  ultimoEstadoBoton = estadoBoton;

  // ======================
  // DETECCIÓN LONG PRESS
  // ======================
  if (pressed && !longPressHandled) {
    if (millis() - pressed_on >= 3000) {
      DEMO = true;
      longPressHandled = true;

      Serial.println("MODO DEMO ACTIVADO");
    }
  }

  // ======================
  // CONTROL MOTORES
  // ======================
  if (robotActivo) {
    vel1 = vel2 = vel3 = vel4 = 180;
  } else {
    vel1 = vel2 = vel3 = vel4 = 0;
  }

  aplicarVelocidades(true, true, true, true);
}

void definitive() {
  bool estadoBoton = digitalRead(BOTON_START);

  // ======================
  // DETECCIÓN DE CAMBIO (debounce)
  // ======================
  if (estadoBoton != ultimoEstadoBoton) {
    if (millis() - ultimoCambio > debounceTime) {

      // FLANCO DE BAJADA (pulsación)
      if (estadoBoton == LOW) {
        pressed = true;
        longPressHandled = false;
        pressed_on = millis();
      }

      // FLANCO DE SUBIDA (soltar botón)
      if (estadoBoton == HIGH && pressed) {

        // Si NO fue long press → toggle normal
        if (!longPressHandled) {
          robotActivo = !robotActivo;

          if (robotActivo) {
            Serial.println("ROBOT ACTIVADO");
            delay(3000);
          }
          else {
            Serial.println("ROBOT DETENIDO");
            ATTACK = false;
          }
        }

        pressed = false;
      }

      ultimoCambio = millis();
    }
  }

  ultimoEstadoBoton = estadoBoton;

  // ======================
  // DETECCIÓN LONG PRESS
  // ======================
  if (pressed && !longPressHandled) {
    if (millis() - pressed_on >= 3000) {
      DEMO = true;
      longPressHandled = true;

      Serial.println("MODO DEMO ACTIVADO");
    }
  }

  // ======================
  // CONTROL MOTORES
  // ======================
  if (robotActivo) {
    IA();
  } else {
    vel1 = vel2 = vel3 = vel4 = 0;
    aplicarVelocidades(true, true, true, true);
  }
}

void MovSinPutoMiedo() {
  vel1 = vel2 = vel3 = vel4 = 255;
  aplicarVelocidades(true, true, true, true);
}

void MovSTOP() {
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
}

void MovExploracion() {
  vel1 = vel2 = vel3 = vel4 = 150;
  aplicarVelocidades(true, true, true, true);
}

void MovCorregir(bool izq, bool dch, bool tr) {
  vel1 = vel2 = vel3 = vel4 = 255;
  String res = String(izq) + " " + String (dch) + " " + String(tr);
  Serial.println(res);

  if (izq && !dch && !tr) {           // Tocamos izq
    Serial.print("A");
    aplicarVelocidades(true, false, false, true);
  } else if (dch && !izq && !tr) {    // Tocamos dch
    Serial.print("B");
    aplicarVelocidades(false, true, true, false);
  } else if (tr && !izq && !dch) {    // Tocamos tr
    Serial.print("C");
    aplicarVelocidades(true, true, true, true);
  }

  else if (izq && dch && !tr) {      // Tocamos izq y dch
    Serial.print("D");
    aplicarVelocidades(false, false, false, false);
  } else if (tr && izq && !dch) {    // Tocamos izq y tr
    Serial.print("E");
    vel2 = vel3 = 0;
    aplicarVelocidades(true, true, true, true);
  } else if (tr && dch && !izq) {    // Tocamos dch y tr
    Serial.print("F");
    vel1 = vel4 = 0;
    aplicarVelocidades(true, true, true, true);
  }

  else if (tr && dch && izq) {       // TODOS
    Serial.print("G");
    aplicarVelocidades(true, true, true, true);
  }
}

void demo() {
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);

  delay(5000);


  // ADELANTE
  vel1 = vel2 = vel3 = vel4 = 180;
  aplicarVelocidades(true, true, true, true);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // ATRAS
  vel1 = vel2 = vel3 = vel4 = 180;
  aplicarVelocidades(false, false, false, false);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // IZQ
  vel1 = vel2 = vel3 = vel4 = 180;
  aplicarVelocidades(false, true, true, false);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // DCHA
  vel1 = vel2 = vel3 = vel4 = 180;
  aplicarVelocidades(true, false, false, true);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // DIAG IzU
  vel1 = vel2 = vel3 = vel4 = 180;
  vel1 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // DIAG DcU
  vel1 = vel2 = vel3 = vel4 = 180;
  vel2 = vel3 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // DIAG IzD
  vel1 = vel2 = vel3 = vel4 = 180;
  vel2 = vel3 = 0;
  aplicarVelocidades(false, false, false, false);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // DIAG DcD
  vel1 = vel2 = vel3 = vel4 = 180;
  vel1 = vel4 = 0;
  aplicarVelocidades(false, false, false, false);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // ROT I
  vel1 = vel2 = vel3 = vel4 = 180;
  aplicarVelocidades(true, false, true, false);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  // ROT D
  vel1 = vel2 = vel3 = vel4 = 180;
  aplicarVelocidades(false, true, false, true);
  delay(3000);
  // STOP
  vel1 = vel2 = vel3 = vel4 = 0;
  aplicarVelocidades(true, true, true, true);
  delay(500);

  DEMO = false;
}