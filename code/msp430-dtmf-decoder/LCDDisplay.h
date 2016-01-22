//
//  LCDDisplay.h
//  MSP430RealTimeOS
//
//  Created by Benn Thomsen on 17/10/2014.
//  Copyright (c) 2014 Benn Thomsen. All rights reserved.
//

#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

#define ADDRESS 0x3E                // LCD Shield I2C Address

// Switch port connections
#define SW2 BIT3                    // Launchpad Switch 2 (P1.3)
#define UP BIT0                     // LCD Shield Switch 1 (P2.0)
#define LEFT BIT1                   // LCD Shield Switch 1 (P2.1)
#define DOWN BIT2                   // LCD Shield Switch 1 (P2.2)
#define RIGHT BIT3                  // LCD Shield Switch 1 (P2.3)
#define SELECT BIT5                 // LCD Shield Switch 1 (P2.5)

#define BACKLIGHT BIT4              // LCD Shield Switch 1 (P2.4)


char pressRelease1;	   // Flag set to switch number when a switch on port1 is clicked
char pressRelease2;    // Flag set to switch number when a switch on port2 is clicked

void LCDConfigure(void);
void LCDWriteString(char *string);
void LCDWriteChar(char character);
void LCDHome(void);
void LCDClear(void);
void LCDBlinkOn(void);
void LCDBlinkOff(void);
void LCDCursorOn(void);
void LCDCursorOff(void);
void LCDForward(void);
void LCDBack(void);
void LCDUp(void);
void LCDDown(void);
void LCDSetLocation(char rowindex, char colindex);
void LCDBackLight(int level);
