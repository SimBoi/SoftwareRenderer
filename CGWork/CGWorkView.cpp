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
#include "MouseSensitivityDialog.h"
#include <string>
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
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, &CCGWorkView::OnOptionsMouseSensitivity)
	ON_COMMAND(ID_VIEW_SPACE, &CCGWorkView::OnViewSpace)
	ON_COMMAND(ID_OBJECT_SPACE, &CCGWorkView::OnObjectSpace)
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

	// enlarge or reduce parentObject when window is resized
	//parentObject.Scale(vec4(m_AspectRatio, m_AspectRatio, m_AspectRatio, m_AspectRatio));

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

void printMat(CG::mat4 mat)
{
	char debugStream[100];
	sprintf_s(debugStream, "heeeeeeeeeeeeeeeeeeeeeeeeeeeeq    printing mat:\n");
	OutputDebugStringA(debugStream);
	sprintf_s(debugStream, "heeeeeeeeeeeeeeeeeeeeeeeeeeeeq    %f  -  %f  -  %f  -  %f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
	OutputDebugStringA(debugStream);
	sprintf_s(debugStream, "heeeeeeeeeeeeeeeeeeeeeeeeeeeeq    %f  -  %f  -  %f  -  %f\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
	OutputDebugStringA(debugStream);
	sprintf_s(debugStream, "heeeeeeeeeeeeeeeeeeeeeeeeeeeeq    %f  -  %f  -  %f  -  %f\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
	OutputDebugStringA(debugStream);
	sprintf_s(debugStream, "heeeeeeeeeeeeeeeeeeeeeeeeeeeeq    %f  -  %f  -  %f  -  %f\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	OutputDebugStringA(debugStream);
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
void DrawLine(CDC* pDCToUse, CG::vec4 from, CG::vec4 to, const CG::Camera& camera, const CG::mat4& screenProjection)
{
	if (!ClipLine(from, to, camera)) return; // ClipLine will return false if line is out of frustum
	from = CG::HomogeneousToEuclidean(screenProjection * from);
	to = CG::HomogeneousToEuclidean(screenProjection * to);
	CG::MoveTo(from.x, from.y);
	CG::LineTo(pDCToUse, to.x, to.y);
}

void DrawFace(CDC* pDCToUse, const CG::Face& face, const CG::Camera& camera, const CG::mat4& modelToCameraFrame, const CG::mat4& screenProjection)
{
	if (face.vertices.size() <= 1) return;

	// draw face
	CG::Vertex prevVertex = face.vertices.back();
	CG::vec4 prevCoords = modelToCameraFrame * prevVertex.localPosition;
	for (auto const& vertex : face.vertices)
	{
		CG::vec4 coords = modelToCameraFrame * vertex.localPosition;
		DrawLine(pDCToUse, prevCoords, coords, camera, screenProjection);
		prevCoords = coords;
	}
}

int x_location = 0;
void CCGWorkView::OnDraw(CDC* pDC)
{
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	    return;
	CRect r;

	GetClientRect(&r);
	CDC *pDCToUse = /*m_pDC*/m_pDbDC;
	
	// set background color
	pDCToUse->FillSolidRect(&r, RGB(255, 255, 0));

	if (!initialized)
	{
		initialized = true;
		
		double aspectRatio = 16.0 / 9;
		//camera.Ortho(-1000 * aspectRatio, 1000 * aspectRatio, -1000, 1000, 0.1, 1000);
		camera.Perspective(90, aspectRatio, 0.1, 1000);
		camera.LookAt(CG::vec4(0, 0, 300, 1), parentObject.wPosition(), CG::vec4(0, 1, 0).normalized());
		
		parentObject.Scale(CG::vec4(50, 50, 50));

	}

	//parentObject.Translate(CG::vec4(0, 0, -1));
	//parentObject.RotateY(30);
	camera.LookAt(CG::vec4(0, 0, 300, 1), parentObject.wPosition(), CG::vec4(0, 1, 0).normalized());
	
	CG::mat4 parentToCameraFrame = camera.cInverse * parentObject.wTransform * parentObject.mTransform;
	CG::mat4 screenProjection = camera.ToScreenSpace(r.Width(), r.Height()) * camera.projection;

	int i = 0;
	for (auto &child : parentObject.children)
	{
		CG::mat4 childToCameraFrame = parentToCameraFrame * child.wTransform * child.mTransform;

		// draw child object faces
		for (auto const& face : child.faces)
		{
			DrawFace(pDCToUse, face, camera, childToCameraFrame, screenProjection);
		}

		//// draw child object bounding box
		//for (auto const& face : child.boundingBox)
		//{
		//	DrawFace(pDCToUse, r, face, finalProjection);
		//}

		i++;
	}
	
	// draw parent object bounding box
	for (auto const& face : parentObject.boundingBox)
	{
		DrawFace(pDCToUse, face, camera, parentToCameraFrame, screenProjection);
	}

	// for testing
	//const CString text = std::to_string(x_location).c_str();
	//pDC->DrawText(text, -1, &r, DT_CENTER);
	
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


void CCGWorkView::doAction(int val)
{
	if (m_nAction == ID_ACTION_ROTATE)
	{
		doRotate(val);
	}
	else if (m_nAction == ID_ACTION_TRANSLATE)
	{
		doTranslate(val);
	}
	else if (m_nAction == ID_ACTION_SCALE)
	{
		doScale(val);
	}
}


static double calcRotateValue(int val)
{
	val = val * parentObject.rotation_sensitivity;
	return val;
}

void CCGWorkView::doRotate(int val)
{
	double rotate_value = calcRotateValue(val);

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
	}
}


static double calcTranslateValue(int val)
{
	val = val * parentObject.translation_sensitivity;
	return val;
}

void CCGWorkView::doTranslate(int val)
{
	double translate_value = calcTranslateValue(val);

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
	}
}


static double calcScaleValue(int val)
{
	val = val * parentObject.scale_sensitivity;
	double s = (val >= 0) ? val : (-1.0 / val);
	return s;
}

void CCGWorkView::doScale(int val)
{
	double scale_value = calcScaleValue(val);

	if (m_nSpace == VIEW)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			parentObject.Scale(vec4(scale_value, 0, 0));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			parentObject.Scale(vec4(0, scale_value, 0));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			parentObject.Scale(vec4(0, 0, scale_value));
		}
	}
	else if (m_nSpace == OBJECT)
	{
		if (m_nAxis == ID_AXIS_X)
		{
			parentObject.LocalScale(vec4(scale_value, 0, 0));
		}
		else if (m_nAxis == ID_AXIS_Y)
		{
			parentObject.LocalScale(vec4(0, scale_value, 0));
		}
		else if (m_nAxis == ID_AXIS_Z)
		{
			parentObject.LocalScale(vec4(0, 0, scale_value));
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


void CCGWorkView::OnViewSpace()
{
	m_nSpace = VIEW;
}


void CCGWorkView::OnObjectSpace()
{
	m_nSpace = OBJECT;
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


void CCGWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);
	int current_x_position = point.x;
	int value = (current_x_position - old_x_position) > 0 ? 1 : -1;
	if (nFlags == MK_LBUTTON)
	{
		// ONLY The left mouse button is down.
		// parent transformations		
		doAction(value);

		x_location = point.x;
	}
	else if ((nFlags == (MK_LBUTTON | MK_CONTROL)) || (nFlags == MK_RBUTTON))
	{
		// The left mouse button and the CTRL key are down,
		// OR The right mouse button is down.
		x_location = point.x * -1;
		// child transformation
		// add selection mechanism
	}

	old_x_position = current_x_position;
	Invalidate();
	UpdateWindow();
}

void CCGWorkView::OnOptionsMouseSensitivity()
{
	MouseSensitivityDialog dialog;
	dialog.m_translation_slider = parentObject.translation_sensitivity;
	dialog.m_rotation_slider = parentObject.rotation_sensitivity;
	dialog.m_scale_slider = parentObject.scale_sensitivity;
	if (dialog.DoModal() == IDOK)
	{
		parentObject.translation_sensitivity = dialog.m_translation_slider;
		parentObject.rotation_sensitivity = dialog.m_rotation_slider;
		parentObject.scale_sensitivity = dialog.m_scale_slider;
	}
}


