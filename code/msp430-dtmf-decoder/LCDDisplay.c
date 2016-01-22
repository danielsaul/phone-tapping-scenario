/*
LCDDisplay.c


LCDDriver.cpp - Library for using the MIDAS MCCOG21605B6W-FPTLWI 16x2 Black on white
LCDDriver, with I2C interface with the MSP430G2553 that comes with the MSP430 Launchpad.
The I2C interface uses
MSP4302553|         |LCDDriver
|Pin 1 Boost Capacitor 1
|Pin 2 Capacitor 2 N
|Pin 3 Capacitor 2 P
Vcc (3.5V) |---------|Pin 4 Vdd (Boost Capacitor 1)
Gnd |---------|Pin 5 Gnd
P1.7|---------|Pin 6 (SDA)
P1.6|---------|Pin 7 (SCL)
P2.5|---------|Pin 8 (RST)
P2.4|---------| A (Backlight Anode)
Gnd |---------| K (Backlight Cathode)
P1.4|---------| Switch 1
P2.0|---------| Switch 2
P2.1|---------| Switch 3
P2.2|---------| Switch 5
P2.3|---------| Switch 4


4.7kohm pullups are used on pins 6, 7, 8. Two 1uF capacitors are required
between pins 1 and 4 and between 2 and 3 when using a 3V supply.
The switches are connected from the microcontroller pins to ground and require the internal
pullups to be configured.

Note as P1.6 is used for SCL you must remove the LED2 jumper on the MSP430 Launchpad board.
Created by Benn Thomsen, 15 February, 2014.
Released into the public domain.

Created by Benn Thomsen on 17/10/2014.
Copyright (c) 2014 Benn Thomsen.
Released into the public domain.
*/

#include "LCDDisplay.h"
#include "i2c.h"


#define SWITCHPORT1 SW2
#define SWITCHPORT2 (SELECT + UP + DOWN + LEFT +RIGHT)   //Bit locations for switches on port 2

#define CHARCONTROL 0x40
#define BLINK 0x01
#define CURSOR 0x02
#define DISPLAYMEM 0x80

volatile char button1;
volatile char button2;

char TxData[] =     // Table of LCD display initialisation commands
{ 0x00,
    0x38,
    0x00,
    0x39,
    0x14,
    0x70,
    0x54,
    0x6F,
    0x0F, //Display On, Cursor On, Cursor Blink On
    0x01
};

char home[] = {0x80,0x02};
char clear[] = {0x80,0x01};
char display[] = {0x80,0x0F};
char column = 0x00;
char col[] = {0x80,0x00};
char writeChar[] = {0x40,0x00};

void LCDConfigure(void)
{
    i2cConfigure();
    
    i2cTx(ADDRESS,TxData,10);
    
    /* Configure timer A as a pulse width modulator */
    TA1CCR0 = 1000-1;             // Set maximum count value to determine PWM Frequency = SMCLK/TACCR0 (1MHz/1000 = 1kHz)
    TA1CCR2 = 100;                // Initialise counter compare value 1 to control Duty Cycle = TACCR1/TACCR0 (500/1000 = 50%)
    TA1CCTL2 = OUTMOD_7;          // Set output to on when counter resets and off when counter equals TACCR1. Normal PWM.
    TA1CTL = TASSEL_2 + MC_1;     // Use the SMCLK to clock the counter and set to count up mode
    
    P2DIR |= BACKLIGHT;               // Set P2.4 as output
    P2SEL |= BACKLIGHT;               // Select output P2.4 to be TA1.2
    P2SEL2 &= ~BACKLIGHT;             // Select output P2.4 to be TA1.2
    
    /* Define input pin functionality for switches */
    P1SEL &= ~ SWITCHPORT1;
    P1SEL2 &= ~ SWITCHPORT1;
    P1DIR &= ~ SWITCHPORT1;  // Set switch pin as an input
    P1OUT |= SWITCHPORT1;    // Set pull up resistor on for button
    P1REN |= SWITCHPORT1;    // Enable pullup or pulldown resistors
    P1IES |= SWITCHPORT1;    // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
    P1IFG &= ~SWITCHPORT1;   // Clear the interrupt flag for the button
    P1IE = SWITCHPORT1;     // Enable interrupts on port 1 for the button
    pressRelease1 = 0;
    pressRelease2 = 0;
    
    
    P2SEL &= ~ SWITCHPORT2;
    P2SEL2 &= ~ SWITCHPORT2;
    P2DIR &= ~ SWITCHPORT2;   // Set button pin as an input pin
    P2OUT |= SWITCHPORT2;    // Set pull up resistor on for button
    P2REN |= SWITCHPORT2;    // Enable pull up resistor for button to keep pin high until pressed
    P2IES |= SWITCHPORT2;    // Enable Interrupt to trigger on the falling edge (high (unpressed) to low (pressed) transition)
    P2IFG &= ~SWITCHPORT2;   // Clear the interrupt flag for the button
    P2IE |= SWITCHPORT2;     // Enable interrupts on port 1 for the button
}

void LCDWriteString(char *string)
{
    while (*string) LCDWriteChar(*string++);
}

void LCDWriteChar(char character)
{
    writeChar[1] = character;               // Append the character control character to the start of the string
    i2cTx(ADDRESS,writeChar,2);
    column = column + 1;
}


void LCDHome(void)
{
    i2cTx(ADDRESS,home,2);
}

void LCDClear(void)
{
    i2cTx(ADDRESS,clear,2);
}

void LCDBlinkOn(void)
{
    display[1] |= BLINK;
    i2cTx(ADDRESS,display,2);
}

void LCDBlinkOff(void)
{
    display[1] &= ~BLINK;
    i2cTx(ADDRESS,display,2);
}

void LCDCursorOn(void)
{
    display[1] |= CURSOR;
    i2cTx(ADDRESS,display,2);
}

void LCDCursorOff(void)
{
    display[1] &= ~CURSOR;
    i2cTx(ADDRESS,display,2);
}

void LCDForward(void)
{
    if (column == 0x0F) column = 0x40;        // Check if at end of top row and if so go to start of bottom row
    else if (column == 0x4F) column = 0x00;   // Check if at end of bottom row and if so go to start of top row
    else column++;                            // Increment column
    col[1] = column | DISPLAYMEM;             // Append to command string and display memory address
    i2cTx(ADDRESS,col,2);
}

void LCDBack(void)
{
    if (column == 0x00) column = 0x4F;
    else if (column == 0x40) column = 0x0F;
    else column--;
    col[1] = column | DISPLAYMEM;
    i2cTx(ADDRESS,col,2);
}

void LCDUp(void)
{
    if (column & 0x40)
    {
        column &= 0x0F;
        col[1] = column | DISPLAYMEM;
        i2cTx(ADDRESS,col,2);
    }
}

void LCDDown(void)
{
    if (!(column & 0x40))
    {
        column |= 0x40;
        col[1] = column | DISPLAYMEM;
        i2cTx(ADDRESS,col,2);
    }
}

void LCDSetLocation(char rowindex, char colindex)
{
    if (rowindex) column = 0x40;
    else column = 0x00;
    column = column + colindex;
    col[1] = column | DISPLAYMEM;
    i2cTx(ADDRESS,col,2);
}



void LCDBackLight(int level)
{
    TA1CCR2 = level;                // Initialise counter compare value 1 to control Duty Cycle = TACCR1/TACCR0 (500/1000 = 50%)
}

/* Port 1 interrupt to service the button press */
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    button1 = P1IFG & SWITCHPORT1;   // Read PORT1 interrupt flag to determine source pin masking off Switch Inputs
    if (button1)
    {
        P1IE &= ~button1;                // Disable Button interrupt
        P1IFG &= ~button1;               // Clear the interrupt flag for the button
        
        if (P1IES & button1) {
            P1OUT |= BIT0;
            P1IES &= ~button1;           // Clear edge select to detect rising edge
        }
        else
        {
            P1OUT &= ~BIT0;
            P1IES |= button1;            // Set edge select to detect falling edge
            pressRelease1 = button1;
        }
        
        WDTCTL = WDT_MDLY_32;           // Start and set watchdog timer (WDT) to trigger every 32ms
        IFG1 &= ~WDTIFG;                // Clear the interrupt flag for the WDT
        IE1 |= WDTIE;                   // enable WDT interrupt
    }
}

/* Port 2 interrupt to service the button press */
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    button2 = P2IFG & SWITCHPORT2;   // Read PORT1 interrupt flag to determine source pin masking off Switch Inputs
    if (button2)
    {
        P2IE &= ~button2;                // Disable Button interrupt
        P2IFG &= ~button2;               // Clear the interrupt flag for the button
        
        if (P2IES & button2) {
            P1OUT |= BIT0;
            P2IES &= ~button2;           // Clear edge select to detect rising edge
        }
        else
        {
            P1OUT &= ~BIT0;
            P2IES |= button2;            // Set edge select to detect falling edge
            pressRelease2 = button2;
        }
        
        WDTCTL = WDT_MDLY_32;           // Start and set watchdog timer (WDT) to trigger every 32ms
        IFG1 &= ~WDTIFG;                // Clear the interrupt flag for the WDT
        IE1 |= WDTIE;                   // enable WDT interrupt
    }
}

// WDT Interrupt Service Routine used to de-bounce button press
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    IE1 &= ~WDTIE;                   // disable Watchdog timer (WDT) interrupt
    IFG1 &= ~WDTIFG;                 // clear WDT interrupt flag
    WDTCTL = WDTPW + WDTHOLD;        // put WDT back in hold state
    P1IE |= SWITCHPORT1;             // Reenable interrupts for the switches on port 1
    P2IE |= SWITCHPORT2;             // Reenable interrupts for the switches on port 2
}
