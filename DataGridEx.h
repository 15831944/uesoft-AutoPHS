// DataGridEx.h: interface for the CDataGridEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAGRIDEX_H__E061F047_855A_43B7_97AA_14B8D2181707__INCLUDED_)
#define AFX_DATAGRIDEX_H__E061F047_855A_43B7_97AA_14B8D2181707__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// 
// ��DataGrid�ؼ��������ţ�����ˮƽ�봹ֱ������������ʾ����������ʾ����Ϣ
// ʵ�������м������Ĺ�����
//
// Ҫ�����м������Ĺ���������Ҫ����InitMouseWheel
//
///////////////////////////////////////////////////////////////////////////////

#include "datagrid.h"

#define WM_MOUSEWHEEL2 WM_USER+32

#define DATAGRIDEX_FIRSTID	30000

#define IDC_GRID_ASC	DATAGRIDEX_FIRSTID+1	// ����˵�ID
#define IDC_GRID_DESC	DATAGRIDEX_FIRSTID+2	// ����˵�ID

class CDataGridEx : public CDataGrid
{
// Construction
public:
	CDataGridEx();
	virtual ~CDataGridEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataGridEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnSortDesc();
	void OnSortAsc();

	// ���ӵĻ������
	static LRESULT CALLBACK GetMsgProc(int code,WPARAM wParam,LPARAM lParam);

	// ��ʼ���м������Ĺ�����Ӧ
	void InitMouseWheel();
	// �ͷŹ�����Դ��Ӧ�ڴ�������ǰ����
	void UnInitMouseWheel();


	// Generated message map functions
protected:
	BOOL OnNotify(WPARAM wParam,LPARAM lParam,LRESULT *pResult);
	//{{AFX_MSG(CDataGridEx)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseWheel2(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:

	struct tagHookCountInfo
	{
		HHOOK hHook;
		DWORD Count;
	};

	//���ӵľ��Map,��ͬ���̵߳Ĺ��Ӳ�ͬ
	static CMap<DWORD,DWORD,tagHookCountInfo*,tagHookCountInfo*> m_MouseThreadHookMap;

private:
	//����ToolTip�ؼ��ں�����ʾ��λ��
	void PlaceToolTipWndToRightPosition();

	BOOL m_PreVScrollFlag;
	UINT m_PreVScrollPos;
	short m_SelectCol;

	int m_RecordCount;			//��¼��������
	TCHAR m_ToolTipInfo[256];	//���TOOLTIP�ؼ�����ʾ��Ϣ
	void InitToolTipControl();	//��ʼ��TOOLTIP�ؼ�
	CToolTipCtrl m_ToolTip;		//TOOLTIP�ؼ�
};

#endif // !defined(AFX_DATAGRIDEX_H__E061F047_855A_43B7_97AA_14B8D2181707__INCLUDED_)
