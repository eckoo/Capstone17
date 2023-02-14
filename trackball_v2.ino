#include <splash.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>


#define pinA 3
#define pinB 4
#define pinC 5
#define pinD 6

int initA;
int initB;
int initC;
int initD;

int newvalA;
int newvalB;
int newvalC;
int newvalD;

int recordA;
int recordB;
int recordC;
int recordD;
int delta;

long int x = 0;
long int y = 0;

String output_array[30];
int arrayCounter = 0;

int invert(int init) {
  int notvalue ;
  if (init == 0) {
    notvalue = 1; 
  }
  else {
    notvalue = 0;
  }
  return notvalue;
}


int compare(int init, int newval) {
  int record = 0;
  if (newval == init) {
  delta = 0;
  }
  else { 
    delta = 1 ;
    record = record + 1;
  }
  return record;
}


void setup () {
  Serial.begin(38400);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinC, INPUT);
  pinMode(pinD, INPUT);
  initA = digitalRead(pinA);
  initB = digitalRead(pinB);
  initC = digitalRead(pinC);
  initD = digitalRead(pinD);
}

void loop () { 
  newvalA = digitalRead(pinA);
  newvalB = digitalRead(pinB);
  newvalC = digitalRead(pinC);
  newvalD = digitalRead(pinD);

  recordA = compare(initA, newvalA);
  if (delta == 1) {
    initA = invert(initA);
  } 

  recordB = compare(initB, newvalB);
  if (delta == 1) {
    initB = invert(initB);
  } 

  recordC = compare(initC, newvalC);
  if (delta == 1) {
    initC = invert(initC);
  } 

  recordD = compare(initD, newvalD);
  if (delta == 1) {
    initD = invert(initD);
  } 
  
  x = x + recordA - recordC;
 
  y = y + recordB - recordD;

  //x_direction = abs(x - prevX);
  //outputToDisplay(display);

  //Serial.print("X = ");
  //Serial.print(x, DEC);
  //Serial.print(", Y = ");
  //Serial.print(y, DEC);

  if(arrayCounter >= 30) {
    arrayCounter = 0;
    for(int i = 0; i < 30; i++) {
      Serial.println(output_array[i]);
    }
    Serial.print("!");
    delay(5);
  String output = "X = " + String(x) + ", Y = " + String(y) + "\n";
  output_array[arrayCounter] = output;
  arrayCounter++;
  }
}