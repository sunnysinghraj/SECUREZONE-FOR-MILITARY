#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define IRsensor1 7
#define IRsensor2 6

Servo myservo;   // Main servo for gate control
Servo myservo1;  // Auxiliary servo or additional control
int pos_main = 0; // Position for main servo
int pos_aux = 0;  // Position for auxiliary servo
int ct1 = 0;      // Counter for sensor1
int ct2 = 0;      // Counter for RFID authentication

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() {
  myservo.attach(5);
  myservo1.attach(3);
  pinMode(IRsensor1, INPUT);
  pinMode(IRsensor2, INPUT);
  pinMode(8, OUTPUT); // LED or signal for authorized access
  pinMode(4, OUTPUT); // General purpose output
  pinMode(2, OUTPUT); // Additional output
  myservo.write(0);
  myservo1.write(0);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
}

void loop() {
  int sensorStatus1 = digitalRead(IRsensor1);
  int sensorStatus2 = digitalRead(IRsensor2);

  if (sensorStatus1 == 0 && sensorStatus2 == 1) {
    Serial.println("sensor1 motion detected");
    ct1++;
    delay(1000);
  }

  if (ct1 > 0) {
    Serial.println("Approximate your card to the reader...");
    rfid();
    delay(1000);
  }

  if (sensorStatus1 == 1 && sensorStatus2 == 0 && ct2 == 0) {
    servoup1(); // Move myservo1
  }

  if (sensorStatus1 == 1 && sensorStatus2 == 0 && ct2 > 0) {
    resetServoToZero();
    servoup();  // Move myservo
    ct1 = 0;
    ct2 = 0;
    digitalWrite(4, HIGH); // Turn on indicator
    delay(1000);
    digitalWrite(4, LOW);  // Turn off indicator
    servodown();           // Reset myservo
    delay(1000);
  }
}

void servoup() {
  for (pos_main = 0; pos_main <= 90; pos_main += 1) {
    myservo.write(pos_main);
    delay(15);
  }
}

void servoup1() {
  for (pos_aux = 0; pos_aux <= 180; pos_aux += 1) {
    myservo1.write(pos_aux);
    delay(15);
  }
}

void servodown() {
  for (pos_main = 90; pos_main >= 0; pos_main -= 1) {
    myservo.write(pos_main);
    delay(15);
  }
}

void resetServoToZero() {
  myservo1.write(0);  // Set auxiliary servo to 0 degrees
}

void rfid() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.print("UID tag: ");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();

  content.toUpperCase();
  if (content.substring(1) == "6A 4E B1 1A") { // Change this UID to match the authorized card
    Serial.println("Authorized access");
    digitalWrite(8, HIGH);
    ct2++;
    delay(2000);
    digitalWrite(8, LOW);
  } else {
    Serial.println("Access denied");
    // Optional: Add actions for unauthorized access
  }
}
