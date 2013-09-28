// ExportPaint.h: interface for the CExportPaint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPORTPAINT_H__0A13B517_585A_4898_924C_F7E01645428A__INCLUDED_)
#define AFX_EXPORTPAINT_H__0A13B517_585A_4898_924C_F7E01645428A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExportPaint  
{
public:
	bool StartupAutoIPED();

    CString m_strDbPath, //��Ŀ���ݿ�·��
			m_strWorkPath,//����·��
			m_strInsPath;//����·��

	CString m_strVlmCODE;  //������
	CString m_strPrjID;		//���̴���

	BOOL SelEngVol();
	CExportPaint();
	virtual ~CExportPaint();
	////�õ�IPEDע�����Ϣ
	BOOL GetIPEDPath();

protected:
	void SetCodePath(CString& strPath);
};

#endif // !defined(AFX_EXPORTPAINT_H__0A13B517_585A_4898_924C_F7E01645428A__INCLUDED_)
