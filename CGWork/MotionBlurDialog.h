#pragma once
#include "afxdialogex.h"


// MotionBlurDialog dialog

class MotionBlurDialog : public CDialog
{
	DECLARE_DYNAMIC(MotionBlurDialog)

public:
	MotionBlurDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MotionBlurDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOTIONBLUR_DIALOG };
#endif

private:
	// min, max and acceleration for speed
	static const double BLUR_ACCEL;
	static const double MIN_BLUR;
	static const double MAX_BLUR;
	static const double DEFAULT_BLUR;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CSpinButtonCtrl m_SpinBlurCtrl;
public:
	double m_blur_value;
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinBlurValue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditBlurValue();
};
