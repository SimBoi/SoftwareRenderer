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
}


BEGIN_MESSAGE_MAP(RenderToFileDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RENDERTO, &RenderToFileDialog::OnBnClickedButtonRenderto)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_SIZE, &RenderToFileDialog::OnBnClickedButtonDefaultSize)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_RENDERTO_FILE, &RenderToFileDialog::OnBnClickedButtonCancelRendertoFile)
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
