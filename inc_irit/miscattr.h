/*****************************************************************************
* Setting attributes for objects.					     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
* Written by:  Gershon Elber				Ver 0.2, Mar. 1990   *
*****************************************************************************/

#ifndef IRIT_MISC_ATTR_H
#define IRIT_MISC_ATTR_H

#include "inc_irit/irit_sm.h"
#include "inc_irit/misc_attr_id.h"

typedef enum {
    IP_ATTR_NONE,
    IP_ATTR_INT,
    IP_ATTR_REAL,
    IP_ATTR_REALPTR,
    IP_ATTR_UV,
    IP_ATTR_STR,
    IP_ATTR_OBJ,
    IP_ATTR_PTR,
    IP_ATTR_REFPTR
} IPAttributeType;

#define IP_ATTR_BAD_INT		-2147182588
#define IP_ATTR_BAD_REAL	1e30
#define IP_ATTR_NO_COLOR	999
#define IP_ATTR_NO_WIDTH	1e30

#define IP_ATTR_MAX_STR_LENGTH	100000

#define IP_ATTR_IS_BAD_INT(I)	((I) == IP_ATTR_BAD_INT)
#define IP_ATTR_IS_BAD_REAL(R)	((R) > IP_ATTR_BAD_REAL / 10.0)
#define IP_ATTR_IS_BAD_COLOR(C)	((C) == IP_ATTR_NO_COLOR)
#define IP_ATTR_IS_BAD_WIDTH(W)	((W) > IP_ATTR_NO_WIDTH / 10.0)

#define IP_ATTR_RESET_ATTRS(Attr)	{ (Attr) = NULL; }
#define IP_ATTR_FREE_ATTRS(Attr)	{ if ((Attr) != NULL) \
					      AttrFreeAttributes(&(Attr)); }
#define IP_ATTR_COPY_ATTRS(Attr) \
	(Attr) != NULL ? AttrCopyAttributes(Attr) : NULL;
#define IP_ATTR_COPY_ATTRS2(NewAttr, OldAttr) { \
	NewAttr = IP_ATTR_COPY_ATTRS(OldAttr); }
#define IP_ATTR_SAFECOPY_ATTRS(NewAttr, OldAttr) { \
	IP_ATTR_FREE_ATTRS(NewAttr); \
	NewAttr = IP_ATTR_COPY_ATTRS(OldAttr); }

// GERSHON - will require Locking
#define IP_ATTR_INIT_UNIQUE_ID_NUM(AttrID, StrID) \
    if (AttrID == ATTRIB_NAME_BAD_NUMBER) { \
        AttrID = AttrGetAttribHashNumber(StrID); \
    }

#define IP_ATTR_IRIT_COLOR_TABLE_SIZE	16

typedef unsigned int IPAttrNumType;

#define ATTRIB_NAME_BAD_NUMBER    ((IPAttrNumType) -1)

/*****************************************************************************
* Attributes - an attribute has a name and can be one of the following:	     *
* an integer, real, string, or a pointer to an Object.			     *
*****************************************************************************/
typedef struct IPAttributeStruct {
    struct IPAttributeStruct *Pnext;
    IPAttributeType Type;
    union {
	char *Str;
   	int I;
	IrtRType R;
	struct {
	    IrtRType *Coord;
	    int Len;
	} Vec;
	float UV[2];
	struct IPObjectStruct *PObj;
	VoidPtr Ptr;
	VoidPtr RefPtr;
    } U;
    IPAttrNumType _AttribNum;           /* Used in string based attributes. */
    IPAttrIDType _AttribID;                  /* Used in Attribute ID attrs. */
} IPAttributeStruct;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

IRIT_GLOBAL_DATA_HEADER const IPAttrIDDefStruct _IPAttrIDDefTable[];
IRIT_GLOBAL_DATA_HEADER const int AttrIritColorTable[][3];
IRIT_GLOBAL_DATA_HEADER const int AttrIritColorTableSize;

/* Functions in miscattr.c */

void AttrGetIndexColor(int Color, int *Red, int *Green, int *Blue);
int AttrGetRGBColor2(const IPAttributeStruct *Attrs, 
		     const char *Name,
		     int *Red, 
		     int *Green, 
		     int *Blue);
void AttrSetIntAttrib(IPAttributeStruct **Attrs, const char *Name, int Data);
void AttrSetIntAttrib2(IPAttributeStruct **Attrs,
		       IPAttrNumType AttribNum,
		       int Data);
int AttrGetIntAttrib(const IPAttributeStruct *Attrs, const char *Name);
int AttrGetIntAttrib2(const IPAttributeStruct *Attrs, IPAttrNumType AttrNum);

void AttrSetRealAttrib(IPAttributeStruct **Attrs,
		       const char *Name,
		       IrtRType Data);
void AttrSetRealAttrib2(IPAttributeStruct **Attrs,
			IPAttrNumType AttribNum,
			IrtRType Data);
IrtRType AttrGetRealAttrib(const IPAttributeStruct *Attrs, const char *Name);
IrtRType AttrGetRealAttrib2(const IPAttributeStruct *Attrs,
			    IPAttrNumType AttrNum);

void AttrSetRealPtrAttrib(IPAttributeStruct **Attrs,
			  const char *Name,
			  IrtRType *Data,
			  int DataLen);
void AttrSetRealPtrAttrib2(IPAttributeStruct **Attrs,
			   IPAttrNumType AttribNum,
			   IrtRType *Data,
			   int DataLen);
IrtRType *AttrGetRealPtrAttrib(const IPAttributeStruct *Attrs,
			       const char *Name);
IrtRType *AttrGetRealPtrAttrib2(const IPAttributeStruct *Attrs,
				IPAttrNumType AttrNum);

void AttrSetUVAttrib(IPAttributeStruct **Attrs,
		     const char *Name,
		     IrtRType U,
		     IrtRType V);
void AttrSetUVAttrib2(IPAttributeStruct **Attrs,
		      IPAttrNumType AttribNum,
		      IrtRType U,
		      IrtRType V);
float *AttrGetUVAttrib(const IPAttributeStruct *Attrs, const char *Name);
float *AttrGetUVAttrib2(const IPAttributeStruct *Attrs, IPAttrNumType AttrNum);

void AttrSetPtrAttrib(IPAttributeStruct **Attrs,
		      const char *Name,
		      VoidPtr Data);
void AttrSetPtrAttrib2(IPAttributeStruct **Attrs,
		       IPAttrNumType AttribNum,
		       VoidPtr Data);
VoidPtr AttrGetPtrAttrib(const IPAttributeStruct *Attrs, const char *Name);
VoidPtr AttrGetPtrAttrib2(const IPAttributeStruct *Attrs,
			  IPAttrNumType AttrNum);

void AttrSetRefPtrAttrib(IPAttributeStruct **Attrs,
			 const char *Name,
			 VoidPtr Data);
void AttrSetRefPtrAttrib2(IPAttributeStruct **Attrs,
			  IPAttrNumType AttribNum,
			  VoidPtr Data);
VoidPtr AttrGetRefPtrAttrib(const IPAttributeStruct *Attrs, const char *Name);
VoidPtr AttrGetRefPtrAttrib2(const IPAttributeStruct *Attrs,
			     IPAttrNumType AttrNum);

void AttrSetStrAttrib(IPAttributeStruct **Attrs,
		      const char *Name,
		      const char *Data);
void AttrSetStrAttrib2(IPAttributeStruct **Attrs,
		       IPAttrNumType AttribNum,
		       const char *Data);
const char *AttrGetStrAttrib(const IPAttributeStruct *Attrs, const char *Name);
const char *AttrGetStrAttrib2(const IPAttributeStruct *Attrs,
			      IPAttrNumType AttrNum);

const IPAttributeStruct *AttrTraceAttributes(
					  const IPAttributeStruct *TraceAttrs,
					  const IPAttributeStruct *FirstAttrs);
const char *Attr2StringToData(const IPAttributeStruct *Attr,
			      int DataFileFormat,
			      char *Str);
const char *Attr2StringMalloc(const IPAttributeStruct *Attr,
			      int DataFileFormat);
int AttrCmpTwoAttrByName(const IPAttributeStruct *AttrList1,
			 const IPAttributeStruct *AttrList2,
			 const char *AttrName);

IPAttributeStruct *AttrReverseAttributes(IPAttributeStruct *Attr);

void AttrFreeOneAttribute(IPAttributeStruct **Attrs, const char *Name);
void AttrFreeAttributes(IPAttributeStruct **Attrs);

IPAttributeStruct *AttrFindAttribute(const IPAttributeStruct *Attrs,
				     const char *Name);

IPAttributeStruct *_AttrMallocAttributeNameType(const char *Name,
						IPAttributeType Type);
IPAttributeStruct *_AttrMallocAttributeHashNum(IPAttrNumType AttribHashNum, 
					       IPAttributeType Type);
IPAttributeStruct *_AttrMallocAttributeIDType(IPAttrNumType AttribID, 
					  IPAttributeType Type);
void _AttrFreeAttributeData(IPAttributeStruct *Attr);

const char **AttrCopyValidAttrNameList(const char **AttrNames);
IPAttributeStruct *AttrCopyAttributes(const IPAttributeStruct *Src);
IPAttributeStruct *AttrCopyAttributes2(const IPAttributeStruct *Src,
				       int AllAttrs);
IPAttributeStruct *AttrCopyOneAttribute(const IPAttributeStruct *Src);
IPAttributeStruct *AttrCopyOneAttribute2(const IPAttributeStruct *Src,
					 int AllAttr);
IPAttributeStruct *AttrMergeAttributes(IPAttributeStruct *Orig,
				       const IPAttributeStruct *Src,
				       int Replace);
IPAttributeStruct *AttrGetLastAttr(IPAttributeStruct *AList);
IPAttributeStruct *AttrGetPrevAttr(IPAttributeStruct *AList,
				   const IPAttributeStruct *A);

/* Functions in miscatt1.c */

const char *AttrGetAttribName(const IPAttributeStruct *Attr);
IPAttrNumType AttrGetAttribHashNumber(const char *AttribName);
IPAttributeStruct *AttrFindAttributeHashNum(const IPAttributeStruct *Attrs, 
				   	    IPAttrNumType AttrNum);
void AttrInitHashTbl(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_MISC_ATTR_H */
