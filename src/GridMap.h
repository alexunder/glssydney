#if !defined(AFX_GRIDMAP_H__F0A094B1_F6D9_485E_B005_EFBB849E4C43__INCLUDED_)
#define AFX_GRIDMAP_H__F0A094B1_F6D9_485E_B005_EFBB849E4C43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridMap dialog
#include <afxtempl.h>

class CGridMap : public CDialog
{
// Construction
public:
	CGridMap(CWnd* pParent = NULL);   // standard constructor
	typedef struct tagPoint
	{
		double x;
		double y;
		double z;
	} Point;
	Point m_point;
	CArray<Point,Point> m_PointArray; // 用于记录坐标点数据
	double dxMax,dxMin,dyMin,dyMax,dzMax,dzMin;
// Dialog Data
	//{{AFX_DATA(CGridMap)
	enum { IDD = IDD_DLG_MAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridMap)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDMAP_H__F0A094B1_F6D9_485E_B005_EFBB849E4C43__INCLUDED_)
