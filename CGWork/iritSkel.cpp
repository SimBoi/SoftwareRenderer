#include "stdafx.h"
#include "iritSkel.h"

/*****************************************************************************
* Skeleton for an interface to a parser to read IRIT data files.			 *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
* Written by:  Gershon Elber				Ver 1.0, Feb 2002				 *
* Minimal changes made by Amit Mano			November 2008					 *
******************************************************************************/

// global variable to hold data contained in the currently open .itd file
using namespace CG;
Object parentObject;
Object* selectedObject = nullptr;
int object_index = 0;
bool initialized = false;
Camera camera;
int polygonal_fineness = 20; // Defaule Polygonal FineNess

IPFreeformConvStateStruct CGSkelFFCState = {
	FALSE,          /* Talkative */
	FALSE,          /* DumpObjsAsPolylines */
	TRUE,           /* DrawFFGeom */
	FALSE,          /* DrawFFMesh */
	{ 10, 10, 10 }, /* 10 isocurves peru/v/w direction. */
	100,            /* 100 point samples along a curve. */
	SYMB_CRV_APPROX_UNIFORM,  /* CrvApproxMethod */
	FALSE,   /* ShowIntrnal */
	FALSE,   /* CubicCrvsAprox */
	20,      /* Polygonal FineNess */
	FALSE,   /* ComputeUV */
	TRUE,    /* ComputeNrml */
	FALSE,   /* FourPerFlat */
	0,       /* OptimalPolygons */
	FALSE,   /* BBoxGrid */
	TRUE,    /* LinearOnePolyFlag */
	FALSE
};

//CGSkelProcessIritDataFiles(argv + 1, argc - 1);


/*****************************************************************************
* DESCRIPTION:                                                               *
* Main module of skeleton - Read command line and do what is needed...	     *
*                                                                            *
* PARAMETERS:                                                                *
*   FileNames:  Files to open and read, as a vector of strings.              *
*   NumFiles:   Length of the FileNames vector.								 *
*                                                                            *
* RETURN VALUE:                                                              *
*   bool:		false - fail, true - success.                                *
*****************************************************************************/
bool CGSkelProcessIritDataFiles(CString &FileNames, int NumFiles)
{
	// delete objects from previous file and reset the camera in preparation to store data from the new file
	parentObject = Object();
	initialized = false;
	camera = Camera();

	IPObjectStruct *PObjects;
	IrtHmgnMatType CrntViewMat;
	IPTraverseObjHierarchyStruct TraversState;
	/* Get the data files: */
	IPSetFlattenObjects(FALSE);
	CStringA CStr(FileNames);
	if ((PObjects = IPGetDataFiles((const char* const *)&CStr, 1/*NumFiles*/, TRUE, FALSE)) == NULL)
		return false;
	PObjects = IPResolveInstances(PObjects);

	if (IPWasPrspMat)
		MatMultTwo4by4(CrntViewMat, IPViewMat, IPPrspMat);
	else
		IRIT_GEN_COPY(CrntViewMat, IPViewMat, sizeof(IrtHmgnMatType));

	/* Here some useful parameters to play with in tesselating freeforms: */
	CGSkelFFCState.FineNess = polygonal_fineness;   /* Res. of tesselation, larger is finer. */
	CGSkelFFCState.ComputeUV = TRUE;   /* Wants UV coordinates for textures. */
	CGSkelFFCState.FourPerFlat = TRUE;/* 4 poly per ~flat patch, 2 otherwise.*/
	CGSkelFFCState.LinearOnePolyFlag = TRUE;    /* Linear srf gen. one poly. */

	/* Traverse ALL the parsed data, recursively. */
	IPTraverseObjHierarchyInitState(&TraversState);
	TraversState.ApplyFunc = (IPApplyObjFuncType) CGSkelDumpOneTraversedObject;
	IRIT_GEN_COPY(TraversState.Mat, CrntViewMat, sizeof(IrtHmgnMatType));
	IPTraverseObjListHierarchy(PObjects, &TraversState);

	// set intuitive sensitivity
	setDefaultSensitivity();
	return true;
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Call back function of IPTraverseObjListHierarchy. Called on every non    *
* list object found in hierarchy.                                            *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:       Non list object to handle.                                   *
*   Mat:        Transformation matrix to apply to this object.               *
*   Data:       Additional data.                                             *
*                                                                            *
* RETURN VALUE:                                                              *
*   void									                                 *
*****************************************************************************/
void CGSkelDumpOneTraversedObject(IPObjectStruct *PObj,
                                  IrtHmgnMatType Mat,
                                  void *Data)
{
	IPObjectStruct *PObjs;

	if (IP_IS_FFGEOM_OBJ(PObj))
		PObjs = IPConvertFreeForm(PObj, &CGSkelFFCState);
	else
		PObjs = PObj;

	for (PObj = PObjs; PObj != NULL; PObj = PObj -> Pnext)
		if (!CGSkelStoreData(PObj)) 
			exit(1);
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Prints the data from given geometry object.								 *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:       Object to print.                                             *
*   Indent:     Column of indentation.                                       *
*                                                                            *
* RETURN VALUE:                                                              *
*   bool:		false - fail, true - success.                                *
*****************************************************************************/
bool CGSkelStoreData(IPObjectStruct *PObj)
{
	// create new Object
	Object childObject;
	childObject.hasShape = true;

	int i;
	const char *Str;
	double RGB[3], Transp;
	IPPolygonStruct *PPolygon;
	IPVertexStruct *PVertex;
	const IPAttributeStruct *Attrs =
        AttrTraceAttributes(PObj -> Attr, PObj -> Attr);

	if (PObj -> ObjType != IP_OBJ_POLY) {
		AfxMessageBox(_T("Non polygonal object detected and ignored"));
		return true;
	}

	/* You can use IP_IS_POLYGON_OBJ(PObj) and IP_IS_POINTLIST_OBJ(PObj) 
	   to identify the type of the object*/

	if (CGSkelGetObjectColor(PObj, RGB))
	{
		int r = (int)(RGB[0] * 255);
		int g = (int)(RGB[1] * 255);
		int b = (int)(RGB[2] * 255);
		childObject.color = RGB(r, g, b);
	}
	if (CGSkelGetObjectTransp(PObj, &Transp))
	{
		/* transparency code */
	}
	if ((Str = CGSkelGetObjectTexture(PObj)) != NULL)
	{
		/* volumetric texture code */
	}
	if ((Str = CGSkelGetObjectPTexture(PObj)) != NULL)
	{
		/* parametric texture code */
	}
	if (Attrs != NULL) 
	{
		printf("[OBJECT\n");
		while (Attrs) {
			/* attributes code */
			Attrs = AttrTraceAttributes(Attrs, NULL);
		}
	}
	for (PPolygon = PObj->U.Pl; PPolygon != NULL; PPolygon = PPolygon->Pnext)
	{
		// create new face
		Face face;
		
		if (PPolygon->PVertex == NULL) {
			AfxMessageBox(_T("Dump: Attemp to dump empty polygon"));
			return false;
		}
		/* Count number of vertices. */
		for (PVertex = PPolygon->PVertex->Pnext, i = 1;
			PVertex != PPolygon->PVertex && PVertex != NULL;
			PVertex = PVertex->Pnext, i++);
		
		PVertex = PPolygon->PVertex;
		do {			     /* Assume at least one edge in polygon! */
			// crate new vertex
			Vertex vertex;

			// store provided vertex normal
			if (IP_HAS_NORMAL_VRTX(PVertex))
			{
				vertex.normal.x = PVertex->Normal[0];
				vertex.normal.y = PVertex->Normal[1];
				vertex.normal.z = PVertex->Normal[2];
				vertex.normal.w = 0;
			}

			// store vertex position
			vertex.localPosition.x = PVertex->Coord[0];
			vertex.localPosition.y = PVertex->Coord[1];
			vertex.localPosition.z = PVertex->Coord[2];
			vertex.localPosition.w = 1;

			// add vertex to face
			face.vertices.push_back(vertex);

			PVertex = PVertex->Pnext;
		} while (PVertex != PPolygon->PVertex && PVertex != NULL);

		if (IP_HAS_PLANE_POLY(PPolygon))
		{
			face.normal.x = PPolygon->Plane[0];
			face.normal.y = PPolygon->Plane[1];
			face.normal.z = PPolygon->Plane[2];
			face.normal.w = 0;
		}
		else // calculate face normal
		{
			vec4 u, v;
			// calculate u
			Vertex prevVertex = face.vertices.back();
			for (auto& vertex : face.vertices)
			{
				if (prevVertex.localPosition != vertex.localPosition)
				{
					u = vertex.localPosition - prevVertex.localPosition;
					break;
				}
				prevVertex = vertex;
			}
			// calculate v
			prevVertex = face.vertices.back();
			for (auto& vertex : face.vertices)
			{
				if (prevVertex.localPosition != vertex.localPosition)
				{
					v = vertex.localPosition - prevVertex.localPosition;
					if (!vec4::AreParallel(u, v)) break;
				}
				prevVertex = vertex;
			}

			face.normal = vec4::cross(u, v).normalized();
		}

		// calculate center of face (the point will be used to render the normal from)
		for (auto& vertex : face.vertices)
		{
			face.center += vertex.localPosition;
		}
		face.center = face.center / face.vertices.size();

		// add face to object
		childObject.faces.push_back(face);
	}

	// calculate child bounding box
	childObject.CalcBoundingBox();

	// add child object to the parents children list
	parentObject.children.push_back(childObject);
	
	// calculate parent bounding box
	parentObject.ReCalcBoundingBox(childObject);


	return true;
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Returns the color of an object.                                          *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:   Object to get its color.                                         *
*   RGB:    as 3 floats in the domain [0, 1].                                *
*                                                                            *
* RETURN VALUE:                                                              *
*   int:    TRUE if object has color, FALSE otherwise.                       *
*****************************************************************************/
int CGSkelGetObjectColor(IPObjectStruct *PObj, double RGB[3])
{
	static int TransColorTable[][4] = {
		{ /* BLACK	*/   0,    0,   0,   0 },
		{ /* BLUE	*/   1,    0,   0, 255 },
		{ /* GREEN	*/   2,    0, 255,   0 },
		{ /* CYAN	*/   3,    0, 255, 255 },
		{ /* RED	*/   4,  255,   0,   0 },
		{ /* MAGENTA 	*/   5,  255,   0, 255 },
		{ /* BROWN	*/   6,   50,   0,   0 },
		{ /* LIGHTGRAY	*/   7,  127, 127, 127 },
		{ /* DARKGRAY	*/   8,   63,  63,  63 },
		{ /* LIGHTBLUE	*/   9,    0,   0, 255 },
		{ /* LIGHTGREEN	*/   10,   0, 255,   0 },
		{ /* LIGHTCYAN	*/   11,   0, 255, 255 },
		{ /* LIGHTRED	*/   12, 255,   0,   0 },
		{ /* LIGHTMAGENTA */ 13, 255,   0, 255 },
		{ /* YELLOW	*/   14, 255, 255,   0 },
		{ /* WHITE	*/   15, 255, 255, 255 },
		{ /* BROWN	*/   20,  50,   0,   0 },
		{ /* DARKGRAY	*/   56,  63,  63,  63 },
		{ /* LIGHTBLUE	*/   57,   0,   0, 255 },
		{ /* LIGHTGREEN	*/   58,   0, 255,   0 },
		{ /* LIGHTCYAN	*/   59,   0, 255, 255 },
		{ /* LIGHTRED	*/   60, 255,   0,   0 },
		{ /* LIGHTMAGENTA */ 61, 255,   0, 255 },
		{ /* YELLOW	*/   62, 255, 255,   0 },
		{ /* WHITE	*/   63, 255, 255, 255 },
		{		     -1,   0,   0,   0 }
	};
	int i, j, Color, RGBIColor[3];

	if (AttrGetObjectRGBColor(PObj,
		&RGBIColor[0], &RGBIColor[1], &RGBIColor[2])) {
			for (i = 0; i < 3; i++)
				RGB[i] = RGBIColor[i] / 255.0;

			return TRUE;
	}
	else if ((Color = AttrGetObjectColor(PObj)) != IP_ATTR_NO_COLOR) {
		for (i = 0; TransColorTable[i][0] >= 0; i++) {
			if (TransColorTable[i][0] == Color) {
				for (j = 0; j < 3; j++)
					RGB[j] = TransColorTable[i][j+1] / 255.0;
				return TRUE;
			}
		}
	}

	return FALSE;
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Returns the volumetric texture of an object, if any.                     *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:   Object to get its volumetric texture.                            *
*                                                                            *
* RETURN VALUE:                                                              *
*   char *:    Name of volumetric texture map to apply, NULL if none.        *
*****************************************************************************/
const char *CGSkelGetObjectTexture(IPObjectStruct *PObj)
{
	return AttrGetObjectStrAttrib(PObj, "texture");
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Returns the parametric texture of an object, if any.                     *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:   Object to get its parametric texture.                            *
*                                                                            *
* RETURN VALUE:                                                              *
*   char *:    Name of parametric texture map to apply, NULL if none.        *
*****************************************************************************/
const char *CGSkelGetObjectPTexture(IPObjectStruct *PObj)
{
	return AttrGetObjectStrAttrib(PObj, "ptexture");
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Returns the transparency level of an object, if any.                     *
*                                                                            *
* PARAMETERS:                                                                *
*   PObj:   Object to get its volumetric texture.                            *
*   Transp: Transparency level between zero and one.                         *
*                                                                            *
* RETURN VALUE:                                                              *
*   int:    TRUE if object has transparency, FALSE otherwise.                *
*****************************************************************************/
int CGSkelGetObjectTransp(IPObjectStruct *PObj, double *Transp)
{
	*Transp = AttrGetObjectRealAttrib(PObj, "transp");

	return !IP_ATTR_IS_BAD_REAL(*Transp);
}

/*****************************************************************************
* DESCRIPTION:                                                               *
*   Computes the INVERSE of a given matrix M.			             *
*	The matrix M is is not modified.			             *
*   The matrix M is assumed to be 4 by 4 (a transformation matrix).          *
*   Return TRUE if the inverted matrix (InvM) exists.                        *
*                                                                            *
* PARAMETERS:                                                                *
*   M:          Original matrix to invert.                                   *
*   InvM:       Inverted matrix will be placed here.	                     *
*                                                                            *
* RETURN VALUE:                                                              *
*   int:        TRUE if inverse exists, FALSE otherwise.                     *
*****************************************************************************/
int CGSkelInverseMatrix(double M[4][4], double InvM[4][4])
{
	return MatInverseMatrix(M, InvM);
}

