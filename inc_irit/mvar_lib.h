/******************************************************************************
* MVar_lib.h - header file for the multi variate library.		      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber, May. 96.					      *
******************************************************************************/

#ifndef IRIT_MVAR_LIB_H
#define IRIT_MVAR_LIB_H

#include <stdio.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/geom_lib.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/trim_lib.h"
#include "inc_irit/triv_lib.h"
#include "inc_irit/mdl_lib.h"

typedef enum {
    MVAR_ERR_DIR_NOT_VALID,
    MVAR_ERR_UNDEF_CRV,
    MVAR_ERR_UNDEF_SRF,
    MVAR_ERR_UNDEF_MVAR,
    MVAR_ERR_UNDEF_GEOM,
    MVAR_ERR_GEOM_NO_SUPPORT,
    MVAR_ERR_PERIODIC_NO_SUPPORT,
    MVAR_ERR_RATIONAL_NO_SUPPORT,
    MVAR_ERR_RATIONAL_EXPECTED,
    MVAR_ERR_WRONG_ORDER,
    MVAR_ERR_KNOT_NOT_ORDERED,
    MVAR_ERR_NUM_KNOT_MISMATCH,
    MVAR_ERR_INDEX_NOT_IN_MESH,
    MVAR_ERR_POWER_NO_SUPPORT,
    MVAR_ERR_WRONG_DOMAIN,
    MVAR_ERR_INCONS_DOMAIN,
    MVAR_ERR_SCALAR_PT_EXPECTED,
    MVAR_ERR_WRONG_PT_TYPE,
    MVAR_ERR_INVALID_AXIS,
    MVAR_ERR_NO_CLOSED_POLYGON,
    MVAR_ERR_TWO_INTERSECTIONS,
    MVAR_ERR_NO_MATCH_PAIR,
    MVAR_ERR_FAIL_READ_FILE,
    MVAR_ERR_INVALID_STROKE_TYPE,
    MVAR_ERR_READ_FAIL,
    MVAR_ERR_MVS_INCOMPATIBLE,
    MVAR_ERR_PT_OR_LEN_MISMATCH,
    MVAR_ERR_TOO_FEW_PARAMS,
    MVAR_ERR_TOO_MANY_PARAMS,
    MVAR_ERR_FAIL_CMPT,
    MVAR_ERR_NO_CROSS_PROD,
    MVAR_ERR_BEZIER_EXPECTED,
    MVAR_ERR_BSPLINE_EXPECTED,
    MVAR_ERR_BEZ_OR_BSP_EXPECTED,
    MVAR_ERR_SAME_GTYPE_EXPECTED,
    MVAR_ERR_SAME_PTYPE_EXPECTED,
    MVAR_ERR_ONE_OR_THREE_EXPECTED,
    MVAR_ERR_POWER_EXPECTED,
    MVAR_ERR_MSC_TOO_FEW_OBJ,
    MVAR_ERR_MSC_FAILED,
    MVAR_ERR_MSS_INCONSISTENT_NUM_OBJ,
    MVAR_ERR_SCALAR_EXPECTED,
    MVAR_ERR_DIM_TOO_HIGH,
    MVAR_ERR_INVALID_MV,
    MVAR_ERR_CANNT_MIX_BSP_BEZ,
    MVAR_ERR_CH_FAILED,
    MVAR_ERR_MSC_CURVES,
    MVAR_ERR_ROUND_CURVE,
    MVAR_ERR_ONLY_2D,
    MVAR_ERR_ONLY_3D,
    MVAR_ERR_2D_OR_3D,
    MVAR_ERR_1D_OR_3D,
    MVAR_ERR_WRONG_INDEX,
    MVAR_ERR_MSC_TOO_FEW_PTS,
    MVAR_ERR_ET_DFRNT_DOMAINS,
    MVAR_ERR_SRF_NOT_ADJ,
    MVAR_ERR_CURVATURE_CONT,
    MVAR_ERR_ZER_PRBLM_CNSTRCT,	
    MVAR_ERR_ZER_ORDER_CNSTRCT,
    MVAR_ERR_ZER_NUMERIC_TOO_EARLY,
    MVAR_ERR_ZER_SOL_CNSTRCT,
    MVAR_ERR_ZER_SCT_TOO_EARLY,
    MVAR_ERR_ZER_GZT_TOO_EARLY,
    MVAR_ERR_TWO_SAME_MVS,
    MVAR_ERR_TWO_SAME_INPUTS,
    MVAR_ERR_ZER_SINGULAR_SOLS,
    MVAR_ERR_ZER_CRT_PTS_NO_SUPPORT,
    MVAR_ERR_EXPR_TREE_NO_SUPPORT,
    MVAR_ERR_INVALID_INPUT,
    MVAR_ERR_INV_PROJ_FAILED,
    MVAR_ERR_INVALID_COMPOSE_INPUT,
    MVAR_ERR_EXPECTED_CLOSED_SRF,
    MVAR_ERR_DIV_BY_ZERO,
    MVAR_ERR_UNDEFINE_ERR
} MvarFatalErrorType;

typedef enum {
    MVAR_UNDEF_TYPE = 1240,
    MVAR_BEZIER_TYPE,
    MVAR_BSPLINE_TYPE,
    MVAR_POWER_TYPE
} MvarGeomType;

typedef enum {
    MVAR_CNSTRNT_ZERO = 1320,
    MVAR_CNSTRNT_ZERO_SUBDIV,    /* Examine zeros during subdiv. stage only. */
    MVAR_CNSTRNT_POSITIVE,  /* Examine positivity during subdiv. stage only. */
    MVAR_CNSTRNT_POSITIVE_PARTIAL,/* Partial positivity in subdiv.stage only.*/
    MVAR_CNSTRNT_NEGATIVE,  /* Examine negativity during subdiv. stage only. */
    MVAR_CNSTRNT_NEGATIVE_PARTIAL /* Partial negativity on subdiv.stage only.*/
} MvarConstraintType;

/* This flag determines what to do with domain's center upon reaching subdiv */
/* tolerance, in zero dim solutions.					     */
typedef enum {
    MVAR_ZERO_ALWAYS_PURGE = 0, /* Return nothing when reaching subdiv tol'. */
    MVAR_ZERO_NEVER_PURGE,            /* Return the domain's center, always. */
    MVAR_ZERO_RETURN_VERIFIED,    /* Return the center if L1 error is small. */
    MVAR_ZERO_NUMERIC_STEP,   /* Try to improve numerically from the center. */
    MVAR_ZERO_NUMERIC_STEP_VERIFIED   /* Improve numerically from the center */
                     /* and then verify the L1 error of the answer is small. */
} MvarZeroSubdivTolActionType;

/* The following type should match CagdPointType for the shared domain.      */
typedef enum {		/* Type of control point. The P-types are rationals. */
    MVAR_PT_NONE = CAGD_PT_NONE,
    MVAR_PT_BASE = CAGD_PT_BASE,                  /* Must be an even number. */
    MVAR_PT_E1_TYPE = CAGD_PT_E1_TYPE,
    MVAR_PT_P1_TYPE,
    MVAR_PT_E2_TYPE,
    MVAR_PT_P2_TYPE,
    MVAR_PT_E3_TYPE,
    MVAR_PT_P3_TYPE,
    MVAR_PT_E4_TYPE,
    MVAR_PT_P4_TYPE,
    MVAR_PT_E5_TYPE,
    MVAR_PT_P5_TYPE,
    MVAR_PT_E6_TYPE,
    MVAR_PT_P6_TYPE,
    MVAR_PT_E7_TYPE,
    MVAR_PT_P7_TYPE,
    MVAR_PT_E8_TYPE,
    MVAR_PT_P8_TYPE,
    MVAR_PT_E9_TYPE,
    MVAR_PT_P9_TYPE,
    MVAR_PT_E10_TYPE,
    MVAR_PT_P10_TYPE,
    MVAR_PT_E11_TYPE,
    MVAR_PT_P11_TYPE,
    MVAR_PT_E12_TYPE,
    MVAR_PT_P12_TYPE,
    MVAR_PT_E13_TYPE,
    MVAR_PT_P13_TYPE,
    MVAR_PT_E14_TYPE,
    MVAR_PT_P14_TYPE,
    MVAR_PT_E15_TYPE,
    MVAR_PT_P15_TYPE,
    MVAR_PT_E16_TYPE,
    MVAR_PT_P16_TYPE,
    MVAR_PT_E17_TYPE,
    MVAR_PT_P17_TYPE,
    MVAR_PT_E18_TYPE,
    MVAR_PT_P18_TYPE,
    MVAR_PT_E19_TYPE,
    MVAR_PT_P19_TYPE,
    MVAR_PT_MAX_SIZE_TYPE	     /* See also MVAR_MAX_* constants below. */
} MvarPointType;

#define MVAR_MAX_PT_SIZE		20    /* Rational P19 has 20 coords. */
#define MVAR_MAX_PT_COORD		19		       /* Without w. */

typedef enum {
    MVAR_SK2D_PRIM_POINT,
    MVAR_SK2D_PRIM_LINE,
    MVAR_SK2D_PRIM_ARC,
    MVAR_SK2D_PRIM_CRV
} MvarSkel2DPrimType;

typedef enum {
    MVAR_ET_NODE_NONE,
    MVAR_ET_NODE_LEAF,
    MVAR_ET_NODE_ADD,
    MVAR_ET_NODE_SUB,
    MVAR_ET_NODE_MULT,
    MVAR_ET_NODE_SCALAR_MULT,
    MVAR_ET_NODE_MERGE,
    MVAR_ET_NODE_DOT_PROD,
    MVAR_ET_NODE_CROSS_PROD,
    MVAR_ET_NODE_EXP,
    MVAR_ET_NODE_LOG,
    MVAR_ET_NODE_COS,
    MVAR_ET_NODE_SQRT,
    MVAR_ET_NODE_SQR,
    MVAR_ET_NODE_NPOW,
    MVAR_ET_NODE_RECIP,
    MVAR_ET_NODE_COMMON_EXPR
} MvarExprTreeNodeType;

typedef enum {
    MVAR_ZER_SLVR_UNKNOWN,
    MVAR_ZER_SLVR_MVS,
    MVAR_ZER_SLVR_EXP_TREE,
    MVAR_ZER_SLVR_TRANSCEND,
    MVAR_ZER_SLVR_SOLUTION_POLYLINE,
    MVAR_ZER_SLVR_SOLUTION_PT_LIST,
    MVAR_ZER_SLVR_SOLUTION_TR_LIST,
    MVAR_ZER_SLVR_SOLUTION_DETECT
} MvarZrSlvrRepresentationType;

typedef enum {
    MVAR_MVD_NO_DECOMPOSITION,
    MVAR_MVD_DECOMPOSITION_COMPOSITION,
    MVAR_MVD_DECOMPOSITION_POINT_TRACING
} MvarMVDDecompositionModeType;

typedef int MvarMVDirType;
typedef CagdRType MvarMinMaxType[2];

typedef struct MvarComposedSrfStruct {
    struct MvarComposedSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdBType IsTrimmedSurface;
    union {
	CagdSrfStruct *Srf;
	TrimSrfStruct *TSrf;
    } U;
} MvarComposedSrfStruct;

typedef struct MvarComposedTrivStruct
{
    struct MvarComposedTrivStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdBType IsVMdl;
    union MyUnion {
	TrivTVStruct *TV;
	struct VMdlVModelStruct *VMdl;
    } U;
} MvarComposedTrivStruct;

/* This struct will hold the result of the constructed surface based        */
/* kernel. The struct variables described as follows:                       */
/*  ResStatus          0 - Faild to construct a valid surface.              */
/*  		       1 - Successed to construct a valid surface without   */
/*                         add more dofs to the input curves.               */
/*		       2 - Successed to construct a valid surface with add  */
/*                         more dofs to the input curves.		    */
/*  DegRaisItrNum:     Hold the number of degree raising iteration needed   */
/*                     to construct a surface with a valid parameterization.*/
/*                     -1, if not succeeded.                                */
/*  KnotInsItrNum:     Hold the number of knot insertion iteration needed   */
/*                     to construct a surface with a valid parameterization.*/
/*                     -1, if not succeeded.                                */
/*  KrnlSrf:           The kernel surface constructed upon the given input  */
/*                     curves, without add more dofs.                       */
/*  RegularSrf:        The regular surface constructed by using the regular */
/*                     operator on the given input curves.                  */
/*  InitCpStateSrf:    Holds the kernel surface initial state. i.e. before  */
/*		       moving the inner control points toward the kernel    */
/*                     point. The inner control points will be ordered in a */
/*                     unifrom gride. The surface construcetd upon the      */
/*                     given input curves.                                  */
/*  DegRaisSrf:        The kernel surface constructed by using the kernel   */
/*                     operator variation, while adding more dofs by        */
/*                     raising the degree of the input curves.              */
/*  DegRaisRglrSrf:    The regular surface constructed by using the regular */
/*                     operator, while adding more dofs by raising the      */
/*                     degree of the input curves.                          */
/*  DegRaisInitSrf:    Holds the kernel surface initial state. i.e. before  */
/*		       moving the inner control points toward the kernel    */
/*                     point. The inner control points will be ordered in a */
/*                     unifrom gride. The surface construcetd upon the      */
/*                     degree raised input curves.                          */
/*  KnotInsSrf:        The kernel surface constructed by using the kernel   */
/*                     operator variation, while adding more dofs by        */
/*                     inserting unifrom knots into the input curves.       */
/*  KnotInsRglrSrf:    The regular surface constructed by using the regular */
/*                     operator, while adding more dofs by inserting        */
/*                     unifrom knots into the input curves.                 */
/*  KnotInsInitSrf:    Holds the kernel surface initial state. i.e. before  */
/*		       moving the inner control points toward the kernel    */
/*                     point. The inner control points will be ordered in a */
/*                     unifrom gride. The surface construcetd upon the      */
/*                     refined input curves.                                */
/*  CnvxHull:          The convex hull of the evaluated kernel points. NULL */
/*                     If there is no kernel points or the convex hull      */
/*                     provided by the user and not computed.               */
typedef struct MvarPlnrKrnlSrfRes 
{
    int ResStatus;
    int DegRaisItrNum;
    int KnotInsItrNum;
    CagdSrfStruct *KrnlSrf;
    CagdSrfStruct *RegularSrf;
    CagdSrfStruct *InitCpStateSrf;
    CagdSrfStruct *DegRaisSrf;
    CagdSrfStruct *DegRaisRglrSrf;
    CagdSrfStruct *DegRaisInitSrf;
    CagdSrfStruct *KnotInsSrf;
    CagdSrfStruct *KnotInsRglrSrf;
    CagdSrfStruct *KnotInsInitSrf;
    struct IPObjectStruct *CnvxHull;
} MvarPlnrKrnlSrfRes;

typedef struct MvarKrnlTVRes 
{
    int ResStatus;
    int DegRaisItrNum;
    int KnotInsItrNum;
    TrivTVStruct *KrnlTV;
    TrivTVStruct *RegularTV;
    TrivTVStruct *InitCpStateTV;
    TrivTVStruct *DegRaisTV;
    TrivTVStruct *DegRaisRglrTV;
    TrivTVStruct *DegRaisInitTV;
    TrivTVStruct *KnotInsTV;
    TrivTVStruct *KnotInsRglrTV;
    TrivTVStruct *KnotInsInitTV;
    struct IPObjectStruct *KrnlPts;
    IrtPtType KrnlPt;
} MvarKrnlTVRes;

#define MVAR_HF_DIST_MAX_PARAM		3

#define MVAR_IS_RATIONAL_PT(PType)  ((int) ((PType) & 0x01))
#define MVAR_IS_RATIONAL_MV(MV)		MVAR_IS_RATIONAL_PT((MV) -> PType)
#define MVAR_NUM_OF_PT_COORD(PType) ((((int) ((PType) - MVAR_PT_BASE)) >> 1) + 1)
#define MVAR_NUM_OF_MV_COORD(MV)    ((((int) (((MV) -> PType) - \
				              MVAR_PT_BASE)) >> 1) + 1)
#define MVAR_MAKE_PT_TYPE(IsRational, NumCoords) \
				    ((MvarPointType) (MVAR_PT_BASE + \
				         ((((IsRational) ? -1 : -2) \
						       + ((NumCoords) << 1)))))

#define MVAR_PREV_DIR(Dir) ((Dir) + 1)
#define MVAR_NEXT_DIR(Dir) ((Dir) - 1)

#define MVAR_PT_RESET(P) IRIT_ZAP_MEM((P) -> Pt, (P) -> Dim * sizeof(CagdRType))
#define MVAR_VEC_RESET(V) IRIT_ZAP_MEM((V) -> Vec, (V) -> Dim * sizeof(CagdRType))
#define MVAR_PLANE_RESET(P) IRIT_ZAP_MEM((P) -> Pln, (P) -> Dim * sizeof(CagdRType))

#define MVAR_PT_COPY(Dst, Src) IRIT_GEN_COPY((Dst) -> Pt, (Src) -> Pt, \
				             (Dst) -> Dim * sizeof(CagdRType))
#define MVAR_VEC_COPY(Dst, Src) IRIT_GEN_COPY((Dst) -> Vec, (Src) -> Vec, \
				              (Dst) -> Dim * sizeof(CagdRType))
#define MVAR_PLANE_COPY(Dst, Src) IRIT_GEN_COPY((Dst) -> Plane, (Src) -> Plane, \
				                (Dst) -> Dim * sizeof(CagdRType))

#define MVAR_MV_EVAL_SCALAR(Mv, Params, PtE1) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE]; \
		  MvarMVEvalToData((Mv), (Params), _R); \
		  *(PtE1) = MVAR_IS_RATIONAL_MV((Mv)) ? _R[1] / _R[0] : _R[1]; }
#define MVAR_MV_EVAL_E2(Mv, Params, PtE2) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE], *PR = _R; \
		  MvarMVEvalToData((Mv), (Params), _R); \
		  CagdCoerceToE2(PtE2, &PR, -1, (CagdPointType) ((Mv) -> PType)); }
#define MVAR_MV_EVAL_E3(Mv, Params, PtE3) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE], *PR = _R; \
		  MvarMVEvalToData((Mv), (Params), _R); \
		  CagdCoerceToE3(PtE3, &PR, -1, (CagdPointType) ((Mv) -> PType)); }
#define MVAR_MV_EVAL_P2(Mv, Params, PtP2) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE], *PR = _R; \
		  MvarMVEvalToData((Mv), (Params), _R); \
		  CagdCoerceToP2(PtP2, &PR, -1, (CagdPointType) ((Mv) -> PType)); }
#define MVAR_MV_EVAL_P3(Mv, Params, PtP3) \
		{ CagdRType _R[MVAR_MAX_PT_SIZE], *PR = _R; \
		  MvarMVEvalToData((Mv), (Params), _R); \
		  CagdCoerceToP3(PtP3, &PR, -1, (CagdPointType) ((Mv) -> PType)); }

#define MVAR_MVS_ZERO_INIT_PROBLEM_SPEC(ZeroProblemSpec, Multivars, \
				        Cnstrs, NumMVs, \
				        SubTol, NumTol, StpTol) { \
    IRIT_ZAP_MEM(&ZeroProblemSpec, sizeof(MvarZeroPrblmSpecStruct)); \
    ZeroProblemSpec.U.MVs = Multivars; \
    ZeroProblemSpec.Constraints = Cnstrs; \
    ZeroProblemSpec.NumOfMVs = NumMVs; \
    ZeroProblemSpec.SubdivTol = SubTol; \
    ZeroProblemSpec.NumericTol = NumTol; \
    ZeroProblemSpec.StepTol = StpTol; \
}

#define MVAR_EQS_ZERO_INIT_PROBLEM_SPEC(ZeroProblemSpec, ExprTreeEqns, \
				        Cnstrs, NumMVs, \
				        SubTol, NumTol, StpTol) { \
    IRIT_ZAP_MEM(&ZeroProblemSpec, sizeof(MvarZeroPrblmSpecStruct)); \
    ZeroProblemSpec.U.MVETs = ExprTreeEqns; \
    ZeroProblemSpec.Constraints = Cnstrs; \
    ZeroProblemSpec.NumOfMVs = NumMVs; \
    ZeroProblemSpec.SubdivTol = SubTol; \
    ZeroProblemSpec.NumericTol = NumTol; \
    ZeroProblemSpec.StepTol = StpTol; \
}

typedef struct MvarPtStruct {
    struct MvarPtStruct *Pnext;
    struct IPAttributeStruct *Attr;
    int Dim;				     /* Number of coordinates in Pt. */
    CagdRType *Pt;	       /* The coordinates of the multivariate point. */
} MvarPtStruct;

typedef struct MvarVecStruct {
    struct MvarVecStruct *Pnext;
    struct IPAttributeStruct *Attr;
    int Dim;				    /* Number of coordinates in Vec. */
    CagdRType *Vec;	      /* The coordinates of the multivariate vector. */
} MvarVecStruct;

typedef struct MvarPolylineStruct {		    /* A polyline structure. */
    struct MvarPolylineStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MvarPtStruct *Pl;
    VoidPtr PAux;
} MvarPolylineStruct;

typedef struct MvarTriangleStruct {		    /* A triangle structure. */
    struct MvarTriangleStruct *Pnext;		    
    int Dim;
    CagdRType *Vrtcs[3];
    CagdRType *Nrmls[3];
} MvarTriangleStruct;

typedef struct MvarPlaneStruct {
    struct MvarPlaneStruct *Pnext;
    struct IPAttributeStruct *Attr;
    int Dim;    /* Number of coordinates in Pln (one above space dimension). */
    CagdRType *Pln;	       /* The coordinates of the multivariate plane. */
} MvarPlaneStruct;


typedef struct GMBBBboxStruct MvarBBoxStruct;

typedef struct MvarNormalConeStruct {        /* Normalized cone axis vector. */
    struct MvarNormalConeStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MvarVecStruct *ConeAxis;
    CagdRType ConeAngleCosine;
    CagdRType AxisMinMax[2];
} MvarNormalConeStruct;

typedef struct MvarMVStruct {
    struct MvarMVStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MvarGeomType GType;
    MvarPointType PType;
    int Dim;		      /* Number of dimensions in this multi variate. */
    int *Lengths;               /* Dimensions of mesh size in multi-variate. */
    int *SubSpaces;	   /* SubSpaces[i] = Prod(i = 0, i-1) of Lengths[i]. */
    int *Orders;                 /* Orders of multi variate (B-spline only). */
    CagdBType *Periodic;           /* Periodicity - valid only for B-spline. */
    CagdRType *Points[MVAR_MAX_PT_SIZE];     /* Pointer on each axis vector. */
    CagdRType **KnotVectors;
    MvarMinMaxType *AuxDomain;		      /* Optional to hold MV domain. */
    VoidPtr PAux;			        /* Auxiliary data structure. */
    VoidPtr PAux2;			        /* Auxiliary data structure. */
} MvarMVStruct;

typedef struct MvarExprTreeStruct {
    MvarExprTreeNodeType NodeType;
    int Dim;
    int PtSize;					    /* vector function size. */
#ifndef SUN4                       /* No support form nameless union/struct. */
    union {
	struct {
#endif /* SUN4 */
	    MvarMVStruct *MV;
	    CagdBType IsRef;       /* TRUE if an MV reference - do not free. */
#ifndef SUN4
	}; /* Leaf node. */
	struct {
#endif /* SUN4 */
	    struct MvarExprTreeStruct *Left, *Right;
#ifndef SUN4
	}; /* Internal node. */
    };
#endif /* SUN4 */
    MvarNormalConeStruct *MVBCone;
    MvarBBoxStruct MVBBox;
    int Val;			             /* Integer value for constants. */
    int IAux, IAux2;		     /* Auxiliary integers for internal use. */
    VoidPtr PAux;		      /* Auxiliary pointer for internal use. */
    char *Info;			   /* Optional info on this expression tree. */
} MvarExprTreeStruct;

typedef struct MvarMVGradientStruct {
    int Dim;
    CagdBType IsRational, HasOrig;
    MvarMVStruct *MV;			       /* The original multivariate. */
    MvarMVStruct *MVGrad;		    /* The gradient if not rational. */
    MvarMVStruct *MVRGrad[MVAR_MAX_PT_COORD + 1];  /* The grad. if rational. */
} MvarMVGradientStruct;

/* Eqns structure - holds a set of expression trees and related info.        */
typedef struct MvarExprTreeEqnsStruct {
    MvarExprTreeStruct **Eqns;		          /* The equations to solve. */
    int NumEqns, NumZeroEqns, NumZeroSubdivEqns;
    MvarExprTreeStruct **CommonExprs;       /* The common expressions found. */
    int NumCommonExprs, MaxNumCommonExprs;
    MvarConstraintType *ConstraintTypes;
} MvarExprTreeEqnsStruct;

/* The general zero finding problem structures. */
typedef struct MvarZeroPrblmStruct {
    struct MvarZeroPrblmStruct *Pnext;
    struct IPAttributeStruct *Attr;
    union {
	MvarMVStruct **MVs;
	MvarExprTreeEqnsStruct *Eqns;
    } U;
    MvarZrSlvrRepresentationType ActiveRepresentation;
    MvarConstraintType *Constraints;
    int NumOfConstraints;
    int NumOfZeroConstraints;
    CagdRType SubdivTol;
    CagdRType NumericTol;
    CagdRType StepTol;
    CagdBType OnlyDtctSol;   /* TRUE to only detect existence of a solution. */
    int *NoSubdivDirs;/* Optional vector of disabled subdivision directions. */
    struct MvarMVZR1DAuxStruct *AS;
    struct MvarZeroPrblmIntrnlStruct *_Internal;
} MvarZeroPrblmStruct;

typedef struct MvarSkel2DPrimPointStruct {
    CagdPType Pt;
} MvarSkel2DPrimPointStruct;

typedef struct MvarSkel2DPrimLineStruct {
    CagdPType Pt1, Pt2;
} MvarSkel2DPrimLineStruct;

typedef struct MvarSkel2DPrimArcStruct {
    CagdPType Center;
    IrtRType StartAngle, EndAngle, Radius;
} MvarSkel2DPrimArcStruct;

typedef struct MvarSkel2DPrimCrvStruct {
    CagdCrvStruct *Crv;
} MvarSkel2DPrimCrvStruct;

typedef struct MvarSkel2DPrimStruct {
    struct MvarSkel2DPrimStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MvarSkel2DPrimType Type;
#ifndef SUN4                       /* No support form nameless union/struct. */
    union {
#endif /* SUN4 */
        MvarSkel2DPrimPointStruct Pt;
        MvarSkel2DPrimLineStruct Ln;
        MvarSkel2DPrimArcStruct Arc;
        MvarSkel2DPrimCrvStruct Crv;
#ifndef SUN4
    };
#endif /* SUN4 */
    int _Index;
    CagdCrvStruct *_CrvRep;
} MvarSkel2DPrimStruct;

typedef struct MvarSkel2DInter3PrimsStruct {
    struct MvarSkel2DInter3PrimsStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdPType PosPrim1, PosPrim2, PosPrim3;
    CagdPType EquadistPoint;
} MvarSkel2DInter3PrimsStruct;

typedef struct MvarHFDistParamStruct {
    int NumOfParams;  /* Number of locations where the Hausdorff dist holds. */
    int ManifoldDim;                        /* 1 for curves, 2 for surfaces. */
#ifndef SUN4                       /* No support form nameless union/struct. */
    union {
#endif /* SUN4 */
	CagdRType T[MVAR_HF_DIST_MAX_PARAM];		/* Params of curves. */
	CagdUVType UV[MVAR_HF_DIST_MAX_PARAM];	      /* Params of surfaces. */
#ifndef SUN4
    };
#endif /* SUN4 */
} MvarHFDistParamStruct;

typedef struct MvarHFDistPairParamStruct {
    struct MvarHFDistPairParamStruct *Pnext;
    MvarHFDistParamStruct Param1, Param2;	 /* Param. info of the pair. */
    CagdRType Dist;		     /* Euclidean distance at this location. */
} MvarHFDistPairParamStruct;

/* Matlab interface for equation solving. */
typedef struct MvarMatlabEqStruct {
    int NumOfMonoms;		     /* Number of monomials in the equation. */
    CagdRType *CoeffArr;		     /* The coeffs of each monomial. */
    int *PowersMat;    /* The power of variable j (col) in monomial i (row). */
    int *MaxPowers;	  /* Maximal power of each variable in the equation. */
} MvarMatlabEqStruct;

/* Data structures for Srf-Srf intersection cache. */
typedef struct MvarSrfSrfInterCacheDataStruct {
    struct MvarSrfSrfInterCacheDataStruct *Pnext;
    int Id1;
    int Id2;
    CagdRType KnotSpan1[4];
    CagdRType KnotSpan2[4];
    MvarPolylineStruct *InterRes;
} MvarSrfSrfInterCacheDataStruct;

typedef struct MvarSrfSrfInterCacheStruct {
    MvarSrfSrfInterCacheDataStruct *CacheData;
    IPAttrIDType AttribID;
    int NextId;
} MvarSrfSrfInterCacheStruct;

typedef void (*MvarExprTreePrintFuncType)(const char *s);
typedef void (*MvarSetErrorFuncType)(MvarFatalErrorType ErrorFunc);
typedef int (*MvarMVsZerosMVsCBFuncType)(MvarMVStruct **MVs,
					 int n,
					 void *AuxData);
typedef int (*MvarMVsZerosSubdivCBFuncType)(MvarZeroPrblmStruct *p,
					    int i,
					    void *AuxData,
					    int *IsTopoGuarantee);
typedef int (*MvarMVsZerosVerifyOneSolPtCBFuncType)(MvarPtStruct *Pt,
						    void *AuxData);
typedef int (*MvarMVsZerosVerifyAllSolsCBFuncType)(MvarPolylineStruct **MVPls,
						   void *AuxData);
typedef CagdRType *(*MvarMapPrm2EucCBFuncType)(
					    CagdRType *R,
					    int n,
					    CagdRType OutParam[3],
					    void *AuxData);

/* Possible external call backs exposed to end users in the zeros solver. */
typedef struct MvarZeroPrblmExternalCBStruct {
    /* All possible call back functions/data. */
    MvarSetErrorFuncType SetErrorFunc;           /* Not used at this time. */
    void *SetErrorData1;
    MvarExprTreePrintFuncType ExprTreePrintFunc; /* Not used at this time. */
    void *ZerosSubdivCBData;
    /* Alternative termination function for the subdivision process. */
    MvarMVsZerosSubdivCBFuncType MVsZerosSubdivFunc;
    void *MVsZerosSubdivData;
    /* Optional verification/filtering of solution points. */
    MvarMVsZerosVerifyOneSolPtCBFuncType MVsZerosVerifyOneSolPtFunc;
    void *MVsZerosVerifyOneSolPtData;
    /* Optional verification/filtering of general solutions. */
    MvarMVsZerosVerifyAllSolsCBFuncType MVsZerosVerifyAllSolsFunc;
    void *MVsZerosVerifyAllSolsData;
    /* Allows in-place mapping of solutions from MV solver parametric space */
    /* to actual solution space, typically Euclidean.			    */
    MvarMapPrm2EucCBFuncType MapPtParamSpace2EuclidSpace;
    void *MapPtParamSpace2EuclidData;
    /* Allows in-place mapping of solution normals from MV solver           */
    /* parametric space to actual solution space, typically Euclidean.      */
    MvarMapPrm2EucCBFuncType MapPtParamSpace2EuclidNormal;
    void *MapPtParamSpace2EuclidNormalData;
    MvarMVsZerosMVsCBFuncType CnvrtETs2MVsFunc;   /* Not used at this time. */
    void *CnvrtETs2MVsData;
} MvarZeroPrblmExternalCBStruct;

/* The general zero finding problem specification and solution structures. */
typedef struct MvarZeroPrblmSpecStruct {
    union {
	 MvarMVStruct * const *MVs;
	 MvarExprTreeStruct * const *MVETs;
    } U;
    int NumOfMVs;
    MvarConstraintType *Constraints;
    CagdRType SubdivTol;
    CagdRType NumericTol;
    CagdRType StepTol;

    /* All possible call back functions/data. */
    MvarZeroPrblmExternalCBStruct ECB;

    int _HighDimBndry;				        /* Used internally. */
} MvarZeroPrblmSpecStruct;

typedef struct MvarZeroSolutionStruct {
    struct MvarZeroSolutionStruct *Pnext;
    struct IPAttributeStruct *Attr;
    union {
	MvarPtStruct *Pt;
	MvarPolylineStruct *Pl;
	MvarTriangleStruct *Tr;
	CagdBType DtctSol;
    } U;
    MvarZrSlvrRepresentationType ActiveRepresentation;
    struct MvarZeroTJunctionStruct *TJList;/*of current problem, not handled.*/
} MvarZeroSolutionStruct;

#define MVAR_MALLOC_STRUCT_ONCE     /* Faster allocation of MVAR structures. */

#define MVAR_IS_BEZIER_MV(MV)		((MV) -> GType == MVAR_BEZIER_TYPE)
#define MVAR_IS_POWER_MV(MV)		((MV) -> GType == MVAR_POWER_TYPE)
#define MVAR_IS_BSPLINE_MV(MV)		((MV) -> GType == MVAR_BSPLINE_TYPE)

#define MVAR_CTL_MESH_LENGTH(MV)	((MV) -> SubSpaces[(MV) -> Dim])

/******************************************************************************
*  Provides easy access to multivariates up to dimension six.		      *
******************************************************************************/
#define MVAR_NEXT_U(MV)			(1)         /* == MV -> SubSpaces[0] */
#define MVAR_NEXT_V(MV)			((MV) -> SubSpaces[1])
#define MVAR_NEXT_W(MV)			((MV) -> SubSpaces[2])
#define MVAR_NEXT_FOURTH(MV)		((MV) -> SubSpaces[3])
#define MVAR_NEXT_FIFTH(MV)		((MV) -> SubSpaces[4])
#define MVAR_NEXT_SIXTH(MV)		((MV) -> SubSpaces[5])
#define MVAR_NEXT_DIM(MV, Dim)		((MV) -> SubSpaces[(Dim)])

#define MVAR_MESH_UV(MV, i, j)		((i) + \
					 ((MV) -> SubSpaces[1]) * (j))
#define MVAR_MESH_UVW(MV, i, j, k)	((i) + \
					 ((MV) -> SubSpaces[1]) * (j) + \
					 ((MV) -> SubSpaces[2]) * (k))
#define MVAR_MESH_UVW4(MV, i, j, k, l)  ((i) + \
					 ((MV) -> SubSpaces[1]) * (j) + \
					 ((MV) -> SubSpaces[2]) * (k) + \
					 ((MV) -> SubSpaces[3]) * (l))
#define MVAR_MESH_UVW45(MV, i, j, k, l, m) \
					((i) + \
					 ((MV) -> SubSpaces[1]) * (j) + \
					 ((MV) -> SubSpaces[2]) * (k) + \
					 ((MV) -> SubSpaces[3]) * (l) + \
					 ((MV) -> SubSpaces[4]) * (m))
#define MVAR_MESH_UVW456(MV, i, j, k, l, m, n) \
					((i) + \
					 ((MV) -> SubSpaces[1]) * (j) + \
					 ((MV) -> SubSpaces[2]) * (k) + \
					 ((MV) -> SubSpaces[3]) * (l) + \
					 ((MV) -> SubSpaces[4]) * (m) + \
					 ((MV) -> SubSpaces[5]) * (n))

/* If a mvarariate is periodic, the control polygon/mesh should warp up.     */
/* Length does hold the real allocated length but the virtual periodic       */
/* length is a little larger. Note allocated KV's are larger.                */
#define MVAR_MVAR_UPT_LST_LEN(MV)	((MV) -> Lengths[0] + \
			 ((MV) -> Periodic[0] ? (MV) -> Orders[0] - 1 : 0))
#define MVAR_MVAR_VPT_LST_LEN(MV)	((MV) -> Lengths[1] + \
			 ((MV) -> Periodic[1] ? (MV) -> Orders[1] - 1 : 0))
#define MVAR_MVAR_WPT_LST_LEN(MV)	((MV) -> Lengths[2] + \
			 ((MV) -> Periodic[2] ? (MV) -> Orders[2] - 1 : 0))
#define MVAR_MVAR_FOURTH_PT_LST_LEN(MV)	((MV) -> Lengths[3] + \
			 ((MV) -> Periodic[3] ? (MV) -> Orders[3] - 1 : 0))
#define MVAR_MVAR_FIFTH_PT_LST_LEN(MV)	((MV) -> Lengths[4] + \
			 ((MV) -> Periodic[4] ? (MV) -> Orders[4] - 1 : 0))
#define MVAR_MVAR_SIXTH_PT_LST_LEN(MV)	((MV) -> Lengths[5] + \
			 ((MV) -> Periodic[5] ? (MV) -> Orders[5] - 1 : 0))
#define MVAR_MVAR_ITH_PT_LST_LEN(MV, i)	((MV) -> Lengths[i] + \
			 ((MV) -> Periodic[i] ? (MV) -> Orders[i] - 1 : 0))

#define MVAR_IS_UPERIODIC_MVAR(MV)	((MV) -> Periodic[0])
#define MVAR_IS_VPERIODIC_MVAR(MV)	((MV) -> Periodic[1])
#define MVAR_IS_WPERIODIC_MVAR(MV)	((MV) -> Periodic[2])
#define MVAR_IS_FOURTH_PERIODIC_MVAR(MV) ((MV) -> Periodic[3])
#define MVAR_IS_FIFTH_PERIODIC_MVAR(MV) ((MV) -> Periodic[4])
#define MVAR_IS_SIXTH_PERIODIC_MVAR(MV) ((MV) -> Periodic[5])
#define MVAR_IS_ITH_PERIODIC_MVAR(MV, i) ((MV) -> Periodic[i])

/* Ease the handling of the splitting of a multivariate to scalar fields. */
#define MVAR_CLEAR_SCALARS(MV) { \
	int ii; \
	for (ii = 0; ii < MVAR_MAX_PT_SIZE; ii++) \
	    (MV)[ii] = NULL; \
    } 
#define MVAR_FREE_SCALARS(MV) { \
        int ii; \
        if ((MV)[0] != NULL) \
	    MvarMVFree((MV)[0]); \
	for (ii = 1; ii <= MVAR_MAX_PT_COORD; ii++) { \
	    if ((MV)[ii] == NULL) \
	        break; \
	    MvarMVFree((MV)[ii]); \
	} \
    }

#define MVAR_INCREMENT_MESH_INDICES(MV, Indices, Index) \
    (++(*Indices) >= MV -> Lengths[0] ? \
	_MvarIncrementMeshIndices(MV, Indices, &Index) : ++Index)

#define MVAR_INCREMENT_MESH_ORDER_INDICES(MV, Indices, Index) \
    (++(*Indices) >= MV -> Orders[0] ? \
	_MvarIncrementMeshOrderIndices(MV, Indices, &Index) : ++Index)

#define MVAR_INC_SKIP_MESH_INDICES_1ST(MV, Indices) \
    _MvarIncSkipMeshIndices1st(MV, Indices)

#define MVAR_INC_SKIP_MESH_INDICES(MV, Indices, SkipDir, Index) \
    (MV -> Dim <= 1 ? (Index = 0) : \
     (++(Indices[SkipDir == 0]) >= MV -> Lengths[SkipDir == 0] ? \
	 _MvarIncSkipMeshIndices(MV, Indices, SkipDir, &Index) : \
	 (Index += MVAR_NEXT_DIM(MV, SkipDir == 0))))

#define MVAR_INC_BOUND_MESH_INDICES(MV, Indices, LowBound, UpBound, Index) \
    (++(*Indices) >= UpBound[0] ? \
	_MvarIncBoundMeshIndices(MV, Indices, LowBound, UpBound, \
				 &Index) : ++Index)

#define MVAR_BBOX_RESET(BBox)    IRIT_ZAP_MEM(&BBox, sizeof(MvarBBoxStruct));

#define MVAR_BBOX_INIT(BBox, Dimen) { \
    int _m; \
    MVAR_BBOX_RESET(BBox); \
    (BBox).Dim = (Dimen); \
    for (_m = 0; _m < (Dimen); _m++) { \
	(BBox).Min[_m] = IRIT_INFNTY; \
	(BBox).Max[_m] = -IRIT_INFNTY; \
    } \
}
#define MVAR_IS_BBOX_RESET(BBox)  ((BBox).Dim == 0)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/******************************************************************************
* General routines of the Mvar library:					      *
******************************************************************************/
MvarMVStruct *MvarMVNew(int Dim,
			MvarGeomType GType,
			MvarPointType PType,
			const int *Lengths);
MvarMVStruct *MvarBspMVNew(int Dim,
			   const int *Lengths,
			   const int *Orders,
			   MvarPointType PType);
MvarMVStruct *MvarBzrMVNew(int Dim, const int *Lengths, MvarPointType PType);
MvarMVStruct *MvarPwrMVNew(int Dim, const int *Lengths, MvarPointType PType);
MvarMVStruct *MvarBuildParamMV(int Dim, int Dir, CagdRType Min, CagdRType Max);
MvarMVStruct *MvarMVCopy(const MvarMVStruct *MV);
MvarMVStruct *MvarMVCopyList(const MvarMVStruct *MVList);
void MvarMVFree(MvarMVStruct *MV);
void MvarMVFreeList(MvarMVStruct *MVList);

#ifdef DEBUG
#define MvarMVFree(MV)         { MvarMVFree(MV); MV = NULL; }
#define MvarMVFreeList(MVList) { MvarMVFreeList(MVList); MVList = NULL; }
#endif /* DEBUG */

MvarPtStruct *MvarPtNew(int Dim);
MvarPtStruct *MvarPtRealloc(MvarPtStruct *Pt, int NewDim);
MvarPtStruct *MvarPtCopy(const MvarPtStruct *Pt);
MvarPtStruct *MvarPtCopyList(const MvarPtStruct *PtList);
MvarPtStruct *MvarPtSortListAxis(MvarPtStruct *PtList, int Axis);
void MvarPtFree(MvarPtStruct *Pt);
void MvarPtFreeList(MvarPtStruct *PtList);

#ifdef DEBUG
#define MvarPtFree(Pt)         { MvarPtFree(Pt); Pt = NULL; }
#define MvarPtFreeList(PtList) { MvarPtFreeList(PtList); PtList = NULL; }
#endif /* DEBUG */

MvarPtStruct *MvarPolyReverseList(MvarPtStruct *Pts);

MvarPolylineStruct *MvarPolylineNew(MvarPtStruct *Pl);
MvarPolylineStruct *MvarPolylineCopy(const MvarPolylineStruct *Poly);
MvarPolylineStruct *MvarPolylineCopyList(MvarPolylineStruct *PolyList);
void MvarPolylineFree(MvarPolylineStruct *Poly);
void MvarPolylineFreeList(MvarPolylineStruct *PolyList);

#ifdef DEBUG
#define MvarPolylineFree(Poly)         { MvarPolylineFree(Poly); Poly = NULL; }
#define MvarPolylineFreeList(PolyList) { MvarPolylineFreeList(PolyList); \
				         PolyList = NULL; }
#endif /* DEBUG */

MvarVecStruct *MvarVecNew(int Dim);
MvarVecStruct *MvarVecArrayNew(int Size, int Dim);
MvarVecStruct *MvarVecRealloc(MvarVecStruct *Vec, int NewDim);
MvarVecStruct *MvarVecCopy(const MvarVecStruct *Vec);
MvarVecStruct *MvarVecCopyList(const MvarVecStruct *VecList);
void MvarVecFree(MvarVecStruct *Vec);
void MvarVecFreeList(MvarVecStruct *VecList);
void MvarVecArrayFree(MvarVecStruct *MVVecArray, int Size);
void MvarVecAdd(MvarVecStruct *VRes,
		const MvarVecStruct *V1,
		const MvarVecStruct *V2);
void MvarVecAddScale(MvarVecStruct *VRes,
		     const MvarVecStruct *V1,
		     const MvarVecStruct *V2,
		     CagdRType Scale2);
void MvarVecSub(MvarVecStruct *VRes,
		const MvarVecStruct *V1,
		const MvarVecStruct *V2);
CagdRType MvarVecDotProd(const MvarVecStruct *V1, const MvarVecStruct *V2);
CagdRType MvarVecSqrLength(const MvarVecStruct *V);
CagdRType MvarVecSqrLength2(const CagdRType *v, int Dim);
CagdRType MvarVecLength(const MvarVecStruct *V);
MvarVecStruct *MvarVecScale(MvarVecStruct *V, CagdRType ScaleFactor);
void MvarVecBlend(MvarVecStruct *VRes,
		  const MvarVecStruct *V1,
		  const MvarVecStruct *V2,
		  CagdRType t);
int MvarVecNormalize(MvarVecStruct *V);
int MvarVecOrthogonalize(MvarVecStruct *Dir, const MvarVecStruct *Vec);
int MvarVecOrthogonal2(MvarVecStruct *Dir,
		       const MvarVecStruct *Vec1,
		       const MvarVecStruct *Vec2);
int MvarVecSetOrthogonalize(const MvarVecStruct **Vecs,
			    MvarVecStruct **OrthoVecs,
			    int Size);
CagdBType MvarVecWedgeProd(MvarVecStruct **Vectors,
			   int Size, 
			   MvarVecStruct **NewVecs,
			   int NewSize, 
			   CagdBType CheckDet,
			   CagdRType *DetVal);
int MvarPlaneNormalize(MvarPlaneStruct *Pln);
MvarVecStruct *MvarLinePlaneInter(const MvarVecStruct *P,
				  const MvarVecStruct *V,
				  const MvarPlaneStruct *Pln,
				  CagdRType *Param);

#ifdef DEBUG
#define MvarVecFree(Vec)         { MvarVecFree(Vec); Vec = NULL; }
#define MvarVecFreeList(VecList) { MvarVecFreeList(VecList); VecList = NULL; }
#define MvarVecArrayFree(VecArray, Size) { MvarVecArrayFree(VecArray, Size); \
					   VecArray = NULL; }
#endif /* DEBUG */

MvarPlaneStruct *MvarPlaneNew(int Dim);
MvarPlaneStruct *MvarPlaneCopy(const MvarPlaneStruct *Plane);
MvarPlaneStruct *MvarPlaneCopyList(const MvarPlaneStruct *PlaneList);
void MvarPlaneFree(MvarPlaneStruct *Plane);
void MvarPlaneFreeList(MvarPlaneStruct *PlaneList);

#ifdef DEBUG
#define MvarPlaneFree(Plane)         { MvarPlaneFree(Plane); Plane = NULL; }
#define MvarPlaneFreeList(PlaneList) { MvarPlaneFreeList(PlaneList); \
				       PlaneList = NULL; }
#endif /* DEBUG */

MvarNormalConeStruct *MvarNormalConeNew(int Dim);
MvarNormalConeStruct *MvarNormalConeCopy(const MvarNormalConeStruct
					                         *NormalCone);
MvarNormalConeStruct *MvarNormalConeCopyList(const MvarNormalConeStruct
					                        *NormalCones);

void MvarNormalConeFree(MvarNormalConeStruct *NormalCone);
void MvarNormalConeFreeList(MvarNormalConeStruct *NormalConeList);

#ifdef DEBUG
#define MvarConeFree(Cone)         { MvarConeFree(Cone); PCone = NULL; }
#endif /* DEBUG */

MvarPtStruct *MvarGetLastPt(MvarPtStruct *Pts);
int MvarPtCmpTwoPoints(const MvarPtStruct *P1,
		       const MvarPtStruct *P2,
		       CagdRType Eps);
int MvarVecCmpTwoVectors(const CagdRType *P1,
		         const CagdRType *P2,
		         int Length,
			 CagdRType Eps);
CagdRType MvarPtDistTwoPoints(const MvarPtStruct *P1, const MvarPtStruct *P2);
CagdRType MvarPtDistSqrTwoPoints(const MvarPtStruct *P1,
				 const MvarPtStruct *P2);
MvarPtStruct *MvarPtInBetweenPoint(const MvarPtStruct *Pt1,
				   const MvarPtStruct *Pt2,
				   CagdRType t);
MvarPolylineStruct *MvarPolyMergePolylines(MvarPolylineStruct *Polys,
					   IrtRType Eps);
MvarPolylineStruct *MvarMatchPointListIntoPolylines(const MvarPtStruct
						                     *PtsList,
						    IrtRType MaxTol);
MvarPtStruct *MvarCnvrtCagdPtsToMVPts(const CagdPtStruct *Pts);
MvarPtStruct *MvarCnvrtMVPolysToMVPts(const MvarPolylineStruct *MVPlls);
struct IPObjectStruct *MvarCnvrtMVPolysToCtlPts(const MvarPolylineStruct
						                     *MVPlls);
CagdPtStruct *MvarCnvrtMVPtsToCagdPts(const MvarPtStruct *MVPts);
struct IPObjectStruct *MvarCnvrtMVPtsToCtlPts(const MvarPtStruct *MVPts,
					      IrtRType MergeTol);
struct IPObjectStruct *MvarCnvrtMVPtsToPolys(const MvarPtStruct *MVPts,
					     const MvarMVStruct *MV,
					     IrtRType MergeTol);
struct IPPolygonStruct *MvarCnvrtMVPtsToPolys2(const MvarPtStruct *InPts,
					       CagdRType FineNess,
					       int Dim,
					       IrtRType *ParamDomain);
struct IPObjectStruct *MvarCnvrtMVPolysToIritPolys(const MvarPolylineStruct
						                      *MVPlls);
struct IPObjectStruct *MvarCnvrtMVPolysToIritPolys2(const MvarPolylineStruct
						                       *MVPlls,
						    int IgnoreIndividualPts);
CagdCrvStruct *MvarCnvrtMVPolysToIritCrvs(const MvarPolylineStruct *MVPlls,
					  int Order);
MvarPolylineStruct *MvarCnvrtIritLinCrvsToMVPolys(const CagdCrvStruct *Crvs);
struct IPObjectStruct *MvarCnvrtMVTrsToIritPolygons(const MvarTriangleStruct
						                        *MVTrs,
						    int *Coords);
MvarTriangleStruct *MvarIrit2DTrTo2DMVTrs(struct IPObjectStruct *ObjTrs);

MvarMVStruct *MvarCnvrtBzr2BspMV(const MvarMVStruct *MV);
MvarMVStruct *MvarCnvrtBsp2BzrMV(const MvarMVStruct *MV);
MvarMVStruct *MvarCnvrtPwr2BzrMV(const MvarMVStruct *MV);
MvarMVStruct *MvarCnvrtBzr2PwrMV(const MvarMVStruct *MV);

MvarMVStruct *MvarBzrLinearInOneDir(int Dim, int Dir, MvarPointType PType);

MvarMVStruct *MvarMVUnitMaxCoef(MvarMVStruct *MV);

void MvarMVTransform(MvarMVStruct *MV, CagdRType *Translate, CagdRType Scale);
MvarMVStruct *MvarMVMatTransform(const MvarMVStruct *MV, CagdMType Mat);
MvarMVStruct *MvarMVListMatTransform(const MvarMVStruct *MVs, CagdMType Mat);
void MvarMVMatTransform2(MvarMVStruct *MV, CagdMType Mat);

MvarMVStruct *MvarCoerceMVsTo(const MvarMVStruct *MV, MvarPointType PType);
MvarMVStruct *MvarCoerceMVTo(const MvarMVStruct *MV, MvarPointType PType);
MvarPointType MvarMergeTwoPointTypes(MvarPointType PType1, MvarPointType PType2);

void MvarMVDomain(const MvarMVStruct *MV,
		  CagdRType *Min,
		  CagdRType *Max, 
		  int Axis);
void MvarMVDomainAlloc(const MvarMVStruct *MV,
		       CagdRType **MinDmn, 
		       CagdRType **MaxDmn);
void MvarMVDomainFree(CagdRType *MinDmn, CagdRType *MaxDmn);
MvarMVStruct *MvarMVSetDomain(MvarMVStruct *MV,
			      CagdRType Min,
			      CagdRType Max,
			      int Axis,
			      int InPlace);
IrtRType MvarMVVolumeOfDomain(MvarMVStruct * const MVs, int Dim);

void MvarMVAuxDomainSlotReset(MvarMVStruct *MV);
int MvarMVAuxDomainSlotCopy(MvarMVStruct *MVDst, const MvarMVStruct *MVSrc);
void MvarMVAuxDomainSlotSet(MvarMVStruct *MV,
			    CagdRType Min,
			    CagdRType Max,
			    int Dir);
void MvarMVAuxDomainSlotSetRel(MvarMVStruct *MV,
			       CagdRType Min,
			       CagdRType Max,
			       int Dir);
int MvarMVAuxDomainSlotGet(const MvarMVStruct *MV,
			   CagdRType *Min,
			   CagdRType *Max,
			   int Dir);

MvarMVStruct *MvarMVSetAllDomains(MvarMVStruct *MV,
				  CagdRType *Min,
				  CagdRType *Max,
				  int InPlace);
CagdBType MvarParamInDomain(const MvarMVStruct *MV,
			    CagdRType t,
			    MvarMVDirType Dir);
CagdBType MvarParamsInDomain(const MvarMVStruct *MV, const CagdRType *Params);
void MvarMVUpdateConstDegDomains(MvarMVStruct **MVs, int NumOfMVs);
MvarPtStruct *MvarMVIntersPtOnBndry(MvarMVStruct *MV, 
				    MvarPtStruct *PointIns, 
				    MvarPtStruct *PointOuts);

CagdRType *MvarMVEvalMalloc(const MvarMVStruct *MV, const CagdRType *Params);
void MvarMVEvalToData(const MvarMVStruct *MV,
		      const CagdRType *Params,
		      CagdRType *Pt);

CagdRType *MvarMVEvalGradient2(const MvarMVStruct *MV,
			       const CagdRType *Params,
			       int *HasOrig,
			       CagdRType *Grad);
MvarPlaneStruct *MvarMVEvalTanPlane(const MvarMVStruct *MV,
				    const CagdRType *Params);

MvarMVStruct *MvarMVFromMV(const MvarMVStruct *MV,
			   CagdRType t,
			   MvarMVDirType Dir);
MvarMVStruct *MvarMVFromMesh(const MvarMVStruct *MV,
			     int Index,
			     MvarMVDirType Dir);
MvarMVStruct *MvarCnvrtCrvToMV(const CagdCrvStruct *Crv);
CagdCrvStruct *MvarCnvrtMVToCrv(const MvarMVStruct *MV);
MvarMVStruct *MvarCnvrtSrfToMV(const CagdSrfStruct *Srf);
CagdSrfStruct *MvarCnvrtMVToSrf(const MvarMVStruct *MV);
MvarMVStruct *MvarCnvrtTVToMV(const TrivTVStruct *TV);
TrivTVStruct *MvarCnvrtMVToTV(const MvarMVStruct *MV);

MvarMVStruct *MvarMVRegionFromMV(const MvarMVStruct *MV,
				 CagdRType t1,
				 CagdRType t2,
				 MvarMVDirType Dir);
MvarMVStruct *MvarBzrMVRegionFromMV(const MvarMVStruct *MV,
				    CagdRType t1,
				    CagdRType t2,
				    MvarMVDirType Dir);
MvarMVStruct *MvarMVOpenEnd(const MvarMVStruct *MV);
MvarMVStruct *MvarMVRefineAtParams(const MvarMVStruct *MV,
				   MvarMVDirType Dir,
				   CagdBType Replace,
				   CagdRType *t,
				   int n);
MvarMVStruct *MvarBspMVKnotInsertNDiff(const MvarMVStruct *MV,
				       MvarMVDirType Dir,
				       int Replace,
				       CagdRType *t,
				       int n);
MvarMVStruct *MvarMVDerive(const MvarMVStruct *MV, MvarMVDirType Dir);
MvarMVStruct *MvarBzrMVDerive(const MvarMVStruct *MV, MvarMVDirType Dir,
			      CagdBType DeriveScalar);
MvarMVStruct *MvarBzrMVDeriveScalar(const MvarMVStruct *MV, MvarMVDirType Dir);
MvarMVStruct *MvarBspMVDerive(const MvarMVStruct *MV,
			      MvarMVDirType Dir,
			      CagdBType DeriveScalar);
MvarMVStruct *MvarBspMVDeriveScalar(const MvarMVStruct *MV, MvarMVDirType Dir);
void MvarMVDeriveBound(const MvarMVStruct *MV,
		       MvarMVDirType Dir,
		       CagdRType MinMax[2]);
void MvarBzrMVDeriveBound(const MvarMVStruct *MV,
			  MvarMVDirType Dir,
			  CagdRType MinMax[2]);
void MvarBspMVDeriveBound(const MvarMVStruct *MV,
			  MvarMVDirType Dir,
			  CagdRType MinMax[2]);
void MvarMVDeriveAllBounds(const MvarMVStruct *MV, CagdMinMaxType *MinMax);
void MvarBzrMVDeriveAllBounds(const MvarMVStruct *MV, CagdMinMaxType *MinMax);
void MvarBspMVDeriveAllBounds(const MvarMVStruct *MV, CagdMinMaxType *MinMax);
MvarMVGradientStruct *MvarMVPrepGradient(const MvarMVStruct *MV,
					 CagdBType Orig);
void MvarMVFreeGradient(MvarMVGradientStruct *MV);
CagdRType *MvarMVEvalGradient(const MvarMVGradientStruct *MV,
			      const CagdRType *Params,
			      int Axis,
			      CagdRType *Grad);
MvarMVGradientStruct *MvarMVBoundGradient(const MvarMVStruct *MV);
MvarMVStruct *MvarMVSubdivAtParam(const MvarMVStruct *MV,
				  CagdRType t,
				  MvarMVDirType Dir);
MvarMVStruct *MvarBspMVSubdivAtParam(const MvarMVStruct *MV,
				     CagdRType t,
				     MvarMVDirType Dir);
MvarMVStruct *MvarBzrMVSubdivAtParam(const MvarMVStruct *MV,
				     CagdRType t,
				     MvarMVDirType Dir);
MvarMVStruct *MvarMVSubdivAtParamOneSide(const MvarMVStruct *MV,
					 CagdRType t,
					 MvarMVDirType Dir,
					 IrtBType LeftSide);
MvarMVStruct *MvarBzrMVSubdivAtParamOneSide(const MvarMVStruct *MV,
					    CagdRType t,
					    MvarMVDirType Dir,
					    IrtBType LeftSide);
MvarMVStruct *MvarBspMVSubdivAtParamOneSide(const MvarMVStruct *MV,
					    CagdRType t,
					    MvarMVDirType Dir,
					    IrtBType LeftSide);
MvarMVStruct *MvarMVDegreeRaise(const MvarMVStruct *MV, MvarMVDirType Dir);
MvarMVStruct *MvarMVDegreeRaiseN(const MvarMVStruct *MV, int *NewOrders);
MvarMVStruct *MvarMVDegreeRaiseN2(const MvarMVStruct *MV, int *NewOrders);
MvarMVStruct *MvarMVPwrDegreeRaise(const MvarMVStruct *MV,
				   int Dir,
				   int IncOrder);
CagdBType MvarMakeMVsCompatible(MvarMVStruct **MV1,
				MvarMVStruct **MV2,
				CagdBType SameOrders,
				CagdBType SameKVs);
CagdBType MvarMakeMVsCompatible2(MvarMVStruct **MV1,
				 MvarMVStruct **MV2,
				 CagdBType SameOrders,
				 CagdBType SameKVs);
CagdBType MvarMakeMVsOneDimCompatible(MvarMVStruct **MV1,
				      MvarMVStruct **MV2,
				      int Dim,
				      CagdBType SameOrders,
				      CagdBType SameKVs);
void MvarMVMinMax(const MvarMVStruct *MV,
		  int Axis,
		  CagdRType *Min,
		  CagdRType *Max);
void MvarMVCornersMinMax(const MvarMVStruct *MV,
			 int Coord,
			 CagdRType *Min,
			 CagdRType *Max);
MvarBBoxStruct *MvarMVBBox(const MvarMVStruct *MV, MvarBBoxStruct *BBox);
void MvarMVListBBox(const MvarMVStruct *MVs, MvarBBoxStruct *BBox);
void MvarMergeBBox(MvarBBoxStruct *DestBBox, const MvarBBoxStruct *SrcBBox);
int MvarMVIsConstant(const MvarMVStruct *MV, IrtRType Eps);
void MvarBBoxOfDotProd(const MvarBBoxStruct *BBox1,
		       const MvarBBoxStruct *BBox2,
		       MvarBBoxStruct *DProdBBox);
void MvarBBoxOfDotProd2(const MvarBBoxStruct *BBox1,
			const MvarBBoxStruct *BBox2,
			MvarBBoxStruct *DProdBBox);
void MvarBBoxOfCrossProd(const MvarBBoxStruct *BBox1,
			 const MvarBBoxStruct *BBox2,
			 MvarBBoxStruct *DCrossBBox);

MvarMVStruct **MvarBndryMVsFromMV(const MvarMVStruct *MV);
MvarPtStruct *MvarMVPreciseBBox(const MvarMVStruct *MV,
			        MvarBBoxStruct *BBox,
				CagdRType Tol);
void MvarMVListPreciseBBox(const MvarMVStruct *MVs,
			   MvarBBoxStruct *BBox,
			   CagdRType Tol);
void MvarTrivarPreciseBBox(const TrivTVStruct *TV,
			   MvarBBoxStruct *BBox,
			   CagdRType Tol);
void MvarTrivarListPreciseBBox(const TrivTVStruct *Trivars,
			       MvarBBoxStruct *BBox,
			       CagdRType Tol);
void MvarSrfPreciseBBox(const CagdSrfStruct *Srf,
			MvarBBoxStruct *BBox,
			CagdRType Tol);
void MvarSrfListPreciseBBox(const CagdSrfStruct *Srfs,
			    MvarBBoxStruct *BBox,
			    CagdRType Tol);
void MvarMdlTrimSrfPreciseBBox(const MdlTrimSrfStruct *TSrf,
			       MvarBBoxStruct *BBox,
			       CagdRType Tol);
void MvarMdlTrimSrfListPreciseBBox(const MdlTrimSrfStruct *TSrfs,
				   MvarBBoxStruct *BBox,
				   CagdRType Tol);
void MvarTrimSrfPreciseBBox(const TrimSrfStruct *TSrf,
			    MvarBBoxStruct *BBox,
			    CagdRType Tol);
void MvarTrimSrfListPreciseBBox(const TrimSrfStruct *TSrfs,
				MvarBBoxStruct *BBox,
				CagdRType Tol);
void MvarCrvPreciseBBox(const CagdCrvStruct *Crv,
			MvarBBoxStruct *BBox,
			CagdRType Tol);
void MvarCrvListPreciseBBox(const CagdCrvStruct *Crvs,
			    MvarBBoxStruct *BBox,
			    CagdRType Tol);

int _MvarIncrementMeshIndices(const MvarMVStruct *MV, int *Indices, int *Index);
int _MvarIncrementMeshOrderIndices(const MvarMVStruct *MV,
				   int *Indices,
				   int *Index);
int _MvarIncSkipMeshIndices1st(const MvarMVStruct *MV, int *Indices);
int _MvarIncSkipMeshIndices(const MvarMVStruct *MV,
			    int *Indices,
			    int Dir,
			    int *Index);
int _MvarIncBoundMeshIndices(const MvarMVStruct *MV,
			     int *Indices,
			     int *LowerBound,
			     int *UpperBound,
			     int *Index);
int MvarGetPointsMeshIndices(const MvarMVStruct *MV, int *Indices);
int MvarGetPointsPeriodicMeshIndices(const MvarMVStruct *MV, int *Indices);
int MvarMeshIndicesFromIndex(int Index, const MvarMVStruct *MV, int *Indices);

MvarMVStruct *MvarEditSingleMVPt(MvarMVStruct *MV,
				 CagdCtlPtStruct *CtlPt,
				 int *Indices,
				 CagdBType Write);
CagdBType MvarMVsSameSpace(const MvarMVStruct *MV1,
			   const MvarMVStruct *MV2,
			   CagdRType Eps);
CagdBType MvarMVsSame(const MvarMVStruct *MV1,
		      const MvarMVStruct *MV2,
		      CagdRType Eps);
CagdBType MvarMVsSame3(const MvarMVStruct *MV1,
		       const MvarMVStruct *MV2,
		       CagdRType Eps,
		       int *Modified);
MvarMVStruct *MvarPromoteMVToMV(const MvarMVStruct *MV, int Axis);
MvarMVStruct *MvarPromoteMVToMV2(const MvarMVStruct *MV,
				 int NewDim, 
				 int StartAxis);
MvarMVStruct *MvarCrvMakeCtlPtParam(const CagdCrvStruct *Crv,
				    int CtlPtIdx,
				    CagdRType Min,
				    CagdRType Max);
MvarMVStruct *MvarMVShiftAxes(const MvarMVStruct *MV, int Axis);
MvarMVStruct *MvarMVParamShift(const MvarMVStruct *MV, int AxisSrc, int AxisTar);
MvarMVStruct *MvarMVReverse(const MvarMVStruct *MV, int Axis1, int Axis2);
MvarMVStruct *MvarMVReverseDir(const MvarMVStruct *MV, int Axis);
CagdBType MvarAre2MVsPossiblySharingBndry(const MvarMVStruct *MV1,
					  const MvarMVStruct *MV2,
					  int Dir,
					  CagdBType *MV1Rev,
					  CagdBType *MV2Rev,
					  CagdRType Eps);
int MvarAre2MVsSharingBndry(const MvarMVStruct *MV1,
			    int Dir1,
			    CagdBType MaxBndry1,
			    const MvarMVStruct *MV2,
			    int Dir2,
			    CagdBType MaxBndry2,
			    IrtRType Tolerance,
			    int *Modified);
MvarMVStruct *MvarMergeMVList(MvarMVStruct *MVList,
			      int Dir,
			      IrtRType Tolerance,
			      int InterpDiscont);
MvarMVStruct *MvarMergeMVMV(const MvarMVStruct *MV1,
			    const MvarMVStruct *MV2,
			    MvarMVDirType Dir,
			    CagdBType Discont);
MvarMVStruct *MvarMergeMVMV2(const MvarMVStruct *MV1,
			     const MvarMVStruct *MV2,
			     MvarMVDirType Dir,
			     CagdBType Discont);
CagdRType MvarMVAvgArcLenMesh(const MvarMVStruct *MV, MvarMVDirType Dir);

CagdBType MvarBspMVHasOpenECInDir(const MvarMVStruct *MV, MvarMVDirType Dir);
CagdBType MvarBspMVHasOpenEC(const MvarMVStruct *MV);
CagdBType MvarBspMVIsPeriodicInDir(const MvarMVStruct *MV, MvarMVDirType Dir);
CagdBType MvarBspMVIsPeriodic(const MvarMVStruct *MV);
int MvarBspMVInteriorKnots(const MvarMVStruct *MV, CagdRType *Knot);

MvarMVStruct *MvarMVMultiLinearMV(const IrtRType *Min,
				  const IrtRType *Max, 
				  int Dim);

void MvarDbg(const void *Obj);
#ifdef DEBUG
void MvarDbgDsp(const void *Obj);
void MvarDbgInfo(const void **Objs, int Size);
void MvarETDbg(const MvarExprTreeStruct *ET);
#endif /* DEBUG */

MvarMVStruct *MvarMVExtension(const MvarMVStruct *OrigMV,
			      const CagdBType *ExtMins,
			      const CagdBType *ExtMaxs,
			      const CagdRType *Epsilons);
CagdBType MvarMVKnotHasC0Discont(const MvarMVStruct *MV,
			         int *Dir,
			         CagdRType *t);
CagdBType MvarMVMeshC0Continuous(const MvarMVStruct *MV,
				 int Dir,
				 int Idx);
CagdBType MvarMVIsMeshC0DiscontAt(const MvarMVStruct *MV,
				  int Dir,
				  CagdRType t);
CagdBType MvarMVKnotHasC1Discont(const MvarMVStruct *MV,
				 int *Dir,
				 CagdRType *t);
CagdBType MvarMVMeshC1Continuous(const MvarMVStruct *MV,
				 int Dir,
				 int Idx);
CagdBType MvarMVIsMeshC1DiscontAt(const MvarMVStruct *MV,
				  int Dir,
				  CagdRType t);
MvarKrnlTVRes *MvarKrnlTVResNew();
void MvarKrnlTVResFree(MvarKrnlTVRes *Res);
MvarKrnlTVRes *MvarKrnlBooleanSumTV(const CagdSrfStruct *Srf1,
				const CagdSrfStruct *Srf2,
				const CagdSrfStruct *Srf3,
				const CagdSrfStruct *Srf4,
				const CagdSrfStruct *Srf5,
				const CagdSrfStruct *Srf6,
				IrtRType DistRatio,
				int DegreeRaiseLimit,
				int KnotInsertionLimit,
				int IsSingular,
				struct IPObjectStruct *PKrnlPts);
MvarKrnlTVRes *MvarKrnRuledSumTV(const CagdSrfStruct *Srf1,
					 const CagdSrfStruct *Srf2,
					IrtRType DistRatio,
					int DegreeRaiseLimit,
					int KnotInsertionLimit,
					int IsSingular,
					struct IPObjectStruct *PKrnlPts);
/******************************************************************************
* Fitting and interpolation						      *
******************************************************************************/
CagdCrvStruct *MvarBspCrvInterpVecs(const MvarVecStruct *PtList,
				    int Order,
				    int CrvSize,
				    CagdParametrizationType ParamType,
				    CagdBType Periodic);
MvarVecStruct *MvarPtsSortAxis(MvarVecStruct *PtList, int Axis);
void MvarPointFromPointLine(const MvarVecStruct *Point,
			    const MvarVecStruct *Pl,
			    const MvarVecStruct *Vl,
			    MvarVecStruct *ClosestPoint);
IrtRType MvarDistPointLine(const MvarVecStruct *Point,
			   const MvarVecStruct *Pl,
			   const MvarVecStruct *Vl);
CagdRType MvarLineFitToPts(const MvarVecStruct *PtList,
			   MvarVecStruct *LineDir,
			   MvarVecStruct *LinePos);

/******************************************************************************
* Symbolic computation over multivariates.				      *
******************************************************************************/
MvarMVStruct *MvarMVAdd(const MvarMVStruct *MV1, const MvarMVStruct *MV2);
MvarMVStruct *MvarMVSub(const MvarMVStruct *MV1, const MvarMVStruct *MV2);
MvarMVStruct *MvarMVMult(const MvarMVStruct *MV1, const MvarMVStruct *MV2);
MvarMVStruct *MvarMVInvert(const MvarMVStruct *MV);
MvarMVStruct *MvarMVScalarScale(const MvarMVStruct *MV, CagdRType Scale);
MvarMVStruct *MvarMVScalarScale2(const MvarMVStruct *CMV,
				 const CagdRType *Scale);
MvarMVStruct *MvarMVMultScalar(const MvarMVStruct *MV1,
			       const MvarMVStruct *MV2);
MvarMVStruct *MvarMVDotProd(const MvarMVStruct *MV1, const MvarMVStruct *MV2);
MvarMVStruct *MvarMVVecDotProd(const MvarMVStruct *MV, const CagdRType *Vec);
MvarMVStruct *MvarMVCrossProd(const MvarMVStruct *MV1,
			      const MvarMVStruct *MV2);
MvarMVStruct *MvarMVCrossProdZ(const MvarMVStruct *MV1,
			       const MvarMVStruct *MV2);
MvarMVStruct *MvarMVCrossProd2D(const MvarMVStruct *MV1X,
				const MvarMVStruct *MV1Y,
				const MvarMVStruct *MV2X,
				const MvarMVStruct *MV2Y);
MvarMVStruct *MvarMVRtnlMult(const MvarMVStruct *MV1X,
			     const MvarMVStruct *MV1W,
			     const MvarMVStruct *MV2X,
			     const MvarMVStruct *MV2W,
			     CagdBType OperationAdd);
MvarMVStruct *MvarMVMultBlend(const MvarMVStruct *MV1,
			      const MvarMVStruct *MV2,
			      CagdRType Blend);
void MvarMVSplitScalar(const MvarMVStruct *MV, MvarMVStruct **MVs);
MvarMVStruct *MvarMVMergeScalar(MvarMVStruct * const *ScalarMVs);

int MvarBspMultComputationMethod(int BspMultUsingInter);

void MVarBzrMVDivide(const MvarMVStruct *MV1,
		     const MvarMVStruct *MV2,
		     MvarMVStruct **Q,
		     MvarMVStruct **R);
MvarMVStruct *MvarBzrMultBrnBasis(const MvarMVStruct *MV,
			          int *MulOrders,
				  int *MulIndices);
MvarMVStruct *MvarBzrMVMult(const MvarMVStruct *MV1, const MvarMVStruct *MV2);
MvarMVStruct *MvarBspMVMult(const MvarMVStruct *MV1, const MvarMVStruct *MV2);

MvarMVStruct *MvarBzrMVDeriveRational(const MvarMVStruct *MV,
				      MvarMVDirType Dir);
MvarMVStruct *MvarBspMVDeriveRational(const MvarMVStruct *MV,
				      MvarMVDirType Dir);

MvarMVStruct *MvarMVDeterminant2(const MvarMVStruct *MV11,
				 const MvarMVStruct *MV12,
				 const MvarMVStruct *MV21,
				 const MvarMVStruct *MV22);
MvarMVStruct *MvarMVDeterminant3(const MvarMVStruct *MV11,
				 const MvarMVStruct *MV12,
				 const MvarMVStruct *MV13,
				 const MvarMVStruct *MV21,
				 const MvarMVStruct *MV22,
				 const MvarMVStruct *MV23,
				 const MvarMVStruct *MV31,
				 const MvarMVStruct *MV32,
				 const MvarMVStruct *MV33);
MvarMVStruct *MvarMVDeterminant4(const MvarMVStruct *MV11,
				 const MvarMVStruct *MV12,
				 const MvarMVStruct *MV13,
				 const MvarMVStruct *MV14,
				 const MvarMVStruct *MV21,
				 const MvarMVStruct *MV22,
				 const MvarMVStruct *MV23,
				 const MvarMVStruct *MV24,
				 const MvarMVStruct *MV31,
				 const MvarMVStruct *MV32,
				 const MvarMVStruct *MV33,
				 const MvarMVStruct *MV34,
				 const MvarMVStruct *MV41,
				 const MvarMVStruct *MV42,
				 const MvarMVStruct *MV43,
				 const MvarMVStruct *MV44);
MvarMVStruct *MvarMVDeterminant5(const MvarMVStruct *MV11,
				 const MvarMVStruct *MV12,
				 const MvarMVStruct *MV13,
				 const MvarMVStruct *MV14,
				 const MvarMVStruct *MV15,
				 const MvarMVStruct *MV21,
				 const MvarMVStruct *MV22,
				 const MvarMVStruct *MV23,
				 const MvarMVStruct *MV24,
				 const MvarMVStruct *MV25,
				 const MvarMVStruct *MV31,
				 const MvarMVStruct *MV32,
				 const MvarMVStruct *MV33,
				 const MvarMVStruct *MV34,
				 const MvarMVStruct *MV35,
				 const MvarMVStruct *MV41,
				 const MvarMVStruct *MV42,
				 const MvarMVStruct *MV43,
				 const MvarMVStruct *MV44,
				 const MvarMVStruct *MV45,
				 const MvarMVStruct *MV51,
				 const MvarMVStruct *MV52,
				 const MvarMVStruct *MV53,
				 const MvarMVStruct *MV54,
				 const MvarMVStruct *MV55);
MvarMVStruct *MvarMVDeterminant(const MvarMVStruct * const *MVsMatrix,
				int MatrixSize);

MvarMVStruct *MvarBlendMVMV(const MvarMVStruct *MV1,
			    const MvarMVStruct *Scalar1,
			    const MvarMVStruct *MV2,
			    const MvarMVStruct *Scalar2);
MvarMVStruct *MvarBlendConvexMVMV(const MvarMVStruct *MV1,
				  const MvarMVStruct *MV2,
				  const MvarMVStruct *MVT);

MvarMVStruct *MvarSrfCalcAsympDirsVolume(const CagdSrfStruct *Srf);
int MvarSrfCalcAsympDirsCones(const CagdSrfStruct *Srf,
			      SymbNormalConeStruct *DirsCones);

/******************************************************************************
* Routines to compute zeros of MVs constraints.				      *
******************************************************************************/
MvarZeroSolutionStruct *MvarZeroSolveMatlabEqns(
					      MvarMatlabEqStruct **Eqns,
                                              int NumOfEqns,
                                              int MaxVarsNum,
					      CagdRType *MinDmn,
					      CagdRType *MaxDmn,
                                              CagdRType NumericTol,
                                              CagdRType SubdivTol,
                                              CagdRType StepTol,
					      MvarConstraintType *Constraints);
MvarPtStruct *MvarMVsZeros0D(MvarZeroPrblmSpecStruct *ZeroProblemSpec);
MvarPtStruct *MvarMVsZeros0DNumeric(MvarMVStruct * const *MVs,
				    int NumOfMVs,
				    CagdRType NumericTol,
				    int SuccessOnDmnErr,
				    MvarPtStruct *ZeroPt);
MvarPtStruct *MvarZero0DNumeric(MvarPtStruct *ZeroPt,
				const MvarExprTreeEqnsStruct *Eqns,
				MvarMVStruct const * const *MVs,
				int NumMVs,
				CagdRType NumericTol,
				int SuccessOnDmnErr,
				const CagdRType *InputMinDmn,
				const CagdRType *InputMaxDmn);
MvarPtStruct *MvarMVsZeros2DBy0D(MvarZeroPrblmSpecStruct *ZeroProblemSpec);
MvarPtStruct *MvarETsZeros0D(MvarZeroPrblmSpecStruct *ZeroProblemSpec);
CagdPtStruct *MvarCrvZeroSet(const CagdCrvStruct *Curve,
			     int Axis,
			     CagdRType SubdivTol,
			     CagdRType NumericTol,
			     CagdBType FilterTangencies);
CagdBType MvarMVsDetectZeros(MvarMVStruct * const *MVs,
			     MvarConstraintType *Constraints,
			     int NumOfMVs,
			     CagdRType SubdivTol,
			     CagdRType NumericTol,
			     int HighDimBndry);
CagdBType MvarMVsZerosSameSpace(MvarMVStruct **MVs, int NumOfMVs);
int MvarMVsZerosNormalConeTest(int NormalConeTest);
CagdRType MvarMVsZerosDmnExt(CagdRType DmnExt);
MvarZeroSubdivTolActionType MvarZerosSubdivTolAction(
				 MvarZeroSubdivTolActionType SubdivTolAction);
int MvarMVsZerosGradPreconditioning(int GradPreconditioning);
int MvarMVsZeros2DPolylines(int IsPolyLines2DSolution);
int MvarMVsZeros2DCornersOnly(int Is2DSolutionCornersOnly);
int MvarMVsZerosCrtPts(int CrtPtsDetectionByDim);
int MvarMVsZerosNormalizeConstraints(int NormalizeConstraints);
int MvarMVsZerosDomainReduction(int DomainReduction);
int MvarMVsZerosParallelHyperPlaneTest(int ParallelHPlaneTest);
int MvarMVsZerosKantorovichTest(int KantorovichTest);

MvarZeroSolutionStruct *MvarZeroSolverSolutionNew(
					    MvarTriangleStruct *Tr,
					    MvarPolylineStruct *Pl,
					    MvarPtStruct *Pt,
					    MvarZrSlvrRepresentationType Rep);
MvarZeroSolutionStruct *MvarZeroSolverSolCpy(MvarZeroSolutionStruct const *Sol);
MvarZeroSolutionStruct *MvarZeroSolutionCpyList(MvarZeroSolutionStruct 
						const *SolutionList);
MvarMVDDecompositionModeType MvarMVDSetDecompositionMode(
					      MvarMVDDecompositionModeType m);
void MvarZeroSolverSolutionFree(MvarZeroSolutionStruct *Solution,
				CagdBType FreeUnion);

#ifdef DEBUG
void MvarMVsZerosVerifier(MvarMVStruct * const *MVs,
			  int NumOfZeroMVs,
			  MvarPtStruct *Sols,
			  CagdRType NumerEps);
#endif /* DEBUG */

/******************************************************************************
* Routines to compute srf-srf intersections and MV univariate solutions.      *
******************************************************************************/
MvarPolylineStruct *MvarSrfSrfInter(const CagdSrfStruct *Srf1, 
				    const CagdSrfStruct *Srf2,
				    CagdRType Step,
				    CagdRType SubdivTol,
				    CagdRType NumericTol);
MvarPolylineStruct *MvarSrfSrfInterDisc(const CagdSrfStruct *Srf1, 
					const CagdSrfStruct *Srf2,
					CagdRType Step,
					CagdRType SubdivTol,
					CagdRType NumericTol);
CagdRType MvarSrfSrfInterNormalizeDomain(CagdRType NewValue);
IrtBType MvarSrfSrfInterExamineBBoxes(const CagdSrfStruct *Srf1,
				      const CagdSrfStruct *Srf2);
MvarPolylineStruct *MvarMVsZeros1D(MvarZeroPrblmSpecStruct *ZeroProblemSpec);
MvarPolylineStruct *MvarMVsZeros1DTrace2Pts(MvarMVStruct * const *MVs,
				            MvarConstraintType *Constraints,
				            int NumOfMVs,
				            MvarPtStruct *StartEndPts,
				            CagdRType Step,
				            CagdRType SubdivTol,
				            CagdRType NumericTol);
int MvarMVsZeros1DMergeSingularPts(int MergeSingularPts); 

MvarPolylineStruct *MvarSrfZeroSet(const CagdSrfStruct *Surface,
			           int Axis,
			           CagdRType Step,
			           CagdRType SubdivTol,
			           CagdRType NumericTol,
				   MvarMVsZerosVerifyAllSolsCBFuncType
							    OutputPtsFilterCB,
				   void *OutputPtsFilterCBData);
CagdBType MvarSrfSrfTestInter(const CagdSrfStruct *Srf1, 
			      const CagdSrfStruct *Srf2,
			      CagdRType Step,
			      CagdRType SubdivTol,
			      CagdRType NumericTol);


/******************************************************************************
* Routines to manage Srf-Srf intersection results cache.		      *
******************************************************************************/
MvarSrfSrfInterCacheStruct *MvarSrfSrfInterCacheAlloc(
						    IPAttrIDType AttribID,
						    CagdBType ShouldAssignIds);
int MvarSrfSrfInterCacheGetSrfId(const MvarSrfSrfInterCacheStruct *SSICache, 
				 const CagdSrfStruct *Srf);
MvarSrfSrfInterCacheDataStruct *MvarSrfSrfInterCacheGetData(
				   const MvarSrfSrfInterCacheStruct *SSICache,
				   const CagdSrfStruct *Srf1, 
				   const CagdSrfStruct             *Srf2);
CagdBType MvarSrfSrfCacheShouldAssignIds(const MvarSrfSrfInterCacheStruct 
					                        *DataCache);
MvarSrfSrfInterCacheDataStruct *MvarSrfSrfInterCacheAddData(
				       MvarSrfSrfInterCacheStruct *SSICache, 
				       CagdSrfStruct *Srf1, 
				       CagdSrfStruct *Srf2, 
				       MvarPolylineStruct *Data);
void MvarSrfSrfInterCacheClear(MvarSrfSrfInterCacheStruct *SSICahce);
void MvarSrfSrfInterCacheFree(MvarSrfSrfInterCacheStruct *SSICahce);

/******************************************************************************
* Medial axis bi- and tri-tangent circles to freeform (self) curve(s).        *
******************************************************************************/
MvarPtStruct *MvarSCvrCircTanToCrvEndPtCntrOnCrv(
					       const CagdCrvStruct *Crv1,
					       const CagdPType Pt2,
					       const CagdCrvStruct *CntrOnCrv,
					       CagdRType RadiusLB,
					       CagdRType SubdivTol,
					       CagdRType NumericTol);
MvarPtStruct *MvarSCvrCircTanTo2CrvsCntrOnCrv(const CagdCrvStruct *Crv,
					      const CagdCrvStruct *CntrOnCrv,
					      CagdRType RadiusLB,
					      CagdRType SubdivTol,
					      CagdRType NumericTol);
MvarPtStruct *MvarSCvrCircTanTo3CrvsExprTreeNoDiagonal(
						     const CagdCrvStruct *Crv,
						     CagdRType RadiusLB,
						     CagdRType SubdivTol,
						     CagdRType NumericTol);
MvarPtStruct *MvarSCvrCircTanTo2CrvsEndPtNoDiag(const CagdCrvStruct *Crv1,
						const CagdCrvStruct *Crv2,
						const CagdPType Pt3,
						CagdBType ElimDiagonals,
						CagdRType RadiusLB,
						CagdRType SubdivTol,
						CagdRType NumericTol);
MvarPtStruct *MvarSCvrCircTanToCrvEndPt(const CagdCrvStruct *Crv1,
					const CagdPType Pt2,
					CagdRType RadiusLB,
					CagdRType SubdivTol,
					CagdRType NumericTol);
MvarPtStruct *MvarSCvrCircTanToCrv2EndPt(const CagdCrvStruct *Crv1,
					 const CagdPType Pt2,
					 const CagdPType Pt3,
					 CagdRType RadiusLB,
					 CagdRType SubdivTol,
					 CagdRType NumericTol);
MvarPtStruct *MvarSCvrBiNormals(const CagdCrvStruct *Crv1,
				const CagdCrvStruct *Crv2,
				CagdBType ElimDiagonals,
				CagdRType RadiusLB,
				CagdRType SubdivTol,
				CagdRType NumericTol);
CagdSrfStruct *MvarSCvrUMinusVSrf(const CagdRType *TMin, const CagdRType *TMax);
CagdSrfStruct *MvarSCvrPromoteCrvToSrfWithOtherCrv(
						const CagdCrvStruct *Crv,
						CagdSrfDirType Dir,
						const CagdCrvStruct *OtherCrv);

/******************************************************************************
* Circle packing related routines.				              *
******************************************************************************/
MvarPtStruct *MvarCircTanToCircCrv3By3(const CagdCrvStruct *Bndry,
				       const CagdCrvStruct *InNrml,
				       CagdPType XBnd,
				       CagdPType YBnd,
				       const CagdPType Center,
				       CagdRType Radius,
				       CagdBType BndBndryPar,
				       CagdRType BndryPar,
				       CagdRType NumericTol,
				       CagdRType SubdivTol);
MvarPtStruct *MvarCircOnLineTangToBdry(const CagdCrvStruct *Bndry,
				       const CagdCrvStruct *InNrml,
				       CagdRType Radius,
				       const CagdPType Dir,
				       const CagdPType Pt,
				       CagdRType NumericTol,
				       CagdRType SubdivTol);
MvarPtStruct *MvarCircTanAtTwoPts(const CagdCrvStruct *Bndry,
				  CagdPType XBnd,
				  CagdPType YBnd,
				  CagdRType Radius,
				  CagdRType NumericTol,
				  CagdRType SubdivTol);
MvarPtStruct *MvarCircAtDirMax(const CagdCrvStruct *Bndry,
			       CagdPType XBnd,
			       CagdPType YBnd,
			       CagdRType Radius,
			       const CagdPType Dir,
			       CagdRType NumericTol,
			       CagdRType SubdivTol);
MvarPtStruct *MvarCircTanToCrvXCoord(const CagdCrvStruct *Bndry,
				     const CagdCrvStruct *InNrml,
				     CagdPType YBnd,
				     CagdRType Radius,
				     CagdRType XCoord,
				     CagdRType NumericTol,
				     CagdRType SubdivTol);
MvarPtStruct *MvarCircTanToCrvYCoord(const CagdCrvStruct *Bndry,
				     const CagdCrvStruct *InNrml,
				     CagdPType XBnd,
				     CagdRType Radius,
				     CagdRType YCoord,
				     CagdRType NumericTol,
				     CagdRType SubdivTol);
/******************************************************************************
* Routines to compute zeros of MVs constraints with bivariate solutions.      *
******************************************************************************/
MvarZeroSolutionStruct *MvarMVsZeros2D(MvarZeroPrblmSpecStruct
				                             *ZeroProblemSpec);
MvarPolylineStruct *MvarZeroSolverPolyProject(MvarPolylineStruct *PolyList,
					      int *Coords,
					      int ProjDim);
int MvarMinSpanConeAvg(MvarVecStruct *MVVecs,
		       int VecsNormalized,
		       int NumOfVecs,
		       MvarNormalConeStruct *MVCone);
int MvarMinSpanCone(MvarVecStruct *MVVecs,
		    int VecsNormalized,
		    int NumOfVecs,
		    MvarNormalConeStruct *MVCone);
int MVHyperPlaneFromNPoints(MvarPlaneStruct *MVPlane,
			    MvarVecStruct * const *Vecs,
			    int n);
int MVHyperConeFromNPoints(MvarNormalConeStruct *MVCone,
			   MvarVecStruct * const *Vecs,
			   int n);
int MVHyperConeFromNPoints2(MvarNormalConeStruct *MVCone,
			    MvarVecStruct * const *Vecs,
			    int m);
int MVHyperConeFromNPoints3(MvarNormalConeStruct *MVCone,
			    MvarVecStruct * const *Vecs,
			    int m);
MvarNormalConeStruct *MVarMVNormalCone(const MvarMVStruct *MV);
MvarNormalConeStruct *MVarMVNormalCone2(const MvarMVStruct *MV,
					CagdRType * const *GradPoints,
					int TotalLength,
					int *MaxDevIndex);
MvarNormalConeStruct *MVarMVNormalConeMainAxis(const MvarMVStruct *MV,
					       MvarVecStruct **MainAxis);
MvarNormalConeStruct *MVarMVNormalConeMainAxis2(const MvarMVStruct *MV,
					        CagdRType * const *GradPoints,
						int TotalLength,
						MvarVecStruct **MainAxis);
MvarNormalConeStruct *MvarMVNormal2Cones(const MvarMVStruct *MV,
					 CagdRType ExpandingFactor,
					 int NumOfZeroMVs,
					 MvarNormalConeStruct **Cone1,
					 MvarNormalConeStruct **Cone2);
CagdBType MvarMVConesOverlap(MvarMVStruct **MVs, int NumOfZeroMVs);

/******************************************************************************
* Routines to handle Minkowski sums.					      *
******************************************************************************/
struct IPObjectStruct *MvarSrfSrfMinkowskiSum(const CagdSrfStruct *Srf1, 
				              const CagdSrfStruct *Srf2,
				              CagdRType SubdivTol,
					      CagdRType CrvTraceStep,
				              CagdRType NumericTol, 
					      int ParallelNrmls, 
					      CagdRType OffsetTrimDist);
struct IPObjectStruct *MvarCrvSrfMinkowskiSum(const CagdCrvStruct *Crv, 
                                              const CagdSrfStruct *Srf,
                                              CagdRType SubdivTol,
                                              CagdRType CrvTraceStep,
                                              CagdRType NumericTol, 
					      CagdRType OffsetTrimDist);

/******************************************************************************
* Routines to handle surface ray intersections.				      *
******************************************************************************/
MvarPtStruct *MvarSurfaceRayIntersection(const CagdSrfStruct *Srf,
					 const IrtPtType RayOrigin,
					 const IrtVecType RayDir,
					 IrtRType SubdivTol);
int MvarSrfRayIntersect(const CagdSrfStruct *Srf, 
			const CagdVType RayPt,
			const CagdVType RayDir, 
			CagdUVStruct **InterPntsUV);
int MvarTrimSrfRayIntersect(const struct TrimSrfStruct *TrimSrf, 
			    const CagdVType RayPt,
			    const CagdVType RayDir, 
			    CagdUVStruct **InterPntsUV);

/*****************************************************************************
* Routines to compute 2contact motion curves.				     *
*****************************************************************************/
MvarPolylineStruct *Mvar2CntctCompute2CntctMotion(const CagdCrvStruct *CCrvA,
						  const CagdCrvStruct *CCrvB,
						  CagdRType Step,
						  CagdRType SubdivTol,
						  CagdRType NumericTol);

/******************************************************************************
* Multivariate expression trees.					      *
******************************************************************************/

/* Conversions between expression trees and multivariates. */
MvarExprTreeStruct *MvarExprTreeFromCrv(const CagdCrvStruct *Crv,
					int NewDim,
					int StartAxis);
MvarExprTreeStruct *MvarExprTreeFromSrf(const CagdSrfStruct *Srf,
					int NewDim,
					int StartAxis);
MvarExprTreeStruct *MvarExprTreeFromMV(const MvarMVStruct *MV,
				       int NewDim,
				       int StartAxis);
MvarExprTreeStruct *MvarExprTreeFromMV2(const MvarMVStruct *MV);
MvarMVStruct *MvarExprTreeToMV(const MvarExprTreeStruct *ET);

/* Maintenance function on multivariate expression trees. */
MvarExprTreeStruct *MvarExprTreeLeafNew(CagdBType IsRef,
					MvarMVStruct *MV,
					int NewDim,
					int StartAxis,
					MvarNormalConeStruct *MVBCone,
					const MvarBBoxStruct *MVBBox);
MvarExprTreeStruct *MvarExprTreeIntrnlNew(MvarExprTreeNodeType NodeType,
					  MvarExprTreeStruct *Left,
					  MvarExprTreeStruct *Right,
					  const MvarBBoxStruct *MVBBox);
MvarExprTreeStruct *MvarExprTreeCopy(const MvarExprTreeStruct *ET,
				     CagdBType ThisNodeOnly,
				     CagdBType DuplicateMVs);
void MvarExprTreeFreeSlots(MvarExprTreeStruct *ET, CagdBType ThisNodeOnly);
void MvarExprTreeFree(MvarExprTreeStruct *ET, CagdBType ThisNodeOnly);
int MvarExprTreeSize(MvarExprTreeStruct *ET);
CagdBType MvarExprTreesSame(const MvarExprTreeStruct *ET1,
			    const MvarExprTreeStruct *ET2,
			    CagdRType Eps);
void MvarExprTreePrintInfo(const MvarExprTreeStruct *ET,
			   CagdBType CommonExprIdx,
			   CagdBType PrintMVInfo,
			   MvarExprTreePrintFuncType PrintFunc);

/* Constraint constructors using simple math operations. */
MvarExprTreeStruct *MvarExprTreeAdd(MvarExprTreeStruct *Left,
				    MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeSub(MvarExprTreeStruct *Left,
				    MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeMult(MvarExprTreeStruct *Left,
				     MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeMultScalar(MvarExprTreeStruct *Left,
					   MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeMergeScalar(MvarExprTreeStruct *Left,
					    MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeDotProd(MvarExprTreeStruct *Left,
					MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeCrossProd(MvarExprTreeStruct *Left,
					  MvarExprTreeStruct *Right);
MvarExprTreeStruct *MvarExprTreeExp(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeLog(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeCos(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeSqrt(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeSqr(MvarExprTreeStruct *Left);
MvarExprTreeStruct *MvarExprTreeNPow(MvarExprTreeStruct *Left, int Power);
MvarExprTreeStruct *MvarExprTreeRecip(MvarExprTreeStruct *Left);

/* Operations on multivariate expression trees. */
int MvarExprTreeSubdivAtParam(const MvarExprTreeStruct *ET,
			      CagdRType t,
			      MvarMVDirType Dir,
			      MvarExprTreeStruct **Left,
			      MvarExprTreeStruct **Right);
const MvarBBoxStruct *MvarExprTreeBBox(MvarExprTreeStruct *ET);
MvarBBoxStruct *MvarExprTreeCompositionDerivBBox(MvarExprTreeStruct *ET,
						 MvarBBoxStruct *BBox);
int MvarETDomain(const MvarExprTreeStruct *ET,
		 CagdRType *Min,
		 CagdRType *Max,
		 int Axis);
int MvarExprTreeLeafDomain(MvarExprTreeStruct *ET,
			   CagdRType *Min,
			   CagdRType *Max,
			   int Axis);
int MvarETUpdateConstDegDomains(MvarExprTreeStruct **MVETs, int n);
int MvarExprTreesVerifyDomain(MvarExprTreeStruct *ET1,
			      MvarExprTreeStruct *ET2);
int MvarExprTreesMakeConstDomainsUpdate(MvarExprTreeStruct **MVETs, int n);
void MvarExprAuxDomainReset(MvarExprTreeStruct *ET);
int MvarExprTreeCnvrtBsp2BzrMV(MvarExprTreeStruct *ET,
			       MvarMinMaxType *Domain);
int MvarExprTreeCnvrtBzr2BspMV(MvarExprTreeStruct *ET);

int MvarExprTreeInteriorKnots(const MvarExprTreeStruct *ET, CagdRType *Knot);
CagdRType *MvarExprTreeEval(const MvarExprTreeStruct *ET,
			    CagdRType *Params,
			    CagdRType *Pt);
CagdRType *MvarExprTreeGradient(const MvarExprTreeStruct *ET,
				CagdRType *Params,
				int *Dim,
				CagdRType *GradData,
				CagdRType *V);
MvarPlaneStruct *MvarExprTreeEvalTanPlane(const MvarExprTreeStruct *ET,
					  CagdRType *Params);

/* Zero finding over multivariate expression trees. */
int MvarExprTreeZerosUseCommonExpr(int UseCommonExpr);
int MvarExprTreeZerosCnvrtBezier2MVs(int Bezier2MVs);
MvarMVsZerosMVsCBFuncType MvarExprTreeZerosCnvrtBezier2MVsCBFunc(
					 MvarMVsZerosMVsCBFuncType MVsCBFunc);
MvarPtStruct *MvarExprTreesZeros(MvarExprTreeStruct * const *MVETs,
				 MvarConstraintType *Constraints,
				 int NumOfMVETs,
				 CagdRType SubdivTol,
				 CagdRType NumericTol);
MvarPtStruct *MvarExprTreeEqnsZeros(MvarExprTreeEqnsStruct *Eqns,
				    CagdRType SubdivTol,
				    CagdRType NumericTol);
MvarNormalConeStruct *MVarExprTreeNormalCone(MvarExprTreeStruct *Eqn);
CagdBType MvarExprTreeConesOverlap(MvarExprTreeEqnsStruct *Eqns);

/******************************************************************************
* Intersections/contacts/antipodal points in crvs and srfs.		      *
******************************************************************************/
MvarPtStruct *MvarCrvCrvInter(const CagdCrvStruct *Crv1,
			      const CagdCrvStruct *Crv2,
			      CagdRType SubdivTol,
			      CagdRType NumericTol,
			      CagdBType UseExprTree);
MvarPtStruct *MvarCrvCrvContact(const CagdCrvStruct *Crv1,
				const CagdCrvStruct *Crv2,
				const CagdCrvStruct *MotionCrv1,
				const CagdCrvStruct *ScaleCrv1,
				CagdRType SubdivTol,
				CagdRType NumericTol,
				CagdBType UseExprTree);
MvarPtStruct *MvarSrfSrfSrfInter(const CagdSrfStruct *Srf1,
				 const CagdSrfStruct *Srf2,
				 const CagdSrfStruct *Srf3,
				 CagdRType SubdivTol,
				 CagdRType NumericTol,
				 CagdBType UseExprTree);
MvarPtStruct *MvarSrfSrfContact(const CagdSrfStruct *Srf1,
				const CagdSrfStruct *Srf2,
				const CagdCrvStruct *Srf1Motion,
				const CagdCrvStruct *Srf1Scale,
				CagdRType SubdivTol,
				CagdRType NumericTol,
				CagdBType UseExprTree);

MvarMVStruct *MvarMVFactorUMinusV(const MvarMVStruct *MV);
MvarMVStruct *MvarMVFactorRMinusT(const MvarMVStruct *MV, int RIdx, int TIdx);
MvarPtStruct *MvarCrvAntipodalPoints(const CagdCrvStruct *Crv,
				     CagdRType SubdivTol,
				     CagdRType NumericTol);
MvarPtStruct *MvarSrfAntipodalPoints(const CagdSrfStruct *Srf,
				     CagdRType SubdivTol,
				     CagdRType NumericTol);
MvarPtStruct *MvarCrvSelfInterDiagFactor(const CagdCrvStruct *Crv,
					 CagdRType SubdivTol,
					 CagdRType NumericTol);
MvarPtStruct *MvarCrvSelfInterNrmlDev(const CagdCrvStruct *Crv,
				      CagdRType SubdivTol,
				      CagdRType NumericTol,
				      CagdRType MinNrmlDeviation);
void MvarBzrSelfInter4VarDecomp(const CagdSrfStruct *Srf,
				MvarMVStruct **U1MinusU3Factor,
				MvarMVStruct **U2MinusU4Factor);
MvarPolylineStruct *MvarBzrSrfSelfInterDiagFactor(const CagdSrfStruct *Srf,
					          CagdRType SubdivTol,
					          CagdRType NumericTol);
MvarPolylineStruct *MvarBspSrfSelfInterDiagFactor(const CagdSrfStruct *Srf,
					          CagdRType SubdivTol,
					          CagdRType NumericTol);
MvarPolylineStruct *MvarAdjacentSrfSrfInter(const CagdSrfStruct *Srf1,
					    const CagdSrfStruct *Srf2,
					    CagdSrfBndryType Srf1Bndry,
					    CagdRType SubdivTol,
					    CagdRType NumericTol);
MvarPolylineStruct *MvarSrfSelfInterDiagFactor(const CagdSrfStruct *Srf,
					       CagdRType SubdivTol,
					       CagdRType NumericTol);
MvarPolylineStruct *MvarSrfSelfInterNrmlDev(const CagdSrfStruct *Srf,
					    CagdRType SubdivTol,
					    CagdRType NumericTol,
					    CagdRType MinNrmlDeviation);
MvarPtStruct *MvarFindSrfMiterPointsPartial(const CagdSrfStruct *Srf, 
					    const CagdSrfStruct *NrmlSrf,
					    int Axis1, 
					    int Axis2,
					    CagdRType SubdivTol,
					    CagdRType NumericTol,
					    CagdRType UVDiffTol,
					    CagdBType Euclidean);
MvarPtStruct *MvarFindSrfMiterPoints(const CagdSrfStruct *Srf,
				     int Axis,
				     CagdRType SubdivTol,
				     CagdRType NumericTol,
				     CagdBType Euclidean);
MvarPtStruct *MvarCntctTangentialCrvCrvC1(const CagdCrvStruct *Crv1,
					  const CagdCrvStruct *Crv2,
					  CagdRType Epsilon);

/******************************************************************************
* Bisectors and Trisectors of multivariates.				      *
******************************************************************************/
MvarMVStruct *MvarMVsBisector(const MvarMVStruct *MV1,
			      const MvarMVStruct *MV2);
MvarMVStruct *MvarCrvSrfBisector(const MvarMVStruct *MV1,
				 const MvarMVStruct *MV2);
MvarMVStruct *MvarSrfSrfBisector(const MvarMVStruct *MV1,
				 const MvarMVStruct *MV2);
VoidPtr MvarCrvSrfBisectorApprox2(const MvarMVStruct *MV1,
				  const MvarMVStruct *MV2,
				  int OutputType,
				  CagdRType SubdivTol,
				  CagdRType NumericTol);
MvarZeroSolutionStruct *MvarCrvSrfBisectorApprox(const MvarMVStruct *CMV1,
					         const MvarMVStruct *CMV2,
					         CagdRType SubdivTol,
						 CagdRType NumericTol);
MvarZeroSolutionStruct *MvarSrfSrfBisectorApprox(const MvarMVStruct *CMV1,
					         const MvarMVStruct *CMV2,
					         CagdRType SubdivTol,
						 CagdRType NumericTol);
VoidPtr MvarSrfSrfBisectorApprox2(const MvarMVStruct *MV1,
				  const MvarMVStruct *MV2,
				  int OutputType,
				  CagdRType SubdivTol,
				  CagdRType NumericTol);
CagdCrvStruct *MvarCrvCrvBisector2D(const CagdCrvStruct *Crv1,
				    const CagdCrvStruct *Crv2, 
				    CagdRType Step, 
				    CagdRType SubdivTol,
				    CagdRType NumericTol, 
				    CagdRType *BBoxMin,
				    CagdRType *BBoxMax,
				    CagdBType SupportPrms);

MvarMVStruct **MvarTrisector3DCreateMVs(VoidPtr FF1, 
					VoidPtr FF2,
					VoidPtr FF3,
					CagdRType *BBoxMin,
					CagdRType *BBoxMax,
					int *Eqns);
MvarPolylineStruct *MvarTrisectorCrvs(VoidPtr FF1,
				      VoidPtr FF2,
				      VoidPtr FF3,
				      CagdRType Step, 
				      CagdRType SubdivTol,
				      CagdRType NumericTol,
				      CagdRType *BBoxMin,
				      CagdRType *BBoxMax);

/******************************************************************************
* Voronoi cell computation						      *
******************************************************************************/
CagdCrvStruct *MvarCrv2DMAT(const CagdCrvStruct *OCrv,
			    CagdRType SubdivTol,
			    CagdRType NumericTol,
			    CagdBType InvertOrientation);
struct IPObjectStruct *MvarComputeVoronoiCell(CagdCrvStruct *Crv);
CagdCrvStruct *MvarBsctTrimCrvPt(CagdCrvStruct *Crv, 
				 CagdRType *Pt, 
				 CagdRType Alpha,
				 CagdCrvStruct *BaseCrv);
void MvarUniFuncsComputeLowerEnvelope(CagdCrvStruct *InputCurves, 
				      CagdCrvStruct **LowerEnvelope);

/******************************************************************************
* Bitangents/Tritangents.    						      *
******************************************************************************/

MvarPtStruct *MvarMVBiTangentLine(const CagdCrvStruct *Crv1,
				  const CagdCrvStruct *Crv2,
				  CagdRType SubdivTol,
				  CagdRType NumericTol);
MvarPolylineStruct *MvarMVBiTangents(const MvarMVStruct *MV1,
				     const MvarMVStruct *MV2,
				     CagdRType SubdivTol,
				     CagdRType NumericTol);
MvarPtStruct *MvarMVTriTangents(const MvarMVStruct *MV1,
				const MvarMVStruct *MV2,
				const MvarMVStruct *MV3,
				int Orientation,
				CagdRType SubdivTol,
				CagdRType NumericTol);
MvarPtStruct *MvarCircTanTo2Crvs(const CagdCrvStruct *Crv1,
				 const CagdCrvStruct *Crv2,
				 CagdRType Radius,
				 CagdRType Tol);
MvarPtStruct *MvarCircTanTo3Crvs(const CagdCrvStruct *Crv1,
				 const CagdCrvStruct *Crv2,
				 const CagdCrvStruct *Crv3,
				 CagdRType SubdivTol,
				 CagdRType NumericTol,
				 CagdBType OneSideOrientation);
void MvarMVTriTangentLineCreateMVs(const MvarMVStruct *CMV1,
				   const MvarMVStruct *CMV2,
				   const MvarMVStruct *CMV3,
				   MvarMVStruct **MVs,
				   MvarConstraintType *Constraints);
void MvarMVTriTangentLineCreateETs(const MvarMVStruct *CMV1,
				   const MvarMVStruct *CMV2,
				   const MvarMVStruct *CMV3,
				   MvarExprTreeStruct **ETs,
				   MvarConstraintType *Constraints);
CagdCrvStruct *MvarMVTriTangentLine(const CagdSrfStruct *Srf1,
				    const CagdSrfStruct *Srf2,
				    const CagdSrfStruct *Srf3,
				    CagdRType StepSize,
				    CagdRType SubdivTol,
				    CagdRType NumericTol,
				    int Euclidean);

CagdCrvStruct *MvarRoundChamferCrvAtC1DiscontArc(CagdCrvStruct **TCrv1,
						 CagdCrvStruct **TCrv2,
						 CagdCrvCornerType CornerType,
						 CagdRType Radius);
CagdCrvStruct *MvarRoundChamferCrvAtC1Discont(const CagdCrvStruct *Crv,
					      CagdCrvCornerType CornerType,
				              CagdRType Radius);

/******************************************************************************
 * Kernel and related analysis of curves.				      *
******************************************************************************/
MvarMVStruct *MvarCrvKernel(const CagdCrvStruct *Crv);
MvarMVStruct *MvarCrvGammaKernel(const CagdCrvStruct *Crv, CagdRType Gamma);
MvarMVStruct *MvarCrvGammaKernelSrf(const CagdCrvStruct *Crv,
				    CagdRType ExtentScale,
				    CagdRType GammaMax);
MvarPolylineStruct *MvarCrvKernelSilhouette(const CagdCrvStruct *Crv,
					    CagdRType Gamma,
					    CagdRType SubEps,
					    CagdRType NumEps);
struct IPObjectStruct *MvarCrvDiameter(const CagdCrvStruct *Crv,
				       CagdRType SubEps,
				       CagdRType NumEps);
struct IPObjectStruct *MvarCrvKernelPoint(const CagdCrvStruct *Crvs,
					  CagdRType SubEps,
					  int OneKernelPoint,
					  IrtRType PreciseBBoundTol,
					  CagdRType Gamma, 
					  int NumTanSamples,
					  CagdBType CnvxHull);
struct IPObjectStruct *MvarCrvArtGalleryPoint(const CagdCrvStruct *Crv,
					      CagdRType SubEps,
					      int NumGuards,
					      int BBoxSubdivDepth);

/******************************************************************************
 * Kernel and related analysis and construction of surfaces.		      *
******************************************************************************/
struct IPObjectStruct *MvarSrfKernelPointIneql(const CagdSrfStruct *Srfs,
					       CagdRType SubEps,
					       CagdRType ZeroEps,
					       CagdRType BoxScale,
					       CagdRType Gamma,
					       int NumTanSamples,
					       CagdBType DmnBoxOutput);
struct IPObjectStruct *MvarEvalCnvxHullRegion4Curves(
						   const CagdCrvStruct *Left,
						   const CagdCrvStruct *Right,
						   const CagdCrvStruct *Top,
						   const CagdCrvStruct *Bottom,
						   int PreciseCnvxHull);
void MvarPlnrKrnlSrfResFree(MvarPlnrKrnlSrfRes *Res);
MvarPlnrKrnlSrfRes *MvarPlnrKrnlBooleanSumSrf(
                                             const CagdCrvStruct *CrvLeft,
					     const CagdCrvStruct *CrvRight,
					     const CagdCrvStruct *CrvTop,
					     const CagdCrvStruct *CrvBottom,
					     IrtRType DistRatio,
					     int DegreeRaiseLimit,
					     int KnotInsertionLimit,
					     int PreciseCnvxHull,
					     int IsSingular,
					     struct IPObjectStruct *PCnvxHull);
CagdSrfStruct *MvarPlnrKrnlBSumSrfUsingDegreeRaising(
					         const CagdCrvStruct *Left,
						 const CagdCrvStruct *Right,
						 const CagdCrvStruct *Top,
						 const CagdCrvStruct *Bottom,
						 IrtRType DistRatio,
						 int DegreeRaiseLimit,
						 int PreciseCnvxHull,
						 int IsSingular);
CagdSrfStruct *MvarPlnrKrnlBSumSrfUsingKnotInsertion(
						 const CagdCrvStruct *Left,
						 const CagdCrvStruct *Right,
						 const CagdCrvStruct *Top,
						 const CagdCrvStruct *Bottom,
						 IrtRType DistRatio,
						 int KnotInsertionLimit,
						 int PreciseCnvxHull,
						 int IsSingular);
MvarPlnrKrnlSrfRes *MvarPlnrKrnlRuledSrf(const CagdCrvStruct *CrvLeft,
					 const CagdCrvStruct *CrvRight,
					 IrtRType DistRatio,
					 int DegreeRaiseLimit,
					 int KnotInsertionLimit,
					 int PreciseCnvxHull,
					 int IsSingular,
					 struct IPObjectStruct *CnvxHullP);
CagdSrfStruct *MvarPlnrKrnlRuledSrfUsingDegreeRaising(
					         const CagdCrvStruct *Left,
						 const CagdCrvStruct *Right,
						 IrtRType DistRatio,
						 int DegreeRaiseLimit,
						 int PreciseCnvxHull,
						 int IsSingular);
CagdSrfStruct *MvarPlnrKrnlRuledSrfUsingKnotInsertion(
						 const CagdCrvStruct *Left,
						 const CagdCrvStruct *Right,
						 IrtRType DistRatio,
						 int KnotInsertionLimit,
						 int PreciseCnvxHull,
						 int IsSingular);
MvarPlnrKrnlSrfRes *MvarPlnrKrnlOneSidedBSumSrf(
					     const CagdCrvStruct *CrvLeft,
					     const CagdCrvStruct *CrvTop,
					     IrtRType DistRatio,
					     int DegreeRaiseLimit,
					     int KnotInsertionLimit,
					     int PreciseCnvxHull,
					     int IsSingular,
					     struct IPObjectStruct *CnvxHullP);
CagdSrfStruct *MvarPlnrKrnlOSBSumSrfUsingDegreeRaising(
						 const CagdCrvStruct *Left,
						 const CagdCrvStruct *Top,
						 IrtRType DistRatio,
						 int DegreeRaiseLimit,
						 int PreciseCnvxHull,
						 int IsSingular);
CagdSrfStruct *MvarPlnrKrnlOSBSumSrfUsingKnotInsertion(
						 const CagdCrvStruct *Left,
						 const CagdCrvStruct *Top,
						 IrtRType DistRatio,
						 int KnotInsertionLimit,
						 int PreciseCnvxHull,
						 int IsSingular);

/******************************************************************************
* Distances between manifolds as multivariates.				      *
******************************************************************************/
CagdRType *MvarDistSrfPoint(const CagdSrfStruct *Srf,
			    void *SrfPtPrepHandle,
			    const CagdPType Pt,
			    MvarPtStruct **InitialSol,
			    CagdBType MinDist,
			    CagdRType SubdivTol,
			    CagdRType NumericTol,
			    MvarPtStruct **ExtremePts,
			    CagdRType *ExtremeDistUV,
			    CagdPointType DistSpace);
void *MvarDistSrfPointPrep(const CagdSrfStruct *CSrf);
void MvarDistSrfPointFree(void *SrfPtPrepHandle);
MvarPtStruct *MvarLclDistSrfPoint(const CagdSrfStruct *Srf,
				  void *SrfPtPrepHandle,
				  const CagdPType Pt,
				  MvarPtStruct **InitialSol,
				  CagdRType SubdivTol,
				  CagdRType NumericTol);
CagdRType *MvarDistSrfLine(const CagdSrfStruct *Srf,
			   const CagdPType LnPt,
			   const CagdVType LnDir,
			   CagdBType MinDist,
			   CagdRType SubdivTol,
			   CagdRType NumericTol,
			   CagdUVType ExtremeDistUV);
MvarPtStruct *MvarLclDistSrfLine(const CagdSrfStruct *Srf,
				 const CagdPType LnPt,
				 const CagdVType LnDir,
				 CagdRType SubdivTol,
				 CagdRType NumericTol);
MvarMVStruct *MvarMVDistCrvSrf(const CagdCrvStruct *Crv1,
			       const CagdSrfStruct *Srf2,
			       int DistType);
MvarMVStruct *MvarMVDistSrfSrf(const CagdSrfStruct *Srf1,
			       const CagdSrfStruct *Srf2,
			       int DistType);

void *MvarInverseCrvOnSrfProjPrep(const CagdSrfStruct *Srf);
void MvarInverseCrvOnSrfProjFree(void *SrfPrepHandle);
CagdCrvStruct *MvarInverseCrvOnSrfProj(const CagdCrvStruct *E3Crv,
				       const CagdSrfStruct *Srf,
				       void *SrfPrepHandle,
				       CagdRType ApproxTol,
				       const CagdRType *PrevUVPt,
				       int *RedundantSol,
				       int IncrSol);
CagdCrvStruct *MvarProjCrvOnSrf(const CagdCrvStruct *Crv,
				const CagdSrfStruct *Srf,
				CagdRType Step, 
				CagdRType SubdivTol,
				CagdRType NumericTol,
				void *SrfPrepHandle,
				CagdBType GenUVCrv);
MvarPolylineStruct *MvarProjCrvOnSrf1(const CagdCrvStruct *Crv1,
				      const CagdSrfStruct *Srf2,
				      int Axes,
				      CagdRType Step, 
				      CagdRType SubdivTol,
				      CagdRType NumericTol);
MvarPolylineStruct *MvarProjCrvOnSrf2(const CagdCrvStruct *Crv1,
				      const CagdSrfStruct *Srf2,
				      CagdRType Step, 
				      CagdRType SubdivTol,
				      CagdRType NumericTol);

int MvarNumericImporveSharedPoints(const CagdSrfStruct *Srf1,
				   void *DistSrfPointPreps1,
				   CagdRType *UV1,
				   const CagdSrfStruct *Srf2,
				   void *DistSrfPointPreps2,
				   CagdRType *UV2);
CagdCrvStruct *MvarProjUVCrvOnE3CrvMatchSpeed(const CagdCrvStruct *UVLinCrv1,
					      const CagdSrfStruct *Srf1,
					      const CagdCrvStruct *UVCrv2,
					      const CagdSrfStruct *Srf2);
CagdCrvStruct *MvarEucCrvOffsetOnSrf(const CagdCrvStruct *Crv,
				     const CagdSrfStruct *Srf,
				     CagdRType Dist,
				     CagdRType Step,
				     CagdRType SubdivTol,
				     CagdRType NumerTol,
				     int Orient,
				     int LstSqrFit);
CagdCrvStruct *MvarSpiralCrvOnSrf(const CagdSrfStruct *Srf,
				  CagdSrfDirType Dir,
				  CagdRType Pitch,
				  CagdRType Step,
				  CagdRType SubdivTol,
				  CagdRType NumerTol,
				  int OutputType);
CagdCrvStruct *MvarDistCrvOfPtFromSrf(const CagdRType *Pt,
                                      const CagdSrfStruct *Srf,
                                      CagdRType Dist,
                                      CagdRType Step,
                                      CagdRType SubdivTol,
                                      CagdRType NumerTol,
                                      int LstSqrFit);

/******************************************************************************
* Metamorphosis of multivariates.					      *
******************************************************************************/
MvarMVStruct *MvarTwoMVsMorphing(const MvarMVStruct *MV1,
				 const MvarMVStruct *MV2,
				 CagdRType Blend);

/******************************************************************************
* Packing problems.							      *
******************************************************************************/
MvarPtStruct *Mvar3CircsInTriangles(const CagdPType Pts[3],
				    CagdRType SubdivTol,
				    CagdRType NumericTol);
MvarPtStruct *Mvar6CircsInTriangles(const CagdPType Pts[3],
				    CagdRType SubdivTol,
				    CagdRType NumericTol);

/******************************************************************************
* Steward Platform problem.						      *
******************************************************************************/
MvarMVStruct **MvarStewartPlatformGenEqns(const CagdPType BottomBase[3],
					  const CagdRType TopBaseEdgeLengths[6],
					  const CagdPType WorkDomain[2]);
MvarPtStruct *MvarStewartPlatformSolve(const MvarMVStruct **AllCnstrnts,
				       const CagdRType BaseConnectLengths[6],
				       const CagdPType WorkDomain[2],
				       CagdRType SubdivTol,
				       CagdRType NumericTol);
MvarPtStruct *MvarStewartPlatform2Solve(const CagdPType BottomBasePoints[3],
					const CagdRType BotTopEdgeLengths[6],
					const CagdRType TopEdgeLengths[3],
					CagdBType Rational,
					CagdRType SubdivTol,
					CagdRType NumericTol);

/******************************************************************************
* Light ray traps between n curves/surface.				      *
******************************************************************************/
MvarPtStruct *MvarComputeRayTraps(const CagdCrvStruct *Crvs,
				  int Orient,
				  CagdRType SubdivTol,
				  CagdRType NumerTol,
				  CagdBType UseExprTree);
MvarPtStruct *MvarComputeRayTraps3D(const CagdSrfStruct *Srfs,
				    int Orient,
				    CagdRType SubdivTol,
				    CagdRType NumerTol,
				    CagdBType UseExprTree);

/******************************************************************************
* Control related analysis.						      *
******************************************************************************/
MvarPtStruct *MvarCtrlComputeCrvNCycle(const CagdCrvStruct *Crv,
				       int CycLen,
				       CagdRType SubdivTol,
				       CagdRType NumerTol);
MvarPtStruct *MvarCtrlComputeSrfNCycle(const CagdSrfStruct *Srf,
				       int CycLen,
				       CagdRType SubdivTol,
				       CagdRType NumerTol);

/******************************************************************************
* Surface/Check surface accessibility analysis.				      *
******************************************************************************/
MvarPolylineStruct *MvarSrfAccessibility(const CagdSrfStruct *PosSrf,
				         const CagdSrfStruct *OrientSrf,
				         const CagdSrfStruct *CheckSrf,
				         CagdRType SubdivTol,
				         CagdRType NumerTol);
MvarPtStruct *MvarSrfSilhInflections(const CagdSrfStruct *Srf,
				     const CagdVType ViewDir,
				     CagdRType SubdivTol,
				     CagdRType NumerTol);
MvarMVStruct **MvarFlecnodalCrvsCreateMVCnstrnts(const CagdSrfStruct *CSrf);
MvarPolylineStruct *MvarSrfFlecnodalCrvs(const CagdSrfStruct *Srf, 
				         CagdRType Step, 
				         CagdRType SubdivTol, 
				         CagdRType NumerTol);
MvarPtStruct *MvarSrfFlecnodalPts(const CagdSrfStruct *Srf,
				  CagdRType SubdivTol,
				  CagdRType NumerTol);
MvarMVStruct *MVarProjNrmlPrmt2MVScl(const CagdSrfStruct *Srf,
				     const CagdSrfStruct *NrmlSrf,
				     const MvarMVStruct *MVScl);

/******************************************************************************
* Freeform curvature analysis.						      *
******************************************************************************/
MvarPtStruct *MvarSrfRadialCurvature(const CagdSrfStruct *Srf,
				     const CagdVType ViewDir,
				     CagdRType SubdivTol,
				     CagdRType NumerTol);
MvarMVStruct *MvarCrvCrvtrByOneCtlPt(const CagdCrvStruct *Crv,
				     int CtlPtIdx,
				     CagdRType Min,
				     CagdRType Max);

/******************************************************************************
* Freeform topology analysis.						      *
******************************************************************************/
MvarPtStruct *MvarImplicitCrvExtreme(const CagdSrfStruct *Srf,
				     const CagdSrfDirType Dir,
				     CagdRType SubdivTol,
				     CagdRType NumerTol);

/******************************************************************************
* Freeform poles treatments.						      *
******************************************************************************/
MvarPolylineStruct *MvarRationalSrfsPoles(const CagdSrfStruct *Srf,
				      CagdRType SubdivTol,
				      CagdRType NumericTol);
struct TrimSrfStruct *MvarSrfSplitPoleParams(const CagdSrfStruct *Srf,
					     CagdRType SubdivTol,
					     CagdRType NumericTol,
					     CagdRType OutReach);

/******************************************************************************
* Silhouette (and related curves') tracing.				      *
******************************************************************************/
struct IPObjectStruct *MvarSrfSilhouette(const CagdSrfStruct *Srf,
					 const CagdVType VDir,
					 CagdRType Step,
					 CagdRType SubdivTol,
					 CagdRType NumericTol,
					 CagdBType Euclidean);
struct IPObjectStruct *MvarSrfSilhouetteThroughPoint(const CagdSrfStruct *Srf,
					             const CagdPType VPoint,
					             CagdRType Step,
					             CagdRType SubdivTol,
					             CagdRType NumericTol,
					             CagdBType Euclidean);
struct IPObjectStruct *MvarSrfSilhouetteThroughPoint2(
						   MvarMVStruct *SrfMv,
						   const MvarMVStruct *NrmlMv,
						   const CagdPType VPoint,
						   CagdRType Step,
						   CagdRType SubdivTol,
						   CagdRType NumericTol);

/******************************************************************************
* Routines to handle the computation of 2D skeletons and minimum spanning     *
* circles.								      *
******************************************************************************/
CagdRType MvarSkel2DSetEpsilon(CagdRType NewEps);
CagdRType MvarSkel2DSetFineNess(CagdRType NewFineNess);
CagdRType MvarSkel2DSetMZeroTols(CagdRType SubdivTol, CagdRType NumerTol);
CagdRType MvarSkel2DSetOuterExtent(CagdRType NewOutExtent);
MvarSkel2DInter3PrimsStruct *MvarSkel2DInter3Prims(MvarSkel2DPrimStruct *Prim1,
						   MvarSkel2DPrimStruct *Prim2,
						   MvarSkel2DPrimStruct *Prim3);
void MvarSkel2DInter3PrimsFree(MvarSkel2DInter3PrimsStruct *SK2DInt);
void MvarSkel2DInter3PrimsFreeList(MvarSkel2DInter3PrimsStruct *SK2DIntList);

int MvarMSCircOfTwoCurves(const CagdCrvStruct *Crv1,
			  const CagdCrvStruct *Crv2,
			  CagdRType Center[2],
			  CagdRType *Radius,
			  CagdRType SubdivTol,
			  CagdRType NumerTol);
int MvarMSCircOfThreeCurves(const CagdCrvStruct *Crv1,
			    const CagdCrvStruct *Crv2,
			    const CagdCrvStruct *Crv3,
			    CagdRType Center[2],
			    CagdRType *Radius,
			    CagdRType SubdivTol,
			    CagdRType NumerTol);
int MVarIsCrvInsideCirc(const CagdCrvStruct *Crv,
			CagdRType Center[2],
			CagdRType Radius,
			CagdRType Tolerance);
int MvarMinSpanCirc(struct IPObjectStruct *Objs,
		    CagdRType *Center,
		    CagdRType *Radius,
		    CagdRType SubdivTol,
		    CagdRType NumerTol);
MvarPtStruct *MvarTanHyperSpheresofNManifolds(MvarMVStruct **MVs,
					      int NumOfMVs,
					      CagdRType SubdivTol,
					      CagdRType NumerTol,
					      CagdBType UseExprTree);

/******************************************************************************
* Routines to handle offsets.						      *
******************************************************************************/
CagdCrvStruct *MvarCrvTrimGlblOffsetSelfInter(CagdCrvStruct *Crv,
					      const CagdCrvStruct *OffCrv,
					      CagdRType TrimAmount,
					      CagdRType SubdivTol,
					      CagdRType NumericTol);
CagdCrvStruct *MvarCrvTrimGlblOffsetSelfInter2(const CagdCrvStruct *Crv,
					       CagdRType OffsetDist,
					       int Operation,
					       CagdRType SubdivTol,
					       CagdRType NumericTol);

MvarPtStruct *MvarCrvDtctSelfInterLocations(const CagdCrvStruct *Crv,
					    CagdRType OffsetDist,
					    CagdRType SubdivTol,
					    CagdRType NumericTol);
CagdCrvStruct *MvarCrvTrimSelfInterLocations(const CagdCrvStruct *Crv,
					     CagdRType OffsetDist,
					     CagdRType SubdivTol,
					     CagdRType NumericTol,
					     CagdBType PurgeSelfInters);

struct IPObjectStruct *MvarSrfTrimGlblOffsetSelfInter(
						   CagdSrfStruct *Srf,
						   const CagdSrfStruct *OffSrf,
						   CagdRType TrimAmount,
						   int Validate,
						   int Euclidean,
						   CagdRType SubdivTol,
						   CagdRType NumerTol,
						   CagdBType NumerImp);
struct IPObjectStruct *MvarSrfTrimGlblOffsetSelfInterNI(
					 	struct IPPolygonStruct *Plls, 
						const CagdSrfStruct *OffSrf, 
						CagdRType SubdivTol, 
						CagdRType NumerTol,
						int Euclidean,
						CagdRType SameUVTol);
struct IPObjectStruct *MvarSrfFindOffsetSelfInter(const CagdSrfStruct *Srf,
					          CagdRType OffsetDist,
					          CagdRType SubdivTol,
					          CagdRType NumericTol, 
					          int OutputType);

/******************************************************************************
* Routines to handle Hausdorff/minimal/maximal distances between freeforms.   *
******************************************************************************/

CagdRType MvarDistPointCrvC1(CagdPType P,
			     const CagdCrvStruct *Crv,
			     MvarHFDistParamStruct *Param,
			     CagdBType MinDist,
			     CagdRType Epsilon);
CagdRType MvarHFExtremeLclDistPointCrvC1(CagdPType P,
					 const CagdCrvStruct *Crv1,
					 const CagdCrvStruct *Crv2,
					 MvarHFDistParamStruct *Param2,
					 CagdRType Epsilon);
MvarPtStruct *MvarHFDistAntipodalCrvCrvC1(const CagdCrvStruct *Crv1,
					  const CagdCrvStruct *Crv2,
					  CagdRType Epsilon);
MvarHFDistPairParamStruct *MvarHFDistInterBisectCrvCrvC1(
						     const CagdCrvStruct *Crv1,
						     const CagdCrvStruct *Crv2,
						     CagdRType Epsilon);
CagdRType MvarHFDistFromCrvToCrvC1(const CagdCrvStruct *Crv1,
				   const CagdCrvStruct *Crv2,
				   MvarHFDistParamStruct *Param1,
				   MvarHFDistParamStruct *Param2,
				   CagdRType Epsilon);
CagdRType MvarHFDistCrvCrvC1(const CagdCrvStruct *Crv1,
			     const CagdCrvStruct *Crv2,
			     MvarHFDistParamStruct *Param1,
			     MvarHFDistParamStruct *Param2,
			     CagdRType Epsilon);

CagdRType MvarHFDistPointSrfC1(const CagdPType P,
			       const CagdSrfStruct *Srf,
			       MvarHFDistParamStruct *Param,
			       CagdBType MinDist);
CagdRType MvarHFExtremeLclDistPointSrfC1(const CagdPType P,
					 const CagdSrfStruct *Srf1,
					 const CagdSrfStruct *Srf2,
					 MvarHFDistParamStruct *Param2);
MvarPtStruct *MvarHFDistAntipodalCrvSrfC1(const CagdSrfStruct *Srf1,
					  const CagdCrvStruct *Crv2);
CagdRType MvarHFDistFromCrvToSrfC1(const CagdCrvStruct *Crv1,
				   const CagdSrfStruct *Srf2,
				   MvarHFDistParamStruct *Param1,
				   MvarHFDistParamStruct *Param2);
CagdRType MvarHFDistFromSrfToCrvC1(const CagdSrfStruct *Srf1,
				   const CagdCrvStruct *Crv2,
				   MvarHFDistParamStruct *Param1,
				   MvarHFDistParamStruct *Param2);
CagdRType MvarHFDistSrfCrvC1(const CagdSrfStruct *Srf1,
			     const CagdCrvStruct *Crv2,
			     MvarHFDistParamStruct *Param1,
			     MvarHFDistParamStruct *Param2);
MvarPtStruct *MvarHFDistAntipodalSrfSrfC1(const CagdSrfStruct *Srf1,
					  const CagdSrfStruct *Srf2);
MvarHFDistPairParamStruct *MvarHFDistBisectSrfSrfC1(const CagdSrfStruct *Srf1,
						    const CagdSrfStruct *Srf2);
MvarHFDistPairParamStruct *MvarHFDistInterBisectSrfSrfC1(
						    const CagdSrfStruct *Srf1,
						    const CagdSrfStruct *Srf2);
CagdRType MvarHFDistFromSrfToSrfC1(const CagdSrfStruct *Srf1,
				   const CagdSrfStruct *Srf2,
				   MvarHFDistParamStruct *Param1,
				   MvarHFDistParamStruct *Param2);
CagdRType MvarHFDistSrfSrfC1(const CagdSrfStruct *Srf1,
			     const CagdSrfStruct *Srf2,
			     MvarHFDistParamStruct *Param1,
			     MvarHFDistParamStruct *Param2);

MvarPtStruct *MvarCrvCrvMinimalDist(const CagdCrvStruct *Crv1,
				    const CagdCrvStruct *Crv2,
				    CagdRType *MinDist,
				    CagdBType ComputeAntipodals,
				    CagdRType Eps);
MvarPtStruct *MvarCrvSrfMinimalDist(const CagdSrfStruct *Srf1,
				    const CagdCrvStruct *Crv2,
				    CagdRType *MinDist);
MvarPtStruct *MvarSrfSrfMinimalDist(const CagdSrfStruct *Srf1,
				    const CagdSrfStruct *Srf2,
				    CagdRType *MinDist);
CagdRType MvarCrvMaxXYOriginDistance(const CagdCrvStruct *Crv,
				     CagdRType Epsilon,
				     CagdRType *Param);
CagdRType MvarSrfLineOneSidedMaxDist(const CagdSrfStruct *Srf,
				     const CagdUVType UV1,
				     const CagdUVType UV2,
				     CagdSrfDirType ClosedDir,
				     CagdRType Epsilon);
MvarPtStruct *MvarLineSrfInter(const CagdPType LinePt,
			       const CagdVType LineDir,
			       const CagdSrfStruct *Srf,
			       CagdRType SubdivTol,
			       CagdRType NumericTol);
MvarPtStruct *MvarCrvSrfInter(const CagdCrvStruct *Crv,
			      const CagdSrfStruct *Srf,
			      CagdRType SubdivTol,
			      CagdRType NumericTol);

/******************************************************************************
* Routines to handle curve on surface projections.			      *
******************************************************************************/
MvarPolylineStruct *MvarMVOrthoCrvProjOnSrf(const CagdCrvStruct *Crv,
					    const CagdSrfStruct *Srf,
					    CagdRType Tol);
MvarPolylineStruct *MvarMVOrthoCrvProjOnTrimSrf(const CagdCrvStruct *Crv,
						const TrimSrfStruct *TSrf,
						CagdRType Tol);
MvarPolylineStruct *MvarMVOrthoCrvProjOnModel(const CagdCrvStruct *Crv,
					      const MdlModelStruct *Mdl,
					      CagdRType Tol,
					      TrimSrfStruct **TSrfs);
MvarPolylineStruct *MvarMVOrthoIsoCrvProjOnSrf(const CagdSrfStruct *Srf1,
					       const CagdRType RVal,
					       const CagdRType CrvT0,
					       const CagdRType CrvT1,
					       CagdSrfDirType Dir,
					       const CagdSrfStruct *Srf2,
					       CagdRType Tol);

/******************************************************************************
* Routines to handle basis function conversions.			      *
******************************************************************************/
MvarMVStruct *MvarCnvrtPeriodic2FloatMV(const MvarMVStruct *MV);
MvarMVStruct *MvarCnvrtFloat2OpenMV(const MvarMVStruct *MV);

/******************************************************************************
* Routines to handle ruled surface approximation over freeform surfaces.      *
* Piecewise ruled surface approximation can be used for, for example flank    *
* milling, wire EDM, hot wire cutting).					      *
******************************************************************************/
CagdCrvStruct *MvarPiecewiseRuledAlgApproxLineAnalyze(
						const CagdSrfStruct *Srf,
						CagdRType Tolerance, 
						CagdCrvStruct **StripBoundries,
						int CrvSizeReduction,
						CagdRType SubdivTol,
						CagdRType NumericTol);
struct IPObjectStruct *MvarPiecewiseRuledAlgApproxBuildRuledSrfs(
					   const struct IPObjectStruct *Srf,
					   const struct IPObjectStruct *TPath);

CagdSrfStruct *MvarPiecewiseDvlpAlgApproxLineAnalyze(
					      const CagdSrfStruct *Srf,
					      CagdRType Tolerance,
					      CagdCrvStruct **StripBoundriesUV,
					      int CrvSizeReduction,
					      CagdRType SubdivTol,
					      CagdRType NumericTol,
					      CagdRType SrfExtent,
					      int DvlpSteps);
struct IPObjectStruct *MvarDevelopSrfFromCrvSrf(
					      const CagdSrfStruct *Srf,
					      const CagdCrvStruct *Crv,
					      const CagdCrvStruct *OrientField,
					      int CrvSizeReduction,
					      CagdRType SubdivTol,
					      CagdRType NumericTol,
					      CagdBType Euclidean);
CagdSrfStruct *MvarDevelopSrfFromCrvSrfMakeSrfs(const CagdCrvStruct *Crv,
						const CagdSrfStruct *Srf,
						const CagdCrvStruct *UVTCrvs,
						int CrvSizeReduction);

/******************************************************************************
* Routines to multivariate algebraic symbolic manipulation as strings.        *
******************************************************************************/
void *MvarZrAlgCreate();
void MvarZrAlgDelete(void *MVZrAlg);
int MvarZrAlgAssignExpr(void *MVZrAlg, const char *Name, const char *Expr);
int MvarZrAlgAssignNumVar(void *MVZrAlg, const char *Name, CagdRType Val);
int MvarZrAlgAssignMVVar(void *MVZrAlg,
			 const char *Name,
			 CagdRType DmnMin,
			 CagdRType DmnMax,
			 const MvarMVStruct *MV);
int MvarZrAlgGenMVCode(void *MVZrAlg, const char *Expr, FILE *f);

/******************************************************************************
* Routines to manipulate trivariate functions.				      *
******************************************************************************/
TrivTVStruct *MvarTrivarBoolOne(const CagdSrfStruct *Srf);
TrivTVStruct *MvarTrivarBoolSum(const CagdSrfStruct *Srf1,
				const CagdSrfStruct *Srf2,
				const CagdSrfStruct *Srf3,
				const CagdSrfStruct *Srf4,
				const CagdSrfStruct *Srf5,
				const CagdSrfStruct *Srf6);
TrivTVStruct *MvarTrivarBoolSum2(const CagdSrfStruct *UMin,
				 const CagdSrfStruct *UMax,
				 const CagdSrfStruct *VMin,
				 const CagdSrfStruct *VMax,
				 const CagdSrfStruct *WMin,
				 const CagdSrfStruct *WMax);
TrivTVStruct *MvarTrivarBoolSum3(const CagdSrfStruct *Srf1,
				 const CagdSrfStruct *Srf2,
				 const CagdSrfStruct *Srf3,
				 const CagdSrfStruct *Srf4,
				 const CagdSrfStruct *Srf5,
				 const CagdSrfStruct *Srf6);
TrivTVStruct *MvarTrivarBoolSumRtnl(const CagdSrfStruct *Srf1,
				    const CagdSrfStruct *Srf2,
				    const CagdSrfStruct *Srf3,
				    const CagdSrfStruct *Srf4,
				    const CagdSrfStruct *Srf5,
				    const CagdSrfStruct *Srf6);
TrivTVStruct *MvarTrivarHalfBoolSum(const CagdSrfStruct *Srf1,
                                    const CagdSrfStruct *Srf2);
int MvarGetSrfsCommonCrvs(const CagdSrfStruct *Srf1, 
			  const CagdSrfStruct *Srf2, 
			  CagdCrvStruct **CommonCrv);
TrivTVStruct *MvarTrivarOneSidedBoolSum2Srfs(const CagdSrfStruct *Srf1,
				             const CagdSrfStruct *Srf2);
TrivTVStruct *MvarTrivarOneSidedBoolSum3Srfs(const CagdSrfStruct *Srf1,
					     const CagdSrfStruct *Srf2,
					     const CagdSrfStruct *Srf3);
TrivTVStruct *MvarTrivarCubicTVFit(const TrivTVStruct *TV);
TrivTVStruct *MvarTrivarQuadraticTVFit(const TrivTVStruct *TV);
MvarPtStruct *MvarCalculateExtremePoints(const MvarMVStruct *MV);
void MvarTrivJacobianImprove(TrivTVStruct *TV,
			     CagdRType StepSize,
			     int NumIters);
MvarMVStruct *MvarCalculateTVJacobian(const TrivTVStruct *TV);
void MvarMakeUniquePointsList(MvarPtStruct **PtList, CagdRType Tol);
TrivTVStruct *MvarMergeTVList(const TrivTVStruct *TVList,
			     int Dir,
			     IrtRType Tolerance,
			     int InterpDiscont);
int MvarTVRglrIsNegJacobian(const TrivTVStruct *TV);
TrivTVStruct *MvarTVsRglrCorrectJacobian(TrivTVStruct *TVs);

/******************************************************************************
* Routines to handle multivariate compositions.				      *
******************************************************************************/
MvarMVStruct *MvarMVCompose(const MvarMVStruct *TargetMV,
			    const MvarMVStruct *SrcMV,
			    const int *DimMapping,
			    CagdBType DoMerge);
struct IPObjectStruct *MvarMVCompose2(const MvarMVStruct *MVMapping,
				      const MvarMVStruct *MVToCompose,
				      int HandleCrossKVLines);
struct IPObjectStruct *MvarMVCompose3(const struct IPObjectStruct
				                             *MappingFunction,
				      const struct IPObjectStruct *Obj,
				      int HandleCrossKVLines);
CagdBType MvarMVSetCompositionCheckDomains(CagdBType NewValue);
CagdBType MvarMVSetCompositionPropagateHigherDims(CagdBType NewValue);

MvarComposedSrfStruct *MvarTrimComposeMVSrf(const MvarMVStruct *MV,
					    const CagdSrfStruct *Srfs);
CagdSrfStruct *MvarUntrimComposeMVSrf(
				   const MvarMVStruct *MV,
				   const CagdSrfStruct *Srfs,
				   CagdQuadSrfWeightFuncType UntrimWeightFunc);
MdlModelStruct *MvarComposeMVMdl(const MvarMVStruct *MV,
				 const MdlModelStruct *Models);
struct VMdlVModelStruct *MvarComposeMVVModel(
					  const MvarMVStruct *MV,
					  const struct VMdlVModelStruct *Vmdl);
MvarComposedTrivStruct *MvarTrimComposeMVTV(const MvarMVStruct *MV,
				            const TrivTVStruct *TV);

MvarComposedSrfStruct *MvarComposedSrfAssumeSrf(CagdSrfStruct *Srf);
MvarComposedSrfStruct *MvarComposedSrfAssumeTSrf(TrimSrfStruct *TSrf);
MvarComposedSrfStruct *MvarComposedSrfCopy(
					 const MvarComposedSrfStruct *CompSrf);
MvarComposedSrfStruct *MvarComposedSrfCopyList(
					const MvarComposedSrfStruct *CompSrfs);
void MvarComposedSrfFree(MvarComposedSrfStruct *CompSrf);
void MvarComposedSrfFreeList(MvarComposedSrfStruct *CompSrfs);

MvarComposedTrivStruct *MvarComposedTrivAssumeTV(TrivTVStruct *TV);
MvarComposedTrivStruct *MvarComposedTrivAssumeVMdl(
					        struct VMdlVModelStruct *VMdl);
void MvarComposedTrivFree(MvarComposedTrivStruct *TVP);
void MvarComposedTrivFreeList(MvarComposedTrivStruct *TVPs);
MvarComposedTrivStruct *MvarComposedTrivCopy(const MvarComposedTrivStruct *TVP);
MvarComposedTrivStruct *MvarComposedTrivCopyList(
					    const MvarComposedTrivStruct *TVP);

/******************************************************************************
* Hyperbolic analysis  of tangent line accessibility.			      *
******************************************************************************/
CagdBType MvarSrfHyperbolicLocallyAccessibleDirs(
					const CagdSrfStruct *Srf,
					const CagdVType SrfVAxis,
					SymbNormalConeStruct *AccessibleCones);
CagdBType MvarSrfHyperbolicLocallyAccessibleDirs2(
					const CagdSrfStruct *Srf,
					SymbNormalConeStruct *AccessibleCones);

/******************************************************************************
* Error handling.							      *
******************************************************************************/
MvarSetErrorFuncType MvarSetFatalErrorFunc(MvarSetErrorFuncType ErrorFunc);
void MvarFatalError(MvarFatalErrorType ErrID);
const char *MvarDescribeError(MvarFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MVAR_LIB_H */
