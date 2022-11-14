/******************************************************************************
* User_lib.h - Header file for the User Interaction library.		      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber, Mar. 90.					      *
******************************************************************************/

#ifndef IRIT_USER_LIB_H
#define IRIT_USER_LIB_H

#include "irit_sm.h"
#include "bool_lib.h"
#include "cagd_lib.h"
#include "geom_lib.h"
#include "mvar_lib.h"
#include "iritprsr.h"

#ifdef __WINNT__
#include <wchar.h>
#define USER_FONT_STR_CONST(Str)	L##Str
#define USER_FONT_STR_CPY(DStr, SStr)	wcscpy(DStr, SStr)
#define USER_FONT_STR_CAT(DStr, SStr)	wcscat(DStr, SStr)
#define USER_FONT_STR_DUP(Str)		_wcsdup(Str)
#define USER_FONT_STR_CHR(Str, Chr)	wcschr(Str, L##Chr)
#define USER_FONT_STR_LEN(Str)		wcslen(Str)
#define USER_FONT_IS_SPACE(c)		iswspace(c)
#define USER_FONT_TEXT2INT(Str)		_wtoi(Str)
#define USER_FONT_GET_WORD_ASCII(Str)	UserWChar2Ascii(Str)
#define USER_FONT_GET_WORD_UNICODE(Str) UserAscii2WChar(Str)
#else
#define USER_FONT_STR_CONST(Str)	Str
#define USER_FONT_STR_CPY(DStr, SStr)	strcpy(DStr, SStr)
#define USER_FONT_STR_CAT(DStr, SStr)	strcat(DStr, SStr)
#define USER_FONT_STR_DUP(Str)		strdup(Str)
#define USER_FONT_STR_CHR(Str, Chr)	strchr(Str, Chr)
#define USER_FONT_STR_LEN(Str)		strlen(Str)
#define USER_FONT_IS_SPACE(c)		isspace(c)
#define USER_FONT_TEXT2INT(Str)		atoi(Str)
#define USER_FONT_GET_WORD_ASCII(Str)	(Str)
#define USER_FONT_GET_WORD_UNICODE(Str) (Str)
#endif /* __WINNT__ */

#define USER_HC_VEC_DRAW_SCALE	0.25

/* Microstructures package. */
#define USER_MICRO2_MAX_DIM 3

/* Bit settings to request a complete shell on six global faces of output.  */
/* Mutually exclusive with respect to capping.				    */
#define USER_MICRO_BIT_SHELL_UMIN 1
#define USER_MICRO_BIT_SHELL_UMAX 2
#define USER_MICRO_BIT_SHELL_VMIN 4
#define USER_MICRO_BIT_SHELL_VMAX 8
#define USER_MICRO_BIT_SHELL_WMIN 16
#define USER_MICRO_BIT_SHELL_WMAX 32
#define USER_MICRO_BIT_SHELL_ALL  0x003f

/* Bit settings to request a complete capping on six global faces of        */
/* output. Mutually exclusive with respect to shelling.			    */
#define USER_MICRO_BIT_CAP_UMIN 64
#define USER_MICRO_BIT_CAP_UMAX 128
#define USER_MICRO_BIT_CAP_VMIN 256
#define USER_MICRO_BIT_CAP_VMAX 512
#define USER_MICRO_BIT_CAP_WMIN 1024
#define USER_MICRO_BIT_CAP_WMAX 2048
#define USER_MICRO_BIT_CAP_ALL  0x0fc0

#define USER_MICRO_BIT_ALLOW_OPEN_BNDRY 4096

/* Helper macros to manage shelling and capping, in microstructures. */
#define USER_MICRO_SET_SHELL_UMIN(Flag) (Flag |= USER_MICRO_BIT_SHELL_UMIN)
#define USER_MICRO_SET_SHELL_UMAX(Flag) (Flag |= USER_MICRO_BIT_SHELL_UMAX)
#define USER_MICRO_SET_SHELL_VMIN(Flag) (Flag |= USER_MICRO_BIT_SHELL_VMIN)
#define USER_MICRO_SET_SHELL_VMAX(Flag) (Flag |= USER_MICRO_BIT_SHELL_VMAX)
#define USER_MICRO_SET_SHELL_WMIN(Flag) (Flag |= USER_MICRO_BIT_SHELL_WMIN)
#define USER_MICRO_SET_SHELL_WMAX(Flag) (Flag |= USER_MICRO_BIT_SHELL_WMAX)

#define USER_MICRO_IS_SHELL_UMIN(Flag) (Flag & USER_MICRO_BIT_SHELL_UMIN)
#define USER_MICRO_IS_SHELL_UMAX(Flag) (Flag & USER_MICRO_BIT_SHELL_UMAX)
#define USER_MICRO_IS_SHELL_VMIN(Flag) (Flag & USER_MICRO_BIT_SHELL_VMIN)
#define USER_MICRO_IS_SHELL_VMAX(Flag) (Flag & USER_MICRO_BIT_SHELL_VMAX)
#define USER_MICRO_IS_SHELL_WMIN(Flag) (Flag & USER_MICRO_BIT_SHELL_WMIN)
#define USER_MICRO_IS_SHELL_WMAX(Flag) (Flag & USER_MICRO_BIT_SHELL_WMAX)

#define USER_MICRO_SET_CAP_UMIN(Flag) (Flag |= USER_MICRO_BIT_CAP_UMIN)
#define USER_MICRO_SET_CAP_UMAX(Flag) (Flag |= USER_MICRO_BIT_CAP_UMAX)
#define USER_MICRO_SET_CAP_VMIN(Flag) (Flag |= USER_MICRO_BIT_CAP_VMIN)
#define USER_MICRO_SET_CAP_VMAX(Flag) (Flag |= USER_MICRO_BIT_CAP_VMAX)
#define USER_MICRO_SET_CAP_WMIN(Flag) (Flag |= USER_MICRO_BIT_CAP_WMIN)
#define USER_MICRO_SET_CAP_WMAX(Flag) (Flag |= USER_MICRO_BIT_CAP_WMAX)

#define USER_MICRO_IS_CAP_UMIN(Flag) (Flag & USER_MICRO_BIT_CAP_UMIN)
#define USER_MICRO_IS_CAP_UMAX(Flag) (Flag & USER_MICRO_BIT_CAP_UMAX)
#define USER_MICRO_IS_CAP_VMIN(Flag) (Flag & USER_MICRO_BIT_CAP_VMIN)
#define USER_MICRO_IS_CAP_VMAX(Flag) (Flag & USER_MICRO_BIT_CAP_VMAX)
#define USER_MICRO_IS_CAP_WMIN(Flag) (Flag & USER_MICRO_BIT_CAP_WMIN)
#define USER_MICRO_IS_CAP_WMAX(Flag) (Flag & USER_MICRO_BIT_CAP_WMAX)

#define USER_MICRO_IS_SHELL_OR_CAP_UMIN(Flag) \
		(Flag & (USER_MICRO_BIT_SHELL_UMIN | USER_MICRO_BIT_CAP_UMIN))
#define USER_MICRO_IS_SHELL_OR_CAP_UMAX(Flag) \
		(Flag & (USER_MICRO_BIT_SHELL_UMAX | USER_MICRO_BIT_CAP_UMAX))
#define USER_MICRO_IS_SHELL_OR_CAP_VMIN(Flag) \
		(Flag & (USER_MICRO_BIT_SHELL_VMIN | USER_MICRO_BIT_CAP_VMIN))
#define USER_MICRO_IS_SHELL_OR_CAP_VMAX(Flag) \
		(Flag & (USER_MICRO_BIT_SHELL_VMAX | USER_MICRO_BIT_CAP_VMAX))
#define USER_MICRO_IS_SHELL_OR_CAP_WMIN(Flag) \
		(Flag & (USER_MICRO_BIT_SHELL_WMIN | USER_MICRO_BIT_CAP_WMIN))
#define USER_MICRO_IS_SHELL_OR_CAP_WMAX(Flag) \
		(Flag & (USER_MICRO_BIT_SHELL_WMAX | USER_MICRO_BIT_CAP_WMAX))

#define USER_NON_TRIMMED_SRF_ATTR "NON_TRIMMED_SRF"

#define USER_PATCH_ACCESS_TEST    1
#define USER_PATCH_ACCESS_NO_TEST 0
#define USER_PATCH_ACCESS_ALL     -1

/* The different types of microstructures' tilings. */
typedef enum {
     USER_MICRO_TILE_REGULAR		 = 1, 
     USER_MICRO_TILE_FUNCTIONAL		 = 2, 
     USER_MICRO_TILE_RANDOM		 = 3,
     USER_MICRO_TILE_BIFURCATION_RANDOM  = 4,
     USER_MICRO_TILE_BIFURCATION_REGULAR = 5,

     USER_MICRO_TILE_FORCE_POLY      = 0x100, 
     USER_MICRO_TILE_FORCE_QUADRATIC = 0x200, 
     USER_MICRO_TILE_FORCE_CUBIC     = 0x400
} UserMicroTilingType;

typedef enum {
    USER_MICRO2_BIFUTILE_INVALID    = -1,
    USER_MICRO2_BIFUTILE_BASIC	    = 0,
    USER_MICRO2_BIFUTILE_1_2	    = 1,
    USER_MICRO2_BIFUTILE_1_4	    = 2,
    USER_MICRO2_BIFUTILE_2_2	    = 3,
    USER_MICRO2_NUM_BIFUTILE_TOPOLOGY
} UserMicro2TileTopology;
#define USER_MICRO2_TILE_MAX_BIFURCATION_TOPOLOGY 4

typedef struct UserMicro2TilingStruct *UserMicro2TilingStructPtr;

typedef char UserMicroTileName[IRIT_LINE_LEN];

/* Control points' values function definition.                              */
/*   T: A tiling.                                                           */
/*   TileIndex: The indices of the tile that contains the control point.    */
/*   CPGlobalIndex: The global indices in the whole tiling of the control   */
/*                  point.                                                  */
/*   CPIndexInTile: The indices of the control point in the tile.           */
typedef CagdRType (*UserMicroFunctionalTileCBFuncType)(
						    UserMicro2TilingStructPtr T,
						    const int *TileIndex,
						    const int *CPGlobalIndex,
						    const int *CPIndexInTile);

typedef struct UserMicroPreProcessTileCBStruct {
    /* Domain of tile in local deformation func.  Always [0, 1]. */
    CagdRType TileLclDmnMin[CAGD_MAX_PT_SIZE], TileLclDmnMax[CAGD_MAX_PT_SIZE];
    /* Domain of local deformation func.in global (orig.) deformation func. */
    CagdRType DefMapDmnMin[CAGD_MAX_PT_SIZE],
              DefMapDmnMax[CAGD_MAX_PT_SIZE];  
    /* Base index of this tile in the grid of tiles. */
    int TileIdxs[CAGD_MAX_PT_SIZE]; 
    /* Number of tiles to place in all directions in each Bezier            */
    /* patch/global (following GlobalPlacement) of deformation map.         */
    int NumOfTiles[CAGD_MAX_PT_SIZE]; 
    int NumBranchesUV[2]; /* 1 + Number of C0 discontinuities in u and v    */
			  /* directions of trivariate tiles.		    */
    IrtHmgnMatType Mat;/* Mapping of [0, 1]^3 to tile position in def. mat. */
    void *CBFuncData;  /* Input data pointer given by UserMicroParamStruct. */
} UserMicroPreProcessTileCBStruct;

typedef struct UserMicroPostProcessTileCBStruct {
    /* Domain in local deformation func. */
    CagdRType TileLclDmnMin[CAGD_MAX_PT_SIZE], TileLclDmnMax[CAGD_MAX_PT_SIZE];
    /* Domain in global (original)  deformation func. */ 
    CagdRType DefMapDmnMin[CAGD_MAX_PT_SIZE],
              DefMapDmnMax[CAGD_MAX_PT_SIZE];  
    /* Base index of this tile in the grid of tiles. */
    int TileIdxs[CAGD_MAX_PT_SIZE]; 
    /* Number of tiles to place in all directions in each Bezier            */
    /* patch/global (following GlobalPlacement) of deformation map.         */
    int NumOfTiles[CAGD_MAX_PT_SIZE];
    int NumBranchesUV[2]; /* 1 + Number of C0 discontinuities in u and v    */
			  /* directions of trivariate tiles.		    */
    IrtHmgnMatType Mat;  /* Mapping of [0, 1]^3 to tile position in defmat. */
    void *CBFuncData;  /* Input data pointer given by UserMicroParamStruct. */
} UserMicroPostProcessTileCBStruct;

typedef IPObjectStruct *(*UserMicroPreProcessTileCBFuncType)
		   (IPObjectStruct *Tile, UserMicroPreProcessTileCBStruct *d);

typedef IPObjectStruct *(*UserMicroPostProcessTileCBFuncType)
		  (IPObjectStruct *Tile, UserMicroPostProcessTileCBStruct *d);

typedef struct UserMicroTileStruct {
    struct UserMicroTileStruct *Pnext;
    struct IPAttributeStruct *Attr;
    IPObjectStruct *Geom; /* Geometry of a tile (curves/surfaces/polys etc. */
} UserMicroTileStruct;

typedef struct UserMicroTileBndryPrmStruct {
    IrtRType OuterRadius;                       /* Outer dimension of tube. */
    IrtRType InnerRadius;     /* Inner dimension of tube.  Zero to disable. */
    IrtRType YScale; /* Optional scale of cross section in higher dimension */
       /* of face, for elliptic/rectangular cross sections. Zero to ignore. */
    CagdBType Circular;/* TRUE for circular cross section, FALSE for square.*/
    CagdRType Bndry[4];   /* >0 for all four corners in lexicographic order */
                           /* if on boundary deform. TV, of that thickness. */
    CagdRType BndryShape;  /* Between zero (flat & smooth) and one (bulky). */
} UserMicroTileBndryPrmStruct;

typedef struct UserMicroRegularParamStruct {
    UserMicroTileStruct *Tile;    /* The tile to be used in regular tiling. */
    CagdBType FFDeformApprox;  /* If TRUE, do approximated Freeform deform. */
    CagdBType TilingStepMode;   /* Repeat tiling if TRUE, tiling Repeat[i]  */
    /* tiles, either globally or per Bezier domain. If FALSE, each tile is  */
    /* displace TilingSteps[i] amount, each dir (which can be overlapping). */
  CagdRType *TilingSteps[CAGD_MAX_PT_SIZE]; /* In each dimension:           */
    /* If TilingStepMode is TRUE, defines indices repetition count in each  */
    /* knot interval as (n, r1, r2, r3, ...) where n is the size of this    */
    /* vector and r1 is the number of tiles in first knot interval, r2 in   */
    /* second, etc.  In round robin mode so once last ri is visited r1 is   */
    /* visited again.  Example (3, 1, 2) for a vector of size three that    */
    /* defines repetitions of 1, 2, 1, 2, 1, 2,...			    */
    /* If TilingStepMode is FALSE, sets the displacements per tile in all   */
    /* dimensions of the deformation function's domain, with the same       */
    /* vector construction scheme of (n, d1, d2, d3, ...) of displacements. */
    CagdBType ApprxTrimCrvsInShell; /* TRUE to piecewise-linear-approximate */
    /* global boundary crvs of tiles in E3, FALSE for precise composition.  */
    /* Tolerance of approximation controlled by TrimSetTrimCrvLinearApprox. */
    CagdRType C0DiscontWScale;           /* W scale of C^0 discont. tiling. */
    CagdRType MaxPolyEdgeLen; /* For poly tiles, max edge length in tile to */
		     /* allow (longer edges are split), or zero to disable. */
    CagdBType GlobalPlacement; /* TRUE for tile placement along entire      */
    /* domain of deformation map, FALSE for placement per each Bezier patch.*/
    UserMicroPreProcessTileCBFuncType PreProcessCBFunc; /* If !NULL, a call */
    /* back func. called just before composition, editing the tile.         */
    UserMicroPostProcessTileCBFuncType PostProcessCBFunc; /* If !NULL, a    */
    /* call back func. called just after composition, editing the tile.     */
    void *CBFuncData;      /* Optional pointer to be passed to CB functions.*/
} UserMicroRegularParamStruct;

typedef struct UserMicroImplicitParamStruct {
    int NumCells[USER_MICRO2_MAX_DIM];
    int Orders[USER_MICRO2_MAX_DIM];
    int NumCPInTile[USER_MICRO2_MAX_DIM];
    CagdRType MinCPValue;
    CagdRType MaxCPValue;
    CagdBType IsC1;
} UserMicroImplicitParamStruct;

typedef struct UserMicroRandomParamStruct {
    UserMicroImplicitParamStruct TilesParams;
    CagdBType UseConnectivityGraph;
} UserMicroRandomParamStruct;

typedef struct UserMicroRandomBiFurcationParamStruct {
    int Orders[USER_MICRO2_MAX_DIM];
    int NumCPInTile[USER_MICRO2_MAX_DIM];
    CagdRType SubdThreshold;
    CagdRType RandomFactor;
} UserMicroRandomBiFurcationParamStruct;

typedef struct UserMicroRegularBiFurcationParamStruct {
     IPObjectStruct *TileTopologies[USER_MICRO2_NUM_BIFUTILE_TOPOLOGY];
     CagdRType SubdThreshold;
     int SubdDir;
} UserMicroRegularBiFurcationParamStruct;

typedef struct UserMicroFunctionalParamStruct {
    UserMicroImplicitParamStruct TilesParams;
    UserMicroFunctionalTileCBFuncType CBValueFunc;
} UserMicroFunctionalParamStruct;

typedef struct UserMicroParamStruct {
    MvarMVStruct *DeformMV;           /* The freeform deformation function. */
    UserMicroTilingType TilingType;/* Type of tiling - controls union below.*/
    int ShellCapBits;  /* 6 bits controlling global boundary shelling and 6 */
    /* bits for global capping alternatives. See USER_MICRO_BIT_SHELL/CAP_*.*/
    CagdRType ShellThickness;  /* The thickness of a SHELL global boundary. */
    int ApproxLowOrder; /* If 3 or 4, higher order freeforms results are    */
                        /* reduced (approximated) to quadratics or cubics.  */
    int _UniqueTileID;        /* Internal - current running unique tile ID. */
    int _UniqueGeomID;    /* Internal - current unique running geometry ID. */
    union {
	UserMicroRegularParamStruct RegularParam;
	UserMicroRandomParamStruct RandomParam;
	UserMicroFunctionalParamStruct FunctionalParam;
	UserMicroRandomBiFurcationParamStruct RandomBiFurcationParam;
	UserMicroRegularBiFurcationParamStruct RegularBiFurcationParam;
    } U;
} UserMicroParamStruct;

/* Used by the C^0 discont tiles to hold the respective refinement matrix. */
#define USER_ATTR_MICRO_SPLIT_TILE_UREF_MAT "UKnotRefineMatrix"
#define USER_ATTR_MICRO_SPLIT_TILE_VREF_MAT "VKnotRefineMatrix"

typedef enum {
    USER_ERR_WRONG_SRF,
    USER_ERR_MISSING_ATTRIB,
    USER_ERR_WRONG_ANGLE,
    USER_ERR_INVALID_PLANE,
    USER_ERR_RATIONAL_NO_SUPPORT,
    USER_ERR_INVALID_TEXT,
    USER_ERR_INVALID_FONT,
    USER_ERR_NON_CRV_OBJ_IN_FONT,
    USER_ERR_NO_ADJ_INFO,
    USER_ERR_NO_NRML_INFO,
    USER_ERR_NO_CRVTR_INFO,
    USER_ERR_NO_INTERSECTION,
    USER_ERR_EXPCT_REG_TRIANG,
    USER_ERR_EXPCT_POLY_OBJ,
    USER_ERR_EXPCT_SRF_OBJ,
    USER_ERR_EXPCT_VRTX_NRMLS,
    USER_ERR_EXPCT_VRTX_UVS,
    USER_ERR_UNDEFINE_ERR,
    USER_ERR_WRONG_CTLPT_INDEX,
    USER_ERR_INVALID_SIZE,
    USER_ERR_INVALID_CURVE,
    USER_ERR_INVALID_SURFACE,
    USER_ERR_INVALID_TRIM_SRF,
    USER_ERR_INVALID_DIR,
    USER_ERR_INVALID_IMAGE_SIZE,
    USER_ERR_INVALID_KV_END_COND,
    USER_ERR_INVALID_OPERATION,
    USER_ERR_XRANGE_EMPTY,
    USER_ERR_YRANGE_EMPTY,
    USER_ERR_ZRANGE_EMPTY,

    USER_ERR_NC_INVALID_PARAM,
    USER_ERR_NC_INVALID_INTER,
    USER_ERR_NC_NO_POLYLINES,
    USER_ERR_NC_MIX_CRVS_PLLNS
} UserFatalErrorType;

typedef enum {					/* Type of surface marching. */
    USER_SRF_MARCH_ISO_PARAM,
    USER_SRF_MARCH_ISOCLINES,
    USER_SRF_MARCH_ORTHOCLINES,
    USER_SRF_MARCH_PRIN_CRVTR
} UserSrfMarchType;

typedef enum {
    USER_3D_SPREAD_RANDOM,
    USER_3D_SPREAD_DIAG_PLANE,
    USER_3D_SPREAD_DIAG_PLANE2,
    USER_3D_SPREAD_ANTI_DIAG_PLANE,
    USER_3D_SPREAD_ANTI_DIAG_PLANE2,
    USER_3D_SPREAD_ANTI_DIAG_PLANE3,
    USER_3D_SPREAD_DISCONT2PLANE,
    USER_3D_SPREAD_DISCONT4PLANE,
} User3DSpreadType;

typedef enum {
    USER_IMG_SHD_3D_BLOB_NO_COLOR,
    USER_IMG_SHD_3D_BLOB_GRAY_LEVEL,
    USER_IMG_SHD_3D_BLOB_COLOR,
} UserImgShd3dBlobColorType;

typedef enum {
    USER_CA_SPLIT_NONE = 0x0000,
    USER_CA_SPLIT_INFLECTION_PTS = 0x0001,
    USER_CA_SPLIT_MAX_CRVTR_PTS =  0x0002,
    USER_CA_SPLIT_C1DISCONT_PTS =  0x0004,
    USER_CA_SPLIT_REAL_C1DISCONT_PTS = 0x0008
} UserCASplitType;

typedef enum {
    USER_CA_INPUT_NONE = 0x0000,
    USER_CA_INPUT_POLYLINES =     0x0001,
    USER_CA_INPUT_CURVES =        0x0002,
    USER_CA_INPUT_TCRVS_IN_SRFS = 0x0004
} UserCAInputType;

typedef enum {
    USER_CA_UNDEF_TYPE,
    USER_CA_HANGING_TYPE,
    USER_CA_SIMPLE_TYPE,
    USER_CA_LOOP_TYPE,
    USER_CA_COMPLEX_TYPE,
    USER_CA_LEFTOVERS_TYPE
} UserCAObjType;

typedef enum {
    USER_CA_OPER_NONE,
    USER_CA_OPER_CREATE,
    USER_CA_OPER_COPY,
    USER_CA_OPER_FILTER_DUP,
    USER_CA_OPER_FILTER_TAN,
    USER_CA_OPER_SPLIT_CRV,
    USER_CA_OPER_BREAK_LIN,
    USER_CA_OPER_BREAK_INTER,
    USER_CA_OPER_BREAK_NEAR_PTS,
    USER_CA_OPER_UNION_CRV,
    USER_CA_OPER_LSTSQR_CRV,
    USER_CA_OPER_EVAL_CA,
    USER_CA_OPER_CLASSIFY,
    USER_CA_OPER_REPORT,
    USER_CA_OPER_OUTPUT,
    USER_CA_OPER_FREE,
} UserCAOpType;

/* Type of kinematic point constraints. */
typedef enum {
    USER_KNMTCS_PT_NONE = 0,
    USER_KNMTCS_PT_FIXED,
    USER_KNMTCS_PT_XY_PLANE,
    USER_KNMTCS_PT_X_DIRECTION,
    USER_KNMTCS_PT_Y_DIRECTION,
    USER_KNMTCS_PT_Z_DIRECTION,
    USER_KNMTCS_PT_MOVES_ALONG_CURVE,
    USER_KNMTCS_PT_MOVES_ALONG_SURFACE,
    USER_KNMTCS_PT_MOVES_ALONG_ROT_CURVE,
} UserKnmtcsMovabilityPointType;

/* Type of kinematic constraints. */
typedef enum {
    USER_KNMTCS_CONSTR_NONE = 0,
    USER_KNMTCS_CONSTR_DIST_PT_PT,
    USER_KNMTCS_CONSTR_DIST_PT_BAR,
    USER_KNMTCS_CONSTR_DIST_PT_CRV,
    USER_KNMTCS_CONSTR_DIST_PT_SRF,
    USER_KNMTCS_CONSTR_DIST_BAR_BAR,
    USER_KNMTCS_CONSTR_DIST_BAR_CRV,
    USER_KNMTCS_CONSTR_DIST_BAR_SRF,
    USER_KNMTCS_CONSTR_ANGLE,
    USER_KNMTCS_CONSTR_ORTHOGONALITY,
    USER_KNMTCS_CONSTR_TANGENCY,
    USER_KNMTCS_CONSTR_PARALLEL,
    USER_KNMTCS_CONSTR_ROT_CRV,

    USER_KNMTCS_CONSTR_MIN_DIST_PT_PT,/* In equality constraints from here. */
    USER_KNMTCS_CONSTR_MAX_DIST_PT_PT,
    USER_KNMTCS_CONSTR_XDIFF_POS,
    USER_KNMTCS_CONSTR_YDIFF_POS,
    USER_KNMTCS_CONSTR_ZDIFF_POS
} UserKnmtcsConstrType;

typedef enum {
    USER_NC_GCODE_UNIT_INCHES = 0,
    USER_NC_GCODE_UNIT_MM
} UserNCGCodeUnitType;

typedef struct UserFEKElementStruct {
    IrtRType k[2][2];			  /* (x, y) x (x, y) contributions. */
} UserFEKElementStruct;

typedef struct UserFECElementStruct {
    IrtRType c[2];			           /* (x, y) contributions. */
} UserFECElementStruct;

typedef struct UserFEInterIntervalStruct {
    struct UserFEInterIntervalStruct *Pnext;
    CagdRType T1Min, T1Max;		    /* Interval of overlap in Crv1. */
    CagdRType T2Min, T2Max;		    /* Interval of overlap in Crv2. */
    CagdRType Antipodal1, Antipodal2;  /* Locations of maximal penetration. */
    CagdVType ProjNrml;		    /* Direction to project penetration on. */
} UserFEInterIntervalStruct;

/* Curves arrangement local information handled internally. */
typedef struct UserCAGenInfoStruct {
    int MapCrvsXYCurves, MapCrvsZOffsetCount;
    IrtRType MapCrvsZOffset;
    IPObjectStruct *PointOrientationList;
    char Error[IRIT_LINE_LEN];/* Last error description will be placed here.*/
} UserCAGenInfoStruct;

/* Curves arrangement holds a vector of curves, a vector of curves' end     */
/* points, and a vector of regions.					    */
/*   Tolerances and other aux. data are included as well.	            */
typedef struct UserCrvArngmntStruct {
    struct CagdCrvStruct *CagdCrvs;
    struct UserCAPtStruct *Pts;
    struct UserCACrvStruct *Crvs;
    struct UserCARegionStruct **Regions;
    struct IPObjectStruct *Output;               /* CA output is kept here. */

    IrtRType EndPtEndPtTol;  /* Tolerance to consider crvs' end points same. */
    IrtRType InternalTol;    /* Internal tolerance for CCI, inflections etc. */
    IrtRType PlanarityTol;   /* Tolerance of curves to be considered planar. */
    IrtHmgnMatType XYZ2XYMat, XY2XYZMat;     /* General plane <--> XY plane. */
    IrtPlnType CrvsPlane;
    int ProjectOnPlane;/* TRUE to force crvs to be planar, on computed plane.*/
    int AllocSizePts;      /* Size of the allocated vectors of Pts and Crvs. */
    int AllocSizeCrvs;
    int NumOfPts;
    int NumOfOrigCrvs;                     /* Number of curves in the input. */
    int NumOfCrvs;			        /* Current number of curves. */
    int NumOfRegions;			       /* Current number of regions. */

    UserCAGenInfoStruct GI;				 /* Used internally. */
} UserCrvArngmntStruct;

/* Structure which represent a kinematic point. */
typedef struct UserKnmtcsPtStruct {
    struct UserKnmtcsPtStruct *Pnext;
    UserKnmtcsMovabilityPointType Type;
    int Idx;
    CagdPType Pt;
    union {
        CagdCrvStruct *Crv;			    /* Pt moves along curve. */
        CagdSrfStruct *Srf;		 	  /* Pt moves along surface. */
    } U;
    CagdPtStruct Center;			          /* If Crv rotates. */
} UserKnmtcsPtStruct;

/* Structure which represent a kinematic bar. */
typedef struct UserKnmtcsBarStruct {
    struct UserKnmtcsBarStruct *Pnext;
    UserKnmtcsPtStruct *P1, *P2;      /* Starting & ending point of the bar. */
} UserKnmtcsBarStruct;

/* Structure which represent a kinematic constraint. */
typedef struct UserKnmtcsConstrStruct {
    struct UserKnmtcsConstrStruct *Pnext;
    UserKnmtcsConstrType Type;
    union{
        CagdRType distance;
        CagdRType angle;
    } V;
    union{
        struct {
            UserKnmtcsPtStruct *Pt1;			  /* Distance PT_PT. */
            UserKnmtcsPtStruct *Pt2;
        } DstPtPt;
        struct {
            UserKnmtcsPtStruct *Pt;			 /* Distance PT_BAR. */
            UserKnmtcsBarStruct *Bar;
        } DstPtBar;
        struct {
            UserKnmtcsPtStruct *Pt;			 /* Distance PT_CRV. */
	    UserKnmtcsPtStruct *CrvPt;			      /* Foot point. */
        } DstPtCrv;
	struct {
            UserKnmtcsPtStruct *Pt;			 /* Distance PT_SRF. */
	    UserKnmtcsPtStruct *SrfPt;			      /* Foot point. */
        } DstPtSrf;
        struct {
            UserKnmtcsBarStruct *Bar1;			/* Distance BAR_BAR. */
            UserKnmtcsBarStruct *Bar2;
        } DstBarBar;        
        struct {
            UserKnmtcsBarStruct *Bar;			/* Distance BAR_CRV. */
            CagdCrvStruct *Crv;
        } DstBarCrv;
        struct {
            UserKnmtcsBarStruct *Bar1;		    /* Angle, orthogonality. */
            UserKnmtcsBarStruct *Bar2;
        } Angle;
        struct {
            UserKnmtcsBarStruct *Bar;				/* Tangnecy. */
	    UserKnmtcsPtStruct *Pt;			   /* Contact point. */
        } Tan;
	struct {
            UserKnmtcsBarStruct *Bar;				/* Tangnecy. */
            CagdSrfStruct *Srf;
	    UserKnmtcsPtStruct *Pt;			   /* Contact point. */
        } TanSrf;
        struct {
            UserKnmtcsBarStruct *Bar1;			        /* Parallel. */
            UserKnmtcsBarStruct *Bar2;
        } Par;
    } C;
} UserKnmtcsConstrStruct;

/* Structure which represent all the kinematic simulations data. */
typedef struct UserKnmtcsStruct {
    CagdRType XMin;
    CagdRType XMax;
    CagdRType YMin;
    CagdRType YMax;
    CagdRType ZMin;
    CagdRType ZMax;
    int PtsNum;
    int BarsNum;
    int ConstraintsNum;
    struct UserKnmtcsPtStruct *Pts;		   /* Pointer to point list. */
    struct UserKnmtcsBarStruct *Bars;		     /* Pointer to bar list. */
    struct UserKnmtcsConstrStruct *Constraints;	     /* List of constraints. */
} UserKnmtcsStruct;

/* Dexel package. */

typedef enum {
    SWP_SEC_DXGRID_X,
    SWP_SEC_DXGRID_Y,
    SWP_SEC_DXGRID_Z
} UserDexelDxGridType;

typedef struct UserDexelDxGridStruct {
    UserDexelDxGridType GType;
    double Origin[2];
    double Distance[2];
    int NumDexel[2];
    struct UserDexelDxIntervalStruct ***Intrvls;    
} UserDexelDxGridStruct;

/* Font styles. */ 
typedef enum {
    USER_FONT_STYLE_REGULAR,
    USER_FONT_STYLE_ITALICS,
    USER_FONT_STYLE_BOLD,
    USER_FONT_STYLE_BOLD_ITALICS
} UserFontStyleType;

typedef enum {
    USER_FONT_3D_EDGE_NORMAL,
    USER_FONT_3D_EDGE_CHAMFER,
    USER_FONT_3D_EDGE_ROUND,
} UserFont3DEdgeType;

typedef enum {
    USER_FONT_ALIGN_LEFT,
    USER_FONT_ALIGN_CENTER,
    USER_FONT_ALIGN_RIGHT,
    USER_FONT_ALIGN_WIDE
} UserFontAlignmentType;

typedef enum {
    USER_FONT_OUTPUT_BEZIER_CRVS = 0,
    USER_FONT_OUTPUT_BSPLINE_CRVS,
    USER_FONT_OUTPUT_FILLED2D_POLYS,
    USER_FONT_OUTPUT_OUTLINE_FILLED2D_POLYS,
    USER_FONT_OUTPUT_SOLID3D_POLYS,
    USER_FONT_OUTPUT_FILLED2D_TSRFS,
    USER_FONT_OUTPUT_SOLID3D_TSRFS,
    USER_FONT_OUTPUT_SWEPT_TUBES
} UserFontGeomOutputType;

typedef struct UserFontDimInfoStruct {
    IrtRType DescentLineHeight;      /* The four height lines of this font. */
    IrtRType BaseLineHeight;
    IrtRType MeanLineHeight;
    IrtRType AscentLineHeight;
    IrtRType SpaceWidth;               /* The estimated space width to use. */
    GMBBBboxStruct BBox;    /* Of a generic char 's' in this font/size etc. */
} UserFontDimInfoStruct;

typedef struct UserPatchAccessPatchDataStruct {
    IrtRType PMax[2];
    IrtRType PMin[2];
    IrtRType EMax[3];
    IrtRType EMin[3];
    IrtRType Dir[3];
    const CagdSrfStruct *Srf;
    IrtRType Angle;
    int SrfId;
} UserPatchAccessPatchDataStruct;

typedef struct UserPatchAccessSrfParamsStruct {
    IrtRType ReqSize;                 /* Maximal size of subdivided patches. */
    IrtRType MinSize;     /* Minimal size of patches (due to angular change. */
    IrtRType Angle;     /* Maximal angular change of normals in the surface. */
} UserPatchAccessSrfParamsStruct;


#ifdef __WINNT__
typedef wchar_t *UserFontText;
typedef wchar_t UserFontChar;
#else
typedef char *UserFontText;
typedef char UserFontChar;
#endif /* __WINNT__ */

typedef char *UserFontName;

typedef struct UserFontWordLayoutStruct {
    struct UserFontWordLayoutStruct *Pnext;
    UserFontText Word;
    UserFontName FontName;
    UserFontStyleType FontStyle;
    IrtRType RelSize;                         /* Relative scale to the text. */
    UserFont3DEdgeType Font3DEdge;
    IrtPtType Font3DOptions;
    UserFontAlignmentType FontAlignment;
    struct IPObjectStruct *Geom;      /* The geometry representing the text. */
    GMBBBboxStruct BBox;       /* BBox of Geom, ignoring (X, Y) translation. */
    IrtRType X, Y;					   /* Word position. */
    IrtRType LeftOverSpace;  /* For last word in line only.  Otherwise zero. */
    IrtBType NewLine;			      /* A new line after this word. */
} UserFontWordLayoutStruct;


typedef void (*UserSetErrorFuncType)(UserFatalErrorType ErrorFunc);
typedef int (*UserRegisterTestConverganceFuncType)(IrtRType CrntDist, int i);
typedef int (*UserCntrIsValidCntrPtFuncType)(const CagdSrfStruct *Srf,
					     CagdRType U,
					     CagdRType V);
typedef void (*UserHCEditDrawCtlPtFuncType)(int PtIndex,
					    int PtUniqueID,
					    IrtRType *Pos,
					    IrtRType *TanBack,
					    IrtRType *TanForward);

/* Rocket fuel design call back functions. */
typedef CagdRType (*UserRocketFuelThrustFuncType)(void *CBData,
						  CagdRType t);
typedef CagdRType (*UserRocketFuelBasicFuelThrustFuncType)(
						   void *CBData,
						   CagdSrfStruct *LclElement,
						   CagdSrfStruct *GlblFront,
						   const int NumElements[],
						   CagdRType GlblFrontArea);
typedef CagdRType (*UserRocketFuelAccelerantRatioFuncType)(
						     void *CBData,
						     CagdRType DesiredBoost);
typedef int (*UserRocketFuelDeformElementFuncType)(void *CBData,
						   TrivTVStruct **MElemTV,
						   CagdRType *Layer1Boost,
						   CagdRType *Layer2Boost);
typedef void (*UserRocketFuelElementCBFuncType)(int LayerNum,
					        const int NumElements[],
					        int x,
					        int y,
					        CagdRType LayerArea,
					        CagdRType PatchArea,
					        CagdRType PatchDensity);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* Micro-structures package. */

IPObjectStruct *UserMicro3DCrossTile(
			          const UserMicroTileBndryPrmStruct *UMinPrms,
				  const UserMicroTileBndryPrmStruct *UMaxPrms,
				  const UserMicroTileBndryPrmStruct *VMinPrms,
				  const UserMicroTileBndryPrmStruct *VMaxPrms,
				  const UserMicroTileBndryPrmStruct *WMinPrms,
				  const UserMicroTileBndryPrmStruct *WMaxPrms);
IPObjectStruct *UserMicroBifurcate1to2Tile(
				 const UserMicroTileBndryPrmStruct *WMinPrms,
				 const UserMicroTileBndryPrmStruct *WMax1Prms,
				 const UserMicroTileBndryPrmStruct *WMax2Prms,
				 CagdRType SeparationGap,
				 CagdRType SaddleSize,
				 CagdBType Trivars);
UserMicroTileStruct *UserMicroTileNew(IPObjectStruct *Geom);
void UserMicroTileFree(UserMicroTileStruct *Tile);
void UserMicroTileFreeList(UserMicroTileStruct *Tile);
UserMicroTileStruct *UserMicroTileCopy(const UserMicroTileStruct *Tile);
UserMicroTileStruct *UserMicroTileCopyList(const UserMicroTileStruct *Tile);
UserMicroTileStruct *UserMicroParseTileFromObj(IPObjectStruct *IPObject);
UserMicroTileStruct *UserMicroTileTransform(const UserMicroTileStruct *Tile,
					    IrtHmgnMatType Mat);
UserMicroTileStruct *UserMicroReadTileFromFile(const char *FileName,
					       int Messages,
					       int MoreMessages);
IPObjectStruct *UserMicroStructComposition(UserMicroParamStruct *Param);
IPObjectStruct *UserMicroStructComposition2(UserMicroParamStruct *Param,
					    IPObjectStruct *DeformMVs);

/* Heterogeneous rocket fuel design. */
IPObjectStruct *UserRocketFuelDesign3D( 
		    const TrivTVStruct *TV,
		    int NumLayers,
		    const int NumElements[2],
		    int SliceThrough,
		    int BndrySrfs,
		    const CagdRType *ApplyRGB,
		    UserRocketFuelThrustFuncType ThrustFunc,
		    UserRocketFuelBasicFuelThrustFuncType BasicFuelThrustFunc,
		    UserRocketFuelAccelerantRatioFuncType AccelerantRatioFunc,
		    UserRocketFuelDeformElementFuncType DeformElementFunc,
		    UserRocketFuelElementCBFuncType ElementCBFunc,
		    void *CBData);

/* Generate a Tiling using a given control points values function. */
UserMicro2TilingStructPtr UserMicroFunctionalTiling(
				const MvarMVStruct *DeformMV,
				const int *NumCells,
				const int *Orders, 
				const int *NumCPInTile,
				CagdRType MinCPValue, 
				CagdRType MaxCPValue, 
				CagdRType Capping,
				int CappingBits,
				CagdBType IsC1,
				CagdBType UseConnectivityGraph,
				UserMicroFunctionalTileCBFuncType CBValueFunc);

/* Generates a random Tiling of the parametric space ([0,1]^Dim). */
UserMicro2TilingStructPtr UserMicroFunctionalRandomTiling(
					      const MvarMVStruct *DeformMV,
					      const int *NumCells,
					      const int *Orders, 
					      const int *NumCPInTile,
					      CagdRType MinVal, 
					      CagdRType MaxVal, 
					      CagdRType Capping,
					      int CappingBits,
					      CagdBType IsC1,
					      CagdBType UseConnectivityGraph);
UserMicro2TilingStructPtr UserMicroRandomBifurcationTiling(
					          const MvarMVStruct *DeformMV,
						  const int *Orders, 
						  const int *NumCPInTile,
						  CagdRType SubdivTol,
						  CagdRType RandomFactor,
						  int CappingBits,
						  CagdRType CappingValue);
IPObjectStruct *UserMicroRegularBifurcationTiling(
					    const UserMicroParamStruct *Param);
void UserMicroFunctionalFreeTiling(UserMicro2TilingStructPtr Tiling);
CagdBType UserMicroFunctionalEvaluateEucl(UserMicro2TilingStructPtr Tiling, 
					  const MvarMVStruct *DeformMV, 
					  const CagdRType *EuclideanPnt,
					  CagdRType *ResValue);
CagdBType UserMicroFunctionalEvaluateUV(UserMicro2TilingStructPtr Tiling,
					const MvarMVStruct *DeformMV, 
					const CagdRType *UVPnt,
					CagdRType *ResValue);
IPObjectStruct *UserMicroFunctionalTilingIsoSurface(
					      UserMicro2TilingStructPtr Tiling,
					      int SamplingFactor);
CagdRType UserMicroFunctionalTilingVolume(UserMicro2TilingStructPtr Tiling, 
					  CagdRType CubeSize,
					  CagdBType PositiveVol);

/* Microstructures slicings toward 3D printing. */
struct UserMicroSlicerInfoStruct *UserMicroSlicerInit(
					  const UserMicroParamStruct *MSParam,
					  int Levels);
void UserMicroSlicerSetLevel(struct UserMicroSlicerInfoStruct *Slicer,
                             const UserMicroParamStruct *MSParam,
                             int Level);
void UserMicroSlicerFree(struct UserMicroSlicerInfoStruct *Slicer);
void UserMicroSlicerSetTolerances(struct UserMicroSlicerInfoStruct *Slicer,
                                  CagdRType NumericTol,
                                  CagdRType SubdivTol,
                                  CagdRType TraceTol,
                                  CagdRType SlopeTol);
TrivTVStruct *UserMicroSlicerCreateAll(const struct UserMicroSlicerInfoStruct
				                                     *Slicer);
IPPolygonStruct *UserMicroSlicerGetOutline(struct UserMicroSlicerInfoStruct
					                              *Slicer,
                                           CagdRType z);

/* Surface-Primitive Geometry (rays, points, etc.) interactions. */
VoidPtr IntrSrfHierarchyPreprocessSrf(const CagdSrfStruct *Srf,
				      IrtRType FineNess);
void IntrSrfHierarchyFreePreprocess(VoidPtr Handle);
CagdBType IntrSrfHierarchyTestRay(VoidPtr Handle,
				  CagdPType RayOrigin,
				  CagdVType RayDir,
				  CagdUVType InterUV);
CagdBType IntrSrfHierarchyTestPt(VoidPtr Handle,
				 CagdPType Pt,
				 CagdBType Nearest,
				 CagdUVType InterUV);

/* Surface-plane contouring. */
struct IPPolygonStruct *UserCntrSrfWithPlane(const CagdSrfStruct *Srf,
					     const IrtPlnType Plane,
					     IrtRType FineNess,
					     int UseSSI,
					     int Euclidean);
struct IPPolygonStruct *UserCntrEvalToE3(
			      const CagdSrfStruct *Srf,
			      struct IPPolygonStruct *Cntrs,
			      UserCntrIsValidCntrPtFuncType ValidCntrPtFunc);
CagdSrfDirType UserInterSrfAtAllKnots(CagdSrfStruct *Srfs,
				      IrtPlnType Pln,
				      int Axis,
				      const CagdRType *KV,
				      int MinKV,
				      int MaxKV,
				      CagdRType *Param);
TrimSrfStruct *UserDivideSrfAtInterCrvs(const CagdSrfStruct *Srf,
					const CagdCrvStruct *ICrvs);
TrimSrfStruct *UserDivideOneSrfAtAllTVInteriorKnot(CagdSrfStruct *Srf,
						   const TrivTVStruct *TV);
TrimSrfStruct *UserDivideSrfsAtAllTVInteriorKnot(CagdSrfStruct *Srfs,
						 const TrivTVStruct *TV);

TrimSrfStruct *UserDivideOneSrfAtAllMVInteriorKnot(CagdSrfStruct *Srf,
						   const MvarMVStruct *MV);
TrimSrfStruct *UserDivideSrfsAtAllMVInteriorKnot(CagdSrfStruct *Srfs,
						 const MvarMVStruct *MV);
MvarPolylineStruct *UserInterSrfByAlignedHyperPlane2(const CagdSrfStruct *Srf,
				                     int Axis,
					             CagdRType t);
CagdCrvStruct *UserInterSrfByAlignedHyperPlane(const CagdSrfStruct *Srf,
				               int Axis,
					       CagdRType t);

TrimSrfStruct *UserDivideTSrfAtAllKnots(TrimSrfStruct *TSrf,
					IrtPlnType Pln,
					int Axis,
					const CagdRType *KV,
					int MinKV,
					int MaxKV,
					CagdRType *Param);
TrimSrfStruct *UserDivideOneTSrfAtAllMVInteriorKnot(const TrimSrfStruct *TSrf,
					            const MvarMVStruct *MV);
TrimSrfStruct *UserDivideTSrfsAtAllMVInteriorKnot(const TrimSrfStruct *TSrfs,
					          const MvarMVStruct *MV);

struct MdlModelStruct *UserDivideMdlAtAllKnots(struct MdlModelStruct *Model,
					IrtPlnType Pln,
					int Axis,
					const CagdRType *KV,
					int MinKV,
					int MaxKV,
					CagdRType *Param);
struct MdlModelStruct *UserDivideOneMdlAtAllMVInteriorKnot(
					    const struct MdlModelStruct *Model,
					    const MvarMVStruct *MV);
struct MdlModelStruct *UserDivideMdlsAtAllMVInteriorKnot(
					    const struct MdlModelStruct *Models,
					    const MvarMVStruct *MV);

struct VMdlVModelStruct *UserDivideVMdlAtAllKnots(
					       struct VMdlVModelStruct *VModel,
					       IrtPlnType Pln,
					       int Axis,
					       const CagdRType *KV,
					       int MinKV,
					       int MaxKV,
					       CagdRType *Param);
struct VMdlVModelStruct *UserDivideOneVMdlAtAllMVInteriorKnot(
					const struct VMdlVModelStruct *VModel,
					const MvarMVStruct *MV);
struct VMdlVModelStruct *UserDivideVMdlsAtAllMVInteriorKnot(
					const struct VMdlVModelStruct *VModels,
					const MvarMVStruct *MV);

MvarComposedTrivStruct *UserDivideTVsAtAllMVInteriorKnot(
						      const TrivTVStruct *TVs,
					              const MvarMVStruct *MV);
MvarComposedTrivStruct *UserDivideOneTVAtAllMVInteriorKnot(
						       const TrivTVStruct *TV,
						       const MvarMVStruct *MV);
MvarComposedTrivStruct *UserDivideTVAtAllKnots(MvarComposedTrivStruct *TVP,
					       IrtPlnType Pln,
					       int Axis,
					       const CagdRType *KV,
					       int MinKV,
					       int MaxKV,
					       CagdRType *Param);

/* Linear Bsplines vs polylines conversion. */

CagdCrvStruct *UserPolyline2LinBsplineCrv(const struct IPPolygonStruct *Poly,
					  CagdBType FilterDups);
CagdCrvStruct *UserPolylines2LinBsplineCrvs(
					 const struct IPPolygonStruct *Polys,
					 CagdBType FilterDups);
struct IPPolygonStruct *UserCnvrtCagdPolyline2IritPolyline(
					      const CagdPolylineStruct *Poly);
struct IPPolygonStruct *UserCnvrtCagdPolylines2IritPolylines(
					     const CagdPolylineStruct *Polys);
CagdPolylineStruct *UserCnvrtIritPolyline2CagdPolyline(
					  const struct IPPolygonStruct *Plln);

struct IPPolygonStruct *UserCnvrtLinBspCrv2IritPolyline(
						   const CagdCrvStruct *Crv,
						   int FilterIdentical);
struct IPPolygonStruct *UserCnvrtLinBspCrvs2IritPolylines(
						   const CagdCrvStruct *Crvs,
						   int FilterIdentical);
void UserCnvrtObjApproxLowOrderBzr(IPObjectStruct *Obj, int ApproxLowOrder);


/* Surface cone decomposition. */

struct IPObjectStruct *UserSrfVisibConeDecomp(const CagdSrfStruct *Srf,
				       CagdRType Resolution,
				       CagdRType ConeSize);
struct TrimSrfStruct *UserVisibilityClassify(const struct IPObjectStruct *SclrSrf,
				      struct TrimSrfStruct *TrimmedSrfs);
struct IPObjectStruct *UserViewingConeSrfDomains(
					const CagdSrfStruct *Srf,
					const CagdSrfStruct *NSrf,
					const struct IPPolygonStruct *ConeDirs,
					CagdRType SubdivTol,
					CagdRType ConeSize,
					CagdRType Euclidean);
struct IPPolygonStruct *UserSrfTopoAspectGraph(CagdSrfStruct *PSrf,
					       CagdRType SubdivTol);

/* Surface marching. */

struct IPPolygonStruct *UserMarchOnSurface(UserSrfMarchType MarchType,
					   const CagdUVType UVOrig,
					   const CagdVType DirOrig,
					   const CagdSrfStruct *Srf,
					   const CagdSrfStruct *NSrf,
					   const CagdSrfStruct *DuSrf,
					   const CagdSrfStruct *DvSrf,
					   CagdRType Length,
					   CagdRType FineNess,
					   CagdBType ClosedInU,
					   CagdBType ClosedInV);
struct IPPolygonStruct *UserMarchOnPolygons(
				        const struct IPObjectStruct *PObj,
					UserSrfMarchType MarchType,
					const struct IPPolygonStruct *PlHead,
					struct IPVertexStruct *VHead,
					CagdRType Length);

/* Curve/Surface visibility and accessibility. */

struct IPObjectStruct *UserCrvViewMap(const CagdCrvStruct *Crv,
				      const CagdCrvStruct *ViewCrv,
				      CagdRType SubTol,
				      CagdRType NumTol,
				      CagdBType TrimInvisible);
struct IPObjectStruct *UserCrvAngleMap(const CagdCrvStruct *Crv,
				       CagdRType SubdivTol,
				       CagdRType Angle);
struct IPObjectStruct *UserCrvOMDiagExtreme(const CagdCrvStruct *Crv,
					    const struct IPObjectStruct *OM,
					    int DiagExtRes);

CagdCrvStruct *UserCrvVisibleRegions(const CagdCrvStruct *Crv,
				     const CagdRType *View,
				     CagdRType Tolerance);

struct TrimSrfStruct *UserMoldReliefAngle2Srf(const CagdSrfStruct *Srf,
				       const CagdVType VDir,
				       CagdRType Theta,
				       int MoreThanTheta,
				       CagdRType SubdivTol);
CagdSrfStruct *UserMoldRuledRelief2Srf(const CagdSrfStruct *Srf,
				       const CagdVType VDir,
				       CagdRType Theta,
				       CagdRType SubdivTol);

/* Minimal distance to polylines/gons. */

IrtRType UserMinDistLineBBox(const IrtPtType LinePos,
			     const IrtVecType LineDir,
			     IrtBboxType BBox);
IrtRType UserMinDistLinePolygonList(const IrtPtType LinePos,
				    const IrtVecType LineDir,
				    struct IPPolygonStruct *Pls,
				    struct IPPolygonStruct **MinPl,
				    IrtPtType MinPt,
				    IrtRType *HitDepth,
				    IrtRType *IndexFrac);
IrtRType UserMinDistLinePolylineList(const IrtPtType LinePos,
				     const IrtVecType LineDir,
				     struct IPPolygonStruct *Pls,
				     int PolyClosed,
				     struct IPPolygonStruct **MinPl,
				     IrtPtType MinPt,
				     IrtRType *HitDepth,
				     IrtRType *IndexFrac);
IrtRType UserMinDistPointPolylineList(const IrtPtType Pt,
				      struct IPPolygonStruct *Pls,
				      struct IPPolygonStruct **MinPl,
				      struct IPVertexStruct **MinV,
				      int *Index);

/* Surface surface intersection. */

int UserSrfSrfInter(const CagdSrfStruct *Srf1,
		    const CagdSrfStruct *Srf2,
		    int Euclidean,
		    CagdRType Eps,
		    int AlignSrfs,
		    CagdCrvStruct **Crvs1,
		    CagdCrvStruct **Crvs2);

/* Jacobian of trivariates and zero set. */

struct IPObjectStruct *UserTVZeroJacobian(const struct TrivTVStruct *Tv,
					  CagdBType Euclidean,
					  int SkipRate,
					  const CagdRType Fineness[3]);
struct IPObjectStruct *UserTrivarZeros(const struct TrivTVStruct *Tv,
				       const struct TrivTVStruct *TvEuclidean,
				       int SkipRate,
				       const CagdRType Fineness[3]);

/* Z direction collision. */

IrtRType UserTwoObjMaxZRelMotion(struct IPObjectStruct *PObj1,
				 struct IPObjectStruct *PObj2,
				 IrtRType FineNess,
				 int NumIters);

/* Create 3D geometric statues from a set of images. */

struct IPObjectStruct *UserMake3DStatueFrom2Images(
					    const char *Image1Name,
					    const char *Image2Name,
					    int DoTexture,
					    const struct IPObjectStruct *Blob,
					    User3DSpreadType BlobSpreadMethod,
					    UserImgShd3dBlobColorType
						             BlobColorMethod,
					    int Resolution,
					    int Negative,
					    IrtRType Intensity,
					    IrtRType MinIntensity,
					    int MergePolys);
struct IPObjectStruct *UserMake3DStatueFrom3Images(
					    const char *Image1Name,
					    const char *Image2Name,
					    const char *Image3Name,
					    int DoTexture,
					    const struct IPObjectStruct *Blob,
					    User3DSpreadType BlobSpreadMethod,
					    UserImgShd3dBlobColorType
						             BlobColorMethod,
					    int Resolution,
					    int Negative,
					    IrtRType Intensity,
					    IrtRType MinIntensity,
					    int MergePolys);
struct IPObjectStruct *User3DMicroBlobsFrom3Images(
					    const char *Image1Name,
					    const char *Image2Name,
					    const char *Image3Name,
					    User3DSpreadType BlobSpreadMethod,
					    IrtRType Intensity,
					    const IrtVecType MicroBlobSpacing,
					    const IrtVecType RandomFactors,
					    int Resolution,
					    int Negative,
					    IrtRType CubeSize,
					    int MergePts);
struct IPPolygonStruct *User3DMicroBlobsTiling(
					IrtRType XZIntensity,
					IrtRType YZIntensity,
					IrtRType XYIntensity,
					const IrtVecType MicroBlobSpacing,
					const IrtVecType RandomFactors);
struct IPPolygonStruct *User3DMicroBlobsTiling2(
					 IrtRType XZIntensity,
					 IrtRType YZIntensity,
					 IrtRType XYIntensity,
					 const IrtVecType MicroBlobSpacing,
					 const IrtVecType RandomFactors);
int *User3DMicroBlobsCreateRandomVector(int Size,
					User3DSpreadType BlobSpreadMethod,
					IrtBType FirstVec);
int **User3DMicroBlobsCreateRandomMatrix(int Size,
					 User3DSpreadType BlobSpreadMethod);

struct IPVertexStruct *User3DDitherSetXYTranslations(
					        struct IPVertexStruct *Vrtcs);

struct IPObjectStruct *User3DDither2Images(const char *Image1Name,
					   const char *Image2Name,
					   int DitherSize,
					   int MatchWidth,
					   int Negate,
					   int AugmentContrast,
					   User3DSpreadType SpreadMethod,
					   IrtRType SphereRad,
					   IrtRType *AccumPenalty);
struct IPObjectStruct *User3DDither3Images(const char *Image1Name,
					   const char *Image2Name,
					   const char *Image3Name,
					   int DitherSize,
					   int MatchWidth,
					   int Negate,
					   int AugmentContrast,
					   User3DSpreadType SpreadMethod,
					   IrtRType SphereRad,
					   IrtRType *AccumPenalty);

/* Geometry registration. */

int UserRegisterTestConvergance(IrtRType Dist, int i);
IrtRType UserRegisterTwoPointSets(int n1,
				  IrtPtType *PtsSet1,
				  int n2,
				  IrtPtType *PtsSet2,
				  IrtRType AlphaConverge,
				  IrtRType Tolerance,
				  UserRegisterTestConverganceFuncType
				      RegisterTestConvergance,
				  IrtHmgnMatType RegMat);
IrtRType UserRegisterPointSetSrf(int n,
				 IrtPtType *PtsSet,
				 const CagdSrfStruct *Srf,
				 IrtRType AlphaConverge,
				 IrtRType Tolerance,
				 UserRegisterTestConverganceFuncType
				                    RegisterTestConvergance,
				 IrtHmgnMatType RegMat);

/* Bump mapping. */

struct IPObjectStruct *UserDDMPolysOverTrimmedSrf(
					 const struct TrimSrfStruct *TSrf,
					 const struct IPObjectStruct *Texture,
					 IrtRType UDup,
					 IrtRType VDup,
					 int LclUV,
					 int Random);
struct IPObjectStruct *UserDDMPolysOverSrf(
					const CagdSrfStruct *Srf,
					const struct IPObjectStruct *Texture,
					IrtRType UDup,
					IrtRType VDup,
					int LclUV,
					int Random);
struct IPObjectStruct *UserDDMPolysOverPolys(
					 struct IPObjectStruct *PlSrf,
					 const struct IPObjectStruct *Texture,
					 IrtRType UDup,
					 IrtRType VDup,
					 int LclUV);

/* Freeform kernels. */

struct IPObjectStruct *UserSrfKernel(const CagdSrfStruct *Srf,
				     CagdRType SubdivTol,
				     int SkipRate);
struct IPObjectStruct *UserSrfParabolicLines(const CagdSrfStruct *Srf,
					     CagdRType Step,
					     CagdRType SubdivTol,
					     CagdRType NumericTol,
					     int Euclidean,
					     int DecompSrfs);
struct IPObjectStruct *UserSrfParabolicSheets(const CagdSrfStruct *Srf,
					      CagdRType Step,
					      CagdRType SubdivTol,
					      CagdRType NumericTol,
					      CagdRType SheetExtent);

/* Freeform umbilical and curvature analysis. */

struct MvarPtStruct *UserSrfUmbilicalPts(const CagdSrfStruct *Srf,
					 CagdRType SubTol,
					 CagdRType NumTol);
struct IPObjectStruct *UserSrfFixedCurvatureLines(const CagdSrfStruct *Srf,
						  CagdRType k1,
						  CagdRType Step,
						  CagdRType SubdivTol,
						  CagdRType NumericTol,
						  int Euclidean);
struct IPObjectStruct *UserCrvCrvtrByOneCtlPt(const CagdCrvStruct *Crv,
					      int CtlPtIdx,
					      CagdRType Min,
					      CagdRType Max,
					      CagdRType SubdivTol,
					      CagdRType NumerTol,
					      int Operation);

/* Polygonal mesh rounding. */

int User2PolyMeshRoundEdge(struct IPPolygonStruct *Pl1,
			   struct IPPolygonStruct *Pl2,
			   const struct IPPolygonStruct *Edge12,
			   IrtRType RoundRadius,
			   IrtRType RoundPower);

/* Image scaling by bivariate spline surface. */

IrtImgPixelStruct *IrtImgScaleImage(IrtImgPixelStruct *InImage,
				    int InMaxX,
				    int InMaxY,
				    int InAlpha,
				    int OutMaxX,
				    int OutMaxY,
				    int Order);

/* Warping of text using composition with surfaces. */

struct IPObjectStruct *UserWarpTextOnSurface(CagdSrfStruct *Srf,
					     const char *Txt,
					     IrtRType HSpace,
					     IrtRType VBase,
					     IrtRType VTop,
					     IrtRType Ligatures);

/* User inteface to construct piecewise planar cubic Hermite crvs. */

VoidPtr UserHCEditInit(CagdRType StartX, CagdRType StartY, CagdBType Periodic);
VoidPtr UserHCEditFromCurve(const CagdCrvStruct *Crv, CagdRType Tol);
int UserHCEditIsPeriodic(VoidPtr HC);
void UserHCEditSetPeriodic(VoidPtr HC, CagdBType Periodic);
CagdBType UserHCEditGetCtlPtCont(VoidPtr HC, int Index);
void UserHCEditSetCtlPtCont(VoidPtr HC, int Index, CagdBType Cont);
void UserHCEditSetDrawCtlptFunc(VoidPtr HC,
				UserHCEditDrawCtlPtFuncType CtlPtDrawFunc);
void UserHCEditDelete(VoidPtr HC);
VoidPtr UserHCEditCopy(VoidPtr HC);

int UserHCEditTranslate(VoidPtr HC, CagdRType Dx, CagdRType Dy);
int UserHCEditCreateAppendCtlpt(VoidPtr HC,
				CagdRType x,
				CagdRType y,
				int MouseMode);
int UserHCEditCreateDone(VoidPtr HC, CagdRType StartX, CagdRType StartY);
int UserHCEditInsertCtlpt(VoidPtr HC, CagdRType x, CagdRType y, CagdRType t);
int UserHCEditDeleteCtlpt(VoidPtr HC, CagdRType x, CagdRType y);
int UserHCEditUpdateCtl(VoidPtr HC,
			int CtlIndex,
			CagdBType IsPosition,
			CagdRType NewX,
			CagdRType NewY);
int UserHCEditMoveCtl(VoidPtr HC,
		      CagdRType OldX,
		      CagdRType OldY,
		      CagdRType NewX,
		      CagdRType NewY,
		      int MouseMode,
		      CagdRType *MinDist);
int UserHCEditMoveCtlPt(VoidPtr HC, 
			CagdRType OldX,
			CagdRType OldY,
			CagdRType NewX,
			CagdRType NewY,
			int MouseMode);
int UserHCEditMoveCtlTan(VoidPtr HC,
			 CagdRType OldX,
			 CagdRType OldY,
			 CagdRType NewX,
			 CagdRType NewY,
			 int MouseMode);
int UserHCEditIsNearCrv(VoidPtr HC,
			CagdRType x,
			CagdRType y,
			CagdRType *t,
			CagdRType Eps,
			int NormalizeZeroOne);
int UserHCEditIsNearCtlPt(VoidPtr HC,
			  CagdRType *x,
			  CagdRType *y,
			  int *Index,
			  int *UniqueID,
			  CagdRType Eps);
int UserHCEditIsNearCtlTan(VoidPtr HC,
			   CagdRType *x,
			   CagdRType *y,
			   int *Index,
			   int *UniqueID,
			   CagdBType *Forward,
			   CagdRType Eps);
CagdCrvStruct *UserHCEditGetCrvRepresentation(VoidPtr HC, int ArcLen);
int UserHCEditGetCtlPtTan(VoidPtr HC, int Index, CagdPType Pos, CagdPType Tan);
int UserHCEditGetNumCtlPt(VoidPtr HC);
int UserHCEditDrawCtlpts(VoidPtr HC, int DrawTans);
int UserHCEditMatTrans(VoidPtr HC, IrtHmgnMatType Mat);
int UserHCEditTransform(VoidPtr HC, CagdRType *Dir, CagdRType Scl);
int UserHCEditRelativeTranslate(VoidPtr HC, CagdRType *Dir);
int UserHCEditEvalDefTans(VoidPtr HC, int Index);

/* Functions to create machining NC tool path. */

struct IPObjectStruct *UserNCContourToolPath(const struct IPObjectStruct *PObj,
					     IrtRType Offset,
					     IrtRType ZBaseLevel,
					     IrtRType Tolerance,
					     UserNCGCodeUnitType Units);
struct IPObjectStruct *UserNCPocketToolPath(const struct IPObjectStruct *PObj,
					    IrtRType ToolRadius,
					    IrtRType RoughOffset,
					    IrtRType TPathSpace,
					    IrtRType TPathJoin,
					    UserNCGCodeUnitType Units,
					    int TrimSelfInters);

/* Functions related to finite elements' evaluations. */

UserFEKElementStruct *UserFEKBuildMat(CagdSrfStruct *Srf,
				      int IntegRes,
				      IrtRType E,
				      IrtRType Nu,
				      int *Size);
UserFEKElementStruct *UserFEKBuildMat2(CagdPType *Points,
				       int ULength,
				       int VLength,
				       int UOrder,
				       int VOrder,
				       CagdEndConditionType EndCond,
				       int IntegRes,
				       IrtRType E,
				       IrtRType Nu,
				       int *Size);
CagdBType UserFEPointInsideSrf(CagdSrfStruct *Srf, CagdPType Pt);
UserFEInterIntervalStruct *UserFEGetInterInterval(CagdCrvStruct *Crv1,
						  CagdSrfStruct *Srf1,
						  CagdCrvStruct *Crv2,
						  CagdSrfStruct *Srf2);
UserFECElementStruct *UserFEBuildC1Mat(CagdCrvStruct *Crv1,
				       CagdSrfStruct *Srf1,
				       CagdCrvStruct *Crv2,
				       CagdSrfStruct *Srf2,
				       int IntegRes);
UserFECElementStruct *UserFEBuildC1Mat2(CagdPType *Crv1Pts,
					int Crv1Length,
					int Crv1Order,
					CagdPType *Srf1Pts,
					int Srf1ULength,
					int Srf1VLength,
					int Srf1UOrder,
					int Srf1VOrder,
					CagdPType *Crv2Pts,
					int Crv2Length,
					int Crv2Order,
					CagdPType *Srf2Pts,
					int Srf2ULength,
					int Srf2VLength,
					int Srf2UOrder,
					int Srf2VOrder,
					CagdEndConditionType EndCond,
					int IntegRes);
UserFECElementStruct *UserFEBuildC2Mat(CagdCrvStruct *Crv1,
				       CagdSrfStruct *Srf1,
				       CagdCrvStruct *Crv2,
				       CagdSrfStruct *Srf2,
				       int IntegRes);
UserFECElementStruct *UserFEBuildC2Mat2(CagdPType *Crv1Pts,
					int Crv1Length,
					int Crv1Order,
					CagdPType *Srf1Pts,
					int Srf1ULength,
					int Srf1VLength,
					int Srf1UOrder,
					int Srf1VOrder,
					CagdPType *Crv2Pts,
					int Crv2Length,
					int Crv2Order,
					CagdPType *Srf2Pts,
					int Srf2ULength,
					int Srf2VLength,
					int Srf2UOrder,
					int Srf2VOrder,
					CagdEndConditionType EndCond,
					int IntegRes);
IrtRType UserFEEvalRHSC(UserFECElementStruct *C,
			CagdCrvStruct *Crv1,
			CagdCrvStruct *Crv2);

/* Curve arrangment. */

UserCrvArngmntStruct *UserCrvArngmnt(UserCAOpType Operation,
				     const UserCrvArngmntStruct *CA,
				     const void *Params[]);
UserCrvArngmntStruct *UserCrvArngmntCreate(const struct IPObjectStruct *PCrvs,
					   CagdRType EndPtEndPtTol,
					   CagdRType PlanarityTol,
					   int ProjectOnPlane,
					   int InputMaskType);
int UserCAMergeCrvsAtAngularDev(UserCrvArngmntStruct *CA,
				IrtRType AngularDeviation,
				IrtRType PtPtEps);
int UserCABreakLiNCrvsAtAngularDev(UserCrvArngmntStruct *CA,
				   IrtRType AngularDeviation);
int UserCrvArngmntFilterDups(UserCrvArngmntStruct *CA,
			     CagdBType UpdateEndPts,
			     CagdRType EndPtEndPtTol,
			     CagdRType Eps);
int UserCrvArngmntFilterTans(UserCrvArngmntStruct *CA, CagdRType FilterTans);
int UserCrvArngmntSplitAtPts(UserCrvArngmntStruct *CA,
			     const struct IPObjectStruct *PtsObj,
			     CagdRType Eps);
int UserCrvArngmntLinearCrvsFitC1(UserCrvArngmntStruct *CA, int FitSize);
int UserCrvArngmntProcessIntersections(UserCrvArngmntStruct *CA,
				       CagdRType Tolerance);
int UserCrvArngmntProcessSpecialPts(UserCrvArngmntStruct *CA,
				    CagdRType Tolerance,
				    UserCASplitType CrvSplit);
int UserCrvArngmntPrepEval(UserCrvArngmntStruct *CA);
int UserCrvArngmntProcessEndPts(UserCrvArngmntStruct *CA);
int UserCrvArngmntClassifyConnectedRegions(UserCrvArngmntStruct *CA,
					   int IgnoreInteriorHangingCrvs);
CagdCrvStruct *UserCrvArngmntGetCurves(UserCrvArngmntStruct *CA, int XYCurves);
int UserCrvArngmntRegions2Curves(const UserCrvArngmntStruct *CA,
				 int Merge,
				 int XYCurves,
				 IrtRType ZOffset);
int UserCrvArngmntRegionsTopology(const UserCrvArngmntStruct *CA,
				  int XYCurves,
				  IrtRType ZOffset);
int UserCrvArngmntIsContained(const UserCrvArngmntStruct *CA,
			      const CagdCrvStruct *InnerShape,
			      const CagdCrvStruct *OuterLoop);
int UserCrvArngmntIsContained2(const UserCrvArngmntStruct *CA,
			       const CagdPType Pt,
			       const CagdCrvStruct *Loop);
void UserCrvArngmntReport(const UserCrvArngmntStruct *CA,
			  int DumpCurves,
			  int DumpPts,
			  int DumpRegions,
			  int DumpXYData);
int UserCrvArngmntOutput(const UserCrvArngmntStruct *CA,
			 int OutputStyle,
			 CagdRType Tolerance,
			 CagdRType ZOffset);
UserCrvArngmntStruct *UserCrvArngmntCopy(const UserCrvArngmntStruct *CA);
int UserCrvArngmntFree(UserCrvArngmntStruct *CA);

/* 5-axis patch accessibility analysis. */

struct UserPatchAccessInfoStruct *UserPatchAccessPrep(
			CagdSrfStruct **const Srf,
			const struct UserPatchAccessSrfParamsStruct *SrfParams,
			int SrfNum);
void UserPatchAccessFree(struct UserPatchAccessInfoStruct *Patches);
void UserPatchAccessSetDir(struct UserPatchAccessInfoStruct *Patches,
                           const IrtRType *Dir,
                           IrtRType AccessAngle,
                           IrtRType ExtraRadius);
int UserPatchAccessGetNumOfSrf(const struct UserPatchAccessInfoStruct *Patches);
const CagdSrfStruct *UserPatchAccessGetSrfs(
			      const struct UserPatchAccessInfoStruct *Patches,
			      int SrfId);
int UserPatchAccessGetNumOfPatches(
			     const struct UserPatchAccessInfoStruct *Patches);
int UserPatchAccessGetPatchVisible(
			      const struct UserPatchAccessInfoStruct *Patches,
			      int PatchId);
void UserPatchAccessGetPatchData(
			      const struct UserPatchAccessInfoStruct *Patches,
			      int PatchId,
			      struct UserPatchAccessPatchDataStruct *Data);
void UserPatchAccessSetPatchTest(
			      const struct UserPatchAccessInfoStruct *Patches,
			      int PatchId,
			      int SrfId,
			      int Test);

/* 2D curves' Booleans (using curve arrangement). */

struct IPObjectStruct *UserCrvs2DBooleans(const CagdCrvStruct *Crvs1,
					  const CagdCrvStruct *Crvs2,
					  BoolOperType BoolOper,
					  int MergeLoops,
					  int *ResultState);

/* Belt curves creation around circles. */

struct IPObjectStruct *UserBeltCreate(struct IPVertexStruct *Circs,
				      IrtRType BeltThickness,
				      IrtRType BoundingArcs,
				      int ReturnCrvs,
				      int *Intersects,
				      const char **Error);

/* 2D convex domain covering by a random curve.*/

CagdCrvStruct *UserSCvrCoverSrf(const CagdCrvStruct *DomainBndry, 
				CagdCrvStruct *FillCrv, 
				CagdRType CoverEps, 
				CagdRType NumericTol, 
				CagdRType SubdivTol, 
				int TopK, 
				CagdRType TopEps,
				CagdRType IntrpBlndRatio);

/* Packing circles in 2D containers*/

CagdCrvStruct *UserPkPackCircles(CagdCrvStruct *Bndry,
				 CagdRType Radius,
				 int NumIter,
				 CagdRType NumericTol,
				 CagdRType SubdivTol);


/* Ruled surface fitting. */

CagdSrfStruct *UserRuledSrfFit(const CagdSrfStruct *Srf,
			       CagdSrfDirType RulingDir,
			       CagdRType ExtndDmn,
			       int Samples,
			       CagdRType *Error,
			       CagdRType *MaxError);

/* Kinematics simulation using the MV solver. */

typedef struct UserKnmtcsGenInfoStruct *UserKnmtcsGenInfoStructPtr;

int UserKnmtcsSolveMotion(UserKnmtcsGenInfoStructPtr *GI,
			  const UserKnmtcsStruct *System,
			  CagdRType NumTol,
			  CagdRType SubTol,
			  CagdRType Step,
			  int *SolDim,
			  CagdBType FilterSols);
int UserKnmtcsNumOfSolPts(UserKnmtcsGenInfoStructPtr GI, int PolyIdx);
void UserKnmtcsEvalAtParams(UserKnmtcsGenInfoStructPtr GI,
			    int PolyIdx,
			    int PtIdx);
CagdCrvStruct *UserKnmtcsEvalCrvTraces(
		        UserKnmtcsGenInfoStructPtr GI);
void UserKnmtcsFreeSol(UserKnmtcsGenInfoStructPtr GI);
void UserKnmtcsSolveDone(UserKnmtcsGenInfoStructPtr GI);

/* Dexels processing package. */

struct UserDexelDxGridStruct *UserDexelGridNew(int GridType,
					       CagdRType Ori0,
					       CagdRType End0,
					       CagdRType Ori1,
					       CagdRType End1,
					       int NumDx0,
					       int NumDx1);
void UserDexelDxGridCopy(struct UserDexelDxGridStruct *Dest,
			 const struct UserDexelDxGridStruct *Src);
void UserDexelDxGridSubtract(struct UserDexelDxGridStruct *GridA,
			     const struct UserDexelDxGridStruct *GridB);
void UserDexelDxGridUnion(struct UserDexelDxGridStruct *GridA,
			  const struct UserDexelDxGridStruct *GridB);
void UserDexelDxGridFree(struct UserDexelDxGridStruct *DxGrid);
void UserDexelDxClearGrid(struct UserDexelDxGridStruct *DxGrid);
struct UserDexelDxGridStruct *UserDexelGetDexelGridEnvelope0D(
				  CagdPtStruct *EnvlPts,
				  CagdPtStruct *EnvlNrmls,
				  struct UserDexelDxGridStruct *Stock);
struct UserDexelDxGridStruct *UserDexelGetDexelGridEnvelope1D(
				  CagdPolylineStruct *PlaneEnvelope,
				  CagdPolylineStruct *EnvlNrmls,
				  struct UserDexelDxGridStruct *Stock);
void UserDexelInitStock(struct UserDexelDxGridStruct *DxGrid,
			CagdRType Top,
			CagdRType Btm);
void UserDexelInitStockSrf2(struct UserDexelDxGridStruct *DxGrid,
			    CagdSrfStruct *SrfList,
			    CagdRType BtmLevel);
void UserDexelInitStockSrf(struct UserDexelDxGridStruct *DxGrid,
			   const CagdSrfStruct *Srf);

void UserDexelWriteDexelGridToFile(char *FileName, 
				   struct UserDexelDxGridStruct *DxGrid);
struct UserDexelDxGridStruct *UserDexelReadDexelGridFromFile(char *FileName);



IPObjectStruct *UserDexelColorTriangles(IPPolygonStruct *PolyList);
IPPolygonStruct *UserDexelTriangulateDxGrid(struct UserDexelDxGridStruct
					                             *DxGrid);

/* CNC tool sweep computation code. */

typedef struct UserSwpGenInfoStruct *UserSwpGenInfoStructPtr;

UserSwpGenInfoStructPtr UserSweepSectionInit(const IrtRType ToolOrigin[3]);
void UserSwpSecCnstrctToolSph(UserSwpGenInfoStructPtr GI,
			      double Center[3],
			      double Radius);
void UserSwpSecCnstrctToolCyl(UserSwpGenInfoStructPtr GI,
			      double Center[3],
	      		      double Radius,
			      double Height);
void UserSwpSecCnstrctToolCone(UserSwpGenInfoStructPtr GI,
			       double Center[3],
			       double MajorRadius,
			       double MinorRadius,
			       double Height);
void UserSwpSecCnstrctToolGnrl(UserSwpGenInfoStructPtr GI,
			       const CagdCrvStruct *Profile);
void UserSwpSecElimRedundantToolShapes(UserSwpGenInfoStructPtr GI);
int UserSwpSecToolMove(UserSwpGenInfoStructPtr GI,
		       double Position[3],
		       double Orientation[3]);
int UserSwpSecToolCut(UserSwpGenInfoStructPtr GI,
		      double Position[3],
		      double Orientation[3]);
CagdBType UserSwpSecGetPlaneEnvelope(UserSwpGenInfoStructPtr GI,
				     int PlnNrmlDir,
				     double PlnValue,
				     CagdPolylineStruct **PlnEnvl,
				     CagdPolylineStruct **Nrmls);
CagdBType UserSwpSecGetLineEnvelope(UserSwpGenInfoStructPtr GI,
				    int PlnNrmlDir1,
				    double PlnValue1,
				    int PlnNrmlDir2,
				    double PlnValue2,
				    CagdPtStruct **EnvlPts,
				    CagdPtStruct **Nrmls);
IPObjectStruct *UserSwpSecGetSrfEnvelope(UserSwpGenInfoStructPtr GI);
IPPolygonStruct *UserSwpSecMachiningSimulation(const CagdCrvStruct *ToolProfile,
					       const CagdPType ToolOrigin,
					       const IPObjectStruct *MotionData,
					       int DexelGridType,
					       const CagdPType GridOrigin,
					       const CagdPType GridEnd,
					       int NumDexel0,
					       int NumDexel1,
					       const CagdSrfStruct *StockSrf,
					       CagdRType RectStockTopLevel,
					       CagdRType RectStockBtmLevel,
					       const char *OutputSavePath);
struct IPObjectStruct *UserSwpSecRenderTool(UserSwpGenInfoStructPtr GI);
int UserSweepSectionDone(UserSwpGenInfoStructPtr GI);

/* Font processing into curves and geometry. */

struct IPObjectStruct *UserText2OutlineCurves2DInit(const char *FName);
struct IPObjectStruct *UserText2OutlineCurves2D(const char *Str,
						IrtRType Space,
						IrtRType ScaleFactor,
						IrtRType *Height);
#ifdef __WINNT__
/* #define _FREETYPE_FONTS_                 On windows use native windows fonts. */
#define USER_FONT_DEFAULT_WIDTH	-1 

char *UserWChar2Ascii(const UserFontText Str);
UserFontText UserAscii2WChar(const char *Str);
struct IPObjectStruct *UserFontConvertFontToBezier(
			                        const UserFontText Text,
						const UserFontName FontName,
						UserFontStyleType FontStyle,
						IrtRType SpaceWidth,
						int MergeToBsp,
						const char *RootObjName);
#else
/* On other systems try to use the freetype library. */
#define _FREETYPE_FONTS_

struct IPObjectStruct *UserFontFTStringOutline2BezierCurves(
						 const UserFontText Text,
						 const UserFontName FontName,
						 IrtRType Spacing,
						 int MergeToBsp,
						 const char *RootObjName,
						 const char **ErrStr);
#endif /* __WINNT__ */

struct IPPolygonStruct *UserFontBspCrv2Poly(CagdCrvStruct *BspCrv,
					    IrtRType Tolerance);
CagdCrvStruct *UserFontBzrList2BspList(struct IPObjectStruct *BzrListObj,
				       IrtBType *HaveHoles);
struct IPObjectStruct *UserFontBspList2Plgns(struct IPObjectStruct *BspListObj,
					     IrtRType Tol,
					     const char *Name);
struct IPObjectStruct *UserFontBspList2TrimSrfs(
					    struct IPObjectStruct *BspListObj,
					    IrtRType Tol,
					    const char *Name);
struct IPObjectStruct *UserFontBspList2Solids(
					   struct IPObjectStruct *BspListObj,
					   UserFont3DEdgeType ExtStyle, 
					   IrtRType ExtOffset, 
					   IrtRType ExtHeight,
					   IrtRType Tol,
					   CagdBType GenTrimSrfs,
					   const char *Name);
IPObjectStruct *UserFontBspList2SweptTubes(const IPObjectStruct *BspListObj,
					   UserFont3DEdgeType CornerStyle, 
					   const CagdCrvStruct *CrossSection,
					   IrtRType Tol,
					   const char *Name);
int UserFontConvertTextToGeom(const UserFontText Text,
			      const UserFontName FontName,
			      UserFontStyleType FontStyle,
			      IrtRType FontSize,
			      IrtRType TextSpace,
			      UserFont3DEdgeType Text3DEdgeType,
			      const IrtRType Text3DSetup[3],
			      IrtRType Tolerance,
			      UserFontGeomOutputType OutputType,
			      IrtBType CompactOutput,
			      const char *PlacedTextBaseName,
			      struct IPObjectStruct **PlacedTextGeom,
			      char **ErrorSt);
int UserFontLayoutOverShape(const UserFontText Text,
			    const UserFontName FontName,
			    UserFontStyleType FontStyle,
			    IrtRType FontSize,
			    const IrtRType FontSpace[3],
			    IrtRType Tolerance,
			    UserFont3DEdgeType Text3DEdge,
			    const IrtRType Text3DSetup[2],
			    UserFontAlignmentType Alignment,
			    const struct IPPolygonStruct *BoundingPoly,
			    UserFontGeomOutputType OutputType,
			    struct IPObjectStruct **PlacedTextGeom,
			    char **ErrorStr);
int UserFontLayoutOverShape2(const UserFontText Text,
			     const UserFontName FontName,
			     UserFontStyleType FontStyle,
			     IrtRType FontSize,
			     const IrtRType FontSpace[3],
			     IrtRType Tolerance,
			     UserFont3DEdgeType Text3DEdge,
			     const IrtRType Text3DSetup[2],
			     UserFontAlignmentType Alignment,
			     const CagdCrvStruct *BoundingCrv,
			     UserFontGeomOutputType OutputType,
			     struct IPObjectStruct **PlacedTextGeom,
			     char **ErrorStr);
void UserFontLayoutOverShapeFree(struct UserFontWordLayoutStruct *Words);
struct UserFontWordLayoutStruct *UserFontLayoutOverShapeGenWords(
				    const UserFontText Text,
				    const UserFontName FontName,
				    UserFontStyleType FontStyle,
				    IrtRType FontSize,
				    const IrtRType FontSpace[3],
				    IrtRType Tolerance,
				    UserFont3DEdgeType Text3DEdge,
				    const IrtRType Text3DSetup[2],
				    UserFontAlignmentType Alignment,
				    const struct IPPolygonStruct *BoundingPoly,
				    UserFontGeomOutputType OutputType,
				    IrtBType CompactOutput,
				    const char *OutputBaseName,
				    UserFontDimInfoStruct *FontDims,
				    char **ErrorStr);
int UserFontLayoutOverShapePlaceWords(
				   struct UserFontWordLayoutStruct *Words,
				   IrtRType FontSize,
				   const IrtRType FontSpace[3],
				   UserFontAlignmentType Alignment,
				   const UserFontDimInfoStruct *FontDims,
				   const struct IPPolygonStruct *BoundingPoly,
				   struct IPObjectStruct **PlacedTextGeom);

/* Error handling. */

UserSetErrorFuncType UserSetFatalErrorFunc(UserSetErrorFuncType ErrorFunc);
const char *UserDescribeError(UserFatalErrorType ErrorNum);
void UserFatalError(UserFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_USER_LIB_H */
