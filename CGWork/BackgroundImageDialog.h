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
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioNone();
	afx_msg void OnBnClickedRadioStretch();
	afx_msg void OnBnClickedRadioRepeat();
	void setLayoutRadios();
	CButton m_RadioNoneCtrl;
	CButton m_RadioStretchCtrl;
	CButton m_RadioRepeatCtrl;
};
