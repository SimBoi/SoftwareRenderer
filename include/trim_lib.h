/******************************************************************************
* Trim_lib.h - header file for the TRIMmed surfaces library.		      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber, Oct. 94.					      *
******************************************************************************/

#ifndef IRIT_TRIM_LIB_H
#define IRIT_TRIM_LIB_H

#include <stdio.h>
#include "irit_sm.h"
#include "miscattr.h"
#include "misc_lib.h"
#include "cagd_lib.h"
#include "symb_lib.h"

typedef enum {
    TRIM_ERR_TRIM_CRV_E2 = 2000,
    TRIM_ERR_BSPLINE_EXPECT,
    TRIM_ERR_BZR_BSP_EXPECT,
    TRIM_ERR_DIR_NOT_CONST_UV,
    TRIM_ERR_ODD_NUM_OF_INTER,
    TRIM_ERR_TCRV_ORIENT,
    TRIM_ERR_INCONSISTENT_CNTRS,
    TRIM_ERR_FAIL_MERGE_TRIM_SEG,
    TRIM_ERR_INVALID_TRIM_SEG,
    TRIM_ERR_INCON_PLGN_CELL,
    TRIM_ERR_TRIM_TOO_COMPLEX,
    TRIM_ERR_TRIMS_NOT_LOOPS,
    TRIM_ERR_LINEAR_TRIM_EXPECT,
    TRIM_ERR_NO_INTERSECTION,
    TRIM_ERR_POWER_NO_SUPPORT,
    TRIM_ERR_UNDEF_SRF,
    TRIM_ERR_TRIM_OPEN_LOOP,
    TRIM_ERR_TRIM_OUT_DOMAIN,
    TRIM_ERR_SINGULAR_TRIM_SEG,
    TRIM_ERR_UNTRIM_FAILED,

    TRIM_ERR_UNDEFINE_ERR
} TrimFatalErrorType;

/******************************************************************************
* A trimmed surface can have trimming curves that either form a closed loop   *
* or start and end on the boundary of the surface. A trimming curve will be   *
* defined using a list of TrimCrvSegStruct, creating a closed loop or a       *
* curve that starts and ends in the boundary of the surface.		      *
*   Orientation of TrimCrvSegStruct should be such that the trimming curve    *
* tangent direction crossed with the surface normal points into the inside.   *
*   EucCrv can be either NULL where the Euclidean location must be            *
* computed on the fly from parametric information or, if exist, must be       *
* used to prevent from black holes with adjacent surfaces.		      *
*   The trimming curves have no order what so ever.			      *
*   An outmost loop will always be present even if the entire four boundary   *
* curves are untrimmed.						 	      *
******************************************************************************/
typedef struct TrimCrvSegStruct {
    struct TrimCrvSegStruct *Pnext;
    IPAttributeStruct *Attr;
    CagdCrvStruct *UVCrv;    /* Trimming crv segment in srf's param. domain. */
    CagdCrvStruct *EucCrv;       /* Trimming curve as an E3 Euclidean curve. */
} TrimCrvSegStruct;

typedef struct TrimCrvStruct {
    struct TrimCrvStruct *Pnext;
    IPAttributeStruct *Attr;
    TrimCrvSegStruct *TrimCrvSegList;    /* List of trimming curve segments. */
} TrimCrvStruct;

typedef struct TrimSrfStruct {
    struct TrimSrfStruct *Pnext;
    IPAttributeStruct *Attr;
    int Tags;
    CagdSrfStruct *Srf;			  /* Surface trimmed by TrimCrvList. */
    TrimCrvStruct *TrimCrvList;		         /* List of trimming curves. */
} TrimSrfStruct;

typedef struct TrimUntrimResultStruct {
    struct TrimUntrimResultStruct *Pnext;
    IPAttributeStruct *Attr;
    CagdSrfStruct *ContainingSrf;
    CagdCrvQuadTileStruct *UVTiles;
    CagdSrfStruct *UntrimmedSrfs;
} TrimUntrimResultStruct; 

/* Subdivision of trimmed surfaces may result in only one surface returned   */
/* as the other is completely trimmed away. This macros should be used to    */
/* define and identify the two parts.					     */
#define TRIM_IS_FIRST_SRF(Srf)		(((Srf) -> Tags & 0x01) == 0)
#define TRIM_IS_SECOND_SRF(Srf)		(((Srf) -> Tags & 0x01) == 1)
#define TRIM_SET_FIRST_SRF(Srf)		((Srf) -> Tags &= ~0x01)
#define TRIM_SET_SECOND_SRF(Srf)	((Srf) -> Tags |= 0x01)


typedef struct TrimIsoInterStruct {      /* Holds intersections of iso curve */
    struct TrimIsoInterStruct *Pnext;		    /* with trimming curves. */
    CagdRType Param;
} TrimIsoInterStruct;

typedef void (*TrimSetErrorFuncType)(TrimFatalErrorType ErrorFunc);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

TrimCrvSegStruct *TrimCrvSegNew(CagdCrvStruct *UVCrv, CagdCrvStruct *EucCrv);
TrimCrvSegStruct *TrimCrvSegNewList(CagdCrvStruct *UVCrvs,
				    CagdCrvStruct *EucCrvs);
TrimCrvSegStruct *TrimCrvSegCopy(const TrimCrvSegStruct *TrimCrvSeg);
TrimCrvSegStruct *TrimCrvSegCopyList(const TrimCrvSegStruct *TrimCrvSegList);
void TrimCrvSegFree(TrimCrvSegStruct *TrimCrvSeg);
void TrimCrvSegFreeList(TrimCrvSegStruct *TrimCrvSegList);

#ifdef DEBUG
#define TrimCrvSegFree(TrimCrvSeg)         { TrimCrvSegFree(TrimCrvSeg); \
					     TrimCrvSeg = NULL; }
#define TrimCrvSegFreeList(TrimCrvSegList) { TrimCrvSegFreeList(TrimCrvSegList); \
					     TrimCrvSegList = NULL; }
#endif /* DEBUG */

TrimCrvStruct *TrimCrvNew(TrimCrvSegStruct *TrimCrvSegList);
TrimCrvStruct *TrimCrvCopy(const TrimCrvStruct *TrimCrv);
TrimCrvStruct *TrimCrvCopyList(const TrimCrvStruct *TrimCrvList);
void TrimCrvFree(TrimCrvStruct *TrimCrv);
void TrimCrvFreeList(TrimCrvStruct *TrimCrvList);

#ifdef DEBUG
#define TrimCrvFree(TrimCrv)         { TrimCrvFree(TrimCrv); TrimCrv = NULL; }
#define TrimCrvFreeList(TrimCrvList) { TrimCrvFreeList(TrimCrvList); \
				       TrimCrvList = NULL; }
#endif /* DEBUG */

TrimSrfStruct *TrimSrfNew(CagdSrfStruct *Srf,
			  TrimCrvStruct *TrimCrvList,
			  CagdBType HasTopLvlTrim);
TrimSrfStruct *TrimSrfNew2(CagdSrfStruct *Srf,
			   CagdCrvStruct *TrimCrvList,
			   CagdBType HasTopLvlTrim);
TrimSrfStruct *TrimSrfNew3(CagdSrfStruct *Srf,
			   CagdCrvStruct *TrimCrvList,
			   CagdBType HasTopLvlTrim);
TrimSrfStruct *TrimSrfFromE3TrimmingCurves(TrimCrvStruct *TCrvs,
					   const IrtPlnType Plane);
int TrimSrfVerifyTrimCrvsValidity(TrimSrfStruct *TrimSrf);
TrimSrfStruct *TrimSrfCopy(const TrimSrfStruct *TrimSrf);
TrimSrfStruct *TrimSrfCopyList(const TrimSrfStruct *TrimSrfList);
void TrimSrfFree(TrimSrfStruct *TrimSrf);
void TrimSrfFreeList(TrimSrfStruct *TrimSrfList);

#ifdef DEBUG
#define TrimSrfFree(TrimSrf)         { TrimSrfFree(TrimSrf); TrimSrf = NULL; }
#define TrimSrfFreeList(TrimSrfList) { TrimSrfFreeList(TrimSrfList); \
				       TrimSrfList = NULL; }
#endif /* DEBUG */

void TrimSrfTransform(TrimSrfStruct *TrimSrf,
		      CagdRType *Translate,
		      CagdRType Scale);
void TrimSrfMatTransform(TrimSrfStruct *TrimSrf, CagdMType Mat);
CagdBType TrimSrfsSame(const TrimSrfStruct *TSrf1,
		       const TrimSrfStruct *TSrf2,
		       CagdRType Eps);
TrimSrfStruct *TrimGetLargestTrimmedSrf(TrimSrfStruct **TSrfs, int Extract);
const TrimCrvSegStruct *TrimGetOuterTrimCrv(const TrimSrfStruct *TSrf);
const TrimCrvSegStruct *TrimGetFullDomainTrimCrv(const TrimSrfStruct *TSrf);
CagdCrvStruct *TrimGetTrimmingCurves(const TrimSrfStruct *TrimSrf,
				     CagdBType ParamSpace,
				     CagdBType EvalEuclid);
CagdCrvStruct *TrimGetTrimmingCurves2(const TrimCrvStruct *TrimCrvList,
				      const TrimSrfStruct *TrimSrf,
				      CagdBType ParamSpace,
				      CagdBType EvalEuclid);
TrimSrfStruct *TrimManageTrimmingCurves(TrimSrfStruct *TrimSrf,
					int FitOrder,
					CagdBType EvalEuclid);

TrimCrvStruct *TrimLinkTrimmingCurves2Loops(const TrimCrvStruct *TCrvs,
					    CagdRType MaxTol,
					    CagdBType *ClosedLoops);
TrimCrvStruct *TrimLinkTrimmingCurves2Loops1(const TrimCrvSegStruct *TSegs,
					    CagdRType MaxTol,
					    CagdBType *ClosedLoops);
TrimCrvStruct *TrimLinkTrimmingCurves2Loops2(TrimCrvStruct *TCrvs,
					     CagdRType Tol,
					     CagdBType *ClosedLoops);

void TrimClassifyTrimCrvsOrientation(TrimCrvStruct *TCrvs, CagdRType Tol);
CagdBType TrimVerifyClosedTrimLoop(TrimCrvStruct *TCrv,
				   CagdRType Tolerance,
				   CagdBType CoerceIdentical);
int TrimCoerceTrimUVCrv2Plane(TrimCrvSegStruct *TSeg);

TrimCrvStruct *TrimMergeTrimmingCurves2Loops(TrimCrvStruct *TrimCrvs);
CagdCrvStruct *TrimMergeTrimmingCurves2Loops2(CagdCrvStruct *UVCrvs,
					      CagdRType Tol);

void TrimAffineTransTrimCurves(TrimCrvStruct *TrimCrvList,
			       CagdRType OldUMin,
			       CagdRType OldUMax,
			       CagdRType OldVMin,
			       CagdRType OldVMax,
			       CagdRType NewUMin,
			       CagdRType NewUMax,
			       CagdRType NewVMin,
			       CagdRType NewVMax);
TrimSrfStruct *TrimAffineTransTrimSrf(const TrimSrfStruct *TrimSrf,
				      CagdRType NewUMin,
				      CagdRType NewUMax,
				      CagdRType NewVMin,
				      CagdRType NewVMax);
CagdPolylineStruct *TrimCrvs2Polylines(TrimSrfStruct *TrimSrf,
				       CagdBType ParamSpace,
				       CagdRType TolSamples,
				       SymbCrvApproxMethodType Method);
CagdPolylineStruct *TrimCrv2Polyline(const CagdCrvStruct *TrimCrv,
				     CagdRType TolSamples,
				     SymbCrvApproxMethodType Method,
				     CagdBType OptiLin);
CagdCrvStruct *TrimEvalTrimCrvToEuclid(const CagdSrfStruct *Srf,
				       const CagdCrvStruct *UVCrv);
CagdCrvStruct *TrimEvalTrimCrvToEuclid2(const CagdSrfStruct *Srf,
					const CagdCrvStruct *UVCrv,
					CagdCrvStruct **UVCrvLinear);
void TrimSrfFreeEuclideanTrimCrvs(TrimSrfStruct *TrimSrf);
int TrimSetEuclidLinearFromUV(int EuclidLinearFromUV);
int TrimSetEuclidComposedFromUV(int EuclidComposedFromUV);
CagdRType *TrimPointInsideTrimmedCrvsToData(TrimCrvStruct *TrimCrvList,
					    const TrimSrfStruct *TSrf,
					    CagdUVType UVRetVal);
CagdBType TrimSrfTrimCrvSquareDomain(const TrimCrvStruct *TrimCrvList,
				     CagdRType *UMin,
				     CagdRType *UMax,
				     CagdRType *VMin,
				     CagdRType *VMax);
CagdBType TrimSrfTrimCrvAllDomain(const TrimSrfStruct *TrimSrf);
TrimSrfStruct *TrimClipSrfToTrimCrvs(TrimSrfStruct *TrimSrf);
TrimSrfStruct *TrimSrfDegreeRaise(const TrimSrfStruct *TrimSrf,
				  CagdSrfDirType Dir);
int TrimSrfSetStateTrimCrvsManagement(int TrimmingFitOrder);
TrimSrfStruct *TrimSrfSubdivAtParam(TrimSrfStruct *TrimSrf,
				    CagdRType t,
				    CagdSrfDirType Dir);
TrimSrfStruct *TrimSrfSubdivAtInnerLoops(TrimSrfStruct *TSrf);
TrimCrvStruct *TrimSrfSubdivTrimCrvsAtInnerLoops(const TrimCrvStruct *TCrvs);
CagdSrfDirType TrimSrfSubdivValAtInnerLoop(const TrimCrvStruct *TCrvs,
					   CagdRType *SubdivVal);

TrimSrfStruct *TrimCnvrtBsp2BzrSrf(TrimSrfStruct *TrimSrf);
TrimSrfStruct *TrimSrfCnvrt2BzrTrimSrf(TrimSrfStruct *TrimSrf);
CagdSrfStruct *TrimSrfCnvrt2BzrRglrSrf(TrimSrfStruct *TrimSrf);
CagdSrfStruct *TrimSrfCnvrt2BzrRglrSrf2(const TrimSrfStruct *TSrf,
					int ComposeE3,
					int OnlyBzrSrfs,
					CagdRType Eps);
CagdSrfStruct *TrimSrfCnvrt2TensorProdSrf(const TrimSrfStruct *TSrf,
					  int ComposeE3,
					  CagdRType Eps);

int TrimSrfSubdivTrimmingCrvs(const TrimCrvStruct *TrimCrvs,
			      CagdRType t,
			      CagdSrfDirType Dir,
			      TrimCrvStruct **TrimCrvs1,
			      TrimCrvStruct **TrimCrvs2);
TrimSrfStruct *TrimSrfRegionFromTrimSrf(TrimSrfStruct *TrimSrf,
					CagdRType t1,
					CagdRType t2,
					CagdSrfDirType Dir);
TrimSrfStruct *TrimSrfRefineAtParams(const TrimSrfStruct *Srf,
				     CagdSrfDirType Dir,
				     CagdBType Replace,
				     CagdRType *t,
				     int n);
TrimSrfStruct *TrimSrfReverse(const TrimSrfStruct *TrimSrf);
TrimSrfStruct *TrimSrfReverse2(const TrimSrfStruct *TrimSrf);

int TrimRemoveCrvSegTrimCrvs(TrimCrvSegStruct *TrimCrvSeg,
			     TrimCrvStruct **TrimCrvs);
int TrimRemoveCrvSegTrimCrvSegs(TrimCrvSegStruct *TrimCrvSeg,
				TrimCrvSegStruct **TrimCrvSegs);

void TrimSrfDomain(const TrimSrfStruct *TrimSrf,
		   CagdRType *UMin,
		   CagdRType *UMax,
		   CagdRType *VMin,
		   CagdRType *VMax);
#define TrimSrfSetDomain TrimAffineTransTrimSrf

int TrimCrvSegBBox(const TrimCrvSegStruct *TCrvSeg,
		   int UV,
		   CagdBBoxStruct *BBox);
int TrimCrvSegListBBox(const TrimCrvSegStruct *TCrvSegs,
		       int UV,
		       CagdBBoxStruct *BBox);

int TrimCrvBBox(const TrimCrvStruct *TCrv, int UV, CagdBBoxStruct *BBox);
int TrimCrvListBBox(const TrimCrvStruct *TCrvs, int UV, CagdBBoxStruct *BBox);

CagdBBoxStruct *TrimSrfBBox(const TrimSrfStruct *TSrf, CagdBBoxStruct *BBox);
CagdBBoxStruct *TrimSrfListBBox(const TrimSrfStruct *TSrfs, CagdBBoxStruct *BBox);

int TrimSrfNumOfTrimLoops(const TrimSrfStruct *TSrf);
int TrimSrfNumOfTrimCrvSegs(const TrimSrfStruct *TSrf);

void TrimSrfEvalToData(const TrimSrfStruct *TrimSrf,
		       CagdRType u,
		       CagdRType v,
		       CagdRType *Pt);
CagdRType *TrimSrfEvalMalloc(const TrimSrfStruct *TrimSrf,
			     CagdRType u,
			     CagdRType v);

CagdCrvStruct *TrimSrf2Curves(TrimSrfStruct *TrimSrf, 
			      int NumOfIsocurves[2]);
CagdCrvStruct *TrimCrvTrimParamList(CagdCrvStruct *Crv,
				    TrimIsoInterStruct *InterList);
TrimIsoInterStruct **TrimIntersectTrimCrvIsoVals(const TrimSrfStruct *TrimSrf,
						 int Dir,
						 CagdRType *OrigIsoParams,
						 int NumOfIsocurves,
						 CagdBType Perturb);
TrimIsoInterStruct **TrimIntersectCrvsIsoVals(const CagdCrvStruct *UVCrvs,
					      int Dir,
					      CagdRType *IsoParams,
					      int NumOfIsocurves);
CagdCrvStruct *TrimCrvAgainstTrimCrvs(CagdCrvStruct *UVCrv,
				      const TrimSrfStruct *TrimSrf,
				      CagdRType Eps);
CagdPolylineStruct *TrimSrf2Polylines(TrimSrfStruct *TrimSrf,
				      int NumOfIsocurves[2],
				      CagdRType TolSamples,
				      SymbCrvApproxMethodType Method);
CagdPolygonStruct *TrimSrfAdap2Polygons(const TrimSrfStruct *TrimSrf,
					CagdRType Tolerance,
					CagdBType ComputeNormals,
					CagdBType ComputeUV);
struct IPPolygonStruct *TrimCrvsHierarchy2Polys(TrimCrvStruct *TrimLoops);
void TrimMake2ndCrvSameLengthAs1stCrv(const CagdCrvStruct *Crv1,
				      CagdCrvStruct **Crv2);
void TrimCrvSegReverse(TrimCrvSegStruct *TSeg);
TrimCrvSegStruct *TrimCrvSegListReverse(TrimCrvSegStruct *TSegs);
TrimCrvSegStruct *TrimOrderTrimCrvSegsInLoop(TrimCrvSegStruct *TSegs);
CagdBType TrimClassifyTrimmingLoops(TrimCrvStruct **TrimLoops);
CagdBType TrimClassifyTrimLoopOrient(const TrimCrvSegStruct *TSegs);
void TrimCrvFreeListWithSubTrims(TrimCrvStruct *TrimCrv);
void TrimCrvFreeWithSubTrims(TrimCrvStruct *TrimCrv);
CagdBType TrimClassifyTrimCurveOrient(const CagdCrvStruct *UVCrv);
CagdPolygonStruct *TrimSrf2Polygons2(const TrimSrfStruct *Srf,
				     int FineNess, 
				     CagdBType ComputeNormals,
				     CagdBType ComputeUV);
int TrimSetNumTrimVrtcsInCell(int NumTrimVrtcsInCell);
SymbCrvApproxMethodType TrimSetTrimCrvLinearApprox(CagdRType UVTolSamples,
					   SymbCrvApproxMethodType UVMethod);
CagdRType TrimGetTrimCrvLinearApprox(void);

TrimSrfStruct *TrimSrfsFromContours(const CagdSrfStruct *Srf,
				    const struct IPPolygonStruct *Cntrs);
TrimSrfStruct *TrimSrfsFromContours2(const CagdSrfStruct *Srf,
				     const CagdCrvStruct *CCntrs);
struct IPPolygonStruct *TrimValidateNewTrimCntrs(const CagdSrfStruct *Srf,
						 const struct IPPolygonStruct
						                      *Cntrs);
int TrimLoopWeightRelationInside(CagdRType V1, CagdRType V2, CagdRType V);
CagdRType TrimLoopUV2Weight(const IrtRType *UV,
			    IrtRType *BndryUV,
			    CagdRType UMin,
			    CagdRType UMax,
			    CagdRType VMin,
			    CagdRType VMax,
			    CagdBType Last);
CagdRType *TrimLoopWeight2UV(CagdRType Wgt,
			     CagdRType UMin,
			     CagdRType UMax,
			     CagdRType VMin,
			     CagdRType VMax,
			     CagdUVType UV);

TrimSrfStruct *TrimSrfsFromTrimPlsHierarchy(struct IPPolygonStruct *TopLevel,
					    struct IPPolygonStruct *TrimPls,
					    const CagdSrfStruct *Srf);
TrimCrvStruct *TrimPolylines2LinTrimCrvs(const struct IPPolygonStruct *Polys);

CagdBType TrimIsPointInsideTrimSrf(const TrimSrfStruct *TrimSrf,
				   CagdUVType UV);
CagdBType TrimIsPointInsideTrimCrvs(const TrimCrvStruct *TrimCrvs,
				    CagdUVType UV);
int TrimIsPointInsideTrimUVCrv(const CagdCrvStruct *UVCrv,
			       CagdUVType UV);

TrimSetErrorFuncType TrimSetFatalErrorFunc(TrimSetErrorFuncType ErrorFunc);
const char *TrimDescribeError(TrimFatalErrorType ErrorNum);
void TrimFatalError(TrimFatalErrorType ErrID);

void TrimDbg(const void *Obj);
void TrimDbgTCrvs(const TrimCrvStruct *TrimCrv);
void TrimDbgTCrvSegs(const TrimCrvSegStruct *TrimSegs);

/******************************************************************************
* Routines to handle layout (prisa) of trimmed surfaces.		      *
******************************************************************************/
TrimSrfStruct *TrimAllPrisaSrfs(const TrimSrfStruct *TSrfs,
				int SamplesPerCurve,
			        CagdRType Epsilon,
				CagdSrfDirType Dir,
			        CagdVType Space);
TrimSrfStruct *TrimPiecewiseRuledSrfApprox(const TrimSrfStruct *TSrf,
					   CagdBType ConsistentDir,
					   CagdRType Epsilon,
					   CagdSrfDirType Dir);
TrimSrfStruct *TrimPrisaRuledSrf(const TrimSrfStruct *TSrf,
				 int SamplesPerCurve,
				 CagdRType Space,
				 CagdVType Offset,
				 CagdSrfDirType Dir);

/******************************************************************************
* Untrimming trimmed surfaces.                                                *
******************************************************************************/
TrimUntrimResultStruct *TrimUntrimTrimSrf(TrimSrfStruct *TSrf,
					  CagdQuadSrfWeightFuncType WeightFunc,
					  CagdBType Compose,
					  int ApproxOrder);
CagdBType TrimUntrimSetLineSweepOutputCrvPairs(CagdBType NewValue);
struct IPObjectStruct *TrimUntrimmingResultToObj(
				      const TrimUntrimResultStruct *Untrimmed);
void TrimUntrimmingResultFree(TrimUntrimResultStruct *Untrim);
void TrimUntrimmingResultFreeList(TrimUntrimResultStruct *Untrim);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_TRIM_LIB_H */
