// GLSDoc.cpp : implementation of the CGLSDoc class
//

#include "stdafx.h"
#include "GLS.h"

#include "GLSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLSDoc

IMPLEMENT_DYNCREATE(CGLSDoc, CDocument)

BEGIN_MESSAGE_MAP(CGLSDoc, CDocument)
	//{{AFX_MSG_MAP(CGLSDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLSDoc construction/destruction

CGLSDoc::CGLSDoc()
{
	// TODO: add one-time construction code here

}

CGLSDoc::~CGLSDoc()
{
}

BOOL CGLSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGLSDoc serialization

void CGLSDoc::Serialize(CArchive& ar)
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
// CGLSDoc diagnostics

#ifdef _DEBUG
void CGLSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGLSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLSDoc commands
