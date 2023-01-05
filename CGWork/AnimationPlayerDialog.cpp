// AnimationPlayerDialog.cpp : implementation file
//

//#include "pch.h"
#include "stdafx.h"

#include "CGWork.h"
#include "afxdialogex.h"
#include "AnimationPlayerDialog.h"



// AnimationPlayerDialog dialog

IMPLEMENT_DYNAMIC(AnimationPlayerDialog, CDialog)

/*************************Modifiable Parameters*************************/

const double AnimationPlayerDialog::SPEED_ACCEL = 0.25;
const double AnimationPlayerDialog::MIN_SPEED = 0;
const double AnimationPlayerDialog::MAX_SPEED = 10;
const double AnimationPlayerDialog::DEFAULT_SPEED = 1;

const double AnimationPlayerDialog::STEP_ACCEL = 0.05;
const double AnimationPlayerDialog::MIN_STEP = 0.0001;
const double AnimationPlayerDialog::MAX_STEP = 1;
const double AnimationPlayerDialog::DEFAULT_STEP = 0.10;

/***************************************************************************/

AnimationPlayerDialog::AnimationPlayerDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ANIMATION_PLAYER_DIALOG, pParent)
	, m_speed(DEFAULT_SPEED)
	, m_step(DEFAULT_STEP)
	, m_rewind(FALSE)
	, m_render_mode(CG::WIREFRAME)
	, m_bSeparateThread(FALSE)
{
	
}

AnimationPlayerDialog::~AnimationPlayerDialog()
{
}

void AnimationPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_SPEED, m_SpinSpeedCtrl);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_speed);
	DDV_MinMaxDouble(pDX, m_speed, MIN_SPEED, MAX_SPEED);
	DDX_Control(pDX, IDC_SPIN_STEP, m_SpinStepCtrl);
	DDX_Text(pDX, IDC_EDIT_STEP, m_step);
	DDV_MinMaxDouble(pDX, m_step, MIN_STEP, MAX_STEP);
	DDX_Check(pDX, IDC_CHECK_REWIND, m_rewind);
	DDX_Control(pDX, IDC_COMBO_RENDER_MODE, m_RenderModeComboCtrl);
	DDX_Check(pDX, IDC_CHECK_THREAD, m_bSeparateThread);
}


BEGIN_MESSAGE_MAP(AnimationPlayerDialog, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED, &AnimationPlayerDialog::OnDeltaposSpinSpeed)
	ON_EN_CHANGE(IDC_EDIT_SPEED, &AnimationPlayerDialog::OnEnChangeEditSpeed)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEP, &AnimationPlayerDialog::OnDeltaposSpinStep)
	ON_EN_CHANGE(IDC_EDIT_STEP, &AnimationPlayerDialog::OnEnChangeEditStep)
	ON_CBN_SELCHANGE(IDC_COMBO_RENDER_MODE, &AnimationPlayerDialog::OnCbnSelchangeComboRenderMode)
	ON_BN_CLICKED(IDC_CHECK_REWIND, &AnimationPlayerDialog::OnBnClickedCheckRewind)
	ON_BN_CLICKED(IDC_CHECK_THREAD, &AnimationPlayerDialog::OnBnClickedCheckThread)
END_MESSAGE_MAP()


// AnimationPlayerDialog message handlers


BOOL AnimationPlayerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_RenderModeComboCtrl.AddString(_T("Wireframe"));
	m_RenderModeComboCtrl.AddString(_T("Solid"));
	m_RenderModeComboCtrl.SetCurSel(0);

	m_SpinSpeedCtrl.SetRange(0, short((MAX_SPEED - MIN_SPEED) / SPEED_ACCEL));
	m_SpinSpeedCtrl.SetPos(int(m_speed / SPEED_ACCEL));

	m_SpinStepCtrl.SetRange(0, short((MAX_STEP - MIN_STEP) / STEP_ACCEL));
	m_SpinStepCtrl.SetPos(int(m_step / STEP_ACCEL));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void AnimationPlayerDialog::OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int spin_speed_progress = pNMUpDown->iPos + pNMUpDown->iDelta;
	m_speed = spin_speed_progress * SPEED_ACCEL;
	if (m_speed > MAX_SPEED) m_speed = MAX_SPEED;
	if (m_speed < MIN_SPEED) m_speed = MIN_SPEED;

	UpdateData(FALSE);
}


void AnimationPlayerDialog::OnEnChangeEditSpeed()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	if (m_speed > MAX_SPEED) m_speed = MAX_SPEED;
	if (m_speed < MIN_SPEED) m_speed = MIN_SPEED;

	m_SpinSpeedCtrl.SetPos(int(m_speed / SPEED_ACCEL));

}


void AnimationPlayerDialog::OnDeltaposSpinStep(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int spin_step_progress = pNMUpDown->iPos + pNMUpDown->iDelta;
	m_step = spin_step_progress * STEP_ACCEL;
	if (m_step > MAX_STEP) m_step = MAX_STEP;
	if (m_step < MIN_STEP) m_step = MIN_STEP;

	UpdateData(FALSE);
}


void AnimationPlayerDialog::OnEnChangeEditStep()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData(TRUE);

	if (m_step > MAX_STEP) m_step = MAX_STEP;
	if (m_step < MIN_STEP) m_step = MIN_STEP;

	m_SpinStepCtrl.SetPos(int(m_step / STEP_ACCEL));
}


void AnimationPlayerDialog::OnCbnSelchangeComboRenderMode()
{
	// TODO: Add your control notification handler code here
	int selected_index = m_RenderModeComboCtrl.GetCurSel();
	m_render_mode = (selected_index == 0 ? CG::WIREFRAME : CG::SOLID);
	UpdateData(FALSE);
}


void AnimationPlayerDialog::OnBnClickedCheckRewind()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


void AnimationPlayerDialog::OnBnClickedCheckThread()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
