#define INCLUDE_MMSYSTEM_H
#include "sciwin.hpp"
#include "sol.hpp"
#include "scidlg.hpp"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include "event.hpp"
#include "audio.hpp"
#include "audiow.hpp"
#include "config.hpp"
#include "graphmw.hpp"
#include "kernel.hpp"
#include "memmgrw.hpp"
#include "mousew.hpp"
#include "moview.hpp"
#include "sci.hpp"
#include "kbdmgrw.hpp"
#include "pmachine.hpp"
#include "sound.hpp"
#include "soundwin.hpp"
#include "timew.hpp"
#include "mss.h"
#include "winthunk.hpp"
#include "networkw.hpp"

//#define _SOUND_ENABLED  1

SoundMgr *soundMgr = NULL;
char gSoundDir[128] = ".";
int gSoundMemory = 0;

extern int directX;


#ifdef _SOUND_ENABLED

void AILCALLBACK sampleDoneCallback ( HSAMPLE sample )
{
	for ( int i=0; i<_MAX_SFX; i++ ) {
		if ( soundMgr->sfxTbl[i] == sample ) {
			soundMgr->resMgr.unload ( soundMgr->sfxDataTbl[i] );
			soundMgr->sfxDataTbl[i] = NULL;

			// clear my id for future use
			int id = soundMgr->sfxIDTbl[i];
			soundMgr->sfxIDTbl[i] = 0;
			soundMgr->freeSndID ( id );

			return;
		}
	}
}

void AILEXPORT sequenceDoneCallback ( HSEQUENCE sequence )
{
	for ( int i=0; i<_MAX_MIDI; i++ ) {
		if ( soundMgr->midiTbl[i] == sequence ) {
			soundMgr->resMgr.unload ( soundMgr->midiDataTbl[i] );
			soundMgr->midiDataTbl[i] = NULL;

			// clear my id for future use
			int id = soundMgr->midiIDTbl[i];
			soundMgr->midiIDTbl[i] = 0;
			soundMgr->freeSndID ( id );

			return;
		}
	}
}
#endif


SoundMgr::SoundMgr()
{
#ifdef _SOUND_ENABLED
	dig = 0;
	mid = 0;
	dls = 0;

	AIL_startup();

	if ( !directX ) 
		AIL_set_preference ( DIG_USE_WAVEOUT, 1 );

	// return value handler
	int ret = 0;

	// try to initialize the audio system
//	AIL_quick_startup ( 0, 1, 11025, 8, 1 );
	AIL_quick_startup ( 1, 0, 11025, 8, 1 );

//	AIL_quick_handles ( (HDIGDRIVER *)&dig, (HMDIDRIVER *)&mid );
	AIL_quick_handles ( (HDIGDRIVER *)&dig, (HMDIDRIVER *)&mid , (HDLSDEVICE *)&dls );

	if ( dig ) {
		for ( int i=0; i<_MAX_SFX; i++ ) {
			sfxTbl[i] = AIL_allocate_sample_handle ( (HDIGDRIVER)dig );
			AIL_init_sample ( (HSAMPLE)sfxTbl[i] );
			sfxDataTbl[i] = NULL;
			sfxIDTbl[i] = -1;
		}
	}

	if ( mid ) {
		for ( int i=0; i<_MAX_MIDI; i++ ) {
			midiTbl[i] = AIL_allocate_sequence_handle ( (HMDIDRIVER)mid );

			midiDataTbl[i] = NULL;
			midiIDTbl[i] = -1;
		}
	}

	// complain that the digital system did not initialize properly
	if ( !dig ) 
		MessageBox ( GetFocus(), "We could not initialize your sound effects device.  This is usually caused by some conflict with another program that uses sound effects that is running at the same time as The Realm.  This has also been caused by some Sound Blaster AWE 32/64 cards that have outdated drivers.  Sound effects have been disabled.", "Initialization Error", MB_OK );

#if 0
	if ( !mid || !dig ) {
		// complain that the digital system did not initialize properly
		if ( mid ) {
			MessageBox ( GetFocus(), "We could not initialize your sound effects device.  This is usually caused by some conflict with another program that uses sound effects that is running at the same time as The Realm.  This has also been caused by some Sound Blaster AWE 32/64 cards that have outdated drivers.  Sound effects have been disabled.", "Initialization Error", MB_OK );
		}

		else if ( dig ) {
			MessageBox ( GetFocus(), "We could not initialize your MIDI device.  This is usually caused by some conflict with another program that uses MIDI music that is running at the same time as The Realm.  This has also been caused by some Sound Blaster AWE 32/64 cards that have outdated drivers.  MIDI music has been disabled.", "Initialization Error", MB_OK );
		} 

		else {
			MessageBox ( GetFocus(), "We could not initialize your MIDI device or your sound effects device.  This is usually caused by some conflict with another program that is using your sound card running at the same time as The Realm.  Sound support has been disabled.", "Initialization Error", MB_OK );
		}
	}
#endif

	curSndID = 0;
	memset ( sndIDTrackingTbl, 0, sizeof ( sndIDTrackingTbl ) );
#endif
}

SoundMgr::~SoundMgr()
{
#ifdef _SOUND_ENABLED
	AIL_quick_shutdown();
	AIL_shutdown();
#endif
}

// allocate an ID from the sndIDTrackingTbl
int SoundMgr::allocateSndID ( void )
{
	int index = curSndID;

	while ( 1 ) {
		if ( index > _MAX_SND_ID )
			index = 0;

		if ( !sndIDTrackingTbl[index] ) {
			sndIDTrackingTbl[index] = 1;
			curSndID = index + 1;

			return index;
		}

		index++;
	}

	return -1;
}

// free an allocated sndID
void SoundMgr::freeSndID ( int id )
{
	sndIDTrackingTbl[id] = 0;
}

// play a sound 
int SoundMgr::play ( char *name, int loopCount )
{
#ifdef _SOUND_ENABLED
	if ( dig && strstr ( name, ".wav" ) ) {
		HSAMPLE sample = NULL;

		for ( int i=0; i<_MAX_SFX; i++ ) {
			sample = (HSAMPLE)sfxTbl[i];

			if ( sfxDataTbl[i] == NULL ) {
				break;
			}

			sample = NULL;
		}

		if ( sample ) {
			// find a new sfxID
			int id = allocateSndID();
			sfxIDTbl[i] = id;
			
			void far *data = resMgr.load ( name );
			sfxDataTbl[i] = data;

			AIL_init_sample ( sample );
			AIL_set_sample_file ( sample, data, 0 );
			AIL_register_EOS_callback ( sample, sampleDoneCallback );
			AIL_set_sample_loop_count ( sample, loopCount );
			AIL_start_sample ( sample );

			return id;
		}
	}

	else if ( strstr ( name, ".xmi" ) ) {
		char filename[1024];
		strcpy ( filename, name );

		char *ptr = strstr ( filename, ".xmi" );
		*ptr = 0;

		strcat ( filename, ".mid" );

		mciSendString ( "close music", NULL, 0, NULL );

		char command[1024];
		sprintf ( command, "open %s alias music", filename );
		mciSendString ( command, NULL, 0, NULL );
		mciSendString ( "play music", NULL, 0, NULL );


#if 0
		HSEQUENCE sequence = NULL;

		for ( int i=0; i<_MAX_MIDI; i++ ) {
			sequence = (HSEQUENCE)midiTbl[i];

			if ( midiDataTbl[i] == NULL ) {
				break;
			}

			sequence = NULL;
		}

		if ( sequence ) {
			// find a new sndID
			int id = allocateSndID();
			midiIDTbl[i] = id;
			
			void far *data = resMgr.load ( name );
			midiDataTbl[i] = data;

			AIL_init_sequence ( sequence, data, 0 );
			AIL_register_sequence_callback ( sequence, sequenceDoneCallback );
			AIL_set_sequence_loop_count ( sequence, loopCount );
			AIL_start_sequence ( sequence );

			return id;
		}
#endif
	}
#endif

	return -1;
}

void far *SoundMgr::findSfxByID ( int id )
{
	for ( int i=0; i<_MAX_SFX; i++ ) {
		if ( sfxIDTbl[i] == id )
			return sfxTbl[i];
	}

	return NULL;
}

void far *SoundMgr::findMIDIByID ( int id )
{
	for ( int i=0; i<_MAX_MIDI; i++ ) {
		if ( midiIDTbl[i] == id )
			return midiTbl[i];
	}

	return NULL;
}

int SoundMgr::stop ( int handle ) 
{
#ifdef _SOUND_ENABLED
	HSAMPLE sample = (HSAMPLE)findSfxByID ( handle );

	if ( sample ) {
		AIL_end_sample ( sample );
		return 1;
	}

	HSEQUENCE sequence = (HSEQUENCE)findMIDIByID ( handle );

	if ( sequence ) {
		AIL_end_sequence ( sequence );
		return 1;
	}
#endif

	return -1;
}

int SoundMgr::setPan ( int handle, int pan ) 
{
#ifdef _SOUND_ENABLED
	HSAMPLE sample = (HSAMPLE)findSfxByID ( handle );

	if ( sample ) {
		AIL_set_sample_pan ( sample, pan );
		return 1;
	}
#endif

	return -1;
}

int SoundMgr::setVolume ( int handle, int volume ) 
{
#ifdef _SOUND_ENABLED
	HSAMPLE sample = (HSAMPLE)findSfxByID ( handle );

	if ( sample ) {
		AIL_set_sample_volume ( sample, volume );
		return 1;
	}

	HSEQUENCE sequence = (HSEQUENCE)findMIDIByID ( handle );

	if ( sequence ) {
		AIL_set_sequence_volume ( sequence, volume, 0 );
		return 1;
	}
#endif

	return -1;
}

int SoundMgr::setLoopCount ( int handle, int count ) 
{
#ifdef _SOUND_ENABLED
	HSAMPLE sample = (HSAMPLE)findSfxByID ( handle );

	if ( sample ) {
		AIL_set_sample_loop_count ( sample, count );
		return 1;
	}

	HSEQUENCE sequence = (HSEQUENCE)findMIDIByID ( handle );

	if ( sequence ) {
		AIL_set_sequence_loop_count ( sequence, count );
		return 1;
	}
#endif

	return -1;
}

SoundResMgr::SoundResMgr()
{
	count = 0;
	memset ( &resources, 0, sizeof ( resources ) );
}

SoundResMgr::~SoundResMgr()
{
}

void far *SoundResMgr::load ( char *name )
{
#ifdef _SOUND_ENABLED
	// look for the resource to already be loaded
	for ( int i=0; i<_MAX_SND_RES; i++ ) {
		if ( resources[i].data != NULL && !strcmp ( resources[i].name, name ) ) {
			resources[i].refCount++;
			return resources[i].data;
		}
	}

	// scan and free sound resources until we have less than 500K taken up
	while ( gSoundMemory > (500 * 1024) ) {
		for ( i=0; i<_MAX_SND_RES; i++ ) {
			if ( (resources[i].data != NULL) && !resources[i].refCount ) {
				AIL_mem_free_lock ( resources[i].data );
				resources[i].data = NULL;

				gSoundMemory -= resources[i].size;

				break;
			}
		}

		if ( i >= _MAX_SND_RES )
			break;
	}

	// look for a free space in the resource list
	for ( i=0; i<_MAX_SND_RES; i++ ) {
		if ( resources[i].data == NULL ) {
			char filename[1024];
			sprintf ( filename, "%s\\%s", gSoundDir, name );

			strcpy ( resources[i].name, name );
			resources[i].refCount = 1;
			resources[i].data = (void far *)AIL_file_read ( filename, NULL );

			int size = AIL_file_size ( filename );
			resources[i].size = size;
			gSoundMemory += size;

			return resources[i].data;
		}
	}
#endif

	return NULL;
}

void SoundResMgr::unload ( void far *ptr )
{
	for ( int i=0; i<_MAX_SND_RES; i++ ) {
		if ( resources[i].data == ptr ) {
			resources[i].refCount--;
			break;
		}
	}
}

void KSound ( argList )
{
	enum {
		SPlay,
		SStop,
		SLock,
		SUnlock,
		SSetPan,
		SSetPanPosn,
		SSetLoopCount,
		SDoFootStep,
		SMax
	};

	switch ( arg ( 1 ) ) {
		case SPlay: {
			MemID file = (MemID)arg(2);
			pm.acc = soundMgr->play ( (char *)*file, (argCount > 2)? arg(3) : 1 );
		}

		break;

		case SStop: {
			pm.acc = soundMgr->stop ( arg(2) );
		}

		break;

		case SSetPan: {
			pm.acc = soundMgr->setPan ( arg(2), arg(3) );
		}

		break;

		case SSetPanPosn: {
			pm.acc = soundMgr->setPan ( arg(2), (((arg(3) * 100) / 640) * 127) / 100 );

			if ( arg(5) )
				soundMgr->setVolume ( arg(2), (((arg(4) * 100) / arg(5)) * 127) / 100 );
		}

		break;

		case SSetLoopCount: {
			pm.acc = soundMgr->setLoopCount ( arg(2), arg(3) );
		}

		break;

		case SDoFootStep: {
			int view = (int)((unsigned short)arg(2));
			int cel = arg(3);
			int x = arg(4);
			int scaleX = arg(5);
			int frontSize = arg(6);

			char *name = NULL;

			switch ( view ) {
				case 100:
				case 200: {
					if ( (cel % 5) == 1 ) {
						static char *names[] = { "step1.wav", "step2.wav", "step3.wav" };
						name = names[rand() % 3];
					}
				}

				break;

				case 40200: {
					if ( (cel % 6) == 3 ) 
						name = "trmove.wav";
				}

				break;
			}

			if ( name ) {
				int id = soundMgr->play ( name, 1 );
				soundMgr->setPan ( id, (((x * 100) / 640) * 127) / 100 );
				soundMgr->setVolume ( id, (((scaleX * 100) / frontSize) * 127) / 100 );
			}
		}

		break;
	}
}



