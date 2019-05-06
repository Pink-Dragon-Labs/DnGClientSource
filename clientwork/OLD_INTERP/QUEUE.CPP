#include <stdlib.h>
#include <stdio.h>

#include "queue.hpp"

Queue::Queue ( int theSize )
{
	data = (char *)malloc ( theSize );
	head = tail = _count = 0;
	size = theSize;
}

Queue::~Queue()
{
	if ( data ) {
		free ( data );
		data = NULL;
	}

	size = head = tail = _count = 0;
}

void Queue::put ( int ch )
{
	data[tail++] = (char)ch;
	_count++;

	if ( tail > size )
		tail = 0;
}

int Queue::get ( void )
{
	if ( !_count ) 
		return -1;

	int ret = (int)(unsigned char)data[head++];
	_count--;

	if ( head > size )
		head = 0;

	return ret;
}
