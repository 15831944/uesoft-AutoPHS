// PCLASel.h: interface for the CPCLASel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCLASEL_H__43D4256E_5AD7_4FED_83DE_37C5B87AAA9D__INCLUDED_)
#define AFX_PCLASEL_H__43D4256E_5AD7_4FED_83DE_37C5B87AAA9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CompSel.h"
#include "SelParameter.h"

class PipePara;
class PCLASel : public CompSel  
{
public:
	PCLASel();
	virtual ~PCLASel();
public:
	virtual void GetSearchComp(SupportCompData *pObj);

protected:
	PipePara* m_pPipePara; //�ܵ����Σ����¶ȣ��ܾ�
	double m_dbtmpSelPJG; //ѡ��ʱ�õĺ���ֵ
	int m_iNumPart; //����������
	double m_dbDWdelta;//���ҹܾ���Χ����,0~7(%)
	CString m_strCustomID; //��ӦPtype(i)
};

#endif // !defined(AFX_PCLASEL_H__43D4256E_5AD7_4FED_83DE_37C5B87AAA9D__INCLUDED_)
