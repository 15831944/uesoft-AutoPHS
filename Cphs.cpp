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
		if(!rsID.IsOpen())
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
			brsIDStatus=TRUE;
		}
		if (!rsID.FindFirst(temp))
			return false;
		else
			return true;
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
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
	CDaoRecordset rs(&EDIBgbl::dbPRJ);
	try
	{
		int	i;
		if(!rsID.IsOpen())
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
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
		if(rs.IsOpen())
			rs.Close();		
		rs.Open(dbOpenSnapshot,sSQL);
		
        if (rs.IsBOF() && rs.IsEOF())
		{
			ShowMessage(GetResStr(IDS_NoAnyRecordInPictureClipData));
			return 0;
		}
		else
		{
			try{ COleVariant vTmp; 
			//COleVariant vTmp;
			ResultObj->ResetContent();
			i=0;
			RsDeleteAll(rsUnCheckedType);
			while(!rs.IsEOF())
			{
				i++;
				rs.GetFieldValue(_T("CustomID"),vTmp);
				//	ResultObj->AddString(vtos(vTmp));
				//	rs.MoveNext();
				// try{
				/*if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ,_T("rsUnCheckedType")))
				EDIBgbl::dbPRJ.Execute(_T("DELETE * FROM rsUnCheckedType"));
				else 
				EDIBgbl::dbPRJ.Execute(_T("CREATE TABLE rsUnCheckedType (CustomID TEXT (20))"));*/
				
				//	   for(i=0;i<ResultObj->GetCount();i++)
				//	   {
				//		   ResultObj->GetText(i,temp);
				rsUnCheckedType.AddNew();
				//		   vTmp.TrimLeft();Vtmp.TrimRight();
				rsUnCheckedType.SetFieldValue(_T("CustomID"),vTmp);//STR_VAR(vTmp));
				rsUnCheckedType.Update();
				//EDIBgbl::dbPRJ.Execute(_T("INSERT INTO rsUnCheckedType (CustomID) VALUES ('")+Trim(temp)+_T("')"));
				// }
				//  return i;
				rs.MoveNext();
			}
			}
			catch(::CDaoException * e)
			{
				e->ReportError();
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
	catch(::CDaoException * e)
	{
		e->ReportError();
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
	//CDaoRecordset rs((CDaoDatabase *)&EDIBgbl::dbPRJ);
	//CDaoRecordset rsUnCheckedType((CDaoDatabase *)&EDIBgbl::dbPRJ);
	
	CDaoRecordset rs(&EDIBgbl::dbPRJ);
	CString FldName;
	CString tmp,sSQL;
	long ret;
	long i,mvClassID;
	
	try
	{
		//����3�����Ҫͬʱƥ��
		if(!rsID.IsOpen())
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
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
			//rsConnect.Requery();
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
			if (!rsConnect.FindFirst(sSQL))
			{            
			/*sSQL=CString(_T("SELECT * FROM connect WHERE trim(cntb)=\'"))
			+modPHScal::sFindID(Trim(Cntb))+_T("\'");
				rs.Open(dbOpenSnapshot,sSQL,0); */           
				sSQL=CString(_T(" trim(cntb)=\'"))
					+modPHScal::sFindID(Trim(Cntb))+_T("\'");
				//rsConnect.m_strFilter=sSQL;
				//rsConnect.Requery();
				FldName = _T("cnte");
			}
			else
			{				
				FldName = _T("cntn");//����3���������ƥ��
			}
		}
		//�Ƿ���Ҫ����?
		if (!rsConnect.FindFirst(sSQL))
		{
			//ShowMessage(GetResStr(IDS_NoCustomIDinConnect));
			return 0;
			//throw;
		}
		
		//��������m
		CString tbn;
		bool bAdd;
		COleVariant vTmp;
		ResultObj->ResetContent();
		i=0;
		modPHScal::ZdjType=vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("TYPE")));
		do
		{
			i++;
			rsConnect.GetFieldValue(FldName,vTmp);
			//CString str = rsConnect.GetSQL();
			tmp=vtos(vTmp);
			tmp.TrimLeft();tmp.TrimRight();tmp.MakeUpper();
			
			if (rsID.FindFirst(_T("trim(ID)=\'")+tmp+_T("\'")))
			{
				//�ҵ���Ӧ��ID
				rsID.GetFieldValue(_T("CustomID"),vTmp);
				tmp=vtos(vTmp);
				tmp.TrimLeft();tmp.TrimRight();tmp.MakeUpper();
				i=ResultObj->FindStringExact(-1,tmp);
				//i=SendMessage(ResultObj->m_hWnd,LB_FINDSTRINGEXACT,0,(long)vTmp.lVal);
				//�ò����Ƿ�ɼ���_T("���ÿ�"),bAdd=true�ɼ���
				rsID.GetFieldValue(_T("ClassID"),vTmp);
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
					rsID.GetFieldValue(_T("Index"),vTmp);
					iIndex=vtoi(vTmp);
					if(iIndex== iPA)
						SQLx.Format(_T("SELECT CustomID FROM tmpCustomIDPA WHERE CustomID=\'%s\'"),tmp);						
					else if(iIndex== iConnectPART)
						SQLx.Format(_T("SELECT CustomID FROM tmpCustomIDPART WHERE CustomID=\'%s\'"),tmp);						
					else if(iIndex== iSA)
						SQLx.Format(_T("SELECT CustomID FROM tmpCustomIDSA WHERE CustomID=\'%s\'"),tmp);
					if(rs.IsOpen()) rs.Close();
					rs.Open(dbOpenSnapshot,SQLx);
					if(rs.IsBOF() && rs.IsEOF())
						//���㲿���������ڵ�ǰ�淶,���ܼ����б��,��ֹ��ѡ
						bAdd=false;
					else
						bAdd=true;
				}
				
				if ((i==-1)&&bAdd) ResultObj->AddString(tmp);
			}//if block
		}
		while(rsConnect.FindNext(sSQL));
		//�ѳ�����õĿ������д����ʱ��RsUnCheckedPart
		ret=i;
		i=SaveRsUnCheckedTypeFromResultObj();		  
   }//try block
   catch(::CDaoException * e)
   {
	   e->ReportError();
	   e->Delete();
   }
   return ret;
   return 0;
}

long Cphs::SetPhsPreviousTypeToListBox()
{
	//����ѡ������Ӽ���ʽCntb��CntbP,ȷ������ǰ����֮���������,
	//�����ɹ�ѡ�����ʽ�����б��ؼ�ResultObj,�Ա�ѡ��
	//Input:���Cntb,CntbP(CntbP����Cntb��ǰһ�����,��Ϊ��)
	//Output:���п���Cntb(��CntbP)������ǰһ����������뵽�б��ؼ�ResultObj
	//����:�����������
	
	
	/*   	CDaoRecordset rs((CDaoDatabase *)&EDIBgbl::dbPRJ);
	CDaoRecordset rsUnCheckedType((CDaoDatabase *)&EDIBgbl::dbPRJ);
	bool bNullCntn=false;
	CString tmp,ret;
	int i;
	try{
	if ( (IsNull(_variant_t(mvarCntb))) || (mvarCntb.IsEmpty()) )
	throw;//Err.Raise iUE_NoSetCntbToCphs, , ResolveResString(iUE_NoSetCntbToCphs)
	else{
	if ( (IsNull(_variant_t(mvarCntbP))) || (mvarCntbP.IsEmpty())) {
				EDIBgbl::SQLx=_T("SELECT * FROM connect WHERE trim(cnte)='");
				EDIBgbl::SQLx+=modPHScal::sFindID(Trim(mvarCntb))+_T("'");
				bNullCntn=true;
				}
				else{
				EDIBgbl::SQLx=_T("SELECT * FROM connect WHERE trim(cnte)='");
				EDIBgbl::SQLx+=modPHScal::sFindID(Trim((LPCTSTR)CntbP))+_T("\' AND trim(cntn)=\'");
				EDIBgbl::SQLx+=modPHScal::sFindID(Trim((LPCTSTR)mvarCntb))+_T("\' AND Trim(Cntn)<>\'\'");
				rs.Open(AFX_DAO_USE_DEFAULT_TYPE,EDIBgbl::SQLx,0);
				if ((rs.IsEOF())&&(rs.IsBOF())){
				bNullCntn=true;
				EDIBgbl::SQLx=_T("SELECT * FROM connect WHERE trim(cnte)='")
				+modPHScal::sFindID(Trim(mvarCntb))+_T("\'");
				}
				}
				}//else block
				rs.Open(AFX_DAO_USE_DEFAULT_TYPE,EDIBgbl::SQLx);
				if ( (rs.IsEOF())&&(rs.IsBOF()))
				throw;//Err.Raise iUE_NoCustomIDinConnect, , ResolveResString(iUE_NoCustomIDinConnect)
				else if (!rs.IsEOF()){
				rs.MoveLast();
				rs.MoveFirst();
				ret=rs.GetRecordCount();
				}
				ResultObj->ResetContent();
				COleVariant vTmp;
				while(!rs.IsEOF()){
				if (bNullCntn){
				
				  rs.GetFieldValue(_T("cntb"),vTmp);
				  tmp=modPHScal::sFindCustomID(Trim((char *)vTmp.pbVal));
				  i=ResultObj->FindStringExact(-1,tmp);
				  if (i==LB_ERR) ResultObj->AddString(tmp);
				  }
				  else{
				  rs.GetFieldValue(_T("cntn"),vTmp);
				  if ((vTmp.vt!=VT_NULL) && (Trim((char *)vTmp.pbVal)!=_T(""))){
				  rs.GetFieldValue(_T("cntb"),vTmp);
				  tmp=modPHScal::sFindCustomID(Trim((char *)vTmp.pbVal));
				  i=ResultObj->FindStringExact(-1,tmp);
				  if (i==-1) ResultObj->AddString(tmp);
				  }//if block
				  }
				  rs.MoveNext();
				  }//while block
				  rs.Close();
				  //�ѳ�����õĿ������д����ʱ��
				  i=SaveRsUnCheckedTypeFromResultObj();
				  }//try block
				  catch(::CDaoException * e)
				  {
				  e->ReportError();
				  e->Delete();
				  }
	return ret;*/
	return 0;
}

long Cphs::SaveRsUnCheckedTypeFromResultObj()
{
	int i;
	CString temp;
	try{
	   /*if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ,_T("rsUnCheckedType")))
	   EDIBgbl::dbPRJ.Execute(_T("DELETE * FROM rsUnCheckedType"));
	   else 
		EDIBgbl::dbPRJ.Execute(_T("CREATE TABLE rsUnCheckedType (CustomID TEXT (20))"));*/
		RsDeleteAll(rsUnCheckedType);
		for(i=0;i<ResultObj->GetCount();i++)
		{
			ResultObj->GetText(i,temp);
			rsUnCheckedType.AddNew();
			temp.TrimLeft();temp.TrimRight();
			rsUnCheckedType.SetFieldValue(_T("CustomID"),STR_VAR(temp));
			rsUnCheckedType.Update();
			//EDIBgbl::dbPRJ.Execute(_T("INSERT INTO rsUnCheckedType (CustomID) VALUES ('")+Trim(temp)+_T("')"));
		}
		return i;
		
	}
	catch(::CDaoException * e)
	{
		//e->ReportError();
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
	//CDaoRecordset rs((CDaoDatabase *)&EDIBgbl::dbPRJ);
	//CDaoRecordset rsUnCheckedType((CDaoDatabase *)&EDIBgbl::dbPRJ);
	CDaoRecordset rsTmpCheckedType((CDaoDatabase *)&EDIBgbl::dbPRJ);
	
	//CDaoRecordset rsTZB,rsTZC,rsTZF;
	CString temp;
	int j=0,m=0;
	long boolk=0 ;
	try{
		//EDIBgbl::SQLx=_T("SELECT * FROM rsUnCheckedType");
		//rsUnCheckedType.Open(dbOpenSnapshot,EDIBgbl::SQLx);
		InitListRs();
		if ((rsUnCheckedType.IsEOF())&&(rsUnCheckedType.IsBOF()))
		{
			::ShowMessage(GetResStr(IDS_NullRsUnCheckedTypeIndbPRJ));
			goto errH;
		}
		//throw;//Err.Raise iUE_NullRsUnCheckedTypeIndbPRJ, , ResolveResString(iUE_NullRsUnCheckedTypeIndbPRJ)
		else if (!rsUnCheckedType.IsEOF()){
			rsUnCheckedType.MoveLast();
			rsUnCheckedType.MoveFirst();
			//'�����ʱ��rsTmpCheckedType,׼����ż��Ľ��
			if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ,_T("rsTmpCheckedType")))
				EDIBgbl::dbPRJ.Execute(_T("DELETE * FROM rsTmpCheckedType"));
			else
				EDIBgbl::dbPRJ.Execute(_T("CREATE TABLE rsTmpCheckedType (CustomID char(20))"));
			MakeRsTZB(idbPRJ, _T("rsTmp"));
			for(m=0;m<rsUnCheckedType.GetRecordCount();m++){
				//�����ʱ��trsTmpREF,׼�����ѡ�����Ͻṹ��
				if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ,_T("rsTmpREF")))
					EDIBgbl::dbPRJ.Execute(_T("DELETE * FROM rsTmpREF"));
				else
					EDIBgbl::dbPRJ.Execute(_T("CREATE TABLE rsTmpREF (CustomID char(20))"));
				if (iSAPAConnect!=iPA)//����������浽trsTmpREF��
					for(j=0;j<=SourceObj->GetCurSel();j++){
						SourceObj->GetText(j,temp);
						EDIBgbl::SQLx=CString(_T("INSERT INTO rsTmpREF (CustomID) VALUES (\'"))+Trim(temp)+_T("\')");
						EDIBgbl::dbPRJ.Execute(EDIBgbl::SQLx);
					}
					
					//��������һ��Ԫ�ر��浽trsTmpREF��
					COleVariant vTmp;
					rsUnCheckedType.GetFieldValue(_T("CustomID"),vTmp);
					EDIBgbl::SQLx=CString(_T("INSERT INTO rsTmpREF (CustomID) VALUES (\'"))+vtos(vTmp)+_T("\')");
					EDIBgbl::dbPRJ.Execute(EDIBgbl::SQLx);
					
					//'��ʼ����2:
					//ÿ���޸�AllPrjdb.mdb��ZB�����ݱ�ṹ֮��Ī����ͬʱ�޸�F4511s.mdb��ZB��ṹ��workprj.mdb�е�rsTmpZB��Ľṹ��
					//MakeRsTZB(idbPRJ, _T("rsTmp"));
					//��ʼ����3:
					EDIBgbl::SQLx = _T("SELECT * FROM rsTmpREF");
					if(rsTmpREF.IsOpen())
						rsTmpREF.Close();
					rsTmpREF.m_pDatabase=&EDIBgbl::dbPRJ;
					rsTmpREF.Open(dbOpenDynaset,EDIBgbl::SQLx);
					//'��ʼ����4��5
					bool k=GetphsBHandSizesTest();
					if (k){
						rsUnCheckedType.GetFieldValue(_T("CustomID"),vTmp);
						EDIBgbl::SQLx=_T("INSERT INTO rsTmpCheckedType (CustomID) VALUES ('")+vtos(vTmp)+_T("')");
						EDIBgbl::dbPRJ.Execute(EDIBgbl::SQLx);
					}
					rsUnCheckedType.MoveNext();
			}//for block
			//��ʼ����7:�������ResultObj�б��
			
			rsTmpCheckedType.Open(AFX_DAO_USE_DEFAULT_TYPE,_T("SELECT CustomID FROM rsTmpCheckedType"));
			if ((rsTmpCheckedType.IsEOF())&&(rsTmpCheckedType.IsBOF()));
			else if (!rsTmpCheckedType.IsEOF()){
				rsTmpCheckedType.MoveLast();
				rsTmpCheckedType.MoveFirst();
			}
			//����б���Ա�����װ��
			ResultObj->ResetContent();
			COleVariant vTmp;
			for(j=0;j<rsTmpCheckedType.GetRecordCount();j++)
			{
				rsTmpCheckedType.GetFieldValue(_T("CustomID"),vTmp);
				ResultObj->AddString(vtos(vTmp));
				rsTmpCheckedType.MoveNext();
			}
			return rsTmpCheckedType.GetRecordCount();
			//Set rsTmpCheckedType = Nothing
		}
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
	}
errH:
/*	if(rsTmpCheckedType.IsOpen())
rsTmpCheckedType.Close();
if(rsUnCheckedType.IsOpen())
	rsUnCheckedType.Close();*/
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
	CDaoFieldInfo fi;
	
	try
	{
		CDaoRecordset rs1(&EDIBgbl::dbPHScode), rs(&EDIBgbl::dbPRJ);//20071101 "dbSORT" ��Ϊ "dbPHScode"
		if(!rsID.IsOpen())
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
			brsIDStatus=TRUE;
		}
		
		SQLx = _T("SELECT * FROM phsStructureREF WHERE SampleID=") + ltos(SampleID) + _T(" ORDER BY SEQ ");
		rs1.Open(dbOpenDynaset,SQLx);
		
		if(rs1.IsEOF() && rs1.IsBOF())
		{
			sTmp.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),ltos(SampleID));
			throw sTmp;
		}
		rs1.MoveLast();
		IC = rs1.GetRecordCount();
		Ptype=new CString[IC];
		COleVariant vTmp;
		iNumSPR = 0;
		iNumCSPR = 0;
		iNumRod = 0;
		sSPR = _T(" ");
		for(i=IC-1;i>=0;i--)
		{
			//ע�͵��жϾ䣬�ɼӿ��ٶ�
			//If Not IsNull(rs1.Fields(_T("ID"))) Then
			rs1.GetFieldValue(_T("ID"),vTmp);
			Ptype[i] = vtos(vTmp);
			//End If
			rs1.MovePrev();
		}
		rs.Open(dbOpenSnapshot,_T("SELECT * FROM PictureClipData"));
		rs1.MoveFirst();
		for( i = 0 ;i<IC;i++)
		{
			//�ӵ�һ����ʼ����ƥ��
			if(i == IC - 1)
				//����
				SQLx = _T("Trim(ID)=\'") + Ptype[i] + _T("\' AND index=0");
			else
				SQLx = _T("Trim(ID)=\'") + Ptype[i] + _T("\'");
			if(!rs.FindFirst(SQLx))
			{
				sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("ID"),Ptype[i]);
				throw sTmp;
			}
			//��һ���ǹܲ�
			rs.GetFieldValue(_T("Description"),vTmp);
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
				rs.GetFieldValue(_T("ClassID"),vTmp);
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
			rs.GetFieldValue(_T("ID"),vTmp);
			if(rsID.FindFirst(_T("trim(ID)=\'") + vtos(vTmp)+_T("\'")))
			{
				rsID.GetFieldValue(_T("CustomID"),vTmp);
				strTmp = vtos(vTmp);
				rs1.Edit();
				rs1.SetFieldValue(_T("CustomID"),STR_VAR(strTmp));
				rs1.Update();
			}
			rs1.MoveNext();
		}
		rs1.Close();
		
		//��ʼ����������
		//������ģ�������е�ʹ��Ƶ����Ϣ
		
		SQLx = _T("SELECT * FROM phsStructureName WHERE SampleID=") + ltos(SampleID);
		rs1.Open(dbOpenDynaset,SQLx);
		sTmp.Format(_T("%05d"),SampleID);
		//�����ʹ��������������Ϊ��
		rs1.GetFieldInfo(_T("SampleName"),fi);
		strTmp=ret;
		//�˴�Ҫע���ַ������ֽ���
		int n=ret.GetLength();
		CString ss=ret.Left(2);
		if(n > fi.m_lSize)
			strTmp=ret.Left(fi.m_lSize);
		if(rs1.IsEOF() && rs1.IsBOF())
		{
			//����Ŀ��ؿ���ʹ��SQL�����ݼ�¼������֮���л���
			//ʹ��SQLʱ������ģ���������ǡ���֧���Ե���������ʹ�ö����򲻻���������⣬��˲�����ΪSQL���ܼӿ��ٶȶ�����ʹ��rs1��
			SQLx.Format(_T("INSERT INTO phsStructureName (SampleID,BmpName,SampleName,PA,SA,SPR,iNumOfPart,iNumSPR,iNumCSPR,iNumRod,FreQuence,bAllowUse) ")
				_T("VALUES (%s,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%s,%s,%s,%s,1,-1)"),
				ltos(SampleID),sTmp,strTmp,Ptype[0],Ptype[IC - 1],sSPR,ltos(IC), ltos(iNumSPR),ltos(iNumCSPR),ltos(iNumRod));
			EDIBgbl::dbPHScode.Execute(SQLx);//20071101 "dbSORT" ��Ϊ "dbPHScode"
			EDIBgbl::dbPRJ.Execute(SQLx);
		}
		else
		{
			SQLx.Format(_T("UPDATE phsStructureName SET BmpName=\'%s\',SampleName=\'%s\',PA=\'%s\',SA=\'%s\',SPR=\'%s\',iNumOfPart=%s,iNumSPR=%s,iNumCSPR=%s,iNumRod=%s,bAllowUse=-1 WHERE SampleID=%s"),
				sTmp,strTmp,Ptype[0],Ptype[IC - 1],sSPR,ltos(IC), ltos(iNumSPR),ltos(iNumCSPR),ltos(iNumRod),ltos(SampleID));
			EDIBgbl::dbPHScode.Execute(SQLx);//20071101 "dbSORT" ��Ϊ "dbPHScode"
			EDIBgbl::dbPRJ.Execute(SQLx);
		}
		delete [] Ptype;
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
		if(Ptype!=NULL)
			delete [] Ptype;
	}
	catch(CException *e)
	{
		e->Delete();
		if(Ptype!=NULL)
			delete [] Ptype;
	}
	catch(...)
	{
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
		   CDaoRecordset rs1(&EDIBgbl::dbPHScode), rs(&EDIBgbl::dbPRJ);//20071019 "dbSORT" ��Ϊ "dbPHScode"
		   long  i =0, iNumTmp=0;
		   CString tmpStr;
		   CString SQLx = _T("SELECT * FROM PictureClipData");
		   rs.Open(dbOpenDynaset,SQLx );
		   if( rs.IsEOF() && rs.IsBOF())
		   {
			   throw GetResStr(IDS_NoAnyRecordInPictureClipData);
		   }
		   SQLx = _T("SELECT * FROM phsStructureREF WHERE SampleID=") + ltos(SampleID) + _T(" ORDER BY SEQ ");
		   rs1.Open(dbOpenDynaset,SQLx );
		   if( rs1.IsBOF() && rs1.IsEOF())
		   {
			   tmpStr.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),ltos(SampleID));
			   throw tmpStr;
		   }
		   COleVariant vTmp;
		   rs1.MoveLast();
		   rs1.MoveFirst();
		   int C=rs1.GetRecordCount();
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
			   rs1.GetFieldValue(_T("ID"),vTmp);
			   if(vTmp.vt!=VT_NULL)
			   {
				   Ptype[i]=vtos(vTmp);
				   
				   if(rs.FindFirst(_T("Trim(ID)=\'") + Ptype[i] + _T("\'")))
				   {
					   rs.GetFieldValue(_T("ClassID"),vTmp);
					   mlPartClassID[i]=vtoi(vTmp);
					   rs.GetFieldValue(_T("Index"),vTmp);
					   mlPartIndex[i]=vtoi(vTmp);
					   if(mlPartClassID[i]== iPtype)
					   {
						   if(iPtype == iSPR)
						   {
							   rs.GetFieldValue(_T("CustomID"),vTmp);
							   modPHScal::sSprInfo[iNumTmp].CustomID = vtos(vTmp);
							   iNumTmp ++;
						   }
						   ret++;
					   }
				   }
			   }
			   rs1.MoveNext();
		   }
	   }
	   catch(CString e)
	   {
		   ShowMessage(e);
	   }
	   catch(CDaoException * e)
	   {
		   e->ReportError();
		   e->Delete();
	   }
	   catch(CException *e)
	   {
		   e->Delete();
	   }
	   catch(...)
	   {
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
	CDaoRecordset rs1;
	long iNumTmp=0;
	CString SQLx;
	COleVariant v;
	long ret=0;
	try
	{
		rs1.m_pDatabase=&EDIBgbl::dbPHScode;//20071018 "dbSORT" ��Ϊ "dbPHScode"
		CString sTmp;
		char sID[64];
		ltoa(SampleID,sID,10);
		if(!rsID.IsOpen())
		{
			SQLx=_T("SELECT * FROM PictureClipData");
			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID.Open(dbOpenDynaset,SQLx);
		}
		else
			rsID.Requery();
		brsIDStatus=TRUE;
		if( rsID.IsEOF() && rsID.IsBOF() )
		{
			ShowMessage(GetResStr(IDS_NoAnyRecordInPictureClipData));
			rsID.Close();
			brsIDStatus=FALSE;
		}
		else
		{
			SQLx = _T("SELECT * FROM phsStructureREF WHERE SampleID=");
			sTmp= sID;
			SQLx=SQLx + sTmp;
			sTmp=_T(" ORDER BY SEQ ");
			SQLx=SQLx + sTmp;
			rs1.Open(dbOpenDynaset,SQLx);
			if( rs1.IsBOF() && rs1.IsEOF() )
			{
				sTmp.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),sID);
				ShowMessage(sTmp,MB_TOPMOST|MB_OK);
			}
			else
			{
				while(!rs1.IsEOF())
				{
					rs1.GetFieldValue(_T("ID"),v);
					if( v.vt!=VT_NULL)
					{
						if( rsID.FindFirst(CString(_T("Trim(ID)=\'")) + vtos(v) + _T("\'")))
						{
							rsID.GetFieldValue(_T("ClassID"),v);
							if( vtoi(v) == iPtype )
							{
								if( iPtype == iSPR )
								{
									rsID.GetFieldValue(_T("CustomID"),v);
									modPHScal::sSprInfo[iNumTmp].CustomID = vtos(v);
									iNumTmp = iNumTmp + 1;
								}
								ret++;
							}
						}
					}
					rs1.MoveNext();
				}
				RCount = rs1.GetRecordCount();
			}
		}
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
		RCount=rs1.GetRecordCount();
	}
	rs1.Close();
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
		CDaoRecordset rs1(&EDIBgbl::dbPHScode);//20071101 "dbSORT" ��Ϊ "dbPHScode"
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
			+ modPHScal::dbZDJcrude.GetName() + _T(" ;PWD=") + ModEncrypt::gstrDBZdjCrudePassWord + _T("]WHERE Pmax >=") 
			+ ftos(tmppjg) + _T(" AND (Dw >= ") + ftos(modPHScal::dj* (1 - modPHScal::gnDW_delta * 0.01))
			+ _T(" AND Dw <= ") + ftos(modPHScal::dj* (1 + modPHScal::gnDW_delta * 0.01)) 
			+ _T(" AND Dw>0 OR Dw Is NULL OR Dw=0) AND tj >= ") + ftos(modPHScal::t0) + _T("))) ");
		
		strSA.Format(_T(" (SA=\'%s\') "),SA);
		int Gnum;			
		Gnum= (modPHScal::glNumSA!=0 ? modPHScal::glNumSA : 1);
		strSAexists = _T(" ( SA IN ( Select ID FROM PictureClipData ")
			_T(" WHERE EXISTS ( Select CustomID FROM [") + modPHScal::tbnSA + _T("] IN \"\" [; DATABASE=") 
			+ modPHScal::dbZDJcrude.GetName() + _T(" ;PWD=") + ModEncrypt::gstrDBZdjCrudePassWord + _T("] WHERE PictureClipData.CustomID = CustomID AND ((PictureClipData.ClassID= ") + ltos(iG100) + _T(") OR PMAXH >=") 
			+ ftos(tmppjg / Gnum * (vtob(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("ifLongVertPipe"))) ? Gnum : 1))
			+_T(" AND ((( PictureClipData.ClassID = ")
			+ ltos(iSACantilever) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbrace) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbraceFixG47) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbraceFixG48) + _T(" OR PictureClipData.ClassID = ") + ltos(iG51) + _T(" OR PictureClipData.ClassID = ") + ltos(iG56) + _T(" OR PictureClipData.ClassID = ") + ltos(iG57)  + _T(") AND GDW1 >=")
			+ ftos(modPHScal::gdw1) + _T(") OR (( PictureClipData.ClassID =") + ltos(iSAbeam) +_T(" OR PictureClipData.ClassID =") + ltos(iG52_55) + _T(" ) AND GDW1 >=")
			+ _T(" IIF(")+ ftos(modPHScal::gdw1+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0)/2) +_T(" > L1/2 , L1 - ") + ftos(modPHScal::gdw1+(modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2)
			+ _T(" , ")+ ftos(modPHScal::gdw1+(modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2) + _T(") AND L1> ") + ftos(modPHScal::Lspan)
			+_T(" ) OR (PictureClipData.ClassID= ") + ltos(iGCement) + _T(")))))) ");
		
		SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
			SQL1 + _T(" AND ") + strPA + _T(" AND ") + strPAexists + _T(" AND ") + strSA + _T(" AND ") + strSAexists );
		rs1.Open(dbOpenSnapshot,SQLx);
		if( !(rs1.IsEOF() && rs1.IsBOF()) )
		{
			//���ƹܲ�,���ƹܲ�������ڣ����Ƹ���/����������ڣ���
		}
		else
		{
			//���ƹܲ�,���ƹܲ�������ڣ����Ƹ���/����������ڣ�û��
			SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
				SQL1 + _T(" AND ") + strPAexists + _T(" AND ")  + strSA + _T(" AND ") + strSAexists );
			if(rs1.IsOpen()) rs1.Close();
			rs1.Open(dbOpenSnapshot,SQLx);
			if( !(rs1.IsEOF() && rs1.IsBOF()) )
			{
				//�����ƹܲ�,���ƹܲ�������ڣ����Ƹ���/����������ڣ���
			}
			else
			{
				//�����ƹܲ�,���ƹܲ�������ڣ����Ƹ���/����������ڣ�û��
				SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
					SQL1 + _T(" AND ") + strSA + _T(" AND ") + strSAexists );
				if(rs1.IsOpen()) rs1.Close();
				rs1.Open(dbOpenSnapshot,SQLx);
				if( !(rs1.IsEOF() && rs1.IsBOF()) )
				{
					//�����ƹܲ�,�����ƹܲ�������ڣ����Ƹ���/����������ڣ���
				}
				else
				{
					//�����ƹܲ�,�����ƹܲ�������ڣ����Ƹ���/����������ڣ�û��
					SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
						SQL1 + _T(" AND ") + strSAexists );
					if(rs1.IsOpen()) rs1.Close();
					rs1.Open(dbOpenSnapshot,SQLx);
					if( !(rs1.IsEOF() && rs1.IsBOF()) )
					{
						//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ��������Ƹ���������ڣ���
					}
					else
					{
						//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ��������Ƹ���������ڣ�û��
						SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
							SQL1 );
						if(rs1.IsOpen()) rs1.Close();
						rs1.Open(dbOpenSnapshot,SQLx);
						if( !(rs1.IsEOF() && rs1.IsBOF()) )
						{
							//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ����������Ƹ���������ڣ���
						}
						else
						{
							//�����ƹܲ�,�����ƹܲ�������ڣ������Ƹ����������Ƹ���������ڣ�û��
							SQL1=(modPHScal::ZdjType == _T("9999") || modPHScal::ZdjType == _T("5000") ? _T("iNumCSPR>=1") : _T("iNumSPR>=1"));
							SQLx.Format(_T("SELECT * FROM phsStructureName WHERE %s ORDER BY Frequence DESC,iNumOfPart"),
								SQL1 );
							if(rs1.IsOpen()) rs1.Close();
							rs1.Open(dbOpenSnapshot,SQLx);
							if( !(rs1.IsEOF() && rs1.IsBOF()) )
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
		
		/*
		if (iLIST == mnuiPASALimited)
		{
		//���ƹܲ�����������Ϊ�û�ָ����ʽ
		if(PA == _T("") )
		{
		//�ܲ�Ϊ��
		if( SA == _T("") )
		{
		//�ܲ�Ϊ�գ�����Ϊ��
		SQLx=_T("SELECT * FROM phsStructureName WHERE ")+SQL1+strSprNum+ _T("ORDER BY Frequence DESC,iNumOfPart");
		
		  rs1.Open(dbOpenSnapshot,SQLx);
		  if( rs1.IsEOF() && rs1.IsBOF() )
		  {
		  sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDSprNum),strSprNum);
		  throw sTmp;
		  }
		  }
		  else
		  {
		  //�ܲ�Ϊ�գ�������Ϊ��
		  SQLx = _T("SELECT * FROM phsStructureName WHERE  trim(SA)=\'") + SA + _T("\' AND ")+SQL1+strSprNum+ _T(" ORDER BY Frequence DESC,iNumOfPart");
		  //rs1.m_pDatabase=&EDIBgbl::dbSORT;
		  rs1.Open(dbOpenSnapshot,SQLx);
		  if (rs1.IsEOF() && rs1.IsBOF())
		  {
		  sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDSASprNum),strSprNum,SA);
		  ShowMessage(sTmp);
		  if(!modPHScal::gbAutoPASA)
		  return -1;
		  else
		  {
		  //�ܲ�Ϊ�գ�����Ϊ�գ�����ѡ��
		  //�ܲ�Ϊ�գ�����Ϊ��
		  SQLx = _T("SELECT * FROM phsStructureName WHERE ") +SQL1+strSprNum+  _T(" ORDER BY Frequence DESC,iNumOfPart");
		  rs1.Close();
		  rs1.Open(dbOpenSnapshot,SQLx);
		  if( rs1.IsEOF() && rs1.IsBOF())
		  {
		  sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDSprNum),strSprNum);
		  throw sTmp;
		  }
		  }
		  }
		  }
		  }
		  else
		  {
		  //�ܲ���Ϊ��
		  if( SA == _T(""))
		  {
		  //�ܲ���Ϊ�գ�����Ϊ��
		  SQLx = _T("SELECT * FROM phsStructureName WHERE  trim(PA)=\'")+PA +_T("\' AND ") +SQL1+strSprNum+ _T(" ORDER BY Frequence DESC,iNumOfPart");
		  rs1.Open(dbOpenSnapshot,SQLx);
		  if(rs1.IsEOF()&& rs1.IsBOF())
		  {
		  sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDPASprNum),strSprNum,PA);
		  ShowMessage(sTmp);
		  if(!modPHScal::gbAutoPASA)
		  return -1;
		  else
		  {
		  //�����ƹܲ�������Ϊ�գ�����ѡ��
		  //�ܲ�Ϊ�գ�����Ϊ��
		  SQLx = _T("SELECT * FROM phsStructureName WHERE ")+SQL1+strSprNum+ _T(" ORDER BY Frequence DESC,iNumOfPart");
		  rs1.Close();
		  rs1.Open(dbOpenSnapshot,SQLx);
		  if(rs1.IsEOF()&& rs1.IsBOF())
		  {
		  sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDSprNum),strSprNum);
		  throw sTmp;
		  }
		  }
		  }
		  }
		  else
		  {
		  //�ܲ���Ϊ�գ�������Ϊ��
		  SQLx = _T("SELECT * FROM phsStructureName WHERE  trim(PA)=\'") +PA + _T("\' AND  trim(SA)=\'") +SA +_T("\' AND ") +SQL1+strSprNum+  _T(" ORDER BY Frequence DESC,iNumOfPart");
		  rs1.Open(dbOpenSnapshot,SQLx);
		  if( rs1.IsEOF()&& rs1.IsBOF())
		  {
		  sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDPASASprNum),strSprNum,PA,SA);
		  ShowMessage(sTmp);
		  if(!modPHScal::gbAutoPASA)
		  return -1;
		  else
		  {
		  //�����ƹܲ�������ѡ��
		  //�ܲ�Ϊ�գ�������Ϊ��
		  SQLx = _T("SELECT * FROM phsStructureName WHERE  trim(SA)=\'")+SA +_T("\' AND ")+SQL1+strSprNum+ _T(" ORDER BY Frequence DESC,iNumOfPart");
		  rs1.Close();
		  rs1.Open(dbOpenSnapshot,SQLx);
		  if( rs1.IsEOF()&& rs1.IsBOF())
		  {
		  sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDSASprNum),strSprNum,SA);
		  ShowMessage(sTmp);
		  //�����ƹܲ��������Ƹ���������ѡ��
		  //�ܲ�Ϊ�գ�����Ϊ��
		  SQLx = _T("SELECT * FROM phsStructureName WHERE ")+SQL1+strSprNum+ _T(" ORDER BY Frequence DESC,iNumOfPart");
		  rs1.Close();
		  rs1.Open(dbOpenSnapshot,SQLx);
		  if( rs1.IsEOF()&& rs1.IsBOF())
		  {
		  sTmp.Format(GetResStr(IDS_NotFoundThisSampleIDSprNum),strSprNum);
		  ShowMessage(sTmp);
		  return -1;
		  }
		  }
		  }
		  }
		  }
		  }
		  }
*/
		if(!rs1.IsOpen() || rs1.IsEOF()||rs1.IsBOF())
			return -1;
		rs1.MoveFirst();
		rs1.GetFieldValue(_T("SampleID"),vTmp);
		ret=vtoi(vTmp);
		rs1.GetFieldValue(_T("PA"),vTmp);
		PA = vtos(vTmp);
		rs1.GetFieldValue(_T("SA"),vTmp);
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
	   catch(CDaoException * e)
	   {
		   e->ReportError();
		   e->Delete();
		   return -1;
	   }
	   catch(...)
	   {
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
	catch(...)
	{
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
		CDaoRecordset rs(&EDIBgbl::dbPRJ);
		//	int i;
		CString sTmp,sTmp2,SQLx;
		sTmp.Format(_T("%d"),modPHScal::iSelSampleID);
		SQLx = _T("SELECT *  FROM phsStructureREF WHERE SampleID=") + sTmp + _T(" ORDER BY seq");
		rs.Open(dbOpenSnapshot,SQLx);
		if(rs.IsBOF() && rs.IsEOF())
		{
			sTmp2.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),sTmp);
			ShowMessage(sTmp2);
			return IDS_NoSampleIDinphsStructureREF;
		}
		else
		{
			rs.MoveLast();
			rs.MoveFirst();
			if(rsTmpREF.IsOpen())
				rsTmpREF.Close();
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("rsTmpREF")))
				EDIBgbl::dbPRJ.Execute(_T("DELETE * FROM rsTmpREF"));
			else
				EDIBgbl::dbPRJ.Execute(_T("CREATE TABLE rsTmpREF (CustomID char (20))"));
			COleVariant vTmp;
			while(!rs.IsEOF())
			{
				rs.GetFieldValue(_T("CustomID"),vTmp);
				SQLx = _T("INSERT INTO rsTmpREF (CustomID) VALUES (\'") + vtos(vTmp) + _T("\')");
				EDIBgbl::dbPRJ.Execute(SQLx);
				rs.MoveNext();
			}
			
			SQLx = _T("SELECT CustomID FROM rsTmpREF");
			
			this->rsTmpREF.m_pDatabase=&EDIBgbl::dbPRJ;
			rsTmpREF.Open(dbOpenDynaset,SQLx);
			return -1;
		}
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
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
	CDaoRecordset rsTmpZB(&EDIBgbl::dbPRJDB); 
	//rsTmpZB.m_pDatabase=&EDIBgbl::dbPRJDB;
	CString SQLx;
	_variant_t vTmp;

	CString * SampleArray = NULL;
	try
	{
		//��ʼ�����
		k = 1;
		//ɾ�����������ֶ�zdjhֵ=zdjh�ļ�¼
		//modPHScal::MakeZDJ_ZBZG();
		CDaoRecordset rsX;
		//���ӱ��������IDֵ
		if(!rsConnect.IsOpen())
		{
			SQLx = _T("SELECT *  FROM connect");
			rsConnect.m_pDatabase=&EDIBgbl::dbPRJ;
			rsConnect.Open(dbOpenDynaset,SQLx);
		}
		//ģ���������ID��CustomIDֵ
		
		sTmp.Format(_T("%d"),modPHScal::iSelSampleID);
		SQLx = CString(_T("SELECT *  FROM phsStructureREF WHERE SampleID=")) + sTmp + _T(" ORDER BY seq");
		rsX.m_pDatabase=&EDIBgbl::dbPHScode;//20071018 "dbSORT" ��Ϊ "dbPHScode"
		rsX.Open(dbOpenDynaset,SQLx);
		if(rsX.IsEOF() && rsX.IsBOF())
		{
			sTmp2.Format(_T("%d"),modPHScal::iSelSampleID);
			sTmp.Format(GetResStr(IDS_NoSampleIDinphsStructureREF),sTmp2);
			ShowMessage(sTmp);
			ret = IDS_NoSampleIDinphsStructureREF;
		
			return ret;
		}
		else
			rsX.MoveLast();
		long iCount = rsX.GetRecordCount();
		COleVariant rsXv;

		SampleArray = new CString[iCount];
		for(int i =iCount-1;i >= 0 ;i--)
		{
			rsX.GetFieldValue("ID",rsXv);
			SampleArray[i] = vtos(rsXv);
			rsX.MovePrev();
		}

		
		SQLx = CString(_T("SELECT *  FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") +ltos(zdjh) + _T(" AND VolumeID=")+ ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY RECNO");
		rsTmpZB.Open(dbOpenDynaset,SQLx);
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
			else if( i == rsX.GetRecordCount() - 1)
				rsza->PutCollect(_T("gn1"),mvCustomID);
			_variant_t tv;
			rsTmpZB.AddNew();
			rsTmpZB.SetFieldValue(_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
			tv.ChangeType(VT_I4);
			tv.intVal=zdjh;
			rsTmpZB.SetFieldValue(_T("zdjh"),tv);
			tv.ChangeType(VT_I2);
			tv.iVal=k;
			rsTmpZB.SetFieldValue(_T("recno"),tv);
			tv.iVal = nth;
			rsTmpZB.SetFieldValue(_T("nth"),tv);

			rsTmpZB.SetFieldValue(_T("index"),variant_t((short)vtoi(modPHScal::sFindFLD(_T("ID"), _T("Index"),SampleArray[i]))));
			
			//���Ǹ���
			tv.iVal=(short)vtoi(mvClassID);
			rsTmpZB.SetFieldValue(_T("ClassID"),tv);
			rsTmpZB.SetFieldValue(_T("ID"),STR_VAR(SampleArray[i]));
			rsTmpZB.SetFieldValue(_T("CustomID"),STR_VAR(modPHScal::sFindCustomID(SampleArray[i])));
			rsTmpZB.Update();
			if (i< iCount-1)// Then
			{
				//ȡ����һ�εĿ�ͷ���
				if(!rsConnect.FindFirst (CString(_T("trim(cntb)=\'")) + (SampleArray[i]) + _T("\' AND trim(cnte)=\'") + (SampleArray[i+1]) + _T("\'")))
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
							rsConnect.GetFieldValue( sTmp,v1);
							if (vtos(v1) != _T("") )
							{
								k = k + 1;
								rsTmpZB.AddNew();
								rsTmpZB.SetFieldValue(_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
								rsTmpZB.SetFieldValue(_T("zdjh"),_variant_t((long)zdjh));
								rsTmpZB.SetFieldValue(_T("recno"),_variant_t((short)k));
								rsTmpZB.SetFieldValue(_T("nth"),_variant_t((short)nth));

								//�Ǹ���
								rsTmpZB.SetFieldValue(_T("ClassID"),_variant_t((short)vtoi(modPHScal::sFindFLD(_T("ID"), _T("ClassID"),vtos(v1)))));
								//����sFindCustomID����ȡ������ֵ
								rsConnect.GetFieldValue(_T("CntbF")+ltos(j)+_T("num"),v2);
								rsTmpZB.SetFieldValue(_T("CLnum"),v2);

								rsTmpZB.SetFieldValue(_T("ID"),v1);
								rsTmpZB.SetFieldValue(_T("CustomID"),STR_VAR(modPHScal::sFindCustomID(vtos(v1))));
								rsTmpZB.Update();
							}
						}
					}
				}
			}
			k = k + 1;
		}
		ret = -1;
		rsTmpZB.Close();
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(_com_error )
	{
		//ShowMessage(e.Description());
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
		CDaoRecordset rs(&EDIBgbl::dbPRJDB);
		TZBResultObj->ResetContent();
		
		CString SQLx;
		SQLx.Format(CString(_T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=%d AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 ORDER BY recno"),modPHScal::zdjh);
		rs.Open(dbOpenSnapshot,SQLx);
		if(rs.IsEOF() && rs.IsBOF())
		{
			throw GetResStr(IDS_NoComponentForZDJHInTZB);
		}
		rs.MoveLast();
		COleVariant vTmp;
		rs.GetFieldValue(_T("CustomID"),vTmp);
		modPHScal::UpdateTZB4zdjh(_T("gn1"), _variant_t(vtos(vTmp)));
		rs.MoveFirst();
		rs.GetFieldValue(_T("CustomID"),vTmp);
		modPHScal::UpdateTZB4zdjh(_T("dn1"), _variant_t(vtos(vTmp)));
		while(!rs.IsEOF())
		{
			rs.GetFieldValue(_T("CustomID"),vTmp);
			TZBResultObj->AddString(vtos(vTmp));
			rs.MoveNext();
		}
		rs.Close();
	}
	catch(CString )
	{
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
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
			EDIBgbl::dbPRJDB.Execute(SQLx);
		}
		else
		{
			SQLx = CString(_T("DELETE  FROM [rstmp")) +EDIBgbl::Btype[EDIBgbl::TZB]+ _T("] WHERE zdjh=") + sTmp + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
			EDIBgbl::dbPRJ.Execute(SQLx);
		}
		if(rsTZB.IsOpen())
			rsTZB.Close();
		if( iDbID == idbPRJDB )
		{
			SQLx = CString(_T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + sTmp + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
			rsTZB.m_pDatabase=&EDIBgbl::dbPRJDB;
			rsTZB.Open(dbOpenDynaset,SQLx);
		}
		else
		{
			SQLx = CString(_T("SELECT * FROM [rstmp")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + sTmp + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
			rsTZB.m_pDatabase=&EDIBgbl::dbPRJ;
			rsTZB.Open(dbOpenDynaset,SQLx);
		}
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
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
		CDaoRecordset rsTmp,rsX;
		rsTmp.m_pDatabase=&EDIBgbl::dbSORT;
		rsX.m_pDatabase=&modPHScal::dbZDJcrude;
		
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
		//SQLx = _T("SELECT * FROM connect");
		//rsConnect.m_pDatabase=&EDIBgbl::dbPRJ;
		//rsConnect.Open(dbOpenSnapshot,SQLx);
		
		
		//Set mvarrsTmpREF1 = rsTmpREF.Clone
		/*if(rsTmpREF.IsOpen())
		rsTmpREF.Requery();
		else
		{
		rsTmpREF.m_pDatabase=&EDIBgbl::dbPRJ;*/
		COleVariant v,v1,v2;
		if( !rsTmpREF.IsOpen() || (rsTmpREF.IsEOF() && rsTmpREF.IsBOF() )){
			throw (GetResStr(IDS_Null_rsTmpREF));
			return false;
		}
		//����Ptype���鱣����mvarrsTmpREF�Ŀ���
		//�Ա�ȡ��֧������ϵ�����2���������,���в���
		if(!rsID.IsOpen())
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
			brsIDStatus=TRUE;
		}
		if( ! rsTmpREF.IsEOF() )
		{
			rsTmpREF.MoveLast();
			//չ��sFindTBN���ӿ��ٶȡ�
			//tbn1 = sFindTBN(Trim(rsTmpREF.Fields(_T("CustomID"))))
			rsTmpREF.GetFieldValue(_T("CustomID"),v);
			if( !rsID.FindFirst(_T("trim(CustomID)=\'") + vtos(v) + _T("\'")))
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
			rsID.GetFieldValue(_T("Index"),v);
			mvSA = vtoi(v);
			
			if( rsTmpREF.GetRecordCount() >= 2 )
			{
				//����������ڻ����2���������ȡ��ͷβ�����
				rsTmpREF.MoveFirst();
				//չ��sFindTBN���ӿ��ٶȡ�
				//tbn1 = sFindTBN(Trim(rsTmpREF.Fields(_T("CustomID"))))
				rsTmpREF.GetFieldValue(_T("CustomID"),v);
				if( !rsID.FindFirst(CString(_T("trim(CustomID)=\'")) + vtos(v) + _T("\'")))
				{
					//û�ҵ�CustomID,����
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(v));
					throw (sTmp);
					return false;
				}
				//�����һ������ǹܲ�������ܲ�����롣
				rsID.GetFieldValue(_T("Index"),v);
				mvPA = vtoi(v);
				
			}
			CString sv,sv1;
			//������������ʼֵ=0
			mvarSATotalWeight = 0;
			//��ʼ����
			for (i = 0 ;i< rsTmpREF.GetRecordCount();i++)
			{
				//���������ڵ�ԭʼ���ݱ�tbn1,�Ա����
				//չ��sFindTBN���ӿ��ٶȡ�
				//tbn1 = sFindTBN(Trim(rsTmpREF.Fields(_T("CustomID"))))
				rsTmpREF.GetFieldValue(_T("CustomID"),v);
				sv=vtos(v);
				
				if(!rsID.FindFirst(CString(_T("trim(CustomID)=\'")) + sv + _T("\'")))
				{
					//û���ҵ�����
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(v));
					throw (sTmp);
					return false;
				}
				else 
				{
					rsID.GetFieldValue(_T("ClassID"),v);
					modPHScal::glClassID = vtoi(v);
					rsID.GetFieldValue(_T("Index"),v);
					modPHScal::glIDIndex =vtoi(v);
					rsID.GetFieldValue(_T("ID"),v);
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
					if( i < rsTmpREF.GetRecordCount() - 1 )
					{
						rsID.GetFieldValue(_T("ID"),v);
						tmpID1=vtos(v);
						/*rsID.FindFirst(CString(_T("trim(CustomID)=\'")) + sv + _T("\'"))*/
						if( rsID.FindFirst(CString(_T("trim(CustomID)=\'")) + sv + _T("\'")))
						{
							rsID.GetFieldValue(_T("ID"),v);
							tmpID1 = vtos(v);
						}
						//�������һ�����\
						//��Connect���м������Ƿ���ڼ���ƥ��ĳߴ���Ϣ
						//tmpCustomID1 = Ptype(i + 1);
						rsTmpREF.MoveNext();
						if(!rsTmpREF.IsEOF())
						{
							rsTmpREF.GetFieldValue(_T("CustomID"),v);
							sv1=vtos(v);
							sv1.TrimLeft();sv1.TrimRight();
							tmpCustomID1=sv1;
						}
						rsTmpREF.MovePrev();
						if( !rsConnect.FindFirst(CString(_T("trim(cntb)=\'")) + tmpID0 + _T("\' AND trim(cnte)=\'") + tmpID1 + _T("\'")) )
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
						if( IsNull(rsConnect.Fields(_T("ConnectCondition"))) ){
						tmpSQL0 = _T("")
						}else {
						tmpSQL0 = UCase(Trim(rsConnect.Fields(_T("ConnectCondition"))))
						}
							/*}*/
							rsConnect.GetFieldValue(_T("CntbNum"),v);                
							if( vtoi(v)<= 0 )
							{
								iCntbNum = 1;
							}else 
							{
								iCntbNum = vtoi(v);//rsConnect.Fields(_T("CntbNum"))
							}
							rsConnect.GetFieldValue(_T("CnteNum"),v);                
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
				else if( i == rsTmpREF.GetRecordCount() - 1 && modPHScal::glIDIndex == iSA )
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
						if(rsX.IsOpen())
							rsX.Close();
						rsX.Open(dbOpenSnapshot,SQLx);
						if( rsX.IsEOF() && rsX.IsBOF() )
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
						if(rsX.IsOpen())
							rsX.Close();
						rsX.Open(dbOpenSnapshot,SQLx);
						if( rsX.IsEOF() && rsX.IsBOF() ){
						}else {
							//�ҵ����˳�ѭ��
							goto commonHandle;
						}
						
					}

					//2007.09.27(start)
					if(modPHScal::gbPAForceZero && (rsX.IsEOF() && rsX.IsBOF()))
					{
						for (k = 0 ;k<=(int) modPHScal::gnDW_delta;k++)
						{
							//SQLx = _T("SELECT * FROM ") & tbnPA & _T(" WHERE Dw>= ") & modPHScal::dj * (1 - k * 0.01) & _T(" AND Dw<= ") & modPHScal::dj * (1 + k * 0.01) & _T(" AND Pmax>=") & tmpSelPJG / iNumPart & _T(" AND Tj>=") & tmpT0 & _T(" AND trim(CustomID)='") & Ptype(i) & _T("' ORDER BY Dw,tj,Weight")
							
							sTmp.Format(_T("%g"),modPHScal::dj * (1 - k * 0.01));
							SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Dw>= ") + sTmp;
							sTmp.Format(_T("%g"),modPHScal::dj * (1 + k * 0.01));
							SQLx+=(_T(" AND Dw<= ") + sTmp + _T(" AND Pmax IS NULL ")) ;
							SQLx+= (_T(" AND Tj>=") + stmpT0 + _T(" AND trim(CustomID)=\'") + sv + _T("\' ORDER BY tj,Dw,Weight"));
							if(rsX.IsOpen())
								rsX.Close();
							rsX.Open(dbOpenSnapshot,SQLx);
							if( rsX.IsEOF() && rsX.IsBOF() ){
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
					
					/*  if(rsX.IsOpen())
					rsX.Close();
					rsX.Open(dbOpenSnapshot,SQLx);*/
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
			 if(rsX.IsOpen())
				 rsX.Close();
			 rsX.Open(dbOpenSnapshot,SQLx);
         }
spZ1Z2:
         if( rsX.IsEOF() && rsX.IsBOF() )
		 {
			 //Err.Raise iUE_NotFoundPartInZDJCrude, , ResolveResString(iUE_NotFoundPartInZDJCrude, _T("|1"), Ptype(i), _T("|2"), SQLx, _T("|3"), Iif((i > 0, Ptype[i-1], _T("")), _T("|4"), Iif((i < rsTmpREF.RecordCount - 1, Ptype(i + 1), _T("")))
			 if( i > 0 )
			 {
				 CString sv2;
				 rsTmpREF.MovePrev();
				 if(!rsTmpREF.IsBOF())
				 {
					 rsTmpREF.GetFieldValue(_T("CustomID"),v);
					 sv2=vtos(v);
				 }
				 rsTmpREF.MoveNext();
				 if( i < rsTmpREF.GetRecordCount() - 1 ){
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
				 rsTmpREF.MovePrev();
				 if(!rsTmpREF.IsBOF())
				 {
					 rsTmpREF.GetFieldValue(_T("CustomID"),v);
					 sv2=vtos(v);
				 }
				 rsTmpREF.MoveNext();
				 sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx,sv,_T("NULL"),sv1);
				 throw (sTmp);
				 return false;
			 }
         }
		 else 
		 {//1
			 rsX.MoveFirst();
			 if(phsAvailableTypeRs.IsOpen())
				 phsAvailableTypeRs.Close();
			 phsAvailableTypeRs.m_pDatabase=rsX.m_pDatabase;
			 phsAvailableTypeRs.Open(dbOpenDynaset,rsX.GetSQL());
			 //phsAvailableTypeRs=CDaoRecordset::GetSQL()
			 //Set phsAvailableTypeRs = rsX
			 //�ܲ����Լ��㣬��ʱ�����˳�
			 if( modPHScal::gbAddPartWeight2PMax && modPHScal::glIDIndex != iSA )
			 {
				 //�ۼ��������������
				 //���Ը�������˨��ĸ���������Լ���������������1m���ǣ�
				 rsX.GetFieldValue(_T("Weight"),v);
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
         rsTmpREF.MoveNext();
      }//2
      return true;
   }
   return false;
   }
   catch(::CDaoException * e)
   {
	   e->ReportError();
	   e->Delete();
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
   catch(...)
   {
	   return false;
   }
}

void Cphs::simplify2(CDaoRecordset &rsPartBoltNuts,int nth)
{
	CString SQLx;
	try
	{
		
        SQLx.Format("SELECT * FROM [ZB] WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND ( [ClassID]=%d OR [ClassID]=%d OR [ClassID]=%d ) AND [IsSAPart]<>-1 ORDER BY [recno]",
			EDIBgbl::SelVlmID,modPHScal::zdjh ,nth,iBolts,iNuts,iAttached);
		rsPartBoltNuts.Open(dbOpenDynaset,SQLx);				
	}
	catch(::CDaoException *e)
	{
		e->ReportError();
		e->Delete();
	}
	
	
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::simplify(CDaoRecordset &rsSAPart,int nth)
{
	try{
		CString SQLx;
		SQLx.Format("SELECT * FROM [ZB] WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND [IsSAPart]<>0 AND IsSAPart IS NOT NULL ORDER BY recno ",
			EDIBgbl::SelVlmID,modPHScal::zdjh,nth);
		rsSAPart.Open(dbOpenDynaset,SQLx);
		if( rsSAPart.IsBOF() && rsSAPart.IsEOF() )
			;
		else
		{
			//ɾ��ZB���б�·֧���ܵĸ������������ظ���Ӹ����ĸ���	
			if(!rsSAPart.IsBOF()) 
				rsSAPart.MoveFirst();
			for(; !rsSAPart.IsEOF(); rsSAPart.MoveFirst())
			{		
				rsSAPart.Delete();
				if( rsSAPart.IsEOF() )
					break;	
			}
		}
	}
	catch(::CDaoException *e)
	{
		e->ReportError();
		e->Delete();
	}
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
bool Cphs::GetphsBHandSizes(CFrmStatus& frmStatus,int  FirstCal, int  MaxCalCount,int nth,float& TmpPaz,float& TmpPgz)
{
	//	return true;
	//Ŀ��:��ԭʼ���ݱ��в���ÿ������ı��,�ߴ�,����,����,����,���������������
	//����:CFrmStatus& frmStatus,��ʾ���ȵĴ���;
	//int  FirstCal,��ĳ��֧����ĳ·�ļ������;
	//��һ�����,FirstCal=1;�ڶ�����㣬FirstCal=2;��������㣬FirstCal=3;
	// int  MaxCalCount,��������������Ե���ֻ����һ�飬���Ե��ܼ���2�飻
	// int nth,֧����·�������԰�ĳ·������λ֧���ܡ�
	//����:���ӳɹ�,true;����False
	//gdw��������λ��ƫװ��������GetPhsBHandSizes�и���ƫװ��ĸ�����λֵ���
//	AfxMessageBox("begin");
	CString dbstr = "\n" ;
	bool ret=false;
	CString strSelNumCS;//ѡ��������۸������Ĺ����ַ���
	CString *PtypeID=NULL;   //��������������ṹID����
	CString *Ptype=NULL;   //��������������ṹCustomID����
	long *PtypeClassID=NULL;   //��������������ṹClassID����
	long *PtypeIndex=NULL;  //��������������ṹIndex����
	
	float m_fDiaM1 = 0.0f;//����ܼ���˨M1��ֵ��
	CString strDiaM1FieldName;//�ܼ���˨M1�ĳߴ��ֶΣ�һ��Ϊsize3��size2
	try
	{
		CString strPipeClampBoltDiameterChanged;//����ı�����˨ֱ���Ĺܼ�ID��־�ַ�
		_variant_t vnil;
		CString SQL1;
		CString  strClbz;     //add by LFX 2005.5.20
		
		vnil.ChangeType(VT_NULL);
		CString LogicalOpr = _T("=<>") ;  //�߼����������
		int iLOPnum = 3 ;         //�߼�������������ȣ����ڳߴ�����ƥ��Ƚ�
		float msOpr=0;long n=0,m=0;
		bool bFound=0;
		CString sTmpSQL,SQLx;   //��ʱ����SQLx
		
		//����һ��������ڼ���̶�֧�ܵĵ�����sngSAfixP
		float sngSAfixP=0;   //�̶�֧�ܵĵ�����sngSAfixP
		float sngSAfixPl=0 ; //��̬�̶�֧�ܵĵ�����sngSAfixPl
		float sngSAfixPr=0 ; //��̬�̶�֧�ܵĵ�����sngSAfixPr
		//float PAfixH=0 ;  //�̶�֧�ܹܲ��߶�H,cm
		float F1ToWx1=0 ;    //������͸ֺ��йص��������ݣ�������zdjcrudeXBDLY�Ĳ۸����Ա�SSteelPropertyCS��F1ToWx1�ֶ�
		float PAfixC1=0 ;    //�̶�֧��C1ֵ,mm
		float mvBWidth=0 ;     //�̶�֧�ܸ������۸ּ��,cm,G47=������chdist�ֶ�ֵ��G48=TZA��A01�ֶ�ֵ
		float pnl =0 , pnr=0 ;  //�̶�֧��Z1�ܲ���˨��/��̬���ܵ�����Z2�ϳ���,kgf
		float ml =0 , mr=0  ;   //�̶�֧��Z2�ĺϳ�����,kgf.m
		float pxl =0 , pzl =0 , pxr  =0, pzr =0 ; //�̶�֧������̬���ܵ���,kgf
		float mxl =0 , myl  =0, mzl =0 , mxr =0 , myr  =0, mzr=0 ; //�̶�֧������̬���ܵ�����,kgf.m
		//	float sngCoef;     //�̶�֧�ܵ�ϵ��
		
		//modPHScal::gdw=����ƫװ�����ĸ�����λֵ,mm
		//modPHScal::gdw1=�û������δ����ƫװ�����ĸ�����λֵ,mm
		float Height_SPRINGsL5 =0;   //���ɵĻ�����˿����
		//float modPHScal::DblHangerRodDist=0 ;  //˫���ļ��,�Ƶ�modPHScalģ�飬��Ϊȫ�ֱ�����
		float p1 =0, p2=0   ; //�����Զ�ѡ��˫�����֧���۸��ͺŵĺ���,kgf
		float tmpPJG=0 , tmpT0 =0, tmpGDW1 =0, tmpPmax4PA =0, tmpPmax4Part =0;
		float tmpSelPJG =0 ;//�ۼ����������ļ������
		float tmpSelPJG4SPRPaz =0; //�ۼ����������ĵ��ɰ�װ���أ����ڼ��㵯�ɹ��Ҳ�����ɰ�װ���أ���������ǰ������������=֧���㰲װ����+�㲿������
		float tmpSelPJG4SPRPgz =0; //�ۼ����������ĵ��ɼ�����أ����ڼ��㵯�ɹ��Ҳ�����ɹ������أ���������ǰ������������=֧���㹤������+�㲿������
		float tmpSelPJG4SPRPgzNext=0;  //�ۼ����������ļ�����أ����ڼ�����һ�����ɹ��Ҳ����һ�����ɹ�������أ�����ǰһ������������tmpSelPJG4SPRPgzNext=tmpSelPJG4SPRPgz+ǰһ����������
		float tmpSelPJG4SPRPazNext=0;  //�ۼ����������ļ�����أ����ڼ�����һ�����ɹ��Ҳ����һ�����ɹ�������أ�����ǰһ������������tmpSelPJG4SPRPgzNext=tmpSelPJG4SPRPgz+ǰһ����������
		float tmpSelPJG4CSPRPgz =0; //�ۼ����������ļ�����أ����ڼ��������Ҳ������������أ���������ǰ�����������=֧���㹤������+�㲿������
		float tmpSelPJG4CSPRPgzNext=0;  //�ۼ����������ļ�����أ����ڼ�����һ��������Ҳ����һ�������������أ�����ǰһ�����������tmpSelPJG4CSPRPgzNext=tmpSelPJG4CSPRPgz+ǰһ���������
		float tmpSelPJG4CSPRFiJ =0 ;//��ʱ�ۼ����������ļ�����أ����ڼ���������������ʱ�ĺ��أ�Ҳ������ɽ���ĸֱ���������
		float tmpSelPJG4CSPRFiK=0 ; //��ʱ�ۼ����������ļ�����أ����ڼ���������ӵ�����ʱ�ĺ��أ�Ҳ������·�����ΪtmpSelPJG,�Ϸ�����ΪtmpSelPJG4CSPRFiJ+Weight
		float tmpSelOpLoad =0  ; //����λ��ԣ��֮����õĹ��������غ�ֵtmpSelOpLoad
		//���������ʧ,��Ĭ��Ϊ���ء��¶ȡ�������λ����Ӧ��ȫ������
		if( iChkCondition==0 ) iChkCondition = iPJG + iTJ + iGDW1 + iCNT + iDW;
		//��Ϊ���˵�����������Ӧ���ۼ��㲿��������
		//��������tmpPJG��Ϊ���ؼ����׼��
		tmpPJG = fabs(modPHScal::pjg);
		//�ܲ����㲿��ѡ�����tmpPmax4PartҲʹ��tmpPmax4PA����Ϊ����
		if( modPHScal::gbCalPARTbyMaxLoad )
			tmpPmax4Part = (fabs(modPHScal::pgz) >= fabs(modPHScal::paz) ? fabs(modPHScal::pgz) : fabs(modPHScal::paz));
		else
			tmpPmax4Part = fabs(modPHScal::pjg);
		
		//���ü�����س�ʼֵ���Ա��ۼ�
		tmpSelPJG = tmpPmax4Part;
		//���ú���ɽ���ĸֱ�������غɳ�ʼֵ���Ա��ۼ�
		if( modPHScal::gbCalCSPRLugDiaByPjg )
			tmpSelPJG4CSPRFiJ = tmpPmax4Part;
		else
			tmpSelPJG4CSPRFiJ = fabs(modPHScal::pgz);
		
		//����������غɳ�ʼֵ=֧���㹤�����أ��Ա��ۼ�
		tmpSelPJG4CSPRPgz = fabs(modPHScal::pgz);
		//���ɹ������غɳ�ʼֵ=֧���㹤�����أ��Ա��ۼ�
		tmpSelPJG4SPRPgz = fabs(modPHScal::pgz) ;
		tmpSelPJG4SPRPaz = fabs(modPHScal::paz);
		tmpT0 =modPHScal::t0;

		CString strOrderBy ;      //�����ַ���
		CString strTmp ;          //��ʱ�ַ�������
		CString tmpSQL ;          //��ǰ������ƥ���ѯ�ֶ���Ϣ
		CString tmpSQL0 ;         //ǰһ�ε�����ƥ���ѯ�ֶ���Ϣ
		long mlfx =0, mlfx0 =0; //���/ǰһ���������ͼ�ķ���
		long mlRot =0, mlRot0 =0; //���/ǰһ�����Y��ķ�������з�����ͬ��=0���෴��=180��
		//long lngErrCalPAfix;   //����̶�֧��ʱ�����Ĵ����
		_Recordset* rsza ;
		CDaoRecordset rsX(&modPHScal::dbZDJcrude) ;
		rsza=FrmTxsr.m_pViewTxsr->m_ActiveRs;
		
		CDaoRecordset rsPartBoltNuts(&EDIBgbl::dbPRJDB); //�����˨��ĸ
		CDaoRecordset rsSAPart(&EDIBgbl::dbPRJDB); //��ɸ����ĸ�����

		CDaoRecordset rsTZB;//��ǰ֧���ܵ�ǰ·�����������㲿���ı�������˨��ĸ������¼������
		
		CDaoRecordset rs1(&modPHScal::dbZDJcrude);
		CDaoRecordset rs3(&EDIBgbl::dbPHScode);//���Ϲ淶ѡ���¼��//20071018 "dbSORT" ��Ϊ "dbPHScode"
		CString m_strMaterial;//�ܲ����²�������㲿��ѡ��Ĳ���
		CDaoRecordset rs(&modPHScal::dbZDJcrude);
		CDaoRecordset rsTmp,rsTmp1,rsTmp2;
		CDaoRecordset rsCal;	//����ǿ�ȼ��㹫ʽ��CalFormulaOfFixPASA��λ��sort.mdb���û����޸�
		CDaoRecordset rsDiaOfCSPRFiJ(&modPHScal::dbZDJcrude);//������������ֱ�����¼�����ü���Դ�ڱ�tbnLugDiaOfCSPR���Ϸ�����ɽ���ĸ���ز�Ҫ���Ǻ������
		CDaoRecordset rsDiaOfCSPRFiK;//�����������Ӷ���ֱ�����¼�����ü���Դ�ڱ�tbnLugDiaOfCSPR�����Ӷ��Ӻ���Ҫ���Ǻ������
		rsDiaOfCSPRFiK.m_pDatabase=&modPHScal::dbZDJcrude;
		CString tbn1,SAfDPmax;
		bool m_bFoundL3=false;//������ܼ����ӵ�˫�׵���(��ʱ�ɲ��ػ�С�ܼ���˨)
		//��һ�μ��㣬ֱ����ʼ��
		if( FirstCal == 1 )
		{
			modPHScal::gmiDiameter = modPHScal::giDiameter = 0;
		}
		long i , j, k ;
		CString sBHFormat, sBH ;
		CString sPartID, sRodPASA;
		CString tmpCustomID0 , tmpCustomID1 ;
		CString tmpID0, tmpID1 ;
		CString tmpFD0 ;
		CString tmpExtLenFD;
		float tmpExtLenValue=0 ;  //��һ��������������⣩���뵱ǰ���L8�����
		float tmpFD0value=0, tmpFD0valuePrevious=0 ;  //����ǰһ�����������ƥ���ֶ�ֵ
		float sngDim=0 , mvSumHeight =0, sngCSLen=0 , sngWeight =0, sngW=0 ;
		long iCntbNum=0 , iCnteNum =0, iNum =0, iNumPA=0; 
		long mvSA  =0; //���һ���������������������index�������ж����һ������Ƿ�Ϊ����
		long mvPA =0;  //��һ��������ܲ�����������index�������жϵ�һ������Ƿ�Ϊ�ܲ�
		long mviSASSClassID   =0;//�����͸ֵ�����ClassID
		long mviSASSIndex   =0;//�����������������Index
		CString mvsSASSMaterial ;  //�����͸ֵĲ���
		long mviPreviousClassID=0;   //ǰһ������������ClassID
		// mviSAClassID As Long  //����������ClassID
		bool bWithBoltsNuts;   //�����Դ���˨��ĸ
		int iSumSerialNum =0;  //����ǰ����ֹ���ܵĵ��ɴ�������Ϊ�����ɴ���Ƭ�����ۼ�ֵ��
		bool bSPRphs =0;  //��ǰ֧�����ǵ�(��)��֧����
		float sngH1xmax=0 ;  //����ߴ�H1ֵ��һ��H1
		
		//float Tmpyr=0 , TmpPgz=0 ,TmpPaz=0 , tmpCapacity=0;    //pfg2007.08.15ԭ���� ���ߺ����λ�ƣ����/���ɹ������أ����ɰ�װ���أ��غ�����
		float Tmpyr=0 ,  tmpCapacity=0;    //pfg2007.08.15���ߺ����λ�ƣ����/���ɹ������أ����ɰ�װ���أ��غ�����
		float Cmin =0, Cmax=0;   //��һ��/���һ�������غ�����
		float C1 =0, C2 =0 ;  //��ǰ/ǰһ������غ�����
		float tmpDist=0 ;   //��ʱ������λ��
		short tmpDistID=0 ;   //��ʱ������λ�ƴ���
		bool m_bFoundCSPR=false ;   //�ҵ����ʺ��
		
		CString mvL1CustomID ;  //��������L1��CustomIDֵ�����ڵ��ɻ���������ֱ��ѡ�񡣵��е���ʱ�ɼӿ������ٶȡ�
		//���ڸ�������������˨��ĸ��
		CString tmpCustomID ;
		float tmpSize2=0 , tmpSizeH=0 ;
		// iMODE As Long       //����ģʽ�������ڼӿ������ٶȡ�
		CString mvSAattachedCustomID ;  //����������CustomID
		
		//���������ʧ,��Ĭ��Ϊ��һ�����״̬��
		//����Ϊ�ڶ������״̬��
		if( FirstCal==0 ) FirstCal = 1;
		//iNum=(iCnteNum/iCntbNum)
		//ÿ������������iNumPart=��ǰһ������������iNumPart*iNum*iCntbNum
		//iNumPart=iNumPart*iNum*iCntbNum
		//iSA=0,iCSPR=7
		//mviSASSClassID=���������͸�����
		//mvSA=���һ�������������,�����б��Ƿ��Ǹ���,
		//��Ϊ����װ��δ���ʱ,���һ�������һ���Ǹ�������
		//mvPA=�ܲ�����,����ͳ�����˵��ܳ���
		//mvariSeqofSPR=��n-1����������(0��ʼ)
		//iSEQofPart=��n��֧���������������ĸ,1��ʼ��
		//mvSumHeight=��װ�ܸ߶�(mm)
		mvSumHeight = 0;
		iSEQofSPR = 0;
		iSEQofPart = 1;
		InitListRs();
		
		//Set mvarrsTmpREF1 = rsTmpREF.Clone
		COleVariant vTmp1,vTmp2,vTmp3,vTmp;
		COleVariant vBH;
		CString sTmp,sTmp2;
		CString strSQL;

		simplify(rsSAPart,nth);
		simplify2(rsPartBoltNuts,nth);

		strSQL.Format("SELECT * FROM %s WHERE zdjh=%d AND VolumeID=%d AND nth =%d ORDER BY recno",
			EDIBgbl::Btype[EDIBgbl::TZB],modPHScal::zdjh,EDIBgbl::SelVlmID,nth);
		rsTZB.m_pDatabase=&EDIBgbl::dbPRJDB;
		rsTZB.Open(dbOpenDynaset,strSQL);

		CDaoRecordset rsTmpZB(&EDIBgbl::dbPRJDB);//��ǰ֧���ܵ�ǰ·���㲿����������˨��ĸ����������
		strSQL.Format("SELECT CustomID FROM ZB WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND [ClassID]<>%d AND [ClassID]<>%d AND [ClassID]<>%d AND [IsSAPart]<>-1 Order By recno",
				EDIBgbl::SelVlmID,modPHScal::zdjh,nth,iBolts,iNuts,iAttached);
		rsTmpZB.Open(dbOpenDynaset,strSQL);
		int C;
 		if( rsTmpZB.IsEOF()&& rsTmpZB.IsBOF() )
			throw GetResStr(IDS_Null_rsTmpREF);
		else
		{
			rsTmpZB.MoveLast();
			rsTmpZB.MoveFirst();
			Ptype=new CString[rsTmpZB.GetRecordCount()];
			PtypeID=new CString[rsTmpZB.GetRecordCount()];
			PtypeIndex=new long[rsTmpZB.GetRecordCount()];
			PtypeClassID=new long[rsTmpZB.GetRecordCount()];
			C=rsTmpZB.GetRecordCount();
			CString strBH = sBHFormat;
			for( i = 0 ;i < C;i++)
			{
				rsTmpZB.GetFieldValue(_T("CustomID"),vTmp1);
				Ptype[i] = vtos(vTmp1);
				if( !rsID.FindFirst(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")))
				{
					//����
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(vTmp1));
					throw sTmp;
				}
				else
				{
					rsID.GetFieldValue(_T("Index"),vTmp1);
					PtypeIndex[i] = vtoi(vTmp1);
					rsID.GetFieldValue(_T("ClassID"),vTmp1);
					PtypeClassID[i] = vtoi(vTmp1);
					rsID.GetFieldValue(_T("ID"),vTmp1);
					PtypeID[i] = vtos(vTmp1);
					if(i>=1 && PtypeID[i]=="L3" && (PtypeID[i-1]=="D1" ||PtypeID[i-1]=="D1A" || PtypeID[i-1]=="D2" || PtypeID[i-1]=="D2A" || PtypeID[i-1]=="D3" || PtypeID[i-1]=="D3A" || PtypeID[i-1]=="D9" || PtypeID[i-1]=="D9A" || PtypeID[i-1]=="D10")) 
						m_bFoundL3=true;//�������L3���壬�ܼ���˨�ǲ���Ҫ��С��
				}
				rsTmpZB.MoveNext();
			}
			rsTmpZB.MoveFirst();
		}
		//����Ptype���鱣����mvarrsTmpZB�Ŀ���
		//�Ա�ȡ��֧������ϵ�����2���������,���в���
		
		rsTmpZB.MoveFirst();
		if( ! rsTmpZB.IsEOF() )
		{
			rsTmpZB.MoveLast();
			//չ��sFindTBN���ӿ��ٶȡ�
			rsTmpZB.GetFieldValue(_T("CustomID"),vTmp1);
			
			if( !rsID.FindFirst(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")))
			{
				//����
				sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(vTmp1));
				throw sTmp;
			}
			//�������һ������Ǹ�����������������롣
			rsID.GetFieldValue(_T("Index"),vTmp1);
			mvSA = vtoi(vTmp1);
			//�������һ������Ǹ�����������������롣����ͳ����˨��ĸʱ���жϵ�ǰ����Ǻ��ʱ��һ������Ƿ�Ϊ������
			PtypeIndex[rsTmpZB.GetRecordCount()- 1] = mvSA;
			//����ȷ�����õ�˫�۸�
			rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
			rsTmp.Open(dbOpenSnapshot,CString(_T("SELECT DISTINCT PNum1 From ")) + modPHScal::tbnSA + 
				_T(" WHERE CustomID=\'") + Ptype[C-1] + _T("\' AND PNum1 is not null"));
			if(rsTmp.GetRecordCount()==1)
			{
				//ֻ��һ����¼,����Pnum1�ֶ�=1��ֻ�ܲ��õ��۸֣�Pnum2=2��ֻ�ܲ���˫�۸֡�
				rsTmp.GetFieldValue(0,vTmp1);
				if(vtoi(vTmp1) ==1)
					strSelNumCS=_T(" AND PNUM1=1 ");
				if(vtoi(vTmp1) ==2)
					strSelNumCS=_T(" AND PNUM1=2 ");
			}
			else
			{
				switch(modPHScal::iNumCSVal)
				{
				case 1:
					strSelNumCS=_T(" AND PNum1=1 ");
					break;
				case 2:
					strSelNumCS=_T(" AND PNum1=2 ");
					break;
				case 3:
					strSelNumCS=_T(" AND  ");
					break;
				case 0:
					strSelNumCS=_T("");
					break;
				}
			}
			rsTmp.Close();
			if( rsTmpZB.GetRecordCount() >= 2 )
			{
				//����������ڻ����2���������ȡ��ͷβ�����
				rsTmpZB.MoveFirst();
				//չ��sFindTBN���ӿ��ٶȡ�
				rsTmpZB.GetFieldValue(_T("CustomID"),vTmp1);
				
				if( !rsID.FindFirst(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")))
				{
					//����
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(vTmp1));
					throw sTmp;
				}
				//�����һ������ǹܲ�������ܲ�����롣
				rsID.GetFieldValue(_T("Index"),vTmp1);
				mvPA = vtoi(vTmp1);
				
				//��һ������ǹܲ�,�������һ������Ǹ�����
				//˵��Ŀǰ���ڵ���֧������ʽ����ģʽ,������ѡ��֧����ģ��ģʽ��
				//��Ϊ���ߵ����һ�����������������mvSA<>iSA��
				//���ϲ�����ȫ��ȷ��
				
				
				//Ϊ�˼ӿ�ģ����Լ���ʱ���ٶȣ����Լ���ʱҪ�����˶γ���
				
				//Ԥ�Ȳ�����˵�CustomID����������㵯�ɻ���ʱѡ������ֱ��ʹ�á�
				//��Ϊ���˿����ڵ���֮����֣�����Ҫ�Ȳ���뵯�ɻ�����ϵ�����ֱ��modPHScal::giDiameter��
				//���ݵ�����е����������ITT���������ĺ��������ֱ�����ڹ��ڱ�׼��
				//Ϊ�˱�֤�淶��ͳһ��AutoPHS����tbnPART���ڵ�����ֱ����Ϊѡ�����ݡ�
				
				//�¶γ������Ѿ��ŵ�GetBoltsNutsAndAttachmentsCLgg��������ɡ�
				
				
				//������������ʼֵ=0
				mvarSATotalWeight = 0;
				//��ʼ����
				C=rsTmpZB.GetRecordCount();
				CString strBH = sBHFormat;
				for (i = 0 ;i<C;i++)
				{
					if(i==0) rsTZB.MoveFirst();
					frmStatus.UpdateStatus((float)(i+1)/C,true);
					frmStatus.UpdateWindow();
					
					//���������ڵ�ԭʼ���ݱ�tbn1,�Ա����
					//չ��sFindTBN���ӿ��ٶȡ�
					
					if(! rsID.FindFirst (_T("trim(CustomID)=\'") + Ptype[i] + _T("\'")) )
					{
						//û���ҵ�����
						sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),Ptype[i]);
						throw sTmp;
					}
					rsID.GetFieldValue(_T("ClassID"),vTmp1);
					modPHScal::glClassID =vtoi(vTmp1);
					rsID.GetFieldValue(_T("Index"),vTmp1);
					modPHScal::glIDIndex = vtoi(vTmp1);
					rsID.GetFieldValue(_T("ID"),vTmp1);
					sPartID = vtos(vTmp1);
					tmpID0 = sPartID;
					if( modPHScal::glIDIndex == iSA )
					{
						//��������ʾһ��ʱ��
						//��ǰ����Ǹ��������һ�����
						if( modPHScal::gbCalSAbyPJG )
							//������ṹ���ؼ������
							tmpSelPJG = tmpPJG;
						//����ǰһ����ļ������ѡ�����
						
					}
					else
					{
						//��ʾ���û�������
						//������Ǹ��������һ�����,��Ҫ�������ӵ���һ�����,�����ߴ��ֶκ�����
						//ǰ����ӵ����ID-->tmpCustomID0,tmpCustomID1
						tmpCustomID0 = Ptype[i];
						if( i <C - 1 )
						{
							if( rsID.FindFirst( _T("trim(CustomID)=\'") + Ptype[i + 1] + _T("\'")))
							{
								rsID.GetFieldValue(_T("ID"),vTmp1);
								tmpID1 = vtos(vTmp1);
							}
							//�������һ�����
							tmpCustomID1 = Ptype[i + 1];
							//��Connect���м������Ƿ���ڼ���ƥ��ĳߴ���Ϣ
							
							if( !rsConnect.FindFirst( _T("trim(cntb)=\'") + tmpID0 + _T("\' AND trim(cnte)=\'") + tmpID1 + _T("\'")))
							{
								//Connect���в������������.һ�㲻�ᷢ�������.
								sTmp.Format(GetResStr(IDS_PartCanNotMatchOrNotExistInConnect),tmpCustomID0,tmpCustomID1);
								throw sTmp;
							}
							//����ConnectCondition�ֶεĸ���SQL�����������ӳߴ��飬��������Ӧ�Ը��㡣�ٶ���һ�㡣
							//���﷨���ƣ�Cntb.Size2=Cnte.Size2 AND Cntb.size4<=Cnte.Size5
							rsConnect.GetFieldValue(_T("ConnectCondition"),vTmp1);
							if( vTmp1.vt==VT_NULL)
								tmpSQL0 = _T("");
							else
							{
								tmpSQL0 = vtos(vTmp1);
								tmpSQL0.MakeUpper();
								//�������L3���壬�ܼ���˨�ǲ���Ҫ��С��
								if(modPHScal::gbPipeClampBolt && !m_bFoundL3)
								{
									//���Ҫ��ܼ���˨���Ի�С����ô�ܼйܲ������ӳߴ����ȡ��
									if( modPHScal::glIDIndex ==iPA &&( tmpID0 == "D1" || tmpID0 == "D1A" ||tmpID0 == "D2" ||   \
										tmpID0 == "D2A" || tmpID0 == "D3" ||tmpID0 == "D3A" ||tmpID0 == "D9" ||tmpID0 == "D9A" ||tmpID0 == "D10" ))
									{
										modPHScal::g_bPipeClampBoltDiameterChanged=TRUE;
										//�ҳ��ܼ���˨M1�ĳߴ��ֶΣ�һ������������ַ���cntb.֮��=֮ǰ
										int i1=tmpSQL0.Find("<",5);
										if(i1==-1) 
										{
											i1=tmpSQL0.Find(">",5);
											if(i1==-1) 
											{
												i1=tmpSQL0.Find("=",5);
											}
											{
												//û�ҵ��κ��߼��������ţ�һ���������ݿ���˴���
											}
										}
										strDiaM1FieldName=tmpSQL0.Mid(5,i1-5);
										//�����ַ����ÿգ��򲻼��������˨�ߴ磬�����ܼ���˨����С
										tmpSQL0 = _T("");
									}
									else
									{
										
									}
									
								}
								else
								{
									
								}
								
							}
							
							rsConnect.GetFieldValue(_T("CnteXfx"),vTmp1);
							mlfx0 =vtoi(vTmp1);
							rsConnect.GetFieldValue(_T("CnteRotation"),vTmp1);
							mlRot0 = vtoi(vTmp1);
							rsConnect.GetFieldValue(_T("CntbNum"),vTmp1);
							iCntbNum=(vtoi(vTmp1)<=0 ? 1 : vtoi(vTmp1));
							rsConnect.GetFieldValue(_T("CnteNum"),vTmp1);
							iCnteNum=(vtoi(vTmp1)<=0 ? 1 : vtoi(vTmp1));
						}
						
						//�����һ�����
					}
					if( i == 0 )
					{
						//��һ������ض��ǹܲ�
						mlfx = modPHScal::GetPhsSAfx(_T("P") + modPHScal::df);
						modPHScal::glPAid = modPHScal::glClassID;
						//��ǰ�������������ȷ���Ƿ��ܹ����ܺ��ء�
						iNum = iCnteNum / iCntbNum;
						iNumPart = iCntbNum;
					}
					else if( i == (C - 1) )
					{
						//���һ�����
						//ֻ�и������ܳ��ֹ������,glNumSA=rsza(_T("Gnum"))
						iNumPart = modPHScal::glNumSA;
					}
					else
					{
						//�����м����Ӽ�
						//ÿ������������iNumPart=��ǰһ������������iNumPart*iNum*iCntbNum
						
						iNumPart = iNumPart * iNum * iCntbNum;
						iNum = iCnteNum / iCntbNum;
						if( iNumPart <= 0 ) iNumPart = modPHScal::gintParallelNum;
						//�ܲ����������Ĳ��ϣ���ϸ�µ������ǰ��ղ��Ϲ淶ѡ�����ͬ�������ֱ�ѡ�񣻴˴��򵥵�ȡĬ��ֵ��
						SQLx.Format(_T("SELECT * FROM SpecificationOfMaterial WHERE ClassIndex=%d AND ID=\'default\' AND tmin<=%g AND %g<tmax ORDER BY tmin,tmax,SEQ"),modPHScal::giClassIndex,modPHScal::gnConnectTJ,modPHScal::gnConnectTJ);
						if(rs3.IsOpen())
							rs3.Close();
						rs3.Open(dbOpenSnapshot,SQLx);
						if(rs3.IsEOF() && rs3.IsBOF())
						{
							//��%s��%s����ѡ��淶��û��%s�ֶ�ֵΪĬ��ֵ%s�ļ�¼(�ǹܲ��������ѡ�����)
							sTmp.Format(IDS_NoDefaultInCustomIDInSpecificationOfMaterial,rs3.m_pDatabase->GetName(),_T("SpecificationOfMaterial"),_T("ID"),_T("default"));
							throw sTmp;
						}
						else
						{
							rs3.GetFieldValue(_T("Material"),vTmp);
							m_strMaterial=vtos(vTmp);
						}
					}
					//��ʾ���û�������
					//������ֶα仯ֻ�ʺ��ڸ���,�Ǹ����ĺ����ֶ����ƾ�ΪSAfDPmax=_T("Pmax")
					if( modPHScal::glPAid == iPAh || modPHScal::glPAid == iPAD4LA || modPHScal::glPAid == iPAD4LQ || modPHScal::glPAid == iPAD11LD || modPHScal::glPAid == iPADLR  || modPHScal::glPAid == iPALX )
					{
						//����
						SAfDPmax = _T("PmaxH");
					}
					else if( modPHScal::glPAid == iPAs || modPHScal::glPAid == iPASS|| modPHScal::glPAid == iPAGS  || modPHScal::glPAid == iPAXZ1_XZ3 )
					{
						//����֧��
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
					if( tmpSQL != _T("") )
						tmpSQL = tmpSQL + _T(" AND ");
					
					//
					//�������Ҫ�������ӿס��˳ߴ��Զ����
					if( ! modPHScal::gbConnectHoleDimMatch ) 
						tmpSQL = _T("");
					else
					{
						//Ҫ��׳ߴ�ƥ��
						if( tmpSQL != _T("") )
						{
						}
						else 
							tmpSQL = _T("");
					}
					
					//��һ������ǹܲ�����������ƥ���ֶ���Ϣ��������Ϣ���������������ʹ�á�
					switch(modPHScal::glClassID)
					{
					case iPAXZ1_XZ3:
					case iPAh:						
						for (k = 0 ;k<= modPHScal::gnDW_delta;k++)
						{//���㲿�����ݱ�����һ���뵱ǰ֧���ܹܲ��⾶modPHScal::dj��ӽ����⾶
						 //�����⾶���ᳬ��modPHScal::gnDW_delta/100��
							sTmp.Format(_T("%g"),modPHScal::dj*(1-k*0.01));
							SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Dw>= ") + sTmp + _T(" AND Dw<= ") ;
							sTmp.Format(_T("%g"),modPHScal::dj * (1 + k * 0.01));
							SQLx+=sTmp  + _T(" AND Pmax>=");
							sTmp.Format(_T("%g"),tmpSelPJG / iNumPart);
							SQLx+=sTmp  + _T(" AND Tj>=");
							sTmp.Format(_T("%g"),tmpT0);
							SQLx+= sTmp + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY tj,Dw,Pmax,Weight");
							if(rsX.IsOpen())
								rsX.Close();
							rsX.Open(dbOpenSnapshot,SQLx);
							if( rsX.IsEOF() && rsX.IsBOF() )
								;
							else
								//û�ҵ���ÿ���⾶����1%��������
								k=modPHScal::gnDW_delta+1;		  
						}
						break;
					case iPAfixZ1://�ܼй̶�֧��Z1
					case iPAfixZ2://���ӹ̶�֧��Z2
						modPHScal::CalPAfixZ1Z2(Ptype[i], tmpSelPJG, tmpT0, iNumPart, rsX);
						goto spZ1Z2;
						break;
					case iPAs://��֧ͨ��
					case iPASS://����֧��
					case iPAGS://����֧��
						for( k = 0 ;k<= modPHScal::gnDW_delta;k++)
						{
							sTmp.Format(_T("%g"),modPHScal::dj * (1 - k * 0.01));
							SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Dw>= ") +sTmp+ _T(" AND Dw<= ") ;
							sTmp.Format(_T("%g"),modPHScal::dj * (1 + k * 0.01));
							SQLx+= sTmp + _T(" AND Pmax>=");
							sTmp.Format(_T("%g"),tmpSelPJG/iNumPart);
							SQLx+=sTmp + _T(" AND Tj>=");
							sTmp.Format(_T("%g"),tmpT0);
							SQLx+= sTmp + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY tj,Dw,Weight");
							if(rsX.IsOpen())
								rsX.Close();
							rsX.Open(dbOpenSnapshot,SQLx);
							if( rsX.IsEOF() && rsX.IsBOF() );
							else
								//û�ҵ���ÿ���⾶����1%��������
								k=modPHScal::gnDW_delta+1;
							
						}
						//2007.09.26(start)
						if(modPHScal::gbPAForceZero && (rsX.IsEOF() && rsX.IsBOF()))
						{
							for( k = 0 ;k<= modPHScal::gnDW_delta;k++)
							{
								sTmp.Format(_T("%g"),modPHScal::dj * (1 - k * 0.01));
								SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Dw>= ") +sTmp+ _T(" AND Dw<= ") ;
								sTmp.Format(_T("%g"),modPHScal::dj * (1 + k * 0.01));
								SQLx+= sTmp + _T(" AND Pmax IS NULL");
								SQLx+= _T(" AND Tj>=");
								sTmp.Format(_T("%g"),tmpT0);
								SQLx+= sTmp + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY tj,Dw,Weight");
								if(rsX.IsOpen())
									rsX.Close();
								rsX.Open(dbOpenSnapshot,SQLx);
								if( rsX.IsEOF() && rsX.IsBOF() )
								{
									;
								}
								else
								{
									k=modPHScal::gnDW_delta+1;
									AfxMessageBox("����:\n\n�ܲ��������û��,\n���򽫲�����ܲ�����!");
								}

							}
						}
						//2007.09.26(end)
						break;
					case iPAD4LA:
					case iPAD11LD:
					case iPADLR:
					case iPALX:
						//��Щ(�̷�ú�۹ܵ��ܲ�)��ܾ��޹�
						SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Pmax>=") ;
						sTmp.Format(_T("%g"),tmpSelPJG/iNumPart);
						SQLx+=sTmp+ _T(" AND Tj>=");
						sTmp.Format(_T("%g"),tmpT0);
						SQLx+=sTmp+_T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY tj,Pmax,Weight");
						if(rsX.IsOpen())
							rsX.Close();
						rsX.Open(dbOpenSnapshot,SQLx);
						break;
					case iROD:
						//���ܵ�֧��������p202 12.1.2(4)�롶�������糧��ˮ�ܵ���Ƽ����涨(DL/T 5054-1996)��p67 7.5.4��ָ����
						//����ֱ��DN<=50,d>=10mm;DN>=65,d>=12mm����Щ�涨�����е�֧���ܱ�׼ì�ܡ�
						//����������Ժ��ˮ�ܵ�֧�����ֲ� ���ӵ���D4.76S�׾�13mm,ֻ����L8.10(d=10mm)���ӡ������涨ì�ܡ�
						//��ˣ����ﲻ�ܰ��������涨������ֱ�����������ӳߴ��齫ʧ�ܡ�
						
						SQLx = _T("SELECT * FROM ") + modPHScal::tbnPART + _T(" WHERE ") + tmpSQL + _T(" Pmax>=");
						sTmp.Format(_T("%g"),(tmpSelPJG / iNumPart) * (vtob(rsza->GetCollect(_T("ifLongVertPipe"))) ? iNumPart : 1 ));
						SQLx+=sTmp ;
						if(modPHScal::gbAutoApplyCSPRLugDia 
							&& (FirstCal == 2) 
							&& modPHScal::gbPhsIsCSPR 
							&& (i<modPHScal::PtypeIndexForDiameter )
							&& (modPHScal::PtypeDiameter[i] > 0)
							)
							//������������ֱ��
							sTmp.Format(_T(" AND size2>=%d"),modPHScal::PtypeDiameter[i]);
						else if(!modPHScal::gbAutoSPRRodDiaDecrease 
							&& (FirstCal == 2) 
							&& modPHScal::gbPhsIsSPR 
							&& (i<modPHScal::PtypeIndexForDiameter )
							&& (modPHScal::PtypeDiameter[i] > 0)
							)
							//��������ֱ��
							sTmp.Format(_T(" AND size2>=%d"),modPHScal::PtypeDiameter[i]);
						else
							sTmp=_T("");
						SQLx+=sTmp;
						
						//AfxMessageBox("Rod diameter="+ltos(modPHScal::PtypeDiameter[i])); //HS->add 
						//modPHScal::PtypeDiameter[i] 
						
						sTmp=(modPHScal::gbLimitLugMinDia ? (modPHScal::dj >= 76 ? _T(" AND size2>=12") : (modPHScal::dj >= 57 ? _T(" AND size2>=10") : _T(" AND size2>=10"))) : _T(" "));
						SQLx+= sTmp + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY dh,tj,Weight");
						
						break; 
						
					case iConnected:
					case iXL1:
					case iAttached: 
						if( PtypeID[i] == _T("L5") || PtypeID[i] == _T("L6") || PtypeID[i] == _T("L7") || PtypeID[i] == _T("L8")  || PtypeID[i] == _T("L7Dd") || PtypeID[i] == _T("L8Dd") )
						{
							//����ֱ��
							if(PtypeClassID[i-1]==iROD)
							{
								//ǰһ��������			  
								modPHScal::PtypeDiameter[i]=modPHScal::PtypeDiameter[i-1];
								if(modPHScal::iMaxRodDia<modPHScal::PtypeDiameter[i])
								{
									
								}
								else
								{
									
								}
								
								//added by ywh 2003/07/19	
								if(modPHScal::gbRodDiaByMax)
								{
									if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[i])
									{
										modPHScal::PtypeDiameter[i]=modPHScal::iMaxRodDia;
										modPHScal::PtypeDiameter[i-1]=modPHScal::iMaxRodDia;
									}
									else
										modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[i];
								}
								
								
							}
							else
							{
								//ǰһ����������
								if(PtypeClassID[i+1]==iROD)
								{
									//��һ��������
									modPHScal::PtypeDiameter[i]=modPHScal::PtypeDiameter[i+1];
									
									if(modPHScal::gbRodDiaByMax)
									{
										if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[i])
										{
											modPHScal::PtypeDiameter[i]=modPHScal::iMaxRodDia;
											modPHScal::PtypeDiameter[i+1]=modPHScal::iMaxRodDia;
										}
										else
											modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[i];
									}
									
								}
								else
								{
									//��һ����������
									if(i+2<=C-1 && PtypeClassID[i+2]==iROD)
									{
										//��ڶ���������
										modPHScal::PtypeDiameter[i]=modPHScal::PtypeDiameter[i+2];
										
										if(modPHScal::gbRodDiaByMax)
										{
											if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[i])
											{
												modPHScal::PtypeDiameter[i]=modPHScal::iMaxRodDia;
												modPHScal::PtypeDiameter[i+2]=modPHScal::iMaxRodDia;
											}
											else
												modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[i];
										}
										
									}
									else
									{
										//��ڶ�����������
										if(i>=2 && PtypeClassID[i-2]==iROD)
										{
											//ǰ�ڶ���������
											modPHScal::PtypeDiameter[i]=modPHScal::PtypeDiameter[i-2];
											
											if(modPHScal::gbRodDiaByMax)
											{
												if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[i])
												{
													modPHScal::PtypeDiameter[i]=modPHScal::iMaxRodDia;
													modPHScal::PtypeDiameter[i-2]=modPHScal::iMaxRodDia;
												}
												else
													modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[i];
											}
											
										}
										else
										{
											//ǰ�ڶ�����������
										}
									}
								}
							}
							
							SQLx = _T("SELECT * FROM ") + modPHScal::tbnPART + _T(" WHERE ") + tmpSQL + _T(" Pmax>=");
							sTmp.Format(_T("%g"),tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe")))? iNumPart : 1));
							SQLx+=sTmp;
							if(modPHScal::gbAutoApplyCSPRLugDia 
								&& (FirstCal == 2) 
								&& modPHScal::gbPhsIsCSPR 
								&& (i<modPHScal::PtypeIndexForDiameter )
								&& (modPHScal::PtypeDiameter[i] > 0)
								)
								//������������ֱ��
								sTmp.Format(_T(" AND size2>=%d"),modPHScal::PtypeDiameter[i]);
							else if(!modPHScal::gbAutoSPRRodDiaDecrease 
								&& (FirstCal == 2) 
								&& modPHScal::gbPhsIsSPR 
								&& (i<modPHScal::PtypeIndexForDiameter )			  
								&& (modPHScal::PtypeDiameter[i] > 0)
								)
							{
								sTmp.Format(_T(" AND size2>=%d"),modPHScal::PtypeDiameter[i]);
							}
							else
							{
								sTmp=_T("");
								if(modPHScal::gbRodDiaByMax)//�����������һ�£���ô�޸�ѡ������
								{
									CString str;
									str.Format ("AND size2>=%d",modPHScal::iMaxRodDia);
									sTmp = str;	
								}		  
							}
							SQLx+=sTmp;
							sTmp=(modPHScal::gbLimitLugMinDia ? (modPHScal::dj >= 76 ? _T(" AND size2>=12") : (modPHScal::dj >= 57 ? _T(" AND size2>=10") : _T(""))): _T(""));
							
							SQLx+=sTmp  + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY dh,tj,Weight");
							
						}
						else if(PtypeID[i]==_T("L15"))
						{	//���Ӽ�L15ʵ������һ��ְ�
							//��֤�ְ��С
							if(modPHScal::gfPSWidth<=(modPHScal::dj+20))
								modPHScal::gfPSWidth=int((modPHScal::dj+20)/10)*10;
							if(modPHScal::gfPSLength<=(modPHScal::dj+20))
								modPHScal::gfPSLength=int((modPHScal::dj+20)/10)*10;
							//�ְ���
							sTmp.Format(_T("%sx%sx%s"),ftos(modPHScal::gfPSThickness),ftos(modPHScal::gfPSWidth),ftos(modPHScal::gfPSLength));
							//�ְ�����kg
							float fPSWeight=modPHScal::gfPSThickness * modPHScal::gfPSWidth * modPHScal::gfPSLength * 7850e-9;
							SQLx = _T("SELECT * FROM ") + modPHScal::tbnAttachment + _T(" WHERE CustomID=\'PS\' AND BH=\'") + sTmp + _T("\' ORDER BY BH,Weight");
							if(rsTmp.IsOpen())
								rsTmp.Close();
							rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
							rsTmp.Open(dbOpenDynaset,SQLx);
							if(rsTmp.IsBOF() && rsTmp.IsEOF())
							{
								//������������ְ�,��������
								/*rsTmp.AddNew();
								rsTmp.SetFieldValue(_T("CustomID"),COleVariant(_T("PS")));
								rsTmp.SetFieldValue(_T("BH"),COleVariant(sTmp));
								rsTmp.SetFieldValue(_T("size2"),COleVariant(modPHScal::gfPSThickness));
								rsTmp.SetFieldValue(_T("Weight"),COleVariant(fPSWeight));
								rsTmp.Update();*/
								modPHScal::dbZDJcrude.Execute(_T("INSERT INTO [")+ modPHScal::tbnAttachment + _T("] (CustomID,BH,size2,Weight) VALUES (\'PS\',\'") + sTmp + _T("\',") + ftos(modPHScal::gfPSThickness) + _T(",") + ftos(fPSWeight) + _T(")"));
							}		  
						}
						else if(PtypeID[i]==_T("L3"))
						{
							//�¿׾�size3���������У���������ѡ��С�׵�˫�׵���
							SQLx = _T("SELECT * FROM ") + modPHScal::tbnPART + _T(" WHERE ") + tmpSQL + _T(" Pmax>=");
							sTmp.Format(_T("%g"),tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe")))? iNumPart : 1));
							SQLx+=sTmp + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY tj,size3,size2,Weight");
						}
						else
						{
							SQLx = _T("SELECT * FROM ") + modPHScal::tbnPART + _T(" WHERE ") + tmpSQL + _T(" Pmax>=");
							sTmp.Format(_T("%g"),tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe")))? iNumPart : 1));
							SQLx+=sTmp + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY dh,tj,Weight");
						}
			   break;
	case iSPR:
		//���ɵĵ���ֱ�����Զ��Ӵ�ʱ����Ҫȡ��ֱ���׼��
		//ȡ���൯�ɵ����һ�����
		//SQLx = _T("SELECT TOP 1 * FROM [") + tbnSPRINGCrude + _T("] WHERE ") + IIf(tmpSQL <> _T(""), IIf(gbSPRAutoLugDia, _T(""), tmpSQL), _T("")) + _T(" (dh mod 100)=") + modPHScal::sSprInfo[iSEQofSPR].SprNo + _T(" AND int(dh/100)<=") + (giSumSerialNum - iSumSerialNum) + _T(" AND trim(CustomID)=//") + Ptype[i] + _T("// ORDER BY dh DESC,Weight")
		SQLx = _T("SELECT * FROM [") + modPHScal::tbnSPRINGCrude + _T("] WHERE ") + (tmpSQL != _T("") ? (modPHScal::gbSPRAutoLugDia ? _T("") : tmpSQL) : _T(""));
		if(iSEQofSPR < modPHScal::SprInfoIndex)
			sTmp.Format(_T(" dh=%d "),modPHScal::sSprInfo[iSEQofSPR].DH);
		else
			sTmp=_T(" ");
		SQLx+=sTmp + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY dh,Weight");
		if ( sPartID == _T("T5") && FirstCal >=2 )
		{
			if(!modPHScal::gbAddPartWeight2SPRPgz) //pfg2007.08.21
			{
				modPHScal::g_fLHGWeight=0;	
			}
			TmpPgz = (fabs(tmpSelPJG4SPRPgz)+modPHScal::g_fLHGWeight) / modPHScal::gintParallelNum;
			TmpPaz = (fabs(tmpSelPJG4SPRPaz)+modPHScal::g_fLHGWeight) / modPHScal::gintParallelNum;
		}
		else
		{
			TmpPgz = fabs(tmpSelPJG4SPRPgz) / modPHScal::gintParallelNum;
			TmpPaz = fabs(tmpSelPJG4SPRPaz) / modPHScal::gintParallelNum;
		}
		break;
	case iCSPR:
		if(iSEQofSPR < modPHScal::giWholeSprNum)
			modPHScal::giCSPRindex[iSEQofSPR] = i;
		//��λ������Ʊ�־
		EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET SpecialDesign=\'\'"));
		if ( sPartID == _T("LHG") && FirstCal >=2 )
		{
			TmpPgz = (fabs(tmpSelPJG4CSPRPgz)+modPHScal::g_fLHGWeight) / modPHScal::gintParallelNum;
			TmpPaz= TmpPgz;
		}
		else
		{
			TmpPgz = fabs(tmpSelPJG4CSPRPgz) / modPHScal::gintParallelNum;
			TmpPaz= TmpPgz;
		}
		Tmpyr = fabs(modPHScal::yr);         //yr ��λ:mm
		if( fabs(Tmpyr) * modPHScal::gUseConstantSpringPercentSurplusDist < modPHScal::gUseConstantSpringMinSurplusDist )
		{
			Tmpyr = fabs(Tmpyr) + modPHScal::gUseConstantSpringMinSurplusDist;
		}
		else
		{
			Tmpyr = fabs(Tmpyr) * (1 + modPHScal::gUseConstantSpringPercentSurplusDist);
		}
		SQLx = _T("SELECT * FROM [") + modPHScal::tbnHDproperty + _T("] ORDER BY Capacity");
		if(rs.IsOpen())
			rs.Close();
		rs.Open(dbOpenSnapshot,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{
			//����غ�������Ϊ�գ�������
			sTmp.Format(GetResStr(IDS_NullTableInXMdb),modPHScal::dbZDJcrude.GetName(), modPHScal::tbnHDproperty);
			throw sTmp;
		}
		else
		{
			rs.GetFieldValue(_T("Capacity"),vTmp1);
			Cmin = vtof(vTmp1);
			rs.MoveLast();
			rs.GetFieldValue(_T("Capacity"),vTmp1);
			Cmax = vtof(vTmp1);
			rs.MoveFirst();
			//��λ���ҵ����ʺ����־
			m_bFoundCSPR = false;
			//��ʼ��ʱ������iSumSerialNum=0
			iSumSerialNum = iSumSerialNum + 1;
			tmpDist = Tmpyr / modPHScal::giWholeSprNum;
			//��λ��ϵ��ֵ�����ͽ��Ͽ�ԭ��ȷ��λ��ϵ�С�
			if(rs1.IsOpen())
				rs1.Close();
			sTmp.Format(_T("%g"),tmpDist);
			rs1.Open(dbOpenSnapshot,_T("SELECT * FROM [") + modPHScal::tbnDisplacementSerial + _T("] WHERE DisplacementSerial>=") + sTmp + _T(" ORDER BY DisplacementSerial"));
			
			if( rs1.IsEOF() && rs1.IsBOF() )
				//λ�Ƴ���ϵ�����ֵ
				;
			else
			{
				rs1.GetFieldValue(_T("DisplacementSerial"),vTmp1);
				tmpDist =vtof(vTmp1);
				//by ligb on 2008.05.09 
				try
				{
					rs1.GetFieldValue(_T("MovementID"),vTmp1);//����ֶ����¼ӵģ�ֻ��2007�潭����Դ�������ɲ�������ֶ�ֵ�������������ܳ�����Ҫ�������
					tmpDistID =vtoi(vTmp1);
				}
				catch(...)
				{
					tmpDistID=0;//�����������ɲ���Ҫ���λ�ƴ���ֵ������Ϊ0���ɡ�
				}
				
				//�غ�������λΪkN*mm,��N*m,λ�Ƶ�λΪmm,���ص�λΪkgf(��daN=10N)
				tmpCapacity = TmpPgz * tmpDist * EDIBgbl::kgf2N / 1000;
				if( tmpCapacity < Cmin )
				{
					//������������С�Ļ�С
					sTmp.Format(GetResStr(IDS_CapacityLitterCmin),_T("\%g"),_T("\%g"));
					sTmp2.Format(sTmp,tmpCapacity, Cmin);
					throw sTmp2;
				}
				else
				{
					if( tmpCapacity > Cmax )
					{
						//�������������Ļ���
						//��������1������ѭ��
					}
					else
					{
						//������������
						sTmp.Format(_T("%g"),tmpCapacity);
						
						if(!rs.FindFirst(_T("Capacity>=") + sTmp) );
						//û�ҵ���������
						
						else
						{
							//�ҵ���
							rs.GetFieldValue(_T("minDist"),vTmp1);
							if( tmpDist < vtof(vTmp1) )
							{
								//����Сλ��С��Ӧ���õ���
								sTmp.Format(GetResStr(IDS_ShouldUseSpring),_T("\%g"),_T("\%g"));
								sTmp2.Format(sTmp,tmpDist,vtof(vTmp1));
								throw sTmp2;
								
							}
							else
							{
								rs.GetFieldValue(_T("maxDist"),vTmp1);
								if( tmpDist > vtof(vTmp1) )
								{
									//�����λ�ƻ���
									if( iSumSerialNum >= modPHScal::giWholeSprNum )
									{
										//����ģ���еĴ�����
										//if( gbCSPRCanSpecialDesign ){
										//�����Ҫ������ƣ��������������ʹ�ã�,
										//ֻҪһ������Ϳ�����
										//��������λ����Ϊ��������
										modPHScal::gbCSPRneedSpecialDesign = true;
										EDIBgbl::dbPRJ.Execute(GetResStr(IDS_ESPECIAL_DESIGN_SQL));
										m_bFoundCSPR = true;
										//}else{
										//�������Ҫ������ƣ������������ʹ�ã�
										//����ѭ��
										//}
									}
									//	 else{
									//С��ģ���еĴ�����
									//����ѭ��
									//}
								}
								else
								{
									//����
									//��λ�ƿ�����Ϊ��������
									modPHScal::gbCSPRneedSpecialDesign = false;
									EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET SpecialDesign=\'\'"));
									m_bFoundCSPR = true;
								}
							}
						}
					}
				}
			}
			if( ! m_bFoundCSPR )
				//�������ﵽ���ֵ����û�к��ʵĺ�������
				throw GetResStr(IDS_NotFoundConstantSPRInTBNHDproperty);
			
			else
			{
				//�ҵ����ʵĺ��
				//���غ������ͽ�ԭ��ѡ����
				//��¼��ǰ�غ�����
				rs.GetFieldValue(_T("Capacity"),vTmp1);
				C1 = vtof(vTmp1);
				//��¼ǰһ���غ�����
				rs.MovePrev();
				rs.GetFieldValue(_T("Capacity"),vTmp1);
				C2 = vtof(vTmp1);
				if( fabs(C1 - tmpCapacity) >= fabs(C2 - tmpCapacity) );
				//�غ��������ӽ�C2
				else
				{
					//�غ��������ӽ�C1
					//�ص������ӽ��ļ�¼
					rs.MoveNext();
				}
				//�������ֵ
				if(iSEQofSPR<modPHScal::SprInfoIndex)
				{
					modPHScal::sSprInfo[iSEQofSPR].SerialNum = 1;
					rs.GetFieldValue(_T("dh"),vTmp1);
					modPHScal::sSprInfo[iSEQofSPR].DH =vtoi(vTmp1);
					modPHScal::sSprInfo[iSEQofSPR].haz = fabs(modPHScal::yr) / modPHScal::giWholeSprNum * modPHScal::sSprInfo[iSEQofSPR].SerialNum;
					modPHScal::sSprInfo[iSEQofSPR].HeatDisp = modPHScal::sSprInfo[iSEQofSPR].haz;
					modPHScal::sSprInfo[iSEQofSPR].CheckDisp = (modPHScal::gbMinMaxDispByActualDisp?modPHScal::sSprInfo[iSEQofSPR].HeatDisp:tmpDist);
					modPHScal::sSprInfo[iSEQofSPR].SumDisp = tmpDist;
					modPHScal::sSprInfo[iSEQofSPR].SumDistID = tmpDistID;
				}
				//ѡ��Ĺ������أ���λΪkgf
				rs.GetFieldValue(_T("Capacity"),vTmp1);
				tmpSelOpLoad = vtof(vTmp1)/ tmpDist * 1000 / EDIBgbl::kgf2N;
			}
	   }
	   
	   SQLx = _T("SELECT * FROM [") + modPHScal::tbnHDCrude + _T("] WHERE ") + tmpSQL ;
	   if(iSEQofSPR < modPHScal::SprInfoIndex)
		   sTmp.Format(_T(" dh=%d"),modPHScal::sSprInfo[iSEQofSPR].DH);
	   else sTmp = _T(" ");
	   SQLx+=sTmp;
	   if(iSEQofSPR < modPHScal::SprInfoIndex)
	   {
		   sTmp.Format(_T(" AND fBmin<=%g AND fBmax>=%g"),modPHScal::sSprInfo[iSEQofSPR].CheckDisp,modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
	   }
	   else
	   {
		   sTmp=_T(" ");
	   }
	   SQLx+=(modPHScal::gbCSPRneedSpecialDesign ? _T(" ") : sTmp) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY dh,Weight");
	   break;
	case iSAh:
		SQLx.Format(_T("SELECT * FROM  %s  WHERE  + %s %s >=  %g  AND trim(CustomID)=\'%s\' ORDER BY GDW1,%s"),
			modPHScal::tbnSA,
			tmpSQL,
			SAfDPmax,
			tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe"))) ? iNumPart : 1),
			Ptype[i],
			SAfDPmax);
		break;
	case iSACantilever:
	case iSALbrace:
		SQLx.Format(_T("SELECT * FROM  %s  WHERE GDW1>= %g AND %s >=%g AND trim(CustomID)=\'%s\' %s ORDER BY GDW1,%s"),
			modPHScal::tbnSA,
			modPHScal::gdw,
			SAfDPmax ,
			tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe"))) ? iNumPart : 1),
			Ptype[i],
			strSelNumCS,
			SAfDPmax);
		break;
	case iG51:
	case iG56:
	case iG57:
		SQLx.Format(_T("SELECT * FROM %s WHERE GDW1>= %g AND size2>=%g AND %s >=%g AND trim(CustomID)=\'%s\' %s ORDER BY GDW1,size2,%s"),
			modPHScal::tbnSA,
			modPHScal::gdw,
			modPHScal::gdw/modPHScal::WidthB,
			SAfDPmax,
			tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe"))) ? iNumPart : 1),
			Ptype[i],
			strSelNumCS,
			SAfDPmax);
		break;
	case iSAbeam:
	case iG52_55:
		//2006.06.09 by pfg,ligb
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
		break;
	case iSALbraceFixG47:
	case iSALbraceFixG48:               
		if((Ptype[i]!=_T("SJ8")))
		{
			//��ʽ�б����ĵ�λ:mvBWidth,cm;PAfixH,mm->cm; modPHScal::gdw,mm->cm; ��������,kgf.m->kgf.cm; ������,kgf->kgf.
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
			//��ʼѭ�����Һ��ʵĹ̶�֧��G47��G48
			//��۸����Ա���F1ToWx1
			SQLx = _T("SELECT * FROM SSteelPropertyCS WHERE F1toWx1 is not null ORDER BY ID");
			if(rsTmp.IsOpen())
				rsTmp.Close();
			rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;	
			rsTmp.Open(dbOpenSnapshot,SQLx);
			//���������������Ժ֧�����ֲ�1983���117ҳ����,��"GDW1>="��Ϊ"GDW1=". lgb and pfg20050927
			SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1= ") + ftos(int((modPHScal::gdw+99)/100)*100) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
			if(rsX.IsOpen())
				rsX.Close();
			rsX.Open(dbOpenSnapshot,SQLx);
			if( rsX.IsEOF() && rsX.IsBOF() )
				;
			else
			{
				while (!rsX.IsEOF())
				{
					rsX.GetFieldValue(_T("P1"),vTmp1);
					rsX.GetFieldValue(_T("PmaxSF"),vTmp3);
					sngSAfixP=vtof(vTmp3);
					if(rsTmp.FindFirst(_T("BH=\'")+vtos(vTmp1)+_T("\'")))
					{
						rsTmp.GetFieldValue(_T("F1ToWx1"),vTmp2);
						F1ToWx1=vtof(vTmp2);
						sngSAfixPl = 1.3 * modPHScal::pylc / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * modPHScal::pxlc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * modPHScal::pzlc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 1 / 4 * F1ToWx1) * modPHScal::mxlc * 100 + modPHScal::mylc * 100 / mvBWidth + 2.6 / mvBWidth * modPHScal::mzlc * 100;
						sngSAfixPr = 1.3 * modPHScal::pyrc / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * modPHScal::pxrc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * modPHScal::pzrc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 1 / 4 * F1ToWx1) * modPHScal::mxrc * 100 + modPHScal::myrc * 100 / mvBWidth + 2.6 / mvBWidth * modPHScal::mzrc * 100;
						//��������Ժ֧�����ֲ�1983���116ҳ���ͣ������ء����ķ�������⡣
						if(1)
						{
							
							sngSAfixPl = 1.3 * fabs(modPHScal::pylc) / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * fabs(modPHScal::pxlc) + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * fabs(modPHScal::pzlc) + (1.95/ (modPHScal::gdw/10) + 1 / 4 * F1ToWx1) * fabs(modPHScal::mxlc) * 100 + fabs(modPHScal::mylc) * 100 / mvBWidth + 2.6 / mvBWidth * fabs(modPHScal::mzlc) * 100;
							sngSAfixPr = 1.3 * fabs(modPHScal::pyrc) / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * fabs(modPHScal::pxrc) + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * fabs(modPHScal::pzrc) + (1.95/ (modPHScal::gdw/10) + 1 / 4 * F1ToWx1) * fabs(modPHScal::mxrc) * 100 + fabs(modPHScal::myrc) * 100 / mvBWidth + 2.6 / mvBWidth * fabs(modPHScal::mzrc) * 100;							
						}
						//���������<0,ȡ�����ֵ��Ϊ������ by ligb on 2004.09.11
						if(sngSAfixPl<0) sngSAfixPl=fabs(sngSAfixPl);
						if(sngSAfixPr<0) sngSAfixPr=fabs(sngSAfixPr);

						if( (sngSAfixP > sngSAfixPl) && (sngSAfixP > sngSAfixPr) )
						{
							//��̬����̬��������������
							SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + ftos(modPHScal::gdw) + _T(" AND PmaxSF>=")
								+ ftos(sngSAfixP)+ _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
							break;
						}
						else
						{
							//������
							rsX.MoveNext();
						}
					}
				}
				if(rsX.IsEOF())
				{
					//���������������Ժ֧�����ֲ�1983���117ҳ����,��"GDW1>="��Ϊ"GDW1=".lgb and pfg20050927
					SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1= ") + ftos(int((modPHScal::gdw+99)/100)*100) + _T(" AND PmaxSF>=")
						+ ftos((sngSAfixPl>sngSAfixPr)?sngSAfixPl:sngSAfixPr)+ _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
				}
			}
		}
		else
		{
			//��ʼǿ�ȼ���:����Ժ�̶�֧�ܸ���˫�����Ǽ�SJ8(ClassID=iSALbraceFixG47=4)
			
			//�ܲ����������Ĳ��ϣ���ϸ�µ������ǰ��ղ��Ϲ淶ѡ�����ͬ�������ֱ�ѡ�񣻴˴��򵥵�ȡĬ��ֵ��
			SQLx.Format(_T("SELECT * FROM SpecificationOfMaterial WHERE ClassIndex=%d AND ID=\'default\' AND tmin<=%g AND %g<tmax ORDER BY tmin,tmax,SEQ"),modPHScal::giClassIndex,modPHScal::gnConnectTJ,modPHScal::gnConnectTJ);
			if(rs3.IsOpen())
				rs3.Close();
			rs3.Open(dbOpenSnapshot,SQLx);
			if(rs3.IsEOF() && rs3.IsBOF())
			{
				//��%s��%s����ѡ��淶��û��%s�ֶ�ֵΪĬ��ֵ%s�ļ�¼(�ǹܲ��������ѡ�����)
				sTmp.Format(IDS_NoDefaultInCustomIDInSpecificationOfMaterial,rs3.m_pDatabase->GetName(),_T("SpecificationOfMaterial"),_T("ID"),_T("default"));
				throw sTmp;
			}
			else
			{
				rs3.GetFieldValue(_T("Material"),vTmp);
				m_strMaterial=vtos(vTmp);
			}
			
			//����Ӧ��
			float Sigmat=0.0,Sigmat100=0.0,Sigma0=0.0;
			modPHScal::StressOfMaterial(m_strMaterial,modPHScal::t0,Sigmat);
			modPHScal::StressOfMaterial(m_strMaterial,100,Sigmat100);
			modPHScal::StressOfMaterial(m_strMaterial,20,Sigma0);
			//�򿪹̶�֧�ܼ��㹫ʽ��������
			SQLx = _T("SELECT * FROM CalFormulaOfFixPASA WHERE CustomID=\'") + Ptype[i] + _T("\' ORDER BY CustomID,SEQ");
			if(rsCal.IsOpen())
				rsCal.Close();
			rsCal.m_pDatabase=&EDIBgbl::dbSACal;	//20071103 "dbSORT" ��Ϊ "dbSACal"
			rsCal.Open(dbOpenSnapshot,SQLx);
			if(rsCal.IsEOF() && rsCal.IsBOF())
			{
				//�����ݿ�%s�еĸ���ǿ�ȼ��㹫ʽ��%sû���κμ�¼��
				sTmp.Format(IDS_NullTableCalFormulaOfFixPASA,EDIBgbl::dbSACal.GetName(),_T("CalFormulaOfFixPASA"));//20071103 "dbSORT" ��Ϊ "dbSACal"
				throw sTmp;
			}
			//�򿪸���ǿ�ȼ��㹫ʽ��������
			SQLx = _T("SELECT * FROM tmpCalFixPhs");
			if(rsTmp1.IsOpen())
				rsTmp1.Close();
			rsTmp1.m_pDatabase=&EDIBgbl::dbPRJ;	
			//rsTmp1��Ҫ�޸Ĺ���
			rsTmp1.Open(dbOpenDynaset,SQLx);
			//�򿪲۸����Ա�����
			SQLx = _T("SELECT * FROM SSteelPropertyCS ORDER BY ID");
			if(rsTmp.IsOpen())
				rsTmp.Close();
			rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;	
			rsTmp.Open(dbOpenSnapshot,SQLx);
			//�򿪽Ǹ����Ա�����
			SQLx = _T("SELECT * FROM SSteelPropertyLS ORDER BY ID");
			if(rs1.IsOpen())
				rs1.Close();
			rs1.m_pDatabase=&modPHScal::dbZDJcrude;	
			rs1.Open(dbOpenSnapshot,SQLx);
			//ȡ�������е����㶨λ���������м�¼,��PmaxSF�����������
			SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + ftos(modPHScal::gdw) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
			if(rsX.IsOpen())
				rsX.Close();
			rsX.Open(dbOpenSnapshot,SQLx);
			if( rsX.IsEOF() && rsX.IsBOF())
				;
			else
			{				
				CString strP1,strP2,strP3;
				int n,dh;
				float bb,PL3,K;
				int i4,i5;
				float mfpx,mfpy,mfpz;//���������̬����kgf
				float mfmx,mfmy,mfmz;//���������̬���أ�kgf.m
				CString F1sa,Wyh1sa,Wzh1sa;
				CString F2sa,Wyh2sa,Wzh2sa;
				CString SIGMA1sa,SIGMA1saMax,TAO1sa,TAO1saMax,TAOx1sa,TAOy1sa,TAOz1sa,lamda1;
				CString SIGMA2sa,SIGMA2saMax,TAO2sa,TAO2saMax,TAOx2sa,TAOy2sa,TAOz2sa,lamda2;
				float mfSIGMA1sa,mfSIGMA1saMax,mfTAO1sa,mfTAO1saMax,mfLamda1,mfFi1;
				float mfSIGMA2sa,mfSIGMA2saMax,mfTAO2sa,mfTAO2saMax,mfLamda2,mfFi2;
				CDaoRecordset rs2;
				
				//modPHScal::PAfixH=320.0;//����ɲ��Ի���Ժ����
				
				rsTmp1.Edit();
				//rsTmp1.SetFieldValue(_T("CustomIDsa"),COleVariant(Ptype(i)));
				rsTmp1.SetFieldValue(_T("CustomIDsa"),COleVariant(sPartID));
				rsTmp1.SetFieldValue(_T("H"),COleVariant(modPHScal::PAfixH));
				rsTmp1.SetFieldValue(_T("gdw2"),COleVariant(modPHScal::gdw));
				rsTmp1.SetFieldValue(_T("tj"),COleVariant(modPHScal::t0));
				//����Ӧ����λkgf/cm2->kgf/mm2
				rsTmp1.SetFieldValue(_T("SIGMAjt"),COleVariant(Sigmat/100.0));
				rsTmp1.SetFieldValue(_T("SIGMAj20"),COleVariant(Sigma0/100.0));
				rsTmp1.SetFieldValue(_T("SIGMAj100"),COleVariant(Sigmat100/100.0));
				rsTmp1.Update();
				//����һ��ӿ���£�������ֹ������
				
				//��ù���3
				rsX.GetFieldValue(_T("P3"),vTmp);
				strP3=vtos(vTmp);	
				rsTmp1.Edit();
				rsTmp1.SetFieldValue(_T("P3"),vTmp);
				//�˴�ֻ��Ӧ�й���׼�����ʻ�ʱҪ�޸Ĵ���
				n=strP3.Find((_T("x")));
				//����3�ȿ�bb,unit:mm
				bb=_tcstod(strP3.Mid(1,n-1),NULL);
				rsTmp1.SetFieldValue(_T("bb"),COleVariant(bb));
				//������͸�P1
				rsX.GetFieldValue(_T("P1"),vTmp);
				strP1=vtos(vTmp);
				rsTmp1.SetFieldValue(_T("P1"),vTmp);
				//��ù���2�͸�P2
				rsX.GetFieldValue(_T("P2"),vTmp);
				strP2=vtos(vTmp);
				rsTmp1.SetFieldValue(_T("P2"),vTmp);
				//˫�۸���Ե������PL3,mm
				rsX.GetFieldValue(_T("PL3"),vTmp);
				PL3=vtof(vTmp);
				//rsX�е�PL3��CHAR(20),��rsTmp1�е�PL3��REAL,�ɼ������ܹ��Զ�ת����
				rsTmp1.SetFieldValue(_T("PL3"),vTmp);
				//�۸ֺ���߶�K,mm
				rsX.GetFieldValue(_T("size2"),vTmp);
				K=vtof(vTmp);
				rsTmp1.SetFieldValue(_T("K"),vTmp);
				rsTmp1.Update();
				//���DH,mm
				rsX.GetFieldValue(_T("dh"),vTmp);
				dh=vtoi(vTmp);				
				
				//�ȼ�����̬
				
				bool bEOF=false;
				bool bErr=false;//����һ�����������㣬��Ϊtrue
				do
				{
					if(!rsTmp.FindFirst(_T("BH=\'")+strP1+_T("\'")))
					{
						//�ڲ۸����Ա�%s��û�ҵ��۸��ͺ�%s
						sTmp.Format(GetResStr(IDS_NotFoundThisCSNo),_T("SSteelPropertyCS"),strP1);
						throw sTmp;
					}
					//����1�۸ֹ��Ծ�Ix,cm4->mm4
					rsTmp.GetFieldValue(_T("Ix"),vTmp);
					float J1y=vtof(vTmp)*10000.0;
					rsTmp1.Edit();
					rsTmp1.SetFieldValue(_T("J1y"),COleVariant(J1y));
					//����1�۸ֹ��Ծ�Iy,cm4->mm4
					rsTmp.GetFieldValue(_T("Iy"),vTmp);
					float J1z=vtof(vTmp)*10000.0;
					rsTmp1.SetFieldValue(_T("J1z"),COleVariant(J1z));
					//����1�۸���С���Ծ�,cm4->mm4
					rsTmp1.SetFieldValue(_T("Imin1"),COleVariant(J1z>J1y?J1y:J1z));
					//����1�۸ֿ����Wx,cm3->mm3
					rsTmp.GetFieldValue(_T("Wx"),vTmp);
					float W1y=vtof(vTmp)*1000.0;
					rsTmp1.SetFieldValue(_T("W1y"),COleVariant(W1y));
					//����1�۸ֿ����Wy,cm3->mm3
					rsTmp.GetFieldValue(_T("Wy"),vTmp);
					float W1z=vtof(vTmp)*1000.0;
					rsTmp1.SetFieldValue(_T("W1z"),COleVariant(W1z));
					//����1�۸ֽ����S,cm2->mm2
					rsTmp.GetFieldValue(_T("S"),vTmp);
					float F1=vtof(vTmp)*100.0;
					rsTmp1.SetFieldValue(_T("F1"),COleVariant(F1));
					//����1�۸ָ߶�h,mm
					rsTmp.GetFieldValue(_T("h"),vTmp);
					float h1=vtof(vTmp);
					rsTmp1.SetFieldValue(_T("h1"),vTmp);
					//����1�۸��ȿ�b,mm
					rsTmp.GetFieldValue(_T("b"),vTmp);
					float b1=vtof(vTmp);
					rsTmp1.SetFieldValue(_T("b1"),vTmp);
					rsTmp1.Update();
					
					if(!rsTmp.FindFirst(_T("BH=\'")+strP2+_T("\'")))
					{
						//�ڲ۸����Ա�%s��û�ҵ��۸��ͺ�%s
						sTmp.Format(GetResStr(IDS_NotFoundThisCSNo),_T("SSteelPropertyCS"),strP2);
						throw sTmp;
					}
					//����2�۸ֹ��Ծ�Ix,cm4->mm4
					rsTmp.GetFieldValue(_T("Ix"),vTmp);
					float J2y=vtof(vTmp)*10000.0;
					rsTmp1.Edit();
					rsTmp1.SetFieldValue(_T("J2y"),COleVariant(J2y));
					//����2�۸ֹ��Ծ�Iy,cm4->mm4
					rsTmp.GetFieldValue(_T("Iy"),vTmp);
					float J2z=vtof(vTmp)*10000.0;
					rsTmp1.SetFieldValue(_T("J2z"),COleVariant(J2z));
					//����1�۸���С���Ծ�,cm4->mm4
					rsTmp1.SetFieldValue(_T("Imin2"),COleVariant(J2z>J2y?J2y:J2z));
					//����2�۸ֿ����Wx,cm3->mm3
					rsTmp.GetFieldValue(_T("Wx"),vTmp);
					float W2y=vtof(vTmp)*1000.0;
					rsTmp1.SetFieldValue(_T("W2y"),COleVariant(W2y));
					//����2�۸ֿ����Wy,cm3->mm3
					rsTmp.GetFieldValue(_T("Wy"),vTmp);
					float W2z=vtof(vTmp)*1000.0;
					rsTmp1.SetFieldValue(_T("W2z"),COleVariant(W2z));
					//����2�۸ֽ����S,cm2->mm2
					rsTmp.GetFieldValue(_T("S"),vTmp);
					float F2=vtof(vTmp)*100.0;
					rsTmp1.SetFieldValue(_T("F2"),COleVariant(F2));
					//����2�۸ָ߶�h,mm
					rsTmp.GetFieldValue(_T("h"),vTmp);
					float h2=vtof(vTmp);
					rsTmp1.SetFieldValue(_T("h2"),COleVariant(h2));
					//����2�۸��ȿ�b,mm
					rsTmp.GetFieldValue(_T("b"),vTmp);
					float b2=vtof(vTmp);
					rsTmp1.SetFieldValue(_T("b2"),COleVariant(b2));
					
					//˫�۸־��վ���g,mm
					float g=PL3-2*h1;
					rsTmp1.SetFieldValue(_T("g"),COleVariant(g));
					//˫�۸����ľ�B1,mm
					float B1=PL3-h1;
					//��ΪrsTmp1�д����ֶ�b1,��Jet���ݿ��ֶ����ǲ����ִ�Сд�ģ��ʻ�B1->Bx,������Ӧ�Ĺ�ʽB1�Ѿ��滻��
					rsTmp1.SetFieldValue(_T("Bx"),COleVariant(B1));
					//��������׼������
					rsTmp1.Update();					
					
					//��̬/��̬����
					for(i4=1;i4<=2;i4++)
					{
						if(i4==2)
						{
							mfpx=modPHScal::pxrc;
							mfpy=modPHScal::pyrc;
							mfpz=modPHScal::pzrc;
							mfmx=modPHScal::mxrc;
							mfmy=modPHScal::myrc;
							mfmz=modPHScal::mzrc;
						}
						else
						{
							mfpx=modPHScal::pxlc;
							mfpy=modPHScal::pylc;
							mfpz=modPHScal::pzlc;
							mfmx=modPHScal::mxlc;
							mfmy=modPHScal::mylc;
							mfmz=modPHScal::mzlc;
						}
						
						rsTmp1.Edit();
						rsTmp1.SetFieldValue(_T("Px"),COleVariant(mfpx));
						rsTmp1.SetFieldValue(_T("Py"),COleVariant(mfpy));
						rsTmp1.SetFieldValue(_T("Pz"),COleVariant(mfpz));
						rsTmp1.SetFieldValue(_T("Mx"),COleVariant(mfmx));
						rsTmp1.SetFieldValue(_T("My"),COleVariant(mfmy));
						rsTmp1.SetFieldValue(_T("Mz"),COleVariant(mfmz));
						rsTmp1.Update();
						
						//����1�͹���2����Ŀ����,mm3
						//float Wyh1=0.233*K*(2*h1+g)*(2*h1+g)+1.4*K*(bb+b1)*(h1+0.5*g);
						//float Wzh1=0.233*K*((b1+bb)*(b1+bb)+b1*b1)+0.35*K*(2*h1+g)*(bb+b1);
						//float Wyh2=(1.4*K*h2*h2*h2+4.2*K*h2*B1*B1)/3/(B1+h2)+0.7*K*b2*2*(B1*B1+h2*h2)/(B1+h2);
						//float Wzh2=K*b2*(0.467*b2+1.4*h2);
						
						if(!rsCal.FindFirst(_T("SEQ=1")))
						{
							//����%s��%s�������ļ��㹫ʽû�ҵ���
							sTmp.Format(IDS_SACalFormulaXNotFound,Ptype[i],_T("1"));
							throw sTmp;
						}
						else
						{
							if(i4==2)
							{//�������ݣ�ֻ����һ�Σ��ӿ��ٶ�
								//rsCal.GetFieldValue(_T("F1saFormula"),vTmp);
								//F1sa=vtos(vTmp);
								//rsCal.GetFieldValue(_T("F2saFormula"),vTmp);
								//F2sa=vtos(vTmp);
								rsCal.GetFieldValue(_T("Wyh1saFormula"),vTmp);
								Wyh1sa=vtos(vTmp);
								rsCal.GetFieldValue(_T("Wzh1saFormula"),vTmp);
								Wzh1sa=vtos(vTmp);
								rsCal.GetFieldValue(_T("Wyh2saFormula"),vTmp);
								Wyh2sa=vtos(vTmp);
								rsCal.GetFieldValue(_T("Wzh2saFormula"),vTmp);
								Wzh2sa=vtos(vTmp);
								
								//Ӧ���оݿ��Ȼ�ã������Ժ����
								rsCal.GetFieldValue(_T("SIGMA1saMaxFormula"),vTmp);
								SIGMA1saMax=vtos(vTmp);
								rsCal.GetFieldValue(_T("TAO1saMaxFormula"),vTmp);
								TAO1saMax=vtos(vTmp);
								rsCal.GetFieldValue(_T("SIGMA2saMaxFormula"),vTmp);
								SIGMA2saMax=vtos(vTmp);
								rsCal.GetFieldValue(_T("TAO2saMaxFormula"),vTmp);
								TAO2saMax=vtos(vTmp);
								rsCal.GetFieldValue(_T("lamda1Formula"),vTmp);
								lamda1=vtos(vTmp);
								rsCal.GetFieldValue(_T("lamda2Formula"),vTmp);
								lamda2=vtos(vTmp);
								
								//��ʼ���㼸�Ρ���������
								SQLx=_T("SELECT ")+ Wyh1sa+_T(" AS Wyh1sac, ")+ Wyh2sa+_T(" AS Wyh2sac, ") + Wzh1sa+_T(" AS Wzh1sac, ")+ Wzh2sa+_T(" AS Wzh2sac, ")+SIGMA1saMax+_T(" AS SIGMA1saMaxc, ")+TAO1saMax+_T(" AS TAO1saMaxc, ")+SIGMA2saMax+_T(" AS SIGMA2saMaxc, ")+TAO2saMax+_T(" AS TAO2saMaxc, ")+lamda1+_T(" AS lamda1c, ")+lamda2+_T(" AS lamda2c FROM tmpCalFixPhs");
								if(rsTmp2.IsOpen())
									rsTmp2.Close();
								rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
								rsTmp2.Open(dbOpenDynaset,SQLx);
								
								rsTmp1.Edit();
								rsTmp2.GetFieldValue(_T("Wyh1sac"),vTmp);
								rsTmp1.SetFieldValue(_T("Wyh1"),vTmp);
								
								rsTmp2.GetFieldValue(_T("Wzh1sac"),vTmp);
								rsTmp1.SetFieldValue(_T("Wzh1"),vTmp);
								
								rsTmp2.GetFieldValue(_T("Wyh2sac"),vTmp);
								rsTmp1.SetFieldValue(_T("Wyh2"),vTmp);
								
								rsTmp2.GetFieldValue(_T("Wzh2sac"),vTmp);
								rsTmp1.SetFieldValue(_T("Wzh2"),vTmp);							
								
								rsTmp2.GetFieldValue(_T("TAO1saMaxc"),vTmp);
								rsTmp1.SetFieldValue(_T("TAO1saMax"),vTmp);
								mfTAO1saMax=vtof(vTmp);
								rsTmp2.GetFieldValue(_T("SIGMA1saMaxc"),vTmp);
								rsTmp1.SetFieldValue(_T("SIGMA1saMax"),vTmp);
								mfSIGMA1saMax=vtof(vTmp);
								
								rsTmp2.GetFieldValue(_T("TAO2saMaxc"),vTmp);
								rsTmp1.SetFieldValue(_T("TAO2saMax"),vTmp);
								mfTAO2saMax=vtof(vTmp);
								rsTmp2.GetFieldValue(_T("SIGMA2saMaxc"),vTmp);
								rsTmp1.SetFieldValue(_T("SIGMA2saMax"),vTmp);
								mfSIGMA2saMax=vtof(vTmp);
								
								rsTmp2.GetFieldValue(_T("lamda1c"),vTmp);
								rsTmp1.SetFieldValue(_T("lamda1"),vTmp);
								mfLamda1=vtof(vTmp);
								rsTmp2.GetFieldValue(_T("lamda2c"),vTmp);
								rsTmp1.SetFieldValue(_T("lamda2"),vTmp);
								mfLamda2=vtof(vTmp);
								
								rsTmp1.Update();
								float mf1=0.0,mf2=0,mfl1=0,mfl2=0;
								SQLx=_T("SELECT Lamda,Fi FROM SteadyDecreaseCoef WHERE Material=\'") + m_strMaterial + _T("\'");
								if(rs2.IsOpen())
									rs2.Close();
								rs2.m_pDatabase=&EDIBgbl::dbSACal;//20071103 "dbSORT" ��Ϊ "dbSACal"
								rs2.Open(dbOpenSnapshot,SQLx);
								if(rs2.IsEOF() && rs2.IsBOF())
								{
									//��%s���ȶ��ۼ�ϵ����%s��û��%s���ֲ��ϡ�
									sTmp.Format(IDS_NoMaterialInSteadyDecreaseCoef,rs2.m_pDatabase->GetName(),_T("SteadyDecreaseCoef"),m_strMaterial);
									throw sTmp;
								}
								else
								{
									if(!rs2.FindFirst(_T("Lamda>")+ftos(mfLamda1)))
									{
										//�����ϸ����%s����%s��%s�������ֵ��
										sTmp.Format(IDS_LamdaOverMaxValue,ftos(mfLamda1),modPHScal::dbZDJcrude.GetName(),_T("SteadyDecreaseCoef"));
										throw sTmp;
									}
									else
									{
										rs2.GetFieldValue(_T("Lamda"),vTmp);
										mfl2=vtof(vTmp);
										rs2.GetFieldValue(_T("Fi"),vTmp);
										mf2=vtof(vTmp);
										rs2.MovePrev();
										if(rs2.IsBOF())
										{
											//һ�㲻����,��Ϊ������ֵ��0��ʼ
										}
										else
										{
											//��ֵ��Fi1
											rs2.GetFieldValue(_T("Lamda"),vTmp);
											mfl1=vtof(vTmp);
											rs2.GetFieldValue(_T("Fi"),vTmp);
											mf1=vtof(vTmp);
											mfFi1=(mf2-mf1)/(mfl2-mfl1)*(mfLamda1-mfl1)+mf1;
											rsTmp1.Edit();
											rsTmp1.SetFieldValue(_T("Fi1"),COleVariant(mfFi1));																					
											rsTmp1.Update();
										}
									}
									//�󹹼�2�ȶ��ۼ�ϵ��Fi2
									if(!rs2.FindFirst(_T("Lamda>")+ftos(mfLamda2)))
									{
										//�����ϸ����%s����%s��%s�������ֵ��
										sTmp.Format(IDS_LamdaOverMaxValue,ftos(mfLamda2),modPHScal::dbZDJcrude.GetName(),_T("SteadyDecreaseCoef"));
										throw sTmp;
									}
									else
									{
										rs2.GetFieldValue(_T("Lamda"),vTmp);
										mfl2=vtof(vTmp);
										rs2.GetFieldValue(_T("Fi"),vTmp);
										mf2=vtof(vTmp);
										rs2.MovePrev();
										if(rs2.IsBOF())
										{
											//һ�㲻����,��Ϊ������ֵ��0��ʼ
										}
										else
										{
											//��ֵ��Fi1
											rs2.GetFieldValue(_T("Lamda"),vTmp);
											mfl1=vtof(vTmp);
											rs2.GetFieldValue(_T("Fi"),vTmp);
											mf1=vtof(vTmp);
											mfFi2=(mf2-mf1)/(mfl2-mfl1)*(mfLamda2-mfl1)+mf1;
											rsTmp1.Edit();
											rsTmp1.SetFieldValue(_T("Fi2"),COleVariant(mfFi2));																					
											rsTmp1.Update();
										}
									}
								}
							}
						}
						
						//ȡPy1=1.5*(Py+/-fabs(1000*mx+pz*modPHScal::PAfixH)/B1),�Ծ���ֵ�ϴ�����ΪУ�˹�����unit,mm
						//��ʼ��̬����
						float Py1r1=1.5*(mfpy+fabs(1000*mfmx+mfpz*modPHScal::PAfixH)/B1);
						float Py1r2=1.5*(mfpy-fabs(1000*mfmx+mfpz*modPHScal::PAfixH)/B1);
						float Py1r=((fabs(Py1r1)>fabs(Py1r2))?Py1r1:Py1r2);
						
						rsTmp1.Edit();
						rsTmp1.SetFieldValue(_T("Py1"),COleVariant(Py1r1));
						rsTmp1.SetFieldValue(_T("Py2"),COleVariant(Py1r2));
						rsTmp1.Update();
						
						if( (mfpx>=0 && 0>=Py1r) || (0>=mfpx && mfpx>=1.732*Py1r))
						{
							i5=1;
						}
						else if( (mfpx>=1.732*Py1r) && (Py1r>0))
						{
							i5=2;
						}
						if( (Py1r>=0 && 0>mfpx) || (1.732*Py1r>=mfpx && mfpx>=0))
						{
							i5=3;
						}
						else if( (mfpx<=1.732*Py1r) && (Py1r<0))
						{
							i5=4;
						}
						
						if(!rsCal.FindFirst(_T("SEQ=")+ltos(i5)))
						{
							//����%s��%s�������ļ��㹫ʽû�ҵ���
							sTmp.Format(IDS_SACalFormulaXNotFound,Ptype[i],_T("1"));
							throw sTmp;
						}
						else
						{
							rsCal.GetFieldValue(_T("SIGMA1saFormula"),vTmp);
							SIGMA1sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOx1saFormula"),vTmp);
							TAOx1sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOy1saFormula"),vTmp);
							TAOy1sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOz1saFormula"),vTmp);
							TAOz1sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAO1saFormula"),vTmp);
							TAO1sa=vtos(vTmp);
							
							rsCal.GetFieldValue(_T("SIGMA2saFormula"),vTmp);
							SIGMA2sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOx2saFormula"),vTmp);
							TAOx2sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOy2saFormula"),vTmp);
							TAOy2sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOz2saFormula"),vTmp);
							TAOz2sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAO2saFormula"),vTmp);
							TAO2sa=vtos(vTmp);
							
							//ѹ�˵ĳ���ϵ��u1,u2:����=2,��֧=0.5							
							rsCal.GetFieldValue(_T("u1"),vTmp);
							rsTmp1.Edit();
							rsTmp1.SetFieldValue(_T("u1"),vTmp);
							rsCal.GetFieldValue(_T("u2"),vTmp);
							rsTmp1.SetFieldValue(_T("u2"),vTmp);
							rsTmp1.Update();
							
							//��ʼ������Ӧ��sigma����Ӧ��tao
							SQLx=_T("SELECT ")+ TAOx1sa+_T(" AS TAOx1sac, ")+ TAOy1sa+_T(" AS TAOy1sac, ")+ TAOz1sa+_T(" AS TAOz1sac,")
								+ TAOx2sa+_T(" AS TAOx2sac, ")+ TAOy2sa+_T(" AS TAOy2sac, ")+ TAOz2sa+_T(" AS TAOz2sac FROM tmpCalFixPhs");
							if(rsTmp2.IsOpen())
								rsTmp2.Close();
							rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
							rsTmp2.Open(dbOpenDynaset,SQLx);
							
							rsTmp1.Edit();
							rsTmp2.GetFieldValue(_T("TAOx1sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOx1sa"),vTmp);
							rsTmp2.GetFieldValue(_T("TAOy1sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOy1sa"),vTmp);
							rsTmp2.GetFieldValue(_T("TAOz1sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOz1sa"),vTmp);
							
							rsTmp2.GetFieldValue(_T("TAOx2sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOx2sa"),vTmp);
							rsTmp2.GetFieldValue(_T("TAOy2sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOy2sa"),vTmp);
							rsTmp2.GetFieldValue(_T("TAOz2sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOz2sa"),vTmp);
							rsTmp1.Update();
							
							SQLx=_T("SELECT ")+ TAO1sa+_T(" AS TAO1sac,")+ TAO2sa+_T(" AS TAO2sac,")+SIGMA1sa+_T(" AS SIGMA1sac,")+SIGMA2sa+_T(" AS SIGMA2sac FROM tmpCalFixPhs");
							if(rsTmp2.IsOpen())
								rsTmp2.Close();
							rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
							rsTmp2.Open(dbOpenDynaset,SQLx);
							
							rsTmp2.GetFieldValue(_T("SIGMA1sac"),vTmp);
							mfSIGMA1sa=vtof(vTmp);
							rsTmp1.Edit();
							rsTmp1.SetFieldValue(_T("SIGMA1sa"),vTmp);
							
							rsTmp2.GetFieldValue(_T("SIGMA2sac"),vTmp);
							mfSIGMA2sa=vtof(vTmp);
							rsTmp1.SetFieldValue(_T("SIGMA2sa"),vTmp);
							
							rsTmp2.GetFieldValue(_T("TAO1sac"),vTmp);
							mfTAO1sa=vtof(vTmp);							
							rsTmp1.SetFieldValue(_T("TAO1sa"),vTmp);
							
							rsTmp2.GetFieldValue(_T("TAO2sac"),vTmp);
							mfTAO2sa=vtof(vTmp);
							rsTmp1.SetFieldValue(_T("TAO2sa"),vTmp);
							rsTmp1.Update();
							
							//����1(���͸�P1)ǿ��У��(�۸ֲ���)
							//unit,kgf/mm2
							if(mfSIGMA1sa>mfSIGMA1saMax)
							{
								if(bEOF)
								{
									//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
									sTmp.Format(GetResStr(IDS_SSteelStressOver),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(mfSIGMA1sa),ftos(mfSIGMA1saMax));
									throw sTmp;
								}
								else
									bErr=true;							
							}
							//����1(���͸�P1)ǿ��У��(���첿��)
							//����1����ϳ�Ӧ��
							if(mfTAO1sa>mfTAO1saMax)
							{
								if(bEOF)
								{
									//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
									sTmp.Format(GetResStr(IDS_SSteelStressOver1),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(mfTAO1sa),ftos(mfTAO1saMax));
									throw sTmp;
								}
								else
									bErr=true;							
							}
							
							//����2(б�Ų۸�P2)ǿ��У��(�۸ֲ���)
							//unit,kgf/mm2
							if(mfSIGMA2sa>mfSIGMA2saMax)
							{
								if(bEOF)
								{
									//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
									sTmp.Format(GetResStr(IDS_SSteelStressOver),ltos(modPHScal::zdjh),_T("2"),strP2,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(mfSIGMA2sa),ftos(mfSIGMA2saMax));
									throw sTmp;
								}
								else
									bErr=true;							
							}
							//����2(б�Ų۸�P2)ǿ��У��(���첿��)
							//����2����ϳ�Ӧ��
							if(mfTAO1sa>mfTAO1saMax)
							{
								if(bEOF)
								{
									//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
									sTmp.Format(GetResStr(IDS_SSteelStressOver1),ltos(modPHScal::zdjh),_T("2"),strP2,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(mfTAO2sa),ftos(mfTAO2saMax));
									throw sTmp;
								}
								else
									bErr=true;							
							}
						}						
					}
					
					if(!bErr /*&& (dh>=36)*/) //ע�͵��Ĳ��ֿɲ��Ի���Ժ�����ֲ�����
					{
						//û���ޣ��ҵ�
						break;
					}
					else
					{
						rsX.MoveNext();
						if(rsX.IsEOF())
						{
							//�Ѿ�����¼β������ѭ��һ�Σ��Ա㱨��
							bEOF=true;
							continue;
						}
						else
							//���¿�ʼ֮ǰ�����ó��ޱ�־
							bErr=false;
						
						//��ù���3
						rsX.GetFieldValue(_T("P3"),vTmp);
						strP3=vtos(vTmp);	
						rsTmp1.Edit();
						rsTmp1.SetFieldValue(_T("P3"),vTmp);
						//�˴�ֻ��Ӧ�й���׼�����ʻ�ʱҪ�޸Ĵ���
						n=strP3.Find((_T("x")));
						//����3�ȿ�bb,unit:mm
						bb=_tcstod(strP3.Mid(1,n-1),NULL);
						rsTmp1.SetFieldValue(_T("bb"),COleVariant(bb));
						//������͸�P1
						rsX.GetFieldValue(_T("P1"),vTmp);
						strP1=vtos(vTmp);
						rsTmp1.SetFieldValue(_T("P1"),vTmp);
						//��ù���2�͸�P2
						rsX.GetFieldValue(_T("P2"),vTmp);
						strP2=vtos(vTmp);
						rsTmp1.SetFieldValue(_T("P2"),vTmp);
						//˫�۸���Ե������PL3,mm
						rsX.GetFieldValue(_T("PL3"),vTmp);
						PL3=vtof(vTmp);
						//rsX�е�PL3��CHAR(20),��rsTmp1�е�PL3��REAL,�ɼ������ܹ��Զ�ת����
						rsTmp1.SetFieldValue(_T("PL3"),vTmp);
						//�۸ֺ���߶�K,mm
						rsX.GetFieldValue(_T("size2"),vTmp);
						K=vtof(vTmp);
						rsTmp1.SetFieldValue(_T("K"),vTmp);
						rsTmp1.Update();
						//���DH,mm
						rsX.GetFieldValue(_T("dh"),vTmp);
						dh=vtoi(vTmp);				
					}
					
					/*
					//����1�͹���2����Ŀ����,mm3
					float Wyh1=0.233*K*(2*h1+g)*(2*h1+g)+1.4*K*(bb+b1)*(h1+0.5*g);
					float Wzh1=0.233*K*((b1+bb)*(b1+bb)+b1*b1)+0.35*K*(2*h1+g)*(bb+b1);
					float Wyh2=(1.4*K*h2*h2*h2+4.2*K*h2*B1*B1)/3/(B1+h2)+0.7*K*b2*2*(B1*B1+h2*h2)/(B1+h2);
					float Wzh2=K*b2*(0.467*b2+1.4*h2);
					
					  //ȡPy1=1.5*(Py+/-fabs(1000*mx+pz*modPHScal::PAfixH)/B1),�Ծ���ֵ�ϴ�����ΪУ�˹�����unit,mm
					  //��ʼ��̬����
					  float Py1r1=1.5*(modPHScal::pyrc+fabs(1000*modPHScal::mxrc+modPHScal::pzrc*modPHScal::PAfixH)/B1);
					  float Py1r2=1.5*(modPHScal::pyrc-fabs(1000*modPHScal::mxrc+modPHScal::pzrc*modPHScal::PAfixH)/B1);
					  float Py1r=((fabs(Py1r1)>fabs(Py1r2))?Py1r1:Py1r2);
					  
						if( (modPHScal::pxrc>=0 && 0>=Py1r) || (0>=modPHScal::pxrc && modPHScal::pxrc>=1.732*Py1r))
						{
						//����1(���͸�P1)ǿ��У��(�۸ֲ���)
						float sigma1r=(modPHScal::pxrc-1.732*Py1r)/2/F1+fabs(1000*modPHScal::myrc-modPHScal::pzrc*modPHScal::gdw)*(B1+h1)*J1y/4/(J1y+0.25*F1*B1*B1)/(J1y+0.65*J2y)+fabs(1000.0*modPHScal::mzrc-modPHScal::pxrc*modPHScal::PAfixH)*J1z/4/W1z/(J1z+0.65*J2z);//unit,kgf/mm2
						//float sigma1r=(px-1.732*Py1)/2/F1+abs(1000*my-pz*gdw)*(B1+h1)*J1y/4/(J1y+0.25*F1*B1*B1)/(J1y+0.65*J2y)+fabs(1000.0*mz-px*H)*J1z/4/W1z/(J1z+0.65*J2z);//unit,kgf/mm2
						if(sigma1r>Sigma0)
						{
						if(bEOF)
						{
						//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
						sTmp.Format(GetResStr(IDS_SSteelStressOver),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(sigma1r),ftos(Sigma0));
						throw sTmp;
						}
						else
						bErr=true;							
						}
						//����1(���͸�P1)ǿ��У��(���첿��)
						//float TAOx1r=(px-1.732*Py1)/1.4/K/(PL3+2*b1+bb)+abs(1000*my-pz*gdw)*J1y/Wyh1/(J1y+0.65*J2y)+(1000*mz-px*H)*J1z/2/Wzh1/(J1z+0.65*J2z);//unit,kgf/mm2
						//float TAOy1r=3*abs(1000.0*mz-px*H)*J1z/2.8/gdw/(J1z+0.65*J2z)/(bb+2*b1)/K;//unit,kgf/mm2
						//float TAOz1r=pz/2.8/K/(PL3-h1);//unit,kgf/mm2
						float TAOx1r=(modPHScal::pxrc-1.732*Py1r)/1.4/K/(PL3+2*b1+bb)+fabs(1000*modPHScal::myrc-modPHScal::pzrc*modPHScal::gdw)*J1y/Wyh1/(J1y+0.65*J2y)+(1000*modPHScal::mzrc-modPHScal::pxrc*modPHScal::PAfixH)*J1z/2/Wzh1/(J1z+0.65*J2z);//unit,kgf/mm2
						float TAOy1r=3*fabs(1000.0*modPHScal::mzrc-modPHScal::pxrc*modPHScal::PAfixH)*J1z/2.8/modPHScal::gdw/(J1z+0.65*J2z)/(bb+2*b1)/K;//unit,kgf/mm2
						float TAOz1r=modPHScal::pzrc/2.8/K/(PL3-h1);//unit,kgf/mm2
						//����1����ϳ�Ӧ��
						float TAO1r=sqrt(TAOx1r*TAOx1r+TAOy1r*TAOy1r+TAOz1r*TAOz1r);//unit,kgf/mm2
						//׼��
						if(TAO1r>0.6*Sigma0)
						{							
						if(bEOF)
						{
						//%s��֧���ܹ���%s�۸�%s������̬��Ӧ��ֵ%s��������ֵ%s
						sTmp.Format(GetResStr(IDS_SSteelStressOver1),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(TAO1r),ftos(0.6*Sigma0));
						throw sTmp;
						}
						else
						bErr=true;							
						}
						}
						
						  float Py1l1=1.5*(modPHScal::pylc+fabs(1000*modPHScal::mxlc+modPHScal::pzlc*modPHScal::PAfixH)/B1);
						  float Py1l2=1.5*(modPHScal::pylc-fabs(1000*modPHScal::mxlc+modPHScal::pzlc*modPHScal::PAfixH)/B1);
						  float Py1l=((fabs(Py1l1)>fabs(Py1l2))?Py1l1:Py1l2);
						  
							if( (modPHScal::pxlc>=0 && 0>=Py1l) || (0>=modPHScal::pxlc && modPHScal::pxlc>=1.732*Py1l))
							{
							//����1(���͸�P1)ǿ��У��(�۸ֲ���)
							float sigma1l=(modPHScal::pxlc-1.732*Py1l)/2/F1+fabs(1000*modPHScal::mylc-modPHScal::pzlc*modPHScal::gdw)*(B1+h1)*J1y/4/(J1y+0.25*F1*B1*B1)/(J1y+0.65*J2y)+fabs(1000.0*modPHScal::mzlc-modPHScal::pxlc*modPHScal::PAfixH)*J1z/4/W1z/(J1z+0.65*J2z);//unit,kgf/mm2
							if(sigma1l>Sigma0)
							{
							if(bEOF)
							{
							//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
							sTmp.Format(GetResStr(IDS_SSteelStressOver),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(sigma1l),ftos(Sigma0));
							throw sTmp;
							}
							else
							bErr=true;							
							}
							//����1(���͸�P1)ǿ��У��(���첿��)
							float TAOx1l=(modPHScal::pxlc-1.732*Py1l)/1.4/K/(PL3+2*b1+bb)+fabs(1000*modPHScal::mylc-modPHScal::pzlc*modPHScal::gdw)*J1y/Wyh1/(J1y+0.65*J2y)+(1000*modPHScal::mzlc-modPHScal::pxlc*modPHScal::PAfixH)*J1z/2/Wzh1/(J1z+0.65*J2z);//unit,kgf/mm2
							float TAOy1l=3*fabs(1000.0*modPHScal::mzlc-modPHScal::pxlc*modPHScal::PAfixH)*J1z/2.8/modPHScal::gdw/(J1z+0.65*J2z)/(bb+2*b1)/K;//unit,kgf/mm2
							float TAOz1l=modPHScal::pzlc/2.8/K/(PL3-h1);//unit,kgf/mm2
							//����1����ϳ�Ӧ��
							float TAO1l=sqrt(TAOx1l*TAOx1l+TAOy1l*TAOy1l+TAOz1l*TAOz1l);//unit,kgf/mm2
							//׼��
							if(TAO1l>0.6*Sigma0)
							{							
							if(bEOF)
							{
							//%s��֧���ܹ���%s�۸�%s������̬��Ӧ��ֵ%s��������ֵ%s
							sTmp.Format(GetResStr(IDS_SSteelStressOver1),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptCoolStatus),ftos(TAO1l),ftos(0.6*Sigma0));
							throw sTmp;
							}
							else
							bErr=true;							
							}
							}
							
							  if(!bErr && (dh>36))
							  {
							  //û���ޣ��ҵ�
							  break;
							  }
							  else
							  {
							  rsX.MoveNext();
							  if(rsX.IsEOF())
							  {
							  //�Ѿ�����¼β������ѭ��һ�Σ��Ա㱨��
							  bEOF=true;
							  continue;
							  }
							  else
							  //���¿�ʼ֮ǰ�����ó��ޱ�־
							  bErr=false;
							  
								//��ù���3
								rsX.GetFieldValue(_T("P3"),vTmp);
								strP3=vtos(vTmp);
								//�˴�ֻ��Ӧ�й���׼�����ʻ�ʱҪ�޸Ĵ���
								n=strP3.Find((_T("x")));
								//����3�ȿ�bb,unit:mm
								bb=atof(strP3.Mid(1,n-1));
								//������͸�P1
								rsX.GetFieldValue(_T("P1"),vTmp);
								strP1=vtos(vTmp);
								//��ù���2�͸�P2
								rsX.GetFieldValue(_T("P2"),vTmp);
								strP2=vtos(vTmp);
								//˫�۸���Ե������PL3,mm
								rsX.GetFieldValue(_T("PL3"),vTmp);
								PL3=vtof(vTmp);
								//�۸ֺ���߶�K,mm
								rsX.GetFieldValue(_T("size2"),vTmp);
								K=vtof(vTmp);
								//���DH,mm
								rsX.GetFieldValue(_T("dh"),vTmp);
								dh=vtoi(vTmp);
								}
*/
				}while(true);//��¼β��ѭ��һ��,�Ա㱨��
				
				//����һ������У�˵����ݿ��ļ�����Ӧ��Excel�ļ�
				EDIBgbl::ChangeColumnsToRows(EDIBgbl::dbPRJ,_T("tmpCalFixPhs"),_T("CheckCalFixPhs"));
				EDIBDB::OutPutTable( _T("CheckCalFixPhs"), basDirectory::ProjectDBDir+_T("CheckCalculation.xls"), _T("Excel 5.0;"), EDIBgbl::dbPRJ,  _T("CheckCalFixPhs"), _T("Excel 5.0;"));
				
				SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + ftos(modPHScal::gdw) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' AND dh=") + ltos(dh) + _T(" ORDER BY GDW1,PmaxSF");
			}
			//����ǿ�ȼ���:����Ժ�̶�֧�ܸ���˫�����Ǽ�SJ8(ClassID=iSALbraceFixG47=4)
		}			
		break;
	case iG100:
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
		//vtoi(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("G100No"))));
		break;
	case iGCement:
		//�������ṹ���ش���
		SQLx.Format(_T("SELECT * FROM %s WHERE trim(CustomID)=\'%s\'"),
			modPHScal::tbnSA,
			Ptype[i]);
		break;
	default:
		sTmp.Format(GetResStr(IDS_CanNotHandlePART),sPartID);
		throw sTmp;
	}
	
	//��ԭʼ���ݿ��в����������������,
	//if( FirstCal = MaxCalCount ){ MsgBox _T("Sqlx=") + SQLx
	//commonHandle:
	
	//�ж�˫�����֧���ܷ���ܺ���
	if( modPHScal::glClassID == iSAbeam || modPHScal::glClassID == iG52_55)
	{
		//����SQLx�Ա���Ϊ������ʾ��Ϣ,Ҫ������һ�γ����в�Ҫʹ��sTmpSQL
		//����Ҫ,��ΪGetBHforDoubleCSBeam����û��ʹ��SQLx�������ƻ�SQLxֵ
		//sTmpSQL = SQLx
		if( ! modPHScal::gbCalSAbyConservativenessMethod )
		{
			//����������ط�ʽѡ��������򰴻���Ժ�̷�ú�۹ܵ�֧��������ֲ�֮˫������㹫ʽ��������͸֣�����ǿ�ȹ�ʽ��δ����ˮƽ�����͸�ƫС��
			//�Ǹ��������Զ�ѡ��۸ּ�֧������
			if( modPHScal::gintParallelNum == 2 )
			{
				if( modPHScal::glNumSA == 1 )
				{
					//˫���㣬������
					p1 = tmpSelPJG / 2;
					p2 = p1;
					//���ǰһ���Ǻ�������,���������ֱ������˨���ӣ���Ҫ˫�۸�
					if( PtypeID[i - 1] == _T("PHD") || PtypeID[i - 1] == _T("PHD1") || PtypeID[i-1] == _T("LHD") || PtypeID[i-1] == _T("LHD1") || PtypeID[i-1] == _T("ZHA") || PtypeID[i-1] == _T("ZHB") )
					{
						//ֱ����˫�۸�
						k = 2;
					}
					else
					{
						//�����ҵ��۸�
						k = 1;
					}
					strTmp = GetBHforDoubleCSBeam(modPHScal::gdw / 10, modPHScal::DblHangerRodDist / 10, modPHScal::Lspan / 10, p1, p2, tmpCustomID1, k, FirstCal);
					if( strTmp == _T("") )
					{
						sTmp.Format(GetResStr(IDS_NotFoundAnyBHinXSteelProperty),_T("SSteelPropertyCS"));
						throw sTmp;
					}
					else
					{
						SQLx.Format(_T("SELECT * FROM %s WHERE GDW1>=IIF( %g > L1/2 , L1 - %g , %g ) AND L1>= %g AND Pnum1=%d AND Val(Mid(P1,2))>=%g AND Mid(P1,2)>=\'%s\' AND trim(CustomID)=\'%s\' ORDER BY GDW1,L1,%s"),
							modPHScal::tbnSA,
							modPHScal::gdw + (modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2,
							modPHScal::gdw + (modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2,
							modPHScal::gdw + (modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2,
							modPHScal::Lspan,
							k,
							_tcstod(strTmp,NULL),
							Trim(strTmp),
							Ptype[i],
							SAfDPmax);
						if(rsX.IsOpen())
							rsX.Close();
						rsX.Open(dbOpenSnapshot,SQLx);
						if( rsX.IsEOF() && rsX.IsBOF() )
						{//1
							if( k == 2 )
							{
								//�Ѿ�����˫�۸֣��˳�
								if( i > 0 )
								{
									if( i < C - 1 )
									{
										sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx, Ptype[i],  Ptype[i - 1],Ptype[i + 1]);
										throw sTmp;
									}
									else
									{
										sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude) ,SQLx, Ptype[i], Ptype[i - 1], _T("NULL"));
										throw sTmp;
									}
								}
								else
								{
									sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx,  Ptype[i], _T("NULL"), Ptype[i + 1]);
									throw sTmp;
								}
							}
							else
							{
								//û���ҵ����۸�
								//����˫�۸�
								k = 2;
							}
							strTmp = GetBHforDoubleCSBeam(modPHScal::gdw / 10, modPHScal::DblHangerRodDist / 10, modPHScal::Lspan / 10, p1, p2, tmpCustomID1, k, FirstCal);
							if( strTmp == _T("") )
							{
								sTmp.Format(GetResStr(IDS_NotFoundAnyBHinXSteelProperty),_T("SSteelPropertyCS"));
								throw sTmp;
							}
							else
							{
								SQLx.Format(_T("SELECT * FROM %s WHERE GDW1>=IIF( %g > L1/2 , L1 - %g , %g ) AND L1>= %g AND Pnum1=%d AND Val(Mid(P1,2))>=%g AND Mid(P1,2)>=\'%s\' AND trim(CustomID)=\'%s\' ORDER BY GDW1,L1,%s"),
									modPHScal::tbnSA,
									modPHScal::gdw + (modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2,
									modPHScal::gdw + (modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2,
									modPHScal::gdw + (modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2,
									modPHScal::Lspan,
									k,
									_tcstod(strTmp,NULL),
									Trim(strTmp),
									Ptype[i],
									SAfDPmax);
								//SQLx = _T("SELECT * FROM ") + tbnSA + _T(" WHERE GDW1>=IIF(") + (modPHScal::gdw + IIf(glNumSA = 1, modPHScal::DblHangerRodDist, 0) / 2) + _T(">L1/2,L1-") + (modPHScal::gdw + IIf(glNumSA = 1, modPHScal::DblHangerRodDist, 0) / 2) + _T(",") + (modPHScal::gdw + IIf(glNumSA = 1, modPHScal::DblHangerRodDist, 0) / 2) + _T(") AND L1>= ") + modPHScal::Lspan + _T(" AND Pnum1=") + k + _T(" AND Val(Mid(P1,2))>=") + Val(strTmp) + _T(" AND Mid(P1,2)>=//") + Trim(strTmp) + _T("// AND trim(CustomID)=//") + Ptype[i] + _T("// ORDER BY GDW1,L1,") + SAfDPmax
								if(rsX.IsOpen())
									rsX.Close();
								rsX.Open(dbOpenSnapshot,SQLx);
								if( rsX.IsEOF() && rsX.IsBOF() )
								{//
									if( k == 2 )
									{
										if( i > 0 )
										{
											if( i < C - 1 )
											{
												sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx,Ptype[i],   Ptype[i - 1],Ptype[i + 1]);
												throw sTmp;
											}
											else
											{
												sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx, Ptype[i],  Ptype[i - 1], _T("NULL"));
												throw sTmp;
											}
										}
										else
										{
											sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx, Ptype[i],  _T("NULL"), Ptype[i + 1]);
											throw sTmp;
										}
									}
								}//
							}
						}//1
					}
			  }
			  else
			  {
				  //˫���㣬˫����
				  if(rsX.IsOpen())
					  rsX.Close();
				  rsX.Open(dbOpenSnapshot,SQLx);
				  p1 = tmpSelPJG / 2;
				  p2 = 0;
				  strTmp = GetBHforDoubleCSBeam(modPHScal::gdw / 10, modPHScal::DblHangerRodDist / 10, modPHScal::Lspan / 10, p1, p2, tmpCustomID1, 2, FirstCal);
			  }
		   }
		   
		   else
		   {
			   //�����㣬������
			   if(rsX.IsOpen())
				   rsX.Close();
			   rsX.Open(dbOpenSnapshot,SQLx);
			   p1 = tmpSelPJG;
			   p2 = 0;
			   strTmp = GetBHforDoubleCSBeam(modPHScal::gdw / 10, modPHScal::DblHangerRodDist / 10, modPHScal::Lspan / 10, p1, p2, tmpCustomID1, 2, FirstCal);
		   }
		}
		else
		{
			//�����ط�ʽ�������
			if(rsX.IsOpen())
				rsX.Close();
			rsX.Open(dbOpenSnapshot,SQLx);
		}
	}
	else
	{
		if(rsX.IsOpen())
			rsX.Close();
		rsX.Open(dbOpenSnapshot,SQLx);
	}
spZ1Z2:
	//�����Ҫ�������ӿס��˳ߴ��Զ����, Ҫ��׳ߴ�ƥ����в���ʱ
	//�Ҳ���ƥ���Ӧȥ�����������ٲ��� add by luorijin 2009.05.12
	if( modPHScal::gbConnectHoleDimMatch && tmpSQL != _T("") && rsX.IsEOF() && rsX.IsBOF() )
	{
		if(rsX.IsOpen())
			rsX.Close();
		SQLx.Replace(tmpSQL," ");
			rsX.Open(dbOpenSnapshot,SQLx);
	}
	if( rsX.IsEOF() && rsX.IsBOF() )
	{
		switch(modPHScal::glClassID)
		{
		case iSAbeam:
			//�Ǹ��������Զ�ѡ��۸ּ�֧������
			if( i > 0 )
			{
				if( i < C - 1 )
				{
					sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx, Ptype[i],  Ptype[i - 1],Ptype[i + 1]);
					throw sTmp;
				}
				else
				{
					sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude), SQLx,Ptype[i],  Ptype[i - 1], _T("NULL"));
					throw sTmp;
				}
			}
			else
			{
				sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude), SQLx,Ptype[i],  _T("NULL"), Ptype[i + 1]);
				throw sTmp;
			}
			break;
		case iROD:
		case iConnected:
			//case iAttached:
			//���L5��L7��L6��L8��L8Dd��L7Dd�����Ӽ��Ҳ������������������ѡ�������ȡ�������ֱ��Ϊ���ֱ��
			tmpSQL = _T("SELECT * FROM ") + modPHScal::tbnPART + _T(" WHERE ") + _T(" Pmax>=");
			sTmp.Format(_T("%g"),tmpSelPJG / iNumPart * (vtob(rsza->GetCollect(_T("ifLongVertPipe")))? iNumPart : 1));
			tmpSQL+=sTmp;
			tmpSQL=tmpSQL  + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY dh,tj,Weight");
			if(rsX.IsOpen())
				rsX.Close();
			rsX.Open(dbOpenSnapshot,tmpSQL);
			if( !rsX.IsEOF() )
			{
				goto spZ1Z2;
			}
			else
			{
				if( modPHScal::gbAutoApplyCSPRLugDia && (FirstCal == 2) && modPHScal::gbPhsIsCSPR && modPHScal::PtypeDiameter[i] > 0 )
				{
					sTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia),modPHScal::dbZDJcrude.GetName(),modPHScal::tbnPART, SQLx);
					throw sTmp;
				}
				else
				{
					if( i > 0 )
					{
						if( i < C - 1 )
						{
							sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx,Ptype[i],   Ptype[i - 1],Ptype[i + 1]);
							throw sTmp;
						}
						else
						{
							sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx, Ptype[i],  Ptype[i - 1], _T("NULL"));
							throw sTmp;
						}
					}
					else
					{
						sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx, Ptype[i],  _T("NULL"), Ptype[i + 1]);
						throw sTmp;
					}
				}
			}
			rsX.Close();
			break;
		default:
			if( i > 0 )
			{
				if( i < C - 1 )
				{
					sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude),SQLx,Ptype[i],   Ptype[i - 1],Ptype[i + 1]);
					throw sTmp;
				}
				else
				{
					sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude), SQLx,Ptype[i],  Ptype[i - 1], _T("NULL"));
					throw sTmp;
				}
			}
			else
			{
				sTmp.Format(GetResStr(IDS_NotFoundPartInZDJCrude), SQLx,Ptype[i],  _T("NULL"), Ptype[i + 1]);
				throw sTmp;
			}
			break;
		}
	}
	else
	{
		rsX.MoveFirst();
		if(phsAvailableTypeRs.IsOpen())
			phsAvailableTypeRs.Close();
		phsAvailableTypeRs.m_pDatabase=rsX.m_pDatabase;
		phsAvailableTypeRs.Open(dbOpenDynaset,rsX.GetSQL());
		//ע�����漸���λ�ã���Ҫ�ƶ����𴦣�������ؼ��㲻��ȷ
		if( modPHScal::glClassID == iCSPR )
		{
			//�����ɽ���ĸֱ��FiJ��FiK��S��I��T��
			//���ݵ�����е������p20 C�ͺ����TֵӦ�ø�ΪIֵ������ǰ���LHE��PHE(1<=DH<=83)�ĳߴ��Ǻϡ�
			//��̲ŷ��㡣
			//�����ݵ�����е������������ֱ����Ҫ�������������ҲҪ������Сֱ����������װ��������
			//�ڶ��μ���,��¼����ֱ��,�Ա��ҳ����ֵ�����еĺ���ɽ���ĸԼ�������ֱ��	   
			if( sPartID == _T("LHG") )
			{
				//�·��������ɣ���Ҫѭ��3�β����ҳ��������ֱ��
				if( FirstCal >= 2 )
				{
					//�·�����ɽ���ĸֱ��=��������ֱ��
					modPHScal::PtypeDiameter[i] = modPHScal::PtypeDiameter[i + 1];
					if(modPHScal::gbByForce)
					{
						rsX.GetFieldValue(_T("Weight"),vTmp1);
						sTmp.Format(_T(" PmaxKgf>=%g "),
							(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ + vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
					}
					else
					{
						sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
					}
					rsX.GetFieldValue(_T("CustomID"),vTmp2);
					SQLx.Format( _T("SELECT * FROM [%s] WHERE %s  AND FiJ>=%d AND minDH<=%d AND maxDH>=%d AND trim(CustomID)=\'%s\' %s"),
						modPHScal::tbnLugDiaOfCSPR,
						sTmp,
						(modPHScal::PtypeDiameter[i] <= 0 ? 10 : modPHScal::PtypeDiameter[i]),
						modPHScal::sSprInfo[iSEQofSPR].DH,
						modPHScal::sSprInfo[iSEQofSPR].DH,
						vtos(vTmp2),
						(modPHScal::gbByForce ? _T(" ORDER BY Pmax,FiJ") : _T(" ORDER BY FiJ")));
				}
				else
				{
					if(modPHScal::gbByForce)
					{
						rsX.GetFieldValue(_T("Weight"),vTmp1);
						sTmp.Format(_T(" PmaxKgf>=%g "),
							(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ +vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
					}
					else
					{
						sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
					}
					rsX.GetFieldValue(_T("CustomID"),vTmp2);
					rsX.GetFieldValue(_T("Fjmin"),vTmp3);
					SQLx.Format( _T("SELECT * FROM [%s] WHERE %s  AND FiJ>=%d AND minDH<=%d AND maxDH>=%d AND trim(CustomID)=\'%s\' %s"),
						modPHScal::tbnLugDiaOfCSPR,
						sTmp,
						(vtoi(vTmp3)<=0 ? 10 : vtoi(vTmp3)),
						modPHScal::sSprInfo[iSEQofSPR].DH,
						modPHScal::sSprInfo[iSEQofSPR].DH,
						vtos(vTmp2),
						(modPHScal::gbByForce ? _T(" ORDER BY Pmax,FiJ") : _T(" ORDER BY FiJ")));
				}
			}
			else
			{
				//�����������ɣ�ѭ��2�β����ҳ��������ֱ��
				if( FirstCal >= 2 )
				{
					//���·�����ɽ���ĸֱ��=��ǰ������ֱ��
					modPHScal::PtypeDiameter[i] = modPHScal::PtypeDiameter[i - 1];
					if(modPHScal::gbByForce)
					{
						rsX.GetFieldValue(_T("Weight"),vTmp1);
						sTmp.Format(_T(" PmaxKgf>=%g "),
							(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ +vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
					}
					else
					{
						sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
					}
					rsX.GetFieldValue(_T("CustomID"),vTmp2);
					SQLx.Format( _T("SELECT * FROM [%s] WHERE %s AND FiJ>=%d AND minDH<=%d AND maxDH>=%d  AND trim(CustomID)=\'%s\' %s"),
						modPHScal::tbnLugDiaOfCSPR,					
						sTmp,
						(modPHScal::PtypeDiameter[i] <= 0 ? 10 : modPHScal::PtypeDiameter[i]),
						modPHScal::sSprInfo[iSEQofSPR].DH,
						modPHScal::sSprInfo[iSEQofSPR].DH,
						vtos(vTmp2),
						(modPHScal::gbByForce ? _T(" ORDER BY Pmax,FiJ") : _T(" ORDER BY FiJ")));
				}
				else
				{
					if(modPHScal::gbByForce)
					{
						rsX.GetFieldValue(_T("Weight"),vTmp1);
						sTmp.Format(_T(" PmaxKgf>=%g "),
							(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ +vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
					}
					else
					{
						sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
					}
					rsX.GetFieldValue(_T("CustomID"),vTmp2);
					rsX.GetFieldValue(_T("Fjmin"),vTmp3);
					SQLx.Format( _T("SELECT * FROM [%s] WHERE %s  AND FiJ>=%d AND minDH<=%d AND maxDH>=%d AND trim(CustomID)=\'%s\' %s"),
						modPHScal::tbnLugDiaOfCSPR,
						sTmp,
						(vtoi(vTmp3)<=0 ? 10 : vtoi(vTmp3)),
						modPHScal::sSprInfo[iSEQofSPR].DH,
						modPHScal::sSprInfo[iSEQofSPR].DH,
						vtos(vTmp2),
						(modPHScal::gbByForce ? _T(" ORDER BY Pmax,FiJ") : _T(" ORDER BY FiJ")));
				}
			}
			if(rsDiaOfCSPRFiJ.IsOpen())
				rsDiaOfCSPRFiJ.Close();
			rsDiaOfCSPRFiJ.Open(dbOpenSnapshot,SQLx);
			if( rsDiaOfCSPRFiJ.IsEOF() && rsDiaOfCSPRFiJ.IsBOF() )
			{
				sTmp.Format(GetResStr(IDS_NotFoundRecordInLugDiaOfCSPR),modPHScal::tbnLugDiaOfCSPR,SQLx);
				throw sTmp;
			}
			if( iSEQofSPR + 1 <= modPHScal::giWholeSprNum )
			{
				if( iSEQofSPR == 0 )
				{
					//��һ����������
					for(k = 0 ;k<= modPHScal::giCSPRindex[iSEQofSPR];k++)
					{
						//��һ�μ��㣬֮ǰ����������ֱ��Լ��ֵ=��ǰ������ɽ���ĸֱ��
						//�ڶ��μ��㣬��������ֱ��ѡ�������ɽ���ĸֱ������ʱ����ֱ����Ȼ�����˺���ɽ���ĸ����Сֱ����
						if( FirstCal == 1 )
						{ 
							rsDiaOfCSPRFiJ.GetFieldValue(_T("FiJ"),vTmp1);
							modPHScal::PtypeDiameter[k] = vtoi(vTmp1);
							if(modPHScal::gbRodDiaByMax)
							{
								if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[k])
									modPHScal::PtypeDiameter[k]=modPHScal::iMaxRodDia;
								else
									modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[k];
							}
						}
					}
				}
				else
				{
					//�ڶ������Ϻ�������
					//bool asc=true;
					//modPHScal::giCSPRindex[iSEQofSPR - 1] + 1 > modPHScal::giCSPRindex[iSEQofSPR] ? asc=false : asc=true;
					for(k = modPHScal::giCSPRindex[iSEQofSPR - 1] + 1 ;k<=modPHScal::giCSPRindex[iSEQofSPR] ;k++)
					{
						//��һ�μ��㣬֮ǰ����������ֱ��Լ��ֵ=��ǰ������ɽ���ĸֱ��
						//�ڶ��μ��㣬��������ֱ��ѡ�������ɽ���ĸֱ������ʱ����ֱ����Ȼ�����˺���ɽ���ĸ����Сֱ����
						if( FirstCal == 1 )
						{ 
							rsDiaOfCSPRFiJ.GetFieldValue(_T("FiJ"),vTmp1);
							modPHScal::PtypeDiameter[k] = vtoi(vTmp1);
							if(modPHScal::gbRodDiaByMax)
							{
								if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[k])
									modPHScal::PtypeDiameter[k]=modPHScal::iMaxRodDia;
								else
									modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[k];
							}
						}
					}
				}
			}
	}
	else if( modPHScal::glClassID == iSPR )
	{
		//���õ��ɱ�־
		bSPRphs = true;
		//����,��ʱ����䵥�����ɵ��������
		rsX.GetFieldValue(_T("dh"),vTmp1);
		iSumSerialNum +=vtoi(vTmp1) / 100;
		if( iSumSerialNum > modPHScal::giSumSerialNum )
		{
			sTmp.Format(GetResStr(IDS_SumSerialNumOverNeedSerialNum),_T("\%d"),_T("\%d"));
			sTmp2.Format(sTmp,iSumSerialNum,  modPHScal::giSumSerialNum);
			throw sTmp2;
		}
		else
		{
			//���ɴ�����
			modPHScal::sSprInfo[iSEQofSPR].SerialNum =vtoi(vTmp1) / 100;
			//modPHScal::sSprInfo[iSEQofSPR].SprNo = rsX.Fields(_T("dh")) Mod 100
			//modPHScal::sSprInfo[iSEQofSPR].DH = rsX.Fields(_T("dh"))
			modPHScal::sSprInfo[iSEQofSPR].haz = modPHScal::gsngTotalInstallCompressValue / modPHScal::giSumSerialNum * modPHScal::sSprInfo[iSEQofSPR].SerialNum;
			//modPHScal::sSprInfo[iSEQofSPR].HeatDisp = yr / giSumSerialNum * modPHScal::sSprInfo[iSEQofSPR].SerialNum
			rsX.GetFieldValue(_T("BH"),vTmp1);
			//���ɹ���
			modPHScal::sSprInfo[iSEQofSPR].BH=vtos(vTmp1);
			//��õ��������ɽ���ĸֱ��
			rsX.GetFieldValue(_T("size2"),vTmp1);
			//�ڶ��μ���,��¼����ֱ��,�Ա��ҳ����ֵ�����еĵ���/����ɽ���ĸԼ�������ֱ��	   
			if( sPartID == _T("T5") || sPartID == _T("T3A") )
			{
				//�·�����/�������ɣ���Ҫѭ��3�β����ҳ��������ֱ��
				//�·�/�ᵣ�����ɽ���ĸֱ��=��������ֱ������ֱ��
				if(vtoi(vTmp1)>=modPHScal::PtypeDiameter[i + 1] )
				{
					//��������ɽ���ĸֱ��>=����ֱ��
					if(modPHScal::gbAutoSPRRodDiaDecrease)
					{
						//�����ɽ���ĸֱ���Զ���С
						//modPHScal::giDiameter=modPHScal::PtypeDiameter[i + 1];
						modPHScal::PtypeDiameter[i] = modPHScal::PtypeDiameter[i + 1];
						modPHScal::sSprInfo[iSEQofSPR].FiJ = modPHScal::PtypeDiameter[i + 1];
					}
					else
					{
						//�����ɽ���ĸֱ�����Զ���С����
						//����ֱ��ȡ�ϴ���
						modPHScal::giDiameter=vtoi(vTmp1);
						if(modPHScal::gbRodDiaByMax)
						{
							if(modPHScal::iMaxRodDia>modPHScal::giDiameter)
							{
							}
							else
							{
								modPHScal::iMaxRodDia=modPHScal::giDiameter;
							}
							modPHScal::PtypeDiameter[i]=modPHScal::iMaxRodDia;
							modPHScal::PtypeDiameter[i+1]=modPHScal::iMaxRodDia;
							modPHScal::sSprInfo[iSEQofSPR].FiJ=modPHScal::iMaxRodDia;
						}
						else
						{
							modPHScal::PtypeDiameter[i] = modPHScal::giDiameter;
							modPHScal::PtypeDiameter[i + 1] = modPHScal::giDiameter;
							modPHScal::sSprInfo[iSEQofSPR].FiJ = modPHScal::giDiameter;
						}
					}
				}
				else
				{
					//��������ɽ���ĸ<����ֱ��
					//modPHScal::giDiameter=modPHScal::PtypeDiameter[i + 1];
					modPHScal::PtypeDiameter[i] = modPHScal::PtypeDiameter[i + 1];
					modPHScal::sSprInfo[iSEQofSPR].FiJ = modPHScal::PtypeDiameter[i + 1];
				}
			}
			else
			{
				//��������/�������ɣ�ѭ��2�β����ҳ��������ֱ��
				//���·�����/����ɽ���ĸֱ��=��ǰ������ֱ��
				if(vtoi(vTmp1)>=modPHScal::PtypeDiameter[i - 1] )
				{
					//��������ɽ���ĸֱ��>=����ֱ��
					if(modPHScal::gbAutoSPRRodDiaDecrease)
					{
						//�����ɽ���ĸֱ���Զ���С
						//modPHScal::giDiameter=modPHScal::PtypeDiameter[i - 1];
						modPHScal::PtypeDiameter[i] = modPHScal::PtypeDiameter[i - 1];
						modPHScal::sSprInfo[iSEQofSPR].FiJ = modPHScal::PtypeDiameter[i - 1];
					}
					else
					{
						//�����ɽ���ĸֱ�����Զ���С����
						//����ֱ��ȡ�ϴ���
						modPHScal::giDiameter=vtoi(vTmp1);
						if(modPHScal::gbRodDiaByMax)
						{
							if(modPHScal::iMaxRodDia>modPHScal::giDiameter)
							{
							}
							else
							{
								modPHScal::iMaxRodDia=modPHScal::giDiameter;
							}
							modPHScal::PtypeDiameter[i]=modPHScal::iMaxRodDia;
							modPHScal::PtypeDiameter[i-1]=modPHScal::iMaxRodDia;
							modPHScal::sSprInfo[iSEQofSPR].FiJ=modPHScal::iMaxRodDia;
						}
						else
						{
							modPHScal::PtypeDiameter[i] = modPHScal::giDiameter;
							modPHScal::PtypeDiameter[i - 1] = modPHScal::giDiameter;
							modPHScal::sSprInfo[iSEQofSPR].FiJ = modPHScal::giDiameter;
						}
					}
				}
				else
				{
					//��������ɽ���ĸ<����ֱ��
					//modPHScal::giDiameter=modPHScal::PtypeDiameter[i - 1];
					modPHScal::PtypeDiameter[i] = modPHScal::PtypeDiameter[i - 1];
					modPHScal::sSprInfo[iSEQofSPR].FiJ = modPHScal::PtypeDiameter[i - 1];
				}
			}
	   }
	}
	
	
	if( modPHScal::gbAddPartWeight2CSPRPgz )
	{
		//�ۼ������������������   //�ۼ����������ɹ�������
		//���Ը�������˨��ĸ���������Լ���������������1m���ǣ�
		rsX.GetFieldValue(_T("Weight"),vTmp1);
		//����������غ�
		tmpSelPJG4CSPRPgzNext = tmpSelPJG4CSPRPgz + vtof(vTmp1) * iNumPart;
	}
	if(modPHScal::gbAddPartWeight2SPRPgz) //pfg2007.08.16
	{
		//���ɹ������غ�
		tmpSelPJG4SPRPgzNext = tmpSelPJG4SPRPgz + vtof(vTmp1) * iNumPart;
		tmpSelPJG4SPRPazNext = tmpSelPJG4SPRPaz + vtof(vTmp1) * iNumPart;
	}
	
	if(  modPHScal::glIDIndex != iSA)
	{
		if( modPHScal::gbAddPartWeight2PMax)
		{
			//�ۼ��������������
			//���Ը�������˨��ĸ���������Լ���������������1m���ǣ�
			rsX.GetFieldValue(_T("Weight"),vTmp1);
			//����ɽ���ĸֱ�������غ�
			tmpSelPJG4CSPRFiK = tmpSelPJG4CSPRFiJ + vtof(vTmp1) * iNumPart;
			//������������غ�
			tmpSelPJG = tmpSelPJG + vtof(vTmp1) * iNumPart;
		}
		else
		{
			//����ɽ���ĸֱ�������غ�
			tmpSelPJG4CSPRFiK = tmpSelPJG4CSPRFiJ;
		}
	}
	//ע�����漸���λ�ã���Ҫ�ƶ����𴦣�������ؼ��㲻��ȷ
	if( modPHScal::glClassID == iCSPR )
	{
		//�����ɽ���ĸֱ��FiJ��FiK��S��I��T��
		//���ݵ�����е������p20 C�ͺ����TֵӦ�ø�ΪIֵ������ǰ���LHE��PHE(1<=DH<=83)�ĳߴ��Ǻϡ�
		//��̲ŷ��㡣�����в�δ�ᵽҪ�������ڼ���������˨ֱ��������ֵ���ƺ��Ѿ����������ء�
		//���Ҽ����غ���ʱ�Ҳ�����FiJ��Ӧ��FiK���ʴ�������ز������ء�
		//_T("WHERE ") + IIf(gbByForce, _T(" PmaxKgf>=") + IIf(sPartID = _T("LHG"), tmpSelPJG4CSPRFiJ + IIf(IsNull(rsX.Fields(_T("Weight"))), 0, rsX.Fields(_T("Weight"))) * gintParallelNum, tmpSelPJG4CSPRFiJ) / gintParallelNum, _T(" fBmin<=") + modPHScal::sSprInfo[iSEQofSPR].HeatDisp + _T(" AND fBmax>=") + modPHScal::sSprInfo[iSEQofSPR].HeatDisp) + _T(" AND FiJ>=") + IIf(modPHScal::PtypeDiameter[i] <= 0, 10, modPHScal::PtypeDiameter[i]) + _T(" AND minDH<=") + modPHScal::sSprInfo[iSEQofSPR].DH + _T(" AND maxDH>=") + modPHScal::sSprInfo[iSEQofSPR].DH + _T(" AND trim(CustomID)=//") + Trim(rsX.Fields(_T("CustomID"))) + _T("// ") + IIf(gbByForce, _T(" ORDER BY Pmax,FiJ"), _T(""))
		if(modPHScal::gbByForce)
		{
			rsX.GetFieldValue(_T("Weight"),vTmp1);
			sTmp.Format(_T(" PmaxKgf>=%g "),
				(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ +vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
		}
		else
		{
			sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
				modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
				modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
		}
		rsX.GetFieldValue(_T("CustomID"),vTmp2);
		SQLx.Format( _T("SELECT * FROM [%s] WHERE %s  AND FiJ>=%d AND minDH<=%d AND maxDH>=%d  AND trim(CustomID)=\'%s\' %s"),
			modPHScal::tbnLugDiaOfCSPR,
			sTmp,
			(modPHScal::PtypeDiameter[i] <= 0 ? 10 : modPHScal::PtypeDiameter[i]),
			modPHScal::sSprInfo[iSEQofSPR].DH,
			modPHScal::sSprInfo[iSEQofSPR].DH,
			vtos(vTmp2),
			(modPHScal::gbByForce ? _T(" ORDER BY Pmax,FiJ") : _T(" ORDER BY FiJ")));
		
		if(rsDiaOfCSPRFiK.IsOpen())
			rsDiaOfCSPRFiK.Close();
		rsDiaOfCSPRFiK.Open(dbOpenSnapshot,SQLx);
		if( rsDiaOfCSPRFiK.IsEOF() && rsDiaOfCSPRFiK.IsBOF() )
		{
			sTmp.Format(GetResStr(IDS_NotFoundRecordInLugDiaOfCSPR), modPHScal::tbnLugDiaOfCSPR, SQLx);
			throw sTmp;
		}
		rsDiaOfCSPRFiJ.GetFieldValue(_T("FiJ"),vTmp1);
		modPHScal::sSprInfo[iSEQofSPR].FiJ = vtof(vTmp1);
		rsDiaOfCSPRFiK.GetFieldValue(_T("FiK"),vTmp1);
		modPHScal::sSprInfo[iSEQofSPR].FiK = vtof(vTmp1);
	}
	rsX.GetFieldValue(_T("CustomID"),vTmp1);
	
	if(rsID.FindFirst(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")))
	{
		rsID.GetFieldValue(_T("BHFormat"),vTmp1);
		sBHFormat = vtos(vTmp1);
	}
	if( sBHFormat.GetLength()<=0 )
	{
		sBHFormat = _T("trim(BH)");
	}
	rsX.GetFieldValue(_T("BH"),vTmp1);
	sBH=vtos(vTmp1);
	
	if( tmpSQL0 != _T("") )
	{
		//�������ӱ��ʽ
		//����ȥ�����е�Cnte.�ַ�
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
	
	
	//�����������˳���
	//����������ݵ�����rsTZB(�ṹ���ݱ�JZB)��
	//������mvarRsTZC(�������ݱ�JZC)��
	//	rsTZB.AddNew();
	int iTmpRecno;
	rsTZB.GetFieldValue(_T("recno"),vTmp);
	iTmpRecno=vtoi(vTmp);
	dbstr = dbstr +"iNnerRecno = " + ltos(iTmpRecno);
	rsTZB.GetFieldValue(_T("ID"),vTmp);
	dbstr=dbstr+"  ID= "+vtos(vTmp);
	//rsTZB.FindFirst("recno="+ltos(iTmpRecno));
	rsTZB.Edit();
	rsTZB.SetFieldValue(_T("nth"),COleVariant((short)nth));//��׼֧����·��=1�������Գ�˫���͹��ø���˫��

	dbstr = dbstr + "  FirstCal: "+ltos(FirstCal) + " nth: " + ltos(nth);
	
	//��ǰ���������һ�����(L8)�ĵĳ��ȳߴ��ֶ�
	rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp1);
	if( vTmp1.vt==VT_NULL || vtos(vTmp1) == _T("") )
	{
		if( sPartID==_T("L7") || sPartID==_T("G12") || sPartID==_T("L7Dd") || sPartID==_T("G12Lc") )
		{
			//�����ֶ��ӿ�������L8����,���Ҫ���⴦�����ܲ鵽��ȷ�Ĳ���L8�ĳߴ硣
			if( PtypeID[i - 1] == _T("L8") || PtypeID[i - 1] == _T("L8Dd") )
			{
				//ǰһ����L8
				//�Ȳ�����
				//�Ժ��ڼ������˳��Ⱥ���CalLugLength��ֻҪ������������������ɡ�
				rsX.GetFieldValue(tmpExtLenFD,vTmp2);
				rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
			}
			////////////////////////////////////////////
			if( PtypeID[i + 1] == _T("L8") || PtypeID[i + 1] == _T("L8Dd"))
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
		if(sPartID.Left(2) == _T("L7")||sPartID.Left(2) == _T("G12")||sPartID == _T("L3")||sPartID == _T("L4")||sPartID == _T("T1")||sPartID == _T("D4")||sPartID == _T("D5")||sPartID == _T("D11"))
		{
			rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp2);
			tmpExtLenFD = vtos(vTmp2);
			
			//��Ϊ�ܲ���������ǰһ��������ֻ��Ҫ�ж����ĺ�һ��
			if(sPartID != _T("D4") && sPartID != _T("D5") && sPartID != _T("D11"))
			{
				//�����ֶ��ӿ�������L8����,���Ҫ���⴦�����ܲ鵽��ȷ�Ĳ���L8�ĳߴ硣
				if( PtypeID[i - 1] == _T("L8") || PtypeID[i - 1] == _T("L8Dd") )
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
						if( PtypeID[i - 1] == _T("PHE1") || PtypeID[i - 1] == _T("PHE") || PtypeID[i - 1] == _T("LHE") || PtypeID[i - 1] == _T("LHE1") )
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
						rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)tmpExtLenValue));
					}
				}
			}
			//////////////////////////////////////////////////////////////////////
			if( PtypeID[i + 1] == _T("L8") || PtypeID[i + 1] == _T("L8Dd") )
			{
				rsX.GetFieldValue(tmpExtLenFD,vTmp2);
				rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
			}
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			if( sPartID == _T("L8") || sPartID == _T("L8Dd") )
			{
				//��ǰ����L7��G12,��L8
				if( PtypeID[i + 1] == _T("L7") || PtypeID[i + 1] == _T("G12") || PtypeID[i + 1] == _T("L7Dd") || PtypeID[i + 1] == _T("G12Lc") )
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
					rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)tmpExtLenValue));
				}
				///////////////////////////////////////////////////////
				//			 if( PtypeID[i - 1] == _T("L7") || PtypeID[i - 1] == _T("L7Dd"))
				//			 {
				//				 rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp1);
				//				 tmpExtLenFD = vtos(vTmp1);
				//			 }
				//			 else
				//			 {
				//				 rsConnect.GetFieldValue(_T("CNTBExtLenFLD"),vTmp1);
				//			 	 tmpExtLenFD = vtos(vTmp1);
				//				 rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)tmpExtLenValue));
				//			 }
				////////////////////////////////////////////////////////////
			}
			else
			{
				//��ǰ����L7��G12,����L8
				if( sPartID == _T("L5") || sPartID == _T("L6") )
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
						if( PtypeID[i-1] == _T("PHE1") || PtypeID[i-1] == _T("PHE") || PtypeID[i-1] == _T("LHE") || PtypeID[i-1] == _T("LHE1") )
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
						rsTZB.SetFieldValue(_T("PreviousPartInsertLen"),COleVariant((long)tmpExtLenValue));
					}
				}
			}
		}
	}
	
	
	if(i==0)
	{
		//˫���ܲ������˼��
		if( modPHScal::gintParallelNum == 2 )
		{
			if( modPHScal::glClassID == iPAh )
			{
				//˫��
				rsX.GetFieldValue(_T("sizeC"),vTmp1);
				modPHScal::DblHangerRodDist =vtof(vTmp1);
				rsTZB.SetFieldValue(_T("sizeC"),COleVariant((float)modPHScal::DblHangerRodDist));
			}
			else
			{
				if( modPHScal::glClassID == iPAs )
				{
					//˫֧
					rsX.GetFieldValue(_T("size3"),vTmp1);
					modPHScal::DblHangerRodDist =vtof(vTmp1);
					rsTZB.SetFieldValue(_T("sizeC"),COleVariant((float)modPHScal::DblHangerRodDist));
				}
				else
				{
					if( modPHScal::glClassID == iPAD11LD || modPHScal::glClassID == iPADLR || modPHScal::glPAid == iPALX )
					{
						//��ܾ��йص�˫�����
						rsX.GetFieldValue(_T("sizeC"),vTmp1);
						modPHScal::DblHangerRodDist =vtof(vTmp1) + modPHScal::dj;
						rsTZB.SetFieldValue(_T("sizeC"),COleVariant((float)modPHScal::DblHangerRodDist));
					}
				}
			}
		}
	}
	
	   //���ں���򵯻ɲ�����SQL�����ķ�����ñ�ţ���ȥ�����е����Ӳ�������
	   EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET BH=\'") + sBH + _T("\'"));
	   rsTZB.SetFieldValue(_T("BH"),STR_VAR(sBH));
	   //���ںᵣ����T5��ᵣ���LHG��������ݹܲ�D13/D14�ҳ������ľࡣ
	   //�����·�����T3A��ᵣ����T5��ᵣ���LHG�������Ȳ�������ֱ����
	   //������˨��ĸ�Ĺ��ͨ��GetBoltsNutsAndAttachmentsCLgg��������
	   sTmp.Format(_T("%d"),modPHScal::PtypeDiameter[i]);
	   EDIBgbl::dbPRJ.Execute (_T("UPDATE tmpCSLen SET LugDiameter=") + sTmp);
	   if( modPHScal::glClassID == iCSPR )
	   {
		   //ȷ���������
		   if( modPHScal::gbAutoAlignCSPRtoSA && sPartID !=_T("LHG") )
		   {
			   //������Զ���������
			   mlfx = modPHScal::GetPhsSAfx(modPHScal::fx + modPHScal::gf);
			   mlfx0 = 0;
		   }
		   
		   //�غ�������λΪkN*mm,��N*m,λ�Ƶ�λΪmm,���ص�λΪkgf(��daN=10N)
		   //tmpCapacity = TmpPgz * Tmpyr *EDIBgbl::kgf2N / 10 / iSumSerialNum
		   //SQLx = _T("SELECT * FROM [") + tbnHDproperty + _T("] WHERE Capacity>=") + tmpCapacity + _T(" AND MaxDist>=") + Tmpyr / iSumSerialNum + _T(" ORDER BY dh,Capacity,MaxDist")
		   //rs.Open(dbOpenSnapshot,SQLx);
		   //if( rs.IsEOF() And rs.IsBOF() ){
		   sTmp.Format(_T("%d"),modPHScal::PtypeDiameter[i]);
		   EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET LugDiameter=") + sTmp);
		   //EDIBgbl::dbPRJ.Execute _T("UPDATE tmpCSLen SET SumDist=") + IIf(tmpDist > IIf(IsNull(rsX(_T("fBmax"))), 0, rsX(_T("fBmax"))), IIf(IsNull(rsX(_T("fBmax"))), 0, rsX(_T("fBmax"))), tmpDist)
		   sTmp.Format(_T("%g"),tmpDist);
		   EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET SumDist=") +sTmp);
		   sTmp.Format(_T("%d"),tmpDistID);
		   EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET SumDistID=") +sTmp);//���ڻ�糧��ˮ�ܵ�֧����2007�������������ɣ�����λ�ƴ���
		   rsX.GetFieldValue(_T("sizeC"),vTmp1);
		   sTmp.Format(_T("%g"),(sPartID == _T("LHG") ? modPHScal::DblHangerRodDist : vtof(vTmp1)));
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET sizeC=") + sTmp);
		   //�������ľ࣬����T5/LHG�������ֱ�����ٴ����ɹ��CLgg
		   modPHScal::sSprInfo[iSEQofSPR].sizeC=_ttoi(sTmp);
		   sTmp.Format(_T("%g"),modPHScal::sSprInfo[iSEQofSPR].HeatDisp);
		   EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET HeatDist=") + sTmp);
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET UnitForce=\'") +modPHScal::gsCSPRUnitOfLoad + _T("\'"));
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET DistFX=\'") + (modPHScal::yr > 0 ? modPHScal::gSprDirectionChar.CSPR_UP : modPHScal::gSprDirectionChar.CSPR_Down) + _T("\'"));
		   if(modPHScal::gsCSPRUnitOfLoad == _T("daN") )
		   {
			   //SelOpLoad=ѡ��Ĺ�������,����λ��ԣ��֮����õ��غ�ֵtmpSelOpLoad
			   sTmp.Format(_T("%d"),(long)(tmpSelOpLoad * EDIBgbl::kgf2N / 10));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET SelOpLoad=") + sTmp);  //���ݵ�����е������ITT��������λΪdaN����������ΪN��
			   
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum * EDIBgbl::kgf2N / 10 ));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET OpLoad=") + sTmp); //���ݵ�����е������ITT��������λΪdaN����������ΪN��
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum * EDIBgbl::kgf2N / 10));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET InstallLoad=") + sTmp); //���ݵ�����е������ITT��������λΪdaN����������ΪN��
		   }
		   else if(modPHScal::gsCSPRUnitOfLoad == _T("N") )
		   {
			   //SelOpLoad=ѡ��Ĺ�������,����λ��ԣ��֮����õ��غ�ֵtmpSelOpLoad
			   sTmp.Format(_T("%d"),(long)(tmpSelOpLoad * EDIBgbl::kgf2N));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET SelOpLoad=") +sTmp);
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")? tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum * EDIBgbl::kgf2N));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET OpLoad=") + sTmp);
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum * EDIBgbl::kgf2N));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET InstallLoad=") + sTmp);
		   }
		   else
		   {
			   //SelOpLoad=ѡ��Ĺ�������,����λ��ԣ��֮����õ��غ�ֵtmpSelOpLoad
			   sTmp.Format(_T("%d"),(long)(tmpSelOpLoad));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET SelOpLoad=") +sTmp);
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz )/modPHScal::gintParallelNum));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET OpLoad=") + sTmp);
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET InstallLoad=") + sTmp);
		   }
	   }
	   else if(modPHScal::glClassID == iSPR )
	   {
		   //���ɱ���к����ð�װ���ر�ʾ 
		   if(modPHScal::gbSPRBHLOADBYInstallLoad)
		   {
			   sBHFormat.Replace("OpLoad","InstallLoad");
		   }
		   
		   sTmp.Format(_T("%d"),modPHScal::sSprInfo[iSEQofSPR].SerialNum);
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET SerialNum=") + sTmp);
		   sTmp.Format(_T("%d"),modPHScal::sSprInfo[iSEQofSPR].SprNo);
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET SprNo=") + sTmp);
		   sTmp.Format(_T("%d"),modPHScal::gSumDistOfSpring);
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET SumDist=") +sTmp);
		   sTmp.Format(_T("%d"),modPHScal::sSprInfo[iSEQofSPR].SumDistID);
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET SumDistID=") +sTmp);
		   if( (PtypeID[i-1] == _T("D13") || PtypeID[i-1] == _T("D14")) && (sPartID == _T("T5") || sPartID == _T("TDG") || sPartID == _T("LHG")) )
		   {
			   //�·����ɻ��·��������ɵ����ľ��ǹܲ����ľ�C�����ģ�
			   //���CֵҪд�뵯�ɹ���У���Ϊ��������
			   sTmp.Format(_T("%g"),modPHScal::DblHangerRodDist);
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET sizeC=") + sTmp);
			   //�������ľ࣬����T5/LHG�������ֱ�����ٴ����ɹ��CLgg
			   modPHScal::sSprInfo[iSEQofSPR].sizeC=_ttoi(sTmp);
		   }		  
		   sTmp.Format(_T("%g"),modPHScal::sSprInfo[iSEQofSPR].HeatDisp);
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET HeatDist=") + sTmp);
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET UnitForce=\'") +modPHScal::gsSPRUnitOfLoad + _T("\'"));
		   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET DistFX=\'") + (modPHScal::yr > 0 ? modPHScal::gSprDirectionChar.SPR_UP : modPHScal::gSprDirectionChar.SPR_Down) + _T("\'"));
		   if(modPHScal::gsSPRUnitOfLoad == _T("daN") )
		   {
			   //���صĵ�λ��daN(10N)
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPgz) / iNumPart * EDIBgbl::kgf2N / 10));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET OpLoad=") + sTmp); //���ݵ�����е������ITT��������λΪdaN����������ΪN��
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPaz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPaz) / iNumPart * EDIBgbl::kgf2N / 10));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET InstallLoad=") + sTmp); //���ݵ�����е������ITT��������λΪdaN����������ΪN��
		   }
		   else if(modPHScal::gsSPRUnitOfLoad == _T("N") )
		   {
			   //���صĵ�λ��N
			   //���������������룬������ֺ�����ֵ��һ�µ����,��bug���ȱ�����Ժ���㷢��
			   //(����,pgz=9000N,yr=14mm,�����䲻��0.5�����"T2.110,14����/8999,M20")
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPgz) / iNumPart * EDIBgbl::kgf2N+0.5));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET OpLoad=") + sTmp);
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPaz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPaz) / iNumPart * EDIBgbl::kgf2N+0.5));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET InstallLoad=") + sTmp);
		   }
		   else
		   {
			   //���صĵ�λ��kgf
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPgz) / iNumPart));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET OpLoad=") + sTmp);
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPaz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPaz) / iNumPart));
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET InstallLoad=") + sTmp);
		   }
	   }	   
	   if(modPHScal::glIDIndex != iSA )
	   {
		   if(modPHScal::glIDIndex == iPA )
		   {
			   //�¶ȿ�����Ϊ��־�ܲ�����Ϣ
			   sTmp.Format(_T("%g"),modPHScal::t0);
			   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET t0=") + sTmp);
			   rsX.GetFieldValue(_T("Material"),vTmp1);
			   rsTZB.SetFieldValue(_T("CLcl"),vTmp1);
			   if(modPHScal::glClassID==iPAfixZ2)
			   {
				   //�̶�֧��Z2A/Z2/SZ5�ܲ���������߶�k1(size4),k2(size5)--mm
				   rsX.GetFieldValue(_T("size4"),vTmp1);				 
				   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET K=") + vtos(vTmp1));
				   rsX.GetFieldValue(_T("size5"),vTmp1);				 
				   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET K2=") + vtos(vTmp1));
				   //�̶�֧��Z2A/Z2/SZ5�ܲ�֧�ܹܾ�DW1xS(sizeC x size3),�����⾶DW--mm,����Ժ�ֲ���Ҫ��������ǡ�
				   rsX.GetFieldValue(_T("DW"),vTmp1);				 
				   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET DW=") + vtos(vTmp1));
				   rsX.GetFieldValue(_T("sizeC"),vTmp1);				 
				   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET dw1=") + vtos(vTmp1));
				   rsX.GetFieldValue(_T("size3"),vTmp1);				 
				   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET S=") + vtos(vTmp1));
				   //SZ5�������Ҫ�¶�t0/�ܾ�dj			 
				   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET temperature=") + ftos(modPHScal::t0));
			   }
		   }
		   
		   SQLx = _T("SELECT (") + sBHFormat + _T(") AS sBHFormat  FROM tmpCSLen");
		   if(rsTmp.IsOpen())
			   rsTmp.Close();
		   rsTmp.m_pDatabase=&EDIBgbl::dbPRJ;
		   rsTmp.Open(dbOpenDynaset,SQLx);
		   rsTmp.GetFieldValue(_T("sBHFormat"),vTmp1);
		   sBHFormat =vtos(vTmp1);
		   rsTZB.SetFieldValue(_T("BHformat"),STR_VAR(sBHFormat));
		   if(modPHScal::glClassID == iSPR || modPHScal::glClassID == iCSPR )
		   {
			   rsTZB.SetFieldValue(_T("CLgg"),STR_VAR(sBHFormat));
			   //���浯��/����������Ϣ����T5/LHG�������ɱ�ţ���������ɶ������
			   modPHScal::sSprInfo[iSEQofSPR].CustomID=Ptype[i];
			   modPHScal::sSprInfo[iSEQofSPR].BHFormat=sBHFormat;
			   modPHScal::sSprInfo[iSEQofSPR].FiJ=modPHScal::PtypeDiameter[i];
		   }
		   else if(modPHScal::glClassID == iROD || modPHScal::glClassID == iXL1 )
			   //���˵Ĺ����Ҫ�ڼ��㳤�Ⱥ�ȷ�����˴����븳��CLgg�ֶ���ȷ��BH��
			   //��CalLugLength��������Ҫ��ȷ�ĸ���BHֵ��CLggֵ��
		   {
			   rsTZB.SetFieldValue(_T("CLgg"),STR_VAR(sBH));
		   }
		   else if( modPHScal::glIDIndex ==iPA && modPHScal::g_bPipeClampBoltDiameterChanged)
			   //С���ش�ܾ��ܼа�����ѡ����˨��ֱ��M1��С���ܼй���б���ע����M1ֵ
		   {
			   //ȡ���ܼ���˨M1��ֵ
			   rsX.GetFieldValue(strDiaM1FieldName,vTmp1);
			   m_fDiaM1 = vtof(vTmp1);
			   //�ܼ���˨M1��U�ζ���L8����˨M��������
			   CString strSQLTmp ;
				strSQLTmp.Format("SELECT * FROM [%s] WHERE [CUSTOMID]=\'%s\' AND [SIZE2]=%d ORDER BY SIZE2",
					modPHScal::tbnPART,modPHScal::sFindCustomID (_T("L8")),int(modPHScal::gmiDiameter));			   
			   CDaoRecordset m_rs;
			   COleVariant vT1;
			   if(m_rs.IsOpen())
				   m_rs.Close();
			   
			   m_rs.m_pDatabase=&modPHScal::dbZDJcrude;
			   m_rs.Open(dbOpenSnapshot,strSQLTmp);
			   if((!m_rs.IsBOF()) && !(m_rs.IsEOF()))
			   {
				   m_rs.GetFieldValue(_T("size3"),vT1);
				   int x = vtoi(vT1);
				   //����ܼ���˨��M1��ֵ����Ҫ��L8����˨M��			   
				   if(m_fDiaM1 > x)
				   {
						strSQLTmp.Format("SELECT * FROM [%s] WHERE [CUSTOMID]=\'%s\' AND [SIZE2]=%d ORDER BY SIZE2",
							modPHScal::tbnPART,modPHScal::sFindCustomID (_T("L7")),int(modPHScal::gmiDiameter));
						m_rs.Close();
						m_rs.Open(dbOpenSnapshot,strSQLTmp);
						m_rs.GetFieldValue(_T("size3"),vT1);
						int y = vtoi(vT1);
						if(m_fDiaM1 > y)
						{
							//ͬʱҲ��L7���ڻ�ֱ��D��,
							//�򽫹ܼ���˨��С
							strSQLTmp.Format(", M1=%d ",x);
							sBH = sBH + strSQLTmp;
						}
						else
							modPHScal::g_bPipeClampBoltDiameterChanged = FALSE;
				   }
				   else
				   {
					   modPHScal::g_bPipeClampBoltDiameterChanged = FALSE;
				   }
			   }
			   m_rs.Close ();
			   strPipeClampBoltDiameterChanged = PtypeID[i];//����IDֵ
			   rsTZB.SetFieldValue(_T("CLgg"),STR_VAR(sBH));
			   
		   }
		   else
		   {
			   rsTZB.SetFieldValue(_T("CLgg"),STR_VAR(sBHFormat));
		   }
	   }
	}
	rsTZB.SetFieldValue(_T("recno"),COleVariant((long)iSEQofPart));
	dbstr = dbstr + " recno= " + ltos(iSEQofPart);
	rsTZB.SetFieldValue(_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
	dbstr = dbstr + " VolumeID= " + ltos(EDIBgbl::SelVlmID);
	rsTZB.SetFieldValue(_T("zdjh"),COleVariant((long)modPHScal::zdjh));
	dbstr = dbstr + " zdjh= " + ltos(modPHScal::zdjh);
	dbstr = dbstr + "\n";
//	AfxMessageBox(dbstr);
	rsTZB.SetFieldValue(_T("CustomID"),STR_VAR(Ptype[i]));
	rsTZB.SetFieldValue(_T("ClassID"),COleVariant((long)modPHScal::glClassID));
	//�������д����������У�˼���
	rsTZB.SetFieldValue(_T("Pjs"),COleVariant((float)tmpSelPJG));
	//�������ͼ��һ·����
	rsTZB.SetFieldValue(_T("fx"),COleVariant((long)mlfx));
	//���y���귽��
	rsTZB.SetFieldValue(_T("Rotation"),COleVariant((long)mlRot));
	if( sPartID == _T("LS") || sPartID == _T("LT") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)-(vtof(vTmp1) + modPHScal::dwB / 2)));
		//���ʵ�ʳߴ�
		rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)-(vtof(vTmp1) + modPHScal::dwB / 2)));
	}
	else if( sPartID == _T("D4LQ") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)(vtof(vTmp1) + modPHScal::dwB / 2)));
		//���ʵ�ʳߴ�
		rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)(vtof(vTmp1) + modPHScal::dwB / 2)));
	}
	else if( sPartID == _T("DLR") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		rsX.GetFieldValue(_T("size4"),vTmp2);
		rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)(vtof(vTmp1)+ modPHScal::dwB / 2 - vtof(vTmp2))));
		//���ʵ�ʳߴ�
		rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)(long)(vtof(vTmp1)+ modPHScal::dwB / 2 - vtof(vTmp2))));
	}
	else if( sPartID == _T("LM2") || sPartID == _T("LM3") || sPartID == _T("LM4") )
	{
		//ˮƽԲ�ιܵ��������ܣ��߶�ֵҪ�Ӳ۸ֺᵣ�߶�
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		//rsTZB.SetFieldValue(_T("sizeDIM")) = -(IIf(IsNull(rsX.Fields(_T("size5"))), 0, rsX.Fields(_T("size5"))) + rsX.Fields(_T("size7")) * 10)
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)vtof(vTmp1)));
		//���ʵ�ʳߴ�
		rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)vtoi(vTmp1)));
	}
	else if( sPartID == _T("LN2") || sPartID == _T("LN3") )
	{
		//ˮƽԲ�ιܵ��۸ֺ����������ܣ��߶�ֵҪ��
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX.GetFieldValue(_T("sizeH"),vTmp1);
		rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)vtof(vTmp1)));
		//���ʵ�ʳߴ�
		rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)vtoi(vTmp1)));
	}
	else if(sPartID==_T("L15"))
	{
		//����Ժ�̶�֧��Z2/Z2A�µ�һ��ְ�L15,����ֵ������tbnAttachement���ֶ�size2�л�modPHScal::gfPSThickness
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		if(PtypeID[i+1] == _T("GZD"))
		{
			//������֧�գ��ְ���������
			rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)0));
			//���ʵ�ʳߴ�
			rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)0));
		}
		else
		{
			rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)modPHScal::gfPSThickness));
			//���ʵ�ʳߴ�
			rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)modPHScal::gfPSThickness));
		}
	}
	else
	{
		if(modPHScal::glIDIndex==iSA)
		{
			//�����߶���Ϊ0,�Ա���ȷͳ�����˳���
			rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)0));
			//���ʵ�ʳߴ�
			rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)0));
		}
		else
		{
			//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
			rsX.GetFieldValue(_T("sizeH"),vTmp1);
			rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((long)vtof(vTmp1)));
			//���ʵ�ʳߴ�
			rsTZB.SetFieldValue(_T("sizeH"),COleVariant((double)vtoi(vTmp1)));
		}
	}
	
	rsTZB.SetFieldValue(_T("CLnum"),COleVariant((long)iNumPart));
	rsTZB.SetFieldValue(_T("SEQ"),COleVariant((long)iSEQofPart));
	rsTZB.SetFieldValue(_T("CLbz"),vnil);
	
	if(rsID.FindFirst(_T("trim(ID)=\'") + sPartID + _T("\'")))
	{
		rsID.GetFieldValue(_T("Description"),vTmp1);
		rsTZB.SetFieldValue(_T("CLmc"),vTmp1);
	}
	//rsTZB.SetFieldValue(_T("CLmc")) = sFindFLD(_T("ID"), _T("Description"), sPartID)
	rsX.GetFieldValue(_T("Weight"),vTmp1);
	if( sPartID == _T("LHG") )
		modPHScal::g_fLHGWeight = vtof(vTmp1);
	if( sPartID == _T("T5") )//T5��LHG����ͬʱ���֣���Ϊ���ɺͺ������ɲ���ͬʱ����
		modPHScal::g_fLHGWeight = vtof(vTmp1);
	rsTZB.SetFieldValue(_T("CLdz"),COleVariant(vtof(vTmp1)));
	rsTZB.GetFieldValue(_T("CLnum"),vTmp2);
	rsTZB.SetFieldValue(_T("CLzz"),COleVariant(vtof(vTmp1)* vtof(vTmp2)));
	
	if(modPHScal::glIDIndex != iSA )
	{
		if(i==0)
		{
			//������Ǹ���
			if(modPHScal::gintParallelNum == 2 )
			{
				if(modPHScal::glClassID == iPAh )
				{
					//˫��
					rsX.GetFieldValue(_T("sizeC"),vTmp1);
					modPHScal::DblHangerRodDist =vtof(vTmp1);
					rsTZB.SetFieldValue(_T("sizeC"),COleVariant(modPHScal::DblHangerRodDist));
				}
				else
				{
					if(modPHScal::glClassID == iPAs )
					{
						//˫֧
						rsX.GetFieldValue(_T("size3"),vTmp1);
						modPHScal::DblHangerRodDist = vtof(vTmp1);
						rsTZB.SetFieldValue(_T("sizeC"),COleVariant(modPHScal::DblHangerRodDist));
					}
					else
					{
						if(modPHScal::glClassID == iPAD11LD ||modPHScal::glClassID == iPADLR || modPHScal::glClassID == iPALX )
						{
							//��ܾ��йص�˫�����
							rsX.GetFieldValue(_T("sizeC"),vTmp1);
							modPHScal::DblHangerRodDist = vtof(vTmp1) + modPHScal::dj;
							rsTZB.SetFieldValue(_T("sizeC"),COleVariant(modPHScal::DblHangerRodDist));
						}
						else
						{
							//����������������ľ࣬��T5,LHG��
							rsX.GetFieldValue(_T("sizeC"),vTmp1);
							rsTZB.SetFieldValue(_T("sizeC"),COleVariant(vtof(vTmp1)));
						}
					}
				}
			}
		}
	}
	else
	{
		//����Ǹ���������˫�۸ֵļ�ࡣ
		if(modPHScal::glClassID==iSALbraceFixG48)
		{
			//G48�����ӿ�ȱ�����ԭʼ���ݿ��У���=˫�۸ֵļ��
			vTmp1=rsza->GetCollect(_T("A01"));
			rsTZB.SetFieldValue(_T("C"),vtof(vTmp1));
		}
		else if(modPHScal::glClassID==iG56 || modPHScal::glClassID==iG57)
		{
			//G56,57�����ӿ�ȱ�����ԭʼ���ݿ��У���=˫�۸ֵļ��
			vTmp1=rsza->GetCollect(_T("A01"));
			COleVariant vTmp2;
			//������۸ֵļ�϶ֵm,һ��=5
			rsX.GetFieldValue(_T("m"),vTmp2);
			rsTZB.SetFieldValue(_T("C"),vtof(vTmp1)+2*vtof(vTmp2));
		}
		else
		{
			rsX.GetFieldValue(_T("CHDIST"),vTmp1);
			rsTZB.SetFieldValue(_T("C"),vtof(vTmp1));
		}
		
		//�¶δ������׳�����������ȷ�Ŀ��ֵ�ĳɸ����ֵ������ע�͵������û����ƿ��ֵ��׼ȷ�ԡ�
		if ( (modPHScal::gintParallelNum==2) && (modPHScal::glNumSA==1) /*&& modPHScal::bPAIsHanger()*/)
		{
			//˫������������У�˸������
			if( (modPHScal::glClassID==iSAbeam) || (modPHScal::glClassID==iG51) || (modPHScal::glClassID==iG52_55) || (modPHScal::glClassID==iG100) || (modPHScal::glClassID==iSACantilever) && sPartID!=_T("G23") && sPartID!=_T("G24"))
			{
				//ֻ����֧��
				if( modPHScal::DblHangerRodDist > 0 && modPHScal::Lspan <modPHScal::gdw + modPHScal::DblHangerRodDist + 100 )
				{
					modPHScal::Lspan =modPHScal::gdw + modPHScal::DblHangerRodDist + 100;
					rsza->PutCollect(_T("LC1"),_variant_t((float)modPHScal::Lspan));
				}
			}
		}
	}
	//���������ţ��Ա��ڻ��ܲ���ʱ�ܹ�Ѹ��ʶ��������������
	rsTZB.SetFieldValue(_T("Index"),COleVariant((long)modPHScal::glIDIndex));
	rsTmpZB.GetFieldValue(_T("CustomID"),vTmp1);
	
	if( rsID.FindFirst (_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")) )
	{
		rsID.GetFieldValue(_T("ID"),vTmp1);
		rsTZB.SetFieldValue(_T("ID"),vTmp1);
	}
	   CString strBH,strID;
	   CDaoRecordset rsTmp2;
	   switch(modPHScal::glClassID)
	   {
	   case iConnected:
		   //�����˵����Ӽ�
		   if( sPartID.Left(2)== _T("L5") ||sPartID.Left(2)== _T("L6") ||sPartID.Left(2)== _T("L7") ||sPartID.Left(2)== _T("L8"))
		   {
			   rsX.GetFieldValue(_T("size4"),vTmp1);
			   rsTZB.SetFieldValue(_T("AllowedMinLugExt"),vtof(vTmp1));
			   rsX.GetFieldValue(_T("size5"),vTmp1);
			   rsTZB.SetFieldValue(_T("AllowedMaxLugExt"),vtof(vTmp1));
			   //�ڶ��μ���,��¼����ֱ��,�Ա��ҳ����ֵ�����еĵ���/����ɽ���ĸԼ�������ֱ��
			   if( FirstCal == 1 )
			   {
				   rsX.GetFieldValue(_T("size2"),vTmp1);
				   modPHScal::PtypeDiameter[i] =vtoi(vTmp1);
				   if(modPHScal::gbRodDiaByMax)
				   {
					   if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[i])
						   modPHScal::PtypeDiameter[i]=modPHScal::iMaxRodDia;
					   else
						   modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[i];
					   modPHScal::gmiDiameter=modPHScal::iMaxRodDia;
				   }
				   else
				   {
					   //��¼����ܲ��ĵ�һ�����˵�ֱ����
					   //�Ա㵱�ܼ���˨�ɻ�Сʱ�ܸ�������ֱ��ȷ����С����˨��ֱ����
					   //added by ligb on 2003.08.17
					   if(modPHScal::giDiameter==0)	
						   modPHScal::gmiDiameter=modPHScal::PtypeDiameter[i];
				   }				   
			   }
		   }
		   
		   rsTZB.Update();  //?,ÿ��case����������������
		   break;
	   case iROD:
		   //����
		   //�����������˵����Ƴ���
		   //L1�������ƣ�L2��������
		   rsX.GetFieldValue(_T("size4"),vTmp1);
		   rsTZB.SetFieldValue(_T("AllowedMinLugExt"),vtof(vTmp1));
		   //L1�������ƣ�L2��������
		   rsX.GetFieldValue(_T("size3"),vTmp1);
		   rsTZB.SetFieldValue(_T("AllowedMaxLugExt"),vtof(vTmp1));
		   //���ˣ����ֱ��
		   rsX.GetFieldValue(_T("size2"),vTmp1);
		   modPHScal::giDiameter = vtof(vTmp1);
		   //���ֱ��<10mm,�Զ�������������ֱ��
		   if(modPHScal::giDiameter<10)
		   {
			   modPHScal::gbLimitLugMinDia=true;
			   SetRegValue(_T("Settings"), _T("LimitLugMinDia"), modPHScal::gbLimitLugMinDia);
		   }
		   //�ڶ��μ���,��¼����ֱ��,�Ա��ҳ����ֵ�����еĵ���/����ɽ���ĸԼ�������ֱ��		  
		   //��һ�μ��㣬������Щֱ��
		   if( FirstCal ==1 /*|| FirstCal ==2*/ ) 
		   {		   
			   modPHScal::PtypeDiameter[i] = modPHScal::giDiameter;		  
			   if(modPHScal::gbRodDiaByMax)
			   {
				   if(modPHScal::iMaxRodDia>modPHScal::PtypeDiameter[i])
					   modPHScal::PtypeDiameter[i]=modPHScal::iMaxRodDia;
				   else
					   modPHScal::iMaxRodDia=modPHScal::PtypeDiameter[i];
				   modPHScal::gmiDiameter=modPHScal::iMaxRodDia;
			   }
			   else
			   {
				   //��¼����ܲ��ĵ�һ�����˵�ֱ����
				   //�Ա㵱�ܼ���˨�ɻ�Сʱ�ܸ�������ֱ��ȷ����С����˨��ֱ����
				   //added by ligb on 2003.08.17
				   if(modPHScal::giDiameter==0)	   modPHScal::gmiDiameter=modPHScal::giDiameter;
			   }
		   }
		   //��ʱ����sizeC�ֶα�������ֱ���������Ժ�������˳��ȡ�
		   rsTZB.SetFieldValue(_T("sizeC"),COleVariant(modPHScal::giDiameter));
		   //����ֱ�����浽���飬�Ա����ĩβ���Ҹ���ʱʹ�á���Ҫ��������¼λ������֮ǰ�������������������ʼ�˵ĸ����������������
		   //sizeH��Ϊ0���ں���ͳ���ܳ�
		   rsTZB.SetFieldValue(_T("sizeH"),COleVariant((long)0));
		   //��ʱ����GDW1�ֶα������˵���(kg/��),�����Ժ�������Ϊ����������嵥�ء�
		   rsX.GetFieldValue(_T("Weight"),vTmp1);
		   rsTZB.SetFieldValue(_T("GDW1"),vTmp1);
		   rsTZB.Update();  //?,ÿ��case����������������
		   break;
	   case iSAh:
	   case iSAbeam:
	   case iSACantilever:
	   case iSALbrace:
	   case iSALbraceFixG47:
	   case iSALbraceFixG48:
	   case iG51:
	   case iG52_55:
	   case iG56:
	   case iG57:
	   case iG100://�û��Զ��������������أ���Ҫͳ�Ʋ���
		   {
			   rsTZB.SetFieldValue(_T("CLcl"),STR_VAR(GetResStr(IDS_GROUPWARE)));
			   j = 1; //���͸�
			   CString sj;
			   sj.Format(_T("%d"),j);
			   rsX.GetFieldValue(_T("P")+sj,vTmp1);
			   CString sTmp3=vtos(vTmp1);
			   sTmp3=sTmp3.Mid(1);
			   sTmp3.Format(_T("%g"),_tcstod(sTmp3,NULL)*10);
			   rsX.GetFieldValue(_T("Pnum")+sj,vTmp2);
			   sTmp=vtos(vTmp1);
			   //Pnum1=NULL��0���������͸���������
			   if( sTmp!=_T("") &&  vtoi(vTmp2) > 0 )
				   //if( sTmp!=_T("") )			
			   {
				   if(modPHScal::glClassID == iSAh )
				   {
					   //ֱ�ӵ�G11
					   rsX.GetFieldValue(_T("PL")+sj,vTmp1);
					   sTmp=vtos(vTmp1);
					   if( sTmp!= _T("") )
					   {
						   rsX.GetFieldValue(_T("BH"),vTmp1);
						   EDIBgbl::dbPRJ.Execute( CString(_T("UPDATE tmpCSLen SET BH=\'")) + vtos(vTmp1) + _T("\'"));
						   rsTZB.SetFieldValue(_T("BH"),vTmp1);
						   rsX.GetFieldValue(_T("A"),vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));						
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET a=") +sTmp2);
						   //G11�Ĺ�ʽΪsizeH+A,����sizeH=�ݸ˳���,���ȴ�����A01�ֶΡ�
						   sTmp2.Format(_T("%g"),modPHScal::WidthA);
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET sizeH=") + sTmp2);
						   //��ʱֻҪ����������ĳ���
						   sngCSLen = modPHScal::CSLength(sBHFormat, sTmp, sBH);
					   }
					   rsTZB.SetFieldValue(_T("sizeH"),COleVariant((float)modPHScal::WidthA));
					   rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant((float)modPHScal::WidthA));
				   }
				   else
				   {
					   //��ֱ�ӵ�G11,���۸�
					   rsX.GetFieldValue(_T("PL")+sj,vTmp1);
					   sTmp=vtos(vTmp1);
					   if( sTmp!= _T("") )
					   {
						   rsX.GetFieldValue(_T("BH"),vTmp1);
						   EDIBgbl::dbPRJ.Execute( CString(_T("UPDATE tmpCSLen SET BH=\'")) + vtos(vTmp1) + _T("\'"));                           
						   rsTZB.SetFieldValue(_T("BH"),vTmp1);
						   //G100�Զ�����������͸��ͺ�P1д��tmpCSLen���Ա���Ϊ���Ϲ���.
						   rsX.GetFieldValue(_T("P1"),vTmp1);
						   EDIBgbl::dbPRJ.Execute( CString(_T("UPDATE tmpCSLen SET P1=\'")) + vtos(vTmp1) + _T("\'"));                           
						   
						   rsX.GetFieldValue(_T("A"),vTmp1);
						   double fLenA=0;//��¼���۶˲�������������֮��ľ���ֵA  Add by luorijin 2008.10.30
						   fLenA = vtof(vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));
						   if(modPHScal::glClassID==iG52_55)
						   {
							   //G52_55�Ĺ�ʽΪH+h+100,����H=���߸߶�,���ȴ�����A01�ֶΡ�
							   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET CHLegHeight=")+sTmp3 );
							   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET CHheight=") +sTmp3);					   
						   }
						   else
						   {
							   //G47��˫�۸ּ��chdist=300,G48��˫�۸ּ��=Bwidth(���ӿ��)
							   
						   }
						   
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET a=") +sTmp2);
						   rsX.GetFieldValue(_T("c"),vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET c=") +sTmp2);
						   sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("B01"))));
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET B=") +sTmp2);
						   sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET BWidth=") +sTmp2);
						   rsTZB.SetFieldValue(_T("B"),rsza->GetCollect(_T("B01")));
						   rsTZB.SetFieldValue(_T("A"),rsza->GetCollect(_T("A01")));
						   
						   //H=�������ĸ߶�
						   sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET H=") +sTmp2);
						   
						   
						   rsX.GetFieldValue(_T("m"),vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET m=") +sTmp2);
						   rsX.GetFieldValue(_T("CHdist"),vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET CHDist=") +sTmp2);
						   sTmp2.Format(_T("%g"),modPHScal::Lspan);
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET Lspan=") +sTmp2);
						   sTmp2.Format(_T("%g"),modPHScal::gdw);
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET GDW1=") +sTmp2);

						   sTmp2.Format(_T("%g"),modPHScal::Lspan);
						   EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET L1=") +sTmp2);
						   
						   //�Ѽ�֧�����l1д��ZB ��LSpan�ֶΣ��Է�����άʹ�ã����ұ��ڼ�¼�Ǳ����
						   rsTZB.SetFieldValue(_T("LSpan"),COleVariant((float)modPHScal::Lspan));
						   rsTZB.SetFieldValue(_T("XBL1"),COleVariant((float)(modPHScal::gdw+fLenA)));//XBL1��¼���۳���  Add by luorijin 2008.10.30
						   //��ʱֻҪ�������͸ֵĳ���
						   sngCSLen = modPHScal::CSLength(sBHFormat, sTmp, sBH);
					   }
					   //�۸յ�aֵ���浽�ֶ�T,��ֵ�ڻ��Ƹ�����λ�ߴ�ʱ����Ҫ����G22~G24,G3x,G6x,G7x��
					   //if( (sPartID.Left(2) == _T("G3")) || (sPartID.Left(2) ==  _T("G6") ) || (sPartID.Left(2) == _T("G7")) || (sPartID.Left(2) == _T("G5")) && (sPartID.Left(3) != _T("G51"))  && (sPartID.Left(3) != _T("G56"))  && (sPartID.Left(3) != _T("G57")) )
					   if( (modPHScal::glClassID==iSAbeam) || (modPHScal::glClassID==iG52_55 ) )
						   rsTZB.SetFieldValue(_T("T"),COleVariant((float)(modPHScal::Lspan - modPHScal::gdw)));
					   else
					   {
						   rsX.GetFieldValue(_T("A"),vTmp1);
						   rsTZB.SetFieldValue(_T("T"),vtof(vTmp1));
					   }
					   
					   rsTZB.SetFieldValue(_T("L1"),COleVariant((float)sngCSLen));
					   rsTZB.SetFieldValue(_T("GDW1"),COleVariant(modPHScal::gdw));
					   //����ԭʼ���ݱ�TZA�еĸ������۸�����
					   rsX.GetFieldValue(_T("Pnum1"),vTmp1);
					   if( vTmp1.vt==VT_NULL)
					   {
						   modPHScal::iCSnum = 1;
					   }
					   else
					   {
						   modPHScal::iCSnum = vtoi(vTmp1);
					   }
					   rsza->PutCollect(_T("iCSnum"),_variant_t((long)modPHScal::iCSnum));
					   
				   }
				   rsTZB.SetFieldValue(_T("CLgg"),STR_VAR(sBH));
				   //rsTZB.SetFieldValue(_T("BH"),STR_VAR(sBH));
				   //���¸������۸��ܳ���
				   rsza->PutCollect(_T("SATotalLength"),_variant_t(sngCSLen));
				   if(modPHScal::glClassID == iSAbeam )
				   {
				   }
				   else if(modPHScal::glClassID == iSALbrace || modPHScal::glClassID == iSALbraceFixG47 || modPHScal::glClassID == iSALbraceFixG48 || modPHScal::glClassID == iSACantilever )
				   {
					   //rsza->PutCollect(_T("SATotalLength"),_variant_t(sngCSLen));
					   rsza->PutCollect(_T("LC1"),_variant_t(sngCSLen));
				   }
				   
		  }
		  else
		  {
			  //����Ϊ�յĸ������˳����ӿ��ٶȡ�
			  //����ԭʼ���ݿ�ʱӦ��ע�����еĸ�����1��ʼ��д����Ҫ��ϡ�
			  //������ǰ��������ݲ�����ȫ��������Ҫ�󣬹�Ҫȡ���˳�������������Щ�������ᱻͳ�ơ�
			  //Exit For
			  if( j = 1 )
			  {
				  if(modPHScal::glClassID == iSAh ||modPHScal::glClassID == iGCement )
				  {
					  //ֱ�ӵ������������֧��
				  }
				  else
				  {
					  //�͸ָ�����P1�ֶα�����ֵ
					  rsX.GetFieldValue(_T("BH"),vTmp1);
					  sTmp2.Format(GetResStr(IDS_SAP1MustBeChannelSteelNo),modPHScal::tbnSA,vtos(vTmp1));
					  //frmStatus.MessageBox(sTmp2);
					  ShowMessage(sTmp2);
				  }
			  }
		  }
		  
		  rsTZB.Update();
		  //��������,j=1�����͸֣�ͳ��ʱ,�����֧������װͼ��
		  //���������ϸ��,��ֻ�����������ṹ;
		  //��ͳ�Ʋ���ʱ,Ӧ�ò���͸�.
		  //��������Ҫ��һ���ٶ����Ա�򻯴˴��ı�̡�
		  //�������и���Ҫô�ڸ������У�Ҫô�ڸ����������У�Ҫô����˨��ĸ���С�
		  //�Ҹ������е����и������Pi(i=1~12)�ֶε����ݼ��Ǹ����������е�BH�ֶΡ�
		  //��ˣ��ƶ�֧���ܸ����淶ʱ�� һ��Ҫʹ����ͬ��׼�ĸ�����͸�����
		  //ѡ��淶ʱ�򲻱ؿ��ǣ���Ϊ����������������ݱ����Ϣ������ڸ�����Ϣ�����PhsManuSA�С�
		  CString strBH;
		  
		  CString tmpID;
		  for (j = 1 ;j<= 12;j++)
		  {
			  sj.Format(_T("%d"),j);
			  rsX.GetFieldValue(_T("P")+sj,vTmp1);
			  rsX.GetFieldValue(_T("Pnum")+sj,vTmp2);
			  sTmp=vtos(vTmp1);
			  if( sTmp!=_T("") && vtoi(vTmp2)>0 )
			  {
				  mvSAattachedCustomID =sTmp;
				  //���Ƿ����͸�:�۸֡��Ǹ֡����ָ֡�Բ�֡���֣����ְ���Ϊ��������
				  //���(�ڸ�������͸ֱ���)�Ĵ�Ÿ�ʽӦΪ��-��x����-90x16��
				  SQL1 = _T("SELECT * FROM [") + modPHScal::tbnSectionSteel + _T("] WHERE trim(BH)=\'") + mvSAattachedCustomID + _T("\' ORDER BY bh");
				  if(rsTmp.IsOpen())
					  rsTmp.Close();
				  rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
				  rsTmp.Open(dbOpenSnapshot,SQL1);
				  if( rsTmp.IsEOF() && rsTmp.IsBOF() )
				  {
					  //�����͸�
					  //****************************
					  //�Ҳ���,���Ǹ���,�ٲ鸽����
					  rsTmp.Close();
					  SQL1 = _T("SELECT * FROM [") + modPHScal::tbnAttachment + _T("] WHERE trim(BH)=\'") + mvSAattachedCustomID + _T("\' ORDER BY bh");
					  /*if(rsTmp.IsOpen())
					  rsTmp.Close();
					  rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;*/
					  rsTmp.Open(dbOpenSnapshot,SQL1);
					  if( rsTmp.IsEOF() && rsTmp.IsBOF() )
					  {
						  //���Ǹ���,������˨��ĸ,�ٲ���˨��ĸ��
						  //��˨��ĸ������ʹ�õĿ������ض���׼����˨��ĸ�����������ʹ��������׼����˨��ĸ������ұ�׼��˨��ĸ��
						  //���ŷ�ʽΪM36x200,��M20
						  //��ˣ����Ȳ��Ҹ�����˨��ĸ��tbnSABoltsnuts���ٸ��ݲ���Ĺ���ͨ����˨��ĸ��tbnBoltsNutsID�в�����Ӧ�Ĺ��
						  rsTmp.Close();
						  SQL1 = _T("SELECT * FROM [") + modPHScal::tbnSABoltsNuts + _T("] WHERE trim(BH)=\'") + mvSAattachedCustomID + _T("\' ORDER BY bh");
						  /*if(rsTmp.IsOpen())
						  rsTmp.Close();
						  rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;*/
						  rsTmp.Open(dbOpenSnapshot,SQL1);
						  if( rsTmp.IsEOF() && rsTmp.IsBOF() )
						  {
							  //û�ҵ���˨��ĸ��¼
							  sBH = _T("");
							  strBH = _T("");
							  sngWeight = 0;
							  mviSASSClassID = iSABoltsNutsUnknown;
							  //rsTmp.Close();
							  sTmp2.Format(GetResStr(IDS_NotFoundThisSAAttached),mvSAattachedCustomID);
							  //frmStatus.MessageBox(sTmp2);
							  ShowMessage(sTmp2);
							  //Err.Number = iUE_NotFoundThisSAAttached
							  //Err.Description = ResolveResString(iUE_NotFoundThisSAAttached, _T("|1"), mvSAattachedCustomID)
							  //Err.Raise iUE_NotFoundThisSAAttached
							  //����һ�����󣬵���Ҫ�˳���
							  //MsgBox ResolveResString(iUE_NotFoundThisSAAttached, _T("|1"), mvSAattachedCustomID), vbMsgBoxSetForeground
						  }
						  else
						  {
							  //�ҵ���Ӧ����˨����ĸ
							  strBH = _T("");
							  rsTmp.MoveFirst();
							  rsTmp.GetFieldValue(_T("BH"),vTmp1);
							  sBH = vtos(vTmp1);
							  strBH=sBH;
							  sTmp=CString(_T("UPDATE tmpCSLen SET BH=\'")) + sBH + _T("\'");
							  EDIBgbl::dbPRJ.Execute( sTmp);
							  rsTmp.GetFieldValue(_T("CustomID"),vTmp1);
							  tmpCustomID = vtos(vTmp1);
							  sTmp= CString(_T("UPDATE tmpCSLen SET CustomID=\'")) + tmpCustomID + _T("\'");
							  EDIBgbl::dbPRJ.Execute( sTmp);
							  
							  rsTmp.GetFieldValue(_T("size2"),vTmp1);
							  tmpSize2 =vtof(vTmp1);
							  rsTmp.GetFieldValue(_T("sizeH"),vTmp1);
							  tmpSizeH =vtof(vTmp1);
							  tmpID=modPHScal::sFindID(tmpCustomID);
							  if(tmpID==_T("F9") || tmpID==_T("F10"))
							  {
								  if(modPHScal::glClassID==iG56 || modPHScal::glClassID==iG57 || modPHScal::glClassID==iG52_55 || modPHScal::glClassID==iG51)
								  {
									  rsX.GetFieldValue(_T("PL")+sj,vTmp1);
									  sTmp=vtos(vTmp1);
									  if(rsID.FindFirst(_T("trim(CustomID)=\'") + tmpCustomID + _T("\'")))
									  {
										  rsID.GetFieldValue(_T("BHFormat"),vTmp1);
										  sBHFormat = vtos(vTmp1);
									  }
									  else
									  {
									  }
									  sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
									  EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET BWidth=") +sTmp2);								
									  sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
									  EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET H=") +sTmp2);
									  sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("B01"))));
									  EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET B=") +sTmp2);								
									  //������˨�ĳ���
									  sngCSLen = modPHScal::CSLength(sBHFormat, sTmp, sBH);
									  SQL1.Format(_T("SELECT * FROM [%s] WHERE trim(CustomID)=\'%s\' AND SIZE2=%g  ORDER BY bh"),
										  modPHScal::tbnBoltsNuts,
										  tmpCustomID,
										  tmpSize2);
									  
								  }
							  }
							  else if(tmpID==_T("F14"))
							  {
								  //����ͷ��˨F14
								  //�ٸ��ݲ���Ĺ���ͨ����˨��ĸ��tbnBoltsNuts�в�����Ӧ�Ĺ��
								  SQL1.Format(_T("SELECT * FROM [%s] WHERE trim(CustomID)=\'%s\' AND SIZE2=%g AND sizeH=%g ORDER BY bh"),
									  modPHScal::tbnBoltsNuts,
									  tmpCustomID,
									  tmpSize2 ,
									  tmpSizeH);
								  sngCSLen=1000;
							  }
							  else
							  {
								  //������ĸF1�����Ǳ���ĸF2�������ȦF4��
								  //�ٸ��ݲ���Ĺ���ͨ����˨��ĸ��tbnBoltsNuts�в�����Ӧ�Ĺ��
								  SQL1.Format(_T("SELECT * FROM [%s] WHERE trim(CustomID)=\'%s\' AND SIZE2=%g ORDER BY bh"),
									  modPHScal::tbnBoltsNuts,
									  tmpCustomID,
									  tmpSize2);
							  }
							  rsTmp.Close();
							  rsTmp.Open(dbOpenSnapshot,SQL1);
							  if( rsTmp.IsEOF() && rsTmp.IsBOF() )
							  {
								  //û�м�¼
								  sBH = _T("");
								  sngWeight = 0;
								  mviSASSClassID = iBoltsNutsUnknown;
								  sTmp2.Format(GetResStr(IDS_NotFoundThisSAAttached),tmpCustomID);
								  //frmStatus.MessageBox(sTmp2);
								  ShowMessage(sTmp2);
							  }
							  else
							  {
								  //��ͨ����˨��ĸ�����ҵ���Ӧ����˨����ĸ
								  rsTmp.MoveFirst();
								  if(tmpID==_T("F9") || tmpID==_T("F10"))
								  {
									  //F9��F10�Ѿ������ֱ���ͳ���,ֻҪ��ͨ����˨��ĸ���в�����ǵ�CustomID��BH,������CLgg���Ϳ����ˡ�
									  rsTmp.GetFieldValue(_T("BH"),vTmp1);
									  sBH = vtos(vTmp1);
									  sTmp= CString(_T("UPDATE tmpCSLen SET BH=\'")) + sBH + _T("\'");
									  EDIBgbl::dbPRJ.Execute(sTmp);
									  
									  rsID.GetFieldValue(_T("BHFormat"),vTmp1);
									  sBHFormat = vtos(vTmp1);															    
									  sngCSLen = modPHScal::CSLength(sBHFormat, ftos(sngCSLen), sBH);
								  }
								  else if(tmpID==_T("F14"))
								  {
									  rsTmp.GetFieldValue(_T("BH"),vTmp1);
									  sBH = vtos(vTmp1);
									  sngCSLen = 1000;  //����Ϊ1000,���ڱ��.
								  }						
								  else
								  {
									  rsTmp.GetFieldValue(_T("BH"),vTmp1);
									  sBH = vtos(vTmp1);
									  sngCSLen = 1000;  //����Ϊ1000,���ڱ��.
								  }
								  rsTmp.GetFieldValue(_T("Weight"),vTmp1);
								  sngWeight =vtof(vTmp1)*sngCSLen/1000;
								  sngW = sngWeight;
								  rsTmp.GetFieldValue(_T("CustomID"),vTmp1);
								  tmpCustomID = vtos(vTmp1);                    
								  if(rsID.FindFirst( _T("trim(CustomID)=\'") + tmpCustomID + _T("\'")))
								  {
									  rsID.GetFieldValue(_T("ClassID"),vTmp1);
									  mviSASSClassID =vtoi(vTmp1);
									  rsID.GetFieldValue(_T("Index"),vTmp1);
									  mviSASSIndex =vtoi(vTmp1);
								  }
							  }
					  }
				   }
				   else
				   {
					   //�鸽�����ҵ�����,������˨��ĸ,
					   strBH=_T("");
					   rsTmp.MoveFirst();
					   rsTmp.GetFieldValue(_T("BH"),vTmp1);
					   sBH = vtos(vTmp1);
					   rsTmp.GetFieldValue(_T("Weight"),vTmp1);
					   sngWeight =vtof(vTmp1);
					   sngW = sngWeight;
					   rsTmp.GetFieldValue(_T("CustomID"),vTmp1);
					   tmpCustomID = vtos(vTmp1);
					   tmpID=modPHScal::sFindID(tmpCustomID);
					   if(rsID.FindFirst( _T("trim(CustomID)=\'") + tmpCustomID + _T("\'")))
					   {
						   rsID.GetFieldValue(_T("ClassID"),vTmp1);
						   mviSASSClassID =vtoi(vTmp1);
						   rsID.GetFieldValue(_T("Index"),vTmp1);
						   mviSASSIndex =vtoi(vTmp1);
					   }
					   sngCSLen = 1000;  //����Ϊ1000,���ڱ��.
				   }
				   //sngCSLen = 1000;  //����Ϊ1000,���ڱ��.
				   //****************************
				   //���������͸�
				}
				else
				{
					//���͸�
					//if(modPHScal::glClassID = iSACantilever OrmodPHScal::glClassID = iSAbeam OrmodPHScal::glClassID = iSALbrace )
					if(modPHScal::glIDIndex == iSA )
					{
						//�Ǹ��������е��͸�
						//���͸ֵ���
						if( rsTmp.IsEOF() && rsTmp.IsBOF() )
						{
							//û�������͸ֹ��
							sTmp.Format(GetResStr(IDS_NothisSectionSteelInZDJcrudeMdb),modPHScal::dbZDJcrude.GetName(),mvSAattachedCustomID);
							throw sTmp;
						}
						else
						{
							//�������͸ֹ��
							rsTmp.MoveFirst();
							rsTmp.GetFieldValue(_T("CustomID"),vTmp1);
							tmpCustomID =vtos(vTmp1);
							rsTmp.GetFieldValue(_T("Material"),vTmp1);
							mvsSASSMaterial = vtos(vTmp1);
							
							if( rsID.FindFirst(_T("trim(CustomID)=\'") + tmpCustomID + _T("\'")))
							{
								rsID.GetFieldValue(_T("ClassID"),vTmp1);
								mviSASSClassID = vtoi(vTmp1);
								rsID.GetFieldValue(_T("Index"),vTmp1);
								mviSASSIndex =vtoi(vTmp1);
								//��ʱ����������͸ָ߶ȣ������Ժ�ȷ�������������ӵ���˨�ĸ߶ȡ�
								if( j == 1 )
								{
									rsTmp.GetFieldValue(_T("sizeH"),vTmp1);
									SACSHeight =vtof(vTmp1);
								}
							}
						}
						rsX.GetFieldValue(_T("PL")+sj,vTmp1);
						sTmp=vtos(vTmp1);
						if(sTmp!=_T("") )
						{
							rsTmp.GetFieldValue(_T("BH"),vTmp1);
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET BH=\'") + vtos(vTmp1) + _T("\'"));
							vBH = vTmp1;			
							rsTmp.GetFieldValue(_T("CustomID"),vTmp1);
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET CustomID=\'")+vtos(vTmp1)+_T("\'"));
							rsX.GetFieldValue(_T("A"),vTmp1);
							sTmp2.Format(_T("%g"),vtof(vTmp1));
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET a=") +sTmp2);
							rsX.GetFieldValue(_T("c"),vTmp1);
							sTmp2.Format(_T("%g"),vtof(vTmp1));
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET c=") +sTmp2);
							if( modPHScal::glClassID==iG52_55 || modPHScal::glClassID==iG51)
							{
								sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
								EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET H=") +sTmp2);
							}
							else
							{
								sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
								EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET BWidth=") +sTmp2);
							}
							sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("B01"))));
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET B=") +sTmp2);
							rsX.GetFieldValue(_T("m"),vTmp1);
							sTmp2.Format(_T("%g"),vtof(vTmp1));
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET m=") +sTmp2);
							rsX.GetFieldValue(_T("CHdist"),vTmp1);
							sTmp2.Format(_T("%g"),vtof(vTmp1));
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET CHDist=") +sTmp2);
							rsTmp.GetFieldValue(_T("sizeH"),vTmp1);
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET CHHeight=")+(vtos(vTmp1)==_T("") ? _T("0") : vtos(vTmp1)) );
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET CHLegHeight=")+(vtos(vTmp1)==_T("") ? _T("0") : vtos(vTmp1)) );
							sTmp2.Format(_T("%g"),modPHScal::gdw);
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET GDW1=") +sTmp2);
							sTmp2.Format(_T("%g"),modPHScal::Lspan);
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET Lspan=") +sTmp2);
							sTmp2.Format(_T("%g"),modPHScal::Lspan);
							EDIBgbl::dbPRJ.Execute( _T("UPDATE tmpCSLen SET [L1]= ") +sTmp2);
							sngCSLen = modPHScal::CSLength(sBHFormat, sTmp, sBH);
						}
						//sBH�к�����Чֵ,�����͸ֲ��罫����Ϊ����������
						sBH = mvSAattachedCustomID;
						//�͸�����=ÿ������x����(mm)/1000
						rsTmp.GetFieldValue(_T("Weight"),vTmp1);
						sngWeight =vtof(vTmp1);
						sngW = sngWeight * sngCSLen / 1000;
					}
				}
				
				rsX.GetFieldValue(_T("Pnum")+sj,vTmp1);
				//������������=����ÿ������������֮��kg
				mvarSATotalWeight = mvarSATotalWeight + sngW * vtof(vTmp1);
				rsSAPart.AddNew();
				
				rsSAPart.SetFieldValue(_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
				rsSAPart.SetFieldValue(_T("zdjh"),COleVariant((long)modPHScal::zdjh));
				//2002.05.26�Ժ��д�˶δ��룬����phs.arx˼·һ�¡�
				//�Ǹ�������,��д��¼��,
				//�Ա����ָ��������벿��.Ҳ���ڶ������š�
				iSEQofPart++;
				rsSAPart.SetFieldValue(_T("recno"),COleVariant((long)iSEQofPart));
				//��Ϊ������������������ֱ�־��IsSAPart�ֶ��Ƿ�=-1��
				rsSAPart.SetFieldValue(_T("IsSAPart"),COleVariant((long)-1));
				//rsSAPart.SetFieldValue(_T("Index")) = iSA
				rsSAPart.SetFieldValue(_T("nth"),COleVariant((short)nth));//��׼֧����·��=1�������Գ�˫���͹��ø���˫��
				rsSAPart.SetFieldValue(_T("ClassID"),COleVariant((long)mviSASSClassID));
				rsSAPart.SetFieldValue(_T("Index"),COleVariant((long)mviSASSIndex));
				rsTmp.GetFieldValue(_T("CustomID"),vTmp1);
				rsSAPart.SetFieldValue(_T("CustomID"),STR_VAR(vtos(vTmp1)));
				//sBH				
				rsSAPart.SetFieldValue(_T("CLgg"),STR_VAR(sBH));
				if(tmpID=="F9" || tmpID=="F10" || tmpID=="F14")
				{				
					rsSAPart.SetFieldValue(_T("BH"),STR_VAR(strBH));
					//������˨���ݸ˵ĳ��ȣ�
					rsSAPart.SetFieldValue(_T("L1"),COleVariant( sngCSLen));
				}
				else
				{
					
					if(mviSASSIndex==iSectionSteel )
					{
						//�����͸ֵĳ��ȣ�
						rsSAPart.SetFieldValue(_T("L1"),COleVariant( sngCSLen));
						//rsSAPart.SetFieldValue(_T("GDW1"),COleVariant(modPHScal::gdw));
						rsSAPart.SetFieldValue(_T("BH"),STR_VAR(sBH));
					}
					else
						rsSAPart.SetFieldValue(_T("BH"),STR_VAR(sBH));
				}
				
				rsTmp.GetFieldValue(_T("Material"),vTmp1);
				
				rsSAPart.SetFieldValue(_T("CLcl"),STR_VAR(vtos(vTmp1)));
				rsX.GetFieldValue(_T("Pnum")+sj,vTmp1);
				rsSAPart.SetFieldValue(_T("CLnum"),vTmp1);
				rsSAPart.SetFieldValue(_T("CLbz"),vnil);
				rsSAPart.SetFieldValue(_T("CLdz"),COleVariant(sngWeight));
				rsX.GetFieldValue(_T("Pnum")+sj,vTmp1);
				rsSAPart.SetFieldValue(_T("CLzz"),COleVariant(sngW * vtof(vTmp1)));
				rsID.GetFieldValue(_T("Description"),vTmp1);
				rsSAPart.SetFieldValue(_T("CLmc"),vTmp1);
				rsSAPart.Update();
			 }
			 else
			 {
				 //����Ϊ�յĸ������˳����ӿ��ٶȡ�
				 //����ԭʼ���ݿ�ʱӦ��ע�����еĸ�����1��ʼ��д����Ҫ��ϡ�
				 //������ǰ��������ݲ�����ȫ��������Ҫ�󣬹�Ҫȡ���˳�������������Щ�������ᱻͳ�ơ�
				 //Exit For
				 //�¾������ѭ��
				 //continue;
			 }
		  }
		  }
		  break;
	   case iSPR:
		   {
			   rsTZB.SetFieldValue(_T("CLcl"),STR_VAR(GetResStr(IDS_GROUPWARE)));
			   //Height_SPRINGsL5 = IIf(IsNull(rsX(_T("size3"))), 0, rsX(_T("size3")))
			   if(sPartID==_T("T4"))
			   {
				   rsX.GetFieldValue(_T("sizeH"),vTmp1);
				   sngDim = vtof(vTmp1) - /*Sgn(modPHScal::yr) */ modPHScal::sSprInfo[iSEQofSPR].haz;//z.jia add
				   //rsTZB.SetFieldValue(_T("Size")) = Height_SPRINGsL5 / 2
			   }
			   else if(sPartID==_T("T3") || sPartID==_T("T3A"))
			   {
				   rsX.GetFieldValue(_T("sizeH"),vTmp1);
				   sngDim = fabs(vtof(vTmp1)) - modPHScal::sSprInfo[iSEQofSPR].haz;
				   //��/�·����ɣ���ֵ�ڼ�������ʱӦ��ʹ���˼ӳ�
				   sngDim = -sngDim;
			   }
			   //rsTZB.SetFieldValue(_T("Size")) = Height_SPRINGsL5 / 2
			   else
			   {
				   rsX.GetFieldValue(_T("sizeH"),vTmp1);
				   sngDim = vtof(vTmp1) + modPHScal::sSprInfo[iSEQofSPR].haz;
			   }
			   //���ɵĳߴ綼�ǰ��ɽ���ĸ��Ե���ġ��Ұ���Сλ��ʱ�����߶ȡ�
			   //���ײ�ͬ���ɽ���ĸʱӦ�û��㲻ͬ�ĸ߶ȡ����ִ�������ݲ��ṩ��
			   //���ɵ��ɽ���ĸ��CalLugLength�в��
			   rsTZB.SetFieldValue(_T("SizeH"),COleVariant(sngDim));
			   rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant(sngDim));
			   //rsTZB.SetFieldValue(_T("CLgg")) = sBHformat
			   if( EDIBAcad::g_bBomSprFactory )//newHS->ֻ��ѡ���˱�ע���ɳ��Ҳ�д�����ݿ�
			   {
				   rsTZB.SetFieldValue(_T("CLbz"),STR_VAR(modPHScal::gsSPRmanufactory));
			   }
			   rsTZB.SetFieldValue(_T("SpringNo"), COleVariant( (long)modPHScal::sSprInfo[iSEQofSPR].DH) );
			   rsTZB.SetFieldValue(_T("LugDia"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].FiJ));
			   rsTZB.SetFieldValue(_T("HeatDisp"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].HeatDisp));
			   rsTZB.SetFieldValue(_T("HAZ"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].haz));
			   
			   rsTZB.Update();
		   }
		   break;
	   case iCSPR:
		   {
			   rsTZB.SetFieldValue(_T("CLcl"),STR_VAR(GetResStr(IDS_GROUPWARE)));
			   //��¼ԭʼ�ߴ�T�����ڹ���GetBoltsNutsAndAttachmentsCLgg׼ȷ������������ӵ���˨����
			   rsX.GetFieldValue(_T("T"),vTmp1);
			   rsTZB.SetFieldValue(_T("T"),COleVariant((long)vtoi(vTmp1)));
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
				   tmpMovement=modPHScal::sSprInfo[iSEQofSPR].SumDisp;
			   else
				   tmpMovement=modPHScal::sSprInfo[iSEQofSPR].haz;
			   SQLx.Format(_T("SELECT * FROM [%s] WHERE trim(CustomID)=\'%s\' AND minDH<=%d AND %d<=maxDH AND fBmin<=%g AND fBmax>=%g "),
				   modPHScal::tbnCSPRINGL5Crude,modPHScal::sFindCustomID(sPartID),modPHScal::sSprInfo[iSEQofSPR].DH,modPHScal::sSprInfo[iSEQofSPR].DH,
				   tmpMovement,tmpMovement);
			   
			   rsDiaOfCSPRFiJ.GetFieldValue(_T("FiJ"),vTmp1);
			   //SQLx.Format(_T("SELECT * FROM [%s] WHERE size2=%g"),modPHScal::tbnCSPRINGL5Crude,vtof(vTmp1));
			   if(rsTmp.IsOpen())
				   rsTmp.Close();
			   rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
			   rsTmp.Open(dbOpenSnapshot,SQLx);
			   if( rsTmp.IsEOF() && rsTmp.IsBOF() )
			   {
				   //û���ҵ�ֱ��ֵ
				   sTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude.GetName,modPHScal::tbnCSPRINGL5Crude,SQLx);
				   throw sTmp;
			   }
			   else
			   {
				   rsTmp.GetFieldValue(_T("sizeH"),vTmp1);
				   Height_SPRINGsL5 =vtof(vTmp1);
				   //��ȡ���˿ɲ������󳤶Ⱥ���С����
				   rsTmp.GetFieldValue(_T("size4"),vTmp1);
				   rsTZB.SetFieldValue(_T("AllowedMinLugExt"),COleVariant(vtof(vTmp1)));//���浽���ݿ⣬�Ա�CalLugLength����ʹ��
				   rsTmp.GetFieldValue(_T("size5"),vTmp1);
				   rsTZB.SetFieldValue(_T("AllowedMaxLugExt"),COleVariant(vtof(vTmp1)));
			   }
			   //���������Ϸ��������˵��¶˲���ֵ�����浽ZB���AllowedMaxLugExtUp��AllowedMinLugExtUp�ֶ���
			   if( sPartID == _T("LHA") || sPartID == _T("LHB") || sPartID == _T("PHA")|| sPartID == _T("PHB"))
			   {
				   rsX.GetFieldValue (_T("PP"),vTmp1);//pfg2005.12.23 �϶���С����ֵӦ��ΪPP
				   rsTZB.SetFieldValue(_T("AllowedMaxLugExtUp"),COleVariant(vtof(vTmp1)));
				   rsTZB.SetFieldValue(_T("AllowedMinLugExtUp"),COleVariant(vtof(vTmp1)));
			   }
			   
			   //����������ɵ�ѡ��λ��modPHScal::sSprInfo[iSEQofSPR].SumDisp���õ�������λ���ֶ�fBmin��һ��ֵ��
			   //��������ɵİ�װ�߶�H1(sngH1xmax)=ֱ�Ӳ���������λ���ֶ�fBmin��ֵ��
			   //���򣬰���ֵ������λ�ơ�add by ligb on 2004.11.24
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
			   tmpSQL+=(modPHScal::gbCSPRneedSpecialDesign ? _T(" ") : sTmp) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY dh,Weight");
			   rsX1.m_pDatabase=&modPHScal::dbZDJcrude;
			   rsX1.Open(dbOpenSnapshot,tmpSQL);
			   if(rsX1.IsBOF() && rsX1.IsEOF()||1)//�������ٽ�ֵʱ����(sizeH�ĸ߶�û�м���ʵ��λ��)pfg and lgb 2005.12.15
			   {
				   //���ݵ�����е���������ݿ��ֶ�sizeH�洢����ȷ�ĸ߶�ֵ��
				   rsX.GetFieldValue(_T("sizeH"),vTmp1);
				   //rsX.GetFieldValue(_T("E"),vTmp1);
				   sngH1xmax =vtof(vTmp1);
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
				   if(sPartID == _T("LHD") || sPartID==_T("PHD"))
				   {
				   }
				   else if(sPartID==_T("LHA") ||sPartID==_T("PHA") || sPartID==_T("LHB") || sPartID==_T("PHB"))
				   {
					   //��ʽ��������
					   //�ֶ�A�������˲��볤��
					   //ǧ�򲻿�����������䣬�������˳ߴ������󣬲��ҵ��������ߴ�������
					   //rsTZB.SetFieldValue(_T("A")) = IIf(IsNull(rsX(_T("PP"))), 0, rsX(_T("PP")))
				   }
				   else if(sPartID==_T("LHE") || sPartID==_T("LHE1") || sPartID==_T("PHE") || sPartID==_T("PHE1") || sPartID==_T("PHC") || sPartID==_T("LHC"))
				   {
					   //��ʽ�������ӣ�Ҫ���϶���߶�T��
					   if( !modPHScal::gbCH1IncludeT )
					   {
						   rsDiaOfCSPRFiK.GetFieldValue(_T("T"),vTmp1);
						   sngDim = sngDim +vtof(vTmp1);
					   }
				   }
				   else if(sPartID==_T("ZHB"))
				   {
					   //֧��ʽ��������
				   }
				   else
				   {
				   }
			   }
			  /* else
			   {
				   rsX1.GetFieldValue(_T("sizeH"),vTmp1);
				   sngH1xmax =vtof(vTmp1);
				   if(modPHScal::gCH1distPos == 1 )
				   {
					   //E�ֶθ������м�λ�õĸ߶�
					   if(modPHScal::gbCalcCSPRHeight_BySelDisp)
						   sngDim = sngH1xmax;
					   else
						   sngDim = sngH1xmax;
				   }
				   else if(modPHScal::gCH1distPos == 0 )
				   {
					   //E�ֶθ�������Сλ��λ�õĸ߶�
					   sngDim = sngH1xmax;
					   //���������򻪶�ʯ�;�����װ�������ܳ��ֳܼܷ��Ľ��ͣ����Ϲ�ʽ�ɷ���,Ӧ�ð����й�ʽ
					   // sngDim = sngH1xmax + Sgn(modPHScal::yr) * modPHScal::sSprInfo[iSEQofSPR].haz /(modPHScal::bE_FormulaPlusHalf?2:2);
				   }
				   else if(modPHScal::gCH1distPos == 2 )
				   {
					   //E�ֶθ��������λ��λ�õĸ߶�
					   rsX.GetFieldValue(_T("fBmax"),vTmp1);
					   rsX.GetFieldValue(_T("fBmin"),vTmp2);
					   //sngDim = sngH1xmax + (vtof(vTmp1) - vtof(vTmp2))/2;
					   sngDim = sngH1xmax;
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
				   //if( !modPHScal::gbH1IncludeB ){ sngDim = sngDim + IIf(IsNull(rsX.Fields(_T("y"))), 0, rsX.Fields(_T("y")))
				   if( !modPHScal::gbCH1IncludeB )
				   {
					   rsTmp.GetFieldValue(_T("size4"),vTmp1);
					   sngDim = sngDim + vtof(vTmp1);
				   }
				   if(sPartID == _T("LHD") || sPartID==_T("PHD"))
				   {
				   }
				   else if(sPartID==_T("LHA") ||sPartID==_T("PHA") || sPartID==_T("LHB") || sPartID==_T("PHB"))
				   {
					   //��ʽ��������
					   //�ֶ�A�������˲��볤��
					   //ǧ�򲻿�����������䣬�������˳ߴ������󣬲��ҵ��������ߴ�������
					   //rsTZB.SetFieldValue(_T("A")) = IIf(IsNull(rsX(_T("PP"))), 0, rsX(_T("PP")))
				   }
				   else if(sPartID==_T("LHE") || sPartID==_T("LHE1") || sPartID==_T("PHE") || sPartID==_T("PHE1") || sPartID==_T("PHC") || sPartID==_T("LHC"))
				   {
					   //��ʽ��������
					   if( !modPHScal::gbCH1IncludeT )
					   {
						   rsDiaOfCSPRFiK.GetFieldValue(_T("T"),vTmp1);
						   sngDim = sngDim +vtof(vTmp1);
					   }
				   }
				   else if(sPartID==_T("ZHB"))
				   {
					   //֧��ʽ��������
				   }
				   else
				   {
				   }
			   }*/
			   rsX1.Close();

			   //����ɽ���ĸ���ȵ���ֵ
			   rsTZB.SetFieldValue(_T("SizeH"),COleVariant(sngDim));
			   rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant(sngDim));
			   if( EDIBAcad::g_bBomSprFactory )//newHS->ֻ��ѡ���˱�ע�������ɳ��Ҳ�д�����ݿ�
			   {
				   rsTZB.SetFieldValue(_T("CLbz"),STR_VAR(modPHScal::gsCSPRmanufactory));
			   }
			   //��ʱ����gdw1�ֶα��������أ�����������GetBoltsNutsAndAttachmentsCLggʹ��
			   rsTZB.SetFieldValue(_T("GDW1"),COleVariant(tmpSelPJG4CSPRFiK / iNumPart));
			   rsTZB.SetFieldValue(_T("SpringNo"), COleVariant( (long)modPHScal::sSprInfo[iSEQofSPR].DH) );
			   rsTZB.SetFieldValue(_T("LugDia"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].FiJ));
			   rsTZB.SetFieldValue(_T("HeatDisp"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].HeatDisp));
			   
			   rsTZB.Update();
		  }
		  break;
	default:
		//�������㲿������
		rsTZB.Update();
	}
	//�����ڴ����������ܴ������Ӹ�������ΪҪ���Ҹ�����
	//���8�����Ӹ���,eg. F1/F2/...,etc.
	//���һ������Ǹ���,�����ܻ����������,
	//�ʸ�������i < rsTmpZB.RecordCount - 1
	//���ӱ������˨��ĸ������Ժ��CustomID,����˵��ID�������ʹ��������׼����������˨ʱ��Ҫ�Ƚ��б任��
	//�任������Cphs.GetphsBHandSizes��ʵ�֡�
	//���Ӹ�������˨��ĸ��Ƭ��壩�ļ���Ϣд��TZB���������ȼ�����ɺ�
	//��һ�������Ĺ���CalLugLength�д��������л�Ҫ�����·����ɵı�š���˨��ĸ��ֱ���ȡ�
	if( iAttachIncd == iAttachIncluded && i < C - 1 )
	{
		//����˨��ĸ��־ΪbWithBoltsNuts=False
		bWithBoltsNuts = false;
		if(modPHScal::glClassID == iCSPR )
		{
			//��ǰ����Ǻ�������
			if( PtypeIndex[i + 1] == iSA )
			{
				//��һ������Ǹ���
				if(modPHScal::gbWorksWithSABoltsNuts )
					//�����Դ���������ӵ���˨��ĸ
					bWithBoltsNuts = true;
			}
			else
			{
				//��һ��������Ǹ����������Ӽ�
				if(modPHScal::gbWorksWithBoltsNuts )
					//�����Դ������Ӽ����ӵ���˨��ĸ
					bWithBoltsNuts = true;
			}
		}
		
		//������Ҳ��Դ���˨��ĸ
		
		for (j = 1;j<= 8;j++)
		{
			CString sj;
			strClbz.Empty();   //ADD BY LFX 2005.5.20
			sj.Format(_T("%d"),j);
			rsConnect.GetFieldValue(_T("CntbF")+sj,vTmp1);
			if(vtos(vTmp1)!=_T("") )
			{
				
				
				if( rsPartBoltNuts.IsEOF() )
					continue;
				rsPartBoltNuts.Edit();
				iSEQofPart = iSEQofPart + 1;
				rsPartBoltNuts.SetFieldValue(_T("BHformat"),STR_VAR(sBHFormat));
				rsPartBoltNuts.SetFieldValue(_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
				rsPartBoltNuts.SetFieldValue(_T("zdjh"),COleVariant((long)modPHScal::zdjh));
				rsPartBoltNuts.SetFieldValue(_T("recno"),COleVariant((long)iSEQofPart));

				//rsPartBoltNuts.SetFieldValue(_T("index"),COleVariant((long)iBoltsNuts));
				rsPartBoltNuts.SetFieldValue(_T("nth"),COleVariant((short)nth));//��׼֧����·��=1�������Գ�˫���͹��ø���˫��
				rsConnect.GetFieldValue(_T("CntbF")+sj+_T("num"),vTmp1);

				if( vTmp1.vt==VT_NULL)
				{
					//��˨��ĸ����=����������*1///
					rsPartBoltNuts.SetFieldValue(_T("CLnum"),COleVariant(iNumPart * iNum * iCntbNum));
				}
				else
				{
					//��������=ʵ������,����vtmp1ֵ Ϊ������
					rsPartBoltNuts.SetFieldValue(_T("CLnum"),COleVariant(vtoi(vTmp1) * iNumPart * iNum * iCntbNum));
										//lfx 2005.5.20�� 
					//����Ժ�������Դ���ĸ,�������װͼ���ƹ����еĲ���ͳ����
					//�����ĸ��ĿΪ��,��Ҫ������Ӧ��˵��
					//����Ժ�������Դ���ĸ,��˻�����װͼʱ��ע"�����Դ�"����
					if (0 == vtoi(vTmp1))
					{
						strClbz.Format("�����Դ�");
					}
					//end lfx 2005.5.20��
				}
				
				
				//�����¼ID,�����ͼ����ȷ��
				rsConnect.GetFieldValue(_T("CntbF")+sj,vTmp1);
				rsPartBoltNuts.SetFieldValue(_T("ID"),vTmp1);
				
				if(rsID.FindFirst (_T("trim(ID)=\'") + vtos(vTmp1) + _T("\'")))
				{
					rsID.GetFieldValue(_T("Description"),vTmp1);
					rsPartBoltNuts.SetFieldValue(_T("CLmc"),vTmp1);
					rsID.GetFieldValue(_T("CustomID"),vTmp1);
					rsPartBoltNuts.SetFieldValue(_T("CustomID"),vTmp1);
					rsID.GetFieldValue(_T("ClassID"),vTmp1);
					rsPartBoltNuts.SetFieldValue(_T("ClassID"),vTmp1);
				}
				//rsPartBoltNuts.SetFieldValue(_T("CustomID")) = IIf(gbFDid, sFindCustomID(rsc.Fields(_T("CntbF") + j)), rsc.Fields(_T("CntbF") + j))
				//rsPartBoltNuts.SetFieldValue(_T("ID")) = IIf(gbFDid, rsc.Fields(_T("CntbF") + j), sFindID(rsc.Fields(_T("CntbF") + j)))
				//rsPartBoltNuts.SetFieldValue(_T("CLmc")) = sFindFLD(IIf(gbFDid, _T("ID"), _T("CustomID")), _T("Description"), IIf(gbFDid, rsc.Fields(_T("CntbF") + j), sFindCustomID(rsc.Fields(_T("CntbF") + j))))
				if( ! bWithBoltsNuts )
				{
					//���Ҳ��ṩ����˨��ĸ
					//��������ǿյ����֣����ڱ��
					rsPartBoltNuts.SetFieldValue(_T("SEQ"),COleVariant((long) i));

					//lfx 2005.5.20�� 
					//˵��������//lfx 2005.5.20�� ע��
					//ԭrsPartBoltNuts.SetFieldValue(_T("CLbz"),vnil);
					rsPartBoltNuts.SetFieldValue(_T("CLbz"),_bstr_t(strClbz));  
					//End 2005.5.20��
				}
				else
				{
					rsPartBoltNuts.SetFieldValue(_T("SEQ"),vnil);
					rsPartBoltNuts.SetFieldValue(_T("CLnum"),COleVariant((long)0));
					CString str;
					str.LoadString(IDS_PRODUCEFAC_OFFERGOODS);
					rsPartBoltNuts.SetFieldValue(_T("CLbz"),STR_VAR(str));
				}
				rsPartBoltNuts.Update();
				rsPartBoltNuts.MoveNext();
				rsTZB.MoveNext();
			}
			else
			{
				//����Ϊ�յĸ������˳����ӿ��ٶȡ�
				//����ԭʼ���ݿ�ʱӦ��ע�����еĸ�����1��ʼ��д����Ҫ��ϡ�
				break;
			}
		}
	}
	
	//NotEditData:
	if(modPHScal::glClassID == iCSPR )
	{
		iSEQofSPR = iSEQofSPR + 1;
		if( iSEQofSPR >modPHScal::giWholeSprNum )
		{
			sTmp.Format(GetResStr(IDS_iSEQofSPROvergiWholeSprNum),_T("\%d"),_T("\%d"));
			sTmp2.Format(sTmp,iSEQofSPR,modPHScal::giWholeSprNum);
			throw sTmp2;
		}
	}
	else if(modPHScal::glClassID == iSPR )
	{
		iSEQofSPR = iSEQofSPR + 1;
		if( iSEQofSPR >modPHScal::giWholeSprNum )
		{
			sTmp.Format(GetResStr(IDS_iSEQofSPROvergiWholeSprNum),_T("\%d"),_T("\%d"));
			sTmp2.Format(sTmp,iSEQofSPR,modPHScal::giWholeSprNum);
			throw sTmp2;
		}
		else
		{
			if( iSumSerialNum ==modPHScal::giSumSerialNum )
			{
				//Err.Raise iUE_SumSerialNumEQVNeedSerialNum, , ResolveResString(iUE_SumSerialNumEQVNeedSerialNum, _T("|1"), iSEQofSPR, _T("|2"), iSumSerialNum)
			}
			else
			{
				if( iSumSerialNum >modPHScal::giSumSerialNum )
				{
					sTmp.Format(GetResStr(IDS_SumSerialNumOverNeedSerialNum),_T("\%d"),_T("\%d"));
					sTmp2.Format(sTmp,iSumSerialNum,modPHScal::giSumSerialNum);
					throw sTmp2;
				}
			}
		}
	}
	//����ǰһ�ε�����ƥ���ѯ�ֶ���Ϣ
	//tmpFD0valuePrevious = tmpFD0value
	//tmpRelationPrevious = tmpExtLenValue
	//tmpFD1Previous = tmpExtLenFD
	mlfx = mlfx + mlfx0;
	mlRot = mlRot0;
	tmpSQL = tmpSQL0;
	//��һ����������غ�
	tmpSelPJG4CSPRFiJ = tmpSelPJG4CSPRFiK;
	if( tmpSelPJG4CSPRPgzNext != 0 ) tmpSelPJG4CSPRPgz = tmpSelPJG4CSPRPgzNext;
	//��һ�����ɼ����غ�
	if( tmpSelPJG4SPRPgzNext != 0 )  tmpSelPJG4SPRPgz  = tmpSelPJG4SPRPgzNext;
	if( tmpSelPJG4SPRPazNext != 0 )  tmpSelPJG4SPRPaz  = tmpSelPJG4SPRPazNext;
	//���������
	iSEQofPart = iSEQofPart + 1;
	//����ǰһ����������ͺţ������б�
	mviPreviousClassID =modPHScal::glClassID;
	rsTmpZB.MoveNext();
	rsTZB.MoveNext();
	}//��ѭ������
	
	//�жϵ����Ƿ����ɹ�
	if(modPHScal::giWholeSprNum > 0 && bSPRphs )
	{
		if( iSumSerialNum >modPHScal::giSumSerialNum )
		{
			sTmp.Format(GetResStr(IDS_SumSerialNumOverNeedSerialNum),_T("\%d"),_T("\%d"));
			sTmp2.Format(sTmp,iSumSerialNum,modPHScal::giSumSerialNum);
			throw sTmp2;
		}
		else
		{
			if( iSumSerialNum <modPHScal::giSumSerialNum ){
				sTmp.Format(GetResStr(IDS_SumSerialNumLitterNeedSerialNum),_T("\%d"),_T("\%d"));
				sTmp2.Format(sTmp,iSumSerialNum,modPHScal::giSumSerialNum);
				throw sTmp2;
			}
		}
	}
	ret = true;
	
	
	//��˨��ĸ�������Ѿ��ĵ�ModPhsCalģ���CalBoltsNutsAndAttachmentsCLgg���̴���
	//���������Ĺ���Ѿ��ڴ����������1~9ʱ��ɡ�
	//���¸�����������,���һ��recno��Ϊ�յļ�¼���Ǹ�����¼,
	//����,Ҫ�ų������ǻ��������������
	
	//if(rsTZB.FindFirst(_T("IsNull(recno) AND (bUserAdd is null OR bUserAdd=0)")))
	//2002.05.25�Ժ����������д��¼�ţ��ʸ�д�˶γ���LEE Guobin
	if(rsTZB.FindFirst(_T(" (bUserAdd is null OR bUserAdd=0) AND Index=") + ltos(iSA)))
	{	
		rsTZB.Edit();
		if( rsTZB.IsBOF() )			
		{
			sTmp.Format(GetResStr(IDS_BOForEOFofmvarRsTZB),  _T("IsBOF()"));
			throw sTmp;
		}
		if( rsTZB.IsEOF() )			
		{
			sTmp.Format(GetResStr(IDS_BOForEOFofmvarRsTZB),  _T("IsEOF()"));
			throw sTmp;
		}
	}
	//д��seq�ֶ�,�����Ժ��������,
	//��������֮��,��GetphsBOMandCrd�и���Ч������ʱ,
	//ֻҪ����������seq<>Null�ļ�¼,
	//���ɻ������Ҫ�����AutoCAD�����ϸ��ļ�¼,
	//�����ȫ�ų��˶Ի�������������Ч����ͳ��
	rsTZB.GetFieldValue(_T("CustomID"),vTmp1);
	
	if( !rsID.FindFirst(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'"))){
		sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),  _T("CustomID"), vtos(vTmp1));
		throw sTmp;
	}
	else
	{
		rsTZB.Edit ();
		rsID.GetFieldValue(_T("ClassID"),vTmp1);
		if(  vtoi(vTmp1)== iGCement )
		{
			rsTZB.SetFieldValue(_T("SEQ"),vnil);
			rsTZB.SetFieldValue(_T("CLdz"),COleVariant((long)0));
			rsTZB.SetFieldValue(_T("CLzz"),COleVariant((long)0));
			mvarSATotalWeight = 0;
		}
		else
		{
			//�͸ָ���,����һ�ǿյ�����ֵ
			rsTZB.SetFieldValue(_T("sizeDIM"),COleVariant(SACSHeight));
			rsTZB.GetFieldValue(_T("index"),vTmp1);
			vTmp2=rsza->GetCollect(_T("bNotSumSA"));
			if( vtoi(vTmp1)== iSA && !vtob(vTmp2)){
				//��������Ҫͳ��
				rsTZB.SetFieldValue(_T("SEQ"),COleVariant((long)i));
				rsTZB.SetFieldValue(_T("CLdz"),COleVariant(mvarSATotalWeight));
				rsTZB.GetFieldValue(_T("CLNum"),vTmp1);
				rsTZB.SetFieldValue(_T("CLzz"),COleVariant(mvarSATotalWeight * vtof(vTmp1)));
			}
			else
			{
				//�������ϲ�Ҫͳ��
				//������������ţ���ֹ��д�������ϸ��
				rsTZB.SetFieldValue(_T("SEQ"),vnil);
				rsTZB.SetFieldValue(_T("CLNum"),COleVariant((long)0));
				rsTZB.SetFieldValue(_T("CLdz"),COleVariant((long)0));
				rsTZB.SetFieldValue(_T("CLzz"),COleVariant((long)0));
			}
		}
	}
	rsTZB.Update();
	
	}
	
	}
	if( modPHScal::g_bPipeClampBoltDiameterChanged && FirstCal ==1 && modPHScal::giWholeSprNum ==0 )
		//С���ش�ܾ��ܼа�����ѡ����˨��ֱ��M1��С���ܼй���б���ע����M1ֵ
	{
		CString strSQLTmp ;
		//�ܼ���˨M1��U�ζ���L8����˨M��������
		strSQLTmp.Format("SELECT * FROM [%s] WHERE [CUSTOMID]=\'%s\' AND [SIZE2]=%d ORDER BY SIZE2",
			modPHScal::tbnPART,modPHScal::sFindCustomID (_T("L8")),int(modPHScal::gmiDiameter));
		CDaoRecordset m_rs;
		COleVariant vT1;
		if(m_rs.IsOpen())
			m_rs.Close();
		
		m_rs.m_pDatabase=&modPHScal::dbZDJcrude;
		m_rs.Open(dbOpenDynaset,strSQLTmp);
		CString str;
		int x=0;
		if((!m_rs.IsBOF()) && !(m_rs.IsEOF()))
		{
			m_rs.GetFieldValue(_T("size3"),vT1);
			x = vtoi(vT1);
		}
		m_rs.Close ();
		
		strSQLTmp.Format(" [NTH]=1 AND recno=1 ");
		rsTZB.MoveFirst ();
		if(rsTZB.FindFirst(strSQLTmp))
		{
			rsTZB.Edit();
			rsTZB.GetFieldValue(_T("CLgg"),vT1);
			CString strTmp=vtos(vT1);			
		   //����ܼ���˨��M1��ֵ����Ҫ��L8����˨M��			   
		   if(m_fDiaM1 > x)
		   {
				strSQLTmp.Format("SELECT * FROM [%s] WHERE [CUSTOMID]=\'%s\' AND [SIZE2]=%d ORDER BY SIZE2",
					modPHScal::tbnPART,modPHScal::sFindCustomID (_T("L7")),int(modPHScal::gmiDiameter));
				m_rs.Close();
				m_rs.Open(dbOpenSnapshot,strSQLTmp);
				m_rs.GetFieldValue(_T("size3"),vT1);
				int y = vtoi(vT1);
				if(m_fDiaM1 > y)
				{
					//ͬʱҲ��L7���ڻ�ֱ��D��,
					//�򽫹ܼ���˨��С
					strSQLTmp.Format(", M1=%d ",x);
					strTmp = strTmp + strSQLTmp;
				}
				else
					modPHScal::g_bPipeClampBoltDiameterChanged = FALSE;
		   }
		   else
			   modPHScal::g_bPipeClampBoltDiameterChanged = FALSE;
			
			rsTZB.SetFieldValue(_T("CLgg"),STR_VAR(strTmp));
			rsTZB.Update();
		}
		
	}
	
	}
	
	catch(CString e)
	{
		ret=false;
		
		//�������Ĳ����ж���䲻ע�͵���ģ�����󳬹�64K���ܾ�����
		//���ڵڶ��Σ���ʽ������ʱ�������
		if(FirstCal == MaxCalCount)
			ShowMessage(e);
		
	}
	catch(::CDaoException * e)
	{
		ret=false;
		if(FirstCal == MaxCalCount)
			e->ReportError();
		e->Delete();
	}
	catch(_com_error e)
	{
		ret=false;
		if(FirstCal == MaxCalCount)
			//frmStatus.MessageBox(e.Description());
			ShowMessage(e.Description());
	}
	catch(CException *e)
	{
		e->Delete();
		ret=false;
	}
	catch(...)
	{
		ret=false;
	}

	if(rsTZB.IsOpen())
		rsTZB.Close();

	if(Ptype)
		delete [] Ptype;   //��������������ṹCustomID����
	if(PtypeID)
		delete [] PtypeID;   //��������������ṹID����
	if(PtypeClassID)
		delete [] PtypeClassID;   //��������������ṹClassID����
	if(PtypeIndex)
		delete [] PtypeIndex;
	FrmTxsr.UpdateData(false);
	return ret;
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
	CDaoRecordset rsX(&modPHScal::dbZDJcrude), rsTmp, rs(&modPHScal::dbZDJcrude);
	CDaoRecordset rsTmpZB(&EDIBgbl::dbPRJDB);
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
		rsTmpZB.m_pDatabase=&EDIBgbl::dbPRJDB;
		rsTmpZB.Open(dbOpenDynaset,SQLx);
		if( rsTmpZB.IsEOF() && rsTmpZB.IsBOF() ){
			strTmp.Format(GetResStr(IDS_NoThisZDJHResultInTBNSelPrjspecTZB),  EDIBgbl::dbPRJDB.GetName(), EDIBgbl::Btype[EDIBgbl::TZB], SQLx);
			throw strTmp;
		}
		//����������������������
		rsTmpZB.MoveLast();
		rsTmpZB.MoveFirst();
		lngCurrentPartNo = -1;
		IC = rsTmpZB.GetRecordCount();
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
			rsTmpZB.GetFieldValue(_T("CustomID"),vTmp1);
			NewPtypeCustomID[i] = vtos(vTmp1);
			rsTmpZB.GetFieldValue(_T("ID"),vTmp1);
			NewPtypeID[i] = vtos(vTmp1);
			rsTmpZB.GetFieldValue(_T("ClassID"),vTmp1);
			NewPtypeClassID[i] = vtoi(vTmp1);
			rsTmpZB.GetFieldValue(_T("index"),vTmp1);
			NewPtypeIndex[i] = vtoi(vTmp1);
			if( NewPtypeClassID[i] == iROD ){
				//����
				rsTmpZB.GetFieldValue(_T("recno"),vTmp1);
				rsTmpZB.GetFieldValue(_T("sizeC"),vTmp2);
				iLugDia[vtoi(vTmp1)] =vtoi(vTmp2); //����ֱ��������sizeC�ֶ�
				//iRecNoOfLugDia(iLugNo) = RsTZB.Fields(_T("recno"))   //��ʼ��Ϊ1
				lngCurrentPartNo++;
				//ReDim Preserve iRecNo(lngCurrentPartNo)
				//ReDim Preserve PtypeCustomID(lngCurrentPartNo)
				PtypeCustomID[lngCurrentPartNo] =NewPtypeCustomID[i];
				rsTmpZB.GetFieldValue(_T("recno"),vTmp1);
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
						rsTmpZB.GetFieldValue(_T("T"),vTmp);
						tmpT = vtoi(vTmp);
					}
					lngCurrentPartNo ++;
					//ReDim Preserve iRecNo(lngCurrentPartNo)
					//ReDim Preserve PtypeCustomID(lngCurrentPartNo)
					PtypeCustomID[lngCurrentPartNo]= NewPtypeCustomID[i];
					rsTmpZB.GetFieldValue(_T("recno"),vTmp);
					iRecNo[lngCurrentPartNo] = vtoi(vTmp);
					//��λ������ź������Ȧ���ֱ�־
					lngAttachedPartNo = -1;
					bFoundF4 = false;
					//����G12��G13
					if( NewPtypeID[i] == _T("G12") || NewPtypeID[i] == _T("G13") )
						bFoundG12G13 = true;
				}
			}
			rsTmpZB.MoveNext();
		}
		
		//��ʽ��ʼ
		rsTmpZB.MoveFirst();
		lngCurrentPartNo = -1;
		n = 0;
		miSEQofCSPR = -1;
		for (i = 0 ;i< IC;i++)
		{
			//�ҳ��������ɵ�CustomID,���ڲ��
			
			rsID.FindFirst (_T("trim(ID)=\'") + NewPtypeID[i] + _T("\'"));
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
					rsTmpZB.GetFieldValue(_T("sizeC"),vTmp);
					SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + vtos(vTmp);
					rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
					rsTmp.Open(dbOpenSnapshot,SQLx );
					if( rsTmp.IsEOF() && rsTmp.IsBOF() )
					{
						//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
						strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude.GetName(),modPHScal::tbnBoltsSurplusLength,SQLx);
						throw strTmp;
					}
					rsTmpZB.Edit();
					if( AttachmentGroupHavingF4[lngCurrentPartNo] )
					{
						//�����麬�����Ȧ(F4)
						rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2F4"),vTmp);
					}
					else
					{
						rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2"),vTmp);
					}
					rsTmpZB.SetFieldValue(_T("A"),COleVariant(vtof(vTmp)));
					//ͬʱ�������������С���ȱ������ֶ�C�����ڼ������˳��Ⱥ���CalLugLength����
					rsTmpZB.SetFieldValue(_T("C"),COleVariant(vtof(vTmp)));
					rsTmpZB.Update();
					rsTmp.Close();
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
					rsTmpZB.GetFieldValue(_T("sizeC"),vTmp);
					SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") +vtos(vTmp);
					rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
					rsTmp.Open(dbOpenSnapshot,SQLx);
					if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
						//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
						strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude.GetName(),modPHScal::tbnBoltsSurplusLength,SQLx);
						throw strTmp;
					}
					rsTmpZB.Edit();
					if( AttachmentGroupHavingF4[lngCurrentPartNo]){
						//�����麬�����Ȧ(F4)
						rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2F4"),vTmp);
					}
					else{
						rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2"),vTmp);
					}
					rsTmpZB.SetFieldValue(_T("B"),COleVariant(vtof(vTmp)));
					//ͬʱ�������������С���ȱ������ֶ�C�����ڼ������˳��Ⱥ���CalLugLength����
					rsTmpZB.SetFieldValue(_T("C"),COleVariant(vtof(vTmp)));
					rsTmpZB.Update();
					rsTmp.Close();
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
								rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
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
									rsTmp.Open(dbOpenSnapshot,SQLx);
									if( rsTmp.IsEOF() && rsTmp.IsBOF() )
									{
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										rsTmp.Close();
										if( !modPHScal::gbAutoApplyCSPRLugDia )
										{
											//�Զ�����������������ֱ��
											SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
											rsTmp.Open(dbOpenSnapshot,SQLx);
											if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
												//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
												strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(), modPHScal::tbnBoltsSurplusLength, SQLx);
												throw strTmp;
											}
										}
										else
										{
											strTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia),modPHScal::dbZDJcrude.GetName(),modPHScal::tbnBoltsSurplusLength,SQLx);
											throw strTmp;
										}
									}
									//if( True ){
									//�����˨�׾�FiK
									rsTmp.GetFieldValue(_T("FiK"),vTmp);
									iBoltsNutsDia[i] =vtoi(vTmp);
									//������˨ֱ��ϵ�б�tbnCSPRDiameterSerial,���׼ȷֱ��
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnCSPRDiameterSerial + _T("] WHERE Diameter<") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter DESC");
									rs.Open(dbOpenSnapshot,SQLx );
									if( rs.IsEOF() && rs.IsBOF() ){
										//û���ҵ�ƥ��ֱ��ϵ��ֵ
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal::tbnBoltsSurplusLength, SQLx);
										throw strTmp;
									}
									rs.GetFieldValue(_T("Diameter"),vTmp);
									iBoltsNutsDia[i] =vtoi(vTmp);
									rsTmp.GetFieldValue(_T("S"),vTmp1);
									rsTmp.GetFieldValue(_T("I"),vTmp2);
									LenBolts = vtoi(vTmp1) + 2 * vtoi(vTmp2);
									rsTmp.Close();
									//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter");
									rsTmp.Open(dbOpenSnapshot,SQLx);
									if( rsTmp.IsEOF() && rsTmp.IsBOF() )
									{
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude.GetName(),modPHScal::tbnBoltsSurplusLength, SQLx);
										throw strTmp;
									}
									if( AttachmentGroupHavingF4[lngCurrentPartNo] )
									{
										//�����麬��Ƭ(F4)
										rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2F4"),vTmp);
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
										rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2"),vTmp);
										if( NewPtypeID[i] == _T("F10") )
										{    //˫ͷ��˨
											LenBolts += 2 * vtof(vTmp);
										}
										else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") )
										{     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									rsTmp.Close();
									rs.Close();
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
									rsTmp.Open(dbOpenSnapshot,SQLx);
									if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										rsTmp.Close();
										if( ! modPHScal::gbAutoApplyCSPRLugDia )
										{
											//�Զ�����������������ֱ��
											SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
											rsTmp.Open(dbOpenSnapshot,SQLx);
											if( rsTmp.IsEOF() && rsTmp.IsBOF() )
											{
												//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
												strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude.GetName(), modPHScal::tbnBoltsSurplusLength,  SQLx);
												throw strTmp;
											}
										}
										else
										{
											strTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia),modPHScal::dbZDJcrude.GetName(),modPHScal::tbnBoltsSurplusLength, SQLx);
											throw strTmp;
										}
									}
									//�����˨�׾�FiK
									rsTmp.GetFieldValue(_T("FiK"),vTmp);
									rsTmp.Close();
									iBoltsNutsDia[i] = vtoi(vTmp);
									//������˨ֱ��ϵ�б�tbnCSPRDiameterSerial,���׼ȷֱ��
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnCSPRDiameterSerial + _T("] WHERE Diameter<") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter DESC");
									rs.Open(dbOpenSnapshot,SQLx);
									if( rs.IsEOF() && rs.IsBOF() )
									{
										//û���ҵ�ƥ��ֱ��ϵ��ֵ
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(), modPHScal::tbnBoltsSurplusLength, SQLx);
										throw strTmp;
									}
									rs.GetFieldValue(_T("Diameter"),vTmp);
									iBoltsNutsDia[i] = vtoi(vTmp);
									LenBolts =SACSHeight + tmpT;
									//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter");
									rsTmp.Open(dbOpenSnapshot,SQLx);
									if( rsTmp.IsEOF() && rsTmp.IsBOF() )
									{
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
										throw strTmp;
									}
									if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
										//�����麬��Ƭ(F4)
										rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2F4"),vTmp);
										if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
											LenBolts += 2 * vtof(vTmp);
										}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									else{
										rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2"),vTmp);
										if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
											LenBolts +=2*vtof(vTmp);
										}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									rsTmp.Close();
									rs.Close();
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
									rsTmp.Open(dbOpenSnapshot,SQLx);
									if( rsTmp.IsEOF() && rsTmp.IsBOF() )
									{
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
										throw strTmp;
									}
									//����ֱ���Ǳ�׼�ģ�����Ҫ����ϵ�б�
									rsTmp.GetFieldValue(_T("FiJ"),vTmp);iBoltsNutsDia[i]=vtoi(vTmp);
									LenBolts = Cavphs->SACSHeight + 15;
									//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
									SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]);
									rsTmp.Close();
									rsTmp.Open(dbOpenSnapshot,SQLx);
									if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
										//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
										strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
										throw strTmp;
									}
									if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
										//�����麬��Ƭ(F4)
										rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2F4"),vTmp);
										if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
											LenBolts+= 2 * vtof(vTmp);
										}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}else{
										rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2"),vTmp);
										if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
											LenBolts+= 2 * vtof(vTmp);
										}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
											LenBolts +=vtof(vTmp);
										}
									}
									rsTmp.Close();
								}
								
								//���ȣ�����˨��ĸ������Ҹ�������˨��ĸ
								
								tbn1 = modPHScal::tbnBoltsNuts;
								if( bCalBoltsDiaOfCSPR )
								{
									CString sID;
									rsTmpZB.GetFieldValue(_T("ID"),vTmp);
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
									rsX.Open(dbOpenSnapshot,SQLx);
									if( rsX.IsEOF() && rsX.IsBOF() )
									{
										//���������˨��ĸ����û�ҵ���������˨��ĸ
										rsTmpZB.GetFieldValue(_T("CustomID"),vTmp);
										strTmp.Format(GetResStr(IDS_NoFoundAttachInZDJCrude),modPHScal::dbZDJcrude.GetName(),SQLx,vtos(vTmp));
										ShowMessage(strTmp);
									}
									else
									{
										//����˨��ĸ�����ҵ���������˨��ĸ
										rsTmpZB.Edit();
										//������д��seq�ֶ�,��Ϊ���㺯��GetphsBHandSizes�Ѿ�ȷ������Щ����ǿ��Ա�ţ���Щ�����Ա�š�����Ҫ��ŵĶ������ա�
										//rsTmpZB.Fields(_T("SEQ")) = i
										if( NewPtypeID[i] == _T("F10") || NewPtypeID[i] == _T("F9") )
										{
											//˫ͷ��˨���ߵ�ͷ��˨
											rsX.GetFieldValue(_T("BH"),vTmp);
											EDIBgbl::dbPRJ.Execute(CString(_T("UPDATE tmpCSLen SET BH=\'")) + vtos(vTmp)+_T("\'"));
											rsTmpZB.SetFieldValue(_T("BH"),vTmp);
											EDIBgbl::dbPRJ.Execute(CString( _T("UPDATE tmpCSLen SET L1=")) + ltos(LenBolts));
											SQLx = _T("SELECT ") + vtos(modPHScal::sFindFLD(_T("ID"), _T("BHFormat"), NewPtypeID[i])) + _T(" AS sBH FROM tmpCSLen");
											rsTmp.m_pDatabase=&EDIBgbl::dbPRJ;
											rsTmp.Open(dbOpenSnapshot,SQLx);
											if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
												
											}
											else
											{
												rsTmp.GetFieldValue(_T("sBH"),vTmp);
												rsTmpZB.SetFieldValue(_T("CLgg"),vTmp);
												//												rsTmpZB.SetFieldValue(_T("BH"),vTmp);
												//˫ͷ��˨���ذ�1000mm���㱣����ԭʼ��������ֶ�
												rsX.GetFieldValue(_T("Weight"),vTmp1);
												rsTmpZB.GetFieldValue(_T("CLnum"),vTmp2);
												rsTmpZB.SetFieldValue(_T("CLdz"),COleVariant(vtof(vTmp1) / 1000 * LenBolts));
												rsTmpZB.SetFieldValue(_T("CLzz"),COleVariant(vtof(vTmp1) / 1000 * LenBolts * vtof(vTmp2)));
												if(NewPtypeID[i] == _T("F4"))
												{
													//һ�����ʹ��45��
													rsX.GetFieldValue(_T("Material"),vTmp1);
													rsTmpZB.SetFieldValue(_T("CLcl"),vTmp1);
												}
											}
											rsTmp.Close();
										}
										else
										{
											//����ͷ����׼����˨����ֱ������������ȡ��С��һ��
											//��ĸ��Ƭ
											rsX.GetFieldValue(_T("BH"),vTmp);
											rsTmpZB.SetFieldValue(_T("CLgg"),vTmp);
											rsTmpZB.SetFieldValue(_T("BH"),vTmp);
											rsX.GetFieldValue(_T("Weight"),vTmp1);
											rsTmpZB.GetFieldValue(_T("CLnum"),vTmp2);
											rsTmpZB.SetFieldValue(_T("CLdz"),vTmp1);
											rsTmpZB.SetFieldValue(_T("CLzz"),COleVariant(vtof(vTmp1)* vtof(vTmp2)));
											if(NewPtypeID[i] == _T("F4"))
											{
												//һ�����ʹ��45��
												rsX.GetFieldValue(_T("Material"),vTmp1);
												rsTmpZB.SetFieldValue(_T("CLcl"),vTmp1);
											}
										}
										rsTmpZB.Update();
									}
									rsX.Close();
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
									rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
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
										rsTmp.Open(dbOpenSnapshot,SQLx);
										if( rsTmp.IsEOF() && rsTmp.IsBOF() )
										{
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											rsTmp.Close();
											if( !modPHScal::gbAutoApplyCSPRLugDia )
											{
												//�Զ�����������������ֱ��
												SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
												rsTmp.Open(dbOpenSnapshot,SQLx);
												if( rsTmp.IsEOF() && rsTmp.IsBOF() )
												{
													//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
													strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
													throw strTmp;
												}
											}
											else
											{
												strTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx);
												throw strTmp;
											}
										}
										//�����˨�׾�FiK
										rsTmp.GetFieldValue(_T("FiK"),vTmp);
										iBoltsNutsDia[i]=vtoi(vTmp);
										//������˨ֱ��ϵ�б�tbnCSPRDiameterSerial,���׼ȷֱ��
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnCSPRDiameterSerial + _T("] WHERE Diameter<") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter DESC");
										rs.Open(dbOpenSnapshot,SQLx);
										if( rs.IsEOF() && rs.IsBOF() ){
											//û���ҵ�ƥ��ֱ��ϵ��ֵ
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										rs.GetFieldValue(_T("Diameter"),vTmp);iBoltsNutsDia[i]=vtoi(vTmp);
										rsTmp.GetFieldValue(_T("S"),vTmp1);rsTmp.GetFieldValue(_T("I"),vTmp2);
										LenBolts=vtof(vTmp1)+2*vtof(vTmp2);
										//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter");
										rsTmp.Close();
										rsTmp.Open(dbOpenSnapshot,SQLx);
										if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
											//�����麬��Ƭ(F4)
											rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2F4"),vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts += 2 * vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}
										else
										{
											rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2"),vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts += 2 * vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}
										rs.Close();
										rsTmp.Close();
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
										rsTmp.Open(dbOpenSnapshot,SQLx);
										if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
											rsTmp.Close();
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											if( !modPHScal::gbAutoApplyCSPRLugDia )
											{
												//�Զ�����������������ֱ��
												SQLx = _T("SELECT * FROM [") + modPHScal::tbnLugDiaOfCSPR + _T("] WHERE minDH<=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND maxDH>=") + ltos(modPHScal::sSprInfo[miSEQofCSPR].DH) + _T(" AND trim(CustomID)=\'") + PtypeCustomID[lngCurrentPartNo] + _T("\' ");
												rsTmp.Open(dbOpenSnapshot,SQLx);
												if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
													//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
													strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
													throw strTmp;
												}
											}else{
												strTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx);
												throw strTmp;
											}
										}
										//�����˨�׾�FiK
										rsTmp.GetFieldValue(_T("FiK"),vTmp);
										iBoltsNutsDia[i]=vtoi(vTmp);
										//������˨ֱ��ϵ�б�tbnCSPRDiameterSerial,���׼ȷֱ��
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnCSPRDiameterSerial + _T("] WHERE Diameter<") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY Diameter DESC");
										rs.Open(dbOpenSnapshot,SQLx);
										if( rs.IsEOF() && rs.IsBOF() ){
											//û���ҵ�ƥ��ֱ��ϵ��ֵ
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										rs.GetFieldValue(_T("Diameter"),vTmp);
										iBoltsNutsDia[i]=vtoi(vTmp);
										LenBolts = SACSHeight + tmpT;
										//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]);
										rsTmp.Close();
										rsTmp.Open(dbOpenSnapshot,SQLx);
										if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
											//�����麬��Ƭ(F4)
											rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2F4"),vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts +=2*vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}else{
											rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2"),vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts +=2*vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}
										rs.Close();
										rsTmp.Close();
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
										rsTmp.Open(dbOpenSnapshot,SQLx);
										if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										rsTmp.GetFieldValue(_T("FiJ"),vTmp);
										iBoltsNutsDia[i]=vtoi(vTmp);
										LenBolts = SACSHeight + 15;
										rsTmp.Close();
										//���ջ���Ժ֧�����ֲ����˨����ԣ������׼�������˨���ȡ�
										SQLx = _T("SELECT * FROM [") + modPHScal::tbnBoltsSurplusLength + _T("] WHERE Diameter=") + ltos(iBoltsNutsDia[i]);
										rsTmp.Open(dbOpenSnapshot,SQLx);
										if( rsTmp.IsEOF() && rsTmp.IsBOF() ){
											//û�ҵ���˨ֱ�����������ݱ����ݲ�ȫ��
											strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb), modPHScal::dbZDJcrude.GetName(),modPHScal:: tbnBoltsSurplusLength, SQLx); 
											throw strTmp;
										}
										if( AttachmentGroupHavingF4[lngCurrentPartNo] ){
											//�����麬��Ƭ(F4)
											rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2F4"),vTmp);
											if( NewPtypeID[i] == _T("F10") ){    //˫ͷ��˨
												LenBolts +=2*vtof(vTmp);
											}else if( NewPtypeID[i] == _T("F9") || NewPtypeID[i] == _T("F14") ){     //��ͷ��˨/����ͷ��˨
												LenBolts +=vtof(vTmp);
											}
										}else{
											rsTmp.GetFieldValue(_T("SurplusLengthWithF1F2"),vTmp);
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
										rsTmpZB.GetFieldValue(_T("ID"),vTmp);
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
										rsX.Open(dbOpenSnapshot,SQLx);
										if( rsX.IsEOF() && rsX.IsBOF() ){
											//���������˨��ĸ����û�ҵ���������˨��ĸ
											//Err.Number = iUE_NoFoundAttachInZDJCrude
											//Err.Description = ResolveResString(iUE_NoFoundAttachInZDJCrude, _T("|1"), dbZDJcrude.Name, _T("|2"), SQLx, _T("|3"), rsTmpZB.Fields(_T("CustomID")))
											//Err.Raise iUE_NoFoundAttachInZDJCrude
											rsTmpZB.GetFieldValue(_T("CustomID"),vTmp);
											strTmp.Format(GetResStr(IDS_NoFoundAttachInZDJCrude),modPHScal::dbZDJcrude.GetName(),SQLx,vtos(vTmp));
											ShowMessage(strTmp);
										}
										else
										{
											//����˨��ĸ�����ҵ���������˨��ĸ
											rsTmpZB.Edit();
											//������д��seq�ֶ�,��Ϊ���㺯��GetphsBHandSizes�Ѿ�ȷ������Щ����ǿ��Ա�ţ���Щ�����Ա�š�����Ҫ��ŵĶ������ա�
											//rsTmpZB.Fields(_T("SEQ")) = i
											if( NewPtypeID[i] == _T("F10") || NewPtypeID[i] == _T("F9") )
											{
												//˫ͷ��˨
												rsX.GetFieldValue(_T("BH"),vTmp);
												EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET BH=\'") + vtos(vTmp) + _T("\'"));
												rsTmpZB.SetFieldValue(_T("BH"),vTmp);
												EDIBgbl::dbPRJ.Execute(_T("UPDATE tmpCSLen SET L1=") + ltos(LenBolts));
												SQLx = _T("SELECT ") + vtos(modPHScal::sFindFLD(_T("ID"), _T("BHFormat"), NewPtypeID[i])) + _T(" AS sBH FROM tmpCSLen");
												rsTmp.m_pDatabase=&EDIBgbl::dbPRJ;
												rsTmp.Open(dbOpenSnapshot,SQLx);
												if( rsTmp.IsEOF() && rsTmp.IsBOF() )
												{
													strTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),EDIBgbl::dbPRJ.GetName(),_T("tmpCSLen"),SQLx);
													throw strTmp;
												}
												rsTmp.GetFieldValue(_T("sBH"),vTmp);
												rsTmpZB.SetFieldValue(_T("CLgg"),vTmp);
												rsTmpZB.SetFieldValue(_T("BH"),vTmp);
												//˫ͷ��˨���ذ�1000mm���㱣����ԭʼ��������ֶ�
												rsX.GetFieldValue(_T("Weight"),vTmp1);
												rsTmpZB.GetFieldValue(_T("CLnum"),vTmp2);
												rsTmpZB.SetFieldValue(_T("CLdz"),COleVariant(vtof(vTmp1) / 1000 * LenBolts));
												rsTmpZB.SetFieldValue(_T("CLzz"),COleVariant(vtof(vTmp1)/ 1000 * LenBolts * vtof(vTmp2)));
												rsTmp.Close();
												if(NewPtypeID[i] == _T("F4"))
												{
													//һ�����ʹ��45��
													rsX.GetFieldValue(_T("Material"),vTmp1);
													rsTmpZB.SetFieldValue(_T("CLcl"),vTmp1);
												}
											}
											else
											{
												//����ͷ����׼����˨����ֱ������������ȡ��С��һ��
												//��ĸ��Ƭ
												rsX.GetFieldValue(_T("BH"),vTmp);
												rsTmpZB.SetFieldValue(_T("CLgg"),vTmp);
												rsTmpZB.SetFieldValue(_T("BH"),vTmp);
												rsX.GetFieldValue(_T("Weight"),vTmp1);
												rsTmpZB.GetFieldValue(_T("CLnum"),vTmp2);
												rsTmpZB.SetFieldValue(_T("CLdz"),vTmp1);
												rsTmpZB.SetFieldValue(_T("CLzz"),COleVariant(vtof(vTmp1)*vtof(vTmp2)));
												if(NewPtypeID[i] == _T("F4"))
												{
													//һ�����ʹ��45��
													rsX.GetFieldValue(_T("Material"),vTmp1);
													rsTmpZB.SetFieldValue(_T("CLcl"),vTmp1);
												}
											}
											rsTmpZB.Update();
										}
										rsX.Close();
									}
								}
							}
						}
					}
					
					//�Ѿ������˨��ĸֱ��
					//�ڴ˿�ʼ������˨��ĸ��
					//д����(��˨����ĸ)����
					rsTmpZB.GetFieldValue(_T("ClassID"),vTmp);
					if( ! bCalBoltsDiaOfCSPR && (vtoi(vTmp) == iAttached || vtoi(vTmp) == iBolts || vtoi(vTmp) == iNuts) )
					{
						//���к����˨��ĸ�ļ�������������γ�����Ϊ��ʱbCalBoltsDiaOfCSPR=true
						//���ȣ�����˨��ĸ������Ҹ�������˨��ĸ
						tbn1 = modPHScal::tbnBoltsNuts;
						rsTmpZB.GetFieldValue(_T("CustomID"),vTmp);
						SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + vtos(vTmp) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]);
						
						rsX.Open(dbOpenSnapshot,SQLx);
						if( rsX.IsEOF() && rsX.IsBOF() ){
							//���������˨��ĸ����û�ҵ���������˨��ĸ
							//��ô���ڸ�������������Ҹ�������˨��ĸ
							rsX.Close();
							tbn1 = modPHScal::tbnAttachment;
							SQLx = _T("SELECT * FROM [") + tbn1 + _T("] WHERE Trim(CustomID)=\'") + vtos(vTmp) + _T("\' AND size2=") + ltos(iBoltsNutsDia[i]) + _T(" ORDER BY size2");
							rsX.Open(dbOpenSnapshot,SQLx);
							if( rsX.IsEOF() && rsX.IsBOF() ){
								//�ڸ�����������û�ҵ���������˨��ĸ
								//Err.Number = iUE_NoFoundAttachInZDJCrude
								//Err.Description = ResolveResString(iUE_NoFoundAttachInZDJCrude, _T("|1"), dbZDJcrude.Name, _T("|2"), SQLx, _T("|3"), rsTmpZB.Fields(_T("CustomID")))
								//Err.Raise iUE_NoFoundAttachInZDJCrude
								rsTmpZB.GetFieldValue(_T("CustomID"),vTmp);
								strTmp.Format(GetResStr(IDS_NoFoundAttachInZDJCrude),modPHScal::dbZDJcrude.GetName(),SQLx,vtos(vTmp));
								ShowMessage(strTmp);
							}
							else{
								//�ڸ������������ҵ���������˨��ĸ
								rsTmpZB.Edit();
								//������д��seq�ֶ�,��Ϊ���㺯��GetphsBHandSizes�Ѿ�ȷ������Щ����ǿ��Ա�ţ���Щ�����Ա�š�����Ҫ��ŵĶ������ա�
								//rsTmpZB.Fields(_T("SEQ")) = i
								rsX.GetFieldValue(_T("BH"),vTmp);
								rsTmpZB.SetFieldValue(_T("CLgg"),vTmp);
								rsTmpZB.SetFieldValue(_T("BH"),vTmp);
								rsX.GetFieldValue(_T("Weight"),vTmp1);
								rsTmpZB.GetFieldValue(_T("CLnum"),vTmp2);
								rsTmpZB.SetFieldValue(_T("CLdz"),vTmp1);
								rsTmpZB.SetFieldValue(_T("CLzz"),COleVariant(vtof(vTmp1)*vtof(vTmp2)));
								if(NewPtypeID[i] == _T("F4"))
								{
									//һ�����ʹ��45��
									rsX.GetFieldValue(_T("Material"),vTmp1);
									rsTmpZB.SetFieldValue(_T("CLcl"),vTmp1);
								}
								rsTmpZB.Update();
							}
						}
						else{
							//����˨��ĸ�����ҵ���������˨��ĸ
							rsTmpZB.Edit();
							//������д��seq�ֶ�,��Ϊ���㺯��GetphsBHandSizes�Ѿ�ȷ������Щ����ǿ��Ա�ţ���Щ�����Ա�š�����Ҫ��ŵĶ������ա�
							//rsTmpZB.Fields(_T("SEQ")) = i
							rsX.GetFieldValue(_T("BH"),vTmp);
							rsTmpZB.SetFieldValue(_T("CLgg"),vTmp);
							rsTmpZB.SetFieldValue(_T("BH"),vTmp);
							rsX.GetFieldValue(_T("Weight"),vTmp1);
							rsTmpZB.GetFieldValue(_T("CLnum"),vTmp2);
							rsTmpZB.SetFieldValue(_T("CLdz"),vTmp1);
							rsTmpZB.SetFieldValue(_T("CLzz"),COleVariant(vtof(vTmp1)*vtof(vTmp2)));
							if(NewPtypeID[i] == _T("F4"))
							{
								//һ�����ʹ��45��
								rsX.GetFieldValue(_T("Material"),vTmp1);
								rsTmpZB.SetFieldValue(_T("CLcl"),vTmp1);
							}
							rsTmpZB.Update();
						}
						rsX.Close();
					}
				}
				else{
					//�������ˣ�������˨��ĸ�򸽼�
					lngCurrentPartNo ++;
					lngAttachedPartNo = -1;
					//�������
					if( NewPtypeClassID[i] == iCSPR ){
						miSEQofCSPR ++;
						rsTmpZB.GetFieldValue(_T("gdw1"),vTmp);
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
							rsTmpZB.GetFieldValue(_T("CLgg"),vTmp);
							sBH =vtos(vTmp);
							//ע��˴�����M��ΪT5�ɽ���ĸֱ����־�ģ�����Ӧ������׼ʱ������������!
							k = sBH.Find(_T("M"));
							if( k !=-1 ){
								sBH = sBH.Left(k+1) + ltos(iBoltsNutsDia[i]);
								rsTmpZB.Edit();
								rsTmpZB.SetFieldValue(_T("CLgg"),STR_VAR(sBH));
								//								rsTmpZB.SetFieldValue(_T("BH"),STR_VAR(sBH));
								rsTmpZB.Update();
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
								rsTmpZB.Edit();
								rsTmpZB.SetFieldValue(_T("sizeH"),COleVariant(SACSHeight));
								rsTmpZB.Update();
							}
						}
					}
					//������ǰ����������ˣ�������˨��ĸ�򸽼�
				}
			}
			rsTmpZB.MoveNext();
		}
		
		//���¸�����������,���һ��recno��Ϊ�յļ�¼���Ǹ�����¼,
		//����,Ҫ�ų������ǻ��������������
		
		SQLx.Format("SELECT * FROM [ZB] WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND [Index]=%d ORDER BY recno",
			EDIBgbl::SelVlmID,modPHScal::zdjh,nth,iSA);
		if( rsTmpZB.IsOpen() )
			rsTmpZB.Close();
		rsTmpZB.Open(dbOpenDynaset,SQLx);

		if(rsTmpZB.IsEOF() && rsTmpZB.IsBOF())
		{
			return ;
		}
		else 
		{
			rsTmpZB.Edit();
			if( rsTmpZB.IsBOF() )
			{
				strTmp.Format(GetResStr(IDS_BOForEOFofmvarRsTZB), _T("BOF"));
				throw strTmp;
			}
			if( rsTmpZB.IsEOF() )
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
		rsTmpZB.GetFieldValue(_T("CustomID"),vTmp);
		
		if( !rsID.FindFirst(_T("trim(CustomID)=\'") + vtos(vTmp) + _T("\'")))
		{
			strTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(vTmp));
			ShowMessage(strTmp);
		}
		else
		{
			rsID.GetFieldValue(_T("ClassID"),vTmp);
			if( vtoi(vTmp)== iGCement )
			{
				COleVariant nul;
				nul.ChangeType(VT_NULL);
				rsTmpZB.SetFieldValue(_T("SEQ"),nul);
			}
			else
			{
				//�͸ָ���,����һ�ǿյ�����ֵ
				//���ɣ�GetphsBHandSizes�����Ѿ�ȷ����Щ�������Ա��
				//rsTmpZB.Fields(_T("SEQ")) = i
			}
		}
		rsTmpZB.SetFieldValue(_T("CLdz"),COleVariant(mvarSATotalWeight));
		rsTmpZB.GetFieldValue(_T("CLnum"),vTmp);
		rsTmpZB.SetFieldValue(_T("CLzz"),COleVariant(mvarSATotalWeight * vtof(vTmp)));
		rsTmpZB.Update();
		
		//���һ��ѭ��������˨��ĸֱ����ȷ��ʱ
	}
	catch(CString e)
	{
		ShowMessage(e);
	}
	catch(CDaoException * e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
	}

	if(rsTmpZB.IsOpen())
		rsTmpZB.Close();

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
		CDaoRecordset rs, rs1;
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
				rs.m_pDatabase=&EDIBgbl::dbPRJ;
				for (i = 0;i<= IC - 2;i++)
				{
					//����ÿһ������ƥ��
					FlgMatch = false;
					//�ӵ�һ����ʼ����ƥ��
					SQLx = _T("SELECT * FROM connect WHERE trim(cntb)=\'") + Ptype[i] + _T("\' AND  trim(cnte)=\'") + Ptype[i + 1] + _T("\'");
					rs.Open(dbOpenSnapshot,SQLx);
					if( rs.IsEOF() && rs.IsBOF() ){
						FlgMatch = false;
						break;
					}
					else if( ! rs.IsEOF() )
					{
						rs.MoveLast();
						rs.MoveFirst();
						for( j = 0 ;j<rs.GetRecordCount();j++)
						{
							//3����3�����ϵ����,ƥ���������ϸ�
							rs.GetFieldValue(_T("cntn"),vTmp);
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
							rs.MoveNext();
						}
						if( j >= rs.GetRecordCount() ){
							FlgMatch = false;
							break;
						}
					}
					
					//������û��ƥ��,���˳�
					if( ! FlgMatch ) break;
					rs.Close();
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
	catch(::CDaoException* e)
	{
		e->ReportError();
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
	CDaoRecordset rs1,rs;
	CString sTmp;
	try{
		EDIBgbl::SQLx = _T("SELECT * FROM phsStructureName");
		//ʹ��rs1�����GetphsAssembleName�е�ģ�鼶����rs��ͻ
		rs1.m_pDatabase=&EDIBgbl::dbPHScode; //20071101 "dbSORT" ��Ϊ "dbPHScode"
		rs1.Open(dbOpenDynaset,EDIBgbl::SQLx);
		EDIBgbl::SQLx = _T("SELECT * FROM phsStructureREF");
		rs.m_pDatabase=&EDIBgbl::dbPHScode; //20071101 "dbSORT" ��Ϊ "dbPHScode"
		rs.Open(dbOpenSnapshot,EDIBgbl::SQLx);
		
		COleVariant v;
		if(rs1.IsBOF()&& rs1.IsEOF())
		{
			sTmp=GetResStr(IDS_NoAnyRecordInphsStructureName)+GetResStr(IDS_NoAnyRecordInphsStructureName);
			ShowMessage(sTmp);
			goto errH;
		}
		if( !rs1.IsEOF())
		{
			rs1.MoveLast();
			rs1.MoveFirst();
			while(!rs1.IsEOF())
				//for (int i = 0 ;i<rs1.GetRecordCount();i++)
			{
				rs1.GetFieldValue(_T("SampleID"),v);
				sTmp.Format(_T("%d"),vtoi(v));
				sTmp=CString(_T("SampleID="))+sTmp+_T("");
				//��phsStructureREF��û�з���phsStructureName��
				//���ڵ�SampleID������,��ɾ��phsStructureName�е����SampleID��¼
				if(! rs.FindFirst(sTmp))
				{
					rs1.Delete();
				}
				rs1.MoveNext();
			}
		}
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
	}
errH:
	AfxGetApp()->EndWaitCursor();
	if(rs.IsOpen())
		rs.Close();
	if(rs1.IsOpen())
		rs1.Close();
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
		CDaoRecordset rs1;
		
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
		rs1.m_pDatabase=&EDIBgbl::dbPHScode;//20071101 "dbSORT" ��Ϊ "dbPHScode"
		rs1.Open(dbOpenDynaset,SQLx);
		CString DupRec;
		if(!rs1.IsEOF() && !rs1.IsBOF())
		{
			m=1;
			rs1.GetFieldValue(_T("SampleID"),vTmp);
			DupRec=vtos(vTmp);
		}
		else
			m=0;
		rs1.Close();
		
		//��m=0,�����ظ���¼
		//rs2.m_pDatabase=&EDIBgbl::dbSORT;
		if( m == 0 )
		{
			//��ȡ��δʹ�õ�SampleID��,��ֵ��Ϊ���µ�SampleID
			SQLx = _T("SELECT * FROM phsStructureREF WHERE Seq=0 ORDER BY SampleID");
			rs1.Open(dbOpenDynaset,SQLx);
			if( rs1.IsBOF() && rs1.IsEOF() )
			{
				//MsgBox ResolveResString(iUE_NoAnyRecordInphsStructureName)
				//Exit Function
				IC = 0;
			}
			else if( ! rs1.IsEOF() )
			{
				rs1.MoveLast();
				rs1.MoveFirst();
				for (k = 1 ;k<= rs1.GetRecordCount();k++)
				{
					rs1.GetFieldValue(_T("SampleID"),vTmp);
					IC =vtoi(vTmp);
					//�����¼������SampleID���ȣ�˵����������
					//���ȱ��SampleID�ſ���
					if( k != IC ) 
						break;
					rs1.MoveNext();
				}
				//���k > rs2.RecordCount,˵��SampleID������
				//���һ��SampleID��+1����
				if( k > rs1.GetRecordCount() )
				{
					IC = k - 1;
				}
				else
				{
					//���õ�SampleID-1=IC
					IC = IC - 2;
				}
			}
			rs1.Close();
			modPHScal::iSelSampleID = IC + 1;
			sTmp.Format(_T("%04d"),modPHScal::iSelSampleID);
			SQLx = _T("INSERT INTO phsStructureName (FREQUENCE,SampleID,BmpName,bAllowUse) VALUES (1,") + ltos(modPHScal::iSelSampleID) + _T(",\'") + sTmp + _T("\',-1)");
			//������빲��⣬�Ա㼯�й���
			EDIBgbl::dbPHScode.Execute(SQLx);//20071101 "dbSORT" ��Ϊ "dbPHScode"
			//ͬʱ���뱾�ؿ��У��Ա��������
			EDIBgbl::dbPRJ.Execute(SQLx);
			for( k = 0 ;k<SourceObj->GetCount();k++)
			{
				SourceObj->GetText(k,sTmp);
				SQLx = _T("INSERT INTO phsStructureREF (SampleID,SEQ,ID,CustomID,FREQUENCE) VALUES (")+
					ltos(modPHScal::iSelSampleID)+_T(",")+ltos(k)+_T(", \'") +modPHScal::sFindID(Trim(sTmp)) +_T("\' , \'") +Trim(sTmp) + _T("\' , 1)");
				//������빲��⣬�Ա㼯�й���
				EDIBgbl::dbPHScode.Execute(SQLx);//20071101 "dbSORT" ��Ϊ "dbPHScode"
				//ͬʱ���뱾�ؿ��У��Ա��������
				EDIBgbl::dbPRJ.Execute(SQLx);
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
				EDIBgbl::dbPHScode.Execute(SQLx);//20071101 "dbSORT" ��Ϊ "dbPHScode"
				//ͬʱ���뱾�ؿ��У��Ա��������
				EDIBgbl::dbPRJ.Execute(SQLx);
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

			sTmp.Format(GetResStr(IDS_ExistsThisSampleIDinSampleREF), EDIBgbl::dbSORT.GetName(), DupRec);  //ԭ

			SQLx.Format(_T("SELECT * FROM phsStructureName WHERE SampleID = %d"),_ttoi(DupRec));  
			rs1.Open(dbOpenDynaset,SQLx);			
			rs1.GetFieldValue(_T("DefaultFavoriteTemplate"),vTmp);
			bDefFavTem = vtob(vTmp);
			rs1.GetFieldValue(_T("Favorite"),vTmp);
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
	catch(::CDaoException* e)
	{
		ret=-1;
		e->ReportError();
		e->Delete();
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
	catch(...)
	{
		ret=-1;
	}
	return ret;
}
#pragma warning( disable : 4129 )  //Added by Shuli Luo
void Cphs::CheckAllCustomIDandIDinPhsStructureREF()
{
	//Ŀ�ģ�������е�CustomID�ֶΣ�����ת��ΪID�ֶΡ�����ID�ֶ���Ϊ�˱���PhsStructureREF����ģ��ŵ�Ψһ��/ͨ���ԡ�
	//���еĲ��ұ��־���IDΪ����.
	
	
	//��������գ������ٶȱȵ���modPHScal::sFindID��sFindCustomID�졣
	CDaoRecordset rs,rsX;
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
	if(!rsID.IsOpen())
	{
	EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
	rsID.m_pDatabase=&EDIBgbl::dbPRJ;
	rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
	brsIDStatus=TRUE;
	}
	
	  EDIBgbl::SQLx = _T("SELECT SampleID,seq,CustomID,ID  FROM phsStructureREF ORDER BY SampleID,seq");
	  rsX.m_pDatabase=&EDIBgbl::dbSORT;
	  rsX.Open( dbOpenDynaset ,EDIBgbl::SQLx);   
	  if( rsX.IsEOF() && rsX.IsBOF() ) 
	  {
	  sTmp=GetResStr(IDS_NoAnySEQeqZeroRecordInphsStructureREF);
	  ShowMessage(sTmp);
	  goto errH;
	  }
	  rsX.MoveLast();
	  int iC2 = rsX.GetRecordCount();
	  rsX.GetFieldValue(_T("SampleID"),v);
	  int iCSampleID =vtoi(v);
	  rsX.MoveFirst();
	  frmStatus.m_Label1=GetResStr(IDS_CheckAllCustomIDandIDinPhsStructureREF);
	  CString CustomID,ID;
	  for(int j = 0 ;j< iC2 ;j++)
	  {
	  AfxGetApp()->BeginWaitCursor();
	  //Screen.MousePointer = vbHourglass
	  //��������ʾһ��ʱ��
	  rsX.GetFieldValue(_T("CustomID"),v);
	  CustomID=vtos(v);
	  if(rsID.FindFirst (CString(_T("Trim(CustomID)=\'")) + CustomID + _T("\'")))
	  {
	  rsID.GetFieldValue(_T("ID"),v);
	  ID=vtos(v);
	  if (ID== _T("") )
	  {
	  rsX.GetFieldValue(_T("ID"),v);
	  ID=vtos(v);
	  if(rsID.FindFirst(CString(_T("Trim(ID)=\'")) + ID + _T("\'")))
	  //���Ϊ�գ�����Ȼ���ܴ�CustomIDת��ΪID,���������任��
	  {
						rsID.GetFieldValue(_T("CustomID"),v);
						CustomID=vtos(v);
						if( CustomID != _T("") )
						//��ȻΪ�գ�����һ������
						//msgbox
						{
						rsX.Edit();
						rsX.SetFieldValue(_T("CustomID"),v);
						rsX.Update();
						}
						}
						
						  //��ID-CustomID���ձ���û�з��������㲿��
						  //msgbox
						  }
						  else
						  {
						  rsID.GetFieldValue(_T("ID"),v);
						  rsX.Edit();
						  rsX.SetFieldValue(_T("ID"),v);
						  rsX.Update();
						  }
						  }
						  else
						  {
						  //���Ϊ�գ�����Ȼ���ܴ�CustomIDת��ΪID,���������任��
						  rsX.GetFieldValue(_T("ID"),v);
						  ID=vtos(v);
						  if(rsID.FindFirst( CString(_T("Trim(ID)=\'")) + ID + _T("\'")))
						  {
						  //���Ϊ�գ�����Ȼ���ܴ�CustomIDת��ΪID,���������任��
						  rsID.GetFieldValue(_T("CustomID"),v);
						  CustomID=vtos(v);
						  if(CustomID!=_T(""))
						  {
						  rsX.Edit();
						  rsX.SetFieldValue(_T("CustomID"),v);
						  rsX.Update();
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
						  rsX.GetFieldValue(_T("SampleID"),v);
						  sf1.Format(_T("%d"),vtoi(v));
						  rsX.GetFieldValue(_T("seq"),v);
						  sf2.Format(_T("%d"),vtoi(v));
						  sTmp1.Format(_T("%d"),iCSampleID);
						  stmp2.Format(_T("%d"),iC2);
						  //��%s��ģ��%s�����,��%s��ģ���%s�����CustomID=%s
						  frmStatus.m_Label2.Format(GetResStr(IDS_CheckNoXCustomIDandIDinPhsStructureREF),sTmp1,stmp2,sf1,sf2,CustomID);
						  frmStatus.UpdateStatus((float)(j+1)/(float)iC2,true);
						  frmStatus.UpdateData(false);
						  
							rsX.MoveNext();
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
	rsX.m_pDatabase=&EDIBgbl::dbPRJ;
	rsX.Open(dbOpenSnapshot,SQLx);
	if( rsX.IsEOF() && rsX.IsBOF() ) 
	{
		sTmp=GetResStr(IDS_NoAnySEQeqZeroRecordInphsStructureREF);
		ShowMessage(sTmp);
		goto errH;
	}
	else
	{
		frmStatus.m_Label1.Format(GetResStr(IDS_UpdateCustomIDinPhsStructureREF));
		
		rsX.MoveLast();
		int iC2=rsX.GetRecordCount();						
		for(int j=1;j<=iC2;j++)
		{
			rsX.GetFieldValue(_T("CustomID"),v);
			rsX.GetFieldValue(_T("ID"),vTmp);
			SQLx = _T("UPDATE phsStructureREF SET CustomID=\'") + vtos(v)+ _T("\' WHERE ID=\'") + vtos(vTmp)+_T("\'");
			EDIBgbl::dbPRJ.Execute(SQLx);
			EDIBgbl::dbPHScode.Execute(SQLx);//20071018 "dbSORT" ��Ϊ "dbPHScode"
			//�Ѿ��滻
			frmStatus.m_Label2=_T("");
			frmStatus.UpdateStatus((float)j/(float)iC2,true);
			frmStatus.UpdateData(false);
			
			
			rsX.MovePrev();
		}		
	}
}
catch(::CDaoException * e)
{
	e->ReportError();
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
	CDaoRecordset rs1;//rs1��rs2ȡ��ͬһ����phsStructureREF
	CDaoRecordset rs2;
	CDaoRecordset rs3;
	CDaoRecordset rs4;
	rs1.m_pDatabase=&EDIBgbl::dbPRJ;
	rs2.m_pDatabase=&EDIBgbl::dbPRJ;
	rs3.m_pDatabase=&EDIBgbl::dbPRJ;
	rs4.m_pDatabase=&EDIBgbl::dbPHScode;//20071018 "dbSORT" ��Ϊ "dbPHScode"
	//�򿪱���
	CDaoRecordset rsPA(&EDIBgbl::dbPRJ);
	CDaoRecordset rsSA(&EDIBgbl::dbPRJ);
	CDaoRecordset rsPR(&EDIBgbl::dbPRJ);
	CDaoRecordset rsSP(&EDIBgbl::dbPRJ);
	CDaoRecordset rsCS(&EDIBgbl::dbPRJ);
	rsPA.Open(dbOpenTable,_T("tmpCustomIDPA"));
	rsSA.Open(dbOpenTable,_T("tmpCustomIDSA"));
	rsPR.Open(dbOpenTable,_T("tmpCustomIDPART"));
	rsSP.Open(dbOpenTable,_T("tmpCustomIDSPR"));
	rsCS.Open(dbOpenTable,_T("tmpCustomIDCSPR"));
	rsPA.SetCurrentIndex(_T("Index1"));
	rsSA.SetCurrentIndex(_T("Index1"));
	rsPR.SetCurrentIndex(_T("Index1"));
	rsSP.SetCurrentIndex(_T("Index1"));
	rsCS.SetCurrentIndex(_T("Index1"));

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
	long t2;
	t1=::GetTickCount();		
	//2002.01.03����ʱ��31084ms<12413+55730ms,���ۺ���һ���ѯ�Ͽ�
	
	//Ԥ�����õ�ǰ�淶���õ�ģ��Ϊȫ�������ã�ֻҪ������һ�������bAllowUse=0����Ϊ������
	strSQL=_T("UPDATE PhsStructureName SET bAllowUse=0");
	EDIBgbl::dbPHScode.Execute(strSQL);//20071018 "dbSORT" ��Ϊ "dbPHScode"



	if(!rsID.IsOpen())
	{
		EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
		//if(Cavphs->rsID==NULL)
		//Cavphs->rsID.CreateInstance(__uuidof(Recordset));
		//Cavphs->rsID->CursorLocation=adUseClient;
		//Cavphs->rsID->Open(_variant_t(EDIBgbl::SQLx),(IDispatch*)EDIBgbl::dbPRJ,adOpenDynamic,adLockOptimistic,adCmdText);
		rsID.m_pDatabase=&EDIBgbl::dbPRJ;
		rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
		brsIDStatus=TRUE;
	}	
	try
	{
		while(!rsID.IsEOF())
		{
			rsID.GetFieldValue(_T("ID"), vTmp);
			vecID.push_back(vTmp);
			rsID.GetFieldValue(_T("CustomID"), vTmp);
			vecCustomID.push_back(vTmp);
			rsID.GetFieldValue(_T("Description"), vTmp);
			vecDescription.push_back(vtos2(vTmp));
			rsID.GetFieldValue(_T("ClassID"), vTmp);
			vecClassID.push_back(vtoi2(vTmp));
			rsID.MoveNext();	
		}


		EDIBgbl::SQLx=_T("SELECT SampleID FROM phsStructureREF WHERE SEQ=0");
		rs2.Open(dbOpenSnapshot,EDIBgbl::SQLx);
		rs2.MoveLast();
		rs2.MoveFirst();
		IC2=rs2.GetRecordCount();
		rs2.Close();

		EDIBgbl::SQLx = _T("SELECT * FROM phsStructureREF ORDER BY SampleID ASC, SEQ ASC");
		rs2.Open(dbOpenDynaset ,EDIBgbl::SQLx);
		CString strSQL = _T("select * from phsStructureNAME");
		rs4.Open(dbOpenDynaset ,strSQL);
		bool stop=false;
		int iNum=0;
		int iTimes=-1;//���㲿�����ñ�־�����˻�
		if( rs2.IsEOF() && rs2.IsBOF() )
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
			rs3.Open(dbOpenDynaset ,EDIBgbl::SQLx);
			int fdL;
			CDaoFieldInfo info;
			rs3.GetFieldInfo(_T("SampleName"),info);
			fdL=info.m_lSize;	
			sSPR = _T(" ");
			//�ƶ����ṹ��ͷ
			rs2.MoveFirst();
			while(!rs2.IsEOF())
			{
				AfxGetApp()->BeginWaitCursor();
				rs2.GetFieldValue( _T("SampleID"),vTmp);
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
					rs2.MoveNext();
					if(rs2.IsEOF())
					{
						stop=true;
					}
					else
					{
						rs2.GetFieldValue( _T("SampleID"),vTmp);
						SampleID =vtoi( vTmp);
						if(SampleID!=oldSampleID)
							stop=true;
					}
					rs2.MovePrev();

					rs2.GetFieldValue( _T("SampleID"),vTmp);
					SampleID =vtoi( vTmp);
					
					rs2.GetFieldValue(_T("ID"),vTmp);
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

							if(rsPA.Seek(_T("="),&vecCustomID[iPos]))
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
						rs2.MoveNext();
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
								if(rsSA.Seek(_T("="),&vecCustomID[iPos]))
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
									if(rsSP.Seek(_T("="),&vecCustomID[iPos])) //��ǰ��׼����������� 
										iTimes =iTimes*(-1);
									else
										//��ǰ��׼��û���������
										iTimes =0;
								}
								else if(ix==iCSPR)
								{
									iNumCSPR++;
									if(rsCS.Seek(_T("="),&vecCustomID[iPos]))
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
									if(rsPR.Seek(_T("="),&vecCustomID[iPos]))										//��ǰ��׼����������Ӽ�
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
							rs2.MoveNext();
							continue;
						}
					}

				}while(!stop);

				
				//��ʼ��ģ������
				//������ģ�������е�ʹ��Ƶ����Ϣ

				EDIBgbl::SQLx=_T("SampleID=")+sTmp1;
				
				if(!rs3.FindFirst(EDIBgbl::SQLx))
				{
					rs3.AddNew();

					rs3.SetFieldValue(_T("SampleID"),_variant_t(SampleID));
					sTmp1.Format(_T("%05d"),SampleID);
					rs3.SetFieldValue(_T("BmpName"),STR_VAR(sTmp1));					

					if(strTmpName.GetLength() > fdL)						
						strTmp=strTmpName.Left(fdL);
					else
						strTmp=strTmpName;

					rs4.AddNew();
					rs4.SetFieldValue(_T("SampleID"), COleVariant(SampleID));
					rs4.SetFieldValue(_T("BmpName"), COleVariant(sTmp1, VT_BSTRT));
					rs4.SetFieldValue(_T("SampleName"), COleVariant(strTmp, VT_BSTRT));
					rs4.SetFieldValue(_T("PA"), COleVariant(Ptype0, VT_BSTRT));
					rs4.SetFieldValue(_T("SA"), COleVariant(PtypeC, VT_BSTRT));
					rs4.SetFieldValue(_T("SPR"), COleVariant(sSPR, VT_BSTRT));
					rs4.SetFieldValue(_T("iNumOfPart"), COleVariant((long)iNum));
					rs4.SetFieldValue(_T("iNumSPR"), COleVariant(iNumSPR));
					rs4.SetFieldValue(_T("iNumCSPR"), COleVariant(iNumCSPR));
					rs4.SetFieldValue(_T("iNumRod"), COleVariant(iNumRod));
					rs4.SetFieldValue(_T("bAllowUse"), COleVariant((long)iTimes));
					rs4.Update();

				}
				else
				{
					rs3.Edit();

					rs4.FindFirst(EDIBgbl::SQLx);

					if(strTmpName.GetLength() > fdL)
						strTmp=strTmpName.Left(fdL);
					else
						strTmp=strTmpName;
					rs4.Edit();
					rs4.SetFieldValue(_T("SampleName"), COleVariant(strTmp, VT_BSTRT));
					rs4.SetFieldValue(_T("PA"), COleVariant(Ptype0, VT_BSTRT));
					rs4.SetFieldValue(_T("SA"), COleVariant(PtypeC, VT_BSTRT));
					rs4.SetFieldValue(_T("SPR"), COleVariant(sSPR, VT_BSTRT));
					rs4.SetFieldValue(_T("iNumOfPart"), COleVariant((long)iNum));
					rs4.SetFieldValue(_T("iNumSPR"), COleVariant(iNumSPR));
					rs4.SetFieldValue(_T("iNumCSPR"), COleVariant(iNumCSPR));
					rs4.SetFieldValue(_T("iNumRod"), COleVariant(iNumRod));
					rs4.SetFieldValue(_T("bAllowUse"), COleVariant((long)iTimes));
					rs4.Update();

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
						rs3.SetFieldValue(_T("SampleName"),STR_VAR(strTmp)); 
					}
					if(Ptype0.GetLength()>0)
						rs3.SetFieldValue(_T("PA"), STR_VAR(Ptype0));
					if(PtypeC.GetLength()>0)
						rs3.SetFieldValue(_T("SA"),STR_VAR(PtypeC));
					if(sSPR.GetLength()>0)
						rs3.SetFieldValue(_T("SPR"),STR_VAR( sSPR));
					rs3.SetFieldValue(_T("iNumOfPart"),_variant_t((long)iNum));
					rs3.SetFieldValue(_T("iNumSPR"),_variant_t(iNumSPR));
					rs3.SetFieldValue(_T("iNumCSPR"),_variant_t(iNumCSPR));
					rs3.SetFieldValue(_T("iNumRod"),_variant_t(iNumRod));
					//ģ���Ƿ�����ʹ��
					rs3.SetFieldValue(_T("bAllowUse"),_variant_t((bool) iTimes));
					rs3.Update();
//					EXECUTE_TIME_END
				}     
				sTmp1.Format(GetResStr(IDS_ChangeNoXNameInphsStructureName),(_T("\%d")),(_T("\%d")),strTmpName);
				frmStatus.m_Label2.Format(sTmp1, IC2, oldSampleID);
				frmStatus.UpdateData(false);
				frmStatus.UpdateStatus( (float)oldSampleID/(float)IC2, TRUE);
				frmStatus.UpdateWindow();
				rs2.MoveNext();
			}
			rs2.Close();
			rs3.Close();
			rs4.Close();
		}
	}
catch(CDaoException * e)
{
	e->ReportError();
	e->Delete();
}
//errH:
AfxGetApp()->EndWaitCursor();
if(rs2.IsOpen())
rs2.Close();
if(rs3.IsOpen())
rs3.Close();
//	timee=CTime::GetCurrentTime();
//CTimeSpan t=timee-timeb;
//CString s=t.Format(_T("%M��%S��"));
//s=_T("����ʱ:")+s;
//ShowMessage(s);
frmStatus.ShowWindow(SW_HIDE);
}


#pragma warning( disable : 4129 )  //Added by Shuli Luo
CDaoRecordset* Cphs::AvailableSampleIDrsForphsSamp(CString strOrderByDesc,CString strFilter)
{
	//������frmphsSamp��������ݿؼ�ѡ������
	CString tmpSQL;
	static CDaoRecordset ret;
	if(ret.IsOpen())
		ret.Close();
	ret.m_strFilter=_T("");
	ret.m_strSort=_T("");
	ret.m_pDatabase=&EDIBgbl::dbPRJ;
	tmpSQL=CString((_T("SELECT * FROM phsStructureName ")));
	if(Trim(strFilter)!=_T(""))
		tmpSQL +=_T("WHERE ") + strFilter ;
	if(Trim(SortFieldName) !=_T(""))
		tmpSQL+=CString((_T(" ORDER BY "))) + (_T("  [")) + Trim(SortFieldName) + (_T("] ")) + strOrderByDesc;
	ret.Open(dbOpenDynaset,tmpSQL);
	return & ret;
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
		CDaoRecordset rs(&EDIBgbl::dbPHScode);//20071018 "dbSORT" ��Ϊ "dbPHScode"
		//���Ϲ淶ѡ���SpecificationOfMaterial
		CString SQLx ;
		SQLx.Format(_T("SELECT * FROM SpecificationOfMaterial WHERE ClassIndex=%d AND ID=\'default\' AND tmin<=%g AND %g<tmax ORDER BY tmin,tmax,SEQ"),modPHScal::giClassIndex,modPHScal::gnConnectTJ,modPHScal::gnConnectTJ);
		rs.Open(dbOpenSnapshot,SQLx);
		if(rs.IsEOF() && rs.IsBOF())
		{
			//��%s��%s����ѡ��淶��û��%s�ֶ�ֵΪĬ��ֵ%s�ļ�¼(�ǹܲ��������ѡ�����)
			sTmp.Format(IDS_NoDefaultInCustomIDInSpecificationOfMaterial,rs.m_pDatabase->GetName(),_T("SpecificationOfMaterial"),_T("ID"),_T("default"));
			throw sTmp;
		}
		rs.MoveFirst();
		//���в��ǹܲ�����Ų�Ϊnull���Ҳ�������ֵ�����,����϶����ɻ��������¶Ȳ��
		//�û���ӵĲ���bUserAdd!=0�����Ϊnull
		SQLx=CString(_T("UPDATE ["))  
			+ EDIBgbl::Btype[EDIBgbl::TZB] 
			+ _T("] SET CLcl=\'");
		rs.GetFieldValue(_T("Material"),vTmp);
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
		EDIBgbl::dbPRJDB.Execute(SQLx); 
	}
	catch(CDaoException *e)
	{
		e->ReportError();
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
		CDaoRecordset rs, rsTmp2,rsTmpZB;
		CDaoRecordset rsCL, rsTmpCLgroup;
		long i;
		//		bool bFoundSA;// As Boolean
		long mvIndex =0; //������
		CString tbn , tmpBlkID , tmpID ;
		CString mvCL, sGROUPBY ,tmpStr;
		CString *Ptype=NULL;
		//		float sTmp=0, sngSumWeight=0;
		
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")))
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
		//����һ���ձ�
		sGROUPBY = _T("CLgg,CLcl,CLmc,CLID,CLdw,CLdz");
		EDIBgbl::dbPRJDB.Execute(_T("SELECT ") + sGROUPBY + _T(" INTO TmpCLgroup IN \'") + EDIBgbl::dbPRJ.GetName() + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=-1"));
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�,
		//����seq,CLnum���ܱ�����,��Ҫ�������seq,CLnum�ֶε���ʱ��
		EDIBgbl::dbPRJ.Execute(_T("ALTER TABLE TmpCLgroup ADD COLUMN seq short"));
		EDIBgbl::dbPRJ.Execute(_T("ALTER TABLE TmpCLgroup ADD COLUMN CLnum SINGLE"));
		//���򡢷���ͳ�������
		EDIBgbl::dbPRJDB.Execute (_T("INSERT INTO TmpCLgroup IN \'") + EDIBgbl::dbPRJ.GetName() + _T("\' SELECT DISTINCT ") + sGROUPBY + _T(" FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(modPHScal::zdjh) + _T(" AND NOT ISNULL(seq)  GROUP BY ") + sGROUPBY);
		
		sGROUPBY = _T("seq,CLgg,CLcl,CLmc,CLID,CLdw,CLdz,CLnum,CLzz,CLbz");
		
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("Tmp2")))
			EDIBgbl::dbPRJ.Execute(_T("DROP TABLE tmp2"));
		CString SQLx;
		//�����ձ�tmp2
		SQLx = _T("SELECT ") + sGROUPBY + _T(" INTO tmp2 IN \'") + EDIBgbl::dbPRJ.GetName() + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=-1");
		EDIBgbl::dbPRJDB.Execute( SQLx);
		SQLx = _T("DELETE FROM tmp2");
		EDIBgbl::dbPRJ.Execute(SQLx);
		rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
		rsTmp2.Open(dbOpenDynaset,_T("SELECT * FROM tmp2") );
		
		//��֧���ܲ��ϻ��ܹ���ת�Ƶ������Ĺ�����GetPhsBOM���Ա����ͳ�Ƶ�ѡ��ı�֮���ܹ���ȷ���ɲ��ϻ��ܱ�
		//����ɾ����֧���ܲ�������
		SQLx = _T("DELETE * FROM [") +EDIBgbl:: Btype[EDIBgbl::TCL] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
		EDIBgbl::dbPRJDB.Execute(SQLx);
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
		rsCL.m_pDatabase=&EDIBgbl::dbPRJDB;
		rsCL.Open(dbOpenDynaset,SQLx);
		
		//���Ҵӹܲ����������θ�������
		//ͬʱ���������ϸ��tmp2�Ͳ��ϻ��ܱ�F????S-JCL
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND NOT ISNULL(seq) ORDER BY recno");
		if(rsTmpZB.IsOpen())
			rsTmpZB.Close();
		rsTmpZB.m_pDatabase=&EDIBgbl::dbPRJDB;
		rsTmpZB.Open(dbOpenDynaset,SQLx);
		if(rsTmpZB.IsEOF() && rsTmpZB.IsBOF())
		{
			//����ṹ��û���κ����
			tmpStr.Format(GetResStr(IDS_NoRecordInTZB),EDIBgbl::dbPRJDB.GetName(),EDIBgbl::Btype[EDIBgbl::TZB],
				EDIBgbl::SelJcdm,ltos(modPHScal::zdjh));
			throw tmpStr;
		}
		i = 1;
		COleVariant vTmp;
		rsTmpCLgroup.m_pDatabase=&EDIBgbl::dbPRJ;
		COleVariant nil;
		nil.ChangeType(VT_NULL);
		while(!rsTmpZB.IsEOF())
		{
			CString CLgg,CLcl,CLmc;
			rsTmpZB.GetFieldValue(_T("CLgg"),vTmp);
			CLgg=vtos(vTmp);
			rsTmpZB.GetFieldValue(_T("CLcl"),vTmp);
			CLcl=vtos(vTmp);
			rsTmpZB.GetFieldValue(_T("CLmc"),vTmp);
			CLmc=vtos(vTmp);
			//�յ�seq�ֶΣ�д���Ÿ���
			SQLx=_T("UPDATE tmpCLgroup SET seq=") + ltos(i) + _T(" WHERE ISNULL(seq) AND trim(CLgg)=\'");
			SQLx+=CLgg+_T("\' AND trim(CLcl)=\'");
			SQLx+=CLcl+_T("\' AND (trim(CLmc))=\'") ;
			SQLx+=CLmc+_T("\'");
			EDIBgbl::dbPRJ.Execute(SQLx);
			//Debug.Print dbPRJ.RecordsAffected, i, rsTmpZB(_T("CLmc")), rsTmpZB(_T("CLgg")), rsTmpZB(_T("CLcl"))
			//SQLx = _T("SELECT seq FROM tmpCLgroup INNER JOIN [") + EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] AS tbn ON  WHERE ISNULL(seq) AND trim(CLgg)=\'")
			//	 +CLgg + _T("\' AND trim(CLcl)=\'") +CLcl + _T("\' AND (trim(CLmc))=\'") +CLmc+ _T("\' ");
			// SQLx = _T("SELECT * FROM tmpCLgroup WHERE NOT ISNULL(seq) AND trim(CLgg)='") & Trim(rsTmpZB.Fields(_T("CLgg"))) & _T("' AND trim(CLcl)='") & Trim(rsTmpZB.Fields(_T("CLcl"))) & _T("' AND (trim(CLmc))='") & Trim(rsTmpZB.Fields(_T("CLmc"))) & _T("' ")
			SQLx = _T("SELECT seq FROM tmpCLgroup WHERE NOT ISNULL(seq) AND trim(CLgg)=\'")
				+ CLgg+ _T("\' AND trim(CLcl)=\'") +CLcl+ _T("\' AND (trim(CLmc))=\'") +CLmc+ _T("\' ");
			rsTmpCLgroup.Open(dbOpenSnapshot,SQLx);
			if( rsTmpCLgroup.IsBOF() && rsTmpCLgroup.IsEOF())
			{
				tmpStr.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),EDIBgbl::dbPRJ.GetName(),_T("tmpCLgroup"),SQLx);
				throw tmpStr;
			}
			rsTmpCLgroup.MoveLast();
			//�м�¼�ܵ�Ӱ�죬��ŵ�����
			if(EDIBgbl::dbPRJ.GetRecordsAffected()>0)//�������һ�θ��¡��༭�����Ӳ�����Execute������Ӱ��ļ�¼��
				i++;
			rsTmpZB.GetFieldValue(_T("CustomID"),vTmp);
			if(vtoi(vTmp)!= iGCement)
			{
				//���ǻ�������
				//�������е������д����������,����������Ϊ�յ��������Ҫ���
				mvIndex = vtoi(modPHScal::sFindFLD(_T("CustomID"), _T("Index"), vtos(vTmp)));
				rsTmpZB.Edit();
				rsTmpCLgroup.GetFieldValue(_T("seq"),vTmp);
				if(mvIndex != iSA)
				{
					//���Ǹ���
					rsTmpZB.SetFieldValue(_T("seq"),vTmp);
				}
				else
				{
					if(!vtob(rsza->GetCollect(_T("bNotSumSA"))))
					{
						//�Ǹ���,Ҫͳ�Ƹ�������
						rsTmpZB.SetFieldValue(_T("seq"),vTmp);
					}
					else
						//�Ǹ���,��Ҫͳ�Ƹ�������
						rsTmpZB.SetFieldValue(_T("seq"),nil);
				}
				rsTmpZB.Update();
				
				//д�����ϸ��Tmp2
				rsTmp2.AddNew();
				rsTmpZB.GetFieldValue(_T("seq"),vTmp);
				rsTmp2.SetFieldValue(_T("seq"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLID"),vTmp);
				rsTmp2.SetFieldValue(_T("CLID"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLmc"),vTmp);
				rsTmp2.SetFieldValue(_T("CLmc"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLgg"),vTmp);
				rsTmp2.SetFieldValue(_T("CLgg"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLcl"),vTmp);
				rsTmp2.SetFieldValue(_T("CLcl"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLdw"),vTmp);
				rsTmp2.SetFieldValue(_T("CLdw"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLdz"),vTmp);
				rsTmp2.SetFieldValue(_T("CLdz"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLnum"),vTmp);
				rsTmp2.SetFieldValue(_T("CLnum"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLzz"),vTmp);
				rsTmp2.SetFieldValue(_T("CLzz"),vTmp);
				rsTmpZB.GetFieldValue(_T("CLbz"),vTmp);
				rsTmp2.SetFieldValue(_T("CLbz"),vTmp);
				rsTmp2.Update();
			}
			rsTmpZB.MoveNext();
			rsTmpCLgroup.Close();
		}
		}
		catch(CDaoException * e)
		{
			e->ReportError();
			e->Delete();
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
		CDaoRecordset rs , rsza, rsCL ;
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
		EDIBgbl::dbPRJDB.Execute(SQLx);
		//ȷ��Ҫ������ֶ�:����ֶ���Ϣ���Դ�TableINFO���t??����,���ҿ����û���
		sGROUPBY = _T("seq,CLgg,CLcl,CLmc,CLID,CLdw,CLdz,CLbz");
		//��dbprj�н�����ʱ������ϻ��ܱ�
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpTCL")) )
		{
			//WorkPrj.mdb�еı�TmpTCL���ڱ��������ܵ��м���
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpTCL"));
		}
		//����TCL��TmpTCL���Ա����ɾ�����е�ѡ����Ĳ��ϡ�
		SQLx = _T("SELECT * INTO TmpTCL IN \'") + EDIBgbl::dbPRJ.GetName() + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] WHERE VolumeID=-1");
		EDIBgbl::dbPRJDB.Execute( SQLx);
		if(!rsID.IsOpen())
		{
			EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
			rsID.m_pDatabase=&EDIBgbl::dbPRJ;
			rsID.Open(dbOpenDynaset,EDIBgbl::SQLx);
			brsIDStatus=TRUE;
		}
		//��dbprj�����ɿյ�Tmp2��
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("Tmp2")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE Tmp2"));
		}
		CString strOrderBy;
		strOrderBy = sGROUPBY;
		//���ɿյ�Tmp2��
		EDIBgbl::dbPRJDB.Execute( CString(_T("SELECT ")) + sGROUPBY + _T(",CLnum,CLnum*CLdz AS CLzz INTO Tmp2 IN \'") + EDIBgbl::dbPRJ.GetName() + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] WHERE zdjh=0 AND VolumeID=-1"));
		rs.m_pDatabase=&EDIBgbl::dbPHScode;//20071101 "dbSORT" ��Ϊ "dbPHScode"
		rs.Open(dbOpenSnapshot,_T("SELECT * FROM BomName ORDER BY BomIndex"));
		COleVariant vTmp,vTmp1,vTmp2;
		while(!rs.IsEOF())
		{
			rs.GetFieldValue(_T("BomTable"),vTmp);
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
						EDIBgbl::dbPRJ.Execute( _T("DROP TABLE [") + strTmp + _T("]"));
					}
					//����tmp2�Ľṹ��������
					EDIBgbl::dbPRJ.Execute( _T("SELECT * INTO [") + strTmp + _T("] FROM Tmp2"));
				}
			}
			rs.MoveNext();
		}
		rs.Close();   
		
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
		EDIBgbl::dbPRJDB.Execute( SQLx);
		
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
		EDIBgbl::dbPRJDB.Execute( SQLx);
		
		//��ʽ��ʼ���ܲ���
		SQLx = _T("SELECT * FROM TmpTCL");
		rsCL.m_pDatabase=&EDIBgbl::dbPRJ; 
		rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
		
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
		rs.m_pDatabase=&EDIBgbl::dbPRJDB;
		rs.Open(dbOpenDynaset,SQLx );
		if( rs.IsEOF() && rs.IsBOF() )
		{
			
		}
		else
		{
			rs.MoveLast();
			iSum+=rs.GetRecordCount();
		}
		rs.Close();
		
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;
		rs.Open(dbOpenDynaset,SQLx );
		if( rs.IsEOF() && rs.IsBOF() ) 
		{	}
		else
		{
			rs.MoveLast();
			iSum+=rs.GetRecordCount();
		}
		rs.Close();
		
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{	}
		else
		{
			rs.MoveLast();
			iSum+=rs.GetRecordCount();
		}
		rs.Close();
		
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() ) {	}
		else
		{
			rs.MoveLast();
			iSum+=rs.GetRecordCount();
		}
		rs.Close();
		
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{	}
		else
		{
			rs.MoveLast();
			//����Ҫ����2��
			iSum+=2*rs.GetRecordCount();
		}
		rs.Close();
		
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{	}
		else
		{
			rs.MoveLast();
			//�͸�Ҫ����3��
			iSum+=3*rs.GetRecordCount();
		}
		rs.Close();
		
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() ) 
		{	}
		else
		{
			rs.MoveLast();
			iSum+=rs.GetRecordCount();
		}
		rs.Close();
		
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
			rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
			if( rs.IsEOF() && rs.IsBOF() )
			{	}
			else
			{
				rs.MoveLast();
				iSum+=rs.GetRecordCount();
			}
			rs.Close();
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
		rs.m_pDatabase=&EDIBgbl::dbPRJDB;
		rs.Open(dbOpenDynaset,SQLx );
		if( rs.IsEOF() && rs.IsBOF() )
		{
		}
		else
		{
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomCSPR);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				//SQLx = _T("SELECT * FROM [") +  EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(vTmp1) + _T(" AND zdjh=") + vtos(vTmp2);
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					//iSEQ = iSEQ + 1
					rsCL.AddNew();
					rsCL.SetFieldValue(_T("seq"),nil);
					rs.GetFieldValue(_T("zdjh"),v);
					rsCL.SetFieldValue(_T("zdjh"),v);
					rs.GetFieldValue(_T("VolumeID"),v);
					rsCL.SetFieldValue(_T("VolumeID"),v);
					rs.GetFieldValue(_T("CLID"),v);
					rsCL.SetFieldValue(_T("CLID"),v);
					rs.GetFieldValue(_T("CLmc"),v);
					rsCL.SetFieldValue(_T("CLmc"),v);
					rs.GetFieldValue(_T("CLgg"),v);
					rsCL.SetFieldValue(_T("CLgg"),v);
					rs.GetFieldValue(_T("CLcl"),v);
					rsCL.SetFieldValue(_T("CLcl"),v);
					rs.GetFieldValue(_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL.SetFieldValue(_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs.GetFieldValue(_T("CLdz"),v);
					rsCL.SetFieldValue(_T("CLdz"),v);					
					rs.GetFieldValue(_T("CLzz"),v);
					rsCL.SetFieldValue(_T("CLzz"),v);
					if( EDIBAcad::g_bSumBomSprFactory )//newHS->�Ƿ��ڲ��ϻ��ܱ��б�ע����/�������ɳ���
					{
						rs.GetFieldValue(_T("CLbz"),v);
						rsCL.SetFieldValue(_T("CLbz"),v);
					}
					rs.GetFieldValue(_T("CLnum"),v);
					rsza.GetFieldValue(_T("Num"),vTmp1);
					rsCL.SetFieldValue(_T("CLnum"),COleVariant(vtof(v)*vtof(vTmp1)));
					rsCL.Update();
				}
				rsza.Close();
				rs.MoveNext();
			}
		}
		rs.Close();
		//���̽��������ɴ�TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
		}
		//rsCL.Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		EDIBgbl::dbPRJ.Execute( _T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup IN \'") + EDIBgbl::dbPRJ.GetName() + _T("\' FROM TmpTCL GROUP BY ") + sGROUPBY);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL"));
		//���������ɵ����ݿ�����TmpCSPR��
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpCSPR SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{
		}
		else
		{
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomSPR);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					rsCL.AddNew();
					rsCL.SetFieldValue(_T("seq"),nil);
					rs.GetFieldValue(_T("zdjh"),v);
					rsCL.SetFieldValue(_T("zdjh"),v);
					rs.GetFieldValue(_T("VolumeID"),v);
					rsCL.SetFieldValue(_T("VolumeID"),v);
					rs.GetFieldValue(_T("CLID"),v);
					rsCL.SetFieldValue(_T("CLID"),v);
					rs.GetFieldValue(_T("CLmc"),v);
					rsCL.SetFieldValue(_T("CLmc"),v);
					rs.GetFieldValue(_T("CLgg"),v);
					rsCL.SetFieldValue(_T("CLgg"),v);
					rs.GetFieldValue(_T("CLcl"),v);
					rsCL.SetFieldValue(_T("CLcl"),v);
					rs.GetFieldValue(_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL.SetFieldValue(_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs.GetFieldValue(_T("CLdz"),v);
					rsCL.SetFieldValue(_T("CLdz"),v);					
					rs.GetFieldValue(_T("CLzz"),v);
					rsCL.SetFieldValue(_T("CLzz"),v);
					if( EDIBAcad::g_bSumBomSprFactory )//newhs->�Ƿ��ڲ��ϻ��ܱ��б�ע����/�������ɳ���
					{
						rs.GetFieldValue(_T("CLbz"),v);
						rsCL.SetFieldValue(_T("CLbz"),v);
					}
					rs.GetFieldValue(_T("CLnum"),v);
					rsza.GetFieldValue(_T("Num"),vTmp1);
					rsCL.SetFieldValue(_T("CLnum"),COleVariant(vtof(v)*vtof(vTmp1)));
					rsCL.Update();
				}
				rsza.Close();
				rs.MoveNext();
			}
		}
		rs.Close();
		//���̽����ɴ�TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
		}
		//rsCL.Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx =_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ.Execute(SQLx);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//�����ɵ����ݿ�����TmpSPR��
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpSPR SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") +  EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL"));
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{
		}
		else
		{
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomPA);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					rsCL.AddNew();
					rsCL.SetFieldValue(_T("seq"),nil);
					rs.GetFieldValue(_T("zdjh"),v);
					rsCL.SetFieldValue(_T("zdjh"),v);
					rs.GetFieldValue(_T("VolumeID"),v);
					rsCL.SetFieldValue(_T("VolumeID"),v);
					rs.GetFieldValue(_T("CLID"),v);
					rsCL.SetFieldValue(_T("CLID"),v);
					rs.GetFieldValue(_T("CLmc"),v);
					rsCL.SetFieldValue(_T("CLmc"),v);
					rs.GetFieldValue(_T("CLgg"),v);
					rsCL.SetFieldValue(_T("CLgg"),v);
					rs.GetFieldValue(_T("CLcl"),v);
					rsCL.SetFieldValue(_T("CLcl"),v);
					rs.GetFieldValue(_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL.SetFieldValue(_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs.GetFieldValue(_T("CLdz"),v);
					rsCL.SetFieldValue(_T("CLdz"),v);					
					rs.GetFieldValue(_T("CLzz"),v);
					rsCL.SetFieldValue(_T("CLzz"),v);
					rs.GetFieldValue(_T("CLbz"),v);
					rsCL.SetFieldValue(_T("CLbz"),v);
					rs.GetFieldValue(_T("CLnum"),v);
					rsza.GetFieldValue(_T("Num"),vTmp1);
					rsCL.SetFieldValue(_T("CLnum"),COleVariant(vtof(v)*vtof(vTmp1)));
					rsCL.Update();
				}
				rsza.Close();
				rs.MoveNext();
			}
		}
		rs.Close();
		//���̽��ܲ���TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
		}
		//rsCL.Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup    FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ.Execute(SQLx);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//���ܲ����ݿ�����TmpPA��
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpPA SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL"));
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{
		}
		else
		{
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomNotSPRConnect);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					rsCL.AddNew();
					rsCL.SetFieldValue(_T("seq"),nil);
					rs.GetFieldValue(_T("zdjh"),v);
					rsCL.SetFieldValue(_T("zdjh"),v);
					rs.GetFieldValue(_T("VolumeID"),v);
					rsCL.SetFieldValue(_T("VolumeID"),v);
					rs.GetFieldValue(_T("CLID"),v);
					rsCL.SetFieldValue(_T("CLID"),v);
					rs.GetFieldValue(_T("CLmc"),v);
					rsCL.SetFieldValue(_T("CLmc"),v);
					rs.GetFieldValue(_T("CLgg"),v);
					rsCL.SetFieldValue(_T("CLgg"),v);
					rs.GetFieldValue(_T("CLcl"),v);
					rsCL.SetFieldValue(_T("CLcl"),v);
					rs.GetFieldValue(_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL.SetFieldValue(_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs.GetFieldValue(_T("CLdz"),v);
					rsCL.SetFieldValue(_T("CLdz"),v);					
					rs.GetFieldValue(_T("CLzz"),v);
					rsCL.SetFieldValue(_T("CLzz"),v);
					rs.GetFieldValue(_T("CLbz"),v);
					rsCL.SetFieldValue(_T("CLbz"),v);
					rs.GetFieldValue(_T("CLnum"),v);
					rsza.GetFieldValue(_T("Num"),vTmp1);
					rsCL.SetFieldValue(_T("CLnum"),COleVariant(vtof(v)*vtof(vTmp1)));
					rsCL.Update();
				}
				rsza.Close();
				rs.MoveNext();
			}
		}
		rs.Close();
		//���̽��ǵ������Ӽ���TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
		}
		//rsCL.Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ.Execute(SQLx);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//���ǵ������Ӽ�(����������)�����ݿ�����TmpConnection��
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpConnection SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") +EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL"));
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{
			//û���κ�����
		}
		else
		{
			//���κ�����,RS-Բ��
			this->rsID.FindFirst( _T("trim(ID)=\'RS\'"));
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomRod);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
				rsza.m_pDatabase=&EDIBgbl::dbPRJDB; rsza.Open(dbOpenSnapshot,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					
					sTmp=((vTmp=GetFields(rsza,_T("num"))).vt==VT_NULL ? 1.0 : vtof(vTmp)) * ((vTmp=GetFields(rs,_T("CLnum"))).vt==VT_NULL ? 1.0 : vtof(vTmp));					
					sngSumWeight=sTmp * vtof(GetFields(rs,_T("GDW1"))) * vtof(GetFields(rs,_T("L1")) ) / 1000.0; //����1m���ر�����GDW1�ֶ�
					rsCL.AddNew();
					rsCL.SetFieldValue(_T("seq"),nil);
					rs.GetFieldValue(_T("zdjh"),v);
					rsCL.SetFieldValue(_T("zdjh"),v);
					rs.GetFieldValue(_T("VolumeID"),v);
					rsCL.SetFieldValue(_T("VolumeID"),v);
					rs.GetFieldValue(_T("CLID"),v);
					rsCL.SetFieldValue(_T("CLID"),v);
					rs.GetFieldValue(_T("CLmc"),v);
					rsCL.SetFieldValue(_T("CLmc"),v);
					rs.GetFieldValue(_T("CLgg"),v);
					rsCL.SetFieldValue(_T("CLgg"),v);
					rs.GetFieldValue(_T("CLcl"),v);
					rsCL.SetFieldValue(_T("CLcl"),v);
					rs.GetFieldValue(_T("CLdw"),v);
					CString str;
					str.LoadString(IDS_PIECE);
					rsCL.SetFieldValue(_T("CLdw"),(v.vt==VT_NULL) ? STR_VAR(str) : v);
					rs.GetFieldValue(_T("CLdz"),v);
					rsCL.SetFieldValue(_T("CLdz"),v);					
					rs.GetFieldValue(_T("CLbz"),v);
					rsCL.SetFieldValue(_T("CLbz"),v);
					rs.GetFieldValue(_T("GDW1"),v);
					rs.GetFieldValue(_T("L1"),vTmp1);
					
					rsCL.SetFieldValue(_T("CLdz"),COleVariant(vtof(v) * vtof(vTmp1)/1000.0)); //����1m���ر�����GDW1�ֶ�
					rsCL.SetFieldValue(_T("CLnum"),COleVariant(sTmp));
					rsCL.SetFieldValue(_T("CLzz"),COleVariant(sngSumWeight));
					rsCL.Update();
				}
				rsza.Close();
				rs.MoveNext();
			}
		}
		//���̽����������TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
		}
		//rsCL.Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ.Execute(SQLx);
		if(modPHScal::gbSumRodByRoundSteel)
			;
		else
		{
			//����tmpCLgroup��tmp2,�Ա㱣���������
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
			//�����˵����ݿ�����TmpConnection��
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpConnection SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		}
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL"));
		//ɾ����ʱ��
		RsDeleteAll(rsCL);
		//������ʱ���ϱ����ݼǶ���
		
		//************
		//ͳ�����˳��ȣ���Բ��ֱ�����ࣩ
		//�ص���һ����¼
		if( rs.IsEOF() && rs.IsBOF() )
		{
			//û���κ�����
		}
		else
		{
			rs.MoveFirst();
			//���κ�����
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomRodLen);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
				rsza.m_pDatabase=&EDIBgbl::dbPRJDB; rsza.Open(dbOpenSnapshot,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					
					sTmp=((vTmp=GetFields(rsza,_T("num"))).vt==VT_NULL ? 1.0 : vtof(vTmp)) * vtof(GetFields(rs,_T("CLnum"))) * vtof(GetFields(rs,_T("L1"))) / 1000.0;
					sngSumWeight = sTmp * vtof(GetFields(rs,_T("GDW1")));  //����1m���ر�����GDW1�ֶ�
					//if( sTmp <> 0 ){ Debug.Print sTmp
					rsCL.AddNew();
					rsCL.SetFieldValue(_T("seq"),nil);
					rs.GetFieldValue(_T("zdjh"),v);
					rsCL.SetFieldValue(_T("zdjh"),v);
					rs.GetFieldValue(_T("VolumeID"),v);
					rsCL.SetFieldValue(_T("VolumeID"),v);
					rs.GetFieldValue(_T("CLID"),v);
					rsCL.SetFieldValue(_T("CLID"),v);
					rsCL.SetFieldValue(_T("CLmc"),GetFields(this->rsID,_T("Description")));
					rsCL.SetFieldValue(_T("CLgg"),STR_VAR(CString(_T("d=")) +vtos(GetFields(rs,_T("sizeC")))));
					rsCL.SetFieldValue(_T("CLcl"),GetFields(rs,_T("CLcl")));
					CString str;
					str.LoadString(IDS_METER);
					rsCL.SetFieldValue(_T("CLdw"),(GetFields(rs,_T("Cldw")).vt==VT_NULL ?  STR_VAR(str) : GetFields(rs,_T("CLdw"))));
					//rs.GetFieldValue(_T("GDW1"),vTmp); vTmp.vt==VT_NULL ? strTmp=_T("0") : strTmp.Format(_T("%.3f"),vtof(vTmp));rsCL.SetFieldValue(_T("CLdz"),STR_VAR(strTmp));
					
					rsCL.SetFieldValue(_T("CLdz"),GetFields(rs,_T("GDW1")));
					//rsCL.SetFieldValue(_T("CLdz")) = Format(IIf(IsNull(rsCL.SetFieldValue(_T("CLdz"))), 0, rsCL.SetFieldValue(_T("CLdz"))), _T("0.##"))
					rsCL.SetFieldValue(_T("CLnum"),COleVariant(sTmp));
					rsCL.SetFieldValue(_T("CLzz"),COleVariant(sngSumWeight));
					rsCL.SetFieldValue(_T("CLbz"),GetFields(rs,_T("CLbz")));
					rsCL.Update();
				}
				rsza.Close();
				rs.MoveNext();
			}
		}
		rs.Close();
		//���̽����˲��ϴ�TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
		}
		//rsCL.Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ.Execute(SQLx);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		//EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy
		if(modPHScal::gbSumRodByRoundSteel)
		{
			//����tmpCLgroup��tmp2,�Ա㱣���������,tmp2�����ݼ�¼�����
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
			//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL"));
		}
		
		//�����˵����ݿ�����TmpLugBom��
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpLugBom SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		//EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL")
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{
			//û���κη��͸�,����˨��ĸ�ȵĸ���
		}
		else
		{
			//���κη��͸�,����˨��ĸ�ȵ����
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomAttachements);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				//ֻѡ�����������Ҫͳ�Ƶ�֧����
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					
					if( this->rsID.FindFirst( _T("trim(CustomID)=\'") + vtos(GetFields(rs,_T("CustomID"))) + _T("\'")))
					{
						int intIndex=vtoi(GetFields(this->rsID,_T("Index")));
						if( intIndex==iSectionSteel )
						{
						}
						else
						{
							rsza.GetFieldValue(_T("num"),v);
							m_iNum=vtoi(v);
							rsza.GetFieldValue(_T("Gnum"),v);
							m_iSANum=vtoi(v);
							rsza.GetFieldValue(_T("bNotSumSA"),v);
							m_bNotSumSA=vtob(v);
							rs.GetFieldValue(_T("CLnum"),v);
							m_iCLNum=vtoi(v);
							rs.GetFieldValue(_T("bUserAdd"),v);
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
							rsCL.AddNew();
							rsCL.SetFieldValue(_T("seq"),nil);
							rs.GetFieldValue(_T("zdjh"),v);
							rsCL.SetFieldValue(_T("zdjh"),v);
							rs.GetFieldValue(_T("VolumeID"),v);
							rsCL.SetFieldValue(_T("VolumeID"),v);
							rs.GetFieldValue(_T("CLID"),v);
							rsCL.SetFieldValue(_T("CLID"),v);
							rs.GetFieldValue(_T("CLmc"),v);
							rsCL.SetFieldValue(_T("CLmc"),v);
							rsCL.SetFieldValue(_T("CLgg"),GetFields(rs,_T("CLgg")));
							rsCL.SetFieldValue(_T("CLcl"),GetFields(rs,_T("CLcl")));
							//rsCL.SetFieldValue(_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(_T("��")) : vTmp));
							CString str;
							str.LoadString(IDS_PIECE);
							rsCL.SetFieldValue(_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(str) : vTmp));
							rsCL.SetFieldValue(_T("CLdz"),GetFields(rs,_T("CLdz")));
							//rsCL.SetFieldValue(_T("CLdz")) = Format(IIf(IsNull(rsCL.SetFieldValue(_T("CLdz"))), 0, rsCL.SetFieldValue(_T("CLdz"))), _T("0.##"))
							rsCL.SetFieldValue(_T("CLnum"),COleVariant(sTmp));
							rsCL.SetFieldValue(_T("CLzz"),COleVariant(sngSumWeight));
							rsCL.SetFieldValue(_T("CLbz"),GetFields(rs,_T("CLbz")));
							rsCL.Update();
						}
					}
				}
				rsza.Close();
				rs.MoveNext();
			}
			//���̽�����ĸ,��˨����������TCL������ܵ�Tmp2
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
			{
				//��TmpCLgroup���ڱ��������
				EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
			}
			//rsCL.Close();
			//��tmp2����һ��������TmpCLgroup
			//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
			SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
			SQLx +=_T(" GROUP BY ") + sGROUPBY;
			EDIBgbl::dbPRJ.Execute(SQLx);
			//����tmpCLgroup��tmp2,�Ա㱣���������
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
			//������ĸ,��˨�������������ݿ�����TmpAttachment��
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpAttachment SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
			//����ʱ������ݿ�������ʽ���ϱ�TCL
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL"));
			//ɾ����ʱ��
			RsDeleteAll(rsCL);
			//������ʱ���ϱ����ݼǶ���
			//rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
		}
		
		
		//�͸ְ�����ͳ�Ƶ���дһ����TmpSSBom
		if( rs.IsEOF() && rs.IsBOF() )
		{
			//û���͸�
		}else
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomSSLen);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh"))) + _T(" AND  NOT bNotSumSA ");
				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					
					if(this->rsID.FindFirst( _T("trim(CustomID)=\'") + vtos(GetFields(rs,_T("CustomID"))) + _T("\'")))
					{					
						if( vtoi(GetFields(this->rsID,_T("Index")))==iSectionSteel )
						{                  
							rsza.GetFieldValue(_T("num"),v);
							m_iNum=vtoi(v);
							rsza.GetFieldValue(_T("Gnum"),v);
							m_iSANum=vtoi(v);
							rsza.GetFieldValue(_T("bNotSumSA"),v);
							m_bNotSumSA=vtob(v);
							rs.GetFieldValue(_T("CLnum"),v);
							m_iCLNum=vtoi(v);
							rs.GetFieldValue(_T("bUserAdd"),v);
							m_bUserAdd=vtob(v);
							rs.GetFieldValue(_T("L1"),v);
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
							rsCL.AddNew();
							rsCL.SetFieldValue(_T("seq"),nil);
							rs.GetFieldValue(_T("zdjh"),v);
							rsCL.SetFieldValue(_T("zdjh"),v);
							rs.GetFieldValue(_T("VolumeID"),v);
							rsCL.SetFieldValue(_T("VolumeID"),v);
							rs.GetFieldValue(_T("CLID"),v);
							rsCL.SetFieldValue(_T("CLID"),v);
							rs.GetFieldValue(_T("CLmc"),v);
							rsCL.SetFieldValue(_T("CLmc"),v);
							rs.GetFieldValue(_T("CLgg"),v);
							rsCL.SetFieldValue(_T("CLgg"),v);
							rsCL.SetFieldValue(_T("CLcl"),GetFields(rs,_T("CLcl")));
							CString str;
							str.LoadString(IDS_METER);
							rsCL.SetFieldValue(_T("CLdw"),(GetFields(rs,_T("Cldw")).vt==VT_NULL ?  STR_VAR(str) : GetFields(rs,_T("CLdw"))));
							rsCL.SetFieldValue(_T("CLdz"),GetFields(rs,_T("CLdz")));
							//rsCL.SetFieldValue(_T("CLdz")) = Format(IIf(IsNull(rsCL.SetFieldValue(_T("CLdz"))), 0, rsCL.SetFieldValue(_T("CLdz"))), _T("0.##"))
							rsCL.SetFieldValue(_T("CLnum"),COleVariant(sTmp));
							rsCL.SetFieldValue(_T("CLzz"),COleVariant(sngSumWeight));
							rsCL.SetFieldValue(_T("CLbz"),GetFields(rs,_T("CLbz")));
							rsCL.Update();
						}
					}
				}
				rsza.Close();
				rs.MoveNext();
			}
			//���̽��͸ִ�TCL������ܵ�Tmp2
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
			{
				//��TmpCLgroup���ڱ��������
				EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
			}
			//rsCL.Close();
			//��tmp2����һ��������TmpCLgroup
			//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
			SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
			SQLx +=_T(" GROUP BY ") + sGROUPBY;
			EDIBgbl::dbPRJ.Execute(SQLx);
			//����tmpCLgroup��tmp2,�Ա㱣���������
			//�¾䲻���٣���ΪAutoCAD������ϱ�ʱ����Ҫ����GetphsSumBom������в��ϵļ��ϣ���ACAD�иü�����ԴΪtmp2��
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
			//���͸ֵ����ݿ�����TmpSSBom��
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpSSBom SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
			//����ʱ������ݿ�������ʽ���ϱ�TCL
			//EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL")
			//ɾ����ʱ��
			RsDeleteAll(rsCL);
			//������ʱ���ϱ����ݼǶ���
		}
		
		
		//�͸ְ����ͳ�Ƶ���дһ����TmpSS
		if( rs.IsEOF() && rs.IsBOF() )
		{
			//û���͸�
		}
		else
		{
			rs.MoveFirst();
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomSS);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh"))) + _T(" AND NOT bNotSumSA");
				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					
					if(this->rsID.FindFirst( _T("trim(CustomID)=\'") + vtos(GetFields(rs,_T("CustomID"))) + _T("\'")))
					{
						//�͸�(��2001.12.31��ְ�PS�����͸�(index=3)��Ϊ����(index=4)
						if( (vtoi(GetFields(this->rsID,_T("Index")))==iSectionSteel) )
						{
							rsza.GetFieldValue(_T("Num"),v);
							m_iNum=vtoi(v);
							rsza.GetFieldValue(_T("Gnum"),v);
							m_iSANum=vtoi(v);
							rsza.GetFieldValue(_T("bNotSumSA"),v);
							m_bNotSumSA=vtob(v);
							rs.GetFieldValue(_T("CLnum"),v);
							m_iCLNum=vtoi(v);
							rs.GetFieldValue(_T("bUserAdd"),v);
							m_bUserAdd=vtob(v);
							rs.GetFieldValue(_T("L1"),v);
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
							rsCL.AddNew();
							rsCL.SetFieldValue(_T("seq"),nil);
							rs.GetFieldValue(_T("zdjh"),v);
							rsCL.SetFieldValue(_T("zdjh"),v);
							rs.GetFieldValue(_T("VolumeID"),v);
							rsCL.SetFieldValue(_T("VolumeID"),v);
							rs.GetFieldValue(_T("CLID"),v);
							rsCL.SetFieldValue(_T("CLID"),v);
							rs.GetFieldValue(_T("CLmc"),v);
							rsCL.SetFieldValue(_T("CLmc"),v);
							
							rsCL.SetFieldValue(_T("CLgg"),STR_VAR(vtos(GetFields(rs,_T("CLgg"))) + _T(" L=") + ltos(m_iL1)));
							rsCL.SetFieldValue(_T("CLcl"),GetFields(rs,_T("CLcl")));
							rsCL.SetFieldValue(_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(_T("��")) : vTmp));
							
							rsCL.SetFieldValue(_T("CLdz"),COleVariant(vtof(GetFields(rs,_T("CLdz"))) * m_iL1/1000.0));
							rsCL.SetFieldValue(_T("CLnum"),COleVariant(sTmp));
							rsCL.SetFieldValue(_T("CLzz"),COleVariant(sngSumWeight));
							rsCL.SetFieldValue(_T("CLbz"),GetFields(rs,_T("CLbz")));
							rsCL.Update();
							
						}
					}
					
				}
				rsza.Close();
				rs.MoveNext();
			}
			//���̽��͸ִ�TCL������ܵ�Tmp2
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
			{
				//��TmpCLgroup���ڱ��������
				EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
			}
			//rsCL.Close();
			//��tmp2����һ��������TmpCLgroup
			//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
			SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
			SQLx +=_T(" GROUP BY ") + sGROUPBY;
			EDIBgbl::dbPRJ.Execute(SQLx);
			//����tmpCLgroup��tmp2,�Ա㱣���������
			//EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy
			//���͸ֵ����ݿ�����TmpSS��
			EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpSS SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
			//����ʱ������ݿ�������ʽ���ϱ�TCL
			//EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL")
			//ɾ����ʱ��
			RsDeleteAll(rsCL);
			//������ʱ���ϱ����ݼǶ���
		}
		rs.Close();
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
			rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
			if( rs.IsEOF() && rs.IsBOF() )
			{
			}
			else
			{
				while(!rs.IsEOF())
				{
					frmStatus.m_Label1= GetResStr(IDS_SumPhsBomSA);
					strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
					frmStatus.m_Label2=strTmp;
					frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
					frmStatus.UpdateData(false);
					iTmp+=1;
					
					SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh"))) + _T(" AND NOT bNotSumSA");
					rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
					if( rsza.IsEOF() && rsza.IsBOF() )
					{
					}
					else
					{
						rsza.MoveFirst();
						rsCL.AddNew();
						rsCL.SetFieldValue(_T("seq"),nil);
						rs.GetFieldValue(_T("zdjh"),v);
						rsCL.SetFieldValue(_T("zdjh"),v);
						rs.GetFieldValue(_T("VolumeID"),v);
						rsCL.SetFieldValue(_T("VolumeID"),v);
						rs.GetFieldValue(_T("CLID"),v);
						rsCL.SetFieldValue(_T("CLID"),v);
						rs.GetFieldValue(_T("CLmc"),v);
						rsCL.SetFieldValue(_T("CLmc"),v);
						rs.GetFieldValue(_T("CLgg"),v);
						rsCL.SetFieldValue(_T("CLgg"),v);
						rs.GetFieldValue(_T("CLcl"),v);
						rsCL.SetFieldValue(_T("CLcl"),v);
						CString str;
						str.LoadString(IDS_PIECE);
						rsCL.SetFieldValue(_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(str) : vTmp));
						rsCL.SetFieldValue(_T("CLdz"),GetFields(rs,_T("CLdz")));//, 0, rs.Fields(_T("CLdz"))), _T("0.##"))
						rsCL.SetFieldValue(_T("CLnum"),COleVariant(vtof(GetFields(rsza,_T("Gnum"))) * vtof(GetFields(rsza,_T("num")))));
						rsCL.SetFieldValue(_T("CLzz"),GetFields(rs,_T("CLzz")));
						rsCL.SetFieldValue(_T("CLbz"),GetFields(rs,_T("CLbz")));
						rsCL.Update();
					}
					rsza.Close();
					rs.MoveNext();
				}
				//���̽��������ܴ�TCL������ܵ�TmpSA
				if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
				{
					//��TmpCLgroup���ڱ��������
					EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
				}
				//rsCL.Close();
				//��tmpCL����һ��������TmpCLgroup
				//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
				SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
				SQLx +=_T(" GROUP BY ") + sGROUPBY;
				EDIBgbl::dbPRJ.Execute(SQLx);
				//���������ܵ����ݿ�����TmpSA��
				EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpSA SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
				//����ʱ������ݿ�������ʽ���ϱ�TCL
				//��������������ܻ��ܵ����ϱ���
				//ɾ����ʱ��
				RsDeleteAll(rsCL);
				//������ʱ���ϱ����ݼǶ���
				//rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
			}
		}
		rs.Close();
		
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
		rs.m_pDatabase = &EDIBgbl::dbPRJDB;rs.Open(dbOpenDynaset,SQLx);
		if( rs.IsEOF() && rs.IsBOF() )
		{
		}
		else
		{
			while(!rs.IsEOF())
			{
				frmStatus.m_Label1= GetResStr(IDS_SumPhsBomBoltsNuts);
				strTmp.Format(GetResStr(IDS_SumPhsBomRec),ltos(iSum),ltos(iTmp));
				frmStatus.m_Label2=strTmp;
				frmStatus.UpdateStatus ( (float)iTmp/iSum, true);
				frmStatus.UpdateData(false);
				iTmp+=1;
				
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + vtos(GetFields(rs,_T("VolumeID"))) + _T(" AND zdjh=") + vtos(GetFields(rs,_T("zdjh")));
				rsza.m_pDatabase = &EDIBgbl::dbPRJDB;rsza.Open(dbOpenDynaset,SQLx,dbReadOnly);
				if( rsza.IsEOF() && rsza.IsBOF() )
				{
				}
				else
				{
					rsza.MoveFirst();
					rsza.GetFieldValue(_T("bNotSumSA"),v);
					m_bNotSumSA=vtob(v);
					rs.GetFieldValue(_T("bUserAdd"),v);
					m_bUserAdd=vtob(v);					
					rs.GetFieldValue(_T("recno"),v);
					/*
					rs.GetFieldValue(_T("SEQ"),vSEQ);
					if(v.vt==VT_NULL)
					if(m_bUserAdd)
					;
					else
					if(!m_bNotSumSA)
					;
					else
					//ֻ�д�ʱ��ͳ������;
					else
					;
					*/
					
					if(!( !(v.vt==VT_NULL) ||  (v.vt==VT_NULL) && (m_bUserAdd || !m_bUserAdd && !m_bUserAdd) ))
					{
						//�յļ�¼�ţ��Ҳ����û���ӵĲ��ϣ���ͳ�Ƹ���
						//����Ҫͳ�Ƶĸ�����˨��ĸ
					}else
					{
						rsza.GetFieldValue(_T("Num"),v);
						m_iNum=vtoi(v);
						rsza.GetFieldValue(_T("Gnum"),v);
						m_iSANum=vtoi(v);
						rsza.GetFieldValue(_T("bNotSumSA"),v);
						m_bNotSumSA=vtob(v);
						rs.GetFieldValue(_T("CLnum"),v);
						m_iCLNum=vtoi(v);
						rs.GetFieldValue(_T("bUserAdd"),v);
						m_bUserAdd=vtob(v);
						rs.GetFieldValue(_T("L1"),v);
						m_iL1=vtoi(v);
						sTmp=m_iNum*m_iCLNum;
						rsCL.AddNew();
						rsCL.SetFieldValue(_T("seq"),nil);
						rs.GetFieldValue(_T("zdjh"),v);
						rsCL.SetFieldValue(_T("zdjh"),v);
						rs.GetFieldValue(_T("VolumeID"),v);
						rsCL.SetFieldValue(_T("VolumeID"),v);
						rs.GetFieldValue(_T("CLID"),v);
						rsCL.SetFieldValue(_T("CLID"),v);
						rs.GetFieldValue(_T("CLmc"),v);
						rsCL.SetFieldValue(_T("CLmc"),v);
						rs.GetFieldValue(_T("CLgg"),v);
						rsCL.SetFieldValue(_T("CLgg"),v);
						rs.GetFieldValue(_T("CLcl"),v);
						rsCL.SetFieldValue(_T("CLcl"),v);
						CString str;
						str.LoadString(IDS_PIECE);
						rsCL.SetFieldValue(_T("CLdw"),((vTmp=GetFields(rs,_T("Cldw"))).vt==VT_NULL ? STR_VAR(str) : vTmp));
						rsCL.SetFieldValue(_T("CLdz"),GetFields(rs,_T("CLdz")));
						rsCL.SetFieldValue(_T("CLnum"),COleVariant(sTmp));
						rsCL.SetFieldValue(_T("CLzz"),GetFields(rs,_T("CLzz")));
						rsCL.SetFieldValue(_T("CLbz"),GetFields(rs,_T("CLbz")));
						rsCL.Update();
					}
				}
				rsza.Close();
				rs.MoveNext();
			}
		}
		rs.Close();
		//���̽���˨��ĸ��TCL������ܵ�Tmp2
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")) )
		{
			//��TmpCLgroup���ڱ��������
			EDIBgbl::dbPRJ.Execute( _T("DROP TABLE TmpCLgroup"));
		}
		//rsCL.Close();
		//��tmp2����һ��������TmpCLgroup
		//GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
		SQLx=_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup   FROM TmpTCL ");
		SQLx +=_T(" GROUP BY ") + sGROUPBY;
		EDIBgbl::dbPRJ.Execute(SQLx);
		//����tmpCLgroup��tmp2,�Ա㱣���������
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO Tmp2 SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//����˨��ĸ�����ݿ�����TmpBoltsNuts��
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO TmpBoltsNuts SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz FROM tmpCLgroup ORDER BY ") + strOrderBy);
		//����ʱ���ϱ�����ݿ�������ʽ���ϱ�TCL
		EDIBgbl::dbPRJ.Execute( _T("INSERT INTO [") + EDIBgbl::Btype[EDIBgbl::TCL] + _T("] IN \'") + EDIBgbl::dbPRJDB.GetName() + _T("\' SELECT * FROM TmpTCL"));
		//ɾ����ʱ��
		EDIBgbl::dbPRJ.Execute( _T("DELETE * FROM TmpTCL"));
		//������ʱ���ϱ����ݼǶ���
		//rsCL.Open(dbOpenDynaset,_T("SELECT * FROM TmpTCL") );
	}
	catch(::CDaoException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
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
	CDaoRecordset rs(&EDIBgbl::dbPHScode),rsTmpZB(&EDIBgbl::dbPRJDB);//20071101 "dbSORT" ��Ϊ "dbPHScode"
	//   long i;
	CString tbn,tmpID,tmpBlkID,SQLx;
	CString tmpStr;
	try
	{
		SQLx = _T("SELECT * FROM phsBlkDimPos");
		rs.Open(dbOpenSnapshot,SQLx);
		//���пɻ������recno<>Null
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 ORDER BY recno");
		rsTmpZB.Open(dbOpenDynaset,SQLx);
		if(rsTmpZB.IsEOF() && rsTmpZB.IsBOF())
		{
			tmpStr.Format(GetResStr(IDS_NoRecordInTZB),EDIBgbl::dbPRJDB.GetName(),EDIBgbl::Btype[EDIBgbl::TZB], EDIBgbl::SelJcdm,ltos(modPHScal::zdjh));
			throw tmpStr;
		}
		COleVariant vTmp;
		while(!rsTmpZB.IsEOF())
		{
			//չ��sFindTBN�Լӿ��ٶ�
			//tbn = sFindTBN(rsTmpZB.Fields(_T("CustomID")))
			rsTmpZB.GetFieldValue(_T("CustomID"),vTmp);
			
			if(rsID.FindFirst(_T("trim(CustomID)=\'") +vtos(vTmp) + _T("\'")))
			{
				rsID.GetFieldValue(_T("Index"),vTmp);
				modPHScal::glIDIndex = vtoi(vTmp);
				rsID.GetFieldValue(_T("ClassID"),vTmp);
				modPHScal::glClassID=vtoi(vTmp);
			}
			if(modPHScal::glIDIndex == iPA)
				modPHScal::glPAid = modPHScal::glClassID;
			rsTmpZB.GetFieldValue(_T("ID"),vTmp);
			if(vtos(vTmp)==_T(""))
			{
				//�����󻪶�Ժ��׼�ĸ���������鲻��ID,���CustomID,Ȼ������ID.
				rsTmpZB.GetFieldValue(_T("CustomID"),vTmp);
				tmpID = modPHScal::sFindID(vtos(vTmp));
				rsTmpZB.Edit();
				rsTmpZB.SetFieldValue(_T("ID"),STR_VAR(tmpID));
				rsTmpZB.Update();
			}
			else
			{
				rsTmpZB.GetFieldValue(_T("ID"),vTmp);
				tmpID = vtos(vTmp);
			}
			tmpBlkID =tmpID;
			//���ȿ������Ƿ����ID���ƴ���Ŀ�
			bool bm=false;
			bm=rs.FindFirst( _T("trim(blkID)=\'") + tmpBlkID + _T("\'"));
			if( modPHScal::glIDIndex == iSA)
			{
				//�Ǹ���
				if(!bm)
				{
					//���û��,��϶��ǲ۸���ɵĸ��������ϲ۸����������µĿ�,��������
					if( modPHScal::glClassID != iGCement)
						tmpBlkID = tmpBlkID +vtos( rsza->GetCollect(_T("iCSnum")));
					bm=rs.FindFirst( _T("trim(blkID)=\'") + tmpBlkID + _T("\'"));
				}
			}
			rsTmpZB.Edit();
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
					rs.GetFieldValue(_T("Zh"),vTmp);
					rsTmpZB.SetFieldValue(_T("Crd"),vTmp);
				}
				else
				{
					rs.GetFieldValue(_T("Dh"),vTmp);
					rsTmpZB.SetFieldValue(_T("Crd"),vTmp);
				}
			}
			rsTmpZB.SetFieldValue(_T("blkID"),STR_VAR(tmpBlkID));
			rsTmpZB.Update();
			rsTmpZB.MoveNext();
		}
	}
	catch(CDaoException * e)
	{
		e->ReportError();
		e->Delete();
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
		CDaoRecordset rs(&EDIBgbl::dbPRJDB);
		CString SQLx;
		COleVariant vTmp;
		modPHScal::glPAid = vtoi(modPHScal::sFindFLD(_T("CustomID"), _T("ClassID"), modPHScal::dn));
		if(! modPHScal::bPAIsHanger())
		{
			//֧��
			//If glPAid = iPAs Or glPAid = iPAfixZ1 Or glPAid = iPAfixZ2 Then
			//�ų�����(index=iSA)���丽��(��¼��Ϊ��)����������ĸ߶Ⱥ�
			SQLx = _T("SELECT sum(sizeH) as sumH FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 AND index<>") + ltos(iSA);
			rs.Open(dbOpenSnapshot,SQLx );
			if(!rs.IsEOF() && ! rs.IsBOF())
			{
				//MsgBox rs.RecordCount
				rs.GetFieldValue(_T("sumH"),vTmp);
				modPHScal::sngSEL =modPHScal::sngPEL- (vtof(vTmp)/ 1000);
				modPHScal::UpdateTZB4zdjh(_T("gh1"), _variant_t(modPHScal::sngSEL));
			}
		}
		
		//�ܲ�,����ƫװλ��ֵ�ļ���
		if( giUPxyz == 1)
		{
			//X������
			//����ͼ��λ
// 			modPHScal::dxl = modPHScal::dxa + modPHScal::yl;
// 			modPHScal::dxr = modPHScal::dxl + modPHScal::yr1;

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
	catch(CDaoException *e)
	{
		e->ReportError();
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
		CDaoRecordset rs;
		rs.m_pDatabase=&modPHScal::dbZDJcrude;
		sTmp.Format(_T("%d"),(Wx1>Wx ? Wx1 : Wx));
		sSQLx = _T("SELECT * FROM [SSteelPropertyCS] WHERE Wx>=");
		sSQLx+=sTmp;
		sSQLx+= _T(" ORDER BY ID");
		rs.Open(dbOpenSnapshot,sSQLx);
		if(rs.IsEOF() && rs.IsBOF())
		{
			sTmp.Format(GetResStr(IDS_NotFoundAnyBHinXSteelProperty),_T("SSteelPropertyCS"));
			throw sTmp;		
		}
		else
		{
			rs.GetFieldValue(_T("BH"),v);
			ret=vtos(v);
			sTmp2.Format(GetResStr(IDS_SAMadeBySSteel),_T("\%d"),ret,SACustomID);
			sTmp.Format(sTmp2,iDCS);
			throw sTmp;
		}
		rs.Close();
		return ret;
	}
	catch(::CDaoException * e)
	{
		if(FirstCal==2)
			e->ReportError();
		e->Delete();
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
		CDaoRecordset rs(&EDIBgbl::dbPHScode); //20071018 "dbSORT" ��Ϊ "dbPHScode"
		_Recordset* rsza = FrmTxsr.m_pViewTxsr->m_ActiveRs;
		CString SQLx,sTmp,sTmp2;
		COleVariant v;
		if(modPHScal::iSelSampleID > 0)
		{
			sTmp.Format(_T("%d"),SampleID);
			SQLx = _T("SELECT * FROM phsStructureName WHERE SampleID=") + sTmp;
			rs.Open(dbOpenDynaset,SQLx);
			if (rs.IsEOF() && rs.IsBOF())
			{
				sTmp2.Format(GetResStr(IDS_NotFoundSampleIDSprNum),_T("\%d"),_T("\%d"));
				sTmp.Format(sTmp2,modPHScal::iSelSampleID,modPHScal::giWholeSprNum);
				ShowMessage(sTmp);
				ret = -1;
			}
			else if(!rs.IsEOF())
			{
				rs.MoveLast();
				rs.GetFieldValue(_T("SampleID"),v);
				ret=vtoi(v);
				rs.GetFieldValue(_T("PA"),v);
				//�ܲ����Ը�ֵ
				PA=vtos(v);
				rsza->PutCollect(_T("IDdn1"),stov(PA));
				rs.GetFieldValue(_T("SA"),v);
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
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
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
	rsphsStructureREF.m_pDatabase=&EDIBgbl::dbSORT;
	TZBResultObj=NULL;
	m_rsObj=NULL;
}

Cphs::~Cphs()
{
	if(rsID.IsOpen())
		rsID.Close();
}

void Cphs::InitListRs()
{
	//�����ӱ�rsConnect��rsUnCheckedType
	try
	{
		CString sSQL;
		if(!rsConnect.IsOpen())
		{
			rsConnect.m_pDatabase=&EDIBgbl::dbPRJ;
			sSQL=(_T("SELECT * FROM connect "));
			rsConnect.Open(dbOpenDynaset,sSQL);
		}
		else
			rsConnect.Requery();
			/*if(rsPictureClipData.IsOpen())
			rsPictureClipData.Close();
			rsPictureClipData.m_pDatabase=&EDIBgbl::dbPRJ;
			sSQL=(_T("SELECT * FROM PictureClipData "));
		rsPictureClipData.Open(dbOpenSnapshot,sSQL);*/
		if(!rsUnCheckedType.IsOpen())
		{
			rsUnCheckedType.m_pDatabase=&EDIBgbl::dbPRJ;
			sSQL=(_T("SELECT * FROM rsUnCheckedType "));
			rsUnCheckedType.Open(dbOpenDynaset,sSQL);
		}
		else
			rsUnCheckedType.Requery();
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
	}
	
}

void Cphs::SelectItemRsObj()
{
/*	try
{
CString tbn1, tmpID, _Recordset* rsza;
rsza =FrmTxsr.m_pViewTxsr->m_ActiveRs;
if(m_rsObj==NULL || m_rsObj->BOF || m_rsObj->adoEOF)
{
return;
}
if(rsza==NULL || rsza->BOF || rsza->adoBOF)
return;
modPHScal::gsPartType = vtos(m_rsObj->GetCollect(_T("CustomID")));
tmpID = modPHScal::sFindID(gsPartType);
//MsgBox gsPartType
if(tmpID.Left(1)==_T("G"))
{
modPHScal::iCSnum = vtoi(m_rsObj->GetCollect(_T("P1num")));
rsza->PutCollect(_T("iCSnum"),_variant_t((long)iCSnum));
rsza->PutCollect(_T("gn1"),stov(modPHScal::gsPartType));
rsza->Update();
}
else if(tmpID.Left(1)==_T("D") || tmpID.Left(1)==_T("Z"))
{
rsza->PutCollect(_T("dn1"),stov(modPHScal::gsPartType));
rsza->Update();
}
}
catch(_com_error e)
{
ShowMessage(e.Description());
}*/
}

void Cphs::CloseRecordsets()
{
	//�ر�rsConnect��rsUnCheckedType
	try
	{
		if(rsConnect.IsOpen())
			rsConnect.Close();
		if(rsUnCheckedType.IsOpen())
			rsUnCheckedType.Close();
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
	}
}

