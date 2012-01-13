// GLSDoc.h : interface of the CGLSDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLSDOC_H__15891A7D_B8AF_4FB0_AEFA_89E728A4B2C2__INCLUDED_)
#define AFX_GLSDOC_H__15891A7D_B8AF_4FB0_AEFA_89E728A4B2C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGLSDoc : public CDocument
{
protected: // create from serialization only
	CGLSDoc();
	DECLARE_DYNCREATE(CGLSDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLSDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGLSDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLSDOC_H__15891A7D_B8AF_4FB0_AEFA_89E728A4B2C2__INCLUDED_)
