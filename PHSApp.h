// PHSApp.h: interface for the CPHSApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHSAPP_H__2AEEAB99_3C46_4DBC_8422_B3FBBE5026FB__INCLUDED_)
#define AFX_PHSAPP_H__2AEEAB99_3C46_4DBC_8422_B3FBBE5026FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
#include "..\Standardofboltnut.h"
#include "..\Standardofclamp.h"
#include "..\standardofconnect.h"
#include "..\standardofconstspring.h"
#include "..\standardofsa.h"
#include "..\standardofspring.h"
#include "..\standardofAttach.h"
*/
#define DllExport __declspec( dllexport )

class DllExport CPHSApp  
{
public:
	void CloseConnection();
	void InitApplication();
	void SaveApplication();
	long GetCurProjectVolumeID();
	void SetCurProjectVolumeID(long iVlmID);
	const _ConnectionPtr& GetWorkPrjConnection();
	const _ConnectionPtr& GetZdjcrudeConnection();
	const _ConnectionPtr& GetSortConnection();
	const _ConnectionPtr& GetAllPrjDBConnection();

	CString GetWorkPrjDBPath();
	CString GetZdjcrudeDBPath();
	CString GetSortDBPath();
	CString GetAllPrjDBPath();

	INT GetCurZdjh();

	void SetCurZdjh(INT nZdjh);

	CString GetCurPATable();
	CString GetCurPAFixTable();

	CString GetCurSATable();

	CString GetCurSABoltNutTable();

	CString GetCurBoltNutTable();

	CString GetCurRodTable();

	CString GetCurConnectTable();

	CString GetCurProfiledBarTable();

	CString GetCurSpringTable();

	CString GetCurConstSpringTable();
	
	CString GetCurAttachTable();

	CString GetConstSpringRodDiameterTable();

	void SetCurStringNum(LONG nNum);
	LONG GetCurStringNum();

	//�Ƿ�Ϊ��װ��ĵ�һ�γ�������
	BOOL IsInstalledAndFirstRun(); 
	void SetInstalledAndFirstRun(BOOL bFirstRun);

	//�����Ƿ����������
	bool IsStartedOnceAPHS();
	void SetStartedOnceAPHS(bool bStarted);

	//�ܲ�ѡ��淶
	CString GetPhsPASel();
	void SetPhsPASel(CString strSel);

	//����ѡ��淶
	CString GetPhsSASel();
	void SetPhsSASel(CString strSel);

	//���Ӽ�ѡ��淶
	CString GetPhsPARTSel();
	void SetPhsPARTSel(CString strSel);
	
	//����ѡ��淶
	CString GetPhsSPRINGSel();
	void SetPhsSPRINGSel(CString strSel);

	//��������ѡ��淶
	CString GetPhsConstSpringSel();
	void SetPhsConstSpringSel(CString strSel);
	
	//��˨��ĸѡ��淶
	CString GetPhsBoltsNutsSel();
	void SetPhsBoltsNutsSel(CString strSel);

	//��ȡ�������֧���ܹ淶
	void GetRecentWorkSPEC();
		
	//�����������֧���ܹ淶
	void SaveRecentWorkSPEC();

	//ע�����ϵͳע������ǰ׺
	//���� shit,��֪������
	CString GetRegAppPreName();
	void SetRegPreAppName(CString strAppName);

	CPHSApp();
	virtual ~CPHSApp();


private:
	_ConnectionPtr m_pConAllPrj;
	_ConnectionPtr m_pConWorkPrj;
	_ConnectionPtr m_pConSort;
	_ConnectionPtr m_pConZdjCrude;
	INT m_nCurStringNum;//��ǰ·��
};

DllExport extern CPHSApp theAutoPhsApp;
DllExport CPHSApp* GetPHSApp();
DllExport BOOL IsNumber(CString& str);
#endif // !defined(AFX_PHSAPP_H__2AEEAB99_3C46_4DBC_8422_B3FBBE5026FB__INCLUDED_)
