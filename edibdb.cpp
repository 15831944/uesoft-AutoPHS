// EDIBDB.cpp
//

#include "stdafx.h"
#include "EDIBDB.h"
#include "EDIBgbl.h"
#include "basDirectory.h"
#include "Mobject.h"
#include "Column.h"
#include "Columns.h"
#include "Adodc.h"
#include "_Recordset.h"
#include "Field.h"
#include "Fields.h"
#include "modRegistry.h"
#include "user.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int EDIBDB::Qt = (int) _T("'");
int EDIBDB::FldSep = (int) _T(",");
int EDIBDB::IDSep = (int) _T("-");
int EDIBDB::DensityOfSteel = (int) 0.00785;
int EDIBDB::pi = (int) 3.1416;
CString EDIBDB::SJHY = CString();
int EDIBDB::SJHYIndex = int();
int EDIBDB::TB0Len = int();
int EDIBDB::TB2Len = int();
int EDIBDB::TB0Hei = int();
int EDIBDB::TB2Hei = int();
LONG EDIBDB::A0H = (LONG) 0;
LONG EDIBDB::A0W = (LONG) 0;
CString EDIBDB::DrawIDMask = CString();
CString EDIBDB::DrawIDinitNo = CString();
CString EDIBDB::PrjName = CString();
CString EDIBDB::DrawName = CString();
CString DrawNameEnglish = CString();
CString EDIBDB::DrawID = CString();
CString EDIBDB::DsgnName = CString();
long EDIBDB::DwgScale =0;
int EDIBDB::drawinfoFN = (int) _T("DrawInfo.txt");
CString EDIBDB::SelFileName = CString();
_RecordsetPtr* EDIBDB::rs = (_RecordsetPtr*) 0;
_RecordsetPtr* EDIBDB::rs1 = (_RecordsetPtr*) 0;
CMObject	EDIBDB::ObjExcelApp;
CMObject	EDIBDB::ExcWorkBook;
void EDIBDB::StartEXCEL(CString  FileName)
{
	try
	{
		//if(ObjExcelApp.GetActiveObject(_T("Excel.application"))!=S_OK)
		if(FAILED(ObjExcelApp.GetActiveObject(_T("Excel.application"))))		
		{
			//if(ObjExcelApp.CreateObject(_T("Excel.application"))!=S_OK)
			if( FAILED(ObjExcelApp.CreateObject(_T("Excel.application"))) )
			{
				ShowMessage(GetResStr(IDS_NotStartExcel));
				return;
			}
		}
		ObjExcelApp.PutPropertyByName(_T("Visible"),&_variant_t(true));
		CMObject objTmp;
		ExcWorkBook=ObjExcelApp.GetPropertyByName(_T("Workbooks"));
		//objTmp.p->AddRef();
		if(FileName!=_T(""))
		{
			if(!FileExists(FileName))
			{
				ExcWorkBook.Invoke(_T("Add"),1,&_variant_t(FileName));
			}
			else
			{
				//PathString = ObjExcelApp.Path
				//PathString = PathString & _T("\Excel.exe") & _T(" ") & FileName
				//PathString = PathString & _T("\Excel.exe e:\phsBomF4461.xls")
				ExcWorkBook.Invoke(_T("Open"),1,&_variant_t( FileName));
			}
		}
		//�����ڴ˴��ͷ�һ�μ�������Ϊ����COM�淶�����ü���=0ʱ�Զ��ͷ�Ӧ�ó���
		//���ԳƵ�ʹ��AddRef��Release�⽫����Excel������ȫ�ͷš�
		//ObjExcelApp.Release();
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
		//ShowMessage(_T("ErrorExcel"));
	}
}

bool EDIBDB::bExcelStart()
{
	return true;
}

bool EDIBDB::DrawInfoIsNull(COleVariant Obj)
{
	return true;
}

void EDIBDB::DrawInfoMakeFile(CString PrjName, CString DrawName, CString DrawID)
{
}

void EDIBDB::DrawInfoGet()
{
		//Ŀ��:���ͼֽ��Ϣ
	try
	{
		/*long i, DsgnID;
		SJHY =::GetRegKey(_T("directory"), _T("SJHY"),CString(_T("����")));
  
		if(!EDIBgbl::dbPRJ.IsOpen())
			EDIBgbl::dbPRJ.Open(basDirectory::ProjectDir+_T("workprj.mdb"));
		CDaoRecordset rs(&EDIBgbl::dbPRJ);
		rs.Open(dbOpenSnapshot,_T("SELECT * FROM CurrentWork"));
		COleVariant vTmp;
		if(!rs.IsEOF() && !rs.IsBOF())
		{
			 rs.MoveFirst();
			 rs.GetFieldValue(_T("gcmc"),vTmp);
			 PrjName = vtos(vTmp);
			 rs.GetFieldValue(_T("sjjdmc"),vTmp);
			 DsgnName = vtos(vTmp);
			 rs.GetFieldValue(_T("jcmc"),vTmp);
			 EDIBgbl::SelVlmName = vtos(vTmp);
			 rs.GetFieldValue(_T("CompanyID"),vTmp);
			 EDIBgbl::CompanyID = vtos(vTmp);
			 rs.GetFieldValue(_T("sjhy"),vTmp);
			 SJHY=vtos(vTmp);
			 SJHY.TrimLeft();SJHY.TrimRight();
			 if(SJHY==_T(""))
				SJHY = _T("����");
		}*/
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
	}
}

void EDIBDB::MakeTmp2ToBom()
{
//Ramaouther:likan
//����:
//Ŀ��:����֧������װ�����ϸ��
//����:
//   ---����֧������װ�����ϸ��:
//   ---����ǰ,GetphsSEQofBOM�ӹ����Ѿ���֧������ϸ
//   ---д��dbPrj����ʱ��Tmp2��.������δ����ͳ������
//   ---��ʱ���ܽ������������,��������Ӧ��=���и�����������֮��
//����:
   CString msOutFields, sGROUPBY;
   CDaoRecordset rs , rsCL, rsTmp2, rsTmpCLgroup ;
//   long i, j;
   CString sWHERE, sFLD;
//   bool bPhsLJ ;
   
   //ȷ��Ҫ������ֶ�:����ֶ���Ϣ���Դ�TableINFO���t??����,���ҿ����û���
   sGROUPBY = _T("seq,CLgg,CLcl,CLmc,CLID,CLdw,CLdz,CLbz");
      
   if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TmpCLgroup")))
      //��TmpCLgroup���ڱ��������
      EDIBgbl::dbPRJ.Execute(_T("DROP TABLE TmpCLgroup"));
   
   //��tmp2����һ��������TmpCLgroup
   //GROUP BY�Ӿ���ֶα������SELECT�־��ȫ���ֶ�(���˾ۺϺ������ֶ�)
   EDIBgbl::dbPRJ.Execute(_T("SELECT ") + sGROUPBY + _T(",sum(CLnum) as CLnums,CLnums*CLdz as CLzz INTO TmpCLgroup IN \'") + EDIBgbl::dbPRJ.GetName() + _T("\' FROM tmp2 GROUP BY ") + sGROUPBY);
   EDIBgbl::dbPRJ.Execute(_T("DROP TABLE Tmp2"));
   //����tmpCLgroup��tmp2,�Ա㱣���������
   EDIBgbl::dbPRJ.Execute(_T("SELECT ") + sGROUPBY + _T(", CLnums as CLnum, CLzz INTO Tmp2 FROM tmpCLgroup ORDER BY seq"));

}

void EDIBDB::GetTBsize()
{
	if(!EDIBgbl::dbSORT.IsOpen())
		EDIBgbl::dbSORT.Open(basDirectory::ProjectDBDir+_T("sort.mdb"));
	CDaoRecordset rs(&EDIBgbl::dbDSize);//20071101 "dbSORT" ��Ϊ "dbDSize"
	rs.Open(dbOpenSnapshot,_T("SELECT * FROM DrawSize WHERE trim(sjhy)=\'")+EDIBgbl::SelHyName+_T("\'"));
	COleVariant vTmp;
	if(!rs.IsBOF() && !rs.IsBOF())
	{
		rs.GetFieldValue(_T("tb0len"),vTmp);
		TB0Len=vtoi(vTmp);
		rs.GetFieldValue(_T("tb2len"),vTmp);
		TB2Len=vtoi(vTmp);
		rs.GetFieldValue(_T("tb0hei"),vTmp);
		TB0Hei=vtoi(vTmp);
		rs.GetFieldValue(_T("tb2hei"),vTmp);
		TB2Hei=vtoi(vTmp);
		rs.GetFieldValue(_T("A0Height"),vTmp);
		A0H=vtoi(vTmp);
		rs.GetFieldValue(_T("A0Width"),vTmp);
		A0W=vtoi(vTmp);
		rs.GetFieldValue(_T("mask"),vTmp);
		DrawIDMask=vtos(vTmp);
		rs.GetFieldValue(_T("initno"),vTmp);
		DrawIDinitNo=vtos(vTmp);
		rs.GetFieldValue(_T("SJHYIndex"),vTmp);
		EDIBDB::SJHYIndex=vtoi(vTmp);
	}
   rs.Close();
}

void EDIBDB::BackupDb(CString mdb)
{
}

void EDIBDB::MakeTbn(CString  tbn)
{
   _variant_t tmpvar;
   if(EDIBgbl::tdfExists (EDIBgbl::dbPRJ, tbn))
	   EDIBgbl::dbPRJ.Execute((CString( _T("DELETE * FROM [")) + tbn + _T("]")));
   else
   {
      //�����IN�Ӿ��в��ܼ���WHERE�Ӿ�,�������.
	   EDIBgbl::dbPRJ.Execute((CString(_T("SELECT * INTO [") + tbn + _T("] FROM [F4511S-JCL] IN \'"))+
		   basDirectory::TemplateDir + _T("F4511s.mdb\'"))); 
      
	   EDIBgbl::dbPRJ.Execute((CString(_T("DELETE * FROM [")) + tbn + _T("]")));
   }
}

void EDIBDB::AnalysisFieldsMAT(CString B, COleVariant fld())
{
}

void EDIBDB::TxtMat2Mdb(CString FN)
{
}

void EDIBDB::MDB2Txt(COleVariant MyData)
{
}

void EDIBDB::TBNTMP2Txt(CString DestFN)
{
}

void EDIBDB::SumMaterial(CString  VlmID)
{
	try{
	_variant_t tmpvar;
	if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("TMP2")))
		EDIBgbl::dbPRJ.Execute((_T("DROP TABLE TMP2 ")));
	
	EDIBgbl::SQLx = _T("SELECT * INTO TMP2 IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::SelBillType] + _T("]");
   VlmID.TrimLeft();VlmID.TrimRight();
   if(VlmID!=_T(""))
	   EDIBgbl::SQLx+=CString(_T(" WHERE trim(VolumeID)=\'")) + VlmID+ _T("\'");
   EDIBgbl::dbPRJDB.Execute(EDIBgbl::SQLx);
   SumNumbers();

	}
	catch(_com_error & e)
	{
		ShowMessage(e.Description());
	}
}

void EDIBDB::SumNumbers()
{
	//Ŀ��:������˳��ͳ�Ʋ��ϻ������
//����:
//���:������ܺ�Ĳ��ϴ����dbprj��tmp2���С�
   //Dim rs2 As Recordset, rs1 As Recordset
	try{
	_variant_t tmpvar;
	CDaoRecordset rs1,rs2;
	if( EDIBgbl::SelBillType == EDIBgbl::TCL || EDIBgbl::SelBillType == EDIBgbl::TLJ_BJBW ||EDIBgbl::SelBillType == EDIBgbl::TLJ || EDIBgbl::SelBillType == EDIBgbl::TZC)
	{
		//����ǲ��ϻ��ܱ������ϸ��֧���ܲ��ϻ��ܱ�
      MakeTbn(_T("TMP1"));
	  EDIBgbl::dbPRJ.Execute((_T("ALTER TABLE TMP1 DROP COLUMN CLnum")));
	  EDIBgbl::dbPRJ.Execute((_T("ALTER TABLE TMP1 ADD COLUMN [CLnum1] SINGLE")));
      
      //����tmp2�Ĳ��ϵ�tmp1��
	  EDIBgbl::SQLx = CString(_T("INSERT INTO TMP1 SELECT CLID,CLmc,CLgg,CLcl,sum(CLnum) as CLnum1 FROM TMP2 GROUP BY CLgg,CLcl,CLmc,CLID"));
	  EDIBgbl::dbPRJ.Execute((EDIBgbl::SQLx));
	}
  // ElseIf SelBillType = TSC Then
      //������豸���ϱ�
  // End If
   //�������¸���λ�����ص�
	rs1.m_pDatabase=&EDIBgbl::dbPRJ;
	rs1.Open(dbOpenDynaset ,_T("SELECT * FROM TMP1"));
	rs2.m_pDatabase=&EDIBgbl::dbPRJ;
	rs2.Open(dbOpenSnapshot ,_T("SELECT * FROM TMP2"));
    _variant_t v1,v2;
	COleVariant CLgg,CLmc,CLcl,CLID,CLdz,CLdw,CLnum1;
   while(!rs1.IsEOF())
   {
	   rs1.GetFieldValue(_T("CLgg"),CLgg);
	   rs1.GetFieldValue(_T("CLmc"),CLmc);
	   rs1.GetFieldValue(_T("CLcl"),CLcl);
	   rs1.GetFieldValue(_T("CLID"),CLID);
	   EDIBgbl::SQLx=CString(_T("trim(CLgg)=\'")) +vtos(CLgg)+
		   _T("\' AND Trim(CLmc)=\'") +vtos(CLmc) + 
		   _T("\' AND Trim(CLcl)=\'") +vtos(CLcl) +
		   _T("\' AND Trim(CLID)=\'") + vtos(CLID) + _T("\'");
	   //EDIBgbl::SQLx = _T(" trim(CLgg)=\'") + Trim((char*)CLgg.bstrVal) + _T("// AND trim(CLmc)=//") + Trim(rs1(_T("CLmc"))) + _T("// AND trim(CLcl)=//") + Trim(rs1(_T("CLcl"))) + _T("// AND trim(CLID)=//") + Trim(rs1(_T("CLID"))) + _T("//")
	   
      if(rs2.FindFirst(EDIBgbl::SQLx))
	  {
		  rs2.GetFieldValue(_T("CLdz"),CLdz);
		  rs2.GetFieldValue(_T("CLdw"),CLdw);
		  rs2.GetFieldValue(_T("CLnum1"),CLnum1);
		  rs1.Edit();
		  rs1.SetFieldValue( _T("CLdz"),CLdz);
		  rs1.SetFieldValue( _T("CLdw"),CLdw);
		  if(CLdz.vt!=VT_NULL && CLnum1.vt!=VT_NULL)
			CLdz.fltVal*=CLnum1.fltVal;
		  else{
			  CLdz.ChangeType(VT_R4);
			  CLdz.fltVal=0;
		  }
		  rs1.SetFieldValue( _T("CLzz"),CLdz);
		rs1.Update();
			rs1.MoveNext();
      }
	 }
	  rs1.Close();
	  rs2.Close();
	EDIBgbl::SQLx = _T("DELETE * FROM TMP2");
	EDIBgbl::dbPRJ.Execute((EDIBgbl::SQLx));
	EDIBgbl::SQLx = _T("INSERT INTO TMP2 SELECT CLID,CLmc,CLgg,CLcl,CLdw,CLdz,CLnum1 as CLnum,CLzz,ifLJ FROM TMP1");
	EDIBgbl::dbPRJ.Execute((EDIBgbl::SQLx));
	}
	catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
}
}

void EDIBDB::UpdTotalWeightAndMaterial(COleVariant tbn)
{
}

void EDIBDB::Txt2Mdb(CString FN)
{
}

void EDIBDB::AnalysisEngID(CString B)
{
}

void EDIBDB::AnalysisFields(CString B, COleVariant fld())
{
}

void EDIBDB::SetColumnsProperty(CDataGrid& DBGrid1, int  BILL)
{
	   //Ramaouther:likan
   //����:
   //Ŀ��:����DBGrid�ؼ�Columns���������:�ɼ�/����/etc.
   //����:
   //����:
   //_RecordsetPtr rs;
	CDaoRecordset rs;
   EDIBgbl::SQLx.Format(_T("SELECT * FROM tableINFO WHERE ID= %d "),BILL);
   try
   {
	   rs.m_pDatabase=&EDIBgbl::dbTable;//20071015 "dbSORT"��Ϊ"dbTable"
	   rs.Open(dbOpenSnapshot,EDIBgbl::SQLx);
	   CString sTmp;
	   if(rs.IsBOF() && rs.IsEOF())
	   {
		   sTmp.Format(GetResStr(IDS_NoRecordInXtableInXmdb),EDIBgbl::dbSORT.GetName(),_T("tableINFO"),EDIBgbl::SQLx);
		   ShowMessage(GetResStr(IDS_NoRecordInXtableInXmdb)+sTmp);
		   return;
	   }
   COleVariant v;
   _variant_t tmpvar;
   CString dbName;
	   rs.GetFieldValue(_T("TableName"),v);
	   if(vtos(v)==_T(""))
		   return;
		EDIBgbl::SQLx = CString(_T("SELECT * FROM ") )+ vtos(v) + _T(" WHERE NOT ISNULL(seq) ORDER BY seq");
	   rs.Close();
		rs.Open(dbOpenSnapshot,EDIBgbl::SQLx);
	   if(rs.IsEOF() && rs.IsBOF())
	   {
		   dbName=EDIBgbl::dbSORT.GetName();
		   sTmp.Format(GetResStr(IDS_NoRecordInXtableInXmdb),dbName,_T("TableName"),EDIBgbl::SQLx);
		   sTmp=GetResStr(IDS_NoRecordInXtableInXmdb)+sTmp;
		   ShowMessage(sTmp);
		   return;
	   }
	  _variant_t ix;
	  ix.ChangeType(VT_I4);
	  int c=DBGrid1.GetColumns().GetCount();
	  for(int i=0;i < c;i++)
	  {
		  ix.intVal=i;
		  sTmp=DBGrid1.GetColumns().GetItem(ix).GetDataField();
		  sTmp.TrimLeft();
		  sTmp.TrimRight();
		  sTmp.MakeUpper();
		  if(rs.FindFirst(CString(_T("trim(FieldName)=\'"))+sTmp+_T("\'")))
		  {
			  rs.GetFieldValue(_T("[Visible]"),v);
			  if(v.vt!=VT_NULL)
			DBGrid1.GetColumns().GetItem(ix).SetVisible(v.boolVal);
		  rs.GetFieldValue(_T("[Locked]"),v);
		  if(v.vt!=VT_NULL)
			DBGrid1.GetColumns().GetItem(ix).SetLocked(v.boolVal);
		  rs.GetFieldValue(_T("[LocalCaption]"),v);
		  if(v.vt!=VT_NULL)
		  {
			DBGrid1.GetColumns().GetItem(ix).SetCaption(vtos(v));
		  }
		  rs.GetFieldValue(_T("[Width]"),v);
		  if(v.vt!=VT_NULL)
			DBGrid1.GetColumns().GetItem(ix).SetWidth((float)(v.iVal/20));
		  }
	  }
	  rs.Close();
   }
   catch(::CDaoException * e)
	{
		e->ReportError();
		e->Delete();
   }
}

CString EDIBDB::GetOutputFields(int /*ByVal*/ BILL)
{
	return _T("");
}


void EDIBDB::RefreshGrid(CDataGrid &grid, _RecordsetPtr rs)
{
	int FCount=grid.GetColumns().GetCount();
	_variant_t ix;
	if(rs==NULL)
		return;
	ix.ChangeType(VT_I4);
	int i;
	for( i=FCount-1;i>0;i--)
	{
		ix.intVal=i;
		grid.GetColumns().Remove(ix);
	}
	FCount=rs->Fields->Count;
	//FCount=adodc.GetRecordset().GetFields().GetCount();
	CString sTmp;
	for( i=0;i<FCount;i++)
	{
		ix.intVal=i;
		sTmp=(char*)rs->Fields->GetItem(ix)->GetName();
		//sTmp=adodc.GetRecordset().GetFields().GetItem(ix).GetName();
		if(i!=0)
			grid.GetColumns().Add(ix);
		grid.GetColumns().GetItem(ix).SetDataField(sTmp);
		grid.GetColumns().GetItem(ix).SetCaption(sTmp);
	}
	LONG lRowCount;
	rs->get_RecordCount(&lRowCount);
	//Modified by Shuli Luo
	//Modify Date:2003-12-26
	if(lRowCount <= 0) 
	{
		return;
	}
	//Modify Ended;
	grid.SetRefDataSource(rs->GetDataSource());
	//grid.SetRefDataSource(adodc.GetRecordset().GetDataSource());
	grid.Refresh();
}

_variant_t EDIBDB::GridCol(CDataGrid &grid, LPCSTR ColName)
{
	_variant_t ret;
	ret.ChangeType(VT_I4);
	int FC=grid.GetColumns().GetCount();
	CString ss;
	for(int i=0;i<FC;i++)
	{
		ret.intVal=i;
		ss=grid.GetColumns().GetItem(ret).GetDataField();
		if(ss==ColName)
			break;
	}
	return ret;
}

void EDIBDB::CloseExcel()
{
	CMObject objTmp;
	try
	{
		if(ObjExcelApp.p!=NULL)//GetActiveObject(_T("Excel.Application")) == S_OK)
		//if(FAILED(ObjExcelApp.GetActiveObject(_T("Excel.Application"))))
		{
			//tmp.m_lpDispatch=ObjExcelApp.p;
			//tmp.InvokeHelper(0x12e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
			
			//objTmp=ObjExcelApp.GetPropertyByName(_T("Workbooks"));
			//objTmp.Invoke0(_T("Close"));
			//ObjExcelApp.PutPropertyByName(_T("DisplayAlerts"),&_variant_t((long)0));
			if(ExcWorkBook.p!=NULL)
			{
				ExcWorkBook.Invoke0(_T("Close"));
				ExcWorkBook.Release();
			}
			ObjExcelApp.Invoke0(_T("Quit"));
			ObjExcelApp.Release();
			//ObjExcelApp.Release();
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(...)
	{
	}
}

bool EDIBDB::OutPutTable(CString OutputTableName, CString DestFileName, CString  sCnnType, CDaoDatabase&  db, CString sSrcTableName, CString  Table_Format)
{
	try
	{
		CString strOrderBy;
		CDaoDatabase db1;
		CString SQLx;
		if( EDIBgbl::tdfExists(db, sSrcTableName) )
		{
			//����ڲ�ִ��
			/*
			if( EDIBgbl::tdfExists(db, _T("tmpTable")) )
			{
				db.Execute( _T("DROP TABLE tmpTable"));
			}
			//��tmp2����tmpPhsBOM�ṹ
			SQLx = _T("SELECT * INTO tmpPhsBOM FROM [") + sSrcTableName + _T("] ")+ strOrderBy;//WHERE trim(CLmc)=\_T('\') AND trim(CLgg)=\_T('\')";
			db.Execute( SQLx);
			*/
			if(Table_Format == _T("ACAD") )
			{
				Table_Format=_T(";");
			}
			else
			{
			}
			db1.Open(DestFileName,FALSE,FALSE,(Table_Format == _T("ACAD") ? _T(";") : Table_Format));
			//Set db1 = OpenDatabase(DestFileName, dbDriverNoPrompt, False, IIf(Table_Format = _T("ACAD"), _T(";"), Table_Format))
			if( EDIBgbl::tdfExists(db1, OutputTableName) )
			{
				db1.Execute( _T("DROP TABLE ") + OutputTableName);
			}
			//db1.Close();
			SQLx = _T("SELECT * INTO [") + OutputTableName + _T("] IN \'") + DestFileName + _T("\' ") + (sCnnType == _T("") ?  _T("") :  _T("\'") + sCnnType + _T("\'")) + _T(" FROM ") + sSrcTableName;
			db.Execute( SQLx);
			//SQLx = _T("SELECT * INTO [") + OutputTableName +_T("] FROM [")+sSrcTableName+_T("] IN \"\" [\; DATABASE=")+EDIBgbl::GetDBName(db)+_T("]");
			//db1.Execute( SQLx);
			return true;
		}
	}
	catch(CDaoException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}
	return true;
}
//
//���ܣ�����һ��Excel�ļ���������AutoIPED
void EDIBDB::CreateTableToAutoIPED(CString& strFileName, CString& strTblName, CString remark, double CLzz)
{
	CDaoDatabase db, dbExcel;
	CDaoRecordset pRs;
	CString strSQL,strFieldTbl,strTmp;
	try
	{
		//test
		if( strFileName.IsEmpty() )
		{	//���ҵ������ݵ�AutoIPEDʱ��ʹ�õ�һ�������("Excel2Access.mdb").
			//��õ����������ֶ�����Excel �ļ���
			strFileName = basDirectory::TemplateDir + "Excel2Access.mdb";
			db.Open(strFileName);
			pRs.m_pDatabase = &db;
			strSQL  = "SELECT * FROM TableExcelToAccess WHERE ID = 1";
			pRs.Open( dbOpenSnapshot, strSQL );
			if( pRs.IsBOF() && pRs.IsEOF() )
			{
			}
			else
			{
				strFieldTbl = vtos(GetFields(pRs, "TableName"));
				strTblName = vtos(GetFields(pRs, "ExcelTblName"));
			}
			pRs.Close();
		//	db.Close();
			strFileName = basDirectory::ProjectDir + strTblName + ".xls";
			if( FileExists(strFileName) )
			{
				DeleteFile( strFileName );
			}
		}
		//���ҵ�
		if( strFileName.Find(".xls",0) != -1 )
		{
			dbExcel.Open(strFileName, FALSE, FALSE, "Excel 5.0;");
			if( !EDIBgbl::tdfExists( dbExcel, strTblName) )
			{
				strSQL = "SELECT * FROM ["+strFieldTbl+"]";
				pRs.Open(dbOpenSnapshot, strSQL);
				//test create
				strSQL = "CREATE TABLE ["+strTblName+"] ";
				for(int i=0; !pRs.IsEOF(); pRs.MoveNext(), i++)
				{
					if( i == 0 )
					{
						strSQL += "(["+vtos(GetFields(pRs,"ChineseCaption"))+"] CHAR(50)";
					}
					else
					{
						strSQL += ", ["+vtos(GetFields(pRs,"ChineseCaption"))+"] CHAR(50)";
					}
				}
				strSQL += ")";
				//
				//û�б��򴴽���
			//	strSQL = "CREATE TABLE ["+strTblName+"] ([���] CHAR(50), [����] CHAR(50), [�ܵ�/�豸����] CHAR(50), [�ܵ��⾶] CHAR(50), [�豸������] CHAR(50), [�ܵ�����/�豸��] CHAR(50), [������ɫ] CHAR(50), [��������] CHAR(50), [��ע] CHAR(50))";
				dbExcel.Execute( strSQL );
			}
			CString strCLzz;
			strCLzz.Format("%lf", CLzz);
			//strSQL = "INSERT "+strTblName+" (����,�ܵ�/�豸����,�豸������, �ܵ�����/�豸��,������ɫ, ��������, ��ע)	VALUES('J"+EDIBgbl::SelJcdm+"',  '֧����', '"+strCLzz+"', '1', '֧����', '22', '"+remark+"')";
			strSQL = "INSERT INTO "+strTblName+"([����],[�ܵ�/�豸����],[�豸������],[�ܵ�����/�豸��],[������ɫ],[��������],[��ע])\
				VALUES('J"+EDIBgbl::SelJcdm+"','֧����','"+strCLzz+"','1','22','֧����','"+remark+"')";
			dbExcel.Execute( strSQL );
		}
	}
	catch(CDaoException* e)
	{
		e->ReportError();
		e->Delete();
	}

}
