#include "ArduPID.h"
#include <TimerOne.h>

//TIMER
const int interval = 10000; // 1 millisecond interval
int x=0;

//PID
ArduPID myController;
ArduPID myController2;

double setpoint = 400;
volatile double input;
volatile double output;
volatile double input2;
volatile double output2;

double p = 0.1;
double i = 0.005;
double d = 0;

double p2 = 10;
double i2 = 0;
double d2 = 0;


//ENCODER
volatile  int temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder 

//MOTOR
#define IN_1  4
#define IN_2  5
#define EN    6 


void setup() {
  Serial.begin(9600);
  //PID
  myController.begin(&input, &output, &setpoint, p, i, d);
  myController2.begin(&input, &output2, &setpoint, p2, i2, d2);


  // myController.reverse()               // Uncomment if controller output is "reversed"
  // myController.setSampleTime(10);      // OPTIONAL - will ensure at least 10ms have past between successful compute() calls
  myController.setOutputLimits(-255, 255);
  myController.setBias(0);
  myController.setWindUpLimits(-255, 255); // Groth bounds for the integral term to prevent integral wind-up
  myController.start();

  myController2.setOutputLimits(-255, 255);
  myController2.setBias(0);
  myController2.setWindUpLimits(-255, 255); // Groth bounds for the integral term to prevent integral wind-up
  myController2.start();
 
  // myController.reset();               // Used for resetting the I and D terms - only use this if you know what you're doing
  // myController.stop();                // Turn off the PID controller (compute() will not do anything until start() is called
  
  //TIMER
  Timer1.initialize(interval);
  Timer1.attachInterrupt(timerIsr); // Attach the ISR function

  //ENCODER
  pinMode(2, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(3, INPUT_PULLUP); // internalเป็น pullup input pin 3
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);
  
  //MOTOR
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(EN, OUTPUT); 
}

void motor_stop(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
}

void motor_drive(int output){
  if (output>0){
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
  }else{
    digitalWrite(IN_2, HIGH);
    digitalWrite(IN_1, LOW);
  }
  analogWrite(EN,abs(output));

}

void loop()
{ 
  if(true){
      
      
      //Serial.print((float)counter/800*360);
      //analogWrite(9, counter)
      //Serial.print(",");
      //Serial.print(output);
      //Serial.print(",");
      //Serial.print(setpoint);
      //Serial.print(",");
      int to_p = (int)setpoint-counter ;
      int tp_pp = map(to_p, 0 , 800 , 0, 255);
      Serial.println(tp_pp);
      delay(200);
    }

}




void timerIsr() {
  //x++;
  //if (x%10==0){
    input = counter; // Replace with sensor feedback
    myController.compute();
    myController2.compute();
    if(counter>300 && counter<500)
      motor_drive((int)output2);
    else
      motor_drive((int)output);
    
    //analogWrite(3, output); // Replace with plant control signal
  //}
}

void ai0() {
// ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
// Check pin 3 to determine the direction
if(digitalRead(3)==LOW) {
counter++;
}else{
counter--;
}
if(counter >=800)
        counter=0;
      if(counter <0)
        counter=800;
}
  
void ai1() {
// ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
// Check with pin 2 to determine the direction
if(digitalRead(2)==LOW) {
counter--;
}else{
counter++;
}
if(counter >=800)
        counter=0;
      if(counter <0)
        counter=800;
}
