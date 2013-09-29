// InputSeleParaSA.h: interface for the InputSeleParaSA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTSELEPARASA_H__E713D7FC_9671_4A69_90CF_C4CEF2AC850C__INCLUDED_)
#define AFX_INPUTSELEPARASA_H__E713D7FC_9671_4A69_90CF_C4CEF2AC850C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AnalyseHangPara  
{
public:

//����bool Cphs::GetphsBHandSizes(CFrmStatus& frmStatus,int  FirstCal, 
//int  MaxCalCount,int nth,float& TmpPaz,float& TmpPgz)�ڵľֲ�����.
 	CString m_strSelNumCS;//ѡ��������۸������Ĺ����ַ���
	CString *m_Ptype;   //��������������ṹCustomID����
	float m_mvBWidth;     //�̶�֧�ܸ������۸ּ��,cm,G47=������chdist�ֶ�ֵ��G48=TZA��A01�ֶ�ֵ
	float m_tmpSelPJG;//�ۼ����������ļ������
	CString m_tmpSQL ;          //��ǰ������ƥ���ѯ�ֶ���Ϣ
	
	CString m_tbn1;
	CString m_SAfDPmax;

	//��Cphs.cpp�����ݳ�Ա(start)
	int m_iNumPart;//iNumPart 
	//��Cphs.cpp�����ݳ�Ա(end)
public:
	CString GetstrSelNumCS();
	CString *GetPtype();
	float GetmvBWidth();
	float GettmpSelPJG();
	CString GettmpSQL();

	CString Gettbn1();
	CString GetSAfDPmax();
	int GetiNumPart();//iNumPart 
	
	void SetstrSelNumCS(const CString &strSelNumCS);
	void SetPtype(CString* Ptype);
	void SetmvBWidth(float mvBWidth);
	
	void SettmpSelPJG(float tmpSelPJG);
	void SettmpSQL(const CString &tmpSQL);
	void Settbn1(const CString &tbn1);
	void SetSAfDPmax(const CString &SAfDPmax);
	void SetiNumPart(int iNumPart);//iNumPart 

public:
	AnalyseHangPara();
	virtual ~AnalyseHangPara();

};

#endif // !defined(AFX_INPUTSELEPARASA_H__E713D7FC_9671_4A69_90CF_C4CEF2AC850C__INCLUDED_)
