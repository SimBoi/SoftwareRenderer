// AnimationSaveDialog.cpp : implementation file
//

//#include "pch.h"
#include "stdafx.h"

#include "CGWork.h"
#include "afxdialogex.h"
#include "AnimationSaveDialog.h"


// AnimationSaveDialog dialog

IMPLEMENT_DYNAMIC(AnimationSaveDialog, CDialog)

/*************************Modifiable Parameters*************************/

const double AnimationSaveDialog::STEP_ACCEL = 0.05;
const double AnimationSaveDialog::MIN_STEP = 0.0001;
const double AnimationSaveDialog::MAX_STEP = 1;
const double AnimationSaveDialog::DEFAULT_STEP = 0.10;

/***************************************************************************/

AnimationSaveDialog::AnimationSaveDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SAVE_ANIMATION_DIALOG, pParent)
	, m_step(DEFAULT_STEP)
	, m_rewind(FALSE)
	, m_AnimationName(_T("Animation"))
	, m_AnimationPath(_T("\\"))
	, m_Folder(_T("\\"))
	, m_ImageWidth(0)
	, m_ImageHeight(0)
	, m_WindowWidth(0)
	, m_WindowHeight(0)
{

}

AnimationSaveDialog::~AnimationSaveDialog()
{
}


static void DDV_PATH(CDataExchange* pDX, CString value)
{
	if (value == "")
	{
		CString msg;
		msg.Format(_T("Invalid Path: " + value + "\nselect folder and choose name to save the animation"));
		AfxMessageBox(msg, MB_ICONSTOP);
		pDX->Fail();
	}
}

void AnimationSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_SAVE_STEP, m_SpinStepCtrl);
	DDX_Text(pDX, IDC_EDIT_SAVE_STEP, m_step);
	DDV_MinMaxDouble(pDX, m_step, MIN_STEP, MAX_STEP);
	DDX_Check(pDX, IDC_CHECK_SAVE_REWIND, m_rewind);
	DDX_Text(pDX, IDC_EDIT_NAME, m_AnimationName);
	DDX_Text(pDX, IDC_EDIT_PATH, m_AnimationPath);
	DDV_PATH(pDX, m_AnimationPath);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_ImageWidth);
	DDV_MinMaxInt(pDX, m_ImageWidth, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_ImageHeight);
	DDV_MinMaxInt(pDX, m_ImageHeight, 0, INT_MAX);
}


BEGIN_MESSAGE_MAP(AnimationSaveDialog, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SAVE_STEP, &AnimationSaveDialog::OnDeltaposSpinSaveStep)
	ON_EN_CHANGE(IDC_EDIT_SAVE_STEP, &AnimationSaveDialog::OnEnChangeEditSaveStep)
	ON_BN_CLICKED(IDC_CHECK_SAVE_REWIND, &AnimationSaveDialog::OnBnClickedCheckSaveRewind)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FOLDER, &AnimationSaveDialog::OnBnClickedButtonSelectFolder)
	ON_EN_CHANGE(IDC_EDIT_NAME, &AnimationSaveDialog::OnEnChangeEditName)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_SIZE, &AnimationSaveDialog::OnBnClickedButtonDefaultSize)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &AnimationSaveDialog::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &AnimationSaveDialog::OnEnChangeEditHeight)
END_MESSAGE_MAP()


// AnimationSaveDialog message handlers


BOOL AnimationSaveDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_SpinStepCtrl.SetRange(0, short((MAX_STEP - MIN_STEP) / STEP_ACCEL));
	m_SpinStepCtrl.SetPos(int(m_step / STEP_ACCEL));

	updateAnimationPath();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void AnimationSaveDialog::OnDeltaposSpinSaveStep(NMHDR* pNMHDR, LRESULT* pResult)
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


void AnimationSaveDialog::OnEnChangeEditSaveStep()
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


void AnimationSaveDialog::OnBnClickedCheckSaveRewind()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


void AnimationSaveDialog::updateAnimationPath()
{
	if (m_AnimationName != "" && m_Folder != "")
	{
		m_AnimationPath = m_Folder + _T("\\") + m_AnimationName + _T("\\");
	}
	else
	{
		m_AnimationPath = "";
	}
	UpdateData(FALSE);
}


void AnimationSaveDialog::OnBnClickedButtonSelectFolder()
{
	// TODO: Add your control notification handler code here


	/*CFileDialog dlg(FALSE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		m_FilePathString = dlg.GetPathName(); // Full path and filename
		UpdateData(false);
	}*/

	CFolderPickerDialog dlg;

	dlg.m_ofn.lpstrTitle = _T("Save Animation to...");
	if (dlg.DoModal() == IDOK)
	{
		m_Folder = dlg.GetPathName();   // selected folder name 
		updateAnimationPath();
	}

}


void AnimationSaveDialog::OnEnChangeEditName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	updateAnimationPath();
}


void AnimationSaveDialog::OnBnClickedButtonDefaultSize()
{
	// TODO: Add your control notification handler code here
	m_ImageWidth = m_WindowWidth;
	m_ImageHeight = m_WindowHeight;
	UpdateData(FALSE);
}


void AnimationSaveDialog::OnEnChangeEditWidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}


void AnimationSaveDialog::OnEnChangeEditHeight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}
