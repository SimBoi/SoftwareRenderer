/******************************************************************************
* Mdl_lib.h - header file for the Model library.			      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber and Bogdanov Alexander   July, 1996		      *
******************************************************************************/

#ifndef IRIT_MDL_LIB_H
#define IRIT_MDL_LIB_H

#include "cagd_lib.h" 
#include "trim_lib.h" 

typedef enum {
    MDL_BOOL_UNKNOWN = 6000,
    MDL_BOOL_UNION,
    MDL_BOOL_INTERSECTION,
    MDL_BOOL_SUBTRACTION,
    MDL_BOOL_CUT,
    MDL_BOOL_INTER_CRVS
} MdlBooleanType;

typedef enum {
    MDL_ERR_NO_ERROR = 0,

    MDL_ERR_PTR_REF = 1000,
    MDL_ERR_TSEG_NO_SRF,
    MDL_ERR_BOOL_MERGE_FAIL,
    MDL_ERR_TSEG_NOT_FOUND,
    MDL_ERR_EXPECTED_LINEAR_TSEG,
    MDL_ERR_TSRF_NOT_FOUND,
    MDL_ERR_FP_ERROR,
    MDL_ERR_BOOL_DISJOINT,
    MDL_ERR_BOOL_GET_REF,
    MDL_ERR_BOOL_CLASSIFY_FAIL,
    MDL_ERR_BOOL_UVMATCH_FAIL,

    MDL_ERR_UNDEFINE_ERR
} MdlFatalErrorType;

struct MdlModelStruct;
struct MdlIntersectionCBStruct;
struct MvarSrfSrfInterCacheStruct;

typedef void *MdlIntersectionCBData;
typedef struct MvarPolylineStruct * (*MdlIntersectionCBFunc)(
 					   const CagdSrfStruct *Srf1,
					   const CagdSrfStruct *Srf2, 
					   CagdSrfStruct **ModifiedSrf1, 
					   CagdSrfStruct **ModifiedSrf2,
					   MdlIntersectionCBData InterCBData);
typedef void (*MdlPreIntersectionCBFunc)(struct MdlModelStruct *Mdl1,
				         struct MdlModelStruct *Mdl2,
				         MdlIntersectionCBData InterCBData);
typedef void (*MdlPostIntersectionCBFunc)(
				        struct MdlModelStruct *Mdl,
				        MdlIntersectionCBData InterCBData);
					
typedef struct MdlIntersectionCBStruct {
    MdlIntersectionCBFunc InterCBFunc;
    MdlPreIntersectionCBFunc PreInterCBFunc;
    MdlIntersectionCBData InterCBData;
    MdlPostIntersectionCBFunc PostInterCBFunc;
} MdlIntersectionCBStruct;

typedef struct MdlBopsParams {
    CagdBType PertubrateSecondModel;
    CagdBType ExtendUVSrfResult;
    /* If IntersectedSurfacesAttrib != NULL then each surface in both models */
    /* will have an integer attribute named IntersectedSurfacesAttrib that   */
    /* will have value of 1 if the surface participated is trimmed           */
    /* (intersects) as result of the boolean operation.			     */
    char *IntersectedSurfacesAttrib;
    MdlIntersectionCBStruct *SSICBData;
} MdlBopsParams;

typedef struct MdlTrimSegStruct {
    struct MdlTrimSegStruct *Pnext;
    struct IPAttributeStruct *Attr;
    struct MdlTrimSrfStruct *SrfFirst;
    struct MdlTrimSrfStruct *SrfSecond;
    CagdCrvStruct *UVCrvFirst;   /* Trim crv segment in srf's param. domain. */
    CagdCrvStruct *UVCrvSecond;  /* Trim crv segment in srf's param. domain. */
    CagdCrvStruct *EucCrv;       /* Trimming curve as an E3 Euclidean curve. */
    IrtBType Tags;
} MdlTrimSegStruct;

typedef struct MdlTrimSegRefStruct {
    struct MdlTrimSegRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSegStruct *TrimSeg;
    IrtBType Reversed;
    IrtBType Tags;
} MdlTrimSegRefStruct;

typedef struct MdlLoopStruct {
    struct MdlLoopStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSegRefStruct *SegRefList;
} MdlLoopStruct;

typedef struct MdlTrimSrfStruct {
    struct MdlTrimSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdSrfStruct *Srf;                /* Surface trimmed by MdlTrimSegList. */
    MdlLoopStruct *LoopList;
} MdlTrimSrfStruct;

typedef struct MdlModelStruct {
    struct MdlModelStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSrfStruct *TrimSrfList;
    MdlTrimSegStruct *TrimSegList;       /* List of trimming curve segments. */
} MdlModelStruct;

typedef void (*MdlSetErrorFuncType)(MdlFatalErrorType ErrorFunc);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

void MdlTrimSegFree(MdlTrimSegStruct *MTSeg);
void MdlTrimSegFreeList(MdlTrimSegStruct *MTSegList);
void MdlTrimSegRefFree(MdlTrimSegRefStruct *MTSegRef);
void MdlTrimSegRefFreeList(MdlTrimSegRefStruct *MTSegRefList);
void MdlLoopFree(MdlLoopStruct *MdlLoop);
void MdlLoopFreeList(MdlLoopStruct *MdlLoopList);
void MdlTrimSrfFree(MdlTrimSrfStruct *TrimSrf);
void MdlTrimSrfFreeList(MdlTrimSrfStruct *MdlTrimSrfList);
void MdlModelFree(MdlModelStruct *Model);
void MdlModelFreeList(MdlModelStruct *Model);


#ifdef DEBUG
#define MdlTrimSegFree(MTSeg)         { MdlTrimSegFree(MTSeg); MTSeg = NULL; }
#define MdlTrimSegFreeList(MTSegList) { MdlTrimSegFreeList(MTSegList); \
					MTSegList = NULL; }
#define MdlTrimSegRefFree(MTSegRef)   { MdlTrimSegRefFree(MTSegRef); \
					MTSegRef = NULL; }
#define MdlTrimSegRefFreeList(MTSegRefList) { MdlTrimSegRefFreeList(MTSegRefList); \
					MTSegRefList = NULL; }
#define MdlLoopFree(MdlLoop)          { MdlLoopFree(MdlLoop); MdlLoop = NULL; }
#define MdlTrimSrfFree(TrimSrf)       { MdlTrimSrfFree(TrimSrf); \
					TrimSrf = NULL; }
#define MdlTrimSrfFreeList(MdlTrimSrfList) { MdlTrimSrfFreeList(MdlTrimSrfList); \
					     MdlTrimSrfList = NULL; }
#define MdlModelFree(Model)           { MdlModelFree(Model); Model = NULL; }
#define MdlModelFreeList(Model)       { MdlModelFreeList(Model); Model = NULL; }
#endif /* DEBUG */

MdlTrimSegStruct *MdlTrimSegCopy(const MdlTrimSegStruct *MdlTrimSeg,
				 const MdlTrimSrfStruct *TrimSrfList);
MdlTrimSegStruct *MdlTrimSegCopyList(const MdlTrimSegStruct *MdlTrimSegList,
				     const MdlTrimSrfStruct *TrimSrfList);
MdlTrimSegRefStruct *MdlTrimSegRefCopy(const MdlTrimSegRefStruct *SegRefList,
				       const MdlTrimSegStruct *TrimSegList);
MdlTrimSegRefStruct *MdlTrimSegRefCopyList(const MdlTrimSegRefStruct *SegRefList,
					   const MdlTrimSegStruct *TrimSegList);
MdlLoopStruct *MdlLoopNew(MdlTrimSegRefStruct *MdlTrimSegRefList);
MdlLoopStruct *MdlLoopCopy(const MdlLoopStruct *MdlLoop, 
			   const MdlTrimSegStruct *TrimSegList);
MdlLoopStruct *MdlLoopCopyList(const MdlLoopStruct *MdlLoopList, 
			       const MdlTrimSegStruct *TrimSegList);

MdlTrimSrfStruct *MdlTrimSrfNew(CagdSrfStruct *Srf,
				MdlLoopStruct *LoopList, 
				CagdBType HasTopLvlTrim,
				CagdBType UpdateBackTSrfPtrs);
MdlTrimSrfStruct *MdlTrimSrfNew2(CagdSrfStruct *Srf,
	 		         CagdCrvStruct **LoopList,
				 int NumLoops,
			         CagdBType HasTopLvlTrim);
MdlTrimSrfStruct *MdlTrimSrfCopy(const MdlTrimSrfStruct *MdlTrimSrf, 
				 const MdlTrimSegStruct *TrimSegList);
MdlTrimSrfStruct *MdlTrimSrfCopyList(const MdlTrimSrfStruct *MdlTrimSrfList, 
				     const MdlTrimSegStruct *TrimSegList);

MdlTrimSegStruct *MdlTrimSegNew(CagdCrvStruct *UVCrv1,
				CagdCrvStruct *UVCrv2,
                                CagdCrvStruct *EucCrv1,
                                MdlTrimSrfStruct *SrfFirst,
                                MdlTrimSrfStruct *SrfSecond);
int MdlTrimSegRemove(const MdlTrimSegStruct *TSeg, MdlTrimSegStruct **SegList);
int MdlTrimSegRemove2(MdlTrimSegStruct *TSeg, MdlModelStruct *Mdl);

MdlTrimSegRefStruct *MdlTrimSegRefNew(MdlTrimSegStruct *MdlTrimSeg);
int MdlTrimSegRefRemove(const MdlTrimSegStruct *TSeg,
			MdlTrimSegRefStruct **TSegRefList,
			int FreeRef);
int MdlTrimSegRefRemove2(const MdlTrimSegStruct *TSeg,
			 MdlLoopStruct *Loops,
			 int FreeRef);
MdlTrimSegRefStruct *MdlGetOtherSegRef(const MdlTrimSegRefStruct *SegRef,
				       const MdlTrimSrfStruct *TSrf);
MdlTrimSegRefStruct *MdlGetOtherSegRef2(const MdlTrimSegRefStruct *SegRef,
					const MdlTrimSrfStruct *TSrf,
					MdlTrimSrfStruct **OtherTSrf,
					MdlLoopStruct **OtherLoop);
MdlTrimSegRefStruct *MdlGetSrfTrimSegRef(const MdlTrimSrfStruct *TSrf,
					 const MdlTrimSegStruct *TSeg);
int MdlGetModelTrimSegRef(const MdlModelStruct *Mdl,
			  const MdlTrimSegStruct *TSeg,
			  MdlTrimSegRefStruct **TSegRef1,
			  MdlTrimSrfStruct **TSrf1,
			  MdlTrimSegRefStruct **TSegRef2,
			  MdlTrimSrfStruct **TSrf2);

MdlTrimSegStruct *MdlTrimSrfChainTrimSegs(MdlTrimSrfStruct *TSrfs);

MdlModelStruct *MdlModelNew(CagdSrfStruct *Srf,
		            CagdCrvStruct **LoopList,
			    int NumLoops,
		            CagdBType HasTopLvlTrim);
MdlModelStruct *MdlModelNew2(MdlTrimSrfStruct *TrimSrfs,
			     MdlTrimSegStruct *TrimSegs);
MdlModelStruct *MdlModelCopy(const MdlModelStruct *Model);
MdlModelStruct *MdlModelCopyList(const MdlModelStruct *ModelList);
void MdlModelTransform(MdlModelStruct *Model,
		       const CagdRType *Translate,
		       CagdRType Scale);
void MdlModelMatTransform(MdlModelStruct *Model, CagdMType Mat);
CagdBType MdlModelsSame(const MdlModelStruct *Model1,
		        const MdlModelStruct *Model2,
		        CagdRType Eps);

void MdlPatchTrimmingSegPointers(MdlModelStruct *Model);
IritIntPtrSizeType MdlGetLoopSegIndex(const MdlTrimSegRefStruct *TrimSeg,
				      const MdlTrimSegStruct *TrimSegList);
IritIntPtrSizeType MdlGetSrfIndex(const MdlTrimSrfStruct *Srf,
				  const MdlTrimSrfStruct *TrimSrfList);


/******************************************************************************
* Bounding boxes routines.						      *
******************************************************************************/

CagdBBoxStruct *MdlModelBBox(const MdlModelStruct *Mdl, CagdBBoxStruct *BBox);
CagdBBoxStruct *MdlModelListBBox(const MdlModelStruct *Mdls,
				 CagdBBoxStruct *BBox);
CagdBBoxStruct *MdlModelTSrfTCrvsBBox(const MdlTrimSrfStruct *TSrf,
				      CagdBBoxStruct *BBox);

/******************************************************************************
* Primitives routines.							      *
******************************************************************************/

int MdlTwoTrimSegsSameEndPts(const MdlTrimSegStruct *TSeg1,
			     const MdlTrimSegStruct *TSeg2,
			     CagdRType Tol);
CagdCrvStruct *MdlGetTrimmingCurves(const MdlTrimSrfStruct *TrimSrf,
				    CagdBType ParamSpace,
				    CagdBType EvalEuclid);
int MdlStitchModel(MdlModelStruct *Mdl, CagdRType StitchTol);
MdlModelStruct *MdlPrimPlane(CagdRType MinX,
			     CagdRType MinY,
			     CagdRType MaxX,
			     CagdRType MaxY,
			     CagdRType ZLevel);
MdlModelStruct *MdlPrimPlaneSrfOrderLen(CagdRType MinX,
					CagdRType MinY,
					CagdRType MaxX,
					CagdRType MaxY,
					CagdRType ZLevel,
					int Order,
					int Len);
MdlModelStruct *MdlPrimListOfSrfs2Model(CagdSrfStruct *Srfs, int *n);
MdlModelStruct *MdlPrimBox(CagdRType MinX,
			   CagdRType MinY,
			   CagdRType MinZ,
			   CagdRType MaxX,
			   CagdRType MaxY,
			   CagdRType MaxZ);
MdlModelStruct *MdlPrimSphere(const CagdVType Center,
			      CagdRType Radius,
			      CagdBType Rational);
MdlModelStruct *MdlPrimTorus(const CagdVType Center,
			     CagdRType MajorRadius,
			     CagdRType MinorRadius,
			     CagdBType Rational);
MdlModelStruct *MdlPrimCone2(const CagdVType Center,
			     CagdRType MajorRadius,
			     CagdRType MinorRadius,
			     CagdRType Height,
			     CagdBType Rational,
			     CagdPrimCapsType Caps);
MdlModelStruct *MdlPrimCone(const CagdVType Center,
			    CagdRType Radius,
			    CagdRType Height,
			    CagdBType Rational,
			    CagdPrimCapsType Caps);
MdlModelStruct *MdlPrimCylinder(const CagdVType Center,
				CagdRType Radius,
				CagdRType Height,
				CagdBType Rational,
				CagdPrimCapsType Caps);
int MdlStitchSelfSrfPrims(int Stitch);
int MdlCreateCubeSpherePrim(int CubeTopoSphere);

/******************************************************************************
* Boolean operations.							      *
******************************************************************************/

CagdCrvStruct *MdlExtructReversUVCrv(const MdlTrimSrfStruct *MdlSrf, 
				     const MdlTrimSegStruct *MdlSeg);

void MdlBooleanSetTolerances(CagdRType SubdivTol,
			     CagdRType NumerTol,
			     CagdRType TraceTol);

struct IPObjectStruct *MdlBooleanUnion(
				 const MdlModelStruct *Model1, 
				 const MdlModelStruct *Model2,
				 struct MvarSrfSrfInterCacheStruct *SSICache, 
				 MdlBopsParams *BopsParams);
struct IPObjectStruct *MdlBooleanIntersection(
				 const MdlModelStruct *Model1, 
				 const MdlModelStruct *Model2,
				 struct MvarSrfSrfInterCacheStruct *SSICache,
				 MdlBopsParams *BopsParams);
struct IPObjectStruct *MdlBooleanSubtraction(
				 const MdlModelStruct *Model1, 
				 const MdlModelStruct *Model2,
				 struct MvarSrfSrfInterCacheStruct *SSICache,
				 MdlBopsParams *BopsParams);
struct IPObjectStruct *MdlBooleanCut(
				 const MdlModelStruct *Model1,
				 const MdlModelStruct *Model2,
				 struct MvarSrfSrfInterCacheStruct *SSICache,
				 MdlBopsParams *BopsParams);
struct IPObjectStruct *MdlBooleanMerge(const MdlModelStruct *Model1,
				       const MdlModelStruct *Model2,
				       CagdBType StitchBndries);
MdlModelStruct *MdlBooleanMerge2(const MdlModelStruct *Model1,
				 const MdlModelStruct *Model2,
				 CagdBType StitchBndries);
CagdCrvStruct *MdlBooleanInterCrv(const MdlModelStruct *Model1,
				  const MdlModelStruct *Model2,
				  int InterType,
				  MdlModelStruct **InterModel,
				  MdlBopsParams *BopsParams);
int MdlBoolSetOutputInterCrv(int OutputInterCurve);
int MdlBoolSetOutputInterCrvType(int OutputInterCurveType);
int MdlBoolSetHandleInterDiscont(int HandleInterDiscont);
MdlModelStruct *MdlModelNegate(const MdlModelStruct *Model);

int MdlBoolCleanRefsToTSrf(MdlModelStruct *Model, MdlTrimSrfStruct *TSrf);
void MdlBoolResetAllTags(MdlModelStruct *Model);
int MdlBoolCleanUnusedTrimCrvsSrfs(MdlModelStruct *Model);
void MdlBoolClipTSrfs2TrimDomain(MdlModelStruct *Model, 
				 CagdBType ExtendSrfDomain);

CagdCrvStruct *MdlInterSrfByPlane(const CagdSrfStruct *Trf,
				  const IrtPlnType Pln);
TrimSrfStruct *MdlClipSrfByPlane(const CagdSrfStruct *Srf,
				 const IrtPlnType Pln);
TrimSrfStruct *MdlClipTrimmedSrfByPlane(const TrimSrfStruct *TSrf,
					const IrtPlnType Pln);
MdlModelStruct *MdlClipModelByPlane(const MdlModelStruct *Mdl,
				    const IrtPlnType Pln,
				    MdlBooleanType BoolOp);

struct MdlBopsParams *MdlBoolOpParamsAlloc(
                                    CagdBType PertubrateSecondModel,
				    CagdBType ExtendUVSrfResult,
				    const char *IntersectedSurfacesAttrib,
				    MdlIntersectionCBFunc InterCBFunc,
				    MdlPreIntersectionCBFunc PreInterCBFunc,
				    MdlPostIntersectionCBFunc PostInterCBFunc);
void MdlBoolOpParamsFree(struct MdlBopsParams *BopsParams);

/******************************************************************************
* Model maintenance routines.						      *
******************************************************************************/

int MdlSplitTrimCrv(MdlTrimSegStruct *Seg,
		    const CagdPtStruct *Pts,
		    int Idx,
		    CagdRType Eps,
		    int *Proximity);
MdlTrimSegStruct *MdlDivideTrimCrv(MdlTrimSegStruct *Seg,
				   const CagdPtStruct *Pts,
				   int Idx,
				   CagdRType Eps,
				   int *Proximity);
MdlTrimSegStruct *MdlFilterOutCrvs(MdlTrimSegStruct *TSegs);
CagdBType MdlIsPointInsideTrimSrf(const MdlTrimSrfStruct *TSrf,
				  CagdUVType UV);
int MdlIsPointInsideTrimLoop(const MdlTrimSrfStruct *TSrf,
			     const MdlLoopStruct *Loop,
			     CagdUVType UV);
CagdBType MdlIsPointInsideModel(CagdPType Pnt, const MdlModelStruct *Mdl);
int MdlIsLoopNested(const MdlLoopStruct *L, const MdlTrimSrfStruct *TSrf);
int MdlGetUVLocationInLoop(const MdlLoopStruct *L,
			   const MdlTrimSrfStruct *TSrf,
			   CagdUVType UV);
void MdlEnsureMdlTrimCrvsPrecision(MdlModelStruct *Mdl);
void MdlEnsureTSrfTrimCrvsPrecision(MdlTrimSrfStruct *MdlTrimSrf);
int MdlEnsureTSrfTrimLoopPrecision(MdlLoopStruct *Loop,
				   MdlTrimSrfStruct *MdlTrimSrf,
				   CagdRType Tol);
int MdlModelIsClosed(const MdlModelStruct *Model);
CagdPType *MdlGetTrimmingCurvesEndPts(MdlModelStruct *Mdl, int *N);

/******************************************************************************
* Conversion routines.							      *
******************************************************************************/

TrimSrfStruct *MdlCnvrtMdl2TrimmedSrfs(const MdlModelStruct *Model,
				       CagdRType TrimCrvStitchTol);
TrimSrfStruct *MdlCnvrtMdls2TrimmedSrfs(const MdlModelStruct *Models,
					CagdRType TrimCrvStitchTol);
MdlModelStruct *MdlCnvrtSrfs2Mdls(const CagdSrfStruct *Srfs);
MdlModelStruct *MdlCnvrtSrf2Mdl(const CagdSrfStruct *Srf);
MdlModelStruct *MdlCnvrtTrimmedSrfs2Mdls(const TrimSrfStruct *TSrfs);
MdlModelStruct *MdlCnvrtTrimmedSrf2Mdl(const TrimSrfStruct *TSrf);
MdlModelStruct *MdlAddSrf2Mdl(const MdlModelStruct *Mdl,
			      const CagdSrfStruct *Srf);
MdlModelStruct *MdlAddTrimmedSrf2Mdl(const MdlModelStruct *Mdl,
				     const TrimSrfStruct *TSrf);
CagdCrvStruct *MdlExtractUVCrv(const MdlTrimSrfStruct *Srf,
			       const MdlTrimSegStruct *Seg);
MdlModelStruct *MdlSplitDisjointComponents(const MdlModelStruct *Mdl);

/******************************************************************************
* Error handling.							      *
******************************************************************************/

MdlSetErrorFuncType MdlSetFatalErrorFunc(MdlSetErrorFuncType ErrorFunc);
void MdlFatalError(MdlFatalErrorType ErrID);
const char *MdlDescribeError(MdlFatalErrorType ErrID);

void MdlDbg(void *Obj);

#ifdef DEBUG
void MdlDbg2(void *Obj);
void MdlDbgDsp(void *Obj, CagdRType Trans, int Clear);
void MdlDbgDsp2(void *Obj, CagdRType Trans, int Clear);
int MdlDbgMC(const MdlModelStruct *Mdl, int Format);
int MdlDbgTC(const MdlTrimSegStruct *TSegs, 
	     const MdlTrimSrfStruct *TSrf,
	     int Format);
int MdlDbgSC(const MdlTrimSrfStruct *TSrf, int Format);
int MdlDbgRC(const MdlTrimSegRefStruct *Refs, int Format);
int MdlDbgRC2(const MdlTrimSegRefStruct *Refs,
	      const MdlTrimSrfStruct *TSrf,
	      int Format);

int MdlDebugHandleTCrvLoops(const MdlTrimSrfStruct *TSrf,
			    const MdlLoopStruct *Loops,
			    const CagdPType Trans,
			    int Display,
			    int TrimEndPts);
int MdlDebugHandleTSrfCrvs(const MdlTrimSegStruct *TCrvs,
			   const MdlTrimSrfStruct *TSrf,
			   const CagdPType Trans,
			   int Display,
			   int TrimEndPts);
int MdlDebugHandleTSrfRefCrvs(const MdlTrimSegRefStruct *Refs,
			      const MdlTrimSrfStruct *TSrf,
			      const CagdPType Trans,
			      int Loop,
			      int Display,
			      int TrimEndPts);
int MdlDebugWriteTrimSegs(const MdlTrimSegStruct *TSegs,
			  const MdlTrimSrfStruct *TSrf,
			  const CagdPType Trans);

int MdlDebugVerifyTrimSeg(const MdlTrimSegStruct *TSeg, int VerifyBackPtrs);
int MdlDebugVerifyTrimSegsArcLen(const MdlTrimSegStruct *TSegs);
int MdlDebugVerify(const MdlModelStruct *Model, int Complete, int TestLoops);
struct IPObjectStruct *MdlDebugVisual(const MdlModelStruct *Model,
		  		      CagdBType TCrvs,
				      CagdBType TSrfs,
				      CagdBType Srfs);
void MdlDbgVsl(const MdlModelStruct *Model,
	       CagdBType TCrvs,
	       CagdBType TSrfs,
	       CagdBType Srfs);
#endif /* DEBUG */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MDL_LIB_H */
