// InputSeleParaSA.cpp: implementation of the InputSeleParaSA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\INC\AnalyseHangPara.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AnalyseHangPara::AnalyseHangPara()
{
 	m_strSelNumCS=_T("");//ѡ��������۸������Ĺ����ַ���
	m_Ptype=NULL;   //��������������ṹCustomID����
	m_mvBWidth=0.0;     //�̶�֧�ܸ������۸ּ��,cm,G47=������chdist�ֶ�ֵ��G48=TZA��A01�ֶ�ֵ
	m_tmpSelPJG=0.0;//�ۼ����������ļ������
	m_tmpSQL=_T("");          //��ǰ������ƥ���ѯ�ֶ���Ϣ
	m_tbn1=_T("");
	m_SAfDPmax=_T("");

	//��Cphs.cpp�����ݳ�Ա(start)
	m_iNumPart=0;//iNumPart 
	//��Cphs.cpp�����ݳ�Ա(end)
}

AnalyseHangPara::~AnalyseHangPara()
{

}

CString AnalyseHangPara::GetstrSelNumCS()
{
	return m_strSelNumCS;
}

CString *AnalyseHangPara::GetPtype()
{
	return m_Ptype;
}

float AnalyseHangPara::GetmvBWidth()
{
	return m_mvBWidth;
}

float AnalyseHangPara::GettmpSelPJG()
{
	return m_tmpSelPJG;
}

CString AnalyseHangPara::GettmpSQL()
{
	return m_tmpSQL;
}

CString AnalyseHangPara::Gettbn1()
{
	return m_tbn1;
}

CString AnalyseHangPara::GetSAfDPmax()
{
	return m_SAfDPmax;
}

int AnalyseHangPara::GetiNumPart()
{
	return m_iNumPart;
}

void AnalyseHangPara::SetstrSelNumCS(const CString &strSelNumCS)
{
	m_strSelNumCS=strSelNumCS;
}

void AnalyseHangPara::SetPtype(CString* Ptype)
{
	m_Ptype=Ptype;
}

void AnalyseHangPara::SetmvBWidth(float mvBWidth)
{
	m_mvBWidth=mvBWidth;
}

void AnalyseHangPara::SettmpSelPJG(float tmpSelPJG)
{
	m_tmpSelPJG=tmpSelPJG;
}

void AnalyseHangPara::SettmpSQL(const CString &tmpSQL)
{
	m_tmpSQL=tmpSQL;
}

void AnalyseHangPara::Settbn1(const CString &tbn1)
{
	m_tbn1=tbn1;
}

void AnalyseHangPara::SetSAfDPmax(const CString &SAfDPmax)
{
	m_SAfDPmax=SAfDPmax;
}

void AnalyseHangPara::SetiNumPart(int iNumPart)
{
	m_iNumPart=iNumPart;
}
