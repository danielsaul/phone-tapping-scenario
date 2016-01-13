/*
 * Phone Tapping Scenario
 * DTMF Decoder
 * MSP430G2553
 * Daniel Saul, 2016
 *
 * main.c
 *
 */

#include <msp430g2553.h>
#include <math.h>
#include <stdbool.h>
#include "main.h"
#include "LCDDisplay.h"

int main(void){

  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;

  // P1.0 8kHz loop pulse
  P1DIR |= BIT0;
  P1OUT |= BIT0;

  // LCD Config
  LCDConfigure();
  LCDBackLight(300);

  // Display configuring message
  LCDHome();
  LCDCursorOff();
  LCDBlinkOff();
  LCDWriteString("Configuring...");

  // ADC Config
  ADC10CTL0 = SREF_1 | ADC10SHT_0 | REFON | ADC10ON; // 1.5V ref
  ADC10CTL1 = INCH_4 | ADC10DIV_0; // ADC channel 4,
  ADC10AE0 = BIT4; // P1.4
  ADC10CTL0 |= ENC;

  // Generate coeffs
  for(i=0; i<4; i++){     // Rows
    int k = (int)(0.5 + ((NUM*rows[i])/fs));
    long x = (long)(256 * (2 * cos((2*PI*k)/NUM)));
    row_coeffs[i] = x;

    /*LCDClear();
    LCDHome();
    char c[16];
    sprintf(c, "%u", x);
    LCDWriteString(c);
    __delay_cycles(10000000);*/

  }
  for(i=0; i<3; i++){     // Cols
    int k = (int)(0.5 + ((NUM*cols[i])/fs));
    long x = (long)(256 * (2 * cos((2*PI*k)/NUM)));
    col_coeffs[i] = x;

    /*LCDClear();
    LCDHome();
    char c[16];
    sprintf(c, "%u", x);
    LCDWriteString(c);
    __delay_cycles(10000000);*/

  }

  // Display waiting message
  LCDClear();
  LCDHome();
  LCDWriteString("Waiting for");
  LCDSetLocation(1,0);
  LCDWriteString("number...");

  // Timer A Config
  TACCTL0 |= CCIE;
  TACCR0 = 250-1;	// 8kHz (16MHz/82/8kHz)
  TACTL |= TASSEL_2 + ID_3; // SMCLK & clk/8
  TACTL |= MC_1;	//Use UP mode timer


  while(1){

    // Flag is enabled at 8kHz
    if(!flag){
      continue;
    }
    flag = false;

    P1OUT |= BIT0; // P1.0 High - Start of 8kHz loop

    n++; // Increment sample counter

    ADC10CTL0 |= ADC10SC; // Trigger ADC Conversion
    while( ADC10CTL1 & ADC10BUSY ) ;
    long adcvalue = (long)ADC10MEM / 4; // Divide into 8bit

    goertzel_feedback(adcvalue);

    if(n >= NUM){ // If enough samples
      n=0; // Reset sample counter

      bool aboveThreshold = calculate_goertzel_magnitudes(); // Calculate magnitudes

      if(aboveThreshold && waiting){ // Must be both a row and col above the threshold, and be waiting for a tone

        int col = determineCol();
        int row = determineRow();

        number[m] = result;
        LCDSetLocation(0,m);
        LCDWriteString(number[m]);

        m++;
        if(m > 10){
          m = 0;
        }

        waiting = false;
      }

      if(!aboveThreshold){
        waiting = true;
      }

    }

    P1OUT &= ~BIT0; // P1.0 Low - End of 8kHz loop

  }

}

int determineCol(void){

  if(magc[0] > magc[1] && magc[0] > magc[2]){
    return 0;
  }
  if(magc[1] > magc[0] && magc[1] > magc[2]){
    return 1;
  }
  return 2;

}

int determineRow(void){

  if(magr[0] > magr[1] && magr[0] > magr[2] && magr[0] > magr[3]){
    return 0;
  }
  if(magr[1] > magr[0] && magr[1] > magr[2] && magr[1] > magr[3]){
    return 1;
  }
  if(magr[2] > magr[0] && magr[2] > magr[1] && magr[2] > magr[3]){
    return 2;
  }
  return 3;

}

bool calculate_goertzel_magnitudes(void){

  bool aboveThreshold = true;

  //Magnitude Rows
  for(i=0; i<4; i++){
    Qr[i][1] = Qr[i][1]/256;
    Qr[i][2] = Qr[i][2]/256;

    long x = (long)row_coeffs[i] * (long)Qr[i][1] * (long)Qr[i][2];
    magr[i] = (((int)Qr[i][1] * (int)Qr[i][1]) + ((int)Qr[i][2] * (int)Qr[i][2]) - x);

    Qr[i][1] = 0;
    Qr[i][2] = 0;

    if(magr[i] < threshold){
      aboveThreshold = false;
    }
  }

  if(!aboveThreshold){
    return false;
  }

  //Magnitude Cols
  for(i=0; i<3; i++){
    Qc[i][1] = Qc[i][1]/256;
    Qc[i][2] = Qc[i][2]/256;

    long x = (long)col_coeffs[i] * (long)Qc[i][1] * (long)Qc[i][2];
    magc[i] = (((int)Qc[i][1] * (int)Qc[i][1]) + ((int)Qc[i][2] * (int)Qc[i][2]) - x);

    Qc[i][1] = 0;
    Qc[i][2] = 0;

    if(magc[i] < threshold){
      aboveThreshold = false;
    }
  }

  if(!aboveThreshold){
    return false;
  }

  return true;

}

void update_goertzel(long val){

  //Rows
  for(i=0; i<4; i++){
    Qr[i][0] = (((long)row_coeffs[i] * (long)Qr[i][1])/256);
    Qr[i][0] -= Qr[i][2];
    Qr[i][0] += adcvalue;
    Qr[i][2] = Qr[i][1];
    Qr[i][1] = Qr[i][0];
  }

  //Cols
  for(i=0; i<3; i++){
    Qc[i][0] = (((long)col_coeffs[i] * (long)Qc[i][1])/256);
    Qc[i][0] -= Qc[i][2];
    Qc[i][0] += adcvalue;
    Qc[i][2] = Qc[i][1];
    Qc[i][1] = Qc[i][0];
  }

}

// 8kHz Loop
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
  flag = true;
  return;
}
