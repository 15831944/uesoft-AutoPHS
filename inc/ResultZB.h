// ResultZB.h: interface for the ResultZB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTZB_H__BF11B6F9_49C0_4F6B_94D7_561EFDD7BF64__INCLUDED_)
#define AFX_RESULTZB_H__BF11B6F9_49C0_4F6B_94D7_561EFDD7BF64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ConstantSpringDataBase.h"

class ResultZB : public ConstantSpringDataBase
{
public:
	ResultZB();
	virtual ~ResultZB();
public:
	void SetNth( int iNth );
	void SetPreviousPartInsertLen( double dbValue );
	void SetBHformat( const CString& strValue );
	void SetCLgg( const CString& strValue );
	void SetRecno(int iValue);
	void SetVolumeID(int iValue);
	void SetZdjh(int iValue);
	void SetCustomID(CString strValue);
	void SetClassID(int iValue);
	void SetPjs(double dbValue); //ѡ�񲿼�ʱ���õļ������ֵ�����ֵ�����ۼ���ǰ������� kgf
	void SetFx(int iValue);//�������ͼ��һ·����
	void SetRotation(int iValue);//���y���귽��
	void SetSizeDim(int iValue);//�õ�������ʵ�ʸ߶Ⱥͳ���
	void SetSizeH(double dbValue);//
	void SetCLnum(int iValue);
	void SetSEQ(int iValue);
	void SetCLmc(CString strValue);
	void SetWeight(double dbValue);
	void SetCldz(double dbValue);
	void SetClzz(double dbValue);
	void SetIndex( int iValue);
	void SetID( CString strValue );
	void SetCLcl(CString strValue);
	void SetT(int iValue);
	void SetAllowedMinLugExt(double dbValue);
	void SetAllowedMaxLugExt(double dbValue);
	void SetClbz(CString strValue);
	void SetGDW1(double dbValue);
	void SetSpringNo(int iValue);
	void SetLugDia(int iValue);
	void SetHeatDisp(double dbValue);
public:
	const CString& GetCustomIDField()
	{
		return m_strCustomIDField;
	}
	const CString& GetZdjhField()
	{
		return m_strZdjhField;
	}
	const CString& GetNthField()
	{
		return m_strNthField;
	}
	const CString& GetRecnoField()
	{
		return m_strRecnoField;
	}
private:
	static CString m_strNthField;
	static CString m_strRecnoField;
	static CString m_strPreviousPartInsertLenField; //�ֶ���
	static CString m_strBHformatField;
	static CString m_strCLggField; //���
	static CString m_strVolumeIDField; //���������
	static CString m_strZdjhField; //֧���ܺ�
	static CString m_strCustomIDField;//��������
	static CString m_strClassIDField; //��������
	static CString m_strPjsField; //ѡ�񲿼��ļ������
	static CString m_strFxField; //����ͼ�ο���ת����
	static CString m_strRotationField; //����ͼ�ο���ת�Ƕ�
	static CString m_strSizeDimField; //�����ߴ��ע����
	static CString m_strSizeHField; //������ʵ�ʸ߶Ȼ򳤶�
	static CString m_strCLnumField; //��������
	static CString m_strSEQField; //�������
	static CString m_strCLmcField; //��������
	static CString m_strWeightField; //��������
	static CString m_strCldzField; //��������
	static CString m_strClzzField; //��������
	static CString m_strIndexField; //�������������
	static CString m_strIDField; //�����ڲ���־
	static CString m_strCLclField; //������������
	static CString m_strTField;
	static CString m_strAllowedMinLugExtField; //���Ӽ����˵���С������볤��
	static CString m_strAllowedMaxLugExtField; //���Ӽ����˵���������볤��
	static CString m_strClbzField; //������ע
	static CString m_strGDW1; //������λֵ
	static CString m_strSpringNo; //���ɺ�
	static CString m_strLugDia; //����ֱ��
	static CString m_strHeatDisp; //���ɻ���������������λ��
};

#endif // !defined(AFX_RESULTZB_H__BF11B6F9_49C0_4F6B_94D7_561EFDD7BF64__INCLUDED_)
