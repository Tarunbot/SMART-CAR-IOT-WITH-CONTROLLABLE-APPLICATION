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
int state=0;
int force=0;
int right=0;
int turns=0;
int motor1Pin1 = 27;    //inA
int motor1Pin2 = 26;    //inB
int enable1Pin = 14;   //enableA
float wrot=0;
int sep=99999;
int motor2Pin1 = 18;    //inb
int motor2Pin2 = 12;    //inB
int enable2Pin = 21;   //enableA
int sec=1.3;

int trigPin=32;
int echoPin=33;
// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 1;
int dutyCycle = 1000;

String action="";
String prev="";
long duration;
float distanceCm;
float distanceInch;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
int LED_BUILTIN=2;
void setup() {
  Serial.begin(115200);
  SerialBT.begin("Binod"); //Bluetooth device name
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
      else {action += String(incomingChar);
            prev=action;
      }
    }
    Serial.print(action);
  
  
  if(action.equals("W")){
    Serial.print("started");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    action="";

  }

  if(action.equals("info")){
      digitalWrite(trigPin,LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin,LOW);
      duration=pulseIn(echoPin,HIGH);
      distanceCm=duration*SOUND_SPEED/2;
      distanceInch=distanceCm*CM_TO_INCH;
     SerialBT.println(String(distanceInch) + "," + String(wrot)+",");
     wrot=0.0;
    
  }


  if(action=="x"){
    Serial.print("stopped");
      digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, LOW); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, LOW); 
   
  }

  if(action.equals("S")){
     Serial.print("stopped");
      digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, HIGH); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, HIGH); 
  }

if(action.equals("A")){
     Serial.print("turning left");
      digitalWrite(motor1Pin1, HIGH);
     digitalWrite(motor1Pin2, LOW); 
   digitalWrite(motor2Pin1, LOW);
   digitalWrite(motor2Pin2, HIGH); 
  }

  if(action.equals("D")){
     Serial.print("turning right");
     digitalWrite(motor1Pin1, LOW);
     digitalWrite(motor1Pin2, HIGH); 
     digitalWrite(motor2Pin1, HIGH);
     digitalWrite(motor2Pin2,LOW); 
  }

 
  while(action.equals("F")){
      double leftdis=0;
      double rightdis=0;
      
      dutyCycle=80;
      digitalWrite(trigPin,LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin,LOW);
      duration=pulseIn(echoPin,HIGH);
      distanceCm=duration*SOUND_SPEED/2;
      distanceInch=distanceCm*CM_TO_INCH;

      
      
      
         while(distanceInch>=6){
          delay(10);
          wrot=wrot+(0.0013*float(22));
          digitalWrite(motor1Pin1, HIGH);
          digitalWrite(motor1Pin2, LOW); 
          digitalWrite(motor2Pin1, HIGH);
          digitalWrite(motor2Pin2, LOW); 
          while (SerialBT.available()) {
             char incomingChar = SerialBT.read();
             if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
             else action += String(incomingChar);
          }

          if(action.equals("X"))break;
        
        digitalWrite(trigPin,LOW);
       delayMicroseconds(2);
       digitalWrite(trigPin,HIGH);
       delayMicroseconds(10);
       digitalWrite(trigPin,LOW);
       duration=pulseIn(echoPin,HIGH);
       distanceCm=duration*SOUND_SPEED/2;
       distanceInch=distanceCm*CM_TO_INCH;
      }
      
      while(distanceInch<6){

        
        
        digitalWrite(trigPin,LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin,LOW);
        duration=pulseIn(echoPin,HIGH);
        distanceCm=duration*SOUND_SPEED/2;
        distanceInch=distanceCm*CM_TO_INCH;

        SerialBT.println(String(distanceInch) + "," + String(wrot)+","+String("nil")+","+String(turns));
        
        if(action.equals("X"))break;


          if(right==1){
             SerialBT.println(String(distanceInch) + "," + String(wrot)+","+String("right")+","+String(turns));
             wrot=0.0;
             turns=turns+1;
             digitalWrite(motor1Pin1, LOW);
              digitalWrite(motor1Pin2, HIGH); 
              digitalWrite(motor2Pin1, HIGH);
              digitalWrite(motor2Pin2,LOW); 
              delay(700);

             digitalWrite(trigPin,LOW);
             delayMicroseconds(2);
             digitalWrite(trigPin,HIGH);
             delayMicroseconds(10);
             digitalWrite(trigPin,LOW);
             duration=pulseIn(echoPin,HIGH);
             distanceCm=duration*SOUND_SPEED/2;
             rightdis=distanceCm*CM_TO_INCH;

             if(rightdis<6){
                  digitalWrite(motor1Pin1, HIGH);
                  digitalWrite(motor1Pin2, LOW); 
                  digitalWrite(motor2Pin1, LOW);
                  digitalWrite(motor2Pin2, HIGH);
                  delay(1400);
                  digitalWrite(trigPin,LOW);
                  delayMicroseconds(2);
                  digitalWrite(trigPin,HIGH);
                  delayMicroseconds(10);
                  digitalWrite(trigPin,LOW);
                  duration=pulseIn(echoPin,HIGH);
                  distanceCm=duration*SOUND_SPEED/2;
                  leftdis=distanceCm*CM_TO_INCH;

                  if(leftdis<6){
                     digitalWrite(motor1Pin1, LOW);
                       digitalWrite(motor1Pin2, HIGH); 
                       digitalWrite(motor2Pin1, LOW);
                       digitalWrite(motor2Pin2, HIGH);
                       delay(100);
                    while(leftdis<6){
                       digitalWrite(motor1Pin1, HIGH);
                       digitalWrite(motor1Pin2, LOW); 
                       digitalWrite(motor2Pin1, LOW);
                       digitalWrite(motor2Pin2, HIGH);
                       delay(350);
                       digitalWrite(trigPin,LOW);
                        delayMicroseconds(2);
                        digitalWrite(trigPin,HIGH);
                        delayMicroseconds(10);
                        digitalWrite(trigPin,LOW);
                        duration=pulseIn(echoPin,HIGH);
                        distanceCm=duration*SOUND_SPEED/2;
                        leftdis=distanceCm*CM_TO_INCH;

                    }
                  }

             }
              right=0;
             
            
          }else{
            SerialBT.println(String(distanceInch) + "," + String(wrot)+","+String("left")+","+String(turns));
            turns=turns+1;
            wrot=0.0;

             digitalWrite(motor1Pin1, HIGH);
             digitalWrite(motor1Pin2, LOW); 
             digitalWrite(motor2Pin1, LOW);
             digitalWrite(motor2Pin2,HIGH); 
             delay(700);
            digitalWrite(trigPin,LOW);
            delayMicroseconds(2);
            digitalWrite(trigPin,HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin,LOW);
            duration=pulseIn(echoPin,HIGH);
            distanceCm=duration*SOUND_SPEED/2;
            leftdis=distanceCm*CM_TO_INCH;

            if(leftdis<6){
                 digitalWrite(motor1Pin1, LOW);
                  digitalWrite(motor1Pin2, HIGH); 
                  digitalWrite(motor2Pin1, HIGH);
                  digitalWrite(motor2Pin2, LOW);
                  delay(1800);
                  digitalWrite(trigPin,LOW);
                  delayMicroseconds(2);
                  digitalWrite(trigPin,HIGH);
                  delayMicroseconds(10);
                  digitalWrite(trigPin,LOW);
                  duration=pulseIn(echoPin,HIGH);
                  distanceCm=duration*SOUND_SPEED/2;
                  rightdis=distanceCm*CM_TO_INCH;


                  if(rightdis<6){
                    digitalWrite(motor1Pin1, LOW);
                       digitalWrite(motor1Pin2, HIGH); 
                       digitalWrite(motor2Pin1, LOW);
                       digitalWrite(motor2Pin2, HIGH);
                       delay(100);
                    while(rightdis<6){
                       digitalWrite(motor1Pin1, LOW);
                       digitalWrite(motor1Pin2, HIGH); 
                       digitalWrite(motor2Pin1, HIGH);
                       digitalWrite(motor2Pin2, LOW);
                       delay(250);
                       digitalWrite(trigPin,LOW);
                        delayMicroseconds(2);
                        digitalWrite(trigPin,HIGH);
                        delayMicroseconds(10);
                        digitalWrite(trigPin,LOW);
                        duration=pulseIn(echoPin,HIGH);
                        distanceCm=duration*SOUND_SPEED/2;
                        rightdis=distanceCm*CM_TO_INCH;

                    }
                  }
            }

            SerialBT.println(String(distanceInch) + "," + String(wrot)+",nil"+","+String(turns));
            right=1;
            
          }
         wrot=0.0;
         digitalWrite(trigPin,LOW);
         
          
          digitalWrite(motor1Pin1, LOW);
          digitalWrite(motor1Pin2, LOW); 
          digitalWrite(motor2Pin1, LOW);
          digitalWrite(motor2Pin2, LOW);
         digitalWrite(trigPin,HIGH);
         delay(100);
         digitalWrite(trigPin,LOW);
         
         duration=pulseIn(echoPin,HIGH);
         delay(200);
         distanceCm=duration*SOUND_SPEED/2;
         distanceInch=distanceCm*CM_TO_INCH;
         delay(200);
         while (SerialBT.available()) {
             char incomingChar = SerialBT.read();
             if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
             else action += String(incomingChar);
          }
       
      }
      
      
      while (SerialBT.available()) {
             char incomingChar = SerialBT.read();
             if (incomingChar == '\r' || incomingChar == '\n')SerialBT.read();
             else action += String(incomingChar);
      }
      if(action.equals("X"))break;


}

  if(action.equals("X")){
    action="br";
    prev="br";
    dutyCycle=1000;
     digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW); 
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW); 

  }
  if(prev.equals("s") || prev.equals("l") || prev.equals("r") || prev.equals("back")){
    wrot=wrot+(0.0013*float(20));
  }
  action="";
  
  delay(20);
}