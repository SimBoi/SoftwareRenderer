/*****************************************************************************
* Setting attributes for geometric objects.				     *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
* Written by:  Gershon Elber				Ver 0.2, Mar. 1990   *
*****************************************************************************/

#ifndef IRIT_ATTRIBUTE_H
#define IRIT_ATTRIBUTE_H

#include "iritprsr.h"
#include "miscattr.h"

#define ATTR_OBJ_IS_INVISIBLE(PObj) \
	(AttrGetObjectIntAttrib((PObj), "Invisible") != IP_ATTR_BAD_INT)

#define ATTR_OBJ_ATTR_EXIST(PObj, Name) (AttrFindAttribute(PObj -> Attr, \
							   Name) != NULL)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

void AttrSetObjectColor(IPObjectStruct *PObj, int Color);
int AttrGetObjectColor(const IPObjectStruct *PObj);
void AttrSetObjectRGBColor(IPObjectStruct *PObj, int Red, int Green, int Blue);
int AttrGetObjectRGBColor(const IPObjectStruct *PObj,
			  int *Red,
			  int *Green,
			  int *Blue);
int AttrGetObjectRGBColor2(const IPObjectStruct *PObj,
			   const char *Name,
			   int *Red,
			   int *Green,
			   int *Blue);
void AttrSetRGBDoubleColor(IPAttributeStruct **Attrs,
			   double Red,
			   double Green,
			   double Blue);
int AttrGetRGBDoubleColor(const IPAttributeStruct *Attrs,
			  double *Red,
			  double *Green,
			  double *Blue);
void AttrSetObjectWidth(IPObjectStruct *PObj, IrtRType Width);
IrtRType AttrGetObjectWidth(const IPObjectStruct *PObj);

void AttrSetObjectIntAttrib(IPObjectStruct *PObj, const char *Name, int Data);
void AttrSetObjectIntAttrib2(IPObjectStruct *PObj,
			     IPAttrNumType AttribNum,
			     int Data);
int AttrGetObjectIntAttrib(const IPObjectStruct *PObj, const char *Name);
int AttrGetObjectIntAttrib2(const IPObjectStruct *PObj,
			    IPAttrNumType AttribNum);

void AttrSetObjectRealAttrib(IPObjectStruct *PObj,
			     const char *Name,
			     IrtRType Data);
void AttrSetObjectRealAttrib2(IPObjectStruct *PObj,
			      IPAttrNumType AttribNum,
			      IrtRType Data);
IrtRType AttrGetObjectRealAttrib(const IPObjectStruct *PObj, const char *Name);
IrtRType AttrGetObjectRealAttrib2(const IPObjectStruct *PObj,
				  IPAttrNumType AttribNum);

void AttrSetObjectRealPtrAttrib(IPObjectStruct *PObj,
				const char *Name,
				IrtRType *Data,
				int DataLen);
void AttrSetObjectRealPtrAttrib2(IPObjectStruct *PObj,
				 IPAttrNumType AttribNum,
				 IrtRType *Data,
				 int DataLen);
IrtRType *AttrGetObjectRealPtrAttrib(const IPObjectStruct *PObj,
				     const char *Name);
IrtRType *AttrGetObjectRealPtrAttrib2(const IPObjectStruct *PObj,
				      IPAttrNumType AttribNum);

void AttrSetObjectUVAttrib(IPObjectStruct *PObj,
			   const char *Name,
			   IrtRType U,
			   IrtRType V);
void AttrSetObjectUVAttrib2(IPObjectStruct *PObj,
			    IPAttrNumType AttribNum,
			    IrtRType U,
			    IrtRType V);
float *AttrGetObjectUVAttrib(const IPObjectStruct *PObj, const char *Name);
float *AttrGetObjectUVAttrib2(const IPObjectStruct *PObj,
			      IPAttrNumType AttribNum);

void AttrSetObjectPtrAttrib(IPObjectStruct *PObj,
			    const char *Name,
			    VoidPtr Data);
void AttrSetObjectPtrAttrib2(IPObjectStruct *PObj,
			     IPAttrNumType AttribNum,
			     VoidPtr Data);
VoidPtr AttrGetObjectPtrAttrib(const IPObjectStruct *PObj, const char *Name);
VoidPtr AttrGetObjectPtrAttrib2(const IPObjectStruct *PObj,
				IPAttrNumType AttribNum);

void AttrSetObjectRefPtrAttrib(IPObjectStruct *PObj,
			       const char *Name,
			       VoidPtr Data);
void AttrSetObjectRefPtrAttrib2(IPObjectStruct *PObj,
				IPAttrNumType AttribNum,
				VoidPtr Data);
VoidPtr AttrGetObjectRefPtrAttrib(const IPObjectStruct *PObj,
				  const char *Name);
VoidPtr AttrGetObjectRefPtrAttrib2(const IPObjectStruct *PObj,
				   IPAttrNumType AttribNum);

void AttrSetObjectStrAttrib(IPObjectStruct *PObj,
			    const char *Name,
			    const char *Data);
void AttrSetObjectStrAttrib2(IPObjectStruct *PObj,
			     IPAttrNumType AttribNum,
			     const char *Data);
const char *AttrGetObjectStrAttrib(const IPObjectStruct *PObj,
				   const char *Name);
const char *AttrGetObjectStrAttrib2(const IPObjectStruct *PObj,
				    IPAttrNumType AttribNum);

void AttrSetObjectObjAttrib(IPObjectStruct *PObj,
			    const char *Name,
			    IPObjectStruct *Data,
			    int CopyData);
void AttrSetObjectObjAttrib2(IPObjectStruct *PObj,
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

void AttrFreeObjectAttribute(IPObjectStruct *PObj, const char *Name);

IPAttributeStruct *AttrCopyOneAttribute(const IPAttributeStruct *Src);
IPAttributeStruct *AttrCopyAttributes(const IPAttributeStruct *Src);
void AttrPropagateAttr(IPObjectStruct *PObj, const char *AttrName);
void AttrPropagateRGB2Vrtx(IPObjectStruct *PObj);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_ATTRIBUTE_H */
