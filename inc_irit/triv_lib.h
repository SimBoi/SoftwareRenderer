/******************************************************************************
* Triv_lib.h - header file for the TRIV library.			      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber, Sep. 94.					      *
******************************************************************************/

#ifndef IRIT_TRIV_LIB_H
#define IRIT_TRIV_LIB_H

#include <stdio.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/miscattr.h"
#include "inc_irit/misc_lib.h"
#include "inc_irit/cagd_lib.h"

#define TV_PT_COPY(PtDest, PtSrc) \
            IRIT_GEN_COPY((char *) (PtDest), (char *) (PtSrc), \
						    4 * sizeof(CagdRType))
#define TV_PLANE_COPY(PlDest, PlSrc) \
	    IRIT_GEN_COPY((char *) (PlDest), (char *) (PlSrc),  \
						    5 * sizeof(CagdRType))
#define TV_PT_SQR_LENGTH(Pt)	(IRIT_SQR(Pt[0]) + IRIT_SQR(Pt[1]) + \
				 IRIT_SQR(Pt[2]) + IRIT_SQR(Pt[3]))

#define TV_PT_LENGTH(Pt)	sqrt(TV_PT_SQR_LENGTH(Pt))

#define TV_PT_RESET(Pt)		IRIT_ZAP_MEM((Pt), 4 * sizeof(IrtRType))

#define TV_PT_NORMALIZE(Pt)	{    CagdRType Size = TV_PT_LENGTH(Pt); \
				     _IRIT_PT_NORMALIZE_MSG_ZERO(Size) \
				     { \
					 Pt[0] /= Size; \
					 Pt[1] /= Size; \
				         Pt[2] /= Size; \
				         Pt[3] /= Size; \
				     } \
				}

#define TV_PT_BLEND(Res, Pt1, Pt2, t) \
				{ Res[0] = Pt1[0] * t + Pt2[0] * (1 - t); \
				  Res[1] = Pt1[1] * t + Pt2[1] * (1 - t); \
				  Res[2] = Pt1[2] * t + Pt2[2] * (1 - t); \
				  Res[3] = Pt1[3] * t + Pt2[3] * (1 - t); \
			        }

#define TV_PT_ADD(Res, Pt1, Pt2) { Res[0] = Pt1[0] + Pt2[0]; \
				   Res[1] = Pt1[1] + Pt2[1]; \
				   Res[2] = Pt1[2] + Pt2[2]; \
				   Res[3] = Pt1[3] + Pt2[3]; \
			         }

#define TV_PT_SUB(Res, Pt1, Pt2) { Res[0] = Pt1[0] - Pt2[0]; \
				   Res[1] = Pt1[1] - Pt2[1]; \
				   Res[2] = Pt1[2] - Pt2[2]; \
				   Res[3] = Pt1[3] - Pt2[3]; \
				 }

#define TV_PT_SWAP(Pt1, Pt2)	{ IRIT_SWAP(CagdRType, Pt1[0], Pt2[0]); \
				  IRIT_SWAP(CagdRType, Pt1[1], Pt2[1]); \
				  IRIT_SWAP(CagdRType, Pt1[2], Pt2[2]); \
				  IRIT_SWAP(CagdRType, Pt1[3], Pt2[3]); \
				}

#define TV_PT_PT_DIST(Pt1, Pt2) sqrt(IRIT_SQR(Pt1[0] - Pt2[0]) + \
				     IRIT_SQR(Pt1[1] - Pt2[1]) + \
				     IRIT_SQR(Pt1[2] - Pt2[2]) + \
				     IRIT_SQR(Pt1[3] - Pt2[3]))

#define TV_PT_PT_DIST_SQR(Pt1, Pt2) (IRIT_SQR(Pt1[0] - Pt2[0]) + \
				     IRIT_SQR(Pt1[1] - Pt2[1]) + \
				     IRIT_SQR(Pt1[2] - Pt2[2]) + \
				     IRIT_SQR(Pt1[3] - Pt2[3]))

#define TV_DOT_PROD(Pt1, Pt2)	(Pt1[0] * Pt2[0] + \
				 Pt1[1] * Pt2[1] + \
				 Pt1[2] * Pt2[2] + \
				 Pt1[3] * Pt2[3])

typedef CagdRType TrivP4DType[4];
typedef CagdRType TrivV4DType[4];
typedef CagdRType TrivUVWType[3];
typedef CagdRType TrivPln4DType[5];

typedef enum {
    TRIV_ERR_DIR_NOT_VALID,
    TRIV_ERR_UNDEF_CRV,
    TRIV_ERR_UNDEF_SRF,
    TRIV_ERR_UNDEF_TRIVAR,
    TRIV_ERR_UNDEF_GEOM,
    TRIV_ERR_UNSUPPORT_PT,
    TRIV_ERR_RATIONAL_NO_SUPPORT,
    TRIV_ERR_RATIONAL_EXPECTED,
    TRIV_ERR_WRONG_ORDER,
    TRIV_ERR_KNOT_NOT_ORDERED,
    TRIV_ERR_NUM_KNOT_MISMATCH,
    TRIV_ERR_INDEX_NOT_IN_MESH,
    TRIV_ERR_POWER_NO_SUPPORT,
    TRIV_ERR_WRONG_DOMAIN,
    TRIV_ERR_INCONS_DOMAIN,
    TRIV_ERR_DIR_NOT_CONST_UVW,
    TRIV_ERR_SCALAR_PT_EXPECTED,
    TRIV_ERR_INVALID_AXIS,
    TRIV_ERR_NO_CLOSED_POLYGON,
    TRIV_ERR_TWO_INTERSECTIONS,
    TRIV_ERR_NO_MATCH_PAIR,
    TRIV_ERR_2_OR_4_INTERS,
    TRIV_ERR_FAIL_FIND_PT,
    TRIV_ERR_FAIL_READ_FILE,
    TRIV_ERR_INVALID_STROKE_TYPE,
    TRIV_ERR_READ_FAIL,
    TRIV_ERR_TVS_INCOMPATIBLE,
    TRIV_ERR_PT_OR_LEN_MISMATCH,
    TRIV_ERR_BZR_TV_EXPECT,
    TRIV_ERR_BSP_TV_EXPECT,
    TRIV_ERR_PERIODIC_EXPECTED,
    TRIV_ERR_PERIODIC_NOT_SUPPORTED,
    TRIV_ERR_CRV_OR_SRF_EXPECTED,
    TRIV_ERR_CRV_SRF_TV_EXPECTED,
    TRIV_ERR_WRONG_NUM_BNDRY_TILES,
    TRIV_ERR_UNSUPPORT_DERIV,
    TRIV_ERR_INVALID_VALUE,
    TRIV_ERR_NO_INTERSECTION,
    TRIV_ERR_SWEEP_AXIS_TOO_COMPLEX,
    TRIV_ERR_FILLET_UNSUPPORTED_TOPOLOGY,
    TRIV_ERR_FILLET_UNSUPPORTED_SRF_TYPE,

    TRIV_ERR_UNDEFINE_ERR
} TrivFatalErrorType;

typedef enum {
    TRIV_UNDEF_TYPE = 1220,
    TRIV_TVBEZIER_TYPE,
    TRIV_TVBSPLINE_TYPE,
    TRIV_TVPOWER_TYPE
} TrivGeomType;

typedef enum {
    TRIV_NO_DIR = CAGD_NO_DIR,
    TRIV_CONST_U_DIR = CAGD_CONST_U_DIR,
    TRIV_CONST_V_DIR = CAGD_CONST_V_DIR,
    TRIV_CONST_W_DIR,
    TRIV_END_DIR
} TrivTVDirType;

#define TRIV_IMAGE_VAR_BYTE 0x100000
#define TRIV_IMAGE_VAR_FLOAT 0x200000
#define TRIV_IMAGE_VAR_DOUBLE 0x300000

typedef enum  {
    TRIV_IMAGE_UNDEFINED_TYPE = 0x000000,
    TRIV_IMAGE_BYTE_SCALAR_TYPE =   TRIV_IMAGE_VAR_BYTE   + 0x001000 + sizeof(unsigned char),
    TRIV_IMAGE_FLOAT_SCALAR_TYPE =  TRIV_IMAGE_VAR_FLOAT  + 0x001000 + sizeof(float),
    TRIV_IMAGE_DOUBLE_SCALAR_TYPE = TRIV_IMAGE_VAR_DOUBLE + 0x001000 + sizeof(double),
    TRIV_IMAGE_BYTE_ARGB_TYPE =     TRIV_IMAGE_VAR_BYTE   + 0x004000 + sizeof(unsigned char) * 4,
    TRIV_IMAGE_FLOAT_ARGB_TYPE =    TRIV_IMAGE_VAR_FLOAT  + 0x004000 + sizeof(float) * 4,
    TRIV_IMAGE_DOUBLE_ARGB_TYPE =   TRIV_IMAGE_VAR_DOUBLE + 0x004000 + sizeof(double) * 4
} TrivImagePixelType;

#define TRIV_IMAGE_PIXEL_SIZE(Type)  ((int) ((Type) & 0xfff))
#define TRIV_IMAGE_PIXEL_NUM_COORDS(Type)  ((int) (((Type) & 0x0ff000) >> 12))
#define TRIV_IMAGE_PIXEL_VAR_TYPE(Type)  ((int) ((Type) & 0xf00000))
#define TRIV_IMAGE_PIXEL_CREATE_BYTE_TYPE(N) (TRIV_IMAGE_VAR_BYTE + ((N) << 12) + sizeof(unsigned char) * (N))
#define TRIV_IMAGE_PIXEL_CREATE_FLOAT_TYPE(N) (TRIV_IMAGE_VAR_FLOAT + ((N) << 12) + sizeof(float) * (N))
#define TRIV_IMAGE_PIXEL_CREATE_DOUBLE_TYPE(N) (TRIV_IMAGE_VAR_DOUBLE + ((N) << 12) + sizeof(double) * (N))

#define TRIV_PREV_DIR(Dir) (((int) Dir) + 1 > ((int) TRIV_CONST_W_DIR) ? \
			TRIV_CONST_U_DIR : (TrivTVDirType) ((int) Dir) + 1)
#define TRIV_NEXT_DIR(Dir) (((int) Dir) - 1 < ((int) TRIV_CONST_U_DIR) ? \
			TRIV_CONST_W_DIR : (TrivTVDirType) ((int) Dir) - 1)
#define TRIV_DIR_TO_INT(Dir) \
		  Dir == TRIV_CONST_W_DIR ? 2 : \
                                            (Dir == TRIV_CONST_V_DIR ? 1 : 0))
#define TRIV_INT_TO_DIR(IDir) \
		  IDir == 2 ? TRIV_CONST_W_DIR \
		            : (IDir == 1 ? TRIV_CONST_V_DIR : TRIV_CONST_U_DIR)

typedef enum {
    TRIV_UVW_BNDRY_MASK = CAGD_UV_BNDRY_MASK,     /* 8 lower bits must be 0. */
    TRIV_NO_BNDRY = TRIV_UVW_BNDRY_MASK,
    TRIV_U_MIN_BNDRY = CAGD_U_MIN_BNDRY,
    TRIV_U_MAX_BNDRY = CAGD_U_MAX_BNDRY,
    TRIV_V_MIN_BNDRY = CAGD_V_MIN_BNDRY,
    TRIV_V_MAX_BNDRY = CAGD_V_MAX_BNDRY,
    TRIV_W_MIN_BNDRY = TRIV_UVW_BNDRY_MASK + 16,
    TRIV_W_MAX_BNDRY = TRIV_UVW_BNDRY_MASK + 32,
    TRIV_U_MINMAX_BNDRY = CAGD_U_MINMAX_BNDRY,
    TRIV_V_MINMAX_BNDRY = CAGD_V_MINMAX_BNDRY,
    TRIV_W_MINMAX_BNDRY = TRIV_W_MIN_BNDRY | TRIV_W_MAX_BNDRY
} TrivTVBndryType;

#define TRIV_TV_BNDRY_FILTER_MASK(B) ((B) & ~TRIV_UVW_BNDRY_MASK)

typedef enum {
    TRIV_GEOM_CONST,
    TRIV_GEOM_LINEAR,
    TRIV_GEOM_TV_OF_REV,
    TRIV_GEOM_TRILINEAR,
    TRIV_GEOM_EXTRUSION,
    TRIV_GEOM_RULED_TV,
    TRIV_GEOM_SWEEP_TV
} TrivIsGeometryType;

typedef enum {
    FILLETING_METHOD_RLD_VLM,
    FILLETING_METHOD_BOOL_SUM
} TrivFilletingMethodType;

typedef struct TrivTriangleStruct {
    struct TrivTriangleStruct *Pnext;
    struct IPAttributeStruct *Attr;
    struct {
	CagdPType Pt;
	CagdVType Nrml;
	TrivUVWType UVW;
    } T[3];
} TrivTriangleStruct;

typedef struct TrivTVStruct {
    struct TrivTVStruct *Pnext;
    struct IPAttributeStruct *Attr;
    TrivGeomType GType;
    CagdPointType PType;
    int ULength, VLength, WLength;/* Mesh size in tri-variate tensor product.*/
    int UVPlane;	  /* Should equal ULength * VLength for fast access. */
    int UOrder, VOrder, WOrder;      /* Order in trivariate (B-spline only). */
    CagdBType UPeriodic, VPeriodic, WPeriodic;   /* Valid only for B-spline. */
    CagdRType *Points[CAGD_MAX_PT_SIZE];     /* Pointer on each axis vector. */
    CagdRType *UKnotVector, *VKnotVector, *WKnotVector;
} TrivTVStruct;

typedef struct TrivTVBlockEvalStruct {
    CagdPType Pos;
    CagdRType Jcbn[3][3];
} TrivTVBlockEvalStruct;

typedef struct TrivTVBlockEvalGenInfoStruct *TrivTVBlockEvalGenInfoStructPtr;
typedef struct TrivTVCurvEvalGenInfoStruct *TrivTVCurvEvalGenInfoStructPtr;

typedef void (*TrivSetErrorFuncType)(TrivFatalErrorType ErrorFunc);
typedef CagdBType (*TrivTVTestingFuncType)(const TrivTVStruct *TV,
					   TrivTVDirType *Dir,
					   CagdRType *t);
typedef CagdBType (*TrivTVAdjSrfCmpFuncType)(const CagdSrfStruct *Srf1,
					     const CagdSrfStruct *Srf2,
					     CagdRType Eps,
					     int *Modified);

#define TRIV_IS_BEZIER_TV(TV)		((TV) -> GType == TRIV_TVBEZIER_TYPE)
#define TRIV_IS_BSPLINE_TV(TV)		((TV) -> GType == TRIV_TVBSPLINE_TYPE)
#define TRIV_IS_POWER_TV(TV)		((TV) -> GType == TRIV_TVPOWER_TYPE)

#define TRIV_IS_RATIONAL_PT(PType)	CAGD_IS_RATIONAL_PT(PType)
#define TRIV_IS_RATIONAL_TV(TV)		CAGD_IS_RATIONAL_PT((TV) -> PType)
#define TRIV_NUM_OF_PT_COORD(PType)	CAGD_NUM_OF_PT_COORD(PType)
#define TRIV_NUM_OF_TV_COORD(TV)	CAGD_NUM_OF_PT_COORD((TV) -> PType)

#define TRIV_PARAM_IN_DOMAIN(Prm, UMin, UMax, VMin, VMax, WMin, WMax) \
    (Prm[0] >= UMin && Prm[0] <= UMax && \
     Prm[1] >= VMin && Prm[1] <= VMax && \
     Prm[2] >= WMin && Prm[2] <= WMax)

#define TRIV_COERCE_TO_DOMAIN(Pt, UMin, UMax, VMin, VMax, WMin, WMax) \
	if (Pt[0] < UMin) \
	    Pt[0] = UMin; \
	if (Pt[0] > UMax) \
	    Pt[0] = UMax; \
	if (Pt[1] < VMin) \
	    Pt[1] = VMin; \
	if (Pt[1] > VMax) \
	    Pt[1] = VMax; \
	if (Pt[2] < WMin) \
	    Pt[2] = WMin; \
	if (Pt[2] > WMax) \
	    Pt[2] = WMax;

/******************************************************************************
*           +-----------------------+					      *
*       W  /                       /|					      *
*      /  /                       / |					      *
*     /  /	U -->		 /  |	    The mesh is ordered raw after raw *
*       +-----------------------+   |	or the increments along U are 1 while *
*   V | |P0                 Pi-1|   +	the increment along V is full raw.    *
*     v	|Pi                P2i-1|  /	    Once a full UV plane is complete  *
*	|			| /	W is incremented by 1.		      *
*	|Pn-i		    Pn-1|/          To encapsulate it, NEXTU/V/W are  *
*	+-----------------------+	defined below.			      *
******************************************************************************/
#define TRIV_NEXT_U(TV)			(1)
#define TRIV_NEXT_V(TV)			(TV -> ULength)
#define TRIV_NEXT_W(TV)			(TV -> UVPlane)
#define TRIV_MESH_UVW(TV, i, j, k)	((i) + (TV -> ULength) * (j) + (TV -> UVPlane) * (k))

#define TRIV_CTL_MESH_LENGTH(TV)	((TV) -> UVPlane * (TV) -> WLength)

/* If a trivariate is periodic, the control polygon/mesh should warp up.     */
/* Length does hold the real allocated length but the virtual periodic       */
/* length is a little larger. Note allocated KV's are larger.                */
#define TRIV_TV_UPT_LST_LEN(TV)	((TV) -> ULength + \
				 ((TV) -> UPeriodic ? (TV) -> UOrder - 1 : 0))
#define TRIV_TV_VPT_LST_LEN(TV)	((TV) -> VLength + \
				 ((TV) -> VPeriodic ? (TV) -> VOrder - 1 : 0))
#define TRIV_TV_WPT_LST_LEN(TV)	((TV) -> WLength + \
				 ((TV) -> WPeriodic ? (TV) -> WOrder - 1 : 0))
#define TRIV_IS_UPERIODIC_TV(TV)	((TV) -> UPeriodic)
#define TRIV_IS_VPERIODIC_TV(TV)	((TV) -> VPeriodic)
#define TRIV_IS_WPERIODIC_TV(TV)	((TV) -> WPeriodic)
#define TRIV_IS_PERIODIC_TV(TV)	(TRIV_IS_UPERIODIC_TV(TV) || \
				 TRIV_IS_VPERIODIC_TV(TV) || \
				 TRIV_IS_WPERIODIC_TV(TV))

#define TRIV_DEL_GEOM_TYPE(Obj)		 CAGD_DEL_GEOM_TYPE(Obj)
#define TRIV_SET_GEOM_TYPE(Obj, Geom)	 CAGD_SET_GEOM_TYPE(Obj, Geom)
#define TRIV_PROPAGATE_ATTR(NewObj, Obj) CAGD_PROPAGATE_ATTR(NewObj, Obj)

#define TRIV_TV_EVAL_SCALAR(Trivar, u, v, w, PtE1) \
	     { CagdRType _R[CAGD_MAX_PT_SIZE]; \
	       TrivTVEvalToData((Trivar), (u), (v), (w), _R); \
	       *PtE1 = TRIV_IS_RATIONAL_TV((Trivar)) ? _R[1] / _R[0] : _R[1]; }
#define TRIV_TV_EVAL_E3(Trivar, u, v, w, PtE3) \
		{ CagdRType _R[CAGD_MAX_PT_SIZE], *PR = _R; \
		  TrivTVEvalToData((Trivar), (u), (v), (w), _R); \
		  CagdCoerceToE3(PtE3, &PR, -1, (Trivar) -> PType); }

/******************************************************************************
* This macro is called when the library has detected an unrecoverable error.  *
* Default action is to call TrivFatalError, but you may want to reroute this  *
* to invoke your handler and recover yourself (by long jump for example).     *
******************************************************************************/
#define TRIV_FATAL_ERROR(Msg)	TrivFatalError(Msg)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/******************************************************************************
* General routines of the Triv library:					      *
******************************************************************************/
TrivTVStruct *TrivTVNew(TrivGeomType GType,
			CagdPointType PType,
			int ULength,
			int VLength,
			int WLength);
TrivTVStruct *TrivBspTVNew(int ULength,
			   int VLength,
			   int WLength,
			   int UOrder,
			   int VOrder,
			   int WOrder,
			   CagdPointType PType);
TrivTVStruct *TrivBspPeriodicTVNew(int ULength,
				   int VLength,
				   int WLength,
				   int UOrder,
				   int VOrder,
				   int WOrder,
				   CagdBType UPeriodic,
				   CagdBType VPeriodic,
				   CagdBType WPeriodic,
				   CagdPointType PType);
TrivTVStruct *TrivBzrTVNew(int ULength,
			   int VLength,
			   int WLength,
			   CagdPointType PType);
TrivTVStruct *TrivPwrTVNew(int ULength,
			   int VLength,
			   int WLength,
			   CagdPointType PType);
TrivTVStruct *TrivTVCopy(const TrivTVStruct *TV);
TrivTVStruct *TrivTVCopyList(const TrivTVStruct *TVList);
void TrivTVFree(TrivTVStruct *TV);
void TrivTVFreeList(TrivTVStruct *TVList);

#ifdef DEBUG
#define TrivTVFree(TV)         { TrivTVFree(TV); TV = NULL; }
#define TrivTVFreeList(TVList) { TrivTVFreeList(TVList); TVList = NULL; }
#endif /* DEBUG */

TrivTriangleStruct *TrivTriangleNew(void);
TrivTriangleStruct *TrivTriangleCopy(const TrivTriangleStruct *Triangle);
TrivTriangleStruct *TrivTriangleCopyList(const TrivTriangleStruct
					                        *TriangleList);
void TrivTriangleFree(TrivTriangleStruct *Triangle);
void TrivTriangleFreeList(TrivTriangleStruct *TriangleList);

#ifdef DEBUG
#define TrivTriangleFree(Triangle)         { TrivTriangleFree(Triangle); \
					     Triangle = NULL; }
#define TrivTriangleFreeList(TriangleList) { TrivTriangleFreeList(TriangleList); \
					     TriangleList = NULL; }
#endif /* DEBUG */

TrivTVStruct *TrivPrimCylinder(const CagdVType Center,
			       CagdRType Radius,
			       CagdRType Height,
			       CagdBType Rational);
TrivTVStruct *TrivPrimCone(const CagdVType Center,
			   CagdRType Radius,
			   CagdRType Height,
			   CagdBType Rational);
TrivTVStruct *TrivPrimCone2(const CagdVType Center,
			    CagdRType MajorRadius,
			    CagdRType MinorRadius,
			    CagdRType Height,
			    CagdBType Rational);
TrivTVStruct *TrivPrimSphere(const CagdVType Center,
			     CagdRType Radius,
			     CagdBType Rational);
TrivTVStruct *TrivPrimSphere2(const CagdVType Center,
			      CagdRType Radius,
			      CagdBType Rational);
TrivTVStruct *TrivPrimTorus(const CagdVType Center,
			    CagdRType MajorRadius,
			    CagdRType MinorRadius,
			    CagdBType Rational);

TrivTVStruct *TrivNSPrimBox(CagdRType MinX,
			    CagdRType MinY,
			    CagdRType MinZ,
			    CagdRType MaxX,
			    CagdRType MaxY,
			    CagdRType MaxZ);
TrivTVStruct *TrivNSPrimGenBox(const CagdPType P000,
			       const CagdPType P001,
			       const CagdPType P010,
			       const CagdPType P011,
			       const CagdPType P100,
			       const CagdPType P101,
			       const CagdPType P110,
			       const CagdPType P111);
TrivTVStruct *TrivNSPrimGenBox2(const CagdVType VecUMin,
				const CagdVType VecUMax,
				const CagdVType VecVMin,
				const CagdVType VecVMax,
				const CagdVType VecWMin,
				const CagdVType VecWMax,
				int Normalize);
TrivTVStruct *TrivNSPrimCylinder(const CagdVType Center,
				 CagdRType Radius,
				 CagdRType Height,
				 CagdBType Rational,
				 CagdRType InternalCubeEdge);
TrivTVStruct *TrivNSPrimCone(const CagdVType Center,
			     CagdRType Radius,
			     CagdRType Height,
			     CagdBType Rational,
			     CagdRType InternalCubeSize);
TrivTVStruct *TrivNSPrimCone2(const CagdVType Center,
			      CagdRType MajorRadius,
			      CagdRType MinorRadius,
			      CagdRType Height,
			      CagdBType Rational,
			      CagdRType InternalCubeEdge);
TrivTVStruct *TrivNSPrimSphere(const CagdVType Center,
			       CagdRType Radius,
			       CagdBType Rational,
			       CagdRType InternalCubeEdge);
TrivTVStruct *TrivNSPrimTorus(const CagdVType Center,
			      CagdRType MajorRadius,
			      CagdRType MinorRadius,
			      CagdBType Rational,
			      CagdRType InternalCubeEdge);

TrivTVStruct *TrivCnvrtBzr2BspTV(const TrivTVStruct *TV);
TrivTVStruct *TrivCnvrtBsp2BzrTV(const TrivTVStruct *TV);

void TrivTVTransform(TrivTVStruct *TV,
		     const CagdRType *Translate,
		     CagdRType Scale);
TrivTVStruct *TrivTVMatTransform(const TrivTVStruct *TV, CagdMType Mat);
TrivTVStruct *TrivTVListMatTransform(const TrivTVStruct *TVs, CagdMType Mat);
void TrivTVMatTransform2(TrivTVStruct *TV, CagdMType Mat);

TrivTVStruct *TrivCoerceTVsTo(const TrivTVStruct *TV, CagdPointType PType);
TrivTVStruct *TrivCoerceTVTo(const TrivTVStruct *TV, CagdPointType PType);

void TrivTVDomain(const TrivTVStruct *TV,
		  CagdRType *UMin,
		  CagdRType *UMax,
		  CagdRType *VMin,
		  CagdRType *VMax,
		  CagdRType *WMin,
		  CagdRType *WMax);
TrivTVStruct *TrivTVSetDomain(TrivTVStruct *TV,
			      CagdRType UMin,
			      CagdRType UMax,
			      CagdRType VMin,
			      CagdRType VMax,
			      CagdRType WMin,
			      CagdRType WMax);
TrivTVStruct *TrivTVSetDomain2(TrivTVStruct *TV,
			       CagdRType Min,
			       CagdRType Max,
			       TrivTVDirType Dir);
CagdBType TrivParamInDomain(const TrivTVStruct *TV,
			    CagdRType t,
			    TrivTVDirType Dir);
CagdBType TrivParamsInDomain(const TrivTVStruct *TV,
			     CagdRType u,
			     CagdRType v,
			     CagdRType w);

void TrivTVEvalToData(const TrivTVStruct *TV,
		      CagdRType u,
		      CagdRType v,
		      CagdRType w,
		      CagdRType *Pt);
CagdRType *TrivTVEvalMalloc(const TrivTVStruct *TV,
			 CagdRType u,
			 CagdRType v,
			 CagdRType w);
void TrivTVEval2ToData(const TrivTVStruct *TV,
		       CagdRType u,
		       CagdRType v,
		       CagdRType w,
		       CagdRType *Pt);
CagdRType *TrivTVEval2Malloc(const TrivTVStruct *TV,
			     CagdRType u,
			     CagdRType v,
			     CagdRType w);
CagdSrfStruct *TrivSrfFromTV(const TrivTVStruct *TV,
			     CagdRType t,
			     TrivTVDirType Dir,
			     int OrientBoundary);
CagdSrfStruct **TrivBndrySrfsFromTVToData(const TrivTVStruct *TV,
					  int OrientBoundary,
					  int FilterSimilar,
					  CagdSrfStruct **Srfs);
CagdSrfStruct *TrivBndrySrfsFromTVs(const TrivTVStruct *Trivars,
				    CagdRType Eps,
				    int OrientBoundary,
				    int FilterSelfSimilar,
				    int FilterDupSrfs);
CagdCrvStruct *TrivBndryEdgesFromTV(const TrivTVStruct *TV);
CagdPtStruct *TrivBndryCrnrsFromTV(const TrivTVStruct *TV);

CagdSrfStruct *TrivSrfFromMesh(const TrivTVStruct *TV,
			       int Index,
			       TrivTVDirType Dir);
void TrivSrfToMesh(const CagdSrfStruct *Srf,
		   int Index,
		   TrivTVDirType Dir,
		   TrivTVStruct *TV);
CagdRType *TrivTVMultEval(CagdRType *UKnotVector,
			  CagdRType *VKnotVector,
			  CagdRType *WKnotVector,
			  int ULength,
			  int VLength,
			  int WLength,
			  int UOrder,
			  int VOrder,
			  int WOrder,
			  CagdPType *Mesh,
			  CagdPType *Params,
			  int NumOfParams,
			  int *RetSize,
			  CagdBspBasisFuncMultEvalType EvalType);
TrivTVBlockEvalGenInfoStructPtr TrivTVBlockEvalInit(CagdRType *UKnotVector,
						    CagdRType *VKnotVector,
						    CagdRType *WKnotVector,
						    int Lengths[3],
						    int Orders[3],
						    int BlockSizes[3],
						    CagdPType *Params,
						    int NumOfParams[3]);
void TrivTVBlockEvalSetMesh(TrivTVBlockEvalGenInfoStructPtr TVBlock,
			    CagdPType *Mesh);
TrivTVBlockEvalStruct *TrivTVBlockEvalOnce(
				         TrivTVBlockEvalGenInfoStructPtr TVBlock,
				         int i, int j, int k);
void TrivTVBlockEvalDone(TrivTVBlockEvalGenInfoStructPtr TVBlock);
TrivTVStruct *TrivTVRegionFromTV(const TrivTVStruct *TV,
				 CagdRType t1,
				 CagdRType t2,
				 TrivTVDirType Dir);
TrivTVStruct *TrivTVRefineAtParams(const TrivTVStruct *TV,
				   TrivTVDirType Dir,
				   CagdBType Replace,
				   CagdRType *t,
				   int n);
TrivTVStruct *TrivBspTVKnotInsertNDiff(const TrivTVStruct *TV,
				       TrivTVDirType Dir,
				       int Replace,
				       const CagdRType *t,
				       int n);
TrivTVStruct *TrivTVDerive(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *TrivTVDeriveScalar(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *TrivBzrTVDerive(const TrivTVStruct *TV, 
			      TrivTVDirType Dir,
			      CagdBType DeriveScalar);
TrivTVStruct *TrivBzrTVDeriveScalar(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *TrivBspTVDerive(const TrivTVStruct *TV,
			      TrivTVDirType Dir,
			      CagdBType DeriveScalar);
TrivTVStruct *TrivBspTVDeriveScalar(const TrivTVStruct *TV, TrivTVDirType Dir);
CagdRType TrivTVEvalJacobian(const TrivTVStruct *TV,
			     CagdRType u,
			     CagdRType v,
			     CagdRType w);
TrivTVStruct *TrivTVSubdivAtParam(const TrivTVStruct *TV,
				  CagdRType t,
				  TrivTVDirType Dir);
TrivTVStruct *TrivTVsSubdivAtAllDetectedLocations(const TrivTVStruct *TV,
						  TrivTVTestingFuncType
						                TVTestFunc);
TrivTVStruct *TrivTVSubdivAtAllC0Discont(const TrivTVStruct *TV);
TrivTVStruct *TrivTVsSubdivAtAllC1Discont(const TrivTVStruct *TVs);
TrivTVStruct *TrivTVSubdivAtAllC0Discont(const TrivTVStruct *TV);
TrivTVStruct *TrivTVsSubdivAtAllC1Discont(const TrivTVStruct *TVs);
TrivTVStruct *TrivTVDegreeRaise(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *TrivTVDegreeRaiseN(const TrivTVStruct *TV,
				 TrivTVDirType Dir,
				 int NewOrder);
TrivTVStruct *TrivBspTVDegreeRaise(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *TrivBzrTVDegreeRaise(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *TrivTVBlossomDegreeRaise(const TrivTVStruct *TV,
				       TrivTVDirType Dir);
TrivTVStruct *TrivTVBlossomDegreeRaiseN(const TrivTVStruct *TV,
					int NewUOrder,
					int NewVOrder,
					int NewWOrder);
TrivTVStruct *TrivTVReverseDir(const TrivTVStruct *TV, TrivTVDirType Dir);
TrivTVStruct *TrivTVReverse2Dirs(const TrivTVStruct *TV,
				 TrivTVDirType Dir1,
				 TrivTVDirType Dir2);
TrivTVStruct *TrivMergeTVTV(const TrivTVStruct *CTV1,
			    const TrivTVStruct *CTV2,
			    TrivTVDirType Dir,
			    CagdBType Discont);
int TrivUpdateBndrySrfinTV(TrivTVStruct *TV,
			   const CagdSrfStruct *Srf,
			   TrivTVBndryType TVBndry);
int TrivMakeTVsCompatibleDomain(const TrivTVStruct *TV1, TrivTVStruct **TV2);
CagdBType TrivMakeTVsCompatible(TrivTVStruct **TV1,
				TrivTVStruct **TV2,
				CagdBType SameUOrder,
				CagdBType SameVOrder,
				CagdBType SameWOrder,
				CagdBType SameUKV,
				CagdBType SameVKV,
				CagdBType SameWKV);
CagdBBoxStruct *TrivTVBBox(const TrivTVStruct *TV, CagdBBoxStruct *BBox);
CagdBBoxStruct *TrivTVListBBox(const TrivTVStruct *TVs, CagdBBoxStruct *BBox);
CagdPolylineStruct *TrivTV2CtrlMesh(const TrivTVStruct *Trivar);
CagdRType TrivTVVolume(const TrivTVStruct *TV, CagdBType VolType);
CagdRType TrivSrfArea(const CagdSrfStruct *Srf, CagdBType VolType);

void TrivTVPointInclusionPrep(TrivTVStruct *TV, int n);
CagdBType TrivTVPointInclusion(TrivTVStruct *TV, const IrtPtType Pt);
void TrivTVPointInclusionFree(TrivTVStruct *TV);

TrivTVStruct *TrivInterpTrivar(const TrivTVStruct *TV);
TrivTVStruct *TrivTVInterpPts(const TrivTVStruct *PtGrid,
			      int UOrder,
			      int VOrder,
			      int WOrder,
			      int TVUSize,
			      int TVVSize,
			      int TVWSize);
TrivTVStruct *TrivTVInterpolate(const TrivTVStruct *PtGrid,
				int ULength,
				int VLength,
				int WLength,
				int UOrder,
				int VOrder,
				int WOrder);
TrivTVStruct *TrivTVInterpScatPts(const CagdCtlPtStruct *PtList,
				  int USize,
				  int VSize,
				  int WSize,
				  int UOrder,
				  int VOrder,
				  int WOrder,
				  CagdRType *UKV,
				  CagdRType *VKV,
				  CagdRType *WKV);
TrivTVStruct *TrivCnvrtCrvToTV(const CagdCrvStruct *Crv, TrivTVDirType Dir);
TrivTVStruct *TrivCnvrtSrfToTV(const CagdSrfStruct *Srf, TrivTVDirType Dir);
TrivTVStruct *TrivTVFromSrfs(const CagdSrfStruct *SrfList,
			     int OtherOrder,
			     CagdEndConditionType OtherEC,
			     IrtRType *OtherParamVals);
CagdRType *TrivTVInterpolateSrfsChordLenParams(const CagdSrfStruct *SrfList);
TrivTVStruct *TrivTVInterpolateSrfs(const CagdSrfStruct *SrfList,
				    int OtherOrder,
				    CagdEndConditionType OtherEC,
				    CagdParametrizationType OtherParam,
				    IrtRType *OtherParamVals);
TrivTVStruct *TrivRuledTV(const CagdSrfStruct *Srf1,
			  const CagdSrfStruct *Srf2,
			  int OtherOrder,
			  int OtherLen);
TrivTVStruct *TrivTrilinearSrf(const CagdPtStruct *Pt000,
			       const CagdPtStruct *Pt001,
			       const CagdPtStruct *Pt010,
			       const CagdPtStruct *Pt011,
			       const CagdPtStruct *Pt100,
			       const CagdPtStruct *Pt101,
			       const CagdPtStruct *Pt110,
			       const CagdPtStruct *Pt111,
			       CagdPointType PType);
TrivTVStruct *TrivExtrudeTV(const CagdSrfStruct *Srf,
			    const CagdVecStruct *Vec);
TrivTVStruct *TrivExtrudeTV2(const CagdSrfStruct *Srf,
			     const CagdCrvStruct *Crv);
TrivTVStruct *TrivZTwistExtrudeSrf(const CagdSrfStruct *Srf,
				   CagdBType Rational,
				   CagdRType ZPitch);
TrivTVStruct *TrivTVOfRev(const CagdSrfStruct *Srf);
TrivTVStruct *TrivTVOfRev2(const CagdSrfStruct *Srf,
			   CagdBType PolyApprox,
			   CagdRType StartAngle,
			   CagdRType EndAngle);
TrivTVStruct *TrivTVOfRevPolynomialApprox(const CagdSrfStruct *Srf);
TrivTVStruct *TrivTVOfRevAxis(const CagdSrfStruct *Srf,
			      const TrivP4DType AxisPoint,
			      const TrivV4DType AxisVector,
			      CagdBType PolyApprox);
CagdSrfStruct *TrivExtractSleeveSrf(const TrivTVStruct *TV);
TrivTVStruct *TrivSweepTV(const CagdSrfStruct *CrossSection,
			  const CagdCrvStruct *Axis,
			  const CagdCrvStruct *ScalingCrv,
			  CagdRType Scale,
			  const VoidPtr Frame,
			  int FrameOption);
TrivTVStruct *TrivSweepTVC1(const CagdSrfStruct *CrossSection,
			    const CagdCrvStruct *Axis,
			    const CagdCrvStruct *ScalingCrv,
			    CagdRType Scale,
			    const VoidPtr Frame,
			    int FrameOption,
			    CagdCrvCornerType CornerType,
			    CagdRType C1DiscontCropTol);
CagdSrfStruct *TrivSweepTVError(const TrivTVStruct *SweepTV,
				const CagdSrfStruct *CrossSection,
				const CagdCrvStruct *Axis,
				const CagdCrvStruct *ScalingCrv,
				CagdRType Scale);
TrivTVStruct *TrivEditSingleTVPt(const TrivTVStruct *TV,
				 CagdCtlPtStruct *CtlPt,
				 int UIndex,
				 int VIndex,
				 int WIndex,
				 CagdBType Write);
CagdBType TrivTVsSame(const TrivTVStruct *Tv1,
		      const TrivTVStruct *Tv2,
		      CagdRType Eps);

CagdBType TrivTVKnotHasC0Discont(const TrivTVStruct *TV,
			         TrivTVDirType *Dir,
			         CagdRType *t);
CagdBType TrivTVMeshC0Continuous(const TrivTVStruct *TV,
				 TrivTVDirType Dir,
				 int Idx);
CagdBType TrivTVIsMeshC0DiscontAt(const TrivTVStruct *TV,
			          int Dir,
			          CagdRType t);
CagdBType TrivTVKnotHasC1Discont(const TrivTVStruct *TV,
			         TrivTVDirType *Dir,
			         CagdRType *t);
CagdBType TrivTVMeshC1Continuous(const TrivTVStruct *TV,
				 TrivTVDirType Dir,
				 int Idx);
CagdBType TrivTVIsMeshC1DiscontAt(const TrivTVStruct *TV,
			          int Dir,
			          CagdRType t);
CagdBType TrivBspTVHasBezierKVs(const TrivTVStruct *TV);
CagdBType TrivBspTVHasOpenEC(const TrivTVStruct *TV);
CagdBType TrivBspTVHasOpenECDir(const TrivTVStruct *TV, TrivTVDirType Dir);
CagdBType TrivIsTVClosed(const TrivTVStruct *TV, int Dim);
void TrivTVGenAdjacencyInfo(TrivTVStruct *TVList,
			    TrivTVAdjSrfCmpFuncType SrfCmpFuncPtr,
			    TrivTVAdjSrfCmpFuncType SrfCmpApxFuncPtr,
			    CagdRType SrfGapTol,
			    CagdBType MarkUVWBndry);
void TrivTVGenAdjacencyInfo2(TrivTVStruct **TVVec,
			     int NumTVs,
			     TrivTVAdjSrfCmpFuncType SrfCmpFuncPtr,
			     TrivTVAdjSrfCmpFuncType SrfCmpApxFuncPtr,
			     CagdRType SrfGapTol,
			     CagdBType MarkUVWBndry);
TrivTVStruct *TrivMakeTVArrngmntCompatible(const TrivTVStruct *TVList);

void TrivDbg(const void *Obj);
#ifdef DEBUG
void TrivDbgDsp(const void *Obj);
#endif /* DEBUG */

/******************************************************************************
* Routines to handle basis function conversions.			      *
******************************************************************************/
TrivTVStruct *TrivCnvrtPeriodic2FloatTV(const TrivTVStruct *TV);
TrivTVStruct *TrivCnvrtFloat2OpenTV(const TrivTVStruct *TV);
TrivTVStruct *TrivTVOpenEnd(const TrivTVStruct *TV);

/******************************************************************************
* Metamorphosis and FFD of trivariates.					      *
******************************************************************************/
TrivTVStruct *TrivTwoTVsMorphing(const TrivTVStruct *TV1,
				 const TrivTVStruct *TV2,
				 CagdRType Blend);
void TrivFFDCtlMeshUsingTV(CagdRType **Points,
			   int Length,
			   CagdPointType PType,
			   const TrivTVStruct *DeformTV);
struct IPObjectStruct *TrivFFDObjectTV(struct IPObjectStruct *PObj,
				       const TrivTVStruct *DeformTV);
struct IPObjectStruct *TrivFFDTileObjectInTV(const struct IPObjectStruct *PObj,
					     const TrivTVStruct *DeformTV,
					     IrtRType UTimes,
					     IrtRType VTimes,
					     IrtRType WTimes,
					     int FitObj,
					     IrtRType CropBoundaries,
					     IrtRType MaxEdgeLen);
int TrivFFDTileCropBndries(struct IPObjectStruct *BndryTiles[3][3][3],
			   const struct IPObjectStruct *Tile,
			   IrtHmgnMatType Mat,
			   IrtRType CropBoundaries);
void TrivFFDTileFreeBndries(struct IPObjectStruct *BndryTiles[3][3][3]);

/******************************************************************************
* Local curvature processing.						      *
******************************************************************************/
TrivTVCurvEvalGenInfoStructPtr TrivEvalTVCurvaturePrelude(
						      const TrivTVStruct *TV);
CagdBType TrivEvalCurvature(TrivTVCurvEvalGenInfoStructPtr TrivTVCurvature,
			    CagdPType Pos,
			    CagdRType *PCurv1,
			    CagdRType *PCurv2,
			    CagdVType PDir1,
			    CagdVType PDir2);
CagdBType TrivEvalGradient(TrivTVCurvEvalGenInfoStructPtr TrivTVCurvature,
			   CagdPType Pos,
			   CagdVType Gradient);
CagdBType TrivEvalHessian(TrivTVCurvEvalGenInfoStructPtr TrivTVCurvature,
			  CagdPType Pos,
			  CagdVType Hessian[3]);
void TrivEvalTVCurvaturePostlude(TrivTVCurvEvalGenInfoStructPtr
				                            TrivTVCurvature);

/******************************************************************************
* Geometry in R^4.							      *
******************************************************************************/
int TrivPlaneFrom4Points(const TrivP4DType Pt1,
			 const TrivP4DType Pt2,
			 const TrivP4DType Pt3,
			 const TrivP4DType Pt4,
			 TrivPln4DType Plane);
void TrivVectCross3Vecs(const TrivV4DType A,
			const TrivV4DType B,
			const TrivV4DType C,
			TrivV4DType Res);

/******************************************************************************
* Routines to compute trivariate compositions.				      *
******************************************************************************/
struct IPObjectStruct *TrivComposeTileObjectInTV(
					    const struct IPObjectStruct *PObj,
					    const TrivTVStruct *DeformTV,
					    IrtRType UTimes,
					    IrtRType VTimes,
					    IrtRType WTimes,
					    int FitObj,
					    IrtRType CropBoundaries);
struct IPObjectStruct *TrivComposeTileObjectInTVBzr(
					     const struct IPObjectStruct *PObj,
					     const TrivTVStruct *DeformTV,
					     IrtRType UTimes,
					     IrtRType VTimes,
					     IrtRType WTimes,
					     int FitObj);
struct IPObjectStruct *TrivComposeOneObjectInTVBzr(
					     const struct IPObjectStruct *PObj,
					     const TrivTVStruct *DeformTV);
CagdCrvStruct *TrivComposeTVCrv(const TrivTVStruct *TV,
				const CagdCrvStruct *Crv);
CagdCrvStruct *TrivBzrComposeTVCrv(const TrivTVStruct *TV,
				   const CagdCrvStruct *Crv);
CagdSrfStruct *TrivComposeTVSrf(const TrivTVStruct *TV,
				const CagdSrfStruct *Srf);
CagdSrfStruct *TrivBzrComposeTVSrf(const TrivTVStruct *TV,
				   const CagdSrfStruct *Srf);
TrivTVStruct *TrivComposeTVTV(const TrivTVStruct *TV1,
			      const TrivTVStruct *TV2);

/******************************************************************************
* Routines to handle adaptive extraction of surfaces and curves from trivars. *
******************************************************************************/
struct TrimSrfStruct *TrivAdapIsoExtractSrfs(const TrivTVStruct *Trivar,
					     TrivTVDirType Dir,
					     CagdRType Epsilon,
					     int InitialDiv,
					     CagdRType CntrEps);
CagdCrvStruct *TrivAdapIsoExtractCrvs(const TrivTVStruct *Trivar,
				      TrivTVDirType SrfDir,
				      CagdRType Epsilon,
				      int InitialDiv,
				      CagdSrfDirType CrvDir,
				      CagdRType CntrEps);

/******************************************************************************
* Routines to handle inverse queries over trivariates.			      *
******************************************************************************/
struct TrivInverseQueryStruct *TrivPrepInverseQueries(const TrivTVStruct
						                     *Trivar);
int TrivInverseQuery(struct TrivInverseQueryStruct *Handle,
		     const CagdRType *XYZPos,
		     CagdRType *UVWParams,
		     int InitialGuess);
void TrivFreeInverseQueries(struct TrivInverseQueryStruct *Handle);
struct TrivInverseQueryStruct *TrivCopyInverseQueries(
				  const struct TrivInverseQueryStruct *Handle);
int TrivInverseQueryPolys(struct IPObjectStruct *PlObj,
			  const TrivTVStruct *TV);

/******************************************************************************
* Symbolic computations routines.					      *
******************************************************************************/
TrivTVStruct *TrivTVAdd(const TrivTVStruct *TV1, const TrivTVStruct *TV2);
TrivTVStruct *TrivTVSub(const TrivTVStruct *TV1, const TrivTVStruct *TV2);
TrivTVStruct *TrivTVMult(const TrivTVStruct *TV1, const TrivTVStruct *TV2);
TrivTVStruct *TrivTVInvert(const TrivTVStruct *TV);
TrivTVStruct *TrivTVMultScalar(const TrivTVStruct *TV1,
			       const TrivTVStruct *TV2);
TrivTVStruct *TrivTVDotProd(const TrivTVStruct *TV1, const TrivTVStruct *TV2);
TrivTVStruct *TrivTVVecDotProd(const TrivTVStruct *TV, const CagdVType Vec);
TrivTVStruct *TrivTVCrossProd(const TrivTVStruct *TV1, const TrivTVStruct *TV2);
TrivTVStruct *TrivTVRtnlMult(const TrivTVStruct *TV1X,
			     const TrivTVStruct *TV1W,
			     const TrivTVStruct *TV2X,
			     const TrivTVStruct *TV2W,
			     CagdBType OperationAdd);

TrivTVStruct **TrivTVSplitScalarNToData(const TrivTVStruct *TV,
					TrivTVStruct **Tvs);
void TrivTVSplitScalar(const TrivTVStruct *TV,
		       TrivTVStruct **TVW,
		       TrivTVStruct **TVX,
		       TrivTVStruct **TVY,
		       TrivTVStruct **TVZ);
TrivTVStruct *TrivTVMergeScalarN(TrivTVStruct * const *TVVec, int NumTVs);
TrivTVStruct *TrivTVMergeScalar(const TrivTVStruct *TVW,
				const TrivTVStruct *TVX,
				const TrivTVStruct *TVY,
				const TrivTVStruct *TVZ);

/******************************************************************************
* Special trivariate constructors.					      *
******************************************************************************/
TrivTVStruct *TrivAlgebraicSumTV(const CagdCrvStruct *Crv,
				 const CagdSrfStruct *Srf);
TrivTVStruct *TrivAlgebraicProdTV(const CagdCrvStruct *Crv,
				  const CagdSrfStruct *Srf);
TrivTVStruct *TrivSwungAlgSumTV(const CagdCrvStruct *Crv,
				const CagdSrfStruct *Srf);

/******************************************************************************
* Fillets methods.                                                            *
******************************************************************************/
TrivTVStruct *TrivTVFillet(const CagdSrfStruct *Srf1List,
                           const CagdSrfStruct *Srf2List,
                           CagdRType RailDist,
                           int R1Orient,
                           int R2Orient,
                           CagdRType TanScale,
                           int ApproxCrvsCtlPts,
                           CagdRType Tol,
                           CagdRType NumerTol,
                           TrivFilletingMethodType FilletingMethod,
                           CagdBType PreciseFillet,
                           CagdSrfStruct **PrimSrfs1,
                           CagdSrfStruct **PrimSrfs2);

void TrivBlendFilletProperties(TrivTVStruct **FilletTV,
                               const CagdSrfStruct *PrimSrf1,
                               const CagdSrfStruct *PrimSrf2);

/******************************************************************************
* Error handling.							      *
******************************************************************************/
TrivSetErrorFuncType TrivSetFatalErrorFunc(TrivSetErrorFuncType ErrorFunc);
void TrivFatalError(TrivFatalErrorType ErrID);
const char *TrivDescribeError(TrivFatalErrorType ErrID);

/******************************************************************************
* Trivariate fitting methods.                                                 *
******************************************************************************/
TrivTVStruct *TrivFitTV2PolyMesh(struct IPObjectStruct *MeshObj, 
				 struct IPObjectStruct *MedialAxisObj,
				 IrtRType MeanErrThr, 
				 IrtRType HausdorffErrThr);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_TRIV_LIB_H */
