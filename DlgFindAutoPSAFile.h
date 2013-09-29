#if !defined(AFX_DLGFINDAUTOPSAFILE_H__18F68D47_3367_4661_8C79_4344C57AEED8__INCLUDED_)
#define AFX_DLGFINDAUTOPSAFILE_H__18F68D47_3367_4661_8C79_4344C57AEED8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFindAutoPSAFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFindAutoPSAFile dialog

class CDlgFindAutoPSAFile : public CDialog
{
// Construction
public:
	short GetCAESARIIRecentFile();
	//��û��AutoPSA�ļ���ʱ,״̬Ϊ0;
	//��ѡ��һ��AutoPSA�ļ���ʱ,״̬Ϊ1
	//��ѡ�������ťʱ,״̬Ϊ2
	//Ĭ��Ϊ0;
	int m_bFlag;
	CDlgFindAutoPSAFile(CWnd* pParent = NULL);   // standard constructor
	int GetPSARecentFile();
	int GetPHSRecentFile();

// Dialog Data
	//{{AFX_DATA(CDlgFindAutoPSAFile)
	enum { IDD = IDD_DLG_FIND_AUTOPSA };
	CListCtrl	m_ctrlFileList;
	int m_iFileOpen;//ѡ��������ļ��ĳ���
	//}}AFX_DATA

	int m_nCount;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFindAutoPSAFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFindAutoPSAFile)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkFind(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnConfirm();
	afx_msg void OnRecenfile();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strFileList[16];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFINDAUTOPSAFILE_H__18F68D47_3367_4661_8C79_4344C57AEED8__INCLUDED_)
