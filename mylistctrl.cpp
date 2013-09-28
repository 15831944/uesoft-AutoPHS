// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AutoPHS.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////
// CMyListCtrl

// ��������
CMyListCtrl::CMyListCtrl()
{
}

// ��������
CMyListCtrl::~CMyListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
   //{{AFX_MSG_MAP(CMyListCtrl)
	//}}AFX_MSG_MAP

#ifdef CMyListCtrl_SORTMODE
   ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
#endif

END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////
// CMyListCtrl helper functions

// ��ѡ���Ƿ����
BOOL CMyListCtrl::IsItem(int nItem) const
{
   return GetItemCount() > nItem;
}

//���б���������
BOOL CMyListCtrl::AddColumn(LPCTSTR strItem,int nItem,int nWidth,int nSubItem,int nMask,int nFmt)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	if(nWidth==-1)	
		lvc.cx = GetStringWidth(lvc.pszText) + 65;
	else
		lvc.cx =nWidth;
	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	return  InsertColumn(nItem,&lvc);
}

//���б���������
BOOL CMyListCtrl::AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
//	lvItem.Height=20;
	if(nImageIndex != -1){
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage |= LVIF_IMAGE;
	}
	if(nSubItem == 0)
		return InsertItem(&lvItem);
	return SetItem(&lvItem);
}

// ��ѡ���Ƿ����
BOOL CMyListCtrl::IsColumn(int nCol) const
{
   LV_COLUMN lvc;
   memset(&lvc, 0, sizeof(lvc));
   lvc.mask = LVCF_WIDTH;
   return GetColumn(nCol, &lvc);
}

// ������ѡ��
int CMyListCtrl::GetSelectedItem(int nStartItem /* = -1 */ ) const
{
   return GetNextItem(nStartItem, LVNI_SELECTED);
}

// ѡ��ָ������
BOOL CMyListCtrl::SelectItem(int nItem) 
{
   return SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
}

// ѡ���б���е�������
BOOL CMyListCtrl::SelectAll()
{
   BOOL bReturn(TRUE);

   for (int nItem = 0; IsItem(nItem); nItem++) 
      if (! SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED)) {
         bReturn = FALSE;
         break;
      }

   return bReturn;
}


// ���ظú����Խ���ѡ�е����и�����ʾ.
/*void CMyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
   // ȡ����ʾ�豸������ָ��
   CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
 
   // ���ñ�־
   UINT uiFlags(ILD_TRANSPARENT);

   // �������Ƿ�ѡ��
   if (lpDrawItemStruct->itemState & ODS_SELECTED) 
   {
	  //�������ѡ�������ֺͱ�������ɫ��Ϊ������ʾʱ����ɫ
      pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
      pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
      uiFlags |= ILD_BLEND50;
   }
   else 
   {
	  //���򽫽����ֺͱ�������ɫ��Ϊ������ɫ
      pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
      pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
   }

   // �õ��ͼ���б�ָ��
   CImageList* plistImages = GetImageList(LVSIL_SMALL);

   // �õ���������ʾ����
   LV_ITEM lvi;
   memset(&lvi, 0, sizeof(&lvi));
   lvi.mask = LVIF_IMAGE | LVIF_STATE;
   lvi.iItem = lpDrawItemStruct->itemID;
   GetItem(&lvi);

   // ������ѡ�е�ͼ���б�
   if (plistImages) {
      CPoint ptAt(lpDrawItemStruct->rcItem.left,
         lpDrawItemStruct->rcItem.top);
	  if ( ! plistImages->Draw(pDC, 0/*lvi.iImage*//*, ptAt, uiFlags) ) {
		  int x = 0;
		  x++;
	  }
   }

   // �������ڻ��Ʊ���ߵĻ���
   CPen pen(PS_SOLID, 0, ::GetSysColor(COLOR_WINDOWTEXT));
   CPen* penOld = pDC->SelectObject(&pen);

   char szText[256];     // �����ı�����,����ı����Ƚϴ�,�������Ӹ�����ĳ���
   LV_COLUMN lvc;
   memset(&lvc, 0, sizeof(lvc));
   lvc.mask = LVCF_WIDTH;

   for (int nColumn = 0; GetColumn(nColumn, &lvc); nColumn++) 
   {
      CRect rTextClip;

      // ����ǻ���һ��
      if (0 == nColumn) 
	  {
		 //����һ��ʱ�����ͼ���б��򽫻����ı��ľ��������Կճ�����ͼ�εĿռ�
         rTextClip.left   = lpDrawItemStruct->rcItem.left +
            (plistImages ? 16 : 0);
         rTextClip.top    = lpDrawItemStruct->rcItem.top;
         rTextClip.right  = lpDrawItemStruct->rcItem.left + lvc.cx;
         rTextClip.bottom = lpDrawItemStruct->rcItem.bottom;
      }
      else 
	  {
         // ������ǵ�һ�����ı�������������һ���ı����ε��Ҳ�
         rTextClip.left  = rTextClip.right;
         rTextClip.right = rTextClip.left + lvc.cx;
      }

      // ����ı�
      lvi.iItem = lpDrawItemStruct->itemID;
      lvi.mask = LVIF_TEXT;
      lvi.iSubItem = nColumn;
      lvi.pszText = szText;
      lvi.cchTextMax = sizeof(szText);
      GetItem(&lvi);

      // �õ��ı�����
      int nTextCount(lstrlen(lvi.pszText));

	  //�����ı�
      CSize sizText(pDC->GetOutputTextExtent(lvi.pszText,
         nTextCount));

      //ʹ�ı�������СһЩ��ʹ�м���ַָ�,��������ı��ϳ�������м��ı���������
      rTextClip.right -= 4;

	  //����ı���ȴ��ڻ��ƾ��εĿ��
      if (sizText.cx >= rTextClip.Width()) 
	  {

         // ���...�ĳ���
         CSize sizEllipse = pDC->GetOutputTextExtent("...", 3);

         // �����϶̵��ı�
         while (sizText.cx+sizEllipse.cx > (rTextClip.Width())  &&
           nTextCount > 1) 
		 {
            lvi.pszText[--nTextCount] = 0;
            sizText = pDC->GetOutputTextExtent(lvi.pszText,
               nTextCount);
         }

         // ���ı������ "...".
         lvi.pszText[nTextCount] = '.';
         lvi.pszText[nTextCount + 1] = '.';
         lvi.pszText[nTextCount + 2] = '.';
         lvi.pszText[nTextCount + 3] = 0;
      }

      // �ָ���������
      rTextClip.right += 4;

      // �����ı�
     pDC->ExtTextOut(rTextClip.left + 2, rTextClip.top-1,
       ETO_CLIPPED | ETO_OPAQUE, &rTextClip, lvi.pszText,
       lstrlen(lvi.pszText), NULL);


      //���Ʊ����
//      pDC->MoveTo(rTextClip.left+1, rTextClip.bottom-1);
//      pDC->LineTo(rTextClip.right-1, rTextClip.bottom-1);
//      pDC->MoveTo(rTextClip.right-1, rTextClip.bottom-1);
//      pDC->LineTo(rTextClip.right-1, rTextClip.top-1);
   }

   // ������ɫ
   if (lpDrawItemStruct->itemState & ODS_SELECTED) 
   {
      pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
      pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
   }

   // ������б�ѡ�����ѡ�����
   if (lpDrawItemStruct->itemState & ODS_FOCUS) 
   {
      CRect rTextClip(lpDrawItemStruct->rcItem);
      rTextClip.left += (plistImages ? 16 : 0);
      pDC->DrawFocusRect(&rTextClip);
   }

   // �ָ�ԭ�л���
   pDC->SelectObject(penOld);
}*/



void CMyListCtrl::DeleteAllColumns()
{	
	for(int i=get_ColumnCount()-1;i>=0;i--)
	{
		DeleteColumn(i);
	}
}

long CMyListCtrl::get_ColumnCount()
{
	return  GetHeaderCtrl()->GetItemCount();
}
