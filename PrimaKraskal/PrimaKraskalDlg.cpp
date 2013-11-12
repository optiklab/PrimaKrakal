//////////////////////////////////////////////////////////
//                  Coded by OptikLab                    /
//                                                       /
//                         **                            /
//                        /()\                           /
//                         /\                            / 
//                        2009                           /
//////////////////////////////////////////////////////////

// PrimaKraskalDlg.cpp : implementation file
#include "stdafx.h"
#include "PrimaKraskal.h"
#include "PrimaKraskalDlg.h"
#include "EnterDialog.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPrimaKraskalDlg dialog
CPrimaKraskalDlg::CPrimaKraskalDlg(CWnd* pParent)	: CDialog(CPrimaKraskalDlg::IDD, pParent),
																m_nNumColumns(0),
																m_bSetCoords(FALSE),
																m_nCounts(0),
																m_bStarted(FALSE), 
																m_bOrient(FALSE),
																m_bPrima(FALSE),
																m_bDeikstra(FALSE),
																m_nStart(0),
																m_nFinish(0)
{
	m_PointsArray.SetSize(0,1);
	m_VertexArray.SetSize(0,1);
	m_PrimaArray.SetSize(0,1);
	m_DeikstraPath.SetSize(0,1);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}



void CPrimaKraskalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, CMyDirections);
	DDX_Text(pDX, IDC_EDIT1, m_nNumColumns);
	DDX_Check(pDX, IDC_CHECK1, m_bOrient);
}




BEGIN_MESSAGE_MAP(CPrimaKraskalDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPrimaKraskalDlg::OnBnClickedButton1)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CPrimaKraskalDlg::OnNMDblclkList1)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_PRIMAKRASKAL, &CPrimaKraskalDlg::OnBnClickedPrimakraskal)
	ON_BN_CLICKED(IDC_DEIKSTRA, &CPrimaKraskalDlg::OnBnClickedDeikstra)
END_MESSAGE_MAP()



// CPrimaKraskalDlg message handlers
BOOL CPrimaKraskalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CPrimaKraskalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CPrimaKraskalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;	
		GetClipBox(dc, rect);
		CPoint TL = rect.TopLeft();  
		CPoint BR = rect.BottomRight();
		dc.Rectangle(TL.x+13, TL.y+290, BR.x-15, BR.y - 13); //���� ���������

		CString str;

		//������������ �����
		//����������� �����
		CPen Pen;
		UINT nPenStyle=PS_SOLID;
		UINT nPenWidth=1;
		COLORREF rgbPenColor = RGB(0,0,0);
		if(m_bPrima)
			rgbPenColor = RGB(230,230,230);
		if(HPEN(Pen)!=NULL)
			Pen.DeleteObject();
		Pen.CreatePen(nPenStyle,nPenWidth,rgbPenColor);
		if(HPEN(Pen)!=NULL)
			dc.SelectObject(&Pen);
		//�����
		for(INT nI=0; nI<m_PointsArray.GetSize(); nI++)
			for(INT nJ=0; nJ<m_PointsArray.GetSize(); nJ++)
			{
				dc.MoveTo(m_PointsArray.GetAt(nI));
				if(m_VertexArray.GetAt((nI*m_nNumColumns)+nJ) != 0)
					dc.LineTo(m_PointsArray.GetAt(nJ));
			}
		//�����-��������
		if(m_bPrima)
		{
			rgbPenColor = RGB(0,0,0);
			if(HPEN(Pen)!=NULL)
				Pen.DeleteObject();
			Pen.CreatePen(nPenStyle,nPenWidth,rgbPenColor);
			if(HPEN(Pen)!=NULL)
				dc.SelectObject(&Pen);
			for(INT nI=0; nI<m_PointsArray.GetSize(); nI++)
				for(INT nJ=0; nJ<m_PointsArray.GetSize(); nJ++)
				{
					dc.MoveTo(m_PointsArray.GetAt(nI));
					if(m_PrimaArray.GetAt((nI*m_nNumColumns)+nJ) != 0)
						dc.LineTo(m_PointsArray.GetAt(nJ));
				}
		}
		//��������
		if(m_bDeikstra)
		{
			nPenStyle=PS_SOLID;
			nPenWidth=2;
			COLORREF rgbPenColor = RGB(255,0,0);
			if(HPEN(Pen)!=NULL)
				Pen.DeleteObject();
			Pen.CreatePen(nPenStyle,nPenWidth,rgbPenColor);
			if(HPEN(Pen)!=NULL)
				dc.SelectObject(&Pen);

			INT Count = m_nStart - 1;
			for(INT nI = m_DeikstraPath.GetSize()-1; nI>0; nI--)
			{
				dc.MoveTo(m_PointsArray.GetAt(Count));
				Count = m_DeikstraPath.GetAt(nI);
				dc.LineTo(m_PointsArray.GetAt(Count));
			}
		}
		//������������ �������
		nPenStyle=PS_SOLID;
		nPenWidth=1;
		rgbPenColor = RGB(0,0,0);
		if(HPEN(Pen)!=NULL)
			Pen.DeleteObject();
		Pen.CreatePen(nPenStyle,nPenWidth,rgbPenColor);
		if(HPEN(Pen)!=NULL)
			dc.SelectObject(&Pen);
		for(INT nI=0; nI<m_PointsArray.GetSize(); nI++)
		{
			str.Format(_T("%d"),nI+1);
			dc.Ellipse(m_PointsArray.GetAt(nI).x-2,m_PointsArray.GetAt(nI).y-2,m_PointsArray.GetAt(nI).x+2, m_PointsArray.GetAt(nI).y+2);
			dc.TextOutW(m_PointsArray.GetAt(nI).x-5, m_PointsArray.GetAt(nI).y-20, str);
		}
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPrimaKraskalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//������� ��������� ������  "������"
void CPrimaKraskalDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if(!m_bStarted) //���� ������ �� ����� ��� �� ��� �����
		if(m_nNumColumns>=2 && m_nNumColumns<20)  
		{
			CRect ListClRect;
			CMyDirections.GetClientRect(&ListClRect);
			CString strBuf;
			//��������� ��� ������� � �������
			CMyDirections.InsertColumn(0, _T("������"), LVCFMT_LEFT, 60);
			for(UINT nI=1; nI<=m_nNumColumns; nI++)
			{
				strBuf.Format(_T("����� %d"),nI);
				CMyDirections.InsertColumn(nI, strBuf, LVCFMT_LEFT, 60);
			}
			//������������� �������� ������� �� ��������� � 0
			for(UINT nI=0; nI<m_nNumColumns*m_nNumColumns; nI++)
				m_VertexArray.Add(0);
			INT nCount = 0;
			//��������� ��� ������ �������
			for(UINT nI=m_nNumColumns; nI>=1; nI--)
			{
				strBuf.Format(_T("����� %d"),nI);
				CMyDirections.InsertItem(0,strBuf);
				for(UINT nJ=1; nJ<=m_nNumColumns; nJ++)
				{
					strBuf.Format(_T("%g"),m_VertexArray.GetAt(nCount++));
					CMyDirections.SetItemText(0,nJ,strBuf);
				}
			}
			m_bSetCoords=TRUE;
			CString str;  //��������� � ������������� ������� ��������� �����
			str.Format(_T("���������� %d ����� ������� � ����� �������������� ����!"),m_nNumColumns);
			AfxMessageBox(str);

			CEdit* pCtrl1 = (CEdit*)GetDlgItem(IDC_EDIT1); //������� �� �������� �� ������
			pCtrl1->SetReadOnly(TRUE);

			CButton* pCtrl2 = (CButton*)GetDlgItem(IDC_CHECK1);
			pCtrl2->EnableWindow(FALSE);

			m_bStarted = TRUE;//���� ��������
		}
		else
			AfxMessageBox(_T("������� ������� ��� ������� ��������� ���������� �������!"));

}



void CPrimaKraskalDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDblClk(nFlags, point);
}


//������� ��������� �����, ��� ������������ ����� ���������� ������� �����
void CPrimaKraskalDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bSetCoords && m_nCounts<m_nNumColumns) //���� ��� ����� �� � �� ����������������� 
	{
		m_PointsArray.Add(point); //��������� ����������
		m_nCounts+=1;
		if(m_nCounts==m_nNumColumns)
		{
			VertexCount(); //������� ����������
			m_bSetCoords = FALSE;
			m_nCounts=0;
		}
		Invalidate();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

//������� ����������
void CPrimaKraskalDlg::VertexCount()
{
	INT nCount = 0;
	for(INT nI=0; nI<m_PointsArray.GetSize(); nI++)
	{	
		for(INT nJ=0; nJ<m_PointsArray.GetSize(); nJ++)
		{  //��������� ���������� ����� ����� ���������� - �����������
			DOUBLE nX = (DOUBLE)abs(m_PointsArray.GetAt(nI).x-m_PointsArray.GetAt(nJ).x);
			DOUBLE nY = (DOUBLE)abs(m_PointsArray.GetAt(nI).y-m_PointsArray.GetAt(nJ).y);
			DOUBLE NV = sqrt(pow(nX,2)+pow(nY,2));
			m_VertexArray.SetAt(nCount++, NV);//���������� ���
		}
	}
	UpdateTable();
}

//�������� ���������� ������� �� ���������� �����
void CPrimaKraskalDlg::UpdateTable()
{
	CString strBuf;
	UINT nCount=0;

	for(UINT nI=0; nI<m_nNumColumns; nI++)
		for(UINT nJ=1; nJ<=m_nNumColumns; nJ++)
		{//������� �������� ������� �� ���� �������
			strBuf.Format(_T("%g"),m_VertexArray.GetAt(nCount));
			CMyDirections.SetItemText(nI,nJ,strBuf);
			nCount+=1;
		}
}

//���������, ��� �� ������ � ��������� ������� ������ �����
BOOL CPrimaKraskalDlg::IsCorrectData()
{
	DOUBLE dTemp = 0;
	UINT nCount = 0;
	for(UINT nI=0; nI<m_nNumColumns; nI++)
	{
		dTemp = 0;
		for(UINT nJ=0; nJ<m_nNumColumns; nJ++)
			dTemp += m_VertexArray.GetAt(nCount++);
		if(dTemp == 0)
			return FALSE;
	}
	return TRUE;
}



//��������� �������� ����� ����� �� ������� ����������
void CPrimaKraskalDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	//����� ������ �������� ������, � ������� ������� �����
	INT nStringID = CMyDirections.GetNextItem(-1, LVNI_SELECTED);
	if(nStringID>=0)
	{
		CEnterDialog Dlg;
		//��� ���� �������� � ������ �������� ���� ����� ������ ��������
		for(UINT nI = 0; nI<m_nNumColumns; nI++)
		{
			Dlg.m_dValue = m_VertexArray.GetAt((nStringID*m_nNumColumns)+nI);
			Dlg.m_nCount = nI+1;
			if(Dlg.DoModal()==IDOK)
			{  //���� ����� �������� �������, �� ���������� ���
				m_VertexArray.SetAt((nStringID*m_nNumColumns)+nI,Dlg.m_dValue);
				if(!m_bOrient)  //���� ���� �����������������, �� ���� ������ �������� � �� 2�� ����� �������
					m_VertexArray.SetAt((nI*m_nNumColumns)+nStringID,Dlg.m_dValue);
			}
		}
		UpdateTable();
	}
}


//������� �������������� ����, ���� ��� ������������
void CPrimaKraskalDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	Invalidate();
}



//������� �� ������ "��������� ����������� ���� (�����-�������)"
//��������� �������� �����-�������� � ������������������ �����
void CPrimaKraskalDlg::OnBnClickedPrimakraskal()
{
	//������� ������������������� ���� �� ������
	if(m_bStarted)
	{
		//���� ������ ���� �����������������
		if(!m_bOrient)
		{
			//�� ���������, ������� ������ �������������� - �� ���� �� ������ ���� ������ ��� �����
			if(IsCorrectData())
			{
				for(INT nI = 0; nI<m_VertexArray.GetSize(); nI++)
					m_PrimaArray.Add(0);
				UINT nJ=0;
				//���������� ������ ��������������� ������
				CArray <UINT> nUID;
				for(UINT nJ=0; nJ<m_nNumColumns; nJ++)
					nUID.Add(nJ);
				UINT v1 = 0, v2 = 0, id = 0;
				//�����-�������
				for(UINT nI=0; nI<m_nNumColumns; nI++)
					{
						//����� ����� �������� ����� � �����, ����������� ��� ������� v1 � v2 c ������� ID
						DOUBLE dMin = 100000000;//m_VertexArray.GetAt(0);
						for(UINT nJ=1; nJ<m_nNumColumns; nJ++)
							for(UINT nK=0; nK<nJ; nK++)
							{
								//��������� ����� ����� �� ���� ��������� ����� � ����  Min
								if((nUID.GetAt(nJ) != nUID.GetAt(nK)) &&  (dMin > m_VertexArray.GetAt(nJ*m_nNumColumns+nK)))
								{
									v1 = nJ;
									v2 = nK;
									dMin = m_VertexArray.GetAt(nJ*m_nNumColumns+nK);
								}
							}
						//��������� ����� �� ���� �����
						id = nUID.GetAt(v2); 
						for(UINT nI=0; nI<m_nNumColumns; nI++)
							if(nUID.GetAt(nI) == id)
								nUID.SetAt(nI,nUID.GetAt(v1));
						//������� � ������ ������� ���������� ����� ����������� ����������
						m_PrimaArray.SetAt(v1*m_nNumColumns+v2,m_VertexArray.GetAt(v1*m_nNumColumns+v2));
						m_PrimaArray.SetAt(v2*m_nNumColumns+v1,m_VertexArray.GetAt(v2*m_nNumColumns+v1));
					}
				m_bPrima = TRUE;
				UpdateTable();
				Invalidate();
			}
			else
				AfxMessageBox(_T("���� �������, � ������� ���� �����������!"));
		}
		else
			AfxMessageBox(_T("���� ������ ���� �����������������!"));
	}
	else
		AfxMessageBox(_T("��������� ������ �� �������!"));
}




//������� �� ������ "����� ����������� ���� �� ��������� ��������"
//��������� �������� �������� � �����
void CPrimaKraskalDlg::OnBnClickedDeikstra()
{
	//���� ���� ����� �������� - ������ � �����
	AfxMessageBox(_T("��� ���������� ��������� ����� ������ � ������. ������� �� � ��������� �������!"));
	CEnterDialog Dlg;
	for(UINT nI = 0; nI<2; nI++)
	{
		Dlg.m_dValue = nI+1;
		Dlg.m_nCount = nI+1;
		if(Dlg.DoModal()==IDOK)
		{ 
			if(nI==0)
				m_nStart = (UINT)Dlg.m_dValue;
			else if(nI==1)
				m_nFinish = (UINT)Dlg.m_dValue;
		}
	}
	if(m_nStart<1 || m_nStart>m_nNumColumns || m_nFinish<1 || m_nFinish>m_nNumColumns)
		AfxMessageBox(_T("��������� ������ ������ �� ����������!"));
	else
	{
		//������
		m_DeikstraPath.RemoveAll();
		CArray <SVertex> m_DeikstraArray;
		//�������������
		for(UINT nI = 0; nI<m_nNumColumns; nI++)
		{
			SVertex vTemp;
			vTemp.bMarked=FALSE;
			vTemp.dDistFromStart= 1000000;
			m_DeikstraArray.Add(vTemp);
		}
		INT nV = m_nStart-1; //� ��������� �������
		m_DeikstraArray.GetAt(nV).bMarked = TRUE;//��������� ������� ��������
		m_DeikstraArray.GetAt(nV).nPrevVertex = 0; //���������� ���
		m_DeikstraArray.GetAt(nV).dDistFromStart = 0; //�� ��� ���� 0
		while(1)
		{
			// ���������� ��� �������, ������� nV, � ���� ��� ��� ���������� ����
			for(UINT nI = 0; nI<m_nNumColumns; nI++)
				if(m_VertexArray.GetAt(nV*m_nNumColumns+nI) != 0)
					if(m_DeikstraArray.GetAt(nI).bMarked==FALSE && m_DeikstraArray.GetAt(nI).dDistFromStart > (m_DeikstraArray.GetAt(nV).dDistFromStart + m_VertexArray.GetAt(nV*m_nNumColumns+nI)))
					{
						m_DeikstraArray.GetAt(nI).dDistFromStart = m_DeikstraArray.GetAt(nV).dDistFromStart + m_VertexArray.GetAt(nV*m_nNumColumns+nI);
						m_DeikstraArray.GetAt(nI).nPrevVertex = nV;
					}
			// ���� �� ���� ���� ������������ ����� ����� ��������
			DOUBLE dMinDist = 100000000;
			nV = -1;

			for(UINT nI = 0; nI<m_nNumColumns; nI++)
			{
				// ���� ��� ������� �� ������ ���������� 
				// ���� � ���� ����� ���� � ������� u ������
				// ��� ���������, ��
				if(m_DeikstraArray.GetAt(nI).bMarked==FALSE && m_DeikstraArray.GetAt(nI).dDistFromStart < dMinDist)
				{
					dMinDist = m_DeikstraArray.GetAt(nI).dDistFromStart;
					nV = nI;  // ������� �������� ���������� nI-� �������
				}
			}
			if(nV == -1)
			{
				AfxMessageBox(_T("��� ������ ����!"));
				break;
			}
			else if(nV == m_nFinish-1)
			{
				UINT nI = m_nFinish-1;
				CString Buf = _T("");
				m_DeikstraPath.Add(m_nFinish-1);
				while(nI != m_nStart-1)
				{
					m_DeikstraPath.Add(nI);
					nI = m_DeikstraArray.GetAt(nI).nPrevVertex;		
				}
				
				m_bDeikstra = TRUE;
				Buf.Format(_T("����� ���� %g"), m_DeikstraArray.GetAt(m_nFinish-1).dDistFromStart);
				AfxMessageBox(Buf);
				break;
			}
			m_DeikstraArray.GetAt(nV).bMarked = TRUE;
		}
		Invalidate();
	}
}
