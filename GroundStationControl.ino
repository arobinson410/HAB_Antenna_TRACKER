#include <Servo.h>
#define INPUT_SIZE 10

Servo panServo;
Servo tiltServo;

int oldPanPos = 90;
int sensorPin = A0;
int sensorValue = 0;

void setup() {
  panServo.attach(9);
  tiltServo.attach(6);
  Serial.begin(9600);
  panServo.write(90);
}

void loop() {
  
  if(Serial.available() > 0){
    
    char input[INPUT_SIZE + 1];
    byte size = Serial.readBytes(input, INPUT_SIZE);
    input[size] = 0;
    
    char* separator = strchr(input, ':');

    if (separator != 0)
    {
        *separator = 0;
        int servoId = atoi(input);
        ++separator;
        int position = atoi(separator);
//Pan Servo
        if(servoId == 0){

          if(position > 180){
            position = 180;
          }

          if(position < -180){
            position = -180;
          }

          position = (position/4)+90;
          panServo.attach(9);      

          Serial.println(position);
          
          if(oldPanPos < position){
            while(oldPanPos != position){
              Serial.println(oldPanPos);
                ++oldPanPos;
                panServo.write(oldPanPos);
                delay(500);
            }
          }

          if(oldPanPos > position){
            while(oldPanPos != position){
              Serial.println(oldPanPos);
               --oldPanPos;
               panServo.write(oldPanPos);
               delay(500);
               
            }
          }
        }
        
//Tilt Mechanism
        if(servoId == 1){
          
          int zeroPos = 730;
          int ninetyPos = 175;
          
          int positionD = position;
          position = (zeroPos + (((zeroPos-ninetyPos)/(-90)) * position));
          
          sensorValue = analogRead(sensorPin);
          Serial.print("Potentiometer Value: ");
          Serial.println(sensorValue);

          Serial.println(positionD);
          
         if(sensorValue < position){
            while(sensorValue != position){
                sensorValue = analogRead(sensorPin);
                Serial.println(sensorValue);
                Serial.println(positionD);
                tiltServo.writeMicroseconds(1000);
            }
          }

          if(sensorValue > position){
            while(sensorValue != position){
                sensorValue = analogRead(sensorPin);
                Serial.println(sensorValue);
                Serial.println(position);
                tiltServo.writeMicroseconds(2000);
            }
          }

          tiltServo.writeMicroseconds(1500);
       }
    }
  }
}
