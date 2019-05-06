extern "C" {

void* 	pmLock0();
void 		setupTossRet();

void		checkInt( char * note );
#pragma aux checkInt parm [EBX];

void		SetUpSend( void );

void		CleanUpSend( void );

void		SetupCheckInt( void );

void		Clear( void );

void		Apop( void );

void		PushObject( int obj );
#pragma aux PushObject parm [EBX];

void		PushSelector( int selector );
#pragma aux PushSelector parm [EDX];

void pmDoNew ( ObjectID obj );
#pragma aux pmDoNew parm [EBX];

void		asm_fatal(int which, int parm1, int parm2, int parm3);
#pragma aux asm_fatal parm [EDX] [EAX] [EBX] [ECX];
}

extern "C" void *pmFoundObj;


