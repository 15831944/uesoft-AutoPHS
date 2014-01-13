#include "stdafx.h"
#include "user.h"
#include "ReadPSAResult.h"
#include <fstream.h>
#include <math.h>
#include "EDIBDB.h"
#include "phsdata.h"
#include "Frmtxsr.h"
#include "DlgFindAutoPSAFile.h"
#include "UnitsTransformDlg.h"


#include "frmstatus.h"
#include "modPHScal.h"
#include "EDIBgbl.h"
#include "basDirectory.h"
#include "InputBox.h"
#include "modRegistry.h"
#include "SelPSAProjectNameDlg.h"

#include "BestrowJSJDH.h"
#include "PSAPointElevation.h"
#include "Outfunction.h"

#include "CAESARIIToPHS.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
const _TCHAR* strSprNWEPDI;
const _TCHAR* strSprECEPDI;
const _TCHAR* strSprGB10182;
bool gbNewGLIFV31=false;

CString gstrUNIT_FORCE="N";//�ڲ�ʹ��N��λ
CString gstrUNIT_MOMENTAL="N.m";//�ڲ�ʹ��N.m��λ

CString YLJSVer="";
int IsCaesarError;//pfg20050627
#pragma warning (disable : 4244)
long GetFileNameId(CString SourceDataFileName,IDispatch *pConID);
void Delete1();
//void ElevationFileToList(CString FileName,ElevationList *pElevationList,int ICC);
//д֧�������ݵ�PHS(֧����������)��
//extern "C" __declspec(dllimport) int WritePHSDataToTable(_ConnectionPtr pRefInfoCon,_Recordset *rsResult,CString strDataFileName,long imaxZdjh,long iFileNameID,long iSelVlmID,double dbRatioOfPjg2Pgz);

typedef CList<CNodeUpElevation *,CNodeUpElevation *> ElevationList;


CString Mid(CString& strTmp,int nStart,int nCount)
{
	if(nStart>= strTmp.GetLength())
		return CString(_T(""));
	else
	{
		if(nCount==-1)
			return strTmp.Mid(nStart);
		else 
			return strTmp.Mid(nStart,nCount);
	}
}

//�����ַ������Ӵ���(�Կո�������ַ���);
CString GetStrAt1(CString strText,int i)
{
	int start=0,end=0;
	CString str;
	str.Empty();
	strText.TrimLeft();
	strText.TrimRight();
	strText = strText+" ";
	for(int index=1;index<=i-1;index++)
	{
		start=strText.Find(' ',0);
		if(start != -1)
			strText = strText.Mid(start);
		else 
			return "";
		strText.TrimLeft();
	}
	end=strText.Find(' ',0);
	if(end !=-1)
	{
		str=strText.Left(end);
	}
	str.TrimLeft();
	str.TrimRight();
	return str;
}


struct FileJSDBH
	{
		int JSDBH;
        struct FileJSDBH *NextNode;
	};
//����ȡ�Ľڵ�Ŵ��������У����ļ��еĽڵ�����ظ�ʱ�������д�ŵĽڵ����ظ���//���ߣ� chengbailian
void SaveFileJSDBH(FileJSDBH **Headpos,FileJSDBH **Noncepos,FileJSDBH **Nextpos,int iCalNodeNumber)
{
	FileJSDBH *Movepos=NULL;
	Movepos=*Headpos;
	while(Movepos!=NULL)
	{
		if(Movepos->JSDBH==iCalNodeNumber)
				break;
		Movepos=Movepos->NextNode;
	}
     if(Movepos==NULL)
	 {
		*Nextpos=(FileJSDBH *)malloc(sizeof(FileJSDBH));
		(*Nextpos)->JSDBH=iCalNodeNumber;
		(*Nextpos)->NextNode=NULL;
		if(*Headpos==NULL)
			*Headpos=*Nextpos;
		else
		    (*Noncepos)->NextNode=*Nextpos;
			*Noncepos=*Nextpos;
	 }

}
////��Ҫ��ȡ�ļ��Ľڵ�ţ�//���ߣ� chengbailian
void PickFileJSDBH(CString FileName,FileJSDBH **Headpos,FileJSDBH **Noncepos,FileJSDBH **Nextpos,CString YLJSVer)
{
	ifstream f;
	CString temp,strTemp;
	int iCalNodeNumber,fdFlg;
    
	f.open(FileName);//SourceDataFileName
	//�����ļ����Ͳ�ͬ���ò�ͬ������ȡ�ļ��еĽڵ��
	if(YLJSVer=="GLIF1.2")  ///GLIFӦ����������Ϊ��̬������㵯��
	{
		while(!f.eof())
		{
			 f.getline(strTemp.GetBuffer(255),255);
			 strTemp.ReleaseBuffer();
			 if( strTemp.Find("SUMMARY FOR RESTRAINTS") !=-1 ) //�ҵ���SUMMARY FOR RESTRAINTS��
				 break;
		}
						
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			if( atoi(Mid(temp,4, 3)) > 0 )
			{
				iCalNodeNumber = atoi(Mid(temp,0, 7));
              
				//�ѽڵ�ŷ��������У�ÿһ��֧��Ҫ������
                   SaveFileJSDBH(Headpos,Noncepos,Nextpos,iCalNodeNumber);
                //
				f.getline(temp.GetBuffer(255),255);
			    temp.ReleaseBuffer();  //����1����
			}				  
			else
				if(temp.Find("COLD SPRINGS TABLE") !=-1)
					break;
		}
		f.close();
	}
	else if(YLJSVer=="GLIF3.x"||YLJSVer.Left(7)=="AutoPSA")
	{
		//ReadResult_GLIF31New(rs1, DataFileName,maxZdjh,FileNameID1);
		//GLIFӦ����������Ϊ��̬������㵯��
		if(gbNewGLIFV31)
		{
			while(!f.eof())
			{
				f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
				if(strTemp.Find("CW-DISPLACEMENT (cold/work-status)") !=-1 )//�ҵ�"CW-DISPLACEMENT (cold/work-status)"
					break;
			}
			fdFlg=0;
			while(!f.eof())
			{
				f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			    //���ϵ���Ժ1999��5��ǰʹ�õ�GLIFV3.x�汾��iCalNodeNumber���ַ���ȴӵ�1-10λ
			    //���ݵ���Ժ2001��5��ǰʹ�õ�GLIFV3.1�汾��iCalNodeNumber���ַ���ȴӵ�1-5λ��
				if( atof(Mid(temp, 0, 5)) >0 )
				{
					fdFlg=1;
					if( gbNewGLIFV31 )
						iCalNodeNumber = atoi(Mid(temp,0, 5)) ;     //�ڵ��
					else
						iCalNodeNumber = atoi(Mid(temp, 0, 10));     //�ڵ��

                      //�ѽڵ�ŷ��������У�ÿһ��֧��Ҫ������
				      SaveFileJSDBH(Headpos,Noncepos,Nextpos,iCalNodeNumber);
				}
				else
				{
					if( fdFlg==1)
					{
						do
						{
							if(temp.Find("CW-DISPLACEMENT (cold/work-status)") != -1 )
								break;
							f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
						}while(!f.eof());
						fdFlg=0;
					}
				}
			}
			f.close();
			f.open(FileName);
			//GLIFV3.x:���Һ�������ͷ
			while(!f.eof())
			{
				f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
				if( strTemp.Find("STRUCTURE LOAD OF RESTRAINTS") !=-1 ) //�ҵ�"STRUCTURE LOAD OF RESTRAINTS"
					break;
			}
                     
		//�ҵ�"STRUCTURE LOAD OF RESTRAINTS"
		//GLIFV3.x:��ʼ����������
		fdFlg=0;
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			if( atof(Mid(temp,0, (gbNewGLIFV31 ? 5 : 16))) > 0 )
			{
				fdFlg=1;
				if( gbNewGLIFV31 )
					iCalNodeNumber = atoi(Mid(temp, 0, 5));      //�ڵ��
				else
					iCalNodeNumber = atoi(Mid(temp, 0, 16)); //�ڵ��
				SaveFileJSDBH(Headpos,Noncepos,Nextpos,iCalNodeNumber);
			}
			else
			{
				if( fdFlg==1)
				{
					do
					{
						if(temp.Find("STRUCTURE LOAD OF RESTRAINTS") != -1 )
							break;
						f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
					}while(!f.eof());
					fdFlg=0;
				}
			}
		}
		f.close();
		}else
		{

				//ReadResult_GLIF31(rs1, DataFileName,maxZdjh,FileNameID1);
			while(!f.eof())
			{
				f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
					if(strTemp.Find("CW-DISPLACEMENT (cold/work-status)") !=-1 ) //�ҵ�"CW-DISPLACEMENT (cold/work-status)"
						break;
			}
   
			//�ҵ�"CW-DISPLACEMENT (cold/work-status)"
			//GLIFV3.x:��ʼ��λ������
			while(!f.eof())
			{
				f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
				//���ϵ���Ժ1999��5��ǰʹ�õ�GLIFV3.x�汾��iPreINT(1)���ַ���ȴӵ�1-10λ
				//���ݵ���Ժ2001��5��ǰʹ�õ�GLIFV3.1�汾��iPreINT(1)���ַ���ȴӵ�1-5λ��
				//iPreINT(2)���ַ���ȴӵ�6-10λ��
				if( atof(Mid(temp, 0, 10)) >0)
				{
					if( gbNewGLIFV31 )
						iCalNodeNumber = atoi(Mid(temp,0, 5)) ;     //�ڵ��
					else
						iCalNodeNumber = atoi(Mid(temp, 0, 10));     //�ڵ��
					SaveFileJSDBH(Headpos,Noncepos,Nextpos,iCalNodeNumber);
				}
				else
				{
					if( temp.Find("CW-DISPLACEMENT (cold/work-status)") ==-1 )
						break;
				}
			}
			f.close();
			f.open(FileName);
		      //GLIFV3.x:���Һ�������ͷ
			while(!f.eof())
			{
				f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
				if( strTemp.Find("STRUCTURE LOAD OF RESTRAINTS") !=-1 ) //�ҵ�"STRUCTURE LOAD OF RESTRAINTS"
					break;
			}
                     
		//�ҵ�"STRUCTURE LOAD OF RESTRAINTS"
		//GLIFV3.x:��ʼ��������
			while(!f.eof())
			{
				f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			    if( atof(Mid(temp,0, (gbNewGLIFV31 ? 5 : 16))) > 0 )
				{
					if( gbNewGLIFV31 )
						iCalNodeNumber= atoi(Mid(temp, 0, 5));      //�ڵ��
					else
						iCalNodeNumber = atoi(Mid(temp, 0, 16));  //�ڵ��	
					SaveFileJSDBH(Headpos,Noncepos,Nextpos,iCalNodeNumber);
				}else
					if( temp.Find("STRUCTURE LOAD OF RESTRAINTS") ==-1 )
						break;
			}
			f.close();
		}
		}
		else if(YLJSVer=="ZHDYF3.0")
		{
			//ReadResult_ZHDYF30(rs1, DataFileName,maxZdjh,FileNameID1);
			while(!f.eof())
			{
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				if(temp=="")		//��������
					continue; 
				if( atoi(Mid(temp,0, 4)) > 0 )  
				{
					iCalNodeNumber = atoi(Mid(temp,0, 4));
			        SaveFileJSDBH(Headpos,Noncepos,Nextpos,iCalNodeNumber);
				}
			//��һ���ǹܾ����¶ȡ���ߵ�����
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
			//��һ����Լ�����͡���װ���ء��������ء��ṹ���ص�����
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
		
			//������3������
			//��1�У���λ��(x,y,z),��λ��(x,y,z),
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();

			//��2�У���̬��(x,y,z),��̬����(x,y,z),��̬��(x,y,z),��̬����(x,y,z)
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
			
			//��3�У�������(x,y,z),��������(x,y,z),żȻ��(x,y,z),żȻ����(x,y,z)
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();			
			}
			f.close();
		}
		else if(YLJSVer=="XNP 2.0")
		{
					//����ԺӦ����������Ϊ��̬������㵯��
					//ReadResult_SWEDPSA(rs1, DataFileName,maxZdjh,FileNameID1);
			while(!f.eof())
			{       
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
					if(temp.Find("JIE GOU SUPPORT LOAD")!=-1 || temp.Find("WORKING SUPPORT LOAD")!=-1)
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
                
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();

						do
						{
							temp.TrimLeft();temp.TrimRight();
							PhsInfo tmpPhsInfo;
							iCalNodeNumber = GetParam(temp)*10.0f;
							SaveFileJSDBH(Headpos,Noncepos,Nextpos,iCalNodeNumber);
							f.getline(temp.GetBuffer(255),255);
							temp.ReleaseBuffer();
							temp.TrimLeft();

						}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) &&!f.eof());

					}
			}
			f.close();
		}
}


BestrowJSJDH JSDBH_Dlg;
//�õ����п��ܱ����ǵ��ļ���   //add by chengbailian
CString GetOldFileName(CString NewFileName,IDispatch* pCon,long SelVlmID,CString YLJSVer)
{
    int JSDBHNumer,JSDBHNumer1,NewJSDBHNumer=0,OldJSDBHNumer;
	_RecordsetPtr rs,ZaRs,FileRs,ZaRsCount;
	CString Sql,CSql;
	CString EndFileName=NewFileName;
    FileJSDBH *NewFileHeadpos,*NewFileNoncepos,*NewFileNextpos,*NewFileMovepos;
		     // *OldFileHeadpos,*OldFileNoncepos,*OldFileNextpos,*OldFileMovepos;
    NewFileHeadpos=NewFileNoncepos=NewFileNextpos=NewFileMovepos=NULL;

	rs.CreateInstance(__uuidof(Recordset));
	FileRs.CreateInstance(__uuidof(Recordset));
	ZaRs.CreateInstance(__uuidof(Recordset));
	ZaRsCount.CreateInstance(__uuidof(Recordset));
		//OldFileHeadpos=OldFileNoncepos=OldFileNextpos=OldFileMovepos=NULL;
	try
	{
		

		/*
		Sql="SELECT * FROM ZY WHERE FileName='"+NewFileName+"' AND VolumeID="+ltos(SelVlmID);
		rs->Open(_variant_t(Sql),pCon,adOpenDynamic,adLockOptimistic,adCmdText);
		if(!rs->adoEOF || !rs->BOF)
		{
			rs->Close();
			return NewFileName;
		}
		rs->Close();*/
		
		Sql="SELECT count(*) FROM ZY WHERE FileName='"+NewFileName+"' AND VolumeID="+ltos(SelVlmID);
		rs->Open(_variant_t(Sql),pCon,adOpenDynamic,adLockOptimistic,adCmdText);
		//����ȡ�ļ��Ƿ�����Ҫ��ȡ���ļ�
		if(long(rs->GetCollect(_variant_t(long(0))))>0)
		{
			rs->Close();
			return NewFileName;
		}
		rs->Close();
		
		Sql="SELECT count(*) FROM zy WHERE VolumeID="+ltos(SelVlmID);	
		rs->Open(_variant_t(Sql),pCon,adOpenDynamic,adLockOptimistic,adCmdText);
	///	rs->Requery(-1);
		//�Ƿ�����ͬ�ľ����ļ�
		if(long(rs->GetCollect(_variant_t(long(0))))<=0)
		{ 
		   rs->Close();
		   return NewFileName;
		}
        rs->Close();
        
		PickFileJSDBH(NewFileName,&NewFileHeadpos,&NewFileNoncepos,&NewFileNextpos,YLJSVer);
		
		NewFileMovepos=NewFileHeadpos;
		
		//�����Ҫ��ȡ�ļ��нڵ�Ÿ���NewJSDBHNumer
		while(NewFileMovepos!=NULL)
		{
			NewJSDBHNumer++;
			NewFileMovepos=NewFileMovepos->NextNode;
		}
		
		JSDBHNumer=-1;
		
		Sql="SELECT * FROM zy WHERE VolumeID="+ltos(SelVlmID);
		FileRs->Open(_variant_t(Sql),pCon,adOpenDynamic,adLockOptimistic,adCmdText);
		
		while(!FileRs->adoEOF)
		{
			int FileNameId=long(FileRs->GetCollect("FileNameID"));
			CString FileName=vtos(FileRs->GetCollect("FileName"));
			 //JSDBH_Dlg.DoModal();
			//JSDBH_Dlg.m_list.AddString(FileName);
			Sql="SELECT * FROM za WHERE FileNameID="+ltos(FileNameId);
			CSql="SELECT count(*) FROM za WHERE FileNameID="+ltos(FileNameId);
			//ZaRs=ConDB->Execute(_bstr_t(Sql),NULL,adCmdText);
			
			ZaRsCount->Open(_variant_t(CSql),pCon,adOpenDynamic,adLockOptimistic,adCmdText);
			OldJSDBHNumer=long(ZaRsCount->GetCollect(_variant_t(long(0))));
            ZaRsCount->Close();

			ZaRs->Open(_variant_t(Sql),pCon,adOpenDynamic,adLockOptimistic,adCmdText);    
			JSDBHNumer1=0;
			NewFileMovepos=NewFileHeadpos;
			while(NewFileMovepos!=NULL)
			{
				ZaRs->Filter=_variant_t("JSDBH1="+ltos(NewFileMovepos->JSDBH));
				if(!ZaRs->adoEOF || !ZaRs->BOF)
					JSDBHNumer1++;	
				NewFileMovepos=NewFileMovepos->NextNode;
			}
			ZaRs->Close();
			JSDBHNumer1=(OldJSDBHNumer+NewJSDBHNumer)-2*JSDBHNumer1;

			if(JSDBHNumer1<JSDBHNumer || JSDBHNumer==-1)
			{
				JSDBHNumer=JSDBHNumer1;
				EndFileName=FileName;
			}
			FileRs->MoveNext();
			/*SaveFileJSDBH(FileName,&OldFileHeadpos,&OldFileNoncepos,&OldFileNextpos);
			if(OldFileHeadpos==NULL)
			{   rs->MoveNext();
				continue;
			}
			JSDBHNumer1=0;
			NewFileMovepos=NewFileHeadpos;
			while(NewFileMovepos!=NULL)
			{
				
				OldFileMovepos=OldFileHeadpos;
				while(OldFileMovepos!=NULL)
				{
					if(NewFileMovepos->JSDBH==OldFileMovepos->JSDBH)
							JSDBHNumer1++;
					OldFileMovepos=OldFileMovepos->NextNode;
				}
				NewFileMovepos=NewFileMovepos->NextNode;
			}
			if(JSDBHNumer1>JSDBHNumer)
			{
				JSDBHNumer=JSDBHNumer1;
				EndFileName=FileName;
			}
			OldFileMovepos=OldFileHeadpos;
			while(OldFileMovepos!=NULL)
			{
				OldFileHeadpos=OldFileMovepos;
				OldFileMovepos=OldFileMovepos->NextNode;
				free(OldFileHeadpos);
			}
			OldFileHeadpos=OldFileNoncepos=OldFileNextpos=NULL;
			
			rs->MoveNext();
			*/
		}
		//ጷŃȴ�
        NewFileMovepos=NewFileHeadpos;
		while(NewFileMovepos!=NULL)
		{
			NewFileHeadpos=NewFileMovepos;
			NewFileMovepos=NewFileMovepos->NextNode;
			free(NewFileHeadpos);
		}
		FileRs->Close();
}
	catch(CException *e)
	{
		e->Delete();
	}
		catch(...)
		{
		
			if(rs->State!=adStateClosed)
				rs->Close();
			if(FileRs->State!=adStateClosed)
				FileRs->Close();
			if(ZaRs->State!=adStateClosed)
				ZaRs->Close();
		}
		
    return EndFileName;


}


extern CString JSJDH_Dlg_FileName;
extern IDispatch* JSJDH_Dlg_pCon;
void ReadYljsResult(_Recordset* rs)
{
	//RemAuthor:
	//����:
	//Ŀ��:��ȡ�ܵ�Ӧ����������
	//����:
	//����: 
	AfxGetApp()->BeginWaitCursor();
	_ConnectionPtr ConnDB;
	ConnDB.CreateInstance(__uuidof(Connection));
	ConnDB=rs->GetActiveConnection();
	
	try
	{ 
		long X=0;
		CString DataFileName;
		CString strTemp;
		//��������ԺӦ�������ӿ��ļ�cad8.dat
		CString strFileNameCAD8;
		bool  bExit=false; //�˳���־
        CString strAutoPSAVer;
		//FrmPhsData.ShowWindow(SW_SHOWNORMAL);
		ifstream f;
		//"��Ҫ�滻" + EDIBgbl::strSelSpec + EDIBgbl::SelJcdm + "���е�֧����ԭʼ������"
		strTemp.Format(GetResStr(IDS_ReplacePSADataInSpecVolume),EDIBgbl::SelSpecName,EDIBgbl::SelJcdm);

		if(ShowMessage(strTemp, MB_YESNO|MB_ICONQUESTION, GetResStr(IDS_AutoImportPSAData)) ==IDYES)
		{
            IDispatch* pCon=rs->GetActiveConnection();
			_RecordsetPtr RD;
			RD.CreateInstance(__uuidof(Recordset));
			CDlgFindAutoPSAFile dlg;
			int Flg=1;
			while(Flg == 1)
			{
				 if( IDOK != dlg.DoModal() )
				 {
					return;
				 }
				 //���ѡ����һ��AutoPSA�򿪵��ļ���,ͬʱ���ļ�����,���ø��ļ���λ.
				 if( dlg.m_bFlag==1/*��ѡ����һ���ļ���.*/ && !modPHScal::gsPSAOutFilePath.IsEmpty() && FileExists(modPHScal::gsPSAOutFilePath) )
				 {
					DataFileName = user::SelFileDwg(GetResStr(IDS_PSAOutputFileList),modPHScal::gsPSAOutFilePath,modPHScal::g_iPSAFilterIndex);
				 }
				 else
				 {
					 //����,�����һ�ε�����ļ���λ.���û��,����ģ���ļ���·����λ.

					CString StrSql="SELECT * FROM ZY WHERE VolumeID="+ltos(EDIBgbl::SelVlmID);
					RD->Open(_variant_t(StrSql),pCon,adOpenDynamic,adLockOptimistic,adCmdText);
					if(RD->adoEOF)
						DataFileName = user::SelFileDwg(GetResStr(IDS_PSAOutputFileList),modPHScal::gsPSAOutFilePath,modPHScal::g_iPSAFilterIndex);
					else
					{
						CString StrFileNamePath;
						RD->MoveLast();
						StrFileNamePath=vtos(RD->GetCollect("FileName"));
						StrFileNamePath=GetFilePath(StrFileNamePath);
						DataFileName = user::SelFileDwg(GetResStr(IDS_PSAOutputFileList),StrFileNamePath,modPHScal::g_iPSAFilterIndex);
					}
					RD->Close();
				 }
				 if(DataFileName != "" && FileExists(DataFileName) || dlg.m_bFlag == 0/*û�е����ɹ�ѡ���AutoPSA�ļ���ʱ,����ѡ�����ļ�,��ûѡ��.����ѭ�� */) 
					 Flg=0;
			}
				//modPHScal::gsPSAOutFilePath=GetFilePath(DataFileName);
                 //GetOldFileName(DataFileName,pCon,EDIBgbl::SelVlmID);
			SetRegValue(_T("Settings"),"PSAOutFilePath",modPHScal::gsPSAOutFilePath);
			SetRegValue(_T("Settings"),"PSAOutFilterIndex",modPHScal::g_iPSAFilterIndex);
			if(DataFileName == "" || !FileExists(DataFileName)) return;
//add		
			long maxZdjh=FrmTxsr.m_pViewTxsr->GetMaxZdjh();

			
			_variant_t SQLx=rs->GetSource();
		    Delete1();
			UpDataZY();
			if(rs->State!=adStateClosed)
			{
				if(!rs->adoEOF && !rs->BOF)
				{
					try
					{
						rs->Update();
					}
					catch(...)
					{
						rs->CancelUpdate();
					}
				}
				try
				{
					rs->Close();
				}
				catch(...)
				{
				}
					
			}
			::FrmPhsData.m_DBGbill.SetRefDataSource(NULL);
//			FrmTxsr.m_pViewTxsr->m_Databill.SetRefRecordset(NULL);
			//rs->Close();�˾������������� ***�������������¡�����رպ���ִ�����������������
			_RecordsetPtr rs1;
			rs1.CreateInstance(__uuidof(Recordset));
			rs1->Open(SQLx,pCon,adOpenKeyset,adLockOptimistic,adCmdText);
			
			CString temp;
			temp = DataFileName.Right(3);
			temp.MakeLower();
			strFileNameCAD8 = DataFileName.Right(8);
			strFileNameCAD8.MakeLower();

//�ж���CAESARII�����Ľ������GLIF�����Ľ��
			if ( temp == "mdb" )   //��mdb���ݿ�
			{
				try
				{
					CString strTableName = "OUTPUT_GLOBAL_ELEMENT_FORCES";
					CString connTempString="Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s";
					CString connString;
					connString.Format(connTempString,DataFileName,"");
					_ConnectionPtr m_pConn;
					m_pConn.CreateInstance(__uuidof(Connection));
					m_pConn->Open(_bstr_t(connString),"","",0);
					if (!EDIBgbl::tdfExists(m_pConn,strTableName))//��һ���ж��Ƿ���ָ���ı�
					{
						ShowMessage(GetResStr(IDS_PSANameVer)+" Unknown");	
						m_pConn->Close();
						m_pConn = NULL;
					}
					else
					{	
						//����CAESARII�����ݷ������
						_RecordsetPtr m_prsVersion;  //�汾��¼
                        CString  Version;
						 //�жϱ�"version"�Ƿ����pfg20050602
						//���û�� version=4.2
						if(EDIBgbl::tdfExists(m_pConn,"Version"))
						{
							
						//pfg20050602

							_bstr_t  VersionSQL("SELECT VERSION FROM VERSION");
							_variant_t Hoder;

							m_prsVersion=m_pConn->Execute(VersionSQL,NULL,adCmdText);						
							Hoder=m_prsVersion->GetCollect("Version");
							Version.Format("%s",vtos(Hoder));
							Version.TrimLeft();
							Version.TrimRight();
						}
						else//pfg20050602 start
						{
							Version=_T("4.20");
						}//pfg20050602 end
						m_pConn->Close();
						m_pConn = NULL;
						CString  m_version;
						m_version.Format("CAESARII %s",Version);
						ShowMessage(GetResStr(IDS_PSANameVer)+m_version);

						CAESARIIToPHS mCaesarIIToPHS;
						if (Version=="4.50")
						{  
						  long FileNameID1=GetFileNameId(DataFileName,pCon);  //���ݿ��ļ�û�������ļ����޸ĺ��Զ���������ȡ��¼���ܣ�
   						  mCaesarIIToPHS.ReadResult_CAESARII45(rs1,DataFileName,maxZdjh,FileNameID1,"CASEARII 4.50");
						  //CAESARIIӦ����������Ϊ��̬������㵯��
						}
						else if (Version=="4.20")
						{						                      
						  long FileNameID1=GetFileNameId(DataFileName,pCon);  //���ݿ��ļ�û�������ļ����޸ĺ��Զ���������ȡ��¼���ܣ�
   						  //ReadResult_CAESARII42(rs1,DataFileName,maxZdjh,FileNameID1);//pfg20050624ԭ����
   						  mCaesarIIToPHS.ReadResult_CAESARII45(rs1,DataFileName,maxZdjh,FileNameID1,"CASEARII 4.20");//pfg20050624
						  //CAESARIIӦ����������Ϊ��̬������㵯��
						}
					    else
						{
						  long FileNameID1=GetFileNameId(DataFileName,pCon);  //���ݿ��ļ�û�������ļ����޸ĺ��Զ���������ȡ��¼���ܣ�
   						  //ReadResult_CAESARII42(rs1,DataFileName,maxZdjh,FileNameID1);//pfg20050624ԭ����
   						  mCaesarIIToPHS.ReadResult_CAESARII45(rs1,DataFileName,maxZdjh,FileNameID1,"CASEARII 4.20");//pfg20050624
						  //CAESARIIӦ����������Ϊ��̬������㵯��
						}
										
						
						gbHotStatus2Zero=0;
						::SetRegValue(_T("Settings"),"HotStatus2Zero",(gbHotStatus2Zero));
					}
				}
				catch(_com_error & e)
				{
					AfxMessageBox(e.Description());
				}
			}
			else			 
			{ 
				if(strFileNameCAD8=="cad8.dat")
				{
					//���������������ԺӦ����������ZHDYF�����ݷ������
					ShowMessage(GetResStr(IDS_PSANameVer)+" ZHDYF3.0");
					YLJSVer = "ZHDYF3.0";
				}
				else
				{
					//����GLIF�����ݷ������
					X = 1;
					f.open(DataFileName);
					while((!f.eof() || X<=30) && !bExit)
					{
						f.getline(strTemp.GetBuffer(255),255);
						strTemp.ReleaseBuffer();
						strTemp.TrimLeft();strTemp.TrimRight();
						strTemp.MakeUpper();
						if ( strTemp.Find("#$ VERSION") != -1)
						{
							f.getline(strTemp.GetBuffer(255),255);
							strTemp.ReleaseBuffer();
							strTemp.TrimLeft();strTemp.TrimRight();
							
							YLJSVer = "AutoPSA" + GetStrAt1(strTemp,1);
							ShowMessage(GetResStr(IDS_PSANameVer)+YLJSVer);
							break;
							
							
						}
						else if(strTemp.Find("GLIF---V/1.2")!=-1)
						{
							ShowMessage(GetResStr(IDS_PSANameVer)+" GLIFV1.2");
							YLJSVer = "GLIF1.2";
							break;
						}
						else if(strTemp.Find("GLIF---V/3.")!=-1)
						{
							ShowMessage(GetResStr(IDS_PSANameVer)+" GLIFV3.x");
							YLJSVer = "GLIF3.x";
							while(!f.eof() || X <= 70)
							{
								f.getline(strTemp.GetBuffer(255),255);
								strTemp.ReleaseBuffer();
								if(strTemp.Left(5) == " ++++ ")
								{
									//���ϵ���Ժ1999��5��ǰʹ�õ�GLIFV3.1�汾��Ӧ���ǽϾɵİ汾
									gbNewGLIFV31 = false;
									bExit = true;
									break;
								}
								else
								{
									if(strTemp.Left(3)==" + ")
									{
										//����Ժ2001��5��ǰʹ�õ�GLIFV3.1�汾���Լ�����Ժʹ�õ���νGLIFV3.2,Ӧ���ǽ��µİ汾
										gbNewGLIFV31 = true;
										bExit = true;
										break;
									}
										//δ��������
									//End If
								}
								X ++;
							}
							break;
						}
						else if(strTemp.Find("UESOFT AUTOPSA")!=-1 || strTemp.Find("UESOFT-AUTOPSA")!=-1)
						{
							strAutoPSAVer=strTemp.Mid(strTemp.Find("UESOFT AUTOPSA")+14,3);
							ShowMessage(GetResStr(IDS_PSANameVer)+" AutoPSA"+strAutoPSAVer);
							YLJSVer = "UESOFT AutoPSA"+strAutoPSAVer;
							while(!f.eof() || X <= 70)
							{
								f.getline(strTemp.GetBuffer(255),255);
								strTemp.ReleaseBuffer();
								if(strTemp.Left(3)==" + ")
								{
									//AutoPSA1.0ʹ��GLIFv3.1���ݸ�ʽ
									gbNewGLIFV31 = true;
									bExit = true;
									break;
								}
								X ++;
							}
							break;
						}
						else if (strTemp.Find("NGZ=")!=-1&&strTemp.Find("NGW=")!=-1)
						{

							ShowMessage(GetResStr(IDS_PSANameVer)+" XNP 2.0");
							YLJSVer = "XNP 2.0";
							break;
						}
						else
						{
							if(X >= 30)
							{
								AfxMessageBox(GetResStr(IDS_PSANameVer)+" Unknown");
								YLJSVer = "";
								break;
							}
						}
						//MsgBox X & strTemp
						X++;
					}
					f.close();
					//����GLIF����
				}
      
/*				frmStatus.ShowWindow(SW_SHOW);
				frmStatus.m_Label1= "���ļ� " + DataFileName;
				frmStatus.m_Label2= "�����ݿ� " + EDIBgbl::dbPRJDB.GetName() + "�еı�" + EDIBgbl::TBNRawData;
				frmStatus.SetWindowText( "����ת��");
				frmStatus.UpdateData(false);
				frmStatus.UpdateStatus(0,true);
				*/
				 //���Ѿ���ȡ�˵��ļ����бȽϣ�ȷ���Ƿ�Ҫ��ʾ�Ի���
            JSJDH_Dlg_pCon=pCon;
			JSJDH_Dlg_FileName=GetOldFileName(DataFileName,pCon,EDIBgbl::SelVlmID,YLJSVer);
			CString m_listStr;
			long FileNameID1;
			if(JSJDH_Dlg_FileName!=DataFileName)
			{
				BestrowJSJDH::WriteFileName=DataFileName;
				if(JSDBH_Dlg.DoModal()==IDOK && JSJDH_Dlg_FileName!=" ")  
					FileNameID1=GetFileNameId(JSJDH_Dlg_FileName,pCon);
				else
					FileNameID1=GetFileNameId(DataFileName,pCon);
			}
			else
				FileNameID1=GetFileNameId(DataFileName,pCon);  

				frmStatus.m_Label1 = "���ڵ����ļ�:";	//��ʼ����������ʾ����.	  by	zsy 2005.3.1
				frmStatus.m_Label2 = DataFileName;
				frmStatus.SetWindowText( "����Ӧ����������" );
				frmStatus.UpdateData(false);

  
				if( YLJSVer == "AutoPSA7.0" || YLJSVer == "AutoPSA8.0")
				{
					gbHotStatus2Zero=true;
					::SetRegValue(_T("Settings"),"HotStatus2Zero",(gbHotStatus2Zero));
					ReadPSAData(rs1,DataFileName,maxZdjh,FileNameID1);
				}
				else if(YLJSVer=="GLIF1.2")
				{
						//GLIFӦ����������Ϊ��̬������㵯��
						gbHotStatus2Zero=true;
						::SetRegValue(_T("Settings"),"HotStatus2Zero",(gbHotStatus2Zero));
						ReadResult_GLIF12(rs1, DataFileName,maxZdjh,FileNameID1);
				}
				else if(YLJSVer=="GLIF3.x"||YLJSVer.Find("UESOFT AutoPSA") != -1)
				{
						//GLIFӦ����������Ϊ��̬������㵯��
						gbHotStatus2Zero=true;
						::SetRegValue(_T("Settings"),"HotStatus2Zero",(gbHotStatus2Zero));
						if(gbNewGLIFV31)
							ReadResult_GLIF31New(rs1, DataFileName,maxZdjh,FileNameID1);
						else
							ReadResult_GLIF31(rs1, DataFileName,maxZdjh,FileNameID1);
				}
				else if(YLJSVer=="ZHDYF3.0")
				{
						//����ԺӦ����������Ϊ��̬������㵯��
						gbHotStatus2Zero=0;
						::SetRegValue(_T("Settings"),"HotStatus2Zero",(gbHotStatus2Zero));
						ReadResult_ZHDYF30(rs1, DataFileName,maxZdjh,FileNameID1);
				}
				else if(YLJSVer=="XNP 2.0")
				{
					//����ԺӦ����������Ϊ��̬������㵯��
					gbHotStatus2Zero=true;
					::SetRegValue(_T("Settings"),"HotStatus2Zero",(gbHotStatus2Zero));
					ReadResult_SWEDPSA(rs1, DataFileName,maxZdjh,FileNameID1);
				}
			}
/*
			try
			{
				rs1->Update();
			}
			catch(...)
			{
				rs1->CancelUpdate();
			}
*/
			rs1->Close();
			rs1=NULL;
			//********ע���¾䲻���ƶ���������ͷ,��������
			//rs->Close();
			rs->Open(SQLx,pCon,adOpenKeyset,adLockOptimistic,adCmdText);
			FrmTxsr.m_pViewTxsr->m_bAllowUpd=false;
			FrmTxsr.m_pViewTxsr->m_Databill.SetRefRecordset(rs);
			FrmTxsr.m_pViewTxsr->m_Databill.SetEnabled(TRUE);
			FrmPhsData.m_DBGbill.SetRefDataSource(rs);
			EDIBDB::SetColumnsProperty(FrmPhsData.m_DBGbill, EDIBgbl::SelBillType);
			FrmTxsr.m_pViewTxsr->m_bAllowUpd=true;
	
		}
	}
    
	catch(_com_error e)
	{
		ShowMessage(e.Description());
		FrmTxsr.m_pViewTxsr->m_bAllowUpd=true;
	}
	
	frmStatus.UpdateStatus(1,true);
	frmStatus.ShowWindow(SW_HIDE);
	AfxGetApp()->EndWaitCursor();
}

//��һ���Կո�Ϊ��������ַ�����ȡ��һ��doubleֵ,��
double GetNextValue(CString &str)
{
	str.TrimLeft();

	if(str.IsEmpty())
	{
		return 0.0;
	}

	int nFind = str.Find(_T(" "));
	double dbVal = atof(Mid(str, 0, nFind));

	if(nFind < 0)
	{
		str.Empty();
	}
	else
	{
		str = str.Mid(nFind);
	}
	
	return dbVal;
}

void ReadResult_ZHDYF30(_Recordset* rsResult ,CString SourceDataFileName,long maxZdjh,long FileNameID)
{
   //Remauther:
   //����:
   //Ŀ��:������Ϊ����������Ժ��ϵ�Ԫ��Ӧ����������ZHDYF���ɵ�֧���ܽӿ������ļ�cad8.dat�ļ�����������msgbox��������Գ������ȷ��
   //����:rsResult���ݱ��ڲ���λ:λ��mm,����kgf.
   //����:
	try
	{
		float phsVAL[13][13];//������������ĵ��������ݣ�Xά�����������ݺ�����У�//Yά������
		int iPreINT[3];//����������¼ǰ�������������
		long ltemp=maxZdjh;			 
		_Recordset* rsData;
		_RecordsetPtr tmprs;
		tmprs.CreateInstance(__uuidof(Recordset));

		_variant_t varTmp;
		CString strOldPhsSPRINGSel;
		CString strTemp, temp ;int iTemp=0;
		CString strSQL;		
		CString strSprTbn;

		const char* strPIPING_STRESS_OUTUNIT = "UNIT OF OUTPUTDATA IS";
		const char* strSUMMARY_FOR_RESTRAINTS = "SUMMARY FOR RESTRAINTS";
		const char* strCOLD_SPRING_TABLE = "COLD SPRINGS TABLE";
		const char* strEND_RawData = "10       1       1         1";
		const char* strPSAVER = "ZHDYF3.0";
		const char* strSPRINGType = "SPRING VERSION:";   
		const char* strEND_THIS_PIPING = "*THIS PIPING STRUCTURE *CONTAINS*";
//		int fdFlg;
            
		//�������Լ�¼:GLIF1.2,NC=6;GLIF3.x,NC=60(�䵥Ԫ)��61(ֱ��Ԫ),�ɶ�ʹ��.
		//�������Լ�¼:GLIF1.2,NC=9;GLIF3.x,NC=90(��ƹ���).
		//const iNCtj = 90, iNCdw = 60
		const int iNCtj = 9;
		const int iNCdw = 6;   
		float sngDWTemp=0, TjTemp=0;
		int iJSJDH_start=0, iJSJDH_end=0;
		long i=0;   
		int j=0, iC1=0;
		int iFlag=0, iCount=0;
		int mviSPR=0;
		CString mvS;
		ifstream f;
		CArray<long,long> iJSJDH;
		CArray<float,float> sngDW;
		CArray<float,float> Tj;
		
		int nFind = 0;

		iJSJDH.SetSize(100);
		sngDW.SetSize(100);
		Tj.SetSize(100);
   
		f.open(SourceDataFileName);
		//��ʼ��ԭʼ����
				//�����᷽��x/y/z������=1/2/3
				//ZHDYF3.0��֧���ܽӿ�ʱֻ�� [Y] ������
	
		CInputBox dlg;
		CString strUPxyz;
		while(strUPxyz == "" || (strUPxyz.GetAt(0) < '1' || strUPxyz.GetAt(0) > '3')  || (strUPxyz != "1" && strUPxyz != "2" && strUPxyz != "3"))
		{
			dlg.m_strTitle=GetResStr(IDS_INPUTUPXYZ);
			dlg.m_strWndText=GetResStr(IDS_SELUPXYZ);
			dlg.m_strValue="3";
			dlg.DoModal();
			strUPxyz =dlg.m_strValue;
			strUPxyz.TrimLeft();
			strUPxyz.TrimRight();
		}

		giUPxyz = atoi(strUPxyz);

		//ZHDYF3.0��,ֻ�ܰ��µ��ɼ���
		mviSPR=20;
		//mviSPR = temp.Find(strSPRINGType);
		//mviSPR = atoi(Trim(Mid(temp, mviSPR + strlen(strSPRINGType), 3)));
		modPHScal::gnSPR_CHZBH = 0.35f;
		strOldPhsSPRINGSel = modPHScal::gsPhsSPRINGSel;
		//��ΪӦ������ѡ��ĵ��ɱ�׼���ܲ�ͬ,�����¸��¹淶
		//�������ɹ淶����ʱ��ϳ�������ֻ�е��淶�仯ʱ������Ҫ���¸���
		if( mviSPR == 20 )
		{
			strSQL = "SELECT Spring_property FROM phsManuSPRING WHERE StandardDesc=\'";
				strSQL += strSprNWEPDI;
				strSQL += "\'";
			tmprs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(tmprs->BOF && tmprs->adoEOF)
			{
			}
			else
			{
				tmprs->get_Collect((_variant_t)"Spring_property",&varTmp);
				strSprTbn=vtos(varTmp);//��õ������Ա�
			}
			tmprs->Close();
			if( strOldPhsSPRINGSel != strSprNWEPDI )
			{
				modPHScal::gsPhsSPRINGSel = strSprNWEPDI;
				//if( blnSelphsSPEC(False) ) SaveRecentWorkSPEC();
			}
		}
		else if( mviSPR == 16 )
		{
 
		}

		//���ر仯ϵ�����浽�ⲿ
		::SetRegValue(_T("Settings"), "SPR_Chzbh", (modPHScal::gnSPR_CHZBH));
		//����ԺӦ����������Ϊ��̬������㵯��
		gbHotStatus2Zero=0;
		::SetRegValue(_T("Settings"),"HotStatus2Zero",(gbHotStatus2Zero));
  
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));
		strSQL.Format("SELECT G,SEQ FROM %s",strSprTbn);
		rs1->Open((_bstr_t)strSQL, _variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
			adOpenKeyset, adLockOptimistic, adCmdText); 

		iCount = 1;
                    
		rsData = rsResult;
   
		//ZHDYF3.0:��ʼ���������
		while(!f.eof())
		{
			int iType=7,iPnum=1,iSnum=0,iSPRSeq=0;
			float fDW=108,fTj=20,fPEL=0,fPaz=0,fPgz=0,fPjg=0;
			float fThick = 0; //�ں�
			 
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			temp.TrimLeft();
			temp.TrimRight();

			if(temp.IsEmpty())		//��������
				continue; 

			//line1
			if( atoi(Mid(temp,0, 4)) > 0 )  iPreINT[1] = atoi(Mid(temp,0, 4));
			
			//line2
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
	
			fDW = GetNextValue(temp);    //�⾶			
			fThick = GetNextValue(temp); //�ں�	
			fTj = GetNextValue(temp);    //�¶�
			fPEL = GetNextValue(temp);   //���

			//line3
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();

			iType = (int)GetNextValue(temp);   //֧��������
			iPnum = (int)GetNextValue(temp);   //������
			iSnum = (int)GetNextValue(temp);   //������
			iSPRSeq = (int)GetNextValue(temp); //���ɺ�
			fPaz = GetNextValue(temp);         //��װ����
			fPgz = GetNextValue(temp);         //���Ӻ���
			fPjg = GetNextValue(temp);         //�ṹ����

			
			//���ɼ���GLIF�����֧��������
			if(iSnum==0 )
			{
				//���ɴ�����=0���ǵ���֧����
				if(iType==5 || iType==7)
				{
					//���Ե���
					iPreINT[2]=120;
				}
				else if(iType==3)
				{
					//�̶�֧��
					iPreINT[2]=177;
				}
				else
				{
					//δ֪
					iPreINT[2]=120;
				}
			}
			else if(iSnum>=1)
			{
				if(iSPRSeq==99)
				{
					//��������
					iPreINT[2]=9999;
				}
				else
				{
					iPreINT[2]=iSnum*1000+iPnum*100+iSPRSeq;
				}
			}
		
			
			//line4����λ��(x,y,z),��λ��(x,y,z),
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			
			for(j=1; j<=6; j++)//ÿ��6������
			{
				phsVAL[j][1] = GetNextValue(temp);
			}
		
			//line5����̬��(x,y,z),��̬����(x,y,z),��̬��(x,y,z),��̬����(x,y,z)
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			
			for (j=1; j<=12; j++)//ÿ��12������
			{
				phsVAL[j][2] = GetNextValue(temp);
			}
			
			//line6��������(x,y,z),��������(x,y,z),żȻ��(x,y,z),żȻ����(x,y,z)
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();

			for (j=1; j<=12; j++)//ÿ��12������
			{
				phsVAL[j][3] = GetNextValue(temp);
			}

			rsData->Filter=_variant_t(CString("[jsdbh1] = ") + ltos(iPreINT[1])+" AND "+CString("[FileNameID]=")+ltos(FileNameID));
			if( rsData->BOF && rsData->adoEOF )
			{
				rsData->AddNew();
				rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
				rsData->PutCollect("Zdjh",_variant_t((long)(ltemp+1)));
				rsData->PutCollect("FileNameID",_variant_t(FileNameID));
				//rsData->PutCollect("Zdjh",_variant_t((long)iPreINT[1]));
				rsData->Update();
				ltemp++;
			}
			rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
			rsData->PutCollect("PSAver",strPSAVER);
			rsData->PutCollect("JSDBH1",_variant_t((long)iPreINT[1]));
			rsData->PutCollect("dj1",_variant_t((float)fDW));
			rsData->PutCollect("t01",_variant_t((float)fTj));
			rsData->PutCollect("Type",_variant_t((long)iPreINT[2]));
			rsData->PutCollect("psaType",_variant_t((long)iPreINT[2]));
			rsData->PutCollect("dh1", _variant_t(fPEL));
			rsData->PutCollect("Unit_Force", "N");
			rsData->PutCollect("Unit_Momental", "N.m");
			rsData->PutCollect("Unit_Displacement", "mm");
			rsData->PutCollect("paz0", _variant_t(fPaz));
			rsData->PutCollect("paz1", _variant_t(fPaz));
			rsData->PutCollect("pgz1", _variant_t(fPgz));
			rsData->PutCollect("pjg1", _variant_t(fPjg));
			rsData->PutCollect("pxl1", _variant_t(phsVAL[1][2]));
			rsData->PutCollect("pyl1", _variant_t(phsVAL[2][2]));
			rsData->PutCollect("pzl1", _variant_t(phsVAL[3][2]));
			rsData->PutCollect("pxr1", _variant_t(phsVAL[7][2]));
			rsData->PutCollect("pyr1", _variant_t(phsVAL[8][2]));
			rsData->PutCollect("pzr1", _variant_t(phsVAL[9][2]));


			rsData->PutCollect("xr1", _variant_t(phsVAL[4][1] * 1));
			rsData->PutCollect("yr1", _variant_t(phsVAL[5][1] * 1));
			rsData->PutCollect("zr1", _variant_t(phsVAL[6][1] * 1));					

			if( iSPRSeq!=99 )//ֻ�е�����Ҫ������λ��
			{
				float fG,fR;

				if(iPreINT[2]>1000 && iPreINT[2]<5000)
				{
					CString strTmp;
					strTmp.Format("%d",iPreINT[2]);
					_variant_t vTmp;
					rs1->Find((_bstr_t)("SEQ=" + ltos(iSPRSeq)), 0, adSearchForward);
					if(!rs1->adoEOF)
					{
						rs1->get_Collect((_variant_t)"G",&varTmp);
						fG=vtof(varTmp);
						fR=fabs(fPaz) - fabs(fPgz);
						fR/=EDIBgbl::kgf2N;
						fR/=iPnum;
						fR/=(fG/iSnum);
						if(giUPxyz==1)
						{
							rsData->PutCollect("xr1", _variant_t(fR));
						}
						else if(giUPxyz==2)
						{
							rsData->PutCollect("yr1", _variant_t(fR));
						}
						else
						{
							rsData->PutCollect("zr1", _variant_t(fR));
						}
					}
				}else
				{
				   rsData->PutCollect("yr1", _variant_t(phsVAL[5][1] * 1));
				}
			}
			else
			{
			   rsData->PutCollect("yr1", _variant_t(phsVAL[5][1] * 1));
			}

			rsData->PutCollect("xl1", _variant_t(phsVAL[1][1] * 1));
			rsData->PutCollect("yl1", _variant_t(phsVAL[2][1] * 1));
			rsData->PutCollect("zl1", _variant_t(phsVAL[3][1] * 1));

			//rsData->PutCollect("haz", _variant_t(phsVAL[1][8] * 1));
			rsData->PutCollect("mxl1", _variant_t(phsVAL[4][2]));
			rsData->PutCollect("myl1", _variant_t(phsVAL[5][2]));
			rsData->PutCollect("mzl1", _variant_t(phsVAL[6][2]));
			rsData->PutCollect("mxr1", _variant_t(phsVAL[10][2]));
			rsData->PutCollect("myr1", _variant_t(phsVAL[11][2]));
			rsData->PutCollect("mzr1", _variant_t(phsVAL[12][2]));

			rsData->Filter=_variant_t((long)adFilterNone);
			iC1++;
			 if( iCount <= 0 ) iCount = 1;
			 frmStatus.UpdateStatus((float)iC1/(float)iCount,true);
			 frmStatus.UpdateWindow();
		}
		f.close();
		if(!rsData->adoEOF || !rsData->BOF)
			rsData->MoveFirst();
		_variant_t vtmp;
		while(!rsData->adoEOF)
		{
			vtmp=rsData->GetCollect("ZDJH");
			rsData->PutCollect("ZDJH",_variant_t((long)(vtoi(vtmp)+1000)));
			rsData->MoveNext();
		}
		_ConnectionPtr pCon;
		strSQL=(char*)(bstr_t)rsData->GetSource();
		pCon=rsData->GetActiveConnection();
		strSQL.Replace("ORDER BY JSDBH1","ORDER BY FileNameID");
		rsData->Close();
		rsData->Open(_variant_t(strSQL),(IDispatch*)pCon,adOpenKeyset,adLockOptimistic,adCmdText);
		//rsData->Sort=_bstr_t("JSDBH1");
		int itmpZdjh=1;
		while(!rsData->adoEOF)
		{
			rsData->PutCollect("ZDJH",_variant_t((long)itmpZdjh));
			itmpZdjh++;
			rsData->MoveNext();
		}
	}
	catch(_com_error& e)
	{
		ShowMessage(e.Description());
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
		ShowMessage("C++ Exception at readpsaresult.cpp::ReadResult_ZHDYF30()");
	}
}

//�����ַ����е����� //add by cbl
double GetStrAt(CString str,int i)
{
	str = str.Mid(str.Find('+')+1);
	while(str.Find('(') != -1 && str.Find(')') != -1)
	{
		str.Replace('(',' ');
		str.Replace(')',' ');
	}
	str.TrimLeft();
	str = str +"  ";
	for(int index=1; index <=i-1;index++)
	{
		str = str.Mid(str.Find(' ')+1);
		str.TrimLeft();
	}
	return atof(str.Left(str.Find(' ')));

}

//�ӽڵ������ļ��ж�ȡ���ݵ����� add by cbl
void ElevationFileToList(CString FileName,ElevationList *pElevationList,int ICC)
{
	ifstream fin;
	CString strText;
	int NodeNum;
	double UpElevation=0;
	fin.open(FileName);
	if(!fin)
		return;
	CNodeUpElevation *pNodeUpElevationData;
	while(!fin.eof())
	{
		fin.getline(strText.GetBufferSetLength(255),255);
		strText.ReleaseBuffer();
		NodeNum = GetStrAt(strText,1);
	    UpElevation = GetStrAt(strText,ICC+1);

		pNodeUpElevationData = new CNodeUpElevation();
		pElevationList->AddTail(pNodeUpElevationData);
		pNodeUpElevationData->NodeNum = NodeNum;
		pNodeUpElevationData->UpElevation = UpElevation;
	}
	
}


void ReadResult_GLIF12(_Recordset* rsResult ,CString SourceDataFileName,long maxZdjh,long FileNameID)
{
   //Remauther:
   //����:
   //Ŀ��:������Ϊ��*.out�ļ�����������msgbox��������Գ������ȷ��
   //����:rsResult���ݱ��ڲ���λ:λ��mm,����kgf.
   //����:
	try
	{
		long ltemp=maxZdjh;

		CString strOldPhsSPRINGSel;
		CString strTemp, temp ;int iTemp=0;
		CString strSQL;
		float phsVAL[7][9];//������������ĵ��������ݣ�Xά�����������ݺ�����У�Yά������
		int iPreINT[3];    //����������¼ǰ�������������
		_Recordset* rsData;

		const char* strPIPING_STRESS_OUTUNIT = "UNIT OF OUTPUTDATA IS";
		const char* strSUMMARY_FOR_RESTRAINTS = "SUMMARY FOR RESTRAINTS";
		const char* strCOLD_SPRING_TABLE = "COLD SPRINGS TABLE";
		const char* strEND_RawData = "10       1       1         1";
		const char* strPSAVER = "GLIF V12";
		const char* strSPRINGType = "SPRING VERSION:";
   
		const char* strEND_THIS_PIPING = "*THIS PIPING STRUCTURE *CONTAINS*";
		CString strSprTbn;
		_RecordsetPtr tmprs;
		tmprs.CreateInstance(__uuidof(Recordset));
		_variant_t varTmp;
//		int fdFlg;
            
		//�������Լ�¼:GLIF1.2,NC=6;GLIF3.x,NC=60(�䵥Ԫ)��61(ֱ��Ԫ),�ɶ�ʹ��.
		//�������Լ�¼:GLIF1.2,NC=9;GLIF3.x,NC=90(��ƹ���).
		//const iNCtj = 90, iNCdw = 60
		const int iNCtj = 9;
		const int iNCdw = 6;
   
		float sngDWTemp=0, TjTemp=0;
		int iJSJDH_start=0, iJSJDH_end=0;
		CArray<long,long  > iJSJDH;
		CArray<float,float  > sngDW;
		CArray<float,float  > Tj;
		CArray<float,float &> sngDH1;

		sngDH1.SetSize(100);
		iJSJDH.SetSize(100);
		sngDW.SetSize(100);
		Tj.SetSize(100);
		long i=0;
   
		int j=0, iC1=0;
		ifstream f;
		int iFlag=0, iCount=0;
		CString mvS,tempsign;
		int mviSPR=0;
        bool sign=false;//chengbailian 2003.11.6 Ϊ����Ӧ����һ���ļ���90��¼�Ƿ���10��¼֮¼����		
		long pos;

		//cbl 2004.1.10 ������ÿ���ڵ�ı��
		ElevationList mElevationList;
		CNodeUpElevation *pNodeUpElevation;
		int ICC;
		
		
		//�����ļ���
		CString ElevationFileName=SourceDataFileName.Left(SourceDataFileName.ReverseFind('.'))+".et";//����ļ���

		//���������ļ�(��¼��ÿ���ڵ������)(���ӿ��еĺ���)
		ICC=PSAPointCoordinateWirteFile(SourceDataFileName,ElevationFileName);
	  
		//���ļ��ж�ȡÿ����ı�߷�������mElevationList��
		ElevationFileToList(ElevationFileName,&mElevationList,ICC);
        ::DeleteFile(ElevationFileName); 
	//cbl

		f.open(SourceDataFileName);
		//��ʼ��ԭʼ����
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			if(temp.Find(strPIPING_STRESS_OUTUNIT)!=-1)
			{
				if( temp.Find("NATIONAL") !=-1 ){
					EDIBgbl::PSA_OutDataUnit = "NATIONAL";
				}else{
					EDIBgbl::PSA_OutDataUnit = "ENGINEERING";
				}
				//�����᷽��x/y/z������=1/2/3
				//GLIFV1.2��ֻ��Y������
				giUPxyz = 2;
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				//GLIFV1.2��,ԭʼ�����ļ���2����¼��4������Ϊ��������,(ITW=20,���µ��ɼ���;ITW=16,���ܹ浯��)
				mviSPR = temp.Find(strSPRINGType);
				mviSPR = atoi(Trim(Mid(temp, mviSPR + strlen(strSPRINGType), 3)));
				modPHScal::gnSPR_CHZBH = 0.35f;
				strOldPhsSPRINGSel = modPHScal::gsPhsSPRINGSel;
				//��ΪӦ������ѡ��ĵ��ɱ�׼���ܲ�ͬ,�����¸��¹淶
				//�������ɹ淶����ʱ��ϳ�������ֻ�е��淶�仯ʱ������Ҫ���¸���
				if( mviSPR == 20 )
				{
					strSQL = "SELECT Spring_property FROM phsManuSPRING WHERE StandardDesc=\'";
					strSQL += strSprNWEPDI;
					strSQL += "\'";
					tmprs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
						adOpenKeyset, adLockOptimistic, adCmdText); 
					if(tmprs->BOF && tmprs->adoEOF)
					{
					}
					else
					{
						tmprs->get_Collect((_variant_t)0L, &varTmp);
						strSprTbn=vtos(varTmp);
					}
					tmprs->Close();
					if( strOldPhsSPRINGSel != strSprNWEPDI )
					{
						modPHScal::gsPhsSPRINGSel = strSprNWEPDI;
						//if( blnSelphsSPEC(False) ) SaveRecentWorkSPEC();
					}
				}
				else if( mviSPR == 16 )
				{
         
				}
				//���ر仯ϵ�����浽�ⲿ
				::SetRegValue(_T("Settings"), "SPR_Chzbh", (modPHScal::gnSPR_CHZBH));
				break;
			}
		}
   
		iCount = 1;
		while(!f.eof())
		{
			iFlag=0;
			do
			{
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				//�������ԭʼ���ݽ�����־���˳�ѭ��
				if( Trim(Mid(temp,24)).Find(strEND_RawData)==0) break;
				iTemp =atoi(Mid(temp,24,4));
				if( iTemp == iNCtj ) 
				{
					TjTemp = atof(Mid(temp, 51, 10));
				    sign=true;
				}
				if( iTemp == iNCdw ) sngDWTemp = atof(Mid(temp,35, 10));
				//����ǽڵ��¼(NC=10)
				if( (iTemp == iNCdw)  ) 
				{
					sngDWTemp = atof(Mid(temp,35, 10));
					if( iFlag != 0 ) 
						sngDW.SetAtGrow(iCount-1,sngDWTemp);
				}
				if( iTemp == 10 )
				{
                   if(!sign)//chengbailian 2003.11.6
					{//���10��¼���ֶ�9(�������ԣ���¼��û�г��֣����ҵ�90��¼��ȡ�����¶ȣ���PSA��������ļ�û����������������ܵ��������������������
						pos=f.tellg();
						while(!f.eof())
						{
							f.getline(tempsign.GetBuffer(255),255);
						    tempsign.ReleaseBuffer();
							if(atoi(Mid(tempsign, 24, 4))==iNCtj)
							{
								TjTemp = atof(Mid(tempsign, 51, 10));
								break;
							}

						}
						f.seekg(pos);//ָ��ص�ԭ��λ��
					}

					//�����ʼ�ڵ��
					iJSJDH_start = atoi(Mid(temp, 26, 8));
					//�����ĩ�ڵ��
					iJSJDH_end = atoi(Mid(temp,35 ,8));
					if( iFlag == 0 )
					{
						//��һ���ڵ��¼ֻҪ�򵥵ر���ʼĩ�ڵ�
						iJSJDH.SetAtGrow(iCount,iJSJDH_start);
						sngDW.SetAtGrow(iCount,sngDWTemp);
						Tj.SetAtGrow(iCount,TjTemp);
						iCount++;
               
                			pos=f.tellg();
			                int iFlg = 0;
							while(!f.eof() && iFlg==0)
							{
								f.getline(tempsign.GetBuffer(255),255);
								tempsign.ReleaseBuffer();
								if (atoi(Mid(tempsign, 24, 4))==10)
								{
									break;
								}
								if(atoi(Mid(tempsign, 24, 4))==iNCtj)
								{
                                    long Ppos;
                                    float fTjTemp = atof(Mid(tempsign, 51, 10));
									while (!f.eof() && iFlg==0)
									{
                                        Ppos = f.tellg();
								        f.getline(tempsign.GetBuffer(255),255);
								        tempsign.ReleaseBuffer();
										int ipipetype = atoi(Mid(tempsign, 24, 4));
										if (ipipetype == iNCtj)
										{
											f.seekg(Ppos);//ָ��ص�ԭ��λ��
											break;
										}
        								if (ipipetype == 10)
										{
											iFlg = 1;
									        break;
										}
										if (ipipetype==0 || ipipetype==1 || ipipetype==2 ||ipipetype==3 ||
											ipipetype==4 || ipipetype==5 || ipipetype==6)
										{
											TjTemp = fTjTemp;
										}
									}
								}
							}
							f.seekg(pos);//ָ��ص�ԭ��λ��

						iJSJDH.SetAtGrow(iCount,iJSJDH_end);
						sngDW.SetAtGrow(iCount,sngDWTemp);
						Tj.SetAtGrow(iCount,TjTemp);
						iCount++;
					}
					else
					{
						for( i = 1 ;i<iCount;i++)
						{
							if( iJSJDH_end == iJSJDH[i] ) break;
						}
						if( i >= iCount )
						{
                			pos=f.tellg();
			                int iFlg = 0;
							while(!f.eof() && iFlg==0)
							{
								f.getline(tempsign.GetBuffer(255),255);
								tempsign.ReleaseBuffer();
								if (atoi(Mid(tempsign, 24, 4))==10)
								{
									break;
								}
								if(atoi(Mid(tempsign, 24, 4))==iNCtj)
								{
                                    long Ppos;
                                    float fTjTemp = atof(Mid(tempsign, 51, 10));
									while (!f.eof() && iFlg==0)
									{
                                        Ppos = f.tellg();
								        f.getline(tempsign.GetBuffer(255),255);
								        tempsign.ReleaseBuffer();
										int ipipetype = atoi(Mid(tempsign, 24, 4));
										if (ipipetype == iNCtj)
										{
											f.seekg(Ppos);//ָ��ص�ԭ��λ��
											break;
										}
        								if (ipipetype == 10)
										{
											iFlg = 1;
									        break;
										}
										if (ipipetype==0 || ipipetype==1 || ipipetype==2 ||ipipetype==3 ||
											ipipetype==4 || ipipetype==5 || ipipetype==6)
										{
											TjTemp = fTjTemp;
										}
									}
								}
							}
							f.seekg(pos);//ָ��ص�ԭ��λ��
							
							iJSJDH.SetAtGrow(iCount,iJSJDH_end);
							sngDW.SetAtGrow(iCount,sngDWTemp);
							Tj.SetAtGrow(iCount,TjTemp);
							iCount++;
						}
					}
					//���ñ�־�����Ƿ��ǵ�һ���ڵ��¼
					if( iFlag == 0 ) iFlag = 1;
				}
			}while(Trim(Mid(temp,24)).Find(strEND_RawData)!=0 && !f.eof());
			if( Trim(Mid(temp,24)).Find(strEND_RawData)==0) break;
		}
   
		//GLIFV1.2:���ҽ������ͷ��
		while(!f.eof())
		{
			 f.getline(strTemp.GetBuffer(255),255);
			 strTemp.ReleaseBuffer();
			 if( strTemp.Find(strSUMMARY_FOR_RESTRAINTS) !=-1 ) //�ҵ���SUMMARY FOR RESTRAINTS��
					break;
		}
                  
		rsData = rsResult;
   
		//GLIFV1.2:��ʼ���������
		while(!f.eof())
		{
			 f.getline(temp.GetBuffer(255),255);
			 temp.ReleaseBuffer();
			 if( atoi(Mid(temp,4, 3)) > 0 ){
				  iPreINT[1] = atoi(Mid(temp,0, 7));
				  iPreINT[2] = atoi(Mid(temp, 7, 11));
				  for( i = 1 ;i<= 6;i++)  //ÿ���ڵ�6������
				  {
						for (j = 1 ;j<= 8 ;j++)//ÿ�����8������,����Ϊpaz,pgz,pjg,psl,psr,dl,dr,h(uy,Ԥѹ��ֵ)
						{
							phsVAL[i][ j] = atof(Mid(temp,19 + (j - 1) * 13-1, 13));
							if( iPreINT[2] == 177 && j == 8 ){
								//����GLIFV1.2���BUG,�ڵ�����177�ṹ����д�����һ����,�����˴���.
								phsVAL[i][3] = atof(Mid(temp, 74 + (j - 4) * 13-1, 13));
							}
						}
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();  //����1����
				  }				  
				  rsData->Filter=_variant_t(CString("[jsdbh1] = ") + ltos(iPreINT[1])+" AND "+CString("[FileNameID]=")+ltos(FileNameID));
				  //Set rs = rsData.OpenRecordset()
					
					if( rsData->BOF && rsData->adoEOF ){
						 rsData->AddNew();
						 rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						 rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						 rsData->PutCollect("Zdjh",_variant_t((long)(ltemp+1)));
						 rsData->PutCollect("FileNameID",_variant_t(FileNameID));
						 //rsData->PutCollect("Zdjh",_variant_t((long)iPreINT[1]));
						 rsData->Update();
						 ltemp++;
					}
					for(i = 1 ;i< iCount;i++)
					{
						//ȷ��iֵ
						if( iJSJDH[i] == iPreINT[1] )break;
						//Debug.Print i, iPreINT(1),iJSJDH(i), sngDW(i), Tj(i)
					}
					//if( IsNull(!PSAver) Or (Not IsNull(!PSAver) And Trim(!PSAver) = "") Or (Not IsNull(!PSAver) And Trim(!PSAver) <> "" And UCase(Trim(!PSAver)) = strPSAVER) ){
					rsData->PutCollect("PSAver",strPSAVER);
					rsData->PutCollect("JSDBH1",_variant_t((long)iPreINT[1]));
					rsData->PutCollect("dj1",_variant_t((float)sngDW[i]));


					//chengbailian
					 POSITION pos=mElevationList.GetHeadPosition();
					 while(pos !=NULL)
					 {
					     //���ҵ�ǰ֧����ı��	
						 pNodeUpElevation = mElevationList.GetNext(pos);

						 if(pNodeUpElevation->NodeNum == iPreINT[1])
						 {
							 sngDH1.SetAtGrow(i,pNodeUpElevation->UpElevation); 
							 sngDH1[i]=int(sngDH1[i]*1000+0.5)/1000.0; //��ȷ��λС��
							 break;
						 }
						 
					 }

					 if(vtof(rsData->GetCollect("dh1"))==0.0)//���ԭ�������ĸ�
						rsData->PutCollect("dh1", _variant_t(sngDH1[i]));
                     //end cbl


					rsData->PutCollect("t01",_variant_t((float)Tj[i]));
					rsData->PutCollect("Type",_variant_t((long)iPreINT[2]));
					rsData->PutCollect("psaType",_variant_t((long)iPreINT[2]));
					if( EDIBgbl::PSA_OutDataUnit == "NATIONAL" ){
						//����3����������Ϊpaz1,pgz1,pjg1
						//phsVAL(2, 1) = Format(phsVAL(2, 1) / kgf2N, "0.0")
						//phsVAL(2, 2) = Format(phsVAL(2, 2) / kgf2N, "0.0")
						//phsVAL(2, 3) = Format(phsVAL(2, 3) / kgf2N, "0.0")
             
						//����4����������Ϊx.z��������(��/��)
						//phsVAL(1, 1) = Format(phsVAL(1, 1) / kgf2N, "0.0")
						//phsVAL(3, 1) = Format(phsVAL(3, 1) / kgf2N, "0.0")
						//phsVAL(1, 2) = Format(phsVAL(1, 2) / kgf2N, "0.0")
						//phsVAL(3, 2) = Format(phsVAL(3, 2) / kgf2N, "0.0")
               
						//����6����������Ϊx.y.z��������(��/��)
						//phsVAL(4, 1) = Format(phsVAL(4, 1) / kgf2N, "0.0")
						//phsVAL(5, 1) = Format(phsVAL(5, 1) / kgf2N, "0.0")
						//phsVAL(6, 1) = Format(phsVAL(6, 1) / kgf2N, "0.0")
						//phsVAL(4, 2) = Format(phsVAL(4, 2) / kgf2N, "0.0")
						//phsVAL(5, 2) = Format(phsVAL(5, 2) / kgf2N, "0.0")
						//phsVAL(6, 2) = Format(phsVAL(6, 2) / kgf2N, "0.0")
						rsData->PutCollect("Unit_Force", "N");
						rsData->PutCollect("Unit_Momental", "N.m");
					 }else{
						rsData->PutCollect("Unit_Force", "kgf");
						rsData->PutCollect("Unit_Momental", "kgf.m");
					 }
					 rsData->PutCollect("Unit_Displacement", "mm");
					rsData->PutCollect("paz0", _variant_t(phsVAL[2][1]));
					rsData->PutCollect("paz1", _variant_t(phsVAL[2][1]));
					rsData->PutCollect("pgz1", _variant_t(phsVAL[2][2]));
					rsData->PutCollect("pjg1", _variant_t(phsVAL[2][3]));
					rsData->PutCollect("pxl1", _variant_t(phsVAL[1][1]));
					rsData->PutCollect("pyl1", _variant_t(phsVAL[2][1]));
					rsData->PutCollect("pzl1", _variant_t(phsVAL[3][1]));
					rsData->PutCollect("pxr1", _variant_t(phsVAL[1][2]));
					rsData->PutCollect("pyr1", _variant_t(phsVAL[2][2]));
					rsData->PutCollect("pzr1", _variant_t(phsVAL[3][2]));
					rsData->PutCollect("xl1", _variant_t(phsVAL[1][6] * EDIBgbl::cm2mm));
					rsData->PutCollect("yl1", _variant_t(phsVAL[2][6] * EDIBgbl::cm2mm));
					rsData->PutCollect("zl1", _variant_t(phsVAL[3][6] * EDIBgbl::cm2mm));
					rsData->PutCollect("xr1", _variant_t(phsVAL[1][7] * EDIBgbl::cm2mm));
					rsData->PutCollect("yr1", _variant_t(phsVAL[2][7] * EDIBgbl::cm2mm));
					rsData->PutCollect("zr1", _variant_t(phsVAL[3][7] * EDIBgbl::cm2mm));
					rsData->PutCollect("haz", _variant_t(phsVAL[1][8] * EDIBgbl::cm2mm));
					rsData->PutCollect("mxl1", _variant_t(phsVAL[4][1]));
					rsData->PutCollect("myl1", _variant_t(phsVAL[5][1]));
					rsData->PutCollect("mzl1", _variant_t(phsVAL[6][1]));
					rsData->PutCollect("mxr1", _variant_t(phsVAL[4][2]));
					rsData->PutCollect("myr1", _variant_t(phsVAL[5][2]));
					rsData->PutCollect("mzr1", _variant_t(phsVAL[6][2]));
					
				  rsData->Filter=_variant_t((long)adFilterNone);
				  iC1++;
			 }
			 else
			 {
				if( temp.Find(strCOLD_SPRING_TABLE) !=-1 )
					 break;
			 }
			 
			 if( iCount <= 0 ) iCount = 1;
			 //MsgBox iC1 & ":" & iCount
			 frmStatus.UpdateStatus((float)iC1/(float)iCount,true);
			 frmStatus.UpdateWindow();
			 //frmStatus.picstatus.Refresh
			 //frmStatus.Refresh
		}
		f.close();

		//�ͷ��ڴ� cbl
		POSITION posList=mElevationList.GetHeadPosition();
		POSITION preposList;
		CNodeUpElevation *pNodeUpElevationData;
		while(posList !=NULL)
		{
			preposList=posList;
			pNodeUpElevationData = mElevationList.GetNext(posList);
            mElevationList.RemoveAt(preposList);
			delete pNodeUpElevationData;
		}
		 

        if(!rsData->adoEOF || !rsData->BOF)
			rsData->MoveFirst();
		_variant_t vtmp;
		while(!rsData->adoEOF)
		{
			vtmp=rsData->GetCollect("ZDJH");
			rsData->PutCollect("ZDJH",_variant_t((long)(vtoi(vtmp)+1000)));
			rsData->MoveNext();
		}
		_ConnectionPtr pCon;
		strSQL=(char*)(bstr_t)rsData->GetSource();
		pCon=rsData->GetActiveConnection();
		strSQL.Replace("ORDER BY JSDBH1","ORDER BY FileNameID");
		rsData->Close();
		rsData->Open(_variant_t(strSQL),(IDispatch*)pCon,adOpenKeyset,adLockOptimistic,adCmdText);
		//rsData->Sort=_bstr_t("JSDBH1");
		int itmpZdjh=1;
		while(!rsData->adoEOF)
		{
			rsData->PutCollect("ZDJH",_variant_t((long)itmpZdjh));
			itmpZdjh++;
			rsData->MoveNext();
		}
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
	}
}




void ReadResult_GLIF31(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID)
{
	  //Remauther:
		//����:
		//Ŀ��:������Ϊ��*.out�ļ�����������msgbox��������Գ������ȷ��
		//����:rsResult���ݱ��ڲ���λ:λ��mm,����kgf.
		//����:
   try
	{
		_variant_t nil;
		nil.ChangeType(VT_NULL);
		 CString strOldPhsSPRINGSel; //�����ȡӦ����������֮ǰ�ĵ��ɱ�׼
		 float mvMultiple=0;
		 CString sFx, sFx1;
		 CString strTemp;
		 CString temp;
		 int iTemp=0;
		 float phsVAL[7][9]; //������������ĵ��������ݣ�Xά�����������ݺ�����У�
												 //Yά������
		 int iPreINT[3]; //����������¼ǰ�������������
		 _Recordset* rsData;

		LPCTSTR strPIPING_STRESS_OUTUNIT = "++++";   //GLIFV3.1��������ļ��е�һ����"++++"�����ǵ�λ���Ƽ�¼,���е�һ������IIN�����뵥λ����,�ڶ�������IOU�������λ����(IOU=0������λ��,IOU=1������)
		LPCTSTR strSTRUCTURE_LOAD_OF_RESTRAINTS = "STRUCTURE LOAD OF RESTRAINTS";
		LPCTSTR strREACTION_ON_ANCHORS_ColdInitial_CASE4 = "REACTION OF PINING ON ANCHORS (COLD-INITIAL)";
		LPCTSTR strREACTION_ON_ANCHORS_ColdRelease_CASE3 = "REACTION OF PIPING ON ANCHORS (COLD-RELEASE)";
		LPCTSTR strREACTION_ON_ANCHORS_WorkInitial_CASE2 = "REACTION OF PINING ON ANCHORS (WORK-INITIAL)";
		LPCTSTR strREACTION_ON_ANCHORS_Cold_CASE1 = "REACTION OF PINING ON ANCHORS";
		LPCTSTR strCOLD_SPRING_TABLE = "COLD SPRING JOINT POINT";
		LPCTSTR strEND_RawData = "10         1         1         1" ; //�ո������GLIFV1.2��ͬ
		LPCTSTR strPSAVER = "GLIF V31";
   
		LPCTSTR strCW_DISPLACEMENT = "CW-DISPLACEMENT (cold/work-status)";
   
		LPCTSTR strEND_THIS_PIPING = "*THIS PIPING STRUCTURE *CONTAINS*";
   
		CString strSprTbn;
		_RecordsetPtr tmprs;
		tmprs.CreateInstance(__uuidof(Recordset));
		_variant_t varTmp;
		CString mvsCASE, mvsCASEcool;
		int mviSPR=0;
		long i=0;
		//����ʾ�����֮ǰ���ؽ��ȿ�
		frmStatus.ShowWindow(SW_HIDE);
		CInputBox dlg;
		while(sFx == "" || (sFx.GetAt(0) < '0' || sFx.GetAt(0) > '9')  || (sFx != "1" && sFx != "3" && sFx != "4"))
		{
			dlg.m_strTitle=GetResStr(IDS_SelThrustQuaGist);
			dlg.m_strWndText=GetResStr(IDS_SelWorkCon);
			dlg.m_strValue="4";
			dlg.DoModal();
			sFx =dlg.m_strValue;
			sFx.TrimLeft();
			sFx.TrimRight();
		}
		i = atoi(sFx);
		if( i == 1 ){
			mvsCASEcool = strREACTION_ON_ANCHORS_Cold_CASE1;
		}else{
			if( i == 3 ){
				mvsCASEcool = strREACTION_ON_ANCHORS_ColdRelease_CASE3;
			}else{
				mvsCASEcool = strREACTION_ON_ANCHORS_ColdInitial_CASE4;
			}
		}
		frmStatus.ShowWindow(SW_SHOW);
   
		//�������Լ�¼:GLIF1.2,NC=6;GLIF3.x,NC=60(�䵥Ԫ)��61(ֱ��Ԫ),�ɶ�ʹ��.
		//�������Լ�¼:GLIF1.2,NC=9;GLIF3.x,NC=90(��ƹ���).
		const int iNCtj = 90, iNCdw1 = 61, iNCdw = 60;
		//Const iNCtj = 9, iNCdw = 6
   
		float sngDWTemp=0, TjTemp=0;
		long iJSJDH_start=0, iJSJDH_end=0;
		CArray<long,long & > iJSJDH;
		CArray<float,float & > sngDW;
		CArray<float ,float & > Tj;
		CArray<float,float &> sngDH1;

		sngDH1.SetSize(100);
		iJSJDH.SetSize(100);
		sngDW.SetSize(100);
		Tj.SetSize(100);
   
		int j=0, iC1=0;
		ifstream f;
		int iFlag=0, iCount=0;
		CString mvS,tempsign;
		bool bCwDisplacement=false, bStrucLoad=false;
        bool sign=false;//chengbailian 2003.11.6 Ϊ����Ӧ����һ���ļ���90��¼�Ƿ���10��¼֮¼����		
		long pos;

		//cbl 2004.1.10 ������ÿ���ڵ�ı��
		ElevationList mElevationList;
		CNodeUpElevation *pNodeUpElevation;
		int ICC;
		
		
		//�����ļ���
		CString ElevationFileName=SourceDataFileName.Left(SourceDataFileName.ReverseFind('.'))+".et";//����ļ���

		//���������ļ�(��¼��ÿ���ڵ������)(���ӿ��еĺ���)
		ICC=PSAPointCoordinateWirteFile(SourceDataFileName,ElevationFileName);
	  
		//���ļ��ж�ȡÿ����ı�߷�������mElevationList��
		ElevationFileToList(ElevationFileName,&mElevationList,ICC);
        ::DeleteFile(ElevationFileName); 
	//cbl

		f.open(SourceDataFileName);

		CString strSQL;
		//��ʼ��ԭʼ����
		while(!f.eof())
		{			
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			if( temp.Find(strPIPING_STRESS_OUTUNIT)!=-1){
				//��һ����¼��һ���ֶ������뵥λ�ƣ���2���ֶ��������λ��(=0��������λ��)����3���ֶ��������᷽��
				if( atoi(Trim(Mid(temp,16, 4))) == 0 ){
					EDIBgbl::PSA_OutDataUnit = "NATIONAL";
				}else{
					EDIBgbl::PSA_OutDataUnit = "ENGINEERING";
				}
				//�����᷽��x/y/z������=1/2/3
				giUPxyz = atoi(Mid(temp,20, 12));
				f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
				//GLIFV3.x��,ԭʼ�����ļ���2����¼��1������Ϊ��������,��3������Ϊ���ɺ��ر仯ϵ��
				mviSPR = atoi(Trim(Mid(temp, 6, 10)));
				modPHScal::gnSPR_CHZBH = atof(Trim(Mid(temp, 21, 10)));
				strOldPhsSPRINGSel = modPHScal::gsPhsSPRINGSel;
				//��ΪӦ������ѡ��ĵ��ɱ�׼���ܲ�ͬ,�����¸��¹淶
				//�������ɹ淶����ʱ��ϳ�������ֻ�е��淶�仯ʱ������Ҫ���¸���
				if( mviSPR == 3 ){
					strSQL = "SELECT Spring_property FROM phsManuSPRING WHERE StandardDesc=\'";
					strSQL += strSprNWEPDI;
					strSQL += "\'";
					tmprs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
						adOpenKeyset, adLockOptimistic, adCmdText); 
					if(tmprs->BOF && tmprs->adoEOF)
					{
					}
					else
					{
						tmprs->get_Collect((_variant_t)0L, &varTmp);
						strSprTbn=vtos(varTmp);
					}
					tmprs->Close();
					if( strOldPhsSPRINGSel != strSprNWEPDI ){
						modPHScal::gsPhsSPRINGSel = strSprNWEPDI;
						//if( blnSelphsSPEC(False) ) SaveRecentWorkSPEC();
					}
				}else if( mviSPR == 4 ){
					strSQL = "SELECT Spring_property FROM phsManuSPRING WHERE StandardDesc=\'";
					strSQL += strSprGB10182;
					strSQL += "\'";
					tmprs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
						adOpenKeyset, adLockOptimistic, adCmdText); 
					if(tmprs->BOF && tmprs->adoEOF)
					{
					}
					else
					{
						tmprs->get_Collect((_variant_t)0L, &varTmp);
						strSprTbn=vtos(varTmp);
					}
					tmprs->Close();
					if( strOldPhsSPRINGSel != strSprGB10182 ){
						modPHScal::gsPhsSPRINGSel = strSprGB10182;
						//if( blnSelphsSPEC(False) ) SaveRecentWorkSPEC();
					}
				}
         
				//���ر仯ϵ�����浽�ⲿ
				SetRegValue(_T("Settings"), "SPR_Chzbh", (modPHScal::gnSPR_CHZBH));
				break;
			}
		}
   
		iCount = 1;
		while(!f.eof()){
			iFlag=0;
			do
			{
				f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
				//�������ԭʼ���ݽ�����־���˳�ѭ��
				if( Trim(Mid(temp,18)).Find( strEND_RawData)==0) break;
				iTemp = atoi(Mid(temp, 16, 4));
				//����ǽڵ��¼(NC=10)
				if( iTemp == iNCtj ) 
				{
					TjTemp = atof(Mid(temp, 32, 7));
					sign=true;
				}
				if( (iTemp == iNCdw)  ) sngDWTemp = atof(Mid(temp, 22, 7));
				if( (iTemp == iNCdw)  ) 
				{
					sngDWTemp = atof(Mid(temp, 22, 7));
					if( iFlag != 0 ) 
						sngDW.SetAtGrow(iCount-1,sngDWTemp);
				}

				if( iTemp == 10 )
				{
					if(!sign)//chengbailian 2003.11.6
					{//���10��¼���ֶ�90��¼��û�г��֣����ҵ�90��¼��ȡ�����¶ȣ���PSA��������ļ�û����������������ܵ��������������������
						pos=f.tellg();
						while(!f.eof())
						{
							f.getline(tempsign.GetBuffer(255),255);
						    tempsign.ReleaseBuffer();
							if(atoi(Mid(tempsign, 16, 4))==iNCtj)
							{
								TjTemp = atof(Mid(tempsign, 32, 7));
								break;
							}

						}
						f.seekg(pos);//ָ��ص�ԭ��λ��
					}

					//�����ʼ�ڵ��
					iJSJDH_start = atoi(Mid(temp, 21, 10));
					//�����ĩ�ڵ��
					iJSJDH_end = atoi(Mid(temp, 31, 10));
					if( iFlag == 0 ){
						//��һ���ڵ��¼ֻҪ�򵥵ر���ʼĩ�ڵ�
						iJSJDH.SetAtGrow(iCount,iJSJDH_start);
						sngDW.SetAtGrow(iCount,sngDWTemp);
						Tj.SetAtGrow(iCount,TjTemp);
						iCount++;
						
                			pos=f.tellg();
			                int iFlg = 0;
							while(!f.eof() && iFlg==0)
							{
								f.getline(tempsign.GetBuffer(255),255);
								tempsign.ReleaseBuffer();
								if (atoi(Mid(tempsign, 16, 4))==10)
								{
									break;
								}
								if(atoi(Mid(tempsign, 16, 4))==iNCtj)
								{
                                    long Ppos;
                                    float fTjTemp = atof(Mid(tempsign, 32, 7));
									while (!f.eof() && iFlg==0)
									{
                                        Ppos = f.tellg();
								        f.getline(tempsign.GetBuffer(255),255);
								        tempsign.ReleaseBuffer();
										int ipipetype = atoi(Mid(tempsign, 16, 4));
										if (ipipetype == iNCtj)
										{
											f.seekg(Ppos);//ָ��ص�ԭ��λ��
											break;
										}
        								if (ipipetype == 10)
										{
											iFlg = 1;
									        break;
										}
										if (ipipetype==0 || ipipetype==1 || ipipetype==2 ||ipipetype==3 ||
											ipipetype==4 || ipipetype==5 || ipipetype==6)
										{
											TjTemp = fTjTemp;
										}
									}
								}
							}
							f.seekg(pos);//ָ��ص�ԭ��λ��
               
						iJSJDH.SetAtGrow(iCount,iJSJDH_end);
						sngDW.SetAtGrow(iCount,sngDWTemp);
						Tj.SetAtGrow(iCount,TjTemp);
						iCount++;
					}else{
						for( i = 1 ;i<iCount;i++)
						{
							if( iJSJDH_end == iJSJDH[i] ) break;
						}
						if( i >= iCount ){

                			pos=f.tellg();
			                int iFlg = 0;
							while(!f.eof() && iFlg==0)
							{
								f.getline(tempsign.GetBuffer(255),255);
								tempsign.ReleaseBuffer();
								if (atoi(Mid(tempsign, 16, 4))==10)
								{
									break;
								}
								if(atoi(Mid(tempsign, 16, 4))==iNCtj)
								{
                                    long Ppos;
                                    float fTjTemp = atof(Mid(tempsign, 32, 7));
									while (!f.eof() && iFlg==0)
									{
                                        Ppos = f.tellg();
								        f.getline(tempsign.GetBuffer(255),255);
								        tempsign.ReleaseBuffer();
										int ipipetype = atoi(Mid(tempsign, 16, 4));
										if (ipipetype == iNCtj)
										{
											f.seekg(Ppos);//ָ��ص�ԭ��λ��
											break;
										}
        								if (ipipetype == 10)
										{
											iFlg = 1;
									        break;
										}
										if (ipipetype==0 || ipipetype==1 || ipipetype==2 ||ipipetype==3 ||
											ipipetype==4 || ipipetype==5 || ipipetype==6)
										{
											TjTemp = fTjTemp;
										}
									}
								}
							}
							f.seekg(pos);//ָ��ص�ԭ��λ��

							iJSJDH.SetAtGrow(iCount,iJSJDH_end);
							sngDW.SetAtGrow(iCount,sngDWTemp);
							Tj.SetAtGrow(iCount,TjTemp);
							iCount++;
						}
					}
					//���ñ�־�����Ƿ��ǵ�һ���ڵ��¼
					if( iFlag == 0 ) iFlag = 1;
				}
			}while(Trim(Mid(temp, 18)).Find(strEND_RawData)!=0 && !f.eof());
			if( Trim(Mid(temp, 18)).Find(strEND_RawData)==0) break;
		}
      long ltemp=maxZdjh ;  
		rsData=rsResult;
   
		//�ܵ��˵�����������ļ�ǰ��
		//GLIFV3.x:��ʼ���ҹܵ��˵���Ⱥ�������ͷ��
		/*mvsCASE = strREACTION_ON_ANCHORS_WorkInitial_CASE2;
		while(!f.eof()){
			f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();

			if(strTemp.Find(mvsCASE) !=-1){
				break;
			}
		}            
		//rsResult.Requery
		//GLIFV3.x:��ʼ���ܵ��˵���Ⱥ�������
		while(!f.eof()){
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			if( atof(Mid(temp, 0, 16)) > 0 ){
				iPreINT[1] = atoi(Mid(temp, 0, 16));   //�ڵ��
				iPreINT[2] = atoi(Mid(temp, 16, 10));  //�ڵ�����
				//�˴�ֻ���ǹ̶�������
				iPreINT[2] = 100 ; //or 177
				for (j = 1 ;j<= 6;j++) //ÿ�����6������,����ΪFx,Fy,Fz,Mx,My,Mz
					phsVAL[j][1] = atof(Mid(temp, 26 + (j - 1) * 15-1, 15));
				

				rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(iPreINT[1]));
					 if(rsData->BOF && rsData->adoEOF ){
						  rsData->AddNew();
						  rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						  rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						  rsData->PutCollect("Zdjh",_variant_t((long)(ltemp+1)));
						 //rsData->PutCollect("Zdjh",_variant_t((long)iPreINT[1]));
						   rsData->Update();
						  ltemp++;
					 }else{
					 }
					 for (i = 1 ;i< iCount;i++)
					 {
						 if( iJSJDH[i] == iPreINT[1] )break;
					 }
					 rsData->PutCollect("PSAver", _variant_t(strPSAVER));
					 rsData->PutCollect("JSDBH1", _variant_t((long)iPreINT[1]));
					 rsData->PutCollect("Type", _variant_t((long)iPreINT[2]));
					 if( EDIBgbl::PSA_OutDataUnit== "NATIONAL" ){
						rsData->PutCollect("Unit_Force", "N");
						rsData->PutCollect("Unit_Momental", "N.m");
					 }else{
						rsData->PutCollect("Unit_Force", "kgf");
						rsData->PutCollect("Unit_Momental", "kgf.m");
					 }
					 rsData->PutCollect("Unit_Displacement", "mm");
					 rsData->PutCollect("pxr1", _variant_t(phsVAL[1][1]));
					 rsData->PutCollect("pyr1", _variant_t(phsVAL[2][1]));
					 rsData->PutCollect("pgz1", _variant_t(phsVAL[giUPxyz][1]));
					 rsData->PutCollect("pjg1", _variant_t(phsVAL[giUPxyz][1]));
					 rsData->PutCollect("pzr1", _variant_t(phsVAL[3][1]));
					 rsData->PutCollect("mxr1", _variant_t(phsVAL[4][1]));
					 rsData->PutCollect("myr1", _variant_t(phsVAL[5][1]));
					 rsData->PutCollect("mzr1", _variant_t(phsVAL[6][1]));
					 rsData->Filter=_variant_t((long)adFilterNone);
				iC1++;
			}else{
				if( temp.Find(mvsCASE) == -1 ){
					break;
				}
			}
		}
		//�������ȹ���4�ܵ��˵���ش���
		f.close();
   
		//�ܵ��˵㹤��1��̬�������ļ�ǰ��
		//�ܵ��˵㹤��3/4��̬�����ڹ���2��
		//GLIFV3.x:��ʼ���ҹܵ��˵���̬��������ͷ��
		f.open(SourceDataFileName);
		while(!f.eof()){
			f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
			if( strTemp.Find(mvsCASEcool) !=-1){ //�ҵ��ܵ��˵���̬��������ͷ��
				break;
			}
		}
                     
		//rsResult.Requery
		//Set rsData = rsResult
   
		//GLIFV3.x:��ʼ���ܵ��˵���̬��������
		while(!f.eof()){
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			
			if( atof(Mid(temp, 0, 16)) > 0 ){
				iPreINT[1] = atoi(Mid(temp, 0, 16))  ; //�ڵ��
				iPreINT[2] = atoi(Mid(temp, 16, 10)) ; //�ڵ�����
				//�˴�ֻ���ǹ̶�������
				iPreINT[2] = 100 ; //or 177
				for( j = 1 ;j<= 6;j++) //ÿ�����6������,����ΪFx,Fy,Fz,Mx,My,Mz
					phsVAL[j][1] = atof(Mid(temp, 26 + (j - 1) * 15-1, 15));
				rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(iPreINT[1]));
				
					 if( rsData->BOF && rsData->adoEOF ){
						  rsData->AddNew();
						  rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						  rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						  rsData->PutCollect("Zdjh",_variant_t((long)(ltemp+1)));
						 //rsData->PutCollect("Zdjh",_variant_t((long)iPreINT[1]));
						   rsData->Update();
						  ltemp++;
					 }else{
					 }
					 for(i = 1;i<iCount;i++)
					 {
						 if( iJSJDH[i] == iPreINT[1])
							 break;
					 }
					 rsData->PutCollect("PSAver",_variant_t(strPSAVER));
					 rsData->PutCollect("JSDBH1",_variant_t((long)iPreINT[1]));
					 rsData->PutCollect("Type",_variant_t((long)iPreINT[2]));
					 if( EDIBgbl::PSA_OutDataUnit == "NATIONAL" ){
						rsData->PutCollect("Unit_Force", "N");
						rsData->PutCollect("Unit_Momental", "N.m");
					 }else{
						rsData->PutCollect("Unit_Force", "kgf");
						rsData->PutCollect("Unit_Momental", "kgf.m");
					 }
					 rsData->PutCollect("Unit_Displacement", "mm");
					 rsData->PutCollect("pxl1", _variant_t(phsVAL[1][1]));
					 rsData->PutCollect("pyl1", _variant_t(phsVAL[2][1]));
					 rsData->PutCollect("paz0", _variant_t(phsVAL[giUPxyz][1]));
					 rsData->PutCollect("paz1", _variant_t(phsVAL[giUPxyz][1]));
					 rsData->PutCollect("pzl1", _variant_t(phsVAL[3][1]));
					 rsData->PutCollect("mxl1", _variant_t(phsVAL[4][1]));
					 rsData->PutCollect("myl1", _variant_t(phsVAL[5][1]));
					 rsData->PutCollect("mzl1", _variant_t(phsVAL[6][1]));
					 rsData->Filter=_variant_t((long)adFilterNone);
				//�ܵ��˵���̬���ر�������̬������ͬһ����¼�����Լ�¼�Ų��ܵ�����
				//iC1 = iC1 + 1
			}else{
				if( temp.Find(mvsCASEcool)==-1 ){
					break;
				}
			}
		}
		//�����ܵ��˵���̬���ش���*/
		f.close();
   
		f.open(SourceDataFileName);
		//GLIFV3.x:��ʼ����λ������ͷ��
		while(!f.eof()){
			f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
				if(strTemp.Find(strCW_DISPLACEMENT) !=-1 ) //�ҵ�"CW-DISPLACEMENT (cold/work-status)"
					break;
		}
   
		//�ҵ�"CW-DISPLACEMENT (cold/work-status)"
		//GLIFV3.x:��ʼ��λ������
		while(!f.eof()){
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			//���ϵ���Ժ1999��5��ǰʹ�õ�GLIFV3.x�汾��iPreINT(1)���ַ���ȴӵ�1-10λ
			//���ݵ���Ժ2001��5��ǰʹ�õ�GLIFV3.1�汾��iPreINT(1)���ַ���ȴӵ�1-5λ��
			//iPreINT(2)���ַ���ȴӵ�6-10λ��
			if( atof(Mid(temp, 0, 10)) >0)
			{
				if( gbNewGLIFV31 ){
					iPreINT[1] = atoi(Mid(temp,0, 5)) ;     //�ڵ��
					iPreINT[2] = atoi(Mid(temp, 5, 7)) ;     //�ڵ�����
				}else{
					iPreINT[1] = atoi(Mid(temp, 0, 10));     //�ڵ��
					iPreINT[2] = atoi(Mid(temp, 10, 10));    //�ڵ�����
				}
				for(j = 1;j<= 6;j++) //ÿ�����6������,����Ϊdcx,dcy,dcz,dwx,dwy,dwz,SP-Set,SP-Press
				{
					if( gbNewGLIFV31 ){
						phsVAL[j][1] = atof(Mid(temp, (j <= 3 ? 13 : 35 - 3 * 7) + (j - 1) * 7-1, 7));
					}else{
						phsVAL[j][1] =atof(Mid(temp, (j <= 3 ? 51 : 83 - 4 * 7) + (j - 1) * 7 -1, 7));
					}
				}
				rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(iPreINT[1])+" AND "+CString("[FileNameID]=")+ltos(FileNameID));
				//MsgBox rsData.RecordCount
					 if( rsData->BOF && rsData->adoEOF ){
						  rsData->AddNew();
						  rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						  rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						  rsData->PutCollect("Zdjh",_variant_t((long)(ltemp+1)));
						  rsData->PutCollect("FileNameID",_variant_t(FileNameID));
						 //rsData->PutCollect("Zdjh",_variant_t((long)iPreINT[1]));
						   rsData->Update();
						  ltemp++;
					 }else{
					 }
					 for(i = 1 ;i<iCount;i++)
					 {
						 //if( iJSJDH(i) = 85 ){ Debug.Print i,iPreINT(1), iJSJDH(i), sngDW(i), Tj(i)
						 if( iJSJDH[i] == iPreINT[1] ) break;
					 }
					 rsData->PutCollect("JSDBH1", _variant_t((long)iPreINT[1]));
					 rsData->PutCollect("dj1", _variant_t(sngDW[i]));
					
					 //chengbailian
					 POSITION pos=mElevationList.GetHeadPosition();
					 while(pos !=NULL)
					 {
					     //���ҵ�ǰ֧����ı��	
						 pNodeUpElevation = mElevationList.GetNext(pos);

						 if(pNodeUpElevation->NodeNum == iPreINT[1])
						 {
							 sngDH1.SetAtGrow(i,pNodeUpElevation->UpElevation); 
							 sngDH1[i]=int(sngDH1[i]*1000+0.5)/1000.0; //��ȷ��λС��
							 break;
						 }
						 
					 }

					 if(vtof(rsData->GetCollect("dh1"))==0.0)//���ԭ�������ĸ�
						rsData->PutCollect("dh1", _variant_t(sngDH1[i]));
                     //end cbl

					 rsData->PutCollect("t01", _variant_t(Tj[i]));
					 rsData->PutCollect("xl1", _variant_t(phsVAL[1][1]));
					 rsData->PutCollect("yl1", _variant_t(phsVAL[2][1]));
					 rsData->PutCollect("zl1", _variant_t(phsVAL[3][1]));
					 rsData->PutCollect("xr1", _variant_t(phsVAL[4][1]));
					 rsData->PutCollect("yr1", _variant_t(phsVAL[5][1]));
					 rsData->PutCollect("zr1", _variant_t(phsVAL[6][1]));
					 rsData->PutCollect("haz", nil);
					 rsData->Filter=_variant_t((long)adFilterNone);
				iC1++;
				frmStatus.UpdateStatus((float)iC1/(float)iCount,true);
				frmStatus.UpdateWindow();
			}else{
				if( temp.Find(strCW_DISPLACEMENT) ==-1 ){
					break;
				}
			}
			
			if( iCount <= 0 ) iCount = 1;
			//MsgBox iC1 & ":" & iCount
			frmStatus.UpdateStatus((float)iC1/(float)iCount,false);
			frmStatus.UpdateWindow();
			//frmStatus.picstatus.Refresh
			//frmStatus.Refresh
		}
		f.close();
   
		f.open(SourceDataFileName);
		//GLIFV3.x:���Һ�������ͷ
		while(!f.eof()){
			f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
			if( strTemp.Find(strSTRUCTURE_LOAD_OF_RESTRAINTS) !=-1 ){ //�ҵ�"STRUCTURE LOAD OF RESTRAINTS"
				break;
			}
		}
                     
		//�ҵ�"STRUCTURE LOAD OF RESTRAINTS"
		//GLIFV3.x:��ʼ����������
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));
		strSQL.Format("SELECT G,SEQ FROM %s",strSprTbn);
		rs1->Open((_bstr_t)strSQL, _variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
			adOpenKeyset, adLockOptimistic, adCmdText); 
		while(!f.eof()){
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			if( atof(Mid(temp,0, (gbNewGLIFV31 ? 5 : 16))) > 0 ){
				if( gbNewGLIFV31 ){
					iPreINT[1] = atoi(Mid(temp, 0, 5));      //�ڵ��
					iPreINT[2] = atoi(Mid(temp, 5, 5));      //�ڵ�����
				}else{
					iPreINT[1] = atoi(Mid(temp, 0, 16));   //�ڵ��
					iPreINT[2] = atoi(Mid(temp, 16, 10));  //�ڵ�����
				}
				if( iPreINT[2] < 1000 && iPreINT[2] != (100 + giUPxyz * 10) && iPreINT[2] != (200 + giUPxyz * 10) && iPreINT[2] != (300 + giUPxyz * 10) ){
					//X/Y/Z�����ϣ��յ���������Ϊ110/120/130(��������ϵ)��210/220/230(��Ԫ����ϵ)��310/320/330(�ض�������������)
					//Debug.Print temp
					//�ǵ����ҷǸ��Ե���
					//����֧������6������,����ΪFx,Fy,Fz,Mx,My,Mz
					//�ȴ����һ��
					i = 1;
					for( j = 1 ;j<=6;j++) //ÿ�����6������,����Ϊ1C-Load(����1�������),IN-Load(��װ����),WK-Load(��������),CR-LOAD/S-SCALE(�������/�ṹ����ϵ��),WT-LOAD(ˮ����),ST-LOAD(�ṹ����)
					{
						if( gbNewGLIFV31 ){
							phsVAL[j][i] = atof(Mid(temp, 11 + (j - 1) * 12 -1, 12));
							mvMultiple+=phsVAL[j][1];
						}else{
							phsVAL[j] [i] = atof(Mid(temp, 26 + (j - 1) * 15 -1, 15));
							mvMultiple+=phsVAL[j][1];
						}
					}
					f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
            
					for(i = 2 ;i<= 6;i++)
					{
						//�ٶ�5��
						f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
						//f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
						
						mvMultiple = 0;
						for(j = 1 ;j<= 6;j++) //ÿ�����6������,����Ϊ1C-Load(����1�������),IN-Load(��װ����),WK-Load(��������),CR-LOAD/S-SCALE(�������/�ṹ����ϵ��),WT-LOAD(ˮ����),ST-LOAD(�ṹ����)
						{
							if( gbNewGLIFV31 ){
								phsVAL[j][i] = atof(Mid(temp, 10 + (j - 1) * 12, 12));
								mvMultiple+=phsVAL[j][i];
							}else{
								phsVAL[j][i] =atof(Mid(temp, 26 + (j - 1) * 15 -1, 15));
								mvMultiple+=phsVAL[j][i];
							}
						}
						//���6�����ݲ�ȫΪ0
						//if( mvMultiple <> 0 ){
							//sFx������֧���ܽڵ������ķ���
						 //  sFx = Mid(temp, Iif((gbNewGLIFV31, 85, 124), 1)
						  // Exit For
						//}else{
							//���ϵ���Ժ1999��5��ǰʹ�õ�GLIFV3.x�汾��ÿ��������֮���һ��
							//���ݵ���Ժ2001��5��ǰʹ�õ�GLIFV3.1�汾������Ժʹ�õĺų�GLIFv3.2�汾��ÿ��������֮�󲻿���
							//f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();  //����
							//Debug.Print temp
						//}
					}
				}else{
					//����֧����ֻ��1������:���Ի���Ե���
					for(j = 1 ;j<=6;j++) //ÿ�����6������,����Ϊ1C-Load(����1�������),IN-Load(��װ����),WK-Load(��������),CR-LOAD/S-SCALE(�������/�ṹ����ϵ��),WT-LOAD(ˮ����),ST-LOAD(�ṹ����)
					{
						if( gbNewGLIFV31 ){
							phsVAL[j][1]= atoi(Mid(temp, 11 + (j - 1) * 12-1, 12));
							mvMultiple+=phsVAL[j][1];
						}else{
							phsVAL[j][1] =atoi(Mid(temp, 26 + (j - 1) * 15-1, 15));
							mvMultiple+=phsVAL[j][1];
						}
					}
				}
				rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(iPreINT[1]) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));
				//MsgBox rsData.RecordCount
					 if( rsData->BOF && rsData->adoEOF ){
						  rsData->AddNew();
						  rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						  rsData->PutCollect("VolumeID", _variant_t(EDIBgbl::SelVlmID));
						  rsData->PutCollect("Zdjh",_variant_t((long)(ltemp+1)));
						 //rsData->PutCollect("Zdjh",_variant_t((long)iPreINT[1]));
						   rsData->Update();
						  ltemp++;
					 }else{
					 }
					 for(i = 1 ;i<iCount;i++)
					 {
						 if( iJSJDH[i] == iPreINT[1] ) break;
					 }
					 rsData->PutCollect("PSAver",_variant_t(strPSAVER));
					 if( iPreINT[2] > 1000 && iPreINT[2] < 5000 ){
					 //����֧���ܲ�����
						rsData->PutCollect("ParallelingNum",_variant_t((long)atoi(Mid(ltos(iPreINT[2]), 1, 1))));
					 }
					 rsData->PutCollect("JSDBH1",_variant_t((long)iPreINT[1]));
					 rsData->PutCollect("Type", _variant_t((long)iPreINT[2]));
					 rsData->PutCollect("psaType", _variant_t((long)iPreINT[2]));
					 if( EDIBgbl::PSA_OutDataUnit=="NATIONAL" ){
						rsData->PutCollect("Unit_Force", "N");
						rsData->PutCollect("Unit_Momental", "N.m");
					 }else{
						rsData->PutCollect("Unit_Force", "kgf");
						rsData->PutCollect("Unit_Momental", "kgf.m");
					 }
					 rsData->PutCollect("Unit_Displacement", "mm");
					 if( iPreINT[2] < 1000 && iPreINT[2] != (100 + giUPxyz * 10) && iPreINT[2] != (200 + giUPxyz * 10) && iPreINT[2] != (300 + giUPxyz * 10) )
					 {
						sFx=Mid(ltos(iPreINT[2]),1, 1);     //��λ��ʾ������λ��
						sFx1=Mid(ltos(iPreINT[2]), 2, 1);  //��λ��ʾ���ƽ�λ��
               
							rsData->PutCollect("paz0", _variant_t(phsVAL[giUPxyz][1]));
							rsData->PutCollect("paz1", _variant_t(phsVAL[giUPxyz][1]));
							rsData->PutCollect("pxl1", _variant_t(phsVAL[1][1]));
							rsData->PutCollect("pyl1", _variant_t(phsVAL[1][2]));
							rsData->PutCollect("pzl1", _variant_t(phsVAL[1][3]));
							rsData->PutCollect("pxr1", _variant_t(phsVAL[3][1]));
							rsData->PutCollect("pyr1", _variant_t(phsVAL[3][2]));
							rsData->PutCollect("pzr1", _variant_t(phsVAL[3][3]));
							rsData->PutCollect("pgz1", _variant_t(phsVAL[3][2]));
							rsData->PutCollect("pjg1", _variant_t(phsVAL[6][2]));
							rsData->PutCollect("mxl1", _variant_t(phsVAL[1][4]));
							rsData->PutCollect("myl1", _variant_t(phsVAL[1][5]));
							rsData->PutCollect("mzl1", _variant_t(phsVAL[1][6]));
							rsData->PutCollect("mxr1", _variant_t(phsVAL[3][4]));
							rsData->PutCollect("myr1", _variant_t(phsVAL[3][5]));
							rsData->PutCollect("mzr1", _variant_t(phsVAL[3][6]));
					 }else{
						//���ɻ�յ�
						rsData->PutCollect("paz0", _variant_t(phsVAL[2][1]));
						rsData->PutCollect("paz1", _variant_t(phsVAL[2][1]));
						rsData->PutCollect("pgz1", _variant_t(phsVAL[3][1]));
						rsData->PutCollect("pjg1", _variant_t(phsVAL[6][1]));
						if(giUPxyz==1)
						{
							rsData->PutCollect("pxl1", _variant_t(phsVAL[2][1]));
							rsData->PutCollect("pxr1", _variant_t(phsVAL[3][1]));
						}
						else if(giUPxyz==2)
						{
							rsData->PutCollect("pyl1", _variant_t(phsVAL[2][1]));
							rsData->PutCollect("pyr1", _variant_t(phsVAL[3][1]));
						}
						else						
						{
							rsData->PutCollect("pzl1", _variant_t(phsVAL[2][1]));
							rsData->PutCollect("pzr1", _variant_t(phsVAL[3][1]));
						}
						float fG,fR;int iSerialNum,iParelleNum,iSEQ;
						if(iPreINT[2]>1000 && iPreINT[2]<5000)
						{
							CString strTmp;
							strTmp.Format("%d",iPreINT[2]);
							iSerialNum=atoi(strTmp.Left(1));
							iParelleNum=atoi(strTmp.Mid(1,1));
							iSEQ=atoi(strTmp.Right(2));
							_variant_t vTmp;
							rs1->Find((_bstr_t)("SEQ=" + ltos(iSEQ)), 0, adSearchForward);
							if(!rs1->adoEOF)
							{
								rs1->get_Collect((_variant_t)"G", &varTmp);
								fG=vtof(varTmp);
								fR=fabs(phsVAL[2][1]) - fabs(phsVAL[3][1]);
								if(EDIBgbl::PSA_OutDataUnit== "NATIONAL")
									fR/=EDIBgbl::kgf2N;
								fR/=iParelleNum;
								fR/=(fG/iSerialNum);
								if(giUPxyz==1)
									rsData->PutCollect("xr1", _variant_t(fR));
								else if(giUPxyz==2)
									rsData->PutCollect("yr1", _variant_t(fR));
								else
									rsData->PutCollect("zr1", _variant_t(fR));
							}
						}
					 }
					 rsData->Update();
					 rsData->Filter=_variant_t((long)adFilterNone);
				iC1++;
			}else{
				if( temp.Find(strSTRUCTURE_LOAD_OF_RESTRAINTS) ==-1 ){
					break;
				}
			}
			
			if( iCount <= 0 ) iCount = 1;
			//MsgBox iC1 & ":" & iCount
			frmStatus.UpdateStatus((float)iC1/(float)iCount,true);
			frmStatus.UpdateWindow();
			//frmStatus.picstatus.Refresh
			//frmStatus.Refresh
		}
		f.close();

		
		//�ͷ��ڴ� cbl
		POSITION posList=mElevationList.GetHeadPosition();
		POSITION preposList;
		CNodeUpElevation *pNodeUpElevationData;
		while(posList !=NULL)
		{
			preposList=posList;
			pNodeUpElevationData = mElevationList.GetNext(posList);
            mElevationList.RemoveAt(preposList);	
			delete pNodeUpElevationData;
		}

		

		if(!rsData->adoEOF || !rsData->BOF)
			rsData->MoveFirst();
		_variant_t vtmp;
		while(!rsData->adoEOF)
		{
			vtmp=rsData->GetCollect("ZDJH");
			rsData->PutCollect("ZDJH",_variant_t((long)(vtoi(vtmp)+1000)));
			rsData->MoveNext();
		}
		_ConnectionPtr pCon;
		strSQL=(char*)(bstr_t)rsData->GetSource();
		pCon=rsData->GetActiveConnection();
		strSQL.Replace("ORDER BY JSDBH1","ORDER BY FileNameID");
		rsData->Close();
		rsData->Open(_variant_t(strSQL),(IDispatch*)pCon,adOpenKeyset,adLockOptimistic,adCmdText);
		//rsData->Sort=_bstr_t("JSDBH1");
		int itmpZdjh=1;
		while(!rsData->adoEOF)
		{
			rsData->PutCollect("ZDJH",_variant_t((long)itmpZdjh));
			itmpZdjh++;
			rsData->MoveNext();
		}
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
	}
}






void ReadResult_GLIF31New(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID)
{
	  //Remauther:
		//����:
		//Ŀ��:������Ϊ��*.out�ļ�����������msgbox��������Գ������ȷ��
		//����:rsResult���ݱ��ڲ���λ:λ��mm,����kgf.
		//����:
   try
	{
		_variant_t nil;
		nil.ChangeType(VT_NULL);
		 CString strOldPhsSPRINGSel; //�����ȡӦ����������֮ǰ�ĵ��ɱ�׼
		 float mvMultiple=0;
		 CString sFx, sFx1;
		 CString strTemp;
		 CString temp;
		 int iTemp=0;
		 float phsVAL[7][9]; //������������ĵ��������ݣ�Xά�����������ݺ�����У�
												 //Yά������
		 int iPreINT[3]; //����������¼ǰ�������������
		 _Recordset* rsData;

		LPCTSTR strPIPING_STRESS_OUTUNIT = " + ";   
		LPCTSTR strSTRUCTURE_LOAD_OF_RESTRAINTS = "STRUCTURE LOAD OF RESTRAINTS";
		LPCTSTR strREACTION_ON_ANCHORS_ColdRelease_CASE3 = "REACTION OF PIPING ON ANCHORS (COLD-RELEASE)";
		
		LPCTSTR strREACTION_ON_ANCHORS_ColdInitial_CASE4;
		LPCTSTR strREACTION_ON_ANCHORS_WorkInitial_CASE2;
		LPCTSTR strREACTION_ON_ANCHORS_Cold_CASE1;
		if(YLJSVer.Left(7)!="AutoPSA")
		{
			strREACTION_ON_ANCHORS_ColdInitial_CASE4 = "REACTION OF PINING ON ANCHORS (COLD-INITIAL)";			
			strREACTION_ON_ANCHORS_WorkInitial_CASE2 = "REACTION OF PINING ON ANCHORS (WORK-INITIAL)";
			strREACTION_ON_ANCHORS_Cold_CASE1 = "REACTION OF PINING ON ANCHORS";
		}
		else
		{
			//UEsoft AutoPSA1.0 correct PINING to PIPING
			strREACTION_ON_ANCHORS_ColdInitial_CASE4 = "REACTION OF PIPING ON ANCHORS (COLD-INITIAL)";
			strREACTION_ON_ANCHORS_WorkInitial_CASE2 = "REACTION OF PIPING ON ANCHORS (WORK-INITIAL)";
			strREACTION_ON_ANCHORS_Cold_CASE1 = "REACTION OF PIPING ON ANCHORS";
		}

		LPCTSTR strCOLD_SPRING_TABLE = "COLD SPRING JOINT POINT";
		LPCTSTR strEND_RawData = "10     1     1    1" ; //�ո������GLIFV1.2��ͬ
		LPCTSTR strPSAVER = "GLIF V31";
   
		LPCTSTR strCW_DISPLACEMENT = "CW-DISPLACEMENT (cold/work-status)";
   
		LPCTSTR strEND_THIS_PIPING = "*THIS PIPING STRUCTURE *CONTAINS*";
		CString strSprTbn;
		_RecordsetPtr tmprs;
		tmprs.CreateInstance(__uuidof(Recordset));
		_variant_t varTmp;
		int fdFlg;
		CString mvsCASE, mvsCASEcool;
		int mviSPR=0;
		long i=0;
		//����ʾ�����֮ǰ���ؽ��ȿ�
		frmStatus.ShowWindow(SW_HIDE);
		CInputBox dlg;
		while(sFx == "" || (sFx.GetAt(0) < '0' || sFx.GetAt(0) > '9')  || ( sFx != "1" && sFx != "3" && sFx != "4"))
		{
			dlg.m_strTitle=GetResStr(IDS_SelThrustQuaGist);
			dlg.m_strWndText=GetResStr(IDS_SelWorkCon);
			dlg.m_strValue="4";
			dlg.DoModal();
			sFx =dlg.m_strValue;
			sFx.TrimLeft();
			sFx.TrimRight();
		}
		i = atoi(sFx);
		if( i == 1 )
		{
			mvsCASEcool = strREACTION_ON_ANCHORS_Cold_CASE1;
		}
		else
		{
			if( i == 3 )
			{
				mvsCASEcool = strREACTION_ON_ANCHORS_ColdRelease_CASE3;
			}
			else
			{
				mvsCASEcool = strREACTION_ON_ANCHORS_ColdInitial_CASE4;
			}
		}
		frmStatus.ShowWindow(SW_SHOW);
		frmStatus.UpdateWindow();
		//�������Լ�¼:GLIF1.2,NC=6;GLIF3.x,NC=60(�䵥Ԫ)��61(ֱ��Ԫ),�ɶ�ʹ��.
		//�������Լ�¼:GLIF1.2,NC=9;GLIF3.x,NC=90(��ƹ���).
		const int iNCtj = 90, iNCdw1 = 61, iNCdw = 60;
   
		float sngDWTemp=0, TjTemp=0;
		long iJSJDH_start=0, iJSJDH_end=0;
		CArray<long,long & > iJSJDH;
		CArray<float,float & > sngDW;
		CArray<float ,float & > Tj;
		CArray<float,float &> sngDH1;

		sngDH1.SetSize(100);
		iJSJDH.SetSize(100);
		sngDW.SetSize(100);
		Tj.SetSize(100);
   
		int j=0, iC1=0;
		ifstream f;
		int iFlag=0, iCount=0;
		CString mvS,tempsign;
		bool bCwDisplacement=false, bStrucLoad=false;
        bool sign=false;//chengbailian 2003.11.6 Ϊ����Ӧ����һ���ļ���90��¼�Ƿ���10��¼֮¼����		
		long pos;
		ElevationList mElevationList;
		CNodeUpElevation *pNodeUpElevation;
		int ICC;
		
		//cbl 2004.1.10 ������ÿ���ڵ�ı��
		//�����ļ���
		CString ElevationFileName=SourceDataFileName.Left(SourceDataFileName.ReverseFind('.'))+".et";//����ļ���

		//���������ļ�(��¼��ÿ���ڵ������)(���ӿ��еĺ���)
		ICC=PSAPointCoordinateWirteFile(SourceDataFileName,ElevationFileName);
	  
		//���ļ��ж�ȡÿ����ı�߷�������mElevationList��
		ElevationFileToList(ElevationFileName,&mElevationList,ICC);
        ::DeleteFile(ElevationFileName); 
	//cbl

		f.open(SourceDataFileName);

		CString strSQL;
		//��ʼ��ԭʼ����
		while(!f.eof())
		{//4��һ�ζ��ı��ļ�ѭ��(start)
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			if( temp.Find(strPIPING_STRESS_OUTUNIT)!=-1)
			{//3
				//��һ����¼��һ���ֶ������뵥λ�ƣ���2���ֶ��������λ��(=0��������λ��)����3���ֶ��������᷽��
				if( atoi(Trim(Mid(temp,8, 4))) == 0 )
				{//1
					EDIBgbl::PSA_OutDataUnit = "NATIONAL";
				}//1
				else
				{//1
					EDIBgbl::PSA_OutDataUnit = "ENGINEERING";
				}//1
				//�����᷽��x/y/z������=1/2/3
				giUPxyz = atoi(Mid(temp,12, 6));

				f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
				//GLIFV3.x��,ԭʼ�����ļ���2����¼��1������Ϊ��������,��3������Ϊ���ɺ��ر仯ϵ��
				if(gbNewGLIFV31)
				{//1
					mviSPR = atoi(Trim(Mid(temp, 3, 6)));
					modPHScal::gnSPR_CHZBH = atof(Trim(Mid(temp, 13, 10)));
				}//1
				else
				{//1
					mviSPR = atoi(Trim(Mid(temp, 6, 10)));
					modPHScal::gnSPR_CHZBH = atof(Trim(Mid(temp, 21, 10)));
				}//1
				strOldPhsSPRINGSel = modPHScal::gsPhsSPRINGSel;
				//��ΪӦ������ѡ��ĵ��ɱ�׼���ܲ�ͬ,�����¸��¹淶
				//�������ɹ淶����ʱ��ϳ�������ֻ�е��淶�仯ʱ������Ҫ���¸���
				if( mviSPR == 3 )
				{//2
					strSQL = "SELECT Spring_property FROM phsManuSPRING WHERE StandardDesc=\'";
					strSQL += strSprNWEPDI;
					strSQL += "\'";
					tmprs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
						adOpenKeyset, adLockOptimistic, adCmdText); 
					if(tmprs->BOF && tmprs->adoEOF)
					{//1
					}//1
					else
					{//1
						tmprs->get_Collect((_variant_t)0L, &varTmp);
						strSprTbn=vtos(varTmp);
					}//1
					tmprs->Close();
					if( strOldPhsSPRINGSel != strSprNWEPDI )
					{//1
						modPHScal::gsPhsSPRINGSel = strSprNWEPDI;
					}//1
				}//2
				else if( mviSPR == 4 )
				{//2
					strSQL = "SELECT Spring_property FROM phsManuSPRING WHERE StandardDesc=\'";
					strSQL += strSprGB10182;
					strSQL += "\'";
					tmprs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
						adOpenKeyset, adLockOptimistic, adCmdText); 
					if(tmprs->BOF && tmprs->adoEOF)
					{//1
					}//1
					else
					{//1
						tmprs->get_Collect((_variant_t)0L, &varTmp);
						strSprTbn=vtos(varTmp);
					}//1
					tmprs->Close();
					if( strOldPhsSPRINGSel != strSprGB10182 )
					{//1
						modPHScal::gsPhsSPRINGSel = strSprGB10182;
					}//1
				}//2         
				//���ر仯ϵ�����浽�ⲿ
				SetRegValue(_T("Settings"), "SPR_Chzbh", (modPHScal::gnSPR_CHZBH));
				break;
			}//3
		}//4��һ�ζ��ı��ļ�ѭ��(end)
   
		iCount = 1;
		while(!f.eof())
		{//9 �����������ݶ��ļ�ѭ��(start)
			iFlag=0;
			do
			{//8
				f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();

				if(temp.IsEmpty() )
					continue;
				
				if( temp.Mid(1,1) != "+")
					continue;
				//�������ԭʼ���ݽ�����־���˳�ѭ��
				if( Trim(Mid(temp,10)).Find( strEND_RawData)==0) break;
				iTemp = atoi(Mid(temp, 8, 4));
				if( iTemp == iNCtj ) 
				{//1
					TjTemp = atof(Mid(temp, 23, 7));//TjTemp�����¶�
					sign=true;
				}//1
				if( (iTemp == iNCdw)  ) 
				{//1
					sngDWTemp = atof(Mid(temp, 13, 7));
					if( iFlag != 0 ) 
						sngDW.SetAtGrow(iCount-1,sngDWTemp);
				}//1
				if( iTemp == 10 )
				{//7 //�ǽڵ��ż�¼(start)
					if(!sign)//chengbailian 2003.11.6
					{//3 //���10��¼���ֶ�90��¼��û�г��֣����ҵ�90��¼��ȡ�����¶ȣ���PSA��������ļ�û����������������ܵ��������������������
						pos=f.tellg();
						while(!f.eof())
						{//2
							f.getline(tempsign.GetBuffer(255),255);
						    tempsign.ReleaseBuffer();

							if(tempsign.IsEmpty() )
								continue;
							
							if( tempsign.Mid(1,1) != "+")
								continue;
								
							if(atoi(Mid(tempsign, 8, 4))==iNCtj)
							{//1
								TjTemp = atof(Mid(tempsign, 23, 7));
								break;
							}//1

						}//2
						f.seekg(pos);//ָ��ص�ԭ��λ��
					}//3
					//�����ʼ�ڵ��
					iJSJDH_start = atoi(Mid(temp, 13, 10));
					//�����ĩ�ڵ��
					iJSJDH_end = atoi(Mid(temp, 20, 10));
					if( iFlag == 0 )
					{//5 //�ǵ�һ���ڵ��¼(start)
						//��һ���ڵ��¼ֻҪ�򵥵ر���ʼĩ�ڵ�
						iJSJDH.SetAtGrow(iCount,iJSJDH_start);
						sngDW.SetAtGrow(iCount,sngDWTemp);
						Tj.SetAtGrow(iCount,TjTemp);
						iCount++;
						
                			pos=f.tellg();
			                int iFlg = 0;
							while(!f.eof() && iFlg==0)
							{//4
								f.getline(tempsign.GetBuffer(255),255);
								tempsign.ReleaseBuffer();

								if(tempsign.IsEmpty() )
									continue;
								
								if( tempsign.Mid(1,1) != "+")
									continue;
								if (atoi(Mid(tempsign, 8, 4))==10)
								{//1
									break;
								}//1
								if(atoi(Mid(tempsign, 8, 4))==iNCtj)
								{//3
                                    long Ppos;
                                    float fTjTemp = atof(Mid(tempsign, 23, 7));
									while (!f.eof() && iFlg==0)
									{//2
                                        Ppos = f.tellg();
								        f.getline(tempsign.GetBuffer(255),255);
								        tempsign.ReleaseBuffer();

										if(tempsign.IsEmpty() )
											continue;
										
										if( tempsign.Mid(1,1) != "+")
											continue;
									
										int ipipetype = atoi(Mid(tempsign, 8, 4));
										if (ipipetype == iNCtj)
										{//1
											f.seekg(Ppos);//ָ��ص�ԭ��λ��
											break;
										}//1
        								if (ipipetype == 10)
										{//1
											iFlg = 1;
									        break;
										}//1
										if (ipipetype==0 || ipipetype==1 || ipipetype==2 ||ipipetype==3 ||
											ipipetype==4 || ipipetype==5 || ipipetype==6)
										{//1
											TjTemp = fTjTemp;
										}//1
									}//2
								}//3
							}//4
							f.seekg(pos);//ָ��ص�ԭ��λ��


						iJSJDH.SetAtGrow(iCount,iJSJDH_end);
						sngDW.SetAtGrow(iCount,sngDWTemp);
						Tj.SetAtGrow(iCount,TjTemp);
						iCount++;
					}//5 //�ǵ�һ���ڵ��¼(end)
					else
					{//6 //���ǵ�һ���ڵ��¼(start)
						for( i = 1 ;i<iCount;i++)
						{//1
							if( iJSJDH_end == iJSJDH[i] ) break;
						}//1
						if( i >= iCount )
						{//5
                            pos=f.tellg();
			                int iFlg = 0;
							while(!f.eof() && iFlg==0)
							{//4
								f.getline(tempsign.GetBuffer(255),255);
								tempsign.ReleaseBuffer();
								
								if(tempsign.IsEmpty() )
									continue;
								
								if( tempsign.Mid(1,1) != "+")
									continue;

								if (atoi(Mid(tempsign, 8, 4))==10)
								{//1
									break;
								}//1
								if(atoi(Mid(tempsign, 8, 4))==iNCtj)
								{//3
                                    long Ppos;
                                    float fTjTemp = atof(Mid(tempsign, 23, 7));
									while (!f.eof() && iFlg==0)
									{//2
                                        Ppos = f.tellg();
								        f.getline(tempsign.GetBuffer(255),255);
								        tempsign.ReleaseBuffer();
										
										if(tempsign.IsEmpty() )
											continue;
										
										if( tempsign.Mid(1,1) != "+")
											continue;

										int ipipetype = atoi(Mid(tempsign, 8, 4));
										if (ipipetype == iNCtj)
										{//1
											f.seekg(Ppos);//ָ��ص�ԭ��λ��
											break;
										}//1
        								if (ipipetype == 10)
										{//1
											iFlg = 1;
									        break;
										}//1
										if (ipipetype==0 || ipipetype==1 || ipipetype==2 ||ipipetype==3 ||
											ipipetype==4 || ipipetype==5 || ipipetype==6)
										{//1
											TjTemp = fTjTemp;
										}//1
									}//2
								}//3
							}//4
							f.seekg(pos);//ָ��ص�ԭ��λ��

							
							iJSJDH.SetAtGrow(iCount,iJSJDH_end);
							sngDW.SetAtGrow(iCount,sngDWTemp);
							Tj.SetAtGrow(iCount,TjTemp);
							iCount++;
						}//5
					}//6 //���ǵ�һ���ڵ��¼(end)
					//���ñ�־�����Ƿ��ǵ�һ���ڵ��¼
					if( iFlag == 0 ) iFlag = 1;
				}//7 //�ǽڵ��ż�¼(end)
			}while(Trim(Mid(temp, 10)).Find(strEND_RawData)!=0 && !f.eof());//8
			if( Trim(Mid(temp, 10)).Find(strEND_RawData)==0) break;
		}//9 �����������ݶ��ļ�ѭ��(end)
   
		frmStatus.UpdateStatus(0.02f,true);
		frmStatus.UpdateWindow();
		long ltemp=maxZdjh;
		rsData=rsResult;
		//֧���ܵ㲻�ð����˵�
		//�ܵ��˵�����������ļ�ǰ��
		//GLIFV3.x:��ʼ���ҹܵ��˵���Ⱥ�������ͷ��
		f.close();
		f.open(SourceDataFileName);
		//GLIFV3.x:��ʼ����λ������ͷ��
		while(!f.eof())
		{
			f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
			if(strTemp.Find(strCW_DISPLACEMENT) !=-1 )
			{ //�ҵ�"CW-DISPLACEMENT (cold/work-status)"
				break;
			}

		}
   
		//�ҵ�"CW-DISPLACEMENT (cold/work-status)"
		//GLIFV3.x:��ʼ��λ������
		fdFlg=0;
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			//���ϵ���Ժ1999��5��ǰʹ�õ�GLIFV3.x�汾��iPreINT(1)���ַ���ȴӵ�1-10λ
			//���ݵ���Ժ2001��5��ǰʹ�õ�GLIFV3.1�汾��iPreINT(1)���ַ���ȴӵ�1-5λ��
			//iPreINT(2)���ַ���ȴӵ�6-10λ��
			if( atof(Mid(temp, 0, 5)) >0 )
			{
				fdFlg=1;
				if( gbNewGLIFV31 )
				{
					iPreINT[1] = atoi(Mid(temp,0, 5)) ;     //�ڵ��
					iPreINT[2] = atoi(Mid(temp, 5, 7)) ;     //�ڵ�����
				}
				else
				{
					iPreINT[1] = atoi(Mid(temp, 0, 10));     //�ڵ��
					iPreINT[2] = atoi(Mid(temp, 10, 10));    //�ڵ�����
				}
				for(j = 1;j<= 6;j++) //ÿ�����6������,����Ϊdcx,dcy,dcz,dwx,dwy,dwz,SP-Set,SP-Press
				{
					if( gbNewGLIFV31 )
					{
						phsVAL[j][1] = atof(Mid(temp, (j <= 3 ? 13 : 35 - 3 * 7) + (j - 1) * 7-1, 7));
					}
					else
					{
						phsVAL[j][1] =atof(Mid(temp, (j <= 3 ? 51 : 83 - 4 * 7) + (j - 1) * 7 -1, 7));
					}
				}
				rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(iPreINT[1])+" AND "+CString("[FileNameID]=")+ltos(FileNameID));
				//MsgBox rsData.RecordCount
					 if( rsData->BOF && rsData->adoEOF )
					 {
						  rsData->AddNew();
						  rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						  rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						  rsData->PutCollect("Zdjh",_variant_t((long)(ltemp+1)));
						  rsData->PutCollect("FileNameID",_variant_t(FileNameID));
						  rsData->PutCollect("PSACoefofVarLoad",_variant_t(modPHScal::gnSPR_CHZBH));//pfg20051019 //����Ӧ������ĵ��ɺ��ر仯ϵ��
						   rsData->Update();
						  ltemp++;
					 }
					 else
					 {
					 }
					 for(i = 1 ;i<iCount;i++)
					 {
						 if( iJSJDH[i] == iPreINT[1] ) break;
					 }
					 rsData->PutCollect("JSDBH1", _variant_t((long)iPreINT[1]));
					 rsData->PutCollect("dj1", _variant_t(sngDW[i]));
                     
					 POSITION pos=mElevationList.GetHeadPosition();
					 while(pos !=NULL)
					 {
					     //���ҵ�ǰ֧����ı��	
						 pNodeUpElevation = mElevationList.GetNext(pos);

						 if(pNodeUpElevation->NodeNum == iPreINT[1])
						 {
							 sngDH1.SetAtGrow(i,pNodeUpElevation->UpElevation); 
							 sngDH1[i]=int(sngDH1[i]*1000+0.5)/1000.0; //��ȷ��λС��
							 break;
						 }
						 
					 }

					 rsData->PutCollect("dh1", _variant_t(sngDH1[i]));
					 rsData->PutCollect("t01", _variant_t(Tj[i]));
					 rsData->PutCollect("xl1", _variant_t(phsVAL[1][1]));
					 rsData->PutCollect("yl1", _variant_t(phsVAL[2][1]));
					 rsData->PutCollect("zl1", _variant_t(phsVAL[3][1]));
					 rsData->PutCollect("xr1", _variant_t(phsVAL[4][1]));
					 rsData->PutCollect("yr1", _variant_t(phsVAL[5][1]));
					 rsData->PutCollect("zr1", _variant_t(phsVAL[6][1]));
					 rsData->PutCollect("haz", nil);
					 rsData->Filter=_variant_t((long)adFilterNone);
				iC1++;
				frmStatus.UpdateStatus((float)iC1/(float)iCount,true);
				frmStatus.UpdateWindow();
			}
			else
			{
				if( fdFlg==1)
				{
					do
					{
						if(temp.Find(strCW_DISPLACEMENT) != -1 )
						{
							break;
						}
						f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
					}while(!f.eof());
					fdFlg=0;
				}
			}
			
			if( iCount <= 0 ) iCount = 1;
			//MsgBox iC1 & ":" & iCount
			frmStatus.UpdateStatus((float)iC1/(float)iCount,false);
			frmStatus.UpdateWindow();
		}
		f.close();
		//////////////////////////////////cbl
		//�ͷ��ڴ�
		POSITION posList=mElevationList.GetHeadPosition();
		POSITION preposList;
		CNodeUpElevation *pNodeUpElevationData;
		while(posList !=NULL)
		{
			preposList=posList;
			pNodeUpElevationData = mElevationList.GetNext(posList);
            mElevationList.RemoveAt(preposList);
			delete pNodeUpElevationData;
		}
		/////////////////////////////////////////////
   
		f.open(SourceDataFileName);
		//GLIFV3.x:���Һ�������ͷ
		while(!f.eof())
		{
			f.getline(strTemp.GetBuffer(255),255); strTemp.ReleaseBuffer();
			if( strTemp.Find(strSTRUCTURE_LOAD_OF_RESTRAINTS) !=-1 )
			{ //�ҵ�"STRUCTURE LOAD OF RESTRAINTS"
				break;
			}
		}
                     
		//�ҵ�"STRUCTURE LOAD OF RESTRAINTS"
		//GLIFV3.x:��ʼ����������
		fdFlg=0;
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));

		strSQL.Format("SELECT G,SEQ FROM %s",strSprTbn);
		tmprs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
			adOpenKeyset, adLockOptimistic, adCmdText); 
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
			if( atof(Mid(temp,0, (gbNewGLIFV31 ? 5 : 16))) > 0 )
			{
				fdFlg=1;
				if( gbNewGLIFV31 )
				{
					iPreINT[1] = atoi(Mid(temp, 0, 5));      //�ڵ��
					iPreINT[2] = atoi(Mid(temp, 5, 5));      //�ڵ�����
				}
				else
				{
					iPreINT[1] = atoi(Mid(temp, 0, 16));   //�ڵ��
					iPreINT[2] = atoi(Mid(temp, 16, 10));  //�ڵ�����
				}
				if( iPreINT[2] < 1000 && iPreINT[2] != (100 + giUPxyz * 10) && iPreINT[2] != (200 + giUPxyz * 10) && iPreINT[2] != (300 + giUPxyz * 10) )
				{
					//X/Y/Z�����ϣ��յ���������Ϊ110/120/130(��������ϵ)��210/220/230(��Ԫ����ϵ)��310/320/330(�ض�������������)
					//Debug.Print temp
					//�ǵ����ҷǸ��Ե���
					//����֧������6������,����ΪFx,Fy,Fz,Mx,My,Mz
					//�ȴ����һ��
					i = 1;
					//�����
					for( j = 1 ;j<=6;j++) //ÿ�����6������,����Ϊ1C-Load(����1�������),IN-Load(��װ����),WK-Load(��������),CR-LOAD/S-SCALE(�������/�ṹ����ϵ��),WT-LOAD(ˮ����),ST-LOAD(�ṹ����)
					{
						if( gbNewGLIFV31 )
						{
							phsVAL[j][i] = atof(Mid(temp, 11 + (j - 1) * 12 -1, 12));
							mvMultiple+=phsVAL[j][1];
						}
						else
						{
							phsVAL[j] [i] = atof(Mid(temp, 26 + (j - 1) * 15 -1, 15));
							mvMultiple+=phsVAL[j][1];
						}
					}					
            
					for(i = 2 ;i<= 6;i++)
					{
						//�ٶ�5��
						f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
						mvMultiple = 0;
						for(j = 1 ;j<= 6;j++) //ÿ�����6������,����Ϊ1C-Load(����1�������),IN-Load(��װ����),WK-Load(��������),CR-LOAD/S-SCALE(�������/�ṹ����ϵ��),WT-LOAD(ˮ����),ST-LOAD(�ṹ����)
						{
							if( gbNewGLIFV31 )
							{
								phsVAL[j][i] = atof(Mid(temp, 10 + (j - 1) * 12, 12));
								mvMultiple+=phsVAL[j][i];
							}
							else
							{
								phsVAL[j][i] =atof(Mid(temp, 26 + (j - 1) * 15 -1, 15));
								mvMultiple+=phsVAL[j][i];
							}
						}
						//���6�����ݲ�ȫΪ0
						//if( mvMultiple <> 0 ){
							//sFx������֧���ܽڵ������ķ���
						 //  sFx = Mid(temp, Iif((gbNewGLIFV31, 85, 124), 1)
						  // Exit For
						//}else{
							//���ϵ���Ժ1999��5��ǰʹ�õ�GLIFV3.x�汾��ÿ��������֮���һ��
							//���ݵ���Ժ2001��5��ǰʹ�õ�GLIFV3.1�汾������Ժʹ�õĺų�GLIFv3.2�汾��ÿ��������֮�󲻿���
							//f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();  //����
							//Debug.Print temp
						//}
					}
				}
				else
				{
					//����֧����ֻ��1������:���Ի���Ե���
					for(j = 1 ;j<=6;j++) //ÿ�����6������,����Ϊ1C-Load(����1�������),IN-Load(��װ����),WK-Load(��������),CR-LOAD/S-SCALE(�������/�ṹ����ϵ��),WT-LOAD(ˮ����),ST-LOAD(�ṹ����)
					{
						if( gbNewGLIFV31 )
						{
							phsVAL[j][1]= atoi(Mid(temp, 11 + (j - 1) * 12-1, 12));
							mvMultiple+=phsVAL[j][1];
						}
						else
						{
							phsVAL[j][1] =atoi(Mid(temp, 26 + (j - 1) * 15-1, 15));
							mvMultiple+=phsVAL[j][1];
						}
					}
				}
				rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(iPreINT[1]) + " AND "+CString("[FileNameID]=")+ltos(FileNameID));
				//MsgBox rsData.RecordCount
					 if( rsData->BOF && rsData->adoEOF )
					 {
						  rsData->AddNew();
						  rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						  rsData->PutCollect("VolumeID", _variant_t(EDIBgbl::SelVlmID));
						  rsData->PutCollect("Zdjh",_variant_t((long)(ltemp+1)));
						  rsData->PutCollect("PSACoefofVarLoad",_variant_t(modPHScal::gnSPR_CHZBH));//pfg20051019 //����Ӧ������ĵ��ɺ��ر仯ϵ��
						  rsData->Update();
						  ltemp++;
					 }
					 else
					 {
					 }
					 for(i = 1 ;i<iCount;i++)
					 {
						 if( iJSJDH[i] == iPreINT[1] ) break;
					 }
					 rsData->PutCollect("PSAver",_variant_t(strPSAVER));
					 if( iPreINT[2] > 1000 && iPreINT[2] < 5000 )
					 {
					 //����֧���ܲ�����
						rsData->PutCollect("ParallelingNum",_variant_t((long)atoi(Mid(ltos(iPreINT[2]), 1, 1))));
					 }
					 rsData->PutCollect("JSDBH1",_variant_t((long)iPreINT[1]));
					 rsData->PutCollect("Type", _variant_t((long)iPreINT[2]));
					 rsData->PutCollect("psaType", _variant_t((long)iPreINT[2]));
					 if( EDIBgbl::PSA_OutDataUnit=="NATIONAL" )
					 {
						rsData->PutCollect("Unit_Force", "N");
						rsData->PutCollect("Unit_Momental", "N.m");
					 }
					 else
					 {
						rsData->PutCollect("Unit_Force", "kgf");
						rsData->PutCollect("Unit_Momental", "kgf.m");
					 }
					 rsData->PutCollect("Unit_Displacement", "mm");
					 if( iPreINT[2] < 1000 && iPreINT[2] != (100 + giUPxyz * 10) && iPreINT[2] != (200 + giUPxyz * 10) && iPreINT[2] != (300 + giUPxyz * 10) )
					 {
						sFx=Mid(ltos(iPreINT[2]),1, 1);     //��λ��ʾ������λ��
						sFx1=Mid(ltos(iPreINT[2]), 2, 1);  //��λ��ʾ���ƽ�λ��
					
               
							rsData->PutCollect("paz0", _variant_t(phsVAL[giUPxyz][1]));
							rsData->PutCollect("paz1", _variant_t(phsVAL[giUPxyz][1]));
							rsData->PutCollect("pxl1", _variant_t(phsVAL[2][1]));
							rsData->PutCollect("pyl1", _variant_t(phsVAL[2][2]));
							rsData->PutCollect("pzl1", _variant_t(phsVAL[2][3]));
							rsData->PutCollect("pxr1", _variant_t(phsVAL[3][1]));
							rsData->PutCollect("pyr1", _variant_t(phsVAL[3][2]));
							rsData->PutCollect("pzr1", _variant_t(phsVAL[3][3]));
							rsData->PutCollect("pgz1", _variant_t(phsVAL[3][giUPxyz]));
							rsData->PutCollect("pjg1", _variant_t(phsVAL[6][giUPxyz]));
							rsData->PutCollect("pjgFX1", _variant_t(phsVAL[6][1]));
							rsData->PutCollect("pjgFY1", _variant_t(phsVAL[6][2]));
							rsData->PutCollect("pjgFZ1", _variant_t(phsVAL[6][3]));
							rsData->PutCollect("pjgMX1", _variant_t(phsVAL[6][4]));
							rsData->PutCollect("pjgMY1", _variant_t(phsVAL[6][5]));
							rsData->PutCollect("pjgMZ1", _variant_t(phsVAL[6][6]));
							rsData->PutCollect("mxl1", _variant_t(phsVAL[2][4]));
							rsData->PutCollect("myl1", _variant_t(phsVAL[2][5]));
							rsData->PutCollect("mzl1", _variant_t(phsVAL[2][6]));
							rsData->PutCollect("mxr1", _variant_t(phsVAL[3][4]));
							rsData->PutCollect("myr1", _variant_t(phsVAL[3][5]));
							rsData->PutCollect("mzr1", _variant_t(phsVAL[3][6]));
					 }
					 else
					 {
						//���ɻ�յ�
						rsData->PutCollect("paz0", _variant_t(phsVAL[2][1]));
						rsData->PutCollect("paz1", _variant_t(phsVAL[2][1]));
						rsData->PutCollect("pgz1", _variant_t(phsVAL[3][1]));
						rsData->PutCollect("pjg1", _variant_t(phsVAL[6][1]));
						if(giUPxyz==1)
						{
							rsData->PutCollect("pxl1", _variant_t(phsVAL[2][1]));
							rsData->PutCollect("pxr1", _variant_t(phsVAL[3][1]));
							rsData->PutCollect("pjgFX1", _variant_t(phsVAL[6][1]));
						}
						else if(giUPxyz==2)
						{
							rsData->PutCollect("pyl1", _variant_t(phsVAL[2][1]));
							rsData->PutCollect("pyr1", _variant_t(phsVAL[3][1]));
							rsData->PutCollect("pjgFY1", _variant_t(phsVAL[6][1]));
						}
						else						
						{
							rsData->PutCollect("pzl1", _variant_t(phsVAL[2][1]));
							rsData->PutCollect("pzr1", _variant_t(phsVAL[3][1]));
							rsData->PutCollect("pjgFZ1", _variant_t(phsVAL[6][1]));
						}
						float fG,fR;int iSerialNum,iParelleNum,iSEQ;
						if(iPreINT[2]>1000 && iPreINT[2]<5000)
						{
							CString strTmp;
							strTmp.Format("%d",iPreINT[2]);
							iSerialNum=atoi(strTmp.Left(1));
							iParelleNum=atoi(strTmp.Mid(1,1));
							iSEQ=atoi(strTmp.Right(2));
							_variant_t vTmp;
							rs1->Find((_bstr_t)("SEQ=" + ltos(iSEQ)), 0, adSearchForward);
							if(!rs1->adoEOF)
							{
								rs1->get_Collect((_variant_t)"G", &varTmp);
								fG=vtof(varTmp);
								fR=fabs(phsVAL[2][1]) - fabs(phsVAL[3][1]);
								if(EDIBgbl::PSA_OutDataUnit== "NATIONAL")
									fR/=EDIBgbl::kgf2N;
								fR/=iParelleNum;
								fR/=(fG/iSerialNum);
								if(giUPxyz==1)
									rsData->PutCollect("xr1", _variant_t(fR));
								else if(giUPxyz==2)
									rsData->PutCollect("yr1", _variant_t(fR));
								else
									rsData->PutCollect("zr1", _variant_t(fR));
							}
						}
					 }
					 rsData->Filter=_variant_t((long)adFilterNone);
				iC1++;
			}
			else
			{
				if( fdFlg==1)
				{
					do
					{
						if(temp.Find(strSTRUCTURE_LOAD_OF_RESTRAINTS) != -1 )
						{
							break;
						}
						f.getline(temp.GetBuffer(255),255); temp.ReleaseBuffer();
					}while(!f.eof());
					fdFlg=0;
				}
			}
			
			if( iCount <= 0 ) iCount = 1;
			//MsgBox iC1 & ":" & iCount
			frmStatus.UpdateStatus((float)iC1/(float)iCount,true);
			frmStatus.UpdateWindow();
		}
		f.close();

		if(!rsData->adoEOF || !rsData->BOF)
			rsData->MoveFirst();
		_variant_t vtmp;
		while(!rsData->adoEOF)
		{
			vtmp=rsData->GetCollect("ZDJH");
			rsData->PutCollect("ZDJH",_variant_t((long)(vtoi(vtmp)+1000)));
			rsData->MoveNext();
		}

		_ConnectionPtr pCon;
		strSQL=(char*)(bstr_t)rsData->GetSource();
		pCon=rsData->GetActiveConnection();
		strSQL.Replace("ORDER BY JSDBH1","ORDER BY FileNameID");
		rsData->Close();
		rsData->Open(_variant_t(strSQL),(IDispatch*)pCon,adOpenKeyset,adLockOptimistic,adCmdText);
		int itmpZdjh=1;
		while(!rsData->adoEOF)
		{
			rsData->PutCollect("ZDJH",_variant_t((long)itmpZdjh));
			itmpZdjh++;
			rsData->MoveNext();
		}
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(CException *e)
	{
		e->Delete();
	}
}
/*
void  ReadResult_CAESARII45(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID)
{
 
	//Zhufang  kuang   add  2005.5.30
	const char* strPSAVER = "CASEARII 4.50";  
	CString ConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s;Jet OLEDB:Database Password=%s";
	CString dbConnectionString;
   
	_ConnectionPtr m_pConnSourceDB;			   //Դ���ݿ�����Ӷ���
	_RecordsetPtr m_prsCASENUM;					//��̬����̬���ļ�¼��
	_RecordsetPtr m_prsHangers;					//��[OUTPUT_HANGERS]��¼��
	_RecordsetPtr m_prsDisplacements;			//��[OUTPUT_DISPLACEMENTS]��¼��
	_RecordsetPtr m_prsRestraints;				//��[OUTPUT_RESTRAINTS]��¼��
	_RecordsetPtr m_prsJGHZ;						//ÿ���ڵ����ֵ
	_RecordsetPtr m_prsJOBNAME;					//��ͬ�������ļ�¼��
	_RecordsetPtr m_prsBasicData;					//������ȡ�¶ȣ��ܾ��ļ�¼��
   _RecordsetPtr m_prsTempCASENUM;				//��������TO_NODE����ʱ��¼��
	m_pConnSourceDB.CreateInstance(__uuidof(Connection));
	m_prsJOBNAME.CreateInstance(__uuidof(Recordset));
	m_prsCASENUM.CreateInstance(__uuidof(Recordset));

	CString m_strSQL;			
	_variant_t tmpvar;		
	long ltemp;
	int node_start;									//FROM_NODE��
	int node_end ;									//END_NODE��
	CString PSA_OutDataFUnit;						//Դ��ĵ�λ
	CString PSA_OutDataMUnit;

	_RecordsetPtr rsData = rsResult;
	CFile m_CAESARIIFile;							//�������ļ�����
	CString m_strCAESARIIFileName;				   //�������ļ���

//����������ļ����õı���
	float buffer[1];
	long loff = 272 ;		//ƫ����
	long node_startfromfile;//FROM_NODE 
	long node_endfromfile;  //TO_NODE
	long basepoint = 180;   //����ƫ����
	float tmp1;
	float tmp2;
	bool bReadinDB = true;					//�ж��Ǵ����ݿ��ȡ���ǴӶ������ļ��ж�ȡ,ȱʡ�����ݿ��ж�ȡ
//********************************
	CString m_strFilePath;				
	CString m_strTYPE;
	CString m_strJOBNAME_P;				//�����ļ���*._P
	CString m_strJOBNAME_A;				//�����ļ���*._A
	CString m_strGKname;				//������
	CString m_strRGKname;				//�ȹ���������Դ������
	CString m_strLGKname;				//�乤��������Դ������
	CString* m_pstrTempJobname;
	CString* m_pstrTempGKname;
	float PI=3.1416f;
	int iC1 = 0;
	long iCount = 0;   //�ڵ����
	int nCount = 0;	   //�洢�ڵ����,�õ��������ļ��нڵ�ĸ���
	int iUPxyz = 0;	   //�ж����ϵ������ᣬ1=x��2=y��3=z��
	ltemp = maxZdjh;
	int n = 0;
	CSelPSAProjectNameDlg* m_selDlg;


	try
	{
		dbConnectionString.Format(ConnectionString,SourceDataFileName,"");
		m_pConnSourceDB->Open(_bstr_t(dbConnectionString),"","",0);

		m_strSQL.Format(_T("SELECT DISTINCT [JOBNAME] FROM [OUTPUT_LOCAL_ELEMENT_FORCES]"));
      //�ô˷����򿪵ļ�¼�����ܵ���GetRecodsetCount()��ü�¼��
		m_prsJOBNAME->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenKeyset,adLockOptimistic,adCmdText); 
		//�ڶԻ�����б���м��빤���ļ���,��Ͽ���빤����������ȡ�û���ѡ��
		m_selDlg = new CSelPSAProjectNameDlg;
 
		n = m_prsJOBNAME->GetRecordCount();

		m_selDlg->JobNameNum = n;
		m_pstrTempJobname = new CString[n];

		for ( int i=0 ;i<n;i++)
		{
			m_pstrTempJobname[i] = vtos(m_prsJOBNAME->GetCollect("JOBNAME"));
			m_prsJOBNAME->MoveNext();
		}
		m_prsJOBNAME->Close();

		m_strSQL.Empty();
		m_strSQL.Format(_T("SELECT DISTINCT [CASE] FROM [OUTPUT_LOCAL_ELEMENT_FORCES]"));
		m_prsJOBNAME->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenKeyset,adLockOptimistic,adCmdText);
		
		n = m_prsJOBNAME->GetRecordCount();
		m_selDlg->GKNum = n;
		m_pstrTempGKname = new CString[n];
		
		for (i=0;i<n;i++)
		{
			m_pstrTempGKname[i] = vtos(m_prsJOBNAME->GetCollect("CASE"));
			m_prsJOBNAME->MoveNext();
		}

		m_selDlg->m_pstrGKName		=  m_pstrTempGKname;
		m_selDlg->m_pstrProjectName = m_pstrTempJobname;


		if ( m_selDlg->DoModal() == IDOK)
		{
			m_strJOBNAME_P = m_selDlg->m_strTempProject ;
			m_strGKname    = m_selDlg->m_strTempGK ;
			iUPxyz			= m_selDlg->iUpxyz ;
			m_strRGKname   = m_selDlg->m_strTempRGK ;
			m_strLGKname	= m_selDlg->m_strTempLGK ;
		}
		else
		{
			return;   //���¶Ի����ȡ����ť�˳���ȡ����
		}
		m_prsJOBNAME->Close();
		delete m_selDlg;
		delete [] m_pstrTempGKname;
		delete [] m_pstrTempJobname;


		//��ʾ������
		frmStatus.ShowWindow(SW_SHOW);
		frmStatus.m_Label1= GetResStr(IDS_FROMFILE) + SourceDataFileName;
		frmStatus.m_Label2= GetResStr(IDS_TODATABASE) + EDIBgbl::dbPRJDB.GetName() + GetResStr(IDS_InTable) + EDIBgbl::TBNRawData;
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
		m_prsCASENUM->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenKeyset,adLockOptimistic,adCmdText);
		
		iCount = m_prsCASENUM->GetRecordCount();

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
		

///****************************��ʼ������̬����*******************************

		for (i=0; i <= iCount; i++)
		{
				tmpvar = m_prsCASENUM->GetCollect("FROM_NODE");
				node_start = vtoi(tmpvar);
				if(i == 0)
					node_end = node_start;
	//***********************����������¼��TO_NODE��*******************
				if (node_start != node_end ) 
				{//��������ж�ZA�������޴ε��¼��������룬�����޸�
					rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_end)+" AND [FileNameID]="+ltos(FileNameID));
					if ( rsData->BOF && rsData->adoEOF)
					{
						rsData->AddNew();
						rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						rsData->PutCollect("zdjh",_variant_t((long)(ltemp+1)));
						rsData->PutCollect("FileNameID",_variant_t(FileNameID));
						ltemp++;
						rsData->Update();					
					}
					else
					{
					}
					rsData->PutCollect("PSAver", _variant_t(strPSAVER));
					rsData->PutCollect("UPxyz",_variant_t((long)iUPxyz));
					rsData->PutCollect("JSDBH1", _variant_t((long)node_end));

					//д��ṹ����,TO_NODE��		
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT DISTINCT [NODE] ,max([RESULTANTF]) AS PJG FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' GROUP BY [NODE] "),node_end,m_strJOBNAME_P);
					m_prsJGHZ = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if(m_prsJGHZ->adoEOF && m_prsJGHZ->BOF)
						;
					else
						rsData->PutCollect("PJG1",m_prsJGHZ->GetCollect("PJG"));		

					rsData->PutCollect("Type", _variant_t(m_strTYPE));
					
					//�ж�Դ��ĵ�λ�ƣ�д��ZA����			 				 
					if( PSA_OutDataFUnit  == "N." )
						rsData->PutCollect("Unit_Force", "N");
					else
						rsData->PutCollect("Unit_Force", "kgf");

					if (PSA_OutDataMUnit == "N.m.")
						rsData->PutCollect("Unit_Momental", "N.m");
					else
						rsData->PutCollect("Unit_Momental", "kgf.m");
					
					//д����������					
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_LOCAL_ELEMENT_FORCES] WHERE [TO_NODE] = %d AND [CASE] = '%s' "),node_end,m_strRGKname);
					m_prsTempCASENUM = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					rsData->PutCollect("pxr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FXT")))));
					rsData->PutCollect("pyr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FYT")))));
					rsData->PutCollect("pzr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FZT")))));

					rsData->PutCollect("mxr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MXT")))));
					rsData->PutCollect("myr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MYT")))));
					rsData->PutCollect("mzr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MZT")))));

					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_HANGERS] WHERE [NODE] = %d AND [JOBNAME] =  '%s'"),node_end,m_strJOBNAME_P);
					m_prsHangers = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if ( m_prsHangers->BOF && m_prsHangers->adoEOF )
					{
					}
					else
					{	
					// ����֧��������
						CString strSize;
						CString strTemp;
						rsData->PutCollect("pgz1",m_prsHangers->GetCollect("HOT_LOAD"));
						rsData->PutCollect("paz0",m_prsHangers->GetCollect("TH_INSTALL_LOAD"));
						rsData->PutCollect("paz1",m_prsHangers->GetCollect("AC_INSTALL_LOAD"));
						rsData->PutCollect("CoefOfVarLoad",_variant_t(vtof(m_prsHangers->GetCollect("LOAD_VARIATION"))/100));
						tmpvar = m_prsHangers->GetCollect("SIZE");
						strSize.Format(_T("%02d"),vtoi(tmpvar));
					//�γ�����GLIF�����֧�������ͺ�								
						strTemp = vtos(m_prsHangers->GetCollect("NUMREQ"))+"1"+strSize;
						rsData->PutCollect("TYPE",_variant_t(strTemp));							
					}
					//�����λ�Ʊ�OUTPUT_DISPLACEMENTS�в���FROM_NODE��������CASE��ȵļ�¼,ʹ�ֶ�DX->�ֶ�dxr1,�ֶ�DY->�ֶ�dyr1,�ֶ�DZ->�ֶ�dzr1					
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s'"),node_end,m_strRGKname,m_strJOBNAME_P);			
					m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if ( !(m_prsDisplacements->BOF && m_prsDisplacements->adoEOF) )
					{
						rsData->PutCollect("xr1",m_prsDisplacements->GetCollect("DX"));
						rsData->PutCollect("yr1",m_prsDisplacements->GetCollect("DY"));
						rsData->PutCollect("zr1",m_prsDisplacements->GetCollect("DZ"));

						tmpvar = m_prsDisplacements->GetCollect("DUNITS");
						if (vtos(tmpvar) == "mm")
							rsData->PutCollect("UNIT_Displacement",_variant_t(tmpvar));
						
						tmpvar = m_prsDisplacements->GetCollect("RUNITS");
						if (vtos(tmpvar) == "deg.")
						{
							 tmpvar = m_prsDisplacements->GetCollect("RX");
							 rsData->PutCollect("rxr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));
							 rsData->PutCollect("ryr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));					 
							 rsData->PutCollect("rzr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RZ"))/180*PI));	
						}
						else
						{
							rsData->PutCollect("rxr1",m_prsDisplacements->GetCollect("RX"));
							rsData->PutCollect("ryr1",m_prsDisplacements->GetCollect("RY"));
							rsData->PutCollect("rzr1",m_prsDisplacements->GetCollect("RZ"));
						}
					}
					else
					{
					}
					//��[INPUT_BASIC_ELEMENT_DATA]������ȡ�¶ȣ��ܾ�д��ZA����	TO_NODE��				

					if ( bReadinDB )
					{
						m_strSQL.Empty();
						m_strSQL.Format(_T("SELECT * FROM [INPUT_BASIC_ELEMENT_DATA] WHERE [JOBNAME] =  '%s' AND [TO_NODE] = %d"),m_strJOBNAME_A,node_end);
						m_prsBasicData = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);

						if (!(m_prsBasicData->BOF && m_prsBasicData->adoEOF))
						{
							rsData->PutCollect("DJ1",m_prsBasicData->GetCollect("DIAMETER"));
							rsData->PutCollect("T01",m_prsBasicData->GetCollect("TEMP_EXP_C1"));
						}
						else
						{
						}
						m_prsBasicData->Close();
					}
					else
					{
						//�Ӷ������ļ��ж�ȡ
						//��ȡCAESARII�Ķ������ļ������л�ȡ�ڵ�ţ��ܾ����¶ȣ�д��ZA����Ӧ�ڵ�ŵĹܾ����¶��ֶ���
						try
						{
							if ( !FileExists(m_strCAESARIIFileName))
								; 
							else
							{
								m_CAESARIIFile.Open(m_strCAESARIIFileName,CFile::modeRead);
								
								for (int i = 0;i<nCount;i++)
								{
									m_CAESARIIFile.Seek(basepoint+4+(i*loff),CFile::begin);
									m_CAESARIIFile.Read(buffer,4);
									node_endfromfile = (long)buffer[0];
									rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_endfromfile) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));;
									if (!(rsData->BOF && rsData->adoEOF))
									{
										m_CAESARIIFile.Seek(basepoint+20+(i*loff),CFile::begin);//��ȡ�ܾ�
										m_CAESARIIFile.Read(buffer,4);
										if ( buffer[0] > 0 )
											tmp1 = buffer[0];
										else 
											buffer[0] = tmp1;
										rsData->PutCollect("DJ1",_variant_t(buffer[0]));			
										
										m_CAESARIIFile.Seek(basepoint+36+(i*loff),CFile::begin);//��ȡ�¶�  
										m_CAESARIIFile.Read(buffer,4);
										if ( buffer[0] > 0 )
											tmp2 = buffer[0];
										else
											buffer[0] = tmp2;
										rsData->PutCollect("T01",_variant_t(buffer[0]));		
									}
									rsData->Filter = "";
								}
								m_CAESARIIFile->Close();
							}

						}catch(CFileException *e)
						{
							e->ReportError ();
						}

					}
					m_prsDisplacements->Close();
					m_prsTempCASENUM->Close();
					m_prsHangers->Close();
					m_prsJGHZ->Close();
				
				}//end if


				
	//	/***********************�����Ǵ���������¼��FROM_NODE��**********************
				if (i!=iCount)   //���һ��ѭ��ʱ���ô���FROM_NODE�㣬��һ���Ѿ�������
				{
					rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_start)+" AND [FileNameID]="+ltos(FileNameID));
					if ( rsData->BOF && rsData->adoEOF )
					{
						rsData->AddNew();
						// rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						rsData->PutCollect("zdjh",_variant_t((long)(ltemp+1)));
						rsData->PutCollect("FileNameID",_variant_t(FileNameID));
						ltemp++;
						rsData->Update();					
					}
					else{
					}

					rsData->PutCollect("PSAver", _variant_t(strPSAVER));
					rsData->PutCollect("UPxyz",_variant_t((long)iUPxyz));
					rsData->PutCollect("JSDBH1", _variant_t((long)node_start));					
					//д��ṹ���أ�FROM_NODE��

					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT DISTINCT [NODE] ,max([RESULTANTF]) AS PJG FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' GROUP BY [NODE] "),node_start,m_strJOBNAME_P);
					m_prsJGHZ = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if(m_prsJGHZ->adoEOF && m_prsJGHZ->BOF)
						;
					else
						rsData->PutCollect("PJG1",m_prsJGHZ->GetCollect("PJG"));					

					rsData->PutCollect("Type", _variant_t(m_strTYPE));


					//��[INPUT_BASIC_ELEMENT_DATA]������ȡ�¶ȣ��ܾ�д��ZA����
					if ( bReadinDB )
					{
						m_strSQL.Empty();
						m_strSQL.Format(_T("SELECT * FROM [INPUT_BASIC_ELEMENT_DATA] WHERE [JOBNAME] =  '%s' AND [FROM_NODE] = %d"),m_strJOBNAME_A,node_start);
						m_prsBasicData = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
						if (!(m_prsBasicData->BOF && m_prsBasicData->adoEOF))
						{
							rsData->PutCollect("DJ1",m_prsBasicData->GetCollect("DIAMETER"));
							rsData->PutCollect("T01",m_prsBasicData->GetCollect("TEMP_EXP_C1"));
						}
						else
						{
						}
						m_prsBasicData->Close();
					}
					else
					{
						//�Ӷ������ļ��ж�ȡ
						//��ȡCAESARII�Ķ������ļ������л�ȡ�ڵ�ţ��ܾ����¶ȣ�д��ZA����Ӧ�ڵ�ŵĹܾ����¶��ֶ���
						try
						{
							if ( !FileExists(m_strCAESARIIFileName))
								;
							else
							{
								m_CAESARIIFile.Open(m_strCAESARIIFileName,CFile::modeRead);
								
								for (int i = 0;i<nCount;i++)
								{
									m_CAESARIIFile.Seek(basepoint+(i*loff),CFile::begin);
									m_CAESARIIFile.Read(buffer,4);
									node_startfromfile = (long)buffer[0];
									rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_startfromfile) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));;
									if (!(rsData->BOF && rsData->adoEOF))
									{
										m_CAESARIIFile.Seek(basepoint+20+(i*loff),CFile::begin);//��ȡ�ܾ�
										m_CAESARIIFile.Read(buffer,4);
										if ( buffer[0] > 0 )
											tmp1 = buffer[0];
										else 
											buffer[0] = tmp1;
										rsData->PutCollect("DJ1",_variant_t(buffer[0]));			
										
										m_CAESARIIFile.Seek(basepoint+36+(i*loff),CFile::begin);//��ȡ�¶�  
										m_CAESARIIFile.Read(buffer,4);
										if ( buffer[0] > 0 )
											tmp2 = buffer[0];
										else
											buffer[0] = tmp2;
										rsData->PutCollect("T01",_variant_t(buffer[0]));		
									}
									rsData->Filter = "";
								}
								m_CAESARIIFile->Close();
							}

						}catch(CFileException *e)
						{
							e->ReportError ();
						}

					}
					
					//�ж�Դ��ĵ�λ�ƣ�д��ZA����			 				 
					if( PSA_OutDataFUnit  == "N." )
						rsData->PutCollect("Unit_Force", "N");
					else
						rsData->PutCollect("Unit_Force", "kgf");
					if (PSA_OutDataMUnit == "N.m.")
						rsData->PutCollect("Unit_Momental", "N.m");
					else
						rsData->PutCollect("Unit_Momental", "kgf.m");


					rsData->PutCollect("pxr1",m_prsCASENUM->GetCollect("FXF"));
					rsData->PutCollect("pyr1",m_prsCASENUM->GetCollect("FYF"));
					rsData->PutCollect("pzr1",m_prsCASENUM->GetCollect("FZF"));
					
					rsData->PutCollect("mxr1",m_prsCASENUM->GetCollect("MXF"));
					rsData->PutCollect("myr1",m_prsCASENUM->GetCollect("MYF"));
					rsData->PutCollect("mzr1",m_prsCASENUM->GetCollect("MZF"));

					//��������ɱ�OUTPUT_HANGERS�в���NODE=FROM_NODE��¼��ʹpgz1=HOT_LOAD,paz0=TH_INSTALL_LOAD,paz1=AC_INSTALL_LOAD,

					m_strSQL.Empty();			
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_HANGERS] WHERE [NODE] = %d AND [JOBNAME] =  '%s'"),node_start,m_strJOBNAME_P);

					m_prsHangers = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if ( m_prsHangers->BOF && m_prsHangers->adoEOF)
					{
					}
					else
					{	
						// ����֧��������
						CString strSize;
						CString strTemp;
						rsData->PutCollect("pgz1",m_prsHangers->GetCollect("HOT_LOAD"));
						rsData->PutCollect("paz0",m_prsHangers->GetCollect("TH_INSTALL_LOAD"));
						rsData->PutCollect("paz1",m_prsHangers->GetCollect("AC_INSTALL_LOAD"));
						rsData->PutCollect("CoefOfVarLoad",_variant_t(vtof(m_prsHangers->GetCollect("LOAD_VARIATION"))/100));
						tmpvar = m_prsHangers->GetCollect("SIZE");
						strSize.Format(_T("%02d"),vtoi(tmpvar));
					//�γ�����GLIF�����֧�������ͺ�								
						strTemp = vtos(m_prsHangers->GetCollect("NUMREQ"))+"1"+strSize;
						rsData->PutCollect("TYPE",_variant_t(strTemp));							
					}

				//�����λ�Ʊ�OUTPUT_DISPLACEMENTS�в���FROM_NODE��������CASE��ȵļ�¼,ʹ�ֶ�DX->�ֶ�dxr1,�ֶ�DY->�ֶ�dyr1,�ֶ�DZ->�ֶ�dzr1,

				m_strSQL.Empty();
				m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s'"),node_start,m_strRGKname,m_strJOBNAME_P);
				m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
				if ( !(m_prsDisplacements->BOF && m_prsDisplacements->adoEOF) )
				{
					rsData->PutCollect("xr1",m_prsDisplacements->GetCollect("DX"));
					rsData->PutCollect("yr1",m_prsDisplacements->GetCollect("DY"));
					rsData->PutCollect("zr1",m_prsDisplacements->GetCollect("DZ"));

					tmpvar = m_prsDisplacements->GetCollect("DUNITS");
					if (vtos(tmpvar) == "mm")
						rsData->PutCollect("UNIT_Displacement",_variant_t(tmpvar));
					
					tmpvar = m_prsDisplacements->GetCollect("RUNITS");
					if (vtos(tmpvar) == "deg.")
					{
						 tmpvar = m_prsDisplacements->GetCollect("RX");
						 rsData->PutCollect("rxr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));
						 rsData->PutCollect("ryr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));					 
						 rsData->PutCollect("rzr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RZ"))/180*PI));	
					}
					else
					{
						rsData->PutCollect("rxr1",m_prsDisplacements->GetCollect("RX"));
						rsData->PutCollect("ryr1",m_prsDisplacements->GetCollect("RY"));
						rsData->PutCollect("rzr1",m_prsDisplacements->GetCollect("RZ"));
					}
				}
				else
				{
				}	
 				m_prsDisplacements->Close();
				m_prsHangers->Close();
				m_prsJGHZ->Close();
			}
			//���汾����¼��TO_NODE�㣬��������һ����¼��FROM_NODE�Ƚ�
			node_end = vtoi(m_prsCASENUM->GetCollect("TO_NODE"));  

			if ( i < iCount-1 )
				m_prsCASENUM->MoveNext();
			
			iC1++;
			frmStatus.UpdateStatus((float)iC1/(float)iCount,true);
			frmStatus.UpdateWindow();
			rsData->Filter = "";
			
		   
			nCount++;
		}//  end  for
		m_prsCASENUM->Close();

      m_strSQL.Empty();
		//��ȡ��̬��¼��
		m_strSQL.Format(_T("SELECT * FROM [OUTPUT_LOCAL_ELEMENT_FORCES] WHERE [CASE] = '%s' AND [JOBNAME] =  '%s'"),m_strLGKname,m_strJOBNAME_P);
		m_prsCASENUM = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
		node_end = -1;
				
//******************************��ʼ������̬����*******************************		
		for (i=0; i<= iCount; i++)
		{
				tmpvar = m_prsCASENUM->GetCollect("FROM_NODE");
				node_start = vtoi(tmpvar);
				if (i==0)
					node_end = node_start;
	//	/***********************����TO_NODE��¼*************************
				if (node_start != node_end ) 
				{
					//����������node_end��(һ������,��̬�Ѿ�����)������Ӧ����̬�ֶ���ȥ
					
					rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_end) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));;
										
					//д����������					
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_LOCAL_ELEMENT_FORCES] WHERE [TO_NODE] = %d AND [CASE] = '%s' "),node_end,m_strLGKname);
					m_prsTempCASENUM = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' AND [CASE] = '%s' AND [TYPE] = 'Rigid ANC' "),node_end,m_strJOBNAME_P,m_strGKname);	
					m_prsRestraints = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					
					if (m_prsRestraints->adoEOF && m_prsRestraints->BOF)   //���ǹ̶���
					{
						rsData->PutCollect("pxl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FXT")))));
						rsData->PutCollect("pyl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FYT")))));
						rsData->PutCollect("pzl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FZT")))));

						rsData->PutCollect("mxl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MXT")))));
						rsData->PutCollect("myl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MYT")))));
						rsData->PutCollect("mzl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MZT")))));
					}
					else																	//�ǹ̶���
					{
						rsData->PutCollect("pxl1",m_prsRestraints->GetCollect("FX"));
						rsData->PutCollect("pyl1",m_prsRestraints->GetCollect("FY"));
						rsData->PutCollect("pzl1",m_prsRestraints->GetCollect("FZ"));

						rsData->PutCollect("mxl1",m_prsRestraints->GetCollect("MX"));
						rsData->PutCollect("myl1",m_prsRestraints->GetCollect("MY"));
						rsData->PutCollect("mzl1",m_prsRestraints->GetCollect("MZ"));
					}

					//�����λ�Ʊ�OUTPUT_DISPLACEMENTS�в���FROM_NODE��������CASE��ȵļ�¼,ʹ�ֶ�DX->�ֶ�dxr1,�ֶ�DY->�ֶ�dyr1,�ֶ�DZ->�ֶ�dzr1					
					
					if(m_prsRestraints->adoEOF && m_prsRestraints->BOF)   //������ǹ̶���
					{
						m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s' "),node_end,m_strLGKname,m_strJOBNAME_P);						
						m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					}
					else     //�ǹ̶���
					{
						m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [CASE] = '%s' AND [JOBNAME] = '%s' AND [NODE] = %d"),m_strGKname,m_strJOBNAME_P,node_end);
						m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					}
							
					if ( !(m_prsDisplacements->BOF && m_prsDisplacements->adoEOF))
					{
						rsData->PutCollect("xl1",m_prsDisplacements->GetCollect("DX"));
						rsData->PutCollect("yl1",m_prsDisplacements->GetCollect("DY"));
						rsData->PutCollect("zl1",m_prsDisplacements->GetCollect("DZ"));

						tmpvar = m_prsDisplacements->GetCollect("DUNITS");
						if (vtos(tmpvar) == "mm")
							rsData->PutCollect("UNIT_Displacement",_variant_t(tmpvar));
						
						tmpvar = m_prsDisplacements->GetCollect("RUNITS");
						if (vtos(tmpvar) == "deg.")
						{
							 rsData->PutCollect("rxl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RX"))/180*PI));
							 rsData->PutCollect("ryl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));					 
							 rsData->PutCollect("rzl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RZ"))/180*PI));	
						}
						else
						{
							rsData->PutCollect("rxl1",m_prsDisplacements->GetCollect("RX"));
							rsData->PutCollect("ryl1",m_prsDisplacements->GetCollect("RY"));
							rsData->PutCollect("rzl1",m_prsDisplacements->GetCollect("RZ"));
						}
					}
					else
					{
					}
					m_prsDisplacements->Close();
					m_prsTempCASENUM->Close();
					m_prsRestraints->Close();				
					rsData->Filter = "";
				}//end if				
				
//	/*********************����FROM_NODE��**********************
				if( i != iCount )
				{
					rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_start) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));;
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' AND [CASE] = '%s' AND [TYPE] = 'Rigid ANC' "),node_start,m_strJOBNAME_P,m_strGKname);	
					m_prsRestraints = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					
					if (m_prsRestraints->adoEOF && m_prsRestraints->BOF) //�ǹ̶��� 
					{
						rsData->PutCollect("pxl1",m_prsCASENUM->GetCollect("FXF"));
						rsData->PutCollect("pyl1",m_prsCASENUM->GetCollect("FYF"));
						rsData->PutCollect("pzl1",m_prsCASENUM->GetCollect("FZF"));

						rsData->PutCollect("mxl1",m_prsCASENUM->GetCollect("MXF"));
						rsData->PutCollect("myl1",m_prsCASENUM->GetCollect("MYF"));
						rsData->PutCollect("mzl1",m_prsCASENUM->GetCollect("MZF"));
					}
					else																  //���ǹ̶���
					{
						rsData->PutCollect("pxl1",m_prsRestraints->GetCollect("FX"));
						rsData->PutCollect("pyl1",m_prsRestraints->GetCollect("FY"));
						rsData->PutCollect("pzl1",m_prsRestraints->GetCollect("FZ"));

						rsData->PutCollect("mxl1",m_prsRestraints->GetCollect("MX"));
						rsData->PutCollect("myl1",m_prsRestraints->GetCollect("MY"));
						rsData->PutCollect("mzl1",m_prsRestraints->GetCollect("MZ"));
					}
				
				//�����λ�Ʊ�OUTPUT_DISPLACEMENTS�в���FROM_NODE��������CASE��ȵļ�¼,ʹ�ֶ�DX->�ֶ�dxr1,�ֶ�DY->�ֶ�dyr1,�ֶ�DZ->�ֶ�dzr1,
				m_strSQL.Empty();
				if(m_prsRestraints->adoEOF && m_prsRestraints->BOF)   //������ǹ̶���
				{
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s' "),node_start,m_strLGKname,m_strJOBNAME_P);
					m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
				}
				else     //�ǹ̶���
				{
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [CASE] = '%s' AND [JOBNAME] = '%s' AND [NODE] = %d"),m_strGKname,m_strJOBNAME_P,node_start);
					m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
				}

				if ( !(m_prsDisplacements->BOF && m_prsDisplacements->adoEOF))
				{
					rsData->PutCollect("xl1",m_prsDisplacements->GetCollect("DX"));
					rsData->PutCollect("yl1",m_prsDisplacements->GetCollect("DY"));
					rsData->PutCollect("zl1",m_prsDisplacements->GetCollect("DZ"));

					tmpvar = m_prsDisplacements->GetCollect("DUNITS");
					if (vtos(tmpvar) == "mm")
						rsData->PutCollect("UNIT_Displacement",_variant_t(tmpvar));
					
					tmpvar = m_prsDisplacements->GetCollect("RUNITS");
					if (vtos(tmpvar) == "deg.")
					{
						 rsData->PutCollect("rxl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RX"))/180*PI));
						 rsData->PutCollect("ryl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));					 
						 rsData->PutCollect("rzl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RZ"))/180*PI));	
					}
					else
					{
						rsData->PutCollect("rxl1",m_prsDisplacements->GetCollect("RX"));
						rsData->PutCollect("ryl1",m_prsDisplacements->GetCollect("RY"));
						rsData->PutCollect("rzl1",m_prsDisplacements->GetCollect("RZ"));
					}
				}
				else
				{
				}
			m_prsDisplacements->Close();
			m_prsRestraints->Close();
			}	
			//������μ�¼��TO_NODE�㣬����һ����¼��FROM_NODE�Ƚ�			
			node_end = vtoi(m_prsCASENUM->GetCollect("TO_NODE"));

			if ( i < iCount-1 )
				m_prsCASENUM->MoveNext();
			
			rsData->Filter = "";
				
		}//  end  for
		m_prsCASENUM = NULL;					
		m_prsHangers = NULL;			
		m_prsDisplacements = NULL;	
		m_prsRestraints = NULL;		
		m_prsJGHZ = NULL;				
		m_prsJOBNAME = NULL;		
		m_prsBasicData = NULL;		
		m_prsTempCASENUM = NULL;		

		m_pConnSourceDB->Close();
		m_pConnSourceDB = NULL;
		
	}//   end   try
	catch(CException *e)
	{
		e->ReportError();
		e->Delete();
	}

}
*/


void  ReadResult_CAESARII42(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID)
{
	const char* strPSAVER = "CASEARII 4.20";  
	CString ConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s;Jet OLEDB:Database Password=%s";
	CString dbConnectionString;
   
	_ConnectionPtr m_pConnSourceDB;			   //Դ���ݿ�����Ӷ���
	_RecordsetPtr m_prsCASENUM;					//��̬����̬���ļ�¼��
	_RecordsetPtr m_prsHangers;					//��[OUTPUT_HANGERS]��¼��
	_RecordsetPtr m_prsDisplacements;			//��[OUTPUT_DISPLACEMENTS]��¼��
	_RecordsetPtr m_prsRestraints;				//��[OUTPUT_RESTRAINTS]��¼��
	_RecordsetPtr m_prsJGHZ;						//ÿ���ڵ����ֵ
	_RecordsetPtr m_prsJOBNAME;					//��ͬ�������ļ�¼��
	_RecordsetPtr m_prsBasicData;					//������ȡ�¶ȣ��ܾ��ļ�¼��
   _RecordsetPtr m_prsTempCASENUM;				//��������TO_NODE����ʱ��¼��
	m_pConnSourceDB.CreateInstance(__uuidof(Connection));
	m_prsJOBNAME.CreateInstance(__uuidof(Recordset));
	m_prsCASENUM.CreateInstance(__uuidof(Recordset));

	CString m_strSQL;			
	_variant_t tmpvar;		
	long ltemp;
	int node_start;									//FROM_NODE��
	int node_end ;									//END_NODE��
	CString PSA_OutDataFUnit;						//Դ��ĵ�λ
	CString PSA_OutDataMUnit;
	_RecordsetPtr rsData = rsResult;
	CFile m_CAESARIIFile;							//�������ļ�����
	CString m_strCAESARIIFileName;				//�������ļ���
//����������ļ����õı���
	float buffer[1];
	long loff = 272 ;			//ƫ����
	long node_startfromfile;//FROM_NODE 
	long node_endfromfile;  //TO_NODE
	long basepoint = 180;   //����ƫ����
	float tmp1;
	float tmp2;
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
	CString* m_pstrTempGKname;
	float PI=3.1416f;
	int iC1 = 0;
	long iCount = 0;	//�ڵ����
	int nCount = 0;	//�洢�ڵ����,�õ��������ļ��нڵ�ĸ���
	int iUPxyz = 0;	//�ж����ϵ������ᣬ1=x��2=y��3=z��
	ltemp = maxZdjh;//���֧���ܺ�
	int n = 0;
	CSelPSAProjectNameDlg* m_selDlg;
	
	try
	{
		dbConnectionString.Format(ConnectionString,SourceDataFileName,"");
		m_pConnSourceDB->Open(_bstr_t(dbConnectionString),"","",0);

		m_strSQL.Format(_T("SELECT DISTINCT [JOBNAME] FROM [OUTPUT_LOCAL_ELEMENT_FORCES]"));
      //�ô˷����򿪵ļ�¼�����ܵ���GetRecodsetCount()��ü�¼��
		m_prsJOBNAME->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenKeyset,adLockOptimistic,adCmdText); 
		//�ڶԻ�����б���м��빤���ļ���,��Ͽ���빤����������ȡ�û���ѡ��
		m_selDlg = new CSelPSAProjectNameDlg;
 
		n = m_prsJOBNAME->GetRecordCount();

		m_selDlg->JobNameNum = n;
		m_pstrTempJobname = new CString[n];

		for ( int i=0 ;i<n;i++)
		{
			m_pstrTempJobname[i] = vtos(m_prsJOBNAME->GetCollect("JOBNAME"));
			m_prsJOBNAME->MoveNext();
		}
		m_prsJOBNAME->Close();

		m_strSQL.Empty();
		m_strSQL.Format(_T("SELECT DISTINCT [CASE] FROM [OUTPUT_LOCAL_ELEMENT_FORCES]"));
		m_prsJOBNAME->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenKeyset,adLockOptimistic,adCmdText);
		
		n = m_prsJOBNAME->GetRecordCount();
		m_selDlg->GKNum = n;
		m_pstrTempGKname = new CString[n];
		
		for (i=0;i<n;i++)
		{
			m_pstrTempGKname[i] = vtos(m_prsJOBNAME->GetCollect("CASE"));
			m_prsJOBNAME->MoveNext();
		}

		m_selDlg->m_pstrGKName		=  m_pstrTempGKname;
		m_selDlg->m_pstrProjectName = m_pstrTempJobname;


		if ( m_selDlg->DoModal() == IDOK)
		{
			m_strJOBNAME_P = m_selDlg->m_strTempProject ;
			m_strGKname    = m_selDlg->m_strTempGK ;
			iUPxyz			= m_selDlg->iUpxyz ;
			m_strRGKname   = m_selDlg->m_strTempRGK ;
			m_strLGKname	= m_selDlg->m_strTempLGK ;
		}
		else
		{
			return;   //���¶Ի����ȡ����ť�˳���ȡ����
		}
		m_prsJOBNAME->Close();
		delete m_selDlg;
		delete [] m_pstrTempGKname;
		delete [] m_pstrTempJobname;


		//��ʾ������
		frmStatus.ShowWindow(SW_SHOW);
		frmStatus.m_Label1= GetResStr(IDS_FROMFILE) + SourceDataFileName;
		frmStatus.m_Label2= GetResStr(IDS_TODATABASE) + EDIBgbl::GetDBName(EDIBgbl::dbPRJDB) + GetResStr(IDS_InTable) + EDIBgbl::TBNRawData;
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
		m_prsCASENUM->Open(_variant_t(m_strSQL),(IDispatch*)m_pConnSourceDB,adOpenKeyset,adLockOptimistic,adCmdText);
		
		iCount = m_prsCASENUM->GetRecordCount();

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
		

/****************************��ʼ������̬����********************************/

		for (i=0; i <= iCount; i++)
		{
				tmpvar = m_prsCASENUM->GetCollect("FROM_NODE");
				node_start = vtoi(tmpvar);
				if(i == 0)
					node_end = node_start;
		/***********************����������¼��TO_NODE��********************/
				if (node_start != node_end ) 
				{//��������ж�ZA�������޴ε��¼��������룬�����޸�
					rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_end)+" AND [FileNameID]="+ltos(FileNameID));
					if ( rsData->BOF && rsData->adoEOF)
					{
						rsData->AddNew();
						rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						rsData->PutCollect("zdjh",_variant_t((long)(ltemp+1)));
						rsData->PutCollect("FileNameID",_variant_t(FileNameID));
						ltemp++;
						rsData->Update();					
					}
					else
					{
					}
					rsData->PutCollect("PSAver", _variant_t(strPSAVER));
					rsData->PutCollect("UPxyz",_variant_t((long)iUPxyz));
					rsData->PutCollect("JSDBH1", _variant_t((long)node_end));

					//д��ṹ����,TO_NODE��		
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT DISTINCT [NODE] ,max([RESULTANTF]) AS PJG FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' GROUP BY [NODE] "),node_end,m_strJOBNAME_P);
					m_prsJGHZ = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if(m_prsJGHZ->adoEOF && m_prsJGHZ->BOF)
						;
					else
						rsData->PutCollect("PJG1",m_prsJGHZ->GetCollect("PJG"));		

					rsData->PutCollect("Type", _variant_t(m_strTYPE));
					rsData->PutCollect("psaType", _variant_t(m_strTYPE));
					//�ж�Դ��ĵ�λ�ƣ�д��ZA����			 				 
					if( PSA_OutDataFUnit  == "N." )
						rsData->PutCollect("Unit_Force", "N");
					else
						rsData->PutCollect("Unit_Force", "kgf");

					if (PSA_OutDataMUnit == "N.m.")
						rsData->PutCollect("Unit_Momental", "N.m");
					else
						rsData->PutCollect("Unit_Momental", "kgf.m");
					
					//д����������					
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_LOCAL_ELEMENT_FORCES] WHERE [TO_NODE] = %d AND [CASE] = '%s' "),node_end,m_strRGKname);
					m_prsTempCASENUM = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					rsData->PutCollect("pxr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FXT")))));
					rsData->PutCollect("pyr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FYT")))));
					rsData->PutCollect("pzr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FZT")))));

					rsData->PutCollect("mxr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MXT")))));
					rsData->PutCollect("myr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MYT")))));
					rsData->PutCollect("mzr1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MZT")))));
					
				
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_HANGERS] WHERE [NODE] = %d AND [JOBNAME] =  '%s'"),node_end,m_strJOBNAME_P);
					m_prsHangers = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if ( m_prsHangers->BOF && m_prsHangers->adoEOF )
					{
					}
					else
					{	
					// ����֧��������
						CString strSize;
						CString strTemp;
						rsData->PutCollect("pgz1",m_prsHangers->GetCollect("HOT_LOAD"));
						rsData->PutCollect("paz0",m_prsHangers->GetCollect("TH_INSTALL_LOAD"));
						rsData->PutCollect("paz1",m_prsHangers->GetCollect("AC_INSTALL_LOAD"));
						rsData->PutCollect("CoefOfVarLoad",_variant_t(vtof(m_prsHangers->GetCollect("LOAD_VARIATION"))/100));
						tmpvar = m_prsHangers->GetCollect("SIZE");
						strSize.Format(_T("%02d"),vtoi(tmpvar));
					//�γ�����GLIF�����֧�������ͺ�								
						strTemp = vtos(m_prsHangers->GetCollect("NUMREQ"))+"1"+strSize;
						rsData->PutCollect("TYPE",_variant_t(strTemp));							
						rsData->PutCollect("psaTYPE",_variant_t(strTemp));							
					}
					//�����λ�Ʊ�OUTPUT_DISPLACEMENTS�в���FROM_NODE��������CASE��ȵļ�¼,ʹ�ֶ�DX->�ֶ�dxr1,�ֶ�DY->�ֶ�dyr1,�ֶ�DZ->�ֶ�dzr1					
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s'"),node_end,m_strRGKname,m_strJOBNAME_P);			
					m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if ( !(m_prsDisplacements->BOF && m_prsDisplacements->adoEOF) )
					{
						rsData->PutCollect("xr1",m_prsDisplacements->GetCollect("DX"));
						rsData->PutCollect("yr1",m_prsDisplacements->GetCollect("DY"));
						rsData->PutCollect("zr1",m_prsDisplacements->GetCollect("DZ"));

						tmpvar = m_prsDisplacements->GetCollect("DUNITS");
						if (vtos(tmpvar) == "mm")
							rsData->PutCollect("UNIT_Displacement",_variant_t(tmpvar));
						
						tmpvar = m_prsDisplacements->GetCollect("RUNITS");
						if (vtos(tmpvar) == "deg.")
						{
							 tmpvar = m_prsDisplacements->GetCollect("RX");
							 rsData->PutCollect("rxr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));
							 rsData->PutCollect("ryr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));					 
							 rsData->PutCollect("rzr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RZ"))/180*PI));	
						}
						else
						{
							rsData->PutCollect("rxr1",m_prsDisplacements->GetCollect("RX"));
							rsData->PutCollect("ryr1",m_prsDisplacements->GetCollect("RY"));
							rsData->PutCollect("rzr1",m_prsDisplacements->GetCollect("RZ"));
						}
					}
					else
					{
					}
					//��[INPUT_BASIC_ELEMENT_DATA]������ȡ�¶ȣ��ܾ�д��ZA����	TO_NODE��				

					if ( bReadinDB )
					{
						m_strSQL.Empty();
						m_strSQL.Format(_T("SELECT * FROM [INPUT_BASIC_ELEMENT_DATA] WHERE [JOBNAME] =  '%s' AND [TO_NODE] = %d"),m_strJOBNAME_A,node_end);
						m_prsBasicData = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);

						if (!(m_prsBasicData->BOF && m_prsBasicData->adoEOF))
						{
							rsData->PutCollect("DJ1",m_prsBasicData->GetCollect("DIAMETER"));
							rsData->PutCollect("T01",m_prsBasicData->GetCollect("TEMP_EXP_C1"));
						}
						else
						{
						}
						m_prsBasicData->Close();
					}
					else
					{
						//�Ӷ������ļ��ж�ȡ
						//��ȡCAESARII�Ķ������ļ������л�ȡ�ڵ�ţ��ܾ����¶ȣ�д��ZA����Ӧ�ڵ�ŵĹܾ����¶��ֶ���
						try
						{
							if ( !FileExists(m_strCAESARIIFileName))
								; 
							else
							{
								m_CAESARIIFile.Open(m_strCAESARIIFileName,CFile::modeRead);
								
								for (int i = 0;i<nCount;i++)
								{
									m_CAESARIIFile.Seek(basepoint+4+(i*loff),CFile::begin);
									m_CAESARIIFile.Read(buffer,4);
									node_endfromfile = (long)buffer[0];
									rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_endfromfile) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));;
									if (!(rsData->BOF && rsData->adoEOF))
									{
										m_CAESARIIFile.Seek(basepoint+20+(i*loff),CFile::begin);//��ȡ�ܾ�
										m_CAESARIIFile.Read(buffer,4);
										if ( buffer[0] > 0 )
											tmp1 = buffer[0];
										else 
											buffer[0] = tmp1;
										rsData->PutCollect("DJ1",_variant_t(buffer[0]));			
										
										m_CAESARIIFile.Seek(basepoint+36+(i*loff),CFile::begin);//��ȡ�¶�  
										m_CAESARIIFile.Read(buffer,4);
										if ( buffer[0] > 0 )
											tmp2 = buffer[0];
										else
											buffer[0] = tmp2;
										rsData->PutCollect("T01",_variant_t(buffer[0]));		
									}
									rsData->Filter = "";
								}
								m_CAESARIIFile.Close();
							}

						}catch(CFileException *e)
						{
							e->ReportError ();
						}

					}
					m_prsDisplacements->Close();
					m_prsTempCASENUM->Close();
					m_prsHangers->Close();
					m_prsJGHZ->Close();
				
				}//end if


				
		/***********************�����Ǵ���������¼��FROM_NODE��***********************/
				if (i!=iCount)   //���һ��ѭ��ʱ���ô���FROM_NODE�㣬��һ���Ѿ�������
				{
					rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_start)+" AND [FileNameID]="+ltos(FileNameID));
					if ( rsData->BOF && rsData->adoEOF )
					{
						rsData->AddNew();
						// rsData->PutCollect("UPxyz",_variant_t((long)giUPxyz));
						rsData->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
						rsData->PutCollect("zdjh",_variant_t((long)(ltemp+1)));
						rsData->PutCollect("FileNameID",_variant_t(FileNameID));
						ltemp++;
						rsData->Update();					
					}
					else{
					}

					rsData->PutCollect("PSAver", _variant_t(strPSAVER));
					rsData->PutCollect("UPxyz",_variant_t((long)iUPxyz));
					rsData->PutCollect("JSDBH1", _variant_t((long)node_start));					
					//д��ṹ���أ�FROM_NODE��

					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT DISTINCT [NODE] ,max([RESULTANTF]) AS PJG FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' GROUP BY [NODE] "),node_start,m_strJOBNAME_P);
					m_prsJGHZ = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if(m_prsJGHZ->adoEOF && m_prsJGHZ->BOF)
						;
					else
						rsData->PutCollect("PJG1",m_prsJGHZ->GetCollect("PJG"));					

					rsData->PutCollect("Type", _variant_t(m_strTYPE));
					rsData->PutCollect("psaType", _variant_t(m_strTYPE));

					//��[INPUT_BASIC_ELEMENT_DATA]������ȡ�¶ȣ��ܾ�д��ZA����
					if ( bReadinDB )
					{
						m_strSQL.Empty();
						m_strSQL.Format(_T("SELECT * FROM [INPUT_BASIC_ELEMENT_DATA] WHERE [JOBNAME] =  '%s' AND [FROM_NODE] = %d"),m_strJOBNAME_A,node_start);
						m_prsBasicData = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
						if (!(m_prsBasicData->BOF && m_prsBasicData->adoEOF))
						{
							rsData->PutCollect("DJ1",m_prsBasicData->GetCollect("DIAMETER"));
							rsData->PutCollect("T01",m_prsBasicData->GetCollect("TEMP_EXP_C1"));
						}
						else
						{
						}
						m_prsBasicData->Close();
					}
					else
					{
						//�Ӷ������ļ��ж�ȡ
						//��ȡCAESARII�Ķ������ļ������л�ȡ�ڵ�ţ��ܾ����¶ȣ�д��ZA����Ӧ�ڵ�ŵĹܾ����¶��ֶ���
						try
						{
							if ( !FileExists(m_strCAESARIIFileName))
								;
							else
							{
								m_CAESARIIFile.Open(m_strCAESARIIFileName,CFile::modeRead);
								
								for (int i = 0;i<nCount;i++)
								{
									m_CAESARIIFile.Seek(basepoint+(i*loff),CFile::begin);
									m_CAESARIIFile.Read(buffer,4);
									node_startfromfile = (long)buffer[0];
									rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_startfromfile) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));;
									if (!(rsData->BOF && rsData->adoEOF))
									{
										m_CAESARIIFile.Seek(basepoint+20+(i*loff),CFile::begin);//��ȡ�ܾ�
										m_CAESARIIFile.Read(buffer,4);
										if ( buffer[0] > 0 )
											tmp1 = buffer[0];
										else 
											buffer[0] = tmp1;
										rsData->PutCollect("DJ1",_variant_t(buffer[0]));			
										
										m_CAESARIIFile.Seek(basepoint+36+(i*loff),CFile::begin);//��ȡ�¶�  
										m_CAESARIIFile.Read(buffer,4);
										if ( buffer[0] > 0 )
											tmp2 = buffer[0];
										else
											buffer[0] = tmp2;
										rsData->PutCollect("T01",_variant_t(buffer[0]));		
									}
									rsData->Filter = "";
								}
								m_CAESARIIFile.Close();
							}

						}catch(CFileException *e)
						{
							e->ReportError ();
						}

					}
					
					//�ж�Դ��ĵ�λ�ƣ�д��ZA����			 				 
					if( PSA_OutDataFUnit  == "N." )
						rsData->PutCollect("Unit_Force", "N");
					else
						rsData->PutCollect("Unit_Force", "kgf");
					if (PSA_OutDataMUnit == "N.m.")
						rsData->PutCollect("Unit_Momental", "N.m");
					else
						rsData->PutCollect("Unit_Momental", "kgf.m");


					rsData->PutCollect("pxr1",m_prsCASENUM->GetCollect("FXF"));
					rsData->PutCollect("pyr1",m_prsCASENUM->GetCollect("FYF"));
					rsData->PutCollect("pzr1",m_prsCASENUM->GetCollect("FZF"));
					
					rsData->PutCollect("mxr1",m_prsCASENUM->GetCollect("MXF"));
					rsData->PutCollect("myr1",m_prsCASENUM->GetCollect("MYF"));
					rsData->PutCollect("mzr1",m_prsCASENUM->GetCollect("MZF"));

					//��������ɱ�OUTPUT_HANGERS�в���NODE=FROM_NODE��¼��ʹpgz1=HOT_LOAD,paz0=TH_INSTALL_LOAD,paz1=AC_INSTALL_LOAD,

					m_strSQL.Empty();			
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_HANGERS] WHERE [NODE] = %d AND [JOBNAME] =  '%s'"),node_start,m_strJOBNAME_P);

					m_prsHangers = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					if ( m_prsHangers->BOF && m_prsHangers->adoEOF)
					{
					}
					else
					{	
						// ����֧��������
						CString strSize;
						CString strTemp;
						rsData->PutCollect("pgz1",m_prsHangers->GetCollect("HOT_LOAD"));
						rsData->PutCollect("paz0",m_prsHangers->GetCollect("TH_INSTALL_LOAD"));
						rsData->PutCollect("paz1",m_prsHangers->GetCollect("AC_INSTALL_LOAD"));
						rsData->PutCollect("CoefOfVarLoad",_variant_t(vtof(m_prsHangers->GetCollect("LOAD_VARIATION"))/100));
						tmpvar = m_prsHangers->GetCollect("SIZE");
						strSize.Format(_T("%02d"),vtoi(tmpvar));
					//�γ�����GLIF�����֧�������ͺ�								
						strTemp = vtos(m_prsHangers->GetCollect("NUMREQ"))+"1"+strSize;
						rsData->PutCollect("TYPE",_variant_t(strTemp));							
						rsData->PutCollect("psaTYPE",_variant_t(strTemp));							
					}

				//�����λ�Ʊ�OUTPUT_DISPLACEMENTS�в���FROM_NODE��������CASE��ȵļ�¼,ʹ�ֶ�DX->�ֶ�dxr1,�ֶ�DY->�ֶ�dyr1,�ֶ�DZ->�ֶ�dzr1,

				m_strSQL.Empty();
				m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s'"),node_start,m_strRGKname,m_strJOBNAME_P);
				m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
				if ( !(m_prsDisplacements->BOF && m_prsDisplacements->adoEOF) )
				{
					rsData->PutCollect("xr1",m_prsDisplacements->GetCollect("DX"));
					rsData->PutCollect("yr1",m_prsDisplacements->GetCollect("DY"));
					rsData->PutCollect("zr1",m_prsDisplacements->GetCollect("DZ"));

					tmpvar = m_prsDisplacements->GetCollect("DUNITS");
					if (vtos(tmpvar) == "mm")
						rsData->PutCollect("UNIT_Displacement",_variant_t(tmpvar));
					
					tmpvar = m_prsDisplacements->GetCollect("RUNITS");
					if (vtos(tmpvar) == "deg.")
					{
						 tmpvar = m_prsDisplacements->GetCollect("RX");
						 rsData->PutCollect("rxr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));
						 rsData->PutCollect("ryr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));					 
						 rsData->PutCollect("rzr1",_variant_t(vtof(m_prsDisplacements->GetCollect("RZ"))/180*PI));	
					}
					else
					{
						rsData->PutCollect("rxr1",m_prsDisplacements->GetCollect("RX"));
						rsData->PutCollect("ryr1",m_prsDisplacements->GetCollect("RY"));
						rsData->PutCollect("rzr1",m_prsDisplacements->GetCollect("RZ"));
					}
				}
				else
				{
				}	
 				m_prsDisplacements->Close();
				m_prsHangers->Close();
				m_prsJGHZ->Close();
			}
			//���汾����¼��TO_NODE�㣬��������һ����¼��FROM_NODE�Ƚ�
			node_end = vtoi(m_prsCASENUM->GetCollect("TO_NODE"));  

			if ( i < iCount-1 )
				m_prsCASENUM->MoveNext();
			
			iC1++;
			frmStatus.UpdateStatus((float)iC1/(float)iCount,true);
			frmStatus.UpdateWindow();
			rsData->Filter = "";
			
		   
			nCount++;
		}//  end  for
		m_prsCASENUM->Close();

      m_strSQL.Empty();
		//��ȡ��̬��¼��
		m_strSQL.Format(_T("SELECT * FROM [OUTPUT_LOCAL_ELEMENT_FORCES] WHERE [CASE] = '%s' AND [JOBNAME] =  '%s'"),m_strLGKname,m_strJOBNAME_P);
		m_prsCASENUM = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
		node_end = -1;
				
/******************************��ʼ������̬����********************************/		
		for (i=0; i<= iCount; i++)
		{
				tmpvar = m_prsCASENUM->GetCollect("FROM_NODE");
				node_start = vtoi(tmpvar);
				if (i==0)
					node_end = node_start;
		/***********************����TO_NODE��¼**************************/
				if (node_start != node_end ) 
				{
					//����������node_end��(һ������,��̬�Ѿ�����)������Ӧ����̬�ֶ���ȥ
					
					rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_end) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));;
										
					//д����������					
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_LOCAL_ELEMENT_FORCES] WHERE [TO_NODE] = %d AND [CASE] = '%s' "),node_end,m_strLGKname);
					m_prsTempCASENUM = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' AND [CASE] = '%s' AND [TYPE] = 'Rigid ANC' "),node_end,m_strJOBNAME_P,m_strGKname);	
					m_prsRestraints = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					
					if (m_prsRestraints->adoEOF && m_prsRestraints->BOF)   //���ǹ̶���
					{
						rsData->PutCollect("pxl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FXT")))));
						rsData->PutCollect("pyl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FYT")))));
						rsData->PutCollect("pzl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("FZT")))));

						rsData->PutCollect("mxl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MXT")))));
						rsData->PutCollect("myl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MYT")))));
						rsData->PutCollect("mzl1",_variant_t(0-(vtof(m_prsTempCASENUM->GetCollect("MZT")))));
					}
					else																	//�ǹ̶���
					{
						rsData->PutCollect("pxl1",m_prsRestraints->GetCollect("FX"));
						rsData->PutCollect("pyl1",m_prsRestraints->GetCollect("FY"));
						rsData->PutCollect("pzl1",m_prsRestraints->GetCollect("FZ"));

						rsData->PutCollect("mxl1",m_prsRestraints->GetCollect("MX"));
						rsData->PutCollect("myl1",m_prsRestraints->GetCollect("MY"));
						rsData->PutCollect("mzl1",m_prsRestraints->GetCollect("MZ"));
					}

					//�����λ�Ʊ�OUTPUT_DISPLACEMENTS�в���FROM_NODE��������CASE��ȵļ�¼,ʹ�ֶ�DX->�ֶ�dxr1,�ֶ�DY->�ֶ�dyr1,�ֶ�DZ->�ֶ�dzr1					
					
					if(m_prsRestraints->adoEOF && m_prsRestraints->BOF)   //������ǹ̶���
					{
						m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s' "),node_end,m_strLGKname,m_strJOBNAME_P);						
						m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					}
					else     //�ǹ̶���
					{
						m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [CASE] = '%s' AND [JOBNAME] = '%s' AND [NODE] = %d"),m_strGKname,m_strJOBNAME_P,node_end);
						m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					}
							
					if ( !(m_prsDisplacements->BOF && m_prsDisplacements->adoEOF))
					{
						rsData->PutCollect("xl1",m_prsDisplacements->GetCollect("DX"));
						rsData->PutCollect("yl1",m_prsDisplacements->GetCollect("DY"));
						rsData->PutCollect("zl1",m_prsDisplacements->GetCollect("DZ"));

						tmpvar = m_prsDisplacements->GetCollect("DUNITS");
						if (vtos(tmpvar) == "mm")
							rsData->PutCollect("UNIT_Displacement",_variant_t(tmpvar));
						
						tmpvar = m_prsDisplacements->GetCollect("RUNITS");
						if (vtos(tmpvar) == "deg.")
						{
							 rsData->PutCollect("rxl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RX"))/180*PI));
							 rsData->PutCollect("ryl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));					 
							 rsData->PutCollect("rzl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RZ"))/180*PI));	
						}
						else
						{
							rsData->PutCollect("rxl1",m_prsDisplacements->GetCollect("RX"));
							rsData->PutCollect("ryl1",m_prsDisplacements->GetCollect("RY"));
							rsData->PutCollect("rzl1",m_prsDisplacements->GetCollect("RZ"));
						}
					}
					else
					{
					}
					m_prsDisplacements->Close();
					m_prsTempCASENUM->Close();
					m_prsRestraints->Close();				
					rsData->Filter = "";
				}//end if				
				
	/*********************����FROM_NODE��***********************/
				if( i != iCount )
				{
					rsData->Filter = _variant_t(CString("[jsdbh1] = ") + ltos(node_start) +" AND "+CString("[FileNameID]=")+ltos(FileNameID));;
					m_strSQL.Empty();
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_RESTRAINTS] WHERE [NODE] = %d AND [JOBNAME] = '%s' AND [CASE] = '%s' AND [TYPE] = 'Rigid ANC' "),node_start,m_strJOBNAME_P,m_strGKname);	
					m_prsRestraints = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
					
					if (m_prsRestraints->adoEOF && m_prsRestraints->BOF) //�ǹ̶��� 
					{
						rsData->PutCollect("pxl1",m_prsCASENUM->GetCollect("FXF"));
						rsData->PutCollect("pyl1",m_prsCASENUM->GetCollect("FYF"));
						rsData->PutCollect("pzl1",m_prsCASENUM->GetCollect("FZF"));

						rsData->PutCollect("mxl1",m_prsCASENUM->GetCollect("MXF"));
						rsData->PutCollect("myl1",m_prsCASENUM->GetCollect("MYF"));
						rsData->PutCollect("mzl1",m_prsCASENUM->GetCollect("MZF"));
					}
					else																  //���ǹ̶���
					{
						rsData->PutCollect("pxl1",m_prsRestraints->GetCollect("FX"));
						rsData->PutCollect("pyl1",m_prsRestraints->GetCollect("FY"));
						rsData->PutCollect("pzl1",m_prsRestraints->GetCollect("FZ"));

						rsData->PutCollect("mxl1",m_prsRestraints->GetCollect("MX"));
						rsData->PutCollect("myl1",m_prsRestraints->GetCollect("MY"));
						rsData->PutCollect("mzl1",m_prsRestraints->GetCollect("MZ"));
					}
				
				//�����λ�Ʊ�OUTPUT_DISPLACEMENTS�в���FROM_NODE��������CASE��ȵļ�¼,ʹ�ֶ�DX->�ֶ�dxr1,�ֶ�DY->�ֶ�dyr1,�ֶ�DZ->�ֶ�dzr1,
				m_strSQL.Empty();
				if(m_prsRestraints->adoEOF && m_prsRestraints->BOF)   //������ǹ̶���
				{
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [NODE] = %d AND [CASE] = '%s' AND [JOBNAME] =  '%s' "),node_start,m_strLGKname,m_strJOBNAME_P);
					m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
				}
				else     //�ǹ̶���
				{
					m_strSQL.Format(_T("SELECT * FROM [OUTPUT_DISPLACEMENTS] WHERE [CASE] = '%s' AND [JOBNAME] = '%s' AND [NODE] = %d"),m_strGKname,m_strJOBNAME_P,node_start);
					m_prsDisplacements = m_pConnSourceDB->Execute(_bstr_t(m_strSQL),NULL,adCmdText);
				}

				if ( !(m_prsDisplacements->BOF && m_prsDisplacements->adoEOF))
				{
					rsData->PutCollect("xl1",m_prsDisplacements->GetCollect("DX"));
					rsData->PutCollect("yl1",m_prsDisplacements->GetCollect("DY"));
					rsData->PutCollect("zl1",m_prsDisplacements->GetCollect("DZ"));

					tmpvar = m_prsDisplacements->GetCollect("DUNITS");
					if (vtos(tmpvar) == "mm")
						rsData->PutCollect("UNIT_Displacement",_variant_t(tmpvar));
					
					tmpvar = m_prsDisplacements->GetCollect("RUNITS");
					if (vtos(tmpvar) == "deg.")
					{
						 rsData->PutCollect("rxl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RX"))/180*PI));
						 rsData->PutCollect("ryl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RY"))/180*PI));					 
						 rsData->PutCollect("rzl1",_variant_t(vtof(m_prsDisplacements->GetCollect("RZ"))/180*PI));	
					}
					else
					{
						rsData->PutCollect("rxl1",m_prsDisplacements->GetCollect("RX"));
						rsData->PutCollect("ryl1",m_prsDisplacements->GetCollect("RY"));
						rsData->PutCollect("rzl1",m_prsDisplacements->GetCollect("RZ"));
					}
				}
				else
				{
				}
			m_prsDisplacements->Close();
			m_prsRestraints->Close();
			}	
			//������μ�¼��TO_NODE�㣬����һ����¼��FROM_NODE�Ƚ�			
			node_end = vtoi(m_prsCASENUM->GetCollect("TO_NODE"));

			if ( i < iCount-1 )
				m_prsCASENUM->MoveNext();
			
			rsData->Filter = "";
				
		}//  end  for
		m_prsCASENUM = NULL;					
		m_prsHangers = NULL;			
		m_prsDisplacements = NULL;	
		m_prsRestraints = NULL;		
		m_prsJGHZ = NULL;				
		m_prsJOBNAME = NULL;		
		m_prsBasicData = NULL;		
		m_prsTempCASENUM = NULL;		

		m_pConnSourceDB->Close();
		m_pConnSourceDB = NULL;
		
	}//   end   try
	catch(CException *e)
	{
		e->ReportError();
		e->Delete();
	}
}

typedef struct _tagSWEDChildNodeInfo{
	int iStartNode;
	int iNum;
} tagSWEDChildNodeInfo,FAR * LPSWEDChildNodeInfo;

typedef CList<tagSWEDChildNodeInfo,tagSWEDChildNodeInfo&> tagListSWEDChildNodeInfo;

int GetSWEDChildNodeNum(tagListSWEDChildNodeInfo* pLstChildNodeInfo,int iStartNode)
{
	POSITION pos=pLstChildNodeInfo->GetHeadPosition();
	tagSWEDChildNodeInfo ChildNodeInfo;
	while(pos)
	{
		ChildNodeInfo=pLstChildNodeInfo->GetNext(pos);
		if(ChildNodeInfo.iStartNode==iStartNode)
			return ChildNodeInfo.iNum;
	}
	return 0;
}

double GetParam(CString & strText)
{
	LPTSTR lpsz;
	double res=_tcstod(strText,&lpsz);
	strText.Delete(0,lpsz-(LPCTSTR)strText);
	return res;
}

void ReadResult_SWEDPSA(_Recordset* rsResult ,CString SourceDataFileName,long maxZdjh,long FileNameID)
{
   //Remauther:
   //����:
   //Ŀ��:������Ϊ�����ϵ���ԺӦ�����������������ļ�*.dat�ļ�
   //����:rsResult���ݱ��ڲ���λ:λ��mm,����kgf.
   //����:
   //�޸�:�Լ�
   //�޸�ԭ��:����Ժ��Ӧ����������ļ������ĵĴ���
	try
	{
		
		long ltemp=maxZdjh;
		CString strOldPhsSPRINGSel;
		CString strTemp, temp ;int iTemp=0;
		CString strSprTbn;
		CString strSQL;
		_RecordsetPtr tmprs;
		tmprs.CreateInstance(__uuidof(Recordset));
		_variant_t varTmp;
		tagListSWEDChildNodeInfo ListChildNodeInfo;
		tagSWEDChildNodeInfo ChildNodeInfo;
		const char* strPIPING_STRESS_OUTUNIT = "UNIT OF OUTPUTDATA IS";
		const char* strSUMMARY_FOR_RESTRAINTS = "SUMMARY FOR RESTRAINTS";
		const char* strCOLD_SPRING_TABLE = "COLD SPRINGS TABLE";
		const char* strEND_RawData = "10       1       1         1";
		const char* strPSAVER = "XNP 2.0";
		const char* strSPRINGType = "SPRING VERSION:";
		const char* strEND_THIS_PIPING = "*THIS PIPING STRUCTURE *CONTAINS*";
		POSITION pos,pos2;

		float sngDWTemp=0, TjTemp=0;
		long i=0;
		int iJSJDH_start=0, iJSJDH_end=0;
		int j=0, iC1=0;
		int iFlag=0, iCount=0;
		int mviSPR=0;
		CString mvS;
		CArray<tagVector,tagVector&> arrVector;
		arrVector.SetSize(500,500);
		int iVectorCount=0;

		//CArray<int,int> arrPhsNode;
		//CArray<int,int> arrPhsType;//
		CList<_PhsInfo,_PhsInfo&> list_PhsInfo;
		CList<float,float> listPhsDW;
		CList<float,float> listPhsTJ;
		CList<SECT,SECT&> listSECT;
		CList<PhsInfo,PhsInfo&> listPhsInfo;
		

		const char* pszZTS = "ZTS=";//֧������Ϣ
		const char* pszGZ  = "GZ="; //�ܲ�
		const char* pszGW  = "GW="; //����
		const char* pszPNM = "PNM=";//��֧����Ϣ

		
		ifstream f;
		frmStatus.ShowWindow(SW_SHOW);
		frmStatus.UpdateStatus(0.0f);
		frmStatus.UpdateWindow();
		//XNPֻ���÷�����λ���
		EDIBgbl::PSA_OutDataUnit = "NATIONAL";
		//�����᷽��x/y/z������=1/2/3
		//XNPֻ��Z������
		giUPxyz = 3;
		//XNPֻ��ѡ������Ժ֧�����ֲᵯ������
		//modPHScal::gnSPR_CHZBH = 0.35f;
		strOldPhsSPRINGSel = modPHScal::gsPhsSPRINGSel;
		//��ΪӦ������ѡ��ĵ��ɱ�׼���ܲ�ͬ,�����¸��¹淶
		//�������ɹ淶����ʱ��ϳ�������ֻ�е��淶�仯ʱ������Ҫ���¸���
		strSQL = "SELECT Spring_property FROM phsManuSPRING WHERE StandardDesc=\'";
		strSQL += strSprNWEPDI;
		strSQL += "\'";
		tmprs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
			adOpenKeyset, adLockOptimistic, adCmdText); 
		if(tmprs->BOF && tmprs->adoEOF)
		{
		}
		else
		{
			tmprs->get_Collect((_variant_t)0L, &varTmp);
			strSprTbn=vtos(varTmp);
		}
		tmprs->Close();
		if( strOldPhsSPRINGSel != strSprNWEPDI )
		{
			modPHScal::gsPhsSPRINGSel = strSprNWEPDI;					
		}

		f.open(SourceDataFileName);
		//��ʼ��ԭʼ����
		temp=_T("");
		while(!f.eof())
		{
			if((i=temp.Find(_T(" IC=")))!=-1)
			{
				int iIC=_ttoi(temp.Mid(i+4));
				if(iIC==0)
					modPHScal::gnSPR_CHZBH=0.35f;
				else if(iIC==1)
					modPHScal::gnSPR_CHZBH=0.25f;
				//���ر仯ϵ�����浽�ⲿ
				::SetRegValue(_T("Settings"), "SPR_Chzbh", (modPHScal::gnSPR_CHZBH));
			}
//			int ttt = temp[0];
			if(temp.GetLength()>0 && !(temp[0]>=_T('0')&&temp[0]<=_T('9'))/*_T('A') <= temp[0] && temp[0]<=_T('Z')*/)
			{
				if( temp.Find(pszZTS) != -1 )
				{
					do  //����ǰ��Ŀ���
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}while(temp.GetLength()==0 && !f.eof());
//					ttt = temp[0];
					while(temp.GetLength()>0 && /*(temp[0] < _T('A')  ||  _T('Z') < temp[0])*/((temp[0]>=_T('0')&&temp[0]<=_T('9'))||temp[0]==_T('.')) && !f.eof())
					{//֧������Ϣ ��"ZTS="�ؼ��ֺ�
												
						_PhsInfo tmp;
						tmp.iNode = GetParam(temp)*10.0f;
						tmp.lPhsType = GetParam(temp);
						list_PhsInfo.AddTail(tmp);
						GetParam(temp);
						temp.TrimRight();
						if(temp.GetLength()>0)
						{
							tmp.iNode = GetParam(temp)*10.0f;
							tmp.lPhsType = GetParam(temp);
							list_PhsInfo.AddTail(tmp);
						}
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}
				}
				else if(temp.Find("GZ=")!=-1)
				{
					do  //����ǰ��Ŀ���
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}while(temp.GetLength()==0 && !f.eof());
					while(temp.GetLength()>0 && /*(temp[0] < _T('A')  ||  _T('Z') < temp[0])*/((temp[0]>=_T('0')&&temp[0]<=_T('9'))||temp[0]==_T('.')) && !f.eof())
					{//�ܾ���Ϣ ��"GZ="�ؼ��ֺ�
						GetParam(temp);
						listPhsDW.AddTail(GetParam(temp)*10.0f);
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}
				}
				else if(temp.Find("GW=")!=-1)
				{
					do  //����ǰ��Ŀ���
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}while(temp.GetLength()==0 && !f.eof());
					while(temp.GetLength()>0 && /*(temp[0] < _T('A')  ||  _T('Z') < temp[0])*/ ((temp[0]>=_T('0')&&temp[0]<=_T('9'))||temp[0]==_T('.')) && !f.eof())
					{//������Ϣ ��"GW="�ؼ��ֺ�

						listPhsTJ.AddTail(GetParam(temp));
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}
				}
				else if(temp.Find(_T("FS="))!=-1)
				{
					do  //����ǰ��Ŀ���
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}while(temp.GetLength()==0 && !f.eof());
					while(temp.GetLength()>0 && ((temp[0]>=_T('0')&&temp[0]<=_T('9'))||temp[0]==_T('.'))/*(temp[0] < _T('A')  ||  _T('Z') < temp[0])*/ && !f.eof())
					{//��֧��Ϣ  ��"FS="�ؼ��ֺ�
						
						SECT tmpSECT;
						tmpSECT.iDWIndex = GetParam(temp);
						tmpSECT.iTJIndex = GetParam(temp);
						GetParam(temp);
						GetParam(temp);
						tmpSECT.iElementNums = GetParam(temp);
						listSECT.AddTail(tmpSECT);
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}
				}
				else if(temp.Find("PNM=")!=-1)
				{
					do  //����ǰ��Ŀ���
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.TrimRight();
						temp.MakeUpper();
					}while(temp.GetLength()==0 && !f.eof());
					while(temp.GetLength()>0 && /*(temp[0] < _T('A')  ||  _T('Z') < temp[0])*/((temp[0]>=_T('0')&&temp[0]<=_T('9'))||temp[0]==_T('.')) && !f.eof())
					{//��ӵ�Ԫ����Ϣ  ��"PNM="�ؼ��ֺ�
						while(temp.GetLength()!=0)
						{
							ChildNodeInfo.iStartNode=GetParam(temp);
							ChildNodeInfo.iNum=GetParam(temp);
							ListChildNodeInfo.AddTail(ChildNodeInfo);
						}
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.TrimRight();
						temp.MakeUpper();
					}
				}
				else if(temp.Find("ES=")!=-1)
				{
					do  //����ǰ��Ŀ���
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.TrimRight();
						temp.MakeUpper();
					}while(temp.GetLength()==0 && !f.eof());
					/////z.jia add/////
					int ifind = temp.Find("���");
					if(ifind!=-1)
						temp = temp.Right(temp.GetLength()-ifind-4);
					/////z.jia add////
					while(temp.GetLength()>0 && /*(temp[0] < _T('A')  ||  _T('Z') < temp[0])*/((temp[0]>=_T('0')&&temp[0]<=_T('9'))||temp[0]==_T('.')) && !f.eof())
					{//��ӵ�Ԫ����Ϣ  ��"ES="�ؼ��ֺ�						
						GetParam(temp);
						tagVector vecTmp;
						vecTmp.x=GetParam(temp);
						vecTmp.y=GetParam(temp);
						vecTmp.z=GetParam(temp);
						arrVector.SetAtGrow(iVectorCount++,vecTmp);
						f.getline(temp.GetBuffer(255),255);
						////z.jia add//
						ifind = temp.Find("���");
						if(ifind)
							temp = temp.Right(temp.GetLength()-ifind-4);
						//////
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.TrimRight();
						temp.MakeUpper();
					}
					break;
				}
				else
				{
					do  //����ǰ��Ŀ���
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}while(temp.GetLength()==0 && !f.eof());
					while(temp.GetLength()>0 && /*(temp[0] < _T('A')  ||  _T('Z') < temp[0])*/((temp[0]>=_T('0')&&temp[0]<=_T('9'))||temp[0]==_T('.')) && !f.eof())
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						temp.MakeUpper();
					}
				}
			}
			else
			{	
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				temp.TrimLeft();
				temp.MakeUpper();
			}
		}
		f.close();
		frmStatus.UpdateStatus(0.02f,true);
		frmStatus.UpdateWindow();

		f.open(SourceDataFileName);
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			if(temp.Find("FULCRUM DISPLACEMENT")!=-1)
			{
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				do
				{//�ǵ�����̬λ��

					PhsInfo tmpPhsInfo;

					tmpPhsInfo.iCalNodeNumber = GetParam(temp)*10.0f;
					tmpPhsInfo.fLX = GetParam(temp)*10.0f;
					tmpPhsInfo.fLY = GetParam(temp)*10.0f;
					tmpPhsInfo.fLZ = GetParam(temp)*10.0f;
					tmpPhsInfo.fQX = GetParam(temp);
					tmpPhsInfo.fQY = GetParam(temp);
					tmpPhsInfo.fQZ = GetParam(temp);
					listPhsInfo.AddTail(tmpPhsInfo);
					
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
					temp.TrimLeft();
				}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) &&!f.eof());

				do
				{//�ǵ�����̬����
					if( temp.Find("SUPPORT LOAD")!= -1)
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						do
						{
							PhsInfo tmpPhsInfo;

							tmpPhsInfo.iCalNodeNumber = GetParam(temp)*10;
							tmpPhsInfo.fPX = GetParam(temp);        //update cheng
							tmpPhsInfo.fPY = GetParam(temp);
							tmpPhsInfo.fPZ = GetParam(temp);
							tmpPhsInfo.fMX = GetParam(temp);
							tmpPhsInfo.fMY = GetParam(temp);
							tmpPhsInfo.fMZ = GetParam(temp);
							bool bFound=false;
							pos=listPhsInfo.GetHeadPosition();
							while(pos)
							{	
								PhsInfo& infoTmp=listPhsInfo.GetNext(pos);
								if(infoTmp.iCalNodeNumber == tmpPhsInfo.iCalNodeNumber )
								{
									bFound=true;
									infoTmp.fPX=tmpPhsInfo.fPX;    //update cheng
									infoTmp.fPY=tmpPhsInfo.fPY;
									infoTmp.fPZ=tmpPhsInfo.fPZ;
									infoTmp.fMX=tmpPhsInfo.fMX;
									infoTmp.fMY=tmpPhsInfo.fMY;
									infoTmp.fMZ=tmpPhsInfo.fMZ;
								}
							}
							if(!bFound)
								listPhsInfo.AddTail(tmpPhsInfo);
							f.getline(temp.GetBuffer(255),255);
							temp.ReleaseBuffer();
							temp.TrimLeft();
						}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) &&!f.eof());
						break;
					}
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
				}while(!f.eof());

			}
		}
		f.close();

		frmStatus.UpdateStatus(0.1f,true);
		frmStatus.UpdateWindow();

		f.open(SourceDataFileName);
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			if(temp.Find("COLD PULL DISPLACEMENT")!=-1)
			{
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				do
				{//����֧������̬λ��

					PhsInfo tmpPhsInfo;

					tmpPhsInfo.iCalNodeNumber = GetParam(temp)*10;
					tmpPhsInfo.fCoolLX = GetParam(temp)*10.0f;
					tmpPhsInfo.fCoolLY = GetParam(temp)*10.0f;
					tmpPhsInfo.fCoolLZ = GetParam(temp)*10.0f;
					tmpPhsInfo.fCoolQX = GetParam(temp);
					tmpPhsInfo.fCoolQY = GetParam(temp);
					tmpPhsInfo.fCoolQZ = GetParam(temp);
					bool bFound=false;
					pos=listPhsInfo.GetHeadPosition();
					while(pos)
					{	
						PhsInfo& infoTmp=listPhsInfo.GetNext(pos);
						if(infoTmp.iCalNodeNumber == tmpPhsInfo.iCalNodeNumber )
						{
						    bFound=true;
							infoTmp.fCoolLX = tmpPhsInfo.fCoolLX;
							infoTmp.fCoolLY = tmpPhsInfo.fCoolLY;
							infoTmp.fCoolLZ = tmpPhsInfo.fCoolLZ;
							infoTmp.fCoolQX = tmpPhsInfo.fCoolQX;
							infoTmp.fCoolQY = tmpPhsInfo.fCoolQY;
							infoTmp.fCoolQZ = tmpPhsInfo.fCoolQZ;
						}
					}
					if( !bFound )
						listPhsInfo.AddTail(tmpPhsInfo);

				
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
					temp.TrimLeft();
				}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) &&!f.eof());

				do
				{//����֧������̬����
					if( temp.Find("SUPPORT LOAD")!= -1)
					{
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						do
						{
							PhsInfo tmpPhsInfo;

							tmpPhsInfo.iCalNodeNumber = GetParam(temp)*10;
							tmpPhsInfo.fCoolPX = GetParam(temp);
							tmpPhsInfo.fCoolPY = GetParam(temp);
							tmpPhsInfo.fCoolPZ = GetParam(temp);
							tmpPhsInfo.fCoolMX = GetParam(temp);
							tmpPhsInfo.fCoolMY = GetParam(temp);
							tmpPhsInfo.fCoolMZ = GetParam(temp);

							bool bFound=false;
							pos=listPhsInfo.GetHeadPosition();
							
							while(pos)
							{	
								PhsInfo& infoTmp=listPhsInfo.GetNext(pos);
								if(infoTmp.iCalNodeNumber == tmpPhsInfo.iCalNodeNumber )
								{
									bFound=true;
									infoTmp.fCoolPX = tmpPhsInfo.fCoolPX;
									infoTmp.fCoolPY = tmpPhsInfo.fCoolPY;
									infoTmp.fCoolPZ = tmpPhsInfo.fCoolPZ;
									infoTmp.fCoolMX = tmpPhsInfo.fCoolMX;
									infoTmp.fCoolMY = tmpPhsInfo.fCoolMY;
									infoTmp.fCoolMZ = tmpPhsInfo.fCoolMZ;
								}
							}
							if( !bFound )
								listPhsInfo.AddTail(tmpPhsInfo);
							
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) &&!f.eof());
						break;
					}
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
				}while(!f.eof());
			}
		}
		f.close();
   

		frmStatus.UpdateStatus(0.2f,true);
		frmStatus.UpdateWindow();

		f.open(SourceDataFileName);
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
//			if(temp.Find("SPRING TABLE <1>")!=-1)
			if(temp.Find("SPRING TABLE")!=-1 && temp.Find("<1>")!=-1)
			{
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();
				while(temp.Find("HAO")==-1)
				{
					//��Ѱ��HAO�ַ������е�������������Ϣ
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
				}
				do
				{
					PhsInfo tmpPhsInfo;
					tmpPhsInfo.iCalNodeNumber = GetParam(temp)*10.0f;
					int iHA,iBI,iCHM,iCHN;
					float fDAZ,fCAZ;
					iHA=GetParam(temp);
					iBI=GetParam(temp);
					iCHM=GetParam(temp);
					fDAZ=GetParam(temp);
					iCHN=GetParam(temp);
					fCAZ=GetParam(temp);
					if( iHA == 0 )
					{
						tmpPhsInfo.lZDJType = 9999;
					}
					else
					{
						tmpPhsInfo.lZDJType = iHA + iBI*100 + (iCHM*2+iCHN)*1000;
					}
					pos=list_PhsInfo.GetHeadPosition();
					while(pos)
					{//ֻ�е���֧���ܲ�ת��������ΪAutoPSA�ĵ���֧��������
						tag_PhsInfo& infoTmp=list_PhsInfo.GetNext(pos);
						if(infoTmp.iNode == tmpPhsInfo.iCalNodeNumber )
						{
							if( infoTmp.lPhsType == 5 || infoTmp.lPhsType == 6)
							{
								bool bFound=false;
								pos2=listPhsInfo.GetHeadPosition();
								while(pos2)
								{	
									PhsInfo & infoTmp2=listPhsInfo.GetNext(pos2);
									if(infoTmp2.iCalNodeNumber == tmpPhsInfo.iCalNodeNumber )
									{//�Ѵ��ڵĽڵ㣬ֻ���޸���������Ϣ
										bFound=true;
										infoTmp2.lZDJType = tmpPhsInfo.lZDJType;
									}
								}
								if( !bFound )
									listPhsInfo.AddTail(tmpPhsInfo);
							}
						}
					}
				
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
					temp.TrimLeft();
				}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) /*&&!f.eof()*/ && temp.Find("SPRING TABLE")==-1 && temp.Find("<2>")==-1);


				do
				{
//					if(temp.Find("SPRING TABLE <2>")!=-1)
					if(temp.Find("SPRING TABLE")!=-1 && temp.Find("<2>")!=-1)
					{//����֧������̬����
						while(temp.Find("PGZ")==-1)
						{
							//��Ѱ��HAO�ַ������е�������������Ϣ
							f.getline(temp.GetBuffer(255),255);
							temp.ReleaseBuffer();
						}
						do
						{
							PhsInfo tmpPhsInfo;
							tmpPhsInfo.iCalNodeNumber = GetParam(temp)*10.0f;
							tmpPhsInfo.fPGZ = GetParam(temp);
							tmpPhsInfo.fPAZ = GetParam(temp);
							tmpPhsInfo.fPJG = GetParam(temp);
							tmpPhsInfo.fLX = GetParam(temp)*10.0f;
							tmpPhsInfo.fLY = GetParam(temp)*10.0f;
							tmpPhsInfo.fLZ = GetParam(temp)*10.0f;
							tmpPhsInfo.fPZ=tmpPhsInfo.fPGZ;
							tmpPhsInfo.fCoolPZ=tmpPhsInfo.fPAZ;
							bool bFound=false;
							pos=listPhsInfo.GetHeadPosition();
							while(pos)
							{	
								PhsInfo& infoTmp=listPhsInfo.GetNext(pos);
								if(infoTmp.iCalNodeNumber == tmpPhsInfo.iCalNodeNumber )
								{//�Ѵ��ڵĽڵ㣬ֻ���޸���������Ϣ
									bFound=true;
									infoTmp.fPGZ = tmpPhsInfo.fPGZ;
									infoTmp.fPAZ = tmpPhsInfo.fPAZ;
									infoTmp.fPJG = tmpPhsInfo.fPJG;
									infoTmp.fLX = tmpPhsInfo.fLX;
									infoTmp.fLY = tmpPhsInfo.fLY;
									infoTmp.fLZ = tmpPhsInfo.fLZ;
									infoTmp.fPZ = tmpPhsInfo.fPZ;
									infoTmp.fCoolPZ = tmpPhsInfo.fCoolPZ;
								}
							}
							if( !bFound )
								listPhsInfo.AddTail(tmpPhsInfo);
								
						f.getline(temp.GetBuffer(255),255);
						temp.ReleaseBuffer();
						temp.TrimLeft();
						}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) &&!f.eof());
					}
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
					temp.TrimLeft();
				}while(!f.eof());
			}
		}
		f.close();

		frmStatus.UpdateStatus(0.3f,true);
		frmStatus.UpdateWindow();


		f.open(SourceDataFileName);//SourceDataFileName
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			if(temp.Find("JIE GOU SUPPORT LOAD")!=-1)
			{
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();

				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();

				do
				{
					temp.TrimLeft();temp.TrimRight();
					PhsInfo tmpPhsInfo;
					tmpPhsInfo.iCalNodeNumber = GetParam(temp)*10.0f;
					GetParam(temp);
					GetParam(temp);
					tmpPhsInfo.fPJG=GetParam(temp);
					bool bFound=false;
					pos=listPhsInfo.GetHeadPosition();
					while(pos)
					{	
						PhsInfo& infoTmp = listPhsInfo.GetNext(pos);
						if(infoTmp.iCalNodeNumber == tmpPhsInfo.iCalNodeNumber )
						{//�Ѵ��ڵĽڵ㣬ֻ���޸���������Ϣ
							bFound=true;
							infoTmp.fPJG = tmpPhsInfo.fPJG;
						}
					}
					if(!bFound)
						listPhsInfo.AddTail(tmpPhsInfo);
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
					temp.TrimLeft();

				}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) &&!f.eof());

			}

		}
		f.close();

		frmStatus.UpdateStatus(0.4f,true);
		frmStatus.UpdateWindow();

		f.open(SourceDataFileName);
		while(!f.eof())
		{
			f.getline(temp.GetBuffer(255),255);
			temp.ReleaseBuffer();
			if(temp.Find("WORKING SUPPORT LOAD")!=-1)
			{
				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();

				f.getline(temp.GetBuffer(255),255);
				temp.ReleaseBuffer();

				do
				{
					temp.TrimLeft();temp.TrimRight();
					PhsInfo tmpPhsInfo;
					tmpPhsInfo.iCalNodeNumber = GetParam(temp)*10.0f;
					GetParam(temp);
					GetParam(temp);
					tmpPhsInfo.fPGZ=GetParam(temp);
					bool bFound=false;
					pos=listPhsInfo.GetHeadPosition();
					while(pos)
					{	
						PhsInfo& infoTmp=listPhsInfo.GetNext(pos);
						if(infoTmp.iCalNodeNumber == tmpPhsInfo.iCalNodeNumber )
						{//�Ѵ��ڵĽڵ㣬ֻ���޸���������Ϣ
							bFound=true;
							infoTmp.fPGZ = tmpPhsInfo.fPGZ;
						}
					}
					if(!bFound)
						listPhsInfo.AddTail(tmpPhsInfo);
					f.getline(temp.GetBuffer(255),255);
					temp.ReleaseBuffer();
					temp.TrimLeft();	
				}while(temp.GetLength() >0 && ((_T('0') <= temp[0] && temp[0] <= _T('9')) || temp[0]==_T('.')) &&!f.eof());

			}

		}
		f.close();
		
		frmStatus.UpdateStatus(0.6f,true);
		frmStatus.UpdateWindow();

		//��ʼд�����ݿ�
		pos=listSECT.GetHeadPosition();
		int iNodeNo=1;
		int iTrueNodeNo=0;
		int iZdjh=maxZdjh+1;
		CString strFilter;
		float fStatusPos=0.6f;
		int iVectorIndex=0;
		CList<int,int &> listTrueNodeNo;
		while(pos)
		{
			int i,j,k;
			tagSECT &tmpSect=listSECT.GetNext(pos);
			for(i=0;i<tmpSect.iElementNums-1;i++)
			{
				int ChildNum=GetSWEDChildNodeNum(&ListChildNodeInfo,iNodeNo);
				float fDw=0.0f,fTj=0.0f;
				pos2=listPhsDW.GetHeadPosition();
				k=1;
				while(pos2)
				{
					if(k==tmpSect.iDWIndex)
						fDw=listPhsDW.GetNext(pos2);
					else
						listPhsDW.GetNext(pos2);
					k++;
				}
				pos2=listPhsTJ.GetHeadPosition();
				k=1;
				while(pos2)
				{
					if(k==tmpSect.iTJIndex)
						fTj=listPhsTJ.GetNext(pos2);
					else
						listPhsTJ.GetNext(pos2);
					k++;
				}

				for(j=0;j<=ChildNum;j++)
				{
					float fNodeNo=iNodeNo+j*0.1f;
					iTrueNodeNo=(int)(fNodeNo*10.0f);
					if(iTrueNodeNo==560)
					{
						int iDbg=0;
					}
					BOOL bFound=FALSE;
					pos2=list_PhsInfo.GetHeadPosition();
					int iPhsType=0;
					while(pos2)
					{
						_PhsInfo& infoTmp=list_PhsInfo.GetNext(pos2);
						if(infoTmp.iNode==iTrueNodeNo)
						{
							bFound=TRUE;
							iPhsType=infoTmp.lPhsType;
							break;
						}
					}
					TRACE(_T("node=%d\n"),iTrueNodeNo);
					if(bFound)
					{
						listTrueNodeNo.AddTail(iTrueNodeNo);
						pos2=listPhsInfo.GetHeadPosition();
						PhsInfo* pInfo=NULL;
						while(pos2)
						{
							pInfo=&listPhsInfo.GetNext(pos2);
							if(pInfo->iCalNodeNumber==iTrueNodeNo)
								break;
							else pInfo=NULL;
						}
						
                        //strFilter.Format(_T("[jsdbh1]=%d"),iTrueNodeNo);
						strFilter.Format(_T("[jsdbh1]=%d AND [FileNameID]=%ld"),iTrueNodeNo,FileNameID);
						rsResult->Filter=_variant_t(strFilter);
                        
						if(rsResult->BOF || rsResult->adoEOF)
						{

							rsResult->AddNew();
							rsResult->PutCollect(L"FileNameID",_variant_t(FileNameID));
							rsResult->PutCollect(L"VolumeID",_variant_t(EDIBgbl::SelVlmID));
							rsResult->PutCollect(L"Zdjh",_variant_t((long)(iZdjh)));
							rsResult->PutCollect(L"jsdbh1",_variant_t((long)iTrueNodeNo));
							iZdjh++;
						}
						rsResult->PutCollect(L"PSAVer",_variant_t(strPSAVER));
						rsResult->PutCollect(L"UPxyz",_variant_t((long)giUPxyz));
						rsResult->PutCollect(L"DJ1",fDw);
						rsResult->PutCollect(L"T01",fTj);
						if(pInfo)
						{
							if(pInfo->lZDJType<1000)
							{
								switch(iPhsType)
								{
								case 0:
									pInfo->lZDJType=177;
									break;
								case 1:
									pInfo->lZDJType=130;
									break;
								case 2:
									if(fabs(arrVector[iVectorIndex].x-0.0f)>0.01f)
									{
										pInfo->lZDJType=166;
									}
									else if(fabs(arrVector[iVectorIndex].y-0.0f)>0.01f)
									{
										pInfo->lZDJType=155;
									}
									else if(fabs(arrVector[iVectorIndex].z-0.0f)>0.01f)
									{
										pInfo->lZDJType=144;
									}
									break;
								case 3:
								case 7:
									pInfo->lZDJType=130;
									break;
								case -1:
									if(fabs(arrVector[iVectorIndex].x-0.0f)>0.01f)
									{
										pInfo->lZDJType=110;
									}
									else if(fabs(arrVector[iVectorIndex].y-0.0f)>0.01f)
									{
										pInfo->lZDJType=120;
									}
									break;
								case -2:
									if(fabs(arrVector[iVectorIndex].x-0.0f)>0.01f)
									{
										pInfo->lZDJType=120;
									}
									else if(fabs(arrVector[iVectorIndex].y-0.0f)>0.01f)
									{
										pInfo->lZDJType=110;
									}
									break;
								case -3:
									if(fabs(arrVector[iVectorIndex].x-0.0f)>0.01f)
									{
										pInfo->lZDJType=112;
									}
									else if(fabs(arrVector[iVectorIndex].y-0.0f)>0.01f)
									{
										pInfo->lZDJType=121;
									}
									break;
								case -4:
									if(fabs(arrVector[iVectorIndex].x-0.0f)>0.01f)
									{
										pInfo->lZDJType=123;
									}
									else if(fabs(arrVector[iVectorIndex].y-0.0f)>0.01f)
									{
										pInfo->lZDJType=113;
									}
									break;
								case -5:
									pInfo->lZDJType=110;
									break;
								case -6:
									pInfo->lZDJType=120;
									break;
								case -7:
									pInfo->lZDJType=152;
									break;
								case -8:
									pInfo->lZDJType=161;
									break;
								}
							}
							rsResult->PutCollect(L"PJG1",pInfo->fPJG);
							rsResult->PutCollect(L"PGZ1",pInfo->fPGZ);
							rsResult->PutCollect(L"TYPE",(LPCTSTR)ltos(pInfo->lZDJType));
							rsResult->PutCollect(L"psaTYPE",(LPCTSTR)ltos(pInfo->lZDJType));
							rsResult->PutCollect(L"XL1",pInfo->fCoolLX);
							rsResult->PutCollect(L"YL1",pInfo->fCoolLY);
							rsResult->PutCollect(L"ZL1",pInfo->fCoolLZ);
							rsResult->PutCollect(L"XR1",pInfo->fLX);
							rsResult->PutCollect(L"YR1",pInfo->fLY);
							rsResult->PutCollect(L"ZR1",pInfo->fLZ);
							rsResult->PutCollect(L"MXL1",pInfo->fCoolMX);
							rsResult->PutCollect(L"MYL1",pInfo->fCoolMY);
							rsResult->PutCollect(L"MZL1",pInfo->fCoolMZ);
							rsResult->PutCollect(L"MXR1",pInfo->fMX);
							rsResult->PutCollect(L"MYR1",pInfo->fMY);
							rsResult->PutCollect(L"MZR1",pInfo->fMZ);
							rsResult->PutCollect(L"PXL1",pInfo->fCoolPX);
							rsResult->PutCollect(L"PYL1",pInfo->fCoolPY);
							rsResult->PutCollect(L"PZL1",pInfo->fCoolPZ);
							rsResult->PutCollect(L"PXR1",pInfo->fPX);
							rsResult->PutCollect(L"PYR1",pInfo->fPY);
							rsResult->PutCollect(L"PZR1",pInfo->fPZ);
							rsResult->PutCollect(L"RXL1",pInfo->fCoolQX);
							rsResult->PutCollect(L"RYL1",pInfo->fCoolQY);
							rsResult->PutCollect(L"RZL1",pInfo->fCoolQZ);
							rsResult->PutCollect(L"RXR1",pInfo->fQX);
							rsResult->PutCollect(L"RYR1",pInfo->fQY);
							rsResult->PutCollect(L"RZR1",pInfo->fQZ);
							rsResult->PutCollect(L"Paz1",pInfo->fPAZ);
							rsResult->PutCollect("Unit_Force", L"N");
							rsResult->PutCollect("Unit_Momental", L"N.m");
 
						}
						rsResult->Update();
						fStatusPos+=0.02f;
						frmStatus.UpdateStatus(fStatusPos ,true);
						frmStatus.UpdateWindow();
					}
					iVectorIndex++;
				}
				i+=(j-1);
				iNodeNo++;
			}
			iVectorIndex++;
		}
		pos=list_PhsInfo.GetHeadPosition();
		CList<long ,long &> listNoFoundNodeNo;
		while(pos)
		{
			tag_PhsInfo& infoTmp=list_PhsInfo.GetNext(pos);
			pos2=listTrueNodeNo.GetHeadPosition();
			BOOL bFound=FALSE;
			while(pos2)
			{
				int iNodeNo2=listTrueNodeNo.GetNext(pos2);
				if(iNodeNo2==infoTmp.iNode)
				{
					bFound=TRUE;
					break;
				}
			}
			if(!bFound)
			{
				listNoFoundNodeNo.AddTail(infoTmp.iNode);
			}
		}
		CString strNodeSet;
		pos=listNoFoundNodeNo.GetHeadPosition();
		i=0;
		while(pos)
		{
			int iNodeNo2=listNoFoundNodeNo.GetNext(pos);
			if(i==0)
			{
				strTemp.Format(_T(" �ڵ�� %d "),iNodeNo2);
				i++;
			}
			else
			{
				strTemp.Format(_T(", �ڵ�� %d "),iNodeNo2);
			}
			strNodeSet+=strTemp;
		}
		if(strNodeSet.GetLength()>0)
		{
			CString strMsg;
			strMsg.Format(_T("����������֧���ܽڵ����Ϣδ�ҵ� ��%s����"),strNodeSet);
			AfxMessageBox(strMsg);
		}
		frmStatus.UpdateStatus(1.0f ,true);
		frmStatus.UpdateWindow();
		rsResult->Filter=_variant_t(L"");
		rsResult->MoveFirst();
		_variant_t vtmp;
		while(!rsResult->adoEOF)
		{
			vtmp=rsResult->GetCollect("ZDJH");
			rsResult->PutCollect("ZDJH",_variant_t((long)(vtoi(vtmp)+10000)));
			rsResult->MoveNext();
		}
		_ConnectionPtr pCon;
		strSQL=(char*)(bstr_t)rsResult->GetSource();
		pCon=rsResult->GetActiveConnection();
		strSQL.Replace("ORDER BY JSDBH1","ORDER BY FileNameID"); 
		//strSQL.Replace("SElECT * FROM za");
		rsResult->Close();
		rsResult->Open(_variant_t(strSQL),(IDispatch*)pCon,adOpenKeyset,adLockOptimistic,adCmdText);
		//rsData->Sort=_bstr_t("JSDBH1");
		int itmpZdjh=1;
		while(!rsResult->adoEOF)
		{
			rsResult->PutCollect("ZDJH",_variant_t((long)itmpZdjh));
			rsResult->Update();
			itmpZdjh++;
			rsResult->MoveNext();
		}
	}
	       
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
	}
	frmStatus.ShowWindow(SW_HIDE);
}

long GetFileNameId(CString SourceDataFileName,IDispatch *pConID)
{
 		//����ļ�ID{
	    long FileNameID1;
		_ConnectionPtr ConDB;
		_RecordsetPtr FilenameIdRd,FilenameId1;
		ConDB.CreateInstance(__uuidof(Connection));
		FilenameIdRd.CreateInstance(__uuidof(Recordset));
		FilenameId1.CreateInstance(__uuidof(Recordset));
		CString FSql,FSql1,ExSql;
		SourceDataFileName.MakeUpper();
        //FilenameId1->Close();
		FSql="SELECT * FROM ZY ORDER BY FileNameID";
		FSql1="SELECT * FROM ZY WHERE FileNameID>=1";
		FilenameId1->Open(_variant_t(FSql1),pConID,adOpenDynamic,adLockOptimistic,adCmdText);	
		ConDB=FilenameId1->GetActiveConnection();
		if(FilenameId1->adoEOF)
		{
			FilenameId1->Close();
			FileNameID1=1;
		}
		else
		{
			FilenameId1->Close();
			FilenameIdRd->Open(_variant_t(FSql),pConID,adOpenDynamic,adLockOptimistic,adCmdText);
		    FilenameIdRd->MoveLast();
			FileNameID1=long(FilenameIdRd->GetCollect("FileNameID"))+1;
		    FilenameIdRd->Close();
		}			
		
		FSql.Format("SELECT * FROM ZY WHERE FileName=\"%s\" AND VolumeID=%ld",SourceDataFileName,EDIBgbl::SelVlmID);
		FilenameIdRd->Open(_variant_t(FSql),pConID,adOpenDynamic,adLockOptimistic,adCmdText);
		if(FilenameIdRd->adoEOF)
		{
			
			ExSql.Format("INSERT INTO [ZY] (FileName,VolumeID,FileNameID) VALUES(\"%s\",%d,%ld)",SourceDataFileName,EDIBgbl::SelVlmID,FileNameID1);
            
			ConDB->Execute(_bstr_t(ExSql),NULL,adCmdText);
            
			//AfxMessageBox("cc");
			//FilenameIdRd->AddNew();
			//FilenameIdRd->PutCollect("FileName",_variant_t(SourceDataFileName));
			//FilenameIdRd->PutCollect("VolumeID",_variant_t(EDIBgbl::SelVlmID));
			//FilenameIdRd->PutCollect("FileNameID",_variant_t(FileNameID1));
		    //FilenameIdRd->Update();
		}
		else	
			FileNameID1=long(FilenameIdRd->GetCollect("FileNameID"));
    FilenameIdRd->Close();
	//AfxMessageBox("bb");
	return FileNameID1;
}

void Delete1()
{   
	_ConnectionPtr ConDB;
	ConDB.CreateInstance(__uuidof(Connection));
	ConDB=::conPRJDB;
	CString strSQL;
	try
	{
            
			strSQL="DELETE * FROM ZY WHERE ZY.FileNameID NOT IN(SELECT DISTINCT FileNameID FROM ZA WHERE NOT ISNULL(ZA.FileNameID))";
            
			ConDB->Execute(_bstr_t(strSQL),NULL,adCmdText);

			//AfxMessageBox("aa");
	}
	catch(CException *e)
	{
		e->ReportError();
		e->Delete();
	}
}












void UpDataZY()
{
	_ConnectionPtr pCon;
	pCon.CreateInstance(__uuidof(Connection));
	pCon=::conPRJDB;
	_RecordsetPtr RD;
	RD.CreateInstance(__uuidof(Recordset));
	CString strUPdata;
	CString strZASQL = "SELECT DISTINCT [ZA].[FileNameID] AS [KEY],[ZY].[VolumeID],[ZA].[VolumeID] FROM ZA,ZY WHERE [ZY].[FileNameID] = [ZA].[FileNameID]";
	try
	{
		RD->Open(_variant_t(strZASQL),(IDispatch *)pCon,adOpenDynamic,adLockOptimistic,adCmdText);
		if(!RD->adoEOF && !RD->BOF)
		{
			RD->MoveFirst();
			CString strZAVolumeID;
			CString strZYVolumeID;
			CString strFileNameID;
			while(!RD->adoEOF)
			{
				//ZA��ZY������ͬ��FileNameID��Ӧ��ͬ��VolumeIDʱ��ZY���VolumeID
				strZAVolumeID = vtos(RD->GetCollect("ZA.VolumeID"));
				strZYVolumeID = vtos(RD->GetCollect("ZY.VolumeID"));
				strFileNameID = vtos(RD->GetCollect("KEY"));
				if(strZYVolumeID.CompareNoCase(strZAVolumeID))//����ͬ
				{
					strUPdata.Format("UPDATE ZY SET [VolumeID] = %s WHERE [FileNameID] = %s ",strZAVolumeID,strFileNameID);
					pCon->Execute(_bstr_t(strUPdata),NULL,adCmdText);
				}	
				RD->MoveNext();
			}
		}
	}
	catch(_com_error & e)
	{
		AfxMessageBox(e.Description());
		RD->Close();
	}
	RD->Close();
}

void ReadPSAData(_Recordset* rsResult ,CString SourceDataFileName,long maxZdjh,long FileNameID)
{
	CString strShareDbPath; //���ݿ�·��
	CString strSql;
	strShareDbPath = GetShareDbPath();
	_ConnectionPtr pRefInfoCon;
	try
	{
		pRefInfoCon.CreateInstance(__uuidof(Connection));
		pRefInfoCon->Open(_bstr_t(::dbConnectionString4+strShareDbPath+_T("ShareRefInfo.mdb")),
			_T(""),_T(""),0);
		WritePHSDataToTable(pRefInfoCon,rsResult,SourceDataFileName,maxZdjh,FileNameID,EDIBgbl::SelVlmID,modPHScal::gsngRatioOfPjg2Pgz);

	}
	catch(CException *e)
	{
		e->ReportError();
	    e->Delete();
	}
	if ( pRefInfoCon->GetState() == adStateOpen)
		pRefInfoCon->Close();

}