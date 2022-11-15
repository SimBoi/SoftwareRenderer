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

#include "inc_irit/irit_sm.h"
#include "inc_irit/bool_lib.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/geom_lib.h"
#include "inc_irit/mvar_lib.h"
#include "inc_irit/iritprsr.h"

#ifdef __WINNT__
#include <wchar.h>
typedef wchar_t UserFontChar;
#define USER_FONT_STR_CONST(Str)	L##Str
#define USER_FONT_STR_MALLOC(n)		malloc(sizeof(UserFontChar) * (n) + 2)
#define USER_FONT_STR_FREE(Str)		free(Str)
#define USER_FONT_STR_CPY(DStr, SStr)	wcscpy(DStr, SStr)
#define USER_FONT_STR_NCPY(DStr, SStr, n) wcsncpy(DStr, SStr, n)
#define USER_FONT_STR_CAT(DStr, SStr)	wcscat(DStr, SStr)
#define USER_FONT_STR_CMP(DStr, SStr)	wcscmp(DStr, SStr)
#define USER_FONT_STR_DUP(Str)		_wcsdup(Str)
#define USER_FONT_STR_CHR(Str, Chr)	wcschr(Str, L##Chr)
#define USER_FONT_STR_LEN(Str)		wcslen(Str)
#define USER_FONT_IS_SPACE(c)		iswspace(c)
#define USER_FONT_TEXT2INT(Str)		_wtoi(Str)
#define USER_FONT_GET_WORD_ASCII(Str)	IritWChar2Ascii(Str)
#define USER_FONT_GET_WORD_UNICODE(Str) IritAscii2WChar(Str)
#else
#include <string.h>
typedef char UserFontChar;
#define USER_FONT_STR_CONST(Str)	Str
#define USER_FONT_STR_MALLOC(n)		malloc(sizeof(UserFontChar) * (n) + 1)
#define USER_FONT_STR_FREE(Str)		free(Str)
#define USER_FONT_STR_CPY(DStr, SStr)	strcpy(DStr, SStr)
#define USER_FONT_STR_NCPY(DStr, SStr, n) strncpy(DStr, SStr, n)
#define USER_FONT_STR_CAT(DStr, SStr)	strcat(DStr, SStr)
#define USER_FONT_STR_CMP(DStr, SStr)	strcmp(DStr, SStr)
#define USER_FONT_STR_DUP(Str)		strdup(Str)
#define USER_FONT_STR_CHR(Str, Chr)	strchr(Str, Chr)
#define USER_FONT_STR_LEN(Str)		strlen(Str)
#define USER_FONT_IS_SPACE(c)		isspace(c)
#define USER_FONT_TEXT2INT(Str)		atoi(Str)
#define USER_FONT_GET_WORD_ASCII(Str)	strdup(Str)
#define USER_FONT_GET_WORD_UNICODE(Str) strdup(Str)
#endif /* __WINNT__ */

typedef UserFontChar *UserFontText;
typedef char *UserFontName;

#define USER_HC_VEC_DRAW_SCALE	0.25

#define USER_PACK_TILE_MAX_SPACE_DIM	3

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

/* Tile membership. */
#define USER_MICROJOIN_TILE_UNDEFINED 0
#define USER_MICROJOIN_TILE_INSIDE 1
#define USER_MICROJOIN_TILE_ONBNDRY 2
#define USER_MICROJOIN_TILE_OUTSIDE 3

#define USER_NON_TRIMMED_SRF_ATTR_ID	IRIT_ATTR_CREATE_ID(NON_TRIMMED_SRF)

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
} UserMicro2TileTopologyType;
#define USER_MICRO2_TILE_MAX_BIFURCATION_TOPOLOGY 4

#define USER_PACK_TRANS_VEC1_ATTR_NAME	    IRIT_ATTR_CREATE_ID(vec1)
#define USER_PACK_TRANS_VEC2_ATTR_NAME	    IRIT_ATTR_CREATE_ID(vec2)
#define USER_PACK_TRANS_VEC3_ATTR_NAME	    IRIT_ATTR_CREATE_ID(vec3)
#define USER_PACK_TRANS_USTEP_ATTR_NAME	    IRIT_ATTR_CREATE_ID(USteps)
#define USER_PACK_TRANS_VSTEP_ATTR_NAME	    IRIT_ATTR_CREATE_ID(VSteps)
#define USER_PACK_TILE_POS_ATTR_NAME	    IRIT_ATTR_CREATE_ID(TileBndryType)

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

typedef struct UserMicroTileBndryPrmStruct {           /* Static tile info. */
    IrtRType OuterRadius;                       /* Outer dimension of tube. */
    IrtRType InnerRadius;     /* Inner dimension of tube.  Zero to disable. */
    IrtRType YScale; /* Optional scale of cross section in higher dimension */
       /* of face, for elliptic/rectangular cross sections. Zero to ignore. */
    CagdBType Circular;/* TRUE for circular cross section, FALSE for square.*/
    CagdRType Bndry[4];  /* >0 for all four corners in lexicographic order, */
            /* if on boundary deform. TV, to create skin of that thickness. */
    CagdRType BndryShape;  /* Between zero (flat & smooth) and one (bulky). */
    CagdPType FaceCenter;             /* [0, 1]^3 to shift center location, */
} UserMicroTileBndryPrmStruct;

typedef struct UserMicroParamTileBndryPrmStruct {  /* Parametric tile info. */
    MiscExprTreeGenInfoStructPtr E2TGI;    /* Handler of expr. tree module. */
    IritE2TExprNodeStruct *OuterRadius;         /* Outer dimension of tube. */
    IritE2TExprNodeStruct *InnerRadius;/* Inner dimension of tube.  Zero to */
    /* disable (no hole).						    */
    IritE2TExprNodeStruct *Circular;/* > 0.0 for circular cross section,    */
    /* <0.0 square.							    */
    IritE2TExprNodeStruct *BndryShape;/* Between zero (flat & smooth) & one */
    /* (bulky).								    */
    /* These designates if TV is in UVW space (TRUE) or Euclidean (FALSE):  */
    UserMicroTileBndryPrmStruct StaticTileParam;
} UserMicroParamTileBndryPrmStruct;

typedef struct UserMicroRegularParamStruct {
    UserMicroTileStruct *Tile;    /* The tile to be used in regular tiling. */
    CagdBType FFDeformApprox;  /* If TRUE, do approximated Freeform deform. */
    CagdBType TilingStepMode;   /* Repeat tiling if TRUE, tiling Repeat[i]  */
    /* tiles, either globally or per Bezier domain. If FALSE, each tile is  */
    /* displace TilingSteps[i] amount, each dir (which can be overlapping). */
    CagdRType *TilingSteps[CAGD_MAX_PT_SIZE]; /* In each dimension:         */
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
    UserMicroParamTileBndryPrmStruct ParamTilePrm; /* If we have parametric */
    int HasParamTilePrm; /* tiles, parameter functions will be stored here. */
    IPAttrIDType *_AttrsToPropagate; /* An array of attribute Ids to        */
    /* propagate through the tiling process. Must end with                  */
    /* IRIT_ATTR_CREATE_ID(___INVALID_ATTR). Can be NULL.                   */
    int _DisableParallelRun;			      /* Internal use only. */
    UserMicroTileStruct *_BndryTiles[3][3][3];	      /* Internal use only. */
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
     int SubdDir;
     int FFDeformApprox;
     CagdRType SubdThreshold;
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
    union {
	UserMicroRegularParamStruct RegularParam;
	UserMicroRandomParamStruct RandomParam;
	UserMicroFunctionalParamStruct FunctionalParam;
	UserMicroRandomBiFurcationParamStruct RandomBiFurcationParam;
	UserMicroRegularBiFurcationParamStruct RegularBiFurcationParam;
    } U;
} UserMicroParamStruct;

typedef struct UserMicroAuxeticTileBndryPrmStruct {
    int HasOutJoint;          /* TRUE to also have a joint to the neighbor. */
    CagdRType InnerJointDiameter;             /* Positive - joint diameter. */
    CagdRType PossionRatio;     /* For this side boundaries. Ignored for Z. */
    CagdRType _Z;	   /* Used internally to set Z level of this joint. */
    int _FaceIdx; /* Used internally to set face (0 to 5 for UMin to WMax). */
} UserMicroAuxeticTileBndryPrmStruct;

/* Used by the C^0 discont tiles to hold the respective refinement matrix. */
#define USER_ATTR_MICRO_SPLIT_TILE_UREF_MAT  IRIT_ATTR_CREATE_ID(UKnotRefineMatrix)
#define USER_ATTR_MICRO_SPLIT_TILE_VREF_MAT  IRIT_ATTR_CREATE_ID(VKnotRefineMatrix)

/* Data structure to store micro tiles of a tensor product trivariate	    */
/* volume and the macro shape of the micro tile set.	 		    */
typedef struct UserMJTileStrctStruct {
    struct UserMJTileStrctStruct *Pnext;
    /* A list of the geometries of the micro tiles.			    */
    IPObjectStruct *TileGeoms;
    /* A pointer to the deformation TV map to represent the macro shape.    */
    const TrivTVStruct *BaseTVRef;
    /* Whether the deformation map is geometrically periodic or not.	    */
    CagdBType UVWPeriodic[3];
    /* Priority of TV in the construction of joint tiles.		    */
    IrtRType TVMapPriority;
} UserMJTileStrctStruct;

/* Joint surface that connects one tile to the other tile.   */
typedef struct UserMJTileJointSrfStruct {
    struct UserMJTileJointSrfStruct *Pnext;
    /* Joint surface that is one of the bounding surface of a micro tile.    */
    IPObjectStruct *JointSrf; 
    /* A reference pointer to TV where the joint surface comes from. Can be  */
    /* the entire tile if the tile is composed of a single TV. Otherwise,    */
    /* pointer to one TV among a list of TVs of the tile.		     */
    const TrivTVStruct *JointTVRef;
    /* The boundary direction of the joint surface in joint TV. 0 ~ 5 if the */
    /* joint surface is the UMin / UMax / VMin / VMax / WMin / WMax boundary */
    /* of the joint TV.							     */
    int JointSrfType; 
    /* A reference pointer to the tile where the joint surface comes from.   */
    const IPObjectStruct *JointTileRef;
} UserMJTileJointSrfStruct;

typedef struct UserMJTileJointSrfRefStruct {
    struct UserMJTileJointSrfRefStruct *Pnext;
    UserMJTileJointSrfStruct *Obj;
} UserMJTileJointSrfRefStruct;

/* The trimmed surface that interfaces two sets of the tile joint surfaces. */
/* The associated tile joint surfaces are also stored here.		    */
typedef struct UserMJJoinInterSrfStruct {
    struct UserMJJoinInterSrfStruct *Pnext;
    /* The trimmed surface that separates two micro tile sets.		    */
    TrimSrfStruct *InterSrf;
    /* Auxilliary cache used in the acceleration of the surface distance    */
    /* computation.							    */
    void *InterSrfDistPrep;
    /* Opposite volume element which this join surface interfaces. NULL in  */
    /* case of the outmost boundary surface.				    */
    const VMdlVolumeElementStruct *OppoVolElem;
    /* Whether the joint tile is already created or not. */
    IrtBType TileCreated;
    /* A list of joint surfaces on this interface surface.		    */
    UserMJTileJointSrfStruct *TouchingSrfs;
    /* A list of joint surfaces in the interior of the trimmed volume.	    */
    UserMJTileJointSrfStruct *InteriorSrfs;
} UserMJJoinInterSrfStruct;

typedef struct UserMJJoinInterSrfRefStruct {
    struct UserMJJoinInterSrfRefStruct *Pnext;
    UserMJJoinInterSrfStruct *Obj;
} UserMJJoinInterSrfRefStruct;

/* Geometry and auxiliary information of an anchor, which connects one tile */
/* to another tile or the boundary of the macro object.			    */
typedef struct UserMJAnchorGeomStruct {
    struct UserMJAnchorGeomStruct *Pnext;
    /* Anchor geometry. Either a surface or a point.			    */	    
    IPObjectStruct *AnchorGeom; 
    /* A start direction of an axis of a bridging tile that is constructed  */
    /* from this anchor.						    */
    CagdVType AnchorDir; 
    /* A pointer to the tile where this anchor originates from.		    */
    const IPObjectStruct *AnchorTileRef;
    /* If a tile is of trivariate type, a pointer to the tile trivariate    */
    /* where this anchor originates from.				    */
    const TrivTVStruct *AnchorTVRef;
    /* The boundary direction of this anchor surface in AnchorTVRef. (valid */
    /* only in case of trivariate types. 0 ~5 if the anchor is UMin / UMax /*/
    /* VMin / VMax / WMin / WMax boundary of AnchorTVRef.		    */
    int AnchorBndType;
    /* A pointer to the boundary geometry which this anchor connects to.    */
    /* Valid only when the anchor connects to the macro boundary object.    */
    const IPObjectStruct *MacroGeom;
    /* Id of the macro object when MacroGeom is a list or a polygon.	    */
    int MacroGeomId;
} UserMJAnchorGeomStruct;

/* Packing spheres and circles in 3D and 2D containers - data structures */

typedef struct UserSpkRandomPoolStruct *UserSpkRandomPoolStructPtr;

/* A struct-wrapper for a 3D vector to allow dense vector arrays */
typedef struct UserSpkVectorStruct {
    IrtVecType V;
} UserSpkVectorStruct;

/* A border, including all its method implementations */
typedef struct UserSpkBorderFieldsStruct {
    void *Instance;
    IrtBType Is3D;
    IrtBType IsThreadSafe;
    void(*GetBoundingBox)(void *Instance, GMBBBboxStruct *BoundingBox);
    IrtBType(*PointIsInside)(void *Instance, const IrtVecType Point);
    void(*ClosestPoint)(void *Instance,
			const IrtVecType Point,
			IrtVecType ClosestPoint,
			IrtBType *WasInside);
    void(*ClosestPointInside)(void *Instance,
			      const IrtVecType Point,
			      IrtVecType ClosestPoint);
} UserSpkBorderFieldsStruct;

typedef struct UserSpkBorderFieldsStruct *UserSpkBorderStructPtr;

/* Different algorithms of sphere packing */
typedef enum UserSpkAlgorithmType {
    USER_SPK_ALG_HONEYCOMB,
    USER_SPK_ALG_REPULSION,
    USER_SPK_ALG_REPULSION_PARALLEL,
    USER_SPK_ALG_GRAVITY,
    USER_SPK_ALG_GRAVITY_LOCALLY_DISTRIBUTED,
} UserSpkAlgorithmType;

typedef struct UserSpkSolvingSettingsStruct {                  /* Settings. */
    IrtRType SphereRadius;				  /* Sphere radius. */
    IrtRType PenetrationTolerance;   /* Penetration depth, below which two  */
			  /* spheres are still considered non-intersecting. */
    IrtRType ConnectivityTolerance;    /* Distance, below which spheres are */
                                             /* still considered connected. */
    int MaxIterationsPerAttempt;     /* Maximum allowed number of repulsion */
					         /* iterations per attempt. */
    IrtRType MinCostDecrease;     /* Minimal cost decrease, below which the */
			  /* cost is considered to have stopped decreasing. */
    int CostDecreaseGracePeriod;     /* Number of iterations after cost has */
			 /* stop decreasing until the attempt is abandoned. */
    int StartSphereCount;  /* Sphere count to start the first attempt with. */
              /* Ignored if less than honeycomb configuration sphere count. */
    int RandomNumberPoolCapacity; /* Capacity of the random number pool.    */
    IrtBType SingleThreadedRepulsion; /* Prohibits parallelization          */
                                      /* of repulsion.                      */
} UserSpkSolvingSettingsStruct;

typedef struct UserSpkSolvingProcessFieldsStruct *UserSpkSolvingProcessStructPtr;

/* A pair of connected spheres, represented as indices in the               */
/* configuration's sphere array.					    */
typedef struct UserSpkConnectivityPairStruct {
    int SphereIndices[2];
} UserSpkConnectivityPairStruct;

typedef struct UserSpkSphereConfigurationStruct {/* A sphere configuration. */
    int NumSpheres;
    UserSpkVectorStruct *SphereCenters;
    int NumConnectivityPairs;
    UserSpkConnectivityPairStruct *ConnectivityPairs;
} UserSpkSphereConfigurationStruct;

typedef struct UserSpkSolvingStatusStruct { /* Solving status & statistics. */
    UserSpkSphereConfigurationStruct LatestSuccess;    /* Latest successful */
							  /* configuration. */
    int AttemptsSinceLatestSuccess;
    double SecondsSinceLatestSuccess;
    int AttemptsForLatestSuccess;
    double SecondsForLatestSuccess;
    int AttemptsTotal;
    double SecondsTotal;
} UserSpkSolvingStatusStruct;

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
    USER_ERR_EXPCT_CRV_OBJ,
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
    USER_ERR_TRIVAR_UNSUPPORTED,
    USER_ERR_EXPECTED_POLY_OR_ONE_SRF,
    USER_ERR_NON_UNIFORM_BEAMS_UNSUPPORTED,
    USER_ERR_XRANGE_EMPTY,
    USER_ERR_YRANGE_EMPTY,
    USER_ERR_ZRANGE_EMPTY,

    USER_ERR_NC_INVALID_PARAM,
    USER_ERR_NC_INVALID_INTER,
    USER_ERR_NC_NO_POLYLINES,
    USER_ERR_EXPCT_C1_GEOM,
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

typedef enum {
    USER_TILE_SEM_REG_UNIVARIATE,
    USER_TILE_SEM_REG_BIVARIATE,
    USER_TILE_SEM_REG_TRIVARIATE
} UserTileSemiRegType;

typedef enum {
    USER_TILE_SEM_REG_UNIVARIATE_CLOSED,
    USER_TILE_SEM_REG_UNIVARIATE_OPEN,
    USER_TILE_SEM_REG_BIVARIATE_SURF,
    USER_TILE_SEM_REG_TRVARIATE_EXTRUDE
} UserTileSemiRegDualType;

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

typedef struct UserLineAccessParamsStruct {
    int SrfToProcess;
    int PatchToProcess;
    CagdRType PatchAdjacencyTolerance;
    CagdRType TangentIntersectionTolerance;
    CagdBType CountOnly;
    int NumDirectionFrustums;
    UserPatchAccessSrfParamsStruct SubdivParams;
} UserLineAccessParamsStruct;

typedef struct UserLineAccessSrfLineAccessResStruct {
    struct UserLineAccessSrfLineAccessResStruct *Pnext;
    IPAttributeStruct *Attr;
    const CagdSrfStruct **OrigSrfs;
    int NumSrfs;
    int NumPatches;
    struct UserLineAccessPatchLineAccessStruct *PatchesAccessibility;
} UserLineAccessSrfLineAccessResStruct;

/* Data structures used by the AM fiber 3-axis printing. */
typedef struct UserAMFiber3AxisTValListStruct {
    struct UserAMFiber3AxisTValListStruct *Pnext;
    IrtRType TVal;
} UserAMFiber3AxisTValListStruct;

typedef struct UserAMFiber3AxisFragStruct {
    const CagdCrvStruct *Crv;
    int Num;
    IrtRType *TVals;
} UserAMFiber3AxisFragStruct;

typedef struct UserAMFiber3AxisCrvOrderStruct {
    CagdCrvStruct *Crv;
    IrtRType Val;
} UserAMFiber3AxisCrvOrderStruct;

/* Gear design via conjugation using 2D swept volumes - data structures */
typedef enum {
    USER_GEAR2D_SWEEP_CONJUGATE = 1,
    USER_GEAR2D_SWEEP_ARRANGE_TEETH = 2,
    USER_GEAR2D_SWEEP_CENTRODE = 3,
    USER_GEAR2D_SWEEP_UNIFORM_MOTION = 4,
    USER_GEAR2D_SWEEP_NON_UNIFORM_MOTION = 5,
    USER_GEAR2D_SWEEP_OBLONG_MOTION = 6,
    USER_GEAR2D_SWEEP_INVERSE_MOTION = 7,
} UserGr2DSwpFunctionType;

typedef struct UserGr2DSwpParamStruct {
    UserGr2DSwpFunctionType FunctionSelect;	/* Type of functionality.   */
    IPObjectStruct *Crv1;   /* Input gear to conjugate, or overall shape of */
       /* gear to arrange teeth on, or centroid (return argument), or       */
       /* non-uniform circular displacement profile for non-uniform motion. */
    IPObjectStruct *Crv2;      /* Conjugate gear(return argument) or tooth  */
                               /* shape to arrange around an overall shape. */
    IPObjectStruct *CrvSrf; /* Gear obtained after arranging teeth (return  */
          /* argument), or planar surface within which to compute centroid. */
    MvarMVStruct *Rot[2];               /* Rotational part of rigid motion. */
    MvarMVStruct *Trans;             /* Translational part of rigid motion. */
    MvarMVStruct *RotInv[2];	/* Rotational part of inverse motion        */
                                /*(return argument).			    */
    MvarMVStruct *TransInv;	/* Translational part of inverse motion     */
			        /* (return argument).			    */ 
    CagdRType SolverStepSize;	/* Step-size for univariate solver.	    */
    CagdRType ToothLen;	        /* Arc-length of the base of tooth.	    */
    CagdRType GearDist;	          /* Distance between centers of two gears. */
    int NumTeeth;                    /* Number of teeth in the source gear. */
    int NumTeethCirc;         /* Number of teeth in circular portion of the */
			      /* oblong gear.				    */
    int NumTeethLin;	        /* Number of teeth in linear portion of the */
				/* oblong gear.				    */
    CagdBType InverseMotion;   /* TRUE if the inverse motion is desired     */
                               /* while generating motion for oblong gears. */
} UserGr2DSwpParamStruct;

/* Truss construction. */

typedef enum UserTrussOutputType {
    USER_TRUSS_OUTPUT_TRIM_SRFS,
    USER_TRUSS_OUTPUT_TRIVARS,
    USER_TRUSS_OUTPUT_MDLS,
} UserTrussOutputType;

typedef enum {
    USER_CRVNETWORK_OUT_CRV,
    USER_CRVNETWORK_OUT_SRF,
    USER_CRVNETWORK_OUT_TRIV
} UserCrvNetworkOutType;

typedef const struct UserTrussNodeDefStruct *UserTrussNodeDefPtr;

typedef void *(*UserTrussBeamInfoPrepFuncType)(UserTrussNodeDefPtr,
					       int NumNodes,
					       const CagdSrfStruct *ShellSrf,
					       void *ExtraData);
typedef void (*UserTrussBeamInfoFuncType)(const UserTrussNodeDefPtr,
					  int NumNodes,
					  void *PrepData,
					  int CurrNode,
					  int CurrBeam,
					  const CagdSrfStruct *ShellSrf,
					  const CagdPType ShellPt,
					  CagdRType ShellDist,
					  CagdRType *BeamR,
					  CagdRType *FilletR, 
					  CagdRType *FilletH,
					  CagdRType *ShellFilletR,
					  CagdRType *ShellFilletH,
					  CagdRType *SphereR);
typedef void (*UserTrussBeamInfoCleanFuncType)(const UserTrussNodeDefPtr,
					       int NumNodes,
					       void *PrepData);

typedef struct UserTrussTolerancesStruct {
    /* The tolerance for considering two end points the same in the curve   */
    /* arrangement (in the trimming of the beams and spheres).              */
    CagdRType CrvArngmntTol;
    /* The tolerance of intersection locations' computation in the curve    */
    /* arrangement (in the trimming of the beams and spheres).              */
    CagdRType CrvArngmntInterTol;
    /* The tolerance determining if points are non co-linear, and therefore */
    /* define a single plane (in trivariate truss lattice construction).    */
    CagdRType BeamBisectPlnTol;
    /* The tolerance for determining if points are non co-linear, and       */
    /* therefore define a plane (in trivariate truss lattice construction). */
    CagdRType BeamBisectDbgTol;
    /* The tolerance for verifying that the bisectors between beams are     */
    /* indeed co-planar (used in DEBUG mode only).                          */
    CagdRType CloseLoopTestTol;
    /* The offset by which to extend the intersection loops outside the UV  */
    /* domain of the surfaces. This is done to increase the robustness of   */
    /* the curve arrangement computation.                                   */
    CagdRType CloseLoopOffset;
    /* Tolerance for merging intersection curve segments in which the end   */
    /* ponits are close to each other. Should be greater than CrvArngmntTol.*/
    CagdRType CrvMergeEps;
    /* The subdivision tolerance for computing the intersection curves.     */
    CagdRType ContourSubdivTol;
    /* The numeric tolerance for computing the intersection curves.	    */
    CagdRType ContourNumTol;
    /* The tracing tolerance for computing the intersection curves.	    */
    CagdRType ContourTraceTol;
    /* The tolerance for filtering out very short intersection curves.      */
    /* Should be greater than CrvMergeEps.                                  */
    CagdRType ContourFilterTol;
    /* The tolerance for filtering out very short trimming loops. Used in   */
    /* post-processing truss lattices for conversion to model.              */
    CagdRType MdlSmallLoopTol;
    /* The subdivision tolerance for back-projecting subdivision markers on */
    /* surfaces. Used in post-processing truss lattices for conversion to   */
    /* models.                                                              */
    CagdRType MdlBackProjSrfSubdivTol;
    /* The numeric tolerance for back-projecting subdivision markers on     */
    /* surfaces. Used in post-processing truss lattices for conversion to   */
    /* models.                                                              */
    CagdRType MdlBackProjSrfNumTol;
    /* The numeric tolerance for back-projecting subdivision markers on     */
    /* curves. Used in post-processing truss lattices for conversion to     */
    /* models.                                                              */
    CagdRType MdlBackProjCrvNumTol;
    /* The tolerance for verifying that the back-projection of subdivision  */
    /* markers is close to its requires position. Used in post-processing   */
    /* truss lattices for conversion to models.                             */
    CagdRType MdlBackProjTestTol;
    /* The tolerance for ignoring subdivision markers that are very close   */
    /* to the end of a trimming curve segment. Used in post-processing      */
    /* truss lattices for conversion to models.                             */
    CagdRType MdlBackProjCrvEndTol;
    /* The tolerance for snapping subdivision markers to the UV-domain      */
    /* boundary of a surface. Used in post-processing truss lattices for    */
    /* conversion to models.                                                */
    CagdRType MdlBackProjBndryTol;
    /* The tolerance for determining whether a node point is outside the    */
    /* shell surface.                                                       */
    CagdRType PtOutsideSrfTol;
    /* The subdivision tolerance for computing the distance between a node  */
    /* point and the shell (if not provided).                               */
    CagdRType ShellPtDistSubdivTol;
    /* The numeric tolerance for computing the distance between a node      */
    /* point and the shell (if not provided).                               */
    CagdRType ShellPtDistNumTol;
} UserTrussTolerancesStruct;

typedef struct UserTrussNodeDefCallbacksStruct {
    UserTrussBeamInfoPrepFuncType PrepCallback;
    UserTrussBeamInfoFuncType SetDataCallback;
    UserTrussBeamInfoCleanFuncType CleanupCallback;
    void *ExtraData;
} UserTrussNodeDefCallbacksStruct;

/* Struct for constant beam width trusses: */
typedef struct UserTrussConstBeamWidthStruct {
    CagdRType SphereR, BeamR, FilletR, FilletH;
} UserTrussConstBeamWidthStruct;

/* Struct for graded beam width trusses: */
typedef struct UserTrussGradedBeamWidthStruct {
    CagdVType GradientVec;
    CagdRType SphereR, BeamRStartEnd[2],
	      FilletRStartEnd[2], FilletHStartEnd[2], GMinMax[2];
} UserTrussGradedBeamWidthStruct;

typedef struct UserTrussSpherePackParamsStruct {
    CagdRType SphereRadius;
    CagdRType SpherePackTime;
    CagdRType SubdivTol;
    CagdRType NumericTol;
    UserSpkAlgorithmType SpherePackAlg;
} UserTrussSpherePackParamsStruct;

typedef struct UserTrussLatticeParamsStruct {
    CagdRType ConnectDist;
    UserTrussNodeDefCallbacksStruct *Callbacks;
    CagdRType SphereRadius;
    CagdRType BeamRadius;
    CagdRType FilletRadius;
    CagdRType FilletHeight;
    const IPObjectStruct *ShellObj;
    CagdBType ShellPruneOnly;
    UserTrussOutputType OutputType;
} UserTrussLatticeParamsStruct;

/* Dither by curves structs. */

#define USER_DBC2_MAX_DIM		3
#define USER_DBC2_MAX_IMG_SIZE		1000

struct UserDitherImagesByCurvesDBStruct;

typedef struct UserDitherImagesByCurvesInfoStruct {
    int Dim, DiffuseFSError, MergeCurves, DumpPolys, Rounded;     /* State. */
    CagdRType NoiseLevel, SweepRadius;

    /* Info on input images. */
    IrtImgPixelStruct *Imgs[USER_DBC2_MAX_DIM];
    int ImgMaxX, ImgMaxY;

    /* Auxiliary structure for error diffusion. */
    IrtRType
	_ErrorDiffusion[USER_DBC2_MAX_IMG_SIZE][USER_DBC2_MAX_IMG_SIZE]
		       [USER_DBC2_MAX_DIM];
} UserDitherImagesByCurvesInfoStruct;

/****************************************************************************/

typedef void (*UserSetErrorFuncType)(UserFatalErrorType ErrorFunc);
typedef int (*UserRegisterTestConverganceFuncType)(IrtRType CrntDist, int i);
typedef int (*UserCntrIsValidCntrPtFuncType)(const CagdSrfStruct *Srf,
					     CagdRType U,
					     CagdRType V);
typedef void (*UserHCEditDrawCtlPtFuncType)(int PtIndex,
					    int PtUniqueID,
					    IrtRType *Pos,
					    IrtRType *TanBack,
					    IrtRType *TanForward,
					    void *FuncData);

/* Rocket fuel design call back functions. */
typedef CagdRType (*UserRocketFuelThrustProfileFuncType)(void *CBData,
						         CagdRType t);
typedef CagdRType (*UserRocketFuelEvalLclFuelThrustFuncType)(
						   void *CBData,
						   CagdSrfStruct *LclElement,
						   CagdRType LclDepth,
						   CagdSrfStruct *GlblFront,
						   const int Indices[],
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
				  const UserMicroTileBndryPrmStruct *WMaxPrms,
				  char ** const Error);
IPObjectStruct *UserMicroBifurcate1to2Tile(
				 const UserMicroTileBndryPrmStruct *WMinPrms,
				 const UserMicroTileBndryPrmStruct *WMax1Prms,
				 const UserMicroTileBndryPrmStruct *WMax2Prms,
				 CagdRType SeparationGap,
				 CagdRType SaddleSize,
				 CagdBType Trivars,
				 char ** const Error);
IPObjectStruct *UserMicroZSpringTile(int SpringOrder,
				     const CagdRType *SpringParams,
				     const CagdRType *BotTopCrossWidth,
				     CagdRType BotTopCrossHeight);
IPObjectStruct *UserMicroDiagTile1(CagdRType CntrSize,
				   CagdRType CrnrSizes[8],
				   CagdRType SmoothFactor);
IPObjectStruct *UserMicroAuxeticTile(
			       UserMicroAuxeticTileBndryPrmStruct *XMinParams,
			       UserMicroAuxeticTileBndryPrmStruct *XMaxParams,
			       UserMicroAuxeticTileBndryPrmStruct *YMinParams,
			       UserMicroAuxeticTileBndryPrmStruct *YMaxParams,
			       UserMicroAuxeticTileBndryPrmStruct *ZMinParams,
			       UserMicroAuxeticTileBndryPrmStruct *ZMaxParams,
			       int CircularBars,
			       CagdRType FlexClipRatio,
			       char ** const Error);
IPObjectStruct *UserMicroAuxFrameTile(
			 const UserMicroAuxeticTileBndryPrmStruct *FaceParams,
			 int CircularBars,
			 char ** const Error);
TrivTVStruct *UserMicroAuxClipFlexEnds(TrivTVStruct **TVs,
				       CagdRType FlexClipRatio,
				       char ** const Error);
IPObjectStruct *UserMicroBiStableTile2D(CagdRType FrameSize,
					CagdRType FrameHGap,
					CagdRType WeightRodsThickness,
					CagdRType WeightRodsSpring,
					CagdRType WeightRodsAngle,
					CagdRType FlexClipRatio,
				        const CagdRType CenterWeightSizes[3],
					char ** const Error);
IPObjectStruct *UserMicroBiStableTile3D(CagdRType FrameSize,
					CagdRType WeightRodsThickness,
					CagdRType WeightRodsSpring,
					CagdRType WeightRodsAngle,
					CagdRType FlexClipRatio,
					const CagdRType CenterWeightSizes[3],
					char ** const Error);
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
					    const IPObjectStruct *DeformMVs);
void UserMicroStructParamFree(UserMicroParamStruct *MSParam, int FreeSelf);

/* Micro-structures joining in complex (VModel) objects. */

UserMJTileStrctStruct *UserMJTileStrctNew(const TrivTVStruct *BaseTVMap,
					  const IPObjectStruct *MicroTiles);
UserMJTileStrctStruct *UserMJTileStrctNew2(const TrivTVStruct *BaseMap,
					   const IPObjectStruct *MicroTiles);
void UserMJTileStrctFree(UserMJTileStrctStruct *MJT);
void UserMJTileStrctListFree(UserMJTileStrctStruct *MJT);
void UserMJTileJointSrfFree(UserMJTileJointSrfStruct *TileJSrf);
void UserMJJoinInterSrfFree(UserMJJoinInterSrfStruct *JISrf);
void UserMJJoinInterSrfListFree(UserMJJoinInterSrfStruct *JISrfList);
void UserMJAnchorFree(UserMJAnchorGeomStruct *Anchor);
void UserMJAnchorFreeList(UserMJAnchorGeomStruct *Anchors);

void UserMJVMdlBooleanCallBack(const VMdlParamsStruct *Params,
			       const VMdlVModelStruct *VMdl1,
			       const VMdlVModelStruct *VMdl2,
			       VMdlVModelStruct *ResVMdl,
			       VMdlBoolOpType BoolOp,
			       void *AuxData);
IPObjectStruct *UserMJMicroJoinSynthesize(const VMdlVModelStruct *VMdlOrig,
					  CagdRType NrmScale,
					  CagdRType NrmBlendingRatio,
					  CagdRType SaddleRatio,
					  CagdRType BndTileMargin,
					  CagdBType CheckJacobian);

void UserMJClassifyFieldElmnts(const MdlModelStruct *MacroMdl,
	    		       const IPObjectStruct *Elments,
			       IPAttrIDType SubPartAttrID,
			       IPObjectStruct **InElmnts, 
	    		       IPObjectStruct **OutElmnts,
			       IPObjectStruct **InterElmnts);
IPObjectStruct *UserMJMicroFieldTiles(const IPObjectStruct *Tiles,
			    	      const IPObjectStruct *TileField,
				      const IPObjectStruct *MacroGeomObj,
				      CagdRType AxisNrmRatio,
				      CagdRType AxisNrmBlendingRatio,
				      CagdRType LastSrfScale);

/* Tile packing in some domain module: */
struct UserTilePackInfoStruct *UserPackTileCreateTileObject(
						const IPObjectStruct *TileObj,
						const int *StepsMin,
						const int *StepsMax,
						int Dim);
void UserPackTileFreeTileObject(struct UserTilePackInfoStruct *Tile);
IPObjectStruct *UserPackTilesInDomain(
				   struct UserTilePackInfoStruct *PackTileInfo,
				   const GMBBBboxStruct *TilingDomain,
				   int TileInclusion,
				   CagdRType SkewFactor);
void UserTileGetSteps(const struct UserTilePackInfoStruct *PackTileInfo,
		      int *StepsMin, 
		      int *StepsMax,
		      CagdRType *RelStepsSize);
void UserTileSetSteps(struct UserTilePackInfoStruct *PackTileInfo,
		      const int *StepsMin, 
		      const int *StepsMax,
		      const CagdRType *RelStepsSize);


/* Tiles in 2D (including srfs and extruded 3D) - semi-regular. */
IPObjectStruct *UserTile2DSemi848(UserTileSemiRegType TileType,
				  const CagdRType TileSize);
IPObjectStruct *UserTile2DSemi1246(UserTileSemiRegType TileType,
				   const CagdRType TileSize);
IPObjectStruct *UserTile2DSemi4346(UserTileSemiRegType TileType,
				   const CagdRType TileSize);
IPObjectStruct *UserTile2DSemi6363(UserTileSemiRegType TileType,
				   const CagdRType TileSize);
IPObjectStruct *UserTile2DSemi12312(UserTileSemiRegType TileType,
				    const CagdRType TileSize);
IPObjectStruct *UserTile2DSemi33336(UserTileSemiRegType TileType,
				    const CagdRType TileSize);
IPObjectStruct *UserTile2DSemi43433(UserTileSemiRegType TileType,
				    const CagdRType TileSize);
IPObjectStruct *UserTile2DSemi44333(UserTileSemiRegType TileType,
				    const CagdRType TileSize);

IPObjectStruct *UserTile2DSemi848D(UserTileSemiRegDualType TileType,
				   const CagdRType TileSize,
				   const CagdRType Indent,
				   const CagdRType CrvAmt);
IPObjectStruct *UserTile2DSemi1246D(UserTileSemiRegDualType TileType,
				    const CagdRType TileSize,
				    const CagdRType Indent,
				    const CagdRType CrvAmt);
IPObjectStruct *UserTile2DSemi4346D(UserTileSemiRegDualType TileType,
				    const CagdRType TileSize,
				    const CagdRType Indent,
				    const CagdRType CrvAmt);
IPObjectStruct *UserTile2DSemi6363D(UserTileSemiRegDualType TileType,
				    const CagdRType TileSize,
				    const CagdRType Indent,
				    const CagdRType CrvAmt);
IPObjectStruct *UserTile2DSemi12312D(UserTileSemiRegDualType TileType,
				     const CagdRType TileSize,
				     const CagdRType Indent,
				     const CagdRType CrvAmt);
IPObjectStruct *UserTile2DSemi33336D(UserTileSemiRegDualType TileType,
				     const CagdRType TileSize,
				     const CagdRType Indent,
				     const CagdRType CrvAmt);
IPObjectStruct *UserTile2DSemi43433D(UserTileSemiRegDualType TileType,
				     const CagdRType TileSize,
				     const CagdRType Indent,
				     const CagdRType CrvAmt);
IPObjectStruct *UserTile2DSemi44333D(UserTileSemiRegDualType TileType,
				     const CagdRType TileSize,
				     const CagdRType Indent,
				     const CagdRType CrvAmt);

/* Heterogeneous rocket fuel design. */
IPObjectStruct *UserRocketFuelDesign3D( 
		  const TrivTVStruct *TV,
		  int NumLayers,
		  const int NumElements[2],
		  int SliceThrough,
		  int BndrySrfs,
		  const CagdRType *ApplyRGB,
		  UserRocketFuelThrustProfileFuncType ThrustFunc,
		  UserRocketFuelEvalLclFuelThrustFuncType EvalLclFuelThrustFunc,
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
					  const CagdRType *EuclideanPnt,
					  CagdRType *ResValue);
CagdBType UserMicroFunctionalEvaluateUV(UserMicro2TilingStructPtr Tiling,
					const CagdRType *UVPnt,
					CagdRType *ResValue);
IPObjectStruct *UserMicroFunctionalTilingIsoSurface(
					      UserMicro2TilingStructPtr Tiling,
					      int SamplingFactor);
CagdRType UserMicroFunctionalTilingVolume(UserMicro2TilingStructPtr Tiling, 
					  CagdRType CubeSize,
					  CagdBType PositiveVol);

/* Micro4strct.c - tile neighborhoods, etc. */
IPObjectStruct **UserMicroTileFindNeighbors(const IPObjectStruct *Tile,
					    const IPObjectStruct *TileList,
					    const MvarMVStruct *DeformMV,
					    const CagdBType *DeformMVPeriodic,
					    IPObjectStruct **Neighbors);

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

/* Create tiles from networks of curves. */
IPObjectStruct *UserCrvNetworktoTile(const IPObjectStruct *InObject,
				     CagdRType DfltOffset, 
				     UserCrvNetworkOutType OutType);

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
void UserSnapInterCrvs2Bndry(CagdCrvStruct *Crvs,
			     CagdRType UMin,
			     CagdRType UMax,
			     CagdRType VMin,
			     CagdRType VMax);
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
TrimSrfStruct *UserClipSrfAtPlane(const CagdSrfStruct *Srf,
				  const IrtPlnType Pln);
CagdSrfStruct *UserConservativeClipSrfByPlane(const CagdSrfStruct *Srf,
					      const IrtPlnType Pln);
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
			     const IrtBboxType BBox);
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

/* Dither by wire. */

IPObjectStruct *UserWDDitherCombiBW(IritImgPrcssImgStruct *Image0,
				    IritImgPrcssImgStruct *Image1,
				    int NumberOfLines,
				    int NumberOfPins,
				    float LineIntensity,
				    IritImgPrcssImgStruct **Projection0,
				    IritImgPrcssImgStruct **Projection1,
				    float Fairness0,
				    float Fairness1,
				    float FeatureImportance);
IPObjectStruct *UserWDDitherCombiRGB(IritImgPrcssImgStruct *Image0,
				IritImgPrcssImgStruct *Image1,
				int NumberOfLines,
				int NumberOfPins,
				float LineIntensity,
				IritImgPrcssImgStruct **Projection0,
				IritImgPrcssImgStruct **Projection1,
				float Fairness0,
				float Fairness1,
				float FeatureImportance);
 
IPObjectStruct *UserWDDitherStochasticBW(IritImgPrcssImgStruct *Image0,
					 IritImgPrcssImgStruct *Image1,
					 int NumberOfLines,
					 int NumberOfPins,
					 int NumberOfRandomTrials,
					 float LineIntensity,
					 IritImgPrcssImgStruct **Projection0,
					 IritImgPrcssImgStruct **Projection1,
					 float Fairness0,
					 float Fairness1,
					 int BoundWireLength,
					 float FeatureImportance);
IPObjectStruct *UserWDDitherStochasticRGB(IritImgPrcssImgStruct *Image0,
					  IritImgPrcssImgStruct *Image1,
					  int NumberOfLines,
					  int NumberOfPins,
					  int NumberOfRandomTrials,
					  float LineIntensity,
					  IritImgPrcssImgStruct **Projection0,
					  IritImgPrcssImgStruct **Projection1,
					  float Fairness0,
					  float Fairness1,
					  int BoundWireLength,
					  float FeatureImportance);

/* Dither by curves. */

int UserDitherImagesByCurvesGetDB(
			  const char *DBFileName,
			  int RequestedDim,
			  struct UserDitherImagesByCurvesDBStruct **DtrDBInfo);
void UserDitherImagesByCurvesFreeDB(
			   struct UserDitherImagesByCurvesDBStruct *DtrDBInfo);
int UserDitherImagesByCurvesGetImages(
				   UserDitherImagesByCurvesInfoStruct *DtrInfo,
				   const char **Images);
IPObjectStruct *UserDitherImagesByCurvesSweepTubes(
				   UserDitherImagesByCurvesInfoStruct *DtrInfo,
				   const IPObjectStruct *PCrvs);
IPObjectStruct *UserDitherImagesByCurves(
		     const struct UserDitherImagesByCurvesDBStruct *DtrDBInfo,
		     UserDitherImagesByCurvesInfoStruct *DtrInfo);

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
					 const struct IPObjectStruct *PlSrf,
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
				UserHCEditDrawCtlPtFuncType CtlPtDrawFunc,
				void *FuncData);
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
int UserCrvArngmntClassifyConnectedRegions(UserCrvArngmntStruct *CA);
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
					  int *ResultState,
					  const CagdRType *Tols,
					  CagdRType ZOffset);

/* Belt curves creation around circles. */

struct IPObjectStruct *UserBeltCreate(struct IPVertexStruct *Circs,
				      IrtRType BeltThickness,
				      IrtRType BoundingArcs,
				      int ReturnCrvs,
				      int *Intersects,
				      const char **Error);

/* 2D convex domain covering by a random curve. */

CagdCrvStruct *UserSCvrCoverSrf(const CagdCrvStruct *DomainBndry, 
				CagdCrvStruct *FillCrv, 
				CagdRType CoverEps, 
				CagdRType NumericTol, 
				CagdRType SubdivTol, 
				int TopK, 
				CagdRType TopEps,
				CagdRType IntrpBlndRatio);

/* Packing circles in 2D containers. */

CagdCrvStruct *UserPkPackCircles(CagdCrvStruct *Bndry,
				 CagdRType Radius,
				 int NumIter,
				 CagdRType NumericTol,
				 CagdRType SubdivTol);

/* Packing spheres and circles in 3D and 2D containers. */

/* Create a new polygonal border. */
UserSpkBorderStructPtr UserSpkPolyObjBorderNew(/* Create a new poly border. */
               const IPObjectStruct *PObj,
               IrtRType GridCellSize,    /* Size of a cell in look-up grid. */
	                                            
	       IrtRType MaxSphereRadius);     /* Maximum sphere radius this */
					       /* border will be used with. */

/* Delete a polygonal border. */
void UserSpkPolyObjBorderDelete(UserSpkBorderStructPtr Border);

/* Create a new surface border. */
UserSpkBorderStructPtr UserSpkSurfObjBorderNew(const IPObjectStruct *PObj,
					       IrtRType SubdivTol,
					       IrtRType NumerTol);

/* Delete a surface border */
void UserSpkSurfObjBorderDelete(UserSpkBorderStructPtr Border);

/* Initializes default solver settings for a given sphere radius. */
UserSpkSolvingSettingsStruct UserSpkSolvingSettingsGetDefault(
						       IrtRType SphereRadius);

/* Creates a new solving process. */
UserSpkSolvingProcessStructPtr UserSpkSolvingProcessNew(
				 const UserSpkSolvingSettingsStruct *Settings,
				 UserSpkBorderStructPtr Border,
                                 UserSpkSolvingStatusStruct *Status);

/* Forks a new solving process from an existing one. */
UserSpkSolvingProcessStructPtr UserSpkSolvingProcessFork(
                                        UserSpkSolvingProcessStructPtr Parent);

/* Deletes a solving process */
void UserSpkSolvingProcessDelete(UserSpkSolvingProcessStructPtr ProcessHandle);

/* Runs a solving process for a given number of attempts. */
void UserSpkSolvingProcessRunRepulsionsAttempt(
                                        UserSpkSolvingProcessStructPtr Process,
					UserSpkSolvingStatusStruct *Status);

/* Runs a solving process using a random gravity algorithm. */
void UserSpkSolvingProcessRunGravityAttempt(
					UserSpkSolvingProcessStructPtr Process,
					UserSpkSolvingStatusStruct *Status);

/* Run a given sphere packing algorithm on a given border within a given    */
/* time limit (budget).							    */
IPObjectStruct *UserSpkSolveForBorder(UserSpkBorderStructPtr Border,
                                      IrtRType SphereRadius,
                                      IrtRType TimeLimitInSeconds, 
                                      UserSpkAlgorithmType Algorithm);

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

/* Line Accessibility related code. */

void UserLineAccessSrfLineAccessResFree(UserLineAccessSrfLineAccessResStruct
					                      *SrfLineAccess);
IPObjectStruct *UserLineAccessSrfLineAccessToObject(
		   const UserLineAccessSrfLineAccessResStruct *SrfLineAccess);
UserLineAccessSrfLineAccessResStruct *UserLineAcccessSrfLineAccessFromObject(
						    const IPObjectStruct *Obj);
CagdSrfStruct *UserLineAccessSrfPreProcess(CagdSrfStruct **Srf);
UserLineAccessSrfLineAccessResStruct *UserLineAccessSrfLineAccessibility(
					   CagdSrfStruct *InSrf,
					   UserLineAccessParamsStruct *Params,
					   IPObjectStruct **ExtraOutObjs);
IPVertexStruct *UserLineCutPathToCutDirs(
		 const CagdSrfStruct *Srf,
		 const UserLineAccessSrfLineAccessResStruct *SrfAccessibility,
		 const CagdCrvStruct *CuttingCrv,
		 CagdRType MinAngleFromPathTan,
		 int NumSamples);
CagdSrfStruct *UserLineCutRobotPathToRuledSrf(const IPVertexStruct *CutPath,
					      CagdRType HalfWidth);

/* Gear design via conjugation using 2D swept volumes. */
IPObjectStruct *UserGr2DSwpReadCrvFromFile(char *Filename);
MvarMVStruct *UserGr2DSwpMatVecMult(const MvarMVStruct *Mat[2],
				    const MvarMVStruct *Vec);
void UserGr2DSwpGenInverseMotion(const MvarMVStruct *Rot[2],
				 const MvarMVStruct *Trans,
				 MvarMVStruct *InvRot[2],
				 MvarMVStruct **InvTrans);
void UserGr2DSwpGenMotionOblong(CagdRType ToothLen,
				int NumTeethSrc,
				int NumTeethCirc,
				int NumTeethLin,
				CagdBType InverseMotion,
				MvarMVStruct *Rot[2],
				MvarMVStruct **Trans);
void UserGr2DSwpGenUniformMotion(MvarMVStruct *Rot[2],
				 MvarMVStruct **Trans,
				 CagdRType GearDist);
void UserGr2DSwpGenNonUniformMotion(MvarMVStruct *Rot[2],
				    MvarMVStruct **Trans,
				    CagdRType GearDist,
				    const CagdCrvStruct *RelVeloFn);
CagdCrvStruct *UserGr2DSwpComputeCentrode(const MvarMVStruct *Rot[2],
					  const MvarMVStruct *Trans,
					  const CagdSrfStruct *Plane,
					  CagdRType SolverStepSize);
IPObjectStruct *UserGr2DSwpArrangeTeeth(const CagdCrvStruct *Centrode,
					const CagdCrvStruct *Tooth,
					CagdRType ToothLen,
					int NTeeth);
IPObjectStruct *UserGr2DSwpConjugateShape(const IPObjectStruct *CrvSegList,
					  const MvarMVStruct *Rot[2],
					  const MvarMVStruct *Trans,
					  CagdRType SolverStepSize);
void UserGr2DSwpMain(UserGr2DSwpParamStruct *Params);


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
/* #define _FREETYPE_FONTS_            On windows use native windows fonts. */
#define USER_FONT_DEFAULT_WIDTH	-1 

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

/* AM Fiber 3-Axis - fFragment split code. */
UserAMFiber3AxisTValListStruct *UserAMFiber3AxisGetMonotoneTVals(
						     const CagdCrvStruct *Crv);
UserAMFiber3AxisTValListStruct *UserAMFiber3AxisGetKnotsTVals(
						     const CagdCrvStruct *Crv);
UserAMFiber3AxisTValListStruct *UserAMFiber3AxisGetBBoxMaxTVals(
						     const CagdCrvStruct *Crv,
						     IrtRType Size);
UserAMFiber3AxisTValListStruct *UserAMFiber3AxisMergeTValLists(
					UserAMFiber3AxisTValListStruct *List1,
					UserAMFiber3AxisTValListStruct *List2);
UserAMFiber3AxisFragStruct *UserAMFiber3AxisGetTValFragments(
				   const CagdCrvStruct *Crv,
				   const UserAMFiber3AxisTValListStruct *List);
CagdCrvStruct *UserAMFiber3AxisGetFragmentCrvs(
				  const UserAMFiber3AxisFragStruct *Fragments);
void UserAMFiber3AxisGetCrvsFromTValArray(const CagdCrvStruct *Crv,
					  CagdCrvStruct **CrvArray,
					  IrtRType *TVals,
					  int First,
					  int Last);
  
void UserAMFiber3AxisFreeTValList(UserAMFiber3AxisTValListStruct *List);
void UserAMFiber3AxisFreeFragments(UserAMFiber3AxisFragStruct *Fragments);

/* AM Fiber 3-Axis - subtract fibers from ambient curves. */
CagdCrvStruct *UserAMFiber3AxisSubCrvs(const CagdCrvStruct *SubCrvs,
				       const CagdCrvStruct *FromCrvs,
				       IrtRType Dist,
				       IrtRType Accuracy,
				       IrtBType Invert);

/* AM Fiber 3-Axis - ordering curves code. */
CagdCrvStruct *UserAMFiber3AxisOrderCrvs(
				   const UserAMFiber3AxisCrvOrderStruct *Crvs,
				   int Num,
				   IrtRType Radius,
				   IrtRType XYRadius,
				   IrtRType Angle,
				   IrtRType ZOffs,
				   IrtRType Accuracy);
void UserAMFiber3AxisSaveCrvsAsSweeps(const CagdCrvStruct *Crvs,
				      const char *FileName);

/* 3D Puzzles constructions. */

IPObjectStruct *UserPuz3DComposeTileOverModel(
					const IPObjectStruct *InputTiles,
					const GMBBBboxStruct *TileBBox,
					const MdlModelStruct *Mdl,
					const CagdVType PolyApproxInfo,
					const CagdRType *BoolTols,
					CagdRType MergeStitchedTrimmedTiles);
IPObjectStruct *UserPuz3DComposeTileOverSrf(const IPObjectStruct *InputTiles,
					    const GMBBBboxStruct *TileBBox,
					    CagdSrfStruct **Srf,
					    CagdBType MapTo3D,
					    const CagdVType PolyApproxInfo,
					    IrtRType *TileXYZScale);

/* Truss construction. */

IPObjectStruct *UserTrussConstructLatticeMain(
			     const IPObjectStruct *InObj,
			     UserTrussSpherePackParamsStruct *SpherePackParams,
			     UserTrussLatticeParamsStruct *TrussParams,
			     const UserTrussTolerancesStruct *Tol);

void UserTrussDefaultTol(UserTrussTolerancesStruct *TolStruct);

IPObjectStruct *UserTrussTrimmedLattice(
				   const CagdPType *Pts,
				   int NumPts,
				   CagdRType DistToConnect,
				   UserTrussNodeDefCallbacksStruct *Callbacks,
				   CagdBType PrepForMdl,
				   CagdBType MultiObj,
				   const UserTrussTolerancesStruct *Tol);
IPObjectStruct *UserTrussTrimmedLatticeWithShell(
				    const CagdPType *Pts,
				    const CagdPType *ShellPts,
				    CagdRType *ShellDists,
				    int NumPts,
				    CagdRType DistToConnect,
				    UserTrussNodeDefCallbacksStruct *Callbacks,
				    CagdBType PruneOnly,
				    const IPObjectStruct *ShellObj,
				    CagdBType PrepForMdl,
				    const UserTrussTolerancesStruct *Tol);
IPObjectStruct *UserTrussTrivLattice(const CagdPType *Pts,
				     int NumPts,
				     CagdRType DistToConnect,
				     UserTrussNodeDefCallbacksStruct *Callbacks,
				     const UserTrussTolerancesStruct *Tol);
IPObjectStruct *UserTrussTrivLatticeWithShell(
				    const CagdPType *Pts,
				    const CagdPType *ShellPts,
				    CagdRType *ShellDists,
				    int NumPts,
				    CagdRType DistToConnect,
				    UserTrussNodeDefCallbacksStruct *Callbacks,
				    const IPObjectStruct *ShellObj,
				    const UserTrussTolerancesStruct *Tol);
IPObjectStruct *UserTrussLatticeWithQualityInfo(
				  const CagdPType *Pts,
				  int NumPts,
				  CagdRType DistToConnect,
				  UserTrussNodeDefCallbacksStruct *Callbacks,
				  const CagdRType *QuantizationVector,
				  const UserTrussTolerancesStruct *Tol);
void UserTrussDefaultTol(UserTrussTolerancesStruct *TolStruct);
void UserTrussSetConstBeamCallbacks(
				 UserTrussConstBeamWidthStruct *ConstBeamData,
				 UserTrussNodeDefCallbacksStruct *Callbacks);
void UserTrussSetGradedBeamCallbacks(
				UserTrussGradedBeamWidthStruct *GradedBeamData,
				UserTrussNodeDefCallbacksStruct *Callbacks);
void UserTrussSetCustomBeamCallbacks(
				UserTrussBeamInfoPrepFuncType PrepCallback,
				UserTrussBeamInfoFuncType SetDataCallback,
				UserTrussBeamInfoCleanFuncType CleanupCallback,
				void *ExtraData,
				UserTrussNodeDefCallbacksStruct *Callbacks);

/* Constructs Hoberman-like expanding structures. */
IPObjectStruct *UserHoberConstRadMdl(const CagdCrvStruct *Crv,
				     CagdRType Offset,
				     int NumOfScissors,
				     int RefineCycles,
				     const IrtVecType PinHoleDiams,
				     CagdRType ScissorThickness,
				     CagdRType ScissorRelWidth,
				     int AddColorCode,
				     CagdRType Tol);
IPObjectStruct *UserHoberConstAngleMdl(const CagdCrvStruct *Crv,
				       CagdRType Offset,
				       int NumOfScissors,
				       int RefineCycles,
				       const IrtVecType PinHoleDiams,
				       CagdRType ScissorThickness,
				       CagdRType ScissorRelWidth,
				       CagdRType ScissorRelYShift,
				       int AddColorCodes,
				       CagdRType Tol);
  
/* Visualization (of multi dim. data). */

typedef IPObjectStruct *(*UserVolVisGenPolygonRepresentationFuncType)(
						   void *DataCB,
						   const IPObjectStruct *PObj,
						   char *Error);
typedef int (*UserVolVisGetRGBFromRealFuncType)(void *DataCB,
						IrtRType PropVal,
						IrtRType *r,
						IrtRType *g,
						IrtRType *b);

int UserVolVisVerifyCorrespondence(const IPObjectStruct *GeomObj,
				   const IPObjectStruct *PropObj,
				   char *Error);
IPObjectStruct *UserVolVisMapPropertyOnGeometry(
				const IPObjectStruct *GeomObj,
				const IPObjectStruct *PropObj,
				UserVolVisGenPolygonRepresentationFuncType
				                      GenPolygonRepresentFunc,
				void *GenPolygonRepresentData,
				UserVolVisGetRGBFromRealFuncType
							   GetRGBFromRealFunc,
				void *GetRGBFromRealData,
				int PropCoord,
				IrtRType MaxEdgeLen,
				IrtRType MinProp,
				IrtRType MaxProp,
				char *Error);
IPObjectStruct *UserVolVisMapPropertyOnIsoSrfGeometry(
				const IPObjectStruct *GeomObj,
				const IPObjectStruct *PropObj,
				UserVolVisGenPolygonRepresentationFuncType
				                      GenPolygonRepresentFunc,
				void *GenPolygonRepresentData,
				UserVolVisGetRGBFromRealFuncType
							   GetRGBFromRealFunc,
				void *GetRGBFromRealData,
				TrivTVDirType VisDir,
				IrtRType ParamVal,
				int PropCoord,
				IrtRType MaxEdgeLen,
				IrtRType MinProp,
				IrtRType MaxProp,
				char *Error);
void UserVolVisMapPropertyOnPolygons(IPObjectStruct *PlObj,
				     int UVReversed,
				     const TrivTVStruct *PropTV,
				     UserVolVisGetRGBFromRealFuncType
							  GetRGBFromRealFunc,
				     void *GetRGBFromRealData,
				     int PropCoord,
				     IrtRType MinProp,
				     IrtRType MaxProp,
				     char *Error);
IPObjectStruct *UserVolVisMapPropertyOnPlanarSliceGeometry(
					const IPObjectStruct *GeomObj,
					const IPObjectStruct *PropObj,
					const IrtPlnType PlaneEqn,
					UserVolVisGetRGBFromRealFuncType
							    GetRGBFromRealFunc,
					void *GetRGBFromRealData,
					int PropCoord,
					IrtRType MaxEdgeLen,
					IrtRType MinProp,
					IrtRType MaxProp,
					char *Error);
IPObjectStruct *UserVolVisMapPropertyOnMarchingCubeGeometry(
					const IPObjectStruct *GeomObj,
					const IPObjectStruct *PropObj,
					UserVolVisGetRGBFromRealFuncType
							  GetRGBFromRealFunc,
					void *GetRGBFromRealData,
					IrtRType IsoVal,
					int PropCoord,
					IrtRType MaxEdgeLen,
					IrtRType MinProp,
					IrtRType MaxProp,
					char *Error);
IPObjectStruct *UserVolVisDisplaceGeometry(const IPObjectStruct *GeomObj,
					   const IPObjectStruct *PropObj,
					   IrtRType PropScale,
					   char *Error);
IPObjectStruct *UserSrfVisSurfaceVisualize(
				const IPObjectStruct *GeomSrf,
				const IPObjectStruct *PropSrf,
				UserVolVisGenPolygonRepresentationFuncType
				                      GenPolygonRepresentFunc,
				void *GenPolygonRepresentData,
				UserVolVisGetRGBFromRealFuncType
							   GetRGBFromRealFunc,
				void *GetRGBFromRealData,
				int PropCoord,
				IrtRType *MinVal,
				IrtRType *MaxVal,
				const char *MapFuncStr,
				IrtRType MaxEdgeLen,
				char *Error);

/* Error handling. */

UserSetErrorFuncType UserSetFatalErrorFunc(UserSetErrorFuncType ErrorFunc);
const char *UserDescribeError(UserFatalErrorType ErrorNum);
void UserFatalError(UserFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_USER_LIB_H */
