#include "sol.hpp"

#include "graphmgr.hpp"
#include "kernel.hpp"
#include "msg.hpp"
#include "object.hpp"
#include "plane.hpp"
#include "plist.hpp"
#include "pmachine.hpp"
#include "rectlist.hpp"
#include "selector.hpp"
#ifdef WIN32S
#include "shiftpal.hpp"
#endif
#include "si.hpp"
#include "graph.hpp"

char *gPlaneFatalStr = "trails";

long SOL_Plane::nextId = 100000;

inline void
TranslateMacColor(uchar& color)
{
#ifdef MACINTOSH
	if (color == SOL_BLACK)
		color = SOL_WHITE;
	else if (color == SOL_WHITE)
		color = SOL_BLACK;
#else
	color = color;
#endif
}

//
// Constructors
//
SOL_Plane::SOL_Plane(ObjectID ID):
	vanish(ID.GetProperty(s_vanishingX),ID.GetProperty(s_vanishingY))
{
	// Plane created from an SCI plane object
	SOL_Rect rect;

	planeId = ID;

	int xdim = graphMgr->Xdim();
	int ydim = graphMgr->Ydim();

#ifdef NETWORK
	gameRect.A.x = ID.GetProperty(s_left);
	gameRect.A.y = ID.GetProperty(s_top);
	gameRect.B.x = ID.GetProperty(s_right); // + 1) * xdim) / SCIRESX) - 1;
	gameRect.B.y = ID.GetProperty(s_bottom); // + 1) * ydim) / SCIRESY) - 1;
#else
	gameRect.A.x = ID.GetProperty(s_insetRectL);
	gameRect.A.y = ID.GetProperty(s_insetRectT);
	gameRect.B.x = ID.GetProperty(s_insetRectR); // + 1) * xdim) / SCIRESX) - 1;
	gameRect.B.y = ID.GetProperty(s_insetRectB); // + 1) * ydim) / SCIRESY) - 1;
#endif

	ConvGameRectToPlaneRect();

#ifndef WIN32S
	color = ID.GetProperty(s_back);
#else
	color = (uchar)AddPalShift((int)ID.GetProperty(s_back));
#endif

	TranslateMacColor(color);

	priority	= ID.GetProperty(s_priority);
	picResNum = ID.GetProperty(s_picture);
	SetType();
	picMirror = ID.GetProperty(s_mirrored);

	GetPicResolution();

	int screenCount = graphMgr->ScreenCount();
	onScreen = plane;
	addCount = screenCount;
	deleteCount = 0;
	eraseCount = 0;
	priorityCount = 0;
	redrawAllCount = screenCount;
	updateCount = 0;

	ChangePic();
}

SOL_Plane::SOL_Plane(const SOL_Plane& src)
{
	// This constructor should only be used to create a copy
	// for the last screen's planes list when a Plane has been
	// added.
	
	planeId = src.planeId;
	priority = src.priority;
	picResNum = src.picResNum;
	picMirror = src.picMirror;
	color = src.color;
	TranslateMacColor(color);

	plane = src.plane;
	gameRect = src.gameRect;
	onScreen = src.onScreen;

	// Copy over all the new screen items
	siArray = src.siArray;
}

SOL_Plane::SOL_Plane(const SOL_Rect& rect)
{
	// Plane created internally

	planeId = nextId;
	nextId++;

	gameRect = rect;
	ConvGameRectToPlaneRect();

	// Default to highest priority
	color = SOL_BLACK;
	priority = graphMgr->TopPlane()+1;
	if (priority < PLANEPRIORITYSTART)
		priority = PLANEPRIORITYSTART;

	picResNum = -1;
	SetType();
	picMirror = False;

	GetPicResolution();

	int screenCount = graphMgr->ScreenCount();
	onScreen = plane;
	addCount = screenCount;
	deleteCount = 0;
	eraseCount = 0;
	priorityCount = 0;
	redrawAllCount = screenCount;
	updateCount = 0;
}

SOL_Plane&
SOL_Plane::operator=(ObjectID& id)
{
	// This assignment operator is used to update
	// a planes data from an SCI plane object
	
	/*
		If the plane moved then every screenItem in the nextList
		gets added to the drawList except screenItems not on the
		plane and the screen.

		A plane moved if:
			1) It's resolution changed.
			2) It's top,left corner changed.
			3) There is a new pic for it.

		A plane did not move if:
			1) It's bottom,right moved.
	*/


	vanish.x = id.GetProperty(s_vanishingX);
	vanish.y = id.GetProperty(s_vanishingY);

#ifdef NETWORK
	gameRect.Set(id.GetProperty(s_left),
					 id.GetProperty(s_top),
					 id.GetProperty(s_right),
					 id.GetProperty(s_bottom));
#else
	gameRect.Set(id.GetProperty(s_insetRectL),
					 id.GetProperty(s_insetRectT),
					 id.GetProperty(s_insetRectR),
					 id.GetProperty(s_insetRectB));
#endif

	ConvGameRectToPlaneRect();

	priority = id.GetProperty(s_priority);

	int newPic = id.GetProperty(s_picture);
	if (picResNum != newPic) {
		picResNum = newPic;
		changePic = True;
	}
	picMirror = id.GetProperty(s_mirrored);

#ifndef WIN32S
	color = id.GetProperty(s_back);
#else
	color = (uchar)AddPalShift((int)id.GetProperty(s_back));
#endif

	TranslateMacColor(color);

	return *this;
}

SOL_Plane&
SOL_Plane::operator=(const SOL_Plane& src)
{
	// This assignment should only be used to
	// update a plane from next to last screens
	// The siList will be updated separtely by
	// plane::Calclists

	gameRect = src.gameRect;
	plane = src.plane;
	vanish = src.vanish;
	picResNum = src.picResNum;
	type = src.type;
	picMirror = src.picMirror;
	priority = src.priority;
	color = src.color;

	TranslateMacColor(color);

	picResX = src.picResX;
	picResY = src.picResY;
	coordScaleX = src.coordScaleX;
	coordScaleY = src.coordScaleY;
	onScreen = src.onScreen;
	priorityCount = src.priorityCount;
	return *this;
}

void
SOL_Plane::UpdateRedrawAllCount()
{
	redrawAllCount = graphMgr->ScreenCount();
}

void
SOL_Plane::UpdatePlane(SOL_Plane* lastPlane)
{
	// Update from the last screen plane

	//	If the plane moved then every screenItem in the nextList
	//	gets added to the drawList except screenItems not on the
	//	plane and the screen.
	//
	//	A plane moved if:
	//		1) It's resolution changed.
	//		2) It's top,left corner changed.
	//		3) There is a new pic for it.
	//
	//	A plane did not move if:
	//		1) It's bottom,right moved.

	int screenCount = graphMgr->ScreenCount();

	if (lastPlane == NULL) {
		if (changePic) {
			DeleteAllPics();
			SetType();
			ChangePic();
			redrawAllCount = screenCount;
		}
		else
			SetType();
	}
	else {
		if (plane.A != lastPlane->DefRect().A) {
			// The plane moved
			redrawAllCount = screenCount;
			eraseCount = screenCount;
		}
		else {
			if ((plane.B.x > lastPlane->DefRect().B.x) ||
				(plane.B.y > lastPlane->DefRect().B.y)) {
				// Plane got larger in either x or y direction but did not move
				redrawAllCount = screenCount;
				eraseCount = screenCount;
			}
			else {
				if (plane != lastPlane->DefRect()) {
					// plane got smaller but did not move
					eraseCount = screenCount;
				}
			}
		}

		if (priority != lastPlane->Priority())
			priorityCount = screenCount;

		if ((picResNum != lastPlane->PicResNum())	||
		 	(picMirror != lastPlane->PicMirror())  ||
         changePic) {
			DeleteAllPics();
			SetType();
			ChangePic();
			redrawAllCount = screenCount;
		}

		if (color != lastPlane->Color())
			redrawAllCount = screenCount;
	}
	deleteCount	= 0;
	if (!addCount)
		updateCount = screenCount;
	ConvGameRectToPlaneRect();
	GetPicResolution();
	onScreen = plane;
	onScreen.Clip(graphMgr->NextScreen()->ScreenRect());
}

void
SOL_Plane::NewScreenDim(SOL_Rect theScreen)
{
	int screenCount = graphMgr->ScreenCount();

	ConvGameRectToPlaneRect();
	redrawAllCount = screenCount;

	// clip the onScreen by the screen
	onScreen = plane;
	onScreen.Clip(theScreen);
}

void
SOL_Plane::SetType() {
	switch (picResNum) {
		case -1:
			type = coloredPlane;
			break;
		case -2:
			type = transparentPlane;
			break;
		case -3:
			type = opaquePlane;
			break;
		case -4:
			type = pictureSkipPlane;
			break;
		default:
			if ((type != picturePlane) && (type != pictureSkipPlane))
				// Set if it's not already a picture plane
				type = picturePlane;
			break;
	}
}

void	
SOL_Plane::ChangePic()
{
   changePic = False;
	if ((type != picturePlane) && (type != pictureSkipPlane))
		return;

	//  Add the pic cels to the plane
	int count = 1000000;
	Bool hasSkipInBackground = True;
	for (int i = 0;i < count; i++)  {
		CelObjPic* picCel = New CelObjPic(picResNum,i);
		if (count == 1000000) {
			count = picCel->CelCount();
		}
		if (!picCel->HasSkip())
			hasSkipInBackground = False;
		ScreenItem* si = New ScreenItem(planeId,Bitmap(picResNum,i));
		if (picMirror)
			si->SetUserMirror(True);
		si->SetPriority(picCel->Priority());
		si->pos = picCel->Pos();
		si->SetPicResNum(picResNum);
		AddScreenItem(si);
		si->SetCel(picCel);
	}
	if (hasSkipInBackground)
		type = pictureSkipPlane;
	else
		type = picturePlane;
}

int
SOL_Plane::AddPic(int thePic, const SOL_Point& thePoint,int mirror,Bool delCopy)
{
	// Delete the pic if it's already present
	// This is needed for restore	because they are
	// deleting the plane then adding the plane 
	// then adding the pics again.
	if (delCopy)
		DeletePic(thePic);

	//  Add the pic cels to the plane
	int count = 1000;
	for (int i = 0; i < count; i++)  {
		CelObjPic* picCel = New CelObjPic(thePic,i);
		if (count == 1000)
			count = picCel->CelCount();
		ScreenItem* si = New ScreenItem(planeId,Bitmap(thePic,i));
		if (mirror)
			si->SetUserMirror(True);
		si->SetPriority(picCel->Priority());
		si->pos = picCel->Pos();
		si->SetPicResNum(thePic);
		// Position the cels 
		si->pos.x += thePoint.x;
		si->pos.y += thePoint.y;
		AddScreenItem(si);
		si->SetCel(picCel);
	}
	return picResNum;
}


void
SOL_Plane::DeleteAllPics()
{
	int count = siArray.Length();
	for (int i = 0; i < count; i++) {
		if (siArray.Exists(i))  {
			if (siArray[i].bitmap.IsPic()) {
				if (siArray[i].AddCount()) {
					AbortScreenItem(&siArray[i]);
				}
				else {
					siArray[i].SetDeleteCount(graphMgr->ScreenCount());
					siArray[i].SetAddCount(0);
					siArray[i].SetUpdateCount(0);
	  			}
			}
		}
	}
	siArray.Pack();
}

void
SOL_Plane::DeletePic(int oldPic,int newPic)
{
	DeletePic(oldPic);
	picResNum = newPic;
}

void
SOL_Plane::DeletePic(int picNumber)
{
	int count = siArray.Length();
	for (int i = 0; i < count; i++) {
		if (siArray.Exists(i))  {
			if (siArray[i].PicResNum() == picNumber) {
				siArray[i].SetDeleteCount(graphMgr->ScreenCount());
				siArray[i].SetAddCount(0);
				siArray[i].SetUpdateCount(0);
			}
		}
	}
}

void
SOL_Plane::ScrollScreenItems(int xStep,int yStep,Bool movePic)
{
	redrawAllCount = graphMgr->ScreenCount();
	int count = siArray.Length();
	for (int i = 0; i < count; i++) {
		if (siArray.Exists(i))  {
			if (!siArray[i].DeleteCount() 					&&
				 (!siArray[i].bitmap.IsPic()	||	movePic)) {
				siArray[i].pos.x += xStep;
				siArray[i].pos.y += yStep;
			}
		}
	}
}

void
SOL_Plane::GetPicResolution()
{
	picResX = SCIRESX;
	picResY = SCIRESY;
}

void
SOL_Plane::SetOnScreen(const SOL_Rect& r)
{
	onScreen = r;
}


void
SOL_Plane::Move(const SOL_Point& ul)
{
	MoveTo(ul);
	onScreen = plane;
}

void
SOL_Plane::MoveDelta(int deltaX, int deltaY)
{
	gameRect.Offset(deltaX,deltaY);
	ConvGameRectToPlaneRect();
	// Mark as update
	deleteCount	= 0;
	if (!addCount)
		updateCount = graphMgr->ScreenCount();
}

void
SOL_Plane::MoveTo(SOL_Point ul)
{
	Ratio scaleX(graphMgr->Xdim(), SCIRESX);
	Ratio scaleY(graphMgr->Ydim(), SCIRESY);
	ul.x = ul.x * scaleX;
	ul.y = ul.y * scaleY;
	gameRect.MoveTo(ul.x,ul.y);
	ConvGameRectToPlaneRect();
	// Mark as update
	deleteCount	= 0;
	if (!addCount)
		updateCount = graphMgr->ScreenCount();
}

void
SOL_Plane::ConvGameRectToPlaneRect()
{
	plane = gameRect;
	Ratio scaleX(graphMgr->Xdim(), SCIRESX);
	Ratio scaleY(graphMgr->Ydim(), SCIRESY);
	plane.A.x = plane.A.x * scaleX;
	plane.A.y = plane.A.y * scaleY;
	plane.B.x = ((plane.B.x + 1) * scaleX) - 1;
	plane.B.y = ((plane.B.y + 1) * scaleY) - 1;
}

void
SOL_Plane::ResizePlane(int xdim,int ydim)
{
	gameRect.Set(gameRect.A.x,gameRect.A.y,gameRect.A.x + xdim - 1,gameRect.A.y + ydim - 1);
	ConvGameRectToPlaneRect();
	onScreen = plane;
	// Mark as update
	deleteCount	= 0;
	if (!addCount) {
		if ((xdim > 0) || (ydim > 0))
			// Plane got larger in either x or y direction
			redrawAllCount = graphMgr->ScreenCount();
		updateCount = graphMgr->ScreenCount();
		eraseCount = graphMgr->ScreenCount();
	}

	onScreen.Clip(graphMgr->NextScreen()->ScreenRect());
}

void	
SOL_Plane::AddScreenItem(ScreenItem* si)
{
	siArray.Add(si);
}

void
SOL_Plane::AbortScreenItem(const ScreenItem* si)
{
	siArray.Delete(si);
}

int
SOL_Plane::TopScreenItem() const
{
	int thisPri;
	int topPri = -100000;
	int length = siArray.Length();
	for (int i = 0;i < length;i++) {
		if (siArray.Exists(i)) {
			if (siArray[i].FixedPri()) {
				if ((thisPri = siArray[i].Priority()) > topPri)
					topPri = thisPri;
			}
			else {
				if ((thisPri = siArray[i].pos.y) > topPri)
					topPri = thisPri;
			}
		}
	}
	return topPri;
}

Bool
SOL_Plane::NowSeen(MemID id,SOL_Rect& nowSeen)
{
	int index = siArray.Search(id);
	if (index != -1) {
		siArray[index].NowSeen(*this,nowSeen);
		return True;
	}
	return False;
}

void
SOL_Plane::CalcLists(SOL_Plane* last,PlaneList& nextPlanesList,DrawList& drawList,RectList& eraseList)
{
	//	Create the first entries into the eraseList and drawlist based
	//	upon screenItems that where added, updated or deleted.

	// Note
	// There is an assumption in this routine that
	// siArray in the next screen is in sync with
	// it's counterpart in the last screen

	ScreenItemList& lastList = last->SIList();

	int siArraySize = siArray.Length();
	SOL_Rect rects[4];
	int i;
	// Any screenItem marked Deleted add to the eraseList.
	// Any screenItem marked Added add to the drawList.
	// Any screenItem marked Updated add to the eraseList and the drawList.

	CheckIntegrity();

	for (i=0;i<siArraySize;++i) {
		if (siArray.Exists(i)) {
			if (siArray[i].DeleteCount())	{
				// Add to the eraseList if it was visable in the lastList.
				if (lastList.Exists(i)  &&  !lastList[i].OnScreen().IsEmpty()) {
					if (Remap::remapCount)
						MergeToRectList(lastList[i].OnScreen(),eraseList);
					else
						eraseList.Add(lastList[i].OnScreen());
				}
			}
			else if (siArray[i].AddCount())	{
				// Update the screenItem
				siArray[i].DoUpdate(*this);

				// Add to the drawList if visable.
				if (!siArray[i].OnScreen().IsEmpty()) {
					if (Remap::remapCount) {
					   drawList.Add(&siArray[i],siArray[i].OnScreen());
						MergeToRectList((SOL_Rect)(siArray[i].OnScreen()),eraseList);
					} else
					   drawList.Add(&siArray[i],siArray[i].OnScreen());
				}
			}
			else if (siArray[i].UpdateCount()) {
				// Update the screenItem
				siArray[i].DoUpdate(*this);
            if (Remap::remapCount) {
               if(!siArray[i].OnScreen().IsEmpty() && lastList.Exists(i) &&  
                  !lastList[i].OnScreen().IsEmpty() &&
                  lastList[i].OnScreen().Intersect(siArray[i].OnScreen())
               ) {
                  rects[0] = lastList[i].OnScreen();
                  rects[0].Both(siArray[i].OnScreen());
					   drawList.Add(&siArray[i],siArray[i].OnScreen());
						MergeToRectList(rects[0],eraseList);
               } else {

				      if (!siArray[i].OnScreen().IsEmpty()) {
					      drawList.Add(&siArray[i],siArray[i].OnScreen());
						   MergeToRectList((SOL_Rect)(siArray[i].OnScreen()),eraseList);
					   }

				      if (lastList.Exists(i)  &&  !lastList[i].OnScreen().IsEmpty()) 
				         MergeToRectList(lastList[i].OnScreen(),eraseList);
               }
            } else {
				   // Add to the drawList if visable.
				   if (!siArray[i].OnScreen().IsEmpty()) {
					   drawList.Add(&siArray[i],siArray[i].OnScreen());
			  	   }

				   // Add to the eraseList if it was visable in the lastList.
				   if (lastList.Exists(i)  &&  !lastList[i].OnScreen().IsEmpty()) {
						eraseList.Add(lastList[i].OnScreen());
				   }
            }
			}
		}
	}

	// If an erase rect is overlaped by a non transparent plane 
	// of higher priority, delete the overlap from the erase 
	// list for this plane
	BreakEraseListByPlanes(eraseList,nextPlanesList);

	// If a draw rect is overlaped by a plane of higher priority,
	//	delete the overlap from the erase list for this plane
	BreakDrawListByPlanes(drawList,nextPlanesList);

	// Don't need to check the drawlist member from the erase rects
	// against non changing member of the siArray
	int drawListSize = drawList.Length();

	//	Now go through the eraselist looking for anything that
	//	collides	with members of the siArray that are unchanged,
	//	and add them to the drawList.

	// Sort the siArray by priority
	siArray.Sort();

	int pictureDrawn = 0;
	int screenItemDrawn = 0;

	int eraseListSize = eraseList.Length();

	for (i=0;i<eraseListSize;++i) {
		for (int j=0;j<siArraySize;++j) {
			if (siArray.Exists(j)) {
				if (eraseList[i].Intersect(siArray[j].OnScreen(),rects)) {
					// Don't add to the drawList if:
					//		If it's a picture screen Item after the first picture
					//		screenItem encountered and no non-picture screen item
					//		has been encountered and the subsequent picture item is
					// 	completely within the previously drawn screen item then
					//		it doesn't have to be drawn. In other words don't draw 
					//		picture screenItems next to picture screenItems unless 
					//		neccessary.
					// This assumes that each picture screen item contains
					// all the rest of the picture priority from it's priority 
					// and to any higher picture priority and that the lowest
					// priority picture element contains the entire picture.

					// This version takes a shortcut in that if a non picuture
					// screen item is drawn all the other picture screen items
					// will be drawn.
					if (!siArray[j].DeleteCount()) {
						// This item may be added to the drawList

						if (pictureDrawn)	{
							if ((siArray[j].bitmap.IsPic())) {
								if (screenItemDrawn || (siArray[j].bitmap.Cel() == 0)) {
									// The cel == 0 means it's a lowest 
									// priority picture element
									drawList.Add(&siArray[j],rects[0]);
								}
							}
							else {
								// Add to the drawList
								if (!(siArray[j].UpdateCount() || siArray[j].AddCount()))
									drawList.Add(&siArray[j],rects[0]);
								screenItemDrawn = True;
							}
						}
						else {
							// Add to the drawList
							if (!(siArray[j].UpdateCount() || siArray[j].AddCount()))
								drawList.Add(&siArray[j],rects[0]);
							if (siArray[j].bitmap.IsPic())
								pictureDrawn = True;
						}
					}
				}
			}
		}
	}
	// Reset the siArray to it's orginal state
	siArray.Unsort();

	//	Go through the drawList. If a member of the drawList intersects a
	//	member of the siArray that is unchanged and has higher priority,
	//	then add	this member	to the drawList
	if(!Remap::remapCount) {
   	for (i=0;i<drawListSize;++i) {	// Don't use dynamic list size!!
   		for (int j=0;j<siArraySize;++j) {
				if (drawList.Exists(i) && 
					 siArray.Exists(j) && 
					 !(siArray[j].UpdateCount() ||
					   siArray[j].DeleteCount() ||
						siArray[j].AddCount()
					  )
				   ) {
				   if	(
					   (siArray[j].Priority() > drawList[i].SI().Priority()) ||
					   (
					   (siArray[j].Priority() == drawList[i].SI().Priority()) &&
					   (siArray[j].Id() > drawList[i].SI().Id())
					   )
					   ) {
					   if (drawList[i].OnScreen().Intersect(siArray[j].OnScreen(),rects)) {
						   // Add to the drawList
							drawList.Add(&siArray[j],rects[0]);
					   }
				   }
			   }
		   }
      }
   }

//	if ( drawList.Length() )
//		msgMgr->Mono ( "DrawList size = %d\n", drawList.Length() );

	DecSIArrayCounts(last);
	siArray.Pack();
	lastList.Pack();
}


void
SOL_Plane::RedrawAll(SOL_Plane* last,PlaneList& nextPlanesList,DrawList& drawList,RectList& eraseList)
{
	// Note
	// There is an assumption in this routine that
	// siArray in the next screen is in sync with
	// it's counterpart in the last screen

	int siArraySize = siArray.Length();

	// All screenItems get added to the drawList if visable and not deleted.
	for (int i=0;i<siArraySize;++i) {
		if ((siArray.Exists(i) && !siArray[i].DeleteCount())) {
			// Update the screenItem
			siArray[i].DoUpdate(*this);
			// Add to the drawList if visable.
			if (!siArray[i].OnScreen().IsEmpty()) {
				drawList.Add(&siArray[i],siArray[i].OnScreen());
			}
		}
	}

	// Clear eraseList (it may have erases from other planes).
	eraseList.Clear();

	// Add the planes onScreen rectangle to the eraseList
	if (!onScreen.IsEmpty()			&& 
		 (type != picturePlane) 	&&
		 (type != opaquePlane))
		eraseList.Add(onScreen);

	// If an erase rect is overlaped by a non transparent plane 
	// of higher priority, delete the overlap from the erase 
	// list for this plane
	BreakEraseListByPlanes(eraseList,nextPlanesList);

	// If a draw rect is overlaped by a non transparent plane 
	// of higher priority, delete the overlap from the erase 
	// list for this plane
	BreakDrawListByPlanes(drawList,nextPlanesList);
	redrawAllCount--;
	DecSIArrayCounts(last,True);
	siArray.Pack();
   if (last != NULL) {
	   ScreenItemList& lastList = last->SIList();
	   lastList.Pack();
   }
}

void
SOL_Plane::RemapMarkRedraw()
{
   int count = siArray.Length();
	for (int i = 0; i < count; i++) {
		if (siArray.Exists(i) 							&&
			 !siArray[i].DeleteCount()					&&
			 !siArray[i].AddCount() 					&&	
			 (siArray[i].RemapState() == RemapInCel)) {
 
	      siArray[i].SetUpdateCount(graphMgr->ScreenCount());
		}
	}
}

void
SOL_Plane::BreakDrawListByPlanes(DrawList& drawList,PlaneList& nextPlanesList)
{
	//	Go through the drawList and break apart members intersecting
	//	higher priority planes in the PlaneList.

	SOL_Rect rects[4];

	int nextPlanesListSize = nextPlanesList.Length();

	// Start from the next higher plane
	int nextHigherPlane = nextPlanesList.Search(planeId) + 1;

	for (int i=0;i< drawList.Length(); ++i) {
		// See if intersected by a plane of higher priority
		for (int j=nextHigherPlane;j<nextPlanesListSize;++j) {
			if (!nextPlanesList[j].IsTransparent() &&
				 !nextPlanesList[j].IsPictureSkip()) {
				int newRects = drawList[i].OnScreen().Split
					(
					nextPlanesList[j].OnScreen(),
					rects
					);
				if (newRects != -1) {
					// Eliminate this screenItem and add newRects new ones
					while (newRects--) {
						// Add to the drawList
						drawList.Add(&drawList[i].SI(),rects[newRects]);
					}
					drawList.Delete(i);
					break;
				}
			}
		}
	}

	// Pack the drawList to get rid of deleted screenItems.
	drawList.Pack();
}

void
SOL_Plane::BreakEraseListByPlanes(RectList& eraseList,PlaneList& nextPlanesList)
{
	//	Go through the eraseList and break apart members intersecting
	//	higher priority planes in the PlaneList.

	SOL_Rect rects[4];
	int nextPlanesListSize = nextPlanesList.Length();

	// Start from the next higher plane
	int nextHigherPlane = nextPlanesList.Search(planeId) + 1;

	for (int i=0;i< eraseList.Length();++i) {
		// See if intersected by a plane of higher priority
		for (int j=nextHigherPlane;j<nextPlanesListSize;++j) {
			if (!nextPlanesList[j].IsTransparent()	&&
				 !nextPlanesList[j].IsPictureSkip()) {
				int newRects = eraseList[i].Split
					(
					nextPlanesList[j].OnScreen(),
					rects
					);
				if (newRects != -1) {
					// Eliminate this screenItem and add newRects new ones
					while (newRects--) {
						// Add to the eraseList
						eraseList.Add(rects[newRects]);
					}
					eraseList.Delete(i);
					break;
				}
			}
		}
	}
	// Pack the eraseList to get rid of deleted rectangles.
	eraseList.Pack();
}


void
SOL_Plane::DecSIArrayCounts(SOL_Plane* last,Bool forceUpdate)
{
	//  Decrement counts if needed to reflect processed
	//  Also updates the siArray lists for the last screen

	ScreenItemList& lastList = last->SIList();

	int siArraySize = siArray.Length();
	for (int i = 0; i < siArraySize; ++i) {
		if (siArray.Exists(i))  {
			if (siArray[i].UpdateCount() ||
				 (
				 forceUpdate 											&& 
				 (last != NULL) 										&&
				 (last->SIList().Search(&siArray[i]) != -1)
				 )) {
			
				// The assigment operator is designed
				// for specifically to update the 
				// screenitems data in the previous
				// screen
				lastList[i] = siArray[i];
			}

			if (siArray[i].UpdateCount()) {
				siArray[i].DecUpdateCount();
			}

			if (siArray[i].AddCount()) {
				siArray[i].DecAddCount();
				// Add the screen item in last screen.
				// The constructor of a screen item from a screen
				// item is designed to put a copy of a screen item
				// into the last screen.
				if (last != NULL) {
					// If the last plane does not yet exist all
					// of the screen items will be copied to
					// the created last plane in Screen::CalcLists()
					ScreenItem* siCopy = New ScreenItem(&siArray[i]);
					lastList.Add(siCopy);
				}
			}

			//  Decrement delete count, if it hits zero, remove it from list
			if (siArray[i].DeleteCount())  {
				siArray[i].DecDeleteCount();
				if (!siArray[i].DeleteCount()) {
					// Delete the screen item in last screen
					if ((last != NULL) && lastList.Exists(i))
						lastList.Delete(i);
					// Delete the screen item in next screen
					siArray.Delete(i);
				}
			}
		}
	}
}

void
KGlobalToLocal(argList)
{
	// Given an SCI plane object, convert the event to local coordinates
	// arg(1) = event
	//	arg(2) = plane

   SOL_Point point(0,0);

   pm.acc = GlobalToLocalGame((int)(unsigned short)arg(2),point);

	// The point comes back as negative offsets
	ObjectID evt = arg(1);
	evt.SetIndexedProperty(evX,evt.GetIndexedProperty(evX) + point.x);
	evt.SetIndexedProperty(evY,evt.GetIndexedProperty(evY) + point.y);
}

Bool
GlobalToLocalGame(int planeID,SOL_Point& point)
{
	Bool		exitBool = True;
	SOL_Rect r;
	Screen& 	last =	graphMgr->VisibleScreen();

	// Given a plane def, convert the point to local game coordinates
	int index = last.Planes().Search(planeID);
	if (index == -1) {
		// Must be only in the next screen's planes list
		Screen* next = graphMgr->NextScreen();
		index = next->Planes().Search(planeID);
		if (index == -1)
			msgMgr->Fatal(SrcLoc,Msg_PlaneNotFound,planeID);
		r = next->Planes()[index].GameRect();
		exitBool = False;
	}
	else
		r = last.Planes()[index].GameRect();
	point.x -= r.A.x;
	point.y -= r.A.y;
	return exitBool;
}


// HOOK
// Never used
//void
//GlobalToLocal(int planeID,SOL_Point& point)
//{
//	// Given a plane def, convert the point to local coordinates
//	int index = graphMgr->VisibleScreen().Planes().Search(planeID);
//	if (index == -1)
//		msgMgr->Fatal(SrcLoc,Msg_PlaneNotFound,index);
//
//	SOL_Rect r = graphMgr->VisibleScreen().Planes()[index].DefRect();
//
//	point.x -= r.A.x;
//	point.y -= r.A.y;
//}

void
KLocalToGlobal(argList)
{
	// Given an SCI plane object, convert the event to global (screen) coordinates
	// arg(1) = event
	//	arg(2) = plane
   SOL_Point point(0,0);

   pm.acc = LocalGameToGlobal((int)(unsigned short)arg(2),point);

	// The point comes back as positive offsets
	ObjectID evt = arg(1);
	evt.SetIndexedProperty(evX,evt.GetIndexedProperty(evX) + point.x);
	evt.SetIndexedProperty(evY,evt.GetIndexedProperty(evY) + point.y);
}

Bool
LocalGameToGlobal(int planeID,SOL_Point& point)
{

	Bool		exitBool = True;
	SOL_Rect r;
	Screen& 	last =	graphMgr->VisibleScreen();

	// Given a plane def, convert the point to global game coordinates
	int index = last.Planes().Search(planeID);
	if (index == -1) {
		// Must be only in the next screen's planes list
		Screen* next = graphMgr->NextScreen();
		index = next->Planes().Search(planeID);
		if (index == -1)
			msgMgr->Fatal(SrcLoc,Msg_PlaneNotFound,planeID);
		r = next->Planes()[index].GameRect();
		exitBool = False;
	}
	else
		r = last.Planes()[index].GameRect();
	point.x += r.A.x;
	point.y += r.A.y;
	return exitBool;
}

// HOOK
// Never used
//void
//LocalToGlobal(int planeID,SOL_Point& point)
//{
//	// Given a plane def, convert the point to global coordinates
//	int index = graphMgr->VisibleScreen().Planes().Search(planeID);
//	if (index == -1)
//		msgMgr->Fatal(SrcLoc,Msg_PlaneNotFound,index);
//
//
//	SOL_Rect r = graphMgr->VisibleScreen().Planes()[index].DefRect();
//
//	point.x += r.A.x;
//	point.y += r.A.y;
//}

void
SOL_Plane::FilterUpEraseRects(DrawList& myDrawList,RectList& theirEraseList)
{
	// If a rectangle in theirEraseList intersects my plane, then any	screen
	// item in my plane that intersects the rectangle must be redrawn. My
	// plane is transparent. The rectangle will be passed up to higher
	// transparent priority planes by screen.

	int theirCount = theirEraseList.Length();
	for (int i = 0;i < theirCount;++i) {
		SOL_Rect r = theirEraseList[i];
		int myCount = siArray.Length();
		for (int j = 0;j < myCount;j++) {
			if (siArray.Exists(j))  {
				if (r.Intersect(siArray[j].OnScreen()))
					MergeToDrawList(j,r,myDrawList);
			}
		}
	}
}

void
SOL_Plane::FilterDownEraseRects(DrawList& myDrawList,RectList& myEraseList,RectList& theirEraseList)
{
	// If a rectangle in theirEraseList intersects my plane, then any	screen
	// item in my plane that intersects the rectangle must be redrawn. If my
	// plane is transparent, the rectangle will be passed down to lower
	// priority planes. If my plane is opaque then the part of the rectangle
	// that intersects my plane will	not be passed on.

	if (IsTransparent() || IsPictureSkip()) {
		// I'm transparent
		int theirCount = theirEraseList.Length();
		for (int i = 0;i < theirCount;++i) {
			SOL_Rect r = theirEraseList[i];
			int myCount = siArray.Length();
			for (int j = 0;j < myCount;j++) {
				if (siArray.Exists(j))  {
					if (r.Intersect(siArray[j].OnScreen()))
						MergeToDrawList(j,r,myDrawList);
				}
			}
		}
	}
	else {
		// I'm not transparent
		int theirCount = theirEraseList.Length();
		for (int i = 0;i < theirCount;++i) {
			SOL_Rect r = theirEraseList[i];
			if (r.Intersect(OnScreen())) {
				r.Clip(OnScreen());
				MergeToRectList(r,myEraseList);
				int myCount = siArray.Length();
				for (int j = 0;j < myCount;j++) {
					if (siArray.Exists(j))  {
						if (r.Intersect(siArray[j].OnScreen()))
							MergeToDrawList(j,r,myDrawList);
					}
				}
				UpdateTheirEraseList(r,i,theirEraseList);
			}
		}
		theirEraseList.Pack();
	}
}

void
SOL_Plane::FilterUpDrawRects(DrawList& myDrawList,DrawList& theirDrawList)
{
	// If a rectangle in theirDrawList intersects my plane, then any
	// screen item in my plane that intersects the rectangle must be
	// redrawn. My plane is transparent. The rectangle will be passed
	// up to higher priority transparent planes.


	int theirCount = theirDrawList.Length();
	for (int i = 0;i < theirCount;++i) {
		SOL_Rect r = theirDrawList[i].OnScreen();
		int myCount = siArray.Length();
		for (int j = 0;j < myCount;j++) {
			if (siArray.Exists(j))  {
				if (r.Intersect(siArray[j].OnScreen()))
					MergeToDrawList(j,r,myDrawList);
			}
		}
	}
}

void
SOL_Plane::MergeToDrawList(int i,SOL_Rect& r,DrawList& myDrawList)
{
	// If the screen item siArray[i] is already in myDrawList
	// and either r is enclosed by siArray[i].OnScreen() or
	//	siArray[i].OnScreen() is enclosed by r then change the
	// drawlist entry. Else add a new draw list entry.

	SOL_Rect mergeRect = siArray[i].OnScreen();
	mergeRect.Clip(r);

	RectList mergeList;
	mergeList.Add(mergeRect);
	int j, k;
	for (j = 0;j < mergeList.Length();++j) {
		mergeRect = mergeList[j];

		int myCount = myDrawList.Length();
		for (k = 0;k < myCount;++k) {
			if (siArray[i].Id() == myDrawList[k].SI().Id()) {
				SOL_Rect drawRect = myDrawList[k].OnScreen();
				if (drawRect.Includes(mergeRect)) {
					// Already being complety drawn
					mergeList.Delete(j);
					break;
				}
				else {
					// Check for being partly drawn
					SOL_Rect rects[4];
					int newRects = mergeRect.Split(drawRect,rects);
					if (newRects != -1) {
						// Eliminate the merege rect and add new ones
						while (newRects--) {
							mergeList.Add(rects[newRects]);
						}
						mergeList.Delete(j);
						mergeRect = mergeList[++j];
					}
				}
			}
		}
	}
	// Now add any thing left in the mergeList
	mergeList.Pack();
	for (j = 0;j < mergeList.Length();++j) {
		myDrawList.Add(&siArray[i],mergeList[j]);
	}
}

void
SOL_Plane::UpdateTheirEraseList(SOL_Rect& r,int i,RectList& theirEraseList)
{
	// theirEraseList[i] will be removed from theirEraseList and whats 
	// left of theirEraseList[i] after subtracting r will be added to 
	// their erase list

	SOL_Rect	rects[4];

	SOL_Rect bigRect = theirEraseList[i];
	int newRects = bigRect.Split(r,rects);
	while (newRects--) {
		// Add to the theirEraseList
		theirEraseList.Add(rects[newRects]);
	}
	theirEraseList.Delete(i);
}

void SOL_Plane::CheckIntegrity ( void )
{
	if ( type == picturePlane || type == pictureSkipPlane )	{
		int siArraySize = siArray.Length(), picSI = 0;

		for (int i=0;i<siArraySize;++i) {
			if (siArray.Exists(i)) {
				if ( siArray[i].bitmap.Type() == 1 )
					return;
			}
		}

		msgMgr->Fatal ( "Picture screen item has been lost.  (%s)", gPlaneFatalStr );
	}
}

void
MergeToRectList(const SOL_Rect& r,RectList& myEraseList)
{
	int j;
	SOL_Rect mergeRect = r;
	RectList mergeList;
	mergeList.Add(mergeRect);
	for (j = 0;j < mergeList.Length();++j) {
		mergeRect = mergeList[j];

		int myCount = myEraseList.Length();
		for (int k = 0;k < myCount;++k) {
			SOL_Rect eraseRect = myEraseList[k];
			if (eraseRect.Includes(mergeRect)) {
				// Already being complety erased
				mergeList.Delete(j);
				break;
			}
			else {
				// Check for being partly drawn
				SOL_Rect rects[4];
				int newRects = mergeRect.Split(eraseRect,rects);
				if (newRects != -1) {
					// Eliminate the merege rect and add new ones
					while (newRects--) {
						mergeList.Add(rects[newRects]);
					}
					mergeList.Delete(j);
					mergeRect = mergeList[++j];
				}
			}
		}
	}
	// Now add any thing left in the mergeList
	mergeList.Pack();
	for (j = 0;j < mergeList.Length();++j) {
		myEraseList.Add(mergeList[j]);
	}
}


