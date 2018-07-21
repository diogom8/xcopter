void ControlPID(){


  /* Lift Control */
  float lift_spd = MOTOR_MIN_LEVEL; //initialization
  float roll_spd = 0;
  float pitch_spd = 0;
  float r_spd = 0;

  if (ref_lift > 1100){
    lift_spd = constrain(map(ref_lift,RxLift_MIN,RxLift_MAX,MOTOR_MIN_LEVEL,MOTOR_MAX_LEVEL),MOTOR_MIN_LEVEL,MOTOR_MAX_LEVEL);


    /*Roll Control*/
    float err_phi = ref_phi-phi;
    float P_roll =kp*err_phi;
    I_roll =(float)(I_roll + ai*err_phi_prev);
    D_roll =(float)(ad*D_roll + bd*(phi - phi_prev));

    err_phi_prev = err_phi;
    phi_prev = phi;

    roll_spd = P_roll+I_roll+D_roll;

    /*Pitch Control*/
    float err_theta = ref_theta-theta;
    float P_pitch = kp*err_theta;
    I_pitch =(float)(I_pitch + ai*err_theta_prev);
    D_pitch =(float)(ad*D_pitch + bd*(theta - theta_prev));

    err_theta_prev = err_theta;
    theta_prev = theta;

    pitch_spd = P_pitch+I_pitch+D_pitch;

    /*Yaw Rate Control*/
    float err_r = ref_r-r;
    float P_r = (float)(kp_r*err_r);

    r_spd = P_r;
  }
  /*Motors Speed*/
  mSpeed[0] = lift_spd + roll_spd + pitch_spd + r_spd;
  mSpeed[1] = lift_spd + roll_spd - pitch_spd - r_spd;	
  mSpeed[2] = lift_spd - roll_spd - pitch_spd + r_spd;
  mSpeed[3] = lift_spd - roll_spd + pitch_spd - r_spd;
  
  /*Send Motors Command*/
  set_motors();	
}
