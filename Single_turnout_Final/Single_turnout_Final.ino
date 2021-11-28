/*pins 0 - 1 are usedfor Rx and Tx no trigger pin required due to using
  TTL to RS485 Module Serial Port MCU Automatic Flow Control Module.
  simple servo cmri node which moves 1 servo and the sends back a bit back to jmri to indicate
  the servo has been moved
  also will control 8 off signal leds
  This code can be used with a usb connected to pc when using jmri.
  This code use Chris Sharp slow motion servo code and help with servo disconnection
*/
#include <CMRI.h>
#include <Servo.h>

#define CMRI_ADDR 3
#define turnout1ClosedPosition 65 // previous 70
#define turnout1ThrownPosition 110  // previous 108

// define signal leds turnout 1
#define throughGreenLed 4 //jmri *002
#define throughRedLed 5
#define divergingGreenLed 6
#define divergingRedLed 7
#define throughApproachGreenLed 8
#define throughApproachRedLed 9
#define divergingApproachGreenLed  10
#define divergingApproachRedLed  11

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

CMRI cmri(CMRI_ADDR, 24, 48); // defaults to a SMINI with address 0. SMINI = 24 inputs, 48 outputs
Servo turnOut1;


// added code start

byte turnout1Position = turnout1ClosedPosition;
byte turnout1Target   = turnout1ClosedPosition;

//  added code stop

void setup() {
  delay(2000);
  //setup output pins
  pinMode(throughGreenLed, OUTPUT);
  pinMode(throughRedLed, OUTPUT);
  pinMode(divergingGreenLed, OUTPUT);
  pinMode(divergingRedLed, OUTPUT);
  pinMode(throughApproachGreenLed, OUTPUT);
  pinMode(throughApproachRedLed, OUTPUT);
  pinMode(divergingApproachGreenLed , OUTPUT);
  pinMode(divergingApproachRedLed , OUTPUT);

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

  Serial.begin(9600);
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

  digitalWrite (throughGreenLed, !cmri.get_bit(1)); //jmri 3002 SH1
  digitalWrite (throughRedLed, !cmri.get_bit(2)); //SH1
  digitalWrite (divergingGreenLed, !cmri.get_bit(3)); //SH2
  digitalWrite (divergingRedLed, !cmri.get_bit(4)); //SH2
  digitalWrite (throughApproachGreenLed, !cmri.get_bit(5)); //SH3
  digitalWrite (throughApproachRedLed, !cmri.get_bit(6)); //SH3
  digitalWrite (divergingApproachGreenLed , !cmri.get_bit(7)); //SH4
  digitalWrite (divergingApproachRedLed , !cmri.get_bit(8)); //jmri 3009 SH4

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
