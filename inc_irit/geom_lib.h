/******************************************************************************
* Geom_lib.h - header file of the geometry library.			      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber, June 1996.					      *
******************************************************************************/

#ifndef	IRIT_GEOM_LIB_H
#define IRIT_GEOM_LIB_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/misc_lib.h"
#include "inc_irit/miscattr.h"

typedef enum {
    GEOM_ERR_NO_OGL_SUPPORT,
    GEOM_ERR_OGL_NO_X_SERVER,
    GEOM_ERR_X_NO_OGL_SERVER,
    GEOM_ERR_X_NO_VISUAL,
    GEOM_ERR_X_NO_GCONTEXT,
    GEOM_ERR_CH_STACK_OVERFLOW,
    GEOM_ERR_CH_STACK_UNDERFLOW,
    GEOM_ERR_NO_INSTANCE_ORIGIN,
    GEOM_ERR_ANIM_MAT_OR_CRV,
    GEOM_ERR_UNKNOWN_ANIM_CRVS,
    GEOM_ERR_NO_ANIM_CRVS,
    GEOM_ERR_UNEQUAL_NUM_OF_POLYS,
    GEOM_ERR_UNEQUAL_NUM_OF_VRTXS,
    GEOM_ERR_TOO_MANY_ADJACENCIES,
    GEOM_ERR_NO_IRIT_PATH,
    GEOM_ERR_INVALID_FONT,
    GEOM_ERR_MSC_TOO_FEW_PTS,
    GEOM_ERR_MSC_COLIN_CIRC,
    GEOM_ERR_TRIANGLES_ONLY,
    GEOM_ERR_INVALID_POLYGON,
    GEOM_ERR_VRTX_MTCH_FAILED,
    GEOM_ERR_EXPCT_POLYHEDRA,
    GEOM_ERR_EXPCT_POLYLINE,
    GEOM_ERR_EXPCT_LIST_OBJ,
    GEOM_ERR_EXPCT_TWO_PTS,
    GEOM_ERR_PROJ_FAILED,
    GEOM_ERR_DECIM_BDRY_FAILED,
    GEOM_ERR_OPEN_OBJ_VOL_COMP,
    GEOM_ERR_NO_INV_MAT,
    GEOM_ERR_NO_POLY_PLANE,
    GEOM_ERR_NO_VRTX_NRML,
    GEOM_ERR_REGULAR_POLY,
    GEOM_ERR_REORIENT_STACK_OF,
    GEOM_ERR_DISJOINT_PARTS,
    GEOM_ERR_VRTX_MUST_HAVE_NRML,
    GEOM_ERR_MISS_VRTX_IDX,
    GEOM_ERR_CMPLX_T_JUNC,
    GEOM_ERR_TOL_INVALID,

    GEOM_ERR_UNDEFINE_ERR
} GeomFatalErrorType;

typedef enum {
    GM_FIT_OTHER = -1,
    GM_FIT_PLANE = 0,
    GM_FIT_SPHERE,
    GM_FIT_CYLINDER,
    GM_FIT_CIRCLE,
    GM_FIT_CONE,
    GM_FIT_TORUS
} GMFittingModelType;

typedef enum {
    GM_GEN_PRIM_POLYS = 0,
    GM_GEN_PRIM_SRFS,
    GM_GEN_PRIM_MDLS,
    GM_GEN_PRIM_TVS,
    GM_GEN_PRIM_TVS_SNGLR,
    GM_GEN_PRIM_VMDLS,
    GM_GEN_PRIM_VMDLS_SNGLR
} GMGenPrimType;

#define GM_FIT_MODEL_MAX_PARAM 10

/* Used by the Ray & Polygon intersection (Jordan theorem): */
typedef enum {
    GM_GEOM_BELOW_RAY = -1,
    GM_GEOM_ON_RAY = 0,
    GM_GEOM_ABOVE_RAY = 1
} GMGeomRayRelationType;

#define GM_ANIM_DEFAULT_FILE_NAME	"IAnim"
#define PRIM_MIN_RESOLUTION		3

#define GM_ANIM_NO_DEFAULT_TIME IRIT_INFNTY

#define GM_QUAT_COPY(SrcQ, DstQ) IRIT_GEN_COPY(DstQ, SrcQ, sizeof(GMQuatType))

typedef struct GMAnimationStruct {
    IrtRType
	StartT,		                     /* Starting time of animation. */
	FinalT,		                  /* Termination time of animation. */
	Dt,		                         /* Step size pf animation. */
	RunTime;		              /* Current time of animation. */
    int TwoWaysAnimation,   /* Should the animation bounce back and forth!? */
	SaveAnimationGeom,          /* Save animation geometry into files!? */
	SaveAnimationImage,           /* Save animation images into files!? */
	BackToOrigin,	           /* Should we terminate at the beginning? */
	NumOfRepeat,			            /* How many iterations? */
	StopAnim,		   /* If TRUE, must stop the animation now. */
	SingleStep,			     /* Are we in single step mode? */
	TextInterface,		/* Are we communicating using a textual UI? */
	MiliSecSleep,	  /* How many milliseconds to sleep between frames. */
	_Count;						/* Used internally. */
    const char *ExecEachStep;	      /* Program to execute each iteration. */
    char BaseFileName[IRIT_LINE_LEN];/* Base name of animation files saved. */
} GMAnimationStruct;

#define GM_BBOX_MAX_DIM 19

typedef struct GMBBBboxStruct {
    IrtRType Min[GM_BBOX_MAX_DIM];
    IrtRType Max[GM_BBOX_MAX_DIM];
    int Dim;                  /* Actual number of valid dimensions in bbox. */
} GMBBBboxStruct;

#define GM_BBOX_RESET(BBox)     IRIT_ZAP_MEM(&(BBox), sizeof(GMBBBboxStruct));

#define GM_BBOX3D_INIT(BBox) { \
    GM_BBOX_RESET((BBox)); \
    (BBox).Min[0] = (BBox).Min[1] = (BBox).Min[2] = IRIT_INFNTY; \
    (BBox).Max[0] = (BBox).Max[1] = (BBox).Max[2] = -IRIT_INFNTY; \
    (BBox).Dim = 3; }

#define GM_BBOX3D_MAX_SIZE(BBox) \
    IRIT_MAX((BBox).Max[0] - (BBox).Min[0], \
	     IRIT_MAX((BBox).Max[1] - (BBox).Min[1], \
		      (BBox).Max[2] - (BBox).Min[2]))

#define GM_BBOX3D_HOLD_PT(Pt, BBox, Eps) /* Point in BBox to within Eps. */ \
    ((Pt)[0] >= (BBox) -> Min[0] - (Eps) && \
     (Pt)[0] <= (BBox) -> Max[0] + (Eps) && \
     (Pt)[1] >= (BBox) -> Min[1] - (Eps) && \
     (Pt)[1] <= (BBox) -> Max[1] + (Eps) && \
     (Pt)[2] >= (BBox) -> Min[2] - (Eps) && \
     (Pt)[2] <= (BBox) -> Max[2] + (Eps))

#define GM_BBOX3D_SAME(BBox1, BBox2, Eps)  /* Check for BBox similarity. */ \
     ((IRIT_FABS((BBox1).Max[0] - (BBox2).Max[0]) < Eps) && \
      (IRIT_FABS((BBox1).Max[1] - (BBox2).Max[1]) < Eps) && \
      (IRIT_FABS((BBox1).Max[2] - (BBox2).Max[2]) < Eps) && \
      (IRIT_FABS((BBox1).Min[0] - (BBox2).Min[0]) < Eps) && \
      (IRIT_FABS((BBox1).Min[1] - (BBox2).Min[1]) < Eps) && \
      (IRIT_FABS((BBox1).Min[2] - (BBox2).Min[2]) < Eps)) \

#define GM_BBOX3D_OVERLAP_EPS(BBox1, BBox2, Eps)/* Checks if BBs overlap.*/ \
    (!((BBox1) -> Max[0] < (BBox2) -> Min[0] - Eps || \
       (BBox1) -> Max[1] < (BBox2) -> Min[1] - Eps || \
       (BBox1) -> Max[2] < (BBox2) -> Min[2] - Eps || \
       (BBox2) -> Max[0] < (BBox1) -> Min[0] - Eps || \
       (BBox2) -> Max[1] < (BBox1) -> Min[1] - Eps || \
       (BBox2) -> Max[2] < (BBox1) -> Min[2] - Eps))

#define GM_BBOX2_IN_BBOX1_EPS(BBox1, BBox2, Eps)   /* Containment of BBs. */ \
    ((BBox1) -> Min[0] < (BBox2) -> Min[0] + Eps && \
     (BBox1) -> Min[1] < (BBox2) -> Min[1] + Eps && \
     (BBox1) -> Min[2] < (BBox2) -> Min[2] + Eps && \
     (BBox1) -> Max[0] > (BBox2) -> Max[0] - Eps && \
     (BBox1) -> Max[1] > (BBox2) -> Max[1] - Eps && \
     (BBox1) -> Max[2] > (BBox2) -> Max[2] - Eps)


typedef IrtRType GMLsPoint[3];   /* The Z component is pretty much ignored. */

typedef struct GMLsLineSegStruct {
    struct GMLsLineSegStruct *Pnext;
    GMLsPoint Pts[2];
    long Id;			   /* Lines with unique ID never intersect. */
    VoidPtr PAux;	  /* Auxiliary back pointer - not used by ln_sweep. */
    struct GMLsIntersectStruct *Inters;
    GMLsPoint _MinVals;			        /* Bounding box on the line */
    GMLsPoint _MaxVals;
    GMLsPoint _Vec;		    /* A vector from first point to second. */
    IrtRType _ABC[3];			   /* Line equation as Ax + By + C. */
} GMLsLineSegStruct;

typedef struct GMLsIntersectStruct {
    struct GMLsIntersectStruct *Pnext;
    IrtRType t;
    IrtRType OtherT;
    struct GMLsLineSegStruct *OtherSeg;
    long Id;				      /* Unique ID of intersection. */
} GMLsIntersectStruct;

typedef struct GMBoxBVHInfoStruct {
    IrtRType Max[3];
    IrtRType Min[3];
    int Id;
} GMBoxBVHInfoStruct;

typedef struct GMFrustumSideStruct {
    IrtRType Corners[4][3];
    IrtRType Normal[3];
} GMFrustumSideStruct;

typedef struct GMGeneralFrustumInfoStruct {
    GMFrustumSideStruct Sides[6];
} GMGeneralFrustumInfoStruct;

/* Plane struct used to store the plane divsion metadata. */
typedef struct GMBspPtsSliceInfoStruct {
    struct GMBspPtsSliceInfoStruct *Pnext;
    struct IPAttributeStruct *Attr;
    IrtPlnType Pln;
    IrtPtType Pt;
    IrtVecType Normal, Tangent;
} GMBspPtsSliceInfoStruct;

/* The node of the Binary space partition tree.  */
typedef struct GMBspPtsNodeStruct {
    struct GMBspPtsNodeStruct *Pnext;
    struct IPAttributeStruct *Attr;
    struct IPVertexStruct *PC;
    struct GMBspPtsNodeStruct *Left, *Right;
    GMBspPtsSliceInfoStruct *DivisionSlice; 
} GMBspPtsNodeStruct;

typedef enum {            /* Predefined indices for the TransformIrtVecType */
    GM_QUAT_ROT_X = 0,
    GM_QUAT_ROT_Y,
    GM_QUAT_ROT_Z, 
    GM_QUAT_TRANS_X,
    GM_QUAT_TRANS_Y,
    GM_QUAT_TRANS_Z, 
    GM_QUAT_SCALE
} GMQuatTransformationsType;

typedef enum {
    GM_ZBUF_Z_LARGER,
    GM_ZBUF_Z_LARGER_EQUAL,
    GM_ZBUF_Z_SMALLER,
    GM_ZBUF_Z_SMALLER_EQUAL,
    GM_ZBUF_Z_ALWAYS,
    GM_ZBUF_Z_NEVER
} GMZTestsType;

/* Dummy declarations to prevent compiler warnings later on. */
typedef struct IPVertexStruct *IPVertexStructGMRef;
typedef struct IPPolygonStruct *IPPolygonStructGMRef;
typedef struct IPObjectStruct *IPObjectStructGMRef;
typedef struct IPPolyVrtxArrayStruct *IPPolyVrtxArrayStructGMRef;
typedef struct CagdPolylineStruct *CagdPolylineStructGMRef;

typedef	IrtRType GMQuatType[4];                            /* A Quaternion. */
typedef IrtRType GMQuatTransVecType[7];       /* Transformation parameters. */

typedef IrtRType (*GMPolyOffsetAmountFuncType)(IrtRType *Coord);
typedef void (*GeomSetErrorFuncType)(GeomFatalErrorType ErrorFunc);
typedef void (*GMZBufferUpdateFuncType)(VoidPtr ZbufferID, int x, int y);
typedef void (*GMScanConvertApplyFuncType)(int x, int y);
typedef void (*GMTransObjUpdateFuncType)(const struct IPObjectStruct *OldPObj,
					 struct IPObjectStruct *NewPObj,
					 IrtHmgnMatType Mat,
					 int AnimUpdate);
typedef IrtRType (*GMFetchVertexPropertyFuncType)(struct IPVertexStruct *V,
						  struct IPPolygonStruct *Pl,
						  void *AuxData);
typedef void (*GMSphConeQueryCallBackFuncType)(struct IPVertexStruct *V);
typedef int (*GMSphConeQueryDirFuncType)(IrtVecType Vec, IrtRType ConeAngle);
typedef void (*GMPolyAdjacncyVertexFuncType)(struct IPVertexStruct *V1,
					     struct IPVertexStruct *V2,
					     struct IPPolygonStruct *Pl1,
					     struct IPPolygonStruct *Pl2);
typedef struct IPObjectStruct *(*GMTransObjUpdateAnimCrvsFuncType)(
						struct IPObjectStruct *PAnim,
						IrtHmgnMatType Mat);
typedef int (*GMMergePolyVrtxCmpFuncType)(struct IPVertexStruct *V1,
					  struct IPVertexStruct *V2,
					  IrtRType Eps);
typedef void (*GMMergeGeomInitFuncType)(VoidPtr GenericData, VoidPtr Entty);
typedef IrtRType (*GMMergeGeomDistFuncType)(VoidPtr GenericData,
					    VoidPtr Entty1,
					    VoidPtr Entty2);
typedef IrtRType (*GMMergeGeomKeyFuncType)(VoidPtr GenericData, VoidPtr Entty);
typedef int (*GMMergeGeomMergeFuncType)(VoidPtr GenericData, 
					void **Entty1,
					void **Entty2);
typedef void (*GMIdentifyTJunctionFuncType)(struct IPVertexStruct *V0,
					    struct IPVertexStruct *V1,
					    struct IPVertexStruct *V2,
					    struct IPPolygonStruct *Pl0,
					    struct IPPolygonStruct *Pl1,
					    struct IPPolygonStruct *Pl2);
typedef IrtRType *(*GMPointDeformVrtxDirFuncType)(const struct IPVertexStruct
						                           *V,
						  void *UserData);
typedef IrtRType (*GMPointDeformVrtxFctrFuncType)(struct IPVertexStruct *V,
						  void *UserData);
typedef IrtRType (*GMQuadWeightFuncType)(const struct CagdPolylineStruct *P,
					 int *VIndices, 
					 int numV,
					 void *UserData);

/* Points equally spread over a sphere. */
#define GM_DFLT_SPHERE_CONE_DENSITY 300

IRIT_GLOBAL_DATA_HEADER IrtVecType const
    GMSphereCoverVectors4[],
    GMSphereCoverVectors20[],
    GMSphereCoverVectors50[],
    GMSphereCoverVectors100[],
    GMSphereCoverVectors130[];

IRIT_GLOBAL_DATA_HEADER int _PrimGlblResolution;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* And prototypes of the functions: */
IrtRType GMBasicSetEps(IrtRType Eps);
void GMVecCopy(IrtVecType Vdst, const IrtVecType Vsrc);
void GMVecNormalize(IrtVecType V);
IrtRType GMVecLength(const IrtVecType V);
int GMVecMinAbsValueIndex(const IrtVecType V);
int GMVecMaxAbsValueIndex(const IrtVecType V);
void GMVecCrossProd(IrtVecType Vres, const IrtVecType V1, const IrtVecType V2);
IrtRType GMVecVecAngle(const IrtVecType V1,
		       const IrtVecType V2,
		       int Normalize);
void GMSLerp(const IrtVecType V1,
	     const IrtVecType V2,
	     IrtRType t,
	     IrtVecType VOut);
IrtRType GMPlanarVecVecAngle(const IrtVecType V1,
			     const IrtVecType V2,
			     int Normalize);
int GMOrthogonalVector(const IrtVecType V, IrtVecType OV, int UnitLen);
int GMCollinear3Pts(const IrtPtType Pt1,
		    const IrtPtType Pt2,
		    const IrtPtType Pt3);
int GMCollinear3PtsInside(const IrtPtType Pt1,
			  const IrtPtType Pt2,
			  const IrtPtType Pt3);
int GMCoplanar4Pts(const IrtPtType Pt1,
		   const IrtPtType Pt2,
		   const IrtPtType Pt3,
		   const IrtPtType Pt4);
IrtRType GMVecDotProd(const IrtVecType V1, const IrtVecType V2);
IrtRType GMVecDotProdLen(const IrtRType *V1, const IrtRType *V2, int Len);
void GMVecReflectPlane(IrtVecType Dst, IrtVecType Src, IrtVecType PlaneNormal);

struct IPObjectStruct *GMGenMatObjectRotX(const IrtRType *Degree);
struct IPObjectStruct *GMGenMatObjectRotY(const IrtRType *Degree);
struct IPObjectStruct *GMGenMatObjectRotZ(const IrtRType *Degree);
struct IPObjectStruct *GMGenMatObjectTrans(const IrtVecType Vec);
struct IPObjectStruct *GMGenMatObjectScale(const IrtVecType Vec);
struct IPObjectStruct *GMGetMatTransPortion(
				     const struct IPObjectStruct *MatObj,
				     int TransPortion);
struct IPPolygonStruct *GMTransformPolyList(const struct IPPolygonStruct *Pls,
				     IrtHmgnMatType Mat,
				     int IsPolygon);
GMTransObjUpdateFuncType GMTransObjSetUpdateFunc(GMTransObjUpdateFuncType
								   UpdateFunc);
GMTransObjUpdateAnimCrvsFuncType GMTransObjSetAnimCrvUpdateFunc(
			      GMTransObjUpdateAnimCrvsFuncType AnimUpdateFunc);
struct IPObjectStruct *GMTransformObjectInPlace(struct IPObjectStruct *PObj,
						IrtHmgnMatType Mat);
struct IPObjectStruct *GMTransformObject(const struct IPObjectStruct *PObj,
					 IrtHmgnMatType Mat);
struct IPObjectStruct *GMTransformObjectList(const struct IPObjectStruct *PObj,
					     IrtHmgnMatType Mat);
struct IPObjectStruct *GMTransObjUpdateAnimCrvs(struct IPObjectStruct *PAnim,
						IrtHmgnMatType Mat);
struct IPObjectStruct *GMObjExplodeObject(const struct IPObjectStruct *PObj,
				          int ExplosionType,
				          const IrtVecType CenterOfExplosion,
				          IrtRType ExplosionAmount,
				          const IrtVecType LineExplodeDir,
				          int AnimCrvs);

int GMObjectNumCoordinates(const struct IPObjectStruct *Obj);
struct IPObjectStruct *GMGenMatObjectZ2Dir(const IrtVecType Dir);
struct IPObjectStruct *GMGenMatObjectZ2Dir2(const IrtVecType Dir,
					    const IrtVecType Dir2);
struct IPObjectStruct *GMGenMatObjectRotVec(const IrtVecType Vec,
					    const IrtRType *Degree);
struct IPObjectStruct *GMGenMatObjectV2V(const IrtVecType V1,
					 const IrtVecType V2);
struct IPObjectStruct *GMGenMatrix3Pts2EqltrlTri(const IrtPtType Pt1,
					  const IrtPtType Pt2,
					  const IrtPtType Pt3);

/* General basic computational geometry routines: */
IrtRType GMDistPointPoint(const IrtPtType P1, const IrtPtType P2);
int GMFindLinComb2Vecs(const IrtVecType V1,
		       const IrtVecType V2,
		       const IrtVecType V,
		       IrtRType w[2]);
int GMLineFrom2Points(IrtLnType Line,
		      const IrtPtType Pt1, 
		      const IrtPtType Pt2);
void GMPointVecFromLine(const IrtLnType Line, IrtPtType Pt, IrtVecType Dir);
int GMPlaneFrom3Points(IrtPlnType Plane,
		       const IrtPtType Pt1,
		       const IrtPtType Pt2,
		       const IrtPtType Pt3);
IrtRType GMPointFromPointLine(const IrtPtType Point,
			      const IrtPtType Pl,
			      const IrtPtType Vl,
			      IrtPtType ClosestPoint);
IrtRType GMDistPointLine(const IrtPtType Point,
			 const IrtPtType Pl,
			 const IrtPtType Vl);
IrtRType GMDistPointPlane(const IrtPtType Point, const IrtPlnType Plane);
int GMPointFromPointPlane(const IrtPtType Pt,
			  const IrtPlnType Plane,
			  IrtPtType ClosestPoint);
int GMVectorFromVectorPlane(const IrtVecType Vec,
			    const IrtVecType PlaneN,
			    IrtPtType ProjVec);
int GMPointFromLinePlane(const IrtPtType Pl,
			 const IrtPtType Vl,
			 const IrtPlnType Plane,
			 IrtPtType InterPoint,
			 IrtRType *t);
int GMPointFromLinePlane01(const IrtPtType Pl,
			   const IrtPtType Vl,
			   const IrtPlnType Plane,
			   IrtPtType InterPoint,
			   IrtRType *t);
int GMPointFromPlanarLineLine(const IrtPtType Pl1,
			      const IrtPtType Vl1,
			      const IrtPtType Pl2,
			      const IrtPtType Vl2,
			      IrtPtType Pi,
			      IrtRType *t1,
			      IrtRType *t2);
int GM2PointsFromLineLine(const IrtPtType Pl1,
			  const IrtPtType Vl1,
			  const IrtPtType Pl2,
			  const IrtPtType Vl2,
			  IrtPtType Pt1,
			  IrtRType *t1,
			  IrtPtType Pt2,
			  IrtRType *t2);
IrtRType GMDistLineLine(const IrtPtType Pl1,
			const IrtPtType Vl1,
			const IrtPtType Pl2,
			const IrtPtType Vl2);
int GMPointFrom3Planes(const IrtPlnType Pl1,
		       const IrtPlnType Pl2,
		       const IrtPlnType Pl3,
		       IrtPtType Pt);
int GMLineFrom2Planes(const IrtPlnType Pl1,
		       const IrtPlnType Pl2,
		       IrtPtType Pt,
		       IrtVecType Dir);

IrtRType GMDistPolyPt(const struct IPPolygonStruct *Pl,
		      IrtPtType Pt,
		      const struct IPVertexStruct **ExtremeV,
		      int MaxDist);
IrtRType GMDistPolyPt2(const struct IPPolygonStruct *Pl,
		       int IsPolyline,
		       IrtPtType Pt,
		       struct IPVertexStruct **ExtremeV,
		       int MaxDist);
IrtRType GMDistPolyPoly(const struct IPPolygonStruct *Pl1,
			const struct IPPolygonStruct *Pl2,
			struct IPVertexStruct **V1,
			struct IPVertexStruct **V2,
			int TagIgnoreV);

int GMPolygonPlaneInter(const struct IPPolygonStruct *Pl,
			const IrtPlnType Pln,
			IrtRType *MinDist);
int GMPolygonListPlaneInter(const struct IPPolygonStruct *Pl,
                            const IrtPlnType Pln);
int GMSplitPolygonAtPlane(struct IPPolygonStruct *Pl, const IrtPlnType Pln);
void GMPolygonGetCentroid(const struct IPPolygonStruct *Polyline, IrtPtType *Pt);
int GMSegmentRayInter(const IrtPtType v1, 
                      const IrtPtType v2, 
                      const IrtPtType Pt, 
                      const IrtPtType Dir, 
                      IrtPtType InterPt,
		      IrtRType Eps);

IrtRType GMPolyPlaneClassify(const struct IPPolygonStruct *Pl,
			     const IrtPlnType Pln);
int GMTrianglePointInclusion(const IrtRType *V1,
			     const IrtRType *V2,
			     const IrtRType *V3,
			     const IrtPtType Pt);
int GMPolygonPointInclusion(const struct IPPolygonStruct *Pl,
			    const IrtPtType Pt,
			    IrtRType OnBndryEps);
IrtRType GMAreaSphericalTriangle(const IrtVecType Dir1,
				 const IrtVecType Dir2,
				 const IrtVecType Dir3);
IrtRType GMAngleSphericalTriangle(const IrtVecType Dir,
				  const IrtVecType ODir1,
				  const IrtVecType ODir2);
int GMPolygonPointInclusion3D(const struct IPPolygonStruct *Pl,
			      const IrtPtType Pt);

int GMPolygonXYRayInter(const struct IPPolygonStruct *Pl,
		        const IrtPtType PtRay,
		        int RayAxes);
int GMPolygonXYRayInter2(const struct IPPolygonStruct *Pl,
		         const IrtPtType PtRay,
		         int RayAxes,
		         struct IPVertexStruct **FirstInterV,
		         IrtRType *FirstInterP,
		         IrtRType *AllInters);
int GMPolygonXYRayInter3D(const struct IPPolygonStruct *Pl,
			  const IrtPtType PtRay,
			  int RayAxes);
int GMPolylineRayInter3D(const struct IPPolygonStruct *Polyline,
		         const IrtPtType Pt,
		         const IrtVecType Dir,
		         IrtPtType InterPt);
struct IPPolygonStruct *GMPolyHierarchy2SimplePoly(
					    struct IPPolygonStruct *Root,
					    struct IPPolygonStruct *Islands);
void GMGenTransMatrixZ2Dir(IrtHmgnMatType Mat,
			   const IrtVecType Trans,
			   const IrtVecType Dir,
			   IrtRType Scale);
void GMGenMatrixX2Dir(IrtHmgnMatType Mat, const IrtVecType Dir);
void GMGenMatrixY2Dir(IrtHmgnMatType Mat, const IrtVecType Dir);
void GMGenMatrixZ2Dir(IrtHmgnMatType Mat, const IrtVecType Dir);
void GMGenTransMatrixZ2Dir2(IrtHmgnMatType Mat,
			    const IrtVecType Trans,
			    const IrtVecType Dir,
			    const IrtVecType Dir2,
			    IrtRType Scale);
void GMGenMatrixZ2Dir2(IrtHmgnMatType Mat,
		       const IrtVecType Dir,
		       const IrtVecType Dir2);
int GMMatFromPosDir(const IrtPtType Pos,
		    const IrtVecType Dir,
		    const IrtVecType UpDir,
		    IrtHmgnMatType M);
void GMGenMatrixRotVec(IrtHmgnMatType Mat,
		       const IrtVecType Vec,
		       IrtRType Angle);
void GMGenMatrixRotV2V(IrtHmgnMatType Mat,
		       const IrtVecType V1,
		       const IrtVecType V2);
int GMGenMatrix4Pts2Affine4Pts(const IrtPtType P0,
			       const IrtPtType P1,
			       const IrtPtType P2,
			       const IrtPtType P3,
			       const IrtPtType Q0,
			       const IrtPtType Q1,
			       const IrtPtType Q2,
			       const IrtPtType Q3,
			       IrtHmgnMatType Trans);
void GMGenProjectionMat(const IrtPlnType ProjPlane,
			const IrtRType EyePos[4],
			IrtHmgnMatType Mat);
void GMGenReflectionMat(const IrtPlnType ReflectPlane, IrtHmgnMatType Mat);
int GM3Pts2EqltrlTriMat(const IrtPtType Pt1Orig,
			const IrtPtType Pt2Orig,
			const IrtPtType Pt3Orig,
			IrtHmgnMatType Mat);
IrtRType *GMBaryCentric3Pts2DToData(const IrtPtType Pt1,
				    const IrtPtType Pt2,
				    const IrtPtType Pt3,
				    const IrtPtType Pt,
				    IrtVecType RetVal);
IrtRType *GMBaryCentric3PtsToData(const IrtPtType Pt1,
				  const IrtPtType Pt2,
				  const IrtPtType Pt3,
				  const IrtPtType Pt,
				  IrtVecType RetVal);
int GM2PointsFromCircCirc(const IrtPtType Center1,
			  IrtRType Radius1,
			  const IrtPtType Center2,
			  IrtRType Radius2,
			  IrtPtType Inter1,
			  IrtPtType Inter2);
int GM2PointsFromCircCirc3D(const IrtPtType Cntr1,
			    const IrtVecType Nrml1,
			    IrtRType Rad1,
			    const IrtPtType Cntr2,
			    const IrtVecType Nrml2,
			    IrtRType Rad2,
			    IrtPtType Inter1,
			    IrtPtType Inter2);
int GMCircleFrom3Points(IrtPtType Center,
			IrtRType *Radius,
			const IrtPtType Pt1,
			const IrtPtType Pt2,
			const IrtPtType Pt3);
int GMCircleFrom2Pts2Tans(IrtPtType Center,
			  IrtRType *Radius,
			  const IrtPtType Pt1,
			  const IrtPtType Pt2,
			  const IrtVecType Tan1,
			  const IrtVecType Tan2);
int GMCircleFromLstSqrPts(IrtPtType Center,
			  IrtRType *Radius,
			  const IrtPtType *Pts,
			  int PtsSize);
int GM2BiTansFromCircCirc(const IrtPtType Center1,
			  IrtRType Radius1,
			  const IrtPtType Center2,
			  IrtRType Radius2,
			  int OuterTans,
			  IrtPtType TanPts[2][2]);
int GM2TanLinesFromCircCirc(const IrtPtType Center1,
			    IrtRType Radius1,
			    const IrtPtType Center2,
			    IrtRType Radius2,
			    int OuterTans,
			    IrtLnType Tans[2]);
int GMIsPtInsideCirc(const IrtRType *Point,
		     const IrtRType *Center,
		     IrtRType Radius);
int GMIsPtOnCirc(const IrtRType *Point,
		 const IrtRType *Center,
		 IrtRType Radius);
IrtRType GMAreaOfTriangle(const IrtRType *Pt1,
			  const IrtRType *Pt2,
			  const IrtRType *Pt3);

/* Convex polygon - ray intersections in R3. */

int GMRayCnvxPolygonInter(const IrtPtType RayOrigin,
			  const IrtVecType RayDir,
			  const struct IPPolygonStruct *Pl,
			  IrtPtType InterPoint);
int GMPointInsideCnvxPolygon(const IrtPtType Pt,
			     const struct IPPolygonStruct *Pl);
int GMPointOnPolygonBndry(const IrtPtType Pt,
			  const struct IPPolygonStruct *Pl,
			  IrtRType Eps);

/* List of convex polygon - ray intersections in R3. */
int GMRayCnvxPolygonListInter(const IrtPtType RayOrigin,
		              const IrtVecType RayDir,
			      const struct IPPolygonStruct *Plgns,
		              IrtPtType InterPoint);

/* Polynomial solvers. */
int GMSolveQuadraticEqn(IrtRType A, IrtRType B, IrtRType *Sols);
int GMSolveQuadraticEqn2(IrtRType B,
			 IrtRType C,
			 IrtRType *RSols,
			 IrtRType *ISols);
int GMSolveCubicEqn(IrtRType A, IrtRType B, IrtRType C, IrtRType *Sols);
int GMSolveCubicEqn2(IrtRType A,
		     IrtRType B,
		     IrtRType C, 
		     IrtRType *RSols,
		     IrtRType *ISols);
int GMSolveQuarticEqn(IrtRType A,
		      IrtRType B,
		      IrtRType C,
		      IrtRType D, 
		      IrtRType *Sols);
void GMComplexRoot(IrtRType RealVal,
		   IrtRType ImageVal,
		   IrtRType *RealRoot,
		   IrtRType *ImageRoot);

/* Geometric properties routines: */
IrtRType GMPolyLength(const struct IPPolygonStruct *Pl);
int GMPolyCentroid(const struct IPPolygonStruct *Pl, IrtPtType Centroid);
IrtRType GMPolyObjectArea(const struct IPObjectStruct *PObj, int SingedArea);
IrtRType GMPolyOnePolyArea(const struct IPPolygonStruct *Pl, int SingedArea);
IrtRType GMPolyOnePolyXYArea(const struct IPVertexStruct *VHead, int SingedArea);

IrtRType GMPolyObjectVolume(const struct IPObjectStruct *PObj);
int GMInverseBilinearMap(const IrtPtType P00,
			 const IrtPtType P01,
			 const IrtPtType P10,
			 const IrtPtType P11,
			 const IrtPtType PtInt,
			 IrtPtType UV[2]);

/* Box BVH routines. */
struct GMBoxBVHStruct *GMBoxBVHCreate(const GMBoxBVHInfoStruct **Boxes,
				      int BoxNum, 
				      int Size);
void GMBoxBVHFree(struct GMBoxBVHStruct *BVH);
int GMBoxBVHGetBoxNum(const struct GMBoxBVHStruct *BVH);
int GMBoxBVHGetBoxIntersection(const struct GMBoxBVHStruct *BVH,
			       const IrtRType *Max,
			       const IrtRType *Min,
			       int *Res);
int GMBoxBVHGetFrustumIntersection(const struct GMBoxBVHStruct *BVH,
				   const IrtRType *Max,
				   const IrtRType *Min,
				   IrtRType Angle,
				   int *Res);
int GMBoxBVHTestFrustumIntersection(const struct GMBoxBVHStruct *BVH,
				    const IrtRType *Max,
				    const IrtRType *Min,
				    IrtRType Angle);
void GMBoxBVHConstructGeneralAlignedFrustum(
				    const IrtRType Center[3],
				    IrtRType SizeX,
				    IrtRType SizeY,
				    const IrtRType XAxis[3],
				    const IrtRType HeightAxis[3],
				    IrtRType Height,
				    IrtRType XAngle,
				    IrtRType YAngle,
				    GMGeneralFrustumInfoStruct *Frustum);
int GMBoxBVHGetGeneralAlignedFrustumIntersection(
					     const struct GMBoxBVHStruct *BVH,
					     const IrtRType Center[3],
					     IrtRType SizeX,
					     IrtRType SizeY,
					     const IrtRType XAxis[3],
					     const IrtRType HeightAxis[3],
					     IrtRType Height,
					     IrtRType XAngle,
					     IrtRType YAngle,
					     int *Res);
struct IPObjectStruct *GMBoxBVHDbgConstructGeneralAlignedFrustum2(
					    const IrtRType Center[3],
					    IrtRType SizeX,
					    IrtRType SizeY,
					    const IrtRType XAxis[3],
					    const IrtRType HeightAxis[3],
					    IrtRType Height,
					    IrtRType XAngle,
					    IrtRType YAngle);
int GMBoxBVHGetGeneralAlignedFrustumIntersection2(
			    const struct GMBoxBVHStruct *BVH,
			    const GMGeneralFrustumInfoStruct *Frustum,
			    int *Res);
IrtBType GMGetGeneralAlignedFrustumPtsIntersection2(
			    const GMGeneralFrustumInfoStruct *Frustum,
			    const IrtRType **Pts,
			    int NumPts,
			    IrtBType RationalPts,
			    IrtRType Tolerance);

/* Functions from sphere's cone distribution - Sph_Cone.c. */
const IrtVecType *GMSphConeGetPtsDensity(int *n);
VoidPtr GMSphConeQueryInit(struct IPObjectStruct *PObj, int n);
void GMSphConeQueryFree(VoidPtr SphCone);
void GMSphConeQueryGetVectors(VoidPtr SphConePtr,
			      IrtVecType Dir,
			      IrtRType Angle,
			      GMSphConeQueryCallBackFuncType SQFunc);
void GMSphConeQuery2GetVectors(VoidPtr SphConePtr,
			       GMSphConeQueryDirFuncType SQQuery,
			       GMSphConeQueryCallBackFuncType SQFunc);

/* Functions from the convex hull computation package. */

int GMConvexHull(IrtE2PtStruct *DTPts, int *NumOfPoints);
int GMMonotonePolyConvex(struct IPVertexStruct *VHead, int Cnvx);

/* Functions from the minimum spanning circle/sphere packages. */
int GMMinSpanCirc(IrtE2PtStruct *DTPts,
		  int NumOfPoints,
		  IrtE2PtStruct *Center,
		  IrtRType *Radius);
int GMMinSpanConeAvg(IrtVecType *DTVecs,
		     int VecsNormalized,
		     int NumOfPoints,
		     IrtVecType Center,
		     IrtRType *Angle);
int GMMinSpanCone(IrtVecType *DTVecs,
		  int VecsNormalized,
		  int NumOfPoints,
		  IrtVecType Center,
		  IrtRType *Angle);
int GMMinSpanSphere(IrtE3PtStruct *DTPts,
		    int NumOfPoints,
		    IrtE3PtStruct *Center,
		    IrtRType *Radius);
int GMSphereWith3Pts(IrtE3PtStruct *Pts,
		     IrtRType *Center,
		     IrtRType *RadiusSqr);
int GMSphereWith4Pts(IrtE3PtStruct *Pts,
		     IrtRType *Center,
		     IrtRType *RadiusSqr);

/* Functions to extract silhouette and boundary curves from polygonal data. */
VoidPtr GMSilPreprocessPolys(struct IPObjectStruct *PObj, int n);
int GMSilPreprocessRefine(VoidPtr PrepSils, int n);
struct IPObjectStruct *GMSilExtractSilDirect(struct IPObjectStruct *PObj,
					     IrtHmgnMatType ViewMat);
struct IPObjectStruct *GMSilExtractSilDirect2(struct IPObjectStruct *PObjReg,
					      IrtHmgnMatType ViewMat);
struct IPObjectStruct *GMSilExtractSil(VoidPtr PrepSils,
				       IrtHmgnMatType ViewMat);
struct IPObjectStruct *GMSilExtractDiscont(struct IPObjectStruct *PObjReg,
					   IrtRType MinAngle);
struct IPObjectStruct *GMSilExtractBndry(struct IPObjectStruct *PObj);
void GMSilProprocessFree(VoidPtr PrepSils);
int GMSilOrigObjAlive(int ObjAlive);

/* Functions from the animate package. */
void GMAnimResetAnimStruct(GMAnimationStruct *Anim);
void GMAnimGetAnimInfoText(GMAnimationStruct *Anim);
int GMAnimHasAnimation(const struct IPObjectStruct *PObjs);
int GMAnimHasAnimationOne(const struct IPObjectStruct *PObj);
int GMAnimAffineTransAnimation(const struct IPObjectStruct *PObjs,
			       IrtRType Trans,
			       IrtRType Scale);
int GMAnimAffineTransAnimationOne(const struct IPObjectStruct *PObj,
				  IrtRType Trans,
				  IrtRType Scale);
int GMAnimAffineTransAnimation2(const struct IPObjectStruct *PObjs,
				IrtRType Min,
				IrtRType Max);
int GMAnimAffineTransAnimationOne2(const struct IPObjectStruct *PObj,
				   IrtRType Min,
				   IrtRType Max);
void GMAnimFindAnimationTimeOne(GMAnimationStruct *Anim,
				const struct IPObjectStruct *PObj);
void GMAnimFindAnimationTime(GMAnimationStruct *Anim,
			     const struct IPObjectStruct *PObjs);
void GMAnimSaveIterationsToFiles(GMAnimationStruct *Anim,
				 struct IPObjectStruct *PObjs);
void GMAnimSaveIterationsAsImages(GMAnimationStruct *Anim,
				  struct IPObjectStruct *PObjs);
IrtRType GMExecuteAnimationEvalMat(struct IPObjectStruct *AnimationP,
				   IrtRType Time,
				   IrtHmgnMatType ObjMat);
void GMAnimDoAnimation(GMAnimationStruct *Anim, struct IPObjectStruct *PObjs);
int GMAnimSetReverseHierarchyMatProd(int ReverseHierarchyMatProd);
int GMAnimSetAnimInternalNodes(int AnimInternalNodes);
int GMAnimHasDispPropAttrib(const struct IPObjectStruct *PObj);
void GMAnimEvalAnimation(IrtRType t, struct IPObjectStruct *PObj);
void GMAnimEvalAnimationList(IrtRType t, struct IPObjectStruct *PObjList);
struct IPObjectStruct *GMAnimEvalObjAtTime(IrtRType t,
					   struct IPObjectStruct *PObj);
void GMAnimDoSingleStep(GMAnimationStruct *Anim, struct IPObjectStruct *PObjs);
int GMAnimCheckInterrupt(GMAnimationStruct *Anim);

/* Functions from the bbox package. */
int GMBBSetBBoxPrecise(int Precise);
GMBBBboxStruct *GMBBComputeBboxObject(const struct IPObjectStruct *PObj,
				      GMBBBboxStruct *Bbox,
				      int HandleInvisibelObjs);
GMBBBboxStruct *GMBBComputeBboxObjectList(const struct IPObjectStruct *PObj,
					  GMBBBboxStruct *Bbox,
					  int HandleInvisibelObjs);
const struct IPObjectStruct *GMBBSetGlblBBInstncObjList(
				   const struct IPObjectStruct *BBInstObjList);
GMBBBboxStruct *GMBBComputeOnePolyBbox(const struct IPPolygonStruct *PPoly,
				       GMBBBboxStruct *Bbox);
GMBBBboxStruct *GMBBComputePolyListBbox(const struct IPPolygonStruct *PPoly,
				        GMBBBboxStruct *Bbox);
GMBBBboxStruct *GMBBComputePointBbox(const IrtRType *Pt, GMBBBboxStruct *Bbox);
GMBBBboxStruct *GMBBMergeBboxTo(const GMBBBboxStruct *Bbox1,
			        const GMBBBboxStruct *Bbox2,
			        GMBBBboxStruct *MergedBbox);
GMBBBboxStruct *GMBBMergeBbox(GMBBBboxStruct *MergedBbox,
			      const GMBBBboxStruct *Bbox);

/* Functions from the convex polygons package. */
int GMConvexRaysToVertices(int RaysToVertices);
int GMConvexNormalizeNormal(int NormalizeNormals);
struct IPObjectStruct *GMConvexPolyObjectN(const struct IPObjectStruct *PObj,
					   int HandleNormals);
void GMConvexPolyObject(struct IPObjectStruct *PObj,
			int HandleNormals);
int GMIsConvexPolygon2(const struct IPPolygonStruct *Pl);
int GMIsConvexPolygon(struct IPPolygonStruct *Pl);

struct IPPolygonStruct *GMSplitNonConvexPoly(struct IPPolygonStruct *Pl,
					     int HandleNormals);
void GMGenRotateMatrix(IrtHmgnMatType Mat, const IrtVecType Dir);

/* Functions from general polygons to triangles package. */
struct IPPolygonStruct *GMTriangulatePolygon(const struct CagdPolylineStruct
					                                  *Pl);
struct IPPolygonStruct *GMTriangulatePolygon2(const struct IPPolygonStruct *Pl);
struct IPPolygonStruct *GMTriangulatePolygonList(const struct IPPolygonStruct
						                    *PlgnList);
/* Functions from general polygons to quads. */
struct IPPolygonStruct *GMQuadrangulatePolygon(const struct
						        CagdPolylineStruct *Pl,
					       GMQuadWeightFuncType WF,
					       void *UserData);
struct IPPolygonStruct *GMQuadrangulatePolygon2(const struct 
							   IPPolygonStruct *Pl,
						GMQuadWeightFuncType WF,
						void *UserData);
struct IPPolygonStruct *GMQuadrangulatePolygonList(const struct 
						     IPPolygonStruct *PlgnList,
						   GMQuadWeightFuncType WF,
						   void *UserData);
IrtRType GMQuadAreaPerimeterRatioWeightFunc(const struct CagdPolylineStruct *P,
				            const int *VIndices, 
				            int numV);

/* Functions from the normal/uv/rgb/etc. interpolation package. */
void GMUpdateVerticesByInterp(struct IPPolygonStruct *PlList,
			      const struct IPPolygonStruct *OriginalPl);
void GMUpdateVertexByInterp(struct IPVertexStruct *VUpdate,
			    const struct IPVertexStruct *V,
			    const struct IPVertexStruct *VNext,
			    int DoRgb,
			    int DoUV,
			    int DoNrml);
int GMCollinear3Vertices(const struct IPVertexStruct *V1,
			 const struct IPVertexStruct *V2,
			 const struct IPVertexStruct *V3);
int GMEvalWeightsVFromPl(const IrtRType *Coord,
			 const struct IPPolygonStruct *Pl,
			 IrtRType *Wgt);
void GMInterpVrtxNrmlBetweenTwo(struct IPVertexStruct *V,
				const struct IPVertexStruct *V1,
				const struct IPVertexStruct *V2);
void GMInterpVrtxNrmlBetweenTwo2(IrtPtType Pt,
				 IrtVecType Normal,
				 const struct IPVertexStruct *V1,
				 const struct IPVertexStruct *V2,
				 int Normalize);
int GMInterpVrtxNrmlFromPl(struct IPVertexStruct *V,
			   const struct IPPolygonStruct *Pl);
int GMInterpVrtxRGBBetweenTwo(struct IPVertexStruct *V,
			      const struct IPVertexStruct *V1,
			      const struct IPVertexStruct *V2);
int GMInterpVrtxRGBFromPl(struct IPVertexStruct *V,
			  const struct IPPolygonStruct *Pl);
int GMInterpVrtxUVBetweenTwo(struct IPVertexStruct *V,
			     const struct IPVertexStruct *V1,
			     const struct IPVertexStruct *V2);
int GMInterpVrtxUVFromPl(struct IPVertexStruct *V,
			 const struct IPPolygonStruct *Pl);
void GMBlendNormalsToVertices(struct IPPolygonStruct *PlList,
			      IrtRType MaxAngle);
void GMFixOrientationOfPolyModel(struct IPPolygonStruct *Pls);
void GMFixNormalsOfPolyModel(struct IPPolygonStruct *PlList, int TrustFixedPt);
void GMFixPolyNormals(struct IPObjectStruct *PObj, int TrustFixPt);

/* Functions from the line sweep package. */
void GMLineSweep(GMLsLineSegStruct **Lines);

/* Functions from the polygonal cleaning package. */
int GMTwoPolySameGeom(const struct IPPolygonStruct *Pl1,
		      const struct IPPolygonStruct *Pl2,
		      IrtRType Eps);
struct IPPolygonStruct *GMCleanUpDupPolys(struct IPPolygonStruct **PPolygons,
					  IrtRType Eps);
struct IPPolygonStruct *GMCleanUpPolygonList(
					   struct IPPolygonStruct **PPolygons,
					   IrtRType Eps);
struct IPPolygonStruct *GMCleanUpPolylineList(
				          struct IPPolygonStruct **PPolylines,
					  IrtRType Eps);
struct IPPolygonStruct *GMCleanUpPolylineList2(struct IPPolygonStruct
					                         *PPolylines);
int GMIsPolygonPlanar(const struct IPPolygonStruct *Pl, IrtRType Tol);
struct IPPolygonStruct *GMVerifyPolygonsPlanarity(struct IPPolygonStruct *Pls,
						  IrtRType Tol);
void GMVrtxListToCircOrLin(struct IPPolygonStruct *Pls, int DoCirc);
void GMVrtxListToCircOrLinDup(struct IPPolygonStruct *Pls, int DoCirc);
struct IPVertexStruct *GMFilterInteriorVertices(
					 struct IPVertexStruct *VHead,
					 IrtRType MinTol,
					 int n);
struct IPPolygonStruct *GMClipPolysAgainstPlane(struct IPPolygonStruct *PHead,
					 struct IPPolygonStruct **PClipped,
					 struct IPPolygonStruct **PInter,
					 IrtPlnType Plane);
struct IPVertexStruct *GMFindThirdPointInTriangle(
					   const struct IPPolygonStruct *Pl,
					   const struct IPVertexStruct *V,
					   const struct IPVertexStruct *VNext);

/* Functions from the points on polygonal objects package. */
int GMGetMaxNumVrtcsPoly(struct IPObjectStruct *PolyObj);
int GMPolyHasCollinearEdges(const struct IPPolygonStruct *Pl);
struct IPPolygonStruct *GMConvertPolyToTriangles(struct IPPolygonStruct *Pl);
struct IPObjectStruct *GMConvertPolysToNGons(
					  const struct IPObjectStruct *PolyObj,
					  int n,
					  int HandleNormals);
struct IPObjectStruct *GMConvertPolysToTriangles(const struct IPObjectStruct
						                    *PolyObj);
struct IPObjectStruct *GMConvertPolysToTriangles2(const struct IPObjectStruct
						                    *PolyObj);
struct IPObjectStruct *GMConvertPolysToTrianglesIntrrPt(struct IPObjectStruct
							            *PolyObj);
struct IPObjectStruct *GMConvertPolysToRectangles(struct IPObjectStruct
						                    *PolyObj);
struct IPPolygonStruct *GMLimitTrianglesEdgeLen(const struct IPPolygonStruct
						                     *OrigPls,
						IrtRType MaxLen);
void GMAffineTransUVVals(struct IPObjectStruct *PObj,
			 const IrtRType Scale[2],
			 const IrtRType Trans[2]);
void GMGenUVValsForPolys(const struct IPObjectStruct *PObj,
			 IrtRType UTextureRepeat,
			 IrtRType VTextureRepeat,
			 IrtRType WTextureRepeat,
			 int HasXYZScale);
int GMMergeSameGeometry(void **GeomEntities,
			int NumOfGEntities,
			IrtRType IdenticalEps,
			VoidPtr GenericData,
			GMMergeGeomInitFuncType InitFunc,
			GMMergeGeomDistFuncType DistSqrFunc,
			GMMergeGeomKeyFuncType *KeyFuncs,
			GMMergeGeomMergeFuncType MergeFunc);
int GMMergeGeometry(void **GeomEntities,
		    int NumOfGEntities,
		    IrtRType Eps,
		    IrtRType IdenticalEps,
		    VoidPtr GenericData,
		    GMMergeGeomInitFuncType InitFunc,
		    GMMergeGeomDistFuncType DistSqrFunc,
		    GMMergeGeomKeyFuncType *KeyFuncs,
		    GMMergeGeomMergeFuncType MergeFunc);
int *GMMergeFindSimilarPoints(IrtRType *VecPts,
			      int PtLen,
			      int VecLen,
			      IrtRType Tolerance);
struct IPPolygonStruct *GMMergeClosedLoopHoles(struct IPPolygonStruct *PlMain,
					       struct IPPolygonStruct *PClosedPls);
struct IPPolygonStruct *GMMergePolylines(struct IPPolygonStruct *Polys,
					 IrtRType Eps);
struct IPPolygonStruct *GMMatchPointListIntoPolylines(
					       struct IPObjectStruct *PtsList,
					       IrtRType MaxTol);
struct IPObjectStruct *GMPointCoverOfPolyObj(struct IPObjectStruct *PolyObj,
					     int n,
					     IrtRType *Dir,
					     char *PlAttr);
struct IPObjectStruct *GMRegularizePolyModel(const struct IPObjectStruct *PObj,
					     int SplitCollinear,
					     IrtRType MinRefineDist);
struct IPPolygonStruct *GMSplitPolysAtCollinearVertices(
						 struct IPPolygonStruct *Pls);
struct IPPolygonStruct *GMSplitPolyInPlaceAtVertex(
					    struct IPPolygonStruct *Pl,
					    struct IPVertexStruct *VHead);
struct IPPolygonStruct *GMSplitPolyInPlaceAt2Vertices(
					       struct IPPolygonStruct *Pl,
					       struct IPVertexStruct *V1,
					       struct IPVertexStruct *V2);

/* Functions from the polygonal offsets package. */
IrtRType GMPolyOffsetAmountDepth(const IrtRType *Coord);
struct IPPolygonStruct *GMPolyOffset(const struct IPPolygonStruct *Poly,
				     int IsPolygon,
				     IrtRType Ofst,
				     GMPolyOffsetAmountFuncType AmountFunc);
struct IPPolygonStruct *GMPolyOffset3D(const struct IPPolygonStruct *Poly,
				       IrtRType Ofst,
				       int ForceSmoothing,
				       IrtRType MiterEdges,
				       GMPolyOffsetAmountFuncType AmountFunc);

/* Functions from the primitive constructions' package. */
int PrimSetGeneratePrimType(int PolygonalPrimitive);
int PrimSetSurfacePrimitiveRational(int SurfaceRational);

struct IPObjectStruct *PrimGenBOXObject(const IrtVecType Pt,
					IrtRType WidthX,
					IrtRType WidthY,
					IrtRType WidthZ);
struct IPObjectStruct *PrimGenBOXWIREObject(const IrtVecType Pt,
					    IrtRType WidthX,
					    IrtRType WidthY,
					    IrtRType WidthZ);
struct IPObjectStruct *PrimGenGBOXObject(const IrtVecType Pt,
					 const IrtVecType Dir1,
					 const IrtVecType Dir2,
					 const IrtVecType Dir3);
struct IPObjectStruct *PrimGenCONEObject(const IrtVecType Pt,
					 const IrtVecType Dir,
					 IrtRType R,
					 int Bases);
struct IPObjectStruct *PrimGenCONE2Object(const IrtVecType Pt,
				   const IrtVecType Dir,
					  IrtRType R1,
					  IrtRType R2,
					  int Bases);
struct IPObjectStruct *PrimGenCYLINObject(const IrtVecType Pt,
					  const IrtVecType Dir,
					  IrtRType R,
					  int Bases);
struct IPObjectStruct *PrimGenSPHEREObject(const IrtVecType Center,
					   IrtRType R);
struct IPObjectStruct *PrimGenTORUSObject(const IrtVecType Center,
					  const IrtVecType Normal,
					  IrtRType Rmajor,
					  IrtRType Rminor);
struct IPObjectStruct *PrimGenPOLYDISKObject(const IrtVecType Nrml,
					     const IrtVecType Trns, 
					     IrtRType R);
struct IPObjectStruct *PrimGenPOLYGONObject(struct IPObjectStruct *PObjList,
					    int IsPolyline);
struct IPObjectStruct *PrimGenObjectFromPolyList(struct IPObjectStruct
						                   *PObjList);
struct IPObjectStruct *PrimGenCROSSECObject(const struct IPObjectStruct *PObj);
struct IPObjectStruct *PrimGenSURFREVObject(const struct IPObjectStruct *Cross);
struct IPObjectStruct *PrimGenSURFREVAxisObject(struct IPObjectStruct *Cross,
						const IrtVecType Axis);
struct IPObjectStruct *PrimGenSURFREV2Object(const struct IPObjectStruct
					                              *Cross,
					     IrtRType StartAngle,
					     IrtRType EndAngle);
struct IPObjectStruct *PrimGenSURFREV2AxisObject(struct IPObjectStruct *Cross,
						 IrtRType StartAngle,
						 IrtRType EndAngle,
						 const IrtVecType Axis);
struct IPObjectStruct *PrimGenEXTRUDEObject(const struct IPObjectStruct *Cross,
					    const IrtVecType Dir,
					    int Bases);
struct IPObjectStruct *PrimGenRULEDObject(const struct IPObjectStruct *Cross1,
					  const struct IPObjectStruct *Cross2);

struct IPPolygonStruct *PrimGenPolygon4Vrtx(const IrtVecType V1,
					    const IrtVecType V2,
					    const IrtVecType V3,
					    const IrtVecType V4,
					    const IrtVecType Vin,
					    int *VrtcsRvrsd,
					    struct IPPolygonStruct *Pnext);
struct IPPolygonStruct *PrimGenPolygon4Vrtx2(const IrtVecType V1,
					     const IrtVecType V2,
					     const IrtVecType V3,
					     const IrtVecType V4,
					     const IrtVecType Vin,
					     int *VrtcsRvrsd,
					     int *Singular,
				      struct IPPolygonStruct *Pnext);
struct IPPolygonStruct *PrimGenPolygon3Vrtx(const IrtVecType V1,
					    const IrtVecType V2,
					    const IrtVecType V3,
					    const IrtVecType Vin,
					    int *VrtcsRvrsd,
					    struct IPPolygonStruct *Pnext);

struct IPObjectStruct *PrimGenTransformController2D(const GMBBBboxStruct *BBox,
						    int HasRotation,
						    int HasTranslation,
						    int HasScale);
struct IPObjectStruct *PrimGenTransformController2DCrvs(
						 const GMBBBboxStruct *BBox);
struct IPObjectStruct *PrimGenTransformControllerSphere(
						 const GMBBBboxStruct *BBox, 
						 int HasRotation,
						 int HasTranslation,
						 int HasUniformScale,
						 IrtRType BoxOpacity,
						 IrtRType RelTesalate);
struct IPObjectStruct *PrimGenTransformControllerBox(
					      const GMBBBboxStruct *BBox, 
					      int HasRotation,
					      int HasTranslation,
					      int HasUniformScale,
					      IrtRType BoxOpacity,
					      IrtRType RelTesalate);
struct IPObjectStruct *PrimGenFrameController(IrtRType BBoxLen, 
				       IrtRType NLeverLen, 
				       IrtRType TLeverLen, 
				       const char *HandleName);
struct IPPolygonStruct *PrimGenPolyline4Vrtx(const IrtVecType V1,
				      const IrtVecType V2,
				      const IrtVecType V3,
				      const IrtVecType V4,
				      struct IPPolygonStruct *Pnext);

int PrimSetResolution(int Resolution);

/* Functions from the quaternions package. */
void GMQuatToMat(GMQuatType q, IrtHmgnMatType Mat);
void GMQuatMatToQuat(IrtHmgnMatType Mat, GMQuatType q);
void GMQuatRotationToQuat(IrtRType Xangle,
			  IrtRType Yangle, 
			  IrtRType Zangle,
			  GMQuatType q);
void GMQuatToRotation(GMQuatType q, IrtVecType *Angles, int *NumSolutions);
void GMQuatMul(GMQuatType q1, GMQuatType q2, GMQuatType QRes);
void GMQuatAdd(GMQuatType q1, GMQuatType q2, GMQuatType QRes);
int GMQuatIsUnitQuat(GMQuatType q);
void GMQuatNormalize(GMQuatType q);
void GMQuatInverse(GMQuatType SrcQ, GMQuatType DstQ);
void GMQuatRotateVec(IrtVecType OrigVec, GMQuatType RotQ, IrtVecType DestVec);
void GMQuatLog(GMQuatType SrcQ, IrtVecType DstVec);
void GMQuatExp(IrtVecType SrcVec, GMQuatType DstQ);
void GMQuatPow(GMQuatType MantisQ, IrtRType Expon, GMQuatType DstQ);
int GMQuatMatrixToAngles(IrtHmgnMatType Mat, IrtVecType *Vec);
void GMQuatMatrixToTranslation(IrtHmgnMatType Mat, IrtVecType Vec);
IrtRType GMQuatMatrixToScale(IrtHmgnMatType Mat);
int GMQuatMatrixToVector(IrtHmgnMatType Mat, GMQuatTransVecType TransVec);
void GMQuatVectorToMatrix(GMQuatTransVecType TransVec, IrtHmgnMatType Mat);
void GMQuatVecToScaleMatrix(GMQuatTransVecType TransVec,
			    IrtHmgnMatType ScaleMatrix);
void GMQuatVecToRotMatrix(GMQuatTransVecType TransVec,
			  IrtHmgnMatType RotMatrix);
void GMQuatVecToTransMatrix(GMQuatTransVecType TransVec,
			    IrtHmgnMatType TransMatrix);
void GMMatrixToTransform(IrtHmgnMatType Mat, 
			 IrtVecType S,
			 GMQuatType R,
			 IrtVecType T);

/* Functions from the spherical coverage package. */
struct IPObjectStruct *GMPointCoverOfUnitHemiSphere(IrtRType HoneyCombSize);

/* Functions from the software z buffer. */
VoidPtr GMZBufferInit(int Width, int Height);
void GMZBufferFree(VoidPtr ZbufferID);
void GMZBufferClear(VoidPtr ZbufferID);
void GMZBufferClearSet(VoidPtr ZbufferID, IrtRType Depth);
GMZTestsType GMZBufferSetZTest(VoidPtr ZbufferID, GMZTestsType ZTest);
GMZBufferUpdateFuncType GMZBufferSetUpdateFunc(VoidPtr ZbufferID,
					       GMZBufferUpdateFuncType
					                           UpdateFunc);
VoidPtr GMZBufferInvert(VoidPtr ZbufferID);
VoidPtr GMZBufferRoberts(VoidPtr ZbufferID);
VoidPtr GMZBufferLaplacian(VoidPtr ZbufferID);
IrtRType GMZBufferQueryZ(VoidPtr ZbufferID, int x, int y);
VoidPtr GMZBufferQueryInfo(VoidPtr ZbufferID, int x, int y);
IrtRType GMZBufferUpdatePt(VoidPtr ZbufferID, int x, int y, IrtRType z);
VoidPtr GMZBufferUpdateInfo(VoidPtr ZbufferID, int x, int y, VoidPtr Info);
void GMZBufferUpdateHLn(VoidPtr ZbufferID,
			int x1,
			int x2,
			int y,
			IrtRType z1,
			IrtRType z2);
void GMZBufferUpdateLine(VoidPtr ZbufferID,
			 int x1,
			 int y1,
			 int x2,
			 int y2,
			 IrtRType z1,
			 IrtRType z2);
void GMZBufferUpdateTri(VoidPtr ZbufferID,
			int x1,
			int y1,
			IrtRType z1,
			int x2,
			int y2,
			IrtRType z2,
			int x3,
			int y3,
			IrtRType z3);

/* Functions from the z buffer based on Open GL package. */
IritIntPtrSizeType GMZBufferOGLInit(int Width,
				    int Height,
				    IrtRType ZMin,
				    IrtRType ZMax,
				    int OffScreen);
void GMZBufferOGLClear(void);
void GMZBufferOGLSetColor(int Red, int Green, int Blue);
void GMZBufferOGLMakeActive(IritIntPtrSizeType Id);
IrtRType GMZBufferOGLQueryZ(IrtRType x, IrtRType y);
void GMZBufferOGLQueryColor(IrtRType x,
			    IrtRType y,
			    int *Red,
			    int *Green,
			    int *Blue);
void GMZBufferOGLFlush(void);

/* Functions to fit analytic functions to point data sets. */
IrtPtType *GMSrfBilinearFit(IrtPtType *ParamDomainPts,
			    IrtPtType *EuclideanPts,
			    int FirstAtOrigin,
			    int NumPts,
			    IrtPtType *FitPts);
IrtPtType *GMSrfQuadricFit(IrtPtType *ParamDomainPts,
			   IrtPtType *EuclideanPts,
			   int FirstAtOrigin,
			   int NumPts,
			   IrtPtType *FitPts);
IrtPtType *GMSrfQuadricQuadOnly(IrtPtType *ParamDomainPts,
				IrtPtType *EuclideanPts,
				int FirstAtOrigin,
				int NumEucDim,
				int NumPts,
				IrtPtType *QuadData);
IrtPtType *GMSrfCubicQuadOnly(IrtPtType *ParamDomainPts,
			      IrtPtType *EuclideanPts,
			      int FirstAtOrigin,
			      int NumEucDim,
			      int NumPts,
			      IrtPtType *CubicData);

/* Metamorphosis of polygonal objects. */
struct IPPolygonStruct *GMPolygonalMorphosis(const struct IPPolygonStruct *Pl1,
					     const struct IPPolygonStruct *Pl2,
					     IrtRType t);

/* Scan conversion of polygons. */
void GMScanConvertTriangle(int Pt1[2],
			   int Pt2[2],
			   int Pt3[2],
			   GMScanConvertApplyFuncType ApplyFunc);

/* Text and string data sets. */
int GMLoadTextFont(const char *FName);
struct IPObjectStruct *GMMakeTextGeometry(const char *Str,
					  const IrtVecType Spacing,
					  const IrtRType *Scaling);

/* Curvature analysis over polygonal meshes. */
void GMPlCrvtrSetCurvatureAttr(struct IPPolygonStruct *PolyList,
			       int NumOfRings,
			       int EstimateNrmls);
int GMPlCrvtrSetFitDegree(int UseCubic);

/* Importance analysis over polygonal meshes. */
void GMPlSilImportanceAttr(struct IPPolygonStruct *PolyList);
struct IPPolygonStruct *GMPlSilImportanceRange(struct IPPolygonStruct
					                         *PolyList);


/* Extraction of properties from polygonal meshes. */
struct IPPolygonStruct *GMPolyPropFetchAttribute(struct IPPolygonStruct *Pls,
						 IPAttrIDType AttrID,
					         IrtRType Value);
struct IPPolygonStruct *GMPolyPropFetchIsophotes(struct IPPolygonStruct *Pls,
					  const IrtVecType ViewDir,
					  IrtRType InclinationAngle);
struct IPPolygonStruct *GMPolyPropFetchCurvature(struct IPPolygonStruct *Pls,
					  int CurvatureProperty,
					  int NumOfRings,
					  IrtRType CrvtrVal);
struct IPPolygonStruct *GMPolyPropFetch(struct IPPolygonStruct *Pls,
				 GMFetchVertexPropertyFuncType VertexProperty,
				 IrtRType ConstVal,
				 void *AuxData);
struct IPPolygonStruct *GMGenPolyline2Vrtx(IrtVecType V1,
				    IrtVecType V2,
				    struct IPPolygonStruct *Pnext);

/* Function for primitive fitting to point clouds. */
IrtRType GMFitData(IrtRType **PointData,
		   unsigned int NumberOfPointsToFit,
		   GMFittingModelType FittingModel,
		   IrtRType ModelParams[],
		   IrtRType Tolerance);
IrtRType GMFitDataWithOutliers(IrtRType **PointData,
			       unsigned int NumberOfPointsToFit,
			       GMFittingModelType FittingModel,
			       IrtRType ModelParams[],
			       IrtRType Tolerance,
			       unsigned int NumOfChecks);
IrtRType GMFitObjectWithOutliers(struct IPPolygonStruct *PPoly,
				 GMFittingModelType FittingModel,
				 IrtRType ModelExtParams[],
				 IrtRType Tolerance,
				 unsigned int NumOfChecks);	
IrtRType GMFitEstimateRotationAxis(IrtPtType *PointsOnObject,
				   IrtVecType *Normals,
				   unsigned int NumberOfPoints, 
				   IrtPtType PointOnRotationAxis,
				   IrtVecType RotationAxisDirection);

/* Functions to construct an adjacency data structure for polygonal meshes. */
VoidPtr GMPolyAdjacncyGen(struct IPObjectStruct *PObj, IrtRType EqlEps);
void GMPolyAdjacncyVertex(struct IPVertexStruct *V,
			  VoidPtr PolyAdj,
			  GMPolyAdjacncyVertexFuncType AdjVertexFunc);
void GMPolyAdjacncyFree(VoidPtr PolyAdj);
int GMIdentifyTJunctions(struct IPObjectStruct *PolyObj,
			 GMIdentifyTJunctionFuncType TJuncCB,
			 IrtRType Eps);
int GMRefineDeformedTriangle(struct IPPolygonStruct *Pl,
			     GMPointDeformVrtxFctrFuncType DeformVrtxFctrFunc,
			     GMPointDeformVrtxDirFuncType DeformVrtxDirFunc,
			     void *UserData,
			     IrtRType DeviationTol,
			     IrtRType MaxEdgeLen);
int GMRefineDeformedTriangle2(struct IPPolygonStruct *Pl,
			      GMPointDeformVrtxFctrFuncType DeformVrtxFctrFunc,
			      void *UserData,
			      IrtBType Ref12,
			      IrtBType Ref23,
			      IrtBType Ref31);

/* Functions to smooth poly data. */
struct IPObjectStruct *GMPolyMeshSmoothing(
			       struct IPObjectStruct *PolyObj,
			       const struct IPPolygonStruct *VerticesToRound,
			       int AllowBndryMove,
			       IrtRType RoundingRadius,
			       int NumIters,
			       IrtRType BlendFactor,
			       int CurvatureLimits);
void GMFindUnConvexPolygonNormal(const struct IPVertexStruct *VL,
				 IrtVecType Nrml);
int GMFindPtInsidePoly(const struct IPPolygonStruct *Pl, IrtPtType PtInside);
int GMFindPtInsidePolyKernel(const struct IPVertexStruct *VE,
			     IrtPtType KrnlPt);
int GMIsVertexBoundary(int Index, const struct IPPolyVrtxArrayStruct *PVIdx);
int GMIsInterLinePolygon2D(const struct IPVertexStruct *VS, 
			   const IrtPtType V1, 
			   const IrtPtType V2, 
			   IrtRType *t);
int GMAllIntersLinePolygon2D(const struct IPVertexStruct *VS, 
			     const IrtPtType V1, 
			     const IrtPtType V2, 
			     IrtRType *AllPrms);
int GMComputeAverageVertex(const struct IPVertexStruct *VS, 
			   IrtPtType CenterPoint, 
			   IrtRType BlendFactor);
int GMComputeAverageVertex2(const int *NS, 
			    const struct IPPolyVrtxArrayStruct *PVIdx,
			    IrtPtType CenterPoint, 
			    int CenterIndex,
			    IrtRType BlendFactor,
			    IrtRType DesiredRadius);

  /* Subdivision surfaces functions. */
struct IPObjectStruct *GMSubCatmullClark(const struct IPObjectStruct
							        *OriginalObj);
struct IPObjectStruct *GMSubLoop(const struct IPObjectStruct *OriginalObj);
struct IPObjectStruct *GMSubButterfly(const struct IPObjectStruct *OriginalObj, 
				      IrtRType ButterflyWCoef);

/* Error handling. */
GeomSetErrorFuncType GeomSetFatalErrorFunc(GeomSetErrorFuncType ErrorFunc);
void GeomFatalError(GeomFatalErrorType ErrID);
const char *GeomDescribeError(GeomFatalErrorType ErrID);


/* Polygon mesh handling functions using BVH. */
struct GMPolyBVHStruct *GMPolyBVHCreate(const struct IPPolygonStruct *Pl);
void GMPolyBVHFree(struct GMPolyBVHStruct *PolyBVH);

int GMPolyBVHGetClosestPoint(const IrtPtType query, 
			     const struct GMPolyBVHStruct *PolyBVH,
			     IrtPtType ClosestPt,
			     int *ClosestPlIdx);
int GMPolyBVHGetClosestPoint2(const IrtPtType QueryPt,
			      const struct GMPolyBVHStruct *PolyBVH,
			      IrtPtType ClosestPt,
			      int *ClosestPlIdx);
int GMPolyBVHGetRayBVHIntersectionPt(const IrtPtType RayPt, 
				     const IrtVecType RayDir, 
				     const struct GMPolyBVHStruct *PolyBVH,
				     IrtPtType *Res);
int GMPolyBVHPointInsidePolys(const IrtPtType Pt,
			      const struct GMPolyBVHStruct *PolyBVH);
int GMPolyBVHSrfsPolyInter(const struct IPObjectStruct *PSrfs,
			   const struct GMPolyBVHStruct *PolyBVH);
int GMPolyBVHCrvsPolyInter(const struct IPObjectStruct *PCrvs,
			   const struct GMPolyBVHStruct *PolyBVH);

/* BSP tree API, for 3D points. */
GMBspPtsSliceInfoStruct *GMBspPtsAllocSliceStruct();
void GMBspPtsFreeSliceStruct(GMBspPtsSliceInfoStruct *Slice);
void GMBspPtsFreeSliceStructList(GMBspPtsSliceInfoStruct *SliceList);
GMBspPtsNodeStruct *GMBspPtsCreateTree(struct IPVertexStruct *PC, 
				       GMBspPtsSliceInfoStruct *Slices, 
				       int StartIdx, 
				       int EndIdx);
void GMBspPtsFreeTree(GMBspPtsNodeStruct *Head);
struct IPVertexStruct *GMBspPtsGetVertices(GMBspPtsNodeStruct *Head, 
	                                   GMBspPtsSliceInfoStruct *LeftSlc, 
				           GMBspPtsSliceInfoStruct *RightSlc);
void GMBspPtsAddSlices(GMBspPtsNodeStruct *Head, 
		       GMBspPtsSliceInfoStruct *AddedSlices);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif	/* IRIT_GEOM_LIB_H */
