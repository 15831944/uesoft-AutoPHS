// LugDiaOfCSPR.cpp: implementation of the LugDiaOfCSPR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LugDiaOfCSPR.h"
#include "user.h"
#include "modPHScal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString LugDiaOfCSPR::m_strPmaxKgfField = _T("PmaxKgf");
CString LugDiaOfCSPR::m_strFiJField = _T("FiJ");
CString LugDiaOfCSPR::m_strCustomIDField = _T("CustomID"); //����������ʽ�ֶ���
CString LugDiaOfCSPR::m_strminDHField = _T("minDH"); //����������С�����ֶ���
CString LugDiaOfCSPR::m_strmaxDHField = _T("maxDH"); //���������������ֶ���
CString LugDiaOfCSPR::m_strPmaxField = _T("Pmax"); //�������ɹ�����(N)�ֶ���
CString LugDiaOfCSPR::m_strFiKField = _T("FiK"); //�������ɵĵ����ֱ��
CString LugDiaOfCSPR::m_strTField = _T("T");

LugDiaOfCSPR::LugDiaOfCSPR()
{
	m_pRs.m_pDatabase = &modPHScal::dbZDJcrude;
}

LugDiaOfCSPR::~LugDiaOfCSPR()
{

}

//���Ժ��������ɽ���ĸֱ��
int LugDiaOfCSPR::GetFiJ() 
{
	return vtoi(m_pRs.GetFieldValue(m_strFiJField));
}

//�������ɵĵ����ֱ��
int LugDiaOfCSPR::GetFiK() 
{
	return vtoi(m_pRs.GetFieldValue(m_strFiKField));
}

double LugDiaOfCSPR::GetT()
{
	return vtof(m_pRs.GetFieldValue(m_strTField));
}



