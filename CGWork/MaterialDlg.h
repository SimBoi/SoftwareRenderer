#pragma once


// CMaterialDlg dialog

class CMaterialDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaterialDlg)

public:
	CMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaterialDlg();

// Dialog Data
	enum { IDD = IDD_MATERIAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

    // ambient property of the material
    double m_ambient;
    // Diffuse property of the material
    double m_diffuse;
    // shininess property of the material
    double m_shininess;
    // specular property of the material (integer values only)
    int m_specular;
};
