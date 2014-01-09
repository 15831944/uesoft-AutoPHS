// PHSApp.cpp: implementation of the CPHSApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PHSApp.h"
#include "modphscal.h"
#include "user.h"
#include "modSelPDSV.h"	
#include "basdirectory.h"
#include "edibgbl.h"
#include "modregistry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CPHSApp theAutoPhsApp;

CPHSApp::CPHSApp()
{
	m_nCurStringNum = 1;
}

CPHSApp::~CPHSApp()
{
//	this->SaveApplication();
	this->CloseConnection();
}

const _ConnectionPtr CPHSApp::GetAllPrjDBConnection()
{
	try
	{
		if(m_pConAllPrj==NULL)
			m_pConAllPrj.CreateInstance(__uuidof(Connection));
		if(m_pConAllPrj->State == adStateClosed)
			m_pConAllPrj->Open(_bstr_t(dbConnectionString + GetAllPrjDBPath()),"","",-1);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	return m_pConAllPrj;
}

const _ConnectionPtr CPHSApp::GetSortConnection()
{
	if(m_pConSort==NULL)
		m_pConSort.CreateInstance(__uuidof(Connection));
	if(m_pConSort->State == adStateClosed)
		m_pConSort->Open(_bstr_t(dbConnectionString + GetSortDBPath()),"","",-1);

	return m_pConSort;
}

const _ConnectionPtr CPHSApp::GetZdjcrudeConnection()
{
	if(m_pConZdjCrude==NULL)
		m_pConZdjCrude.CreateInstance(__uuidof(Connection));
	if(m_pConZdjCrude->State == adStateClosed)
		m_pConZdjCrude->Open(_bstr_t(dbConnectionString + GetZdjcrudeDBPath()),"","",-1);

	return m_pConZdjCrude;
}

const _ConnectionPtr CPHSApp::GetWorkPrjConnection()
{
	if(m_pConWorkPrj==NULL)
		m_pConWorkPrj.CreateInstance(__uuidof(Connection));
	if(m_pConWorkPrj->State == adStateClosed)
	{			
		m_pConWorkPrj->Open(_bstr_t(dbConnectionString + GetWorkPrjDBPath()),"","",-1);
	}

	return m_pConWorkPrj;
}

CPHSApp* GetPHSApp()
{
	return &theAutoPhsApp;
}

long CPHSApp::GetCurProjectVolumeID()
{
	return EDIBgbl::SelVlmID;
}

void SetCurProjectVolumeID(long iVlmID)
{
	EDIBgbl::SelVlmID = iVlmID; 
}

void CPHSApp::InitApplication()
{
	//�Ժ�����
}


void CPHSApp::SaveApplication()
{
	//�Ժ�����
}

CString CPHSApp::GetWorkPrjDBPath()
{
	return basDirectory::ProjectDir + _T("WorkPrj.mdb");
}

CString CPHSApp::GetZdjcrudeDBPath()
{
	return basDirectory::ProjectDBDir + _T("zdjcrude.mdb");
}

CString CPHSApp::GetSortDBPath()
{
	return basDirectory::ProjectDBDir + _T("sort.mdb");
}

CString CPHSApp::GetAllPrjDBPath()
{
	return basDirectory::DBShareDir + _T("AllPrjDB.mdb");//20071109 "ProjectDBDir" ��Ϊ "DBShareDir"
}

INT CPHSApp::GetCurZdjh(){
	return modPHScal::zdjh;
}

void CPHSApp::SetCurZdjh(INT nZdjh){
	modPHScal::zdjh = nZdjh;
}


CString CPHSApp::GetCurPATable(){
	return modPHScal::tbnPA;
}

CString CPHSApp::GetCurPAFixTable(){
	return modPHScal::tbnPAfix;
}

CString CPHSApp::GetCurSATable(){
	return modPHScal::tbnSA;
}

CString CPHSApp::GetCurSABoltNutTable(){
	return modPHScal::tbnSABoltsNuts;
}

CString CPHSApp::GetCurBoltNutTable(){
	return modPHScal::tbnBoltsNuts;
}

CString CPHSApp::GetCurRodTable(){
	return modPHScal::tbnPART;
}

CString CPHSApp::GetCurConnectTable(){
	return modPHScal::tbnPART;
}

CString CPHSApp::GetCurProfiledBarTable(){
	return modPHScal::tbnSectionSteel;
}

CString CPHSApp::GetCurSpringTable(){
	return modPHScal::tbnSPRINGCrude;
}

CString CPHSApp::GetCurConstSpringTable(){
	return modPHScal::tbnHDCrude;
}

CString CPHSApp::GetCurAttachTable(){
	return modPHScal::tbnAttachment;
}

CString CPHSApp::GetConstSpringRodDiameterTable(){
	return modPHScal::tbnLugDiaOfCSPR;
}

void CPHSApp::SetCurStringNum(LONG nNum)
{
	m_nCurStringNum = nNum;
}

LONG CPHSApp::GetCurStringNum()
{
	return m_nCurStringNum;
}

void CPHSApp::CloseConnection()
{
	if(m_pConAllPrj != NULL && m_pConAllPrj->GetState() == adStateOpen)
	{
		m_pConAllPrj->Close();
		m_pConAllPrj = NULL;
	}

	if(m_pConWorkPrj != NULL && m_pConWorkPrj->GetState() == adStateOpen)
	{
		m_pConWorkPrj->Close();
		m_pConWorkPrj = NULL;
	}

	if(m_pConSort != NULL && m_pConSort->GetState() == adStateOpen)
	{
		m_pConSort->Close();
		m_pConSort = NULL;
	}

	if(m_pConZdjCrude != NULL && m_pConZdjCrude->GetState() == adStateOpen)
	{
		m_pConZdjCrude->Close();
		m_pConZdjCrude = NULL;
	}
}

BOOL IsNumber(CString& str)
{
	TCHAR c;
	for(int i = 0; i < str.GetLength(); i++)
	{
		c = str.GetAt(i);
		if((c >= '0' && c <= '9') || c == '.' || c == '-')
		{
			if(i == 0 && c == '.')
			{
				return FALSE;
			}
			else if(str.GetLength() == 1 && c == '-')
			{
				return FALSE;
			}
			else if(str.GetLength() > 1 && i != 0 && c == '-')
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	if(str.IsEmpty())
	{
		str = _T("0");
	}
	return TRUE;
}


//�Ƿ�Ϊ��װ��ĵ�һ�γ�������
BOOL CPHSApp::IsInstalledAndFirstRun()
{
	return modPHScal::bInstalledAndFirstRun;
}

void CPHSApp::SetInstalledAndFirstRun(BOOL bFirstRun)
{
	modPHScal::bInstalledAndFirstRun = bFirstRun;
}

//�����Ƿ����������
bool CPHSApp::IsStartedOnceAPHS()
{
	return modPHScal::gbStartedOnceAPHS;
}

void CPHSApp::SetStartedOnceAPHS(bool bStarted)
{
	modPHScal::gbStartedOnceAPHS = bStarted;
}


//�ܲ�ѡ��淶
CString CPHSApp::GetPhsPASel()
{
	return modPHScal::gsPhsPASel;
}

void CPHSApp::SetPhsPASel(CString strSel)
{
	modPHScal::gsPhsPASel = strSel;
}

//����ѡ��淶
CString CPHSApp::GetPhsSASel()
{
	return modPHScal::gsPhsSASel;
}

void CPHSApp::SetPhsSASel(CString strSel)
{
	modPHScal::gsPhsSASel = strSel;
}

//���Ӽ�ѡ��淶
CString CPHSApp::GetPhsPARTSel()
{
	return modPHScal::gsPhsPARTSel;
}

void CPHSApp::SetPhsPARTSel(CString strSel)
{
	modPHScal::gsPhsPARTSel = strSel;
}

//����ѡ��淶
CString CPHSApp::GetPhsSPRINGSel()
{
	return modPHScal::gsPhsSPRINGSel;
}

void CPHSApp::SetPhsSPRINGSel(CString strSel)
{
	modPHScal::gsPhsSPRINGSel = strSel;
}

//��������ѡ��淶
CString CPHSApp::GetPhsConstSpringSel()
{
	return modPHScal::gsPhsConstantSpringSel;
}

void CPHSApp::SetPhsConstSpringSel(CString strSel)
{
	modPHScal::gsPhsConstantSpringSel = strSel;
}

//��˨��ĸѡ��淶
CString CPHSApp::GetPhsBoltsNutsSel()
{
	return modPHScal::gsPhsBoltsNutsSel;
}
void CPHSApp::SetPhsBoltsNutsSel(CString strSel)
{
	modPHScal::gsPhsSPRINGSel == strSel;
}


void CPHSApp::GetRecentWorkSPEC()
{
	//��ȡ�������֧���ܹ淶
	CString s=_T("");
	SetPhsPASel( GetRegKey(_T("RecentWork"), GetRegAppPreName() + _T("_phsPASel"),s) );
	SetPhsSASel( Trim(GetRegKey(_T("RecentWork"), GetRegAppPreName() + _T("_phsSASel"), s)) );
	SetPhsPARTSel( Trim(GetRegKey(_T("RecentWork"), GetRegAppPreName() + _T("_phsPARTSel"), s)) );
	SetPhsSPRINGSel( Trim(GetRegKey(_T("RecentWork"), GetRegAppPreName() + _T("_phsSpringSel"), s)) );
	SetPhsConstSpringSel( Trim(GetRegKey(_T("RecentWork"), GetRegAppPreName() + _T("_phsConstantSpringSel"), s)) );
	SetPhsBoltsNutsSel( Trim(GetRegKey(_T("RecentWork"), GetRegAppPreName() + _T("_phsBoltsNutsSel"), s)) );
}

void CPHSApp::SaveRecentWorkSPEC()
{
	//�����������֧���ܹ淶
	::SetRegValue(_T("RecentWork"), GetRegAppPreName() + _T("_phsPASel"), GetPhsPASel());
	::SetRegValue(_T("RecentWork"), GetRegAppPreName() + _T("_phsPARTSel"), GetPhsPARTSel());
	::SetRegValue(_T("RecentWork"), GetRegAppPreName() + _T("_phsSASel"), GetPhsSASel());
	::SetRegValue(_T("RecentWork"), GetRegAppPreName() + _T("_phsSpringSel"), GetPhsSPRINGSel());
	::SetRegValue(_T("RecentWork"), GetRegAppPreName() + _T("_phsConstantSpringSel"), GetPhsConstSpringSel());
	::SetRegValue(_T("RecentWork"), GetRegAppPreName() + _T("_phsBoltsNutsSel"), GetPhsBoltsNutsSel());
	
}


CString CPHSApp::GetRegAppPreName()
{
	return user::gstrAppName;
}

void CPHSApp::SetRegPreAppName(CString strAppName)
{
	user::gstrAppName = strAppName;
}
