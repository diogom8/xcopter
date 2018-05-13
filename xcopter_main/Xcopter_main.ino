#include <Boards.h>
#include <Firmata.h>

#include <Wire.h>
#include <math.h>
#include "Config.h"

//Globals
/*control varibles*/
float phi = 0.0, theta = 0.0, r = 0.0;

/*References*/
float ref_phi = 0.0, ref_theta = 0.0, ref_r = 0.0; //Converted references for control
volatile unsigned int prev_ref_lift = 0,ref_lift = 0,ref_roll = 0,ref_pitch = 0,ref_yaw = 0; //References from the Radio (pilot)
volatile unsigned long t_ref_lift  = 0, t_ref_roll = 0, t_ref_pitch = 0,t_ref_yaw = 0;
/*Filters*/
float dp = 0.0, dq = 0.0;
float r_WIN[WIN] = {0};
int point_WIN = 0;
/*Control*/
float I_roll = 0.0, D_roll = 0.0;
float I_pitch = 0.0,D_pitch = 0.0;
float phi_prev = 0.0,err_phi_prev = 0.0;
float theta_prev = 0.0,err_theta_prev = 0.0;
/*motors*/
int m0_spd = 0;
int m1_spd = 0; 
int m2_spd = 0;
int m3_spd = 0;


/*RX*/
int RxLossCount = 0;
boolean RxLoss_flag = false;

unsigned long timer_control = 0;
unsigned long timer_rx = 0;
unsigned long timer_print = 0;
float max_t = 0;
unsigned long proof_t = 0;
int ZeromSpeed[4] = {MOTOR_MIN_LEVEL};

/*Control Tuning*/
float kp = 0.0;
float ki = 0.0;
float kd = 0.0;
float kp_r = 0.0;

float N = 10;
float ai = ki*T_LOOP;
float ad = kd/(kd+N*T_LOOP);
float bd = -kd/(kd+N*T_LOOP);


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    leds_init();
    led(G_LED,HIGH,0);
    //calibrate_motors();
    //arm_motors();
    led(R_LED,HIGH,2); //blink*/
    calibrate_sensors();
    led(R_LED,HIGH,2); //blink
    rx_init();
    led(G_LED,LOW,0);
    timer_control = millis();
    timer_rx = millis();
    timer_print = millis();

}

void loop() {
// put your main code here, to run repeatedly:
    
    float dt_loop = (millis()-timer_control)/1000.0;
    
          
    if( dt_loop >= T_LOOP)
    {
      timer_control = millis();
      update_ControlVariables();
      
      if(RxLoss_flag == false){
        ControlPID();
      }
      else
      {
        set_motors(ZeromSpeed);
                    
       }
      
     }
     
    if((millis()-timer_rx)/1000.0 >= T_RX)
    {
      timer_rx = millis();
      update_ControlReferences();
      RxLoss_flag = CheckRxLoss();
      
    }
    
    
    //Tuning
    if(Serial.available() > 0){
      while(Serial.available()>0){Serial.read();delay(25);} //clean buffer
      led(G_LED,HIGH,0);
      set_motors(ZeromSpeed);delay(100);
      Serial.println("What to adjust: P(1) - I(2) - D(3) ");while(Serial.available()<=0){delay(25);}
      int rx_byte = Serial.read()-48;delay(25);Serial.println(rx_byte);
      while(Serial.available()>0){Serial.read();delay(25);} //clean buffer
      int i = 0;
      switch (rx_byte) {
        case 1:
              Serial.print("P gain: ");Serial.println(kp,6);/*AQUI*/
              Serial.println("new gain?");while(Serial.available()<=0){delay(25);}
              rx_byte = Serial.read();
              kp = 0.0;/*AQUI*/
              while( rx_byte != '\n')
              {
                 if( rx_byte >= '0' && rx_byte <= '9'){
                  
                  kp += (float)(rx_byte-48)/pow(10,i);/*AQUI*/
                  i++;
                 }
                  rx_byte = Serial.read();delay(25);
                  
               }
               Serial.println(kp,6);/*AQUI*/
               break;
         case 2:
              Serial.print("I gain: ");Serial.println(ki,6);/*AQUI*/
              Serial.println("new gain?");while(Serial.available()<=0){delay(25);}
              rx_byte = Serial.read();
              ki = 0.0;/*AQUI*/
              while( rx_byte != '\n')
              {
                 if( rx_byte >= '0' && rx_byte <= '9'){
                  
                  ki += (float)(rx_byte-48)/pow(10,i);/*AQUI*/
                  i++;
                 }
                  rx_byte = Serial.read();delay(25);
                  
               }
               Serial.println(ki,6);/*AQUI*/
               break;
         case 3:
              Serial.print("D gain: ");Serial.println(kd,6);/*AQUI*/
              Serial.println("new gain?");while(Serial.available()<=0){delay(25);}
              rx_byte = Serial.read();
              kd = 0.0;/*AQUI*/
              while( rx_byte != '\n')
              {
                 if( rx_byte >= '0' && rx_byte <= '9'){
                  
                  kd += (float)(rx_byte-48)/pow(10,i);/*AQUI*/
                  i++;
                 }
                  rx_byte = Serial.read();delay(25);
                  
               }
               Serial.println(kd,6);/*AQUI*/
               break;
          default:
            Serial.println("not option");
            break;
       }
       ai = (float)(ki*T_LOOP);
       ad = (float)(kd/(kd+N*T_LOOP));
       bd = (float)(-kd/(kd+N*T_LOOP));
       I_roll = 0.0;
       D_roll = 0.0;
       I_pitch = 0.0;
       D_pitch = 0.0;        
       while(ref_lift > 950){led(R_LED,HIGH,2);}//blink
       led(G_LED,LOW,0);
       }
    //proof_t = millis()-proof_t;
    
    /*if(proof_t > max_t)
      max_t = proof_t;*/
    
    if(millis() - timer_print > 500){
       Serial.print("m0: ");Serial.print(m0_spd);Serial.print(" ");
       Serial.print("m1: ");Serial.print(m1_spd);Serial.print(" ");
       Serial.print("m2: ");Serial.print(m2_spd);Serial.print(" ");
       Serial.print("m3: ");Serial.print(m3_spd);Serial.print(" | ");
       Serial.print("phi: ");Serial.print(phi);Serial.print(" ");
       Serial.print("ref_phi: ");Serial.print(ref_phi);Serial.print(" | ");
       Serial.print("theta: ");Serial.print(theta);Serial.print(" ");
       Serial.print("ref_theta: ");Serial.print(ref_theta);Serial.println(" ");
       //Serial.print("time: ");Serial.println(max_t);
       timer_print = millis();
     }
//led(G_LED,HIGH,1);
}
