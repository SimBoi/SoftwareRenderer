// MouseSensitivityDialog.cpp : implementation file
//

// #include "pch.h"
#include "stdafx.h"


#include "CGWork.h"
#include "afxdialogex.h"
#include "MouseSensitivityDialog.h"

#include "iritSkel.h"


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
	DDV_MinMaxInt(pDX, m_translation_slider, 0, 100);
	DDX_Slider(pDX, IDC_ROTATION_SLIDER, m_rotation_slider);
	DDV_MinMaxInt(pDX, m_rotation_slider, 0, 100);
	DDX_Slider(pDX, IDC_SCALE_SLIDER, m_scale_slider);
	DDV_MinMaxInt(pDX, m_scale_slider, 0, 100);
}


BEGIN_MESSAGE_MAP(MouseSensitivityDialog, CDialog)
	ON_BN_CLICKED(IDDEFAULTS, &MouseSensitivityDialog::OnBnClickedDefaults)
END_MESSAGE_MAP()


// MouseSensitivityDialog message handlers


void MouseSensitivityDialog::OnBnClickedDefaults()
{
	/*parentObject.setDefaultSensitivity();
	this->m_translation_slider = parentObject.translation_sensitivity;
	this->m_rotation_slider = parentObject.rotation_sensitivity;
	this->m_scale_slider = parentObject.scale_sensitivity;*/
}
