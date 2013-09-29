// OutputPDSFileTr.h: interface for the OutputPDSFileTr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTPDSFILETR_H__9F0E9F0C_2D8B_49F1_B2D7_D5E9058B493B__INCLUDED_)
#define AFX_OUTPUTPDSFILETR_H__9F0E9F0C_2D8B_49F1_B2D7_D5E9058B493B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OutputPDSFileTr  
{
public:
	OutputPDSFileTr();
	virtual ~OutputPDSFileTr();
	//���֧���ܵ����ݵ�PDS�ı��ļ���ʽ   mPHSNumberArray:Ҫ���֧���ܵı��
	void OutPutText(int iVolumeID, CArray<int,int>& mPHSNumberArray,CArray<int,int>& mGNumArray,const CString& strFileName );
private:
	int GetParalleledNum( const CString& strID ); //�õ�֧���ܵĲ����� 
	//�õ�֧�������� ����ֵ strType 1:��֧�� 2:����  dPB:�ܲ����  dRB:�������
	BOOL GetZDJInfo( int iVolumeID,int iPHSNumber,CString& strType,double& dPB,double& dRB,double& dDW ); 
	CString GetCustomIDOutText( int iVolumeID,int iPHSNumber,const CString& strBH, const CString& strCustomID,_RecordsetPtr pZBRs ); //�õ����벿��������ı�
	CString GetValue( int iVolumeID,int iPHSNumber,const CString& strBH,const CString& strTableName,const CString& strFieldName,const CString& strmdbName,_RecordsetPtr pZBRs );

//	CString GetStrandStr( const CString& strCustomID); //�õ����ݿ��׼�ַ�
	
	CString m_strBH;
	CString m_strTableName;
	CString m_strFieldName;
	CString m_strMdbName;
	_RecordsetPtr m_pRs;
};

#endif // !defined(AFX_OUTPUTPDSFILETR_H__9F0E9F0C_2D8B_49F1_B2D7_D5E9058B493B__INCLUDED_)
