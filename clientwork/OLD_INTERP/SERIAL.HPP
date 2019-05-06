
//
// SERIAL.HPP
//
// Defines and function declarations for SERIAL.ASM
//
// Author: Stephen Nichols
//

#ifndef _SERIAL_HPP_
#define _SERIAL_HPP_

#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "queue.hpp"

// declare the available baud rates
enum {
	BAUD_2400,
	BAUD_4800,
	BAUD_9600,
	BAUD_14400,
	BAUD_19200,
	BAUD_28800,
	BAUD_38400,
	BAUD_57600
};

// declare the baud description table
extern char *baudDescriptions[];
extern int baudRateTbl[];

// declare the available COM ports
enum {
	COM1,
	COM2,
	COM3,
	COM4,
};

// declare available parity choices
#define PAR_NONE  0
#define PAR_ODD   8
#define PAR_EVEN  24

// declare stop bit choices
#define SBITS_1		0
#define SBITS_2		4

// declare data bit choices
#define DBITS_5		0
#define DBITS_6		1
#define DBITS_7		2
#define DBITS_8		3

// define the size (in bytes) for the input buffer
#define INBUFSIZE 	4096

class SerialPort 
{
public:
	SerialPort();
	virtual ~SerialPort();

	// return the number of bytes in the input buffer
	int icount ( void );

	// clear the incoming buffer
	void iclear ( void );

	// init the port
	void init ( void );

	// set the baud rate
	void setBaud ( unsigned int rate );

	// set the port
	void setPort ( int thePort );

	// various pin manipulators
	void dtr ( int value );
	void rts ( int value );
	int carrier ( void );

	// open this serial port
	int open ( int port, int baud, int theConfig = PAR_NONE | DBITS_8 | SBITS_1 );

	// close this serial port
	int close ( void );

	// get one character from this serial port
	int getch ( void );

	// put one character to this serial port
	int putch ( int ch );

	// printf to the port
	void printf ( char *format, ... );

	// wait for a specific string to come across the port
	int waitfor ( char *string, int seconds, char *result = NULL );

	// process a string to remove the '\' sequences and replace with real characters
	void processString ( char *text );

	// pause for a variable number of seconds
	void pause ( int seconds );

	// port information
	int iir, fcr, ier, mcr, lsr, lcr, msr, config;
	
	int installed, base, irq, port, baud;
};

#endif
