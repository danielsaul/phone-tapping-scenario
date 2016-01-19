/*
 * Phone Tapping Scenario
 * MSP430 Launchpad
 * Daniel Saul, 2016
 *
 * main.h
 *
 */

#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define PI 3.1416f

static const int fs = 4000;
static const int NUM = 95;
static const int signal_lower_threshold = 60;
static const int signal_upper_threshold = 132;
static const long threshold = 10000;

static const long rows[4] = {697, 770, 852, 941};
static const long cols[3] = {1209, 1336, 1477};
static const char nums[4][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'}, {'*','0','#'} };

char i = 0;

volatile char flag = 0;
volatile int cnt = 0;
volatile bool newtone = false;
volatile int n = 0;
volatile int m = 0;
volatile long row_coeffs[4];
volatile long col_coeffs[3];
volatile int Qr[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
volatile int Qc[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
volatile long magr[4];
volatile long magc[4];
volatile char number[11] = "xxxxxxxxxxx";


int determineCol(void);
int determineRow(void);
char calculate_goertzel_magnitudes(void);
void update_goertzel(int val);

#endif
