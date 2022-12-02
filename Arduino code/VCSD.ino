#include <Servo.h>
#include <AFMotor.h>

#define Echo A0
#define Trig A1
#define motor 10
#define Speed 170
#define spoint 103
#define Echo2 A2
#define Trig2 A3
#define motor2 9

char value;
int distance;
int Left;
int Right;
int L = 0;
int R = 0;
int L1 = 0;
int R1 = 0;
int led= 10;
long duration, dist, average;   
long aver[3];

Servo servo;
Servo servo2;
AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);


void setup() {
  Serial.begin(9600);
  pinMode(Trig2, OUTPUT);  
  pinMode(Echo2, INPUT);
  servo2.attach(motor2);   
  servo2.write(0);         //close cap on power on
  delay(100);
  servo2.detach(); 
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  servo.attach(motor); 
  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);
}
void loop() {
  voicecontrol();
  Obstacle();
  Object();
}


void Obstacle() {
  distance = ultrasonic();
  if (distance <= 50) {
    Stop();
    backward();
    delay(100);
    Stop();
  }
}

void voicecontrol() {
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
    if (value == 'a') {
      forward();
      /*delay(500);
      Stop();*/
    } else if (value == 'c') {
      backward();
      delay(500);
      Stop();
    } else if (value == 'f') {
      L = leftsee();
      servo.write(spoint);
      if (L >= 10 ) {
        left();
        delay(500);
        Stop();
      } else if (L < 10) {
        Stop();
      }
    } else if (value == 'e') {
      R = rightsee();
      servo.write(spoint);
      if (R >= 10 ) {
        right();
        delay(500);
        Stop();
      } else if (R < 10) {
        Stop();
      }
    } else if (value == 'g') {
      Stop();
    }
  }
}

void Object(){
  for (int i=0;i<=2;i++) {   //average distance
    measure();               
   aver[i]=dist;            
    delay(10);              //delay between measurements
  }
 dist=(aver[0]+aver[1]+aver[2])/3;    

  if ( dist<20 ) {
 //Change distance as per your need
    servo2.attach(motor2);
    delay(1);
    servo2.write(0);  
    delay(3000);       
    servo2.write(150);    
    delay(1000);
    servo2.detach();      
  }
  Serial.print(dist);
}

void measure() {  
  digitalWrite(10,HIGH);
  digitalWrite(Trig2, LOW);
  delayMicroseconds(5);
  digitalWrite(Trig2, HIGH);
  delayMicroseconds(15);
  digitalWrite(Trig2, LOW);
  pinMode(Echo2, INPUT);
  duration = pulseIn(Echo2, HIGH);
  dist = (duration/2) / 29.1;    //obtain distance
}

int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t / 29 / 2; //time convert distance
  return cm;
}

void left() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}
void right() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}
void forward() {
  distance = ultrasonic();
  if (distance <= 50) {
    Stop();
    backward ();
  }
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
  distance = ultrasonic();
  if (distance <= 50) {
    Stop();
    backward ();
  }
}
void backward() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}
void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}
int rightsee() {
  servo.write(20);
  delay(800);
  Left = ultrasonic();
  return Left;
}
int leftsee() {
  servo.write(180);
  delay(800);
  Right = ultrasonic();
  return Right;
}
