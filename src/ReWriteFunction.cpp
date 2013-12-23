
#include "stdafx.h"
#include "ReWriteFunction.h"
#include "edibgbl.h"
#include "modPHScal.h"

extern CString vtos(COleVariant& v);
extern int vtoi(COleVariant & v);
extern float vtof(COleVariant &v);
extern float Sgn(float x);
/*
������:�°���
ʱ��: 2007.11.08
������� TableName:��ǰ֧���ܵ�ǰ·���㲿������ Zdjh:֧���Ӻ�;SelVlmID������; nth:·��
������� rsTZB:��ǰ֧���ܵ�ǰ·���㲿��  rsTmpZB:��ǰ֧���ܵ�ǰ·���㲿����������˨��ĸ����������
*/
BOOL GetPHSCompRs(CString TableName ,long zdjh,long SelVlmID, int nth,CDaoRecordset &rsTZB,CDaoRecordset &rsTmpZB)
{
	
	CString strSQL ;
	
	strSQL.Format(_T("SELECT * FROM %s WHERE zdjh=%d AND VolumeID=%d AND nth =%d ORDER BY recno"),
		TableName,zdjh,SelVlmID,nth);
	rsTZB.m_pDatabase=&EDIBgbl::dbPRJDB;
	rsTZB.Open(dbOpenDynaset,strSQL);
	
	strSQL.Format(_T("SELECT CustomID FROM ZB WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND [ClassID]<>%d AND [ClassID]<>%d AND [ClassID]<>%d AND [IsSAPart]<>-1 Order By recno"),
		SelVlmID,zdjh,nth,iBolts,iNuts,iAttached);
	rsTmpZB.Open(dbOpenDynaset,strSQL);
	if( rsTmpZB.IsEOF()&& rsTmpZB.IsBOF() )
		return FALSE;
	return TRUE;
}

/*
������:�°���
ʱ��: 2007.11.09
��ʵ�ʵ����滻ƥ��ʽ
tmpSQL0:��Ҫ�滻���ַ�;rsDiaOfCSPRFiK:??;rsX:??
*/
void ReplaceCNTB(CString &tmpSQL0,CDaoRecordset &rsDiaOfCSPRFiK,CDaoRecordset &rsX )
{

	CString tmpFD0;
	bool bFound = false;  
	int n = 0,m = 0,k = 0;
	COleVariant vTmp1;
	CString LogicalOpr = _T("=<>") ;  //�߼����������
	if( tmpSQL0 != _T("") )
	{//�������ӱ��ʽ,����ȥ�����е�Cnte.�ַ�
		tmpSQL0.MakeUpper();
		n=tmpSQL0.Find((_T("CNTE.")));
		while(n!=-1)
		{
			tmpSQL0 =tmpSQL0.Left(n)+tmpSQL0.Mid(n+5);
			n=tmpSQL0.Find((_T("CNTE.")));
		}
		
		n=tmpSQL0.Find((_T("CNTB.")));
		while(n!=-1)
		{
			bFound = false;
			for( m = n + 5 ;m< tmpSQL0.GetLength();m++)
			{
				for( k = 0;k<LogicalOpr.GetLength();k++)
				{
					//���Ƿ�����κ��߼�������
					if( LogicalOpr.Mid(k, 1) == tmpSQL0.Mid(m, 1) )
					{
						tmpFD0 = tmpSQL0.Mid( n + 5, m - n - 5);
						if( modPHScal::glClassID == iCSPR )
						{
							//�����������Ϣ������һ����¼��rsDiaOfCSPRFiK������rsX���档
							rsDiaOfCSPRFiK.GetFieldValue(tmpFD0,vTmp1);
							tmpSQL0 =tmpSQL0.Left(n) + (vTmp1.vt==VT_NULL || vTmp1.vt==VT_EMPTY ? _T("0") : vtos(vTmp1)) + tmpSQL0.Mid( m);
						}
						else
						{
							rsX.GetFieldValue(tmpFD0,vTmp1);
							tmpSQL0 =tmpSQL0.Left(n) + (vTmp1.vt==VT_NULL || vTmp1.vt==VT_EMPTY ? _T("0") : vtos(vTmp1)) + tmpSQL0.Mid( m);
						}
						bFound = true;
						break;
					}
				}
				
				//����һ�����˳���������һ��_T("Cntb.")
				if( bFound ) break;
			}
			
			n=tmpSQL0.Find((_T("CNTB.")),n+1);
		}
	}
}


//����ǰһ������Ĳ��볤��
void SetPrePartInsertLen(int i,CString* pstrPartID,CString mviPreviousClassID,CDaoRecordset &rsConnect,CDaoRecordset &rsX,CDaoRecordset &rsTZB,CDaoRecordset &rsDiaOfCSPRFiK,CString &tmpExtLenFD)
{
	COleVariant vTmp2,vTmp1;
	rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp1);
	if( vTmp1.vt==VT_NULL || vtos(vTmp1) == _T("") )
	{
		if( pstrPartID[i]==_T("L7") || pstrPartID[i]==_T("G12") || pstrPartID[i]==_T("L7Dd") || pstrPartID[i]==_T("G12Lc") )
		{
			//�����ֶ��ӿ�������L8����,���Ҫ���⴦�����ܲ鵽��ȷ�Ĳ���L8�ĳߴ硣
			if( pstrPartID[i-1] == _T("L8") || pstrPartID[i-1] == _T("L8Dd") )
			{
				//ǰһ����L8
				//�Ȳ�����
				//�Ժ��ڼ������˳��Ⱥ���CalLugLength��ֻҪ������������������ɡ�
				rsX.GetFieldValue(tmpExtLenFD,vTmp2);
				rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
			}
			////////////////////////////////////////////
			if( pstrPartID[i+1] == _T("L8") || pstrPartID[i+1] == _T("L8Dd"))
			{
				rsX.GetFieldValue (tmpExtLenFD,vTmp2);
				rsTZB.SetFieldValue (_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
				rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp2);
				tmpExtLenFD = vtos(vTmp2);
			}
			////////////////////////////////////////
		}
	}
	else
	{
		if( pstrPartID[i].Left(2) == _T("L7")||pstrPartID[i].Left(2) == _T("G12")||pstrPartID[i] == _T("L3")||pstrPartID[i] == _T("L4")||pstrPartID[i] == _T("T1")||pstrPartID[i] == _T("D4")||pstrPartID[i] == _T("D5")||pstrPartID[i] == _T("D11"))
		{
			rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp2);
			tmpExtLenFD = vtos(vTmp2);
			
			//��Ϊ�ܲ���������ǰһ��������ֻ��Ҫ�ж����ĺ�һ��
			if(pstrPartID[i] != _T("D4") && pstrPartID[i] != _T("D5") && pstrPartID[i] != _T("D11"))
			{
				//�����ֶ��ӿ�������L8����,���Ҫ���⴦�����ܲ鵽��ȷ�Ĳ���L8�ĳߴ硣
				if( pstrPartID[i-1] == _T("L8") || pstrPartID[i-1] == _T("L8Dd") )
				{
					//ǰһ����L8
					//�Ȳ�����
					//�Ժ��ڼ������˳��Ⱥ���CalLugLength��ֻҪ������������������ɡ�
					rsX.GetFieldValue(tmpExtLenFD,vTmp2);
					rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
					//�ٲ��ֶ�����
					rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp2);
					tmpExtLenFD = vtos(vTmp2);
				}
				else
				{
					//ǰһ������L8
					if( i > 0 && mviPreviousClassID == iCSPR )
					{
						//ǰһ���Ǻ��
						//��Ҫ��������һ��������ɽ���ĸֱ����ȥ�������Ӵ�һ��ֱ��(�ֶ�R)
						//�⼸�ֺ������ɶ����ö�������
						if( pstrPartID[i-1] == _T("PHE1") || pstrPartID[i-1] == _T("PHE") || pstrPartID[i-1] == _T("LHE") || pstrPartID[i-1] == _T("LHE1") )
						{
							rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp2);
							tmpExtLenFD = vtos(vTmp2);
							rsDiaOfCSPRFiK.GetFieldValue(tmpExtLenFD,vTmp2);
							rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
						}
					}
					else
					{
						rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp2);
						tmpExtLenFD = vtos(vTmp2);
						rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)0));
					}
				}
			}
			//////////////////////////////////////////////////////////////////////
			if( pstrPartID[i+1] == _T("L8") || pstrPartID[i+1] == _T("L8Dd") )
			{
				rsX.GetFieldValue(tmpExtLenFD,vTmp2);
				rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
			}
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			if( pstrPartID[i] == _T("L8") || pstrPartID[i] == _T("L8Dd") )
			{
				//��ǰ����L7��G12,��L8
				if( pstrPartID[i+1] == _T("L7") || pstrPartID[i+1] == _T("G12") || pstrPartID[i+1] == _T("L7Dd") || pstrPartID[i+1] == _T("G12Lc") )
				{
					//����������ֶ�ֵ���´β��
					rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp1);
					tmpExtLenFD = vtos(vTmp1);
				}
				else
				{
					//���ݲ�����ֶ�ֱֵ�Ӳ��
					rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp1);
					tmpExtLenFD = vtos(vTmp1);
					rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)0));
				}
			}
			else
			{
				//��ǰ����L7��G12,����L8
				if( pstrPartID[i] == _T("L5") || pstrPartID[i] == _T("L6") )
				{
					//��ǰ����L7��G12,����L8,��L5��L6
				}
				else
				{
					//��ǰ����L7��G12,����L8,����L5��L6
					if( mviPreviousClassID == iCSPR )
					{
						//��ǰ�Ǻ��
						//��Ҫ��������һ��������ɽ���ĸֱ����ȥ�������Ӵ�һ��ֱ��(�ֶ�R)
						if( pstrPartID[i-1] == _T("PHE1") || pstrPartID[i-1] == _T("PHE") || pstrPartID[i-1] == _T("LHE") || pstrPartID[i-1] == _T("LHE1") )
						{
							rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp1);
							tmpExtLenFD = vtos(vTmp1);
							rsDiaOfCSPRFiK.GetFieldValue(tmpExtLenFD,vTmp1);
							rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),vTmp1);
						}
					}
					else
					{
						rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp1);
						tmpExtLenFD = vtos(vTmp1);
						rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)0));
					}
				}
			}
		}
	}
}

//�õ�������ʵ�ʸ߶Ⱥͳ���
double GetPartSizeH(int i,CDaoRecordset &rsX,const CString& strPartID,const CString*PtypeID)
{
	double dbSizeH = 0;
	COleVariant vTmp1,vTmp2;
	if( strPartID == _T("LS") || strPartID == _T("LT") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX.GetFieldValue( _T("sizeH"),vTmp1 );
		dbSizeH = -(vtof(vTmp1) + modPHScal::dwB / 2);
	}
	else if( strPartID == _T("D4LQ") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		dbSizeH = vtof(vTmp1) + modPHScal::dwB / 2;
		}
	else if( strPartID == _T("DLR") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		rsX.GetFieldValue(_T("size4"),vTmp2);
		dbSizeH = vtof(vTmp1)+ modPHScal::dwB / 2 - vtof(vTmp2);
	}
	else if( strPartID == _T("LM2") || strPartID == _T("LM3") || strPartID == _T("LM4") )
	{
		//ˮƽԲ�ιܵ��������ܣ��߶�ֵҪ�Ӳ۸ֺᵣ�߶�
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		//rsTZB.SetFieldValue(_T("sizeDIM")) = -(IIf(IsNull(rsX.Fields(_T("size5"))), 0, rsX.Fields(_T("size5"))) + rsX.Fields(_T("size7")) * 10)
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		dbSizeH = vtof(vTmp1);
	}
	else if( strPartID == _T("LN2") || strPartID == _T("LN3") )
	{
		//ˮƽԲ�ιܵ��۸ֺ����������ܣ��߶�ֵҪ��
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		dbSizeH = vtof(vTmp1);
	}
	else if(strPartID==_T("L15"))
	{
		//����Ժ�̶�֧��Z2/Z2A�µ�һ��ְ�L15,����ֵ������tbnAttachement���ֶ�size2�л�modPHScal::gfPSThickness
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		if(PtypeID[i+1] == _T("GZD"))
		{
			dbSizeH = 0.0;
			//������֧�գ��ְ���������
			//���ʵ�ʳߴ�
		}
		else
		{
			dbSizeH = modPHScal::gfPSThickness;
			//���ʵ�ʳߴ�
		}
	}
	else
	{
		if(modPHScal::glIDIndex==iSA)
		{
			dbSizeH = 0;
			//�����߶���Ϊ0,�Ա���ȷͳ�����˳���
			//���ʵ�ʳߴ�
		}
		else
		{
			//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
			rsX.GetFieldValue(_T("sizeH"),vTmp1);
			dbSizeH = vtof(vTmp1);
			//���ʵ�ʳߴ�
		}
	}
	return dbSizeH;
}

//�õ��������ɵİ�װ�߶�
double GetSngDim(float sngH1xmax,float Height_SPRINGsL5,int iSEQofSPR,CString PType,CString sPartID,CDaoRecordset &rsX,CDaoRecordset &rsTmp,CDaoRecordset &rsDiaOfCSPRFiK) 
{
	//����������ɵ�ѡ��λ��modPHScal::sSprInfo[iSEQofSPR].SumDisp���õ�������λ���ֶ�fBmin��һ��ֵ��
	//��������ɵİ�װ�߶�H1(sngH1xmax)=ֱ�Ӳ���������λ���ֶ�fBmin��ֵ��
	//���򣬰���ֵ������λ�ơ�add by ligb on 2004.11.24
	CString sTmp;
	CString tmpSQL;
	CDaoRecordset rsX1;			   
	tmpSQL = _T("SELECT * FROM [") + modPHScal::tbnHDCrude + _T("] WHERE ") + tmpSQL ;
	if(iSEQofSPR < modPHScal::SprInfoIndex)
		sTmp.Format(_T(" dh=%d"),modPHScal::sSprInfo[iSEQofSPR].DH);
	else sTmp = _T(" ");
	tmpSQL+=sTmp;
	if(iSEQofSPR < modPHScal::SprInfoIndex)
	{
		//sTmp.Format(_T(" AND fBmin<=%g AND fBmax>=%g "),modPHScal::sSprInfo[iSEQofSPR].CheckDisp,modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
		sTmp.Format(_T(" AND fBmin=%g "),modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
	}
	else
	{
		sTmp=_T(" ");
	}
	tmpSQL+=(modPHScal::gbCSPRneedSpecialDesign ? _T(" ") : sTmp) + _T(" AND trim(CustomID)=\'") + PType + _T("\' ORDER BY dh,Weight");
	rsX1.m_pDatabase=&modPHScal::dbZDJcrude;
	rsX1.Open(dbOpenSnapshot,tmpSQL);
	double sngDim = 0;
	COleVariant vTmp1,vTmp2;
	if(rsX1.IsBOF() && rsX1.IsEOF()||1)//�������ٽ�ֵʱ����(sizeH�ĸ߶�û�м���ʵ��λ��)pfg and lgb 2005.12.15
	{
		if(modPHScal::gCH1distPos == 1 )
		{
			//E�ֶθ������м�λ�õĸ߶�
			if(modPHScal::gbCalcCSPRHeight_BySelDisp)
				sngDim = sngH1xmax + Sgn(modPHScal::yr) * modPHScal::sSprInfo[iSEQofSPR].haz /(modPHScal::bE_FormulaPlusHalf?2:1);
			else
				sngDim = sngH1xmax + Sgn(modPHScal::yr) * modPHScal::sSprInfo[iSEQofSPR].SumDisp /(modPHScal::bE_FormulaPlusHalf?2:1);
		}
		else if(modPHScal::gCH1distPos == 0 )
		{
			//E�ֶθ�������Сλ��λ�õĸ߶�
			rsX.GetFieldValue(_T("fBmin"),vTmp1);
			if( modPHScal::yr < 0 )
			{
				//��λ��<0
				//sngDim = sngH1xmax + (Sgn(modPHScal::yr) * modPHScal::sSprInfo[iSEQofSPR].haz + vtof(vTmp1)) /(modPHScal::bE_FormulaPlusHalf?2:1);
				//�о�������������λ�Ƽ��㹫ʽ������������㹫ʽ��ͬ������bE_FormulaPlusHalfΪ�Σ���Ҫ��2
				//
				sngDim = sngH1xmax + (Sgn(modPHScal::yr) * (modPHScal::gbCalcCSPRHeight_BySelDisp?modPHScal::sSprInfo[iSEQofSPR].SumDisp:modPHScal::sSprInfo[iSEQofSPR].haz) + vtof(vTmp1)) /(modPHScal::bE_FormulaPlusHalf?2:2);
			}
			else
			{
				//��λ��>=0��������H1s=H1x+��λ��T
				sngDim = sngH1xmax + (Sgn(modPHScal::yr) * (modPHScal::gbCalcCSPRHeight_BySelDisp?modPHScal::sSprInfo[iSEQofSPR].SumDisp:modPHScal::sSprInfo[iSEQofSPR].haz) + vtof(vTmp1)) /(modPHScal::bE_FormulaPlusHalf?2:2) + (sPartID == _T("ZHB") ? -1 : 0) * modPHScal::sSprInfo[iSEQofSPR].haz;
			}
			//���������򻪶�ʯ�;�����װ�������ܳ��ֳܼܷ��Ľ��ͣ����Ϲ�ʽ�ɷ���,Ӧ�ð����й�ʽ
			// sngDim = sngH1xmax + Sgn(modPHScal::yr) * modPHScal::sSprInfo[iSEQofSPR].haz /(modPHScal::bE_FormulaPlusHalf?2:2);
		}
		else if(modPHScal::gCH1distPos == 2 )
		{
			//E�ֶθ��������λ��λ�õĸ߶�
			rsX.GetFieldValue(_T("fBmax"),vTmp1);
			rsX.GetFieldValue(_T("fBmin"),vTmp2);
			sngDim = sngH1xmax + (vtof(vTmp1) - vtof(vTmp2))/2;
			if( modPHScal::yr < 0 )
			{
				//��λ��<0
				sngDim = sngDim + (Sgn(modPHScal::yr) * (modPHScal::gbCalcCSPRHeight_BySelDisp?modPHScal::sSprInfo[iSEQofSPR].SumDisp:modPHScal::sSprInfo[iSEQofSPR].haz) + vtof(vTmp2)) /(modPHScal::bE_FormulaPlusHalf?2:2);
			}else
			{
				//��λ��>=0
				sngDim = sngDim + (Sgn(modPHScal::yr) * (modPHScal::gbCalcCSPRHeight_BySelDisp?modPHScal::sSprInfo[iSEQofSPR].SumDisp:modPHScal::sSprInfo[iSEQofSPR].haz) + vtof(vTmp2)) /(modPHScal::bE_FormulaPlusHalf?2:2) + (sPartID == _T("ZHB")? -1:0) * modPHScal::sSprInfo[iSEQofSPR].haz;
			}
			//���������򻪶�ʯ�;�����װ�������ܳ��ֳܼܷ��Ľ��ͣ����Ϲ�ʽ�ɷ���,Ӧ�ð����й�ʽ
			//sngDim = sngH1xmax + Sgn(modPHScal::yr) * modPHScal::sSprInfo[iSEQofSPR].haz /(modPHScal::bE_FormulaPlusHalf?2:0);
		}
		//���ݵĿ������й�ʽ
		//rsTZB.SetFieldValue(_T("AllowedMinLugExt")) = IIf(IsNull(rsX.Fields(_T("x"))), 0, rsX.Fields(_T("x")))
		//rsTZB.SetFieldValue(_T("AllowedMaxLugExt")) = IIf(IsNull(rsX.Fields(_T("y"))), 0, rsX.Fields(_T("y")))+rsTZB.SetFieldValue(_T("AllowedMinLugExt"))
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
			rsTmp.GetFieldValue(_T("size4"),vTmp1);
			sngDim = sngDim + vtof(vTmp1);
		}
		if(sPartID==_T("LHE") || sPartID==_T("LHE1") || sPartID==_T("PHE") || sPartID==_T("PHE1") || sPartID==_T("PHC") || sPartID==_T("LHC"))
		{
			//��ʽ�������ӣ�Ҫ���϶���߶�T��
			if( !modPHScal::gbCH1IncludeT )
			{
				rsDiaOfCSPRFiK.GetFieldValue(_T("T"),vTmp1);
				sngDim = sngDim +vtof(vTmp1);
			}
		}
		else
		{
		}
	}
	rsX1.Close();
	return sngDim;
}
