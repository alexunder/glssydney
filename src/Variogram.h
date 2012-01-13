#if !defined(AFX_VARIOGRAM_H__C6C381A5_4D69_4B64_9CED_5AA090A16DA7__INCLUDED_)
#define AFX_VARIOGRAM_H__C6C381A5_4D69_4B64_9CED_5AA090A16DA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Variogram.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVariogram dialog
#include <afxtempl.h>
class CVariogram : public CDialog
{
// Construction
public:
	CVariogram(CWnd* pParent = NULL);   // standard constructor
	typedef struct tagPoint
	{
		double x;
		double y;
	} Point;
	Point m_point;
	CArray<Point,Point> m_PointArray; // 用于记录坐标点数据
    Point m_PointCurve[50];
	
	int m_PointNum;   // 用于记录坐标点个数，在视图类构造函数中初始化为0.
	double para[3];
	double dxMax,dxMin,dyMin,dyMax;
// Dialog Data
	//{{AFX_DATA(CVariogram)
	enum { IDD = IDD_DLG_VARIAGRAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVariogram)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVariogram)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARIOGRAM_H__C6C381A5_4D69_4B64_9CED_5AA090A16DA7__INCLUDED_)
