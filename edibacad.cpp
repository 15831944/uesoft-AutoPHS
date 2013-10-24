// EDIBAcad.cpp
//

#include "stdafx.h"
#include "user.h"
#include "EDIBAcad.h"
#include "basDirectory.h"
//#include "_RecordsetPtr.h"
#include "user.h"
#include "modPHScal.h"
#include "EDIBgbl.h" 
#include "EDIBDB.h"
#include "FrmTxsr.h"
#include "CPhs.h"
#include "mainfrm.h"
#include "FrmDataEdit.h"
#include "Columns.h"
#include "Column.h"
#include <memory.h>
//#include "CUSTOM.h"
#include "modRegistry.h"
#include "Excel9.h"
#include <math.h>
#include "ModEncrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CString DrawNameEnglish;
BOOL EDIBAcad::g_bTagUsingCircle=TRUE;
BOOL EDIBAcad::g_bDrawNameWithoutNo=TRUE;
BOOL EDIBAcad::g_bLJMX_BJBW=FALSE;
BOOL EDIBAcad::g_bBGForBJBW=FALSE;
BOOL EDIBAcad::g_bDWForBJBW=FALSE;//������ʽ�Ķ�λ

CString EDIBAcad::g_strCustomPicNoFileName=_T("");
BOOL EDIBAcad::g_bCustomPicNo=FALSE;
int  EDIBAcad::g_bGroupWare=0;
CString EDIBAcad::g_strGroupWareName=_T("");
BOOL EDIBAcad::g_bDrawSumWeight=FALSE;
BOOL EDIBAcad::g_bNotDrawSingleWeight=FALSE;
BOOL EDIBAcad::g_bInsertLength =  TRUE;
BOOL EDIBAcad::g_bPrintSA_Value = TRUE;
BOOL EDIBAcad::g_bInstallElevation = TRUE;
BOOL EDIBAcad::g_bColdHeatState = TRUE;
BOOL EDIBAcad::g_bHzabs = FALSE;
BOOL EDIBAcad::g_bSumBomSprFactory=FALSE;
BOOL EDIBAcad::g_bBomSprFactory=FALSE;
BOOL EDIBAcad::g_bLocationDimension=FALSE;
BOOL EDIBAcad::g_bDLOADBJBW=FALSE;
BOOL EDIBAcad::g_bDimNumber = TRUE;

float EDIBAcad::g_fDistanceOffset = 0.0f;
float EDIBAcad::g_fOvertopDim = 2.0f;
float EDIBAcad::g_fSetTKWidth = 0.3;
float EDIBAcad::g_fSetTableWidth = 0.0;
float EDIBAcad::g_fSumWeight=0;

int EDIBAcad::sdiMode=0;
int EDIBAcad::iGIPmanual = (int) 0;
int EDIBAcad::iGIPAuto = (int) 1;
CCadPoint EDIBAcad::MinPoint;
CCadPoint EDIBAcad::MaxPoint;
double EDIBAcad::tmpBound[4];
double EDIBAcad::Bound[4];
CADatt EDIBAcad::AcAtt[16];
bool EDIBAcad::gbACADR14 = bool();
bool EDIBAcad::gbACADR2000 = bool();
COleSafeArray EDIBAcad::AcATTR ;//= COleVariant();
CString EDIBAcad::gsACADVER = CString();
CString EDIBAcad::sysVarName = CString();
CString EDIBAcad::sysVarData;
CMObject EDIBAcad::objAcadApp;
CMObject	EDIBAcad::objAcadDoc;
CMObject	EDIBAcad::UtilObj;
CMObject	EDIBAcad::MoSpace;
CMObject	EDIBAcad::PaSpace;
CMObject	EDIBAcad::AcEnt;
CMObject	EDIBAcad::AcObj;
CMObject	EDIBAcad::sset;
int EDIBAcad::osMode=0;
int EDIBAcad::iA0PrjNameWidth=0;
int EDIBAcad::iA2PrjNameWidth=0;
int EDIBAcad::iA0DrawNoWidth=0;
int EDIBAcad::iA2DrawNoWidth=0;
int EDIBAcad::iA0DrawNameWidth=0;
int EDIBAcad::iA2DrawNameWidth=0;
int EDIBAcad::iA0VlmNameWidth=0;
int EDIBAcad::iA2VlmNameWidth=0;
int EDIBAcad::iA0DateWidth=0;
int EDIBAcad::iA2DateWidth=0;

CString EDIBAcad::gstrApplicatonName="AutoCAD.Application";

HWND EDIBAcad::FindAcad()
{
	if(gbACADR14)
		return GetWindowHandle(_T("AutoCAD"));
	else
		return GetWindowHandle(_T("AutoCAD 2000"));

}

void EDIBAcad::SetAcadTop()
{
	HWND hwnd=FindAcad();
	if(hwnd!=NULL)
		::SetWindowPos(hwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

void EDIBAcad::SetAcadNoTop()
{
	HWND hwnd=FindAcad();
	if(hwnd!=NULL)
		::SetWindowPos(hwnd,HWND_BOTTOM,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

void EDIBAcad::DrawTKphs(long SizeIndex, long TkExtRatio, long DrawType, long TbhqFlag, long tbosFlag, long TKsubzone, long NoWaiKuang,long scal)
{
	MakeNewLayer(_T("TK"));
	SetACADCurrentStyle(_T("hz"));
	CCadPoint p0;

	//��ȡ��������
	EDIBgbl::GetSelPrjName();
	
	//ѡ�����Խ�����д

	AcAtt[0].Name = _T("PROJECT_NAME");
	AcAtt[0].Text = EDIBgbl::SelPrjName;
	AcAtt[0].iWidth=0;
	
	AcAtt[14].Name = _T("PROJECT_NAME_ENGLISH");
	AcAtt[14].Text = EDIBgbl::SelPrjEnglishName;
	AcAtt[14].iWidth=0;

	AcAtt[1].Name = _T("DRAWING_NAME");
	AcAtt[1].Text = EDIBDB::DrawName;
	AcAtt[1].iWidth=0;

	AcAtt[15].Name = _T("DRAWING_NAME_ENGLISH");
	AcAtt[15].Text = DrawNameEnglish;
	AcAtt[15].iWidth=0;
	
	

	AcAtt[2].Name = _T("STAGE");
	AcAtt[2].Text = EDIBgbl::SelDsgnName;
	AcAtt[2].iWidth=0;
	

	AcAtt[3].Name = _T("DRAWING_NO");
	if( EDIBAcad::g_bCustomPicNo )
	{
		AcAtt[3].Text = EDIBAcad::g_strCustomPicNoFileName;
	}
	else
	{
		AcAtt[3].Text = EDIBDB::DrawID;
	}
	
	AcAtt[4].Name = _T("SCALE");
	AcAtt[4].Text = _T("");
	AcAtt[4].iWidth=0;
	
	AcAtt[5].Name = _T("DATE");
	AcAtt[5].Text = CTime::GetCurrentTime().Format(_T("%d"));
	AcAtt[5].iWidth=0;
	
	AcAtt[6].Name = _T("JC_NAME");
	AcAtt[6].Text = EDIBgbl::SelVlmName;
	AcAtt[6].iWidth=0;	//������Щ��ͷ������дΪSPECILITY,SPECIALITY
	
	AcAtt[7].Name = _T("SPECIALITY");
	AcAtt[7].Text = EDIBgbl::SelSpecName;
	AcAtt[7].iWidth=0;
	
	AcAtt[8].Name = _T("SPECILITY");
	AcAtt[8].Text = EDIBgbl::SelSpecName;
	AcAtt[8].iWidth=0;
	
	AcAtt[9].Name = _T("DATETIME");

	if(EDIBAcad::g_bLJMX_BJBW)
	{
		AcAtt[9].Text = "";
	}
	else
	{
		AcAtt[9].Text = CTime::GetCurrentTime().Format(_T("%Y.%m.%d"));
	}
	AcAtt[9].iWidth=0;
	

	AcAtt[10].Name = _T("WEIGHT");//����
	CString strSumWeight;
	strSumWeight.Format("%0.1f",EDIBAcad::g_fSumWeight);
	AcAtt[10].Text = strSumWeight;
	AcAtt[10].iWidth=0;

	AcAtt[11].Name = _T("DRAWING_NO1");
	AcAtt[11].Text = AcAtt[3].Text;
	
	AcAtt[12].Name = _T("YEAR");
	AcAtt[12].Text = CTime::GetCurrentTime().Format(_T("%Y"));
	AcAtt[12].iWidth=0;
	
	AcAtt[13].Name = _T("MONTH");
	AcAtt[13].Text = CTime::GetCurrentTime().Format(_T("%m"));
	AcAtt[13].iWidth=2;
	
	if(SizeIndex<=1)
	{
		AcAtt[0].iWidth=iA0PrjNameWidth;
		AcAtt[1].iWidth=iA0DrawNameWidth;
		AcAtt[3].iWidth=iA0DrawNoWidth;
		AcAtt[5].iWidth=iA0DateWidth;
		AcAtt[6].iWidth=iA0VlmNameWidth;		
	}
	else
	{
		AcAtt[0].iWidth=iA2PrjNameWidth;
		AcAtt[1].iWidth=iA2DrawNameWidth;
		AcAtt[3].iWidth=iA2DrawNoWidth;
		AcAtt[5].iWidth=iA2DateWidth;
		AcAtt[6].iWidth=iA2VlmNameWidth;
	}
	drawTK(SizeIndex, TkExtRatio, DrawType, TbhqFlag, tbosFlag, TKsubzone, basDirectory::BlkDir, p0, AcAtt,16,NoWaiKuang);
}

void EDIBAcad::WriteToProjectDirphsTxt()
{
	try
	{
		CFile f;
		int i=0;
		if(!f.Open(basDirectory::ProjectDir + _T("phs.txt"),CFile::modeCreate|CFile::modeReadWrite))
			return;
		//��Ŀ��·��DestPath$ (format _T("X:\[dir\[...\[dir\]]]")) ��ʼ����һϵ���ļ�
		CString sFiles;
		WIN32_FIND_DATA FindData;
		memset(&FindData,0,sizeof(WIN32_FIND_DATA));
		//HANDLE FindHandle;
		//FindData.dwFileAttributes=FILE_ATTRIBUTE_NORMAL;
		user::AddDirSep(basDirectory::BlkDir);
		FindAllFileToTxt(basDirectory::BlkDir,&f);
		f.Close();
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void EDIBAcad::WriteToACADR14_lsp()
{
/*	try
	{
		CFile f;
		if(FileExists(AcadFirstSrchDir + _T("acadr14.bak")))
			CopyFile( AcadFirstSrchDir + _T("acadr14.bak"), AcadFirstSrchDir + _T("acadr14.lsp"),FALSE);
		else
			CopyFile( AcadFirstSrchDir + _T("acadr14.lsp"), AcadFirstSrchDir + _T("acadr14.bak"),TRUE);
		CString sTmp;
		if(f.Open(AcadFirstSrchDir + _T("acadr14.lsp"),CFile::modeNoTruncate|CFile::modeCreate|CFile::modeReadWrite))
		{
			sTmp=_T("(defun c:phs()\r\n");
			sTmp+=CString(_T("(setq fout (open \"")) + user::ChangePathSlash(basDirectory::ProjectDir + "phslsplog.txt") + "\" \"w\" ))\r\n";
			sTmp+=CString(_T("(setq f (open \"")) + user::ChangePathSlash(basDirectory::ProjectDir + "phs.txt") + "\" \"r\" ))\r\n";
			sTmp+=CString(_T("(while (setq blkName (read-line f))\r\n"));
			sTmp+=CString(_T("(if (/= (substr blkName 1 1) \" \") (progn (command \".insert\" blkName (list 0 0 0) 1 1 0 \" \" )) (write-line fout blkname) )\r\n"));
			sTmp+=CString(_T(")\r\n"));
			sTmp+=CString(_T("(Close fout)\r\n"));
			sTmp+=CString(_T("(Close f)\r\n"));
			sTmp+=CString(_T("(command \".Erase\" \"all\" \"\")\r\n"));
			sTmp+=CString(_T("(command \".saveas\" \"R14\" \"")) + user::ChangePathSlash(basDirectory::PhsBlkDir + "phs.dwg") + "\")\r\n";
			sTmp+=CString(_T("(command \".quit\" \"y\" \"\")\r\n"));
			sTmp+=CString(_T(")\r\n"));
		f.SeekToEnd();
		f.Write(sTmp,sTmp.GetLength());
		f->Close();
	}
	}
	catch(CException *e)
	{
		e->Delete();
	}
	catch(CException *e)
	{
	}
	*/
}

void EDIBAcad::AcadActivate()
{
}

CString EDIBAcad::GetAcadSrchDir(CString sACADVER)
{
   try
	{
		CString sKEY , sValue , sRet , ret;
		int i =0;
		sValue = _T("CurVer");//
		sKEY = _T("Software\\Autodesk\\AutoCAD\\")+sACADVER;
		DWORD kType;
		HKEY hKey,hKey2;
		DWORD len;
		if(RegOpenKey(HKEY_LOCAL_MACHINE,sKEY,&hKey)==ERROR_SUCCESS)
		{
			len=256;
			if(RegQueryValueEx(hKey,sValue,NULL,&kType,(unsigned char*)sRet.GetBuffer(256), &len)==ERROR_SUCCESS)
			{
				sRet.ReleaseBuffer();
				sKEY = sKEY + _T("\\") + sRet;
				len=256;
				if(::RegOpenKey(HKEY_LOCAL_MACHINE,sKEY,&hKey2)==ERROR_SUCCESS)
				{
					if(RegQueryValueEx(hKey2,_T("ACAD"),NULL,&kType,(unsigned char*)sRet.GetBuffer(256),&len)==ERROR_SUCCESS)
					{
						sRet.ReleaseBuffer();
						i=sRet.Find(_T(";"),0);
						if (i > 0 )
							ret= sRet.Left( i);
						else
							ret=  sRet;
         
						user::AddDirSep(ret);
					}
					::RegCloseKey(hKey2);
				}
			}
			::RegCloseKey(hKey);
		}
		CString sDir;
		CMObject PreferencesFiles,tmpObj;
		_variant_t v;
		if( gbACADR14)
		{
			v = objAcadApp.GetPropertyByName(_T("Preferences"));
			PreferencesFiles = v.pdispVal;
		}
		else
		{
			//  Set PreferencesFiles = objAcadApp.Preferences.Files
			v=objAcadApp.GetPropertyByName(_T("Preferences"));
			tmpObj = v.pdispVal;
			v = tmpObj.GetPropertyByName(_T("Files"));		
			PreferencesFiles = v.pdispVal;
		}



		sDir = vtos(PreferencesFiles.GetPropertyByName(_T("SupportPath")));
		i = sDir.Find ( _T(";"),0);
		if( i > 0)	
			ret = sDir.Left ( i );
		else
			ret = sDir;

		user::AddDirSep( ret);
		user::RemoveDirSep( basDirectory::PhsBlkDir);
		if (sDir.Find( basDirectory::PhsBlkDir,0) == -1 ) sDir +=  _T(";") + basDirectory::PhsBlkDir;
		user::RemoveDirSep (basDirectory::BlkDir);
		if (sDir.Find(basDirectory::BlkDir + _T(";")) == -1)
		{
			if (sDir.Right(basDirectory::BlkDir.GetLength()) != basDirectory::BlkDir)
				//blkDir=sdir�ַ���β��
			//else
				//blkDir�����ַ���β��
				sDir+=  (CString(_T(";")) + basDirectory::BlkDir);      
		}
		PreferencesFiles.PutPropertyByName(_T("SupportPath"),&_variant_t(sDir));
		user::AddDirSep (basDirectory::PhsBlkDir);
		user::AddDirSep (basDirectory::BlkDir);
		return ret;
	}
	catch(CException *e)
	{
		e->Delete();
	}
		return CString(_T(""));
}

BOOL EDIBAcad::StartAcad(CString /*Optional*/ DwgName)
{
   //ShowMessage( GetResStr(IDS_NotClickMouseInAnyWindowWhenACADstarting), MB_SYSTEMMODAL|MB_OK);
	CString tmpsz;
	_variant_t VarName;
	_variant_t tmpvar;
	CString strTmp,strTmp2;
	static int StartCount=0;
	AfxGetApp()->BeginWaitCursor();
	if(modPHScal::gIsACAD==0)//ѡ����AutoCAD��ͼ
		gstrApplicatonName="AutoCAD.Application";
	else//ѡ����MicroStation��ͼ
		gstrApplicatonName="MicroStationDGN.Application";
	try
	{
		if(FAILED(objAcadApp.GetActiveObject(gstrApplicatonName)))
		{
			StartCount=0;
			if(FAILED(objAcadApp.CreateObject(gstrApplicatonName)))
			{
				ShowMessage( GetResStr(IDS_NotStartAcad));
				return FALSE;
			}
			objAcadApp.PutPropertyByName(_T("Visible"), &_variant_t(true));
			if(modPHScal::gIsACAD==1)
				objAcadApp.Invoke(_T("CreateDesignFile"), 3, &_variant_t(_T("seed3d")), &_variant_t(_T("345435.dgn")), &_variant_t(true));
			return FALSE;
		}
//		return FALSE;
		tmpvar=objAcadApp.GetPropertyByName(_T("Path"));
		basDirectory::AcadwinDir=vtos(tmpvar);

		user::AddDirSep(basDirectory::AcadwinDir);
		user::AddDirSep(basDirectory::TemplateDir);

		if(modPHScal::gIsACAD==0)//ѡ����AutoCAD��ͼ
		{
			if(!::FileExists(basDirectory::AcadwinDir+_T("fonts\\hzfs.shx")))
			{
				if(!CopyFile(basDirectory::TemplateDir + _T("hzfs.shx"),basDirectory::AcadwinDir + _T("fonts\\hzfs.shx"),FALSE))
				{
					if(FileExists(basDirectory::AcadwinDir +_T("fonts\\hztxt.shx")))
						CopyFile(basDirectory::AcadwinDir +_T("fonts\\hztxt.shx"),basDirectory::AcadwinDir+_T("fonts\\hzfs.shx"),FALSE);
					else
						ShowMessage(GetResStr(IDS_AT) + basDirectory::AcadwinDir +GetResStr(IDS_CANNOTSHOW));

				}		
			}
			if(!::FileExists(basDirectory::AcadwinDir+_T("fonts\\hztxt.shx")))
			{
				if(CopyFile(basDirectory::TemplateDir + _T("hzfs.shx"),basDirectory::AcadwinDir + _T("fonts\\hztxt.shx"),FALSE))
				{
				}
			
			}
		}

		tmpvar=objAcadApp.GetPropertyByName(_T("Version"));
		gsACADVER=vtos(tmpvar);
		//MicroStation V8�İ汾�ַ���������: Version 08.00.00.21 Windows x86
		if(modPHScal::gIsACAD==0)//ѡ����AutoCAD��ͼ
		{
			if(gsACADVER.Left(2)==_T("14"))
			{
				gbACADR14=true;
				CMObject ObjPreferences;
				_variant_t v;
				v=objAcadApp.GetPropertyByName(_T("Preferences"));
				ObjPreferences = v.pdispVal;
				_variant_t varStr;
				varStr.SetString("<<Unnamed Profile>>");
				ObjPreferences.PutPropertyByName(_T("ActiveProfile"),&varStr);

				gbACADR2000 = false;
			}
			else 
			{
				gbACADR14 = false;
				if(gsACADVER.Left(2)==_T("15"))
				{
					gbACADR2000 = true;
				}
				else
				{
					gbACADR2000 = false;
				}
			}
		}
		else//ѡ����MicroStation
		{
		}

		if(DwgName!=_T("")) 
		{
			CString strFileName;
			strFileName=vtos(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("DWGPREFIX"))));
			user::AddDirSep(strFileName);
			strFileName+=vtos(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("DWGNAME"))));
			strFileName.MakeUpper();
			DwgName.MakeUpper();
			if(strFileName!=DwgName)
			{
				if(modPHScal::gIsACAD==0)//ѡ����AutoCAD��ͼ
				{
					objAcadDoc.Invoke(_T("Open"),1,&_variant_t(DwgName));
				}
				else
				{					
					objAcadDoc.Invoke(_T("OpenDesignFile"),1,&_variant_t(DwgName));
				}
			}
		}
		else
		{
			//û��ͼ���ļ��������½�ͼ��
			//objAcadDoc.Invoke(_T("New"),1,&_variant_t(DwgName));
		}

		_variant_t v;
		if(modPHScal::gIsACAD==0)//ѡ����AutoCAD��ͼ
		{
			v=objAcadDoc.GetPropertyByName(_T("ModelSpace"));
			MoSpace = v.pdispVal;
			PaSpace=objAcadDoc.GetPropertyByName(_T("PaperSpace")).pdispVal;
			//Set OSNAP mode for duration of the VB program
			sysVarName = _T("OSMODE");
			VarName=_variant_t(sysVarName);
			tmpvar=objAcadDoc.Invoke(_T("GetVariable"),1,&VarName);
			//sysVarData =(char*)_bstr_t(tmpvar);
			osMode = tmpvar.operator long();//VT_I2
			tmpvar.ChangeType(VT_I2);
			tmpvar.intVal=0;
			objAcadDoc.Invoke(_T("SetVariable"),2,&VarName,&tmpvar);
		   //���óߴ����
			
			EDIBAcad::SetACADCurrentStyle(_T("Dim"),modPHScal::strBigFontDim,modPHScal::strSHXFontDim,modPHScal::fFWDim);
			EDIBAcad::SetACADCurrentStyle(_T("BOM"),modPHScal::strBigFontLjmx,modPHScal::strSHXFontLjmx,modPHScal::fFWLjmx);
			EDIBAcad::SetACADCurrentStyle(_T("LOAD"),modPHScal::strBigFontLoad,modPHScal::strSHXFontLoad,modPHScal::fFWLoad);
			EDIBAcad::SetACADCurrentStyle(_T("hz"));
			objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("DIMTXSTY")),&_variant_t(_T("Dim")));
		   tmpvar.ChangeType(VT_R8);

		   //������˾�޸�Ϊ0 ��ǰΪ3
		   tmpvar.dblVal=EDIBAcad::g_fDistanceOffset;

		   VarName=_variant_t(_T("dimexo"));
		   objAcadDoc.Invoke(_T("SetVariable"),2,&VarName,&tmpvar);
		   //�����ߴ��ߵĲ���

		   //������˾�޸�Ϊ2 ��ǰΪ5


		   tmpvar.dblVal=EDIBAcad::g_fOvertopDim;

		   VarName=_variant_t(_T("dimexe"));
		   objAcadApp.Invoke(_T("SetVariable"),2,&VarName,&tmpvar);
		   tmpvar.ChangeType(VT_BOOL);
		   tmpvar.boolVal=TRUE;
		   objAcadApp.PutPropertyByName(_T("Visible"),&tmpvar);
			StartCount++;
			LoadMyArx("PHS.arx");
		//	LoadMyArx("sm.arx");
		}
		else //MicroStation
		{
			MoSpace=objAcadDoc.GetPropertyByName(_T("ActiveModelReference")).pdispVal;			
		}

	}
	catch(CException *e)
	{
		CString str;
		str.LoadString(IDS_AUTOCAD_INITIALIZE_ERROR);
		ShowMessage(str);
		return FALSE;
	}
	return TRUE;
}

void EDIBAcad::SaveDwgAndExitACAD(COleVariant prompt, COleVariant DwgName)
{
}

void EDIBAcad::CloseAcadWin(COleVariant /*Optional*/ ExitMode)
{
}

void EDIBAcad::ClearAcadDash(CString DwgName)
{
}

void EDIBAcad::MakeNewLayer(CString layerName, int LayerColor)
{
	try
	{
		CMObject layerObj,tmpObj;
		long i=0;
		bool bFound=false ;
		tmpObj=objAcadDoc.GetPropertyByName(_T("Layers")).pdispVal;
		int C=(long)tmpObj.GetPropertyByName(_T("Count"));
		CString sTmp;
		layerName.TrimLeft();layerName.TrimRight();
		layerName.MakeUpper();
		//============== �ж��Ƿ�Ҫ�����Ĳ��Ѿ����� =================
		for( i = 0 ;i<C;i++)
		{
			layerObj =tmpObj.Invoke(_T("Item"),1,&_variant_t(i)).pdispVal;
			sTmp=vtos(layerObj.GetPropertyByName(_T("Name")));
			sTmp.TrimLeft();sTmp.TrimRight();
			sTmp.MakeUpper();
			if(sTmp==layerName)
			{
				bFound=true;
				break;
			}
		}
		if( !bFound )
			layerObj = tmpObj.Invoke(_T("Add"),1,&_variant_t(layerName)).pdispVal;
		
		if(LayerColor!=-10)
			layerObj.PutPropertyByName(_T("Color"),&_variant_t((long)LayerColor));

		GetActiveAcadDoc();
		objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("clayer")),&_variant_t(layerName));
		MoSpace=objAcadDoc.GetPropertyByName(_T("ModelSpace")).pdispVal;
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
}

void EDIBAcad::SetDimVariables()
{
	GetActiveAcadDoc();
	objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("dimrnd")),&_variant_t((double)0.5));
	objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("dimtih")),&_variant_t(false));
	objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("dimtoh")),&_variant_t(false));
	objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("dimtad")),&_variant_t((long)1));   //;����ƽ���ڳߴ���
	objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("dimtix")),&_variant_t((short)1));  //;������д�ڳߴ����м�
	objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("dimtix")));
}

void EDIBAcad::ModifyAttBG(float /*ByVal*/ ELvalue, COleVariant /*Optional*/ BGtype, COleVariant /*Optional*/ strTagStringOfBG)
{
}

void EDIBAcad::ModifyAttributes(CADatt* BlkAtt,long acCount, CMObject& AcObject)
{
	try
	{
		IDispatch* acAtt;
		long i=0,C=0, k=0, n=0;
		CCadPoint p1, p ;
		CCadPoint minExt, MaxExt;
		CCadPoint p2, ps, pe;
		CMObject tmpObj;
		CString sTmp;
		if( (long)AcObject.GetPropertyByName(_T("EntityType")) == acBlockReference)
		{
			if( ((bool)AcObject.GetPropertyByName(_T("HasAttributes")))!=false)
			{
				AcATTR = COleSafeArray(AcObject.Invoke0(_T("GetAttributes")).parray,VT_VARIANT);
				p1 = AcObject.GetPropertyByName(_T("insertionPoint"));
				AcATTR.GetLBound(1,&i);
				AcATTR.GetUBound(1,&C);
				long ix=0;
				for(;i<=C;i++)
				{
					AcATTR.GetElement(&ix,&acAtt);
					tmpObj=acAtt; 
					sTmp=vtos(tmpObj.GetPropertyByName(_T("TagString")));
					sTmp.TrimLeft();sTmp.TrimRight();
					sTmp.MakeUpper(); 
					for(k=0;k<acCount;k++)
					{
						BlkAtt[k].Text.TrimLeft();BlkAtt[k].Text.TrimRight();
						BlkAtt[k].Name.TrimLeft();BlkAtt[k].Name.TrimRight();
						BlkAtt[k].Name.MakeUpper();
						if( (BlkAtt[k].Text != _T("")) && (BlkAtt[k].Name == sTmp))
						{
							tmpObj.PutPropertyByName(_T("TextString"),&_variant_t(BlkAtt[k].Text));
							tmpObj.PutPropertyByName(_T("Visible"),&_variant_t(true));
							p = tmpObj.GetPropertyByName(_T("insertionPoint"));
							//����AutoCAD R14��BUG��ActiveX���ɵĿ�����λ��������ԭ�㣬
							//�������ֹ��������������������
							//�����˹��޸����Բ����;
							//�������Բ���λ����������ԭ��Ϊ����ġ�
							//�ܶ����Զ������е�Ϊ��׼�ģ�������VB��õĶ��󶼱��������ˡ�����Ҫ��ȥ��ȵ�1/2
							//���ڼ����ϵ�ԭ�����ڶ�������ʱȫ��ʹ������뷽ʽ������������ȷ���޸�����λ�á�
							p2.SetPoint(p[0] + p1[0],p[1] + p1[1],p[2] + p1[2]); //- IIf(n = 0, (MaxExt(n) - minExt(n)) / 2, 0)
							if( gsACADVER==_T("14.0"))
							{
								tmpObj.PutPropertyByName(_T("insertionPoint"),(LPVARIANT)p2);
								
								
								//chengbailian 2004.3.31
								if(BlkAtt[k].Name == "DRAWING_NAME_ENGLISH")
									AcAtt[k].iWidth=DrawNameEnglish.GetLength()*3.8/2;

								if(BlkAtt[k].Name == "PROJECT_NAME" || BlkAtt[k].Name == "PROJECT_NAME_ENGLISH")
									tmpObj.PutPropertyByName(_T("StyleName"),&_variant_t("STYLE1"));
								if(BlkAtt[k].Name =="DATETIME")
									AcAtt[k].iWidth=BlkAtt[k].Text.GetLength()*3.24*0.7;


								
							}
							
							if( BlkAtt[k].iWidth >0 )
							{
							   p2 = tmpObj.GetPropertyByName(_T("insertionPoint"));
							   CCadPoint minExt,MaxExt,txtP;
							   GetBoundingBox(tmpObj,minExt, MaxExt);
							   if(fabs(MaxExt[0]-minExt[0]) > BlkAtt[k].iWidth)
							   {
								   txtP.SetPoint(p2[0] + BlkAtt[k].iWidth,p2[1],p2[2]);
								   tmpObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentFit));
								   tmpObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)txtP);
							   }
							}							

							tmpObj.Invoke0(_T("Update"));
					 }
				 }
				ix++;
			 }
		  }
		}
	}
	catch(COleException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(CException *e)
	{
		e->Delete();
	}
}
void EDIBAcad::DrawPLine(CCadPoint& StartPoint, float  Length, float  sngRotAng, float  PLwidth)
{
	 CVariantArray p(4);
   p.SetData(4,StartPoint[0],StartPoint[1],StartPoint[0] + Length * cos(sngRotAng),StartPoint[1] + Length * sin(sngRotAng));
   AcObj = MoSpace.Invoke(_T("AddLightWeightPolyline"),1,(LPVARIANT)p).pdispVal;
   AcObj.Invoke(_T("SetWidth"),3,&_variant_t((long)0), &_variant_t((double)PLwidth), &_variant_t((double)PLwidth));
}

void EDIBAcad::DrawPLine(CCadPoint& StartPoint, CCadPoint& EndPoint, float  PLwidth)
{
   CVariantArray p(4);
   p.SetData(4,StartPoint[0],StartPoint[1],EndPoint[0],EndPoint[1]);
   AcObj = MoSpace.Invoke(_T("AddLightWeightPolyline"),1,(LPVARIANT)p).pdispVal;
   AcObj.Invoke(_T("SetWidth"),3,&_variant_t((long)0), &_variant_t((double)PLwidth), &_variant_t((double)PLwidth));
}

void EDIBAcad::drawTK(long SizeIndex, long TkExtRatio, long DrawType, long TbhqFlag, long tbosFlag, long TKsubzone, CString TbDir, CCadPoint& p0, CADatt* BlkAtt,long acCount ,long NoWaiKuang,long scal)
{

		// This is Programmed by Goobin Lee,
		// Hunan Electric Power Design Institute, Changsha, China. 410007
		// Tel:0731-5527700-7480(H),0731 5527700-7992(Office)
		// EaseSoft,Ltd. Changsha, China. 410007
		// Tel:13707311224, Email:EaseSoft@126.com, http://EaseSoft.Webjump.com
		// Email:GBLee@public.cs.hn.cn
		//AutoLISP routine written since
		// 1997.08.28 initial vesion
		// 1997.08.29 revision
		// 1999.12.19 revision
		//VBA
		// 2000.01.07 revision
		// ͼ����Ƴ���
		// ����ͼ���С(0~4)���ӳ�ϵ��(1~8)��ͼ����ʽ(��ʽ0����ʽ1)���Ƿ��ǩ(��0����1)��ͼ��Ŀ¼TbDir
		// ���ɻ���ͼ��
		//SizeIndex----ͼ���С,0=A0,...,4=A4
		//ExtRatio-----�ӳ�ϵ��(1~8)/8��(1~4)/4
		//DrawType-----ͼ����ʽ(��ʽ0����ʽ1)
		//DistSep------ͼ��������(25<DistSep<75)
	try{
		CVariantArray p1(10);
		CCadPoint p2s, p2e, txtP;
		CVariantArray pi(10);
		long X=0, Y=0, tmp=0, xi=0, yi=0, i=0, k=0;
		CCadPoint InsPnt;
		float  DistSep =0, TxtHeight=0 , TKscal=0;
		float  NewDistSep=0, NumSep =0;
		float  PLwidth=0 ;
		float AxH=0 , AxW=0;
		SetAcadTop();
		MakeNewLayer(_T("TK"));
		SetDimVariables();
		EDIBDB::GetTBsize();
		if(scal==0) 
		  scal = 1;
		
		if(EDIBDB::A0H == 0) EDIBDB::A0H = 841;
		if( EDIBDB::A0W == 0) EDIBDB::A0W = 1189;
		AxW = EDIBDB::A0W;
		AxH = EDIBDB::A0H;
		long LeftMarginWidth=0, RightMarginWidth=0;
		LeftMarginWidth = 25;
		RightMarginWidth = 10;
		if(SizeIndex >= 3 )RightMarginWidth = 5;
		AxW = int(0.5 + EDIBDB::A0W / pow(sqrt(2.0) , SizeIndex));
		AxH = int(0.5 + EDIBDB::A0H / pow(sqrt(2.0) , SizeIndex));
		//;�ӳ�ͼֽ
		AxW = AxW * (1 + TkExtRatio * ((SizeIndex % 2) == 0 ? 0.125 : 0.25));
		//;A4ͼֻ������ʽͼ��,�ʽ������ȡ����;��A4ͼ��ʽ��A0~A3ͼ��ʽ��ͬ
		if((SizeIndex < 4 && DrawType == 1) || SizeIndex == 4)
		{
		  tmp = AxW; AxW = AxH;AxH = tmp;
		}
		if(DrawType == 0 || SizeIndex == 4 )
		{
		  xi = AxW - LeftMarginWidth - RightMarginWidth;
		  yi = AxH - 2 * RightMarginWidth;
		}
		else
		{
		  xi = AxW - 2 * RightMarginWidth;
		  yi = AxH - LeftMarginWidth - RightMarginWidth;
		}
		double p0x=p0[0];
		double p0y=p0[1];
		p1.SetData(10,
			p0x,
			p0y,
			p0x+AxW,
			p0y,
		    p0x + AxW,
			p0y + AxH,
			p0x,
			p0y + AxH,
			p0x,
			p0y);
		if(NoWaiKuang==0)
		{
			MoSpace.Invoke(_T("AddLightWeightPolyline"),1,(LPVARIANT)p1);
			pi.SetData(10,
				p0x + LeftMarginWidth,
				p0y + RightMarginWidth,
				p0x + AxW - RightMarginWidth,
				p0y + RightMarginWidth,
				p0x + AxW - RightMarginWidth,
				p0y + AxH - RightMarginWidth,
				p0x + LeftMarginWidth,
				p0y + AxH - RightMarginWidth,
				p0x + LeftMarginWidth,
				p0y + RightMarginWidth);
			//ͼ���߿�
			PLwidth = 0.3;
			AcObj = MoSpace.Invoke(_T("AddLightWeightPolyline"),1,(LPVARIANT)pi).pdispVal;
			for( i = 0 ;i<= 3;i++)
				AcObj.Invoke(_T("SetWidth"),3,&_variant_t((long)i),&_variant_t((double)EDIBAcad::g_fSetTKWidth), &_variant_t((double)EDIBAcad::g_fSetTKWidth));
		}
		//;�������÷����������ָ߶�
		DistSep = 75 * scal;
		TxtHeight = 3 * scal;
		_TCHAR ch[4]={0,0,0,0};
		if( TKsubzone == 0)
		{
		  //;����д��ֱ����ķ�������
		  NumSep = Fix(yi / DistSep / 2 + 1.5) * 2;
		  NewDistSep = yi / NumSep;
		  for( k = NumSep - 1 ;k>= 0;k--)
		  {
			 p2s.SetPoint(
				 p0[0],
				p0[1] + k * NewDistSep + RightMarginWidth,
				p0[2]);
			 p2e.SetPoint(
				 p0[0] + LeftMarginWidth,
				p0[1] + k * NewDistSep + RightMarginWidth,
				p0[2]);
			 txtP.SetPoint(
				 p0[0] + LeftMarginWidth / 2,
				p0[1] + (k + 0.5) * NewDistSep + RightMarginWidth,
				p0[2]);
			 if( k > 0 )
				 MoSpace.Invoke(_T("AddLine"),2,(LPVARIANT)p2s, (LPVARIANT)p2e);
			 ch[0]=_T('A')+NumSep-1-k;
			 AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(ch), (LPVARIANT)txtP,&_variant_t((double)TxtHeight)).pdispVal;
			 AcObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentCenter));
			 AcObj.PutPropertyByName(_T("VerticalAlignment"),&_variant_t((long)acVerticalAlignmentMiddle));
			 AcObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)txtP);
		  }
		  for(k = NumSep - 1 ;k>= 0 ;k--)
		  {
			 p2s.SetPoint(
				 p0[0] + AxW - RightMarginWidth,
				p0[1] + k * NewDistSep + RightMarginWidth,
				p0[2]);

			 p2e.SetPoint(
				 p0[0] + AxW,
				p0[1] + k * NewDistSep + RightMarginWidth,
				p0[2]);
			 txtP.SetPoint(
				 p0[0] + AxW - RightMarginWidth / 2,
				p0[1] + (k + 0.5) * NewDistSep + RightMarginWidth,
				p0[2]);
			 if( k > 0)
				 MoSpace.Invoke(_T("AddLine"),2,(LPVARIANT)p2s, (LPVARIANT)p2e);
			 ch[0]=_T('A')+NumSep-1-k;
			 AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(ch), (LPVARIANT)txtP,&_variant_t((double)TxtHeight)).pdispVal;
			 AcObj.PutPropertyByName(_T("VerticalAlignment"),&_variant_t((long)acVerticalAlignmentMiddle));
			 AcObj.PutPropertyByName(_T("TextAlignmentPoint"), (LPVARIANT)txtP);
		  }
		  //;����дˮƽ����ķ�������
		  NumSep = Fix(xi / DistSep / 2 + 1.5) * 2;
		  NewDistSep = xi / NumSep;
		  for( k = NumSep - 1 ;k>= 0;k--)
		  {
			 p2s.SetPoint(
				 p0[0] + k * NewDistSep + RightMarginWidth,
				p0[1] + AxH,
				p0[2]);
			 p2e.SetPoint(
				 p0[0] + k * NewDistSep + RightMarginWidth,
				p0[1] + AxH - RightMarginWidth,
				p0[2]);
			 txtP.SetPoint(
				 p0[0] + (k + 0.5) * NewDistSep + RightMarginWidth,
				p0[1] + AxH - RightMarginWidth / 2,
				p0[2]);
			 if(k > 0 )
				 MoSpace.Invoke(_T("AddLine"),2,(LPVARIANT)p2s, (LPVARIANT)p2e);
			 _stprintf(ch,_T("%d"),k+1);
			 AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(ch), (LPVARIANT)txtP, &_variant_t((double)TxtHeight)).pdispVal;
			 AcObj.PutPropertyByName(_T("VerticalAlignment"),&_variant_t((long)acVerticalAlignmentMiddle));
			 AcObj.PutPropertyByName(_T("TextAlignmentPoint"), (LPVARIANT)txtP);
		  }
		  for( k = NumSep - 1 ;k>= 0;k--)
		  {
			 p2s.SetPoint(
				 p0[0] + k * NewDistSep + RightMarginWidth,
				p0[1],
				p0[2]);
			 p2e.SetPoint(
				 p0[0] + k * NewDistSep + RightMarginWidth,
				p0[1] + RightMarginWidth,
				p0[2]);
			 txtP.SetPoint(
				 p0[0] + (k + 0.5) * NewDistSep + RightMarginWidth,
				p0[1] + RightMarginWidth / 2,
				p0[2]);
			 if( k > 0 )
				 	 MoSpace.Invoke(_T("AddLine"),2,(LPVARIANT)p2s, (LPVARIANT)p2e);
			 _stprintf(ch,_T("%d"),k+1);
			 AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(ch), (LPVARIANT)txtP, &_variant_t((double)TxtHeight)).pdispVal;
			 AcObj.PutPropertyByName(_T("VerticalAlignment"),&_variant_t((long)acVerticalAlignmentMiddle));
			 AcObj.PutPropertyByName(_T("TextAlignmentPoint"), (LPVARIANT)txtP);
		  }
		}
		//���²���ͼ��ȵ�,����ΪԤ��,�ʲ�����д����
		InsPnt.SetPoint(
			p0[0] + AxW - RightMarginWidth,
			p0[1] + RightMarginWidth,
			p0[2]);
		if( gbACADR14)
			AcEnt=MoSpace.Invoke(_T("InsertBlock"),5,(LPVARIANT)InsPnt, &_variant_t(CString((SizeIndex <= 1 ? _T("tb0") : _T("tb2"))) + ltos(EDIBDB::SJHYIndex)), &_variant_t((double)1), &_variant_t((double)1), &_variant_t((double)0)).pdispVal;
		else
		{
			CString str(CString((SizeIndex <= 1 ? _T("tb0") : _T("tb2"))) + ltos(EDIBDB::SJHYIndex));

			AcEnt=MoSpace.Invoke(_T("InsertBlock"),6,(LPVARIANT)InsPnt, &_variant_t(basDirectory::PhsBlkDir + str + ".dwg"), &_variant_t((double)1),&_variant_t((double)1), &_variant_t((double)1), &_variant_t((double)0)).pdispVal;
		}

		
		//�޸�ͼ������
		ModifyAttributes(BlkAtt,acCount, AcEnt);



		if( tbosFlag == 1)
		{
		  InsPnt.SetPoint(
			  p0[0] + LeftMarginWidth,
			p0[1] + AxH - RightMarginWidth,
			p0[2]);
		  if (gbACADR14)
			  MoSpace.Invoke(_T("InsertBlock"),5,(LPVARIANT)InsPnt,&_variant_t( _T("tb-os")),&_variant_t((double) 1), &_variant_t((double)1),&_variant_t((double) 0));
		  else
			 MoSpace.Invoke(_T("InsertBlock"),6,(LPVARIANT)InsPnt,&_variant_t( basDirectory::PhsBlkDir + CString("tb-os") + ".dwg"),&_variant_t((double) 1),&_variant_t((double) 1), &_variant_t((double)1),&_variant_t((double) 0));
		}
		if( TbhqFlag == 0 )
		{
		  InsPnt.SetPoint(
			  p0[0] + AxW - RightMarginWidth - (SizeIndex <= 1 ? EDIBDB::TB0Len : EDIBDB::TB2Len),
			p0[1] + RightMarginWidth,
			p0[2]);
		  if (gbACADR14)
			  MoSpace.Invoke(_T("InsertBlock"),5,(LPVARIANT)InsPnt,&_variant_t( _T("tb-hq")),&_variant_t((double) 1), &_variant_t((double)1),&_variant_t((double) 0));
		  else
			 MoSpace.Invoke(_T("InsertBlock"),6,(LPVARIANT)InsPnt,&_variant_t( basDirectory::PhsBlkDir + CString("tb-hq")+ ".dwg"),&_variant_t((double) 1),&_variant_t((double) 1), &_variant_t((double)1),&_variant_t((double) 0));
		}
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
}

void EDIBAcad::SetACADCurrentStyle(CString sStyle,CString sBigFont,CString sShxFont,float fWidthFactor)
{
	try
	{
		//GetActiveAcadDoc();
		//Dim TS As Object, i As Integer, Found As Boolean
		CMObject TS,tmpObj;
		CString sTmp;
		long i=0;bool Found=false;
		tmpObj=objAcadDoc.GetPropertyByName(_T("TextStyles")).pdispVal;
		sStyle.TrimLeft();sStyle.TrimRight();
		sStyle.MakeUpper();
		int C=(long)tmpObj.GetPropertyByName(_T("Count"));
		for( i = 0;i<C;i++)
		{
			TS = tmpObj.Invoke(_T("Item"),1,&_variant_t(i)).pdispVal;
			sTmp=vtos(TS.GetPropertyByName(_T("Name")));
			sTmp.TrimLeft();sTmp.TrimRight();
			sTmp.MakeUpper();
			if(sTmp==sStyle)
			{
				//���������Ϊ�ջ��߲����з�����hzfs
				TS.PutPropertyByName(_T("BigFontFile"),&_variant_t(sBigFont));
				TS.PutPropertyByName(_T("FontFile"),&_variant_t(sShxFont));
				TS.PutPropertyByName(_T("Width"),&_variant_t((double)fWidthFactor));

				//////������˾�޸�4��5 ��ǰ��û�� 
//				TS.PutPropertyByName(_T("height"),&_variant_t((double)4.5));
				
				Found=true;
				break;
			}
		}
		 // Debug.Print objAcadDoc.TextStyles.count, i, TS.Name, TS.fontfile, TS.BigFontFile
		if(Found)
			objAcadDoc.Invoke(_T("ActiveTextStyle"),1,&_variant_t(TS.p));
		else
		{
			tmpObj.Invoke(_T("Add"),1,&_variant_t(sStyle));
			TS = tmpObj.Invoke(_T("Item"),1,&_variant_t(sStyle)).pdispVal;
			if(TS.p)
			{
				TS.PutPropertyByName(_T("BigFontFile"),&_variant_t(sBigFont));
				TS.PutPropertyByName(_T("FontFile"),&_variant_t(sShxFont));
				TS.PutPropertyByName(_T("Width"),&_variant_t((double)fWidthFactor));

				//////������˾�޸�4��5 ��ǰ��û�� 
//				TS.PutPropertyByName(_T("height"),&_variant_t((double)4.5));

				objAcadDoc.Invoke(_T("ActiveTextStyle"),1,&_variant_t(TS.p));
			}
		}
	}
	catch(_com_error)
	{
	}
}

void EDIBAcad::SetACADStyleBigFont2Hzfs(CString sStyle)
{
	try
	{
		GetActiveAcadDoc();
		CMObject TS ,tmpObj;
		long i=0 ,C=0;
		bool Found=0;
		tmpObj=objAcadDoc.GetPropertyByName(_T("TestStyles")).pdispVal;
		C=(long)tmpObj.GetPropertyByName(_T("Count"));
		CString sTmp;
		sStyle.MakeUpper();
		for(i=0;i<C;i++)
		{
			TS =tmpObj.Invoke(_T("Item"),1,&_variant_t((long)i)).pdispVal;
			sTmp=vtos(TS.GetPropertyByName(_T("Name")));
			sTmp.MakeUpper();
			if(sStyle==_T("") || sTmp==sStyle)
			{
				sTmp=vtos(TS.GetPropertyByName(_T("BigFontFile")));
				sTmp.MakeLower();
				if(sTmp==_T("") || sTmp.Find(_T("hzfs"))!=-1)
					TS.PutPropertyByName(_T("BigFontFile"),&_variant_t(_T("hzfs")));
			}
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void EDIBAcad::DeleteACADStyle(CString sStyle)
{
	//ȱʡʱɾ��standard��hz��������֮�����������
	try
	{
		GetActiveAcadDoc();
		CMObject TS,tmpObj;
		long i=0,C=0;
		tmpObj=objAcadDoc.GetPropertyByName(_T("TextStyles")).pdispVal;
		C=(long)tmpObj.GetPropertyByName(_T("Count"));
		sStyle.MakeUpper();
		CString sTmp;
		for(i=C-1;i>=0;i--)
		{
			TS=tmpObj.Invoke(_T("Item"),1,&_variant_t((long)i)).pdispVal;
			sTmp=vtos(TS.GetPropertyByName(_T("Name")));
			sTmp.MakeUpper();
			if(sStyle==_T(""))
			{
				if( sTmp != _T("HZ") && sTmp != _T("STANDARD"))
					TS.Invoke0(_T("Delete"));
			}
			else
			{
				if(sTmp==sStyle)
					TS.Invoke0(_T("Delete"));
					break;
			}
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

long EDIBAcad::GetTableHeaderBlockAttributes(_RecordsetPtr rs, bool  &bATTBEGIN, long & NPage,long & iNPage,long & SPage, 
											 CADatt* & BlkAtt,int iTabIndex)
{
	//Ŀ��:��ȡtxx����ATTBEGIN�����ĺ�����¼��ÿ����¼��LocalCaption�ֶ�ֵָ����AutoCAD��ͷ�����Ե�Tag,
	//��˿������е����Զ������������Щ��¼�У������¼�����������������Զ���Ӧ��
	try
	{
		long iPosATTBEGIN=0 ;
		long i=0 ;
		EDIBgbl::GetSelPrjName();
//		bool bf=rs.FindFirst(_T("Ucase(Trim(LocalCaption))=\'ATTBEGIN\'"));
		_variant_t vTmp;
		rs->Find((_bstr_t)(_T("Ucase(Trim(LocalCaption))=\'ATTBEGIN\'")), 0, adSearchForward, vTmp);
		bool bf = rs->adoEOF;

		iPosATTBEGIN = rs->AbsolutePosition;
		long count=0,ret=0;
		if(BlkAtt!=NULL)
			delete [] BlkAtt;
		BlkAtt=NULL;
// 		COleVariant vTmp;
		if(bf)
		{
			bATTBEGIN = true;
			count=rs->RecordCount-iPosATTBEGIN;
			BlkAtt=new CADatt [count];
			for( i = 0 ;i< rs->RecordCount - iPosATTBEGIN;i++)
			{
				rs->MoveNext();
				if( rs->adoEOF)
					break;
				rs->get_Collect((_variant_t)_T("LocalCaption"),&vTmp);
				BlkAtt[i].Name = vtos(vTmp);
				BlkAtt[i].Name.TrimLeft();
				BlkAtt[i].Name.TrimRight();
				BlkAtt[i].Name.MakeUpper();
				BlkAtt[i].iWidth=0;
				try
				{
					rs->get_Collect((_variant_t)_T("cadWidth"),&vTmp);
					BlkAtt[i].iWidth=vtoi(vTmp);
				}
				catch(CException *e)
				{
					e->Delete();
				}
				if(BlkAtt[i].Name == _T("NPAGE"))
				{
					BlkAtt[i].Text.Format(_T("%d"),NPage);
					iNPage = i;
				}
				else if( BlkAtt[i].Name == _T("SPAGE") )
					BlkAtt[i].Text.Format(_T("%d"),SPage);
				else if( BlkAtt[i].Name == _T("ENGNAME") || BlkAtt[i].Name == _T("PROJECT_NAME") )
				{
					BlkAtt[i].Text = EDIBgbl::SelPrjName;					
					BlkAtt[i].iWidth=EDIBAcad::iA2PrjNameWidth;
				}
				else if( BlkAtt[i].Name == _T("DRAWING_NAME") )
				{
					CString str;
					if( iTabIndex==EDIBgbl::TCL || iTabIndex==EDIBgbl::TCL4 || iTabIndex==EDIBgbl::TCLA4 )
						str.LoadString(IDS_PHS_STUFF_GATHERTABLE);
					else if ( iTabIndex==EDIBgbl::TZDSWED180 || iTabIndex==EDIBgbl::TZD800HSY)
						str.LoadString(IDS_PHS_DETAIL_TABLE);
					BlkAtt[i].Text = str;
					BlkAtt[i].iWidth=EDIBAcad::iA2DrawNameWidth;					
				}
				else if( BlkAtt[i].Name == _T("DRAWING_NO") )
				{
					if( EDIBAcad::g_bCustomPicNo )
						BlkAtt[i].Text = EDIBAcad::g_strCustomPicNoFileName;
					else
						BlkAtt[i].Text = EDIBAcad::GetDrawIDAndName(modPHScal::zdjh + modPHScal::glDrawNoStartNumber - 1, EDIBDB::DrawName);
					BlkAtt[i].iWidth=EDIBAcad::iA2DrawNoWidth;
				}
				else if( BlkAtt[i].Name == _T("STAGE") )
					BlkAtt[i].Text = EDIBgbl::SelDsgnName;
				else if( BlkAtt[i].Name == _T("SPECILITY") || BlkAtt[i].Name == _T("SPECIALITY"))	//������Щ��ͷ������дΪSPECILITY,SPECIALITY
				{
					BlkAtt[i].Text = EDIBgbl::SelSpecName;
				}
				else if( BlkAtt[i].Name == _T("JC-NAME") || BlkAtt[i].Name == _T("JC_NAME") )	//��ϸ�ֱ�����JC-NAME��ͬ��JC_NAME(�м�ָ������»���)
				{
					BlkAtt[i].Text = EDIBgbl::SelVlmName;
					BlkAtt[i].iWidth=EDIBAcad::iA2VlmNameWidth;
				}
				else if( BlkAtt[i].Name == _T("VOLUME") )
				{
					if(EDIBgbl::SelJcdm.GetLength()>=4)
						BlkAtt[i].Text = EDIBgbl::SelJcdm.Left(2);
					else
						BlkAtt[i].Text = EDIBgbl::SelJcdm;
				}
				else if( BlkAtt[i].Name == _T("SUB-VOLUME") )
				{
					if(EDIBgbl::SelJcdm.GetLength()>=4)
						BlkAtt[i].Text = EDIBgbl::SelJcdm.Mid(2);
					else
						BlkAtt[i].Text = EDIBgbl::SelJcdm;
				}
				else if( BlkAtt[i].Name == _T("WEIGHT") )
					BlkAtt[i].Text.Format(_T("%2.1f"),(float)EDIBgbl::gsngSumBomWeight);
				else if( BlkAtt[i].Name == _T("DRAWINGS") )
					BlkAtt[i].Text = _T("36");
				else if( BlkAtt[i].Name == _T("YEAR") )
					BlkAtt[i].Text = CTime::GetCurrentTime().Format("%y");
				else if( BlkAtt[i].Name == _T("MONTH") )
					BlkAtt[i].Text = CTime::GetCurrentTime().Format("%m");
				else if( BlkAtt[i].Name == _T("DATE") )
				{
					BlkAtt[i].Text = CTime::GetCurrentTime().Format("%d");
					BlkAtt[i].iWidth=EDIBAcad::iA2DateWidth;
				}
				else if( BlkAtt[i].Name == _T("SCALE") )
					BlkAtt[i].Text = _T("/");
				else if(BlkAtt[i].Name == _T("DATETIME") )
				{
					BlkAtt[i].Text = CTime::GetCurrentTime().Format(_T("%Y.%m.%d"));
					BlkAtt[i].iWidth=EDIBAcad::iA2DateWidth;
				}
				else if( BlkAtt[i].Name == _T("CORP_NAME") )
					BlkAtt[i].Text = ModEncrypt::gstrLegalCompanyName;
			}
		}
		return i;
	}
	catch(CException *e)
	{
		e->Delete();
		
		if(BlkAtt!=NULL)
			delete [] BlkAtt;
		BlkAtt=NULL;
		return 0;
	}
}
extern BOOL gStartAcad();
void EDIBAcad::DrawTableACAD(CCadPoint& pB, long BillID,
         double sngRotAng, _RecordsetPtr rsText,
         long lRowsPerPages, float fBlkScale,
         LPCTSTR Layer, LPCTSTR  Style,
         LPCTSTR lpszColTxtFmt,int iAlign)
{
	if( rsText->State != adStateOpen )//12/17  ʹ�ö�̬�⻭ͼ
	{
		try  
		{
			CString sTmp,sTmp2;
		//	modPHScal::iSelSampleID=(long)FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("iSelSampleID"));
			
			SetRegValue(_T("Settings"),_T("PhsCurZdjhNo"),modPHScal::zdjh);
			SetRegValue(_T("Settings"),_T("DrawTableFlag"),bool(true));           //�����14���CAD������COM������Ϣ,ֻ����PHS���ٷ�һ��.
			CString strLayer = Layer; 
			SetRegValue("Settings", "PhsCurTblLayer", strLayer);             //����
			SetRegValue("Settings","PhsCurVlmID",EDIBgbl::SelVlmID );		//����
			SetRegValue("Settings","PhsCurTblID",BillID);					//������ID��
			SetRegValue("Settings","PhsCurTblScale",fBlkScale);					//�����ı���

			if( EDIBAcad::gbACADR14 )
			{
				gStartAcad();     //��ʼCAD
			}
			else
			{
				EDIBAcad::StartAcad();
			}
			EDIBAcad::DisplayAcadTop();
			EDIBAcad::objAcadDoc=EDIBAcad::objAcadApp.GetPropertyByName(_T("ActiveDocument")).pdispVal;

			CString strTblID;
			strTblID.Format("drawzdjtable\n%d\n", BillID);
			EDIBAcad::objAcadDoc.Invoke(_T("SendCommand"),1,&_variant_t(strTblID));
    
			SetRegValue(_T("Settings"),_T("DrawTableFlag"),bool(false));

			return;	
		}
		catch(CException *e)
		{
			e->ReportError();
			e->Delete();
		}
		catch(COleException* e)
		{
			e->ReportError();
			e->Delete();
		}
		catch(_com_error e)
		{		
			AfxMessageBox(e.Description());
		}
		return ;  //12/17
	}

	//����:���Ʊ��
	//Pb------����,3Ԫ������,˫����
	//BILLID--�������,һ����SelBillType����
	//sngRotAng--��ת��(����),˫����
	//rsText-----���Ƶ�����,recordset����
	//lRowsPerPages--ÿҳ����,��ѡ����
	//BlkScale-------�����,��ѡ����
	//Layer----------���ƵĲ�
	//Style----------���Ƶ����壬Ĭ��Ϊ_T("hz")
	//���:���Ƶı����Դ�������(����bUP=True)��Ҳ���Դ�������(����bUP=False)
	CADatt *BlkAtt=NULL; long blkAttCount=0;
	try
	{
		_TCHAR* ptTemp;
		bool bATTBEGIN=0; 
		bool bGridLine=0;
		bool bUP=0;   //д���ֵķ���:From Down to UP,bUP=true
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		CString strTableName;
		CString BlkName;
		CString ColTxt, ColFormat;
		CString tmpStr;
		CString tmp;
		CCadPoint p00, pR0, p0c, pB1;
		CCadPoint p0s, p0e, pAfter1, pAfter2;
		CCadPoint txtP;
		CCadPoint minExt;
		CCadPoint MaxExt;
		CCadPoint InsPnt;
		CMObject Block;
		long i=0, j=0, k=0;
		long RowsPerPage=0;
		long iNPage=0;
		long NPage=0;    //��nҳ
		long SPage=0;    //��ҳ��
		long  StartNPage=0;  //ͼ�����Ѿ����ڵı����,��ѯuseri1�õ�
		long VerticalPages=0; //��ֱ�����ҳ��
		float sumWeight=0;    //�������
		float ColW=0, ColTxtHeight=0;
		float sumColW=0;
		float sngRowHeight=0, W0=0 , H0=0 ;
		float fTemp;
		float  DistX=0 , DistY=0;
		//	BOOL  count = TRUE;
		
		
		

		bool flag = false;
		DistX = 20;
		DistY = 20;
		CString SQLx;
		//���ȴӹ����t??�л�ȡ���ĸ߿�ÿҳ��������Ϣ
		SQLx = _T("SELECT * FROM TableINFO WHERE ID=") + ltos(BillID);
// 		rs.m_pDatabase=&EDIBgbl::dbTable;//20071109 "dbSORT" ��Ϊ "dbTable"
// 		rs.Open(dbOpenSnapshot,SQLx);
		rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbTable,true), 
			adOpenForwardOnly, adLockReadOnly, adCmdText); 
		COleVariant vTmp;
		rs->get_Collect((_variant_t)_T("TableName"),&vTmp);//�����Ҫ���Ƶı���ʽ��Ϣ��ı���
		strTableName = vtos(vTmp);
		
		rs->get_Collect((_variant_t)_T("CADtitleBlock"),&vTmp);//�����Ŀ���
		BlkName=vtos(vTmp);
		
		rs->get_Collect((_variant_t)_T("CADRowsPerpage"),&vTmp);//��Ҫ���Ƶı�ÿҳ�����ɵ�����
		RowsPerPage=vtoi(vTmp);
		
		rs->get_Collect((_variant_t)_T("CADRowHeight"),&vTmp);//�����и�
		sngRowHeight=vtof(vTmp);
		
		rs->get_Collect((_variant_t)_T("CADFromDownToUP"),&vTmp);//����Ƿ�������»���
		bUP=vtob(vTmp);
		
		rs->get_Collect((_variant_t)_T("CADGridLine"),&vTmp);//�Ƿ��и���
		bGridLine=vtob(vTmp);
		
		rs->get_Collect((_variant_t)_T("CADW0"),&vTmp);//
		W0=vtof(vTmp);
		
		rs->get_Collect((_variant_t)_T("CADh0"),&vTmp);//
		H0=vtof(vTmp);
		
		rs->Close();
		
		if( (sngRowHeight*RowsPerPage) < 200)
			VerticalPages = 3;
		else
			VerticalPages = 2;
		
		
		p00.SetX(W0);
		p00.SetY(H0);
		
		if(!bUP)
			p00.SetY(p00[1] - sngRowHeight);
		
		InsPnt.SetPoint(0.0,0.0,0.0);
		
		MakeNewLayer(Layer);
		
		//���������������ÿҳ����,�������趨
		rsText->MoveLast();
		if( rsText->RecordCount < RowsPerPage) RowsPerPage = rsText->RecordCount;
		NPage = 1;
		SPage = (rsText->RecordCount / RowsPerPage) +((rsText->RecordCount % RowsPerPage) == 0 ? 0 : 1);
		
		
		SQLx = _T("SELECT * FROM [") + strTableName + _T("]");
// 		rs.Open(dbOpenSnapshot,SQLx );
		rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbTable,true), 
			adOpenForwardOnly, adLockReadOnly, adCmdText); 
		rs->MoveLast();
		//��ÿ�����ֵ
		blkAttCount=GetTableHeaderBlockAttributes( rs, bATTBEGIN, NPage, iNPage, SPage, BlkAtt,BillID);
		rs->Close();
		
		
		SQLx = _T("SELECT * FROM [") + strTableName + _T("] WHERE NOT ISNULL(CADFieldSeq) ORDER BY CADFieldSeq");
// 		rs.Open(dbOpenSnapshot,SQLx );
		rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbTable,true), 
			adOpenForwardOnly, adLockReadOnly, adCmdText); 
		rs->MoveLast();
		//MsgBox rsText(0)
		if(rsText->adoEOF || rsText->BOF)
		{
			if(BlkAtt!=NULL)
				delete [] BlkAtt;
			return;
		}
		rsText->MoveFirst();
		//��һҳ�����ʼҳ��
		StartNPage = (NPage == 1 ? 0 : (long)objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("USERI1"))));
		//���һҳ������
		long iLastPageRecNo;
		iLastPageRecNo = rsText->RecordCount % RowsPerPage;
		if(iLastPageRecNo == 0)
			iLastPageRecNo = RowsPerPage;
		
		//��ʼ���л����
		while(!rsText->adoEOF)
		{
			//�����BJBW��Ҫ�µ�Y���겢����LJMX
			if( BillID == EDIBgbl::TLJ_BJBW )
			{
				if(EDIBAcad::g_bLJMX_BJBW)
				{
					pB1.SetPoint(
						pB[0] + ((StartNPage + NPage - 1) / VerticalPages) * ((210 + DistX) * cos(sngRotAng) - (297 + DistY) * sin(sngRotAng)),
						pB[1] + ((StartNPage + NPage - 1) % VerticalPages) * ((210 + DistX) * sin(sngRotAng) + (297 + DistY) * cos(sngRotAng)) + 9);
				}
				else
				{
					pB1.SetPoint(
						pB[0] + ((StartNPage + NPage - 1) / VerticalPages) * ((210 + DistX) * cos(sngRotAng) - (297 + DistY) * sin(sngRotAng)),
						pB[1] + ((StartNPage + NPage - 1) % VerticalPages) * ((210 + DistX) * sin(sngRotAng) + (297 + DistY) * cos(sngRotAng)));
				}
				
			}
			else if( BillID == EDIBgbl::TzDLOADNEW )
			{
				if( EDIBAcad::g_bDLOADBJBW )
				{
					pB1.SetPoint(
						pB[0] + ((StartNPage + NPage - 1) / VerticalPages) * ((210 + DistX) * cos(sngRotAng) - (287 + DistY) * sin(sngRotAng)),
						pB[1] + ((StartNPage + NPage - 1) % VerticalPages) * ((210 + DistX) * sin(sngRotAng) + (287 + DistY) * cos(sngRotAng)) - 35);
					pB.SetY( pB[1] - 35 );
				}
				else
				{
					pB1.SetPoint(
						pB[0] + ((StartNPage + NPage - 1) / VerticalPages) * ((210 + DistX) * cos(sngRotAng) - (297 + DistY) * sin(sngRotAng)),
						pB[1] + ((StartNPage + NPage - 1) % VerticalPages) * ((210 + DistX) * sin(sngRotAng) + (297 + DistY) * cos(sngRotAng)));
				}
			}
			else
			{
				pB1.SetPoint(
					//	pB[0] + ((StartNPage + NPage - 1) / VerticalPages) * ((210 + DistX) * cos(sngRotAng) - (297 + DistY) * sin(sngRotAng)),
					//	pB[1] + ((StartNPage + NPage - 1) % VerticalPages) * ((210 + DistX) * sin(sngRotAng) + (297 + DistY) * cos(sngRotAng)));
					pB[0] + ((StartNPage + NPage - 1) / VerticalPages) * (((fabs(W0)+20) + DistX) * cos(sngRotAng) - ((sngRowHeight*RowsPerPage+80) + DistY) * sin(sngRotAng)),
					pB[1] + ((StartNPage + NPage - 1) % VerticalPages) * (((fabs(W0)+20) + DistX) * sin(sngRotAng) + ((sngRowHeight*RowsPerPage+80) + DistY) * cos(sngRotAng)));
			}
			//��ͷ�����
			//����Ժ
			//if( BillID == EDIBgbl::TLJ )
			//	pB1.SetY(pB1.GetY()+2);
			Block = objAcadDoc.GetPropertyByName(_T("ModelSpace")).pdispVal;
			if(gbACADR14)
				AcEnt = Block.Invoke(_T("InsertBlock"),5,(LPVARIANT)pB1, &_variant_t(BlkName), &_variant_t((double)1), &_variant_t((double)1), &_variant_t((double)0)).pdispVal;
			else
				AcEnt = Block.Invoke(_T("InsertBlock"),6,(LPVARIANT)pB1, &_variant_t(basDirectory::PhsBlkDir + BlkName + ".dwg"), &_variant_t((double)1), &_variant_t((double)1), &_variant_t((double)1), &_variant_t((double)0)).pdispVal;
			
			//��ͷ���������,��Щ���û��
			ModifyAttributes(BlkAtt,blkAttCount, AcEnt);
			NPage++;
			if( bATTBEGIN )
				BlkAtt[iNPage].Text.Format(_T("%d"),NPage);
			//��ÿһҳ
			for( k = 0 ;k< RowsPerPage;k++)
			{
				if( rsText->adoEOF)
				{
					if(BlkAtt!=NULL)
						delete [] BlkAtt;
					return;
				}
				rs->MoveFirst();
				//	VARIANT va1;
				//	va1.vt= VT_I2;
				//	va1.iVal=1;
				//	VARIANT va2;
				//	va2.vt  = VT_I2;
				//	va2.iVal=2;
				
				COleVariant Tmp;
				//	CString strZDMC = _T("ZDMC");
				
				//	COleVariant vt;
				COleVariant vpaz1;
				//	vpaz1.Attach(va1);
				
				COleVariant vpgz1;
				//	vpgz1.Attach(va2);
				
				if(BillID == EDIBgbl::TLJ || BillID == EDIBgbl::TCL || BillID == EDIBgbl::TCLA4 )
				{
					rsText->get_Collect((_variant_t)_T("CLdw"),&Tmp);
					rsText->get_Collect((_variant_t)_T("CLzz"),&vTmp);
					if(vTmp.vt!=VT_NULL)
					{
						//�ۼ�����
						double sumweight = vtof(vTmp);
						sumweight = 100*sumweight;
						sumweight = int (sumweight+0.5);
						sumweight = sumweight/100;
						sumWeight = sumWeight + sumweight;
						//sumWeight = sumWeight +vtof(vTmp);
					}
				}
				else if(BillID == EDIBgbl::TzDLOADNEW )//|| EDIBgbl::TzDLOADS)
				{
					
					//	rsText.GetFieldValue (strZDMC,vt);
					rsText->get_Collect((_variant_t)"PAZ1",&vpaz1);
					rsText->get_Collect((_variant_t)"PGZ1",&vpgz1);
				}
				sumColW = 0;
				//��0�л���
				pR0.SetPoint(
					pB1[0] + p00[0],
					pB1[1] + p00[1] + (bUP ? k : -k) * sngRowHeight);
				//rs�к����ֶμ����ʽ��Ϣ
				
				CString strCLgg= _T("CLgg");//���
				CString strMaterial= _T("CLcl");
				CString strPartNumber= _T("CLnum");
				CString strPartSingleWeight= _T("CLdz");
				CString strPartZZ;
				BOOL bNotDraw = FALSE;
				for( i = 0 ;i<rs->RecordCount;i++)
				{
					rs->get_Collect((_variant_t)_T("CADFieldWidth"),&vTmp);
					if(vTmp.vt==VT_NULL)
						ColW = 10;
					else
						ColW = vtof(vTmp);
					rs->get_Collect((_variant_t)_T("CADFieldTxtHeight"),&vTmp);
					if(vTmp.vt==VT_NULL)
						ColTxtHeight = 3;
					else
						ColTxtHeight =vtof(vTmp);
					rs->get_Collect((_variant_t)_T("FieldName"),&vTmp);
					strPartZZ = ColTxt = vtos(vTmp);
					
					if(ColTxt!=_T(""))
					{
						try
						{
							rsText->get_Collect((_variant_t)ColTxt,&vTmp); //Ĭ�ϵ�(ԭ������Ķ�ȡ����)
							
							//��"�û��ֹ������ͼ��"��ѡ��ѡ�� �� ��ǰ���ݿ��ȡ���ֶ�ΪDrawNo
							//���ȡALLPRJDB.MDB�е�ZA����ֶ�ΪCustomDwgName��ֵ��
							//�޸��ߣ�YWH
							CString str = ColTxt;
							str.MakeUpper();
							if( EDIBAcad::g_bCustomPicNo && str == _T("DRAWNO") )//���Ϊ�û��ֹ������ͼ��
							{
								long m_volumeID,m_zdjh;
								COleVariant var;
								
								rsText->get_Collect((_variant_t)_T("VolumeID"),&var);
								m_volumeID = vtoi(var);
								
								rsText->get_Collect((_variant_t)_T("ZDJH"),&var);
								m_zdjh = vtoi(var);
								
								_RecordsetPtr rs1;
								rs1.CreateInstance(__uuidof(Recordset));
								CString SQLstr;
								SQLstr.Format("SELECT * FROM ZA WHERE VolumeID = %d AND ZDJH = %d ",m_volumeID,m_zdjh);
								
// 								rs1.m_pDatabase=&EDIBgbl::dbPRJDB;
// 								rs1.Open(dbOpenSnapshot,SQLstr);
								rs1->Open((_bstr_t)SQLstr, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
									adOpenForwardOnly, adLockReadOnly, adCmdText); 
								
								if(!rs1->adoEOF && !rs1->BOF)
								{
									rs1->get_Collect((_variant_t)_T("CustomDwgName"),&vTmp);
								}
								rs1->Close();
								
							}
							//�޸����  BY YWH
							
							
							//��ʱ�����޸ģ�������˾��Ҫ�����޸�
//							if(vtos(vTmp).Find("�ܼ�")!=-1)
//								vTmp = STR_VAR("װ���");
						    

							if( (ColTxt == strMaterial)				//����Ϊ"����"���ֶ�
								&& (BillID == EDIBgbl::TCLA4		/*���ϻ��ܱ�A4*/ 
								|| BillID == EDIBgbl::TLJ		/*�����ϸ��*/ 
								|| BillID == EDIBgbl::TLJ_BJBW  /*BJBW�����ϸ��*/ 
								|| BillID == EDIBgbl::TCL))/*���ϻ��ܱ�*/
							{
								//"���"����"װ���"�����Զ���
								if( vtos(vTmp) == "���")
								{
									if(EDIBAcad::g_bGroupWare == 1)
										vTmp = STR_VAR("װ���");
									else if(EDIBAcad::g_bGroupWare == 2)
										vTmp = STR_VAR(EDIBAcad::g_strGroupWareName);
								}
							}
							/*
							&& EDIBAcad::g_bGroupWare==1 )			"���"����"װ���"
							{
							if( vtos(vTmp) == "���")
							{
							vTmp = STR_VAR("װ���");//HS->Add
							}
							}
							*/								
							
							else if( EDIBAcad::g_bNotDrawSingleWeight//�������ϸ���в��浥�����������
								&&
								(ColTxt == strPartNumber)//����Ϊ"�������"���ֶ�
								&& (BillID == EDIBgbl::TCLA4		/*���ϻ��ܱ�A4*/ 
								|| BillID == EDIBgbl::TLJ		/*�����ϸ��*/ 
								|| BillID == EDIBgbl::TLJ_BJBW  /*BJBW�����ϸ��*/ 
								|| BillID == EDIBgbl::TCL)		/*���ϻ��ܱ�*/
								)			
							{
								if( vtof(vTmp) == 1.0f )
									bNotDraw = TRUE;//���������ĿΪ1��
							}
							else if( EDIBAcad::g_bNotDrawSingleWeight//�������ϸ���в��浥�����������
								&& bNotDraw
								&& (ColTxt == strPartSingleWeight)//����Ϊ"�������"���ֶ�
								&& (BillID == EDIBgbl::TCLA4		/*���ϻ��ܱ�A4*/ 
								|| BillID == EDIBgbl::TLJ		/*�����ϸ��*/ 
								|| BillID == EDIBgbl::TLJ_BJBW  /*BJBW�����ϸ��*/ 
								|| BillID == EDIBgbl::TCL)		/*���ϻ��ܱ�*/
								)			
							{
								vTmp = STR_VAR("");//���浥��������������Խ��ַ����ÿ�
								bNotDraw=FALSE;//�������Ҫ���˱������λ							
							}
							else if( BillID == EDIBgbl::TzDLOAD		/*֧����λ�ƺ��ر�(NW)*/ 
								)		/*֧���ܺ��ر�(EC)*/ 
							{
								if(ColTxt == _T("PAZ1") )//����Ϊ"��װ����"���ֶ�,���Ұ�װ
								{
									COleVariant vpaz1,vpgz1;
									rsText->get_Collect((_variant_t)"PAZ1",&vpaz1);
									rsText->get_Collect((_variant_t)"PAZ1",&vpgz1);
									if(vtof(vpaz1)==0.0f || vtof(vpaz1)==vtof(vpgz1))
									{
										vTmp = STR_VAR("");//��װ����д��"/"
										bNotDraw=FALSE;//�������Ҫ���˱������λ	
									}
								}
							}
							
						}
						catch(CException *e)
						{
							e->Delete();
						}
						if(vTmp.vt==VT_NULL)
							ColTxt = _T("");
						else
						{
							ColTxt=vtos(vTmp);

							if(EDIBAcad::g_bLJMX_BJBW)
							{
								if(ColTxt.Find("�ܼ�")!=-1)
								{
									flag = true;
									
								}
								
								CString str = strPartZZ;
								str.MakeLower();
								
								if(flag && str == "clcl")
								{
									ColTxt = "װ���";
									flag = false;
								}
							}
						
						

							//�������AutoCAD����д����ĸ,ת��
							if(ColTxt.Find((_T("��")))!=-1)
								ColTxt.Replace(_T("��"),_T("%%c"));
							if(ColTxt.Find((_T("��")))!=-1)
								ColTxt.Replace(_T("��"),_T("%%c"));
							rs->get_Collect((_variant_t)_T("Format"),&vTmp);
							ColFormat = vtos(vTmp);
							if(ColFormat!=_T(""))
							{
								fTemp=_tcstod(ColTxt,&ptTemp);
								if(*ptTemp==0)
								{
									if(vtos(Tmp)!="��")
									{
										if(  strPartZZ == _T("CLzz"))
										{
											ColTxt = FormatVBString(ColFormat,fTemp+0.005);	
										}
								//		else if((vtof(vpaz1)==0.0f || vtof(vpaz1)==vtof(vpgz1))&&strPartZZ == _T("paz1"))
								//		{
								//				ColTxt = "  /";
								//		}	
										else
										{
											ColTxt = FormatVBString(ColFormat,fTemp);
										}
									} 
								}
								
							}
							//		if(BillID == EDIBgbl::TzDLOAD)
							//		 {
							/*	 if((vtof(vpaz1)==0.0f || vtof(vpaz1)==vtof(vpgz1))&&strPartZZ == _T("paz1"))
							{
							ColTxt = "  /";
							}									
							else
							{
							ColTxt=FormatVBString(ColFormat,fTemp);
						}*/
							//	 }
							
						} 
						//pR0--��λ��
						//p0c--��λ��
						p0c.SetPoint(pR0[0] + sumColW,pR0[1]);
						if(bGridLine && (k == 0))
						{
							//���ֶΣ���ֱ)�ָ���
							//��һ��ֱ��(����),Ӧ�û�����
							p0s.SetPoint(p0c[0],p0c[1] + (bUP ? 0 : 1) * sngRowHeight,p0c[2]);
							p0e.SetX(p0s[0]);
							//��������һҳ����¼����=iLastPageRecNo,�����¼����=RowsPerPage�����Ѿ�������ֻ��һҳ�������
							p0e.SetY(p0s[1] + (bUP ? 1 : -1) * (NPage - 1 == SPage ? iLastPageRecNo : RowsPerPage) * sngRowHeight);
							p0e.SetZ(p0s[2]);
							//AcEnt = Block.Invoke(_T("AddLine"),2,(LPVARIANT)p0s, (LPVARIANT)p0e);
							DrawPLine(p0s,p0e,EDIBAcad::g_fSetTableWidth);
							if( i == rs->RecordCount - 1 ) 
							{
								////////�����һ�е�����. 05/1/4
								pAfter1 = p0s; 
								pAfter1.SetX( pAfter1.GetX() + ColW);

								pAfter2 = p0e;
								pAfter2.SetX( pAfter2.GetX() + ColW);
								DrawPLine(pAfter1, pAfter2, EDIBAcad::g_fSetTableWidth);
								/////�����һ�е�����. 05/1/4
							}
						}
						//������ʵ��λ��
						//�����1mmд
						p0c.SetPoint(pR0[0] + sumColW + 1,pR0[1] + 1);
						
						//	if(ColTxt!=_T(""))
						//	{
						//		if((vtof(vpaz1)==0.0f || vtof(vpaz1)==vtof(vpgz1))&&count)
						//		{
						//				AcEnt = Block.Invoke (_T("AddText"),3,&_variant_t("   /"),(LPVARIANT)p0c, &_variant_t((double)ColTxtHeight));
						//               AcEnt.PutPropertyByName(_T("StyleName"),&_variant_t(Style));							
						//             	GetBoundingBox(AcEnt,minExt, MaxExt);
						//		        count = FALSE;
						//		}
						//	    else
						//		{
						AcEnt = Block.Invoke(_T("AddText"),3,&_variant_t(ColTxt), (LPVARIANT)p0c, &_variant_t((double)ColTxtHeight)).pdispVal;
						AcEnt.PutPropertyByName(_T("StyleName"),&_variant_t(Style));
						ApplyTextStyleProperty(AcEnt, Style,"�����ϸ��" );
						GetBoundingBox(AcEnt,minExt, MaxExt);
						//		}
						double x,y;
						x=minExt[0];
						y=minExt[1];
						x=MaxExt[0];
						y=MaxExt[1];
						//���ֳ����涨��Χ���ĳ�ˮƽѹ�����뷽ʽ
						if(fabs(MaxExt[0] - minExt[0]) > ColW - 2)
						{
							txtP.SetPoint(p0c[0] + ColW - 2,p0c[1],p0c[2]);
							AcEnt.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentFit));
							AcEnt.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)txtP);
						}
						else
						{
							txtP=p0c;
							switch(iAlign)
							{
							case 0://�����
								break;
							case 1://�Ҷ���
								txtP.SetX(txtP[0] + ColW - 2);
								AcEnt.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentRight));
								AcEnt.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)txtP);
								break;
							case 2://����
								txtP.SetX(txtP[0] + (ColW - 2)/2.0);
								AcEnt.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentCenter));
								AcEnt.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)txtP);
								break;
							}
						}
					}
					//HS->�����һ�У������ϸ�����ϻ��ܱ�Ҫͳ������
					if((rsText->AbsolutePosition == rsText->RecordCount - 1) &&
						(BillID == EDIBgbl::TLJ/*�����ϸ��*/ ||
						BillID == EDIBgbl::TCL/*���ϻ��ܱ�*/ ||
						BillID == EDIBgbl::TCLA4) )
					{
						if( BillID == EDIBgbl::TLJ )
						{
							if( EDIBAcad::g_bDrawSumWeight )
							{		
								rs->get_Collect((_variant_t)_T("FieldName"),&vTmp);
								tmpStr=vtos(vTmp);
								tmpStr.TrimLeft();tmpStr.TrimRight();
								tmpStr.MakeUpper();
								if(tmpStr==_T("CLZZ"))//��������Ϣ
								{
									txtP.SetPoint(p0c[0],p0c[1] + (bUP ? 1 : -1) * sngRowHeight,p0c[2]);
									ColTxt.Format(_T("%s%.2fkg"), GetResStr(IDS_MsgBox_60640) ,sumWeight);
									AcEnt=Block.Invoke(_T("AddText"),3,&_variant_t(ColTxt), (LPVARIANT)txtP, &_variant_t((double)ColTxtHeight)).pdispVal;				
									AcEnt.PutPropertyByName(_T("StyleName"),&_variant_t(Style));
                                    ApplyTextStyleProperty(AcEnt, Style,"�����ϸ��");
								}
							}
						}
						else
						{
							rs->get_Collect((_variant_t)_T("FieldName"),&vTmp);
							tmpStr=vtos(vTmp);
							tmpStr.TrimLeft();tmpStr.TrimRight();
							tmpStr.MakeUpper();
							if(tmpStr==_T("CLZZ"))//��������Ϣ
							{
								txtP.SetPoint(p0c[0],p0c[1] + (bUP ? 1 : -1) * sngRowHeight,p0c[2]);
								ColTxt.Format(_T("%s%.2fkg"), GetResStr(IDS_MsgBox_60640) ,sumWeight);
								AcEnt=Block.Invoke(_T("AddText"),3,&_variant_t(ColTxt), (LPVARIANT)txtP, &_variant_t((double)ColTxtHeight)).pdispVal;				
								AcEnt.PutPropertyByName(_T("StyleName"),&_variant_t(Style));
                                ApplyTextStyleProperty(AcEnt, Style,"�����ϸ��");
							}						
						}
					}
					rs->MoveNext();
					sumColW = sumColW + ColW;
					if( bGridLine && (i == rs->RecordCount - 1) )
					{
						if( k == 0 && BillID == EDIBgbl::TZD800HSY )
						{	
							p0s.SetPoint(pR0[0] + sumColW,
								pR0[1] + (bUP ? 0 : 1) * sngRowHeight,
								pR0[2]);

							p0e.SetPoint(p0s[0],
								0,
								p0s[2]);

							p0e.SetY(p0s[1] + (bUP ? 1 : -1) * (NPage - 1 == SPage ? iLastPageRecNo : RowsPerPage) * sngRowHeight);

							AcEnt=Block.Invoke(_T("AddLine"),2,(LPVARIANT)p0s, (LPVARIANT)p0e).pdispVal;
						}
						//д�����һ�к�Ҫ����¼�ָ���,������������֮��ķָ��ߣ����ı�Y���ꡣ
						//p0s--����㣬p0e--���յ�
						p0s.SetPoint(pR0[0],pR0[1] + (bUP ? 1 : 0) * sngRowHeight,pR0[2]);
						p0e.SetPoint(p0s[0] + sumColW,p0s[1],p0s[2]);
						AcEnt=Block.Invoke(_T("AddLine"),2,(LPVARIANT)p0s, (LPVARIANT)p0e).pdispVal;
					}
				}
				rsText->MoveNext();
			}
			AcEnt.Invoke0(_T("Update"));
		}
		objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("USERI1")), &_variant_t((long)(StartNPage + SPage)));
   }
   catch(CException *e)
   {
	   e->ReportError();
	   e->Delete();
   }
   if(BlkAtt!=NULL)
	   delete [] BlkAtt;
}


void EDIBAcad::DrawTableExcel(long BillID, _RecordsetPtr rsText)
{
	//����:���Ʊ��
	//Pb------����,3Ԫ������,˫����
	//BILLID--�������,һ����SelBillType����
	//sngRotAng--��ת��(����),˫����
	//rsText-----���Ƶ�����,recordset����
	//lRowsPerPages--ÿҳ����,��ѡ����
	//BlkScale-------�����,��ѡ����
	//Layer----------���ƵĲ�
	//Style----------���Ƶ����壬Ĭ��Ϊ_T("hz")
	//���:���Ƶı����Դ�������(����bUP=True)��Ҳ���Դ�������(����bUP=False)
	try
	{
		bool bUP=0;   //д���ֵķ���:From Down to UP,bUP=true
		bool bATTBEGIN=0;
		bool bGridLine=0;
		CString BlkName;
		CString tmpStr;
		CString tmp;
		CString SQLx;
		CString strTableName;
		CString strWorksheet;
		CString strTableCaption;
		CString ColTxt, ColFormat;
		CString strSheet,strCorpCell,strPrjCell,strVlmCell,strDateCell,strZYCell,strJDCell,strInsCell;
		CADatt *BlkAtt=NULL; long blkAttCount=0;
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		CMObject WorkBook;
		long RowsPerPage=0;
		long iNPage=0;
		long NPage=0;    //��nҳ
		long i=0, j=0, k=0;
		long SPage=0;    //��ҳ��
		long  StartNPage=0;  //ͼ�����Ѿ����ڵı����,��ѯuseri1�õ�
		long VerticalPages=0; //��ֱ�����ҳ��
		float sumWeight=0;    //�������
		float ColW=0, ColTxtHeight=0;
		float sumColW=0;
		float sngRowHeight=0, W0=0 , H0=0 ;
		float  DistX=0 , DistY=0;
				
		DistX = 20;
		DistY = 20;
		
		SQLx = _T("SELECT * FROM TableINFO WHERE ID=") + ltos(BillID);
// 		rs.m_pDatabase=&EDIBgbl::dbTable;//20071109 "dbSORT" ��Ϊ "dbTable"
// 		rs.Open(dbOpenSnapshot,SQLx);
		rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbTable,adOpenForwardOnly,adLockReadOnly,adCmdText);
		COleVariant vTmp;
		
		rs->get_Collect((_variant_t)_T("TableName"),&vTmp);
		strTableName = vtos(vTmp);
	
		rs->get_Collect((_variant_t)_T("CADtitleBlock"),&vTmp);
		strWorksheet = vtos(vTmp);      
	
		rs->get_Collect((_variant_t)_T("TableCaption"),&vTmp);
		strTableCaption=vtos(vTmp);   	
		rs->Close();
	
		SQLx = _T("SELECT * FROM [") + strTableName + _T("]");
// 		rs.Open(dbOpenSnapshot,SQLx );
		rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbTable,adOpenForwardOnly,adLockReadOnly,adCmdText);
	
		//��ÿ�����ֵ
		blkAttCount=GetTableHeaderBlockAttributes(rs, bATTBEGIN, NPage, iNPage, SPage, BlkAtt);
		rs->Close();
		
		//modified by fln
		CString strNFileName;  //����ģ��(��װ����ģ��ĸ���)
		CString strDFileName;  //Excel����ļ�
		CString strFileName;   //��װ���µ�ģ��
        CString strDefaultPath;

		strDefaultPath = basDirectory::ProjectDir;  //Excel����ļ�·��
        strNFileName = basDirectory::ProjectDir + _T("List.xlt");
		strDFileName = basDirectory::ProjectDir + _T("List") + EDIBgbl::SelPrjID + _T(".xls");
		strFileName = basDirectory::TemplateDir + _T("List.xlt"); 
	
		//����һ��ģ�忽��������,�����޸�
		if( !FileExists(strNFileName) )
		{
			if( !FileExists(strFileName) )
			{
				AfxMessageBox("ϵͳ�ļ���,�����°�װautoPHS!");
				return;
			}
			else
			{
				::CopyFile(strFileName, strNFileName, FALSE);
			}
		}
	
		//����Excel
		Worksheets objWorksheets;  //������
		try
		{
			StartExcelAndLoadTemplate(strNFileName,strDFileName,objWorksheets);
		}
		catch(CException *e)
		{
			AfxMessageBox("���Ѿ����˸��ļ�����رպ������и����");
			return;
		}
	
		_Worksheet Worksheet;    //������
		try
		{
			Worksheet.m_lpDispatch=objWorksheets.GetItem(_variant_t(strWorksheet));
			Worksheet.SetName(strTableCaption);
		}
		catch(COleDispatchException * e)
		{
			//e->ReportError();
			e->Delete();
			Worksheet.m_lpDispatch=objWorksheets.GetItem(_variant_t(strTableCaption));
		}
		Worksheet.Activate();
        //�޸���� fln

		CString strCell1,strCell2;
		_variant_t vEmp;
		Range objTmp;  
		Range objRange;
		objTmp = Worksheet.GetRange(_variant_t(_T("A1")),_variant_t(_T("Z500")));
		for(i=0;i<blkAttCount;i++)
		{			
			objRange = objTmp.Find(_variant_t(BlkAtt[i].Name),vtMissing,
				_variant_t((long)xlValues),_variant_t((long)xlWhole),_variant_t((long)xlByRows),
				(long)xlNext,vtMissing,vtMissing);
			objRange.SetValue(_variant_t(BlkAtt[i].Text));						
		}
		if(BlkAtt)
		{
			delete [] BlkAtt;
			BlkAtt=NULL;
		}
		
		objRange = objTmp.Find(_variant_t(_T("BEGIN")),vtMissing,
			_variant_t((long)xlValues),_variant_t((long)xlWhole),_variant_t((long)xlByRows),
			(long)xlNext,vtMissing,vtMissing);
				
		CString strTopTitleRows;
		strTopTitleRows.Format(_T("$%d:$%d"),1,objRange.GetRow()-1);
		PageSetup objPageSetup;
		objPageSetup.m_lpDispatch=Worksheet.GetPageSetup();
		objPageSetup.SetPrintTitleRows(strTopTitleRows);
		//	objPageSetup.SetLeftMargin(0.0);
		//	objPageSetup.SetRightMargin(0.0);
		strInsCell.Format(_T("%c%d"),(char)(objRange.GetColumn()-1) +_T('A'),objRange.GetRow());
		//���������������ÿҳ����,�������趨
		SQLx = _T("SELECT * FROM [") + strTableName + _T("] WHERE NOT ISNULL(CADFieldSeq) ORDER BY CADFieldSeq");
// 		rs.Open(dbOpenSnapshot,SQLx );
		rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbTable,adOpenForwardOnly,adLockReadOnly,adCmdText);
		rs->MoveLast();
		//��ʼ���л����
		int iRow=0,iCol1=0,iCol2=0;
		bool bpageSetup=false;
		int colNum=0;
		while(!rsText->adoEOF)
		{
			//��ÿһҳ
			rs->MoveFirst();
			//rs�к����ֶμ����ʽ��Ϣ
			iCol1=0;iCol2=0;
			for( i = 0 ;i<rs->RecordCount;i++)
			{
				rs->get_Collect((_variant_t)_T("FieldName"),&vTmp);
				ColTxt=vtos(vTmp);
				rs->get_Collect((_variant_t)_T("ExcelColNum"),&vTmp);
				colNum+=vtoi(vTmp);
				iCol2=(vtoi(vTmp)-1)+iCol1;
				if(ColTxt!=_T("") && iCol2>=iCol1)
				{
					try
					{
						rsText->get_Collect((_variant_t)ColTxt,&vTmp);  //ԭ������Ķ�ȡ����
						
						//��"�û��ֹ������ͼ��"��ѡ��ѡ�� �� ��ǰ���ݿ��ȡ���ֶ�ΪDrawNo
						//���ȡALLPRJDB.MDB�е�ZA����ֶ�ΪCustomDwgName��ֵ��
						//�޸��ߣ�YWH
						CString str = ColTxt;
						str.MakeUpper();
						if( EDIBAcad::g_bCustomPicNo && str == _T("DRAWNO") )//���Ϊ�û��ֹ������ͼ��
						{
							long m_volumeID,m_zdjh;
							COleVariant var;
							
							rsText->get_Collect((_variant_t)_T("VolumeID"),&var);
							m_volumeID = vtoi(var);
							
							rsText->get_Collect((_variant_t)_T("ZDJH"),&var);
							m_zdjh = vtoi(var);
							
							_RecordsetPtr rs1;
							rs1.CreateInstance(__uuidof(Recordset));
							CString SQLstr;
							SQLstr.Format("SELECT * FROM ZA WHERE VolumeID = %d AND ZDJH = %d ",m_volumeID,m_zdjh);
							
// 							rs1.m_pDatabase=&EDIBgbl::dbPRJDB;
// 							rs1.Open(dbOpenSnapshot,SQLstr);
							rs1->Open(_variant_t(SQLstr),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
							
							if(!rs1->adoEOF && !rs1->BOF)
							{
								rs1->get_Collect((_variant_t)_T("CustomDwgName"),&vTmp);
							}
							rs1->Close();
							
						}
						
						//�޸����  BY YWH
					}
					catch(CException *e)
					{
						e->Delete();
					}
					CString strVal=vtos(vTmp);
					strCell1=GetCell(strInsCell,iRow,iCol1);
					strCell2=GetCell(strInsCell,iRow,iCol2);
					objRange = Worksheet.GetRange(_variant_t(strCell1),_variant_t(strCell2));
					//objRange=Worksheet.Invoke(_T("GetRange"),2,&_variant_t(strCell),&_variant_t(strCell));
					//objRange.PutPropertyByName(_T("Value"),&_variant_t(strVal));
					if(iCol1!=iCol2) objRange.Merge(_variant_t());
					objRange.SetValue(_variant_t(strVal));
				}
				rs->MoveNext();
				iCol1=iCol2+1;
			}
			if(!bpageSetup)
			{
				bpageSetup=true;
				CString strPrintArea;
				strPrintArea.Format(_T("%c:%c"),strInsCell[0],strInsCell[0]+colNum-1);
				objPageSetup.SetPrintArea(strPrintArea);
				colNum=0;
			}
			rsText->MoveNext();
			iRow++;
		}
		
        SaveAndGetControl(objWorksheets,strDefaultPath);

		//�رռ�¼��
		rs->Close();
		rsText->Close();		
   }
   catch(COleException * e)
   {
	   e->ReportError();
	   e->Delete();
   }
   catch(COleDispatchException * e)
   {
	   //e->ReportError();
	   e->Delete();
   }  
   catch(CException *e)
   {
	   e->ReportError();
	   e->Delete();
   }
}


//����Excel�ͼ���ģ��
bool StartExcelAndLoadTemplate(const CString &strSFileName,const CString &strDFileName,
							                                    Worksheets &objWorksheets)
{
	COleVariant covOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);
    CMObject	ObjExcelApp;   //ExcelӦ�ó��������
	CMObject ExcWorkBook;      //Excel������������
	char szTempDirect[MAX_PATH];
	CString strTempPath;
	CString strFileName;
	int nFind;

	::GetTempPath(MAX_PATH,szTempDirect);

	strFileName = strDFileName;
	strFileName.MakeReverse();
	nFind=strFileName.Find("\\",0);
	strFileName = strFileName.Mid(0,nFind);
	strFileName.MakeReverse();
	strTempPath = (CString)szTempDirect + strFileName;

	//����Excel
	if(FAILED(ObjExcelApp.GetActiveObject(_T("Excel.application"))))		
	{
		if( FAILED(ObjExcelApp.CreateObject(_T("Excel.application"))) )
		{
			AfxMessageBox("��������Excel��û�а�װExcel");
			return false;
		}
	}
	
	if(!FileExists(strSFileName))
	{
		AfxMessageBox("ϵͳ�ļ��𻵣������°�װAuto PD!");
		return false;
	}
	else
	{
		//ɾ���Ѿ��е��ļ�
		if(::FileExists(strDFileName))
		{
			::DeleteFile(strDFileName);
		}
		if(::FileExists(strTempPath))
		{
			::DeleteFile(strTempPath);
		}
		
		//����ģ���ļ�
		::CopyFile(strSFileName,strTempPath,FALSE);
		::CopyFile(strSFileName,strDFileName,FALSE);
	}

	//��ù�����������
	ExcWorkBook=ObjExcelApp.GetPropertyByName(_T("Workbooks")).pdispVal;

	//����ģ��
	CMObject WorkBook;
//    WorkBook=ExcWorkBook.Invoke(_T("Open"),1,&_variant_t(strTempPath));
    WorkBook=ExcWorkBook.Invoke(_T("Open"),1,&_variant_t(strDFileName)).pdispVal;
	objWorksheets.m_lpDispatch=(IDispatch*)WorkBook.GetPropertyByName(_T("Sheets"));

	//����Excel�ɼ� 
	ObjExcelApp.PutPropertyByName(_T("Visible"),&_variant_t(true));

	return true;
}


//�����ļ��͸����û�����
void SaveAndGetControl(Worksheets objWorksheets,const CString &strDefaultPath)
{
	_Application Application;
	_Workbook workbook;
    Application = objWorksheets.GetApplication();
	workbook=Application.GetActiveWorkbook();
	Application.SetDefaultFilePath(strDefaultPath);
	workbook.Save(); 

	Application.SetUserControl(true);
}


void EDIBAcad::GetAcadImage()
{
}

bool EDIBAcad::InsertPhsBLK(CCadPoint& InsPnt, CString  blk, double  xs, double  ys, double  rotA, int  iGetInsPointMODE)
{
try
{
	CCadPoint pnt;
	CString BlkName;
   //Dim pnt As Variant, BlkName As String
   //Dim tmpMin As Variant, tmpMax As Variant
	CMObject tmpObj;
	_variant_t tmpVar;
	tmpVar.ChangeType(VT_NULL);
   if(iGetInsPointMODE == iGIPmanual)
   {
      SetAcadTop();
		CCadPoint pt0;
	  tmpObj=objAcadDoc.GetPropertyByName(_T("Utility")).pdispVal;
      pnt =tmpObj.Invoke(_T("GetPoint"),2,(LPVARIANT)pt0, &_variant_t(_T("From AutoCAD enter a point: ")));
      InsPnt=pnt;
   }
   CMObject objBlk;
   bool bFoundBlk=0;
   long i=0;
   //�����Ƿ���ڣ��ٶȽ��������ǿɱ���ACAD��������
   tmpObj=objAcadDoc.GetPropertyByName(_T("Blocks")).pdispVal;
	//long blkCount=(long)tmpObj.GetPropertyByName(_T("Count"));
   bFoundBlk = false;
	blk.MakeUpper();
	objBlk=tmpObj.Invoke(_T("Item"),1,&_variant_t(blk)).pdispVal;
	BlkName =vtos(objBlk.GetPropertyByName(_T("Name")));
	BlkName.MakeUpper();
	if(BlkName==blk)
		bFoundBlk=true;

	/*for(i=0;i<blkCount;i++)
	{
		tmpObj=tmpObj.Invoke(_T("Item"),1,&_variant_t(i));
		BlkName =(char*)_bstr_t(tmpObj.GetPropertyByName(_T("Name")));
		BlkName.MakeUpper();
		if(BlkName==blk)
		{
			bFoundBlk=true;
			break;
		}
	}*/
   if( gbACADR14)
   {
      if( bFoundBlk)
	  {
		  AcObj = MoSpace.Invoke(_T("InsertBlock"),5,(LPVARIANT)InsPnt,&_variant_t( blk ),&_variant_t( xs), &_variant_t(ys), &_variant_t(rotA)).pdispVal;
         GetBoundingBox(AcObj,MinPoint,MaxPoint);
        // MsgBox MinPoint(0)
        // MsgBox MinPoint(1)
         //MsgBox MaxPoint(0)
         //MsgBox MaxPoint(1)
         OrdinateBound();
	  }
      else
         throw GetResStr(IDS_NoThisBlockInphsDotDwg)+_T(":\"")+blk+_T("\"");
   }
   else
   {
      if( !bFoundBlk)
	  {
//		 AcObj = MoSpace.Invoke(_T("InsertBlock"),6,(LPVARIANT)InsPnt,&_variant_t(basDirectory::PhsBlkDir + blk  + ".dwg"),&_variant_t( xs), &_variant_t(ys), &_variant_t((double)1),&_variant_t(rotA));
	     AcObj = MoSpace.Invoke(_T("InsertBlock"),6,(LPVARIANT)InsPnt,&_variant_t( basDirectory::PhsBlkDir + blk+_T(".dwg") ),&_variant_t( xs), &_variant_t(ys), &_variant_t((double)1),&_variant_t(rotA)).pdispVal;

		 GetBoundingBox(AcObj,MinPoint,MaxPoint);
        // MsgBox MinPoint(0)
        // MsgBox MinPoint(1)
         //MsgBox MaxPoint(0)
         //MsgBox MaxPoint(1)
         OrdinateBound();
	  }
      else
	  {
       //  throw GetResStr(IDS_NoThisBlockInphsDotDwg)+_T(":\"")+blk+_T("\"");
        AcObj = MoSpace.Invoke(_T("InsertBlock"),6,(LPVARIANT)InsPnt,&_variant_t( blk ),&_variant_t( xs), &_variant_t(ys), &_variant_t((double)1),&_variant_t(rotA)).pdispVal;
         GetBoundingBox(AcObj,MinPoint,MaxPoint);
        // MsgBox MinPoint(0)
        // MsgBox MinPoint(1)
         //MsgBox MaxPoint(0)
         //MsgBox MaxPoint(1)
         OrdinateBound();
	  }
	}
   SetAcadNoTop();
   return true;
}
catch(CString e)
{
	//ShowMessage(e);
	return false;
}
catch(_com_error e)
{
	ShowMessage(e.Description());
	return false;
}
catch(CException *e)
{
	e->Delete();
	return false;
}
}

void EDIBAcad::AddData2rsTZG(_RecordsetPtr rs, long  iRecNo, long  zdjh, CString  FD, CString  sView, CString sBlkName, COleSafeArray& InsPnt)
{
	try
	{
      rs->AddNew();
      rs->put_Collect((_variant_t)_T("VolumeID"),COleVariant(EDIBgbl::SelVlmID));
      rs->put_Collect((_variant_t)_T("recno"),COleVariant(iRecNo));
      rs->put_Collect((_variant_t)_T("zdjh"),COleVariant(zdjh));
		CString sTmp;
		sTmp=vtos(AcObj.GetPropertyByName(_T("Handle")));
		if(sTmp!=_T(""))
			rs->put_Collect((_variant_t)_T("Handle"),STR_VAR(sTmp));
      rs->put_Collect((_variant_t)_T("EntityName"),STR_VAR(vtos(AcObj.GetPropertyByName(_T("EntityName")))));
      rs->put_Collect((_variant_t)_T("Layer"),STR_VAR(vtos(AcObj.GetPropertyByName(_T("Layer")))));
		long ix=0;
		double va;
		InsPnt.GetElement(&ix,&va);
      rs->put_Collect((_variant_t)_T("x"),COleVariant(va));
		ix=1;
		InsPnt.GetElement(&ix,&va);
      rs->put_Collect((_variant_t)_T("y"),COleVariant(va));
		ix=2;
		InsPnt.GetElement(&ix,&va);
      rs->put_Collect((_variant_t)_T("z"),COleVariant(va));
      rs->put_Collect((_variant_t)_T("FieldName"),STR_VAR(FD));
      rs->put_Collect((_variant_t)_T("View"),STR_VAR(sView));
      rs->put_Collect((_variant_t)_T("BlkName"),STR_VAR(sBlkName));
	  rs->put_Collect((_variant_t)_T("nth"),(_variant_t)1L);//��׼֧����·��=1�������Գ�˫���͹��ø���˫��
      rs->Update();
	}
	catch(COleException *e)
	{
		e->ReportError();
		e->Delete();
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void EDIBAcad::DrawPhsAssemble(_RecordsetPtr rsRefZB, long iView)
{
	//Ŀ�ģ�����֧������װͼ��
	//���룺rstzb,��ͼ������������
	//����˼·���Ե���Ϊ��������֧����������3��,��X2Z,XZ2,XZ
	
	//  First:X2Z         Second:XZ2        Third:XZ
	//  |  |     |        |     |  |        |     |
	//  |  |     |        |     |  |        |     |
	//  |  |     |        |     |  |        |     |
	//  1  2     3        1     2  3        1     2
	//    x      z        x       z         x     z
	//ÿ�ζ��������·����������������Ż����ڵ�1·��
	//�ߴ��ע�ڵڶ�·�������ɼ��ٳ���ά����������
	//���浱ǰ֧������Ͻṹ���ݣ��������������ڻ�ͼ��
	CString *Ptype=NULL;  //��¼֧���ܽṹ���������������
	CString mstrCurrentPart;    //��ǰ���ID
	CString mstrPAID;	//�ܲ�ID�����ڱ�ע�������
	long  *mlPartClassID=NULL;    //��¼����������
	long *mlPartIndex=NULL;     //��¼��������
	long lngLastPartNo=0;   //��¼����֧���ܵ������������������

	long  dimDist = 10;
	try
	{
		float tmpLugInsLen=0 ; //���˲������Ӽ��Ļ��Ƴ���
		tmpLugInsLen = 1.5 ;  //����1.5�ᷢ������ճ��
		CCadPoint tmpSApnt;
		CCadPoint  tmpLUGpnt;
		CCadPoint  tmpH1;
		
		CCadPoint  tmpL1;
		CCadPoint  tmpL2;
		double tmpdbl;
		float tmpLUGyOffset=0;
		double tmpHeight=0;
		double tmpLowPoint=0;
		double tmpGBwidth=0;
		double tmpGBJJwidth=0;
		double tmpLeft=0 , tmpRight=0 , tmpHigh=0 , tmpLow=0 ;
		
		bool bPHDorPHD1=false;//��������PHD��PHD1֮���Ǹ���
		float sngTmp=0;  //PHD��PHD1����˨ƫ����
		sngTmp = 3.5;
		
		CString blkID , sVX , sVZ , tbn1 ;
		CString sVfx , sVfx1 , sBlk;
		float rotA=0 , xScal=0 ;
		CString Xblk , Zblk ;
		CCadPoint InsPnt, SeqPnt, tmpInsPnt;
		COleVariant vTmp,vTmp1,vTmp2;
		float xPos=0 , yPos=0;
		float sngCrd0=0, sngCrd=0;
		float sngCrdSEQ0=0, sngCrdSEQ=0;
		CString SQLx;
		CCadPoint p1, p2, p3;
		//h1,h2,h3���ڱ�������ߴ��עʱ����������
		CCadPoint h1, h2, h3;
		CCadPoint InsPnt1, pp1, pp2;
		long lngSumLayoutColumn=0;   //��ͼ���ܵĲ���·�������Ϊ3��
		long lngCurrentLayoutColumn=0;   //��ͼ�е�ǰ���ڻ��Ƶ�·����Ϊ��ţ�<=��·������
		long lngSEQColumn=0;  //��ͼ�б�����ڵ��У�Ĭ��Ϊ1��<=��·������
		long lngDIMColumn=0;  //��ͼ�гߴ����ڵ��У�Ĭ��Ϊ2��<=��·������
		lngSEQColumn = 1;
		lngDIMColumn = 3;
		long i =0, k=0;
		bool bTmp=false , bHighPoint=false, bLowPoint=false , bDJ=false;
		//CADatt *BlkAtt=NULL;
		long lngCurrentPartNo=0 ;  //��¼��ǰ��������������������
		long lngAttachedPartNo=0 ;   //��¼��ǰ�����ǰһ�����֮��ĸ�����
		float mvMaxLugLen=0, mvMaxPartRatioOfHeight=0;    //������Ƹ߶���ʵ�ʸ߶ȵı�
		float mvMaxLugRatioOfHeight=0;  //���˻��Ƹ߶���ʵ�ʸ߶ȵı�
		long mlRot0=0, mlRot=0 ; //�����ת����

		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rsTZG;
		rsTZG.CreateInstance(__uuidof(Recordset));
		float lngMaxLUGDrawingLength=0;    //������˻��Ƴ���,����ֱ��ʹ��pline�߻�������ʱ��
		float lngLUGDrawingLength=0;    //�������˻��Ƴ���,����ֱ��ʹ��pline�߻�������ʱ��
		float sngYoffsetOfAttached=0;  //��������ĸ/����ĸY������Ƽ�ࣩ
		float DblHangerOffset=0;       //˫����ͼƫ��ֵ
		float HeightOffset=0;          //������λ�ߴ�Y����ƫ��ֵ
		sngYoffsetOfAttached = 1; //0.5 //1.2 //��������ĸ/����ĸY������Ƽ�ࣩ
		DblHangerOffset = 18;    //˫����ͼƫ��ֵ

//		HeightOffset = 5;        //������λ�ߴ�Y����ƫ��ֵ
		
		tmpLUGyOffset = 6;
		//tmpGBwidth = 1.5;�Ѿ������sor.mdb��PhsBlkDimPos�е�xC�ֶΡ�
		MakeNewLayer(_T("DIM"));
		modPHScal::gsPhsColor.MakeUpper();
		if(modPHScal::gsPhsColor==_T("WHITE"))
		{
			MakeNewLayer(_T("Phs"), acWhite);
		}
		else
		{
			if(modPHScal::gsPhsColor== _T("GREEN") )
			{
				MakeNewLayer(_T("phs"), acGreen);
			}
			else
			{
				MakeNewLayer(_T("phs"));
			}
		}
		SetDimVariables();
		bHighPoint = false;
		bLowPoint = false;
		
		objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("USERI5")),&_variant_t((short)modPHScal::zdjh));
		//ɾ��TZG�����ֶ�zdjh=Zdjh�ļ�¼
		
		CString strSql = CString( _T("DELETE * FROM [") )+ EDIBgbl::Btype[EDIBgbl::TZG] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND ZDJH=") +ltos(modPHScal::zdjh);
		EDIBgbl::dbPRJDB->Execute((_bstr_t)strSql, NULL, adCmdText); // iDelZdjh
		modPHScal::iSelSampleID = vtoi(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("iSelSampleID")));
		modPHScal::giNumRod = Cavphs->GetPhsOneClassPartNumAndPartInfo(iROD, modPHScal::iSelSampleID, Ptype, mlPartClassID, mlPartIndex,lngLastPartNo);
		//���A4ͼ�����ϸ�������λ��
		long rc =0;
// 		rs.m_pDatabase=&EDIBgbl::dbPRJ;
// 		rs.Open(dbOpenSnapshot,_T("SELECT count(*) AS rc FROM tmp2"));
		strSql = _T("SELECT count(*) AS rc FROM tmp2");
		rs->Open(_variant_t(strSql),(IDispatch*)EDIBgbl::dbPRJ,adOpenForwardOnly,adLockReadOnly,adCmdText);
		if( rs->BOF || rs->adoEOF )
		{
			rc = 0;
		}
		else
		{
			rs->get_Collect((_variant_t)_T("rc"),&vTmp);
			rc =vtoi(vTmp);
		}
		rs->Close();
		Bound[0]=25.0;
		//Bound[1]=(modPHScal::Ax == _T("A3") ? (5.0+30.0) : 74.0 + 7.0 * rc);//pfg20050922ԭ����
		Bound[1]=(modPHScal::Ax == _T("A3") ? (5.0+(110-EDIBgbl::MaxLGLong)) : 74.0 + 7.0 * rc);//pfg20050922
		Bound[2]=(modPHScal::Ax == _T("A3") ? 420 - 5 - 180 : 210 - 5);
		//Bound[3]=(modPHScal::Ax == _T("A3") ? 297 - 5 - (EDIBgbl::gbTbOS ? 18 : 0) : 211);//pfg20050922ԭ����
		Bound[3]=(modPHScal::Ax == _T("A3") ? 297 - 5-30+(110-EDIBgbl::MaxLGLong)- (EDIBgbl::gbTbOS ? 18 : 0) : 211);//pfg20050922
		//���·���������ͼ��λֵ
		//if( Ax = _T("A4") ){
		//5=����ͼ�ܲ���ͼ��ߵľ���,20=��ͼ����ͼ����ͼ��߿�֮��ľ���
		//modPHScal::VX_pt0x = Bound(0) + 5 + (Bound(2) - Bound(0) - 20) / 4
		// modPHScal::VX_pt0y = Bound(1) + 5 + (Bound(3) - Bound(1) - 20) / 4
		//modPHScal::VZ_pt0x = Bound(0) + 15 + (Bound(2) - Bound(0) - 20) / 2
		//}
		tmpBound[0]=Bound[2]/2;
		tmpBound[1]=Bound[3] / 2;
		tmpBound[2]=Bound[2] / 2;
		tmpBound[3]=Bound[3] / 2;
		
		long OldgiAttDOffsetX=0 ; //����ǰ����ͼ�����ӵ�ƫ��ֵ
		//���x/z��λֵΪ��ֵ����λ�ߴ��߻��ڷ�����
		if( vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dxa1"))) < 0 )
			modPHScal::giAttDxOffsetX = -modPHScal::giAttDxOffsetX;
		if( vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dza1"))) < 0 )
			modPHScal::giAttDzOffsetX = -modPHScal::giAttDzOffsetX;
		
		//�������˻��Ƴ���
		SQLx = _T("SELECT sum(Crd) as sumCrd FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 AND NOT IsNull(SEQ)");// ORDER BY SEQ";
// 		rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 		rs.m_strFilter=_T("");
// 		rs.m_strSort=_T("");
// 		rs.Open(dbOpenSnapshot,SQLx );
		rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
		rs->get_Collect((_variant_t)_T("sumCrd"),&vTmp);
		if(lngMaxLUGDrawingLength > vtof(vTmp))
			lngMaxLUGDrawingLength = 260 -30 - vtof(vTmp);	
		else
			//lngMaxLUGDrawingLength=110-30;//ԭ����pfg20050922
			lngMaxLUGDrawingLength=EDIBgbl::MaxLGLong;//pfg20050922
		rs->Close();
		//��С���˻��Ƴ���
		if(modPHScal::giNumRod!=0)
			lngLUGDrawingLength = lngMaxLUGDrawingLength / modPHScal::giNumRod;
		//Э����������������Ļ��Ʊ���
		mvMaxPartRatioOfHeight = 0;
		mvMaxLugRatioOfHeight = 0;
		mvMaxLugLen = 0;
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 AND NOT IsNull(SEQ) ORDER BY SEQ");
// 		rs.Open(dbOpenSnapshot,SQLx );
		rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
		if(!rs->BOF && !rs->adoEOF)
		{
			rs->get_Collect((_variant_t)_T("crd"),&vTmp);
			sngCrd0 = fabs(vtof(vTmp));
			while(!rs->adoEOF)
			{
				rs->get_Collect((_variant_t)_T("sizeDIM"),&vTmp);
				if(vTmp.vt!=VT_NULL)
				{
					if( vtof(vTmp)!=0)
					{
						rs->get_Collect((_variant_t)_T("crd"),&vTmp);
						if( vTmp.vt!=VT_NULL)
						{
							//if( rs.Fields(_T("crd")) <> 0 ){
							rs->get_Collect((_variant_t)_T("ClassID"),&vTmp);
							if(vtoi(vTmp)!= iROD && vtoi(vTmp) != iConnected )
							{						
								rs->get_Collect((_variant_t)_T("crd"),&vTmp);
								rs->get_Collect((_variant_t)_T("sizeDIM"),&vTmp1);
								if( mvMaxPartRatioOfHeight < fabs(vtof(vTmp) / vtof(vTmp1)) )
								{
									mvMaxPartRatioOfHeight = fabs(vtof(vTmp)/vtof(vTmp1));
								}
							}
							else if( vtoi(vTmp) == iROD )
							{
								rs->get_Collect((_variant_t)_T("crd"),&vTmp);
								rs->get_Collect((_variant_t)_T("sizeDIM"),&vTmp1);
								if( mvMaxLugLen < fabs(vtof(vTmp1)) )
									mvMaxLugLen = fabs(vtof(vTmp1));
								if( mvMaxLugRatioOfHeight < fabs(lngLUGDrawingLength / vtof(vTmp1)) )
								{
									mvMaxLugRatioOfHeight = fabs(lngLUGDrawingLength / vtof(vTmp1));
								}
							}
							//}
						}
					}
				}
				rs->MoveNext();
			}
			rs->Close();
			
			if( mvMaxPartRatioOfHeight > mvMaxLugRatioOfHeight )
			{
				//���ݹܲ��߶ȵ������˻��Ƴ���
				if( mvMaxPartRatioOfHeight * mvMaxLugLen > lngMaxLUGDrawingLength / modPHScal::giNumRod )
				{
					//�������������˻���̫��
					if( fabs(sngCrd0) > lngMaxLUGDrawingLength / modPHScal::giNumRod )
					{
						//����ܲ����Ƹ߶�̫��
						lngLUGDrawingLength = fabs(sngCrd0) + 2;
					}
					else
					{
						//����ܲ����Ƹ߶Ȳ�̫��
						//û�취������ʹ�ò�Э���ı�����������
					}
				}
				else
				{
					//�������������˻��Ʋ�̫��
					lngLUGDrawingLength = mvMaxPartRatioOfHeight * mvMaxLugLen;
				}
			}
		}
		
		
		//ѡ���¼��rsTZG�Ա��¼ͼ������
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZG] + _T("] WHERE zdjh=") +ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID);
// 		rsTZG.m_pDatabase=&EDIBgbl::dbPRJDB;
// 		rsTZG.Open(dbOpenDynaset,SQLx );
		rsTZG->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
		//ѡ���¼��rs�Ա����ͼ������
		
		//**************************************************************************************//
		SQLx = _T("SELECT * FROM phsBlkDimPos");
		if(rs->State == adStateOpen)
			rs->Close();
// 		rs.m_pDatabase=&EDIBgbl::dbPHScode;//"dbSORT" ��Ϊ "dbPHScode"
// 		rs.Open(dbOpenSnapshot,SQLx );
		rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPHScode,adOpenForwardOnly,adLockReadOnly,adCmdText);
		float xGDW1=0,xSAxa=0,xL1=0;
		bool bxSAxa_Null=false;
		bool brsFind=true;
		if( rs->adoEOF && rs->BOF ) return;
		//MsgBox Zdjh
		
		EDIBAcad::GetActiveAcadDoc();
		SetACADCurrentStyle(_T("hz"));
		SetACADCurrentStyle(_T("standard"));
		objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("USER15")),&_variant_t((long)modPHScal::zdjh));
		//���ȶ�rsRefZB��¼����recno�ֶ��������򣬷�ֹ��������λ��ǰ�����³���blkID�Ҳ������������Ϊ����������blkID=Null����
		//�¾�û��Ч����
		//rsRefZB.Sort = _T("recno")
		CString tmpStr1,tmpStr2;
		if( rsRefZB->adoEOF || rsRefZB->BOF)
		{
			tmpStr1.Format(GetResStr(IDS_NoRecordInTZB),EDIBgbl::dbPRJDB->DefaultDatabase,  EDIBgbl::TBNSelPrjSpec + EDIBgbl::Btype[EDIBgbl::TZB],EDIBgbl::SelJcdm,ltos(modPHScal::zdjh));
			throw tmpStr1;
		}
		rsRefZB->MoveLast();
		rsRefZB->MoveFirst();
		//�ж��Ƿ�Ϊ����
		if( modPHScal::sngSEL > modPHScal::sngPEL )
		{
			bDJ = true;
		}
		else
		{
			bDJ = false;
		}
		int C=rsRefZB->RecordCount;
		
		//*********************************************************************************************
		for( i = 0 ;i<C;i++)
		{
			rsRefZB->get_Collect((_variant_t)_T("blkID"),&vTmp);
			if(vtos(vTmp)==_T(""))
			{
				//������ֵ
				tmpStr1.Format(GetResStr(IDS_NullXFieldValueInXtableInXmdb),EDIBgbl::dbPRJDB->DefaultDatabase,EDIBgbl::TBNSelPrjSpec+EDIBgbl::Btype[EDIBgbl::TZB],EDIBgbl::SelJcdm,ltos(modPHScal::zdjh),ltos(i+1),_T("blkID"));
				ShowMessage(tmpStr1);
			}
			else
			{
				//������ֵ
				blkID = vtos(vTmp);
				rsRefZB->get_Collect((_variant_t)_T("ID"),&vTmp);
				mstrCurrentPart =vtos(vTmp);
				rsRefZB->get_Collect((_variant_t)_T("CustomID"),&vTmp);
				tbn1 = modPHScal::sFindTBN(vtos(vTmp));
				//*************************************************
//				brsFind=rs.FindFirst(_T("Trim(BlkID)=\'")+blkID+_T("\'"));
				_variant_t vTmp;
				SQLx = _T("Trim(BlkID)=\'")+blkID+_T("\'");
				rs->Find((_bstr_t)SQLx, 0, adSearchForward, vTmp);

				//���˫�۸ּ��ֵxC
				rs->get_Collect((_variant_t)_T("xC"),&vTmp);
				tmpGBwidth=vtof(vTmp);
				rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
				xGDW1=vtof(vTmp);
				rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
				bxSAxa_Null=false;
				if(vTmp.vt==VT_NULL)
					bxSAxa_Null=true;
				xSAxa=vtof(vTmp);
				rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
				xL1=vtof(vTmp);
				if( iView == iViewXZ )
				{
					//X����ͼ
					if( i == 0 )
					{
						//�ܲ�
						mstrPAID=mstrCurrentPart;
						rsRefZB->get_Collect((_variant_t)_T("crd"),&vTmp);
						sngCrd =vtof(vTmp);
						sngCrd0 = sngCrd;
						//����ͼ�ܲ�����
						InsPnt.SetPoint(modPHScal::VX_pt0x,modPHScal::VX_pt0y);
						//��ע˫�������ľ�ߴ�
						if( modPHScal::gintParallelNum == 2 )
						{
							//˫֧��
							lngSumLayoutColumn = 3;
							if( modPHScal::df == _T("Z") )
							{
								//����ͼ��·
								//��ֱ�ߴ���
								if( lngDIMColumn == 1 ) lngDIMColumn = 3;
								if( lngDIMColumn == 2 )
									//insPnt1(0)����ߴ��ߵĻ���
									InsPnt1.SetX(modPHScal::VX_pt0x + modPHScal::giDimOffset + DblHangerOffset);
								else
									InsPnt1.SetX(modPHScal::VZ_pt0x + modPHScal::giDimOffset);
								tmpdbl=modPHScal::VX_pt0x - DblHangerOffset;
								p1.SetX(tmpdbl);
								tmpdbl=modPHScal::VX_pt0y - 10.0;
								p1.SetY(tmpdbl);
								p2.SetPoint((float)(modPHScal::VX_pt0x + DblHangerOffset),(float)p1[1],(float)0.0);
							}
							else
							{
								//����ͼһ·
								//��������³ߴ粻Ҫ��ע�ڵڶ���
								if( lngDIMColumn == 2 ) lngDIMColumn = 3;
								if( lngDIMColumn == 1 )
									InsPnt1.SetX(modPHScal::VX_pt0x + modPHScal::giDimOffset);
								else
									InsPnt1.SetX(modPHScal::VZ_pt0x + modPHScal::giDimOffset + DblHangerOffset);
								p1.SetPoint((double)(modPHScal::VZ_pt0x - DblHangerOffset),(double)(modPHScal::VX_pt0y - 10.0));
								p2.SetPoint((double)(modPHScal::VZ_pt0x + DblHangerOffset),(double)p1[1]);
							}
							p3.SetPoint(p1[0],p1[1] + 5.0);
							//��ע˫�����ľ�
							AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)p1, (LPVARIANT)p2, (LPVARIANT)p3).pdispVal;
							GetBoundingBox(AcObj,MinPoint,MaxPoint);
							OrdinateBound();
							rsRefZB->get_Collect((_variant_t)_T("sizeC"),&vTmp);
							if( vTmp.vt!=VT_NULL)
							{
								AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(vtos(vTmp)));
								GetBoundingBox(AcObj,MinPoint,MaxPoint);
								OrdinateBound();
							}
						}
						else
						{
							//��֧��
							lngSumLayoutColumn = 2;
							lngDIMColumn = 2;
							InsPnt1.SetX(modPHScal::VZ_pt0x + modPHScal::giDimOffset);
						}
						
						if( !brsFind)
						{
							//brsFind=false;
							tmpStr1.Format(GetResStr(IDS_NoThisBlockInphsBlkDimPos),blkID);
							ShowMessage(tmpStr1);
						}
						else
						{
							//����������
							//�����ͼ,�ܲ�
							//֧�ܣ���ʼ���귶Χ�����ڹܲ�����λ�á�
							rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
							xGDW1=vtof(vTmp);
							rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
							bxSAxa_Null=false;
							if(vTmp.vt==VT_NULL)
								bxSAxa_Null=true;
							xSAxa=vtof(vTmp);
							rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
							xL1=vtof(vTmp);
							tmpBound[0]=InsPnt[0];
							tmpBound[1]=InsPnt[1];
							tmpBound[2]=tmpBound[0];
							tmpBound[3]=InsPnt[1] + 4;
							sVX = _T("P") + modPHScal::df;
							rs->get_Collect((_variant_t)(sVX+_T("blk")),&vTmp);
							Xblk = vtos(vTmp);
							bTmp = InsertPhsBLK(InsPnt, Xblk, 1, 1, 0, iGIPAuto);
							GetBoundingBox(AcObj,MinPoint,MaxPoint);
							//AcObj.Invoke(_T("boundingbox"),MinPoint, MaxPoint);
							tmpBound[0]=MinPoint[0];
							tmpBound[1]=MinPoint[1];
							tmpBound[2]=MaxPoint[0];
							tmpBound[3]=MaxPoint[1];
							if( bTmp )
							{
								rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
								AddData2rsTZG(rsTZG, vtoi(vTmp), modPHScal::zdjh, _T(""), _T("X"), Xblk, InsPnt);
							}
							//�����ͼ,�ܲ��⾶�ߴ�
							DrawphsDimOfDW(InsPnt, AcObj);
							//�����ͼ,�ܲ����
							//�浽���
							//SeqPnt(0) = InsPnt(0) - 28 - IIf(gintParallelNum = 2 And df = _T("Z"), DblHangerOffset, 0)
							//�浽�Ҳ�
							CString strBG_blockName ;
							if( EDIBAcad::g_bBGForBJBW )
							{
								strBG_blockName = _T("BG_BW");
								SeqPnt.SetPoint(
									InsPnt[0] + (modPHScal::gintParallelNum == 2 && modPHScal::df == _T("Z") ? DblHangerOffset + 5 : 28),
									InsPnt[1],//+2.0
									InsPnt[2]);
							}
							else
							{
								strBG_blockName = _T("BG_D");
								SeqPnt.SetPoint(
									InsPnt[0] + (modPHScal::gintParallelNum == 2 && modPHScal::df == _T("Z") ? DblHangerOffset + 5 : 28),
									InsPnt[1],
									InsPnt[2]);
							}
					
							if( mlPartIndex[0] == iPA )
							{
								//��һ���ǹܲ�
								bTmp = InsertPhsBLK(SeqPnt, strBG_blockName, 1, 1, 0, iGIPAuto);
								if( EDIBAcad::g_bBGForBJBW )
								{
									SeqPnt.SetPoint(
										InsPnt[0] + (modPHScal::gintParallelNum == 2 && modPHScal::df == _T("Z") ? DblHangerOffset + 5 : 28),
										InsPnt[1]+5.0,
										InsPnt[2]);
								}
								ModifyTextOfBG(SeqPnt, modPHScal::sngPEL,"PEL",EDIBAcad::g_bBGForBJBW,TRUE);
							}
							if( bTmp )  
							{
								rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
								AddData2rsTZG(rsTZG, vtoi(vTmp), modPHScal::zdjh, _T(""), _T("X"), Xblk, InsPnt);
							}
							//�����ͼ,�ܲ����
							rsRefZB->get_Collect((_variant_t)_T("blkID"),&vTmp);
							xPos = vtof(modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("x1"), vtos(vTmp)));
							yPos = vtof(modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("y1"), vtos(vTmp)));
							SeqPnt.SetPoint(
								InsPnt[0] + xPos,
								InsPnt[1] + yPos,
								InsPnt[2]);
							rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
							rsRefZB->get_Collect((_variant_t)_T("seq"),&vTmp2);
							DrawTagPoint(SeqPnt, vtoi(vTmp), vtoi(vTmp2));
							//���������ͼ���ӱ��
							tmpLowPoint = InsPnt[1];
							tmpHeight = InsPnt[1];
							tmpL1.SetX(InsPnt[0]);
							tmpL1.SetZ(InsPnt[2]);
							tmpH1.SetX(InsPnt[0]);
							tmpH1.SetZ(InsPnt[2]);
							// DrawphsZUZI[rsRefZB,InsPnt[], iViewX]
							//�Ҳ���ͼ,�ܲ�
							InsPnt.SetX(modPHScal::VZ_pt0x);
							tmpL2.SetX(InsPnt[0]);
							tmpL2.SetZ(InsPnt[2]);
							tmpInsPnt.SetX(InsPnt[0]);
							tmpInsPnt.SetZ(InsPnt[2]);
							sVZ = CString(_T("P")) + (modPHScal::df == _T("X") ? _T("Z") : _T("X"));
							rs->get_Collect((_variant_t)(sVZ+_T("blk")),&vTmp);
							Zblk = vtos(vTmp);
							rsRefZB->get_Collect((_variant_t)_T("SizeC"),&vTmp);
							tmpGBJJwidth = vtof(vTmp);
							InsertPhsBLK(InsPnt, Zblk, 1, 1, 0, iGIPAuto);
							//�Ҳ���ͼ,�ܲ��⾶�ߴ�
							DrawphsDimOfDW(InsPnt, AcObj);
							if( bTmp )
							{ 
								rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
								AddData2rsTZG(rsTZG, vtoi(vTmp), modPHScal::zdjh, _T(""), _T("Z"), Zblk, InsPnt);
							}
							//�����Ҳ���ͼ���ӱ��
							// DrawphsZUZI(rsRefZB,InsPnt(), iViewZ)
					}
					//��ע�ߴ�
					
					////add by ywh
					if(i==0)
					{
						InsPnt1.SetX(InsPnt1[0] - 10  /* +EDIBAcad::g_fDistanceOffset*/ );
					}
					
					////add by ywh
					if(modPHScal::sngSEL > modPHScal::sngPEL)
					{
						HeightOffset = 0;        //������λ�ߴ�Y����ƫ��ֵ
						dimDist = 10;
					}
					else
					{
						HeightOffset = 5;        //������λ�ߴ�Y����ƫ��ֵ
						dimDist = 0;
					}

					if( sngCrd0 != 0 && modPHScal::glClassID != iAttached && modPHScal::glClassID != iBolts && modPHScal::glClassID != iNuts )
					{
						p1.SetPoint((double)InsPnt1[0], (double)modPHScal::VX_pt0y,0.0);
						p2.SetPoint(p1[0],p1[1] + sngCrd,0);
						p3.SetPoint(p1[0] + (modPHScal::sngSEL > modPHScal::sngPEL ? 5 : 10) + HeightOffset + dimDist,p1[1] + sngCrd / 2);////֧�ܳߴ���Ҫ�ƿ�һЩ������ճ��
						AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)p1, (LPVARIANT)p2, (LPVARIANT)p3).pdispVal;
						GetBoundingBox(AcObj,MinPoint,MaxPoint);
						OrdinateBound();
						//Set AcObj = MoSpace.AddDimRotated(p1(), p2(), p3(), Atn(1) * 2)
						rsRefZB->get_Collect((_variant_t)_T("sizeDIM"),&vTmp);

						if( vTmp.vt==VT_NULL)
							ShowMessage(GetResStr(IDS_NullPartSizeInTZB));
						else
						{
							AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
							GetBoundingBox(AcObj,MinPoint,MaxPoint);
							OrdinateBound();
						}
						rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
						AddData2rsTZG(rsTZG, vtoi(vTmp), modPHScal::zdjh, _T("sizeDIM"), _T("X"), _T(""), p1);
						//����Ǻᵣ֮�࣬���ƿ��ߴ��߻���x���꣬��ֹ�����غϡ�
						if( modPHScal::gintParallelNum == 2 )
							InsPnt1.SetX(InsPnt1[0] + modPHScal::giDimOffset);
					}
					//��ǰ�����Ӧ����Ϊ1�ˡ���Ϊ�ܲ��Ѿ������ꡣ
					lngCurrentPartNo = 0;
					//��������Ϊ-1����Ϊ��һ������Ӧ����0��
					lngAttachedPartNo = -1;
				}
				else
				{
					//�������Ӽ������
					//��phsBlkDimPos��ȡ�����Ķ�λֵ
					if( modPHScal::glIDIndex == iSA)
					{
						rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
						xGDW1=vtof(vTmp);
						rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
						bxSAxa_Null=false;
						if(vTmp.vt==VT_NULL)
							bxSAxa_Null=true;
						xSAxa=vtof(vTmp);
						rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
						xL1=vtof(vTmp);
					}
					if( modPHScal::glClassID != iAttached && modPHScal::glClassID != iBolts && modPHScal::glClassID != iNuts )
					{
						//���Ǹ������������
						//������·�/�Ϸ�����֮�࣬���ƿ��ߴ��߻���x���꣬��ֹ�����غϡ�
						if( mstrCurrentPart == _T("T3A") || mstrCurrentPart == _T("T3") || mstrCurrentPart == _T("LHG") )
							InsPnt1.SetX(InsPnt1[0] + modPHScal::giDimOffset);
						
						lngCurrentPartNo = lngCurrentPartNo + 1;
						lngAttachedPartNo = -1;
					}
					else
						//��������
						lngAttachedPartNo = lngAttachedPartNo + 1;



					//��ʼ���λ���3·ͼ��
					for( k = 1 ;k<= lngSumLayoutColumn;k++)
					{
						if( k == 1 )
							//y��������ֻ��Ҫ��ֵһ�Ρ�
							InsPnt.SetY(modPHScal::VX_pt0y + sngCrd);
						InsPnt.SetZ(0.0);
						//�趨��ת����Ϊ0��
						rotA = 0;
						if( modPHScal::gintParallelNum == 2 )
						{
							//˫֧��
							//if( glNumSA = 2 ){
							//�ı������������������ܻ����ȷ�����
							rsRefZB->get_Collect((_variant_t)_T("CLnum"),&vTmp);
							if( vtoi(vTmp) == 2 )
							{
								//˫֧����˫����
								if( modPHScal::df == _T("Z") )
								{
									//˫֧����˫����,����ͼ��·,����ͼһ·
									if( k == 1 )
									{
										//˫֧����˫����,����ͼ��·,����ͼһ·
										//����ͼ1st·
										InsPnt.SetX(modPHScal::VX_pt0x - DblHangerOffset);
										
										if( modPHScal::glIDIndex == iSA )
										{
											if( modPHScal::bPAIsHanger() )
												tmpHeight = tmpHeight + 2 * HeightOffset;
											else
												tmpHeight = tmpHeight - 4 * HeightOffset;
											
											//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����
											sVfx = modPHScal::fx + modPHScal::gf;
											xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											pp1.SetPoint(InsPnt[0],InsPnt[1],InsPnt[2]);
											if( modPHScal::gf == _T("X") )
											{
												//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����
												//if( sVfx.Right(1) == _T("X") ){
												if( sVfx.Left(1) == _T("P") )
												{
													//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,P��
													//������λgdw
													h2.SetX(xGDW1);
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0] - h2[0]);
													h2.SetY(h1[1]);
													h3.SetX((h1[0] + h2[0]) / 2 );
													h3.SetY(h1[1] + HeightOffset + dimDist);
													if( modPHScal::glClassID != iGCement )
													{
													//	if(g_bPrintSA_Value)
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
														}
														//�������쳤��a														
														rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
														xGDW1=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
														bxSAxa_Null=false;
														if(vTmp.vt==VT_NULL)
															bxSAxa_Null=true;
														xSAxa=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
														xL1=vtof(vTmp);
														if(bxSAxa_Null) 
															h2.SetX(h1[0] + xL1 - xGDW1);
														else 
															h2.SetX(h1[0] + xSAxa - xGDW1);
														
														h3.SetX((h1[0] + h2[0]) / 2 );
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												else
												{ //IF LEFT(sVfx,1)=_T("O")
													//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,O��
													//������λgdw
													rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
													xGDW1=vtof(vTmp);
													rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
													bxSAxa_Null=false;
													if(vTmp.vt==VT_NULL)
														bxSAxa_Null=true;
													xSAxa=vtof(vTmp);
													rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
													xL1=vtof(vTmp);
													h1.SetPoint(InsPnt[0],tmpHeight);
													h2.SetPoint(h1[0] + xGDW1,h1[1]);
													h3.SetPoint((h1[0] + h2[0]) / 2 ,h1[1] + HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
													//	if(g_bPrintSA_Value)
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
														}
														//�������쳤��a
														//rs->get_Collect((_variant_t)_T("xSAxa"),vTmp);
														//rs.GetFieldValeu(_T("xGDW1"),vTmp2);
														if(bxSAxa_Null)
														{	
															// rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
															h2.SetX(h1[0] - xL1 + xGDW1);
														}
														else
															h2.SetX(h1[0] - xSAxa + xGDW1);
														h3.SetX((h1[0] + h2[0]) / 2 );
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												//������λgdwx
												h1.SetY(tmpHeight + HeightOffset);
												h2.SetX(h1[0] - (modPHScal::giAttDxOffsetX - DblHangerOffset));
												h2.SetY(h1[1]);
												h3.SetX((h1[0] + h2[0]) / 2);
												h3.SetY(h1[1] + HeightOffset + dimDist);
												h3.SetX((h1[0] + h2[0]) / 2 );
												if(g_bPrintSA_Value)
												{
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(modPHScal::gdwx -  tmpGBJJwidth / 2)));
												}
												//����//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����
											}
											else
											{ //sVfx.Right(1) = _T("Z")
												//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����,������Z����
												//˫�۸ּ��
												if( modPHScal::iCSnum == 1 )
												{
												}
												else
												{
													h1.SetPoint(InsPnt[0] - tmpGBwidth,tmpHeight);
													h2.SetPoint(InsPnt[0] + tmpGBwidth,h1[1]);
													h3.SetPoint((h1[0] + h2[0]) / 2  , h1[1] + HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												//������λgdwx
												h1.SetPoint(InsPnt[0],tmpHeight);
												h2.SetPoint(h1[0] - (modPHScal::giAttDxOffsetX - DblHangerOffset),h1[1]);
												h3.SetPoint((h1[0] + h2[0]) / 2,h1[1] + 2 * HeightOffset + 2 * dimDist);
												if(g_bPrintSA_Value)
												{
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx - tmpGBJJwidth / 2))));
												}
											}
											//����//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����
										}
										else
										{
											//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,���Ǹ���
											rsRefZB->get_Collect((_variant_t)_T("fx"),&vTmp);
											sVfx = modPHScal::GetPhsSAfx(vtoi(vTmp) % 4);
											xScal = (sVfx.Left(1) == _T("P") ? 1: -1);
										}
										sVfx1 = _T("X");
									}
									else if( k == 2 )
									{
										//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ2nd·
										//����ͼ2nd·
										InsPnt.SetX(modPHScal::VX_pt0x + DblHangerOffset);
										if( modPHScal::glIDIndex == iSA )
										{
											//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ2nd·,����
											//2nd��1st�Գ�
											sVfx = (sVfx.Left( 1) == _T("P") ? _T("O") : _T("P")) + modPHScal::gf;
											xScal = (sVfx.Left( 1) == _T("P") ? 1 : -1);
											//������λ
											if( sVfx.Right( 1) == _T("X") )
											{
												//˫֧����˫����,����ͼ��·,����ͼ2nd·,����,������X����
												if( sVfx.Left(1) == _T("P") )
												{
													//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ2nd·,����,������X����,P��
													//������λgdw
													//	rs->get_Collect((_variant_t)_T("xGDW1"),vTmp);
													rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
													xGDW1=vtof(vTmp);
													rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
													bxSAxa_Null=false;
													if(vTmp.vt==VT_NULL)
														bxSAxa_Null=true;
													xSAxa=vtof(vTmp);
													rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
													xL1=vtof(vTmp);
													h2.SetX(xGDW1);
													h1.SetPoint(InsPnt[0],tmpHeight);
													h2.SetPoint(h1[0] - h2[0],h1[1]);
													h3.SetPoint((h1[0] + h2[0]) / 2,h1[1] + HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
													//	if(g_bPrintSA_Value)
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
														}
														//�������쳤��a
														//rs->get_Collect((_variant_t)_T("xSAxa"),vTmp);
														//	  rs.GetFieldValeu(_T("xGDW1"),vTmp2);
														rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
														xGDW1=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
														bxSAxa_Null=false;
														if(vTmp.vt==VT_NULL)
															bxSAxa_Null=true;
														xSAxa=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
														xL1=vtof(vTmp);
														if(bxSAxa_Null)
														{
															//  rs->get_Collect((_variant_t)_T("xL1"),vTmp);
															h2.SetX(h1[0] + xL1 - xGDW1);
														}
														else
															h2.SetX(h1[0] + xSAxa - xGDW1);
														h3.SetX((h1[0] + h2[0]) / 2);
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												else
												{ //IF LEFT(sVfx,1)=_T("O")
													//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ2nd·,����,������X����,O��
													//������λgdw
													h2.SetX(xGDW1);
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0] + h2[0]);
													h2.SetY(h1[1]);
													h3.SetX((h1[0] + h2[0]) / 2 );
													h3.SetY(h1[1] + HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
													//	if(g_bPrintSA_Value)
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
														}
														//�������쳤��a
														//rs->get_Collect((_variant_t)_T("xSAxa"),vTmp);
														//rs.GetFieldValeu(_T("xGDW1"),vTmp2);
														rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
														xGDW1=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
														bxSAxa_Null=false;
														if(vTmp.vt==VT_NULL)
															bxSAxa_Null=true;
														xSAxa=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
														xL1=vtof(vTmp);
														if(bxSAxa_Null)
														{
															//rs->get_Collect((_variant_t)_T("xL1"),vTmp);
															h2.SetX(h1[0] - xL1 + xGDW1);
														}
														else
															h2.SetX(h1[0] - xSAxa + xGDW1);
														
														h3.SetX((h1[0] + h2[0]) / 2 );
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														
													}
												}
												//}else{ //sVfx.Right(1) = _T("Z")
												//}
												}
												else
												{
													//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ2nd·,����,������Z����
													//˫�۸ּ��
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														h1.SetPoint(InsPnt[0] - tmpGBwidth, tmpHeight);
														h2.SetPoint(InsPnt[0] + tmpGBwidth,h1[1]);
														h3.SetPoint((h1[0] + h2[0]) / 2 ,h1[1] + HeightOffset + dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
												}												
											}
											else
											{
												//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ2nd·,���Ǹ���
												sVfx = (sVfx.Left(1) == _T("P") ? _T("O") : _T("P")) + sVfx.Mid( 1, 1);
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											}
											sVfx1 = _T("X");
										}
										else
										{
											//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·
											//����ͼ1st·
											InsPnt.SetX(modPHScal::VZ_pt0x);
											if( modPHScal::glIDIndex == iSA )
											{
												//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����
												sVfx = modPHScal::GetPhsSAfx((modPHScal::GetPhsSAfx(modPHScal::gf1) + 1) % 4);
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
												//������λ
												
												if( sVfx.Right(1) == _T("X") )
												{
													//˫֧����˫����,����ͼ��·,����ͼ1st·,����,������X����
													
													if( sVfx.Left(1) == _T("P") )
													{
														//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,P��
														//������λgdw
														//rs->get_Collect((_variant_t)_T("xGDW1"),vTmp);
														rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
														xGDW1=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
														bxSAxa_Null=false;
														if(vTmp.vt==VT_NULL)
															bxSAxa_Null=true;
														xSAxa=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
														xL1=vtof(vTmp);
														h2.SetX(xGDW1);
														h1.SetPoint(InsPnt[0],tmpHeight);
														h2.SetPoint(h1[0] - h2[0],h1[1]);
														h3.SetPoint((h1[0] + h2[0]) / 2,h1[1] + HeightOffset + dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															//rs->get_Collect((_variant_t)_T("xSAxa"),vTmp);
															//	  rs.GetFieldValeu(_T("xGDW1"),vTmp2);
															rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
															xGDW1=vtof(vTmp);
															rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
															bxSAxa_Null=false;
															if(vTmp.vt==VT_NULL)
																bxSAxa_Null=true;
															xSAxa=vtof(vTmp);
															rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
															xL1=vtof(vTmp);
															if(bxSAxa_Null)	
																h2.SetX(h1[0] + xL1 - xGDW1);
															else
																h2.SetX(h1[0] + xSAxa - xGDW1);
															h3.SetX((h1[0] + h2[0]) / 2 );
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													else
													{ //IF LEFT(sVfx,1)=_T("O")
														//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,P��
														//������λgdw
														//rs->get_Collect((_variant_t)_T("xGDW1"),vTmp);
														rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
														xGDW1=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
														bxSAxa_Null=false;
														if(vTmp.vt==VT_NULL)
															bxSAxa_Null=true;
														xSAxa=vtof(vTmp);
														rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
														xL1=vtof(vTmp);
														h2.SetX(xGDW1);
														h1.SetPoint(InsPnt[0],tmpHeight);
														h2.SetPoint(h1[0] + h2[0],h1[1]);
														h3.SetPoint((h1[0] + h2[0]) / 2,h1[1] + HeightOffset + dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															//rs->get_Collect((_variant_t)_T("xSAxa"),vTmp);
															//rs.GetFieldValeu(_T("xGDW1"),vTmp2);
															rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
															xGDW1=vtof(vTmp);
															rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
															bxSAxa_Null=false;
															if(vTmp.vt==VT_NULL)
																bxSAxa_Null=true;
															xSAxa=vtof(vTmp);
															rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
															xL1=vtof(vTmp);
															if(bxSAxa_Null)
															{
																//rs->get_Collect((_variant_t)_T("xL1"),vTmp);
																h2.SetX(h1[0] - xL1 + xGDW1);
															}
															else
																h2.SetX(h1[0] - xSAxa + xGDW1);
															h3.SetX((h1[0] + h2[0]) / 2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													h1.SetPoint(InsPnt[0],tmpHeight);
													h2.SetPoint(h1[0] - modPHScal::giAttDzOffsetX,h1[1]);
													h3.SetPoint((h1[0] + h2[0]) / 2,h1[1] + 2 * HeightOffset + dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz))));
													}
												}
												else
												{ //sVfx.Right(1) = _T("Z")
													//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,����,������Z����
													//˫�۸ּ��
													
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														h1.SetPoint(InsPnt[0] - tmpGBwidth,tmpHeight);
														h2.SetPoint(InsPnt[0] + tmpGBwidth,h1[1]);
														h3.SetPoint((h1[0] + h2[0]) / 2,h1[1] + HeightOffset + dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													h1.SetPoint(InsPnt[0],tmpHeight);
													h2.SetPoint(h1[0] - modPHScal::giAttDzOffsetX,h1[1]);
													h3.SetPoint((h1[0] + h2[0]) / 2,h1[1] + 2 * HeightOffset+ dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz))));
													}
												}
											}
											else
											{
												//˫֧����˫����,����ͼ��·,����ͼһ·,����ͼ1st·,���Ǹ���
												sVfx = CString(_T("P")) +(sVfx.Mid( 1, 1) == _T("X") ?  _T("Z") : _T("X"));
												xScal = 1;
											}
											sVfx1 = _T("Z");
										}
										//����//˫֧����˫����,����ͼ��·
									}
									else
									{
										//˫֧����˫����,����ͼһ·,����ͼ��·
										//df=_T("X")
										if( k == 1 )
										{
											//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·
											InsPnt.SetX(modPHScal::VX_pt0x);
											if( modPHScal::glIDIndex == iSA )
											{
												if( modPHScal::bPAIsHanger() )
													tmpHeight = tmpHeight + 2 * HeightOffset;
												else
													tmpHeight = tmpHeight - 4 * HeightOffset;
												
												//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����
												sVfx = modPHScal::fx + modPHScal::gf;
												//������λ
												if( sVfx.Right(1) == _T("X") )
												{
													//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����
													
													if( sVfx.Left(1) == _T("P") )
													{
														//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����,P��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]-h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															/*rs->get_Collect((_variant_t)_T("xSAxa"),vTmp);
															bxSAxa_Null=false;
															if(vTmp.vt==VT_NULL)
															bxSAxa_Null=true;*/
															if(bxSAxa_Null)
																h2.SetX(h1[0]+xL1-xGDW1);
															else
																h2.SetX(h1[0]+xSAxa-xGDW1);
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													else
													{ //IF LEFT(sVfx,1)=_T("O")
														//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����,O��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]+h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null)
																h2.SetX(h1[0]-xL1+xGDW1);
															else 
																h2.SetX(h1[0]-xSAxa+xGDW1);
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]- modPHScal::giAttDxOffsetX);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+2* HeightOffset+ dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx))));
													}
												}
												else
												{ //sVfx.Right(1) = _T("Z")
													//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����,������Z����
													//˫�۸ּ��
													
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														h1.SetX(InsPnt[0] - tmpGBwidth);
														h1.SetY(tmpHeight);
														h2.SetX(InsPnt[0] + tmpGBwidth);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]- modPHScal::giAttDxOffsetX);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+2* HeightOffset+ dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx))));
													}
												}
												//����//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����
											}
											else
											{
												//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,���Ǹ���
												rsRefZB->get_Collect((_variant_t)_T("fx"),&vTmp);
												sVfx = modPHScal::GetPhsSAfx(vtoi(vTmp)%4);
											}
											sVfx1 = _T("X");
											xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
										}
										else if( k == 2 )
										{
											//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ2nd·
											//����ͼ1st·
											InsPnt.SetX(modPHScal::VZ_pt0x - DblHangerOffset);
											if( modPHScal::glIDIndex == iSA )
											{
												//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����
												sVfx = modPHScal::GetPhsSAfx((modPHScal::GetPhsSAfx(modPHScal::gf1) + 1) % 4);
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
												if( sVfx.Right(1) == _T("X") )
												{
													//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����
													
													if( sVfx.Left(1) == _T("P") )
													{
														//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����,P��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]-h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null)
																h2.SetX(h1[0]+xL1-xGDW1);
															else
																h2.SetX(h1[0]+xSAxa-xGDW1);
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													else
													{ //IF LEFT(sVfx,1)=_T("O")
														//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����,O��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]+h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null)
																h2.SetX(h1[0]-xL1+xGDW1);
															else
																h2.SetX(h1[0]-xSAxa+xGDW1);
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													//������λgdwx
													h1.SetY(tmpHeight + HeightOffset);
													h2.SetX(h1[0] - (modPHScal::giAttDzOffsetX - DblHangerOffset));
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+HeightOffset+ dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz-tmpGBJJwidth / 2))));
													}
												}
												else
												{ //sVfx.Right(1) = _T("Z")
													//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����,������Z����
													//˫�۸ּ��
													
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														h1.SetX(InsPnt[0] - tmpGBwidth);
														h1.SetY(tmpHeight);
														h2.SetX(InsPnt[0] + tmpGBwidth);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													//������λgdwz
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]- (modPHScal::giAttDxOffsetX - DblHangerOffset));
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+2* HeightOffset+ dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz-tmpGBJJwidth / 2))));
													}
												}
												//����//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,����
											}
											else
											{
												//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ1st·,���Ǹ���
												sVfx = CString(_T("P")) +(sVfx.Mid( 1, 1) == _T("X") ?  _T("Z") : _T("X"));
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											}
											sVfx1 = _T("Z");                              
										}
										else
										{
											//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ2nd·
											//����ͼ2nd·
											InsPnt.SetX(modPHScal::VZ_pt0x+ DblHangerOffset); 
											if( modPHScal::glIDIndex == iSA )
											{
												//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ2nd·,����
												//�����ԳƲ���
												sVfx.SetAt(0,(sVfx.Left(1) == _T("P") ? _T('O') : _T('P')));
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
												if( sVfx.Right(1) == _T("X") )
												{
													//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ2nd·,����,������X����
													
													if( sVfx.Left(1) == _T("P") )
													{
														//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ2nd·,����,������X����,P��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]-h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null)
																h2.SetX(h1[0]+xL1-xGDW1);
															else
																h2.SetX(h1[0]+xSAxa-xGDW1);
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													else
													{ //IF LEFT(sVfx,1)=_T("O")
														//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ2nd·,����,������X����,O��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]+h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null)
																h2.SetX(h1[0]-xL1+xGDW1);
															else
																h2.SetX(h1[0]-xSAxa+xGDW1);
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}                                    
													//}else{ //sVfx.Right(1) = _T("Z")
													//}
												}
												else
												{
													//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ2nd·,����,������Z����
													//˫�۸ּ��
													
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														h1.SetX(InsPnt[0] - tmpGBwidth);
														h1.SetY(tmpHeight);
														h2.SetX(InsPnt[0] + tmpGBwidth);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
												}
											}
											else
											{
												//˫֧����˫����,����ͼһ·,����ͼ��·,����ͼ2nd·,���Ǹ���
												sVfx = (sVfx.Left(1) == _T("P") ? _T("O") : _T("P")) + sVfx.Mid( 1, 1);
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											}
											sVfx1 = _T("Z");
										}
									}
								}
								
								//**************************************************************************************************************************************************//
								else
								{
									//˫֧����������
									if( modPHScal::df == _T("Z") )
									{
										//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,O��
										//����ͼ��·
										if( k == 1 )
										{
											//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·
											InsPnt.SetX(modPHScal::VX_pt0x - DblHangerOffset);
											if( modPHScal::glIDIndex == iSA )
											{
												//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����
												if( modPHScal::bPAIsHanger() )
													tmpHeight = tmpHeight + 2 * HeightOffset;
												else
													tmpHeight = tmpHeight - 4 * HeightOffset;
												
												sVfx=modPHScal::fx+modPHScal::gf;
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
												pp1=InsPnt;
												if( modPHScal::gf == _T("X"))
												{
													//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����
													
													if( sVfx.Left(1) == _T("P") )
													{
														//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,P��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]-h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ 2 * dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null)
																h2.SetX(h1[0]+xL1-xGDW1);
															else
																h2.SetX(h1[0]+xSAxa-xGDW1);	
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													else
													{ //IF LEFT(sVfx,1)=_T("O")
														//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,O��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]+h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null){
																h2.SetX(h1[0]-xL1+xGDW1);
															}else{
																h2.SetX(h1[0]-xSAxa+xGDW1);
															}
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													//������λgdwx
													h1.SetY(tmpHeight + HeightOffset);
													h2.SetX(h1[0]- (modPHScal::giAttDxOffsetX - DblHangerOffset));
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+HeightOffset+ 1.5 *  dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx-tmpGBJJwidth/2))));
													}
												}
												else
												{ //sVfx.Right(1) = _T("Z")
													//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������Z����
													//˫�۸ּ��
													
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														h1.SetX(InsPnt[0] - tmpGBwidth);
														h1.SetY(tmpHeight);
														h2.SetX(InsPnt[0] + tmpGBwidth);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													//������λgdwx
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]- (modPHScal::giAttDxOffsetX - DblHangerOffset));
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+HeightOffset+ 2* dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx-tmpGBJJwidth/2))));
													}
												}
											}
											else
											{
												//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,���Ǹ���
												rsRefZB->get_Collect((_variant_t)_T("fx"),&vTmp); sVfx = modPHScal::GetPhsSAfx(vtoi(vTmp)%4);
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											}
											sVfx1 = _T("X");
										}
										else if( k == 2 )
										{
											//˫֧����������,����ͼ��·,����ͼһ·,����ͼ2nd·
											//����ͼ2nd·,�����Ƴߴ磨�����������Ƹ���ͼ�εĳ����һ�£�
											InsPnt.SetX(modPHScal::VX_pt0x + DblHangerOffset);
											if( modPHScal::glIDIndex == iSA )
											{
												//˫֧����������,����ͼ��·,����ͼһ·,����ͼ2nd·,����
												//2nd��1st�Գ�
												sVfx = (sVfx.Left(1) == _T("P") ? _T("O") : _T("P")) + modPHScal::gf;
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											}
											else
											{
												//˫֧����������,����ͼ��·,����ͼһ·,����ͼ2nd·,���Ǹ���
												sVfx = (sVfx.Left(1) == _T("P") ? _T("O") : _T("P")) + sVfx.Mid( 1, 1);
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											}
											sVfx1 = _T("X");
										}
										else
										{
											//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·
											//����ͼ1st·
											InsPnt.SetX(modPHScal::VZ_pt0x);
											if( modPHScal::glIDIndex == iSA )
											{
												//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����
												sVfx = modPHScal::GetPhsSAfx((modPHScal::GetPhsSAfx(modPHScal::gf1) + 1) % 4);
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
												//������λ
												if( sVfx.Right(1) == _T("X") )
												{
													//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����
													if( sVfx.Left(1) == _T("P") )
													{
														//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,P��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]-h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
					//										if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null){
																h2.SetX(h1[0]+xL1-xGDW1);
															}else{
																h2.SetX(h1[0]+xSAxa-xGDW1);
															}
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													else
													{ //IF LEFT(sVfx,1)=_T("O")
														//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������X����,O��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]+h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
													//		if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null)
																h2.SetX(h1[0]-xL1+xGDW1);
															else
																h2.SetX(h1[0]-xSAxa+xGDW1);	
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0] - modPHScal::giAttDzOffsetX);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+2* HeightOffset+ dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz))));
													}
												}
												else
												{ //sVfx.Right(1) = _T("Z")
													//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,����,������Z����
													//˫�۸ּ��
													
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														h1.SetX(InsPnt[0] - tmpGBwidth);
														h1.SetY(tmpHeight);
														h2.SetX(InsPnt[0] + tmpGBwidth);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0] - modPHScal::giAttDzOffsetX);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+2* HeightOffset+ dimDist);
													
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz))));
													}
												}
											}
											else
											{
												//˫֧����������,����ͼ��·,����ͼһ·,����ͼ1st·,���Ǹ���
												sVfx = CString(_T("P")) +(sVfx.Mid( 1, 1) == _T("X") ?  _T("Z") : _T("X"));
												xScal = 1;
											}
											sVfx1 = _T("Z");
										}
									}
									//*******888888888888*******************************************************************************************************************
									else
									{
										//˫֧����������,����ͼһ·,����ͼ��·
										//����ͼһ·
										if( k == 1 )
										{
											//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·
											//����ͼ1st·
											InsPnt.SetX(modPHScal::VX_pt0x);
											if( modPHScal::glIDIndex == iSA )
											{
												if( modPHScal::bPAIsHanger() )
													tmpHeight = tmpHeight + 2 * HeightOffset;
												else
													tmpHeight = tmpHeight - 4 * HeightOffset;
												
												//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·,����
												sVfx=modPHScal::fx+modPHScal::gf;
												//������λ
												if( sVfx.Right(1) == _T("X") )
												{
													//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����
													
													if( sVfx.Left(1) == _T("P") )
													{
														//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����,P��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]-h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null){
																h2.SetX(h1[0]+xL1-xGDW1);
															}else{
																h2.SetX(h1[0]+xSAxa-xGDW1);
															}
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													else
													{ //IF LEFT(sVfx,1)=_T("O")
														//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·,����,������X����,O��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]+h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null)
																h2.SetX(h1[0]-xL1+xGDW1);
															else
																h2.SetX(h1[0]-xSAxa+xGDW1);
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]- modPHScal::giAttDxOffsetX);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+2* HeightOffset+ dimDist);
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx))));
													}
												}
												else
												{ //sVfx.Right(1) = _T("Z")
													//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·,����,������Z����
													//˫�۸ּ��
													
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														// if( Not modPHScal::bPAIsHanger() ){
														h1.SetX(InsPnt[0] - tmpGBwidth);
														h1.SetY(tmpHeight);
														h2.SetX(InsPnt[0] + tmpGBwidth);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													h1.SetX(InsPnt[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]- modPHScal::giAttDxOffsetX);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);
													h3.SetY(h1[1]+2* HeightOffset+ 2 * dimDist);
													
													if(g_bPrintSA_Value)
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx))));
													}
												}
											}
											else
											{
												//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·,���Ǹ���
												rsRefZB->get_Collect((_variant_t)_T("fx"),&vTmp); sVfx = modPHScal::GetPhsSAfx(vtoi(vTmp)%4);
											}
											sVfx1 = _T("X");
											xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
										}
										else if( k == 2 )
										{
											//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·
											//����ͼ1st·
											InsPnt.SetX(modPHScal::VZ_pt0x - DblHangerOffset);
											if( modPHScal::glIDIndex == iSA )
											{
												//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·,����
												sVfx = modPHScal::GetPhsSAfx((modPHScal::GetPhsSAfx(modPHScal::gf1) + 1) % 4);
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
												h1.SetX(InsPnt[0]);
												h1.SetY(tmpHeight + HeightOffset);

												//pfg20050531 "modPHScal::giAttDxOffsetX" ��Ϊ "modPHScal::giAttDzOffsetx"
												h2.SetX(h1[0]- (modPHScal::giAttDzOffsetX - DblHangerOffset));
												
												h2.SetY(h1[1]);
												h3.SetX((h1[0]+h2[0])/2);
												h3.SetY(h1[1]+HeightOffset+ dimDist + 5);
												if(g_bPrintSA_Value)
												{
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz-tmpGBJJwidth / 2))));
												}
											}
											else
											{
												//˫֧����������,����ͼһ·,����ͼ��·,����ͼ1st·,���Ǹ���
												sVfx = CString(_T("P")) +(sVfx.Mid( 1, 1) == _T("X") ?  _T("Z") : _T("X"));
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											}
											sVfx1 = _T("Z");
										}
										else
										{
											//˫֧����������,����ͼһ·,����ͼ��·,����ͼ2nd·
											//����ͼ2nd·
											InsPnt.SetX(modPHScal::VZ_pt0x+ DblHangerOffset); 
											if( modPHScal::glIDIndex == iSA )
											{
												//˫֧����������,����ͼһ·,����ͼ��·,����ͼ2nd·,����
												//�����ԳƲ���
												sVfx.SetAt(0,(sVfx.Left(1) == _T("P") ? _T('O') : _T('P')));
												xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
												if( sVfx.Right(1) == _T("X") )
												{
													//˫֧����������,����ͼһ·,����ͼ��·,����ͼ2nd·,����,������X����
													
													if( sVfx.Left(1) == _T("P") )
													{
														//˫֧����������,����ͼһ·,����ͼ��·,����ͼ2nd·,����,������X����,P��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]-h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null){
																h2.SetX(h1[0]+xL1-xGDW1);
															}else{
																h2.SetX(h1[0]+xSAxa-xGDW1);
															}
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													else
													{ //IF LEFT(sVfx,1)=_T("O")
														//˫֧����������,����ͼһ·,����ͼ��·,����ͼ2nd·,����,������X����,O��
														//������λgdw
														h2.SetX(xGDW1);
														h1.SetX(InsPnt[0]);
														h1.SetY(tmpHeight);
														h2.SetX(h1[0]+h2[0]);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
														//	if(g_bPrintSA_Value)
															{
																AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
																GetBoundingBox(AcObj,MinPoint,MaxPoint);
																OrdinateBound();
																AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
															}
															//�������쳤��a
															if(bxSAxa_Null){ 
																h2.SetX(h1[0]-xL1+xGDW1);
															}else{ 
																h2.SetX(h1[0]-xSAxa+xGDW1);
															}
															h3.SetX((h1[0]+h2[0])/2);
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
													//}else{ //sVfx.Right(1) = _T("Z")
													//}
												}
												else
												{
													//˫֧����������,����ͼһ·,����ͼ��·,����ͼ2nd·,����,������Z����
													//˫�۸ּ��
													
													if( modPHScal::iCSnum == 1 )
													{
													}
													else
													{
														h1.SetX(InsPnt[0] - tmpGBwidth);
														h1.SetY(tmpHeight);
														h2.SetX(InsPnt[0] + tmpGBwidth);
														h2.SetY(h1[1]);
														h3.SetX((h1[0]+h2[0])/2);
														h3.SetY(h1[1]+HeightOffset+ dimDist);
														if( modPHScal::glClassID != iGCement )
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
														}
													}
													
												}
												}
												else
												{
													//˫֧����������,����ͼһ·,����ͼ��·,����ͼ2nd·,���Ǹ���
													sVfx = (sVfx.Left(1) == _T("P") ? _T("O") : _T("P")) + sVfx.Mid( 1, 1);
													xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
												}
												sVfx1 = _T("Z");
											}
										}
									}
								}
								else
								{
									//��֧��
									if( k == 1 )
									{
										//��֧��,����ͼ1st·
										InsPnt.SetX(modPHScal::VX_pt0x);
										if( modPHScal::glIDIndex == iSA )
										{
											if( modPHScal::bPAIsHanger() )
											{
												tmpHeight = tmpHeight + 2 * HeightOffset;
											}
											else
											{
												tmpHeight = tmpHeight - 15 * HeightOffset; //glgl
											}
											//��֧��,����ͼ1st·,����
											sVfx=modPHScal::fx+modPHScal::gf;
											//�����Ϸ����
											//if( Not bHighPoint ){
											//tmpHeight = tmpInsPnt(1) + Abs(sngCrd0) + HeightOffset
											//}
											if( sVfx.Right(1) == _T("X") )
											{
												//��֧��,����ͼ1st·,����,������X����
												
												if( sVfx.Left(1) == _T("P") )
												{
													//��֧��,����ͼ1st·,����,������X����,P��
													//������λgdw
													h2.SetX(xGDW1);
													h1.SetX(tmpH1[0]); 
													h1.SetY(tmpHeight);///add ywh 
													h2.SetX(h1[0]-h2[0]);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2 );
													h3.SetY(h1[1]+HeightOffset + dimDist);
													if( modPHScal::glClassID != iGCement )
													{
													//	if(g_bPrintSA_Value)
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
														}
														//�������쳤��a
														if(bxSAxa_Null){
															h2.SetX(h1[0]+xL1-xGDW1);
														}else{
															h2.SetX(h1[0]+xSAxa-xGDW1);
														}
														h3.SetX((h1[0]+h2[0])/2);
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												else
												{ //IF LEFT(sVfx,1)=_T("O")
													//��֧��,����ͼ1st·,����,������X����,O��
													//������λgdw
													h2.SetX(xGDW1);
													h1.SetX(tmpH1[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]+h2[0]);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);//
													h3.SetY(h1[1]+HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
													//	if(g_bPrintSA_Value)
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
														}
														//�������쳤��a
														if(bxSAxa_Null)
															h2.SetX(h1[0]-xL1+xGDW1);
														else
															h2.SetX(h1[0]-xSAxa+xGDW1);	
														h3.SetX((h1[0]+h2[0])/2);
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												
												//������λgdwx
												h1.SetY(tmpHeight + HeightOffset);
												h2.SetX(h1[0]- modPHScal::giAttDxOffsetX);
												h2.SetY(h1[1]);
												h3.SetX((h1[0]+h2[0])/2);//
												h3.SetY(h1[1] + HeightOffset + dimDist);
												
												if(g_bPrintSA_Value)
												{
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx))));
												}
											}
											else
											{ //sVfx.Right(1) = _T("Z")
												//��֧��,����ͼ1st·,����,������Z����
												//˫�۸ּ��
												
												if( modPHScal::iCSnum == 1 )
												{
												}
												else
												{
													h1.SetX(tmpH1[0] - tmpGBwidth);
													h1.SetY(tmpHeight);
													h2.SetX(tmpH1[0] + tmpGBwidth);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);//
													h3.SetY(h1[1]+HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												
												//������λgdwx
												h1.SetX(tmpH1[0]);
												h1.SetY(tmpHeight);
												h2.SetX(h1[0]- modPHScal::giAttDxOffsetX);
												h2.SetY(h1[1]);
												h3.SetX((h1[0]+h2[0])/2);//
												h3.SetY(h1[1]+2* HeightOffset+ dimDist);
												if(g_bPrintSA_Value)
												{
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwx))));
												}
											}
										}
										else
										{ //���Ǹ���
											//��֧��,����ͼ1st·,���Ǹ���
											rsRefZB->get_Collect((_variant_t)_T("fx"),&vTmp); sVfx = modPHScal::GetPhsSAfx(vtoi(vTmp)%4);
										}
										sVfx1 = _T("X");
										xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
									}
									else if( k == 2 )
									{
										//��֧��,����ͼ1st·,����,������X����,P��
										//����ͼ1st·
										InsPnt.SetX(modPHScal::VZ_pt0x);
										if( modPHScal::glIDIndex == iSA )
										{
											//��֧��,����ͼ1st·,����
											sVfx = modPHScal::GetPhsSAfx((modPHScal::GetPhsSAfx(modPHScal::gf1) + 1) % 4);
											xScal = (sVfx.Left(1) == _T("P") ? 1 : -1);
											//����x����ĵڶ�·
											if( sVfx.Right(1) == _T("Z") )
											{
												//��֧��,����ͼ1st·,����,������Z����
												//˫�۸ּ��
												
												if( modPHScal::iCSnum == 1 )
												{
												}
												else
												{
													h1.SetX(tmpH1[0] - tmpGBwidth);
													h1.SetY(tmpHeight);
													h2.SetX(tmpH1[0] + tmpGBwidth);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);//
													h3.SetY(h1[1]+HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("C"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												
												//������λgdwz
												h1.SetX(tmpH1[0]);
												h1.SetY(tmpHeight);
												h2.SetX(h1[0] - modPHScal::giAttDzOffsetX);
												h2.SetY(h1[1]);
												h3.SetX((h1[0]+h2[0])/2); 
												h3.SetY(h1[1]+2* HeightOffset + dimDist );
												if(g_bPrintSA_Value)
												{
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz))));
												}
											}
											else
											{ //if sVfx.Right(1) = _T("X")
												//��֧��,����ͼ1st·,����,������X����                              
												if( sVfx.Left(1) == _T("P") )
												{
													//��֧��,����ͼ1st·,����,������X����,P��
													//������λgdw
													h2.SetX(xGDW1);
													h1.SetX(tmpH1[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]-h2[0]);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);//
													h3.SetY(h1[1]+HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
													//	if(g_bPrintSA_Value)
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
														}
														//�������쳤��a
														if(bxSAxa_Null){
															h2.SetX(h1[0]+xL1-xGDW1);
														}else{
															h2.SetX(h1[0]+xSAxa-xGDW1);
														}
														h3.SetX((h1[0]+h2[0])/2);
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												else
												{ //IF LEFT(sVfx,1)=_T("O")
													//��֧��,����ͼ1st·,����,������X����,O��
													//������λgdw
													h2.SetX(xGDW1);
													h1.SetX(tmpH1[0]);
													h1.SetY(tmpHeight);
													h2.SetX(h1[0]+h2[0]);
													h2.SetY(h1[1]);
													h3.SetX((h1[0]+h2[0])/2);//
													h3.SetY(h1[1]+HeightOffset+ dimDist);
													if( modPHScal::glClassID != iGCement )
													{
													//	if(g_bPrintSA_Value)
														{
															AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
															GetBoundingBox(AcObj,MinPoint,MaxPoint);
															OrdinateBound();
															AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdw))));
														}
														//�������쳤��a
														if(bxSAxa_Null){
															h2.SetX(h1[0]-xL1+xGDW1);
														}else{ 
															h2.SetX(h1[0]-xSAxa+xGDW1);
														}
														h3.SetX((h1[0]+h2[0])/2 );  
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														rsRefZB->get_Collect((_variant_t)_T("T"),&vTmp);AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
												}
												
												//������λgdw
												h1.SetY(tmpHeight + HeightOffset);
												h2.SetX(h1[0] - modPHScal::giAttDzOffsetX);
												h2.SetY(h1[1]);
												h3.SetX((h1[0]+h2[0])/2 ); //
												h3.SetY(h1[1]+HeightOffset + dimDist);
												if(g_bPrintSA_Value)
												{
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)h1, (LPVARIANT)h2, (LPVARIANT)h3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(modPHScal::gdwz))));
												}
											}
										}
										else
										{ //���Ǹ���
											//��֧��,����ͼ1st·,���Ǹ���
											sVfx = CString(_T("P")) +(sVfx.Mid( 1, 1) == _T("X") ?  _T("Z") : _T("X"));
											xScal = 1;
										}
										sVfx1 = _T("Z");
									}
								}
								//�������
								//��������PHD��PHD1����˨����ʱ����tmpInsPnt(0)ƫ����sngTmp(=3.5)������Ҫ�ָ�ƫ�ơ�
								//ֻҪ�Ե�֧������
								tmpH1.SetPoint(tmpInsPnt[0] + (bPHDorPHD1 ? sngTmp : 0), tmpInsPnt[1],tmpInsPnt[2]);
								//����ʹ������ķ������������ַ��򡣹�ע�͵���2001.05.08�����
								//tmph1.SetX(InsPnt[0]);
								//tmpH1(1) = InsPnt(1)
								//tmpH1(2) = InsPnt(2)
								tmpInsPnt=InsPnt;
								double inx=InsPnt[0];
								double iny=InsPnt[1];
								//***************************************************************************************************************                  
								//���Ӽ�/����
								if( modPHScal::glClassID == iROD || brsFind!=false )
								{
									if( k == 1 )
									{
										if( modPHScal::bPAIsHanger() )
										{ //����
											if( ! bLowPoint )
											{
												if( tmpInsPnt[1] > tmpLowPoint )
													bLowPoint = true;
												else
												{
													rsRefZB->get_Collect((_variant_t)_T("Crd"),&vTmp);
													if(vtof(vTmp) <= 0 )
														tmpLowPoint = tmpInsPnt[1] + vtof(vTmp);
													else
														tmpLowPoint = tmpInsPnt[1] - HeightOffset;
												}
											}
											if( tmpInsPnt[1] < tmpHeight )
											{
											}
											else
											{
												rsRefZB->get_Collect((_variant_t)_T("Crd"),&vTmp);
												if(vtof(vTmp) <= 0 )
													tmpHeight = tmpInsPnt[1] - vtof(vTmp);
												else
													tmpHeight = tmpInsPnt[1] + HeightOffset;
											}
										}
										else
										{ //֧��
											if( ! bLowPoint )
											{
												if( tmpInsPnt[1] > tmpLowPoint )
													bLowPoint = true;
												else
												{
													rsRefZB->get_Collect((_variant_t)_T("Crd"),&vTmp);
													if(vtof(vTmp) <= 0 )
														tmpLowPoint = tmpInsPnt[1] - vtof(vTmp);
													else
														tmpLowPoint = tmpInsPnt[1] + HeightOffset;
												}
											}
											if( tmpInsPnt[1] < tmpHeight )
											{
											}
											else
											{
												rsRefZB->get_Collect((_variant_t)_T("Crd"),&vTmp);
												if(vtof(vTmp) <= 0 )
													tmpHeight = tmpInsPnt[1] + vtof(vTmp);
												else
													tmpHeight = tmpInsPnt[1] - HeightOffset;
											}
										}
									}
									//Care Start //by Yattie
									//��������˨��ĸ�Ļ���
									if( modPHScal::glClassID == iAttached || modPHScal::glClassID == iBolts || modPHScal::glClassID == iNuts )
									{
										//�Ǹ���
										if( mlPartIndex[lngCurrentPartNo] == iPA || Ptype[lngCurrentPartNo] == _T("T3A") )
										{
											//��ǰ����Ǹ���,ǰһ��/��ǰ����ǹܲ����·�����
											if( mlPartClassID[lngCurrentPartNo + 1] == iROD )
											{
												//��һ�����������
												//�ض���2��3������,�ҷ������»���
												tmpInsPnt.SetY(InsPnt[1] - (lngAttachedPartNo + 1) * sngYoffsetOfAttached);
												tmpInsPnt.SetZ(InsPnt[2]);
												//�鷽����ת180��,���������ȦF3Ҫ��ת180��
												rotA = atan(1.0) * 4;
												//�������
												if( k == lngSEQColumn )
												{
													rsRefZB->get_Collect((_variant_t)_T("blkID"),&vTmp);
													xPos = vtof(modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("x1"), vtos(vTmp)));
													SeqPnt.SetPoint(tmpInsPnt[0] + xPos,tmpInsPnt[1],tmpInsPnt[2]);
												}
											}
											else
											{
												//��һ�������������
											}
										}
										else
										{
											//��ǰ����Ǹ���,ǰһ��/��ǰ������ǹܲ����·�����
											if( mlPartClassID[lngCurrentPartNo] == iROD )
											{
												//��ǰ����Ǹ���,ǰһ��/��ǰ������ǹܲ����·�����,ǰһ��/��ǰ���������
												if( mlPartIndex[lngCurrentPartNo + 1] == iSA || Ptype[lngCurrentPartNo + 1] == _T("T3") )
												{
													//��ǰ����Ǹ���,ǰһ��/��ǰ������ǹܲ����·�����,ǰһ��/��ǰ���������,��һ������Ǹ������Ϸ�����
													//�ض���2��3������,�ҷ������ϻ���
													tmpInsPnt.SetPoint(InsPnt[0],InsPnt[1] + (lngAttachedPartNo + 1) * sngYoffsetOfAttached,InsPnt[2]);
													//�ҵ���ߵ�
													//if( Not bHighPoint ){
													//  tmpHeight = tmpInsPnt(1) + HeightOffset
													//  bHighPoint = True
													//}
													//�鷽����ת
													rotA = atan(1.0) * -0;
													//�������
													if( k == lngSEQColumn )
													{
														rsRefZB->get_Collect((_variant_t)_T("blkID"),&vTmp);
														xPos = vtof(modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("x1"), vtos(vTmp)));
														SeqPnt.SetPoint(tmpInsPnt[0] + xPos,tmpInsPnt[1],tmpInsPnt[2]);
													}
												}
												else
												{
													//��ǰ����Ǹ���,ǰһ��/��ǰ������ǹܲ����·�����,ǰһ��/��ǰ���������,��һ��������Ǹ������Ϸ�����
													tmpInsPnt.SetPoint(InsPnt[0],InsPnt[1] - (lngAttachedPartNo + 1) * sngYoffsetOfAttached,InsPnt[2]);
													//�������
													rotA = 0;
													if( k == lngSEQColumn )
													{
														rsRefZB->get_Collect((_variant_t)_T("blkID"),&vTmp);
														xPos = vtof(modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("x1"), vtos(vTmp)));
														SeqPnt.SetPoint(tmpInsPnt[0] + xPos,tmpInsPnt[1],tmpInsPnt[2]);
													}
												}
											}
											else
											{
												if( mlPartClassID[lngCurrentPartNo] == iCSPR )
												{
													//ǰһ������Ǻ�������
													if( mlPartIndex[lngCurrentPartNo + 1] == iSA )
													{
														//ǰһ������Ǻ�������,��һ������Ǹ���
														if( Ptype[lngCurrentPartNo] == _T("PHD") || Ptype[lngCurrentPartNo] == _T("PHD1") )
														{
															COleVariant vID;
															rsRefZB->get_Collect((_variant_t)_T("ID"),&vID);
															if( vtos(vID)== _T("F9") )
															{
																bPHDorPHD1 = true;
																tmpInsPnt.SetPoint(InsPnt[0] + sngTmp,InsPnt[1] - 10.8 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7),InsPnt[2]);
																rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp);
																sBlk = vtos(vTmp);
																bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
																tmpInsPnt.SetX(InsPnt[0] - sngTmp);
																tmpInsPnt.SetZ(InsPnt[2]);
															}
															else if( vtos(vID)== _T("F1") )
															{
																bPHDorPHD1 = true;
																tmpInsPnt.SetPoint(InsPnt[0] + sngTmp,InsPnt[1] - 9 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7),InsPnt[2]);
																rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp);
																sBlk = vtos(vTmp);
																bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
																tmpInsPnt.SetY(InsPnt[1] + 2.4 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7));
																rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp); sBlk=vtos(vTmp);
																bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
																tmpInsPnt.SetX(InsPnt[0] - sngTmp);
																tmpInsPnt.SetY(InsPnt[1] - 9 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7));
																rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp); sBlk=vtos(vTmp);
																bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
																tmpInsPnt.SetY(InsPnt[1] + 2.4 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7));
															}
															else if( vtos(vID)== _T("F2") )
															{
																bPHDorPHD1 = true;
																tmpInsPnt.SetPoint(InsPnt[0] + sngTmp,InsPnt[1] - 7.5 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7),InsPnt[2]);
																rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp); sBlk=vtos(vTmp);
																bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
																tmpInsPnt.SetY(InsPnt[1] + 0.9 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7));
																rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp); sBlk=vtos(vTmp);
																bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
																tmpInsPnt.SetX(InsPnt[0] - sngTmp);
																tmpInsPnt.SetY(InsPnt[1] - 7.5 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7));
																rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp); sBlk=vtos(vTmp);
																bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
																tmpInsPnt.SetY(InsPnt[1] + 0.9 + (Ptype[lngCurrentPartNo] == _T("PHD") ? 0 : 7));
															}
														}
														if( k == lngSEQColumn )
														{
															//xPos = modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("x1"), rsRefZB.Fields(_T("blkID")))
															SeqPnt= tmpInsPnt;
														}
													}
													else
													{
														//ǰһ������Ǻ�������,��һ��������Ǹ���
														if( Ptype[lngCurrentPartNo + 1] == _T("L7") || Ptype[lngCurrentPartNo + 1] == _T("G12") || Ptype[lngCurrentPartNo + 1] == _T("L7Dd") || Ptype[lngCurrentPartNo + 1] == _T("G12Lc") )
														{
															//��һ�������L7��G12
															rotA = atan(1.0) * 2;
															COleVariant vID;
															rsRefZB->get_Collect((_variant_t)_T("ID"),&vID);
															if( vtos(vID)== _T("F14") )
															{
																tmpInsPnt.SetPoint(InsPnt[0] + 2.5,InsPnt[1],InsPnt[2]);
															}
															else if( vtos(vID) == _T("F1") )
															{
																tmpInsPnt.SetPoint(InsPnt[0] - 3,InsPnt[1],InsPnt[2]);
															}
															else if( vtos(vID) == _T("F2") )
															{
																tmpInsPnt.SetPoint(InsPnt[0] - 4.3,InsPnt[1],InsPnt[2]);
															}
															if( k == lngSEQColumn )
															{
																//xPos = modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("x1"), rsRefZB.Fields(_T("blkID")))
																SeqPnt=tmpInsPnt;
															}
														}
														else
														{
															if( Ptype[lngCurrentPartNo + 1] == _T("L8") || Ptype[lngCurrentPartNo + 1] == _T("G13") )
															{
																//��һ�������L8��G13���������û����˨�����ô���
															}
															else
															{
																if( mlPartClassID[lngCurrentPartNo + 1] == iROD )
																{
																	//��һ�����������
																}
																else
																{
																	//��������
																}
															}
														}
													}
											}
											else
											{
												if( mlPartClassID[lngCurrentPartNo - 1] == iSPR )
												{
													//ǰһ������ǵ���
												}
												else
												{
													//ǰһ��������ǵ���
													if( Ptype[lngCurrentPartNo - 1] == _T("L7") )
													{
														//ǰһ����L7                                       
													}
													else
													{
														if( Ptype[lngCurrentPartNo - 1] == _T("L8") )
														{
															//ǰһ����L8
														}
														else
														{
															//ǰһ������L8
														}
													}
												}
												//ǰһ��������Ǻ�������
											}
											//��ǰ����Ǹ���,ǰһ��/��ǰ������ǹܲ����·�����,ǰһ��/��ǰ�����������
											if( mlPartClassID[lngCurrentPartNo + 1] == iROD )
											{
												//��ǰ����Ǹ���,ǰһ��/��ǰ������ǹܲ����·�����,ǰһ��/��ǰ�����������,��һ�����������
												//�ض���1��2������,�ҷ������ϻ���
												tmpInsPnt.SetPoint(InsPnt[0],InsPnt[1] + (lngAttachedPartNo + 1) * sngYoffsetOfAttached,InsPnt[2]);
												//������ת
												//�������
												if( k == lngSEQColumn )
												{
													rsRefZB->get_Collect((_variant_t)_T("blkID"),&vTmp);
													xPos = vtof(modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("x1"), vtos(vTmp)));
													SeqPnt.SetPoint(tmpInsPnt[0] + xPos,tmpInsPnt[1],tmpInsPnt[2]);
												}
											}								
											else
											{
												//��ǰ����Ǹ���,ǰһ��/��ǰ������ǹܲ����·�����,ǰһ��/��ǰ�����������,��һ�������������
											}
										}
									}
									rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp); sBlk=vtos(vTmp);
									bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
									if( bTmp )
									{
										rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
										AddData2rsTZG(rsTZG, vtoi(vTmp), modPHScal::zdjh, _T(""), sVfx1, sBlk, tmpInsPnt);
									}
								}
								else if( modPHScal::glClassID == iROD )
								{
									//Care end //by Yattie
									//���Ǹ�����������
									//�����ˣ�ֱ����PLine�߻���
									//���˳����Ѿ�ȷ��
									//lngLUGDrawingLength = rsTzb.Fields(_T("crd"))
									if( mlPartIndex[lngCurrentPartNo - 1] == iPA || Ptype[lngCurrentPartNo - 1] == _T("T3A") )
									{
										tmpLUGpnt.SetPoint(tmpInsPnt[0],tmpInsPnt[1] - tmpLUGyOffset,tmpInsPnt[2]);
										if( mlPartIndex[lngCurrentPartNo + 1] == iSA || Ptype[lngCurrentPartNo + 1] == _T("T3") )
										{
											if( mlPartIndex[lngCurrentPartNo + 1] == iSA && k==1)
												tmpHeight=tmpHeight+lngLUGDrawingLength;
											DrawPLine(tmpLUGpnt, lngLUGDrawingLength + 2 * tmpLUGyOffset, atan(1.0) * 2,0.5);
										}
										else
										{
											DrawPLine(tmpLUGpnt, lngLUGDrawingLength + tmpLUGyOffset + tmpLugInsLen, atan(1.0) * 2,0.5);
										}
									}
									else
									{
										//���˻��������ƶ������Ӽ���
										tmpLUGpnt.SetPoint(tmpInsPnt[0],tmpInsPnt[1] - tmpLugInsLen, tmpInsPnt[2]);
										if( mlPartIndex[lngCurrentPartNo + 1] == iSA || Ptype[lngCurrentPartNo + 1] == _T("T3") )
										{
											if( mlPartIndex[lngCurrentPartNo + 1] == iSA && k==1)
												tmpHeight=tmpHeight+lngLUGDrawingLength;
											DrawPLine(tmpLUGpnt, lngLUGDrawingLength + tmpLUGyOffset + tmpLugInsLen, atan(1.0) * 2,0.5);
										}
										else
										{
											DrawPLine(tmpLUGpnt, lngLUGDrawingLength + 2 * tmpLugInsLen, atan(1.0) * 2,0.5);
										}
									}
								}
								else
								{
									//���Ǹ�����Ҳ��������
									//�������
									if( modPHScal::glIDIndex == iSA )
									{
										//���Ƹ���
										if( k == 1 )
										{
											tmpL1.SetY(tmpLowPoint);
											tmpL2.SetY(tmpLowPoint);
											
											if( EDIBAcad::g_bLocationDimension )
											{
												DrawphsZUZI(rsRefZB, tmpL1, iViewX);
												DrawphsZUZI(rsRefZB, tmpL2, iViewZ);
											}
										}
										//����
										tmpSApnt.SetX(tmpInsPnt[0]);
										tmpSApnt.SetZ(tmpInsPnt[2]);
										if( modPHScal::bPAIsHanger() )
										{
											//����
											if( Ptype[lngCurrentPartNo - 1] == _T("G12") || Ptype[lngCurrentPartNo - 1] == _T("G12Lc") || Ptype[lngCurrentPartNo - 1] == _T("G13") || Ptype[lngCurrentPartNo - 1] == _T("L9") || Ptype[lngCurrentPartNo - 1] == _T("L10") )
												//�²����������·�ֻ������G12��G13,L9/L10
												tmpSApnt.SetY(tmpInsPnt[1]);
											else
											{
												if( Ptype[lngCurrentPartNo - 1] == _T("PHD1") )
													tmpSApnt.SetY(tmpInsPnt[1]); //+ 2 * Abs(rsRefZB.Fields(_T("Crd")))
												else //�ϲ�������λ������
												{
													rsRefZB->get_Collect((_variant_t)_T("Crd"),&vTmp);
													tmpSApnt.SetY(tmpInsPnt[1] - fabs(vtof(vTmp)));
												}
											}
										}
										else
										{
											//֧��
											rsRefZB->get_Collect((_variant_t)_T("Crd"),&vTmp);
											tmpSApnt.SetY(tmpInsPnt[1] - fabs(vtof(vTmp)));
										}
										//������ʱ�����tmpInspnt()�����ڸ�����߱�ע
										tmpInsPnt=tmpSApnt;
										
										rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp); 
										sBlk=vtos(vTmp);
										if( modPHScal::gintParallelNum == 2 && modPHScal::glNumSA == 1 && k == 2 )
										{
											//�����˫֧������������ֻ����·��
											//��ǰ���ע�ߴ�ʱ��Ӧ�ñ��ֱ�עλ����˴�һ��(����עgintParallelNum = 2 And glNumSA = 1 And k = 2�����)��
										}
										else
										{
											bTmp = InsertPhsBLK(tmpSApnt, sBlk, xScal, 1, rotA, iGIPAuto);
											if( bTmp )
											{
												rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
												AddData2rsTZG(rsTZG, vtoi(vTmp), modPHScal::zdjh, _T(""), sVfx1, sBlk, tmpSApnt);
											}
										}
									}
									else
									{
										//���Ǹ���
										//if( Ptype[lngCurrentPartNo] == iCSPR && mlPartClassID[lngCurrentPartNo + 1] == iROD ){
										//}else{
										//}
										//��ת180�������Ҫ�����(with 0 tail)
										rsRefZB->get_Collect((_variant_t)_T("Rotation"),&vTmp);
										mlRot0 = vtof(vTmp);
										if( mlRot0 == 180 )
										{
											_variant_t vTmp;
											rs->Find((_bstr_t)(_T("Trim(BlkID)=\'") + blkID + _T("0\'")), 0, adSearchForward, vTmp);
// 											rs.FindFirst();
											rs->get_Collect((_variant_t)_T("xGDW1"),&vTmp);
											xGDW1=vtof(vTmp);
											rs->get_Collect((_variant_t)_T("xSAxa"),&vTmp);
											bxSAxa_Null=false;
											if(vTmp.vt==VT_NULL)
												bxSAxa_Null=true;
											xSAxa=vtof(vTmp);
											rs->get_Collect((_variant_t)_T("xL1"),&vTmp);
											xL1=vtof(vTmp);
											if( mlPartClassID[0] == iPAh )
											{
												rs->get_Collect((_variant_t)_T("Dh"),&vTmp);
												sngCrd0 =vtof(vTmp);
											}
											else
											{
												rs->get_Collect((_variant_t)_T("Zh"),&vTmp);
												sngCrd0 =vtof(vTmp);
											}
										}
										rs->get_Collect((_variant_t)(sVfx+_T("blk")),&vTmp); sBlk=vtos(vTmp);
										bTmp = InsertPhsBLK(tmpInsPnt, sBlk, xScal, 1, rotA, iGIPAuto);
										if( bTmp )
										{
											rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
											AddData2rsTZG(rsTZG, vtoi(vTmp), modPHScal::zdjh, _T(""), sVfx1, sBlk, tmpInsPnt);
										}
									}
							}
							
							//�������
							if( k == 1 )
							{
								if( modPHScal::glClassID == iROD )
								{
									//���˵Ļ��Ƴ���
									sngCrd0 = lngLUGDrawingLength;
									sngCrd = sngCrd + sngCrd0;
								}
								else
								{
									rsRefZB->get_Collect((_variant_t)_T("Crd"),&vTmp);
									sngCrd0 = vtof(vTmp);
									sngCrd = sngCrd + sngCrd0;
									if( (modPHScal::glClassID == iSPR || modPHScal::glClassID == iCSPR) && modPHScal::gbAnnotateCSPR )
									{
										//������ע���Լ����
										SeqPnt.SetPoint(tmpInsPnt[0],tmpInsPnt[1] + fabs(sngCrd0) / 2,tmpInsPnt[2]);
										rsRefZB->get_Collect((_variant_t)_T("CLgg"),&vTmp);
										AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(vtos(vTmp)), (LPVARIANT)SeqPnt, &_variant_t((double)3)).pdispVal;
                                        AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("��ע�ߴ�"), _T("DIM"))));
										AcObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentLeft));
										AcObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)SeqPnt);
                                        ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("��ע�ߴ�"), _T("DIM")),_T("��ע�ߴ�"));
									}
								}
								
								if( i == rsRefZB->RecordCount - 1 )
								{
									//�������,λ���ڲ۸��ϲ�
									//�浽���
									//SeqPnt(0) = tmpInsPnt(0) - 28
									//�浽�Ҳ�
									
									CString strBG_blockName;
									if( EDIBAcad::g_bBGForBJBW )
									{
										strBG_blockName = _T("BG_BW");
										SeqPnt.SetPoint(
											tmpInsPnt[0] + (modPHScal::gintParallelNum == 2 && modPHScal::df == _T("Z") ? DblHangerOffset + 5 : 28),
											tmpInsPnt[1]+ fabs(sngCrd0),//+2.2
											tmpInsPnt[2]);
									}
									else
									{
										strBG_blockName = _T("BG_D");
										SeqPnt.SetPoint(
											tmpInsPnt[0] + (modPHScal::gintParallelNum == 2 && modPHScal::df == _T("Z") ? DblHangerOffset + 5 : 28),
											tmpInsPnt[1] + fabs(sngCrd0),
											tmpInsPnt[2]);
									}
									
									if( (mlPartIndex[lngCurrentPartNo - 1] = iPA) && (mstrPAID==_T("Z9") || mstrPAID==_T("Z10") || mstrPAID==_T("CZ4")))
									{}
									else
									{
										bTmp = InsertPhsBLK(SeqPnt, strBG_blockName , 1, 1, 0, iGIPAuto);
										if( EDIBAcad::g_bBGForBJBW )
										{
											SeqPnt.SetPoint(
												tmpInsPnt[0] + (modPHScal::gintParallelNum == 2 && modPHScal::df == _T("Z") ? DblHangerOffset + 5 : 28),
												tmpInsPnt[1]+ fabs(sngCrd0)+5.0,
												tmpInsPnt[2]);
										}
										ModifyTextOfBG(SeqPnt, modPHScal::sngSEL, _T("SEL"),EDIBAcad::g_bBGForBJBW,FALSE);
									}
								}
							}
							
							if( k == lngSEQColumn )
							{
								//�����
								if( modPHScal::glClassID != iAttached && modPHScal::glClassID != iBolts && modPHScal::glClassID != iNuts )
								{
									rsRefZB->get_Collect((_variant_t)_T("blkID"),&vTmp);
									xPos = vtof(modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("x1"), vtos(vTmp)));
									yPos = vtof(modPHScal::sFindBlkPosFLD(_T("BlkID"), _T("y1"), vtos(vTmp)));
									SeqPnt.SetPoint(tmpInsPnt[0] + xPos,tmpInsPnt[1] + yPos,tmpInsPnt[2]);
								}
								/*
								else
								{
								//�����ı�Ų�����Ѿ���ǰ�渳ֵ
								}
								*/
								rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
								rsRefZB->get_Collect((_variant_t)_T("seq"),&vTmp2);
								DrawTagPoint(SeqPnt,vtoi(vTmp),vtoi(vTmp2));
							}
							
							if( k == lngDIMColumn )
							{//5
								//�ߴ���
								rsRefZB->get_Collect((_variant_t)_T("sizeDIM"),&vTmp);
								if( vTmp.vt!=VT_NULL)
								{//4
									if( vtof(vTmp) != 0 )
									{//3
										if( sngCrd0 != 0 && modPHScal::glClassID != iAttached && modPHScal::glClassID != iBolts && modPHScal::glClassID != iNuts )
										{//2
											//��ע�ߴ�
											if( modPHScal::glIDIndex == iSA && modPHScal::glClassID != iGCement )
											{
												//��ע�������Ӹ�������㿪ʼ����������Ե����ֹ
												if( Ptype[lngCurrentPartNo - 1] == _T("PHD") || Ptype[lngCurrentPartNo - 1] == _T("LHD") || Ptype[lngCurrentPartNo - 1] == _T("ZHA") || mlPartClassID[lngCurrentPartNo - 1] == iROD )
												{
													//���ڸ���������ߴ���Ҫ�ƿ�������ճ��һ��
													p1.SetPoint(InsPnt1[0],tmpSApnt[1]);
													p2.SetPoint(p1[0],p1[1] + fabs(sngCrd0));
													p3.SetPoint(p1[0] + (modPHScal::sngSEL > modPHScal::sngPEL ? 12 : 24),p2[1]);
													p3.SetZ(0.0);
												}
												else
												{
													if(modPHScal::glClassID==iG52_55)
													{
														p1.SetPoint(InsPnt1[0]+15,tmpSApnt[1]+fabs(sngCrd0));
														//G52~G55���߸߶�Hͼ��߶�28.3mm
														p2.SetPoint(p1[0],p1[1] + 28.3);
														p3.SetPoint(p1[0] + (modPHScal::sngSEL > modPHScal::sngPEL ? 5 : 10),(p1[1] + p2[1]) / 2);
														AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)p1, (LPVARIANT)p2, (LPVARIANT)p3).pdispVal;
														GetBoundingBox(AcObj,MinPoint,MaxPoint);
														OrdinateBound();
														vTmp=(VARIANT)FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("A01"));
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
													}
													p1.SetPoint(InsPnt1[0],tmpSApnt[1]);
													p2.SetPoint(p1[0],p1[1] + fabs(sngCrd0));
													p3.SetPoint(p1[0] + (modPHScal::sngSEL > modPHScal::sngPEL ? 5 : 10)+ dimDist,(p1[1] + p2[1]) / 2);
												}
											}
											else
											{
												if(  Ptype[lngCurrentPartNo] == _T("T3A"))
												{
													p1.SetPoint(InsPnt1[0]-5,tmpInsPnt[1]);
												}
												else
												{
													p1.SetPoint(InsPnt1[0],tmpInsPnt[1]);
												}
												p2.SetPoint(p1[0],p1[1] + sngCrd0);
												p3.SetPoint(p1[0] + (modPHScal::sngSEL > modPHScal::sngPEL ? 5 : 10) + dimDist,p1[1] + sngCrd0 / 2);
											}
											//���̫С(<2.5mm)��д���³ߴ磬����5mmд���� 
											if(fabs(sngCrd0)<2.5) 
												p3.SetPoint(p3[0] + 5,p1[1] );
											
											AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)p1, (LPVARIANT)p2, (LPVARIANT)p3).pdispVal;
											GetBoundingBox(AcObj,MinPoint,MaxPoint);
											OrdinateBound();
											//Set AcObj = MoSpace.AddDimRotated(p1(), p2(), p3(), Atn(1) * 2)
											rsRefZB->get_Collect((_variant_t)_T("sizeDIM"),&vTmp);
											if(vTmp.vt==VT_NULL)
												ShowMessage(GetResStr(IDS_NullPartSizeInTZB));
											else
											{
												if(modPHScal::glClassID == iROD )
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(vtof(vTmp))));
												else
													AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(fabs(vtof(vTmp)))));
											}
											AcObj.Invoke0(_T("Update"));
											rsRefZB->get_Collect((_variant_t)_T("recno"),&vTmp);
											AddData2rsTZG(rsTZG, vtoi(vTmp), modPHScal::zdjh, _T("sizeDIM"), _T("X"), _T(""), p1);
											
											if( modPHScal::glClassID == iROD )
											{//1
												//��������ˣ��������˻������볤�ȳߴ磬�Ա�˶ԺͰ�װ
												//1st�����¶�
												p1.SetPoint(InsPnt1[0],tmpInsPnt[1]);
												p2.SetX(p1[0]);
												if( mlPartIndex[lngCurrentPartNo - 1] == iPA || Ptype[lngCurrentPartNo - 1] == _T("T3A") )
													//��ǰ����Ǹ���,ǰһ��/��ǰ����ǹܲ����·�����
													p2.SetY(p1[1] - tmpLUGyOffset);
												else
													//�������Ӽ�������ƫ��tmpLugInsLen �������볤��
													p2.SetY(p1[1] - tmpLugInsLen);
												p2.SetZ(0.0);
												p3.SetPoint(p1[0] - 5,(p1[1] + p2[1]) / 2);
												
												
												if( g_bInsertLength )
												{
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)p1, (LPVARIANT)p2, (LPVARIANT)p3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													//�ֶ�A�������¶�����ߴ�
													rsRefZB->get_Collect((_variant_t)_T("A"),&vTmp);
													if( vTmp.vt==VT_NULL)
													{
														//MsgBox ResolveResString(iUE_NullLugExtendDimInTZB, _T("|1"), _T("A"), _T("|2"), ResolveResString(iUE_UP)), vbMsgBoxSetForeground
													}
													else
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(vtof(vTmp))));
													
													
													AcObj.Invoke0(_T("Update"));
													//2nd�����϶�
												}
												p1.SetPoint(InsPnt1[0] /*- 10*/,tmpInsPnt[1] + lngLUGDrawingLength);
												p2.SetX(p1[0]);
												if( mlPartIndex[lngCurrentPartNo + 1] == iSA || Ptype[lngCurrentPartNo + 1] == _T("T3") )
													//��ǰ����Ǹ���,ǰһ��/��ǰ����ǹܲ����·�����
													p2.SetY(p1[1] + tmpLUGyOffset);
												else
													//�������Ӽ�������ƫ��2mm�������볤��
													p2.SetY(p1[1] + tmpLugInsLen);
												p2.SetZ(0.0);
												p3.SetPoint(p1[0] - 5,(p1[1] + p2[1]) / 2);
												if( g_bInsertLength )
												{	
													AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)p1, (LPVARIANT)p2, (LPVARIANT)p3).pdispVal;
													GetBoundingBox(AcObj,MinPoint,MaxPoint);
													OrdinateBound();
													
													//�ֶ�B�������϶�����ߴ�
													rsRefZB->get_Collect((_variant_t)_T("B"),&vTmp);
													if( vTmp.vt==VT_NULL)
													{
														//MsgBox ResolveResString(iUE_NullLugExtendDimInTZB, _T("|1"), _T("B"), _T("|2"), ResolveResString(iUE_DOWN)), vbMsgBoxSetForeground
													}
													else
														AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(ftos1(vtof(vTmp))));
													
													
													AcObj.Invoke0(_T("Update"));
												}
											}//1
										}//2
									}//3
								}//4
							}//5
						}//6
						else
						{
							//��phsBlkDimPos����û���ҵ������
							tmpStr1.Format(GetResStr(IDS_NoThisBlockInphsBlkDimPos),blkID);
							ShowMessage(tmpStr1);
						}   //endof rs.nomatch
					}
				}
				//�������
			}
			//MsgBox blk
		}
		rsRefZB->MoveNext();
	}
	
	//�������������š�ֻ�����������ж���Щ����������棬��Щ�������棬���ָ���߲Ų��ύ�档
    DrawTagAll();
	//���Ҫ�Զ�������װͼ����λ�ã�ʹ���׼�հ״����롣
	if( modPHScal::AutoOrdinateDrawing !=false )
	{
        MoveObj(1,_T("phs"));
        MoveObj(1,_T("tag"));
	}

	objAcadDoc.Invoke(_T("SendCommand"),1,&_variant_t(_T("-dimstyle\n_apply\nALL\n\n")));
}
catch(_com_error e)
{
	ShowMessage(e.Description());
}
catch(COleException *e )
{
	e->ReportError();
	e->Delete();
}
catch(CException *e)
{
	e->ReportError();
	e->Delete();
}
delete [] Ptype;  //��¼֧���ܽṹ���������������
delete [] mlPartClassID;    //��¼����������
delete [] mlPartIndex; 
//������ΪAutoCAD���ڻ�ͼ,��VB�ַ������������ɵ�����,�������׷����Զ�������
//objAcadDoc.Regen (acActiveViewport) //�����������AutoCAD�ٻ�ͼ,�����Զ�������
//objAcadDoc.ActiveViewport.ZoomAll
}

void EDIBAcad::DrawphsZUZI(_RecordsetPtr rsRefZB, CCadPoint& InsPnt, long iView)
{
   //Dim rsza As Recordset
	try
	{
		int iAttDOffsetX;
		_Recordset* rsza =FrmTxsr.m_pViewTxsr->m_ActiveRs;
		//��������
		CCadPoint p, p1, p2, p3;
		CCadPoint pU;
		CCadPoint px;
		long i;
		CString sV, sZU;
		long  sngHS ;    //��ǵ��ܻ�֧�ܵ����������Ա�ȷ�����ӱ�Ż��Ʒ���
		//sngHS(����/֧��)=1/-1
		if( modPHScal::bPAIsHanger())
			sngHS = 1;
		else
			sngHS = -1;
		
		
		//���ҵ�
		if( iView == iViewX )
		{
			sV = _T("X");
			p1.SetX(modPHScal::VX_pt0x);
			iAttDOffsetX = modPHScal::giAttDxOffsetX;
		}
		else
		{
			sV = _T("Z");
			p1.SetX(modPHScal::VZ_pt0x);
			iAttDOffsetX = modPHScal::giAttDzOffsetX;
		}
		//p1(1) = modPHScal::VX_pt0y - giAttDOffsetY / 2
		p1.SetY(InsPnt[1] - sngHS * 4);
		
		//�����
		p2.SetPoint(p1[0] - iAttDOffsetX,p1[1],0);
		//�ߴ���λ��
		p3.SetPoint(p2[0] + iAttDOffsetX / 2,p2[1] - sngHS * modPHScal::giAttDOffsetY / 2 + sngHS * 5);//0
		AcObj = MoSpace.Invoke(_T("AddDimAligned"),3,(LPVARIANT)p1, (LPVARIANT)p2, (LPVARIANT)p3).pdispVal;
		GetBoundingBox(AcObj,MinPoint,MaxPoint);
		OrdinateBound();
		//��װ��λ
		//���б�ע���־��Ծ���ֵ��ʾ
		CString str;

		if(EDIBAcad::g_bBGForBJBW)//������ʽ��װֵ
			str.Empty();
		else
			str.LoadString(IDS_INSTALL);

		AcObj.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t((fabs(modPHScal::yr) > modPHScal::gUseSpringMinDist ? str :_T("")) + (iView == iViewX ? ftos(fabs(modPHScal::dxa)) : ftos(fabs(modPHScal::dza)))));
		AcObj.Invoke0(_T("Update"));
		//�������λ��
		px = AcObj.GetPropertyByName(_T("TextPosition"));

		pU=p1;
		if(modPHScal::bPAIsHanger())
			pU.SetY(pU[1]-25);
		else
			pU.SetY(pU[1]-40);

		CString strUB;
		if( iView == iViewX )
		{
			if( giUPxyz == 1)//x����ֱ����
				strUB=_T("UPxRy");
			else if(giUPxyz==2)//y����ֱ����
				strUB=_T("UPyRx");
			else					//z����ֱ����
				strUB=_T("UPzRx");
		}
		else
		{
			if( giUPxyz == 1)
				strUB=_T("UPxRz");
			else if(giUPxyz==2)
				strUB=_T("UPyRz");
			else
				strUB=_T("UPzRy");
		}
		InsertPhsBLK(pU,strUB,1,1,0,1);
		p3.SetY(px[1]);
		//MsgBox p3(1)
		double msTxtHeight;
		msTxtHeight = vtof(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("dimtxt")))) * vtof(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("dimscale"))));
		if (fabs(modPHScal::yr) > modPHScal::gUseSpringMinDist && !EDIBAcad::g_bDWForBJBW)//λ�ƴ�����Сλ���Ҳ��ǰ����Ķ�λ��ʽ
		{
			CString text;
			//��̬��λ
			p.SetPoint((p1[0] + p2[0]) / 2,p3[1] + sngHS * (msTxtHeight + 4));
			if((iView == iViewX) && (modPHScal::dxl!=modPHScal::dxa) || (iView == iViewZ) && (modPHScal::dzl!=modPHScal::dza))
			{
				text.Format(IDS_COLE_FORM,(int)(iView == iViewX ? fabs(modPHScal::dxl) : fabs(modPHScal::dzl)));	  
				if( giUPxyz == 1)
					AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(text), (LPVARIANT)p, &_variant_t((double)msTxtHeight)).pdispVal;
				else if( giUPxyz == 2)
					AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(text), (LPVARIANT)p, &_variant_t((double)msTxtHeight)).pdispVal;
				else if( giUPxyz == 3 )
					AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(text), (LPVARIANT)p, &_variant_t((double)msTxtHeight)).pdispVal;
                AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("��ע�ߴ�"), _T("DIM"))));
				AcObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentMiddle));
				AcObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)p);
 //               ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("��ע�ߴ�"), _T("DIM")),_T("��ע�ߴ�"));
                ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("��ע�ߴ�"), _T("DIM")));
			}     
			else
			{
			}
			
			//��̬��λ
			p.SetPoint((p1[0] + p2[0]) / 2,p[1] + sngHS * (msTxtHeight + 2));
			if((iView == iViewX) && (modPHScal::dxr!=modPHScal::dxa) || (iView == iViewZ) && (modPHScal::dzr!=modPHScal::dza))
			{
				text.Format(IDS_HOT_FORM,(int)(iView == iViewX ? fabs(modPHScal::dxr) : fabs(modPHScal::dzr)));
				AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(text), (LPVARIANT)p, &_variant_t((double)msTxtHeight)).pdispVal;
                AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("��ע�ߴ�"), _T("DIM"))));
				AcObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentMiddle));
				AcObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)p);
                ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("��ע�ߴ�"), _T("DIM")));
			}
		}
		
		//���óߴ��������dimexe=5,���ú�����ԲȦ�ķ�Բ������

		objAcadDoc.Invoke(_T("SetVariable"), 2,&_variant_t(_T("dimexe")), &_variant_t((long)EDIBAcad::g_fOvertopDim));

		//һ�����Ϊ8������

		for( i = 1 ;i<= 8;i++)
		{
			sZU=vtos((_variant_t)rsza->GetCollect(_variant_t(sV + _T("ZU") + ltos( i))));
			if( sZU != _T("") )
			{
				//���Ŵ���,������
				p.SetPoint(
					InsPnt[0] - iAttDOffsetX + (iView == iViewX ? Sgn(modPHScal::giAttDxOffsetX) : Sgn(modPHScal::giAttDzOffsetX)) * 10 * ((i - 1) % 2),
					InsPnt[1] - sngHS * (0.5 * modPHScal::giAttDOffsetY + 7 + 6 * ((i - 1) / 2)),
					InsPnt[2]);
				AcObj = MoSpace.Invoke(_T("AddCircle"),2,(LPVARIANT)p, &_variant_t((double)3)).pdispVal;
				GetBoundingBox(AcObj,MinPoint,MaxPoint);
				int iTemp=(iView == iViewX ? modPHScal::giAttDxOffsetX : modPHScal::giAttDzOffsetX);
				
				if((iTemp>=0 && i % 2 ==1) || (iTemp<0 && i % 2 ==0) )
				{
					CCadPoint startP,endP;
					startP.SetPoint(p[0]+3,p[1]);
					endP.SetPoint(startP[0]+4,p[1]);
					MoSpace.Invoke(_T("AddLine"),2,(LPVARIANT)startP,(LPVARIANT)endP);
				}
				OrdinateBound();
				if( sZU.GetLength() <= 2 )
				{
					//2�ַ����е����
					AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sZU), (LPVARIANT)p, &_variant_t((double)3)).pdispVal;
                    AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("��ע�ߴ�"), _T("DIM"))));
					AcObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentMiddle));
					AcObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)p);
                    ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("��ע�ߴ�"), _T("DIM")));
				}
				else
				{
					//>2�ַ������˶���
					p.SetX(p[0] - 2.5);
					p.SetY(p[1] - 1.5);
					AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sZU), (LPVARIANT)p, &_variant_t((double)3)).pdispVal;
                    AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("��ע�ߴ�"), _T("DIM"))));
					AcObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentFit));
					p.SetX(p[0] + 5);
					p.SetY(p[1]);
					AcObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)p);
                    ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("��ע�ߴ�"), _T("DIM")));
				}
			}
		}
		
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(COleException *e )
	{
		e->ReportError();
		e->Delete();
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void EDIBAcad::DrawphsDimOfDW(CCadPoint& InsPnt, CMObject& AcObject, long iView)
{
	// Ŀ��:��ע�ܵ��⾶�ߴ硣
	try
	{
		IDispatch* tmpvar;
		//��������
		CCadPoint p, px, p2, p3;
		CCadPoint p1;
		long i=0,LB=0,UB=0;
		CString sV , sZU ;
		long n=0;
		CMObject tmpObj;
		//�����ܲ����ԣ�
		//������ڹܲ����ԣ�
		//����ܲ�������DW��־���ǹܲ��⾶
		bool bFoundDW=0; //�ҵ��ܾ����Ա�־
		bool bFoundDWB=0; //�ҵ���ֱ�ܾ����Ա�־
		CString sTmp;
		
		if((long)AcObject.GetPropertyByName(_T("EntityType")) == acBlockReference)
		{
			if( (bool)AcObject.GetPropertyByName(_T("HasAttributes")))
			{
				AcATTR = COleSafeArray(AcObject.Invoke0(_T("GetAttributes")).parray,VT_VARIANT);
				p1 = AcObject.GetPropertyByName(_T("insertionPoint"));
				AcATTR.GetLBound(1,&LB);
				AcATTR.GetUBound(1,&UB);
				long ix=0;

				for(i=LB;i<=UB;i++)
			{
				AcATTR.GetElement(&ix,&tmpvar);
				tmpObj=tmpvar;
				sTmp=vtos(tmpObj.GetPropertyByName(_T("TagString")));
				sTmp.MakeUpper();
				if(sTmp==_T("DWB"))
				{
					//�Ǵ�ֱ����Ĺܾ�DwB,��ֱ��ע
					bFoundDWB = true;
					tmpObj.PutPropertyByName(_T("TextString"),&_variant_t(modPHScal::dwB));
					tmpObj.PutPropertyByName(_T("Visible"),&_variant_t(true));
					p=tmpObj.GetPropertyByName(_T("insertionPoint"));
					//MsgBox p(n)
					//����AutoCAD R14��BUG��ActiveX���ɵĿ�����λ��������ԭ�㣬
					//�������ֹ��������������������
					//�����˹��޸����Բ����;
					//�������Բ���λ����������ԭ��Ϊ����ġ�
					if(gbACADR14)
					{
						p2.SetPoint(p[0] + p1[0],p[1] + p1[1],p[2] + p1[2]);
						tmpObj.PutPropertyByName(_T("insertionPoint"),(LPVARIANT)p2);
						tmpObj.Invoke0(_T("Update"));
					}
					break;
				}
				ix++;
			}
			
			ix=0;
			for(i=LB;i<=UB;i++)
			{
				AcATTR.GetElement(&ix,&tmpvar);
				tmpObj=tmpvar;
				sTmp=vtos(tmpObj.GetPropertyByName(_T("TagString")));
				sTmp.MakeUpper();
				if (sTmp==_T("DW") )
				{
					//�ǹܾ�,ˮƽ��ע
					bFoundDW = true;
					//����/Բ�ιܵ�
					tmpObj.PutPropertyByName(_T("TextString"),&_variant_t((bFoundDWB ? ftos(modPHScal::dj) : CString(_T("\%\%C")) + ftos(modPHScal::dj))));
					tmpObj.PutPropertyByName(_T("Visible"),&_variant_t(true));

					p = tmpObj.GetPropertyByName(_T("insertionPoint"));
					//MsgBox p(n)
					//����AutoCAD R14��BUG��ActiveX���ɵĿ�����λ��������ԭ�㣬
					//�������ֹ��������������������
					//�����˹��޸����Բ����;
					//�������Բ���λ����������ԭ��Ϊ����ġ�
					if(gbACADR14)
					{
						p2.SetPoint(p[0] + p1[0],p[1] + p1[1],p[2] + p1[2]);
						tmpObj.PutPropertyByName(_T("insertionPoint"),(LPVARIANT)p2);
						tmpObj.Invoke0(_T("Update"));
					}
					break;
				}
				ix++;
			}
		}
   }
   
   if(!bFoundDW)
   {
      //���������ܲ�����ʵ�壬�ҵ�һ��Բ����Ϊ���ǹܵ�
      //ֱ�Ӹ�����ֱ���ߴ��ע
      float mvA, mvR; //ֱ���ߴ���X������ļн�/�뾶
      mvA = 165.0 / 180.0 * atan(1.0) * 4;   //135degree
      CMObject ent , ent1;
      CMObject PAblk;
      long PAblkCount;
      if((long)AcObject.GetPropertyByName(_T("EntityType")) != acBlockReference)
		  return;
		sTmp=vtos(AcObject.GetPropertyByName(_T("Name")));
		tmpObj=objAcadDoc.GetPropertyByName(_T("Blocks")).pdispVal;
		PAblk=tmpObj.Invoke(_T("Item"),1,&_variant_t(sTmp)).pdispVal;
      PAblkCount = (long)PAblk.GetPropertyByName(_T("Count"));
      for( i = 0 ;i< PAblkCount;i++)
	  {
         ent = PAblk.Invoke(_T("Item"),1,&_variant_t((long)i)).pdispVal;
         //MsgBox ent.EntityName
		 sTmp=vtos(ent.GetPropertyByName(_T("EntityName")));
		 sTmp.MakeUpper();
         if(sTmp==_T("ACDBCIRCLE"))
		 {
            mvR = (double)ent.GetPropertyByName(_T("radius"));
            if( mvR > 2.5)
			{
               //ֻ��ֱ������5mm��Բ���ǹܵ�
               //��ߴ����ֽϽ��ĵ�
               //The WCS coordinate for the first diameter point on the circle or arc.
               px.SetPoint(
					InsPnt[0] + mvR * cos(mvA + atan(1.0) * 4),
					InsPnt[1] + mvR * sin(mvA + atan(1.0) * 4));
               //��ߴ����ֽ�Զ�ĵ�
               //The WCS coordinate for the second diameter point on the circle or arc.
               p2.SetPoint(
					InsPnt[0] + mvR * cos(mvA),
					InsPnt[1] + mvR * sin(mvA));
               ent1 = MoSpace.Invoke(_T("AddDimDiametric"),3,(LPVARIANT)p2, (LPVARIANT)px, &_variant_t((long)1)).pdispVal;
               //�ܾ�
               ent1.PutPropertyByName((gbACADR14 ? _T("TextString") : _T("TextOverride")),&_variant_t(CString(_T("\%\%C")) + ftos(modPHScal::dj)));
               ent1.Invoke0(_T("Update"));
               break;
            }
         }
      }
   }
   
   }
   catch(_com_error e)
   {
	   ShowMessage(e.Description());
   }
   catch(COleException * e)
   {
	   e->ReportError();
	   e->Delete();
   }
	catch(CException *e)
	{
		e->Delete();
	}
}

void EDIBAcad::DrawphsDataEdit()
{
	try
	{
		_RecordsetPtr rsY;
		rsY.CreateInstance(__uuidof(Recordset));
		CCadPoint pnt;
		CCadPoint tempPoint;
		CCadPoint Point;
		CCadPoint Point1;
		CString sHandle , tmpFD ;
		bool bFound=false;
		long iZdjhInDwg=0;
		int i=0, k=0, j =0;
		bool bSSExists=false;
		
		GetActiveAcadDoc();
		CMObject UtilObj ;
		UtilObj=objAcadDoc.GetPropertyByName(_T("Utility")).pdispVal;
		ShowMessage(GetResStr(IDS_PickAsingleItemOnAutoCADscreen));
		EDIBAcad::DisplayAcadTop();
		pnt = UtilObj.Invoke(_T("GetPoint"),2,(LPVARIANT)tempPoint,&_variant_t(GetResStr(IDS_PickAnObjectInACAD)));
		tempPoint=pnt;
		Point1 = UtilObj.Invoke(_T("TranslateCoordinates"),4,(LPVARIANT)tempPoint, &_variant_t((long)0), &_variant_t((long)1), &_variant_t((long)0));
		Point=Point1;
   
		//On Error Resume Next

		sset = SetSelectionSet(_T("SS1")).pdispVal;
      
		iZdjhInDwg = (long)objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("USERI5")));
		if(iZdjhInDwg <= 0 )
		{
			ShowMessage(GetResStr(IDS_NotSetZDJHinDWG));
			return;
		}
   
		//֧���������ϼ�¼��TZB
		if(!IsWindow(FrmDataEDIT.m_hWnd))
			FrmDataEDIT.Create(IDD_DATAEDIT,AfxGetApp()->m_pMainWnd);
		CString SQLx = _T("SELECT recno,SEQ,CLmc,CLgg,CLcl,sizeDIM,CLnum,CLdz,CLzz,bUserAdd,Index,L1,ClassID FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID)+ _T(" AND zdjh=") + ltos(iZdjhInDwg) + _T(" AND IsSAPart<>-1 ORDER BY recno");
		
		FrmDataEDIT.m_DBGrid1.SetRefDataSource(NULL);
		if(FrmDataEDIT.m_Data1==NULL)
			FrmDataEDIT.m_Data1.CreateInstance(__uuidof(_Recordset));
		else if(FrmDataEDIT.m_Data1->State==adStateOpen)
			FrmDataEDIT.m_Data1->Close();
		FrmDataEDIT.m_Data1->CursorLocation=adUseClient;
		FrmDataEDIT.m_Data1->Open(_variant_t(SQLx),(IDispatch*)::conPRJDB,adOpenStatic,adLockOptimistic,adCmdText);
		//ͼ�����ݼ�¼��Rsy=TZG
		FrmDataEDIT.m_DBGrid1.SetRefDataSource(FrmDataEDIT.m_Data1->GetDataSource());
		EDIBDB::RefreshGrid(FrmDataEDIT.m_DBGrid1,FrmDataEDIT.m_Data1);
		EDIBgbl::SetDBGridColumnCaptionAndWidth(FrmDataEDIT.m_DBGrid1,_T("tzx"));
		FrmDataEDIT.m_DBGrid1.GetColumns().GetItem(_variant_t(_T("bUserAdd"))).SetVisible(FALSE);
		FrmDataEDIT.m_DBGrid1.GetColumns().GetItem(_variant_t(_T("Index"))).SetVisible(FALSE);

		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZG] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(iZdjhInDwg) + _T(" AND IsSAPart<>-1 ORDER BY recno");
// 		rsY.m_pDatabase=&EDIBgbl::dbPRJDB;
// 		rsY.Open(dbOpenDynaset,SQLx);
		rsY->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
		if(!FrmDataEDIT.IsWindowVisible())
			FrmDataEDIT.ShowWindow(SW_SHOW);
		CMObject tmpObj;
		sset.Invoke(_T("SelectAtPoint"),1,(LPVARIANT)Point);
		int c=(long)sset.GetPropertyByName(_T("Count"));
		CString sTmp;
		for(i = 0 ;i<c;i++)
		{
			tmpObj=sset.Invoke(_T("Item"),1,&_variant_t((long)i)).pdispVal;
			sHandle = vtos(tmpObj.GetPropertyByName(_T("Handle")));
			if(!rsY->BOF && !rsY->adoEOF)
			{
				_variant_t vTmp;
				CString strSql = CString(_T("ucase(trim(Handle))=\'")) + sHandle + _T("\'");
				rsY->Find((_bstr_t)strSql, 0, adSearchForward, vTmp);
				if(!rsY->adoEOF)
				{
					sTmp=vtos(::GetFields(rsY,_T("recno")));
					FrmDataEDIT.m_Data1->Find(_bstr_t(CString(_T("recno="))+sTmp),0,adSearchForward);
				}
			}
		}
		SetWindowPos(FrmDataEDIT.m_hWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(CException *e)
	{
		e->ReportError();
		e->Delete();
	}
}

void EDIBAcad::DisplayDataZB()
{
   try
	{
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		CString SQLx;
		//SQLx.Format(_T("UPDATE [ZB] SET [Zdjh]=[zdjh] WHERE [VolumeID]=%d AND [ZDJH]=%d"),EDIBgbl::SelVlmID,modPHScal::zdjh);
		//EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx);
		//֧���������ϼ�¼��TZB
		if(modPHScal::zdjh==0) {modPHScal::zdjh=1;}//pfg20050228
		SQLx = _T("SELECT zdjh,recno,nth,SEQ,CLmc,CLgg,CLcl,sizeDIM,CLnum,CLdz,CLzz,bUserAdd,Index,L1,sizeH,ClassID FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(modPHScal::zdjh) + _T(" AND (IsSAPart<>-1 OR bUserAdd=-1 ) ORDER BY bUserAdd DESC, nth,recno");
		if(FrmDataEDIT.m_Data1==NULL)
			FrmDataEDIT.m_Data1.CreateInstance(__uuidof(Recordset));
		if(FrmDataEDIT.m_Data1->State!=adStateClosed)
			FrmDataEDIT.m_Data1->Close();
		FrmDataEDIT.m_Data1->CursorLocation=adUseClient;
		FrmDataEDIT.m_Data1->Open(_variant_t(SQLx),(IDispatch*)::conPRJDB,adOpenStatic,adLockOptimistic,adCmdText);
		
		SQLx = _T("SELECT SUM(CLzz) as SumWeight FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(modPHScal::zdjh) + _T(" AND (IsSAPart<>-1 OR bUserAdd=-1 )");
// 		rs.m_pDatabase=&EDIBgbl::dbPRJDB;
// 		rs.Open (dbOpenSnapshot,SQLx);
		rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
   
		//���ݼ�¼��Rsy=TZG
		//SQLx = _T("SELECT * FROM [") & TBNSelPrjSpec & Btype(TZG) & _T("] WHERE zdjh=") & zdjh & _T(" AND IsSAPart<>-1 ORDER BY recno")
		//Set rsY = dbPRJDB.OpenRecordset(SQLx, dbOpenDynaset)
		if(!IsWindow(FrmDataEDIT.m_hWnd))
		{
			FrmDataEDIT.Create(IDD_DATAEDIT,AfxGetApp()->m_pMainWnd);
			FrmDataEDIT.UpdateWindow();
		}
		//if(!FrmDataEDIT.IsWindowVisible())
		//	FrmDataEDIT.ShowWindow(SW_SHOW);
		//FrmDataEDIT.Caption = _T("��ǰ֧���������ϸ��:֧���ܺ�=") & zdjh
		CString sTmp,sTmp1;
		COleVariant vTmp;
		rs->get_Collect((_variant_t)_T("SumWeight"),&vTmp);
		sTmp1.Format(_T("%0.3f"),vtof(vTmp));
		sTmp.Format(GetResStr(IDS_CurrentPhsBOMzdjh),ltos(modPHScal::zdjh),sTmp1);
		FrmDataEDIT.SetWindowText(sTmp);
		SetWindowPos(FrmDataEDIT.m_hWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		//FrmDataEDIT.SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(CException *e)
	{
		e->ReportError();
		e->Delete();
	}
}

void EDIBAcad::Data2DWG(int /*ByVal*/ recno, CString /*ByVal*/ FieldName)
{
}

void EDIBAcad::DrawTag(
					   int iRecNo, 
					   CCadPoint& p0, 
					   CCadPoint& p1, 
					   long  iSEQ, 
					   CString  sLine, 
					   CString  sLayer)
{
	try
	{
		CString sCLayer;
		
		//���浱ǰ��
		sCLayer = vtos(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("CLayer"))));
		
		//�л���ǰ��
		MakeNewLayer(sLayer);
		
		MoSpace = objAcadDoc.GetPropertyByName(_T("ModelSpace")).pdispVal;
		

		double txtHeight = modPHScal::gfSeqTxtHeight;//������ָ߶�3mm
		CCadPoint txtP;

		CVariantArray p2(4);
		
	
		sLine.TrimLeft();sLine.TrimRight();
		sLine.MakeUpper();
		if( sLine == _T("LINE"))
		{
			//���ָ����(б��)���p0/�յ�p1
			//�����ָ���ߣ�б�ߣ��յ�����p1д��TZG��
			//���������p0�Ѿ���DrawTagPoint�ӳ�����д�뵽TZG��
			AcObj = MoSpace.Invoke(_T("AddLine"),2,(LPVARIANT)p0, (LPVARIANT)p1).pdispVal;

			GetBoundingBox(AcObj,MinPoint,MaxPoint);
			OrdinateBound();

			p2.SetData(4,
				p1[0],
				p1[1],
				p1[0] + 4 * (p0[0] <= p1[0] ? 1 : -1), //�ֶ̺��߳���4mm
				p1[1]);
			AcObj = MoSpace.Invoke(_T("AddLightWeightPolyline"),1,(LPVARIANT)p2).pdispVal;
			//��һ���߶�������=0��AutoCAD help���ܴ��ˡ�
			AcObj.Invoke(_T("SetWidth"), 3,&_variant_t((long)0), &_variant_t((double)0.5),&_variant_t((double)0.5));
			
			GetBoundingBox(AcObj,MinPoint,MaxPoint);
			OrdinateBound();

			//д�������
			txtP.SetPoint(
				p1[0] + 2 * (p0[0] <= p1[0] ? 1 : -1),
				p1[1] + 1,
				0);			
			AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(ltos(iSEQ)), (LPVARIANT)txtP, &_variant_t((double)txtHeight)).pdispVal;
            AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
            ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));
			GetBoundingBox(AcObj,MinPoint,MaxPoint);
			OrdinateBound();

			AcObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentCenter));
			AcObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)txtP);
		}
		else if ( sLine == _T("CIRCLE") )
		{
			//�������ˮƽ�߼н�fSita
			float fSita;
			double d=p1[0]-p0[0];
			if (fabs(p1[0]-p0[0])<=0.001)
				fSita=0;
			else
				fSita= atan((p1[1]-p0[1])/(p1[0]-p0[0]));

			CCadPoint p11;//ָ��������ԲȦ�Ľ��㣬���ӳ���ͨ�����ԲȦԲ��
			p11.SetPoint(
				p1[0]+modPHScal::gfSeqCircleDiameter/2.0*cos(fSita),
				p1[1]+modPHScal::gfSeqCircleDiameter/2.0*sin(fSita),
				0);
			
			CCadPoint ptCenter;
			ptCenter.SetPoint(
				p1[0],
				p1[1],
				0);
			//�����ԲȦ
			AcObj = MoSpace.Invoke(_T("AddCircle"),2,(LPVARIANT)ptCenter,&_variant_t(modPHScal::gfSeqCircleDiameter/2.0)).pdispVal;

			GetBoundingBox(AcObj,MinPoint,MaxPoint);
			OrdinateBound();
			//���ָ����(б��)���p0/�յ�p11
			//�����ָ���ߣ�б�ߣ��յ�����p11
			//���������p0�Ѿ���DrawTagPoint�ӳ�����д�뵽TZG��
			AcObj = MoSpace.Invoke(_T("AddLine"),2,(LPVARIANT)p0, (LPVARIANT)p11).pdispVal;

			GetBoundingBox(AcObj,MinPoint,MaxPoint);
			OrdinateBound();

			//д�������
			txtP.SetPoint(
				ptCenter[0],
				ptCenter[1]-txtHeight/2,
				//p1[0] + 2 * (p0[0] <= p1[0] ? 1 : -1),
				//p1[1] + 1,
				0);			
			AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(ltos(iSEQ)), (LPVARIANT)txtP, &_variant_t((double)txtHeight)).pdispVal;
            AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
            ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));
			GetBoundingBox(AcObj,MinPoint,MaxPoint);
			OrdinateBound();

			AcObj.PutPropertyByName(_T("HorizontalAlignment"),&_variant_t((long)acHorizontalAlignmentCenter));
			AcObj.PutPropertyByName(_T("TextAlignmentPoint"),(LPVARIANT)txtP);
		}


		//�ָ���ǰ��
		objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("CLayer")), &_variant_t(sCLayer));
	}
	catch(CMemoryException& e)
	{
		e.ReportError();
		e.Delete();
	}
	catch(COleException& e)
	{
		e.ReportError();
		e.Delete();
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void EDIBAcad::DrawTagByPickEntity(COleVariant /*Optional*/ pnt)
{
}

void EDIBAcad::DrawTagAll()
{
	//Ŀ�ģ�һ���Ի������������š����ڻ�����װͼ���ʱ���Ʊ�š�
	try
	{

		//�����Ƿ�����ظ��ļ��� ��������Ҫ������ã�
		if(!EDIBAcad::g_bDimNumber)
		{
// 			 rsRefZB(&EDIBgbl::dbPRJDB);
			_RecordsetPtr rsRefZB;
			rsRefZB.CreateInstance(__uuidof(Recordset));
			CString SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 ORDER BY recno");
// 			rsRefZB.Open(dbOpenDynaset,SQLx );
			rsRefZB->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
			if( rsRefZB->adoEOF || rsRefZB->BOF )
			{
			}
			else
			{
				int i = 1;
				CString str;
				while( i < 20 )
				{
					rsRefZB->MoveFirst();
					str.Format(_T("seq=%d"), i);
					_variant_t vTmp;
					rsRefZB->Find((_bstr_t)str, 0, adSearchForward, vTmp);
					while( !rsRefZB->adoEOF )
					{
//							rsRefZB.Edit();
						rsRefZB->put_Collect((_variant_t)_T("seq"),COleVariant());
						rsRefZB->Update();

						rsRefZB->Find((_bstr_t)str, 0, adSearchForward, vTmp);

					}

					i++;
				}

			}

			rsRefZB->Close();
		}

		CCadPoint p0,p1,p2;
		CVariantArray p3(12);				//Crossingpolygonѡ�񼯵���
		long iRecNo=0, iSEQ=0;
		long iMaxSeq=0;
// 		 rsTZG(&EDIBgbl::dbPRJDB);
// 		 rsRefZB(&EDIBgbl::dbPRJDB);
		_RecordsetPtr rsTZG;
		rsTZG.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rsRefZB;
		rsRefZB.CreateInstance(__uuidof(Recordset));
		COleVariant vTmp1,vTmp2,vTmp3;
		long i =0, k=0;
		long j =0;
		float p2y=0;    //������ı��ָ�����յ�
		const long xOffset = 28;   //�ߴ���x����ƫ��
		const long SEQyOffset = 1; //�����������ֹ�ص�ʱ����������(1mm,y����)
		//long modPHScal::gnSEQyOffset = 15;    //���֮�����С��ֱ����
		if(modPHScal::gnSEQyOffset<6) 
			modPHScal::gnSEQyOffset=6;
		CString SQLx;
		
		//ѡ��֧���ܽṹ��¼��rsRefZB
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 ORDER BY recno");
// 		rsRefZB.Open(dbOpenDynaset,SQLx );
		rsRefZB->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
		if( rsRefZB->adoEOF || rsRefZB->BOF )
		{
			//Err.Raise
		}
		else
		{
			//ѡ���¼��rsTZG�Ա�ȷ�����ָ����������꣬ͬʱ��¼ͼ������
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZG] ;
			SQLx+=_T("] WHERE zdjh=");
			SQLx+=ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND  ucase(trim(Layer))=\'TAG\' AND trim(EntityName)=\'AcDbPoint\' AND ucase(trim(FieldName))=\'SEQ\' ORDER BY y");
// 			rsTZG.Open(dbOpenDynaset,SQLx);
			rsTZG->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
			if( rsTZG->adoEOF || rsTZG->BOF )
			{
			}
			else
			{
				//xOffset += maxX;

				rsTZG->MoveFirst();
				while(!rsTZG->adoEOF)
				{
					i++;
					//���ָ�������
					rsTZG->get_Collect((_variant_t)_T("x"),&vTmp1);
					rsTZG->get_Collect((_variant_t)_T("y"),&vTmp2);
					rsTZG->get_Collect((_variant_t)_T("z"),&vTmp3);
					p0.SetPoint(vtof(vTmp1),vtof(vTmp2),vtof(vTmp3));
					p1 = p0;
					//���ָ�����յ�
					//�߶��յ�x����
					if( i == 1 )
					{
						//����ǻ���������ı�ţ����ס�����յ�x���꣬�Ա���롣
						p2.SetPoint(p0[0] - xOffset,p0[1] - 5,0);
					}
					else
					{
						for(k = 1 ;k<= 20;k++)
						{
							//Y����ÿ������λ��modPHScal::gnSEQyOffset(һ��=8mm)
							p2y = p2[1] + modPHScal::gnSEQyOffset * k;
							//������ָ���߽Ƕ�
							if( atan(fabs((p2y - p1[1]) / xOffset)) * 180 / atan(1.0) / 4 < 10 )
							{
								//���ָ���߽Ƕ�̫ƽ
								p2y = p1[1] + Sgn(p2y - p1[1]) * xOffset * atan(atan(1.0) * 4 / 180 * 10);
								if( p2y >= p2[1] + modPHScal::gnSEQyOffset )
								{
									//���ָ���߽Ƕ�����
									p3.SetData(12,
										p0[0],
										p2y,
										0,
										
										p2[0],
										p2y,
										0,
										
										p2[0],
										p2y + 3,
										0,
										
										p0[0],
										p2y + 3,
										0);
									if( !bExistEntityInSelectionSet(p3) )
									{
										//û���ص��������˳�������߼�����
										break;
									}
									else
									{
										//����߶�̧��SEQyOffset(1mm),����
										p2.SetY(p2[1] + SEQyOffset);
									}
								}
								else
								{
									if( k == 20 )
									{
										//�ﵽ�������޴�������ʱ�Ѿ��޷������Ƕ�>10
										p2y = p2[1] + Sgn(p2y - p1[1]) * modPHScal::gnSEQyOffset;
									}
									else{
										//���ɼ���ѭ��Ѱ��
									}
								}
							}
							else
							{
								if( atan(fabs((p2y - p1[1]) / xOffset)) * 180 / atan(1.0) / 4 > 50 )
								{
									//���ָ���߽Ƕ�̫��
									p2y = p1[1] + Sgn(p2y - p1[1]) * xOffset * tan(atan(1.0) * 4 / 180 * 50);
									if( p2y >= p2[1] + modPHScal::gnSEQyOffset )
									{
										//���ָ���߽Ƕ�����
										p3.SetData(12,
											p0[0],
											p2y,
											0,
											
											p2[0],
											p2y,
											0,
											
											p2[0],
											p2y + 3,
											0,
											
											p0[0],
											p2y + 3,
											0);
										if( ! bExistEntityInSelectionSet(p3) ){
											//û���ص��������˳�������߼�����
											break;
										}
										else
										{
											//����߶�̧��SEQyOffset(1mm),����
											p2.SetY(p2[1] + SEQyOffset);
										}
									}
									else{
										if( k ==  20 ){
											//�ﵽ�������޴�������ʱ�Ѿ��޷������Ƕ�<60
											p2y = p2[1] + Sgn(p2y - p1[1]) * modPHScal::gnSEQyOffset;
										}
										else{
											//���ɼ���ѭ��Ѱ��
										}
									}
								}
								else
								{
									//���ָ���߽Ƕ�����
									if( p2y >= p2[1] + modPHScal::gnSEQyOffset )
									{
										//���ָ���߽Ƕ�����
										p3.SetData(12,
											p0[0],
											p2y,
											0,
											
											p2[0],
											p2y,
											0,
											
											p2[0],
											p2y + 3,
											0,
											
											p0[0],
											p2y + 3,
											0);
										if( ! bExistEntityInSelectionSet(p3) ){
											//û���ص��������˳�������߼�����
											break;
										}else
										{
											//����߶�̧��SEQyOffset(1mm),����
											p2.SetY(p2[1] + SEQyOffset);
										}
									}
								}
							}
						}//16
						//����y����
						p2.SetY(p2y);
					}
					p2.SetZ(p0[2]);
					//�ҳ����
					COleVariant vTmp;
					rsTZG->get_Collect((_variant_t)_T("recno"),&vTmp);
					iRecNo = vtoi(vTmp);

					rsRefZB->Find((_bstr_t)(_T("recno=") + ltos(iRecNo)), 0, adSearchForward, vTmp);
					if( !rsRefZB->adoEOF)
					{
						rsRefZB->get_Collect((_variant_t)_T("seq"),&vTmp1);
						if(vTmp1.vt==VT_NULL)
						{
							//���Ϊ�գ������Ʊ��
						}
						else
						{
							iSEQ = vtoi(vTmp1);
							if(iSEQ>iMaxSeq)
								iMaxSeq=iSEQ;
							//���Ʊ��
							//dMaxX = max( p2[0],oldp2X );
							DrawTag(iRecNo, p1, p2, iSEQ,
								(EDIBAcad::g_bTagUsingCircle?"CIRCLE":"LINE") );
							//oldp2X = p2[0];
						}
					}

					rsTZG->MoveNext();
				}
			}
		}
		//dMaxX=dMaxX;
		SetRegValue(_T("Settings"),_T("TagNO"),iMaxSeq);
    }
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
	catch(COleException *e )
	{
		e->ReportError();
		e->Delete();
	}
	catch(CException *e)
	{
		e->ReportError();
		e->Delete();
	}
}

void EDIBAcad::DrawTagAuto(double p0(), int /*ByVal*/ iRecNo, int /*ByVal*/ iSEQ)
{
}

void EDIBAcad::DrawTagPoint(CCadPoint& p0, int iRecNo, int iSEQ, CString sLayer)
{
	//Ŀ�ģ����Ʊ��ָ�������СԲ�㡣�����Ϊ��װͼ������ɺ���Ʊ�ŵĻ�׼��㡣
	
	CString sCLayer;
	try
	{
		//On Error Resume Next
		CCadPoint p1, p2;
// 		 rsTZG(&EDIBgbl::dbPRJDB);
		_RecordsetPtr rsTZG;
		rsTZG.CreateInstance(__uuidof(Recordset));
		CString SQLx;
		
		CString sCLayer;
		sCLayer = vtos(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("CLayer"))));
		MakeNewLayer(sLayer);
		MoSpace = objAcadDoc.GetPropertyByName(_T("ModelSpace")).pdispVal;
		//���ָ�������
		AcObj = MoSpace.Invoke(_T("AddPoint"),1,(LPVARIANT)p0).pdispVal;
		
		//ѡ���¼��rsTZG�Ա��¼ͼ������
		SQLx = _T("SELECT * FROM [")+EDIBgbl::Btype[EDIBgbl::TZG] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND  ucase(trim(Layer))=\'TAG\' AND trim(EntityName)=\'AcDbPoint\' AND ucase(trim(FieldName))=\'SEQ\' ORDER BY recno");
// 		rsTZG.Open(dbOpenDynaset,SQLx);
		rsTZG->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJDB,adOpenForwardOnly,adLockReadOnly,adCmdText);
		
		AddData2rsTZG(rsTZG, iRecNo, modPHScal::zdjh, _T("seq"), _T("X"), _T("POINT"), p0);
		//�ָ�ԭ����
		MakeNewLayer(sCLayer);
		
	}
	catch(CException *e)
	{
		e->Delete();
		MakeNewLayer(sCLayer);
	}
}

bool EDIBAcad::AddEntities2SSetInLayers(int  Mode, int Count,...)
{
	//Mode--ѡ��ģʽ��acSelectionSetAll,ѡ��ȫ��;
	va_list argList;
	va_start(argList,Count);
	bool ret;
	if(!gbACADR14 /*|| gsACADVER!=_T("14.0")*/)
	{
		ret=SelectLayerV(Count,argList);
	}
	else
	{
		ret=SelectLayerAllV(Count,argList);
	}
	va_end(argList);
	return ret;
}

bool EDIBAcad::DeleteAllEntitiesInLayers(int Count,...)
{
	try
	{
		CMObject tmpObj;
		va_list argList;
		va_start(argList,Count);
		bool ret;
		if(!gbACADR14 /*|| gsACADVER!=_T("14.0")*/)
		{
			ret=SelectLayerV(Count,argList);
		}
		else
		{
			ret=SelectLayerAllV(Count,argList);
		}
		va_end(argList);
		if(ret==false)
			return false;
		int count=(long)sset.GetPropertyByName(_T("Count"));
		sset.Invoke0( _T("Erase"));
		tmpObj=objAcadDoc.GetPropertyByName(_T("ActiveViewport")).pdispVal;
		tmpObj.Invoke0(_T("ZoomAll"));
		return true;
	}
	catch(CException *e)
	{
		e->Delete();
		return false;
	}
}

void EDIBAcad::EraseAllEntities()
{
	try
	{
		if(!gbACADR14 && gsACADVER.Left(2)!=_T("14"))
		{
			objAcadDoc.Invoke(_T("SendCommand"),1,&_variant_t(_T("_.e\rall\r\r")));
			return;
		}
		CMObject lagers,lager;
		lagers=objAcadDoc.GetPropertyByName(_T("Layers")).pdispVal;
		int C=(long)lagers.GetPropertyByName(_T("Count"));
		for(int i = 0 ;i<C;i++)
		{
			lager = lagers.Invoke(_T("Item"),1,&_variant_t((long)i)).pdispVal;
			CString sLayer=vtos(lager.GetPropertyByName(_T("Name")));
			DeleteAllEntitiesInLayers(1,sLayer);
		}
	}
	catch(_com_error e)
	{
		ShowMessage(e.Description());
	}
}

_variant_t EDIBAcad::SetSelectionSet(CString /*ByVal*/ ssetName)
{
	try
	{
		CString ss=ssetName;
		ssetName.TrimLeft();ssetName.TrimRight();
		ssetName.MakeUpper();
		CMObject see,si;
		see=objAcadDoc.GetPropertyByName(_T("SelectionSets")).pdispVal;
		int C=(long)see.GetPropertyByName(_T("Count"));
		CString siN;
		for (int i = 0 ;i<C;i++)
		{
			si=see.Invoke(_T("Item"),1,&_variant_t((long)i)).pdispVal;
			siN=vtos(si.GetPropertyByName(_T("Name")));
			siN.TrimLeft();siN.TrimRight();
			siN.MakeUpper();
			if(siN==ssetName)
			{
				si.Invoke0(_T("Clear"));
				return si;
			}
		}
		return see.Invoke(_T("Add"),1,&_variant_t(ss));
	}
	catch(CException *e)
	{
		e->Delete();
		return _variant_t();
	}
}

//void EDIBAcad::VBSetXdata(Object ent, COleVariant /*ByVal*/ vbApp, COleVariant ParamArray xData())
//{
//}

//bool EDIBAcad::VBGetXdata(Object ent, CString /*ByVal*/ vbApp, int xdataType(), COleVariant xData())
//{
//}

//bool EDIBAcad::VBGetEXdata(Object ent, CString /*ByVal*/ vbApp, COleVariant /*Optional*/ IxdataType, COleVariant /*Optional*/ VxData)
//{
//}

void EDIBAcad::WblockFromPhsToPhsBlkDir(CString /*ByVal*/ DestBlkDir)
{
}

CString EDIBAcad::GetDrawIDAndName(long lngSEQ, CString& strDrawName)
{
	CString sTmp,sTmp1,sTmp2,sTmp3,sTmp4;
	CString sTmpDrawNo, sTmpDrawNa;
	CString sTmpDrawNaEnglish;
	_RecordsetPtr rs;
	rs.CreateInstance(__uuidof(_Recordset));
	COleVariant tmpvar;
	CString strExecute;
	//�����Ŀ¼��Ϣ֮ǰ������һ����ʱ���Ա�����ͼ����ͼ�ŵ���Ϣ��
	try
	{
		if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("tmpDrawNameAndDrawNo")))
			//�����ȽϷ��㣬���Ұ汾����ʱ�����Ժá�
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE tmpDrawNameAndDrawNo"), NULL, adCmdText);

		//������,������ʱ��tmpDrawNameAndDrawNo,׼�����ͼֽ��ź�ͼֽ����

		strExecute = _T("CREATE TABLE tmpDrawNameAndDrawNo (PhsTypeName char(255),PhsTypeChineseName char(255),PhsTypeNameEnglish char(255),CompanyID char(50),gcdm CHAR (50),sjjddm char(50),zydm char(20),jcdm char(50),Version char(50),seq LONG,zdjh LONG)");
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

		strExecute = _T("INSERT INTO tmpDrawNameAndDrawNo (zdjh) VALUES (0)");
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

		if(!EDIBgbl::dbSORT->State != adStateOpen)
		{
// 			EDIBgbl::dbSORT.Open(basDirectory::ProjectDBDir+_T("sort.mdb"));
			CString ConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=" + basDirectory::ProjectDBDir+_T("sort.mdb");
			EDIBgbl::dbSORT->Open((_bstr_t)ConnectionString, "", "", adConnectUnspecified);
		}
		CString strSQL;
		if( EDIBAcad::g_bDrawNameWithoutNo )
		{//ͼ�Ų���֧���ܺ�
			strSQL.Format("UPDATE [DrawSize] SET PhsDrawNameFormat=\'trim(PhsTypeName)\'");
			EDIBgbl::dbDSize->Execute((_bstr_t)strSQL, NULL, adCmdText); //20071018 "dbSORT" ��Ϊ "dbDSize"
		}
		else
		{
			strSQL.Format("UPDATE [DrawSize] SET PhsDrawNameFormat=defaultPhsDrawNameFormat");
			EDIBgbl::dbDSize->Execute((_bstr_t)strSQL, NULL, adCmdText); //20071018 "dbSORT" ��Ϊ "dbDSize"
		}

		//��ʼ���ó�ʼ��Ϣ
		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET PhsTypeName=\'") + modPHScal::gsPhsName + _T("\'"));
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);
		CString m_strTmp;
		m_strTmp = modPHScal::gsPhsName +" ";
		m_strTmp.TrimLeft();
	
		CString strTmpSql; //= _T("UPDATE tmpDrawNameAndDrawNo SET PhsTypeChineseName=\'") + + _T("\'");
		if( m_strTmp.Find(' ') >= 0 )
			strTmpSql =  modPHScal::gsPhsName.Left(m_strTmp.Find(' '));	
		strTmpSql = _T("UPDATE tmpDrawNameAndDrawNo SET PhsTypeChineseName=\'") + strTmpSql + _T("\'");
		EDIBgbl::dbPRJ->Execute((_bstr_t)strTmpSql, NULL, adCmdText);
	
		if( m_strTmp.Find(' ') >= 0 )
			modPHScal::gsPhsEnglishName = m_strTmp.Mid(m_strTmp.Find(' '));
		modPHScal::gsPhsEnglishName.TrimLeft();
		modPHScal::gsPhsEnglishName.TrimRight();

		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET PhsTypeNameEnglish=\'") + modPHScal::gsPhsEnglishName + _T("\'"));
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET CompanyID=\'") + EDIBgbl::CompanyID + _T("\'"));
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET gcdm=\'") + EDIBgbl::SelPrjID + _T("\'"));
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

		strExecute = CString( _T("UPDATE tmpDrawNameAndDrawNo SET sjjddm=\'") + EDIBgbl::strSelDsgn + _T("\'"));
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET zydm=\'") + EDIBgbl::strSelSpec + _T("\'"));
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET Version=\'") + EDIBgbl::strDwgVersion + _T("\'"));
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET jcdm=\'") + EDIBgbl::SelJcdm + _T("\'"));
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);
		sTmp.Format(_T("%d"),modPHScal::zdjh);

		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET zdjh=") + sTmp);
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);
		sTmp.Format(_T("%d"),lngSEQ);

		strExecute = CString(_T("UPDATE tmpDrawNameAndDrawNo SET SEQ=") + sTmp);
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);
		
		
		CString SQLx;
		SQLx=_T("SELECT * FROM DrawSize WHERE sjhyIndex=") + ltos(EDIBDB::SJHYIndex);
// 		rs.m_pDatabase=&EDIBgbl::dbDSize;//20071018 "dbSort" ��Ϊ "dbDSize"
// 		rs.Open(dbOpenSnapshot,SQLx);
		rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbDSize,adOpenForwardOnly,adLockReadOnly,adCmdText);
		if(rs->BOF || rs->adoEOF)
		{
			if(rs->State == adStateOpen)
				rs->Close();
		}
		else
		{
			//���ͼֽ��Ÿ�ʽ
			rs->get_Collect((_variant_t)_T("DrawNoFormat"),&tmpvar);
			sTmpDrawNo=vtos(tmpvar);
			//���֧����ͼֽ�������Ƹ�ʽ
			rs->get_Collect((_variant_t)_T("PhsDrawNameFormat"),&tmpvar);
			sTmpDrawNa=vtos(tmpvar);
			//���֧����ͼֽӢ�����Ƹ�ʽ
			rs->get_Collect((_variant_t)_T("PhsDrawNameFormatEnglish"),&tmpvar);
			sTmpDrawNaEnglish=vtos(tmpvar);

			//��ô��ڱ��������ݸ�ʽ
			rs->get_Collect((_variant_t)_T("APHSCaptionFormat"),&tmpvar);
			sTmp1=vtos(tmpvar);
			//���ͼֽ���ǰ׺(����ͼֽ���)��ʽ
			rs->get_Collect((_variant_t)_T("DrawNoPreFormat"),&tmpvar);
			sTmp2=vtos(tmpvar);
			//���֧���ܱ�Ÿ�ʽ(���Ṥ����Ժ��ʽ��ʵ������ͼ��)
			rs->get_Collect((_variant_t)_T("VolumeFormat"),&tmpvar);
			sTmp3=vtos(tmpvar);
			//���֧���ܱ�Ÿ�ʽ(��No.1�ȸ�ʽ)
			rs->get_Collect((_variant_t)_T("PhsNoFormat"),&tmpvar);
			sTmp4=vtos(tmpvar);

			rs->get_Collect((_variant_t)_T("A0PrjNameWidth"),&tmpvar);
			iA0PrjNameWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A2PrjNameWidth"),&tmpvar);
			iA2PrjNameWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A0DrawNoWidth"),&tmpvar);
			iA0DrawNoWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A2DrawNoWidth"),&tmpvar);
			iA2DrawNoWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A0DrawNameWidth"),&tmpvar);
			iA0DrawNameWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A2DrawNameWidth"),&tmpvar);
			iA2DrawNameWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A0VlmNameWidth"),&tmpvar);
			iA0VlmNameWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A2VlmNameWidth"),&tmpvar);
			iA2VlmNameWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A0DateWidth"),&tmpvar);
			iA0DateWidth=vtoi(tmpvar);
			rs->get_Collect((_variant_t)_T("A2DateWidth"),&tmpvar);
			iA2DateWidth=vtoi(tmpvar);

			if(rs->State == adStateOpen)
				rs->Close();

			if(sTmpDrawNo.GetLength()>0  && sTmpDrawNa.GetLength()>0 && sTmpDrawNaEnglish.GetLength()>0 && sTmp1.GetLength()>0 && sTmp2.GetLength()>0 && sTmp3.GetLength()>0 && sTmp4.GetLength()>0)
			{
				EDIBgbl::SQLx = _T("SELECT ") + sTmpDrawNa + _T(" AS DrawNa,") + sTmpDrawNaEnglish + _T(" AS DrawNaEnglish,") + sTmpDrawNo + _T(" AS DrawNo,") + sTmp1 + _T(" AS APHSCaption,") + sTmp2 + _T(" AS DrawNoPreFmt,") + sTmp3 + _T(" AS VolumeNoFmt,") + sTmp4 + _T(" AS PhsNoFmt FROM tmpDrawNameAndDrawNo");
				//AfxMessageBox(EDIBgbl::SQLx);
// 				rs.m_pDatabase=&EDIBgbl::dbPRJ;
// 				rs.Open(dbOpenSnapshot,EDIBgbl::SQLx);
				rs->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbPRJ,adOpenForwardOnly,adLockReadOnly,adCmdText);
				//rs = EDIBgbl::dbPRJ.Execute  (_bstr_t(),&tmpvar,adCmdText);
				if( rs->BOF || rs->adoEOF)
				{
				//����ͼ����Ϣʧ��
				}
				else
				{   
					rs->get_Collect((_variant_t) _T("DrawNa"),&tmpvar);
					sTmpDrawNa =vtos(tmpvar);
					if(sTmpDrawNa.GetLength()<=0)
						sTmpDrawNa=_T("");
					else
					{
						sTmpDrawNa.TrimLeft();
						sTmpDrawNa.TrimRight();
					}

					rs->get_Collect((_variant_t) _T("DrawNaEnglish"),&tmpvar);
					sTmpDrawNaEnglish =vtos(tmpvar);
					if(sTmpDrawNaEnglish.GetLength()<=0)
						sTmpDrawNaEnglish=_T("");
					else
					{
						sTmpDrawNaEnglish.TrimLeft();
						sTmpDrawNaEnglish.TrimRight();
					}

					rs->get_Collect((_variant_t) _T("DrawNo"),&tmpvar);
					sTmpDrawNo = vtos(tmpvar);//_bstr_t(rs->GetCollect(_T("DrawNo")));
					if(sTmpDrawNo.GetLength()<=0)
						sTmpDrawNo=_T("");
					else
					{
						sTmpDrawNo.TrimLeft();
						sTmpDrawNo.TrimRight();
					}

					rs->get_Collect((_variant_t) _T("APHSCaption"),&tmpvar);
					sTmp1 = vtos(tmpvar);//_bstr_t(rs->GetCollect(_T("DrawNo")));
					if(sTmp1.GetLength()<=0)
						sTmp1=_T("");
					else
					{
						sTmp1.TrimLeft();
						sTmp1.TrimRight();
					}

					rs->get_Collect((_variant_t) _T("DrawNoPreFmt"),&tmpvar);
					sTmp2 = vtos(tmpvar);
					if(sTmp2.GetLength()<=0)
						sTmp2=_T("");
					else
					{
						sTmp2.TrimLeft();
						sTmp2.TrimRight();
					}

					rs->get_Collect((_variant_t) _T("VolumeNoFmt"),&tmpvar);
					sTmp3 = vtos(tmpvar);
					if(sTmp3.GetLength()<=0)
						sTmp3=_T("");
					else
					{
						sTmp3.TrimLeft();
						sTmp3.TrimRight();
					}
					EDIBgbl::VolumeNo=sTmp3;

					rs->get_Collect((_variant_t) _T("PhsNoFmt"),&tmpvar);
					sTmp4 = vtos(tmpvar);
					if(sTmp4.GetLength()<=0)
						sTmp4=_T("");
					else
					{
						sTmp4.TrimLeft();
						sTmp4.TrimRight();
					}
					EDIBgbl::PhsNo=sTmp4;
				}
			}
			strDrawName = sTmpDrawNa;

			DrawNameEnglish = sTmpDrawNaEnglish;
			//��ô��ڱ���������
			EDIBgbl::TBNSelPrjSpec=sTmp1;
			EDIBgbl::TBNSelPrjVlm=sTmp2;
			return sTmpDrawNo;
		}
	}
	catch(CException *e)
	{
		e->ReportError();
		e->Delete();
	}
	return _T("");
}

void EDIBAcad::DisplayAcadTop()
{
	//Ŀ��:��AutoCAD������ʾ�������档
	try
	{
	   //LocateFrm gObjMainFrm
	SetAcadTop();
	HWND hwnd = FindAcad();
	long nPos=0;
	   if(objAcadApp.GetActiveObject(_T(gstrApplicatonName))==S_OK)
	   {
		  GetAllMenu(GetSystemMenu(hwnd, false), nPos);
		  DisabledSystemMenu(hwnd, GetSystemMenu(hwnd, false));
		  //ʹ��objAcadApp��������
		  CRect rect,rc1;
		  ::GetClientRect(GetDesktopWindow(),&rect);
		  long atop=((CMainFrame*)::AfxGetApp()->m_pMainWnd)->GetMenuHeight();
		  ::SystemParametersInfo(SPI_GETWORKAREA,0,(LPVOID)&rc1,0);
		  long w=rc1.Width();
		  long h=rc1.Height()-atop;
			if( gbACADR14 )
			{
				objAcadApp.PutPropertyByName(_T("Top"),&_variant_t(atop));
				objAcadApp.PutPropertyByName(_T("Left"),&_variant_t((long)0));
				objAcadApp.PutPropertyByName(_T("Width"),&_variant_t(w));
				objAcadApp.PutPropertyByName(_T("Height"),&_variant_t(h));
			}
			else
			{
				objAcadApp.PutPropertyByName(_T("WindowTop"),&_variant_t(atop));
				objAcadApp.PutPropertyByName(_T("WindowLeft"),&_variant_t((long)0));
				objAcadApp.PutPropertyByName(_T("Width"),&_variant_t(w));
				objAcadApp.PutPropertyByName(_T("Height"),&_variant_t(h));
			}

		  //TraceMainFrm(hwnd);
	   }
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void EDIBAcad::OrdinateBound()
{
	//������ͼλ��
    
    if(MinPoint[0] < tmpBound[0])
        tmpBound[0]=MinPoint[0];
    
    if( MinPoint[1] < tmpBound[1])
        tmpBound[1]=MinPoint[1];
    
    if(MaxPoint[0] > tmpBound[2] )
        tmpBound[2]=MaxPoint[0];
    if( MaxPoint[1] > tmpBound[3])
        tmpBound[3]=MaxPoint[1];
   
}

void EDIBAcad::MoveObj(long Count,...)
{
	//Ŀ��:�ƶ�ĳЩ���ϵ�����ʵ�嵽�µ�λ�á�
	try
	{
		CCadPoint p1,p2;
		CMObject tmpObj;
		va_list argList;
		va_start(argList,Count);
		bool ret=false;
		if(!gbACADR14 /*|| gsACADVER!=_T("14.0")*/)
		{
			ret=SelectLayerV(Count,argList);
		}
		else
		{
			ret=SelectLayerAllV(Count,argList);
		}
		va_end(argList);
		if(ret==false)
			return ;
		//Debug.Print _T("sset.count="); sset.count
		//��p1�Ƶ�p2
		//p2.SetX(tmpBound[0]);
		//p2.SetY(tmpBound[1]);
		p1.SetX((tmpBound[0] + tmpBound[2]) / 2);
		p1.SetY((tmpBound[1] + tmpBound[3]) / 2);
		p2.SetX((Bound[0] + Bound[2]) / 2);
		p2.SetY((Bound[1] + Bound[3]) / 2);
		long C=(long)sset.GetPropertyByName(_T("count"));
		for( int i = 0 ;i<C;i++)
		{
			tmpObj=sset.Invoke(_T("Item"),1,&_variant_t((long)i)).pdispVal;
			tmpObj.Invoke(_T("Move"),2,(LPVARIANT)p1, (LPVARIANT)p2);
		}

		//����"ZoomAll"��AutoCAD��ͬ�汾��λ�ò�ͬ��
		//��AutoCAD R14������Viewport object�У�AutoCAD2000ȴ����Application object��
		if( EDIBAcad::gbACADR14 )
		{
			tmpObj=objAcadDoc.GetPropertyByName(_T("ActiveViewport")).pdispVal;
			tmpObj.Invoke0(_T("ZoomAll"));
		}
		else
		{
			objAcadApp.Invoke0("ZoomAll");
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
}



bool EDIBAcad::GetDisPid(IDispatch *lpDispatch, CString csName,DISPID* lpDispid)
{
	if(!lpDispatch || csName==_T(""))
		return FALSE;
	char* pName=(char*)LPCTSTR(csName);
	if(FAILED(lpDispatch->GetIDsOfNames(IID_NULL,
		(LPOLESTR*)&pName,
		1,
		LOCALE_SYSTEM_DEFAULT,
		lpDispid)))
		return FALSE;
	return TRUE;
}

void EDIBAcad::ModifyTextOfBG(CCadPoint &InsPnt, float ELvalue, CString BGtype,BOOL bBJBW,BOOL bPA)
{
	try
	{
		CCadPoint p0, p1;
		CString sTmp;
		const long txtHeight = 3;
		CADatt BlkAtt[2];// As CADatt
		//ȱʡΪ�ܲ����

		InsPnt.SetY(InsPnt[1]+0.5f);//Ϊʹ��ע������ʾ�������

		if( bBJBW )
			sTmp.Format("%.0f",ELvalue*1000);
		else
		{
			if(bPA)
				if( !g_bInstallElevation)  //��Ҫ��װ����
				{
					sTmp.Format ("%.3f",ELvalue);
				}
				else
				{
					sTmp.Format(IDS_INSTALL2,ELvalue);
				}
			else
				sTmp.Format("%.3f",ELvalue);
		}

		if( BGtype == _T("PEL") )
		{
			if( fabs(modPHScal::yr) > modPHScal::gUseSpringMinDist)
			{
				p0.SetPoint(
					InsPnt[0] + 3,
					InsPnt[1],
					InsPnt[2]);

				AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sTmp), (LPVARIANT)p0, &_variant_t(txtHeight)).pdispVal;
                AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
                ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));
				
				if( !bBJBW )		 
				{
					if(EDIBAcad::g_bColdHeatState)
					{
						sTmp.Format(IDS_COLE_FORM2,modPHScal::dyl / 1000);
						p0.SetY(p0[1] + 4);
						AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sTmp), (LPVARIANT)p0, &_variant_t(txtHeight)).pdispVal;
                        AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
                        ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));
						
						sTmp.Format(IDS_HOT_FORM2,modPHScal::dyr / 1000);
						p0.SetY(p0[1] + 4);
						AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sTmp), (LPVARIANT)p0, &_variant_t(txtHeight)).pdispVal;
                        AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
                        ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));
					}
					else
					{
						sTmp=_T("");
					}
				}
			}
			else
			{
				p0.SetPoint(
					InsPnt[0] + 3,
					InsPnt[1],
					InsPnt[2]);
				AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sTmp), (LPVARIANT)p0, &_variant_t(txtHeight)).pdispVal;
                AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
                ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));

				if( !bBJBW )		 
				{
					if(EDIBAcad::g_bColdHeatState)
					{
						sTmp.Format(IDS_COLE_FORM2,modPHScal::dyl / 1000);
						p0.SetY(p0[1] + 4);
						AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sTmp), (LPVARIANT)p0, &_variant_t(txtHeight)).pdispVal;
                        AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
                        ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));
						
						sTmp.Format(IDS_HOT_FORM2,modPHScal::dyr / 1000);
						p0.SetY(p0[1] + 4);
						AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sTmp), (LPVARIANT)p0, &_variant_t(txtHeight)).pdispVal;
                        AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
                        ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));
					}
					else
					{
						sTmp=_T("");
					}
				}
			}
		}
		else
		{
			//�������
			p0.SetPoint(
				InsPnt[0] + 3,
				InsPnt[1],
				InsPnt[2]);
			AcObj = MoSpace.Invoke(_T("AddText"),3,&_variant_t(sTmp), (LPVARIANT)p0, &_variant_t(txtHeight)).pdispVal;
            AcObj.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("���"), _T("BOM"))));
            ApplyTextStyleProperty(AcObj, GetTextStyleName(_T("���"), _T("BOM")),_T("���"));
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

bool EDIBAcad::bExistEntityInSelectionSet(COleSafeArray &pPointList)
{
	sset = SetSelectionSet(_T("SS1")).pdispVal;
	sset.Invoke0(_T("Clear"));
	sset.Invoke(_T("SelectByPolygon"),2,&_variant_t((long)acSelectionSetCrossingPolygon), (LPVARIANT)pPointList);
	long count=(long)sset.GetPropertyByName(_T("count"));
	if(count > 0 )
		return true;
	return false;
}

HWND EDIBAcad::GetWindowHandle(CString WindowText)
{
	HWND CurrWnd = GetActiveWindow();
	HWND CurrParentWindow = GetParent(CurrWnd);
	HWND Handle = CurrWnd;
	
	while(CurrParentWindow != 0)
	{
		Handle = CurrParentWindow;
		CurrParentWindow = GetParent(CurrParentWindow);
	}
	CurrWnd = GetWindow(Handle, GW_HWNDFIRST);
	//CurrWnd = GetWindow(AutoPHSD.hwnd, GW_HWNDFIRST)
	
	// Loop while the hWnd returned by GetWindow is valid.
	_TCHAR szTmp[256];
	CString szTitle;
	while(CurrWnd != 0)
	{
		//Get the length of the task name identified by
		//CurrWnd in the list.
		DWORD nResult = GetWindowText(CurrWnd, szTmp, 256);
		//MsgBox Left(szTitle, nLen)
		szTitle=szTmp;
		if( szTitle.Find(WindowText) ==0)// Then
		{
			//MsgBox _T("Found ") & szTitle
			return CurrWnd;
	  }
      CurrWnd = GetWindow(CurrWnd, GW_HWNDNEXT);
      //Process Windows events.
   }
   return NULL;
}

void EDIBAcad::GetBoundingBox(CMObject &AcObj,	VARIANT* minPoint, VARIANT* maxPoint)
{
	BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	COleDispatchDriver oleObj;
	if(AcObj.p==NULL)
		return;
	AcObj.p->AddRef();
	oleObj.m_lpDispatch=AcObj.p;
	DISPID dispid;
	AcObj.GetIDOfName(_T("GetBoundingBox"),&dispid);
	oleObj.InvokeHelper(/*0x6002001b*/dispid, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 minPoint, maxPoint);
}


bool EDIBAcad::SelectLayerAllV(long Count, va_list argList)
{
	//ѡ���ĵ�ʵ��
	CString sLN;
	if(!gbACADR14 /*|| gsACADVER!=_T("14.0")*/)
	{
		return SelectLayerV(Count,argList);
	}
	sset = SetSelectionSet(_T("SS1")).pdispVal;
	int i=0,j=0;
	CMObject tmpObj;
	int sC=Count;
	try
	{
		if(Count==0)
		{
			ShowMessage(GetResStr(IDS_MsgBox_60642));
			//delete []sLN;
			//sC=0;
			while( sLN== _T(""))
			{
				sset.Invoke0(_T("SelectOnScreen"));
				int C=(long)sset.GetPropertyByName(_T("Count"));
				if(C>0)
				{
					tmpObj=sset.Invoke(_T("Item"),1,&_variant_t((long)0)).pdispVal;
					sLN=vtos(tmpObj.GetPropertyByName(_T("Layer")));
					//If MsgBox(_T("�����Ҫ���") & sLN(0) & _T("���ϵ�����ʵ�嵽ѡ����?"), vbDefaultButton1 + vbQuestion + vbSystemModal + vbYesNo) = vbNo Then Exit Function
					if(ShowMessage(GetResStr(IDS_MsgBox_60643)+sLN+GetResStr(IDS_MsgBox_60644),MB_DEFBUTTON1|MB_ICONQUESTION|MB_SYSTEMMODAL|MB_YESNO)==IDNO)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		CCadPoint p1,p2,p1v,p2v;
		p1.SetPoint(0.0,0.0,0.0);
		p2.SetPoint(420.0,297.0,0.0);
		p1v = p1;
		p2v = p2;
		//p1v = objAcadDoc.Invoke( _T("GetVariable"),1,&_variant_t(_T("MinExt")));
		//p2v = objAcadDoc.Invoke( _T("GetVariable"),1,&_variant_t(_T("MaxExt")));
		COleSafeArray groupCode,dataCode;
		long ix=0;
		DWORD Em=1;
		long gx=8;
		groupCode.Create(VT_I4,1,&Em);
		dataCode.Create(VT_VARIANT,1,&Em);
		groupCode.PutElement(&ix,&gx);
		_variant_t tmpdc;
		_variant_t dataCode1,groupCode1;
		
		//groupCode1.ChangeType(VT_VARIANT);
		groupCode1=(LPVARIANT)groupCode;
		//groupCode1=(LPVARIANT)groupCode;
		if(Count==0)
		{
			tmpdc=_variant_t(sLN);
			dataCode.PutElement(&ix,&tmpdc);
			sset.Invoke(_T("Select"),5,&_variant_t((long)acSelectionSetAll),(LPVARIANT)p1v,(LPVARIANT)p2v,(LPVARIANT)groupCode,(LPVARIANT)dataCode);
			return true;
		}
		for(i=0;i<Count;i++)
		{
			sLN=(LPCTSTR)va_arg(argList,LPCTSTR);
			tmpdc=_bstr_t(sLN);
			dataCode.PutElement(&ix,&tmpdc);
			//dataCode1.ChangeType(VT_VARIANT);
			dataCode1 =(LPVARIANT)dataCode;
			//dataCode1=(LPVARIANT)dataCode;
			sset.Invoke(_T("Select"),5,&_variant_t((long)acSelectionSetAll),(LPVARIANT)p1v,(LPVARIANT)p2v,(LPVARIANT)&groupCode1,(LPVARIANT)&dataCode1);
		}
		return true;
	}
	catch(CException *e)
	{
		e->Delete();
	}
	return false;
}

void EDIBAcad::FindAllFileToTxt(CString dir,CFile *f)
{
	//Ŀ�ģ��ҳ�dirĿ¼�µ�����.dwg�ļ��������ļ���(��·��)д���ļ�f��
	try
	{
		CString sFiles,sDir,newDir;
		WIN32_FIND_DATA FindData;
		memset(&FindData,0,sizeof(WIN32_FIND_DATA));
		HANDLE FindHandle;
		sDir=dir;
		dir+=_T("*.*");
		FindHandle=FindFirstFile(dir,&FindData);
		if(FindHandle!=INVALID_HANDLE_VALUE)
		{
			do
			{
				sFiles=FindData.cFileName;
				sFiles.MakeLower ();
				if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
				{
					if(sFiles.Right(4)==_T(".dwg"))
					{
						sFiles=sDir+sFiles;
						sFiles+=_T("\r\n");
						f->Write(sFiles,sFiles.GetLength());
					}
				}
				else if(sFiles!=_T(".") && sFiles!=_T(".."))
				{

					newDir=sDir+FindData.cFileName;
					newDir+=_T("\\");
					FindAllFileToTxt(newDir,f);
				}
			}while(FindNextFile(FindHandle,&FindData));
			::FindClose(FindHandle);
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void EDIBAcad::GetActiveAcadDoc()
{
	try
	{
		CString strTmp,strTmp2;
		_variant_t tmpvar;
//		CString DwgName = basDirectory::PhsBlkDir + (gbACADR14 ? _T("phs.dwg") : _T("Phs2000.dwg"));
		if(gbACADR14)
		{
			objAcadDoc=objAcadApp.GetPropertyByName(_T("ActiveDocument")).pdispVal;
			strTmp=vtos(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("DWGPREFIX"))));
			strTmp2=vtos(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("DWGNAME"))));
			strTmp.MakeLower();strTmp2.MakeLower();
			basDirectory::PhsBlkDir.MakeLower();
/*			if(strTmp==basDirectory::PhsBlkDir && strTmp2 == _T("phs.dwg") )
			{

			}
			else
			{
				tmpvar=_variant_t(DwgName);
				//���򿪿��޸�����ͼֽ
				objAcadDoc.Invoke(_T("Open"),1,&tmpvar);
			}
*/
		}
		else
		{
			int count,i;
			CMObject acadDocs;
			CMObject objTmp;
			acadDocs=objAcadApp.GetPropertyByName(_T("Documents")).pdispVal;
			count=(long)acadDocs.GetPropertyByName(_T("Count"));
			bool bf=false;
			for(i=count-1;i>=0;i--)
			{
				objAcadDoc=acadDocs.Invoke(_T("Item"),1,&_variant_t((long)i)).pdispVal;
				strTmp=vtos(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("DWGPREFIX"))));
				user::AddDirSep(strTmp);
				strTmp2=vtos(objAcadDoc.Invoke(_T("GetVariable"),1,&_variant_t(_T("DWGNAME"))));
				strTmp.MakeLower();strTmp2.MakeLower();
				basDirectory::PhsBlkDir.MakeLower();
				if(modPHScal::gsDwgFN==_T(""))
				{
					modPHScal::zdjh=vtoi(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("Zdjh")));
					modPHScal::gsDwgFN=modPHScal::GetSaveAsDwgFileName(modPHScal::zdjh,FrmTxsr.m_pViewTxsr->m_ActiveRs);
				}
				modPHScal::gsDwgFN.MakeLower();
				if(vtob(objAcadDoc.GetPropertyByName(_T("Active"))))
				{
				//	if((strTmp==basDirectory::PhsBlkDir && strTmp2 == _T("phs2000.dwg")))// || (strTmp + strTmp2==modPHScal::gsDwgFN) )
						bf=true;
				}
				else if((strTmp==basDirectory::PhsBlkDir && strTmp2 == _T("phs2000.dwg")))// || (strTmp + strTmp2==modPHScal::gsDwgFN) )
				{
				//	objAcadDoc.Invoke(_T("Close"),1,&_variant_t(false));
				}
				else 
				{
				//	objAcadDoc.Invoke(_T("Close"),1,&_variant_t(true));
				}
			}
			objAcadDoc=objAcadApp.GetPropertyByName(_T("ActiveDocument")).pdispVal;
//			objAcadDoc.Invoke(_T("SetVariable"),2,&_variant_t(_T("SDI")),&_variant_t((long)1));
			if(bf==false)
			{
			//	objAcadDoc.Invoke0(_T("Save"));
			//	tmpvar=_variant_t(DwgName);
			//	objAcadDoc.Invoke(_T("Open"),1,&tmpvar);
			}
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

bool EDIBAcad::SelectLayerV(long Count, va_list argList)
{
	COleSafeArray Entitys;
	CMObject tmpObj;
	CMObject tmpLayer;
	CMObject tmpMods;
	int i,c,i1;
	CString * pLayerName=NULL;
	bool ret=false;
	sset = SetSelectionSet(_T("SS1")).pdispVal;
	if(Count<=0)
	{
		pLayerName=new CString[1];
		while( pLayerName[0].GetLength()<=0)
		{
			sset.Invoke0(_T("SelectOnScreen"));
			int C=(long)sset.GetPropertyByName(_T("Count"));
			if(C>0)
			{
				tmpObj=sset.Invoke(_T("Item"),1,&_variant_t((long)0)).pdispVal;
				pLayerName[0]=vtos(tmpObj.GetPropertyByName(_T("Layer")));
				//If MsgBox(_T("�����Ҫ���") & sLN(0) & _T("���ϵ�����ʵ�嵽ѡ����?"), vbDefaultButton1 + vbQuestion + vbSystemModal + vbYesNo) = vbNo Then Exit Function
				if(ShowMessage(GetResStr(IDS_MsgBox_60643)+pLayerName[0]+GetResStr(IDS_MsgBox_60644),MB_DEFBUTTON1|MB_ICONQUESTION|MB_SYSTEMMODAL|MB_YESNO)==IDNO)
				{
					delete [] pLayerName;
					return false;
				}
			}
			else
			{
				delete [] pLayerName;
				return false;
			}
		}
		pLayerName[0].MakeUpper();
		Count=1;
	}
	else
	{
		pLayerName=new CString [Count];
		for(i=0;i<Count;i++)
		{
			pLayerName[i]=(LPCTSTR)va_arg(argList,LPCTSTR);
			pLayerName[i].MakeUpper();
		}
	}
	CString sTmp;
	try
	{
		tmpMods=objAcadDoc.GetPropertyByName(_T("ModelSpace")).pdispVal;
		c=(long)tmpMods.GetPropertyByName(_T("Count"));
		if(c==0)
			return ret;
		unsigned long Em=1;
		Em=c;
		long ix=0;
		Entitys.Create(VT_DISPATCH,1,&Em);
		BOOL bf=FALSE;
		DISPID pidItem=0,pidLayer=0;
		tmpMods.GetIDOfName(_T("Item"),&pidItem);
		tmpObj=tmpMods.Invoke(pidItem,1,&_variant_t((long)0)).pdispVal;
		tmpObj.GetIDOfName(_T("Layer"),&pidLayer);
		for(i=0;i<c;i++)
		{
			tmpObj=tmpMods.Invoke(pidItem,1,&_variant_t((long)i)).pdispVal;
			sTmp=(char*)_bstr_t(tmpObj.GetProperty(pidLayer));
			sTmp.MakeUpper();
			bf=FALSE;
			for(i1=0;i1<Count;i1++)
			{
				if(pLayerName[i1]==sTmp)
				{
					bf=TRUE;
					break;
				}
			}
			if(bf)
			{
				Entitys.PutElement(&ix,tmpObj.p);
				ix++;
			}
		}
		sset.Invoke(_T("AddItems"),1,(LPVARIANT)Entitys);
		ret=true;
	}
	catch(COleException * e)
	{
		e->ReportError();
		e->Delete();
		ret=false;
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		ret=false;
	}
	if(pLayerName!=NULL) delete [] pLayerName;
	return ret;
}

CString EDIBAcad::GetCell(CString strInp, int row, int col)
{
	CString strCell;
	strInp.TrimLeft();strInp.TrimRight();
	char thecol=strInp[0];
	thecol+=col;
	int therow=_ttol(strInp.Mid(1)) + row;
	strCell.Format(_T("%c%d"),thecol,therow);
	return strCell;
}

void EDIBAcad::GetRange(CString strCell, CString &strCell1, CString &strCell2)
{
	int i=strCell.Find(_T("-"));
	if(i!=-1)
	{
		strCell1=strCell.Left(i);
		strCell2=strCell.Mid(i+1);
	}
	else
	{
		strCell1=strCell;
		strCell2=strCell;
	}
}



BOOL EDIBAcad::LoadArx(CString strArxName)
{
	try
	{
		/*_variant_t varTmp;
		varTmp=EDIBAcad::objAcadApp.Invoke0(_T("ListArx"));
		::COleSafeArray arxList(varTmp.parray,VT_BSTR);
		int i,iLb,iUb;
		arxList.GetLBound(1,&iLb);
		arxList.GetUBound(1,&iUb);
		for(i=iLb;i<=iUb;i++)
		{
			arxList.GetElemSize();
			arxList.GetElement();*/
		EDIBAcad::objAcadApp.Invoke(_T("LoadArx"),1,&_variant_t(strArxName));
		return TRUE;
	}
	catch(CException *e)
	{
		e->Delete();
		return FALSE;
	}
}

double EDIBAcad::GetTextStyleWidthFactor(LPCTSTR lpszStyle)
{
	double dResult=1.0;
	GetActiveAcadDoc();
	//Dim TS As Object, i As Integer, Found As Boolean
	CMObject tmpObj,TS;
	TS=objAcadDoc.GetPropertyByName(_T("TextStyles")).pdispVal;
	tmpObj=TS.Invoke(_T("Item"),1,&_variant_t(lpszStyle)).pdispVal;
	if(tmpObj.p!=NULL)
	{
		dResult=(double)tmpObj.GetPropertyByName(_T("Width"));
	}
	return dResult;
}


BOOL EDIBAcad::ApplyTextStyleProperty(CMObject &obj, CString strStyleName,CString strChinaName)
{
    ///////////////////////////////////////////////////
    // Ӧ�����ֶ���ķ������
    // ���&��̣��޳�
    // ��     �ڣ�2003/10/27

    double  dHeight, dWidthFactor, dAngle;
    CString strShx, strBig;
    short   iGenFlag;
	CMObject tmp, tsobj;

	tmp = objAcadDoc.GetPropertyByName(_T("TextStyles")).pdispVal;
	tsobj = tmp.Invoke(_T("Item"), 1, &_variant_t(strStyleName)).pdispVal;

	if(tsobj.p != NULL)
    {

		if(strChinaName.IsEmpty())
		{
			strShx       = vtos(tsobj.GetPropertyByName(
							 _T("FontFile")));            //��������ļ���
			strBig       = vtos(tsobj.GetPropertyByName(
							 _T("BigFontFile")));         //��ô������ļ���
        
			dHeight      = vtod(tsobj.GetPropertyByName(
							 _T("Height")));              //�������߶�
        
			dWidthFactor = vtod(tsobj.GetPropertyByName(
							  _T("Width")));              //��ÿ�ȱ�������***
        
			dAngle       = vtod(tsobj.GetPropertyByName(
							  _T("ObliqueAngle")));        //���������б�Ƕ�
			if(dAngle > 4.71) dAngle -= 2 * 3.14159265359;
        
			iGenFlag     = vtoi(tsobj.GetPropertyByName(
							 _T("TextGenerationFlag"))); //������������ֶ�
			iGenFlag    &= 0x06;
		}
		else
		{
			strShx = GetRegKey(_T("Settings"), strChinaName + _T("-SHXFILENAME"), CString("txt.shx"));

			strBig = GetRegKey(_T("Settings"), strChinaName + _T("-BIGFILENAME"), CString("hzfs.shx"));
			//�߶�
			dHeight = GetRegKey(_T("Settings"), strChinaName + _T("-HEIGHT"), float(4.0));
			//���
			dWidthFactor = GetRegKey(_T("Settings"), strChinaName + _T("-WIDTH"), float(1.0));
			//��б�Ƕ�
			dAngle = GetRegKey(_T("Settings"), strChinaName + _T("-OBLIQUEANGLE"), float(0.0));
			if(dAngle > 4.71) dAngle -= 2 * 3.14159265359;
			
			iGenFlag     = vtoi(tsobj.GetPropertyByName(_T("TextGenerationFlag"))); //������������ֶ�
			iGenFlag    &= 0x06;
		}

        obj.PutPropertyByName(_T("FontFile"), &_variant_t(strShx));
        obj.PutPropertyByName(_T("BigFontFile"), &_variant_t(strBig));
        obj.PutPropertyByName(_T("Height"), &_variant_t(dHeight));
        obj.PutPropertyByName(_T("ScaleFactor"), &_variant_t(dWidthFactor));
        obj.PutPropertyByName(_T("ObliqueAngle"), &_variant_t(dAngle));
        obj.PutPropertyByName(_T("TextGenerationFlag"), &_variant_t(iGenFlag));
        return TRUE;
    }
	return FALSE;
}

CString EDIBAcad::GetTextStyleName(CString strTextType,
                                   CString strDefaultStyleName)
{
    ///////////////////////////////////////////////////
    // ��ע����ȡ���ֶ���ķ����
    // ���&��̣��޳�
    // ��     �ڣ�2003/10/28

    //����strtextstyle���������¼���
    //�����ϸ����ע�ߴ硢����λ�Ʊ�
    //ע�͡���š���ߡ���������
    
    if(strTextType == _T("�����ϸ��") ||
       strTextType == _T("��ע�ߴ�")   ||
       strTextType == _T("����λ�Ʊ�") ||
       strTextType == _T("ע��")       ||
       strTextType == _T("���")       ||
       strTextType == _T("���")       ||
       strTextType == _T("��������"))
    {
        CString  strValue;
        strValue = GetRegKey(_T("Settings"),
            strTextType + _T("-STYLENAME"), strValue);
        if(strValue == _T("")) strValue = strDefaultStyleName;
        if(strValue == _T("")) strValue = _T("standard");
        return strValue;
    }
    else
    {
        return _T("standard");
    }
}


void EDIBAcad::SetACADSupportPath()
{
	//����ͼ�ο������·��������Ϊÿһ����������ȫ·��,����Ķ�����С
	if(gbACADR2000)
	{
		CMObject ObjPreferences;
		CMObject ObjFiles;
		CMObject ObjSupportPath;
		ObjPreferences=EDIBAcad::objAcadApp.GetPropertyByName(_T("Preferences")).pdispVal;
		_variant_t varStr;
		varStr.SetString("<<Unnamed Profile>>");
		ObjFiles=ObjPreferences.GetPropertyByName(_T("Files")).pdispVal;
		varStr=ObjFiles.GetPropertyByName(_T("SupportPath"));
		//��SupportPath ������ͼ�ο�·���Ƿ���ڣ������������Ӧ����ͼ�ο�·��
		
		CString str = vtos(varStr);
		if(str.Find(basDirectory::PhsBlkDir)!= -1)
		{
		}
		else
		{
			str += (";"+basDirectory::PhsBlkDir);
			varStr = (_variant_t)str;
			ObjFiles.PutPropertyByName(_T("SupportPath"),&varStr);
		}
	}
}

void EDIBAcad::LoadMyArx(CString strArxAppName)
{
	CString strArxPrefix;//ARX������ǰ׺,��.arxǰ��Ĳ���,��phs.arx�е�phs��
	strArxPrefix=strArxAppName.Left(strArxAppName.Find("."));
	CString str;
	if( FileExists(basDirectory::InstallDir + _T("2002\\") + strArxPrefix + _T("14.arx")) || 
				FileExists(basDirectory::InstallDir + _T("2002\\") + strArxPrefix + _T("2000.arx")) ||
				FileExists(basDirectory::InstallDir + _T("2004\\") + strArxPrefix + _T("2004.arx")) )
			{	
				
				EDIBAcad::objAcadApp.Invoke(_T("UNLoadArx"),1,&_variant_t(basDirectory::InstallDir + strArxAppName));

				if(EDIBAcad::gbACADR14)
				{
					::CopyFile(basDirectory::InstallDir+_T("2002\\") + strArxPrefix + _T("14.arx"),basDirectory::InstallDir+_T("2002\\")  + strArxAppName,FALSE);
					str = basDirectory::InstallDir+_T("2002\\");
				}
				else if( EDIBAcad::gbACADR2000 )
				{
					::CopyFile(basDirectory::InstallDir +_T("2002\\")+ strArxPrefix + _T("2000.arx"),basDirectory::InstallDir+_T("2002\\")  + strArxAppName,FALSE);
					str = basDirectory::InstallDir+_T("2002\\");
				}
				else
				{
					::CopyFile(basDirectory::InstallDir +_T("2004\\")+ strArxPrefix + _T("2004.arx"),basDirectory::InstallDir+_T("2004\\")  + strArxAppName,FALSE);
					str = basDirectory::InstallDir+_T("2004\\");
				}
				EDIBAcad::DeleteSupportPath();
				EDIBAcad::AddSupportPath(str);
				EDIBAcad::LoadArx(str + strArxAppName);
				EDIBAcad::DeleteSupportPath();
			}
			else
			{
				CString strMsg;
				strMsg.Format(IDS_ARX_NOTEXIST,strArxAppName,str,strArxAppName);
				AfxMessageBox(strMsg);
			}

}
//AutoPDMS8.0��AutoPDMS3.0��2009��9��֮���˳�CADʱ�Ὣ��װ·����
//�ʳ����ͼʱ��Ҫ��CADѡ�������·���������Ҳ��������ӿ⡣//ADD By luorijin 2009.02.13
//��CAD��������ת�ƹ���

void EDIBAcad::AddSupportPath(const CString &strInsPath)
{
	CMObject ObjAcadAppPreferencesFiles;
	CMObject ObjPreferences;
	ObjPreferences=EDIBAcad::objAcadApp.GetPropertyByName(_T("Preferences")).pdispVal;
	ObjAcadAppPreferencesFiles=ObjPreferences.GetPropertyByName(_T("Files")).pdispVal;
	
	
	_variant_t varAllPah;
	varAllPah = ObjAcadAppPreferencesFiles.GetPropertyByName( _T("SupportPath") ); 
	CString strAllPath = varAllPah.bstrVal;
	if( strAllPath.Find( strInsPath ) == -1 )  //�ж��Ƿ��Ѿ����
	{	
		//	//Find ����ʧ��
        
		if( strAllPath.Right( 1 ) != _T(";"))
			strAllPath += _T(";");
		strAllPath += strInsPath;
		//strAllPath = _T("");
		_variant_t varSupportPath;
		varSupportPath.SetString( strAllPath );
		ObjAcadAppPreferencesFiles.PutPropertyByName(_T("SupportPath"),&varSupportPath);
		
	}
}

void EDIBAcad::DeleteSupportPath()
{
	CString strPath;
	CString strLeft;
	CString strRight;
	CString strInsPath = _T("��ɳ�������");
	CMObject ObjAcadApp;
	if(SUCCEEDED(ObjAcadApp.GetActiveObject(_T("AutoCAD.Application"))))
	{	
		CMObject ObjAcadAppPreferencesFiles;
		CMObject ObjPreferences;
		ObjPreferences=ObjAcadApp.GetPropertyByName(_T("Preferences")).pdispVal;
		ObjAcadAppPreferencesFiles=ObjPreferences.GetPropertyByName(_T("Files")).pdispVal;
		
		
		_variant_t varAllPah;
		varAllPah = ObjAcadAppPreferencesFiles.GetPropertyByName( _T("SupportPath") );  //���SupportPath����·��
		
		CString strAllPath = varAllPah.bstrVal;
	strInsPath.MakeUpper();
	int pos = strAllPath.Find( _T(";") );
	strRight = strAllPath;
	while ( pos != -1 )
	{
		strLeft = strRight.Left( pos + 1 );
		strRight = strRight.Mid( pos + 1 );
		CString strLeftTmp = strLeft;
		strLeftTmp.MakeUpper();
		if ( strLeftTmp.Find( strInsPath ) == -1 )
		{
			strPath += strLeft;
		}
		pos = strRight.Find( _T(";") );
	}
	
	CString strRightTmp = strRight;
	strRightTmp.MakeUpper();
	if ( strRightTmp.Find( strInsPath ) == -1 )
	{
		strPath += strRight;
	}
	if( strPath.Right( 1 ) == _T(";") )
		strPath = strPath.Left( strPath.GetLength() -1 );
	_variant_t varSupportPath;
	varSupportPath.SetString( strPath );
	ObjAcadAppPreferencesFiles.PutPropertyByName(_T("SupportPath"),&varSupportPath);
	}
}
