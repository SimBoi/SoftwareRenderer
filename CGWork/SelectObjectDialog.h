#pragma once
#include "afxdialogex.h"


// SelectObjectDialog dialog

class SelectObjectDialog : public CDialog
{
	DECLARE_DYNAMIC(SelectObjectDialog)

public:
	SelectObjectDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SelectObjectDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_OBJECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ObjectComboCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeObjectCombo();
	int selected_index;
};
