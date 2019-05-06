#include "stdafx.h"
#include "Thread.hpp"

#include <process.h>

void WorkerThread::InitThread( void ) {
	m_thread_addr = 0;
	m_thread = 0;
	m_total = m_done = -1;
	m_mutex = 0;
	m_global_mutex = CreateMutex( NULL, FALSE, "UTHREAD_GLOBAL_MUTEX" );
	m_kill_flag = 0;
}

WorkerThread::WorkerThread( void ) {
	InitThread();
}

WorkerThread::~WorkerThread( void ) {
	CloseThreadHandle();
	if (m_mutex)
		CloseHandle(m_mutex);
}

void WorkerThread::CloseThreadHandle( void ) {
	if (m_thread) {
		CloseHandle(m_thread);
		m_thread = 0;
	}
}

int WorkerThread::CreateThread( void ) {
	int result = 0;

	if (!IsRunning()) {
		if (m_global_mutex) {
			WaitForSingleObject( m_global_mutex, INFINITE );
		}

		  // come up with an un-used mutex name
		char base_name[256];
		char temp_name[256];
		char temp[256];

		memset(base_name,0,(size_t)256);
		memset(temp_name,0,(size_t)256);
		memset(temp,0,(size_t)256);

		strcpy(base_name,"WorkerThreadMutex");

		int index = 0;

		while ( TRUE ) {
			strcpy( temp_name, base_name );
			sprintf( temp, "%d", index );
			index++;
			strcat( temp_name, temp );

			HANDLE h = OpenMutex( MUTEX_ALL_ACCESS, FALSE, temp_name );

			if ( h == NULL ) {
				strcpy( m_mutex_name, temp_name );
				h = CreateMutex( NULL, FALSE, m_mutex_name );
				m_mutex = h;
				break;
			} else {
				CloseHandle(h);
			}

			index++;
		}

		if ( m_global_mutex ) {
			ReleaseMutex( m_global_mutex );
		}

		m_thread = (HANDLE) _beginthreadex( (void *) NULL, (unsigned) 0, (PTHREAD_START) WorkerThreadFunction, (void *) this, (unsigned) 0, (unsigned *) &m_thread_addr );
		
		if (m_thread)
			result++;
	}

	return result;
}

int WorkerThread::SetPriority(int priority) {
	int result = 0;

	if ( IsRunning() ) {
		if ( ::SetThreadPriority( m_thread, priority ) ) {
			result++;
		}
	}

	return result;
}

int WorkerThread::SetThreadPriority( WorkerThreadPriorities pri ) {
	int result = 0;

	if ( IsRunning() ) {
		switch ( pri ) {
			case IdlePriority:
				::SetThreadPriority( m_thread, THREAD_PRIORITY_IDLE );
				break;
			case LowPriority:
				::SetThreadPriority( m_thread, THREAD_PRIORITY_BELOW_NORMAL );
				break;
			case NormalPriority:
				::SetThreadPriority( m_thread, THREAD_PRIORITY_NORMAL );
				break;
			case HighPriority:
				::SetThreadPriority( m_thread, THREAD_PRIORITY_ABOVE_NORMAL );
				break;
			case VeryHighPriority:
				::SetThreadPriority( m_thread, THREAD_PRIORITY_HIGHEST );
				break;
		}
	}

	return result;
}

int WorkerThread::IsCreated( void ) const {
	return ( m_thread > 0 );
}

int WorkerThread::IsRunning( void ) const {
	int result = IsCreated();

	if ( result ) {
		DWORD status;
		int code = GetExitCodeThread( m_thread, &status );

		if ( status != STILL_ACTIVE ) {
			result = 0;
		}
	}

	return result;
}

int WorkerThread::Terminate( void ) {
	int result = IsRunning();

	if (result) {
		DWORD status = 1;
		int code = TerminateThread( m_thread, status );
		if ( code == 0 ) {
			result = 0;
		}
	}

	return result;
}

int WorkerThread::GetTotal( void ) const {
	int result = 0;

	if (m_mutex)
		WaitForSingleObject( m_mutex, INFINITE );

	result = m_total;

	if (m_mutex)
		ReleaseMutex( m_mutex );

	return result;
}

int WorkerThread::GetProgress( void ) const {
	int result = 0;

	if (m_mutex)
		WaitForSingleObject( m_mutex, INFINITE );

	result = m_done;

	if (m_mutex)
		ReleaseMutex( m_mutex );

	return result;
}

void WorkerThread::SetProgress( int progress ) {
	if (m_mutex)
		WaitForSingleObject( m_mutex, INFINITE );

	m_done = progress;

	if (m_mutex)
		ReleaseMutex( m_mutex );
}

void  WorkerThread::SetTotal( int total ) {
	if (m_mutex)
		WaitForSingleObject( m_mutex, INFINITE );

	m_total = total;

	if (m_mutex)
		ReleaseMutex( m_mutex );
}

void WorkerThread::SetKillFlag( void ) {
	WaitForSingleObject( m_mutex, INFINITE );

	m_kill_flag++;

	ReleaseMutex( m_mutex );
}

void WorkerThread::ResetKillFlag( void ) {
	WaitForSingleObject( m_mutex, INFINITE );

	m_kill_flag = 0;

	ReleaseMutex( m_mutex );
}

int WorkerThread::CheckKillFlag( void ) {
	int result = 0;

	WaitForSingleObject( m_mutex, INFINITE );

	result = m_kill_flag;

	ReleaseMutex( m_mutex );

	return result;
}

void WorkerThread::Wait( void ) const {
	WaitForSingleObject( m_mutex, INFINITE );
}

void WorkerThread::Release( void ) const {
	ReleaseMutex( m_mutex );
}

int WorkerThread::RunThread( void ) {
	int result = 0;

	while ( !CheckKillFlag() ){
		Sleep(500);
	}

	return result;
}

unsigned int WINAPI WorkerThreadFunction( LPVOID lpThreadParameter ) {
	unsigned int result = 0;

	WorkerThread * ptr = (WorkerThread *) lpThreadParameter;
	result = ptr->RunThread();

	return result;
}
