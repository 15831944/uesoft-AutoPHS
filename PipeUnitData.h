// PipeUnitData1.h: interface for the CPipeUnitData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIPEUNITDATA1_H__7EF2C07D_7609_4A8C_A1B5_962F3BE33462__INCLUDED_)
#define AFX_PIPEUNITDATA1_H__7EF2C07D_7609_4A8C_A1B5_962F3BE33462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OtherStruct.h"

//��Ԫ����������
class AFX_EXT_CLS CPipeUnitFeature
{
public:
	CPipeUnitFeature();
	virtual ~CPipeUnitFeature(){};
	CAverageForce m_AverageForce;//������
	COffSetsData m_OffSetsData;//ƫ������
	MediumFeature m_MediumFeature;//��������
	CWind m_Wind; //��
	PipeFeature m_PipeFeature; //��Ԫ����
	MaterialFeature m_MaterialFeature; //��������
	SoilModels m_SoilModels;//�������ģ��
	ProjectionLength m_ProjectionLength;//ͶӰ��
};

class AFX_EXT_CLS CPipeNodeFeature
{
public:
	CPipeNodeFeature(){};
	virtual ~CPipeNodeFeature(){};
	CHanger m_Hanger; //����
	RestrantSel m_RestrantSel; //Լ��
	Displacement m_Displacement;//λ��
	CForcmnt m_Forcmnt; //��������
	ThreeOrSifsOpen m_ThreeOrSifsOpen;//��ͨ��Ӧ����ǿ
	
	int m_nNodeNumTo; //�յ�
	int m_nNodeNumFrom; //���
	CNodeName m_NodeName; //�ڵ�����
};

class AFX_EXT_CLS CPipeUnitData : public CPipeUnitFeature,public CPipeNodeFeature
{
public:
	CPipeUnitData();
	virtual ~CPipeUnitData();
	BOOL m_bModified;//�б�Ԫ��������һ�α��������Ƿ�ı��
	Coordinates m_Coordinate1; //�������(��������,�������ʱ!=�����ܵ��ϵ�����)
	Coordinates m_Coordinate2; //ĩ������(��������)
	Coordinates m_TrueCoordinate1; //���ڹܵ��ϵ���ʵ����
	Coordinates m_TrueCoordinate2; //���ڹܵ��ϵ���ʵ����
	
	Coordinates m_ResultResult1; //�ܵ�����λ��ʱ������(��ʵ����)
	Coordinates m_ResultResult2; //�ܵ�����λ��ʱ������(��ʵ����)
	
	AssistPosition m_AssistPosition;//��Ԫ�ĸ������ݵ�λ��
	StraightUnit m_StraightUnit; //ֱ��Ԫ
	RigidUnit m_RigidUnit;//���Լ�
	ReducerUnit m_ReducerUnit;//��Сͷ
	
	NoPipeStrongUnit m_NoPipeStrongUnit;//������
	
	int m_nUnitType;//��Ԫ���� 0:ֱ��Ԫ; 1:���Լ� 2:��Сͷ 3:������)
	BYTE m_bNOCOLOR;
	
	double GetSumWeight();
	double GetBendWeight();
	double GetStraightNoBendWeight();
	HighDisplay m_BeginHighData;//��������
	HighDisplay m_EndHighData;
	HighDisplay m_CenterHighDataB;
	HighDisplay m_CenterHighDataE;
	BOOL m_bHigh;//�����߼�

	//PDMS->CII�ӿ����
	CString m_strFromPDMSCompName; //���Ķ˿ڵ�PDMS�Ĺܼ�����
	CString m_strToPDMSCompName; //����ĩ�˵�PDMS�Ĺܼ�����
	int m_iNodeFromPort; //���Ķ˿ں�
	int m_iNodeToPort; //ĩ��Ķ˿ں�
};


class AFX_EXT_CLS CPipeGlobalData
{
public:
	CPipeGlobalData();
	virtual ~CPipeGlobalData();
	CUnits m_Units;
	CUnitsScale m_UnitsScale;
	OutUnitsScale m_OutUnitsScale; //��������ļ�������ļ��ı���
	int m_ncheckInUnit;
	int m_ncheckOutUnit;
	int m_iICC;//������(0:X;1:Y;2:Z)
	Coordinates m_OriginalCoordinate; //ԭ������
	double	m_dbDesignConditionEnvironmentTemp; //�����¶�
	int m_dbRigidityType; //֧���ܸնȷ�ʽ
	int m_dbRigidityTimes;//���Լ��ն�Ϊֱ�ܸնȵı���
	double m_dbOccLoadk;//żȻ����ϵ��
	BOOL m_bSoid; //����ʱ�Ƿ�����������
	
	CString m_strGversion; //Ӧ�ó������汾
	CString m_strRversion; //Ӧ�ó��򸱰汾
	
	CInitHangerAttribute m_InitHangerAtt; //��������Ĭ��ֵ
	
	BOOL m_bStrongWeight;//����ʱ�Ƿ��ǲ����������� 2007.05.24 Modified by lyf
	BOOL m_bLoadWeightratio;//����ʱ�Ƿ�ʹ�ú��ر�g's:Ĭ��FALSE(��ʹ��);TRUEΪʹ��
	BOOL m_bAutoSettingDef;//�Ƿ��Զ�����Ĭ��ֵ:Ĭ��TRUEΪ�Զ�;FALSE(���Զ�)

	BOOL m_bIsInsWeight; //�����Ƿ񵼳�Ϊ����,CII5.3���õ�,��PDMS��AutoPDMS����CIIʱ�õ�
};


#endif // !defined(AFX_PIPEUNITDATA1_H__7EF2C07D_7609_4A8C_A1B5_962F3BE33462__INCLUDED_)
