import controlP5.*;

ControlP5 cp5;
Toggle stopToggle;

float quadScale = 1.5;

void setup() {

    colorMode(RGB, 256);
    
    //Create window in white
    size(1200,600);
    background(255,255,255);
    
    //A. Title
    fill(255,0,0);ellipse(20,20,5,5);//reference point
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
    text("M4",-cos(PI/4.0)*quadScale*150,-sin(PI/4.0)*quadScale*150);
    text("M3",cos(PI/4.0)*quadScale*150,-sin(PI/4.0)*quadScale*150);
    text("M2",cos(PI/4.0)*quadScale*150,sin(PI/4.0)*quadScale*150);
    text("M1",-cos(PI/4.0)*quadScale*150,sin(PI/4.0)*quadScale*150);
    popMatrix();
    
    cp5 = new ControlP5(this);
    
    stopToggle = cp5.addToggle("stopValue")
     .setMode(CENTER)
     .setPosition(800,50)
     .setSize(200,50)
     .setColorActive(color(255,0,0))      //Active
     .setColorForeground(color(255,0,0)) //Mouse Over
     .setColorBackground(color(224,224,224)) //Not Active
     .setValue(false)
     .setCaptionLabel("RUNNING")
     .setColorCaptionLabel(0)
     ;

  
}

void draw()  {
  
   
  
}
