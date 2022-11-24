// MouseSensitivityDialog.cpp : implementation file
//

// #include "pch.h"
#include "stdafx.h"


#include "CGWork.h"
#include "afxdialogex.h"
#include "MouseSensitivityDialog.h"

#include "iritSkel.h"
#include "CG_Object.h"


// MouseSensitivityDialog dialog

IMPLEMENT_DYNAMIC(MouseSensitivityDialog, CDialog)

MouseSensitivityDialog::MouseSensitivityDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MOUSE_SENSITIVITY_DIALOG, pParent)
	, m_translation_slider(0)
	, m_rotation_slider(0)
	, m_scale_slider(0)
{

}

MouseSensitivityDialog::~MouseSensitivityDialog()
{
}

void MouseSensitivityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_TRANSLATION_SLIDER, m_translation_slider);
	DDV_MinMaxInt(pDX, m_translation_slider, MIN_SENSITIVITY, MAX_SENSITIVITY);
	DDX_Slider(pDX, IDC_ROTATION_SLIDER, m_rotation_slider);
	DDV_MinMaxInt(pDX, m_rotation_slider, MIN_SENSITIVITY, MAX_SENSITIVITY);
	DDX_Slider(pDX, IDC_SCALE_SLIDER, m_scale_slider);
	DDV_MinMaxInt(pDX, m_scale_slider, MIN_SENSITIVITY, MAX_SENSITIVITY);
	DDX_Control(pDX, IDC_TRANSLATION_SLIDER, m_translation_slider_ctrl);
	DDX_Control(pDX, IDC_ROTATION_SLIDER, m_rotation_slider_ctrl);
	DDX_Control(pDX, IDC_SCALE_SLIDER, m_scale_slider_ctrl);
}


BEGIN_MESSAGE_MAP(MouseSensitivityDialog, CDialog)
	ON_BN_CLICKED(IDDEFAULTS, &MouseSensitivityDialog::OnBnClickedDefaults)
END_MESSAGE_MAP()


// MouseSensitivityDialog message handlers


void MouseSensitivityDialog::OnBnClickedDefaults()
{
	CG::setDefaultSensitivity();
	this->m_translation_slider = CG::translation_sensitivity;
	this->m_rotation_slider = CG::rotation_sensitivity;
	this->m_scale_slider = CG::scale_sensitivity;

	UpdateData(FALSE);
}


BOOL MouseSensitivityDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_translation_slider_ctrl.SetRange(MIN_SENSITIVITY, MAX_SENSITIVITY, TRUE);
	m_rotation_slider_ctrl.SetRange(MIN_SENSITIVITY, MAX_SENSITIVITY, TRUE);
	m_scale_slider_ctrl.SetRange(MIN_SENSITIVITY, MAX_SENSITIVITY, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
