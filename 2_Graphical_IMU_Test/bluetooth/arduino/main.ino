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
#include <SoftwareSerial.h>

#define FRQ				100.0//Control loop frequency 100Hz
#define T_LOOP			(1/FRQ)//Control loop period 0.01s

#define FRQ_SEND		10.0//Send loop frequency 100Hz
#define T_LOOP_SEND		(1/FRQ_SEND)//Send loop period 0.01s

#define RAD2DEG			180/M_PI	
		

#define MPU6050_ADD		0x68
#define PWR_MGMT_1		0X6B
#define ACCEL_XOUT_H	0x3B
#define ACCEL_CONFIG	0x1C
#define GYRO_XOUT_H		0x43
#define GYRO_CONFIG		0x1B

#define ACCEL_CONV		16384//+- 2g
#define GYRO_CONV		131//+- 250 degree/s
#define G_LED 13
//Filter Gains
//float K1 = 1/100;
//float K2 = 1/1000;
float K1 = 0.02;
float K2 = 0.0;
float last_dt = 0;

float phi = 0, phi_acc = 0, p = 0, sp = 0;
float theta = 0, theta_acc = 0, q = 0, sq = 0;
float sensors[6] = {0,0,0,0,0,0};
float sensors_raw[6] = {0,0,0,0,0,0};
float sensors_cal[6] = {0,0,0,0,0,0};


//Time
unsigned long t_prev = 0;
unsigned long t_prev_send = 0;

SoftwareSerial BTSerial(11, 12); // RX, TX

	
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

  	BTSerial.begin(9600);

	delay(100);
	pinMode(G_LED,OUTPUT);
    digitalWrite(G_LED,HIGH);
	/*Calibration*/
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
 	
	digitalWrite(G_LED,LOW);
	t_prev = millis(); 
	
		
}

void loop()
{
	//Compute ElapsedTime
	unsigned long t_now = millis();
	float dt = (t_now - t_prev)/1000.0;
	float dt_send = (t_now - t_prev_send)/1000.0;
	
	if(dt >= T_LOOP)
	{
		//Save ElapsedTime
		t_prev = t_now;
		last_dt = dt;
		//Read Accelerometer
		read_sensor(MPU6050_ADD,ACCEL_XOUT_H,6,sensors_raw,ACCEL_CONV);
		//Read Gyro
		read_sensor(MPU6050_ADD,GYRO_XOUT_H,6,&sensors_raw[3],GYRO_CONV);
		
		for(int j=0;j<6;j++)
		{
			sensors[j] = sensors_raw[j]-sensors_cal[j]; 	
		}
		
		//Compute Attitude
		//Roll
		phi_acc = RAD2DEG*atan(sensors[1]/(1+sensors[2]));
		p = sensors[3];
			//Roll Filter
			float p_c = p + sp;
			float phi_gy = phi + p_c*dt;
			phi =  phi_gy + K1*(phi_acc - phi_gy);
			sp = sp + K2*(phi_acc - p);
			
		//Pitch
		theta_acc = RAD2DEG*atan(sensors[0]/sqrt(pow(sensors[1],2) + pow(1+sensors[2],2)));
		q = -sensors[4];
			//Pitch Filter
			float q_c = q + sq;
			float theta_gy = theta + q_c*dt;
			theta =  theta_gy + K1*(theta_acc - theta_gy);
			sq = sq + K2*(theta_acc - q);
	}
	delayMicroseconds(50);	
		
	if(dt_send >= T_LOOP_SEND)
	{
		//Save ElapsedTime
		t_prev_send = t_now;	
		//Send data by serial
		BTSerial.print(F("# "));//StartFlag
		//Serial.print(last_dt,3);//Time
		//Serial.print(F(" "));
		BTSerial.print(phi,2);//Sensors
		BTSerial.print(F(" "));
		BTSerial.print(theta,2);//Sensors
		BTSerial.println(F(""));
	}
	//delay(1000);
		
		
		
}
		
