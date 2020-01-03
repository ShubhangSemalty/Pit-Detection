//pin defination
const int trigPin = 9;
const int echoPin = 10;

const int ir_in = 8;
const int relay = 7;

// defines variables
long duration;
int distance;
volatile byte relayState = LOW;
int ground_dist = 3;
#define STATUS_PIN                      13            //status led    

#define MPU_POWER_REG                   0x6B          //Power on the sensor (Joop's Video)
#define MPU_POWER_CYCLE                 0b00000000    // ?
#define MPU_READ_TIMEOUT                2000          //if we give a command to read from 6050 and it doesnt respond we move on to next command
#define MPU_SAMP_FREQ                   250           // takes new reading in every 250th of second

#define MPU_GYRO_CFG_REG                0x1B          //  to select from diffrent sensitivity ranges present in this sensor
#define MPU_GYRO_READ_REG               0x43          // which register do we want to read the data from for the values of the gyroscope
#define MPU_GYRO_READ_REG_SIZE          6             // the data that we are reading is of 6 bytes
#define MPU_GYRO_CFG_250DEG             0b00000000    //
#define MPU_GYRO_READINGSCALE_250DEG    131.0         //
#define MPU_GYRO_CFG_500DEG             0b00001000    //
#define MPU_GYRO_READINGSCALE_500DEG    65.5          //
#define MPU_GYRO_CFG_1000DEG            0b00010000    // this section is abput which sensitivity mode so want to choose fir the sensor to read the values 
#define MPU_GYRO_READINGSCALE_1000DEG   32.8          //
#define MPU_GYRO_CFG_2000DEG            0b00011000    //
#define MPU_GYRO_READINGSCALE_2000DEG   16.4          //
#define MPU_CALIBRATE_READING_NUM       4000          // for caliberation max no. of reading to calibrate the device

#define MPU_ACCEL_CFG_REG               0x1C
#define MPU_ACCEL_READ_REG              0x3B
#define MPU_ACCEL_READ_REG_SIZE         6
#define MPU_ACCEL_CFG_2G                0b00000000
#define MPU_ACCEL_READINGSCALE_2G       16384.0
#define MPU_ACCEL_CFG_4G                0b00001000
#define MPU_ACCEL_READINGSCALE_4G       8192.0
#define MPU_ACCEL_CFG_8G                0b00010000
#define MPU_ACCEL_READINGSCALE_8G       4096.0
#define MPU_ACCEL_CFG_16G               0b00011000
#define MPU_ACCEL_READINGSCALE_16G      2048.0

#define MPU_I2C_ADDRESS                0b1101000

#include <Wire.h>

float gForceX, gForceY, gForceZ;
float rotX, rotY, rotZ;
float calibX, calibY, calibZ;
//float pitch, roll, yaw;
long loopTimer = 0;


void setup() {
  Serial.begin(57600);
  pinMode(STATUS_PIN,OUTPUT);
  digitalWrite(STATUS_PIN, LOW);
  pinMode(LED_BUILTIN,OUTPUT);
  Wire.begin();
  SetupMPU();
  delay(3000);
  calibrateGyro();
  Serial.println("setup complete.");

pinMode(trigPin, OUTPUT);       // Sets the trigPin as an Output
pinMode(echoPin, INPUT);      // Sets the echoPin as an Input
pinMode(ir_in, INPUT);
pinMode(relay, OUTPUT);
digitalWrite(relay, HIGH);
}

void SetupMPU()
{
  Wire.beginTransmission(MPU_I2C_ADDRESS);
  Wire.write(MPU_POWER_REG);
  Wire.write(MPU_POWER_CYCLE); 
  Wire.endTransmission();
  Wire.beginTransmission(MPU_I2C_ADDRESS);
  Wire.write(MPU_GYRO_CFG_REG);
  Wire.write(MPU_GYRO_CFG_250DEG); 
  Wire.endTransmission();
  Wire.beginTransmission(MPU_I2C_ADDRESS);
  Wire.write(MPU_ACCEL_CFG_REG);
  Wire.write(MPU_ACCEL_CFG_2G); 
  Wire.endTransmission();
}

bool ReadMPU()
{
  if(MPUReadAccel() && MPUReadGyro())
  {
//    calcRotation();
   // printData();
    delay(100);
    return true;
  }
  return false;
}

bool MPUReadAccel()
{
  Wire.beginTransmission(MPU_I2C_ADDRESS);
  Wire.write(MPU_ACCEL_READ_REG);
  Wire.endTransmission();
  Wire.requestFrom(MPU_I2C_ADDRESS, MPU_ACCEL_READ_REG_SIZE);
  long timeout = millis() + MPU_READ_TIMEOUT;
  while(Wire.available() < MPU_ACCEL_READ_REG_SIZE && timeout < millis());
  if (timeout <= millis()) return false;
  gForceX = (long)(Wire.read() << 8 | Wire.read()) / MPU_ACCEL_READINGSCALE_2G;
  gForceY = (long)(Wire.read() << 8 | Wire.read()) / MPU_ACCEL_READINGSCALE_2G;
  gForceZ = (long)(Wire.read() << 8 | Wire.read()) / MPU_ACCEL_READINGSCALE_2G;
  return true;
}

bool MPUReadGyro()
{
  Wire.beginTransmission(MPU_I2C_ADDRESS);
  Wire.write(MPU_GYRO_READ_REG);
  Wire.endTransmission();
  Wire.requestFrom(MPU_I2C_ADDRESS, MPU_GYRO_READ_REG_SIZE);
  long timeout = millis() + MPU_READ_TIMEOUT;
  while(Wire.available() < MPU_ACCEL_READ_REG_SIZE && timeout < millis());
  if (timeout <= millis()) return false;
  rotX = (long)(Wire.read() << 8 | Wire.read()) / MPU_GYRO_READINGSCALE_250DEG;
  rotY = (long)(Wire.read() << 8 | Wire.read()) / MPU_GYRO_READINGSCALE_250DEG;
  rotZ = (long)(Wire.read() << 8 | Wire.read()) / MPU_GYRO_READINGSCALE_250DEG;
  
  return true;
}

void loop() {
int flg = 0;
if(ReadMPU())
  {                          
    while(micros() - loopTimer < 4000);
    loopTimer = micros();
  }
// Clears the trigPin
digitalWrite(trigPin, LOW);
delay(10);

// Sets the trigPin on HIGH state for 10 micro seconds

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor

//Serial.print("Distance: ");
//Serial.println(distance);

/*if(digitalRead(ir_in)==1)
{
Serial.println("ON");
delay(10); 
}
else
{
Serial.println("OFF");
delay(10);
}*/  
if(distance<ground_dist && digitalRead(ir_in) == 0){
Serial.println("Puddle detected please slow down");
  if(gForceY>-0.40)
  { Serial.println("Slowing DOWN");
    delay(5000);
    if(gForceY>-0.10)
    {
    Serial.println("Pouring Oil");
    digitalWrite(relay,HIGH);
    delay(7000);
    Serial.println("Lets Roll");
    delay(5000);
    flg++;
    }
    
  }
}
else
digitalWrite(relay,LOW);
}

/*void printData() 
{ Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ); 
}*/

void calibrateGyro()
{
  loopTimer = 0;
  
  digitalWrite(STATUS_PIN, HIGH);
  Serial.println("Calibrating Gyro");
  
  calibX = 0;
  calibY = 0;
  calibZ = 0;
  
  
  for(int i=0; i<MPU_CALIBRATE_READING_NUM;i++)
  {
    if(MPUReadGyro())
    {
      calibX += rotX;
      calibY += rotY;
      calibZ += rotZ;

      //wait for the next sample cycle
      while(micros() - loopTimer < 4000);
      loopTimer = micros();
    }
    else
    {
      i--;
    }
  }
  calibX = calibX / MPU_CALIBRATE_READING_NUM;
  calibY = calibY / MPU_CALIBRATE_READING_NUM;
  calibZ = calibZ / MPU_CALIBRATE_READING_NUM;

  Serial.print("x: ");
  Serial.print(calibX);
  Serial.print("y: ");
  Serial.print(calibY);
  Serial.print("z: ");
  Serial.println(calibZ);

  Serial.println("Calibration Done.");
  
  digitalWrite(STATUS_PIN, LOW);
}


