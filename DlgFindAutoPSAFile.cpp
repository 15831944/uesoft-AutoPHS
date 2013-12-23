// DlgFindAutoPSAFile.cpp : implementation file
//

#include "stdafx.h"
#include "autophs.h"
#include "DlgFindAutoPSAFile.h"
#include "modregistry.h"
#include "user.h"
#include "modphscal.h"
#include "EDIBgbl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFindAutoPSAFile dialog


CDlgFindAutoPSAFile::CDlgFindAutoPSAFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindAutoPSAFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFindAutoPSAFile)
    m_iFileOpen = 0;
	//}}AFX_DATA_INIT
	m_bFlag = 0 ;
}


void CDlgFindAutoPSAFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFindAutoPSAFile)
	DDX_Control(pDX, IDC_LIST1, m_ctrlFileList);
    DDX_Radio(pDX, IDC_RADIO_PHS, m_iFileOpen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFindAutoPSAFile, CDialog)
	//{{AFX_MSG_MAP(CDlgFindAutoPSAFile)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkFind)
	ON_BN_CLICKED(IDCONFIRM, OnConfirm)
	ON_BN_CLICKED(IDC_RADIO_PHS, OnRecenfile)
	ON_BN_CLICKED(IDC_RADIO_PSA, OnRecenfile)
	ON_BN_CLICKED(IDC_RADIO_CAESARII, OnRecenfile)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFindAutoPSAFile message handlers

BOOL CDlgFindAutoPSAFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ctrlFileList.InsertColumn(0, "���", LVCFMT_LEFT, 40);
	m_ctrlFileList.InsertColumn(2, "�ļ�·��", LVCFMT_LEFT, 500);

	DWORD styles = m_ctrlFileList.GetExtendedStyle();
	m_ctrlFileList.SetExtendedStyle( styles|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

	modPHScal::g_iPSAFilterIndex=1;//�ļ��Ի������ļ�������ΪAutoPSA/CAESARII(*.out,*.mdb)����

	if (GetPHSRecentFile() == 0)
	{ //PHSû�д򿪹��ļ�      
        if (GetPSARecentFile() != 0)
		{//PSA�򿪹��ļ�
            m_iFileOpen = 1;
	        this->UpdateData(false);
		}
    }
	else
	{
		m_iFileOpen = 0;
	    this->UpdateData(false);
	}
	
	if( m_nCount == 0 )
	{//PSA��PHS��û�д򿪹��ļ�
		m_bFlag = 0;//��û���ļ�ʱ,�������Ի���.m_Flag������Ϊһ��״̬
		CDialog::OnOK();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//˫��.
//��ѡ����ļ�����Ϊ��ʼ·��.
void CDlgFindAutoPSAFile::OnDblclkFind(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int pos = m_ctrlFileList.GetSelectionMark();	
	if( pos >= 0 && pos < m_nCount )
	{
		modPHScal::gsPSAOutFilePath = m_strFileList[pos];
		CDialog::OnOK();
	}
	*pResult = 0;
	m_bFlag = 1;  //ѡ��״̬.
}

void CDlgFindAutoPSAFile::OnOK() 
{
	//ѡ����� 
	//modPHScal::gsPSAOutFilePath = "";//pfg20050309
	this->m_bFlag = 2;
	CDialog::OnOK();
}

void CDlgFindAutoPSAFile::OnCancel() 
{
	m_bFlag = 0;
	CDialog::OnCancel();
}

void CDlgFindAutoPSAFile::OnConfirm() 
{
	//ѡ��ȷ��
    int pos = m_ctrlFileList.GetSelectionMark();	
	if( pos >= 0 && pos < m_nCount )
	{
		modPHScal::gsPSAOutFilePath = m_strFileList[pos];
		m_bFlag = 1;  //ѡ��״̬.
		CDialog::OnOK();
	}
//	*pResult = 0;
}

/*****************************************************
 �������ܣ������û���ѡ������ʾ������ļ���·��
 �������:��
 �������:��
 ����ֵ:��
*****************************************************/
void CDlgFindAutoPSAFile::OnRecenfile()
{
	this->UpdateData(true);
    m_ctrlFileList.DeleteAllItems();//���list ctrl�ؼ�����ʾ����

	if (m_iFileOpen == 2)
	{//ѡ���CAESARII�򿪵��ļ�
		modPHScal::g_iPSAFilterIndex=3;//�ļ��Ի������ļ�������ΪCAESARII(*.mdb)����
        GetCAESARIIRecentFile();
	}
	else if (m_iFileOpen == 1)
	{//ѡ���PSA�򿪵��ļ�
		modPHScal::g_iPSAFilterIndex=2;//�ļ��Ի������ļ�������ΪAutoPSA(*.out)����
        GetPSARecentFile();
	}
	else
	{//ѡ���PHS�򿪵��ļ�
		modPHScal::g_iPSAFilterIndex=1;//�ļ��Ի������ļ�������ΪAutoPSA/CAESARII(*.out,*.mdb)����
        GetPHSRecentFile();
	}
}

/**************************************************************************
 �������ܣ���ע����еõ�PSA����򿪵��ļ�·��,����list ctrl�ؼ�����ʾ����
 �������:��
 �������:��
 ����ֵ:�õ�����ʾ�Ĵ�PSA�򿪵��ļ�������
***************************************************************************/
int CDlgFindAutoPSAFile::GetPSARecentFile()
{
	  CString strValue;//�ļ�·�� 
	  CString strItem;//���
	  CString strPathNameKey;//AutoPSA7.0��1.1·��
	  LVITEM  lvItem;
	  short i=0;//����ļ�����
	  m_nCount = 0;

	  //���Ȼ�ȡAutoPSA 7.0�汾ע���
	  strPathNameKey.Format("%s8.0\\",szAutoPSAKey);
	  for( i=1; i<=8; i++ )
	  {//�����ʾ8���ļ�·��
		strItem.Format("File%d", i);
		strValue = GetUSER_RegKey(strPathNameKey, "Recent File List", strItem, "");
		if (!strValue.IsEmpty() && FileExists(strValue))
		{
			m_strFileList[m_nCount] = strValue;
			strItem.Format("%d", m_nCount+1);
			m_ctrlFileList.InsertItem(m_nCount, strItem);
			lvItem.mask = LVIF_TEXT ;
			lvItem.iItem = m_nCount;
			lvItem.iSubItem = 1;
			lvItem.pszText = strValue.GetBuffer(255);

			m_ctrlFileList.SetItem(&lvItem);
			m_nCount++;
		}
	  }
	  
	  //��λ�ȡAutoPSA 1.1�汾ע���
	  strPathNameKey.Format("%s1.1\\",szAutoPSAKey);
	  for( i=1; i<=8; i++ )
	  {//�����ʾ8���ļ�·��
		strItem.Format("File%d", i);
		strValue = GetUSER_RegKey(strPathNameKey, "Recent File List", strItem, "");
		if (!strValue.IsEmpty() && FileExists(strValue))
		{
			m_strFileList[m_nCount] = strValue;
			strItem.Format("%d", m_nCount+1);
			m_ctrlFileList.InsertItem(m_nCount, strItem);
			lvItem.mask = LVIF_TEXT ;
			lvItem.iItem = m_nCount;
			lvItem.iSubItem = 1;
			lvItem.pszText = strValue.GetBuffer(255);

			m_ctrlFileList.SetItem(&lvItem);
			m_nCount++;
		}
	  }
	  
	  return m_nCount;
}

/**************************************************************************
 �������ܣ������ݿ��еõ�PHS����򿪵��ļ�·��,����list ctrl�ؼ�����ʾ����
 �������:��
 �������:��
 ����ֵ:�õ�����ʾ�Ĵ�PHS�򿪵��ļ�������
**************************************************************************/
int CDlgFindAutoPSAFile::GetPHSRecentFile()
{
	 CDaoRecordset pfileroad;
	 pfileroad.m_pDatabase = &EDIBgbl::dbPRJDB;
	 CString fileSQL;
	
     fileSQL.Format("SELECT * FROM ZY WHERE [VolumeID]=%d Order by FileNameID DESC",EDIBgbl::SelVlmID);
	 pfileroad.Open(dbOpenDynaset,fileSQL);//�õ����������ļ�¼��

     CString strValue;//�ļ�·�� 
	 CString strItem;//���
   	 LVITEM  lvItem;
	 m_nCount = 0;
	 
	 while (!pfileroad.IsEOF() && m_nCount<8)
	 {//�з��������ļ�¼������ʾ��·��û�г���8��
		strValue = vtos(pfileroad.GetFieldValue(2));//�õ��ļ�·��

		if (!strValue.IsEmpty() && FileExists(strValue))
		{
			m_strFileList[m_nCount] = strValue;
			strItem.Format("%d", m_nCount+1);
			m_ctrlFileList.InsertItem(m_nCount, strItem);
			lvItem.mask = LVIF_TEXT ;
			lvItem.iItem = m_nCount;
			lvItem.iSubItem = 1;
			lvItem.pszText = strValue.GetBuffer(255);

			m_ctrlFileList.SetItem(&lvItem);
			m_nCount++;
		}
		pfileroad.MoveNext();//�õ���һ����¼
	 }

	 return m_nCount;
}

/**************************************************************************
 �������ܣ���ע����еõ�CAESARII����򿪵��ļ�·��,����list ctrl�ؼ�����ʾ����
 �������:��
 �������:��
 ����ֵ:�õ�����ʾ�Ĵ�CAESARII�򿪵��ļ�������
***************************************************************************/
short CDlgFindAutoPSAFile::GetCAESARIIRecentFile()
{
	  CString strValue;//�ļ�·�� 
	  CString strItem;//���
	  CString strPathNameKey = szCAESARIIKey;
	  LVITEM  lvItem;
	  m_nCount = 0;

	  for( int i=1; i<=8; i++ )
	  {//�����ʾ8���ļ�·��
		strItem.Format("File%d", i);
		strValue = GetUSER_RegKey(strPathNameKey, "Recent ._A Files", strItem, "");
		if (!strValue.IsEmpty() && FileExists(strValue))
		{
			m_strFileList[m_nCount] = strValue;
			strItem.Format("%d", m_nCount+1);
			m_ctrlFileList.InsertItem(m_nCount, strItem);
			lvItem.mask = LVIF_TEXT ;
			lvItem.iItem = m_nCount;
			lvItem.iSubItem = 1;
			lvItem.pszText = strValue.GetBuffer(255);

			m_ctrlFileList.SetItem(&lvItem);
			m_nCount++;
		}
	  }
	  
	  return m_nCount;

}

void CDlgFindAutoPSAFile::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int pos = m_ctrlFileList.GetSelectionMark();	
	if( pos >= 0 && pos < m_nCount )
	{
		modPHScal::gsPSAOutFilePath = m_strFileList[pos];
	}
	*pResult = 0;
	m_bFlag = 1;  //ѡ��״̬.
}
