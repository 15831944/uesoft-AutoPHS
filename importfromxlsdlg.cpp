// ImportFromXLSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportFromXLSDlg.h"
#include "resource.h"
#include "modregistry.h"
#include "user.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportFromXLSDlg dialog


CImportFromXLSDlg::CImportFromXLSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportFromXLSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportFromXLSDlg)
	m_XLSFilePath = _T("");
	m_HintInformation = _T("");
	//}}AFX_DATA_INIT

	//����Ĭ�ϵ��Ӽ�������
	m_RegSubKey=_T("ImportFromXLSDlg");
}

CImportFromXLSDlg::~CImportFromXLSDlg()
{
	CPropertyWnd::ElementDef Element;

	//
	// �ͷ����ڱ�����ʾ��Ϣ���ڴ�
	//

	for(int i=0;i<this->m_PropertyWnd.GetElementCount();i++)
	{
		this->m_PropertyWnd.GetElementAt(&Element,i);

		if(Element.pVoid)
		{
			delete ((CString*)Element.pVoid);
			Element.pVoid=NULL;
		}
	}
}

void CImportFromXLSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportFromXLSDlg)
	DDX_Control(pDX, IDC_OPEN_FILEDLG, m_OpenFileDlgButton);
	DDX_Text(pDX, IDC_FILE_FULLPATH, m_XLSFilePath);
	DDX_Text(pDX, IDC_HINT_INFORMATION, m_HintInformation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportFromXLSDlg, CDialog)
	//{{AFX_MSG_MAP(CImportFromXLSDlg)
	ON_BN_CLICKED(IDC_OPEN_FILEDLG, OnOpenFiledlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BEGIN_IMPORT1, OnBeginImport)
	//}}AFX_MSG_MAP
	ON_NOTIFY(PWN_SELECTCHANGE,IDC_PROPERTY_WND, OnSelectChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportFromXLSDlg message handlers

BOOL CImportFromXLSDlg::OnInitDialog() 
{
	CBitmap Bitmap;

	CDialog::OnInitDialog();

	Bitmap.LoadBitmap(IDB_OPENFILE);
	this->m_OpenFileDlgButton.SetBitmap((HBITMAP)Bitmap.Detach());


	CRect Rect;

	//
	// �����ص�Group�ؼ�����λ���Կؼ�
	//
	GetDlgItem(IDC_PROP_WND_RECT)->GetWindowRect(&Rect);

	this->ScreenToClient(&Rect);

	m_PropertyWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP,Rect,this,IDC_PROPERTY_WND);

	//��ʼ�����Կؼ�
	InitPropertyWnd();
	
	//��ע����ó�ʼ��Ϣ
	InitFromReg();

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////
//
// ��Ӧ��ѡ���ļ���ť��
//
void CImportFromXLSDlg::OnOpenFiledlg() 
{
	CFileDialog  FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Worksheet Files (*.xls)|*.xls|All Files (*.*)|*.*||"));
	
	FileDlg.m_ofn.lpstrInitialDir = m_strPrecFilePath;			 //05/1/4
	//
	// ���ѡ����XLS�ļ��ʹ���������ù�������
	//
	if(FileDlg.DoModal()==IDOK)
	{
		CWaitCursor WaitCursor;
		
		this->m_XLSFilePath=FileDlg.GetPathName();
		this->UpdateData(FALSE);
		
		int iPos;
		CString strSheetName;
		CPropertyWnd::ElementDef ElementInfo;	//���Է������Կؼ���Ԫ����Ϣ
		CComboBox * pCtlTblName;
		
		//����"Excel��������"��Ԫ����Ϣ
		iPos=this->m_PropertyWnd.FindElement(0,_T("Excel��������"));
		this->m_PropertyWnd.CreateElementControl(iPos);
		this->m_PropertyWnd.GetElementAt(&ElementInfo,iPos);
		//��õ�ǰ״̬�Ĺ�������   1/11     
		strSheetName = ElementInfo.RightElementText;
		//
		pCtlTblName = (CComboBox*)ElementInfo.pControlWnd;
		pCtlTblName->ResetContent();
		
		if ( ConnectExcelFile( m_XLSFilePath, m_pConExcel) )
		{
			_RecordsetPtr pRsTmp;
			pRsTmp = m_pConExcel->OpenSchema( adSchemaTables );
			if ( !pRsTmp->adoEOF && !pRsTmp->BOF )
			{
				CString strTableName;
				for ( pRsTmp->MoveFirst(); !pRsTmp->adoEOF; pRsTmp->MoveNext() )
				{
					strTableName = vtos( pRsTmp->GetCollect(_variant_t("TABLE_NAME")));
					strTableName.TrimLeft("\'");
					strTableName.TrimRight("\'");
					strTableName.TrimRight("$");
					if ( !strTableName.IsEmpty() && -1 == pCtlTblName->FindString( -1, strTableName ) )
					{
						pCtlTblName->AddString( strTableName );
					}
				}
			}
		}
		///////���ָ���Ĺ�������      05/1/4	
		iPos = pCtlTblName->FindString(0, strSheetName);
		iPos = (iPos>0) ? iPos : 0;
		((CComboBox*)ElementInfo.pControlWnd)->SetCurSel(iPos);
		//////05/1/4
		
		//�ػ����Կؼ��Ĵ���
		this->m_PropertyWnd.Invalidate();
		
		///��ǰ��05/1/4
		m_strPrecFilePath = m_XLSFilePath;
	}
}

void CImportFromXLSDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	//�رմ���ʱ�������ݵ�ע���
	SaveSetToReg();
}

//////////////////////////////////////////////////////////////

///////////////////////////////////////////
//
// ��ʼ�����Դ���
//
// �����ɹ�����TRUE�����򷵻�FALSE
//
// �˺���Ӧ����WM_INITDIALOG��Ϣ�е���
//
BOOL CImportFromXLSDlg::InitPropertyWnd()
{
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
		{_T("Excel"),			CPropertyWnd::TitleElement,NULL},								//0

		{_T("Excel��������"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ�����ű������")},	//1

		{_T("�������ݿ�ʼ�к�"),CPropertyWnd::ChildElement|CPropertyWnd::EditElement,NULL},		//2
		{_T("�������������"),	CPropertyWnd::ChildElement|CPropertyWnd::EditElement,NULL},		//3
	};


	for(int i=0;i<sizeof(ElementSet)/sizeof(ElementSet[0]);i++)
	{
		ElementDef.szElementName=ElementSet[i].ElementName;
		ElementDef.Style=ElementSet[i].Style;

		ElementDef.pVoid=NULL;

		if(ElementSet[i].HintInfo!=NULL)
		{
			
			ElementDef.pVoid=new CString(ElementSet[i].HintInfo);
		}

		m_PropertyWnd.InsertElement(&ElementDef);

	}

	m_PropertyWnd.RefreshData();

	return TRUE;
}
///////////////////////////////////////////
//
// ��ע��������ݳ�ʼ��
//
void CImportFromXLSDlg::InitFromReg()
{
	LONG lRet;
	DWORD dwDisposition;
	HKEY hKey;
	CString SubKey;
	CPropertyWnd::ElementDef ElementDef;
	TCHAR szTemp[256];
	DWORD dwData;

	SubKey=szSoftwareKey;
	SubKey+=m_RegSubKey;

	//��ע���
	lRet=::RegCreateKeyEx(HKEY_LOCAL_MACHINE,SubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_READ,NULL,&hKey,&dwDisposition);
	
	if(lRet!=ERROR_SUCCESS)
	{
		AfxMessageBox(_T("��ע���ʧ��"));
		return;
	}
	// ��ע��������ݷ������Կؼ�
	//
//	for(int i=0;i<this->m_PropertyWnd.GetElementCount();i++)
	int nPos = this->m_PropertyWnd.FindElement(0, "�������������");
	if ( nPos < 0 )
		nPos = 3;
	
	for(int i=0;i<=nPos;i++)
	{
		this->m_PropertyWnd.CreateElementControl(i);
		this->m_PropertyWnd.GetElementAt(&ElementDef,i);

		dwData=256;

		lRet=RegQueryValueEx(hKey,ElementDef.szElementName,NULL,NULL,(BYTE*)szTemp,&dwData);

		if(lRet==ERROR_SUCCESS)
		{
			if(ElementDef.pControlWnd && IsWindow(ElementDef.pControlWnd->GetSafeHwnd()))
			{
				if(dwData>0)
					ElementDef.pControlWnd->SetWindowText(szTemp);
			}
		}
	}
	/////////  ��ʼExcel �ļ�·��
	dwData=256;
	lRet=RegQueryValueEx(hKey,"ExcelFilePath",NULL,NULL,(BYTE*)szTemp,&dwData);  

	if(lRet==ERROR_SUCCESS)
	{
		m_strPrecFilePath = szTemp;
	}
	m_XLSFilePath = m_strPrecFilePath;
	//////
	::RegCloseKey(hKey);
	
}

////////////////////////////////////////////
//
// ����Ϣ���浽ע�����
//
void CImportFromXLSDlg::SaveSetToReg()
{
	LONG lRet;
	DWORD dwDisposition;
	HKEY hKey;
	CString SubKey;
	CPropertyWnd::ElementDef ElementDef;

	SubKey=szSoftwareKey;
	SubKey+=m_RegSubKey;

	// ��ע���
	lRet=::RegCreateKeyEx(HKEY_LOCAL_MACHINE,SubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hKey,&dwDisposition);
	
	if(lRet!=ERROR_SUCCESS)
	{
		AfxMessageBox(_T("��ע���ʧ��"));
		return;
	}

	//
	// �����Կؼ������ݴ���ע���
	//
//	for(int i=0;i<this->m_PropertyWnd.GetElementCount();i++)
	int nPos = this->m_PropertyWnd.FindElement(0, "�������������");
	if ( nPos < 0 )
		nPos = 3;

	for(int i=0;i<=nPos;i++)
	{
		this->m_PropertyWnd.GetElementAt(&ElementDef,i);

		if(!ElementDef.RightElementText.IsEmpty())
		{
			lRet=RegSetValueEx(hKey,ElementDef.szElementName,NULL,REG_SZ,(BYTE*)(LPCTSTR)ElementDef.RightElementText,
						  strlen(ElementDef.RightElementText)+sizeof(TCHAR));
		}
		else
		{
			lRet=RegSetValueEx(hKey,ElementDef.szElementName,NULL,REG_SZ,(BYTE*)NULL,
						  0);
		}

	}
	///////  ����·��
	if ( !m_strPrecFilePath.IsEmpty() )
	{
			lRet=RegSetValueEx(hKey,"ExcelFilePath",NULL,REG_SZ,(BYTE*)(LPCTSTR)m_strPrecFilePath,
						  strlen(m_strPrecFilePath)+sizeof(TCHAR));
	}
	//////

	::RegCloseKey(hKey);
	
}


/////////////////////////////////////////////////////////////////////
//
// ��Ӧ���Կؼ�ѡ�е�֪ͨ
//
void CImportFromXLSDlg::OnSelectChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPropertyWnd::PWNSelectChangeStruct *pSelect;

	pSelect=(CPropertyWnd::PWNSelectChangeStruct*)pNMHDR;

	this->m_HintInformation=(pSelect->szElementName);

	if(pSelect->pVoid)
	{
		this->m_HintInformation+=_T(":\r\n");

		this->m_HintInformation+=*((CString*)pSelect->pVoid);
	}

	this->UpdateData(FALSE);
	*pResult=0;
}

///////////////////////////////////////////
//
// ��Ӧ��ʼ�����롰
//
void CImportFromXLSDlg::OnBeginImport() 
{
	BOOL bRet;

	this->UpdateData();

	// ��֤�������� �Ƿ���Ч
	bRet=ValidateData();

	if(!bRet)
	{
		return;
	}

	BeginImport();
}

/////////////////////////////////////////////
//
// ��֤�������ݵ���Ч��
//
BOOL CImportFromXLSDlg::ValidateData()
{
	CPropertyWnd::ElementDef ElementDef;
	CString strTemp;

	this->UpdateData();

	//�ж��ļ�·���Ƿ�Ϊ��
	if(this->m_XLSFilePath.IsEmpty())
	{
		AfxMessageBox(_T("�ļ�·������Ϊ��"));
		return FALSE;
	}

	//
	// ������Կؼ���ָ���ĵ�Ԫ�Ƿ�Ϊ��
	//
	for(int i=0;i<this->m_PropertyWnd.GetElementCount();i++)
	{
		LPCTSTR szTableName[]=
		{
			_T("Excel��������"),
			_T("�������ݿ�ʼ�к�"),
			_T("�������������"),
		};

		this->m_PropertyWnd.GetElementAt(&ElementDef,i);

		for(int j=0;j<sizeof(szTableName)/sizeof(szTableName[0]);j++)
		{
			if(ElementDef.szElementName==szTableName[j] && ElementDef.RightElementText.IsEmpty())
			{
				strTemp.Format(_T("%s����Ϊ��"),szTableName[j]); 
				AfxMessageBox(strTemp); 
				return FALSE; 
			}
		}

	}

	return TRUE;
}

//////////////////////////////////////////////////////////
//
// ���ù��̱��
//
// szProjectID[in]	���̱��
//
void CImportFromXLSDlg::SetProjectID(LPCTSTR szProjectID)
{
	if(szProjectID==NULL)
	{
		m_strProjectID=_T("");
	}
	else
	{
		m_strProjectID=szProjectID;
	}
}

//////////////////////////////////////////////////////
//
// ���ع��̵ı��
//
CString CImportFromXLSDlg::GetProjectID()
{
	return m_strProjectID;
}

//////////////////////////////////////////////////////////////////////
//
// �����빤��������ݿ������
//
// IConnection[in]	���ݿ������
//
void CImportFromXLSDlg::SetProjectDbConnect(_ConnectionPtr IConnection)
{
	this->m_ProjectDbConnect=IConnection;
}

////////////////////////////////////////////////////////////////////
//
// ���������ݿ������
//
_ConnectionPtr CImportFromXLSDlg::GetProjectDbConnect()
{
	return this->m_ProjectDbConnect;
}

//////////////////////////////////////////////////////////
//
// �������ڴ����Ϣ���Ӽ���
//
// szRegSubKey[in]	���ڴ����Ϣ���Ӽ���
//
void CImportFromXLSDlg::SetRegSubKey(LPCTSTR szRegSubKey)
{
	if(szRegSubKey)
		m_RegSubKey=szRegSubKey;
}

////////////////////////////////////////////////////////
//
// �������ڴ����Ϣ���Ӽ���
//
CString CImportFromXLSDlg::GetRegSubKey()
{
	return m_RegSubKey;
}

///////////////////////////////////////////////////////////
//
// �������ڶԻ����²���ʾ��Ϣ����ʾ������
//
// szHint[in]	�ڶԻ����²���ʾ��Ϣ����ʾ������
//
void CImportFromXLSDlg::SetHintInformation(LPCTSTR szHint)
{
	if(szHint)
		m_HintInformation=szHint;
	else
		m_HintInformation=_T("");

	if(IsWindow(GetSafeHwnd()))
	{
		UpdateData(FALSE);
	}
}

////////////////////////////////////////////////////////
//
// �������ڶԻ����²���ʾ��Ϣ����ʾ������
//
CString CImportFromXLSDlg::GetHintInformation()
{
	return m_HintInformation;
}

////////////////////////////////////////////////////////
//
// ������Կؼ��Ĵ���
//
CPropertyWnd* CImportFromXLSDlg::GetPropertyWnd()
{
	return &m_PropertyWnd;
}

//////////////////////////////////////////////////
//
// ִ�е������ݵĲ���
//
void CImportFromXLSDlg::BeginImport()
{

}

BOOL CImportFromXLSDlg::ImportExcelToAccess(ImportFromXLSStruct *pImportStruct)
{
	CString strSQL;			//SQL���
	CString strTblName = pImportStruct->ProjectDBTableName;		//ACCESS���ݿ�ı���
	long nBeginRow		= pImportStruct->BeginRow;
	long nExcelRecCount = pImportStruct->RowCount;
	long nAllFieldCount = pImportStruct->ElementNum;			//�ֶθ���
	CString strExcelFileName = pImportStruct->XLSFilePath;
	CString strWorksheetName = pImportStruct->XLSTableName;
	ImportFromXLSElement * pAllFieldStruct = pImportStruct->pElement;
	CString strTmp;
	_variant_t varTmp;
	long	nColNO;
	int		i;
	_ConnectionPtr pConDes;
	_RecordsetPtr pRsExcel;
	_RecordsetPtr pRsAccess;
	pRsExcel.CreateInstance(__uuidof(Recordset));
	pRsAccess.CreateInstance(__uuidof(Recordset));
	try
	{
		pConDes = GetProjectDbConnect();
		if ( pConDes == NULL )
		{
			return FALSE;
		}
		if ( pImportStruct == NULL )
		{
			return FALSE;
		}
		//����EXCEL�ļ�
		if ( !OpenExcelTable( pRsExcel, strWorksheetName, strExcelFileName ) )
		{
			return FALSE;
		}
		if (pRsExcel->adoEOF && pRsExcel->BOF)
		{
			return TRUE;
		}
		//ʹ�ü�¼���ķ�ʽ����EXCEL�ļ�ʱ�����һ���ֶδ��ڶ�����������ʱ����һЩ���ݿ���ȡ��������(�ֶ��е�ֵʹ������һ��������Ϊ�����ֶε����͡���
		//���µĴ��뿼���ˣ���һ���ֶ����������ͺ��ַ��ͣ����ַ���ȫ��ɾ�������������͵�ֵ����ȡ������
		// �滻��¼���еĿ��ַ���  [2005/12/06]
		if (nExcelRecCount <= 0)
		{
			nExcelRecCount = pRsExcel->GetRecordCount();
		}
		for (pRsExcel->MoveFirst(), i = 2; !pRsExcel->adoEOF && i < nBeginRow; pRsExcel->MoveNext(), i++)
			;		//��λ����ʼ��
		for (; !pRsExcel->adoEOF; pRsExcel->MoveNext())
		{
			for (int i=0; i < nAllFieldCount; i++)
			{
				try
				{
					//����ֶ���Ϊ��ʱ����� 
					if (pAllFieldStruct[i].SourceFieldName.IsEmpty() || pAllFieldStruct[i].DestinationName.IsEmpty())
						continue;
					StringToNumber( pAllFieldStruct[i].SourceFieldName, nColNO );

					varTmp = pRsExcel->GetCollect(_variant_t( nColNO ));
					if ( varTmp.vt == VT_BSTR && vtos( varTmp ).IsEmpty() )
					{	//ɾ�����ַ���
						pRsExcel->PutCollect(_variant_t( nColNO ), _variant_t("") );
						pRsExcel->Update();
					}
				}
				catch (_com_error) 
				{
					continue;
				}
			}
		}

		//��������EXCEL�ļ�
		m_pConExcel->Close();
		if ( !OpenExcelTable( pRsExcel, strWorksheetName, strExcelFileName ) )
		{
			return FALSE;
		}
		//����֮ǰ����������ݱ�
		pConDes->Execute(_bstr_t("DELETE * FROM ["+strTblName+"]"), NULL, -1);
		//��ACCESSԭʼ���ݱ�
		strSQL = "SELECT * FROM ["+strTblName+"] ";
		pRsAccess->Open(_variant_t(strSQL), pConDes.GetInterfacePtr(), adOpenKeyset, adLockOptimistic, adCmdText);
		
		if (nExcelRecCount <= 0)
		{
			nExcelRecCount = pRsExcel->GetRecordCount();
		}
		for (pRsExcel->MoveFirst(), i = 2; !pRsExcel->adoEOF && i < nBeginRow; pRsExcel->MoveNext(), i++)
			;

		//��ACCESSԭʼ���д�����ǰ�������ͬһ���͵ļ�¼��
		for (; !pRsExcel->adoEOF && nExcelRecCount > 0; pRsExcel->MoveNext(), nExcelRecCount--)
		{
			pRsAccess->AddNew();
			for (int i=0; i < nAllFieldCount; i++)
			{
				try
				{
					//����ֶ���Ϊ��ʱ����� 
					if (pAllFieldStruct[i].SourceFieldName.IsEmpty() || pAllFieldStruct[i].DestinationName.IsEmpty())
						continue;
					StringToNumber( pAllFieldStruct[i].SourceFieldName, nColNO );

					varTmp = pRsExcel->GetCollect(_variant_t( nColNO ));
				}catch (_com_error& e) 
				{
					if (e.Error() == -2146825023)
					{
						strTmp = "ԭʼ���ݱ���û���ֶ� '"+pAllFieldStruct[i].SourceFieldName+"' ��";
						AfxMessageBox(strTmp);
						continue;
					}
					AfxMessageBox(e.Description());
					return FALSE;
				}				
				pRsAccess->PutCollect(_variant_t(pAllFieldStruct[i].DestinationName),varTmp);
			}
			pRsAccess->Update();
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}	
	return TRUE;
}

void CImportFromXLSDlg::StringToNumber(CString strNO, long &nNO)
{
	nNO = -1;
	TCHAR cNum;
	strNO.MakeUpper();
	int  nLeng = strNO.GetLength();
	for (int i = 1; i <= nLeng; i++)
	{ 
		cNum = strNO[nLeng - i];
		if ( 1 == i )
			nNO = cNum - 'A';
		else
			nNO += ( cNum - 'A' + 1 ) * 26;
	}
}


//------------------------------------------------------------------
// DATE         :[2005/12/05]
// Parameter(s) :
// Return       :
// Remark       :��ADO�ķ�ʽ������EXCEL�ļ�.
//------------------------------------------------------------------
BOOL CImportFromXLSDlg::ConnectExcelFile(const CString strExcelName, _ConnectionPtr &pConExcel)
{
	CString strSQL;
	try
	{
		if (pConExcel == NULL)
		{
			pConExcel.CreateInstance(__uuidof(Connection));
		}
		if ( pConExcel->State == adStateOpen )
		{
			pConExcel->Close();
		}

		strSQL = CONNECTSTRING_EXCEL + strExcelName;
		pConExcel->Open(_bstr_t(strSQL), "", "", adCmdUnspecified);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;

}

//------------------------------------------------------------------
// DATE         :[2005/12/05]
// Parameter(s) :
// Return       :
// Remark       :��ADO��¼���ķ�ʽ�򿪣�EXCEL�ļ��еı�
//------------------------------------------------------------------
BOOL CImportFromXLSDlg::OpenExcelTable(_RecordsetPtr pRsTbl, CString& strSheetName, CString strExcelFileName)
{ 
	CString strSQL;
	try
	{
		if ( m_pConExcel == NULL || m_pConExcel->State == adStateClosed )
		{
			//ʹ��һ����Ա�����ݿ����Ӷ���
			if ( !ConnectExcelFile( strExcelFileName, m_pConExcel ) )
			{
				return FALSE;
			}
		}
		if (pRsTbl == NULL)
		{
			pRsTbl.CreateInstance(__uuidof(Recordset));
		}
		try
		{
			if (pRsTbl->State == adStateOpen)
			{
				pRsTbl->Close();
			}
			strSQL = "SELECT * FROM ["+strSheetName+"$] ";		//��EXCEL�еı�������$
			pRsTbl->Open(_variant_t(strSQL), m_pConExcel.GetInterfacePtr(), adOpenKeyset, adLockOptimistic, adCmdText);
			strSheetName += "$";								//�ڵ��øú���֮��ĳ�������ΪEXCEL����
		}
		catch (_com_error)
		{
			strSQL = "SELECT * FROM ["+strSheetName+"] ";
			pRsTbl->Open(_variant_t(strSQL), m_pConExcel.GetInterfacePtr(), adOpenKeyset, adLockOptimistic, adCmdText);
		} 
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}

