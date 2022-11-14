/*****************************************************************************
* Module to support IGA (isogeometric analysis) related queries of trivars.  *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
* Written by:  Gershon Elber				Ver 1.0, June 2012   *
*****************************************************************************/

#ifndef IRIT_TRIV_IGA_H
#define IRIT_TRIV_IGA_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/triv_lib.h"

#define TRIV_IGA_FATAL_ERROR(H, Str) { \
    fprintf(stderr, "%s:%d :: %s\n\tIGA Error: %s\n", \
	    __FILE__, __LINE__, Str, \
	    TrivIGADescribeError(TrivIGAGetLastError(H, TRUE))); \
            exit(1); \
    }

typedef enum {
    TRIV_IGA_GEOMETRY_TYPE,
    TRIV_IGA_SCALAR_FIELD_TYPE,
    TRIV_IGA_VECTOR_FIELD_TYPE
} TrivIGAFieldType;

typedef enum {
    TRIV_IGA_EVAL_VALUE,
    TRIV_IGA_EVAL_DU,
    TRIV_IGA_EVAL_DV,
    TRIV_IGA_EVAL_DW,
    TRIV_IGA_EVAL_D2U,
    TRIV_IGA_EVAL_D2V,
    TRIV_IGA_EVAL_D2W,
    TRIV_IGA_EVAL_DUDV,
    TRIV_IGA_EVAL_DUDW,
    TRIV_IGA_EVAL_DVDW,
    TRIV_IGA_EVAL_BNRML
} TrivIGAEvalType;

typedef enum {
    TRIV_IGA_BOUNDARY_FIXED = 0,
    TRIV_IGA_BOUNDARY_PRESCRIBED,
    TRIV_IGA_NUM_BOUNDARY_TYPES,
    TRIV_IGA_INVALID_BOUNDARY_TYPE
} TrivIGANodeBoundaryType;

typedef enum {
    TRIV_IGA_ERR_NO_ERROR,
    TRIV_IGA_ERR_INVALID_ARRANGMENT,
    TRIV_IGA_ERR_INVALID_INPUT,
    TRIV_IGA_ERR_RATIONAL_NO_SUPPORT,
    TRIV_IGA_ERR_BEZIER_NO_SUPPORT,
    TRIV_IGA_ERR_INVALID_PATCH_INDEX,
    TRIV_IGA_ERR_INVALID_DIR,
    TRIV_IGA_ERR_ID_NOT_FOUND,
    TRIV_IGA_ERR_INVALID_EVAL_TYPE,
    TRIV_IGA_ERR_NOT_BNDRY_PARAMS,
    TRIV_IGA_ERR_NOT_ZERO_ONE_PARAM,
    TRIV_IGA_ERR_EXISTING_MATERIAL_ADDED,
    TRIV_IGA_ERR_INVALID_FIELD_ATTRIBUTES_FORMAT,
    TRIV_IGA_ERR_UNDEFINED_FIELD_MATERIAL,
    TRIV_IGA_ERR_INCOMPATIBLE_PT_TYPE,
    TRIV_IGA_ERR_SRFS_NOT_COMPATIBLE,
    TRIV_IGA_ERR_ID_OUT_OF_RANGE,
    TRIV_IGA_ERR_SRFS_NOT_SUBSPACES,
    TRIV_IGA_ERR_UNDEFINE_ERR
} TrivIGAErrorType;

typedef int TrivIGAArrangementID;
typedef int TrivIGATVID;
typedef int TrivIGAMaterialID;

typedef struct TrivIGACtrlPtStruct {
    CagdPointType PtType;
    CagdRType Coord[CAGD_MAX_PT_SIZE];
    int ID;
} TrivIGACtrlPtStruct;

typedef struct TrivIGAAdjacencyInfoStruct {
    const TrivTVStruct *AdjTV;
    int AdjBndryIdx;   /* 0 to 5 for Umin, Umax, Vmin, Vmax, Wmin, Wmax. */
    CagdBType ReverseU, ReverseV, ReverseUwithV;
    CagdBType SameSpace;
} TrivIGAAdjacencyInfoStruct;

#define TRIV_IGA_INVALID_ARRANGEMENT_ID	    0
#define TRIV_IGA_INVALID_TV_ID		    0
#define TRIV_IGA_INVALID_FACE_ID	    -1
#define TRIV_IGA_INVALID_TV_CTLPT_ID	    -1
#define TRIV_IGA_INVALID_MATERIAL_ID	    0

#define TRIV_IGA_MAX_MATERIAL_NAME_LEN		256
#define TRIV_IGA_MAX_XML_PROPERTY_LEN		16
#define TRIV_IGA_MAX_MATERIAL_PROPERTIES	8
#define TRIV_IGA_MAX_MATERIALS			16
#define TRIV_IGA_MAX_FIELD_TYPE_LEN		256

typedef struct TrivIGAXMLProperty {
    char Name[TRIV_IGA_MAX_MATERIAL_NAME_LEN];
    char Value[TRIV_IGA_MAX_MATERIAL_NAME_LEN];
} TrivIGAXMLProperty;

typedef struct TrivIGAMaterialStruct {
    TrivIGAMaterialID Id;
    char Name[TRIV_IGA_MAX_MATERIAL_NAME_LEN];
    char Type[TRIV_IGA_MAX_MATERIAL_NAME_LEN];
    TrivIGAXMLProperty Properties[TRIV_IGA_MAX_XML_PROPERTY_LEN];
    int NumProperties;
} TrivIGAMaterialStruct;

struct TrivIGAArrangementStruct;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef void (*TrivIGANeighboringConstraintCallBackType)(
					    struct TrivIGAArrangementStruct *H,
					    CagdBType IsRational,
					    int MaxCoord,
					    const int *FirstIDs,
					    int FirstIDsSize,
					    int SecondID,
					    int SecondIdx,
					    const CagdSrfStruct **Mat,
					    void *CallBackData);

/* basic build blocks of IGA arrangements: */

int TrivIGANewArrangement(TrivIGAArrangementID *NewArgmntID);
int TrivIGANewField(TrivIGAArrangementID ArgmntID,
		    const char *FieldAttributes);
struct TrivIGATVStruct *TrivIGANewTV(TrivIGAArrangementID ArgmntID,
				     TrivTVStruct *TV);
struct TrivIGATVStruct *TrivIGAUpdateTV(TrivIGAArrangementID ArgmntID,
				        TrivTVStruct *ExistingTV,
				        TrivTVStruct *NewTV);
int TrivIGAArrangementComplete(TrivIGAArrangementID ArgmntID);
int TrivIGAPrintTVContent(TrivIGAArrangementID ArgmntID,
			  const TrivTVStruct *TV);
int *TrivIGAGetGlblMaxIDs(TrivIGAArrangementID ArgmntID, int *IDs);
int *TrivIGAGetCtlPtIDRange(TrivIGAArrangementID ArgmntID,
			    const TrivTVStruct *TV,
			    int *IDs);
int TrivIGAGetNumBzrElements(TrivIGAArrangementID ArgmntID,
			     const TrivTVStruct *TV,
			     int *NumU,
			     int *NumV,
			     int *NumW);
TrivIGACtrlPtStruct *TrivIGAGetBzrElementCtrlPts(
				           TrivIGAArrangementID ArgmntID,
					   const TrivTVStruct *TV,
					   int IndexU,
					   int IndexV,
					   int IndexW);
const CagdRType *TrivIGAGetKnotInterval(TrivIGAArrangementID ArgmntID,
					const TrivTVStruct *TV,
					TrivTVDirType Dir,
					int BzrIntervalIndex);
int TrivIGAUpdateCtrlPtsPositions(TrivIGAArrangementID ArgmntID,
				  int NumCtrlPts,
				  const TrivIGACtrlPtStruct *DeltaVals);
int TrivIGASetCtrlPtsPositions(TrivIGAArrangementID ArgmntID,
			       int NumCtrlPts,
			       const TrivIGACtrlPtStruct *Vals);
const TrivIGACtrlPtStruct *TrivIGATVEvalToData(TrivIGAArrangementID ArgmntID,
					       const TrivTVStruct *TV,
					       TrivIGAEvalType EvalType,
					       int IndexU,
					       int IndexV,
					       int IndexW,
					       CagdRType U,
					       CagdRType V,
					       CagdRType W,
					       TrivIGACtrlPtStruct *CtlPt);
CagdRType *TrivIGATVEvalBasis(TrivIGAArrangementID ArgmntID,
			      const TrivTVStruct *TV,
			      TrivIGAEvalType EvalType,
			      TrivTVDirType Dir,
			      int Index,
			      CagdRType t,
			      CagdRType *Basis);

int TrivIGAFreeArrangement(TrivIGAArrangementID ArgmntID);

/* Trivar constructors/modifiers: */
int TrivIGASetDefaultSeeding(TrivIGAArrangementID ArgmntID,
			     TrivTVDirType Dir,
			     CagdRType Alpha,
			     int NumIntervals);
int TrivIGASetDefaultDomain(TrivIGAArrangementID ArgmntID,
			    TrivTVDirType Dir,
			    CagdRType Min,
			    CagdRType Max);
TrivIGATVID TrivIGAAddTrivar(TrivIGAArrangementID ArgmntID,
                             const TrivTVStruct *TV,
			     int ID);
TrivIGATVID TrivIGAExtrudeTV(TrivIGAArrangementID ArgmntID, 
			     const CagdSrfStruct *Srf,
			     const IrtVecType Vec,
			     int ID);
TrivIGATVID TrivIGAExtrudeTV2(TrivIGAArrangementID ArgmntID,
			      const CagdSrfStruct *Srf,
			      const CagdCrvStruct *Crv,
			      int ID);
TrivIGATVID TrivIGATVofRevol(TrivIGAArrangementID ArgmntID,
			     const CagdSrfStruct *Srf,
			     const IrtPtType AxisPoint,
			     const IrtVecType AxisVector,
			     CagdRType StartAngleRad, 
			     CagdRType EndAngleRads,
			     CagdBType IsRational,
			     int ID);
TrivIGATVID TrivIGATVFromSurfaces(TrivIGAArrangementID ArgmntID,
				  const CagdSrfStruct *SrfList,
				  int OtherOrder,
				  CagdBType IsInterpolating,
				  int ID);
TrivIGATVID TrivIGATVFromSurfaces2(TrivIGAArrangementID ArgmntID,
				   const CagdSrfStruct *Srf,
				   IrtHmgnMatType Transforms[],
				   int NumTransforms,
				   unsigned int OtherOrder,
				   CagdBType IsInterpolating,
				   int ID);
struct TrivIGATVStruct *TrivIGATVRefine(TrivIGAArrangementID ArgmntID,
				        TrivIGATVID TVID,
					TrivTVDirType Dir,
					CagdRType t);
struct TrivIGATVStruct *TrivIGATDegreeRaise(TrivIGAArrangementID ArgmntID,
					    TrivIGATVID TVID,
					    TrivTVDirType Dir);

/* Query functions. */

TrivIGATVID TrivIGADataManagerGetTrivID(const TrivTVStruct *TV);
TrivTVStruct *TrivIGADataManagerGetTrivariate(TrivIGATVID TVID);
struct TrivIGATVStruct *TrivIGADataManagerGetIGATrivariate(TrivIGATVID TVID);

int TrivIGAGetFaceNeighboringTVs(TrivIGAArrangementID ArgmntID,
				 const TrivTVStruct *TV,
				 TrivIGAAdjacencyInfoStruct *AdjInfo);
int *TrivIGAGetEdgeNeighboringTVs(TrivIGAArrangementID ArgmntID,
				  const TrivTVStruct *TV);
int *TrivIGAGetVrtxNeighboringTVs(TrivIGAArrangementID ArgmntID,
				  const TrivTVStruct *TV);
TrivIGATVID *TrivIGAGetAllTVs(TrivIGAArrangementID ArgmntID);
TrivTVStruct *TrivIGAGetTV(TrivIGAArrangementID ArgmntID, TrivIGATVID TVID);
int *TrivIGAGetTVFaceCtlPtsIDs(TrivIGAArrangementID ArgmntID,
			       TrivIGATVID TVID,
			       int FaceID);
CagdSrfStruct *TrivIGAGetTVFaceAsSrf(TrivIGAArrangementID ArgmntID,
				     TrivIGATVID TVID,
				     int FaceID);
int *TrivIGAGetTVCtlPtsIndices(TrivIGAArrangementID ArgmntID,
			       TrivIGATVID TVID);
CagdCtlPtStruct *TrivIGAGetCtlPt(TrivIGAArrangementID ArgmntID,
				 int Index,
				 CagdCtlPtStruct *CtlPt);
TrivIGAMaterialID TrivIGAGetMaterial(TrivIGAArrangementID ArgmntID,
				     TrivIGATVID TVID);
void TrivIGAGenNeighboringConstraints(TrivIGAArrangementID ArgmntID,
				      void *CallbackData,
				      TrivIGANeighboringConstraintCallBackType
				               NeighboringConstraintCallBack);
int TrivIgaGenOneFaceNeighboringConstraints(
				     TrivIGAArrangementID ArgmntID,
				     TrivIGANeighboringConstraintCallBackType
					       NeighboringConstraintCallBack,
				     const TrivTVStruct *TV1,
				     int FaceID1,
				     const TrivTVStruct *TV2,
				     int FaceID2,
				     void *CallbackData);

/* Export to XML related functionality. */

TrivIGAMaterialID TrivIGANewMaterial(TrivIGAArrangementID ArgmntID,
				     const char *MaterialStr);
TrivIGAMaterialStruct *TrivIGAParseMaterial(const char *MaterialStr);
TrivIGAMaterialID TrivIGAAddMaterial(TrivIGAArrangementID ArgmntID,
				     TrivIGAMaterialStruct *Material);
int TrivIGALoadMaterialXML(const char *FileName,
			   TrivIGAMaterialStruct **Materials,
			   int *NumMaterials);
int TrivIGAExportToXML(TrivIGAArrangementID ArgmntID,
                       const char *FileName,
                       const char *TemplateFileName);
int TrivIGALoadMaterialFromXML(TrivIGAArrangementID ArgmntID,
			       const char *FileName);

int TrivIGAAddBoundaryFace(TrivIGAArrangementID ArgmntID,
			   const TrivTVStruct *TV,
			   TrivTVBndryType BoundaryType,
			   TrivIGANodeBoundaryType NodeBoundaryType,
			   const char *BoundaryAxisConditions,
			   CagdRType Value);
int TrivIGAAddBoundaryFace2(TrivIGAArrangementID ArgmntID,
			    const TrivTVStruct *TV,
			    TrivTVBndryType BoundaryType,
			    TrivIGANodeBoundaryType NodeBoundaryType,
			    const char *BoundaryAxisConditions,
			    CagdRType Value);
int TrivIGAAddBoundaryFaceByPt(TrivIGAArrangementID ArgmntID,
			       const TrivTVStruct *TV,
			       const CagdPType Pt,
			       TrivIGANodeBoundaryType NodeBoundary,
			       const char *BoundaryAxisConditions,
			       CagdRType Value);
int *TrivIGAGetBoundaryFaceByPtToData(TrivIGAArrangementID ArgmntID,
				      const TrivTVStruct *TV,
				      const CagdPType Pt,
				      int *ReturnedIDs);

TrivIGAErrorType TrivIGAGetLastError(TrivIGAArrangementID ArgmntID,
				     int Reset);
const char *TrivIGADescribeError(TrivIGAErrorType ErrorNum);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_TRIV_IGA_H */
