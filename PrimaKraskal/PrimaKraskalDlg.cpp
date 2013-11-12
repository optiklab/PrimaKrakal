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
		dc.Rectangle(TL.x+13, TL.y+290, BR.x-15, BR.y - 13); //поле рисования

		CString str;

		//Отрисовываем графы
		//Настраиваем кисть
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
		//Общий
		for(INT nI=0; nI<m_PointsArray.GetSize(); nI++)
			for(INT nJ=0; nJ<m_PointsArray.GetSize(); nJ++)
			{
				dc.MoveTo(m_PointsArray.GetAt(nI));
				if(m_VertexArray.GetAt((nI*m_nNumColumns)+nJ) != 0)
					dc.LineTo(m_PointsArray.GetAt(nJ));
			}
		//Прима-Краскала
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
		//Дейкстра
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
		//отрисовываем вершины
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


//Функция обработки кнопки  "Начать"
void CPrimaKraskalDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if(!m_bStarted) //Если расчет до этого ещё не был начат
		if(m_nNumColumns>=2 && m_nNumColumns<20)  
		{
			CRect ListClRect;
			CMyDirections.GetClientRect(&ListClRect);
			CString strBuf;
			//добавляем все колонки в таблицу
			CMyDirections.InsertColumn(0, _T("Города"), LVCFMT_LEFT, 60);
			for(UINT nI=1; nI<=m_nNumColumns; nI++)
			{
				strBuf.Format(_T("Город %d"),nI);
				CMyDirections.InsertColumn(nI, strBuf, LVCFMT_LEFT, 60);
			}
			//Устанавливаем значения массива по умолчанию в 0
			for(UINT nI=0; nI<m_nNumColumns*m_nNumColumns; nI++)
				m_VertexArray.Add(0);
			INT nCount = 0;
			//добавляем все строки таблицы
			for(UINT nI=m_nNumColumns; nI>=1; nI--)
			{
				strBuf.Format(_T("Город %d"),nI);
				CMyDirections.InsertItem(0,strBuf);
				for(UINT nJ=1; nJ<=m_nNumColumns; nJ++)
				{
					strBuf.Format(_T("%g"),m_VertexArray.GetAt(nCount++));
					CMyDirections.SetItemText(0,nJ,strBuf);
				}
			}
			m_bSetCoords=TRUE;
			CString str;  //сообщение о необходимости выбрать НЕСКОЛЬКО точек
			str.Format(_T("Расставьте %d точек кликами в белом прямоугольнике ниже!"),m_nNumColumns);
			AfxMessageBox(str);

			CEdit* pCtrl1 = (CEdit*)GetDlgItem(IDC_EDIT1); //элемент не доступен на запись
			pCtrl1->SetReadOnly(TRUE);

			CButton* pCtrl2 = (CButton*)GetDlgItem(IDC_CHECK1);
			pCtrl2->EnableWindow(FALSE);

			m_bStarted = TRUE;//была запущена
		}
		else
			AfxMessageBox(_T("Слишком большое или слишком маленькое количество городов!"));

}



void CPrimaKraskalDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDblClk(nFlags, point);
}


//Функция считывает места, где пользователь хочет разместить вершины графа
void CPrimaKraskalDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bSetCoords && m_nCounts<m_nNumColumns) //если уже можно но и не переусердствовали 
	{
		m_PointsArray.Add(point); //считываем координаты
		m_nCounts+=1;
		if(m_nCounts==m_nNumColumns)
		{
			VertexCount(); //считаем расстояния
			m_bSetCoords = FALSE;
			m_nCounts=0;
		}
		Invalidate();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

//считает расстояния
void CPrimaKraskalDlg::VertexCount()
{
	INT nCount = 0;
	for(INT nI=0; nI<m_PointsArray.GetSize(); nI++)
	{	
		for(INT nJ=0; nJ<m_PointsArray.GetSize(); nJ++)
		{  //вычисляем расстояние между двумя вершинками - гиппотенузу
			DOUBLE nX = (DOUBLE)abs(m_PointsArray.GetAt(nI).x-m_PointsArray.GetAt(nJ).x);
			DOUBLE nY = (DOUBLE)abs(m_PointsArray.GetAt(nI).y-m_PointsArray.GetAt(nJ).y);
			DOUBLE NV = sqrt(pow(nX,2)+pow(nY,2));
			m_VertexArray.SetAt(nCount++, NV);//запоминаем его
		}
	}
	UpdateTable();
}

//функцмия обновления таблицы со значениями путей
void CPrimaKraskalDlg::UpdateTable()
{
	CString strBuf;
	UINT nCount=0;

	for(UINT nI=0; nI<m_nNumColumns; nI++)
		for(UINT nJ=1; nJ<=m_nNumColumns; nJ++)
		{//Выводим значения массива во всей таблице
			strBuf.Format(_T("%g"),m_VertexArray.GetAt(nCount));
			CMyDirections.SetItemText(nI,nJ,strBuf);
			nCount+=1;
		}
}

//Проверяет, нет ли ошибок в начальной таблице данных графа
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



//обработка двойного клика мышью на таблице расстояний
void CPrimaKraskalDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	//берем индекс элемента списка, в который тыкнули мышью
	INT nStringID = CMyDirections.GetNextItem(-1, LVNI_SELECTED);
	if(nStringID>=0)
	{
		CEnterDialog Dlg;
		//для всех столбцов в строке вызываем окно ввода нового значения
		for(UINT nI = 0; nI<m_nNumColumns; nI++)
		{
			Dlg.m_dValue = m_VertexArray.GetAt((nStringID*m_nNumColumns)+nI);
			Dlg.m_nCount = nI+1;
			if(Dlg.DoModal()==IDOK)
			{  //если новое значение введено, то запоминаем его
				m_VertexArray.SetAt((nStringID*m_nNumColumns)+nI,Dlg.m_dValue);
				if(!m_bOrient)  //если граф неориентированный, то надо менять значение и во 2ом месте таблицы
					m_VertexArray.SetAt((nI*m_nNumColumns)+nStringID,Dlg.m_dValue);
			}
		}
		UpdateTable();
	}
}


//Функция перерисовывает окно, если оно активируется
void CPrimaKraskalDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	Invalidate();
}



//Нажатие на кнопку "Расчитать оптимальный граф (Прима-Краскал)"
//Применяет алгоритм Прима-Краскала к неориентированному графу
void CPrimaKraskalDlg::OnBnClickedPrimakraskal()
{
	//таблица проинициализирована хотя бы нулями
	if(m_bStarted)
	{
		//граф должен быть неориентированный
		if(!m_bOrient)
		{
			//По умолчанию, решение должно присутствовать - то есть не должно быть вершин без путей
			if(IsCorrectData())
			{
				for(INT nI = 0; nI<m_VertexArray.GetSize(); nI++)
					m_PrimaArray.Add(0);
				UINT nJ=0;
				//генерируем массив идентификаторов вершин
				CArray <UINT> nUID;
				for(UINT nJ=0; nJ<m_nNumColumns; nJ++)
					nUID.Add(nJ);
				UINT v1 = 0, v2 = 0, id = 0;
				//Прима-Краскал
				for(UINT nI=0; nI<m_nNumColumns; nI++)
					{
						//найти самое короткое ребро в графе, соединяющее две вершины v1 И v2 c разными ID
						DOUBLE dMin = 100000000;//m_VertexArray.GetAt(0);
						for(UINT nJ=1; nJ<m_nNumColumns; nJ++)
							for(UINT nK=0; nK<nJ; nK++)
							{
								//проверяем чтобы ребро не было добавлено ранее и ищем  Min
								if((nUID.GetAt(nJ) != nUID.GetAt(nK)) &&  (dMin > m_VertexArray.GetAt(nJ*m_nNumColumns+nK)))
								{
									v1 = nJ;
									v2 = nK;
									dMin = m_VertexArray.GetAt(nJ*m_nNumColumns+nK);
								}
							}
						//проверяем чтобы не было цикла
						id = nUID.GetAt(v2); 
						for(UINT nI=0; nI<m_nNumColumns; nI++)
							if(nUID.GetAt(nI) == id)
								nUID.SetAt(nI,nUID.GetAt(v1));
						//заносим в чистую таблицу расстояний новое оптимальное расстояние
						m_PrimaArray.SetAt(v1*m_nNumColumns+v2,m_VertexArray.GetAt(v1*m_nNumColumns+v2));
						m_PrimaArray.SetAt(v2*m_nNumColumns+v1,m_VertexArray.GetAt(v2*m_nNumColumns+v1));
					}
				m_bPrima = TRUE;
				UpdateTable();
				Invalidate();
			}
			else
				AfxMessageBox(_T("Есть вершины, к которым пути отсутствуют!"));
		}
		else
			AfxMessageBox(_T("Граф должен быть неориентированным!"));
	}
	else
		AfxMessageBox(_T("Начальные данные не введены!"));
}




//Нажатие на кнопку "Найти оптимальный путь по алгоритму Дейкстра"
//Применяет алгоритм Дейкстра к графу
void CPrimaKraskalDlg::OnBnClickedDeikstra()
{
	//ввод двух точек маршрута - начала и конца
	AfxMessageBox(_T("Для вычисления требуются точки старта и финиша. Введите их в следующем диалоге!"));
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
		AfxMessageBox(_T("Введенные номера вершин не существуют!"));
	else
	{
		//расчет
		m_DeikstraPath.RemoveAll();
		CArray <SVertex> m_DeikstraArray;
		//инициализация
		for(UINT nI = 0; nI<m_nNumColumns; nI++)
		{
			SVertex vTemp;
			vTemp.bMarked=FALSE;
			vTemp.dDistFromStart= 1000000;
			m_DeikstraArray.Add(vTemp);
		}
		INT nV = m_nStart-1; //в стартовой позиции
		m_DeikstraArray.GetAt(nV).bMarked = TRUE;//начальная вершина отмечена
		m_DeikstraArray.GetAt(nV).nPrevVertex = 0; //предыдущих нет
		m_DeikstraArray.GetAt(nV).dDistFromStart = 0; //от нее путь 0
		while(1)
		{
			// Перебираем все вершины, смежные nV, и ищем для них кратчайший путь
			for(UINT nI = 0; nI<m_nNumColumns; nI++)
				if(m_VertexArray.GetAt(nV*m_nNumColumns+nI) != 0)
					if(m_DeikstraArray.GetAt(nI).bMarked==FALSE && m_DeikstraArray.GetAt(nI).dDistFromStart > (m_DeikstraArray.GetAt(nV).dDistFromStart + m_VertexArray.GetAt(nV*m_nNumColumns+nI)))
					{
						m_DeikstraArray.GetAt(nI).dDistFromStart = m_DeikstraArray.GetAt(nV).dDistFromStart + m_VertexArray.GetAt(nV*m_nNumColumns+nI);
						m_DeikstraArray.GetAt(nI).nPrevVertex = nV;
					}
			// Ищем из всех длин некратчайших путей самый короткий
			DOUBLE dMinDist = 100000000;
			nV = -1;

			for(UINT nI = 0; nI<m_nNumColumns; nI++)
			{
				// Если для вершины не найден кратчайший 
				// путь и если длина пути в вершину u меньше
				// уже найденной, то
				if(m_DeikstraArray.GetAt(nI).bMarked==FALSE && m_DeikstraArray.GetAt(nI).dDistFromStart < dMinDist)
				{
					dMinDist = m_DeikstraArray.GetAt(nI).dDistFromStart;
					nV = nI;  // текущей вершиной становится nI-я вершина
				}
			}
			if(nV == -1)
			{
				AfxMessageBox(_T("Нет такого пути!"));
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
				Buf.Format(_T("Длина пути %g"), m_DeikstraArray.GetAt(m_nFinish-1).dDistFromStart);
				AfxMessageBox(Buf);
				break;
			}
			m_DeikstraArray.GetAt(nV).bMarked = TRUE;
		}
		Invalidate();
	}
}
