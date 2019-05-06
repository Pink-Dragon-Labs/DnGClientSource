#include "sol.hpp"

#ifndef MACINTOSH
#include <malloc.h>
#endif

#include <stdio.h>

#include "si.hpp"
#include "plane.hpp"
#include "celobj.hpp"
#include "graphmgr.hpp"
#include "msg.hpp"
// BEW CLEANUP #include "mono.hpp"
#include "object.hpp"
#include "selector.hpp"
#include "resource.hpp"

// Internally created screenItems and SCI
// created lines get their ID from nextId
long ScreenItem::nextId = 100000;


//  Destructor
//  ==========
ScreenItem::~ScreenItem()
{
	SetCel(NULL);
}


//  Constructors
//  ============

ScreenItem::ScreenItem(const ScreenItem* si)
{
	// Screen item created for adding
	// to the last screen when a screen
	// item is added to the next screen
	id = si->id;
	planeId = si->planeId;
	cel = NULL;

// HOOK
//	pos = si->pos;
//	z = si->z;
//	priority = si->priority;
	bitmap = si->bitmap;
	onScreen = si->onScreen;

	// Needed for IsOnMe() checks
	userMirror = si->userMirror;
	useInsetRect = si->useInsetRect;
	if (useInsetRect)
		insetRect = si->insetRect;
	scale	= si->scale;
	x1 = si->x1;
	y1 = si->y1;
}

ScreenItem::ScreenItem(long plane, const Bitmap& theBitmap)
{
	// This constructor is used for pictures
	id = nextId++;
	pos.x = 0;
	pos.y = 0;
	z = 0;
	planeId = plane;
	bitmap = theBitmap;
	cel = NULL;
	useInsetRect = False;
	fixedPriFlag = False;
	userMirror = False;
	picResNum = -1;
	updateCount = 0;
	addCount = graphMgr->ScreenCount();
	deleteCount = 0;
}

ScreenItem::ScreenItem(long plane, const Bitmap& theBitmap, const SOL_Rect& r)
{
	id = nextId++;
	pos.x = r.A.x;
	pos.y = r.A.y;
	z = 0;
	planeId = plane;
	bitmap = theBitmap;
	cel = NULL;
	fixedPriFlag = False;
	userMirror = False;
	picResNum = -1;
	updateCount = 0;
	addCount = graphMgr->ScreenCount();
	deleteCount = 0;
	useInsetRect = False;
	if (bitmap.IsColor())
		// Color screenitem
		insetRect = r;
}

ScreenItem::ScreenItem(long plane, const Bitmap& theBitmap, SOL_Point p, ScaleInfo scaleInfo)
{
	id = nextId++;
	pos = p;
	z = 0;
	planeId = plane;
	bitmap = theBitmap;
	cel = NULL;
	scale = scaleInfo;
	useInsetRect = False;
	fixedPriFlag = False;
	userMirror = False;
	picResNum = -1;
	updateCount = 0;
	addCount = graphMgr->ScreenCount();
	deleteCount = 0;
}

ScreenItem::ScreenItem(long plane, const Bitmap& theBitmap, SOL_Point p, ScaleInfo scaleInfo, int theID)
{
	id = theID;
	pos = p;
	z = 0;
	planeId = plane;
	bitmap = theBitmap;
	cel = NULL;
	scale = scaleInfo;
	useInsetRect = False;
	fixedPriFlag = False;
	userMirror = False;
	picResNum = -1;
	updateCount = 0;
	addCount = graphMgr->ScreenCount();
	deleteCount = 0;
}

ScreenItem::ScreenItem(const ObjectID& theId)
{
	// This assignment operator is used for creating
	// a ScreenItem from an SCI screen object.

	id = theId;

	// Set critical properties
	pos.x = (int)(signed short)theId.GetIndexedProperty(actX);
	pos.y = (int)(signed short)theId.GetIndexedProperty(actY);

	scale.scaleX = theId.GetIndexedProperty(actScaleX);
	scale.scaleY = theId.GetIndexedProperty(actScaleY);
	scale.maxScale = theId.GetIndexedProperty(actMaxScale);
	scale.scaleType = theId.GetIndexedProperty(actScaleSignal) & 3;

	uint newView = theId.GetIndexedProperty(actView);
	uint newLoop = theId.GetIndexedProperty(actLoop);
	uint newCel = theId.GetIndexedProperty(actCel);

   if (newView < (uint) -2)
   {
	   // Load the view into memory
		MemID viewID;
	   viewID = resMgr->Get(MemResView, newView);

	   ViewHeader* viewPtr = (ViewHeader*) *viewID;

	   if (viewPtr->loopCount <= newLoop)
	   {
         newLoop = viewPtr->loopCount - 1;
			theId.SetIndexedProperty(actLoop, newLoop);
      }

	   LoopHeader* loopPtr = (LoopHeader*) ((char*) viewPtr + viewPtr->loopHeaderSize * newLoop + viewPtr->viewHeaderSize + 2); 
      if (loopPtr->altLoop != -1)       
 		   loopPtr = (LoopHeader*) ((char*) viewPtr + viewPtr->loopHeaderSize*loopPtr->altLoop + viewPtr->viewHeaderSize + 2);

	   if (loopPtr->numCels <= newCel)
	   {
         newCel = loopPtr->numCels - 1;
			theId.SetIndexedProperty(actCel, newCel);
      }
   }    
   bitmap = Bitmap(newView, newLoop, newCel);
	
   // If there is a font bitmap use it
	MemID FBM = theId.GetIndexedProperty(actBitmap);
   if (FBM)
   {
      bitmap.SetMemID(FBM);
      bitmap.SetType(Bitmap::tMem);
   }

	planeId = (unsigned int)((unsigned short)theId.GetIndexedProperty(actPlaneID));

	if (theId.GetIndexedProperty(actFixedPri))
	{
		priority = theId.GetIndexedProperty(actPri);
		fixedPriFlag = True;
	}
	else
	{
		fixedPriFlag = False;
		theId.SetIndexedProperty(actPri, pos.y);
	}

	//  Adjust for Z property
	z = theId.GetIndexedProperty(actZ);
	pos.y -= z;

	ObjectID rectID;

	if (rectID = theId.GetIndexedProperty(actSeenRect)) {
		insetRect.A.x =	rectID.GetIndexedProperty(actLeft);
		insetRect.A.y =	rectID.GetIndexedProperty(actTop);
		insetRect.B.x =	rectID.GetIndexedProperty(actRight);
		insetRect.B.y =	rectID.GetIndexedProperty(actBottom);
		useInsetRect = True;
	} else {
		useInsetRect = False;
	}

	userMirror = False;
	picResNum = -1;

	theId->ClearGraphicsUpdate();

	cel = NULL;

	addCount = graphMgr->ScreenCount();
	deleteCount = 0;
	updateCount = 0;
}

//  Assignment operators
//  ===================

ScreenItem&
ScreenItem::operator=(const ObjectID& theId)
{
	// This assignment operator is used for updating
	// a ScreenItem from an SCI screen object.

	// Set critical properties
	pos.x = (int)(signed short)theId.GetIndexedProperty(actX);
	pos.y = (int)(signed short)theId.GetIndexedProperty(actY);

	scale.scaleX = theId.GetIndexedProperty(actScaleX);
	scale.scaleY = theId.GetIndexedProperty(actScaleY);
	scale.maxScale = theId.GetIndexedProperty(actMaxScale);
	scale.scaleType = theId.GetIndexedProperty(actScaleSignal) & 3;

	uint newView = theId.GetIndexedProperty(actView);
	uint newLoop = theId.GetIndexedProperty(actLoop);
	uint newCel = theId.GetIndexedProperty(actCel);

	if ((newView != bitmap.View())	||
		 (newLoop != bitmap.Loop())	||
		 (newCel != bitmap.Cel()))
	{

   	if (newView < (uint) -2)
   	{
	   	// Load the view into memory
	   	MemID viewID = resMgr->Get(MemResView, newView);
	   	ViewHeader* viewPtr = (ViewHeader*) *viewID;

	   	if (viewPtr->loopCount <= newLoop)
	   	{
         	newLoop = viewPtr->loopCount - 1;
				theId.SetIndexedProperty(actLoop, newLoop);
      	}

	   	LoopHeader* loopPtr = (LoopHeader*) ((char*) viewPtr + viewPtr->loopHeaderSize * newLoop + viewPtr->viewHeaderSize + 2); 
      	if (loopPtr->altLoop != -1)       
 		   	loopPtr = (LoopHeader*) ((char*) viewPtr + viewPtr->loopHeaderSize*loopPtr->altLoop + viewPtr->viewHeaderSize + 2);

	   	if (loopPtr->numCels <= newCel)
	   	{
         	newCel = loopPtr->numCels - 1;
				theId.SetIndexedProperty(actCel, newCel);
      	}
   	}    
   	bitmap = Bitmap(newView,newLoop,newCel);
		SetCel(NULL);
	}
	
   // If there is a font bitmap use it
	MemID FBM = theId.GetIndexedProperty(actBitmap);
   if (FBM)
   {
//		msgMgr->Mono ( "SI::ObjectID (setting bitmap to 0x%x)\n", (SOL_Handle)FBM );
      bitmap.SetMemID(FBM);
      bitmap.SetType(Bitmap::tMem);
		SetCel(NULL);
   }

	if (theId.GetIndexedProperty(actFixedPri))
	{
		priority = theId.GetIndexedProperty(actPri);
		fixedPriFlag = True;
	}
	else
	{
		fixedPriFlag = False;
		theId.SetIndexedProperty(actPri, pos.y);
	}

	//  Adjust for Z property
	z = theId.GetIndexedProperty(actZ);
	pos.y -= z;

#ifdef NETWORK
	ObjectID rectID;

	if (rectID = theId.GetIndexedProperty(actSeenRect))
	{
		insetRect.A.x =	rectID.GetIndexedProperty(actLeft);
		insetRect.A.y =	rectID.GetIndexedProperty(actTop);
		insetRect.B.x =	rectID.GetIndexedProperty(actRight);
		insetRect.B.y =	rectID.GetIndexedProperty(actBottom);
		useInsetRect = True;
	}
	else
	{
		useInsetRect = False;
	}
#else
	if (theId.GetIndexedProperty(actUseInsetRect))
	{
		insetRect.A.x =	theId.GetIndexedProperty(actInsetRectL);
		insetRect.A.y =	theId.GetIndexedProperty(actInsetRectT);
		insetRect.B.x =	theId.GetIndexedProperty(actInsetRectR);
		insetRect.B.y =	theId.GetIndexedProperty(actInsetRectB);
		useInsetRect = True;
	}
	else
	{
		useInsetRect = False;
	}
#endif

	userMirror = False;
	picResNum = -1;

	theId->ClearGraphicsUpdate();

	if (!addCount)
		updateCount = graphMgr->ScreenCount();
	deleteCount = 0;

	return *this;
}

ScreenItem&
ScreenItem::operator=(const ScreenItem& si)
{
	// Used for Updating a screen item in the last screen

// HOOK
//	pos = si.pos;
//	z = si.z;
//	priority = si.priority;
	bitmap = si.bitmap;
	onScreen = si.onScreen;

	// Needed for IsOnMe() checks
	userMirror = si.userMirror;
	useInsetRect = si.useInsetRect;
	if (useInsetRect)
		insetRect = si.insetRect;
	scale	= si.scale;
	x1 = si.x1;
	y1 = si.y1;
	return *this;
}

/*******************************************************************************


  insetRect and useInsetRect are parameters that come from SCI (or magnify) only
  and need to be validated.  The insetRect is NOT scaled.

 *******************************************************************************/

void
ScreenItem::Update(const SOL_Plane& plane)
{
	GetCelObj();

	#ifdef DEBUG
	assert(graphMgr != NULL);
	assert(cel != NULL);
//	assert(graphMgr->Xdim() == cel->ResX());
//	assert(graphMgr->Ydim() == cel->ResY());
//	assert(graphMgr->Xdim() == SCIRESX);
//	assert(graphMgr->Ydim() == SCIRESY);
//	if (useInsetRect)
//		assert(insetRect.IsWithin(cel->Width(), cel->Height()));
	#endif

	if (useInsetRect)
		insetRect.Clip(SOL_Rect(0, 0, cel->Width() - 1, cel->Height() - 1));


	// Calculate the scaling for either a vanishing point or manual scaling
	Ratio userScaleX;
	Ratio userScaleY;
	switch (scale.scaleType)
	{
		case SCALE_MANUAL:
//			assert(scale.scaleX >= 0);
//			assert(scale.scaleY >= 0);
			userScaleX = Ratio(scale.scaleX, 128);
			userScaleY = Ratio(scale.scaleY, 128);
			break;

		case SCALE_VANISHING:
		{
			SOL_Point vanish;
			vanish = plane.GetVanish();
			assert(scale.maxScale > 0);
			userScaleX = Ratio((scale.maxScale * (pos.y - vanish.y)) / (SCIRESY - vanish.y), 128);
			userScaleY = userScaleX;
			break;
		}

		case SCALE_OFF:
			break;

		default:
			assert(!"Unknown scale type!");
			break;
	}
	
	// If the screen item is scaled into non existance,  it won't get added to the draw list
	if (userScaleX.numer != 0  &&  userScaleY.numer != 0)
	{
		SOL_Rect	toDraw;
		if (useInsetRect)
			toDraw = insetRect;
		else
			toDraw.Set(0, 0, cel->Width() - 1, cel->Height() - 1);

		//  Flip origin if mirroring a non-pic Cel
		int xorg = cel->XOrg();
		int yorg = cel->YOrg();

//		if (!bitmap.IsPic()  &&  (userMirror ^ cel->Mirrored()))
//			xorg = cel->Width() - xorg - 1;

		//
		//  Multiply toDraw, origin by user scale
		//

		if (!userScaleX.IsOne()  ||  !userScaleY.IsOne())
		{
			toDraw.A.x = toDraw.A.x ^ userScaleX;
			toDraw.A.y = toDraw.A.y ^ userScaleY;

			if (userScaleX.IsLargerThanOne())
				toDraw.B.x = ((toDraw.B.x + 1) ^ userScaleX) - 1;
			else
				toDraw.B.x = toDraw.B.x ^ userScaleX;

			if (userScaleY.IsLargerThanOne())
				toDraw.B.y = ((toDraw.B.y + 1) ^ userScaleY) - 1;
			else
				toDraw.B.y = toDraw.B.y ^ userScaleY;

			assert(toDraw.B.x >= toDraw.A.x);
			assert(toDraw.B.y >= toDraw.A.y);

			xorg = xorg ^ userScaleX;
			yorg = yorg ^ userScaleY;
		}

		//  Use origin to determine cels upper left corner offset
	  	x1 = pos.x - xorg;
	  	y1 = pos.y - yorg;
		toDraw.Offset(x1, y1);

		//  If mirroring a pic, adjust x1, toDraw
		if (bitmap.IsPic()  &&  (userMirror ^ cel->Mirrored()))
		{
			assert(userScaleX.IsOne());
			assert(userScaleY.IsOne());

			int x = ((CelObjPic*) cel)->Pos().x;
			SOL_Rect draw = toDraw;
			draw.Offset(x - xorg, 0);
			int newx1 = plane.DefRect().Xlen() - draw.B.x - 1;
			int offx = newx1 - draw.A.x;

			x1 += offx;
			toDraw.Offset(offx, 0);
		}

		//  Make x1, y1, toDraw screen relative
		int planeX1 = plane.DefRect().A.x;
		int planeY1 = plane.DefRect().A.y;
		x1 += planeX1;
		y1 += planeY1;
		toDraw.Offset(planeX1, planeY1);

		//  Calculate scaling factors for CelObjDraw routines
		finalScaleX = userScaleX;
		finalScaleX.Normalize();

		finalScaleY = userScaleY;
		finalScaleY.Normalize();

		//  Clip the onScreen rectangle by the plane 
		//  The plane has been clipped by the screen
		assert(toDraw.IsValid());
		assert(plane.OnScreen().IsWithin(graphMgr->Xdim(), graphMgr->Ydim()));

		onScreen = toDraw;
		onScreen.Clip(plane.OnScreen());

		if (!fixedPriFlag)
			priority = pos.y + z;
	}
	else
		onScreen.MakeEmpty();
}


void	
ScreenItem::NowSeen(const SOL_Plane& plane, SOL_Rect& nowSeen)
{
	GetCelObj();

	#ifdef DEBUG
	assert(cel != NULL);
//	assert(graphMgr->Xdim() == cel->ResX());
//	assert(graphMgr->Ydim() == cel->ResY());
//	assert(graphMgr->Xdim() == SCIRESX);
//	assert(graphMgr->Ydim() == SCIRESY);
//	if (useInsetRect)
//		assert(insetRect.IsWithin(cel->Width(), cel->Height()));
	#endif


	SOL_Rect inset(0, 0, cel->Width() - 1, cel->Height() - 1);
	if (useInsetRect)
	{
		inset = insetRect;
		inset.Clip(SOL_Rect(0, 0, cel->Width() - 1, cel->Height() - 1));
	}

	// Calculate the scaling for either a vanishing point or manual scaling
	Ratio userScaleX;
	Ratio userScaleY;
	switch (scale.scaleType)
	{
		case SCALE_MANUAL:
			assert(scale.scaleX >= 0);
			assert(scale.scaleY >= 0);
			userScaleX = Ratio(scale.scaleX, 128);
			userScaleY = Ratio(scale.scaleY, 128);
			break;

		case SCALE_VANISHING:
		{
			SOL_Point vanish = plane.GetVanish();
			assert(scale.maxScale > 0);
			userScaleX = Ratio((scale.maxScale * (pos.y - vanish.y)) / (SCIRESY - vanish.y), 128);
			userScaleY = userScaleX;
			break;
		}

		case SCALE_OFF:
			break;

		default:
			assert(!"Unknown scale type!");
			break;
	}
	
	// If the screen item is scaled into non existance
	// make sure the nowseen rectangle is empty
	if (userScaleX.numer != 0  &&  userScaleY.numer != 0)
	{
		//  Flip origin if mirroring a non-pic Cel
		nowSeen = inset;
		int xorg = cel->XOrg();
		int yorg = cel->YOrg();

//		if ((userMirror ^ cel->Mirrored())  &&  !bitmap.IsPic())
//			xorg = cel->Width() - xorg - 1;

		//
		//  Multiply everything by user scale
		//

		if (!userScaleX.IsOne()  ||  !userScaleY.IsOne())
		{
			nowSeen.A.x = nowSeen.A.x ^ userScaleX;
			nowSeen.A.y = nowSeen.A.y ^ userScaleY;

			if (userScaleX.IsLargerThanOne())
				nowSeen.B.x = ((nowSeen.B.x + 1) ^ userScaleX) - 1;
			else
				nowSeen.B.x = nowSeen.B.x ^ userScaleX;

			if (userScaleY.IsLargerThanOne())
				nowSeen.B.y = ((nowSeen.B.y + 1) ^ userScaleY) - 1;
			else
				nowSeen.B.y = nowSeen.B.y ^ userScaleY;

			assert(nowSeen.B.x >= nowSeen.A.x);
			assert(nowSeen.B.y >= nowSeen.A.y);

			xorg = xorg ^ userScaleX;
			yorg = yorg ^ userScaleY;
		}

		//  Make x, y upper left of cel
		int x = pos.x - xorg;
		int y = pos.y - yorg;
		nowSeen.Offset(x, y);
	}
	else
		nowSeen.MakeEmpty();
}

void
ScreenItem::GetCelObj()
{
	if (cel == NULL)
	{
		switch (bitmap.Type())
		{
			case Bitmap::tView:
				cel = New CelObjView(bitmap.View(),bitmap.Loop(),bitmap.Cel());
				break;

			case Bitmap::tPic:
				msgMgr->Fatal(SrcLoc,"Internal error, pic screen item with no cel.");
				break;

			case Bitmap::tMem:
				cel = New CelObjMem(bitmap.Handle());
				break;

			case Bitmap::tColor:
				cel = New CelObjColor(bitmap.Color(),insetRect.Xlen(),insetRect.Ylen());
				break;

			default:
				assert(!"ScreenItem::GetCelObj - Unknown bitmap type!");
		}
	}
}

