/*****************************************************************************
* user_gc.h - Header file for Calculating Geometric Covering.                *
******************************************************************************
* Written by Nadav Shragai, February 2010.                                   *
*****************************************************************************/

/****************************************************************************
*                                                                           *
*   This file conaains an interface for the Geometric Covering Problem      *
* solver. The main function for this interface is UserGCSolveGeoProblem.    *
* The diagram below display the main inputs and outputs for this function.  *
* The input/output rectangles have a first line describing the input/output *
* while the following lines present the name of the variables containing    *
* the input/output as they are given to UserGCSolveGeoProblem.              *
*                                                                           *
*   *******************************        *********************            *
*   * Possible observation points *        * Geometry to cover *            *
*   *   Problem -> ObsPtsGroups   *    .---* Problem -> GeoObj *            *
*   *******************************    |   *********************            *
*                        |             |                                    *
*                        |             |         ************************   *
*                        |             |         *  Occluding geometry  *   *
*                        |             |         * Problem -> Obstacles *   *
*                      \ | /         \ | /       ************************   *
*                       \|/           \|/              |                    *
*               $$$$$$$$$$$$$$$$$$$$$$$$$  /           |                    *
*               $ UserGCSolveGeoProblem $ -------------.                    *
*               $$$$$$$$$$$$$$$$$$$$$$$$$  \                                *
*                 |          |       /|\                                    *
*                 |          |      / | \                                   *
*                 |          |        |      ****************************   *
*                 |          |        -------*        Parameters        *   *
*                 |          |               * Problem -> UVMap         *   *
*                 |          |               * Problem -> SolvingParams *   *
*                 |          |               * Problem -> DebugParams   *   *
*               \ | /      \ | /             ****************************   *
*                \|/        \|/                                             *
*   *******************   ******************************                    *
*   *  Total coverage *   * Solving observation points *                    *
*   * CoverPercentage *   *        SolutionOps         *                    *
*   *******************   ******************************                    *
*                                                                           *
*   In a general Geometric Covering Problem a geometry is given and several *
* possible observation points are given from which the geometry can be      *
* observed. The goal is to find the minimum number of observation points    *
* which together observe the entire geometry, that is, covering the entire  *
* geometry.                                                                 *
*   In this problem we support this basic Geometric Covering Problem with   *
* few additions such as an occluding geometry which isn't required to be    *
* covered but can possibly interefer with the vision of some of the         *
* obsveration points.                                                       *
*                                                                           *
*   All the inputs are given to UserGCSolveGeoProblem by                    *
* UserGCProblemDefinitionStruct *Problem.                                   *
* Obstacles: An IPObjectStruct struct of type IP_OBJ_POLY. That is, the     *
*     input is expected to be a group of polygons. Any number of such       *
*     structs can be connected using Pnext (However, IP_OBJ_LIST_OBJ are    *
*     not acceptable). Any other types of inputs are just ignored. If       *
*     Obstacles is NULL, it's ignored.                                      *
* GeoObj: Has the same requirements as Obstacles. Additionally it must      *
*     contain UV parameterizatoin (See AttrSetUVAttrib) for every polygon.  *
*     Each IPObjectStruct in the connected Pnext list is considered as one  *
*     surface. The UV paremetrization of polygons in each surface mustn't   *
*     overlap each other. However, the UV parametrization between differen  *
*     surfaces are allowed to overlap each other since the algorithm        *
*     rearrange their UV parametrization in order to eliminate those        *
*     overalps. (the function IRndrVisMapPrepareUVValuesOfGeoObj rndr_lib.h *
*     can be used in order to eliminate overlapping between several         *
*     polygonal objects prior to calling UserGCSolveGeoProblem).            *
* ObsPtsGroups: Defines the observation points. This is a NULL terminated   *
*     vector of pointers to UserGCObsPtGroupTypeStruct (Which will be       *
*     referred here as suggestions group). Each suggestions group defines   *
*     several UserGCObsPtSuggestionStruct (Which will be referred here as   *
*     suggestion) which are used by the solver as possible obsevation       *
*     points for the Geometric Covering Problem. Those suggestions can be   *
*     defined in different ways according to                                *
*     ObsPtsGroups -> PredefinedSuggestions.PredefinedSuggestionType.       *
*     USER_GC_CUSTOM_SUGGESTION: The suggestions are given in               *
*         ObsPtsGroups -> Suggestions as a NULL terminated vector of        *
*         pointer of UserGCObsPtSuggestionStruct.                           *
*         Suggestions.Direction is the direction the observation point look *
*         at. Suggestions.ObsPt is the location of the observation point.   *
*         The location can be USER_GC_INF_VEC to mark infinity in           *
*         orthographic view.                                                *
*     USER_GC_SIX_MAIN_AXIS: The suggestions are the three main axis and    *
*         their antipodals.                                                 *
*     USER_GC_SPHERE_POLYHEDRON: A polyhedron such as dodecahedron is given *
*         in PredefinedSuggestionType.Polyhedron.Poly (no check is done     *
*         over the polygonal object to make sure it's a closed shape). The  *
*         polyhedron is divided into triangles. In each level each triangle *
*         is divided into four triangles by connecting the middles of its   *
*         three edges. When the levels end, the vertices of all the         *
*         triangles are normalized and used as locations over the unit      *
*         sphere. The number of levels is given by                          *
*         PredefinedSuggestionType.Polyhedron.Level                         *
*     USER_GC_SPHERE_COVER_#: A # number of location spread evenly over the *
*         unit sphere. # can be 4, 20, 50, 100 and 130.                     *
*     USER_GC_PERIMETER: Define guards in equals distance on a curver over  *
*         a terrain. The file PredefinedSuggestionType.Perimeter.CrfFileName*
*         must contain:                                                     *
*         1. An object containing a surface with the name "Terrain" (case   *
*            insensitive). The surface must face up. This can also be a     *
*            polygonal object as long as it has a parametrization and       *
*            normals pointing down (which means the surface points up).     *
*         2. An object containing a 2D curve (2 coordinates or 3 with z=0). *
*            The curve must be inside the parametric domain of the          *
*            surface (1).                                                   *
*            The tangents of the curve must point clockwise.                *
*         3. The file may contain one or more matrices (only the last one   *
*            is considered) which will be applied only on the surface.      *
*         There will be PredefinedSuggestionType.Perimeter.GuardsNumber     *
*         observation points. They will be placed on height                 *
*         PredefinedSuggestionType.Perimeter.GuardsHeight above the surface.*
*         The directions of the observation points will be perpendicular to *
*         the curve while tangent to the surface, pointing outside.         *
*     USER_GC_SURFACE_GRID: Define guards over a surface. The file          *
*         PredefinedSuggestionType.SurfaceGrid.SrfFileName must contain:    *
*         1. An object containing a surface with the name "Terrain" (case   *
*            insensitive). This can also be a polygonal object as long as it*
*            has a parametrization.                                         *
*         2. An object containing a 2D curve (2 coordinates or 3 with z=0). *
*            The curve must be inside the parametric domain of the          *
*            surface (1).                                                   *
*         3. An optional obstacle objects may be defined as well. Each      *
*            object must be a closed shape (surface of polygons).           *
*         4. The file may contain one or more matrices (only the last one   *
*            is considered) which will be applied only on the surface (1)   *
*            and obstacles (3).                                             *
*         PredefinedSuggestionType.SurfaceGrid.GuardsNumber[] defines       *
*         number of guards spread evenly on the x axis and y axis over the  *
*         parametric domain of the surface (1) in a height of               *
*         PredefinedSuggestionType.SurfaceGrid.GuardsHeight. If an          *
*         observation point is defined outside the 2D curve (2) or if it's  *
*         defined inside one of the obstacles (3) it is ignored.            *
*                                                                           *         
*       Notice that in all cases, the suggestiosn are defined regardless of *
*     the geometry defined by Problem -> GeoObj and Problem -> Obstacles.   *
*     It's the user's responsiblity to keep the the compatibility between   *
*     all the definitions.                                                  *
*       When ObsPtsGroups -> PredefinedSuggestions.PredefinedSuggestionType *
*     isn't USER_GC_CUSTOM_SUGGESTION, UserGCSolveGeoProblem interprets the *
*     PredefinedSuggestions into several suggestions and put them into      *
*     ObsPtsGroups -> Suggestions while setting                             *
*     ObsPtsGroups -> PredefinedSuggestions.PredefinedSuggestionType to     *
*     USER_GC_CUSTOM_SUGGESTION. This is an additional output of            *
*     UserGCSolveGeoProblem.                                                *
*       Each suggestions group has additional two parameters in ObsPtType.  *
*     ZAngle: The vertical aperture of all the suggestions in the           *
*         suggestions group. Allowed values are between 0 to 175 degrees.   *
*     XYAngle: The horizontal aperture of all the suggestions in the        *
*         suggestions group. Allowed values are between 0 to 360 degrees.   *
*         (for USER_GC_SURFACE_GRID this values is forced to be 360 degrees)*
* UVMap: A buffer in the size of the visiblity map (the visibility map is   *
*     described below) where value 0 mark a UV value which isn't required   *
*     to be covered. Notice that the UV values here are according to the    *
*     final UV values in the visibility maps regardless of the individual   *
*     UV domains of the different input objects in GeoObj.                  *
* SolvingParams:                                                            *
*     VisMapWidth, VisMapHeight: The dimensions of the visibilty map and the*
*         zbuffer used during the calculation of the visibilty map.         *
*     VisMapTanAng: The minimum allowed angle between the tangent plane of  *
*         any surface and the view direction of the camera. The angle is in *
*         degrees. Angles below that will render the pixel as invisible.    *
*     VisMapCriticAR: – The maximum allowed ratio between the largest    *
*         edge of a triangle and the smallest edge of a triangle. Ratios    *
*         above that will render the pixel as invisible. Value of 0 means   *
*         ignoring this limitation.                                         *
*     SetCoverParams: Parameter regarding the algoirhtm solving the set     *
*         cover done over the visibility maps in order to find the minimum  *
*         number of obervation points covering the geometry.                *
*         Algorithm: The algorithm used.                                    *
*             USER_GC_GREEDY: Selecting in each stage the visibility map    *
*                 which add the largest contribution to the total cover.    *
*             USER_GC_EXHAUSTIVE: Check all the possible combinations.      *
*             USER_GC_EXACT: Check all the possible combination starting    *
*                 from the most likely combinations. Cannot handle          *
*                 cases when less than 100% coverage is acceptable.         *
*         CoverLimit: Used in USER_GC_EXHAUSTIVE. Accept values between 0   *
*             to 1. Coverage equals or above CoverLimit is acceptable as    *
*             well. Value of 0 means to ignore this variable.               *
*         SizeLimit: Used in USER_GC_EXHAUSTIVE and USER_GC_EXACT. Look     *
*             for solutions with no more than SizeLimit number of           *
*             observation points.                                           *
* DebugParams: Those are described throughly in their definition below.     *
*                                                                           *
*   These are the output of UserGCSolveGeoProblem:                          *
* int: The function returns FALSE if it failed (e.g. because of missing     *
*     file).                                                                *
* SolutionOps: A NULL terminated pointers for UserGCSolutionIndexStruct     *
*     (referred here as solutions) which are the solving observation points *
*     of the Geometric Covering Problem. Each solution contain three        *
*     indices pointing to one suggestion in Problem -> ObsPtsGroups.        *
*     ObsPtGroupIndex: The index of the suggestions group in                *
*         Problem -> ObsPtsGroups.                                          *
*     SuggestionIndex: The index of the suggestion inside the suggestions   *
*         group pointed by ObsPtGroupIndex (This also true when             *
*         ObsPtsGroups -> PredefinedSuggestions.PredefinedSuggestionType    *
*         isn't USER_GC_CUSTOM_SUGGESTION and the suggestions are           *
*         interpreted by UserGCSolveGeoProblem).                            *
*     Index: All the suggestions in all the suggestions groups can be       *
*         enumerated. Enumerate the suggestions in the first suggestions    *
*         group, then the suggestions in the second suggestions group and   *
*         so on. Index points to the index of the suggestion in this        *
*         enumeration.                                                      *
* CoverPercentage: This is the final cover percentage achieved by the       *
*     observation points given in SolutionOps. This is calculated as the    *
*     total number of covered pixel in the visibilty map                    *
*     (RNDR_VISMAP_EMPTY_COLOR) divided by the total number of non empty    *
*     pixels (any pixel except RNDR_VISMAP_DEGEN_COLOR and                  *
*     RNDR_VISMAP_EMPTY_COLOR) in the visibility map (see definitino of     *
*     visibility map below).                                                *
*                                                                           *
*   This is a good place to define visibilty map.                           *
*   Visibility map is a map defined for each observation point over the UV  *
* parametrization. The visibilty map refers to the UV domain after the      *
* changes applied for each input surface in order to prevent overlaps       *
* (when only single surface is given, it may also have its UV               *
* parametrization changed abit). Since the visibilty maps is defined over   *
* the parameterizaiton domain, each pixel corresponds to part of the        *
* geometry to cover. Therefore, each pixel marks the visibility of the      *
* corresponding part of the geometry from the related observation point.    *
* The different possible color values and there meanings are defined in     *
* rndr_lib.h:                                                               *
* RNDR_VISMAP_EMPTY_COLOR: A pixel which doesn't correspond to any part of  *
*     the geometry and should therefore be ignored.                         *
* RNDR_VISMAP_MAPPED_COLOR: A pixel which isn't visible from the related    *
*     observation point.                                                    *
* RNDR_VISMAP_VISIBLE_COLOR: A pixel which is visible from the related      *
*     observation point.                                                    *
* RNDR_VISMAP_TANGENT_COLOR: A pixel which fails the                        *
*     SolvingParams.VisMapTanAng requirement.                               *
* RNDR_VISMAP_POOR_AR_COLOR: A pixel which fails the                        *
*     SolvingParams.VisMapCriticAR requirement.                             *
* RNDR_VISMAP_DEGEN_COLOR: A pixel which corresponds to a degenerated       *
*     triangle in the geometry.                                             *
* RNDR_VISMAP_NOT_RENDERED_COLOR: A pixel outside the field view of the     *
*     observation point.                                                    *
*                                                                           *
* UserGCLoadVisMap: This function load a visbility map from disk            *
*     according to enumerated index of the observation points (See          *
*     UserGCSolutionIndexStruct.Index).                                     *
* The rest of the functions expose inner functionalities and using them     *
* requires defining USER_GC_EXPOSE_INNER_FUNCTIONALITIES.                   *
* UserGCExposeCreatePrspMatrix: Create perspective matrix for the given     *
*     observation point.						    *
* UserGCExposeCreateViewMatrix, UserGCExposeCreateViewMatrix2: Create       *
*     view matrix for the given observation point.  			    *
* UserGCExposeDivideAndCreateViewMatrices: When ObsPtsGroups -> XYAngle is  *
*     above 125 degrees few visibilty maps are created and combined to      *
*     create the final visibility map. This function divide the observation *
*     point toward this purpose and return the number of visibilty maps and *
*     the view matrices required to create the observatoin point corresponds*
*     to each of those visibility maps.                                     *
* UserGCExposeDivideOP: The same but return observation points instead of   *
*     matrices.                                                             *
* UserGCExposePrepareScene: Does the UV transformation on GeoObj and        *
*     Obstacles and nothing more.                                           *
* UserGCExposeInterpretOPGroupsSuggestion: Does the interpretation of the   *
*     suggestions and nothing more.                                         *
****************************************************************************/

#ifndef IRIT_USER_GEOM_COVER_H
#define IRIT_USER_GEOM_COVER_H

#include "inc_irit/iritprsr.h"

typedef enum UserGCPredefinedSuggestionsType {
    USER_GC_CUSTOM_SUGGESTION, /* No predefined suggestion. */
    USER_GC_SIX_MAIN_AXIS,
    USER_GC_SPHERE_POLYHEDRON,        
    USER_GC_SPHERE_COVER_4,
    USER_GC_SPHERE_COVER_20,
    USER_GC_SPHERE_COVER_50,
    USER_GC_SPHERE_COVER_100,
    USER_GC_SPHERE_COVER_130,
    USER_GC_PERIMETER,
    USER_GC_SURFACE_GRID
} UserGCPredefinedSuggestionsType;

typedef struct UserGCPredefinedObsPtSuggestionsStruct {
    UserGCPredefinedSuggestionsType PredefinedSuggestionType;
    int AddAntipodal;
    union {
        struct {
            /* Polyhedron made of polygons. Used as a base to divide and     */
            /* create more vertices to be observation points.                */
            IPObjectStruct *Poly; 
            int Level;             /* Number of times to divid each polygon. */
        }  Polyhedron;
        struct { 
            /* See UserGCSetSuggestionPointsFromPerimeter */
            const char *CrfFileName;
            int GuardsNumber;          /* Number of guards on the perimeter. */
            IrtRType GuardsHeight;  /* The height of the guard above the     */
                                    /* surface.                              */
        } Perimeter;
        struct {
            /* See UserGCSetSuggestionPointsFromSurfaceGrid */
            const char *SrfFileName;
            int GuardsNumber[2];          /* Number of guards on U and on V. */
            IrtRType GuardsHeight;  /* The height of the guard above the     */
                                    /* surface.                              */
        } SurfaceGrid;
    };
} UserGCPreDefinedObsPtSuggestionsStruct;

typedef struct UserGCObsPtTypeStruct {
    IrtRType ZAngle;                      /* Angle in degrees in the Z axis. */
    IrtRType XYAngle;                   /* Angle in degrees in the XY plane. */
} UserGCObsPtTypeStruct;

typedef struct UserGCObsPtSuggestionStruct {
    IrtPtType ObsPt; /* If it's USER_GC_INF_VEC then the ObsPt is at         */
                     /* infinity.                                            */
    IrtVecType Direction;
} UserGCObsPtSuggestionStruct;

typedef struct UserGCObsPtGroupTypeStruct {
    UserGCObsPtTypeStruct ObsPtType;
    UserGCPreDefinedObsPtSuggestionsStruct PredefinedSuggestions;
    UserGCObsPtSuggestionStruct **Suggestions;
} UserGCObsPtGroupTypeStruct;

typedef enum UserGCSetCoverAlgorithmType {
    USER_GC_GREEDY,
    USER_GC_EXHAUSTIVE,
    USER_GC_EXACT,
    USER_GC_ALGORITHM_NUM
} UserGCSetCoverAlgorithmType;

typedef struct UserGCSetCoverParams {
    IrtRType CoverLimit;    /* Used only for exhaustive algorithms.          */
    int SizeLimit;          /* Used only by exhaustive and exact algorithms. */
    UserGCSetCoverAlgorithmType Algorithm;
} UserGCSetCoverParams;

/* In both functions FileName is the name without the extension. */
/* The returned image must be allocated using IritMalloc.        */
typedef IrtImgPixelStruct *(*UserGCLoadRgbImageFromFileFuncType)
                                                        (const char* FileName, 
                                                         int *Width, 
                                                         int *Height);
typedef int (*UserGCSaveRgbImageToFileFuncType)(const char *FileName, 
                                                IrtImgPixelStruct *VisMap, 
                                                int Width, 
                                                int Height);

typedef struct UserGCSolvingParamsStruct {
    int VisMapWidth;
    int VisMapHeight;
    IrtRType VisMapTanAng;
    IrtRType VisMapCriticAR;
    UserGCSetCoverParams SetCoverParams;
} UserGCSolvingParamsStruct;

typedef struct UserGCDebugParamsStruct {
    /* Don't do the set cover, display all the visibility maps. */
    int DisableSetCover;
    /* The geometric object and the obstacles are going through several      */
    /* transformations such as adapting their UV parametrization and         */
    /* combining all the polygons into one polygon struct. When this flag is */
    /* TRUE, those objects after those transformation are saved to disk as   */
    /* an itd file. The objects doesn't include any scene transformations    */
    /* depended on the observation points and without the projection to the  */
    /* the screen.                                                           */
    /* Effective only if LoadObjectsFromDisk > 0 (The objects are actually   */
    /* saved regardless of this flag. This flag just tells not to erase the  */
    /* objects at the end).                                                  */
    int StoreObjectsBeforeOPs;
    /* For each observation point save the object after the transformation   */
    /* saved for StoreObjectsBeforeOPs and additional scene transfomration   */
    /* defined by the observation point and the projection to the screen.    */
    int StoreObjectsForOPs;
    /* Don't delete the visibility map from disk after creation. */
    int StoreVisMap;
    /* Create imd matrix files with the matrix used in order to calculate    */
    /* each visibility map. This doesn't include the projection              */
    /* transformations.                                                      */
    int StoreVisMapMatrix;
    /* Don't create visibility maps. Load them from disk. */
    int LoadVisMaps;
    /* Control saving/loading of geometric objects and obstacles to disk.    */
    /* (The same objects StoreObjectsBeforeOPs relates to).                  */
    /* 0 - Doesn't save the objects to disk. Keeps the originals and creates */
    /*     copies as needed.                                                 */
    /* 1 - Saves the objects. Keeps the originals and creates copies as      */
    /*     needed.                                                           */
    /* 2 - Saves the objects. Doesn't Keep the originals. Load from disk     */
    /*     for each creation of visibility maps. Reduce memory but increase  */
    /*     time (though may reduce time if it reduces swapping).             */
    /* 3 - Saves the objects. Doesn't Keep the originals. Load from disk     */
    /*     for each creation of sub visibility maps (once for each visibility*/
    /*     map in orthographic and up to 3 times in perspective). Reduce     */
    /*     memory but increases time (though may reduce time if it reduces   */
    /*     swapping).                                                        */
    /* The objects if saved, are deleted at the end unless                   */
    /* StoreObjectsBeforeOPs is TRUE.                                        */
    int LoadObjectsFromDisk;
    /* This is an input and output of UserGCSolveGeoProblem. GeoObj contains */
    /* several polygonal objects connected using Pnext. Those polygonal      */
    /* objects could possibly be originated from surface objects. GeoObjOrig */
    /* when given to UserGCSolveGeoProblem cotnains a list of objects with   */
    /* the original surfaces (or the polygonal objects if no original        */
    /* surface exists) in the same order as in GeoObj. UserGCSolveGeoProblem */
    /* will change the UV parametrization of all the surfaces (or polygonal  */
    /* objects) according to the same UV parametrization changes done to     */
    /* GeoObj.                                                               */
    IPObjectStruct *GeoObjOrig;
    /* The function to load and save images. Must be compatible to each      */
    /* other. If any of the load or save functions are NULL, they are both   */
    /* replaced by default functions for PPM image.                          */
    /* LoadImageFunc must allocate memory using IritMalloc since the memroy  */
    /* is freed internally by IritFree.                                      */
    UserGCLoadRgbImageFromFileFuncType LoadImageFunc;
    UserGCSaveRgbImageToFileFuncType SaveImageFunc;
    /* The extension of the visibility maps as saved/loaded by               */
    /* SaveImageFunc/LoadImageFunc. If the default PPM functions are used    */
    /* this field will be set to "ppm" by the application.                   */
    char VisMapExtension[4];
    /* This is a printf function to write debug messages to. If it's NULL,   */
    /* it will be replaced by inner function which doesn't write anything    */
    /* and always returns 0.                                                 */
    int (*Print)(const char *Format, ...);
} UserGCDebugParamsStruct;

typedef struct UserGCProblemDefinitionStruct {
    UserGCObsPtGroupTypeStruct **ObsPtsGroups;    /* NULL terminated vector. */
    IPObjectStruct *GeoObj;                     /* Object to compute GC for. */
    IPObjectStruct *Obstacles;
    IrtImgPixelStruct *UVMap;    /* The zeroes pixels are ignored UV values. */
    UserGCSolvingParamsStruct SolvingParams;
    UserGCDebugParamsStruct DebugParams;
} UserGCProblemDefinitionStruct;

typedef struct UserGCSolutionIndexStruct {
    int ObsPtGroupIndex,       /* The indexes of the visibility map in the   */
        SuggestionIndex,       /* groups and suggestions.                    */
        Index;       /* The index of the visibility map when looking at all  */
                     /* the visibility maps as one list (the index in which  */
                     /* it is saved to disk).	                             */
} UserGCSolutionIndexStruct;

IRIT_GLOBAL_DATA_HEADER const IrtPtType 
    USER_GC_INF_VEC;

IRIT_GLOBAL_DATA_HEADER const IrtRType 
    USER_GC_NEAR, USER_GC_FAR;

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int UserGCSolveGeoProblem(UserGCProblemDefinitionStruct *Problem,
                          UserGCSolutionIndexStruct *** SolutionOps,
                          IrtRType *CoverPercentage);
IrtImgPixelStruct *UserGCLoadVisMap(UserGCProblemDefinitionStruct *Problem, 
                                    int Index);

#define USER_GC_EXPOSE_INNER_FUNCTIONALITIES
#ifdef USER_GC_EXPOSE_INNER_FUNCTIONALITIES

void UserGCExposeCreatePrspMatrix(IrtRType ZAngle, 
                                  IrtRType XYAngle,
                                  IrtHmgnMatType PrspMat);
void UserGCExposeCreateViewMatrix(const UserGCObsPtSuggestionStruct *Op,
                                  IrtHmgnMatType ViewMat);
void UserGCExposeCreateViewMatrix2(const UserGCObsPtSuggestionStruct *Op,
                                   IrtHmgnMatType ViewMat,
                                   const IrtVecType Up);
void UserGCExposeDivideAndCreateViewMatrices(UserGCObsPtSuggestionStruct *ObsPt,
                                             IrtRType OpeningInXY, 
                                             IrtRType OpeningInZ,
                                             int *ObsPtsNum, 
                                             IrtRType *OpeningOutXY,
                                             IrtRType *OpeningOutZ,
                                             IrtHmgnMatType ViewMats[6]);
void UserGCExposeDivideOP(UserGCObsPtSuggestionStruct *ObsPt,
                          IrtRType OpeningInXY, 
                          IrtRType OpeningInZ,
                          int *ObsPtsNum,
                          UserGCObsPtSuggestionStruct *ObsPts,
                          IrtRType *OpeningOutXY,
                          IrtRType *OpeningOutZ);
int UserGCExposePrepareScene(UserGCProblemDefinitionStruct *Problem);
int UserGCExposeInterpretOPGroupsSuggestion(
                                       UserGCProblemDefinitionStruct *Problem);
#endif /* USER_GC_EXPOSE_INNER_FUNCTIONALITIES */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_USER_GEOM_COVER_H */
