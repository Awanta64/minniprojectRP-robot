#include <Servo.h>
#include <Encoder.h>
#include <Wire.h>
#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Empty.h>


Servo servopotentiometer;
Servo servoencoder;

///encider
const int PINClk =2;
const int PINDT =3;

const int SW_PIN = 4;
const int homePosition = 0; 
const int stepValue = 5;
const int servo_encoder = 9;
int servoAngel = homePosition;

///poten
int val = 0;
int servo_potentiometer = A3;
int potentiometerPin = A0;
//int angle = 0;


Encoder encoder(PINClk, PINDT);

ros::NodeHandle nh;

std_msgs::Int16 servo_msg;
ros::Publisher servo_pub("servo_angle", &servo_msg);

// poten ส่งองศา
std_msgs::Float64 poten_msg;
ros::Publisher poten_pub("poten_angle", &poten_msg);


void setup() {
    pinMode(SW_PIN,INPUT_PULLUP);
//  Serial.println("Robojax Encoder with Servo");
  servoencoder.attach(servo_encoder);  

    //potentiometer
  servopotentiometer.attach(servo_potentiometer);
  pinMode(servo_potentiometer,OUTPUT);
  pinMode(potentiometerPin,INPUT);

  nh.initNode();
  nh.advertise(servo_pub);
  nh.advertise(poten_pub);
  
}


long oldPosition  = -999;


void loop() {
  //Potentiometer 
    val = analogRead(potentiometerPin);
    float angle = map(val, 0, 1023, 0, 180);
//    servopotentiometer.write(angle);
    poten_msg.data = angle;
    poten_pub.publish(&poten_msg);
    nh.spinOnce();


  
  //Encoder
  long newPosition = encoder.read();
 
  if(newPosition != oldPosition){

    if(newPosition >  oldPosition)
    {
    int newStep = abs(newPosition - oldPosition);     
      servoAngel -= stepValue;
      if(servoAngel < 0){
          servoAngel =0;    
      }  
      servo_msg.data = servoAngel;
      servo_pub.publish(&servo_msg);
      nh.spinOnce();


    }
    if(newPosition <  oldPosition )
    {
    int newStep = abs(newPosition - oldPosition);      
      servoAngel += stepValue;
      if(servoAngel > 180)
          servoAngel =180;
      servo_msg.data = servoAngel;
      servo_pub.publish(&servo_msg);
      nh.spinOnce();  

    }
   oldPosition = newPosition;
  }

  nh.spinOnce();
  delay(10);
  
}
