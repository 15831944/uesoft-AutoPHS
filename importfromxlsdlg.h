#if !defined(AFX_IMPORTFROMXLSDLG_H__B50DF418_09BE_4479_9AD3_1EB20C186C24__INCLUDED_)
#define AFX_IMPORTFROMXLSDLG_H__B50DF418_09BE_4479_9AD3_1EB20C186C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportFromXLSDlg.h : header file
//
#define CONNECTSTRING_EXCEL "Provider=Microsoft.Jet.OLEDB.4.0;Extended properties=Excel 5.0;Data Source="

/////////////////////////////////////////////////////////////////////////////
// 
// ���ⲿ����ԭʼ���ݶԻ���Ļ���
//
// ���ôζԻ���ǰ��Ҫ����SetProjectID��SetProjectDbConnect
// ���ù��̵ı��������
//
/////////////////////////////////////////////////////////////////////////
class _Application;

#include "MyButton.h"
#include "PropertyWnd.h"	// ���Դ�����
#include "excel9.h"
#include "resource.h"

class CImportFromXLSDlg : public CDialog
{
public:
	// ��EXCEL�ļ��������ݵ��ӽṹ
	struct ImportFromXLSElement		
	{
		CString SourceFieldName;	// Excel����ʼ�к�
		int		ExcelColNum;		// Excel������.
		CString DestinationName;	// ��Ӧ��Ŀ�����ݿ���ֶ���
	};

	// ����EXCEL�ļ��������ݵĽṹ
	struct ImportFromXLSStruct
	{
		CString XLSFilePath;		// EXCEL�ļ���·��
		CString	XLSTableName;		// EXCEL�Ĺ�������
		int		BeginRow;			// ��ʼ������к�
		int		RowCount;			// ��Ҫ�ĵ���ļ�¼��

		CString ProjectDBTableName;	// ���뵽��Ŀ�Ĺ��̱���
		CString ProjectID;			// ���뵽��Ŀ�Ĺ�����

		ImportFromXLSElement *pElement;	// ָ���ӵ�Ԫ�Ľṹ
		int ElementNum;	//�ӵ�Ԫ�Ľṹ��Ŀ
	};

public:
	BOOL ImportExcelToAccess(ImportFromXLSStruct *pImportStruct);

// Construction
public:
	CImportFromXLSDlg(CWnd* pParent = NULL);   // standard constructor
	~CImportFromXLSDlg();

public:
	CPropertyWnd* GetPropertyWnd();
	CString GetHintInformation();
	void SetHintInformation(LPCTSTR szHint);

	CString GetRegSubKey();
	void SetRegSubKey(LPCTSTR szRegSubkey);

	_ConnectionPtr GetProjectDbConnect();
	void SetProjectDbConnect(_ConnectionPtr IConnection);

	CString GetProjectID();
	void SetProjectID(LPCTSTR szProjectID);


// Dialog Data
	//{{AFX_DATA(CImportFromXLSDlg)
	enum { IDD = IDD_IMPORT_FROM_XLS };
	CMyButton	m_OpenFileDlgButton;
	CString	m_XLSFilePath;
	CString	m_HintInformation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportFromXLSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// ����Ϣ���浽ע�����
	virtual void SaveSetToReg();
	// ��ע��������ݳ�ʼ��
	virtual void InitFromReg();
	
	// ��ʼ�����Դ���
	virtual BOOL InitPropertyWnd();

	// �ڵ���ʱ��֤�������ݵ���Ч��
	virtual BOOL ValidateData();

	// ִ�е������ݵĲ���
	virtual void BeginImport();

protected:
	CPropertyWnd m_PropertyWnd;		// ���Դ��ڿؼ�

private:
	void StringToNumber(CString strNO, long & nNO);

	CString m_strProjectID;				// ���̵ı��
	_ConnectionPtr m_ProjectDbConnect;	// ������ص����ݿ�
	_ConnectionPtr m_pConExcel;			// ��ADO��ʽ����EXCEL
	
	CString	m_RegSubKey;	// ���ڴ����Ϣ���Ӽ�����
	CString m_strPrecFilePath;

protected:
	BOOL OpenExcelTable(_RecordsetPtr pRsTbl, CString& strSheetName, CString strExcelFileName);
	BOOL ConnectExcelFile(const CString strExcelName, _ConnectionPtr & pConExcel);

	// Generated message map functions
	//{{AFX_MSG(CImportFromXLSDlg)
	virtual BOOL OnInitDialog();
	virtual afx_msg void OnOpenFiledlg();
	afx_msg void OnDestroy();
	virtual afx_msg void OnBeginImport();
	//}}AFX_MSG
	virtual afx_msg	void OnSelectChange(NMHDR *pNMHDR,LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTFROMXLSDLG_H__B50DF418_09BE_4479_9AD3_1EB20C186C24__INCLUDED_)
