/******************************************************************************
* VMdl_lib.h - header file for the V-Model library.			      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Fady Massarwi and Gershon Elber   July, 2016		      *
******************************************************************************/

#ifndef IRIT_VOL_MDL_H
#define IRIT_VOL_MDL_H

#include "phashmap.h"
#include "cagd_lib.h"
#include "triv_lib.h"
#include "trim_lib.h"

typedef enum VMdlInterBndryType {
    VMDL_INTER_UMIN = 0,
    VMDL_INTER_UMAX,
    VMDL_INTER_VMIN,
    VMDL_INTER_VMAX,
    VMDL_INTER_WMIN,
    VMDL_INTER_WMAX,
    VMDL_INTER_INTERNAL
} VMdlInterBndryType;

typedef enum VMdlBoolOpType {
    VMDL_BOOL_OP_INTERSECTION = 0,
    VMDL_BOOL_OP_UNION,
    VMDL_BOOL_OP_SUBTRACTION
} VMdlBoolOpType;

struct VMdlInterTrimCurveSegStruct;
struct VMdlInterTrimCurveSegRefStruct;
struct VMdlInterTrimSrfStruct;
struct VMdlInterTrimSrfRefStruct;
struct VMdlVolumeElementStruct;
struct VMdlVolumeElementRefStruct;

/* A generic reference structure, to be used in overloaded operations. */
typedef struct VMdlGenericRefStruct {
    struct VMdlGenericRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    void *ObjRef;
} VMdlGenericRefStruct;

/* Intersection point - holds information about trimmed intersecting      */
/* curves. From curves it is possible to reach the intersecting surfaces. */
typedef struct VMdlInterTrimPointStruct  {
    struct VMdlInterTrimPointStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* curves that pass through this point. */
    struct VMdlInterTrimCurveSegRefStruct *InterCurveSegRefList; 
    CagdPType E3Pnt;
} VMdlInterTrimPointStruct;

/* List of intersection points. */
typedef struct VMdlInterTrimPointRefStruct  {
    struct VMdlInterTrimPointRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    VMdlInterTrimPointStruct *Obj;
} VMdlInterTrimPointRefStruct;

typedef struct VMdlInterTrimCurveSegStruct  {
    struct VMdlInterTrimCurveSegStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* List of trimmed surfaces sharing this boundary, Typically  */
    /* two surfaces.                                              */
    struct VMdlInterTrimSrfRefStruct *IncidentSrfsRefList;

    /* The actual trimmed curve. */
    /* Euclidean curve */
    CagdCrvStruct *TrimSeg;
    /*MdlTrimSegStruct *TrimSeg; TODO - MdlTrimSegStruct or CagdCrvStruct */

    /* Start & End intersection points. If the end point is NULL, then */
    /* the curve seg. is closed.                                       */
    VMdlInterTrimPointRefStruct *StartPntRef;
    VMdlInterTrimPointRefStruct *EndPntRef;
} VMdlInterTrimCurveSegStruct;

/* Add a bit that states that the curve is a boundary or from SSI. */

/* Trimming loops in one trimmed surface, including the boundary loop. */
typedef struct VMdlInterTrimCurveSegLoopInSrfStruct {
    /* Next curve segment in loop. */
    struct VMdlInterTrimCurveSegLoopInSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* The reference curve. */
    struct VMdlInterTrimCurveSegRefStruct *CrvRef;
    /* Next curve segment in loop. */
    /* struct VMdlInterTrimCurveSegStruct *PnextCrv; */
    /* Previous curve segment in loop. */
    struct VMdlInterTrimCurveSegLoopInSrfStruct *PprevCrv;    
    /* Surface holding this trimming loop. */
    struct VMdlInterTrimSrfRefStruct *IncidentSrfRef;
    
    /* The trimming curve in UV space of this Incident Srf. */
    CagdCrvStruct *UVCrv;
    CagdBType IsCrvInverted;
} VMdlInterTrimCurveSegLoopInSrfStruct;

/* Trimmed surface, a boundary face of volumetric element */
typedef struct VMdlInterTrimSrfStruct {
    struct VMdlInterTrimSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* The volumetric element this is one of its faces. */
    struct VMdlVolumeElementRefStruct *TrimmedTVRef;  
    /* The adjacent Srf to this one.  Can be NULL if none. */
    struct VMdlInterTrimSrfRefStruct *OppositeSrfRef;   
    /* List of trimming loops. */
    VMdlInterTrimCurveSegLoopInSrfStruct **BoundaryTrimmingCurveLoops;
    unsigned int NumOfBoundaryLoops;
    /* The real surface geometry. */
    CagdSrfStruct *UVWSrf;
    VMdlInterBndryType BndryType;
} VMdlInterTrimSrfStruct;

/* List of trimmed surfaces. */
typedef struct VMdlInterTrimSrfRefStruct {
    struct VMdlInterTrimSrfRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    VMdlInterTrimSrfStruct *Obj;
} VMdlInterTrimSrfRefStruct;

/* List of trivariates. */
typedef struct VMdlInterTrivTVRefStruct {
    struct VMdlInterTrivTVRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    TrivTVStruct *Obj; 
} VMdlInterTrivTVRefStruct;

typedef struct VMdlInterTrimCurveSegRefStruct {
    struct VMdlInterTrimCurveSegRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    VMdlInterTrimCurveSegStruct *Obj;
} VMdlInterTrimCurveSegRefStruct;

/* Volumetric element, encapsulates the minimal volumetric entity. */
typedef struct VMdlVolumeElementStruct {
    struct VMdlVolumeElementStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* Boundary surfaces. */
    VMdlInterTrimSrfRefStruct *BoundarySrfRefList;
    /* Associated trimmed curve segments. Maybe redundancy !? */
    VMdlInterTrimCurveSegRefStruct *TrimCurveSegRefList;
    /* All trimming curves' end points. Maybe redundancy !? */
    VMdlInterTrimPointRefStruct *TrimPointRefList;
    /* All TVs that their intersection created this element. */
    VMdlInterTrivTVRefStruct  *TVRefList;
    /* A 2D model that represents the boundaries of this element. */
    MdlModelStruct *__BoundaryModel;
} VMdlVolumeElementStruct;

typedef struct VMdlVolumeElementRefStruct {
    struct VMdlVolumeElementRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    VMdlVolumeElementStruct *Obj;
} VMdlVolumeElementRefStruct;

/* Volumetric model. */
typedef struct VMdlVModelStruct {
    struct VMdlVModelStruct *Pnext;
    struct IPAttributeStruct *Attr;
    /* List of volumetric elements in the model. */
    VMdlVolumeElementStruct *VolumeElements;
    /* All surfaces in the model in the model. */
    VMdlInterTrimSrfStruct *InterSrfList;
    /* All trimming curve segments in the model. */
    VMdlInterTrimCurveSegStruct *InterCurveSegList;  
    /* All trimming curves end points in the model. */
    VMdlInterTrimPointStruct *InterPointList;
    /* All TV's in the entire model in the model. */
    TrivTVStruct *TVList;
} VMdlVModelStruct;

/* An object that describes blending function between attributes */
typedef struct VMdlAttribBlendObj {
    /* TBD */
    int Dummy;
} VMdlAttribBlendObj;

typedef struct VMdlSlicerParamsStruct {
    CagdRType ZRes;       /* Z size of a single pixel. */
    CagdRType XYRes[2];   /* XY size of a single pixel. */

    CagdRType Max[2];     /* XY bounding box of domain to slice. */
    CagdRType Min[2];
} VMdlSlicerParamsStruct;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

VMdlVModelStruct *VMdlVModelCopy(const VMdlVModelStruct *VMdl);
VMdlVModelStruct *VMdlVModelCopyList(const VMdlVModelStruct *Mdls);

void VMdlVModelFree(VMdlVModelStruct *Mdl);
void VMdlVModelFreeList(VMdlVModelStruct *VMdls);

VMdlVModelStruct *VMdlGlueVModels(VMdlVModelStruct *Mdl1,
				  VMdlVModelStruct *Mdl2);
CagdBType VMdlGlueVModelsAppend(VMdlVModelStruct **Mdl1, 
				const VMdlVModelStruct *Mdl2, 
				CagdRType SrfDiffEps,
				CagdBType CalculateConnectivity,
				MiscPHashMap TVHMap);
void VMdlCalcMdlEuclCrvs(MdlModelStruct *BMdl);

/******************************* Debugging operations ***********************/

#ifdef DEBUG
void VMdlDbg(void *Obj);
#endif /* DEBUG */

void VMdlPrintVModel(VMdlVModelStruct *VM);
void VMdlPrintVE(VMdlVModelStruct *VM);
void VMdlSplitVModelInDir(VMdlVModelStruct *VM,
			  CagdRType Dx,
			  CagdRType Dy,
			  CagdRType Dz);

/********************************** Alloc and free **************************/

VMdlVModelStruct *VMdlAllocVModel();
VMdlVolumeElementStruct *VMdlAllocTrimVolumeElem();
VMdlVolumeElementRefStruct *VMdlAllocTrimVolumeElemRef();
VMdlInterTrimPointStruct *VMdlAllocTrimInterPoint();
VMdlInterTrimSrfStruct *VMdlAllocInterTrimSrf();
VMdlInterTrimCurveSegStruct *VMdlAllocInterTrimCurveSeg();
VMdlInterTrimCurveSegRefStruct *VMdlAllocInterTrimCurveSegRef();
VMdlInterTrivTVRefStruct *VMdlAllocInterTrivTVRef();
VMdlInterTrimSrfRefStruct *VMdlAllocInterTrimSrfRef();
VMdlInterTrimCurveSegLoopInSrfStruct *VMdlAllocInterTrimCurveSegLoopInSrf();
VMdlInterTrimPointRefStruct *VMdlAllocInterTrimPointRef();

VMdlVModelStruct *VMdlVModelFromVElement(const VMdlVolumeElementStruct *VElem, 
					 CagdBType UseVElemInPlace);

void VMdlFreeTrimVolElem(VMdlVolumeElementStruct *TrimVolElem);
void VMdlFreeTrimVolumeElemRef(VMdlVolumeElementRefStruct *TrimVolElemRef);
void VMdlFreeInterTrimPnt(VMdlInterTrimPointStruct *IntJunctionList);
void VMdlFreeInterTrimSrf(VMdlInterTrimSrfStruct *TrimSrfList);
void VMdlFreeInterTrimCurveSeg(VMdlInterTrimCurveSegStruct *CurveSegList);
void VMdlFreeInterTrimCurveSegRef(VMdlInterTrimCurveSegRefStruct *CrvSegRef);
void VMdlFreeInterTrivTVRef(VMdlInterTrivTVRefStruct *TrivTVRef);
void VMdlFreeInterTrimSrfRef(VMdlInterTrimSrfRefStruct *SrfRef);
void VMdlFreeInterTrimCurveSegLoopInSrf(
			     VMdlInterTrimCurveSegLoopInSrfStruct *CrvSegLoop);
void VMdlFreeInterTrimPointRef(VMdlInterTrimPointRefStruct *Ref);

/********************************** queries *********************************/

CagdBType VMdlIsPointInsideVModel(const VMdlVModelStruct *VMdl,
				  const CagdPType *Pnt);
CagdBType VMdlIsUVWPnttInsideVModel(const VMdlVModelStruct *VMdl,
				    const CagdPType *UVWPnt);
CagdBType VMdlGetVModelEnclosedVolume(const VMdlVModelStruct *VMdl,
				      CagdRType *EnclosedVol);

CagdBBoxStruct *VMdlVModelBBox(const VMdlVModelStruct *VMdl,
			       CagdBBoxStruct *BBox);
CagdBBoxStruct *VMdlVModelListBBox(const VMdlVModelStruct *Mdls,
				   CagdBBoxStruct *CagdBbox);

MdlModelStruct *VMdlGetBoundaryVModel(const VMdlVModelStruct *Mdl);
struct IPObjectStruct *VMdlGetBoundarySurfaces2(const VMdlVModelStruct *Mdl);
MdlModelStruct *VMdlGetOuterBoundarySurfacesVModel(const VMdlVModelStruct *Mdl);
MdlModelStruct *VMdlGetBndryVElement(VMdlVolumeElementStruct *VCell);
CagdCrvStruct *VMdlGetBoundaryCurves(const VMdlVModelStruct *Mdl);
TrivTVStruct *VMdlIsNonTrimmedVModel(const VMdlVModelStruct *VMdl);
CagdBType VMdlIsTVBoundaryVSrf(const VMdlInterTrimSrfStruct *VSrf, 
			       const TrivTVStruct *TV, 
			       TrivTVDirType *IsoDir, 
			       CagdRType *IsoVal);

/************************** primitives **************************************/

VMdlVModelStruct *VMdlVolElementFromBoundaryModel(MdlModelStruct *InBMdl, 
						  VMdlInterTrivTVRefStruct 
						          *ElementTVsRefList);

VMdlVModelStruct *VMdlPrimBoxVMdl(CagdRType MinX,
				  CagdRType MinY,
				  CagdRType MinZ,
				  CagdRType MaxX,
				  CagdRType MaxY,
				  CagdRType MaxZ);
VMdlVModelStruct *VMdlPrimCubeSphereVMdl(const CagdVType Center,
					 CagdRType Radius,
					 CagdBType Rational,
					 CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimTorusVMdl(const CagdVType Center,
				    CagdRType MajorRadius,
				    CagdRType MinorRadius,
				    CagdBType Rational,
				    CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimConeVMdl(const CagdVType Center,
				   CagdRType Radius,
				   CagdRType Height,
				   CagdBType Rational,
				   CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimCone2VMdl(const CagdVType Center,
				    CagdRType MajorRadius,
				    CagdRType MinorRadius,
				    CagdRType Height,
				    CagdBType Rational,
				    CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimCylinderVMdl(const CagdVType Center,
				       CagdRType Radius,
				       CagdRType Height,
				       CagdBType Rational,
				       CagdRType InternalCubeEdge);
VMdlVModelStruct* VMdlRuledTrimSrf(const TrimSrfStruct *TSrf1,
				   const CagdSrfStruct *Srf2,
				   int OtherOrder,
				   int OtherLen);
VMdlVModelStruct *VMdlExtrudeTrimSrf(const TrimSrfStruct *Section,
				     CagdVecStruct *Dir);
VMdlVModelStruct *VMdlOfRevTrimSrf(const TrimSrfStruct *Section,
			    CagdRType StartAngle,
			    CagdRType EndAngle,
			    CagdBType PolyApprox);
VMdlVModelStruct *VMdlOfRevAxisTrimSrf(const TrimSrfStruct *Section,
				const TrivV4DType AxisPoint,
				const TrivV4DType AxisVector,
				CagdRType StartAngle,
				CagdRType EndAngle,
				CagdBType PolyApprox);

/***************************  operations ************************************/

CagdBType VMdlTrimVModelBySurface(const VMdlVModelStruct *VMdl,
				  const CagdSrfStruct *Srf);
CagdBType VMdlRemoveTrimmingSurface(const VMdlVModelStruct *VMdl,
				    const CagdSrfStruct *Srf);
void VMdlVModelTransform(VMdlVModelStruct *VMdl, IrtHmgnMatType Mat);
TrivTVStruct *VMdlFetchTrivar(const VMdlVolumeElementStruct *VMTrimmedTV);
TrimSrfStruct *VMdlFetchTrimmingSrfs(const VMdlVolumeElementStruct *VMTrimmedTV);
CagdBType VMdlVModelReplaceTV(VMdlVModelStruct *VMdl, 
			     TrivTVStruct *OldTV,
			     const TrivTVStruct *NewTV);

/*************************** Boolean operations *****************************/

VMdlVModelStruct *VMdlVModelIntersect(const VMdlVModelStruct *VMdlA,
				      const VMdlVModelStruct *VMdlB,
				      const VMdlAttribBlendObj *AttribBlendObj);
VMdlVModelStruct *VMdlVModelUnion(const VMdlVModelStruct *VMdlA,
				  const VMdlVModelStruct *VMdlB,
				  VMdlBoolOpType OpType,
				  const VMdlAttribBlendObj *AttrBlendObj);
VMdlVModelStruct *VMdlVModelSubtract(const VMdlVModelStruct *VMdlA,
				     const VMdlVModelStruct *VMdlB,
				     const VMdlAttribBlendObj *AttrBlendObj);
VMdlVModelStruct *VMdlVModelSymDiff(const VMdlVModelStruct *VMdlA,
				    const VMdlVModelStruct *VMdlB,
				    const VMdlAttribBlendObj *AttrBlendObj);
VMdlVModelStruct *VMdlVModelNegate(const VMdlVModelStruct *VMdl);
VMdlVModelStruct *VMdlVModelCut(const VMdlVModelStruct *VMdlA,
			        const VMdlVModelStruct *VMdlB,
			        const VMdlAttribBlendObj *AttrBlendObj);
VMdlVModelStruct *VMdlClipVModelByPlane(const VMdlVModelStruct *Mdl,
					const IrtPlnType Pln,
					VMdlBoolOpType BoolOp);

/************************** Subdivision *************************************/

VMdlVModelStruct *VMdlSubdivideVElement(VMdlVolumeElementStruct *VElem,
					const TrivTVStruct *TV,
					TrivTVDirType Dir,
					IrtRType t,
					IrtRType *OtherParamAttribVals,
					CagdBType HandleKnotLineIntersections);
VMdlVModelStruct *VMdlSubdivideVModel(VMdlVModelStruct *VMdl,
				      TrivTVDirType Dir,
				      IrtRType t);
VMdlVModelStruct *VMdlSubdivideVElemToBezierVElements(
					 const VMdlVolumeElementStruct *VElem,
					 const TrivTVStruct *TV);
VMdlVModelStruct *VMdlSubdivideVMdlToBezierVElements(
						const VMdlVModelStruct *VMdl);
TrivTVStruct *VMdlUntrimVModel(const VMdlVModelStruct *VMdl, 
			       const TrivTVStruct *TV,
			       const TrivTVStruct *OriginalTV,
			       CagdBType InParamSpace,
			       int InvSrfApproxOrder,
			       CagdRType InvApproxErr);

/************************** Properties **************************************/

IPAttributeStruct *VMdlGetPointVMdlAttribute(const VMdlVModelStruct *VMdl,
					     const CagdPType *UVW,
					     int AttributeID);

/*************************** Conversion functions ************************** */

VMdlVModelStruct *VMdlCnvrtSrf2VMdl(const CagdSrfStruct *Srf);
VMdlVModelStruct *VMdlCnvrtTrimmedSrf2VMdl(const TrimSrfStruct *TSrf);
VMdlVModelStruct *VMdlCnvrtTrivar2VMdl(const TrivTVStruct *TV);
VMdlVModelStruct *VMdlCnvrtTrivar2VMdlList(TrivTVStruct *TVList); 
TrimSrfStruct *VMdlCnvrtVMdls2TrimmedSrfs(const VMdlVModelStruct *VMdl);
TrimSrfStruct *VMdlCnvrtVMdl2TrimmedSrfs(const VMdlVModelStruct *VMdl);
MdlModelStruct *VMdlCnvrtVMdls2Mdls(const VMdlVModelStruct *VMdls);
MdlModelStruct *VMdlCnvrtVMdl2Mdl(const VMdlVModelStruct *VMdl);
void VMdlAddTrimSrfToVMdl(VMdlVModelStruct *VMdl,
			  const TrimSrfStruct *TSrf);
CagdBType VMdlSetSplitPeriodicTV(CagdBType Split);
VMdlVModelStruct *VMdlExtractVElements(const VMdlVModelStruct *VMdl);
/*************************** Volumetric slicing ************************** */

struct VMdlSlicerInfoStruct *VMdlSlicerInitVElement(
				       VMdlVolumeElementStruct *VolumeElement,
				       const VMdlSlicerParamsStruct *Params);
struct VMdlSlicerInfoStruct *VMdlSlicerInitTrivar(
				       const TrivTVStruct *Trivar, 
				       const VMdlSlicerParamsStruct *Params);
struct VMdlSlicerInfoStruct *VMdlSlicerInitTrivMdl(
				       const TrivTVStruct *Trivar,
				       const MdlModelStruct *BMdl,
				       const VMdlSlicerParamsStruct *Params);
void VMdlSlicerFree(struct VMdlSlicerInfoStruct *Info);
void VMdlSlicerSetCurrSliceZ(struct VMdlSlicerInfoStruct *Info, CagdRType z);
int VMdlSlicerGetCurrSliceXY(struct VMdlSlicerInfoStruct *Info,
			     int x,
			     int y,
			     CagdRType *Params,
			     CagdRType *Pos);
void VMdlSlicerNormParams(struct VMdlSlicerInfoStruct *Info,
			  CagdRType *Params);
void VMdlSlicerGetSliceSize(struct VMdlSlicerInfoStruct *Info, int *Size);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_VOL_MDL_H */
