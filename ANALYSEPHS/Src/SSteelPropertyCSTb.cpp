// SSteelPropertyCSTb.cpp: implementation of the SSteelPropertyCSTb class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SSteelPropertyCSTb.h"
#include "PHSUser.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SSteelPropertyCSTb::SSteelPropertyCSTb()
{
	J1y = 0.0;
	J1z = 0.0;
	W1y = 0.0;
	W1z = 0.0;
	F1 = 0.0;
	h1 = 0.0;
	b1 = 0.0;
	J2y = 0.0;
	J2z = 0.0;
	W2y = 0.0;
	W2z = 0.0;
	F2 = 0.0;
	h2 = 0.0;
	b2 = 0.0;
}

SSteelPropertyCSTb::~SSteelPropertyCSTb()
{
	
}

BOOL SSteelPropertyCSTb::GetData(  _RecordsetPtr pRs  )
{
	if( pRs == NULL )
		return FALSE;
				
	//_variant_t vTmp;
	COleVariant vTmp;
	
	CString strP1;
	CString strP2;
	if(!pRs->FindFirst(_T("BH=\'")+strP1+_T("\'")))//Cg?
	{
		//�ڲ۸����Ա�%s��û�ҵ��۸��ͺ�%s
		return FALSE;
	}
	//����1�۸ֹ��Ծ�Ix,cm4->mm4
	pRs->GetFieldValue(_T("Ix"),vTmp);
	J1y=vtof(vTmp)*10000.0;
	//����1�۸ֹ��Ծ�Iy,cm4->mm4
	pRs->GetFieldValue(_T("Iy"),vTmp);
	J1z=vtof(vTmp)*10000.0;
	//����1�۸ֿ����Wx,cm3->mm3
	pRs->GetFieldValue(_T("Wx"),vTmp);
	W1y=vtof(vTmp)*1000.0;
	//����1�۸ֿ����Wy,cm3->mm3
	pRs->GetFieldValue(_T("Wy"),vTmp);
	W1z=vtof(vTmp)*1000.0;
	//����1�۸ֽ����S,cm2->mm2
	pRs->GetFieldValue(_T("S"),vTmp);
	F1=vtof(vTmp)*100.0;
	//����1�۸ָ߶�h,mm
	pRs->GetFieldValue(_T("h"),vTmp);
	h1=vtof(vTmp);
	//����1�۸��ȿ�b,mm
	pRs->GetFieldValue(_T("b"),vTmp);
	b1=vtof(vTmp);
	
	if(!pRs->FindFirst(_T("BH=\'")+strP2+_T("\'")))
	{
		//�ڲ۸����Ա�%s��û�ҵ��۸��ͺ�%s
		return FALSE;
	}
	//����2�۸ֹ��Ծ�Ix,cm4->mm4
	pRs->GetFieldValue(_T("Ix"),vTmp);
	J2y=vtof(vTmp)*10000.0;
	//����2�۸ֹ��Ծ�Iy,cm4->mm4
	pRs->GetFieldValue(_T("Iy"),vTmp);
	J2z=vtof(vTmp)*10000.0;
	//����2�۸ֿ����Wx,cm3->mm3
	pRs->GetFieldValue(_T("Wx"),vTmp);
	W2y=vtof(vTmp)*1000.0;
	//����2�۸ֿ����Wy,cm3->mm3
	pRs->GetFieldValue(_T("Wy"),vTmp);
	W2z=vtof(vTmp)*1000.0;
	//����2�۸ֽ����S,cm2->mm2
	pRs->GetFieldValue(_T("S"),vTmp);
	F2=vtof(vTmp)*100.0;
	//����2�۸ָ߶�h,mm
	pRs->GetFieldValue(_T("h"),vTmp);
	h2=vtof(vTmp);
	//����2�۸��ȿ�b,mm
	pRs->GetFieldValue(_T("b"),vTmp);
	b2=vtof(vTmp);
	
	return TRUE;
}