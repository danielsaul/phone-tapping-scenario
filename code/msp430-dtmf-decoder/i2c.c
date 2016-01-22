//
//  i2c.c
//  MSP430RealTimeOS
//
//  Created by Benn Thomsen on 17/10/2014.
//

#include "i2c.h"

#define IDLE 0
#define TRANSMIT 1
#define RECEIVE 2



char *tx_dataptr;
unsigned char rxdata[12];
unsigned char tx_byte_count;
unsigned char rx_byte_count;
unsigned char tx_byte_counter;
unsigned char rx_byte_counter;
unsigned char i;
unsigned char tx_rx = IDLE;

void i2cConfigure(void)
{

    UCB0CTL1 |= UCSWRST;							//Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;			//I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;					//Use SMCLK, keep SW reset
    P1SEL |= BIT6 + BIT7;							//Set I2C pins
    P1SEL2|= BIT6 + BIT7;
    UCB0BR0 = 192;									//fSCL = SMCLK/192 = ~100kHz
    UCB0BR1 = 0;

    UCB0CTL1 &= ~UCSWRST;							//Clear SW reset, resume operation
    IE2 |= UCB0TXIE;								//Enable TX interrupt
    IE2 |= UCB0RXIE;								//Enable RX interrupt
}

void i2cTx(unsigned char address, char *tx_data, unsigned char tx_count)
{
    //while (!(tx_rx == 0));                              // Wait until I2C Tx is not busy
    UCB0I2CSA = address;							//Slave Address
    tx_rx = TRANSMIT;
    tx_dataptr = tx_data;
    tx_byte_counter = tx_count;						// Load TX byte counter
    UCB0CTL1 |= UCTR + UCTXSTT;						// I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);				// Enter LPM0 w/ interrupts
    // Remain in LPM0 until all data is TX'd
}

void i2cRx(unsigned char address, unsigned char rx_count)
{
    UCB0I2CSA = address;							//Slave Address
    tx_rx = RECEIVE;
    rx_byte_counter = rx_count;						// Load RX byte counter
    UCB0CTL1 &= ~UCTR;								// I2C RX
    UCB0CTL1 |= UCTXSTT;							// I2C start condition
    __bis_SR_register(CPUOFF + GIE);				// Enter LPM0 w/ interrupts
    // Remain in LPM0 until all data is RX'd
}

//interrupt(USCIAB0TX_VECTOR) USCIAB0TX_ISR(void)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)				//For mspgcc
{
    switch(tx_rx)
    {
        case TRANSMIT:
            if (tx_byte_counter > 0)					//Check TX byte counter
            {
                UCB0TXBUF = *tx_dataptr++;			    // Load TX buffer
                tx_byte_counter--;						//Decrement TX byte counter
            }
            else if(tx_byte_counter == 0)
            {
                UCB0CTL1 |= UCTXSTP;					//I2C stop condition
                while (UCB0CTL1 & UCTXSTP);				//Ensure stop condition got sent
                IFG2 &= ~UCB0TXIFG;						//Clear USCI_B0 TX int flag
                tx_rx = IDLE;
                __bic_SR_register_on_exit(CPUOFF);		//Exit LPM0
            }
            break;
        case RECEIVE:
            if (rx_byte_counter > 0)					//Check RX byte counter
            {
                rxdata[rx_byte_count - rx_byte_counter] = UCB0RXBUF;
                rx_byte_counter--;						//Decrement RX byte counter
            }
            else if(rx_byte_counter == 0)
            {
                UCB0CTL1 |= UCTXSTP;					// I2C stop condition
                while (UCB0CTL1 & UCTXSTP);				// Ensure stop condition got sent
                rxdata[rx_byte_count - (rx_byte_counter + 1)] = UCB0RXBUF;
                rxdata[rx_byte_count - (rx_byte_counter + 1)] = UCB0RXBUF;
                IFG2 &= ~UCB0RXIFG;						// Clear USCI_B0 RX int flag
                tx_rx = IDLE;
                __bic_SR_register_on_exit(CPUOFF);		// Exit LPM0
            }
            break;
    }

}
