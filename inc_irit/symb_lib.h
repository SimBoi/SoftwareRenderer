/******************************************************************************
* Symb_lib.h - header file for the SYMBolic library.			      *
* This header is also the interface header to the world.		      *
* This library is closely related to symb_lib and should be linked with it.   *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber, Mar. 90.					      *
******************************************************************************/

#ifndef IRIT_SYMB_LIB_H
#define IRIT_SYMB_LIB_H

#include <stdio.h>
#include "inc_irit/irit_sm.h"
#include "inc_irit/cagd_lib.h"

#define SYMB_CRV_APRX_C1_DISCONT_ATTR_ID IRIT_ATTR_CREATE_ID(C1Discont)

typedef enum {
    SYMB_ERR_WRONG_INPUT,
    SYMB_ERR_WRONG_SRF,
    SYMB_ERR_BZR_CRV_EXPECT,
    SYMB_ERR_BZR_SRF_EXPECT,
    SYMB_ERR_BSP_CRV_EXPECT,
    SYMB_ERR_BSP_SRF_EXPECT,
    SYMB_ERR_RATIONAL_EXPECTED,
    SYMB_ERR_NO_CROSS_PROD,
    SYMB_ERR_POWER_NO_SUPPORT,
    SYMB_ERR_CRV_FAIL_CMPT,
    SYMB_ERR_SRF_FAIL_CMPT,
    SYMB_ERR_UNDEF_CRV,
    SYMB_ERR_UNDEF_SRF,
    SYMB_ERR_INVALID_CRV,
    SYMB_ERR_INVALID_SRF,
    SYMB_ERR_UNDEF_GEOM,
    SYMB_ERR_OUT_OF_RANGE,
    SYMB_ERR_DIR_NOT_CONST_UV,
    SYMB_ERR_REPARAM_NOT_MONOTONE,
    SYMB_ERR_BSPLINE_NO_SUPPORT,
    SYMB_ERR_WRONG_PT_TYPE,
    SYMB_ERR_ONLY_2D_OR_3D,
    SYMB_ERR_ONLY_2D,
    SYMB_ERR_ONLY_3D,
    SYMB_ERR_RATIONAL_NO_SUPPORT,
    SYMB_ERR_SRFS_INCOMPATIBLE,
    SYMB_ERR_CRVS_INCOMPATIBLE,
    SYMB_ERR_CANNOT_COMP_NORMAL,
    SYMB_ERR_TOO_COMPLEX,
    SYMB_ERR_UNSUPPORT_PT,
    SYMB_ERR_W_NOT_SAME,
    SYMB_ERR_SCALAR_EXPECTED,
    SYMB_ERR_POLY_CONST_SRF,
    SYMB_ERR_COPLANAR_GEOMETRY,
    SYMB_ERR_ILLEGAL_PARAMETERS,
    SYMB_ERR_INCONSIST_EDGE_BHOLE,
    SYMB_ERR_BIARC_FIT_FAIL,
    SYMB_ERR_SPL_PROD_FAILED,
    SYMB_ERR_MATCH_FAILED,
    SYMB_ERR_MINIMUM_LINEAR,
    SYMB_ERR_DIV_ZERO,
    SYMB_ERR_INVALID_AXIS,
    SYMB_ERR_WRONG_KNOT_INDEX,
    SYMB_ERR_SUBDIV_TREE_BEZ_ONLY,
    SYMB_ERR_IDENTICAL_ZERO_DATA,
    SYMB_ERR_PERODIC_NOT_SUPPORTED,
    SYMB_ERR_MISMATCH_DOMAIN,

    SYMB_ERR_UNDEFINE_ERR
} SymbFatalErrorType;

typedef enum {
    SYMB_SUBDIV_STRAT_ALTERNATE = 1,
    SYMB_SUBDIV_STRAT_MIN_MAX,
    SYMB_SUBDIV_STRAT_MIN_MIN
} SymbPlSubdivStrategyType;

typedef enum {
    SYMB_CRV_APPROX_UNIFORM = 0,
    SYMB_CRV_APPROX_TOLERANCE,
    SYMB_CRV_APPROX_CURVATURE
} SymbCrvApproxMethodType;

typedef enum {                       /* Type of relation between two curves. */
    SYMB_CRVREL_NO_RELATION = 0,
    SYMB_CRVREL_SAME_CRVS,
    SYMB_CRVREL_OVERLAPPING_CRVS,		
    SYMB_CRVREL_DISTINCT_CRVS
} SymbCrvRelType;

typedef enum {
    SYMB_APPROX_LOW_DEG_NO_TREE = 0,
    SYMB_APPROX_LOW_DEG_CREATE_TREE,
    SYMB_APPROX_LOW_DEG_TREE_COMPLETE,
    SYMB_APPROX_LOW_DEG_FREE_TREE
} SymbApproxLowDegStateType;

typedef enum {
    BSP_MULT_BEZ_DECOMP,
    BSP_MULT_INTERPOL,
    BSP_MULT_BLOSSOMING
} BspMultComputationMethodType;

typedef enum { 
    SYMB_CRV_LS_APX_ERROR_PRECISE,
    SYMB_CRV_LS_APX_ERROR_PRECISE_FAST,
    SYMB_CRV_LS_APX_ERROR_FAST,
} SymbCrvLSErrorMeasureType;

typedef enum {
    SYMB_ASYMP_BOUNDS_B,
    SYMB_ASYMP_BOUNDS_A,
    SYMB_ASYMP_BOUNDS_T
} SymbSrfAsympBoundsMethod;

typedef struct SymbMultiResCrvStruct {
    struct SymbMultiResCrvStruct *Pnext;
    CagdCrvStruct **HieCrv;
    int RefineLevel, Levels, Periodic;
} SymbMultiResCrvStruct;

typedef struct SymbNormalConeStruct {
    CagdVType ConeAxis;
    CagdRType ConeAngle;
} SymbNormalConeStruct;

typedef struct SymbArcStruct {
    struct SymbArcStruct *Pnext;
    struct IPAttributeStruct *Attr;
    int Arc;
    CagdPType Pt1, Cntr, Pt2;
} SymbArcStruct;

#if defined(_WIN64) || defined(LINUX386)
#define SYMB_MAX_CRV_SUBDIV_LEN  100000        /* No subdiv. of larger crvs. */
#else
#define SYMB_MAX_CRV_SUBDIV_LEN    5000        /* No subdiv. of larger crvs. */
#endif /* _WIN64 || LINUX386 */

typedef void (*SymbSetErrorFuncType)(SymbFatalErrorType ErrorFunc);
typedef CagdCrvStruct *(*SymbAdapIsoDistSqrFuncType)(int Level,
						     CagdCrvStruct *Crv1,
						     CagdCrvStruct *NCrv1,
						     CagdCrvStruct *Crv2,
						     CagdCrvStruct *NCrv2);
typedef CagdRType (*SymbPlErrorFuncType)(CagdSrfStruct *Srf,
					 CagdSrfDirType Dir,
					 int SubdivDepth);
typedef CagdCrvStruct *(*SymbOffCrvFuncType)(const CagdCrvStruct *Crv,
					     CagdRType Off,
					     CagdBType B);
typedef CagdCrvStruct *(*SymbVarOffCrvFuncType)(const CagdCrvStruct *Crv,
						const CagdCrvStruct *VarOff,
						CagdBType B);
typedef int (*SymbUniformAprxSrfPtImportanceFuncType)(const CagdSrfStruct *Srf,
						      CagdRType u,
						      CagdRType v);
typedef CagdBType (*SymbCrv2PolylineTlrncErrorFuncType)(const CagdCrvStruct
							                 *Crv);
typedef IrtRType (*SymbDistEnergy1DFuncType)(IrtRType e, 
				             IrtRType CrvtrCrvTMin,
				             IrtRType CrvtrCrvTMax,
				             CagdCrvStruct *CrvtrCrv,
				             CagdCrvStruct *Deriv1MagSqrCrv);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/******************************************************************************
* Routines to handle objects generically.				      *
******************************************************************************/
SymbArcStruct *SymbArcArrayNew(int Size);
SymbArcStruct *SymbArcNew(int Arc);
SymbArcStruct *SymbArcCopy(SymbArcStruct *Arc);
SymbArcStruct *SymbArcCopyList(SymbArcStruct *ArcList);
void SymbArcFree(SymbArcStruct *Arc);
void SymbArcFreeList(SymbArcStruct *ArcList);
void SymbArcArrayFree(SymbArcStruct *ArcArray, int Size);

/******************************************************************************
* Routines to handle curves generically.				      *
******************************************************************************/
CagdPolylineStruct *SymbCrv2Polyline(const CagdCrvStruct *Crv,
				     CagdRType TolSamplesPerCurve,
				     SymbCrvApproxMethodType Method,
				     CagdBType OptiLin);
CagdPtStruct *SymbHugeCrv2Polyline(const CagdCrvStruct *Crv,
				   int Samples,
				   CagdBType AddFirstPt,
				   CagdBType AddLastPt,
				   CagdBType AddParamVals);
SymbCrv2PolylineTlrncErrorFuncType SymbCrv2PolylineSetTlrncErrorFunc(
			         SymbCrv2PolylineTlrncErrorFuncType ErrorFunc);

CagdCrvStruct *SymbCrvAdd(const CagdCrvStruct *Crv1,
			  const CagdCrvStruct *Crv2);
CagdCrvStruct *SymbCrvSub(const CagdCrvStruct *Crv1,
			  const CagdCrvStruct *Crv2);
CagdCrvStruct *SymbCrvMult(const CagdCrvStruct *Crv1,
			   const CagdCrvStruct *Crv2);
CagdCrvStruct *SymbCrvInvert(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrvScalarScale(const CagdCrvStruct *Crv,
				  CagdRType Scale);
CagdCrvStruct *SymbCrvDotProd(const CagdCrvStruct *Crv1,
			      const CagdCrvStruct *Crv2);
CagdCrvStruct *SymbCrvVecDotProd(const CagdCrvStruct *Crv,
				 const CagdRType *Vec);
CagdCrvStruct *SymbCrvMultScalar(const CagdCrvStruct *Crv1,
				 const CagdCrvStruct *Crv2);
CagdCrvStruct *SymbCrvCrossProd(const CagdCrvStruct *Crv1,
				const CagdCrvStruct *Crv2);
CagdCrvStruct *SymbCrvVecCrossProd(const CagdCrvStruct *Crv,
				   const CagdVType Vec);
CagdCrvStruct *SymbCrvRtnlMult(const CagdCrvStruct *Crv1X,
			       const CagdCrvStruct *Crv1W,
			       const CagdCrvStruct *Crv2X,
			       const CagdCrvStruct *Crv2W,
			       CagdBType OperationAdd);
CagdCrvStruct *SymbCrvDeriveRational(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrvEnclosedArea(const CagdCrvStruct *Crv);
CagdRType SymbCrvEnclosedAreaEval(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrv2DCurvatureSqr(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrv3DCurvatureSqr(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrv3DRadiusNormal(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrv2DUnnormNormal(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrv3DCurvatureNormal(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrv2DCurvatureSign(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrvGenSignedCrvtr(const CagdCrvStruct *Crv,
				     int Samples,
				     int Order,
				     int ArcLen);
CagdCrvStruct *SymbSignedCrvtrGenCrv(const CagdCrvStruct *Crvtr,
				     CagdRType Tol,
				     int Order,
				     CagdBType Periodic);
CagdCrvStruct *SymbMakePosCrvCtlPolyPos(const CagdCrvStruct *OrigCrv);
CagdPtStruct *SymbCrv2DInflectionPts(const CagdCrvStruct *Crv,
				     CagdRType Epsilon);
CagdPtStruct *SymbCrvExtremCrvtrPts(const CagdCrvStruct *Crv,
				    CagdRType Epsilon,
				    CagdBType Crv2D);
CagdPtStruct *SymbCrvExtremCrvtrPts2(const CagdCrvStruct *Crv,
				     CagdRType Epsilon,
				     CagdBType Crv2D);
void SymbCrvSplitScalarN(const CagdCrvStruct *Crv, CagdCrvStruct **Crvs);
void SymbCrvSplitScalar(const CagdCrvStruct *Crv,
			CagdCrvStruct **CrvW,
			CagdCrvStruct **CrvX,
			CagdCrvStruct **CrvY,
			CagdCrvStruct **CrvZ);
CagdCrvStruct *SymbCrvMergeScalarN(CagdCrvStruct * const *CrvVec, 
				   int NumCrvs);
CagdCrvStruct *SymbCrvMergeScalar(const CagdCrvStruct *CrvW,
				  const CagdCrvStruct *CrvX,
				  const CagdCrvStruct *CrvY,
				  const CagdCrvStruct *CrvZ);
CagdCrvStruct *SymbCrvUnitLenScalar(const CagdCrvStruct *OrigCrv,
				    CagdBType Mult,
				    CagdRType Epsilon);
CagdCrvStruct *SymbCrvUnitLenCtlPts(const CagdCrvStruct *Crv);
CagdCrvStruct *SymbCrvSqrtScalar(const CagdCrvStruct *OrigCrv,
				 CagdRType Epsilon);
CagdCrvStruct *SymbCrvArcLenSclrCrv(const CagdCrvStruct *Crv,
				    CagdRType Epsilon);
CagdCrvStruct *SymbCrvArcLenCrv(const CagdCrvStruct *Crv,
				CagdRType Fineness,
				int Order);

CagdRType SymbCrvArcLen(const CagdCrvStruct *Crv, CagdRType Epsilon);
CagdRType SymbCrvArcLen2(const CagdCrvStruct *Crv, CagdRType Epsilon);
CagdPtStruct *SymbCrvArcLenSteps(const CagdCrvStruct *Crv,
				 CagdRType Length,
				 CagdRType Epsilon);
CagdRType *SymbCrvArcLenSteps2(const CagdCrvStruct *Crv,
			       int NumSteps,
			       CagdRType Epsilon);
int SymbCrvMonotoneCtlPt(const CagdCrvStruct *Crv, int Axis);
CagdCrvStruct *SymbComposeCrvCrv(const CagdCrvStruct *Crv1,
				 const CagdCrvStruct *Crv2);
CagdCrvStruct *SymbComposePeriodicCrvCrv(const CagdCrvStruct *Crv1,
					 const CagdCrvStruct *Crv2,
					 CagdRType Epsilon);
int SymbComposeSrfSetCache(int Cache);
void SymbComposeSrfClrCache(const CagdSrfStruct *Srf);
CagdCrvStruct *SymbComposeSrfCrv(const CagdSrfStruct *Srf,
				 const CagdCrvStruct *Crv,
				 int EvalLinCrvsDirectly,
				 CagdRType FilterTinysegs);
CagdCrvStruct *SymbComposeSrfCrv2(const CagdSrfStruct *Srf,
				  const CagdCrvStruct *Crv,
				  CagdRType FilterTinysegs);
CagdSrfStruct *SymbComposeSrfPatch(const CagdSrfStruct *Srf,
				   const CagdUVType UV00,
				   const CagdUVType UV01,
				   const CagdUVType UV10,
				   const CagdUVType UV11);
CagdCrvStruct *SymbComposePeriodicSrfCrv(const CagdSrfStruct *Srf,
					 const CagdCrvStruct *Crv,
					 CagdRType Epsilon);
CagdSrfStruct *SymbComposeSrfSrf(const CagdSrfStruct *Srf1,
				 const CagdSrfStruct *Srf2);
CagdCrvStruct *SymbMapUVCrv2E3(const CagdCrvStruct *Crv,
			       const CagdSrfStruct *Srf,
			       CagdBType Compose);

CagdCrvStruct **SymbComputeCurvePowers(const CagdCrvStruct *Crv, int Order);
CagdSrfStruct **SymbComputeSurfacePowers(const CagdSrfStruct *CSrf, int Order);
CagdCrvStruct *SymbDecomposeCrvCrv(CagdCrvStruct *Crv);
struct IPObjectStruct *SymbComposeTileObjectInSrf(const struct IPObjectStruct
						                       *PTile,
						  const CagdSrfStruct
						                   *DeformSrf,
						  IrtRType UTimes,
						  IrtRType VTimes,
						  int FitObj);
CagdRType SymbDistCrvPoint(const CagdCrvStruct *Crv,
			   void *CrvPtPrepHandle,
			   const CagdPType Pt,
			   CagdBType MinDist,
			   CagdRType Epsilon,
			   CagdPointType DistSpace);
void *SymbDistCrvPointPrep(const CagdCrvStruct *CCrv);
void SymbDistCrvPointFree(void *CrvPtPrepHandle);
CagdPtStruct *SymbLclDistCrvPoint(const CagdCrvStruct *Crv,
				  void *CrvPtPrepHandle,
				  const CagdRType *Pt,
				  CagdRType Epsilon);
CagdRType SymbDistCrvLine(const CagdCrvStruct *Crv,
			  const CagdLType Line,
			  CagdBType MinDist,
			  CagdRType Epsilon);
CagdPtStruct *SymbLclDistCrvLine(const CagdCrvStruct *Crv,
				 const CagdLType Line,
				 CagdRType Epsilon,
				 CagdBType InterPos,
				 CagdBType ExtremPos);
int SymbCrvPointInclusion(const CagdCrvStruct *Crv,
			  const CagdPType Pt,
			  CagdRType Epsilon);
CagdPtStruct *SymbCrvRayInter(const CagdCrvStruct *Crv,
			      const CagdPType RayPt,
			      const CagdVType RayDir,
			      CagdRType Epsilon);
CagdRType SymbDistBuildMapToCrv(const CagdCrvStruct *Crv,
				CagdRType Tolerance,
				CagdRType *XDomain,
				CagdRType *YDomain,
				CagdRType **DiscMap,
				CagdRType DiscMapXSize,
				CagdRType DiscMapYSize);
CagdPtStruct *SymbCrvZeroSet(const CagdCrvStruct *Crv,
			     int Axis,
			     CagdRType Epsilon,
			     CagdBType NoSolsOnEndPts);
CagdPtStruct *SymbCrvExtremSet(const CagdCrvStruct *Crv,
			       int Axis,
			       CagdRType Epsilon,
			       CagdBType NoSolsOnEndPts);
CagdPtStruct *SymbCrvConstSet(const CagdCrvStruct *Crv,
			      int Axis,
			      CagdRType Epsilon,
			      CagdRType ConstVal,
			      CagdBType NoSolsOnEndPts);
CagdCrvStruct *SymbCrvSliceCrvsByPrllLines(const CagdCrvStruct *Crvs,
					   int Axis,
					   CagdRType Epsilon,
					   CagdVType Range);
CagdPtStruct *SymbScalarCrvLowDegZeroSet(CagdCrvStruct *Crv);
CagdBType SymbCrvPosNegWeights(const CagdCrvStruct *Crv);
CagdPtStruct *SymbSplitRationalCrvsPoles(const CagdCrvStruct *Crv,
					 CagdRType Epsilon);
CagdCrvStruct *SymbCrvSplitPoleParams(const CagdCrvStruct *Crv,
				      CagdRType Eps,
				      CagdRType OutReach);
CagdCrvStruct *SymbCrvsSplitPoleParams(const CagdCrvStruct *Crvs,
				       CagdRType Eps,
				       CagdRType OutReach);
CagdCrvStruct *SymbCrvOffset(const CagdCrvStruct *Crv,
			     CagdRType OffsetDist,
			     CagdBType BezInterp);
CagdCrvStruct *SymbCrvVarOffset(const CagdCrvStruct *Crv,
				const CagdCrvStruct *VarOffsetDist,
				CagdBType BezInterp);
CagdCrvStruct *SymbCrvSubdivOffset(const CagdCrvStruct *Crv,
				   CagdRType OffsetDist,
				   CagdRType Tolerance,
				   CagdBType BezInterp);
CagdSrfStruct *SymbSrfCloseParallelSrfs2Shell(const CagdSrfStruct *Srf1,
					      const CagdSrfStruct *Srf2);
CagdCrvStruct *SymbCrvAdapOffset(const CagdCrvStruct *OrigCrv,
				 CagdRType OffsetDist,
				 CagdRType OffsetError,
				 SymbOffCrvFuncType OffsetAprxFunc,
				 CagdBType BezInterp);
CagdCrvStruct *SymbCrvAdapVarOffset(const CagdCrvStruct *OrigCrv,
				    const CagdCrvStruct *VarOffsetDist,
				    CagdRType OffsetError,
				    SymbVarOffCrvFuncType VarOffsetAprxFunc,
				    CagdBType BezInterp);
CagdCrvStruct *SymbCrvAdapOffsetTrim(const CagdCrvStruct *OrigCrv,
				     CagdRType OffsetDist,
				     CagdRType OffsetError,
				     SymbOffCrvFuncType OffsetAprxFunc,
				     CagdBType BezInterp);
CagdCrvStruct *SymbCrvLeastSquarOffset(const CagdCrvStruct *Crv,
				       CagdRType OffsetDist,
				       int NumOfSamples,
				       int NumOfDOF,
				       int Order,
				       CagdRType *Tolerance);
CagdCrvStruct *SymbCrvMatchingOffset(CagdCrvStruct *Crv,
				     CagdRType OffsetDist,
				     CagdRType Tolerance);
CagdCrvStruct *SymbCrvTrimGlblOffsetSelfInter(const CagdCrvStruct *Crv,
					      const CagdCrvStruct *OffCrv,
					      CagdRType SubdivTol,
					      CagdRType TrimAmount,
					      CagdRType NumerTol);
CagdBType SymbIsOffsetLclSelfInters(CagdCrvStruct *Crv,
				    CagdCrvStruct *OffCrv,
				    CagdPtStruct **SIDmns);
CagdCrvStruct *SymbCrvCrvConvolution(CagdCrvStruct *Crv1,
				     CagdCrvStruct *Crv2,
				     CagdRType OffsetDist,
				     CagdRType Tolerance);
CagdSrfStruct *SymbEnvOffsetFromCrv(const CagdCrvStruct *Crv,
				    CagdRType Height,
				    CagdRType Tolerance);
CagdCrvStruct *SymbCrvOffset2CrvsJoint(CagdCrvStruct *OrigCrv1,
				       CagdCrvStruct *OrigCrv2,
				       CagdCrvStruct **OffCrv1,
				       CagdCrvStruct **OffCrv2);
CagdCrvStruct *SymbCrvCrvtrTrim(const CagdCrvStruct *Crv,
				CagdRType Dist,
				CagdRType Eps);

CagdRType *SymbUniformAprxPtOnCrvDistrib(const CagdCrvStruct *Crv,
					 CagdBType ParamUniform,
					 int n);
IrtRType *SymbDistPoint1DWithEnergy(int N,
				    IrtRType XMin,
				    IrtRType XMax,
				    CagdCrvStruct *CrvtrCrv,
				    CagdCrvStruct *Deriv1MagSqrCrv,
				    int Resolution,
				    SymbDistEnergy1DFuncType EnergyFunc);
CagdPtStruct *SymbInsertNewParam2(CagdPtStruct *PtList, CagdRType t);
void SymbEvalCrvCurvPrep(const CagdCrvStruct *Crv, CagdBType Init);
CagdBType SymbEvalCrvCurvature(const CagdCrvStruct *Crv,
			       CagdRType t,
			       CagdRType *k,
			       CagdVType Tan,
			       CagdVType BiNrml);
void SymbEvalCrvCurvTN(CagdVType Nrml,
		       CagdVType Tan,
		       CagdVType BiNrml,
		       int Normalize);

/******************************************************************************
* Routines to handle surfaces generically.				      *
******************************************************************************/
struct IPPolygonStruct *SymbSrf2Polygons(const CagdSrfStruct *Srf,
				         int FineNess,
				         CagdBType ComputeNormals,
				         CagdBType FourPerFlat,
				         CagdBType ComputeUV);
CagdPolylineStruct *SymbSrf2Polylines(const CagdSrfStruct *Srf,
				      int NumOfIsocurves[2],
				      CagdRType TolSamples,
				      SymbCrvApproxMethodType Method);
CagdCrvStruct *SymbSrf2Curves(const CagdSrfStruct *Srf,
			      int NumOfIsocurves[2]);
CagdSrfStruct *SymbSrfAdd(const CagdSrfStruct *Srf1,
			  const CagdSrfStruct *Srf2);
CagdSrfStruct *SymbSrfSub(const CagdSrfStruct *Srf1,
			  const CagdSrfStruct *Srf2);
CagdSrfStruct *SymbSrfMult(const CagdSrfStruct *Srf1, 
			   const CagdSrfStruct *Srf2);
CagdSrfStruct *SymbSrfInvert(const CagdSrfStruct *Srf);
CagdSrfStruct *SymbSrfScalarScale(const CagdSrfStruct *Srf,
				  CagdRType Scale);
CagdSrfStruct *SymbSrfDotProd(const CagdSrfStruct *Srf1,
			      const CagdSrfStruct *Srf2);
CagdSrfStruct *SymbSrfVecDotProd(const CagdSrfStruct *Srf,
				 const CagdVType Vec);
CagdSrfStruct *SymbSrfMultScalar(const CagdSrfStruct *Srf1,
				 const CagdSrfStruct *Srf2);
CagdSrfStruct *SymbSrfCrossProd(const CagdSrfStruct *Srf1,
				const CagdSrfStruct *Srf2);
CagdSrfStruct *SymbSrfVecCrossProd(const CagdSrfStruct *Srf,
				   const CagdVType Vec);
CagdSrfStruct *SymbSrfRtnlMult(const CagdSrfStruct *Srf1X,
			       const CagdSrfStruct *Srf1W,
			       const CagdSrfStruct *Srf2X,
			       const CagdSrfStruct *Srf2W,
			       CagdBType OperationAdd);
CagdSrfStruct *SymbSrfDeriveRational(const CagdSrfStruct *Srf,
				     CagdSrfDirType Dir);
CagdSrfStruct *SymbSrfNormalSrf(const CagdSrfStruct *Srf);
CagdSrfStruct *SymbSrfNormalSrfReversed(const CagdSrfStruct *Srf);
CagdSrfStruct *Symb2DSrfJacobian(const CagdSrfStruct *Srf);
void SymbMeshAddSub(CagdRType **DestPoints,
		    CagdRType * const *Points1,
		    CagdRType * const *Points2,
		    CagdPointType PType,
		    int Size,
		    CagdBType OperationAdd);
void SymbMeshAddSubTo(CagdRType **DestPoints,
		      CagdRType * const *Points2,
		      CagdPointType PType,
		      int Size,
		      CagdBType OperationAdd);
void SymbSrfSplitScalarN(const CagdSrfStruct *Srf,
			 CagdSrfStruct *Srfs[CAGD_MAX_PT_SIZE]);
void SymbSrfSplitScalar(const CagdSrfStruct *Srf,
			CagdSrfStruct **SrfW,
			CagdSrfStruct **SrfX,
			CagdSrfStruct **SrfY,
			CagdSrfStruct **SrfZ);
CagdSrfStruct *SymbSrfMergeScalarN(CagdSrfStruct * const *SrfVec, 
				   int NumSrfs);
CagdSrfStruct *SymbSrfMergeScalar(const CagdSrfStruct *SrfW,
				  const CagdSrfStruct *SrfX,
				  const CagdSrfStruct *SrfY,
				  const CagdSrfStruct *SrfZ);
CagdCrvStruct *SymbPrmtSclrCrvTo2D(const CagdCrvStruct *Crv,
				   CagdRType Min,
				   CagdRType Max);
CagdSrfStruct *SymbPrmtSclrSrfTo3D(const CagdSrfStruct *Srf,
				   CagdRType UMin, CagdRType UMax,
				   CagdRType VMin, CagdRType VMax);
CagdRType *SymbExtremumCntPtValsToData(CagdRType * const *Points,
				       int Length,
				       CagdBType FindMinimum,
				       CagdRType *Extremum);
CagdRType *SymbExtremumCntPtValsMalloc(CagdRType * const *Points,
				       int Length,
				       CagdBType FindMinimum);

/* Adaptive iso synthsis modeul: */
typedef struct SymbAdapIsoGenInfoStruct *SymbAdapIsoGenInfoStructPtr;

SymbAdapIsoGenInfoStructPtr SymbAdapIsoExtractInit(void);
void SymbAdapIsoExtractFree(SymbAdapIsoGenInfoStructPtr GI);
CagdCrvStruct *SymbAdapIsoSkewDistSqr(int Level,
				      CagdCrvStruct *Crv1,
				      CagdCrvStruct *NCrv1,
				      CagdCrvStruct *Crv2,
				      CagdCrvStruct *NCrv2);
CagdCrvStruct *SymbAdapIsoExtract(SymbAdapIsoGenInfoStructPtr GI,
				  const CagdSrfStruct *Srf,
				  const CagdSrfStruct *NSrf,
				  SymbAdapIsoDistSqrFuncType AdapIsoDistFunc,
				  CagdSrfDirType Dir,
				  CagdRType Eps,
				  CagdBType FullIso,
				  CagdBType SinglePath);
struct IPObjectStruct *SymbAdapIsoExtractRectRgns(
					      SymbAdapIsoGenInfoStructPtr GI,
					      const CagdSrfStruct *Srf,
					      CagdSrfDirType Dir,
					      CagdRType Size,
					      int Smoothing,
					      int OutputType);
void SymbAdapIsoSetWeightPt(SymbAdapIsoGenInfoStructPtr GI,
			    CagdRType *Pt,
			    CagdRType Scale,
			    CagdRType Width);
int SymbAdapIsoSetExtractMinLevel(SymbAdapIsoGenInfoStructPtr GI,
				  int MinLevel);


CagdSrfStruct *SymbSrfVolume1Srf(const CagdSrfStruct *Srf,
				 CagdBType Integrate);
CagdRType SymbSrfVolume1(const CagdSrfStruct *Srf);
CagdSrfStruct *SymbSrfVolume2Srf(const CagdSrfStruct *Srf,
				 CagdBType Integrate);
CagdRType SymbSrfVolume2(const CagdSrfStruct *Srf);
CagdSrfStruct *SymbSrfFirstMomentSrf(const CagdSrfStruct *Srf,
				     int Axis,
				     CagdBType Integrate);
CagdRType SymbSrfFirstMoment(const CagdSrfStruct *Srf, int Axis);
CagdSrfStruct *SymbSrfSecondMomentSrf(const CagdSrfStruct *Srf,
				      int Axis1,
				      int Axis2,
				      CagdBType Integrate);
CagdRType SymbSrfSecondMoment(const CagdSrfStruct *Srf, int Axis1, int Axis2);

void SymbSrfFff(const CagdSrfStruct *Srf,
		CagdSrfStruct **DuSrf,
		CagdSrfStruct **DvSrf,
		CagdSrfStruct **FffG11,
		CagdSrfStruct **FffG12,
		CagdSrfStruct **FffG22);
void SymbSrfSff(const CagdSrfStruct *DuSrf,
		const CagdSrfStruct *DvSrf,
		CagdSrfStruct **SffL11,
		CagdSrfStruct **SffL12,
		CagdSrfStruct **SffL22,
		CagdSrfStruct **SNormal);
void SymbSrfTff(const CagdSrfStruct *Srf,
		CagdSrfStruct **TffL11,
		CagdSrfStruct **TffL12,
		CagdSrfStruct **TffL22);
CagdSrfStruct *SymbSrfDeterminant2(const CagdSrfStruct *Srf11,
				   const CagdSrfStruct *Srf12,
				   const CagdSrfStruct *Srf21,
				   const CagdSrfStruct *Srf22);
CagdSrfStruct *SymbSrfDeterminant3(const CagdSrfStruct *Srf11,
				   const CagdSrfStruct *Srf12,
				   const CagdSrfStruct *Srf13,
				   const CagdSrfStruct *Srf21,
				   const CagdSrfStruct *Srf22,
				   const CagdSrfStruct *Srf23,
				   const CagdSrfStruct *Srf31,
				   const CagdSrfStruct *Srf32,
				   const CagdSrfStruct *Srf33);
CagdCrvStruct *SymbCrvDeterminant2(const CagdCrvStruct *Crv11,
				   const CagdCrvStruct *Crv12,
				   const CagdCrvStruct *Crv21,
				   const CagdCrvStruct *Crv22);
CagdCrvStruct *SymbCrvDeterminant3(const CagdCrvStruct *Crv11,
				   const CagdCrvStruct *Crv12,
				   const CagdCrvStruct *Crv13,
				   const CagdCrvStruct *Crv21,
				   const CagdCrvStruct *Crv22,
				   const CagdCrvStruct *Crv23,
				   const CagdCrvStruct *Crv31,
				   const CagdCrvStruct *Crv32,
				   const CagdCrvStruct *Crv33);
CagdSrfStruct *SymbSrfGaussCurvature(const CagdSrfStruct *Srf,
				     CagdBType NumerOnly);
CagdSrfStruct *SymbSrfMeanNumer(const CagdSrfStruct *Srf);
CagdSrfStruct *SymbSrfMeanCurvatureSqr(const CagdSrfStruct *Srf);
CagdSrfStruct *SymbSrfMeanEvolute(const CagdSrfStruct *Srf);
CagdSrfStruct *SymbSrfIsoFocalSrf(const CagdSrfStruct *Srf,
				  CagdSrfDirType Dir);
CagdSrfStruct *SymbSrfCurvatureUpperBound(const CagdSrfStruct *Srf);
CagdSrfStruct *SymbSrfIsoDirNormalCurvatureBound(const CagdSrfStruct *Srf,
						 CagdSrfDirType Dir);

/* Surfaces's normal curvatrue bounding routines. */
typedef struct SymbSrfCrvtrBndsInfoStruct *SymbSrfCrvtrBndsInfoStructPtr;

SymbSrfCrvtrBndsInfoStructPtr SymbSrfCrvtrBndsInfoCreate(const CagdSrfStruct
							                *Srf);
void SymbSrfCrvtrBndsSplitInfo(const SymbSrfCrvtrBndsInfoStructPtr Info,
			       SymbSrfCrvtrBndsInfoStructPtr Ret,
			       CagdSrfDirType Dir);
SymbSrfCrvtrBndsInfoStructPtr SymbSrfCrvtrBndsSubInfo(
				      const SymbSrfCrvtrBndsInfoStructPtr Info,
				      IrtRType UMin,
				      IrtRType UMax,
				      IrtRType VMin,
				      IrtRType VMax);

void SymbSrfCrvtrBndsCalcBnds(const SymbSrfCrvtrBndsInfoStructPtr Info,
			      IrtRType *K1Limits,
			      IrtRType *K2Limits);

void SymbSrfCrvtrBndsCalcBnds2(const SymbSrfCrvtrBndsInfoStructPtr Info,
			       int Count,
			       IrtRType ValMin,
			       IrtRType ValMax,
			       IrtRType ValRes,
			       IrtRType *K1Limits,
			       IrtRType *K2Limits);
void SymbSrfCrvtrBndsInfoClear(SymbSrfCrvtrBndsInfoStructPtr Info);
void SymbSrfCrvtrBndsInfoFree(SymbSrfCrvtrBndsInfoStructPtr Info);

int SymbSrfCalcAsympDirsCoeffs(const CagdSrfStruct *SffMatrix[3],
                               IrtRType *AsympLimits,
			       SymbSrfAsympBoundsMethod *ChosenMethod);

/**************************/

CagdSrfStruct *SymbSrfDistCrvCrv(const CagdCrvStruct *Crv1,
				 const CagdCrvStruct *Crv2,
				 int DistType);
CagdPtStruct *SymbSrfDistFindPoints(const CagdSrfStruct *Srf,
				    CagdRType Epsilon,
				    CagdBType SelfInter);
CagdPtStruct *SymbCrvCrvInter(const CagdCrvStruct *Crv1,
			      const CagdCrvStruct *Crv2,
			      CagdRType CCIEpsilon,
			      CagdBType SelfInter);
CagdBType SymbIntersectCrvWithLine(const CagdCrvStruct *OrigCrv,
				   int Axis,
				   CagdRType Val,
				   CagdCrvStruct **CrvsBelow,
				   CagdCrvStruct **CrvsAbove);
CagdSrfStruct *SymbConicDistCrvCrv(const CagdCrvStruct *Crv1,
				   const CagdCrvStruct *Crv2,
				   CagdRType Dist);
CagdRType SymbHausDistOfSamplefPts(CagdPType * const V1,
				   CagdPType * const V2,
				   int V1Size,
				   int V2Size,
				   int HDistSide);
CagdRType SymbHausDistBySamplesCrvCrv(const CagdCrvStruct *Crv1,
				      const CagdCrvStruct *Crv2,
				      int Samples,
				      int HDistSide);
CagdRType SymbHausDistBySamplesCrvSrf(const CagdCrvStruct *Crv1,
				      const CagdSrfStruct *Srf2,
				      int Samples,
				      int HDistSide);
CagdRType SymbHausDistBySamplesSrfSrf(const CagdSrfStruct *Srf1,
				      const CagdSrfStruct *Srf2,
				      int Samples,
				      int HDistSide);
CagdCrvStruct *SymbTwoCrvsMorphing(const CagdCrvStruct *Crv1,
				   const CagdCrvStruct *Crv2,
				   CagdRType Blend);
CagdCrvStruct *SymbTwoCrvsMorphingCornerCut(const CagdCrvStruct *Crv1,
					    const CagdCrvStruct *Crv2,
					    CagdRType MinDist,
					    CagdBType SameLength,
					    CagdBType FilterTangencies);
CagdCrvStruct *SymbTwoCrvsMorphingMultiRes(const CagdCrvStruct *Crv1,
					   const CagdCrvStruct *Crv2,
					   CagdRType BlendStep);
CagdSrfStruct *SymbTwoSrfsMorphing(const CagdSrfStruct *Srf1,
				   const CagdSrfStruct *Srf2,
				   CagdRType Blend);
CagdSrfStruct *SymbSrfOffset(const CagdSrfStruct *Srf, CagdRType OffsetDist);
CagdSrfStruct *SymbSrfVarOffset(const CagdSrfStruct *CSrf,
				const CagdSrfStruct *VarOffsetDist);
CagdSrfStruct *SymbSrfSubdivOffset(const CagdSrfStruct *Srf,
				   CagdRType OffsetDist,
				   CagdRType Tolerance);
CagdUVType *SymbUniformAprxPtOnSrfDistrib(
			const CagdSrfStruct *Srf,
			CagdBType ParamUniform,
			int n,
			SymbUniformAprxSrfPtImportanceFuncType EvalImportance);
void SymbUniformAprxPtOnSrfPrepDistrib(const CagdSrfStruct *Srf,
				       CagdUVType **DistUV,
				       CagdRType **DistProb,
				       int *DistSize,
				       int n);
CagdUVType *SymbUniformAprxPtOnSrfGetDistrib(const CagdSrfStruct *Srf,
					     CagdUVType *DistUV,
					     CagdRType *DistProb,
					     int DistSize,
					     int *n);
void SymbEvalSrfCurvPrep(const CagdSrfStruct *Srf, CagdBType Init);
CagdBType SymbEvalSrfCurvature(const CagdSrfStruct *Srf,
			       CagdRType U,
			       CagdRType V,
			       CagdBType DirInUV,
			       CagdRType *K1,
			       CagdRType *K2,
			       CagdVType D1,
			       CagdVType D2);
int SymbEvalSrfAsympDir(const CagdSrfStruct *Srf,
			CagdRType U,
			CagdRType V,
			CagdBType DirInUV,
			CagdVType AsympDir1,
			CagdVType AsympDir2);

/******************************************************************************
* Routines to handle Bezier curves.					      *
******************************************************************************/
CagdCrvStruct *BzrCrvMult(const CagdCrvStruct *Crv1,
			  const CagdCrvStruct *Crv2);
void BzrCrvMultPtsVecs(const CagdRType *Pts1,
		       int Order1,
		       const CagdRType *Pts2,
		       int Order2,
		       CagdRType *ProdPts);
CagdCrvStruct *BzrCrvMultList(const CagdCrvStruct *Crv1Lst,
			      const CagdCrvStruct *Crv2Lst);

CagdCrvStruct *BzrCrvFactorT(const CagdCrvStruct *Crv);
CagdCrvStruct *BzrCrvFactor1MinusT(const CagdCrvStruct *Crv);

CagdCrvStruct *SymbApproxCrvAsBzrCubics(const CagdCrvStruct *Crv,
					CagdRType Tol,
					CagdRType MaxLen);
CagdCrvStruct *SymbApproxCrvAsBzrQuadratics(const CagdCrvStruct *Crv,
					    CagdRType Tol,
					    CagdRType MaxLen);
CagdCrvStruct *BzrComposeCrvCrv(const CagdCrvStruct *Crv1,
				const CagdCrvStruct *Crv2);
CagdCrvStruct *BzrComposeSrfCrv(const CagdSrfStruct *Srf,
				const CagdCrvStruct *Crv);
CagdSrfStruct *BzrComposeSrfSrf(const CagdSrfStruct *Srf1,
				const CagdSrfStruct *Srf2);
CagdSrfStruct *BzrSrfSubdivAtCurve(const CagdSrfStruct *Srf,
				   const CagdCrvStruct *DivCrv);

/******************************************************************************
* Routines to handle B-spline curves.					      *
******************************************************************************/
BspMultComputationMethodType BspMultComputationMethod(
				   BspMultComputationMethodType BspMultMethod);
CagdCrvStruct *BspCrvMult(const CagdCrvStruct *Crv1,
			  const CagdCrvStruct *Crv2);
CagdCrvStruct *BspCrvBlossomMult(const CagdCrvStruct *Crv1,
				 const CagdCrvStruct *Crv2);
void SymbBspBasisInnerProdPrep(const CagdRType *KV,
			       int Len,
			       int Order1,
			       int Order2,
			       CagdCrvStruct **InnerProdCrv1,
			       CagdCrvStruct **InnerProdCrv2);
void SymbBspBasisInnerProdPrep2(const CagdRType *KV1,
				const CagdRType *KV2,
				int Len1,
				int Len2,
				int Order1,
				int Order2,
    			        CagdCrvStruct **InnerProdCrv1,
			        CagdCrvStruct **InnerProdCrv2);
CagdRType SymbBspBasisInnerProd(int Index1, int Index2,
				CagdCrvStruct *InnerProdCrv1,
			        CagdCrvStruct *InnerProdCrv2);

CagdRType **SymbBspBasisInnerProdMat(const CagdRType *KV,
				     int Len,
				     int Order1,
				     int Order2);
CagdRType SymbBspBasisInnerProd2(const CagdRType *KV,
				 int Len,
				 int Order1,
				 int Order2,
				 int Index1,
				 int Index2);

/******************************************************************************
* Routines to handle Bezier surfaces.					      *
******************************************************************************/
CagdSrfStruct *BzrSrfMult(const CagdSrfStruct *Srf1,
			  const CagdSrfStruct *Srf2);
CagdSrfStruct *BzrSrfFactorBilinear(const CagdSrfStruct *Srf,
				    const CagdRType *A);
CagdSrfStruct *BzrSrfFactorUMinusV(const CagdSrfStruct *Srf);
void BzrSrfFactorLowOrders(const CagdSrfStruct *Srf,
			   CagdSrfStruct **S11,
			   CagdSrfStruct **S12,
			   CagdSrfStruct **S21,
			   CagdSrfStruct **S22);
CagdSrfStruct *BzrSrfFactorExtremeRowCol(const CagdSrfStruct *Srf,
					 CagdSrfBndryType Bndry);

/******************************************************************************
* Routines to handle B-spline surfaces.					      *
******************************************************************************/
CagdSrfStruct *BspSrfMult(const CagdSrfStruct *Srf1,
			  const CagdSrfStruct *Srf2);
CagdSrfStruct *BspSrfBlossomMult(const CagdSrfStruct *Srf1,
				 const CagdSrfStruct *Srf2);
CagdSrfStruct *BspSrfFactorBilinear(const CagdSrfStruct *Srf,
				    const CagdRType *A);
CagdSrfStruct *BspSrfFactorUMinusV(const CagdSrfStruct *Srf);

/******************************************************************************
* Routines to handle layout (prisa) of surfaces.			      *
******************************************************************************/
CagdSrfStruct *SymbAllPrisaSrfs(const CagdSrfStruct *Srfs,
				int SamplesPerCurve,
				CagdRType Epsilon,
				CagdSrfDirType Dir,
				const CagdVType Space);
CagdSrfStruct *SymbPiecewiseRuledSrfApprox(const CagdSrfStruct *Srf,
					   CagdBType ConsistentDir,
					   CagdRType Epsilon,
					   CagdSrfDirType Dir);
CagdSrfStruct *SymbPrisaRuledSrf(const CagdSrfStruct *Srf,
				 int SamplesPerCurve,
				 CagdRType Space,
				 CagdVType Offset);
CagdCrvStruct *SymbPrisaGetCrossSections(const CagdSrfStruct *RSrfs,
					 CagdSrfDirType Dir,
					 const CagdVType Space);
CagdCrvStruct *SymbPrisaGetOneCrossSection(const CagdSrfStruct *RSrf,
					   CagdSrfDirType Dir,
					   CagdBType Starting,
					   CagdBType Ending);

/******************************************************************************
* Routines to handle Multi Resolution Bspline curves.                         *
******************************************************************************/
CagdBType SymbCrvMultiResKVBuild(const CagdCrvStruct *Crv,
				 CagdBType Discont,
				 CagdRType ***KVList,
				 int **KVListSizes,
				 int *KVListSize);
SymbMultiResCrvStruct *SymbCrvMultiResDecomp(const CagdCrvStruct *Crv,
					     CagdBType Discont);
SymbMultiResCrvStruct *SymbCrvMultiResDecomp2(const CagdCrvStruct *Crv,
					      CagdBType Discont,
					      CagdBType SameSpace);
CagdCrvStruct *SymbCrvMultiResCompos(const SymbMultiResCrvStruct *MRCrv);
CagdCrvStruct *SymbCrvMultiResComposAtT(const SymbMultiResCrvStruct *MRCrv,
					CagdRType T);
void SymbCrvMultiResEdit(const SymbMultiResCrvStruct *MRCrv,
			 CagdRType t,
			 const CagdVType TransDir,
			 CagdRType Level,
			 CagdRType FracLevel);
CagdRType *SymbCrvMultiResRefineLevelToData(SymbMultiResCrvStruct *MRCrv,
					    CagdRType T,
					    CagdBType SpanDiscont,
					    CagdRType *Domain);
CagdRType *SymbCrvMultiResRefineLevelMalloc(SymbMultiResCrvStruct *MRCrv,
					    CagdRType T,
					    CagdBType SpanDiscont);
CagdCrvStruct *SymbCrvMultiResBWavelet(CagdRType *KV,
				       int Order,
				       int Len,
				       int KnotIndex);
void SymbCrvMultiResFree(SymbMultiResCrvStruct *MRCrv);
SymbMultiResCrvStruct *SymbCrvMultiResNew(int Levels, CagdBType Periodic);
SymbMultiResCrvStruct *SymbCrvMultiResCopy(const SymbMultiResCrvStruct
					                          *MRCrvOrig);

/******************************************************************************
* Routines to compute the convex hull of piecewise freeforms		      *
******************************************************************************/
CagdCrvStruct *SymbCrvCnvxHull(const CagdCrvStruct *Crv, CagdRType SubdivTol);
CagdCrvStruct *SymbCrvListCnvxHull(CagdCrvStruct *Crvs, CagdRType SubdivTol);
struct IPPolygonStruct *SymbCrvDiameter(const CagdCrvStruct *Crv,
					CagdRType SubdivTol);
CagdRType *SymbCrvDiameterMinMaxToData(const CagdCrvStruct *Crv,
				       struct IPPolygonStruct *Cntrs,
				       int Min,
				       CagdRType *Params);
CagdRType *SymbCrvDiameterMinMaxMalloc(const CagdCrvStruct *Crv,
				       struct IPPolygonStruct *Cntrs,
				       int Min);
CagdPtStruct *SymbCrvPtTangents(const CagdCrvStruct *Crv,
				const CagdPType Pt,
				CagdRType Tolerance);
CagdPtStruct *SymbTangentToCrvAtTwoPts(const CagdCrvStruct *Crv,
				       CagdRType SubdivTol);
CagdPtStruct *SymbCircTanTo2Crvs(const CagdCrvStruct *Crv1,
				 const CagdCrvStruct *Crv2,
				 CagdRType Radius,
				 CagdRType Tol);

/******************************************************************************
* Routines to compute the bisector of piecewise freeforms		      *
******************************************************************************/
CagdRType *SymbBsctComputeInterMidPointToData(const CagdCrvStruct *Crv1,
					      CagdRType t1,
					      const CagdCrvStruct *Crv2,
					      CagdRType t2,
					      CagdPType *Inter);
CagdRType *SymbBsctComputeInterMidPointMalloc(const CagdCrvStruct *Crv1,
					      CagdRType t1,
					      const CagdCrvStruct *Crv2,
					      CagdRType t2);
CagdCrvStruct *SymbCrvBisectors(const CagdCrvStruct *Crv,
				int BisectFunc,
				CagdRType SubdivTol,
				CagdBType NumerImprove,
				CagdBType SameNormal,
				CagdBType SupportPrms);
CagdSrfStruct *SymbCrvBisectorsSrf(const CagdCrvStruct *Crv, int BisectFunc);
CagdSrfStruct *SymbCrvBisectorsSrf2(const CagdCrvStruct *Crv);
CagdSrfStruct *SymbCrvBisectorsSrf3(const CagdCrvStruct *Crv);
CagdSrfStruct *SymbCrvCrvBisectorSrf3D(const CagdCrvStruct *Crv1,
				       const CagdCrvStruct *Crv2,
				       CagdRType Alpha);
CagdSrfStruct *SymbCrvBisectorsSrf3D(CagdSrfStruct *Srf1,
				     CagdSrfStruct *Srf2,
				     CagdSrfStruct *DSrf1,
				     CagdSrfStruct *DSrf2,
				     CagdRType Alpha);
CagdCrvStruct *SymbCrvPtBisectorCrv2D(const CagdCrvStruct *Crv,
				      const CagdPType Pt,
				      CagdRType Alpha);
CagdSrfStruct *SymbCrvPtBisectorSrf3D(const CagdCrvStruct *Crv,
				      const CagdPType Pt,
				      CagdRType RulingScale);
CagdSrfStruct *SymbSrfPtBisectorSrf3D(const CagdSrfStruct *Srf,
				      const CagdPType Pt);

CagdCrvStruct *SymbPtCrvBisectOnSphere(const CagdPType Pt,
				       const CagdCrvStruct *Crv);
CagdCrvStruct *SymbPtCrvBisectOnSphere2(const CagdPType Pt,
					const CagdCrvStruct *Crv,
					CagdRType SubdivTol);
CagdSrfStruct *SymbCrvCrvBisectOnSphere(const CagdCrvStruct *Crv1,
					const CagdCrvStruct *Crv2);
CagdCrvStruct *SymbCrvCrvBisectOnSphere2(const CagdCrvStruct *Crv1,
					 const CagdCrvStruct *Crv2,
					 CagdRType SubdivTol);
CagdSrfStruct *SymbCrvCrvBisectOnSphere3(const CagdCrvStruct *Crv1,
					 const CagdCrvStruct *Crv2);
CagdSrfStruct *SymbPlanePointBisect(const CagdPType Pt, CagdRType Size);
CagdSrfStruct *SymbCylinPointBisect(const CagdPType CylPt,
				    const CagdVType CylDir,
				    CagdRType CylRad,
				    const CagdPType Pt,
				    CagdRType Size);
CagdSrfStruct *SymbConePointBisect(const CagdPType ConeApex,
				   const CagdVType ConeDir,
				   CagdRType ConeAngle,
				   const CagdPType Pt,
				   CagdRType Size);
CagdSrfStruct *SymbSpherePointBisect(const CagdPType SprCntr,
				     CagdRType SprRad,
				     const CagdPType Pt);
CagdSrfStruct *SymbTorusPointBisect(const CagdPType TrsCntr,
				    const CagdVType TrsDir,
				    CagdRType TrsMajorRad,
				    CagdRType TrsMinorRad,
				    const CagdPType Pt);
CagdSrfStruct *SymbPlaneLineBisect(const CagdVType LineDir, CagdRType Size);
CagdSrfStruct *SymbConeLineBisect(const CagdVType ConeDir,
				  CagdRType ConeAngle,
				  const CagdVType LineDir,
				  CagdRType Size);
CagdSrfStruct *SymbSphereLineBisect(const CagdPType SprCntr,
				    CagdRType SprRad,
				    CagdRType Size);
CagdSrfStruct *SymbSpherePlaneBisect(const CagdPType SprCntr,
				     CagdRType SprRad,
				     CagdRType Size);
CagdSrfStruct *SymbCylinPlaneBisect(const CagdPType CylPt,
				    const CagdVType CylDir,
				    CagdRType CylRad,
				    CagdRType Size);
CagdSrfStruct *SymbConePlaneBisect(const CagdPType ConeApex,
				   const CagdVType ConeDir,
				   CagdRType ConeAngle,
				   CagdRType Size);
CagdSrfStruct *SymbCylinSphereBisect(const CagdPType CylPt,
				     const CagdVType CylDir,
				     CagdRType CylRad,
				     const CagdPType SprCntr,
				     CagdRType SprRad,
				     CagdRType Size);
CagdSrfStruct *SymbSphereSphereBisect(const CagdPType SprCntr1,
				      CagdRType SprRad1,
				      const CagdPType SprCntr2,
				      CagdRType SprRad2);
CagdSrfStruct *SymbConeSphereBisect(const CagdPType ConeApex,
				    const CagdVType ConeDir,
				    CagdRType ConeAngle,
				    const CagdPType SprCntr,
				    CagdRType SprRad,
				    CagdRType Size);
CagdSrfStruct *SymbConeConeBisect(const CagdVType Cone1Dir,
				  CagdRType Cone1Angle,
				  const CagdVType Cone2Dir,
				  CagdRType Cone2Angle,
				  CagdRType Size);
CagdSrfStruct *SymbTorusSphereBisect(const CagdPType TrsCntr,
				     const CagdVType TrsDir,
				     CagdRType TrsMajorRad,
				     CagdRType TrsMinorRad,
				     const CagdPType SprCntr,
				     CagdRType SprRad);
CagdSrfStruct *SymbTorusTorusBisect(const CagdVType Trs1Cntr,
				    const CagdVType Trs1Dir,
				    CagdRType Trs1MajorRad,
				    const CagdVType Trs2Cntr,
				    const CagdVType Trs2Dir,
				    CagdRType Trs2MajorRad,
				    CagdRType Alpha);
CagdSrfStruct *SymbCylinCylinBisect(const CagdVType Cyl1Pos,
				    const CagdVType Cyl1Dir,
				    CagdRType Cyl1Rad,
				    const CagdVType Cyl2Pos,
				    const CagdVType Cyl2Dir,
				    CagdRType Cyl2Rad);
CagdSrfStruct *SymbConeConeBisect2(const CagdVType Cone1Pos,
				   const CagdVType Cone1Dir,
				   CagdRType Cone1Angle,
				   const CagdVType Cone2Pos,
				   const CagdVType Cone2Dir,
				   CagdRType Cone2Angle);
CagdSrfStruct *SymbConeCylinBisect(const CagdVType Cone1Pos,
				   const CagdVType Cone1Dir,
				   CagdRType Cone1Angle,
				   const CagdVType Cyl2Pos,
				   const CagdVType Cyl2Dir,
				   CagdRType Cyl2Rad);

/******************************************************************************
* Routines to handle Normal Cones.					      *
******************************************************************************/
const SymbNormalConeStruct *SymbTangentConeForCrvToData(const CagdCrvStruct 
							                *Crv,
							CagdBType Planar,
							SymbNormalConeStruct
							        *TangentCone);
const SymbNormalConeStruct *SymbTangentConeForCrvMalloc(const CagdCrvStruct 
							                *Crv,
							CagdBType Planar);
const SymbNormalConeStruct *SymbNormalConeForSrfToData(const CagdSrfStruct 
								        *Srf,
						       SymbNormalConeStruct
							         *NormalCone);
const SymbNormalConeStruct *SymbDirectionsConeAvgToData(
					     const CagdRType * const *Pts,
					     int NumPts,
					     CagdPointType PType,
					     SymbNormalConeStruct *NormalCone);
const SymbNormalConeStruct *SymbDirectionsConeOptToData(
					     const CagdRType * const *Pts,
					     int NumPts,
					     CagdPointType PType,
					     SymbNormalConeStruct *NormalCone);
const SymbNormalConeStruct *SymbDirectionsConeToData(
					      const CagdRType * const *Pts,
					      int NumPts,
					      CagdPointType PType,
					      SymbNormalConeStruct *NormalCone);
int SymbNormalConeForSrfDoOptimal(int Optimal);
const SymbNormalConeStruct *SymbNormalConeForSrfAvgToData(const CagdSrfStruct 
								         *Srf,
							  SymbNormalConeStruct
							         *NormalCone);
const SymbNormalConeStruct *SymbNormalConeForSrfOptToData(const CagdSrfStruct 
								         *Srf,
							  SymbNormalConeStruct
							         *NormalCone);
const SymbNormalConeStruct *SymbNormalConeForSrfMainAxisToData(
						  const CagdSrfStruct *Srf,
						  CagdVType MainAxis,
						  SymbNormalConeStruct *Cone);
CagdBType SymbNormal2ConesForSrf(const CagdSrfStruct *Srf,
				 CagdRType ExpandingFactor,
				 SymbNormalConeStruct *Cone1,
				 SymbNormalConeStruct *Cone2);
CagdBType SymbNormalConeOverlap(const SymbNormalConeStruct *NormalCone1,
				const SymbNormalConeStruct *NormalCone2);
SymbNormalConeStruct *SymbNormalConvexHullConeForSrf(const CagdSrfStruct *Srf,
						     CagdRType ***CH,
						     int *NPts);
CagdBType SymbNormalConvexHullConeOverlap(const SymbNormalConeStruct
					                        *NormalCone1,
					  const CagdRType **CH1,
					  int NPts1,
					  const SymbNormalConeStruct
					                        *NormalCone2,
					  const CagdRType **CH2,
					  int NPts2);

/******************************************************************************
* Routines to handle Reflection Lines.					      *
******************************************************************************/
void SymbRflctLnPrepSrf(CagdSrfStruct *Srf,
			const CagdVType ViewDir,
			const CagdVType LnDir);
CagdSrfStruct *SymbRflctLnGen(CagdSrfStruct *Srf,
			      const CagdVType ViewDir,
			      const CagdPType LnPt,
			      const CagdVType LnDir);
void SymbRflctLnFree(CagdSrfStruct *Srf);
void SymbRflctCircPrepSrf(CagdSrfStruct *Srf,
			  const CagdVType ViewDir,
			  const CagdPType SprCntr);
CagdSrfStruct *SymbRflctCircGen(CagdSrfStruct *Srf,
				const CagdVType ViewDir,
				const CagdPType SprCntr,
				CagdRType ConeAngle);
void SymbRflctCircFree(CagdSrfStruct *Srf);
void SymbHighlightLnPrepSrf(CagdSrfStruct *Srf,
			    const CagdVType LnDir);
CagdSrfStruct *SymbHighlightLnGen(CagdSrfStruct *Srf,
				  const CagdPType LnPt,
				  const CagdVType LnDir);
void SymbHighlightLnFree(CagdSrfStruct *Srf);

/******************************************************************************
* Routines to handle K-orthotomics, silhouettes, and isoclines.		      *
******************************************************************************/
CagdCrvStruct *SymbCrvOrthotomic(const CagdCrvStruct *Crv,
				 const CagdPType P,
				 CagdRType K);
CagdSrfStruct *SymbSrfOrthotomic(const CagdSrfStruct *Srf,
				 const CagdPType P,
				 CagdRType K);
struct IPPolygonStruct *SymbSrfSilhouette(const CagdSrfStruct *Srf,
					  const CagdVType VDir,
					  CagdRType SubdivTol,
					  CagdBType Euclidean);
struct IPPolygonStruct *SymbSrfPolarSilhouette(const CagdSrfStruct *Srf,
					       const CagdVType VDir,
					       CagdRType SubdivTol,
					       CagdBType Euclidean);
struct IPPolygonStruct *SymbSrfIsocline(const CagdSrfStruct *Srf,
					const CagdVType VDir,
					CagdRType Theta,
					CagdRType SubdivTol,
					CagdBType Euclidean);

/******************************************************************************
* Routines to construct surfaces algebraically.				      *
******************************************************************************/
CagdSrfStruct *SymbAlgebraicSumSrf(const CagdCrvStruct *Crv1,
				   const CagdCrvStruct *Crv2);
CagdSrfStruct *SymbAlgebraicProdSrf(const CagdCrvStruct *Crv1,
				    const CagdCrvStruct *Crv2);
CagdSrfStruct *SymbSwungAlgSumSrf(const CagdCrvStruct *Crv1,
				  const CagdCrvStruct *Crv2);
CagdSrfStruct *SymbOrthoNetSrf(const CagdCrvStruct *Crv1,
			       const CagdCrvStruct *Crv2,
			       int FrontSamples,
			       int LayerSamples);

/******************************************************************************
* Routines to construct dual curves and surfaces.			      *
******************************************************************************/
CagdCrvStruct *SymbCrvDual(const CagdCrvStruct *Crv);
CagdSrfStruct *SymbSrfDual(const CagdSrfStruct *Srf);


/******************************************************************************
* Routines to construct developable surface.				      *
******************************************************************************/
CagdSrfStruct *SymbSrfDevelopableCrvOnSrf(const CagdSrfStruct *Srf,
					  const CagdCrvStruct *Crv,
					  IrtRType Scale);
CagdSrfStruct *SymbSrfDevelopableSrfBetweenFrames(const CagdVType Frame1Pos,
						  const CagdVType Frame1Tan,
						  const CagdVType Frame1Nrml,
						  const CagdVType Frame2Pos,
						  const CagdVType Frame2Tan,
						  const CagdVType Frame2Nrml,
						  CagdRType OtherScale,
						  CagdRType Tension);
CagdSrfStruct *SymbSrfDevelopableSrfBetweenFrames2(const CagdVType Frame1Pos,
						   const CagdVType Frame1Tan,
						   const CagdVType Frame1Nrml,
						   const CagdVType Frame2Pos,
						   const CagdVType Frame2Tan,
						   const CagdVType Frame2Nrml,
						   CagdRType OtherScale,
						   CagdRType Tension,
						   int DOFs);

/******************************************************************************
* Routines to handle surface surface intersections.			      *
******************************************************************************/
CagdSrfStruct *SymbRuledRuledZeroSetFunc(CagdCrvStruct *C1,
					 CagdCrvStruct *C2,
					 CagdCrvStruct *D1,
					 CagdCrvStruct *D2);
CagdCrvStruct *SymbRuledRuledIntersection(CagdCrvStruct *C1,
					  CagdCrvStruct *C2,
					  CagdCrvStruct *D1,
					  CagdCrvStruct *D2,
					  CagdRType SubdivTol,
					  CagdCrvStruct **PCrvs1,
					  CagdCrvStruct **PCrvs2);
CagdCrvStruct *SymbRingRingIntersection(CagdCrvStruct *C1,
					CagdCrvStruct *r1,
					CagdCrvStruct *C2,
					CagdCrvStruct *r2,
					CagdRType SubdivTol,
					CagdCrvStruct **PCrvs1,
					CagdCrvStruct **PCrvs2);
CagdSrfStruct *SymbRingRingZeroSetFunc(CagdCrvStruct *C1,
				       CagdCrvStruct *r1,
				       CagdCrvStruct *C2,
				       CagdCrvStruct *r2);

/******************************************************************************
* Routines to handle knot removal of B-spline curves.             	      *
******************************************************************************/
CagdCrvStruct *SymbRmKntBspCrvRemoveKnots(const CagdCrvStruct *Crv,
					  CagdRType Tolerance);
CagdCrvStruct *SymbRmKntBspCrvCleanKnots(const CagdCrvStruct *Crv);
CagdSrfStruct *SymbRmKntBspSrfRemoveKnots(const CagdSrfStruct *CSrf,
					  CagdRType Tolerance);
CagdSrfStruct *SymbRmKntBspSrfRemoveKnotsDir(const CagdSrfStruct *CSrf,
					     CagdSrfDirType Dir,
					     CagdRType Tolerance);
CagdSrfStruct *SymbRmKntBspSrfCleanKnots(const CagdSrfStruct *Srf);


/******************************************************************************
* Routines to handle reverse engineering and surface recognition.	      *
******************************************************************************/
CagdBType SymbIsConstCrv(const CagdCrvStruct *Crv,
			 CagdCtlPtStruct *ConstVal,
			 CagdRType Eps);
CagdBType SymbIsZeroCrv(const CagdCrvStruct *Crv, CagdRType Eps);
CagdBType SymbIsCircularCrv(const CagdCrvStruct *Crv,
			    CagdPType Center,
			    CagdRType *Radius,
			    CagdRType Eps);
CagdBType SymbIsLineCrv(const CagdCrvStruct *Crv,
			CagdPType LnPos,
			CagdVType LnDir,
			CagdRType Eps);
CagdBType SymbIsConstSrf(const CagdSrfStruct *Srf,
			 CagdCtlPtStruct *ConstVal,
			 CagdRType Eps);
CagdBType SymbIsZeroSrf(const CagdSrfStruct *Srf, CagdRType Eps);
CagdBType SymbIsSphericalSrf(const CagdSrfStruct *Srf,
			     CagdPType Center,
			     CagdRType *Radius,
			     CagdRType Eps);
int SymbIsExtrusionSrf(const CagdSrfStruct *Srf,
		       CagdCrvStruct **Crv,
		       CagdVType ExtDir,
		       CagdRType Eps);
CagdBType SymbIsDevelopSrf(const CagdSrfStruct *Srf, CagdRType Eps);
int SymbIsRuledSrf(const CagdSrfStruct *Srf,
		   CagdCrvStruct **Crv1,
		   CagdCrvStruct **Crv2,
		   CagdRType Eps);
CagdBType SymbIsSrfOfRevSrf(const CagdSrfStruct *Srf,
			    CagdCrvStruct **CrossSec,
			    CagdPType AxisPos,
			    CagdVType AxisDir,
			    CagdRType Eps);
CagdBType SymbIsPlanarSrf(const CagdSrfStruct *Srf,
			  IrtPlnType Plane,
			  CagdRType Eps);

/******************************************************************************
* Routines to handle injectivity testing of Bspline surfaces.         	      *
******************************************************************************/
CagdBType SymbCubicBspInjective(CagdRType x[4][4], CagdRType y[4][4]);

/******************************************************************************
* Routines to handle blending of Bspline surfaces.	         	      *
******************************************************************************/
CagdCrvStruct *SymbClipCrvToSrfDomain(const CagdSrfStruct *Srf,
				      const CagdCrvStruct *UVCrv);
CagdSrfStruct *SymbShapeBlendOnSrf(CagdSrfStruct *Srf,
				   CagdCrvStruct *UVCrv,
				   const CagdCrvStruct *CrossSecShape,
				   CagdRType TanScale,
				   CagdRType Width,
				   const CagdCrvStruct *WidthField,
				   CagdRType Height,
				   const CagdCrvStruct *HeightField);
CagdSrfStruct *SymbShapeBlendSrf(const CagdCrvStruct *Pos1Crv,
				 const CagdCrvStruct *Pos2Crv,
				 const CagdCrvStruct *Dir1Crv,
				 const CagdCrvStruct *Dir2Crv,
				 const CagdCrvStruct *CrossSecShape,
				 const CagdCrvStruct *Normal);

/******************************************************************************
* Routines to handle bi arc approximations of curves.	         	      *
******************************************************************************/
SymbArcStruct *SymbCrvBiArcApprox(const CagdCrvStruct *Crv,
				  CagdRType Tolerance,
				  CagdRType MaxAngle);
CagdCrvStruct *SymbArcs2Crvs(const SymbArcStruct *Arcs);

/******************************************************************************
* Routines to handle cubic approximations of curves.	         	      *
******************************************************************************/
CagdCrvStruct *SymbCrvCubicApprox(const CagdCrvStruct *CCrv,
				  CagdRType Tolerance);

/******************************************************************************
* Routines to compare the similarity of freeforms.	         	      *
******************************************************************************/
SymbCrvRelType SymbCrvsCompare(const CagdCrvStruct *Crv1,
			       const CagdCrvStruct *Crv2,
			       CagdRType Eps,
			       CagdRType *StartOvrlpPrmCrv1,
			       CagdRType *EndOvrlpPrmCrv1,
			       CagdRType *StartOvrlpPrmCrv2,
			       CagdRType *EndOvrlpPrmCrv2);
CagdCrvStruct *SymbCanonicBzrCrv(const CagdCrvStruct *Crv, CagdRType Eps);
CagdCrvStruct *SymbBzrDegReduce(const CagdCrvStruct *Crv, CagdRType Eps);

/******************************************************************************
* Routines to handle lower envelopes of curve arrangments.	              *
******************************************************************************/
CagdCrvStruct *SymbCrvsLowerEnvelop(const CagdCrvStruct *Crvs,
				    CagdRType *Pt,
				    CagdRType Eps);
CagdCrvStruct *SymbSplitCrvsAtExtremums(const CagdCrvStruct *Crvs,
					int Axis,
					const CagdPType Pt,
					CagdRType Eps);

/******************************************************************************
* Routines to handle parameterization of regions enclosed in planar curve.    *
******************************************************************************/
CagdSrfStruct *Symb2DCrvParameterizing2Crvs(const CagdCrvStruct *Crv1,
					    const CagdCrvStruct *Crv2,
					    int Dir,
					    CagdBType ForceMatch);
CagdSrfStruct *Symb2DCrvParamerize2Prms(const CagdCrvStruct *Crv,
					CagdRType T1,
					CagdRType T2,
					int Dir,
					CagdRType *Error);
CagdSrfStruct *Symb2DCrvParameterizeDomain(const CagdCrvStruct *UVCrv,
					   CagdRType Eps);
void SymbSrfSmoothInternalCtlPts(CagdSrfStruct *Srf, CagdRType Weight);
CagdRType SymbSrfJacobianImprove(CagdSrfStruct *Srf,
				 CagdRType StepSize,
				 int MaxIter);

/******************************************************************************
* Routines to handle curv curve intersection related functionality	      *
******************************************************************************/
CagdCrvStruct *SymbGet2CrvsIntersectionRegions(const CagdCrvStruct *Crv1,
					       const CagdCrvStruct *Crv2,
					       CagdRType Eps);
CagdRType SymbGet2CrvsIntersectionAreas(const CagdCrvStruct *Crv1,
					const CagdCrvStruct *Crv2,
					CagdRType Eps);
int SymbGet2CrvsInterDAreaDCtlPts(CagdCrvStruct *Crv1,
				  CagdCrvStruct *Crv2,
				  CagdRType Eps,
				  CagdRType **InterDomains,
				  CagdRType **dAreadPts);
CagdRType *SymbGetCrvSubRegionAlphaMatrix(const CagdCrvStruct *Crv,
					  CagdRType t1,
					  CagdRType t2,
					  int *Dim);

/******************************************************************************
* Routines to handle approximating a polyline by a curve                      *
******************************************************************************/

CagdCrvStruct *SymbCrvLstSqrAprxPlln(const CagdCtlPtStruct *Polyline,
				     CagdRType ExpectedError,
				     int Order,
				     CagdRType NumericTol,
				     CagdBType ForceC1Continuity,
				     CagdRType C1DiscontThresh,
				     SymbCrvLSErrorMeasureType ErrMeasure);

/******************************************************************************
* Error handling.							      *
******************************************************************************/
SymbSetErrorFuncType SymbSetFatalErrorFunc(SymbSetErrorFuncType ErrorFunc);
const char *SymbDescribeError(SymbFatalErrorType ErrorNum);
void SymbFatalError(SymbFatalErrorType ErrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_SYMB_LIB_H */
