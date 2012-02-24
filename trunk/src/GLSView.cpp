          // GLSView.cpp : implementation of the CGLSView class
//

#include "stdafx.h"
#include "GLS.h"

#include "GLSDoc.h"
#include "GLSView.h"
#include "Variogram.h"
#include "GridMap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <afxmt.h>
// for MultiThread
CEvent eventdraw;
#include <math.h>
extern   "C"     
{   
#include "lm.h"
}
#define MAX 50
/////////////////////////////////////////////////////////////////////////////
// CGLSView

IMPLEMENT_DYNCREATE(CGLSView, CFormView)

BEGIN_MESSAGE_MAP(CGLSView, CFormView)
	//{{AFX_MSG_MAP(CGLSView)
	ON_BN_CLICKED(IDC_BTN_SRC, OnBtnSrc)
	ON_BN_CLICKED(IDC_BTN_GRID, OnBtnGrid)
	ON_BN_CLICKED(IDC_BTN_SEEDATA, OnBtnSeedata)
	ON_BN_CLICKED(IDC_BTN_RUNKRIGING, OnBtnRunkriging)
	ON_BN_CLICKED(IDC_BTN_RUNRK, OnBtnRunrk)
	ON_BN_CLICKED(IDC_BTN_RUNGLS, OnBtnRungls)
	ON_BN_CLICKED(IDC_RADIO_GLOBAL, OnRadioGlobal)
	ON_BN_CLICKED(IDC_RADIO_LOCAL, OnRadioLocal)
	ON_BN_CLICKED(IDC_BTN_VARIOGRAM, OnBtnVariogram)
	ON_BN_CLICKED(IDC_BTN_MAP, OnBtnMap)
	ON_BN_CLICKED(IDC_BTN_VIEWOUTPUT, OnBtnViewoutput)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, OnBtnOutput)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLSView construction/destruction

CGLSView::CGLSView()
	: CFormView(CGLSView::IDD)
{
	//{{AFX_DATA_INIT(CGLSView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_PointNum = 0;
	m_GridPointNum = 0;
	m_ParaNum =0;
	m_columnNum = 0;
	m_icolumnPara1=0;
	m_icolumnPara2=0;
	m_icolumnData = 0;
	m_icolumnX=0;
	m_icolumnY=0;

	Xmax=1E-15;Xmin=1E15;
	Ymax=1E-15;Ymin=1E15;
	Zmax=1E-15;Zmin=1E15;
	dyMax=1E-15;dyMin=1E+15;
	dxMax=1E-15;dxMin=1E+15;

}

CGLSView::~CGLSView()
{
}

void CGLSView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGLSView)
	DDX_Control(pDX, IDC_STATIC_LOCAL, m_staticLocal);
	DDX_Control(pDX, IDC_BTN_MAP, m_map);
	DDX_Control(pDX, IDC_BTN_VIEWOUTPUT, m_viewoutput);
	DDX_Control(pDX, IDC_BTN_VARIOGRAM, m_variogram);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_EDIT_LOCALNUM, m_localNum);
	//}}AFX_DATA_MAP
}

BOOL CGLSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CGLSView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_output = "E:";
	m_filename = "swkriged.txt";
	m_fileRK = "swRK.txt";
	m_fileGLS = "swGLS.txt";
	SetDlgItemText(IDC_EDIT_FILERK,m_fileRK);
	SetDlgItemText(IDC_EDIT_FILEGLS,m_fileGLS);

	SetDlgItemText(IDC_EDIT_FILE,m_filename);
	SetDlgItemText(IDC_EDIT_OUTPUT,m_output);
	SetDlgItemText(IDC_EDIT_LOCALNUM,"100");
	SetDlgItemText(IDC_EDIT_DISTANCE,"2000");
	SetDlgItemText(IDC_EDIT_ITERATE,"1");
	m_localNum.EnableWindow(FALSE);
	m_progress.ShowWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO_GLOBAL))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO_LOCAL))->SetCheck(FALSE);
	
	((CButton *)GetDlgItem(IDC_CHECK_XY))->SetCheck(FALSE);
	((CComboBox*)GetDlgItem(IDC_COMBO_WEIGHT))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_ESTIMATORS))->SetCurSel(0);

	b_fopen1 =FALSE;
	b_fopen2 = FALSE;	

}

/////////////////////////////////////////////////////////////////////////////
// CGLSView printing

BOOL CGLSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGLSView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CGLSView diagnostics

#ifdef _DEBUG
void CGLSView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGLSView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGLSDoc* CGLSView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLSDoc)));
	return (CGLSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLSView message handlers
UINT CommProckriging(LPVOID pParam)
{
	CGLSView *pView=(CGLSView *)pParam;
	pView->FunKriging();
	return 0;
}
UINT CommProcRK(LPVOID pParam)
{
	CGLSView *pView=(CGLSView *)pParam;
	pView->FunRK();
	return 0;
}
UINT CommProcGLS(LPVOID pParam)
{
	CGLSView *pView=(CGLSView *)pParam;
	pView->FunGLS();
	return 0;
}

void CGLSView::OnBtnSrc() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE); 
    if(dlg.DoModal()==IDOK)
    {
		m_srcpath = dlg.GetPathName(); 
		SetDlgItemText(IDC_EDIT_SRCPATH,m_srcpath);
	}
	else
	{
		return;
	}
	b_fopen1 = FALSE;
	b_fopen2 =FALSE; 
}

void CGLSView::OnBtnGrid() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE); 
    if(dlg.DoModal()==IDOK)
    {
		m_gridpath = dlg.GetPathName(); 
		SetDlgItemText(IDC_EDIT_GRIDPATH,m_gridpath);
	}
	else
	{
		return;
	}
	b_fopen1 = FALSE;
	b_fopen2 =FALSE; 
}

void CGLSView::OnBtnSeedata() 
{
	// TODO: Add your control notification handler code here
	m_columnNum = 0;
	GetDlgItemText(IDC_EDIT_SRCPATH,m_srcpath);
	if (m_srcpath.IsEmpty())
	{
		MessageBox("Please Choose data","warning",MB_OK);
	}
	else
	{
		if (!b_fopen1)
		{
			b_fopen1 = Filesrc.Open(m_srcpath,CFile::modeRead); 
		}
		//display the format of data
		Filesrc.ReadString(m_str);
		SetDlgItemText(IDC_STATIC_DATAVIEW,m_str);
		
		//get the total No.columns
		int n=0,m=0,p=0;
		do 
		{
			n= m_str.Find("\t",n+1);
			m=m_str.Find(" ",m+1);
			p=m_str.Find(",",p+1);
			m_columnNum++;
		} while(n!= -1||m!=-1||p!=-1);
		m_str.Format("%d",m_columnNum);
		SetDlgItemText(IDC_EDIT_NoCols,m_str);
	}	
}
void CGLSView::OnBtnRunkriging() 
{
	// TODO: Add your control notification handler code here
	CWinThread *pRunkriging =AfxBeginThread(CommProckriging,
		this,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pRunkriging->ResumeThread();
}

int CGLSView::ReadData()
{
	int i;
	m_PointArray2.RemoveAll();
	m_PointArray2.SetSize(0);
	m_PointArrayGrid2.RemoveAll();
	m_PointArrayGrid2.SetSize(0);
	if (!b_fopen1)
	{
		if (m_srcpath.IsEmpty())
		{
			MessageBox("Please Choose data","warning",MB_OK);
			return 0;
		}
		else
		{
			
			b_fopen1 = Filesrc.Open(m_srcpath,CFile::modeRead); 
			
			
			//display the format of data
			Filesrc.ReadString(m_str);
			SetDlgItemText(IDC_STATIC_DATAVIEW,m_str);
			
			//get the total No.columns
			int n=0,m=0,p=0;
			m_columnNum = 0;
			do 
			{
				n= m_str.Find("\t",n+1);
				m=m_str.Find(" ",m+1);
				p=m_str.Find(",",p+1);
				m_columnNum++;
			} while(n!= -1||m!=-1||p!=-1);
			m_str.Format("%d",m_columnNum);
			SetDlgItemText(IDC_EDIT_NoCols,m_str);
		}
	}
	Filesrc.SeekToBegin();
	CString FileData; 
	m_PointNum = 0;
	while (Filesrc.ReadString(FileData)!=FALSE)
	{
		m_PointNum++;  // get the number of data
	}
	if (m_PointNum == 0)
	{
		MessageBox("read data failed",NULL,MB_OK);
		return 0;
	}
	
	//get the column of X Y data parameter 
	CString m_strX,m_strY,m_strData,m_strPara1,m_strPara2;
	GetDlgItemText(IDC_EDIT_XCol,m_strX);
	GetDlgItemText(IDC_EDIT_YCol,m_strY);
	GetDlgItemText(IDC_EDIT_DataCol,m_strData);
	GetDlgItemText(IDC_EDIT_PARA1,m_strPara1);
	GetDlgItemText(IDC_EDIT_PARA2,m_strPara2);
	
	if (m_strX.IsEmpty()||m_strY.IsEmpty()||m_strData.IsEmpty())
	{
		MessageBox("Please fill the columns of X,Y,Data","warning",MB_OK);
		return 0;
	}
	else
	{
		m_icolumnX = atoi(m_strX);
		m_icolumnY = atoi(m_strY);
		m_icolumnData = atoi(m_strData);
		m_icolumnPara1 = atoi(m_strPara1);
		m_icolumnPara2 = atoi(m_strPara2);
		
		m_ParaNum = m_icolumnPara2 - m_icolumnPara1+1;
		
		if (m_icolumnData>m_columnNum||m_icolumnX>m_columnNum||m_icolumnY>m_columnNum||m_icolumnPara1>m_columnNum||m_icolumnPara2>m_columnNum)
		{
			MessageBox("Sorry,input is wrong","warning",MB_OK);
			return 0;
		}
		CString m_strleft,m_strright;
		double temp ;
		Filesrc.SeekToBegin();	

		for (i = 0;i<m_PointNum;i++)
		{
			Filesrc.ReadString(FileData);
			m_strright = FileData;
			m_Element.RemoveAll();
			m_Element.SetSize(0);
			for(int j =0 ;j<m_columnNum-1;j++)
			{	
				m_strleft = m_strright.Left(m_strright.Find('\t'));
				m_strright = m_strright.Right(m_strright.GetLength()-m_strright.Find('\t')-1);	
				temp =atof(m_strleft);
				m_Element.Add(temp);
			}
			temp = atof(m_strright);
			m_Element.Add(temp);
			if (((CButton *)GetDlgItem(IDC_CHECK_XY))->GetCheck())
			{
				double X = m_Element.GetAt(m_icolumnX -1);
				double Y = m_Element.GetAt(m_icolumnY-1);
				double XY = X*Y;
				m_Element.InsertAt(m_icolumnData-1,X);
				m_Element.InsertAt(m_icolumnData,Y);
				m_Element.InsertAt(m_icolumnData+1,XY);
			}
			m_PointArray2.Add(m_Element);//array of observe data
		}
		Filesrc.Close();
		//read grid data
		if (m_gridpath.IsEmpty())
		{
			MessageBox("Please Choose grid file","warning",MB_OK);
			return 0;
		}
		else
		{
			if (!b_fopen2)
			{	
				b_fopen2 = Filegrid.Open(m_gridpath,CFile::modeRead); 
			}
		}
		Filegrid.SeekToBegin();
		m_GridPointNum = 0;
		while (Filegrid.ReadString(FileData)!=FALSE)
		{
			m_GridPointNum++;  // get the number of data
		}
		if (m_GridPointNum == 0)
		{
			MessageBox("read grid data failed",NULL,MB_OK);
			return 0;
		}
		Filegrid.SeekToBegin();
		Filegrid.ReadString(FileData);
		int n=0,m=0,m_colNum=0;
		do 
		{
			n= FileData.Find("\t",n+1);
			m=FileData.Find(" ",m+1);
			m_colNum++;
		} while(n!= -1||m!=-1);
		Filegrid.SeekToBegin();	
		for (i = 0;i<m_GridPointNum;i++)
		{
			Filegrid.ReadString(FileData);
			m_strright = FileData;
			m_Element.RemoveAll();
			m_Element.SetSize(0);
			for(int j =0 ;j<(m_colNum<m_columnNum?m_colNum-1:m_columnNum-1);j++)
			{	
				m_strleft = m_strright.Left(m_strright.Find('\t'));
				m_strright = m_strright.Right(m_strright.GetLength()-m_strright.Find('\t')-1);	
				temp =atof(m_strleft);
				m_Element.Add(temp);
			}
			temp = atof(m_strright);
			m_Element.Add(temp);
			if (((CButton *)GetDlgItem(IDC_CHECK_XY))->GetCheck())
			{
				double X = m_Element.GetAt(m_icolumnX -1);
				double Y = m_Element.GetAt(m_icolumnY-1);
				double XY = X*Y;
				m_Element.InsertAt(m_icolumnData-1,X);
				m_Element.InsertAt(m_icolumnData,Y);
				m_Element.InsertAt(m_icolumnData+1,XY);
			}
			m_PointArrayGrid2.Add(m_Element);//array of grid data
		}
		
	}
	if (((CButton *)GetDlgItem(IDC_CHECK_XY))->GetCheck())
	{
		m_ParaNum+=3;
		m_icolumnData+=3;
	}

	Filegrid.Close();
	
	b_fopen2 =FALSE;
	b_fopen1 =FALSE;
	return 1;
}


void CGLSView::GetResidual()
{
	m_Residual.RemoveAll();
	m_Residual.SetSize(0);
	double z;
	for (int i =0;i<m_PointNum;i++)
	{	
		z = 0;
		m_point.x = m_PointArray2[i][m_icolumnX-1];
		m_point.y = m_PointArray2[i][m_icolumnY-1];
		if (g_linearpara.GetSize()!=0)
		{
			for (int j =0;j<m_ParaNum;j++)
			{
				z = z + g_linearpara[j+1]*m_PointArray2[i][m_icolumnPara1-1+j];
			}
			z = z+ g_linearpara[0];
			
		}
		m_point.z = m_PointArray2[i][m_icolumnData-1]-z;
	 	m_Residual.Add(m_point);
	}

}


void CGLSView::FunKriging()
{
	b_RunbtndownFlag =TRUE;
	int n = ReadData()	;
	g_linearpara.RemoveAll();
	g_linearpara.SetSize(0);
	m_ParaNum =0;
	for (int i=0;i<=m_ParaNum;i++)
	{
		g_linearpara.Add(0);
	}
	if(n)
	{
		
		GetResidual();
		
		Calc_vario();
		
		if (m_estimators)
		{
			Calc_semvdistance_median();
		} 
		else
		{
			Calc_semvdistance();
		}
		
		levmar();
		
		predict();
 
 		outputdataK();
		
		MessageBox("kriging finished",NULL,MB_OK);

		m_map.EnableWindow(TRUE);
		m_viewoutput.EnableWindow(TRUE);
		m_variogram.EnableWindow(TRUE);
	}
	return;
}

void CGLSView::Calc_vario()
{
	Phv.RemoveAll();
	Phv.SetSize(0);
	double xi,yi,xj,yj;
	double a,b,c,d,distance=0,r2,u,v;
	int i,j;						//???????
	Var var;						//????????
	for (i = 0 ;i < m_Residual.GetSize(); i++ )
	{
		xi = m_Residual.GetAt(i).x;
		yi = m_Residual.GetAt(i).y;
		for(j=1;j<m_Residual.GetSize()-i;j++)
		{
			xj = m_Residual.GetAt(i+j).x;
			yj = m_Residual.GetAt(i+j).y;
			
			
			distance=::sqrt(::pow((xi-xj),2)+::pow((yi-yj),2));
			if(distance>maxh)
				maxh = distance;
		}
	}
	m_estimators = 	((CComboBox*)GetDlgItem(IDC_COMBO_ESTIMATORS))->GetCurSel();
	maxh = maxh*0.8;
	for (i = 0 ;i < m_Residual.GetSize(); /*i=i+10*/i++ )
	{
 		m_point=m_Residual.GetAt(i);
		a=m_point.x;
		b=m_point.y;
		u=m_point.z;

		for(j=1;j<m_Residual.GetSize()-i;/*j=j+10*/j++)//???
		{
			m_point=m_Residual.GetAt(i+j);
			c=m_point.x;
			d=m_point.y;
			v=m_point.z;
			
// 			if(distance>maxh)
// 				maxh = distance;//?????
			distance=::sqrt(::pow((a-c),2)+::pow((b-d),2));//????
			if(distance > maxh) 
				continue;

			if (m_estimators)
			{
				r2=u-v;
				r2=fabs(r2);
			} 
			else
			{
				r2=::pow((u-v),2);//????
			}
			var.h=distance;
			var.var=r2;
			var.semv = r2/2;
			Phv.Add(var);
		}
	}
// 	CString str;
// 	str.Format("%f",maxh);
// 	SetDlgItemText(IDC_EDIT_DISTANCE,str);
}
void CGLSView::Calc_semvdistance_median()
{
	int i;
	int n;

	Phr.RemoveAll();
	Phr.SetSize(0);
	m_ArrayForMedian.RemoveAll();
	m_ArrayForMedian.SetSize(0);
	int nlag = MAX; // section
	double m_tolag ;//  toleration is 0.5 of step 
	double m_mediansemv[MAX]; //each section meansemv
	double m_sumhm[MAX];
	int m_count[MAX];    //each section pair
	Semivar semivar;
	for (i = 0;i< MAX ;i++)
	{
		m_count[i] = 0;
		m_mediansemv[i] = 0;
		m_sumhm[i]=0;
	}
	CString strDistance;
	GetDlgItemText(IDC_EDIT_DISTANCE,strDistance);
	maxh = atof(strDistance);
	double m_step = maxh /(nlag+1);//the length of step
	m_tolag = m_step*0;//???????????????????????????

	for (n=0;n<nlag;n++)
	{
		m_Element.RemoveAll();
		m_Element.SetSize(0);

		for (i =0;i<Phv.GetSize();i++)	
		{
			semivar.hm = Phv.GetAt(i).h;
			semivar.semivar= Phv.GetAt(i).var;
			if(semivar.hm /m_step < n+1)
			{
				m_Element.Add(semivar.semivar);
				m_count[n]++;
				m_sumhm[n]+= semivar.hm;
				Phv.RemoveAt(i);
				i--;
				
			}
		}
		m_ArrayForMedian.Add(m_Element);
	}

	//seqencing to find median value

	for ( n=0;n<nlag;n++)
	{
		for(int i=0; i<m_count[n]-1; i++) 
		{ 
			for(int j=i+1; j<m_count[n]; j++) 
			{ 
				if (m_ArrayForMedian[n][i]>m_ArrayForMedian[n][j])
				{
					double t=m_ArrayForMedian[n][i]; 
					m_ArrayForMedian[n][i]=m_ArrayForMedian[n][j]; 
					m_ArrayForMedian[n][j]=t;
				}	
			} 			
		} 
		if (m_count[n]==0)
		{
			continue;
		}
		if (m_count[n]%2==0&&m_count[n]>=2)
		{
			m_mediansemv[n]=(m_ArrayForMedian[n][m_count[n]/2]+m_ArrayForMedian[n][(m_count[n]-2)/2])/2;
		} 
		else
		{
			m_mediansemv[n]=m_ArrayForMedian[n][m_count[n]/2];
		}


		m_mediansemv[n] = pow(m_mediansemv[n],2);
		m_mediansemv[n]= 2.198/2*m_mediansemv[n];
		m_sumhm[n] =m_sumhm[n]/m_count[n];

		semivar.hm = m_sumhm[n];
		semivar.semivar = m_mediansemv[n];
		semivar.num =m_count[n];
		if (semivar.hm!=0)
		{
			Phr.Add(semivar);
		}	

	}
	dyMax=1E-15;dyMin=1E+15;
	dxMax=1E-15;dxMin=1E+15;
	
	for(i=0;i<Phr.GetSize();i++)  //find max value and min value ;
	{
		if(Phr[i].semivar>dyMax)
			dyMax=Phr[i].semivar;
		if(Phr[i].semivar<dyMin)
			dyMin=Phr[i].semivar;
		if (Phr[i].hm>dxMax)
			dxMax = Phr[i].hm;
		if (Phr[i].hm<dxMin)
			dxMin = Phr[i].hm;
	}

}

void CGLSView::Calc_semvdistance()
{ 
	Phr.RemoveAll();
	Phr.SetSize(0);
	int nlag = MAX; // section
	double m_tolag ;//  toleration is 0.5 of step 
	double m_sumsemv[MAX]; //each section meansemv
	double m_sumhm[MAX];
	int m_count[MAX];    //each section pair
	int i;

	Semivar semivar;
	for (i = 0;i< MAX ;i++)
	{
		m_count[i] = 0;
		m_sumsemv[i] = 0;
		m_sumhm[i]=0;
	}
	CString strDistance;
	GetDlgItemText(IDC_EDIT_DISTANCE,strDistance);
	//maxh = atof(strDistance);
	double m_step = maxh /(nlag);//the length of step
	m_tolag = m_step*0;//??????????
	int nn=0;
	for (i =0;i<Phv.GetSize();i++)	
	{
		semivar.hm = Phv.GetAt(i).h;
		semivar.semivar =Phv.GetAt(i).semv;
		
		for (int n=1;n<=nlag;n++)
		{
		
			if (semivar.hm<(n*m_step+m_tolag))
			{
				nn++;
				m_count[n-1]++;
				m_sumsemv[n-1] +=semivar.semivar;
				m_sumhm[n-1]+= semivar.hm;
				break;
			}
		}			 
	}	
// ??????????
// 	for (int n=0;n<nlag;n++)
// 	{
// 		for (i =0;i<Phv.GetSize();i++)	
// 		{
// 			semivar.hm = Phv.GetAt(i).h;
// 			semivar.semivar= Phv.GetAt(i).semv;
// 			if(semivar.hm /m_step < n+1)
// 			{
// 				m_count[n]++;
// 				m_sumsemv[n] +=semivar.semivar;
// 				m_sumhm[n]+= semivar.hm;
// 				Phv.RemoveAt(i);
// 				i--;
// 				
// 			}
// 		}
// 	}
	for (i=0;i<nlag;i++)
	{
		if (m_count[i]>0)
		{
			m_sumsemv[i] = m_sumsemv[i] /m_count[i];
			m_sumhm[i] =m_sumhm[i]/m_count[i];
		}
		else
		{
			m_sumsemv[i] =0;
			m_sumhm[i] =0;
		}	
		semivar.hm = m_sumhm[i];
		semivar.semivar = m_sumsemv[i];
		semivar.num =m_count[i];
		if (semivar.hm!=0)
		{
			Phr.Add(semivar);
		}	
	}
	dyMax=1E-15;dyMin=1E+15;
	dxMax=1E-15;dxMin=1E+15;
//---------------------
	CStdioFile   fobjGLS("c:\\variance.txt",   CFile::modeCreate|CFile::modeNoTruncate    |   CFile::modeWrite);  
	CString str;
	for (i =0;i<50;i++)
	{	
		str.Format("%f,%f\n",m_sumhm[i],m_sumsemv[i]);								   
		fobjGLS.WriteString(str);   
		
	}
	fobjGLS.Close();  
	for(i=0;i<Phr.GetSize();i++)  //find max value and min value ;
	{
		if(Phr[i].semivar>dyMax)
			dyMax=Phr[i].semivar;
		if(Phr[i].semivar<dyMin)
			dyMin=Phr[i].semivar;
		if (Phr[i].hm>dxMax)
			dxMax = Phr[i].hm;
		if (Phr[i].hm<dxMin)
			dxMin = Phr[i].hm;
	}

}
struct mydata 
{
	double h[50];
	double r[50];
	int count[50];
};
struct mydata m_data;
int m_weight;

/* model to be fitted to measurements: x_i = p[0]*exp(-p[1]*i) + p[2], i=0...n-1 */
void expfunc(double *p, double *x, int m, int n, void *data)
{
  register int i;
  for(i=0; i<n; ++i)
  {
		x[i]=p[0]+p[1]*(1-exp(-m_data.h[i]/p[2]));
		if(m_weight)
		{
			x[i] = x[i]*m_data.count[i];
		}
  }
}

void CGLSView::levmar()
{
	int k;
	const int n=Phr.GetSize(), m=3; // 50 measurements, 3 parameters
	double p[3], opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	double dbMax=0,dbMin=6;
	double nMaxDistance=0, meandis=0;
	m_weight = 	((CComboBox*)GetDlgItem(IDC_COMBO_WEIGHT))->GetCurSel();

	for (k = 0;k<50;k++)
	{
		m_data.h[k] = 0;
		m_data.r[k] = 0;
		m_data.count[k] = 0;
	}
	for ( k=0;k<Phr.GetSize();k++)
	{
		m_data.h[k] = Phr.GetAt(k).hm;
		m_data.r[k] = Phr.GetAt(k).semivar;
		m_data.count[k] = Phr.GetAt(k).num;
		if (m_weight)
		{
			m_data.r[k] = m_data.r[k] *m_data.count[k];
		}
		if(Phr.GetAt(k).semivar>dbMax)
			dbMax=Phr.GetAt(k).semivar;
		if(Phr.GetAt(k).semivar<dbMin)
			if(Phr.GetAt(k).semivar!=0)
				dbMin=Phr.GetAt(k).semivar;	
		if(Phr.GetAt(k).hm>nMaxDistance)
			nMaxDistance=Phr.GetAt(k).hm;
		meandis+= Phr.GetAt(k).hm;
	}
	meandis = meandis/Phr.GetSize();

	/* initial parameters estimate: (1.0, 0.0, 0.0) */
	p[0]=dbMin; p[1]=dbMax-dbMin; p[2]=meandis;
	
	/* optimization control parameters; passing to levmar NULL instead of opts reverts to defaults */
	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
	opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference Jacobian version is used 
	
    double lb[3], ub[3];

    lb[0]=0; lb[1]=0.01; lb[2]=1E-15;
    ub[0]=dbMax/2; ub[1]=dbMax; ub[2]=10*nMaxDistance;

	/* invoke the optimization function */
//	ret=dlevmar_dif(expfunc, p, m_data.r, m, n, 1000, opts, info, NULL, NULL,m_data.h); // without Jacobian
	dlevmar_bc_dif(expfunc, p, m_data.r, m, n,lb,ub, 1000, opts, info, NULL, NULL,m_data.h);
	para[0] = p[0];
	para[1] = p[1];
	para[2] = p[2];
// 	if(p[2]==0)
// 		para[2]=1e-10;
}

void CGLSView::predict()
{
	m_PointArrayGridNew.RemoveAll();
	m_PointArrayGridNew.SetSize(0);
	m_PredictPoint2.RemoveAll();
	m_PredictPoint2.SetSize(0);
	varianceK.RemoveAll();
	varianceK.SetSize(0);
	//??????????
	int I, i,j,n,N;
	double dist=0,hh=0,Gx=0,Gy=0,rr=0,VarEsum;
	int GetNum =100;

		for(i=0;i<m_PointArray2.GetSize();i++)//???
		{   
// 			dist=::sqrt(::pow((m_PointArrayGrid2[I][m_icolumnX-1]-m_PointArray2[i][m_icolumnX-1]),2)+
// 				::pow((m_PointArrayGrid2[I][m_icolumnY-1]-m_PointArray2[i][m_icolumnY-1]),2));
// 			if(dist<100)//?????????h??????m_PredictPoint????,????
// 			  m_PredictPoint2.Add(m_PointArray2[i]);//global
			if(m_PointArray2[i][m_icolumnY-1]>Ymax)
				Ymax=m_PointArray2[i][m_icolumnY-1];
			if(m_PointArray2[i][m_icolumnY-1]<Ymin)
				Ymin=m_PointArray2[i][m_icolumnY-1];
			if (m_PointArray2[i][m_icolumnX-1]>Xmax)
				Xmax = m_PointArray2[i][m_icolumnX-1];
			if (m_PointArray2[i][m_icolumnX-1]<Xmin)
				Xmin = m_PointArray2[i][m_icolumnX-1];
			if (m_PointArray2[i][m_icolumnData-1]>Zmax)
				Zmax = m_PointArray2[i][m_icolumnData-1];
			if (m_PointArray2[i][m_icolumnData-1]<Zmin)
				Zmin = m_PointArray2[i][m_icolumnData-1];
		}
		dyMax= Ymax;dyMin = Ymin;dxMax =Xmax;dxMin = Xmin;
		for( i=0;i<m_PointArrayGrid2.GetSize();i++)  
		{
			if(m_PointArrayGrid2[i][m_icolumnY-1]>Ymax)
				Ymax=m_PointArrayGrid2[i][m_icolumnY-1];
			if(m_PointArrayGrid2[i][m_icolumnY-1]<Ymin)
				Ymin=m_PointArrayGrid2[i][m_icolumnY-1];
			if (m_PointArrayGrid2[i][m_icolumnX-1]>Xmax)
				Xmax = m_PointArrayGrid2[i][m_icolumnX-1];
			if (m_PointArrayGrid2[i][m_icolumnX-1]<Xmin)
				Xmin = m_PointArrayGrid2[i][m_icolumnX-1];
		}
		double area = (dxMax-dxMin)*(dyMax-dyMin);  //field??
		double times = m_PointArray2.GetSize()/GetNum;//??????????
		double perarea = area/((int)times+1);//?????
		double ratio = fabs(sqrt(perarea/3.14));//??
		double areaX1,areaX2,areaY1,areaY2;
		for (I=0;I<m_PointArrayGrid2.GetSize();I++)
		{	
			areaX1 = m_PointArrayGrid2[I][m_icolumnX-1]-ratio;
			areaX2 = ratio + m_PointArrayGrid2[I][m_icolumnX-1];
			areaY1 = m_PointArrayGrid2[I][m_icolumnY-1]-ratio;
			areaY2 = ratio + m_PointArrayGrid2[I][m_icolumnY-1];
			do 
			{
				m_PredictPoint2.RemoveAll();
				for (int J=0;J<m_PointArray2.GetSize();J++)
				{
					if (m_PointArray2[J][m_icolumnX-1]>areaX1&& m_PointArray2[J][m_icolumnX-1]<areaX2
						&&m_PointArray2[J][m_icolumnY-1]>areaY1&&m_PointArray2[J][m_icolumnY-1]<areaY2)
					{
						m_PredictPoint2.Add(m_PointArray2[J]);
					}		
				}
				areaX1 = areaX1 -ratio;
				areaX2 = areaX2+ratio;
				areaY1 = areaY1 -ratio;
				areaY2 = areaY2 +ratio;
			} while (m_PredictPoint2.GetSize()<GetNum);
			
			if (m_PredictPoint2.GetSize()>GetNum)
			{
				for (i=0;i<m_PredictPoint2.GetSize();i++)
				{
					m_Element = m_PredictPoint2[i];
					dist = sqrt(pow((m_PointArrayGrid2[I][m_icolumnX-1]-m_PredictPoint2[i][m_icolumnX-1]),2)
						+pow((m_PointArrayGrid2[I][m_icolumnY-1]-m_PredictPoint2[i][m_icolumnY-1]),2));
					m_Element.Add(dist);
					m_PredictPoint2.SetAt(i,m_Element);
				}
				
				
				//??
				for(int i=0; i<m_PredictPoint2.GetSize()-1; i++) 
				{ 
					for(int j=i+1; j<m_PredictPoint2.GetSize();j++) 
					{ 
						if (m_PredictPoint2[i][m_icolumnData]>m_PredictPoint2[j][m_icolumnData])
						{
							m_Element =m_PredictPoint2[i]; 
							m_PredictPoint2[i]=m_PredictPoint2[j]; 
							m_PredictPoint2[j]=m_Element;
						}	
					} 			
				} 
				N=m_PredictPoint2.GetSize();
			m_PredictPoint2.RemoveAt(GetNum,N-GetNum);
		}
		
		N=m_PredictPoint2.GetSize();//?????????
		n=N+1;//?????
		m_PointNum = m_PredictPoint2.GetSize();
		double **A=new double *[n];//????
		double *A1 = new double[n*n];
		double *p = new double[n];
		for(i=0;i<n;i++)
		{
			A[i]=new double[n];
		}
		double *B=new double[n];//????
		
		int ii;
		int jj;

		for(ii=0;ii<N;ii++)
		{
			for(jj=ii;jj<N;jj++)
			{
				if (ii==jj)
				{
					A[ii][jj] =0;//?????
				}
				else
				{
					hh=::sqrt(::pow((m_PredictPoint2[ii][m_icolumnX-1]-m_PredictPoint2[jj][m_icolumnX-1]),2)+::pow((m_PredictPoint2[ii][m_icolumnY-1]-m_PredictPoint2[jj][m_icolumnY-1]),2));
					rr=para[0]+para[1]*(1-exp(-hh/para[2]));
					A[ii][jj]=rr;
				}
				A[jj][ii] = A[ii][jj];
			}
			A[ii][jj]=1;//??????1
		}
		for(i=0;i<N;i++)
			A[N][i]=1;//??????1
		A[N][N]=0;  
		//????????
		int k = 0;
		for (i=0;i<n;i++)
		{
			for (j=0;j<n;j++)
			{
				A1[k++] = A[i][j];	
			}
		}
		rinv(A1,n);

		VarEsum =0;
		for(i=0;i<n;i++)
			p[i]=0;
		
		//??????????--?????
		Gx=m_PointArrayGrid2[I][m_icolumnX-1];
		Gy=m_PointArrayGrid2[I][m_icolumnY-1];//?????
		for(j=0;j<N;j++)
		{
			hh=::sqrt(::pow((m_PredictPoint2[j][m_icolumnX-1]-Gx),2)+::pow((m_PredictPoint2[j][m_icolumnY-1]-Gy),2));
			rr=para[0]+para[1]*(1-exp(-hh/para[2]));
			B[j]=rr;
		}
		B[j]=1;//?????????1           //????????
		trmul(A1,B,n,n,1,p);

		for(i=0;i<n;i++)
		{
			VarEsum += p[i]*B[i];
		}
		varianceK.Add(VarEsum);

		//?????????
		double Value=0;
		for(i=0;i<n-1;i++)
		{
			m_point = m_Residual.GetAt(i);
			Value+=p[i]*m_point.z;
		}
		
		m_point.x=m_PointArrayGrid2[I][m_icolumnX-1];
		m_point.y=m_PointArrayGrid2[I][m_icolumnY-1];
		m_point.z=Value;
		m_PointArrayGridNew.Add(m_point);
	
		CString tmp; 
		tmp.Format("%d/%d", I+1,m_GridPointNum); 
		m_progress.ShowWindow(TRUE);
		m_progress.SetWindowText(tmp); 
		for(i=0;i<n;i++)
		{
			delete[] A[i];
		}
		delete[] A; delete[] B,delete[] p,delete[] A1;
/////////////////////////////////////////////////////////
		double **M,**MT,*z,*bf;
		z = new double[m_PointNum];
		M = new double*[m_PointNum];
		MT= new double*[m_ParaNum+1];
		bf = new double[m_ParaNum+1];
		for ( i=0; i<m_PointNum; i++) 
		{
			M[i] = new double[m_ParaNum+1];
		}
		for (i=0;i<=m_ParaNum;i++)
		{
			MT[i] = new double[m_PointNum];
		}
		for (int j=0;j<m_PointNum;j++)
		{
			z[j]=m_PredictPoint2[j][m_icolumnData-1];
		}
		for (i=0;i<m_ParaNum+1;i++)
		{
			bf[i]=0;
		}
		
		for (i=0;i<m_PointNum;i++)
		{
			for (int j=0;j<=m_ParaNum;j++)
			{
				if (j==0)
				{
					M[i][j] =1;
				}
				else
				{
					M[i][j] = m_PredictPoint2[i][m_icolumnPara1-1+j-1];
				}
			}			
		}
		for(i=0;i<m_PointNum;i++) 
			for(int j=0;j<=m_ParaNum;j++)
				MT[j][i]=M[i][j]; 	//M ?????
			double *M1 = new double[m_PointNum*(m_ParaNum+1)];
			double *MT1 = new double[m_PointNum*(m_ParaNum+1)];//M ?????
			double *temp = new double[m_PointNum*(m_ParaNum+1)];
			double *temp1 = new double[(m_ParaNum+1)*(m_ParaNum+1)];
			double *temp2 = new double[m_PointNum*(m_ParaNum+1)];
			double k0 = para[0]+para[1];

			k = 0;
			for (i=0;i<m_PointNum;i++)
			{
				for (int j=0;j<=m_ParaNum;j++)
				{
					M1[k++] = M[i][j];	
				}
			}
			k=0;
			for ( i=0;i<=m_ParaNum;i++)
			{
				for (j=0;j<m_PointNum;j++)
				{
					MT1[k++] = MT[i][j];	
				}
			}
			//iteration 
			double Z;
			double finalZ =0;
			double **K = new double*[m_PointNum];
			double *K1 = new double[m_PointNum*m_PointNum];
			for (i=0;i<m_PointNum;i++)
			{
				K[i] = new double[m_PointNum];
			}
			
			double h =0;
			
			double *sk = new double[m_PointNum];
			double *temp3 = new double[m_PointNum];
			double *temp4 = new double;
			double *temp5 =new double[m_ParaNum+1];	
			double *temp6 =new double[m_ParaNum+1];	
			double *m= new double[m_ParaNum+1];

		//k0 -kt*K-1*k+dt*(MT*K-1*M)-1*d
		if (m_PointArrayGridNew.GetSize()==0)
		{
			return;
		}
		//?K
		for (i=0;i<m_PointNum;i++)
		{
			for (j=i;j<m_PointNum;j++)
			{
				if (i==j)
				{
					K[i][j] = para[0]+para[1];//?????
				}
				else
				{
					h=::sqrt(::pow((m_PointArray2[i][m_icolumnX-1]-m_PointArray2[j][m_icolumnX-1]),2)+::pow((m_PointArray2[i][m_icolumnY-1]-m_PointArray2[j][m_icolumnY-1]),2));
					K[i][j] = para[1]*exp(-h/para[2]);
				}
				K[j][i] = K[i][j];
			}
		}
		k=0;
		for ( i=0;i<m_PointNum;i++)
		{
			for (j=0;j<m_PointNum;j++)
			{
				K1[k++] = K[i][j];	
			}
		}
		//(MT*K-1*M)-1*MT*K-1*Z

		rinv(K1,m_PointNum);
				
			Z = 0;
			m_fianlpoint.x = m_PointArrayGrid2[I][m_icolumnX-1];
			m_fianlpoint.y = m_PointArrayGrid2[I][m_icolumnY-1];
			for (j =0;j<m_ParaNum;j++)
			{
				Z= Z +/*m_Glinearpara*/g_linearpara.GetAt(j+1)*m_PointArrayGrid2[I][m_icolumnPara1-1+j];
			}
			Z =Z+ /*m_Glinearpara*/g_linearpara[0];
			
			m_fianlpoint.regression = Z;
			finalZ = m_PointArrayGridNew.GetAt(I).z + Z;
			m_fianlpoint.x = m_PointArrayGridNew.GetAt(I).x;
			m_fianlpoint.y = m_PointArrayGridNew.GetAt(I).y;
			m_fianlpoint.z = finalZ;
			m_fianlpoint.reskrig = m_PointArrayGridNew.GetAt(I).z;
			m_fianlpoint.varianceK = varianceK.GetAt(I);
			for (j=0;j<m_PointNum;j++)
			{
				h=::sqrt(::pow((m_PointArray2[j][m_icolumnX-1]-m_PointArrayGrid2[I][m_icolumnX-1]),2)+::pow((m_PointArray2[j][m_icolumnY-1]-m_PointArrayGrid2[I][m_icolumnY-1]),2));
				sk[j] = para[1]*exp(-h/para[2]);
			}
			trmul(sk,K1,1,m_PointNum,m_PointNum,temp3);
			trmul(temp3,sk,1,m_PointNum,1,temp4);//kt*K-1*k
			temp4[0] = k0 - temp4[0];
			m_fianlpoint.variance1 = temp4[0];
			//////////////////////
			trmul(MT1,K1,m_ParaNum+1,m_PointNum,m_PointNum,temp);//MT*K-1
			trmul(temp,sk,m_ParaNum+1,m_PointNum,1,temp5);//MT*K-1*k      d = m-MT*K-1*k
			for (i = 0;i<=m_ParaNum;i++)
			{
				if (i==0)
				{
					m[i]=1;
				}
				else
				{
					m[i] = m_PointArrayGrid2[I][m_icolumnPara1-1+i-1];
				}	
			}
			for (i=0;i<=m_ParaNum;i++)
			{
				m[i]= m[i] -temp5[i];//d
			}
			trmul(temp,M1,m_ParaNum+1,m_PointNum,m_ParaNum+1,temp1);
			rinv(temp1,m_ParaNum+1);//(MT*K-1*M)-1
			trmul(m,temp1,1,m_ParaNum+1,m_ParaNum+1,temp6);
			trmul(temp6,m,1,m_ParaNum+1,1,temp4);//dt*()*d
			m_fianlpoint.variance2 = temp4[0];
			m_fianlpoint.variance = m_fianlpoint.variance1+ m_fianlpoint.variance2;
			m_fianlpoint.para1 =para[0];
			m_fianlpoint.para2 = para[1];
			m_fianlpoint.para3 = para[2];
			for (i=0;i<=m_ParaNum;i++)
			{
				m_fianlpoint.bf[i] = g_linearpara[i];
 			}
			m_fianlPredictPoint.Add(m_fianlpoint);
			if (m_fianlPredictPoint.GetSize()==5)
			{
				outputdataGLS();
				m_fianlPredictPoint.RemoveAll();
			}
			tmp; 
			tmp.Format("%d/%d", I+1,m_GridPointNum); 
			m_progress.ShowWindow(TRUE);
			m_progress.SetWindowText(tmp); 

		delete[] K1;delete[] MT1;delete[] M1;delete[] temp;delete[] temp1;delete[] temp2;delete[] z;delete[] temp3;delete[] temp4;
		delete[] temp5;delete[] temp6;delete[] sk;	
		for (i=0;i<=m_ParaNum;i++)
		{
			delete[] MT[i];
		}
		for (i=0;i<m_PointNum;i++)
		{
			delete[] M[i];
			delete[] K[i];
		}
		delete[] MT;delete[] M;delete[] K;delete[] m;
	}
}

int CGLSView::rinv(double *a, int n)
{
	int *is,*js,i,j,k,l,u,v;
    double d,p;
    is=(int*)malloc(n*sizeof(int));
    js=(int*)malloc(n*sizeof(int));
    for (k=0; k<=n-1; k++)
      { d=0.0;
        for (i=k; i<=n-1; i++)
        for (j=k; j<=n-1; j++)
          { l=i*n+j; p=fabs(a[l]);
            if (p>d) { d=p; is[k]=i; js[k]=j;}
          }
        if (d+1.0==1.0)
          {
			free(is); free(js); 
			//MessageBox("inverse metric error");
			return 0;
          }
        if (is[k]!=k)
          for (j=0; j<=n-1; j++)
            { u=k*n+j; v=is[k]*n+j;
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        if (js[k]!=k)
          for (i=0; i<=n-1; i++)
            { u=i*n+k; v=i*n+js[k];
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        l=k*n+k;
        a[l]=1.0/a[l];
        for (j=0; j<=n-1; j++)
          if (j!=k)
            { u=k*n+j; a[u]=a[u]*a[l];}
        for (i=0; i<=n-1; i++)
          if (i!=k)
            for (j=0; j<=n-1; j++)
              if (j!=k)
                { u=i*n+j;
                  a[u]=a[u]-a[i*n+k]*a[k*n+j];
                }
        for (i=0; i<=n-1; i++)
          if (i!=k)
            { u=i*n+k; a[u]=-a[u]*a[l];}
      }
    for (k=n-1; k>=0; k--)
      { if (js[k]!=k)
          for (j=0; j<=n-1; j++)
            { u=k*n+j; v=js[k]*n+j;
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
        if (is[k]!=k)
          for (i=0; i<=n-1; i++)
            { u=i*n+k; v=i*n+is[k];
              p=a[u]; a[u]=a[v]; a[v]=p;
            }
      }
    free(is); free(js);return 1;
}

void CGLSView::trmul(double *a, double *b, int m, int n, int k, double *c)
{
	int i,j,l,u;
    for (i=0; i<=m-1; i++)
    for (j=0; j<=k-1; j++)
      { u=i*k+j; c[u]=0.0;
        for (l=0; l<=n-1; l++)
          c[u]=c[u]+a[i*n+l]*b[l*k+j];
      }
}

void CGLSView::outputdataK()
{
	GetDlgItemText(IDC_EDIT_OUTPUT,m_strpath);
	m_strpath = m_strpath +"\\";
	GetDlgItemText(IDC_EDIT_FILE,m_filename);
	m_strname = m_strpath +m_filename;
	CStdioFile   fobjK(m_strname,   CFile::modeCreate   |   CFile::modeWrite);   

	CString str ="x 	y	 z  	variance\n";
	fobjK.WriteString(str);  
	for (int i =0;i<m_GridPointNum;i++)
		{	
			str.Format("%.7f  %.7f  %.7f  %.10f\n",m_PointArrayGridNew.GetAt(i).x,
				                       m_PointArrayGridNew.GetAt(i).y,
									   m_PointArrayGridNew.GetAt(i).z,
									   varianceK.GetAt(i));
			 fobjK.WriteString(str); 
		}
	fobjK.Close();  
}

void CGLSView::OnBtnRunrk() 
{
	// TODO: Add your control notification handler code here
	CWinThread *pRunRK =AfxBeginThread(CommProcRK,
		this,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pRunRK->ResumeThread();
}

void CGLSView::FunRK()
{
	b_RunbtndownFlag = FALSE;
	CString m_strPara1,m_strPara2;
	GetDlgItemText(IDC_EDIT_PARA1,m_strPara1);
	GetDlgItemText(IDC_EDIT_PARA2,m_strPara2);
	if (m_strPara1.IsEmpty()||m_strPara2.IsEmpty())
	{
		MessageBox("Please fill the columns of parameters","warning",MB_OK);
		return;
	}
	
	if (ReadData())
	{	
		Regression();
		
		GetResidual();
		
		Calc_vario();
		
		if (m_estimators)
		{
			Calc_semvdistance_median();
		} 
		else
		{
			Calc_semvdistance();
		}
				
		levmar();
		
		predict();
		
		GetResult();
		
		outputdataRK();
		
		MessageBox("kriging finished",NULL,MB_OK);
		
		m_map.EnableWindow(TRUE);
		m_viewoutput.EnableWindow(TRUE);
		m_variogram.EnableWindow(TRUE);

	}
	return;
}

void CGLSView::Regression()
{
	int i;
	int j;

	double **M,*z,*bf,**MT,*MM;
	z = new double[m_PointNum];
	bf = new double[m_ParaNum+1];
	M = new double*[m_PointNum];
	MT= new double*[m_ParaNum+1];
	MM = new double[(m_ParaNum+1)*(m_ParaNum+1)];
	for (i=0; i<m_PointNum; i++) 
	{
		M[i] = new double[m_ParaNum+1];
	}
	for (i=0;i<=m_ParaNum;i++)
	{
		MT[i] = new double[m_PointNum];
	}
	for (int j=0;j<m_PointNum;j++)
	{
		z[j]=m_PointArray2[j][m_icolumnData-1];
	}
	for (i=0;i<m_PointNum;i++)
	{
		for (j=0;j<=m_ParaNum;j++)
		{
			if (j==0)
			{
				M[i][j] =1;
			}
			else
			{
				M[i][j] = m_PointArray2[i][m_icolumnPara1-1+j-1];
			}
		}			
	}
	for(i=0;i<m_PointNum;i++) 
		for(int j=0;j<=m_ParaNum;j++)
			MT[j][i]=M[i][j]; 	//M ?????
    double *m = new double[m_PointNum*(m_ParaNum+1)];
	double *mt = new double[m_PointNum*(m_ParaNum+1)];//M ?????
	double *temp = new double[m_PointNum*(m_ParaNum+1)];

	int k = 0;
	for (i=0;i<m_PointNum;i++)
	{
		for (int j=0;j<=m_ParaNum;j++)
		{
			m[k++] = M[i][j];	
		}
	}
	k=0;
	for ( i=0;i<=m_ParaNum;i++)
	{
		for (j=0;j<m_PointNum;j++)
		{
			
			mt[k++] = MT[i][j];	
		}
	}

	trmul(mt,m,m_ParaNum+1,m_PointNum,m_ParaNum+1,MM);//M???M??MM
	rinv(MM,m_ParaNum+1);//???,??MM
	trmul(MM,mt,m_ParaNum+1,m_ParaNum+1,m_PointNum,temp);//MM??MT??temp
	trmul(temp,z,m_ParaNum+1,m_PointNum,1,bf);
	g_linearpara.RemoveAll();
	g_linearpara.SetSize(0);
	for (i=0;i<m_ParaNum+1;i++)
	{
		g_linearpara.Add(bf[i]);
	}
	for (i=0; i<m_PointNum; i++) 
	{
		delete[] M[i];
	}
	for (i=0;i<=m_ParaNum;i++)
	{
		delete[] MT[i];
	}
	delete[] M;delete[] MT;delete MM;delete[] z;delete[] bf;delete[] m;delete[] mt;
}

void CGLSView::GetResult()
{
	if (m_PointArrayGridNew.GetSize()==0)
	{
		return;
	}
	int i;

	m_fianlPredictPoint.RemoveAll();
	m_fianlPredictPoint.SetSize(0);
	double  finalZ =0;
	double **M,**MT;
	double *m =new double[m_ParaNum+1];
	double	*MM = new double[(m_ParaNum+1)*(m_ParaNum+1)];
	M = new double*[m_PointNum];
	for (i =0;i<m_PointNum;i++)
	{
		M[i] = new double[m_ParaNum+1];
	}
	MT = new double*[m_ParaNum+1];
	for (i=0;i<=m_ParaNum;i++)
	{
		MT[i] =new double[m_PointNum];
	}
	
	double *temp1 =new double[m_ParaNum+1];
	double *m1 =new double[m_ParaNum+1];
	for (i=0;i<m_PointNum;i++)
	{
		for (int j=0;j<=m_ParaNum;j++)
		{
			if (j==0)
			{
				M[i][j] =1;
			}
			else
			{
				M[i][j] = m_PointArray2[i][m_icolumnPara1-1+j-1];
			}
		}			
	}
	for(i=0;i<m_PointNum;i++) 
		for(int j=0;j<=m_ParaNum;j++)
			MT[j][i]=M[i][j]; 	//M ?????
    double *M1 = new double[m_PointNum*(m_ParaNum+1)];
	double *MT1 = new double[m_PointNum*(m_ParaNum+1)];//M ?????
	double *temp = new double[1];

	int k = 0;
	for (i=0;i<m_PointNum;i++)
	{
		for (int j=0;j<=m_ParaNum;j++)
		{
			M1[k++] = M[i][j];	
		}
	}
	k=0;
	for ( i=0;i<=m_ParaNum;i++)
	{
		for (int j=0;j<m_PointNum;j++)
		{
			
			MT1[k++] = MT[i][j];	
		}
	}
	
 	trmul(MT1,M1,m_ParaNum+1,m_PointNum,m_ParaNum+1,MM);//M?M????MM
	rinv(MM,m_ParaNum+1);//???,??MM
	//get the SSR/N-P-1
	double SSR =0;
	for (i=0;i<m_Residual.GetSize();i++)
	{
		SSR += ::pow(m_Residual.GetAt(i).z,2);
	}
	SSR = SSR/(m_Residual.GetSize()-m_ParaNum-1-1);
	double z;
	for (int I=0;I<m_PointArrayGrid2.GetSize();I++)
	{
		z = 0;
		m_fianlpoint.x = m_PointArrayGrid2[I][m_icolumnX-1];
		m_fianlpoint.y = m_PointArrayGrid2[I][m_icolumnY-1];
 		for (int j =0;j<m_ParaNum;j++)
		{
			z = z + g_linearpara.GetAt(j+1)*m_PointArrayGrid2[I][m_icolumnPara1-1+j];
		}
		z = z+ g_linearpara[0];
	
		finalZ = m_PointArrayGridNew.GetAt(I).z + z;
		m_fianlpoint.x = m_PointArrayGridNew.GetAt(I).x;
		m_fianlpoint.y = m_PointArrayGridNew.GetAt(I).y;
		m_fianlpoint.z = finalZ;
		m_fianlpoint.reskrig = m_PointArrayGridNew.GetAt(I).z;
		m_fianlpoint.varianceK = varianceK.GetAt(I);
		
		for (i = 0;i<=m_ParaNum;i++)
		{
			if (i==0)
			{
				m[i]=1;
			}
			else
			{
				m[i] = m_PointArrayGrid2[I][m_icolumnPara1-1+i-1];
			}
			
		}
		
		trmul(m,MM,1,m_ParaNum+1,m_ParaNum+1,temp1);//m???MM???
		trmul(temp1,m,1,m_ParaNum+1,1,temp);
		m_fianlpoint.variance = temp[0]*SSR;
		m_fianlPredictPoint.Add(m_fianlpoint);
		
	}
	delete[] m1;delete[] temp;delete[] M1;delete[] MT1;delete[] MM;delete[] temp1;

	for(i=0;i<m_PointNum;i++)
	{
		delete[] M[i];
	}
	for(i=0;i<=m_ParaNum;i++)
	{
		delete[] MT[i];
	}
	delete[] M;delete[] MT;delete[] m;
}

void CGLSView::outputdataRK()
{
	GetDlgItemText(IDC_EDIT_OUTPUT,m_strpath);
	m_strpath = m_strpath +"\\";
	GetDlgItemText(IDC_EDIT_FILERK,m_fileRK);
	m_strname = m_strpath +m_fileRK;
	CStdioFile   fobjRK(m_strname,   CFile::modeCreate   |   CFile::modeWrite);  
	CString str ="x	y	z	reskrig	Kriging-variance	linear-variance   var\n";
	fobjRK.WriteString(str);
	for (int i=0;i<m_GridPointNum;i++)
	{
			str.Format("%.7f	%.7f	%.7f	%.10f	%.10f	%.15f	%.15f\n",m_fianlPredictPoint.GetAt(i).x,
				                       m_fianlPredictPoint.GetAt(i).y,
									   m_fianlPredictPoint.GetAt(i).z,
									   m_fianlPredictPoint.GetAt(i).reskrig,
									   m_fianlPredictPoint.GetAt(i).varianceK,
									   m_fianlPredictPoint.GetAt(i).variance,
									   m_fianlPredictPoint.GetAt(i).varianceK+m_fianlPredictPoint.GetAt(i).variance);
			 fobjRK.WriteString(str); 
	}
	fobjRK.Close();
}

void CGLSView::OnBtnRungls() 
{
	// TODO: Add your control notification handler code here
	CWinThread *pRunGLS =AfxBeginThread(CommProcGLS,
		this,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED);
	pRunGLS->ResumeThread();

}

void CGLSView::FunGLS()
{
	b_RunbtndownFlag = FALSE;

	if (((CButton *)GetDlgItem(IDC_RADIO_LOCAL))->GetCheck())
	{
		CString m_strPara1,m_strPara2;
		GetDlgItemText(IDC_EDIT_PARA1,m_strPara1);
		GetDlgItemText(IDC_EDIT_PARA2,m_strPara2);
		if (m_strPara1.IsEmpty()||m_strPara2.IsEmpty())
		{
			MessageBox("Please fill the columns of parameters","warning",MB_OK);
			return;
		}
 		ReadData();

		m_staticLocal.ShowWindow(TRUE);
 		predictLocalGLS();
		m_map.EnableWindow(TRUE);
		m_viewoutput.EnableWindow(TRUE);
		m_variogram.EnableWindow(TRUE);

	}
	
	else if (((CButton *)GetDlgItem(IDC_RADIO_GLOBAL))->GetCheck())
	{
		CString m_strPara1,m_strPara2;
		GetDlgItemText(IDC_EDIT_PARA1,m_strPara1);
		GetDlgItemText(IDC_EDIT_PARA2,m_strPara2);
		if (m_strPara1.IsEmpty()||m_strPara2.IsEmpty())
		{
			MessageBox("Please fill the columns of parameters","warning",MB_OK);
			return;
		}
//--------get exponential parameter---------------
		ReadData();

		Regression();
		
		GetResidual();
		
		Calc_vario();
		
		if (m_estimators)
		{
			Calc_semvdistance_median();
		} 
		else
		{
			Calc_semvdistance();
		}
				
		levmar();

		m_fianlPredictPoint.RemoveAll();
		predict();

 	}
	
	outputdataGLS();
	MessageBox("finished!!!","OK",MB_OK);
	m_map.EnableWindow(TRUE);
	m_viewoutput.EnableWindow(TRUE);
	m_variogram.EnableWindow(TRUE);

}

void CGLSView::predictLocalGLS()
{
	varianceK.RemoveAll();
	varianceK.SetSize(0);
	m_PointArrayGridNew.RemoveAll();
	m_PointArrayGridNew.SetSize(0);
	m_fianlPredictPoint.RemoveAll();
	m_fianlPredictPoint.SetSize(0);

	int I,J,i,j,N,n;
	double dist=0,hh=0,Gx=0,Gy=0,rr=0,VarEsum;
	int pro =0;
	
	CString strNum;
	int GetNum ;
	GetDlgItemText(IDC_EDIT_LOCALNUM,strNum);
	GetNum = atoi(strNum);
	double dyMax=1E-15;dyMin=1E+15;
	double dxMax=1E-15;dxMin=1E+15;

	//Checking the point array, to identify the coordinates of
	//the rectangle which can include all the points
	for(i=0;i<m_PointArray2.GetSize();i++)  
	{
		if(m_PointArray2[i][m_icolumnY-1]>Ymax)
			Ymax=m_PointArray2[i][m_icolumnY-1];
	
		if(m_PointArray2[i][m_icolumnY-1]<Ymin)
			Ymin=m_PointArray2[i][m_icolumnY-1];

		if (m_PointArray2[i][m_icolumnX-1]>Xmax)
			Xmax = m_PointArray2[i][m_icolumnX-1];

		if (m_PointArray2[i][m_icolumnX-1]<Xmin)
			Xmin = m_PointArray2[i][m_icolumnX-1];

		if (m_PointArray2[i][m_icolumnData-1]>Zmax)
			Zmax = m_PointArray2[i][m_icolumnData-1];

		if (m_PointArray2[i][m_icolumnData-1]<Zmin)
			Zmin = m_PointArray2[i][m_icolumnData-1];
	}

	dyMax= Ymax;dyMin = Ymin;dxMax =Xmax;dxMin = Xmin;
	
	for( i=0;i<m_PointArrayGrid2.GetSize();i++)  
	{
		if(m_PointArrayGrid2[i][m_icolumnY-1]>Ymax)
			Ymax=m_PointArrayGrid2[i][m_icolumnY-1];
	
		if(m_PointArrayGrid2[i][m_icolumnY-1]<Ymin)
			Ymin=m_PointArrayGrid2[i][m_icolumnY-1];

		if (m_PointArrayGrid2[i][m_icolumnX-1]>Xmax)
			Xmax = m_PointArrayGrid2[i][m_icolumnX-1];

		if (m_PointArrayGrid2[i][m_icolumnX-1]<Xmin)
			Xmin = m_PointArrayGrid2[i][m_icolumnX-1];
	}

	double area = (dxMax-dxMin)*(dyMax-dyMin);
	double times = m_PointArray2.GetSize()/GetNum;
	double perarea = area/((int)times+1);
	double ratio = fabs(sqrt(perarea/3.14));
	
	double areaX1;
	double areaX2;
	double areaY1;
	double areaY2;

	for (I=0;I<m_PointArrayGrid2.GetSize();I++)
	{	
		areaX1 = m_PointArrayGrid2[I][m_icolumnX-1]-ratio;
		areaX2 = ratio + m_PointArrayGrid2[I][m_icolumnX-1];
		areaY1 = m_PointArrayGrid2[I][m_icolumnY-1]-ratio;
		areaY2 = ratio + m_PointArrayGrid2[I][m_icolumnY-1];
		do 
		{
			m_PredictPoint2.RemoveAll();
			for (J=0;J<m_PointArray2.GetSize();J++)
			{
				if (m_PointArray2[J][m_icolumnX-1]>areaX1&& m_PointArray2[J][m_icolumnX-1]<areaX2
					&&m_PointArray2[J][m_icolumnY-1]>areaY1&&m_PointArray2[J][m_icolumnY-1]<areaY2)
				{
					m_PredictPoint2.Add(m_PointArray2[J]);
				}		
			}
			areaX1 = areaX1 -ratio;
			areaX2 = areaX2+ratio;
			areaY1 = areaY1 -ratio;
			areaY2 = areaY2 +ratio;
		} while (m_PredictPoint2.GetSize()<GetNum);

		if (m_PredictPoint2.GetSize()>GetNum)
		{
			for (i=0;i<m_PredictPoint2.GetSize();i++)
			{
				m_Element = m_PredictPoint2[i];
				dist = sqrt(pow((m_PointArrayGrid2[I][m_icolumnX-1]-m_PredictPoint2[i][m_icolumnX-1]),2)
					+pow((m_PointArrayGrid2[I][m_icolumnY-1]-m_PredictPoint2[i][m_icolumnY-1]),2));
				m_Element.Add(dist);
				m_PredictPoint2.SetAt(i,m_Element);
			}
			
			
			for(int i=0; i<m_PredictPoint2.GetSize()-1; i++) 
			{ 
				for(int j=i+1; j<m_PredictPoint2.GetSize();j++) 
				{ 
					if (m_PredictPoint2[i][m_icolumnData]>m_PredictPoint2[j][m_icolumnData])
					{
						m_Element =m_PredictPoint2[i]; 
						m_PredictPoint2[i]=m_PredictPoint2[j]; 
						m_PredictPoint2[j]=m_Element;
					}	
				} 			
			} 
			N=m_PredictPoint2.GetSize();
			m_PredictPoint2.RemoveAt(GetNum,N-GetNum);//Local sampling points
		}
		N=m_PredictPoint2.GetSize();
		n=N+1;

		double **A=new double *[n];
		double *A1 = new double[n*n];
		double *p = new double[n];
		double *z = new double[N];

		for(i=0;i<n;i++)
		{
			A[i]=new double[n];
		}

		double *B=new double[n];
		double k0;
		double	*bf = new double[m_ParaNum+1];
		double *sk = new double[N];
		double *temp = new double[N*(m_ParaNum+1)];
		double *temp1 = new double[(m_ParaNum+1)*(m_ParaNum+1)];
		double *temp2 = new double[N*(m_ParaNum+1)];
		
		double *temp3 = new double[N];
		double *temp4 = new double;
		double *temp5 =new double[m_ParaNum+1];	
		double *temp6 =new double[m_ParaNum+1];	
		
		double *m =new double[m_ParaNum+1];
		
	//MLR	3-22
		double **M,**MT;
		double	*MM = new double[(m_ParaNum+1)*(m_ParaNum+1)];
		M = new double*[N];
		for (i =0;i<N;i++)
		{
			M[i] = new double[m_ParaNum+1];
		}
		MT = new double*[m_ParaNum+1];
		for (i=0;i<=m_ParaNum;i++)
		{
			MT[i] =new double[N];//×ªÖÃ
		}
		
		 for (i=0;i<N;i++)
		{
			z[i]=m_PredictPoint2[i][m_columnNum-1] ;
			for (int j=0;j<=m_ParaNum;j++)
			{
				if (j==0)
				{
					M[i][j] =1;
				}
				else
				{
					M[i][j] = m_PredictPoint2[i][m_icolumnPara1-1+j-1];
				}
			}			
		}
		for(i=0;i<N;i++) 
			for(int j=0;j<=m_ParaNum;j++)
				MT[j][i]=M[i][j];
		double *M1 = new double[N*(m_ParaNum+1)];
		double *MT1 = new double[N*(m_ParaNum+1)];
		
		int k = 0;
		for (i=0;i<N;i++)
		{
			for (int j=0;j<=m_ParaNum;j++)
			{
				M1[k++] = M[i][j];	
			}
		}
		k=0;
		for ( i=0;i<=m_ParaNum;i++)
		{
			for (int j=0;j<N;j++)
			{
				
				MT1[k++] = MT[i][j];	
			}
		}
		//--------get linear parameter---------------
		
		trmul(MT1,M1,m_ParaNum+1,N,m_ParaNum+1,MM);//M???M??MM
		if(!rinv(MM,m_ParaNum+1)) 
			continue;// MessageBox("MM",NULL,MB_OK);//???,??MM
		trmul(MM,MT1,m_ParaNum+1,m_ParaNum+1,N,temp);//MM??MT??temp
		trmul(temp,z,m_ParaNum+1,N,1,bf);
		g_linearpara.RemoveAll();
		g_linearpara.SetSize(0);
		for (i=0;i<m_ParaNum+1;i++)
		{
			g_linearpara.Add(bf[i]);
		}
		//----------------------------Get Residual---------------------------
		
		m_Residual.RemoveAll();
		m_Residual.SetSize(0);
		double z1;
		for (i =0;i<N;i++)
		{	
			z1 = 0;
			m_point.x = m_PredictPoint2[i][m_icolumnX -1];
			m_point.y =m_PredictPoint2[i][m_icolumnY -1];
			for (int j =0;j<m_ParaNum;j++)
			{
				z1 = z1 + g_linearpara[j+1]*m_PredictPoint2[i][m_icolumnPara1-1+j];
			}
			z1 = z1+ g_linearpara[0];
			
			
			m_point.z = m_PredictPoint2[i][m_columnNum-1]-z1;
			m_Residual.Add(m_point);
		}
		//---------------------------------------------------------------------		
		Calc_vario();
		
		if (m_estimators)
		{
			Calc_semvdistance_median();
		} 
		else
		{
			Calc_semvdistance();
		}
		
		levmar();
		//-------------------over-----------------
		//?K
		double a1=para[0],a2=para[1],a3=para[2];
		double **K = new double*[N];
		double *K1 = new double[N*N];
		double **dis = new double*[N];
		for (i=0;i<N;i++)
		{
			dis[i] = new double[N];
		}
		double h =0;
		for (i=0;i<N;i++)
		{
			K[i] = new double[N];
		}
		for (i=0;i<N;i++)
		{
			for (int j=i;j<N;j++)
			{
				if (i==j)
				{
					K[i][j] = para[0]+para[1];//?????
					hh=::sqrt(::pow((m_PredictPoint2[i][m_icolumnX-1]-m_PredictPoint2[j][m_icolumnX-1]),2)+::pow((m_PredictPoint2[i][m_icolumnY-1]-m_PredictPoint2[j][m_icolumnY-1]),2));
					dis[i][j] = hh;
				}
				else
				{
					hh=::sqrt(::pow((m_PredictPoint2[i][m_icolumnX-1]-m_PredictPoint2[j][m_icolumnX-1]),2)+::pow((m_PredictPoint2[i][m_icolumnY-1]-m_PredictPoint2[j][m_icolumnY-1]),2));
					K[i][j] = para[1]*exp(-hh/para[2]);
					dis[i][j] = hh;
				}
				K[j][i] = K[i][j];
				dis[j][i] = dis[i][j];
			}
		}
		k=0;
		for ( i=0;i<N;i++)
		{
			for (int j=0;j<N;j++)
			{
				
				K1[k++] = K[i][j];	
			}
		}
		//linear model bf=(MT*K-1*M)-1*MT*K-1*Z
		if(!rinv(K1,N))
		{
		//	continue;
			CStdioFile   fobjGLS("c:\\dis.txt",   CFile::modeCreate   |   CFile::modeWrite);  
			CString str ;
			for (int i =0;i<N;i++)
			{	
				for (int j=0;j<N;j++)
				{
					str.Format("%.10f\t",K[i][j]);
					fobjGLS.WriteString(str);   
				}
				str = "\n";
				fobjGLS.WriteString(str);   
			}
			fobjGLS.Close();
			CStdioFile   fobjGLS1("c:\\varpara.txt",   CFile::modeCreate   |   CFile::modeWrite);  
			str.Format("%.10f	%.10f	%.10f\n",para[0],para[1],para[2]);
			fobjGLS1.WriteString(str);   
			fobjGLS1.Close();
			
			MessageBox("K1",NULL,MB_OK);
		}
		trmul(MT1,K1,m_ParaNum+1,N,N,temp);//MT*K-1
		trmul(temp,M1,m_ParaNum+1,N,m_ParaNum+1,temp1);
		if (!rinv(temp1,m_ParaNum+1))//(MT*K-1*M)-1
		{
			continue;
// 			MessageBox("bf",NULL,MB_OK);
		}
		trmul(temp1,MT1,m_ParaNum+1,m_ParaNum+1,N,temp);
		trmul(temp,K1,m_ParaNum+1,N,N,temp2);
		trmul(temp2,z,m_ParaNum+1,N,1,bf);
//---------------------------------the second time iterate-------------------------------------------
		//get residual the second time
		m_Residual.RemoveAll();
		m_Residual.SetSize(0);
		double Z;
		for (i =0;i<N;i++)
		{
			Z = 0;
			m_point.x = m_PredictPoint2[i][m_icolumnX-1];
			m_point.y = m_PredictPoint2[i][m_icolumnY-1];
			for (int j =0;j<m_ParaNum;j++)
			{
				Z = Z + bf[j+1]*m_PredictPoint2[i][m_icolumnPara1-1+j];
			}
			Z = Z+ bf[0];
			m_point.z = m_PredictPoint2[i][m_columnNum -1]-Z;
			m_Residual.Add(m_point);
			
		}
		
		m_Glinearpara.RemoveAll();
		m_Glinearpara.SetSize(0);
		for (i=0;i<m_ParaNum+1;i++)
		{
			m_Glinearpara.Add(bf[i]);
		}
		Calc_vario();
		
		//mean semv<-->distance of residual
		if (m_estimators)
		{
			Calc_semvdistance_median();
		} 
		else
		{
			Calc_semvdistance();
		}
		
		//nonlinear least squares
		levmar();
		int ii;
		int jj;

//----------------------------------kriging---------------------------------------------
		for(ii=0;ii<N;ii++)
		{
			for(jj=ii;jj<N;jj++)
			{
				if (ii==jj)
				{
					A[ii][jj] =0;//?????
				}
				else
				{
					hh=::sqrt(::pow((m_PredictPoint2[ii][m_icolumnX-1]-m_PredictPoint2[jj][m_icolumnX-1]),2)+::pow((m_PredictPoint2[ii][m_icolumnY-1]-m_PredictPoint2[jj][m_icolumnY-1]),2));
					rr=para[0]+para[1]*(1-exp(-hh/para[2]));
					A[ii][jj]=rr;
				}
				A[jj][ii] = A[ii][jj];
			}
			A[ii][jj]=1;//??????1
		}
	 	for(i=0;i<N;i++)
			A[N][i]=1;//??????1
		A[N][N]=0;  
		//????????
		k = 0;
		for (i=0;i<n;i++)
		{
			for (int j=0;j<n;j++)
			{
				A1[k++] = A[i][j];	
			}
		}
		if(!rinv(A1,n)) 
			continue;//MessageBox("A",NULL,MB_OK);
		for (i=0;i<N;i++)
		{
			for (j=i;j<N;j++)
			{
				if (i==j)
				{
					K[i][j] = para[0]+para[1];//?????
					dis[i][j]=0;
				}
				else
				{
					hh=::sqrt(::pow((m_PredictPoint2[i][m_icolumnX-1]-m_PredictPoint2[j][m_icolumnX-1]),2)+::pow((m_PredictPoint2[i][m_icolumnY-1]-m_PredictPoint2[j][m_icolumnY-1]),2));
					K[i][j] = para[1]*exp(-hh/para[2]);
				}
				K[j][i] = K[i][j];
			}
		}
		k=0;
		for ( i=0;i<N;i++)
		{
			for (int j=0;j<N;j++)
			{
				
				K1[k++] = K[i][j];	
			}
		}
		//linear modle bf=(MT*K-1*M)-1*MT*K-1*Z
		if(!rinv(K1,N)) continue;//MessageBox("K2",NULL,MB_OK);
		
		VarEsum =0;
		for(i=0;i<n;i++)
			p[i]=0;
		
		//??????????--?????
		Gx=m_PointArrayGrid2[I][m_icolumnX-1] ;
		Gy=m_PointArrayGrid2[I][m_icolumnY-1] ;//?????
		
		for(int j=0;j<N;j++)
		{
			hh=::sqrt(::pow((m_PredictPoint2[j][m_icolumnX-1] -Gx),2)+::pow((m_PredictPoint2[j][m_icolumnY-1]-Gy),2));
			rr=para[0]+para[1]*(1-exp(-hh/para[2]));
			B[j]=rr;
			sk[j] = para[1]*exp(-hh/para[2]);
			
		}
		B[j]=1;//?????????1           //????????
		trmul(A1,B,n,n,1,p);
		
		for(i=0;i<n;i++)
		{
			VarEsum += p[i]*B[i];
		}
		varianceK.Add(VarEsum);
		
		//?????????
		double Value=0;
		for(i=0;i<n-1;i++)
		{
			m_point= m_Residual.GetAt(i);
			Value+=p[i]*m_point.z;
		}
		//----------------------------------------------end----------------------------------	
		//graph --------------------------------------------------
		CWnd* pWnd=GetDlgItem(IDC_STATIC_LOCAL); 
		pControlDC=pWnd->GetDC(); 
		
		pWnd->Invalidate(); 
		pWnd->UpdateWindow(); 
		CRect rect; 
		pWnd->GetClientRect(rect); 
		CBitmap memBitmap;
		CBitmap* pOldBmp = NULL;
		CDC memDC;            //??????
		
		memDC.CreateCompatibleDC(pControlDC);
		memBitmap.CreateCompatibleBitmap(pControlDC,rect.right,rect.bottom);
		pOldBmp = memDC.SelectObject(&memBitmap);
		memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pControlDC,0,0,SRCCOPY);
		DrawWave(&memDC,Gx,Gy);
		pControlDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);
		
		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		memBitmap.DeleteObject();
		//------------------------------------------------------------------	
		///////////////////////////////RK variace///////////////////////////////////////////////////////
		double regression = 0;
		m_fianlpoint.x =m_PointArrayGrid2[I][m_icolumnX-1] ;
		m_fianlpoint.y = m_PointArrayGrid2[I][m_icolumnY-1];
// 		for ( j =0;j<m_ParaNum;j++)
// 		{
// 			regression = regression + m_Glinearpara.GetAt(j+1)*m_PointArrayGrid2[I][ m_icolumnPara1-1+j];// ITERATE
// 		}
// 		regression = regression+ m_Glinearpara[0];
		
		for ( j =0;j<m_ParaNum;j++)
		{
			regression = regression + g_linearpara.GetAt(j+1)*m_PointArrayGrid2[I][ m_icolumnPara1-1+j];
		}
		regression = regression+ g_linearpara[0];
		
		m_fianlpoint.regression = regression;
		m_fianlpoint.z =Value +regression;
		m_fianlpoint.reskrig =Value;
		m_fianlpoint.varianceK = VarEsum;
		
		for (i = 0;i<=m_ParaNum;i++)
		{
			if (i==0)
			{
				m[i]=1;
			}
			else
			{
				m[i] =m_PointArrayGrid2[I][m_icolumnPara1-1+i-1];
			}
			
		}
		
		trmul(sk,K1,1,N,N,temp3);
		trmul(temp3,sk,1,N,1,temp4);//kt*K-1*k
		k0 = para[0]+para[1];
	 	double var1 = k0 - temp4[0];
		m_fianlpoint.variance1 = var1;
		//////////////////////////////////////////////////////////////////////////////////////////
		trmul(MT1,K1,m_ParaNum+1,N,N,temp);//MT*K-1
		trmul(temp,sk,m_ParaNum+1,N,1,temp5);//MT*K-1*k      d = m-MT*K-1*k
		for (i=0;i<=m_ParaNum;i++)
		{
			m[i]= m[i] -temp5[i];//d
		}
		trmul(temp,M1,m_ParaNum+1,N,m_ParaNum+1,temp1);//MT*K-1*M
		if(!rinv(temp1,m_ParaNum+1)) 
			continue;//MessageBox("5",NULL,MB_OK);//(MT*K-1*M)-1
		
		trmul(m,temp1,1,m_ParaNum+1,m_ParaNum+1,temp6);
		trmul(temp6,m,1,m_ParaNum+1,1,temp4);//dt*()*d
		m_fianlpoint.variance2 = temp4[0];
		m_fianlpoint.variance = m_fianlpoint.variance1+ m_fianlpoint.variance2;
		m_fianlpoint.para1 =para[0];
		m_fianlpoint.para2 = para[1];
		m_fianlpoint.para3 = para[2];
		for (i=0;i<=m_ParaNum;i++)
		{
			m_fianlpoint.bf[i] = g_linearpara[i];
		}
	 	m_fianlPredictPoint.Add(m_fianlpoint);
		CString tmp; 
		pro++;
		tmp.Format("%d/%d",pro ,m_GridPointNum); 
		m_progress.ShowWindow(TRUE);
		m_progress.SetWindowText(tmp); 
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		delete[] m;	delete[] temp3;	delete[] temp4;	delete[] temp5;	delete[] temp6;	delete[] sk;delete[]temp2;delete[] z;delete[] temp;
		delete[] B;	delete[] p;	delete[] A1;delete[] K1;delete[] M1;delete[] MT1;delete[] MM;
		for(i=0;i<N;i++)
		{
			delete[] M[i];delete[] K[i];delete[] dis[i];
		}
		for(i=0;i<=m_ParaNum;i++)
		{
			delete[] MT[i];
		}
		delete[] M;	delete[] MT;delete[] K;	delete[] bf;delete[] temp1;	delete[] dis;
		for(i=0;i<n;i++)	
		{
			delete[] A[i];
		}
		delete[] A;

		if (m_fianlPredictPoint.GetSize()==50)
		{
			outputdataGLS();
			m_fianlPredictPoint.RemoveAll();
		}

	}

	m_staticLocal.ShowWindow(FALSE);
}

void CGLSView::DrawWave(CDC *pDC, double x, double y)
{
 	double X,Y,X1,Y1;
 	CRect rct;
 	int i;
 	// ??????????
 	CWnd* pWnd = GetDlgItem(IDC_STATIC_LOCAL);
 	
	pWnd->GetClientRect(&rct);
	pDC->Rectangle(&rct);
	CPen RectPen(PS_SOLID,2,0x0000ff);
	pControlDC->SelectObject(&RectPen); 
			
	pControlDC->Rectangle(rct.left+10,rct.top+10,rct.right-10,rct.bottom-10); 
	for (i = 0;i<m_PredictPoint2.GetSize();i++)
	{
		X =50+ (m_PredictPoint2[i][m_icolumnX-1] -Xmin)/(Xmax-Xmin)*(rct.right/2+100);
		Y = (rct.bottom/2-50) - (m_PredictPoint2[i][m_icolumnY-1]-Ymin)/(Ymax-Ymin)*(rct.bottom/2-50);
		pControlDC->Ellipse(X-2,Y-2,X+2,Y+2);			
	}
	RectPen.DeleteObject();
		
	CPen RectPen1(PS_SOLID,2,0x800000);
	pControlDC->SelectObject(&RectPen1); 
		
	X = 50+(x-Xmin)/(Xmax-Xmin)*(rct.right/2+100);
	Y = (rct.bottom/2-50) - (y-Ymin)/(Ymax-Ymin)*(rct.bottom/2-50);
	pControlDC->Ellipse(X-3,Y-3,X+3,Y+3);
	RectPen1.DeleteObject();
	//exp	
	pControlDC->MoveTo(40,250);
	pControlDC->LineTo(40,480);
	pControlDC->LineTo(300,480);	
			
	for(i=1;i<Phr.GetSize();i++)
	{  
		X1 = 40 + (Phr[i].hm-dxMin)/(dxMax-dxMin)*(rct.right/4+100);
		Y1 = (rct.bottom-50)-((Phr[i].semivar-dyMin)/(dyMax-dyMin)*(rct.bottom/2-50));
		pControlDC->Ellipse(X1-3,Y1-3,X1+3,Y1+3);
	}
	X1=40 + (Phr[1].hm-dxMin)/(dxMax-dxMin)*(rct.right/4+100);
	Y1 = para[0]+para[1]*(1-exp(-Phr[1].hm/para[2]));
	Y1=((rct.bottom-50)-(Y1-dyMin)/(dyMax-dyMin)*(rct.bottom/2-50));
	pControlDC->MoveTo(X1,Y1);	
	for (i=2;i<Phr.GetSize();i++)
	{
		X1=40 + (Phr[i].hm-dxMin)/(dxMax-dxMin)*(rct.right/4+100);
		Y1 = para[0]+para[1]*(1-exp(-Phr[i].hm/para[2]));
		Y1=((rct.bottom-50)-(Y1-dyMin)/(dyMax-dyMin)*(rct.bottom/2-50));
			
		pControlDC->LineTo(X1,Y1);	
	}
	//linear		
	pControlDC->MoveTo(320,250);
	pControlDC->LineTo(320,480);
	pControlDC->LineTo(580,480);	
	for(i=0;i<m_PredictPoint2.GetSize();i++)
	{  
		X1 =320+(m_PredictPoint2[i][m_icolumnData-1]-Zmin)/(Zmax-Zmin)*(rct.bottom/2-50);
		Y1 =0;
		for (int j =0;j<m_ParaNum;j++)
		{
			Y1 = Y1 + m_Glinearpara.GetAt(j+1)*m_PredictPoint2[i][m_icolumnPara1-1+j];
		}
		Y1 = Y1 + m_Glinearpara.GetAt(0);
		Y1 = (rct.bottom-50) - (Y1-Zmin)/(Zmax-Zmin)*(rct.bottom/2-50);
		pControlDC->Ellipse(X1-3,Y1-3,X1+3,Y1+3);
	}
	pWnd->ReleaseDC(pControlDC); 
}

void CGLSView::OnRadioGlobal() 
{
	// TODO: Add your control notification handler code here
	m_localNum.EnableWindow(FALSE);
}

void CGLSView::OnRadioLocal() 
{
	// TODO: Add your control notification handler code here
	m_localNum.EnableWindow(TRUE);
	
}

void CGLSView::OnBtnVariogram() 
{
	// TODO: Add your control notification handler code here
	int i;
	b_RunbtndownFlag =TRUE;
	int n = ReadData()	;
	g_linearpara.RemoveAll();
	g_linearpara.SetSize(0);
	m_ParaNum =0;
	for (i=0;i<=m_ParaNum;i++)
	{
		g_linearpara.Add(0);
	}
	if(n)
	{
		Regression();
		
		GetResidual();

		Calc_vario();
		
		if (m_estimators)
		{
			Calc_semvdistance_median();
		} 
		else
		{
			Calc_semvdistance();
		}
				
		levmar();
	}
	CVariogram dlg;
 	dlg.m_PointNum = Phr.GetSize();
	dlg.para[0] = para[0];
	dlg.para[1] = para[1];
	dlg.para[2] = para[2];

	for (i =0;i<Phr.GetSize();i++)
	{
		dlg.m_point.x = Phr.GetAt(i).hm;
		dlg.m_point.y = Phr.GetAt(i).semivar;
		dlg.m_PointArray.Add(dlg.m_point);
	}
	dlg.DoModal();
}

void CGLSView::OnBtnMap() 
{
	// TODO: Add your control notification handler code here
	CGridMap dlg;
	if (b_RunbtndownFlag)
	{
		for (int i =0;i<m_PointArrayGridNew.GetSize();i++)
		{
			dlg.m_point.x = m_PointArrayGridNew.GetAt(i).x;
			dlg.m_point.y = m_PointArrayGridNew.GetAt(i).y;
			dlg.m_point.z = m_PointArrayGridNew.GetAt(i).z;
			dlg.m_PointArray.Add(dlg.m_point);
		}
		
	}
	else	
	{
		for (int i =0;i<m_fianlPredictPoint.GetSize();i++)
		{
			dlg.m_point.x = m_fianlPredictPoint.GetAt(i).x;
			dlg.m_point.y = m_fianlPredictPoint.GetAt(i).y;
			dlg.m_point.z = m_fianlPredictPoint.GetAt(i).z;
			dlg.m_PointArray.Add(dlg.m_point);
		}
	}
	dlg.DoModal();
}

void CGLSView::OnBtnViewoutput() 
{
	// TODO: Add your control notification handler code here
	CString path;
	path = m_strpath + m_filename;
	ShellExecute(NULL,"open",path,NULL,NULL,SW_SHOWNORMAL);
	path = m_strpath + m_fileRK;
	ShellExecute(NULL,"open",path,NULL,NULL,SW_SHOWNORMAL);
	path = m_strpath + m_fileGLS;
	ShellExecute(NULL,"open",path,NULL,NULL,SW_SHOWNORMAL);
}

void CGLSView::OnBtnOutput() 
{
	// TODO: Add your control notification handler code here
		CFileDialog dlg(TRUE); 
    if(dlg.DoModal()==IDOK)
    {
		m_output = dlg.GetPathName(); 
	    m_strpath = m_output.Left(m_output.ReverseFind('\\'));
		m_strname = m_output.Right(m_output.GetLength()-m_output.ReverseFind('\\')-1);
		m_strname = m_strname.Left(m_strname.ReverseFind('.'));
		m_filename = m_strname+".txt";
		SetDlgItemText(IDC_EDIT_OUTPUT,m_strpath);
		SetDlgItemText(IDC_EDIT_FILE,m_filename);
		m_fileRK = m_strname+"RK.txt";
		SetDlgItemText(IDC_EDIT_FILERK,m_fileRK);
		m_fileGLS = m_strname+"GLS.txt";
		SetDlgItemText(IDC_EDIT_FILEGLS,m_fileGLS);
	}
	else
	{
		return;
	}
}
void CGLSView::outputdataGLS()  
{
	GetDlgItemText(IDC_EDIT_OUTPUT,m_strpath);
	m_strpath = m_strpath +"\\";
	GetDlgItemText(IDC_EDIT_FILEGLS,m_fileGLS);
	m_strname = m_strpath +m_fileGLS;

	CStdioFile   fobjGLS(m_strname,   CFile::modeCreate|CFile::modeNoTruncate    |   CFile::modeWrite);  
	CString str ="x,y,z,regression,res,variancek,variance1,variance2,variance,p1,p2,p3,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10\n";
	fobjGLS.WriteString(str); 
	fobjGLS.SeekToEnd();
	for (int i =0;i<m_fianlPredictPoint.GetSize();i++)
		{	
			str.Format("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
									   m_fianlPredictPoint.GetAt(i).x,
				                       m_fianlPredictPoint.GetAt(i).y,
									   m_fianlPredictPoint.GetAt(i).z,
									   m_fianlPredictPoint.GetAt(i).regression,
									   m_fianlPredictPoint.GetAt(i).reskrig,
									   m_fianlPredictPoint.GetAt(i).varianceK,
									   m_fianlPredictPoint.GetAt(i).variance1,
									   m_fianlPredictPoint.GetAt(i).variance2,
									   m_fianlPredictPoint.GetAt(i).variance,
									   m_fianlPredictPoint.GetAt(i).para1,
									   m_fianlPredictPoint.GetAt(i).para2,
									   m_fianlPredictPoint.GetAt(i).para3,
									   m_fianlPredictPoint.GetAt(i).bf[0],
									   m_fianlPredictPoint.GetAt(i).bf[1],
									   m_fianlPredictPoint.GetAt(i).bf[2],
									   m_fianlPredictPoint.GetAt(i).bf[3],
									   m_fianlPredictPoint.GetAt(i).bf[4],
									   m_fianlPredictPoint.GetAt(i).bf[5],
									   m_fianlPredictPoint.GetAt(i).bf[6],
									   m_fianlPredictPoint.GetAt(i).bf[7],
									   m_fianlPredictPoint.GetAt(i).bf[8],
									   m_fianlPredictPoint.GetAt(i).bf[9],
									   m_fianlPredictPoint.GetAt(i).bf[10]						  
									   );								   
			fobjGLS.WriteString(str);   

		}
	fobjGLS.Close();  
}





















