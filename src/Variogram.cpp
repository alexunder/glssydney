// Variogram.cpp : implementation file
//

#include "stdafx.h"
#include "GLS.h"
#include "Variogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVariogram dialog

#include <math.h>
CVariogram::CVariogram(CWnd* pParent /*=NULL*/)
	: CDialog(CVariogram::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVariogram)
		// NOTE: the ClassWizard will add member initialization here
	m_PointArray.RemoveAll();
	m_PointArray.SetSize(0);
	dyMax=1E-15;dyMin=1E+15;
	dxMax=1E-15;dxMin=1E+15;
	//}}AFX_DATA_INIT
}


void CVariogram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVariogram)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVariogram, CDialog)
	//{{AFX_MSG_MAP(CVariogram)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVariogram message handlers

BOOL CVariogram::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
			int i;
	if (m_PointArray.GetSize()==0)
	{
		MessageBox("there is no data",NULL,MB_OK);
		OnCancel();
	}
	for(i=0;i<m_PointArray.GetSize();i++)  //find max value and min value ;
	{
		if(m_PointArray[i].y>dyMax)
			dyMax=m_PointArray[i].y;
		if(m_PointArray[i].y<dyMin)
			dyMin=m_PointArray[i].y;
		if (m_PointArray[i].x>dxMax)
			dxMax = m_PointArray[i].x;
		if (m_PointArray[i].x<dxMin)
			dxMin = m_PointArray[i].x;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVariogram::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
		int i;
	CString str;

	str.Format(_T("%.7f"),para[0]);
	str = "C0 = "+str;
	dc.TextOut(450,130,str);
	str.Format(_T("%.7f"),para[1]);
	str = "C1 = "+str;
	dc.TextOut(450,160,str);
	str.Format(_T("%.7f"),para[2]);
	str = "r = "+str;
	dc.TextOut(450,190,str);



	dc.MoveTo(40,20);
	dc.LineTo(40,290);
	dc.LineTo(430,290);	

	for(i=0;i<=4;i++)   //x coordinate 
	{
		str.Format(_T("%.7f"),dyMin+(dyMax-dyMin)/5*(5-i));
		dc.TextOut(10,35+i*60,str,4);
		dc.MoveTo(35,30+i*60);
		dc.LineTo(40,30+i*60);
	}
	for(i=0;i<9;i++)  //y coordinate 
	{
		str.Format(_T("%.7f"),dxMin+(dxMax-dxMin)/8*i);
		dc.TextOut(45+i*45,295,str,4);
		dc.MoveTo(50+i*45,290);
		dc.LineTo(50+i*45,295);
	}
	double x,y;
	
	for(i=1;i<m_PointNum;i++)
	{  
		x = 40 + (m_PointArray[i].x-dxMin)/(dxMax-dxMin)*360;
		y = 290-((m_PointArray[i].y-dyMin)/(dyMax-dyMin)*260);
		dc.Ellipse(x-3,y-3,x+3,y+3);
 	}
 	x=40 + (m_PointArray[1].x-dxMin)/(dxMax-dxMin)*360;
	y = para[0]+para[1]*(1-exp(-m_PointArray[1].x/para[2]));
	y=(290-(y-dyMin)/(dyMax-dyMin)*260);
	dc.MoveTo(x,y);	
	for (i=2;i<m_PointNum;i++)
	{
		x=40 + (m_PointArray[i].x-dxMin)/(dxMax-dxMin)*360;
		y = para[0]+para[1]*(1-exp(-m_PointArray[i].x/para[2]));
		y=(290-(y-dyMin)/(dyMax-dyMin)*260);
		
		dc.LineTo(x,y);
	}
	// Do not call CDialog::OnPaint() for painting messages
}
