# A simple Arduino Nano node to control a single turnout and control 4 signal 2 aspect heads in JMRI and also has 5 sensor inputs, 

It will send the servo into a closed position at startup and send a single bit to JMRI which i use as feed back for the turnout, this will use in this case 3001 as the sensor the rest of the sensors use 3002,to 3006 turnout 1 uses 3001, and the leds  use 3002 to 3009

# This uses Slow motion servo movement 

# Also this will move the servo to closed and then disconnect the servo until it need to be moved again so helps stops jitter of the servo
