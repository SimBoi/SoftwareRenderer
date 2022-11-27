// PolygonalFineNessDialog.cpp : implementation file
//

// #include "pch.h"
#include "stdafx.h"

#include "CGWork.h"
#include "afxdialogex.h"
#include "iritSkel.h"

#include "PolygonalFineNessDialog.h"


// PolygonalFineNessDialog dialog

IMPLEMENT_DYNAMIC(PolygonalFineNessDialog, CDialog)

PolygonalFineNessDialog::PolygonalFineNessDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_FINENESS_DIALOG, pParent)
	, m_polygonal_fineness(0)
	, m_fineness_text(_T("set Polygonal FineNess"))
{

}

PolygonalFineNessDialog::~PolygonalFineNessDialog()
{
}

void PolygonalFineNessDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_FINENESS_SLIDER, m_polygonal_fineness);
	DDV_MinMaxInt(pDX, m_polygonal_fineness, 2, INT_MAX);
	DDX_Control(pDX, IDC_FINENESS_SLIDER, m_fineness_slider_ctrl);
	DDX_Text(pDX, IDC_FINENESS_STATIC, m_fineness_text);
}


BEGIN_MESSAGE_MAP(PolygonalFineNessDialog, CDialog)
	ON_BN_CLICKED(ID_DEFAULT_FINENESS, &PolygonalFineNessDialog::OnClickedDefaultFineness)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// PolygonalFineNessDialog message handlers


BOOL PolygonalFineNessDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_fineness_slider_ctrl.SetRange(MIN_FINENESS, MAX_FINENESS, TRUE);
	m_polygonal_fineness = polygonal_fineness;
	m_fineness_slider_ctrl.SetPos(polygonal_fineness);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void PolygonalFineNessDialog::OnClickedDefaultFineness()
{
	this->m_polygonal_fineness = DEFAULT_FINENESS;
	m_fineness_text.Format(_T("FineNess: %d"), m_polygonal_fineness);
	UpdateData(FALSE);
}


void PolygonalFineNessDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_fineness_slider_ctrl)
	{
		int value = m_fineness_slider_ctrl.GetPos();
		m_polygonal_fineness = value;
		m_fineness_text.Format(_T("FineNess: %d"), value);
		UpdateData(FALSE);
	}
	else
	{
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}
