// Sensor Arduino

#include <Wire.h>

int Slave_address=0x68;                               // (7-bit) I2C address of the MPU (Section 9.2)

long accX,accY,accZ;                                  // for storing and using values from Accelerometers in the sensor
double aX,aY,aZ;

long gyroX,gyroY,gyroZ;                               // for storing and using values form Gyroscopes in the sensor

double errorZ;
double gX,gY,gZ;

 double compYaw;
int Pitch, Yaw;

void setup() {
  
 Serial.begin(9600);                               
  Wire.begin();         
  setMPU();                                           // initial programming of the sensor done here
  checkGyro_Offset();
  compYaw=0;

 

}


void setMPU()                
{
  Wire.beginTransmission(Slave_address);              // Start communicating with the MPU-6050
  Wire.write(0x6B);                                   // Accessing Reg-6B                                        
  Wire.write(0x00);                                   // Set Reg-6B to zero...required to Activate the Sensor 
  Wire.endTransmission();                                             
  
  Wire.beginTransmission(Slave_address);                       
  Wire.write(0x1B);                                   // Accessing Reg-1B                  
  Wire.write(0x08);                                   // Set Reg-1B to 0x08...setting gyroscope full-scale=± 500 °/s                 
  Wire.endTransmission();

  Wire.beginTransmission(Slave_address);                       
  Wire.write(0x1C);                                   // Accessing Reg-1C                  
  Wire.write(0x00);                                   // Set Reg-1C to 0x00...setting accelerometer full-scale=± 2g                 
  Wire.endTransmission();  
}

void checkGyro_Offset()                              // checking for error in gyro reading...non-zero values when MPU stationary
{
  for(int i=0; i<1000; i++)
  {
  Wire.beginTransmission(Slave_address);                                        
  Wire.write(0x43);                                                     // starting register for gyro readings
  Wire.endTransmission();  
  Wire.requestFrom(Slave_address,6);                                    // Request 6 bytes from the MPU-6050
  while(Wire.available() < 6);                                          // Wait until all the bytes are recieved
  gyroX += Wire.read()<<8|Wire.read();                                 
  gyroY += Wire.read()<<8|Wire.read();                                 
  gyroZ += Wire.read()<<8|Wire.read();
  }

  // calculating avg error form a 1000 gyro readings
  errorZ=gyroZ/1000;

}


void Event1(int p)
{

p=p+500;
  
  Serial.print('p');
delay(20);
Serial.print(p);
delay(20);

 
}

void Event2(int y)
{
y=y+500;

if(Yaw>670)
{
  Yaw=670;
}

if(Yaw<310)
{
  Yaw=310;
}

  
Serial.print('y');
delay(20);
Serial.print(y);
delay(20);
}


void loop() {

  getAcc_Raw();
  getGyro_Raw();
  process_Raw();
  //displayProcessed_Data();

  Pitch = (atan2(aX, aZ)*-57.23)+92;
  compYaw=compYaw+((.04*gZ));
Event1(Pitch);


getOnly_gyro();
compYaw=compYaw+((.02*gZ));
Yaw=(-compYaw/2)+90;
Event2(Yaw);
compYaw=compYaw+((.02*gZ));

//printAngles();


 
   
}


void getOnly_gyro()
{
   Wire.beginTransmission(Slave_address);                                        
  Wire.write(0x43);                                                     // starting register for gyro readings
  Wire.endTransmission();  
  Wire.requestFrom(Slave_address,6);                                    // Request 6 bytes from the MPU-6050
  while(Wire.available() < 6);                                          // Wait until all the bytes are recieved
  gyroX = Wire.read()<<8|Wire.read();                                 
  gyroY = Wire.read()<<8|Wire.read();                                 
  gyroZ = Wire.read()<<8|Wire.read();

  gZ=(gyroZ-errorZ)/65.5;
}


void getAcc_Raw() 
{
  Wire.beginTransmission(Slave_address);                                        
  Wire.write(0x3B);                                                    // starting register accelerometer readings
  Wire.endTransmission();  
  Wire.requestFrom(Slave_address,6);                                   // Request 6 bytes from the MPU-6050
  while(Wire.available() < 6);                                         // Wait until all the bytes are received
  accX = Wire.read()<<8|Wire.read();                                  
  accY = Wire.read()<<8|Wire.read();                                  
  accZ = Wire.read()<<8|Wire.read();                                                                     
}

void getGyro_Raw()
{
  Wire.beginTransmission(Slave_address);                                        
  Wire.write(0x43);                                                     // starting register for gyro readings
  Wire.endTransmission();  
  Wire.requestFrom(Slave_address,6);                                    // Request 6 bytes from the MPU-6050
  while(Wire.available() < 6);                                          // Wait until all the bytes are recieved
  gyroX = Wire.read()<<8|Wire.read();                                 
  gyroY = Wire.read()<<8|Wire.read();                                 
  gyroZ = Wire.read()<<8|Wire.read();
}

void process_Raw()
{
  aX=accX/16384.0;                                                       // attaining meaningful accelerometer readings from raw
  aY=accY/16384.0;
  aZ=accZ/16384.0;

 // attaining meaningful and error-free gyroscope readings from raw 
  gZ=(gyroZ-errorZ)/65.5;
}


void printAngles()
{
  Serial.println();
  Serial.print("Pitch=");Serial.print(Pitch); Serial.print("\t");
        Serial.print("Yaw=");Serial.print(Yaw); Serial.print("\t");
Serial.println();
              
}

void displayProcessed_Data()                                            // Displaying the processed data(readings)
{
  Serial.print("acc (g):\t");
  Serial.print("x=");Serial.print(aX); Serial.print("\t");
  Serial.print("y=");Serial.print(aY); Serial.print("\t");
  Serial.print("z=");Serial.print(aZ); Serial.print("\t");
  Serial.print("gyro (deg):\t");
  Serial.print("x=");Serial.print(gX); Serial.print("\t");
  Serial.print("y=");Serial.print(gY); Serial.print("\t");
  Serial.print("z=");Serial.println(gZ);
}



