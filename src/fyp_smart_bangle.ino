// Define Sensor & Actuator Pins
const int pulsePin = A0;        // Simulated Pulse Sensor (Potentiometer)
const int trigPin = 8;          // Ultrasonic Sensor - Trigger
const int echoPin = 9;          // Ultrasonic Sensor - Echo
const int buzzerPin = 10;       // Buzzer
const int cameraLed = 6;        // LED simulating Camera
const int pulseLedPin = 7;      // Pulse Alert LED
const int buttonPin = 4;        // Master ON/OFF Button

// System state control
bool systemActive = false;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(9600);

  // Set pin modes
  pinMode(pulsePin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(cameraLed, OUTPUT);
  pinMode(pulseLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Master button with pull-up
}

void loop() {
  // Master button toggles the system ON/OFF
  bool currentButtonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    systemActive = !systemActive;
    Serial.println(systemActive ? "✅ System ON" : "❌ System OFF");
    delay(300);  // debounce delay
  }
  lastButtonState = currentButtonState;

  if (systemActive) {
    // Simulate Pulse Sensor Reading
    int pulse = analogRead(pulsePin);
    Serial.println("Pulse: " + String(pulse));

    // Simulate Ultrasonic Distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;
    Serial.print("Distance (cm): ");
    Serial.println(distance);

    // Modified: Detect when user is TOO FAR (> 60 cm)
    if (pulse > 500 || distance > 60) {
      // Pulse Alert
      if (pulse > 500) {
        digitalWrite(pulseLedPin, HIGH);
        Serial.println("⚠️ Abnormal Heart Rate Detected");
      } else {
        digitalWrite(pulseLedPin, LOW);
      }

      // Buzzer Alert for User Too Far
      if (distance > 60) {
        digitalWrite(buzzerPin, HIGH);
        Serial.println("⚠️ Hazard Detected: User too far from guidance");
      } else {
        digitalWrite(buzzerPin, LOW);
      }

      digitalWrite(cameraLed, HIGH);  // Turn camera ON
      delay(250);
      digitalWrite(cameraLed, LOW);   // Turn camera OFF
      Serial.println("🚨 Camera ON - Recording suspicious activity");
      Serial.println("[Cloud] Footage saved");
      Serial.println("[SIM800L] SMS Alert sent to caregiver");
    } else {
      digitalWrite(pulseLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(cameraLed, LOW);   // Turn camera OFF
      Serial.println("Camera OFF - Normal state");
    }

    // Simulated GPS
    Serial.println("GPS: Lat 3.1400, Long 101.6869");
  }

  delay(500);  // Delay for readability
}

