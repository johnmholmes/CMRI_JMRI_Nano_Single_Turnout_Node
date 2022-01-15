/*
  The sketch is used in conjunction with JMRI to control a Arduino Nano.
  
  It simulates a CMRI Smini device which can have 24 input lines and 48 output lines.
  In this case we will not use that many.

  The Node will have an address of 3

  We will have 1 sg90 servo to move the turnout attached to pin 3. JMRI address 3001.

  We will have 3 signal heads each will have 2 leds 1 Green and 1 Red. i use comman anode
  leds signals made by TMC so the anode goes to 5v and the negatives go to indivual pins
  on the Nano. 

  Pins 0 - 1 are used for Rx and Tx no trigger pin required due to using a TTL to RS485 Module 
  Serial Port MCU Automatic Flow Control Module.
  
  The Node can be used as is with a usb cable 

  Or as is with a usb to RS485 bus adapter connected to pins 1 and 2.

  Or you can use a cheap RS485 adapter which uses 3 wires from the module. In this case you will
  need to un comment out a few lines as marked in the sketch.

  
  This code use Chris Sharp a MERG member who slow motion servo code and help with servo disconnection
  
  This is the final version i will be doing of this node. for demonstration purposes only
  
  feel free to download and change the code for your use. see MIT licence for details
*/
#include <CMRI.h>  //include the cmri library you will need to download this to you libaray folder
#include <Servo.h> // include the servo library
//#include <Auto485.h>   //uncomment this if you want to use a 3 wire rs485 module
//#define DE_PIN 2       //uncomment this if you want to use a 3 wire rs485 module

#define CMRI_ADDR 3  //sets the address of the node this must be different for each node on your system
#define turnout1ClosedPosition 65 // previous 70
#define turnout1ThrownPosition 110  // previous 108

// define signal leds turnout 1
#define throughApproachGreenLed 4 //jmri 3002
#define throughApproachRedLed 5 //3003
#define divergingApproachGreenLed 6 //3004
#define divergingApproachRedLed 7 //3005
#define throughGreenLed 8 //3006
#define throughRedLed 9 //3007

//define infrared detection address 3001 is used to send turnout state
#define Sensor1 A0 //3002
#define Sensor2 A1 //3003
#define Sensor3 A2 //3004
#define Sensor4 A3 //3005
#define Sensor5 A4 //3006

int t1state = 0; //0 = closed 1 = thrown
int turnout1 = 0; //the value is not important

#define turnoutMoveSpeed 8   // [ms] lower number is faster
unsigned long turnoutMoveDelay; // variable used servo timer

/*The next bit of the code we setup the communication bus we will use. the 3 values inside the 
 *brackets picks up the address in this case 3 and then sets the 24 inputs and 48 outputs which
 *the cmri library requires to be able to do its work. If you want to use the 3 wire module
 *you will have to comment out the first line and uncomment out the next 2 lines of code.
 */
CMRI cmri(CMRI_ADDR, 24, 48); // comment this out if useing 3 wire module
//Auto485 bus(DE_PIN); //  uncomment this for 3 wire module
//CMRI cmri(CMRI_ADDR, 24, 48, bus); //  uncomment this for 3 wire module


Servo turnOut1;// this seup the instance of the servoand gives it the name turnOut1


// added code start

byte turnout1Position = turnout1ClosedPosition;
byte turnout1Target   = turnout1ClosedPosition;

//  added code stop

void setup() {
  delay(2000);
  //setup output pins
  pinMode(throughApproachGreenLed, OUTPUT);
  pinMode(throughApproachRedLed, OUTPUT);
  pinMode(divergingApproachGreenLed, OUTPUT);
  pinMode(divergingApproachRedLed, OUTPUT);
  pinMode(throughGreenLed, OUTPUT);
  pinMode(throughRedLed, OUTPUT);


  //setup input pins
  pinMode(Sensor1, INPUT_PULLUP);
  pinMode(Sensor2, INPUT_PULLUP);
  pinMode(Sensor3, INPUT_PULLUP);
  pinMode(Sensor4, INPUT_PULLUP);
  pinMode(Sensor5, INPUT_PULLUP);

  digitalWrite(throughGreenLed, LOW);  // light led 1 on start up  to show its working
  delay(2000);
  digitalWrite(throughGreenLed, HIGH);
  turnOut1.attach(3);
  turnOut1.write(turnout1ClosedPosition); 

  Serial.begin(9600);  // comment this out for 3 wire use
  //bus.begin(96000); //uncomment this for 3 wire module
}

void loop() {
  cmri.process();
  turnout1 = (cmri.get_bit(0)); //turnout 3001 jmri address

  if (turnout1 == 1 && t1state == 0) {
    turnOut1.attach(3);   // reconnect the servo ready for movement
    turnout1Target = turnout1ThrownPosition;   //  changed to set the target line below added to attach servo
    t1state = 1;
    cmri.set_bit(0, HIGH); //3001 jmri address output
  }
  if (turnout1 == 0 && t1state == 1) {
    turnOut1.attach(3);    // reconnect the servo ready for movement
    turnout1Target = turnout1ClosedPosition;    //  changed to set the target
    t1state = 0;
    cmri.set_bit(0, LOW);
  }
   if (turnout1Position == turnout1Target){
    turnOut1.detach();  //Disconnect the servo once its in the correct position
   }

  digitalWrite (throughApproachGreenLed, !cmri.get_bit(1)); //jmri 3002 
  digitalWrite (throughApproachRedLed, !cmri.get_bit(2)); 
  digitalWrite (divergingApproachGreenLed, !cmri.get_bit(3)); 
  digitalWrite (divergingApproachRedLed, !cmri.get_bit(4)); 
  digitalWrite (throughGreenLed, !cmri.get_bit(5)); //SH3
  digitalWrite (throughRedLed, !cmri.get_bit(6)); //SH3


  cmri.set_bit(1, !digitalRead(A0)); //jmri 3002
  cmri.set_bit(2, !digitalRead(A1));
  cmri.set_bit(3, !digitalRead(A2));
  cmri.set_bit(4, !digitalRead(A3));
  cmri.set_bit(5, !digitalRead(A4));

  if (turnout1Position != turnout1Target) {
    if (millis() > turnoutMoveDelay) {
      turnoutMoveDelay = millis() + turnoutMoveSpeed;
      if (turnout1Position < turnout1Target) turnout1Position++;
      if (turnout1Position > turnout1Target) turnout1Position--;
      turnOut1.write(turnout1Position);
    }
  } 
  }
