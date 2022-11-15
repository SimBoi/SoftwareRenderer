/******************************************************************************
* unstrct_grid.c - Implementation of unstructured grid.	                      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Jinesh Machchhar and Gershon Elber, Sep. 2017.		      *
******************************************************************************/

#ifndef IRIT_USER_TOPO_LIB_H
#define IRIT_USER_TOPO_LIB_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/bool_lib.h"
#include "inc_irit/cagd_lib.h"
#include "inc_irit/geom_lib.h"
#include "inc_irit/mvar_lib.h"
#include "inc_irit/user_lib.h"
#include "inc_irit/allocate.h"
#include "inc_irit/attribut.h"
#include "inc_irit/iritprsr.h"

/* Unstructured grid package. */

typedef enum {
    USER_UG_CREATE,
    USER_UG_FREE,
    USER_UG_SET_POINTS,
    USER_UG_ADD_POINTS,
    USER_UG_EXTRACT_POINTS,
    USER_UG_MERGE_POINTS,
    USER_UG_MODIFY_POINT,
    USER_UG_ADD_CELL,
    USER_UG_APPEND_UG,
    USER_UG_UPDATE,
    USER_UG_PURGE_UNUSED,
    USER_UG_ID_TO_CELL,
    USER_UG_CELL_TO_ID,
    USER_UG_SET_FILTER_CB_FUNC,
    USER_UG_FILTER_GRID,
    USER_UG_DATA,
    USER_UG_CRV_BNDRY_FILTER,
    USER_UG_SRF_BNDRY_FILTER,
    USER_UG_TRIV_BNDRY_FILTER,
    USER_UG_SET_POINT_ATTR,    
    USER_UG_GET_POINT_ATTR,
    USER_UG_SET_CELL_ATTR,
    USER_UG_GET_CELL_ATTR,
    USER_UG_GET_ADJACENCY_LIST,
    USER_UG_GET_PTID_CELLID_LIST,
    USER_UG_GET_CELL_PTID_LIST,
    USER_UG_GET_UG_POINT_LIST,
    USER_UG_SEQ_POINT_IDS,
    USER_UG_SEQ_CELL_IDS,
    USER_UG_ADD_OBJECT_TO_FIELD,
    USER_UG_GET_FIELD,
    USER_UG_ADD_NEW_CELL,
    USER_UG_GET_BEZIER_PATCHES,
    USER_UG_REFINE_CELLS,
    USER_UG_PT_CELL_SELECTOR,
    USER_UG_POLY_CELL_SELECTOR,
    USER_UG_REFINE_CELLS_AT_PT,
    USER_UG_REFINE_UNREF_CELLS,
    USER_UG_WRITE_GRID_TO_FILE,
    USER_UG_READ_GRID_FROM_FILE,
    USER_UG_GET_LINEAR_PATCHES
} UserTopoUnstructGridOpType;

typedef enum {
    USER_UG_READING_HEADER,
    USER_UG_READING_GRID,
    USER_UG_READING_PATCH,
    USER_UG_READING_DEGREE,
    USER_UG_READING_NCTLPTS,
    USER_UG_READING_KNOTS,
    USER_UG_READING_WEIGHTS,
    USER_UG_READING_CONNECTIVITY,
    USER_UG_READING_POINTS,
    USER_UG_CREATING_PATCH,
    USER_UG_CREATING_GRID,
} UserTopoUnstrctGridStateType;

typedef enum {
    USER_UG_ATTR_INT_TYPE,
    USER_UG_ATTR_REAL_TYPE,
    USER_UG_ATTR_STR_TYPE,  
} UserTopoUnstrctGridAttrType;

typedef enum {
    USER_UG_CLOSE_NONE_TYPE,
    USER_UG_CLOSE_FACE_TYPE,
    USER_UG_CLOSE_EDGE_TYPE,
    USER_UG_CLOSE_CORNER_TYPE,  
} UserTopoUnstrctGridClosestEntityType;

typedef enum {
    USER_TOPO_ADJ_REL_NONE = 0,
    USER_TOPO_ADJ_REL_ONE_PT,
    USER_TOPO_ADJ_REL_OTHER_PT,
    USER_TOPO_ADJ_REL_ALL_PT
} UserTopoAdjRelType;

/* Unstructured grid package. */
/* #define USER_UNSTRCT_GRID_REF_MAT_SAVE_MAT        Enable to save as attr. */
#define USER_UNSTRCT_GRID_REF_MAT_ATTR_ID IRIT_ATTR_CREATE_ID(_KnotRefineMatrix)
#define USER_TOPO_PTID_ATTR_ID		IRIT_ATTR_CREATE_ID(PointIDs)
#define USER_TOPO_ENTID_ATTR_ID		IRIT_ATTR_CREATE_ID(CellID)
#define USER_TOPO_UREF_IDX_ATTR_ID	IRIT_ATTR_CREATE_ID(URefIdx)
#define USER_TOPO_VREF_IDX_ATTR_ID	IRIT_ATTR_CREATE_ID(VRefIdx)
#define USER_TOPO_WREF_IDX_ATTR_ID	IRIT_ATTR_CREATE_ID(WRefIdx)
#define USER_TOPO_SRFDIST_SUBD_TOL  1e-3

typedef struct UserTopoCellRefStruct {
    struct UserTopoCellRefStruct *Pnext;
    IPAttributeStruct *Attr;    
    IPObjectStruct *Cell;
} UserTopoCellRefStruct;

typedef struct UserTopoAdjRelStruct {
    struct UserTopoAdjRelStruct *Pnext;
    IPAttributeStruct *Attr;
    UserTopoCellRefStruct *CellRef;
    UserTopoAdjRelType AdjType; 
} UserTopoAdjRelStruct;

typedef struct UserTopoAdjStruct {
    struct UserTopoAdjStruct *Pnext;
    IPAttributeStruct *Attr;    
    UserTopoCellRefStruct *CellRef;
    UserTopoAdjRelStruct *AdjRelList;
} UserTopoAdjStruct;

typedef struct UserTopoUnstrctGeomPtStruct {
    IPAttributeStruct *Attr;
    CagdPType Pt;    
    int ID;
} UserTopoUnstrctGeomPtStruct;

typedef struct UserTopoUnstrctGeomStruct {
    struct UserTopoUnstrctGeomStruct *Pnext;
    IPAttributeStruct *Attr;
    struct UserTopoUnstrctGeomStruct *PnextRetUd;
    UserTopoUnstrctGeomPtStruct *PtsVec;	         /* All the points. */
    int NumPts;
    int _MaxNumPts;
    int _NextEntId;
    int UnstrctGeomID;				      /* The ID of this UG. */
    IPObjectStruct *CellList;		           /* List of all entities. */
    UserTopoAdjStruct *_AdjList;	        /* List of all adjacencies. */
    IPObjectStruct *_Field;	          /* List of unstructured elements. */
} UserTopoUnstrctGeomStruct;

typedef int *(*UserTopoFilterGridCBFuncType) (
					const UserTopoUnstrctGeomStruct *UG);

typedef struct UserTopoUnstrctGeomParamStruct {
    int UdId, UdId2;
    int Idx1, Idx2, Idx3;
    struct {
        UserTopoUnstrctGeomPtStruct *PtVec;
	int PtVecLen;
    } PointSet; 
    struct {
	int *IdVec;
	int IdVecLen;
    } IDSet;
    CagdBType IdentifyPtsNoMerge;
    IPObjectStruct *Cell;
    UserTopoFilterGridCBFuncType FilterCBFunc;
    /* following for attribute setting. */
    struct {
	int NumAttr;
	char **AttrNames;
	void *AttrValVec;
	int *AttrValVecLen;	
	UserTopoUnstrctGridAttrType AttrType;
    } AttrParamStruct;
    int UpdateGeom;
    UserTopoUnstrctGridClosestEntityType ClosestEntity;
    CagdRType RefineDfltLength, RefineEdgeRatio;
    CagdRType Eps;
    CagdPType Pt;
    IPPolygonStruct *Poly;
    IPObjectStruct *AttrMark;
    char FileName[IRIT_LINE_LEN_LONG];
} UserTopoUnstrctGeomParamStruct;

typedef struct UserTopoUnstrctGeomReturnStruct {
    CagdBType Success;
    int UdId;
    int UdId2;
    int UdId3;
    int Id;
    int FaceIdx;
    int EdgeIdx;
    int CrnrIdx;
    int UVWMax[3];
    int Length;
    int NumPts;
    int Type;
    int *CloneMap;
    int *RealIDMap;
    int *CellIDVec;
    void **AttrValVec;
    IPObjectStruct *Cell;
    struct {
	int NumCells;
	int NumCells123D[3];
	int NumPtAttr;
	int NumCellAttr;
    } UGDataStruct;
    struct {
	UserTopoUnstrctGeomPtStruct *PtVec;
	int PtVecLen;
    } PointSet;
    UserTopoFilterGridCBFuncType FilterCBFunc;
} UserTopoUnstrctGeomReturnStruct;


#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

UserTopoUnstrctGeomStruct *UserTopoUnstrctGeomNew(void);
void UserTopoUnstrctGeomPtCopyData(UserTopoUnstrctGeomPtStruct *Dest,
				   const UserTopoUnstrctGeomPtStruct *Src);
void UserTopoUnstrctGeomFree(UserTopoUnstrctGeomStruct *Ud);
void UserTopoUDData(const UserTopoUnstrctGeomStruct *Ud,
		    UserTopoUnstrctGeomReturnStruct *Data);
CagdBType UserTopoSetPoints(UserTopoUnstrctGeomStruct *Ud,
			    UserTopoUnstrctGeomPtStruct *Pts,
			    int NumPt);
UserTopoUnstrctGeomStruct *UserTopoAddPoints(
				       const UserTopoUnstrctGeomStruct *Ud,
				       const UserTopoUnstrctGeomPtStruct *Pts,
				       int NumPt,
				       int **RealIDMap);
CagdBType UserTopoAddNewCell(UserTopoUnstrctGeomStruct *Ud,
			     const IPObjectStruct *Cell,
			     int UpdateGeom,
			     int *CellID);
CagdBType UserTopoAddNewCell2(UserTopoUnstrctGeomStruct *Ud,
			     const IPObjectStruct *Cell,
			     int UpdateGeom,
			     int *CellID);

UserTopoUnstrctGeomStruct *UserTopoMergePoints(
					  const UserTopoUnstrctGeomStruct *Ud,
					  CagdRType Eps,
					  CagdBType IdentifyNoMerge,
					  const CagdBType *MergePtIndices,
					  int **MergedIDMap,
					  int *MergeIDMapLen);
UserTopoUnstrctGeomStruct *UserTopoPurgeUnusedPts(
					  const UserTopoUnstrctGeomStruct *Ud);
void UserTopoPtsOfCellsWithAttrib(const UserTopoUnstrctGeomStruct *Ud,
				  UserTopoUnstrctGridAttrType AttrType,
				  int NumAttr,
				  const int *NumAttrVals,
				  const char **AttrNames,
				  const void *AttrVals,
				  CagdBType **PtIndxVec);
CagdBType UserTopoAddCell(UserTopoUnstrctGeomStruct *Ud,
		          const int *PtIdVec,
		          int PtIdVecLen,
		          IPObjectStruct *Cell,
		          int *CellID);
int UserTopoModifyPoint(UserTopoUnstrctGeomStruct *Ud,
			int PtId,
			const UserTopoUnstrctGeomPtStruct *Pt);
int UserTopoSetPointIntAttr(UserTopoUnstrctGeomStruct *Ud,
			    int PtId,
			    const char *AttrName,
			    int AttrValue);
int UserTopoSetPointIntAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       const int *PtIdVec,
			       int NumPtId,
			       const char *AttrName,
			       const int *AttrValueVec,
			       int NumVals);
int UserTopoSetPointRealAttr(UserTopoUnstrctGeomStruct *Ud,
			     int PtId,
			     const char *AttrName,
			     CagdRType AttrValue);
int UserTopoSetPointRealAttrVec(UserTopoUnstrctGeomStruct *Ud,
			        const int *PtIdVec,
			        int NumPtId,
			        const char *AttrName,
			        const CagdRType *AttrValueVec,
			        int NumVals);
int UserTopoSetPointStrAttr(UserTopoUnstrctGeomStruct *Ud,
			    int PtId,
			    const char *AttrName,
			    const char *AttrValue);
int UserTopoSetPointStrAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       const int *PtIdVec,
			       int NumPtId,
			       const char *AttrName,
			       const char **AttrValueVec,
			       int NumVals);
int UserTopoSetCellIntAttr(UserTopoUnstrctGeomStruct *Ud,
			     IPObjectStruct *Cell,
			     const char *AttrName,
			     int AttrValue);
int UserTopoSetCellIntAttrVec(UserTopoUnstrctGeomStruct *Ud,
			      const int *CellIdVec,
			      int NumCellId,
			      const char *AttrName,
			      const int *AttrValueVec,
			      int AttrValueVecLen);
int UserTopoSetCellRealAttr(UserTopoUnstrctGeomStruct *Ud,
			    IPObjectStruct *Cell,
			    const char *AttrName,
			    CagdRType AttrValue);
int UserTopoSetCellRealAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       const int *CellIdVec,
			       int NumCellId,
			       const char *AttrName,
			       const CagdRType *AttrValueVec,
			       int NumVals);
int UserTopoSetCellStrAttr(UserTopoUnstrctGeomStruct *Ud,
			   IPObjectStruct *Cell,
			   const char *AttrName,
			   const char *AttrValue);
int UserTopoSetCellStrAttrVec(UserTopoUnstrctGeomStruct *Ud,
			      const int *CellIdVec,
			      int NumCellId,
			      const char *AttrName,
			      const char **AttrValueVec,
			      int NumVals);
UserTopoUnstrctGeomStruct *UserTopoAppendUnstrctGeoms(
					const UserTopoUnstrctGeomStruct *UdA,
					const UserTopoUnstrctGeomStruct *UdB,
					CagdRType Eps,
					int **RealIDMap);
UserTopoUnstrctGeomStruct *UserTopoAssignSequentialPointIDs(
					const UserTopoUnstrctGeomStruct *Ud);
UserTopoUnstrctGeomStruct *UserTopoAssignSequentialCellIDs(
					const UserTopoUnstrctGeomStruct *Ud);
void UserTopoUnstrctGeomUpdate(UserTopoUnstrctGeomStruct **Ud,
			       CagdRType Eps);
int UserTopoObjectToId(const UserTopoUnstrctGeomStruct *Ud,
		       const IPObjectStruct *Cell);
IPObjectStruct *UserTopoIdToObject(const UserTopoUnstrctGeomStruct *Ud,
				   int Id);
int UserTopoGetPointIntAttr(const UserTopoUnstrctGeomStruct *Ud,
			    int PtId,
			    char *AttrName);
int UserTopoGetPointIntAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       int *PtIdVec,
			       int NumPtId,
			       char *AttrName,
			       int **AttrValueVec);
CagdRType UserTopoGetPointRealAttr(const UserTopoUnstrctGeomStruct *Ud,
				   int PtId,
				   char *AttrName);
int UserTopoGetPointRealAttrVec(UserTopoUnstrctGeomStruct *Ud,
			        int *PtIdVec,
			        int NumPtId,
			        char *AttrName,
			        CagdRType **AttrValueVec);
const char *UserTopoGetPointStrAttr(const UserTopoUnstrctGeomStruct *Ud,
				    int PtId,
				    char *AttrName);
int UserTopoGetPointStrAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       int *PtIdVec,
			       int NumPtId,
			       char *AttrName,
			       const char ***AttrValueVec);
int UserTopoGetCellIntAttr(const UserTopoUnstrctGeomStruct *Ud,
			   int CellId,
			   char *AttrName);
int UserTopoGetCellIntAttrVec(UserTopoUnstrctGeomStruct *Ud,
			      int *CellIdVec,
			      int NumCellId,
			       char *AttrName,
			      int **AttrValueVec);
CagdRType UserTopoGetCellRealAttr(const UserTopoUnstrctGeomStruct *Ud,
				  int CellId,
				  char *AttrName);
int UserTopoGetCellRealAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       int *CellIdVec,
			       int NumCellId,
			       char *AttrName,
			       CagdRType **AttrValueVec);
const char *UserTopoGetCellStrAttr(const UserTopoUnstrctGeomStruct *Ud,
				   int CellId,
				   char *AttrName);
int UserTopoGetCellStrAttrVec(UserTopoUnstrctGeomStruct *Ud,
			      int *CellIdVec,
			      int NumCellId,
			      char *AttrName,
			      const char ***AttrValueVec);
int UserTopoPtsOfCell(const UserTopoUnstrctGeomStruct *Ud,
		      int EntId,
		      int **PtIds);
int UserTopoAllEntitiesWithPoint(const UserTopoUnstrctGeomStruct *Ud,
				 int PtId,
				 int **EntIds);
int UserTopoNumOfEntOfType(const UserTopoUnstrctGeomStruct *Ud,
			   IPObjStructType Type);
int UserTopoCellsAdjacentToCell(const UserTopoUnstrctGeomStruct *Ud,
				int CellID,
				int **EntIDs);
int UserTopoGetCellAttrThreshold(const UserTopoUnstrctGeomStruct *Ud,
				   char *AttrName,
				   int AttrMinVal,
				   int AttrMaxVal,
				   int **EntIDs);
int UserTopoGetPointAttrThreshold(const UserTopoUnstrctGeomStruct *Ud,
				  char *AttrName,
				  int AttrMinVal,
				  int AttrMaxVal,
				  int **PtIDs);
UserTopoUnstrctGeomStruct *UserTopoApplyFilterToGrid(
					  const UserTopoUnstrctGeomStruct *Ud,
					  CagdBType PurgeUnusedPts);
UserTopoUnstrctGeomStruct *UserTopoCrvBndryFilter(
					  const UserTopoUnstrctGeomStruct *Ud);
UserTopoUnstrctGeomStruct *UserTopoSrfBndryFilter(
					  const UserTopoUnstrctGeomStruct *Ud);
UserTopoUnstrctGeomStruct *UserTopoTrivBndryFilter(
					  const UserTopoUnstrctGeomStruct *Ud);
UserTopoFilterGridCBFuncType UserTopoSetFilterGridCallBackFunc(
					 UserTopoFilterGridCBFuncType NewFunc);
int UserTopoCellClosestToPoint(const UserTopoUnstrctGeomStruct *Ud,
			       const CagdPType Pt,
			       UserTopoUnstrctGridClosestEntityType
							        ClosestEntity,
			       int *FaceIdx,
			       int *EdgeIdx,
			       int *CrnrIdx,
			       int *UVWMax);
UserTopoUnstrctGeomStruct *UserTopoReadGridFromFile(const char *FileName);
CagdBType UserTopoWriteGridToFile(const UserTopoUnstrctGeomStruct *Ud,
				  const char *FileName);
UserTopoUnstrctGeomReturnStruct *UserTopoUnstrctGeomMain(
				      UserTopoUnstructGridOpType OperationID,
				      UserTopoUnstrctGeomParamStruct *Params);
CagdBType UserTopoAddObjectToField(UserTopoUnstrctGeomStruct *Ud,
				   IPObjectStruct *IPObj);
IPObjectStruct *UserTopoGetField(UserTopoUnstrctGeomStruct *Ud);
#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_USER_TOPO_LIB_H */
