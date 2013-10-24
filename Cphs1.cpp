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

#pragma warning( disable : 4129 )  //Added by Shuli Luo
bool Cphs::GetphsBHandSizes(CFrmStatus& frmStatus,int  FirstCal, int MaxCalCount,int nth,
							float& TmpPaz,float& TmpPgz)
{
	float mvSumHeight =0;
	long iSEQofSPR, iSEQofPart, iChkCondition;
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

//		 rsSAPart(&EDIBgbl::dbPRJDB); //��ɸ����ĸ�����
	_RecordsetPtr rsSAPart;
//		 rsPartBoltNuts(&EDIBgbl::dbPRJDB); //�����˨��ĸ
	_RecordsetPtr rsPartBoltNuts;
	
	simplify(rsSAPart,nth);
	simplify2(rsPartBoltNuts,nth);
	
	//���������ʧ,��Ĭ��Ϊ���ء��¶ȡ�������λ����Ӧ��ȫ������
	if( iChkCondition==0 ) iChkCondition = iPJG + iTJ + iGDW1 + iCNT + iDW;
	
	//��һ�μ��㣬ֱ����ʼ��
	if( FirstCal == 1 )
	{
		modPHScal::gmiDiameter = modPHScal::giDiameter = 0;
	}
	
	//���������ʧ,��Ĭ��Ϊ��һ�����״̬��
	//����Ϊ�ڶ������״̬��
	if( FirstCal==0 ) FirstCal = 1;
	return GetphsBHandSizes1(rsSAPart, rsPartBoltNuts, FirstCal, MaxCalCount, nth, TmpPaz, TmpPgz);
}

bool Cphs::GetphsBHandSizes1(_RecordsetPtr rsSAPart, _RecordsetPtr rsPartBoltNuts, int FirstCal,int MaxCalCount,int nth,float& TmpPaz,float& TmpPgz)
{
	CString dbstr = "\n" ;
	bool ret=false;
	CString strSelNumCS;//ѡ��������۸������Ĺ����ַ���
	CString *PtypeID=NULL;   //��������������ṹID����
	CString *Ptype=NULL;   //��������������ṹCustomID����
	long *PtypeClassID=NULL;   //��������������ṹClassID����
	long *PtypeIndex=NULL;  //��������������ṹIndex����
	
	float m_fDiaM1 = 0.0f;//����ܼ���˨M1��ֵ��
	CString strDiaM1FieldName;//�ܼ���˨M1�ĳߴ��ֶΣ�һ��Ϊsize3��size2
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
	_Recordset* rsza ;
	rsza=FrmTxsr.m_pViewTxsr->m_ActiveRs;
	long i , j, k ;
	CString sBHFormat, sBH ;
	CString sPartID, sRodPASA;
	CString tmpCustomID0 , tmpCustomID1 ;
	CString tmpID0, tmpID1 ;
	CString tmpFD0 ;
	CString tmpExtLenFD;
	float tmpExtLenValue=0 ;  //��һ��������������⣩���뵱ǰ���L8�����
	float tmpFD0value=0, tmpFD0valuePrevious=0 ;  //����ǰһ�����������ƥ���ֶ�ֵ
	float sngDim=0 , sngCSLen=0 , sngWeight =0, sngW=0 ;
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
	//Set mvarrsTmpREF1 = rsTmpREF.Clone
	COleVariant vTmp1,vTmp2,vTmp3,vTmp;
	COleVariant vBH;
	CString sTmp,sTmp2;

//		 rs1(&modPHScal::dbZDJcrude);
//		 rs3(&EDIBgbl::dbPHScode);//���Ϲ淶ѡ���¼��//20071018 "dbSORT" ��Ϊ "dbPHScode"
	_RecordsetPtr rs1;
	_RecordsetPtr rs3;
	CString m_strMaterial;//�ܲ����²�������㲿��ѡ��Ĳ���
// 		 rs(&modPHScal::dbZDJcrude);
// 		 rsTmp,rsTmp1,rsTmp2;
// 		 rsCal;	//����ǿ�ȼ��㹫ʽ��CalFormulaOfFixPASA��λ��sort.mdb���û����޸�
// 		 rsDiaOfCSPRFiJ(&modPHScal::dbZDJcrude);//������������ֱ�����¼�����ü���Դ�ڱ�tbnLugDiaOfCSPR���Ϸ�����ɽ���ĸ���ز�Ҫ���Ǻ������
// 		 rsDiaOfCSPRFiK;//�����������Ӷ���ֱ�����¼�����ü���Դ�ڱ�tbnLugDiaOfCSPR�����Ӷ��Ӻ���Ҫ���Ǻ������
//		rsDiaOfCSPRFiK.m_pDatabase=&modPHScal::dbZDJcrude;
	_RecordsetPtr rs;
	_RecordsetPtr rsTmp;
	_RecordsetPtr rsTmp1;
	_RecordsetPtr rsTmp2;
	_RecordsetPtr rsCal;
	_RecordsetPtr rsDiaOfCSPRFiJ;
	_RecordsetPtr rsDiaOfCSPRFiK;
	CString tbn1,SAfDPmax;
	bool m_bFoundL3=false;//������ܼ����ӵ�˫�׵���(��ʱ�ɲ��ػ�С�ܼ���˨)

	CString strOrderBy ;      //�����ַ���
	CString strTmp ;          //��ʱ�ַ�������
	CString tmpSQL ;          //��ǰ������ƥ���ѯ�ֶ���Ϣ
	CString tmpSQL0 ;         //ǰһ�ε�����ƥ���ѯ�ֶ���Ϣ
	long mlfx =0, mlfx0 =0; //���/ǰһ���������ͼ�ķ���
	long mlRot =0, mlRot0 =0; //���/ǰһ�����Y��ķ�������з�����ͬ��=0���෴��=180��
	//long lngErrCalPAfix;   //����̶�֧��ʱ�����Ĵ����
// 		 rsX(&modPHScal::dbZDJcrude) ;
	_RecordsetPtr rsX;

	CString strSQL;
	strSQL.Format("SELECT * FROM %s WHERE zdjh=%d AND VolumeID=%d AND nth =%d ORDER BY recno",
		EDIBgbl::Btype[EDIBgbl::TZB],modPHScal::zdjh,EDIBgbl::SelVlmID,nth);

	_RecordsetPtr rsTZB;//��ǰ֧���ܵ�ǰ·�����������㲿���ı�������˨��ĸ������¼������
	try
	{
		
//		rsTZB.m_pDatabase=&EDIBgbl::dbPRJDB;
//		rsTZB.Open(dbOpenDynaset,strSQL);
		rsTZB->Open((_bstr_t)strSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}

//		 rsTmpZB(&EDIBgbl::dbPRJDB);//��ǰ֧���ܵ�ǰ·���㲿����������˨��ĸ����������
		_RecordsetPtr rsTmpZB;
		strSQL.Format("SELECT CustomID FROM ZB WHERE [VolumeID]=%d AND [ZDJH]=%d AND [nth]=%d AND [ClassID]<>%d AND [ClassID]<>%d AND [ClassID]<>%d AND [IsSAPart]<>-1 Order By recno",
				EDIBgbl::SelVlmID,modPHScal::zdjh,nth,iBolts,iNuts,iAttached);
	try
	{
//		rsTmpZB.Open(dbOpenDynaset,strSQL);
		rsTmpZB->Open((_bstr_t)strSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}
		int C;
 		if( rsTmpZB->adoEOF&& rsTmpZB->BOF )
			throw GetResStr(IDS_Null_rsTmpREF);
		else
		{
			rsTmpZB->MoveLast();
			rsTmpZB->MoveFirst();
			Ptype=new CString[rsTmpZB->GetRecordCount()];
			PtypeID=new CString[rsTmpZB->GetRecordCount()];
			PtypeIndex=new long[rsTmpZB->GetRecordCount()];
			PtypeClassID=new long[rsTmpZB->GetRecordCount()];
			C=rsTmpZB->GetRecordCount();
			CString strBH = sBHFormat;
			for( i = 0 ;i < C;i++)
			{
				rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
				Ptype[i] = vtos(vTmp1);
				if( !rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")), 0, adSearchForward, vTmp))
				{
					//����
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(vTmp1));
					throw sTmp;
				}
				else
				{
					rsID->get_Collect((_variant_t)_T("Index"), &vTmp1);
					PtypeIndex[i] = vtoi(vTmp1);
					rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp1);
					PtypeClassID[i] = vtoi(vTmp1);
					rsID->get_Collect((_variant_t)_T("ID"), &vTmp1);
					PtypeID[i] = vtos(vTmp1);
					if(i>=1 && PtypeID[i]=="L3" && (PtypeID[i-1]=="D1" ||PtypeID[i-1]=="D1A" || PtypeID[i-1]=="D2" || PtypeID[i-1]=="D2A" || PtypeID[i-1]=="D3" || PtypeID[i-1]=="D3A" || PtypeID[i-1]=="D9" || PtypeID[i-1]=="D9A" || PtypeID[i-1]=="D10")) 
						m_bFoundL3=true;//�������L3���壬�ܼ���˨�ǲ���Ҫ��С��
				}
				rsTmpZB->MoveNext();
			}
			rsTmpZB->MoveFirst();
		}
		//����Ptype���鱣����mvarrsTmpZB�Ŀ���
		//�Ա�ȡ��֧������ϵ�����2���������,���в���
		
		rsTmpZB->MoveFirst();
		if( ! rsTmpZB->adoEOF )
		{
			rsTmpZB->MoveLast();
			//չ��sFindTBN���ӿ��ٶȡ�
			rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
			
			if( !rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")), 0, adSearchForward, vTmp))
			{
				//����
				sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(vTmp1));
				throw sTmp;
			}
			//�������һ������Ǹ�����������������롣
			rsID->get_Collect((_variant_t)_T("Index"), &vTmp1);
			mvSA = vtoi(vTmp1);
			//�������һ������Ǹ�����������������롣����ͳ����˨��ĸʱ���жϵ�ǰ����Ǻ��ʱ��һ������Ƿ�Ϊ������
			PtypeIndex[rsTmpZB->GetRecordCount()- 1] = mvSA;
			//����ȷ�����õ�˫�۸�
//			rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
//			rsTmp.Open(dbOpenSnapshot);
			CString strSQL = (_T("SELECT DISTINCT PNum1 From ")) + modPHScal::tbnSA + 
				_T(" WHERE CustomID=\'") + Ptype[C-1] + _T("\' AND PNum1 is not null");
		try
		{
			rsTmp->Open((_bstr_t)strSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
			if(rsTmp->GetRecordCount()==1)
			{
				//ֻ��һ����¼,����Pnum1�ֶ�=1��ֻ�ܲ��õ��۸֣�Pnum2=2��ֻ�ܲ���˫�۸֡�
				rsTmp->get_Collect((_variant_t)0L, &vTmp1);
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
			rsTmp->Close();
			if( rsTmpZB->GetRecordCount() >= 2 )
			{
				//����������ڻ����2���������ȡ��ͷβ�����
				rsTmpZB->MoveFirst();
				//չ��sFindTBN���ӿ��ٶȡ�
				rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
				
				if( !rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")), 0, adSearchForward, vTmp))
				{
					//����
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),vtos(vTmp1));
					throw sTmp;
				}
				//�����һ������ǹܲ�������ܲ�����롣
				rsID->get_Collect((_variant_t)_T("Index"), &vTmp1);
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
				C=rsTmpZB->GetRecordCount();
				CString strBH = sBHFormat;

				for (i = 0 ;i<C;i++)
				{
					if(i==0) rsTZB->MoveFirst();
					frmStatus.UpdateStatus((float)(i+1)/C,true);
					frmStatus.UpdateWindow();
					
					//���������ڵ�ԭʼ���ݱ�tbn1,�Ա����
					//չ��sFindTBN���ӿ��ٶȡ�
					
					if(! rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + Ptype[i] + _T("\'")), 0, adSearchForward, vTmp) )
					{
						//û���ҵ�����
						sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("CustomID"),Ptype[i]);
						throw sTmp;
					}
					rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp1);
					modPHScal::glClassID =vtoi(vTmp1);
					rsID->get_Collect((_variant_t)_T("Index"), &vTmp1);
					modPHScal::glIDIndex = vtoi(vTmp1);
					rsID->get_Collect((_variant_t)_T("ID"), &vTmp1);
					sPartID = vtos(vTmp1);
					tmpID0 = sPartID;
					if( modPHScal::glIDIndex == iSA )
					{
						//��������ʾһ��ʱ��
						//��ǰ����Ǹ��������һ�����
						if( modPHScal::gbCalSAbyPJG )
						{
							//������ṹ���ؼ������
							tmpSelPJG = tmpPJG;
						}
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
							if( rsID->Find((_bstr_t) (_T("trim(CustomID)=\'") + Ptype[i + 1] + _T("\'")), 0, adSearchForward, vTmp))
							{
								rsID->get_Collect((_variant_t)_T("ID"), &vTmp1);
								tmpID1 = vtos(vTmp1);
							}
							//�������һ�����
							tmpCustomID1 = Ptype[i + 1];
							//��Connect���м������Ƿ���ڼ���ƥ��ĳߴ���Ϣ
							
							if( !rsConnect->Find((_bstr_t) (_T("trim(cntb)=\'") + tmpID0 + _T("\' AND trim(cnte)=\'") + tmpID1 + _T("\'")), 0, adSearchForward, vTmp))
							{
								//Connect���в������������.һ�㲻�ᷢ�������.
								sTmp.Format(GetResStr(IDS_PartCanNotMatchOrNotExistInConnect),tmpCustomID0,tmpCustomID1);
								throw sTmp;
							}
							//����ConnectCondition�ֶεĸ���SQL�����������ӳߴ��飬��������Ӧ�Ը��㡣�ٶ���һ�㡣
							//���﷨���ƣ�Cntb.Size2=Cnte.Size2 AND Cntb.size4<=Cnte.Size5
							rsConnect->get_Collect((_variant_t)_T("ConnectCondition"), &vTmp1);
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
											} else
											{
												//û�ҵ��κ��߼��������ţ�һ���������ݿ���˴���
											}
										}
										strDiaM1FieldName=tmpSQL0.Mid(5,i1-5);
										//�����ַ����ÿգ��򲻼��������˨�ߴ磬�����ܼ���˨����С
										tmpSQL0 = _T("");
									}
								}
							}
							
							rsConnect->get_Collect((_variant_t)_T("CnteXfx"), &vTmp1);
							mlfx0 =vtoi(vTmp1);
							rsConnect->get_Collect((_variant_t)_T("CnteRotation"), &vTmp1);
							mlRot0 = vtoi(vTmp1);
							rsConnect->get_Collect((_variant_t)_T("CntbNum"), &vTmp1);
							iCntbNum=(vtoi(vTmp1)<=0 ? 1 : vtoi(vTmp1));
							rsConnect->get_Collect((_variant_t)_T("CnteNum"), &vTmp1);
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
						if(rs3->State == adOpenStatic)
							rs3->Close();
					try {
//						rs3.Open(dbOpenSnapshot,SQLx);
						rs3->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
							adOpenDynamic, adLockReadOnly, adCmdText); 
					}
					catch(_com_error e)
					{
						ret=false;
					}
						if(rs3->adoEOF && rs3->BOF)
						{
							//��%s��%s����ѡ��淶��û��%s�ֶ�ֵΪĬ��ֵ%s�ļ�¼(�ǹܲ��������ѡ�����)
							sTmp.Format(IDS_NoDefaultInCustomIDInSpecificationOfMaterial,EDIBgbl::dbPHScode->DefaultDatabase,_T("SpecificationOfMaterial"),_T("ID"),_T("default"));
							throw sTmp;
						}
						else
						{
							rs3->get_Collect((_variant_t)_T("Material"), &vTmp);
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
		{
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
				if(rsX->State == adOpenStatic)
					rsX->Close();
			try {
	//							rsX.Open(dbOpenSnapshot,SQLx);
				rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
			}
			catch(_com_error e)
			{
				ret=false;
			}
				if( rsX->adoEOF && rsX->BOF )
					;
				else
					//û�ҵ���ÿ���⾶����1%��������
					k=modPHScal::gnDW_delta+1;		  
			}

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
		{
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
				if(rsX->State == adOpenStatic)
					rsX->Close();
			try {
	//							rsX.Open(dbOpenSnapshot,SQLx);
				rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
			}
			catch(_com_error e)
			{
				ret=false;
			}
				if( rsX->adoEOF && rsX->BOF );
				else
					//û�ҵ���ÿ���⾶����1%��������
					k=modPHScal::gnDW_delta+1;
			}
			//2007.09.26(start)
			if(modPHScal::gbPAForceZero && (rsX->adoEOF && rsX->BOF))
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
					if(rsX->State == adOpenStatic)
						rsX->Close();
		try
		{
	//								rsX.Open(dbOpenSnapshot,SQLx);
					rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
						adOpenDynamic, adLockReadOnly, adCmdText); 
				}
				catch(_com_error e)
				{
					ret=false;
				}
					if( rsX->adoEOF && rsX->BOF )
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
		}

		break;
	case iPAD4LA:
	case iPAD11LD:
	case iPADLR:
	case iPALX:
		{
			//��Щ(�̷�ú�۹ܵ��ܲ�)��ܾ��޹�
			SQLx = _T("SELECT * FROM ") + modPHScal::tbnPA + _T(" WHERE Pmax>=") ;
			sTmp.Format(_T("%g"),tmpSelPJG/iNumPart);
			SQLx+=sTmp+ _T(" AND Tj>=");
			sTmp.Format(_T("%g"),tmpT0);
			SQLx+=sTmp+_T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY tj,Pmax,Weight");
			if(rsX->State == adOpenStatic)
				rsX->Close();
		try
		{
	//						rsX.Open(dbOpenSnapshot,SQLx);
			rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
		}

		break;
	case iROD:
		{
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
			
		}

		break; 
		
	case iConnected:
	case iXL1:
	case iAttached: 
		{
			if( PtypeID[i] == _T("L5") || PtypeID[i] == _T("L6") || PtypeID[i] == _T("L7") || PtypeID[i] == _T("L8")  || PtypeID[i] == _T("L7Dd") || PtypeID[i] == _T("L8Dd") )
			{
				//����ֱ��
				if(PtypeClassID[i-1]==iROD)
				{
					//ǰһ��������			  
					modPHScal::PtypeDiameter[i]=modPHScal::PtypeDiameter[i-1];
					
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
				if(rsTmp->State == adOpenStatic)
					rsTmp->Close();
		try
		{
	//							rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
	//							rsTmp.Open(dbOpenDynaset,SQLx);
				rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
			}
			catch(_com_error e)
			{
				ret=false;
			}
				if(rsTmp->BOF && rsTmp->adoEOF)
				{
					//������������ְ�,��������
					/*rsTmp->AddNew();
					rsTmp->put_Collect((_variant_t)_T("CustomID"),COleVariant(_T("PS")));
					rsTmp->put_Collect((_variant_t)_T("BH"),COleVariant(sTmp));
					rsTmp->put_Collect((_variant_t)_T("size2"),COleVariant(modPHScal::gfPSThickness));
					rsTmp->put_Collect((_variant_t)_T("Weight"),COleVariant(fPSWeight));
					rsTmp->Update();*/
					strSQL = _T("INSERT INTO [")+ modPHScal::tbnAttachment + _T("] (CustomID,BH,size2,Weight) VALUES (\'PS\',\'") + sTmp + _T("\',") + ftos(modPHScal::gfPSThickness) + _T(",") + ftos(fPSWeight) + _T(")");
					modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);
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
		{
			if(iSEQofSPR < modPHScal::giWholeSprNum)
				modPHScal::giCSPRindex[iSEQofSPR] = i;
			//��λ������Ʊ�־
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("UPDATE tmpCSLen SET SpecialDesign=\'\'"), NULL, adCmdText);
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
			if(rs->State == adOpenStatic)
				rs->Close();
		try
		{
	//		rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
			if( rs->adoEOF && rs->BOF )
			{
				//����غ�������Ϊ�գ�������
				sTmp.Format(GetResStr(IDS_NullTableInXMdb),modPHScal::dbZDJcrude->DefaultDatabase, modPHScal::tbnHDproperty);
				throw sTmp;
			}
			else
			{
				rs->get_Collect((_variant_t)_T("Capacity"), &vTmp1);
				Cmin = vtof(vTmp1);
				rs->MoveLast();
				rs->get_Collect((_variant_t)_T("Capacity"), &vTmp1);
				Cmax = vtof(vTmp1);
				rs->MoveFirst();
				//��λ���ҵ����ʺ����־
				m_bFoundCSPR = false;
				//��ʼ��ʱ������iSumSerialNum=0
				iSumSerialNum = iSumSerialNum + 1;
				tmpDist = Tmpyr / modPHScal::giWholeSprNum;
				//��λ��ϵ��ֵ�����ͽ��Ͽ�ԭ��ȷ��λ��ϵ�С�
				if(rs1->State == adOpenStatic)
					rs1->Close();
				sTmp.Format(_T("%g"),tmpDist);
		try
		{
	//			rs1.Open(dbOpenSnapshot,_T("SELECT * FROM [") + modPHScal::tbnDisplacementSerial + _T("] WHERE DisplacementSerial>=") + sTmp + _T(" ORDER BY DisplacementSerial"));
				rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
			}
			catch(_com_error e)
			{
				ret=false;
			}

				if( rs1->adoEOF && rs1->BOF )
					//λ�Ƴ���ϵ�����ֵ
					;
				else
				{
					rs1->get_Collect((_variant_t)_T("DisplacementSerial"), &vTmp1);
					tmpDist =vtof(vTmp1);
					//by ligb on 2008.05.09 
					try
					{
						rs1->get_Collect((_variant_t)_T("MovementID"), &vTmp1);//����ֶ����¼ӵģ�ֻ��2007�潭����Դ�������ɲ�������ֶ�ֵ�������������ܳ�����Ҫ�������
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
				
							if(!rs->Find((_bstr_t)(_T("Capacity>=") + sTmp), 0, adSearchForward, vTmp));
							//û�ҵ���������
							else
							{
								//�ҵ���
								rs->get_Collect((_variant_t)_T("minDist"), &vTmp1);
								if( tmpDist < vtof(vTmp1) )
								{
									//����Сλ��С��Ӧ���õ���
									sTmp.Format(GetResStr(IDS_ShouldUseSpring),_T("\%g"),_T("\%g"));
									sTmp2.Format(sTmp,tmpDist,vtof(vTmp1));
									throw sTmp2;
						
								}
								else
								{
									rs->get_Collect((_variant_t)_T("maxDist"), &vTmp1);
									if( tmpDist > vtof(vTmp1) )
									{
										//�����λ�ƻ���
										if( iSumSerialNum >= modPHScal::giWholeSprNum )
										{
											//����ģ���еĴ�����

											//�����Ҫ������ƣ��������������ʹ�ã�,
											//ֻҪһ������Ϳ�����
											//��������λ����Ϊ��������
											modPHScal::gbCSPRneedSpecialDesign = true;
											EDIBgbl::dbPRJ->Execute((_bstr_t)GetResStr(IDS_ESPECIAL_DESIGN_SQL), NULL, adCmdText);
											m_bFoundCSPR = true;
										}
									}
									else
									{
										//����
										//��λ�ƿ�����Ϊ��������
										modPHScal::gbCSPRneedSpecialDesign = false;
										EDIBgbl::dbPRJ->Execute((_bstr_t)_T("UPDATE tmpCSLen SET SpecialDesign=\'\'"), NULL, adCmdText);
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
					rs->get_Collect((_variant_t)_T("Capacity"), &vTmp1);
					C1 = vtof(vTmp1);
					//��¼ǰһ���غ�����
					rs->MovePrevious();
					rs->get_Collect((_variant_t)_T("Capacity"), &vTmp1);
					C2 = vtof(vTmp1);
					if( fabs(C1 - tmpCapacity) >= fabs(C2 - tmpCapacity) );
					//�غ��������ӽ�C2
					else
					{
						//�غ��������ӽ�C1
						//�ص������ӽ��ļ�¼
						rs->MoveNext();
					}
					//�������ֵ
					if(iSEQofSPR<modPHScal::SprInfoIndex)
					{
						modPHScal::sSprInfo[iSEQofSPR].SerialNum = 1;
						rs->get_Collect((_variant_t)_T("dh"), &vTmp1);
						modPHScal::sSprInfo[iSEQofSPR].DH =vtoi(vTmp1);
						modPHScal::sSprInfo[iSEQofSPR].haz = fabs(modPHScal::yr) / modPHScal::giWholeSprNum * modPHScal::sSprInfo[iSEQofSPR].SerialNum;
						modPHScal::sSprInfo[iSEQofSPR].HeatDisp = modPHScal::sSprInfo[iSEQofSPR].haz;
						modPHScal::sSprInfo[iSEQofSPR].CheckDisp = (modPHScal::gbMinMaxDispByActualDisp?modPHScal::sSprInfo[iSEQofSPR].HeatDisp:tmpDist);
						modPHScal::sSprInfo[iSEQofSPR].SumDisp = tmpDist;
						modPHScal::sSprInfo[iSEQofSPR].SumDistID = tmpDistID;
					}
					//ѡ��Ĺ������أ���λΪkgf
					rs->get_Collect((_variant_t)_T("Capacity"), &vTmp1);
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
		   SQLx += (modPHScal::gbCSPRneedSpecialDesign ? _T(" ") : sTmp);
		   SQLx += _T(" AND trim(CustomID)=\'");
		   SQLx +=  Ptype[i];
		   SQLx += _T("\' ORDER BY dh,Weight");
		}

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
		{
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
		}

		break;
	case iSALbraceFixG47:
	case iSALbraceFixG48:    
		{
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
				if(rsTmp->State == adOpenStatic)
					rsTmp->Close();
		try
		{
	//			rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;	
	//			rsTmp.Open(dbOpenSnapshot,SQLx);
				rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
			}
			catch(_com_error e)
			{
				ret=false;
			}
				//���������������Ժ֧�����ֲ�1983���117ҳ����,��"GDW1>="��Ϊ"GDW1=". lgb and pfg20050927
				SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1= ") + ftos(int((modPHScal::gdw+99)/100)*100) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
				if(rsX->State == adOpenStatic)
					rsX->Close();
		try
		{
	//			rsX.Open(dbOpenSnapshot,SQLx);
				rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
				if( rsX->adoEOF && rsX->BOF )
					;
				else
				{
					while (!rsX->adoEOF)
					{
						rsX->get_Collect((_variant_t)_T("P1"), &vTmp1);
						rsX->get_Collect((_variant_t)_T("PmaxSF"), &vTmp3);
						sngSAfixP=vtof(vTmp3);
						if(rsTmp->Find((_bstr_t)(_T("BH=\'")+vtos(vTmp1)+_T("\'")), 0, adSearchForward, vTmp))
						{
							rsTmp->get_Collect((_variant_t)_T("F1ToWx1"), &vTmp2);
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
								rsX->MoveNext();
							}
						}
					}
					if(rsX->adoEOF)
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
				if(rs3->State == adOpenStatic)
					rs3->Close();
		try
		{
	//			rs3.Open(dbOpenSnapshot,SQLx);
				rs3->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
			}
			catch(_com_error e)
			{
				ret=false;
			}
				if(rs3->adoEOF && rs3->BOF)
				{
					//��%s��%s����ѡ��淶��û��%s�ֶ�ֵΪĬ��ֵ%s�ļ�¼(�ǹܲ��������ѡ�����)
					sTmp.Format(IDS_NoDefaultInCustomIDInSpecificationOfMaterial,modPHScal::dbZDJcrude->DefaultDatabase,_T("SpecificationOfMaterial"),_T("ID"),_T("default"));
					throw sTmp;
				}
				else
				{
					rs3->get_Collect((_variant_t)_T("Material"), &vTmp);
					m_strMaterial=vtos(vTmp);
				}

				//����Ӧ��
				float Sigmat=0.0,Sigmat100=0.0,Sigma0=0.0;
				modPHScal::StressOfMaterial(m_strMaterial,modPHScal::t0,Sigmat);
				modPHScal::StressOfMaterial(m_strMaterial,100,Sigmat100);
				modPHScal::StressOfMaterial(m_strMaterial,20,Sigma0);
				//�򿪹̶�֧�ܼ��㹫ʽ��������
				SQLx = _T("SELECT * FROM CalFormulaOfFixPASA WHERE CustomID=\'") + Ptype[i] + _T("\' ORDER BY CustomID,SEQ");
				if(rsCal->State == adOpenStatic)
					rsCal->Close();
		try
		{
	//			rsCal.m_pDatabase=&EDIBgbl::dbSACal;	//20071103 "dbSORT" ��Ϊ "dbSACal"
	//			rsCal.Open(dbOpenSnapshot,SQLx);
				rsCal->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbSACal,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
				if(rsCal->adoEOF && rsCal->BOF)
				{
					//�����ݿ�%s�еĸ���ǿ�ȼ��㹫ʽ��%sû���κμ�¼��
					sTmp.Format(IDS_NullTableCalFormulaOfFixPASA,EDIBgbl::dbSACal->DefaultDatabase,_T("CalFormulaOfFixPASA"));//20071103 "dbSORT" ��Ϊ "dbSACal"
					throw sTmp;
				}
				//�򿪸���ǿ�ȼ��㹫ʽ��������
				SQLx = _T("SELECT * FROM tmpCalFixPhs");
				if(rsTmp1->State == adOpenStatic)
					rsTmp1->Close();
		try
		{
	//			rsTmp1.m_pDatabase=&EDIBgbl::dbPRJ;	
				//rsTmp1��Ҫ�޸Ĺ���
	//			rsTmp1.Open(dbOpenDynaset,SQLx);
				rsTmp1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
				//�򿪲۸����Ա�����
				SQLx = _T("SELECT * FROM SSteelPropertyCS ORDER BY ID");
				if(rsTmp->State == adOpenStatic)
					rsTmp->Close();
		try
		{
	//			rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;	
	//			rsTmp.Open(dbOpenSnapshot,SQLx);
				rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
				//�򿪽Ǹ����Ա�����
				SQLx = _T("SELECT * FROM SSteelPropertyLS ORDER BY ID");
				if(rs1->State == adOpenStatic)
					rs1->Close();
		try
		{
	//			rs1.m_pDatabase=&modPHScal::dbZDJcrude;	
	//			rs1.Open(dbOpenSnapshot,SQLx);
				rs1->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
				//ȡ�������е����㶨λ���������м�¼,��PmaxSF�����������
				SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + ftos(modPHScal::gdw) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
				if(rsX->State == adOpenStatic)
					rsX->Close();
		try
		{
	//			rsX.Open(dbOpenSnapshot,SQLx);
				rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
				if( rsX->adoEOF && rsX->BOF)
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
					_RecordsetPtr rs2;
					//modPHScal::PAfixH=320.0;//����ɲ��Ի���Ժ����
		
	//				rsTmp1.Edit();
					//rsTmp1->put_Collect((_variant_t)_T("CustomIDsa"),COleVariant(Ptype(i)));
					rsTmp1->put_Collect((_variant_t)_T("CustomIDsa"),COleVariant(sPartID));
					rsTmp1->put_Collect((_variant_t)_T("H"),COleVariant(modPHScal::PAfixH));
					rsTmp1->put_Collect((_variant_t)_T("gdw2"),COleVariant(modPHScal::gdw));
					rsTmp1->put_Collect((_variant_t)_T("tj"),COleVariant(modPHScal::t0));
					//����Ӧ����λkgf/cm2->kgf/mm2
					rsTmp1->put_Collect((_variant_t)_T("SIGMAjt"),COleVariant(Sigmat/100.0));
					rsTmp1->put_Collect((_variant_t)_T("SIGMAj20"),COleVariant(Sigma0/100.0));
					rsTmp1->put_Collect((_variant_t)_T("SIGMAj100"),COleVariant(Sigmat100/100.0));
					rsTmp1->Update();
					//����һ��ӿ���£�������ֹ������
		
					//��ù���3
					rsX->get_Collect((_variant_t)_T("P3"), &vTmp);
					strP3=vtos(vTmp);	
	//				rsTmp1.Edit();
					rsTmp1->put_Collect((_variant_t)_T("P3"),vTmp);
					//�˴�ֻ��Ӧ�й���׼�����ʻ�ʱҪ�޸Ĵ���
					n=strP3.Find((_T("x")));
					//����3�ȿ�bb,unit:mm
					bb=_tcstod(strP3.Mid(1,n-1),NULL);
					rsTmp1->put_Collect((_variant_t)_T("bb"),COleVariant(bb));
					//������͸�P1
					rsX->get_Collect((_variant_t)_T("P1"), &vTmp);
					strP1=vtos(vTmp);
					rsTmp1->put_Collect((_variant_t)_T("P1"),vTmp);
					//��ù���2�͸�P2
					rsX->get_Collect((_variant_t)_T("P2"), &vTmp);
					strP2=vtos(vTmp);
					rsTmp1->put_Collect((_variant_t)_T("P2"),vTmp);
					//˫�۸���Ե������PL3,mm
					rsX->get_Collect((_variant_t)_T("PL3"), &vTmp);
					PL3=vtof(vTmp);
					//rsX�е�PL3��CHAR(20),��rsTmp1�е�PL3��REAL,�ɼ������ܹ��Զ�ת����
					rsTmp1->put_Collect((_variant_t)_T("PL3"),vTmp);
					//�۸ֺ���߶�K,mm
					rsX->get_Collect((_variant_t)_T("size2"), &vTmp);
					K=vtof(vTmp);
					rsTmp1->put_Collect((_variant_t)_T("K"),vTmp);
					rsTmp1->Update();
					//���DH,mm
					rsX->get_Collect((_variant_t)_T("dh"), &vTmp);
					dh=vtoi(vTmp);				
		
					//�ȼ�����̬
		
					bool bEOF=false;
					bool bErr=false;//����һ�����������㣬��Ϊtrue
					do
					{
						if(!rsTmp->Find((_bstr_t)(_T("BH=\'")+strP1+_T("\'")), 0, adSearchForward, vTmp))
						{
							//�ڲ۸����Ա�%s��û�ҵ��۸��ͺ�%s
							sTmp.Format(GetResStr(IDS_NotFoundThisCSNo),_T("SSteelPropertyCS"),strP1);
							throw sTmp;
						}
						//����1�۸ֹ��Ծ�Ix,cm4->mm4
						rsTmp->get_Collect((_variant_t)_T("Ix"), &vTmp);
						float J1y=vtof(vTmp)*10000.0;
	//					rsTmp1.Edit();
						rsTmp1->put_Collect((_variant_t)_T("J1y"),COleVariant(J1y));
						//����1�۸ֹ��Ծ�Iy,cm4->mm4
						rsTmp->get_Collect((_variant_t)_T("Iy"), &vTmp);
						float J1z=vtof(vTmp)*10000.0;
						rsTmp1->put_Collect((_variant_t)_T("J1z"),COleVariant(J1z));
						//����1�۸���С���Ծ�,cm4->mm4
						rsTmp1->put_Collect((_variant_t)_T("Imin1"),COleVariant(J1z>J1y?J1y:J1z));
						//����1�۸ֿ����Wx,cm3->mm3
						rsTmp->get_Collect((_variant_t)_T("Wx"), &vTmp);
						float W1y=vtof(vTmp)*1000.0;
						rsTmp1->put_Collect((_variant_t)_T("W1y"),COleVariant(W1y));
						//����1�۸ֿ����Wy,cm3->mm3
						rsTmp->get_Collect((_variant_t)_T("Wy"), &vTmp);
						float W1z=vtof(vTmp)*1000.0;
						rsTmp1->put_Collect((_variant_t)_T("W1z"),COleVariant(W1z));
						//����1�۸ֽ����S,cm2->mm2
						rsTmp->get_Collect((_variant_t)_T("S"), &vTmp);
						float F1=vtof(vTmp)*100.0;
						rsTmp1->put_Collect((_variant_t)_T("F1"),COleVariant(F1));
						//����1�۸ָ߶�h,mm
						rsTmp->get_Collect((_variant_t)_T("h"), &vTmp);
						float h1=vtof(vTmp);
						rsTmp1->put_Collect((_variant_t)_T("h1"),vTmp);
						//����1�۸��ȿ�b,mm
						rsTmp->get_Collect((_variant_t)_T("b"), &vTmp);
						float b1=vtof(vTmp);
						rsTmp1->put_Collect((_variant_t)_T("b1"),vTmp);
						rsTmp1->Update();
			
						if(!rsTmp->Find((_bstr_t)(_T("BH=\'")+strP2+_T("\'")), 0, adSearchForward, vTmp))
						{
							//�ڲ۸����Ա�%s��û�ҵ��۸��ͺ�%s
							sTmp.Format(GetResStr(IDS_NotFoundThisCSNo),_T("SSteelPropertyCS"),strP2);
							throw sTmp;
						}
						//����2�۸ֹ��Ծ�Ix,cm4->mm4
						rsTmp->get_Collect((_variant_t)_T("Ix"), &vTmp);
						float J2y=vtof(vTmp)*10000.0;
	//					rsTmp1.Edit();
						rsTmp1->put_Collect((_variant_t)_T("J2y"),COleVariant(J2y));
						//����2�۸ֹ��Ծ�Iy,cm4->mm4
						rsTmp->get_Collect((_variant_t)_T("Iy"), &vTmp);
						float J2z=vtof(vTmp)*10000.0;
						rsTmp1->put_Collect((_variant_t)_T("J2z"),COleVariant(J2z));
						//����1�۸���С���Ծ�,cm4->mm4
						rsTmp1->put_Collect((_variant_t)_T("Imin2"),COleVariant(J2z>J2y?J2y:J2z));
						//����2�۸ֿ����Wx,cm3->mm3
						rsTmp->get_Collect((_variant_t)_T("Wx"), &vTmp);
						float W2y=vtof(vTmp)*1000.0;
						rsTmp1->put_Collect((_variant_t)_T("W2y"),COleVariant(W2y));
						//����2�۸ֿ����Wy,cm3->mm3
						rsTmp->get_Collect((_variant_t)_T("Wy"), &vTmp);
						float W2z=vtof(vTmp)*1000.0;
						rsTmp1->put_Collect((_variant_t)_T("W2z"),COleVariant(W2z));
						//����2�۸ֽ����S,cm2->mm2
						rsTmp->get_Collect((_variant_t)_T("S"), &vTmp);
						float F2=vtof(vTmp)*100.0;
						rsTmp1->put_Collect((_variant_t)_T("F2"),COleVariant(F2));
						//����2�۸ָ߶�h,mm
						rsTmp->get_Collect((_variant_t)_T("h"), &vTmp);
						float h2=vtof(vTmp);
						rsTmp1->put_Collect((_variant_t)_T("h2"),COleVariant(h2));
						//����2�۸��ȿ�b,mm
						rsTmp->get_Collect((_variant_t)_T("b"), &vTmp);
						float b2=vtof(vTmp);
						rsTmp1->put_Collect((_variant_t)_T("b2"),COleVariant(b2));
			
						//˫�۸־��վ���g,mm
						float g=PL3-2*h1;
						rsTmp1->put_Collect((_variant_t)_T("g"),COleVariant(g));
						//˫�۸����ľ�B1,mm
						float B1=PL3-h1;
						//��ΪrsTmp1�д����ֶ�b1,��Jet���ݿ��ֶ����ǲ����ִ�Сд�ģ��ʻ�B1->Bx,������Ӧ�Ĺ�ʽB1�Ѿ��滻��
						rsTmp1->put_Collect((_variant_t)_T("Bx"),COleVariant(B1));
						//��������׼������
						rsTmp1->Update();					
			
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
				
	//						rsTmp1.Edit();
							rsTmp1->put_Collect((_variant_t)_T("Px"),COleVariant(mfpx));
							rsTmp1->put_Collect((_variant_t)_T("Py"),COleVariant(mfpy));
							rsTmp1->put_Collect((_variant_t)_T("Pz"),COleVariant(mfpz));
							rsTmp1->put_Collect((_variant_t)_T("Mx"),COleVariant(mfmx));
							rsTmp1->put_Collect((_variant_t)_T("My"),COleVariant(mfmy));
							rsTmp1->put_Collect((_variant_t)_T("Mz"),COleVariant(mfmz));
							rsTmp1->Update();
				
							//����1�͹���2����Ŀ����,mm3						
							if(!rsCal->Find((_bstr_t)(_T("SEQ=1")), 0, adSearchForward, vTmp))
							{
								//����%s��%s�������ļ��㹫ʽû�ҵ���
								sTmp.Format(IDS_SACalFormulaXNotFound,Ptype[i],_T("1"));
								throw sTmp;
							}
							else
							{
								if(i4==2)
								{//�������ݣ�ֻ����һ�Σ��ӿ��ٶ�
									rsCal->get_Collect((_variant_t)_T("Wyh1saFormula"), &vTmp);
									Wyh1sa=vtos(vTmp);
									rsCal->get_Collect((_variant_t)_T("Wzh1saFormula"), &vTmp);
									Wzh1sa=vtos(vTmp);
									rsCal->get_Collect((_variant_t)_T("Wyh2saFormula"), &vTmp);
									Wyh2sa=vtos(vTmp);
									rsCal->get_Collect((_variant_t)_T("Wzh2saFormula"), &vTmp);
									Wzh2sa=vtos(vTmp);
						
									//Ӧ���оݿ��Ȼ�ã������Ժ����
									rsCal->get_Collect((_variant_t)_T("SIGMA1saMaxFormula"), &vTmp);
									SIGMA1saMax=vtos(vTmp);
									rsCal->get_Collect((_variant_t)_T("TAO1saMaxFormula"), &vTmp);
									TAO1saMax=vtos(vTmp);
									rsCal->get_Collect((_variant_t)_T("SIGMA2saMaxFormula"), &vTmp);
									SIGMA2saMax=vtos(vTmp);
									rsCal->get_Collect((_variant_t)_T("TAO2saMaxFormula"), &vTmp);
									TAO2saMax=vtos(vTmp);
									rsCal->get_Collect((_variant_t)_T("lamda1Formula"), &vTmp);
									lamda1=vtos(vTmp);
									rsCal->get_Collect((_variant_t)_T("lamda2Formula"), &vTmp);
									lamda2=vtos(vTmp);
						
									//��ʼ���㼸�Ρ���������
									SQLx=_T("SELECT ")+ Wyh1sa+_T(" AS Wyh1sac, ")+ Wyh2sa+_T(" AS Wyh2sac, ") + Wzh1sa+_T(" AS Wzh1sac, ")+ Wzh2sa+_T(" AS Wzh2sac, ")+SIGMA1saMax+_T(" AS SIGMA1saMaxc, ")+TAO1saMax+_T(" AS TAO1saMaxc, ")+SIGMA2saMax+_T(" AS SIGMA2saMaxc, ")+TAO2saMax+_T(" AS TAO2saMaxc, ")+lamda1+_T(" AS lamda1c, ")+lamda2+_T(" AS lamda2c FROM tmpCalFixPhs");
									if(rsTmp2->State == adOpenStatic)
										rsTmp2->Close();
		try
		{
	//								rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
	//								rsTmp2.Open(dbOpenDynaset,SQLx);
									rsTmp2->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
								}
								catch(_com_error e)
								{
									ret=false;
								}
						
	//								rsTmp1.Edit();
									rsTmp2->get_Collect((_variant_t)_T("Wyh1sac"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("Wyh1"),vTmp);
						
									rsTmp2->get_Collect((_variant_t)_T("Wzh1sac"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("Wzh1"),vTmp);
						
									rsTmp2->get_Collect((_variant_t)_T("Wyh2sac"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("Wyh2"),vTmp);
						
									rsTmp2->get_Collect((_variant_t)_T("Wzh2sac"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("Wzh2"),vTmp);							
						
									rsTmp2->get_Collect((_variant_t)_T("TAO1saMaxc"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("TAO1saMax"),vTmp);
									mfTAO1saMax=vtof(vTmp);
									rsTmp2->get_Collect((_variant_t)_T("SIGMA1saMaxc"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("SIGMA1saMax"),vTmp);
									mfSIGMA1saMax=vtof(vTmp);
						
									rsTmp2->get_Collect((_variant_t)_T("TAO2saMaxc"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("TAO2saMax"),vTmp);
									mfTAO2saMax=vtof(vTmp);
									rsTmp2->get_Collect((_variant_t)_T("SIGMA2saMaxc"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("SIGMA2saMax"),vTmp);
									mfSIGMA2saMax=vtof(vTmp);
						
									rsTmp2->get_Collect((_variant_t)_T("lamda1c"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("lamda1"),vTmp);
									mfLamda1=vtof(vTmp);
									rsTmp2->get_Collect((_variant_t)_T("lamda2c"), &vTmp);
									rsTmp1->put_Collect((_variant_t)_T("lamda2"),vTmp);
									mfLamda2=vtof(vTmp);
						
									rsTmp1->Update();
									float mf1=0.0,mf2=0,mfl1=0,mfl2=0;
									SQLx=_T("SELECT Lamda,Fi FROM SteadyDecreaseCoef WHERE Material=\'") + m_strMaterial + _T("\'");
									if(rs2->State == adOpenStatic)
										rs2->Close();
		try
		{
	//								rs2.m_pDatabase=&EDIBgbl::dbSACal;//20071103 "dbSORT" ��Ϊ "dbSACal"
	//								rs2.Open(dbOpenSnapshot,SQLx);
									rs2->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbSACal,true), 
										adOpenDynamic, adLockReadOnly, adCmdText); 
							}
							catch(_com_error e)
							{
								ret=false;
							}
									if(rs2->adoEOF && rs2->BOF)
									{
										//��%s���ȶ��ۼ�ϵ����%s��û��%s���ֲ��ϡ�
										sTmp.Format(IDS_NoMaterialInSteadyDecreaseCoef, EDIBgbl::dbSACal->DefaultDatabase,_T("SteadyDecreaseCoef"),m_strMaterial);
										throw sTmp;
									}
									else
									{
										if(!rs2->Find((_bstr_t)(_T("Lamda>")+ftos(mfLamda1)), 0, adSearchForward, vTmp))
										{
											//�����ϸ����%s����%s��%s�������ֵ��
											sTmp.Format(IDS_LamdaOverMaxValue,ftos(mfLamda1),modPHScal::dbZDJcrude->DefaultDatabase,_T("SteadyDecreaseCoef"));
											throw sTmp;
										}
										else
										{
											rs2->get_Collect((_variant_t)_T("Lamda"), &vTmp);
											mfl2=vtof(vTmp);
											rs2->get_Collect((_variant_t)_T("Fi"), &vTmp);
											mf2=vtof(vTmp);
											rs2->MovePrevious();
											if(rs2->BOF)
											{
												//һ�㲻����,��Ϊ������ֵ��0��ʼ
											}
											else
											{
												//��ֵ��Fi1
												rs2->get_Collect((_variant_t)_T("Lamda"), &vTmp);
												mfl1=vtof(vTmp);
												rs2->get_Collect((_variant_t)_T("Fi"), &vTmp);
												mf1=vtof(vTmp);
												mfFi1=(mf2-mf1)/(mfl2-mfl1)*(mfLamda1-mfl1)+mf1;
	//											rsTmp1.Edit();
												rsTmp1->put_Collect((_variant_t)_T("Fi1"),COleVariant(mfFi1));																					
												rsTmp1->Update();
											}
										}
										//�󹹼�2�ȶ��ۼ�ϵ��Fi2
										if(!rs2->Find((_bstr_t)(_T("Lamda>")+ftos(mfLamda2)), 0, adSearchForward, vTmp))
										{
											//�����ϸ����%s����%s��%s�������ֵ��
											sTmp.Format(IDS_LamdaOverMaxValue,ftos(mfLamda2),modPHScal::dbZDJcrude->DefaultDatabase,_T("SteadyDecreaseCoef"));
											throw sTmp;
										}
										else
										{
											rs2->get_Collect((_variant_t)_T("Lamda"), &vTmp);
											mfl2=vtof(vTmp);
											rs2->get_Collect((_variant_t)_T("Fi"), &vTmp);
											mf2=vtof(vTmp);
											rs2->MovePrevious();
											if(rs2->BOF)
											{
												//һ�㲻����,��Ϊ������ֵ��0��ʼ
											}
											else
											{
												//��ֵ��Fi1
												rs2->get_Collect((_variant_t)_T("Lamda"), &vTmp);
												mfl1=vtof(vTmp);
												rs2->get_Collect((_variant_t)_T("Fi"), &vTmp);
												mf1=vtof(vTmp);
												mfFi2=(mf2-mf1)/(mfl2-mfl1)*(mfLamda2-mfl1)+mf1;
	//											rsTmp1.Edit();
												rsTmp1->put_Collect((_variant_t)_T("Fi2"),COleVariant(mfFi2));																					
												rsTmp1->Update();
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
				
	//						rsTmp1.Edit();
							rsTmp1->put_Collect((_variant_t)_T("Py1"),COleVariant(Py1r1));
							rsTmp1->put_Collect((_variant_t)_T("Py2"),COleVariant(Py1r2));
							rsTmp1->Update();
				
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
				
							if(!rsCal->Find((_bstr_t)(_T("SEQ=")+ltos(i5)), 0, adSearchForward, vTmp))
							{
								//����%s��%s�������ļ��㹫ʽû�ҵ���
								sTmp.Format(IDS_SACalFormulaXNotFound,Ptype[i],_T("1"));
								throw sTmp;
							}
							else
							{
								rsCal->get_Collect((_variant_t)_T("SIGMA1saFormula"), &vTmp);
								SIGMA1sa=vtos(vTmp);
								rsCal->get_Collect((_variant_t)_T("TAOx1saFormula"), &vTmp);
								TAOx1sa=vtos(vTmp);
								rsCal->get_Collect((_variant_t)_T("TAOy1saFormula"), &vTmp);
								TAOy1sa=vtos(vTmp);
								rsCal->get_Collect((_variant_t)_T("TAOz1saFormula"), &vTmp);
								TAOz1sa=vtos(vTmp);
								rsCal->get_Collect((_variant_t)_T("TAO1saFormula"), &vTmp);
								TAO1sa=vtos(vTmp);
					
								rsCal->get_Collect((_variant_t)_T("SIGMA2saFormula"), &vTmp);
								SIGMA2sa=vtos(vTmp);
								rsCal->get_Collect((_variant_t)_T("TAOx2saFormula"), &vTmp);
								TAOx2sa=vtos(vTmp);
								rsCal->get_Collect((_variant_t)_T("TAOy2saFormula"), &vTmp);
								TAOy2sa=vtos(vTmp);
								rsCal->get_Collect((_variant_t)_T("TAOz2saFormula"), &vTmp);
								TAOz2sa=vtos(vTmp);
								rsCal->get_Collect((_variant_t)_T("TAO2saFormula"), &vTmp);
								TAO2sa=vtos(vTmp);
					
								//ѹ�˵ĳ���ϵ��u1,u2:����=2,��֧=0.5							
								rsCal->get_Collect((_variant_t)_T("u1"), &vTmp);
	//							rsTmp1.Edit();
								rsTmp1->put_Collect((_variant_t)_T("u1"),vTmp);
								rsCal->get_Collect((_variant_t)_T("u2"), &vTmp);
								rsTmp1->put_Collect((_variant_t)_T("u2"),vTmp);
								rsTmp1->Update();
					
								//��ʼ������Ӧ��sigma����Ӧ��tao
								SQLx=_T("SELECT ")+ TAOx1sa+_T(" AS TAOx1sac, ")+ TAOy1sa+_T(" AS TAOy1sac, ")+ TAOz1sa+_T(" AS TAOz1sac,")
									+ TAOx2sa+_T(" AS TAOx2sac, ")+ TAOy2sa+_T(" AS TAOy2sac, ")+ TAOz2sa+_T(" AS TAOz2sac FROM tmpCalFixPhs");
								if(rsTmp2->State == adOpenStatic)
									rsTmp2->Close();
		try
		{
	//							rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
	//							rsTmp2.Open(dbOpenDynaset,SQLx);
								rsTmp2->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
									adOpenDynamic, adLockReadOnly, adCmdText); 
							}
							catch(_com_error e)
							{
								ret=false;
							}
					
	//							rsTmp1.Edit();
								rsTmp2->get_Collect((_variant_t)_T("TAOx1sac"), &vTmp);
								rsTmp1->put_Collect((_variant_t)_T("TAOx1sa"),vTmp);
								rsTmp2->get_Collect((_variant_t)_T("TAOy1sac"), &vTmp);
								rsTmp1->put_Collect((_variant_t)_T("TAOy1sa"),vTmp);
								rsTmp2->get_Collect((_variant_t)_T("TAOz1sac"), &vTmp);
								rsTmp1->put_Collect((_variant_t)_T("TAOz1sa"),vTmp);
					
								rsTmp2->get_Collect((_variant_t)_T("TAOx2sac"), &vTmp);
								rsTmp1->put_Collect((_variant_t)_T("TAOx2sa"),vTmp);
								rsTmp2->get_Collect((_variant_t)_T("TAOy2sac"), &vTmp);
								rsTmp1->put_Collect((_variant_t)_T("TAOy2sa"),vTmp);
								rsTmp2->get_Collect((_variant_t)_T("TAOz2sac"), &vTmp);
								rsTmp1->put_Collect((_variant_t)_T("TAOz2sa"),vTmp);
								rsTmp1->Update();
					
								SQLx=_T("SELECT ")+ TAO1sa+_T(" AS TAO1sac,")+ TAO2sa+_T(" AS TAO2sac,")+SIGMA1sa+_T(" AS SIGMA1sac,")+SIGMA2sa+_T(" AS SIGMA2sac FROM tmpCalFixPhs");
								if(rsTmp2->State == adOpenStatic)
									rsTmp2->Close();
		try
		{
	//							rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
	//							rsTmp2.Open(dbOpenDynaset,SQLx);
								rsTmp2->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
									adOpenDynamic, adLockReadOnly, adCmdText); 
				}
				catch(_com_error e)
				{
					ret=false;
				}
			
								rsTmp2->get_Collect((_variant_t)_T("SIGMA1sac"), &vTmp);
								mfSIGMA1sa=vtof(vTmp);
	//							rsTmp1.Edit();
								rsTmp1->put_Collect((_variant_t)_T("SIGMA1sa"),vTmp);
					
								rsTmp2->get_Collect((_variant_t)_T("SIGMA2sac"), &vTmp);
								mfSIGMA2sa=vtof(vTmp);
								rsTmp1->put_Collect((_variant_t)_T("SIGMA2sa"),vTmp);
					
								rsTmp2->get_Collect((_variant_t)_T("TAO1sac"), &vTmp);
								mfTAO1sa=vtof(vTmp);							
								rsTmp1->put_Collect((_variant_t)_T("TAO1sa"),vTmp);
					
								rsTmp2->get_Collect((_variant_t)_T("TAO2sac"), &vTmp);
								mfTAO2sa=vtof(vTmp);
								rsTmp1->put_Collect((_variant_t)_T("TAO2sa"),vTmp);
								rsTmp1->Update();
					
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
							rsX->MoveNext();
							if(rsX->adoEOF)
							{
								//�Ѿ�����¼β������ѭ��һ�Σ��Ա㱨��
								bEOF=true;
								continue;
							}
							else
								//���¿�ʼ֮ǰ�����ó��ޱ�־
								bErr=false;
				
							//��ù���3
							rsX->get_Collect((_variant_t)_T("P3"), &vTmp);
							strP3=vtos(vTmp);	
	//						rsTmp1.Edit();
							rsTmp1->put_Collect((_variant_t)_T("P3"),vTmp);
							//�˴�ֻ��Ӧ�й���׼�����ʻ�ʱҪ�޸Ĵ���
							n=strP3.Find((_T("x")));
							//����3�ȿ�bb,unit:mm
							bb=_tcstod(strP3.Mid(1,n-1),NULL);
							rsTmp1->put_Collect((_variant_t)_T("bb"),COleVariant(bb));
							//������͸�P1
							rsX->get_Collect((_variant_t)_T("P1"), &vTmp);
							strP1=vtos(vTmp);
							rsTmp1->put_Collect((_variant_t)_T("P1"),vTmp);
							//��ù���2�͸�P2
							rsX->get_Collect((_variant_t)_T("P2"), &vTmp);
							strP2=vtos(vTmp);
							rsTmp1->put_Collect((_variant_t)_T("P2"),vTmp);
							//˫�۸���Ե������PL3,mm
							rsX->get_Collect((_variant_t)_T("PL3"), &vTmp);
							PL3=vtof(vTmp);
							//rsX�е�PL3��CHAR(20),��rsTmp1�е�PL3��REAL,�ɼ������ܹ��Զ�ת����
							rsTmp1->put_Collect((_variant_t)_T("PL3"),vTmp);
							//�۸ֺ���߶�K,mm
							rsX->get_Collect((_variant_t)_T("size2"), &vTmp);
							K=vtof(vTmp);
							rsTmp1->put_Collect((_variant_t)_T("K"),vTmp);
							rsTmp1->Update();
							//���DH,mm
							rsX->get_Collect((_variant_t)_T("dh"), &vTmp);
							dh=vtoi(vTmp);				
						}
					}while(true);//��¼β��ѭ��һ��,�Ա㱨��
		
					//����һ������У�˵����ݿ��ļ�����Ӧ��Excel�ļ�
					EDIBgbl::ChangeColumnsToRows(EDIBgbl::dbPRJ,_T("tmpCalFixPhs"),_T("CheckCalFixPhs"));
					EDIBDB::OutPutTable( _T("CheckCalFixPhs"), basDirectory::ProjectDBDir+_T("CheckCalculation.xls"), _T("Excel 5.0;"), EDIBgbl::dbPRJ,  _T("CheckCalFixPhs"), _T("Excel 5.0;"));
		
					SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + ftos(modPHScal::gdw) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' AND dh=") + ltos(dh) + _T(" ORDER BY GDW1,PmaxSF");
				}
				//����ǿ�ȼ���:����Ժ�̶�֧�ܸ���˫�����Ǽ�SJ8(ClassID=iSALbraceFixG47=4)
			}	
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
						if(rsX->State == adOpenStatic)
							rsX->Close();
	try
	{
//						rsX.Open(dbOpenSnapshot,SQLx);
						rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
							adOpenDynamic, adLockReadOnly, adCmdText); 
					}
					catch(_com_error e)
					{
						ret=false;
					}
						if( rsX->adoEOF && rsX->BOF )
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
								if(rsX->State == adOpenStatic)
									rsX->Close();
	try
	{
//								rsX.Open(dbOpenSnapshot,SQLx);
								rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
									adOpenDynamic, adLockReadOnly, adCmdText); 
							}
							catch(_com_error e)
							{
								ret=false;
							}
								if( rsX->adoEOF && rsX->BOF )
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
				  if(rsX->State == adOpenStatic)
					  rsX->Close();
	try
	{
//				  rsX.Open(dbOpenSnapshot,SQLx);
				  rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
					  adOpenDynamic, adLockReadOnly, adCmdText); 
			  }
			  catch(_com_error e)
			  {
				  ret=false;
			  }
				  p1 = tmpSelPJG / 2;
				  p2 = 0;
				  strTmp = GetBHforDoubleCSBeam(modPHScal::gdw / 10, modPHScal::DblHangerRodDist / 10, modPHScal::Lspan / 10, p1, p2, tmpCustomID1, 2, FirstCal);
			  }
		   }
		   else
		   {
			   //�����㣬������
			   if(rsX->State == adOpenStatic)
				   rsX->Close();
	try
	{
//			   rsX.Open(dbOpenSnapshot,SQLx);
			   rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				   adOpenDynamic, adLockReadOnly, adCmdText); 
		   }
		   catch(_com_error e)
		   {
			   ret=false;
		   }
			   p1 = tmpSelPJG;
			   p2 = 0;
			   strTmp = GetBHforDoubleCSBeam(modPHScal::gdw / 10, modPHScal::DblHangerRodDist / 10, modPHScal::Lspan / 10, p1, p2, tmpCustomID1, 2, FirstCal);
		   }
		}
		else
		{
			//�����ط�ʽ�������
			if(rsX->State == adOpenStatic)
				rsX->Close();
	try
	{
//			rsX.Open(dbOpenSnapshot,SQLx);
			rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
		}
	}
	else
	{
		if(rsX->State == adOpenStatic)
			rsX->Close();
	try
	{
//		rsX.Open(dbOpenSnapshot,SQLx);
		rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
									adOpenDynamic, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}
	}
spZ1Z2:
	//�����Ҫ�������ӿס��˳ߴ��Զ����, Ҫ��׳ߴ�ƥ����в���ʱ
	//�Ҳ���ƥ���Ӧȥ�����������ٲ��� add by luorijin 2009.05.12
	if( modPHScal::gbConnectHoleDimMatch && tmpSQL != _T("") && rsX->adoEOF && rsX->BOF )
	{
		if(rsX->State == adOpenStatic)
			rsX->Close();
		SQLx.Replace(tmpSQL," ");
	try
	{
//			rsX.Open(dbOpenSnapshot,SQLx);
			rsX->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}
	}
	if( rsX->adoEOF && rsX->BOF )
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
			if(rsX->State == adOpenStatic)
				rsX->Close();
	try
	{
//			rsX.Open(dbOpenSnapshot,tmpSQL);
			rsX->Open((_bstr_t)tmpSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
			if( !rsX->adoEOF )
			{
				goto spZ1Z2;
			}
			else
			{
				if( modPHScal::gbAutoApplyCSPRLugDia && (FirstCal == 2) && modPHScal::gbPhsIsCSPR && modPHScal::PtypeDiameter[i] > 0 )
				{
					sTmp.Format(GetResStr(IDS_NotClearCheckAutoApplyCSPRLugDia),modPHScal::dbZDJcrude->DefaultDatabase,modPHScal::tbnPART, SQLx);
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
			rsX->Close();
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
		rsX->MoveFirst();
		if(phsAvailableTypeRs->State == adOpenStatic)
			phsAvailableTypeRs->Close();
	try
	{
//		phsAvailableTypeRs.m_pDatabase=rsX.m_pDatabase;
//		phsAvailableTypeRs.Open(dbOpenDynaset,rsX.GetSQL());
		phsAvailableTypeRs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
									adOpenDynamic, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}
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
						rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
						sTmp.Format(_T(" PmaxKgf>=%g "),
							(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ + vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
					}
					else
					{
						sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
					}
					rsX->get_Collect((_variant_t)_T("CustomID"), &vTmp2);
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
						rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
						sTmp.Format(_T(" PmaxKgf>=%g "),
							(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ +vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
					}
					else
					{
						sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
					}
					rsX->get_Collect((_variant_t)_T("CustomID"), &vTmp2);
					rsX->get_Collect((_variant_t)_T("Fjmin"), &vTmp3);
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
						rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
						sTmp.Format(_T(" PmaxKgf>=%g "),
							(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ +vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
					}
					else
					{
						sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
					}
					rsX->get_Collect((_variant_t)_T("CustomID"), &vTmp2);
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
						rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
						sTmp.Format(_T(" PmaxKgf>=%g "),
							(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ +vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
					}
					else
					{
						sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
							modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
					}
					rsX->get_Collect((_variant_t)_T("CustomID"), &vTmp2);
					rsX->get_Collect((_variant_t)_T("Fjmin"), &vTmp3);
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
			if(rsDiaOfCSPRFiJ->State == adOpenStatic)
				rsDiaOfCSPRFiJ->Close();
	try
	{
//			rsDiaOfCSPRFiJ.Open(dbOpenSnapshot,SQLx);
			rsDiaOfCSPRFiJ->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}
			if( rsDiaOfCSPRFiJ->adoEOF && rsDiaOfCSPRFiJ->BOF )
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
							rsDiaOfCSPRFiJ->get_Collect((_variant_t)_T("FiJ"), &vTmp1);
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
							rsDiaOfCSPRFiJ->get_Collect((_variant_t)_T("FiJ"), &vTmp1);
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
			rsX->get_Collect((_variant_t)_T("dh"), &vTmp1);
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
				rsX->get_Collect((_variant_t)_T("BH"), &vTmp1);
				//���ɹ���
				modPHScal::sSprInfo[iSEQofSPR].BH=vtos(vTmp1);
				//��õ��������ɽ���ĸֱ��
				rsX->get_Collect((_variant_t)_T("size2"), &vTmp1);
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
			rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
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
				rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
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
				rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
				sTmp.Format(_T(" PmaxKgf>=%g "),
					(sPartID == _T("LHG") ? tmpSelPJG4CSPRFiJ +vtof(vTmp1) * modPHScal::gintParallelNum : tmpSelPJG4CSPRFiJ) / modPHScal::gintParallelNum);
			}
			else
			{
				sTmp.Format(_T(" fBmin<=%g AND fBmax>=%g"),
					modPHScal::sSprInfo[iSEQofSPR].CheckDisp,
					modPHScal::sSprInfo[iSEQofSPR].CheckDisp);
			}
			rsX->get_Collect((_variant_t)_T("CustomID"), &vTmp2);
			SQLx.Format( _T("SELECT * FROM [%s] WHERE %s  AND FiJ>=%d AND minDH<=%d AND maxDH>=%d  AND trim(CustomID)=\'%s\' %s"),
				modPHScal::tbnLugDiaOfCSPR,
				sTmp,
				(modPHScal::PtypeDiameter[i] <= 0 ? 10 : modPHScal::PtypeDiameter[i]),
				modPHScal::sSprInfo[iSEQofSPR].DH,
				modPHScal::sSprInfo[iSEQofSPR].DH,
				vtos(vTmp2),
				(modPHScal::gbByForce ? _T(" ORDER BY Pmax,FiJ") : _T(" ORDER BY FiJ")));

			if(rsDiaOfCSPRFiK->State == adOpenStatic)
				rsDiaOfCSPRFiK->Close();
	try
	{
	//		rsDiaOfCSPRFiK.Open(dbOpenSnapshot,SQLx);
			rsDiaOfCSPRFiK->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbSACal,true), 
				adOpenDynamic, adLockReadOnly, adCmdText); 
		}
		catch(_com_error e)
		{
			ret=false;
		}
			if( rsDiaOfCSPRFiK->adoEOF && rsDiaOfCSPRFiK->BOF )
			{
				sTmp.Format(GetResStr(IDS_NotFoundRecordInLugDiaOfCSPR), modPHScal::tbnLugDiaOfCSPR, SQLx);
				throw sTmp;
			}
			rsDiaOfCSPRFiJ->get_Collect((_variant_t)_T("FiJ"), &vTmp1);
			modPHScal::sSprInfo[iSEQofSPR].FiJ = vtof(vTmp1);
			rsDiaOfCSPRFiK->get_Collect((_variant_t)_T("FiK"), &vTmp1);
			modPHScal::sSprInfo[iSEQofSPR].FiK = vtof(vTmp1);
		}
		rsX->get_Collect((_variant_t)_T("CustomID"), &vTmp1);

		if(rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")), 0, adSearchForward, vTmp))
		{
			rsID->get_Collect((_variant_t)_T("BHFormat"), &vTmp1);
			sBHFormat = vtos(vTmp1);
		}
		if( sBHFormat.GetLength()<=0 )
		{
			sBHFormat = _T("trim(BH)");
		}
		rsX->get_Collect((_variant_t)_T("BH"), &vTmp1);
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
								rsDiaOfCSPRFiK->get_Collect((_variant_t)tmpFD0, &vTmp1);
								tmpSQL0 =tmpSQL0.Left(n) + (vTmp1.vt==VT_NULL || vTmp1.vt==VT_EMPTY ? _T("0") : vtos(vTmp1)) + tmpSQL0.Mid( m);
							}
							else
							{
								rsX->get_Collect((_variant_t)tmpFD0, &vTmp1);
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
		//	rsTZB->AddNew();
		int iTmpRecno;
		rsTZB->get_Collect((_variant_t)_T("recno"), &vTmp);
		iTmpRecno=vtoi(vTmp);
		dbstr = dbstr +"iNnerRecno = " + ltos(iTmpRecno);
		rsTZB->get_Collect((_variant_t)_T("ID"), &vTmp);
		dbstr=dbstr+"  ID= "+vtos(vTmp);
		//rsTZB->Find((_bstr_t)"recno="+ltos(iTmpRecno));
	//	rsTZB.Edit();
		rsTZB->put_Collect((_variant_t)_T("nth"),COleVariant((short)nth));//��׼֧����·��=1�������Գ�˫���͹��ø���˫��

		dbstr = dbstr + "  FirstCal: "+ltos(FirstCal) + " nth: " + ltos(nth);

		//��ǰ���������һ�����(L8)�ĵĳ��ȳߴ��ֶ�
		rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp1);
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
					rsX->get_Collect((_variant_t)tmpExtLenFD, &vTmp2);
					rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
				}
				////////////////////////////////////////////
				if( PtypeID[i + 1] == _T("L8") || PtypeID[i + 1] == _T("L8Dd"))
				{
					rsX->get_Collect((_variant_t)tmpExtLenFD, &vTmp2);
					rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
					rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp2);
					tmpExtLenFD = vtos(vTmp2);
				}
				////////////////////////////////////////
			}
		}
		else
		{
			if(sPartID.Left(2) == _T("L7")||sPartID.Left(2) == _T("G12")||sPartID == _T("L3")||sPartID == _T("L4")||sPartID == _T("T1")||sPartID == _T("D4")||sPartID == _T("D5")||sPartID == _T("D11"))
			{
				rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp2);
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
						rsX->get_Collect((_variant_t)tmpExtLenFD, &vTmp2);
						rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
						//�ٲ��ֶ�����
						rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp2);
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
								rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp2);
								tmpExtLenFD = vtos(vTmp2);
								rsDiaOfCSPRFiK->get_Collect((_variant_t)tmpExtLenFD, &vTmp2);
								rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
							}
						}
						else
						{
							rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp2);
							tmpExtLenFD = vtos(vTmp2);
							rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),COleVariant((long)tmpExtLenValue));
						}
					}
				}
				//////////////////////////////////////////////////////////////////////
				if( PtypeID[i + 1] == _T("L8") || PtypeID[i + 1] == _T("L8Dd") )
				{
					rsX->get_Collect((_variant_t)tmpExtLenFD, &vTmp2);
					rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),COleVariant((long)vtoi(vTmp2)));
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
						rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp1);
						tmpExtLenFD = vtos(vTmp1);
					}
					else
					{
						//���ݲ�����ֶ�ֱֵ�Ӳ��
						rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp1);
						tmpExtLenFD = vtos(vTmp1);
						rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),COleVariant((long)tmpExtLenValue));
					}
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
								rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp1);
								tmpExtLenFD = vtos(vTmp1);
								rsDiaOfCSPRFiK->get_Collect((_variant_t)tmpExtLenFD, &vTmp1);
								rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),vTmp1);
							}
						}
						else
						{
							rsConnect->get_Collect((_variant_t)_T("CNTBExtLenFLD"), &vTmp1);
							tmpExtLenFD = vtos(vTmp1);
							rsTZB->put_Collect((_variant_t)_T("PreviousPartInsertLen"),COleVariant((long)tmpExtLenValue));
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
					rsX->get_Collect((_variant_t)_T("sizeC"), &vTmp1);
					modPHScal::DblHangerRodDist =vtof(vTmp1);
					rsTZB->put_Collect((_variant_t)_T("sizeC"),COleVariant((float)modPHScal::DblHangerRodDist));
				}
				else
				{
					if( modPHScal::glClassID == iPAs )
					{
						//˫֧
						rsX->get_Collect((_variant_t)_T("size3"), &vTmp1);
						modPHScal::DblHangerRodDist =vtof(vTmp1);
						rsTZB->put_Collect((_variant_t)_T("sizeC"),COleVariant((float)modPHScal::DblHangerRodDist));
					}
					else
					{
						if( modPHScal::glClassID == iPAD11LD || modPHScal::glClassID == iPADLR || modPHScal::glPAid == iPALX )
						{
							//��ܾ��йص�˫�����
							rsX->get_Collect((_variant_t)_T("sizeC"), &vTmp1);
							modPHScal::DblHangerRodDist =vtof(vTmp1) + modPHScal::dj;
							rsTZB->put_Collect((_variant_t)_T("sizeC"),COleVariant((float)modPHScal::DblHangerRodDist));
						}
					}
				}
			}
		}

	   //���ں���򵯻ɲ�����SQL�����ķ�����ñ�ţ���ȥ�����е����Ӳ�������
	   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET BH=\'") + sBH + _T("\'")), NULL, adCmdText);
	   rsTZB->put_Collect((_variant_t)_T("BH"),STR_VAR(sBH));
	   //���ںᵣ����T5��ᵣ���LHG��������ݹܲ�D13/D14�ҳ������ľࡣ
	   //�����·�����T3A��ᵣ����T5��ᵣ���LHG�������Ȳ�������ֱ����
	   //������˨��ĸ�Ĺ��ͨ��GetBoltsNutsAndAttachmentsCLgg��������
	   sTmp.Format(_T("%d"),modPHScal::PtypeDiameter[i]);
	   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET LugDiameter=") + sTmp), NULL, adCmdText);
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
		   //if( rs->adoEOF And rs->BOF ){
		   sTmp.Format(_T("%d"),modPHScal::PtypeDiameter[i]);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET LugDiameter=") + sTmp), NULL, adCmdText);
		   //EDIBgbl::dbPRJ.Execute _T("UPDATE tmpCSLen SET SumDist=") + IIf(tmpDist > IIf(IsNull(rsX(_T("fBmax"))), 0, rsX(_T("fBmax"))), IIf(IsNull(rsX(_T("fBmax"))), 0, rsX(_T("fBmax"))), tmpDist)
		   sTmp.Format(_T("%g"),tmpDist);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SumDist=") +sTmp), NULL, adCmdText);
		   sTmp.Format(_T("%d"),tmpDistID);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SumDistID=") +sTmp), NULL, adCmdText);//���ڻ�糧��ˮ�ܵ�֧����2007�������������ɣ�����λ�ƴ���
		   rsX->get_Collect((_variant_t)_T("sizeC"), &vTmp1);
		   sTmp.Format(_T("%g"),(sPartID == _T("LHG") ? modPHScal::DblHangerRodDist : vtof(vTmp1)));
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET sizeC=") + sTmp), NULL, adCmdText);
		   //�������ľ࣬����T5/LHG�������ֱ�����ٴ����ɹ��CLgg
		   modPHScal::sSprInfo[iSEQofSPR].sizeC=_ttoi(sTmp);
		   sTmp.Format(_T("%g"),modPHScal::sSprInfo[iSEQofSPR].HeatDisp);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET HeatDist=") + sTmp), NULL, adCmdText);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET UnitForce=\'") +modPHScal::gsCSPRUnitOfLoad + _T("\'")), NULL, adCmdText);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET DistFX=\'") + (modPHScal::yr > 0 ? modPHScal::gSprDirectionChar.CSPR_UP : modPHScal::gSprDirectionChar.CSPR_Down) + _T("\'")), NULL, adCmdText);
		   if(modPHScal::gsCSPRUnitOfLoad == _T("daN") )
		   {
			   //SelOpLoad=ѡ��Ĺ�������,����λ��ԣ��֮����õ��غ�ֵtmpSelOpLoad
			   sTmp.Format(_T("%d"),(long)(tmpSelOpLoad * EDIBgbl::kgf2N / 10));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SelOpLoad=") + sTmp), NULL, adCmdText);  //���ݵ�����е������ITT��������λΪdaN����������ΪN��
	   
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum * EDIBgbl::kgf2N / 10 ));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET OpLoad=") + sTmp), NULL, adCmdText); //���ݵ�����е������ITT��������λΪdaN����������ΪN��
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum * EDIBgbl::kgf2N / 10));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET InstallLoad=") + sTmp), NULL, adCmdText); //���ݵ�����е������ITT��������λΪdaN����������ΪN��
		   }
		   else if(modPHScal::gsCSPRUnitOfLoad == _T("N") )
		   {
			   //SelOpLoad=ѡ��Ĺ�������,����λ��ԣ��֮����õ��غ�ֵtmpSelOpLoad
			   sTmp.Format(_T("%d"),(long)(tmpSelOpLoad * EDIBgbl::kgf2N));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SelOpLoad=") +sTmp), NULL, adCmdText);
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")? tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum * EDIBgbl::kgf2N));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET OpLoad=") + sTmp), NULL, adCmdText);
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum * EDIBgbl::kgf2N));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET InstallLoad=") + sTmp), NULL, adCmdText);
		   }
		   else
		   {
			   //SelOpLoad=ѡ��Ĺ�������,����λ��ԣ��֮����õ��غ�ֵtmpSelOpLoad
			   sTmp.Format(_T("%d"),(long)(tmpSelOpLoad));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SelOpLoad=") +sTmp), NULL, adCmdText);
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz )/modPHScal::gintParallelNum));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET OpLoad=") + sTmp), NULL, adCmdText);
			   sTmp.Format(_T("%d"),(long)((sPartID==_T("LHG")?tmpSelPJG4CSPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4CSPRPgz) /modPHScal::gintParallelNum));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET InstallLoad=") + sTmp), NULL, adCmdText);
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
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SerialNum=") + sTmp), NULL, adCmdText);
		   sTmp.Format(_T("%d"),modPHScal::sSprInfo[iSEQofSPR].SprNo);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SprNo=") + sTmp), NULL, adCmdText);
		   sTmp.Format(_T("%d"),modPHScal::gSumDistOfSpring);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SumDist=") +sTmp), NULL, adCmdText);
		   sTmp.Format(_T("%d"),modPHScal::sSprInfo[iSEQofSPR].SumDistID);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET SumDistID=") +sTmp), NULL, adCmdText);
		   if( (PtypeID[i-1] == _T("D13") || PtypeID[i-1] == _T("D14")) && (sPartID == _T("T5") || sPartID == _T("TDG") || sPartID == _T("LHG")) )
		   {
			   //�·����ɻ��·��������ɵ����ľ��ǹܲ����ľ�C�����ģ�
			   //���CֵҪд�뵯�ɹ���У���Ϊ��������
			   sTmp.Format(_T("%g"),modPHScal::DblHangerRodDist);
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET sizeC=") + sTmp), NULL, adCmdText);
			   //�������ľ࣬����T5/LHG�������ֱ�����ٴ����ɹ��CLgg
			   modPHScal::sSprInfo[iSEQofSPR].sizeC=_ttoi(sTmp);
		   }		  
		   sTmp.Format(_T("%g"),modPHScal::sSprInfo[iSEQofSPR].HeatDisp);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET HeatDist=") + sTmp), NULL, adCmdText);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET UnitForce=\'") +modPHScal::gsSPRUnitOfLoad + _T("\'")), NULL, adCmdText);
		   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET DistFX=\'") + (modPHScal::yr > 0 ? modPHScal::gSprDirectionChar.SPR_UP : modPHScal::gSprDirectionChar.SPR_Down) + _T("\'")), NULL, adCmdText);
		   if(modPHScal::gsSPRUnitOfLoad == _T("daN") )
		   {
			   //���صĵ�λ��daN(10N)
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPgz) / iNumPart * EDIBgbl::kgf2N / 10));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET OpLoad=") + sTmp), NULL, adCmdText); //���ݵ�����е������ITT��������λΪdaN����������ΪN��
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPaz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPaz) / iNumPart * EDIBgbl::kgf2N / 10));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET InstallLoad=") + sTmp), NULL, adCmdText); //���ݵ�����е������ITT��������λΪdaN����������ΪN��
		   }
		   else if(modPHScal::gsSPRUnitOfLoad == _T("N") )
		   {
			   //���صĵ�λ��N
			   //���������������룬������ֺ�����ֵ��һ�µ����,��bug���ȱ�����Ժ���㷢��
			   //(����,pgz=9000N,yr=14mm,�����䲻��0.5�����"T2.110,14����/8999,M20")
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPgz) / iNumPart * EDIBgbl::kgf2N+0.5));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET OpLoad=") + sTmp), NULL, adCmdText);
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPaz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPaz) / iNumPart * EDIBgbl::kgf2N+0.5));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET InstallLoad=") + sTmp), NULL, adCmdText);
		   }
		   else
		   {
			   //���صĵ�λ��kgf
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPgz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPgz) / iNumPart));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET OpLoad=") + sTmp), NULL, adCmdText);
			   sTmp.Format(_T("%d"),(long)(fabs(sPartID==_T("T5")?tmpSelPJG4SPRPaz + modPHScal::g_fLHGWeight : tmpSelPJG4SPRPaz) / iNumPart));
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET InstallLoad=") + sTmp), NULL, adCmdText);
		   }
	   }	   
	   if(modPHScal::glIDIndex != iSA )
	   {
		   if(modPHScal::glIDIndex == iPA )
		   {
			   //�¶ȿ�����Ϊ��־�ܲ�����Ϣ
			   sTmp.Format(_T("%g"),modPHScal::t0);
			   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET t0=") + sTmp), NULL, adCmdText);
			   rsX->get_Collect((_variant_t)_T("Material"), &vTmp1);
			   rsTZB->put_Collect((_variant_t)_T("CLcl"),vTmp1);
			   if(modPHScal::glClassID==iPAfixZ2)
			   {
				   //�̶�֧��Z2A/Z2/SZ5�ܲ���������߶�k1(size4),k2(size5)--mm
				   rsX->get_Collect((_variant_t)_T("size4"), &vTmp1);				 
				   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET K=") + vtos(vTmp1)), NULL, adCmdText);
				   rsX->get_Collect((_variant_t)_T("size5"), &vTmp1);				 
				   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET K2=") + vtos(vTmp1)), NULL, adCmdText);
				   //�̶�֧��Z2A/Z2/SZ5�ܲ�֧�ܹܾ�DW1xS(sizeC x size3),�����⾶DW--mm,����Ժ�ֲ���Ҫ��������ǡ�
				   rsX->get_Collect((_variant_t)_T("DW"), &vTmp1);				 
				   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET DW=") + vtos(vTmp1)), NULL, adCmdText);
				   rsX->get_Collect((_variant_t)_T("sizeC"), &vTmp1);				 
				   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET dw1=") + vtos(vTmp1)), NULL, adCmdText);
				   rsX->get_Collect((_variant_t)_T("size3"), &vTmp1);				 
				   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET S=") + vtos(vTmp1)), NULL, adCmdText);
				   //SZ5�������Ҫ�¶�t0/�ܾ�dj			 
				   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET temperature=") + ftos(modPHScal::t0)), NULL, adCmdText);
			   }
		   }
   
		   SQLx = _T("SELECT (") + sBHFormat + _T(") AS sBHFormat  FROM tmpCSLen");
		   if(rsTmp->State == adOpenStatic)
			   rsTmp->Close();
	try
	{
//		   rsTmp.m_pDatabase=&EDIBgbl::dbPRJ;
//		   rsTmp.Open(dbOpenDynaset,SQLx);
		   rsTmp->Open((_bstr_t)strSQL,_variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			   adOpenForwardOnly, adLockReadOnly, adCmdText); 
	   }
	   catch(_com_error e)
	   {
		   ret=false;
	   }
		   rsTmp->get_Collect((_variant_t)_T("sBHFormat"), &vTmp1);
		   sBHFormat =vtos(vTmp1);
		   rsTZB->put_Collect((_variant_t)_T("BHformat"),STR_VAR(sBHFormat));
		   if(modPHScal::glClassID == iSPR || modPHScal::glClassID == iCSPR )
		   {
			   rsTZB->put_Collect((_variant_t)_T("CLgg"),STR_VAR(sBHFormat));
			   //���浯��/����������Ϣ����T5/LHG�������ɱ�ţ���������ɶ������
			   modPHScal::sSprInfo[iSEQofSPR].CustomID=Ptype[i];
			   modPHScal::sSprInfo[iSEQofSPR].BHFormat=sBHFormat;
			   modPHScal::sSprInfo[iSEQofSPR].FiJ=modPHScal::PtypeDiameter[i];
		   }
		   else if(modPHScal::glClassID == iROD || modPHScal::glClassID == iXL1 )
			   //���˵Ĺ����Ҫ�ڼ��㳤�Ⱥ�ȷ�����˴����븳��CLgg�ֶ���ȷ��BH��
			   //��CalLugLength��������Ҫ��ȷ�ĸ���BHֵ��CLggֵ��
		   {
			   rsTZB->put_Collect((_variant_t)_T("CLgg"),STR_VAR(sBH));
		   }
		   else if( modPHScal::glIDIndex ==iPA && modPHScal::g_bPipeClampBoltDiameterChanged)
			   //С���ش�ܾ��ܼа�����ѡ����˨��ֱ��M1��С���ܼй���б���ע����M1ֵ
		   {
			   //ȡ���ܼ���˨M1��ֵ
			   rsX->get_Collect((_variant_t)strDiaM1FieldName, &vTmp1);
			   m_fDiaM1 = vtof(vTmp1);
			   //�ܼ���˨M1��U�ζ���L8����˨M��������
			   CString strSQLTmp ;
				strSQLTmp.Format("SELECT * FROM [%s] WHERE [CUSTOMID]=\'%s\' AND [SIZE2]=%d ORDER BY SIZE2",
					modPHScal::tbnPART,modPHScal::sFindCustomID (_T("L8")),int(modPHScal::gmiDiameter));			   
				_RecordsetPtr m_rs;
			   COleVariant vT1;
			   if(m_rs->State == adOpenStatic)
				   m_rs->Close();
	   
	try
	{
//			   m_rs.m_pDatabase=&modPHScal::dbZDJcrude;
//			   m_rs.Open(dbOpenSnapshot,strSQLTmp);
			   m_rs->Open((_bstr_t)strSQL,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
				   adOpenForwardOnly, adLockReadOnly, adCmdText); 
		   }
		   catch(_com_error e)
		   {
			   ret=false;
		   }
			   if((!m_rs->BOF) && !(m_rs->adoEOF))
			   {
				   m_rs->get_Collect((_variant_t)_T("size3"),vT1);
				   int x = vtoi(vT1);
				   //����ܼ���˨��M1��ֵ����Ҫ��L8����˨M��			   
				   if(m_fDiaM1 > x)
				   {
						strSQLTmp.Format("SELECT * FROM [%s] WHERE [CUSTOMID]=\'%s\' AND [SIZE2]=%d ORDER BY SIZE2",
							modPHScal::tbnPART,modPHScal::sFindCustomID (_T("L7")),int(modPHScal::gmiDiameter));
						m_rs->Close();
	try
	{
//						m_rs.Open(dbOpenSnapshot,strSQLTmp);
						m_rs->Open((_bstr_t)strSQLTmp,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
							adOpenForwardOnly, adLockReadOnly, adCmdText); 
				   }
				   catch(_com_error e)
				   {
					   ret=false;
				   }
						m_rs->get_Collect((_variant_t)_T("size3"),vT1);
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
			   m_rs->Close();
			   strPipeClampBoltDiameterChanged = PtypeID[i];//����IDֵ
			   rsTZB->put_Collect((_variant_t)_T("CLgg"),STR_VAR(sBH));
	   
		   }
		   else
		   {
			   rsTZB->put_Collect((_variant_t)_T("CLgg"),STR_VAR(sBHFormat));
		   }
	   }
	}
	rsTZB->put_Collect((_variant_t)_T("recno"),COleVariant((long)iSEQofPart));
	dbstr = dbstr + " recno= " + ltos(iSEQofPart);
	rsTZB->put_Collect((_variant_t)_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
	dbstr = dbstr + " VolumeID= " + ltos(EDIBgbl::SelVlmID);
	rsTZB->put_Collect((_variant_t)_T("zdjh"),COleVariant((long)modPHScal::zdjh));
	dbstr = dbstr + " zdjh= " + ltos(modPHScal::zdjh);
	dbstr = dbstr + "\n";
//	AfxMessageBox(dbstr);
	rsTZB->put_Collect((_variant_t)_T("CustomID"),STR_VAR(Ptype[i]));
	rsTZB->put_Collect((_variant_t)_T("ClassID"),COleVariant((long)modPHScal::glClassID));
	//�������д����������У�˼���
	rsTZB->put_Collect((_variant_t)_T("Pjs"),COleVariant((float)tmpSelPJG));
	//�������ͼ��һ·����
	rsTZB->put_Collect((_variant_t)_T("fx"),COleVariant((long)mlfx));
	//���y���귽��
	rsTZB->put_Collect((_variant_t)_T("Rotation"),COleVariant((long)mlRot));
	if( sPartID == _T("LS") || sPartID == _T("LT") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
		rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)-(vtof(vTmp1) + modPHScal::dwB / 2)));
		//���ʵ�ʳߴ�
		rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)-(vtof(vTmp1) + modPHScal::dwB / 2)));
	}
	else if( sPartID == _T("D4LQ") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
		rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)(vtof(vTmp1) + modPHScal::dwB / 2)));
		//���ʵ�ʳߴ�
		rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)(vtof(vTmp1) + modPHScal::dwB / 2)));
	}
	else if( sPartID == _T("DLR") )
	{
		//���ιܵ����߶�ֵҪ��modPHScal::dwB/2
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
		rsX->get_Collect((_variant_t)_T("size4"), &vTmp2);
		rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)(vtof(vTmp1)+ modPHScal::dwB / 2 - vtof(vTmp2))));
		//���ʵ�ʳߴ�
		rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)(long)(vtof(vTmp1)+ modPHScal::dwB / 2 - vtof(vTmp2))));
	}
	else if( sPartID == _T("LM2") || sPartID == _T("LM3") || sPartID == _T("LM4") )
	{
		//ˮƽԲ�ιܵ��������ܣ��߶�ֵҪ�Ӳ۸ֺᵣ�߶�
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		//rsTZB->put_Collect((_variant_t)_T("sizeDIM")) = -(IIf(IsNull(rsX.Fields(_T("size5"))), 0, rsX.Fields(_T("size5"))) + rsX.Fields(_T("size7")) * 10)
		rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
		rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)vtof(vTmp1)));
		//���ʵ�ʳߴ�
		rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)vtoi(vTmp1)));
	}
	else if( sPartID == _T("LN2") || sPartID == _T("LN3") )
	{
		//ˮƽԲ�ιܵ��۸ֺ����������ܣ��߶�ֵҪ��
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
		rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)vtof(vTmp1)));
		//���ʵ�ʳߴ�
		rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)vtoi(vTmp1)));
	}
	else if(sPartID==_T("L15"))
	{
		//����Ժ�̶�֧��Z2/Z2A�µ�һ��ְ�L15,����ֵ������tbnAttachement���ֶ�size2�л�modPHScal::gfPSThickness
		//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
		if(PtypeID[i+1] == _T("GZD"))
		{
			//������֧�գ��ְ���������
			rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)0));
			//���ʵ�ʳߴ�
			rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)0));
		}
		else
		{
			rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)modPHScal::gfPSThickness));
			//���ʵ�ʳߴ�
			rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)modPHScal::gfPSThickness));
		}
	}
	else
	{
		if(modPHScal::glIDIndex==iSA)
		{
			//�����߶���Ϊ0,�Ա���ȷͳ�����˳���
			rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)0));
			//���ʵ�ʳߴ�
			rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)0));
		}
		else
		{
			//�����ע�ߴ磬�Ƚ�sizeDIMҲ��ʱ��Ϊ���ʵ�ʳߴ�
			rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
			rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((long)vtof(vTmp1)));
			//���ʵ�ʳߴ�
			rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((double)vtoi(vTmp1)));
		}
	}

	rsTZB->put_Collect((_variant_t)_T("CLnum"),COleVariant((long)iNumPart));
	rsTZB->put_Collect((_variant_t)_T("SEQ"),COleVariant((long)iSEQofPart));
	rsTZB->put_Collect((_variant_t)_T("CLbz"),vnil);

	_variant_t vTmp;
	if(rsID->Find((_bstr_t)(_T("trim(ID)=\'") + sPartID + _T("\'")), 0, adSearchForward, vTmp))
	{
		rsID->get_Collect((_variant_t)_T("Description"), &vTmp1);
		rsTZB->put_Collect((_variant_t)_T("CLmc"),vTmp1);
	}
	//rsTZB->put_Collect((_variant_t)_T("CLmc")) = sFindFLD(_T("ID"), _T("Description"), sPartID)
	rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
	if( sPartID == _T("LHG") )
		modPHScal::g_fLHGWeight = vtof(vTmp1);
	if( sPartID == _T("T5") )//T5��LHG����ͬʱ���֣���Ϊ���ɺͺ������ɲ���ͬʱ����
		modPHScal::g_fLHGWeight = vtof(vTmp1);
	rsTZB->put_Collect((_variant_t)_T("CLdz"),COleVariant(vtof(vTmp1)));
	rsTZB->get_Collect((_variant_t)_T("CLnum"), &vTmp2);
	rsTZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(vtof(vTmp1)* vtof(vTmp2)));

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
					rsX->get_Collect((_variant_t)_T("sizeC"), &vTmp1);
					modPHScal::DblHangerRodDist =vtof(vTmp1);
					rsTZB->put_Collect((_variant_t)_T("sizeC"),COleVariant(modPHScal::DblHangerRodDist));
				}
				else
				{
					if(modPHScal::glClassID == iPAs )
					{
						//˫֧
						rsX->get_Collect((_variant_t)_T("size3"), &vTmp1);
						modPHScal::DblHangerRodDist = vtof(vTmp1);
						rsTZB->put_Collect((_variant_t)_T("sizeC"),COleVariant(modPHScal::DblHangerRodDist));
					}
					else
					{
						if(modPHScal::glClassID == iPAD11LD ||modPHScal::glClassID == iPADLR || modPHScal::glClassID == iPALX )
						{
							//��ܾ��йص�˫�����
							rsX->get_Collect((_variant_t)_T("sizeC"), &vTmp1);
							modPHScal::DblHangerRodDist = vtof(vTmp1) + modPHScal::dj;
							rsTZB->put_Collect((_variant_t)_T("sizeC"),COleVariant(modPHScal::DblHangerRodDist));
						}
						else
						{
							//����������������ľ࣬��T5,LHG��
							rsX->get_Collect((_variant_t)_T("sizeC"), &vTmp1);
							rsTZB->put_Collect((_variant_t)_T("sizeC"),COleVariant(vtof(vTmp1)));
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
			rsTZB->put_Collect((_variant_t)_T("C"),(vTmp1));
		}
		else if(modPHScal::glClassID==iG56 || modPHScal::glClassID==iG57)
		{
			//G56,57�����ӿ�ȱ�����ԭʼ���ݿ��У���=˫�۸ֵļ��
			vTmp1=rsza->GetCollect(_T("A01"));
			COleVariant vTmp2;
			//������۸ֵļ�϶ֵm,һ��=5
			rsX->get_Collect((_variant_t)_T("m"), &vTmp2);
			vTmp2.dblVal = vTmp2.dblVal * 2 + vTmp1.dblVal;
			rsTZB->put_Collect((_variant_t)_T("C"), vTmp2);
		}
		else
		{
			rsX->get_Collect((_variant_t)_T("CHDIST"), &vTmp1);
			rsTZB->put_Collect((_variant_t)_T("C"),(vTmp1));
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
	rsTZB->put_Collect((_variant_t)_T("Index"),COleVariant((long)modPHScal::glIDIndex));
	rsTmpZB->get_Collect((_variant_t)_T("CustomID"), &vTmp1);

	if( rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")), 0, adSearchForward, vTmp) )
	{
		rsID->get_Collect((_variant_t)_T("ID"), &vTmp1);
		rsTZB->put_Collect((_variant_t)_T("ID"),vTmp1);
	}
	   CString strBH,strID;
	   _RecordsetPtr rsTmp2;
	   switch(modPHScal::glClassID)
	   {
	   case iConnected:
		   //�����˵����Ӽ�
		   if( sPartID.Left(2)== _T("L5") ||sPartID.Left(2)== _T("L6") ||sPartID.Left(2)== _T("L7") ||sPartID.Left(2)== _T("L8"))
		   {
			   rsX->get_Collect((_variant_t)_T("size4"), &vTmp1);
			   rsTZB->put_Collect((_variant_t)_T("AllowedMinLugExt"),(vTmp1));
			   rsX->get_Collect((_variant_t)_T("size5"), &vTmp1);
			   rsTZB->put_Collect((_variant_t)_T("AllowedMaxLugExt"), (vTmp1));
			   //�ڶ��μ���,��¼����ֱ��,�Ա��ҳ����ֵ�����еĵ���/����ɽ���ĸԼ�������ֱ��
			   if( FirstCal == 1 )
			   {
				   rsX->get_Collect((_variant_t)_T("size2"), &vTmp1);
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
   
		   rsTZB->Update();  //?,ÿ��case����������������
		   break;
	   case iROD:
		   //����
		   //�����������˵����Ƴ���
		   //L1�������ƣ�L2��������
		   rsX->get_Collect((_variant_t)_T("size4"), &vTmp1);
		   rsTZB->put_Collect((_variant_t)_T("AllowedMinLugExt"), (vTmp1));
		   //L1�������ƣ�L2��������
		   rsX->get_Collect((_variant_t)_T("size3"), &vTmp1);
		   rsTZB->put_Collect((_variant_t)_T("AllowedMaxLugExt"), (vTmp1));
		   //���ˣ����ֱ��
		   rsX->get_Collect((_variant_t)_T("size2"), &vTmp1);
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
		   rsTZB->put_Collect((_variant_t)_T("sizeC"),COleVariant(modPHScal::giDiameter));
		   //����ֱ�����浽���飬�Ա����ĩβ���Ҹ���ʱʹ�á���Ҫ��������¼λ������֮ǰ�������������������ʼ�˵ĸ����������������
		   //sizeH��Ϊ0���ں���ͳ���ܳ�
		   rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((long)0));
		   //��ʱ����GDW1�ֶα������˵���(kg/��),�����Ժ�������Ϊ����������嵥�ء�
		   rsX->get_Collect((_variant_t)_T("Weight"), &vTmp1);
		   rsTZB->put_Collect((_variant_t)_T("GDW1"),vTmp1);
		   rsTZB->Update();  //?,ÿ��case����������������
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
			   rsTZB->put_Collect((_variant_t)_T("CLcl"),STR_VAR(GetResStr(IDS_GROUPWARE)));
			   j = 1; //���͸�
			   CString sj;
			   sj.Format(_T("%d"),j);
			   rsX->get_Collect((_variant_t)(_T("P")+sj), &vTmp1);
			   CString sTmp3=vtos(vTmp1);
			   sTmp3=sTmp3.Mid(1);
			   sTmp3.Format(_T("%g"),_tcstod(sTmp3,NULL)*10);
			   rsX->get_Collect((_variant_t)(_T("Pnum")+sj), &vTmp2);
			   sTmp=vtos(vTmp1);
			   //Pnum1=NULL��0���������͸���������
			   if( sTmp!=_T("") &&  vtoi(vTmp2) > 0 )
				   //if( sTmp!=_T("") )			
			   {
				   if(modPHScal::glClassID == iSAh )
				   {
					   //ֱ�ӵ�G11
					   rsX->get_Collect((_variant_t)(_T("PL")+sj), &vTmp1);
					   sTmp=vtos(vTmp1);
					   if( sTmp!= _T("") )
					   {
						   rsX->get_Collect((_variant_t)_T("BH"), &vTmp1);
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(CString(_T("UPDATE tmpCSLen SET BH=\'")) + vtos(vTmp1) + _T("\'")), NULL, adCmdText);
						   rsTZB->put_Collect((_variant_t)_T("BH"),vTmp1);
						   rsX->get_Collect((_variant_t)_T("A"), &vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));						
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET a=") +sTmp2), NULL, adCmdText);
						   //G11�Ĺ�ʽΪsizeH+A,����sizeH=�ݸ˳���,���ȴ�����A01�ֶΡ�
						   sTmp2.Format(_T("%g"),modPHScal::WidthA);
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET sizeH=") + sTmp2), NULL, adCmdText);
						   //��ʱֻҪ����������ĳ���
						   sngCSLen = modPHScal::CSLength(sBHFormat, sTmp, sBH);
					   }
					   rsTZB->put_Collect((_variant_t)_T("sizeH"),COleVariant((float)modPHScal::WidthA));
					   rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant((float)modPHScal::WidthA));
				   }
				   else
				   {
					   //��ֱ�ӵ�G11,���۸�
					   rsX->get_Collect((_variant_t)(_T("PL")+sj), &vTmp1);
					   sTmp=vtos(vTmp1);
					   if( sTmp!= _T("") )
					   {
						   rsX->get_Collect((_variant_t)_T("BH"), &vTmp1);
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(CString(_T("UPDATE tmpCSLen SET BH=\'")) + vtos(vTmp1) + _T("\'")), NULL, adCmdText);                           
						   rsTZB->put_Collect((_variant_t)_T("BH"),vTmp1);
						   //G100�Զ�����������͸��ͺ�P1д��tmpCSLen���Ա���Ϊ���Ϲ���.
						   rsX->get_Collect((_variant_t)_T("P1"), &vTmp1);
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(CString(_T("UPDATE tmpCSLen SET P1=\'")) + vtos(vTmp1) + _T("\'")), NULL, adCmdText);                           
				   
						   rsX->get_Collect((_variant_t)_T("A"), &vTmp1);
						   double fLenA=0;//��¼���۶˲�������������֮��ľ���ֵA  Add by luorijin 2008.10.30
						   fLenA = vtof(vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));
						   if(modPHScal::glClassID==iG52_55)
						   {
							   //G52_55�Ĺ�ʽΪH+h+100,����H=���߸߶�,���ȴ�����A01�ֶΡ�
							   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET CHLegHeight=")+sTmp3 ), NULL, adCmdText);
							   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET CHheight=") +sTmp3), NULL, adCmdText);					   
						   }
						   else
						   {
							   //G47��˫�۸ּ��chdist=300,G48��˫�۸ּ��=Bwidth(���ӿ��)
					   
						   }
				   
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET a=") +sTmp2), NULL, adCmdText);
						   rsX->get_Collect((_variant_t)_T("c"), &vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET c=") +sTmp2), NULL, adCmdText);
						   sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("B01"))));
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET B=") +sTmp2), NULL, adCmdText);
						   sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET BWidth=") +sTmp2), NULL, adCmdText);
						   rsTZB->put_Collect((_variant_t)_T("B"),rsza->GetCollect(_T("B01")));
						   rsTZB->put_Collect((_variant_t)_T("A"),rsza->GetCollect(_T("A01")));
				   
						   //H=�������ĸ߶�
						   sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET H=") +sTmp2), NULL, adCmdText);
				   
				   
						   rsX->get_Collect((_variant_t)_T("m"), &vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET m=") +sTmp2), NULL, adCmdText);
						   rsX->get_Collect((_variant_t)_T("CHdist"), &vTmp1);
						   sTmp2.Format(_T("%g"),vtof(vTmp1));
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET CHDist=") +sTmp2), NULL, adCmdText);
						   sTmp2.Format(_T("%g"),modPHScal::Lspan);
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET Lspan=") +sTmp2), NULL, adCmdText);
						   sTmp2.Format(_T("%g"),modPHScal::gdw);
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET GDW1=") +sTmp2), NULL, adCmdText);

						   sTmp2.Format(_T("%g"),modPHScal::Lspan);
						   EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET L1=") +sTmp2), NULL, adCmdText);
				   
						   //�Ѽ�֧�����l1д��ZB ��LSpan�ֶΣ��Է�����άʹ�ã����ұ��ڼ�¼�Ǳ����
						   rsTZB->put_Collect((_variant_t)_T("LSpan"),COleVariant((float)modPHScal::Lspan));
						   rsTZB->put_Collect((_variant_t)_T("XBL1"),COleVariant((float)(modPHScal::gdw+fLenA)));//XBL1��¼���۳���  Add by luorijin 2008.10.30
						   //��ʱֻҪ�������͸ֵĳ���
						   sngCSLen = modPHScal::CSLength(sBHFormat, sTmp, sBH);
					   }
					   //�۸յ�aֵ���浽�ֶ�T,��ֵ�ڻ��Ƹ�����λ�ߴ�ʱ����Ҫ����G22~G24,G3x,G6x,G7x��
					   //if( (sPartID.Left(2) == _T("G3")) || (sPartID.Left(2) ==  _T("G6") ) || (sPartID.Left(2) == _T("G7")) || (sPartID.Left(2) == _T("G5")) && (sPartID.Left(3) != _T("G51"))  && (sPartID.Left(3) != _T("G56"))  && (sPartID.Left(3) != _T("G57")) )
					   if( (modPHScal::glClassID==iSAbeam) || (modPHScal::glClassID==iG52_55 ) )
						   rsTZB->put_Collect((_variant_t)_T("T"),COleVariant((float)(modPHScal::Lspan - modPHScal::gdw)));
					   else
					   {
						   rsX->get_Collect((_variant_t)_T("A"), &vTmp1);
						   rsTZB->put_Collect((_variant_t)_T("T"), (vTmp1));
					   }
			   
					   rsTZB->put_Collect((_variant_t)_T("L1"),COleVariant((float)sngCSLen));
					   rsTZB->put_Collect((_variant_t)_T("GDW1"),COleVariant(modPHScal::gdw));
					   //����ԭʼ���ݱ�TZA�еĸ������۸�����
					   rsX->get_Collect((_variant_t)_T("Pnum1"), &vTmp1);
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
				   rsTZB->put_Collect((_variant_t)_T("CLgg"),STR_VAR(sBH));
				   //rsTZB->put_Collect((_variant_t)_T("BH"),STR_VAR(sBH));
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
					  rsX->get_Collect((_variant_t)_T("BH"), &vTmp1);
					  sTmp2.Format(GetResStr(IDS_SAP1MustBeChannelSteelNo),modPHScal::tbnSA,vtos(vTmp1));
					  //frmStatus.MessageBox(sTmp2);
					  ShowMessage(sTmp2);
				  }
			  }
		  }
  
		  rsTZB->Update();
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
			  rsX->get_Collect((_variant_t)(_T("P")+sj), &vTmp1);
			  rsX->get_Collect((_variant_t)(_T("Pnum")+sj), &vTmp2);
			  sTmp=vtos(vTmp1);
			  if( sTmp!=_T("") && vtoi(vTmp2)>0 )
			  {
				  mvSAattachedCustomID =sTmp;
				  //���Ƿ����͸�:�۸֡��Ǹ֡����ָ֡�Բ�֡���֣����ְ���Ϊ��������
				  //���(�ڸ�������͸ֱ���)�Ĵ�Ÿ�ʽӦΪ��-��x����-90x16��
				  SQL1 = _T("SELECT * FROM [") + modPHScal::tbnSectionSteel + _T("] WHERE trim(BH)=\'") + mvSAattachedCustomID + _T("\' ORDER BY bh");
				  if(rsTmp->State == adOpenStatic)
					  rsTmp->Close();
	try
	{
//				  rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
//				  rsTmp.Open(dbOpenSnapshot,SQL1);
				  rsTmp->Open((_bstr_t)SQL1,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					  adOpenForwardOnly, adLockReadOnly, adCmdText); 
			  }
			  catch(_com_error e)
			  {
				  ret=false;
			  }
				  if( rsTmp->adoEOF && rsTmp->BOF )
				  {
					  //�����͸�
					  //****************************
					  //�Ҳ���,���Ǹ���,�ٲ鸽����
					  rsTmp->Close();
					  SQL1 = _T("SELECT * FROM [") + modPHScal::tbnAttachment + _T("] WHERE trim(BH)=\'") + mvSAattachedCustomID + _T("\' ORDER BY bh");
					  /*if(rsTmp->State == adOpenStatic)
					  rsTmp->Close();
	try
	{
					  rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;*/
//					  rsTmp.Open(dbOpenSnapshot,SQL1);
	try
	{
					  rsTmp->Open((_bstr_t)SQL1,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
						  adOpenForwardOnly, adLockReadOnly, adCmdText); 
				  }
				  catch(_com_error e)
				  {
					  ret=false;
				  }
					  if( rsTmp->adoEOF && rsTmp->BOF )
					  {
						  //���Ǹ���,������˨��ĸ,�ٲ���˨��ĸ��
						  //��˨��ĸ������ʹ�õĿ������ض���׼����˨��ĸ�����������ʹ��������׼����˨��ĸ������ұ�׼��˨��ĸ��
						  //���ŷ�ʽΪM36x200,��M20
						  //��ˣ����Ȳ��Ҹ�����˨��ĸ��tbnSABoltsnuts���ٸ��ݲ���Ĺ���ͨ����˨��ĸ��tbnBoltsNutsID�в�����Ӧ�Ĺ��
						  rsTmp->Close();
						  SQL1 = _T("SELECT * FROM [") + modPHScal::tbnSABoltsNuts + _T("] WHERE trim(BH)=\'") + mvSAattachedCustomID + _T("\' ORDER BY bh");
						  /*if(rsTmp->State == adOpenStatic)
						  rsTmp->Close();
						  rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;*/
//						  rsTmp.Open(dbOpenSnapshot,SQL1);
	try
	{
						  rsTmp->Open((_bstr_t)SQL1,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
							  adOpenForwardOnly, adLockReadOnly, adCmdText); 
					  }
					  catch(_com_error e)
					  {
						  ret=false;
					  }
						  if( rsTmp->adoEOF && rsTmp->BOF )
						  {
							  //û�ҵ���˨��ĸ��¼
							  sBH = _T("");
							  strBH = _T("");
							  sngWeight = 0;
							  mviSASSClassID = iSABoltsNutsUnknown;
							  //rsTmp->Close();
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
							  rsTmp->MoveFirst();
							  rsTmp->get_Collect((_variant_t)_T("BH"), &vTmp1);
							  sBH = vtos(vTmp1);
							  strBH=sBH;
							  sTmp=CString(_T("UPDATE tmpCSLen SET BH=\'")) + sBH + _T("\'");
							  EDIBgbl::dbPRJ->Execute((_bstr_t)(sTmp), NULL, adCmdText);
							  rsTmp->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
							  tmpCustomID = vtos(vTmp1);
							  sTmp= CString(_T("UPDATE tmpCSLen SET CustomID=\'")) + tmpCustomID + _T("\'");
							  EDIBgbl::dbPRJ->Execute((_bstr_t)(sTmp), NULL, adCmdText);
					  
							  rsTmp->get_Collect((_variant_t)_T("size2"), &vTmp1);
							  tmpSize2 =vtof(vTmp1);
							  rsTmp->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
							  tmpSizeH =vtof(vTmp1);
							  tmpID=modPHScal::sFindID(tmpCustomID);
							  if(tmpID==_T("F9") || tmpID==_T("F10"))
							  {
								  if(modPHScal::glClassID==iG56 || modPHScal::glClassID==iG57 || modPHScal::glClassID==iG52_55 || modPHScal::glClassID==iG51)
								  {
									  rsX->get_Collect((_variant_t)(_T("PL")+sj), &vTmp1);
									  sTmp=vtos(vTmp1);
									  if(rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + tmpCustomID + _T("\'")), 0, adSearchForward, vTmp))
									  {
										  rsID->get_Collect((_variant_t)_T("BHFormat"), &vTmp1);
										  sBHFormat = vtos(vTmp1);
									  }
									  else
									  {
									  }
									  sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
									  EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET BWidth=") +sTmp2), NULL, adCmdText);								
									  sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
									  EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET H=") +sTmp2), NULL, adCmdText);
									  sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("B01"))));
									  EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET B=") +sTmp2), NULL, adCmdText);								
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
								  rsTmp->Close();
	try
	{
	//							  rsTmp.Open(dbOpenSnapshot,SQL1);
								rsTmp->Open((_bstr_t)SQL1,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
								  adOpenForwardOnly, adLockReadOnly, adCmdText); 
						  }
						  catch(_com_error e)
						  {
							  ret=false;
						  }
								  if( rsTmp->adoEOF && rsTmp->BOF )
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
									  rsTmp->MoveFirst();
									  if(tmpID==_T("F9") || tmpID==_T("F10"))
									  {
										  //F9��F10�Ѿ������ֱ���ͳ���,ֻҪ��ͨ����˨��ĸ���в�����ǵ�CustomID��BH,������CLgg���Ϳ����ˡ�
										  rsTmp->get_Collect((_variant_t)_T("BH"), &vTmp1);
										  sBH = vtos(vTmp1);
										  sTmp= CString(_T("UPDATE tmpCSLen SET BH=\'")) + sBH + _T("\'");
										  EDIBgbl::dbPRJ->Execute((_bstr_t)(sTmp), NULL, adCmdText);
								  
										  rsID->get_Collect((_variant_t)_T("BHFormat"), &vTmp1);
										  sBHFormat = vtos(vTmp1);															    
										  sngCSLen = modPHScal::CSLength(sBHFormat, ftos(sngCSLen), sBH);
									  }
									  else if(tmpID==_T("F14"))
									  {
										  rsTmp->get_Collect((_variant_t)_T("BH"), &vTmp1);
										  sBH = vtos(vTmp1);
										  sngCSLen = 1000;  //����Ϊ1000,���ڱ��.
									  }						
									  else
									  {
										  rsTmp->get_Collect((_variant_t)_T("BH"), &vTmp1);
										  sBH = vtos(vTmp1);
										  sngCSLen = 1000;  //����Ϊ1000,���ڱ��.
									  }
									  rsTmp->get_Collect((_variant_t)_T("Weight"), &vTmp1);
									  sngWeight =vtof(vTmp1)*sngCSLen/1000;
									  sngW = sngWeight;
									  rsTmp->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
									  tmpCustomID = vtos(vTmp1);                    
									  if(rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + tmpCustomID + _T("\'")), 0, adSearchForward, vTmp))
									  {
										  rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp1);
										  mviSASSClassID =vtoi(vTmp1);
										  rsID->get_Collect((_variant_t)_T("Index"), &vTmp1);
										  mviSASSIndex =vtoi(vTmp1);
									  }
								  }
						  }
					   }
					   else
					   {
						   //�鸽�����ҵ�����,������˨��ĸ,
						   strBH=_T("");
						   rsTmp->MoveFirst();
						   rsTmp->get_Collect((_variant_t)_T("BH"), &vTmp1);
						   sBH = vtos(vTmp1);
						   rsTmp->get_Collect((_variant_t)_T("Weight"), &vTmp1);
						   sngWeight =vtof(vTmp1);
						   sngW = sngWeight;
						   rsTmp->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
						   tmpCustomID = vtos(vTmp1);
						   tmpID=modPHScal::sFindID(tmpCustomID);
						   if(rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + tmpCustomID + _T("\'")), 0, adSearchForward, vTmp))
						   {
							   rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp1);
							   mviSASSClassID =vtoi(vTmp1);
							   rsID->get_Collect((_variant_t)_T("Index"), &vTmp1);
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
							if( rsTmp->adoEOF && rsTmp->BOF )
							{
								//û�������͸ֹ��
								sTmp.Format(GetResStr(IDS_NothisSectionSteelInZDJcrudeMdb),modPHScal::dbZDJcrude->DefaultDatabase,mvSAattachedCustomID);
								throw sTmp;
							}
							else
							{
								//�������͸ֹ��
								rsTmp->MoveFirst();
								rsTmp->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
								tmpCustomID =vtos(vTmp1);
								rsTmp->get_Collect((_variant_t)_T("Material"), &vTmp1);
								mvsSASSMaterial = vtos(vTmp1);
						
								if( rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + tmpCustomID + _T("\'")), 0, adSearchForward, vTmp))
								{
									rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp1);
									mviSASSClassID = vtoi(vTmp1);
									rsID->get_Collect((_variant_t)_T("Index"), &vTmp1);
									mviSASSIndex =vtoi(vTmp1);
									//��ʱ����������͸ָ߶ȣ������Ժ�ȷ�������������ӵ���˨�ĸ߶ȡ�
									if( j == 1 )
									{
										rsTmp->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
										SACSHeight =vtof(vTmp1);
									}
								}
							}
							rsX->get_Collect((_variant_t)(_T("PL")+sj), &vTmp1);
							sTmp=vtos(vTmp1);
							if(sTmp!=_T("") )
							{
								rsTmp->get_Collect((_variant_t)_T("BH"), &vTmp1);
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET BH=\'") + vtos(vTmp1) + _T("\'")), NULL, adCmdText);
								vBH = vTmp1;			
								rsTmp->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET CustomID=\'")+vtos(vTmp1)+_T("\'")), NULL, adCmdText);
								rsX->get_Collect((_variant_t)_T("A"), &vTmp1);
								sTmp2.Format(_T("%g"),vtof(vTmp1));
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET a=") +sTmp2), NULL, adCmdText);
								rsX->get_Collect((_variant_t)_T("c"), &vTmp1);
								sTmp2.Format(_T("%g"),vtof(vTmp1));
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET c=") +sTmp2), NULL, adCmdText);
								if( modPHScal::glClassID==iG52_55 || modPHScal::glClassID==iG51)
								{
									sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
									EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET H=") +sTmp2), NULL, adCmdText);
								}
								else
								{
									sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("A01"))));
									EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET BWidth=") +sTmp2), NULL, adCmdText);
								}
								sTmp2.Format(_T("%g"),vtof(rsza->GetCollect(_T("B01"))));
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET B=") +sTmp2), NULL, adCmdText);
								rsX->get_Collect((_variant_t)_T("m"), &vTmp1);
								sTmp2.Format(_T("%g"),vtof(vTmp1));
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET m=") +sTmp2), NULL, adCmdText);
								rsX->get_Collect((_variant_t)_T("CHdist"), &vTmp1);
								sTmp2.Format(_T("%g"),vtof(vTmp1));
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET CHDist=") +sTmp2), NULL, adCmdText);

								rsTmp->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
								if (vtos(vTmp1)==_T(""))
								{
									sTmp2 = _T("UPDATE tmpCSLen SET CHHeight=0");
								} else {
									sTmp2 = _T("UPDATE tmpCSLen SET CHHeight=")+vtos(vTmp1);
								}
								EDIBgbl::dbPRJ->Execute((_bstr_t)sTmp2, NULL, adCmdText);

								if (vtos(vTmp1)==_T(""))
								{
									sTmp2 = _T("UPDATE tmpCSLen SET CHLegHeight=0");
								} else {
									sTmp2 = _T("UPDATE tmpCSLen SET CHLegHeight=")+vtos(vTmp1);
								}
								EDIBgbl::dbPRJ->Execute((_bstr_t)sTmp2, NULL, adCmdText);
								sTmp2.Format(_T("%g"),modPHScal::gdw);
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET GDW1=") +sTmp2), NULL, adCmdText);
								sTmp2.Format(_T("%g"),modPHScal::Lspan);
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET Lspan=") +sTmp2), NULL, adCmdText);
								sTmp2.Format(_T("%g"),modPHScal::Lspan);
								EDIBgbl::dbPRJ->Execute((_bstr_t)(_T("UPDATE tmpCSLen SET [L1]= ") +sTmp2), NULL, adCmdText);
								sngCSLen = modPHScal::CSLength(sBHFormat, sTmp, sBH);
							}
							//sBH�к�����Чֵ,�����͸ֲ��罫����Ϊ����������
							sBH = mvSAattachedCustomID;
							//�͸�����=ÿ������x����(mm)/1000
							rsTmp->get_Collect((_variant_t)_T("Weight"), &vTmp1);
							sngWeight =vtof(vTmp1);
							sngW = sngWeight * sngCSLen / 1000;
						}
					}
			
					rsX->get_Collect((_variant_t)(_T("Pnum")+sj), &vTmp1);
					//������������=����ÿ������������֮��kg
					mvarSATotalWeight = mvarSATotalWeight + sngW * vtof(vTmp1);
					rsSAPart->AddNew();
			
					rsSAPart->put_Collect((_variant_t)_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
					rsSAPart->put_Collect((_variant_t)_T("zdjh"),COleVariant((long)modPHScal::zdjh));
					//2002.05.26�Ժ��д�˶δ��룬����phs.arx˼·һ�¡�
					//�Ǹ�������,��д��¼��,
					//�Ա����ָ��������벿��.Ҳ���ڶ������š�
					iSEQofPart++;
					rsSAPart->put_Collect((_variant_t)_T("recno"),COleVariant((long)iSEQofPart));
					//��Ϊ������������������ֱ�־��IsSAPart�ֶ��Ƿ�=-1��
					rsSAPart->put_Collect((_variant_t)_T("IsSAPart"),COleVariant((long)-1));
					//rsSAPart->put_Collect((_variant_t)_T("Index")) = iSA
					rsSAPart->put_Collect((_variant_t)_T("nth"),COleVariant((short)nth));//��׼֧����·��=1�������Գ�˫���͹��ø���˫��
					rsSAPart->put_Collect((_variant_t)_T("ClassID"),COleVariant((long)mviSASSClassID));
					rsSAPart->put_Collect((_variant_t)_T("Index"),COleVariant((long)mviSASSIndex));
					rsTmp->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
					rsSAPart->put_Collect((_variant_t)_T("CustomID"),STR_VAR(vtos(vTmp1)));
					//sBH				
					rsSAPart->put_Collect((_variant_t)_T("CLgg"),STR_VAR(sBH));
					if(tmpID=="F9" || tmpID=="F10" || tmpID=="F14")
					{				
						rsSAPart->put_Collect((_variant_t)_T("BH"),STR_VAR(strBH));
						//������˨���ݸ˵ĳ��ȣ�
						rsSAPart->put_Collect((_variant_t)_T("L1"),COleVariant( sngCSLen));
					}
					else
					{
				
						if(mviSASSIndex==iSectionSteel )
						{
							//�����͸ֵĳ��ȣ�
							rsSAPart->put_Collect((_variant_t)_T("L1"),COleVariant( sngCSLen));
							//rsSAPart->put_Collect((_variant_t)_T("GDW1"),COleVariant(modPHScal::gdw));
							rsSAPart->put_Collect((_variant_t)_T("BH"),STR_VAR(sBH));
						}
						else
							rsSAPart->put_Collect((_variant_t)_T("BH"),STR_VAR(sBH));
					}
			
					rsTmp->get_Collect((_variant_t)_T("Material"), &vTmp1);
			
					rsSAPart->put_Collect((_variant_t)_T("CLcl"),STR_VAR(vtos(vTmp1)));
					rsX->get_Collect((_variant_t)(_T("Pnum")+sj), &vTmp1);
					rsSAPart->put_Collect((_variant_t)_T("CLnum"),vTmp1);
					rsSAPart->put_Collect((_variant_t)_T("CLbz"),vnil);
					rsSAPart->put_Collect((_variant_t)_T("CLdz"),COleVariant(sngWeight));
					rsX->get_Collect((_variant_t)(_T("Pnum")+sj), &vTmp1);
					rsSAPart->put_Collect((_variant_t)_T("CLzz"),COleVariant(sngW * vtof(vTmp1)));
					rsID->get_Collect((_variant_t)_T("Description"), &vTmp1);
					rsSAPart->put_Collect((_variant_t)_T("CLmc"),vTmp1);
					rsSAPart->Update();
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
				   rsTZB->put_Collect((_variant_t)_T("CLcl"),STR_VAR(GetResStr(IDS_GROUPWARE)));
				   //Height_SPRINGsL5 = IIf(IsNull(rsX(_T("size3"))), 0, rsX(_T("size3")))
				   if(sPartID==_T("T4"))
				   {
					   rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
					   sngDim = vtof(vTmp1) - /*Sgn(modPHScal::yr) */ modPHScal::sSprInfo[iSEQofSPR].haz;//z.jia add
					   //rsTZB->put_Collect((_variant_t)_T("Size")) = Height_SPRINGsL5 / 2
				   }
				   else if(sPartID==_T("T3") || sPartID==_T("T3A"))
				   {
					   rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
					   sngDim = fabs(vtof(vTmp1)) - modPHScal::sSprInfo[iSEQofSPR].haz;
					   //��/�·����ɣ���ֵ�ڼ�������ʱӦ��ʹ���˼ӳ�
					   sngDim = -sngDim;
				   }
				   //rsTZB->put_Collect((_variant_t)_T("Size")) = Height_SPRINGsL5 / 2
				   else
				   {
					   rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
					   sngDim = vtof(vTmp1) + modPHScal::sSprInfo[iSEQofSPR].haz;
				   }
				   //���ɵĳߴ綼�ǰ��ɽ���ĸ��Ե���ġ��Ұ���Сλ��ʱ�����߶ȡ�
				   //���ײ�ͬ���ɽ���ĸʱӦ�û��㲻ͬ�ĸ߶ȡ����ִ�������ݲ��ṩ��
				   //���ɵ��ɽ���ĸ��CalLugLength�в��
				   rsTZB->put_Collect((_variant_t)_T("SizeH"),COleVariant(sngDim));
				   rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant(sngDim));
				   //rsTZB->put_Collect((_variant_t)_T("CLgg")) = sBHformat
				   if( EDIBAcad::g_bBomSprFactory )//newHS->ֻ��ѡ���˱�ע���ɳ��Ҳ�д�����ݿ�
				   {
					   rsTZB->put_Collect((_variant_t)_T("CLbz"),STR_VAR(modPHScal::gsSPRmanufactory));
				   }
				   rsTZB->put_Collect((_variant_t)_T("SpringNo"), COleVariant( (long)modPHScal::sSprInfo[iSEQofSPR].DH) );
				   rsTZB->put_Collect((_variant_t)_T("LugDia"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].FiJ));
				   rsTZB->put_Collect((_variant_t)_T("HeatDisp"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].HeatDisp));
				   rsTZB->put_Collect((_variant_t)_T("HAZ"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].haz));
		   
				   rsTZB->Update();
			   }
			   break;
		   case iCSPR:
			   {
				   rsTZB->put_Collect((_variant_t)_T("CLcl"),STR_VAR(GetResStr(IDS_GROUPWARE)));
				   //��¼ԭʼ�ߴ�T�����ڹ���GetBoltsNutsAndAttachmentsCLgg׼ȷ������������ӵ���˨����
				   rsX->get_Collect((_variant_t)_T("T"), &vTmp1);
				   rsTZB->put_Collect((_variant_t)_T("T"),COleVariant((long)vtoi(vTmp1)));
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
		   
				   rsDiaOfCSPRFiJ->get_Collect((_variant_t)_T("FiJ"), &vTmp1);
				   //SQLx.Format(_T("SELECT * FROM [%s] WHERE size2=%g"),modPHScal::tbnCSPRINGL5Crude,vtof(vTmp1));
				   if(rsTmp->State == adOpenStatic)
					   rsTmp->Close();
	try
	{
	//			   rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;
	//			   rsTmp.Open(dbOpenSnapshot,SQLx);
				   rsTmp->Open((_bstr_t)SQLx,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					   adOpenForwardOnly, adLockReadOnly, adCmdText); 
			   }
			   catch(_com_error e)
			   {
				   ret=false;
			   }
				   if( rsTmp->adoEOF && rsTmp->BOF )
				   {
					   //û���ҵ�ֱ��ֵ
					   sTmp.Format(GetResStr(IDS_NotMatchDiameterValueInZdjcrudeMdb),modPHScal::dbZDJcrude->DefaultDatabase,modPHScal::tbnCSPRINGL5Crude,SQLx);
					   throw sTmp;
				   }
				   else
				   {
					   rsTmp->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
					   Height_SPRINGsL5 =vtof(vTmp1);
					   //��ȡ���˿ɲ������󳤶Ⱥ���С����
					   rsTmp->get_Collect((_variant_t)_T("size4"), &vTmp1);
					   rsTZB->put_Collect((_variant_t)_T("AllowedMinLugExt"),COleVariant(vtof(vTmp1)));//���浽���ݿ⣬�Ա�CalLugLength����ʹ��
					   rsTmp->get_Collect((_variant_t)_T("size5"), &vTmp1);
					   rsTZB->put_Collect((_variant_t)_T("AllowedMaxLugExt"),COleVariant(vtof(vTmp1)));
				   }
				   //���������Ϸ��������˵��¶˲���ֵ�����浽ZB���AllowedMaxLugExtUp��AllowedMinLugExtUp�ֶ���
				   if( sPartID == _T("LHA") || sPartID == _T("LHB") || sPartID == _T("PHA")|| sPartID == _T("PHB"))
				   {
					   rsX->get_Collect((_variant_t)_T("PP"), &vTmp1);//pfg2005.12.23 �϶���С����ֵӦ��ΪPP
					   rsTZB->put_Collect((_variant_t)_T("AllowedMaxLugExtUp"),COleVariant(vtof(vTmp1)));
					   rsTZB->put_Collect((_variant_t)_T("AllowedMinLugExtUp"),COleVariant(vtof(vTmp1)));
				   }
		   
				   //����������ɵ�ѡ��λ��modPHScal::sSprInfo[iSEQofSPR].SumDisp���õ�������λ���ֶ�fBmin��һ��ֵ��
				   //��������ɵİ�װ�߶�H1(sngH1xmax)=ֱ�Ӳ���������λ���ֶ�fBmin��ֵ��
				   //���򣬰���ֵ������λ�ơ�add by ligb on 2004.11.24
				   CString tmpSQL;
				   _RecordsetPtr rsX1;
				   rsX1.CreateInstance(__uuidof(Recordset));
		   
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
				   tmpSQL += (modPHScal::gbCSPRneedSpecialDesign ? _T(" ") : sTmp);
				   tmpSQL += _T(" AND trim(CustomID)=\'");
				   tmpSQL += Ptype[i];
				   tmpSQL += _T("\' ORDER BY dh,Weight");
	try
	{
	//			   rsX1.m_pDatabase=&modPHScal::dbZDJcrude;
	//			   rsX1.Open(dbOpenSnapshot,tmpSQL);
				   rsX1->Open((_bstr_t)tmpSQL,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
					   adOpenForwardOnly, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}
				   if(rsX1->BOF && rsX1->adoEOF||1)//�������ٽ�ֵʱ����(sizeH�ĸ߶�û�м���ʵ��λ��)pfg and lgb 2005.12.15
				   {
					   //���ݵ�����е���������ݿ��ֶ�sizeH�洢����ȷ�ĸ߶�ֵ��
					   rsX->get_Collect((_variant_t)_T("sizeH"), &vTmp1);
					   //rsX->get_Collect((_variant_t)_T("E"),vTmp1);
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
						   rsX->get_Collect((_variant_t)_T("fBmin"), &vTmp1);
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
						   rsX->get_Collect((_variant_t)_T("fBmax"), &vTmp1);
						   rsX->get_Collect((_variant_t)_T("fBmin"), &vTmp2);
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
					   //rsTZB->put_Collect((_variant_t)_T("AllowedMinLugExt")) = IIf(IsNull(rsX.Fields(_T("x"))), 0, rsX.Fields(_T("x")))
					   //rsTZB->put_Collect((_variant_t)_T("AllowedMaxLugExt")) = IIf(IsNull(rsX.Fields(_T("y"))), 0, rsX.Fields(_T("y")))+rsTZB->put_Collect((_variant_t)_T("AllowedMinLugExt"))
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
						   rsTmp->get_Collect((_variant_t)_T("size4"), &vTmp1);
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
						   //rsTZB->put_Collect((_variant_t)_T("A")) = IIf(IsNull(rsX(_T("PP"))), 0, rsX(_T("PP")))
					   }
					   else if(sPartID==_T("LHE") || sPartID==_T("LHE1") || sPartID==_T("PHE") || sPartID==_T("PHE1") || sPartID==_T("PHC") || sPartID==_T("LHC"))
					   {
						   //��ʽ�������ӣ�Ҫ���϶���߶�T��
						   if( !modPHScal::gbCH1IncludeT )
						   {
							   rsDiaOfCSPRFiK->get_Collect((_variant_t)_T("T"), &vTmp1);
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

				   rsX1->Close();

				   //����ɽ���ĸ���ȵ���ֵ
				   rsTZB->put_Collect((_variant_t)_T("SizeH"),COleVariant(sngDim));
				   rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant(sngDim));
				   if( EDIBAcad::g_bBomSprFactory )//newHS->ֻ��ѡ���˱�ע�������ɳ��Ҳ�д�����ݿ�
				   {
					   rsTZB->put_Collect((_variant_t)_T("CLbz"),STR_VAR(modPHScal::gsCSPRmanufactory));
				   }
				   //��ʱ����gdw1�ֶα��������أ�����������GetBoltsNutsAndAttachmentsCLggʹ��
				   rsTZB->put_Collect((_variant_t)_T("GDW1"),COleVariant(tmpSelPJG4CSPRFiK / iNumPart));
				   rsTZB->put_Collect((_variant_t)_T("SpringNo"), COleVariant( (long)modPHScal::sSprInfo[iSEQofSPR].DH) );
				   rsTZB->put_Collect((_variant_t)_T("LugDia"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].FiJ));
				   rsTZB->put_Collect((_variant_t)_T("HeatDisp"), COleVariant(modPHScal::sSprInfo[iSEQofSPR].HeatDisp));
		   
				   rsTZB->Update();
			  }
			  break;
		default:
			//�������㲿������
			rsTZB->Update();
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
				rsConnect->get_Collect((_variant_t)(_T("CntbF")+sj), &vTmp1);
				if(vtos(vTmp1)!=_T("") )
				{
			
			
					if( rsPartBoltNuts->adoEOF )
						continue;
	//				rsPartBoltNuts.Edit();
					iSEQofPart = iSEQofPart + 1;
					rsPartBoltNuts->put_Collect((_variant_t)_T("BHformat"),STR_VAR(sBHFormat));
					rsPartBoltNuts->put_Collect((_variant_t)_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
					rsPartBoltNuts->put_Collect((_variant_t)_T("zdjh"),COleVariant((long)modPHScal::zdjh));
					rsPartBoltNuts->put_Collect((_variant_t)_T("recno"),COleVariant((long)iSEQofPart));

					//rsPartBoltNuts->put_Collect((_variant_t)_T("index"),COleVariant((long)iBoltsNuts));
					rsPartBoltNuts->put_Collect((_variant_t)_T("nth"),COleVariant((short)nth));//��׼֧����·��=1�������Գ�˫���͹��ø���˫��
					rsConnect->get_Collect((_variant_t)(_T("CntbF")+sj+_T("num")), &vTmp1);

					if( vTmp1.vt==VT_NULL)
					{
						//��˨��ĸ����=����������*1///
						rsPartBoltNuts->put_Collect((_variant_t)_T("CLnum"),COleVariant(iNumPart * iNum * iCntbNum));
					}
					else
					{
						//��������=ʵ������,����vtmp1ֵ Ϊ������
						rsPartBoltNuts->put_Collect((_variant_t)_T("CLnum"),COleVariant(vtoi(vTmp1) * iNumPart * iNum * iCntbNum));
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
					rsConnect->get_Collect((_variant_t)(_T("CntbF")+sj), &vTmp1);
					rsPartBoltNuts->put_Collect((_variant_t)_T("ID"),vTmp1);
			
					if(rsID->Find((_bstr_t)(_T("trim(ID)=\'") + vtos(vTmp1) + _T("\'")), 0, adSearchForward, vTmp))
					{
						rsID->get_Collect((_variant_t)_T("Description"), &vTmp1);
						rsPartBoltNuts->put_Collect((_variant_t)_T("CLmc"),vTmp1);
						rsID->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
						rsPartBoltNuts->put_Collect((_variant_t)_T("CustomID"),vTmp1);
						rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp1);
						rsPartBoltNuts->put_Collect((_variant_t)_T("ClassID"),vTmp1);
					}
					//rsPartBoltNuts->put_Collect((_variant_t)_T("CustomID")) = IIf(gbFDid, sFindCustomID(rsc.Fields(_T("CntbF") + j)), rsc.Fields(_T("CntbF") + j))
					//rsPartBoltNuts->put_Collect((_variant_t)_T("ID")) = IIf(gbFDid, rsc.Fields(_T("CntbF") + j), sFindID(rsc.Fields(_T("CntbF") + j)))
					//rsPartBoltNuts->put_Collect((_variant_t)_T("CLmc")) = sFindFLD(IIf(gbFDid, _T("ID"), _T("CustomID")), _T("Description"), IIf(gbFDid, rsc.Fields(_T("CntbF") + j), sFindCustomID(rsc.Fields(_T("CntbF") + j))))
					if( ! bWithBoltsNuts )
					{
						//���Ҳ��ṩ����˨��ĸ
						//��������ǿյ����֣����ڱ��
						rsPartBoltNuts->put_Collect((_variant_t)_T("SEQ"),COleVariant((long) i));

						//lfx 2005.5.20�� 
						//˵��������//lfx 2005.5.20�� ע��
						//ԭrsPartBoltNuts->put_Collect((_variant_t)_T("CLbz"),vnil);
						rsPartBoltNuts->put_Collect((_variant_t)_T("CLbz"),COleVariant(strClbz));  
						//End 2005.5.20��
					}
					else
					{
						rsPartBoltNuts->put_Collect((_variant_t)_T("SEQ"),vnil);
						rsPartBoltNuts->put_Collect((_variant_t)_T("CLnum"),COleVariant((long)0));
						CString str;
						str.LoadString(IDS_PRODUCEFAC_OFFERGOODS);
						rsPartBoltNuts->put_Collect((_variant_t)_T("CLbz"),STR_VAR(str));
					}
					rsPartBoltNuts->Update();
					rsPartBoltNuts->MoveNext();
					rsTZB->MoveNext();
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
		rsTmpZB->MoveNext();
		rsTZB->MoveNext();
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
			
				//if(rsTZB->Find((_bstr_t)_T("IsNull(recno) AND (bUserAdd is null OR bUserAdd=0)")))
				//2002.05.25�Ժ����������д��¼�ţ��ʸ�д�˶γ���LEE Guobin
				if(rsTZB->Find((_bstr_t)(_T(" (bUserAdd is null OR bUserAdd=0) AND Index=") + ltos(iSA)), 0, adSearchForward, vTmp))
				{	
			//		rsTZB.Edit();
					if( rsTZB->BOF )			
					{
						sTmp.Format(GetResStr(IDS_BOForEOFofmvarRsTZB),  _T("IsBOF()"));
						throw sTmp;
					}
					if( rsTZB->adoEOF )			
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
				rsTZB->get_Collect((_variant_t)_T("CustomID"), &vTmp1);
			
				if( !rsID->Find((_bstr_t)(_T("trim(CustomID)=\'") + vtos(vTmp1) + _T("\'")), 0, adSearchForward, vTmp)){
					sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),  _T("CustomID"), vtos(vTmp1));
					throw sTmp;
				}
				else
				{
			//		rsTZB.Edit ();
					rsID->get_Collect((_variant_t)_T("ClassID"), &vTmp1);
					if(  vtoi(vTmp1)== iGCement )
					{
						rsTZB->put_Collect((_variant_t)_T("SEQ"),vnil);
						rsTZB->put_Collect((_variant_t)_T("CLdz"),COleVariant((long)0));
						rsTZB->put_Collect((_variant_t)_T("CLzz"),COleVariant((long)0));
						mvarSATotalWeight = 0;
					}
					else
					{
						//�͸ָ���,����һ�ǿյ�����ֵ
						rsTZB->put_Collect((_variant_t)_T("sizeDIM"),COleVariant(SACSHeight));
						rsTZB->get_Collect((_variant_t)_T("index"), &vTmp1);
						vTmp2=rsza->GetCollect(_T("bNotSumSA"));
						if( vtoi(vTmp1)== iSA && !vtob(vTmp2)){
							//��������Ҫͳ��
							rsTZB->put_Collect((_variant_t)_T("SEQ"),COleVariant((long)i));
							rsTZB->put_Collect((_variant_t)_T("CLdz"),COleVariant(mvarSATotalWeight));
							rsTZB->get_Collect((_variant_t)_T("CLNum"), &vTmp1);
							rsTZB->put_Collect((_variant_t)_T("CLzz"),COleVariant(mvarSATotalWeight * vtof(vTmp1)));
						}
						else
						{
							//�������ϲ�Ҫͳ��
							//������������ţ���ֹ��д�������ϸ��
							rsTZB->put_Collect((_variant_t)_T("SEQ"),vnil);
							rsTZB->put_Collect((_variant_t)_T("CLNum"),COleVariant((long)0));
							rsTZB->put_Collect((_variant_t)_T("CLdz"),COleVariant((long)0));
							rsTZB->put_Collect((_variant_t)_T("CLzz"),COleVariant((long)0));
						}
					}
				}
				rsTZB->Update();		
			}		
		}
		if( modPHScal::g_bPipeClampBoltDiameterChanged && FirstCal ==1 && modPHScal::giWholeSprNum ==0 )
			//С���ش�ܾ��ܼа�����ѡ����˨��ֱ��M1��С���ܼй���б���ע����M1ֵ
		{
			CString strSQLTmp ;
			//�ܼ���˨M1��U�ζ���L8����˨M��������
			strSQLTmp.Format("SELECT * FROM [%s] WHERE [CUSTOMID]=\'%s\' AND [SIZE2]=%d ORDER BY SIZE2",
				modPHScal::tbnPART,modPHScal::sFindCustomID (_T("L8")),int(modPHScal::gmiDiameter));
			_RecordsetPtr m_rs;
			COleVariant vT1;
			if(m_rs->State == adOpenStatic)
				m_rs->Close();
			
	try
	{
	//		m_rs.m_pDatabase=&modPHScal::dbZDJcrude;
	//		m_rs.Open(dbOpenDynaset,strSQLTmp);
			m_rs->Open((_bstr_t)strSQLTmp,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
				adOpenForwardOnly, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}
			CString str;
			int x=0;
			if((!m_rs->BOF) && !(m_rs->adoEOF))
			{
				m_rs->get_Collect((_variant_t)_T("size3"),vT1);
				x = vtoi(vT1);
			}
			m_rs->Close();
			
			strSQLTmp.Format(" [NTH]=1 AND recno=1 ");
			rsTZB->MoveFirst();
			rsTZB->Find((_bstr_t)strSQLTmp, 0, adSearchForward, vTmp);
			if(!rsTZB->adoEOF)
			{
	//			rsTZB.Edit();
				rsTZB->get_Collect((_variant_t)_T("CLgg"), &vT1);
				CString strTmp=vtos(vT1);			
			   //����ܼ���˨��M1��ֵ����Ҫ��L8����˨M��			   
			   if(m_fDiaM1 > x)
			   {
					strSQLTmp.Format("SELECT * FROM [%s] WHERE [CUSTOMID]=\'%s\' AND [SIZE2]=%d ORDER BY SIZE2",
						modPHScal::tbnPART,modPHScal::sFindCustomID (_T("L7")),int(modPHScal::gmiDiameter));
					m_rs->Close();
	try
	{
	//				m_rs.Open(dbOpenSnapshot,strSQLTmp);
					m_rs->Open((_bstr_t)strSQLTmp,_variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
						adOpenForwardOnly, adLockReadOnly, adCmdText); 
	}
	catch(_com_error e)
	{
		ret=false;
	}
					m_rs->get_Collect((_variant_t)_T("size3"),vT1);
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
				
				rsTZB->put_Collect((_variant_t)_T("CLgg"),STR_VAR(strTmp));
				rsTZB->Update();
			}
			
		}
/*		
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
*/
	if(rsTZB->State == adOpenStatic)
		rsTZB->Close();

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
