#ifndef _QUEUE_HPP_
#define _QUEUE_HPP_

class Queue 
{
	int head, tail, _count, size;
	char *data;

public:
	Queue ( int size );
	virtual ~Queue();

	inline int count ( void ) { return _count; };
	inline void clear ( void ) { _count = head = tail = 0; };

	void put ( int ch );
	int get ( void );
};

#endif


