
/*
	The purpose of the Plane class is to
	define the 	characteristics of a plane

	***** NOTE ****   
	A plane cannot be created until after
	the global graphMgr is initialized!
*/

#ifndef PLANE_HPP
#define PLANE_HPP

#ifndef SILIST_HPP
#include "silist.hpp"
#endif

#ifndef DRAWLIST_HPP
#include "drawlist.hpp"
#endif

class Screen;
class RectList;
class PlaneList;

// The starting priority for 
// interperter generated planes
#define PLANEPRIORITYSTART 10000

class SOL_Plane
{
	public:

	// Plane types
	enum 
		{coloredPlane,		// Color in background that covers entire plane
		picturePlane,		// Picture in background that covers entire plane
		transparentPlane,	// See through background
		opaquePlane,		// Don't draw any background
		pictureSkipPlane	// Picture with skip color
		};

	//  Constructors, etc
	SOL_Plane(ObjectID);
	SOL_Plane(const SOL_Plane&);
	SOL_Plane(const SOL_Rect&);
	
	// Deconstructor
	~SOL_Plane(){}

	// Operators
	SOL_Plane& 			operator=(const SOL_Plane&);
	SOL_Plane& 			operator=(ObjectID&);

	//  Const methods
	int				AddCount() const			{return addCount;}
	uchar				Color() const				{return color;}
	const SOL_Rect& DefRect() const			{return plane;}
	int				DeleteCount() const		{return deleteCount;}
	int				EraseCount() const		{return eraseCount;}
	const	SOL_Rect&
						GameRect() const			{return gameRect;}
	SOL_Point		GetVanish() const			{return vanish;}
	long				Id() const					{return planeId;}
	int				PicResNum() const			{return picResNum;}
	int				PicMirror() const			{return picMirror;}
	int				Priority() const			{return priority;}
	int				PriorityCount() 			{return priorityCount;}
	int				RedrawAllCount() const	{return redrawAllCount;}
	const	ScreenItemList& 
						SIList() const				{return siArray;}
	int				UpdateCount() const		{return updateCount;}
	Bool				IsTransparent() const	{return (type == transparentPlane);}
	Bool				IsPictureSkip() const	{return (type == pictureSkipPlane);}
	Bool				IsColored() const			{return (type == coloredPlane);}
	const SOL_Rect&	OnScreen() const		{return onScreen;}
	int				TopScreenItem() const;

	//  Normal methods
	SOL_Rect&		OnScreen()					{return onScreen;}
	void				AbortScreenItem(const ScreenItem*);
	int				AddPic(int thePic, const SOL_Point&, int mirror, Bool delCopy = True);
	void				AddScreenItem(ScreenItem*);		//  cant be const
	void				CalcLists(SOL_Plane* last,
									 PlaneList& next,
									 DrawList& drawLists,
									 RectList& eraseLists);
	void				ConvGameRectToPlaneRect();
	void				DecAddCount()				{addCount--;}
	void				DecDeleteCount() 			{deleteCount--;}
	void				DecEraseCount()			{eraseCount--;}
	void				DecPriorityCount()		{priorityCount--;}
	void				DecUpdateCount()			{updateCount--;}
	void				DeletePic(int oldPic, int newPic);
	void				DeletePic(int picNumber);
	void				FilterDownEraseRects(DrawList& myDrawList,
													RectList& myEraseList,
													RectList& theirEraseList);
	void				FilterUpDrawRects(DrawList& myDrawList, DrawList& theirDrawList);
	void				FilterUpEraseRects(DrawList& myDrawList, RectList& theirEraseList);
	void				Move(const SOL_Point& ul);
	void				MoveDelta(int deltaX, int deltaY);
	void				MoveTo(SOL_Point ul);
	void				NewScreenDim(SOL_Rect);
	Bool				NowSeen(MemID, SOL_Rect& nowSeen);
	void				PackSiArray()				{siArray.Pack();}
	void				RedrawAll(SOL_Plane* last,
									 PlaneList& next,
									 DrawList& drawLists,
									 RectList& eraseLists);
	void				RemapMarkRedraw();
	void				ResizePlane(int xdim, int ydim);
	void				ScrollScreenItems(int xStep, int yStep, Bool movePic);
	void				SetAddCount(int val)		{addCount = val;}
	void				SetColor(uchar c)			{color = c;}
	void				SetDeleteCount(int val)	{deleteCount = val;}
	void				SetOnScreen(const SOL_Rect&);
	void				SetPicMirror(Bool f)		{picMirror = f;}
	void				SetPicNum(int num)		{picResNum = num;SetType();ChangePic();}
	void				SetPriority(int pri)		{priority = pri;}
	void				SetType();
	void				SetUpdateCount(int val)	{updateCount = val;}
	void				SetVanish(SOL_Point pt)	{vanish = pt;}
	ScreenItemList&
						SIList()						{return siArray;}
	void				UpdatePlane(SOL_Plane*);
	void           UpdateRedrawAllCount(); 
	void CheckIntegrity ( void );

	protected:

	static long 	nextId;			// Interpreter generated IDs

	// User defined properites
	unsigned int	planeId;			//  SCI plane object ID or nextId
	int				type;				//	 Colored, picture, transparent or opaque plane
	uchar 			color;			//  Background color if no pic associated
	SOL_Rect			gameRect;		//  User defined plane rectangle screen relative
	Bool				picMirror;		//  Mirror state for plane
	int				picResNum;		//  Picture number
	int				priority;		//  Plane priority
	SOL_Point		vanish;			//  Vanishing point (scale is local)


	//  All calculated values live here
	int				addCount;		//  Set when a plane is added
	Ratio				coordScaleX;
	Ratio				coordScaleY;
	int				deleteCount;   //  Set when a plane is deleted
	int				eraseCount;    //  Set when a plane is moved or resized
	SOL_Rect			onScreen;
	int				picResX;
	int				picResY;
	SOL_Rect			plane;			//  gameRect scaled to screen resolution
	int				priorityCount;	//  Set when a plane's priorty has changed
	int				redrawAllCount;//  Set when a specific update needs total redraw
	ScreenItemList siArray;			//  List of screen items for this plane
	int				updateCount;	//  Set when a plane is updated
	Bool				changePic;		//  The picResNum has changed

	//  Protected methods
	void	BreakDrawListByPlanes(DrawList& draw, PlaneList& next);
	void	BreakEraseListByPlanes(RectList& erase, PlaneList& next);
	void	ChangePic();
	void	DecSIArrayCounts(SOL_Plane* last, Bool forceUpdate = False);
	void	DeleteAllPics();
	void	GetPicResolution();
	void	MergeToEraseList(RectList& myEraseList, SOL_Rect&);
	void	MergeToDrawList(int i, SOL_Rect&, DrawList& myDrawList);
	void	UpdateTheirEraseList(SOL_Rect&, int i, RectList& theirEraseList);

};

// HOOK
// Never used
//void	GlobalToLocal(int planeID, SOL_Point&);
Bool	GlobalToLocalGame(int planeID, SOL_Point&);
Bool	LocalGameToGlobal(int planeID, SOL_Point&);
// HOOK
// Never used
//void	LocalToGlobal(int planeID, SOL_Point&);
void	MergeToRectList(const SOL_Rect&, RectList& myEraseList);

#endif


