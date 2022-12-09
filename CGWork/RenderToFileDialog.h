#pragma once
#include "afxdialogex.h" 

// RenderToFileDialog dialog
#define IDCANCEL_RENDERTO_FILE	999

class RenderToFileDialog : public CDialog
{
	DECLARE_DYNAMIC(RenderToFileDialog)

public:
	RenderToFileDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RenderToFileDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOPNG_FILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRenderto();
	CString m_FilePathString;
	int m_ImageWidth;
	int m_ImageHeight;
	int m_WindowWidth;
	int m_WindowHeight;
	afx_msg void OnBnClickedButtonDefaultSize();
	afx_msg void OnBnClickedButtonCancelRendertoFile(); 
};
