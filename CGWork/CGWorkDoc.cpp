// CGWorkDoc.cpp : implementation of the CCGWorkDoc class
//

#include "stdafx.h"
#include "CGWork.h"

#include "CGWorkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCGWorkDoc

IMPLEMENT_DYNCREATE(CCGWorkDoc, CDocument)

BEGIN_MESSAGE_MAP(CCGWorkDoc, CDocument)
	//{{AFX_MSG_MAP(CCGWorkDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCGWorkDoc construction/destruction

CCGWorkDoc::CCGWorkDoc()
{
	// TODO: add one-time construction code here

}

CCGWorkDoc::~CCGWorkDoc()
{
}

BOOL CCGWorkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkDoc serialization

void CCGWorkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkDoc diagnostics

#ifdef _DEBUG
void CCGWorkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCGWorkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCGWorkDoc commands
