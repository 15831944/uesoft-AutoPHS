// iSAhSel.h: interface for the CiSAhSel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISAHSEL_H__3D630240_6328_4B74_9026_92220E97E468__INCLUDED_)
#define AFX_ISAHSEL_H__3D630240_6328_4B74_9026_92220E97E468__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//iSAh,ֱ�ӵ�,0

class CiSAhSel  
{
public:
	CString GetiSAhSQL(CString &Ptype);
	CiSAhSel();
	virtual ~CiSAhSel();
protected:
	CString m_tmpSQL ;//Cphs.cpp(2496)tmpSQL��ǰ������ƥ���ѯ�ֶ���Ϣ
	CString m_SAfDPmax;//Cphs.cpp(2519)SAfDPmax
	float m_tmpSelPJG ;//Cphs.cpp(2459)tmpSelPJG�ۼ����������ļ������
	int	m_iNumPart;//Cphs.h(93)iNumPart

};

#endif // !defined(AFX_ISAHSEL_H__3D630240_6328_4B74_9026_92220E97E468__INCLUDED_)
