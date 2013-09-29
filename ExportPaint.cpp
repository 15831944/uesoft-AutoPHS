// ExportPaint.cpp: implementation of the CExportPaint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "autophs2002.h"
#include "ExportPaint.h"
#include "SelEngVolDll.h"
#include "basDirectory.h"
#include "EDIBgbl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExportPaint::CExportPaint()
{
	  m_strVlmCODE = "";
	  m_strPrjID = "";

}

CExportPaint::~CExportPaint()
{

}
//ѡ�񹤳̾��
BOOL CExportPaint::SelEngVol()
{
	CSelEngVolDll dlg;
	LPTSTR lpstrAllPrjDBPathName;
	LPTSTR lpstrsortPathName;
	LPTSTR lpstrworkprjPathName;
		
	if( !GetIPEDPath() )
	{
		AfxMessageBox("��ǰ����û�а�װAutoIPED���!");
		//ѡ�񹤳�ʹ�õ����ݿ���AutoPHS�Ŀ�
		m_strDbPath = basDirectory::DBShareDir;//20071109 "ProjectDBDir" ��Ϊ "DBShareDir"
		SetCodePath( m_strDbPath );
		m_strWorkPath = basDirectory::ProjectDir;
		SetCodePath( m_strWorkPath );
		lpstrAllPrjDBPathName=(LPTSTR) (LPCTSTR) ( m_strDbPath + "AllPrjDB.mdb");
	}
	else
		//ʹ��IPED�Ŀ�
	{
		lpstrAllPrjDBPathName =(LPTSTR) (LPCTSTR) (m_strDbPath + "AutoIPED.mdb");
	}
	lpstrsortPathName     =(LPTSTR) (LPCTSTR) (m_strDbPath + "sort.mdb");
	lpstrworkprjPathName  =(LPTSTR) (LPCTSTR) (m_strWorkPath + "Workprj.mdb");
	if( !dlg.ShowEngVolDlg(lpstrworkprjPathName,lpstrAllPrjDBPathName,lpstrsortPathName) )
	{
		return false;
	}
	m_strPrjID = CString(dlg.m_SelPrjID);
	m_strVlmCODE = "J" + CString(dlg.m_SelVlmCODE);

	return true;
}
//�õ�IPEDע�����Ϣ
BOOL CExportPaint::GetIPEDPath()
{
	HKEY hKey,hKey1;

	char vd[256];
	unsigned long len;
	unsigned long vtype;

	CString strSubKey = "SOFTWARE\\��ɳ��������������޹�˾\\AutoIPED\\";
	if( RegOpenKey(HKEY_LOCAL_MACHINE, strSubKey, &hKey1) != ERROR_SUCCESS )
	{
		return false;
	}
	len = 256;
	if( RegQueryValueEx(hKey1, "AutoIPED_Version", NULL, &vtype, (unsigned char*)vd, &len) != ERROR_SUCCESS )
	{
		RegCloseKey( hKey1 );
		return false;
	}
	RegCloseKey( hKey1 );
	strSubKey += vd;
	strSubKey += "\\Directory\\";
	memset(vd,'\0',256);
	len = 256;

	if( RegOpenKey(HKEY_LOCAL_MACHINE, strSubKey, &hKey) != ERROR_SUCCESS )
	{
		return false;
	}
	//��Ŀ���ݿ�·��
	if( RegQueryValueEx(hKey, "EDInBox_prjDbDir", NULL, &vtype, (unsigned char*)vd, &len) != ERROR_SUCCESS )
	{
		RegCloseKey(hKey);
		return false;
	}
	m_strDbPath = vd;
	SetCodePath( m_strDbPath );

	//��ʱ���ݿ�·��
	memset(vd,'\0',256);
	len = 256;
	if( RegQueryValueEx(hKey, "EDInBox_prjDir", NULL, &vtype, (unsigned char*)vd, &len) != ERROR_SUCCESS )
	{
		RegCloseKey( hKey );
		return false;
	}
	m_strWorkPath  = vd;
	SetCodePath( m_strWorkPath );
	
	//��װ����·��
	memset(vd,'\0',256);
	len = 256;
	if(	RegQueryValueEx(hKey, "EDinBox_InsDir", NULL, &vtype, (unsigned char*)vd, &len) != ERROR_SUCCESS )
	{
		RegCloseKey( hKey );
		return false;
	}
	m_strInsPath = vd;
	SetCodePath( m_strInsPath );
	RegCloseKey( hKey );

	return true;

}
//����Ϊ��׼��·������ĩβ��'\'
void CExportPaint::SetCodePath(CString &strPath)
{
	strPath.TrimRight();
	while( strPath.Right(1) == "\\" )
	{
		strPath.Delete( strPath.GetLength()-1 );
	}
	strPath += "\\";
}
//����AutoIPED
bool CExportPaint::StartupAutoIPED()
{
	static PROCESS_INFORMATION	 ProcessInformation;
	STARTUPINFO	 StartupInfo;

	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO) );
	StartupInfo.cb=sizeof(StartupInfo);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;

	//����ǰ�򿪵ĳ�����WM_QUIT��Ϣ���ȴ�����Ĺر�
	::PostThreadMessage(ProcessInformation.dwThreadId, WM_QUIT, 0, 0 );
	::WaitForSingleObject(ProcessInformation.hThread, INFINITE);
	::CloseHandle(ProcessInformation.hThread);
	CloseHandle(ProcessInformation.hProcess);
	ZeroMemory(&ProcessInformation, sizeof(PROCESS_INFORMATION));

	//���̺��������ΪAutoPHS��ǰ��.
	m_strPrjID = EDIBgbl::SelPrjID;
	m_strVlmCODE =  EDIBgbl::SelJcdm;
	//������
	m_strInsPath += "AutoIPED.exe";
	m_strInsPath += "  -DATA_FILENAME "  + EDIBgbl::strExportPaint;
	m_strInsPath += "  -PROJECT_ID "     + m_strPrjID; 
	m_strInsPath += "  -PROJECT_ENGVOL " + m_strVlmCODE;
//	m_strInsPath += "  -RECORD_COUNT "   + strID;



	//��������
	BOOL bo=CreateProcess(NULL,m_strInsPath.GetBuffer(0),
		NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS ,NULL,NULL,
		&StartupInfo,&ProcessInformation);

	/*	
		static PROCESS_INFORMATION ProcessInfomation;
		STARTUPINFO StartupInfo;
		
		ZeroMemory(&StartupInfo,sizeof(STARTUPINFO));
		StartupInfo.cb=sizeof(StartupInfo);
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_SHOWDEFAULT;
		
		//����ǰ�򿪵ĳ�����WM_QUIT��Ϣ���ȴ�����Ĺر�
		::PostThreadMessage(ProcessInfomation.dwThreadId,WM_QUIT,0,0);
		::WaitForSingleObject(ProcessInfomation.hThread,INFINITE);
		::CloseHandle(ProcessInfomation.hThread);
		CloseHandle(ProcessInfomation.hProcess);
		ZeroMemory(&ProcessInfomation,sizeof(PROCESS_INFORMATION));
		strID.Format("%d",ExportInsul.m_ID);
		strVal.Format("%ld",ExportInsul.m_lEngVol);

		strPrjPath += "  -DATA_FILENAME "  + ExportInsul.m_strDataFileName;
		strPrjPath += "  -PROJECT_ID "     + ExportInsul.m_strProjectID; 
		strPrjPath += "  -PROJECT_ENGVOL " + strVal;
		strPrjPath += "  -RECORD_COUNT "   + strID;
		
		BOOL bo=CreateProcess(NULL,strPrjPath.GetBuffer(0),
			NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS ,NULL,NULL,
			&StartupInfo,&ProcessInfomation);
	}	

*/
	return true;
}
