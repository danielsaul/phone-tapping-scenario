#include <TimerOne.h>


int value = 0;
volatile int flag = 0;
void setup() {
  // set prescale to 16
bitClear(ADCSRA,ADPS0); 
bitSet(ADCSRA,ADPS1); 
bitClear(ADCSRA,ADPS2);

  Serial.begin(921600); 
  pinMode(2, OUTPUT);  
  Serial.println("setup");
  delay(100);
  
  Timer1.initialize(250);
  Timer1.attachInterrupt(timerIsr);

}

void loop() {


}

void timerIsr()
{
    value = analogRead(A0);
      Serial.println(value);
}
