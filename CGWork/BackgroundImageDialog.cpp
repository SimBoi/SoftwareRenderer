// BackgroundImageDialog.cpp : implementation file
//

// #include "pch.h"
#include "stdafx.h"

#include <string>
#include "CGWork.h"
#include "afxdialogex.h"
#include "PngWrapper.h"
#include "CG_Object.h"
#include "BackgroundImageDialog.h"


// BackgroundImageDialog dialog

IMPLEMENT_DYNAMIC(BackgroundImageDialog, CDialog)

BackgroundImageDialog::BackgroundImageDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BACKGROUND_IMAGE_DIALOG, pParent)
	, m_bRadioNone(FALSE)
	, m_bRadioStretch(TRUE)
	, m_bRadioRepeat(TRUE)
{

}

BackgroundImageDialog::~BackgroundImageDialog()
{
}

void BackgroundImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NONE, m_bRadioNone);
	//DDX_Radio(pDX, IDC_RADIO_STRETCH, m_bRadioStretch);
	/*DDX_Radio(pDX, IDC_RADIO_REPEAT, m_bRadioRepeat);
	DDX_Control(pDX, IDC_RADIO_NONE, m_RadioNoneCtrl);
	DDX_Control(pDX, IDC_RADIO_STRETCH, m_RadioStretchCtrl);
	DDX_Control(pDX, IDC_RADIO_REPEAT, m_RadioRepeatCtrl);*/
}


BEGIN_MESSAGE_MAP(BackgroundImageDialog, CDialog)
	ON_BN_CLICKED(IDC_LOAD_IMAGE_BUTTON, &BackgroundImageDialog::OnBnClickedLoadImageButton)
	ON_BN_CLICKED(IDOK, &BackgroundImageDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// BackgroundImageDialog message handlers


void BackgroundImageDialog::OnBnClickedLoadImageButton()
{
	TCHAR szFilters[] = _T("PNG Files (*.png)|*.png|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		CStringA png_file_name(dlg.GetPathName());		// Full path and filename
		CG::BackgroundImage.SetFileName(png_file_name);
		CG::BackgroundImage.ReadPng();
	}
}


void BackgroundImageDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


BOOL BackgroundImageDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

