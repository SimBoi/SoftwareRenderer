#pragma once
#include "afxdialogex.h"


// PerspectiveSettingsDialog dialog

class PerspectiveSettingsDialog : public CDialog
{
	DECLARE_DYNAMIC(PerspectiveSettingsDialog)

public:
	PerspectiveSettingsDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PerspectiveSettingsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PERSPECTIVE_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_zNear;
	double m_zFar;
	double m_Fov;
};
