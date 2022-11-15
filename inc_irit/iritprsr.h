/*****************************************************************************
* Generic parser for the "Irit" solid modeller.				     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
* Written by:  Gershon Elber				Ver 0.2, Sep. 1991   *
*****************************************************************************/

#ifndef	IRIT_PRSR_H
#define	IRIT_PRSR_H

#include <setjmp.h>

#include "inc_irit/irit_sm.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/trim_lib.h"
#include "inc_irit/triv_lib.h"
#include "inc_irit/trng_lib.h"
#include "inc_irit/mdl_lib.h"
#include "inc_irit/vmdl_lib.h"
#include "inc_irit/mvar_lib.h"
#include "inc_irit/misc_lib.h"

/* Dont change the order of these objects (or there values as overloaded     */
/* tables (see overload.c) are hardwired to it. If you add objects update    */
/* that module properly.						     */
typedef enum {
    IP_OBJ_ERROR = -1,
    IP_OBJ_UNDEF = 0,

    IP_OBJ_POLY,                     /* These are the objects in overload.c. */
    IP_OBJ_NUMERIC,
    IP_OBJ_POINT,
    IP_OBJ_VECTOR,
    IP_OBJ_PLANE,
    IP_OBJ_MATRIX,
    IP_OBJ_CURVE,
    IP_OBJ_SURFACE,
    IP_OBJ_STRING,
    IP_OBJ_LIST_OBJ,
    IP_OBJ_CTLPT,
    IP_OBJ_TRIMSRF,
    IP_OBJ_TRIVAR,
    IP_OBJ_INSTANCE,
    IP_OBJ_TRISRF,
    IP_OBJ_MODEL,
    IP_OBJ_MULTIVAR,
    IP_OBJ_VMODEL,
    IP_OBJ_VXL_VMDL,

    IP_OBJ_ANY = 100		 /* Match any object type, in type checking. */
} IPObjStructType;

typedef enum {			 /* Possible error code during data parsing. */
    IP_ERR_NO_LINE_NUM = -100, /* Signals no line num of error is avialable. */

    IP_ERR_NONE = 0,

    IP_ERR_ALLOC_FREED_LOOP,
    IP_ERR_PT_OBJ_EXPECTED,
    IP_ERR_LIST_OBJ_EXPECTED,
    IP_ERR_LIST_OBJ_SHORT,
    IP_ERR_DEL_OBJ_NOT_FOUND,
    IP_ERR_LOCASE_OBJNAME,
    IP_ERR_UNDEF_ATTR,
    IP_ERR_PTR_ATTR_COPY,
    IP_ERR_UNSUPPORT_CRV_TYPE,
    IP_ERR_UNSUPPORT_SRF_TYPE,
    IP_ERR_UNSUPPORT_TV_TYPE,
    IP_ERR_UNSUPPORT_TRNG_TYPE,
    IP_ERR_UNSUPPORT_MV_TYPE,
    IP_ERR_NOT_SUPPORT_CNVRT_IRT,
    IP_ERR_NEIGH_SEARCH,
    IP_ERR_VRTX_HASH_FAILED,
    IP_ERR_INVALID_STREAM_HNDL,
    IP_ERR_STREAM_TBL_FULL,
    IP_ERR_LIST_CONTAIN_SELF,
    IP_ERR_UNDEF_OBJECT_FOUND,
    IP_ERR_ILLEGAL_FLOAT_FRMT,
    IP_ERR_NON_LIST_IGNORED,
    IP_ERR_LIST_TOO_LARGE,
    IP_ERR_LESS_THAN_3_VRTCS,
    IP_ERR_FORK_FAILED,
    IP_ERR_CLOSED_SOCKET,
    IP_ERR_READ_LINE_TOO_LONG,
    IP_ERR_NUMBER_EXPECTED,
    IP_ERR_OPEN_PAREN_EXPECTED,
    IP_ERR_CLOSE_PAREN_EXPECTED,
    IP_ERR_LIST_COMP_UNDEF,
    IP_ERR_UNDEF_EXPR_HEADER,
    IP_ERR_PT_TYPE_EXPECTED,
    IP_ERR_OBJECT_EMPTY,
    IP_ERR_FILE_EMPTY,
    IP_ERR_FILE_NOT_FOUND,
    IP_ERR_MIXED_TYPES,
    IP_ERR_STR_NOT_IN_QUOTES,
    IP_ERR_STR_TOO_LONG,
    IP_ERR_OBJECT_EXPECTED,
    IP_ERR_STACK_OVERFLOW,
    IP_ERR_DEGEN_POLYGON,
    IP_ERR_DEGEN_NORMAL,
    IP_ERR_SOCKET_BROKEN,
    IP_ERR_SOCKET_TIME_OUT,

    IP_ERR_CAGD_LIB_ERR,
    IP_ERR_TRIM_LIB_ERR,
    IP_ERR_TRIV_LIB_ERR,
    IP_ERR_TRNG_LIB_ERR,
    IP_ERR_MDL_LIB_ERR,
    IP_ERR_MVAR_LIB_ERR,

    IP_ERR_BIN_IN_TEXT,
    IP_ERR_BIN_UNDEF_OBJ,
    IP_ERR_BIN_WRONG_SIZE,
    IP_ERR_BIN_SYNC_FAIL,
    IP_ERR_BIN_PL_SYNC_FAIL,
    IP_ERR_BIN_CRV_SYNC_FAIL,
    IP_ERR_BIN_CRV_LIST_EMPTY,
    IP_ERR_BIN_SRF_SYNC_FAIL,
    IP_ERR_BIN_TSRF_SYNC_FAIL,
    IP_ERR_BIN_TCRV_SYNC_FAIL,
    IP_ERR_BIN_TV_SYNC_FAIL,
    IP_ERR_BIN_MV_SYNC_FAIL,
    IP_ERR_BIN_TRISRF_SYNC_FAIL,
    IP_ERR_BIN_MAT_SYNC_FAIL,
    IP_ERR_BIN_INST_SYNC_FAIL,
    IP_ERR_BIN_STR_SYNC_FAIL,
    IP_ERR_BIN_OLST_SYNC_FAIL,
    IP_ERR_BIN_ATTR_SYNC_FAIL,

    IP_ERR_NC_ARC_INVALID_RAD,
    IP_ERR_NC_MAX_ZBUF_SIZE_EXCEED,

    IP_ERR_ONLY_FREEFORM,
    IP_ERR_ONLY_CRV_SRF_MV,
    IP_ERR_ONLY_TRI_SRF,
    IP_ERR_ONLY_TRIM_SRF,
    IP_ERR_CNVRT_TO_PERIODIC,
    IP_ERR_CNVRT_PER_TO_FLOAT,
    IP_ERR_CNVRT_BSP_TO_FLOAT,
    IP_ERR_CNVRT_MV_NOT_UNIVAR,
    IP_ERR_CNVRT_MV_NOT_BIVAR,
    IP_ERR_CNVRT_MV_NOT_TRIVAR,
    IP_ERR_CNVRT_TO_MDL,
    IP_ERR_CNVRT_TO_VMDL,
    IP_ERR_CNVRT_SRF_MDL_TO_TSRF,
    IP_ERR_CNVRT_INVALID_GEOM_TO_MV,
    IP_ERR_CNVRT_INVALID_COERCE,

    IP_WRN_OBJ_NAME_TRUNC = 1000,

    IP_ERR_INFO_SHIFT = 10000
} IPFatalErrorType;

typedef enum {
    IP_FILE_TEXT,
    IP_FILE_BINARY,
    IP_FILE_COMPRESSED
} IPFileType;

typedef enum {
    IP_ANY_FILE = 0,
    IP_IBD_FILE,
    IP_ITD_FILE,
    IP_VRML_FILE,
    IP_HPGL_FILE,
    IP_PS_FILE,
    IP_IGS_FILE,
    IP_STL_FILE,
    IP_NFF_FILE,
    IP_OFF_FILE,
    IP_PLG_FILE,
    IP_POV_FILE,
    IP_RAY_FILE,
    IP_SCN_FILE,
    IP_XFG_FILE,
    IP_GCODE_FILE,
    IP_OBJ_FILE,
    IP_MSH_FILE,
    IP_3MF_FILE
} IPStreamFormatType;

/* Draw mode type. */
typedef enum {
    IRIT2WGL_DRAW_MODE_WIREFRAME = 0x0001,
    IRIT2WGL_DRAW_MODE_SOLID = 0x0002,
    IRIT2WGL_DRAW_MODE_TEXTURE = 0x0004
} Irit2WglDrawModeType;

/* View angle type. */
typedef enum {
    IRIT2WGL_VIEW_ANGLE_ORIGINAL = 0,
    IRIT2WGL_VIEW_ANGLE_FRONT,
    IRIT2WGL_VIEW_ANGLE_BACK,
    IRIT2WGL_VIEW_ANGLE_RIGHT,
    IRIT2WGL_VIEW_ANGLE_LEFT,
    IRIT2WGL_VIEW_ANGLE_TOP,
    IRIT2WGL_VIEW_ANGLE_BOTTOM
} Irit2WglViewAngleType;

/* Projection mode type. */
typedef enum {
    IRIT2WGL_PROJECTION_MODE_ORTHOGONAL = 0,
    IRIT2WGL_PROJECTION_MODE_PERSPECTIVE
} Irit2WglProjectionModeType;

typedef enum {
    IP_NC_GCODE_TOOL_GENERAL = 0,
    IP_NC_GCODE_TOOL_BALL_END,
    IP_NC_GCODE_TOOL_TORUS_END,
    IP_NC_GCODE_TOOL_FLAT_END
} IPNCGCToolType;

typedef enum {
    IP_NC_GCODE_LINE_COMMENT = 0,
    IP_NC_GCODE_LINE_NONE,

    IP_NC_GCODE_LINE_MOTION_G0FAST,     /* G0 line */
    IP_NC_GCODE_LINE_MOTION_G1LINEAR,   /* G1 line */
    IP_NC_GCODE_LINE_MOTION_G2CW,       /* G2 line */
    IP_NC_GCODE_LINE_MOTION_G3CCW,      /* G3 line */
    IP_NC_GCODE_LINE_MOTION_OTHER,      /* Unsupported motion line */

    IP_NC_GCODE_LINE_NON_MOTION
} IPNCGCodeLineType;

typedef enum {
    IP_COERCE_UNTRIMMED_TYPE = 5010,
    IP_COERCE_UNIFORM_PERIODIC,
    IP_COERCE_UNIFORM_FLOAT,
    IP_COERCE_UNIFORM_OPEN,
    IP_COERCE_POWER_TYPE,
    IP_COERCE_BEZIER_TYPE,
    IP_COERCE_BSPLINE_TYPE,
    IP_COERCE_GREGORY_TYPE,
} IPCoerceObjToType;

typedef struct IPNCGCodeLineStruct {
    int StreamLineNumber;                       /* Stream/File line number. */
    int GCodeLineNumber;        /* G code's Nxxxx if has one, -1 otherwise. */
    char *Line;                        /* A copy of the H code as a string. */
    IPNCGCodeLineType GCodeType;
    IrtRType ABC[3], IJK[3], XYZ[3];/* Tool orientation/arc motion/position.*/
    IrtRType FeedRate, UpdatedFeedRate;
    IrtRType SpindleSpeed;
    IrtRType LenStart, Len;       /* Length from start and from last point. */
    IrtRType DFactor;                   /* Used in AM to set material info. */
    IrtRType EFactor;     /* Used in AM to set flow of material deposition. */
    IrtRType Width;                    /* Used in AM to set printing width. */
    int MFactor;             /* GCode M Commands... (No really supported.). */
    int ToolNumber;
    int IsVerticalUpMotion;		  /* TRUE if we move up vertically. */
    int Comment;
    int HasMotion;		 /* TRUE if this line performs some motion. */
    CagdCrvStruct *Crv;
} IPNCGCodeLineStruct;

#define IRIT_DATA_HEADER(File, Name) { \
    char Str[IRIT_LINE_LEN_LONG]; \
    fprintf(File, "Irit %s, %s,\nCreator: %s,\nDate: %s.\n\n", \
	    IRIT_VERSION, IRIT_COPYRIGHT, Name, IritRealTimeDate(Str)); \
}

typedef struct IPIritGenericStruct {
    struct IPIritGenericStruct *Pnext;		        /* To next in chain. */
    struct IPAttributeStruct *Attr;
} IPIritGenericStruct;

/*****************************************************************************
* An instance object - a duplicate at a different location.                  *
*****************************************************************************/
typedef struct IPInstanceStruct {
    struct IPInstanceStruct *Pnext;		        /* To next in chain. */
    struct IPAttributeStruct *Attr;
    struct IPObjectStruct *PRef;/* Reference to object this is its instance. */
    char *Name;		             /* Name of object this is its instance. */
    IrtHmgnMatType Mat;	  /* Transformation from Object Name to this object. */
} IPInstanceStruct;

/*****************************************************************************
* Global data structures:						     *
* Objects in the system might be (real) scalars, (R3) vectors, matrices      *
* (4 by 4 - transformation matrix), strings of chars, lists of objects, or   *
* geometric objects. All but the last are simple and all their data is saved *
* in the object space itself. The last (geometric) object points on a	     *
* curve or a surface or a polygonal list of the form:			     *
*									     *
* Polygon -> Polygon -> Polygon -> Polygon -> .... -> NULL		     *
*    |		|	   |	      |					     *
*    V          V          V          V					     *
*  VList      VList      VList      VList	(VList = Vertex List)	     *
*									     *
* Each VList is usually a CIRCULAR vertex list. Each VList element           *
* (IPVertexStruct) implicitly defines an edge from this vertex, to the next. *
* As each edge is used by exactly two polygons, a pointer to the other       *
* polygon using this edge exists in the IPVertexStruct as PAdj. Each polygon *
* has also its Plane definition for fast processing, with its normal         *
* pointing INTO the object.						     *
*   Few other tags & flags are included in the data structures for different *
* modules.								     *
*   Note, vertices are not shared by few VLists/Polygons although it may     *
* decrease memory usage (suprisingly, not much). The main reason to that is  *
* the basic assumption of this solid modeller, which is simplicity...	     *
*****************************************************************************/

/*****************************************************************************
* Vertex Type - holds single 3D point, including some attributes on it as    *
* Tags. The 3D coordinates are saved in Pt. Pointer to next in chain	     *
* is Pnext, and the pointer to the adjacent polygon (to the edge defined by  *
* this Vertex and Vertex -> Pnext) is PAdj.				     *
*****************************************************************************/

/* Internal edge, or edge generated by the polygon decomposition stage when  */
/* only convex polygons are allowed. This edge was not in the input	     */
/* non-convex polygon, and therefore one may not want to see/display it.     */
/* Note bits 4-7 (high nibble of Tags) are reserved for the different	     */
/* modules to perform their local tasks and so should not be used here.	     */
#define IP_VRTX_INTERNAL_TAG	0x01    /* Internal Tag - Edge is internal.  */
#define IP_VRTX_NORMAL_TAG	0x02     /* Normal Tag - Vertex has normal.  */

#define IP_IS_INTERNAL_VRTX(Vrtx)	(Vrtx -> Tags & IP_VRTX_INTERNAL_TAG)
#define IP_SET_INTERNAL_VRTX(Vrtx)	(Vrtx -> Tags |= IP_VRTX_INTERNAL_TAG)
#define IP_RST_INTERNAL_VRTX(Vrtx)	(Vrtx -> Tags &= ~IP_VRTX_INTERNAL_TAG)
#define IP_HAS_NORMAL_VRTX(Vrtx)	(Vrtx -> Tags & IP_VRTX_NORMAL_TAG)
#define IP_SET_NORMAL_VRTX(Vrtx)	(Vrtx -> Tags |= IP_VRTX_NORMAL_TAG)
#define IP_RST_NORMAL_VRTX(Vrtx)	(Vrtx -> Tags &= ~IP_VRTX_NORMAL_TAG)

typedef struct IPVertexStruct {
    struct IPVertexStruct *Pnext;		        /* To next in chain. */
    struct IPAttributeStruct *Attr;
    struct IPPolygonStruct *PAdj;		     /* To adjacent polygon. */
    IrtBType Tags;					 /* Some attributes. */
    IrtPtType Coord;			       /* Holds X, Y, Z coordinates. */
    IrtNrmlType Normal;		       /* Hold Vertex normal into the solid. */
} IPVertexStruct;

/*****************************************************************************
* Polygon Type - holds single polygon - Its Plane definition, and a pointer  *
* to its vertices contour list V. As for IPVertexStruct, different attributes*
* can be saved in Tags. PAux can be used locally by different modules, for   *
* local usage only, and nothing sould be assumed on entry.		     *
*****************************************************************************/

/* Note bits 4-7 (high nibble of Tags) are reserved for the different	     */
/* modules to perform their local tasks and so should not be used here.	     */
#define IP_POLY_CONVEX_TAG	0x01	   /* Convex Tag - Set if is convex. */
#define IP_POLY_BBOX_TAG	0x02  /* BBox Tag - Set if BBox is computed. */
#define IP_POLY_PLANE_TAG	0x04    /* Plane Tag - set if has plane def. */
#define IP_POLY_STRIP_TAG	0x08     /* A strip of polygons starts/ends. */

#define	IP_IS_CONVEX_POLY(Poly)		((Poly) -> Tags & IP_POLY_CONVEX_TAG)
#define	IP_SET_CONVEX_POLY(Poly)	((Poly) -> Tags |= IP_POLY_CONVEX_TAG)
#define	IP_RST_CONVEX_POLY(Poly)	((Poly) -> Tags &= ~IP_POLY_CONVEX_TAG)
#define	IP_HAS_BBOX_POLY(Poly)		((Poly) -> Tags & IP_POLY_BBOX_TAG)
#define	IP_SET_BBOX_POLY(Poly)		((Poly) -> Tags |= IP_POLY_BBOX_TAG)
#define	IP_RST_BBOX_POLY(Poly)		((Poly) -> Tags &= ~IP_POLY_BBOX_TAG)
#define IP_HAS_PLANE_POLY(Poly)		((Poly) -> Tags & IP_POLY_PLANE_TAG)
#define IP_SET_PLANE_POLY(Poly)		((Poly) -> Tags |= IP_POLY_PLANE_TAG)
#define IP_RST_PLANE_POLY(Poly)		((Poly) -> Tags &= ~IP_POLY_PLANE_TAG)
#define IP_IS_STRIP_POLY(Poly)		((Poly) -> Tags & IP_POLY_STRIP_TAG)
#define IP_SET_STRIP_POLY(Poly)		((Poly) -> Tags |= IP_POLY_STRIP_TAG)
#define IP_RST_STRIP_POLY(Poly)		((Poly) -> Tags &= ~IP_POLY_STRIP_TAG)

typedef struct IPPolygonStruct {
    struct IPPolygonStruct *Pnext;		        /* To next in chain. */
    struct IPAttributeStruct *Attr;
    IPVertexStruct *PVertex;		    		/* To vertices list. */
    VoidPtr PAux;
    IrtBType Tags;				         /* Some attributes. */
    int IAux, IAux2, IAux3;
    IrtPlnType Plane;			 /* Holds Plane as Ax + By + Cz + D. */
    IrtBboxType BBox;				        /* BBox of polygons. */
} IPPolygonStruct;

typedef struct IPPolyPtrStruct {
    struct IPPolyPtrStruct *Pnext;
    struct IPPolygonStruct *Poly;
} IPPolyPtrStruct;

typedef struct IPPolyVrtxArrayStruct {
    struct IPPolyVrtxArrayStruct *Pnext;                /* To next in chain. */
    struct IPAttributeStruct *Attr;                  /* Object's attributes. */
    const struct IPObjectStruct *PObj; /* Pointer to original polygonal obj. */
    IPVertexStruct **Vertices;    /* NULL terminated vector of all vertices. */
    IPPolyPtrStruct **PPolys;     /* Vector of polygons holding each vertex. */
    int **Polygons;        /* A vector of -1 terminated vectors of vertices. */
    int *_AuxVIndices;  /* Auxiliary memory to hold all indices in Polygons. */
    int NumVrtcs;                         /* Number of vertices in geometry. */
    int NumPlys;                          /* Number of polygons in geometry. */
    int TriangularMesh; /* TRUE if a triangular polys only, FALSE otherwise. */
} IPPolyVrtxArrayStruct;

/*****************************************************************************
* Object Type - main system structure, which holds all the objects defined   *
* in the system like Numeric, Geometric etc.				     *
*   Note that as the number of objects will be usually extremely low (100 is *
* high estimate!) we can waste some memory here...			     *
*****************************************************************************/

#define IP_IS_UNDEF_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_UNDEF)
#define IP_IS_POLY_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_POLY)
#define IP_IS_NUM_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_NUMERIC)
#define IP_IS_POINT_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_POINT)
#define IP_IS_VEC_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_VECTOR)
#define IP_IS_PLANE_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_PLANE)
#define IP_IS_CTLPT_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_CTLPT)
#define IP_IS_MAT_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_MATRIX)
#define IP_IS_STR_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_STRING)
#define IP_IS_OLST_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_LIST_OBJ)
#define IP_IS_CRV_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_CURVE)
#define IP_IS_SRF_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_SURFACE)
#define IP_IS_TRIMSRF_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_TRIMSRF)
#define IP_IS_TRIVAR_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_TRIVAR)
#define IP_IS_TRISRF_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_TRISRF)
#define IP_IS_INSTNC_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_INSTANCE)
#define IP_IS_MODEL_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_MODEL)
#define IP_IS_VMODEL_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_VMODEL)
#define IP_IS_VXL_VMDL_OBJ(Obj) ((Obj) -> ObjType == IP_OBJ_VXL_VMDL)
#define IP_IS_MVAR_OBJ(Obj)	((Obj) -> ObjType == IP_OBJ_MULTIVAR)

#define IP_IS_GEOM_OBJ(Obj)	(IP_IS_UNDEF_OBJ(Obj) || \
				 IP_IS_POLY_OBJ(Obj) || \
				 IP_IS_POINT_OBJ(Obj) || \
				 IP_IS_CTLPT_OBJ(Obj) || \
				 IP_IS_VEC_OBJ(Obj) || \
				 IP_IS_CRV_OBJ(Obj) || \
				 IP_IS_SRF_OBJ(Obj) || \
				 IP_IS_TRIMSRF_OBJ(Obj) || \
				 IP_IS_TRIVAR_OBJ(Obj) || \
				 IP_IS_TRISRF_OBJ(Obj) || \
				 IP_IS_MODEL_OBJ(Obj) || \
				 IP_IS_MVAR_OBJ(Obj) || \
				 IP_IS_VXL_VMDL_OBJ(Obj) || \
				 IP_IS_VMODEL_OBJ(Obj) || \
				 IP_IS_INSTNC_OBJ(Obj))

#define IP_IS_FFGEOM_OBJ(Obj)	(IP_IS_CRV_OBJ(Obj) || \
				 IP_IS_SRF_OBJ(Obj) || \
				 IP_IS_TRIMSRF_OBJ(Obj) || \
				 IP_IS_TRIVAR_OBJ(Obj) || \
				 IP_IS_TRISRF_OBJ(Obj) || \
				 IP_IS_MODEL_OBJ(Obj) || \
				 IP_IS_MVAR_OBJ(Obj) || \
				 IP_IS_VMODEL_OBJ(Obj) || \
				 IP_IS_INSTNC_OBJ(Obj))

#define IP_IS_POLYGON_OBJ(Obj)	  (((Obj) -> Tags & 0x03) == 0)
#define IP_SET_POLYGON_OBJ(Obj)	  ((Obj) -> Tags = ((Obj) -> Tags & 0xfc))
#define IP_IS_POLYLINE_OBJ(Obj)	  (((Obj) -> Tags & 0x03) == 1)
#define IP_SET_POLYLINE_OBJ(Obj)  ((Obj) -> Tags = ((Obj) -> Tags & 0xfc) + 1)
#define IP_IS_POINTLIST_OBJ(Obj)  (((Obj) -> Tags & 0x03) == 2)
#define IP_SET_POINTLIST_OBJ(Obj) ((Obj) -> Tags = ((Obj) -> Tags & 0xfc) + 2)
#define IP_IS_POLYSTRIP_OBJ(Obj)  (((Obj) -> Tags & 0x03) == 3)
#define IP_SET_POLYSTRIP_OBJ(Obj) ((Obj) -> Tags = ((Obj) -> Tags & 0xfc) + 3)

#define IP_OBJ_BBOX_TAG		  0x04
#define	IP_HAS_BBOX_OBJ(Obj)	  ((Obj) -> Tags & IP_OBJ_BBOX_TAG)
#define	IP_SET_BBOX_OBJ(Obj)	  ((Obj) -> Tags |= IP_OBJ_BBOX_TAG)
#define	IP_RST_BBOX_OBJ(Obj)	  ((Obj) -> Tags &= ~IP_OBJ_BBOX_TAG)

/* Maximum size of object list to start with (reallocated dynamically). */
#define IP_MAX_OBJ_LIST	10

/* This handler forces the writing to all active clients (broadcasting). */
#define IP_CLNT_BROADCAST_ALL_HANDLES 	13030

/* Minimum tol to consider two knots as identical - for read data validity. */
#define IP_KNOT_SEQ_VALIDITY_TOL	1e-6

typedef struct IPObjectStruct {
    struct IPObjectStruct *Pnext;                       /* To next in chain. */
    struct IPAttributeStruct *Attr;                  /* Object's attributes. */
    struct IPODObjectDpndncyStruct *Dpnds;   /* Dependencies and parameters. */
    unsigned int Count;                                  /* Reference Count. */
    unsigned int Tags;                                   /* Some attributes. */
    IPObjStructType ObjType;        /* Object Type: Numeric, Geometric, etc. */
    IrtBboxType BBox;				  	  /* BBox of object. */
    union {
        IPPolygonStruct *Pl;                           /* Polygon/line list. */
        CagdGenericStruct *Gnrc;    /* The generic structure (Pnext & Attr). */
        CagdCrvStruct *Crvs;                          /* Free form curve(s). */
        CagdSrfStruct *Srfs;                        /* Free form surface(s). */
        TrimSrfStruct *TrimSrfs;            /* Free form trimmed surface(s). */
        TrivTVStruct *Trivars;                   /* Free form trivariate(s). */
        TrngTriangSrfStruct *TriSrfs;    /* Free form triangular surface(s). */
        IPInstanceStruct *Instance;             /* An instance of an object. */
	MdlModelStruct *Mdls;                                    /* A model. */
	VMdlVModelStruct *VMdls;                      /* A volumetric model. */
        VMdlVoxelVModelStruct *VxlVMdls;    /* A Voxelized volumetric model. */
        MvarMVStruct *MultiVars;                  /* Multivariate functions. */
        IrtRType R;                                    /* Numeric real data. */
        IrtPtType Pt;                            /* Numeric real point data. */
        IrtVecType Vec;                         /* Numeric real vector data. */
        IrtPlnType Plane;                        /* Numeric real plane data. */
        CagdCtlPtStruct CtlPt;                        /* Control point data. */
        IrtHmgnMatType *Mat;        /* Numeric 4 by 4 transformation matrix. */
        struct {
            struct IPObjectStruct **PObjList;            /* List of objects. */
            int ListMaxLen;           /* Maximum number of elements in list. */
        } Lst;
        char *Str;                        /* General string for text object. */
        VoidPtr *VPtr;
    } U;
    char *ObjName;		                          /* Name of object. */
} IPObjectStruct;

typedef void (*IPSetErrorFuncType)(IPFatalErrorType ErrorFunc);
typedef void (*IPPrintFuncType)(const char *PrintFunc);
typedef void (*IPProcessLeafObjType)(IPObjectStruct *PObj);
typedef int (*IPStreamReadCharFuncType)(int Handler);
typedef int (*IPStreamWriteBlockFuncType)(int Handler,
					  VoidPtr Block,
					  int Size);
typedef void (*IPApplyObjFuncType)(IPObjectStruct *PObj,
				   IrtHmgnMatType Mat,
				   void *Data);
typedef void (*IPNCGCodeRectangleToolSweepFuncType)(IrtPtType Pt1,
						    IrtPtType Pt2,
						    IrtPtType Pt3,
						    IrtPtType Pt4);
typedef IrtRType (*IPNCGCodeEvalMRRFuncType)(VoidPtr Data);
typedef void (*IPNCGCodeParserErrorFuncType)(char *Line);
typedef void (*IPNCGCodeIndexUpdateFuncType)(void *Data);
typedef IPObjectStruct *(*IPForEachObjCallBack)(IPObjectStruct *PObj, 
                                                void *Param);
typedef IPPolygonStruct *(*IPForEachPolyCallBack)(IPPolygonStruct *Pl, 
                                                   void *Param);
typedef IPVertexStruct *(*IPForEachVertexCallBack)(IPVertexStruct *V, 
                                                   void *Param);

#define IRIT_TEXT_DATA_FILE	    "itd"
#define IRIT_BINARY_DATA_FILE	    "ibd"
#define IRIT_COMPRESSED_DATA_FILE   "icd"
#define IRIT_MATRIX_DATA_FILE	    "imd"
#define STL_BINARY_DATA_FILE	    "bstl"

#define IRIT_TEXT_DATA_WFILE	    L"itd"
#define IRIT_BINARY_DATA_WFILE	    L"ibd"
#define IRIT_COMPRESSED_DATA_WFILE  L"icd"
#define IRIT_MATRIX_DATA_WFILE	    L"imd"
#define STL_BINARY_DATA_WFILE	    L"bstl"

typedef IrtPtType IPMshBoxXYZType[2];

typedef struct IPMshNodeStruct {
    unsigned long Tag;
    double x;
    double y;
    double z;
    double u;
    double v;
    double w;
}IPMshNodeStruct;

typedef struct IPMshCellStruct {
    int             Tag;
    int             NumNodes;
    int	            CellType;
    int	            MaterialID;
    unsigned int    ElmNumber;
    int *NodeTags;
}IPMshCellStruct;

typedef struct IPMshNodeBlockDataStruct {
    int		    Tag;
    unsigned int    Dim;
    unsigned long   NumNodes;
    int             Parametric;
    IPMshNodeStruct *Nodes;
} IPMshNodeBlockDataStruct;

typedef struct IPMshCellBlockDataStruct {
    int		    Tag;
    unsigned int    Dim;
    unsigned long   NumCells;
    int             ElementType;
    IPMshCellStruct *Cells;
} IPMshCellBlockDataStruct;

typedef struct IPMshEntitiesDataStruct {
    unsigned long    NumEntities;
    int              *EntitiesTags;
    unsigned int     *NumPhysicals;
    int              **PhysicalTags;
    IPMshBoxXYZType  *BoxXYZ;
} IPMshEntitiesDataStruct;

typedef struct IPMshPhysicalGroupDataStruct {
    int  Dim;
    int  Tag;
    char *Name;
} IPMshPhysicalGroupDataStruct;

typedef struct IPMshDataStruct {
    float                           Version;
    unsigned int                    FileType;
    unsigned int                    DataSize;
    IPMshEntitiesDataStruct         Points;
    IPMshEntitiesDataStruct         Curves;
    IPMshEntitiesDataStruct         Surfaces;
    IPMshEntitiesDataStruct         Volumes;
    unsigned int                    NumPhysicalGroups;
    IPMshPhysicalGroupDataStruct    *PhysicalGroups;
    unsigned int                    NumNodeBlocks;
    IPMshNodeBlockDataStruct        *NodeBlocks;
    unsigned int                    NumCellBlocks;
    IPMshCellBlockDataStruct        *CellBlocks;
    unsigned int                    NumNodes;
    unsigned int                    MinNodeTag;
    unsigned int                    MaxNodeTag;
    unsigned int                    NumCells;
    unsigned int                    MinCellTag;
    unsigned int                    MaxCellTag;
} IPMshDataStruct;

typedef struct IPIgesLoadDfltFileParamsStruct {
    int ClipTrimmedSrf;
    int DumpAll;
    int IgnoreGrouping;
    int ApproxConversion;
    int InverseProjCrvOnSrfs;
    int Messages;
} IPIgesLoadDfltFileParamsStruct;

typedef struct IPSTLLoadDfltFileParamsStruct {
    int BinarySTL;
    int EndianSwap;
    int NormalFlip;
    int Messages;
} IPSTLLoadDfltFileParamsStruct;

typedef struct IPOBJLoadDfltFileParamsStruct {
    int WarningMsgs;
    int WhiteDiffuseTexture;
    int IgnoreFullTransp;
    int ForceSmoothing;
} IPOBJLoadDfltFileParamsStruct;

typedef struct IPGcodeLoadDfltFileParamsStruct {
    int ArcCentersRelative;
    int Messages;
} IPGcodeLoadDfltFileParamsStruct;

typedef enum {
    IRIT_EPS_DRAW_POINT_CROSS,
    IRIT_EPS_DRAW_POINT_FULL_CIRC,
    IRIT_EPS_DRAW_POINT_HOLLOW_CIRC
} IPEPSSaveDrawPtType;

typedef struct IPEPSSaveDfltFileParamsStruct {
    const char *PSTitle;
    const char *FontName;
    const char *FileName;
    int BackGround;
    int ColorPS;
    int Messages;
    int BBoxClip;
    int WidenPolyEnds;
    IrtRType WidenEndLength;
    IrtRType WidenEndWidthScale;
    int BackGroundColor[3];
    IrtRType BBoxClipDomain[4];
    IrtRType LineWidth;
    IrtRType PointScale;
    int DepthCue;
    int DepthCueGray;
    IrtRType DepthCueZ[2];
    IrtRType PrintSize;
    IPEPSSaveDrawPtType DrawPoint;

    /* Internal data. */
    IrtRType LastScale;
    IrtHmgnMatType _CrntMat;
    GMBBBboxStruct _BBox;
    FILE *_OutputFile;
} IPEPSSaveDfltFileParamsStruct;

/* Gets lists of all freeform curves/(trimmed/triangular) surfaces/          */
/* trivariates in the datafile, process them as needed.			     */
/*   May return a processed version to be put on returned list from          */
/* IPGetObjects (polygonal approximation of the free form data for           */
/* example), or NULL otherwise.						     */
/*   This function is responsible to free the freeform data given if not     */
/* needed any more.							     */
/*   Is function is a call back function that must be provided by the using  */
/* application.	A default function will just concat the data into one list.  */
typedef struct IPFreeFormStruct {
    IPObjectStruct *CrvObjs;
    IPObjectStruct *SrfObjs;
    IPObjectStruct *TrimSrfObjs;
    IPObjectStruct *TrivarObjs;
    IPObjectStruct *TriSrfObjs;
    IPObjectStruct *ModelObjs;
    IPObjectStruct *MultiVarObjs;
    IPObjectStruct *VModelObjs;
} IPFreeFormStruct;

typedef struct IPTraverseObjHierarchyStruct {
    int TraverseObjCopy;
    int TraverseObjAll;
    int TraverseInvisibleObjs;
    int PrintInstance;
    IrtHmgnMatType Mat;
    IPApplyObjFuncType ApplyFunc;
} IPTraverseObjHierarchyStruct;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

IRIT_GLOBAL_DATA_HEADER IPIgesLoadDfltFileParamsStruct IPIgesLoadDfltParams;
IRIT_GLOBAL_DATA_HEADER IPSTLLoadDfltFileParamsStruct IPSTLLoadDfltParams;
IRIT_GLOBAL_DATA_HEADER IPOBJLoadDfltFileParamsStruct IPOBJLoadDfltParams;
IRIT_GLOBAL_DATA_HEADER IPGcodeLoadDfltFileParamsStruct IPGcodeLoadDfltParams;

/* Will be set to VIEW_MAT and PERS_MAT respectively if found in parsed data.*/
IRIT_GLOBAL_DATA_HEADER IrtHmgnMatType IPViewMat, IPPrspMat;
IRIT_GLOBAL_DATA_HEADER int IPWasViewMat, IPWasPrspMat;

const char *IPGetDataFileType(const char *FileName, int *Compressed);

/* Different data file types support: IGES. */
void IPIgesLoadFileSetDefaultParameters(int ClipTrimmedSrf,
					int DumpAll,
					int IgnoreGrouping,
					int ApproxCoversion,
					int InverseProjCrvOnSrfs,
					int Messages);
IPObjectStruct *IPIgesLoadFile(const char *IgesFileName,
			       const IPIgesLoadDfltFileParamsStruct *Params);
int IPIgesSaveFile(const IPObjectStruct *PObj,
		   IrtHmgnMatType CrntViewMat,
		   const char *IgesFileName,
		   int Messages);
int IPIgesSaveEucTrimCrvs(int SaveEucTrimCrvs);

/* Different data file types support: (Encapsulated) Post Script. */
int IPEPSSaveFile(IPObjectStruct *PObjects,
		  IrtHmgnMatType CrntMat,
		  IPEPSSaveDfltFileParamsStruct *EPSParams);

/* Different data file types support: MSH - gmsh finite element data. */
int IPMshSaveFile(const IPObjectStruct *PObj,
		  const char *MSHFileName,
		  int Messages,
		  int *UVWSamples,
		  IrtRType Eps,
		  CagdBType MixedElements,
		  CagdBType Quads);
int IPMshCnvrtIrit2Mesh(const IPObjectStruct *PObj,
			IPMshDataStruct *MeshData,
			int Messages,
			int *UVWSamples,
			IrtRType Eps,
			CagdBType MixedElements,
			CagdBType Quads);
void IPMshFreeMesh(IPMshDataStruct *MeshData);

/* Different data file types support: STL. */
void IPSTLLoadFileSetDefaultParameters(int BinarySTL,
				       int EndianSwap,
				       int NormalFlip,
				       int Messages);
IPObjectStruct *IPSTLLoadFile(const char *STLFileName,
			      const IPSTLLoadDfltFileParamsStruct *Params);
int IPSTLSaveFile(const IPObjectStruct *PObj,
		  IrtHmgnMatType CrntViewMat,
		  int RegularTriang,
		  int ReverseVertices,
		  int MultiObjSplit,
		  const char *STLFileName,
		  int Messages);
IrtRType IPSTLSaveSetVrtxEps(IrtRType SameVrtxEps);

/* Different data file types support: OBJ Wavefront. */
void IPOBJLoadFileSetDefaultParameters(int WarningMsgs,
				       int WhiteDiffuseTexture,
				       int IgnoreFullTransp,
				       int ForceSmoothing);
IPObjectStruct *IPOBJLoadFile(const char *OBJFileName,
			      const IPOBJLoadDfltFileParamsStruct *Params);
int IPOBJSaveFile(const IPObjectStruct *PObj, 
                  const char *OBJFileName,
		  int WarningMsgs,
		  int CnvxTriangPolys,
                  int UniqueVertices);

/* Different data file types support: DXF. */
int IPDXFSaveFile(const IPObjectStruct *PObj,
		  const char *DXFFileName,
		  int DumpFreeForms);

#ifdef IRIT_HAVE_3MF_LIB
int IP3MFSaveFile(IPObjectStruct *PObj,
                  const char *OutFileName,
                  int WarningMsgs,
		  const char *Designer);
#endif /* IRIT_HAVE_3MF_LIB */

/* Different data file types support: HTML. */
int IPWGLSaveFile(IPObjectStruct *PObj,
		  const char *WGLFileName,
		  int HideCtrlBar,
		  int CanvasWidth,
		  int CanvasHeight,
		  int BkRGB[3],
		  int ShowWorldAxes,
		  int DisableDepthTest,
		  int EnablePicking,
		  int ShowModelAxes,
		  Irit2WglDrawModeType DrawMode,
		  int WorldTrans,
		  Irit2WglViewAngleType ViewAngle,
		  Irit2WglProjectionModeType ProjectionMode,
		  IrtRType AmbientRGB[3]);

/* Different data file types support: VRML. */
int IPOpenVrmlFile(const char *FileName, int Messages, IrtRType Resolution);
int IPOpenStreamFromVrml(FILE *f, int Read, int IsBinary, int IsPipe);
void IPPutVrmlObject(int Handler, IPObjectStruct *PObj, int Indent);
void IPPutVrmlViewPoint(int Handler, IrtHmgnMatType *Mat, int Indent);
int IPSetVrmlExternalMode(int On);

/* Save NC (Numerically Controlled) G-Code file. */
int IPNCGCodeSaveFile(const IPObjectStruct *PObj,
		      IrtHmgnMatType CrntViewMat,
		      const char *NCGCODEFileName,
		      int Messages,
		      int Units,
		      int OutputType);

/* Functions to parse NC G-Code files. */
void IPNCGCodeLoadFileSetDefaultParameters(int ArcCentersRelative,
					   int Messages);
IPObjectStruct *IPNCGCodeLoadFile(const char *NCGCODEFileName,
				const IPGcodeLoadDfltFileParamsStruct *Params);
VoidPtr IPNCGCodeParserInit(int ArcCentersRelative,
			    IrtRType DefFeedRate,
			    IrtRType DefSpindleSpeed,
			    int DefToolNumber,
			    int ReverseZDir,
			    int CoerceEndPoints,
			    IPNCGCodeParserErrorFuncType ErrorFunc);
VoidPtr IPNCGCodeParserParseLine(VoidPtr IPNCGCodes,
				 const char *NextLine,
				 int LineNum);
int IPNCGCodeParserDone(VoidPtr IPNCGCodes);
int IPNCGCodeParserNumSteps(VoidPtr IPNCGCodes);
IPNCGCodeLineStruct *IPNCGCodeParserSetStep(VoidPtr IPNCGCodes,
						int NewStep);
IPNCGCodeLineStruct *IPNCGCodeParserGetNext(VoidPtr IPNCGCodes);
IPNCGCodeLineStruct *IPNCGCodeParserGetPrev(VoidPtr IPNCGCodes);
void IPNCGCodeParserFree(VoidPtr IPNCGCodes);
IPObjectStruct *IPNCGCode2Geometry(VoidPtr IPNCGCodes);
IrtRType IPNCGCodeLength(VoidPtr IPNCGCodes, IrtRType *FastLength);
struct GMBBBboxStruct *IPNCGCodeBBox(VoidPtr IPNCGCodes, int IgnoreG0Fast);
IrtRType IPNCGCodeTraverseInit(VoidPtr IPNCGCodes,
			       IrtRType InitTime,
			       IrtRType FastSpeedUpFactor,
			       IrtRType TriggerArcLen);
int IPNCGCodeTraverseTriggerAAL(VoidPtr IPNCGCodes,
				IPNCGCodeEvalMRRFuncType EvalMRR,
				VoidPtr MRRData);
IrtRType IPNCGCodeTraverseTime(VoidPtr IPNCGCodes,
			       IrtRType Dt,
			       IrtRType *NewRealTime,
			       IrtPtType NewToolPosition,
			       IrtPtType NewToolOrientation,
			       IPNCGCodeLineStruct **NewGC);
IrtRType IPNCGCodeTraverseStep(VoidPtr IPNCGCodes,
			       IrtRType Step,
			       IrtRType *NewRealTime,
			       IrtPtType NewToolPosition,
			       IPNCGCodeLineStruct **NewGC);
CagdSrfStruct *IPNCGCodeGenToolGeom(IPNCGCToolType ToolType,
				    IrtRType Diameter,
				    IrtRType Height,
				    IrtRType TorusRadius,
				    CagdCrvStruct **ToolProfile,
				    CagdSrfStruct **ToolBottom);
CagdCrvStruct *IPNCUpdateCrvOffsetJoint(CagdCrvStruct *OrigCrv1,
					CagdCrvStruct *OrigCrv2,
					CagdCrvStruct **OffCrv1,
					CagdCrvStruct **OffCrv2);
int IPNCGCodeSave2File(VoidPtr IPNCGCodes, const char *FName);
const char *IPNCGCodeTraverseLines(VoidPtr IPNCGCodes, int Restart);
void IPNCGCodeResetFeedRates(VoidPtr IPNCGCodes);
int IPNCGCodeHasABC(VoidPtr IPNCGCodes);
IrtRType IPNCGCodeFastSpeedUpFactor(VoidPtr IPNCGCodes, IrtRType NewFactor);
IPNCGCodeIndexUpdateFuncType IPNCGCodeUpdateGCodeIndexCBFunc(
		 	                   VoidPtr IPNCGCodes,
					   IPNCGCodeIndexUpdateFuncType Func,
					   void *Data);

/* General IRIT data file processing. */

void IPPutAttributes(int Handler, const IPAttributeStruct *Attr, int Indent);
int IPPutMatrixFile(const char *File,
		    IrtHmgnMatType ViewMat,
		    IrtHmgnMatType ProjMat,
		    int HasProjMat);
int IPOpenDataFile(const char *FileName, int Read, int Messages);
int IPOpenStreamFromCallBackIO(IPStreamReadCharFuncType ReadFunc,
			       IPStreamWriteBlockFuncType WriteFunc,
			       IPStreamFormatType Format,
			       int Read,
			       int IsBinary);
int IPOpenStreamFromFile(FILE *f,
			 int Read,
			 int IsBinary,
			 int IsCompressed,
			 int IsPipe);
int IPOpenStreamFromFile2(FILE *f, 
			  int Read,
			  IPStreamFormatType Format,
			  int IsBinary,
			  int IsCompressed,
			  int IsPipe);
int IPOpenStreamFromSocket(int Soc, int IsBinary);
void IPCloseStream(int Handler, int Free);
IPObjectStruct *IPGetDataFiles(char const * const *DataFileNames,
			       int NumOfDataFiles,
			       int Messages,
			       int MoreMessages);
IPObjectStruct *IPGetDataWFiles(wchar_t const * const *DataFileNames,
				int NumOfDataFiles,
				int Messages,
				int MoreMessages);
IPObjectStruct *IPGetDataFromFilehandles(FILE **Files,
					 int NumOfFiles,
					 char **Extensions,
					 int Messages,
					 int MoreMessages);
IPObjectStruct *IPGetDataFromFilehandles2(FILE **Files,
					  int NumOfFiles,
					  IPStreamFormatType *Formats,
					  int *IsBinaryIndicators,
					  int Messages,
					  int MoreMessages);
IPObjectStruct *IPGetObjects(int Handler);
IPObjectStruct *IPResolveInstances(IPObjectStruct *PObjects);
IPStreamFormatType IPSenseFileType(const char *FileName);
int IPSenseBinaryFile(const char *FileName);
IPObjectStruct *IPProcessReadObject(IPObjectStruct *PObj);
IPObjectStruct *IPFlattenTree(IPObjectStruct *PObj);
IPObjectStruct *IPFlattenTreeProcessFF(IPObjectStruct *PObj);
IPObjectStruct *IPFlattenForrest(IPObjectStruct *PObjList, int ProcessFF);
IPObjectStruct *IPFlattenForrest2(IPObjectStruct *PObj, int ProcessFF);
void IPStdoutObject(const IPObjectStruct *PObj, int IsBinary);
void IPStderrObject(const IPObjectStruct *PObj);
void IPExportObjectToFile(const char *FName,
			  const IPObjectStruct *PObj,
			  IPStreamFormatType FType);
void IPPutObjectToFile(FILE *f, const IPObjectStruct *PObj, int IsBinary);
void IPPutObjectToFile2(FILE *f, const IPObjectStruct *PObj, int Indent);
void IPPutObjectToFile3(const char *FName,
			const IPObjectStruct *PObj,
			int Indent);
void IPPutObjectToHandler(int Handler, const IPObjectStruct *PObj);
void IPInputUnGetC(int Handler, char c);
int IPSetPolyListCirc(int Circ);
int IPSetFlattenObjects(int Flatten);
int IPSetPropagateAttrs(int Propagate);
int IPFlattenInvisibleObjects(int FlattenInvisib);
int IPSetReadOneObject(int OneObject);
IPProcessLeafObjType IPSetProcessLeafFunc(IPProcessLeafObjType ProcessLeafFunc);

IPPrintFuncType IPSetPrintFunc(IPPrintFuncType PrintFunc);
int IPSetFilterDegen(int FilterDegeneracies);
char *IPSetFloatFormat(const char *FloatFormat);
int IPGetRealNumber(const char *StrNum, IrtRType *RealNum);
int IPGetMatrixFile(const char *File,
		    IrtHmgnMatType ViewMat,
		    IrtHmgnMatType ProjMat,
		    int *HasProjMat);

const char *IPGetObjectTypeAsString(const IPObjectStruct *PObj);
const char *IPGetObjectTypeAsString2(IPObjStructType ObjType);

/* Binary stream functions. */
IPObjectStruct *IPGetBinObject(int Handler);
void IPPutBinObject(int Handler, const IPObjectStruct *PObj);

IPObjectStruct *IPProcessFreeForm(IPFreeFormStruct *FreeForms);
IPObjectStruct *IPConcatFreeForm(IPFreeFormStruct *FreeForms);
IPObjectStruct *IPEvalFreeForms(IPObjectStruct *PObj);
int IPProcessModel2TrimSrfs(IPFreeFormStruct *FreeForms);

/* Last previous and other element retrieval routines. */
IrtHmgnMatType *IPGetViewMat(int *WasViewMat);
IrtHmgnMatType *IPGetPrspMat(int *WasPrspMat);
int IPUpdatePolyPlane(IPPolygonStruct *PPoly);
int IPUpdatePolyPlane2(IPPolygonStruct *PPoly, const IrtVecType Vin);
void IPUpdateVrtxNrml(IPPolygonStruct *PPoly, IrtVecType DefNrml);
IPObjectStruct *IPReverseListObj(IPObjectStruct *ListObj);
IPObjectStruct *IPReverseObjList(IPObjectStruct *PObj);
IPPolygonStruct *IPReversePlList(IPPolygonStruct *PPl);
void IPReverseVrtxList(IPPolygonStruct *Pl);
IPVertexStruct *IPReverseVrtxList2(IPVertexStruct *PVrtx);
IPObjectStruct *IPGetObjectByName(const char *Name,
				  IPObjectStruct *PObjList,
				  int TopLevel);
void IPSetSubObjectName(IPObjectStruct *PListObj,
			int Index,
			const char *Name);

IPObjectStruct *IPGetLastObj(IPObjectStruct *OList);
IPObjectStruct *IPGetPrevObj(IPObjectStruct *OList, IPObjectStruct *O);
IPObjectStruct *IPAppendObjLists(IPObjectStruct *OList1,
				 IPObjectStruct *OList2);
IPObjectStruct *IPAppendListObjects(IPObjectStruct *ListObj1,
				    IPObjectStruct *ListObj2);

IPObjectStruct *IPObjLnkListToListObject(IPObjectStruct *ObjLnkList);
IPObjectStruct *IPLnkListToListObject(VoidPtr LnkList,
				      IPObjStructType ObjType);
IPObjectStruct *IPLinkedListToObjList(const IPObjectStruct *LnkList);
void *IPListObjToLinkedList(const IPObjectStruct *LObjs);
IPObjectStruct *IPListObjToLinkedList2(const IPObjectStruct *LObjs);
void *IPHierarchyObjToLinkedList(const IPObjectStruct *HObj,
				 IPObjStructType ObjType);
int IPHierarchyObjToVector(const IPObjectStruct *HObj,
			   IPObjStructType ObjType,
			   void ***Vec,
			   int ElmntSize);

IPPolygonStruct *IPGetLastPoly(IPPolygonStruct *PList);
IPPolygonStruct *IPGetPrevPoly(IPPolygonStruct *PList,
			       IPPolygonStruct *P);
IPPolygonStruct *IPAppendPolyLists(IPPolygonStruct *PList1,
				   IPPolygonStruct *PList2);
IPVertexStruct *IPGetLastVrtx(IPVertexStruct *VList);
IPVertexStruct *IPGetPrevVrtx(IPVertexStruct *VList, IPVertexStruct *V);
IPVertexStruct *IPAppendVrtxLists(IPVertexStruct *VList1,
				  IPVertexStruct *VList2);
int IPObjListLen(const IPObjectStruct *O);
int IPPolyListLen(const IPPolygonStruct *P);
int IPVrtxListLen(const IPVertexStruct *V);
void IPForEachPoly(IPObjectStruct *OList,
		   void (*CallBack) (IPPolygonStruct *));
void IPForEachVertex(IPObjectStruct *OList,
		     void (*CallBack) (IPVertexStruct *));
IPObjectStruct *IPForEachObj2(IPObjectStruct *OList, 
			      IPForEachObjCallBack CallBack,
			      void *Param);
IPPolygonStruct *IPForEachPoly2(IPPolygonStruct *PlList, 
                                IPForEachPolyCallBack CallBack,
                                void *Param);
IPVertexStruct *IPForEachVertex2(IPVertexStruct *VList, 
                                 IPForEachVertexCallBack CallBack,
                                 void *Param);
void IPTraverseObjHierarchyInitState(IPTraverseObjHierarchyStruct
				                               *TraversState);
void IPTraverseObjListHierarchy(IPObjectStruct *PObjList,
				IPTraverseObjHierarchyStruct *TraversState);
void IPTraverseObjListHierarchy1(IPObjectStruct *PObjList,
				 void *Data,
				 IPTraverseObjHierarchyStruct *TraversState);
void IPTraverseObjListHierarchy2(IPObjectStruct *PObjList,
				 void *Data,
				 IPTraverseObjHierarchyStruct *TraversState);
void IPTraverseObjHierarchy(IPObjectStruct *PObj,
			    IPObjectStruct *PObjList,
			    void *Data,
			    IPTraverseObjHierarchyStruct *TraversState);
void IPTraverseObjHierarchy1(IPObjectStruct *PObj,
			     void *Data,
			     IPTraverseObjHierarchyStruct *TraversState);

/* Coercion of objects. */

IPObjectStruct *IPCoerceGregoryToBezier(const IPObjectStruct *PObj);
IPObjectStruct *IPCoerceBezierToPower(const IPObjectStruct *PObj);
IPObjectStruct *IPCoercePowerToBezier(const IPObjectStruct *PObj);
IPObjectStruct *IPCoerceBezierToBspline(const IPObjectStruct *PObj);
IPObjectStruct *IPCoerceBsplineToBezier(const IPObjectStruct *PObj);
IPObjectStruct *IPCoerceTrimmedSrfToTrimmedBezier(const IPObjectStruct *PObj);
IPObjectStruct *IPCoerceTrimmedSrfToUnTrimmedBezier(const IPObjectStruct *PObj,
						    int ComposeE3);
CagdPointType IPCoerceCommonSpace(IPObjectStruct *PtObjList,
				  CagdPointType Type);
CagdPointType IPCoercePtsListTo(IPObjectStruct *PtObjList, CagdPointType Type);
IPObjectStruct *IPCoerceObjectPtTypeTo(const IPObjectStruct *PObj,
				       int NewType);
IPObjectStruct *IPCoerceObjectTo(const IPObjectStruct *PObj, int NewType);

IPObjectStruct *IPReverseObject(const IPObjectStruct *PObj);

/* Client Server - communication functions. */

/* Socket Read/Write routines. */
int IPSocWriteBlock(int Handler, void *Block, int BlockLen);
void IPSocWriteOneObject(int Handler, IPObjectStruct *PObj);
int IPSocReadCharNonBlock(int Handler);
char *IPSocReadLineNonBlock(int Handler);
IPObjectStruct *IPSocReadOneObject(int Handler);

/* Socket Server routines. */
int IPSocSrvrInit(void);
int IPSocSrvrListen(void);
void IPSocHandleClientEvent(int Handler, IPObjectStruct *PObj);

/* Socket Client routines. */
int IPSocClntInit(void);

/* Socket Communication with other processes. */
int IPSocExecAndConnect(const char *Program, int IsBinary);
int IPSocDisConnectAndKill(int Kill, int Handler);
void IPSocEchoInput(int Handler, int EchoInput);

/* Dat to IRT conversion. */
void IPCnvDataToIrit(const IPObjectStruct *PObjects);
void IPCnvDataToIritOneObject(const char *Indent,
			      const IPObjectStruct *PObject,
			      int Level);
void IPCnvDataToIritAttribs(const char *Indent,
			    const char *ObjName,
			    const IPAttributeStruct *Attr);
const char *IPCnvrtReal2Str(IrtRType R, char *StrBuffer);
IPPrintFuncType IPCnvSetPrintFunc(IPPrintFuncType CnvPrintFunc);
int IPCnvSetLeastSquaresFit(int MinLenFit, int Percent, IrtRType MaxError);
char IPCnvSetDelimitChar(char Delimit);
int IPCnvSetCompactList(int CompactList);
int IPCnvSetDumpAssignName(int DumpAssignName);
int *IPCnvPolyVrtxNeighbors(const IPPolyVrtxArrayStruct *PVIdx,
			    int VIdx,
			    int Ring);
IPVertexStruct *IPCnvFindAdjacentEdge(const IPPolyVrtxArrayStruct *PVIdx, 
				      int ThisPolyIdx,
				      int FirstVertexIndex, 
				      int SecondVertexIndex);
IPPolygonStruct *IPCnvFindAdjacentPoly(const IPPolyVrtxArrayStruct *PVIdx,
				       const IPVertexStruct *V,
				       const IPVertexStruct *VNext);
int IPCnvIsVertexBoundary(const IPPolyVrtxArrayStruct *PVIdx, int VertexIndex);
int IPCnvEstimateBndryVrtxPlaneNrml(const IPPolyVrtxArrayStruct *PVIdx,
				    int BndryVrtxIdx,
				    IrtVecType PlaneNrml);
IPPolyVrtxArrayStruct *IPCnvrtIritPolyToPolyVrtxArray(const IPObjectStruct
									*PObj,
						      int CalcPPolys,
						      int AttribMask);

/* Special objects' read and write functions. These functions are used to   */
/* read and write objects of other libraries to and from data files.        */

CagdCrvStruct *CagdCrvReadFromFile(const char *FileName,
				   char **ErrStr,
				   int *ErrLine);
CagdCrvStruct *CagdCrvReadFromFile2(int Handler, char **ErrStr, int *ErrLine);
int CagdCrvWriteToFile(const CagdCrvStruct *Crvs,
		       const char *FileName,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
int CagdCrvWriteToFile2(const CagdCrvStruct *Crvs,
			int Handler,
			int Indent,
			const char *Comment,
			char **ErrStr);
int CagdCrvWriteToFile3(const CagdCrvStruct *Crvs,
			FILE *f,
			int Indent,
			const char *Comment,
			char **ErrStr);

CagdSrfStruct *CagdSrfReadFromFile(const char *FileName,
				   char **ErrStr,
				   int *ErrLine);
CagdSrfStruct *CagdSrfReadFromFile2(int Handler, char **ErrStr, int *ErrLine);
int CagdSrfWriteToFile(const CagdSrfStruct *Srfs,
		       const char *FileName,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
int CagdSrfWriteToFile2(const CagdSrfStruct *Srfs,
			int Handler,
			int Indent,
			const char *Comment,
			char **ErrStr);
int CagdSrfWriteToFile3(const CagdSrfStruct *Srfs,
			FILE *f,
			int Indent,
			const char *Comment,
			char **ErrStr);
CagdCrvStruct *BzrCrvReadFromFile(const char *FileName,
				  char **ErrStr,
				  int *ErrLine);
CagdCrvStruct *BzrCrvReadFromFile2(int Handler,
				   CagdBType NameWasRead,
				   char **ErrStr,
				   int *ErrLine);
int BzrCrvWriteToFile(const CagdCrvStruct *Crvs,
		      const char *FileName,
		      int Indent,
		      const char *Comment,
		      char **ErrStr);
int BzrCrvWriteToFile2(const CagdCrvStruct *Crvs,
		       int Handler,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
CagdSrfStruct *BzrSrfReadFromFile(const char *FileName,
				  char **ErrStr,
				  int *ErrLine);
CagdSrfStruct *BzrSrfReadFromFile2(int Handler,
				   CagdBType NameWasRead,
				   char **ErrStr,
				   int *ErrLine);
int BzrSrfWriteToFile(const CagdSrfStruct *Srfs,
		      const char *FileName,
		      int Indent,
		      const char *Comment,
		      char **ErrStr);
int BzrSrfWriteToFile2(const CagdSrfStruct *Srfs,
		       int Handler,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
CagdCrvStruct *BspCrvReadFromFile(const char *FileName,
				  char **ErrStr,
				  int *ErrLine);
CagdCrvStruct *BspCrvReadFromFile2(int Handler,
				   CagdBType NameWasRead,
				   char **ErrStr,
				   int *ErrLine);
int BspCrvWriteToFile(const CagdCrvStruct *Crvs,
		      const char *FileName,
		      int Indent,
		      const char *Comment,
		      char **ErrStr);
int BspCrvWriteToFile2(const CagdCrvStruct *Crvs,
		       int Handler,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
CagdSrfStruct *BspSrfReadFromFile(const char *FileName,
				  char **ErrStr,
				  int *ErrLine);
CagdSrfStruct *BspSrfReadFromFile2(int Handler,
				   CagdBType NameWasRead,
				   char **ErrStr,
				   int *ErrLine);
int BspSrfWriteToFile(const CagdSrfStruct *Srfs,
		      const char *FileName,
		      int Indent,
		      const char *Comment,
		      char **ErrStr);
int BspSrfWriteToFile2(const CagdSrfStruct *Srfs,
		       int Handler,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);

TrivTVStruct *TrivTVReadFromFile(const char *FileName,
				 char **ErrStr,
				 int *ErrLine);
TrivTVStruct *TrivTVReadFromFile2(int Handler, char **ErrStr, int *ErrLine);
TrivTVStruct *TrivBzrTVReadFromFile(const char *FileName,
				    char **ErrStr,
				    int *ErrLine);
TrivTVStruct *TrivBzrTVReadFromFile2(int Handler,
				     CagdBType NameWasRead,
				     char **ErrStr,
				     int *ErrLine);
TrivTVStruct *TrivBspTVReadFromFile(const char *FileName,
				    char **ErrStr,
				    int *ErrLine);
TrivTVStruct *TrivBspTVReadFromFile2(int Handler,
				     CagdBType NameWasRead,
				     char **ErrStr,
				     int *ErrLine);
int TrivTVWriteToFile(const TrivTVStruct *TVs,
		      const char *FileName, 
		      int Indent,
		      const char *Comment,
		      char **ErrStr);
int TrivTVWriteToFile2(const TrivTVStruct *TVs,
		       int Handler,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
int TrivTVWriteToFile3(const TrivTVStruct *TVs,
		       FILE *f,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
int TrivBzrTVWriteToFile(const TrivTVStruct *TVs,
			 const char *FileName,
			 int Indent,
			 const char *Comment,
			 char **ErrStr);
int TrivBzrTVWriteToFile2(const TrivTVStruct *TVs,
			  int Handler,
			  int Indent,
			  const char *Comment,
			  char **ErrStr);
int TrivBspTVWriteToFile(const TrivTVStruct *TVs,
			 const char *FileName,
			 int Indent,
			 const char *Comment,
			 char **ErrStr);
int TrivBspTVWriteToFile2(const TrivTVStruct *TVs,
			  int Handler,
			  int Indent,
			  const char *Comment,
			  char **ErrStr);

TrimSrfStruct *TrimReadTrimmedSrfFromFile(const char *FileName,
					  char **ErrStr,
					  int *ErrLine);
TrimSrfStruct *TrimReadTrimmedSrfFromFile2(int Handler,
					   CagdBType NameWasRead,
					   char **ErrStr,
					   int *ErrLine);
int TrimWriteTrimmedSrfToFile(const TrimSrfStruct *TrimSrfs,
			      const char *FileName,
			      int Indent,
			      const char *Comment,
			      char **ErrStr);
int TrimWriteTrimmedSrfToFile2(const TrimSrfStruct *TrimSrfs,
			       int Handler,
			       int Indent,
			       const char *Comment,
			       char **ErrStr);
int TrimWriteTrimmedSrfToFile3(const TrimSrfStruct *TrimSrfs,
			       FILE *f,
			       int Indent,
			       const char *Comment,
			       char **ErrStr);
TrngTriangSrfStruct *TrngTriSrfReadFromFile(const char *FileName,
					    char **ErrStr,
					    int *ErrLine);
TrngTriangSrfStruct *TrngTriSrfReadFromFile2(int Handler,
					     char **ErrStr,
					     int *ErrLine);
TrngTriangSrfStruct *TrngBzrTriSrfReadFromFile(const char *FileName,
					       char **ErrStr,
					       int *ErrLine);
TrngTriangSrfStruct *TrngBzrTriSrfReadFromFile2(int Handler,
						CagdBType NameWasRead,
						char **ErrStr,
						int *ErrLine);
TrngTriangSrfStruct *TrngBspTriSrfReadFromFile(const char *FileName,
					       char **ErrStr,
					       int *ErrLine);
TrngTriangSrfStruct *TrngBspTriSrfReadFromFile2(int Handler,
						CagdBType NameWasRead,
						char **ErrStr,
						int *ErrLine);
TrngTriangSrfStruct *TrngGrgTriSrfReadFromFile(const char *FileName,
					       char **ErrStr,
					       int *ErrLine);
TrngTriangSrfStruct *TrngGrgTriSrfReadFromFile2(int Handler,
						CagdBType NameWasRead,
						char **ErrStr,
						int *ErrLine);
int TrngTriSrfWriteToFile(const TrngTriangSrfStruct *TriSrfs,
			  const char *FileName,
			  int Indent,
			  const char *Comment,
			  char **ErrStr);
int TrngTriSrfWriteToFile2(const TrngTriangSrfStruct *TriSrfs,
			   int Handler,
			   int Indent,
			   const char *Comment,
			   char **ErrStr);
int TrngTriSrfWriteToFile3(const TrngTriangSrfStruct *TriSrfs,
			   FILE *f,
			   int Indent,
			   const char *Comment,
			   char **ErrStr);
int TrngBzrTriSrfWriteToFile(const TrngTriangSrfStruct *TriSrfs,
			     const char *FileName,
			     int Indent,
			     const char *Comment,
			     char **ErrStr);
int TrngBzrTriSrfWriteToFile2(const TrngTriangSrfStruct *TriSrfs,
			      int Handler,
			      int Indent,
			      const char *Comment,
			      char **ErrStr);
int TrngBspTriSrfWriteToFile(const TrngTriangSrfStruct *TriSrfs,
			     const char *FileName,
			     int Indent,
			     const char *Comment,
			     char **ErrStr);
int TrngBspTriSrfWriteToFile2(const TrngTriangSrfStruct *TriSrfs,
			      int Handler,
			      int Indent,
			      const char *Comment,
			      char **ErrStr);
int TrngGrgTriSrfWriteToFile(const TrngTriangSrfStruct *TriSrfs,
			     const char *FileName,
			     int Indent,
			     const char *Comment,
			     char **ErrStr);
int TrngGrgTriSrfWriteToFile2(const TrngTriangSrfStruct *TriSrfs,
			      int Handler,
			      int Indent,
			      const char *Comment,
			      char **ErrStr);

MdlModelStruct *MdlReadModelFromFile(const char *FileName,
				     char **ErrStr,
				     int *ErrLine);
MdlModelStruct *MdlReadModelFromFile2(int Handler,
				      CagdBType NameWasRead,
				      char **ErrStr,
				      int *ErrLine);
int MdlWriteModelToFile(const MdlModelStruct *Models,
			const char *FileName,
			int Indent,
			const char *Comment,
			char **ErrStr);
int MdlWriteModelToFile2(const MdlModelStruct *Models,
			 int Handler,
			 int Indent,
			 const char *Comment,
			 char **ErrStr);
int MdlWriteModelToFile3(const MdlModelStruct *Models,
			 FILE *f,
			 int Indent,
			 const char *Comment,
			 char **ErrStr);

VMdlVModelStruct *VMdlReadModelFromFile(const char *FileName,
				        char **ErrStr,
				        int *ErrLine);
VMdlVModelStruct *VMdlReadModelFromFile2(int Handler,
					 CagdBType NameWasRead,
					 char **ErrStr,
					 int *ErrLine);
int VMdlWriteVModelToFile(const VMdlVModelStruct *VModels,
			  const char *FileName,
			  int Indent,
			  const char *Comment,
			  char **ErrStr);
int VMdlWriteVModelToFile2(const VMdlVModelStruct *VModels,
			   int Handler,
			   int Indent,
			   const char *Comment,
			   char **ErrStr);
int VMdlWriteVModelToFile3(const VMdlVModelStruct *VModels,
			   FILE *f,
			   int Indent,
			   const char *Comment,
			   char **ErrStr);

MvarMVStruct *MvarMVReadFromFile(const char *FileName,
				 char **ErrStr,
				 int *ErrLine);
MvarMVStruct *MvarMVReadFromFile2(int Handler, char **ErrStr, int *ErrLine);
MvarMVStruct *MvarBzrMVReadFromFile(const char *FileName,
				    char **ErrStr,
				    int *ErrLine);
MvarMVStruct *MvarBzrMVReadFromFile2(int Handler,
				     CagdBType NameWasRead,
				     char **ErrStr,
				     int *ErrLine);
MvarMVStruct *MvarBspMVReadFromFile(const char *FileName,
				    char **ErrStr,
				    int *ErrLine);
MvarMVStruct *MvarBspMVReadFromFile2(int Handler,
				     CagdBType NameWasRead,
				     char **ErrStr,
				     int *ErrLine);
int MvarMVWriteToFile(const MvarMVStruct *MVs,
		      const char *FileName,
		      int Indent,
		      const char *Comment,
		      char **ErrStr);
int MvarMVWriteToFile2(const MvarMVStruct *MVs,
		       int Handler,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
int MvarMVWriteToFile3(const MvarMVStruct *MVs,
		       FILE *f,
		       int Indent,
		       const char *Comment,
		       char **ErrStr);
int MvarBzrMVWriteToFile(const MvarMVStruct *MVs,
			 const char *FileName,
			 int Indent,
			 const char *Comment,
			 char **ErrStr);
int MvarBzrMVWriteToFile2(const MvarMVStruct *MVs,
			  int Handler,
			  int Indent,
			  const char *Comment,
			  char **ErrStr);
int MvarBspMVWriteToFile(const MvarMVStruct *MVs,
			 const char *FileName,
			 int Indent,
			 const char *Comment,
			 char **ErrStr);
int MvarBspMVWriteToFile2(const MvarMVStruct *MVs,
			  int Handler,
			  int Indent,
			  const char *Comment,
			  char **ErrStr);

/************************************************************************
* Compress/decompress binary file support.                              *
************************************************************************/

/* IPC quantization steps. */
#define IPC_QUANTIZATION_DEFAULT  0.0000001f
#define IPC_QUANTIZATION_NONE     1.0          /* Quantization is not used. */

#ifdef IPC_BIN_COMPRESSION

/* Compressed stream functions: */
void IpcSetQuantization(int Handler, float QntError);
int IpcCompressObjToFile(const char *FileName,
			 const IPObjectStruct *PObj,
			 float QntError);
int IpcCompressObj(int Handler, const IPObjectStruct *PObj);
IPObjectStruct *IpcDecompressObjFromFile(const char *FileName);
IPObjectStruct *IpcDecompressObj(int Handler);

#endif /* IPC_BIN_COMPRESSION */

int IPSenseCompressedFile(const char *FileName);

/******************************************************************************
* This macro is called when the library has detected an unrecoverable error.  *
* Default action is to call CagdFatalError, but you may want to reroute this  *
* to invoke your handler and recover yourself (by long jump for example).     *
******************************************************************************/
#define IP_FATAL_ERROR(MsgID)	IPFatalError(MsgID)

/* Error handling. */

IPSetErrorFuncType IPSetFatalErrorFunc(IPSetErrorFuncType ErrorFunc);
int IPHasError(const char **ErrorDesc);
const char *IPDescribeError(IPFatalErrorType ErrorNum);
void IPFatalError(IPFatalErrorType ErrorNum);

#ifdef DEBUG
void IPDbg(void);
void IPVrtxDbg(IPVertexStruct *V);
void IPPlDbg(IPPolygonStruct *Pl);
void IPDbgDspObj(IPObjectStruct *PObj);
void AttrDbg(const IPAttributeStruct *Attr);
#endif /* DEBUG */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif	/* IRIT_PRSR_H */
