   // GLSView.h : interface of the CGLSView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLSVIEW_H__8676E6ED_B07A_4A94_B1AF_4924CBFA9058__INCLUDED_)
#define AFX_GLSVIEW_H__8676E6ED_B07A_4A94_B1AF_4924CBFA9058__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

template <class TYPE, class ARG_TYPE>
class UltraCArray_T : public CArray <TYPE, ARG_TYPE>
{
public:
void operator =(const UltraCArray_T<TYPE, ARG_TYPE> &rhs);

};

template <class TYPE, class ARG_TYPE>
void UltraCArray_T<TYPE, ARG_TYPE>
::operator =(const UltraCArray_T<TYPE, ARG_TYPE> &rhs)
{
RemoveAll();
Append(rhs);
}

UINT CommProckriging(LPVOID pParam);


class CGLSView : public CFormView
{
protected: // create from serialization only
	CGLSView();
	DECLARE_DYNCREATE(CGLSView)

public:
	//{{AFX_DATA(CGLSView)
	enum { IDD = IDD_GLS_FORM };
	CStatic	m_staticLocal;
	CButton	m_map;
	CButton	m_viewoutput;
	CButton	m_variogram;
	CStatic	m_progress;
	CEdit	m_localNum;
	CString	m_gridpath;
	CString	m_srcpath;
	//}}AFX_DATA

// Attributes
public:
	CGLSDoc* GetDocument();

// Operations
public:
	typedef UltraCArray_T<double, double&> DouTYPE1;
	DouTYPE1 m_Element;
	typedef UltraCArray_T<DouTYPE1, DouTYPE1&> DouTYPE2;
	DouTYPE2 m_PointArray2,m_PredictPoint2;
	DouTYPE2 m_PointArrayGrid2;
	DouTYPE2 m_ArrayForMedian;

  
	int m_columnNum,m_icolumnPara1,m_icolumnPara2,m_icolumnData,m_icolumnX,m_icolumnY,m_strData;

	int m_PointNum,m_ParaNum, m_GridPointNum;

	int m_estimators;

	CStdioFile Filesrc;
	CStdioFile Filegrid;
	CString m_str;
	CString m_strpath;
	CString m_strname;
	CString	m_fileRK;
	CString	m_fileGLS;
	CString	m_output;
	CString	m_filename;

	BOOL b_fopen1;
	BOOL b_fopen2;
	BOOL b_RunbtndownFlag;
	BOOL b_RunRKdownFlag;

	CArray<double,double> g_linearpara;
	CArray<double,double> m_Glinearpara;

	//结构体Var，h表示两点间的距离，var表示方差
	typedef struct tagVar
	{
		double h;
		double var;//方差
		double semv;//半方差
	}Var;

	//定义PHV是以结构体
	typedef CArray<Var,Var>PHV;
	PHV Phv;
    //最大距离
	double maxh;

	//结构体Semivar，hm表示对maxh进行分段后各段端点，semivar表示端点对应的半方差
	typedef struct tagSemivar
	{
		double hm;
		double semivar;
		int num;
	}Semivar;
	//PHR是以结构Semivar为数组元素的数组类
	typedef CArray<Semivar,Semivar>PHR;
	
	PHR Phr;

	//residual
	typedef struct tagPoint
	{
		double x;
		double y;
		double z;
		double var;
	} Point;
	Point m_point;
	CArray<Point,Point> m_Residual;

	//exponential function parameter
	double para[3];

	double Xmax,Xmin,Ymax,Ymin,Zmax,Zmin;
	double dxMax,dxMin,dyMin,dyMax;

	CArray<Point,Point> m_PointArrayGridNew;
	typedef struct tagFinalPoint
	{
		double x;
		double y;	
		double z;
		double para1;
		double para2;
		double para3;
		double reskrig;
		double regression;
		double varianceK;
		double variance1;
		double variance2;
		double variance;
		double bf[20];

	} FinalPoint;
	FinalPoint m_fianlpoint;

	CArray<FinalPoint,FinalPoint> m_fianlPredictPoint;//最终回归克里格的值
	CArray<FinalPoint,FinalPoint> m_tempFinal;
	CArray<double,double> varianceK;


	CDC* pControlDC; 	
	CDC memDC;            //内存绘图设备

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLSView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawWave(CDC *pDC,double x,double y);
	void predictLocalGLS();
	void FunGLS();
	void outputdataRK();
	void outputdataGLS();
	void GetResult();
	void Regression();
	void FunRK();
	void outputdataK();
	void trmul(double *a, double *b, int m, int n, int k, double *c);
	int rinv(double *a, int n);
	void predict();
	void levmar();
	void Calc_semvdistance();
	void Calc_vario();
	void Calc_semvdistance_median();
	void FunKriging();
	void GetResidual();
	int ReadData();
	virtual ~CGLSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGLSView)
	afx_msg void OnBtnSrc();
	afx_msg void OnBtnGrid();
	afx_msg void OnBtnSeedata();
	afx_msg void OnBtnRunkriging();
	afx_msg void OnBtnRunrk();
	afx_msg void OnBtnRungls();
	afx_msg void OnRadioGlobal();
	afx_msg void OnRadioLocal();
	afx_msg void OnBtnVariogram();
	afx_msg void OnBtnMap();
	afx_msg void OnBtnViewoutput();
	afx_msg void OnBtnOutput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GLSView.cpp
inline CGLSDoc* CGLSView::GetDocument()
   { return (CGLSDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLSVIEW_H__8676E6ED_B07A_4A94_B1AF_4924CBFA9058__INCLUDED_)
