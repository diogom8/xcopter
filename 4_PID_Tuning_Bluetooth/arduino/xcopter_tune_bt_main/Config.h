//***********LED CONFIG**************

//LED Pin
#define R_LED	13
#define	G_LED	10	  
//*************************************
//***********MOTORS CONFIG**************

//Motor PWM Levels
#define MOTOR_MIN_LEVEL  	                100
#define MOTOR_MAX_LEVEL  	                215
#define MOTOR_MAX_LEVEL_CALIBRATE		250 //theoretical max is 250
#define MOTOR_ARM_LEVEL		                150
#define ARM_TIME			         6000 //milliseconds



//Motor Pins
#define MOTOR_0  6
#define MOTOR_1  7
#define MOTOR_2  8
#define MOTOR_3  9
//*************************************
//***********SENSORS CONFIG**************
#define MPU6050_ADD		0x68
#define PWR_MGMT_1		0X6B
#define ACCEL_XOUT_H	0x3B
#define ACCEL_CONFIG	0x1C
#define GYRO_XOUT_H		0x43
#define GYRO_CONFIG		0x1B

#define ACCEL_CONV		16384//+- 2g
#define GYRO_CONV		131//+- 250 degree/s

//Complementary Filter Gains
#define K1		0.02
//#define K2		1/1000.0
#define K2	        0

//Moving average Filter (Yaw rate)
#define WIN		15


//other
#define Rad2Deg 180.0/M_PI
#define FRQ_LOOP      100.0//Control loop frequency (Hz)
#define T_LOOP	      (1/FRQ_LOOP)//Control loop period (seconds)
#define FRQ_RX	      10.0 //RX frequency (Hz)
#define T_RX          (1/FRQ_RX)//RX period (seconds)
#define FRQ_BT	      2.0 //BT communication frequency (Hz)
#define T_BT          (1/FRQ_BT)//BT communication period (seconds)
//***************************************
//***********RX CONFIG**************
#define RxLift_MAX 1990
#define RxLift_MIN 888
#define RX_INT_PIN_LIFT 2

#define RxRoll_MAX 1840
#define RxRoll_MIN 984
#define RX_INT_PIN_ROLL 3

#define RxPitch_MAX 1956
#define RxPitch_MIN 960
#define RX_INT_PIN_PITCH 18

#define RxYaw_MAX 2012
#define RxYaw_MIN 1064
#define RX_INT_PIN_YAW 19

#define TILT_MAX 10.0
#define TILT_MIN -TILT_MAX
#define YawRate_MAX 5.0
#define YawRate_MIN -YawRate_MAX
//***************************************
//***********CONTROL PID CONGIG**************

/*#define kp 0.100
#define ki 0.0
#define kd 0.0

#define N 10
#define ai ki*T_LOOP
#define ad kd/(kd+N*T_LOOP)
#define bd -kd/(kd+N*T_LOOP)

#define kp_r 0.1*/
//***************************************

