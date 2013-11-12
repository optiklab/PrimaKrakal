//////////////////////////////////////////////////////////
//                  Coded by OptikLab                    /
//                                                       /
//                         **                            /
//                        /()\                           /
//                         /\                            / 
//                        2009                           /
//////////////////////////////////////////////////////////

// EnterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PrimaKraskal.h"
#include "EnterDialog.h"


// CEnterDialog dialog

IMPLEMENT_DYNAMIC(CEnterDialog, CDialog)

CEnterDialog::CEnterDialog(CWnd* pParent /*=NULL*/): CDialog(CEnterDialog::IDD, pParent), 
													m_dValue(0),
													m_nCount(0),
													m_strString(_T("¬ведите значение:"))
{

}

CEnterDialog::~CEnterDialog()
{
}

void CEnterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dValue);
	DDX_Text(pDX, IDC_LABEL, m_strString);
}


BEGIN_MESSAGE_MAP(CEnterDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CEnterDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CEnterDialog message handlers

void CEnterDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_dValue < 0)
		AfxMessageBox(_T("¬ведите неотрицательное число!"));
	else
		OnOK();
}

BOOL CEnterDialog::OnInitDialog()
{
	m_strString.Format(_T("¬ведите %d значение:"),m_nCount);
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
