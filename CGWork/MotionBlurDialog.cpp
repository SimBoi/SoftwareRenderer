// MotionBlurDialog.cpp : implementation file
//

//#include "pch.h"
#include "stdafx.h"
#include "CGWork.h"
#include "afxdialogex.h"
#include "MotionBlurDialog.h"


// MotionBlurDialog dialog

IMPLEMENT_DYNAMIC(MotionBlurDialog, CDialog)

/*************************Modifiable Parameters*************************/

const double MotionBlurDialog::BLUR_ACCEL = 0.05;
const double MotionBlurDialog::MIN_BLUR = 0;
const double MotionBlurDialog::MAX_BLUR = 1;
const double MotionBlurDialog::DEFAULT_BLUR = 0.25;

/***************************************************************************/


MotionBlurDialog::MotionBlurDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MOTIONBLUR_DIALOG, pParent)
	, m_blur_value(0)
{

}

MotionBlurDialog::~MotionBlurDialog()
{
}

void MotionBlurDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_BLUR_VALUE, m_SpinBlurCtrl);
	DDX_Text(pDX, IDC_EDIT_BLUR_VALUE, m_blur_value);
	DDV_MinMaxDouble(pDX, m_blur_value, MIN_BLUR, MAX_BLUR);
}


BEGIN_MESSAGE_MAP(MotionBlurDialog, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BLUR_VALUE, &MotionBlurDialog::OnDeltaposSpinBlurValue)
	ON_EN_CHANGE(IDC_EDIT_BLUR_VALUE, &MotionBlurDialog::OnEnChangeEditBlurValue)
END_MESSAGE_MAP()


// MotionBlurDialog message handlers


BOOL MotionBlurDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_SpinBlurCtrl.SetRange(0, short((MAX_BLUR - MIN_BLUR) / BLUR_ACCEL));
	m_SpinBlurCtrl.SetPos(int(m_blur_value / BLUR_ACCEL));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void MotionBlurDialog::OnDeltaposSpinBlurValue(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int spin_blur_progress = pNMUpDown->iPos + pNMUpDown->iDelta;
	m_blur_value = spin_blur_progress * BLUR_ACCEL;
	if (m_blur_value > MAX_BLUR) m_blur_value = MAX_BLUR;
	if (m_blur_value < MIN_BLUR) m_blur_value = MIN_BLUR;

	UpdateData(FALSE);
}


void MotionBlurDialog::OnEnChangeEditBlurValue()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData(TRUE);

	if (m_blur_value > MAX_BLUR) m_blur_value = MAX_BLUR;
	if (m_blur_value < MIN_BLUR) m_blur_value = MIN_BLUR;

	m_SpinBlurCtrl.SetPos(int(m_blur_value / BLUR_ACCEL));
}
