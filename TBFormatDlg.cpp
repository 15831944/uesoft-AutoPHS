// TBFormatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autophs.h"
#include "TBFormatDlg.h"

#include "EDIBgbl.h"
#include "EDIBDB.h"
#include "basDirectory.h"
#include "user.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTBFormatDlg dialog


CTBFormatDlg::CTBFormatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTBFormatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTBFormatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTBFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTBFormatDlg)
	DDX_Control(pDX, IDC_TB_DATAGRID, m_DataGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTBFormatDlg, CDialog)
	//{{AFX_MSG_MAP(CTBFormatDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTBFormatDlg message handlers

BOOL CTBFormatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strSQL;
	// TODO: Add extra initialization here
	m_connSort.CreateInstance(__uuidof(Connection));
	try
	{
		m_rs.CreateInstance(__uuidof(Recordset));
		m_rs->CursorLocation = adUseClient;
		m_connSort->Open(_bstr_t(::dbConnectionString + basDirectory::DBShareDir + (_T("DrawingSize.mdb"))),(_T("")),(_T("")),adConnectUnspecified);//20071101 "ProjectDBDir" ��Ϊ "DBShareDir";"Sort.mdb" ��Ϊ "DrawingSize.mdb"
		strSQL.Format(_T("SELECT SJHY AS ��ҵ����,TB0LEN AS A0ͼ��Ŀ��,TB0HEI AS A0ͼ��ĸ߶�,TB2LEN AS A2ͼ��Ŀ��,TB2HEI AS A2ͼ��ĸ߶�,A0PrjNameWidth AS A0ͼ�������ƿ��,A2PrjNameWidth AS A2ͼ�������ƿ��,A0DrawNoWidth AS A0ͼͼֽͼ�ſ��,A2DrawNoWidth AS A2ͼͼֽͼ�ſ��,A0VlmNameWidth AS A0ͼ������ƿ��,A2VlmNameWidth AS A2ͼ������ƿ��,A0DrawNameWidth AS A0ͼͼֽ���ƿ��,A2DrawNameWidth AS A2ͼͼֽ���ƿ�� FROM [DRAWSIZE] ORDER BY [SJHYID]"));
		m_rs->Open(_variant_t(strSQL),(IDispatch*)m_connSort,adOpenStatic,adLockOptimistic,adCmdText);
		m_DataGrid.SetRefDataSource(m_rs->GetDataSource());
	}catch(_com_error e)
	{
		CString strErrorMsg;
		strErrorMsg.Format(_T("%s: %d, %s"), __FILE__, __LINE__, e.Description());
		AfxMessageBox(strErrorMsg);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTBFormatDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_DataGrid.UpdateData();
	CDialog::OnOK();
}
