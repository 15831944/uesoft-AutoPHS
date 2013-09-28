// Connect.cpp: implementation of the Connect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Connect.h"
#include "EDIBgbl.h"
#include "user.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString Connect::m_strTableName = _T("connect");
CString Connect::m_strCNTBField = _T("CNTB"); //CNTBͷ�������ֶ���
CString Connect::m_strCNTEField = _T("CNTE"); //CNTEβ�������ֶ���
CString Connect::m_strCNTBextlenFLDField = _T("CNTBextlenFLD");//ͷ����������һ�����ĳ���
Connect::Connect()
{
	m_pRs.m_pDatabase = &EDIBgbl::dbPRJ;
}

Connect::~Connect()
{

}

//�õ�ͷ����������һ�����ĳ���
double Connect::GetCNTBextlenFLD() 
{
	return vtof( m_pRs.GetFieldValue(m_strCNTBextlenFLDField) );
}
