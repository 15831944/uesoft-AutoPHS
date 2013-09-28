// ConstantSpring.cpp: implementation of the ConstantSpring class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstantSpring.h"
#include <math.h>
#include "modphscal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ConstantSpring::ConstantSpring()
{

}

ConstantSpring::~ConstantSpring()
{

}

/*
����ԭʼ���صõ�����ѡ�͵ĺ���
*/
double ConstantSpring::GetPGZ(double dbPgz,CString sPartID,int FirstCal)
{
	double TmpPgz = 0;
	if ( sPartID == _T("LHG") && FirstCal >=2 )
	{
		TmpPgz = (fabs(dbPgz)+modPHScal::g_fLHGWeight) / modPHScal::gintParallelNum;
	}
	else
	{
		TmpPgz = fabs(dbPgz) / modPHScal::gintParallelNum;
	}
	return TmpPgz;
}

//����ԭʼλ�Ƶõ�����ѡ�͵�λ��
double ConstantSpring::Getyr(double dbyr)
{
	double Tmpyr;
	Tmpyr = fabs(dbyr);         //yr ��λ:mm
	if( Tmpyr * modPHScal::gUseConstantSpringPercentSurplusDist < modPHScal::gUseConstantSpringMinSurplusDist )
	{
		Tmpyr =Tmpyr + modPHScal::gUseConstantSpringMinSurplusDist;
	}
	else
	{
		Tmpyr = Tmpyr * (1 + modPHScal::gUseConstantSpringPercentSurplusDist);
	}
	return Tmpyr;
}



/*�õ�ѡ�����˵�SQL���
strPrtID:ID��;iSEQofSPR:??;fWeight:��ǰ������������; tmpSelPJG4CSPRFiJ:����ԭʼ���ݼ�����ĺ���
strCustomID:��ǰ�������ɺ�;iFjmin:������˨��ֱ��
*/
CString ConstantSpring::GetSelectLugSql(CString strPrtID,int iSEQofSPR,float tmpSelPJG4CSPRFiJ,float fWeight,CString strCustomID,int iFjmin)
{
	//�����������ɣ�ѭ��2�β����ҳ��������ֱ��
	CString sTmp,SQLx;
	
	if(modPHScal::gbByForce) //�Ƿ��ú���ѡ����
	{
		sTmp.Format(_T(" PmaxKgf>=%g "),
			(strPrtID == _T("LHG") ? tmpSelPJG4CSPRFiJ +fWeight * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
	}
	else
	{
		sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
			modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
			modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
	}
	SQLx.Format( _T("SELECT * FROM [%s] WHERE %s AND FiJ>=%d AND minDH<=%d AND maxDH>=%d  AND trim(CustomID)=\'%s\' %s"),
		modPHScal::tbnLugDiaOfCSPR,					
		sTmp,
		(iFjmin <= 0 ? 10 : iFjmin),
		modPHScal::sSprInfo[iSEQofSPR].DH,
		modPHScal::sSprInfo[iSEQofSPR].DH,
		strCustomID,
		(modPHScal::gbByForce ? _T(" ORDER BY Pmax,FiJ") : _T(" ORDER BY FiJ")));
	
	return SQLx;
}

/*
��һ�μ���ʱ��֮ǰ����������ֱ��Լ��ֵ=��ǰ������ɽ���ĸֱ��
iFjg:��ǰ������ɽ���ĸֱ��;iSEQofSPR:��ǰ���ɵ�������(���һ��:0;�ڶ���:1) iFirstCal:��ǰ����Ĵ���
*/
void ConstantSpring::SetAforeCompDia(int iFiJ,int iSEQofSPR,int iFirstCal)
{
	int k=0;
	if( iSEQofSPR + 1 > modPHScal::giWholeSprNum )
	{
		return ;
	}
	if( iFirstCal > 1 )
	{
		//��һ�μ��㣬֮ǰ����������ֱ��Լ��ֵ=��ǰ������ɽ���ĸֱ��
		//�ڶ��μ��㣬��������ֱ��ѡ�������ɽ���ĸֱ������ʱ����ֱ����Ȼ�����˺���ɽ���ĸ����Сֱ����
		return ;
	}
	if( iSEQofSPR > 0 )
	{
		k = modPHScal::giCSPRindex[iSEQofSPR - 1] + 1;
	}
	for(;k<= modPHScal::giCSPRindex[iSEQofSPR];k++)
	{
		modPHScal::PtypeDiameter[k] = iFiJ;
		if(modPHScal::gbRodDiaByMax) //����ֱ����С����
		{
			if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[k])
				modPHScal::PtypeDiameter[k]=modPHScal::iMaxRodDia;
			else
				modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[k];
		}
	}
}
