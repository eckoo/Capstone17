#include <Arduino.h>

//Pins

int upPin = 36;
int downPin = 39;
int leftPin = 34;
int rightPin = 35;


void setup() {
  Serial.begin(9600);
  pinMode(23, OUTPUT);
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
}

void loop() {                           //Trackball works by pulsing pins when moving, so when a pin != previous val, it moved
  int x , y = 0;
  int up, down, left, right = 0;

  if(up != digitalRead(upPin)){
    y++;
    up = digitalRead(upPin);
  }
  
  if(down != digitalRead(downPin)){
    y--;
    down = digitalRead(downPin);
  }
  
  if(left != digitalRead(leftPin)){
    x--;
    left = digitalRead(leftPin);
  }

  if(right != digitalRead(rightPin)){
    x++;
    right = digitalRead(rightPin);
  }

  Serial.println("X = " + x);
  Serial.println("Y = " + y);

  delay(100);                           //Adjusts speed

}