// SAiG100Sel.cpp: implementation of the SAiG100Sel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SAiG100Sel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SAiG100Sel::SAiG100Sel()
{

}

SAiG100Sel::~SAiG100Sel()
{

}

//iG100  , �û��Զ������
//Cphs.cpp(4394,4403)
bool SAiG100Sel::SelSAiG100(CString &Ptype)
{
	bool flag=0;
	try
	{
		//�û��Զ���������û�����Ĺ���ѡ��
		modPHScal::giSAG100No=_ttoi(FrmTxsr.m_pViewTxsr->m_PagOption->m_csSAG100No);
		if(modPHScal::giSAG100No<=0)
		{
			modPHScal::giSAG100No=1;
		}
		SQLx.Format(_T("SELECT * FROM %s WHERE trim(CustomID)=\'%s\' AND dh=%d"),
			modPHScal::tbnSA,
			Ptype[i],
			modPHScal::giSAG100No);
	}
	catch(_com_error e)
	{
	}
	return flag;
}
