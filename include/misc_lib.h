/******************************************************************************
* Misc_lib.h - header file for the misc. library.			      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber, Oct. 94.					      *
******************************************************************************/

#ifndef IRIT_MISC_LIB_H
#define IRIT_MISC_LIB_H

#include <stdio.h>
#ifdef __WINCE__
#   include <memory.h>
#endif
#ifdef __WINNT__
#    include <malloc.h> 
#endif
#include "irit_sm.h"
#ifdef USE_VARARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif /* USE_VARARGS */

/* #define IRIT_COMPILE_PARALLEL */

/* Parallel execution controls: */
#ifdef IRIT_COMPILE_PARALLEL
    #include <omp.h>
    #ifdef __WINNT__
	#define IRIT_MUTEX		         VoidPtr
	#define IRIT_MUTEX_GEN_VAR(IritMutex) \
					IRIT_STATIC_DATA IRIT_MUTEX IritMutex;
	#define IRIT_MUTEX_LOCK(IritMutex) \
					IritEmulatePthreadMutexLock(&IritMutex);
	#define IRIT_MUTEX_UNLOCK(IritMutex) \
					IritEmulatePthreadMutexUnLock(IritMutex);
	#define IRIT_PARALLEL_LOOP_NO_PARAMS() omp parallel for
	#define IRIT_PARALLEL_LOOP_WITH_PARAMS(Private, Shared) \
					omp parallel for Private Shared
    #else
	#include <pthread.h>
	#define IRIT_MUTEX			 pthread_mutex_t
	#define IRIT_MUTEX_GEN_VAR(IritMutex) \
					IRIT_STATIC_DATA IRIT_MUTEX IritMutex;
	#define IRIT_MUTEX_LOCK(IritMutex)   pthread_mutex_lock(&IritMutex);
	#define IRIT_MUTEX_UNLOCK(IritMutex) pthread_mutex_unlock(&IritMutex);
	#define IRIT_PARALLEL_LOOP_NO_PARAMS() omp parallel for
	#define IRIT_PARALLEL_LOOP_WITH_PARAMS(Private, Shared) \
					omp parallel for Private Shared
    #endif /* __WINNT__ */
#else
    #define IRIT_MUTEX int
    #define IRIT_MUTEX_GEN_VAR(IritMutex)
    #define IRIT_MUTEX_LOCK(IritMutex)
    #define IRIT_MUTEX_UNLOCK(IritMutex)
    #define IRIT_PARALLEL_LOOP_NO_PARAMS()
    #define IRIT_PARALLEL_LOOP(Private, Shared)
#endif /* IRIT_COMPILE_PARALLEL */

typedef enum {
    MISC_ERR_MALLOC_FAILED,
    MISC_ERR_CONFIG_FILE_NO_FOUND,
    MISC_ERR_CONFIG_ERROR,
    MISC_ERR_UNKNOWN_CONFIG,

    MISC_ERR_UNDEFINE_ERR
} MiscFatalErrorType;

typedef enum {
    IC_NONE_TYPE = 0,
    IC_BOOLEAN_TYPE,
    IC_INTEGER_TYPE,
    IC_REAL_TYPE,
    IC_STRING_TYPE,
    IC_MULTI_STR_TYPE
} IrtCfgDataType;

typedef enum {
    IRIT_IMAGE_UNKNOWN_TYPE,
    IRIT_IMAGE_RLE_TYPE,
    IRIT_IMAGE_PPM3_TYPE,
    IRIT_IMAGE_PPM6_TYPE,
    IRIT_IMAGE_PNG_TYPE
} IrtImgImageType;

typedef enum {
    MISC_ISC_BNW,
    MISC_ISC_GRAY
} MiscISCColorTypeEnum;

typedef int (*MiscISCPrintFuncType)(const char *Format, ...);

/*****************************************************************************
* Simple expression trees parser - have variables 'A' to 'Z'.                *
*****************************************************************************/

enum {
    IRIT_E2T_ABS = 10,          /* Functions. */
    IRIT_E2T_ARCCOS,
    IRIT_E2T_ARCSIN,
    IRIT_E2T_ARCTAN,
    IRIT_E2T_COS,
    IRIT_E2T_EXP,
    IRIT_E2T_LN,
    IRIT_E2T_LOG,
    IRIT_E2T_SIN,
    IRIT_E2T_SQR,
    IRIT_E2T_SQRT,
    IRIT_E2T_TAN,

    IRIT_E2T_PLUS = 30,         /* Operators. */
    IRIT_E2T_MINUS,
    IRIT_E2T_MULT,
    IRIT_E2T_DIV,
    IRIT_E2T_POWER,
    IRIT_E2T_UNARMINUS,

    IRIT_E2T_OPENPARA = 40,     /* Paranthesis. */
    IRIT_E2T_CLOSPARA,

    IRIT_E2T_NUMBER = 50,       /* Numbers (or parameter t). */
    IRIT_E2T_PARAMETER,

    IRIT_E2T_TOKENERROR = -1,
    IRIT_E2T_TOKENSTART = 100,
    IRIT_E2T_TOKENEND,

    IRIT_E2T_MAX_PARAM_IDX = 'Z' - 'A'
};

enum {
    IRIT_E2T_PARAM_ALL = -1,       /* Match all IRIT_E2T_PARAMs is searches. */
    IRIT_E2T_PARAM_A = 0,
    IRIT_E2T_PARAM_B,
    IRIT_E2T_PARAM_C,
    IRIT_E2T_PARAM_D,
    IRIT_E2T_PARAM_E,
    IRIT_E2T_PARAM_F,
    IRIT_E2T_PARAM_G,
    IRIT_E2T_PARAM_H,
    IRIT_E2T_PARAM_I,
    IRIT_E2T_PARAM_J,
    IRIT_E2T_PARAM_K,
    IRIT_E2T_PARAM_L,
    IRIT_E2T_PARAM_M,
    IRIT_E2T_PARAM_N,
    IRIT_E2T_PARAM_O,
    IRIT_E2T_PARAM_P,
    IRIT_E2T_PARAM_Q,
    IRIT_E2T_PARAM_R,
    IRIT_E2T_PARAM_S,
    IRIT_E2T_PARAM_T,
    IRIT_E2T_PARAM_U,
    IRIT_E2T_PARAM_V,
    IRIT_E2T_PARAM_W,
    IRIT_E2T_PARAM_X,
    IRIT_E2T_PARAM_Y,
    IRIT_E2T_PARAM_Z,
    IRIT_E2T_PARAM_NUM_PARAM
};

/* Error numbers during expression trees parsing/derivative process. */

enum {
    IRIT_E2T_UNDEF_TOKEN_ERROR = 1,
    IRIT_E2T_PARAMATCH_ERROR,
    IRIT_E2T_EOL_ERROR,
    IRIT_E2T_REAL_MESS_ERROR,
    IRIT_E2T_SYNTAX_ERROR,
    IRIT_E2T_STACK_OV_ERROR,
    IRIT_E2T_ONE_OPERAND_ERROR,
    IRIT_E2T_TWO_OPERAND_ERROR,
    IRIT_E2T_PARAM_EXPECT_ERROR,
    IRIT_E2T_DERIV_NONE_CONST_EXP_ERROR,
    IRIT_E2T_DERIV_NO_ABS_DERIV_ERROR
};

typedef struct IritE2TExprNodeStruct {
     struct IritE2TExprNodeStruct *Right, *Left;
     int NodeKind;
     IrtRType RData;
     char *SData;
} IritE2TExprNodeStruct;

typedef struct MiscExprTreeGenInfoStruct *MiscExprTreeGenInfoStructPtr;
typedef struct MiscWriteGenInfoStruct *MiscWriteGenInfoStructPtr;

typedef IrtRType (*IritE2TExprNodeParamFuncType)(
					    MiscExprTreeGenInfoStructPtr GI,
					    const char *ParamName);

typedef struct IritConfigStruct {
    const char *VarName;
    const char *SomeInfo;
    VoidPtr VarData;
    IrtCfgDataType VarType;
} IritConfigStruct;

typedef struct IrtImgPixelStruct {
    IrtBType r, g, b;
} IrtImgPixelStruct;

typedef struct IrtImgRGBAPxlStruct {
    IrtBType r, g, b, a;
} IrtImgRGBAPxlStruct;

typedef float IrtImgRealClrType;
typedef struct IrtImgRealPxlStruct {
    IrtImgRealClrType r, g, b, a;
} IrtImgRealPxlStruct;

typedef struct IritPriorQue {
    struct IritPriorQue *Right, *Left; /* Pointers to two sons of this node. */
    VoidPtr Data;			     /* Pointers to the data itself. */
} IritPriorQue;

typedef struct IritHashElementStruct {
    struct IritHashElementStruct *Pnext;
    VoidPtr Data;
    IrtRType Key;
} IritHashElementStruct;

typedef struct IritHashTableStruct {
    IrtRType MinKeyVal, MaxKeyVal, DKey, KeyEps;
    IritHashElementStruct **Vec;
    int VecSize;
    VoidPtr AuxData;
} IritHashTableStruct;

typedef struct IritBiPrWeightedMatchStruct {
    int m1, m2, m3;
} IritBiPrWeightedMatchStruct;

typedef struct IritIntrvlArithStruct {
    IrtRType Max;
    IrtRType Min;
} IritIntrvlArithStruct;

typedef struct MiscISCCalculatorStruct* MiscISCCalculatorPtrType;
typedef struct MiscListStruct *MiscListPtrType;
typedef struct MiscListIteratorStruct *MiscListIteratorPtrType;

typedef unsigned char MiscISCPixelType;
typedef unsigned long MiscISCImageSizeType;

typedef void (*IritFatalMsgFuncType)(const char *Msg);
typedef void (*IritWarningMsgFuncType)(const char *Msg);
typedef void (*IritInfoMsgFuncType)(const char *Msg);
typedef void (*MiscSetErrorFuncType)(MiscFatalErrorType ErrorFunc,
				     const char *ErrorDescription);

typedef int (*IritPQCompFuncType)(VoidPtr p1, VoidPtr p2);/* Comparison func. */
typedef int (*IritHashCmpFuncType)(VoidPtr Data1, VoidPtr Data2, VoidPtr AuxData);
typedef void IritLevenEvalFuncType(IrtRType *CurPoint,
				   IrtRType ModelParams[],
				   IrtRType *YPointer,
				   IrtRType YdParams[]);
typedef void IritLevenNumerProtectionFuncType(IrtRType InternalModelParams[]);
typedef int IritLevenIsModelValidFuncType(IrtRType InternalModelParams[]);

/* Return hash value (KeySizeBits length) of the element Elem. */
typedef unsigned long (*MiscHashFuncType)(void * Elem, 
					  unsigned long ElementParam, 
					  unsigned int KeySizeBits);
/* Create copy of the Elem. Used only for adding element to the table. */
typedef void *(*MiscHashCopyFuncType)(void *Elem, unsigned long ElementParam);
/* Free Elem. */
typedef void (*MiscHashFreeFuncType)(void *Elem);
/* Return 0 if Elem1 equals Elem2. 1 otherwise. */
typedef int (*MiscHashCompFuncType)(void *Elem1, 
                                    void *Elem2, 
                                    unsigned long ElementParam);

typedef struct MiscHashTableStruct *MiscHashPtrType;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* Basic dynamic memory allocation routines: */
#ifdef DEBUG_IRIT_MALLOC
VoidPtr IritMalloc(unsigned int Size,
		   const char *ObjType,
		   const char *FileName,
		   int LineNum);
#define IritMalloc(x)	IritMalloc((x), #x, __FILE__, __LINE__)
void IritFree(VoidPtr p);
#define IritFree(x)	{ IritFree((void *) x); x = NULL; }
void IritDynMemoryDbgTestAll(void);
void IritDynMemoryDbgInitTest(void);
void IritDynMemoryDbgInitTest2(int DebugMalloc, int DebugSearchAllocID);
void IritDynMemoryDbgCheckMark(IrtRType *Start,
			       IrtRType *KeepStackStep,
			       IrtRType *TrackAllocID);
void IritDynMemoryDbgMallocSearchID(int ID);
void *IritDynMemoryDbgNoReport(void *p);
/* A debug macro to state this allocation is global and made once. */
#define IRIT_MALLOC_DBG_NO_REPORT(x)  IritDynMemoryDbgNoReport(x)
#else
#define IritMalloc(Size)	malloc(Size)
#define IritFree(Ptr)		free(Ptr)
#define IRIT_MALLOC_DBG_NO_REPORT(x)   (x)
#endif /* DEBUG_IRIT_MALLOC */
VoidPtr IritRealloc(VoidPtr p, unsigned int OldSize, unsigned int NewSize);
void IritFree2UnixFree(void *p);

#ifdef __WINNT__
#   ifdef DEBUG
#define IRIT_MAX_ALLOCA_SIZE	10000
#define IritAlloca(Size) _alloca((Size) > IRIT_MAX_ALLOCA_SIZE ? \
					     assert(0), (Size) : (Size))
#   else
#define IritAlloca(Size) _alloca(Size)
#   endif /* DEBUG */
#else
#define IritAlloca(Size) alloca(Size)
#endif /* __WINNT__ */

/* Prototype of the configuration routines: */
const char *IritConfig(const char *PrgmName,
		       const IritConfigStruct *SetUp,
		       int NumVar,
		       char *FoundName);
void IritConfigPrint(const IritConfigStruct *SetUp, int NumVar);
int IritConfigSave(const char *FileName,
		   const IritConfigStruct *SetUp,
		   int NumVar);

/* Get command line arguments. */
#ifdef USE_VARARGS
int GAGetArgs(int va_alist, ...);
#else
int GAGetArgs(int argc, ...);
#endif /* USE_VARARGS */
char *GAStringErrMsg(int Error, char *OutStr);
void GAPrintErrMsg(int Error);
char *GAStringHowTo(const char *CtrlStr, char *OutStr);
void GAPrintHowTo(const char *CtrlStr);

/* Homogeneous 4x4 matrix routines. */
void MatGenUnitMat(IrtHmgnMatType Mat);
int MatIsUnitMatrix(IrtHmgnMatType Mat, IrtRType Eps);
int MatIsWeightAffected(IrtHmgnMatType Mat, IrtRType Eps);
void MatGenMatTrans(IrtRType Tx, IrtRType Ty, IrtRType Tz, IrtHmgnMatType Mat);
void MatGenMatUnifScale(IrtRType Scale, IrtHmgnMatType Mat);
void MatGenMatScale(IrtRType Sx, IrtRType Sy, IrtRType Sz, IrtHmgnMatType Mat);
void MatGenMatRotX1(IrtRType Teta, IrtHmgnMatType Mat);
void MatGenMatRotX(IrtRType CosTeta, IrtRType SinTeta, IrtHmgnMatType Mat);
void MatGenMatRotY1(IrtRType Teta, IrtHmgnMatType Mat);
void MatGenMatRotY(IrtRType CosTeta, IrtRType SinTeta, IrtHmgnMatType Mat);
void MatGenMatRotZ1(IrtRType Teta, IrtHmgnMatType Mat);
void MatGenMatRotZ(IrtRType CosTeta, IrtRType SinTeta, IrtHmgnMatType Mat);

void MatMultTwo4by4(IrtHmgnMatType MatRes,
		    IrtHmgnMatType Mat1,
		    IrtHmgnMatType Mat2);
void MatAddTwo4by4(IrtHmgnMatType MatRes,
		   IrtHmgnMatType Mat1,
		   IrtHmgnMatType Mat2);
void MatSubTwo4by4(IrtHmgnMatType MatRes,
		   IrtHmgnMatType Mat1,
		   IrtHmgnMatType Mat2);
void MatScale4by4(IrtHmgnMatType MatRes,
		  IrtHmgnMatType Mat,
		  const IrtRType *Scale);
int MatSameTwo4by4(IrtHmgnMatType Mat1,
		   IrtHmgnMatType Mat2,
		   IrtRType Eps);

void MatMultVecby4by4(IrtVecType VecRes,
		      const IrtVecType Vec,
		      IrtHmgnMatType Mat);
void MatMultPtby4by4(IrtPtType PtRes,
		     const IrtPtType Pt,
		     IrtHmgnMatType Mat);
void MatMultWVecby4by4(IrtRType VRes[4],
		       const IrtRType Vec[4],
		       IrtHmgnMatType Mat);

IrtRType MatDeterminantMatrix(IrtHmgnMatType Mat);
int MatInverseMatrix(IrtHmgnMatType M, IrtHmgnMatType InvM);
void MatTranspMatrix(IrtHmgnMatType M, IrtHmgnMatType TranspM);
IrtRType MatScaleFactorMatrix(IrtHmgnMatType M);
IrtRType *MatScaleFactorMatrix2(IrtHmgnMatType M, IrtVecType ScaleVec);
void MatRotateFactorMatrix(IrtHmgnMatType M, IrtHmgnMatType RotMat);
void MatRotSclFactorMatrix(IrtHmgnMatType M, IrtHmgnMatType RotSclMat);
void MatTranslateFactorMatrix(IrtHmgnMatType M, IrtVecType Trans);

/* General matrix routines. */
void MatGnrlCopy(IrtGnrlMatType Dst, IrtGnrlMatType Src, int n);
void MatGnrlUnitMat(IrtGnrlMatType Mat, int n);
int MatGnrlIsUnitMatrix(IrtGnrlMatType Mat, IrtRType Eps, int n);
void MatGnrlMultTwoMat(IrtGnrlMatType MatRes,
		       IrtGnrlMatType Mat1,
		       IrtGnrlMatType Mat2,
		       int n);
void MatGnrlAddTwoMat(IrtGnrlMatType MatRes,
		      IrtGnrlMatType Mat1,
		      IrtGnrlMatType Mat2,
		      int n);
void MatGnrlSubTwoMat(IrtGnrlMatType MatRes,
		      IrtGnrlMatType Mat1,
		      IrtGnrlMatType Mat2,
		      int n);
void MatGnrlScaleMat(IrtGnrlMatType MatRes,
		     IrtGnrlMatType Mat,
		     IrtRType *Scale,
		     int n);
void MatGnrlMultVecbyMat(IrtVecGnrlType VecRes,
			 IrtGnrlMatType Mat,
			 IrtVecGnrlType Vec,
			 int n);
void MatGnrlMultVecbyMat2(IrtVecGnrlType VecRes,
			  IrtVecGnrlType Vec,
			  IrtGnrlMatType Mat,
			  int n);
int MatGnrlInverseMatrix(IrtGnrlMatType M,
			 IrtGnrlMatType InvM,
			 int n);
void MatGnrlTranspMatrix(IrtGnrlMatType M,
			 IrtGnrlMatType TranspM,
			 int n);
IrtRType MatGnrlDetMatrix(IrtGnrlMatType M, int n);
int MatGnrlOrthogonalSubspace(IrtGnrlMatType M, int n);
void MatGnrlPrintMatrix(IrtGnrlMatType M, int n, FILE *F);
IrtRType Mat2x2Determinant(IrtRType a11,
			   IrtRType a12, 
			   IrtRType a21,
			   IrtRType a22);
IrtRType Mat3x3Determinant(IrtRType a11,
			   IrtRType a12,
			   IrtRType a13,
			   IrtRType a21,
			   IrtRType a22,
			   IrtRType a23,
			   IrtRType a31,
			   IrtRType a32,
			   IrtRType a33);

/* QR matrix factorization. */
int IritQRFactorization(IrtRType *A,
			int n,
			int m,
			IrtRType *Q,
			IrtRType *R);
int IritSolveUpperDiagMatrix(const IrtRType *A,
			     int n,
			     const IrtRType *b,
			     IrtRType *x);
int IritSolveLowerDiagMatrix(const IrtRType *A,
			     int n,
			     const IrtRType *b,
			     IrtRType *x);
int IritQRUnderdetermined(IrtRType *A,
			  IrtRType *x,
			  const IrtRType *b,
			  int m,
			  int n,
			  void **QRCache);
int IritQRUnderdetermined2(IrtRType *A,
			   IrtRType *x,
			   const IrtRType *b,
			   int m,
			   int n);

/* Gauss Jordan matrix solver and Levenberg Marquardt local minimum finder. */
int IritGaussJordan(IrtRType *A, IrtRType *B, unsigned int N, unsigned int M);
IrtRType IritLevenMarMin(IrtRType **x,
			 IrtRType y[],
			 IrtRType Sigma[],
			 unsigned int NumberOfDataElements,
			 IrtRType ModelParams[],
			 IritLevenEvalFuncType *ShapeFunc,
			 IritLevenNumerProtectionFuncType *ProtectionFunc,
			 IritLevenIsModelValidFuncType *ModelValidatorFunc,
			 unsigned int NumberOfModelParams,
			 IrtRType Tolerance);
IrtRType IritLevenMarMinSig1(IrtRType **XVals,
			     IrtRType YVals[],
			     unsigned int NumberOfDataElements,
			     IrtRType ModelParams[],
			     IritLevenEvalFuncType *ShapeFunc,
			     IritLevenNumerProtectionFuncType *ProtectionFunc,
			     IritLevenIsModelValidFuncType *ModelValidatorFunc,
			     unsigned int NumberOfMedelParams,
			     IrtRType Tolerance);
unsigned int IritLevenMarSetMaxIterations(unsigned int NewVal);

/* An implementation of a priority queue. */
void IritPQInit(IritPriorQue **PQ);
int IritPQEmpty(IritPriorQue *PQ);
void IritPQCompFunc(IritPQCompFuncType NewCompFunc);
VoidPtr IritPQFirst(IritPriorQue **PQ, int Delete);
VoidPtr IritPQInsert(IritPriorQue **PQ, VoidPtr NewItem);
VoidPtr IritPQDelete(IritPriorQue **PQ, VoidPtr NewItem);
VoidPtr IritPQFind(IritPriorQue *PQ, VoidPtr OldItem);
VoidPtr IritPQNext(IritPriorQue *PQ, VoidPtr CmpItem, VoidPtr BiggerThan);
int IritPQSize(IritPriorQue *PQ);
void IritPQPrint(IritPriorQue *PQ, void (*PrintFunc)(VoidPtr));
void IritPQFree(IritPriorQue *PQ, int FreeItems);
void IritPQFreeFunc(IritPriorQue *PQ, void (*FreeFunc)(VoidPtr));

/* An implementation of a hashing table. */
IritHashTableStruct *IritHashTableCreate(IrtRType MinKeyVal,
					 IrtRType MaxKeyVal,
					 IrtRType KeyEps,
					 int VecSize);
int IritHashTableInsert(IritHashTableStruct *IHT,
			VoidPtr Data,
			IritHashCmpFuncType HashCmpFunc,
			IrtRType Key,
			int RplcSame);
VoidPtr IritHashTableFind(IritHashTableStruct *IHT,
			  VoidPtr Data,
			  IritHashCmpFuncType HashCmpFunc,
			  IrtRType Key);
int IritHashTableRemove(IritHashTableStruct *IHT,
			VoidPtr Data,
			IritHashCmpFuncType HashCmpFunc,
			IrtRType Key);
void IritHashTableFree(IritHashTableStruct *IHT);

/* Another implementation of a hashing table. */

/* Call back functions of the hash data structure. */
/* SizeInByte is a parameter given by the  user and propagated to each       */
/* function. The function may use it for its action (its name origin from    */
/* one use as a length of an array element).                                 */

MiscHashPtrType MiscHashNewHash(unsigned long HashSize,
				MiscHashFuncType HashFunc,
				MiscHashCopyFuncType CopyFunc,
				MiscHashFreeFuncType FreeFunc,
				MiscHashCompFuncType CompFunc);
int MiscHashAddElement(MiscHashPtrType Hash, 
                       void *Elem, 
                       unsigned long SizeInByte);
int MiscHashFindElement(MiscHashPtrType Hash, 
                        void *Elem, 
                        unsigned long SizeInByte);
long *MiscHashGetElementAuxData(MiscHashPtrType Hash,
				void *Elem,
				unsigned long SizeInByte);
void MiscHashFreeHash(MiscHashPtrType Hash);

/* An implementation of a list data structure. */

/* Call back functions of the list data structure to copy, free and compare */
/* elements (zero if equal).  Last param can be used to pass array size.    */
typedef void *(*MiscListCopyFuncType)(void *elmt, unsigned long size);
typedef void (*MiscListFreeFuncType)(void *elmt);
typedef int (*MiscListCompFuncType)(void *e1mt1, void *elmt2, unsigned long l);

MiscListPtrType MiscListNewEmptyList(MiscListCopyFuncType CopyFunc,
				     MiscListFreeFuncType FreeFunc,
				     MiscListCompFuncType CompFunc);

int MiscListAddElement(MiscListPtrType List, 
                       void *Elem, 
                       unsigned long SizeInByte);
int MiscListFindElementInList(MiscListPtrType List, 
                              void *Elem, 
                              unsigned long SizeInByte);
int MiscListCompLists(MiscListPtrType L1, MiscListPtrType L2);
void MiscListFreeList(MiscListPtrType List);
MiscListIteratorPtrType MiscListGetListIterator(MiscListPtrType List);
void MiscListFreeListIterator(MiscListIteratorPtrType It);
void *MiscListIteratorFirst(MiscListIteratorPtrType It);
void *MiscListIteratorNext(MiscListIteratorPtrType It);
int MiscListIteratorAtEnd(MiscListIteratorPtrType It);
void *MiscListIteratorValue(MiscListIteratorPtrType It);

/* Interval arithmetic. */
void IritIntrvlArithAdd(const IritIntrvlArithStruct *A,
			const IritIntrvlArithStruct *B,
			IritIntrvlArithStruct *Result);
void IritIntrvlArithSub(const IritIntrvlArithStruct *A,
			const IritIntrvlArithStruct *B,
			IritIntrvlArithStruct *Result);
void IritIntrvlArithMult(const IritIntrvlArithStruct *A,
			 const IritIntrvlArithStruct *B,
			 IritIntrvlArithStruct *Result);
void IritIntrvlArithDiv(const IritIntrvlArithStruct *A,
			const IritIntrvlArithStruct *B,
			IritIntrvlArithStruct *Result);
void IritIntrvlArithSqrt(const IritIntrvlArithStruct *A,
			 IritIntrvlArithStruct *Result);
void IritIntrvlArithMultScalar(const IritIntrvlArithStruct *A,
			       IrtRType Val, 
			       IritIntrvlArithStruct *Result);
void IritIntrvlArithAbs(const IritIntrvlArithStruct *A,
			IritIntrvlArithStruct *Result);
void IritIntrvlArithUnion(const IritIntrvlArithStruct *A,
			  const IritIntrvlArithStruct *B,
			  IritIntrvlArithStruct *Result);
void IritIntrvlArithVAdd(const IritIntrvlArithStruct *A,
			 const IritIntrvlArithStruct *B,
			 IritIntrvlArithStruct *Result);
void IritIntrvlArithVDot(const IritIntrvlArithStruct *A,
			 const IritIntrvlArithStruct *B,
			 IritIntrvlArithStruct *Result);
void IritIntrvlArithVCross(const IritIntrvlArithStruct *A,
			   const IritIntrvlArithStruct *B,
			   IritIntrvlArithStruct *Result);
void IritIntrvlArithVFromCone(const IrtRType *Dir,
			      IrtRType Angle,
			      IritIntrvlArithStruct *Result);
void IritIntrvlArithVToCone(const IritIntrvlArithStruct *IntervalV,
			    IrtRType *Dir,
			    IrtRType *Angle);
void IritIntrvlArithVToSphere(const IritIntrvlArithStruct *IntervalV,
			      IrtRType *Center,
			      IrtRType *Radius);
void IritIntrvlArithVMultScalar(const IritIntrvlArithStruct *A,
				IrtRType Val,
				IritIntrvlArithStruct *Result);

/* Read/Write of images. */
int IrtImgReadImageXAlign(int Alignment);
IrtImgPixelStruct *IrtImgReadImage(const char *ImageFileName,
				   int *MaxX,
				   int *MaxY,
				   int *Alpha);
IrtImgPixelStruct *IrtImgReadImage2(const char *ImageFileName,
				    int *MaxX,
				    int *MaxY,
				    int *Alpha);
IrtImgPixelStruct *IrtImgReadImage3(const char *ImageFileName,
				    int *MaxX,
				    int *MaxY,
				    int *Alpha);
int IrtImgGetImageSize(const char *ImageFileName,
		       int *Width,
		       int *Height);
int IrtImgReadUpdateCache(const char *ImageFileName,
			  int MaxX,
			  int MaxY,
			  int Alpha,
			  IrtBType *Image);
void IrtImgReadClrCache(void);
void IrtImgReadClrOneImage(const char *ImageName);
IrtImgImageType IrtImgWriteGetType(const char *ImageType);
MiscWriteGenInfoStructPtr IrtImgWriteOpenFile(const char **argv,
					      const char *FName,
					      IrtImgImageType ImageType,
					      int Alpha,
					      int XSize,
					      int YSize);
void IrtImgWritePutLine(MiscWriteGenInfoStructPtr GI,
			IrtBType *Alpha,
			IrtImgPixelStruct *Pixels);
void IrtImgWriteCloseFile(MiscWriteGenInfoStructPtr GI);
IrtImgPixelStruct *IrtImgFlipXYImage(const IrtImgPixelStruct *Img,
				     int MaxX,
				     int MaxY,
				     int Alpha);
IrtImgPixelStruct *IrtImgNegateImage(const IrtImgPixelStruct *InImage,
				     int MaxX,
				     int MaxY);
IrtImgPixelStruct *IrtImgFlipHorizontallyImage(const IrtImgPixelStruct *Img,
					       int MaxX,
					       int MaxY,
					       int Alpha);
IrtImgPixelStruct *IrtImgFlipVerticallyImage(const IrtImgPixelStruct *Img,
					     int MaxX,
					     int MaxY,
					     int Alpha);
int IrtImgParsePTextureString(const char *PTexture,
			      char *FName,
			      IrtRType *Scale,
			      int *Flip,
			      int *NewImage);
int IrtImgParsePTextureString2(const char *PTexture,
			      char *FName,
			      IrtRType *Scale,
			      int *Flip,
			      int *NewImage,
			      int *FlipHorizontally,
			      int *FlipVertically);
IrtBType *IrtImgDitherImage(IrtImgPixelStruct *Image,
			    int XSize,
			    int YSize,
			    int DitherSize,
			    IrtBType ErrorDiffusion);
int IrtImgDitherImage2(const char *InputImage,
		       const char *OututImage,
		       int DitherSize,
		       IrtBType ErrorDiffusion);

/* Read/Write of movies. */
IrtImgPixelStruct **IrtMovieReadMovie(const char *MovieFileName,
				      int *MaxX,
				      int *MaxY,
				      int *Alpha);
IrtImgPixelStruct **IrtMovieReadMovie2(const char *MovieFileName,
				       int *MaxX,
				       int *MaxY,
				       int *Alpha);
IrtImgPixelStruct **IrtMovieReadMovie3(const char *MovieFileName,
				       int *MaxX,
				       int *MaxY,
				       int *Alpha);
int IrtMovieGetMovieProps(const char *MovieFileName,
			  int *Width,
			  int *Height);
void IrtMovieReadClrCache(void);
int IrtMovieParsePMovieString(const char *PMovie,
			      char *FName,
			      IrtRType *Scale,
			      int *NewImage,
			      int *Flip,
			      int *Restart,
			      IrtRType *TimeSetup,
			      int *FlipHorizontally,
			      int *FlipVertically);

/* Searching routines. */
VoidPtr IritSearch2DInit(IrtRType XMin,
			 IrtRType XMax,
			 IrtRType YMin,
			 IrtRType YMax,
			 IrtRType Tol,
			 int DataSize);
void IritSearch2DFree(VoidPtr S2D);
void IritSearch2DInsertElem(VoidPtr S2D,
			    IrtRType XKey,
			    IrtRType YKey,
			    VoidPtr Data);
int IritSearch2DFindElem(VoidPtr S2D,
			 IrtRType XKey,
			 IrtRType YKey,
			 VoidPtr Data);

/* Set cover related routines. */
VoidPtr IritRLNew(void);
void IritRLAdd(VoidPtr RLC, IrtRType l, IrtRType r, int attr);
int *IritRLFindCyclicCover(VoidPtr RLC, IrtRType Tol);
void IritRLDelete(VoidPtr RLC);
int IritRLSetGaurdiansNumber(int g);

/* Image Set Cover related routines. */
char *MiscISCGetTimeStamp(long Time, 
                          char *TimeString, 
                          int TimeZone, 
                          int UseDays);
MiscISCCalculatorPtrType MiscISCNewCalculator(int MaxPictures,
					      MiscISCImageSizeType PixelNumber,
					      MiscISCColorTypeEnum Colors,
                                              MiscISCPrintFuncType Print);
void MiscISCFreeCalculator(MiscISCCalculatorPtrType Calc);
int MiscISCAddPicture(MiscISCCalculatorPtrType Calc,
		      MiscISCPixelType *Picture);
int MiscISCSetImageToCover(MiscISCCalculatorPtrType Calc,
			   MiscISCPixelType *RequiredCover);
int MiscISCCalculateExact(MiscISCCalculatorPtrType Calc,
                          int SizeLimit,
                          int **SolutionByIndex,
                          int *SolutionSize,
                          IrtRType *CoverPart);
int MiscISCCalculateExhaustive(MiscISCCalculatorPtrType Calc,
                               IrtRType CoverLimit,
                               int SizeLimit,
                               int **SolutionByIndex,
                               int *SolutionSize,
                               IrtRType *CoverPart);
int MiscISCCalculateGreedy(MiscISCCalculatorPtrType Calc,
                           int **SolutionByIndex,
                           int *SolutionSize,
                           IrtRType *CoverPart);

/* Bipartite graphs weighted matching. */
int MiscBiPrWeightedMatchBipartite(const IrtRType **Weight,
				   IritBiPrWeightedMatchStruct *Match,
				   int n);

/* Simple infix expression trees parser. */
MiscExprTreeGenInfoStructPtr IritE2TExpr2TreeInit();
void IritE2TExpr2TreeFree(MiscExprTreeGenInfoStructPtr GI);
IritE2TExprNodeStruct *IritE2TExpr2Tree(MiscExprTreeGenInfoStructPtr GI,
					const char s[]);
IritE2TExprNodeParamFuncType IritE2Expr2TreeSetFetchParamValueFunc(
			     MiscExprTreeGenInfoStructPtr GI,
			     IritE2TExprNodeParamFuncType FetchParamValueFunc);
IrtRType IritE2Expr2TreeDefaultFetchParamValue(MiscExprTreeGenInfoStructPtr GI,
					       const char *SData);
int IritE2TCmpTree(const IritE2TExprNodeStruct *Root1,
		   const IritE2TExprNodeStruct *Root2);
IritE2TExprNodeStruct *IritE2TCopyTree(const IritE2TExprNodeStruct *Root);
IritE2TExprNodeStruct *IritE2TDerivTree(MiscExprTreeGenInfoStructPtr GI,
					const IritE2TExprNodeStruct *Root,
					int Param);
void IritE2TPrintTree(const IritE2TExprNodeStruct *Root, char *Str);
IrtRType IritE2TEvalTree(MiscExprTreeGenInfoStructPtr GI,
			 const IritE2TExprNodeStruct *Root);
void IritE2TFreeTree(IritE2TExprNodeStruct *Root);
int IritE2TParamInTree(const IritE2TExprNodeStruct *Root,
		       const char *ParamName);
void IritE2TSetParamValue(MiscExprTreeGenInfoStructPtr GI,
			  IrtRType Value,
			  int Index);
int IritE2TParseError(MiscExprTreeGenInfoStructPtr GI);
int IritE2TDerivError(MiscExprTreeGenInfoStructPtr GI);

/* XGeneral routine - compatibility between Unix and Win95/WinNT/OS2/etc. */
char *IritStrdup(const char *s);
char *IritStrUpper(char *s);
char *IritStrLower(char *s);
void IritSleep(int MiliSeconds);
void IritRandomInit(long Seed);
IrtRType IritRandom(IrtRType Min, IrtRType Max);
void IritPseudoRandomInit(unsigned int Seed);
IrtRType IritPseudoRandom();
IrtRType IritCPUTime(int Reset);
char *IritRealTimeDate(char *StrTime);
IrtRType IritApproxStrStrMatch(const char *Str1,
			       const char *Str2,
			       int IgnoreCase);

#ifndef AMIGA
void movmem(VoidPtr Src, VoidPtr Dest, int Len);
#endif /* AMIGA */
const char *searchpath(const char *Name, char *FullPath);

#ifdef STRICMP
int strnicmp(const char *s1, const char *s2, int n);
int stricmp(const char *s1, const char *s2);
#else
#if !(defined(__WINNT__) || defined(__WINCE__) || defined(OS2GCC) || defined(AMIGA) || defined(__CYGWIN__))
#   define strnicmp(s1, s2, n) strncasecmp((s1), (s2), (n))
#   define stricmp(s1, s2)     strcasecmp((s1), (s2))
#endif /* !(__WINNT__ || __WINCE__|| OS2GCC || AMIGA) */

#ifdef __WINNT__
#   if _MSC_VER >= 1400  /* Visual 8, 2005 */
#       define strnicmp(s1, s2, n) _strnicmp((s1), (s2), (n))
#       define stricmp(s1, s2)     _stricmp((s1), (s2))
#   endif /* _MSC_VER >= 1400 */
#endif /*  __WINNT__ */

#ifdef __WINCE__
#   ifdef strnicmp
#       undef strnicmp
#       undef stricmp
#   endif /* strnicmp */
#   define strnicmp(s1, s2, n) _strnicmp((s1), (s2), (n))
#   define stricmp(s1, s2)     _stricmp((s1), (s2))
#endif /* __WINCE__ */
#endif /* STRICMP */

#ifdef __WINNT__
#   if _MSC_VER >= 1400  /* Visual 8, 2005 */
#   ifdef getcwd
#       undef getcwd
#   endif /* getcwd */
#   define getcwd(buf, maxlen)	_getcwd(buf, maxlen)
#   define chdir(dir)		_chdir(dir)
#   define putenv(str)		_putenv(str)
#   endif /* _MSC_VER >= 1400 */
#   ifdef IRIT_COMPILE_PARALLEL
        void IritEmulatePthreadMutexLock(IRIT_MUTEX *Mx);
        void IritEmulatePthreadMutexUnLock(IRIT_MUTEX Mx);
#   endif /* IRIT_COMPILE_PARALLEL */
#endif /*  __WINNT__ */

const char *IritStrIStr(const char *s, const char *Pattern);
char *IritSubstStr(const char *S,
		   const char *Src,
		   const char *Dst,
		   int CaseInsensitive);


#ifdef STRSTR
char *strstr(const char *s, const char *Pattern);
#endif /* STRSTR */

#ifdef GETCWD
char *getcwd(char *s, int Len);
#endif /* GETCWD */

/* Error handling. */

MiscSetErrorFuncType MiscSetFatalErrorFunc(MiscSetErrorFuncType ErrorFunc);
void MiscFatalError(MiscFatalErrorType ErrID, char *ErrDesc);
const char *MiscDescribeError(MiscFatalErrorType ErrorNum);

int IritLineHasCntrlChar(const char *Line);
IritFatalMsgFuncType IritSetFatalErrorFunc(IritFatalMsgFuncType FatalMsgFunc);
void IritFatalError(const char *Msg);
IritWarningMsgFuncType IritSetWarningMsgFunc(IritWarningMsgFuncType WrnMsgFunc);
void IritWarningMsg(const char *Msg);
IritInfoMsgFuncType IritSetInfoMsgFunc(IritInfoMsgFuncType InfoMsgFunc);
void IritInformationMsg(const char *Msg);

#ifdef USE_VARARGS
void IritFatalErrorPrintf(const char *va_alist, ...);
void IritWarningMsgPrintf(const char *va_alist, ...);
void IritInformationMsgPrintf(const char *va_alist, ...);
#else
void IritFatalErrorPrintf(const char *Format, ...);
void IritWarningMsgPrintf(const char *Format, ...);
void IritInformationMsgPrintf(const char *Format, ...);
#endif /* USE_VARARGS */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MISC_LIB_H */
