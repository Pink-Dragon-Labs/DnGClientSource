#include "sol.hpp"

#ifdef ROBOT

#include "robot.hpp"

#include "array.hpp"
#include "audio.hpp"
#include "config.hpp"
#include "criterr.hpp"
#include "dcomp.hpp"
#include "dos.hpp"
#include "graph.hpp"
#include "graphmgr.hpp"
#include "kernel.hpp"
#include "pmachine.hpp"
#include "resource.hpp"
#include "shiftpal.hpp"
#include "time.hpp"
#include "event.hpp"



// For now - move to MSG.CPP when available
#ifdef DEBUG	//  ----------------------------------

void AssertMsgFail(const char* filename, int line, const char* str);


class LogObj
{
	public:
	LogObj(const char* str);
	~LogObj();

	protected:
	static int tab;
};


static int LogObj::tab = 0;

//#define LOG(exp)	LogObj localLogObj(exp)
#define LOG(exp)
#define DebugPrintf	msgMgr->Mono
#define	assertMsg(exp, str) \
	if (exp) {}				\
	else						\
	AssertMsgFail(__FILE__, __LINE__, str)

#else		//   DEBUG  --------------------------------

inline void DebugPrintf(char*, ...) {}
#define	assertMsg(exp, str)
#define LOG(exp)

#endif


class ReadFile
{
	public:
	ReadFile(int fd);
	~ReadFile()				{}

	Bool		Read(void*, int count);
 	Bool		Close();
	Bool		LSeek(int pos, int type);
	Bool		Tell(long* pos);
	void		ClearErrorAccum()				{errorAccum = False;}
	Bool		DidErrorAccum() const		{return errorAccum;}

	protected:
	int		fd;
	Bool		fileOpenFlag;
	Bool		errorAccum;

};

//#define DANTEST1

const PALSIZE = 1200;

_Packed struct VideoHeader
{
	short	celCount;
	uchar	compressionType;
	uchar	vertScaleFactor;
	short	xsize, ysize;
	short	xoff, yoff;
	short	x1, y1;
	short	size;
};

_Packed struct VideoHeader4
{
	uchar	compressionType;
	uchar	vertScaleFactor;
	short	xsize, ysize;
	short	xoff, yoff;
	short	x1, y1;
	short	size;
	short	checksum;
	long	unused;
};

enum CompType {LZ_COMP, RLE_COMP, UNCOMP};


//  BUGBUG - DF - 5/31/95 - Can't use LSEEK(n,n,SEEK_SET) if robot
//  opened through resource.



/**************************************************************
   Robot constructor


	Prepares to play a single robot.
	Accesses config manager for "adjustPerDAC" flag, which allows
	the user to shut off attempts to correct video/audio timing
	to account for small % differences in audio playback rate.


 **************************************************************/

Robot::Robot(int resNum, long planeId, int pri, int xoff, int yoff) :
	resNum(resNum),
	planeId(planeId), 
	xoff(xoff),
	yoff(yoff)
{
	for (int i = 0; i < SiArraySize; i++)
	{
		robotSi[i] = NULL;
		priority[i] = pri;
	}
	siCount = 0;
	framesSkipped = 0;
	maxFramesSeqSkipped = 0;
	fd = 0;
	cueForceShowFrame = -1;
	robotStartFrame = 0;
	robotStartTime = -1;

	hunkPalette = NULL;
	videoSizeArray = NULL;
	recordPosArray = NULL;
	celHandleArray = NULL;
	celHandleArraySize = 0;
	syncFrame = True;
	lastFrameNum = -1;
	startingFrameNum = -1;
	status = ROBOT_UNUSED;
	lzScratch = New char[SCRATCHRAM_SIZE];
	cueTimeList = New long[CueArraySize];
	masterCueTimeList = New long[CueArraySize];
	cueValueList = New ushort[CueArraySize];
	for (i = 0; i < CueArraySize; i++)
	{
		cueTimeList[i] = -1;
		masterCueTimeList[i] = -1;
		cueValueList[i] = 0;
	}

	adjustPerDAC = True;
	char* cp = configMgr->Get("adjustPerDAC", 0);
	if (strlen(cp) && (*cp == 'F' || *cp == 'f'))
		adjustPerDAC = False;

	hasAudio = False;
	prevFrame = -1;
	firstAudioRecordStartPos = 0;
	audioRecordInterval = 0;

	#ifdef DEBUG
	videoSizeArrayCopy = NULL;
	recordPosArrayCopy = NULL;
	maxFrameSize = 0;
	#endif
	//  BUGBUG - DDF - 5/25/95  -  What if theres no DAC or no audio in robot?
}



/**************************************************************
   Robot destructor

   NOTE:   If robot is not deleted when graphMgr destroyed, 
   this destructor gets called during graphmgr destructor, which
   will reenter a graphmgr method, causing trouble

 **************************************************************/


Robot::~Robot()
{
	assert(Assert());
	delete[] videoSizeArray;
	delete[]	recordPosArray;
	delete[] lzScratch;
	delete[] cueTimeList;
	delete[] masterCueTimeList;
	delete[] cueValueList;
	delete[] hunkPalette;

   //  Deallocate all FrameLifetime handles in cel handle array
	if (celHandleArray)
	{
		for (int i = 0; i < celHandleArraySize; i++)
		{
			if (celHandleArray[i].status == CelHandleInfo::FrameLifetime)
			{
				assert(celHandleArray[i].id.IsValid());
				celHandleArray[i].id.Free();
			}
		}
	}
   //  Deallocate cel handle array
	delete[] celHandleArray;
	celHandleArray = NULL;

   //  Deallocate intermediate buffer
	assert(!squashedCelHandle  ||  squashedCelHandle.IsValid());
	squashedCelHandle.Free();

   //  Deallocate all fixed Cel handles
	for (int i = 0; i < FixedCelArraySize; i++)
		fixedCelHandle[i].Free();

	#ifdef DEBUG
	delete[] videoSizeArrayCopy;
	delete[] recordPosArrayCopy;
	#endif

	status = ROBOT_UNUSED;					//  in case calls made to deleted robot
	if (fd)
		resMgr->Close(fd);

	for (i = 0; i < siCount; i++)
		graphMgr->DeleteScreenItem(robotSi[i]);

	if (hasAudio)
		audioList.StopAudioNow();
}

long
Robot::TickCount() const
{
	return timeMgr->GetTickCount();
}



/**************************************************************
   Init Method


	Opens robot file, loads up primer, inits all main variables
	from file header.

	Sets robot audio in paused state so submissions will not play
	until 1st frame visible

	Robot resolution comes 1st from the robot file. If undefined,
	it will look for "hires=xxx,yyy" in the config file; otherwise,
	it	defaults	to 640x480.


 **************************************************************/

void
Robot::Init()
{
	char buffer[4];
	char junk[32];
	short fcount, asize, vsize1, vsize2;
	ushort palSize;
	char palFlag, audFlag;

	_Packed	struct AudPrimerHdr
	{
		long	totalSampleCount;
		short	compressionType;
		long	evenSize;
		long	oddSize;
	};

	LOG("Init\n");
	assertMsg(fd == 0, "Robot::Init called when already open!");
	assertMsg(status != ROBOT_KILLED, "Robot::Init called after robot killed!");

	if (!Open(&fd, resNum))
		msgMgr->Fatal("Could not open Robot resource %d\n", (ushort)resNum);

	syncFrame = True;
	lastFrameNum = -1;
	cueForceShowFrame = -1;
	frameNum = 0;
	status = ROBOT_PAUSED;

	ushort minrate = 300;

	ReadFile file(fd);

	file.Read(buffer, 4);
	file.Read(&version, 2);
	file.Read(&asize, 2);
	file.Read(&primerZeroCompressFlag, 2);
	file.Read(&minrate, 2);
	file.Read(&fcount, 2);
	file.Read(&palSize, 2);
	file.Read(&primerReservedSize, 2);
	file.Read(&xRes, 2);
	file.Read(&yRes, 2);
	file.Read(&palFlag, 1);
	file.Read(&audFlag, 1);
	file.Read(junk, 2);			//  cueCount, frameRate
	file.Read(&frameRate, 2);
	file.Read(&hiresCoords, 2);
	file.Read(&canSkip, 2);
	file.Read(&maxCelsPerFrame, 2);
	file.Read(&maxCelArea[0], 4);
	file.Read(&maxCelArea[1], 4);
	file.Read(&maxCelArea[2], 4);
	file.Read(&maxCelArea[3], 4);
	file.Read(junk, 8);

	assert(version == 5);
	assert(minrate >= 150  &&  minrate <= 300);
	assert(frameRate > 0  && frameRate <= 30);

	if (file.DidErrorAccum())
		msgMgr->Fatal("Read error on robot: %d\n", (ushort)resNum);

	audioRecordInterval = 22050 / frameRate;
	normalFrameRate = frameRate;
	lowFrameRate = normalFrameRate - 1;
	highFrameRate = normalFrameRate + 1;

	//  If resolution undefined, use hires values, otherwise default
	if (xRes == 0  ||  yRes == 0)
	{
		if (*configMgr->Get("hires", 0))
		{
			xRes = atoi(configMgr->Get("hires", 0));
			yRes = atoi(configMgr->Get("hires", 1));
		}
		else
		{
			xRes = 640;
			yRes = 480;
		}
	}

	audioSize = asize;
	expectedAudCount = audioSize - 8;
	hasAudio = audFlag;
	vPadSize[0] = vsize1;
	vPadSize[1] = vsize2;
	frameCount = fcount;

	if (strcmp(buffer, "SOL"))	
		msgMgr->Fatal("Resource is not Robot type!");

	if (version != 5)
		msgMgr->Fatal("Unsupported version of Robot resource");

	if (!hasAudio)
		file.LSeek(primerReservedSize, SEEK_CUR);
	else
	{
		if (primerReservedSize != 0)
		{
			AudPrimerHdr audPrimerHdr;
			long pos;
			file.Tell(&pos);
			file.Read(&audPrimerHdr, 14);
			file.Tell(&primerPos);
			evenPrimerSize = audPrimerHdr.evenSize;
			oddPrimerSize = audPrimerHdr.oddSize;
			totalPrimerSize = audPrimerHdr.totalSampleCount;

			if (audPrimerHdr.compressionType)
				msgMgr->Fatal("Unknown audio header compression type");

			if (evenPrimerSize + oddPrimerSize != primerReservedSize)
				file.LSeek(pos + primerReservedSize, SEEK_SET);
		}
		else if (primerZeroCompressFlag)
		{
			evenPrimerSize = 19922;
			oddPrimerSize = 21024;
		}

		firstAudioRecordStartPos = evenPrimerSize * 2;		//  Depends on AVIROBOT doing this
	}

	//  Read in palette if flag indicates one in file
	if (palFlag)
	{
		hunkPalette = (void*) New uchar[PALSIZE];
		file.Read(hunkPalette, palSize);
	}
	else
		file.LSeek(palSize, SEEK_CUR);

	delete[] videoSizeArray;
	videoSizeArray = New ushort[frameCount];

	delete[] recordPosArray;
	recordPosArray = New long[frameCount];

	file.Read(videoSizeArray, sizeof(*videoSizeArray) * frameCount);
	ushort* tempSize = New ushort[frameCount];
	file.Read(tempSize, sizeof(ushort) * frameCount);

	file.Read(cueTimeList, sizeof(*cueTimeList) * CueArraySize);
	file.Read(cueValueList, sizeof(*cueValueList) * CueArraySize);
	memcpy(masterCueTimeList, cueTimeList, sizeof(*cueTimeList) * CueArraySize);

	//  Make sure they didn't use last frame or greater to cue with
	#ifdef DEBUG
	for (int ii = 0; ii < CueArraySize; ii++)
	{
		if (cueTimeList[ii] != -1  &&  cueTimeList[ii] >= frameCount - 1)
			msgMgr->Fatal("Cue %d exceeds legal range!\n", cueTimeList[ii]);
	}
	#endif


	long pos;
	file.Tell(&pos);
	int remain = pos % CDSectorSize;
	if (remain)
		file.LSeek(CDSectorSize - remain, SEEK_CUR);

	file.Tell(&fileDataStart);

	#ifdef DEBUG
	maxFrameSize = (minrate * 1024L) / frameRate;
	for (ii = 0; ii < frameCount; ii++)
	{
		assert(tempSize[ii] <= maxFrameSize);
	}
	#endif

	long total = fileDataStart;
	recordPosArray[0] = total;
	for (int i = 0; i < frameCount - 1; i++)
	{
		total += tempSize[i];
		recordPosArray[i + 1] = total;
	}

	#ifdef DEBUG
	delete[] videoSizeArrayCopy;
	videoSizeArrayCopy = New ushort[frameCount];

	delete[] recordPosArrayCopy;
	recordPosArrayCopy = New long[frameCount];

	for (ii = 0; ii < frameCount; ii++)
	{
		recordPosArrayCopy[ii] = recordPosArray[ii];
		videoSizeArrayCopy[ii] = videoSizeArray[ii];
	}
	#endif

	if (hasAudio)
	{
		long usedEachFrame = 11025 / normalFrameRate;
		canSkip = (audioSize / usedEachFrame) - 1;
		if (canSkip < 0)
			canSkip = 0;
	}

	delete[] tempSize;
	framesSkipped = 0;
	maxFramesSeqSkipped = 0;
	prevFrame = -1;

	if (file.DidErrorAccum())
		msgMgr->Fatal("Read error on robot: %d\n", (ushort)resNum);


	//  Allocate Fixed Cel buffers if version 5 or up
	//DebugPrintf("Max cels per frame: %d\n", (int) maxCelsPerFrame);
	int count = maxCelsPerFrame;
	if (count > FixedCelArraySize)
		count = FixedCelArraySize;

	for (i = 0; i < count; i++)
		fixedCelHandle[i].Get(MemBitmap, maxCelArea[i] + CELHEADERSIZE + PALSIZE, LOCKED);

	//  Allocate intermediate buffer
	if (version > 5)				//  BUGBUG - ?? check this
		squashedCelHandle.Get(MemBitmap, maxCelArea[0] + CELHEADERSIZE + PALSIZE, LOCKED);
	else
		squashedCelHandle.Get(MemBitmap, maxCelArea[0] + CELHEADERSIZE + PALSIZE);

	squashedCelArea = maxCelArea[0];
}



/**************************************************************
   InitAudio

   The primer must be non-zero and be at least 1 audio interval long
	to start on a non-zero frame
   


 **************************************************************/

Bool
Robot::InitAudio(int startTick)
{
	LOG("InitAudio\n");
	assert(startTick >= 0);
	assert(fd != 0);
	assert(hasAudio);

	ReadFile file(fd);
	Bool worked = True;
	audioList.Reset();

	//
	//  If we're starting at the beginning, read in primer normally
	//

	if (startTick == 0)
	{
		audioList.PrepareForPrimer();
		uchar* evenPrimerBuff = New uchar[evenPrimerSize];
		uchar* oddPrimerBuff = New uchar[oddPrimerSize];
		worked = ReadPrimerData(evenPrimerBuff, oddPrimerBuff);
		if (worked)
		{
		   //  Parse primer, submit data
			if (evenPrimerSize != 0)
				audioList.AddBlock(0, evenPrimerSize, evenPrimerBuff);

			if (oddPrimerSize != 0)
				audioList.AddBlock(1, oddPrimerSize, oddPrimerBuff);
		}

		//  Free up memory
		delete[] evenPrimerBuff;
		delete[] oddPrimerBuff;
	}
	else
	{
		//  
		//  We're not starting on the first tick here
		//
		//  The primer must be non-zero and be at least 1 audio interval long
		//  to start on a non-zero frame

		assert(evenPrimerSize * 2 >= audioRecordInterval  ||  oddPrimerSize * 2 >= audioRecordInterval);

		//  Calc starting frame, and 22Kx8 starting position (even)
		int audioStartFrame = 0;
		int videoStartFrame = (startTick * (int) frameRate) / 60;
		assert(videoStartFrame < frameCount);

		//  Calculate start position, make it even for now
		int audioStartPos = (startTick * 22050) / 60;
		if (audioStartPos & 1)
			audioStartPos--;
		audioList.SetAudioOffset(audioStartPos);
		audioList.PrepareForPrimer();

		//  See if audio starts in primer
		if (audioStartPos < evenPrimerSize * 2  ||  audioStartPos + 1 < oddPrimerSize * 2)
		{
			//  Allocate memory for primer, read it in
			//DebugPrintf("Data in primer");		
			uchar* evenPrimerBuff = New uchar[evenPrimerSize];
			uchar* oddPrimerBuff = New uchar[oddPrimerSize];
			worked = ReadPrimerData(evenPrimerBuff, oddPrimerBuff);
			if (worked)
			{
			   //  Parse primer, submit data
				int audioStartPosHalf = audioStartPos / 2;
				if (audioStartPos < evenPrimerSize * 2)
				{
					audioList.AddBlock(audioStartPos, evenPrimerSize - audioStartPosHalf, &evenPrimerBuff[audioStartPosHalf]);
					//DebugPrintf("Even: Pos: %d  Size: %d\n", audioStartPos, evenPrimerSize - audioStartPosHalf);
				}

				if (audioStartPos + 1 < oddPrimerSize * 2)
				{
					audioList.AddBlock(audioStartPos + 1, oddPrimerSize - audioStartPosHalf, &oddPrimerBuff[audioStartPosHalf]);
					//DebugPrintf("Odd:  Pos: %d  Size: %d\n", audioStartPos + 1, oddPrimerSize - audioStartPosHalf);
				}
			}

			//  Free up memory
			delete[] evenPrimerBuff;
			delete[] oddPrimerBuff;
		}

		//  If audio is in normal record, look
		if (audioStartPos >= firstAudioRecordStartPos)
		{
		   //  Calculate which record audio starts in (could be even or odd)
			//DebugPrintf("Data in records");		
			int audioRecordSize = expectedAudCount;
			assert(audioRecordSize > 0);
			assert(audioRecordInterval > 0);
			assert(firstAudioRecordStartPos >= 0);

			audioStartFrame = (audioStartPos - firstAudioRecordStartPos) / audioRecordInterval;
			assert(audioStartFrame < videoStartFrame);
			//DebugPrintf("A: audioStartFrame: %d  audioStartPos: %d  firstAudioRecordStartPos: %d  audioRecordInterval: %d\n", 
			//	audioStartFrame, audioStartPos, firstAudioRecordStartPos, audioRecordInterval); 

			//  If there was a previous audio frame, see if our point was in it
			if (audioStartFrame > 0)
			{
				int audioTmpFrame = audioStartFrame - 1;
				int oddRemain = audioTmpFrame & 1;
				int tmpRecordStart = (audioTmpFrame * audioRecordInterval) + oddRemain + firstAudioRecordStartPos;
				int tmpRecordEnd = (tmpRecordStart + ((audioRecordSize - 1) * 2)) + oddRemain + firstAudioRecordStartPos;

				//  If the data is in previous record, back up one
				if (audioStartPos >= tmpRecordStart  &&  audioStartPos <= tmpRecordEnd)
					audioStartFrame--;
			}

			//  If frame is odd, make offset odd
			assert(!(audioStartPos & 1));
			if (audioStartFrame & 1)
			{
				//DebugPrintf("Starting on odd Record!\n");
				audioStartPos++;
			}

			//  Read and submit data
			if (!ReadPartialAudioRecordAndSubmit(audioStartFrame, audioStartPos))
				return False;

			//  Now do the next audio frame (will be compliment even/odd depending on previous record)
			audioStartFrame++;
			assert(audioStartFrame < videoStartFrame); 

			//  If the startPos is within the next record, do partial processing
			int oddRemain = audioStartFrame & 1;
			int tmpRecordStart = (audioStartFrame * audioRecordInterval) + oddRemain + firstAudioRecordStartPos;
			int tmpRecordEnd = (tmpRecordStart + ((audioRecordSize - 1) * 2)) + oddRemain + firstAudioRecordStartPos;

			if (audioStartPos >= tmpRecordStart  &&  audioStartPos <= tmpRecordEnd)
			{
				if (!ReadPartialAudioRecordAndSubmit(audioStartFrame, audioStartPos + 1))
					return False;

				//  Since this one was processed, move to next audio record			
				audioStartFrame++;
			}
		}

		//  Now submit all remaining audio records up until the desired video record
		int audPos, audCount;
		uchar* tmpAudioData = New uchar[expectedAudCount + ZeroCompressCount];

		for (int i = audioStartFrame; i < videoStartFrame; i++)
		{
		   //  read and submit all audio records
			if (!ReadAudioDataFromRecord(i, tmpAudioData, &audPos, &audCount))
				break;

			#ifdef DEBUG
			if (i & 1)
				assert(audPos & 1);
			else
				assert(!(audPos & 1));
			#endif

			audioList.AddBlock(audPos, audCount, tmpAudioData);
		}

		delete[] tmpAudioData;
	}
	return worked;
}



Bool
Robot::ReadPartialAudioRecordAndSubmit(int audioStartFrame, int audioStartPos)
{
	#ifdef DEBUG
	assert(hasAudio);
	assert(audioStartFrame >= 0  &&  audioStartFrame < frameCount);
	assert(audioStartPos >= 0  &&  audioStartPos < ((frameCount * 22050L) / frameRate));
	//DebugPrintf("B: audioStartFrame: %d  audioStartPos: %d  firstAudioRecordStartPos: %d  audioRecordInterval: %d\n", 
	//		audioStartFrame, audioStartPos, firstAudioRecordStartPos, audioRecordInterval); 

	int estStartRecordPos = (audioStartFrame * audioRecordInterval) + firstAudioRecordStartPos;
	//DebugPrintf("audioStartPos: %d  estStartRecordPos: %d\n", audioStartPos, estStartRecordPos);
	assert(audioStartPos >= estStartRecordPos);
	assert(!((audioStartFrame ^ audioStartPos) & 1));		//  both even or both odd
	#endif

	//  read, parse, submit data
	int audPos, audCount;
	uchar* tmpAudioData = New uchar[expectedAudCount + ZeroCompressCount];
	Bool worked = ReadAudioDataFromRecord(audioStartFrame, tmpAudioData, &audPos, &audCount);
	if (worked)
	{
		#ifdef DEBUG
		assert(audPos == estStartRecordPos);
		assert(audioStartPos < audPos + (audCount * 2) - 1);
		if (audioStartFrame & 1)
			assert(audPos & 1);
		else
			assert(!(audPos & 1));
		#endif

		assert(audPos <= audioStartPos);
		assert((audPos + (audCount * 2) - 1) >= audioStartPos);

		int relStartOffset = (audioStartPos - audPos) / 2;
		assert(relStartOffset >= 0);
		assert(audCount > relStartOffset);

		//DebugPrintf("pos: %d  start pos: %d  count: %d  offset: %d\n", audPos, audioStartPos, audCount, relStartOffset); 
		audioList.AddBlock(audioStartPos, audCount - relStartOffset, &tmpAudioData[relStartOffset]);
	}

	delete[] tmpAudioData;
	return worked;
}


/*************************************************************************
  ReadAudioDataFromRecord  

  This method will read AND uncompress audio data from any
  record of the robot file.  Note that this audio data is usually
  for a different time than the video data paired with it in the
  same record.  (Due to the audio primer offsetting the audio).

  In:  Desired record to read
       buffer for audio data (expectedAudCount + ZeroCompressCount size)
		 fill-in variables for audioPos and audCount


  Out: Returns True if sucessful, False if Disk io error OR no audio is
       associated with that video record (near end of file)

		 returns real audio data position (22k x 8),
		 and count of uncompressed audio data bytes.
		 NOTE:
		   Can return an AudPos and AudCount of zero on the last few
			frames of a file (due to the offsetting of audio primer)

 *************************************************************************/

Bool
Robot::ReadAudioDataFromRecord(int index, uchar* buffer, int* pAudPos, int* pAudCount)
{
	assert(index >= 0);
	assert(index < frameCount);
	assert(buffer != NULL);
	assert(pAudPos != NULL);
	assert(pAudCount != NULL);
	assert(fd != 0);
	assert(expectedAudCount > 0);
	assert(hasAudio);

	#ifdef DEBUG
	memset(buffer, 0xFE, expectedAudCount + ZeroCompressCount);
	#endif

	char scratch[10];
	ReadFile file(fd);
	file.LSeek(recordPosArray[index] + videoSizeArray[index], SEEK_SET);
	audioList.SubmitDriverMax();

	file.Read(scratch, 8);
	long audCount = *((long*) &scratch[4]);
	long audPos = *((long*) scratch);
	assert(audCount <= expectedAudCount);

	//  If they tried to read an audio sector with no audio, return False
	if (audPos == 0)
		return False;

	if (audCount != expectedAudCount)
	{
		memset(buffer, 0, ZeroCompressCount);
		file.Read(&buffer[ZeroCompressCount], audCount);
		audCount += ZeroCompressCount;
	}
	else
	{
		file.Read(buffer, audCount);
	}

	*pAudCount = audCount;
	*pAudPos = audPos;
	return !file.DidErrorAccum();
}



/******************************************************************
   ReadPrimerData method

   The ReadPrimerData method is designed to read and uncompress
   the audio primer from the robot file.  

   Input:
	evenBuffer  -  Pointer to buffer to fill (size: evenPrimerSize)
	oddBuffer -    Pointer to buffer to fill (size: oddPrimerSize)
	
	Output:
	Returns True if sucessful, False if disk read error
	current file position is now undefined.

 ******************************************************************/


Bool
Robot::ReadPrimerData(uchar* evenBuffer, uchar* oddBuffer)
{
	assert(evenBuffer != NULL);
	assert(oddBuffer != NULL);
	assert(evenPrimerSize >= 0);
	assert(oddPrimerSize >= 0);
	assert(fd != 0);
	assert(hasAudio);

	#ifdef DEBUG
	memset(evenBuffer, 0xFE, evenPrimerSize);
	memset(oddBuffer, 0xFE, oddPrimerSize);
	#endif

	ReadFile file(fd);
	if (primerReservedSize != 0)
	{
		assert(!primerZeroCompressFlag);

		if (totalPrimerSize != 0)
		{
			assert(evenPrimerSize != 0  ||  oddPrimerSize != 0);
			assert(primerPos > 0);
			file.LSeek(primerPos, SEEK_SET);
			if (evenPrimerSize)
				file.Read(evenBuffer, evenPrimerSize);

			if (oddPrimerSize)
				file.Read(oddBuffer, oddPrimerSize);
		}
		else
		{
			assert(evenPrimerSize == 0  &&  oddPrimerSize == 0);
		}
	}
	else if (primerZeroCompressFlag)
	{
		assert(primerReservedSize == 0);
		memset(evenBuffer, 0, evenPrimerSize);
		memset(oddBuffer, 0, oddPrimerSize);
	}
	else
	{
		assert(!"ReadPrimerData - Flags corrupt");
	}

	return !file.DidErrorAccum();
}



/**************************************************************
   DoRobot method

   Called from beginning of each FrameOut if robot exists.
	Responsible for calculating which frame to show, loading &
	decompressing it, and adding it to the graphMgr.


 **************************************************************/

void
Robot::DoRobot()
{
	LOG("DoRobot\n");
	assertMsg(status != ROBOT_UNUSED, "Robot::DoRobot - Called when robot uninitialized");
	assertMsg(status != ROBOT_KILLED, "Robot::DoRobot - Called after robot killed");
	AsyncEventCheck();
	assert(Assert());

	if (status != ROBOT_PLAYING)
	{
		//DebugPrintf("Robot not playing");
		return;
	}

	if (!syncFrame)
	{
		if (cueForceShowFrame != -1)
		{
			frameNum = cueForceShowFrame;
			cueForceShowFrame = -1;
		}
		else
		{
			int calcFrameNum = CalcFrameNum(delayTime.PredictedTicks());
			if (calcFrameNum < frameNum)
			{
				//DebugPrintf("CalcFrame less!");
				//msgMgr->Mono("Calc: %d  Actual: %d\n", calcFrameNum, frameNum);
				return;
			}
			#ifdef DEBUG
			predictedTime = delayTime.PredictedTicks();
			#endif

			frameNum = calcFrameNum;
			//frameNum++;
		}
	}

	//  If last frame already shown, set final flag and return
	if (frameNum >= frameCount)
	{
		if (lastFrameNum == frameCount - 1)
		{
			status = ROBOT_FINAL;
			if (hasAudio)
			{
				audioList.StopAudio();
				frameRate = normalFrameRate;
				hasAudio = False;
			}
			return;
		}
		else
			frameNum = frameCount - 1;
	}

	//  If desired frame already being animated, return
	if (frameNum == lastFrameNum)
	{
		//DebugPrintf("Same frame already up");
		return;
	}

	if (hasAudio)
	{
		for (long tmp = lastFrameNum + canSkip + 1; tmp < frameNum; tmp += canSkip + 1)
		{
			audioList.SubmitDriverMax();
			uchar* buffer = New uchar[ZeroCompressCount + expectedAudCount];
			int audCount, audPos;
			if (ReadAudioDataFromRecord(tmp, buffer, &audPos, &audCount))
				audioList.AddBlock(audPos, audCount, buffer);

			delete[] buffer;
		}
		audioList.SubmitDriverMax();
	}

	delayTime.StartTiming();
	SeekFrame(frameNum);
	if (version == 5)
		DoVersion5();				//  Returns False if IO failed!
	else
		msgMgr->Fatal("Robot Version %d not supported in this interpreter", version);

	if (hasAudio)
		audioList.SubmitDriverMax();



	//  Set flag, return ok
	if (frameNum != lastFrameNum + 1)
	{
		int skipped = frameNum - lastFrameNum - 1;
		framesSkipped += skipped;
		if (skipped > maxFramesSeqSkipped)
			maxFramesSeqSkipped = skipped;
	}

	lastFrameNum = frameNum;
}


/**************************************************************
   FrameAlmostVisible method

	Called from FrameOut just before ShowBits.


 **************************************************************/

void
Robot::FrameAlmostVisible()
{
	AsyncEventCheck();
	if (status == ROBOT_PLAYING  &&  !syncFrame)
	{
		//  We'll do delay if we are ahead of schedule
		if (prevFrame != frameNum)
		{
			while (CalcFrameNum() < frameNum)
			{
			}
		}
	}
}



/**************************************************************
   FrameNowVisible method

	Called from FrameOut just after ShowBits (even if robot frame
	didn't change).  

   Used to sync initial video frame with beginning of audio.  Also
	attempts to keep variations in audio card rates in sync with video.


 **************************************************************/


void
Robot::FrameNowVisible()
{
	LOG("FrameNowVisible\n");

	if (syncFrame  &&  status == ROBOT_PLAYING)
	{
		syncFrame = False;
		if (hasAudio)
		{
			audioList.StartAudioNow();
			checkAudioSyncTime = robotStartTime + (5 * 60);		//  5 seconds
		}

		SetRobotTime(frameNum);
	}

	if (status == ROBOT_PLAYING)
	{
		#ifdef DEBUG
		static long lastFrameTime = 0;
		#endif

		if (delayTime.TimingInProgress())
		{
			long actualTime = delayTime.EndTiming();
			//DebugPrintf("Actual: %d  Predicted: %d\n", actualTime, predictedTime);
		}

		#ifdef DEBUG
		if (prevFrame != frameNum)
		{
			long newFrameTime = timeMgr->GetTickCount();
			//DebugPrintf("Time 3: %d\n", newFrameTime - lastFrameTime);
			lastFrameTime = newFrameTime;
		}
		#endif

		if (hasAudio)
			audioList.SubmitDriverMax();

		if (prevFrame != frameNum)
			prevFrame = frameNum;

		//  Try to keep audio and video in sync (for audio cards with bad clock rates)
		if (adjustPerDAC  &&  !syncFrame  &&  hasAudio  &&  TickCount() >= checkAudioSyncTime)
		{
			RobotAudStatus astat;
			Bool worked = QueryAudRobot(&astat);

			#ifdef DEBUG
			if (!worked)
				DebugPrintf("Audio Query failed!\n");
			#endif

			if (worked)
			{
				assert(astat.bytesPlayed >= 0);
				assert(astat.bytesPlaying >= astat.bytesPlayed);
				assert(astat.bytesSubmitted >= astat.bytesPlaying);
				assert(startingFrameNum >= 0);
				assert(startingFrameNum <= CalcFrameNum());

				int bytesPerFrame = astat.rate / normalFrameRate;
				if (astat.bits == 16)
					bytesPerFrame *= 2;

				checkAudioSyncTime = TickCount() + (60 / 3);			//  check again in 1/3 sec
				int videoFrameNow = CalcFrameNum() - startingFrameNum;
				assert(videoFrameNow >= 0);
				int audioFrameNow = astat.bytesPlaying / bytesPerFrame;

				//  Make sure we haven't had an unusual delay (alert boxes, etc)
				if (videoFrameNow < frameCount  &&  audioFrameNow < frameCount)
				{
					//DebugPrintf("audioFrame: %d  videoFrame: %d\n", audioFrameNow, videoFrameNow);
	
					if (audioFrameNow < videoFrameNow - 1  &&  frameRate != lowFrameRate)
					{
						frameRate = lowFrameRate;

						//  We don't want to go backwards in time
						if (audioFrameNow < frameNum)
							SetRobotTime(frameNum);
						else
							SetRobotTime(audioFrameNow);

						//DebugPrintf("-");
					}
					else if (audioFrameNow > videoFrameNow + 1  &&  frameRate != highFrameRate)
					{
						frameRate = highFrameRate;

						//  We don't want to go backwards in time
						if (audioFrameNow < frameNum)
							SetRobotTime(frameNum);
						else
							SetRobotTime(audioFrameNow);

					  	//DebugPrintf("+");
					}
					else if (frameRate != normalFrameRate)
					{
						frameRate = normalFrameRate;

						//  We don't want to go backwards in time
						if (audioFrameNow < frameNum)
							SetRobotTime(frameNum);
						else
							SetRobotTime(audioFrameNow);

						//DebugPrintf(".");
					}
				}
			}
		}
	}

	AsyncEventCheck();
}


/**************************************************************
   Pause method

   Note that pause/resume will not do anything if we are at FINAL.
	Pause will *always* terminate the audio for the remaining duration
	of the robot!


 **************************************************************/

void
Robot::Pause()
{
	LOG("Pause\n");
	assertMsg(status != ROBOT_UNUSED, "Robot::Pause - Called when robot uninitialized");
	assertMsg(status != ROBOT_KILLED, "Robot::Pause - Called after robot killed");

	if (status == ROBOT_PLAYING)
	{
		if (hasAudio)
			audioList.StopAudioNow();
		status = ROBOT_PAUSED;
		frameRate = normalFrameRate;
	}
}


/**************************************************************
   SetPriority method

	Called from SCI kernel
	Accepts priority or -1 means floating priority
	The priority will take effect on the next frameout and
	continue until the robot ends or SetPriority called again

 **************************************************************/

void
Robot::SetPriority(int pri)
{
	LOG("SetPriority\n");
	assertMsg(status != ROBOT_UNUSED, "Robot::Pause - Called when robot uninitialized");
	assertMsg(status != ROBOT_KILLED, "Robot::Pause - Called after robot killed");

	for (int i = 0; i < SiArraySize; i++)
		priority[i] = pri;
}


/**************************************************************
   Resume method

   Note that pause/resume will not do anything if we are at FINAL


 **************************************************************/

void
Robot::Resume()
{	
	LOG("Resume\n");
	assertMsg(status != ROBOT_UNUSED, "Robot::Resume - Called when robot uninitialized");
	assertMsg(status != ROBOT_KILLED, "Robot::Resume - Called after robot killed");

	if (status == ROBOT_PAUSED)
	{
		startingFrameNum = frameNum;
		status = ROBOT_PLAYING;
		if (hasAudio)
		{
			Bool worked = InitAudio((frameNum * 60) / frameRate);
			assert(worked);
			syncFrame = True;
		}

		SetRobotTime(frameNum);

		//  BUGBUG - DF  -  Could a cue be skipped or given twice if robot paused & resumed
		//  This might happen due to the uncertain nature of when SCI calls GetCel.
		//  Please consider this situation carefully.

		//  Wipe out all cues that occur before our new starting frameNum
		for (int i = 0; i < CueArraySize; i++)
		{
			cueTimeList[i] = masterCueTimeList[i];
			if (cueTimeList[i] != -1  &&  cueTimeList[i] < frameNum)
				cueTimeList[i] = -1;
		}
	}
}




/**************************************************************
   Assert method


 **************************************************************/
#ifdef DEBUG
Bool
Robot::Assert() const
{
	if (videoSizeArrayCopy)
	{
		for (int i = 0; i < frameCount; i++)
		{
			if (videoSizeArray[i] != videoSizeArrayCopy[i])
			{
				DebugPrintf("videoSizeArray corrupt");
				return False;
			} 
			if (recordPosArray[i] != recordPosArrayCopy[i])
			{
				DebugPrintf("recordPosArray corrupt");
				return False;
			}
		}
	}
	return audioList.Assert();
}
#endif


Bool
Robot::DoVersion5(Bool submitAudio)
{
	LOG("DoVersion5\n");

	int oldSiCount = siCount;
	ushort videoSize = videoSizeArray[frameNum];
	uchar* scratch = New uchar[videoSize];

	ReadFile file(fd);
	if (!file.Read(scratch, videoSize))
	{
		DebugPrintf("Read error!\n");
		delete[] scratch;
		return False;
	}

	siCount = * (short*) scratch;
	assert(siCount > 0);
	assert(siCount <= SiArraySize);
	if (siCount < 0  ||  siCount > SiArraySize)
	{
		delete[] scratch;
		return False;
	}

	if (hasAudio  &&  submitAudio)
	{
		int audCount, audPos;
		uchar* buffer = New uchar[ZeroCompressCount + expectedAudCount];
		if (ReadAudioDataFromRecord(frameNum, buffer, &audPos, &audCount))
			audioList.AddBlock(audPos, audCount, buffer);

		delete[] buffer;
	}

	CreateCel5(scratch + 2, siCount, True);

	for (int i = 0; i < siCount; i++)
	{
		if (!robotSi[i])
		{
			robotSi[i] = New ScreenItem(planeId, Bitmap(celHandleArray[i].id));
			robotSi[i]->pos = SOL_Point(x1[i], y1[i]);
			if (priority[i] == -1)
				robotSi[i]->SetFloatPriority();
			else
				robotSi[i]->SetPriority(priority[i]);
			graphMgr->AddScreenItem(robotSi[i]);
		}
		else
		{
			robotSi[i]->bitmap = celHandleArray[i].id;
			robotSi[i]->pos = SOL_Point(x1[i], y1[i]);
			if (priority[i] == -1)
				robotSi[i]->SetFloatPriority();
			else
				robotSi[i]->SetPriority(priority[i]);
			graphMgr->UpdateScreenItem(robotSi[i]);
		}
	}

	for (i = siCount; i < oldSiCount; i++)
	{
		graphMgr->DeleteScreenItem(robotSi[i]);
		robotSi[i] = NULL;
	}

	delete[] scratch;
	return !file.DidErrorAccum();
}



/**************************************************************
   DisplayFrame method

   Used only from an SCI kernel call.  It cancels the realtime
	aspect of the robot, entering a paused state which kills the audio
	for the remainder of the robot.
	
	If DisplayFrame is repeatedly	called to show the same frame, 
	it will NOT be reloaded from the disk each time.


 **************************************************************/

void
Robot::DisplayFrame(int frame, int newXOff, int newYOff, int newPri)
{
	LOG("DisplayFrame\n");
	assertMsg(status != ROBOT_UNUSED, "Robot::DisplayFrame - Called when robot uninitialized");
	assertMsg(status != ROBOT_KILLED, "Robot::DisplayFrame - Called after robot killed");
	assertMsg(frame >= 0  &&  frame < frameCount, "Robot::DisplayFrame - Frame out of range!\n");

	//  Update xoff, yoff, priority if override wanted
	if (newXOff != Unspecified)
		xoff = newXOff;
	if (newYOff != Unspecified)
		yoff = newYOff;
	if (newPri != Unspecified)
	{
		for (int i = 0; i < SiArraySize; i++)
			priority[i] = newPri;
	}

	frameNum = frame;
	Pause();

	if (frame != lastFrameNum)
	{
		SeekFrame(frameNum);
		if (version == 5)
			DoVersion5(False);			//  Returns False if IO failed!
		else
			msgMgr->Fatal("Unsupported robot version: %d\n", (int) version);
	}
	else
	{
		//  In case xoff, yoff was changed this call
		assert(siCount <= SiArraySize);
		for (int i = 0; i < siCount; i++)
		{
			if (!hiresCoords)
			{
		 		x1[i] = fileX1[i] + xoff;
				y1[i] = fileY1[i] + yoff;
			}
			else
			{
				CelHeader* celHeader;
				assert(celHandleArray[i].id.IsValid());
				celHeader = (CelHeader*) *celHandleArray[i].id;

				xsize = celHeader->xDim;
				ysize = celHeader->yDim;

				int gameResX = graphMgr->Xdim();
				int gameResY = graphMgr->Ydim();

				int highResX1 = fileX1[i] + xoff;
				int highResY1 = fileY1[i] + yoff;
				int highResY2 = highResY1 + ysize - 1;

				int xorg = 0;
				int yorg = ysize - 1;
				celHeader->xHot = xorg;
				celHeader->yHot = yorg;
				x1[i] = highResX1;
				y1[i] = highResY2;
			}

			MemID id;
			id = celHandleArray[i].id;
			assert(id.IsValid());

			if (!robotSi[i])
			{
				robotSi[i] = New ScreenItem(planeId, Bitmap(id));
				robotSi[i]->pos = SOL_Point(x1[i], y1[i]);
				if (priority[i] == -1)
					robotSi[i]->SetFloatPriority();
				else
					robotSi[i]->SetPriority(priority[i]);
				graphMgr->AddScreenItem(robotSi[i]);
			}
			else
			{
				robotSi[i]->bitmap = id;
				robotSi[i]->pos = SOL_Point(x1[i], y1[i]);
				if (priority[i] == -1)
					robotSi[i]->SetFloatPriority();
				else
					robotSi[i]->SetPriority(priority[i]);
				graphMgr->UpdateScreenItem(robotSi[i]);
			}
		}
	}

	lastFrameNum = frameNum;
}

/**************************************************************
   KillRobot method

	Used only from Kernel Call.  Stops the robot and audio, next
	frameout will remove the robot from the screen.


 **************************************************************/

void
Robot::KillRobot()
{
	if (fd)
	{
		resMgr->Close(fd);
		fd = 0;
	}

	for (int i = 0; i < siCount; i++)
	{
		graphMgr->DeleteScreenItem(robotSi[i]);
		robotSi[i] = NULL;
	}
	siCount = 0;

	if (hasAudio)
	{
		audioList.StopAudioNow();
		frameRate = normalFrameRate;
		hasAudio = False;
	}
	status = ROBOT_KILLED;
}




/**************************************************************
   GetCue method

	Used only from Kernel Call.  
	
	Can be called at any time.

	Returns 0 if no cue, cue # if one found.
	If a cue is found, the cue'd frame will be shown on the next
	frameout, irregardless of timing.  This is necessary to sync
	the appearance / disappearance of objects.

   BUGBUG - DF	- GetCues interaction with Resume/DoRobot will depend on SCI calls

	One of the problems with GetCue is that SCI can call it at any time,
	and I'm forced to estimate what the next frame number will be.
	This introduces some element of uncertainty to this section, and
	could cause trouble when we try to save/restore games, pause, etc
	because a cue may or may not have gone off at a particular point in
	the robot.

   It may be better to move this logic to the DoRobot section, have it
	save a "cue" variable or list, and use this routine to report that
	value back to SCI at any time.  I haven't thought it through
	extensively yet, so check this concept carefully.


 **************************************************************/

long
Robot::GetCue()
{
	//  This if statement compensates for bad timing code in Phantasmagoria#1
	//  It should become a Warning or Fatal message that stops game play
	assert(status != ROBOT_UNUSED);
	assert(status != ROBOT_KILLED);
	assert(status != ROBOT_PAUSED);

	if (status == ROBOT_UNUSED  ||  status == ROBOT_KILLED  ||  status == ROBOT_PAUSED)
		return 0;

	//  If we are waiting for handshake, return -1
	if (graphMgr->RobotStatus() == ROBOT_FINAL)
		return -1;

	//  Nothing displayed yet, so ignore cue request
	if (syncFrame)
		return 0;

	//  Search for cue time <= estimated next current frame number
	assert(status == ROBOT_PLAYING);
	int estNextFrameNum = CalcFrameNum(delayTime.PredictedTicks());

	assert(estNextFrameNum >= 0);
	assert(frameCount > 0);

	if (estNextFrameNum >= frameCount)
		estNextFrameNum = frameCount - 1;

	//  Cannot have a cue on the last frame, because we force the next frame
	//  to show.  This condition should be screened during init.
	//  Also, how does cueTimeList work on non-10-per-sec robots?

	for (int i = 0; i < CueArraySize; i++)
	{
		if (cueTimeList[i] != -1  &&  cueTimeList[i] <= estNextFrameNum)
		{
			if (cueTimeList[i] >= lastFrameNum)
				cueForceShowFrame = cueTimeList[i] + 1;

			cueTimeList[i] = -1;
			return cueValueList[i];
		}
	}

	//  No cues found for this time
	return 0;
}

Bool
Robot::IsRobotRealTime() const
{
	if (status == ROBOT_PLAYING  &&  !syncFrame  &&  fd)	// BUGBUG - DDF - syncFrame? maybe frameNum == 0
		return True;
	return False;
}

void
Robot::SetRobotTime(int myFrame)
{
	assert(myFrame >= 0  &&  myFrame < frameCount);
	assert(myFrame >= frameNum);
	robotStartTime = TickCount();
	robotStartFrame = myFrame;
}


int
Robot::CalcFrameNum(int extraTicks)
{
	assert(extraTicks >= 0);
	assert(robotStartTime >= 0);
	assert(robotStartTime <= TickCount());
	assert(robotStartFrame >= 0);

	int calcFrame = ToFrame(TickCount() + extraTicks - robotStartTime) + robotStartFrame;
	assert(calcFrame >= 0);
	return calcFrame;
}



/*************************************************************************
 PreAllocCelMemory method

 Allocates, fills celHandleArray with MemIDs to minimum sized blocks of
 memory for each cel in this frame.  It also handles deallocating previous
 blocks (if needed), before starting the new allocations.

 Also maintains SquashedCel data for intermediate decoding of cel.

 *************************************************************************/

void
Robot::PreAllocCelMemory(const uchar* video, int celCount)
{
	assert(video);
	assert(celCount > 0  &&  celCount <= SiArraySize);
	assert(celHandleArraySize <= SiArraySize);

   //  Deallocate previous dynamically allocated cels (marked as FrameLifeTime)
	if (celHandleArray)
	{
		for (int i = 0; i < celHandleArraySize; i++)
		{
			if (celHandleArray[i].status == CelHandleInfo::FrameLifetime)
			{
				celHandleArray[i].id.Free();
				celHandleArray[i].id = 0;
				celHandleArray[i].status = CelHandleInfo::Invalid;
				celHandleArray[i].area = 0;
			}
		}
	}

   //  Deallocate handle array
	delete[] celHandleArray;

   //  Alloc a handle array, inited to zero, marked as Invalid
	celHandleArray = New CelHandleInfo[celCount];
	celHandleArraySize = celCount;

   //  Copy fixed mem handles to first section of handle array, mark as RobotLifeTime
	int count = celCount;
	if (count > FixedCelArraySize)
		count = FixedCelArraySize;

	for (int i = 0; i < count; i++)
	{
		//  If version 5 robots, max cels per frame is wrong, so consider
		if (!celHandleArray[i].id)
			break;

		celHandleArray[i].id = fixedCelHandle[i];
		celHandleArray[i].status = CelHandleInfo::RobotLifetime;
		celHandleArray[i].area = maxCelArea[i];
	}


	long maxAreaThisFrame = 0;
	for (i = 0; i < celCount; i++)
	{
	   //  Keep running total of max size required for this frame
		const VideoHeader4* vh = (const VideoHeader4*) video;
		long area = vh->xsize * vh->ysize;
		if (area > maxAreaThisFrame)
			maxAreaThisFrame = area;

		//  If memory prealloced, check that size is sufficient to hold the cel
		if (celHandleArray[i].status == CelHandleInfo::RobotLifetime)
		{
			assert(i < FixedCelArraySize);

			//  If insufficient, allocate one with a frame lifetime for this cel
			if (maxCelArea[i] < area)
			{
				//DebugPrintf("Allocating frame lifetime memory!\n");
				celHandleArray[i].id.Get(MemBitmap, area + CELHEADERSIZE + PALSIZE);
				celHandleArray[i].area = area;
				celHandleArray[i].status = CelHandleInfo::FrameLifetime;				
			}
		}
		//  If memory not allocated, do so
		else if (celHandleArray[i].status == CelHandleInfo::Invalid)
		{
			//DebugPrintf("Allocating additional frame lifetime memory!\n");
			celHandleArray[i].id.Get(MemBitmap, area + CELHEADERSIZE + PALSIZE);
			celHandleArray[i].area = area;
			celHandleArray[i].status = CelHandleInfo::FrameLifetime;				
		}
		else
			msgMgr->Fatal("Cel Handle has bad status");

		video += sizeof(VideoHeader4);
		video += vh->size;
	}

   //  test max size required against intermediate buffer storage size
   //  if too small, dealloc old intermediate area and realloc new
	if (maxAreaThisFrame > squashedCelArea)
	{
		//DebugPrintf("Allocating squash memory!\n");
		squashedCelHandle.Free();
		squashedCelHandle.Get(MemBitmap, maxAreaThisFrame);
		squashedCelArea = maxAreaThisFrame;
		assert(version <= 5);
	}

	#ifdef DEBUG
	assert(celHandleArray);
	assert(celHandleArraySize == celCount);
	assert(squashedCelHandle.IsValid());
	assert(squashedCelArea > 0);

	for (i = 0; i < celCount; i++)
	{
		assert(celHandleArray[i].id.IsValid());		
		assert(celHandleArray[i].status == CelHandleInfo::FrameLifetime  ||
				 celHandleArray[i].status == CelHandleInfo::RobotLifetime);
		assert(celHandleArray[i].area > 0);
	}
	#endif
}

void
Robot::CreateCel5(const uchar* video, int celCount, Bool usePalette)
{
	assert(video);
	assert(celCount > 0  &&  celCount <= SiArraySize);

	PreAllocCelMemory(video, celCount);
	for (int i = 0; i < celCount; i++)
	{
		Create1Cel5(video, i, usePalette);
		const VideoHeader4* vh = (const VideoHeader4*) video;
		int size = vh->size;
		video += sizeof(VideoHeader4);
		video += size;
	}
}


void
Robot::Create1Cel5(const uchar* video, int index, Bool usePalette)
{
	assert(video);
	assert(index >= 0  &&  index < SiArraySize);

	VideoHeader4* vh = (VideoHeader4*) video;
	video += sizeof(VideoHeader4);
	vertScaleFactor = vh->vertScaleFactor;

	xsize = vh->xsize;
	ysize = vh->ysize;
	int xorg, yorg;
	int gameResX = graphMgr->Xdim();
	int gameResY = graphMgr->Ydim();

	int highResX1 = vh->x1 + xoff;
	int highResY1 = vh->y1 + yoff;
	int highResY2 = highResY1 + ysize - 1;

	xorg = 0;
	yorg = ysize - 1;
	x1[index] = highResX1;
	y1[index] = highResY2;

	fileX1[index] = vh->x1;
	fileY1[index] = vh->y1;

	assert(celHandleArray[index].area >= xsize * ysize);
	assert(celHandleArray[index].id.IsValid());
	BuildHeader(celHandleArray[index].id, xsize, ysize, 255, xorg, yorg, xRes, yRes, usePalette);

	uchar* destBuff = NULL;
	if (vertScaleFactor == 100)
		destBuff = (uchar*) *celHandleArray[index].id + CELHEADERSIZE;
	else
	{
		assert(squashedCelArea >= xsize * ((ysize * vertScaleFactor) / 100));
 		destBuff = (uchar*) *squashedCelHandle;
	}

	//  Loop through proper number of sub-blocks
	uchar* ptr = destBuff;
	for (int i = 0; i < vh->checksum; i++)
	{
		long	cmpSize = * (long*) &video[0];
		long	destSize = * (long*) &video[4];
		short compType = * (short*) &video[8];

	#ifdef DANTEST1
		short checksum2 = * (short*) &video[10];
		short checksum = 0;
		video += 12;
		uchar* vidptr = video;
		for (int i = 0; i < cmpSize; i++)
			checksum ^= *vidptr++;
		if (checksum != checksum2)
			msgMgr->Fatal("Checksum error!");
	#else
		video += 10;
	#endif


		switch (compType)
		{
			case LZ_COMP:
				LZDecode(destBuff, video, cmpSize, destSize);
				break;

			case UNCOMP:
				memcpy(destBuff, video, destSize);
				break;
				
			default:
				msgMgr->Fatal("Unknown compression type!");
				break;
		}
		
		video += cmpSize;
		destBuff += destSize;
	}


	if (WING)
		OffsetData(ptr, PALSHIFT);

	if (vertScaleFactor != 100)
		ExpandData((uchar*) *celHandleArray[index].id + CELHEADERSIZE, ptr);

	if (usePalette)
		memcpy((uchar*) *celHandleArray[index].id + CELHEADERSIZE + (xsize * ysize), hunkPalette, PALSIZE);
}





void
Robot::LZDecode(uchar* dest, const uchar* src, long srcCount1)
{
	assert(src);
	assert(dest);
	assert(srcCount1 > 0);

	int xdim = xsize;
	int ydim = (ysize * vertScaleFactor) / 100;
	ulong destCount = xdim * ydim;
	long tmpDestCount = destCount;
	uchar* tmpDest = dest;

	Initcompress(lzScratch);

	ulong srcCount = srcCount1;
	int result = Decompress((char**) &src, (char**) &dest, &srcCount, &destCount, lzScratch);
//	if (srcCount  ||  result != 1)

	srcCount = 0;
	result = Decompress((char**) &src, (char**) &dest, &srcCount, &destCount, lzScratch);
	if (destCount > 1)
	{
		#ifdef DEBUG
		DebugPrintf("Decompression error!  Data from CD faulty?\n");
		#endif
		memset(tmpDest, 255, tmpDestCount);	//  otherwise fill with skip
	}
}





void
Robot::LZDecode(uchar* dest, const uchar* src, long srcCount1, long destCount1)
{
	assert(src);
	assert(dest);
	assert(srcCount1 > 0);
	assert(destCount1 >= srcCount1);

	ulong srcCount = srcCount1;
	ulong destCount = destCount1;
	uchar* tmpDest = dest;

	Initcompress(lzScratch);
	int result = Decompress((char**) &src, (char**) &dest, &srcCount, &destCount, lzScratch);
	srcCount = 0;
	result = Decompress((char**) &src, (char**) &dest, &srcCount, &destCount, lzScratch);
	if (destCount > 1)
	{
		#ifdef DEBUG
		DebugPrintf("Decompression error!  Data from CD faulty?\n");
		#endif
		memset(tmpDest, 255, destCount1);	//  otherwise fill with skip
	}
}





void
Robot::ExpandData(uchar* dest, const uchar* src)
{
	assert(src);
	assert(dest);

	int ydim = (ysize * vertScaleFactor) / 100;
	assert(ydim);

	int numer = ysize;
	int denom = ydim;
	int total = 0;

	for (int y = ydim - 1; y >= 0; y--)
	{
		total += numer;
		int count = total / denom;
		total %= denom;

		while (count--)
		{
			memcpy(dest, src, xsize);
			dest += xsize;
		}
		src += xsize;
	}
}


void
Robot::OffsetData(uchar* ptr, char offset)
{
	assert(ptr);
	int ydim = (ysize * vertScaleFactor) / 100;
	for (int y = 0; y < ydim; y++)
	{
		for (int x = 0; x < xsize; x++)
		{
			if (*ptr < 246)
				*ptr += offset;
			ptr++;
		}
	}
}


int
Robot::ToFrame(int ticks)
{
	assert(ticks >= 0);
	return (ticks * frameRate) / 60;
}


int
Robot::Info(SOL_Rect* rect)
{
	assertMsg(status != ROBOT_UNUSED, "Robot::Info - Called when robot uninitialized");
	assertMsg(status != ROBOT_KILLED, "Robot::Info - Called after robot killed");
	assert(rect);

	SOL_Rect rectUnion;
	rectUnion.MakeEmpty();

	if (siCount)
	{
		for (int i = 0; i < siCount; i++)
		{
			SOL_Rect rect1;
			long x = graphMgr->NextScreen()->Planes().Search(robotSi[i]->PlaneId());
			robotSi[i]->NowSeen(graphMgr->NextScreen()->Planes()[x], rect1);
			if (!i)
				rectUnion = rect1;
			else
				rectUnion.Both(rect1);
		}
	}

	*rect = rectUnion;
	return frameCount;
}



void
KRobot(argList)
{
	// Robot functions
	//      arg1-----function code
	//					
	enum {
		rOpen,
		rDisplayFrame,
		rFrameInfo,
		rSaveOffset,
		rPlay,
		rHasEnded,	//  true only if waiting for handshake
		rExists,		//  true anytime robot started but not terminated
		rTerminate,	//  Deletes final cel from graphMgr, clears robot state
						//  Does NOT call frameout.
      rGetCue,		//  returns cues to Robot SCI object
		rIsPaused,
		rPause,
		rFrameNum,
		rSetPriority
	};

	// The arguments vary from function to function
	// The return value varys from function to function

	int function = arg(1);
	SOL_Rect rect;
	ArrayID id;
	SCIWord* data;
	LOG("KRobot\n");

	switch (function)
	{
		case rOpen:
			// Start Robot
			//	-----------
			// Input
			//      arg2-----robot resource number
			//      arg3-----plane ID
			//      arg4-----priority
			//      arg5-----x Offset
			//      arg6-----y Offset
			// Output
			//		  pm.acc = ?

			assert(argCount == 6);
			graphMgr->InitRobot(arg(2), arg(3), arg(4), arg(5), arg(6));
			pm.acc = False;
			break;

		case rDisplayFrame:
			assert(argCount == 2  || argCount == 4);
			if (argCount < 3)
				graphMgr->GRobot().DisplayFrame(arg(2));
			else
				graphMgr->GRobot().DisplayFrame(arg(2), arg(3), arg(4));
			break;

		case rFrameInfo:
			// Input -
			//              arg(1) = ArrayID to array for rect

			assert(argCount == 2);
			id = (ArrayID) arg(2);
			assert(id.IsValid());
			assert(id.Size() >= 8);

			pm.acc = graphMgr->GRobot().Info(&rect);
			data = (SCIWord*) id.Data();
			data[0] = rect.A.x;
			data[1] = rect.A.y;
			data[2] = rect.B.x;
			data[3] = rect.B.y;
			break;


		case rPlay:
			assert(argCount == 1);
			graphMgr->GRobot().Resume();
			break;

		case rPause:
			assert(argCount == 1);
			graphMgr->GRobot().Pause();
			break;

		case rFrameNum:
			assert(argCount == 1);
			pm.acc = graphMgr->GRobot().FrameNum();
			break;

		case rSetPriority:
			assert(argCount == 2);
			graphMgr->GRobot().SetPriority(arg(2));
			break;

		case rHasEnded:
			// return true if waiting for terminating handshake
			//	------------------------------------------------
			// Input
			// 	  None
			// Output
			//		  pm.acc = True / False

			assert(argCount == 1);
			pm.acc = (graphMgr->RobotStatus() == ROBOT_FINAL);
			break;

		case rExists:
			// return whether robot sequence in progress (visible)
			// ---------------------------------------------------
			// Input
			//      None
			// Output
			//      pm.acc = True / False

			assert(argCount == 1);
			pm.acc = (graphMgr->RobotStatus() != ROBOT_UNUSED);
			break;

		case rTerminate:
			// Input
			//      None
			// Output

			assert(argCount == 1);
			graphMgr->KillRobot();
			break;

		case rGetCue:
		{
			// Input
			//      arg2-----robot object
			// Output
         //    cue values returned
         //       0  no ques and not at end of play
         //       -1 play over last frame still on screen
         //       que value of oldest cue encountered that has not been returned 

			assert(argCount == 2);
	      ObjectID robotObject = (ObjectID) arg(2);
			assert(robotObject.IsValid());

			robotObject.SetIndexedProperty(sndSignal, (Property) graphMgr->GRobot().GetCue());
			break;
		}

		case rIsPaused:
			assert(argCount == 1);
			assert(graphMgr->RobotStatus() != ROBOT_UNUSED);
			pm.acc = (graphMgr->RobotStatus() == ROBOT_PAUSED);
			break;

		default:
		case rSaveOffset:
			msgMgr->Alert("Robot Kernel function not implemented");
			break;
	}
}


Bool
Robot::SeekFrame(int frame)
{
	assert(frame < frameCount);
	assert(frame >= 0);

	ReadFile file(fd);
	file.LSeek(recordPosArray[frame], SEEK_SET);
	assert(file.DidErrorAccum() == False);
	return !file.DidErrorAccum();
}


Bool
Robot::Open(int* fd, ResNum num)
{
	int fd1 = resMgr->Open(MemResRobot, num);
	if (critErrHandler->Tripped() ||  fd1 < 1)
		return False;

	*fd = fd1;
	return True;
}


Robot::AudioList::AudioBlock::AudioBlock(int pos, int size) : pos(pos), size(size)
{
	assert(size > 0);
	memid.Get(MemResAudio, size);
	memid.SetNotDiscardable();
	assert(memid);
	assert(memid.IsValid());
}


Robot::AudioList::AudioBlock::~AudioBlock()
{
	assert(memid);
	assert(memid.IsValid());
	memid.Free();
}

Bool
Robot::AudioList::AudioBlock::Submit(int audioStartPos)
{
	LOG("Submit\n");
	assert(audioStartPos >= 0);
	assert(pos >= audioStartPos);
	return AudRobot(RobotAudInfo(memid, size, (pos - audioStartPos) * 2));
}

#ifdef DEBUG
Bool
Robot::AudioList::AudioBlock::Assert() const
{
	if (!memid.IsValid())
	{
		DebugPrintf("Memid invalid");
		return False;
	}

	if (pos < 0)
	{
		DebugPrintf("pos negative");
		return False;
	}

	if (size <= 0  || size > 65536L)
	{
		DebugPrintf("size range error");
		return False;
	}

	return True;
}
#endif
		
uchar*
Robot::AudioList::AudioBlock::operator*()
{
	return (uchar*) *memid;
}


Robot::AudioList::AudioList()
{
	for (int i = 0; i < AudioListSize; i++)
		array[i] = NULL;
	oldestBlockIndex = 0;
	newestBlockIndex = 0;
	blockCount = 0;
	audioStartOffset = 0;
	audioState = AUDIO_READYTOSTART;
}


#ifdef DEBUG
Bool
Robot::AudioList::Assert() const
{
	if (blockCount < 0  ||  blockCount > AudioListSize)
	{
		DebugPrintf("BlockCount out of range");
		return False;
	}

	if (oldestBlockIndex < 0  ||  oldestBlockIndex >= AudioListSize)
	{
		DebugPrintf("oldestBlockIndex out of range");
		return False;
	}

	if (newestBlockIndex < 0  ||  newestBlockIndex >= AudioListSize)
	{
		DebugPrintf("newestBlockIndex out of range");
		return False;
	}

	if (audioStartOffset < 0)
	{
		DebugPrintf("audioStartOffset negative");
		return False;
	}

	if (!(audioState > START_ENUM  ||  audioState < END_ENUM))
	{
		DebugPrintf("audioState corrupt");
		return False;
	}

	int i = oldestBlockIndex;
	int lastPos = -1;
	for (int count = blockCount; count--; )
	{
		if (!array[i])
		{
			DebugPrintf("Array element null");
			return False;
		}

		if (!array[i]->Assert())
		{
			DebugPrintf("element assert failed");
			return False;
		}
		if (array[i]->pos <= lastPos)
		{
			DebugPrintf("Audio blocks out of sequence old: %d  new: %d", lastPos, array[i]->pos);
			return False;
		}
		lastPos = array[i]->pos;

		i++;
		if (i == AudioListSize)
			i = 0;
	}

	return True;
}
#endif


Robot::AudioList::~AudioList()
{
	assert(Assert());
	for (int i = 0; i < AudioListSize; i++)
		delete array[i];
}



void
Robot::AudioList::StopAudio()
{
	LOG("StopAudio\n");
	assert(Assert());
	assert(audioState == AUDIO_PLAYING);

	AudRobot(RobotAudInfo(NULL, -1, -1));
	FreeAudioBlocks();

	audioState = AUDIO_STOPPING;
}



/***************************************************************
 StopAudioNow

 Can be called at any time to ensure that stuff is cleaned up right

 ***************************************************************/

void
Robot::AudioList::StopAudioNow()
{
	LOG("StopAudioNow\n");
	assert(Assert());

	if (audioState == AUDIO_PLAYING  ||  audioState == AUDIO_STOPPING  ||  audioState == AUDIO_PAUSED)
	{
		AudRobot(RobotAudInfo(NULL, 0, 0));
		audioState = AUDIO_STOPPED;
	}

	FreeAudioBlocks();
}


void
Robot::AudioList::FreeAudioBlocks()
{
	LOG("FreeAudioBlocks\n");
	assert(Assert());

	while (blockCount)
	{
		delete array[oldestBlockIndex];
		array[oldestBlockIndex] = NULL;
		oldestBlockIndex++;
		if (oldestBlockIndex == AudioListSize)
			oldestBlockIndex = 0;

		blockCount--;
	}
}




void
Robot::AudioList::AddBlock(int pos, int size, const uchar* data)
{
	LOG("AddBlock\n");
 	assert(Assert());
	assert(pos >= 0);
	assert(size > 0  &&  size < 64000);
	assert(data != NULL);
	assert(audioState == AUDIO_PAUSED  ||  audioState == AUDIO_PLAYING);

	//  If audio List is full, delete the oldest audio block
	if (blockCount == AudioListSize)
	{
		//DebugPrintf("Robot audio list full - Deleting oldest audio!\n");
		delete array[oldestBlockIndex];
		array[oldestBlockIndex] = NULL;
		oldestBlockIndex++;
		if (oldestBlockIndex == AudioListSize)
			oldestBlockIndex = 0;
		blockCount--;
	}

	//  If first block, init variables properly
	if (!blockCount)
		oldestBlockIndex = newestBlockIndex = 0;
	else
	{
		newestBlockIndex++;
		if (newestBlockIndex == AudioListSize)
			newestBlockIndex = 0;
	}

	//  Allocate audio block, copy users data into it
	array[newestBlockIndex] = New AudioBlock(pos, size);
	blockCount++;

	uchar* dest = **array[newestBlockIndex];
	assert(dest);
	memcpy(dest, data, size);
}



void
Robot::AudioList::SubmitDriverMax()
{
	LOG("SubmitDriverMax\n");
	assert(Assert());

	//  Try to output any blocks we have now
	while (blockCount)
	{
		if (!array[oldestBlockIndex]->Submit(audioStartOffset))
			return;

		delete array[oldestBlockIndex];
		array[oldestBlockIndex] = NULL;
		oldestBlockIndex++;
		if (oldestBlockIndex == AudioListSize)
			oldestBlockIndex = 0;

		blockCount--;
	}
}



void
Robot::AudioList::SetAudioOffset(int offset)
{
	LOG("SetAudioOffset\n");
	assert(Assert());
	assert(blockCount == 0);
	assert(offset >= 0);
	assert(audioState == AUDIO_READYTOSTART);

	audioStartOffset = offset;
}

void
Robot::AudioList::PrepareForPrimer()
{
	LOG("PrepareForPrimer\n");
	assert(Assert());

	assert(audioState == AUDIO_READYTOSTART);
	assert(blockCount == 0);
	
	Bool worked = AudioPause(ROBOTS_ONLY);
//	assert(worked);				BUGBUG failed!!

	audioState = AUDIO_PAUSED;
}


void
Robot::AudioList::StartAudioNow()
{
	LOG("StartAudioNow\n");
	assert(Assert());
	assert(audioState == AUDIO_PAUSED);

	SubmitDriverMax();
	Bool worked = AudioResume(ROBOTS_ONLY);
//	assert(worked);			BUGBUG failed!!
	audioState = AUDIO_PLAYING;
}


void
Robot::AudioList::Reset()
{
	LOG("AudioList Reset\n");
	assert(Assert());

	StopAudioNow();
	audioStartOffset = 0;
	audioState = AUDIO_READYTOSTART;
}


Robot::DelayTime::DelayTime()
{
	for (int i = 0; i < DelayArraySize; i++)
	{
		timeStamp[i] = i;
		delays[i] = 0;
	}

	earliestTimeStamp = 0;
	latestTimeStamp = DelayArraySize - 1;
	startTime = -1;
}

#ifdef DEBUG
void
Robot::DelayTime::Dump() const
{
	DebugPrintf("Robot::DelayTime Dump\n");
	DebugPrintf(" earliestTimeStamp: %d  latestTimeStamp: %d  startTime: %d\n",
			earliestTimeStamp, latestTimeStamp, startTime);

	for (int i = 0; i < DelayArraySize; i++)
	{
		DebugPrintf("i: %d  timeStamp: %d  delay: %d\n", i, timeStamp[i], delays[i]);
	}
}
#endif

void
Robot::DelayTime::StartTiming()
{
	assert(startTime == -1);
	startTime = graphMgr->GRobot().TickCount();
}

int
Robot::DelayTime::EndTiming()
{
	assert(startTime != -1);
		
	int time = graphMgr->GRobot().TickCount() - startTime;
	for (int i = 0; i < DelayArraySize; i++)
	{
		if (timeStamp[i] == earliestTimeStamp)
		{
			timeStamp[i] = ++latestTimeStamp;
			delays[i] = time;
			break;
		}
	}
	earliestTimeStamp++;
	startTime = -1;
	SortList();
	return time;
}

Bool
Robot::DelayTime::TimingInProgress() const
{
	return startTime != -1;
}

void
Robot::DelayTime::SortList()
{
	assert(startTime == -1);

	for (int i = 0; i < DelayArraySize - 1; i++)
	{
		long smallDelay = delays[i];
		int smallIndex = i;

		//  Search for smallest remaining in list
		for (int j = i + 1; j < DelayArraySize; j++)
		{
			if (delays[j] < smallDelay)
			{
				smallDelay = delays[j];
				smallIndex = j;
			}
		}

		//  If smaller found, swap it to the top of the list
		if (smallIndex != i)
		{
			long tmpDelay = delays[i];
			int tmpTimeStamp = timeStamp[i];

			delays[i] = smallDelay;
			timeStamp[i] = timeStamp[smallIndex];

			delays[smallIndex] = tmpDelay;
			timeStamp[smallIndex] = tmpTimeStamp;
		}
	}
}


long
Robot::DelayTime::PredictedTicks() const
{
	assert(startTime == -1);
	return delays[DelayArraySize / 2];
}


#ifdef DEBUG

void AssertMsgFail(const char* filename, int line, const char* str)
{
	msgMgr->Fatal("Assert failed: %s(%d) - %s\n", filename, line, str);
}


LogObj::LogObj(const char* str1)
{
	static char tmpStr[300];

	for (int i = 0; i < tab; i++)
		tmpStr[i] = ' ';

	while (*str1)
	{
		tmpStr[i] = *str1++;
		i++;
	}
	tmpStr[i] = 0;

	DebugPrintf(tmpStr);
	tab += 2;
}

LogObj::~LogObj()
{
	tab -= 2;
}

#endif


/***********************************************************************
 ***********************************************************************

  File Stuff






 ***********************************************************************
 ***********************************************************************/



ReadFile::ReadFile(int fd1)
{
	fileOpenFlag = True;
	errorAccum = False;
	fd = fd1;
}


Bool
ReadFile::Read(void* buff, int count)
{
	assert(fileOpenFlag);
	assert(count > 0);

	#ifdef DEBUG
	memset(buff, 0xFE, count);
	#endif

	int ret = ::Read(fd, buff, count);
	if (ret != count)
		errorAccum = True;

	return !errorAccum;
}

Bool
ReadFile::LSeek(int pos, int type)
{
	assert(fileOpenFlag);
	assert(pos >= 0);
	assert(type == SEEK_SET  ||  type == SEEK_CUR  ||  type == SEEK_END);

	int ret = ::LSeek(fd, pos, type);
	if (ret == -1)
		errorAccum = True;

	return errorAccum;
}

Bool
ReadFile::Tell(long* loc)
{
	assert(fileOpenFlag);
	assert(loc);

	int ret = ::LSeek(fd, 0, SEEK_CUR);
	if (ret == -1)
		errorAccum = True;
	else
		*loc = ret;

	return errorAccum;
}

Bool
ReadFile::Close()
{
	assert(fileOpenFlag);
	
	int ret = ::Close(fd);
	if (ret == -1)
		errorAccum = True;

	fileOpenFlag = False;
	fd = -1;
	return errorAccum;
}


//  Version 5 robot file format doco
#if 0

byte		type;				//  0x3D
byte		Unused;			//  0x00
char[4]	"SOL\0"			//  ID
short		version			//  0x0004
short		audioSize		//  0K or 4K   [4408]
short		audioPadSize	//  2K or 0K   (pad to next X boundary)
short		minSpeed			//  150, 300 (K per sec)
short		frameCount		//  Frame count
short		palSize			//  size of palette (Minimum 1200 bytes)
short		audioInitSize	//  primer audio size (4K min)
short		xRes				//  X resolution (0 if same as game)
short		yRes				//  Y resolution (0 if same as game)
byte		paletteFlag
byte		audioFlag
short		cueCount
short		frameRate		//  frames per second (normally 10)
short		accessType		//  0 for now
short		safeFrameSkipAudio	//  Number of records skippable w/o audio dropout
short		maxCelsPerFrame;
long		maxsize0;		//  Used for preallocating memory for robot
long		maxsize1;		//  ..
long		maxsize2;		//  ..
long		maxsize3;
long		0					//  Reserved
long		0					//  Reserved

byte		audioInit[audioInitSize];	//  See below
byte		palette[palSize];
short		videoSizeArray[frameCount];	//  Total size of video block (inc. headers)
long		recordSizeArray[frameCount];	//  Size of each record
long		cueTimeList[256];
short		cueValueList[256];

Padding to next 2K bounds


Video Block (padSize - audioSize)
Audio Block (if any) (audioSize includes all audio headers)
Video Block
Audio Block




Video Block
	short	nCels;				//  1 or more

	byte	compressionType;	//  LZ, RLE, None
	byte	vertScaleFactor;	//  100 = No reduction
	short	xsize;				//  Hires XDim
	short	ysize;				//  Hires YDim
	short	xoff;					//  Hires origin
	short	yoff;					//  Hires origin
	short	x1;					//  Lores origin placement
	short	y1;					//  Lores origin placement
	short	size;					//  compressed size of cel
	short	checksum;			//  now used for chunk count in data block
	short	blockCount;
	short	0						//  Future expansion
	byte 	data[size]			//  videoData

		Contents of videoData block (1 or more chunks)
		This structure repeats chunk count (checksum) times:

		long	cmpSize			//  compressed size ( below 64K )
		long	destSize			//  size of block when uncompressed ( below 64K )
		short compType			//  compression type (LZ or UNCOMP)
		byte	cmpData[cmpSize]	//  actual data (usually LZ compressed)



	If 2 cels, etc
	byte	compressionType;	//  LZ, RLE, None
	byte	vertScaleFactor;	//  100 = No reduction
	short	xsize;				//  Hires XDim
	short	ysize;				//  Hires YDim
	short	xoff;					//  Hires origin
	short	yoff;					//  Hires origin
	short	x1;					//  Lores origin placement
	short	y1;					//  Lores origin placement
	short	size;					//  compressed size of cel
	short	checksum;			//  for block of raw videoData
	short	blockCount;
	short	0						//  Future expansion
	byte 	data[size]			//  videoData (see above)


AudioInit Block
	long	totalSampleCount;
	short	compressType;		//  None, LZ
	long	evenBlockSize;
	long	oddBlockSize;

	byte	evenBlock[evenBlockSize];
	byte	oddBlock[oddBlockSize];

#endif	

#else

#include "kernel.hpp"

void
KRobot(kArgs)
{
}

#endif	
