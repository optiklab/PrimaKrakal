//////////////////////////////////////////////////////////
//                  Coded by OptikLab                    /
//                                                       /
//                         **                            /
//                        /()\                           /
//                         /\                            / 
//                        2009                           /
//////////////////////////////////////////////////////////

// PrimaKraskalDlg.h : header file
//

#pragma once
#include "afxcmn.h"

struct SVertex
{
	BOOL bMarked;
	DOUBLE dDistFromStart;
	INT nPrevVertex;
};

// CPrimaKraskalDlg dialog
class CPrimaKraskalDlg : public CDialog
{
// Construction
public:
	CPrimaKraskalDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PRIMAKRASKAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void VertexCount();
	void UpdateTable();
	//Проверяет, нет ли ошибок в начальной таблице данных графа
	BOOL IsCorrectData();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedPrimakraskal();
	afx_msg void OnBnClickedDeikstra();

public:
	CListCtrl CMyDirections;
	BOOL m_bStarted; //состояние программы - инициализирована ли программа
	BOOL m_bPrima; //состояние программы - посчитан ли Прима-Краскал
	BOOL m_bDeikstra; //состояние программы - посчитан ли Дейкстра
	BOOL m_bSetCoords; //можно ли считывать координаты вершин?
	BOOL m_bOrient; //ориентированные графы или нет
	
	UINT m_nNumColumns; //количество вершин графа
	UINT m_nCounts;  //сколько координат вершин уже записал
	
	UINT m_nStart;  //Для алгоритма Дейкстры - вершины начала и конца поиска
	UINT m_nFinish;

	CArray <CPoint> m_PointsArray; //массив координат городов
	CArray <DOUBLE> m_VertexArray; //массив расстояний между городами
	CArray <DOUBLE> m_PrimaArray; //массив весов, хранящий веса для вершин минимального остова графа
	CArray <UINT> m_DeikstraPath; //массив вершин, хранящий последний кратчайший путь
};
