int value = 0;
volatile char flag = 0;

void setup() {
  Serial.begin(19200); 
  analogReference(INTERNAL1V5);
  
  TACCTL0 |= CCIE;
  TACCR0 = 250-1;	// 8kHz (16MHz/82/8kHz)
  TACTL |= TASSEL_2 + ID_3; // SMCLK & clk/8
  TACTL |= MC_1;	//Use UP mode timer
  
}

void loop() {

    if(!flag){
      return;
    }
    flag = 0;
  
  value = analogRead(A0);            
         
  Serial.println(value);   

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
  flag = 1;
  return;
}
