#define BLYNK_TEMPLATE_ID "TMPL3rsXftSle"
#define BLYNK_TEMPLATE_NAME "Blind Stick"
#define BLYNK_AUTH_TOKEN "B_pQM-WZANFIvL8xFk2UkiCQqGa_Sjh2"

#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const int MPU_addr = 0x68; // I2C address of MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;
boolean fall=false;
boolean trigger1=false, trigger2=false, trigger3=false;
byte trigger1count=0, trigger2count=0, trigger3count=0;
int angleChange=0;

// WiFi info
char auth[] = BLYNK_AUTH_TOKEN;
const char *ssid = "Sudo"; 
const char *pass = "astronaut";

// Ultrasonic sensor pins
const int trigPin=12, echoPin=14, buzzerPin=13;

// Buzzer timing
bool buzzerState=false;
unsigned long buzzerStartTime=0;
const unsigned long buzzerDuration=1000; // 1 second

#define SOUND_VELOCITY 0.034
long duration;
float distanceCm;

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // wake up MPU-6050
  Wire.endTransmission(true);

  Serial.println("MPU initialized");

  Blynk.begin(auth, ssid, pass);
  Serial.println("Connecting to WiFi...");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

// ---------------- Loop ----------------
void loop() {
  Blynk.run();

  // --- Ultrasonic Sensor ---
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_VELOCITY / 2;

  // Obstacle buzzer (non-blocking)
  if(distanceCm>0 && distanceCm<50 && !buzzerState){
    digitalWrite(buzzerPin,HIGH);
    buzzerState=true;
    buzzerStartTime=millis();
  }
  if(buzzerState && (millis()-buzzerStartTime >= buzzerDuration)){
    digitalWrite(buzzerPin,LOW);
    buzzerState=false;
  }

  // --- MPU Reading ---
  mpu_read();
  ax = (AcX-2050)/16384.0;
  ay = (AcY-77)/16384.0;
  az = (AcZ-1947)/16384.0;
  gx = (GyX+270)/131.0;
  gy = (GyY-351)/131.0;
  gz = (GyZ+136)/131.0;

  float Raw_Amp = sqrt(ax*ax + ay*ay + az*az);
  int Amp = Raw_Amp * 10;
  angleChange = sqrt(gx*gx + gy*gy + gz*gz);

  // --- Print for Debug ---
  Serial.print("Amp: "); Serial.print(Amp);
  Serial.print(" | angleChange: "); Serial.print(angleChange);
  Serial.print(" | distanceCm: "); Serial.print(distanceCm);
  Serial.print(" | trigger1: "); Serial.print(trigger1);
  Serial.print(" trigger2: "); Serial.print(trigger2);
  Serial.print(" trigger3: "); Serial.println(trigger3);

  // --- Fall Detection Logic ---
  if(Amp <= 3 && !trigger2){
    trigger1=true;
  }

  if(trigger1){
    trigger1count++;
    if(Amp >= 10){
      trigger2=true;
      trigger1=false; trigger1count=0;
    }
  }

  if(trigger2){
    trigger2count++;
    if(angleChange >= 15 && angleChange <= 400){  // only trigger3 if stick tilts significantly
      trigger3=true; trigger2=false; trigger2count=0;
    }
  }

  if(trigger3){
    trigger3count++;
    if(trigger3count >= 5){ // ~0.5s
      if(angleChange >=0 && angleChange <= 10){  // stick landed flat
        fall=true;
        trigger3=false; trigger3count=0;
      } else {
        trigger3=false; trigger3count=0;
      }
    }
  }

  if(fall){
    Serial.println("FALL DETECTED!");          // <- important print
    Blynk.logEvent("fall_detected");           // Blynk event
    triggerBuzzer();                            // extra buzzer for fall
    fall=false;
  }

  // Reset triggers if timeout
  if(trigger1count>=10){ trigger1=false; trigger1count=0; }
  if(trigger2count>=10){ trigger2=false; trigger2count=0; }

  // Send data to Blynk
  Blynk.virtualWrite(V0,distanceCm);
  Blynk.virtualWrite(V1,ax);
  Blynk.virtualWrite(V2,ay);
  Blynk.virtualWrite(V3,az);

  delay(100);
}

// ---------------- MPU Read ----------------
void mpu_read(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);

  AcX = Wire.read()<<8 | Wire.read();
  AcY = Wire.read()<<8 | Wire.read();
  AcZ = Wire.read()<<8 | Wire.read();
  Tmp = Wire.read()<<8 | Wire.read();
  GyX = Wire.read()<<8 | Wire.read();
  GyY = Wire.read()<<8 | Wire.read();
  GyZ = Wire.read()<<8 | Wire.read();
}

// ---------------- Buzzer Function ----------------
void triggerBuzzer(){
  digitalWrite(buzzerPin,HIGH);
  delay(2000);
  digitalWrite(buzzerPin,LOW);
}
