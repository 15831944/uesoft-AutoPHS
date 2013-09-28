// ResultZB.cpp: implementation of the ResultZB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultZB.h"
#include "EDIBgbl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString ResultZB::m_strCustomIDField = _T("VolumeID"); //�ֶ���
CString ResultZB::m_strZdjhField = _T("ZDJH"); //
CString ResultZB::m_strNthField = _T("nth");
CString ResultZB::m_strRecnoField = _T("recno");
CString ResultZB::m_strPreviousPartInsertLenField = _T("PreviousPartInsertLen");
CString ResultZB::m_strBHformatField = _T("BHformat");
CString ResultZB::m_strCLggField = _T("CLgg");
CString ResultZB::m_strVolumeIDField = _T("VolumeID"); //���������
CString ResultZB::m_strClassIDField = _T("ClassID"); //��������
CString ResultZB::m_strPjsField = _T("Pjs"); //ѡ�񲿼��ļ������
CString ResultZB::m_strFxField = _T("fx"); //����ͼ�ο���ת����
CString ResultZB::m_strRotationField = _T("Rotation"); //����ͼ�ο���ת�Ƕ�
CString ResultZB::m_strSizeDimField = _T("SizeDim"); //�����ߴ��ע����
CString ResultZB::m_strSizeHField = _T("SizeH"); //������ʵ�ʸ߶Ȼ򳤶�
CString ResultZB::m_strCLnumField = _T("CLnum"); //��������
CString ResultZB::m_strSEQField = _T("SEQ"); //�������
CString ResultZB::m_strCLmcField = _T("CLmc"); //��������
CString ResultZB::m_strWeightField = _T("Weight"); //��������
CString ResultZB::m_strCldzField = _T("CLdz"); //��������
CString ResultZB::m_strClzzField = _T("CLzz"); //��������
CString ResultZB::m_strIndexField = _T("Index"); //�������������
CString ResultZB::m_strIDField = _T("ID"); //�����ڲ���־
CString ResultZB::m_strCLclField = _T("CLcl"); //������������
CString ResultZB::m_strTField = _T("T");
CString ResultZB::m_strAllowedMinLugExtField = _T("AllowedMinLugExt"); //���Ӽ����˵���С������볤��
CString ResultZB::m_strAllowedMaxLugExtField = _T("AllowedMaxLugExt"); //���Ӽ����˵���������볤��
CString ResultZB::m_strClbzField = _T("CLbz"); //������ע
CString ResultZB::m_strGDW1 = _T("GDW1"); //������λֵ
CString ResultZB::m_strSpringNo = _T("SpringNo"); //���ɺ�
CString ResultZB::m_strLugDia = _T("LugDia"); //����ֱ��
CString ResultZB::m_strHeatDisp = _T("HeatDisp"); //���ɻ���������������λ��
ResultZB::ResultZB()
{
	m_pRs.m_pDatabase=&EDIBgbl::dbPRJDB;
}

ResultZB::~ResultZB()
{
	
}

void ResultZB::SetNth(int iNth)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strNthField, COleVariant((short)iNth) );
}

void ResultZB::SetPreviousPartInsertLen(double dbValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strPreviousPartInsertLenField, COleVariant(dbValue) );
}

void ResultZB::SetBHformat( const CString& strValue )
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strBHformatField, COleVariant(strValue) );
}

void ResultZB::SetCLgg( const CString& strValue )
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strCLggField, COleVariant(strValue) );
}

void ResultZB::SetRecno(int iValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strRecnoField, COleVariant((long)iValue) );
}
void ResultZB::SetVolumeID(int iValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strVolumeIDField, COleVariant((long)iValue) );
}
void ResultZB::SetZdjh(int iValue) //֧���ܺ�
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strZdjhField, COleVariant((long)iValue) );
}
void ResultZB::SetCustomID(CString strValue)//��������
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strCustomIDField, COleVariant(strValue) );
}
void ResultZB::SetClassID(int iValue) //��������
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strClassIDField, COleVariant((long)iValue) );
}
void ResultZB::SetPjs(double dbValue) //ѡ�񲿼�ʱ���õļ������ֵ�����ֵ�����ۼ���ǰ������� kgf
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strPjsField, COleVariant(dbValue) );
}
void ResultZB::SetFx(int iValue)//�������ͼ��һ·����
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strFxField, COleVariant((long)iValue) );
}
void ResultZB::SetRotation(int iValue)//���y���귽��
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strRotationField, COleVariant((long)iValue) );
}
void ResultZB::SetSizeDim(int iValue)//�õ�������ʵ�ʸ߶Ⱥͳ���
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strSizeDimField, COleVariant((long)iValue) );
}
void ResultZB::SetSizeH(double dbValue)//������ʵ�ʸ߶Ȼ򳤶�
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strSizeHField, COleVariant(dbValue) );
}
void ResultZB::SetCLnum(int iValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strCLnumField, COleVariant((long)iValue) );
}
void ResultZB::SetSEQ(int iValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strSEQField, COleVariant((long)iValue) );
}
void ResultZB::SetCLmc(CString strValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strCLmcField, COleVariant(strValue) );
}
void ResultZB::SetWeight(double dbValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strWeightField, COleVariant(dbValue) );
}
void ResultZB::SetCldz(double dbValue) //��������
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strCldzField, COleVariant(dbValue) );
}
void ResultZB::SetClzz(double dbValue) //��������
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strClzzField, COleVariant(dbValue) );
}
void ResultZB::SetIndex( int iValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strIndexField, COleVariant((long)iValue) );
}
void ResultZB::SetID( CString strValue ) //�����ڲ���־
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strIDField, COleVariant(strValue) );
}
void ResultZB::SetCLcl(CString strValue)//������������
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strCLclField, COleVariant(strValue) );
}
void ResultZB::SetT(int iValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strTField, COleVariant((long)iValue) );
}
void ResultZB::SetAllowedMinLugExt(double dbValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strAllowedMinLugExtField, COleVariant(dbValue) );
}
void ResultZB::SetAllowedMaxLugExt(double dbValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strAllowedMaxLugExtField, COleVariant(dbValue) );
}
void ResultZB::SetClbz(CString strValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strClbzField, COleVariant(strValue) );
}
void ResultZB::SetGDW1(double dbValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strGDW1, COleVariant(dbValue) );
}
void ResultZB::SetSpringNo(int iValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strSpringNo, COleVariant((long)iValue) );
}
void ResultZB::SetLugDia(int iValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strLugDia, COleVariant((long)iValue) );
}
void ResultZB::SetHeatDisp(double dbValue)
{
	m_pRs.Edit();
	m_pRs.SetFieldValue( m_strHeatDisp, COleVariant(dbValue) );
}