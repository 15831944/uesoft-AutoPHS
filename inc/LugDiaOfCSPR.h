// LugDiaOfCSPR.h: interface for the LugDiaOfCSPR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LUGDIAOFCSPR_H__EA04D393_C27B_4500_B4D4_E726872AA332__INCLUDED_)
#define AFX_LUGDIAOFCSPR_H__EA04D393_C27B_4500_B4D4_E726872AA332__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ConstantSpringDataBase.h"

class LugDiaOfCSPR : public ConstantSpringDataBase
{
public:
	LugDiaOfCSPR();
	virtual ~LugDiaOfCSPR();
	double GetT();
	int GetFiJ(); //���Ժ��������ɽ���ĸֱ��
	int GetFiK(); //�õ����ɵ����ֱ��
public:
	const CString& GetCustomIDField()
	{
		return m_strCustomIDField;
	}
	const CString& GetPmaxKgfField()
	{
		return m_strPmaxKgfField;
	}
	const CString& GetFiJField() 
	{
		return m_strFiJField;
	}
	const CString& GetminDHField()
	{
		return m_strminDHField;
	}
	const CString& GetmaxDHField()
	{
		return m_strmaxDHField;
	}
	const CString& GetPmaxField()
	{
		return m_strPmaxField;
	}

private:
	static CString m_strCustomIDField; //����������ʽ�ֶ���
	static CString m_strPmaxKgfField; //�������ɹ��������ֶ���
	static CString m_strFiJField; //���������ɽ���ĸֱ���ֶ���
	static CString m_strminDHField; //����������С�����ֶ���
	static CString m_strmaxDHField; //���������������ֶ���
	static CString m_strPmaxField;  //�������ɹ�����(N)�ֶ���
	static CString m_strFiKField; //�������ɵĵ����ֱ��
	static CString m_strTField; 
};

#endif // !defined(AFX_LUGDIAOFCSPR_H__EA04D393_C27B_4500_B4D4_E726872AA332__INCLUDED_)
