// BuildSeleSQLPA.cpp: implementation of the BuildSeleSQLPA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuildSeleSQLPA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BuildSeleSQLPA::BuildSeleSQLPA()
{
	m_PAPara=NULL;
	m_Para=NULL;
	m_PHSPara=NULL;
}

BuildSeleSQLPA::~BuildSeleSQLPA()
{

}
//iPAh,Cphs.cpp(2942,2961)
//iPAXZ1_XZ3:
//CString BuildSeleSQLPA::GetiPAhSQL(CString &Ptype)
//{
//	CString strSQL=_T("");
//	CString sTmp=_T("");
//	int k=0;
//	CDaoRecordset rsX;//cg?
//	try
//	{
//		for (;k<= m_PAPara->gnDW_delta;k++)
//		{//���㲿�����ݱ�����һ���뵱ǰ֧���ܹܲ��⾶m_PAPara->dj��ӽ����⾶
//		 //�����⾶���ᳬ��m_PAPara->gnDW_delta/100��
//			sTmp.Format(_T("%g"),m_PAPara->dj*(1-k*0.01));
//			strSQL = _T("SELECT * FROM ") + m_PAPara->tbnPA + _T(" WHERE Dw>= ") + sTmp + _T(" AND Dw<= ") ;
//			sTmp.Format(_T("%g"),m_PAPara->dj * (1 + k * 0.01));
//			strSQL+=sTmp  + _T(" AND Pmax>=");
//			sTmp.Format(_T("%g"),m_Para->tmpSelPJG / m_Para->iNumPart);
//			strSQL+=sTmp  + _T(" AND Tj>=");
//			sTmp.Format(_T("%g"),m_PHSPara->t0);
//			strSQL+= sTmp + _T(" AND trim(CustomID)=\'") + Ptype + _T("\' ORDER BY tj,Dw,Pmax,Weight");
//			if(rsX.IsOpen())
//				rsX.Close();
//			rsX.Open(dbOpenSnapshot,strSQL);
//			if( rsX.IsEOF() && rsX.IsBOF() )
//				;
//			else
//				//û�ҵ���ÿ���⾶����1%��������
//				k=m_PAPara->gnDW_delta+1;		  
//		}
//	}
//	catch(_com_error e)
//	{
//	}
//	return strSQL;
//}
//
////iPAfixZ1 Cphs.cpp(2965,2966)�ܼй̶�֧��
////iPAfixZ2://���ӹ̶�֧��Z2
//CString BuildSeleSQLPA::GetiPAfixZ1(CString &Ptype)
//{
//	CString strSQL=_T("");
//	try
//	{
//	}
//	catch(_com_error e)
//	{
//	}
//	return strSQL;
//}
//
////iPAs://��֧ͨ�� Cphs.cpp(2971,3016)
////iPASS://����֧��
////iPAGS://����֧��
//CString BuildSeleSQLPA::GetiPAsSQL(CString &Ptype)
//{
//	CString strSQL=_T("");
//	CString sTmp=_T("");
//	int k=0;
//	CDaoRecordset rsX;//cg?
//	try
//	{
//		for(;k<= m_PAPara->gnDW_delta;k++)
//		{
//			sTmp.Format(_T("%g"),m_PAPara->dj * (1 - k * 0.01));
//			strSQL = _T("SELECT * FROM ") + m_PAPara->tbnPA + _T(" WHERE Dw>= ") +sTmp+ _T(" AND Dw<= ") ;
//			sTmp.Format(_T("%g"),m_PAPara->dj * (1 + k * 0.01));
//			strSQL+= sTmp + _T(" AND Pmax>=");
//			sTmp.Format(_T("%g"),m_Para->tmpSelPJG/m_Para->iNumPart);
//			strSQL+=sTmp + _T(" AND Tj>=");
//			sTmp.Format(_T("%g"),m_PHSPara->t0);
//			strSQL+= sTmp + _T(" AND trim(CustomID)=\'") + Ptype + _T("\' ORDER BY tj,Dw,Weight");
//			if(rsX.IsOpen())
//				rsX.Close();
//			rsX.Open(dbOpenSnapshot,strSQL);
//			if( rsX.IsEOF() && rsX.IsBOF() );
//			else
//				//û�ҵ���ÿ���⾶����1%��������
//				k=m_PAPara->gnDW_delta+1;
//			
//		}
//		if(m_PAPara->gbPAForceZero && (rsX.IsEOF() && rsX.IsBOF()))
//		{
//			for( k = 0 ;k<= m_PAPara->gnDW_delta;k++)
//			{
//				sTmp.Format(_T("%g"),m_PAPara->dj * (1 - k * 0.01));
//				strSQL = _T("SELECT * FROM ") + m_PAPara->tbnPA + _T(" WHERE Dw>= ") +sTmp+ _T(" AND Dw<= ") ;
//				sTmp.Format(_T("%g"),m_PAPara->dj * (1 + k * 0.01));
//				strSQL+= sTmp + _T(" AND Pmax IS NULL");
//				strSQL+= _T(" AND Tj>=");
//				sTmp.Format(_T("%g"),m_PHSPara->t0);
//				strSQL+= sTmp + _T(" AND trim(CustomID)=\'") + Ptype + _T("\' ORDER BY tj,Dw,Weight");
//				if(rsX.IsOpen())
//					rsX.Close();
//				rsX.Open(dbOpenSnapshot,strSQL);
//				if( rsX.IsEOF() && rsX.IsBOF() )
//				{
//					;
//				}
//				else
//				{
//					k=m_PAPara->gnDW_delta+1;
//					AfxMessageBox("����:\n\n�ܲ��������û��,\n���򽫲�����ܲ�����!");
//				}
//
//			}
//		}
//	}
//	catch(_com_error e)
//	{
//	}
//	return strSQL;
//}
//
////iPAD4LA:
////iPAD11LD:
////iPADLR:
////iPALX: Cphs.cpp(3023,3031)
//CString BuildSeleSQLPA::GetiPAD4LA(CString &Ptype)
//{
//	CString strSQL=_T("");
//	CString sTmp=_T("");
//	CDaoRecordset rsX;//cg?
//	try
//	{
//		//��Щ(�̷�ú�۹ܵ��ܲ�)��ܾ��޹�
//		strSQL = _T("SELECT * FROM ") + m_PAPara->tbnPA + _T(" WHERE Pmax>=") ;
//		sTmp.Format(_T("%g"),m_Para->tmpSelPJG/m_Para->iNumPart);
//		strSQL+=sTmp+ _T(" AND Tj>=");
//		sTmp.Format(_T("%g"),m_PHSPara->t0);
//		strSQL+=sTmp+_T(" AND trim(CustomID)=\'") + Ptype + _T("\' ORDER BY tj,Pmax,Weight");
//		if(rsX.IsOpen())
//			rsX.Close();
//		rsX.Open(dbOpenSnapshot,strSQL);
//	}
//	catch(_com_error e)
//	{
//	}
//	return strSQL;
//}
