#define RX_INT_PIN 2
#define PWM_OUT_PIN 6

volatile int rxPrev = 0;
volatile int rxVal = 0;
int prevrxVal = 0;
int inputUser = 0;
char incomingByte;

void flushReceive(){
  while(Serial.available())
    Serial.read();
}

void rxGoesHigh(){
  rxPrev = micros();
  attachInterrupt(digitalPinToInterrupt(RX_INT_PIN),rxGoesLow,FALLING);
 }
void rxGoesLow(){
	rxVal = micros() - rxPrev;
	attachInterrupt(digitalPinToInterrupt(RX_INT_PIN),rxGoesHigh,RISING);	
}
void setup() {
  // put your setup code here, to run once:
  pinMode(PWM_OUT_PIN,OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(RX_INT_PIN),rxGoesHigh,RISING);

}

void loop() {
  // put your main code here, to run repeatedly:
  
	if(Serial.available() > 0 && Serial.read() == '.')
	{
	
		
		detachInterrupt(digitalPinToInterrupt(RX_INT_PIN));
		Serial.print("User input: ");
		flushReceive();
		incomingByte = Serial.read();
		//Serial.println( (int) incomingByte);
		inputUser = 0;
		while(incomingByte != '.')
		{
                        
			if(incomingByte != -1)
			{
				//Serial.println((int) incomingByte);
				inputUser *= 10;
				inputUser = (incomingByte-48) + inputUser;
			}
			incomingByte = Serial.read();
                       //Serial.print((int) incomingByte);Serial.print(" | " );Serial.println(incomingByte != '\n');
                        //delay(50);
		}
		Serial.println(inputUser);
        attachInterrupt(digitalPinToInterrupt(RX_INT_PIN),rxGoesHigh,RISING);
		analogWrite(PWM_OUT_PIN,inputUser);
    
	}
	
	if(prevrxVal != rxVal)

		Serial.println(rxVal);
		prevrxVal = rxVal;
	}
  

  
}
