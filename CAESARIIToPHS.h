// CAESARIIToPHS.h: interface for the CAESARIIToPHS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAESARIITOPHS_H__6113F5D2_DC33_41DC_8068_C064AD3D7E69__INCLUDED_)
#define AFX_CAESARIITOPHS_H__6113F5D2_DC33_41DC_8068_C064AD3D7E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAESARIIToPHS  
{
public:
	CAESARIIToPHS();
	virtual ~CAESARIIToPHS();
	void  ReadResult_CAESARII45(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID,char * strPSAVER);
private:
	void ImportHangerRestraints(_RecordsetPtr &rsData,_RecordsetPtr &rsHR,long &ltemp,long &FileNameID,char *strPSAVER,long &node,int flag);//pfg20050615
	void importUnitsForces(_RecordsetPtr &rsData,long node,CString m_strJOBNAME_P,CString PSA_OutDataFUnit,CString PSA_OutDataMUnit,CString m_strRGKname,CString m_strLGKname);//pfg20050622
	void ImportDisplacements(_RecordsetPtr rsData,long node, CString strLGKname,CString strJOBNAME_P,int flag);
	void ImportDiameter(_RecordsetPtr &rsData,long node,CString strJOBNAME_A);
	//���뵯������
	void ImportHanger(_RecordsetPtr &rsData,long node,CString strJOBNAME_P);
	void ConversionTypeCaesarToPhs(_RecordsetPtr &rsData,long node,CString strJOBNAME_P,CString strGKname,long &iType);
	//��λ����
	double UnitsToUeUnits(_RecordsetPtr rsData,CString SourceUnits,CString UeUnits,double &dOffset);

	BOOL IsHanger( int iNode,const CString& strFileName  ); //�Ƿ��ǵ���(����������)
	double GetMaxLoad( int iNode,const CString& strFileName ); //�õ�֧�������ĺ���ֵ
	BOOL GetHangerLoad( int iNode,const CString& strFileName, double& dWorkLoad,double& dInsLoad ); //�õ����ɵĹ������غͰ�װ����

	BOOL GetHangerSeriesNum(const CString& strHangerType,int& iRtSeriesNum); //���ݵ������͵õ����ɴ�����

	_ConnectionPtr m_pConnSourceDB;			   //Դ���ݿ�����Ӷ���
	BOOL m_IsCaesarError; 
	CString m_strUNITFORCE;//�ڲ�ʹ��N��λ
	CString m_strUNITMOMENTAL;//�ڲ�ʹ��N.m��λ
//	BOOL m_bCheckRigPHSMaxLoad; //����֧�����Ƿ���������ֵ������ȡ����
	int m_iSpringLoadType; //���ɺ�����ȡ������ʽ(0:������ѡ�����/��̬��ȡ����ֵ 1��CAESARII���ɱ��еĺ���ֵ��ȡ)
	int m_iUPxyz ; //������(1:X 2:Y 3:Z)
};

#endif // !defined(AFX_CAESARIITOPHS_H__6113F5D2_DC33_41DC_8068_C064AD3D7E69__INCLUDED_)
