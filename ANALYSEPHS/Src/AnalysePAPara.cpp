// AnalysePAPara.cpp: implementation of the AnalysePAPara class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnalysePAPara.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AnalysePAPara::AnalysePAPara()
{
	m_PAfixH=0.0;//modPHScal::PAfixH�̶�֧�ܹܲ��߶�H,cm
	m_gnDW_delta=0.0;//modPHScal::gnDW_delta���ҹܾ���Χ����,0~7(%)
	m_dj=0.0;//modPHScal::dj���⾶
	m_tbnPA=_T("");//modPHScal::tbnPA�ܲ�������
	m_gbPAForceZero=0;//modPHScal::gbPAForceZero���ܲ������ú���Ϊ��ʱ�����Ǻ���.
}

AnalysePAPara::~AnalysePAPara()
{

}

float AnalysePAPara::GetPAfixH()
{
	return m_PAfixH;
}

float AnalysePAPara::Getdj()
{
	return m_dj;
}

CString AnalysePAPara::GettbnPA()
{
	return m_tbnPA;
}

bool AnalysePAPara::GetgbPAForceZero()
{
	return m_gbPAForceZero;
}

void AnalysePAPara::SetgbPAForceZero(float gbPAForceZero)
{
	m_gbPAForceZero=gbPAForceZero;	
}

void AnalysePAPara::SettbnPA(CString &tbnPA)
{
	m_tbnPA=tbnPA;
}

void AnalysePAPara::Setdj(float dj)
{
	m_dj=dj;
}

void AnalysePAPara::SetgnDW_delta(float gnDW_delta)
{
	m_gnDW_delta=gnDW_delta;
}

void AnalysePAPara::SetPAfixH(float PAfixH)
{
	m_PAfixH=PAfixH;
}

float AnalysePAPara::GetgnDW_delta()
{
	return m_gnDW_delta;
}
