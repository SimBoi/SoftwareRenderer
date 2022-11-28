// SelectObjectDialog.cpp : implementation file
//

// #include "pch.h"
#include "stdafx.h"

#include "CGWork.h"
#include "afxdialogex.h"
#include "SelectObjectDialog.h"

#include <string>
#include "iritSkel.h"
#include "CG_Object.h"


// SelectObjectDialog dialog

IMPLEMENT_DYNAMIC(SelectObjectDialog, CDialog)

SelectObjectDialog::SelectObjectDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SELECT_OBJECT_DIALOG, pParent)
{
	selected_index = 0;
}

SelectObjectDialog::~SelectObjectDialog()
{
}

void SelectObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECT_COMBO, m_ObjectComboCtrl);
}


BEGIN_MESSAGE_MAP(SelectObjectDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_OBJECT_COMBO, &SelectObjectDialog::OnSelchangeObjectCombo)
END_MESSAGE_MAP()


// SelectObjectDialog message handlers


BOOL SelectObjectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int cnt = 0;
	CString str;
	for (auto const& child : parentObject.children)
	{
		str.Format(_T("Object %d"), cnt + 1);
		m_ObjectComboCtrl.AddString(str);
		cnt++;
	}

	m_ObjectComboCtrl.SetCurSel(object_index);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void SelectObjectDialog::OnSelchangeObjectCombo()
{
	selected_index = m_ObjectComboCtrl.GetCurSel();
	UpdateData(FALSE);
}
