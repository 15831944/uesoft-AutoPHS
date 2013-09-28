// SAbeamSel.cpp: implementation of the SAbeamSel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SAbeamSel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SAbeamSel::SAbeamSel()
{

}

SAbeamSel::~SAbeamSel()
{

}

//Cphs.cpp(3500,3534)
//	case iSAbeam: ��֧��
//	case iG52_55: ������˨��֧��
bool SAbeamSel::SelSAbeam(CString &Ptype)
{
	int flag=0;
	try
	{
		//��֧��G31~G39��G52~G55����ʽӦΪORDER BY L1,GDW1,%s����ORDER BY DH
		//������������ʽӦΪORDER BY GDW1,L1,%s����ORDER BY DH
		
		//��2006.06.09�������ָ���ѡ���в����ͺ�ƫ����ǰ��
		//����ѡ������ʽΪORDER BY GDW1,L1,%s
		if( iNumPart == 2 )
		{
			//�����������е����к���
			SQLx.Format(_T("SELECT * FROM %s WHERE GDW1 >= IIF(%g > L1/2 , L1-%g , %g) AND L1>= %g AND %s >=%g AND trim(CustomID)=\'%s\' %s ORDER BY DH"),
				modPHScal::tbnSA,
				modPHScal::gdw+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0)/2,
				modPHScal::gdw+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0) / 2,
				modPHScal::gdw+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0) / 2 ,
				modPHScal::Lspan,
				SAfDPmax,
				tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe"))) ? iNumPart : 1 ),
				Ptype[i],
				strSelNumCS);
		}
		else
		{
			//һ���������е����к���
			//�ٶ�����������һ�㣬����ѡ������ĸ���ƫ�ڰ�ȫ��
			SQLx.Format(_T("SELECT * FROM %s WHERE GDW1>=IIF(%g > L1/2 , L1-%g , %g) AND L1>=%g AND %s>=%g AND trim(CustomID)=\'%s\' %s ORDER BY DH"),
				modPHScal::tbnSA,
				modPHScal::gdw+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0)/2,
				modPHScal::gdw+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0)/2,
				modPHScal::gdw+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0)/2,
				modPHScal::Lspan,
				SAfDPmax,
				tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe"))) ? iNumPart : 1),
				Ptype[i],
				strSelNumCS);
		}
	}
	catch(_com_error e)
	{
	}
	return flag;
}
