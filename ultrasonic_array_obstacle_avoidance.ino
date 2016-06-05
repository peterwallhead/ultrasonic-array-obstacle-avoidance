/*
  Parallax Ping))) ultrasonic sensor measureDistance function from tronixstuff.wordpress.com - CC by-sa-nc
  http://littlebirdelectronics.com.au/blogs/news/4724922-tutorial-parallax-ping-ultrasonic-sensor
  Connect Ping))) signal pin to Arduino digital 8

  Motor shield control functions and direction control added by Peter Wallhead.

  This example code is in the public domain.

*/

int signalL=8;
int signalF=9;
int signalR=10;
int distance;
int distanceL;
int distanceF;
int distanceR;
unsigned long pulseduration=0;

// Assign motor shield pins that start and stop the motor.
int PWM1 = 5;
int PWM2 = 6;

// Assign motor shield pins that dictate motor direction.
int DIR1 = 4;
int DIR2 = 7;

// Set default motor speed.
int SPEED = 150;

void setup()
{
  Serial.begin(9600);
  
  // Set pin modes to output for all those used by the motor shield.
  int mp;
  for(mp=4; mp<=7; mp++) {
      pinMode(mp, OUTPUT);
  }
  
  // Set pin modes to output for all those used by the Pings.
  int pp;
  for(pp=8; pp<=10; pp++) {
      pinMode(pp, OUTPUT);
  }
  
}

void measureDistance(int sensorPin)
{
  // Set pin as output so we can send a pulse.
  pinMode(sensorPin, OUTPUT);

  // Set output to LOW.
  digitalWrite(sensorPin, LOW);
  delayMicroseconds(5);
  
  // Now send the 5uS pulse out to activate Ping)))
  digitalWrite(sensorPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensorPin, LOW);
  
  // Now we need to change the digital pin 
  // to input to read the incoming pulse.
  pinMode(sensorPin, INPUT);
  
  // Finally, measure the length of the incoming pulse.
  pulseduration=pulseIn(sensorPin, HIGH);
  
  // Divide the pulse length by half.
  pulseduration=pulseduration/2;
  
  // Now convert to centimetres. We're metric here people...
  distance = int(pulseduration/29);
  
  // Set distance variable based on which pin is currently being read.
  if(sensorPin == 8) {
    distanceL = distance;
  }
  
  if(sensorPin == 9) {
    distanceF = distance;
  }
  
  if(sensorPin == 10) {
    distanceR = distance;
  }
}

void loop()
{
  // Display on serial monitor (not required, but handy for calibration).
  Serial.print("Distance - ");
  Serial.print(distanceL);
  Serial.print(" | ");
  Serial.print(distanceF);
  Serial.print(" | ");
  Serial.println(distanceR); 
  
  // Measure distance in front of sensor to nearest object
  measureDistance(signalF);
  measureDistance(signalL);
  measureDistance(signalR);
    
  // while driving forward in a straight line.
  if(distanceF>30) {
   
    while(distanceR<30) { // If right sensor detects an object less than 30cm away, turn left.
      left(50);
      measureDistance(signalR);
    }
    
    while(distanceL<30) { // If left sensor detects an object less than 30cm away, turn right.
      right(50);
      measureDistance(signalL);
    }
    
    forward(100);
    
  } else { // If front distance is less than 30cm then pause and measure distance on left and right.
    pause(500);
    measureDistance(signalL);
    measureDistance(signalR);
        
    if(distanceL>distanceR) {
      while(distanceF<30) { //Keep turning until front sensor is clear of obstacle.
        left(50);
        measureDistance(signalF);
      }
    } else {
      while(distanceF<30) {
        right(50);
        measureDistance(signalF);
      }
    } 
  }
  
  delay(100); 
}

void forward(int di) {
  analogWrite(PWM1,SPEED);
  analogWrite(PWM2, SPEED);
  
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  delay(di);
}

void backward(int di) {
  analogWrite(PWM1,SPEED);
  analogWrite(PWM2, SPEED);
  
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, LOW);
  delay(di);
}

void left(int de) {
  int turn_length;
  turn_length = de / 2;
  
  analogWrite(PWM1,SPEED);
  analogWrite(PWM2,SPEED);
  
  digitalWrite(DIR1, LOW);
  digitalWrite(DIR2, HIGH);
  delay(turn_length);
}

void right(int de) {
  int turn_length;
  turn_length = de / 2;
  
  analogWrite(PWM1,SPEED);
  analogWrite(PWM2, SPEED);
  
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, LOW);
  delay(turn_length);
}

void pause(int ti) {
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  
  digitalWrite(DIR1, HIGH);
  digitalWrite(DIR2, HIGH);
  delay(ti);
}

