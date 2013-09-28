// HDCrude.h: interface for the HDCrude class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDCRUDE_H__4C2BBE74_5DAC_48E5_8FF9_CE07764BC18D__INCLUDED_)
#define AFX_HDCRUDE_H__4C2BBE74_5DAC_48E5_8FF9_CE07764BC18D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ConstantSpringDataBase.h"

class HDCrude : public ConstantSpringDataBase
{
public:
	HDCrude();
	virtual ~HDCrude();
public:
	int GetDH();
	CString GetBH();
	int GetFjmin(); //�õ���С��˨��ֱ��
	double GetSizeC(); //
	double GetWeight(); //�����������
	CString GetCustomID(); //�õ���ʽ
	double GetSizeH(); //
	double GetT();
	double GetPP();
	double GetfBmin();
	double GetfBmax();
public:
	const CString& GetFjminField()
	{
		return m_strFjminField;
	}
	const CString& GetDHField()
	{
		return m_strDHField;
	}
	const CString& GetfBminField()
	{
		return m_strfBminField;
	}
	const CString& GetfBmaxField()
	{
		return m_strfBmaxField;
	}
	const CString& GetCustomIDField()
	{
		return m_strCustomIDField;
	}
	const CString& GetWeightField()
	{
		return m_strWeightField;
	}

private:
	static CString m_strFjminField;
	static CString m_strDHField;
	static CString m_strBHField;
	static CString m_strfBminField;
	static CString m_strfBmaxField;
	static CString m_strCustomIDField;
	static CString m_strWeightField;
	static CString m_strSizeCField;
	static CString m_strSizeHField;
	static CString m_strTField; //T�ֶ���
	static CString m_strPPField; 
};

#endif // !defined(AFX_HDCRUDE_H__4C2BBE74_5DAC_48E5_8FF9_CE07764BC18D__INCLUDED_)
