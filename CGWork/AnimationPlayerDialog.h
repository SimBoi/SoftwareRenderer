#pragma once
#include "afxdialogex.h"


// AnimationPlayerDialog dialog

class AnimationPlayerDialog : public CDialog
{
	DECLARE_DYNAMIC(AnimationPlayerDialog)

public:
	AnimationPlayerDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AnimationPlayerDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATION_PLAYER_DIALOG };
#endif

private:
	// control the min and max values of speed and step spins
	static const double MIN_SPEED;
	static const double MAX_SPEED;

	static const double MIN_STEP;
	static const double MAX_STEP;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CSpinButtonCtrl m_SpinSpeedCtrl;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult);
};
