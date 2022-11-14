/*****************************************************************************
*   "Irit" - the 3d (not only polygonal) solid modeller.		     *
*									     *
* Written by:  Gershon Elber				Ver 0.2, Mar. 1990   *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
*   Definitions, visible to others, of Boolean operation modules:	     *
*****************************************************************************/

#ifndef IRIT_BOOL_LIB_H
#define IRIT_BOOL_LIB_H

/* Boolean operations types: */
typedef enum {
    BOOL_OPER_OR = 1,
    BOOL_OPER_AND,
    BOOL_OPER_SUB,
    BOOL_OPER_NEG,
    BOOL_OPER_CUT,
    BOOL_OPER_MERGE,
    BOOL_OPER_SELF,
    BOOL_OPER_CONTOUR
} BoolOperType;

typedef enum {
    BOOL_ERR_NO_POLY_OBJ,
    BOOL_ERR_NO_BOOL_OP_SUPPORT,
    BOOL_ERR_NO_MATCH_POINT,
    BOOL_ERR_NO_ELMNT_TO_DEL,
    BOOL_ERR_SORT_INTER_LIST,
    BOOL_ERR_FIND_VERTEX_FAILED,
    BOOL_ERR_NO_COPLANAR_VRTX,
    BOOL_ERR_NO_OPEN_LOOP,
    BOOL_ERR_NO_NEWER_VERTEX,
    BOOL_ERR_NO_INTERSECTION,
    BOOL_ERR_LOOP_LESS_3_VRTCS,
    BOOL_ERR_NO_INVERSE_MAT,
    BOOL_ERR_ADJ_STACK_OF,
    BOOL_ERR_CIRC_VRTX_LST,
    BOOL_ERR_NO_2D_OP_SUPPORT,
    BOOL_ERR_NO_PLLN_MATCH,
    BOOL_ERR_DISJ_PROP_ERR,
    BOOL_ERR_EMPTY_POLY_OBJ,

    BOOL_ERR_UNDEFINE_ERR
} BoolFatalErrorType;

typedef struct Bool2DInterStruct {  /* Holds info. on 2D intersetion points. */
    struct Bool2DInterStruct *Pnext;
    struct IPVertexStruct *Poly1Vrtx, *Poly2Vrtx;/* Ptr to Pl1/2 inter. vrtx.*/
    struct IPVertexStruct *Poly1Vrtx2, *Poly2Vrtx2;      /* In share corners */
							    /* - two inters! */
    int DualInter;   /* If two intersections at the same location (corners). */
    IrtRType Param1, Param2;     /* Parametrization along the poly vertices. */
    IrtPtType InterPt;				/* Location of intersection. */
    IrtVecType Normal;			/* Estimated normal at intersection. */
} Bool2DInterStruct;

#define BOOL_DISJ_GET_INDEX(Pl)		Pl -> IAux2
#define BOOL_DISJ_SET_INDEX(Pl, Index)	Pl -> IAux2 = Index
#define BOOL_DISJ_RESET(Pl)		BOOL_DISJ_SET_INDEX(Pl, 0)

/* Dummy declarations to prevent compiler warnings later on. */
typedef struct IPVertexStruct *IPVertexStructBoolRef;
typedef struct IPPolygonStruct *IPPolygonStructBoolRef;
typedef struct IPObjectStruct *IPObjectStructBoolRef;

typedef void (*BoolFatalErrorFuncType)(BoolFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* Prototypes of the global routines in adjacency.c module: */
int BoolGenAdjacencies(struct IPObjectStruct *PObj);
void BoolClnAdjacencies(struct IPObjectStruct *PObj);
int BoolMarkDisjointParts(struct IPObjectStruct *PObj);
struct IPPolygonStruct *BoolGetDisjointPart(struct IPObjectStruct *PObj, 
					    int Index);
struct IPVertexStruct *BoolGetAdjEdge(struct IPVertexStruct *V);

/* Prototypes of global functions in bool-2d.c module: */
struct IPPolygonStruct *Boolean2D(struct IPPolygonStruct *Pl1,
				  struct IPPolygonStruct *Pl2,
				  BoolOperType BoolOper);
Bool2DInterStruct *Boolean2DComputeInters(struct IPPolygonStruct *Pl1,
					  struct IPPolygonStruct *Pl2,
					  int HandlePolygons,
					  int DetectIntr);
int BoolFilterCollinearities(struct IPPolygonStruct *Pl);

/* Prototype of the global functions in the Boolean operations module: */
struct IPObjectStruct *BooleanOR(struct IPObjectStruct *PObj1,
				 struct IPObjectStruct *PObj2);
struct IPObjectStruct *BooleanAND(struct IPObjectStruct *PObj1,
				  struct IPObjectStruct *PObj2);
struct IPObjectStruct *BooleanSUB(struct IPObjectStruct *PObj1,
				  struct IPObjectStruct *PObj2);
struct IPObjectStruct *BooleanNEG(struct IPObjectStruct *PObj);
struct IPObjectStruct *BooleanCUT(struct IPObjectStruct *PObj1,
				  struct IPObjectStruct *PObj2);
struct IPObjectStruct *BooleanMERGE(struct IPObjectStruct *PObj1,
				    struct IPObjectStruct *PObj2);
struct IPObjectStruct *BooleanSELF(struct IPObjectStruct *PObj);
struct IPObjectStruct *BooleanCONTOUR(struct IPObjectStruct *PObj,
				      IrtPlnType Pln);

typedef struct BooleanMultiCntrGenInfoStruct *BooleanMultiCntrGenInfoStructPtr;
BooleanMultiCntrGenInfoStructPtr BooleanMultiCONTOURInit(
						  struct IPObjectStruct *PObj,
						  int Axis);
struct IPObjectStruct *BooleanMultiCONTOUR(IrtRType CntrLevel,
				          BooleanMultiCntrGenInfoStructPtr GI);
void BooleanMultiCONTOURFree(BooleanMultiCntrGenInfoStructPtr GI);
 
struct IPPolygonStruct *BoolInterPolyPoly(struct IPPolygonStruct *Pl1,
					  struct IPPolygonStruct *Pl2);

void BoolDfltFatalError(BoolFatalErrorType ErrID);
BoolFatalErrorFuncType BoolSetFatalErrorFunc(BoolFatalErrorFuncType ErrFunc);
const char *BoolDescribeError(BoolFatalErrorType ErrorNum);

int BoolSetOutputInterCurve(int OutputInterCurve);
IrtRType BoolSetPerturbAmount(IrtRType PerturbAmount);
int BoolSetHandleCoplanarPoly(int HandleCoplanarPoly);
int BoolSetParamSurfaceUVVals(int HandleBoolParamSrfUVVals);
int BoolSetPolySortAxis(int PolySortAxis);

struct IPVertexStruct *BoolCutPolygonAtRay(struct IPPolygonStruct *Pl,
					   IrtPtType Pt);

#ifdef DEBUG
void BoolDebugPrintAdjacencies(struct IPObjectStruct *PObj);
#endif /* DEBUG */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_BOOL_LIB_H */
