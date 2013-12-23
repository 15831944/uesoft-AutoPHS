// CommDBOper.h: interface for the CCommDBOper class.
//
//////////////////////////////////////////////////////////////////////

//����		  CCommDBOper   (ͨ�����ݿ����)
//˵��		  �����еĳ�Ա������Ϊ��̬��Ա����
//			  ����������ADO,DAO�����ݿ��̷�ʽ��,�޸�,��ѯ,ɾ���Ȳ������а�װ,�Լ򻯹���
//����		  ����ϲ
//��������    2005.2.25

#if !defined(AFX_COMMDBOPER_H__83C62FDD_08FC_498E_BA3B_75D057844645__INCLUDED_)
#define AFX_COMMDBOPER_H__83C62FDD_08FC_498E_BA3B_75D057844645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning ( disable : 4786 )

class CCommDBOper  
{
	//LFX �� 2005.2.25
public:
	static inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};
	static void WarnMessage(CString strWarn,int iWarn = 1);
	static int ADOOpenDBCon(_ConnectionPtr &pCon,const CString strDBPathName,const CString strDBFileName,CString &strPassword,int iWarn = 1);
	static int ADOOpenDBCon(_ConnectionPtr &pCon,const CString strDB,CString &strPassword,int iWarn = 1);
	static int ADOConExec(_ConnectionPtr &pCon,const CString strCmdSql,CString strDcrCmd = "",int iWarn = 1);
	static int ADORecordsetOpen(_ConnectionPtr &pCon,_RecordsetPtr &pRs,CString strOpenSql,CString strDcrOpen = "",int iWarn = 1);
	static int ADOConIsOpened(_ConnectionPtr &pCon,int iWarn = 1);
	static int ADOCloseConnectDB(_ConnectionPtr &pCon,int iWarn = 1);
	static int ADOCloseRecordset(_RecordsetPtr &pRs,int iWarn = 1);
	static int ADOTestTable(_ConnectionPtr &pCon,const CString strTableName,int iWarn = 1);
	static int ADODelRecord(_ConnectionPtr pCon,const CString strTableName,const CString strDelCond,int iWarn = 1);
	static int ADODropTable(_ConnectionPtr &pCon,const CString tableName,int iWarn = 1);

private:
	CCommDBOper();
	virtual ~CCommDBOper();
	// END LFX


};

#endif // !defined(AFX_COMMDBOPER_H__83C62FDD_08FC_498E_BA3B_75D057844645__INCLUDED_)
