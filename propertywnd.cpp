// PropertyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyWnd.h"

#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_BACKGROUND_COLOR RGB(192,192,192)	// Ĭ�ϵı���ɫ

#define DEFAULT_SELECT_COLOR_FOCUS RGB(170,170,255)			//Ĭ�ϵı�ѡ�в��������뽹��ı���ɫ
#define DEFAULT_SELECT_COLOR_NOFOCUS RGB(192,192,192)	//Ĭ�ϵı�ѡ�е�ʧȥ���뽹��ı���ɫ

/////////////////////////////////////////////////////////////////////////////
// CPropertyWnd

CPropertyWnd::CPropertyWnd()
{
	m_ElementCount=0;

	m_WndBackGroundColor=RGB(255,255,255);	// ��ӰĬ��Ϊ��ɫ

	m_WndBackGroundBrush.CreateSolidBrush(m_WndBackGroundColor);

	m_IsNeedReUpdateElementsListRect=FALSE;	//Ĭ�ϲ���Ҫ���¼��������Ԫ��λ��

	m_CurrentScrollPos=0;	// ���õ�ǰ��������λ��

	m_BitmapColumeWidth=16;	// ����λͼ��Ĭ�ϵĿ��

	m_BitmapColumeColor=DEFAULT_BACKGROUND_COLOR;	// ����λͼ�б���ɫ

	m_BitmapColumeBrush.CreateSolidBrush(m_BitmapColumeColor);	// ����λͼ�б�����ˢ

	m_FrameColor=DEFAULT_BACKGROUND_COLOR;			// ���ñ߿����ɫ

	m_FramePen.CreatePen(PS_SOLID,0,m_FrameColor);	// �������Ʊ߿��õĻ���

	m_SelectRow=-1;			// ����ѡ��ĵ�Ԫ�к�

	m_hDragCursor=LoadCursor(NULL, IDC_SIZEWE); //����ͼ��

	m_IsInChangeElementRect=FALSE;	// ��ʼ�����ڿɸı䵥Ԫ��С������

}

CPropertyWnd::~CPropertyWnd()
{
	POSITION pos;
	ElementDef_Wnd *pElementWnd;

	pos=this->m_ElementList.GetHeadPosition();

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		// �ͷŷ��Զ�����ӿؼ�
		if(!(pElementWnd->Style & DefineControlWnd))
			delete pElementWnd->pControlWnd;

		delete pElementWnd;
	}

	this->m_ElementList.RemoveAll();

	::DeleteObject(m_hDragCursor);	// �ͷ�ͼ����Դ
}


BEGIN_MESSAGE_MAP(CPropertyWnd, CWnd)
	//{{AFX_MSG_MAP(CPropertyWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropertyWnd message handlers

int CPropertyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CWnd *pParent;
	CFont *pFont;
	LOGFONT LogFont;

	//
	// �������븸����ͬ��������
	//
	pParent=GetParent();

	if(pParent && IsWindow(pParent->GetSafeHwnd()))
	{
		pFont=pParent->GetFont();
		pFont->GetLogFont(&LogFont);

		this->m_Font.CreateFontIndirect(&LogFont);
	}
	
	// ���������Ԫ��λ��
	UpdateElementsListRect();
	UpdateScrollBarInfo();
	UpdateElementsListRect();

	return 0;
}

///////////////////////////////
//
// ��ӦWM_PAINT��Ϣ
//
void CPropertyWnd::OnPaint() 
{
	//
	// δ�˼�����˸,�����ڴ��л���,���һ��������Ļ�ϻ���
	//
	CPaintDC dc(this); // device context for painting
	CDC MemDC;
	CBitmap *pOldBitmap;
	CRect ClientRect;
	CFont *OldFont;

	MemDC.CreateCompatibleDC(&dc);

	// ѡ���ͼ������
	if(m_Font.GetSafeHandle()!=NULL)
	{
		OldFont=MemDC.SelectObject(&m_Font);
	}

	pOldBitmap=MemDC.SelectObject(&this->m_BitmapSurface);

	//�Ƿ���Ҫ���¼��������Ԫ��λ��
	if(m_IsNeedReUpdateElementsListRect)
	{
		UpdateElementsListRect();
		UpdateScrollBarInfo();
		UpdateElementsListRect();

	}

	GetClientRect(&ClientRect);

	//���Ƹ�����Ԫ
	PaintElement(&MemDC);

	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&MemDC,0,0,SRCCOPY);

	MemDC.SelectObject(pOldBitmap);

	//�滻��ԭ��������
	if(m_Font.GetSafeHandle()!=NULL)
	{
		MemDC.SelectObject(OldFont);
	}
}

BOOL CPropertyWnd::OnEraseBkgnd(CDC* pDC) 
{
	CDC Memdc;
	CBitmap *pOldBitmap;

	//
	// �ñ���ɫ������λͼ
	//
	Memdc.CreateCompatibleDC(pDC);

	pOldBitmap=Memdc.SelectObject(&this->m_BitmapSurface);

	CRect rect;

	GetClientRect(&rect);

	Memdc.FillRect(&rect,&m_WndBackGroundBrush);

	Memdc.SelectObject(pOldBitmap);

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
// ��ӦWM_VSCROLL
//
void CPropertyWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int iMax,iMin;
	int iPosTemp;

	this->GetScrollRange(SB_VERT,&iMin,&iMax);

	iPosTemp=m_CurrentScrollPos;

	if(nSBCode==SB_LINEUP)
	{
		m_CurrentScrollPos=GetPrevVisibleRow(m_CurrentScrollPos);

		m_CurrentScrollPos=__max(m_CurrentScrollPos,iMin);

	}
	else if(nSBCode==SB_LINEDOWN)
	{
		m_CurrentScrollPos=GetNextVisibleRow(m_CurrentScrollPos);

		m_CurrentScrollPos=__min(m_CurrentScrollPos,iMax);
	}
	else if(nSBCode==SB_PAGEUP)
	{
		m_CurrentScrollPos-=this->GetVisibleRow();

		m_CurrentScrollPos=__max(m_CurrentScrollPos,iMin);
	}
	else if(nSBCode==SB_PAGEDOWN)
	{
		m_CurrentScrollPos+=this->GetVisibleRow();

		m_CurrentScrollPos=__min(m_CurrentScrollPos,iMax);
	}
	else if(nSBCode==SB_THUMBPOSITION)
	{
		m_CurrentScrollPos=nPos;
	}
	else if(nSBCode==SB_THUMBTRACK)
	{
		m_CurrentScrollPos=nPos;
	}

	//
	// ���δ�ı�λ�ò���Ҫ�ػ�
	//
	if(iPosTemp==m_CurrentScrollPos)
		return;

	SetScrollPos(SB_VERT,m_CurrentScrollPos);

	this->Invalidate();
	this->UpdateWindow();
}

////////////////////////////////////////////////////////////
//
// ��Ӧ��������Ϣ
//
void CPropertyWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	::SetFocus(this->GetSafeHwnd());

	if(this->m_IsInChangeElementRect)
	{
		if(m_SelectRow>=0 && GetElementAt(m_SelectRow)->pControlWnd!=NULL)
		{
			GetElementAt(this->m_SelectRow)->pControlWnd->MoveWindow(0,0,0,0);
		}
		return;
	}

	// ��λͼ�е�Ԫ���в���
	BitmapElementsHitTest(point);

	// ������Ԫ�н��в���
	LeftElementsHitTest(point);

	// ���ҵ�Ԫ�н��в���
	RightElementsHitTest(point);

	CWnd::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////
//
// ��Ӧ������뽹��
//
void CPropertyWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	//
	// ���ñ�ѡ�е�Ԫ��������뽹��ʱ�ı���ɫ
	//
	if(this->m_SelectRow>=0)
	{
		GetElementAt(m_SelectRow)->BackGroundColor=DEFAULT_SELECT_COLOR_FOCUS;
	}
	
	this->Invalidate();
}

///////////////////////////////////////////////
//
// ��Ӧʧȥ���뽹��
//
void CPropertyWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd *pParentWnd;

	CWnd::OnKillFocus(pNewWnd);

	//
	// ��������뽹�����THIS�ؼ����Ӵ���ʱ�����ı�THIS�ؼ���ʽ��
	//
	if(pNewWnd!=NULL)
	{
		pParentWnd=pNewWnd->GetParent();


		while(pParentWnd)
		{
			if(pParentWnd==this)
				break;

			pParentWnd=pParentWnd->GetParent();
		}

		if(pParentWnd==this)
		{
			return;
		}
	}
	
	//
	// ���ñ�ѡ�е�Ԫ��ʧȥ���뽹��ʱ�ı���ɫ
	// �������е��ӿؼ����ɼ�
	//
	if(this->m_SelectRow>=0)
	{
		GetElementAt(m_SelectRow)->BackGroundColor=DEFAULT_SELECT_COLOR_NOFOCUS;
		GetElementAt(m_SelectRow)->pControlWnd->MoveWindow(0,0,0,0);
	}

	this->Invalidate();
	
}

BOOL CPropertyWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//
	// ���ӿؼ�ʧȥ���뽹�㣬�ı�ѡ�е�ԪΪ�޽���ı���ɫ
	//
	if(HIWORD(wParam)==EN_KILLFOCUS || HIWORD(wParam)==CBN_KILLFOCUS)
	{
		if(this->m_SelectRow>=0)
		{
			GetElementAt(m_SelectRow)->BackGroundColor=DEFAULT_SELECT_COLOR_NOFOCUS;
			GetElementAt(m_SelectRow)->pControlWnd->MoveWindow(0,0,0,0);
		}

		this->Invalidate();
	}

	//
	// ͨ��PWN_CONTROLͨ��ת���ӿؼ�����Ϣ
	//
	if(lParam!=NULL && m_SelectRow>=0)
	{
		ElementDef_Wnd *pElementWnd;
		PWNControlStruct NofityStruct;
		CWnd *pParent;

		pElementWnd=GetElementAt(m_SelectRow);

		if(pElementWnd->pControlWnd==NULL || pElementWnd->pControlWnd->GetSafeHwnd()!=(HWND)lParam)
		{
			return CWnd::OnCommand(wParam, lParam);;
		}

		//
		// ��дNofityStruct�ṹ
		//
		NofityStruct.nmh.code=PWN_CONTROL;
		NofityStruct.nmh.hwndFrom=this->GetSafeHwnd();
		NofityStruct.nmh.idFrom=GetWindowLong(this->GetSafeHwnd(),GWL_ID);

		NofityStruct.szElementName=pElementWnd->ElementName;
		NofityStruct.Style=pElementWnd->Style;
		NofityStruct.Width=pElementWnd->Width;
		NofityStruct.Height=pElementWnd->Height;
		NofityStruct.BackGroundColor=pElementWnd->BackGroundColor;
		NofityStruct.TextColor=pElementWnd->TextColor;
		NofityStruct.pVoid=pElementWnd->pVoid;
		NofityStruct.szRightElementText=pElementWnd->RightElementText;
		NofityStruct.Row=pElementWnd->Row;

		NofityStruct.idCtrl=(int)LOWORD(wParam);
		NofityStruct.wNotifyCode=HIWORD(wParam);
		NofityStruct.hWndControl=(HWND)lParam;

		pParent=this->GetParent();

		if(pParent==NULL || !IsWindow(pParent->GetSafeHwnd()))
		{
			return CWnd::OnCommand(wParam, lParam);
		}

		// �򸸴��ڷ�����Ϣ
		pParent->SendMessage(WM_NOTIFY,NofityStruct.nmh.idFrom,(LPARAM)&NofityStruct);

	}

	return CWnd::OnCommand(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////
//
// ��Ӧ����м��Ĺ���
//
BOOL CPropertyWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{

	if(zDelta>=0)
	{
		this->OnVScroll(SB_LINEUP,0,NULL);
	}
	else
	{
		this->OnVScroll(SB_LINEDOWN,0,NULL);
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//////////////////////////////////////////////////////////
//
// ��Ӧ����ƶ�
//
void CPropertyWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect DragRect;
	CRect ClientRect;

	ElementDef_Wnd *pElementWnd;

	//
	// ��������������������ڴ��ƶ�ǰ���ڿɸı��С�ķ�Χ��
	// ��������
	//
	if((nFlags & MK_LBUTTON) && m_IsInChangeElementRect==FALSE)
	{
		return;
	}

	if((nFlags & MK_LBUTTON) && m_IsInChangeElementRect==TRUE)
	{
		m_IsInChangeElementRect=TRUE;
	}
	else
	{
		m_IsInChangeElementRect=FALSE;
	}

	//
	// ��õ�һ����ChildElement��ʽ�ĵ�Ԫ
	//
	for(int i=0;;i++)
	{
		pElementWnd=(ElementDef_Wnd*)this->GetElementAt(i);

		if(!pElementWnd)
			return;

		if(pElementWnd->Style & ChildElement)
			break;
	}

	GetClientRect(&ClientRect);

	DragRect.SetRect(pElementWnd->LeftColumeRect.right-3,ClientRect.top,
					 pElementWnd->LeftColumeRect.right+3,ClientRect.bottom);


	if(DragRect.PtInRect(point))
	{

		// ���ÿɸñ䵥Ԫ�Ĵ�С
		m_IsInChangeElementRect=TRUE;
	}

	if(m_IsInChangeElementRect)
	{
		//
		//
		//
		if(nFlags & MK_LBUTTON)
		{
			ChangeLeftElementWidth(pElementWnd->LeftColumeRect.right-
							   pElementWnd->LeftColumeRect.left+
							   point.x-m_MouseMovePt.x);

			this->Invalidate();
			UpdateWindow();
		}
	}

	// ��������λ��
	m_MouseMovePt=point;

	CWnd::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
// �ı�������ʽ
//
BOOL CPropertyWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	//
	// ��������ָ���ķ�Χ�ڽ���Ϊ�ɸñ䵥Ԫ��С��ͼ��
	//
	if(m_IsInChangeElementRect)
	{
		SetCursor(m_hDragCursor);
	}
	else
	{
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

	return TRUE;
}

///////////////////////////////////////////////////////
//
// ��ӦWM_SIZE��Ϣ
//
void CPropertyWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd *pWnd;

	CWnd::OnSize(nType, cx, cy);
	
	CClientDC dc(this);

	m_BitmapSurface.DeleteObject();

	//�����봰��ͬ��С��λͼ���ڻ���
	m_BitmapSurface.CreateCompatibleBitmap(&dc,cx,cy);

	this->UpdateScrollBarInfo();
	this->UpdateElementsListRect();

	if(m_SelectRow>=0)
	{
		pWnd=this->GetElementAt(m_SelectRow)->pControlWnd;

		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
		{
			pWnd->MoveWindow(0,0,0,0);
		}
	}
}

///////////////////////////////////////////////////////////////////////

/////////////////////////////////////////
//
// �����ܵĵ�Ԫ��
//
int CPropertyWnd::GetElementCount()
{
	return m_ElementCount;
}

/////////////////////////////////////////////////////////////////////////
//
// ���뵥Ԫ
// pElement[in]		���ڶ��嵥Ԫ��ElementDef�ṹ
// InsertAfter[in]	��Ԫ�����λ��,���С��0�����ݼ����ڿ�ͷλ��,
//					����������ĵ�Ԫ�����������λ��
//
int CPropertyWnd::InsertElement(ElementDef *pElement, int InsertAfter)
{

	ElementDef_Wnd *pElementWnd;

	if(pElement==NULL)
		return -1;

	pElementWnd=new ElementDef_Wnd;

	pElementWnd->ElementName=pElement->szElementName;
	pElementWnd->RightElementText=pElement->RightElementText;
	pElementWnd->strFieldName = pElement->strFieldName;
	
	pElementWnd->Style=pElement->Style;
	pElementWnd->Width=pElement->Width;
	pElementWnd->Height=pElement->Height;

	if(pElementWnd->Style & DefineControlWnd)
	{
		pElementWnd->pControlWnd=pElement->pControlWnd;
	}

	//
	// �����DefineBackColor ��ʽ�����û��Զ���ı���ɫ
	// ���򽫻���Ĭ�ϵı���ɫ
	//
	if(pElement->Style & DefineBackColor)
		pElementWnd->BackGroundColor=pElement->BackGroundColor;
	else if(pElement->Style & TitleElement)
		pElementWnd->BackGroundColor=DEFAULT_BACKGROUND_COLOR;
	else
		pElementWnd->BackGroundColor=RGB(255,255,255);

	//
	// �����DefineTextColor ��ʽ�����û��Զ��������ɫ
	// ���򽫻���Ĭ�ϵ�����ɫ
	//
	if(pElement->Style & DefineTextColor)
		pElementWnd->TextColor=pElement->TextColor;
	else
		pElementWnd->TextColor=RGB(0,0,0);

	pElementWnd->pVoid=pElement->pVoid;

	//
	// ���InsertAfterС��0������ڿ�ʼλ��
	// ������ڵ������Ԫ�ص�λ��
	// �µ�Ԫ�����������λ����
	//
	if(InsertAfter<0)
	{
		this->m_ElementList.AddHead(pElementWnd);
	}
	else if(InsertAfter>=m_ElementCount-1)
	{
		pElementWnd->Row=m_ElementCount;
		this->m_ElementList.AddTail(pElementWnd);
	}
	else
	{
		ElementDef_Wnd *pTest;
		POSITION pos;

		pos=this->m_ElementList.GetHeadPosition();

		while(pos!=NULL)
		{
			pTest=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

			if(pTest->Row>InsertAfter)
				break;
		}

		this->m_ElementList.InsertBefore(pos,pElementWnd);
	}

	m_ElementCount++;

	// ������Ҫ���¼��������Ԫ��λ�õı�־
	m_IsNeedReUpdateElementsListRect=TRUE;

	return pElementWnd->Row;
}

//////////////////////////////////////////////////////////////////////
//
// ����ָ��λ�õĵ�Ԫ
//
// ElementNum[in]	�践�صĵ�Ԫ�к�
//
// ����ָ��λ�õĵ�Ԫ��ElementDef_Wnd�ṹ
//
CPropertyWnd::ElementDef_Wnd* CPropertyWnd::GetElementAt(int ElementNum)
{
	POSITION pos;
	ElementDef_Wnd *pElementWnd;

	if(ElementNum<0 || ElementNum>=m_ElementCount)
		return NULL;

	pos=this->m_ElementList.GetHeadPosition();

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		if(pElementWnd->Row==ElementNum)
			break;
	}

	if(pElementWnd->pControlWnd && IsWindow(pElementWnd->pControlWnd->GetSafeHwnd()))
	{
		pElementWnd->pControlWnd->GetWindowText(pElementWnd->RightElementText);
	}

	return pElementWnd;
}

/////////////////////////////////////////////////////////////////////
//
// ����ָ��λ�õĵ�Ԫ
//
// pElement[out]	���صĵ�Ԫ�ṹ
// ElementNum[in]	�践�صĵ�Ԫ�к�
//
// ����ɹ����ص�Ԫ���кţ����򷵻�-1
// 
// ע�⣺���ص�pControlWnd��������û��Զ���Ĳ�Ӧ���ͷ�
//
int CPropertyWnd::GetElementAt(ElementDef *pElement, int ElementNum)
{
	ElementDef_Wnd *pElementWnd;

	//���pElementΪNULL��Ԫ�кúŴ��ڵ�Ԫ��������-1
	if(pElement==NULL || ElementNum>=GetElementCount())
		return -1;

	pElementWnd=GetElementAt(ElementNum);
	if ( pElementWnd == NULL )
		return -1;

	pElement->BackGroundColor	=pElementWnd->BackGroundColor;
	pElement->Height			=pElementWnd->Height;
	pElement->pControlWnd		=pElementWnd->pControlWnd;
	pElement->pVoid				=pElementWnd->pVoid;
	pElement->RightElementText	=pElementWnd->RightElementText;
	pElement->strFieldName		=pElementWnd->strFieldName;
	pElement->Style				=pElementWnd->Style;
	pElement->szElementName		=pElementWnd->ElementName;
	pElement->TextColor			=pElementWnd->TextColor;
	pElement->Width				=pElementWnd->Width;

	return -1;
}

/////////////////////////////////////////////////
//
// ���¼��������Ԫ��λ������
//
void CPropertyWnd::UpdateElementsListRect()
{
	CRect ClientRect;
	POSITION pos;
	SIZE TextSize;
	int MaxWidth=0;
	int RowPos;
	ElementDef_Wnd *pElementWnd,*preElementWnd;

	if(!IsWindow(this->GetSafeHwnd()))
	{
		return;
	}

	CClientDC dc(this);

	GetClientRect(&ClientRect);

	pos=this->m_ElementList.GetHeadPosition();
	RowPos=0;

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		//���������к�
		pElementWnd->Row=RowPos++;

		//����λͼ��
		pElementWnd->BitmapColumeRect.left=0;
		pElementWnd->BitmapColumeRect.right=pElementWnd->BitmapColumeRect.left+GetBitmapColumeWidth();

		//��ԪӦ�ý�����λͼ��
		pElementWnd->LeftColumeRect.left=pElementWnd->BitmapColumeRect.right;

		if(pElementWnd->Row==0)
		{
			pElementWnd->LeftColumeRect.top=0;
		}
		else
		{
			pElementWnd->LeftColumeRect.top=preElementWnd->LeftColumeRect.bottom;
		}

		// �����Ԫ���ݵĳ���
		::GetTextExtentPoint32(dc.GetSafeHdc(),pElementWnd->ElementName,
							   lstrlen(pElementWnd->ElementName),&TextSize);

		TextSize.cy+=5;
		TextSize.cx+=5;

		//
		// ������Ŀ�ȣ��߶�С��0ʱ�Զ�����
		//
		if(pElementWnd->Width<0)
		{
			pElementWnd->LeftColumeRect.right=pElementWnd->LeftColumeRect.left+TextSize.cx;
			pElementWnd->Width=TextSize.cx;
		}
		else
		{
			pElementWnd->LeftColumeRect.right=pElementWnd->LeftColumeRect.left+pElementWnd->Width;
		}

		if(pElementWnd->Height<0)
		{
			pElementWnd->LeftColumeRect.bottom=pElementWnd->LeftColumeRect.top+TextSize.cy;
			pElementWnd->Height=TextSize.cy;
		}
		else
		{
			pElementWnd->LeftColumeRect.bottom=pElementWnd->LeftColumeRect.top+pElementWnd->Height;
		}

		// ��Ԫ��Ĵ�С���ܳ����������ڵ����ұ�
		if(pElementWnd->LeftColumeRect.right>ClientRect.right)
		{
			pElementWnd->LeftColumeRect.right=ClientRect.right;
			pElementWnd->Width=pElementWnd->LeftColumeRect.right-pElementWnd->LeftColumeRect.left;
		}

		if(MaxWidth<pElementWnd->LeftColumeRect.right-pElementWnd->LeftColumeRect.left)
			MaxWidth=pElementWnd->LeftColumeRect.right-pElementWnd->LeftColumeRect.left;

		//
		// ȷ���ұߴ��ڵĴ�С
		//
		pElementWnd->RightColumeRect.left=pElementWnd->LeftColumeRect.right;
		pElementWnd->RightColumeRect.top=pElementWnd->LeftColumeRect.top;
		
		// �ұߵ�Ԫ���ڻ��������ʣ�µĲ���
		pElementWnd->RightColumeRect.right=ClientRect.right;
		pElementWnd->RightColumeRect.bottom=pElementWnd->LeftColumeRect.bottom;

		preElementWnd=pElementWnd;
	}

	//
	// ��������е�Ԫʹ�����ȣ����������Ŀ��
	//
	pos=this->m_ElementList.GetHeadPosition();
	
	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		pElementWnd->LeftColumeRect.right=pElementWnd->LeftColumeRect.left+MaxWidth;

		pElementWnd->Width=pElementWnd->LeftColumeRect.right-pElementWnd->LeftColumeRect.left;

		pElementWnd->RightColumeRect.left=pElementWnd->LeftColumeRect.right;

		//�����TitleElement��ʽ������Ԫ�������������ڿ�
		if(pElementWnd->Style & TitleElement)
		{
			pElementWnd->LeftColumeRect.right=ClientRect.right;
		}

		//����λͼ��
		pElementWnd->BitmapColumeRect.top=pElementWnd->LeftColumeRect.top;
		pElementWnd->BitmapColumeRect.bottom=pElementWnd->LeftColumeRect.bottom;

	}

	m_IsNeedReUpdateElementsListRect=FALSE;
}

/////////////////////////////////////////////
//
// ���¸��¹���������Ϣ
//
// �Թ��������з�Χ����
// �˺���Ӧ����UpdateElementsListRect�����̵���
//
void CPropertyWnd::UpdateScrollBarInfo()
{
	POSITION pos;
	int VisibleElementCount=0;
	int VisibleElementHeight=0;
	CRect rect;
	ElementDef_Wnd *pElementWnd;

	if(!IsWindow(this->GetSafeHwnd()))
	{
		return;
	}

	this->GetClientRect(&rect);


//	this->SetScrollRange(SB_VERT,0,0,FALSE);

	pos=this->m_ElementList.GetHeadPosition();

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		VisibleElementHeight+=pElementWnd->Height;
		VisibleElementCount++;

		if(VisibleElementHeight>rect.Height())
		{
			this->SetScrollRange(SB_VERT,0,this->m_ElementCount-VisibleElementCount+1,FALSE);
			break;
		}
		else if(VisibleElementHeight==rect.Height())
		{
			this->SetScrollRange(SB_VERT,0,this->m_ElementCount-VisibleElementCount,FALSE);
			break;
		}
		
	}

	this->SetScrollPos(SB_VERT,0,FALSE);
	m_CurrentScrollPos=0;

	Invalidate();
	UpdateWindow();
}

//////////////////////////////////////////////////////////
//
// �����������ڵı�Ӱɫ
//
// Color[in]	�������ڵı���ɫ
//
void CPropertyWnd::SetWndBackGroundColor(COLORREF Color)
{
	m_WndBackGroundColor=Color;
	m_WndBackGroundBrush.DeleteObject();

	m_WndBackGroundBrush.CreateSolidBrush(Color);
}

/////////////////////////////////////////////////////////
//
// �����������ڵı�Ӱɫ
//
COLORREF CPropertyWnd::GetWndBackGroundColor()
{
	return m_WndBackGroundColor;
}

//////////////////////////////////////////////////////
//
// ���쵥Ԫ
//
void CPropertyWnd::PaintElement(CDC *pDC)
{
	POSITION pos;
	CRect rect;
	int VisibleHeight=0;
	int baseTop;
	ElementDef_Wnd *pElementWnd;
	CBrush *pOldBrush;
	CBrush WhiteBrush;
	CPen *pOldPen;
	BOOL bSelectElementIsDraw=FALSE;		// ��ѡ�еĵ�Ԫ�Ƿ񱻻���

	GetClientRect(&rect);

	//
	// ����λͼ�еı���ɫ
	//
	CRect BitmapColumeRect(0,0,
						   this->m_BitmapColumeWidth,rect.Height());

	pDC->FillRect(&BitmapColumeRect,&this->m_BitmapColumeBrush);


	//�ҵ���ǰ����λ���ϵĵ�Ԫ
	pos=m_ElementList.FindIndex(m_CurrentScrollPos);

	if(pos==NULL)
	{
		return;
	}

	pOldPen=pDC->SelectObject(&this->m_FramePen);

	//
	// ������Ԫ
	//
	while(pos!=NULL)
	{
		CBrush Brush;

		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		//
		// �������ʾ�˵�Ԫ,���Ҳ�����TitleElement��ʽ,��ȡ��һ����Ԫ
		//
		if(pElementWnd->IsHideThisElement && !(pElementWnd->Style & TitleElement))
		{
			continue;
		}

		baseTop=pElementWnd->LeftColumeRect.top-VisibleHeight;
		//
		// ������Ԫ�߿�
		//
		CRect LeftElementRect(pElementWnd->LeftColumeRect.left,pElementWnd->LeftColumeRect.top-baseTop-1,
							  pElementWnd->LeftColumeRect.right,pElementWnd->LeftColumeRect.bottom-baseTop);

		Brush.CreateSolidBrush(pElementWnd->BackGroundColor);
		pOldBrush=pDC->SelectObject(&Brush);

		pDC->Rectangle(&LeftElementRect);

		pDC->SelectObject(pOldBrush);

		//
		// �����ҵ�Ԫ�߿�
		//
		CRect RightElementRect(pElementWnd->RightColumeRect.left-1,pElementWnd->RightColumeRect.top-baseTop-1,
							   pElementWnd->RightColumeRect.right,pElementWnd->RightColumeRect.bottom-baseTop);

		if(pElementWnd->Style & ChildElement)
			pDC->Rectangle(&RightElementRect);

		//
		// ����λͼ�еı���ɫ
		//
		CRect BitmapColumeRect(pElementWnd->BitmapColumeRect.left,pElementWnd->BitmapColumeRect.top-baseTop,
							   pElementWnd->BitmapColumeRect.right,pElementWnd->BitmapColumeRect.bottom-baseTop);

		pDC->FillRect(&BitmapColumeRect,&this->m_BitmapColumeBrush);

		//
		// ����λͼ���е�����
		//
		if(pElementWnd->Style & TitleElement)
		{
			pDC->SelectObject(pOldPen);

			POINT CenterPt;
			CRect FlagRect;
			int   nLineLength = 5 ;
			// λͼ�е�Ԫ������
			CenterPt.x=pElementWnd->BitmapColumeRect.left+
					   (pElementWnd->BitmapColumeRect.right-pElementWnd->BitmapColumeRect.left)/2;
			CenterPt.y=pElementWnd->BitmapColumeRect.top-baseTop+
					   (pElementWnd->BitmapColumeRect.bottom-pElementWnd->BitmapColumeRect.top)/2;

			//
			// ��IsHideThisElementΪTRUEʱ�����л���һ���������м�һ��'+'
			// ��IsHideThisElementΪFALSEʱ�����л���һ�������μ�һ��'-'
			//
			FlagRect.SetRect(CenterPt.x-nLineLength, CenterPt.y-nLineLength, CenterPt.x+nLineLength, CenterPt.y+nLineLength);

			pDC->Rectangle(&FlagRect);
			
			nLineLength -= 2;
			if(pElementWnd->IsHideThisElement)
			{
				pDC->MoveTo(CenterPt.x-nLineLength, CenterPt.y);
				pDC->LineTo(CenterPt.x+nLineLength, CenterPt.y);

				pDC->MoveTo(CenterPt.x, CenterPt.y-nLineLength);
				pDC->LineTo(CenterPt.x, CenterPt.y+nLineLength);
			}
			else
			{
				pDC->MoveTo(CenterPt.x-nLineLength, CenterPt.y);
				pDC->LineTo(CenterPt.x+nLineLength, CenterPt.y);
			}

			pDC->SelectObject(&this->m_FramePen);
		}

		//
		// ������Ԫ����
		//
		pDC->SetTextColor(pElementWnd->TextColor);
		pDC->SetBkColor(pElementWnd->BackGroundColor);
		pDC->DrawText(pElementWnd->ElementName,&LeftElementRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		//
		// ��ʾ�ҵ�Ԫ�ؼ��е�����
		//
		if(pElementWnd->pControlWnd)
		{
			pElementWnd->pControlWnd->GetWindowText(pElementWnd->RightElementText);
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkColor(RGB(255,255,255));

			RightElementRect.left+=5;
			pDC->DrawText(pElementWnd->RightElementText,&RightElementRect,DT_SINGLELINE|DT_VCENTER);

			//
			// ��������Ƶĵ�Ԫ��ѡ�ֵĵ�Ԫ�������Ӧ�Ŀؼ���λ��
			//
			if(pElementWnd->Row==this->m_SelectRow)
			{
				pElementWnd->pControlWnd->SetWindowPos(NULL,pElementWnd->RightColumeRect.left,
													   pElementWnd->RightColumeRect.top-baseTop,
													   pElementWnd->RightColumeRect.right-pElementWnd->RightColumeRect.left,
													   pElementWnd->RightColumeRect.bottom-pElementWnd->RightColumeRect.top,
													   SWP_NOZORDER|SWP_NOSIZE|SWP_SHOWWINDOW);

				// ���ñ�ѡ�еĵ�Ԫ�Ѿ�������
				bSelectElementIsDraw=TRUE;
			}
		}


		//
		//���ɼ����ִ��ڴ���ʱ,ֹͣ��
		//
		VisibleHeight+=pElementWnd->Height;

		if(VisibleHeight>rect.Height())
			break;
	}

	pDC->SelectObject(pOldPen);

	//
	// �����ѡ�еĵ�Ԫδ�����ƣ������ض�Ӧ���ӿ׼�
	//
	if(!bSelectElementIsDraw)
	{
		if(this->m_SelectRow>=0 && GetElementAt(this->m_SelectRow)->pControlWnd)
		{
			GetElementAt(this->m_SelectRow)->pControlWnd->ShowWindow(SW_HIDE);
		}
	}

}


/////////////////////////////////////////////////////
//
// ����Ϊλͼ�еĿ��
//
void CPropertyWnd::SetBitmapColumeWidth(int Width)
{
	if(Width<0)
		Width=0;

	m_BitmapColumeWidth=Width;
}

////////////////////////////////////////////////////
//
// ����λͼ�еĿ��
//
int CPropertyWnd::GetBitmapColumeWidth()
{
	return m_BitmapColumeWidth;
}

////////////////////////////////////////////////////////
//
// ��λͼ�еĵ�����в���
//
void CPropertyWnd::BitmapElementsHitTest(CPoint point)
{
	POSITION pos;
	int VisibleHeight=0;
	int baseTop;
	ElementDef_Wnd *pElementWnd;
	CRect rect;

	GetClientRect(&rect);

	//�ҵ���ǰ����λ���ϵĵ�Ԫ
	pos=m_ElementList.FindIndex(m_CurrentScrollPos);

	if(pos==NULL)
	{
		return;
	}

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		//
		// �������ʾ�˵�Ԫ,���Ҳ�����TitleElement��ʽ,��ȡ��һ����Ԫ
		//
		if(pElementWnd->IsHideThisElement && !(pElementWnd->Style & TitleElement))
		{
			continue;
		}

		baseTop=pElementWnd->LeftColumeRect.top-VisibleHeight;

		if(pElementWnd->Style & TitleElement)
		{
			BOOL HideFlag;
			CRect ElementRect(pElementWnd->BitmapColumeRect.left,pElementWnd->BitmapColumeRect.top-baseTop,
							  pElementWnd->BitmapColumeRect.right,pElementWnd->BitmapColumeRect.bottom-baseTop);

			if(ElementRect.PtInRect(point))
			{
				HideFlag=!(pElementWnd->IsHideThisElement);

				pElementWnd->IsHideThisElement=HideFlag;

				while(pos!=NULL)
				{
					pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

					if(pElementWnd->Style & TitleElement)
						break;

					pElementWnd->IsHideThisElement=HideFlag;
				}

				Invalidate();
				UpdateWindow();

				break;
			}
		}

		VisibleHeight+=pElementWnd->Height;

		if(VisibleHeight>rect.Height())
			break;
	}
}

////////////////////////////////////////////////////////
//
// ����Ԫ�ĵ�����в���
//
void CPropertyWnd::LeftElementsHitTest(CPoint point)
{
	POSITION pos;
	int VisibleHeight=0;
	int baseTop;
	ElementDef_Wnd *pElementWnd;
	CRect rect;

	GetClientRect(&rect);

	//�ҵ���ǰ����λ���ϵĵ�Ԫ
	pos=m_ElementList.FindIndex(m_CurrentScrollPos);

	if(pos==NULL)
	{
		return;
	}

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		//
		// �������ʾ�˵�Ԫ,���Ҳ�����TitleElement��ʽ,��ȡ��һ����Ԫ
		//
		if(pElementWnd->IsHideThisElement && !(pElementWnd->Style & TitleElement))
		{
			continue;
		}

		baseTop=pElementWnd->LeftColumeRect.top-VisibleHeight;

		//
		// �Ծ���ChildElement����ʽ���е������
		//
		if(pElementWnd->Style & ChildElement)
		{
			CRect ElementRect(pElementWnd->LeftColumeRect.left,pElementWnd->LeftColumeRect.top-baseTop,
							  pElementWnd->LeftColumeRect.right,pElementWnd->LeftColumeRect.bottom-baseTop);

			if(ElementRect.PtInRect(point))
			{
				//
				// ����ؼ��Ӵ��ڻ�δ�����򴴽��Ӵ���
				//
				if(pElementWnd->pControlWnd==NULL)
				{
					if(!CreateElementControl(pElementWnd->Row))
						return;
				}

				//
				// �ָ��ϴα�ѡ�е�Ԫ����ɫ
				// ��ʹ�ϴα�ѡ�е��ӿ׼����ɼ�
				//
				if(m_SelectRow>=0)
				{
					GetElementAt(m_SelectRow)->BackGroundColor=m_SelectElementBackGroundColorBank;
					GetElementAt(m_SelectRow)->pControlWnd->MoveWindow(0,0,0,0);
				}

				// ���汻ѡ�е�Ԫԭ������ɫ
				m_SelectElementBackGroundColorBank=pElementWnd->BackGroundColor;

				if(m_SelectRow!=pElementWnd->Row)
				{
					m_SelectRow=pElementWnd->Row;

					// ����PWN_SELECTCHANGE֪ͨ
					SendPWNSelectChangeNotify();
				}

				// ���汻ѡ�е�Ԫ���к�
				m_SelectRow=pElementWnd->Row;

				pElementWnd->BackGroundColor=DEFAULT_SELECT_COLOR_FOCUS;

				//
				// ������ӿؼ��������ӿ׼����ҵ�Ԫ��
				//
				if(pElementWnd->pControlWnd)
				{
					CRect PlaceRect(pElementWnd->RightColumeRect.left,pElementWnd->RightColumeRect.top-baseTop,
									pElementWnd->RightColumeRect.right-1,pElementWnd->RightColumeRect.bottom-baseTop-1);

					pElementWnd->pControlWnd->MoveWindow(&PlaceRect);
					pElementWnd->pControlWnd->ShowWindow(SW_SHOWNORMAL);
				}



				this->Invalidate();
			}

		}

		baseTop=pElementWnd->LeftColumeRect.top-VisibleHeight;


		VisibleHeight+=pElementWnd->Height;

		if(VisibleHeight>rect.Height())
			break;
	}
}

////////////////////////////////////////////////////////
//
// ���ҵ�Ԫ�ĵ�����в���
//
void CPropertyWnd::RightElementsHitTest(CPoint point)
{
	POSITION pos;
	int VisibleHeight=0;
	int baseTop;
	ElementDef_Wnd *pElementWnd;
	CRect rect;

	GetClientRect(&rect);

	//�ҵ���ǰ����λ���ϵĵ�Ԫ
	pos=m_ElementList.FindIndex(m_CurrentScrollPos);

	if(pos==NULL)
	{
		return;
	}

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		//
		// �������ʾ�˵�Ԫ,���Ҳ�����TitleElement��ʽ,��ȡ��һ����Ԫ
		//
		if(pElementWnd->IsHideThisElement && !(pElementWnd->Style & TitleElement))
		{
			continue;
		}

		baseTop=pElementWnd->LeftColumeRect.top-VisibleHeight;

		//
		// �Ծ���ChildElement����ʽ���е������
		//
		if(pElementWnd->Style & ChildElement)
		{
			CRect ElementRect(pElementWnd->RightColumeRect.left,pElementWnd->RightColumeRect.top-baseTop,
							  pElementWnd->RightColumeRect.right,pElementWnd->RightColumeRect.bottom-baseTop);

			if(ElementRect.PtInRect(point))
			{
				//
				// ����ؼ��Ӵ��ڻ�δ�����򴴽��Ӵ���
				//
				if(pElementWnd->pControlWnd==NULL)
				{
					if(!CreateElementControl(pElementWnd->Row))
						return;
				}

				//
				// �ָ��ϴα�ѡ�е�Ԫ����ɫ
				// ��ʹ�ϴα�ѡ�е��ӿ׼����ɼ�
				//
				if(m_SelectRow>=0)
				{
					GetElementAt(m_SelectRow)->BackGroundColor=m_SelectElementBackGroundColorBank;
					GetElementAt(m_SelectRow)->pControlWnd->MoveWindow(0,0,0,0);
				}

				// ���汻ѡ�е�Ԫԭ������ɫ
				m_SelectElementBackGroundColorBank=pElementWnd->BackGroundColor;

				if(m_SelectRow!=pElementWnd->Row)
				{
					m_SelectRow=pElementWnd->Row;

					// ����PWN_SELECTCHANGE֪ͨ
					SendPWNSelectChangeNotify();
				}

				// ���汻ѡ�е�Ԫ���к�
				m_SelectRow=pElementWnd->Row;

				pElementWnd->BackGroundColor=DEFAULT_SELECT_COLOR_FOCUS;

				//
				// ������ӿؼ��������ӿ׼����ҵ�Ԫ��
				//
				if(pElementWnd->pControlWnd)
				{
					CRect PlaceRect(pElementWnd->RightColumeRect.left,pElementWnd->RightColumeRect.top-baseTop,
									pElementWnd->RightColumeRect.right-1,pElementWnd->RightColumeRect.bottom-baseTop-1);

					pElementWnd->pControlWnd->MoveWindow(&PlaceRect);
					pElementWnd->pControlWnd->ShowWindow(SW_SHOWNORMAL);

					pElementWnd->pControlWnd->SetFocus();
				}

				// ����PWN_SELECTCHANGE֪ͨ
				SendPWNSelectChangeNotify();

				this->Invalidate();
			}

		}

		baseTop=pElementWnd->LeftColumeRect.top-VisibleHeight;

		VisibleHeight+=pElementWnd->Height;

		if(VisibleHeight>rect.Height())
			break;
	}
}

/////////////////////////////////////////
//
// ��ÿɼ�������
//
int CPropertyWnd::GetVisibleRow()
{
	POSITION pos;
	int VisibleHeight=0;
	int VisibleRowCount=0;
	int baseTop;
	ElementDef_Wnd *pElementWnd;
	CRect rect;

	GetClientRect(&rect);

	//�ҵ���ǰ����λ���ϵĵ�Ԫ
	pos=m_ElementList.FindIndex(m_CurrentScrollPos);

	if(pos==NULL)
	{
		return 0;
	}

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		//
		// �������ʾ�˵�Ԫ,���Ҳ�����TitleElement��ʽ,��ȡ��һ����Ԫ
		//
		if(pElementWnd->IsHideThisElement && !(pElementWnd->Style & TitleElement))
		{
			continue;
		}

		VisibleRowCount++;

		baseTop=pElementWnd->LeftColumeRect.top-VisibleHeight;

		VisibleHeight+=pElementWnd->Height;

		if(VisibleHeight>rect.Height())
			break;
	}

	return VisibleRowCount;
}

//////////////////////////////////////////////////
//
// ����ָ���е���һ���ɼ����к�
//
// row[in]	ָ������
//
// ����row��һ���ɼ����к�
// ���û�пɼ��ĵ�Ԫ�������ظ���
//
int CPropertyWnd::GetNextVisibleRow(int row)
{
	ElementDef_Wnd *pElementWnd;
	POSITION pos;

	//��row����һ����Ԫ
	pos=m_ElementList.FindIndex(row+1);

	if(pos==NULL)
	{
		return 0;
	}

	//
	// ����һ���ɼ��ĵ�Ԫ�������Ԫ��TitleElement��ʽ��ʼ�տɼ�
	//
	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		if(!pElementWnd->IsHideThisElement || (pElementWnd->Style & TitleElement))
		{
			return pElementWnd->Row;
		}

	}

	return -1;
}

//////////////////////////////////////////////////
//
// ����ָ���е���һ���ɼ����к�
//
// row[in]	ָ������
//
// ����row��һ���ɼ����к�
// ���û�пɼ��ĵ�Ԫ�������ظ���
//
int CPropertyWnd::GetPrevVisibleRow(int row)
{
	ElementDef_Wnd *pElementWnd;
	POSITION pos;

	//��row����һ����Ԫ
	pos=m_ElementList.FindIndex(row-1);

	if(pos==NULL)
	{
		return 0;
	}

	//
	// ����һ���ɼ��ĵ�Ԫ�������Ԫ��TitleElement��ʽ��ʼ�տɼ�
	//
	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetPrev(pos);

		if(!pElementWnd->IsHideThisElement || (pElementWnd->Style & TitleElement))
		{
			return pElementWnd->Row;
		}

	}

	return -1;
}

////////////////////////////////////////////////
//
// ���¶���GetClientRect
//
// ������ȥ����������
//
void CPropertyWnd::GetClientRect(LPRECT lpRect)
{
	int iMax,iMin;

	if(lpRect==NULL)
		return;

	CWnd::GetClientRect(lpRect);

	GetScrollRange(SB_VERT,&iMin,&iMax);

	//����������ɼ��ͼ�ȥ���Ĳ���
	if(iMin<=iMax)
	{
//		lpRect->right-=18;
	}

}

///////////////////////////////////////////////////////
//
// �ñ���Ԫ�����е�Ԫ�Ŀ��
// ���ǲ��ı����TitleElement��ʽ�ĵ�Ԫ
//
// Width[in]	��Ԫ�Ŀ��
//
void CPropertyWnd::ChangeLeftElementWidth(int Width)
{
	POSITION pos;
	ElementDef_Wnd *pElementWnd;

	if(Width<0)
		return;

	pos=this->m_ElementList.GetHeadPosition();

	while(pos)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		if(pElementWnd->Style & TitleElement)
		{
			continue;
		}

		pElementWnd->Width=Width;
		pElementWnd->LeftColumeRect.right=pElementWnd->LeftColumeRect.left+Width;

		pElementWnd->RightColumeRect.left=pElementWnd->LeftColumeRect.right;
	}
}

////////////////////////////////////////////////////////
//
// ������Ԫ���ӿؼ�
//
// ��Ϊ��Ԫ���ӿؼ�������Ҫʱ�Զ��������˺��������û�
// ��Ҫʱ����,�˺������ڴ��ڴ����Ժ����
//
// ElementNum[in]	�贴���ӿؼ����к�
//
// ��������ɹ�����TURE�����ʧ�ܣ��������ӿؼ�����FALSE
//
BOOL CPropertyWnd::CreateElementControl(int ElementNum)
{
	ElementDef_Wnd *pElementWnd;
	BOOL bRet;

	if(ElementNum>=this->GetElementCount())
		return FALSE;

	pElementWnd=GetElementAt(ElementNum);

	//
	// ����ؼ��Ӵ��ڻ�δ����,���Ҳ����û��Զ���ؼ����͸�����ʽ�����Ӵ���
	//
	if(pElementWnd->pControlWnd==NULL && !(pElementWnd->Style & DefineControlWnd))
	{
		CRect EmptyRect(0,0,0,0);

		if(pElementWnd->Style & EditElement)
		{
			pElementWnd->pControlWnd=new CEdit;
			bRet=((CEdit*)(pElementWnd->pControlWnd))->Create(ES_LEFT|ES_AUTOHSCROLL,
														 EmptyRect,this,pElementWnd->Row);

		}
		else if(pElementWnd->Style & ComBoxElement)
		{
			CRect ComboxRect(0,0,100,150);

			pElementWnd->pControlWnd=new CComboBox;
			bRet=((CComboBox*)(pElementWnd->pControlWnd))->Create(CBS_DROPDOWN|WS_CHILD|WS_VSCROLL|CBS_AUTOHSCROLL,
															 ComboxRect,this,pElementWnd->Row);

		}

		if(!bRet)
		{
			delete pElementWnd->pControlWnd;
			pElementWnd->pControlWnd=NULL;
			return FALSE;
		}

		// ��ʼ�ӿؼ���������Ϊ�ҵ�Ԫ������
		if(!pElementWnd->RightElementText.IsEmpty())
			pElementWnd->pControlWnd->SetWindowText(pElementWnd->RightElementText);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//
// ���ҵ�Ԫ�������
//
// Row[in]			��Ԫ���к�
// szContent[in]	��Ҫ���ҵ�Ԫ��ʾ������
//
// ���ô˺���Ӧ�������Դ��ڴ���֮��
//
void CPropertyWnd::SetRightElementContent(int Row,LPCTSTR szContent)
{
	ElementDef_Wnd *pElementWnd;

	if(szContent==NULL)
		return;

	// �����ҵ�Ԫ�ڵ��Ӵ���
	this->CreateElementControl(Row);

	pElementWnd=GetElementAt(Row);

	pElementWnd->RightElementText=szContent;
	pElementWnd->pControlWnd->SetWindowText(szContent);

}

////////////////////////////////////////////////////////////////////////////////
//
// ͨ���ַ����ҵ�ָ����Ԫ���кţ�ͨ���кſ����ҵ�ָ���ĵ�Ԫ
//
// nStartAfter[in]		��ָ���ĵ�Ԫ��֮��ʼ����
// szString[in]			��Ҫ���ҵ�Ԫ���ַ���
// IsLeftElement[in]	ΪTRUEʱ������Ԫ�����ݣ���������ҵ�Ԫ������
//
// ����ҵ����ص�Ԫ���кţ����򷵻�-1
// 
int CPropertyWnd::FindElement(int nStartAfter, LPCTSTR szString,BOOL IsLeftElement)
{
	POSITION pos;
	int iPos=0;
	ElementDef_Wnd *pElementWnd;

	if(szString==NULL)
		return -1;
	
	pos=this->m_ElementList.GetHeadPosition();

	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		if(nStartAfter<iPos)
		{
			iPos++;
			continue;
		}

		//
		// �������Ԫ����ƥ���򷵻���Ԫ���к�
		// ������ҵ�Ԫ������ƥ���򷵻��ҵ�Ԫ���к�
		//
		if(IsLeftElement)
		{
			if(pElementWnd->ElementName==szString)
				return pElementWnd->Row;
		}
		else
		{
			if(pElementWnd->pControlWnd!=NULL && IsWindow(pElementWnd->pControlWnd->GetSafeHwnd()))
			{
				pElementWnd->pControlWnd->GetWindowText(pElementWnd->RightElementText);
			}

			if(pElementWnd->RightElementText==szString)
				return pElementWnd->Row;
		}
	}

	return -1;
}

/////////////////////////////////////////////////
//
// ���ڷ���PWN_SELECTCHANGE֪ͨ
//
void CPropertyWnd::SendPWNSelectChangeNotify()
{
	ElementDef_Wnd *pElementWnd;
	CWnd *pParent;

	PWNSelectChangeStruct NofityStruct;

	pParent=this->GetParent();

	if(pParent==NULL || !IsWindow(pParent->GetSafeHwnd()))
	{
		return;
	}

	if(this->m_SelectRow>0)
	{
		pElementWnd=(ElementDef_Wnd*)this->GetElementAt(m_SelectRow);
	}
	else
	{
		return;
	}

	NofityStruct.nmh.code=PWN_SELECTCHANGE;
	NofityStruct.nmh.hwndFrom=this->GetSafeHwnd();
	NofityStruct.nmh.idFrom=GetWindowLong(this->GetSafeHwnd(),GWL_ID);

	NofityStruct.szElementName=pElementWnd->ElementName;
	NofityStruct.Style=pElementWnd->Style;
	NofityStruct.Width=pElementWnd->Width;
	NofityStruct.Height=pElementWnd->Height;
	NofityStruct.BackGroundColor=pElementWnd->BackGroundColor;
	NofityStruct.TextColor=pElementWnd->TextColor;
	NofityStruct.pVoid=pElementWnd->pVoid;
	NofityStruct.szRightElementText=pElementWnd->RightElementText;
	NofityStruct.Row=pElementWnd->Row;

	// �򸸴��ڷ�����Ϣ
	pParent->SendMessage(WM_NOTIFY,NofityStruct.nmh.idFrom,(LPARAM)&NofityStruct);
}

////////////////////////////////////////
//
// ����������ݺ�Ӧ�õ��ô˺���
//
void CPropertyWnd::RefreshData()
{
	POSITION pos;
	ElementDef_Wnd *pElementWnd;
	int nRow;

	pos=this->m_ElementList.GetHeadPosition();

	//
	// ��������Ԫ���
	//
	nRow=0;
	while(pos!=NULL)
	{
		pElementWnd=(ElementDef_Wnd*)this->m_ElementList.GetNext(pos);

		pElementWnd->Row=nRow++;
	}
}


#undef DEFAULT_BACKGROUND_COLOR
#undef DEFAULT_SELECT_COLOR_FOCUS
#undef DEFAULT_SELECT_COLOR_NOFOCUS



