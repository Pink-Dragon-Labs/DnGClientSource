//	Thread.hpp
//

#ifndef THREAD__HPP
#define THREAD__HPP

class WorkerThread {
public:
	enum WorkerThreadPriorities {
		IdlePriority = 1,
		LowPriority,
		NormalPriority,
		HighPriority,
		VeryHighPriority
	};

private:
	void InitThread(void);

protected:
	DWORD*	m_thread_addr;
	HANDLE	m_thread;
	LPVOID	m_lpParam;
	int		m_total;
	int		m_done;
	char	m_mutex_name[256];
	HANDLE	m_mutex;
	HANDLE	m_global_mutex;
	int		m_kill_flag;

public:
	WorkerThread(void);
	~WorkerThread(void);

	const char*	GetMutexName( void ) const { return m_mutex_name; }

	void		CloseThreadHandle( void );
	int			CreateThread( void );

	int			SetPriority( int priority );
	int			SetThreadPriority( WorkerThreadPriorities pri );

	int			IsCreated( void ) const;
	int			IsRunning( void ) const;

	int			Terminate( void );

	virtual int GetTotal( void ) const;
	void		SetTotal( int total );

	virtual int GetProgress( void ) const;
	void		SetProgress( int progress );

	void		SetKillFlag( void );
	void		ResetKillFlag( void );
	int			CheckKillFlag( void );

	void		Wait( void ) const;
	void		Release( void ) const;
	
	  // override this puppy for derived functionality
	virtual int RunThread( void );
};

unsigned int WINAPI WorkerThreadFunction( LPVOID lpThreadParameter );

///////////////////////////// chBEGINTHREADEX Macro ///////////////////////////


// Create a chBEGINTHREADEX macro that calls the C run-time's
// _beginthreadex function. The C run-time library doesn't
// want to have any reliance on Win32 data types such as
// HANDLE. This means that a Win32 programmer needs to cast
// the return value to a HANDLE. This is terribly inconvenient,
// so I have created this macro to perform the casting.
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define chBEGINTHREADEX(lpsa, cbStack, lpStartAddr, lpvThreadParm, fdwCreate, lpIDThread)            \
      ( (HANDLE) _beginthreadex( (void *) (lpsa), (unsigned) (cbStack), (PTHREAD_START) (lpStartAddr), (void *) (lpvThreadParm), (unsigned) (fdwCreate), (unsigned *) (lpIDThread) ) )

#endif
