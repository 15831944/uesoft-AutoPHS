// CommDBOper.cpp: implementation of the CCommDBOper class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "CommDBOper.h"
#include "autophs.h"
#include "dlgeditdb.h"
#include "InputBox.h"
#include "Shlwapi.h"
#pragma comment( lib, "Shlwapi" )



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define RET_OK    0   //����ִ����ȷ������
#define RET_FAILED -1 //����ִ�д��󷵻�ͨ����

extern const _TCHAR dbConnectionString[];


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommDBOper::CCommDBOper()
{

}

CCommDBOper::~CCommDBOper()
{

}

void CCommDBOper::WarnMessage(CString strWarn,int iWarn) 
{
	if (iWarn != 0) 
	{
		AfxMessageBox(strWarn);
	}
}


//��ָ�����ݿ��������
int CCommDBOper::ADOOpenDBCon(_ConnectionPtr &pCon,const CString strDBPathName,
							  const CString strDBFileName,CString &strPassword,int iWarn)
{
	//��������   ��ָ�����ݿ�
	//�������˵��   pCon Ҫ�򿪵����ݿ����,strDBPathName Ҫ���ӵ�ACCESS���ݿ��ļ�Ŀ¼
	//				strDBFileName Ҫ�򿪵����ݿ��ļ���,strPassword  ����
	//				iWarn �����淽ʽ  0,������,1,����,2,�������˳�����
	//����  LFX
	CString tstrDBPathName;
	tstrDBPathName = strDBPathName;
	
	if (!strDBFileName.IsEmpty()) 
	{
		//Ŀ¼�Ƿ�Ϸ�
		int iret1;
		iret1 = tstrDBPathName.FindOneOf("*?\"<>|");
		if (iret1 != -1)
		{
			CString strTemp;
			strTemp.Format("���ݿ�(%s)��Ŀ¼(%s)�Ƿ�!!!",strDBFileName,strDBPathName);
			WarnMessage(strTemp,iWarn);
			return RET_FAILED;
		}
		
		iret1 = tstrDBPathName.ReverseFind('\\');
		if (iret1  != 1)
		{
			tstrDBPathName += "\\";
		}
	}

	CString strDB;
	strDB.Empty();
	strDB += tstrDBPathName;
	strDB += strDBFileName;
	return ADOOpenDBCon(pCon,strDB,strPassword,iWarn);
}
int CCommDBOper::ADOOpenDBCon(_ConnectionPtr &pCon,const CString strDB,CString &strPassword,int iWarn)
{
	//��������   ��ָ�����ݿ�
	//�������˵��   pCon Ҫ�򿪵����ݿ����,strDB Ҫ���ӵ�ACCESS���ݿ��ļ�ȫ·��
	//	strPassword  ���� iWarn �����淽ʽ  0,������,1,����,2,�������˳�����
	//����  LFX
	CString strCon;
	_TCHAR dbConnectionString[]=_T("Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s;Jet OLEDB:Database Password=%s");
	strCon.Format(dbConnectionString,strDB,strPassword);
	if(!PathFileExists(_T(strDB)))
	{
		WarnMessage("Ҫ�򿪵����ݿ��ļ���������!!!",iWarn);
		return RET_FAILED;
	}

	if (pCon == NULL)
	{
		pCon.CreateInstance(__uuidof(Connection) );
	}
	else
	{
		ADOCloseConnectDB(pCon,0);
	}
	int iLoop = 0;
	while (true)
	{
		//����ACCESS���ݿ�
		iLoop++;
		//���û���룬��Ϊ��һ��ѭ��
		if ( strPassword.IsEmpty() && (iLoop <= 1))   
		{
			try
			{
				pCon->Open(_bstr_t(strCon),"","",-1);         
			}
			catch(_com_error &e)
			{
				CString strTemp;
				CInputBox inputBox;
				inputBox.m_bIsProtected = TRUE;
				strTemp.LoadString(IDS_NEEDPASSWORD);
				inputBox.m_strWndText.Format("%s%s",strDB,strTemp);
				inputBox.m_strTitle.LoadString(IDS_PLEASEINPUTDBPASSWORD);
				if (inputBox.DoModal() != IDOK)
				{
					WarnMessage("�������,�����ݿ�ʧ��!!!",iWarn);	
					return RET_FAILED;
				}
				else
				{
					strPassword = inputBox.m_strValue;
				}
			}
			break;
		}
		//���������
		try
		{
			pCon->Open(_bstr_t(strCon),"","",-1); 
		}
		catch (_com_error &e)
		{
			CString strMsg;
			strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
			AfxMessageBox(strMsg);
			CString errormessage;
			errormessage.Format("�����ݿ� %s ʧ��!\r\n������Ϣ:%s",strDB,e.ErrorMessage());
			WarnMessage(errormessage,iWarn);
			return RET_FAILED;			
		}
		break;
		
	} //END while(true)

	return RET_OK;
}


int CCommDBOper::ADOConExec(_ConnectionPtr &pCon,const CString strCmdSql,CString strDcrCmd,int iWarn)
{	
	//��������˵��: ��pCon(Connection)����ִ��ָ��SQL����
	//pCon ���ݿ����Ӷ���
	//strCmdSql  Ҫִ��SQL���������
	//strDcrCmd�Ƕ�Ҫʹ��SQL���������
	if (ADOConIsOpened(pCon,iWarn) != RET_OK)   //�жϵ�ǰ���Ӷ����Ƿ��Ѵ�
	{
		WarnMessage("ָ�����ݿ����δ����,�˳�!",iWarn);
		return RET_FAILED;
	}
	try
	{
		pCon->Execute(_bstr_t(strCmdSql),NULL,adCmdText);
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
		CString errormessage;
		errormessage.Format("����˵��: %s\r\n���ݿ����Ӷ���ִ������SQL����\r\n%s\r\nʧ��!  ������Ϣ:%s",strDcrCmd,strCmdSql,e.ErrorMessage());
		WarnMessage(errormessage,iWarn);///��ʾ������Ϣ
		return RET_FAILED;
	}
	return RET_OK;
}

int CCommDBOper::ADORecordsetOpen(_ConnectionPtr &pCon,_RecordsetPtr pRs,CString strOpenSql,CString strDcrOpen,int iWarn)
{
	//����˵��:ִ��ָ����¼������ָ��SQL����
	//pCon ���ݿ����Ӷ���
	//pRs  ��ǰ�����ü�¼������
	//strOpenSql  Ҫִ�е�SQL����
	//strDcrOpen�Ƕ�Ҫʹ��SQL���������

	_variant_t tempStrSql;
	tempStrSql = strOpenSql;
	if (pRs == NULL)
	{
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
	}

	ADOCloseRecordset(pRs,0);
	
	while (true)
	{
		try
		{
			//�д� pRs->Open(tempStrSql,_variant_t(pCon),adOpenDynamic,adLockOptimistic,adCmdText);
			pRs->Open(tempStrSql,(IDispatch *)pCon,adOpenDynamic,adLockOptimistic,adCmdText);
			break;
		}
		catch (_com_error &e)
		{
			CString strMsg;
			strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
			AfxMessageBox(strMsg);
		}
		try
		{	
			//�����һ�δ�ʧ��,���ܷ���ֻ����ʽ��
			pRs->Open(tempStrSql,(IDispatch *)pCon,adOpenDynamic,adLockReadOnly,adCmdText);
			return 2;
		}
		catch (_com_error &e)
		{
			CString strMsg;
			strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
			AfxMessageBox(strMsg);
			CString errormessage;
			errormessage.Format("����˵��: %s\r\n��¼������ִ������SQL����\r\n%s\r\nʧ��!  ������Ϣ:%s",strDcrOpen,strOpenSql,e.ErrorMessage());
			WarnMessage(errormessage,iWarn);//��ʾ������Ϣ
			return RET_FAILED;
		}
	}
	return RET_OK;
	
}


//�жϵ�ǰ���ݿ����Ӷ����Ƿ��
int CCommDBOper::ADOConIsOpened(_ConnectionPtr &pCon,int iWarn)
{
	//�жϵ�ǰ���Ӷ����Ƿ��Ѵ�
	if ((pCon == NULL) || pCon->State !=  adStateOpen)
	{
		//WarnMessage("�������ݿ����û��!",iWarn);
		return RET_FAILED;
	}
	else
	{
		return RET_OK;
	}
}

//�Ͽ�ָ�����ݿ����Ӷ�������
int CCommDBOper::ADOCloseConnectDB(_ConnectionPtr &pCon,int iWarn)
{
	if ((pCon != NULL) && (pCon->State == adStateOpen) )
	{
		try
		{
			pCon->Close();
		}
		catch (_com_error &e)
		{
			CString strMsg;
			strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
			AfxMessageBox(strMsg);
		}
	}
	return RET_OK;
}

//�ر�ָ����¼������
int CCommDBOper::ADOCloseRecordset(_RecordsetPtr pRs,int iWarn)
{
	if ((pRs != NULL) && (pRs->State == adStateOpen))
	{
		try
		{
			pRs->Close();
		}
		catch (_com_error &e)
		{
			CString strMsg;
			strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
			AfxMessageBox(strMsg);
		}
	}
	return RET_OK;
}

//ɾ����

int CCommDBOper::ADODropTable(_ConnectionPtr &pCon,const CString strTableName,int iWarn)
{
	CString strSql;
	CString strDcr;
	int iret1;
	
	//���Ա�治����
	iret1 = ADOTestTable(pCon,strTableName,iWarn);
	if ( iret1 == RET_OK)   
	{
		//�����
		strDcr.Format("ɾ����");
		strSql.Format("DROP TABLE %s",strTableName);
		iret1 = ADOConExec(pCon,strSql,strDcr,iWarn);
	}
	return iret1;
}

//���Ա��Ƿ����
int CCommDBOper::ADOTestTable(_ConnectionPtr &pCon,const CString strTableName,int iWarn)
{	
	_RecordsetPtr pRs;
	TESTHR(pRs.CreateInstance(__uuidof(Recordset)) );
	CString strSql;
	CString strDcr;
	int iret1;
	
	strDcr.Format("�򿪱�");
	strSql.Format("SELECT * FROM %s",strTableName);
	
 	iret1 = ADORecordsetOpen(pCon,pRs,strSql,strDcr,iWarn);
	ADOCloseRecordset(pRs,iWarn);
	return iret1;
}


int CCommDBOper::ADODelRecord(_ConnectionPtr pCon,const CString strTableName,const CString strDelCond,int iWarn)
{
	//��������: �ӱ���ɾ��ָ�������ļ�¼
	//delCond  ָ��ɾ������
	int iret1;
	
	iret1 = ADOTestTable(pCon,strTableName,iWarn);
	if (iret1 == RET_OK)
	{
		
		CString strSql;
		CString strDcr;
		
		//��������ֶ�Ϊ��
		if (strDelCond.IsEmpty())
		{
			strSql.Format("DELETE FROM %s",strTableName);
			strDcr.Format("ɾ���������м�¼");
		}
		else
		{
			CString strTempDelCond;
			
			strDcr.Format("ɾ������ָ��������¼");
			strTempDelCond.Format("WHERE ");
			strTempDelCond = strTempDelCond + strDelCond;
			strSql.Format("DELETE FROM %s %s",strTableName,strTempDelCond);
		}
		iret1 = ADOConExec(pCon,strSql,strDcr,iWarn);
	}
	return iret1;
}



#undef RET_OK
#undef RET_FAILED

