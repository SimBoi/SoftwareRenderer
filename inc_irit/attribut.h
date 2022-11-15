/*****************************************************************************
* Setting attributes for geometric objects.				     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
* Written by:  Gershon Elber				Ver 0.2, Mar. 1990   *
*****************************************************************************/

#ifndef IRIT_ATTRIBUTE_H
#define IRIT_ATTRIBUTE_H

#include "inc_irit/iritprsr.h"
#include "inc_irit/miscattr.h"

#define ATTR_OBJ_IS_INVISIBLE(PObj) \
	(AttrIDGetObjectIntAttrib((PObj), \
			  IRIT_ATTR_CREATE_ID(invisible)) != IP_ATTR_BAD_INT)

#define ATTR_OBJ_ATTR_EXIST(PObj, AttrID) (AttrIDFindAttribute(PObj -> Attr, \
							     AttrID) != NULL)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* From attribute.c */

void AttrSetObjectColor(const IPObjectStruct *PObj, int Color);
int AttrGetObjectColor(const IPObjectStruct *PObj);
void AttrSetObjectRGBColor(const IPObjectStruct *PObj,
			   int Red,
			   int Green,
			   int Blue);
int AttrGetObjectRGBColor(const IPObjectStruct *PObj,
			  int *Red,
			  int *Green,
			  int *Blue);
int AttrGetObjectRGBColor2(const IPObjectStruct *PObj,
			   const char *Name,
			   int *Red,
			   int *Green,
			   int *Blue);
void AttrSetRGBDoubleColor(const IPAttributeStruct **Attrs,
			   double Red,
			   double Green,
			   double Blue);
int AttrGetRGBDoubleColor(const IPAttributeStruct *Attrs,
			  double *Red,
			  double *Green,
			  double *Blue);
void AttrSetObjectWidth(const IPObjectStruct *PObj, IrtRType Width);
IrtRType AttrGetObjectWidth(const IPObjectStruct *PObj);

void AttrSetObjectIntAttrib(const IPObjectStruct *PObj,
			    const char *Name,
			    int Data);
void AttrSetObjectIntAttrib2(const IPObjectStruct *PObj,
			     IPAttrNumType AttribNum,
			     int Data);
int AttrGetObjectIntAttrib(const IPObjectStruct *PObj, const char *Name);
int AttrGetObjectIntAttrib2(const IPObjectStruct *PObj,
			    IPAttrNumType AttribNum);

void AttrSetObjectRealAttrib(const IPObjectStruct *PObj,
			     const char *Name,
			     IrtRType Data);
void AttrSetObjectRealAttrib2(const IPObjectStruct *PObj,
			      IPAttrNumType AttribNum,
			      IrtRType Data);
IrtRType AttrGetObjectRealAttrib(const IPObjectStruct *PObj, const char *Name);
IrtRType AttrGetObjectRealAttrib2(const IPObjectStruct *PObj,
				  IPAttrNumType AttribNum);

void AttrSetObjectRealPtrAttrib(const IPObjectStruct *PObj,
				const char *Name,
				IrtRType *Data,
				int DataLen);
void AttrSetObjectRealPtrAttrib2(const IPObjectStruct *PObj,
				 IPAttrNumType AttribNum,
				 IrtRType *Data,
				 int DataLen);
IrtRType *AttrGetObjectRealPtrAttrib(const IPObjectStruct *PObj,
				     const char *Name);
IrtRType *AttrGetObjectRealPtrAttrib2(const IPObjectStruct *PObj,
				      IPAttrNumType AttribNum);

void AttrSetObjectUVAttrib(const IPObjectStruct *PObj,
			   const char *Name,
			   IrtRType U,
			   IrtRType V);
void AttrSetObjectUVAttrib2(const IPObjectStruct *PObj,
			    IPAttrNumType AttribNum,
			    IrtRType U,
			    IrtRType V);
float *AttrGetObjectUVAttrib(const IPObjectStruct *PObj, const char *Name);
float *AttrGetObjectUVAttrib2(const IPObjectStruct *PObj,
			      IPAttrNumType AttribNum);

void AttrSetObjectPtrAttrib(const IPObjectStruct *PObj,
			    const char *Name,
			    VoidPtr Data);
void AttrSetObjectPtrAttrib2(const IPObjectStruct *PObj,
			     IPAttrNumType AttribNum,
			     VoidPtr Data);
VoidPtr AttrGetObjectPtrAttrib(const IPObjectStruct *PObj, const char *Name);
VoidPtr AttrGetObjectPtrAttrib2(const IPObjectStruct *PObj,
				IPAttrNumType AttribNum);

void AttrSetObjectRefPtrAttrib(const IPObjectStruct *PObj,
			       const char *Name,
			       VoidPtr Data);
void AttrSetObjectRefPtrAttrib2(const IPObjectStruct *PObj,
				IPAttrNumType AttribNum,
				VoidPtr Data);
VoidPtr AttrGetObjectRefPtrAttrib(const IPObjectStruct *PObj,
				  const char *Name);
VoidPtr AttrGetObjectRefPtrAttrib2(const IPObjectStruct *PObj,
				   IPAttrNumType AttribNum);

void AttrSetObjectStrAttrib(const IPObjectStruct *PObj,
			    const char *Name,
			    const char *Data);
void AttrSetObjectStrAttrib2(const IPObjectStruct *PObj,
			     IPAttrNumType AttribNum,
			     const char *Data);
const char *AttrGetObjectStrAttrib(const IPObjectStruct *PObj,
				   const char *Name);
const char *AttrGetObjectStrAttrib2(const IPObjectStruct *PObj,
				    IPAttrNumType AttribNum);

void AttrSetObjectObjAttrib(const IPObjectStruct *PObj,
			    const char *Name,
			    IPObjectStruct *Data,
			    int CopyData);
void AttrSetObjectObjAttrib2(const IPObjectStruct *PObj,
			     IPAttrNumType AttribNum,
			     IPObjectStruct *Data,
			     int CopyData);
void AttrSetObjAttrib(IPAttributeStruct **Attrs,
		      const char *Name,
		      IPObjectStruct *Data,
		      int CopyData);
void AttrSetObjAttrib2(IPAttributeStruct **Attrs,
		       IPAttrNumType AttribNum,
		       IPObjectStruct *Data,
		       int CopyData);
IPObjectStruct *AttrGetObjectObjAttrib(const IPObjectStruct *PObj,
				       const char *Name);
IPObjectStruct *AttrGetObjectObjAttrib2(const IPObjectStruct *PObj,
					IPAttrNumType AttribNum);
IPObjectStruct *AttrGetObjAttrib(const IPAttributeStruct *Attrs,
				 const char *Name);
IPObjectStruct *AttrGetObjAttrib2(const IPAttributeStruct *Attrs,
				  IPAttrNumType AttribNum);

void AttrFreeObjectAttribute(const IPObjectStruct *PObj, const char *Name);

IPAttributeStruct *AttrGetObjectGeomAttr(const IPObjectStruct *PObj);
IPObjectStruct *Attr2IritObject(const IPAttributeStruct *Attr);
IPAttributeStruct *AttrCopyOneAttribute(const IPAttributeStruct *Src);
IPAttributeStruct *AttrCopyOneAttribute2(const IPAttributeStruct *Src,
					 int AllAttr);
IPAttributeStruct *AttrCopyAttributes(const IPAttributeStruct *Src);
void AttrPropagateAttr(const IPObjectStruct *PObj, const char *AttrName);
void AttrPropagateRGB2Vrtx(const IPObjectStruct *PObj);
IPObjectStruct const * const *AttrFindObjsWithAttr(
					     const IPObjectStruct *PObjs,
					     const char *AttrName,
					     const IPObjectStruct *AttrVal,
					     int LeavesOnly,
					     int Negate);
IPObjectStruct *AttrMergeGeomSimilarAttrs(const IPObjectStruct *PObjList,
					  const char *AttrName,
					  int MergeOptions);

/* From attribute_id.c */

void AttrIDSetObjectColor(const IPObjectStruct *PObj, int Color);
int AttrIDGetObjectColor(const IPObjectStruct *PObj);
void AttrIDSetObjectRGBColor(const IPObjectStruct *PObj,
			     int Red,
			     int Green,
			     int Blue);
int AttrIDGetObjectRGBColor(const IPObjectStruct *PObj,
			    int *Red,
			    int *Green,
			    int *Blue);
int AttrIDGetObjectRGBColor2(const IPObjectStruct *PObj,
			     int *Red,
			     int *Green,
			     int *Blue);
void AttrIDSetObjectWidth(const IPObjectStruct *PObj, IrtRType Width);
IrtRType AttrIDGetObjectWidth(const IPObjectStruct *PObj);
void AttrIDSetObjectIntAttrib(const IPObjectStruct *PObj,
			      IPAttrIDType ID,
			      int Data);
int AttrIDGetObjectIntAttrib(const IPObjectStruct *PObj, IPAttrIDType ID);
void AttrIDSetObjectPtrAttrib(const IPObjectStruct *PObj,
			      IPAttrIDType ID,
			      VoidPtr Data);
VoidPtr AttrIDGetObjectPtrAttrib(const IPObjectStruct *PObj, IPAttrNumType ID);
void AttrIDSetObjectRefPtrAttrib(const IPObjectStruct *PObj,
				 IPAttrIDType ID,
				 VoidPtr Data);
VoidPtr AttrIDGetObjectRefPtrAttrib(const IPObjectStruct *PObj,
				    IPAttrIDType ID);
void AttrIDSetObjectRealAttrib(const IPObjectStruct *PObj,
			       IPAttrIDType ID,
			       IrtRType Data);
IrtRType AttrIDGetObjectRealAttrib(const IPObjectStruct *PObj,
				   IPAttrIDType ID);
void AttrIDSetObjectRealPtrAttrib(const IPObjectStruct *PObj,
				  IPAttrIDType ID,
				  IrtRType *Data,
				  int DataLen);
IrtRType *AttrIDGetObjectRealPtrAttrib(const IPObjectStruct *PObj,
				       IPAttrIDType ID);
void AttrIDSetObjectUVAttrib(const IPObjectStruct *PObj,
			     IPAttrIDType ID,
			     IrtRType U,
			     IrtRType V);
float *AttrIDGetObjectUVAttrib(const IPObjectStruct *PObj, IPAttrIDType ID);
void AttrIDSetObjectStrAttrib(const IPObjectStruct *PObj,
			      IPAttrIDType ID,
			      const char *Data);
const char *AttrIDGetObjectStrAttrib(const IPObjectStruct *PObj,
				     IPAttrIDType ID);
void AttrIDSetObjectObjAttrib(const IPObjectStruct *PObj,
			      IPAttrIDType ID,
			      IPObjectStruct *Data,
			      int CopyData);
void AttrIDSetObjAttrib(IPAttributeStruct **Attrs,
			IPAttrIDType ID,
			IPObjectStruct *Data,
			int CopyData);
IPObjectStruct *AttrIDGetObjectObjAttrib(const IPObjectStruct *PObj,
					 IPAttrIDType ID);
IPObjectStruct *AttrIDGetObjAttrib(const IPAttributeStruct *Attrs,
				   IPAttrIDType ID);
void AttrIDFreeObjectAttribute(const IPObjectStruct *PObj, IPAttrIDType ID);
void AttrIDPropagateAttr(const IPObjectStruct *PObj, IPAttrIDType AttrID);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_ATTRIBUTE_H */
