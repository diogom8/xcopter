import processing.serial.*;

Serial  myPort;
int     lf = 10;       //ASCII linefeed
String  inString;      //String for testing serial communication
boolean sendData = false;
String stop_bt = "0";
String setGains_bt = "0";
String stringToSend;

import controlP5.*;
ControlP5 cp5;
Toggle stopToggle;
Textfield text_PGain, text_DGain, text_IGain;
Bang bang_SetGains;

float quadScale = 1.5;

int M0Val, M1Val, M2Val, M3Val;
float kp, ki, kd;
float phi, ref_phi, theta, ref_theta;

void setup() {

    colorMode(RGB, 256);
    
    //Create window in white
    size(1200,600);
    background(255,255,255);
    
    cp5 = new ControlP5(this);
    
    stopToggle = cp5.addToggle("stopValue")
     .setMode(ControlP5.DEFAULT)
     .setPosition(800,50)
     .setSize(200,50)
     .setColorActive(color(255,0,0))      //Active
     .setColorForeground(color(255,0,0)) //Mouse Over
     .setColorBackground(color(224,224,224)) //Not Active
     .setValue(false)
     .setCaptionLabel("RUNNING")
     .setColorCaptionLabel(0)
     ;
     
    //Textfields for controller gains
    text_PGain = cp5.addTextfield("P")
     .setPosition(800,300)
     .setSize(100,40)
     .setFont(createFont("arial",20))
     .setFocus(true) //not sure what this focus means
     .setCaptionLabel("P")
     .setColorCaptionLabel(0)
     ;
     
     text_IGain = cp5.addTextfield("I")
     .setPosition(800,400)
     .setSize(100,40)
     .setFont(createFont("arial",20))
     .setFocus(true) //not sure what this focus means
     .setCaptionLabel("I")
     .setColorCaptionLabel(0)
     ;
     
     text_DGain = cp5.addTextfield("D")
     .setPosition(800,500)
     .setSize(100,40)
     .setFont(createFont("arial",20))
     .setFocus(true) //not sure what this focus means
     .setCaptionLabel("D")
     .setColorCaptionLabel(0)
     ;
     
     //bang to send new controller values
     bang_SetGains = cp5.addBang("SET")
     .setPosition(1000,400)
     .setSize(80,40)
     ;
     bang_SetGains.getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);
     
    //Setup serial
    printArray(Serial.list());
    myPort = new Serial(this, "/dev/rfcomm0", 9600);
    myPort.clear();
    myPort.bufferUntil(lf);
}

void draw()  {
    background(255,255,255);
    drawStatics();
    //Motor Values text
    fill(0,0,0);textSize(22);
    textAlign(CENTER,CENTER);
    text(M0Val, -cos(PI/4.0)*quadScale*150 + 250, -sin(PI/4.0)*quadScale*150 + 300 + quadScale*25);
    text(M3Val, cos(PI/4.0)*quadScale*150 + 250, -sin(PI/4.0)*quadScale*150 + 300 + quadScale*25);
    text(M2Val, cos(PI/4.0)*quadScale*150 + 250, sin(PI/4.0)*quadScale*150 + 300 + quadScale*25);
    text(M1Val, -cos(PI/4.0)*quadScale*150 + 250, sin(PI/4.0)*quadScale*150 + 300 + quadScale*25);
    
    //Current controller gains
    fill(160,160,160);
    text(kp, text_PGain.getPosition()[0] + 150, text_PGain.getPosition()[1]);
    text(ki, text_IGain.getPosition()[0] + 150, text_IGain.getPosition()[1]);
    text(kd, text_DGain.getPosition()[0] + 150, text_DGain.getPosition()[1]);
    
    //Current attitude and set points
    textAlign(LEFT);fill(0,0,0);
    text("Pitch", 800, 150);
    text("Roll", 950, 150);
    fill(0,0,255);
    text((float) phi, 800, 175);
    text((float) theta, 950, 175);
    
    fill(160,160,160);textSize(18);
    text(ref_theta, 800, 200);
    text(ref_phi, 950, 200);
    
    //Send data
    if(sendData == true){sendData();sendData = false;}
  
}

public void SET() {
  //println(stopToggle.getValue());
  setGains_bt = "1";
  sendData = true;
}

void sendData() {
  String kp_new = text_PGain.getText();
  //print(kp_new);
  String ki_new = text_IGain.getText();
  String kd_new = text_DGain.getText();
  
 stringToSend = '#' + stop_bt + setGains_bt + kp_new + ' ' + ki_new + ' ' + kd_new + '*';
 println(stringToSend);
 myPort.write(stringToSend);
}

void serialEvent(Serial p) {

    
  inString = p.readString();
  try {
    // Parse the data
    String[] dataString = split(inString, " ");
    if(dataString[0].equals("#") && dataString.length == 12)
    {
      println(inString);
      M0Val = int(dataString[1]);
      M1Val = int(dataString[2]);
      M2Val = int(dataString[3]);
      M3Val = int(dataString[4]);
      kp = float(dataString[5]);
      ki = float(dataString[6]);
      kd = float(dataString[7]);
      phi = float(dataString[8]);
      ref_phi = float(dataString[9]);
      theta = float(dataString[10]);
      ref_theta = float(dataString[11]);
    }
    else {
      //Clean buffer
      println("Clean Buffer");
      while(p.available()>0){p.read();}
    }
  } catch (Exception e) {
      println("Caught Exception");
  }
}

void drawStatics() {
 
    //A. Title
    //fill(255,0,0);ellipse(20,20,5,5);//reference point
    fill(0,0,0);
    textAlign(LEFT,TOP);
    textSize(35);
    text("XCOPTER - PID Tuning UI",20,20);
    
    //B. Draw quad-copter X configuration
    pushMatrix();
    translate(250,300); //Center of the quadcopter
    fill(0,0,0);
    ellipse(0, 0, quadScale*60, quadScale*60); //Middle circle
    
    //Arms
    rectMode(CENTER);
    rotate(PI/4.0); //Positive -> Clockwise
    rect(0, 0, quadScale*10, quadScale*300);
    rotate(PI/2.0); //Positive -> Clockwise
    rect(0, 0, quadScale*10, quadScale*300);
    
    //Motors
    fill(255,255,255);
    ellipse(0,150*quadScale,quadScale*25,quadScale*25);ellipse(0,-150*quadScale,quadScale*25,quadScale*25);
    rotate(PI/2.0); //Positive -> Clockwise
    ellipse(0,150*quadScale,quadScale*25,quadScale*25);ellipse(0,-150*quadScale,quadScale*25,quadScale*25);
    
    //Motors tags
    rotate(3.0*PI/4.0); //Original position
    fill(255,190,0);textSize(15);
    textAlign(CENTER,CENTER);
    text("M0",-cos(PI/4.0)*quadScale*150,-sin(PI/4.0)*quadScale*150);
    text("M3",cos(PI/4.0)*quadScale*150,-sin(PI/4.0)*quadScale*150);
    text("M2",cos(PI/4.0)*quadScale*150,sin(PI/4.0)*quadScale*150);
    text("M1",-cos(PI/4.0)*quadScale*150,sin(PI/4.0)*quadScale*150);
    popMatrix();

}
