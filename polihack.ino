#define trigPin 13
#define echoPin 12
#define motorEnablePin 9  // Enable pin for the motor
#define motorInput1 11   // Input 1 pin on the H-bridge
#define motorInput2 10    // Input 2 pin on the H-bridge

void setup() {
  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  long unsigned duration, distance;

  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pulse duration
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  // Output distance to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Adjust motor speed based on distance
  if (distance < 25) {
    // If distance is smaller than 15 cm, go as fast as possible
    analogWrite(motorEnablePin, 255);
    digitalWrite(motorInput1, HIGH);
    digitalWrite(motorInput2, LOW);
  } else if (distance >= 25 && distance < 65) {
    // If distance is between 15 cm and 150 cm, adjust speed proportionally
    int motorSpeed = map(distance, 25, 50, 140,170);
    analogWrite(motorEnablePin, motorSpeed);
    digitalWrite(motorInput1, HIGH);
    digitalWrite(motorInput2, LOW);
  } else if (distance >= 65 && distance <=80 ) {
    // If distance is between 150 cm and 200 cm, toggle the motor state every quarter of a second
    static unsigned long previousMillis = 0;
    static int motorState = LOW;

    if (millis() - previousMillis > 80) {
      previousMillis = millis();
      motorState = (motorState == LOW) ? HIGH : LOW;
      digitalWrite(motorEnablePin, motorState);
    }
  } else {
    // If distance is greater than 200 cm, stop the motor
    analogWrite(motorEnablePin, 0);
    digitalWrite(motorInput1, LOW);
  }

  delay(100); // Add a delay for stability
}
