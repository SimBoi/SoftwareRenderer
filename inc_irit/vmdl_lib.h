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

#include "inc_irit/phashmap.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/triv_lib.h"
#include "inc_irit/trim_lib.h"

#define VMDL_BLEND_NO_VALUE			   0
#define VMDL_SLICER_DEFAULT_PRLL_SLICE_BATCH_SIZE  50

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

typedef enum VMdlSliceOpType {
    VMDL_SLICE_NONE = -1,
    VMDL_SLICE_HETEROGENEOUS = 0,
    VMDL_SLICE_OUTLINE_CURVES,
    VMDL_SLICE_OUTLINE_IMAGE,
    VMDL_SLICE_COVERAGE_CURVES,
    VMDL_SLICE_COVERAGE_IMAGE
} VMdlSliceOpType;

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
    /* The adjacent Srf to this one in the adjacent cell, if any.  Can be   */
    /* NULL if none (on the boundary of the whole VModel).  If exists, that */
    /* adjacent surface better point to this one.			    */
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

typedef struct VMdlSlicerParamsStruct {
    CagdRType ZRes;       /* Z size of a single pixel. */
    CagdRType XYRes[2];   /* XY size of a single pixel. */
    CagdRType Max[2];     /* XY bounding box of domain to slice. */
    CagdRType Min[2];
    TrivImagePixelType PixelType;
} VMdlSlicerParamsStruct;

/* Structures that govern tolerances and constants: */
typedef struct VMdlSubdivParamsStruct {
    CagdRType UVOpTol;
    CagdRType IsoSrfExtendAmount;
    int IsoCrvNumSamples;
    CagdRType SrfRegionMargin;
    CagdRType SrfSubdivTol;
    CagdRType SrfNumericTol;
    CagdRType BspSrfFitSampleRatio;
    CagdRType TCrvSimplifyTol;
    CagdRType DistSubdivTol;
    CagdRType DistNumericTol;
    int MaxErrSamples;
} VMdlSubdivParamsStruct;

typedef struct VMdlBoolParamsStruct {
    CagdRType SpeedTol;
    CagdRType SubdivTol;
    CagdRType NumericTol;
    CagdRType TraceTol;
    CagdRType BRepVerifyE3Tol;
    CagdRType SrfDiffEps;
    CagdBType TCrvLinAprx;
    CagdRType TCrvLinAprxTol;
    int SpeedTestNumSamples;
    CagdRType GeomCompEps;
    struct IPObjectStruct *LeftObj;
    struct IPObjectStruct *RightObj;
} VMdlBoolParamsStruct;

typedef struct VMdlSweepParamsStruct {
    CagdRType WTol;
} VMdlSweepParamsStruct;

typedef struct VMdlFilletParamsStruct {
    CagdRType FilletNumericTol;
} VMdlFilletParamsStruct;

typedef struct VMdlParamsStruct {
    CagdRType GeneralTol;
    VMdlSubdivParamsStruct SubdivParams;
    VMdlBoolParamsStruct BoolParams;
    VMdlSlicerParamsStruct SlicerParams;
    VMdlSweepParamsStruct SweepParams;
    VMdlFilletParamsStruct FilletParams;
} VMdlParamsStruct;

typedef void (*VMdlVModelBoolOpCBFuncType)(const VMdlParamsStruct *Params,
					   const VMdlVModelStruct *VMdl1,
					   const VMdlVModelStruct *VMdl2,
					   VMdlVModelStruct *ResVMdl,
					   VMdlBoolOpType BoolOp,
					   void *AuxData);

/* V-model blending of fields structures/typedefs. */

/* Properties */

typedef struct VMdlSlicerPropertyStruct {
    CagdPointType PType;	      /* Dimension of the property function. */
    CagdRType Property[CAGD_MAX_PT_SIZE];            /* Array of properties. */
} VMdlSlicerPropertyStruct;

typedef struct VMdlBlendFilletStruct {
    struct VMdlBlendFilletStruct *PNext;
    TrivTVStruct *FilletTV;                            /* Fillet trivariate. */
    struct TrivInverseQueryStruct *TVQueryStruct;   /* Handle for inversion. */
    int NumSols;					/* For TV inversion. */
    CagdRType UVWParams[3];				/* For TV inversion. */
    CagdSrfStruct *BndrSrf1;                      /* First boundary surface. */
    CagdSrfStruct *BndrSrf2;                     /* Second boundary surface. */
    void *SrfPointPrep1;                        /* Prep handle for BndrSrf1. */
    void *SrfPointPrep2;                        /* Prep handle for BndrSrf2. */
    CagdUVType ExtremeDistUV1;           /* UV coordinates of the projection */
                                                           /* over BndrSrf1. */
    CagdUVType ExtremeDistUV2;           /* UV coordinates of the projection */
                                                           /* over BndrSrf2. */
} VMdlBlendFilletStruct;

/* Topology structure of the V-model for fast access and pre-computations. */
typedef struct VMdlBlendTopologyStruct {
    struct VMdlBlendTopologyStruct *Pnext;
    TrivTVStruct *TVs;			  /* TV list associated to the VMdl. */
    unsigned int SizeTVs;			   /* Number of TVs in VMdl. */
    CagdSrfStruct **Srfs;	         /* Array of internal boundary SRFs.
					    that belong to the VMdl.         */
    unsigned int SizeSrfs;			  /* Size of previous array. */
    void **SrfPtDistPreps;		    /* Handle for PT - SRF distance. */
    size_t Index;		     /* Unique index associated to the VMdl. */
    int ObjID;
} VMdlBlendTopologyStruct;

/* Handle for computation of the blend. */
typedef struct VMdlBlendPrepStruct {
    struct VMdlBlendTopologyStruct *TopolStruct;   /* Stores V-primitives in */
						                 /* V-model. */
    VMdlBlendFilletStruct* Fillet;                  /* Additional handle for */
                                                   /* blends inside fillets. */
    struct VMdlBlendVolElemStruct *VElems;   /* Stores info about V-cells in */
						                /* V-model.  */
    CagdBBoxStruct *VMdlBBox;		     /* Bounding box of the V-model. */
    size_t NumVPrims;				   /* Number of V-primitives */
    int NumTVs;					    /* Number of trivariates */
    struct VMdlBlendPointStruct *Point;              /* Structure containing */
					        /* point-related quantities. */
    struct VMdlVModelStruct *VMdl;		   /* Corresponding V-model. */
    struct VMdlParamsStruct Params;   /* Parameters for blending operations. */
} VMdlBlendPrepStruct;

/* Structure containing useful quantities related to V-cells for blending and
encoding and avoiding multiple dynamic allocations */
typedef struct VMdlBlendVolElemStruct {
    struct VMdlBlendVolElemStruct *Pnext;
    VMdlVolumeElementStruct *VolumeElement;	    /* Corresponding V-cell. */
    struct TrivInverseQueryStruct **TVQueryStruct; /* Handles for inversion. */
						         /* of V-cell's TVs. */
    int *NumSols;					/* For TV inversion. */
    CagdRType **UVWParams;				/* For TV inversion. */
    size_t SizeTVs;				 /* Number of TVs in V-cell. */
} VMdlBlendVolElemStruct;

/* Structure containing useful quantities related to points for blending and */
/* encoding and avoiding multiple dynamic allocations.			     */
typedef struct VMdlBlendPointStruct {
    struct VMdlBlendPointStruct *Pnext;
    struct IPAttributeStruct *Attr;			 /* Point attribute. */
    CagdRType Coords[CAGD_MAX_PT_SIZE];	   /* Euclidean coords of the point. */
    CagdRType *Params[3];	  /* Parameter values (if image of some TV). */
    CagdBType *InclusionVPrims;		       /* Inclusion in V-primitives. */
    CagdBType Discretized;      /* TRUE if the point is defined by sampling  */
						          /* toward fitting. */
    int *TrivIDs;	             /* If the pt is defined by sampling of  */
				     /* a TV domain these are the TV indices */
    size_t NumberOfTrivariates;	 /* Number of TVs having the point as image. */
    CagdRType *BlendWeights;        /* Blending weights wrt internal surface */
			 	                              /* boundaries. */
    struct VMdlVolumeElementRefStruct *VolumeElement;      /* V-cell holding */
							       /* the point. */
} VMdlBlendPointStruct;

/* Structure storing the property values for a grid of points. */
typedef struct VMdlSlicerOutputImageStruct {
    void *Points;
    int Size[2];
    TrivImagePixelType PixelType;
} VMdlSlicerOutputImageStruct;

typedef struct VMdlVoxelVModelStruct {
    struct VMdlVoxelVModelStruct *Pnext;
    struct IPAttributeStruct *Attr;	
    void **Images;		    /* The different Z levels in the volume. */
    int Size[3];
    int ImageSize;      /* Size of one image: Size[0] * Size[1] * VoxelSize. */
    GMBBBboxStruct BBox;		      /* Bounding box of the volume. */
    TrivImagePixelType VoxelType;
} VMdlVoxelVModelStruct;

typedef void (*VMdlBlendCoordinatesFuncType)(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
				             const CagdRType *Pt);
typedef CagdPointType (*VMdlBlendPropertyFuncType)(
				struct VMdlBlendPrepStruct *BlendCoordPrep,
				const int ObjIndex,
				void *Output);
typedef void *(*VMdlBlendRuleFuncType)(
				VMdlBlendPrepStruct *BlendCoordPrep,
				const IrtRType Pt[4],
				void *OutputPtr,
				VMdlBlendCoordinatesFuncType Coordinates,
				VMdlBlendPropertyFuncType PropertyFunction);
typedef size_t(*VMdlEncVectorFieldFuncType)(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
				            VMdlBlendPointStruct *Pt,
				            VMdlSlicerPropertyStruct *Value);

typedef TrimSrfStruct *(*VMdlTSrfs2StitchedTSrfsFuncType)(TrimSrfStruct *,
							  CagdRType,
							  void *);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

VMdlVModelStruct *VMdlVModelCopy(const VMdlParamsStruct *Params,
				 const VMdlVModelStruct *VMdl);
VMdlVModelStruct *VMdlVModelCopyList(const VMdlParamsStruct *Params,
				     const VMdlVModelStruct *Mdls);

void VMdlVModelFree(VMdlVModelStruct *Mdl);
void VMdlVModelFreeList(VMdlVModelStruct *VMdls);

VMdlVModelStruct *VMdlGlueVModels(VMdlVModelStruct *Mdl1,
				  VMdlVModelStruct *Mdl2);
CagdBType VMdlGlueVModelsAppend(VMdlVModelStruct **Mdl1, 
				const VMdlVModelStruct *Mdl2, 
				CagdRType SrfDiffEps,
				CagdBType CalculateConnectivity,
				MiscPHashMap TVHMap);
void VMdlCalcMdlEuclCrvs(const VMdlParamsStruct *Params,
			 MdlModelStruct *BMdl);
void VMdlEvalEuclidCrvsForTrimCrvs(TrimSrfStruct* TrSrf);
void VMdlDefaultParams(VMdlParamsStruct *Params);

/******************************* Debugging operations ***********************/

#ifdef DEBUG
void VMdlDbg(void *Obj);
#endif /* DEBUG */

void VMdlPrintVModel(VMdlVModelStruct *VM);
void VMdlPrintVE(VMdlVModelStruct *VM);
void VMdlSplitVModelInDir(const VMdlParamsStruct *Params,
			  VMdlVModelStruct *VM,
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

CagdBBoxStruct *VMdlVModelBBox(const VMdlParamsStruct *Params,
			       const VMdlVModelStruct *VMdl,
			       CagdBBoxStruct *BBox,
			       int OnlyGeom);
CagdBBoxStruct *VMdlVModelListBBox(const VMdlParamsStruct *Params,
				   const VMdlVModelStruct *Mdls,
				   CagdBBoxStruct *CagdBbox,
			           int OnlyGeom);

MdlModelStruct *VMdlGetBoundaryVModel(const VMdlParamsStruct *Params,
				      const VMdlVModelStruct *Mdl);
struct IPObjectStruct *VMdlGetBoundarySurfaces2(const VMdlParamsStruct *Params,
						const VMdlVModelStruct *Mdl);
MdlModelStruct *VMdlGetOuterBoundarySurfacesVModel(
						const VMdlParamsStruct *Params,
						const VMdlVModelStruct *VMdl);
MdlModelStruct *VMdlGetBndryVElement(const VMdlParamsStruct *Params,
				     VMdlVolumeElementStruct *VCell);
CagdCrvStruct *VMdlGetBoundaryCurves(const VMdlVModelStruct *Mdl);
TrivTVStruct *VMdlIsNonTrimmedVModel(const VMdlParamsStruct *Params,
				     const VMdlVModelStruct *VMdl);
CagdBType VMdlIsTVBoundaryVSrf(const VMdlInterTrimSrfStruct *VSrf, 
			       const TrivTVStruct *TV, 
			       TrivTVDirType *IsoDir, 
			       CagdRType *IsoVal);
const TrivTVStruct *VMdlGetTVBoundarySrf(const VMdlVModelStruct *VMdl,
					 const CagdSrfStruct *Srf,
					 TrivTVBndryType *TVBndry,
					 int *Reversed);
CagdBType VMdlVModelsSame(const VMdlParamsStruct *Params,
			  const VMdlVModelStruct *VMdl1,
			  const VMdlVModelStruct *VMdl2);

/************************** primitives **************************************/

VMdlVModelStruct *VMdlVolElementFromBoundaryModel(
				   const VMdlParamsStruct *Params,
				   const MdlModelStruct *InBMdl, 
				   VMdlInterTrivTVRefStruct *ElementTVsRefList);
VMdlVModelStruct *VMdlFromBoundaryModel(const VMdlParamsStruct *Params,
					const MdlModelStruct *BMdl,
					const TrivTVStruct *TV);

VMdlVModelStruct *VMdlPrimBoxVMdl(CagdRType MinX,
				  CagdRType MinY,
				  CagdRType MinZ,
				  CagdRType MaxX,
				  CagdRType MaxY,
				  CagdRType MaxZ);
VMdlVModelStruct *VMdlPrimBoxVMdl2(const VMdlParamsStruct *Params,
				   CagdRType MinX,
				   CagdRType MinY,
				   CagdRType MinZ,
				   CagdRType MaxX,
				   CagdRType MaxY,
				   CagdRType MaxZ);
VMdlVModelStruct *VMdlPrimCubeSphereVMdl(const CagdVType Center,
					 CagdRType Radius,
					 CagdBType Rational,
					 CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimCubeSphereVMdl2(const VMdlParamsStruct *Params,
					  const CagdVType Center,
					  CagdRType Radius,
					  CagdBType Rational,
					  CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimTorusVMdl(const CagdVType Center,
				    CagdRType MajorRadius,
				    CagdRType MinorRadius,
				    CagdBType Rational,
				    CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimTorusVMdl2(const VMdlParamsStruct *Params,
				     const CagdVType Center,
				     CagdRType MajorRadius,
				     CagdRType MinorRadius,
				     CagdBType Rational,
				     CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimConeVMdl(const CagdVType Center,
				   CagdRType Radius,
				   CagdRType Height,
				   CagdBType Rational,
				   CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlPrimConeVMdl2(const VMdlParamsStruct *Params,
				    const CagdVType Center,
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
VMdlVModelStruct *VMdlPrimCone2VMdl2(const VMdlParamsStruct *Params,
				     const CagdVType Center,
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
VMdlVModelStruct *VMdlPrimCylinderVMdl2(const VMdlParamsStruct *Params,
					const CagdVType Center,
					CagdRType Radius,
					CagdRType Height,
					CagdBType Rational,
					CagdRType InternalCubeEdge);
VMdlVModelStruct *VMdlRuledTrimSrf(const TrimSrfStruct *TSrf1,
				   const CagdSrfStruct *Srf2,
				   int OtherOrder,
				   int OtherLen);
VMdlVModelStruct *VMdlRuledTrimSrfExtra(const VMdlParamsStruct *Params,
					const TrimSrfStruct *TSrf1,
					const CagdSrfStruct *Srf2,
					int OtherOrder,
					int OtherLen,
					void *ExtraData);
VMdlVModelStruct *VMdlExtrudeTrimSrf(const TrimSrfStruct *Section,
				     CagdVecStruct *Dir);
VMdlVModelStruct *VMdlExtrudeTrimSrfExtra(const VMdlParamsStruct *Params,
					  const TrimSrfStruct *Section,
					  CagdVecStruct *Dir,
					  void *ExtraData);
VMdlVModelStruct *VMdlOfRevTrimSrf(const TrimSrfStruct *Section,
			    CagdRType StartAngle,
			    CagdRType EndAngle,
			    CagdBType PolyApprox);
VMdlVModelStruct *VMdlOfRevTrimSrfExtra(const VMdlParamsStruct* Params,
					const TrimSrfStruct *Section,
					CagdRType StartAngle,
					CagdRType EndAngle,
					CagdBType Rational,
					void *ExtraData);
VMdlVModelStruct *VMdlOfRevAxisTrimSrf(const TrimSrfStruct *Section,
				       const TrivV4DType AxisPoint,
				       const TrivV4DType AxisVector,
				       CagdRType StartAngle,
				       CagdRType EndAngle,
				       CagdBType PolyApprox);
VMdlVModelStruct *VMdlOfRevAxisTrimSrfExtra(const VMdlParamsStruct *Params,
					    const TrimSrfStruct *Section,
					    const TrivV4DType AxisPoint,
					    const TrivV4DType AxisVector,
					    CagdRType StartAngle,
					    CagdRType EndAngle,
					    CagdBType Rational,
					    void *ExtraData);
VMdlVModelStruct *VMdlSweepTrimSrf(const TrimSrfStruct *TSrf,
				   const CagdCrvStruct *Axis,
				   const CagdCrvStruct *ScalingCrv,
				   CagdRType Scale,
				   const VoidPtr Frame,
				   int FrameOption);
VMdlVModelStruct *VMdlSweepTrimSrfExtra(const VMdlParamsStruct *Params,
					const TrimSrfStruct *TSrf,
					const CagdCrvStruct *Axis,
					const CagdCrvStruct *ScalingCrv,
					CagdRType Scale,
					const VoidPtr Frame,
					int FrameOption,
					void* ExtraData);

/***************************  operations ************************************/

CagdBType VMdlTrimVModelBySurface(const VMdlVModelStruct *VMdl,
				  const CagdSrfStruct *Srf);
CagdBType VMdlRemoveTrimmingSurface(const VMdlVModelStruct *VMdl,
				    const CagdSrfStruct *Srf);
void VMdlVModelTransform(const VMdlParamsStruct *Params,
			 VMdlVModelStruct *VMdl,
			 IrtHmgnMatType Mat);
TrivTVStruct *VMdlFetchTrivar(const VMdlVolumeElementStruct *VMTrimmedTV);
TrimSrfStruct *VMdlFetchTrimmingSrfs(const VMdlVolumeElementStruct *VMTrimmedTV);
CagdBType VMdlVModelReplaceTV(VMdlVModelStruct *VMdl, 
			     TrivTVStruct *OldTV,
			     const TrivTVStruct *NewTV);

/*************************** Boolean operations *****************************/

VMdlVModelStruct *VMdlVModelBoolOp(const VMdlParamsStruct *Params,
				   const VMdlVModelStruct *VMdlA,
				   const VMdlVModelStruct *VMdlB,
				   VMdlBoolOpType OpType);
VMdlVModelStruct *VMdlVModelIntersect(const VMdlParamsStruct *Params,
				      const VMdlVModelStruct *VMdlA,
				      const VMdlVModelStruct *VMdlB);
VMdlVModelStruct *VMdlVModelUnion(const VMdlParamsStruct *Params,
				  const VMdlVModelStruct *VMdlA,
				  const VMdlVModelStruct *VMdlB);
VMdlVModelStruct *VMdlVModelSubtract(const VMdlParamsStruct *Params,
				     const VMdlVModelStruct *VMdlA,
				     const VMdlVModelStruct *VMdlB);
VMdlVModelStruct *VMdlVModelSymDiff(const VMdlParamsStruct *Params,
				    const VMdlVModelStruct *VMdlA,
				    const VMdlVModelStruct *VMdlB);
VMdlVModelStruct *VMdlVModelNegate(const VMdlParamsStruct *Params,
				   const VMdlVModelStruct *VMdl);
VMdlVModelStruct *VMdlClipVModelByPlane(const VMdlParamsStruct *Params,
					const VMdlVModelStruct *Mdl,
					const IrtPlnType Pln,
					VMdlBoolOpType BoolOp);
VMdlVModelBoolOpCBFuncType VMdlSetBoolOpCBFunc(
					   VMdlVModelBoolOpCBFuncType CBFunc);

/************************** Subdivision *************************************/

VMdlVModelStruct *VMdlSubdivideVElement(const VMdlParamsStruct *Params,
					VMdlVolumeElementStruct *VElem,
					const TrivTVStruct *TV,
					TrivTVDirType Dir,
					IrtRType t,
					IrtRType *OtherParamAttribVals,
					CagdBType HandleKnotLineIntersections);
VMdlVModelStruct *VMdlSubdivideVModel(const VMdlParamsStruct *Params,
				      VMdlVModelStruct *VMdl,
				      TrivTVDirType Dir,
				      IrtRType t);
VMdlVModelStruct *VMdlSubdivideVElemToBezierVElements(
					 const VMdlParamsStruct *Params,
					 const VMdlVolumeElementStruct *VElem,
					 const TrivTVStruct *TV);
VMdlVModelStruct *VMdlSubdivideVMdlToBezierVElements(
						const VMdlParamsStruct *Params,
						const VMdlVModelStruct *VMdl);
TrivTVStruct *VMdlUntrimVModel(const VMdlParamsStruct *Params,
			       const VMdlVModelStruct *VMdl, 
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

int VMdlStitchMdlModel(const VMdlParamsStruct *Params, MdlModelStruct *Mdl);
VMdlVModelStruct *VMdlCnvrtSrf2VMdl(const CagdSrfStruct *Srf);
VMdlVModelStruct *VMdlCnvrtTrimmedSrf2VMdl(const TrimSrfStruct *TSrf);
VMdlVModelStruct *VMdlCnvrtTrivar2VMdl(const VMdlParamsStruct *Params,
				       const TrivTVStruct *TV,
				       int PrimID);
VMdlVModelStruct *VMdlCnvrtTrivarList2VMdl(const VMdlParamsStruct *Params,
					   TrivTVStruct *TVList,
					   int PrimID);
TrimSrfStruct *VMdlCnvrtVMdls2TrimmedSrfs(const VMdlParamsStruct *Params,
					  const VMdlVModelStruct *VMdls);
TrimSrfStruct *VMdlCnvrtVMdl2TrimmedSrfs(const VMdlParamsStruct *Params,
					 const VMdlVModelStruct *VMdl);
MdlModelStruct *VMdlCnvrtVMdls2Mdls(const VMdlParamsStruct *Params,
				    const VMdlVModelStruct *VMdls);
MdlModelStruct *VMdlCnvrtVMdl2Mdl(const VMdlParamsStruct *Params,
				  const VMdlVModelStruct *VMdl);
TrimSrfStruct *VMdlCnvrtVSrf2TrimmedSrf(const VMdlInterTrimSrfStruct *VSrf);
void VMdlAddTrimSrfToVMdl(VMdlVModelStruct *VMdl,
			  const TrimSrfStruct *TSrf,
			  CagdRType Tol);
CagdBType VMdlSetSplitPeriodicTV(CagdBType Split);
VMdlVModelStruct *VMdlExtractVElements(const VMdlVModelStruct *VMdl);
CagdCrvStruct **VMdlExtractTrimCrvLoop(const VMdlInterTrimSrfStruct  
							       *VTrimSrfStruct,  
				       int *NumLoops);

/*************************** Volumetric slicing ***************************/

struct VMdlSlicerInfoStruct *VMdlSlicerPlanarSlicerInitInfo(
					    const struct IPObjectStruct *Model,
					    const struct IPObjectStruct *AuxMdl,
					    VMdlParamsStruct *Params,
					    TrivTVStruct **TV,
					    int VCellIdx,
					    TrivImagePixelType PixelType,
					    const CagdRType *SliceDimension);
struct MdlModelStruct *VMdlBuildModelFromTVBndries(const TrivTVStruct *Trivar);
struct VMdlSlicerInfoStruct *VMdlSlicerInitVElement(
				       const VMdlParamsStruct *Params,
				       VMdlVolumeElementStruct *VolumeElement);
struct VMdlSlicerInfoStruct *VMdlSlicerInitVModelVElement(
				  const VMdlParamsStruct *Params,
				  struct VMdlSlicerInfoStruct *RetIn,
				  const VMdlVModelStruct *VModel,
				  VMdlVolumeElementStruct *VolumeElement);
struct VMdlSlicerInfoStruct *VMdlSlicerInitVModel(
					const VMdlParamsStruct *Params,
					const VMdlVModelStruct *VModel);
struct VMdlSlicerInfoStruct *VMdlSlicerInitTrivar(
				       const TrivTVStruct *Trivar, 
				       const VMdlSlicerParamsStruct *Params);
struct VMdlSlicerInfoStruct *VMdlSlicerInitTrivMdl(
				       const TrivTVStruct *Trivar,
				       const struct IPObjectStruct *BMdl,
				       const VMdlSlicerParamsStruct *Params);
struct VMdlSlicerInfoStruct *VMdlSlicerInitTrivPoly(
					const TrivTVStruct *Trivar,
					const struct IPObjectStruct *BPls,
					const VMdlSlicerParamsStruct *Params);
struct VMdlSlicerInfoStruct *VMdlSlicerInitModel(
					const MdlModelStruct *BMdl,
					const VMdlSlicerParamsStruct *Params);
int VMdlSlicerAddGeom(const VMdlParamsStruct *Params,
		      struct VMdlSlicerInfoStruct *Info,
		      const struct IPObjectStruct *Geom,
		      int Priority);
void VMdlSlicerFree(struct VMdlSlicerInfoStruct *Info);
void VMdlSlicerSliceAtZLevel(const VMdlParamsStruct *Params,
			     struct VMdlSlicerInfoStruct *Info,
			     CagdRType z,
			     int BatchSize);
struct IPObjectStruct *VMdlSlicerSliceAtZLevelCoverage(
					     const VMdlParamsStruct *Params,
					     VMdlSliceOpType SliceOper,
					     struct VMdlSlicerInfoStruct *Info,
					     CagdRType z);
VMdlSlicerOutputImageStruct *VMdlSlicerSliceAtZBatch(
					     const VMdlParamsStruct *Params,
					     VMdlSliceOpType SliceOper,
					     struct VMdlSlicerInfoStruct *Info,
					     CagdRType *ZVals,
					     int BatchSz);
VMdlSlicerOutputImageStruct *VMdlSlicerSliceAtZBatch2(
					     const VMdlParamsStruct *Params,
					     VMdlSliceOpType SliceOper,
					     struct VMdlSlicerInfoStruct *Info,
					     CagdRType ZMin,
					     CagdRType ZMax,
					     CagdRType ZStep);
void VMdlSlicerSliceAtZBatchToFiles(const VMdlParamsStruct *Params,
				    VMdlSliceOpType SliceOper,
				    struct VMdlSlicerInfoStruct *Info,
				    CagdRType *ZVals,
				    int BatchSz,
				    const char *FileNameBase);
void VMdlSlicerSliceAtZBatchToFiles2(const VMdlParamsStruct *Params,
				     VMdlSliceOpType SliceOper,
				     struct VMdlSlicerInfoStruct *Info,
				     CagdRType ZMin,
				     CagdRType ZMax,
				     CagdRType ZStep,
				     const char *FileNameBase);
int VMdlSlicerGetCurrSliceXY(struct VMdlSlicerInfoStruct *Info,
			     int x,
			     int y,
			     CagdRType *Params,
			     CagdRType *Pos,
			     const TrivTVStruct **TV);
void VMdlSlicerGetSliceSize(struct VMdlSlicerInfoStruct *Info, int *Size);
void VMdlSlicerAssignValueToDataXY(struct VMdlSlicerInfoStruct *Info,
				   int x,
				   int y,
				   CagdRType *ValueBuf);
void VMdlSlicerAssignValueToImgXY(struct VMdlSlicerInfoStruct *Info,
				  int x,
				  int y,
				  VMdlSlicerOutputImageStruct *Img);
void VMdlSlicerSaveImage(VMdlSlicerOutputImageStruct *Image,
                         const char *Output);
void VMdlSlicerOutputImageFree(VMdlSlicerOutputImageStruct *Item);
void VMdlSlicerOutputImageFreeArr(VMdlSlicerOutputImageStruct *Items,
				  const int NumItems);

/**************************** Voxelization module ****************************/

/* This data structure stores for each projection direction, its meta data.  */
/* The image should be a squared gray scale image and its pixels should be   */
/* in [0, 255]. The image values should be stored in a continuous unsigned   */
/* char vector pointed to by ImageData. The width/height of the image should */
/* be in OneSideImage. Finally, the view angle, in degrees, or projection    */
/* direction, is assigned to Theta (in [0, 360) range).		             */

typedef struct VMdlVxlRadonProjStruct {
    unsigned char *ImageData;
    int OneSideImage;
    double Theta;
} VMdlVxlRadonProjStruct;

int VMdlVxlInPixelOnBndry(const VMdlVoxelVModelStruct *VxlVMdl,
			  int i,
			  int j,
			  int k);
int VMdlVxlOutPixelOnBndry(const VMdlVoxelVModelStruct *VxlVMdl,
			   int i,
			   int j,
			   int k);
CagdRType VMdlVxlGetRealVoxelVolume(const VMdlVoxelVModelStruct *VxlVMdl);

void VMdlVxlEdge2VoxelVModel(VMdlVoxelVModelStruct *VxlVMdl,
			     const CagdRType *Pt1,
			     const CagdRType *Pt2);
VMdlVoxelVModelStruct *VMdlVxlCrv2VoxelVModel(const struct IPObjectStruct *Crv,
					      CagdRType Tol,
					      const int *Sizes);
VMdlVoxelVModelStruct *VMdlVxlMdl2VoxelVModel(const struct IPObjectStruct *Mdl,
					      VMdlSliceOpType SliceOper,
					      const CagdRType *Dims);
struct IPObjectStruct *VMdlVxlMrchCubeVxlVMdl(const VMdlVoxelVModelStruct
					                             *VxlVMdl,
					      const CagdRType IsoLevel);
CagdRType VMdlVxlAreaVxlVMdl(const VMdlVoxelVModelStruct *VxlVMdl,
			     const CagdRType IsoLevel);
CagdRType VMdlVxlVolumeVxlVMdl(const VMdlVoxelVModelStruct *VxlVMdl);
VMdlVoxelVModelStruct *VMdlVxlSimpleVoxelOffset(const VMdlVoxelVModelStruct
								     *VxlVMdl,
						int NumPixels);
VMdlVoxelVModelStruct *VMdlVxlCopyVoxelVModel(const VMdlVoxelVModelStruct
					                             *VxlVMdl);
void VMdlVxlFreeVoxelVModel(VMdlVoxelVModelStruct *VxlVMdl);
void VMdlVxlPrintVoxelVModel(VMdlVoxelVModelStruct *VxlVMdl,
			     const char *VxlFormat,
			     FILE *f,
			     int Row);
VMdlVoxelVModelStruct *VMdlVxlHeuristicRadon(VMdlVxlRadonProjStruct 
							       *ProjectionList,
			                     int ProjCount);

/**************************** Blending attributes ****************************/

void VMdlBlendComputeDistCoordinates(VMdlBlendPrepStruct *BlendCoordPrep,
                                     const CagdRType Pt[3]);
VMdlSlicerOutputImageStruct *VMdlBlendPropertiesSlice(
				    VMdlBlendPrepStruct *BlendCoordPrep,
				    const double z,
				    int Resolution[2],
				    VMdlBlendCoordinatesFuncType Coordinates,
				    VMdlBlendPropertyFuncType PropertyFunction,
				    VMdlBlendRuleFuncType BlendingRule,
				    unsigned int SizePixelProperty);
VMdlSlicerOutputImageStruct *VMdlBlendPropertiesSliceOneBatch(
				    VMdlBlendPrepStruct *BlendCoordPrep,
				    int BatchSz,
				    const double *ZVals,
				    const int Resolution[2],
				    VMdlBlendCoordinatesFuncType Coordinates,
				    VMdlBlendPropertyFuncType PropertyFunction,
				    VMdlBlendRuleFuncType BlendingRule,
				    unsigned int SizePixelProperty);
void *VMdlBlendPropertiesPerPoint(VMdlBlendPrepStruct *BlendCoordPrep,
                                  const IrtRType Pt[4],
                                  void *OutputPtr,
                                  VMdlBlendCoordinatesFuncType Coordinates,
                                  VMdlBlendPropertyFuncType PropertyFunction);
VMdlBlendPrepStruct *VMdlBlendCoordPrep(const VMdlParamsStruct *Params,
					VMdlVModelStruct *VMdl);
int VMdlBlendInIntersection(VMdlBlendPrepStruct *BlendCoordPrep,
                            int *VMdlIndex);
void VMdlBlendCoordPrepFree(VMdlBlendPrepStruct *Item);
void VMdlBlendBoolTrivariates(VMdlBlendPrepStruct *BlendCoordPrep,
                              const IrtRType Pt[3],
                              CagdBType UseRayCasting);
void VMdlBlendPointListFree(VMdlBlendPointStruct *Item);
VMdlBlendPointStruct *VMdlBlendPointStructInit(const int SizeTopologyStruct,
                                               const int NumTVs);
void VMdlBlendPointFree(VMdlBlendPointStruct *Item);
CagdPointType VMdlBlendDfltRGBPropertyFunction(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
                                               const int ObjIndex,
                                               void *Output);
void VMdlBlendBoolTrivariatesVElem(VMdlBlendPrepStruct *BlendCoordPrep,
                                   IrtRType Pt[3],
                                   VMdlBlendVolElemStruct *VElem);
CagdPointType VMdlBlendDflt3DPropertyFunction(VMdlBlendPrepStruct
 						 	        *BlendCoordPrep,
        				      const int ObjIndex,
  					      void *Output);
CagdPointType VMdlBlendDflt1DPropertyFunction(VMdlBlendPrepStruct
							 	*BlendCoordPrep,
        				      const int ObjIndex,
                                              void *Output);
#ifdef VMDL_BLEND_DISCONT
void VMdlBlendVElemComputeDistCoordinates(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
                                             const CagdRType Pt[3]);
VMdlSlicerOutputImageStruct *VMdlBlendVCellPropertiesSlice(
                                           VMdlBlendPrepStruct *BlendCoordPrep,
                                           const double z,
                                           const int *Resolution);
VMdlSlicerPropertyStruct VMdlBlendVCellPropertiesPerPoint(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
                                                         IrtRType Pt[3]);
#endif /* VMDL_BLEND_DISCONT */

/*************************** Filleting attributes ****************************/

void VMdlBlendFilletComputeCoordinates(VMdlBlendPrepStruct *BlendCoordPrep,
                                       const CagdRType *Pt);
void *VMdlBlendFilletPropertiesPerPoint(
				    VMdlBlendPrepStruct *BlendCoordPrep,
				    const IrtRType Pt[3],
				    void *OutputPtr,
				    VMdlBlendCoordinatesFuncType Coordinates,
				    VMdlBlendPropertyFuncType PropertyFunction);
VMdlSlicerOutputImageStruct *VMdlBlendFilletPropertiesSlice(
				    const VMdlParamsStruct *Params,
                                    VMdlBlendPrepStruct *BlendCoordPrep,
				    const double z,
				    int Resolution[2],
				    VMdlBlendCoordinatesFuncType Coordinates,
				    VMdlBlendPropertyFuncType PropertyFunction,
				    VMdlBlendRuleFuncType BlendingRule,
				    unsigned int SizePixelProperty);
CagdPointType VMdlBlendFilletDflt1DPropertyFunction(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
                                                    const int ObjIndex,
                                                    void *Output);
CagdPointType VMdlBlendFilletDflt3DPropertyFunction(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
                                                    const int ObjIndex,
                                                    void *Output);
CagdPointType VMdlBlendFilletDfltRGBPropertyFunction(VMdlBlendPrepStruct
                                                               *BlendCoordPrep,
                                                     const int ObjIndex,
                                                     void *Output);
VMdlBlendPrepStruct *VMdlBlendFilletCoordPrep(const VMdlParamsStruct *Params,
					      VMdlVModelStruct *VMdl,
                                              TrivTVStruct *FilletTV,
                                              CagdSrfStruct *BndrSrf1,
                                              CagdSrfStruct *BndrSrf2);
void VMdlBlendFilletFree(VMdlBlendFilletStruct *Item);

void VMdlCreateFillet(const VMdlParamsStruct *Params,
                      const CagdSrfStruct *Srf1List,
                      const CagdSrfStruct *Srf2List,
                      CagdRType RailDist,
                      int R1Orient,
                      int R2Orient,
                      CagdRType TanScale,
                      int ApproxCrvsCtlPts,
                      TrivFilletingMethodType FilletingMethod,
                      CagdBType PreciseFillet,
                      TrivTVStruct **FilletTVs,
                      MdlModelStruct **FilletMdls,
                      CagdSrfStruct **SubSrfs1,
                      CagdSrfStruct **SubSrfs2);
VMdlVModelStruct *VMdlCreateFillet2(const VMdlParamsStruct *Params,
                                    const CagdSrfStruct *Srf1List,
                                    const CagdSrfStruct *Srf2List,
                                    CagdRType RailDist,
                                    int R1Orient,
                                    int R2Orient,
                                    CagdRType TanScale,
                                    int ApproxCrvsCtlPts,
                                    TrivFilletingMethodType FilletingMethod,
                                    CagdBType PreciseFillet);

/**************************** Encoding attributes ****************************/

TrivTVStruct *VMdlEncRetrieveProperties(
                                     VMdlBlendPrepStruct *BlendCoordPrep,
                                     const int NumSamplePoints,
                                     CagdBType CreateNewTrivStruct,
                                     const int AddedRefinedKnots,
                                     VMdlEncVectorFieldFuncType F,
                                     VMdlBlendCoordinatesFuncType Coordinates);
size_t VMdlEncDfltRGBVectorField(VMdlBlendPrepStruct *BlendCoordPrep,
        			 VMdlBlendPointStruct *Pt,
        			 VMdlSlicerPropertyStruct *PropertyOut);
size_t VMdlEncDflt3DVectorField(VMdlBlendPrepStruct *BlendCoordPrep,
        			VMdlBlendPointStruct *Pt,
			        VMdlSlicerPropertyStruct *Value);
size_t VMdlEncDflt1DVectorField(VMdlBlendPrepStruct *BlendCoordPrep,
                                VMdlBlendPointStruct *Pt,
                                VMdlSlicerPropertyStruct *Value);
IrtRType VMdlEncPropertyFunctionError(
			       const VMdlParamsStruct *Params,
                               VMdlBlendPrepStruct *BlendCoordPrep,
                               const int Resolution[3],
                               VMdlBlendPropertyFuncType PropertyFunction,
                               VMdlBlendPropertyFuncType ReconPropertyFunction,
                               VMdlBlendCoordinatesFuncType Coordinates);
struct VMdlSlicerInfoStruct *VMdlEncInitSlicerVElementInIntersection(
                                        VMdlVolumeElementStruct *VolumeElement,
                                        const VMdlSlicerParamsStruct *Params,
                                        const int TrivID);

#ifdef IRIT_NOT_USED
IrtRType VMdlEncVModelVectorFieldError(
                               VMdlBlendPrepStruct *BlendCoordPrep,
                               const int Resolution[3],
                               VMdlEncVectorFieldFuncType VectorField,
                               VMdlBlendPropertyFuncType ReconPropertyFunction,
                               VMdlBlendCoordinatesFuncType Coordinates,
			       VMdlBlendRuleFuncType BlendingRule);
#endif /* IRIT_NOT_USED */

/***************** Stitching and Boolean Operation Callbacks *****************/

VMdlTSrfs2StitchedTSrfsFuncType VMdlSetStitchCallback(
					VMdlTSrfs2StitchedTSrfsFuncType Func);
VMdlTSrfs2StitchedTSrfsFuncType VMdlGetStitchCallback();

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_VOL_MDL_H */
