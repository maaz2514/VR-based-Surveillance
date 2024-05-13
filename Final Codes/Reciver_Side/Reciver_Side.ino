#include <Servo.h>

Servo headServo;                                      // for upper servo
Servo baseServo;                                      // for lower servo

int Yaw;
int Pitch;

int flag = 0;
int value;
char buff;

int up=3;
int down=4;


void setup() {

  headServo.attach(11);                                // attaches the upper servo on pin 10 to the headservo object
  baseServo.attach(10);                               // attaches the lower servo on pin 11 to the baseservo object
  setServo();


pinMode(up,OUTPUT);
pinMode(down,OUTPUT);    
  Serial.begin(9600);

digitalWrite(up,LOW);
digitalWrite(down,LOW);
}

void setServo()
{
  headServo.write(92);                                  // set initiale position(angle) to zero
  baseServo.write(90);                                   // set initiale position(angle) to zero
}



void loop() {

while (flag != 1)
  {
    while (Serial.available() <= 0);

    if (Serial.read() == 'y') {
      flag = 1;
    }
  }


    Yaw = 0;
    flag = 0;
    for (int y = 0; y < 3; y++)
    {


      while (Serial.available() <= 0);

      buff = Serial.read();
      value = int(buff);
      value = value - 48;


        if (y == 0)
        {
          Yaw = Yaw + (value * 100);
        }
        if (y == 1)
        {
          Yaw = Yaw + (value * 10);
        }
        if (y == 2)
        {
          Yaw = Yaw + value;
        }
      

    }
  





  

  while (flag != 1)
  {
    while (Serial.available() <= 0);

    if (Serial.read() == 'p') {
      flag = 1;
    }
  }



    flag = 0;
    for (int y = 0; y < 3; y++)
    {


      while (Serial.available() <= 0);

      buff = Serial.read();
      value = int(buff);
      value = value - 48;


        if (y == 0)
        {
          Pitch = (value * 100);
        }
        if (y == 1)
        {
          Pitch = Pitch + (value * 10);
        }
        if (y == 2)
        {
          Pitch = Pitch + value;
        }


      
        }

  Yaw = Yaw - 500; 
  Pitch = Pitch - 508;



if(Pitch>130)
{
  
  digitalWrite(up,HIGH);
  delay(5);
  digitalWrite(up,LOW);
}

if(Pitch<45)
{
  
  digitalWrite(down,HIGH);
  delay(5);
  digitalWrite(down,LOW);
}


  headServo.write(Pitch);
  baseServo.write(Yaw*2);

}






