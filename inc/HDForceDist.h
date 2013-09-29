// HDForceDist.h: interface for the HDForceDist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDFORCEDIST_H__18A134E0_04C9_4945_A546_3E11BD9C2E5F__INCLUDED_)
#define AFX_HDFORCEDIST_H__18A134E0_04C9_4945_A546_3E11BD9C2E5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ConstantSpringDataBase.h"

class HDForceDist : public ConstantSpringDataBase 
{
public:
	HDForceDist();
	virtual ~HDForceDist();
public:
	int GetDH(); //���ɱ��
	double GetMinCapacity(); //�õ���С������ֵ
	double GetMaxCapacity(); //�õ���������ֵ
	double GetMinDist(); //��ǰ��¼����Сλ��
	double GetMaxDist(); //��ǰ��¼�����λ��
	double GetCapacity(); //�õ�����ֵ
public:
	const CString& GetCapacityField()
	{
		return m_strCapacityField;
	}
private:
	static CString m_strCapacityField;
	static CString m_strMinDistField;
	static CString m_strMaxDistField;
	static CString m_strDHField;
};

#endif // !defined(AFX_HDFORCEDIST_H__18A134E0_04C9_4945_A546_3E11BD9C2E5F__INCLUDED_)
