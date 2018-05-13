int motors[4] = {MOTOR_0,MOTOR_1,MOTOR_2,MOTOR_3};
int prev_speed_motors[4] = {0,0,0,0};

void set_motors(int speed_motors[4]){
	int corrected_speed = 0;
	int dif_speed = 0;
	for(int i=0;i<4;i++){
		dif_speed = speed_motors[i] - prev_speed_motors[i];
		if(dif_speed > 10)
			corrected_speed = min(prev_speed_motors[i] + 10,MOTOR_MAX_LEVEL);
		else if(dif_speed < -10)
			corrected_speed = max(prev_speed_motors[i] - 10,MOTOR_MIN_LEVEL);
		else
			corrected_speed = constrain(speed_motors[i],MOTOR_MIN_LEVEL,MOTOR_MAX_LEVEL);
		
		prev_speed_motors[i] = corrected_speed;
		
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

