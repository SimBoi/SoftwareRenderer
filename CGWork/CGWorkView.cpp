// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "stdafx.h"
#include "CGWork.h"

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"


// For Status Bar access
#include "MainFrm.h"

// our own MoveTo and LineTo implementation
#include "ColorPickerDialog.h"
#include "MouseSensitivityDialog.h"
#include "PolygonalFineNessDialog.h"
#include "SelectObjectDialog.h"
#include "PerspectiveSettingsDialog.h"
#include "BackgroundImageDialog.h"
#include "RenderToFileDialog.h"
#include "AnimationPlayerDialog.h"

#include "CG_Animation.h"
#include <string>
#include <unordered_map>
#include <list>


using namespace CG;

double objectSize;

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView


IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_AXIS_XY, OnAxisXY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_XY, OnUpdateAxisXY)
	ON_COMMAND(ID_AXIS_XYZ, OnAxisXYZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_XYZ, OnUpdateAxisXYZ)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, OnOptionsMouseSensitivity)
	ON_COMMAND(ID_VIEW_SPACE, OnViewSpace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPACE, OnUpdateViewSpace)
	ON_COMMAND(ID_OBJECT_SPACE, OnObjectSpace)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_SPACE, OnUpdateObjectSpace)
	ON_COMMAND(ID_FACE_NORMALS, OnFaceNormals)
	ON_UPDATE_COMMAND_UI(ID_FACE_NORMALS, OnUpdateFaceNormals)
	ON_COMMAND(ID_VERTEX_NORMALS, OnVertexNormals)
	ON_UPDATE_COMMAND_UI(ID_VERTEX_NORMALS, OnUpdateVertexNormals)
	ON_COMMAND(ID_OPTIONS_COLORPICKER, OnOptionsColorpicker)
	ON_COMMAND(ID_OPTIONS_POLYGONALFINENESS, OnOptionsPolygonalFineness)
	ON_COMMAND(ID_OPTIONS_SELECTOBJECT, &CCGWorkView::OnOptionsSelectObject)
	ON_COMMAND(ID_PERSPECCTIVE_SETTINGS, OnPerspecctiveSettings)
	ON_COMMAND(ID_OPTIONS_BACKGROUNDIMAGE, &CCGWorkView::OnOptionsBackgroundImage)
	ON_COMMAND(ID_RENDERMODE_SOLID, &CCGWorkView::OnRendermodeSolid)
	ON_UPDATE_COMMAND_UI(ID_RENDERMODE_SOLID, &CCGWorkView::OnUpdateRendermodeSolid)
	ON_COMMAND(ID_RENDERMODE_WIREFRAME, &CCGWorkView::OnRendermodeWireframe)
	ON_UPDATE_COMMAND_UI(ID_RENDERMODE_WIREFRAME, &CCGWorkView::OnUpdateRendermodeWireframe)
	ON_COMMAND(ID_RENDER_ONSCREEN, &CCGWorkView::OnRenderOnscreen)
	ON_COMMAND(ID_RENDER_TOFILE, &CCGWorkView::OnRenderTofile)
	ON_UPDATE_COMMAND_UI(ID_RENDER_TOFILE, &CCGWorkView::OnUpdateRenderTofile)
//	ON_UPDATE_COMMAND_UI(ID_RENDER_ONSCREEN, &CCGWorkView::OnUpdateRenderOnscreen)
ON_UPDATE_COMMAND_UI(ID_RENDER_ONSCREEN, &CCGWorkView::OnUpdateRenderOnscreen)
ON_COMMAND(ID_FILE_SAVEASPNG, &CCGWorkView::OnFileSaveaspng)
ON_COMMAND(ID_LIGHT_SHADING_PHONG, &CCGWorkView::OnLightShadingPhong)
ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_PHONG, &CCGWorkView::OnUpdateLightShadingPhong)
ON_COMMAND(ID_VIEW_BACKFACECULLING, &CCGWorkView::OnViewBackfaceculling)
ON_UPDATE_COMMAND_UI(ID_VIEW_BACKFACECULLING, &CCGWorkView::OnUpdateViewBackfaceculling)
ON_COMMAND(ID_VIEW_INVERTNORMALS, &CCGWorkView::OnViewInvertnormals)
ON_UPDATE_COMMAND_UI(ID_VIEW_INVERTNORMALS, &CCGWorkView::OnUpdateViewInvertnormals)
ON_COMMAND(ID_VIEW_ALWAYSCALCULATEVERTICESNORMALS, &CCGWorkView::OnViewAlwayscalculateverticesnormals)
ON_UPDATE_COMMAND_UI(ID_VIEW_ALWAYSCALCULATEVERTICESNORMALS, &CCGWorkView::OnUpdateViewAlwayscalculateverticesnormals)
ON_COMMAND(ID_VIEW_SILHOUETTEHIGHLIGHTING, &CCGWorkView::ToggleSilhouette)
ON_UPDATE_COMMAND_UI(ID_VIEW_SILHOUETTEHIGHLIGHTING, &CCGWorkView::OnUpdateToggleSilhouette)
ON_COMMAND(ID_RECORD_BUTTON, &CCGWorkView::OnRecordButton)
ON_COMMAND(ID_PLAY_BUTTON, &CCGWorkView::OnPlayButton)
ON_COMMAND(ID_VIEW_RECORDINGBAR, &CCGWorkView::OnViewRecordingbar)
ON_UPDATE_COMMAND_UI(ID_VIEW_RECORDINGBAR, &CCGWorkView::OnUpdateViewRecordingbar)
ON_UPDATE_COMMAND_UI(ID_RECORD_BUTTON, &CCGWorkView::OnUpdateRecordButton)
ON_COMMAND(ID_STOP_RECORDING_BUTTON, &CCGWorkView::OnStopRecordingButton)
ON_UPDATE_COMMAND_UI(ID_STOP_RECORDING_BUTTON, &CCGWorkView::OnUpdateStopRecordingButton)
ON_COMMAND(ID_SNAPSHOT_BUTTON, &CCGWorkView::OnSnapshotButton)
ON_UPDATE_COMMAND_UI(ID_SNAPSHOT_BUTTON, &CCGWorkView::OnUpdateSnapshotButton)
ON_COMMAND(ID_SAVE_RECORD_BUTTON, &CCGWorkView::OnSaveRecordButton)
ON_UPDATE_COMMAND_UI(ID_SAVE_RECORD_BUTTON, &CCGWorkView::OnUpdateSaveRecordButton)
ON_COMMAND(ID_DISCARD_RECORD_BUTTON, &CCGWorkView::OnDiscardRecordButton)
ON_UPDATE_COMMAND_UI(ID_DISCARD_RECORD_BUTTON, &CCGWorkView::OnUpdateDiscardRecordButton)
ON_UPDATE_COMMAND_UI(ID_PLAY_BUTTON, &CCGWorkView::OnUpdatePlayButton)
ON_COMMAND(ID_PAUSE_BUTTON, &CCGWorkView::OnPauseButton)
ON_UPDATE_COMMAND_UI(ID_PAUSE_BUTTON, &CCGWorkView::OnUpdatePauseButton)
ON_COMMAND(ID_NEXT_FRAME_BUTTON, &CCGWorkView::OnNextFrameButton)
ON_UPDATE_COMMAND_UI(ID_NEXT_FRAME_BUTTON, &CCGWorkView::OnUpdateNextFrameButton)
ON_COMMAND(ID_RESET_PLAYER_BUTTON, &CCGWorkView::OnResetPlayerButton)
ON_UPDATE_COMMAND_UI(ID_RESET_PLAYER_BUTTON, &CCGWorkView::OnUpdateResetPlayerButton)
ON_WM_LBUTTONUP()
ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
        GLUquadric *quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluCylinder(quad, radius, 0.0, height, 20, 20);
        gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView()
{
	// Set default values
	m_nAxis = ID_AXIS_X;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	m_nSpace = VIEW;
	m_drawFaceNormals = false;
	m_drawVertexNormals = false;
	m_normalFlip = 1;
	m_alwaysCalcNormals = false;
	m_renderMode = WIREFRAME;
	m_backFaceCulling = true;
	m_renderSilhouette = false;

	old_x_position = 0;
	old_y_position = 0;

	// default colors
	setDefaultColors();
	// background image
	IsBackgroundImageLoaded = false;
	BackgroundImageLayout = NONE;
	SetDefaultPerspectiveSettings();

	m_bRenderOnScreen = true;
	// render to file
	m_bRenderToPngFile = false;
	m_pRenderToPng = nullptr;

	// animation record
	m_pTempRecord = nullptr;
	last_toched_object = nullptr;
	m_nRecordingStatus = EMPTY;
	m_pRecord = nullptr;
	m_pPlayer = nullptr;
	play_in_separate_thread = false;

	m_nLightShading = FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;
	m_ambientLight.ambientIntensity = 0.2;
	m_cosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled = true;
	m_lights[LIGHT_ID_1].dirZ = -1;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;
}

CCGWorkView::~CCGWorkView()
{

}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}



int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	return 0;
}


// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);

	if ( NULL == m_pDC ) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	SetTimer(1, 1, NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers


void CCGWorkView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if ( 0 >= cx || 0 >= cy ) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth/(GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap);
    	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{
    return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}





BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC)
{
	// Windows will clear the window with the background color every time your window
	// is redrawn, and then CGWork will clear the viewport with its own background color.


	return true;
}


static Object* getObjectByIndex(int index)
{
	if (parentObject.children.empty())
		return nullptr;

	// Initialize iterator to objects list
	std::list<Object>::iterator it = parentObject.children.begin();

	// Move the iterator by object_index elements
	advance(it, index);

	// return the object
	return &(*it);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////

// returns false if line is out of frustum, points should be relative to camera frame (before applying projection)
bool ClipLine(vec4& from, vec4& to, const Camera& camera)
{
	if (camera.IsInsideFrustum(from))
	{
		if (camera.IsInsideFrustum(to)) return true; // both ends of the line are inside the frustum, no need to clip the line

		Line line = Line(from, to); // t(from) = 0, t(to) = 1
		double t;
		for (int i = 0; i < 6; i++)
		{
			vec4 intersection;
			if (!camera.clipPlanes[i].Intersection(line, t, intersection)) continue;
			if (t > 0 && t < 1)
			{
				to = intersection;
				break;
			}
		}
	}
	else if (camera.IsInsideFrustum(to))
	{
		Line line = Line(from, to); // t(from) = 0, t(to) = 1
		double t;
		for (int i = 0; i < 6; i++)
		{
			vec4 intersection;
			if (!camera.clipPlanes[i].Intersection(line, t, intersection)) continue;
			if (t > 0 && t < 1)
			{
				from = intersection;
				break;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool ClipEdge(Edge& edge, const Camera& camera)
{
	vec4 p1 = edge.line.p1, p2 = edge.line.p2;
	if (!ClipLine(p1, p2, camera)) return false;
	Line line = Line(p1, p2);

	double a1, a2;
	if (edge.line.p1.x != edge.line.p2.x)
	{
		a1 = (p1.x - edge.line.p1.x) / (edge.line.p2.x - edge.line.p1.x);
		a2 = (p2.x - edge.line.p1.x) / (edge.line.p2.x - edge.line.p1.x);
	}
	else if (edge.line.p1.y != edge.line.p2.y)
	{
		a1 = (p1.y - edge.line.p1.y) / (edge.line.p2.y - edge.line.p1.y);
		a2 = (p2.y - edge.line.p1.y) / (edge.line.p2.y - edge.line.p1.y);
	}
	else
	{
		a1 = (p1.z - edge.line.p1.z) / (edge.line.p2.z - edge.line.p1.z);
		a2 = (p2.z - edge.line.p1.z) / (edge.line.p2.z - edge.line.p1.z);
	}

	edge.startNormal = (edge.startNormal * (1 - a1) + edge.endNormal * a1).normalized();
	edge.endNormal = (edge.startNormal * (1 - a2) + edge.endNormal * a2).normalized();
	edge.line = line;
	return true;
}

// renders line between two points in camera frame
void DrawLine(CDC* pDCToUse, vec4 from, vec4 to, const Camera& camera, const mat4& screenProjection, const COLORREF& color)
{
	if (!ClipLine(from, to, camera)) return; // ClipLine will return false if line is out of frustum
	from = screenProjection * from;
	to = screenProjection * to;
	from.RoundXY();
	from.RoundXY();

	MoveTo(from.x, from.y, from.z);
	LineTo(pDCToUse, to.x, to.y, to.z, color);
}

// renders line between two points in camera frame
void DrawThickLine(CDC* pDCToUse, vec4 from, vec4 to, const Camera& camera, const mat4& screenProjection, const COLORREF& color)
{
	if (!ClipLine(from, to, camera)) return; // ClipLine will return false if line is out of frustum
	from = screenProjection * from;
	to = screenProjection * to;
	from.RoundXY();
	from.RoundXY();

	MoveTo(from.x, from.y, from.z);
	LineTo(pDCToUse, to.x, to.y, to.z, color);

	MoveTo(from.x + 1, from.y, from.z);
	LineTo(pDCToUse, to.x + 1, to.y, to.z, color);
	MoveTo(from.x - 1, from.y, from.z);
	LineTo(pDCToUse, to.x - 1, to.y, to.z, color);
	MoveTo(from.x, from.y + 1, from.z);
	LineTo(pDCToUse, to.x, to.y + 1, to.z, color);
	MoveTo(from.x, from.y - 1, from.z);
	LineTo(pDCToUse, to.x, to.y - 1, to.z, color);

	MoveTo(from.x + 1, from.y + 1, from.z);
	LineTo(pDCToUse, to.x + 1, to.y + 1, to.z, color);
	MoveTo(from.x - 1, from.y - 1, from.z);
	LineTo(pDCToUse, to.x - 1, to.y - 1, to.z, color);
	MoveTo(from.x - 1, from.y + 1, from.z);
	LineTo(pDCToUse, to.x - 1, to.y + 1, to.z, color);
	MoveTo(from.x + 1, from.y - 1, from.z);
	LineTo(pDCToUse, to.x + 1, to.y - 1, to.z, color);
}

void CCGWorkView::MapShadows(const Face& face, const Camera perspective, int mapResolution, const mat4& finalProjection, const mat4& modelToLightFrame, ZBuffer& buffer)
{
	// clip face edges
	std::list<Line> edges;
	vec4 currCoords, prevCoords = modelToLightFrame * face.vertices.back().localPosition;
	for (auto const& vertex : face.vertices)
	{
		currCoords = modelToLightFrame * vertex.localPosition;
		vec4 from = prevCoords;
		vec4 to = currCoords;
		if (ClipLine(from, to, perspective)) edges.push_back(Line(from, to));
		prevCoords = currCoords;
	}

	if (edges.size() == 0) return;

	// connect edges outside of the frustum
	Line* prevEdge = &edges.back();
	for (auto it = edges.begin(); it != edges.end(); it++)
	{
		if (prevEdge->p2 != it->p1) edges.insert(it, Line(prevEdge->p2, it->p1));
		prevEdge = &(*it);
	}

	// projection
	for (auto& edge : edges)
	{
		vec4 projectedP1 = finalProjection * edge.p1;
		vec4 projectedP2 = finalProjection * edge.p2;
		projectedP1.RoundXY();
		projectedP2.RoundXY();
		edge = Line(projectedP1, projectedP2);
	}

	ShadowScanConversion(mapResolution, mapResolution, edges, buffer);
}

void CCGWorkView::DrawFace(
	CDC* pDCToUse,
	RenderMode renderMode,
	const Face& face,
	bool drawFaceNormal,
	bool drawVertexNormal,
	const Camera& camera,
	const mat4& modelToCameraFrame,
	const mat4& projectionToModelFrame,
	const mat4& cameraToGlobalFrame,
	const mat4& cameraToModelFrame,
	const mat4& cameraToModelFrameTranspose,
	const mat4& modelToGlobalFrame,
	const mat4& globalToModelFrameTranspose,
	const mat4& screenProjection,
	const COLORREF& color,
	const COLORREF& faceNormalColor,
	const COLORREF& vertexNormalColor)
{
	if (face.vertices.size() <= 2) return;

	// clip face edges
	std::list<Edge> edges;
	vec4 currCoords, prevCoords = modelToCameraFrame * face.vertices.back().localPosition;
	vec4 currNormal, prevNormal = face.vertices.back().normal;
	for (auto const& vertex : face.vertices)
	{
		currCoords = modelToCameraFrame * vertex.localPosition;
		currNormal = vertex.normal;
		Edge edge = Edge(Line(prevCoords, currCoords), prevNormal, currNormal);
		if (ClipEdge(edge, camera)) edges.push_back(edge);
		prevCoords = currCoords;
		prevNormal = currNormal;
	}

	if (edges.size() == 0) return;

	// connect edges outside of the frustum
	Edge* prevEdge = &edges.back();
	for (auto it = edges.begin(); it != edges.end(); it++)
	{
		if (prevEdge->line.p2 != it->line.p1)
		{
			edges.insert(it, Edge(Line(prevEdge->line.p2, it->line.p1), prevEdge->endNormal, it->startNormal));
		}
		prevEdge = &(*it);
	}

	// scan edges
	std::list<ScanEdge> scanEdges;
	for (auto& edge : edges)
	{
		// projection
		vec4 projectedP1 = screenProjection * edge.line.p1;
		vec4 projectedP2 = screenProjection * edge.line.p2;
		projectedP1.RoundXY();
		projectedP2.RoundXY();

		// global
		vec4 globalP1 = cameraToGlobalFrame * edge.line.p1;
		vec4 globalP2 = cameraToGlobalFrame * edge.line.p2;
		vec4 globalStartNormal = globalToModelFrameTranspose * edge.startNormal;
		vec4 globalEndNormal = globalToModelFrameTranspose * edge.endNormal;

		scanEdges.push_back(ScanEdge(edge.line, Line(projectedP1, projectedP2), Edge(Line(globalP1, globalP2), globalStartNormal, globalEndNormal)));
	}

	if (renderMode == SOLID)
	{
		// render solid face
		DrawScanConversion(
			pDCToUse,
			m_WindowHeight,
			m_WindowWidth,
			scanEdges,
			projectionToModelFrame,
			cameraToGlobalFrame,
			modelToGlobalFrame,
			globalToModelFrameTranspose,
			modelToGlobalFrame * face.center,
			globalToModelFrameTranspose * face.normal,
			m_normalFlip,
			color,
			m_ambientLight,
			m_lights,
			m_cosineFactor,
			m_nLightShading
		);
	}
	else
	{
		// render wireframe
		for (auto const& edge : scanEdges)
		{
			MoveTo(edge.projected.p1.x, edge.projected.p1.y, edge.projected.p1.z);
			LineTo(pDCToUse, edge.projected.p2.x, edge.projected.p2.y, edge.projected.p2.z, color);
		}
	}

	// draw Silhouette
	if (m_renderSilhouette)
	{
		auto it = face.adjacentFaces.begin();
		for (Line line : face.edges)
		{
			Face* adjacentFace = *it;
			bool isSilhouette = false;

			if (adjacentFace == NULL)
			{
				isSilhouette = true;
			}
			else if (m_nView == ID_VIEW_PERSPECTIVE)
			{
				vec4 cameraModelPos = cameraToModelFrame * vec4(0, 0, 0, 1);
				vec4 faceViewDirection = face.center - cameraModelPos;
				double faceDirection = vec4::dot(faceViewDirection, face.normal * m_normalFlip);
				vec4 adjacentFaceViewDirection = adjacentFace->center - cameraModelPos;
				double adjacentFaceDirection = vec4::dot(adjacentFaceViewDirection, adjacentFace->normal * m_normalFlip);
				if (faceDirection * adjacentFaceDirection <= 0) isSilhouette = true;
			}
			else
			{
				vec4 faceNormalCameraFrame = cameraToModelFrameTranspose * face.normal * m_normalFlip;
				double faceDirection = vec4::dot(faceNormalCameraFrame, vec4(0, 0, -1, 0));
				vec4 adjacentFaceNormalCameraFrame = cameraToModelFrameTranspose * adjacentFace->normal * m_normalFlip;
				double adjacentFaceDirection = vec4::dot(adjacentFaceNormalCameraFrame, vec4(0, 0, -1, 0));
				if (faceDirection * adjacentFaceDirection <= 0) isSilhouette = true;
			}

			if (isSilhouette)
			{
				vec4 from = modelToCameraFrame * line.p1;
				vec4 to = modelToCameraFrame * line.p2;
				DrawThickLine(pDCToUse, from, to, camera, screenProjection, vertexNormalColor);
			}

			it++;
		}
	}

	// draw face normals
	if (drawFaceNormal)
	{
		vec4 from = modelToCameraFrame * face.center;
		vec4 to = modelToCameraFrame * (face.center + face.normal);
		DrawLine(pDCToUse, from, to, camera, screenProjection, faceNormalColor);
	}

	// draw vertex normals
	if (drawVertexNormal)
	{
		for (auto const& vertex : face.vertices)
		{
			vec4 from = modelToCameraFrame * vertex.localPosition;
			vec4 to = modelToCameraFrame * (vertex.localPosition + vertex.normal);
			DrawLine(pDCToUse, from, to, camera, screenProjection, vertexNormalColor);
		}
	}
}

void CCGWorkView::CalculateVertexNormals()
{
	std::unordered_map<vec4, std::list<Face*>, vec4Hash> incidentFaces;
	for (auto& child : parentObject.children)
	{
		for (auto& face : child.faces)
		{
			for (auto& vertex : face.vertices)
			{
				vec4 key = coordsKey(vertex.localPosition, objectSize);
				incidentFaces[key].push_back(&face);
			}
		}
	}

	for (auto& child : parentObject.children)
	{
		for (auto& face : child.faces)
		{
			for (auto& vertex : face.vertices)
			{
				vec4 key = coordsKey(vertex.localPosition, objectSize);
				incidentFaces[key].push_back(&face);
			}
		}
	}

	vec4 zeroVector = vec4();
	for (auto& child : parentObject.children)
	{
		for (auto& face : child.faces)
		{
			for (auto& vertex : face.vertices)
			{
				if (m_alwaysCalcNormals == false && vertex.normal != zeroVector) continue;

				vec4 key = coordsKey(vertex.localPosition, objectSize);
				if (incidentFaces.count(key) == 0) continue;
				for (Face* incidentFace : incidentFaces[key])
				{
					vertex.normal += incidentFace->normal;
				}
				vertex.normal = (vertex.normal / incidentFaces[key].size()).normalized();
			}
		}
	}
}

void CCGWorkView::FindEdgeAdjacentFaces()
{
	std::unordered_map<Line, std::list<Face*>, LineHash> edgeAdjacentFaces;

	for (auto& child : parentObject.children)
	{
		for (auto& face : child.faces)
		{
			for (auto& line : face.edges)
			{
				Line key = lineKey(line, objectSize);
				edgeAdjacentFaces[key].push_back(&face);
			}
		}
	}

	for (auto& child : parentObject.children)
	{
		for (auto& face : child.faces)
		{
			for (auto& line : face.edges)
			{
				Line key = lineKey(line, objectSize);
				bool foundFace = false;
				for (auto adjacentFace : edgeAdjacentFaces[key])
				{
					if (&face != adjacentFace)
					{
						foundFace = true;
						face.adjacentFaces.push_back(adjacentFace);
						break;
					}
				}
				if (!foundFace) face.adjacentFaces.push_back(NULL);
			}
		}
	}
}

void CCGWorkView::InitializeView()
{
	object_index = 0;
	selectedObject = getObjectByIndex(object_index);

	// animation record
	m_nRecordingStatus = EMPTY;
	delete m_pRecord;
	m_pRecord = nullptr;
	delete m_pPlayer;
	m_pPlayer = nullptr;
	last_toched_object = &parentObject;

	objectSize = max(parentObject.maxX - parentObject.minX, parentObject.maxY - parentObject.minY);
	objectSize = max(objectSize, parentObject.maxZ - parentObject.minZ);
	double scale = 400 / objectSize;
	double normalScale = 0.05 * objectSize;
 	parentObject.Scale(vec4(scale, scale, scale));

	CalculateVertexNormals();

	// initialize face edges
	for (auto& child : parentObject.children)
	{
		for (auto& face : child.faces)
		{
			vec4 currCoords, prevCoords = face.vertices.back().localPosition;
			for (auto& vertex : face.vertices)
			{
				currCoords = vertex.localPosition;
				face.edges.push_back(Line(prevCoords, currCoords));
				prevCoords = currCoords;
			}
		}
	}

	FindEdgeAdjacentFaces();

	// set scale for normals
	for (auto& child : parentObject.children)
	{
		for (auto& face : child.faces)
		{
			face.normal = face.normal * normalScale;
			for (auto& vertex : face.vertices)
			{
				vertex.normal = vertex.normal * normalScale;
			}
		}
	}

	// check if all the polygons are convex
	for (auto& child : parentObject.children)
		for (auto& face : child.faces)
			if (!IsConvex(face)) throw;

	// set camera position and orientation
	camera.LookAt(vec4(0, 0, 600, 1), parentObject.wPosition(), vec4(0, 1, 0).normalized());

	// initialize light sources
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		if (m_lights[i].enabled)
		{
			m_lights[i].CalculatePerspective();
		}
	}
}

void CCGWorkView::DrawScene(CRect& SceneRect, CDC* pDCToUse, int SceneWidth, int SceneHeight, double SceneAspectRatio, RenderMode renderMode)
{
	// draw background
	CG::DrawBackground(SceneRect, pDCToUse);

	// initialize the view
	if (!initialized)
	{
		initialized = true;
		InitializeView();
	}

	// reset zBuffer
	zBuffer.resize(SceneHeight, SceneWidth);
	zBuffer.init();

	// commpute shadow map
	if (renderMode == SOLID)
	{
		for (int i = 0; i < MAX_LIGHT; i++)
		{
			if (m_lights[i].enabled)
			{
				if (m_lights[i].shadowType == SHADOW_TYPE_MAP)
				{
					if (m_lights[i].type == LIGHT_TYPE_DIRECTIONAL)
					{
						m_lights[i].directionalBuffer.resize(m_lights[i].shadowMapResolution, m_lights[i].shadowMapResolution);
						m_lights[i].directionalBuffer.init();

						mat4 parentToLightFrame = m_lights[i].directionalPerspective.cInverse * parentObject.wTransform * parentObject.mTransform;
						for (auto& child : parentObject.children)
						{
							mat4 childToLightFrame = parentToLightFrame * child.wTransform * child.mTransform;
							for (auto const& face : child.faces)
							{
								MapShadows(
									face,
									m_lights[i].directionalPerspective,
									m_lights[i].shadowMapResolution,
									m_lights[i].directionalFinalProjection,
									childToLightFrame,
									m_lights[i].directionalBuffer);
							}
						}
					}
					else
					{
						for (int j = 0; j < 6; j++)
						{
							m_lights[i].pointBuffer[j].resize(m_lights[i].cubeMapSideResolution, m_lights[i].cubeMapSideResolution);
							m_lights[i].pointBuffer[j].init();

							mat4 parentToLightFrame = m_lights[i].pointPerspective[j].cInverse * parentObject.wTransform * parentObject.mTransform;
							for (auto& child : parentObject.children)
							{
								mat4 childToLightFrame = parentToLightFrame * child.wTransform * child.mTransform;
								for (auto const& face : child.faces)
								{
									MapShadows(
										face,
										m_lights[i].pointPerspective[j],
										m_lights[i].cubeMapSideResolution,
										m_lights[i].pointFinalProjection[j],
										childToLightFrame,
										m_lights[i].pointBuffer[j]);
								}
							}
						}
					}
				}
			}
		}
	}

	// Orthographic or Perspective View
	if (m_bIsPerspective) camera.Perspective(fovY, SceneAspectRatio, zNear, zFar);
	else camera.Ortho(-800 * SceneAspectRatio, 800 * SceneAspectRatio, -800, 800, zNear, zFar);

	// Parent to/from Frames
	mat4 globalToParentFrame = parentObject.mInverse * parentObject.wInverse;
	mat4 parentToGlobalFrame = parentObject.wTransform * parentObject.mTransform;
	mat4 parentToCameraFrame = camera.cInverse * parentToGlobalFrame;
	mat4 screenProjection = camera.ToScreenSpace(SceneRect.Width(), SceneRect.Height()) * camera.projection;

	// render to screen
	int i = 0;
	for (auto& child : parentObject.children)
	{
		mat4 globalToChildFrame = child.mInverse * child.wInverse * globalToParentFrame;
		mat4 globalToChildFrameTranspose = mat4::Transpose(globalToChildFrame);
		mat4 childToParentFrame = child.wTransform * child.mTransform;
		mat4 childToGlobalFrame = parentToGlobalFrame * childToParentFrame;
		mat4 childToCameraFrame = parentToCameraFrame * childToParentFrame;
		mat4 cameraToChildFrame = globalToChildFrame * camera.cTransform;
		mat4 cameraToChildFrameTranspose = mat4::Transpose(cameraToChildFrame);
		mat4 projectionToChildFrame = mat4::InverseTransform(screenProjection * childToCameraFrame);
		COLORREF child_color = (bIsModelColor ? ModelColor : child.color);

		// draw child object faces
		for (auto const& face : child.faces)
		{
			if (m_backFaceCulling)
			{
				// back face culling
				if (m_nView == ID_VIEW_PERSPECTIVE)
				{
					vec4 cameraModelPos = cameraToChildFrame * vec4(0, 0, 0, 1);
					vec4 viewDirection = face.center - cameraModelPos;
					if (vec4::dot(viewDirection, face.normal * m_normalFlip) >= 0) continue;
				}
				else
				{
					vec4 faceNormalCameraFrame = cameraToChildFrameTranspose * face.normal * m_normalFlip;
					if (vec4::dot(faceNormalCameraFrame, vec4(0, 0, -1, 0)) >= 0) continue;
				}
			}

			DrawFace(
				pDCToUse,
				renderMode,
				face,
				m_drawFaceNormals,
				m_drawVertexNormals,
				camera,
				childToCameraFrame,
				projectionToChildFrame,
				camera.cTransform,
				cameraToChildFrame,
				cameraToChildFrameTranspose,
				childToGlobalFrame,
				globalToChildFrameTranspose,
				screenProjection,
				child_color,
				FaceNormalColor,
				VertexNormalColor
			);
		}

		//// draw child object bounding box
		//for (auto const& face : child.boundingBox)
		//{
		//	DrawFace(pDCToUse, face, false, false, camera, childToCameraFrame, screenProjection, child_color, FaceNormalColor, VertexNormalColor);
		//}

		i++;
	}

	//// draw parent object bounding box
	//for (auto const& face : parentObject.boundingBox)
	//{
	//	DrawFace(pDCToUse, face, false, false, camera, parentToCameraFrame, screenProjection, BoundingBoxColor, FaceNormalColor, VertexNormalColor);
	//}
}

void CCGWorkView::OnDraw(CDC* pDC)
{
	if (m_nRecordingStatus == PLAYING)
		return;

	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (m_bRenderOnScreen && m_bRenderToPngFile
		&& m_pRenderToPng->GetWidth() == m_WindowWidth
		&& m_pRenderToPng->GetHeight() == m_WindowHeight)
	{
		// in case of rendering on screen and to file of the same size
		// no need to recalculate the scene
		CDC* pDCImage = RenderOnScreen(m_renderMode);
		WriteDCToPngFile(pDCImage, m_pRenderToPng, m_WindowWidth, m_WindowHeight);
	}
	else
	{
		if (m_bRenderOnScreen)
		{
			RenderOnScreen(m_renderMode);
		}

		if (m_bRenderToPngFile)
		{
			RenderToPngFile(m_pRenderToPng, m_renderMode);
		}
	}

}

CDC* CCGWorkView::RenderOnScreen(RenderMode renderMode)
{
	CRect r;
	GetClientRect(&r);
	CDC* pDCToUse = /*m_pDC*/m_pDbDC;

	DrawScene(r, pDCToUse, m_WindowWidth, m_WindowHeight, m_AspectRatio, renderMode);

	if (pDCToUse != m_pDC)
	{
		m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);
	}

	return pDCToUse;
}

void CCGWorkView::RenderToPngFile(PngWrapper* png_file, RenderMode renderMode)
{
	if (png_file == nullptr)
		return;

	int img_width = png_file->GetWidth();
	int img_height = png_file->GetHeight();

	// image rect
	CRect img_r(0, 0, img_width, img_height);

	// image Device Context
	CDC* pDCToUse = new CDC();
	pDCToUse->CreateCompatibleDC(m_pDC);
	//SetTimer(1, 1, NULL);
	HBITMAP pImgBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, img_r.right, img_r.bottom);
	pDCToUse->SelectObject(pImgBitMap);

	// compute the aspect ratio
	// this will keep all dimension scales equal
	double ImgAspectRatio = (GLdouble)(img_width) / (GLdouble)(img_height);

	DrawScene(img_r, pDCToUse, img_width, img_height, ImgAspectRatio, renderMode);

	WriteDCToPngFile(pDCToUse, png_file, img_r.Width(), img_r.Height());

	delete pDCToUse;
	pDCToUse = nullptr;
}

void CCGWorkView::WriteDCToPngFile(const CDC* pDCImage, PngWrapper* png_file, int width, int height)
{
	if (png_file == nullptr)
		return;

	if (!png_file->InitWritePng())
		return;

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			int png_val = ColorRefToPngVal(pDCImage->GetPixel(x, y));
			png_file->SetValue(x, y, png_val);
		}
	}

	png_file->WritePng();
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy()
{
	CView::OnDestroy();

	// delete the DC
	if ( m_pDC ) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}

	if (m_pRenderToPng) {
		delete m_pRenderToPng;
		m_pRenderToPng = nullptr;
	}

	if (m_pRecord) {
		delete m_pRecord;
		m_pRecord = nullptr;
	}

	if (m_pPlayer) {
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
}



/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoad()
{
	TCHAR szFilters[] = _T ("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,szFilters);

	if (dlg.DoModal () == IDOK) {
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		// Open the file and read it.
		// Your code here...

		Invalidate();	// force a WM_PAINT for drawing.
	}

}


void CCGWorkView::doAction(int x_val, int y_val, Object& object)
{
	if (m_nAction == ID_ACTION_ROTATE)
	{
		doRotate(x_val, y_val, object);
	}
	else if (m_nAction == ID_ACTION_TRANSLATE)
	{
		doTranslate(x_val, y_val, object);
	}
	else if (m_nAction == ID_ACTION_SCALE)
	{
		doScale(x_val, y_val, object);
	}

	// update last transformed object ptr
	last_toched_object = &object;
}


static double calcRotateValue(int val)
{
	return (val * rotation_sensitivity);
}

void CCGWorkView::doRotate(int x_val, int y_val, Object& object)
{
	double rotate_value = calcRotateValue(x_val);

	if (m_nSpace == VIEW)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			object.RotateX(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			object.RotateY(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			object.RotateZ(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double rotate_y_value = calcRotateValue(y_val);
			object.RotateX(rotate_value);
			object.RotateY(rotate_y_value);
		}
	}
	else if (m_nSpace == OBJECT)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			object.LocalRotateX(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			object.LocalRotateY(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			object.LocalRotateZ(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double rotate_y_value = calcRotateValue(y_val);
			object.LocalRotateX(rotate_value);
			object.LocalRotateY(rotate_y_value);
		}
	}
}


static double calcTranslateValue(int val)
{
	return (val * translation_sensitivity);
}

void CCGWorkView::doTranslate(int x_val, int y_val, Object& object)
{
	double translate_value = calcTranslateValue(x_val);

	if (m_nSpace == VIEW)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			object.Translate(vec4(translate_value, 0, 0));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			object.Translate(vec4(0, translate_value, 0));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			object.Translate(vec4(0, 0, translate_value));
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double translate_y_value = calcTranslateValue(y_val);
			object.Translate(vec4(translate_value, translate_y_value, 0));
		}
	}
	else if (m_nSpace == OBJECT)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			object.LocalTranslate(vec4(translate_value, 0, 0));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			object.LocalTranslate(vec4(0, translate_value, 0));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			object.LocalTranslate(vec4(0, 0, translate_value));
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double translate_y_value = calcTranslateValue(y_val);
			object.LocalTranslate(vec4(translate_value, translate_y_value, 0));
		}
	}
}


static double calcScaleValue(int val)
{
	if (val == 0)
		return 1;

	double s = val * scale_sensitivity * 0.05;
	return ((val >= 0) ? s : (-1.0 / s));
}

void CCGWorkView::doScale(int x_val, int y_val, Object& object)
{
	double scale_value = calcScaleValue(x_val);

	if (m_nSpace == VIEW)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			object.Scale(vec4(scale_value, 1, 1));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			object.Scale(vec4(1, scale_value, 1));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			object.Scale(vec4(1, 1, scale_value));
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double scale_y_value = calcScaleValue(y_val);
			object.Scale(vec4(scale_value, scale_y_value, 1));
		}
		else if (m_nAxis == ID_AXIS_XYZ)
		{
			object.Scale(vec4(scale_value, scale_value, scale_value));
		}
	}
	else if (m_nSpace == OBJECT)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			object.LocalScale(vec4(scale_value, 1, 1));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			object.LocalScale(vec4(1, scale_value, 1));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			object.LocalScale(vec4(1, 1, scale_value));
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double scale_y_value = calcScaleValue(y_val);
			object.LocalScale(vec4(scale_value, scale_y_value, 1));
		}
		else if (m_nAxis == ID_AXIS_XYZ)
		{
			object.LocalScale(vec4(scale_value, scale_value, scale_value));
		}
	}
}



// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewRecordingbar()
{
	// TODO: Add your command handler code here
	CToolBar& RecordingBar = ((CMainFrame*)GetParentFrame())->getRecordingBar();
	if (RecordingBar.IsWindowVisible())
	{
		RecordingBar.ShowWindow(SW_HIDE);
	}
	else
	{
		RecordingBar.ShowWindow(SW_NORMAL);
	}

	Invalidate();
	UpdateWindow();
}

void CCGWorkView::OnUpdateViewRecordingbar(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CToolBar& RecordingBar = ((CMainFrame*)GetParentFrame())->getRecordingBar();
	pCmdUI->SetCheck(RecordingBar.IsWindowVisible());
}


void CCGWorkView::OnViewOrthographic()
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective()
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}


void CCGWorkView::OnViewBackfaceculling()
{
	m_backFaceCulling = !m_backFaceCulling;
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewBackfaceculling(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_backFaceCulling);
}


void CCGWorkView::OnViewInvertnormals()
{
	m_normalFlip = -1 * m_normalFlip;
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewInvertnormals(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_normalFlip == -1);
}


void CCGWorkView::OnViewAlwayscalculateverticesnormals()
{
	m_alwaysCalcNormals = !m_alwaysCalcNormals;
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewAlwayscalculateverticesnormals(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_alwaysCalcNormals);
}

void CCGWorkView::ToggleSilhouette()
{
	m_renderSilhouette = !m_renderSilhouette;
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateToggleSilhouette(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_renderSilhouette == true);
}


// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate()
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate()
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale()
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the
// selected axis.
void CCGWorkView::OnAxisX()
{
	m_nAxis = ID_AXIS_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_X);
}

void CCGWorkView::OnAxisY()
{
	m_nAxis = ID_AXIS_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Y);
}

void CCGWorkView::OnAxisZ()
{
	m_nAxis = ID_AXIS_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Z);
}

void CCGWorkView::OnAxisXY()
{
	m_nAxis = ID_AXIS_XY;
}

void CCGWorkView::OnUpdateAxisXY(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_XY);
}

void CCGWorkView::OnAxisXYZ()
{
	m_nAxis = ID_AXIS_XYZ;
}

void CCGWorkView::OnUpdateAxisXYZ(CCmdUI* pCmdUI)
{
	// uniform XYZ for scale only
	if ((m_nAxis == ID_AXIS_XYZ) && (m_nAction != ID_ACTION_SCALE))
	{
		m_nAxis = ID_AXIS_X;
	}
	pCmdUI->Enable(m_nAction == ID_ACTION_SCALE);

	pCmdUI->SetCheck(m_nAxis == ID_AXIS_XYZ);
}

void CCGWorkView::OnViewSpace()
{
	m_nSpace = VIEW;
}

void CCGWorkView::OnUpdateViewSpace(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nSpace == VIEW);
}

void CCGWorkView::OnObjectSpace()
{
	m_nSpace = OBJECT;
}

void CCGWorkView::OnUpdateObjectSpace(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nSpace == OBJECT);
}

void CCGWorkView::OnFaceNormals()
{
	m_drawFaceNormals = !m_drawFaceNormals;
}

void CCGWorkView::OnUpdateFaceNormals(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_drawFaceNormals);
}

void CCGWorkView::OnVertexNormals()
{
	m_drawVertexNormals = !m_drawVertexNormals;
}

void CCGWorkView::OnUpdateVertexNormals(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_drawVertexNormals);
}



// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat()
{
	m_nLightShading = FLAT;
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == FLAT);
}


void CCGWorkView::OnLightShadingGouraud()
{
	m_nLightShading = GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == GOURAUD);
}


void CCGWorkView::OnLightShadingPhong()
{
	m_nLightShading = PHONG;
}

void CCGWorkView::OnUpdateLightShadingPhong(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == PHONG);
}

// RENDER HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnRendermodeSolid()
{
	m_renderMode = SOLID;
	Invalidate();		// redraw using the new view.
}


void CCGWorkView::OnUpdateRendermodeSolid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_renderMode == SOLID);
}


void CCGWorkView::OnRendermodeWireframe()
{
	m_renderMode = WIREFRAME;
	Invalidate();		// redraw using the new view.
}


void CCGWorkView::OnUpdateRendermodeWireframe(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_renderMode == WIREFRAME);
}


void CCGWorkView::OnRenderOnscreen()
{
	m_bRenderOnScreen = !m_bRenderOnScreen;
}


void CCGWorkView::OnUpdateRenderOnscreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bRenderOnScreen);
}


void CCGWorkView::OnRenderTofile()
{
	RenderToFileDialog dialog;
	dialog.m_FilePathString = (m_pRenderToPng != nullptr) ? m_strPngFileName : "";
	dialog.m_ImageWidth = (m_pRenderToPng != nullptr) ? m_pRenderToPng->GetWidth() : m_WindowWidth;
	dialog.m_ImageHeight = (m_pRenderToPng != nullptr) ? m_pRenderToPng->GetHeight() : m_WindowHeight;
	dialog.m_WindowWidth = m_WindowWidth;
	dialog.m_WindowHeight = m_WindowHeight;

	INT_PTR answer = dialog.DoModal();
	if (answer == IDOK)
	{
		// free memory old render to file
		delete m_pRenderToPng;
		m_pRenderToPng = nullptr;

		// init new render to file
		m_strPngFileName = dialog.m_FilePathString;
		m_pRenderToPng = new PngWrapper(m_strPngFileName, dialog.m_ImageWidth, dialog.m_ImageHeight);
		m_bRenderToPngFile = (m_pRenderToPng != nullptr);
	}
	else if (answer == IDCANCEL_RENDERTO_FILE)
	{
		delete m_pRenderToPng;
		m_pRenderToPng = nullptr;
		m_bRenderToPngFile = false;
	}
	Invalidate();

}


void CCGWorkView::OnUpdateRenderTofile(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bRenderToPngFile);
}


// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants()
{
	CLightDialog dlg;

	for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	{
	    dlg.SetLightData((LightID)id, &m_lights[id]);
	}
	dlg.SetLightData(LIGHT_ID_AMBIENT, &m_ambientLight);
	dlg.SetCosineFactor(m_cosineFactor);
	dlg.SetDynamicRange(dynamicRange);

	if (dlg.DoModal() == IDOK)
	{
	    for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	    {
			m_lights[id] = *dlg.GetLightData((LightID)id);
	    }
	    m_ambientLight = *dlg.GetLightData(LIGHT_ID_AMBIENT);
		m_cosineFactor = dlg.GetCosineFactor();
		dynamicRange = dlg.GetDynamicRange();
		for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
		{
			m_lights[id].CalculatePerspective();
		}
	}
	Invalidate();
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
}

static int getMouseDirection(int old_position, int current_position)
{
	const int epsilon = 5;
	int diff = (current_position - old_position);
	if (diff >= -epsilon && diff <= epsilon)
		return 0;
	return (diff > 0) ? 1 : -1;
}


void CCGWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_nRecordingStatus == PLAYING)
		return;

	CView::OnMouseMove(nFlags, point);
	int current_x_position = point.x;
	int current_y_position = point.y;

	int x_value = getMouseDirection(old_x_position, current_x_position);
	int y_value = -1 * getMouseDirection(old_y_position, current_y_position);

	if (nFlags == MK_LBUTTON)
	{
		// ONLY The left mouse button is down.
		// parent transformations

		doAction(x_value, y_value, parentObject);
	}
	else if ((nFlags == (MK_LBUTTON | MK_CONTROL)) || (nFlags == MK_RBUTTON))
	{
		// The left mouse button and the CTRL key are down,
		// OR The right mouse button is down.
		// child transformation
		if (selectedObject != nullptr)
		{
			doAction(x_value, y_value, *selectedObject);
		}
	}

	old_x_position = current_x_position;
	old_y_position = current_y_position;
	Invalidate();
	//UpdateWindow();
}

// OPTIONS HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnOptionsMouseSensitivity()
{
	MouseSensitivityDialog dialog;
	dialog.m_translation_slider = translation_sensitivity;
	dialog.m_rotation_slider = rotation_sensitivity;
	dialog.m_scale_slider = scale_sensitivity;
	if (dialog.DoModal() == IDOK)
	{
		translation_sensitivity = dialog.m_translation_slider;
		rotation_sensitivity = dialog.m_rotation_slider;
		scale_sensitivity = dialog.m_scale_slider;
	}
	Invalidate();
}


void CCGWorkView::OnOptionsColorpicker()
{
	ColorPickerDialog dialog;
	dialog.DoModal();
	Invalidate();
}


void CCGWorkView::OnOptionsPolygonalFineness()
{
	PolygonalFineNessDialog dialog;
	if (dialog.DoModal() == IDOK)
	{
		polygonal_fineness = dialog.m_polygonal_fineness;
	}
	Invalidate();
}


void CCGWorkView::OnOptionsSelectObject()
{
	SelectObjectDialog dialog;
	if (dialog.DoModal() == IDOK)
	{
		object_index = dialog.selected_index;
		selectedObject = getObjectByIndex(object_index);
	}
	Invalidate();
}

void CCGWorkView::OnPerspecctiveSettings()
{
	PerspectiveSettingsDialog dialog;
	dialog.m_zNear = zNear;
	dialog.m_zFar = zFar;
	dialog.m_Fov = fovY;
	if (dialog.DoModal() == IDOK)
	{
		zNear = dialog.m_zNear;
		zFar = dialog.m_zFar;
		fovY = dialog.m_Fov;
	}
	Invalidate();
}


void CCGWorkView::OnOptionsBackgroundImage()
{
	BackgroundImageDialog dialog;
	dialog.DoModal();
	Invalidate();
}




void CCGWorkView::OnFileSaveaspng()
{
	RenderToFileDialog dialog;
	dialog.m_FilePathString = "";
	dialog.m_ImageWidth = m_WindowWidth;
	dialog.m_ImageHeight = m_WindowHeight;
	dialog.m_WindowWidth = m_WindowWidth;
	dialog.m_WindowHeight = m_WindowHeight;

	INT_PTR answer = dialog.DoModal();
	if (answer == IDOK)
	{
		// init saveto png file
		CStringA saveto_str = dialog.m_FilePathString;
		PngWrapper* saveto_file = new PngWrapper(saveto_str, dialog.m_ImageWidth, dialog.m_ImageHeight);

		// render the scence to saveto png file
		RenderToPngFile(saveto_file, SOLID);

		delete saveto_file;
		saveto_file = nullptr;
	}
	Invalidate();
}


void CCGWorkView::OnRecordButton()
{
	// TODO: Add your command handler code here
	if (m_nRecordingStatus == EMPTY || m_pRecord == nullptr)
	{
		delete m_pRecord;
		m_pRecord = new AnimationRecord(&parentObject);
	}
	else
	{
		//m_pRecord->fillHistoryBuffers(&parentObject); ???

		// capture all changes
		m_pRecord->pushAllChanges();
	}
	m_nRecordingStatus = INPROGRESS;

}


void CCGWorkView::OnUpdateRecordButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == EMPTY || m_nRecordingStatus == STOPPED);
}


void CCGWorkView::OnStopRecordingButton()
{
	// TODO: Add your command handler code here
	m_nRecordingStatus = STOPPED;
}


void CCGWorkView::OnUpdateStopRecordingButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == INPROGRESS);
}


void CCGWorkView::OnSnapshotButton()
{
	// TODO: Add your command handler code here
	
	// capture all changes
	m_pRecord->pushAllChanges();
}


void CCGWorkView::OnUpdateSnapshotButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == INPROGRESS || m_nRecordingStatus == STOPPED);
}


void CCGWorkView::OnSaveRecordButton()
{
	// TODO: Add your command handler code here
}


void CCGWorkView::OnUpdateSaveRecordButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == STOPPED);
}


void CCGWorkView::OnDiscardRecordButton()
{
	// TODO: Add your command handler code here

	int answer = AfxMessageBox(
		_T("Discard the Animation Record?"), 
		MB_YESNO | MB_ICONWARNING);

	if (answer == IDYES)
	{
		m_nRecordingStatus = EMPTY;
		delete m_pRecord;
		m_pRecord = nullptr;
	}
}


void CCGWorkView::OnUpdateDiscardRecordButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == STOPPED);
}


void CCGWorkView::OnPlayButton()
{
	// TODO: Add your command handler code here

	if (m_nRecordingStatus == STOPPED || m_pPlayer == nullptr)
	{
		delete m_pPlayer;
		AnimationPlayerDialog dialog;
		
		if (dialog.DoModal() == IDOK && m_pRecord != nullptr)
		{
			saveCurrentTransformations();

			// initialize player
			m_pPlayer = new AnimationPlayer(*m_pRecord, 
				dialog.m_step, 
				dialog.m_render_mode, 
				dialog.m_speed, 
				dialog.m_rewind);

			play_in_separate_thread = dialog.m_bSeparateThread;
		}
	}
	m_nRecordingStatus = PLAYING;

	if (play_in_separate_thread)
	{
		// operatePlayer in new thread
		AfxBeginThread(operatePlayer, this);
	}
	else
	{
		// operatePlayer on same thread
		operatePlayer();
	}
	
}


void CCGWorkView::OnUpdatePlayButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == STOPPED || m_nRecordingStatus == PAUSED);
}


void CCGWorkView::OnPauseButton()
{
	// TODO: Add your command handler code here
	m_nRecordingStatus = PAUSED;
}


void CCGWorkView::OnUpdatePauseButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == PLAYING);
}


void CCGWorkView::OnNextFrameButton()
{
	// TODO: Add your command handler code here
	if (!m_pPlayer->nextFrame())
	{
		// record player ended
		endPlayer();
	}
	else
	{
		RenderCurrentFrame();

		//CString strPage; !!!
		//strPage.Format(_T("progress: %f"), m_pPlayer->getProgressPercentage());
	}
}


void CCGWorkView::OnUpdateNextFrameButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == PAUSED);
}


void CCGWorkView::OnResetPlayerButton()
{
	// TODO: Add your command handler code here
	endPlayer();
}


void CCGWorkView::OnUpdateResetPlayerButton(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_nRecordingStatus == PLAYING || m_nRecordingStatus == PAUSED);
}


void CCGWorkView::saveCurrentTransformations()
{
	delete m_pTempRecord;
	m_pTempRecord = new AnimationRecord(&parentObject);
}


void CCGWorkView::restoreSavedTransformations()
{
	if (m_pTempRecord == nullptr)
		return;

	m_pTempRecord->restoreInitialHistory();

	delete m_pTempRecord;
	m_pTempRecord = nullptr;
}


void CCGWorkView::endPlayer(bool update_gui)
{
	m_nRecordingStatus = STOPPED;
	if (update_gui)
		STATUS_BAR_TEXT(_T("Player Ended"));

	delete m_pPlayer;
	m_pPlayer = nullptr;

	restoreSavedTransformations();
	const DWORD wait = 500;
	Sleep(wait);
	Invalidate();
	if (update_gui)
		STATUS_BAR_TEXT(_T(""));
}


void CCGWorkView::RenderCurrentFrame(bool update_gui)
{
	if (m_pPlayer == nullptr)
		return;

	RenderOnScreen(m_pPlayer->playing_render_mode);

	CString frame_index;
	frame_index.Format(_T("frame %ld of %ld"), m_pPlayer->getCurrentFrameIndex(), m_pPlayer->getTotalFramesNum());
	if (update_gui)
		STATUS_BAR_TEXT(frame_index);
}


UINT CCGWorkView::operatePlayer(LPVOID p)

{
	CCGWorkView* me = (CCGWorkView*)p;
	me->operatePlayer(false);
	return 0;
}


void CCGWorkView::operatePlayer(bool update_gui)
{
	if (m_pPlayer == nullptr || m_nRecordingStatus != PLAYING)
		return;

	RenderCurrentFrame(update_gui);

	if (m_pPlayer->speed == 0)
	{
		// pause player, use next-frame button.
		m_nRecordingStatus = PAUSED;
		return;
	}

	const int DEFAULT_MILLISECOND = 400;
	DWORD wait_time = DWORD((1.0 / m_pPlayer->speed) * DEFAULT_MILLISECOND);

	while (m_nRecordingStatus == PLAYING && m_pPlayer->nextFrame())
	{
		RenderCurrentFrame(update_gui);
		Sleep(wait_time);
	}

	// record player ended
	if (m_nRecordingStatus == PLAYING)
		endPlayer(update_gui);
}


void CCGWorkView::RecordCurrentFrame()
{
	// if recording in progress, capture current frame
	if (m_nRecordingStatus == INPROGRESS && m_pRecord != nullptr)
	{
		m_pRecord->captureFrame(last_toched_object, m_nSpace);
		STATUS_BAR_TEXT(_T("frame recorded."));
	}
}

void CCGWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
	RecordCurrentFrame();
}


void CCGWorkView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnRButtonUp(nFlags, point);
	RecordCurrentFrame();
}
