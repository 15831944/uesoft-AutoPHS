// CSPRSel.cpp: implementation of the CSPRSel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSPRSel.h"
#include "modPHScal.h"
#include "EDIBgbl.h"
//#include "TmpCSLenTable.h"
#include "ConstantSpring.h"
//#include "ConstantSpringTable.h"
#include "HDForceDist.h"
#include "HDCrude.h"
#include "LugDiaOfCSPR.h"
#include "Connect.h"
#include "TmpCSLen.h"
#include "ResultZB.h"
#include "PictureClipData.h"
#include "CrudeTurnbuckleHDCz.h"
#include "user.h"
#include "EDIBAcad.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSPRSel::CSPRSel()
{
	
}

CSPRSel::~CSPRSel()
{

}

BOOL CSPRSel::SelCSPRToTable(float& TmpPaz,float& tmpSelPJG4CSPRFiJ,long &mlfx,long &mlfx0)
{
	if(m_iSEQofSPR < modPHScal::giWholeSprNum)
			modPHScal::giCSPRindex[m_iSEQofSPR] = m_iSEQofPart;
    //��λ������Ʊ�־
//	TmpCSLenTable::SetSpecialDesign("");
	double TmpPgz = ConstantSpring::GetPGZ(m_dbSelPJG4CSPRPgz,m_strPartID,m_iFirstCal);
	TmpPaz = TmpPgz;

	//�õ�����ѡ�͵�λ��
	double Tmpyr =  ConstantSpring::Getyr(modPHScal::yr);       //yr ��λ:mm
	double tmpDist = Tmpyr / modPHScal::giWholeSprNum;

	//��λ��ϵ��ֵ�����ͽ��Ͽ�ԭ��ȷ��λ��ϵ�С�

//	tmpDist=ConstantSpringTable::GetDispSerial(tmpDist);

	//�غ�������λΪkN*mm,��N*m,λ�Ƶ�λΪmm,���ص�λΪkgf(��daN=10N)	
	double	tmpCapacity = TmpPgz * tmpDist * EDIBgbl::kgf2N / 1000;

	HDForceDist mDist;
	GetHDForceDist(&mDist,tmpCapacity,tmpDist,m_iSumSerialNum); //�ڵ������Ա����ҵ����ʵĵ���
	
	//�������ֵ
	SetCSPRValue(mDist.GetDH(),tmpDist);

	double tmpSelOpLoad = 0; //ѡ��Ĺ������أ���λΪkgf
	tmpSelOpLoad = mDist.GetCapacity()/ tmpDist * 1000 / EDIBgbl::kgf2N;

	//�ڵ��ɰ�װ���ݱ��еõ����ʵĺ�������
	HDCrude mHDCrude;
	GetHDCrude(&mHDCrude); 
	
	int iFjmin = mHDCrude.GetFjmin();
	if( m_iFirstCal >= 2 )
	{
		//�����ɽ���ĸֱ��FiJ��FiK��S��I��T��
		//���ݵ�����е������p20 C�ͺ����TֵӦ�ø�ΪIֵ������ǰ���LHE��PHE(1<=DH<=83)�ĳߴ��Ǻϡ�
		//��̲ŷ��㡣
		//�����ݵ�����е������������ֱ����Ҫ�������������ҲҪ������Сֱ����������װ��������
		//�ڶ��μ���,��¼����ֱ��,�Ա��ҳ����ֵ�����еĺ���ɽ���ĸԼ�������ֱ��
		if( m_strPartID == _T("LHG") )
		{
			//�·��������ɣ���Ҫѭ��3�β����ҳ��������ֱ��,//�·�����ɽ���ĸֱ��=��������ֱ��
			modPHScal::PtypeDiameter[m_i] = modPHScal::PtypeDiameter[m_i + 1];
		}
		else
		{
			////�����������ɣ�ѭ��2�β����ҳ��������ֱ��,���·�����ɽ���ĸֱ��=��ǰ������ֱ��
			modPHScal::PtypeDiameter[m_i] = modPHScal::PtypeDiameter[m_i - 1];
		}
		iFjmin = modPHScal::PtypeDiameter[m_i];
	}
	
	//�õ���������
	LugDiaOfCSPR mLugFig;
	GetLugDiaOfCSPR( &mLugFig,mHDCrude.GetWeight(),mHDCrude.GetCustomID(),iFjmin );
	
	//���ú�������֮ǰ�Ĳ���ѡ��ֱ��
	SetAforeCompDia( mLugFig.GetFiJ() );

	double tmpSelPJG4CSPRPgzNext = 0;
	if( modPHScal::gbAddPartWeight2CSPRPgz )
	{
		//�ۼ������������������   //�ۼ����������ɹ�������
		//���Ը�������˨��ĸ���������Լ���������������1m���ǣ�
		//����������غ�
		tmpSelPJG4CSPRPgzNext = m_dbSelPJG4CSPRPgz + mHDCrude.GetWeight() * m_iNumPart;
	}
	if( modPHScal::gbAddPartWeight2PMax)
	{
		//�ۼ��������������
		//���Ը�������˨��ĸ���������Լ���������������1m���ǣ�
		//����ɽ���ĸֱ�������غ�
		m_dbSelPJG4CSPRFiK = m_dbSelPJG4CSPRFiJ + mHDCrude.GetWeight() * m_iNumPart;
		//������������غ�
		m_dbtmpSelPJG = m_dbtmpSelPJG + mHDCrude.GetWeight() * m_iNumPart;
	}
	else
	{
		//����ɽ���ĸֱ�������غ�
		m_dbSelPJG4CSPRFiK = m_dbSelPJG4CSPRFiJ;
	}
	tmpSelPJG4CSPRFiJ = m_dbSelPJG4CSPRFiK;

	//�õ����ʵ�����(����С�������ƣ������ٵ�һ��)
	LugDiaOfCSPR mLugFik;
	GetLugDiaOfCSPR( &mLugFik,mHDCrude.GetWeight(),mHDCrude.GetCustomID(),modPHScal::PtypeDiameter[m_i] );
	modPHScal::sSprInfo[m_iSEQofSPR].FiJ = mLugFik.GetFiJ();
	modPHScal::sSprInfo[m_iSEQofSPR].FiK = mLugFik.GetFiK();

	ReplaceCNTB(&mLugFik);

	//���в��볤��
	
	SetTempCSLen(&mHDCrude,tmpDist,tmpSelOpLoad); //дTempCSLen���

	SetResultZB(&mHDCrude,&mLugFik,mlfx,mlfx0);
	return TRUE;
}

//�õ������������ݱ�����Ҫ������ݼ�
void CSPRSel::GetHDForceDist(HDForceDist* pDist,float tmpCapacity,float tmpDist,int iSumSerialNum) 
{
	CString SQLx = _T("SELECT * FROM [") + modPHScal::tbnHDproperty + _T("] ORDER BY Capacity");
	pDist->Initialize(SQLx);
	if( pDist->IsNull()  )
	{
		throw "��ʱ����";
		//����غ�������Ϊ�գ�������
	}
	
	float Cmin,Cmax;
	Cmin = pDist->GetMinCapacity();
	Cmax =  pDist->GetMaxCapacity();

	CString sTmp,sTmp2;
	if( tmpCapacity < Cmin ) //������������С�Ļ�С
	{
		//������������С�Ļ�С
		sTmp.Format(GetResStr(IDS_CapacityLitterCmin),_T("\%g"),_T("\%g"));
		sTmp2.Format(sTmp,tmpCapacity, Cmin);
		throw sTmp2;
	}
	if( tmpCapacity > Cmax ) //�������������Ļ���
	{
		throw GetResStr(IDS_NotFoundConstantSPRInTBNHDproperty);
	}

	sTmp.Format(_T("%s>=%g"),pDist->GetCapacityField(),tmpCapacity);
	pDist->FindFirst(sTmp);

	if( tmpDist < pDist->GetMinDist() )//����Сλ��С
	{
		sTmp.Format(GetResStr(IDS_ShouldUseSpring),_T("\%g"),_T("\%g"));
		sTmp2.Format(sTmp,tmpDist,pDist->GetMinDist() );
		throw sTmp2;
	}

	if( tmpDist > pDist->GetMaxDist() ) //�����λ�ƴ�
	{
		if( iSumSerialNum >= modPHScal::giWholeSprNum )
		{
			//����ģ���еĴ�����,�����Ҫ������ƣ��������������ʹ�ã�,
			//ֻҪһ������Ϳ�����,��������λ����Ϊ��������
			modPHScal::gbCSPRneedSpecialDesign = true;
			EDIBgbl::dbPRJ.Execute(GetResStr(IDS_ESPECIAL_DESIGN_SQL));
		}
	}
	
	//�Ƚ�ǰһ���Ƿ���ӽ�����
	double C1=0,C2=0;
	C1 = pDist->GetCapacity();
	pDist->MovePrev();
	if( !pDist->IsBOF() )
	{
		C2 = pDist->GetCapacity();
		if( fabs(C1 - tmpCapacity) < fabs(C2 - tmpCapacity) )
		{
			//�غ��������ӽ�C1,�ص������ӽ��ļ�¼
			pDist->MoveNext();
		}
	}
	else
	{
		pDist->MoveNext();
	}
}

//�������ֵ
void CSPRSel::SetCSPRValue(int iDH,double tmpDist)
{
	if(m_iSEQofSPR<modPHScal::SprInfoIndex)
	{
		modPHScal::sSprInfo[m_iSEQofSPR].SerialNum = 1;
		modPHScal::sSprInfo[m_iSEQofSPR].DH = iDH;
		modPHScal::sSprInfo[m_iSEQofSPR].haz = fabs(modPHScal::yr) / modPHScal::giWholeSprNum * modPHScal::sSprInfo[m_iSEQofSPR].SerialNum;
		modPHScal::sSprInfo[m_iSEQofSPR].HeatDisp = modPHScal::sSprInfo[m_iSEQofSPR].haz;
		modPHScal::sSprInfo[m_iSEQofSPR].CheckDisp = (modPHScal::gbMinMaxDispByActualDisp?modPHScal::sSprInfo[m_iSEQofSPR].HeatDisp:tmpDist);
		modPHScal::sSprInfo[m_iSEQofSPR].SumDisp = tmpDist;
	}
}

//�õ��������ݱ�������Ҫ������ݼ�
void CSPRSel::GetHDCrude(HDCrude *pHDCrude)
{
	CString sTmp,SQLx;
	
	SQLx = _T("SELECT * FROM [") + modPHScal::tbnHDCrude + _T("]")  ;
	if( m_iSEQofSPR < modPHScal::SprInfoIndex  )
	{
		SQLx = SQLx+_T(" WHERE "); 
	}
	if(m_iSEQofSPR < modPHScal::SprInfoIndex)
	{
		sTmp.Format(_T("%s=%d"),pHDCrude->GetDHField(),modPHScal::sSprInfo[m_iSEQofSPR].DH);
		SQLx+=sTmp;
	}
	if(m_iSEQofSPR < modPHScal::SprInfoIndex && modPHScal::gbCSPRneedSpecialDesign)
	{
		sTmp.Format(_T(" AND %s<=%g AND %s>=%g"),pHDCrude->GetfBminField(),pHDCrude->GetfBmaxField(),
			modPHScal::sSprInfo[m_iSEQofSPR].CheckDisp,modPHScal::sSprInfo[m_iSEQofSPR].CheckDisp);
		SQLx += sTmp;
	}
	sTmp.Format(_T(" AND trim(%s)=\'%s\' ORDER BY %s,%s"),pHDCrude->GetCustomIDField(),m_strPtype,
		pHDCrude->GetDHField(),pHDCrude->GetWeightField());
	SQLx += sTmp;
	pHDCrude->Initialize(SQLx);
}

//�õ���������
void CSPRSel::GetLugDiaOfCSPR(LugDiaOfCSPR *pLug,double dbWeight,CString strCustomID,int iFjmin)
{
	//�����������ɣ�ѭ��2�β����ҳ��������ֱ��
	CString sTmp,sTmp1,SQLx;
	
	if(modPHScal::gbByForce) //�Ƿ��ú���ѡ����
	{
		sTmp.Format(_T(" %s>=%g "),pLug->GetPmaxKgfField(),
			(m_strPartID == _T("LHG") ? m_dbSelPJG4CSPRFiJ +dbWeight * modPHScal::gintParallelNum : m_dbSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
		sTmp1.Format(_T(" ORDER BY %s,%s"),pLug->GetPmaxField(),pLug->GetFiJField() );
	}
	else
	{
		sTmp1.Format(_T(" ORDER BY %s"),pLug->GetFiJField() );
		
		//���ݾݿ��и�����û��fBmin��fBmax�ֶ�
	/*	sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
			modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
			modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
		*/
	}
	SQLx.Format( _T("SELECT * FROM [%s] WHERE %s AND %s>=%d AND %s<=%d AND %s>=%d  AND trim(%s)=\'%s\' %s"),
		modPHScal::tbnLugDiaOfCSPR,					
		sTmp,pLug->GetFiJField(),
		(iFjmin <= 0 ? 10 : iFjmin),
		pLug->GetminDHField(),modPHScal::sSprInfo[m_iSEQofSPR].DH,
		pLug->GetmaxDHField(),modPHScal::sSprInfo[m_iSEQofSPR].DH,
		pLug->GetCustomIDField(),strCustomID,
		sTmp1);
	pLug->Initialize(SQLx);
}

//���ú�������֮ǰ�Ĳ���ѡ��ֱ��
void CSPRSel::SetAforeCompDia(int iFiJ)
{
	int k=0;
	if( m_iSEQofSPR + 1 > modPHScal::giWholeSprNum )
	{
		return ;
	}
	if( m_iFirstCal > 1 )
	{
		//��һ�μ��㣬֮ǰ����������ֱ��Լ��ֵ=��ǰ������ɽ���ĸֱ��
		//�ڶ��μ��㣬��������ֱ��ѡ�������ɽ���ĸֱ������ʱ����ֱ����Ȼ�����˺���ɽ���ĸ����Сֱ����
		return ;
	}
	if( m_iSEQofSPR > 0 )
	{
		k = modPHScal::giCSPRindex[m_iSEQofSPR - 1] + 1;
	}
	for(;k<= modPHScal::giCSPRindex[m_iSEQofSPR];k++)
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

/*
������:�°���
ʱ��: 2007.11.16
��ʵ�ʵ����滻ƥ��ʽ
tmpSQL0:��Ҫ�滻���ַ�;pLugFik:??;
*/
void CSPRSel::ReplaceCNTB(LugDiaOfCSPR *pLugFik)
{
	CString tmpFD0;
	bool bFound = false;  
	int n = 0,m = 0,k = 0;
	COleVariant vTmp1;
	CString LogicalOpr = _T("=<>") ;  //�߼����������
	if( m_strtmpSQL0 != _T("") )
	{//�������ӱ��ʽ,����ȥ�����е�Cnte.�ַ�
		m_strtmpSQL0.MakeUpper();
		n=m_strtmpSQL0.Find((_T("CNTE.")));
		while(n!=-1)
		{
			m_strtmpSQL0 =m_strtmpSQL0.Left(n)+m_strtmpSQL0.Mid(n+5);
			n=m_strtmpSQL0.Find((_T("CNTE.")));
		}
		
		n=m_strtmpSQL0.Find((_T("CNTB.")));
		while(n!=-1)
		{
			bFound = false;
			for( m = n + 5 ;m< m_strtmpSQL0.GetLength();m++)
			{
				for( k = 0;k<LogicalOpr.GetLength();k++)
				{
					//���Ƿ�����κ��߼�������
					if( LogicalOpr.Mid(k, 1) == m_strtmpSQL0.Mid(m, 1) )
					{
						tmpFD0 = m_strtmpSQL0.Mid( n + 5, m - n - 5);
						
						//�����������Ϣ������һ����¼��pLugFik��
						CString strValue;
						strValue = pLugFik->GetFieldValue(tmpFD0);
						m_strtmpSQL0 =m_strtmpSQL0.Left(n) + (strValue.IsEmpty() ? _T("0") : strValue) + m_strtmpSQL0.Mid( m);
						bFound = true;
						break;
					}
				}
				
				//����һ�����˳���������һ��_T("Cntb.")
				if( bFound ) break;
			}
			
			n=m_strtmpSQL0.Find((_T("CNTB.")),n+1);
		}
	}
}

//����ǰһ������Ĳ��볤��
double CSPRSel::GetPrePartInsertLen()
{
	if( m_strPartID.IsEmpty() || m_strPrePartID.IsEmpty() )
	{
		return 0.0;
	}
	Connect mConnect;
	CString strSql;
	strSql.Format("SELECT * FROM %s WHERE %s = \'%s\' AND %s = \'%s\'",
		mConnect.GetTableName(),mConnect.GetCNTBField(),m_strPrePartID,
		mConnect.GetCNTEField(),m_strPartID);
	mConnect.Initialize(strSql);
	if( mConnect.IsNull() )
		return 0.0;
	return mConnect.GetCNTBextlenFLD();
}

//�������ݵ�TempCSLen�����
void CSPRSel::SetTempCSLen(HDCrude *pHDCrude,double tmpDist,double tmpSelOpLoad)
{
	TmpCSLen mCSLen;
	mCSLen.SetAllBH( pHDCrude->GetBH() );
	mCSLen.SetAllLugDiameter( modPHScal::PtypeDiameter[m_i] );
	mCSLen.SetAllSumDist( tmpDist );
	double dbSizeC = 0;
	if( m_strPartID == _T("LHG") )
		dbSizeC = modPHScal::DblHangerRodDist;
	else
		dbSizeC = pHDCrude->GetSizeC();
	modPHScal::sSprInfo[m_iSEQofSPR].sizeC = dbSizeC ;
	mCSLen.SetAllSizeC( dbSizeC );
	
	mCSLen.SetAllHeatDist( modPHScal::sSprInfo[m_iSEQofSPR].HeatDisp );

	CString strText;
	strText.Format( _T("%s"),modPHScal::gsCSPRUnitOfLoad );
	mCSLen.SetAllUnitForce(strText);

	if( modPHScal::yr > 0 )
	{
		strText.Format( _T("%s"),modPHScal::gSprDirectionChar.CSPR_UP );
	}
	else
	{
		strText.Format( _T("%s"),modPHScal::gSprDirectionChar.CSPR_Down );
		
	}
	mCSLen.SetAllDistFX( strText );

	double dbScale = 1.0;
	if(modPHScal::gsCSPRUnitOfLoad == _T("daN") )
	{
		dbScale = EDIBgbl::kgf2N / 10.0;
	}
	else
	{
		dbScale = EDIBgbl::kgf2N;
	}
	mCSLen.SetAllSelOpLoad( tmpSelOpLoad * dbScale );
	
	double dbOpLoad = 0.0;
	if( m_strPartID == _T("LHG") )
	{
		dbOpLoad = (m_dbSelPJG4CSPRPgz + modPHScal::g_fLHGWeight)/modPHScal::gintParallelNum;
	}
	else
	{
		dbOpLoad = m_dbSelPJG4CSPRPgz / modPHScal::gintParallelNum;
	}
	mCSLen.SetAllOpLoad( long(dbOpLoad * dbScale) );
	mCSLen.SetAllInstallLoad( long(dbOpLoad * dbScale) );
}

//�������ݵ�ZB�����
/*
mlfx:��ǰ��������ת����; mlfx0:ǰһ��������ת����
*/
void CSPRSel::SetResultZB(HDCrude *pCrude,LugDiaOfCSPR *pLugFik,long& mlfx,long& mlfx0)
{
	CString strSql;
	ResultZB mZB;
	strSql.Format(_T("SELECT * FROM %s WHERE %s=%d AND %s=%d AND %s =%d AND %s = %d ORDER BY %s"),
		EDIBgbl::Btype[EDIBgbl::TZB],mZB.GetZdjhField(),modPHScal::zdjh,
		mZB.GetCustomIDField(),EDIBgbl::SelVlmID,mZB.GetNthField(),m_inth,
		mZB.GetRecnoField(),m_iSEQofPart,
		mZB.GetRecnoField() );
	mZB.Initialize(strSql);
	if( mZB.IsNull() )
		return;

	mZB.SetNth( m_inth ); //��׼֧����·��=1
	double dbPreInsertLen = 0.0;
	dbPreInsertLen = GetPrePartInsertLen();
	mZB.SetPreviousPartInsertLen( dbPreInsertLen ); //ǰһ�������볤��
	

	//�õ������������
	PictureClipData mClipData;
	strSql.Format( _T("SELECT * FROM %s WHERE trim(%s)=\'%s\'"),mClipData.GetTableName(),mClipData.GetCustomIDField(),m_strPtype );
	mClipData.Initialize(strSql);
	CString strBHFormat;
	strBHFormat = mClipData.GetBHFormat();
	if( !strBHFormat.IsEmpty() )
	{
		TmpCSLen mCSLen;
		strSql.Format(_T("SELECT (%s) AS sBHFormat FROM %s"),strBHFormat,mCSLen.GetTableName() );
		mCSLen.Initialize(strSql);
		CString strBHFormat;
		strBHFormat = mCSLen.GetFieldValue( _T("sBHFormat") );
		mZB.SetCLgg(strBHFormat);
		mZB.SetBHformat( strBHFormat ); //�����������
		
		modPHScal::sSprInfo[m_iSEQofSPR].BHFormat=strBHFormat;
	}
	modPHScal::sSprInfo[m_iSEQofSPR].CustomID=m_strPartID;
	modPHScal::sSprInfo[m_iSEQofSPR].FiJ=modPHScal::PtypeDiameter[m_i];
	
	mZB.SetRecno(m_iSEQofPart);
	mZB.SetVolumeID(EDIBgbl::SelVlmID);
	mZB.SetZdjh(modPHScal::zdjh);
	mZB.SetCustomID(m_strPtype);
	mZB.SetClassID(modPHScal::glClassID);
	mZB.SetPjs(m_dbtmpSelPJG);

	//ȷ���������
	if( modPHScal::gbAutoAlignCSPRtoSA && m_strPartID !=_T("LHG") )
	{
		//������Զ���������
		mlfx = modPHScal::GetPhsSAfx(modPHScal::fx + modPHScal::gf);
		mlfx0 = 0;
	}
	mZB.SetFx(mlfx);
	mZB.SetRotation(m_dbmlRot);//���y���귽��

	mZB.SetCLnum(m_iNumPart);
	mZB.SetSEQ(m_iSEQofPart); //��ǰ����������ģ���еĲ���������
	mZB.SetCLmc( mClipData.GetDescription() );

	double dbWeight = 0.0;
	//T5��LHG����ͬʱ���֣���Ϊ���ɺͺ������ɲ���ͬʱ����
	if( m_strPartID == _T("LHG") || m_strPartID == _T("T5") )
		modPHScal::g_fLHGWeight = pCrude->GetWeight() ;
	mZB.SetCldz( dbWeight*m_iNumPart );
	mZB.SetIndex( modPHScal::glIDIndex ); //���������ţ��Ա��ڻ��ܲ���ʱ�ܹ�Ѹ��ʶ��������������
	mZB.SetID( mClipData.GetID() );
	
	mZB.SetCLcl( GetResStr(IDS_GROUPWARE) );
	mZB.SetT( (int)pCrude->GetT() );
	
	//�������Ӽ��ߴ�x,y,��Ҫ�����Ƴ���GL�����ں����������ݱ��С�
	//���ǶԳ��ݵ�����е���������ԡ��ó�sizeH=E +or- fB/2��
	//Height_SPRINGsL5 = IIf(IsNull(rsX(_T("x"))), 0, rsX(_T("x"))) * 2
	//���tbnCSPRINGL5Crude���ҳ��ɽ���ĸ�ĳ��ȡ��ɲ������
	//2005.11.01����������˾���ϸ߼�����ʦָ�������������ȼ��㲻�ԡ�
	//��ligb��飬�����ں��������ɽ���ĸ���ȼ��㱻�򵥵ش���Ϊ������ֱ�����仯��
	//ʵ���ϸ������������۳��ݵ������Ǵ������ɣ����������ɽ���ĸ���ȶ�������������ͺš�λ��ֵ��ͬ���仯��
	//�����Ѿ���ligb�޸���ȷ��2005.11.01 20:00-23:00��
	float tmpMovement=0;//�������ɲ��λ��tmpMovement������ʹ�����ֵtmpMovementȥ������������ݱ����ֵһ����Ϊ�Ǽ���λ�ƻ��ѡ��λ�ƣ�����UESoft��Ϊ��Ҳ������ʵ����λ��
	if(modPHScal::gbCalcCSPRHeight_BySelDisp)
		tmpMovement=modPHScal::sSprInfo[m_iSEQofSPR].SumDisp;
	else
		tmpMovement=modPHScal::sSprInfo[m_iSEQofSPR].haz;
	CrudeTurnbuckleHDCz mCrudeT;
	strSql.Format(_T("SELECT * FROM [%s] WHERE trim(%s)=\'%s\' AND %s<=%d AND %d<=%s AND %s<=%g AND %s>=%g "),
				   modPHScal::tbnCSPRINGL5Crude,mCrudeT.GetCustomIDField(),
				   m_strPtype,mCrudeT.GetminDHField(),modPHScal::sSprInfo[m_iSEQofSPR].DH,
				   modPHScal::sSprInfo[m_iSEQofSPR].DH,mCrudeT.GetmaxDHField(),
				   mCrudeT.GetfBminField(),tmpMovement,
				   mCrudeT.GetfBmaxDHField(),tmpMovement);
	mCrudeT.Initialize( strSql );
	
	double dbMinLug = 0.0, dbMaxLug = 0.0;
	if( m_strPartID == _T("LHA") || m_strPartID == _T("LHB") || m_strPartID == _T("PHA")|| m_strPartID == _T("PHB") )
	{
		dbMinLug = pCrude->GetPP();
		dbMaxLug = dbMinLug;
	}
	else
	{
		dbMinLug = mCrudeT.GetSize4();
		dbMaxLug = mCrudeT.GetSize5();
	}
	mZB.SetAllowedMinLugExt( dbMinLug );
	mZB.SetAllowedMaxLugExt( dbMaxLug );

	double dbSigDim = GetSigDim(pCrude,&mCrudeT,pLugFik);
	mZB.SetSizeDim(dbSigDim); //
	mZB.SetSizeH(dbSigDim); //ʵ�ʸ߶Ⱥͳ���
	if( EDIBAcad::g_bBomSprFactory )//newHS->ֻ��ѡ���˱�ע�������ɳ��Ҳ�д�����ݿ�
	{
		mZB.SetClbz( modPHScal::gsCSPRmanufactory );
	}
	//��ʱ����gdw1�ֶα��������أ�����������GetBoltsNutsAndAttachmentsCLggʹ��
	mZB.SetGDW1( m_dbSelPJG4CSPRFiK/m_iNumPart ); 
	mZB.SetSpringNo( modPHScal::sSprInfo[m_iSEQofSPR].DH );
	mZB.SetLugDia( modPHScal::sSprInfo[m_iSEQofSPR].FiJ );
	mZB.SetHeatDisp( modPHScal::sSprInfo[m_iSEQofSPR].HeatDisp );
	mZB.Update();
}

//�õ�ʵ�ʳߴ�
double CSPRSel::GetSigDim(HDCrude *pCrude,CrudeTurnbuckleHDCz *pCrudeTurn,LugDiaOfCSPR *pLugFik)
{
	//����������ɵ�ѡ��λ��modPHScal::sSprInfo[iSEQofSPR].SumDisp���õ�������λ���ֶ�fBmin��һ��ֵ��
	//��������ɵİ�װ�߶�H1(sngH1xmax)=ֱ�Ӳ���������λ���ֶ�fBmin��ֵ��
	//���򣬰���ֵ������λ�ơ�add by ligb on 2004.11.24
	CString sTmp;
	CString tmpSQL;
	double sngH1xmax = 0.0,sngDim = 0.0;

	sngH1xmax = pCrude->GetSizeH();
	if(modPHScal::gCH1distPos == 1 )
	{
		//E�ֶθ������м�λ�õĸ߶�
		if(modPHScal::gbCalcCSPRHeight_BySelDisp)
			sngDim = sngH1xmax + Sgn(modPHScal::yr) * modPHScal::sSprInfo[m_iSEQofSPR].haz /(modPHScal::bE_FormulaPlusHalf?2:1);
		else
			sngDim = sngH1xmax + Sgn(modPHScal::yr) * modPHScal::sSprInfo[m_iSEQofSPR].SumDisp /(modPHScal::bE_FormulaPlusHalf?2:1);
	}
	else if(modPHScal::gCH1distPos == 0 )
	{
		double fBmin=0.0;
		fBmin = pCrude->GetfBmin();
		//E�ֶθ�������Сλ��λ�õĸ߶�
		if( modPHScal::yr < 0 )
		{
			//��λ��<0
			//sngDim = sngH1xmax + (Sgn(modPHScal::yr) * modPHScal::sSprInfo[iSEQofSPR].haz + vtof(vTmp1)) /(modPHScal::bE_FormulaPlusHalf?2:1);
			//�о�������������λ�Ƽ��㹫ʽ������������㹫ʽ��ͬ������bE_FormulaPlusHalfΪ�Σ���Ҫ��2
			sngDim = sngH1xmax + (Sgn(modPHScal::yr) * (modPHScal::gbCalcCSPRHeight_BySelDisp?modPHScal::sSprInfo[m_iSEQofSPR].SumDisp:modPHScal::sSprInfo[m_iSEQofSPR].haz) + fBmin) /(modPHScal::bE_FormulaPlusHalf?2:2);
		}
		else
		{
			//��λ��>=0��������H1s=H1x+��λ��T
			sngDim = sngH1xmax + (Sgn(modPHScal::yr) * (modPHScal::gbCalcCSPRHeight_BySelDisp?modPHScal::sSprInfo[m_iSEQofSPR].SumDisp:modPHScal::sSprInfo[m_iSEQofSPR].haz) + fBmin) /(modPHScal::bE_FormulaPlusHalf?2:2) + (m_strPartID == _T("ZHB") ? -1 : 0) * modPHScal::sSprInfo[m_iSEQofSPR].haz;
		}
	}
	else if(modPHScal::gCH1distPos == 2 )
	{
		//E�ֶθ��������λ��λ�õĸ߶�
		double fBmin=0.0,fBmax=0.0;
		fBmin = pCrude->GetfBmin();
		fBmax = pCrude->GetfBmax();
		sngDim = sngH1xmax + (fBmin - fBmax)/2;
		if( modPHScal::yr < 0 )
		{
			//��λ��<0
			sngDim = sngDim + (Sgn(modPHScal::yr) * (modPHScal::gbCalcCSPRHeight_BySelDisp?modPHScal::sSprInfo[m_iSEQofSPR].SumDisp:modPHScal::sSprInfo[m_iSEQofSPR].haz) + fBmax) /(modPHScal::bE_FormulaPlusHalf?2:2);
		}else
		{
			//��λ��>=0
			sngDim = sngDim + (Sgn(modPHScal::yr) * (modPHScal::gbCalcCSPRHeight_BySelDisp?modPHScal::sSprInfo[m_iSEQofSPR].SumDisp:modPHScal::sSprInfo[m_iSEQofSPR].haz) + fBmax) /(modPHScal::bE_FormulaPlusHalf?2:2) + (m_strPartID == _T("ZHB")? -1:0) * modPHScal::sSprInfo[m_iSEQofSPR].haz;
		}
		//���������򻪶�ʯ�;�����װ�������ܳ��ֳܼܷ��Ľ��ͣ����Ϲ�ʽ�ɷ���,Ӧ�ð����й�ʽ
		//sngDim = sngH1xmax + Sgn(modPHScal::yr) * modPHScal::sSprInfo[iSEQofSPR].haz /(modPHScal::bE_FormulaPlusHalf?2:0);
	}
	
	double Height_SPRINGsL5 = 0.0;
	Height_SPRINGsL5 = pCrudeTurn->GetSizeH();
	if(modPHScal::gCH1Pos == 1 )
	{
		//H1�ڻ�����˿�м�λ�õĸ߶�
		sngDim = sngDim + Height_SPRINGsL5 / 2;
	}
	else if(modPHScal::gCH1Pos == 2 )
	{
		//H1�ڻ�����˿���λ�õĸ߶�
		sngDim = sngDim + Height_SPRINGsL5;
	}
	else if(modPHScal::gCH1Pos == 0 )
	{
		//H1�ڻ�����˿���λ�õĸ߶�
		
	}
	//���û����������˿��Ե��������
	//���ݵĿ������й�ʽ
	if( !modPHScal::gbCH1IncludeB )
	{
		sngDim = sngDim + pCrudeTurn->GetSize4() ;
	}
	if(m_strPartID==_T("LHE") || m_strPartID==_T("LHE1") || m_strPartID==_T("PHE") 
		|| m_strPartID==_T("PHE1") || m_strPartID==_T("PHC") || m_strPartID==_T("LHC"))
	{
		//��ʽ�������ӣ�Ҫ���϶���߶�T��
		if( !modPHScal::gbCH1IncludeT )
		{
			sngDim = sngDim + pLugFik->GetT();
		}
	}
	return sngDim;
}



