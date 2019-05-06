//	restype.hpp

#ifndef RESTYPE_HPP
#define RESTYPE_HPP

#ifndef DOS_HPP
#include "dos.hpp"
#endif

#ifndef MEMTYPE_HPP
#include "memtype.hpp"
#endif

#ifndef MEMID_HPP
#include "memid.hpp"
#endif

typedef ushort ResNum;

#pragma pack(1)
// VC FIX 	_Packed 
	struct ResTag {
		ResNum	resNum;
		MemID		id;
	};
#pragma pack()

class SOL_ResType {
public:
	SOL_ResType(char* extension = "", char* label = "ResType");

	virtual Bool			Check(ResNum);
	virtual int				Close(int) const;
	virtual const char*	GetExtension() const 	{ return ext; }
	virtual Bool			IsSupported() const		{ return *ext != 0; }
	virtual void			Load(ResTag&,Bool lock);
	virtual void			MakeName(char* dest, char* fileSpec, ResNum) const;
	virtual void  			MakeWildName(char* dest, char* fileSpec) const;
	virtual int				Open(ResNum, char* path = 0);
	int				CanOpen(ResNum, char* path = 0);
	virtual int				FindDirEntry(ResNum, int*) const;
	virtual int				FindPatchEntry(ResNum) const;
	virtual Bool			CheckPatches(ResNum) const;
	virtual MemType		GetType() const 			{ return MemResNone; }
	virtual MemAttrs		GetAttrs() const
								{ return DISCARDABLE | MOVEABLE | CHECKSUMMABLE; }

protected:
	virtual void			ConfirmType(MemType) const;
	virtual void			Decompress(ResTag&) const;
	virtual int				FindPatch36Entry(char*) const;
	virtual long			GetFileSize() const;
	virtual MemType		GetFileType() const 		{ return GetType(); }
	virtual void			MakeName(char* dest, char* fileSpec, char* rootName) const;
	virtual int				Read(void* dest, int num) const;
	virtual int				SeekToData() const;

	char			ext[MaxExt + 1];
	char*			label;

	static int	fd;
	static int	resLength;
	static int	compressedLength;
	static int	resOffset;
};

class	ResView : public SOL_ResType {
public:
	ResView() : SOL_ResType(".v56", "ResView")  {}
	MemType	GetType() const {return MemResView;}
protected:
	int		SeekToData() const;
};

class	ResPic : public SOL_ResType {
public:
	ResPic() : SOL_ResType(".p56", "ResPic")  {}
	MemType GetType() const {return MemResPic;}
protected:
	int		SeekToData() const;
};

struct ResChunk : SOL_ResType {
	ResChunk() : SOL_ResType(".chk", "ResChunk")  {}
#ifdef MACINTOSH
	MemType GetType() const			{return (MemType) 0;} //KGN MemResChunk;}
#else
	MemType GetType() const			{return MemResChunk;}
#endif
};

struct ResScript : SOL_ResType {
	ResScript() : SOL_ResType(".scr", "ResHunk")  {}
	MemType GetType() const			{return MemResHunk;}
};

struct ResSound : SOL_ResType {
	ResSound() : SOL_ResType(".snd", "ResSound")  {}
	MemType GetType() const 	{return MemResSound;}
};

struct ResVocab : SOL_ResType {
	ResVocab() : SOL_ResType(".voc", "ResVocab")  {}
	MemType GetType() const		{ return MemResVocab; }
};

struct ResPatch : SOL_ResType {
	ResPatch() : SOL_ResType(".pat", "ResPatch")  {}
	MemType GetType() const		{ return MemResPatch; }
};

struct ResFont : SOL_ResType {
	ResFont() : SOL_ResType(".fon", "ResFont")  {}
	MemType GetType() const		{ return MemResFont; }
};

class	ResPal : public SOL_ResType {
public:
	ResPal() : SOL_ResType(".pal", "ResPal")  {}
	MemType	GetType() const		{ return MemResPalette; }
protected:
	int		SeekToData() const;
};

struct ResHeap : SOL_ResType {
	ResHeap() : SOL_ResType(".hep", "ResHeap")  {}
	MemType GetType() const 		{ return MemResHeap; }
};

struct ResMsg : SOL_ResType {
	ResMsg() : SOL_ResType(".msg", "ResMsg")  {}
	MemType GetType() const 		{ return MemResMsg; }
};

struct ResAudio : SOL_ResType {
	ResAudio() : SOL_ResType(".aud", "ResAudio")  {}
	MemType GetType() const 		{ return MemResAudio; }
protected:
	int		SeekToData() const;
};

struct ResWave : SOL_ResType {
	ResWave() : SOL_ResType(".wav", "ResWave")  {}
	MemType GetType() const 		{ return MemResWAVE; }
protected:
	int		SeekToData() const;
};

struct ResAudio36 : SOL_ResType {
	ResAudio36() : SOL_ResType("", "ResAudio36")  {}
	MemType GetType() const 		{ return MemResAudio; }
protected:
	int		SeekToData() const;
};

struct ResSync : SOL_ResType {
	ResSync() : SOL_ResType(".syn", "ResSync")  {}
	MemType GetType() const 		{ return MemResSync; }
};

struct ResSync36 : SOL_ResType {
	ResSync36() : SOL_ResType("", "ResSync36")  {}
	MemType GetType() const 		{ return MemResSync; }
};

struct ResMap : SOL_ResType {
	ResMap() : SOL_ResType(".map", "ResMap")  {}
	MemType GetType() const 		{ return MemResMap; }
};

struct ResMsgTranslation : SOL_ResType {
	ResMsgTranslation() : SOL_ResType(".trn", "ResMsgTranslation")  {}
	
	//	a translation resource file is just like a message file, but
	//	in memory, the two types need to be distinguished
	MemType GetType() const 			{ return MemResMsgTranslation; }
	MemType GetFileType() const 		{ return MemResMsg; }
};

struct ResRobot : SOL_ResType {
	ResRobot() : SOL_ResType(".rbt", "ResRobot")  {}
	MemType GetType() const 			{ return MemResRobot; }
};

class	ResVMD : public SOL_ResType {
public:
	ResVMD() : SOL_ResType(".vmd", "ResVMD")  {}
	MemType	GetType() const		{ return MemResVMD; }
protected:
	int		SeekToData() const;
};

#endif  //RESTYPE_HPP
