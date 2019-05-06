#ifndef CELASM_HPP
#define CELASM_HPP

extern "C" uchar*	colorptr;
extern "C" uchar*	destptr;
extern "C" uchar*	controlptr;
extern "C" int*	scaleptr;
extern "C" int		pixelcount;
extern "C" int		beginindex;
extern "C" uchar	skipval;

#ifndef MACINTOSH
extern "C"  {
#endif
	void		WriteUncompLine();
	void		WriteUncompLineBackNoSkip();
	void		WriteUncompLineNoSkip();
	void		WriteUncompLineBack();
	void		WriteUncompScaleLine();
	void		WriteUncompScaleLineNoSkip();
	void		WriteUncompScaleLine2();
	void		WriteUncompScaleLine2Mirr();
	void		WriteCompSkipLine(int, int, int, uchar*, uchar*, long*, long*);
	void		WriteCompLine(int, int, int, uchar*, uchar*, long*, long*);
//	void		WriteCompSkipLine();
	void		WriteCompSkipLineBack(int, int, int, uchar*, uchar*, long*, long*);
	void		WriteCompLineBack(int, int, int, uchar*, uchar*, long*, long*);
//	void		WriteCompSkipLineBack();
	void		WriteWholeCompSkipLine();
#ifndef MACINTOSH
};
#endif

#endif
