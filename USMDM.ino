const int trigger = 6;
const int echo = 5;

const int DIRA=13;
const int PWMA=11;

const int BOTTOM_STOP = 2;

int spead = 255;
int del = 1000;

volatile boolean isDown = false;

void setup()
{
  pinMode(BOTTOM_STOP, INPUT);
  digitalWrite(BOTTOM_STOP, LOW);
  attachInterrupt(0, bottomStop, LOW);
  attachInterrupt(1, topStop, FALLING);
 
  pinMode(DIRA,OUTPUT);
  pinMode(PWMA,OUTPUT);
  
  Serial.begin(115200);
  Serial.println("lets begin");
}

void topStop() {
  halt();
}

void bottomStop() {
  int reading = digitalRead(BOTTOM_STOP);
  delayMicroseconds(2000);
  if( reading == digitalRead(BOTTOM_STOP) )
    halt();
  isDown = true;
  detachInterrupt(0);
  attachInterrupt(0, bottomStopDone, HIGH);
}
  
void bottomStopDone() {
  isDown = false;
  detachInterrupt(0);
  attachInterrupt(0, bottomStop, LOW);
}
 
void up()
{
  Serial.println("up");
  analogWrite(PWMA,255);
  digitalWrite(DIRA,LOW);//turn DC Motor A move anticlockwise
}
void down()//
{
  if(isDown)
    return;
  Serial.println("down");
  analogWrite(PWMA,80);//turn DC Motor A move clockwise
  digitalWrite(DIRA,HIGH);
}

void halt()//
{
//  Serial.println("stop");
  digitalWrite(DIRA,LOW);// Unenble the pin, to stop the motor. this should be done to avid damaging the motor. 
  digitalWrite(PWMA,LOW);
}

boolean shouldStop() {
  
  int cm = getDistance();
  delay(25);
  cm += getDistance();
  delay(25);
  cm += getDistance();
  delay(25);
  
  cm /= 3;
  
//  Serial.print(cm);
//  Serial.println();
  
  if( cm < 14 )
    return true;
  else
    return false;
}

int getDistance() {
    // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches;
  
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigger, LOW);
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echo, HIGH);
  return microsecondsToCentimeters(duration);
  
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void loop(){
  char input = Serial.read();
  switch(input) {
    case 'f':
      up();
      delay(500);
      halt();
      break;  
    case 'b':
      down();
      delay(500);
      halt();
      break;
  }
  delay(50);
}
