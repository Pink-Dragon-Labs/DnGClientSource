// sound.hpp

#ifndef SOUND_HPP
#define SOUND_HPP

// maximum number of sounds effects that can play at one time
#define _MAX_SFX	100

// maximum number of MIDI files that can play at one time
#define _MAX_MIDI	16

// maximum number of sound IDs that exist
#define _MAX_SND_ID	5000

enum {
	_SOUND_MIDI = 1,
	_SOUND_WAVE
};

// maximum number of sound resources that can be loaded
#define _MAX_SND_RES		256

typedef struct
{
	char name[16];
	void far *data;
	int size, refCount;
} snd_t;

class SoundResMgr 
{
public:
	SoundResMgr();
	virtual ~SoundResMgr();

	//
	// This will load a resource into RAM if it is not alread in RAM.
	// If the resource is in RAM it's referenceCount will be increased and
	// a pointer to the resource will be returned.  Be sure to call unload
	// on the returned pointer when you are all done with it.
	//
	void far *load ( char *name );

	// 
	// This will decrement the reference count of the passed resource by
	// one.
	//
	void unload ( void far *ptr );

	// data members
	int count;
	snd_t resources[_MAX_SND_RES];
};

class SoundMgr
{
public:
	SoundMgr();
	virtual ~SoundMgr();

	// start a sound playing
	int play ( char *name, int loopCount = 1 );

	// stop a sound from playing
	int stop ( int sound );

	// set the panpot value for a sound
	int setPan ( int sound, int pan );

	// set the volume for a sound
	int setVolume ( int sound, int volume );

	// set the loopcount for a sound
	int setLoopCount ( int sound, int count );

	// sndID management
	int allocateSndID ( void );
	void freeSndID ( int id );

	// return a handle to a sfx
	void far *findSfxByID ( int id );

	// return a handle to a midi
	void far *findMIDIByID ( int id );

	// data members
	void far *dig;
	void far *mid;
	void far *dls;

	// table of sample handles
	void far *sfxTbl[_MAX_SFX];

	// table of sample resource pointers
	void far *sfxDataTbl[_MAX_SFX];

	// table of sample IDs
	int sfxIDTbl[_MAX_SFX];

	// table of MIDI handles
	void far *midiTbl[_MAX_MIDI];

	// table of MIDI resource pointers
	void far *midiDataTbl[_MAX_MIDI];

	// table of sample IDs
	int midiIDTbl[_MAX_MIDI];

	// current sound ID number
	int curSndID;

	// table to track usage of the sndIDs
	char sndIDTrackingTbl[_MAX_SND_ID];

	// resource loader
	SoundResMgr resMgr;
};

extern SoundMgr *soundMgr;

#endif
