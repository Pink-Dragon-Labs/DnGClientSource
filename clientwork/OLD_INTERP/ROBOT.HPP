#ifndef ROBOT_HPP
#define ROBOT_HPP

#ifndef SI_HPP
#include "si.hpp"
#endif

#ifndef RESTYPE_HPP
#include "restype.hpp"
#endif

enum {ROBOT_UNUSED, ROBOT_PLAYING, ROBOT_FINAL, ROBOT_PAUSED, ROBOT_KILLED};

const SiArraySize = 10;
const AudioListSize = 10;
const DelayArraySize = 10;
const CueArraySize = 256;
const CDSectorSize = 2048;
const FixedCelArraySize = 4;

const Unspecified = 50000;
const ZeroCompressCount = 2048;


class Robot
{
	public:
	Robot(int resNum, long plane, int pri = -1, int xoff = 0, int yoff = 0);
	~Robot();
	int	Status() const				{return status;}
	Bool	IsRobotRealTime() const;
	int	FrameNum() const			{return frameNum;}
	int	FrameCount() const		{return frameCount;}
	int	FramesSkipped() const	{return framesSkipped;}
	int	GetResNum() const			{return resNum;}
	int	MaxFramesSeqSkipped() const {return maxFramesSeqSkipped;}
	long	TickCount() const;

	long	GetCue();
	void	Init();
	void	DisplayFrame(int frame, int newxoff = Unspecified, int newyoff = Unspecified, int newpri = Unspecified);
	int	Info(SOL_Rect*);
	void	DoRobot();
	void	Pause();
	void	Resume();
	void	FrameAlmostVisible();
	void	FrameNowVisible();
	void	SetPriority(int newpri);

	#ifdef DEBUG
	Bool	Assert() const;
	#endif

	protected:
	void		KillRobot();
	Bool		SeekFrame(int frame);
	void		OffsetData(uchar*, char offset);
	void		CreateCel5(const uchar* video, int celCount, Bool usePalette);
	void		Create1Cel5(const uchar* video, int index, Bool usePalette);
	void		LZDecode(uchar* dest, const uchar* src, long srcCount);
	void		LZDecode(uchar* dest, const uchar* src, long srcCount, long destCount);
	void		ExpandData(uchar* dest, const uchar* src);
	Bool		ReadAudioDataFromRecord(int index, uchar* buffer, int* pAudPos, int* pAudCount);
	Bool		ReadPrimerData(uchar* evenBuffer, uchar* oddBuffer);
	Bool		ReadPartialAudioRecordAndSubmit(int audioStartFrame, int audioStartPos);
	Bool		InitAudio(int startTick);
	int		ToTicks(int frame);
	int		ToFrame(int ticks);
	void		SetRobotTime(int frame);
	int		CalcFrameNum(int extraTicks = 0);
	Bool		DoVersion5(Bool submitAudio = True);
	void		PreAllocCelMemory(const uchar* video, int celCount);

	ScreenItem*		robotSi[SiArraySize];
	int				priority[SiArraySize];
	int				fileX1[SiArraySize], fileY1[SiArraySize];
	int				x1[SiArraySize], y1[SiArraySize];
	int				siCount;
	int				evenPrimerSize, oddPrimerSize;
	int				totalPrimerSize;
	ushort			primerReservedSize;
	long				primerPos;
	ushort*			videoSizeArray;
	long*				recordPosArray;
	char*				lzScratch;
	void*				hunkPalette;
	int				audioSize;
	int				vPadSize[2];
	int				frameCount;
	int				frameNum;
	int				startingFrameNum;
	short				frameRate;			//  Frames per second
	int				normalFrameRate;
	int				lowFrameRate;
	int				highFrameRate;
	int				checkAudioSyncTime;

	short				hiresCoords;
	int				resNum;
	short				xRes, yRes;		//  MUST BE SHORT!
	Bool				syncFrame;
	Bool				hasAudio;
	long				planeId;
	int				fd;
	int				xoff, yoff;
	int				status;

	#ifdef DEBUG
	int				maxFrameSize;
	ushort*			videoSizeArrayCopy;
	long*				recordPosArrayCopy;
	int				predictedTime;
	#endif
	
	//	 *** NEW ***
	long				robotStartTime;
	long				fileDataStart;
	int				lastFrameNum;
	long*				cueTimeList;
	long*				masterCueTimeList;
	ushort*			cueValueList;
	short				version;
	int				framesSkipped;
	int				maxFramesSeqSkipped;
	int				cueForceShowFrame;
	int				robotStartFrame;
	int				prevFrame;
	int				firstAudioRecordStartPos;
	int				audioRecordInterval;
	Bool				adjustPerDAC;
	
	short				primerZeroCompressFlag;
	short				canSkip;
	short				maxCelsPerFrame;

	int				vertScaleFactor;
	int				xsize, ysize;
	long				maxCelArea[FixedCelArraySize];
	MemID				fixedCelHandle[FixedCelArraySize];
	MemID				squashedCelHandle;
	long				squashedCelArea;
	int				expectedAudCount;

	class CelHandleInfo
	{
		public:
		enum		{Invalid, FrameLifetime, RobotLifetime};

		CelHandleInfo()				{status = Invalid; area = 0;}
		~CelHandleInfo()				{}
		void		Free()				{id.Free(); status = Invalid; area = 0;}

		MemID		id;
		int		status;
		long		area;
	};

	CelHandleInfo*	celHandleArray;
	int				celHandleArraySize;

	class AudioList
	{
		public:
		enum  {START_ENUM, AUDIO_READYTOSTART, AUDIO_STOPPED, AUDIO_PLAYING, AUDIO_PAUSED, AUDIO_STOPPING, END_ENUM};
		AudioList();
		~AudioList();
		Bool		Assert() const;
		int		AudioStartOffset() const	{return audioStartOffset;}

		void		AddBlock(int pos, int size, const uchar* buffer);
		void		SubmitDriverMax();
		void		PrepareForPrimer();
		void		StartAudioNow();
		void		StopAudio();
		void		StopAudioNow();
		void		SetAudioOffset(int offset);
		void		Reset();

		protected:
		class AudioBlock
		{
			public:
			AudioBlock(int pos, int size);
			~AudioBlock();
			Bool		Assert() const;
			Bool		Submit(int audioStartOffset);
			uchar*	operator*();


			int		pos;
			int		size;
			protected:
			MemID		memid;
		};

		protected:
		void		FreeAudioBlocks();

		AudioBlock* array[AudioListSize];
		int			oldestBlockIndex;
		int			newestBlockIndex;
		int			blockCount;
		int			audioStartOffset;
		int			audioState;
	};

	AudioList	audioList;



	class DelayTime
	{
		public:
		DelayTime();
		Bool	TimingInProgress() const;
		void	StartTiming();
		int	EndTiming();
		long	PredictedTicks() const;
		void	Dump() const;

		protected:
		void	SortList();
		void	AddToList(ulong);

		long		delays[DelayArraySize], timeStamp[DelayArraySize];
		long		earliestTimeStamp;
		long		latestTimeStamp;
		long		startTime;
	};

	DelayTime	delayTime;



	private:
	Bool			Open(int* fd, ResNum);
};

#endif
