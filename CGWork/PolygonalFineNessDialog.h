#pragma once
#include "afxdialogex.h"


// PolygonalFineNessDialog dialog

class PolygonalFineNessDialog : public CDialog
{
	DECLARE_DYNAMIC(PolygonalFineNessDialog)

public:
	PolygonalFineNessDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PolygonalFineNessDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINENESS_DIALOG };
#endif

private:
	// control the min and max values of polygonal fineness
	static const int MIN_FINENESS = 2;
	static const int MAX_FINENESS = 200; // INT_MAX
	static const int DEFAULT_FINENESS = 20;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_polygonal_fineness;
	CSliderCtrl m_fineness_slider_ctrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedDefaultFineness();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CString m_fineness_text;
};
