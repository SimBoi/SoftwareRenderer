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
#include "CG_Line.h"
#include "CG_Matrix.h"
#include "CG_Object.h"
#include "ColorPickerDialog.h"
#include "MouseSensitivityDialog.h"
#include "PolygonalFineNessDialog.h"

#include <string>
#include <unordered_map>
#include "PerspectiveSettingsDialog.h"

using namespace CG;

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
	ON_COMMAND(ID_PERSPECCTIVE_SETTINGS, OnPerspecctiveSettings)
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

	old_x_position = 0;
	old_y_position = 0;

	// default colors
	setDefaultColors();
	SetDefaultPerspectiveSettings();

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled=true;
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

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////

// returns false if line is out of frustum
bool ClipLine(CG::vec4& from, CG::vec4& to, const CG::Camera& camera)
{
	if (camera.IsInsideFrustum(from))
	{
		if (camera.IsInsideFrustum(to)) return true; // both ends of the line are inside the frustum, no need to clip the line

		CG::Line line = CG::Line(from, to); // t(from) = 0, t(to) = 1
		double t;
		for (int i = 0; i < 6; i++)
		{
			CG::vec4 intersection = camera.clipPlanes[i].Intersection(line, t);
			if (t > 0 && t < 1)
			{
				to = intersection;
				break;
			}
		}
	}
	else if (camera.IsInsideFrustum(to))
	{
		CG::Line line = CG::Line(from, to); // t(from) = 0, t(to) = 1
		double t;
		for (int i = 0; i < 6; i++)
		{
			CG::vec4 intersection = camera.clipPlanes[i].Intersection(line, t);
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

// renders line between two points in camera frame
void DrawLine(CDC* pDCToUse, CG::vec4 from, CG::vec4 to, const CG::Camera& camera, const CG::mat4& screenProjection, const COLORREF& color)
{
	if (!ClipLine(from, to, camera)) return; // ClipLine will return false if line is out of frustum
	from = screenProjection * from;
	to = screenProjection * to;
	
	CG::MoveTo(from.x, from.y);
	CG::LineTo(pDCToUse, to.x, to.y, color);
}

void DrawFaceNormal(CDC* pDCToUse, const CG::Face& face, const CG::Camera& camera, const CG::mat4& modelToCameraFrame, const CG::mat4& screenProjection, const COLORREF& color)
{
	vec4 from = modelToCameraFrame * face.center;
	vec4 to = modelToCameraFrame * (face.center + face.normal);
	DrawLine(pDCToUse, from, to, camera, screenProjection, color);
}

void DrawVertexNormal(CDC* pDCToUse, const CG::Vertex& vertex, const CG::Camera& camera, const CG::mat4& modelToCameraFrame, const CG::mat4& screenProjection, const COLORREF& color)
{
	vec4 from = modelToCameraFrame * vertex.localPosition;
	vec4 to = modelToCameraFrame * (vertex.localPosition + vertex.normal);
	DrawLine(pDCToUse, from, to, camera, screenProjection, color);
}

void DrawFace(CDC* pDCToUse, const CG::Face& face, bool drawFaceNormal, bool drawVertexNormal, const CG::Camera& camera, const CG::mat4& modelToCameraFrame, const CG::mat4& screenProjection, const COLORREF& color, const COLORREF& faceNormalColor, const COLORREF& vertexNormalColor)
{
	if (face.vertices.size() <= 1) return;

	// draw face
	CG::Vertex prevVertex = face.vertices.back();
	CG::vec4 prevCoords = modelToCameraFrame * prevVertex.localPosition;
	for (auto const& vertex : face.vertices)
	{
		CG::vec4 coords = modelToCameraFrame * vertex.localPosition;
		DrawLine(pDCToUse, prevCoords, coords, camera, screenProjection, color);
		if (drawVertexNormal) DrawVertexNormal(pDCToUse, vertex, camera, modelToCameraFrame, screenProjection, vertexNormalColor);
		prevCoords = coords;
	}

	if (drawFaceNormal) DrawFaceNormal(pDCToUse, face, camera, modelToCameraFrame, screenProjection, faceNormalColor);
}

vec4 coordsKey(vec4& coords, double range, double precision)
{
	vec4 key = coords / range; 
	key.x = std::round(key.x / precision) * precision;
	key.y = std::round(key.y / precision) * precision;
	key.z = std::round(key.z / precision) * precision;
	key.w = 1;
	return key;
}

void InitializeView()
{
	double objectSize = max(parentObject.maxX - parentObject.minX, parentObject.maxY - parentObject.minY);
	objectSize = max(objectSize, parentObject.maxZ - parentObject.minZ);
	double scale = 400 / objectSize;
	double normalScale = 0.05 * objectSize;
 	parentObject.Scale(CG::vec4(scale, scale, scale));

	// calculate missing vertex normals
	double precision = 0.001;
	std::unordered_map<vec4, std::list<Face*>, vec4Hash> incidentFaces;
	for (auto& child : parentObject.children)
	{
		for (auto& face : child.faces)
		{
			for (auto& vertex : face.vertices)
			{
				vec4 key = coordsKey(vertex.localPosition, objectSize, precision);
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
				vec4 key = coordsKey(vertex.localPosition, objectSize, precision);
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
				if (vertex.normal != zeroVector) continue;
				vec4 key = coordsKey(vertex.localPosition, objectSize, precision);
				if (incidentFaces.count(key) == 0) continue;
				for (Face* incidentFace : incidentFaces[key])
				{
					vertex.normal += incidentFace->normal;
				}
				vertex.normal = (vertex.normal / incidentFaces[key].size()).normalized();
			}
		}
	}

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

	camera.LookAt(CG::vec4(0, 0, 600, 1), parentObject.wPosition(), CG::vec4(0, 1, 0).normalized());
}

void CCGWorkView::OnDraw(CDC* pDC)
{
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect r;

	GetClientRect(&r);
	CDC* pDCToUse = /*m_pDC*/m_pDbDC;

	// set background color
	pDCToUse->FillSolidRect(&r, BackgroundColor);

	if (!initialized)
	{
		initialized = true;
		InitializeView();
	}

	if (m_bIsPerspective) camera.Perspective(fovY, m_AspectRatio, zNear, zFar);
	else camera.Ortho(-800 * m_AspectRatio, 800 * m_AspectRatio, -800, 800, zNear, zFar);

	CG::mat4 parentToCameraFrame = camera.cInverse * parentObject.wTransform * parentObject.mTransform;
	CG::mat4 screenProjection = camera.ToScreenSpace(r.Width(), r.Height()) * camera.projection;

	int i = 0;
	for (auto& child : parentObject.children)
	{
		CG::mat4 childToCameraFrame = parentToCameraFrame * child.wTransform * child.mTransform;
		COLORREF child_color = (bIsModelColor ? ModelColor : child.color);

		// draw child object faces
		for (auto const& face : child.faces)
		{
			DrawFace(pDCToUse, face, m_drawFaceNormals, m_drawVertexNormals, camera, childToCameraFrame, screenProjection, child_color, FaceNormalColor, VertexNormalColor);
		}

		// draw child object bounding box
		for (auto const& face : child.boundingBox)
		{
			DrawFace(pDCToUse, face, false, false, camera, childToCameraFrame, screenProjection, child_color, FaceNormalColor, VertexNormalColor);
		}

		i++;
	}

	//// draw parent object bounding box
	//for (auto const& face : parentObject.boundingBox)
	//{
	//	DrawFace(pDCToUse, face, false, false, camera, parentToCameraFrame, screenProjection, BoundingBoxColor, FaceNormalColor, VertexNormalColor);
	//}

	if (pDCToUse != m_pDC)
	{
		m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);
	}
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


void CCGWorkView::doAction(int x_val, int y_val)
{
	if (m_nAction == ID_ACTION_ROTATE)
	{
		doRotate(x_val, y_val);
	}
	else if (m_nAction == ID_ACTION_TRANSLATE)
	{
		doTranslate(x_val, y_val);
	}
	else if (m_nAction == ID_ACTION_SCALE)
	{
		doScale(x_val, y_val);
	}
}


static double calcRotateValue(int val)
{
	return (val * rotation_sensitivity);
}

void CCGWorkView::doRotate(int x_val, int y_val)
{
	double rotate_value = calcRotateValue(x_val);

	if (m_nSpace == VIEW)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			parentObject.RotateX(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			parentObject.RotateY(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			parentObject.RotateZ(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double rotate_y_value = calcRotateValue(y_val);
			parentObject.RotateX(rotate_value);
			parentObject.RotateY(rotate_y_value);
		}
	}
	else if (m_nSpace == OBJECT)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			parentObject.LocalRotateX(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			parentObject.LocalRotateY(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			parentObject.LocalRotateZ(rotate_value);
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double rotate_y_value = calcRotateValue(y_val);
			parentObject.LocalRotateX(rotate_value);
			parentObject.LocalRotateY(rotate_y_value);
		}
	}
}


static double calcTranslateValue(int val)
{
	return (val * translation_sensitivity);
}

void CCGWorkView::doTranslate(int x_val, int y_val)
{
	double translate_value = calcTranslateValue(x_val);

	if (m_nSpace == VIEW)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			parentObject.Translate(vec4(translate_value, 0, 0));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			parentObject.Translate(vec4(0, translate_value, 0));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			parentObject.Translate(vec4(0, 0, translate_value));
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double translate_y_value = calcTranslateValue(y_val);
			parentObject.Translate(vec4(translate_value, translate_y_value, 0));
		}
	}
	else if (m_nSpace == OBJECT)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			parentObject.LocalTranslate(vec4(translate_value, 0, 0));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			parentObject.LocalTranslate(vec4(0, translate_value, 0));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			parentObject.LocalTranslate(vec4(0, 0, translate_value));
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double translate_y_value = calcTranslateValue(y_val);
			parentObject.LocalTranslate(vec4(translate_value, translate_y_value, 0));
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

void CCGWorkView::doScale(int x_val, int y_val)
{
	double scale_value = calcScaleValue(x_val);

	if (m_nSpace == VIEW)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			parentObject.Scale(vec4(scale_value, 1, 1));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			parentObject.Scale(vec4(1, scale_value, 1));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			parentObject.Scale(vec4(1, 1, scale_value));
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double scale_y_value = calcScaleValue(y_val);
			parentObject.Scale(vec4(scale_value, scale_y_value, 1));
		}
		else if (m_nAxis == ID_AXIS_XYZ)
		{
			parentObject.Scale(vec4(scale_value, scale_value, scale_value));
		}
	}
	else if (m_nSpace == OBJECT)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			parentObject.LocalScale(vec4(scale_value, 1, 1));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			parentObject.LocalScale(vec4(1, scale_value, 1));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			parentObject.LocalScale(vec4(1, 1, scale_value));
		}
		else if (m_nAxis == ID_AXIS_XY)
		{
			double scale_y_value = calcScaleValue(y_val);
			parentObject.LocalScale(vec4(scale_value, scale_y_value, 1));
		}
		else if (m_nAxis == ID_AXIS_XYZ)
		{
			parentObject.LocalScale(vec4(scale_value, scale_value, scale_value));
		}
	}
}



// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

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

// OPTIONS HANDLERS ///////////////////////////////////////////




// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat() 
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}


void CCGWorkView::OnLightShadingGouraud() 
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants() 
{
	CLightDialog dlg;

	for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	{	    
	    dlg.SetDialogData((LightID)id,m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT,m_ambientLight);

	if (dlg.DoModal() == IDOK) 
	{
	    for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	    {
		m_lights[id] = dlg.GetDialogData((LightID)id);
	    }
	    m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
	}	
	Invalidate();
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
	if (nIDEvent == 1)
		Invalidate();
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

	CView::OnMouseMove(nFlags, point);
	int current_x_position = point.x;
	int current_y_position = point.y;

	int x_value = getMouseDirection(old_x_position, current_x_position);
	int y_value = -1 * getMouseDirection(old_y_position, current_y_position);

	if (nFlags == MK_LBUTTON)
	{
		// ONLY The left mouse button is down.
		// parent transformations		
		doAction(x_value, y_value);
	}
	else if ((nFlags == (MK_LBUTTON | MK_CONTROL)) || (nFlags == MK_RBUTTON))
	{
		// The left mouse button and the CTRL key are down,
		// OR The right mouse button is down.
		// child transformation
		// add selection mechanism
	}

	old_x_position = current_x_position;
	old_y_position = current_y_position;
	Invalidate();
	//UpdateWindow();
}

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
	//CColorDialog dialog(CG::ModelColor);
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
