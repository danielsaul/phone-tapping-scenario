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
static const int fs = 8000;

char i = 0;

bool flag = false;

int cnt = 0;
static const reset_cnt = 8000; // 1 second
static const int signal_lower_threshold = 100;
static const int signal_upper_threshold = 156;
bool newtone = true;

int n = 0;
static const int NUM = 125;

static const long rows[4] = {697, 770, 852, 941};
static const long cols[3] = {1209, 1336, 1477};

static long row_coeffs[4];
static long col_coeffs[3];

int Qr[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
int Qc[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

long magr[4];
long magc[4];

static const long threshold = 10000000;

char nums[4][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'}, {'*','0','#'} };

bool waiting = false;
int m = 0;
char number[11];


int determineCol(void);
int determineRow(void);
bool calculate_goertzel_magnitudes(void);
void update_goertzel(int val);



#endif
