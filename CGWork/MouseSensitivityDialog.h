#pragma once
#include "afxdialogex.h"


// MouseSensitivityDialog dialog

class MouseSensitivityDialog : public CDialog
{
	DECLARE_DYNAMIC(MouseSensitivityDialog)

public:
	MouseSensitivityDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MouseSensitivityDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUSE_SENSITIVITY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_translation_slider;
	int m_rotation_slider;
	int m_scale_slider;
	afx_msg void OnBnClickedDefaults();
};
