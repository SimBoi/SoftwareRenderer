#pragma once
#include "afxdialogex.h"

#include "CG_Draw.h"


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
	// min, max and acceleration for speed
	static const double SPEED_ACCEL;
	static const double MIN_SPEED;
	static const double MAX_SPEED;
	static const double DEFAULT_SPEED;

	// min, max and acceleration for step
	static const double STEP_ACCEL;
	static const double MIN_STEP;
	static const double MAX_STEP;
	static const double DEFAULT_STEP;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CSpinButtonCtrl m_SpinSpeedCtrl;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	double m_speed;
	afx_msg void OnEnChangeEditSpeed();
private:
	CSpinButtonCtrl m_SpinStepCtrl;
public:
	double m_step;
	afx_msg void OnDeltaposSpinStep(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditStep();
	BOOL m_rewind;
	CG::RenderMode m_render_mode;
private:
	CComboBox m_RenderModeComboCtrl;
public:
	afx_msg void OnCbnSelchangeComboRenderMode();
};
