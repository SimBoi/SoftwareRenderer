#pragma once
#include "afxdialogex.h"


// ColorPickerDialog dialog

class ColorPickerDialog : public CDialog
{
	DECLARE_DYNAMIC(ColorPickerDialog)

public:
	ColorPickerDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ColorPickerDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_PICKER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedBackgroundColorButton();
	afx_msg void OnClickedVertixNormalColorButton();
	afx_msg void OnClickedFacenormalColorButton();
	afx_msg void OnClickedBoundingboxColorButton();
	afx_msg void OnClickedModelColorButton();
	afx_msg void OnClickedOriginalModelColorButton();
	afx_msg void OnClickedDefaultColorsButton();
	afx_msg void OnBnClickedBackgroundImageButton();
};
