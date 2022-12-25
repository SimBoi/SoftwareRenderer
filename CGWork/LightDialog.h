#pragma once

#include "Light.h"
// CLightDialog dialog



class CLightDialog : public CDialog
{
	DECLARE_DYNAMIC(CLightDialog)

public:
	CLightDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightDialog();

	//dialog interface
	void SetLightData(LightID id,const LightParams& light);
	LightParams GetLightData(LightID id);
	void SetCosineFactor(int cosineFactor);
	void SetDynamicRange(double dynamicRange);
	int GetCosineFactor();
	double GetDynamicRange();

// Dialog Data
	enum { IDD = IDD_LIGHTS_DLG };

protected:
	LightParams m_lights[MAX_LIGHT];
	LightParams m_ambient;
	int m_cosineFactor;
	double m_dynamicRange;
	int m_currentLightIdx;
	int GetCurrentLightIndex();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedRadioLight();
//    afx_msg void On();
    virtual BOOL OnInitDialog();
};
