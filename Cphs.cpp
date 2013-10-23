// Cphs.cpp
//

#include "stdafx.h"
#include "Cphs.h" 
#include "modPHScal.h" 
#include "EDIBgbl.h"
#include "EDIBDB.h" 
#include "user.h"
#include "frmStatus.h"
#include "PhsStart.h"
#include "PhsData.h"//
#include <math.h>
#include "FrmPhsSamp.h"
#include "basDirectory.h"
#include "FrmTxsr.h"
#include "FrmPhsSamp.h"
#include "ModEncrypt.h"
#include "ModRegistry.h" 
#include "EDibacad.h"
#include "resource.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//using namespace std;

Cphs	* Cavphs;
int vRCount;
//�����в���ȡ������Ϊ���������в���ֱ��ʹ��ö���ͱ���
int Cphs::MODEoverwrite=-1;
int Cphs::AttachI=iAttachIncluded;

CString Cphs::GetBH(const CString& sBHFormat)
{
	int iPos;
	CString strRet;
	iPos = sBHFormat.Find(")");
	if(iPos > 0)
	{
		strRet = sBHFormat.Left(iPos + 1);	
	}
	else
	{
		strRet = sBHFormat;	
	}
	return strRet;
}

float Cphs::SATotalWeight()
{
	return this->mvarSATotalWeight;
}

bool Cphs::bFirstPartIsPA()
{
	CString sTmp;
	sTmp.Format(_T("%d"),iPA);
	CString temp=CString(_T("Index=")) + sTmp + _T(" AND trim(CustomID)=\'") + Trim(Cntb) + _T("\'");
	
	try
	{
		if(rsID->State != adOpenStatic)
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
			rsID->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			brsIDStatus=TRUE;
		}
		VARIANT vTmp;
		if(!rsID->Find((_bstr_t)temp, 0, adSearchForward, vTmp))
			return false;
		else
			return true;
	}
	catch(CException *e)
	{
		e->Delete();
		return false;
	}
}

long Cphs::SetPhsPATypeToListBox()
{
	//Ŀ��:���������������йܲ�����������Ӽ���ʽ�����б��ؼ�ResultObj,�Ա�ѡ��
	//Input:iSAPAConnect(iSA/iPA/iConnectPART)
	//Output:�������������йܲ�����������Ӽ���ʽ�����б��ؼ�ResultObj
	//����:�����������
	CString sSQL;
	long ret;
	_RecordsetPtr rs;
	HRESULT hr = S_OK;
	hr = rs.CreateInstance(__uuidof(Recordset));
	try
	{
		int	i;
		if(rsID->State != adOpenStatic)
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
//			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenForwardOnly, adLockReadOnly, adCmdText); 
			brsIDStatus=TRUE;
		}
		//Ĭ��Ϊ�ܲ�
		if (iSAPAConnect==-1) iSAPAConnect=iPA;
		//ֻ���г��ܲ������
		if ((iSAPAConnect!=iPA)&&(iSAPAConnect!=iSA))
		{
			ShowMessage(GetResStr(IDS_CannotSelectConnectAtThisStage));
			return -1;
			//throw;
		}
		else
		{			
			if (iSAPAConnect==iPA) 				
				//�ܲ�
				if((iChkCondition & iOnlyPAh)!=0)
					//����
					sSQL.Format(_T("SELECT DISTINCT CustomID FROM PictureClipData WHERE CustomID IN (SELECT DISTINCT CustomID FROM tmpCustomIDPA) AND Index=%d AND (ClassID=%d OR ClassID=%d OR ClassID=%d OR ClassID=%d OR ClassID=%d OR ClassID=%d ) "),
					iSAPAConnect,
					iPAh,
					iPAD4LA,
					iPAD4LQ,
					iPAD11LD,
					iPALX,
					iPADLR
					);
				else
					//֧��
					sSQL.Format(_T("SELECT DISTINCT CustomID FROM PictureClipData WHERE CustomID IN (SELECT DISTINCT CustomID FROM tmpCustomIDPA) AND Index=%d AND NOT (ClassID=%d OR ClassID=%d OR ClassID=%d OR ClassID=%d OR ClassID=%d OR ClassID=%d ) "),
					iSAPAConnect,
					iPAh,
					iPAD4LA,
					iPAD4LQ,
					iPAD11LD,
					iPALX,
					iPADLR
					);
				else
					//����,��ʱδ����
					;
		}
		if(sSQL==_T("")) return 0;
		if(rs->State == adStateOpen)
		{
			rs->Close();
		}
		rsID->Open((_bstr_t)sSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenForwardOnly, adLockReadOnly, adCmdText); 
		
        if (rs->BOF && rs->BOF)
		{
			ShowMessage(GetResStr(IDS_NoAnyRecordInPictureClipData));
			return 0;
		}
		else
		{
			try{ 
				COleVariant vTmp; 
				//COleVariant vTmp;
				ResultObj->ResetContent();
				i=0;
				RsDeleteAll(rsUnCheckedType);
				while(!rs->BOF)
				{
					i++;
					rs->get_Collect((_variant_t)_T("CustomID"), &vTmp);
					rsUnCheckedType->AddNew();
					rsUnCheckedType->put_Collect((_variant_t)_T("CustomID"),vTmp);//STR_VAR(vTmp));
					rsUnCheckedType->Update();
					rs->MoveNext();
				}
			}
			catch(CException *e)
			{
				e->Delete();
				return 0;
			}
			
			//			}
			ret= i;
			//�ѳ�����õĿ������д����ʱ��
			//			i=SaveRsUnCheckedTypeFromResultObj();
			return ret;
		}
	}
	catch(CException *e)
	{
		e->Delete();
		return (long)0;
	}
}

long Cphs::SetPhsTypeToListBox()
{
	//Ŀ��:����֧����������뵽�б��ؼ�ResultObj(��ʾ��ֵҪ->CustomID�ֶ�ֵ)
	//     ����ѡ������Ӽ���ʽCntb��CntbP(CustomIDֵ),ȷ����֮���������,
	//     �����ɹ�ѡ���(���˵������ϵ�ǰ��׼��)��ʽ�����б��ؼ�ResultObj,�Ա�ѡ��
	//Input:���Cntb
	//      CntbP(CntbP����Cntb��ǰһ�����,��Ϊ��)
	//Output:���п���Cntb(��CntbP)��������������뵽�б��ؼ�ResultObj(��ʾ��ֵҪ->CustomID�ֶ�ֵ)
	//����:�����������
	//ע��:connect����Cntb��Cnte��Cntn�ֶ�����ΪID,����CustomID;
	//     PhsStructureREF����CustomID�ֶ�����ΪCustomID.
	
	_RecordsetPtr rs;
	HRESULT hr = S_OK;
	hr = rs.CreateInstance(__uuidof(Recordset));
	CString FldName;
	CString tmp,sSQL;
	long ret;
	long i,mvClassID;
	
	try
	{
		//����3�����Ҫͬʱƥ��
		if(rsID->State != adOpenStatic)
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
//			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			brsIDStatus=TRUE;
		}
		if (Cntb==_T(""))
		{
			//�ܲ���Ϊ�գ�˵������ѡ��ܲ�
			return SetPhsPATypeToListBox();
		}
		//�ܲ�����Ϊ�գ�˵������ѡ��ܲ���ѡ�����Ӽ������
		if (CntbP==_T(""))
		{
			//����2���������ƥ��
			//sSQL=CString(_T("SELECT * FROM connect WHERE trim(cntb)=\'"))
			//	+modPHScal::sFindID(Trim(Cntb)+_T("\'"));
			sSQL=CString(_T(" trim(cntb)=\'"))+modPHScal::sFindID(Trim(Cntb))+_T("\'");
			//rsConnect.m_strFilter=sSQL;
			//rsConnect->Requery(adExecuteRecord);
			//rs.Open(dbOpenSnapshot,sSQL,0);
			FldName = _T("cnte");
		}
		else
		{
			sSQL=CString(_T(" trim(cntb)=\'"))
				+modPHScal::sFindID(Trim(CntbP))
				+_T("\' AND Trim(cnte)=\'")
				+modPHScal::sFindID(Trim(Cntb))
				+_T("\' AND Trim(Cntn)<>\'\'");

			VARIANT vTmp;
			if(!rsConnect->Find((_bstr_t)sSQL, 0, adSearchForward, vTmp))
			{            
			/*sSQL=CString(_T("SELECT * FROM connect WHERE trim(cntb)=\'"))
			+modPHScal::sFindID(Trim(Cntb))+_T("\'");
				rs.Open(dbOpenSnapshot,sSQL,0); */           
				sSQL=CString(_T(" trim(cntb)=\'"))
					+modPHScal::sFindID(Trim(Cntb))+_T("\'");
				//rsConnect.m_strFilter=sSQL;
				//rsConnect->Requery(adExecuteRecord);
				FldName = _T("cnte");
			}
			else
			{				
				FldName = _T("cntn");//����3���������ƥ��
			}
		}
		//�Ƿ���Ҫ����?
		_variant_t vTmp;
		if(!rsConnect->Find((_bstr_t)sSQL, 0, adSearchForward, vTmp))
		{
			//ShowMessage(GetResStr(IDS_NoCustomIDinConnect));
			return 0;
			//throw;
		}
		
		//��������m
		CString tbn;
		bool bAdd;
//		COleVariant vTmp;
		ResultObj->ResetContent();
		i=0;
		modPHScal::ZdjType=vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("TYPE")));
		do
		{
			i++;
			rsConnect->get_Collect((_variant_t)FldName, &vTmp);
			//CString str = rsConnect.GetSQL();
			tmp=vtos(vTmp);
			tmp.TrimLeft();tmp.TrimRight();tmp.MakeUpper();
			
			if(rsID->Find((_bstr_t)(_T("trim(ID)=\'")+tmp+_T("\'")), 0, adSearchForward, vTmp))
			{
				//�ҵ���Ӧ��ID
				rsID->get_Collect((_variant_t)_T("CustomID"), &vTmp);
				tmp=vtos(vTmp);
				tmp.TrimLeft();tmp.TrimRight();tmp.MakeUpper();
				i=ResultObj->FindStringExact(-1,tmp);
				//i=SendMessage(ResultObj->m_hWnd,LB_FINDSTRINGEXACT,0,(long)vTmp.lVal);
				//�ò����Ƿ�ɼ���_T("���ÿ�"),bAdd=true�ɼ���
				rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp);
				mvClassID=vtoi(vTmp);
				
				if (mvClassID==iCSPR)
				{
					//���������Ƿ����,
					if (modPHScal::ZdjType.GetLength()==4)
					{
						if (modPHScal::ZdjType==_T("9999") || modPHScal::ZdjType==_T("5000"))
							bAdd=true;
						else
							bAdd=false;
					}
					else
						bAdd=false;
				}
				else if (mvClassID==iSPR)
				{
					if (modPHScal::ZdjType.GetLength()==4)
					{
						if (modPHScal::ZdjType!=_T("9999") && modPHScal::ZdjType!=_T("5000")) 
							bAdd=true;
						else
							bAdd=false;
					}
					else
						bAdd=false;
				}
				else
				{
					//not spring and not Constant spring
					//��ʹû�ҵ�i=-1,��Ҫ�ж����Ƿ����ڵ�ǰ֧���ܹ淶
					CString SQLx;
					int iIndex;
					rsID->get_Collect((_variant_t)_T("Index"), &vTmp);
					iIndex=vtoi(vTmp);
					if(iIndex== iPA)
						SQLx.Format(_T("SELECT CustomID FROM tmpCustomIDPA WHERE CustomID=\'%s\'"),tmp);						
					else if(iIndex== iConnectPART)
						SQLx.Format(_T("SELECT CustomID FROM tmpCustomIDPART WHERE CustomID=\'%s\'"),tmp);						
					else if(iIndex== iSA)
						SQLx.Format(_T("SELECT CustomID FROM tmpCustomIDSA WHERE CustomID=\'%s\'"),tmp);
					if(rs->State == adStateOpen) rs->Close();
					rs->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
						adOpenDynamic, adLockReadOnly, adCmdText); 
					if(rs->BOF && rs->BOF)
						//���㲿���������ڵ�ǰ�淶,���ܼ����б��,��ֹ��ѡ
						bAdd=false;
					else
						bAdd=true;
				}
				
				if ((i==-1)&&bAdd) ResultObj->AddString(tmp);
			}//if block
		}
		while(rsConnect->NextRecordset(NULL));
		//�ѳ�����õĿ������д����ʱ��RsUnCheckedPart
		ret=i;
		i=SaveRsUnCheckedTypeFromResultObj();		  
   }//try block
   catch(CException *e)
   {
	   e->Delete();
   }
   return ret;
}

long Cphs::SetPhsPreviousTypeToListBox()
{
	//����ѡ������Ӽ���ʽCntb��CntbP,ȷ������ǰ����֮���������,
	//�����ɹ�ѡ�����ʽ�����б��ؼ�ResultObj,�Ա�ѡ��
	//Input:���Cntb,CntbP(CntbP����Cntb��ǰһ�����,��Ϊ��)
	//Output:���п���Cntb(��CntbP)������ǰһ����������뵽�б��ؼ�ResultObj
	//����:�����������
	return 0;
}

long Cphs::SaveRsUnCheckedTypeFromResultObj()
{
	int i;
	CString temp;
	try{
		RsDeleteAll(rsUnCheckedType);
		for(i=0;i<ResultObj->GetCount();i++)
		{
			ResultObj->GetText(i,temp);
			rsUnCheckedType->AddNew();
			temp.TrimLeft();temp.TrimRight();
			rsUnCheckedType->put_Collect((_variant_t)_T("CustomID"),STR_VAR(temp));
			rsUnCheckedType->Update();
		}
		return i;
		
	}
	catch(CException *e)
	{
		e->Delete();
		return 0;
	}
}

long Cphs::SetPhsCheckedTypeToListBox()
{
	//Ŀ�ģ����б�������õ����(CustomIDֵ)
	//����SetphsTypeToListBox����ѡ���֧����,ȷ��ȷʵ���ڵĿ���֮���������,
	//�����ɹ�ѡ�����ʽ�����б��ؼ�ResultObj,�Ա�ѡ��
	//Input:'SourcObj�б��
	//      'ResultObj�б��
	//      'rsUnCheckedType��ʱ��
	//'����:1.SourceObj�б��listindex����֮ǰ�������rsUnCheckedType�е�ĳ������ɹ���һ����ʱ֧�������(������󲿿����ǲ�������),����д�뵽rsTmpREF��ʱ��
	//     '2.����SavephsStructRsTmpREFToTZB,����ʱ��rsTmpREFд��mvarRsTZB
	//     '3.ɾ����ʱ��rsTmpREF
	//     '4.����GetphsBHandSizesTest,ȷ���Ƿ����ҵ��������������
	//     '5.����ɹ�,�򽫿�������ŵ���ʱ��rsTmpCheckedType
	//     '6.ת��1ѭ��ֱ��rsUnCheckedTypeβ
	//     '7.��rsTmpCheckedType���ݸ���ResultObj�б��,���о������������Ŀ������
	//'Output:������������뵽�б��ؼ�ResultObj
	//'����:�����������
	//'rsTmpCheckedType��rsTmpREF--��������ΪCustomID
	_RecordsetPtr rsTmpCheckedType;
	HRESULT hr = S_OK;
	hr = rsTmpCheckedType.CreateInstance(__uuidof(Recordset));

	CString temp;
	int j=0,m=0;
	long boolk=0 ;
	try{
		InitListRs();
		if ((rsUnCheckedType->adoEOF)&&(rsUnCheckedType->BOF))
		{
			::ShowMessage(GetResStr(IDS_NullRsUnCheckedTypeIndbPRJ));
			goto errH;
		}
		//throw;//Err.Raise iUE_NullRsUnCheckedTypeIndbPRJ, , ResolveResString(iUE_NullRsUnCheckedTypeIndbPRJ)
		else if (!rsUnCheckedType->adoEOF){
			rsUnCheckedType->MoveLast();
			rsUnCheckedType->MoveFirst();
			//'�����ʱ��rsTmpCheckedType,׼����ż��Ľ��
			if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ,_T("rsTmpCheckedType")))
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE * FROM rsTmpCheckedType"), NULL, adCmdText);
			else
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE rsTmpCheckedType (CustomID char(20))"), NULL, adCmdText);
			MakeRsTZB(idbPRJ, _T("rsTmp"));
			for(m=0;m<rsUnCheckedType->GetRecordCount();m++){
				//�����ʱ��trsTmpREF,׼�����ѡ�����Ͻṹ��
				if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ,_T("rsTmpREF")))
					EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE * FROM rsTmpREF"), NULL, adCmdText);
				else
					EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE rsTmpREF (CustomID char(20))"), NULL, adCmdText);
				if (iSAPAConnect!=iPA)//����������浽trsTmpREF��
					for(j=0;j<=SourceObj->GetCurSel();j++){
						SourceObj->GetText(j,temp);
						EDIBgbl::SQLx=CString(_T("INSERT INTO rsTmpREF (CustomID) VALUES (\'"))+Trim(temp)+_T("\')");
						EDIBgbl::dbPRJ->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
					}
					
					//��������һ��Ԫ�ر��浽trsTmpREF��
					COleVariant vTmp;
					rsUnCheckedType->get_Collect((_variant_t)_T("CustomID"), &vTmp);
					EDIBgbl::SQLx=CString(_T("INSERT INTO rsTmpREF (CustomID) VALUES (\'"))+vtos(vTmp)+_T("\')");
					EDIBgbl::dbPRJ->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
					
					//'��ʼ����2:
					//ÿ���޸�AllPrjdb.mdb��ZB�����ݱ�ṹ֮��Ī����ͬʱ�޸�F4511s.mdb��ZB��ṹ��workprj.mdb�е�rsTmpZB��Ľṹ��
					//MakeRsTZB(idbPRJ, _T("rsTmp"));
					//��ʼ����3:
					EDIBgbl::SQLx = _T("SELECT * FROM rsTmpREF");
					if(rsTmpREF->State == adOpenStatic)
						rsTmpREF->Close();
//					rsTmpREF.m_pDatabase=&EDIBgbl::dbPRJ;
//					rsTmpREF.Open(dbOpenDynaset,EDIBgbl::SQLx);
					rsTmpREF->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
						adOpenDynamic, adLockReadOnly, adCmdText); 
					//'��ʼ����4��5
					bool k=GetphsBHandSizesTest();
					if (k){
						rsUnCheckedType->get_Collect((_variant_t)_T("CustomID"), &vTmp);
						EDIBgbl::SQLx=_T("INSERT INTO rsTmpCheckedType (CustomID) VALUES ('")+vtos(vTmp)+_T("')");
						EDIBgbl::dbPRJ->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
					}
					rsUnCheckedType->MoveNext();
			}//for block
			//��ʼ����7:�������ResultObj�б��
			
			CString strSQL = _T("SELECT CustomID FROM rsTmpCheckedType");
			rsTmpCheckedType->Open((_bstr_t)strSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if ((rsTmpCheckedType->adoEOF)&&(rsTmpCheckedType->BOF));
			else if (!rsTmpCheckedType->adoEOF){
				rsTmpCheckedType->MoveLast();
				rsTmpCheckedType->MoveFirst();
			}
			//����б���Ա�����װ��
			ResultObj->ResetContent();
			COleVariant vTmp;
			for(j=0;j<rsTmpCheckedType->GetRecordCount();j++)
			{
				rsTmpCheckedType->get_Collect((_variant_t)_T("CustomID"), &vTmp);
				ResultObj->AddString(vtos(vTmp));
				rsTmpCheckedType->MoveNext();
			}
			return rsTmpCheckedType->GetRecordCount();
			//Set rsTmpCheckedType = Nothing
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
errH:
	return 0;
}

CString Cphs::GetPhsAssembleName(long /*Optional*/ SampleID)
{
	//ע�⣺�����̵��ٶȺ���Ҫ����Ϊ�ڳ�ʼ�������б�ѭ�����á�
	//Ŀ��:����֧��������������,ȷ������ͼ������
	//Input:
	//SampleID,
	//���ȱʡ���SourceObj����(ListBox)����ȡ֧������װ�������
	//����:֧������װ��������
	CString * Ptype=NULL;
	CString ret;
	long IC;
	long iNumCSPR , iNumSPR, iNumRod ;
	CString sPA, sSA, sSPR ;
	long i=0, j=0;
	CString strTmp,sTmp;
	CString SQLx;
	
	try
	{
//		rs1(&EDIBgbl::dbPHScode), rs(&EDIBgbl::dbPRJ);//20071101 "dbSORT" ��Ϊ "dbPHScode"
		_RecordsetPtr rs1;
		HRESULT hr = S_OK;
		hr = rs1.CreateInstance(__uuidof(Recordset));

		_RecordsetPtr rs;
		hr = rs.CreateInstance(__uuidof(Recordset));
		if(rsID->State != adOpenStatic)
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
//			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
//			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
			rsID->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			brsIDStatus=TRUE;
		}
		
		SQLx = _T("SELECT * FROM phsStructureREF WHERE SampleID=") + ltos(SampleID) + _T(" ORDER BY SEQ ");
//		rs1.Open(dbOpenDynaset,SQLx);
		rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		
		if(rs1->adoEOF && rs1->BOF)
		{
			sTmp.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),ltos(SampleID));
			throw sTmp;
		}
		rs1->MoveLast();
		IC = rs1->GetRecordCount();
		Ptype=new CString[IC];
		COleVariant vTmp;
		iNumSPR = 0;
		iNumCSPR = 0;
		iNumRod = 0;
		sSPR = _T(" ");
		for(i=IC-1;i>=0;i--)
		{
			//ע�͵��жϾ䣬�ɼӿ��ٶ�
			rs1->get_Collect((_variant_t)_T("ID"), &vTmp);
			Ptype[i] = vtos(vTmp);
			rs1->MovePrevious();
		}
//		rs.Open(dbOpenSnapshot,_T("SELECT * FROM PictureClipData"));
		SQLx = _T("SELECT * FROM PictureClipData");
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		rs1->MoveFirst();
		for( i = 0 ;i<IC;i++)
		{
			//�ӵ�һ����ʼ����ƥ��
			if(i == IC - 1)
				//����
				SQLx = _T("Trim(ID)=\'") + Ptype[i] + _T("\' AND index=0");
			else
				SQLx = _T("Trim(ID)=\'") + Ptype[i] + _T("\'");

			_variant_t vTmp;
			if(!rs->Find((_bstr_t)SQLx, 0, adSearchForward, vTmp))
			{
				sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("ID"),Ptype[i]);
				throw sTmp;
			}
			//��һ���ǹܲ�
			rs->get_Collect((_variant_t)_T("Description"), &vTmp);
			if( i == 0)
				ret=vtos(vTmp);
			else if( i == IC - 1)
			{
				if( iNumCSPR + iNumSPR == 0 )
				{
					if (ret.Find(modPHScal::gsPhsTypeRigid) == -1) 
						ret+=modPHScal::gsPhsTypeRigid + vtos(vTmp);
				}
				else if( iNumCSPR > 0 )
				{
					if(ret.Find(modPHScal::gsPhsTypeCSPR) == -1)
						ret+=modPHScal::gsPhsTypeCSPR +vtos(vTmp);
				}
				else if( iNumSPR > 0)
				{
					if(ret.Find(modPHScal::gsPhsTypeSPR) ==-1)
						ret+=modPHScal::gsPhsTypeSPR +vtos(vTmp);
				}
			}
			else
			{
				rs->get_Collect((_variant_t)_T("ClassID"), &vTmp);
				if(vtoi(vTmp)==iCSPR)
				{
					iNumCSPR++;
					sSPR = Ptype[i];
				}
				else if(vtoi(vTmp)== iSPR )
				{
					iNumSPR ++;
					sSPR = Ptype[i];
				}
				else if(vtoi(vTmp)== iROD)
					iNumRod ++;
			}
			//��CustomID��ʼ��,��ֹ����ѡ��Ĺ淶ì�ܡ�
			//ʹ���¶γ���ӿ��ٶȡ�
			rs->get_Collect((_variant_t)_T("ID"), &vTmp);
			if(rsID->Find((_bstr_t)(_T("trim(ID)=\'") + vtos(vTmp)+_T("\'")), 0, adSearchForward, vTmp))
			{
				rsID->get_Collect((_variant_t)_T("CustomID"), &vTmp);
				strTmp = vtos(vTmp);
//				rs1.Edit();
				rs1->put_Collect((_variant_t)_T("CustomID"),STR_VAR(strTmp));
				rs1->Update();
			}
			rs1->MoveNext();
		}
		rs1->Close();
		
		//��ʼ����������
		//������ģ�������е�ʹ��Ƶ����Ϣ
		
		SQLx = _T("SELECT * FROM phsStructureName WHERE SampleID=") + ltos(SampleID);
//		rs1.Open(dbOpenDynaset,SQLx);
		rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		sTmp.Format(_T("%05d"),SampleID);
/*		//�����ʹ��������������Ϊ��
		rs1->get_Collect((_variant_t)_T("SampleName"), &fi);
		strTmp=ret;
		//�˴�Ҫע���ַ������ֽ���
		int n=ret.GetLength();
		CString ss=ret.Left(2);
		if(n > fi.m_lSize)
			strTmp=ret.Left(fi.m_lSize);
*/
		if(rs1->adoEOF && rs1->BOF)
		{
			//����Ŀ��ؿ���ʹ��SQL�����ݼ�¼������֮���л���
			//ʹ��SQLʱ������ģ���������ǡ���֧���Ե���������ʹ�ö����򲻻���������⣬��˲�����ΪSQL���ܼӿ��ٶȶ�����ʹ��rs1��
			SQLx.Format(_T("INSERT INTO phsStructureName (SampleID,BmpName,SampleName,PA,SA,SPR,iNumOfPart,iNumSPR,iNumCSPR,iNumRod,FreQuence,bAllowUse) ")
				_T("VALUES (%s,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%s,%s,%s,%s,1,-1)"),
				ltos(SampleID),sTmp,strTmp,Ptype[0],Ptype[IC - 1],sSPR,ltos(IC), ltos(iNumSPR),ltos(iNumCSPR),ltos(iNumRod));
			EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071101 "dbSORT" ��Ϊ "dbPHScode"
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		}
		else
		{
			SQLx.Format(_T("UPDATE phsStructureName SET BmpName=\'%s\',SampleName=\'%s\',PA=\'%s\',SA=\'%s\',SPR=\'%s\',iNumOfPart=%s,iNumSPR=%s,iNumCSPR=%s,iNumRod=%s,bAllowUse=-1 WHERE SampleID=%s"),
				sTmp,strTmp,Ptype[0],Ptype[IC - 1],sSPR,ltos(IC), ltos(iNumSPR),ltos(iNumCSPR),ltos(iNumRod),ltos(SampleID));
			EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071101 "dbSORT" ��Ϊ "dbPHScode"
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		}
		delete [] Ptype;
	}
	catch(CException *e)
	{
		e->Delete();
		if(Ptype!=NULL)
			delete [] Ptype;
	}
	return ret;
}

long Cphs::GetPhsOneClassPartNum(long iPtype, long /*ByVal*/ SampleID)
{
	return 0;
}

long Cphs::GetPhsOneClassPartNumAndPartInfo(long iPtype, long /*ByVal*/ SampleID, CString * & Ptype, long * & mlPartClassID, long * &mlPartIndex,long & Count)
{
	//Ŀ��:����֧��������������ָ������ϽṹID,ȷ�����е�������,������������ź�������
	//Input:SampleID,���ȱʡ���Ĭ��Ϊ1����ȡ֧������װ�������
	//Output:long,�ɳ�����iPtype�涨���������;���iPtype=iSPR or iCSPR,�򵯻ɵ�CustomID���ص�sSprInfo()�����У��±���0���ϱ���GetPhsOneClassPartNumAndPartInfo - 1��
	//�˺�����GetPhsOneClassPartNumAndPartType����ֻ�м�����ֵ��䲻ͬ
	//�����±�Ϊ0��
	   long ret=0;
	   try
	   {
//		   rs1(&EDIBgbl::dbPHScode), rs(&EDIBgbl::dbPRJ);//20071019 "dbSORT" ��Ϊ "dbPHScode"
		   _RecordsetPtr rs1;
		   HRESULT hr = S_OK;
		   hr = rs1.CreateInstance(__uuidof(Recordset));

		   _RecordsetPtr rs;
		   hr = rs.CreateInstance(__uuidof(Recordset));

		   long  i =0, iNumTmp=0;
		   CString tmpStr;
		   CString SQLx = _T("SELECT * FROM PictureClipData");
//		   rs.Open(dbOpenDynaset,SQLx );
		   rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			   adOpenDynamic, adLockReadOnly, adCmdText); 
		   if( rs->adoEOF && rs->BOF)
		   {
			   throw GetResStr(IDS_NoAnyRecordInPictureClipData);
		   }
		   SQLx = _T("SELECT * FROM phsStructureREF WHERE SampleID=") + ltos(SampleID) + _T(" ORDER BY SEQ ");
//		   rs1.Open(dbOpenDynaset,SQLx );
		   rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			   adOpenDynamic, adLockReadOnly, adCmdText); 
		   if( rs1->BOF && rs1->adoEOF)
		   {
			   tmpStr.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),ltos(SampleID));
			   throw tmpStr;
		   }
		   COleVariant vTmp;
		   rs1->MoveLast();
		   rs1->MoveFirst();
		   int C=rs1->GetRecordCount();
		   if(Ptype!=NULL)
			   delete [] Ptype;
		   Ptype=new CString [C];
		   if(mlPartClassID!=NULL)
			   delete [] mlPartClassID;
		   mlPartClassID=new long [C];
		   if(mlPartIndex!=NULL)
			   delete [] mlPartIndex;
		   mlPartIndex=new long[C];
		   Count=C;
		   for(i=0;i<C;i++)
		   {
			   rs1->get_Collect((_variant_t)_T("ID"), &vTmp);
			   if(vTmp.vt!=VT_NULL)
			   {
				   Ptype[i]=vtos(vTmp);
				   
				   if(rs->Find((_bstr_t)(_T("Trim(ID)=\'") + Ptype[i] + _T("\'")), 0, adSearchForward, vTmp))
				   {
					   rs->get_Collect((_variant_t)_T("ClassID"), &vTmp);
					   mlPartClassID[i]=vtoi(vTmp);
					   rs->get_Collect((_variant_t)_T("Index"), &vTmp);
					   mlPartIndex[i]=vtoi(vTmp);
					   if(mlPartClassID[i]== iPtype)
					   {
						   if(iPtype == iSPR)
						   {
							   rs->get_Collect((_variant_t)_T("CustomID"), &vTmp);
							   modPHScal::sSprInfo[iNumTmp].CustomID = vtos(vTmp);
							   iNumTmp ++;
						   }
						   ret++;
					   }
				   }
			   }
			   rs1->MoveNext();
		   }
	   }
	   catch(CString e)
	   {
		   ShowMessage(e);
	   }
	   catch(CException *e)
	   {
		   e->Delete();
	   }
	   return ret;
}

long Cphs::GetPhsIsCSPR(long iPtype, long SampleID, int& RCount)
{
	//Ŀ��:����֧��������������ָ������ϽṹID,ȷ�������Ƿ���ĳ�����,�������������
	//Input:SampleID,���ȱʡ���Ĭ��Ϊ1����ȡ֧������װ�������
	//Output:long,�ɳ�����iPtype�涨���������;���iPtype=iSPR or iCSPR,�򵯻ɵ�CustomID���ص�modPHScal::sSprInfo()�����У��±���0���ϱ���GetPhsIsCSPR - 1��
	//����:RCount,��¼��
	//�˺�����GetPhsOneClassPartNumAndPartType����ֻ�м�����ֵ��䲻ͬ
	//�����±�Ϊ0��
	_RecordsetPtr rs1;
	HRESULT hr = S_OK;
	hr = rs1.CreateInstance(__uuidof(Recordset));
	long iNumTmp=0;
	CString SQLx;
	COleVariant v;
	long ret=0;
	try
	{
//		rs1.m_pDatabase=&EDIBgbl::dbPHScode;//20071018 "dbSORT" ��Ϊ "dbPHScode"
		CString sTmp;
		char sID[64];
		ltoa(SampleID,sID,10);
		if(rsID->State != adOpenStatic)
		{
			SQLx=_T("SELECT * FROM PictureClipData");
//			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		else
			rsID->Requery(adExecuteRecord);
		brsIDStatus=TRUE;
		if( rsID->adoEOF && rsID->BOF )
		{
			ShowMessage(GetResStr(IDS_NoAnyRecordInPictureClipData));
			rsID->Close();
			brsIDStatus=FALSE;
		}
		else
		{
			SQLx = _T("SELECT * FROM phsStructureREF WHERE SampleID=");
			sTmp= sID;
			SQLx=SQLx + sTmp;
			sTmp=_T(" ORDER BY SEQ ");
			SQLx=SQLx + sTmp;
//			rs1.Open(dbOpenDynaset,SQLx);
			rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if( rs1->BOF && rs1->adoEOF )
			{
				sTmp.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),sID);
				ShowMessage(sTmp,MB_TOPMOST|MB_OK);
			}
			else
			{
				while(!rs1->adoEOF)
				{
					rs1->get_Collect((_variant_t)_T("ID"),v);
					if( v.vt!=VT_NULL)
					{
						_variant_t vTmp;
						CString strSql = (_T("Trim(ID)=\'") + vtos(v) + _T("\'"));
						if( rsID->Find((_bstr_t)strSql, 0, adSearchForward, vTmp))
						{
							rsID->get_Collect((_variant_t)_T("ClassID"),v);
							if( vtoi(v) == iPtype )
							{
								if( iPtype == iSPR )
								{
									rsID->get_Collect((_variant_t)_T("CustomID"),v);
									modPHScal::sSprInfo[iNumTmp].CustomID = vtos(v);
									iNumTmp = iNumTmp + 1;
								}
								ret++;
							}
						}
					}
					rs1->MoveNext();
				}
				RCount = rs1->GetRecordCount();
			}
		}
	}
	catch(CException *e)
	{
		e->Delete();
		RCount=rs1->GetRecordCount();
	}
	rs1->Close();
	return ret;
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo

long Cphs::GetphsStructIDsemiAuto()
{
	//���Զ�ѡ��֧���ܽṹ
	//���ݹܲ�mvarPA������mvarSA�͵��ɻ�������ɵ�����giWholeSprNum��
	//��phsStructureName���Զ�ѡ��ʹ��Ƶ�����������ٵ���ϣ�
	//���ܻ������֧������װ����.
	//input:
	//mvarPA--����PA,�ܲ�,��Ϊ��
	//mvarSA--����SA,����,��Ϊ��
	//output:
	//GetphsStructIDsemiAuto=SampleID��-1(ʧ��)
	long ret=-1;
	try
	{
		//long IC , iSampID , iNum ;
		//CString tmpSQLPA , tmpSQLSA , tmpSQLSpr;
		_RecordsetPtr rs1;
		HRESULT hr = S_OK;
		hr = rs1.CreateInstance(__uuidof(Recordset));
		_Recordset* rsza=FrmTxsr.m_pViewTxsr->m_ActiveRs;
		//�ٶ�֧������������100�����
		CString strSprNum;
		strSprNum.Format(_T("%d"),modPHScal::giWholeSprNum);
		CString SQL1;
		CString SQLx;		
		CString strPA,strSA;
		CString strPAexists,strSAexists;
		CString strFilter,strSAFilter;
		//CString strOrderBy=_T(" ORDER BY FREQUENCE DESC,iNumOfPart");//�����ֶ�
		
		SQL1.Format((modPHScal::ZdjType == _T("9999") || modPHScal::ZdjType == _T("5000") ? _T("iNumCSPR=%d") : _T("iNumSPR=%d")),modPHScal::giWholeSprNum);
		PA = Trim(PA);
		SA = Trim(SA);
		CString sTmp,sTmp2;
		COleVariant vTmp;
		//�������ƹܲ�����Ϊָ�����ͣ�ѡ���������ģ��
		//��
		//ѡ��һ��ģ��,exit
		//û��
		//���ƹܲ����ͣ�
		//��
		//ѡ��һ��ģ��,exit
		//û��
		//���Ƹ������ͣ�
		//��
								//ѡ��һ��ģ��,exit
		//û��
								//�����ƹܲ�/�������ͣ�
		//��
		//ѡ��һ��ģ��,exit
		//û��
		//������û�к�������ģ��
		
		sTmp=vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("Unit_Force")));
		sTmp.MakeUpper();
		modPHScal::pjg=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("pjg1")));
		if(sTmp==_T("N"))
			modPHScal::pjg /= EDIBgbl::kgf2N;
		float tmppjg=fabs(modPHScal::pjg);
		modPHScal::t0=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("T01")));
		modPHScal::dj=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dj1")));
		
		strPA.Format(_T(" (PA=\'%s\') "),PA);
		strPAexists = _T("( PA IN (Select ID FROM PictureClipData ")
			_T(" WHERE CustomID IN ( Select CustomID FROM [") + modPHScal::tbnPA + _T("] IN \"\" [\; DATABASE=") 
			+ (LPTSTR)(LPCTSTR)modPHScal::dbZDJcrude->DefaultDatabase + _T(" ;PWD=") + ModEncrypt::gstrDBZdjCrudePassWord + _T("]WHERE Pmax >=") 
			+ ftos(tmppjg) + _T(" AND (Dw >= ") + ftos(modPHScal::dj* (1 - modPHScal::gnDW_delta * 0.01))
			+ _T(" AND Dw <= ") + ftos(modPHScal::dj* (1 + modPHScal::gnDW_delta * 0.01)) 
			+ _T(" AND Dw>0 OR Dw Is NULL OR Dw=0) AND tj >= ") + ftos(modPHScal::t0) + _T("))) ");
		
		strSA.Format(_T(" (SA=\'%s\') "),SA);
		int Gnum;			
		Gnum= (modPHScal::glNumSA!=0 ? modPHScal::glNumSA : 1);
		strSAexists = _T(" ( SA IN ( Select ID FROM PictureClipData ")
			_T(" WHERE EXISTS ( Select CustomID FROM [") + modPHScal::tbnSA + _T("] IN \"\" [; DATABASE=") 
			+ (LPTSTR)(LPCTSTR)modPHScal::dbZDJcrude->DefaultDatabase + _T(" ;PWD=") + ModEncrypt::gstrDBZdjCrudePassWord + _T("] WHERE PictureClipData.CustomID = CustomID AND ((PictureClipData.ClassID= ") + ltos(iG100) + _T(") OR PMAXH >=") 
			+ ftos(tmppjg / Gnum * (vtob(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("ifLongVertPipe"))) ? Gnum : 1))
			+_T(" AND ((( PictureClipData.ClassID = ")
			+ ltos(iSACantilever) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbrace) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbraceFixG47) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbraceFixG48) + _T(" OR PictureClipData.ClassID = ") + ltos(iG51) + _T(" OR PictureClipData.ClassID = ") + ltos(iG56) + _T(" OR PictureClipData.ClassID = ") + ltos(iG57)  + _T(") AND GDW1 >=")
			+ ftos(modPHScal::gdw1) + _T(") OR (( PictureClipData.ClassID =") + ltos(iSAbeam) +_T(" OR PictureClipData.ClassID =") + ltos(iG52_55) + _T(" ) AND GDW1 >=")
			+ _T(" IIF(")+ ftos(modPHScal::gdw1+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0)/2) +_T(" > L1/2 , L1 - ") + ftos(modPHScal::gdw1+(modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2)
			+ _T(" , ")+ ftos(modPHScal::gdw1+(modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2) + _T(") AND L1> ") + ftos(modPHScal::Lspan)
			+_T(" ) OR (PictureClipData.ClassID= ") + ltos(iGCement) + _T(")))))) ");
		
		SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
			SQL1 + _T(" AND ") + strPA + _T(" AND ") + strPAexists + _T(" AND ") + strSA + _T(" AND ") + strSAexists );
//		rs1.Open(dbOpenSnapshot,SQLx);
		rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( !(rs1->adoEOF && rs1->BOF) )
		{
			//���ƹܲ�,���ƹܲ�������ڣ����Ƹ���/����������ڣ���
		}
		else
		{
			//���ƹܲ�,���ƹܲ�������ڣ����Ƹ���/����������ڣ�û��
			SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
				SQL1 + _T(" AND ") + strPAexists + _T(" AND ")  + strSA + _T(" AND ") + strSAexists );
			if(rs1->State == adOpenStatic) rs1->Close();
//			rs1.Open(dbOpenSnapshot,SQLx);
			rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if( !(rs1->adoEOF && rs1->BOF) )
			{
				//�����ƹܲ�,���ƹܲ�������ڣ����Ƹ���/����������ڣ���
			}
			else
			{
				//�����ƹܲ�,���ƹܲ�������ڣ����Ƹ���/����������ڣ�û��
				SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
					SQL1 + _T(" AND ") + strSA + _T(" AND ") + strSAexists );
				if(rs1->State == adOpenStatic) rs1->Close();
				rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( !(rs1->adoEOF && rs1->BOF) )
				{
					//�����ƹܲ�,�����ƹܲ�������ڣ����Ƹ���/����������ڣ���
				}
				else
				{
					//�����ƹܲ�,�����ƹܲ�������ڣ����Ƹ���/����������ڣ�û��
					SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
						SQL1 + _T(" AND ") + strSAexists );
					if(rs1->State == adOpenStatic) rs1->Close();
					rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
						adOpenDynamic, adLockReadOnly, adCmdText); 
					if( !(rs1->adoEOF && rs1->BOF) )
					{
						//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ��������Ƹ���������ڣ���
					}
					else
					{
						//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ��������Ƹ���������ڣ�û��
						SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
							SQL1 );
						if(rs1->State == adOpenStatic) rs1->Close();
						rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
							adOpenDynamic, adLockReadOnly, adCmdText); 
						if( !(rs1->adoEOF && rs1->BOF) )
						{
							//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ����������Ƹ���������ڣ���
						}
						else
						{
							//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ����������Ƹ���������ڣ�û��
							SQL1=(modPHScal::ZdjType == _T("9999") || modPHScal::ZdjType == _T("5000") ? _T("iNumCSPR>=1") : _T("iNumSPR>=1"));
							SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
								SQL1 );
							if(rs1->State == adOpenStatic) rs1->Close();
							rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
								adOpenDynamic, adLockReadOnly, adCmdText); 
							if( !(rs1->adoEOF && rs1->BOF) )
							{
								//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ����������Ƹ���������ڣ������Ƶ��Լ���������
							}
							else
							{
								//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ����������Ƹ���������ڣ������Ƶ��Լ�������û��
								sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDSprNum),_T(">=1"));
								throw sTmp;
							}
						}
					}
				}
			}
		}
		
		if(rs1->State != adOpenStatic || rs1->adoEOF||rs1->BOF)
			return -1;
		rs1->MoveFirst();
		rs1->get_Collect((_variant_t)_T("SampleID"), &vTmp);
		ret=vtoi(vTmp);
		rs1->get_Collect((_variant_t)_T("PA"), &vTmp);
		PA = vtos(vTmp);
		rs1->get_Collect((_variant_t)_T("SA"), &vTmp);
		SA = vtos(vTmp);

		rsza->PutCollect(_T("dn1"),_variant_t(modPHScal::sFindCustomID (PA)));
		rsza->PutCollect(_T("gn1"),_variant_t(modPHScal::sFindCustomID (SA)));
		rsza->PutCollect(_T("iSelSampleID"),_variant_t((long)ret));
		return ret;
	   }
	   catch(CString e)
	   {
		   ShowMessage(e);
		   return -1;
	   }
	   catch(CException *e)
	   {
		   e->Delete();
		   return -1;
	   }
	   
}

long Cphs::GetPhsStructSampleID()
{
	//Ŀ��:�Զ�ѡ��֧���ܽṹģ���
	//1:���ȴ�ԭʼ���ݱ�rsZA���ֶ�iSelSampleID��ȡ������ͼ��
	//2:����ֶ�iSelSampleID=null,����ݹܲ�dn������gn�͵��ɻ�������ɵ�������
	//��phsStructureREF���Զ�����ʹ��Ƶ��������ϣ�
	//���ܻ������֧������װ����.
	//3:����ֶ�iSelSampleID=null,����û���ṩ�ܲ�dn������gn�͵��ɻ�������ɵ�������
	//��ѡ��ʹ��Ƶ������֧�������
	//4:�������3�����ɹ����򷵻�-1
	//input:
	//iSelSampleID--ѡ���֧����ģ���
	//output:
	//if it//s successiful, return SampleID,otherwise -1
	int ret=-1;
	try
	{
		//MsgBox ResolveResString(iUE_NotFoundSampleIDSprNum, _T("|1"), iSelSampleID, _T("|2"), giWholeSprNum)
		ret = GetphsStructIDsemiAuto();
	}
	catch(CException *e)
	{
		e->Delete();
	}
	return ret;
}

long Cphs::SavephsStructTorsTmpREF()
{
	//����֧���ܺź���SampleID��ָ����֧������Ͻṹ,
	//����֧������Ͻṹ��rsTmpREF��,
	//Input:iSelSampleID,zdjh
	//iAttachIncluded=iAttachIncluded/iNotAttachInclude(��������/����������)
	//Output:long,�ɹ�=-1;����=������
	try{
		_RecordsetPtr rs;
		HRESULT hr = S_OK;
		hr = rs.CreateInstance(__uuidof(Recordset));
		//	int i;
		CString sTmp,sTmp2,SQLx;
		sTmp.Format(_T("%d"),modPHScal::iSelSampleID);
		SQLx = _T("SELECT *  FROM phsStructureREF WHERE SampleID=") + sTmp + _T(" ORDER BY seq");
//		rs.Open(dbOpenSnapshot,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if(rs->BOF && rs->adoEOF)
		{
			sTmp2.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),sTmp);
			ShowMessage(sTmp2);
			return IDS_NoSampleIDinphsStructureREF;
		}
		else
		{
			rs->MoveLast();
			rs->MoveFirst();
			if(rsTmpREF->State == adOpenStatic)
				rsTmpREF->Close();
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("rsTmpREF")))
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE * FROM rsTmpREF"), NULL, adCmdText);
			else
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE rsTmpREF (CustomID char (20))"), NULL, adCmdText);
			COleVariant vTmp;
			while(!rs->adoEOF)
			{
				rs->get_Collect((_variant_t)_T("CustomID"), &vTmp);
				SQLx = _T("INSERT INTO rsTmpREF (CustomID) VALUES (\'") + vtos(vTmp) + _T("\')");
				EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
				rs->MoveNext();
			}
			
			SQLx = _T("SELECT CustomID FROM rsTmpREF");
			
//			this->rsTmpREF.m_pDatabase=&EDIBgbl::dbPRJ;
//			rsTmpREF.Open(dbOpenDynaset,SQLx);
			rsTmpREF->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			return -1;
		}
	}
	catch(CException *e)
	{
		e->Delete();
		return 0;
	}
}

long Cphs::SavephsAllStructToTZB(long& zdjh, int nth, int& /*Optional*/ AttachIncluded)
{
	//����֧������Ͻṹ��JZB��,ͬʱ���ݿ�ѡ����AttachIncluded�����Ƿ�
	//����֧������Ͻṹ�ĸ�������(��ĸ/��Ƭ��)��JZF��
	//����֧���ܺź�SampleID��ָ����֧������Ͻṹ,�Լ���connect����ҵ���������ظ���д������
	//Input:
	//SampleID
	//zdjh
	//AttachIncluded=iAttachIncluded/iNotAttachInclude(��������/����������)
	//iMODEoverwrite=-1(��д)/0(����д)
	//Output:long,�ɹ�=-1;����=������
	CString sTmp,sTmp2;
	int j,k;
	int ret=0;
	
	_Recordset* rsza=::FrmTxsr.m_pViewTxsr->m_ActiveRs;
	_RecordsetPtr rsTmpZB;
	HRESULT hr = S_OK;
	hr = rsTmpZB.CreateInstance(__uuidof(Recordset));
	CString SQLx;
	_variant_t vTmp;

	CString * SampleArray = NULL;
	try
	{
		//��ʼ�����
		k = 1;
		//ɾ�����������ֶ�zdjhֵ=zdjh�ļ�¼
		//modPHScal::MakeZDJ_ZBZG();
		_RecordsetPtr rsX;
		hr = rsX.CreateInstance(__uuidof(Recordset));
		//���ӱ��������IDֵ
		if(rsConnect->State != adOpenStatic)
		{
			SQLx = _T("SELECT *  FROM connect");
//			rsConnect.m_pDatabase=&EDIBgbl::dbPRJ;
//			rsConnect.Open(dbOpenDynaset,SQLx);
			rsConnect->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		//ģ���������ID��CustomIDֵ
		
		sTmp.Format(_T("%d"),modPHScal::iSelSampleID);
		SQLx = CString(_T("SELECT *  FROM phsStructureREF WHERE SampleID=")) + sTmp + _T(" ORDER BY seq");
//		rsX.m_pDatabase=&EDIBgbl::dbPHScode;//20071018 "dbSORT" ��Ϊ "dbPHScode"
		rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if(rsX->adoEOF && rsX->BOF)
		{
			sTmp2.Format(_T("%d"),modPHScal::iSelSampleID);
			sTmp.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),sTmp2);
			ShowMessage(sTmp);
			ret = IDS_NoSampleIDinphsStructureREF;
		
			return ret;
		}
		else
			rsX->MoveLast();
		long iCount = rsX->GetRecordCount();
		COleVariant rsXv;

		SampleArray = new CString[iCount];

		int i;
		for(i =iCount-1;i >= 0 ;i--)
		{
			rsX->get_Collect((_variant_t)"ID",rsXv);
			SampleArray[i] = vtos(rsXv);
			rsX->MovePrevious();
		}

		
		SQLx = CString(_T("SELECT *  FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") +ltos(zdjh) + _T(" AND VolumeID=")+ ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY RECNO");
		rsTmpZB->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		//������
		
		
		CString sTmp2;
		COleVariant v,v1,v2,mvClassID,mvCustomID;
		_variant_t vTmp;
		
		for( i = 0 ;i<= iCount-1;i++)// - 1
		{
			mvClassID = modPHScal::sFindFLD(_T("ID"), _T("ClassID"), SampleArray[i]);
			mvCustomID = modPHScal::sFindFLD(_T("ID"), _T("CustomID"), SampleArray[i]);
			//����֧����ԭʼ���ݱ�JZA�Ĺܲ��͸���
			if (i == 0 )
				rsza->PutCollect(_T("dn1"),mvCustomID);
			else if( i == rsX->GetRecordCount() - 1)
				rsza->PutCollect(_T("gn1"),mvCustomID);
			_variant_t tv;
			rsTmpZB->AddNew();
			rsTmpZB->put_Collect((_variant_t)_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
			tv.ChangeType(VT_I4);
			tv.intVal=zdjh;
			rsTmpZB->put_Collect((_variant_t)_T("zdjh"),tv);
			tv.ChangeType(VT_I2);
			tv.iVal=k;
			rsTmpZB->put_Collect((_variant_t)_T("recno"),tv);
			tv.iVal = nth;
			rsTmpZB->put_Collect((_variant_t)_T("nth"),tv);

			rsTmpZB->put_Collect((_variant_t)_T("index"),variant_t((short)vtoi(modPHScal::sFindFLD(_T("ID"), _T("Index"),SampleArray[i]))));
			
			//���Ǹ���
			tv.iVal=(short)vtoi(mvClassID);
			rsTmpZB->put_Collect((_variant_t)_T("ClassID"),tv);
			rsTmpZB->put_Collect((_variant_t)_T("ID"),STR_VAR(SampleArray[i]));
			rsTmpZB->put_Collect((_variant_t)_T("CustomID"),STR_VAR(modPHScal::sFindCustomID(SampleArray[i])));
			rsTmpZB->Update();
			if (i< iCount-1)// Then
			{
				//ȡ����һ�εĿ�ͷ���
				CString strSql = (_T("trim(cntb)=\'") + (SampleArray[i]) + _T("\' AND trim(cnte)=\'") + (SampleArray[i+1]) + _T("\'"));
				_variant_t vTmp;
				if(!rsConnect->Find((_bstr_t)strSql, 0, adSearchForward, vTmp))
				{
					sTmp.Format(GetResStr(IDS_PartCanNotMatchOrNotExistInConnect),SampleArray[i],SampleArray[i+1]);
					ShowMessage(sTmp);
					return ret;
				}
				else
				{
					//��������
					if (AttachIncluded == iAttachIncluded)
					{
						//���8������,eg. F1/F2/...,etc.
						for( j = 1 ;j<= 8;j++)
						{
							sTmp.Format(_T("CntbF%d"),j);
							rsConnect->get_Collect((_variant_t) sTmp,v1);
							if (vtos(v1) != _T("") )
							{
								k = k + 1;
								rsTmpZB->AddNew();
								rsTmpZB->put_Collect((_variant_t)_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
								rsTmpZB->put_Collect((_variant_t)_T("zdjh"),_variant_t((long)zdjh));
								rsTmpZB->put_Collect((_variant_t)_T("recno"),_variant_t((short)k));
								rsTmpZB->put_Collect((_variant_t)_T("nth"),_variant_t((short)nth));

								//�Ǹ���
								rsTmpZB->put_Collect((_variant_t)_T("ClassID"),_variant_t((short)vtoi(modPHScal::sFindFLD(_T("ID"), _T("ClassID"),vtos(v1)))));
								//����sFindCustomID����ȡ������ֵ
								rsConnect->get_Collect((_variant_t)(_T("CntbF")+ltos(j)+_T("num")), &v2);
								rsTmpZB->put_Collect((_variant_t)_T("CLnum"),v2);

								rsTmpZB->put_Collect((_variant_t)_T("ID"),v1);
								rsTmpZB->put_Collect((_variant_t)_T("CustomID"),STR_VAR(modPHScal::sFindCustomID(vtos(v1))));
								rsTmpZB->Update();
							}
						}
					}
				}
			}
			k = k + 1;
		}
		ret = -1;
		rsTmpZB->Close();
	}
	catch(_com_error )
	{
	}
	
	if( SampleArray != NULL )
		delete [] SampleArray;

	return ret;
}

void Cphs::GetPhsStructFromTZB(int zdjh)
{
	//��ȡ֧���ܽṹfrom ZB�� to mvarResultObj(ListBox)
	//Input:zdjh,
	//Output:֧������װ�������(CustomIDֵ)��䵽SourceObj����(ListBox)
	try{
		if(TZBResultObj==NULL || !IsWindow(TZBResultObj->m_hWnd))
			return; 

		_RecordsetPtr rs;
		HRESULT hr = S_OK;
		hr = rs.CreateInstance(__uuidof(Recordset));
		TZBResultObj->ResetContent();
		
		CString SQLx;
		SQLx.Format(CString(_T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=%d AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 ORDER BY recno"),modPHScal::zdjh);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
//		rs.Open(dbOpenSnapshot,SQLx);
		if(rs->adoEOF && rs->BOF)
		{
			throw GetResStr(IDS_NoComponentForZDJHInTZB);
		}
		rs->MoveLast();
		COleVariant vTmp;
		rs->get_Collect((_variant_t)_T("CustomID"), &vTmp);
		modPHScal::UpdateTZB4zdjh(_T("gn1"), _variant_t(vtos(vTmp)));
		rs->MoveFirst();
		rs->get_Collect((_variant_t)_T("CustomID"), &vTmp);
		modPHScal::UpdateTZB4zdjh(_T("dn1"), _variant_t(vtos(vTmp)));
		while(!rs->adoEOF)
		{
			rs->get_Collect((_variant_t)_T("CustomID"), &vTmp);
			TZBResultObj->AddString(vtos(vTmp));
			rs->MoveNext();
		}
		rs->Close();
	}
	catch(CString )
	{
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void Cphs::SetPhsPASAtoTZA(int zdjh, long /*ByVal*/ SampleID)
{
}

void Cphs::MakeRsTZB(long /*ByVal*/ iDbID, CString /*ByVal*/ rsPrefixName,CString strDeleteUserMaterial)
{
	//����:ΪGetphsBHandSizes����׼��mvarrsTZB,mvarrsTZC,mvarrsTZF��¼��//
	//On Error GoTo errH
	// tbn As String
	CString tbn;
	//����ṹ���ݱ�
	bool tbE=false;
	CString SQLx;
	try{
		CString sTmp;
		sTmp.Format(_T("%d"),modPHScal::zdjh);
		
		if( iDbID == idbPRJDB )
		{
			SQLx = CString(_T("DELETE  FROM [")) +EDIBgbl::Btype[EDIBgbl::TZB]+ _T("] WHERE zdjh=") + sTmp + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + strDeleteUserMaterial;
			EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
		}
		else
		{
			SQLx = CString(_T("DELETE  FROM [rstmp")) +EDIBgbl::Btype[EDIBgbl::TZB]+ _T("] WHERE zdjh=") + sTmp + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		}
		if(rsTZB->State == adOpenStatic)
			rsTZB->Close();
		if( iDbID == idbPRJDB )
		{
			SQLx = CString(_T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + sTmp + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
// 			rsTZB.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rsTZB.Open(dbOpenDynaset,SQLx);
			rsTZB->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		else
		{
			SQLx = CString(_T("SELECT * FROM [rstmp")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + sTmp + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
// 			rsTZB.m_pDatabase=&EDIBgbl::dbPRJ;
// 			rsTZB.Open(dbOpenDynaset,SQLx);
			rsTZB->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
	}
	catch(_com_error & e)
	{
		ShowMessage(e.Description());
	}
}

bool Cphs::GetphsBHandSizesTest()
{
	//Ŀ��:��ԭʼ���ݱ��м���ÿ�����������������������
	//����:ZB,zdjh
	//����:���ӳɹ�,true;����False
	//On Error GoTo errH
	try{
		CString LogicalOpr = _T("=<>");   //�߼����������
		int iLOPnum = 3;          //�߼��������������
		CString msOpr,sTmp,sTmp2;
		//   long n=0,m=0;
		//   bool bFound;
		
		//����һ��������ڼ���̶�֧�ܵĵ�����sngSAfixP
		float sngSAfixP  =0; //�̶�֧�ܵĵ�����sngSAfixP
		float sngSAfixPl=0;  //��̬�̶�֧�ܵĵ�����sngSAfixPl
		float sngSAfixPr=0;  //��̬�̶�֧�ܵĵ�����sngSAfixPr*/
		//float PAfixH =0;//�̶�֧�ܹܲ��߶�H,cm
		float F1ToWx1 =0;   //������͸ֺ��йص��������ݣ�������zdjcrudeXBDLY�Ĳ۸����Ա�SSteelPropertyCS��F1ToWx1�ֶ�
		float PAfixC1 =0;   //�̶�֧��C1ֵ,mm
		float mvBWidth=0 ;    //�̶�֧�ܸ������۸ּ��,cm,G47=������chdist�ֶ�ֵ��G48=TZA��A01�ֶ�ֵ
		float pnl =0 , pnr=0 ; //�̶�֧��Z1�ܲ���˨��/��̬���ܵ�����Z2�ϳ���,kgf
		float ml =0 , mr =0;   //�̶�֧��Z2�ĺϳ�����,kgf.m
		float pxl=0  , pzl=0  , pxr =0 , pzr=0;   //�̶�֧������̬���ܵ���,kgf
		float mxl =0 , myl=0  , mzl =0 , mxr=0  , myr =0 , mzr=0;  
		float sngCoef=0 ;   //�̶�֧�ܵ�ϵ��
		_variant_t vTmp;
		float tmpPJG=0,tmpT0=0,tmpGDW1=0,tmpPmax4PA=0,tmpPmax4Part=0,tmpSelPJG=0; //�ۼ����������ļ������
		//���������ʧ,��Ĭ��Ϊ���ء��¶ȡ�������λ����Ӧ��ȫ������
		CString stmpT0,stmpPJG;
		if( iChkCondition==0 )
			iChkCondition = iPJG + iTJ + iGDW1 + iCNT + iDW;
		//��Ϊ���˵�����������Ӧ���ۼ��㲿��������
		//��������tmpPJG��Ϊ���ؼ����׼��
		tmpPJG = fabs(modPHScal::pjg);
		stmpPJG.Format(_T("%g"),tmpPJG);
		//�ܲ����㲿��ѡ�����tmpPmax4PartҲʹ��tmpPmax4PA����Ϊ����
		if( modPHScal::gbCalPARTbyMaxLoad )
		{
			tmpPmax4Part = (fabs(modPHScal::pgz) >= fabs(modPHScal::paz) ? fabs(modPHScal::pgz) : fabs(modPHScal::paz));
		}else {
			tmpPmax4Part = fabs(modPHScal::pjg);
		}
		//���ü�����س�ʼֵ���Ա��ۼ�
		tmpSelPJG = tmpPmax4Part;
		//���õ��ɼ�����س�ʼֵ���Ա��ۼ�
		tmpSelPJG = tmpPmax4Part;
		tmpT0 = modPHScal::t0;
		stmpT0.Format(_T("%g"),tmpT0);
		tmpGDW1 =modPHScal::gdw1;
		
		CString strOrderBy ;      //�����ַ���
		CString tmpSQLlugLen ;    //���˳�������ֵ
		CString  tmpSQL ;         //��ǰ������ƥ���ѯ�ֶ���Ϣ
		CString  tmpSQL0 ;        //ǰһ�ε�����ƥ���ѯ�ֶ���Ϣ
		//   long lngErrCalPAfix ;   //����̶�֧��ʱ�����Ĵ����
		/* rsza As Recordset, rsX As Recordset, rsxx As Recordset
		Set rsza = Crs.rsDS*/
		_RecordsetPtr rsza=FrmTxsr.m_pViewTxsr->m_ActiveRs;

		_RecordsetPtr rsTmp;
		rsTmp.CreateInstance(__uuidof(_Recordset));
		_RecordsetPtr rsX;
		rsX.CreateInstance(__uuidof(_Recordset));
//		rsTmp.m_pDatabase=&EDIBgbl::dbSORT;
//		rsX.m_pDatabase=&modPHScal::dbZDJcrude;
		
		CString tbn1,SAfDPmax;
		long i=0,k=0;
		CString sBHFormat,sBH;
		CString sPartID,sRodPASA;
		CString tmpCustomID0,tmpCustomID1;
		CString tmpID0,tmpID1;
		CString tmpFD0;
		CString tmpExtLenFD;
		//	float tmpExtLenValue;//��һ��������������⣩���뵱ǰ���L8�����
		CString tmpFD1Previous,tmpRelationPrevious;
		
		CString tmpFD0value, tmpFD0valuePrevious;   //����ǰһ�����������ƥ���ֶ�ֵ
		float sngDim=0,mvSumHeight=0,sngCSLen=0,sngWeight=0,sngW=0;
		long iCntbNum=0,iCnteNum=0,iNum=0,iNumPA=0;
		long mvSA=0,mvPA=0;
		
		//    long mviClassID=0;  //�����͸ֵ�����
		//    long mviPreviousClassID=0;  //ǰһ������������
		
		CString mvL1CustomID;  //��������L1��CustomIDֵ�����ڵ��ɻ���������ֱ��ѡ�񡣵��е���ʱ�ɼӿ������ٶȡ�
		//���ڸ�������������˨��ĸ��
		CString tmpCustomID;
		float tmpSize2=0,tmpSizeH=0;
		
		//    long iMODE=0 ;       //����ģʽ�������ڼӿ������ٶȡ�
		CString mvSAattachedCustomID;  //����������CustomID
		///>>>>
		CString *Ptype=NULL;   //��������������ṹCustomID����
		//    CString *PtypeClassID;   //��������������ṹClassID����
		//    long *PtypeIndex;   //��������������ṹIndex����
		///>>>>>
		//���Լ���״̬�����ݷ�����ʱ���У�
		//iNum=(iCnteNum/iCntbNum)
		//ÿ������������iNumPart=��ǰһ������������iNumPart*iNum*iCntbNum
		//iNumPart=iNumPart*iNum*iCntbNum
		//iSA=0,iCSPR=7
		//mviClassID=���������͸�����
		//mvSA=���һ�������������,�����б��Ƿ��Ǹ���,
		//��Ϊ����װ��δ���ʱ,���һ�������һ���Ǹ�������
		//mvPA=�ܲ�����,����ͳ�����˵��ܳ���
		//mvariSeqofSPR=��n-1����������(0��ʼ)
		//iSEQofPart=��n��֧���������������ĸ��
		//mvSumHeight=��װ�ܸ߶�(mm)
		mvSumHeight = 0;
		iSEQofSPR = 0;
		iSEQofPart = 1;
		CString SQLx,SQLx2;
		COleVariant v,v1,v2;
		if( rsTmpREF->State != adOpenStatic || (rsTmpREF->adoEOF && rsTmpREF->BOF )){
			throw (GetResStr(IDS_Null_rsTmpREF));
			return false;
		}
		//����Ptype���鱣����mvarrsTmpREF�Ŀ���
		//�Ա�ȡ��֧������ϵ�����2���������,���в���
		if(rsID->State != adOpenStatic)
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
//			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
//			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
			rsID->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			brsIDStatus=TRUE;
		}
		if( ! rsTmpREF->adoEOF )
		{
			rsTmpREF->MoveLast();
			//չ��sFindTBN���ӿ��ٶȡ�
			//tbn1 = sFindTBN(Trim(rsTmpREF.Fields(_T("CustomID"))))
			rsTmpREF->get_Collect((_variant_t)_T("CustomID"),v);
			if( !rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + vtos(v) + _T("\'")), 0, adSearchForward, vTmp))
			{
				//����
				sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(v));
				throw (sTmp);
				return false;
				//Err.Number = iUE_NoRecordFLDEqvThisValueInPictureClipData
				//Err.Description = ResolveResString(iUE_NoRecordFLDEqvThisValueInPictureClipData, _T("|1"), _T("CustomID"), _T("|2"), Trim(rsTmpREF.Fields(_T("CustomID"))))
				//Err.Raise iUE_NoRecordFLDEqvThisValueInPictureClipData
			}
			
			//�������һ������Ǹ����������������롣
			rsID->get_Collect((_variant_t)_T("Index"),v);
			mvSA = vtoi(v);
			
			if( rsTmpREF->GetRecordCount() >= 2 )
			{
				//����������ڻ����2���������ȡ��ͷβ�����
				rsTmpREF->MoveFirst();
				//չ��sFindTBN���ӿ��ٶȡ�
				//tbn1 = sFindTBN(Trim(rsTmpREF.Fields(_T("CustomID"))))
				rsTmpREF->get_Collect((_variant_t)_T("CustomID"),v);
				CString strSql = (_T("trim(CustomID)=\'")) + vtos(v) + _T("\'");
				_variant_t vTmp;
				if( !rsID->Find((_bstr_t)strSql, 0, adSearchForward, vTmp))
				{
					//û�ҵ�CustomID,����
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(v));
					throw (sTmp);
					return false;
				}
				//�����һ������ǹܲ�������ܲ�����롣
				rsID->get_Collect((_variant_t)_T("Index"),v);
				mvPA = vtoi(v);
				
			}
			CString sv,sv1;
			//������������ʼֵ=0
			mvarSATotalWeight = 0;
			//��ʼ����
			for (i = 0 ;i< rsTmpREF->GetRecordCount();i++)
			{
				//���������ڵ�ԭʼ���ݱ�tbn1,�Ա����
				//չ��sFindTBN���ӿ��ٶȡ�
				//tbn1 = sFindTBN(Trim(rsTmpREF.Fields(_T("CustomID"))))
				rsTmpREF->get_Collect((_variant_t)_T("CustomID"),v);
				sv=vtos(v);
				
				if(!rsID->Find((_bstr_t)CString(_T("trim(CustomID)=\'") + sv + _T("\'")), 0, adSearchForward, vTmp))
				{
					//û���ҵ�����
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(v));
					throw (sTmp);
					return false;
				}
				else 
				{
					rsID->get_Collect((_variant_t)_T("ClassID"),v);
					modPHScal::glClassID = vtoi(v);
					rsID->get_Collect((_variant_t)_T("Index"),v);
					modPHScal::glIDIndex =vtoi(v);
					rsID->get_Collect((_variant_t)_T("ID"),v);
					sPartID = vtos(v);
					tmpID0 = sPartID;
				}
				if( modPHScal::glIDIndex == iSA )
				{
					//��ǰ����Ǹ��������һ�����
					if( modPHScal::gbCalSAbyPJG )
					{
						//������ṹ���ؼ������
						tmpSelPJG = tmpPJG;
						//����ǰһ����ļ������ѡ�����
					}
				}
				else 
				{
					//��ʾ���û�������
					//������Ǹ��������һ�����,��Ҫ�������ӵ���һ�����,�����ߴ��ֶκ�����
					//ǰ����ӵ����ID-->tmpCustomID0,tmpCustomID1
					tmpCustomID0 = sv;
					if( i < rsTmpREF->GetRecordCount() - 1 )
					{
						rsID->get_Collect((_variant_t)_T("ID"),v);
						tmpID1=vtos(v);
						/*rsID->Find((_bstr_t)CString(_T("trim(CustomID)=\'")) + sv + _T("\'"))*/
						if( rsID->Find((_bstr_t)CString(_T("trim(CustomID)=\'") + sv + _T("\'")), 0, adSearchForward, vTmp))
						{
							rsID->get_Collect((_variant_t)_T("ID"),v);
							tmpID1 = vtos(v);
						}
						//�������һ�����\
						//��Connect���м������Ƿ���ڼ���ƥ��ĳߴ���Ϣ
						//tmpCustomID1 = Ptype(i + 1);
						rsTmpREF->MoveNext();
						if(!rsTmpREF->adoEOF)
						{
							rsTmpREF->get_Collect((_variant_t)_T("CustomID"),v);
							sv1=vtos(v);
							sv1.TrimLeft();sv1.TrimRight();
							tmpCustomID1=sv1;
						}
						rsTmpREF->MovePrevious();
						if( !rsConnect->Find((_bstr_t)CString(_T("trim(cntb)=\'") + tmpID0 + _T("\' AND trim(cnte)=\'") + tmpID1 + _T("\'")) , 0, adSearchForward, vTmp))
						{
							//Connect���в������������.һ�㲻�ᷢ�������.
							sTmp.Format(GetResStr(IDS_PartCanNotMatchOrNotExistInConnect),tmpCustomID0,tmpCustomID1);
							throw (sTmp);
							return false;
						}
						else 
						{
						/*if( 0 ){
						//����ConnectCondition�ֶεĸ���SQL�����������ӳߴ��飬��������Ӧ�Ը��㡣�ٶ���һ�㡣
						//���﷨���ƣ�Cntb.Size2=Cnte.Size2 AND Cntb.size4<=Cnte.Size5
							/*}*/
							rsConnect->get_Collect((_variant_t)_T("CntbNum"),v);                
							if( vtoi(v)<= 0 )
							{
								iCntbNum = 1;
							}else 
							{
								iCntbNum = vtoi(v);//rsConnect.Fields(_T("CntbNum"))
							}
							rsConnect->get_Collect((_variant_t)_T("CnteNum"),v);                
							if( vtoi(v)<= 0 )
							{
								iCnteNum = 1;
							}else 
							{
								iCnteNum = vtoi(v);//rsConnect.Fields(_T("CnteNum"))
							}
						}
					}
					//else 
					//{
					//�����һ�����
					//}
				}
				if( i == 0 )
				{
					//��һ������ض��ǹܲ�
					//?
					modPHScal::glPAid = modPHScal::glClassID;
					//��ǰ�������������ȷ���Ƿ��ܹ����ܺ��ء�
					//iNum = iCnteNum / iCntbNum
					//iNumPart = iCntbNum
					//����ģʽ:ѡ��֧����ģ��
					modPHScal::CalSpringParallelNum(sv);
					iNumPart = 1;
				}
				else if( i == rsTmpREF->GetRecordCount() - 1 && modPHScal::glIDIndex == iSA )
				{
					//���һ�����
					//ֻ�и������ܳ��ֹ������,glNumSA=rsza(_T("Gnum"))
					iNumPart = modPHScal::glNumSA;
					//MsgBox glNumSA
				}
				else 
				{
					//�����м����Ӽ�
					if( -1 ){
						//ÿ������������iNumPart=��ǰһ������������iNumPart*iNum*iCntbNum
						iNumPart = iNumPart * iNum * iCntbNum;
						iNum = iCnteNum / iCntbNum;
						if( iNumPart <= 0 ) 
							iNumPart = modPHScal::gintParallelNum;
					}
					else 
					{
						//��ǰ����ģʽ��ѡ��֧����ģ�塣
						//Ĭ���������Ϊ���������Ա�򻯱�̡�
						//��ʵ�ڼ���ᵣ�ܲ����ø���ʱ����������ġ�
						if( modPHScal::glIDIndex == iPA )
						{
							//�ܲ�������һ��=1��ֻ������L4ʱ=2��
							iNumPart = 1;
						}else if( modPHScal::glIDIndex == iSA )
							
						{
							//����������=glNumSA
							//��������Ѿ��ڼ������ʱ�г������������ڴ˴����С�
							iNumPart = modPHScal::glNumSA;
						}
						else 
						{
							iNumPart = modPHScal::gintParallelNum;
						}
					}
				}
				//��ʾ���û�������
				//������ֶα仯ֻ�ʺ��ڸ���,�Ǹ����ĺ����ֶ����ƾ�ΪSAfDPmax=_T("Pmax")
				if( modPHScal::glPAid == iPAh )
				{
					//����
					SAfDPmax = _T("PmaxH");
				}
				else if( modPHScal::glPAid == iPAs || modPHScal::glPAid == iPASS || modPHScal::glPAid == iPAGS )
				{
					//����֧��,ˮƽ����λ��С��100mmʱ,������ܺ��ؽϴ�
					if(giUPxyz==1)
					{
						if( fabs(modPHScal::yr1) < 100 && fabs(modPHScal::zr) < 100 )
							SAfDPmax = _T("PmaxSS100");
						else
							SAfDPmax = _T("PmaxSS150");
					}
					else if(giUPxyz==2)
					{
						if( fabs(modPHScal::zr) < 100 && fabs(modPHScal::xr) < 100 )
							SAfDPmax = _T("PmaxSS100");
						else
							SAfDPmax = _T("PmaxSS150");
					}
					else if(giUPxyz==3)
					{
						if( fabs(modPHScal::xr) < 100 && fabs(modPHScal::yr1) < 100 )
							SAfDPmax = _T("PmaxSS100");
						else
							SAfDPmax = _T("PmaxSS150");
					}
				}
				else if( modPHScal::glPAid == iPAfixZ1 || modPHScal::glPAid == iPAfixZ2 )
				{
					//�̶�֧��
					SAfDPmax = _T("PmaxSF");
				}
				//��ʱ�������ڷǹܲ��Ĳ���
				if( (iChkCondition & iCNT) != 0 )
				{
					//Ҫ��׳ߴ�ƥ��
					if( tmpSQL != _T("") )
					{
						//tmpSQL = tmpSQL + _T(" AND ")
					}
					else 
					{
						tmpSQL = _T("");
					}
				}
				else 
				{
					//�������Ҫ�������ӿס��˳ߴ��Զ����
					if( ! modPHScal::gbConnectHoleDimMatch )
						tmpSQL = _T("");
					else 
					{
						//Ҫ��׳ߴ�ƥ��
						if( tmpSQL != _T("") )
						{
							//tmpSQL = tmpSQL + _T(" AND ")
						}
						else 
							tmpSQL = _T("");
					}
				}
				if( i == 0 )
				{
					//��һ������ǹܲ�����������ƥ���ֶ���Ϣ��������Ϣ���������������ʹ�á�
					tmpSQL = _T("");
					tmpFD0valuePrevious = _T("");
				}
				//MsgBox tmpSQL
				//glClassID�Ѿ���sFindTBN�����л��
				switch(modPHScal::glClassID)
				{
				case iPAh:
					for (k = 0 ;k<= (int)modPHScal::gnDW_delta;k++)
					{
						//SQLx = _T("SELECT * FROM ") & tbnPA & _T(" WHERE Dw>= ") & modPHScal::dj * (1 - k * 0.01) & _T(" AND Dw<= ") & modPHScal::dj * (1 + k * 0.01) & _T(" AND Pmax>=") & tmpSelPJG / iNumPart & _T(" AND Tj>=") & tmpT0 & _T(" AND trim(CustomID)='") & Ptype(i) & _T("' ORDER BY Pmax,Dw,tj,Weight")
						sTmp.Format(_T("%g"),modPHScal::dj* (1 - k * 0.01));
						SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Dw>= ") + sTmp;
						sTmp.Format(_T("%g"),modPHScal::dj* (1 + k * 0.01));
						SQLx+=_T(" AND Dw<= ") + sTmp + _T(" AND Pmax>=");
						sTmp.Format(_T("%g"),(float)(tmpSelPJG / iNumPart));
						SQLx+=sTmp;
						SQLx+= _T(" AND Tj>=") + stmpT0 + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY tj,Dw,Pmax,Weight");
						//Debug.Print SQLx
						if(rsX->State == adOpenStatic)
							rsX->Close();
//						rsX.Open(dbOpenSnapshot,SQLx);
						rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
							adOpenDynamic, adLockReadOnly, adCmdText); 
						if( rsX->adoEOF && rsX->BOF )
						{
						}else {
							//�ҵ����˳�ѭ��
							goto commonHandle;
						}
					}
					break;
				case iPAfixZ1:
				case iPAfixZ2:
					modPHScal::CalPAfixZ1Z2(Ptype[i], tmpSelPJG, tmpT0, iNumPart, rsX);
					goto spZ1Z2;
					break;
				case iPAs:
				case iPASS:
				case iPAGS:
					for (k = 0 ;k<=(int) modPHScal::gnDW_delta;k++)
					{
						//SQLx = _T("SELECT * FROM ") & tbnPA & _T(" WHERE Dw>= ") & modPHScal::dj * (1 - k * 0.01) & _T(" AND Dw<= ") & modPHScal::dj * (1 + k * 0.01) & _T(" AND Pmax>=") & tmpSelPJG / iNumPart & _T(" AND Tj>=") & tmpT0 & _T(" AND trim(CustomID)='") & Ptype(i) & _T("' ORDER BY Dw,tj,Weight")
						
						sTmp.Format(_T("%g"),modPHScal::dj * (1 - k * 0.01));
						SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Dw>= ") + sTmp;
						sTmp.Format(_T("%g"),modPHScal::dj * (1 + k * 0.01));
						SQLx+=(_T(" AND Dw<= ") + sTmp + _T(" AND Pmax>=")) ;
						sTmp.Format(_T("%g"),tmpSelPJG / iNumPart);
						SQLx+=sTmp;
						SQLx+= (_T(" AND Tj>=") + stmpT0 + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY tj,Dw,Weight"));
						if(rsX->State == adOpenStatic)
							rsX->Close();
//						rsX.Open(dbOpenSnapshot,SQLx);
						rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
							adOpenDynamic, adLockReadOnly, adCmdText); 
						if( rsX->adoEOF && rsX->BOF ){
						}else {
							//�ҵ����˳�ѭ��
							goto commonHandle;
						}
						
					}

					//2007.09.27(start)
					if(modPHScal::gbPAForceZero && (rsX->adoEOF && rsX->BOF))
					{
						for (k = 0 ;k<=(int) modPHScal::gnDW_delta;k++)
						{
							//SQLx = _T("SELECT * FROM ") & tbnPA & _T(" WHERE Dw>= ") & modPHScal::dj * (1 - k * 0.01) & _T(" AND Dw<= ") & modPHScal::dj * (1 + k * 0.01) & _T(" AND Pmax>=") & tmpSelPJG / iNumPart & _T(" AND Tj>=") & tmpT0 & _T(" AND trim(CustomID)='") & Ptype(i) & _T("' ORDER BY Dw,tj,Weight")
							
							sTmp.Format(_T("%g"),modPHScal::dj * (1 - k * 0.01));
							SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Dw>= ") + sTmp;
							sTmp.Format(_T("%g"),modPHScal::dj * (1 + k * 0.01));
							SQLx+=(_T(" AND Dw<= ") + sTmp + _T(" AND Pmax IS NULL ")) ;
							SQLx+= (_T(" AND Tj>=") + stmpT0 + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY tj,Dw,Weight"));
							if(rsX->State == adOpenStatic)
								rsX->Close();
//							rsX.Open(dbOpenSnapshot,SQLx);
							rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
								adOpenDynamic, adLockReadOnly, adCmdText); 
							if( rsX->adoEOF && rsX->BOF ){
							}else {
								//�ҵ����˳�ѭ��
								goto commonHandle;
							}
						}
					}
					//2007.09.27(end)

					break;
					
				case iPAD4LA:
				case iPAD11LD:
				case iPADLR:
				case iPALX:
					
					//��Щ(�̷�ú�۹ܵ��ܲ�)��ܾ��޹�
					//SQLx = _T("SELECT * FROM ") & tbnPA & _T(" WHERE Pmax>=") & tmpSelPJG / iNumPart & _T(" AND Tj>=") & tmpT0 & _T(" AND trim(CustomID)='") & Ptype(i) & _T("' ORDER BY Pmax,tj,Weight")
					
					sTmp.Format(_T("%g"),tmpSelPJG / iNumPart);
					SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnPA + _T(" WHERE Pmax>=") + sTmp + _T(" AND Tj>=") + stmpT0 + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY tj,Pmax,Weight");
					
					break;
				case iROD:
					//���ܵ�֧��������p202 12.1.2(4)�롶�������糧��ˮ�ܵ���Ƽ����涨(DL/T 5054-1996)��p67 7.5.4��ָ����
					//����ֱ��DN<=50,d>=10mm;DN>=65,d>=12mm����Щ�涨�����е�֧���ܱ�׼ì�ܡ�
					//����������Ժ��ˮ�ܵ�֧�����ֲ� ���ӵ���D4.76S�׾�13mm,ֻ����L8.10(d=10mm)���ӡ������涨ì�ܡ�
					//��ˣ����ﲻ�ܰ��������涨������ֱ�����������ӳߴ��齫ʧ�ܡ�
					//if( modPHScal::dj <= 57 ){
					sTmp.Format(_T("%g"),tmpSelPJG / iNumPart * (rsza->GetCollect(_T("ifLongVertPipe")).boolVal ? iNumPart : 1));
					SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnPART + _T(" WHERE ") + tmpSQL + _T(" Pmax>=") +sTmp + _T(" ORDER BY dh,Weight");
					//}else {
					//  SQLx = _T("SELECT * FROM ") + tbnPART + _T(" WHERE ") + tmpSQL + _T(" Pmax>=") + tmpSelPJG / iNumPart * Iif((rsza(_T("ifLongVertPipe")), iNumPart, 1) + _T(" AND trim(CustomID)=//") + Ptype(i) + _T("// ORDER BY dh,Weight")
					//}
					break;
				case iConnected:
				case iAttached:
					sTmp.Format(_T("%g"),tmpSelPJG / iNumPart * (rsza->GetCollect(_T("ifLongVertPipe")).boolVal ? iNumPart : 1));
					SQLx =CString( _T("SELECT * FROM ")) + modPHScal::tbnPART + _T(" WHERE ") + tmpSQL + _T(" Pmax>=") + sTmp + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY dh,Weight");
					break;
				case iSPR:
					//���ɵĵ���ֱ�����Զ��Ӵ�ʱ����Ҫȡ��ֱ���׼��
					SQLx = CString(_T("SELECT * FROM [")) + modPHScal::tbnSPRINGCrude + _T("] WHERE ") + (tmpSQL != _T("") ? (modPHScal::gbSPRAutoLugDia ? _T("") : tmpSQL) : _T("")) + _T(" dh>=0 AND trim(CustomID)=\'") + sv + _T("\' ORDER BY tj,Weight");
					break;
					//SQLx = _T("SELECT * FROM [") + tbnSPRINGCrude + _T("] WHERE ") + Iif((tmpSQL <> _T(""), Iif((gbSPRAutoLugDia, _T(""), tmpSQL), _T("")) + _T(" dh=") + modPHScal::sSprInfo[iSEQofSPR].DH + _T(" AND trim(CustomID)=//") + Ptype(i) + _T("// ORDER BY dh,Weight")
				case iCSPR:
					SQLx =CString( _T("SELECT * FROM [")) + modPHScal::tbnHDCrude + _T("] WHERE ") + tmpSQL  + _T(" dh>=0 AND trim(CustomID)=\'") + sv + _T("\' ORDER BY dh,Weight");
					break;
					//SQLx = _T("SELECT * FROM [") + tbnHDCrude + _T("] WHERE ") + Iif((tmpSQL <> _T(""), tmpSQL, _T("")) + _T(" dh=") + modPHScal::sSprInfo[iSEQofSPR].DH + _T(" AND trim(CustomID)=//") + Ptype(i) + _T("// ORDER BY dh,Weight")
				case iSAh:
					sTmp.Format(_T("%g"),tmpSelPJG / iNumPart * (rsza->GetCollect(_T("ifLongVertPipe")).boolVal ? iNumPart : 1)); 
					SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE ") + tmpSQL + SAfDPmax + _T(">=") + sTmp + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY GDW1,") + SAfDPmax;
					break;
				case iSACantilever:
				case iSALbrace:
					sTmp.Format(_T("%g"),modPHScal::gdw1);
					SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + sTmp + _T(" AND ") + SAfDPmax + _T(">=");
					sTmp.Format(_T("%g"),tmpSelPJG / iNumPart * (rsza->GetCollect(_T("ifLongVertPipe")).boolVal ? iNumPart : 1));
					SQLx+=sTmp  + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY GDW1,") + SAfDPmax;
					break;
				case iG51:
				case iG56:
				case iG57:
					sTmp.Format(_T("%g"),modPHScal::gdw1);
					SQLx = _T("SELECT * FROM ") + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + sTmp + _T(" AND size2>= ");
					sTmp.Format(_T("%g"),modPHScal::gdw1 / modPHScal::WidthB);
					SQLx +=sTmp  + _T(" AND ") + SAfDPmax + _T(">=");
					sTmp.Format(_T("%g"),tmpSelPJG / iNumPart* (rsza->GetCollect(_T("ifLongVertPipe")).boolVal ? iNumPart : 1));
					SQLx+=sTmp   + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY GDW1,size2,") + SAfDPmax;
					break;
				case iSAbeam:			
				case iG52_55:
					sTmp.Format(_T("%g"),modPHScal::gdw1);
					if( iNumPart == 2 )
					{
						//�����������е����к���
						SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>=IIF(") + sTmp + _T(">L1/2,L1-") + sTmp + _T(",") + sTmp + _T(") AND L1>= ") ;
						sTmp2.Format(_T("%g"),modPHScal::Lspan);
						SQLx+= sTmp + _T(" AND ") + SAfDPmax + _T(">=");
						sTmp2.Format( _T("%g"), tmpSelPJG / iNumPart * (rsza->GetCollect(_T("ifLongVertPipe")) ? iNumPart : 1));
						SQLx+=sTmp2+ _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY GDW1,L1,") + SAfDPmax;
					}
					else
					{
						//һ���������е����к���
						//�ٶ�����������һ�㣬����ѡ������ĸ���ƫ�ڰ�ȫ��
						SQLx =CString( _T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>=IIF(") + sTmp + _T(">L1/2,L1-") + sTmp + _T(",") + sTmp + _T(") AND L1>= ");
						sTmp2.Format(_T("%g"),modPHScal::Lspan);
						SQLx+= sTmp2 + _T(" AND ") + SAfDPmax + _T(">=");
						sTmp2.Format( _T("%g"), tmpSelPJG / iNumPart * (rsza->GetCollect(_T("ifLongVertPipe")) ? iNumPart : 1));
						SQLx+= sTmp2 + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY GDW1,L1,") + SAfDPmax;
					}
					break;
				case iSALbraceFixG47:
				case iSALbraceFixG48:
					//��ʽ�б����ĵ�λ:mvBWidth,cm;PAfixH,mm->cm; modPHScal::gdw1,mm->cm; ��������,kgf.m->kgf.cm; ������,kgf->kgf.
					if(sv!=_T("SJ8"))
					{
						//ǿ�ȼ���:����Ժ�̶�֧�ܸ���G47(ClassID=iSALbraceFixG47=4),G48(ClassID=iSALbraceFixG48=10)
						if( modPHScal::glClassID == iSALbraceFixG47 )
						{
							mvBWidth = 30;
						}
						else 
						{
							vTmp=rsza->GetCollect(_T("A01"));
							mvBWidth = vtof(vTmp)/ 10;
						}
						bool bLongVertPipe;//�ǳ���ֱ�ܵ��ϵĸ��Ե���
						bLongVertPipe=vtob(rsza->GetCollect(_T("ifLongVertPipe")));
						sngSAfixPl = 1.3 * modPHScal::pxlc / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * modPHScal::pxlc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * modPHScal::pzlc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 1 / 4 * F1ToWx1) * modPHScal::mxlc * 100 + modPHScal::mylc * 100 / mvBWidth + 2.6 / mvBWidth * modPHScal::mzlc * 100;
						sngSAfixPr = 1.3 * modPHScal::pyrc / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * modPHScal::pxrc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * modPHScal::pzrc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 1 / 4 * F1ToWx1) * modPHScal::mxrc * 100 + modPHScal::myrc * 100 / mvBWidth + 2.6 / mvBWidth * modPHScal::mzrc * 100;
						//��������Ժ֧�����ֲ�1983���116ҳ���ͣ������ء����ķ�������⡣
						if(1)
						{

							sngSAfixPl = 1.3 * fabs(modPHScal::pylc) / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * fabs(modPHScal::pxlc) + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * fabs(modPHScal::pzlc) + (1.95 / (modPHScal::gdw/10 )+ 1 / 4 * F1ToWx1) * fabs(modPHScal::mxlc) * 100 + fabs(modPHScal::mylc) * 100 / mvBWidth + 2.6 / mvBWidth * fabs(modPHScal::mzlc) * 100;
							sngSAfixPr = 1.3 * fabs(modPHScal::pyrc) / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * fabs(modPHScal::pxrc) + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * fabs(modPHScal::pzrc) + (1.95 / (modPHScal::gdw/10 ) + 1 / 4 * F1ToWx1) * fabs(modPHScal::mxrc) * 100 + fabs(modPHScal::myrc) * 100 / mvBWidth + 2.6 / mvBWidth * fabs(modPHScal::mzrc) * 100;							
						}
						//���������<0,ȡ�����ֵ��Ϊ������ by ligb on 2004.09.11
						if(sngSAfixPl<0) sngSAfixPl=fabs(sngSAfixPl);
						if(sngSAfixPr<0) sngSAfixPr=fabs(sngSAfixPr);
						if( sngSAfixPl > sngSAfixP )
						{
							sngSAfixP = sngSAfixPl;
						}
						else
						{
							sngSAfixP = sngSAfixPr;
						}
						sTmp.Format(_T("%g"),modPHScal::gdw1);
						SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + sTmp + _T(" AND PmaxSF>=")
							+ ftos(sngSAfixP)+ _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY GDW1,PmaxSF");
					}
					else
					{
						
						//����ǿ�ȼ���:����Ժ�̶�֧�ܸ���˫�����Ǽ�SJ8(ClassID=iSALbraceFixG47=4)
					}
					break;
					/*case iGCement
					//�������ṹ���ش���
					SQLx = _T("SELECT * FROM ") + tbnSA + _T(" WHERE trim(CustomID)=//") + Ptype(i) + _T("//");
					*/			   
				default:
					sTmp.Format(GetResStr(IDS_CanNotHandlePART),sPartID);
					throw (sTmp);
					return false;
         }
         //��ԭʼ���ݿ��в����������������,
         //MsgBox _T("Sqlx=") + SQLx
commonHandle:
         if( modPHScal::glClassID == iPAfixZ1 || modPHScal::glClassID == iPAfixZ2 );
         else 
		 {
			 if(rsX->State == adOpenStatic)
				 rsX->Close();
//			 rsX.Open(dbOpenSnapshot,SQLx);
			 rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
				 adOpenDynamic, adLockReadOnly, adCmdText); 
         }
spZ1Z2:
         if( rsX->adoEOF && rsX->BOF )
		 {
			 //Err.Raise iUE_NotFoundPartInZDJCrude, , ResolveResString(iUE_NotFoundPartInZDJCrude, _T("|1"), Ptype(i), _T("|2"), SQLx, _T("|3"), Iif((i > 0, Ptype[i-1], _T("")), _T("|4"), Iif((i < rsTmpREF.RecordCount - 1, Ptype(i + 1), _T("")))
			 if( i > 0 )
			 {
				 CString sv2;
				 rsTmpREF->MovePrevious();
				 if(!rsTmpREF->BOF)
				 {
					 rsTmpREF->get_Collect((_variant_t)_T("CustomID"),v);
					 sv2=vtos(v);
				 }
				 rsTmpREF->MoveNext();
				 if( i < rsTmpREF->GetRecordCount() - 1 ){
					 sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx,sv,sv2,sv1);
					 throw (sTmp);
					 return false;
				 }else {
					 sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx,sv,sv2,_T("NULL"));
					 throw (sTmp);
					 return false;
				 }
			 }
			 else 
			 {
				 CString sv2;
				 rsTmpREF->MovePrevious();
				 if(!rsTmpREF->BOF)
				 {
					 rsTmpREF->get_Collect((_variant_t)_T("CustomID"),v);
					 sv2=vtos(v);
				 }
				 rsTmpREF->MoveNext();
				 sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx,sv,_T("NULL"),sv1);
				 throw (sTmp);
				 return false;
			 }
         }
		 else 
		 {//1
			 rsX->MoveFirst();
			 if(phsAvailableTypeRs->State == adOpenStatic)
				 phsAvailableTypeRs->Close();
//			 phsAvailableTypeRs.m_pDatabase=rsX.m_pDatabase;
//			 phsAvailableTypeRs.Open(dbOpenDynaset,rsX.GetSQL());
			 phsAvailableTypeRs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
				 adOpenDynamic, adLockReadOnly, adCmdText); 
			 //�ܲ����Լ��㣬��ʱ�����˳�
			 if( modPHScal::gbAddPartWeight2PMax && modPHScal::glIDIndex != iSA )
			 {
				 //�ۼ��������������
				 //���Ը�������˨��ĸ���������Լ���������������1m���ǣ�
				 rsX->get_Collect((_variant_t)_T("Weight"),v);
				 tmpSelPJG = tmpSelPJG + vtof(v) * iNumPart;
			 }
			 
			 if( modPHScal::glClassID == iCSPR || modPHScal::glClassID == iSPR )
			 {
				 iSEQofSPR ++;
				 if( iSEQofSPR > modPHScal::giWholeSprNum )
				 {
					 sTmp2.Format(GetResStr(IDS_iSEQofSPROvergiWholeSprNum),_T("\%d"),_T("\%d"));
					 sTmp.Format(sTmp2,iSEQofSPR,modPHScal::giWholeSprNum);
					 throw (sTmp);
					 return false;
				 }
			 }
         }//1
         //����ǰһ�ε�����ƥ���ѯ�ֶ���Ϣ
         tmpSQL = tmpSQL0;
         //���������
         iSEQofPart ++;
         rsTmpREF->MoveNext();
      }//2
      return true;
   }
   return false;
   }
   catch(_com_error & e)
   {
	   ShowMessage(e.Description());
	   return false;
   }
   catch(CString e)
   {
	   return false;
   }
	catch(CException *e)
	{
		e->Delete();
	}
	   return false;
}

void Cphs::simplify2(_RecordsetPtr rsPartBoltNuts,int nth)
{
	CString SQLx;
	try
	{
		
        SQLx.Format("SELECT * FROM [ZB] WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND ( [ClassID]=%d OR [ClassID]=%d OR [ClassID]=%d ) AND [IsSAPart]<>-1 ORDER BY [recno]",
			EDIBgbl::SelVlmID,modPHScal::zdjh ,nth,iBolts,iNuts,iAttached);
//		rsPartBoltNuts->Open(dbOpenDynaset,SQLx);				
		rsPartBoltNuts->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
	}
	catch(CException *e)
	{
		e->Delete();
	}
	
	
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::simplify(_RecordsetPtr rsSAPart,int nth)
{
	try{
		CString SQLx;
		SQLx.Format("SELECT * FROM [ZB] WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND [IsSAPart]<>0 AND IsSAPart IS NOT NULL ORDER BY recno ",
			EDIBgbl::SelVlmID,modPHScal::zdjh,nth);
//		rsSAPart.Open(dbOpenDynaset,SQLx);
		rsSAPart->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rsSAPart->BOF && rsSAPart->adoEOF )
			;
		else
		{
			//ɾ��ZB���б�·֧���ܵĸ������������ظ���Ӹ����ĸ���	
			if(!rsSAPart->BOF) 
				rsSAPart->MoveFirst();
			for(; !rsSAPart->adoEOF; rsSAPart->MoveFirst())
			{		
				rsSAPart->Delete(adAffectCurrent);
				if( rsSAPart->adoEOF )
					break;	
			}
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::GetBoltsNutsAndAttachmentsCLgg(int nth )
{
	//Ŀ�ģ����Ҹ�������˨��ĸ�ı�ţ������·����ɵȵĹ������������ʹ����ĸֱ����ȷ��
	//���룺RsTZB���㲿���ṹ��\
	
	CString *Ptype=NULL;   //��������������ṹID����
	CString *PtypeCustomID=NULL;   //��������������ṹCustomID����
	long *PtypeClassID=NULL;   //��������������ṹClassID����
	long *PtypeIndex=NULL;  //��������������ṹIndex����
	bool *AttachmentGroupHavingF4=NULL;   //�������к��е�ƬF4������ȷ����˨����ԣ�������Ԫ����NewPtype()������ͬ��
	bool bFoundF4;        //���������ҵ�F4��
	long *iRecNo=NULL;    //��������������ṹ��¼�����飬�������������š�
	CString* NewPtypeCustomID=NULL;   //��������������ṹCustomID����
	CString* NewPtypeID=NULL;   //��������������ṹID����
	long *NewPtypeClassID=NULL;   //��������������ṹClassID����
	long *NewPtypeIndex=NULL;   //��������������ṹIndex����
	long *iRecNoOfLugDia=NULL;    //���飺��������ֱ���ļ�¼�š����ڲ������Ӹ����Ĺ��
	long *iLugDia=NULL;           //���飺��������ֱ�������ڲ������Ӹ����Ĺ��
	//	 long iLugNo ;               //���浱ǰ������ţ���һ������ΪiLugNo=1���ڶ�������ΪiLugNo=2��
	long *iBoltsNutsDia=0;      //���飺������˨��ĸֱ����������=��ǰ��¼�ţ������һ��ϡ�����顣���ڲ������Ӹ����Ĺ��
	CString tbn1;
//	 rsX(&modPHScal::dbZDJcrude), rsTmp, rs(&modPHScal::dbZDJcrude);
//	 rsTmpZB(&EDIBgbl::dbPRJDB);
	_RecordsetPtr rsX, rsTmp, rs, rsTmpZB;
	rsX.CreateInstance(__uuidof(Recordset));
	rsTmp.CreateInstance(__uuidof(Recordset));
	rs.CreateInstance(__uuidof(Recordset));
	rsTmpZB.CreateInstance(__uuidof(Recordset));

	CString sBH ; //��Ź��
	long miSEQofCSPR=0;    //��n�����
	bool bCalBoltsDiaOfCSPR=0;  //������Ҫ���¼�����˨��ĸֱ���ĺ��
	float tmpSelPJG=0;    //��ʱ������أ���GetphsBHandSizes���̱����ں����GDW1�ֶ�
	long LenBolts=0;  //��˨����
	bool bFoundG12G13=0;   //�Ƿ��ҵ�G12��G13���Ӽ���Ϊ��ʹ�ñ�߶���ע�ڲ۸������Ϸ������������Ӽ��������������۸����ĸ߶ȣ�������ȷ�������˳��ȡ�
	long tmpT=0;  //���ԭʼ���ݱ��еĳߴ�T,���ھ�ȷ������������ӵ���˨���ȣ��ɺ���GetphsBHandSizesд���TZB֮T�ֶΡ�
	COleVariant vTmp;
	//���浱ǰ֧������Ͻṹ���ݣ��������������ڻ�ͼ��
	long PtypeCount=0;
	try
	{
		long i=0, k=0, n=0, IC=0;
		long lngLastPartNo=0;  //��¼����֧���ܵ������������������
		long lngCurrentPartNo=0 ; //��¼��ǰ��������������������,��ʼ��Ϊ0
		long lngAttachedPartNo=0 ;   //��¼��ǰ�����ǰһ�����֮��ĸ�����,��ʼ��Ϊ0
		long tmpll=iROD;
		modPHScal::giNumRod = GetPhsOneClassPartNumAndPartInfo(tmpll, modPHScal::iSelSampleID, Ptype, PtypeClassID, PtypeIndex,PtypeCount);
		lngLastPartNo = PtypeCount-1;
		AttachmentGroupHavingF4=new bool[lngLastPartNo+1];
		CString SQLx;
		COleVariant vTmp1,vTmp2;
		CString strTmp;
		//�˴�����ʹ��mvarRsTZB�������rsTmpZB,���������ϸ�����˫���������
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND nth = ") + ltos(nth) + _T(" AND IsSAPart<>-1 ORDER BY recno");
//		rsTmpZB.m_pDatabase=&EDIBgbl::dbPRJDB;
//		rsTmpZB.Open(dbOpenDynaset,SQLx);
		rsTmpZB->Open((_bstr_t)SQLx, EDIBgbl::dbPRJDB.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		if( rsTmpZB->adoEOF && rsTmpZB->BOF ){
			strTmp.Format(GetResStr(IDS_NoThisZDJHResultInTBNSelPrjspecTZB),  EDIBgbl::dbPRJDB->DefaultDatabase, EDIBgbl::Btype[EDIBgbl::TZB], SQLx);
			throw strTmp;
		}
		//����������������������
		rsTmpZB->MoveLast();
		rsTmpZB->MoveFirst();
		lngCurrentPartNo = -1;
		IC = rsTmpZB->GetRecordCount();
		NewPtypeCustomID=new CString[IC];
		PtypeCustomID=new CString[IC];
		NewPtypeID=new CString[IC];
		NewPtypeClassID=new long[IC];
		NewPtypeIndex=new long[IC];
		iBoltsNutsDia=new long[IC];
		iRecNo=new long[IC];
		iLugDia=new long[IC];
		iRecNoOfLugDia=new long[IC];
		for( i = 0;i< IC;i++)
		{
			//this->rsID.FindFirst _T("trim(CustomID)=//") + RsTZB.Fields(_T("CustomID")) + _T("//")
			rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
			NewPtypeCustomID[i] = vtos(vTmp1);
			rsTmpZB->get_Collect((_variant_t)_T("ID"), &vTmp1);
			NewPtypeID[i] = vtos(vTmp1);
			rsTmpZB->get_Collect((_variant_t)_T("ClassID"), &vTmp1);
			NewPtypeClassID[i] = vtoi(vTmp1);
			rsTmpZB->get_Collect((_variant_t)_T("index"), &vTmp1);
			NewPtypeIndex[i] = vtoi(vTmp1);
			if( NewPtypeClassID[i] == iROD ){
				//����
				rsTmpZB->get_Collect((_variant_t)_T("recno"), &vTmp1);
				rsTmpZB->get_Collect((_variant_t)_T("sizeC"), &vTmp2);
				iLugDia[vtoi(vTmp1)] =vtoi(vTmp2); //����ֱ��������sizeC�ֶ�
				//iRecNoOfLugDia(iLugNo) = RsTZB.Fields(_T("recno"))   //��ʼ��Ϊ1
				lngCurrentPartNo++;
				//ReDim Preserve iRecNo(lngCurrentPartNo)
				//ReDim Preserve PtypeCustomID(lngCurrentPartNo)
				PtypeCustomID[lngCurrentPartNo] =NewPtypeCustomID[i];
				rsTmpZB->get_Collect((_variant_t)_T("recno"), &vTmp1);
				iRecNo[lngCurrentPartNo] =vtoi(vTmp1);
				//Debug.Print rsTmpZB.Fields(_T("recno")), iLugDia(i + 1), lngCurrentPartNo, iRecNo(lngCurrentPartNo)
			}
			else
			{
				//��������
				if( NewPtypeClassID[i] == iBolts || NewPtypeClassID[i] == iNuts || NewPtypeClassID[i] == iAttached )
				{
					//�������ˣ�����˨��ĸ�򸽼�
					lngAttachedPartNo ++;
					if( NewPtypeID[i] == _T("F4") ){
						//���������Ȧ
						bFoundF4 = true;
						AttachmentGroupHavingF4[lngCurrentPartNo] = true;
					}
				}
				else
				{
					//�������ˣ�������˨��ĸ�򸽼�
					//�Ǻ������¼��T�ֶ�ֵ�����ں���׼ȷ������������ӵ���˨����
					if( NewPtypeClassID[i] == iCSPR )
					{
						rsTmpZB->get_Collect((_variant_t)_T("T"), &vTmp);
						tmpT = vtoi(vTmp);
					}
					lngCurrentPartNo ++;
					//ReDim Preserve iRecNo(lngCurrentPartNo)
					//ReDim Preserve PtypeCustomID(lngCurrentPartNo)
					PtypeCustomID[lngCurrentPartNo]= NewPtypeCustomID[i];
					rsTmpZB->get_Collect((_variant_t)_T("recno"), &vTmp);
					iRecNo[lngCurrentPartNo] = vtoi(vTmp);
					//��λ������ź������Ȧ���ֱ�־
					lngAttachedPartNo = -1;
					bFoundF4 = false;
					//����G12��G13
					if( NewPtypeID[i] == _T("G12") || NewPtypeID[i] == _T("G13") )
						bFoundG12G13 = true;
				}
			}
			rsTmpZB->MoveNext();
		}
		
		//��ʽ��ʼ
		rsTmpZB->MoveFirst();
		lngCurrentPartNo = -1;
		n = 0;
		miSEQofCSPR = -1;
		for (i = 0 ;i< IC;i++)
		{
			//�ҳ��������ɵ�CustomID,���ڲ��
			
			rsID->Find((_bstr_t)(_T("trim(ID)=\'") + NewPtypeID[i] + _T("\'")), 0, adSearchForward, vTmp);
			//��ʼ�������¼�����ֱ��Ϊ�٣�bCalBoltsDiaOfCSPR=False
			
			bCalBoltsDiaOfCSPR = false;
			if( NewPtypeClassID[i] == iROD )
			{
				//����
				lngCurrentPartNo++;
				lngAttachedPartNo = -1;
				//�ҳ��������˵ĳ���ԣ��
				if( PtypeIndex[lngCurrentPartNo - 1] == iPA || Ptype[lngCurrentPartNo - 1] == _T("T3A") )
				{
					//ǰһ������ǹܲ����·�����
					//���ջ���Ժ֧�����ֲ����˨����ԣ������׼��������˳���ԣ����
					rsTmpZB->get_Collect((_variant_t)_T("sizeC"), &vTmp);
					SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + vtos(vTmp);
//					rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
//					rsTmp.Open(dbOpenSnapshot,SQLx );
					rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
						adOpenDynamic, adLockReadOnly, adCmdText); 
					if( rsTmp->adoEOF && rsTmp->BOF )
					{
						//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
						strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude->DefaultDatabase,modPHScal::tbnBoltsSurplusLength,SQLx);
						throw strTmp;
					}
//					rsTmpZB.Edit();
					if( AttachmentGroupHavingF4[lngCurrentPartNo] )
					{
						//�����麬�����Ȧ(F4)
						rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2F4"), &vTmp);
					}
					else
					{
						rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2"), &vTmp);
					}
					rsTmpZB->put_Collect((_variant_t)_T("A"),COleVariant(vtof(vTmp)));
					//ͬʱ�������������С���ȱ������ֶ�C�����ڼ������˳��Ⱥ���CalLugLength����
					rsTmpZB->put_Collect((_variant_t)_T("C"),COleVariant(vtof(vTmp)));
					rsTmpZB->Update();
					rsTmp->Close();
				}
				/*
				else
				{
				//ǰһ��������ǹܲ����·�����
				//��ʱ�������ŵ�����CalLugLength�д���
				//�����T5
				//LHG���ش�����Ϊ����getphsBHandSizes�Ѿ���LHG����ú�������            
				}
				*/
				if( PtypeIndex[lngCurrentPartNo + 1] == iSA || Ptype[lngCurrentPartNo + 1] == _T("T3") )
				{
					//��һ������Ǹ������Ϸ�����
					//���ջ���Ժ֧�����ֲ����˨����ԣ������׼��������˳���ԣ����
					rsTmpZB->get_Collect((_variant_t)_T("sizeC"), &vTmp);
					SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") +vtos(vTmp);
//					rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
//					rsTmp.Open(dbOpenSnapshot,SQLx);
					rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
						adOpenDynamic, adLockReadOnly, adCmdText); 
					if( rsTmp->adoEOF && rsTmp->BOF ){
						//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
						strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude->DefaultDatabase,modPHScal::tbnBoltsSurplusLength,SQLx);
						throw strTmp;
					}
//					rsTmpZB.Edit();
					if( AttachmentGroupHavingF4[lngCurrentPartNo]){
						//�����麬�����Ȧ(F4)
						rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2F4"), &vTmp);
					}
					else{
						rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2"), &vTmp);
					}
					rsTmpZB->put_Collect((_variant_t)_T("B"),COleVariant(vtof(vTmp)));
					//ͬʱ�������������С���ȱ������ֶ�C�����ڼ������˳��Ⱥ���CalLugLength����
					rsTmpZB->put_Collect((_variant_t)_T("C"),COleVariant(vtof(vTmp)));
					rsTmpZB->Update();
					rsTmp->Close();
				}
				/*
				else
				{
				//ǰһ��������Ǹ������Ϸ�����
				//��ʱ�������ŵ�����CalLugLength�д���
				}
				*/
			}
			else
			{
				//��������
				if( NewPtypeClassID[i] == iBolts || NewPtypeClassID[i] == iNuts || NewPtypeClassID[i] == iAttached ){
					//��ǰ����������ˣ�����˨��ĸ�򸽼�
					lngAttachedPartNo ++;
					if( PtypeIndex[lngCurrentPartNo + 1] == iSA )
					{
						//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ������Ǹ���
						if( PtypeClassID[lngCurrentPartNo] == iROD )
						{
							//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ������Ǹ�����ǰһ�����������
							//�����G11,G21~G56
							iBoltsNutsDia[i] = iLugDia[iRecNo[lngCurrentPartNo]];
						}
						else
						{
							//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ������Ǹ�����ǰһ�������������
							//ĳЩֱ�������ڸ����ĺ�������������
							if( PtypeClassID[lngCurrentPartNo] == iCSPR )
							{  //miSEQofCSPR
								//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ������Ǹ�����ǰһ�������������,ǰһ������Ǻ��
								//��˨��ĸֱ��ȡǰһ�����˵�ֱ��
								iBoltsNutsDia[i] = iLugDia[iRecNo[lngCurrentPartNo]];
								//��һ������Ǹ�����ǰһ�������������,ǰһ������Ǻ��,��ǰ����������ͷ��˨��˫ͷ�ݸ�
								//Case _T("LHA"), _T("PHA"), _T("LHB"), _T("PHB")
								//�����ֺ�������������������ӵ���������
//								rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
								if(Ptype[lngCurrentPartNo]==_T("LHE")
									|| Ptype[lngCurrentPartNo]==_T("LHE1")
									|| Ptype[lngCurrentPartNo]==_T("PHE")
									|| Ptype[lngCurrentPartNo]==_T("PHE1")
									|| Ptype[lngCurrentPartNo]==_T("PHC")
									|| Ptype[lngCurrentPartNo]==_T("LHC"))
								{
									//�����ֺ����Ҫ����ͷ��˨��˫ͷ��˨���ӵ����۵�����
									bCalBoltsDiaOfCSPR = true;
									//if( NewPtypeID[i] = _T("F14") Or NewPtypeID[i] = _T("F10") ){
									//�����ɽ���ĸֱ��FiJ��FiK��S��I��T��
									//���ݵ�����е������p20 C�ͺ����TֵӦ�ø�ΪIֵ������ǰ���LHE��PHE(1<=DH<=83)�ĳߴ��Ǻϡ�
									//��̲ŷ��㡣
									//����ʹ�ú��ز��ң���Ϊ����3��GetphsBHandSizes������FiJ��FiK�Ѿ����
									//SQLx = _T("SELECT * FROM [") + tbnLugDiaOfCSPR + _T("] WHERE  PmaxKgf>=") + tmpSelPJG + _T(" AND FiJ>=") + PtypeDiameter(lngCurrentPartNo) + _T(" AND minDH<=") + sSprInfo(miSEQofCSPR).DH + _T(" AND maxDH>=") + sSprInfo(miSEQofCSPR).DH + _T(" AND trim(CustomID)=//") + PtypeCustomID(lngCurrentPartNo) + _T("// ORDER BY Pmax")
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE FiJ=") + ltos(modPHScal::PtypeDiameter[lngCurrentPartNo]) + _T(" AND minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//									rsTmp.Open(dbOpenSnapshot,SQLx);
									rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rsTmp->adoEOF && rsTmp->BOF )
									{
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										rsTmp->Close();
										if( !modPHScal::gbAutoApplyCSPRLugDia )
										{
											//�Զ�����������������ֱ��
											SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//											rsTmp.Open(dbOpenSnapshot,SQLx);
											rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
												adOpenDynamic, adLockReadOnly, adCmdText); 
											if( rsTmp->adoEOF && rsTmp->BOF ){
												//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
												strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase, modPHScal::tbnBoltsSurplusLength, SQLx);
												throw strTmp;
											}
										}
										else
										{
											strTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia),modPHScal::dbZDJcrude->DefaultDatabase,modPHScal::tbnBoltsSurplusLength,SQLx);
											throw strTmp;
										}
									}
									//if( True ){
									//�����˨�׾�FiK
									rsTmp->get_Collect((_variant_t)_T("FiK"), &vTmp);
									iBoltsNutsDia[i] =vtoi(vTmp);
									//������˨ֱ��ϵ�б�tbnCSPRDiameterSerial,���׼ȷֱ��
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnCSPRDiameterSerial + _T("] WHERE Diameter<") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter DESC");
//									rs.Open(dbOpenSnapshot,SQLx );
									rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rs->adoEOF && rs->BOF ){
										//û���ҵ�ƥ��ֱ��ϵ��ֵ
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal::tbnBoltsSurplusLength, SQLx);
										throw strTmp;
									}
									rs->get_Collect((_variant_t)_T("Diameter"), &vTmp);
									iBoltsNutsDia[i] =vtoi(vTmp);
									rsTmp->get_Collect((_variant_t)_T("S"), &vTmp1);
									rsTmp->get_Collect((_variant_t)_T("I"), &vTmp2);
									LenBolts = vtoi(vTmp1) + 2 * vtoi(vTmp2);
									rsTmp->Close();
									//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter");
//									rsTmp.Open(dbOpenSnapshot,SQLx);
									rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rsTmp->adoEOF && rsTmp->BOF )
									{
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude->DefaultDatabase,modPHScal::tbnBoltsSurplusLength, SQLx);
										throw strTmp;
									}
									if( AttachmentGroupHavingF4[lngCurrentPartNo] )
									{
										//�����麬��Ƭ(F4)
										rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2F4"), &vTmp);
										if( NewPtypeID[i] == _T("F10") )
										{    //˫ͷ��˨
											LenBolts+=2 * vtof(vTmp);
										}
										else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") )
										{//��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									else
									{
										rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2"), &vTmp);
										if( NewPtypeID[i] == _T("F10") )
										{    //˫ͷ��˨
											LenBolts += 2 * vtof(vTmp);
										}
										else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") )
										{     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									rsTmp->Close();
									rs->Close();
								}
								//}else{
								//��������ͷ��˨��˫ͷ�ݸ�
								//}
								else if(Ptype[lngCurrentPartNo]==_T("ZHA") 
									|| Ptype[lngCurrentPartNo]== _T("LHD") 
									|| Ptype[lngCurrentPartNo]== _T("LHD1") 
									|| Ptype[lngCurrentPartNo]== _T("ZHB") 
									|| Ptype[lngCurrentPartNo]== _T("PHD") 
									|| Ptype[lngCurrentPartNo]== _T("PHD1"))
								{
									//��Щ�����ֱ�������ڸ�������
									bCalBoltsDiaOfCSPR = true;
									//if( NewPtypeID[i] = _T("F14") Or NewPtypeID[i] = _T("F10") ){
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE FiJ=") + ltos(modPHScal::PtypeDiameter[lngCurrentPartNo])  + _T(" AND minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH)+ _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//									rsTmp.Open(dbOpenSnapshot,SQLx);
									rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rsTmp->adoEOF && rsTmp->BOF ){
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										rsTmp->Close();
										if( ! modPHScal::gbAutoApplyCSPRLugDia )
										{
											//�Զ�����������������ֱ��
											SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//											rsTmp.Open(dbOpenSnapshot,SQLx);
											rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
												adOpenDynamic, adLockReadOnly, adCmdText); 
											if( rsTmp->adoEOF && rsTmp->BOF )
											{
												//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
												strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude->DefaultDatabase, modPHScal::tbnBoltsSurplusLength,  SQLx);
												throw strTmp;
											}
										}
										else
										{
											strTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia),modPHScal::dbZDJcrude->DefaultDatabase,modPHScal::tbnBoltsSurplusLength, SQLx);
											throw strTmp;
										}
									}
									//�����˨�׾�FiK
									rsTmp->get_Collect((_variant_t)_T("FiK"), &vTmp);
									rsTmp->Close();
									iBoltsNutsDia[i] = vtoi(vTmp);
									//������˨ֱ��ϵ�б�tbnCSPRDiameterSerial,���׼ȷֱ��
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnCSPRDiameterSerial + _T("] WHERE Diameter<") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter DESC");
//									rs.Open(dbOpenSnapshot,SQLx);
									rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rs->adoEOF && rs->BOF )
									{
										//û���ҵ�ƥ��ֱ��ϵ��ֵ
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase, modPHScal::tbnBoltsSurplusLength, SQLx);
										throw strTmp;
									}
									rs->get_Collect((_variant_t)_T("Diameter"), &vTmp);
									iBoltsNutsDia[i] = vtoi(vTmp);
									LenBolts =SACSHeight + tmpT;
									//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter");
//									rsTmp.Open(dbOpenSnapshot,SQLx);
									rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rsTmp->adoEOF && rsTmp->BOF )
									{
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
										throw strTmp;
									}
									if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
										//�����麬��Ƭ(F4)
										rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2F4"), &vTmp);
										if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
											LenBolts += 2 * vtof(vTmp);
										}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									else{
										rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2"), &vTmp);
										if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
											LenBolts +=2*vtof(vTmp);
										}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									rsTmp->Close();
									rs->Close();
								}
								//}else{
								//��������ͷ��˨��˫ͷ�ݸ�
								//}
								else if(Ptype[lngCurrentPartNo]== _T("LHA")||
									Ptype[lngCurrentPartNo]==_T("PHA")||
									Ptype[lngCurrentPartNo]==_T("LHB")||
									Ptype[lngCurrentPartNo]==_T("PHB"))
								{
									//��Щ�����ֱ�������ڸ�������
									bCalBoltsDiaOfCSPR = true;
									//if( NewPtypeID[i] = _T("F14") Or NewPtypeID[i] = _T("F10") ){
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE FiJ=") + ltos(modPHScal::PtypeDiameter[lngCurrentPartNo]) + _T(" AND minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//									rsTmp.Open(dbOpenSnapshot,SQLx);
									rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rsTmp->adoEOF && rsTmp->BOF )
									{
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
										throw strTmp;
									}
									//����ֱ���Ǳ�׼�ģ�����Ҫ����ϵ�б�
									rsTmp->get_Collect((_variant_t)_T("FiJ"), &vTmp);iBoltsNutsDia[i]=vtoi(vTmp);
									LenBolts = Cavphs->SACSHeight + 15;
									//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]);
									rsTmp->Close();
//									rsTmp.Open(dbOpenSnapshot,SQLx);
									rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rsTmp->adoEOF && rsTmp->BOF ){
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
										throw strTmp;
									}
									if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
										//�����麬��Ƭ(F4)
										rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2F4"), &vTmp);
										if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
											LenBolts+= 2 * vtof(vTmp);
										}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}else{
										rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2"), &vTmp);
										if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
											LenBolts+= 2 * vtof(vTmp);
										}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									rsTmp->Close();
								}
								
								//���ȣ�����˨��ĸ������Ҹ�������˨��ĸ
								
								tbn1 = modPHScal::tbnBoltsNuts;
								if( bCalBoltsDiaOfCSPR )
								{
									CString sID;
									rsTmpZB->get_Collect((_variant_t)_T("ID"), &vTmp);
									sID=vtos(vTmp);
									if( NewPtypeID[i] == _T("F14") ){
										//����ͷ����׼����˨����ֱ������������ȡ��С��һ��
										SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + modPHScal::sFindCustomID(sID) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]) + _T(" AND sizeH>=") + ltos(LenBolts) + _T(" ORDER BY size2,sizeH");
									}
									else if( NewPtypeID[i] == _T("F10") || NewPtypeID[i] == _T("F9") ){
										//˫/��ͷ��˨
										SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + modPHScal::sFindCustomID(sID) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY size2");
									}
									else{
										//��ĸ��Ƭ
										SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + modPHScal::sFindCustomID(sID) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY size2");
									}
//									rsX.Open(dbOpenSnapshot,SQLx);
									rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
									if( rsX->adoEOF && rsX->BOF )
									{
										//���������˨��ĸ����û�ҵ���������˨��ĸ
										rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp);
										strTmp.Format(GetResStr(IDS_NoFoundAttachInZDJCrude),modPHScal::dbZDJcrude->DefaultDatabase,SQLx,vtos(vTmp));
										ShowMessage(strTmp);
									}
									else
									{
										//����˨��ĸ�����ҵ���������˨��ĸ
//										rsTmpZB.Edit();
										//������д��seq�ֶ�,��Ϊ���㺯��GetphsBHandSizes�Ѿ�ȷ������Щ����ǿ��Ա�ţ���Щ�����Ա�š�����Ҫ��ŵĶ������ա�
										//rsTmpZB.Fields(_T("SEQ")) = i
										if( NewPtypeID[i] == _T("F10") || NewPtypeID[i] == _T("F9") )
										{
											//˫ͷ��˨���ߵ�ͷ��˨
											rsX->get_Collect((_variant_t)_T("BH"), &vTmp);
											EDIBgbl::dbPRJ->Execute((_bstr_t)(CString(_T("UPDATE tmpCSLen SET BH=\'")) + vtos(vTmp)+_T("\'")), NULL, adCmdText);
											rsTmpZB->put_Collect((_variant_t)_T("BH"),vTmp);
											EDIBgbl::dbPRJ->Execute((_bstr_t)(CString( _T("UPDATE tmpCSLen SET L1=")) + ltos(LenBolts)), NULL, adCmdText);
											SQLx = _T("SELECT ") + vtos(modPHScal::sFindFLD(_T("ID"), _T("BHFormat"), NewPtypeID[i])) + _T(" AS sBH FROM tmpCSLen");
//											rsTmp.m_pDatabase=&EDIBgbl::dbPRJ;
//											rsTmp.Open(dbOpenSnapshot,SQLx);
											rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
												adOpenDynamic, adLockReadOnly, adCmdText); 
											if( rsTmp->adoEOF && rsTmp->BOF ){
												
											}
											else
											{
												rsTmp->get_Collect((_variant_t)_T("sBH"), &vTmp);
												rsTmpZB->put_Collect((_variant_t)_T("CLgg"),vTmp);
												//												rsTmpZB->put_Collect((_variant_t)_T("BH"),vTmp);
												//˫ͷ��˨���ذ�1000mm���㱣����ԭʼ��������ֶ�
												rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
												rsTmpZB->get_Collect((_variant_t)_T("CLnum"), &vTmp2);
												rsTmpZB->put_Collect((_variant_t)_T("CLdz"),COleVariant(vtof(vTmp1) / 1000 * LenBolts));
												rsTmpZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(vtof(vTmp1) / 1000 * LenBolts * vtof(vTmp2)));
												if(NewPtypeID[i] == _T("F4"))
												{
													//һ�����ʹ��45��
													rsX->get_Collect((_variant_t)_T("Material"), &vTmp1);
													rsTmpZB->put_Collect((_variant_t)_T("CLcl"),vTmp1);
												}
											}
											rsTmp->Close();
										}
										else
										{
											//����ͷ����׼����˨����ֱ������������ȡ��С��һ��
											//��ĸ��Ƭ
											rsX->get_Collect((_variant_t)_T("BH"), &vTmp);
											rsTmpZB->put_Collect((_variant_t)_T("CLgg"),vTmp);
											rsTmpZB->put_Collect((_variant_t)_T("BH"),vTmp);
											rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
											rsTmpZB->get_Collect((_variant_t)_T("CLnum"), &vTmp2);
											rsTmpZB->put_Collect((_variant_t)_T("CLdz"),vTmp1);
											rsTmpZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(vtof(vTmp1)* vtof(vTmp2)));
											if(NewPtypeID[i] == _T("F4"))
											{
												//һ�����ʹ��45��
												rsX->get_Collect((_variant_t)_T("Material"), &vTmp1);
												rsTmpZB->put_Collect((_variant_t)_T("CLcl"),vTmp1);
											}
										}
										rsTmpZB->Update();
									}
									rsX->Close();
								}
								//else{
								//�������¼�����˨��ĸ�ߴ�
								//}
							}
							//else{
							//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ������Ǹ�����ǰһ�������������,ǰһ��������Ǻ��
							//}
						}
					}
					else
					{
						//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ��������Ǹ���
						if( PtypeClassID[lngCurrentPartNo + 1] == iROD ){
							//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ��������Ǹ���,������
							//��˨��ĸֱ��ȡ��һ�����˵�ֱ��
							iBoltsNutsDia[i] = iLugDia[iRecNo[lngCurrentPartNo + 1]];
						}
						else
						{
							//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ��������Ǹ���,��������
							if( PtypeClassID[lngCurrentPartNo] == iROD ){
								//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ��������Ǹ���,��������,ǰһ�����������
								//��˨��ĸֱ��ȡǰһ�����˵�ֱ��
								iBoltsNutsDia[i] = iLugDia[iRecNo[lngCurrentPartNo]];
							}
							else
							{
								//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ��������Ǹ���,��������,ǰһ�������������
								//ĳЩ��G12/G13/L7/L8���ӵĺ�������������
								if( PtypeClassID[lngCurrentPartNo] == iCSPR )
								{  //miSEQofCSPR
									//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ��������Ǹ���,��������,ǰһ�������������,ǰһ������Ǻ��
									//��˨��ĸֱ��ȡǰһ�����˵�ֱ��
									iBoltsNutsDia[i] = iLugDia[iRecNo[lngCurrentPartNo]];
									//��ǰ����������ˣ�����˨��ĸ�򸽼�,��һ��������Ǹ���,��������,ǰһ�������������,ǰһ������Ǻ��,��ǰ����������ͷ��˨��˫ͷ�ݸ�
									//Case _T("LHA"), _T("PHA"), _T("LHB"), _T("PHB")
									//�����ֺ�������������������ӵ���������
//									rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
									if(Ptype[lngCurrentPartNo]==_T("LHE")  ||
										Ptype[lngCurrentPartNo]==_T("LHE1") ||
										Ptype[lngCurrentPartNo]==_T("PHE")  ||
										Ptype[lngCurrentPartNo]==_T("PHE1") ||
										Ptype[lngCurrentPartNo]==_T("PHC")  ||
										Ptype[lngCurrentPartNo]==_T("LHC"))
									{
										//_T("LHE"), _T("PHE")�����ֺ����Ҫ����ͷ��˨��˫ͷ��˨���ӵ����۵�����
										bCalBoltsDiaOfCSPR = true;
										//if( NewPtypeID[i] = _T("F14") Or NewPtypeID[i] = _T("F10") ){
										//�����ɽ���ĸֱ��FiJ��FiK��S��I��T��
										//���ݵ�����е������p20 C�ͺ����TֵӦ��ΪIֵ������ǰ���LHE��PHE(1<=DH<=83)�ĳߴ��Ǻϡ�
										//��̲ŷ��㡣
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE FiJ=") + ltos(modPHScal::PtypeDiameter[lngCurrentPartNo]) + _T(" AND minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//										rsTmp.Open(dbOpenSnapshot,SQLx);
										rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rsTmp->adoEOF && rsTmp->BOF )
										{
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											rsTmp->Close();
											if( !modPHScal::gbAutoApplyCSPRLugDia )
											{
												//�Զ�����������������ֱ��
												SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//												rsTmp.Open(dbOpenSnapshot,SQLx);
												rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
													adOpenDynamic, adLockReadOnly, adCmdText); 
												if( rsTmp->adoEOF && rsTmp->BOF )
												{
													//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
													strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
													throw strTmp;
												}
											}
											else
											{
												strTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx);
												throw strTmp;
											}
										}
										//�����˨�׾�FiK
										rsTmp->get_Collect((_variant_t)_T("FiK"), &vTmp);
										iBoltsNutsDia[i]=vtoi(vTmp);
										//������˨ֱ��ϵ�б�tbnCSPRDiameterSerial,���׼ȷֱ��
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnCSPRDiameterSerial + _T("] WHERE Diameter<") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter DESC");
//										rs.Open(dbOpenSnapshot,SQLx);
										rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rs->adoEOF && rs->BOF ){
											//û���ҵ�ƥ��ֱ��ϵ��ֵ
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										rs->get_Collect((_variant_t)_T("Diameter"), &vTmp);iBoltsNutsDia[i]=vtoi(vTmp);
										rsTmp->get_Collect((_variant_t)_T("S"), &vTmp1);rsTmp->get_Collect((_variant_t)_T("I"), &vTmp2);
										LenBolts=vtof(vTmp1)+2*vtof(vTmp2);
										//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter");
										rsTmp->Close();
//										rsTmp.Open(dbOpenSnapshot,SQLx);
										rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rsTmp->adoEOF && rsTmp->BOF ){
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
											//�����麬��Ƭ(F4)
											rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2F4"), &vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts += 2 * vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}
										else
										{
											rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2"), &vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts += 2 * vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}
										rs->Close();
										rsTmp->Close();
									}
									//}else{
									//��������ͷ��˨��˫ͷ�ݸ�
									//}
									else if(Ptype[lngCurrentPartNo]==_T("ZHA") ||
										Ptype[lngCurrentPartNo]==_T("LHD") ||
										Ptype[lngCurrentPartNo]==_T("LHD1") ||
										Ptype[lngCurrentPartNo]==_T("ZHB") ||
										Ptype[lngCurrentPartNo]==_T("PHD") ||
										Ptype[lngCurrentPartNo]==_T("PHD1"))
									{
										//��Щ�����ֱ�������ڸ�������
										bCalBoltsDiaOfCSPR = true ;   
										//if( NewPtypeID[i] = _T("F14") Or NewPtypeID[i] = _T("F10") ){
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE FiJ=") + ltos(modPHScal::PtypeDiameter[lngCurrentPartNo]) + _T(" AND minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//										rsTmp.Open(dbOpenSnapshot,SQLx);
										rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rsTmp->adoEOF && rsTmp->BOF ){
											rsTmp->Close();
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											if( !modPHScal::gbAutoApplyCSPRLugDia )
											{
												//�Զ�����������������ֱ��
												SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//												rsTmp.Open(dbOpenSnapshot,SQLx);
												rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
													adOpenDynamic, adLockReadOnly, adCmdText); 
												if( rsTmp->adoEOF && rsTmp->BOF ){
													//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
													strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
													throw strTmp;
												}
											}else{
												strTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx);
												throw strTmp;
											}
										}
										//�����˨�׾�FiK
										rsTmp->get_Collect((_variant_t)_T("FiK"), &vTmp);
										iBoltsNutsDia[i]=vtoi(vTmp);
										//������˨ֱ��ϵ�б�tbnCSPRDiameterSerial,���׼ȷֱ��
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnCSPRDiameterSerial + _T("] WHERE Diameter<") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter DESC");
//										rs.Open(dbOpenSnapshot,SQLx);
										rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rs->adoEOF && rs->BOF ){
											//û���ҵ�ƥ��ֱ��ϵ��ֵ
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										rs->get_Collect((_variant_t)_T("Diameter"), &vTmp);
										iBoltsNutsDia[i]=vtoi(vTmp);
										LenBolts = SACSHeight + tmpT;
										//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]);
										rsTmp->Close();
//										rsTmp.Open(dbOpenSnapshot,SQLx);
										rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rsTmp->adoEOF && rsTmp->BOF ){
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
											//�����麬��Ƭ(F4)
											rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2F4"), &vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts +=2*vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}else{
											rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2"), &vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts +=2*vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}
										rs->Close();
										rsTmp->Close();
									}
									//}else{
									//��������ͷ��˨��˫ͷ�ݸ�
									//}
									else if(Ptype[lngCurrentPartNo]== _T("LHA")||
										Ptype[lngCurrentPartNo]==_T("PHA")||
										Ptype[lngCurrentPartNo]==_T("LHB")||
										Ptype[lngCurrentPartNo]==_T("PHB"))
									{
										//��Щ�����ֱ�������ڸ�������
										bCalBoltsDiaOfCSPR = true;
										//if( NewPtypeID[i] = _T("F14") Or NewPtypeID[i] = _T("F10") ){
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE FiJ=") + ltos(modPHScal::PtypeDiameter[lngCurrentPartNo]) + _T(" AND minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
//										rsTmp.Open(dbOpenSnapshot,SQLx);
										rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rsTmp->adoEOF && rsTmp->BOF ){
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										rsTmp->get_Collect((_variant_t)_T("FiJ"), &vTmp);
										iBoltsNutsDia[i]=vtoi(vTmp);
										LenBolts = SACSHeight + 15;
										rsTmp->Close();
										//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]);
//										rsTmp.Open(dbOpenSnapshot,SQLx);
										rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rsTmp->adoEOF && rsTmp->BOF ){
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude->DefaultDatabase,modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
											//�����麬��Ƭ(F4)
											rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2F4"), &vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts +=2*vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}else{
											rsTmp->get_Collect((_variant_t)_T("SurplusLengthWithF1F2"), &vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts +=2*vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}
									}
									
									//���ȣ�����˨��ĸ������Ҹ�������˨��ĸ
									tbn1 = modPHScal::tbnBoltsNuts;
									if( bCalBoltsDiaOfCSPR )
									{
										//��Ҫ���¼�����ֱ��
										rsTmpZB->get_Collect((_variant_t)_T("ID"), &vTmp);
										if( NewPtypeID[i] == _T("F14") ){
											//����ͷ����׼����˨����ֱ������������ȡ��С��һ��
											SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + modPHScal::sFindCustomID(vtos(vTmp)) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]) + _T(" AND sizeH>=") + ltos(LenBolts) + _T(" ORDER BY size2,sizeH");
										}else if( NewPtypeID[i] == _T("F10") || NewPtypeID[i] == _T("F9") ){
											//˫/��ͷ��˨
											SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + modPHScal::sFindCustomID(vtos(vTmp)) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY size2");
										}else{
											//��ĸ��Ƭ
											SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + modPHScal::sFindCustomID(vtos(vTmp)) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY size2");
										}
//										rsX.Open(dbOpenSnapshot,SQLx);
										rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
											adOpenDynamic, adLockReadOnly, adCmdText); 
										if( rsX->adoEOF && rsX->BOF ){
											//���������˨��ĸ����û�ҵ���������˨��ĸ
											//Err.Number = iUE_NoFoundAttachInZDJCrude
											//Err.Description = ResolveResString(iUE_NoFoundAttachInZDJCrude, _T("|1"), dbZDJcrude.Name, _T("|2"), SQLx, _T("|3"), rsTmpZB.Fields(_T("CustomID")))
											//Err.Raise iUE_NoFoundAttachInZDJCrude
											rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp);
											strTmp.Format(GetResStr(IDS_NoFoundAttachInZDJCrude),modPHScal::dbZDJcrude->DefaultDatabase,SQLx,vtos(vTmp));
											ShowMessage(strTmp);
										}
										else
										{
											//����˨��ĸ�����ҵ���������˨��ĸ
//											rsTmpZB.Edit();
											//������д��seq�ֶ�,��Ϊ���㺯��GetphsBHandSizes�Ѿ�ȷ������Щ����ǿ��Ա�ţ���Щ�����Ա�š�����Ҫ��ŵĶ������ա�
											//rsTmpZB.Fields(_T("SEQ")) = i
											if( NewPtypeID[i] == _T("F10") || NewPtypeID[i] == _T("F9") )
											{
												//˫ͷ��˨
												rsX->get_Collect((_variant_t)_T("BH"), &vTmp);
												EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET BH=\'") + vtos(vTmp) + _T("\'")), NULL, adCmdText);
												rsTmpZB->put_Collect((_variant_t)_T("BH"),vTmp);
												EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET L1=") + ltos(LenBolts)), NULL, adCmdText);
												SQLx = _T("SELECT ") + vtos(modPHScal::sFindFLD(_T("ID"), _T("BHFormat"), NewPtypeID[i])) + _T(" AS sBH FROM tmpCSLen");
//												rsTmp.m_pDatabase=&EDIBgbl::dbPRJ;
//												rsTmp.Open(dbOpenSnapshot,SQLx);
												rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
													adOpenDynamic, adLockReadOnly, adCmdText); 
												if( rsTmp->adoEOF && rsTmp->BOF )
												{
													strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),EDIBgbl::dbPRJ->DefaultDatabase,_T("tmpCSLen"),SQLx);
													throw strTmp;
												}
												rsTmp->get_Collect((_variant_t)_T("sBH"), &vTmp);
												rsTmpZB->put_Collect((_variant_t)_T("CLgg"),vTmp);
												rsTmpZB->put_Collect((_variant_t)_T("BH"),vTmp);
												//˫ͷ��˨���ذ�1000mm���㱣����ԭʼ��������ֶ�
												rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
												rsTmpZB->get_Collect((_variant_t)_T("CLnum"), &vTmp2);
												rsTmpZB->put_Collect((_variant_t)_T("CLdz"),COleVariant(vtof(vTmp1) / 1000 * LenBolts));
												rsTmpZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(vtof(vTmp1)/ 1000 * LenBolts * vtof(vTmp2)));
												rsTmp->Close();
												if(NewPtypeID[i] == _T("F4"))
												{
													//һ�����ʹ��45��
													rsX->get_Collect((_variant_t)_T("Material"), &vTmp1);
													rsTmpZB->put_Collect((_variant_t)_T("CLcl"),vTmp1);
												}
											}
											else
											{
												//����ͷ����׼����˨����ֱ������������ȡ��С��һ��
												//��ĸ��Ƭ
												rsX->get_Collect((_variant_t)_T("BH"), &vTmp);
												rsTmpZB->put_Collect((_variant_t)_T("CLgg"),vTmp);
												rsTmpZB->put_Collect((_variant_t)_T("BH"),vTmp);
												rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
												rsTmpZB->get_Collect((_variant_t)_T("CLnum"), &vTmp2);
												rsTmpZB->put_Collect((_variant_t)_T("CLdz"),vTmp1);
												rsTmpZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(vtof(vTmp1)*vtof(vTmp2)));
												if(NewPtypeID[i] == _T("F4"))
												{
													//һ�����ʹ��45��
													rsX->get_Collect((_variant_t)_T("Material"), &vTmp1);
													rsTmpZB->put_Collect((_variant_t)_T("CLcl"),vTmp1);
												}
											}
											rsTmpZB->Update();
										}
										rsX->Close();
									}
								}
							}
						}
					}
					
					//�Ѿ������˨��ĸֱ��
					//�ڴ˿�ʼ������˨��ĸ��
					//д����(��˨����ĸ)����
					rsTmpZB->get_Collect((_variant_t)_T("ClassID"), &vTmp);
					if( ! bCalBoltsDiaOfCSPR && (vtoi(vTmp) == iAttached || vtoi(vTmp) == iBolts || vtoi(vTmp) == iNuts) )
					{
						//���к����˨��ĸ�ļ�������������γ�����Ϊ��ʱbCalBoltsDiaOfCSPR=true
						//���ȣ�����˨��ĸ������Ҹ�������˨��ĸ
						tbn1 = modPHScal::tbnBoltsNuts;
						rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp);
						SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + vtos(vTmp) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]);
						
//						rsX.Open(dbOpenSnapshot,SQLx);
						rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
							adOpenDynamic, adLockReadOnly, adCmdText); 
						if( rsX->adoEOF && rsX->BOF ){
							//���������˨��ĸ����û�ҵ���������˨��ĸ
							//��ô���ڸ�������������Ҹ�������˨��ĸ
							rsX->Close();
							tbn1 = modPHScal::tbnAttachment;
							SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + vtos(vTmp) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY size2");
//							rsX.Open(dbOpenSnapshot,SQLx);
							rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
								adOpenDynamic, adLockReadOnly, adCmdText); 
							if( rsX->adoEOF && rsX->BOF ){
								//�ڸ�����������û�ҵ���������˨��ĸ
								//Err.Number = iUE_NoFoundAttachInZDJCrude
								//Err.Description = ResolveResString(iUE_NoFoundAttachInZDJCrude, _T("|1"), dbZDJcrude.Name, _T("|2"), SQLx, _T("|3"), rsTmpZB.Fields(_T("CustomID")))
								//Err.Raise iUE_NoFoundAttachInZDJCrude
								rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp);
								strTmp.Format(GetResStr(IDS_NoFoundAttachInZDJCrude),modPHScal::dbZDJcrude->DefaultDatabase,SQLx,vtos(vTmp));
								ShowMessage(strTmp);
							}
							else{
								//�ڸ������������ҵ���������˨��ĸ
//								rsTmpZB.Edit();
								//������д��seq�ֶ�,��Ϊ���㺯��GetphsBHandSizes�Ѿ�ȷ������Щ����ǿ��Ա�ţ���Щ�����Ա�š�����Ҫ��ŵĶ������ա�
								//rsTmpZB.Fields(_T("SEQ")) = i
								rsX->get_Collect((_variant_t)_T("BH"), &vTmp);
								rsTmpZB->put_Collect((_variant_t)_T("CLgg"),vTmp);
								rsTmpZB->put_Collect((_variant_t)_T("BH"),vTmp);
								rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
								rsTmpZB->get_Collect((_variant_t)_T("CLnum"), &vTmp2);
								rsTmpZB->put_Collect((_variant_t)_T("CLdz"),vTmp1);
								rsTmpZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(vtof(vTmp1)*vtof(vTmp2)));
								if(NewPtypeID[i] == _T("F4"))
								{
									//һ�����ʹ��45��
									rsX->get_Collect((_variant_t)_T("Material"), &vTmp1);
									rsTmpZB->put_Collect((_variant_t)_T("CLcl"),vTmp1);
								}
								rsTmpZB->Update();
							}
						}
						else{
							//����˨��ĸ�����ҵ���������˨��ĸ
//							rsTmpZB.Edit();
							//������д��seq�ֶ�,��Ϊ���㺯��GetphsBHandSizes�Ѿ�ȷ������Щ����ǿ��Ա�ţ���Щ�����Ա�š�����Ҫ��ŵĶ������ա�
							//rsTmpZB.Fields(_T("SEQ")) = i
							rsX->get_Collect((_variant_t)_T("BH"), &vTmp);
							rsTmpZB->put_Collect((_variant_t)_T("CLgg"),vTmp);
							rsTmpZB->put_Collect((_variant_t)_T("BH"),vTmp);
							rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
							rsTmpZB->get_Collect((_variant_t)_T("CLnum"), &vTmp2);
							rsTmpZB->put_Collect((_variant_t)_T("CLdz"),vTmp1);
							rsTmpZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(vtof(vTmp1)*vtof(vTmp2)));
							if(NewPtypeID[i] == _T("F4"))
							{
								//һ�����ʹ��45��
								rsX->get_Collect((_variant_t)_T("Material"), &vTmp1);
								rsTmpZB->put_Collect((_variant_t)_T("CLcl"),vTmp1);
							}
							rsTmpZB->Update();
						}
						rsX->Close();
					}
				}
				else{
					//�������ˣ�������˨��ĸ�򸽼�
					lngCurrentPartNo ++;
					lngAttachedPartNo = -1;
					//�������
					if( NewPtypeClassID[i] == iCSPR ){
						miSEQofCSPR ++;
						rsTmpZB->get_Collect((_variant_t)_T("gdw1"), &vTmp);
						tmpSelPJG = vtof(vTmp);
					}
					if( NewPtypeClassID[i] == iSPR ){
						//��ǰ����������ˣ�������˨��ĸ�򸽼�,�ǵ��Լ�
						//����Ѿ���getphsBHandSizes�д�����ϡ�ֻҪ����T5���ɡ�
						if( PtypeClassID[lngCurrentPartNo + 1] == iROD ){
							//��ǰ����������ˣ�������˨��ĸ�򸽼�,�ǵ��Լ�,��һ�����������
							//�뵯�Լ����������ֱ��ȡ��һ�����˵�ֱ��
							iBoltsNutsDia[i] = iLugDia[iRecNo[lngCurrentPartNo + 1]];
						}else{
							//��ǰ����������ˣ�������˨��ĸ�򸽼�,�ǵ��Լ�,��һ�������������
							if( PtypeClassID[lngCurrentPartNo]== iROD ){
								//��ǰ����������ˣ�������˨��ĸ�򸽼�,�ǵ��Լ�,��һ�������������,ǰһ�����������
								//�뵯�Լ����������ֱ��ȡǰһ�����˵�ֱ��
								iBoltsNutsDia[i] = iLugDia[iRecNo[lngCurrentPartNo]];
							}else{
								//��ǰ����������ˣ�������˨��ĸ�򸽼�,�ǵ��Լ�,��һ�������������,ǰһ�������������
							}
						}
						//�������õ���/�������ɹ�������ݸ�ֱ����
						//����ǵ���ǰ��������ˣ��򸽼�����ֱ������Ź����
						//����Ѿ���getphsBHandSizes�д�����ϡ�ֻҪ����T5���ɡ�
						if( NewPtypeID[i] == _T("T5") )
						{
							rsTmpZB->get_Collect((_variant_t)_T("CLgg"), &vTmp);
							sBH =vtos(vTmp);
							//ע��˴�����M��ΪT5�ɽ���ĸֱ����־�ģ�����Ӧ������׼ʱ������������!
							k = sBH.Find(_T("M"));
							if( k !=-1 ){
								sBH = sBH.Left(k+1) + ltos(iBoltsNutsDia[i]);
//								rsTmpZB.Edit();
								rsTmpZB->put_Collect((_variant_t)_T("CLgg"),STR_VAR(sBH));
								//								rsTmpZB->put_Collect((_variant_t)_T("BH"),STR_VAR(sBH));
								rsTmpZB->Update();
							}
						}
					}
					else
					{
						//��ǰ����������ˣ�������˨��ĸ�򸽼�,���ǵ��Լ�
						if( NewPtypeClassID[i] == iGCement || NewPtypeClassID[i] == iSAh )
						{
							//����������ֱ�ӵ�����
						}
						else
						{
							if( bFoundG12G13 && NewPtypeIndex[i] == iSA ){
								//��߷��ͳһ������ע�۸��ϱ�����
								//if( bFoundG12G13 And NewPtypeIndex[i] = iSA And NewPtypeID[i] <> _T("G43") And NewPtypeID[i] <> _T("G44") ){
								//��߷��ͳһ��G43/G44��ע�۸��±��棬���඼��ע�۸��ϱ����ߡ�
								//G43/G44�������߶�
								//���ø������۸����߶ȣ��Ա���ȷ�������˳���
//								rsTmpZB.Edit();
								rsTmpZB->put_Collect((_variant_t)_T("sizeH"),COleVariant(SACSHeight));
								rsTmpZB->Update();
							}
						}
					}
					//������ǰ����������ˣ�������˨��ĸ�򸽼�
				}
			}
			rsTmpZB->MoveNext();
		}
		
		//���¸�����������,���һ��recno��Ϊ�յļ�¼���Ǹ�����¼,
		//����,Ҫ�ų������ǻ��������������
		
		SQLx.Format("SELECT * FROM [ZB] WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND [Index]=%d ORDER BY recno",
			EDIBgbl::SelVlmID,modPHScal::zdjh,nth,iSA);
		if( rsTmpZB->State == adOpenStatic )
			rsTmpZB->Close();
//		rsTmpZB.Open(dbOpenDynaset,SQLx);
		rsTmpZB->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 

		if(rsTmpZB->adoEOF && rsTmpZB->BOF)
		{
			return ;
		}
		else 
		{
//			rsTmpZB.Edit();
			if( rsTmpZB->BOF )
			{
				strTmp.Format(GetResStr(IDS_BOForEOFofmvarRsTZB), _T("BOF"));
				throw strTmp;
			}
			if( rsTmpZB->adoEOF )
			{
				strTmp.Format(GetResStr(IDS_BOForEOFofmvarRsTZB), _T("EOF"));
				throw strTmp;
			}
		}
		
		//д��seq�ֶ�,�����Ժ��������,
		//��������֮��,��GetphsBOMandCrd�и���Ч������ʱ,
		//ֻҪ����������seq<>Null�ļ�¼,
		//���ɻ������Ҫ�����AutoCAD�����ϸ��ļ�¼,
		//�����ȫ�ų��˶Ի�������������Ч����ͳ��
		rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp);
		
		if( !rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + vtos(vTmp) + _T("\'")), 0, adSearchForward, vTmp))
		{
			strTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(vTmp));
			ShowMessage(strTmp);
		}
		else
		{
			rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp);
			if( vtoi(vTmp)== iGCement )
			{
				COleVariant nul;
				nul.ChangeType(VT_NULL);
				rsTmpZB->put_Collect((_variant_t)_T("SEQ"),nul);
			}
			else
			{
				//�͸ָ���,����һ�ǿյ�����ֵ
				//���ɣ�GetphsBHandSizes�����Ѿ�ȷ����Щ�������Ա��
				//rsTmpZB.Fields(_T("SEQ")) = i
			}
		}
		rsTmpZB->put_Collect((_variant_t)_T("CLdz"),COleVariant(mvarSATotalWeight));
		rsTmpZB->get_Collect((_variant_t)_T("CLnum"), &vTmp);
		rsTmpZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(mvarSATotalWeight * vtof(vTmp)));
		rsTmpZB->Update();
		
		//���һ��ѭ��������˨��ĸֱ����ȷ��ʱ
	}
	catch(CString e)
	{
		ShowMessage(e);
	}
	catch(CException *e)
	{
		e->Delete();
	}

	if(rsTmpZB->State == adOpenStatic)
		rsTmpZB->Close();

	if(NewPtypeClassID!=NULL)
		delete [] NewPtypeClassID;
	if(NewPtypeIndex!=NULL)
		delete [] NewPtypeIndex;
	if(Ptype!=NULL)
		delete [] Ptype;
	if(PtypeCustomID!=NULL)
		delete [] PtypeCustomID;
	if(PtypeClassID!=NULL)
		delete [] PtypeClassID;
	if(PtypeIndex!=NULL)
		delete [] PtypeIndex;
	if(AttachmentGroupHavingF4!=NULL)
		delete [] AttachmentGroupHavingF4;
	if(iRecNo!=NULL)
		delete [] iRecNo;
	if(NewPtypeCustomID!=NULL)
		delete [] NewPtypeCustomID;
	if(NewPtypeID!=NULL)
		delete [] NewPtypeID;
	if(iRecNoOfLugDia!=NULL)
		delete [] iRecNoOfLugDia;
	if(iLugDia!=NULL)
		delete [] iLugDia;
	if(iBoltsNutsDia!=NULL)
		delete [] iBoltsNutsDia;
}

void Cphs::GetphsBlkRotation()
{
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
long Cphs::CheckMatchPhs()
{
	//���ݶ���SourceObj(ListBox or Recordset)�а�����֧����������,ȷ�������Ƿ��ܳɹ�����
	//Input:SourceObj�а�����֧����������
	//Output:mvarResultObj�б����װ���˿����ӵ����
	//�������������Ժ֧�����ֲ���ƹ����ж�û�л�����˿����ϡ�
	//����:�ɹ�Ϊ-1,���򷵻س���λ������
	long ret=0;
	CString* Ptype=NULL;
	try
	{
		_RecordsetPtr rs, rs1;
		rs.CreateInstance(__uuidof(Recordset));
		rs1.CreateInstance(__uuidof(Recordset));
		long IC=0, i=0, j=0 , k=0;bool  FlgMatch=0;
		long iNumL5 ;CString tbn1 ;long iClassPA=0, iNumRod =0;
		bool bLastPartIsSA=false ; //���һ�����Ӧ���Ǹ���
		if(SourceObj==NULL || !IsWindow(SourceObj->m_hWnd))
			return 0;
		IC=SourceObj->GetCount();
		Ptype=new CString [IC];
		if( IC > 1 ){
			//�����װ�㲿����������1
			//�ٶ����һ��������Ǹ���
			bLastPartIsSA = false;
			CString lstText;
			for (i = 0 ;i<IC;i++)
			{
				SourceObj->GetText(i,lstText);
				Ptype[i] = modPHScal::sFindID(Trim(lstText));
				tbn1 = modPHScal::sFindTBN(Trim(lstText));
				//��װ������˿�Ĺ���δ�ڻ���Ժ�ֲ��й᳹һ�£����������������жϺδ���Ҫ���軨����˿�����㡣
				//���㻨����˿�����������������Ժ֧�����ֲ�涨�������ܵĸ�����ܲ�֮�������˳��ȵ��ڴ�ʩʱӦ��װ�軨����˿��
				if( i == 0 ){
					iClassPA = modPHScal::glClassID;
				}else if( i == IC - 1 ){
					if( modPHScal::glIDIndex == iSA ){
						bLastPartIsSA = true;
					}
				}
				if( modPHScal::glClassID == iROD ) iNumRod = iNumRod + 1;
				//����ǵ��ܣ�������˿����
				if( iClassPA == iPAh ){
					if( modPHScal::sFindID(Ptype[i]) == _T("L5") ){
						iNumL5 = iNumL5 + 1;
					}
				}
			}
			CString SQLx,sTmp;
			COleVariant vTmp;
			if( bLastPartIsSA ){
//				rs.m_pDatabase=&EDIBgbl::dbPRJ;
				for (i = 0;i<= IC - 2;i++)
				{
					//����ÿһ������ƥ��
					FlgMatch = false;
					//�ӵ�һ����ʼ����ƥ��
					SQLx = _T("SELECT * FROM connect WHERE trim(cntb)=\'") + Ptype[i] + _T("\' AND  trim(cnte)=\'") + Ptype[i + 1] + _T("\'");
//					rs.Open(dbOpenSnapshot,SQLx);
					rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
						adOpenDynamic, adLockReadOnly, adCmdText); 
					if( rs->adoEOF && rs->BOF ){
						FlgMatch = false;
						break;
					}
					else if( ! rs->adoEOF )
					{
						rs->MoveLast();
						rs->MoveFirst();
						for( j = 0 ;j<rs->GetRecordCount();j++)
						{
							//3����3�����ϵ����,ƥ���������ϸ�
							rs->get_Collect((_variant_t)_T("cntn"), &vTmp);
							if( vtos(vTmp)==_T("")){
								FlgMatch = true;
								break;
							}else {
								if( i + 2 <= IC - 2 ){
									if( vtos(vTmp) == Ptype[i + 2] ){
										FlgMatch = true;
										break;
									}
								}else {
									FlgMatch = true;
									break;
								}
							}
							rs->MoveNext();
						}
						if( j >= rs->GetRecordCount() ){
							FlgMatch = false;
							break;
						}
					}
					
					//������û��ƥ��,���˳�
					if( ! FlgMatch ) break;
					rs->Close();
				}
				
				if( i == IC - 1 ){
					if( iClassPA == iPAh && iNumL5 > 0 && iNumRod > 0 ){
						//�������ܵĸ�����ܲ�֮�������˳��ȵ��ڴ�ʩʱӦ��װ�軨����˿��
						ret = -1;
						AfxMessageBox(GetResStr(IDS_PhsAssembleSuccess),MB_OK,0);
					}else {
						//��װ������˿�Ĺ���δ�ڻ���Ժ�ֲ��й᳹һ�£����������������жϺδ���Ҫ���軨����˿�����㡣
						ret = -1;
						//MsgBox ResolveResString(iUE_LossL5)
						AfxMessageBox(GetResStr(IDS_PhsAssembleSuccess),MB_OK,0);
					}
				}else {
					ret = i;
					//MsgBox _T("֧������װʧ��!��������λ����") & Ptype(i) & _T("��") & Ptype(i + 1) & _T("֮�䡣")
					sTmp.Format(GetResStr(IDS_PhsAssembleFailed),modPHScal::sFindCustomID(Ptype[i]), modPHScal::sFindCustomID(Ptype[i + 1]));
					AfxMessageBox(sTmp,MB_OK,0);
				}
			}else {
				//������һ��������Ǹ���
				//��Ϊ�����Ա㱨��
				//MsgBox _T("֧������װʧ��!(���һ��������Ǹ���)")
				ret= 0;
				AfxMessageBox(GetResStr(IDS_PhsAssembleFailedLastPartIsNotSA),MB_OK,0);
			}
		}else {
			//�����װ�㲿������������1
			//��Ϊ�����Ա㱨��
			//MsgBox _T("֧������װʧ��!(�����̫��)")
			ret = 0;
			AfxMessageBox(GetResStr(IDS_PhsAssembleFailedToolittlePart),MB_OK,0);
		}
		
	}
	catch(CException *e)
	{
		e->Delete();
		ret=0;
	}
	if(Ptype!=NULL)
		delete [] Ptype;
	return ret;
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::CheckDuplicateIndex()
{
	//���sort.mdb�еı�phsStructureName,
	//��֤��SampleIDֵΨһ.
	//���ڳ�����߼�����,�ɱ�֤SampleIDֵ��1���ε���,�м䲻��ȱ��
	//Screen.MousePointer = vbHourglass
	AfxGetApp()->BeginWaitCursor();
	_RecordsetPtr rs, rs1;
	rs.CreateInstance(__uuidof(Recordset));
	rs1.CreateInstance(__uuidof(Recordset));

	CString sTmp;
	try{
		EDIBgbl::SQLx = _T("SELECT * FROM phsStructureName");
		//ʹ��rs1�����GetphsAssembleName�е�ģ�鼶����rs��ͻ
//		rs1.m_pDatabase=&EDIBgbl::dbPHScode; //20071101 "dbSORT" ��Ϊ "dbPHScode"
//		rs1.Open(dbOpenDynaset,EDIBgbl::SQLx);
		rs1->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		EDIBgbl::SQLx = _T("SELECT * FROM phsStructureREF");
//		rs.m_pDatabase=&EDIBgbl::dbPHScode; //20071101 "dbSORT" ��Ϊ "dbPHScode"
//		rs.Open(dbOpenSnapshot,EDIBgbl::SQLx);
		rs->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		
		COleVariant v;
		if(rs1->BOF&& rs1->adoEOF)
		{
			sTmp=GetResStr(IDS_NoAnyRecordInphsStructureName)+GetResStr(IDS_NoAnyRecordInphsStructureName);
			ShowMessage(sTmp);
			goto errH;
		}
		if( !rs1->adoEOF)
		{
			rs1->MoveLast();
			rs1->MoveFirst();
			while(!rs1->adoEOF)
				//for (int i = 0 ;i<rs1->GetRecordCount();i++)
			{
				rs1->get_Collect((_variant_t)_T("SampleID"),v);
				sTmp.Format(_T("%d"),vtoi(v));
				sTmp=CString(_T("SampleID="))+sTmp+_T("");
				//��phsStructureREF��û�з���phsStructureName��
				//���ڵ�SampleID������,��ɾ��phsStructureName�е����SampleID��¼
				_variant_t vTmp;
				if(! rs->Find((_bstr_t)sTmp, 0, adSearchForward, vTmp))
				{
					rs1->Delete(adAffectCurrent);
				}
				rs1->MoveNext();
			}
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
errH:
	AfxGetApp()->EndWaitCursor();
	if(rs->State == adOpenStatic)
		rs->Close();
	if(rs1->State == adOpenStatic)
		rs1->Close();
}

long Cphs::CheckDuplicateREFRecord()
{
    return (long)0;
}

long Cphs::CheckInitRecord()
{
    return (long)0;
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
long Cphs::CheckDuplicateREFRecordWhenAppend(int *ipCheckButton)   //LFX  2005.3.24  ��
{
	//Ŀ�ģ����SampleID��Ψһ�ԡ�
	//���sort.mdb�еı�phsStructureREF,��֤Ҫ�����֧��������������ϲ�ͬ
	//Ҳ����˵,�κβ�ͬ��SampleID���в�ͬ��֧�������
	//input:SourceObj����(listBox)�к��д�������е�֧�������
	//output:�ɹ�,���ؼ�����е�֧������ϵ���������;����,����-1
	//���������Դ���phsStructureName��Ϊ�ռ�¼�����
	long ret=-1;
	// CString *sSampleID=NULL;
	// CString *DupRec=NULL;
	try
	{
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));

		long IC =0;
		long k=0,m=0,n=0;
		long i=0;
		
		//�ҳ�phsStructureREF������seq=0�ļ�¼,��SampleID����
		CString SQLx,sTmp;
		COleVariant vTmp;
		IC=SourceObj->GetCount();
		if(IC<1)
			throw;
		long Ci=0;
		for(i=0;i<IC;i++)
		{
			SourceObj->GetText(i,sTmp);
			sTmp=modPHScal::sFindID(Trim(sTmp));
			if(i==0)
			{
				//������SQL���ʮ�־��ʣ���ȥ�������¼�Ƚϵ�ѭ�����̣���Լ��ʱ�䡣�����ע2001.12.20
				SQLx=(_T("Select SampleID  From phsStructureREF phsStructureREFX Where SEQ = 0 AND ID = \'") + sTmp + _T("\'"));
			}
			else 
			{
				SQLx+=CString((_T(" AND SampleID IN \n( ")))+(_T("Select SampleID From phsStructureREF Where SEQ = ") + ltos(i) +_T(" AND ID = \'") + sTmp + _T("\'"));
				Ci++;
			}
		}
		sTmp=_T("");
		for(i=0;i<Ci;i++)
			sTmp+=(_T(")"));
		SQLx+=sTmp;
		SQLx+=(_T(" AND NOT EXISTS ( Select * From phsStructureREF Where  SEQ >= ")+ ltos(IC) + _T(" AND SampleID = phsStructureREFX.SampleID )"));
//		rs1.m_pDatabase=&EDIBgbl::dbPHScode;//20071101 "dbSORT" ��Ϊ "dbPHScode"
//		rs1.Open(dbOpenDynaset,SQLx);
		rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		CString DupRec;
		if(!rs1->adoEOF && !rs1->BOF)
		{
			m=1;
			rs1->get_Collect((_variant_t)_T("SampleID"), &vTmp);
			DupRec=vtos(vTmp);
		}
		else
			m=0;
		rs1->Close();
		
		//��m=0,�����ظ���¼
		//rs2.m_pDatabase=&EDIBgbl::dbSORT;
		if( m == 0 )
		{
			//��ȡ��δʹ�õ�SampleID��,��ֵ��Ϊ���µ�SampleID
			SQLx = _T("SELECT * FROM phsStructureREF WHERE Seq=0 ORDER BY SampleID");
//			rs1.Open(dbOpenDynaset,SQLx);
			rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if( rs1->BOF && rs1->adoEOF )
			{
				//MsgBox ResolveResString(iUE_NoAnyRecordInphsStructureName)
				//Exit Function
				IC = 0;
			}
			else if( ! rs1->adoEOF )
			{
				rs1->MoveLast();
				rs1->MoveFirst();
				for (k = 1 ;k<= rs1->GetRecordCount();k++)
				{
					rs1->get_Collect((_variant_t)_T("SampleID"), &vTmp);
					IC =vtoi(vTmp);
					//�����¼������SampleID���ȣ�˵����������
					//���ȱ��SampleID�ſ���
					if( k != IC ) 
						break;
					rs1->MoveNext();
				}
				//���k > rs2.RecordCount,˵��SampleID������
				//���һ��SampleID��+1����
				if( k > rs1->GetRecordCount() )
				{
					IC = k - 1;
				}
				else
				{
					//���õ�SampleID-1=IC
					IC = IC - 2;
				}
			}
			rs1->Close();
			modPHScal::iSelSampleID = IC + 1;
			sTmp.Format(_T("%04d"),modPHScal::iSelSampleID);
			SQLx = _T("INSERT INTO phsStructureName (FREQUENCE,SampleID,BmpName,bAllowUse) VALUES (1,") + ltos(modPHScal::iSelSampleID) + _T(",\'") + sTmp + _T("\',-1)");
			//������빲��⣬�Ա㼯�й���
			EDIBgbl::dbPHScode->Execute((_bstr_t)(SQLx), NULL, adCmdText);//20071101 "dbSORT" ��Ϊ "dbPHScode"
			//ͬʱ���뱾�ؿ��У��Ա��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)(SQLx), NULL, adCmdText);
			for( k = 0 ;k<SourceObj->GetCount();k++)
			{
				SourceObj->GetText(k,sTmp);
				SQLx = _T("INSERT INTO phsStructureREF (SampleID,SEQ,ID,CustomID,FREQUENCE) VALUES (")+
					ltos(modPHScal::iSelSampleID)+_T(",")+ltos(k)+_T(", \'") +modPHScal::sFindID(Trim(sTmp)) +_T("\' , \'") +Trim(sTmp) + _T("\' , 1)");
				//������빲��⣬�Ա㼯�й���
				EDIBgbl::dbPHScode->Execute((_bstr_t)(SQLx), NULL, adCmdText);//20071101 "dbSORT" ��Ϊ "dbPHScode"
				//ͬʱ���뱾�ؿ��У��Ա��������
				EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
			}
			ret = modPHScal::iSelSampleID;
			sTmp.Format(GetResStr(IDS_NewPhsSampleID), ltos(modPHScal::iSelSampleID));

 			// LFX 2005.3.24 �� �����û�ѡ���˷ǡ�ȫ��ģ�顱 ��ѡ��ʱ����ʾ�û��Ƿ�
			//��ǰģ����Ϊ����ģ���Ĭ�ϳ���ģ��
			SQLx.Empty();
			if (ipCheckButton != NULL && *ipCheckButton == 0) 
			{
				//���ѡ���� Ĭ�ϳ���ģ�� ��ѡ��
				sTmp = sTmp + "\n���Ҫ���½�ģ����Ϊ Ĭ�ϳ���ģ�� ��?";
				if (AfxMessageBox(sTmp,MB_YESNO) == IDYES)
				{
					SQLx.Format("UPDATE phsStructureName SET Favorite = TRUE,DefaultFavoriteTemplate = TRUE WHERE SampleID = %d",modPHScal::iSelSampleID);
				}
				else
				{
					*ipCheckButton = 2;      //���õ�ѡ��Ϊ ȫ��ģ��
				}
			}
			else if (ipCheckButton != NULL && *ipCheckButton == 1)
			{
				//���ѡ���� Ĭ��ģ�� ��ѡ��
				sTmp = sTmp + "\n���Ҫ���½�ģ����Ϊ ����ģ�� ��?";
				if (AfxMessageBox(sTmp,MB_YESNO) == IDYES)
				{
					SQLx.Format("UPDATE phsStructureName SET Favorite = TRUE WHERE SampleID = %d",modPHScal::iSelSampleID);
				}
				else
				{
					*ipCheckButton = 2;      //���õ�ѡ��Ϊ ȫ��ģ��
				}
			}
			else
			{
				//���ѡ���� ȫ��ģ�� ��ѡ��
				ShowMessage(sTmp);  				
			}

			if (ipCheckButton != NULL && (*ipCheckButton == 0 || * ipCheckButton == 1))
			{
				//������빲��⣬�Ա㼯�й���
				EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071101 "dbSORT" ��Ϊ "dbPHScode"
				//ͬʱ���뱾�ؿ��У��Ա��������
				EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
			}
 


			//ShowMessage(sTmp);  LFX ȥ��

			//END  LFX 2005.3.24 �� �����û�ѡ���˷ǡ�ȫ��ģ�顱 ��ѡ��ʱ����ʾ�û��Ƿ�

		}
		else 
		{
			//MsgBox _T("֧������������������Ѿ�����. ģ���Ϊ") & sSampleID(i)
			// LFX 2005.3.25 �� ���ģ���Ѵ��ڣ���ʾ�û��Ƿ������鿴��ģ��
			bool bDefFavTem;   //�Ƿ�ΪĬ�ϳ���ģ��
			bool bFavTem;      //�Ƿ�Ϊ����ģ��

			sTmp.Format(GetResStr(IDS_ExistsThisSampleIDinSampleREF), EDIBgbl::dbSORT->DefaultDatabase, DupRec);  //ԭ

			SQLx.Format(_T("SELECT * FROM phsStructureName WHERE SampleID = %d"),_ttoi(DupRec));  
//			rs1.Open(dbOpenDynaset,SQLx);			
			rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			rs1->get_Collect((_variant_t)_T("DefaultFavoriteTemplate"), &vTmp);
			bDefFavTem = vtob(vTmp);
			rs1->get_Collect((_variant_t)_T("Favorite"), &vTmp);
			bFavTem = vtob(vTmp);

			if (bDefFavTem)
			{
				sTmp = sTmp + _T("\n��ǰģ���� Ĭ�ϳ���ģ�� ���У�Ҫ�����鿴��ģ��������");
			}
			else if (bFavTem)
			{
				sTmp = sTmp + _T("\n��ǰģ���� ����ģ�� ���У�Ҫ�����鿴��ģ��������");
			}
			else
			{
				sTmp = sTmp + _T("\n��ǰģ���� ����ģ�� ���У�Ҫ�����鿴��ģ��������");
			}

			//ShowMessage(sTmp);  LFX ȥ��
			if (AfxMessageBox(sTmp,MB_YESNO) == IDYES)
			{
				modPHScal::iSelSampleID = _ttoi(DupRec);
				if (bDefFavTem)
				{
					*ipCheckButton = 0;
				}
				else if (bFavTem)
				{
					*ipCheckButton = 1;
				}
				else
				{
					*ipCheckButton = 2;
				}
			}
			ret = _ttoi(DupRec);
		}
	}
	catch(CString e)
	{
		ShowMessage(e);
		ret=-1;
	}
	catch(CException *e)
	{
		e->Delete();
		ret = -1;
	}
	return ret;
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::CheckAllCustomIDandIDinPhsStructureREF()
{
	//Ŀ�ģ�������е�CustomID�ֶΣ�����ת��ΪID�ֶΡ�����ID�ֶ���Ϊ�˱���PhsStructureREF����ģ��ŵ�Ψһ��/ͨ���ԡ�
	//���еĲ��ұ��־���IDΪ����.
	
	
	//��������գ������ٶȱȵ���modPHScal::sFindID��sFindCustomID�졣
	_RecordsetPtr rs, rsX;
	CString sTmp;
	COleVariant v,vTmp;
	CString sf1,sf2,sf3,sTmp1,stmp2;
	frmStatus.ShowWindow(SW_SHOW);
	//������㷨��phsStructureREFΪ���ģ����δ��������������ǧ�����ٶȺ�����
	/*
	try
	{
	//CString SQLx = _T("SELECT CustomID,ID  FROM PictureClipData");
	//rsID.m_pDatabase=&EDIBgbl::dbPRJ;
	//rsID.Open(dbOpenSnapshot,SQLx);
	if(!rsID->State == adOpenStatic)
	{
	EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
	rsID.m_pDatabase=&EDIBgbl::dbPRJ;
	rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
	brsIDStatus=TRUE;
	}
	
	  EDIBgbl::SQLx = _T("SELECT SampleID,seq,CustomID,ID  FROM phsStructureREF ORDER BY SampleID,seq");
	  rsX.m_pDatabase=&EDIBgbl::dbSORT;
	  rsX.Open( dbOpenDynaset ,EDIBgbl::SQLx);   
	  if( rsX->adoEOF && rsX->BOF ) 
	  {
	  sTmp=GetResStr(IDS_NoAnySEQeqZeroRecordInphsStructureREF);
	  ShowMessage(sTmp);
	  goto errH;
	  }
	  rsX->MoveLast();
	  int iC2 = rsX->GetRecordCount();
	  rsX->get_Collect((_variant_t)_T("SampleID"),v);
	  int iCSampleID =vtoi(v);
	  rsX->MoveFirst();
	  frmStatus.m_Label1=GetResStr(IDS_CheckAllCustomIDandIDinPhsStructureREF);
	  CString CustomID,ID;
	  for(int j = 0 ;j< iC2 ;j++)
	  {
	  AfxGetApp()->BeginWaitCursor();
	  //Screen.MousePointer = vbHourglass
	  //��������ʾһ��ʱ��
	  rsX->get_Collect((_variant_t)_T("CustomID"),v);
	  CustomID=vtos(v);
	  if(rsID->Find((_bstr_t)CString(_T("Trim(CustomID)=\'")) + CustomID + _T("\'")))
	  {
	  rsID->get_Collect((_variant_t)_T("ID"),v);
	  ID=vtos(v);
	  if (ID== _T("") )
	  {
	  rsX->get_Collect((_variant_t)_T("ID"),v);
	  ID=vtos(v);
	  if(rsID->Find((_bstr_t)CString(_T("Trim(ID)=\'")) + ID + _T("\'")))
	  //���Ϊ�գ�����Ȼ���ܴ�CustomIDת��ΪID,���������任��
	  {
						rsID->get_Collect((_variant_t)_T("CustomID"),v);
						CustomID=vtos(v);
						if( CustomID != _T("") )
						//��ȻΪ�գ�����һ������
						//msgbox
						{
						rsX.Edit();
						rsX->put_Collect((_variant_t)_T("CustomID"),v);
						rsX->Update();
						}
						}
						
						  //��ID-CustomID���ձ���û�з��������㲿��
						  //msgbox
						  }
						  else
						  {
						  rsID->get_Collect((_variant_t)_T("ID"),v);
						  rsX.Edit();
						  rsX->put_Collect((_variant_t)_T("ID"),v);
						  rsX->Update();
						  }
						  }
						  else
						  {
						  //���Ϊ�գ�����Ȼ���ܴ�CustomIDת��ΪID,���������任��
						  rsX->get_Collect((_variant_t)_T("ID"),v);
						  ID=vtos(v);
						  if(rsID->Find((_bstr_t) CString(_T("Trim(ID)=\'")) + ID + _T("\'")))
						  {
						  //���Ϊ�գ�����Ȼ���ܴ�CustomIDת��ΪID,���������任��
						  rsID->get_Collect((_variant_t)_T("CustomID"),v);
						  CustomID=vtos(v);
						  if(CustomID!=_T(""))
						  {
						  rsX.Edit();
						  rsX->put_Collect((_variant_t)_T("CustomID"),v);
						  rsX->Update();
						  }
						  else
						  {
						  }
						  }
						  else
						  {
						  //��ID-CustomID���ձ���û�з��������㲿��
						  }
						  }
						  rsX->get_Collect((_variant_t)_T("SampleID"),v);
						  sf1.Format(_T("%d"),vtoi(v));
						  rsX->get_Collect((_variant_t)_T("seq"),v);
						  sf2.Format(_T("%d"),vtoi(v));
						  sTmp1.Format(_T("%d"),iCSampleID);
						  stmp2.Format(_T("%d"),iC2);
						  //��%s��ģ��%s�����,��%s��ģ���%s�����CustomID=%s
						  frmStatus.m_Label2.Format(GetResStr(IDS_CheckNoXCustomIDandIDinPhsStructureREF),sTmp1,stmp2,sf1,sf2,CustomID);
						  frmStatus.UpdateStatus((float)(j+1)/(float)iC2,true);
						  frmStatus.UpdateData(false);
						  
							rsX->MoveNext();
							}
							}
*/

//�ı��㷨������ٶ�
//������㷨��PictureClipDataΪ���ģ����δ������������ֻ��138�����ٶȷǳ��졣
#pragma warning( disable : 4129 )  //Added by Shuli Luo
try
{
	AfxGetApp()->BeginWaitCursor();
	
	CString SQLx = _T("SELECT CustomID,ID  FROM PictureClipData WHERE index=") + ltos(iPA) + _T(" OR index=") + ltos(iSA)+ _T(" OR index=") +ltos(iConnectPART);
//	rsX.m_pDatabase=&EDIBgbl::dbPRJ;
//	rsX.Open(dbOpenSnapshot,SQLx);
	rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenDynamic, adLockReadOnly, adCmdText); 
	if( rsX->adoEOF && rsX->BOF ) 
	{
		sTmp=GetResStr(IDS_NoAnySEQeqZeroRecordInphsStructureREF);
		ShowMessage(sTmp);
		goto errH;
	}
	else
	{
		frmStatus.m_Label1.Format(GetResStr(IDS_UpdateCustomIDinPhsStructureREF));
		
		rsX->MoveLast();
		int iC2=rsX->GetRecordCount();						
		for(int j=1;j<=iC2;j++)
		{
			rsX->get_Collect((_variant_t)_T("CustomID"),v);
			rsX->get_Collect((_variant_t)_T("ID"), &vTmp);
			SQLx = _T("UPDATE phsStructureREF SET CustomID=\'") + vtos(v)+ _T("\' WHERE ID=\'") + vtos(vTmp)+_T("\'");
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
			EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071018 "dbSORT" ��Ϊ "dbPHScode"
			//�Ѿ��滻
			frmStatus.m_Label2=_T("");
			frmStatus.UpdateStatus((float)j/(float)iC2,true);
			frmStatus.UpdateData(false);
			
			
			rsX->MovePrevious();
		}		
	}
}
catch(CException *e)
{
	e->Delete();
}
errH:
frmStatus.ShowWindow(SW_HIDE);
//AfxGetApp()->EndWaitCursor();

}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::ChangeNameInphsStructureName()
{
	//Ŀ�ģ���������ģ������
	//������ձ������м�¼���ٸ���ģ��ṹ������ģ�������Ᵽ����ʹ��Ƶ��ֵ��	
	
	//������㷨�����¼���ң��ٶ��Ѿ��Ż����Ⱥ���ע�͵��Ĵ�γ����öࡣ
	AfxGetApp()->BeginWaitCursor();
	long iNumOfSPR=0,SampleID=0,iNumCSPR=0,iNumSPR=0,iNumRod=0,IC=0;
	CString sPA,sSA,sSPR;	
	CString Ptype0,PtypeC,Ptype;
	CString sTmp1;
	CString strTmpName,msNamePA,msNameSA,msNameType;
	CString strTmp;
	int IC2=0;
	int j=0, iPos;
	CString sFl;	
	//ɾ����phsStructureName���м�¼,���ʹ��Ƶ��Ҳɾ���ˡ�FREQUENCEӦ�ñ��档
	
	CString sTmp,strSQL;
	_RecordsetPtr rs1, rs2, rs3, rs4;//rs1��rs2ȡ��ͬһ����phsStructureREF
	rs1.CreateInstance(__uuidof(Recordset));
	rs2.CreateInstance(__uuidof(Recordset));
	rs3.CreateInstance(__uuidof(Recordset));
	rs4.CreateInstance(__uuidof(Recordset));
// 	rs1.m_pDatabase=&EDIBgbl::dbPRJ;
// 	rs2.m_pDatabase=&EDIBgbl::dbPRJ;
// 	rs3.m_pDatabase=&EDIBgbl::dbPRJ;
// 	rs4.m_pDatabase=&EDIBgbl::dbPHScode;//20071018 "dbSORT" ��Ϊ "dbPHScode"
	//�򿪱���
// 	 rsPA(&EDIBgbl::dbPRJ);
// 	 rsSA(&EDIBgbl::dbPRJ);
// 	 rsPR(&EDIBgbl::dbPRJ);
// 	 rsSP(&EDIBgbl::dbPRJ);
// 	 rsCS(&EDIBgbl::dbPRJ);
	_RecordsetPtr rsPA, rsSA, rsPR, rsSP, rsCS;//rs1��rs2ȡ��ͬһ����phsStructureREF
	rsPA.CreateInstance(__uuidof(Recordset));
	rsSA.CreateInstance(__uuidof(Recordset));
	rsPR.CreateInstance(__uuidof(Recordset));
	rsSP.CreateInstance(__uuidof(Recordset));
	rsCS.CreateInstance(__uuidof(Recordset));

// 	rsPA.Open(dbOpenTable,_T("tmpCustomIDPA"));
// 	rsSA.Open(dbOpenTable,_T("tmpCustomIDSA"));
// 	rsPR.Open(dbOpenTable,_T("tmpCustomIDPART"));
// 	rsSP.Open(dbOpenTable,_T("tmpCustomIDSPR"));
// 	rsCS.Open(dbOpenTable,_T("tmpCustomIDCSPR"));
	rsPA->Open((_bstr_t)_T("tmpCustomIDPA"),_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenDynamic, adLockReadOnly, adCmdTable); 

	rsSA->Open((_bstr_t)_T("tmpCustomIDSA"),_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenDynamic, adLockReadOnly, adCmdTable); 
	
	rsPR->Open((_bstr_t)_T("tmpCustomIDPART"),_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenDynamic, adLockReadOnly, adCmdTable); 
	
	rsSP->Open((_bstr_t)_T("tmpCustomIDSPR"),_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenDynamic, adLockReadOnly, adCmdTable); 
	
	rsCS->Open((_bstr_t)_T("tmpCustomIDCSPR"),_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenDynamic, adLockReadOnly, adCmdTable); 
	
// 	rsPA.SetCurrentIndex(_T("Index1"));
// 	rsSA.SetCurrentIndex(_T("Index1"));
// 	rsPR.SetCurrentIndex(_T("Index1"));
// 	rsSP.SetCurrentIndex(_T("Index1"));
// 	rsCS.SetCurrentIndex(_T("Index1"));

	std::vector<COleVariant> vecID;
	std::vector<CString> vecDescription;
	std::vector<COleVariant> vecCustomID;
	std::vector<int> vecClassID;
	std::vector<COleVariant>::iterator iter;
	vecID.reserve(200);
	vecDescription.reserve(200);
	vecCustomID.reserve(200);
	vecClassID.reserve(200);

	
	CString sFind;
	COleVariant vTmp;
	int oldSampleID;
	
	long t1;
//	long t2;
	t1=::GetTickCount();		
	//2002.01.03����ʱ��31084ms<12413+55730ms,���ۺ���һ���ѯ�Ͽ�
	
	//Ԥ�����õ�ǰ�淶���õ�ģ��Ϊȫ�������ã�ֻҪ������һ�������bAllowUse=0����Ϊ������
	strSQL=_T("UPDATE PhsStructureName SET bAllowUse=0");
	EDIBgbl::dbPHScode->Execute((_bstr_t)strSQL, NULL, adCmdText);//20071018 "dbSORT" ��Ϊ "dbPHScode"



	if(rsID->State != adOpenStatic)
	{
		EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
		//if(Cavphs->rsID==NULL)
		//Cavphs->rsID.CreateInstance(__uuidof(Recordset));
		//Cavphs->rsID->CursorLocation=adUseClient;
		//Cavphs->rsID->Open(_variant_t(EDIBgbl::SQLx),(IDispatch*)EDIBgbl::dbPRJ,adOpenDynamic,adLockOptimistic,adCmdText);
//		rsID.m_pDatabase=&EDIBgbl::dbPRJ;
//		rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
		rsID->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		brsIDStatus=TRUE;
	}	
	try
	{
		while(!rsID->adoEOF)
		{
			rsID->get_Collect((_variant_t)_T("ID"), vTmp);
			vecID.push_back(vTmp);
			rsID->get_Collect((_variant_t)_T("CustomID"), vTmp);
			vecCustomID.push_back(vTmp);
			rsID->get_Collect((_variant_t)_T("Description"), vTmp);
			vecDescription.push_back(vtos2(vTmp));
			rsID->get_Collect((_variant_t)_T("ClassID"), vTmp);
			vecClassID.push_back(vtoi2(vTmp));
			rsID->MoveNext();	
		}


		EDIBgbl::SQLx=_T("SELECT SampleID FROM phsStructureREF WHERE SEQ=0");
//		rs2.Open(dbOpenSnapshot,EDIBgbl::SQLx);
		rs2->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		rs2->MoveLast();
		rs2->MoveFirst();
		IC2=rs2->GetRecordCount();
		rs2->Close();

		EDIBgbl::SQLx = _T("SELECT * FROM phsStructureREF ORDER BY SampleID ASC, SEQ ASC");
//		rs2.Open(dbOpenDynaset ,EDIBgbl::SQLx);
		rs2->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		CString strSQL = _T("select * from phsStructureNAME");
//		rs4.Open(dbOpenDynaset ,strSQL);
		rs4->Open((_bstr_t)strSQL,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		bool stop=false;
		int iNum=0;
		long iTimes=-1;//���㲿�����ñ�־�����˻�
		if( rs2->adoEOF && rs2->BOF )
		{
			//�յ�phsStructureREF
			ShowMessage(GetResStr(IDS_NoAnySEQeqZeroRecordInphsStructureREF));
			//goto errH;
		}
		else
		{
			if(!frmStatus.IsWindowVisible())
				frmStatus.ShowWindow(SW_SHOW);
			frmStatus.SetWindowText(GetResStr(IDS_InitializeAutoPHS));
			frmStatus.m_Label1 = GetResStr(IDS_ChangeNameInphsStructureName);
			EDIBgbl::SQLx = _T("SELECT * FROM phsStructureName");
//			rs3.Open(dbOpenDynaset ,EDIBgbl::SQLx);
			rs2->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			int fdL;
			FieldsPtr info;
//			rs3->get_Collect((_variant_t)_T("SampleName"),info);
			rs3->get_Fields(&info);
			fdL=info->GetCount();	
			sSPR = _T(" ");
			//�ƶ����ṹ��ͷ
			rs2->MoveFirst();
			while(!rs2->adoEOF)
			{
				AfxGetApp()->BeginWaitCursor();
				rs2->get_Collect((_variant_t) _T("SampleID"), &vTmp);
				SampleID =vtoi( vTmp);
				oldSampleID=SampleID;
				sTmp1.Format(_T("%d"),SampleID);
				EDIBgbl::SQLx = CString(_T("SampleID=")) + sTmp1;
				iNumSPR=0;
				iNumCSPR=0;
				iNumRod=0;
				stop=false;
				iNum=0;
				do
				{
					rs2->MoveNext();
					if(rs2->adoEOF)
					{
						stop=true;
					}
					else
					{
						rs2->get_Collect((_variant_t) _T("SampleID"), &vTmp);
						SampleID =vtoi( vTmp);
						if(SampleID!=oldSampleID)
							stop=true;
					}
					rs2->MovePrevious();

					rs2->get_Collect((_variant_t) _T("SampleID"), &vTmp);
					SampleID =vtoi( vTmp);
					
					rs2->get_Collect((_variant_t)_T("ID"), &vTmp);
					Ptype=vtos(vTmp);
					if(iNum==0)
					{
						//�ܲ�
						Ptype0=Ptype;
						iter = std::find (vecID.begin(),vecID.end(),vTmp);
						if(iter!=vecID.end())
						{
							iPos = (iter - vecID.begin());

							msNamePA = vecDescription[iPos];

//							if(rsPA->Seek(_T("="),&vecCustomID[iPos]))

							rsPA->Seek(vecCustomID[iPos], adSeekAfter);
							if(!rsPA->adoEOF)
								//��ǰ��׼��������ܲ�
								iTimes=-1;
							else
								//��ǰ��׼��û������ܲ�
								iTimes=0;
						}
						else
						{
							//sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData), _T("ID"), Ptype);
							msNamePA=_T("");
							iTimes=0;
						}
						iNum++;
						rs2->MoveNext();
						continue;
					}
					else 
					{
						if(stop)
						{
							//����
							PtypeC=Ptype;
							iter = std::find(vecID.begin(), vecID.end(), vTmp);
							if(iter!=vecID.end())
							{
								iPos = (iter - vecID.begin());
								msNameSA = vecDescription[iPos];
//								if(rsSA.Seek(_T("="),&vecCustomID[iPos]))
								rsPA->Seek(vecCustomID[iPos], adSeekAfter);
								if(!rsPA->adoEOF)
									//��ǰ��׼��������ܲ�
									iTimes=iTimes*(-1);
								else
									//��ǰ��׼��û������ܲ�
									iTimes=0;
							}
							else
							{
								//sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData), _T("ID"), Ptype);
								msNameSA=_T("");
								iTimes =0;
							}
							
							if( iNumSPR > 0 )
							{
								strTmpName = msNamePA + modPHScal::gsPhsTypeSPR + msNameSA;
							}
							else
							{
								if( iNumCSPR == 0 )
								{
									strTmpName = msNamePA + modPHScal::gsPhsTypeRigid + msNameSA;
								}
								else
								{
									strTmpName = msNamePA + modPHScal::gsPhsTypeCSPR + msNameSA;
								}
							}
							
							iNum++;
							break;
						}
						else
						{
							//�����м��������
							iter = std::find(vecID.begin(), vecID.end(), vTmp);
							if(iter!=vecID.end())
							{
								iPos = (iter - vecID.begin());
								int ix = vecClassID[iPos];
								if(ix==iSPR)
								{
									iNumSPR++;
//									if(rsSP.Seek(_T("="),&vecCustomID[iPos])) //��ǰ��׼����������� 
									rsSP->Seek(vecCustomID[iPos], adSeekAfter);
									if(!rsPA->adoEOF)
										iTimes =iTimes*(-1);
									else
										//��ǰ��׼��û���������
										iTimes =0;
								}
								else if(ix==iCSPR)
								{
									iNumCSPR++;
//									if(rsCS.Seek(_T("="),&vecCustomID[iPos]))
									rsCS->Seek(vecCustomID[iPos], adSeekAfter);
									if(rsPA->adoEOF)
										//��ǰ��׼���������������
										iTimes =iTimes*(-1);
									else
										//��ǰ��׼��û�������������
										iTimes =0;
								}
								else if(ix==iROD)
								{
									iNumRod++;
								}
								else
								{
									rsPR->Seek(vecCustomID[iPos], adSeekAfter);
									if(rsPR->adoEOF)										//��ǰ��׼����������Ӽ�
										iTimes =iTimes*(-1);
									else
										//��ǰ��׼��û��������Ӽ�
										iTimes =0;
								}
								
							}
							else
							{
								iTimes =0;
							}
							iNum++;
							rs2->MoveNext();
							continue;
						}
					}

				}while(!stop);

				
				//��ʼ��ģ������
				//������ģ�������е�ʹ��Ƶ����Ϣ

				EDIBgbl::SQLx=_T("SampleID=")+sTmp1;
				
				if(!rs3->Find((_bstr_t)EDIBgbl::SQLx, 0, adSearchForward, vTmp))
				{
					rs3->AddNew();

					rs3->put_Collect((_variant_t)_T("SampleID"),_variant_t(SampleID));
					sTmp1.Format(_T("%05d"),SampleID);
					rs3->put_Collect((_variant_t)_T("BmpName"),STR_VAR(sTmp1));					

					if(strTmpName.GetLength() > fdL)						
						strTmp=strTmpName.Left(fdL);
					else
						strTmp=strTmpName;

					rs4->AddNew();
					rs4->put_Collect((_variant_t)_T("SampleID"), COleVariant(SampleID));
					rs4->put_Collect((_variant_t)_T("BmpName"), COleVariant(sTmp1, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("SampleName"), COleVariant(strTmp, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("PA"), COleVariant(Ptype0, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("SA"), COleVariant(PtypeC, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("SPR"), COleVariant(sSPR, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("iNumOfPart"), COleVariant((long)iNum));
					rs4->put_Collect((_variant_t)_T("iNumSPR"), COleVariant(iNumSPR));
					rs4->put_Collect((_variant_t)_T("iNumCSPR"), COleVariant(iNumCSPR));
					rs4->put_Collect((_variant_t)_T("iNumRod"), COleVariant(iNumRod));
					rs4->put_Collect((_variant_t)_T("bAllowUse"), COleVariant((long)iTimes));
					rs4->Update();

				}
				else
				{
//					rs3.Edit();

					rs4->Find((_bstr_t)EDIBgbl::SQLx, 0, adSearchForward, vTmp);

					if(strTmpName.GetLength() > fdL)
						strTmp=strTmpName.Left(fdL);
					else
						strTmp=strTmpName;
//					rs4.Edit();
					rs4->put_Collect((_variant_t)_T("SampleName"), COleVariant(strTmp, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("PA"), COleVariant(Ptype0, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("SA"), COleVariant(PtypeC, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("SPR"), COleVariant(sSPR, VT_BSTRT));
					rs4->put_Collect((_variant_t)_T("iNumOfPart"), COleVariant((long)iNum));
					rs4->put_Collect((_variant_t)_T("iNumSPR"), COleVariant(iNumSPR));
					rs4->put_Collect((_variant_t)_T("iNumCSPR"), COleVariant(iNumCSPR));
					rs4->put_Collect((_variant_t)_T("iNumRod"), COleVariant(iNumRod));
					rs4->put_Collect((_variant_t)_T("bAllowUse"), COleVariant((long)iTimes));
					rs4->Update();

				}
				{
					//�����ʹ��������������Ϊ��
					if(strTmpName.GetLength()>0 )
					{
						//rs3->Fields->Item[_variant_t(_T("SampleName"))]->get_DefinedSize(&fdL);
						if(strTmpName.GetLength() > fdL)
							strTmp=strTmpName.Left(fdL);
						else
							strTmp=strTmpName;
						rs3->put_Collect((_variant_t)_T("SampleName"),STR_VAR(strTmp)); 
					}
					if(Ptype0.GetLength()>0)
						rs3->put_Collect((_variant_t)_T("PA"), STR_VAR(Ptype0));
					if(PtypeC.GetLength()>0)
						rs3->put_Collect((_variant_t)_T("SA"),STR_VAR(PtypeC));
					if(sSPR.GetLength()>0)
						rs3->put_Collect((_variant_t)_T("SPR"),STR_VAR( sSPR));
					rs3->put_Collect((_variant_t)_T("iNumOfPart"),_variant_t((long)iNum));
					rs3->put_Collect((_variant_t)_T("iNumSPR"),_variant_t(iNumSPR));
					rs3->put_Collect((_variant_t)_T("iNumCSPR"),_variant_t(iNumCSPR));
					rs3->put_Collect((_variant_t)_T("iNumRod"),_variant_t(iNumRod));
					//ģ���Ƿ�����ʹ��
					rs3->put_Collect((_variant_t)_T("bAllowUse"),_variant_t( iTimes));
					rs3->Update();
//					EXECUTE_TIME_END
				}     
				sTmp1.Format(GetResStr(IDS_ChangeNoXNameInphsStructureName),(_T("\%d")),(_T("\%d")),strTmpName);
				frmStatus.m_Label2.Format(sTmp1, IC2, oldSampleID);
				frmStatus.UpdateData(false);
				frmStatus.UpdateStatus( (float)oldSampleID/(float)IC2, TRUE);
				frmStatus.UpdateWindow();
				rs2->MoveNext();
			}
			rs2->Close();
			rs3->Close();
			rs4->Close();
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
	//errH:
	AfxGetApp()->EndWaitCursor();
	if(rs2->State == adOpenStatic)
	rs2->Close();
	if(rs3->State == adOpenStatic)
	rs3->Close();
	//	timee=CTime::GetCurrentTime();
	//CTimeSpan t=timee-timeb;
	//CString s=t.Format(_T("%M��%S��"));
	//s=_T("����ʱ:")+s;
	//ShowMessage(s);
	frmStatus.ShowWindow(SW_HIDE);
}


#pragma warning( disable : 4129 )  //Added by Shuli Luo
_RecordsetPtr Cphs::AvailableSampleIDrsForphsSamp(CString strOrderByDesc,CString strFilter)
{
	//������frmphsSamp��������ݿؼ�ѡ������
	CString tmpSQL;
	_RecordsetPtr ret;
	ret.CreateInstance(__uuidof(Recordset));
	if(ret->State == adOpenStatic)
		ret->Close();
// 	ret->m_strFilter=_T("");
// 	ret->m_strSort=_T("");
// 	ret->m_pDatabase=&EDIBgbl::dbPRJ;
	tmpSQL=CString((_T("SELECT * FROM phsStructureName ")));
	if(Trim(strFilter)!=_T(""))
		tmpSQL +=_T("WHERE ") + strFilter ;
	if(Trim(SortFieldName) !=_T(""))
		tmpSQL+=CString((_T(" ORDER BY "))) + (_T("  [")) + Trim(SortFieldName) + (_T("] ")) + strOrderByDesc;
//	ret->Open(dbOpenDynaset,tmpSQL);
	ret->Open((_bstr_t)tmpSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenDynamic, adLockReadOnly, adCmdText); 
	return ret;
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::GetMaterial()
{
	//����ѡ��淶��SpecificationOfMaterialλ��sort.mdb��
	//�ñ�������������¶ȡ�������ѹ��(��������ˮ�ܵ�����ѡ��)����ŵȣ�
	//���ݸñ�С�������ԭ���Լ�Ĭ�ϲ��ϣ��ͻ��������¶�te(C,����������������������¶�GBJ19,�������糧��ˮ�ܵ���Ƽ����涨DL/T5054-1996,P76,��7.5.2.2��)����Ψһȷ��֧���ܹܵ�
	try
	{
		CString sTmp;
		COleVariant vTmp;
//		 rs(&EDIBgbl::dbPHScode);//20071018 "dbSORT" ��Ϊ "dbPHScode"
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(_Recordset));
		//���Ϲ淶ѡ���SpecificationOfMaterial
		CString SQLx ;
		SQLx.Format(_T("SELECT * FROM SpecificationOfMaterial WHERE ClassIndex=%d AND ID=\'default\' AND tmin<=%g AND %g<tmax ORDER BY tmin,tmax,SEQ"),modPHScal::giClassIndex,modPHScal::gnConnectTJ,modPHScal::gnConnectTJ);
//		rs.Open(dbOpenSnapshot,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if(rs->adoEOF && rs->BOF)
		{
			//��%s��%s����ѡ��淶��û��%s�ֶ�ֵΪĬ��ֵ%s�ļ�¼(�ǹܲ��������ѡ�����)
			sTmp.Format(IDS_NoDefaultInCustomIDInSpecificationOfMaterial,EDIBgbl::dbPHScode->DefaultDatabase,_T("SpecificationOfMaterial"),_T("ID"),_T("default"));
			throw sTmp;
		}
		rs->MoveFirst();
		//���в��ǹܲ�����Ų�Ϊnull���Ҳ�������ֵ�����,����϶����ɻ��������¶Ȳ��
		//�û���ӵĲ���bUserAdd!=0�����Ϊnull
		SQLx=CString(_T("UPDATE ["))  
			+ EDIBgbl::Btype[EDIBgbl::TZB] 
			+ _T("] SET CLcl=\'");
		rs->get_Collect((_variant_t)_T("Material"), &vTmp);
		SQLx+=vtos(vTmp);
		SQLx+=_T("\' WHERE zdjh=") ;
		sTmp.Format(_T("%d"),modPHScal::zdjh);
		SQLx+=sTmp 
			+ _T(" AND VolumeID=")
			+ ltos(EDIBgbl::SelVlmID) 
			+ _T("  AND ( ClassID<>");
		sTmp.Format(_T("%d"),iPAfixZ1);
		SQLx+=sTmp
			+ _T(" AND ClassID<>");
		sTmp.Format(_T("%d"),iPAfixZ2);
		SQLx+=sTmp
			+_T(" AND ClassID<>");
		sTmp.Format(_T("%d"),iPAs);
		SQLx+=sTmp
			+_T(" AND ClassID<>");
		sTmp.Format(_T("%d"),iPAh);
		SQLx+=sTmp		  
			+_T(") AND (ISNULL(CLcl) or trim(CLcl)=\'\') ");//AND NOT ISNULL(seq)";
		EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText); 
	}
	catch(CException *e)
	{
		e->Delete();
	}
	
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::GetphsSEQ(_RecordsetPtr /*ByVal*/ rsza)
{
	//Ŀ�ģ�����������,ͬ�����������ͬ���
	//input:  dbsort�еı�CLtemperature,
	//        dbprjdb�е�F####?-JZB,
	//output:
	//        dbprj�е�TmpCLgroup,
	try
	{
		_RecordsetPtr rs, rsTmp2,rsTmpZB;
		_RecordsetPtr rsCL, rsTmpCLgroup;
		rs.CreateInstance(__uuidof(Recordset));
		rsTmp2.CreateInstance(__uuidof(Recordset));
		rsTmpZB.CreateInstance(__uuidof(Recordset));
		rsCL.CreateInstance(__uuidof(Recordset));
		rsTmpCLgroup.CreateInstance(__uuidof(Recordset));

		long i;
		//		bool bFoundSA;// As Boolean
		long mvIndex =0; //������
		CString tbn , tmpBlkID , tmpID ;
		CString mvCL, sGROUPBY ,tmpStr;
		CString *Ptype=NULL;
		//		float sTmp=0, sngSumWeight=0;
		
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")))
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
		//����һ���ձ�
		sGROUPBY = _T("CLgg,CLcl,CLmc,CLID,CLdw,CLdz");
		EDIBgbl::dbPRJDB->Execute((_bstr_t)(_T("SELECT ") + sGROUPBY + _T(" INTO TmpCLgroup IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJ->DefaultDatabase + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=-1")), NULL, adCmdText);
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�,
		//����seq,CLnum���ܱ�����,��Ҫ�������seq,CLnum�ֶε���ʱ��
		EDIBgbl::dbPRJ->Execute((_bstr_t)_T("ALTER TABLE TmpCLgroup ADD COLUMN seq short"), NULL, adCmdText);
		EDIBgbl::dbPRJ->Execute((_bstr_t)_T("ALTER TABLE TmpCLgroup ADD COLUMN CLnum SINGLE"), NULL, adCmdText);
		//���򡢷���ͳ�������
		EDIBgbl::dbPRJDB->Execute((_bstr_t)(_T("INSERT INTO TmpCLgroup IN \'") + EDIBgbl::dbPRJ->DefaultDatabase + _T("\' SELECT DISTINCT ") + sGROUPBY + _T(" FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(modPHScal::zdjh) + _T(" AND NOT ISNULL(seq)  GROUP BY ") + sGROUPBY), NULL, adCmdText);
		
		sGROUPBY = _T("seq,CLgg,CLcl,CLmc,CLID,CLdw,CLdz,CLnum,CLzz,CLbz");
		
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("Tmp2")))
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE tmp2"), NULL, adCmdText);
		CString SQLx;
		//�����ձ�tmp2
		SQLx = _T("SELECT ") + sGROUPBY + _T(" INTO tmp2 IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJ->DefaultDatabase + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=-1");
		EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
		SQLx = _T("DELETE FROM tmp2");
		EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
//		rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
//		rsTmp2.Open(dbOpenDynaset,_T("SELECT * FROM tmp2") );
		rsTmp2->Open((_bstr_t)_T("SELECT * FROM tmp2"),_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		
		//��֧���ܲ��ϻ��ܹ���ת�Ƶ������Ĺ�����GetPhsBOM���Ա����ͳ�Ƶ�ѡ��ı�֮���ܹ���ȷ���ɲ��ϻ��ܱ�
		//����ɾ����֧���ܲ�������
		SQLx = _T("DELETE * FROM [") +EDIBgbl:: Btype[EDIBgbl::TCL] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
		EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
//		rsCL.m_pDatabase=&EDIBgbl::dbPRJDB;
//		rsCL.Open(dbOpenDynaset,SQLx);
		rsCL->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		
		//���Ҵӹܲ����������θ�������
		//ͬʱ���������ϸ��tmp2�Ͳ��ϻ��ܱ�F????S-JCL
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND NOT ISNULL(seq) ORDER BY recno");
		if(rsTmpZB->State == adOpenStatic)
			rsTmpZB->Close();
//		rsTmpZB.m_pDatabase=&EDIBgbl::dbPRJDB;
//		rsTmpZB.Open(dbOpenDynaset,SQLx);
		rsCL->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 

		if(rsTmpZB->adoEOF && rsTmpZB->BOF)
		{
			//����ṹ��û���κ����
			tmpStr.Format(GetResStr(IDS_NoRecordInTZB),EDIBgbl::dbPRJDB->DefaultDatabase,EDIBgbl::Btype[EDIBgbl::TZB],
				EDIBgbl::SelJcdm,ltos(modPHScal::zdjh));
			throw tmpStr;
		}
		i = 1;
		COleVariant vTmp;
//		rsTmpCLgroup.m_pDatabase=&EDIBgbl::dbPRJ;
		COleVariant nil;
		nil.ChangeType(VT_NULL);
		while(!rsTmpZB->adoEOF)
		{
			CString CLgg,CLcl,CLmc;
			rsTmpZB->get_Collect((_variant_t)_T("CLgg"), &vTmp);
			CLgg=vtos(vTmp);
			rsTmpZB->get_Collect((_variant_t)_T("CLcl"), &vTmp);
			CLcl=vtos(vTmp);
			rsTmpZB->get_Collect((_variant_t)_T("CLmc"), &vTmp);
			CLmc=vtos(vTmp);
			//�յ�seq�ֶΣ�д���Ÿ���
			SQLx=_T("UPDATE tmpCLgroup SET seq=") + ltos(i) + _T(" WHERE ISNULL(seq) AND trim(CLgg)=\'");
			SQLx+=CLgg+_T("\' AND trim(CLcl)=\'");
			SQLx+=CLcl+_T("\' AND (trim(CLmc))=\'") ;
			SQLx+=CLmc+_T("\'");
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
			//Debug.Print dbPRJ.RecordsAffected, i, rsTmpZB(_T("CLmc")), rsTmpZB(_T("CLgg")), rsTmpZB(_T("CLcl"))
			//SQLx = _T("SELECT seq FROM tmpCLgroup INNER JOIN [") + EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] AS tbn ON  WHERE ISNULL(seq) AND trim(CLgg)=\'")
			//	 +CLgg + _T("\' AND trim(CLcl)=\'") +CLcl + _T("\' AND (trim(CLmc))=\'") +CLmc+ _T("\' ");
			// SQLx = _T("SELECT * FROM tmpCLgroup WHERE NOT ISNULL(seq) AND trim(CLgg)='") & Trim(rsTmpZB.Fields(_T("CLgg"))) & _T("' AND trim(CLcl)='") & Trim(rsTmpZB.Fields(_T("CLcl"))) & _T("' AND (trim(CLmc))='") & Trim(rsTmpZB.Fields(_T("CLmc"))) & _T("' ")
			SQLx = _T("SELECT seq FROM tmpCLgroup WHERE NOT ISNULL(seq) AND trim(CLgg)=\'")
				+ CLgg+ _T("\' AND trim(CLcl)=\'") +CLcl+ _T("\' AND (trim(CLmc))=\'") +CLmc+ _T("\' ");
//			rsTmpCLgroup.Open(dbOpenSnapshot,SQLx);
			rsTmpCLgroup->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if( rsTmpCLgroup->BOF && rsTmpCLgroup->adoEOF)
			{
				tmpStr.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),EDIBgbl::dbPRJ->DefaultDatabase,_T("tmpCLgroup"),SQLx);
				throw tmpStr;
			}
			rsTmpCLgroup->MoveLast();
			//�м�¼�ܵ�Ӱ�죬��ŵ�����
//			if(EDIBgbl::dbPRJ.GetRecordsAffected()>0)//�������һ�θ��¡��༭�����Ӳ�����Execute������Ӱ��ļ�¼��
//			if(EDIBgbl::dbPRJ->()>0)//�������һ�θ��¡��༭�����Ӳ�����Execute������Ӱ��ļ�¼��
//				i++;
			rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp);
			if(vtoi(vTmp)!= iGCement)
			{
				//���ǻ�������
				//�������е������д����������,����������Ϊ�յ��������Ҫ���
				mvIndex = vtoi(modPHScal::sFindFLD(_T("CustomID"), _T("Index"), vtos(vTmp)));
//				rsTmpZB.Edit();
				rsTmpCLgroup->get_Collect((_variant_t)_T("seq"), &vTmp);
				if(mvIndex != iSA)
				{
					//���Ǹ���
					rsTmpZB->put_Collect((_variant_t)_T("seq"),vTmp);
				}
				else
				{
					if(!vtob(rsza->GetCollect(_T("bNotSumSA"))))
					{
						//�Ǹ���,Ҫͳ�Ƹ�������
						rsTmpZB->put_Collect((_variant_t)_T("seq"),vTmp);
					}
					else
						//�Ǹ���,��Ҫͳ�Ƹ�������
						rsTmpZB->put_Collect((_variant_t)_T("seq"),nil);
				}
				rsTmpZB->Update();
				
				//д�����ϸ��Tmp2
				rsTmp2->AddNew();
				rsTmpZB->get_Collect((_variant_t)_T("seq"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("seq"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLID"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLID"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLmc"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLmc"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLgg"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLgg"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLcl"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLcl"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLdw"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLdw"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLdz"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLdz"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLnum"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLnum"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLzz"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLzz"),vTmp);
				rsTmpZB->get_Collect((_variant_t)_T("CLbz"), &vTmp);
				rsTmp2->put_Collect((_variant_t)_T("CLbz"),vTmp);
				rsTmp2->Update();
			}
			rsTmpZB->MoveNext();
			rsTmpCLgroup->Close();
		}
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(CString e)
	{
		ShowMessage(e);
	}
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::GetphsSumBom()
{
	//Ŀ�ģ����ɰ����û���ӵĲ������ڵĲ��ϻ��ܱ����ݱ���gbSumAllVolume�����ǵ����������о�ᣩ
	//���������ˡ������������������ܲ����������Ӽ�������㡣
	//input:  rzTZB
	//        dbPrjDB�е�ZB,
	//   ---�������л�ĳ�����֧���ܵĲ��ϻ��ܱ�:
	//   ---ֱ�Ӵ�ZB������ȡ��������,������*��ZA�е���������=���ϻ��ܱ��е�����,д��AllPrjDb.mdb�еĵ�ǰ����CL��.
	//	  ---���������ļ���ԭ�����£�
	//	�����recno=null,
	//		������û���ӵĲ��ϣ���������=�û������������
	//		����
	//			���֧���ܼ����ѳɹ�
	//			����֧���ܼ���δ�ɹ���
	//				�����������Ҫͳ��
	//					��������=��������Gnum�ֶ�*��֧��������Num�ֶΣ�
	//				����������ϲ�ͳ��
	//					��������=0��
	//	������recno=null,
	//   ---
	//output:���д��dbprjdb�е�TCL��CL
	//   ---ͬʱ��������������Ľ��д��Tmp2���Ա������������
	//
	try
	{
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));

		_RecordsetPtr rsza;
		rsza.CreateInstance(__uuidof(Recordset));

		_RecordsetPtr rsCL;
		rsCL.CreateInstance(__uuidof(Recordset));

		COleVariant v;
		float sTmp=0, sngSumWeight;
		long m_iNum=0,m_iCLNum=0,m_iSANum=0,m_iL1=0;
		bool m_bNotSumSA=false;
		bool m_bSuccess=false;
		bool m_bUserAdd=false;
		CString strTmp;
		CString mvCL ;
		CString sGROUPBY;
		long iSEQ=0;  //���
		long iSEQsub=0; //�ӱ��
		CString SQLx;
		SQLx.Format(_T("DELETE FROM [%s]  tb1 WHERE NOT EXISTES ( SELECT ZDJH FROM [%s]  tb2 WHERE tb1.VolumeID=tb2.VolumeID AND tb1.ZDJH=tb2.ZDJH )"),
			EDIBgbl::Btype[EDIBgbl::TZB],EDIBgbl::Btype[EDIBgbl::TZA]);
		EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
		//ȷ��Ҫ������ֶ�:����ֶ���Ϣ���Դ�TableINFO���t??����,���ҿ����û���
		sGROUPBY = _T("seq,CLgg,CLcl,CLmc,CLID,CLdw,CLdz,CLbz");
		//��dbprj�н�����ʱ������ϻ��ܱ�
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpTCL")) )
		{
			//WorkPrj.mdb�еı�TmpTCL���ڱ��������ܵ��м���
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpTCL"), NULL, adCmdText);
		}
		//����TCL��TmpTCL���Ա����ɾ�����е�ѡ����Ĳ��ϡ�
		SQLx = _T("SELECT * INTO TmpTCL IN \'") + EDIBgbl::dbPRJ->DefaultDatabase + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] WHERE VolumeID=-1");
		EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
		if(rsID->State != adOpenStatic)
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
//			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
//			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
			rsID->Open((_bstr_t)EDIBgbl::SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			brsIDStatus=TRUE;
		}
		//��dbprj�����ɿյ�Tmp2��
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("Tmp2")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE Tmp2"), NULL, adCmdText);
		}
		CString strOrderBy;
		strOrderBy = sGROUPBY;
		//���ɿյ�Tmp2��
		EDIBgbl::dbPRJDB->Execute((_bstr_t)CString(_T("SELECT ") + sGROUPBY + _T(",CLnum,CLnum*CLdz AS CLzz INTO Tmp2 IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJ->DefaultDatabase + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] WHERE zdjh=0 AND VolumeID=-1")), NULL, adCmdText);
// 		rs.m_pDatabase=&EDIBgbl::dbPHScode;//20071101 "dbSORT" ��Ϊ "dbPHScode"
// 		rs.Open(dbOpenSnapshot,_T("SELECT * FROM BomName ORDER BY BomIndex"));
		rs->Open((_bstr_t)_T("SELECT * FROM BomName ORDER BY BomIndex"),_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		COleVariant vTmp,vTmp1,vTmp2;
		while(!rs->adoEOF)
		{
			rs->get_Collect((_variant_t)_T("BomTable"),vTmp);
			if(vTmp.vt!=VT_NULL)
			{
				strTmp =(_T("TMP") + vtos(vTmp));
				strTmp.MakeUpper();
				if( strTmp == _T("TMP2") )
				{
					//��ֹɾ��Tmp2,��Ϊ���ǲ��ϻ��ܱ�
				}
				else
				{
					//�������еĲ���������ܱ�
					if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, strTmp) )
					{
						//��TmpCLgroup���ڱ��������
						EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("DROP TABLE [") + strTmp + _T("]")), NULL, adCmdText);
					}
					//����tmp2�Ľṹ��������
					EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("SELECT * INTO [") + strTmp + _T("] FROM Tmp2")), NULL, adCmdText);
				}
			}
			rs->MoveNext();
		}
		rs->Close();   
		
		//��֧���ܲ��ϻ��ܹ���ת�Ƶ������Ĺ�����GetPhsSumBOM���Ա����ͳ�Ƶ�ѡ��ı�֮���ܹ���ȷ���ɲ��ϻ��ܱ�
		//����ɾ����֧���ܲ�������
		//strSelectVolume =(modPHScal::gbSumAllVolume ? (_T("")) : (_T(" VolumeID=\'") + EDIBgbl::SelVlmID + _T("\'")));
		SQLx = _T("DELETE FROM [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] ") ;
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") ");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID);
		}
		EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
		
		//ͳ�Ʋ���֮ǰ���Զ���δ����ɹ���֧�����Ҳ����û��ӵĲ����޳�
		//���ȣ�ѡ������û�м���ɹ���֧����
		SQLx=_T("DELETE FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE (bUserAdd IS NULL OR bUserAdd =0) AND NOT EXISTS ( SELECT VolumeID,Zdjh FROM ") +
			EDIBgbl::Btype[EDIBgbl::TZA] + _T(" WHERE [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("].VolumeID=[")
			+ EDIBgbl::Btype[EDIBgbl::TZB] + _T("].VolumeID AND [")+ EDIBgbl::Btype[EDIBgbl::TZA] + _T("].Zdjh = [")
			+ EDIBgbl::Btype[EDIBgbl::TZB] + _T("].Zdjh ) OR EXISTS  ( SELECT VolumeID,Zdjh FROM [") 
			+ EDIBgbl::Btype[EDIBgbl::TZA] +_T("] ")
			+ _T(" WHERE [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("].VolumeID=[") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("].VolumeID ")
			+ _T(" AND  [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("].zdjh=[") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("].zdjh AND [")
			+ EDIBgbl::Btype[EDIBgbl::TZA] + _T("].bCalSuccess=0 ");
		if(modPHScal::gbSumAllVolume )
		{
			SQLx +=_T(" AND [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("].VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")) ");
		}
		else
		{
			SQLx +=_T(" AND [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("].VolumeID =") + ltos(EDIBgbl::SelVlmID) + _T(")");
		}
		SQLx+=_T(" AND ( bUserAdd is  null OR bUserAdd =0 )");
		EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
		
		//��ʽ��ʼ���ܲ���
		SQLx = _T("SELECT * FROM TmpTCL");
// 		rsCL.m_pDatabase=&EDIBgbl::dbPRJ; 
// 		rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
		rsCL->Open((_bstr_t)_T("SELECT * FROM TmpTCL"),_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		
		//���ȼ����ܵ�Ҫ����ļ�¼��
		//�ܵ�Ҫ����ļ�¼���������ڽ�������ʾ��
		long iSum=0,iTmp=0;
		
		//��������
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
		if(modPHScal::gbSumAllVolume )
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") AND ClassID=") + ltos(iCSPR);
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) +_T(" AND ClassID=") + ltos(iCSPR);
		}
// 		rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 		rs.Open(dbOpenDynaset,SQLx );
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{
			
		}
		else
		{
			rs->MoveLast();
			iSum+=rs->GetRecordCount();
		}
		rs->Close();
		
		//���ڴ�����
		SQLx = _T("SELECT * FROM [") +  EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") AND ClassID=") + ltos(iSPR);
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) + _T(" AND ClassID=") + ltos(iSPR);
		}
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;
// 		rs.Open(dbOpenDynaset,SQLx );
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF ) 
		{	
		}
		else
		{
			rs->MoveLast();
			iSum+=rs->GetRecordCount();
		}
		rs->Close();
		
		//���ڴ���ܲ�
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") ");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
		}
		SQLx += _T(" AND ( Index=") + ltos(iPA) + _T(" )");
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{	}
		else
		{
			rs->MoveLast();
			iSum+=rs->GetRecordCount();
		}
		rs->Close();
		
		//���ڴ���ǵ������Ӽ������������ˣ�
		SQLx = CString(_T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + (_T("] ")) ;
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID);
		}
		SQLx+=_T(" AND ( Index=") + ltos(iConnectPART) + _T(" AND ClassID<>") + ltos(iROD) + _T(" AND ClassID<>") + ltos(iCSPR) + _T(" AND ClassID<>") + ltos(iSPR) + _T(" AND IsSAPart<>-1)");
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF ) {	}
		else
		{
			rs->MoveLast();
			iSum+=rs->GetRecordCount();
		}
		rs->Close();
		
		//ͳ�������������
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
		}
		SQLx+= _T(" AND ClassID=") + ltos(iROD);
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{	}
		else
		{
			rs->MoveLast();
			//����Ҫ����2��
			iSum+=2*rs->GetRecordCount();
		}
		rs->Close();
		
		//ѡ����������е��͸֡�����ĸ��˨�����
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ") ;
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
		}
		SQLx+=_T(" AND  IsSAPart=-1 AND ClassID<>") + ltos(iBolts) + _T(" AND ClassID<>") + ltos(iNuts);		
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{	}
		else
		{
			rs->MoveLast();
			//�͸�Ҫ����3��
			iSum+=3*rs->GetRecordCount();
		}
		rs->Close();
		
		//ͳ����˨��ĸ����������Ų�Ϊ�գ�Ϊ�յ����ڳ��ҹ�������Ҫͳ�ƣ��������ʹ�õ���˨��ĸ����������Ҫͳ��ʱ��
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ") ;
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
		}
		//AND����������������
		SQLx+=_T(" AND  ((ClassID=") + ltos(iBolts) + _T(" OR ClassID=") + ltos(iNuts) + _T(" OR ClassID=") + ltos(iAttached) + _T(") AND NOT IsNull(SEQ) OR (ClassID=") + ltos(iBolts) + _T(" OR ClassID=") + ltos(iNuts) + _T(") AND IsSAPart=-1) ");
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF ) 
		{	}
		else
		{
			rs->MoveLast();
			iSum+=rs->GetRecordCount();
		}
		rs->Close();
		
		//���Ҫ����������������
		if( modPHScal::gbOutputSA )
		{
			//����������������
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
			if(modPHScal::gbSumAllVolume)
			{
				SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
			}
			else
			{
				SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
			}
			SQLx += _T(" AND  Index=") + ltos(iSA) + _T(" AND ClassID<>") + ltos(iGCement);
// 			rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
			rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if( rs->adoEOF && rs->BOF )
			{	}
			else
			{
				rs->MoveLast();
				iSum+=rs->GetRecordCount();
			}
			rs->Close();
		}
		
		//��ʾ������
		if(!IsWindow(frmStatus.m_hWnd))
			frmStatus.Create(IDD_STATUS,AfxGetMainWnd());
		//frmStatus.ShowWindow(SW_SHOW);
		//frmStatus.UpdateWindow();
		frmStatus.ShowWindow(SW_SHOW);
		frmStatus.m_Label1 = GetResStr(IDS_SumPhsBom);
		frmStatus.m_Label2 = _T("");
		strTmp.Format(GetResStr(IDS_SumPhsBomTitle),(modPHScal::gbSumAllVolume ? GetResStr(IDS_CURRENT_PROJECT):GetResStr(IDS_CURRENT_VOLUME)));
		frmStatus.SetWindowText(strTmp);
		//frmStatus.Show
		frmStatus.UpdateData(false);
		frmStatus.UpdateStatus(0.0 / iSum,true);
		frmStatus.UpdateWindow();
		iTmp+=1;
		
		//************************
		//ͳ�ƹܲ������Ӽ������Լ�
		//���˸�������������������֮�����������
		//���ڴ����������
		COleVariant nil;
		nil.ChangeType(VT_NULL);
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
		if(modPHScal::gbSumAllVolume )
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") AND ClassID=") + ltos(iCSPR);
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) +_T(" AND ClassID=") + ltos(iCSPR);
		}
// 		rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 		rs.Open(dbOpenDynaset,SQLx );
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{
		}
		else
		{
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomCSPR);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				//SQLx = _T("SELECT * FROM [") +  EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(vTmp1) + _T(" AND zdjh=") + vtos(vTmp2);
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
// 				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				rsza->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					//iSEQ = iSEQ + 1
					rsCL->AddNew();
					rsCL->put_Collect((_variant_t)_T("seq"),nil);
					rs->get_Collect((_variant_t)_T("zdjh"),v);
					rsCL->put_Collect((_variant_t)_T("zdjh"),v);
					rs->get_Collect((_variant_t)_T("VolumeID"),v);
					rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
					rs->get_Collect((_variant_t)_T("CLID"),v);
					rsCL->put_Collect((_variant_t)_T("CLID"),v);
					rs->get_Collect((_variant_t)_T("CLmc"),v);
					rsCL->put_Collect((_variant_t)_T("CLmc"),v);
					rs->get_Collect((_variant_t)_T("CLgg"),v);
					rsCL->put_Collect((_variant_t)_T("CLgg"),v);
					rs->get_Collect((_variant_t)_T("CLcl"),v);
					rsCL->put_Collect((_variant_t)_T("CLcl"),v);
					rs->get_Collect((_variant_t)_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL->put_Collect((_variant_t)_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs->get_Collect((_variant_t)_T("CLdz"),v);
					rsCL->put_Collect((_variant_t)_T("CLdz"),v);					
					rs->get_Collect((_variant_t)_T("CLzz"),v);
					rsCL->put_Collect((_variant_t)_T("CLzz"),v);
					if( EDIBAcad::g_bSumBomSprFactory )//newHS->�Ƿ��ڲ��ϻ��ܱ��б�ע����/�������ɳ���
					{
						rs->get_Collect((_variant_t)_T("CLbz"),v);
						rsCL->put_Collect((_variant_t)_T("CLbz"),v);
					}
					rs->get_Collect((_variant_t)_T("CLnum"),v);
					rsza->get_Collect((_variant_t)_T("Num"), &vTmp1);
					rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(vtof(v)*vtof(vTmp1)));
					rsCL->Update();
				}
				rsza->Close();
				rs->MoveNext();
			}
		}
		rs->Close();
		//���̽��������ɴ�TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
		}
		//rsCL->Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJ->DefaultDatabase + _T("\' FROM TmpTCL GROUP BY ") + sGROUPBY), NULL, adCmdText);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")), NULL, adCmdText);
		//���������ɵ����ݿ�����TmpCSPR��
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpCSPR SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//ɾ����ʱ��
		RsDeleteAll(rsCL);
		//������ʱ���ϱ����ݼǶ���
		//rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
		
		//************
		//���ڴ�����
		SQLx = _T("SELECT * FROM [") +  EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") AND ClassID=") + ltos(iSPR);
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) + _T(" AND ClassID=") + ltos(iSPR);
		}
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{
		}
		else
		{
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomSPR);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
// 				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				rsza->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					rsCL->AddNew();
					rsCL->put_Collect((_variant_t)_T("seq"),nil);
					rs->get_Collect((_variant_t)_T("zdjh"),v);
					rsCL->put_Collect((_variant_t)_T("zdjh"),v);
					rs->get_Collect((_variant_t)_T("VolumeID"),v);
					rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
					rs->get_Collect((_variant_t)_T("CLID"),v);
					rsCL->put_Collect((_variant_t)_T("CLID"),v);
					rs->get_Collect((_variant_t)_T("CLmc"),v);
					rsCL->put_Collect((_variant_t)_T("CLmc"),v);
					rs->get_Collect((_variant_t)_T("CLgg"),v);
					rsCL->put_Collect((_variant_t)_T("CLgg"),v);
					rs->get_Collect((_variant_t)_T("CLcl"),v);
					rsCL->put_Collect((_variant_t)_T("CLcl"),v);
					rs->get_Collect((_variant_t)_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL->put_Collect((_variant_t)_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs->get_Collect((_variant_t)_T("CLdz"),v);
					rsCL->put_Collect((_variant_t)_T("CLdz"),v);					
					rs->get_Collect((_variant_t)_T("CLzz"),v);
					rsCL->put_Collect((_variant_t)_T("CLzz"),v);
					if( EDIBAcad::g_bSumBomSprFactory )//newhs->�Ƿ��ڲ��ϻ��ܱ��б�ע����/�������ɳ���
					{
						rs->get_Collect((_variant_t)_T("CLbz"),v);
						rsCL->put_Collect((_variant_t)_T("CLbz"),v);
					}
					rs->get_Collect((_variant_t)_T("CLnum"),v);
					rsza->get_Collect((_variant_t)_T("Num"), &vTmp1);
					rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(vtof(v)*vtof(vTmp1)));
					rsCL->Update();
				}
				rsza->Close();
				rs->MoveNext();
			}
		}
		rs->Close();
		//���̽����ɴ�TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
		}
		//rsCL->Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx =_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//�����ɵ����ݿ�����TmpSPR��
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpSPR SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO [") +  EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")), NULL, adCmdText);
		//ɾ����ʱ��
		RsDeleteAll(rsCL);
		//������ʱ���ϱ����ݼǶ���
		
		//************
		//���ڴ���ܲ�
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") ");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
		}
		SQLx += _T(" AND ( Index=") + ltos(iPA) + _T(" )");
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{
		}
		else
		{
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomPA);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
// 				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					rsCL->AddNew();
					rsCL->put_Collect((_variant_t)_T("seq"),nil);
					rs->get_Collect((_variant_t)_T("zdjh"),v);
					rsCL->put_Collect((_variant_t)_T("zdjh"),v);
					rs->get_Collect((_variant_t)_T("VolumeID"),v);
					rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
					rs->get_Collect((_variant_t)_T("CLID"),v);
					rsCL->put_Collect((_variant_t)_T("CLID"),v);
					rs->get_Collect((_variant_t)_T("CLmc"),v);
					rsCL->put_Collect((_variant_t)_T("CLmc"),v);
					rs->get_Collect((_variant_t)_T("CLgg"),v);
					rsCL->put_Collect((_variant_t)_T("CLgg"),v);
					rs->get_Collect((_variant_t)_T("CLcl"),v);
					rsCL->put_Collect((_variant_t)_T("CLcl"),v);
					rs->get_Collect((_variant_t)_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL->put_Collect((_variant_t)_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs->get_Collect((_variant_t)_T("CLdz"),v);
					rsCL->put_Collect((_variant_t)_T("CLdz"),v);					
					rs->get_Collect((_variant_t)_T("CLzz"),v);
					rsCL->put_Collect((_variant_t)_T("CLzz"),v);
					rs->get_Collect((_variant_t)_T("CLbz"),v);
					rsCL->put_Collect((_variant_t)_T("CLbz"),v);
					rs->get_Collect((_variant_t)_T("CLnum"),v);
					rsza->get_Collect((_variant_t)_T("Num"), &vTmp1);
					rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(vtof(v)*vtof(vTmp1)));
					rsCL->Update();
				}
				rsza->Close();
				rs->MoveNext();
			}
		}
		rs->Close();
		//���̽��ܲ���TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
		}
		//rsCL->Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup    FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//���ܲ����ݿ�����TmpPA��
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpPA SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")), NULL, adCmdText);
		//ɾ����ʱ��
		RsDeleteAll(rsCL);
		//������ʱ���ϱ����ݼǶ���
		
		//*********************************
		//���ڴ���ǵ������Ӽ������������ˣ�
		SQLx = CString(_T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + (_T("] ")) ;
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID);
		}
		SQLx+=_T(" AND ( Index=") + ltos(iConnectPART) + _T(" AND ClassID<>") + ltos(iROD) + _T(" AND ClassID<>") + ltos(iCSPR) + _T(" AND ClassID<>") + ltos(iSPR) + _T(" AND IsSAPart<>-1)");
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{
		}
		else
		{
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomNotSPRConnect);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
// 				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					rsCL->AddNew();
					rsCL->put_Collect((_variant_t)_T("seq"),nil);
					rs->get_Collect((_variant_t)_T("zdjh"),v);
					rsCL->put_Collect((_variant_t)_T("zdjh"),v);
					rs->get_Collect((_variant_t)_T("VolumeID"),v);
					rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
					rs->get_Collect((_variant_t)_T("CLID"),v);
					rsCL->put_Collect((_variant_t)_T("CLID"),v);
					rs->get_Collect((_variant_t)_T("CLmc"),v);
					rsCL->put_Collect((_variant_t)_T("CLmc"),v);
					rs->get_Collect((_variant_t)_T("CLgg"),v);
					rsCL->put_Collect((_variant_t)_T("CLgg"),v);
					rs->get_Collect((_variant_t)_T("CLcl"),v);
					rsCL->put_Collect((_variant_t)_T("CLcl"),v);
					rs->get_Collect((_variant_t)_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL->put_Collect((_variant_t)_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs->get_Collect((_variant_t)_T("CLdz"),v);
					rsCL->put_Collect((_variant_t)_T("CLdz"),v);					
					rs->get_Collect((_variant_t)_T("CLzz"),v);
					rsCL->put_Collect((_variant_t)_T("CLzz"),v);
					rs->get_Collect((_variant_t)_T("CLbz"),v);
					rsCL->put_Collect((_variant_t)_T("CLbz"),v);
					rs->get_Collect((_variant_t)_T("CLnum"),v);
					rsza->get_Collect((_variant_t)_T("Num"), &vTmp1);
					rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(vtof(v)*vtof(vTmp1)));
					rsCL->Update();
				}
				rsza->Close();
				rs->MoveNext();
			}
		}
		rs->Close();
		//���̽��ǵ������Ӽ���TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
		}
		//rsCL->Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//���ǵ������Ӽ�(����������)�����ݿ�����TmpConnection��
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpConnection SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO [") +EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")), NULL, adCmdText);
		//ɾ����ʱ��
		RsDeleteAll(rsCL);
		//������ʱ���ϱ����ݼǶ���
		
		
		//************
		//ͳ�������������
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
		}
		SQLx+= _T(" AND   ClassID=") + ltos(iROD);
// 		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{
			//û���κ�����
		}
		else
		{
			//���κ�����,RS-Բ��
			this->rsID->Find((_bstr_t)(_T("trim(ID)=\'RS\'")), 0, adSearchForward, vTmp);
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomRod);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
// 				rsza.m_pDatabase=&EDIBgbl::dbPRJDB; rsza.Open(dbOpenSnapshot,SQLx,dbReadOnly);
				rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					
					sTmp=((vTmp=GetFields(rsza,_T("num"))).vt==VT_NULL ? 1.0 : vtof(vTmp)) * ((vTmp=GetFields(rs,_T("CLnum"))).vt==VT_NULL ? 1.0 : vtof(vTmp));					
					sngSumWeight=sTmp * vtof(GetFields(rs,_T("GDW1"))) * vtof(GetFields(rs,_T("L1")) ) / 1000.0; //����1m���ر�����GDW1�ֶ�
					rsCL->AddNew();
					rsCL->put_Collect((_variant_t)_T("seq"),nil);
					rs->get_Collect((_variant_t)_T("zdjh"),v);
					rsCL->put_Collect((_variant_t)_T("zdjh"),v);
					rs->get_Collect((_variant_t)_T("VolumeID"),v);
					rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
					rs->get_Collect((_variant_t)_T("CLID"),v);
					rsCL->put_Collect((_variant_t)_T("CLID"),v);
					rs->get_Collect((_variant_t)_T("CLmc"),v);
					rsCL->put_Collect((_variant_t)_T("CLmc"),v);
					rs->get_Collect((_variant_t)_T("CLgg"),v);
					rsCL->put_Collect((_variant_t)_T("CLgg"),v);
					rs->get_Collect((_variant_t)_T("CLcl"),v);
					rsCL->put_Collect((_variant_t)_T("CLcl"),v);
					rs->get_Collect((_variant_t)_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL->put_Collect((_variant_t)_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs->get_Collect((_variant_t)_T("CLdz"),v);
					rsCL->put_Collect((_variant_t)_T("CLdz"),v);					
					rs->get_Collect((_variant_t)_T("CLbz"),v);
					rsCL->put_Collect((_variant_t)_T("CLbz"),v);
					rs->get_Collect((_variant_t)_T("GDW1"),v);
					rs->get_Collect((_variant_t)_T("L1"), &vTmp1);
					
					rsCL->put_Collect((_variant_t)_T("CLdz"),COleVariant(vtof(v) * vtof(vTmp1)/1000.0)); //����1m���ر�����GDW1�ֶ�
					rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(sTmp));
					rsCL->put_Collect((_variant_t)_T("CLzz"),COleVariant(sngSumWeight));
					rsCL->Update();
				}
				rsza->Close();
				rs->MoveNext();
			}
		}
		//���̽����������TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
		}
		//rsCL->Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		if(modPHScal::gbSumRodByRoundSteel)
			;
		else
		{
			//����tmpCLgroup��tmp2,�Ա㱣���������
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
			//�����˵����ݿ�����TmpConnection��
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpConnection SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		}
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")), NULL, adCmdText);
		//ɾ����ʱ��
		RsDeleteAll(rsCL);
		//������ʱ���ϱ����ݼǶ���
		
		//************
		//ͳ�����˳��ȣ���Բ��ֱ�����ࣩ
		//�ص���һ����¼
		if( rs->adoEOF && rs->BOF )
		{
			//û���κ�����
		}
		else
		{
			rs->MoveFirst();
			//���κ�����
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomRodLen);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
//				rsza.m_pDatabase=&EDIBgbl::dbPRJDB; rsza.Open(dbOpenSnapshot,SQLx,dbReadOnly);
				rsza->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					
					sTmp=((vTmp=GetFields(rsza,_T("num"))).vt==VT_NULL ? 1.0 : vtof(vTmp)) * vtof(GetFields(rs,_T("CLnum"))) * vtof(GetFields(rs,_T("L1"))) / 1000.0;
					sngSumWeight = sTmp * vtof(GetFields(rs,_T("GDW1")));  //����1m���ر�����GDW1�ֶ�
					//if( sTmp <> 0 ){ Debug.Print sTmp
					rsCL->AddNew();
					rsCL->put_Collect((_variant_t)_T("seq"),nil);
					rs->get_Collect((_variant_t)_T("zdjh"),v);
					rsCL->put_Collect((_variant_t)_T("zdjh"),v);
					rs->get_Collect((_variant_t)_T("VolumeID"),v);
					rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
					rs->get_Collect((_variant_t)_T("CLID"),v);
					rsCL->put_Collect((_variant_t)_T("CLID"),v);
					rsCL->put_Collect((_variant_t)_T("CLmc"),GetFields(this->rsID,_T("Description")));
					rsCL->put_Collect((_variant_t)_T("CLgg"),STR_VAR(CString(_T("d=")) +vtos(GetFields(rs,_T("sizeC")))));
					rsCL->put_Collect((_variant_t)_T("CLcl"),GetFields(rs,_T("CLcl")));
					CString str;
					str.LoadString(IDS_METER);
					rsCL->put_Collect((_variant_t)_T("CLdw"),(GetFields(rs,_T("Cldw")).vt==VT_NULL ?  STR_VAR(str) : GetFields(rs,_T("CLdw"))));
					//rs->get_Collect((_variant_t)_T("GDW1"), &vTmp); vTmp.vt==VT_NULL ? strTmp=_T("0") : strTmp.Format(_T("%.3f"),vtof(vTmp));rsCL->put_Collect((_variant_t)_T("CLdz"),STR_VAR(strTmp));
					
					rsCL->put_Collect((_variant_t)_T("CLdz"),GetFields(rs,_T("GDW1")));
					//rsCL->put_Collect((_variant_t)_T("CLdz")) = Format(IIf(IsNull(rsCL->put_Collect((_variant_t)_T("CLdz"))), 0, rsCL->put_Collect((_variant_t)_T("CLdz"))), _T("0.##"))
					rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(sTmp));
					rsCL->put_Collect((_variant_t)_T("CLzz"),COleVariant(sngSumWeight));
					rsCL->put_Collect((_variant_t)_T("CLbz"),GetFields(rs,_T("CLbz")));
					rsCL->Update();
				}
				rsza->Close();
				rs->MoveNext();
			}
		}
		rs->Close();
		//���̽����˲��ϴ�TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
		}
		//rsCL->Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		//EDIBgbl::dbPRJ->Execute((_bstr_t)_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy
		if(modPHScal::gbSumRodByRoundSteel)
		{
			//����tmpCLgroup��tmp2,�Ա㱣���������,tmp2�����ݼ�¼�����
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
			//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")), NULL, adCmdText);
		}
		
		//�����˵����ݿ�����TmpLugBom��
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpLugBom SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		//EDIBgbl::dbPRJ->Execute((_bstr_t)_T("INSERT INTO [") + EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")
		//ɾ����ʱ��
		RsDeleteAll(rsCL);
		//������ʱ���ϱ����ݼǶ���
		
		
		//******************
		//ͳ�Ƹ����͸�������
		//ѡ����������еķ���ĸ,��˨�����
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ") ;
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
		}
		SQLx+=_T(" AND IsSAPart=-1 AND ClassID<>") + ltos(iBolts) + _T(" AND ClassID<>") + ltos(iNuts);
//		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{
			//û���κη��͸�,����˨��ĸ�ȵĸ���
		}
		else
		{
			//���κη��͸�,����˨��ĸ�ȵ����
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomAttachements);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				//ֻѡ�����������Ҫͳ�Ƶ�֧����
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
//				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				rsza->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					
					if( this->rsID->Find((_bstr_t) (_T("trim(CustomID)=\'") + vtos(GetFields(rs,_T("CustomID"))) + _T("\'")), 0, adSearchForward, vTmp))
					{
						int intIndex=vtoi(GetFields(this->rsID,_T("Index")));
						if( intIndex==iSectionSteel )
						{
						}
						else
						{
							rsza->get_Collect((_variant_t)_T("num"),v);
							m_iNum=vtoi(v);
							rsza->get_Collect((_variant_t)_T("Gnum"),v);
							m_iSANum=vtoi(v);
							rsza->get_Collect((_variant_t)_T("bNotSumSA"),v);
							m_bNotSumSA=vtob(v);
							rs->get_Collect((_variant_t)_T("CLnum"),v);
							m_iCLNum=vtoi(v);
							rs->get_Collect((_variant_t)_T("bUserAdd"),v);
							m_bUserAdd=vtob(v);
							if(m_bUserAdd)
								sTmp=m_iNum*m_iSANum*m_iCLNum;
							else
							{
								if (!m_bNotSumSA)
									sTmp=m_iNum*m_iSANum*m_iCLNum;
								else
									sTmp=0;
							}
							sngSumWeight = sTmp * vtof( GetFields(rs,_T("CLdz")));
							rsCL->AddNew();
							rsCL->put_Collect((_variant_t)_T("seq"),nil);
							rs->get_Collect((_variant_t)_T("zdjh"),v);
							rsCL->put_Collect((_variant_t)_T("zdjh"),v);
							rs->get_Collect((_variant_t)_T("VolumeID"),v);
							rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
							rs->get_Collect((_variant_t)_T("CLID"),v);
							rsCL->put_Collect((_variant_t)_T("CLID"),v);
							rs->get_Collect((_variant_t)_T("CLmc"),v);
							rsCL->put_Collect((_variant_t)_T("CLmc"),v);
							rsCL->put_Collect((_variant_t)_T("CLgg"),GetFields(rs,_T("CLgg")));
							rsCL->put_Collect((_variant_t)_T("CLcl"),GetFields(rs,_T("CLcl")));
							//rsCL->put_Collect((_variant_t)_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(_T("��")) : vTmp));
							CString str;
							str.LoadString(IDS_PIECE);
							rsCL->put_Collect((_variant_t)_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(str) : vTmp));
							rsCL->put_Collect((_variant_t)_T("CLdz"),GetFields(rs,_T("CLdz")));
							//rsCL->put_Collect((_variant_t)_T("CLdz")) = Format(IIf(IsNull(rsCL->put_Collect((_variant_t)_T("CLdz"))), 0, rsCL->put_Collect((_variant_t)_T("CLdz"))), _T("0.##"))
							rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(sTmp));
							rsCL->put_Collect((_variant_t)_T("CLzz"),COleVariant(sngSumWeight));
							rsCL->put_Collect((_variant_t)_T("CLbz"),GetFields(rs,_T("CLbz")));
							rsCL->Update();
						}
					}
				}
				rsza->Close();
				rs->MoveNext();
			}
			//���̽�����ĸ,��˨����������TCL������ܵ�Tmp2
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
			{
				//��TmpCLgroup���ڱ��������
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
			}
			//rsCL->Close();
			//��tmp2����һ��������TmpCLgroup
			//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
			SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
			SQLx +=_T(" GROUP BY ") + sGROUPBY;
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
			//����tmpCLgroup��tmp2,�Ա㱣���������
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
			//������ĸ,��˨�������������ݿ�����TmpAttachment��
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpAttachment SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
			//����ʱ������ݿ�������ʽ���ϱ�TCL
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")), NULL, adCmdText);
			//ɾ����ʱ��
			RsDeleteAll(rsCL);
			//������ʱ���ϱ����ݼǶ���
			//rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
		}
		
		
		//�͸ְ�����ͳ�Ƶ���дһ����TmpSSBom
		if( rs->adoEOF && rs->BOF )
		{
			//û���͸�
		}else
		{
			rs->MoveFirst();
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomSSLen);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh"))) + _T(" AND  NOT bNotSumSA ");
//				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				rsza->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					
					if(this->rsID->Find((_bstr_t) (_T("trim(CustomID)=\'") + vtos(GetFields(rs,_T("CustomID"))) + _T("\'")), 0, adSearchForward, vTmp))
					{					
						if( vtoi(GetFields(this->rsID,_T("Index")))==iSectionSteel )
						{                  
							rsza->get_Collect((_variant_t)_T("num"),v);
							m_iNum=vtoi(v);
							rsza->get_Collect((_variant_t)_T("Gnum"),v);
							m_iSANum=vtoi(v);
							rsza->get_Collect((_variant_t)_T("bNotSumSA"),v);
							m_bNotSumSA=vtob(v);
							rs->get_Collect((_variant_t)_T("CLnum"),v);
							m_iCLNum=vtoi(v);
							rs->get_Collect((_variant_t)_T("bUserAdd"),v);
							m_bUserAdd=vtob(v);
							rs->get_Collect((_variant_t)_T("L1"),v);
							m_iL1=vtoi(v);
							if(m_bUserAdd)
								sTmp=m_iNum*m_iSANum*m_iCLNum*m_iL1/1000.0;
							else
							{
								if (!m_bNotSumSA)
									sTmp=m_iNum*m_iSANum*m_iCLNum*m_iL1/1000.0;
								else
									sTmp=0;
							}
							sngSumWeight = sTmp * vtof( GetFields(rs,_T("CLdz")));
							rsCL->AddNew();
							rsCL->put_Collect((_variant_t)_T("seq"),nil);
							rs->get_Collect((_variant_t)_T("zdjh"),v);
							rsCL->put_Collect((_variant_t)_T("zdjh"),v);
							rs->get_Collect((_variant_t)_T("VolumeID"),v);
							rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
							rs->get_Collect((_variant_t)_T("CLID"),v);
							rsCL->put_Collect((_variant_t)_T("CLID"),v);
							rs->get_Collect((_variant_t)_T("CLmc"),v);
							rsCL->put_Collect((_variant_t)_T("CLmc"),v);
							rs->get_Collect((_variant_t)_T("CLgg"),v);
							rsCL->put_Collect((_variant_t)_T("CLgg"),v);
							rsCL->put_Collect((_variant_t)_T("CLcl"),GetFields(rs,_T("CLcl")));
							CString str;
							str.LoadString(IDS_METER);
							rsCL->put_Collect((_variant_t)_T("CLdw"),(GetFields(rs,_T("Cldw")).vt==VT_NULL ?  STR_VAR(str) : GetFields(rs,_T("CLdw"))));
							rsCL->put_Collect((_variant_t)_T("CLdz"),GetFields(rs,_T("CLdz")));
							//rsCL->put_Collect((_variant_t)_T("CLdz")) = Format(IIf(IsNull(rsCL->put_Collect((_variant_t)_T("CLdz"))), 0, rsCL->put_Collect((_variant_t)_T("CLdz"))), _T("0.##"))
							rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(sTmp));
							rsCL->put_Collect((_variant_t)_T("CLzz"),COleVariant(sngSumWeight));
							rsCL->put_Collect((_variant_t)_T("CLbz"),GetFields(rs,_T("CLbz")));
							rsCL->Update();
						}
					}
				}
				rsza->Close();
				rs->MoveNext();
			}
			//���̽��͸ִ�TCL������ܵ�Tmp2
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
			{
				//��TmpCLgroup���ڱ��������
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
			}
			//rsCL->Close();
			//��tmp2����һ��������TmpCLgroup
			//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
			SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
			SQLx +=_T(" GROUP BY ") + sGROUPBY;
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
			//����tmpCLgroup��tmp2,�Ա㱣���������
			//�¾䲻���٣���ΪAutoCAD������ϱ�ʱ����Ҫ����GetphsSumBom������в��ϵļ��ϣ���ACAD�иü�����ԴΪtmp2��
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
			//���͸ֵ����ݿ�����TmpSSBom��
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpSSBom SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
			//����ʱ������ݿ�������ʽ���ϱ�TCL
			//EDIBgbl::dbPRJ->Execute((_bstr_t)_T("INSERT INTO [") + EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")
			//ɾ����ʱ��
			RsDeleteAll(rsCL);
			//������ʱ���ϱ����ݼǶ���
		}
		
		
		//�͸ְ����ͳ�Ƶ���дһ����TmpSS
		if( rs->adoEOF && rs->BOF )
		{
			//û���͸�
		}
		else
		{
			rs->MoveFirst();
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomSS);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh"))) + _T(" AND NOT bNotSumSA");
//				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				rsza->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					
					if(this->rsID->Find((_bstr_t) (_T("trim(CustomID)=\'") + vtos(GetFields(rs,_T("CustomID"))) + _T("\'")), 0, adSearchForward, vTmp))
					{
						//�͸�(��2001.12.31��ְ�PS�����͸�(index=3)��Ϊ����(index=4)
						if( (vtoi(GetFields(this->rsID,_T("Index")))==iSectionSteel) )
						{
							rsza->get_Collect((_variant_t)_T("Num"),v);
							m_iNum=vtoi(v);
							rsza->get_Collect((_variant_t)_T("Gnum"),v);
							m_iSANum=vtoi(v);
							rsza->get_Collect((_variant_t)_T("bNotSumSA"),v);
							m_bNotSumSA=vtob(v);
							rs->get_Collect((_variant_t)_T("CLnum"),v);
							m_iCLNum=vtoi(v);
							rs->get_Collect((_variant_t)_T("bUserAdd"),v);
							m_bUserAdd=vtob(v);
							rs->get_Collect((_variant_t)_T("L1"),v);
							m_iL1=vtoi(v);
							if(m_bUserAdd)
								sTmp=m_iNum*m_iSANum*m_iCLNum;
							else
							{
								if (!m_bNotSumSA)
									sTmp=m_iNum*m_iSANum*m_iCLNum;
								else
									sTmp=0;
							}
							sngSumWeight = sTmp * m_iL1/1000.0 * vtof( GetFields(rs,_T("CLdz"))) ;
							rsCL->AddNew();
							rsCL->put_Collect((_variant_t)_T("seq"),nil);
							rs->get_Collect((_variant_t)_T("zdjh"),v);
							rsCL->put_Collect((_variant_t)_T("zdjh"),v);
							rs->get_Collect((_variant_t)_T("VolumeID"),v);
							rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
							rs->get_Collect((_variant_t)_T("CLID"),v);
							rsCL->put_Collect((_variant_t)_T("CLID"),v);
							rs->get_Collect((_variant_t)_T("CLmc"),v);
							rsCL->put_Collect((_variant_t)_T("CLmc"),v);
							
							rsCL->put_Collect((_variant_t)_T("CLgg"),STR_VAR(vtos(GetFields(rs,_T("CLgg"))) + _T(" L=") + ltos(m_iL1)));
							rsCL->put_Collect((_variant_t)_T("CLcl"),GetFields(rs,_T("CLcl")));
							rsCL->put_Collect((_variant_t)_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(_T("��")) : vTmp));
							
							rsCL->put_Collect((_variant_t)_T("CLdz"),COleVariant(vtof(GetFields(rs,_T("CLdz"))) * m_iL1/1000.0));
							rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(sTmp));
							rsCL->put_Collect((_variant_t)_T("CLzz"),COleVariant(sngSumWeight));
							rsCL->put_Collect((_variant_t)_T("CLbz"),GetFields(rs,_T("CLbz")));
							rsCL->Update();
							
						}
					}
					
				}
				rsza->Close();
				rs->MoveNext();
			}
			//���̽��͸ִ�TCL������ܵ�Tmp2
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
			{
				//��TmpCLgroup���ڱ��������
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
			}
			//rsCL->Close();
			//��tmp2����һ��������TmpCLgroup
			//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
			SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
			SQLx +=_T(" GROUP BY ") + sGROUPBY;
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
			//����tmpCLgroup��tmp2,�Ա㱣���������
			//EDIBgbl::dbPRJ->Execute((_bstr_t)_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy
			//���͸ֵ����ݿ�����TmpSS��
			EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpSS SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
			//����ʱ������ݿ�������ʽ���ϱ�TCL
			//EDIBgbl::dbPRJ->Execute((_bstr_t)_T("INSERT INTO [") + EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")
			//ɾ����ʱ��
			RsDeleteAll(rsCL);
			//������ʱ���ϱ����ݼǶ���
		}
		rs->Close();
		//���Ҫ����������������
		if( modPHScal::gbOutputSA )
		{
			//����������������
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ");
			if(modPHScal::gbSumAllVolume)
			{
				SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
			}
			else
			{
				SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
			}
			//�������������������Զ������
			SQLx += _T(" AND  Index=") + ltos(iSA) + _T(" AND ClassID<>") + ltos(iGCement) + _T(" AND ClassID<>") + ltos(iG100);
//			rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
			rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if( rs->adoEOF && rs->BOF )
			{
			}
			else
			{
				while(!rs->adoEOF)
				{
					frmStatus.m_Label1= GetResStr(IDS_SumPhsBomSA);
					strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
					frmStatus.m_Label2=strTmp;
					frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
					frmStatus.UpdateData(false);
					iTmp+=1;
					
					SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh"))) + _T(" AND NOT bNotSumSA");
//					rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
					rsza->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
						adOpenDynamic, adLockReadOnly, adCmdText); 
					if( rsza->adoEOF && rsza->BOF )
					{
					}
					else
					{
						rsza->MoveFirst();
						rsCL->AddNew();
						rsCL->put_Collect((_variant_t)_T("seq"),nil);
						rs->get_Collect((_variant_t)_T("zdjh"),v);
						rsCL->put_Collect((_variant_t)_T("zdjh"),v);
						rs->get_Collect((_variant_t)_T("VolumeID"),v);
						rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
						rs->get_Collect((_variant_t)_T("CLID"),v);
						rsCL->put_Collect((_variant_t)_T("CLID"),v);
						rs->get_Collect((_variant_t)_T("CLmc"),v);
						rsCL->put_Collect((_variant_t)_T("CLmc"),v);
						rs->get_Collect((_variant_t)_T("CLgg"),v);
						rsCL->put_Collect((_variant_t)_T("CLgg"),v);
						rs->get_Collect((_variant_t)_T("CLcl"),v);
						rsCL->put_Collect((_variant_t)_T("CLcl"),v);
						CString str;
						str.LoadString(IDS_PIECE);
						rsCL->put_Collect((_variant_t)_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(str) : vTmp));
						rsCL->put_Collect((_variant_t)_T("CLdz"),GetFields(rs,_T("CLdz")));//, 0, rs.Fields(_T("CLdz"))), _T("0.##"))
						rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(vtof(GetFields(rsza,_T("Gnum"))) * vtof(GetFields(rsza,_T("num")))));
						rsCL->put_Collect((_variant_t)_T("CLzz"),GetFields(rs,_T("CLzz")));
						rsCL->put_Collect((_variant_t)_T("CLbz"),GetFields(rs,_T("CLbz")));
						rsCL->Update();
					}
					rsza->Close();
					rs->MoveNext();
				}
				//���̽��������ܴ�TCL������ܵ�TmpSA
				if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
				{
					//��TmpCLgroup���ڱ��������
					EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
				}
				//rsCL->Close();
				//��tmpCL����һ��������TmpCLgroup
				//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
				SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
				SQLx +=_T(" GROUP BY ") + sGROUPBY;
				EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
				//���������ܵ����ݿ�����TmpSA��
				EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpSA SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
				//����ʱ������ݿ�������ʽ���ϱ�TCL
				//��������������ܻ��ܵ����ϱ���
				//ɾ����ʱ��
				RsDeleteAll(rsCL);
				//������ʱ���ϱ����ݼǶ���
				//rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
			}
		}
		rs->Close();
		
		//************
		//���ڴ�����˨��ĸ���Ǹ����ĸ���(��F3)
		//ͳ����˨��ĸ����Ų�Ϊ�գ�Ϊ�յ����ڳ��ҹ�������Ҫͳ�ƣ��������ʹ�õ���˨��ĸ����������Ҫͳ��ʱ�������û���ӵ���˨��ĸ(bUserAdd=-1)
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] ") ;
		if(modPHScal::gbSumAllVolume)
		{
			SQLx +=_T(" WHERE VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(")");
		}
		else
		{
			SQLx +=_T(" WHERE VolumeID =") + ltos(EDIBgbl::SelVlmID) ;
		}
		//AND����������������
		SQLx+=_T(" AND  ((ClassID=") + ltos(iBolts) + _T(" OR ClassID=") + ltos(iNuts) + _T(" OR ClassID=") + ltos(iAttached) + _T(") AND NOT IsNull(SEQ) AND IsSAPart<>-1  OR (ClassID=") + ltos(iBolts) + _T(" OR ClassID=") + ltos(iNuts) + _T(") AND IsSAPart=-1 ) ");		
//		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if( rs->adoEOF && rs->BOF )
		{
		}
		else
		{
			while(!rs->adoEOF)
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomBoltsNuts);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
//				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				rsza->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
				if( rsza->adoEOF && rsza->BOF )
				{
				}
				else
				{
					rsza->MoveFirst();
					rsza->get_Collect((_variant_t)_T("bNotSumSA"),v);
					m_bNotSumSA=vtob(v);
					rs->get_Collect((_variant_t)_T("bUserAdd"),v);
					m_bUserAdd=vtob(v);					
					rs->get_Collect((_variant_t)_T("recno"),v);
					
					if(!( !(v.vt==VT_NULL) ||  (v.vt==VT_NULL) && (m_bUserAdd || !m_bUserAdd && !m_bUserAdd) ))
					{
						//�յļ�¼�ţ��Ҳ����û���ӵĲ��ϣ���ͳ�Ƹ���
						//����Ҫͳ�Ƶĸ�����˨��ĸ
					}else
					{
						rsza->get_Collect((_variant_t)_T("Num"),v);
						m_iNum=vtoi(v);
						rsza->get_Collect((_variant_t)_T("Gnum"),v);
						m_iSANum=vtoi(v);
						rsza->get_Collect((_variant_t)_T("bNotSumSA"),v);
						m_bNotSumSA=vtob(v);
						rs->get_Collect((_variant_t)_T("CLnum"),v);
						m_iCLNum=vtoi(v);
						rs->get_Collect((_variant_t)_T("bUserAdd"),v);
						m_bUserAdd=vtob(v);
						rs->get_Collect((_variant_t)_T("L1"),v);
						m_iL1=vtoi(v);
						sTmp=m_iNum*m_iCLNum;
						rsCL->AddNew();
						rsCL->put_Collect((_variant_t)_T("seq"),nil);
						rs->get_Collect((_variant_t)_T("zdjh"),v);
						rsCL->put_Collect((_variant_t)_T("zdjh"),v);
						rs->get_Collect((_variant_t)_T("VolumeID"),v);
						rsCL->put_Collect((_variant_t)_T("VolumeID"),v);
						rs->get_Collect((_variant_t)_T("CLID"),v);
						rsCL->put_Collect((_variant_t)_T("CLID"),v);
						rs->get_Collect((_variant_t)_T("CLmc"),v);
						rsCL->put_Collect((_variant_t)_T("CLmc"),v);
						rs->get_Collect((_variant_t)_T("CLgg"),v);
						rsCL->put_Collect((_variant_t)_T("CLgg"),v);
						rs->get_Collect((_variant_t)_T("CLcl"),v);
						rsCL->put_Collect((_variant_t)_T("CLcl"),v);
						CString str;
						str.LoadString(IDS_PIECE);
						rsCL->put_Collect((_variant_t)_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(str) : vTmp));
						rsCL->put_Collect((_variant_t)_T("CLdz"),GetFields(rs,_T("CLdz")));
						rsCL->put_Collect((_variant_t)_T("CLnum"),COleVariant(sTmp));
						rsCL->put_Collect((_variant_t)_T("CLzz"),GetFields(rs,_T("CLzz")));
						rsCL->put_Collect((_variant_t)_T("CLbz"),GetFields(rs,_T("CLbz")));
						rsCL->Update();
					}
				}
				rsza->Close();
				rs->MoveNext();
			}
		}
		rs->Close();
		//���̽���˨��ĸ��TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE TmpCLgroup"), NULL, adCmdText);
		}
		//rsCL->Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//����˨��ĸ�����ݿ�����TmpBoltsNuts��
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO TmpBoltsNuts SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy), NULL, adCmdText);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + _T("\' SELECT * FROM TmpTCL")), NULL, adCmdText);
		//ɾ����ʱ��
		EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE * FROM TmpTCL"), NULL, adCmdText);
		//������ʱ���ϱ����ݼǶ���
		//rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
	}
	catch(CException *e)
	{
		e->Delete();
	}
	frmStatus.UpdateStatus(1,true);
	frmStatus.ShowWindow(SW_HIDE);
}

void Cphs::GetPhsBlkIDandCrd(_RecordsetPtr /*ByVal*/ rsza)
{
	//Ŀ�ģ���ȡͼ������ID�ͻ��Ƹ߶ȣ�Y����λ�ƣ�
	//input:
	//        dbprjdb�е�F####?-JZB,
	//output:
	//        dbprj�е�TmpCLgroup,
	_RecordsetPtr rs;
	rs.CreateInstance(__uuidof(Recordset));

	_RecordsetPtr rsTmpZB;
	rsTmpZB.CreateInstance(__uuidof(Recordset));
	CString tbn,tmpID,tmpBlkID,SQLx;
	CString tmpStr;
	try
	{
		SQLx = _T("SELECT * FROM phsBlkDimPos");
		rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		//���пɻ������recno<>Null
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 ORDER BY recno");
		rsTmpZB->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if(rsTmpZB->adoEOF && rsTmpZB->BOF)
		{
			tmpStr.Format(GetResStr(IDS_NoRecordInTZB),EDIBgbl::dbPRJDB->DefaultDatabase,EDIBgbl::Btype[EDIBgbl::TZB], EDIBgbl::SelJcdm,ltos(modPHScal::zdjh));
			throw tmpStr;
		}
		COleVariant vTmp;
		while(!rsTmpZB->adoEOF)
		{
			//չ��sFindTBN�Լӿ��ٶ�
			//tbn = sFindTBN(rsTmpZB.Fields(_T("CustomID")))
			rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp);
			
			if(rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") +vtos(vTmp) + _T("\'")), 0, adSearchForward, vTmp))
			{
				rsID->get_Collect((_variant_t)_T("Index"), &vTmp);
				modPHScal::glIDIndex = vtoi(vTmp);
				rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp);
				modPHScal::glClassID=vtoi(vTmp);
			}
			if(modPHScal::glIDIndex == iPA)
				modPHScal::glPAid = modPHScal::glClassID;
			rsTmpZB->get_Collect((_variant_t)_T("ID"), &vTmp);
			if(vtos(vTmp)==_T(""))
			{
				//�����󻪶�Ժ��׼�ĸ���������鲻��ID,���CustomID,Ȼ������ID.
				rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp);
				tmpID = modPHScal::sFindID(vtos(vTmp));
//				rsTmpZB.Edit();
				rsTmpZB->put_Collect((_variant_t)_T("ID"),STR_VAR(tmpID));
				rsTmpZB->Update();
			}
			else
			{
				rsTmpZB->get_Collect((_variant_t)_T("ID"), &vTmp);
				tmpID = vtos(vTmp);
			}
			tmpBlkID =tmpID;
			//���ȿ������Ƿ����ID���ƴ���Ŀ�
			rs->Find((_bstr_t) (_T("trim(blkID)=\'") + tmpBlkID + _T("\'")), 0, adSearchForward, vTmp);
			VARIANT_BOOL bm=VARIANT_FALSE;
			bm= rs->adoEOF;
			if( modPHScal::glIDIndex == iSA)
			{
				//�Ǹ���
				if(!bm)
				{
					//���û��,��϶��ǲ۸���ɵĸ��������ϲ۸����������µĿ�,��������
					if( modPHScal::glClassID != iGCement)
						tmpBlkID = tmpBlkID +vtos( rsza->GetCollect(_T("iCSnum")));
					rs->Find((_bstr_t) (_T("trim(blkID)=\'") + tmpBlkID + _T("\'")), 0, adSearchForward, vTmp);
					bm=rs->adoEOF;
				}
			}
//			rsTmpZB.Edit();
			if(!bm)
			{
				//ע�͵�����ֹһЩ������Ϣ�����û���
				tmpStr.Format(GetResStr(IDS_NoThisBlockInphsBlkDimPos),tmpBlkID);
				ShowMessage(tmpStr);
			}
			else
			{
				if( modPHScal::glIDIndex == iSA && modPHScal::glPAid != iPAh)
				{
					rs->get_Collect((_variant_t)_T("Zh"), &vTmp);
					rsTmpZB->put_Collect((_variant_t)_T("Crd"),vTmp);
				}
				else
				{
					rs->get_Collect((_variant_t)_T("Dh"), &vTmp);
					rsTmpZB->put_Collect((_variant_t)_T("Crd"),vTmp);
				}
			}
			rsTmpZB->put_Collect((_variant_t)_T("blkID"),STR_VAR(tmpBlkID));
			rsTmpZB->Update();
			rsTmpZB->MoveNext();
		}
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(CString e)
	{
		ShowMessage(e);
	}
}

void Cphs::GetPhsSAELandPAdxdydz()
{
	//Ŀ��:����֧���ܶ�λ�ߴ磬����֧�ܸ�����ߡ�
	try
	{
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		CString SQLx;
		COleVariant vTmp;
		modPHScal::glPAid = vtoi(modPHScal::sFindFLD(_T("CustomID"), _T("ClassID"), modPHScal::dn));
		if(! modPHScal::bPAIsHanger())
		{
			//֧��
			//If glPAid = iPAs Or glPAid = iPAfixZ1 Or glPAid = iPAfixZ2 Then
			//�ų�����(index=iSA)���丽��(��¼��Ϊ��)����������ĸ߶Ⱥ�
			SQLx = _T("SELECT sum(sizeH) as sumH FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 AND index<>") + ltos(iSA);
			rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if(!rs->adoEOF && ! rs->BOF)
			{
				//MsgBox rs.RecordCount
				rs->get_Collect((_variant_t)_T("sumH"), &vTmp);
				modPHScal::sngSEL =modPHScal::sngPEL- (vtof(vTmp)/ 1000);
				modPHScal::UpdateTZB4zdjh(_T("gh1"), _variant_t(modPHScal::sngSEL));
			}
		}
		
		//�ܲ�,����ƫװλ��ֵ�ļ���
		if( giUPxyz == 1)
		{
			//X������

            //����Ū��,Modify by luorijin 2009.02.24  ��"+"�ĳ�"-"
			modPHScal::dxl = modPHScal::dxa - modPHScal::yl;
 			modPHScal::dxr = modPHScal::dxl - modPHScal::yr1;


			//��ֱ��λ
			modPHScal::dyl = modPHScal::sngPEL * 1000 + modPHScal::xl;
			modPHScal::dyr = modPHScal::dyl + modPHScal::xr;
			//����ͼ��λ
			modPHScal::dzl = modPHScal::dza + modPHScal::zl;
			modPHScal::dzr = modPHScal::dzl + modPHScal::zr;
		}
		else if( giUPxyz == 2 )
		{
			//y������
			//����ͼ��λ
			modPHScal::dxl = modPHScal::dxa + modPHScal::xl;
			modPHScal::dxr = modPHScal::dxl + modPHScal::xr;
			//��ֱ��λ
			modPHScal::dyl = modPHScal::sngPEL * 1000 + modPHScal::yl;
			modPHScal::dyr = modPHScal::dyl + modPHScal::yr;
			//����ͼ��λ
			modPHScal::dzl = modPHScal::dza + modPHScal::zl;
			modPHScal::dzr = modPHScal::dzl + modPHScal::zr;
		}
		else if( giUPxyz == 3)
		{
			//z������
			//����ͼ��λ
			modPHScal::dxl = modPHScal::dxa + modPHScal::xl;
			modPHScal::dxr = modPHScal::dxl + modPHScal::xr;
			//��ֱ��λ
			modPHScal::dyl = modPHScal::sngPEL * 1000 + modPHScal::zl;
			modPHScal::dyr = modPHScal::dyl + modPHScal::zr;
			//����ͼ��λ
 			modPHScal::dzl = modPHScal::dza - modPHScal::yl;
// 			modPHScal::dzr = modPHScal::dzl + modPHScal::yr1; 

            //����ͼ����Ū��,Modify by luorijin 2009.02.24  ��"+"�ĳ�"-"
			modPHScal::dzr = modPHScal::dzl - modPHScal::yr1;
		}
		modPHScal::UpdatePipeDimHeight(); // �ܲ����
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void Cphs::Class_Initialize()
{
}

void Cphs::Class_Terminate()
{
}

_variant_t Cphs::ClassDebugID()
{
	return _variant_t(_T(""));
}

CString Cphs::GetBHforDoubleCSBeam(float /*ByVal*/ GDW1, float /*ByVal*/ OffsetOfP1toP2, float /*ByVal*/ Span, float /*ByVal*/ p1, float /*ByVal*/ p2, CString /*ByVal*/ SACustomID, int /*Optional*/ iDCS, int /*Optional*/ FirstCal)
{
	//Ŀ��:�۸�˫�����֧�����㡣
	//��Դ:<�������糧�̷�ú�۹ܵ�֧��������ֲ�(���б�)>�����������Ժ1977�걾
	//����:GDW1--����P1(kgf)���֧�����ľ���,cm.
	//        OffsetOfP1toP2--����P1(kgf)�����P2(kgf)�ļ��,cm.
	//        C--����P2���֧������һ�˵ľ���,cm,C=Span-GDW1-OffsetOfP1toP2
	//        Span--��֧�����,cm
	//        iDCS--�۸�������˫�۸�iDCS=2,���۸�iDCS=1��Ĭ��Ϊ˫�۸�
	//        FirstCal--��ʾ��ʾ��Ϣ��1=����ʾ��2=��ʾ
	//  ����:�۸��ͺţ�������,����ǰ��ı�־[��
	
	//	int i;
	float C=0,Wx1=0,Wx=0;
	CString sSQLx,sTmp,sTmp2;
	CString ret;
	COleVariant v;
	C = Span - GDW1 - OffsetOfP1toP2;
	try
	{
		Wx = GDW1 * (p1 * (OffsetOfP1toP2 + C) + p2 * C) / 3200 / Span * (2 / iDCS);
		Wx1 = C * (p2 * (OffsetOfP1toP2 + GDW1) + p1 * GDW1) / 3200 / Span * (2 / iDCS);
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
//		rs.m_pDatabase=&modPHScal::dbZDJcrude;
		sTmp.Format(_T("%d"),(Wx1>Wx ? Wx1 : Wx));
		sSQLx = _T("SELECT * FROM [SSteelPropertyCS] WHERE Wx>=");
		sSQLx+=sTmp;
		sSQLx+= _T(" ORDER BY ID");
		rs->Open((_bstr_t)sSQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
		if(rs->adoEOF && rs->BOF)
		{
			sTmp.Format(GetResStr(IDS_NotFoundAnyBHinXSteelProperty),_T("SSteelPropertyCS"));
			throw sTmp;		
		}
		else
		{
			rs->get_Collect((_variant_t)_T("BH"),v);
			ret=vtos(v);
			sTmp2.Format(GetResStr(IDS_SAMadeBySSteel),_T("\%d"),ret,SACustomID);
			sTmp.Format(sTmp2,iDCS);
			throw sTmp;
		}
		rs->Close();
		return ret;
	}
	catch(CString e)
	{
		if(FirstCal==2)
		{
			ShowMessage(e);
		}
	}
	return ret;
}

int Cphs::GetPhsStructFromSampleID(long /*ByVal*/ SampleID)
{
	int ret=-1;
	try
	{
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_Recordset* rsza = FrmTxsr.m_pViewTxsr->m_ActiveRs;
		CString SQLx,sTmp,sTmp2;
		COleVariant v;
		if(modPHScal::iSelSampleID > 0)
		{
			sTmp.Format(_T("%d"),SampleID);
			SQLx = _T("SELECT * FROM phsStructureName WHERE SampleID=") + sTmp;
			rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
			if (rs->adoEOF && rs->BOF)
			{
				sTmp2.Format(GetResStr(IDS_NotFoundSampleIDSprNum),_T("\%d"),_T("\%d"));
				sTmp.Format(sTmp2,modPHScal::iSelSampleID,modPHScal::giWholeSprNum);
				ShowMessage(sTmp);
				ret = -1;
			}
			else if(!rs->adoEOF)
			{
				rs->MoveLast();
				rs->get_Collect((_variant_t)_T("SampleID"),v);
				ret=vtoi(v);
				rs->get_Collect((_variant_t)_T("PA"),v);
				//�ܲ����Ը�ֵ
				PA=vtos(v);
				rsza->PutCollect(_T("IDdn1"),stov(PA));
				rs->get_Collect((_variant_t)_T("SA"),v);
				//�������Ը�ֵ
				SA=vtos(v);
				rsza->PutCollect(_T("IDgn1"),stov(SA));
				rsza->PutCollect(_T("iSelSampleID"),_variant_t((long)ret));
				//rsza->Update();
			}
		}
		else
			ret = -1;
	}
	catch(_com_error )
	{
	}
	return ret;
}



Cphs::Cphs()
{
	iAttachIncd=iAttachIncluded;
	iChkCondition=0;
    iSAPAConnect=-1;
	SACSHeight=0;
	rsTZC=NULL;
//	rsphsStructureREF.m_pDatabase=&EDIBgbl::dbSORT;
	TZBResultObj=NULL;
	m_rsObj=NULL;
	rsID.CreateInstance(__uuidof(_Recordset));
	m_rsObj.CreateInstance(__uuidof(_Recordset));
}

Cphs::~Cphs()
{
	if(rsID->State == adOpenStatic)
		rsID->Close();
}

void Cphs::InitListRs()
{
	//�����ӱ�rsConnect��rsUnCheckedType
	try
	{
		CString sSQL;
		if(rsConnect->State != adOpenStatic)
		{
//			rsConnect.m_pDatabase=&EDIBgbl::dbPRJ;
			sSQL=(_T("SELECT * FROM connect "));
//			rsConnect.Open(dbOpenDynaset,sSQL);
			rsConnect->Open((_bstr_t)sSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		else
			rsConnect->Requery(adExecuteRecord);
		if(rsUnCheckedType->State != adOpenStatic)
		{
//			rsUnCheckedType.m_pDatabase=&EDIBgbl::dbPRJ;
			sSQL=(_T("SELECT * FROM rsUnCheckedType "));
//			rsUnCheckedType.Open(dbOpenDynaset,sSQL);
			rsUnCheckedType->Open((_bstr_t)sSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		else
			rsUnCheckedType->Requery(adExecuteRecord);
	}
	catch(CException *e)
	{
		e->Delete();
	}
	
}

void Cphs::SelectItemRsObj()
{

}

void Cphs::CloseRecordsets()
{
	//�ر�rsConnect��rsUnCheckedType
	try
	{
		if(rsConnect->State == adOpenStatic)
			rsConnect->Close();
		if(rsUnCheckedType->State == adOpenStatic)
			rsUnCheckedType->Close();
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

