/*
 * Project: Smart Parking Assist System
 * Description: Non-blocking ultrasonic distance measurement with multiple zone alerts.
 * Uses HC-SR04 to measure to provide visual (LED) and audio (Buzzer) feedback.
 */
 
 // =======================================
 // 1. HARDWARE PIN MAPPING
 // =======================================
int trigPin=12; // Output: Triggers the ultrasonic sound wave
int echoPin=13; // Input: Listens for the returning sound wave
int buzzPin=5; // Audio feedback
int redLED=2; // Danger Zone Indicator
int yellowLED=3; // Caution Zone Indicator
int greenLED=4; // Safe Zone Indicator

void setup() {
  // Initialize Serial Monitor for debugging and data logging
  Serial.begin(9600);
  
  // Configure pin directions.
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(buzzPin,OUTPUT);
  pinMode(redLED,OUTPUT);
  pinMode(yellowLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
}

// ========================================
// 2. CORE LOGIC: DISTANCE CALCULATION
// ========================================
float getDistance(){
  // Step A: Clean the trigger line (Ensures 0V before firing)
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  
  // Step B: Fire the ultrasonic burst
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  
  // Step C: Read travel time with a 30ms timeout to prevent system hang
  unsigned long pingTravelTime=pulseIn(echoPin,HIGH,30000);
  
  /** MATH CALIBRATION 
   *----------------------------------------
   * Ideal case formula: Distance = (pingTravelTime*0.034)/2
   ** Custom Calibration: Based on my physical hardware testing,
   * an actual distance of 5 cm returned a ping travel time of 274us.
   * Using this empirical data, our custom multiplier is (5.0/274.0).
   * This accounts for specific crystal oscillator inaccuracies and room conditions.
   */
  float distanceCm=(pingTravelTime*(5.0/274.0));
  
  return distanceCm;
}

// ==========================================
// 3. ALERT SYSTEM: VISUAL & AUDIO FEEDBACK
// ==========================================
void triggerAlert(float distance){

  // Print current valid distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm ");
  
  // DANGER ZONE (0 to 20 cm): Stop immediately
  if (distance<=20){
    digitalWrite(redLED,HIGH);
    digitalWrite(yellowLED,LOW);
    digitalWrite(greenLED,LOW);

    digitalWrite(buzzPin,HIGH);
    digitalWrite(buzzPin,LOW);

    Serial.println("DANGER!! STOP!");
  }
  
  // CAUTION ZONE (20 to 50 cm): Slow down
  else if (distance>20 && distance<50){
    digitalWrite(redLED,LOW);
    digitalWrite(yellowLED,HIGH);
    digitalWrite(greenLED,LOW);

    digitalWrite(buzzPin,HIGH);
    delay(50);
    digitalWrite(buzzPin,LOW);
    Serial.println("Caution! Slow Down!");
  }
  
  // SAFE ZONE (> 50 cm): Clear path
  else {
    digitalWrite(redLED,LOW);
    digitalWrite(yellowLED,LOW);
    digitalWrite(greenLED,HIGH);
    
    digitalWrite(buzzPin,LOW);
    Serial.println("Safe! Clear Path Ahead.");
  }
}

// ==========================================
// 4. MAIN EXECUTION LOOP
// ==========================================
void loop() {
  float distance=getDistance();
  
  // Noise Filter: Only process data if it falls within valid physical boundaries (0cm - 400cm)
  // This ignores ghost echoes (0) and out-of-range anomalies.
  if(distance>0 && distance<400){
    triggerAlert(distance);
  }
  else{
    // Log ignored data for debugging without triggering alerts
    Serial.println("Ignored Noise...");
  }
  
  // Short delay to prevent sonic wave overlap between loops
  delay(100);
}
