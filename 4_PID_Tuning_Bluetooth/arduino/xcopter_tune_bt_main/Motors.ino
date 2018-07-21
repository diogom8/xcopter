int motors[4] = {MOTOR_0,MOTOR_1,MOTOR_2,MOTOR_3};

void set_motors(){
	int corrected_speed = 0;
  int prev_speed_motor = 0;
	int dif_speed = 0;
	for(int i=0;i<4;i++){
    prev_speed_motor = mSpeedSat[i];
		dif_speed = mSpeed[i] - prev_speed_motor;
		if(dif_speed > 10)
			corrected_speed = min(prev_speed_motor + 10,MOTOR_MAX_LEVEL);
		else if(dif_speed < -10)
			corrected_speed = max(prev_speed_motor - 10,MOTOR_MIN_LEVEL);
		else
			corrected_speed = constrain(mSpeed[i],MOTOR_MIN_LEVEL,MOTOR_MAX_LEVEL);
		
		mSpeedSat[i] = corrected_speed;
   
		analogWrite(motors[i],corrected_speed);
	}
}



void calibrate_motors(){
	for(int i=0;i<4;i++){
		pinMode(motors[i],OUTPUT);//init
		analogWrite(motors[i],MOTOR_MAX_LEVEL_CALIBRATE);
	}
        delay(4000);
	for(int i=0;i<4;i++){
		analogWrite(motors[i],MOTOR_MIN_LEVEL);
	}
        delay(5000);
        
}

void arm_motors(){
	for(int i=0;i<4;i++){
		int input = MOTOR_MIN_LEVEL;
		while(input<MOTOR_ARM_LEVEL){
                  input++;
		  analogWrite(motors[i], input);
                  delay(100);
                }
                while(input>MOTOR_MIN_LEVEL){
                  input--;
		  analogWrite(motors[i], input);
                  delay(100);
                }
	}
}

