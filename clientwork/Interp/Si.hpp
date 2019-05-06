/*
		The purpose of the ScreenItem class is to hold all the
		information needed to represent one instance of a cel
		or line on a plane.

		When a ScreenItem is created, a plane is needed to help
		calculate the scaling and positioning.	Also, the resource 
		manager must be consulted to find out what the dimensions,
		resolution and hotspot is for a cel.
*/

#ifndef SI_HPP
#define SI_HPP

#ifndef RECT_HPP
#include "rect.hpp"
#endif

#ifndef RATIO_HPP
#include "ratio.hpp"
#endif

#ifndef SCALE_HPP
#include "scale.hpp"
#endif

#ifndef CELOBJ_HPP
#include "celobj.hpp"
#endif

class ObjectID;
class SOL_Plane;
class CelObj;

class ScreenItem
{
	enum {tRect, tLine};

	public:

	//  Constructors, etc
	ScreenItem(const ScreenItem*);
	ScreenItem(const ObjectID&);
	ScreenItem(long plane, const Bitmap&);
	ScreenItem(long plane, const Bitmap&, const SOL_Rect&);
	ScreenItem(long plane, const Bitmap&, SOL_Point, ScaleInfo);
	ScreenItem(long plane, const Bitmap&, SOL_Point, ScaleInfo, int);
	~ScreenItem();
	ScreenItem& operator=(const ObjectID&);
	ScreenItem& operator=(const ScreenItem&);

	// Given
	SOL_Point	pos;				// User specified X, Y coordinates
	Coord			z;					// User specified Z coordinate
	Bitmap		bitmap;			// User bitmap for this object
	ScaleInfo	scale;			// User scaling information
	uchar 		color;			// User color for color celobjs

	//  Const Methods
	Bool			FixedPri() const			{return fixedPriFlag;}
	long			Id() const					{return id;}
	int			PicResNum() const			{return picResNum;}
	long			PlaneId() const			{return planeId;}
	int			Priority() const			{return priority;}
	Ratio			ScaleX() const				{return finalScaleX;}
	Ratio			ScaleY() const				{return finalScaleY;}
	Bool			UserMirror() const		{return userMirror;}
	int			X1() const					{return x1;}
	int			Y1() const					{return y1;}
	int			UpdateCount() const		{return updateCount;}
	int			DeleteCount() const		{return deleteCount;}
	int			AddCount() const			{return addCount;}
	const SOL_Rect& 	OnScreen() const	{return onScreen;}

	//  Normal methods
	SOL_Rect&	InsetRect()					{return insetRect;}
	void			NowSeen(const SOL_Plane&, SOL_Rect& nowSeen);
	void			ClearDeleteCount()		{deleteCount = 0;}
	void			ClearInsetRect()			{useInsetRect = False;}
	void			DecUpdateCount()			{updateCount--;}
	void			DecAddCount()				{addCount--;}
	void			DecDeleteCount()			{deleteCount--;}
	void			Draw(const Buffer& dest, const SOL_Rect& drawRect)
						{cel->Draw(dest,*this,drawRect,userMirror^cel->Mirrored());}
	void 			DoUpdate(const SOL_Plane& plane)	{GetCelObj();Update(plane);}
	void 			GetCelObj();
	uchar			GetSkip()					{GetCelObj();return cel->Skip();}
	CelObj*		GetCel()						{return cel;}
	SOL_Rect& 	OnScreen() 					{return onScreen;}
	uchar			ReadPix(int x, int y)			
						{GetCelObj(); return cel->ReadPix(x, y, userMirror ^ cel->Mirrored());}
	int			RemapState()  				{GetCelObj(); return cel->RemapState();}
	void			SetCel(CelObj* theCel)  {delete cel; cel = theCel;}
	void			SetUpdateCount(int val)	{updateCount = val;}
	void			SetAddCount(int val)		{addCount = val;}
	void			SetDeleteCount(int val)	{deleteCount = val;}
	void			SetInsetRect(const SOL_Rect& r)		{insetRect = r; useInsetRect = True;}
	void			SetOnScreen(const SOL_Rect& rect)	{onScreen = rect;}
	void			SetPicResNum(int pic)	{picResNum = pic;}
	void			SetPriority(int p)		{priority = p; fixedPriFlag = True;}
	void			SetFloatPriority()		{fixedPriFlag = False;}
	void			SetUserMirror(Bool f)	{userMirror = f;}
	void			SubmitPalette()			{GetCelObj(); cel->SubmitPalette();}

	protected:
	static long nextId;			// For internal screen items and SCI lines
	long			id;				// The object's ID
	int			picResNum;		// Picture this si was derived from
	unsigned int planeId;			// The plane's ID  for this object
	int			priority;  		// User specified priority relative to it's plane
	Bool			fixedPriFlag;	// User specified priority
	int			updateCount;
	int			addCount;
	int			deleteCount;
	CelObj*		cel;
	Bool			useInsetRect;
	Bool			userMirror;

	// Calculated
	int			x1, y1;			// Upper left coordinate (Scaled)(Screen relative)
	SOL_Rect		onScreen;		//	Scaled (Screen relative)
	Ratio			finalScaleX;
	Ratio			finalScaleY;
	SOL_Rect		insetRect;

	void			Update(const SOL_Plane&);
	// Do not define
	ScreenItem(const ScreenItem&);
};

#endif

