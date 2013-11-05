// DrawZDJTableID.cpp: implementation of the CDrawZDJTableID class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawZDJTableID.h"
#include "edibacad.h"
#include "edibgbl.h"
#include "edibdb.h"
#include "Frmtxsr.h"
#include "modphscal.h"
#include "user.h"
#include "cphs.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawZDJTableID::CDrawZDJTableID()
{

}

CDrawZDJTableID::~CDrawZDJTableID()
{

}
//���Ʊ��
void CDrawZDJTableID::DrawzdjTable(int index,bool bIsCom)
{
	try
	{
		bool Found=false;
		CCadPoint p0, p1;
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));

		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));

		int i=0;
		int k=0;
		static int n=0;
		if(index!=iZDJDRAWEXECL && index!=iZDJDRAWCatalogueExcel)
		{
			EDIBAcad::StartAcad();
			EDIBAcad::DisplayAcadTop();
			EDIBAcad::SetAcadTop();
			EDIBAcad::GetActiveAcadDoc();
		}
		//�����ǰ�򿪵�ͼ�β���(PhsBlkDir & _T("phs.dwg")),��Ҫ���´�
		//��Ϊ����Ĳ�����Ҫ������ͼ�ο�׼����
		CString sTmp,sTmp1;
		_variant_t vTmp;
		CMObject objTmp;
		CCadPoint pTmp;
		CString SQLx;
		COleVariant varTmp;
		_variant_t varNull;
		varNull.ChangeType(VT_NULL);
		if(index!=iZDJDRAWTag && index!=iZDJDRAWbom)
		{
			//�ж��Ƿ�����֧���ܼ�����ɡ�
			try
			{
				if(!FrmTxsr.m_pViewTxsr->m_ActiveRs->adoEOF && !FrmTxsr.m_pViewTxsr->m_ActiveRs->BOF)
					FrmTxsr.m_pViewTxsr->m_ActiveRs->Update();
			}
			catch(CException *e)
			{
				e->Delete();
			}
			CString SQLx = _T("SELECT count(*) FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE ( bCalSuccess=0  OR bCalSuccess IS NULL )");
			
			if(modPHScal::gbSumAllVolume )
			{
				SQLx +=_T(" AND VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") ");
			}
			else
			{
				SQLx +=_T(" AND VolumeID =") + ltos(EDIBgbl::SelVlmID);
			}

			long i1,i2;

			rs1->Open(_variant_t(SQLx),(IDispatch*)::conPRJDB,adOpenKeyset, adLockOptimistic,adCmdText);
			i1=vtoi(rs1->GetCollect(_variant_t((long)0)));
			rs1->Close();

			SQLx = _T("SELECT count(*) FROM [") + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE ");
			if(modPHScal::gbSumAllVolume )
			{
				SQLx +=_T(" VolumeID IN ( SELECT VolumeID FROM Volume WHERE EnginID=\'") + EDIBgbl::SelPrjID + _T("\' AND ZYID = ") + ltos(EDIBgbl::SelSpecID) + _T(" AND SJJDID = ") + ltos(EDIBgbl::SelDsgnID) + _T(" AND SJHYID = ") + ltos(EDIBgbl::SelHyID) +_T(") ");
			}
			else
			{
				SQLx +=_T(" VolumeID =") + ltos(EDIBgbl::SelVlmID);
			}
			rs1->Open(_variant_t(SQLx),(IDispatch*)::conPRJDB,adOpenKeyset, adLockOptimistic,adCmdText);
			i2=vtoi(rs1->GetCollect(_variant_t((long)0)));
			rs1->Close();
			rs1=NULL;
			CString strTmp;
			strTmp.Format(GetResStr(IDS_SomePhsNotCalSuccess),ltos(i2),ltos(i2-i1),ltos(i1));
			if( IDNO == ShowMessage(strTmp,MB_DEFBUTTON1|MB_ICONQUESTION|MB_YESNO) )
				return;
		}

		switch(index)
		{
		case iZDJDRAWTag:
			break;
		case iZDJDRAWbom:
			modPHScal::SetBasePoint();
			Cavphs->GetphsSEQ(FrmTxsr.m_pViewTxsr->m_ActiveRs);
			EDIBDB::MakeTmp2ToBom();
			p0.SetPoint(modPHScal::pt2x,modPHScal::pt2y);
			EDIBAcad::DeleteAllEntitiesInLayers(1,_T("bom"));
// 			rs.m_pDatabase=&EDIBgbl::dbPRJ;
// 			rs.Open(dbOpenSnapshot,_T("SELECT * FROM TMP2"));
			rs->Open((_bstr_t)_T("SELECT * FROM TMP2"), _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 

			EDIBAcad::DrawTableACAD(p0, EDIBgbl::TLJ, atan(1.0) * 0.0, rs,0,1.0f,_T("Bom"),_T("Bom"),_T("%g"),modPHScal::iAlignLjmx);
			sTmp.Format(GetResStr(IDS_xDrawingFinished),EDIBgbl::Cbtype[EDIBgbl::TCL].MnuCaption);

			ShowMessage(sTmp);
			break;
		case iZDJDRAWbomBJBW:
			modPHScal::SetBasePoint();
			Cavphs->GetphsSEQ(FrmTxsr.m_pViewTxsr->m_ActiveRs);
			EDIBDB::MakeTmp2ToBom();
			p0.SetPoint(modPHScal::pt2x,modPHScal::pt2y);
			EDIBAcad::DeleteAllEntitiesInLayers(1,_T("bom"));
// 			rs.m_pDatabase=&EDIBgbl::dbPRJ;
// 			rs.Open(dbOpenSnapshot,_T("SELECT * FROM TMP2"));
			rs->Open((_bstr_t)_T("SELECT * FROM TMP2"), _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			
			EDIBAcad::DrawTableACAD(p0, EDIBgbl::TLJ_BJBW, atan(1.0) * 0.0, rs,0,1.0f,_T("Bom"),_T("Bom"),_T("%g"),modPHScal::iAlignLjmx);
			sTmp.Format(GetResStr(IDS_xDrawingFinished),EDIBgbl::Cbtype[EDIBgbl::TCL].MnuCaption);

			ShowMessage(sTmp);
			break;
		case iZDJDRAWload:
			modPHScal::SetBasePoint();
			modPHScal::PhsDisplacementLoadINFOMake();
			break;
		case iZDJDRAW270mm:
				//ÿ�μ���һ��֧����ʱ���ɵ�ǰ��ǰ֧����һ����������ɺ��ܵ�һ����������ˡ�
				//ÿ�μ���һ��֧����ʱ���ɵ�ǰ��ǰ֧������ϸһ����������ɺ��ܵ���ϸһ����������ˡ�
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
					ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
					if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
					{
						rs->Close();
					}else
					{
						_variant_t var;
						var=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
						objTmp = var;
						p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
						p0 = p1;
					}
					EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZD, atan(1.0) * 0.0, rs, 0,1.0f, _T("YLB"));
			}
			break;
		case iZDJDRAW180mm:
				//ÿ�μ���һ��֧����ʱ���ɵ�ǰ֧����һ����������ɺ��ܵ�һ����������ˡ�
				//ÿ�μ���һ��֧����ʱ���ɵ�ǰ֧������ϸһ����������ɺ��ܵ���ϸһ����������ˡ�
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
				ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
				if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
				{
					rs->Close();
				}else
				{
					_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
					objTmp = v.pdispVal;
					p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
					p0 = p1;
				}
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZD180, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;	
		case iZDJDRAW270L:
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE ZDJH IN ( SELECT ZDJH FROM [")
				+ EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) 
				+ _T(" AND TYPE LIKE \'????\' ) AND VolumeID =")+ ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
				ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
				if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
				{
					rs->Close();
				}else
				{
					_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
					objTmp = v.pdispVal;
					p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
					p0 = p1;
				}
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZD270L, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;
		case iZDJDRAWEXECL:
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE ZDJH IN ( SELECT ZDJH FROM [")
				+ EDIBgbl::Btype[EDIBgbl::TZA] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) 
				+ _T(" AND TYPE LIKE \'????\' ) AND VolumeID =")+ ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");			
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{
				EDIBAcad::DrawTableExcel( EDIBgbl::TZDExcel, rs);
			}
			sTmp.Format(GetResStr(IDS_xDrawingFinished),EDIBgbl::Cbtype[EDIBgbl::TZDExcel].MnuCaption);
			ShowMessage(sTmp);
			break;
		case iZDJDRAW705mm:
				//�ж��Ƿ�����֧���ܼ�����ɡ�
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
					ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
					if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
					{
						rs->Close();
					}else
					{

						_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
						objTmp =v.pdispVal;
						p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
						p0 = p1;
					}
					EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZD800, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;

		case iZDJDRAW705mmHSY:			////����Ժ֧������ϸһ����
				//�ж��Ƿ�����֧���ܼ�����ɡ�
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
					ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
					if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
					{
						rs->Close();
					}else
					{
						_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
						objTmp =v.pdispVal;
						p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
						p0 = p1;
					}
					EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZD800HSY, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;

		case iZDJDRAW705mmQGY:			////֧������ϸһ����
				//�ж��Ƿ�����֧���ܼ�����ɡ�
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
				ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
				if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
				{
					rs->Close();
				}else
				{
					_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
					objTmp =v.pdispVal;
					p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
					p0 = p1;
				}
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZD800QGY, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;

		case iZDJDRAW270pc:   //����ʯ�����Ժ֧���ܱ�
				//ÿ�μ���һ��֧����ʱ���ɵ�ǰ��ǰ֧����һ����������ɺ��ܵ�һ����������ˡ�
				//ÿ�μ���һ��֧����ʱ���ɵ�ǰ��ǰ֧������ϸһ����������ɺ��ܵ���ϸһ����������ˡ�
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
					ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
					if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
					{
						rs->Close();
					}else
					{
						_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
						objTmp = v.pdispVal;
						p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
						p0 = p1;
					}
					EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZP, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;	
		case iZDJDRAWCatalogue:
			EDIBDB::DrawID = EDIBAcad::GetDrawIDAndName(1, EDIBDB::DrawName);

				//����Ŀ¼
				//ÿ�μ���һ��֧����ʱ���ɵ�ǰ��ǰ֧����ͼֽĿ¼��Ϣ��������ɺ��ܵ�Ŀ¼�������ˡ�
			SQLx=_T("DELETE FROM [ML] WHERE VolumeID IS NULL AND ZDJH IS NULL");
			EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TML] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY SEQ");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenDynaset,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{
					//�����һ��ͼֽ�����>=1����ǰ����ӿյ�ͼֽĿ¼,�Ա��û��ֹ��޸�
				VARIANT vTmp;
				if(!rs->Find((_bstr_t)(_T("ZDJH <> NULL")), 0, adSearchForward, vTmp))
					return;
				rs->get_Collect((_variant_t)_T("SEQ"),varTmp);
				k = vtoi(varTmp);

				for( i = 1 ;i< k ;i++)
				{
					if(!rs->Find((_bstr_t)(_T("SEQ=") + ltos(i)), 0, adSearchForward, vTmp))
					{
						rs->AddNew();
						rs->put_Collect((_variant_t)_T("SEQ"),COleVariant((long)i));
						rs->put_Collect((_variant_t)_T("Pages"),COleVariant((long)1));
						rs->put_Collect((_variant_t)_T("DrawNo"),STR_VAR(_T("1")));
						rs->put_Collect((_variant_t)_T("DrawNa"),STR_VAR(_T("1")));
						rs->put_Collect((_variant_t)_T("Zdjh"),varNull);
						rs->put_Collect((_variant_t)_T("VolumeID"),varNull);
						rs->Update();
					}
				}
				rs->Close();
				//zsy 12/17      ��Ϊ��ARX��ͼ
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TML] + _T("] WHERE VolumeID IS NULL OR VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T("  ORDER BY SEQ");
//				rs.Open(dbOpenDynaset,SQLx);
				rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenKeyset, adLockOptimistic, adCmdText); 
				ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
				if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
				{
					rs->Close();
				}else
				{
					_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
					objTmp = v.pdispVal;
					p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
					p0 = p1;
				}
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TML, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("ML"));
			}
			break;
		case iZDJDRAWCatalogueExcel:
			EDIBDB::DrawID = EDIBAcad::GetDrawIDAndName(1, EDIBDB::DrawName);
				//����Ŀ¼
				//ÿ�μ���һ��֧����ʱ���ɵ�ǰ��ǰ֧����ͼֽĿ¼��Ϣ��������ɺ��ܵ�Ŀ¼�������ˡ�
			SQLx=_T("DELETE FROM [ML] WHERE VolumeID IS NULL AND ZDJH IS NULL");
			EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);

			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TML] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY SEQ");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenDynaset,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			VARIANT vTmp;
			if(!rs->BOF && !rs->adoEOF)
			{
					//�����һ��ͼֽ�����>=1����ǰ����ӿյ�ͼֽĿ¼,�Ա��û��ֹ��޸�
				if(!rs->Find((_bstr_t)(_T("ZDJH <> NULL")), 0, adSearchForward, vTmp))
					return;
				rs->get_Collect((_variant_t)_T("SEQ"),varTmp);
				k = vtoi(varTmp);
				VARIANT vTmp;
				for( i = 1 ;i< k ;i++)
				{
					if(!rs->Find((_bstr_t)(_T("SEQ=") + ltos(i)), 0, adSearchForward, vTmp))
					{
						rs->AddNew();
						rs->put_Collect((_variant_t)_T("SEQ"),COleVariant((long)i));
						rs->put_Collect((_variant_t)_T("Pages"),COleVariant((long)1));
						rs->put_Collect((_variant_t)_T("DrawNo"),STR_VAR(_T("1")));
						rs->put_Collect((_variant_t)_T("DrawNa"),STR_VAR(_T("1")));
						rs->put_Collect((_variant_t)_T("Zdjh"),varNull);
						rs->put_Collect((_variant_t)_T("VolumeID"),varNull);
						rs->Update();
					}
				}
				rs->Close();
				SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TML] + _T("] WHERE VolumeID IS NULL OR VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T("  ORDER BY SEQ");
//				rs.Open(dbOpenDynaset,SQLx);
				rs->Open((_bstr_t)SQLx,_variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
					adOpenKeyset, adLockOptimistic, adCmdText); 
				EDIBAcad::DrawTableExcel(EDIBgbl::TMLE, rs);
			}
			sTmp.Format(GetResStr(IDS_xDrawingFinished),EDIBgbl::Cbtype[EDIBgbl::TML].MnuCaption);
			ShowMessage(sTmp);
			break;
		case iZDJDRAWm_phs :  //��ʼ�ֹ��޸�
			EDIBAcad::MakeNewLayer(_T("M_phs0"), acRed);
			break;
		case iZDJDRAWXN180mm://�������ϵ���Ժ��֧����һ����
			//Z8��
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
				ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
				if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
				{
					rs->Close();
				}else
				{
					_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
					objTmp = v.pdispVal;
					p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
					p0 = p1;
				}
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZDSWED180, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;
		case iZDJDRAW705mmHBYZ: //����ұ��
			//z9
						SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
				ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
				if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
				{
					rs->Close();
				}else
				{
					_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
					objTmp = v;
					p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
					p0 = p1;
				}
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TZD800HBYJ, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;
		case iZDJDRAWloadBJBW://��������֧������λ�Ʊ�
			modPHScal::SetBasePoint();
			modPHScal::PhsDisplacementLoadINFOMake("hz",0,EDIBgbl::TzDLOADNEW);
			break;
		//pfg20050601//��������
		case iZDJDRAWBJHY01:
				//�ж��Ƿ�����֧���ܼ�����ɡ�
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
					ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
					if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
					{
						rs->Close();
					}else
					{

						_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
						objTmp =v.pdispVal;
						p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
						p0 = p1;
					}
					EDIBAcad::DrawTableACAD(p0, 36, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;

		case iZDJDRAWBJHY02:
				//�ж��Ƿ�����֧���ܼ�����ɡ�
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" ORDER BY zdjh");
// 			rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 			rs.Open(dbOpenSnapshot,SQLx);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(!rs->BOF && !rs->adoEOF)
			{//zsy 12/17      ��Ϊ��ARX��ͼ
					ShowMessage(GetResStr(IDS_PleasePickApointAsZDJYLBInsertPointINAutoCAD));
					if( EDIBAcad::gbACADR14 || !bIsCom ) //ʹ�ö�̬�⻭ͼ
					{
						rs->Close();
					}else
					{

						_variant_t v=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Utility"));
						objTmp = v.pdispVal;
						p1=objTmp.Invoke(_T("GetPoint"),2,(LPVARIANT)p0,&_variant_t(GetResStr(IDS_PleasePickApointAsBOMInsertPointINAutoCAD)));
						p0 = p1;
					}
					EDIBAcad::DrawTableACAD(p0, 37, atan(1.0) * 0.0, rs, 0 , 1.0f, _T("YLB"));
			}
			break;
		//pfg20050601
		}
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
}
