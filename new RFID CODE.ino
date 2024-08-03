#include <SPI.h>          // Include the SPI library for communication with the RFID module
#include <MFRC522.h>      // Include the MFRC522 library for RFID functions
#include <Servo.h>        // Include the Servo library for controlling servo motors

#define SS_PIN 10         // Define the Slave Select pin for the RFID module
#define RST_PIN 9         // Define the Reset pin for the RFID module
#define IRsensor1 7       // Define the pin for the first IR sensor
#define IRsensor2 6       // Define the pin for the second IR sensor

Servo myservo;           // Create a Servo object for the main servo
Servo myservo1;          // Create a Servo object for the auxiliary servo
int pos_main = 0;        // Initialize position variable for the main servo
int pos_aux = 0;         // Initialize position variable for the auxiliary servo
int ct1 = 0;             // Initialize counter for sensor1 detection
int ct2 = 0;             // Initialize counter for RFID authentication

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create an MFRC522 object with specified pins

void setup() {
  myservo.attach(5);     // Attach the main servo to pin 5
  myservo1.attach(3);    // Attach the auxiliary servo to pin 3
  pinMode(IRsensor1, INPUT); // Set IRsensor1 pin as input
  pinMode(IRsensor2, INPUT); // Set IRsensor2 pin as input
  pinMode(8, OUTPUT);    // Set pin 8 as output for the authorized access indicator
  pinMode(4, OUTPUT);    // Set pin 4 as a general purpose output
  pinMode(2, OUTPUT);    // Set pin 2 as an additional output
  myservo.write(0);      // Initialize the main servo position to 0 degrees
  myservo1.write(0);     // Initialize the auxiliary servo position to 0 degrees
  Serial.begin(9600);    // Start serial communication at 9600 baud rate
  SPI.begin();           // Initialize SPI bus
  mfrc522.PCD_Init();    // Initialize the MFRC522 module
  Serial.println("Approximate your card to the reader..."); // Print message to serial monitor
}

void loop() {
  int sensorStatus1 = digitalRead(IRsensor1); // Read the status of IRsensor1
  int sensorStatus2 = digitalRead(IRsensor2); // Read the status of IRsensor2

  if (sensorStatus1 == 0 && sensorStatus2 == 1) {
    Serial.println("sensor1 motion detected"); // Print message if sensor1 detects motion
    ct1++;              // Increment the counter for sensor1 detection
    delay(1000);        // Delay for 1 second
  }

  if (ct1 > 0) {
    Serial.println("Approximate your card to the reader..."); // Request for card scanning
    rfid();              // Call the rfid() function to check the card
    delay(1000);         // Delay for 1 second
  }

  if (sensorStatus1 == 1 && sensorStatus2 == 0 && ct2 == 0) {
    servoup1();         // Call the function to move the auxiliary servo
  }

  if (sensorStatus1 == 1 && sensorStatus2 == 0 && ct2 > 0) {
    resetServoToZero(); // Reset the auxiliary servo position to zero
    servoup();          // Call the function to move the main servo
    ct1 = 0;            // Reset the sensor1 counter
    ct2 = 0;            // Reset the RFID authentication counter
    digitalWrite(4, HIGH); // Turn on the general purpose indicator
    delay(1000);        // Delay for 1 second
    digitalWrite(4, LOW);  // Turn off the general purpose indicator
    servodown();        // Reset the main servo position
    delay(1000);        // Delay for 1 second
  }
}

void servoup() {
  for (pos_main = 0; pos_main <= 90; pos_main += 1) {
    myservo.write(pos_main); // Move the main servo to 90 degrees
    delay(15);           // Delay for smooth movement
  }
}

void servoup1() {
  for (pos_aux = 0; pos_aux <= 180; pos_aux += 1) {
    myservo1.write(pos_aux); // Move the auxiliary servo to 180 degrees
    delay(15);           // Delay for smooth movement
  }
}

void servodown() {
  for (pos_main = 90; pos_main >= 0; pos_main -= 1) {
    myservo.write(pos_main); // Move the main servo back to 0 degrees
    delay(15);           // Delay for smooth movement
  }
}

void resetServoToZero() {
  myservo1.write(0);     // Set the auxiliary servo position to 0 degrees
}

void rfid() {
  if (!mfrc522.PICC_IsNewCardPresent()) return; // Exit if no new card is present
  if (!mfrc522.PICC_ReadCardSerial()) return;   // Exit if the card UID cannot be read

  Serial.print("UID tag: "); // Print the "UID tag: " message
  String content = "";       // Initialize a string to store the card UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); // Print leading zero if needed
    Serial.print(mfrc522.uid.uidByte[i], HEX); // Print each byte of the UID in hexadecimal
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); // Add to the content string
    content.concat(String(mfrc522.uid.uidByte[i], HEX)); // Add each byte to the content string
  }
  Serial.println();         // Print a newline

  content.toUpperCase();    // Convert the content string to uppercase
  if (content.substring(1) == "6A 4E B1 1A") { // Check if the UID matches the authorized one
    Serial.println("Authorized access"); // Print authorized access message
    digitalWrite(8, HIGH); // Turn on the authorized access indicator
    ct2++;               // Increment the RFID authentication counter
    delay(2000);         // Delay for 2 seconds
    digitalWrite(8, LOW); // Turn off the authorized access indicator
  } else {
    Serial.println("Access denied"); // Print access denied message
    // Optional: Add actions for unauthorized access
  }
}
