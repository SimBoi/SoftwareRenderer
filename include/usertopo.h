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

#define USER_UNSTRCT_GRID_REF_MAT_ATTR "KnotRefineMatrix"

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
    USER_UG_SEQ_POINT_IDS,
    USER_UG_SEQ_CELL_IDS,
    USER_UG_ADD_OBJECT_TO_FIELD,
    USER_UG_GET_FIELD,
    USER_UG_ADD_NEW_CELL,
    USER_UG_GET_BEZIER_PATCHES,
    USER_UG_REFINE_CELLS,
    USER_UG_PT_CELL_SELECTOR,
    USER_UG_POLY_CELL_SELECTOR,
    USER_UG_WRITE_GRID_TO_FILE,
    USER_UG_READ_GRID_FROM_FILE
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


/* Unstructured grid package. */
#define USER_TOPO_PTID_STR	"PointIDs"
#define USER_TOPO_ENTID_STR	"CellID"
#define USER_TOPO_UDID_STR	"UdID"
#define USER_TOPO_SRFDIST_SUBD_TOL  1e-3

typedef enum {
    USER_TOPO_ADJ_REL_NONE = 0,
    USER_TOPO_ADJ_REL_ONE_PT,
    USER_TOPO_ADJ_REL_OTHER_PT,
    USER_TOPO_ADJ_REL_ALL_PT
} UserTopoAdjRelType;

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
    CagdPtStruct Pt;    
    int ID;
} UserTopoUnstrctGeomPtStruct;

typedef struct UserTopoUnstrctGeomStruct {
    struct UserTopoUnstrctGeomStruct *Pnext;
    IPAttributeStruct *Attr;
    UserTopoUnstrctGeomPtStruct *PtsVec;	   /* All the points. */
    int NumPts;
    int _NextEntId;
    IPObjectStruct *CellList;		     /* List of all entities. */
    UserTopoAdjStruct *_AdjList;	  /* List of all adjacencies. */
    IPObjectStruct *_Field;	    /* List of unstructured elements. */
} UserTopoUnstrctGeomStruct;

typedef int *(*UserTopoFilterGridCBFuncType) (
					const UserTopoUnstrctGeomStruct *UG);

typedef struct UserTopoUnstrctGeomParamStruct {
    int UdId;
    int UdId2;
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
    CagdRType Eps;
    CagdPType Pt;
    IPPolygonStruct *Poly;
    char FileName[IRIT_LINE_LEN_LONG];
} UserTopoUnstrctGeomParamStruct;

typedef struct UserTopoUnstrctGeomReturnStruct {
    int UdId;
    int UdId2;
    int UdId3;
    int *CloneMap;
    int *RealIDMap;
    int *CellIDVec;
    void **AttrValVec;
    CagdBType Success;
    IPObjectStruct *Cell;
    int Id;
    int Length;
    int NumPts;
    struct {
	int NumCells;
	int NumCells123D[3];
	int NumPtAttr;
	int NumCellAttr;
    } UGDataStruct;
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
			     int UpdateGeom);
UserTopoUnstrctGeomStruct *UserTopoMergePoints(
					  const UserTopoUnstrctGeomStruct *Ud,
					  CagdRType Eps,
					  CagdBType IdentifyNoMerge,
					  CagdBType *MergePtIndices,
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
			    char *AttrName,
			    int AttrValue);
int UserTopoSetPointIntAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       int *PtIdVec,
			       int NumPtId,
			       char *AttrName,
			       int *AttrValueVec,
			       int NumVals);
int UserTopoSetPointRealAttr(UserTopoUnstrctGeomStruct *Ud,
			     int PtId,
			     char *AttrName,
			     CagdRType AttrValue);
int UserTopoSetPointRealAttrVec(UserTopoUnstrctGeomStruct *Ud,
			        int *PtIdVec,
			        int NumPtId,
			        char *AttrName,
			        CagdRType *AttrValueVec,
			        int NumVals);
int UserTopoSetPointStrAttr(UserTopoUnstrctGeomStruct *Ud,
			    int PtId,
			    char *AttrName,
			    char* AttrValue);
int UserTopoSetPointStrAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       int *PtIdVec,
			       int NumPtId,
			       char *AttrName,
			       char **AttrValueVec,
			       int NumVals);
int UserTopoSetCellIntAttr(UserTopoUnstrctGeomStruct *Ud,
			     IPObjectStruct *Cell,
			     char *AttrName,
			     int AttrValue);
int UserTopoSetCellIntAttrVec(UserTopoUnstrctGeomStruct *Ud,
			      int *CellIdVec,
			      int NumCellId,
			      char *AttrName,
			      int *AttrValueVec,
			      int AttrValueVecLen);
int UserTopoSetCellRealAttr(UserTopoUnstrctGeomStruct *Ud,
			    IPObjectStruct *Cell,
			    char *AttrName,
			    CagdRType AttrValue);
int UserTopoSetCellRealAttrVec(UserTopoUnstrctGeomStruct *Ud,
			       int *CellIdVec,
			       int NumCellId,
			       char *AttrName,
			       CagdRType *AttrValueVec,
			       int NumVals);
int UserTopoSetCellStrAttr(UserTopoUnstrctGeomStruct *Ud,
			   IPObjectStruct *Cell,
			   char *AttrName,
			   char *AttrValue);
int UserTopoSetCellStrAttrVec(UserTopoUnstrctGeomStruct *Ud,
			      int *CellIdVec,
			      int NumCellId,
			      char *AttrName,
			      char **AttrValueVec,
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
			       const CagdPType Pt);
UserTopoUnstrctGeomStruct *UserTopoReadGridFromTile(const char *FileName);
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
