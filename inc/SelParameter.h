// SelParameter.h: interface for the SelParameter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELPARAMETER_H__C763413F_1000_48B9_9D0D_3BE7FE437BCB__INCLUDED_)
#define AFX_SELPARAMETER_H__C763413F_1000_48B9_9D0D_3BE7FE437BCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//�ܵ�����
class PipePara
{
public:
	PipePara();
	virtual ~PipePara();
public:
	double GetDj()
	{
		return m_dj;
	}
	double GetT0()
	{
		return m_tmpT0;
	}
private:
	double m_dj; //�ܵ��⾶  ��ӦmodPHScal::dj
	double m_tmpT0; //�¶�   ��ӦmodPHScal::t0
};

/*
��������
*/
class ParaBase
{

};

//ѡ���õĲ���
class IPAHParaC : public ParaBase
{
public:
	IPAHParaC();
public:
	double GetDj() //�⾶
	{
		return m_dj;
	}
	double GetT0() //�⾶
	{
		return m_tmpT0;
	}
	double GetSelPJG()
	{
		return m_dbtmpSelPJG;
	}
	int GetiNumPart()
	{
		return m_iNumPart;
	}
	double GetDWdelta()
	{
		return m_dbDWdelta;
	}
	const CString& GetCustomID()
	{
		return m_strCustomID;
	}
	double SetDj(double dbValue) //�⾶
	{
		m_dj = dbValue;
	}
	double SetT0(double dbValue) //�⾶
	{
		m_tmpT0 = dbValue;
	}
	void SetSelPJG(double dbValue)
	{
		m_dbtmpSelPJG = dbValue;
	}
	void SetNumPart(int iValue)
	{
		m_iNumPart = iValue;
	}
	void SetDWdelta( double dbValue )
	{
		m_dbDWdelta = dbValue;
	}

private:
	double m_dj; //�ܵ��⾶  ��ӦmodPHScal::dj
	double m_tmpT0; //�¶�   ��ӦmodPHScal::t0
	double m_dbtmpSelPJG; //ѡ��ʱ�õĺ���ֵ
	int m_iNumPart; //����������
	double m_dbDWdelta;//���ҹܾ���Χ����,0~7(%)
	CString m_strCustomID; //��ӦPtype(i)
};


#endif // !defined(AFX_SELPARAMETER_H__C763413F_1000_48B9_9D0D_3BE7FE437BCB__INCLUDED_)
