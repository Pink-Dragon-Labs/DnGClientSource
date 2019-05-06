// audio.cpp
//		digital audio sample playback

#include <stdio.h>
#include <sys\stat.h>

#include "sol.hpp"

#include "integ.hpp"
#include "audio.hpp"
#include "audiow.hpp"
#include "config.hpp"
#include "dos.hpp"
#include "graphmgr.hpp"
#include	"kernel.hpp"
#include "memid.hpp"
#include "memmgr.hpp"
#include "msg.hpp"
#include "newroom.hpp"
#include "pmachine.hpp"
#include "resource.hpp"
#include	"sync.hpp"
#include	"time.hpp"

#include "event.hpp"
#ifdef DEBUG
#include	"kbdmgr.hpp"
#include	"msg.hpp"
#endif

extern "C" {
int Merge(void *, void *, int, char, char);
void Distort(void *, int, char, char);
void Volume(void *, int, char, char, char);
void Silence(void *, int, char);
char Decomp4To8(void *, void *, int, char);
int  Decomp8To16(void *, void *, int, int);
void Conv16To8(void *, void *, int);
void Conv8To16(void *, int);
void Conv11KTo22K(void *, int, char);
void Conv22KTo11K(void *, void *, int, char);
void CopyEOS(void *, void *, int, char);
void DupeChannel(void *, int, int, char);
int  MonitorNoise(void *, int, char);
}

#pragma aux (PushArgs) Merge;
#pragma aux (PushArgs) Distort;
#pragma aux (PushArgs) Volume;
#pragma aux (PushArgs) Silence;
#pragma aux (PushArgs) Decomp4To8;
#pragma aux (PushArgs) Decomp8To16;
#pragma aux (PushArgs) Conv16To8;
#pragma aux (PushArgs) Conv8To16;
#pragma aux (PushArgs) Conv11KTo22K;
#pragma aux (PushArgs) Conv22KTo11K;
#pragma aux (PushArgs) CopyEOS;
#pragma aux (PushArgs) DupeChannel;
#pragma aux (PushArgs) MonitorNoise;

const unsigned MAXSAMPLES = 5;
const unsigned MAXVOLUME = 127;
#define AUDIO_TEMP MOVEABLE+TRANSITORY
#define AUDIO_PERM MOVEABLE+PERSIST

struct Sample {
	int		attrs;
	uchar		bits;
	int		bytesBuffered;
	int		bytesToRead;
	uchar		channels;
	Bool		compressed;
	int		compVal16;
	uchar		compVal8;
	uchar		cond;
	int		distortion;
	int		div;
	Bool		do16Conv8;
	Bool		do8Conv16;
	Bool		do11KConv22K;
	Bool		do22KConv11K;
	Bool		doDecomp;
	int		fadeBytes;
	Bool		fadeDone;
	int		fadeEnd;
	int		fadeIncr;
	int		fadeStart;
	int		fadeSteps;
	int		fadeVol;
	int		fd;
	int		fileOfs;
	int		filled;
	int		leadIn;
	int		length;
	Bool		loop;
	MemID		memID;
	int		memOfs;
	ResNum	module;
	int		mul;
	uchar		noun;
	int		nowPlaying;
	int		num;
	Bool		patch;
	int		pausedLoc;
	int		rate;
	Bool		robot;
	Bool		scrambled;
	uchar		sequ;
	int		start;
	int		startLoc;
	MemID		submitID;
	int		submitted;
	int		tag;
	int		ticks;
	uchar		verb;
	int		volume;
};
Sample sample[MAXSAMPLES];

RobotAudInfo rbot;

#define	ROBOT_RATE		22050
#define	ROBOT_BITS		16
#define	ROBOT_CHANNELS	1

int		activeSamples = 0;
Bool		audioInstalled = False;
int		audVolFD = -1;
int		bytesPerTick;
int		cbTickCount;
MemID		convID = 0;
Bool		critical = False;
Bool		DACCritical = False;
int		distortion = 0;
int		dontDrop = False;
int		ffloc = -1;
int		globalLoc;
int		ioSize;
int		leadSubmits;
int		maxSubmits;
int		minSubmits;
Bool		mixCheck = True;
int		nowPlaying = 0;
int		numSubmits;
int		pausedLoc = 0;
int		preLoad = 0;
int		prioritySample = -1;
int		rate = 0, bits = 0, channels = 0, volume = MAXVOLUME;
int		rateMax, bitsMax, channelsMax, volumeMax;
Bool		rateSet = False, bitsSet = False, channelsSet = False;
int		robotStart;
Bool		robotsActive = False;
Bool		robotsPaused = False;
int		sfxVolFD = -1;
Bool		staticFilter = False;
MemID		submitID = 0;
int		submitSize;
int		submitted = 0;

void		AddSubmit(int);
void		AudioBits(int);
void		AudioChannels(int);
void		AudioDistort(int,int);
int		AudioLoc(int);
void		AudioPlay();
void		AudioRate(int);
int		AudioSelect();
void		AudioUnLoad();
void		AudioVolume(int,int);
int		CheckNoise(void);
void		CheckRawFormat(int);
Bool		CheckSOLFormat(int);
Bool		CheckWAVEFormat(int);
void		ConvBase36(char*,int,int);
void		DACCallBack(int,int);
void		DistortBuffer(int,char*);
void		DropSample(int);
void		FadeBuffer(int,char*);
void		FillRbotBuffer8(int);
void		FillRbotBuffer16(int);
void		FillRead(int);
void		FillSubmit(int,int);
int		FindSampleN(argList,int,int tag = 0);
void		GetConvBuff(int);
char		GetDigit36(int);
void		MergeBuffer(int,int,char*,char*);
char*		MergeBuffers(void);
Bool		RefillBuffer(int);
void		ResubmitBuffers();
Bool		SubmitBuffer(void);
void		VolumeBuffer(int,char*);
#ifdef DEBUG
void		Play();
int		GetBase36(char*,int);
#endif

void
KDoAudio(argList)
{
	int	n, saveCount, tag;
	Bool	pause = False;

	if (!audioInstalled || argCount < 1) {
		pm.acc = 0;
		return;
	}

	PollWaveBuffs();
	switch (arg(1)) {
		case DACFOUND:
			pm.acc = 1;
			break;
		case PRELOAD:
			if (argCount > 1)
				preLoad = arg(2);
			pm.acc = preLoad;
			break;
		case WPLAY:
			pause = True;
			// fall through to case PLAY:
		case PLAY:   /*  audio playback begins  */
			if (argCount < 2) {
				pm.acc = activeSamples;
				break;
			}
			if ((n = FindSampleN(args,0)) != NO_SAMPLES && sample[n].pausedLoc) {
				AudioResume(n);
				pm.acc = sample[n].ticks;
				break;
			}
			if (activeSamples == MAXSAMPLES) {
				pm.acc = 0;
				break;
			}
			n = activeSamples;
			if (argCount < 6) {
				sample[n].module = SFXMODNUM;
				sample[n].num = arg(2);
				if (argCount >= 3 && arg(3) != 1)
					sample[n].loop = arg(3);
				else
					sample[n].loop = False;
				if (argCount >= 4 && arg(4) >= 0 && arg(4) <= MAXVOLUME)
					sample[n].volume = arg(4);
				else {
					sample[n].volume = MAXVOLUME;
					if (argCount >= 4)
						prioritySample = n;
				}
				if (argCount == 5)
					sample[n].tag = arg(5);
				else
					sample[n].tag = 0;
			} else {
				sample[n].num = 0;
				sample[n].module = arg(2);
				sample[n].noun = (uchar)arg(3);
				sample[n].verb = (uchar)arg(4);
				sample[n].cond = (uchar)arg(5);
				sample[n].sequ = (uchar)arg(6);
				if (argCount >= 7 && arg(7) != 1)
					sample[n].loop = arg(7);
				else
					sample[n].loop = False;
				if (argCount >= 8 && arg(8) >= 0 && arg(8) <= MAXVOLUME)
					sample[n].volume = arg(8);
				else {
					sample[n].volume = MAXVOLUME;
					if (argCount >= 8)
						prioritySample = n;
				}
			}
			sample[n].robot = False;
			if (pm.acc = sample[n].ticks = AudioSelect()) {
				sample[n].pausedLoc = pause? 1 : 0;
				activeSamples++;
				AudioPlay();  // activeSamples may change !!!
				n = activeSamples - 1;
				if (pause)
					sample[n].pausedLoc = sample[n].startLoc;					
			} else if (prioritySample == n)
				prioritySample = -1;
			break;
		case STOP:
			pm.acc = activeSamples;
			n = FindSampleN(args,0,argCount>2?arg(3):0);
			saveCount = activeSamples;
			AudioStop(n);
			if (saveCount != activeSamples)
				ResubmitBuffers();
			break;
		case LOOP:
			n = FindSampleN(args,1,argCount>3?arg(4):0);
			if (n >= 0 && n < activeSamples)
				if (argCount >= 2 && arg(2) != 1)
					sample[n].loop = arg(2);
				else
					sample[n].loop = 0;
			break;
		case PAUSE:
			pm.acc = AudioPause(FindSampleN(args,0,argCount>2?arg(3):0));
			if (pm.acc)
				ResubmitBuffers();
			break;
		case RESUME:
			pm.acc = AudioResume(FindSampleN(args,0,argCount>2?arg(3):0));
			if (pm.acc)
				ResubmitBuffers();
			break;
		case LOC:
			pm.acc = AudioLoc(FindSampleN(args,0,argCount>2?arg(3):0));
			break;
		case RATE:
			if (argCount > 1) {
				if (arg(2)) {
					rateSet = True;
					AudioRate(arg(2));
				} else
					rateSet = False;
			}
			pm.acc = rate;
			break;
		case BITS:
			if (argCount > 1) {
				if (arg(2)) {
					bitsSet = True;
					AudioBits(arg(2));
				} else
					bitsSet = False;
			}
			pm.acc = bits;
			break;
		case CHANNELS:
			if (argCount > 1) {
				if (arg(2)) {
					channelsSet = True;
					AudioChannels(arg(2));
				} else
					channelsSet = False;
			}
			pm.acc = channels;
			break;
		case DACMIXNORMAL:
			n = mixCheck;
			if (argCount > 1)
				mixCheck = (Bool) arg(2);
			pm.acc = mixCheck;			
			if (n != mixCheck)
				ResubmitBuffers();
			break;
		case DACCRITICAL:
			if (argCount > 1 && !DACCritical)
				critical = (Bool) arg(2);
			pm.acc = critical;			
			break;
		case DISTORT:
			if (argCount > 1) {
				AudioDistort(arg(2),FindSampleN(args,1));
				ResubmitBuffers();
			}
			pm.acc = distortion;			
			break;
		case VOLUME:
			n = FindSampleN(args,1,argCount>3?arg(4):0);
			if (argCount >= 2 && arg(2) != -1 && n != NO_SAMPLES) {
				AudioVolume(arg(2),n);
				ResubmitBuffers();
			}
			if (n >= 0 && n < activeSamples)
				pm.acc = sample[n].volume;			
			else
				pm.acc = volume;			
			break;
		case FADE:
			pm.acc = False;
			if (argCount < 5)
				break;		
			tag = argCount > 6? arg(7):0;
			saveCount = argCount;
			argCount = 2;
			n = FindSampleN(args,0,tag);
			argCount = saveCount;
			if (n < 0 || n >= activeSamples || sample[n].module != SFXMODNUM)
				break;
			sample[n].fadeVol = arg(3);
			sample[n].fadeSteps = arg(5);
			sample[n].fadeDone = argCount >= 6? arg(6) : False;
			sample[n].fadeBytes = (bits/8 * rate * channels * arg(4) / 60) & 0xfffffffe;
			sample[n].fadeIncr = (sample[n].volume - sample[n].fadeVol) /
				sample[n].fadeSteps;
			sample[n].fadeStart = sample[n].nowPlaying * submitSize;
			sample[n].fadeEnd = sample[n].fadeStart +
				sample[n].fadeBytes * sample[n].fadeSteps;
			if (sample[n].volume == sample[n].fadeVol) {
				if (sample[n].fadeDone)
					AudioStop(n);
				break;
			}
			if (sample[n].fadeSteps == 0) {
				if (sample[n].fadeDone) {
					AudioStop(n);
					break;
				}
				AudioVolume(arg(3),n);
			}
			ResubmitBuffers();
			pm.acc = True;
			break;
		case FADE36:
			if (argCount < 9 && argCount != 6) {
				pm.acc = -1;
				break;		
			}
			n = FindSampleN(args,0);
			if (n < 0 || n >= activeSamples) {
				pm.acc = -1;
				break;
			}
			if (argCount > 6 && sample[n].volume == arg(7)) {
				pm.acc = -1;
				break;
			}
			if (argCount > 6 && arg(9) == 0) {
				AudioVolume(arg(9),n);
				pm.acc = 0;
				break;
			}
			if (argCount > 6) {
				sample[n].fadeVol = arg(7);
				sample[n].fadeSteps = arg(9);
				sample[n].fadeDone = argCount >= 10? arg(10) : False;
			} else {
				sample[n].fadeVol = 0;
				sample[n].fadeSteps = 10;
				sample[n].fadeDone = True;
			}
			ResubmitBuffers();
			pm.acc = True;
			break;
		case NOISE:
			pm.acc = (Acc) CheckNoise();
			break;
		default:
			msgMgr->Alert(SrcLoc, Msg_NoAudio);
			pm.acc = 0;
			break;   /*  unexpected audio driver function  */
	}
}

int
FindSampleN(argList,int argn,int tag)
{
	// argCount = # kernel args (subfunction takes one)
	// argn = # additional args prior to possible file number(s)
	int	n;

	// return the sample number if found; else return activeSamples

	if (argCount - 1 - argn <= 0)
		return ALL_SAMPLES;

	if (!activeSamples)
		return NO_SAMPLES;

	if (argCount - 1 - argn < 5) {
		for (n = 0; n < activeSamples; n++) {
			if (sample[n].num == arg(2+argn) && sample[n].module == SFXMODNUM) {
				if (tag && tag != sample[n].tag)
					continue;
				return n;
			}
		}
	} else for (n = 0; n < activeSamples; n++) {
		if (!sample[n].robot &&
				sample[n].num == 0 &&
				sample[n].module == (ResNum)arg(2+argn) &&
				sample[n].noun == (uchar)arg(3+argn) &&
				sample[n].verb == (uchar)arg(4+argn) &&
				sample[n].cond == (uchar)arg(5+argn) &&
				sample[n].sequ == (uchar)arg(6+argn))
			return n;
	}
	return NO_SAMPLES;
}

Bool
QueryAudRobot(RobotAudStatus *buff)
{
	PollWaveBuffs();
	for (int n = 0; n < activeSamples; n++) {
		if (sample[n].robot) {
			buff->bytesPlayed = nowPlaying * submitSize;
			buff->bytesPlaying = buff->bytesPlayed +
				(timeMgr->GetTickCount() - cbTickCount) * rate * (bits/8) / 60;
			buff->bytesSubmitted = buff->bytesPlayed + sample[n].submitted*submitSize;
			if (buff->bytesPlaying > buff->bytesSubmitted)
				buff->bytesPlaying = buff->bytesSubmitted;
			buff->rate = rate;
			buff->bits = bits;
			return True;
		}
	}
	buff->bytesPlayed = buff->bytesPlaying = buff->bytesSubmitted = 0;
	return False;
}

Bool
AudRobot(RobotAudInfo buff)
{
	int	n;

	if (!audioInstalled)
		return True;
	rbot.adrID = buff.adrID;
	rbot.len = buff.len;
	rbot.floc = buff.floc;
	rbot.ext = rbot.floc + rbot.len*4;
	rbot.channel = rbot.floc % 4 ? 1 : 0;
	if (rbot.floc <= 2 && ffloc == -1 && rbot.len != 0 && rbot.len != -1) {
		robotsActive = False;
		for (n = 0; n < activeSamples; n++)
			if (sample[n].robot) {
				AudioStop(n);
				ResubmitBuffers();
			}
		rbot.hwm[0] = 0;
		rbot.hwm[1] = 2;
		if ((n=activeSamples) == MAXSAMPLES)
			return False;
		sample[n].module = sample[n].num = 0;
		sample[n].length = rbot.len;
		sample[n].start = 0;
		sample[n].loop = False;
		sample[n].volume = MAXVOLUME;
		sample[n].compressed = True;
		sample[n].robot = True;
		if (!AudioSelect())
			return False;
		sample[n].bytesBuffered = 0;
		rbot.memID = sample[n].submitID;
		rbot.blen = rbot.memID.Size();
		rbot.min = 0;
		rbot.max = rbot.blen * (16/bits);
		++activeSamples;
		ffloc = rbot.floc;
	} else {
		for (n = 0; n < activeSamples; n++)
			if (sample[n].robot)
				break;
		if (n == activeSamples)
			return False;
	}
	if (rbot.len == 0) {
		AudioStop(n);
		ResubmitBuffers();
		ffloc = -1;
	} else if (rbot.len == -1) {
		PollWaveBuffs();
		sample[n].filled = (Max(rbot.hwm[0],rbot.hwm[1]) - rbot.min) / submitSize / (16/bits);
sample[n].filled=sample[n].submitted;
		sample[n].bytesToRead = 0;
		ffloc = -1;
	} else {
		if (rbot.ext <= Max(rbot.min,rbot.hwm[rbot.channel]))
			return True;
		if (rbot.max <= rbot.hwm[rbot.channel])
			return False;
		sample[n].bytesToRead = rbot.len;
		int ext = rbot.ext;
		int rmax = rbot.max;
		if (convID.Size() < rbot.len * 2)
			convID.Realloc(rbot.len * 2);
		if (bits == 8)
			FillRbotBuffer8(n);
		else
			FillRbotBuffer16(n);
		sample[n].filled = (Max(rbot.hwm[0],rbot.hwm[1]) - rbot.min) / submitSize / (16/bits);
		if (ffloc != -1 && ffloc != rbot.floc && !robotsPaused) {
			if (!robotsActive) {
				robotsActive = True;
				AudioPlay();
			}
			ffloc = -1;
		}
		if (ext > rmax)
			return False;
	}
	return True;
}

void
FillRbotBuffer8(int n)
{
	int	ofs, ext, len;

	rbot.cofs = 0;
	Decomp8To16(*convID,*rbot.adrID,rbot.len,0);
	rbot.len *= 2;
	int loc = Max(rbot.min+rbot.channel,rbot.hwm[rbot.channel]);
	int mx = rbot.max+rbot.channel;
	if (rbot.floc < loc) {
		rbot.cofs = (loc - rbot.floc) / 2;
		rbot.len -= rbot.cofs;
		rbot.floc = loc;
	}
	if (rbot.floc > mx) {
		rbot.len += (rbot.floc - mx) / 2;
		rbot.floc = mx;
	}
	if (rbot.ext > mx) {
		rbot.len -= (rbot.ext - mx) / 2;
		rbot.ext = mx;
	}

	rbot.len /= 2;
	Conv16To8(&convID[rbot.cofs],&convID[rbot.cofs],rbot.len);

//	int maxhwm = Max(rbot.hwm[0],rbot.hwm[1]);
//	if (rbot.ext > maxhwm)
//		sample[n].bytesBuffered += (rbot.ext - maxhwm) / 2;
n=n;

	if (rbot.floc > rbot.hwm[rbot.channel]) {
		ext = rbot.floc % (rbot.blen * 2);
		if ((rbot.floc&0xfffffffc) > (rbot.hwm[1-rbot.channel]&0xfffffffc)) {
			ofs = rbot.hwm[1-rbot.channel] % (rbot.blen * 2);
			if (ofs >= ext) {
				len = rbot.blen * 2 - ofs;
				Silence(&rbot.memID[ofs/2],len/2,bits);
				ofs = (1-rbot.channel)?2:0;
			}
			len = ext - ofs;
			if (len > 0)
				Silence(&rbot.memID[ofs/2],len/2,bits);
		}
		ofs = rbot.hwm[rbot.channel] % (rbot.blen * 2);
		if (ofs >= ext) {
			len = rbot.blen * 2 - ofs;
			DupeChannel(&rbot.memID[ofs/2],len/4,0,bits);
			ofs = rbot.channel?2:0;
		}
		len = ext - ofs;
		if (len > 0)
			DupeChannel(&rbot.memID[ofs/2],len/4,0,bits);
	}
	if (rbot.len > 0) {
		ofs = rbot.floc % (rbot.blen * 2);
		ext = rbot.ext % (rbot.blen * 2);
		len = 0;
		if (ofs >= ext) {
			len = (rbot.blen * 2 - (ofs & 0xfffffffc))/2;
			CopyEOS(&rbot.memID[ofs/2],&convID[rbot.cofs],len/2,bits);
			ofs = rbot.channel?2:0;
		}
		CopyEOS(&rbot.memID[ofs/2],&convID[rbot.cofs+len/2],(ext-ofs)/4,bits);
	}
	rbot.hwm[rbot.channel] = rbot.ext;
}

void
FillRbotBuffer16(int n)
{
	int	ofs, ext, len;

	rbot.cofs = 0;
	Decomp8To16(*convID,*rbot.adrID,rbot.len,0);
	rbot.len *= 2;
	int loc = Max(rbot.min+rbot.channel*2,rbot.hwm[rbot.channel]);
	int mx = rbot.max+rbot.channel*2;
	if (rbot.floc < loc) {
		rbot.cofs = (loc - rbot.floc) / 2;
		rbot.len -= rbot.cofs;
		rbot.floc = loc;
	}
	if (rbot.floc > mx) {
		rbot.len += (rbot.floc - mx) / 2;
		rbot.floc = mx;
	}
	if (rbot.ext > mx) {
		rbot.len -= (rbot.ext - mx) / 2;
		rbot.ext = mx;
	}

//	int maxhwm = Max(rbot.hwm[0],rbot.hwm[1]);
//	if (rbot.ext > maxhwm)
//		sample[n].bytesBuffered += rbot.ext - maxhwm;
n=n;

	if (rbot.floc > rbot.hwm[rbot.channel]) {
		ext = rbot.floc % rbot.blen;
		if ((rbot.floc&0xfffffffc) > (rbot.hwm[1-rbot.channel]&0xfffffffc)) {
			ofs = rbot.hwm[1-rbot.channel] % rbot.blen;
			if (ofs >= ext) {
				len = rbot.blen - ofs;
				Silence(&rbot.memID[ofs],len,bits);
				ofs = (1-rbot.channel)?2:0;
			}
			len = ext - ofs;
			if (len > 0)
				Silence(&rbot.memID[ofs],len,bits);
		}
		ofs = rbot.hwm[rbot.channel] % rbot.blen;
		if (ofs >= ext) {
			len = rbot.blen - ofs;
			DupeChannel(&rbot.memID[ofs],len/4,0,bits);
			ofs = rbot.channel?2:0;
		}
		len = ext - ofs;
		if (len > 0)
			DupeChannel(&rbot.memID[ofs],len/4,0,bits);
	}
	if (rbot.len > 0) {
		ofs = rbot.floc % rbot.blen;
		ext = rbot.ext % rbot.blen;
		len = 0;
		if (ofs >= ext) {
			len = (rbot.blen - (ofs & 0xfffffffc))/2;
			CopyEOS(&rbot.memID[ofs],&convID[rbot.cofs],len/2,bits);
			ofs = rbot.channel?2:0;
		}
		CopyEOS(&rbot.memID[ofs],&convID[rbot.cofs+len],(ext-ofs)/4,bits);
	}
	rbot.hwm[rbot.channel] = rbot.ext;
}

int
AudioSelect()
{
	int	fd;
	char	pathName[MaxPath + 1];

	dontDrop = True;
	int n = activeSamples;
	sample[n].patch = False;
	sample[n].fd = 0;
	sample[n].memID = 0;
	sample[n].attrs = 0;
	sample[n].fadeSteps = 0;
	sample[n].distortion = 0;
	sample[n].fileOfs = 0;

	if (sample[n].robot) {

		sample[n].rate = ROBOT_RATE;
		sample[n].bits = ROBOT_BITS;
		sample[n].channels = ROBOT_CHANNELS;
		sample[n].scrambled = False;
		sample[n].compressed = True;

	} else if (sample[n].module == SFXMODNUM) {

		// Search for AUD/WAV resource:
		pathName[0] = '\0';

		if ((sample[n].memID = resMgr->Find(MemResAudio,sample[n].num)) ||
			 (sample[n].memID = resMgr->Find(MemResWAVE, sample[n].num))) {
			sample[n].attrs = sample[n].memID.Attrs();
			memMgr->SetNotDiscardable(sample[n].memID);
/*
		int	ndx;
		// FIRST: Search for .AUD file via 'patchDir=' dir list
		} else if ((ndx = FindPatchEntry(MemResAudio, sample[n].num)) != -1) {
			sprintf(pathName, "%s%s", patchDir[ndx], resMgr->MakeName(MemResAudio, sample[n].num));
			sample[n].fd = Open(pathName, 0);
			sample[n].patch = True;

		// SECOND: Search for .WAV file via 'patchDir=' dir list
		} else if ((ndx = FindPatchEntry(MemResWAVE, sample[n].num)) != -1) {
			sprintf(pathName, "%s%s", patchDir[ndx], resMgr->MakeName(MemResWAVE, sample[n].num));
			sample[n].fd = Open(pathName, 0);
			sample[n].patch = True;
*/

		// THIRD: Search for .AUD file via 'audio=' dir list
		} else if ((fd = resMgr->Open(MemResAudio, sample[n].num, pathName)) != -1 &&
				fd != sfxVolFD) {
			sample[n].fd = fd;
			sample[n].patch = True;
			#ifdef DEBUG   
			CheckDiscStreaming(pathName);
			#endif

		// FOURTH: Searh for .WAV file via 'wave=' dir list
		} else if ((fd = resMgr->Open(MemResWAVE, sample[n].num, pathName)) != -1 &&
				fd != sfxVolFD) {
			sample[n].fd = fd;
			sample[n].patch = True;
			#ifdef DEBUG   
			CheckDiscStreaming(pathName);
			#endif

		// LAST CHANCE: Search for AUD/WAV resource in SFX Resource Volume
		} else if ((sample[n].fileOfs = FindAudEntry((ResNum)sample[n].num)) != -1) {
			sample[n].fd = sfxVolFD;
			#ifdef DEBUG   
			sprintf(pathName,"%d from SFX",sample[n].num);
			CheckDiscStreaming(pathName);
			#endif

		// AUD/WAV resource NOT FOUND!
		} else {
			dontDrop = False;
			return 0;
		}

	} else {

		// Search for @ resource:

		// FIRST: Search for @ file via 'audio=' dir list
		MakeName36(MemResAudio, pathName, (ResNum)sample[n].module, sample[n].noun,
			sample[n].verb, sample[n].cond, sample[n].sequ);
		if ((fd = resMgr->Open(MemResAudio36, -1, pathName)) != -1 &&
				fd != audVolFD && fd != sfxVolFD) {
			sample[n].fd = fd;
			sample[n].patch = True;
			#ifdef DEBUG   
			CheckDiscStreaming(pathName);
			#endif

		// LAST CHANCE: Search for @ resource in AUD Resource Volume */
		} else if ((sample[n].fileOfs = FindAud36Entry((ResNum)sample[n].module,
						sample[n].noun,sample[n].verb,sample[n].cond,sample[n].sequ))
					!= -1) {
			sample[n].fd = audVolFD;
			#ifdef DEBUG   
			CheckDiscStreaming(pathName);
			#endif

		// @ resource NOT FOUND!
		} else {
			dontDrop = False;
			return 0;
		}
	}

	if (!sample[n].robot && !CheckSOLFormat(n) && !CheckWAVEFormat(n)) {
		if (sample[n].module == SFXMODNUM)
			msgMgr->Fatal("Invalid audio file format: %d",sample[n].num);
		else
			msgMgr->Fatal("Invalid audio file format: %s",pathName);
	}

	if (n == 0) {
		if (!rateSet)
			AudioRate(sample[n].rate);
		if (!bitsSet)
			AudioBits(sample[n].bits);
		if (!channelsSet)
			AudioChannels(sample[n].channels);
	}
	if (prioritySample == n) {
		bytesPerTick = rate * (bits/8) / 60;
		if (bits == 16)
			bytesPerTick &= 0xfffe;
	}

	sample[n].div = 1;
	sample[n].mul = 1;
	if (sample[n].compressed && !sample[n].robot) {
		sample[n].doDecomp = True;
		sample[n].compVal8 = 0x80;
		sample[n].compVal16 = 0;
		sample[n].div *= 2;
		GetConvBuff(n);
	} else 
		sample[n].doDecomp = False;

	if (sample[n].bits > bits && !sample[n].robot) {
		sample[n].do16Conv8 = True;
		sample[n].mul *= 2;
		if (sample[n].compressed)
			sample[n].mul *= 2;
		GetConvBuff(n);
		if (sample[n].compressed)
			sample[n].div *= 2;
	} else
		sample[n].do16Conv8 = False;

	if (sample[n].bits < bits && !sample[n].robot) {
		sample[n].do8Conv16 = True;
		sample[n].div *= 2;
	} else
		sample[n].do8Conv16 = False;

	if (sample[n].rate > rate && !sample[n].robot) {
		sample[n].do22KConv11K = True;
		sample[n].mul *= 2;
		if (sample[n].compressed)
			sample[n].mul *= 2;
		GetConvBuff(n);
		if (sample[n].compressed)
			sample[n].div *= 2;
	} else
		sample[n].do22KConv11K = False;

	if (sample[n].rate < rate && !sample[n].robot) {
		sample[n].do11KConv22K = True;
		sample[n].div *= 2;
	} else
		sample[n].do11KConv22K = False;

	if (sample[n].fd && preLoad && n+1 >= preLoad) {
		sample[n].memID.Get(MemAudioBuffer, sample[n].length, AUDIO_TEMP+DISCARDABLE);
		sample[n].attrs = sample[n].memID.Attrs();
		memMgr->SetNotDiscardable(sample[n].memID);
		sample[n].memID.Read(sample[n].fd);
		if (sample[n].patch) {
			sample[n].patch = False;
			Close(sample[n].fd);
		}
		sample[n].fd = 0;
		sample[n].start = 0;
	}

	if (sample[n].fd) {
		sample[n].fileOfs = sample[n].start;
		sample[n].bytesToRead = sample[n].length;
		sample[n].memID.Get(MemAudioBuffer, ioSize * 2, AUDIO_TEMP);
		sample[n].memOfs = 0;
		sample[n].bytesBuffered = 0;
		FillRead(n);
	} else {
		sample[n].memOfs = sample[n].start;
		sample[n].bytesBuffered = sample[n].length;
		sample[n].bytesToRead = 0;
	}

	sample[n].submitID.Get(MemAudioBuffer, submitSize*maxSubmits, AUDIO_TEMP);
	sample[n].nowPlaying = 0;
	sample[n].filled = 0;
	sample[n].submitted = 0;
	sample[n].leadIn = 0;

	dontDrop = False;
	return 1 +
			(sample[n].length * (sample[n].doDecomp?2:1) * 60) /
			(sample[n].bits/8 * sample[n].channels * (int)sample[n].rate);
}

void
AudioPlay()
{
	int n, t;

	for (n = 0; n < activeSamples-1; n++)
		if (!sample[n].leadIn &&
			 !sample[n].submitted &&
			 !sample[n].filled &&
			 !sample[n].bytesBuffered &&
			 !sample[n].bytesToRead)
			AudioStop(n--);
	int nn = --activeSamples;
	PollWaveBuffs();

	n = activeSamples++;
	if (n != nn)
		sample[n] = sample[nn];
	if (!n) {
		DACStop();
		submitted = 0;
		nowPlaying = 0;
		cbTickCount = timeMgr->GetTickCount();
		if (pausedLoc)
			globalLoc = sample[0].startLoc = pausedLoc;
		else
			globalLoc = sample[0].startLoc = cbTickCount;
		if (!sample[0].pausedLoc && !pausedLoc)
			SubmitMinBuffers();
	} else if (sample[n].robot && rateSet && bitsSet /*&& !critical*/) {
		if (submitted) {
			DACStop();
			submitted = 0;
			for (t = 0; t < n; t++) {
				sample[t].submitted = 0;
				sample[t].leadIn = 0;
			}
		}
		nowPlaying = 0;
		sample[n].startLoc = cbTickCount = timeMgr->GetTickCount();
		SubmitMinBuffers();
	} else {
		for (t = 0; t < n; t++)
			if (sample[t].robot)
				break; 
		if (critical && t == n) {
			if (submitted) {
				DACStop();
				submitted = 0;
				for (t = 0; t <= n; t++) {
					sample[t].submitted = 0;
					sample[t].leadIn = 0;
				}
			}
			nowPlaying = 0;
			sample[n].startLoc = cbTickCount = timeMgr->GetTickCount();
			SubmitMinBuffers();

			if (pausedLoc)
				sample[n].startLoc = pausedLoc;
		} else {
			sample[n].submitted = sample[n].leadIn = Min(leadSubmits,submitted);
			if (pausedLoc)
				sample[n].startLoc = pausedLoc;
			else
				sample[n].startLoc = timeMgr->GetTickCount() +
					(sample[n].leadIn * submitSize * 60) / (bits/8 * rate * channels);
			ResubmitBuffers();
		}
	}
}

void
GetConvBuff(int n)
{
	int	size;

	if (sample[n].mul > sample[n].div)
		size = submitSize * sample[n].mul / sample[n].div;
	else
		size = submitSize;
	if (convID.Size() < size)
		convID.Realloc(size);
}

#ifdef DEBUG   
void
CheckDiscStreaming(char* str)
{
	char	pathName[MaxPath + 1];
	Bool	dump = False;

	if (configMgr->Get(configMgr->AudioMonitor)) {
		if (!configMgr->Arg(configMgr->AudioMonitor) ||
				configMgr->Val(configMgr->AudioMonitor) == 0)
			return;
		if (configMgr->Val(configMgr->AudioMonitor) == 1)
			dump = True;
	}
	#ifdef ROBOT
	if (graphMgr &&
		 graphMgr->RobotStatus() != ROBOT_UNUSED &&
		 graphMgr->GRobot().IsRobotRealTime()) {
		if (dump) {
			msgMgr->Dump("Loading %s while playing %d.RBT in room #%d\n",
				str,graphMgr->GRobot().GetResNum(),currentRoom);
		} else if (!msgMgr->Alert
				("Loading %s while playing %d.RBT in room #%d\r(Press ESC/No to quit)",
					str,graphMgr->GRobot().GetResNum(),currentRoom)) {
			msgMgr->Fatal("User abort");
		}
	}
	#endif
	for (int n = 0; n < activeSamples; n++) {
		if (sample[n].fd && !sample[n].robot) {
			if (sample[n].module == SFXMODNUM) {
				if (dump) {
					msgMgr->Dump("Loading %s while playing audio #%d in room #%d\n",
						str,sample[n].num,currentRoom);
				} else if (!msgMgr->Alert
					("Loading %s while playing audio #%d in room #%d\r(Press ESC/No to quit)",
						str,sample[n].num,currentRoom)) {
					msgMgr->Fatal("User abort");
				}
			} else {
				MakeName36(MemResAudio, pathName, (ResNum)sample[n].module,
				 sample[n].noun, sample[n].verb, sample[n].cond, sample[n].sequ);
				if (dump) {
					msgMgr->Dump("Loading %s while playing %s in room #%d\n",
						str,pathName,currentRoom);
				} else if (!msgMgr->Alert
						("Loading %s while playing %s in room #%d\r(Press ESC/No to quit)",
							str,pathName,currentRoom)) {
					msgMgr->Fatal("User abort");
				}
			}
		}
	}
	return;
}
#endif

void
MakeName36(MemType type, char* fname, ResNum module, uchar noun,
			  uchar verb, uchar cond, uchar sequ)
{
	if (type == MemResSync)
		fname[0] = 'S';
	else
		fname[0] = 'A';
	if (module >= 36*36*36) {
		fname[0]++;
		module %= 36*36*36;
	}
	ConvBase36(&fname[1],module,3);
	ConvBase36(&fname[4],(int)noun,2);
	ConvBase36(&fname[6],(int)verb,2);
	fname[8] = '.';
	ConvBase36(&fname[9],(int)cond,2);
	ConvBase36(&fname[11],(int)sequ,1);
	fname[12] = '\0';
}

void
ConvBase36(char *str, int num10, int digits)
{
	int	n, t;

	t = 0;
	if (digits >= 3) {
		str[t++] = GetDigit36(n = num10 / (36*36));
		num10 -= n * (36*36);
	}
	if (digits >= 2) {
		str[t++] = GetDigit36(n = num10 / 36);
		num10 %= 36;
	}
	str[t] = GetDigit36(n = num10);
}

char
GetDigit36(int n)
{
	if (n <=  9)
		return((char)('0' + n));
	else
		return((char)('A' + n - 10));
}

int
FindAudEntry(ResNum id)
{
	int				offset;
	MemID				mapID;
	ResAudEntry*	entry;
	static int mapChecksum = -1;

	if (sfxVolFD == -1 || !resMgr)
		return -1;

	if (!resMgr->Check(MemResMap, SFXMODNUM))
		return -1;
	mapID = resMgr->Get(MemResMap, SFXMODNUM);
	mapID.SetNotDiscardable();

	int theSum = 0, theSize = mapID.Size();
	char *ptr = (char *)*mapID;

	while ( theSize ) {
		theSum += *ptr++;
		theSize--;
	}

	if ( mapChecksum == -1 ) {
		mapChecksum = theSum;
	} else {
		if ( mapChecksum != theSum ) 
			msgMgr->Fatal ( "Audio resource map data failed checksum test." );
	}

	offset = 0;
	for (entry = (ResAudEntry *)*mapID; entry->id != (ushort)-1; ++entry) {
		offset += ((ulong)entry->offsetMSB << 16) + (ulong)entry->offsetLSW;
		if	(entry->id == id)
			return offset;
	}
	return -1;
}

int
FindAud36Entry(ResNum module, uchar noun, uchar verb, uchar cond, uchar sequ)
{
	int				offset;
	MemID				mapID;
	char*				ptr36;
	ResAud36Entry*	entry36;

	if (audVolFD == -1)
		return(-1);

	if (!resMgr->Check(MemResMap, module))
		return(-1);
	mapID = resMgr->Get(MemResMap, module);

	ptr36 = (char *)*mapID;
	offset = *(int *)ptr36;
	ptr36 += 4;
	for (entry36 = (ResAud36Entry*)ptr36; entry36->flag.sequ != 255;
			entry36 = (ResAud36Entry*)ptr36) {
		offset += ((ulong)entry36->offsetMSB << 16) + (ulong)entry36->offsetLSW;
		if	(entry36->noun == noun && entry36->verb == verb &&
			 entry36->cond == cond && (entry36->flag.sequ & SEQUMASK) == sequ) {
			if (entry36->flag.sync & SYNCMASK) {
				offset += entry36->syncLen;
				if (entry36->flag.rave & RAVEMASK)
					offset += entry36->raveLen;
			}
			PreloadSync36(module,noun,verb,cond,sequ);
			return(offset);
		}
		ptr36 += sizeof(ResAud36Entry);
		if (!(entry36->flag.sync & SYNCMASK))
			ptr36 -= sizeof(entry36->syncLen);
		if (!(entry36->flag.rave & RAVEMASK))
			ptr36 -= sizeof(entry36->raveLen);
	}
	return -1;
}

void
AudioStop(int n)
{
	int	t;

	if (n == NO_SAMPLES || !activeSamples || dontDrop)
		return;
	if (n == ALL_SAMPLES) {
		// Stop all samples
		for (t = 0; t < activeSamples; t++)
			DropSample(t);
		activeSamples = 0;
	} else {
		DropSample(n);
		--activeSamples;
		if (activeSamples) {
			for (t = n; t < activeSamples; t++) {
				sample[t] = sample[t+1];
				if (t+1 == prioritySample)
					prioritySample = t;
			}
		}
	}
	if (!activeSamples) {
		// No samples remain for playback...
		if (submitted) {
			DACStop();
			submitted = 0;
		}
		nowPlaying = 0;
		convID.Realloc(submitSize);
	}
}

void
DropSample(int n)
{
	if (sample[n].robot)
		robotsActive = False;
	sample[n].submitID.Free();
	if (sample[n].fd) {
		sample[n].memID.Free();
		if (sample[n].patch)
			Close(sample[n].fd);
	} else if (sample[n].attrs & DISCARDABLE)
		memMgr->SetDiscardable(sample[n].memID);
	if (n == prioritySample)
		prioritySample = -1;
}

Bool
AudioPause(int n)
{
	int	ticks = timeMgr->GetTickCount();

	if (n == NO_SAMPLES)
		return False;
	if (n == ALL_SAMPLES) {
		if (!pausedLoc) {
			pausedLoc = ticks;
			return True;
		}
	} else if (n == ROBOTS_ONLY) {
		robotsPaused = True;
		for (n = 0; n < activeSamples; n++) {
			if (sample[n].robot) {
				sample[n].pausedLoc = ticks;
			}
		}
	} else if (!sample[n].pausedLoc) {
		sample[n].pausedLoc = ticks;
		sample[n].submitted = 0;
		return True;
	}
	return False;
}

Bool
AudioResume(int n)
{
	int	t, ticks = timeMgr->GetTickCount();

	if (n == NO_SAMPLES)
		return False;
	if (n == ALL_SAMPLES) {
		if (pausedLoc) {
			for (t = 0; t < activeSamples; t++)
				if (!sample[t].pausedLoc)
					sample[t].startLoc += ticks - pausedLoc;
			globalLoc += ticks - pausedLoc;
			pausedLoc = 0;
			SubmitMinBuffers();
			return True;
		}
	} else if (n == ROBOTS_ONLY) {
		robotsPaused = False;
		for (n = 0; n < activeSamples; n++) {
			if (sample[n].robot) {
				sample[n].startLoc += ticks - sample[n].pausedLoc;
				sample[n].pausedLoc = 0;
				robotsActive = True;
				AudioPlay();
				ffloc = -1;
				SubmitMinBuffers();
				return True;
			}
		}
	} else if (sample[n].pausedLoc) {
		sample[n].startLoc += ticks - sample[n].pausedLoc;
		sample[n].pausedLoc = 0;
		SubmitMinBuffers();
		return True;
	}
	return False;
}

int
AudioLoc(int n)
{
/* Notes on LOC:
	A return value of -1 indicates that no audio is currently playing.
	A return value of 0 through 65534 indicates the time in sixtieths of
	a second that the current audio selection has been playing for.
*/

	int	ticks = timeMgr->GetTickCount();

	if (n == NO_SAMPLES || (!activeSamples && !DACLoc()))
		return -1;
	if (n == ALL_SAMPLES)
		if (pausedLoc)
			ticks = pausedLoc - globalLoc;
		else
			ticks -= globalLoc;
	else if (sample[n].pausedLoc)
		ticks = sample[n].pausedLoc - sample[n].startLoc;
	else if (pausedLoc)
		ticks = pausedLoc - sample[n].startLoc;
	else
		if (sample[n].leadIn && sample[n].startLoc > ticks)
			ticks = 0;
		else
			ticks -= sample[n].startLoc;
	if (ticks >= 65535)
		return 65534;
	if (ticks < 0)
		return 0;
	return ticks;
}

void
AudioRate(int request)
{
	int t = Min(request,rateMax);
	if (rate != t)
		DACSetRate(rate = t);
}

void
AudioBits(int request)
{
	int t = Min(request,bitsMax);
	if (bits != t)
		DACSetBits(bits = t);
}

void
AudioChannels(int request)
{
	int t = Min(request,channelsMax);
	if (channels != t)
		DACSetChannels(channels = t);
}

void
AudioDistort(int request, int n)
{
	if (n == ALL_SAMPLES)
		distortion = request;
	else if (n != NO_SAMPLES)
		sample[n].distortion = request;
}

void
AudioVolume(int request, int n)
{
	if (n == ALL_SAMPLES)
		volume = Min(request,MAXVOLUME);
	else if (n != NO_SAMPLES)
		sample[n].volume = Min(request,MAXVOLUME);
}

int
CheckNoise()
{
	static int check;
	int t, len, bytesPlayed, ofs, n;

	if (prioritySample == -1)
		return 0;
	n = prioritySample;
	if (!sample[n].submitted)
		return 0;
	bytesPlayed = (timeMgr->GetTickCount()-cbTickCount +1) * bytesPerTick;
	len = bytesPerTick;
//	if (len > sample[n].bytesBuffered)
//		len = sample[n].bytesBuffered;

	if (bytesPlayed < submitSize) {
		ofs = bytesPlayed;
ofs+=nowPlaying*submitSize;
		if (bytesPlayed + len <= submitSize) {
			check = MonitorNoise(&submitID[ofs],len,bits);
			return check;
		}
		t = bytesPlayed + len - submitSize;
		check = MonitorNoise(&submitID[ofs],len - t,bits);
		if (check)
			return check;
		bytesPlayed += len - t;
		len = t;
	}

int noiseOfs = 0;
	ofs = noiseOfs + bytesPlayed;
	if (noiseOfs + bytesPlayed + len > sample[n].submitID.Size()) {
		t = noiseOfs + bytesPlayed + len - sample[n].submitID.Size();
		if (t < len) {
			check = MonitorNoise(&sample[n].submitID[ofs],len - t,bits);
			if (check) {
				return check;
			}
			len = t;
			ofs = 0;
		} else {
			ofs = t - len;
		}
	}
	check = MonitorNoise(&sample[n].submitID[ofs],len,bits);

	return check;
}

Bool
CheckSOLFormat(int n)
{
	// If the sample is in SOL format, the file pointer will be left
	// pointing at the first 'playable' byte:

	uchar	header[256], flag;
	uchar*	sptr = 0;

	if (!sample[n].fd) {
		sptr = (uchar*)*sample[n].memID;
		memcpy(header,sptr,6);
		sptr += 6;
	} else {
		LSeek(sample[n].fd,sample[n].fileOfs,SEEK_SET);
		Read(sample[n].fd,header,6);	// 2-byte header + 4-byte signature
	}
	if ((header[0] != (uchar)(MemResAudio|0x80)) || strcmp((char*)&header[2],"SOL"))
		return False;
	if (sptr) {
		sample[n].rate = ((uint)*sptr) + ((uint)(*(sptr+1)) << 8);
		flag = *(sptr+2);
		sample[n].length = ((uint)*(sptr+3)) + ((uint)(*(sptr+4)) << 8) +
			((uint)(*(sptr+5)) << 16) + ((uint)(*(sptr+6)) << 24);
		sample[n].start = (int)header[1]+2;
	} else {
		Read(sample[n].fd,&sample[n].rate,2);
		Read(sample[n].fd,&flag,1);
		Read(sample[n].fd,&sample[n].length,4);
		LSeek(sample[n].fd,(int)header[1]-11,SEEK_CUR);
		sample[n].start = LSeek(sample[n].fd,0,SEEK_CUR);
	}
	sample[n].bits = flag & 4 ? 16 : 8;
	sample[n].channels = flag & 16 ? 2 : 1;
	sample[n].compressed = flag & 1;
	sample[n].scrambled = flag & 2;
	sample[n].length &= 0xFFFFFFFC;
	return True;
}

Bool
CheckWAVEFormat(int n)
{
	// If the sample is in WAVE format, the file pointer will be left
	// pointing at the first 'playable' byte:

	char*	sptr = 0;
	int slen;
	int extra, br;
	char	okRIFF, okWAVE, okFMT;
	_Packed struct {
		char	id[4];
		long	len;
	} chunk;
	_Packed struct {
		short	fmttag;
		short	channels;
		long	rate;
		long	bytespersec;
		short	blockalign;
		short	bits;	
	} wf;

	if (!sample[n].fd) {
		sptr = (char*)*sample[n].memID;
		slen = sample[n].memID.Size();
	} else {
		LSeek(sample[n].fd,sample[n].fileOfs,SEEK_SET);
	}
	okRIFF = okWAVE = okFMT = 0;
	while(1) {
		if (sptr) {
			if (slen < sizeof(chunk))
				break;
			memcpy((char*)&chunk,sptr,sizeof(chunk));
			sptr += sizeof(chunk); 
			slen -= sizeof(chunk);
		} else {
			if (Read(sample[n].fd,(char*)&chunk,sizeof(chunk)) != sizeof(chunk))
				break;
		}

		if (!strncmp(chunk.id,"RIFF",4)) {
			okRIFF = 1;
			continue;
		}

		if (!strncmp(chunk.id,"WAVE",4)) {
			// WAVE chunk does not have a length dword:
			if (sptr) {
				sptr -= sizeof(chunk.len);
				slen += sizeof(chunk.len);
			} else {
				LSeek(sample[n].fd,-sizeof(chunk.len),SEEK_CUR);
			}
			okWAVE = 1;
			continue;
		}

		if (!strncmp(chunk.id,"fmt ",4)) {
			if (sptr) {
				if (slen < sizeof(wf))
					break;
				memcpy((char *)&wf,sptr,sizeof(wf));
				sptr += sizeof(wf);
				slen -= sizeof(wf);
			} else {
				if (Read(sample[n].fd,(char *)&wf,sizeof(wf)) != sizeof(wf))
					break;
			}
			br = sizeof(wf);
			if (wf.fmttag == 2) {   // compressed wave file
				if (sptr) {
					if (slen < sizeof(int))
						break;
					memcpy(&extra,sptr,sizeof(int));
					sptr += sizeof(int) + extra;
					slen -= sizeof(int) + extra;
				} else {
					if (Read(sample[n].fd,&extra,sizeof(int)) != sizeof(int))
						break;
					LSeek(sample[n].fd,extra,SEEK_CUR);
				}
				br += sizeof(int) + extra;
				sample[n].compressed = True;
			} else
				sample[n].compressed = False;
			sample[n].rate = (int)wf.rate;
			sample[n].bits = wf.bits;
			if (sample[n].compressed  || (sample[n].bits != 8 && sample[n].bits != 16))
				msgMgr->Fatal(SrcLoc,"Can't play compressed WAVE audio");
			sample[n].channels = wf.channels;
			sample[n].scrambled = False;
			if (sptr) {
				sptr += chunk.len - br + (chunk.len & 1);
				slen -= chunk.len - br + (chunk.len & 1);
			} else {
				LSeek(sample[n].fd,chunk.len - br + (chunk.len & 1),SEEK_CUR);
			}
			okFMT = 1;
			continue;
		}

		if (!strncmp(chunk.id,"data",4)) {
			if (!okRIFF || !okWAVE || !okFMT)
				break;
			if (!sample[n].fd) {
				sample[n].start = sptr - (char*)*sample[n].memID;
			} else {
				sample[n].start = LSeek(sample[n].fd,0,SEEK_CUR);
			}
			sample[n].length = chunk.len & 0xFFFFFFFC;
			return True;
		}

		// Unknown chunk type -- skip over it:
		if (!okRIFF)
			break;
		if (sptr) {
			sptr += chunk.len + (chunk.len & 1);
			slen -= chunk.len + (chunk.len & 1);
		} else {
			LSeek(sample[n].fd,chunk.len + (chunk.len & 1),SEEK_CUR);
		}
	}
	return False;
}

void
CheckRawFormat(int n)
{
	if (!sample[n].fd) {
		sample[n].length = sample[n].memID.Size();
	} else {
		LSeek(sample[n].fd,sample[n].fileOfs,SEEK_SET);
		sample[n].length = FileLength(sample[n].fd);
	}
	sample[n].start = 0;
	sample[n].rate = rate;
	sample[n].bits = bits;
	sample[n].channels = channels;
	sample[n].scrambled = False;
	sample[n].compressed = False;
	return;
}

#ifdef DEBUG
void
Play()
{
	SOL_Event event;
	
	char* str = configMgr->Str(configMgr->PlayAudio);
	if (strlen(str) == 12 &&
		(str[0] == 'A' || str[0] == 'a' || str[0] == 'B' || str[0] == 'b')) {
		sample[0].num = 0;
		sample[0].module = GetBase36(&str[1],3);
		if (str[0] == 'B' || str[0] == 'b')
			sample[0].module += 36*36*36;
		sample[0].noun = GetBase36(&str[4],2);
		sample[0].verb = GetBase36(&str[6],2);
		sample[0].cond = GetBase36(&str[9],2);
		sample[0].sequ = GetBase36(&str[11],1);
	} else if (strlen(str)) {
		sample[0].num = atoi(str);
		sample[0].module = SFXMODNUM;
	}
	sample[0].distortion = 0;
	sample[0].volume = MAXVOLUME;
	sample[0].loop = True;
	if (!AudioSelect()) {
		Printf("%s not found.  Press any key...\n",str);
		while (!kbdMgr->Get(&event))
			;
		exit(1);
	}
	Printf("(%s)Press any key to stop audio playback...\n",str);
	activeSamples++;
	AudioPlay();
	while (activeSamples) {
		if (kbdMgr->Get(&event)) {
			EndAudio();
			exit(1);
		}
	}
	exit(1);
}

int
GetBase36(char* str36,int digits)
{
	uint	num36, n;

	num36 = 0;
	for (n = 0; n < digits; n++) {
		num36 *= 36;
		if (str36[n] < '0' || str36[n] > '9' && str36[n] < 'A' ||
				str36[n] > 'Z' && str36[n] < 'a' || str36[n] > 'z') {
			return 0;
		}
		if (str36[n] <= '9')
			num36 += str36[n] - '0';
		else if (str36[n] <= 'Z')
			num36 += str36[n] - 'A' + 10;
		else
			num36 += str36[n] - 'a' + 10;
	}
	return num36;
}
#endif

void
EndAudio()
{
	if (!audioInstalled)
		return;
	AudioStop(ALL_SAMPLES);
	DACTerminate();
	if (sfxVolFD != -1)
		Close(sfxVolFD);
	if (audVolFD != -1)
		Close(audVolFD);
	audioInstalled = False;
}

Bool
InitAudioDriver()
{
	if (DACInit() != 0) {
		msgMgr->Alert(SrcLoc, Msg_NoAudio);
		return False;
	}

	char* cp = configMgr->Get("DACCritical", 0);
	if (strlen(cp) && (*cp == 'T' || *cp == 't'))
		critical = DACCritical = True;

	submitSize = configMgr->GetNum("dacSize", 0, 4096);
	if (!submitSize)
		return False;
	ioSize = configMgr->GetNum("audioSize", 0, 65536);
	if (!ioSize)
		return False;
	leadSubmits = configMgr->GetNum("leadSubmits", 0, 2);
	minSubmits = configMgr->GetNum("minSubmits", 0, 4);
	if (!minSubmits)
		return False;
	maxSubmits = configMgr->GetNum("maxSubmits", 0, 24);
	if (!maxSubmits)
		return False;
	if (maxSubmits < minSubmits)
		maxSubmits = minSubmits;
	numSubmits = ioSize * 2 / submitSize; 
	if (maxSubmits > numSubmits - leadSubmits)
		maxSubmits = numSubmits - leadSubmits;
	
	submitID.Get(MemAudioBuffer,submitSize*maxSubmits,AudioSubmitBufHandle,LOCKED);
	convID.Get(MemAudioBuffer, submitSize, AudioConvBufHandle);

	volumeMax = DACGetVolume();
	channelsMax = DACGetMaxChannels();
	if (channelsMax == 0)
		channelsMax = 1;
	AudioChannels(1);
	channelsSet = True;
	bitsMax = DACGetMaxBits();
	AudioBits(16);
	bitsSet = True;
	rateMax = DACGetMaxRate();
	AudioRate(22050);
	rateSet = True;

	audioInstalled = True;
#ifdef DEBUG
	if (configMgr->Get(configMgr->PlayAudio))
		Play();
#endif
	return True;
}

void
InitAudioVols()
{
	char		pathName[MaxPath + 1];
	char*		cp;

	/* Open optional Audio sound effects Volume */
	if (sfxVolFD != -1) {
		Close(sfxVolFD);
		resMgr->Release(MemResMap,SFXMODNUM);
	}
	strcpy(pathName, configMgr->Get("ressfx",0));
	if (strlen(pathName)) {
		cp = &pathName[strlen(pathName)-1];
		if (*cp != ':' && *cp != '\\')
			strcat(pathName,"\\");
	}
	strcat(pathName,SFXVOLNAME);
	sfxVolFD = Open(pathName, O_RDONLY);

	/* Open optional Base-36 Speech/Sync/Rave Volume */
	if (audVolFD != -1) {
		Close(audVolFD);
		ResNum num;
		while ((num = resMgr->FindType(MemResMap)) != (ResNum)-1)
			resMgr->Release(MemResMap,num);
	}
	strcpy(pathName, configMgr->Get("resaud",0));
	if (strlen(pathName)) {
		cp = &pathName[strlen(pathName)-1];
		if (*cp != ':' && *cp != '\\')
			strcat(pathName,"\\");
	}
	strcat(pathName,AUDVOLNAME);
	audVolFD = Open(pathName, O_RDONLY);
}

void
FillRead(int n)
{
	static Bool	recurseFillRead = False;
	assert(!recurseFillRead);
	static Bool	looping = False;

	if (!sample[n].fd ||	!sample[n].bytesToRead ||
			sample[n].bytesBuffered == sample[n].memID.Size())
		return;
	if (!looping) {
		if (sample[n].bytesBuffered > ioSize)
			return;
		if (sample[n].memOfs) {
			memmove((char *)*sample[n].memID,
				(char *)*sample[n].memID+sample[n].memOfs,sample[n].bytesBuffered);
			sample[n].memOfs = 0;
		}
	}

	recurseFillRead = True;
		
	int len = sample[n].memID.Size() - sample[n].bytesBuffered;
	if (len > sample[n].bytesToRead)
		len = sample[n].bytesToRead;
	LSeek(sample[n].fd,sample[n].fileOfs,SEEK_SET);
	Bool dontPollFlag = dontPollDAC;
	dontPollDAC = True;
	sample[n].memID.Read(sample[n].fd,sample[n].bytesBuffered,len);
	dontPollDAC = dontPollFlag;
	sample[n].fileOfs += len;
	sample[n].bytesBuffered += len;
	sample[n].bytesToRead -= len;

	if (!sample[n].bytesToRead && sample[n].loop) {
		sample[n].compVal8 = 0x80;
		sample[n].compVal16 = 0;
		sample[n].fileOfs = sample[n].start;
		sample[n].bytesToRead = sample[n].length;
		recurseFillRead = False;
		looping = True;
		FillRead(n);
		looping = False;
		return;
	}
	recurseFillRead = False;
}

void
FillSubmit(int n,int cnt)
{
	static Bool	recurseFillSubmit = False;
	assert(!recurseFillSubmit);
	recurseFillSubmit = True;

	while (sample[n].filled < cnt)
		if (sample[n].bytesBuffered || RefillBuffer(n))
			AddSubmit(n);
		else
			break;
	recurseFillSubmit = False;
}

void
AddSubmit(int n)
{
	static int totalLen = 0;

	int submitOfs = (((sample[n].nowPlaying + sample[n].filled) *
							submitSize) % sample[n].submitID.Size()) + totalLen;
	int len = (submitSize - totalLen) * sample[n].mul / sample[n].div;
	if (len > sample[n].bytesBuffered)
		len = sample[n].bytesBuffered;

	if (sample[n].do16Conv8 && sample[n].doDecomp)
		memcpy((char *)*convID+len,
			(char *)*sample[n].memID + sample[n].memOfs, len);
	else if (sample[n].do16Conv8 || sample[n].doDecomp ||
			sample[n].do22KConv11K)
		memcpy((char *)*convID,
			(char *)*sample[n].memID + sample[n].memOfs, len);
	else
		memcpy((char *)*sample[n].submitID + submitOfs,
			(char *)*sample[n].memID + sample[n].memOfs, len);

	sample[n].memOfs += len;
	sample[n].bytesBuffered -= len;

	if (sample[n].doDecomp) {
		if (sample[n].bits == 16)
			if (sample[n].do16Conv8)
				sample[n].compVal16 = Decomp8To16((char *)*convID,
					(char *)*convID + len, len, sample[n].compVal16);
			else
				sample[n].compVal16 = Decomp8To16((char *)*sample[n].submitID + submitOfs,
					(char *)*convID, len, sample[n].compVal16);
		else
				sample[n].compVal8  =  Decomp4To8((char *)*sample[n].submitID + submitOfs,
					(char *)*convID, len, sample[n].compVal8);
		len *= 2;
	}

	if (sample[n].do16Conv8) {
		len /= 2;
		Conv16To8((char *)*sample[n].submitID + submitOfs, (char *)*convID, len);
	}

	if (sample[n].do8Conv16) {
		Conv8To16((char *)*sample[n].submitID + submitOfs, len);
		len *= 2;
	}

	if (sample[n].do22KConv11K) {
		len /= 2;
		Conv22KTo11K((char *)*sample[n].submitID + submitOfs, (char *)*convID, len, bits);
	}

	if (sample[n].do11KConv22K) {
		Conv11KTo22K((char *)*sample[n].submitID + submitOfs, len, bits);
		len *= 2;
	}

	totalLen += len;
	if (totalLen < submitSize) {
		if (RefillBuffer(n)) {
			AddSubmit(n);
			totalLen = 0;
			return;
		}
		// pad this partial buffer to the next DAC-buffer boundary
		Silence((char *)*sample[n].submitID + submitOfs + len,
			submitSize - totalLen, bits);
	}
	sample[n].filled++;
	totalLen = 0;
}

Bool
RefillBuffer(int n)
{
	if (sample[n].fd)
		FillRead(n);
	else if (sample[n].loop && !sample[n].bytesBuffered) {
		sample[n].compVal8 = 0x80;
		sample[n].compVal16 = 0;
		sample[n].memOfs = sample[n].start;
		sample[n].bytesBuffered = sample[n].length;
	}
	return sample[n].bytesBuffered;
}

char*
MergeBuffers()
{
	int	n, cnt = 0;
	char* dst;

	for (n = 0; n < activeSamples; n++) {
		if (sample[n].robot && !robotsActive)
			continue;
		if (sample[n].leadIn > sample[n].submitted) {
			if (!cnt) {
				dst = (char *)*submitID + 
						(((nowPlaying + submitted) * submitSize) % submitID.Size());

				Silence(dst,submitSize,bits);
				cnt++;
			}
			sample[n].submitted++;
			continue;
		}
		if (sample[n].pausedLoc) {
			if (!cnt) {
				dst = (char *)*submitID + 
						(((nowPlaying + submitted) * submitSize) % submitID.Size());
				Silence(dst,submitSize,bits);
				cnt++;
			}
			continue;
		}
		if (prioritySample != -1 && prioritySample != n)
			continue;
		if (sample[n].filled <= sample[n].submitted - sample[n].leadIn && !sample[n].robot) {
			FillSubmit(n,sample[n].filled+minSubmits);
			if (sample[n].filled <= sample[n].submitted - sample[n].leadIn)
				continue;
		}
		char *src = (char *)*sample[n].submitID +
					(((sample[n].nowPlaying + sample[n].submitted - sample[n].leadIn) *
						submitSize) % sample[n].submitID.Size());
		dst = (char *)*submitID + 
					(((nowPlaying + submitted) * submitSize) % submitID.Size());
		MergeBuffer(n,cnt,src,dst);
		sample[n].submitted++;
		cnt++;
	}
	if (!cnt)
		return 0;
	DistortBuffer(ALL_SAMPLES,dst);
	VolumeBuffer(ALL_SAMPLES,dst);
	++submitted;
	return dst;
}

void
MergeBuffer(int n, int cnt, char* src, char* dst)
{
	if (sample[n].robot) {
		int ofs = (sample[n].nowPlaying + sample[n].submitted + 1) * submitSize * (16/bits);
		if (ofs > rbot.hwm[1])
			if (ofs > rbot.hwm[0])
				Silence(src,submitSize,bits);
			else
				DupeChannel(src,submitSize/(2*(bits/8)),1,bits);
		else if (ofs > rbot.hwm[0])
			DupeChannel(src,submitSize/(2*(bits/8)),0,bits);
	}
	if (sample[n].distortion || sample[n].volume != MAXVOLUME || sample[n].fadeSteps) {
		memcpy((char *)*convID,src,submitSize);
		src = (char *)*convID;
		if (sample[n].distortion)
			DistortBuffer(n,src);
		if (sample[n].volume != MAXVOLUME || sample[n].fadeSteps)
			VolumeBuffer(n,src);
	}

	if (prioritySample == -1)
		if (!cnt)
			memcpy(dst,src,submitSize);
		else
			Merge(dst,src,submitSize,bits,mixCheck);
	else if (n == prioritySample)
		memcpy(dst,src,submitSize);
}

void
VolumeBuffer(int n, char* buffer)
{
	if (n == ALL_SAMPLES) {
		if (volumeMax > 1)
			DACSetVolume(volume * (volumeMax-1) / MAXVOLUME);
		else if (!volume)
			Silence(buffer,submitSize,bits);
		else if (volume != MAXVOLUME)
			Volume(buffer,submitSize,bits,volume/4,staticFilter?0x7f:0x80);
	} else if (sample[n].fadeSteps)
		FadeBuffer(n,buffer);
	else {
		if (!sample[n].volume)
			Silence(buffer,submitSize,bits);
		else if (sample[n].volume != MAXVOLUME)
			Volume(buffer,submitSize,bits,sample[n].volume/4,staticFilter?0x7f:0x80);
	}
}

void
FadeBuffer(int n, char* buffer)
{
	int loc = (sample[n].nowPlaying + sample[n].submitted) * submitSize -
			sample[n].fadeStart;
	int steps = loc / sample[n].fadeBytes;
	int len = sample[n].fadeBytes - (loc % sample[n].fadeBytes);
	if (steps > sample[n].fadeSteps) {
		if (sample[n].fadeDone)
			Silence(buffer,submitSize,bits);
		else
			Volume(buffer,submitSize,bits,sample[n].fadeVol/4,staticFilter?0x7f:0x80);
		return;
	}
	int vol = sample[n].volume - steps * sample[n].fadeIncr;
	for (int ofs = 0; ofs < submitSize; vol -= sample[n].fadeIncr) {
		if (ofs + len > submitSize)
			len = submitSize - ofs;
		if ((sample[n].fadeIncr > 0 && vol < sample[n].fadeVol) ||
				(sample[n].fadeIncr < 0 && vol > sample[n].fadeVol))
			if (sample[n].fadeDone)
				vol = 0;
			else
				vol = sample[n].fadeVol;
		Volume(&buffer[ofs],len,bits,vol/4,staticFilter?0x7f:0x80);
		ofs += len;
		len = submitSize - len;
	}
}

void
DistortBuffer(int n, char* buffer)
{
	int	distortMask = (n == ALL_SAMPLES)?
				~distortion : ~sample[n].distortion;

	if (distortMask != -1)
		Distort(buffer,submitSize,bits,distortMask);
}

void
ResubmitBuffers()
{
	static Bool recurseResubmitBuffers = False;
	assert (!recurseResubmitBuffers);
	if (!submitted) {
		SubmitMinBuffers();
		return;
	}
	recurseResubmitBuffers = True;

	int	n;

	if (pausedLoc) {
		DACStop();
		submitted = 0;
		nowPlaying = 0;
		for (n = 0; n < activeSamples; n++)
			sample[n].submitted = 0;
		recurseResubmitBuffers = False;
		return;
	}

	for (n = 0; n < activeSamples; n++)
		sample[n].submitted = 0;
	int toSubmit = submitted;
	submitted = 0;
	Bool dontPollFlag = dontPollDAC;
	dontPollDAC = True;
	while(toSubmit--)
		if (!MergeBuffers()) {
			submitted += toSubmit + 1;
			break;
		}
	SubmitMinBuffers();
	dontPollDAC = dontPollFlag;
	recurseResubmitBuffers = False;
}

Bool
SubmitBuffer()
{
	static Bool recurseSubmitBuffer = False;
	assert (!recurseSubmitBuffer);
	if (pausedLoc)
		return False;
	recurseSubmitBuffer = True;

	char* dst = MergeBuffers();
	if (!dst) {
		recurseSubmitBuffer = False;
		return False;
	}

	if (DACSubmitBuffer(dst,submitSize)) {
		submitted--;
msgMgr->Fatal("SUBMIT_DAC_BUFFER failed");
//?		UnSubmitBuffer();
		recurseSubmitBuffer = False;
		return False;
	}
	recurseSubmitBuffer = False;
	return True;
}

void
SubmitMinBuffers()
{
	static Bool recurseSubmitMin = False;
	assert (!recurseSubmitMin);
	recurseSubmitMin = True;

	for (int n = 0; n < activeSamples; n++)
		if (!sample[n].robot)
			FillSubmit(n,minSubmits);
	while (submitted < minSubmits)
		if (!SubmitBuffer())
			break;
	recurseSubmitMin = False;
}

void
SubmitMaxBuffers()
{
	int	n;

	for (n = 0; n < activeSamples; n++)
		if (sample[n].robot)
			return;
	static Bool recurseSubmitMax = False;
	assert (!recurseSubmitMax);
	recurseSubmitMax = True;

	for (n = 0; n < activeSamples; n++)
		FillSubmit(n,maxSubmits);
	while (submitted < maxSubmits)
		if (!SubmitBuffer())
			break;
	recurseSubmitMax = False;
}

void
DACCallBack(int cbs,int ticksPlayed)
{
	static Bool recurseCallBack = False;
	assert (!recurseCallBack);
	recurseCallBack = True;

	assert(cbs <= submitted);
	cbTickCount = timeMgr->GetTickCount();

	nowPlaying += cbs;
	submitted -= cbs;

	Bool resubmit = False;
	for (int n = 0; n < activeSamples; n++) {
		if (!sample[n].submitted)
			continue;
		int num1 = Min(cbs,sample[n].submitted);
		int num2 = Min(cbs,sample[n].leadIn);
		sample[n].nowPlaying += num1 - num2;
		sample[n].submitted -= num1;
		sample[n].filled -= num1 - num2;
		if (sample[n].robot) {
			if (!robotsActive)
				continue;
			rbot.min = nowPlaying * submitSize * (16/bits);
			rbot.max = rbot.min + rbot.blen * (16/bits);
			if (rbot.min > rbot.hwm[0])
				rbot.hwm[0] = rbot.min;
			if (rbot.min > rbot.hwm[1])
				rbot.hwm[1] = rbot.min + 2;
			sample[n].filled = (Max(rbot.hwm[0],rbot.hwm[1]) - rbot.min) / submitSize / (16/bits);
		}
		if (sample[n].leadIn) {
			if (sample[n].leadIn > cbs) {
				sample[n].leadIn -= cbs;
				sample[n].startLoc = cbTickCount + 
					(sample[n].leadIn * submitSize * 60) / (bits/8 * rate * channels);
			} else if (sample[n].leadIn < cbs) {
				sample[n].leadIn = 0;
			} else {
				sample[n].leadIn = 0;
				sample[n].startLoc = cbTickCount;
			}
ticksPlayed=ticksPlayed;
		}
		if (!sample[n].leadIn &&
			 !sample[n].submitted &&
			 !sample[n].filled &&
			 !sample[n].bytesBuffered &&
			 !sample[n].bytesToRead) {
			AudioStop(n--);
			resubmit = True;
		} else if (sample[n].fadeSteps &&
			  sample[n].nowPlaying * submitSize >= sample[n].fadeEnd) {
			if (sample[n].fadeDone) {
				AudioStop(n--);
				resubmit = True;
			} else {
				sample[n].fadeSteps = 0;
				sample[n].volume = sample[n].fadeVol;
			}
		}
	}
	if (resubmit)
		ResubmitBuffers();
	else
		SubmitMinBuffers();
	recurseCallBack = False;
}

Bool
AudioPurge(ResNum num)
{
	// If the named sfx is active and memory-resident, stop it
	// and return True; else return False
	// (Note: there may be multiple copies of the same sample active)

	Bool rc = False;
	for (int n = 0; n < activeSamples; n++)
		if (sample[n].module == SFXMODNUM && sample[n].num == num && sample[n].memID) {
			AudioStop(n);
			rc = True;
		}
	return False;
}

Bool
AudioLock(ResNum num, Bool lock)
{
	// If the named sfx is active and memory-resident, set its
	// locked status as requested and return True; else return False
	// (Note: there may be multiple copies of the same sample active)

	Bool rc = False;
	for (int n = 0; n < activeSamples; n++)
		if (sample[n].module == SFXMODNUM && sample[n].num == num && sample[n].memID) {
			if (lock)
				sample[n].attrs &= ~DISCARDABLE;
			else
				sample[n].attrs |= DISCARDABLE;
			rc = True;
		}
	return rc;
}

Bool
AudioQueryDiscardable(ResNum num)
{
	// If the named sfx is active and memory-resident, return the
	// locked status it had prior to when it started playing;
	// else return False

	for (int n = 0; n < activeSamples; n++)
		if (sample[n].module == SFXMODNUM && sample[n].num == num)
			if (sample[n].memID)
				return (Bool) (sample[n].attrs & DISCARDABLE);
			else
				return False;
	return False;
}
