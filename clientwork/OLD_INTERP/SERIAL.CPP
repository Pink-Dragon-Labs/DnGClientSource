#include "serial.hpp"

// pointer to BIOS tick count
unsigned int *_bios_ticks = (unsigned int *)0x46c;

// declare the baud rate description text
char *baudDescriptions[] = {
	"2400",
	"4800",
	"9600",
	"14400",
	"19200",
	"28800",
	"38400",
	"57600"
};

#pragma pack (1)

typedef struct
{
	void __far *fpOldPmi; 				// Old protected mode hook
	void *pOldRmi;  						// Old real mode hook
	unsigned short uwRealSelector;  	// Selector for RMI buffer
	unsigned short port;
	unsigned short head;
	unsigned short tail;
	unsigned short icount;
	char buffer[4096];
} PORTINFO;

#pragma pack ()

extern void __cdecl pmisr(void);
extern void __cdecl __far startisr(void);
extern void __cdecl __far rmisr(void);
extern void __cdecl __far endisr(void);

#define	D32RealSeg(P)	((((unsigned long) (P)) >> 4) & 0xFFFF)
#define	D32RealOff(P)	(((unsigned long) (P)) & 0xF)

unsigned short AllocDOSMem( unsigned short uwParagraphs, unsigned short *puwSelector )
{
	union REGS regs;

	// Call DPMI function to allocate dos memory
	regs.w.ax = 0x0100U;
	regs.w.bx = uwParagraphs;
	int386(0x31, &regs, &regs );

	// If carry is set, we failed
	if ( regs.x.cflag )
	{
		return NULL;
	}
	// Save selector and return segment address
	else
	{
		*puwSelector = regs.w.dx;
		return regs.w.ax;
	}
}

unsigned short FreeDOSMem( unsigned short uwSelector  )
{
	union REGS regs;

	// Call DPMI function to free dos memory
	regs.w.ax = 0x0101U;
	regs.w.dx = uwSelector;
	int386(0x31, &regs, &regs );

	// If carry is set, we failed
	return regs.x.cflag;
}

// declare the baud rate table
int baudRateTbl[] = { 48, 24, 12, 8, 6, 4, 3, 2 };

// port info structure

PORTINFO *gpPortInfo = NULL;
extern "C" void *portInfoPtr;

SerialPort::SerialPort()
{
	installed = 0;
	base = 0;
}

SerialPort::~SerialPort()
{
	close();
}

void SerialPort::init ( void )
{
	int temp;

	// install the interrupt handler
	if ( !installed ) {
		union REGS r;

		// get the size of the ASM code
		int asmSize = FP_OFF ( endisr ) - FP_OFF ( startisr ) + 1;

		// allocate room for the real-mode code
		unsigned short realSelector;
		unsigned short realSegment = AllocDOSMem ( (asmSize + 15) >> 4, &realSelector );

		// copy the code to real-mode
		void *realPtr = (void *)((unsigned long) realSegment << 4);
		_fmemcpy ( (void far *)realPtr, startisr, asmSize );

		// get a pointer to the PORTINFO struct
		asmSize = FP_OFF ( endisr ) - FP_OFF ( startisr ) - sizeof ( PORTINFO );
		gpPortInfo = (PORTINFO *)((unsigned char *)realPtr + asmSize);
		portInfoPtr = gpPortInfo;

		// store the offset of the PORTINFO struct into the InfoPtr variable
		unsigned short *infoPtr = (unsigned short *)realPtr;
		*infoPtr = asmSize;

		// point to the actual routine
		realPtr = (unsigned char *)realPtr + sizeof ( unsigned short );

		// save selector
		gpPortInfo->uwRealSelector = realSelector;

		// setup the structure
		gpPortInfo->port = (unsigned short)base;

		// save the starting real and protected mode vectors
		r.x.eax = 0x0204;
		r.h.bl = irq;
		int386 ( 0x31, &r, &r );
		gpPortInfo->fpOldPmi = MK_FP ( r.w.cx, r.x.edx );

		r.x.eax = 0x0200;
		r.h.bl = irq;
		int386 ( 0x31, &r, &r );
		gpPortInfo->pOldRmi = (void *)(((unsigned long)r.w.cx << 16) + (unsigned long)r.w.dx);

		// set the real and protected mode vectors
		r.x.eax = 0x0205;
		r.x.ebx = irq;
		r.x.ecx = (unsigned long)FP_SEG ( (void __far *)pmisr );
		r.x.edx = FP_OFF ( (void __far *)pmisr );
		int386 ( 0x31, &r, &r );

		r.x.eax = 0x0201;
		r.x.ebx = irq;
		r.x.ecx = D32RealSeg ( realPtr );
		r.x.edx = D32RealOff ( realPtr );
		int386 ( 0x31, &r, &r );
	}

	if ( config != -1 ) {
		// set the port configuration
		outp ( lcr, config );

		_disable();
		temp = inp ( lsr );
		temp = inp ( base );

		// enable the hardware interrupt that we would like to use
		outp ( 0x21, inp ( 0x21 ) & ~(1 << (irq - 8)) );

		// enable just RxDY interrupt
		outp ( ier, 1 );

		// prepare the mcr
		outp ( mcr, inp ( mcr ) | 0x0a );

		// prepare the fcr
		outp ( fcr, 0x40 );

		_enable();

		dtr ( 1 );
	}

	installed = 1;
}

// handle setting the baud rate to the value specified
void SerialPort::setBaud ( unsigned int rate )
{
	outp ( lcr, inp ( lcr ) | 0x80 );
	outpw ( base, rate );
	outp ( lcr, inp ( lcr ) & 0x7f );

	baud = rate;
}

// set the working port to the passed value
void SerialPort::setPort ( int thePort ) 
{
	int basePorts[] = { 0x3f8, 0x2f8, 0x3e8, 0x2e8 };
	int baseIRQs[] = { 12, 11, 14, 15 };

	// convert the COM request to it's basePort
	base = basePorts[thePort];
	irq  = baseIRQs[thePort];
	port = thePort;

	// get all of the individual ports
	ier = base + 1;
	iir = base + 2;
	fcr = base + 2;
	lcr = base + 3;
	mcr = base + 4;
	lsr = base + 5;
	msr = base + 6;
}

// set the DTR pin
void SerialPort::dtr ( int i )
{
	int temp;

	if ( installed ) {
		temp = inp ( mcr ) & 0xfe;
		outp ( mcr, i? (temp + 1) : temp );
	}
}

// set the RTS pin
void SerialPort::rts ( int i )
{
	int temp;

	if ( installed ) {
		temp = inp ( mcr ) & 0xfd;
		outp ( mcr, i? (temp + 2) : temp );
	}
}

// return the state of the carrier detect lead
int SerialPort::carrier ( void )
{
	if ( installed )
		return ( (inp ( msr ) & 0x80)? 1 : 0 );

	return 0;
}

int SerialPort::open ( int thePort, int baud, int theConfig )
{
	// skip out if we are already installed
	if ( installed )
		return -1;

	config = theConfig;

	setPort ( thePort );
	setBaud ( baud );
	init();

	return 0;
}

int SerialPort::close ( void )
{
	// skip out if we are not installed
	if ( !installed )
		return -1;

	_disable();

	if ( config != -1 ) {
		// disable the hardware interrupt
		outp ( 0x21, inp ( 0x21 ) | (1 << (irq - 8)) );

		// reset the fcr
		outp ( fcr, 0 );

		// reset the mcr
		outp ( mcr, 3 );
	}

	_enable();

	installed = 0;
	
	return 1;
}

// return the number of characters in the incoming buffer
int SerialPort::icount ( void )
{
	return gpPortInfo->icount;
}

// clear out the incoming buffer
void SerialPort::iclear ( void )
{
	gpPortInfo->icount = 0;
	gpPortInfo->head = 0;
	gpPortInfo->tail = 0;
}

// get the next available byte from the input buffer
int SerialPort::getch ( void )
{
	int ch = -1;

	if ( gpPortInfo->icount ) {
		ch = (int)((unsigned char)gpPortInfo->buffer[gpPortInfo->head++]);

		if ( gpPortInfo->head > 4096 )
			gpPortInfo->head = 0;

		gpPortInfo->icount--;
	}

	return ch;
}

// put one character in the outgoing buffer
int SerialPort::putch ( int ch )
{
	// skip out if not installed
	if ( !installed )
		return -1;

	while ( !(inp ( lsr ) & 0x20) )
		;

	while ( !(inp ( msr ) & 0x10) )
		;

	outp ( base, ch );

	return ch;
}

// printf out the port
void SerialPort::printf ( char *format, ... )
{
	va_list args;
	char output[1024], *ptr = output;

	va_start ( args, format );
	vsprintf ( output, format, args );
	va_end ( args );

	while ( *ptr ) 
		putch ( *ptr++ );
}

// wait for a particular string to come across the port
int SerialPort::waitfor ( char *s, int delay, char *result )
{
	int start = *_bios_ticks, end = start + (18 * delay);
	char *cp = s;

	// wait until the delay has passed
	while ( *cp && (*_bios_ticks < end) ) {
		if ( kbhit() ) 
			if ( ::getch() == 27 ) 
					end = *_bios_ticks;

		if ( icount() ) {
			int ch = getch();

			if ( result ) {
				*result++ = (char)ch;
				*result = 0;
			}

			if ( *cp == (char)ch ) 
				cp++;
			else
				cp = s;
		}
	}

	return !*cp;
}

// process a string to remove the '\' sequences and replace with real characters
void SerialPort::processString ( char *text )
{
	char *src = text, *dest = text;

	while ( *src ) {
		char ch = *src++;

		switch ( ch ) {
			case '\\': {
				switch ( *src++ ) {
					case '\\':
						*dest++ = '\\';
						break;

					case 'r':
						*dest++ = 0x0D;
						break;

					case 'n':
						*dest++ = 0x0A;
						break;

					case 't':
						*dest++ = 0x09;
						break;
				}
			}

			break;

			default:
				*dest++ = ch;
		}
	}

	*dest = 0;
}

// pause for a variable number of ticks
void SerialPort::pause ( int ticks )
{
	int end = *_bios_ticks + ticks;

	while ( *_bios_ticks < end )
		;
}














































							 
