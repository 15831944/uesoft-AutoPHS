///////////////////////////////////////////////
// �������öԻ�����ʵ���ļ�
// PreStylePag.cpp
// ���&��̣��޳�
// ��     �ڣ�2003/10/27

#include "stdafx.h"
#include "math.h"
#include "user.h"
#include "basDirectory.h"

#include "PreStylePag.h"
#include "autophs.h"
#include "NewStyleDlg.h"
#include "modPHScal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAutoPHSApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CPreStylePag property page

IMPLEMENT_DYNCREATE(CPreStylePag, CPropertyPage)

CPreStylePag::CPreStylePag() : CPropertyPage(CPreStylePag::IDD)
{
	//{{AFX_DATA_INIT(CPreStylePag)
	m_blBackward = FALSE;
	m_blUp2Down = FALSE;
	m_blVertical = FALSE;
	m_dHeight = 0.0;
	m_dAngle = 0.0;
	m_dScaleFactor = 0.0;
	m_nBigFonts = 0;
	m_nShxFonts = 0;
	m_nStyle = 0;
	m_nTable = 0;
	m_nAlign = 0;
	m_strMsg = _T("");
	//}}AFX_DATA_INIT
	ActiveTimes = 0;
}

CPreStylePag::~CPreStylePag()
{
}

void CPreStylePag::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreStylePag)
	DDX_Check(pDX, IDC_CHECK_BACKWARD, m_blBackward);
	DDX_Check(pDX, IDC_CHECK_UP2DOWN, m_blUp2Down);
	DDX_Check(pDX, IDC_CHECK_VERTICAL, m_blVertical);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_dHeight);
	DDV_MinMaxDouble(pDX, m_dHeight, 0., 100000000.);
	DDX_Text(pDX, IDC_EDIT_OBLIQUEANGLE, m_dAngle);
	DDV_MinMaxDouble(pDX, m_dAngle, -85., 85.);
	DDX_Text(pDX, IDC_EDIT_SCALEFACTOR, m_dScaleFactor);
	DDV_MinMaxDouble(pDX, m_dScaleFactor, 0., 100000000.);
	DDX_CBIndex(pDX, IDC_COMBO_BIGFONTS, m_nBigFonts);
	DDX_CBIndex(pDX, IDC_COMBO_SHXFONTS, m_nShxFonts);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nStyle);
	DDX_CBIndex(pDX, IDC_COMBO_TABLE, m_nTable);
	DDX_CBIndex(pDX, IDC_COMBO_ALIGN, m_nAlign);
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreStylePag, CPropertyPage)
	//{{AFX_MSG_MAP(CPreStylePag)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE, OnSelchangeComboTable)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnSelchangeComboStyle)
	ON_BN_CLICKED(IDC_BTN_NEWSTYLE, OnBtnNewstyle)
	ON_BN_CLICKED(IDC_BTN_DELETESTYLE, OnBtnDeletestyle)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, OnBtnDefault)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreStylePag message handlers

void CPreStylePag::SaveIniValue(void)
{
    CString   strStyle, strShx, strBig;
    CComboBox *lpStyle, *lpShx, *lpBig;
    int       nShx, nBig, iGenFlag;
    BOOL      blIsFailed;

    UpdateTableData(TRUE);
	
    lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
    lpShx = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    lpBig = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);
    nShx = lpShx->GetCount();
    nBig = lpBig->GetCount();


    //���浱ǰ��ʽֵ
	if(lpStyle->GetCount()>0)//pfg20050330
	{
	 lpStyle->GetLBText(m_nStyle, strStyle);
	}
    lpShx->GetLBText(m_nShxFonts, strShx);
    lpBig->GetLBText(m_nBigFonts, strBig);
    iGenFlag = 0;
    if(m_blBackward == TRUE) iGenFlag += 2;
    if(m_blUp2Down == TRUE)   iGenFlag += 4;
    ModifyTextStyleProperty(strStyle, strShx, strBig,
        m_dHeight, m_dScaleFactor, m_dAngle, iGenFlag);

    //�������б��ĵ�ǰ����
    pCurTable = &dim_style;
    SaveTable2Registry();

    pCurTable = &ljmx_style;
    SaveTable2Registry();

    pCurTable = &load_style;
    SaveTable2Registry();

    pCurTable = &Comments_style;
    SaveTable2Registry();

    pCurTable = &BH_style;
    SaveTable2Registry();

    pCurTable = &BG_style;
    SaveTable2Registry();

    pCurTable = &other_style;
    SaveTable2Registry();

    //���Ի����еĿؼ�����ֵ���浽modPHScal���Ա������

    blIsFailed = FALSE;
    if(nShx <= 0) blIsFailed = TRUE;
    if(nBig <= 0) blIsFailed = TRUE;
    if(dim_style.nShxFonts >= nShx) blIsFailed = TRUE;
    if(dim_style.nBigFonts >= nBig) blIsFailed = TRUE;
    if(dim_style.nShxFonts <  0) blIsFailed = TRUE;
    if(dim_style.nBigFonts <  0) blIsFailed = TRUE;

    if(blIsFailed)
    {
        strShx = _T("txt.shx");
        strBig = _T("gbcbig.shx");
    }
    else
    {
        lpShx->GetLBText(dim_style.nShxFonts, strShx);
        if(dim_style.nBigFonts == 0)
        {
            strBig = " ";
        }
        else
        {
            lpBig->GetLBText(dim_style.nBigFonts, strBig);
        }
    }
    modPHScal::strSHXFontDim  = strShx;
    modPHScal::strBigFontDim  = strBig;

    blIsFailed = FALSE;
    if(nShx <= 0) blIsFailed = TRUE;
    if(nBig <= 0) blIsFailed = TRUE;
    if(ljmx_style.nShxFonts >= nShx) blIsFailed = TRUE;
    if(ljmx_style.nBigFonts >= nBig) blIsFailed = TRUE;
    if(ljmx_style.nShxFonts <  0) blIsFailed = TRUE;
    if(ljmx_style.nBigFonts <  0) blIsFailed = TRUE;

    if(blIsFailed)
    {
        strShx = _T("txt.shx");
        strBig = _T("gbcbig.shx");
    }
    else
    {
        lpShx->GetLBText(ljmx_style.nShxFonts, strShx);
        if(ljmx_style.nBigFonts == 0)
        {
            strBig = " ";
        }
        else
        {
            lpBig->GetLBText(ljmx_style.nBigFonts, strBig);
        }
    }
    modPHScal::strSHXFontLjmx = strShx;
    modPHScal::strBigFontLjmx = strBig;

    blIsFailed = FALSE;
    if(nShx <= 0) blIsFailed = TRUE;
    if(nBig <= 0) blIsFailed = TRUE;
    if(load_style.nShxFonts >= nShx) blIsFailed = TRUE;
    if(load_style.nBigFonts >= nBig) blIsFailed = TRUE;
    if(load_style.nShxFonts <  0) blIsFailed = TRUE;
    if(load_style.nBigFonts <  0) blIsFailed = TRUE;

    if(blIsFailed)
    {
        strShx = _T("txt.shx");
        strBig = _T("gbcbig.shx");
    }
    else
    {
        lpShx->GetLBText(load_style.nShxFonts, strShx);
        if(load_style.nBigFonts == 0)
        {
            strBig = " ";
        }
        else
        {
            lpBig->GetLBText(load_style.nBigFonts, strBig);
        }
    }
    modPHScal::strSHXFontLoad = strShx;
    modPHScal::strBigFontLoad = strBig;

    modPHScal::fFWDim  = dim_style.dScaleFactor;
    modPHScal::fFWLjmx = ljmx_style.dScaleFactor;
    modPHScal::fFWLoad = load_style.dScaleFactor;

    modPHScal::iAlignDim  = dim_style.nAlign;
    modPHScal::iAlignLjmx = ljmx_style.nAlign;
    modPHScal::iAlignLoad = load_style.nAlign;

    //���Ի����еĿؼ�����ֵ���浽ע�����Ӧ�ֶ���
	SetRegValue(_T("Settings"), _T("CADBigFontDim"),  modPHScal::strBigFontDim);
	SetRegValue(_T("Settings"), _T("CADBigFontLjmx"), modPHScal::strBigFontLjmx);
	SetRegValue(_T("Settings"), _T("CADBigFontLoad"), modPHScal::strBigFontLoad);
	
	SetRegValue(_T("Settings"), _T("CADFontWidthFactorDim"),  modPHScal::fFWDim);
	SetRegValue(_T("Settings"), _T("CADFontWidthFactorLjmx"), modPHScal::fFWLjmx);
	SetRegValue(_T("Settings"), _T("CADFontWidthFactorLoad"), modPHScal::fFWLoad);

	SetRegValue(_T("Settings"), _T("CADSHXFontDim"),  modPHScal::strSHXFontDim);
	SetRegValue(_T("Settings"), _T("CADSHXFontLjmx"), modPHScal::strSHXFontLjmx);
	SetRegValue(_T("Settings"), _T("CADSHXFontLoad"), modPHScal::strSHXFontLoad);

	SetRegValue(_T("Settings"), _T("CADAlignDim"),  modPHScal::iAlignDim);
	SetRegValue(_T("Settings"), _T("CADAlignLjmx"), modPHScal::iAlignLjmx);
    SetRegValue(_T("Settings"), _T("CADAlignLoad"), modPHScal::iAlignLoad);
}



void CPreStylePag::SetCtrlValue(void)
{
    CString strShx, strBig;
    CComboBox *lpShx, *lpBig;

    lpShx = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    lpBig = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);

    //��modPHScal���Ա������ֵ��ʼ���Ի����еĿؼ�����

    strShx  = modPHScal::strSHXFontDim;
    strBig  = modPHScal::strBigFontDim;
    dim_style.nShxFonts = lpShx->FindString(0, strShx);
    dim_style.nBigFonts = lpBig->FindString(0, strBig);
    if(dim_style.nShxFonts == -1)
    {
        lpShx->AddString(strShx);
        dim_style.nShxFonts = lpShx->GetCount() -1;
    }
    if(dim_style.nBigFonts == -1)
    {
        lpBig->AddString(strBig);
        dim_style.nBigFonts = lpBig->GetCount() -1;
    }

    strShx = modPHScal::strSHXFontLjmx;
    strBig = modPHScal::strBigFontLjmx;
    ljmx_style.nShxFonts = lpShx->FindString(0, strShx);
    ljmx_style.nBigFonts = lpBig->FindString(0, strBig);
    if(ljmx_style.nShxFonts == -1)
    {
        lpShx->AddString(strShx);
        ljmx_style.nShxFonts = lpShx->GetCount() -1;
    }
    if(ljmx_style.nBigFonts == -1)
    {
        lpBig->AddString(strBig);
        ljmx_style.nBigFonts = lpBig->GetCount() -1;
    }
    
    strShx = modPHScal::strSHXFontLoad;
    strBig = modPHScal::strBigFontLoad;
    load_style.nShxFonts = lpShx->FindString(0, strShx);
    load_style.nBigFonts = lpBig->FindString(0, strBig);
    if(load_style.nShxFonts == -1)
    {
        lpShx->AddString(strShx);
        load_style.nShxFonts = lpShx->GetCount() -1;
    }
    if(load_style.nBigFonts == -1)
    {
        lpBig->AddString(strBig);
        load_style.nBigFonts = lpBig->GetCount() -1;
    }

    dim_style.dScaleFactor  = modPHScal::fFWDim;
    ljmx_style.dScaleFactor = modPHScal::fFWLjmx;
    load_style.dScaleFactor = modPHScal::fFWLoad;

    dim_style.nAlign  = modPHScal::iAlignDim;
    ljmx_style.nAlign = modPHScal::iAlignLjmx;
    load_style.nAlign = modPHScal::iAlignLoad;
}

BOOL CPreStylePag::UpdateTableData(BOOL bSaveAndValidate)
{
    //����򷵻ص�ǰ������
	double dheight;
    if(bSaveAndValidate == TRUE)//���棺���������ݱ��浽�ڴ����
    {
        if(UpdateData(TRUE) == FALSE) return FALSE;
		if(m_dHeight<3)//pfg20050330
		{
			dheight=3;//pfg20050330
		}
		else
		{
			dheight=m_dHeight;//pfg20050330
		}
		SetRegValue(_T("Settings"), _T("��ע�ߴ�-HEIGHT"), dheight);//PFG20050329
        pCurTable->blBackward   = m_blBackward   ;
        pCurTable->blUp2Down    = m_blUp2Down    ;
        pCurTable->blVertical   = m_blVertical   ;

        pCurTable->dHeight      = SetPrecision(m_dHeight, -2)      ;
        pCurTable->dAngle       = SetPrecision(m_dAngle, -2)       ;
        pCurTable->dScaleFactor = SetPrecision(m_dScaleFactor, -2) ;

        m_dHeight               = pCurTable->dHeight      ;
        m_dAngle                = pCurTable->dAngle       ;
        m_dScaleFactor          = pCurTable->dScaleFactor ;

        pCurTable->nBigFonts    = m_nBigFonts    ;
        pCurTable->nShxFonts    = m_nShxFonts    ;
        pCurTable->nStyle       = m_nStyle       ;
        //pCurTable->nTable     ֻ������         ;
        pCurTable->nAlign       = m_nAlign       ;

        UpdateData(FALSE);
		return TRUE;
    }
    else // == FALSE�����أ���������ʾ��������
    {
        m_blBackward   = pCurTable->blBackward   ;
        m_blUp2Down    = pCurTable->blUp2Down    ;
        m_blVertical   = pCurTable->blVertical   ;

        m_dHeight      = SetPrecision(pCurTable->dHeight, -2)      ;
        m_dAngle       = SetPrecision(pCurTable->dAngle, -2)       ;
        m_dScaleFactor = SetPrecision(pCurTable->dScaleFactor, -2) ;

        pCurTable->dHeight      = m_dHeight      ;
        pCurTable->dAngle       = m_dAngle       ;
        pCurTable->dScaleFactor = m_dScaleFactor ;

        m_nBigFonts    = pCurTable->nBigFonts    ;
        m_nShxFonts    = pCurTable->nShxFonts    ;
        m_nStyle       = pCurTable->nStyle       ;
        m_nTable       = pCurTable->nTable       ;
        m_nAlign       = pCurTable->nAlign       ;

        return UpdateData(FALSE);
    }
}

void CPreStylePag::OnSelchangeComboTable() 
{
    int nOldTable;
    int nOldStyle;
    CComboBox *lpStyle, *lpShx, *lpBig;
    CString    strStyleName, strShxFile, strBigFile;
    short      iGenerationFlag;

    nOldTable = m_nTable;
    nOldStyle = m_nStyle;

    UpdateTableData(TRUE);
	m_dHeight=0;//pfg20050330
    //ѡ��û�и���
    if(nOldTable == m_nTable) return ;

    //�û�ѡ���˲�ͬ�ı�
    //���浱ǰ��ʽ������ֵ
    lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
    lpShx   = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    lpBig   = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);
	if(lpStyle->GetCount()>0)//pfg20050330
	{
		lpStyle->GetLBText(nOldStyle, strStyleName);
	}
    lpShx  ->GetLBText(m_nShxFonts, strShxFile);
    lpBig  ->GetLBText(m_nBigFonts, strBigFile);

    iGenerationFlag = 0;
    if(m_blBackward == TRUE) iGenerationFlag += 2;
    if(m_blUp2Down  == TRUE) iGenerationFlag += 4;

    //������ı����ĵ���ʽ����
    ModifyTextStyleProperty(strStyleName, strShxFile, strBigFile,
        m_dHeight, m_dScaleFactor, m_dAngle, iGenerationFlag);

    switch(m_nTable)
    {
    case 0://ѡ���� �����ϸ��
        pCurTable = &ljmx_style;
        break;
    case 1://ѡ���� ��ע�ߴ�
        pCurTable = &dim_style;
		m_dHeight=GetRegKey(_T("Settings"), _T("��ע�ߴ�-HEIGHT"),3);//pfg20050330
        break;
    case 2://ѡ���� ����λ�Ʊ�
        pCurTable = &load_style;
        break;
    case 3://ѡ���� ע��
        pCurTable = &Comments_style;
        break;
    case 4://ѡ���� ���
        pCurTable = &BH_style;
        break;
    case 5://ѡ���� ���
        pCurTable = &BG_style;
        break;
    case 6://ѡ���� ��������
        pCurTable = &other_style;
        break;
    default://����ѡ�񣬸���������ʾ��Ϣ
#ifdef _DEBUG
        MessageBox("�������ñ�ǩ-[ѡ����]���ִ����û���ѡ����ܳ��ַǷ�ֵ�����ʵ������", "����������", 0);
#endif
        //�����������ݻָ�
        m_nTable = nOldTable;
        break;
    }
    if(m_nTable != nOldTable)
    {
        //�����±�����ʽ���� 
		if(lpStyle->GetCount()>0)//pfg20050330
		{
			lpStyle->GetLBText(pCurTable->nStyle, strStyleName);
		}
        GetTextStyleProperty(strStyleName, strShxFile, strBigFile,
            pCurTable->dHeight, pCurTable->dScaleFactor,
            pCurTable->dAngle, iGenerationFlag);
        
        pCurTable->nShxFonts = lpShx->FindString(0, strShxFile);
        if(pCurTable->nShxFonts == CB_ERR)
        {
            lpShx->AddString(strShxFile);
            pCurTable->nShxFonts = lpShx->GetCount() - 1;
        }

        pCurTable->nBigFonts = lpBig->FindString(0, strBigFile);
        if(pCurTable->nBigFonts == CB_ERR) pCurTable->nBigFonts = 0;
        
        if(iGenerationFlag & 0x02) pCurTable->blBackward = TRUE;
        else                       pCurTable->blBackward = FALSE;
        if(iGenerationFlag & 0x04) pCurTable->blUp2Down  = TRUE;
        else                       pCurTable->blUp2Down  = FALSE;
    }
    UpdateTableData(FALSE);
}

void CPreStylePag::OnSelchangeComboStyle() 
{
    CComboBox *lpStyle, *lpShx, *lpBig;
    CString    strStyleName, strShxFile, strBigFile;
    CString    tmpstr;
    short      iGenerationFlag;
    int        oldStyle, nIndex;

    //��ȡ����ʽ
    oldStyle = m_nStyle;

    //����������
    UpdateTableData(TRUE);

    //�û���ѡ��û�и���
    if(oldStyle == m_nStyle) return ;

    //�û�ѡ���˲�ͬ��style
    lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
    lpShx   = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    lpBig   = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);

    lpStyle->GetLBText(oldStyle, strStyleName);
    lpShx  ->GetLBText(m_nShxFonts, strShxFile);
    lpBig  ->GetLBText(m_nBigFonts, strBigFile);

    iGenerationFlag = 0;
    if(m_blBackward == TRUE) iGenerationFlag += 2;
    if(m_blUp2Down  == TRUE) iGenerationFlag += 4;

    //������ı����ĵ���ʽ����
    ModifyTextStyleProperty(strStyleName, strShxFile, strBigFile,
        m_dHeight, m_dScaleFactor, m_dAngle, iGenerationFlag);

    //��ȡ��style������
    lpStyle->GetLBText(m_nStyle, strStyleName);
    GetTextStyleProperty(strStyleName, strShxFile, strBigFile,
        pCurTable->dHeight, pCurTable->dScaleFactor,
        pCurTable->dAngle, iGenerationFlag);

    pCurTable->nShxFonts = lpShx->FindString(0, strShxFile);
    if(pCurTable->nShxFonts == CB_ERR)
    {
        lpShx->AddString(strShxFile);
        pCurTable->nShxFonts = lpShx->GetCount() - 1;
    }
    pCurTable->nBigFonts = lpBig->FindString(0, strBigFile);
    if(pCurTable->nBigFonts == CB_ERR) pCurTable->nBigFonts = 0;

    if(iGenerationFlag & 0x02) pCurTable->blBackward = TRUE;
    else                       pCurTable->blBackward = FALSE;
    if(iGenerationFlag & 0x04) pCurTable->blUp2Down  = TRUE;
    else                       pCurTable->blUp2Down  = FALSE;

    //���½�����ʾ
    UpdateTableData(FALSE);
}

BOOL CPreStylePag::CreateTextStyle(
                       CString strTSName,
                       CString strShxFile,
                       CString strBigFile,
                       double  dHeight,
                       double  dWidthFactor,
                       double  dAngleByDegree,
                       short   iGenerationFlag)
{
    //�ֲ���������ͳ�ʼ��
    CMObject obj, newobj, tsobj;
    long      i, nCount;
    CString  tmpTSName, strtmp;

    strTSName.TrimLeft();
    strTSName.TrimRight();
    if(strTSName.GetLength() <= 0) return FALSE;

    strShxFile.TrimLeft();
    strShxFile.TrimRight();
    if(strShxFile.GetLength() <= 0) return FALSE;

    strBigFile.TrimLeft();
    strBigFile.TrimRight();
    if(strBigFile.GetLength() <= 0) return FALSE;
    if(strBigFile == "��ʹ�ô�����") strBigFile = " ";

    if(dHeight < 0.0) dHeight = 0.0;
    if(dWidthFactor < 0.0) dWidthFactor = 1.0;
    if(dAngleByDegree > 85.0 || dAngleByDegree < -85.0) dAngleByDegree = 0.0;
    iGenerationFlag &= 0x06;
    tmpTSName = strTSName;
    tmpTSName.MakeUpper();

    try
    {
        if(FindAcad() == FALSE)
        {
            return FALSE;
        }
        
        //SETUP-1: ��ȡACAD�ı���ʽ�б�
        obj = EDIBAcad::objAcadDoc.GetPropertyByName(
                                      _T("TextStyles")).pdispVal;
        
        //SETUP-2: ���ָ����ʽ�����Ѿ����ڣ��򷵻�

		nCount = (long)obj.GetPropertyByName(_T("Count"));
		for( i = 0; i < nCount; i++)
		{
			tsobj = obj.Invoke(_T("Item"), 1, &_variant_t(i)).pdispVal;
			strtmp = vtos(tsobj.GetPropertyByName(_T("Name")));
			strtmp.TrimLeft();
            strtmp.TrimRight();
            strtmp.MakeUpper();
			if(strtmp == tmpTSName)
			{
                //Ŀ����ʽ�Ѵ���
                return FALSE;
			}
		}

        //SETUP-3: ����µ��ı���ʽ
        obj.Invoke(_T("Add"),
                   1,
                   &_variant_t(strTSName));
        
        //SETUP-4: ��ȡ����ʽ����
        newobj = obj.Invoke(_T("Item"),
                            1,
                            &_variant_t(strTSName)).pdispVal;
        
        //SETUP-5: �趨����ʽ������ֵ
        if(newobj.p)//�ɹ���ȡ�¶���
        {
            newobj.PutPropertyByName(_T("FontFile"),           //�趨�����ļ���
                   &_variant_t(strShxFile));

            newobj.PutPropertyByName(_T("BigFontFile"),        //�趨�������ļ���
                   &_variant_t(strBigFile));

            newobj.PutPropertyByName(_T("Height"),             //�趨����߶�
                   &_variant_t(dHeight));
            newobj.PutPropertyByName(_T("Width"),              //�趨��ȱ�������***
                   &_variant_t(dWidthFactor));
            newobj.PutPropertyByName(_T("ObliqueAngle"),       //�趨������б�Ƕ�
                   &_variant_t(dAngleByDegree));
            newobj.PutPropertyByName(_T("TextGenerationFlag"), //�趨���������ֶ�
                   &_variant_t(iGenerationFlag));
            
            return TRUE;
        }
    }
	catch(CException *e)
	{
		e->Delete();
	}
    return FALSE;
}

BOOL CPreStylePag::ModifyTextStyleProperty(
                       CString strTSName,
                       CString strShxFile,
                       CString strBigFile,
                       double  dHeight,
                       double  dWidthFactor,
                       double  dAngleByDegree,
                       short   iGenerationFlag)
{
    CMObject obj, tsobj;
    long     i, nCount;
    CString  strtmp;
//    HRESULT  retCode;

    //���в������ͳ�ʼ��
    strTSName.TrimLeft();
    strTSName.TrimRight();
    strTSName.MakeUpper();
    if(strTSName.GetLength() <= 0) return FALSE;

    strShxFile.TrimLeft();
    strShxFile.TrimRight();
    if(strShxFile.GetLength() <= 0) return FALSE;

    strBigFile.TrimLeft();
    strBigFile.TrimRight();
    if(strBigFile.GetLength() <= 0) return FALSE;
    if(strBigFile == "��ʹ�ô�����") strBigFile = "";

    if(dHeight < 0.0) return FALSE;
    if(dWidthFactor < 0.0) return FALSE;
    if(dAngleByDegree > 85.0 || dAngleByDegree < -85.0) return FALSE;
    iGenerationFlag &= 0x06;
    if(dAngleByDegree < 0) dAngleByDegree += 360;
    dAngleByDegree *= 3.14159265359;
    dAngleByDegree /= 180.0;

    try
    {
        if(FindAcad() == FALSE)
        {
            return FALSE;
        }
        
        //SETUP-1: ��ȡACAD�ı���ʽ�б�
        obj = EDIBAcad::objAcadDoc.GetPropertyByName(
                                      _T("TextStyles")).pdispVal;
        
        //SETUP-2: ��ȡ��ʽ����

		nCount = (long)obj.GetPropertyByName(_T("Count"));
		for( i = 0; i < nCount; i++)
		{
			tsobj = obj.Invoke(_T("Item"), 1, &_variant_t(i)).pdispVal;
			strtmp = vtos(tsobj.GetPropertyByName(_T("Name")));
			strtmp.TrimLeft();
            strtmp.TrimRight();
			strtmp.MakeUpper();
			if(strtmp == strTSName)
			{
                //����Ŀ����ʽ������ֵ
                tsobj.PutPropertyByName(_T("FontFile"),           //�趨�����ļ���
                                       &_variant_t(strShxFile));
                
                tsobj.PutPropertyByName(_T("BigFontFile"),        //�趨�������ļ���
                                       &_variant_t(strBigFile));
                
                tsobj.PutPropertyByName(_T("Height"),             //�趨����߶�
                                       &_variant_t(dHeight));
                tsobj.PutPropertyByName(_T("Width"),              //�趨��ȱ�������***
                                       &_variant_t(dWidthFactor));
                tsobj.PutPropertyByName(_T("ObliqueAngle"),       //�趨������б�Ƕ�
                                       &_variant_t(dAngleByDegree));
                tsobj.PutPropertyByName(_T("TextGenerationFlag"), //�趨���������ֶ�
                                       &_variant_t(iGenerationFlag));
                return TRUE;
			}
		}
    }
	catch(CException *e)
	{
		e->Delete();
	}
    return FALSE;
}

BOOL CPreStylePag::GetTextStyleProperty(
                       CString  strTSName,
                       CString &strShxFile,
                       CString &strBigFile,
                       double  &dHeight,
                       double  &dWidthFactor,
                       double  &dAngleByDegree,
                       short   &iGenerationFlag)
{
    CMObject obj, tsobj;
    long     i, nCount;
    CString  strtmp;

    //���в������ͳ�ʼ��
    strTSName.TrimLeft();
    strTSName.TrimRight();
    strTSName.MakeUpper();
    if(strTSName.GetLength() <= 0) return FALSE;

    try
    {
        if(FindAcad() == FALSE)
        {
            return FALSE;
        }
        
        //SETUP-1: ��ȡACAD�ı���ʽ�б�
        obj = EDIBAcad::objAcadDoc.GetPropertyByName(
                                      _T("TextStyles")).pdispVal;
        
        //SETUP-2: ��ȡ��ʽ����

		nCount = (long)obj.GetPropertyByName(_T("Count"));
		for( i = 0; i < nCount; i++)
		{
			tsobj = obj.Invoke(_T("Item"), 1, &_variant_t(i)).pdispVal;
			strtmp = vtos(tsobj.GetPropertyByName(_T("Name")));
			strtmp.TrimLeft();
            strtmp.TrimRight();
			strtmp.MakeUpper();
			if(strtmp == strTSName)
			{
                //���Ŀ����ʽ������ֵ
                strShxFile      = vtos(tsobj.GetPropertyByName(
                                 _T("FontFile")));   //��������ļ���
                strBigFile      = vtos(tsobj.GetPropertyByName(
                                 _T("BigFontFile")));//��ô������ļ���
                
                dHeight         = vtod(tsobj.GetPropertyByName(
                                 _T("Height")));     //�������߶�

                dWidthFactor    = vtod(tsobj.GetPropertyByName(
                                 _T("Width")));      //��ÿ�ȱ�������***

                dAngleByDegree  = vtod(tsobj.GetPropertyByName(
                                 _T("ObliqueAngle")));        //���������б�Ƕ�
                dAngleByDegree  *= 180.0;
                dAngleByDegree  /= 3.14159265359;
                if(dAngleByDegree > 270.0) dAngleByDegree -= 360;
                
                iGenerationFlag = vtoi(tsobj.GetPropertyByName(
                                 _T("TextGenerationFlag")));  //������������ֶ�
                iGenerationFlag &= 0x06;

                return TRUE;
			}
		}
    }
	catch(CException *e)
	{
		e->Delete();
	}
    return FALSE;
}

BOOL CPreStylePag::DeleteTextStyle(CString strTSName)
{
    CMObject obj, tsobj;
    long     i, nCount;
    CString  strtmp;

    //���в������ͳ�ʼ��
    strTSName.TrimLeft();
    strTSName.TrimRight();
    strTSName.MakeUpper();
    if(strTSName.GetLength() <= 0) return FALSE;

    try
    {
        if(FindAcad() == FALSE)
        {
            return FALSE;
        }
        
        //SETUP-1: ��ȡACAD�ı���ʽ�б�
        obj = EDIBAcad::objAcadDoc.GetPropertyByName(
                                      _T("TextStyles")).pdispVal;
        
        //SETUP-2: ��ȡ��ʽ����
		nCount = (long)obj.GetPropertyByName(_T("Count"));
		for( i = 0; i < nCount; i++)
		{
			tsobj = obj.Invoke(_T("Item"), 1, &_variant_t(i)).pdispVal;
			strtmp = vtos(tsobj.GetPropertyByName(_T("Name")));
			strtmp.TrimLeft();
            strtmp.TrimRight();
			strtmp.MakeUpper();
			if(strtmp == strTSName)
			{
                //ɾ��Ŀ����ʽ
                tsobj.Invoke0(_T("Delete"));
                return TRUE;
			}
		}
    }
	catch(CException *e)
	{
		e->Delete();
	}
    return FALSE;
}

void CPreStylePag::OnBtnNewstyle() 
{
    //�ֲ���������ͳ�ʼ��
    CMObject   obj, newobj;
    CString    newTextStyleName, tmpStr;
    CComboBox *lpStyle, *lpShxFonts, *lpBigFonts;

    lpStyle    = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
    lpShxFonts = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    lpBigFonts = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);

    try
    {
        if(FindAcad() == FALSE)
        {
            return;
        }
        
        //��ȡ��������ʽ������
        CNewStyleDlg dlg;
        if(dlg.DoModal() != IDOK) return ;
        newTextStyleName = dlg.m_strStyleName;
        if(lpStyle->FindString(0, newTextStyleName) != -1)
        {
            MessageBox("������ʽ [" + newTextStyleName + "] �Ѵ��ڣ������ظ�������");
            return ;
        }
        
        //�����µ�������ʽ
        //SETUP-1: ��ȡACAD�ı���ʽ�б�
        obj = EDIBAcad::objAcadDoc.GetPropertyByName(
                                      _T("TextStyles")).pdispVal;
        
        //SETUP-2: ����µ��ı���ʽ
        obj.Invoke(_T("Add"),
                   1,
                   &_variant_t(newTextStyleName));
        
        //SETUP-3: ��ȡ����ʽ����
        newobj = obj.Invoke(_T("Item"),
                            1,
                            &_variant_t(newTextStyleName)).pdispVal;
        
        //SETUP-4: �趨����ʽ������ֵ
        if(newobj.p)//�ɹ���ȡ�¶���
        {
            lpShxFonts->GetLBText(0, tmpStr);
            newobj.PutPropertyByName(_T("FontFile"),           //�趨�����ļ���
                                    &_variant_t(tmpStr));

            tmpStr = " ";
            newobj.PutPropertyByName(_T("BigFontFile"),        //�趨�������ļ���
                                    &_variant_t(tmpStr));

            newobj.PutPropertyByName(_T("Height"),             //�趨����߶�
                                    &_variant_t(double(20.0)));
            newobj.PutPropertyByName(_T("Width"),              //�趨��ȱ�������***
                                    &_variant_t(double(1.0)));
            newobj.PutPropertyByName(_T("ObliqueAngle"),       //�趨������б�Ƕ�
                                    &_variant_t(double(0.0)));
            newobj.PutPropertyByName(_T("TextGenerationFlag"), //�趨���������ֶ�
                   &_variant_t(short(0)));                     //���κ������־
                   //&_variant_t(short(2)));                   //�����־
                   //&_variant_t(short(4)));                   //�ߵ���־
            
            //SETUP-5: ���´�����������ʽ�趨Ϊϵͳ�ĵ�ǰ������ʽ
            EDIBAcad::objAcadDoc.Invoke(_T("ActiveTextStyle"),
                                        1,
                                        &_variant_t(newobj.p));

            //SETUP-6: ������ʾ
            lpStyle->AddString(newTextStyleName);

            pCurTable->nStyle = lpStyle->GetCount() - 1;            
            pCurTable->blBackward = FALSE;
            pCurTable->blUp2Down = FALSE;
            pCurTable->blVertical = FALSE;
            pCurTable->dHeight = 0.0;
            pCurTable->dAngle = 0.0;
            pCurTable->dScaleFactor = 1.0;
            pCurTable->nBigFonts = 0;
            pCurTable->nShxFonts = 0;
            pCurTable->nAlign = 0;

            UpdateTableData(FALSE);
        }
        else
        {
            MessageBox("����������ʽʧ�ܣ�������ACAD�д��������������ʽ������������ز���");
        }
    }
	catch(CException *e)
	{
		e->Delete();
	}
}

void CPreStylePag::OnBtnDeletestyle() 
{
    //�ֲ���������ͳ�ʼ��
    CMObject   obj;
    CString    strTSName, strShxFile, strBigFile, tmpstr;
    CComboBox *lpStyle, *lpShx, *lpBig;
    int        nCount, nIndex;
    short      iGenerationFlag;

    lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
    lpShx   = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    lpBig   = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);

    UpdateTableData(TRUE);
    nCount = lpStyle->GetCount();
    if(nCount <= 0)return ;

    if(m_nStyle < 0)
    {
        pCurTable->nStyle = 0;
        UpdateTableData(FALSE);
        return ;
    }
    else if(m_nStyle >= nCount)
    {
        pCurTable->nStyle--;
        UpdateTableData(FALSE);
        return ;
    }
    if(nCount == 1)
    {
        MessageBox("ϵͳ���뱣��һ��������ʽ��ɾ��ʧ�ܣ�",
            "�������", MB_ICONSTOP);
        return ;
    }
    lpStyle->GetLBText(m_nStyle, strTSName);
    if(MessageBox("��ȷ��: �����Ҫɾ��������ʽ [" + strTSName + "] ��",
        "ȷ��ɾ��", MB_YESNO | MB_ICONWARNING) == IDNO)
    {
        return ;
    }

    if(DeleteTextStyle(strTSName) == FALSE) return ;
    lpStyle->DeleteString(m_nStyle);

    //�˴��жϱ�ʾ��
    //1�����û�ɾ�������б����һ����ʽ��������ʽ�Զ����Ϲ���
    //2�����û�ɾ���Ĳ������б���������Զ����¹���
    //3������ǰ���ѡ���û�����ɾ���б���Ψһ����
    if(nCount == m_nStyle + 1) m_nStyle--;

    lpStyle->GetLBText(m_nStyle, strTSName);
    GetTextStyleProperty(strTSName, strShxFile, strBigFile,
               pCurTable->dHeight, pCurTable->dScaleFactor,
               pCurTable->dAngle, iGenerationFlag);

    pCurTable->nStyle = m_nStyle;
    pCurTable->nShxFonts = lpShx->FindString(0, strShxFile);
    if(pCurTable->nShxFonts == CB_ERR)
    {
        lpShx->AddString(strShxFile);
        pCurTable->nShxFonts = lpShx->GetCount() - 1;
    }
    pCurTable->nBigFonts = lpBig->FindString(0, strBigFile);
    if(pCurTable->nBigFonts == CB_ERR) pCurTable->nBigFonts = 0;

    if(iGenerationFlag & 0x02) pCurTable->blBackward = TRUE;
    else                       pCurTable->blBackward = FALSE;
    if(iGenerationFlag & 0x04) pCurTable->blUp2Down  = TRUE;
    else                       pCurTable->blUp2Down  = FALSE;

    UpdateTableData(FALSE);
}

BOOL CPreStylePag::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//���ACAD�Ƿ��Ѿ�����
	//��ѯACAD�����еĿ��õ�������ʽ
	//pfg20050330
	CComboBox *lpStyle;
	lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
	if(lpStyle->GetCount()<1)
	{
	    lpStyle->AddString("DIM");
	}
	//pfg20050330
	/*
	FindAcad(TRUE);

	//�������õ������ļ��ʹ������ļ�
	LoadFontFilesCombo();

	//��ѯACAD�����еĿ��õ�������ʽ
	QueryAcadTextStyleCombo();

	//�趨����ʼ����
	LoadTableCombo();

	//��modPHScal��ÿؼ�ֵ
	SetCtrlValue();

	//��ע���������ʷ����
	LoadHistorySetting();

	//����ʹ����Ϣ
	m_strMsg.LoadString(IDS_PRESTYLEPAGEMSG);

	//���¶Ի�����ʾ
	UpdateTableData(FALSE);*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreStylePag::LoadFontFilesCombo()
{
    //�������õ������ļ��ʹ������ļ�
    CString strFontDir, strFile;
    WIN32_FIND_DATA fd;
    HANDLE fh;
    CComboBox *lpBigCombo, *lpShxCombo;

    strFontDir = GetCADFontDir();
    lpBigCombo = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);
    lpShxCombo = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    memset(&fd, 0, sizeof(fd));
    if(strFontDir.IsEmpty() == TRUE) return;

    strFile = strFontDir + (_T("\\*.shx"));
    
    fh = FindFirstFile(strFile, &fd);
    if(fh != INVALID_HANDLE_VALUE)
    {
        do
        {
            strFile = strFontDir + (_T("\\")) + fd.cFileName;
            strFile.MakeLower();
            if(strFile.GetLength() > 4 && strFile.Right(4) == _T(".shx"))
            {
                try
                {
                    CFile file(strFile, CFile::modeRead);
                    CString strVal;
                    char data[30];
                    int  iCount = file.Read(data, 29);
                    data[iCount]=0;
                    strVal = data;
                    strVal.MakeLower();
                    if(strVal.Find((_T("bigfont"))) != -1)
                    {
                        strFile = strFile.Mid(strFile.ReverseFind((_T('\\'))) + 1);
                        //strFile = strFile.Left(strFile.ReverseFind((_T('.'))));
                        lpBigCombo->AddString(strFile);
                    }
                    else if(strVal.Find((_T("unifont"))) != -1)
                    {
                        strFile = strFile.Mid(strFile.ReverseFind((_T('\\'))) + 1);
                        //strFile = strFile.Left(strFile.ReverseFind((_T('.'))));
                        lpShxCombo->AddString(strFile);
                    }
                    file.Close();
                }
                catch(CFileException * e)
                {
                    if(e->m_cause==11)
                    {
                        //���� AutoCAD ��ǰʹ�ø����壬����������Υ��
                        strFile = strFile.Mid(strFile.ReverseFind((_T('\\'))) + 1);
                        if(::CopyFile(e->m_strFileName,
                            basDirectory::TemplateDir +
                            strFile, FALSE))
                        {
                            try
                            {
                                CFile file(basDirectory::TemplateDir + strFile,
                                    CFile::modeRead);
                                CString strVal;
                                char data[30];
                                int iCount = file.Read(data, 29);
                                data[iCount]=0;
                                strVal = data;
                                strVal.MakeLower();
                                if(strVal.Find(_T("bigfont")) != -1)
                                {
                                    //strFile = strFile.Left(strFile.ReverseFind((_T('.'))));
                                    lpBigCombo->AddString(strFile);
                                }
                                else if(strVal.Find((_T("unifont"))) != -1)
                                {
                                    //strFile = strFile.Left(strFile.ReverseFind((_T('.'))));
                                    lpShxCombo->AddString(strFile);
                                }
                                file.Close();
                            }
                            catch(CFileException * e)
                            {
#ifdef _DEBUG
                                e->ReportError();
#endif
                                e->Delete();
                            }
                        }
                    }
                    else
                    {
#ifdef _DEBUG
                        e->ReportError();
#endif
                    }
                    e->Delete();
                }
            }
        }while(FindNextFile(fh, &fd));
        FindClose(fh);
    }
}

void CPreStylePag::QueryAcadTextStyleCombo()
{
    //��ѯACAD�����еĿ��õ�������ʽ
    CMObject obj, sobj;
    CString  strStyleName;
    CComboBox *lpStyle;
    long i, nCount;
	_variant_t v;
	try
	{
        lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
		v = EDIBAcad::objAcadDoc.GetPropertyByName(_T("TextStyles"));
        obj = v.pdispVal;
		nCount =(long)obj.GetPropertyByName(_T("Count"));

		for(i = 0; i < nCount; i++)
		{
			sobj = obj.Invoke(_T("Item"), 1, &_variant_t(i)).pdispVal;
			strStyleName = vtos(sobj.GetPropertyByName(_T("Name")));
			strStyleName.TrimLeft();
            strStyleName.TrimRight();
            if(strStyleName.GetLength() <= 0) continue;

            lpStyle->AddString(strStyleName);
		}
	}
	catch(CException *e)
	{
		e->Delete();
	}
}

void CPreStylePag::LoadTableCombo()
{
    //�������ʼ����
    memset(&ljmx_style, NULL, sizeof(WHK_STYLE));
    memset(&dim_style,  NULL, sizeof(WHK_STYLE));
    memset(&load_style, NULL, sizeof(WHK_STYLE));

    memset(&Comments_style, NULL, sizeof(WHK_STYLE));
    memset(&BH_style,       NULL, sizeof(WHK_STYLE));
    memset(&BG_style,       NULL, sizeof(WHK_STYLE));
    memset(&other_style,    NULL, sizeof(WHK_STYLE));

    ljmx_style.nTable = 0;
    dim_style.nTable  = 1;
    load_style.nTable = 2;

    Comments_style.nTable = 3;
    BH_style.nTable       = 4;
    BG_style.nTable       = 5;
    other_style.nTable    = 6;

    pCurTable = &ljmx_style;
}

CString CPreStylePag::GetCADFontDir()
{
	HKEY hKey;
	CString strSubKey=(_T("Software\\Autodesk\\AutoCAD\\R14.0"));
	if(ERROR_SUCCESS!=RegOpenKey(HKEY_LOCAL_MACHINE,strSubKey,&hKey))
	{
		strSubKey=(_T("Software\\Autodesk\\AutoCAD\\R14.01"));
		if(ERROR_SUCCESS!=RegOpenKey(HKEY_LOCAL_MACHINE,strSubKey,&hKey))
		{
			strSubKey=(_T("Software\\Autodesk\\AutoCAD\\R15.0"));
			if(ERROR_SUCCESS!=RegOpenKey(HKEY_LOCAL_MACHINE,strSubKey,&hKey))
			{
				return (_T(""));
			}
		}
	}
	_TCHAR szValue[256];
	long len=256*sizeof(_TCHAR);
	ULONG type=REG_SZ;
	if(ERROR_SUCCESS!=RegQueryValueEx(hKey,(_T("CurVer")),NULL,&type,(unsigned char*)szValue,(unsigned long*)&len))
	{
		RegCloseKey(hKey);
		return (_T(""));
	}
	HKEY hKey2;
	if(ERROR_SUCCESS!=RegOpenKey(hKey,szValue,&hKey2))
	{
		RegCloseKey(hKey);
		return (_T(""));
	}
	RegCloseKey(hKey);
	len=256*sizeof(_TCHAR);
	if(ERROR_SUCCESS!=RegQueryValueEx(hKey2,(_T("AcadLocation")),NULL,&type,(unsigned char*)szValue,(unsigned long*)&len))
	{
		RegCloseKey(hKey2);
		return (_T(""));
	}
	RegCloseKey(hKey2);
	CString strRet=szValue;
	user::AddDirSep(strRet);
	strRet+=(_T("FONTS"));
	return strRet;
}

BOOL CPreStylePag::FindAcad(BOOL blCrackFind)
{
    if(blCrackFind == TRUE)
    {
        m_blAcadIsOpened = EDIBAcad::StartAcad();
        if(m_blAcadIsOpened == TRUE)
        {
            EDIBAcad::SetAcadNoTop();
            ::SetForegroundWindow(theApp.m_pMainWnd->m_hWnd);
        }
    }
    else
    {
        if(m_blAcadIsOpened == FALSE)
        {
            if(EDIBAcad::FindAcad() == NULL)
            {
                m_blAcadIsOpened = EDIBAcad::StartAcad(); 
            }
            else m_blAcadIsOpened = TRUE;
            EDIBAcad::SetAcadNoTop();  
            ::SetForegroundWindow(theApp.m_pMainWnd->m_hWnd);
        }
    }
    return m_blAcadIsOpened;
}

void CPreStylePag::OnBtnDefault() 
{
    CComboBox *lpStyle, *lpShx, *lpBig;
    CString    strTSName, strShxFile, strBigFile;
    short      iGenerationFlag;

    lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
    lpBig   = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);
    lpShx   = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    //��ǰ������ȱʡ�Ĳ�������
    switch(pCurTable->nTable)
    {
    case 0://ljmx
        pCurTable->nStyle = lpStyle->FindString(0, _T("BOM"));
        strTSName = "BOM";
        break;
    case 1://dim
        pCurTable->nStyle = lpStyle->FindString(0, _T("DIM"));
        strTSName = "DIM";
        break;
    case 2://load
        pCurTable->nStyle = lpStyle->FindString(0, _T("LOAD"));
        strTSName = "LOAD";
        break;
    case 3://Comments
        pCurTable->nStyle = lpStyle->FindString(0, _T("BOM"));
        strTSName = "BOM";
        break;
    case 4://BH
        pCurTable->nStyle = lpStyle->FindString(0, _T("BOM"));
        strTSName = "BOM";
        break;
    case 5://BG
        pCurTable->nStyle = lpStyle->FindString(0, _T("BOM"));
        strTSName = "BOM";
        break;
    case 6://Other
        pCurTable->nStyle = lpStyle->FindString(0, _T("BOM"));
        strTSName = "BOM";
        break;
    default: return ;
    }
    //��ʽ��

    if(pCurTable->nStyle == -1)
    {
        CreateTextStyle(strTSName, _T("txt.shx"), _T("hzfs.shx"),
            0.0, 0.7, 0.0, 0);
        lpStyle->AddString(strTSName);
        pCurTable->nStyle = lpStyle->GetCount() - 1;
        pCurTable->blBackward = FALSE;
        pCurTable->blUp2Down = FALSE;
        pCurTable->dAngle = 0.0;
        pCurTable->dHeight = 0.0;
        pCurTable->dScaleFactor = 0.7;
        pCurTable->nAlign = 0;
        pCurTable->nBigFonts = lpBig->FindString(0, _T("hzfs.shx"));
        if(pCurTable->nBigFonts == -1)
        {
            lpBig->AddString(_T("hzfs.shx"));
            pCurTable->nBigFonts = lpBig->GetCount() - 1;
        }
        pCurTable->nShxFonts = lpShx->FindString(0, _T("txt.shx"));
        if(pCurTable->nShxFonts == -1)
        {
            lpShx->AddString(_T("txt.shx"));
            pCurTable->nShxFonts = lpShx->GetCount() - 1;
        }

        UpdateTableData(FALSE);
        return ;
    }

    //��ȱʡ��ʽ������ֵ��������Ϊϵͳ�����ֵ
    strShxFile = _T("txt.shx");
    strBigFile = _T("hzfs.shx");
    pCurTable->dHeight = 0.0;
    pCurTable->dScaleFactor = 0.7;
    pCurTable->dAngle = 0.0;
    iGenerationFlag = 0;
    ModifyTextStyleProperty(strTSName, strShxFile, strBigFile,
               pCurTable->dHeight, pCurTable->dScaleFactor,
               pCurTable->dAngle, iGenerationFlag);
    
    pCurTable->nShxFonts = lpShx->FindString(0, strShxFile);
    if(pCurTable->nShxFonts == CB_ERR)
    {
        lpShx->AddString(strShxFile);
        pCurTable->nShxFonts = lpShx->GetCount() - 1;
    }
    pCurTable->nBigFonts = lpBig->FindString(0, strBigFile);
    if(pCurTable->nBigFonts == CB_ERR)
    {
        lpShx->AddString(strBigFile);
        pCurTable->nShxFonts = lpShx->GetCount() - 1;
    }

    pCurTable->blBackward = FALSE;
    pCurTable->blUp2Down  = FALSE;
    pCurTable->nAlign = 0;

    UpdateTableData(FALSE);
}

void CPreStylePag::OnBtnSave() 
{
    UpdateTableData(TRUE);
    SaveTable2Registry();
}

void CPreStylePag::SaveTable2Registry() 
{
    CComboBox *lpTable, *lpStyle, *lpShx, *lpBig;
    CString   strTable, strValue;

    lpTable = (CComboBox *)GetDlgItem(IDC_COMBO_TABLE);
    lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
    lpShx   = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    lpBig   = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);

    //���浱ǰ���Ĳ�������
    lpTable->GetLBText(pCurTable->nTable, strTable);

    //���浱ǰ��ʽ��
	if(lpStyle->GetCount()>0)//pfg20050330
	{
    lpStyle->GetLBText(pCurTable->nStyle, strValue);
	}
	else//pfg20050330
	{
		strValue="";//pfg20050330
	}
    SetRegValue(_T("Settings"), strTable + _T("-STYLENAME"), strValue);

    //���浱ǰ��ʽ���ԣ������µ����ʱ����ϵͳ��û����Ӧ
    //    ����ʽʱ�����Զ���������ϵͳ�Ѿ�����Ӧ����ʽ����
    //    ��������Ӧ��ʽ������ֵ����ʱ�����������á�

    //���浱ǰ�����ļ���
    lpShx->GetLBText(pCurTable->nShxFonts, strValue);
    SetRegValue(_T("Settings"), strTable + _T("-SHXFILENAME"), strValue);
    //���浱ǰ�������ļ���
    lpBig->GetLBText(pCurTable->nBigFonts, strValue);
    SetRegValue(_T("Settings"), strTable + _T("-BIGFILENAME"), strValue);
    //�߶�
	//pfg20050330
	strTable.TrimLeft();
	strTable.TrimRight();
	if(strTable.Compare("��ע�ߴ�"))
	{
		SetRegValue(_T("Settings"), strTable + _T("-HEIGHT"), double(pCurTable->dHeight));//pfg20050330ԭ����
	}
	//pfg20050330
    //���
    SetRegValue(_T("Settings"), strTable + _T("-WIDTH"), double(pCurTable->dScaleFactor));
    //��б�Ƕ�
    SetRegValue(_T("Settings"), strTable + _T("-OBLIQUEANGLE"), double(pCurTable->dAngle));
    //�ߵ�
    SetRegValue(_T("Settings"), strTable + _T("-BLUP2DOWN"), pCurTable->blUp2Down);
    //����
    SetRegValue(_T("Settings"), strTable + _T("-BLBACKWARD"), pCurTable->blBackward);
    //��ֱ
    SetRegValue(_T("Settings"), strTable + _T("-BLVERTICAL"), pCurTable->blVertical);
    //���ֶ��뷽ʽ
    SetRegValue(_T("Settings"), strTable + _T("-ALIGN"), int(pCurTable->nAlign));
}

void CPreStylePag::LoadHistorySetting()
{
    //��������ʷ���ݣ�һ��ֻ�ڳ�ʼ��ʱ�������
    //�������޷��ָ�ʱ���ñ�����
    LoadTableHistory(_T("��������"));
    LoadTableHistory(_T("���"));
    LoadTableHistory(_T("���"));
    LoadTableHistory(_T("ע��"));

    LoadTableHistory(_T("����λ�Ʊ�"));
    LoadTableHistory(_T("��ע�ߴ�"));
    LoadTableHistory(_T("�����ϸ��"));
}

BOOL CPreStylePag::LoadTableHistory(CString strTable)
{
    //��������ʷ����
    
    CComboBox *lpTable, *lpStyle, *lpShx, *lpBig;
    CString   strValue, strShx, strBig;
    double    dHeight, dScaleFactor, dAngle;
    BOOL      blUp2Down, blBackward;
    short     iGenFlag;
    int       retCode;

    lpTable = (CComboBox *)GetDlgItem(IDC_COMBO_TABLE);
    lpStyle = (CComboBox *)GetDlgItem(IDC_COMBO_STYLE);
    lpShx   = (CComboBox *)GetDlgItem(IDC_COMBO_SHXFONTS);
    lpBig   = (CComboBox *)GetDlgItem(IDC_COMBO_BIGFONTS);

    if(strTable == _T("�����ϸ��"))
    {
        pCurTable = &ljmx_style;
    }
    else if(strTable == _T("��ע�ߴ�"))
    {
        pCurTable = &dim_style;
    }
    else if(strTable == _T("����λ�Ʊ�"))
    {
        pCurTable = &load_style;
    }
    else if(strTable == _T("ע��"))
    {
        pCurTable = &Comments_style;
    }
    else if(strTable == _T("���"))
    {
        pCurTable = &BH_style;
    }
    else if(strTable == _T("���"))
    {
        pCurTable = &BG_style;
    }
    else if(strTable == _T("��������"))
    {
        pCurTable = &other_style;
    }
    else
    {
        return FALSE;
    }

    //��ʽ��
    strValue = GetRegKey(_T("Settings"), strTable + _T("-STYLENAME"), strValue);
    retCode = lpStyle->FindString(0, strValue);
    
    //SHX ����
    strShx = GetRegKey(_T("Settings"), strTable + _T("-SHXFILENAME"), strShx);
    //BIG ����
    strBig = GetRegKey(_T("Settings"), strTable + _T("-BIGFILENAME"), strBig);
    //�߶�
    dHeight = GetRegKey(_T("Settings"), strTable + _T("-HEIGHT"), float(0.0));
    //��ȱ���
    dScaleFactor = GetRegKey(_T("Settings"), strTable + _T("-WIDTH"), float(0.0));
    //��б�Ƕ�
    dAngle = GetRegKey(_T("Settings"), strTable + _T("-OBLIQUEANGLE"), float(0.0));
    //�ߵ�
    blUp2Down = GetRegKey(_T("Settings"), strTable + _T("-BLUP2DOWN"), FALSE);
    //����
    blBackward = GetRegKey(_T("Settings"), strTable + _T("-BLBACKWARD"), FALSE);
    
    iGenFlag = 0;
    if(blBackward == TRUE) iGenFlag += 2;
    if(blUp2Down == TRUE)  iGenFlag += 4;
    
    if(retCode == -1 && strValue != "")//��ʽ�б����Ҳ�����ʽ��
    {
        //�Զ�������ʽ
        CreateTextStyle(strValue, strShx, strBig, 
            dHeight, dScaleFactor, dAngle, iGenFlag);

        lpStyle->AddString(strValue);

        pCurTable->nStyle = lpStyle->GetCount() - 1;
        pCurTable->nShxFonts = lpShx->FindString(0, strShx);
        if(pCurTable->nShxFonts == -1)
        {
            lpShx->AddString(strShx);
            pCurTable->nShxFonts = lpShx->GetCount() - 1;
        }
        pCurTable->nBigFonts = lpBig->FindString(0, strBig);
        if(pCurTable->nBigFonts == -1)
        {
            pCurTable->nBigFonts = 0;
        }
        pCurTable->dHeight = dHeight;
        pCurTable->dScaleFactor = dScaleFactor;
        pCurTable->dAngle = dAngle;
        pCurTable->blUp2Down = blUp2Down;
        pCurTable->blBackward = blBackward;
    }
    else if(retCode != -1)//�ҵ���ʽ��
    {
        pCurTable->nStyle = retCode;
        //���ø������ϸ�����Ӧ����
        ModifyTextStyleProperty(strValue, strShx, strBig, 
            dHeight, dScaleFactor, dAngle, iGenFlag);

        pCurTable->nShxFonts = lpShx->FindString(0, strShx);
        if(pCurTable->nShxFonts == -1)
        {
            lpShx->AddString(strShx);
            pCurTable->nShxFonts = lpShx->GetCount() - 1;
        }
        pCurTable->nBigFonts = lpBig->FindString(0, strBig);
        if(pCurTable->nBigFonts == -1)
        {
            pCurTable->nBigFonts = 0;
        }
        pCurTable->dHeight = dHeight;
        pCurTable->dScaleFactor = dScaleFactor;
        pCurTable->dAngle = dAngle;
        pCurTable->blUp2Down = blUp2Down;
        pCurTable->blBackward = blBackward;
    }

    //�����������
    pCurTable->nAlign = GetRegKey(_T("Settings"), strTable + _T("-ALIGN"), 0);

    //������ʾ
    UpdateTableData(FALSE);
    return TRUE;
}

void CPreStylePag::DoDefaultSetting(void)
{
    pCurTable = &other_style;
    OnBtnDefault();
    pCurTable = &BG_style;
    OnBtnDefault();
    pCurTable = &BH_style;
    OnBtnDefault();
    pCurTable = &Comments_style;
    OnBtnDefault();
    pCurTable = &load_style;
    OnBtnDefault();
    pCurTable = &dim_style;
    OnBtnDefault();
    pCurTable = &ljmx_style;
    OnBtnDefault();
}

double SetPrecision(double value, int prec)
{
    double nCmp = pow(10.0, prec);
    double dRet = 0.0;
    double dtmp1 = 0.0;
    double dtmp2 = 0.0;
    double dtmp3 = 0.0;

    if(value < nCmp * 0.5) return 0.0;
    if(value >= nCmp * 0.5 && value < nCmp * 1.5) return nCmp;

    if(prec == 0.0)
    {
        modf(value, &dRet);
        return dRet;
    }
    else if(prec < 0.0) // ��ȷ��С��λ
    {
        dtmp1 = modf(value, &dRet);
        dtmp1 *= pow(10.0, 0 - prec);
        dtmp2 = modf(dtmp1, &dtmp3);
        if(dtmp2 >= 0.5)
        {
            dtmp3 += 1.0;
        }
        dtmp3 /= pow(10.0, 0 - prec);
        return dRet + dtmp3;
    }
    else //��ȷ������λ
    {
        modf(value, &dRet);
        dRet /= pow(10.0, prec);
        dtmp2 = modf(dRet, &dtmp1);
        if(dtmp2 >= 0.5)
        {
            dtmp1 += 1.0;
        }
        dtmp1 *= pow(10.0, prec);
        return dtmp1;
    }
}

void CPreStylePag::OnLButtonDown(UINT nFlags,CPoint point)
{//chengbailian 2003.12.12 �����ںܴ�ʱ,�ڷֱ��ʸı�Сʱ,���ڲ���ȫ����ʾ����,��Ҫ�ƶ�����
	HWND h=::GetActiveWindow();//�õ�����������
	::SendMessage(h,WM_NCLBUTTONDOWN, HTCAPTION, NULL);//�����ƶ���Ϣ
}







BOOL CPreStylePag::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(ActiveTimes != 0)
	{
		FindAcad(TRUE);

		//�������õ������ļ��ʹ������ļ�
		LoadFontFilesCombo();

		//��ѯACAD�����еĿ��õ�������ʽ
		QueryAcadTextStyleCombo();

		//�趨����ʼ����
		LoadTableCombo();

		//��modPHScal��ÿؼ�ֵ
		SetCtrlValue();

		//��ע���������ʷ����
		LoadHistorySetting();

		//����ʹ����Ϣ
		m_strMsg.LoadString(IDS_PRESTYLEPAGEMSG);

		//���¶Ի�����ʾ
		UpdateTableData(FALSE);
	}
	ActiveTimes = ActiveTimes + 1;
	return CPropertyPage::OnSetActive();
}

SHORT CPreStylePag::GetActiveTimes()
{
	return ActiveTimes;
}
