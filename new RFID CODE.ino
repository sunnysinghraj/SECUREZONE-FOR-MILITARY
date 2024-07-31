#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
//#include<LiquidCrystal.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define IRsensor1 7
#define IRsensor2 6 

 Servo myservo;
 Servo myservo1;
 int pos=0;
// int pos=0;
 int ct1=0;
 int ct2=0;

//const int rs=8,en=7,d4=6,d5=5,d6=4,d7=3;
//LiquidCrystal lcd( rs,en,d4,d5,d6,d7);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.



 
void setup() 
{
  myservo.attach(5);
   myservo1.attach(3);
  pinMode(IRsensor1,INPUT);
  pinMode(IRsensor2,INPUT);
  pinMode(8,OUTPUT);
   pinMode(4,OUTPUT);
   pinMode(2,OUTPUT);
   myservo.write(0);
   //digitalWrite(2,LOW);
 // pinMode(7,1);
 // pinMode(6,1);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  //lcd.begin(16,2);
  //lcd.print("Group no 8");

}
void loop(){
  //analoglWrite(20);
  int sensorStatus1=digitalRead(IRsensor1);
  int sensorStatus2=digitalRead(IRsensor2);

 if(  sensorStatus1==0 &&  sensorStatus2==1){
    Serial.println("sensor1 motion  detected");
     Serial.println();
  ct1++;
   delay(1000);
 } 
   if(ct1>0){
      Serial.println("Approximate your card to the reader...");
      Serial.println();
      rfid();
      
      delay(1000);  
   }

   Serial.println(ct2);
      Serial.println();
  if(sensorStatus1==1 && sensorStatus2==0 && ct2==0 ){
    servoup1();
    Serial.println(ct2);
    //digitalWrite(2,HIGH);


  }
  digitalWrite(2,LOW);
  Serial.println(ct2);
   if(sensorStatus1==1 && sensorStatus2==0 && ct2>0 ){
   servoup();
   ct1=0;
   ct2=0;
   digitalWrite(4,HIGH);
  
   delay(1000);
    digitalWrite(4,LOW);
   servodown();
   
   delay(1000);
   
 }
}
//     if(sensorStatus2==0 && ct2==0){
//       servoup2();
//       delay(1000);
//       servodown2();
//       delay(1000);      
//     }

 

 

 //}



// void servoup2(){
//    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo2.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
// }
// void servodown2(){
//    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo2.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//  }
// // 
// }




void servoup(){
   for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}

void servoup1(){
   //for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    //myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    //delay(15);                       // waits 15 ms for the servo to reach the position
  //}
 //digitalWrite(2,HIGH);
  myservo1.write(1);
}
void servodown(){
   for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
 }
// 
}
void sensorread(){
  int sensorStatus1=digitalRead(IRsensor1);
  int sensorStatus2=digitalRead(IRsensor2);
}


















void rfid() 
{
  //lcd.setCursor(0, 0);
  //reset:
  //lcd.print("WELCOME TO ARMY");
  //lcd.setCursor(0, 1);
     //reset:

    // int sensorStatus=digitalRead(IRsensor2);
    // if(sensorStatus==1){
    //   lcd.print("Stop");
    // delay(2000);
    // lcd.setCursor(0, 1);
    //  //reset:
    // lcd.print("Show Your Card");
    // delay(2000);
    // }


    // lcd.print("Stop");
    // delay(2000);
    // lcd.setCursor(0, 1);
    //  //reset:
    // lcd.print("Show Your Card");
    // delay(2000);
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "6A 4E B1 1A"  ) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    digitalWrite(8,HIGH);
    ct2++;

   //lcd.setCursor(0, 1);
    // reset:
     // lcd.print("Authorizedacess");
    delay(2000);
   digitalWrite(8,LOW);
   delay(2000);
    digitalWrite(6,1);
    delay(200);
    digitalWrite(6,0);
    delay(200);
    digitalWrite(6,1);
    delay(200);
    digitalWrite(6,0);
    delay(200);
    digitalWrite(6,1);
    delay(200);
    digitalWrite(6,0);
    delay(1000);
  }
  else 
  {
    Serial.println("Access denied");
    Serial.println();
    //lcd.setCursor(0, 1);
     //reset:
    //lcd.print("Accessdenied");
    //lcd.setCursor(0, 1);
     //reset:
    //lcd.print("Sorry");
   // delay(2000);
   // lcd.setCursor(0, 1);
     //reset:
    //lcd.print("Wrong Card");
    //delay(2000);
    

    //delay(3000);
    digitalWrite(7,1);
    delay(200);
    digitalWrite(7,0);
    delay(200);
    digitalWrite(7,1);
    delay(200);
    digitalWrite(7,0);
    delay(200);
    digitalWrite(7,1);
    delay(200);
    digitalWrite(7,0);
  }
}
    