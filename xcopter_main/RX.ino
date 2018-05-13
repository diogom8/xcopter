void rx_init(){

	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_LIFT),rxLift_GoHigh,RISING);
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_ROLL),rxRoll_GoHigh,RISING);
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_PITCH),rxPitch_GoHigh,RISING);
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_YAW),rxYaw_GoHigh,RISING);

        delay(1000);
        while(ref_lift > 950){led(R_LED,HIGH,2);}//blink
}

boolean CheckRxLoss(){
  
    if(ref_lift == prev_ref_lift){
      RxLossCount = min(RxLossCount++,30);
      }
    else
      RxLossCount = max(RxLossCount--,-10);
      
    prev_ref_lift = ref_lift;
      
      if(RxLossCount >= 28){
        led(G_LED,HIGH,0);
        return true;
        }
      else{
        led(G_LED,LOW,0);
        return false;
        }
}
void update_ControlReferences(){

	if (ref_roll < 1550 && ref_roll > 1350)
          ref_phi = 0;
        else
          ref_phi = constrain(map(ref_roll,RxRoll_MIN,RxRoll_MAX,TILT_MIN,TILT_MAX),TILT_MIN,TILT_MAX);
          
        if (ref_pitch < 1600 && ref_pitch > 1400)
          ref_theta = 0;
        else
          ref_theta = constrain(map(ref_pitch,RxPitch_MIN,RxPitch_MAX,TILT_MIN,TILT_MAX),TILT_MIN,TILT_MAX);
          
        if (ref_yaw < 1650 && ref_yaw > 1450)
          ref_r = 0;
        else  
	  ref_r = constrain(map(ref_yaw,RxYaw_MIN,RxYaw_MAX,YawRate_MIN,YawRate_MAX),YawRate_MIN,YawRate_MAX);
}


void rxLift_GoHigh(){

	t_ref_lift = micros();
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_LIFT),rxLift_GoLow,FALLING);

}

void rxLift_GoLow(){
	ref_lift = micros() - t_ref_lift;
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_LIFT),rxLift_GoHigh,RISING);
}

void rxRoll_GoHigh(){

	t_ref_roll = micros();
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_ROLL),rxRoll_GoLow,FALLING);

}

void rxRoll_GoLow(){
	ref_roll = micros() - t_ref_roll;
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_ROLL),rxRoll_GoHigh,RISING);
}

void rxPitch_GoHigh(){

	t_ref_pitch = micros();
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_PITCH),rxPitch_GoLow,FALLING);

}

void rxPitch_GoLow(){
	ref_pitch = micros() - t_ref_pitch;
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_PITCH),rxPitch_GoHigh,RISING);
}

void rxYaw_GoHigh(){

	t_ref_yaw = micros();
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_YAW),rxYaw_GoLow,FALLING);

}

void rxYaw_GoLow(){
	ref_yaw = micros() - t_ref_yaw;
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_YAW),rxYaw_GoHigh,RISING);
}

