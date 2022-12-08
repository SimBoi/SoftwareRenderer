// ColorPickerDialog.cpp : implementation file
//

// #include "pch.h"
#include "stdafx.h"

#include "CGWork.h"
#include "afxdialogex.h"
#include "ColorPickerDialog.h"
#include "BackgroundImageDialog.h"
#include "CG_Object.h"


// ColorPickerDialog dialog

IMPLEMENT_DYNAMIC(ColorPickerDialog, CDialog)

ColorPickerDialog::ColorPickerDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_COLOR_PICKER_DIALOG, pParent)
{

}

ColorPickerDialog::~ColorPickerDialog()
{
}

void ColorPickerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ColorPickerDialog, CDialog)
	ON_BN_CLICKED(IDC_BACKGROUND_COLOR_BUTTON, &ColorPickerDialog::OnClickedBackgroundColorButton)
	ON_BN_CLICKED(IDC_VERTIX_NORMAL_COLOR_BUTTON, &ColorPickerDialog::OnClickedVertixNormalColorButton)
	ON_BN_CLICKED(IDC_FACENORMAL_COLOR_BUTTON, &ColorPickerDialog::OnClickedFacenormalColorButton)
	ON_BN_CLICKED(IDC_BOUNDINGBOX_COLOR_BUTTON, &ColorPickerDialog::OnClickedBoundingboxColorButton)
	ON_BN_CLICKED(IDC_MODEL_COLOR_BUTTON, &ColorPickerDialog::OnClickedModelColorButton)
	ON_BN_CLICKED(IDC_ORIGINAL_MODEL_COLOR_BUTTON, &ColorPickerDialog::OnClickedOriginalModelColorButton)
	ON_BN_CLICKED(ID_DEFAULT_COLORS_BUTTON, &ColorPickerDialog::OnClickedDefaultColorsButton)
	ON_BN_CLICKED(IDC_BACKGROUND_IMAGE_BUTTON, &ColorPickerDialog::OnBnClickedBackgroundImageButton)
END_MESSAGE_MAP()


// ColorPickerDialog message handlers


void ColorPickerDialog::OnClickedBackgroundColorButton()
{
	CColorDialog color_dialog(CG::BackgroundColor);
	if (color_dialog.DoModal() == IDOK)
	{
		CG::BackgroundColor = color_dialog.GetColor();
	}
}


void ColorPickerDialog::OnClickedVertixNormalColorButton()
{
	CColorDialog color_dialog(CG::VertexNormalColor);
	if (color_dialog.DoModal() == IDOK)
	{
		CG::VertexNormalColor = color_dialog.GetColor();
	}
}


void ColorPickerDialog::OnClickedFacenormalColorButton()
{
	CColorDialog color_dialog(CG::FaceNormalColor);
	if (color_dialog.DoModal() == IDOK)
	{
		CG::FaceNormalColor = color_dialog.GetColor();
	}
}


void ColorPickerDialog::OnClickedBoundingboxColorButton()
{
	CColorDialog color_dialog(CG::BoundingBoxColor);
	if (color_dialog.DoModal() == IDOK)
	{
		CG::BoundingBoxColor = color_dialog.GetColor();
	}
}


void ColorPickerDialog::OnClickedModelColorButton()
{
	CColorDialog color_dialog(CG::ModelColor);
	if (color_dialog.DoModal() == IDOK)
	{
		CG::ModelColor = color_dialog.GetColor();
		CG::bIsModelColor = true;
	}
}


void ColorPickerDialog::OnClickedOriginalModelColorButton()
{
	CG::bIsModelColor = false;
}


void ColorPickerDialog::OnClickedDefaultColorsButton()
{
	CG::setDefaultColors();
}


void ColorPickerDialog::OnBnClickedBackgroundImageButton()
{
	BackgroundImageDialog dialog;
	dialog.DoModal();
}
