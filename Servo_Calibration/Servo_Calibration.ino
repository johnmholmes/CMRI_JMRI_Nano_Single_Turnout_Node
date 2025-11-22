/*
 * Disclaimer this code has been writen by John Holmes and tested on An Arduino Nano only.
 * It may work on other Arduino boards but ha not been tested.
 * 
 * The sketch is used to calibrate a single servo attched to pin 4 in this case and it uses the servo.h library
 * to work with the servo. We must setup a few things for the library to work.
 * 
 * 1 create a servo object and give it a name in this case "servo1"
 * 2 we have to tell the library which pin it is attached to "servo1.attach(4)"
 * 3 to move the servo we will use an int varilble called "pos" and the command is "servo1,write(pos)
 * 
 * The int varible will get changed using the serial monitor which has to be set to "serial.begin(9600)" in the
 * serial monitor window on your pc.
 * 
 */

#include <Servo.h>                                  // include the servo.h library 

Servo servo1;                                       // create servo name object

String readString;                                  // variable for reading the serial bytes
int pos = 90;                                       // set servo to mid position on startup

void setup() {

  servo1.attach(4);                                 // attach servo 0range lead to pin 4 on Arduino
  servo1.write(pos);                                // set the servo to closed from start up

  Serial.begin(9600);                               // starts the serial monitor
/*
 * The rest of the void setup will be displayed in the serial monitor window and is the instruction 
 * explaining how to enter the required calibrated position. The delays are there only to slow the message down
 * to ensure you read the message.
 */

  delay(5000);
  Serial.println("WARNING: Exceeding the max range could damage the servo.");
  delay(200);
  Serial.println("Servo calibration using servo.h and not PWM");
  delay(200);
  Serial.println("Use this to calibrate your servo to find the range of movement required");
  delay(200);
  Serial.println("Servos min and max can vary, try the 0 - 180 range to start with."); 
  delay(200);
  Serial.println("The servo should start close to the centre of the range");
  delay(200);
  Serial.println("Type a value followed by a + to move in one direction or a value followed by a - to move in the other direction");
  delay(200);
  Serial.println("For example 1+ to 2-");  
  delay(200);
  Serial.println("To move to a specific location use strings like 0x or 180x for new servo position");
  delay(200);
  Serial.println("Move the servo to find the required range for whatever you're operating.");
  delay(200);
  Serial.println("WARNING: Exceeding the max range could damage the servo.");
  delay(200);
  Serial.println();
  Serial.println("Centre point:");  
  Serial.println(pos);  
  delay(1000);
}


/*
 * The while loop runs continuosly and is looking to see if the serial monitor is available.
 * 
 * It is also looking to see if you have typed anything in the send box.
 * 
 * Only when you have type a number into the box with either of the following will it work -,+, or leter x
 * entering anything other than a number and pressing enter will return the previous value.
 * 
 * The first if statement is checking that the value is greater than 0 and if so it can then looks at the 
 * next 3 if statements these are used to determin what we want it to do.
 * 
 * x = move to the number value which is between 0 - 180 in degrees
 * - = move by that number from the previous position in a minus direction
 * + = ove by that number from the previous position in a minus direction
 */
void loop()
{
  while (Serial.available()) {
    char c = Serial.read();                  //gets one byte from serial buffer
    readString += c;                         //makes the string readString
    delay(2);                                //slow looping to allow buffer to fill with next character
  }
  if (readString.length() >0) {

    if(readString.indexOf('x') >0) {
      pos = readString.toInt();
    }

    if(readString.indexOf('+') >0) {
      pos = pos + readString.toInt();
    }

    if(readString.indexOf('-') >0) {
      pos = pos - readString.toInt();
    }

    
/*
 * The final part of the sketch will print to the serial monitor the desired position we have sent to the 
 * Arduino .
 * 
 * It the uses the servo1.write to set the servo to the position of the variable "pos"
 * 
 * Once the servo has moved it then will clear the serial monitor edit position ready for 
 * the next reading to be sent
 */
    Serial.println(pos);
    servo1.write(pos);
    readString="";                           //empty for next input
  }
}