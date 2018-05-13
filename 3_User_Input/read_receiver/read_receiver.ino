//int motor = 6;
//int RedLed = 11;
int GreenLed = 12;
unsigned long OnDuration;
//Time
unsigned long FullDuration = 0;

#define RX_INT_PIN_THROTTLE 2
volatile int rxPrev = 0; //variable that will contain the previous time
volatile int rxVal = 0; //variable that will contain the current throttle pulse length
int MAX = 0;
int MIN = 2000;

void rxGoesHigh(){
/* As soon as we get into this function we measure the current time (with micros) 
and put that value in our variable rxPrev. After that we re-attach the same interrupt 
but this time we want it to fire when the pin goes low "FALLING". When that happens it 
will call the function rxGoesLow */
  rxPrev=micros();
  attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_THROTTLE),rxGoesLow,FALLING);
}
void rxGoesLow(){
/* As soon as we get into this function we measure the current time (with micros) and 
subtract the previous time (rxPrev). By doing that we will get the length of the pulse,
from start to the end. We end this process by re-attaching the throttle pin to a RISING 
interrupt, which will repeat the process again... */
  rxVal=micros()-rxPrev;  
  attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_THROTTLE),rxGoesHigh,RISING);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Start");
  //pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(RX_INT_PIN_THROTTLE),rxGoesHigh,RISING);
  //pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
}

void loop()
{
  
  /*FullDuration = millis();
  OnDuration = pulseIn(pin, HIGH);
  FullDuration = millis() - FullDuration;*/
  
  
  
  
  Serial.println(rxVal);
  
  if(rxVal > MAX){
    MAX= rxVal;
    Serial.print("MAX On duration: ");Serial.println(MAX);
  }
  else if(rxVal < MIN && rxVal > 0){
    MIN = rxVal;
    Serial.print("  MIN On duration: ");Serial.println(MIN);
  }
  //Serial.print("  Full Duration: ");Serial.println(FullDuration);
  
  
  //analogWrite(RedLed, map(rxVal,888,2000,0,255));
  analogWrite(GreenLed, map(rxVal,888,2000,0,255));
  
  delay(30);
}
