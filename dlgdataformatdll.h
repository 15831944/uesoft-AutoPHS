// DlgDataFormatDll.h: interface for the CDlgDataFormatDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGDATAFORMATDLL_H__3A15C278_B79C_4549_BC5B_52379BA42AB7__INCLUDED_)
#define AFX_DLGDATAFORMATDLL_H__3A15C278_B79C_4549_BC5B_52379BA42AB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// __declspec(dllexport) 
class  AFX_EXT_CLASS CDlgDataFormatDll  
{
public:
	//���ݿ��ȫ·������(sotr.mdb);
	//CString  m_strDBPath;
	//������ʽ�Ի���(����֮ǰ�����������ݿ��ȫ·����)
	void ShowFormatDlg( LPTSTR m_cDBPath );
	CDlgDataFormatDll();
	virtual ~CDlgDataFormatDll();
};

#endif // !defined(AFX_DLGDATAFORMATDLL_H__3A15C278_B79C_4549_BC5B_52379BA42AB7__INCLUDED_)
