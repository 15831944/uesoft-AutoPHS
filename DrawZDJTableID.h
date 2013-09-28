// DrawZDJTableID.h: interface for the CDrawZDJTableID class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWZDJTABLEID_H__F65C49B4_8504_43D1_900A_9F9453921935__INCLUDED_)
#define AFX_DRAWZDJTABLEID_H__F65C49B4_8504_43D1_900A_9F9453921935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrawZDJTableID  
{
public:
	void DrawzdjTable(int index,bool bIsCom);
	CDrawZDJTableID();
	virtual ~CDrawZDJTableID();
	enum mnuZDJ_DRAWindex
	{
	   iZDJDRAWTag=0,
	   iZDJDRAWphs,
	   iZDJDRAWbom,
	   iZDJDRAWload,
	   iZDJDRAW180mm,
	   iZDJDRAW270mm,
	   iZDJDRAW705mm ,   //֧������ϸһ����
	   iZDJDRAW270pc,    //֧������ϸһ����(ʯ��)
	   iZDJDRAW270L,//֧������ϸһ����(�Ṥ)
	   iZDJDRAWEXECL,//
	   iZDJDRAWCatalogue,//Ŀ¼
	   iZDJDRAWm_phs, //��ʼ�ֹ��޸ģ�ʵ�����л���m_phs��
	   iZDJDRAWCatalogueExcel,
	   iZDJDRAWXN180mm,    //����Ժ֧������ϸһ����(180mm,A4)
	   iZDJDRAWbomBJBW,//�������������ϸ��
	   iZDJDRAWloadBJBW,   //��������֧������λ�Ʊ�(202mm)
	   iZDJDRAW705mmHSY, //����Ժ֧������ϸһ����
	   iZDJDRAW705mmQGY,	 //�๤Ժ֧������ϸһ����
	   iZDJDRAW705mmHBYZ,   //����ұ��
	   iZDJDRAWBJHY01,//���������ʽ<һ>
	   iZDJDRAWBJHY02 //���������ʽ<��>
	};
};

#endif // !defined(AFX_DRAWZDJTABLEID_H__F65C49B4_8504_43D1_900A_9F9453921935__INCLUDED_)
