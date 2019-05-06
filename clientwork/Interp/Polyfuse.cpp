#include "sol.hpp"
#ifdef MERGEPOLYGONS
#include "array.hpp"
#include "graph.hpp"
#include "line.hpp"
#include "math.hpp"
#include "palmgr.hpp"
#include "polyfuse.hpp"
#include "window.hpp"

// MERGE A SINGLE POLYGON INTO A LIST OF POLYGONS
//
//
// NAME---MergePolygonsF
//          ****Floating point version****
// input:   
//          list of SCI points in the polygon to merge
//          size of the polygon to merge
//          list of polygons to merge with the given polygon
// output: 
//          Returns a ARRAY of points corresponding to the merged polygon
//          Also marks polygons in the original list MERGED if they were
//          merged into the given polygon 
// 

#pragma optimize( "", off )

MemID 
MergePolygonsF(SCI_Point* polygon,int size,SOL_PolygonF* polylist)
{
   FPoint   P[MAXPOLYPATH];
   int      i,j;
	int		Ppoints;

   if (size >= MAXPOLYPATH)
		msgMgr->Fatal(SrcLoc,Msg_MergedPolygonTooLarge);

   // copy polygon to merge into P
   for (Ppoints=0;Ppoints < size;++Ppoints)
      P[Ppoints] = polygon[Ppoints];

   P[Ppoints].x = ENDOFPATHF;
   P[Ppoints].y = ENDOFPATHF;

	RemoveDuplicatePointsF(P,&Ppoints);
   if (Ppoints < 3)
	   // Degenerate case no polygon!!!
      return 0;
   RemoveColinearLinesF(P,&Ppoints);
   if (Ppoints < 3)
	   // Degenerate case no polygon!!!
      return 0;

#ifdef SHOWMERGE
   // Show the polygon to merge

	static int mergeCount = 0;
	++mergeCount;
	// Put the points into an array
	Array toMergeArray(2*Ppoints,INTARRAY);
	SCI_Point* toMergePoly = (SCI_Point*) toMergeArray.calcAddress (0);
   for (int k=0;k < Ppoints;++k) 
      toMergePoly[k] = P[k];

	int	z = 2;
	// color close to blue
	uchar	color = graphMgr->GPalette().Current().Match(Rgb24(0,0,255));
	int	style = SOL_Line::tSolid;
	int	pattern = 0;
	int	thickness = 1;
	SOL_Rect rect;

	MemID BM1 = BuildPolygonBitmap(toMergeArray,
                                 Ppoints,
										   color,
										   style,
										   pattern,
										   thickness,
										   rect);
   toMergeArray.dataID().Free();

	Bitmap bitmap1(BM1);
	polyMergeSi = New ScreenItem(polyPlane->Id(),bitmap1,rect);
	polyMergeSi->SetPriority(z);
	// add the screen item
	graphMgr->AddScreenItem(polyMergeSi);
	graphMgr->FrameOut();

	WaitOnUser();

#endif   // SHOWMERGE
   // Merge given polygon with each polygon in the list
   // A merged polygon in the list will be marked as MERGED.

   // See if the given polygon is completely included within another
   for (i=0;polylist[i].n != 0;++i)
   {
      // If this polygon has been marked as MERGED
      if (polylist[i].type < MERGED) 
		{
         int result = PolyIsInPolyF(P,Ppoints,polylist[i].polyPoints,polylist[i].n);
         switch (result)
         {
            case PContainsQ:
                     // P contains polylist[i]
                     polylist[i].type |= MERGED; 
                     break;
            case QContainsP:
                     // polylist[i] contains P
                     // replace P with polylist[i]
                     FPoint* Q = polylist[i].polyPoints;
                     int Qpoints = polylist[i].n;
                     Ppoints = Qpoints;
                     for (j=0;j<Qpoints;++j)
                     {
                        P[j] = Q[j];
                     }
                     polylist[i].type |= MERGED; 
                     break;
         }

		}
   }

   for (i=0;polylist[i].n != 0;++i)
   {
      // If this polygon has been marked as MERGED
      if (polylist[i].type < MERGED) 
		{
         Boolean fused = MergePolygonF(P,Ppoints,polylist[i]);
         // Set Ppoints to the number of points now in P
         if (fused)
         {
	         RemoveDuplicatePointsF(P,&Ppoints);
   			if (Ppoints < 3)
	   			// Degenerate case no polygon!!!
      			return 0;

            RemoveColinearLinesF(P,&Ppoints);
   			if (Ppoints < 3)
	   			// Degenerate case no polygon!!!
      			return 0;
         }
		}
   }

   P[Ppoints].x = ENDOFPATHF;
   P[Ppoints].y = ENDOFPATHF;

   // return the new polygon
	Array array (2*Ppoints,INTARRAY);

	SCI_Point* mergedPoly = (SCI_Point*) array.calcAddress (0);

   for (j=0;j < Ppoints;++j) 
      mergedPoly[j] = P[j];

   return array.dataID();
}

#pragma optimize( "", on )

// MERGE A POLYGON WITH ANOTHER POLYGON
//
//
// NAME---MergePolygonF
//          ****Floating point version****
// input:   
//          array of floating points in the polygon to merge
//          size of the polygon to merge
//          polygon structure of the polygon to merge with the given polygon
// output: 
//          Updates the points to the merged polygon
//          Also marks the polygon in the original list MERGED if it was
//          merged into the given polygon 
// 
Boolean
MergePolygonF(FPoint* P,int& Ppoints,SOL_PolygonF& polygon)
{

   #define MAXPATCHES 20

   int         i,j,k,p; 
	int			Qpoints;
   Boolean     addNode; 
   Boolean     fused = False; 
   int         P_i,Q_i,P_j,Q_j; 
   int         intersectResult; 
   int         angleIn,angleOut,delta,angle; 
   FPoint*     Q;
   FPoint      P_U,Q_U,Result[MAXPOLYPATH];
   FPoint      wrk1,wrk2;
   PolyPatchF  Patch[MAXPATCHES],newPatch;
   int         patches = 0;
   
   Q = polygon.polyPoints;
   Qpoints = polygon.n;

   for (P_i=0;P_i < Ppoints;++P_i)
   {
      for (Q_i=0;Q_i < Qpoints;++Q_i)
      {
         intersectResult = IntersectSegmentsF(P[P_i],
															 P[(P_i+1+Ppoints) % Ppoints],
															 Q[Q_i],
															 Q[(Q_i+1+Qpoints) % Qpoints],
															 &P_U);
         if ((intersectResult != NOINTERSECT) && (intersectResult != INTERSECT+INTERSECTD))
         {
            if (intersectResult == COLINEAR)
            {
               // check that the lines are going different directions
               if (((P[(P_i+1+Ppoints) % Ppoints]-P[P_i])*(Q[(Q_i+1+Qpoints) % Qpoints]-Q[Q_i])) > 0.0)
                  continue;
               // make sure the Q line does not stop within the P line
               if (((Q[(Q_i+1+Qpoints) % Qpoints]-P[P_i])*(Q[(Q_i+1+Qpoints) % Qpoints]-P[(P_i+1+Ppoints) % Ppoints])) <= 0.0 )
                  continue;
               // Now make sure Q is an exit not an entry to P
               if (((Q[(Q_i+1+Qpoints) % Qpoints]-Q[Q_i])^(P[P_i]-P[(P_i-1+Ppoints) % Ppoints])) < 0.0)
                  continue;
               // This is a valid exit and should create a patch
               // The intersection point is

               P_U =  P[P_i];
            }

            /* Is this an exit from P intersection?
                  For example:
                                *P[i+1]
                               *      
                              *           
                             *           
                   INSIDE   *   OUTSIDE
                           *     
                          *   
            Q[j]---------*---------------->Q[j+1]   This is an exiting
                        *                           intersection
                       *
                      *                     
                     *
                P[i]*

                                *P[i+1]
                               *      
                              *           
                             *           
                  INSIDE    *   OUTSIDE
                           *     
                          *   
            Q[j+1]<------*-----------------Q[j]     This is an entering
                        *                           intersection
                       *
                      *                     
                     *
                P[i]*


            */
            // test for exit or entry
            wrk1 = P[(P_i+1+Ppoints) % Ppoints] - P[P_i];
            wrk2 = Q[(Q_i+1+Qpoints) % Qpoints] - Q[Q_i];
            if ((wrk1^wrk2) <= 0)
            {
               // now test for clock wise or counter clockwise
               angle = 0;
               angleIn = ATan((int)Q[Q_i].x,
										(int)Q[Q_i].y,
										(int)Q[(Q_i+1+Qpoints) % Qpoints].x,
										(int)Q[(Q_i+1+Qpoints) % Qpoints].y);
               for (Q_j=Q_i+1;Q_j <= Q_i + Qpoints;++Q_j)
               {
                  angleOut = ATan((int)Q[(Q_j+Qpoints) % Qpoints].x,
											 (int)Q[(Q_j+Qpoints) % Qpoints].y,
											 (int)Q[(Q_j+1+Qpoints) % Qpoints].x,
											 (int)Q[(Q_j+1+Qpoints) % Qpoints].y);
                  delta = angleOut - angleIn;
                  if (delta > 180) delta -= 360;
                  if (delta < -180) delta += 360;
                  angle += delta;
                  angleIn = angleOut;
                  for (P_j = P_i;P_j < P_i + Ppoints ;++P_j)
                  {
                     intersectResult = IntersectSegmentsF(P[(P_j + Ppoints) % Ppoints],
																			P[(P_j+1+Ppoints) % Ppoints],
																			Q[(Q_j+Qpoints) % Qpoints],
																			Q[(Q_j+1+Qpoints) % Qpoints],
																			&Q_U);
                     if ((intersectResult != NOINTERSECT) && (intersectResult != INTERSECT+INTERSECTD))
                     {
                        if (intersectResult == COLINEAR)
                        {
                           // is Q_j is interior to P_j-P_j+1
                           if ((Q[Q_j % Qpoints]-P[P_j])*(Q[Q_j % Qpoints]-P[(P_j+1+Ppoints) % Ppoints]) < 0.0)
                              Q_U = Q[Q_j % Qpoints];
                           else
                              Q_U = P[P_j % Ppoints];
                        }
      
                        // test for exit or entry
                        wrk1 = P[(P_j+1+Ppoints) % Ppoints] - P[(P_j + Ppoints) % Ppoints];
                        wrk2 = Q[(Q_j+1+Qpoints) % Qpoints] - Q[(Q_j + Qpoints) % Qpoints];
                        if ((wrk1^wrk2) >= 0)
                           break;
                        else
                           intersectResult = NOINTERSECT;
                     }
                     else
                        intersectResult = NOINTERSECT;
                  }
                  if (intersectResult != NOINTERSECT) break;
               }
               // we need to find an intersection
               if (intersectResult != NOINTERSECT)
               {
                  // If loop to add is screen clockwise add to P patches
                  if (angle > 0)
                  {
                     if (patches >= MAXPATCHES)
		                  msgMgr->Fatal(SrcLoc,Msg_PolygonTooManyPatches);
                     // add loop to patches
                     if(patches == 0)
                     {
                        Patch[patches].P_i = P_i;
                        Patch[patches].Q_i = Q_i;
                        Patch[patches].P_U = P_U;
                        Patch[patches].P_j = (P_j + Ppoints) % Ppoints;
                        Patch[patches].Q_j = (Q_j + Qpoints) % Qpoints;
                        Patch[patches].Q_U = Q_U;
                        Patch[patches++].deleteIt = False; 
                     }
                     else
                     {
                        newPatch.P_i = P_i;
                        newPatch.Q_i = Q_i;
                        newPatch.P_U = P_U;
                        newPatch.P_j = (P_j + Ppoints) % Ppoints;
                        newPatch.Q_j = (Q_j + Qpoints) % Qpoints;
                        newPatch.Q_U = Q_U;
                        newPatch.deleteIt = False; 
								// If the new patch is dominated don't add to patches
                        for(p = 0;p < patches; p ++)
                           if (DominatesF(Patch[p],newPatch,P)) break;
                        if(p == patches)
                        {
									// If the new patch dominates a patch,
									// remove it from the patches
                           for(p = 0;p < patches ; p++)
                              if (DominatesF(newPatch,Patch[p],P)) Patch[p].deleteIt = True;

									// There may be a patch for this same line
									// that belongs in front of this patch!
									FPoint wrk1 = P_U - P[P_i];
                        	for(p = patches -1;p >= 0;p--)
									{
										if (Patch[p].P_i == P_i)
										{
											FPoint wrk2 = Patch[p].P_U - P[P_i];
											if (wrk1*wrk1 < wrk2*wrk2)
												continue;
											else
												break;
										}
										else
											break;
									}

									if (p != patches-1)
									{
										// Should reverse the patches
                           	newPatch.P_i = Patch[++p].P_i;
                           	newPatch.Q_i = Patch[p].Q_i; 
                           	newPatch.P_U = Patch[p].P_U; 
                           	newPatch.P_j = Patch[p].P_j; 
                           	newPatch.Q_j = Patch[p].Q_j; 
                           	newPatch.Q_U = Patch[p].Q_U; 
                           	newPatch.deleteIt = Patch[p].deleteIt; 

										Patch[p].P_i = P_i;
										Patch[p].Q_i = Q_i;
										Patch[p].P_U = P_U;
										Patch[p].P_j = (P_j + Ppoints) % Ppoints;
										Patch[p].Q_j = (Q_j + Qpoints) % Qpoints;
										Patch[p].Q_U = Q_U;  
                           	Patch[p].deleteIt = False; 
									}

                           Patch[patches].P_i =	newPatch.P_i;
                           Patch[patches].Q_i =	newPatch.Q_i;
                           Patch[patches].P_U =	newPatch.P_U;
                           Patch[patches].P_j =	newPatch.P_j;
                           Patch[patches].Q_j =	newPatch.Q_j;
                           Patch[patches].Q_U =	newPatch.Q_U;
                           Patch[patches++].deleteIt = newPatch.deleteIt; 
                        }
                     }
                  }
               }    
            }
         }
      }
   }
   if (patches)
   {
      // merge the polygons
      fused = True;

      // mark as merged polygon
      polygon.type |= MERGED; 
      j = 0;

      for (P_i=0;P_i < Ppoints;++P_i)
      {
         // if P_i is not in a patch add node
         for (i=0,addNode=True;(i<patches) && (addNode == True);++i)
            if(!Patch[i].deleteIt)
               if (PatchNodeF(Patch[i],P_i,P))
                  addNode = False;
         if (addNode) Result[j++] = P[P_i];
         // if at a patch, add the patch
         for (i=0;i<patches;++i)
         {
            if(Patch[i].deleteIt == False)
            {
               if (P_i == Patch[i].P_i)
               {
                  if (j >= MAXPOLYPATH)
		               msgMgr->Fatal(SrcLoc,Msg_PolygonTooManyPoints);
                  if (Patch[i].P_U != P[P_i])
                     Result[j++] = Patch[i].P_U;
                  for (k=(Patch[i].Q_i+1+Qpoints) % Qpoints;
                       k != Patch[i].Q_j;
                       k = (k+1+Qpoints) % Qpoints,++j) 
                     Result[j] = Q[(k+Qpoints) % Qpoints];
                  Result[j++] = Q[Patch[i].Q_j];
                  if (Patch[i].Q_U != Q[Patch[i].Q_j])                                           
                     Result[j++] = Patch[i].Q_U;
               }
            }
         }
      }
      for (i=0;i<j;++i) P[i] = Result[i];
      if (P[i-1] == P[0]) i--;
      P[i].x = ENDOFPATHF;
      P[i].y = ENDOFPATHF;
      // Update point count
      Ppoints = i;
   }
   return fused;
}

Boolean
PatchNodeF(PolyPatchF& Patch,int theNode,FPoint* thePoly)
{
   if (Patch.P_i < Patch.P_j)
      if ((Patch.P_i < theNode) && (theNode <= Patch.P_j))
         return (True);
   if (Patch.P_j < Patch.P_i)
      if ((Patch.P_i < theNode) || (theNode <= Patch.P_j))
         return (True);
   if (Patch.P_i == Patch.P_j)
      if (
         DistanceSquaredF(Patch.P_U,thePoly[Patch.P_i]) > 
         DistanceSquaredF(Patch.Q_U,thePoly[Patch.P_i])
         )
         return (True);
   return (False);
}

Boolean
DominatesF(PolyPatchF& A,PolyPatchF& B,FPoint* P)
{
	int Api = A.P_i;
   int Apj = A.P_j;
   int Bpi = B.P_i;
   int Bpj = B.P_j;
   if((A.P_U == B.P_U) && (A.Q_U == B.Q_U))
      // The same patch!
      return(True);

   if(Api != Apj)
   {
      if(
         ((Api<Apj) && ((Api < Bpi) && (Bpi < Apj))) ||
         ((Api>Apj) && ((Api < Bpi) || (Bpi < Apj))) 
        )
         /*
              patch A
         *-----------------------------*
         |                             |
         |    patch B                  |
         |  *----------...             |
         |  |                          |
         |  |                          |
         |  |       ********           |
         |  |      *        *          |
         |  *-----*          *         |
         |       *            *--------*
         |      *              *
         |     *                *
         |     *                 * 
         *-----*                  *
               *                   *  
               *                  *
               *                 *
               ******************   

         */
         return(True);


      if(
         ((Api<Apj) && ((Api < Bpj) && (Bpj < Apj))) ||
         ((Api>Apj) && ((Api < Bpj) || (Bpj < Apj))) 
        )
         /*
              patch A
         *-----------------------------*
         |                             |
         |    patch B                  |
         |  *----------*               |
         |             |               |
         |             |               |
         |          ********           |
         |         *        *          |
         |        *          *         |
         |       *            *--------*
         |      *              *
         |     *                *
         |     *                 * 
         *-----*                  *
               *                   *  
               *                  *
               *                 *
               ******************   

         */
         return(True);
   }
   if(Bpi != Bpj)
   {
      if(
         ((Bpi<Bpj) && ((Bpi < Api) && (Api < Bpj))) ||
         ((Bpi>Bpj) && ((Bpi < Api) || (Api < Bpj))) 
       )
         /*
              patch B
         *-----------------------------*
         |                             |
         |    patch A                  |
         |  *----------...             |
         |  |                          |
         |  |                          |
         |  |       ********           |
         |  |      *        *          |
         |  *-----*          *         |
         |       *            *--------*
         |      *              *
         |     *                *
         |     *                 * 
         *-----*                  *
               *                   *  
               *                  *
               *                 *
               ******************   

         */
         return(False);
      if(
     
         ((Bpi<Bpj) && ((Bpi < Apj) && (Apj < Bpj))) ||
         ((Bpi>Bpj) && ((Bpi < Apj) || (Apj < Bpj))) 
       )
         /*
              patch B
         *-----------------------------*
         |                             |
         |    patch A                  |
         |  *----------*               |
         |             |               |
         |             |               |
         |          ********           |
         |         *        *          |
         |        *          *         |
         |       *            *--------*
         |      *              *
         |     *                *
         |     *                 * 
         *-----*                  *
               *                   *  
               *                  *
               *                 *
               ******************   

         */
         return(False);
   }
   if(Api != Apj)
   {
      if(Bpi == Bpj)
      {
         if(Api == Bpi)
            if (DistanceSquaredF(P[A.P_i],A.P_U) < DistanceSquaredF(P[A.P_i],B.P_U))
               /*
                 patch A
               *----------------*       
               |                |      
               |  ************************
               |  *                      *
               |  *                      *-------*
               |  *                      *       |
               |  *                      *       |
               |  *                      *---*p  |
               |  *                      *   |a  |
               |  *                      *   |t  |
               |  *                      *   |c  |
               |  *                      *   |h  |
               |  *                      *   |B  |
               |  *                      *---*   |
               |  *                      *       |
               |  *                      *       |
               |  *                     *        |
               |  *                    *         |
               |  *********************          |
               |                                 |
               *---------------------------------*  
               */
               return(True);
            else
               /*
                 patch A
               *----------------*       
               |                |      
               |  ************************
               |  *                      *
               |  *                      *              
               |  *                      *     
               |  *                      *     
               |  *                      *---*p
               |  *                      *   |a
               |  *                      *   |t
               |  *                      *   |c
               |  *                      *   |h
               |  *                      *   |B
               |  *                      *---*          
               |  *                      *              
               |  *                      *-------*
               |  *                      *       |
               |  *                      *       |
               |  *                     *        |
               |  *                    *         |
               |  *********************          |
               |                                 |
               *---------------------------------*  
               */
               return(False);
         if(Apj == Bpi)
            if (DistanceSquaredF(P[A.P_j],A.Q_U) > DistanceSquaredF(P[A.P_j],B.P_U))
               /*
                 patch A
               *----------------*       
               |                |      
               |    patch B     |      
               |    *-----*     |      
               |    |     |     |      
               |    |     |     |      
               |  ************************
               |  *                      *
               |  *                      *
               |  *                      *
               |  *                      *---*
               |  *                      *   |
               |  *                      *   |
               |  *                      *   |
               |  *                      *   |
               |  *                     *    |
               |  *                    *     |
               |  *********************      |
               |                             |
               *-----------------------------*  
               */
               return(True);
            else
               /*
                 patch A
               *---------*       
               |         |      
               |         |   patch B   
               |         |   *-----*   
               |         |   |     |   
               |         |   |     |   
               |  ************************
               |  *                      *
               |  *                      *
               |  *                      *
               |  *                      *---*
               |  *                      *   |
               |  *                      *   |
               |  *                      *   |
               |  *                      *   |
               |  *                     *    |
               |  *                    *     |
               |  *********************      |
               |                             |
               *-----------------------------*  
               */
               return(False);
      }
      if(Api != Bpi) 
         /*
                                        
              patch A                            patch B                   
         *-------------*                    *-------------*                
         |             |                    |             |                
         |             |                    |             |                
         |          ********                |          ********            
         |         *        *               |         *        *           
         |        *          *              |        *          *          
         |       *            *--------*    |       *            *--------*
         |      *              *       |p   |      *              *       |p
         |     *                *      |a   |     *                *      |a
         |     *                 *     |t   |     *                 *     |t
         *-----*                  *    |c   *-----*                  *    |c
               *                   *   |h         *                   *   |h
               *                  *    |B         *                  *    |A
               *                 *-----*          *                 *-----*
               ******************                 ******************   

         */
         return(False);
      if (DistanceSquaredF(P[A.P_i],A.P_U) < DistanceSquaredF(P[A.P_i],B.P_U))
         /*
              patch A
         *-----------------------------*
         |                             |
         |    patch B                  |
         |  *----------*               |
         |  |          |               |
         |  |          |               |
         |  |       ********           |
         |  |      *        *          |
         |  |     *          *         |
         |  |    *            *--------*
         |  |   *              *
         |  |  *                *
         |  *--*                 * 
         *-----*                  *
               *                   *  
               *                  *
               *                 *
               ******************   

         */
         return(True);
      else
         /*
              patch B
         *-----------------------------*
         |                             |
         |    patch A                  |
         |  *----------*               |
         |  |          |               |
         |  |          |               |
         |  |       ********           |
         |  |      *        *          |
         |  |     *          *         |
         |  |    *            *--------*
         |  |   *              *
         |  |  *                *
         |  *--*                 * 
         *-----*                  *
               *                   *  
               *                  *
               *                 *
               ******************   

         */
         return(False);
      }
   if((Api == Apj) && (Api != Bpi))
   {
      // if A wraps around it dominates B.
      if (DistanceSquaredF(P[A.P_i],A.P_U) > DistanceSquaredF(P[A.P_i],A.Q_U))
         /*
              patch A
         *--------------------------------*
         |                                |
         |          ********              |
         |         *        *             |
         |        *          *--------*   |
         |       *            *       |p  |
         |      *              *      |a  |
         |     *                *     |t  |
         |     *                 *    |c  |
         *-----*                  *   |h  |
               *                   *  |B  |
         *-----*                  *---*   |
         |     *                 *        |
         |     ******************         |
         |                                |
         *--------------------------------*

         */
         return(True);
   }

   // if either patches wrap around then there can not be a dominance.
   if (DistanceSquaredF(P[A.P_i],A.P_U) > DistanceSquaredF(P[A.P_i],A.Q_U))
      /*
           patch A
      *-----------------------------*
      |                             |
      |          ********           |
      |         *        *          |
      |        *          *         |
      |       *            *        |
      |      *              *       |
      |     *                *      |
      |     *                 *     |
      *-----*                  *    |
            *                   *   |
      *-----*                  *    |
      |     *                 *     |
      |     ******************      |
      |                             |
      *-----------------------------*

      */
      return(False);
   if (DistanceSquaredF(P[B.P_i],B.P_U) > DistanceSquaredF(P[B.P_i],B.Q_U))
      /*
           patch B
      *-----------------------------*
      |                             |
      |          ********           |
      |         *        *          |
      |        *          *         |
      |       *            *        |
      |      *              *       |
      |     *                *      |
      |     *                 *     |
      *-----*                  *    |
            *                   *   |
      *-----*                  *    |
      |     *                 *     |
      |     ******************      |
      |                             |
      *-----------------------------*

      */
      return(False);

   if (DistanceSquaredF(P[B.P_i],B.P_U) > DistanceSquaredF(P[B.P_i],A.P_U))
      if (DistanceSquaredF(P[B.P_i],A.Q_U) > DistanceSquaredF(P[B.P_i],B.P_U))
         /*
                                         
                        *************         
                        *            *        
           *------------*             *       
         p |            *              *      
         a |            *               *     
         t |            *                *    
         c |   patchB   *                 *   
         h |  *---------*                  *  
         A |  |         *                   * 
           |  *---------*                  *  
           *------------*                 *   
                        ******************    
                                          

         */
         return(True);
   /*
                                                                
                  ********                          ********         
                  *       *                         *       *        
     *------------*        *           *------------*        *       
   p |            *         *          |            *         *      
   a |            *          *         |            *          *     
   t |            *           *        *------------*           *    
   c |   patchB   *            *                    *            *   
   h |  *---------*             *      *------------*             *  
   A |  |         *              *     |            *              * 
     |  *---------*             *      |            *             *  
     *------------*            *       *------------*            *   
                  *************                     *************    
                                                                 

   */
   return(False);
}


#ifdef SHOWMERGE

MemID
BuildPolygonBitmap(Array array,
                   int size,
                   uchar color,
                   int style,
						 int pattern,
                   int thickness,
                   SOL_Rect& rect)
{
	MemID    BM;

	thickness = Max(1,thickness);
	if (!(thickness & 1))
		thickness -= 1;
	thickness = thickness >> 1;

	SCI_Point* points = (SCI_Point*) array.calcAddress (0);

	rect.A.x = points[0].x;
	rect.B.x = points[0].x;
	rect.A.y = points[0].y;
	rect.B.y = points[0].y;

	for (int i=1;i<size;++i) 
   {
		if (rect.A.x > points[i].x) 
			rect.A.x = points[i].x;
		if (rect.B.x < points[i].x) 
			rect.B.x = points[i].x;

		if (rect.A.y > points[i].y) 
			rect.A.y = points[i].y;
		if (rect.B.y < points[i].y) 
			rect.B.y = points[i].y;
	}

	rect.A.x -= thickness;
	rect.A.y -= thickness;
	rect.B.x += thickness;
	rect.B.y += thickness;

	// Get memory for bitmap
	int BMWidth = Max(0,rect.Xlen());
	int BMHeight = Max(0,rect.Ylen());
	BM.Get(MemBitmap,BMWidth*BMHeight+CELHEADERSIZE);

	// Chose a skip
	uchar skip = 0;
	if (color != SOL_SKIP)
		skip = SOL_SKIP;
	// build the header for the BM
	BuildHeader(BM,BMWidth,BMHeight,skip);

	// Fill the bitmap with skip
	memset((uchar*) memMgr->GetAddr(BM)+CELHEADERSIZE,skip,BMWidth*BMHeight);

	// Draw the lines into the bitmap
	points = (SCI_Point*) array.calcAddress (0);


	for (i=0;i<size;++i) {
		SOL_Line line(points[i].x - rect.A.x,
						  points[i].y - rect.A.y,
						  points[(i+1)%size].x - rect.A.x,
						  points[(i+1)%size].y - rect.A.y);
		if (!thickness)
			line.DrawThin(color,style,pattern,BM,BMWidth);
		else
			line.DrawThick(color,thickness,style,pattern,BM,BMWidth);
	}

	return BM;
}
#endif // SHOWMERGE

#endif // MERGEPOLYGONS
