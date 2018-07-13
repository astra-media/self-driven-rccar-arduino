#include <SPI.h>
#include <Pixy.h>
#include <Servo.h>


Pixy pixy; 

Servo myservo1;
Servo myservo2;

int left = 165;
int right = 15;

int drive = 110;
int neutral = 95;
int reverse = 80;

int far = 30;
int near = 45;

int SpeedD = far - 5;
int SpeedN = far + 5;
int SpeedR = near + 5;


void setup() {

  Serial.begin(9600);

  myservo1.attach(9);
  myservo2.attach(8);
  pixy.init();

}

//if object, drive towards the object. if no object then start searching for it.
void loop() {
  static int i = 0;

  if (pixyBlock() == true) {
    retrieve();
    //if block then frame = 0. 
    i = 0;
  }
  else if (pixyBlock() == false) {
    Speed(SpeedN);
    i++;
    //if there is no block, wait for 10000 frames, then send it to search mood
    if (i % 10000 == 0) {
      patrol();
    }
  }
}

// these function detects block, and returns the result
boolean pixyBlock() {

  uint16_t blocks;
  blocks = pixy.getBlocks();

  if (blocks) {
    return true;
  }
  else {
    return false;
  }
}

// this function steer and drive the car
void retrieve() {

  int i;
  int xValue = pixy.blocks[i].x;
  int distance = pixy.blocks[i].width;

  Serial.print("xPos:  ");
  Serial.print(xValue);
  Serial.print("  Distance:  ");
  Serial.println(distance);

  int steer = map(xValue, 0, 319, left, right);
  steering (steer);
  Speed(distance);
  delay(15);
}

//this function controls the search mood, that makes the car go around to look for object
void patrol() {

  Serial.println("patrol");

  steering(right);
  delay(1000);
  Speed(SpeedR);
  delay(500);
  Speed(SpeedN);
  delay(1000);
  steering(left);
  delay(1000);
  Speed(SpeedD);
  delay(500);
}

//it controls the drive mood: reverse, neutral and drive.
void Speed(int Value) {
  if (Value >= near) {
    motor (reverse);
  }
  else if (Value >= far && Value <= near) {
    motor (neutral);
  }
  else if (Value <= far) {
    motor (drive);
  }
}

//servo control to steer the wheel
void steering (int Value) {
  myservo1.write(Value);
}

//servo control to drive the car
void motor (int Value) {
  myservo2.write(Value);
}


