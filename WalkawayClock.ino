#include <DS3231.h>
#include <Wire.h>
//Our clock will stop after counting the number of steps

//defining the rtc module
DS3231  rtc(SDA, SCL);
Time  t;
#define buz 2 //buzzer is connected to the 2nd pin 
int Hor;
int Min;
int Sec;

const int xpin = A1;
const int ypin = A2;
const int zpin = A3;

//this  is the threeshold that must be crossed in order for the step to be count and not
// ignored as vibration 
float threshold = 10;
// matrices which will hold values when we use loops
float xval[100] = {0};
float yval[100] = {0};
float zval[100] = {0};
float xavg, yavg, zavg;
int steps, flag = 0;
void setup()
{
  Wire.begin();
  rtc.begin();
  Serial.begin(9600);
  pinMode(buz, OUTPUT);
  Serial.print("Arduino Alarm ");
  // uncomment these line sonce and set the time and date in the DS3231 module
  //rtc.setDOW(SATURDAY);     // Set Day-of-Week (all caps)
  //rtc.setTime(20, 35, 0);     // Set the time (24hr format)
  //rtc.setDate(21,11 , 2020);   // Set the date 
  calibrate();
}
void loop()
{
  int acc = 0;
  float totvect[100] = {0};
  float totave[100] = {0};
  float xaccl[100] = {0};
  float yaccl[100] = {0};
  float zaccl[100] = {0};

  // this part will get the time using the library for DS3231
  t = rtc.getTime();  
  Hor = t.hour;
  Min = t.min;
  Sec = t.sec;
  Serial.println("Time: ");
  Serial.print(rtc.getTimeStr());
  //set the time for alarm here
  if ( Hor == 3 &&  (Min == 12 || Min == 13)) //Comparing the current time with the Alarm time.
  {

    digitalWrite(buz, HIGH);
    for (int a = 0; a < 3 00; a++)
    {
      // what we are doing is that we are calculating the total accelaration vector and if 
      // if it is bigger than out threeshold we let it passs outherwise we let it as unecasary vibration
       
      xaccl[a] = float(analogRead(xpin));
      delay(1);
      yaccl[a] = float(analogRead(ypin));
      delay(1);
      zaccl[a] = float(analogRead(zpin));
      delay(1);
      totvect[a] = sqrt(((xaccl[a] - xavg) * (xaccl[a] - xavg)) + ((yaccl[a] - yavg) * (yaccl[a] - yavg)) + ((zval[a] - zavg) * (zval[a] - zavg)));
      totave[a] = (totvect[a] + totvect[a - 1]) / 2 ;
      Serial.println("totave[a]");
      Serial.println(totave[a]);
      delay(100);
      if (totave[a] > threshold && flag == 0)
      {
        steps = steps + 1;
        flag = 1;
      }
      else if (totave[a] > threshold && flag == 1)
      {
        // Don't Count
      }
      if (totave[a] < threshold   && flag == 1)
      {
        flag = 0;
      }
      if (steps < 0) {
        steps = 0;
      }
      Serial.println('\n');
      Serial.print("steps: ");
      Serial.println(steps);

      delay(1000);
      Serial.print(" take 10 steps to stop the alarm");
    if (steps >= 10) {
      digitalWrite(buz, LOW);
    }
    else {
      digitalWrite(buz, HIGH);
    }
delay(400);
  }

  delay(1000);
}

// this part of the code will be executed first for caculating the average of the
// speed in the three directions
// if the person has some net accelaration in the starting, he will as we do not live in an inertial and there will be 
// gravity and this part of the code will nullify any problem that might be cused due to that reason..........
void calibrate()
{ 
  float sum = 0;
  float sum1 = 0;
  float sum2 = 0;
  // run the loop 100 ad then devide by 100 to get the average
  for (int i = 0; i < 100; i++) {
    xval[i] = float(analogRead(xpin));
    sum = xval[i] + sum;
  }
  delay(100);
  xavg = sum / 100.0;
  //Serial.println(xavg);
  for (int j = 0; j < 100; j++)
  {
    yval[j] = float(analogRead(ypin));
    sum1 = yval[j] + sum1;
  }
  yavg = sum1 / 100.0;
  //Serial.println(yavg);
  delay(100);
  for (int q = 0; q < 100; q++)
  {
    zval[q] = float(analogRead(zpin));
    sum2 = zval[q] + sum2;
  }
  zavg = sum2 / 100.0;
  delay(100);
 // Serial.println(zavg);
}
