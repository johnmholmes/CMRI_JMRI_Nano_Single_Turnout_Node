/*pins 0 - 1 are usedfor Rx and Tx no trigger pin required due to using
  TTL to RS485 Module Serial Port MCU Automatic Flow Control Module.
  simple servo cmri node which moves 1 servo and the sends back a bit back to jmri to indicate
  the servo has been moved
  also will control 6 off signal leds
  This code can be used with a usb connected to pc when using jmri.
  This code use Chris Sharp slow motion servo code and help with servo disconnection
  This is the final version i will be doing of this node. for demonstration purposes only
  feel free to download and change the code for your use. see MIT licence for details
*/
#include <CMRI.h>
#include <Servo.h>
//#include <Auto485.h>   //uncomment this if you want to use a 3 wire rs485 module
//#define DE_PIN 2       //uncomment this if you want to use a 3 wire rs485 module

#define CMRI_ADDR 3
#define turnout1ClosedPosition 65 // previous 70
#define turnout1ThrownPosition 110  // previous 108

// define signal leds turnout 1
#define throughApproachGreenLed 4 //jmri 3002
#define throughApproachRedLed 5
#define divergingApproachGreenLed 6
#define divergingApproachRedLed 7
#define throughGreenLed 8
#define throughRedLed 9

//define infrared detection peel
#define Sensor1 A0
#define Sensor2 A1
#define Sensor3 A2
#define Sensor4 A3
#define Sensor5 A4

int t1state = 0; //0 = closed 1 = thrown
int turnout1 = 0;

#define turnoutMoveSpeed 8   // [ms] lower number is faster
unsigned long turnoutMoveDelay;

CMRI cmri(CMRI_ADDR, 24, 48); // comment this out if useing 3 wire module
//Auto485 bus(DE_PIN); //  uncomment this for 3 wire module
//CMRI cmri(CMRI_ADDR, 24, 48, bus); //  uncomment this for 3 wire module
Servo turnOut1;


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
