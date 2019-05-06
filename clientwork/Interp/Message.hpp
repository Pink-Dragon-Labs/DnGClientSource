// message.hpp

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

// BEW CLEANUP #ifndef SAVEABLE_HPP
// BEW CLEANUP #include "saveable.hpp"
// BEW CLEANUP #endif

struct MessageMgr {
};
extern MessageMgr messageMgr;

const int MessageMajorVersion	= 4000;

#pragma pack(1)
// VC FIX _Packed 
struct IndexEntry {
	uchar		noun;
	uchar		verb;
	uchar		cond;
	uchar		seq;
	uchar		talker;
	UInt16	offset;
	uchar		refNoun;
	uchar		refVerb;
	uchar		refCond;
	uchar		refSeq;
};

// VC FIX _Packed 
struct MsgData {
	UInt32		version;
	UInt16		length;			// size of file starting at count field, excluding
										//	comments
	UInt16		lastMsgNum;
	UInt16		nEntries;
	IndexEntry	entries[1];
};

#pragma pack()

#endif
