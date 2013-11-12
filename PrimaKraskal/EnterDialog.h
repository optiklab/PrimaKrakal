#pragma once
//////////////////////////////////////////////////////////
//                  Coded by OptikLab                    /
//                                                       /
//                         **                            /
//                        /()\                           /
//                         /\                            / 
//                        2009                           /
//////////////////////////////////////////////////////////

// CEnterDialog dialog

class CEnterDialog : public CDialog
{
	DECLARE_DYNAMIC(CEnterDialog)

public:
	CEnterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnterDialog();

// Dialog Data
	enum { IDD = IDD_ENTERDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DOUBLE m_dValue;
	UINT m_nCount;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_strString;
};
