// PictureClipData.h: interface for the PictureClipData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURECLIPDATA_H__E6FB53EF_1EEA_46E8_BF8E_1E8EE76DC45B__INCLUDED_)
#define AFX_PICTURECLIPDATA_H__E6FB53EF_1EEA_46E8_BF8E_1E8EE76DC45B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ConstantSpringDataBase.h"

class PictureClipData : public ConstantSpringDataBase
{
public:
	PictureClipData();
	virtual ~PictureClipData();
public:
	CString GetID();
	CString GetBHFormat();
	CString GetDescription();
public:
	
	const CString& GetTableName()
	{
		return m_strTableName;
	}
	const CString& GetCustomIDField()
	{
		return m_strCustomIDField;
	}
private:
	static CString m_strTableName;
	static CString m_strCustomIDField; //�ֶ���
	static CString m_strBHFormatField;	//�ֶ���
	static CString m_strDescriptionField;
	static CString m_strIDField; //ID��
};

#endif // !defined(AFX_PICTURECLIPDATA_H__E6FB53EF_1EEA_46E8_BF8E_1E8EE76DC45B__INCLUDED_)
