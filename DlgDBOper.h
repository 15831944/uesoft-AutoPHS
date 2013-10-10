#if !defined(AFX_DLGDBOPER_H__674AC3F5_C982_4151_8F14_BEF7B105C8CA__INCLUDED_)
#define AFX_DLGDBOPER_H__674AC3F5_C982_4151_8F14_BEF7B105C8CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDBOper.h : header file
//
#include "datagridEx.h"
#include "XListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDBOper dialog

class CDlgDBOper : public CDialog
{
// Construction
private:
	int InitDlgData();
	int PromptSaveChange();
	int DisplayAllTable();
	int InitDlgWithoutPhs();
	int InitDlgWithPhs();
	int SetTableList(int index = 0,int iWarn = 1);
	int InitDlgCommFormat();
	int SetTableDGFromListBox(int index,int iWarn = 1);
	int ResetTableList();

public:

	int CloseDlg();

	int OpenDB(CString strDBName,int iWarn = 1);
	int OpenDB(int iWarn = 1);
	CDlgDBOper(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDBOper)
	enum { IDD = IDD_DLG_DBOPER };
	CButton	m_ButUndoChange;
	CButton	m_ButSaveChange;
	CButton	m_CheckDispAllTable;
	CButton	m_CheckAutoSave;
	CXListBox	m_StandardList;
	CStatic	m_StaticRecordPos;
	CButton	m_CheckAllowEdit;
	CButton	m_CheckAllowDelete;
	CButton	m_CheckAllowAppend;
	CButton	m_ButSetDescribe;
	CXListBox	m_TableTypeList;
	CXListBox	m_TableList;
	CDataGridEx	m_TableDG;
	//}}AFX_DATA
	//CStatusBar m_StatusBar;   //״̬������

	
	_ConnectionPtr m_pSortCon;  //Sort���ݿ����Ӷ���ָ��
	_ConnectionPtr m_pCon;//�������ݿ����ָ��
	//֮���Զ��� m_pDGCon,����Ϊ�ڱ��Ի����������еĲ鿴�������ݱ����ڵ����ݿ�����б仯
	//���鿴���ǵ�ǰϵͳ���ݱ� zdjcrude��ʱ�����Sort���ж�ȡ�������˲鿴�����ʱҪ
	//��Sort���ж�ȡ��������ݣ�Ϊ�����Ժ�ı�̣����������ٶ�������һ��ָ��
	_ConnectionPtr m_pDGCon;   //��ǰ���鿴���ݱ����ڵ����ݿ����ָ��
	_RecordsetPtr m_pRs;   //��ǰ���������ָ��
	_RecordsetPtr m_pCurPhsManuTbRs;   //��ǰ�������ݿ��еĵ�ǰ������¼������ָ��
	_RecordsetPtr m_pPhsManuTbRs;   //ָ��SORT���ݿ���PhsManuTbDescribe�ļ�¼��ָ��
	_RecordsetPtr m_pPhsManuTbStruRs;//ָ��SORT���ݿ���PhsManuTbStruDescribe�ļ�¼��ָ��
	_RecordsetPtr m_pPhsDataRs;      //ָ��SORT���ݿ���PhsDataTbStruDescribe�ļ�¼��ָ��
	_RecordsetPtr m_pTRs;  //��ʱָ��

	CString m_strDBName;    //��ǰ�������ݿ���
	CString m_strDBPassword; //��ǰ�������ݿ�����

	//CWnd *m_pCallWnd;             //���ڱ�Ĵ��ڵ��ñ�����ʱ,�洢�ô��ڵľ��ָ��	
	bool m_bAutoOpen;		//true Ϊ������ m_strDBName��Ϊ��ʱ,���ڳ�ʼ��ʱ�Զ��򿪸����ݿ�

	CString m_strTableType;       //�����ⲿ���ñ�����ʱ,�洢Ҫ���õ����ݱ�����
	CString m_strTableStandard;   //�����ⲿ���ñ�����ʱ,�洢Ҫ���õ����ݱ��׼

private:
	CString m_strCurTbName;//��ǰ�������ݱ����
	CString m_strCurTableType;//��ǰ�������ݱ�����
	CString m_strCurManuTbName;//��ǰ�������ݱ�Ĺ������
	CString m_strTSQL;         //��ʱSQL���
	CString m_strTemp;

	int m_iRet;          //���ڽ������ͺ�������ֵ
	int m_iTemp;

	int m_iModified;             //�޸ı�־
	bool m_bPHSManu;             //��ǰ�����ݱ��Ƿ�Ϊ�����   

	long m_lRun;          //��ǰ�Ի���������DATAGRID�������ݱ�Ĵ���

	CRect m_rcMainDlg;
	int m_iH;    //�ؼ�ԭ��
	int m_iW;    //�ؼ�ԭ��
	int m_fPerH;






// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDBOper)
	public:
	virtual BOOL Create(UINT IDD ,CWnd* pParentWnd=NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DECLARE_DYNCREATE(CDlgDBOper)
	// Generated message map functions
	//{{AFX_MSG(CDlgDBOper)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOpenDB();
	virtual void OnCancel();
	afx_msg void OnClosecommdbdlg();
	afx_msg void OnSelchangeTablelist();
	afx_msg void OnSelchangeTabletypelist();
	afx_msg void OnSelchangeStandardlist();
	afx_msg void OnCheckallowappend();
	afx_msg void OnCheckallowdelete();
	afx_msg void OnCheckallowedit();
	afx_msg void OnCheckautosave();
	afx_msg void OnChangeDatagrid();
	afx_msg void OnCheckdispalltable();
	afx_msg void OnClose();
	afx_msg void OnButclosedlg();
	afx_msg void OnButsavechange();
	afx_msg void OnButundochange();
	afx_msg void OnAfterDeleteDatagrid();
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//declspec(dllexport) extern CDlgDBOper dlgDBOper;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDBOPER_H__674AC3F5_C982_4151_8F14_BEF7B105C8CA__INCLUDED_)
