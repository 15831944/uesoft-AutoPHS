// CrudePA.h: interface for the CCrudePA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRUDEPA_H__4878C90A_9809_4E20_9423_18B67908A018__INCLUDED_)
#define AFX_CRUDEPA_H__4878C90A_9809_4E20_9423_18B67908A018__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SupportCompData.h"

class CrudePA : public SupportCompData  
{
public:
	CrudePA(CDaoDatabase &DataMdb);
	virtual ~CrudePA();
public:
	double GetDW();
	CString GetCustomID();	
	double GetPMAX();
	double GetTJ();
	float GetWeight();
public:
	const CString& GetTableName()
	{
		return m_strTableName;
	}
	const CString& GetCustomIDField()
	{
		return m_strCustomIDField;
	}
	const CString& GetDWField()
	{
		return m_strDWField;
	}
	const CString& GetPmaxField()
	{
		return m_strPMAXField;
	}
	const CString& GetTJField()
	{
		return m_strTJField;
	}
	const CString& GetWeightField()
	{
		return m_strWeightField;
	}
private:
	static CString m_strTableName;      //crudePA����
	static CString m_strCustomIDField;  //�ܲ���ʽ
	static CString m_strDWField;        //�ܲ�ֱ��
	static CString m_strPMAXField;      //�ܲ����������
	static CString m_strTJField;        //�ܲ�������¶�
	static CString m_strWeightField;    //�ܲ��������
};

#endif // !defined(AFX_CRUDEPA_H__4878C90A_9809_4E20_9423_18B67908A018__INCLUDED_)
