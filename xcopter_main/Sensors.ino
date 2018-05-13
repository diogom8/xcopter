float sensors_raw[6] = {0,0,0,0,0,0};
float sensors_cal[6] = {0,0,0,0,0,0};

void read_sensor(int ADDRESS,int REGISTER,int n_bytes,float buffer[],float CONVERSION)
{
		unsigned long timer_read = millis();

                Wire.beginTransmission(ADDRESS);
		Wire.write(REGISTER);  // starting with register 
		Wire.endTransmission();
	
		Wire.requestFrom(ADDRESS,n_bytes,true);  // request a total of n_bytes
                
                
		while(Wire.available() != n_bytes && (millis()-timer_read)<=10);{}
                if(Wire.available() == n_bytes){
    		for(int j = 0;j<n_bytes/2;j++)
    		{
    			buffer[j] = (Wire.read()<<8|Wire.read())/CONVERSION;
    		}
                }
		
}

void calibrate_sensors(){

	//Sensors Initialization
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

}

void update_ControlVariables(){

	//read raw values
	read_sensor(MPU6050_ADD,ACCEL_XOUT_H,6,sensors_raw,ACCEL_CONV);
	read_sensor(MPU6050_ADD,GYRO_XOUT_H,6,&sensors_raw[3],GYRO_CONV);
	
	//compute phi (roll)
	float phi_acc = Rad2Deg*atan((sensors_raw[1]-sensors_cal[1])/(1+sensors_raw[2]-sensors_cal[2]));
	float p = sensors_raw[3]-sensors_cal[3];
	/*Filter*/
	float p_c = p + dp;
	float phi_gy = phi + p_c*T_LOOP;
	phi = (float)(phi_gy + K1*(phi_acc-phi_gy));
	//dp = (float)(dp + K2*(phi_acc-phi_gy));

        
	//compute theta (pitch)
	float theta_acc = Rad2Deg*atan((sensors_raw[0]-sensors_cal[0])/sqrt(pow((sensors_raw[1]-sensors_cal[1]),2) + pow((1+sensors_raw[2]-sensors_cal[2]),2)));
	float q = -(sensors_raw[4]-sensors_cal[4]);
	/*Comp Filter*/
	float q_c = q + dq;
	float theta_gy = theta + q_c*T_LOOP;
	theta = (float)(theta_gy + K1*(theta_acc-theta_gy));
	//dq = (float)(dq + K2*(theta_acc-theta_gy));
	
	//compute r (yaw rate)
	float r_gy = sensors_raw[5]-sensors_cal[5];
	/*Mov Aver Filter*/
	r = (float)((r*WIN-r_WIN[point_WIN]+r_gy)/WIN);
	r_WIN[point_WIN] = r_gy;
	if(point_WIN == WIN-1)
		point_WIN = 0;
	else
		point_WIN++;
	

}



