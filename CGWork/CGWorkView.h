// CGWorkView.h : interface of the CCGWorkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
#define AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "gl\gl.h"    // Include the standard CGWork  headers
#include "gl\glu.h"   // Add the utility library

#include "PngWrapper.h"
#include "Light.h"
#include "CG_Matrix.h"
#include "CG_Object.h"
#include "CG_Draw.h"


class CCGWorkView : public CView
{
protected: // create from serialization only
	CCGWorkView();
	DECLARE_DYNCREATE(CCGWorkView)

// Attributes
public:
	CCGWorkDoc* GetDocument();

// Operations
public:

private:
	int m_nAxis;				// Axis of Action, X Y or Z, XY, XYZ
	int m_nAction;				// Rotate, Translate, Scale
	int m_nView;				// Orthographic, perspective
	bool m_bIsPerspective;		// is the view perspective
	bool m_drawFaceNormals;		// draw face normals or not
	bool m_drawVertexNormals;	// draw vertex normals or not
	CG::TSpace m_nSpace;		// view/ object space
	int m_normalFlip;			// set to -1 to flip normals, otherwise set to 1
	bool m_alwaysCalcNormals;	// calculate vertex normals instead of using the provided normals
	CG::RenderMode m_renderMode;	// rendering: wireframe, solid
	bool m_backFaceCulling;
	
	CString m_strItdFileName;		// file name of IRIT data

	CG::Shading m_nLightShading;			// shading: Flat, Gouraud, Phong.

	double m_lMaterialAmbient;		// The Ambient in the scene
	double m_lMaterialDiffuse;		// The Diffuse in the scene
	double m_lMaterialSpecular;		// The Specular in the scene
	int m_nMaterialCosineFactor;		// The cosine factor for the specular

	LightParams m_lights[MAX_LIGHT];	//configurable lights array
	LightParams m_ambientLight;		//ambient light (only RGB is used)


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGWorkView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCGWorkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitializeCGWork();
	BOOL SetupViewingFrustum(void);
	BOOL SetupViewingOrthoConstAspect(void);

	virtual void RenderScene();


	HGLRC    m_hRC;			// holds the Rendering Context
	CDC*     m_pDC;			// holds the Device Context
	int m_WindowWidth;		// hold the windows width
	int m_WindowHeight;		// hold the windows height
	double m_AspectRatio;		// hold the fixed Aspect Ration
	int old_x_position;		// to save the old mouse position
	int old_y_position;		// to save the old mouse position

	bool m_bRenderOnScreen;		// is the scene should be rendered on Screen
	bool m_bRenderToPngFile;	// is the scene should be rendered to PNG file
	CStringA m_strPngFileName;		// file name of PNG render to file
	PngWrapper* m_pRenderToPng;		// holds the render to file

	HBITMAP m_pDbBitMap;
	CDC* m_pDbDC;

// Generated message map functions
protected:
	//{{AFX_MSG(CCGWorkView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoad();
	afx_msg void OnViewOrthographic();
	afx_msg void OnUpdateViewOrthographic(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnActionRotate();
	afx_msg void OnUpdateActionRotate(CCmdUI* pCmdUI);
	afx_msg void OnActionScale();
	afx_msg void OnUpdateActionScale(CCmdUI* pCmdUI);
	afx_msg void OnActionTranslate();
	afx_msg void OnUpdateActionTranslate(CCmdUI* pCmdUI);
	afx_msg void OnAxisX();
	afx_msg void OnUpdateAxisX(CCmdUI* pCmdUI);
	afx_msg void OnAxisY();
	afx_msg void OnUpdateAxisY(CCmdUI* pCmdUI);
	afx_msg void OnAxisZ();
	afx_msg void OnUpdateAxisZ(CCmdUI* pCmdUI);
	afx_msg void OnAxisXY();
	afx_msg void OnUpdateAxisXY(CCmdUI* pCmdUI);
	afx_msg void OnAxisXYZ();
	afx_msg void OnUpdateAxisXYZ(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingFlat();
	afx_msg void OnUpdateLightShadingFlat(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingGouraud();
	afx_msg void OnUpdateLightShadingGouraud(CCmdUI* pCmdUI);
	afx_msg void OnLightConstants();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// CG1::HW2 Functions:
	void doAction(int x_val, int y_val, CG::Object& object);
	void doRotate(int x_val, int y_val, CG::Object& object);
	void doTranslate(int x_val, int y_val, CG::Object& object);
	void doScale(int x_val, int y_val, CG::Object& object);

	CDC* RenderOnScreen();		// Renders the scene on the screen
	void RenderToPngFile();		// Renders the scene to a file in PNG format
	void WriteDCToPngFile(const CDC* pDCImage, int width, int height);

	void InitializeView();
	void DrawScene(CRect& SceneRect, CDC* pDCToUse, int SceneWidth, int SceneHeight, double SceneAspectRatio);
	void DrawFace(
		CDC* pDCToUse,
		CG::RenderMode renderMode,
		const CG::Face& face,
		bool drawFaceNormal,
		bool drawVertexNormal,
		const CG::Camera& camera,
		const CG::mat4& modelToCameraFrame,
		const CG::mat4& projectionToModelFrame,
		const CG::mat4& cameraToGlobalFrame,
		const CG::mat4& modelToGlobalFrame,
		const CG::mat4& globalToModelFrameTranspose,
		const CG::mat4& screenProjection,
		const COLORREF& color,
		const COLORREF& faceNormalColor,
		const COLORREF& vertexNormalColor);

	afx_msg void OnOptionsMouseSensitivity();
	afx_msg void OnViewSpace();
	afx_msg void OnUpdateViewSpace(CCmdUI* pCmdUI);
	afx_msg void OnObjectSpace();
	afx_msg void OnUpdateObjectSpace(CCmdUI* pCmdUI);
	afx_msg void OnFaceNormals();
	afx_msg void OnUpdateFaceNormals(CCmdUI* pCmdUI);
	afx_msg void OnVertexNormals();
	afx_msg void OnUpdateVertexNormals(CCmdUI* pCmdUI);
	afx_msg void OnOptionsColorpicker();
	afx_msg void OnOptionsPolygonalFineness();
	afx_msg void OnOptionsSelectObject();
	afx_msg void OnPerspecctiveSettings();
	afx_msg void OnOptionsBackgroundImage();
	afx_msg void OnRendermodeSolid();
	afx_msg void OnUpdateRendermodeSolid(CCmdUI* pCmdUI);
	afx_msg void OnRendermodeWireframe();
	afx_msg void OnUpdateRendermodeWireframe(CCmdUI* pCmdUI);
	afx_msg void OnRenderOnscreen();
	afx_msg void OnRenderTofile();
	afx_msg void OnUpdateRenderTofile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderOnscreen(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
   { return (CCGWorkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
