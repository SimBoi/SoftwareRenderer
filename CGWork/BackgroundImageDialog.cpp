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

using namespace CG;


// BackgroundImageDialog dialog

IMPLEMENT_DYNAMIC(BackgroundImageDialog, CDialog)

BackgroundImageDialog::BackgroundImageDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BACKGROUND_IMAGE_DIALOG, pParent)
{

}

BackgroundImageDialog::~BackgroundImageDialog()
{
}

void BackgroundImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_NONE, m_RadioNoneCtrl);
	DDX_Control(pDX, IDC_RADIO_STRETCH, m_RadioStretchCtrl);
	DDX_Control(pDX, IDC_RADIO_REPEAT, m_RadioRepeatCtrl);
}


BEGIN_MESSAGE_MAP(BackgroundImageDialog, CDialog)
	ON_BN_CLICKED(IDC_LOAD_IMAGE_BUTTON, &BackgroundImageDialog::OnBnClickedLoadImageButton)
	ON_BN_CLICKED(IDOK, &BackgroundImageDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_NONE, &BackgroundImageDialog::OnBnClickedRadioNone)
	ON_BN_CLICKED(IDC_RADIO_STRETCH, &BackgroundImageDialog::OnBnClickedRadioStretch)
	ON_BN_CLICKED(IDC_RADIO_REPEAT, &BackgroundImageDialog::OnBnClickedRadioRepeat)
END_MESSAGE_MAP()


// BackgroundImageDialog message handlers

void  BackgroundImageDialog::setLayoutRadios()
{
	m_RadioStretchCtrl.EnableWindow(IsBackgroundImageLoaded);
	m_RadioRepeatCtrl.EnableWindow(IsBackgroundImageLoaded);
}

void BackgroundImageDialog::OnBnClickedLoadImageButton()
{
	TCHAR szFilters[] = _T("PNG Files (*.png)|*.png|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		CStringA png_file_name(dlg.GetPathName());		// Full path and filename
		CG::BackgroundImage.SetFileName(png_file_name);
		IsBackgroundImageLoaded = CG::BackgroundImage.ReadPng();
		setLayoutRadios();
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
	setLayoutRadios();

	m_RadioNoneCtrl.SetCheck((BackgroundImageLayout == NONE) ? TRUE : FALSE);
	m_RadioStretchCtrl.SetCheck((BackgroundImageLayout == STRETCH) ? TRUE : FALSE);
	m_RadioRepeatCtrl.SetCheck((BackgroundImageLayout == REPEAT) ? TRUE : FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void BackgroundImageDialog::OnBnClickedRadioNone()
{
	CG::BackgroundImageLayout = CG::NONE;
}


void BackgroundImageDialog::OnBnClickedRadioStretch()
{
	CG::BackgroundImageLayout = CG::STRETCH;
}


void BackgroundImageDialog::OnBnClickedRadioRepeat()
{
	CG::BackgroundImageLayout = CG::REPEAT;
}
