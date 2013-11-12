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
	//���������, ��� �� ������ � ��������� ������� ������ �����
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
	BOOL m_bStarted; //��������� ��������� - ���������������� �� ���������
	BOOL m_bPrima; //��������� ��������� - �������� �� �����-�������
	BOOL m_bDeikstra; //��������� ��������� - �������� �� ��������
	BOOL m_bSetCoords; //����� �� ��������� ���������� ������?
	BOOL m_bOrient; //��������������� ����� ��� ���
	
	UINT m_nNumColumns; //���������� ������ �����
	UINT m_nCounts;  //������� ��������� ������ ��� �������
	
	UINT m_nStart;  //��� ��������� �������� - ������� ������ � ����� ������
	UINT m_nFinish;

	CArray <CPoint> m_PointsArray; //������ ��������� �������
	CArray <DOUBLE> m_VertexArray; //������ ���������� ����� ��������
	CArray <DOUBLE> m_PrimaArray; //������ �����, �������� ���� ��� ������ ������������ ������ �����
	CArray <UINT> m_DeikstraPath; //������ ������, �������� ��������� ���������� ����
};
