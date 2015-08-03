
/* ***********************************

1. Semester and Year    : 2013 SPRING
2. Course Number        : CS-492
3. Course Title         : ST: Kinect Programming
4. Work Number          : PR-06
5. Work Name            : Final Project Report
6. Work Version         : Version 1
7. Long Date            : Thursday, 09 May 2013
8. Author(s) Name(s)    : Anup Shrestha and Nevena Dragovic


Copyright (C) 2013 Anup Shrestha and Nevena Dragovic.
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:
•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
	in the documentation and/or other materials provided with the distribution.
•	Neither the name of the author nor the names of its contributors may be used to endorse or promote products derived from this 
	software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

************************************ */

// including the AFMotor library from adafruit
// this library comes with there motor shield
// which makes access to the motors connected
// to the motor shield

// the motor shield can run upto 4 motors and 
// are marked with numbers 1-4 

// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009

#include <AFMotor.h>


#include <Servo.h>

// connecting the the left motor of the 
// car on port 4 of the shield 
// and right motor to the port 1

AF_DCMotor leftMotor(4);
AF_DCMotor rightMotor(1);

Servo servo;

// ultrasonic sensor pins..
// assigning the trig pin of the sensor to digital pin 3 
// of the arduino and the echo pin to the digital pin 2

#define trigPin 3
#define echoPin 2

// global variables used for storing 
// the distance information from the sensor

int leftDistance, rightDistance;


// the initial setup for the arduino 

void setup()
{
  //starting the serial monitor at 9600 bauds
  Serial.begin(9600);
  
  // setting the servo on digital pin 9 
  // of the arduino
  servo.attach(9);
  
  // on start set the servo angle at 90 degrees
  servo.write(90);
 
  
  // setting the speed of the motors
  // since they were at an angle at start
  // we determined the speed by testing 
  // so the robot moved in a straight direction
  
  leftMotor.setSpeed(150);
  rightMotor.setSpeed(100);
  
  // stopping the motor at setup
  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);
}

// this is the function that returns the 
// distance of any object infront of the sensor
int getDistance()
{
  // variables declared for storing the 
  // sensor information 
  long duration, distance;
  
  // setting the pins for the ultrasonic
  // sensor (echo and trig pins)
  
  // the trigPin is set for OUTPUT
  // meaning it sends ultrasonic waves
  pinMode(trigPin, OUTPUT);
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(trigPin, LOW);
  
  // receives the ultrasonic sound waves
  // after being reflected off a surface
  
  pinMode(echoPin, INPUT);
  
  // this is how much time the ultrasonic waves took 
  // to get back to the sensor 
  duration = pulseIn(echoPin, HIGH);
  
  // since the duration it takes is double
  // to get to the object and return back 
  // we times it by half 
  // The speed of sound is 340 m/s
  // or 29 microseconds per centimeter.

  distance = (duration/2) / 29;
  
  // printing the distance of the object
  // to the serial monitor.
  Serial.println(distance);
  
  // since the sensor gives 0 when it can't
  // detect or the object is too far
  // so we just return an acceptable range value
  // so the car doesn't stop 
  if (distance <= 0 )
  {
    return 26;
  }
  
  // else we just return the distance 
  // measured by the sensor
  
  return distance; 
}


// the loop is continous
// as long as the code is running

void loop()
{
  // varibales to store the distance 
  // of the oobject
  // since the distance is not always 
  // accurate we double check the distance
  int distValue;
  int recheckValue;
  
  // this boolean variable decides 
  // whether the car should stop or
  // keep moving 
  boolean move = true;
  
  // the intial angle for the servo's position
  int i = 15;
  
  // rotating the servo to four 
  // different angles to check the objects
  // at those locations ..by increasing the
  // angle by 30 and back to 15 if it
  // is greater of equal to 150
  while(i <= 150)
  {
    
    // position the servo at angle i
    servo.write(i);
    delay(350);
    
    // get the distance from the sensor
    distValue = getDistance();
    recheckValue = getDistance();
    
    Serial.println(distValue);
    Serial.println(recheckValue);
    
    
    // checking the range of the distance
    // if the distance is less than 25 cm 
    // then stop the car
    if(distValue < 25 && recheckValue < 25)
    {
      // move false and break out of the loop
      // here as it finds some object 
      // less than the safe range it breaks 
      // out and than stops the car
      
      move = false;
      break;
    }
    
    // increment the servo angle
    i += 30;
  }
  
  //check if the car needs to keep moving
  // or stop
  if(move)
  {
    // moving both motors to the same direction
    leftMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
  }
  else
  {
    
    // stop the motors from runnig 
    leftMotor.run(RELEASE);
    rightMotor.run(RELEASE);
    
    // wait for 200 ms
    delay(200);
    
    // if the distance is less than 
    // 5 cm move back a little and check
    if(distValue <= 5)
    {
      Serial.println("Back");
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
      
      delay(300);
    }
    
    // the function checks both direction 
    // and decides where to turn
    checkDirection();
    
    // keep moving forward
    leftMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
  }

  
}


// the function moves the servo 
// to the left and then right 
// and also stores the distance 
// of the closet object to both 
// direction then calls the function
// that compares the distance

void checkDirection()
{
   Serial.println("Check Right");
    servo.write(0);
    delay(300);
   
    rightDistance = getDistance();
    delay(300);
    
    Serial.println("Check Left");
    servo.write(180);
    delay(300);
    
    leftDistance = getDistance();
    delay(300);
    
    //comparing the two distance
    compareDistance();
    
    // set the robot to move forward
    leftMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
}


// function to decide where to turn 
// if the object distance to the left 
// is less than object distance to the 
// right than turn right else turn left
void compareDistance()
{
  if (leftDistance >= rightDistance) //if left is less obstructed 
  {
    Serial.println("Turning Left");
    leftMotor.run(FORWARD); 
    rightMotor.run(BACKWARD); //turn left
    
    // the delay decides how much to turn 
    // by experimentation we found out that 
    // 800 ms is a good delay for the car to 
    // turn 90 degrees
    
    delay(800); 
  }
  else if (rightDistance > leftDistance) //if right is less obstructed
  {
    Serial.println("Turning Right");
    leftMotor.run(BACKWARD); 
    rightMotor.run(FORWARD); //turn right
    
    delay(800);
  }
  else
  {
    
    // if they get the same distance 
    // both sides then turn 180 degress
    Serial.println("Turning Back");
    leftMotor.run(BACKWARD); 
    rightMotor.run(FORWARD); //turn right
    
    
    // double the delay used for 90 degrees turn
    delay(1500);
  }
 
}
