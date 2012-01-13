// GridMap.cpp : implementation file
//

#include "stdafx.h"
#include "GLS.h"
#include "GridMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridMap dialog


CGridMap::CGridMap(CWnd* pParent /*=NULL*/)
	: CDialog(CGridMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridMap)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT
}


void CGridMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridMap)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	m_PointArray.RemoveAll();
	m_PointArray.SetSize(0);
	dyMax=1E-15;dyMin=1E+15;
	dxMax=1E-15;dxMin=1E+15;
	dzMax=1E-15;dzMin=1E+15;

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridMap, CDialog)
	//{{AFX_MSG_MAP(CGridMap)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridMap message handlers

BOOL CGridMap::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		int i;
	if (m_PointArray.GetSize()==0)
	{
		MessageBox("there is no data",NULL,MB_OK);
		OnCancel();
	}
	for(i=0;i<m_PointArray.GetSize();i++)  //找出最大最小值做为纵坐标的起始点，结束点;
	{
		if(m_PointArray[i].y>dyMax)
			dyMax=m_PointArray[i].y;
		if(m_PointArray[i].y<dyMin)
			dyMin=m_PointArray[i].y;
		if (m_PointArray[i].x>dxMax)
			dxMax = m_PointArray[i].x;
		if (m_PointArray[i].x<dxMin)
			dxMin = m_PointArray[i].x;
		if (m_PointArray[i].z>dzMax)
			dzMax = m_PointArray[i].z;
		if (m_PointArray[i].z<dzMin)
			dzMin = m_PointArray[i].z;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridMap::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
		if (m_PointArray.GetSize()==0)
	{
		MessageBox("there is no data",NULL,MB_OK);
		OnOK();
	}
	dc.MoveTo(40,20);
	dc.LineTo(40,290);
	dc.LineTo(430,290);	
	int Red,Green,Blue;
	CString str;
	for(int i=0;i<=4;i++)   //写纵坐标
	{
		str.Format(_T("%.3f"),(dyMax-dyMin)/5*(5-i)+dyMin);
		dc.TextOut(10,35+i*60,str,10);
		dc.MoveTo(35,30+i*60);
		dc.LineTo(40,30+i*60);
	}
	for(i=0;i<9;i++)  //写横坐标
	{
		str.Format(_T("%.3f"),(dxMax-dxMin)/8*i+dxMin);
		dc.TextOut(45+i*45,295,str,10);
		dc.MoveTo(50+i*45,290);
		dc.LineTo(50+i*45,295);
	}
	double x,y;
	
	for(i=1;i<m_PointArray.GetSize();i++)
	{  	
		x = 40 + (m_PointArray[i].x-dxMin)/(dxMax-dxMin)*360;
		y = 290-((m_PointArray[i].y-dyMin)/(dyMax-dyMin)*260);
		Green=(int)(m_PointArray[i].z-dzMin)/(dzMax-dzMin)*255;
		Blue=(int)255-Green;
		CRgn *rgn=new CRgn;
		CBrush *brush=new CBrush;
		rgn->CreateRectRgn(x,y,x+10,y+10);
		brush->CreateSolidBrush(RGB(((Green+Blue)/2),Green,Blue));//根据值改变颜色
		CBrush *oldbrush=dc.SelectObject(brush);
		dc.PaintRgn(rgn);
		dc.SelectObject(oldbrush);
		delete brush;
		delete rgn;	
	}
	// Do not call CDialog::OnPaint() for painting messages
}
