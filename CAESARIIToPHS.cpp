// CAESARIIToPHS.cpp: implementation of the CAESARIIToPHS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAESARIIToPHS.h"
#include "SelPSAProjectNameDlg.h"
#include "user.h"
#include "frmStatus.h"
#include "EDIBgbl.h"
#include "UnitsTransformDlg.h"
#include "modphscal.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAESARIIToPHS::CAESARIIToPHS()
{
	m_IsCaesarError = FALSE;
	m_iSpringLoadType = 0;
//	m_bCheckRigPHSMaxLoad = FALSE;
	m_iUPxyz = 1;
}

CAESARIIToPHS::~CAESARIIToPHS()
{
	if(m_pConnSourceDB->State == adStateOpen)
		m_pConnSourceDB->Close();
}


//pfg20050614 ��CASEARII 4.50 �е����ݵ��뵽 AutoPhs��ȥ
void  CAESARIIToPHS::ReadResult_CAESARII45(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID,char * strPSAVER)
{
	//char* strPSAVER = "CASEARII 4.50";  
	CString ConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s;Jet OLEDB:Database Password=%s";
	CString dbConnectionString;
   
	_RecordsetPtr m_prsCASENUM;					//��̬����̬���ļ�¼��
	_RecordsetPtr m_prsHangers;					//��[OUTPUT_HANGERS]��¼��
	_RecordsetPtr m_prsDisplacements;			//��[OUTPUT_DISPLACEMENTS]��¼��
	_RecordsetPtr m_prsRestraints;				//��[OUTPUT_RESTRAINTS]��¼��
	_RecordsetPtr m_prsJGHZ;						//ÿ���ڵ����ֵ
	_RecordsetPtr m_prsJOBNAME;					//��ͬ�������ļ�¼��
	_RecordsetPtr prsInputHangers;             //��INPUT_HANGERS ��¼��	pfg20050614
	_RecordsetPtr prsInputRestraints;          //��INPUT_RESTRAINTS��¼�� pfg20050614
	_RecordsetPtr m_prsBasicData;					//������ȡ�¶ȣ��ܾ��ļ�¼��
   _RecordsetPtr m_prsTempCASENUM;				//��������TO_NODE����ʱ��¼��
	m_pConnSourceDB.CreateInstance(__uuidof(Connection));
	m_prsJOBNAME.CreateInstance(__uuidof(Recordset));
	m_prsCASENUM.CreateInstance(__uuidof(Recordset));
	prsInputHangers.CreateInstance(__uuidof(Recordset));
	prsInputRestraints.CreateInstance(__uuidof(Recordset));

	CString m_strSQL;
	CString strFind;
	_variant_t tmpvar;		
	long ltemp;
	long node;
	CString PSA_OutDataFUnit;						//Դ��ĵ�λ
	CString PSA_OutDataMUnit;
	_RecordsetPtr rsData = rsResult;
	CFile m_CAESARIIFile;							//�������ļ�����
	CString m_strCAESARIIFileName;				//�������ļ���
//����������ļ����õı���
	long loff = 272 ;			//ƫ����
	long basepoint = 180;   //����ƫ����
	bool bReadinDB = true;					//�ж��Ǵ����ݿ��ȡ���ǴӶ������ļ��ж�ȡ,ȱʡ�����ݿ��ж�ȡ
/*********************************/
	CString m_strFilePath;				
	CString m_strTYPE;
	CString m_strJOBNAME_P;				//�����ļ���*._P
	CString m_strJOBNAME_A;				//�����ļ���*._A
	CString m_strGKname;					//������
	CString m_strRGKname;				//�ȹ���������Դ������
	CString m_strLGKname;				//�乤��������Դ������
	CString* m_pstrTempJobname;
	float PI=3.1416f;
	int iC1 = 0;
	long iCount = 0;	//�ڵ����
	int nCount = 0;	//�洢�ڵ����,�õ��������ļ��нڵ�ĸ���
	ltemp = maxZdjh;
	int iJobCount=0;//���̵ĸ���
	CSelPSAProjectNameDlg* m_selDlg;
	long iType=0;//֧��������pfg20050624

//	int IsCaesarError=0;//pfg20050627 
	try
	{
		dbConnectionString.Format(ConnectionString,SourceDataFileName,"");
		m_pConnSourceDB->Open(_bstr_t(dbConnectionString),"","",0);

		m_strSQL.Format(_T("SELECT DISTINCT [JOBNAME] FROM [OUTPUT_LOCAL_ELEMENT_FORCES]"));
      //�ô˷����򿪵ļ�¼�����ܵ���GetRecodsetCount()��ü�¼��
		m_prsJOBNAME->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText); 
		//�ڶԻ�����б���м��빤���ļ���,��Ͽ���빤����������ȡ�û���ѡ��
		m_selDlg = new CSelPSAProjectNameDlg;
 
		 iJobCount= m_prsJOBNAME->GetRecordCount();
		//pfg20050705����������û��ʱ����Ӧ�÷���(start)
		if(iJobCount==0)
		{
			AfxMessageBox("����ԴΪ��!");
			return;
		}
		//pfg20050705 ����������û��ʱ����Ӧ�û�(end)
		m_selDlg->JobNameNum =iJobCount;
		m_pstrTempJobname = new CString[iJobCount];

		int i=0 ;
		for ( i=0 ;i<iJobCount;i++)
		{
			m_pstrTempJobname[i] = vtos(m_prsJOBNAME->GetCollect("JOBNAME"));
			m_prsJOBNAME->MoveNext();
		}
		m_prsJOBNAME->Close();

		m_strSQL.Empty();
		m_strSQL.Format(_T("SELECT DISTINCT [CASE],[JOBNAME] FROM [OUTPUT_LOCAL_ELEMENT_FORCES]"));
		m_prsJOBNAME->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText);
		
		int ii = m_prsJOBNAME->GetRecordCount();
		m_selDlg->GKNum = ii;
		//pfg20050705�������й��̵Ĺ���,�ڶԻ�������Ҫ���й���(start)
		m_selDlg->m_pRsJOBNAME=m_prsJOBNAME;
		//pfg20050705�������й��̵Ĺ���,�ڶԻ�������Ҫ���й���(end)
		m_selDlg->m_pstrProjectName = m_pstrTempJobname;
		if ( m_selDlg->DoModal() == IDOK)
		{
			m_strJOBNAME_P = m_selDlg->m_strTempProject ;
			m_strGKname    = m_selDlg->m_strTempGK ;
			m_iUPxyz			= m_selDlg->iUpxyz ;
			m_strRGKname   = m_selDlg->m_strTempRGK ;
			m_strLGKname	= m_selDlg->m_strTempLGK ;
			m_strUNITFORCE=m_selDlg->m_ForceUnits;//pfg20050630
			m_strUNITMOMENTAL=(m_strUNITFORCE=="N")?("N.m"):("kgf.m");//pfg20050630
		//	m_bCheckRigPHSMaxLoad = m_selDlg->m_bCheckRigPHSMaxLoad;
			m_iSpringLoadType = m_selDlg->m_iSpringLoadType;
		}
		else
		{
			return;   //���¶Ի����ȡ����ť�˳���ȡ����
		}
		m_prsJOBNAME->Close();
		delete m_selDlg;
		delete [] m_pstrTempJobname;


		//��ʾ������
		frmStatus.ShowWindow(SW_SHOW);
		frmStatus.m_Label1= GetResStr(IDS_FROMFILE) + SourceDataFileName;
		frmStatus.m_Label2= GetResStr(IDS_TODATABASE) + (LPTSTR)(LPCTSTR)EDIBgbl::dbPRJDB->DefaultDatabase + GetResStr(IDS_InTable) + EDIBgbl::TBNRawData;
		frmStatus.SetWindowText( GetResStr(IDS_DATASWITCH));
		frmStatus.UpdateData(false);
		frmStatus.UpdateStatus(0,true);

		//�ж��ǴӶ������ļ��������ݿ��ж�ȡ�ܾ����¶�
		m_strSQL.Format(_T("SELECT * FROM [INPUT_BASIC_ELEMENT_DATA] "));
		m_prsBasicData = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
		if ( m_prsBasicData->BOF && m_prsBasicData->adoEOF )
			bReadinDB = false;
		m_prsBasicData->Close();

		m_strSQL.Format(_T("SELECT * FROM [OUTPUT_LOCAL_ELEMENT_FORCES] WHERE [JOBNAME] = '%s' AND   [CASE] = '%s' "),m_strJOBNAME_P,m_strRGKname);
		m_prsCASENUM->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText);
		
		iCount = m_prsCASENUM->GetRecordCount();
		if(iCount==0)//pfg20050704û�м�¼����
		{
			CString strError;
			strError.Format("��ʾ:����='%s',����='%s'�ڱ�(OUTPUT_LOCAL_ELEMENT_FORCES)��û�м�¼!",m_strJOBNAME_P,m_strRGKname);
			AfxMessageBox(strError);
			return;
		}
		//��ȡ�������ļ���
		m_strCAESARIIFileName = m_strJOBNAME_P;
		m_strFilePath = GetFilePath(SourceDataFileName);
		m_strCAESARIIFileName = m_strFilePath + "\\" + m_strCAESARIIFileName.Left(m_strCAESARIIFileName.GetLength()-3) + "._A";
		//�������ļ���չ��Ϊ._A�����洢CAESARII�ܵ�Ӧ������ģ��ԭʼ����
		m_strJOBNAME_A = m_strJOBNAME_P.Left(m_strJOBNAME_P.GetLength()-3)+"._A";

		//��ȡԴ���еĵ�λ����
		tmpvar = m_prsCASENUM->GetCollect("FUNITS");
		PSA_OutDataFUnit = vtos(tmpvar);
		
		tmpvar = m_prsCASENUM->GetCollect("MUNITS");
		PSA_OutDataMUnit = vtos(tmpvar);

		//ȡ���ܼ�¼��
		m_strSQL.Format("select * from input_hangers where jobname='%s'",m_strJOBNAME_A);
		try
		{
			prsInputHangers->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText);
			if(prsInputHangers->GetRecordCount()>0)
			{
				prsInputHangers->MoveFirst();
			}
		}
		catch(_com_error e)
		{
			CString strErrorMsg;
			strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
			AfxMessageBox(strErrorMsg);
		}
		//ȡ֧������
		m_strSQL.Format("select * from input_restraints where jobname='%s'",m_strJOBNAME_A);
		try
		{
			prsInputRestraints->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText);
			if(prsInputRestraints->GetRecordCount()>0)
			{
				prsInputRestraints->MoveFirst();
			}
		}
		catch(_com_error e)
		{
			CString strErrorMsg;
			strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
			AfxMessageBox(strErrorMsg);
		}
		//20050624pfg(start)
		int n=0,m=0;
		n=prsInputHangers->GetRecordCount();
		n+=prsInputRestraints->GetRecordCount();
		if(n==0)//pfg20050624 ��֧���ܵĸ���Ϊ��ʱ����
		{
			AfxMessageBox("��:INPUT_HANGERS��INPUT_RESTRAINTSΪ��!");
			return;
		}
		//20050624pfg(end)
		
		for(i=0;i<2;i++)
		{
			if(i==0)
			{
				//�������
				for(;!prsInputHangers->adoEOF;prsInputHangers->MoveNext())
				{
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					//�����������
					ImportHangerRestraints(rsData,prsInputHangers,ltemp,FileNameID,strPSAVER,node,0);
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					
					//��������������
					importUnitsForces(rsData,node,m_strJOBNAME_P,PSA_OutDataFUnit,PSA_OutDataMUnit,m_strRGKname,m_strLGKname);
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					
					//������̬��λ�ơ���λ��
					ImportDisplacements(rsData,node,m_strRGKname,m_strJOBNAME_P,1);
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					
					//������̬��λ�ơ���λ��
					ImportDisplacements(rsData,node,m_strLGKname,m_strJOBNAME_P,0);
					
					//����ܵ�ֱ����
					ImportDiameter(rsData,node,m_strJOBNAME_A);
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					
					//���뵯������(������������ת��)
					ImportHanger(rsData,node,m_strJOBNAME_P);
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					
					//ȷ��֧��������pfg20050622(ת������֧���ܺͺ�����������)
				//	ConversionTypeCaesarToPhs(rsData,node,m_strJOBNAME_P,m_strRGKname,iType);
				//	ConversionTypeCaesarToPhs(rsData,node,m_strJOBNAME_P,m_strLGKname,iType);
					//pfg20050624(start)
					m++;
					frmStatus.UpdateStatus((float)m/(float)n,true);
					frmStatus.UpdateWindow();
					//pfg20050624(end)
					rsData->Filter="";
					rsData->Update();//pfg20050630
				}
				rsData->Filter="";//pfg20050627
			}   //������ܽ���
			else
			{
				//����֧��(start)
				for(;!prsInputRestraints->adoEOF;prsInputRestraints->MoveNext())
				{
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					//�����������
				    ImportHangerRestraints(rsData,prsInputRestraints,ltemp,FileNameID,strPSAVER,node,1);
					if ( node < 1e-6 ) //5.2�汾�����е�Լ����Ϣ�ᵼ��INPUT_RESTRAINTS����(ͬһԼ�������������¼�����Ի���NODE_NUMΪ-1���)
						continue; 
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					
					//ȷ��֧��������pfg20050622
					ConversionTypeCaesarToPhs(rsData,node,m_strJOBNAME_P,m_strRGKname,iType);
			//		ConversionTypeCaesarToPhs(rsData,node,m_strJOBNAME_P,m_strLGKname,iType);
					
					if(iType==177)//pfg20050624���ǹ̶�֧��ʱ��������̬������һ�ֹ���
					{
						//��������������
						importUnitsForces(rsData,node,m_strJOBNAME_P,PSA_OutDataFUnit,PSA_OutDataMUnit,m_strRGKname,m_strGKname);
						if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
						
						//������̬��λ�ơ���λ��
						ImportDisplacements(rsData,node,m_strGKname,m_strJOBNAME_P,0);
						if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					}
					else
					{
						//��������������
						importUnitsForces(rsData,node,m_strJOBNAME_P,PSA_OutDataFUnit,PSA_OutDataMUnit,m_strRGKname,m_strLGKname);
						if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
						
						//������̬��λ�ơ���λ��
						ImportDisplacements(rsData,node,m_strLGKname,m_strJOBNAME_P,0);
						if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					}

					//������̬��λ�ơ���λ��
					ImportDisplacements(rsData,node,m_strRGKname,m_strJOBNAME_P,1);
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					
					//����ܵ�ֱ����
					ImportDiameter(rsData,node,m_strJOBNAME_A);
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					
					//���뵯������
				//	ImportHanger(rsData,node,m_strJOBNAME_P);
					if(m_IsCaesarError) break;//pfg20050627����λת��������ֹ��������
					//pfg20050624(start)
					m++;
					frmStatus.UpdateStatus((float)m/(float)n,true);
					frmStatus.UpdateWindow();
					//pfg20050624(end)
					rsData->Filter="";
					rsData->Update();//pfg20050630
				}
				rsData->Filter="";//pfg20050627
			}   //����֧�ܽ���(end)
			//rsData->Update();//pfg20050630
		}
	}catch(_com_error e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
}


//���ܺ�֧�����ݵĵ���pfg20050615
//flag=0 Ϊ����; flag=1 Ϊ֧��
//��¼���ǽ���� ��ӦZA��
//rsHR��¼��������Դ��(���ɡ�����)
//ltemp ֧���ܺ�
void CAESARIIToPHS::ImportHangerRestraints(_RecordsetPtr rsData,_RecordsetPtr rsHR,long &ltemp,long &FileNameID,char *strPSAVER,long &node,int flag)
{
	_variant_t tmpvar; 
	long lTmp;
	try
	{
		if(flag)
		{
			tmpvar=rsHR->GetCollect(_T("node_num"));
			lTmp=vtoi(rsHR->GetCollect(_T("RES_TYPEID")));//pfg20050624
		}
		else
		{
			tmpvar=rsHR->GetCollect(_T("node"));
		}
		if(tmpvar.vt==VT_EMPTY||tmpvar.vt==VT_NULL)
		{
			return;
		}
		else
		{
			node=vtoi(tmpvar);
		}
		if ( node < 1e-6 ) //5.2�汾�����е�Լ����Ϣ�ᵼ��INPUT_RESTRAINTS����(ͬһԼ�������������¼�����Ի���NODE_NUMΪ-1���)
			return;
		if(rsData->GetRecordCount()>0)
		{
			rsData->MoveFirst();
		}
		
		rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node)+" AND [FileNameID]="+ltos(FileNameID));
		
		if (rsData->adoEOF&&rsData->BOF)
		{
			rsData->AddNew();
			rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
			rsData->PutCollect("zdjh",_variant_t((long)(ltemp+1)));
			rsData->PutCollect("FileNameID",_variant_t(FileNameID));
			ltemp++;
			rsData->Update();					
		}
		rsData->PutCollect("PSAver", _variant_t(strPSAVER));
		rsData->PutCollect("UPxyz",_variant_t((long)m_iUPxyz));
		rsData->PutCollect("JSDBH1", _variant_t((long)node));
		rsData->Update();
	}
	catch(_com_error e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
}

//����֧�ܡ������еĵ�λ���������������(��̬����̬)
//rsDataΪ������ݿ�	m_strLGKnameΪ��̬��m_strRGKnameΪ��̬
//m_strJOBNAME_P ��������
void CAESARIIToPHS::importUnitsForces(_RecordsetPtr rsData,long node,CString m_strJOBNAME_P,CString PSA_OutDataFUnit,CString PSA_OutDataMUnit,CString m_strRGKname,CString m_strLGKname)
{
	CString strSQL;
	CString strMessage;
	_RecordsetPtr prsOutputRestraints;
	int i=0;
	int IsXYZ=0;//�Ƿ���x,y,z�����Ѿ����������ݣ���ΪԼ�����ͺܶ࣬����ͬһ�����ж�
				//��Լ��ʱ����Ҫ��ʾ�û����ǻ�û�ж���һ��������д��� ����λΪ1
				//��ʾx�����Ѿ����������ݣ���������
	int IsmXYZ=0;//��ʾ���أ�������IsXYZ����
	double tmpVar;
	double dFx=1.0;//����ת������
	double dFOffset=0.0;//����ƫ����
	double dMx=1.0;//���ص�ת������
	double dMOffset=0.0;//���ص�ƫ����
	CString strFUnits;//���ĵ�λ��
	CString strMUnits;//���صĵ�λ

	double dICCWorkLoad = 0.0,dICCIncLoad = 0.0; //��������̬����̬����

	strFUnits=PSA_OutDataFUnit;
	strMUnits=PSA_OutDataMUnit;
	CString strTmp;
	prsOutputRestraints.CreateInstance(__uuidof(Recordset));
	try
	{
		//ת�������ĵ�λ:kgf;���صĵ�λ:kgf.mд��ZA����(start)			 				 
		rsData->PutCollect("Unit_Force",_variant_t(m_strUNITFORCE));
		rsData->PutCollect("Unit_Momental",_variant_t(m_strUNITMOMENTAL));
		//ת�������ĵ�λ:kgf;���صĵ�λ:kgf.mд��ZA����(end)

		//������̬�ĺ��ء�����(start)
		strSQL.Format(_T("SELECT * FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [CASE] = '%s' and JOBNAME='%s' "),node,m_strRGKname,m_strJOBNAME_P);
		prsOutputRestraints = m_pConnSourceDB->Execute(_bstr_t(strSQL),NULL,adCmdText);
		if(!(prsOutputRestraints->BOF&&prsOutputRestraints->adoEOF))
		{
			//pfg20050622(start)
			strTmp=vtos(prsOutputRestraints->GetCollect(_T("FUNITS")));
			strFUnits=(strTmp!="")?strTmp:strFUnits;
			//��λ����
			dFx=UnitsToUeUnits(rsData,strFUnits,m_strUNITFORCE,dFOffset);
			if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������
			strTmp=vtos(prsOutputRestraints->GetCollect(_T("MUNITS")));
			strMUnits=(strTmp!="")?strTmp:strMUnits;
			dMx=UnitsToUeUnits(rsData,strMUnits,m_strUNITMOMENTAL,dMOffset);
			if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������
			//pfg20050622(end)
			prsOutputRestraints->MoveFirst();
			while(!prsOutputRestraints->adoEOF)
			{
				tmpVar=vtof(prsOutputRestraints->GetCollect("FX"));
				if( (tmpVar!=0) && (IsXYZ/100==0) || i==0 )
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("pxr1",_variant_t(tmpVar*dFx+dFOffset));
					IsXYZ=(tmpVar!=0)?(IsXYZ+100):IsXYZ;
					if ( m_iUPxyz == 1 ) //X������
					{
						dICCWorkLoad = tmpVar*dFx+dFOffset;
					}
				}
				else if((tmpVar!=0)&&(IsXYZ/100))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������X�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}

				tmpVar=vtof(prsOutputRestraints->GetCollect("FY"));
				if(((tmpVar!=0)&&(((IsXYZ%100)/10)==0))||(i==0))
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("pyr1",_variant_t(tmpVar*dFx+dFOffset));
					IsXYZ=(tmpVar!=0)?(IsXYZ+10):IsXYZ;
					if ( m_iUPxyz == 2 ) //Y������
					{
						dICCWorkLoad = tmpVar*dFx+dFOffset;
					}
				}
				else if((tmpVar!=0)&&((IsXYZ%100)/10))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������Y�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				tmpVar=vtof(prsOutputRestraints->GetCollect("FZ"));
				if(((tmpVar!=0)&&(IsXYZ%10==0))||(i==0))
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("pzr1",_variant_t(tmpVar*dFx+dFOffset));
					IsXYZ=(tmpVar!=0)?(IsXYZ+1):IsXYZ;
					if ( m_iUPxyz == 3 ) //Z������
					{
						dICCWorkLoad = tmpVar*dFx+dFOffset;
					}
				}
				else if((tmpVar!=0)&&(IsXYZ%10))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������Z�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				tmpVar=vtof(prsOutputRestraints->GetCollect("MX"));
				if((tmpVar!=0)&&(IsmXYZ/100==0)||i==0)
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("mxr1",_variant_t(tmpVar*dMx+dMOffset));
					IsmXYZ=(tmpVar!=0)?(IsmXYZ+100):IsmXYZ;
				}
				else if((tmpVar!=0)&&(IsmXYZ/100))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������X�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				tmpVar=vtof(prsOutputRestraints->GetCollect("MY"));
				if(((tmpVar!=0)&&(((IsmXYZ%100)/10)==0))||(i==0))
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("myr1",_variant_t(tmpVar*dMx+dMOffset));
					IsmXYZ=(tmpVar!=0)?(IsmXYZ+10):IsmXYZ;
				}
				else if((tmpVar!=0)&&((IsmXYZ%100)/10))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������Y�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				tmpVar=vtof(prsOutputRestraints->GetCollect("MZ"));
				if(((tmpVar!=0)&&(IsmXYZ%10==0))||(i==0))
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("mzr1",_variant_t(tmpVar*dMx+dMOffset));
					IsmXYZ=(tmpVar!=0)?(IsmXYZ+1):IsmXYZ;
				}
				else if((tmpVar!=0)&&(IsmXYZ%10))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������Z�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				i++;
				prsOutputRestraints->MoveNext();
			}
		}
		if(prsOutputRestraints->State==adStateOpen)
		{
			prsOutputRestraints->Close();
		}
		//������̬�ĺ��ء�����(end)

		//������̬�ĺ��ء�����(start)
		IsXYZ=0;
		IsmXYZ=0;
		i=0;
		strSQL.Format(_T("SELECT * FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [CASE] = '%s' and JOBNAME='%s' "),node,m_strLGKname,m_strJOBNAME_P);
		prsOutputRestraints = m_pConnSourceDB->Execute(_bstr_t(strSQL),NULL,adCmdText);
		if(!(prsOutputRestraints->BOF&&prsOutputRestraints->adoEOF))
		{
			//pfg20050622(start)
			strTmp=vtos(prsOutputRestraints->GetCollect(_T("FUNITS")));
			strFUnits=(strTmp!="")?strTmp:strFUnits;
			//��λ����
			dFx=UnitsToUeUnits(rsData,strFUnits,m_strUNITFORCE,dFOffset);
			if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������
			strTmp=vtos(prsOutputRestraints->GetCollect(_T("MUNITS")));
			strMUnits=(strTmp!="")?strTmp:strMUnits;
			dMx=UnitsToUeUnits(rsData,strMUnits,m_strUNITMOMENTAL,dMOffset);
			if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������
			//pfg20050622(end)
			prsOutputRestraints->MoveFirst();
			while(!prsOutputRestraints->adoEOF)
			{
				tmpVar=vtof(prsOutputRestraints->GetCollect("FX"));
				if((tmpVar!=0)&&(IsXYZ/100==0)||i==0)
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("pxl1",_variant_t(tmpVar*dFx+dFOffset));
					IsXYZ=(tmpVar!=0)?(IsXYZ+100):IsXYZ;

					if ( m_iUPxyz == 1 ) //X������
					{
						dICCIncLoad = tmpVar*dFx+dFOffset;
					}
				}
				else if((tmpVar!=0)&&(IsXYZ/100))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������X�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}

				tmpVar=vtof(prsOutputRestraints->GetCollect("FY"));
				if(((tmpVar!=0)&&(((IsXYZ%100)/10)==0))||(i==0))
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("pyl1",_variant_t(tmpVar*dFx+dFOffset));
					IsXYZ=(tmpVar!=0)?(IsXYZ+10):IsXYZ;
					if ( m_iUPxyz == 2 ) //Y������
					{
						dICCIncLoad = tmpVar*dFx+dFOffset;
					}
				}
				else if((tmpVar!=0)&&((IsXYZ%100)/10))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������Y�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				tmpVar=vtof(prsOutputRestraints->GetCollect("FZ"));
				if(((tmpVar!=0)&&(IsXYZ%10==0))||(i==0))
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("pzl1",_variant_t(tmpVar*dFx+dFOffset));
					IsXYZ=(tmpVar!=0)?(IsXYZ+1):IsXYZ;
					if ( m_iUPxyz == 3 ) //Z������
					{
						dICCIncLoad = tmpVar*dFx+dFOffset;
					}
				}
				else if((tmpVar!=0)&&(IsXYZ%10))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������Z�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				tmpVar=vtof(prsOutputRestraints->GetCollect("MX"));
				if((tmpVar!=0)&&(IsmXYZ/100==0)||i==0)
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("mxl1",_variant_t(tmpVar*dMx+dMOffset));
					IsmXYZ=(tmpVar!=0)?(IsmXYZ+100):IsmXYZ;
				}
				else if((tmpVar!=0)&&(IsmXYZ/100))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������X�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				tmpVar=vtof(prsOutputRestraints->GetCollect("MY"));
				if(((tmpVar!=0)&&(((IsmXYZ%100)/10)==0))||(i==0))
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("myl1",_variant_t(tmpVar*dMx+dMOffset));
					IsmXYZ=(tmpVar!=0)?(IsmXYZ+10):IsmXYZ;
				}
				else if((tmpVar!=0)&&((IsmXYZ%100)/10))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������Y�������м�������(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				tmpVar=vtof(prsOutputRestraints->GetCollect("MZ"));
				if(((tmpVar!=0)&&(IsmXYZ%10==0))||(i==0))
				{//�����ǵ�һ�ε��������ǰ����һ����û�к���
					rsData->PutCollect("mzl1",_variant_t(tmpVar*dMx+dMOffset));
					IsmXYZ=(tmpVar!=0)?(IsmXYZ+1):IsmXYZ;
				}
				else if((tmpVar!=0)&&(IsmXYZ%10))
				{
					strMessage.Format("����:�ڵ��Ϊ%d��֧������Z�������м�����(���Ķ�ʹ��˵��)!",node);
					AfxMessageBox(strMessage);
				}
				i++;
				prsOutputRestraints->MoveNext();
			}
		}
		if(prsOutputRestraints->State==adStateOpen)
		{
			prsOutputRestraints->Close();
		}

		//�Ȱ�Ĭ�ϵ�ֵ���ù������غͰ�װ����
		rsData->PutCollect("PGZ1",_variant_t( dICCWorkLoad) ); //��������
		rsData->PutCollect("paz0", _variant_t( dICCIncLoad ) ); //��װ����
		rsData->PutCollect("paz1", _variant_t( dICCIncLoad ) ); 

		double dMaxLoad = 0.0;
		//������̬�ĺ��ء�����(end)
		if ( IsHanger( node,m_strJOBNAME_P ) )
		{
			if ( m_iSpringLoadType == 1 ) //���ɺ�����ȡ������ʽ(0:������ѡ�����/��̬��ȡ����ֵ 1��CAESARII���ɱ��еĺ���ֵ��ȡ)
			{
				double dWorkLoad = 0.0,dInsLoad = 0.0;
				if ( GetHangerLoad( node,m_strJOBNAME_P,dWorkLoad,dInsLoad ) ) //�õ����ɺ���
				{
					rsData->PutCollect("PGZ1",_variant_t( dWorkLoad*dFx+dFOffset) );
					rsData->PutCollect("paz0", _variant_t( dInsLoad*dFx+dFOffset )); //��̬
					rsData->PutCollect("paz1", _variant_t( dInsLoad*dFx+dFOffset ));

					//�ı�������ĺ���ֵ
					if ( m_iUPxyz == 1 )
					{
						rsData->PutCollect("pxl1", _variant_t( dInsLoad*dFx+dFOffset ));//��̬����
						rsData->PutCollect("pxr1", _variant_t( dWorkLoad*dFx+dFOffset ));//��̬����
					}
					else if ( m_iUPxyz == 2 )
					{
						rsData->PutCollect("pyl1", _variant_t( dInsLoad*dFx+dFOffset ));//��̬����
						rsData->PutCollect("pyr1", _variant_t( dWorkLoad*dFx+dFOffset ));//��̬����
					}
					else if ( m_iUPxyz == 3 )
					{
						rsData->PutCollect("pzl1", _variant_t( dInsLoad*dFx+dFOffset ));//��̬����
						rsData->PutCollect("pzr1", _variant_t( dWorkLoad*dFx+dFOffset ));//��̬����
					}
				}
			}
		}
		
	}
	catch(_com_error e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
}

//����֧���ܵ���λ�ơ���λ��
//rsData �����(ZA)��¼����strLGkname �������ơ�strJDBNAME_P ��������
//flag =1 ��̬ ��flag=0Ϊ��̬
void CAESARIIToPHS::ImportDisplacements(_RecordsetPtr rsData,long node, CString strLGKname,CString strJOBNAME_P,int flag)
{
	double x= 1.0;//��λ��������
	double dLOffset=0.0;//���ȵ�ƫ��
	double dROffset=0.0;//�Ƕȵ�ƫ��
	CString strSQL,strError;
	CString strUnits;
	_RecordsetPtr prsDisplacements;
	prsDisplacements.CreateInstance(__uuidof(Recordset));
	try
	{
		if(flag)
		{
			//��̬
			strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s' "),node,strLGKname,strJOBNAME_P);						
			prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(strSQL),NULL,adCmdText);
			if ( prsDisplacements->adoEOF && prsDisplacements->BOF )
			{
				strError.Format( _T("OUTPUT_DISPLACEMENTS����û�е��Ϊ%d�ù�����Ϊ%s������"),node,strLGKname );
				AfxMessageBox( strError );
				prsDisplacements->Close();
				return;
			}
			
			//���ȵ�λ����
			strUnits=vtos(prsDisplacements->GetCollect(_T("DUNITS")));
			strUnits.TrimLeft();
			strUnits.TrimRight();
			strUnits.MakeLower();
			if (strUnits == "mm")
			{
				x=1.0;
			}
			else
			{
				x=UnitsToUeUnits(rsData,strUnits,"mm",dLOffset);
				if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������

			}
			//��̬��λ��
			rsData->PutCollect("xr1",_variant_t(vtof(prsDisplacements->GetCollect("dX"))*x+dLOffset));
			rsData->PutCollect("yr1",_variant_t(vtof(prsDisplacements->GetCollect("dY"))*x+dLOffset));					 
			rsData->PutCollect("zr1",_variant_t(vtof(prsDisplacements->GetCollect("dZ"))*x+dLOffset));	
			rsData->PutCollect("UNIT_Displacement",_T("mm"));

			//�Ƕȵ�λ����
			strUnits=vtos(prsDisplacements->GetCollect(_T("RUNITS")));
			strUnits.TrimLeft();
			strUnits.TrimRight();
			if(strUnits=="����")
			{
				x=1.0;
			}
			else
			{
				x=UnitsToUeUnits(rsData,strUnits,"����",dROffset);
				if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������

			}
			//��̬��λ��
			rsData->PutCollect("rxR1",_variant_t(vtof(prsDisplacements->GetCollect("RX"))*x+dROffset));
			rsData->PutCollect("ryR1",_variant_t(vtof(prsDisplacements->GetCollect("RY"))*x+dROffset));					 
			rsData->PutCollect("rzR1",_variant_t(vtof(prsDisplacements->GetCollect("RZ"))*x+dROffset));	
		}
		else
		{
			//��̬
			strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s' "),node,strLGKname,strJOBNAME_P);						
			prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(strSQL),NULL,adCmdText);

			if ( prsDisplacements->adoEOF && prsDisplacements->BOF )
			{
				strError.Format( _T("OUTPUT_DISPLACEMENTS����û�е��Ϊ%d�ù�����Ϊ%s������"),node,strLGKname );
				AfxMessageBox( strError );
				prsDisplacements->Close();
				return;
			}
			
			//���ȵ�λ����
			strUnits=vtos(prsDisplacements->GetCollect(_T("DUNITS")));
			strUnits.TrimLeft();
			strUnits.TrimRight();
			strUnits.MakeLower();
			if (strUnits == "mm")
			{
				x=1.0;
			}
			else
			{
				x=UnitsToUeUnits(rsData,strUnits,"mm",dLOffset);
				if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������

			}
			//��̬��λ��
			rsData->PutCollect("xL1",_variant_t(vtof(prsDisplacements->GetCollect("dX"))*x+dLOffset));
			rsData->PutCollect("yL1",_variant_t(vtof(prsDisplacements->GetCollect("dY"))*x+dLOffset));					 
			rsData->PutCollect("zL1",_variant_t(vtof(prsDisplacements->GetCollect("dZ"))*x+dLOffset));	

			//�Ƕȵ�λ����
			strUnits=vtos(prsDisplacements->GetCollect(_T("RUNITS")));
			strUnits.TrimLeft();
			strUnits.TrimRight();
			if(strUnits=="����")
			{
				x=1.0;
			}
			else
			{
				x=UnitsToUeUnits(rsData,strUnits,"����",dROffset);
				if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������

			}
			//��̬��λ��
			rsData->PutCollect("rxL1",_variant_t(vtof(prsDisplacements->GetCollect("RX"))*x+dROffset));
			rsData->PutCollect("ryL1",_variant_t(vtof(prsDisplacements->GetCollect("RY"))*x+dROffset));					 
			rsData->PutCollect("rzL1",_variant_t(vtof(prsDisplacements->GetCollect("RZ"))*x+dROffset));	
		}
	}
	catch(_com_error e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
}

//����ܵ��⾶�������¶�
void CAESARIIToPHS::ImportDiameter(_RecordsetPtr rsData,long node,CString strJOBNAME_A)
{
	CString strSQL;
	_RecordsetPtr pRs;
	double dDiameterx=1.0;//ֱ���Ļ�������
	double dDOffset=0.0;//ֱ��ƫ����
	double dT01x=1.0;//�¶ȵĻ�������
	double dTOffset=0.0;//�¶�ƫ����
	CString str;
	pRs.CreateInstance(__uuidof(Recordset));
	try
	{
		strSQL.Format(_T("select * from input_units where jobname='%s'"),strJOBNAME_A);
		pRs=m_pConnSourceDB->Execute(_bstr_t(strSQL),NULL,adCmdText);
		str=vtos(pRs->GetCollect(_T("LENGTH")));
		str.TrimLeft();
		str.TrimRight();
		str.MakeUpper();
		if(!(str=="MM"))
		{
			dDiameterx=UnitsToUeUnits(rsData,str,"MM",dDOffset);
			if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������
			
		}
		str=vtos(pRs->GetCollect(_T("TEMP")));
		str.TrimLeft();
		str.TrimRight();
		str.MakeUpper();
		//pfg20050630
		dT01x=UnitsToUeUnits(rsData,str,"C",dTOffset);
		if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������

//chenbl 2010.05.15
		
	//�ҳ���Ԫ��¼
		//��������
		pRs->Close();
		strSQL.Format(_T("SELECT * FROM [INPUT_BASIC_ELEMENT_DATA] WHERE [JOBNAME] =  '%s' AND [FROM_NODE] = %d"),strJOBNAME_A,node);
		pRs->Open(_variant_t(strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText);
		if ( pRs->BOF && pRs->adoEOF ) //û���ҵ���¼
		{
			pRs->Close();
			//��ĩ��
			strSQL.Format(_T("SELECT * FROM [INPUT_BASIC_ELEMENT_DATA] WHERE [JOBNAME] =  '%s' AND [TO_NODE] = %d"),strJOBNAME_A,node);
			pRs->Open(_variant_t(strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText);
			if ( pRs->BOF && pRs->adoEOF ) //û���ҵ���¼
			{
				pRs->Close();
				strSQL.Format(_T("SELECT * FROM [INPUT_BASIC_ELEMENT_DATA],[INPUT_BENDS] WHERE [INPUT_BENDS].[JOBNAME] =  '%s' AND  ( [INPUT_BENDS].NODE1 = %d OR [INPUT_BENDS].NODE2 = %d OR [INPUT_BENDS].NODE3 = %d ) AND [INPUT_BASIC_ELEMENT_DATA].BEND_PTR = [INPUT_BENDS].BEND_PTR"),
					strJOBNAME_A,node,node,node);
				pRs->Open(_variant_t(strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText);
				if ( pRs->BOF && pRs->adoEOF ) //û���ҵ���¼
				{
					pRs->Close();
					return;
				}
			}
		}
		rsData->PutCollect("DJ1",_variant_t(vtof(pRs->GetCollect("DIAMETER"))*dDiameterx+dDOffset));
		rsData->PutCollect("T01",_variant_t(vtof(pRs->GetCollect("TEMP_EXP_C1"))*dT01x+dTOffset));
		
		if(pRs->State==adStateOpen)
		{
			pRs->Close();
		}
	}
	catch(_com_error e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
}

//���뵯������(��������)
void CAESARIIToPHS::ImportHanger(_RecordsetPtr rsData,long node,CString strJOBNAME_P)
{
	CString strSQL;
	_RecordsetPtr pRs;
	double dLoadx=1.0;//���ĵ�λת������
	double dFOffset=0.0;//����ƫ����
	_variant_t tmpvar;
	CString str;
	CString strSize;
	CString strPHSType;
	try
	{
		strSQL.Format(_T("SELECT * FROM [OUTPUT_HANGERS] WHERE [NODE] = %d AND [JOBNAME] =  '%s'" ),
			node,strJOBNAME_P);
		pRs = m_pConnSourceDB->Execute(_bstr_t(strSQL),NULL,adCmdText);
		if(!(pRs->BOF&&pRs->adoEOF))
		{
			pRs->MoveFirst();
			//���ĵ�λת��
		/*
			str=vtos(pRs->GetCollect(_T("LOAD_UNITS")));
			str.TrimLeft();
			str.TrimRight();
			str.MakeLower();
			dLoadx=UnitsToUeUnits(rsData,str,m_strUNITFORCE,dFOffset);
			if(m_IsCaesarError) return;//pfg20050629����λת��������ֹ��������
		*/	
			//pgz1���������Ѿ��ڱ�:OUTPUT_RESTRAINTS�е�����
			//	rsData->PutCollect("pgz1",_variant_t(vtof(pRs->GetCollect("HOT_LOAD"))*dLoadx));
		//	rsData->PutCollect("paz0",_variant_t(vtof(pRs->GetCollect("TH_INSTALL_LOAD"))*dLoadx+dFOffset));
		//	rsData->PutCollect("paz1",_variant_t(vtof(pRs->GetCollect("AC_INSTALL_LOAD"))*dLoadx+dFOffset));
			
			double dLoadVariation = 0.0;
			dLoadVariation = vtof( pRs->GetCollect("LOAD_VARIATION") );
			if ( dLoadVariation < 1E-6 )
			{
				strPHSType = _T("9999"); //��������
			}
			else //��������
			{
				rsData->PutCollect("CoefOfVarLoad",_variant_t( dLoadVariation /100 ) );
				tmpvar = pRs->GetCollect("SIZE");
				strSize.Format(_T("%02d"),vtoi(tmpvar));
				
				//������
				int iParalNum = vtoi( pRs->GetCollect( _T("NUMREQ") ) );
				CString strHangerType = vtos( pRs->GetCollect( _T("FIGNUM") ) );
				//������
				int iSeriesNum = 0;
				if ( !GetHangerSeriesNum(strHangerType,iSeriesNum) ) //�õ����ɲ��ɹ�
				{
					CString strError;
					strError.Format( _T("�޷�ͨ����������%s��õ��ɵĴ�����,������Ĭ��Ϊ1"),strHangerType );
					AfxMessageBox( strError );
					iSeriesNum = 1;
				}
				
				//�γ�����GLIF�����֧�������ͺ�								
				strPHSType.Format( _T("%d%d%s"),iSeriesNum,iParalNum,strSize );
				
			}
			rsData->PutCollect("TYPE",_variant_t(strPHSType));
			rsData->PutCollect("psaTYPE",_variant_t(strPHSType));
		}
	}
	catch(_com_error e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
	if ( pRs->State == adStateOpen )
	{
		pRs->Close();
	}
}

//ȷ��֧�ܡ���������(Լ��)
//rsData �����¼��(ZA��)
//node ��ǰ��֧���ڵ�
//strJOBNAME_A ��������,strLGKname
//���أ��ɹ���֧���ܵ����ͣ����û�з���0 �����ٸ�����һ�ֹ������� 
//ע�⣺����֧����ֻ�򵥷�Ϊ�ɱ䵯�ɺͺ�������
//֧��Ҳֻ�������¼���:X��Y��Z��XY��XZ��YZ��XYZ
//iType:֧��������
void CAESARIIToPHS::ConversionTypeCaesarToPhs(_RecordsetPtr rsData,long node,CString strJOBNAME_P,CString strGKname,long &iType)
{
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	int i=0;
	iType=0;//֧��������:AutoPSA��AutoPHS��Լ������
	CString strType;//CAESARII��Լ������
	CString strLineType;//��λ�Ƶ�Լ������
	CString strRadType;//��λ�Ƶ�Լ������
	short iDECType=0;//��λ��(ʮλ)��Լ������
	short iNUMType=0;//��λ��(��λ)��Լ������
	CString strSQL;	
	strLineType=_T("");
	try
	{
		strSQL.Format(_T("SELECT * FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [CASE] = '%s' and JOBNAME='%s' "),node,strGKname,strJOBNAME_P);
		pRs->Open(_variant_t(strSQL),(IDispatch*)m_pConnSourceDB,adOpenStatic,adLockOptimistic,adCmdText);
		
		while ( !pRs->adoEOF )
		{
			strType = vtos( pRs->GetCollect( _T("TYPE") ) );
			strType.MakeUpper();
			//��λ��Լ��
			if(strType=="RIGID X" || strType=="RIGID +X" || strType=="RIGID -X")
			{
				strLineType+="x";	
			}
			else if(strType=="RIGID Y" || strType=="RIGID +Y" || strType=="RIGID -Y")
			{
				strLineType+="y";
			}
			else if(strType=="RIGID Z" || strType=="RIGID +Z" || strType=="RIGID -Z")
			{
				strLineType+="z";
			}
			
			//��λ��Լ��
			if(strType=="RIGID RX" || strType=="RIGID +RX" || strType=="RIGID -RX")
			{
				strRadType+="x";	
			}
			else if(strType=="RIGID RY" || strType=="RIGID +RY" || strType=="RIGID -RY")
			{
				strRadType+="y";
			}
			else if(strType=="RIGID RZ" || strType=="RIGID +RZ" || strType=="RIGID -RZ")
			{
				strRadType+="z";
			}
			else if(strType=="RIGID ANC")
			{
				strLineType = _T("xyz");
				strRadType+="xyz";
			} 
			else if ( strType == _T("GUIDE") )
			{
				iType = 260; //����
				break;
			}
			pRs->MoveNext();
		}
		if( iType == 0)
		{//�ж϶෽��Լ����֧��������(start)
			//��ȷ��Լ������ʮλ�ϵ����ӣ�������λ��ȷ��
			//pfg20050630
			strLineType.TrimLeft();
			strLineType.TrimRight();
			if(!strLineType.IsEmpty())
			{
				//pfg20050630
				if(strLineType.Find("x")!=-1 && strLineType.Find("y")!=-1 && strLineType.Find("z")!=-1)
				{
					iDECType=7;
				}
				else if(strLineType.Find("y")!=-1 && strLineType.Find("z")!=-1 )
				{
					iDECType=6;
				}
				else if(strLineType.Find("x")!=-1 && strLineType.Find("z")!=-1 )
				{
					iDECType=5;
				}
				else if(strLineType.Find("x")!=-1 && strLineType.Find("y")!=-1 )
				{
					iDECType=4;
				}
				else if(strLineType.Find("z")!=-1 )
				{
					iDECType=3;
				}
				else if(strLineType.Find("y")!=-1 )
				{
					iDECType=2;
				}
				else if(strLineType.Find("x")!=-1 )
				{
					iDECType=1;
				}
			}
			//��ȷ��Լ�����͸�λ�ϵ����ӣ����ɽ�λ��ȷ��
			//pfg20050630
			strRadType.TrimLeft();
			strRadType.TrimRight();
			if(!strRadType.IsEmpty())
			{
				//pfg20050630
				if(strRadType.Find("x")!=-1 && strRadType.Find("y")!=-1 && strRadType.Find("z")!=-1)
				{
					iNUMType=7;
				}
				else if(strRadType.Find("y")!=-1 && strRadType.Find("z")!=-1 )
				{
					iNUMType=6;
				}
				else if(strRadType.Find("x")!=-1 && strRadType.Find("z")!=-1 )
				{
					iNUMType=5;
				}
				else if(strRadType.Find("x")!=-1 && strRadType.Find("y")!=-1 )
				{
					iNUMType=4;
				}
				else if(strRadType.Find("z")!=-1 )
				{
					iNUMType=3;
				}
				else if(strRadType.Find("y")!=-1 )
				{
					iNUMType=2;
				}
				else if(strRadType.Find("x")!=-1 )
				{
					iNUMType=1;
				}
			}
			//ת����AutoPSA/AutoPHSԼ������iType
			iType=100+10*iDECType+iNUMType;
		}

		if(iType!=0)
		{
			rsData->PutCollect("TYPE",_variant_t(iType));
			rsData->PutCollect("psaTYPE",_variant_t(iType));
		}
	}
	catch(_com_error e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
}

//��λ����
//SourceUnits ��ת���ĵ�λ
//UeUnits ת����ĵ�λ
//���ص�λ��������
//dOffset ƫ��ֵ
double CAESARIIToPHS::UnitsToUeUnits(_RecordsetPtr rsData,CString SourceUnits,CString UeUnits,double &dOffset)
{
	double x;//��λ��������
	CString strSQL;                                            
	_RecordsetPtr pRs;
	_ConnectionPtr pCon;
	pCon.CreateInstance(__uuidof(Connection));
	pRs.CreateInstance(__uuidof(Recordset));
	strSQL="select * from UnitsToUeUnits ";//ɾ����pfg20050629where Units='"+SourceUnits+"' and UeUnits='"+UeUnits+"'
	try
	{
		pCon=rsData->GetActiveConnection();
		pRs->Open(_variant_t(strSQL),(IDispatch*)pCon,adOpenStatic,adLockOptimistic,adCmdText);
		pRs->Filter=_variant_t("Units='"+SourceUnits+"' and UeUnits='"+UeUnits+"'");//pfg20050629
		if(pRs->GetRecordCount()>0)
		{
			x=vtof(pRs->GetCollect(_T("value")));
			dOffset=vtof(pRs->GetCollect(_T("offset")));//pfg20050701
			return x;
		}
		else if(pRs->GetRecordCount()==0)
		{
			//pfg20050629�������ӵ�λת����ϵ�Ի���(statr)
			CUnitsTransformDlg dlg;
			pRs->Filter="";
			dlg.m_pRsUnits=pRs;
			dlg.m_strDestinationUnits=UeUnits;
			dlg.m_strSourceUnits=SourceUnits;
			if((dlg.DoModal())==IDOK)
			{
				pRs=dlg.m_pRsUnits;//pfg20050630
				pRs->Filter=_variant_t("Units='"+SourceUnits+"' and UeUnits='"+UeUnits+"'");//pfg20050629
				if(pRs->GetRecordCount()>0)
				{
					dOffset=vtof(pRs->GetCollect("Offset"));//pfg20050701
					x=vtof(pRs->GetCollect(_T("value")));
					return x;
				}
				else
					return 1.0;
			}
			else
			{
				m_IsCaesarError=TRUE;//������һ������pfg20050627				
			}
			//pfg20050629�������ӵ�λת����ϵ�Ի���(end)
		}
	}
	catch(_com_error e)
	{
		if(AfxMessageBox("����:��ǰ��λ����ת��,�������λ������(��ϸ�Ķ�ʹ��˵��)!",MB_YESNO)==IDYES)
		{
			m_IsCaesarError=TRUE;//������һ������pfg20050627
		}
	}
	return 1.0;
}

BOOL CAESARIIToPHS::IsHanger( int iNode,const CString& strFileName ) //�Ƿ��ǵ���
{
	_RecordsetPtr pHangerRs;
	pHangerRs.CreateInstance( __uuidof(Recordset) );

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM OUTPUT_HANGERS WHERE [NODE] = %d AND [JOBNAME] = '%s'"),iNode,strFileName);
	pHangerRs->Open(_variant_t(strSQL),(IDispatch *)m_pConnSourceDB,adOpenDynamic,adLockOptimistic,adCmdText);
	if(pHangerRs->adoEOF && pHangerRs->BOF)
	{
		pHangerRs->Close();
		return FALSE;
	}
	pHangerRs->Close();
	return TRUE;
}

double CAESARIIToPHS::GetMaxLoad( int iNode,const CString& strFileName ) //�õ�֧�������ĺ���ֵ
{
	double dRtMaxLoad = 0.0;
	_RecordsetPtr pMaxLoadRs;
	pMaxLoadRs.CreateInstance( __uuidof(Recordset) );
	//���빤������(start)
	CString strSQL;
	strSQL.Format(_T("SELECT DISTINCT [NODE] ,max([RESULTANTF]) AS PJG FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' GROUP BY [NODE] "),iNode,strFileName);
	pMaxLoadRs->Open(_variant_t(strSQL),(IDispatch *)m_pConnSourceDB,adOpenDynamic,adLockOptimistic,adCmdText);
	//prsOutputRestraints = gpconCaesar->Execute(_bstr_t(strSQL),NULL,adCmdText);
	if(pMaxLoadRs->adoEOF && pMaxLoadRs->BOF)
	{
	}
	else
	{	
		dRtMaxLoad = vtof( pMaxLoadRs->GetCollect("PJG") );
	}
	if(pMaxLoadRs->State==adStateOpen)
	{
		pMaxLoadRs->Close();
	}
	return dRtMaxLoad;
}

//�õ����ɵĹ������غͰ�װ����
//û�о�����λת���ĺ���ֵ
BOOL CAESARIIToPHS::GetHangerLoad( int iNode,const CString& strFileName, double& dWorkLoad,double& dInsLoad ) //�õ����ɵĹ������غͰ�װ����
{
	_RecordsetPtr pHangerRs;
	pHangerRs.CreateInstance( __uuidof(Recordset) );
	
	try
	{
		CString strSQL;
		strSQL.Format(_T("SELECT * FROM OUTPUT_HANGERS WHERE [NODE] = %d AND [JOBNAME] = '%s'"),iNode,strFileName);
		pHangerRs->Open(_variant_t(strSQL),(IDispatch *)m_pConnSourceDB,adOpenDynamic,adLockOptimistic,adCmdText);
		if(pHangerRs->adoEOF && pHangerRs->BOF)
		{
			pHangerRs->Close();
			return FALSE;
		}
		else
		{
			dWorkLoad = vtoi ( pHangerRs->GetCollect( _T("NUMREQ") ) ) * vtof( pHangerRs->GetCollect( _T("HOT_LOAD") ) ); //��������
			dInsLoad = vtoi ( pHangerRs->GetCollect( _T("NUMREQ") ) ) * vtof( pHangerRs->GetCollect( _T("TH_INSTALL_LOAD") ) ); //��װ����
		}
	}
	catch ( _com_error e )
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
	if(pHangerRs->State==adStateOpen)
	{
		pHangerRs->Close();
	}
	return TRUE;
}

//���ݵ������͵õ����ɴ�����,���͹���ο�Spring.mdb���ݿ�
BOOL CAESARIIToPHS::GetHangerSeriesNum(const CString& strHangerType,int& iRtSeriesNum ) 
{
	
	int iLen = strHangerType.GetLength();
	if ( strHangerType.GetLength() < 2 )
		return FALSE;
	CString strSubType = strHangerType.Left(2);
	strSubType.MakeUpper();
	if ( strSubType == _T("ZH") ) //��糧��ˮ�ܵ�֧��������ֲᵯ��D-ZD83 ��ӦCAESARII�� 21������ ��ʽ("ZH" & SeriesNum & Format(SPRSpecNo,"00"))
	{
		if ( iLen < 3 )
			return FALSE;
		iRtSeriesNum = atoi( strHangerType.Mid( 2,1 ) ) ;
		return TRUE;
	}
	else if ( strSubType == _T("TH") ) //�����������Ժ��ˮ�ܵ�֧��������ֲᵯ��D-ZD83 ��ӦCAESARII�� 21������ ("TH1-A" & SeriesNum & Format(SPRSpecNo,"00"))
	{
		if ( iLen < 5 )
			return FALSE;
		iRtSeriesNum = atoi( strHangerType.Mid( 5,1 ) ) ;
		return TRUE;
	}
	else if ( strSubType == _T("VS") || strSubType == _T("TD") )  //����ʯ���豸�����ɳ�,�������ɳ�����,���ұ�׼/��е��ҵ��׼����GB10182-88
	{
		//"VS" & Format(SeriesNum*30,"00") & "D" & Format(SprNo,"00"),"TD" & (SeriesNum*30) & "D" & Format(SPRSpecNo,"00"),"TD" & (SeriesNum*30) & "D" & Format(SPRSpecNo,"00")
		int iTmpDisp = 0;
		int iEndIndex = strHangerType.Find( _T("D"),2 );
		if ( iEndIndex == -1 )
		{
			iTmpDisp = atoi( strHangerType.Mid( 2,iLen-2 ) );
		}
		else
		{
			iTmpDisp = atoi ( strHangerType.Mid( 2,iEndIndex-2 ) );
		}
		iRtSeriesNum = iTmpDisp / 30;
		return TRUE;
	}
	
	return FALSE;
}


