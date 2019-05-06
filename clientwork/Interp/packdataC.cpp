//
// packdata
//
// This file contains the CPackData class.  The CPackData class represents a auto-resizable 
// buffer.  Various members exist to add to and remove from this buffer.  Quite useful, really.
//
// author: Stephen Nichols
//

#include "packdataC.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPackedData::CPackedData()
{
	m_nGetIndex = m_nPutIndex = 0;
}

CPackedData::CPackedData ( char *ptr, int size ) : OCArray()
{
	m_bFreeBuffer = false;
	m_pData = (unsigned char *)ptr; 

	m_nActualSize = size;
	m_nSize = size;
	m_nGetIndex = 0;
	m_nPutIndex = size;
}

CPackedData::~CPackedData()
{
}

void CPackedData::Init ( char *ptr, int size )
{
	if ( (m_bFreeBuffer == true) && m_pData ) {
		free ( m_pData );
		m_pData = NULL;
	}

	m_pData = (unsigned char *)malloc ( size );
	memcpy ( m_pData, ptr, size );

	m_nActualSize = size;
	m_nGetIndex = 0;
	m_nPutIndex = size;
}
