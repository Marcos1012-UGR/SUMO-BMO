// Analog Sharp Pin
const int pinSharp = A0;

// Timing control
unsigned long last = 0;
const long interval = 500;

void setupSHARP() {
  
  Serial.println("--- Telemetria: Sensor Infrarrojo Sharp ---");
  Serial.println("Lectura Analoga \t Distancia Aprox (cm)");
  Serial.println("--------------------------------------------");
}

void loopSHARP() {
  unsigned long act = millis();

  // Each interval
  if (act - last >= interval) {
    last = act;

    // 1. Read raw info (0 - 1023)
    int AnalogValue = analogRead(pinSharp);

    // 2. Calculate voltage (0V - 5V)
    float voltage = AnalogValue * (5.0 / 1023.0);

    // 3. Volt to cm
    // Esta fórmula está optimizada para el modelo estándar de 10-80 cm (GP2Y0A21)
    // Evitamos la división por cero si el voltaje es extremadamente bajo
    float distCM = 0.0;
    if (voltage > 0.4) { 
      distCM = 29.988 * pow(voltage, -1.173); 
    } else {
      distCM = -1.0; // Fuera de rango (demasiado lejos)
    }

    // 4. Mostrar los resultados en el Monitor Serie
    Serial.print(AnalogValue);
    Serial.print(" unidades \t\t ");
    
    if (distCM == -1.0 || distCM > 80.0) {
      Serial.println("Fuera de rango (> 80 cm)");
    } else if (distCM < 10.0) {
      Serial.println("Demasiado cerca (< 10 cm - Lectura no confiable)");
    } else {
      Serial.print(distCM, 1); // Imprime con 1 decimal
      Serial.println(" cm");
    }
  }
}