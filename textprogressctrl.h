#if !defined(AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_)
#define AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// TextProgressCtrl.h : header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright 1998.
//
// Modified : 26/05/98 Jeremy Davis, jmd@jvf.co.uk
//				Added colour routines
//
// TextProgressCtrl is a drop-in replacement for the standard 
// CProgressCtrl that displays text in a progress control.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is not sold for
// profit without the authors written consent, and providing that this
// notice and the authors name is included. If the source code in 
// this file is used in any commercial application then an email to
// the me would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl window

class CTextProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CTextProgressCtrl();

// Attributes
public:

// Operations
public:
    int			SetPos(int nPos);//����CTextProgressCtrl��m_nBarwith��ֵ,������һ�ε�m_nPos 
    int			StepIt();//���ص���SetPos�ķ���ֵ
    void		SetRange(int nLower, int nUpper);//���������Сֵ
    int			OffsetPos(int nPos);//���ص���SetPos�ķ���ֵ
    int			SetStep(int nStep);//����m_nStepSize��ֵ��������nSetp��ֵ
	void		SetForeColour(COLORREF col);//����ǰ��ɫ����m_colFore��ֵ
	void		SetBkColour(COLORREF col);//���ñ���ɫ����m_colBk��ֵ
	void		SetTextForeColour(COLORREF col);//�����ı�ǰ��ɫ����m_colTextFore��ֵ
	void		SetTextBkColour(COLORREF col);//�����ı�����ɫ����m_colTextBk��ֵ
	COLORREF	GetForeColour();//ȡ��ǰ��ɫ
	COLORREF	GetBkColour();//ȡ�ñ���ɫ
	COLORREF	GetTextForeColour();//ȡ���ı�ǰ��ɫ
	COLORREF	GetTextBkColour();//ȡ���ı�����ɫ

    void		SetShowText(BOOL bShow);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextProgressCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetText(LPCTSTR lpszText);
	void SetTextFont(LPCTSTR lpszFontName,int nHeight,BOOL bBold=FALSE);//�����ı�������
	virtual ~CTextProgressCtrl();

	// Generated message map functions
protected:
	CFont m_TextFont;
    int			m_nPos, 
				m_nStepSize, 
				m_nMax, 
				m_nMin;
    CString		m_strText;
    BOOL		m_bShowText;
    int			m_nBarWidth;
	COLORREF	m_colFore,
				m_colBk,
				m_colTextFore,
				m_colTextBk;

	//{{AFX_MSG(CTextProgressCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    afx_msg LRESULT OnSetText(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnGetText(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_)
