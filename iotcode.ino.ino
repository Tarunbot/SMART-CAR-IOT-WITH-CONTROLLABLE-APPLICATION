//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
#include <Arduino.h>

#define LED 2
int motor1Pin1 = 27;    //inA
int motor1Pin2 = 26;    //inB
int enable1Pin = 14;   //enableA

int motor2Pin1 = 18;    //inb
int motor2Pin2 = 19;    //inB
int enable2Pin = 21;   //enableA


int trigPin=32;
int echoPin=33;
// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 1;
int dutyCycle = 80;

String action="";

long duration;
float distanceCm;
float distanceInch;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
int LED_BUILTIN=2;
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(trigPin,OUTPUT);
   pinMode(echoPin,INPUT);
 pinMode(LED_BUILTIN, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel);


  // testing
  Serial.print("Testing DC Motor...");
}

void loop() {
   while (SerialBT.available()) {
      char incomingChar = SerialBT.read();
      if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
      else action += String(incomingChar);
    }
    Serial.print(action);
  
  
  if(action.equals("s")){
    Serial.print("started");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 
  }


  if(action=="br"){
    Serial.print("stopped");
      digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, LOW); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, LOW); 
  }

  if(action.equals("back")){
     Serial.print("stopped");
      digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, HIGH); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, HIGH); 
  }

if(action.equals("l")){
     Serial.print("turning left");
      digitalWrite(motor1Pin1, HIGH);
     digitalWrite(motor1Pin2, LOW); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, HIGH); 
  }

  if(action.equals("r")){
     Serial.print("turning right");
     digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, HIGH); 
     digitalWrite(motor2Pin1, HIGH);
     digitalWrite(motor2Pin2,LOW); 
  }


  while(action.equals("automatic")){
      dutyCycle=80;
      digitalWrite(trigPin,LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin,LOW);
      duration=pulseIn(echoPin,HIGH);
      distanceCm=duration*SOUND_SPEED/2;
      distanceInch=distanceCm*CM_TO_INCH;

    
         while(distanceInch>=5){
         digitalWrite(motor1Pin1, HIGH);
         digitalWrite(motor1Pin2, LOW); 
         digitalWrite(motor2Pin1, HIGH);
         digitalWrite(motor2Pin2, LOW); 
          while (SerialBT.available()) {
             char incomingChar = SerialBT.read();
             if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
             else action += String(incomingChar);
          }

          if(action.equals("manual"))break;
        
         digitalWrite(trigPin,LOW);
       delayMicroseconds(2);
       digitalWrite(trigPin,HIGH);
       delayMicroseconds(10);
       digitalWrite(trigPin,LOW);
       duration=pulseIn(echoPin,HIGH);
       distanceCm=duration*SOUND_SPEED/2;
       distanceInch=distanceCm*CM_TO_INCH;
      }
      
      while(distanceInch<8){
        if(action.equals("manual"))break;
         digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH); 
        delay(500);
         digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH); 
         digitalWrite(trigPin,LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin,LOW);
        duration=pulseIn(echoPin,HIGH);
        distanceCm=duration*SOUND_SPEED/2;
        distanceInch=distanceCm*CM_TO_INCH;
        delay(200);
         digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW); 
      }

      while (SerialBT.available()) {
             char incomingChar = SerialBT.read();
             if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
             else action += String(incomingChar);
      }


}

  if(action.equals("manual")){
    action="br";
    dutyCycle=180;
     digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW); 

  }
  
  action="";
  while (SerialBT.available()) {
      char incomingChar = SerialBT.read();
      if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
      else action += String(incomingChar);
  }
  Serial.print(action);
  

  if(action.equals("s")){
    Serial.print("started");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 
  }


  if(action=="br"){
    Serial.print("stopped");
      digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, LOW); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, LOW); 
  }

  if(action.equals("back")){
     Serial.print("stopped");
      digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, HIGH); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, HIGH); 
  }

if(action.equals("l")){
     Serial.print("turning left");
      digitalWrite(motor1Pin1, HIGH);
     digitalWrite(motor1Pin2, LOW); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, HIGH); 
  }

  if(action.equals("r")){
     Serial.print("turning right");
     digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, HIGH); 
     digitalWrite(motor2Pin1, HIGH);
     digitalWrite(motor2Pin2,LOW); 
  }
while(action.equals("automatic")){
      dutyCycle=80;
      digitalWrite(trigPin,LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin,LOW);
      duration=pulseIn(echoPin,HIGH);
      distanceCm=duration*SOUND_SPEED/2;
      distanceInch=distanceCm*CM_TO_INCH;

    
         while(distanceInch>=5){
         digitalWrite(motor1Pin1, HIGH);
         digitalWrite(motor1Pin2, LOW); 
         digitalWrite(motor2Pin1, HIGH);
         digitalWrite(motor2Pin2, LOW); 
          while (SerialBT.available()) {
             char incomingChar = SerialBT.read();
             if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
             else action += String(incomingChar);
          }

          if(action.equals("manual"))break;
        
         digitalWrite(trigPin,LOW);
       delayMicroseconds(2);
       digitalWrite(trigPin,HIGH);
       delayMicroseconds(10);
       digitalWrite(trigPin,LOW);
       duration=pulseIn(echoPin,HIGH);
       distanceCm=duration*SOUND_SPEED/2;
       distanceInch=distanceCm*CM_TO_INCH;
      }
      
      while(distanceInch<8){
        if(action.equals("manual"))break;
         digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH); 
        delay(500);
         digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH); 
         digitalWrite(trigPin,LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin,LOW);
        duration=pulseIn(echoPin,HIGH);
        distanceCm=duration*SOUND_SPEED/2;
        distanceInch=distanceCm*CM_TO_INCH;
        delay(200);
         digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW); 
      }

      while (SerialBT.available()) {
             char incomingChar = SerialBT.read();
             if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
             else action += String(incomingChar);
      }


}

  if(action.equals("manual")){
    action="br";
    dutyCycle=180;
     digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW); 

  }

  if(action.equals("manual")){
    action="br";
  }
  
  action="";
  delay(20);
}