#if !defined(AFX_PROPERTYWND_H__40E4A8DD_5CB7_4CE9_995E_9EAA16BB733D__INCLUDED_)
#define AFX_PROPERTYWND_H__40E4A8DD_5CB7_4CE9_995E_9EAA16BB733D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyWnd.h : header file
//
// �����ҵ�Ԫ������
// ���ص�Ԫ��ѡ�е�֪ͨ

/////////////////////////////////////////////////////////////////////////////
//
// Write By ��ΰ��
// 2004,10,12 
//
// ���Դ��ڣ��������Ե�����
//
// ������������Ե����ƣ��ұ߿���������
//
///////////////////////////////////////////////////////////////////////////////////

#define PWN_FIRST	2000U

#define PWN_CONTROL	PWN_FIRST+1		// ���������ӿؼ���֪ͨ��֪ͨ,��֪ͨת��
									// �ҵ�Ԫ���ӿص�֪ͨ,����PWNControlStruct�ṹ

#define PWN_SELECTCHANGE	PWN_FIRST+2	// ���ı���ѡ�ĵ�Ԫ��ʱ����,
										// ����PWN_SELECTCHANGE�ṹ��

class CPropertyWnd : public CWnd
{
// Construction
public:
	CPropertyWnd();
	virtual ~CPropertyWnd();

public:
	enum{
		TitleElement	=0x01,	// ���б��ⵥԪ
		ChildElement	=0x02,	// ���б����µ��ӵ�Ԫ
		EditElement		=0x04,	// ������Edit�ؼ�
		ComBoxElement	=0x08,	// ������ComBox�ؼ�
		DefineBackColor	=0x10,	// ָ�������������е�Ԫ�ı���ɫ
		DefineTextColor	=0x20,	// ָ�������������е�Ԫ��������ɫ
		DefineControlWnd=0x40	// �û�ָ�����ҵ�Ԫ���õĿؼ�(����û�ָ�����ؼ���IDӦΪ��Ԫ���кŻ�������еĵ�Ԫ)
	};

	//
	// �����û��Ե�Ԫ�Ķ���ṹ
	//
	struct ElementDef
	{
		CString szElementName;		// ���е�Ԫ�������ַ���
		UINT Style;					// ������Ԫ��ʽ��
		int Width;					// ��Ԫ�Ŀ�������Ϊ��λ����Ϊ����ʱ�Զ�����
		int Height;					// ��Ԫ�ĸߣ�������Ϊ��λ����Ϊ����ʱ�Զ�����
		COLORREF BackGroundColor;	// ��Ԫ�ı�Ӱɫ
		COLORREF TextColor;			// ��Ԫ��������ɫ
		LPVOID pVoid;				// ���ڸ��ӵ��ڵ�Ԫ�ڵ�����
		CWnd *pControlWnd;			// ��Ӧ���ҵ�Ԫ�е��ӿؼ�
		CString RightElementText;	// �ҵ�Ԫ��������ʾ������
		CString strFieldName;		// �����Ӧ��ԭʼ���ݱ���ֶ���


		//
		// ����Ĭ�ϵ�ֵ
		//
		ElementDef()
		{
			Style=ChildElement|EditElement;
			Width=-1;
			Height=-1;
			TextColor=RGB(0,0,0);
			BackGroundColor=RGB(255,255,255);
			pVoid=NULL;
			pControlWnd=NULL;
		}
	};

	//
	// ��PWN_CONTROL֪ͨ�Ľṹ
	//
	struct PWNControlStruct
	{
		NMHDR nmh;

		LPCTSTR szElementName;		// ���е�Ԫ�������ַ���
		UINT Style;					// ������Ԫ��ʽ��
		int Width;					// ��Ԫ�Ŀ�������Ϊ��λ��
		int Height;					// ��Ԫ�ĸߣ�������Ϊ��λ��
		COLORREF BackGroundColor;	// ��Ԫ�ı�Ӱɫ
		COLORREF TextColor;			// ��Ԫ��������ɫ
		LPVOID pVoid;				// ���ڸ��ӵ��ڵ�Ԫ�ڵ�����
		LPCTSTR szRightElementText;	// �ҵ�Ԫ��������ʾ������
		int Row;					// �˵�Ԫ���к�

		int		idCtrl;				// �ҵ�Ԫ�ؼ���ID
		WORD	wNotifyCode;		// �ҵ�Ԫ�ؼ����¼���
		HWND	hWndControl;		// �ҵ�Ԫ�ؼ��Ĵ��ھ��
	};

	//
	// ��PWN_SELECTCHANGE���͵Ľṹ
	//
	struct PWNSelectChangeStruct
	{
		NMHDR nmh;

		LPCTSTR szElementName;		// ���е�Ԫ�������ַ���
		UINT Style;					// ������Ԫ��ʽ��
		int Width;					// ��Ԫ�Ŀ�������Ϊ��λ��
		int Height;					// ��Ԫ�ĸߣ�������Ϊ��λ��
		COLORREF BackGroundColor;	// ��Ԫ�ı�Ӱɫ
		COLORREF TextColor;			// ��Ԫ��������ɫ
		LPVOID pVoid;				// ���ڸ��ӵ��ڵ�Ԫ�ڵ�����
		LPCTSTR szRightElementText;	// �ҵ�Ԫ��������ʾ������
		int Row;					// �˵�Ԫ���к�
	};

protected:
	//
	// �˽ṹ���û��Ե�Ԫ�Ķ���ṹ���ƣ����ڳ����ڲ�
	//
	struct ElementDef_Wnd
	{
		CString ElementName;
		UINT Style;
		int Width;
		int Height;
		COLORREF BackGroundColor;
		COLORREF TextColor;
		LPVOID pVoid;
		CWnd *pControlWnd;			// ��Ӧ���ҵ�Ԫ�е��ӿؼ�
		CString RightElementText;	// �ҵ�Ԫ��������ʾ������
		CString strFieldName;		// �����Ӧ��ԭʼ���ݱ���ֶ���

		int Row;					// �˵�Ԫ���кţ�0��ʼ
		RECT LeftColumeRect;		// ��Ԫ��λ�ã�������Ϊ��λ��
		RECT RightColumeRect;		// �ҵ�Ԫ��λ�ã�������Ϊ��λ��

		RECT BitmapColumeRect;		// Ϊ�����Ԥ��������ͼ���λ�ã�������Ϊ��λ)
		BOOL IsHideThisElement;		// �Ƿ���ʾ�˵�Ԫ�ı�־


		ElementDef_Wnd()
		{
			Row=-1;
			pControlWnd=NULL;
			IsHideThisElement=FALSE;	//Ĭ����ʾ�˵�Ԫ
		}
	};
// Attributes
public:
	// �����ܵĵ�Ԫ��
	int GetElementCount();

	// ������Ԫ���ӿؼ�
	BOOL CreateElementControl(int ElementNum);

	// ����ָ��λ�õĵ�Ԫ
	int GetElementAt(ElementDef *pElement,int ElementNum);

	// ����λͼ�еĿ��
	int GetBitmapColumeWidth();
	// ����Ϊλͼ�еĿ��
	void SetBitmapColumeWidth(int Width);

	// �����������ڵı�Ӱɫ
	COLORREF GetWndBackGroundColor();
	// �����������ڵı�Ӱɫ
	void SetWndBackGroundColor(COLORREF Color);

// Operations
public:
	void RefreshData();
	int FindElement(int nStartAfter,LPCTSTR szString,BOOL IsLeftElement=TRUE);
	// ���ҵ�Ԫ�������
	void SetRightElementContent(int Row,LPCTSTR szContent);

	// ���¶���GetClientRect
	void GetClientRect(LPRECT lpRect);

	// ���뵥Ԫ
	int InsertElement(ElementDef *pElement,int InsertAfter=0x7FFFFFFF);

protected:
	// ���¼��������Ԫ��λ������
	void UpdateElementsListRect();

	// ���¸��¹���������Ϣ
	void UpdateScrollBarInfo();

	// ��ÿɼ�������
	int GetVisibleRow();

	// ����ָ���е���һ���ɼ����к�
	int GetPrevVisibleRow(int row);

	// ����ָ���е���һ���ɼ����к�
	int GetNextVisibleRow(int row);

	// ����ָ��λ�õĵ�Ԫ
	ElementDef_Wnd* GetElementAt(int ElementNum);

private:
	// ���쵥Ԫ
	void PaintElement(CDC *pDC);

	// ���ҵ�Ԫ�ĵ�����в���
	void RightElementsHitTest(CPoint point);

	// ����Ԫ�ĵ�����в���
	void LeftElementsHitTest(CPoint point);

	// ����λͼ�еĿ��
	void BitmapElementsHitTest(CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

protected:
	int m_ElementCount;			// �ܹ�������Ԫ�����Ŀ
	int m_CurrentScrollPos;		// ��ǰ��������λ��
	int m_BitmapColumeWidth;	// ����λͼ�еĿ��

private:
	CPtrList	m_ElementList;
	COLORREF	m_BitmapColumeColor;	// λͼ�б���ɫ
	CBrush		m_BitmapColumeBrush;	// λͼ�б���ɫ
	COLORREF	m_FrameColor;			// ���Ʊ߿����ɫ
	CPen		m_FramePen;				// ���Ʊ߿�Ļ���
	COLORREF	m_WndBackGroundColor;	// �������ڵı���ɫ
	CBrush		m_WndBackGroundBrush;	// �������ڵı�Ӱ��ˢ
	BOOL		m_IsNeedReUpdateElementsListRect;	//�Ƿ���Ҫ���¼��������Ԫ��λ��
	int			m_SelectRow;			// �Ѿ�ѡ��ĵ�Ԫ��
	COLORREF	m_SelectElementBackGroundColorBank;	// ���ڱ����Ѿ���ѡ�еĵ�Ԫԭ���ı���ɫ
	HCURSOR		m_hDragCursor;			// �������϶������ͼ����
	BOOL		m_IsInChangeElementRect;	//�Ƿ��ڿɸı䵥Ԫ��С������
	CPoint		m_MouseMovePt;			// ���ڼ�¼��������ǰ��λ��
	CBitmap		m_BitmapSurface;		// ���ڱ�����˸��λͼ

	CFont		m_Font;	//���ֵ�����

	// Generated message map functions
protected:
	void SendPWNSelectChangeNotify();
	void ChangeLeftElementWidth(int Width);
	//{{AFX_MSG(CPropertyWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYWND_H__40E4A8DD_5CB7_4CE9_995E_9EAA16BB733D__INCLUDED_)
