import processing.serial.*;

PrintWriter output;
Serial myPort;
int portIndex = 0;
String inString;  // Input string from serial port
int lf = 10;      // ASCII linefeed


int FRQ = 100;//Control loop frequency 100Hz
int LogTime = 5;// time to log in seconds
int ITERATIONS = LogTime*FRQ;
int iteration = 0;//data iterator
String[][] data = new String[ITERATIONS][7];

void delay(int delay)
{
  int time = millis();
  while(millis() - time <= delay);
}

void setup()
{
  //noStroke();noFill();
  myPort = new Serial(this, Serial.list()[0], 115200);
  myPort.clear();
  myPort.bufferUntil(lf);
  println("Performing readings, wait about " + LogTime  + "sec");
  
}

void draw()
{
    
  if(iteration == ITERATIONS)
  {
    myPort.stop();
    println("Iterations: " + iteration + ". Let's write it to a file...");
    writeData();
    println("Closing!");
    exit();
  }
   
}

void writeData()
{
  output = createWriter("SensorData_5s.txt");
  
  for(int i = 0; i< ITERATIONS; i++)
  {
     output.print(data[i][0] + " " +  data[i][1] + " " + data[i][2] + " " + data[i][3] + " " + data[i][4] + " " + data[i][5] + " " + data[i][6]);
     
     
  }
  
  output.close();
  
}

void serialEvent(Serial p) {
 
  inString = p.readString();
  try {
    // Parse the data
    String[] dataString = split(inString, " ");
    //println(dataString[0] + " " + dataString[0].equals("#"));
    if(dataString[0].equals("#"))
    {
      
      for (int i = 0; i < (dataString.length-1); i++) {
        data[iteration][i] = dataString[i+1];
      }
     iteration++;
     
     //println("Here" + iteration);
    } 
    
  } catch (Exception e) {
      //println("Caught Exception");
      println(e);println(iteration);
  }
 
}
void keyPressed()
{
  if(key == 'E')
    exit(); 
}
