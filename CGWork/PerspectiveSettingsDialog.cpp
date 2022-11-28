// PerspectiveSettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "afxdialogex.h"
#include "PerspectiveSettingsDialog.h"


// PerspectiveSettingsDialog dialog

IMPLEMENT_DYNAMIC(PerspectiveSettingsDialog, CDialog)

PerspectiveSettingsDialog::PerspectiveSettingsDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PERSPECTIVE_SETTINGS, pParent)
	, m_zNear(0)
	, m_zFar(0)
	, m_Fov(0)
{

}

PerspectiveSettingsDialog::~PerspectiveSettingsDialog()
{
}

void PerspectiveSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ZNEAR, m_zNear);
	DDX_Text(pDX, IDC_EDIT_ZFAR, m_zFar);
	DDX_Text(pDX, IDC_EDIT_FOV, m_Fov);
	DDV_MinMaxDouble(pDX, m_zNear, 10, m_zFar);
	DDV_MinMaxDouble(pDX, m_zFar, m_zNear, 10000);
	DDV_MinMaxDouble(pDX, m_Fov, 40, 150);
}


BEGIN_MESSAGE_MAP(PerspectiveSettingsDialog, CDialog)
END_MESSAGE_MAP()


// PerspectiveSettingsDialog message handlers
