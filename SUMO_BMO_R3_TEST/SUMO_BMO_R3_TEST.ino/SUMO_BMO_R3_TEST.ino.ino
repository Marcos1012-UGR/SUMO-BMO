volatile long count1 = 0;
volatile long count2 = 0;

#define EN1 4
#define EN2 8
#define PWM1 5
#define PWM2 6

void encoder1() { count1++; }
void encoder2() { count2++; }

void setup() {
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2), encoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(3), encoder2, RISING);
  
  //EN y PWM
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);

  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);

  //EN
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);

  analogWrite(PWM1, 255);
  analogWrite(PWM2, 255);
}

void loop() {
  count1 = 0;
  count2 = 0;

  delay(1000);

  Serial.print("Motor 1: ");
  Serial.println(count1);

  Serial.print("Motor 2: ");
  Serial.println(count2);

  Serial.println("----");
}