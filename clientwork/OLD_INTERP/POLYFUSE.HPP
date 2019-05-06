#ifndef MERGE_HPP
#define MERGE_HPP

#include "polygon.hpp"

class	SOL_PolygonF
{
public:
	~SOL_PolygonF() {if (polyPointsID) polyPointsID.Free();}
	FPoint*     polyPoints; 	// points of polygon
   uchar       type;       	// polygon type
	uchar	      info;		   	// bit flag info	
   int         n;          	// number of nodes in polygon
	MemID			polyPointsID;	// MemID to the points
};

class	PolyPatchF
{
public:
   int         P_i;     // Intersection node of given polygon
	int      	Q_i;		// start intersection node of patch loop
	FPoint   	P_U;		// start intersection point of patch loop
   int         P_j;     // end intersection node of given polygon
   int         Q_j;     // end intersection node of patch loop
   FPoint   	Q_U;     // end intersection point of patch loop
	Boolean		deleteIt;
};

inline double
DistanceSquaredF(FPoint& P2,FPoint& P1)
{
   return (P2-P1)*(P2-P1);

}


#ifdef FLOATINGPOINT
#ifdef SHOWMERGE
MemID    BuildPolygonBitmap(Array array,
                            int size,
                            uchar color,
                            int style,
						          int pattern,
                            int thickness,
                            SOL_Rect& rect);
#endif
MemID    MergePolygonsF(SCI_Point* polygon,int size,SOL_PolygonF* polylist);
Boolean  MergePolygonF(FPoint* P,int& Ppoints,SOL_PolygonF& polygon);
Boolean  PatchNodeF(PolyPatchF& Patch,int theNode,FPoint* thePoly);
Boolean  DominatesF(PolyPatchF& A,PolyPatchF& B,FPoint* P);
void     RemoveColinearLinesF(FPoint* P,int* n);
#else
MemID    MergePolygons(SCI_Point* polygon,int size,SOL_Polygon* polylist);
void     MergePolygon(SCI_Point* P,int n,SOL_Polygon& polygon);
Boolean  PatchNode(PolyPatch& Patch,int theNode,SCI_Point* thePoly);
Boolean  Dominates(PolyPatch& A,PolyPatch& B,SCI_Point* poly);
#endif

#endif


