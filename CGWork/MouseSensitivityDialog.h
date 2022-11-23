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

private:
	// control the min and max values of sensitivity sliders
	static const int MIN_SENSITIVITY = 1;
	static const int MAX_SENSITIVITY = 100;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_translation_slider;
	int m_rotation_slider;
	int m_scale_slider;
	afx_msg void OnBnClickedDefaults();
	virtual BOOL OnInitDialog();
	CSliderCtrl m_translation_slider_ctrl;
	CSliderCtrl m_rotation_slider_ctrl;
	CSliderCtrl m_scale_slider_ctrl;
};
