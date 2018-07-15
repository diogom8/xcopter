#include <Boards.h>
#include <Firmata.h>

#include <Wire.h>
#include <math.h>
#include "Config.h"

//Needed for bluetooth communication
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(11, 12); // RX, TX (IMPORTANT: THIS IS THE RX,TX pair of ARDUINO BOARD. SO THE BLUETOOTH PAIR WILL BE THE OPPOSITE!)

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
unsigned long timer_bt = 0;
float max_t = 0;
unsigned long proof_t = 0;
int ZeromSpeed[4] = {MOTOR_MIN_LEVEL};

/*Control Tuning*/
float kp = 2.0;
float ki = 0.0;
float kd = 0.0;
float kp_r = 0.0;

float N = 10;
float ai = ki*T_LOOP;
float ad = kd/(kd+N*T_LOOP);
float bd = -kd/(kd+N*T_LOOP);

/* Read data via Bluetooh */
boolean stop_bt = false;
boolean setGains_bt = false;
String readString;

void setup() {
    // setup serial
    //Serial.begin(115200);
    BTSerial.begin(9600);

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
    timer_bt = millis();

}

void loop() {
// put your main code here, to run repeatedly:
    
    float dt_loop = (millis()-timer_control)/1000.0;
    
          
    if( dt_loop >= T_LOOP)
    {
      timer_control = millis();
      update_ControlVariables();
      
      if(RxLoss_flag == false and stop_bt == false){
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
    
    
    //Send data
    if((millis()-timer_bt)/1000.0 >= T_BT)
    {
      //digitalWrite(R_LED,!digitalRead(R_LED));
      timer_bt = millis();

      BTSerial.print(F("# "));//StartFlag
      BTSerial.print(m0_spd);//motor 0 speed
      BTSerial.print(F(" "));
      BTSerial.print(m1_spd);//motor 1 speed
      BTSerial.print(F(" "));
      BTSerial.print(m2_spd);//motor 2 speed
      BTSerial.print(F(" "));
      BTSerial.print(m3_spd);//motor 3 speed
      BTSerial.print(F(" "));
      BTSerial.print(kp,2);//p gain
      BTSerial.print(F(" "));
      BTSerial.print(ki,2);//i gain
      BTSerial.print(F(" "));
      BTSerial.print(kd,2);//d gain
      BTSerial.print(F(" "));
      BTSerial.print(phi,2);//phi (roll)
      BTSerial.print(F(" "));
      BTSerial.print(ref_phi,2);//phi reference (pitch)
      BTSerial.print(F(" "));
      BTSerial.print(theta,2);//theta (pitch)
      BTSerial.print(F(" "));
      BTSerial.print(ref_theta,2);//theta reference (pitch)
      BTSerial.println(F(""));
    }

    //Read data (the data we receive is 18 bytes long + carriage return)
    if(BTSerial.available())
    {
      char c = BTSerial.read();
      if(c == '*') //Ending character
      { 
        if(readString.startsWith("#"))//valid data
        { 
          //Emergency stop
          if(readString.substring(1, 2)  == "1"){
            stop_bt = true;
          }
          else{
            stop_bt = false;
          }
          //Set gains
          if(readString.substring(2, 3)  == "1"){
            setGains_bt = true;
            int ind1 = readString.indexOf(' ');  //finds location of first Space
            kp = readString.substring(3, ind1).toFloat();
            int ind2 = readString.indexOf(' ',ind1+1);  //finds location of second Space
            ki = readString.substring(ind1+1, ind2).toFloat();
            int ind3 = readString.indexOf('*',ind2+1);  //finds location of termination char *
            kd = readString.substring(ind2+1, ind3).toFloat();
          }
          else{
            setGains_bt = false;
          }
          //Clean buffer
          while(Serial.available()>0){Serial.read();}
          readString=""; //clears variable for new input
          
        }
        digitalWrite(R_LED,!digitalRead(R_LED));
      }
      else{
        readString += c; //makes the string readString
      }
    }
}
