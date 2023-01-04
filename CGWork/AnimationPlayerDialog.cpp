// AnimationPlayerDialog.cpp : implementation file
//

//#include "pch.h"
#include "stdafx.h"

#include "CGWork.h"
#include "afxdialogex.h"
#include "AnimationPlayerDialog.h"



// AnimationPlayerDialog dialog

IMPLEMENT_DYNAMIC(AnimationPlayerDialog, CDialog)

const double AnimationPlayerDialog::MIN_SPEED = 0;
const double AnimationPlayerDialog::MAX_SPEED = 100;

const double AnimationPlayerDialog::MIN_STEP = 0.0001;
const double AnimationPlayerDialog::MAX_STEP = 1;

AnimationPlayerDialog::AnimationPlayerDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ANIMATION_PLAYER_DIALOG, pParent)
{

}

AnimationPlayerDialog::~AnimationPlayerDialog()
{
}

void AnimationPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_SPEED, m_SpinSpeedCtrl);
}


BEGIN_MESSAGE_MAP(AnimationPlayerDialog, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED, &AnimationPlayerDialog::OnDeltaposSpinSpeed)
END_MESSAGE_MAP()


// AnimationPlayerDialog message handlers


BOOL AnimationPlayerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_SpinSpeedCtrl.SetRange(0, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void AnimationPlayerDialog::OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int spin_speed = LOWORD(m_SpinSpeedCtrl.GetPos());
	UpdateData(FALSE);
}
