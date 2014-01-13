// ImportPlantVision.cpp: implementation of the CImportPlantVision class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "user.h"
#include "ImportPlantVision.h"
#include "basdirectory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImportPlantVision::CImportPlantVision()
{
	this->SetRegSubKey("Settings");
}

CImportPlantVision::~CImportPlantVision()
{

}

void CImportPlantVision::BeginImport()
{ 
	int iPos;
	CPropertyWnd::ElementDef ElementDef;
	ImportFromXLSStruct ImportTable;
	ImportFromXLSElement ImportTableItem[100];
	int TableItemCount;

	CWaitCursor WaitCursor;

	// �����������ֶεĶ��ձ�
	struct
	{
		LPCTSTR NameCh;		// pre_calc���ֶ�������������
		LPCTSTR NameField;	// pre_calc���ֶ���
	}FieldsName[]=
	{
		{_T("֧���ܺ�"),		_T("ZDJH")},
	};

	this->m_HintInformation=_T("��ʼ��������");
	this->UpdateData(FALSE);
	this->UpdateWindow();

    this->SetProjectDbConnect( conPRJDB );

	//
	// ��дImportFromXLSStruct�ṹ��Ϣ
	//
	ImportTable.ProjectDBTableName=_T("ZA1");

	ImportTable.XLSFilePath=this->m_XLSFilePath;

	iPos=this->m_PropertyWnd.FindElement(0,_T("�������ݿ�ʼ�к�"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.BeginRow=_ttoi(ElementDef.RightElementText);

	iPos=this->m_PropertyWnd.FindElement(0,_T("Excel��������"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.XLSTableName=ElementDef.RightElementText;

	iPos=this->m_PropertyWnd.FindElement(0,_T("�������������"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.RowCount=_ttoi(ElementDef.RightElementText);


	TableItemCount=0;
	iPos++;
	//
	// �����Կؼ��е���������ImportTableItem�ṹ
	//
	while(TRUE)
	{
		if(iPos>=this->m_PropertyWnd.GetElementCount())
			break;

		this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
		iPos++;

		// ���ҵ�Ԫ��Ϊ��ʱ������
		if(!ElementDef.RightElementText.IsEmpty())
		{
			ImportTableItem[TableItemCount].SourceFieldName=ElementDef.RightElementText;
			ImportTableItem[TableItemCount].DestinationName = ElementDef.strFieldName;
			TableItemCount++;
		}
	}

	ImportTable.pElement=ImportTableItem;
	ImportTable.ElementNum=TableItemCount;

	try
	{
		// ZSY  [2005/12/02]
		if ( !ImportExcelToAccess( &ImportTable ) )
		{
			CDialog::OnCancel();
			return;
		}
	}
	catch(_com_error &e)
	{
		this->m_HintInformation=_T("���ݵ���ʧ��");
		this->UpdateData(FALSE);

		AfxMessageBox(e.Description());

		return;
	}
	catch(COleDispatchException *e1)
	{
		this->m_HintInformation=_T("���ݵ���ʧ��");
		this->UpdateData(FALSE);

		AfxMessageBox(e1->m_strDescription);
		e1->Delete();
		return;
	}
	this->m_HintInformation=_T("���ݵ���ɹ�");
	this->UpdateData(FALSE);

	CDialog::OnOK();
}


BOOL CImportPlantVision::InitPropertyWnd()
{
	CImportFromXLSDlg::InitPropertyWnd();
	
	this->SetWindowText("��ȡPlantVision����(.xls��ʽ)");
	//
	// ��ʼ�����Կؼ�
	//
	CPropertyWnd::ElementDef ElementDef;

	struct
	{
		LPCTSTR ElementName;	// ��Ԫ������
		UINT	Style;			// ��ʾ��ʽ
		LPCTSTR HintInfo;		// �뵥Ԫ��ص���ʾ��Ϣ
	}ElementSet[]=
	{
		{_T("PlantVisionԭʼ����"),	CPropertyWnd::TitleElement,NULL},								//4
	};

	for(int i=0;i<sizeof(ElementSet)/sizeof(ElementSet[0]);i++)
	{
		ElementDef.szElementName=ElementSet[i].ElementName;
		ElementDef.Style=ElementSet[i].Style;
		ElementDef.pVoid=NULL;
		if(ElementSet[i].HintInfo!=NULL)
			ElementDef.pVoid=new CString(ElementSet[i].HintInfo);
		m_PropertyWnd.InsertElement(&ElementDef);
	}
	
	// zsy  [2005/12/02]
	try
	{
		_ConnectionPtr pConSort;
		pConSort.CreateInstance(__uuidof(Connection));
		_RecordsetPtr pRsInfo;
		pRsInfo.CreateInstance(__uuidof(Recordset));
		CString strSQL;
		//�������ݿ�
		strSQL = ::dbConnectionString + basDirectory::ProjectDBDir + _T("Sort.mdb");
		pConSort->Open(_bstr_t(strSQL), "", "", -1);
		
		//�򿪽ṹ��
		strSQL = "SELECT * FROM [TZA1] WHERE CADFieldSeq >= 0 ORDER BY CADFieldSeq";
		pRsInfo->Open(_variant_t(strSQL), pConSort.GetInterfacePtr(), adOpenKeyset, adLockOptimistic, adCmdText);
		if ( pRsInfo->adoEOF && pRsInfo->BOF )
		{
			return FALSE;
		}
		int iPos;
		CString strTmp;
		TCHAR ColumeName[128];
		for (pRsInfo->MoveFirst(); !pRsInfo->adoEOF; pRsInfo->MoveNext())
		{
			//ԭʼ���ݱ��ж�Ӧ���ֶ�����
			strTmp = vtos( pRsInfo->GetCollect(_variant_t("FieldName")) );
			if ( strTmp.IsEmpty() )
				continue;
			ElementDef.strFieldName = strTmp;
			//�ֶ�����
			strTmp = vtos( pRsInfo->GetCollect(_variant_t("LocalCaption")) );
			if ( strTmp.IsEmpty() )
				continue;
			ElementDef.szElementName = strTmp;
			//��ʾ��Ϣ
			ElementDef.pVoid = new CString("��ѡ����"+strTmp+"��Ӧ��Excel����Ϊ����");
			//���
			ElementDef.Style = CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement;			
			
			//Ĭ�ϵ�EXCEL�к�
			strTmp = vtos( pRsInfo->GetCollect(_variant_t("ExcelColNO")) );
			ElementDef.RightElementText = strTmp;

			//����һ��
			iPos = m_PropertyWnd.InsertElement(&ElementDef);
			//�����ӿؼ�
			if ( m_PropertyWnd.CreateElementControl(iPos) )
			{
				//�ұ��ṩѡ��EXCEL�к�
				for(int k=0;k<100;k++)
				{
					int j,m,m2;				
					for(j=k,m=1;j/26!=0;m++,j/=26);				
					ColumeName[m]=_T('\0');				
					m--;
					j=k;
					m2=m;
					while(m>=0)
					{
						ColumeName[m]=j%26+_T('A');					
						j=j/26;					
						if(m2!=m)
							ColumeName[m]--;					
						m--;
					}
					this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
					((CComboBox*)ElementDef.pControlWnd)->AddString(ColumeName);
				}	
			}

		}
		//
		m_PropertyWnd.RefreshData();
	}
	catch (_com_error& e)
	{
		AfxMessageBox( e.Description() );
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/12/05]
// Parameter(s) :
// Return       :
// Remark       :�����û���д��״̬
//------------------------------------------------------------------
void CImportPlantVision::SaveSetToReg()
{
	CImportFromXLSDlg::SaveSetToReg();
	
	try
	{
		_ConnectionPtr pConSort;
		pConSort.CreateInstance(__uuidof(Connection));
		_RecordsetPtr pRsInfo;
		pRsInfo.CreateInstance(__uuidof(Recordset));
		CString strSQL;
		CPropertyWnd::ElementDef ElementDef;

		//�������ݿ�
		strSQL = ::dbConnectionString + basDirectory::ProjectDBDir + _T("Sort.mdb");
		pConSort->Open(_bstr_t(strSQL), "", "", -1);
		
		//�򿪽ṹ��
		strSQL = "SELECT * FROM [TZA1] WHERE CADFieldSeq >= 0 ORDER BY CADFieldSeq";
		pRsInfo->Open(_variant_t(strSQL), pConSort.GetInterfacePtr(), adOpenKeyset, adLockOptimistic, adCmdText);
		if ( pRsInfo->adoEOF && pRsInfo->BOF )
		{
			return ;
		}
		int iPos;
		CString strTmp;
		for (pRsInfo->MoveFirst(); !pRsInfo->adoEOF; pRsInfo->MoveNext())
		{
			//�ֶ�����
			strTmp = vtos( pRsInfo->GetCollect(_variant_t("LocalCaption")) );
			if ( strTmp.IsEmpty() )
				continue;
			iPos = this->m_PropertyWnd.FindElement( 0, strTmp );
			if (  -1 == iPos )
			{
				continue;
			}
			this->m_PropertyWnd.GetElementAt( &ElementDef, iPos );

			//д�뵽���ݿ����
			pRsInfo->PutCollect(_variant_t("ExcelColNO"), _variant_t(ElementDef.RightElementText) );
			pRsInfo->Update();

		}
	}
	catch (_com_error& e) 
	{
		AfxMessageBox( e.Description() );
		return;
	}

}
