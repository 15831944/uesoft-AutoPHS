// StressOfMaterialUser.cpp: implementation of the StressOfMaterialUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StressOfMaterialUser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StressOfMaterialUser::StressOfMaterialUser()
{

}

StressOfMaterialUser::~StressOfMaterialUser()
{

}
void StressOfMaterial(CString Material,float temperature,float& Sigmat,int iMODE,float& Density)
{
//	CString SQLx,sTmp,strTbn,strFD;
//	COleVariant vTmp;
//	float t1,t2,Sigma1,Sigma2;
//	CDaoRecordset rsTmp;
//	Sigmat=0.0;
//	Density=0.0;
//	float Coef;
//	if(iMODE==0)
//	{
//		//���Ҳ�����ĳ�¶�temperature�µ�����Ӧ��sigma,����ֵ��λunit:kgf/cm2,
//		//���ݱ��е�λMPa(N/mm2)
//		//��������Ӧ��Sigmat,MPa->kgf/cm2
//		if(modPHScal::tbnAllowStress==_T("")) modPHScal::tbnAllowStress=_T("MechanicalOfMaterialSIGMAt");
//		strTbn=modPHScal::tbnAllowStress;
//		strFD=_T("SIGMA");
//		Coef=100.0/EDIBgbl::kgf2N;	
//	}
//	else if(iMODE==1)
//	{
//		//���ҵ���ģ��Et,kN/mm,
//		strTbn=_T("MechanicalOfMaterialEt");
//		strFD=_T("Et");
//		Coef=1.0;	
//	}
//	else if(iMODE==2)
//	{
//		//������������ϵ��Alphat,unit:10e-6/C
//		strTbn=_T("MechanicalOfMaterialALPHAt");
//		strFD=_T("ALPHAt");
//		Coef=1.0;	
//	}
//	else return;
//
//	try
//	{
//		rsTmp.m_pDatabase=&EDIBgbl::dbMaterial;//20071102 "dbSORT" ��Ϊ "dbMaterial"
//		rsTmp.Open(dbOpenSnapshot,_T("SELECT * FROM [") + strTbn + _T("] WHERE Trim(material)=\'") + Trim(Material) + _T("\' ORDER BY t"));
//		if(rsTmp.IsBOF() && rsTmp.IsEOF())
//			;
//		else
//		{
//			//���Ҽ����¶�t0ʱ������Ӧ��ֵSigma1
//			SQLx.Format(_T("t>=%g"),temperature);
//			if(rsTmp.FindFirst(SQLx))
//			{
//				rsTmp.GetFieldValue(strFD,vTmp);
//				Sigma1 = vtof(vTmp);
//				rsTmp.GetFieldValue(_T("t"),vTmp);
//				t1 = vtof(vTmp);
//				if(iMODE==1)
//				{
//					rsTmp.GetFieldValue(_T("Density"),vTmp);
//					Density=vtof(vTmp);
//				}
//				//�ҵ��¶�ֵ,ǰ��һ����¼�����¶�ֵ�϶��ȼ����¶ȵ͡�
//				rsTmp.MovePrev();
//				if(rsTmp.IsBOF())
//				{
//					if(t1!=temperature)
//					{
//						//�����ܳ����������
//						//�ڲ�������Ӧ����%s��û�ҵ�>=%sC���¶�ֵ��
//						sTmp.Format(GetResStr(IDS_NotFoundTemperatureValueInTableAllowStress),_T("\%d"),modPHScal::tbnAllowStress,temperature);
//						throw sTmp;
//					}
//					else
//					{
//						Sigmat = Sigma1 *Coef;
//					}
//				}
//				else
//				{
//					rsTmp.GetFieldValue(strFD,vTmp);
//					Sigma2 = vtof(vTmp);
//					rsTmp.GetFieldValue(_T("t"),vTmp);
//					t2 = vtof(vTmp);
//					Sigmat = Sigma2 + (temperature - t2) * (Sigma1 - Sigma2) / (t1 - t2);
//					Sigmat = Sigmat *Coef;
//					if(iMODE==1)
//					{
//						rsTmp.GetFieldValue(_T("Density"),vTmp);
//						Density=vtof(vTmp);
//					}
//				}
//			}
//			else
//			{
//				//�ڲ�������Ӧ����%s��û�ҵ�>=%sC���¶�ֵ��
//				//sTmp.Format(GetResStr(IDS_NotFoundTemperatureValueInTableAllowStress),_T("\%d"),modPHScal::tbnAllowStress,temperature);
//				//throw sTmp;
//				//ȡ����¶��µ�ֵ��Ϊ��ѯֵ
//				rsTmp.MoveLast();
//				rsTmp.GetFieldValue(strFD,vTmp);
//				Sigmat = vtof(vTmp) *Coef;
//				if(iMODE==1)
//				{
//					rsTmp.GetFieldValue(_T("Density"),vTmp);
//					Density=vtof(vTmp);
//				}
//			}
//		}
//	}
//	catch(CDaoException * e)
//	{
//		e->ReportError();
//		e->Delete();
//	}
//	catch(CException *e)
//	{
//		e->Delete();
//	}
//	catch(...)
//	{
//	}
}
