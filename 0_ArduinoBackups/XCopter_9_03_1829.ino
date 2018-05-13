/*
 * XCopter.ino
 *
 * Created: 2/22/2015 5:11:45 PM
 * Author: Diogo Monteiro
 */

/*Transformation
-sin(theta)
cos(theta)*sin(phi)
cos(phi)*cos(theta)*/ 
#include <Wire.h>
#include <math.h>
#define RAD2DEG			180/M_PI	
#define ITERATION2SEND	1000			

#define MPU6050_ADD		0x68
#define PWR_MGMT_1		0X6B
#define ACCEL_XOUT_H	0x3B
#define ACCEL_CONFIG	0x1C
#define GYRO_XOUT_H		0x43
#define GYRO_CONFIG		0x1B

#define ACCEL_CONV		16384//+- 2g
#define GYRO_CONV		131//+- 250 degree/s


float psi = 0,theta = 0,phi = 0;
float sensors_raw[6] = {0,0,0,0,0,0};
float sensors_cal[6] = {0,0,0,0,0,0};
unsigned long t_prev = 0;

int iteration = 0;


	
void read_sensor(int ADDRESS,int REGISTER,int n_bytes,float buffer[],float CONVERSION)
{
		Wire.beginTransmission(ADDRESS);
		Wire.write(REGISTER);  // starting with register 
		Wire.endTransmission();
	
		Wire.requestFrom(ADDRESS,n_bytes,true);  // request a total of n_bytes
		while(Wire.available() != n_bytes);{}
		for(int j = 0;j<n_bytes/2;j++)
		{
			buffer[j] = (Wire.read()<<8|Wire.read())/CONVERSION;
		}
		
}

void setup()
{
	
	Wire.begin();
	Wire.beginTransmission(MPU6050_ADD);
	Wire.write(PWR_MGMT_1);  // PWR_MGMT_1 register
	Wire.write(0);     // set to zero (wakes up the MPU-6050)
	Wire.endTransmission();
	
	Wire.beginTransmission(MPU6050_ADD);
	Wire.write(ACCEL_CONFIG);  
	Wire.write(0);     // set sensitivity to +-2g
	Wire.endTransmission();
	
	Wire.beginTransmission(MPU6050_ADD);
	Wire.write(GYRO_CONFIG);
	Wire.write(0);     // set sensitivity to +-250 degree/s
	Wire.endTransmission();

	Serial.begin(115200);
	delay(100);
	/*Calibration*/
	//Serial.println("Calibrating...");
	for(int i = 0; i < 160;i++)
	{
		read_sensor(MPU6050_ADD,ACCEL_XOUT_H,6,sensors_raw,ACCEL_CONV);
		sensors_cal[0] += sensors_raw[0];
		sensors_cal[1] += sensors_raw[1];
		sensors_cal[2] += sensors_raw[2];
		read_sensor(MPU6050_ADD,GYRO_XOUT_H,6,&sensors_raw[3],GYRO_CONV);
		sensors_cal[3] += sensors_raw[3];
		sensors_cal[4] += sensors_raw[4];
		sensors_cal[5] += sensors_raw[5];
		delay(10);
	}
 	sensors_cal[0] /= 160;
 	sensors_cal[1] /= 160;
 	sensors_cal[2] /= 160;
	sensors_cal[3] /= 160;
	sensors_cal[4] /= 160;
	sensors_cal[5] /= 160; 
 	//Serial.println("Calibration complete!");
	
	int time2start = 3;//in seconds
	Serial.print("Countdown in seconds: ");
	while(time2start > 0)
	{
		Serial.print(time2start);
		Serial.print("...");
		delay(1000);
		time2start--;	
	}
	Serial.println("");
	t_prev = millis(); 
	
		
}

void loop()
{
	
	if(iteration < ITERATION2SEND)
	{
		//Read Accelerometer
		read_sensor(MPU6050_ADD,ACCEL_XOUT_H,6,sensors_raw,ACCEL_CONV);
		//Read Gyro
		read_sensor(MPU6050_ADD,GYRO_XOUT_H,6,&sensors_raw[3],GYRO_CONV);
		//Compute ElapsedTime
		unsigned long t_now = millis();
		float dt = (t_now - t_prev)/1000.0;
		t_prev = t_now;
	
		
		//Send data by serial
		Serial.print(F("# "));//StartFlag
		Serial.print(dt,3);//Time
		for(int j=0;j<6;j++)
		{
			Serial.print(F(" "));
			Serial.print(sensors_raw[j]-sensors_cal[j],2);//Sensors
			//Serial.print("Sensor"+(i+1));
		}
			
		Serial.println(F(""));
	
		
		
		iteration++;
	}
		
	delayMicroseconds(50);
	 
	
	
	//Esta parte era para apresentar o ângulos(com base no acc) já com a matriz de rotação aplicada(a ser feito no matlab para teste do Kalman)
	/*Serial.print("theta = "); 
	Serial.print(asin((sensors_raw[0]-sensors_cal[0]))*RAD2DEG);
	Serial.print(" | phi = "); 
	Serial.println(atan((sensors_raw[1]-sensors_cal[1])/(sensors_raw[2]+(1-sensors_cal[2])))*RAD2DEG);*/
	
	
	
}
