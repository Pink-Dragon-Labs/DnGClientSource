// windac.cpp

#define INCLUDE_MMSYSTEM_H
#include "sciwin.hpp"
UINT __pascal _16waveOutPrepareHeader(HWAVEOUT hWaveOut, DWORD, UINT uSize);
UINT __pascal _16waveOutUnprepareHeader(HWAVEOUT hWaveOut, DWORD, UINT uSize);
UINT __pascal _16waveOutWrite(HWAVEOUT hWaveOut, DWORD, UINT uSize);

#include "sol.hpp"
#include "audio.hpp"
#include "audiow.hpp"
#include "mbox.hpp"

static	void	DisplayError (WORD, char *);
static	void	OpenWaveDev (void);
static	int	OpenQuery (int,int,int);
static	void	CloseWaveDev (void);
static	void	ResetWaveDev (void);
static	void	WriteWave (int);
static	void	PrepareHeader (int);
static	void	UnprepareHeader (int);
static	int	CheckDoneFlags (void);
static	int	BumpBuffNum (int);

#define	NUMBUFFERS		60
WAVEHDR			wHdr			[NUMBUFFERS];
DWORD				wHdr_alias	[NUMBUFFERS];
LPSTR				lpBuff		[NUMBUFFERS];
BOOL				buffInUse	[NUMBUFFERS];
int				buffQueue	[NUMBUFFERS];
int				buffSize		[NUMBUFFERS];

Bool	dontPollDAC = False;

static	WAVEOUTCAPS		woc;
static	PCMWAVEFORMAT	pwf;
static	HWAVEOUT			hWave;
static	UINT				wError;
static	Bool				waveDevOpen = False;
static	int				maxBits, maxRate, maxChannels;
static	int				nextIn, nextOut;
static	int				writeCount = 0;
static	int				focusBuffCount = 0;
static	MMTIME			mmTime;
static	int				totSamples;

int DACInit(void)
{
int	bNum;

	if (!waveOutGetNumDevs())		/* any devices? */
		return -1;

	// Because of problems encountered with a Medivision driver, I have 
	// learned not to trust the return from DevCaps.  Instead, I now do
	// an open QUERY for each parameter.

	if (OpenQuery(8,44100,1))
		maxRate = 44100;
	else if (OpenQuery(8,22050,1))
		maxRate = 22050;
	else
		maxRate = 11025;

	if (OpenQuery(16,maxRate,1))
		maxBits = 16;
	else
		maxBits = 8;

	if (OpenQuery(maxBits,maxRate,2))
		maxChannels = 2;
	else
		maxChannels = 1;

	// set default values for the format
	pwf.wf.wFormatTag = WAVE_FORMAT_PCM;
	pwf.wf.nChannels  = 1;
	pwf.wf.nSamplesPerSec = 11025;
	pwf.wf.nAvgBytesPerSec = 11025;
	pwf.wf.nBlockAlign = 1;
	pwf.wBitsPerSample = (WORD)8;

	// set up buffer headers
	for (bNum = 0; bNum < NUMBUFFERS; bNum++) {
		wHdr[bNum].dwBytesRecorded = (DWORD)0;
		wHdr[bNum].dwUser = (DWORD)0;
		wHdr[bNum].dwFlags = (DWORD)0;
		wHdr[bNum].dwLoops = (DWORD)0;
		wHdr_alias[bNum] = AllocAlias16(&wHdr[bNum]);
		buffInUse[bNum] = FALSE;
	}

	OpenWaveDev();

	return 0;
}

int DACTerminate(void)
{
	CloseWaveDev();

	for (int bNum = 0; bNum < NUMBUFFERS; bNum++)
			FreeAlias16((DWORD)wHdr_alias[bNum]);

	return 0;
}

int DACSubmitBuffer(char *addr, int size)
{
int	bNum;

	// find a free header
	for (bNum = 0; bNum < NUMBUFFERS;bNum++) {
		if (!buffInUse[bNum])
			break;
	}
	if (bNum >= NUMBUFFERS) {
		MBox("audio buffs exhausted","");
		return 1;
	}

	if (!waveDevOpen)
		OpenWaveDev();

	buffInUse[bNum] = TRUE;

	UnprepareHeader(bNum);

	lpBuff[bNum] = (LPSTR)addr;
	wHdr[bNum].lpData = (LPSTR)AllocAlias16(lpBuff[bNum]);
	wHdr[bNum].dwBufferLength = (DWORD)size;

	buffSize[bNum] = size / pwf.wf.nBlockAlign;

	PrepareHeader(bNum);
	WriteWave(bNum);

	return 0;
}

int DACStop(void)
{
	ResetWaveDev();
	focusBuffCount = 0;
	return 0;
}

int DACLoc(void)
{
	return writeCount + focusBuffCount;
}

int DACSetVolume(int)
{
	return 0;
}

int DACGetVolume(void)
{
	return (woc.dwSupport & WAVECAPS_VOLUME);
}

int DACGetMaxChannels(void)
{
	return woc.wChannels;
}

int DACGetMaxBits(void)
{
	return maxBits;
}

int DACGetMaxRate(void)
{
	return maxRate;
}

int DACSetRate(int rate)
{
	if (writeCount) {
		MBox("set rate: device busy","");
		return 1;
	}

	CloseWaveDev();

	pwf.wf.nSamplesPerSec = Min(rate,maxRate);
	if (pwf.wBitsPerSample == 16) {
		pwf.wf.nAvgBytesPerSec = pwf.wf.nSamplesPerSec << 1;
		pwf.wf.nBlockAlign		= 2;
	} else {
		pwf.wf.nAvgBytesPerSec = pwf.wf.nSamplesPerSec;
		pwf.wf.nBlockAlign		= 1;
	}

	OpenWaveDev();

	return 0;
}

int DACSetBits(int bits)
{
	if (writeCount) {
		MBox("set bits: device busy","");
		return 1;
	}

	CloseWaveDev();

	pwf.wBitsPerSample = Min((WORD)bits,maxBits);
	if (pwf.wBitsPerSample == 16) {
		pwf.wf.nAvgBytesPerSec = pwf.wf.nSamplesPerSec << 1;
		pwf.wf.nBlockAlign		= 2;
	} else {
		pwf.wf.nAvgBytesPerSec = pwf.wf.nSamplesPerSec;
		pwf.wf.nBlockAlign		= 1;
	}

	OpenWaveDev();

	return 0;
}

int DACSetChannels(int channels)
{
	if (writeCount) {
		MBox("set channels: device busy","");
		return 1;
	}

	CloseWaveDev();

	pwf.wf.nChannels = Max(1,Min((WORD)channels,maxChannels));

	OpenWaveDev();

	return 0;
}

int
OpenQuery (int bits, int rate, int chan)
{
	pwf.wf.wFormatTag = WAVE_FORMAT_PCM;
	pwf.wf.nChannels  = chan;
	pwf.wf.nSamplesPerSec = rate;
	pwf.wBitsPerSample = (WORD)bits;

	if (pwf.wBitsPerSample == 16) {
		pwf.wf.nAvgBytesPerSec = pwf.wf.nSamplesPerSec << 1;
		pwf.wf.nBlockAlign		= 2;
	} else {
		pwf.wf.nAvgBytesPerSec = pwf.wf.nSamplesPerSec;
		pwf.wf.nBlockAlign		= 1;
	}

	wError = waveOutOpen ((LPHWAVEOUT)NULL,
									0,
									(LPWAVEFORMAT)&pwf.wf,
									(DWORD)NULL,
									(DWORD)0,
									WAVE_FORMAT_QUERY);
	return (!wError);
}


void
OpenWaveDev (void)
{
	if (waveDevOpen)
		return;
	wError = waveOutOpen ((LPHWAVEOUT)&hWave,
									WAVE_MAPPER,
									(LPWAVEFORMAT)&pwf.wf,
									NULL,
									(DWORD)0,
									0);
	if (wError)
		DisplayError(wError, "waveOutOpen");
	waveDevOpen = True;
	writeCount = 0;
	totSamples = 0;
	nextIn = nextOut = 0;
}


void
CloseWaveDev (void)
{
	if (!waveDevOpen)
		return;

	ResetWaveDev();

	wError = waveOutClose(hWave);
	if (wError)
		DisplayError(wError, "waveOutClose");
	waveDevOpen = False;
	return;
}

void
ResetWaveDev(void)
{
	// If buffers are outstanding, then do a Reset
	// and wait for buffers to clear.

	if (USEAUDIO) {
		if (writeCount) {
			wError = waveOutReset (hWave);
			totSamples = 0;
			if (wError)
				DisplayError(wError, "waveOutReset");
			while (writeCount)
				CheckDoneFlags();
		}
	}
}

void
PrepareHeader(int bNum)
{
	UnprepareHeader(bNum);
	wError = _16waveOutPrepareHeader (hWave, wHdr_alias[bNum], sizeof(WAVEHDR));
	if (wError)
		DisplayError(wError, "waveOutPrepareHeader");
}

void
UnprepareHeader(int bNum)
{
	if (wHdr[bNum].dwFlags & WHDR_PREPARED) {
		wError = _16waveOutUnprepareHeader (hWave, wHdr_alias[bNum], sizeof(WAVEHDR));
		if (wError)
			DisplayError(wError, "waveOutUnprepareHeader");	
	}
}

void
WriteWave (int bNum)
{
	if (USEAUDIO) {
		wError = _16waveOutWrite (hWave, wHdr_alias[bNum], sizeof(WAVEHDR));
		if (wError)
			DisplayError(wError, "waveOutWrite");
		buffInUse[bNum] = TRUE;
	}
	buffQueue[nextOut] = bNum;
	nextOut = BumpBuffNum(nextOut);
	writeCount++;
}

void
DisplayError (WORD wError, char *title)
{
char	szErrorText[150];

if (!waveOutGetErrorText(wError, (LPSTR)szErrorText, sizeof(szErrorText)))
	MessageBox (NULL, szErrorText, title, MB_OK);
}

void
PollWaveBuffs()
{
int	doneCount;
int	samplesAhead;
int	ticksAhead;

	if (dontPollDAC)
		return;
	if (!writeCount)
		return;
	dontPollDAC = True;

	while ((doneCount = CheckDoneFlags()) != 0) {
		mmTime.wType = TIME_SAMPLES;
		waveOutGetPosition(hWave,(LPMMTIME)&mmTime,sizeof(MMTIME));
		totSamples = Min(mmTime.u.sample,totSamples);
		samplesAhead = mmTime.u.sample - totSamples;
		ticksAhead = (samplesAhead * 60) / pwf.wf.nSamplesPerSec;
//	sprintf(mbuff,"t:%d ",ticksAhead);
//	sprintf(mbuff,"t:%d %d ",samplesAhead,writeCount);
//	OutputDebugString(mbuff);
		DACCallBack(doneCount,ticksAhead);
	}

	dontPollDAC = False;

}

int
CheckDoneFlags()
{
int	bNum, doneCount;

	if (!writeCount)
		return 0;

	doneCount = 0;
	while (TRUE) {
		if (nextIn == nextOut)
			break;
		bNum = buffQueue[nextIn];
		if (!(wHdr[bNum].dwFlags & WHDR_DONE))
			break;
		buffInUse[bNum] = FALSE;
		UnprepareHeader(bNum);
		FreeAlias16((DWORD)wHdr[bNum].lpData);
		nextIn = BumpBuffNum(nextIn);
		writeCount--;
		doneCount++;
		totSamples += buffSize[bNum];
	}

	return doneCount;
}

int
BumpBuffNum(int buffNum)
{
	buffNum++;
	if (buffNum < NUMBUFFERS)
		return buffNum;
	else
		return 0;
}

void
PauseWaveDev(void)
{
	if (audioInstalled) {
		focusBuffCount = writeCount;
		AudioPause(ALL_SAMPLES);
		CloseWaveDev();
	}
}

void
ResumeWaveDev(void)
{
	if (!haveFocus)
		return;

	if (audioInstalled) {
  		OpenWaveDev();
		AudioResume(ALL_SAMPLES);
		if (focusBuffCount) {
			DACCallBack(focusBuffCount,0);
			focusBuffCount = 0;
		}
	}
}
