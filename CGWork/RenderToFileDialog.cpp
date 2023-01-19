// RenderToFileDialog.cpp : implementation file
//

// #include "pch.h"
#include "stdafx.h"

#include "CGWork.h"
#include "afxdialogex.h"
#include "RenderToFileDialog.h"


// RenderToFileDialog dialog

IMPLEMENT_DYNAMIC(RenderToFileDialog, CDialog)

RenderToFileDialog::RenderToFileDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TOPNG_FILE_DIALOG, pParent)
	, m_FilePathString(_T(""))
	, m_ImageWidth(0)
	, m_ImageHeight(0)
	, m_WindowWidth(0)
	, m_WindowHeight(0)
	, m_bAntiAliasing(TRUE)
{

}

RenderToFileDialog::~RenderToFileDialog()
{
}

void RenderToFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_FilePathString);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_ImageWidth);
	DDV_MinMaxInt(pDX, m_ImageWidth, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_ImageHeight);
	DDV_MinMaxInt(pDX, m_ImageHeight, 0, INT_MAX);
	DDX_Check(pDX, IDC_ALIASING_CHECK, m_bAntiAliasing);
}


BEGIN_MESSAGE_MAP(RenderToFileDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RENDERTO, &RenderToFileDialog::OnBnClickedButtonRenderto)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_SIZE, &RenderToFileDialog::OnBnClickedButtonDefaultSize)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_RENDERTO_FILE, &RenderToFileDialog::OnBnClickedButtonCancelRendertoFile)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &RenderToFileDialog::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &RenderToFileDialog::OnEnChangeEditHeight)
	ON_BN_CLICKED(IDC_ALIASING_CHECK, &RenderToFileDialog::OnBnClickedAliasingCheck)
END_MESSAGE_MAP()


// RenderToFileDialog message handlers


void RenderToFileDialog::OnBnClickedButtonRenderto()
{
	TCHAR szFilters[] = _T("PNG Files (*.png)|*.png|All Files (*.*)|*.*||");

	CFileDialog dlg(FALSE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		m_FilePathString = dlg.GetPathName(); // Full path and filename
		UpdateData(false);
	}
}


void RenderToFileDialog::OnBnClickedButtonDefaultSize()
{
	m_ImageWidth = m_WindowWidth;
	m_ImageHeight = m_WindowHeight;
	UpdateData(false);
}


void RenderToFileDialog::OnBnClickedButtonCancelRendertoFile()
{
	EndDialog(IDCANCEL_RENDERTO_FILE);
}


void RenderToFileDialog::OnEnChangeEditWidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);
}


void RenderToFileDialog::OnEnChangeEditHeight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(true);
}


void RenderToFileDialog::OnBnClickedAliasingCheck()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}
