void leds_init(){
	//Red LED
	pinMode(R_LED,OUTPUT);
	digitalWrite(R_LED,LOW); 
	//Green LED 
	pinMode(G_LED,OUTPUT);
	digitalWrite(G_LED,LOW); 
}
  
void led(int LED_PIN,byte stat,int blink_times){
	if(blink_times <= 0) 
		digitalWrite(LED_PIN,stat);
	else{
		for(int i=0;i<blink_times;i++){
		  digitalWrite(LED_PIN,!digitalRead(LED_PIN));
		  delay(750);
		  digitalWrite(LED_PIN,!digitalRead(LED_PIN));
		  delay(750);
		}
	}
}     

