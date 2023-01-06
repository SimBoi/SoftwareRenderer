#pragma once
#include "afxdialogex.h"


// AnimationSaveDialog dialog

class AnimationSaveDialog : public CDialog
{
	DECLARE_DYNAMIC(AnimationSaveDialog)

public:
	AnimationSaveDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AnimationSaveDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAVE_ANIMATION_DIALOG };
#endif

private:
	// min, max and acceleration for step
	static const double STEP_ACCEL;
	static const double MIN_STEP;
	static const double MAX_STEP;
	static const double DEFAULT_STEP;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CSpinButtonCtrl m_SpinStepCtrl;
	CString m_Folder;		// selected folder to save animation

	void updateAnimationPath();
public:
	CString m_AnimationName;
	CString m_AnimationPath;
	int m_ImageWidth;
	int m_ImageHeight;
	int m_WindowWidth;
	int m_WindowHeight;
	double m_step;
	BOOL m_rewind;

	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinSaveStep(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditSaveStep();
	afx_msg void OnBnClickedCheckSaveRewind();
	afx_msg void OnBnClickedButtonSelectFolder();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnBnClickedButtonDefaultSize();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
};
