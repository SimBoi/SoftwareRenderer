#pragma once
#include "afxdialogex.h"


// BackgroundImageDialog dialog

class BackgroundImageDialog : public CDialog
{
	DECLARE_DYNAMIC(BackgroundImageDialog)

public:
	BackgroundImageDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~BackgroundImageDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BACKGROUND_IMAGE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadImageButton();
	afx_msg void OnBnClickedOk();
	BOOL m_bRadioNone;
	BOOL m_bRadioStretch;
	BOOL m_bRadioRepeat;
	CButton m_RadioNoneCtrl;
	CButton m_RadioStretchCtrl;
	CButton m_RadioRepeatCtrl;
	virtual BOOL OnInitDialog();
};
