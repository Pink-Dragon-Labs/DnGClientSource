//
// array
//
// This is a basic resizing array class.
//
// author: Stephen Nichols
//

#include "arrayC.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OCArray::OCArray ( int size )
{
	m_pData = NULL;
	m_nSize = m_nActualSize = 0;
	m_bFreeBuffer = 1;

	Grow ( size );
} 

OCArray::OCArray()
{
	m_pData = NULL;
	m_nSize = m_nActualSize = 0;
	m_bFreeBuffer = true;

	Grow ( _ARRAY_START_SIZE );
}

OCArray::~OCArray()
{
	if ( m_pData ) {
		if ( m_bFreeBuffer )
			free ( m_pData );

		m_pData = NULL;
	}

	m_nSize = m_nActualSize = 0;
}

void OCArray::Grow ( int size )
{
	// you can't grow an array that does not own it's buffer
	if ( !m_bFreeBuffer ) 
		return;

	int newSize = m_nActualSize + size;
	unsigned char *newData = (unsigned char *)calloc ( 1, sizeof ( char ) * newSize ); 

	if ( m_pData ) {
		memcpy ( newData, m_pData, sizeof ( unsigned char ) * m_nActualSize );
		free ( m_pData );
	}

	m_pData = newData;
	m_nActualSize = newSize;
}
