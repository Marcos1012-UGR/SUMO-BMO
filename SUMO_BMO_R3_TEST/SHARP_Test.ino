// ========================
// CALIBRACIÓN SENSOR SHARP
// ========================

#define SHARP_PIN A0

void setup()
{
    Serial.begin(115200);

    Serial.println("Calibracion Sharp");
    Serial.println("Coloca un objeto a distintas distancias");
    Serial.println("y apunta los valores.");
}

void loop()
{
    int lectura = analogRead(SHARP_PIN);

    float voltaje = lectura * (5.0 / 1023.0);

    Serial.print("ADC: ");
    Serial.print(lectura);

    Serial.print("  Voltaje: ");
    Serial.print(voltaje, 3);
    Serial.println(" V");

    delay(200);
}