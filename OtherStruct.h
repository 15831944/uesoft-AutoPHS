//#include "StdAfx.h"
#ifndef OTHERSTRUCT_H
#define OTHERSTRUCT_H
//����
#ifdef _AFXEXTCLS
#define AFX_EXT_CLS       AFX_CLASS_EXPORT
#else
#define AFX_EXT_CLS       AFX_CLASS_IMPORT
#endif
#include <math.h>
class AFX_EXT_CLS Coordinates
{
public:
	Coordinates();
	Coordinates(double x0, double y0, double z0){X=x0; Y=y0; Z=z0;}
	virtual ~Coordinates(){};
	BOOL operator==(const Coordinates& unit2);
	double X;
	double Y;
	double Z;
};

class AFX_EXT_CLS CBendData 
{
public:
	int m_SetMidAngleFlag;//�е�Ƕȱ�־��־:1��ʾĬ���е�Ƕ�Ϊ�Ƕ�1;2��ʾ�Ƕ�2;3��ʾ�Ƕ�3;0��ʾ�����е�Ƕȱ�־
	CBendData();
	virtual ~CBendData(){};
	int m_nBendUnitType; //(1:��ѹ;2:����;3:����)
	double m_dbBendRadius;
	void operator=(CBendData unit2);
	double m_dbAngle;
	Coordinates m_CenterPoint;//���ĵ�����
	Coordinates m_StartPoint;
	Coordinates m_EndPoint;
	
	Coordinates m_ResultCenterPoint; //�ܵ�����λ��ʱ������(���ĵ�����)
	Coordinates m_ResultStartPoint; //�ܵ�����λ��ʱ������(�䵥Ԫ�����ʵ����)
	Coordinates m_ResultEndPoint; //�ܵ�����λ��ʱ������(�䵥Ԫĩ����ʵ����)
	double m_dbResultAngle; //���й�������λ�ƺ���ܽǶ�
	double m_dbResultBendRadius; //���й�������λ�ƺ���ܰ뾶
	
	double m_dbAngle1; //�ڵ�λ��#1�ĽǶ�
	Coordinates m_Point1;//�ڵ�λ��#1������
	int m_dbNode1; //�ڵ�1��
	double m_dbAngle2; //�ڵ�λ��#2�ĽǶ�
	Coordinates m_Point2;//�ڵ�λ��#2������
	int m_dbNode2; //�ڵ�2��
	double m_dbAngle3; //�ڵ�λ��#3�ĽǶ�
	Coordinates m_Point3;//�ڵ�λ��#3������
	int m_dbNode3; //�ڵ�3��
	int m_iMiterPoints; //б���и���
	double m_dbFactor; //����ϵ��
	double m_dbPipeWallThickness; //��ͷ�ıں�(��ֱ�ܲ�ͬʱ������)
	
};

//��
class AFX_EXT_CLS CWind  
{
public:
	CWind();
	virtual ~CWind(){};
	BOOL m_bWindTag;
	double	m_dbBasicWindPressure;//������ѹ
	double	m_dbEWHorizontalLoadDirectionCoefX;
	double	m_dbEWHorizontalLoadDirectionCoefY;
	double	m_dbEWHorizontalLoadDirectionCoefZ;
	double	m_dbPipeToWindDiameter;//ӭ��ֱ��
	double	m_dbWindLoadFigureCoef;//���������ϵ��
	double  m_dbWindPressureChangeCoef; //��ѹ�仯ϵ��
	
};
//�ڵ�����
class AFX_EXT_CLS CNodeName
{
public:
	CNodeName();
	virtual ~CNodeName(){};
	BOOL m_bNodeNameTag;
	CString m_strFromName;
	CString m_strToName;
};
//Լ���ṹ
class AFX_EXT_CLS RestrantData
{
public:
	RestrantData();
	virtual~RestrantData(){};
	int m_iNode;
	int m_iCNode;
	int m_iType;		//Լ������:1��ʾȫ����Լ��;2,3,4�ֱ��ʾx,y,z�����������λ��;5,6,7���ʾ����������Խ�λ��
	double m_dbStif; //Լ���ն�
	double m_dbGap; //Լ�����
	double m_dbMu; //Լ��Ħ��ϵ��
	double m_dbCosX;
	double m_dbCosY;
	double m_dbCosZ;
	int m_iBitTag;
};
//Լ��ѡ��
class AFX_EXT_CLS RestrantSel
{
public:
	RestrantSel();
	virtual ~RestrantSel(){};
	RestrantData m_Restrant[4];
	BOOL m_bRestrantTag; 
	
};
//��Ԫ�ĸ������ݵı�־
class AFX_EXT_CLS AssistPosition
{
public:
	AssistPosition();
	virtual ~AssistPosition(){};
	int m_iBend; //�䵥Ԫ
	int m_iRigit; //���Լ�
	int m_iExpjt; //������
	int m_iRestrant; //Լ��
	int m_iDisp; //λ��
	int m_iForcmnt; //��������
	int m_iUniform; //��������
	int m_iWind; //�����
	int m_iOffsets; //��Ԫƫ��
	int m_iAllowbls; //����Ӧ��
	int m_iSifTees; //��ͨ
	int m_iNodeNume; //�ڵ�����
	int m_iReducer; //�쾶��
	int m_iSoid; //��������
};

//�������صĽṹ
////////////////////////////////////////
class AFX_EXT_CLS CForcmntData
{
public:
	CForcmntData();
	virtual ~CForcmntData(){};
	double m_dbFX;
	double m_dbFY;
	double m_dbFZ;
	double m_dbMX;
	double m_dbMY;
	double m_dbMZ;
};
class AFX_EXT_CLS CForcmntNodeData
{
public:
	CForcmntNodeData();
	virtual ~CForcmntNodeData(){};
	int m_ForcmntNodeNumer;
	CForcmntData m_ForcmntData[11];//��11�ֹ���(GLif����10�ֺ�һ��żȻ����)
};
class AFX_EXT_CLS CForcmnt
{
public:
	CForcmnt();
	virtual ~CForcmnt(){};
	BOOL m_bForcmnt;
	CForcmntNodeData m_ForcmntNodeData[2];
};
//////////////////////////////////////////////////
class AFX_EXT_CLS COffSetsData //��Ԫƫ������
{
public:
	COffSetsData();
	virtual ~COffSetsData(){};
	bool m_bOffsets;
	double m_dbFromX;
	double m_dbFromY;
	double m_dbFromZ;
	double m_dbToX;
	double m_dbToY;
	double m_dbToZ;
};

/////////////////////////////////////////////////

//////////////////////λ�ƽṹ
class AFX_EXT_CLS CDispData
{
public:
	CDispData();
	virtual ~CDispData(){};
	double m_dbLX;
	double m_dbLY;
	double m_dbLZ;
	double m_dbRX;
	double m_dbRY;
	double m_dbRZ;
};
class AFX_EXT_CLS CDispNodeData
{
public:
	CDispNodeData();
	virtual ~CDispNodeData(){};
	int	m_nGivenDisplacementNodeNum;
	CDispData m_DispData[9]; //���ֻ��9�ֹ���
};

class AFX_EXT_CLS Displacement  
{
public:
	Displacement();
	virtual ~Displacement(){};
	BOOL m_bGivenDisplacement;
	CDispNodeData m_DispNodeData[2]; //ÿ����Ԫ���������������λ��
};
//////////////////////////////////////////////////////
//������
class AFX_EXT_CLS CAverageForceData
{
public:
	CAverageForceData();
	virtual ~CAverageForceData(){};
	double m_dbX;
	double m_dbY;
	double m_dbZ;
};

class AFX_EXT_CLS CAverageForce  
{
public:
	CAverageForce();
	virtual ~CAverageForce(){};
	BOOL m_bAverageForce;
	CAverageForceData m_AverageForceData[3];
};
//////////////////////////////////////////////////
class AFX_EXT_CLS CHanger
{
public:
	CHanger();
	virtual ~CHanger(){};
	BOOL m_bHangerTag; //�Ƿ��е���
	int m_iHangerNodeNumer;
	int m_iInitWork; // ��/��̬����
	int m_iMidLineSel;//�Ƿ�������ѡ�񷽷�
	double m_dbHangerRig;//���ɸն�
	double m_dbHangerVarSitf;//����仯ϵ��
	double m_dbDispCriteria;//����֧���ܵ�λ�Ʊ�׼
	double m_dbAvSpace;//��������ռ�
	double m_dbInitLoad;//�����
	double m_dbWorkLoad;//�Ⱥ���
	double m_dbDefineLoad;//�û��������й���f/��������
	double m_dbAllowLimit;//��������г̼���
	double m_iCaseSel;//����ѡ��
	double m_dbConstantHanger; //���ɺ���֧������
	int m_iFreeResNode1; //Ҫ���ɵĹ̶��ڵ�1
	int m_iFreeResNode2; //Ҫ���ɵĹ̶��ڵ�2
	int m_iFreeCode1;//���ɵķ���
	int m_iFreeCode2;
	int m_iHangerNum;//���ɲ�����
	int m_iHangerTableID;//���ɱ��
	int m_iHangerShort;//�Ƿ�����ʹ�ö̵���
	int m_iHangerCNode;//���ӵ��
};

//////////////////////////////////////////////////
//Ĭ�ϵ�����������
class AFX_EXT_CLS CInitHangerAttribute
{
public:
	CInitHangerAttribute();
	virtual ~CInitHangerAttribute(){};
	int m_iInitHangerTableID;//���ɱ��
	double m_dbInitHangerVarSitf;//����仯ϵ��
	double m_dbInitDispCriteria;//����֧���ܵ�λ�Ʊ�׼
	double m_dbInitAllowLimit;//��������г̼���
	int m_iInitHangerShort;//�Ƿ�����ö̵���
	int m_iInitDefMul; //��Ĭ�϶���ع������ѡ��.
	int m_iInitIdfoper; //��Ĭ�ϵ���������й�����
	int m_iInitIactcld;  //��Ĭ������ؼ��㿪��
	int m_iInitIhgrlds; //�ǵ������к�����
	int m_iInitIactual; //�Ƕ���ʵ������صĹ���.
	int m_iInitImultiopts;//�Ƕ๤�����ѡ��
};

//��������
class AFX_EXT_CLS MaterialFeature  
{
public:
	MaterialFeature();
	virtual ~MaterialFeature(){};
	
	BOOL m_bMaterialFeatureTag;
	double	m_dbBendLimitIn20Degree; //�����¶��µ���������
	double	m_dbElasticModulusIn20Degree;//�����¶��µ���ģ��ֵ
	double	m_dbAllowStressIn20Degree;//�����¶�������Ӧ��ֵ
	double m_dbEff;//���򺸽�ϵ��
	double m_dbSy;
	double m_dbFac;
	double m_dbPmax;
	
	//AutoPSA7.0�еĲ���
	double m_dbOperatingCycles[9];//�ܵ��������
	double m_dbMaterialAllowStressValue[9]; //	����Ӧ��ֵ
	double m_dbMaterialLinearExpansionCoefValue[9]; //����ϵ��ֵ
	double m_dbMaterialElasticModulusValue[9]; //����ģ��ֵ
	double m_dbBendLimitInWaterValue[9]; //��������
	void operator =(MaterialFeature unit2);
	
};

//��������
class AFX_EXT_CLS MediumFeature  
{
public:
	MediumFeature();
	virtual ~MediumFeature(){};
	BOOL m_bMediumFeatureTag; //AutoPSA7.0 �еĹ����¶�,ѹ����־
	double m_dbWaterTestConditionPressure1;//ˮѹǿ������ѹ����Һѹ����ѹ��
	double m_dbWaterTestConditionPressure2;//ˮѹ����������ѹ������ѹ����ѹ��
	double m_dbWorkTemp[9]; //AuotPSA7.0�еĹ����¶�,9�ֿ������
	double m_dbWorkPressure[9]; //AuotPSA7.0�еĹ���ѹ��,9�ֿ������
	void operator=(MediumFeature unit2);
	MediumFeature operator / (double dbValue);
	MediumFeature operator + (const MediumFeature& unit2);
};

//������
class AFX_EXT_CLS NoPipeStrongUnit  
{
public:
	NoPipeStrongUnit();
	virtual ~NoPipeStrongUnit(){};
	double	m_dbNoPipeStrongUnitOverallWeight;
	//cbl 2005.11.15
	double  m_dbAxialStif;  //����ն�
	double  m_dbTransStif;  //����ն�
	double  m_dbBendingStif; //�����ն�
	double  m_dbTorsionStif;  //Ťת�ն�
	double  m_dbArea; //��Ч���
	// luow  2006.8.24
	double  m_dbStrongFK[9];  //��ͬ�����¶���,�նȵ�����ϵ��
};

//ͶӰ��
class AFX_EXT_CLS ProjectionLength  
{
public:
	ProjectionLength();
	virtual ~ProjectionLength(){};
	double m_dbProjectionLengthZ;
	double m_dbProjectionLengthY;
	double m_dbProjectionLengthX;
public:
	double GetProjectionLength()
	{
		double temp = m_dbProjectionLengthX*m_dbProjectionLengthX + m_dbProjectionLengthY*m_dbProjectionLengthY + m_dbProjectionLengthZ*m_dbProjectionLengthZ ;
		return sqrt( temp )  ;

	}
	
};

//��Сͷ
class AFX_EXT_CLS ReducerUnit  
{
public:
	ReducerUnit();
	virtual ~ReducerUnit(){};
	double m_dbDiameter2;
	double m_dbThickness2;
	double m_dbReduceAngle;
	double m_dbR1;
	double m_dbR2;
	double m_dDn; //ֱ�� PDMS->PSA�ӿ�����
};

//���Լ�
class AFX_EXT_CLS RigidUnit  
{
public:
	RigidUnit();
	virtual ~RigidUnit(){};
	double	m_dbRigidUnitOverallWeight;
	double m_dbRigidUnitLinearExpansionCoef;
	
};
//��������
class AFX_EXT_CLS SectionFeature  
{
public:
	SectionFeature();
	virtual ~SectionFeature(){};
	double m_dbPipeOuterDiameter;
	double m_dbPipeWallThickness;
	
};

class AFX_EXT_CLS SoilModels
{
public:
	SoilModels();
	virtual ~SoilModels(){};
	BOOL m_bSoilModelsTag;		// ��ص�Ԫ�������ñ�־, Ϊ"TRUE"��ʾ�Ѿ����û�����
	double m_dbSu;//Ħ��ϵ��
	double m_dbDensity;//�ܶ�
	double m_dbDepth;//������
	double m_dbFrictAngle; //Ħ����
	double m_dbShearStrength;//����ˮ����ǿ��
	double m_dbOverburden;//����ѹʵ����
	double m_dbYieldDispFactor;//����λ��ϵ��
	double m_dbExpansionCoef; //�¶�����ϵ��
	double m_dbTemperatureChange;//�¶ȱ仯
	
};
/////////////////////////////////////////////////

class AFX_EXT_CLS CUnits
{
public:
	CUnits();
	virtual ~CUnits(){}
	CString m_strUnitName; //��λ����
	CString m_strNom;//"��" �� "��" ���Ҹ���PREPIP �Ƿ�������ֱ��
	CString m_strLenTip;//����
	CString m_strForTip;//��
	CString m_strWeightTip; //����
	CString m_strMomentTip; //��������
	CString m_strOMomentTip; //�������
	CString m_strStressTip; //Ӧ��
	CString m_strScTip; //�¶�
	CString m_strLineDispTip; //λ��
	CString m_strPreTip; //ѹ��
	CString m_strEModulusTip;//����ģ��
	CString m_strExpansionCoefTip;//����ϵ��
	CString m_strPipeDensityTip; //�ܵ��ܶ�
	CString m_strInsulationDensityTip; //�����ܶ�
	CString m_strFluidDensityTip;//�����ܶ�
	CString m_strTSFTip;//ƽ���ն�
	CString m_strRSFTip;//ת���ն�
	CString m_strAverageForceTip;//��������
	CString m_strLoadWeightRatioTip; //��������ʹ�ú��ر�g's��ǩ
	CString m_strForceTip;//��������
	CString m_strWindForceTip; //�����
	CString m_strEleTip;//���
	CString m_strCLenTip;//�ϳɳ���
	CString m_strDiaTip;//ֱ��
	CString m_strThkTip;//�ں�
	CString m_strAangleTip; //�Ƕ� 
	CString m_strAangleDispTip; //��λ��
	CString m_strMasTip;//����
	CString m_strArea; //��������Ч���
};

class AFX_EXT_CLS CFastUnitsScale
{
public:
	CFastUnitsScale();
	virtual ~CFastUnitsScale(){}
	static double m_dbCmpCoefDia; //����Ӧ����ǿϵ�������⾶��λ֮�����(�ڴ��׼m->mm)
	static double m_dbWriteCmpAngle; //д�������ʱ�ļ�����"�Ƕ�"��λ(rad)
	
	static double m_dbMaterialMdbWallThickness; //�������ݿ��еĵ�λ���ڴ浥λ�ı���(�ں�mm->m) 
	static double m_dbMaterialMdbBendLimitIn; //�������ݿ��еĵ�λ���ڴ浥λ�ı���(��������MPA->pa)
	static double m_dbMaterialMdbStress;//�������ݿ��еĵ�λ���ڴ浥λ�ı���(Ӧ��MPA->pa)
	static double m_dbMaterialMdbElasticModulus; //����ģ��(KN/mm2->N/m2)
	static double m_dbMaterialMdbALPHAt; //����ϵ��
	
	static double m_dbSoidMdbShearStrength; //������ݿⵥλ���ڴ浥λ����
	
	static double m_dbAX; //���������ݿ�����նȵ��ڴ浥λ����
	static double m_dbTorS; //���������ݿ�Ťת�նȵ��ڴ浥λ����
	static double m_dbTraS; //���������ݿ����նȵ��ڴ浥λ����
	static double m_dbBendS; //���������ݿ������նȵ��ڴ浥λ����
	static double m_dbArea; //���������ݿ���Ч������ڴ浥λ����
};

class AFX_EXT_CLS CUnitsScale
{
public:
	CUnitsScale();
	virtual ~CUnitsScale(){};
	double m_dbLen; //��׼��λ����ǰ��λ�ĳ��ȱ��� (M-->��ǰ)
	double m_dbFor; //��׼��λ����ǰ��λ�������� (N-->��ǰ)
	double m_dbWeight; //��׼��λ����ǰ��λ���������� (KG-->��ǰ)
	double m_dbMoment; //��׼��λ����ǰ��λ�����ر��� (N/M-->��ǰ)
	double m_dbStress; //��׼��λ����ǰ��λӦ������ (Pa-->��ǰ)
	double m_dbSc;     //��׼��λ����ǰ��λ�¶ȱ��� (C-->��ǰ)
	double m_dbScDev;  //��׼��λ����ǰ��λ�¶�ƫ�� (C-->��ǰ)
	double m_dbNewScDev;
	double m_dbLineDisp; //��׼��λ����ǰ��λλ�Ʊ��� (M-->��ǰ)
	double m_dbPre;      //��׼��λ����ǰ��λѹ������ (pa-->��ǰ)
	double m_dbEModulus; //��׼��λ����ǰ��λ����ģ������ (pa(N/m2) -->��ǰ)
	double m_dbExpansionCoef; //��׼��λ����ǰ����ϵ������ (1E-6/C -->��ǰ)
	double m_dbPipeDensity;   //��׼��λ����ǰ�ܵ��ܶȱ��� (kg/m3 -->��ǰ)
	double m_dbInsulationDensity; //��׼��λ����ǰ�����ܶȱ��� (kg/m3 -->��ǰ)
	double m_dbFluidDensity; //��׼��λ����ǰ�����ܶȱ��� (kg/m3 -->��ǰ)
	double m_dbTSF;  //��׼��λ����ǰƽ���նȱ��� (N/M -->��ǰ)
	double m_dbRSF; //��׼��λ����ǰת���նȱ��� (N.M/Rad-->��ǰ)
	double m_dbAverageForce; //��׼��λ����ǰ�������ر��� (N/M-->��ǰ)
	double m_dbLodeWeightRatio;//��׼��λ����ǰ�������ر���(g's�������ʹ��,��ֵΪ1.0)
	double m_dbForce; //��׼��λ����ǰ���ر��� (N-->��ǰ)
	double m_dbWindForce; //��׼��λ����ǰ��ѹ���� (Pa-->��ǰ)
	double m_dbEle; //��׼��λ����ǰ��߱��� (M-->��ǰ)
	double m_dbCLen; //��׼��λ����ǰ�ϳɳ��ȱ��� (M-->��ǰ)
	double m_dbDia; //��׼��λ����ǰֱ������ (M-->��ǰ)
	double m_dbThk; //��׼��λ����ǰ�ں���� (M-->��ǰ)
	double m_dbAngle; //��׼��λ����ǰ�Ƕȱ��� (��-->��ǰ)
	double m_dbAngleDisp; //��׼��λ����ǰ��λ�Ʊ��� (rad-->��ǰ)
	double m_dbArea; //��׼��λ����ǰ������������� (M2-->��ǰ)
};

class AFX_EXT_CLS OutUnitsScale
{
public:
	OutUnitsScale();
	virtual ~OutUnitsScale(){};
	double m_dbWritePHSFor;//д��PHS����"��"��λ(N)������ļ��ı���
	double m_dbWritePHSMoment;//д��PHS����"����"��λ(N.M)������ļ��ı���
	double m_dbWritePHSDisp; //д��PHS����"��λ��"��λ(M)������ļ��ı���
	double m_dbWritePHSAngleDisp; //д��PHS����"��λ��"��λ(RAD)������ļ��ı���
};

/////////////// PSA1.1��������֤�����֤������
class AFX_EXT_CLS OldNodeType
{
public:
	OldNodeType();
	virtual ~OldNodeType(){};
	int m_iNodeNumer;
	int m_iNodeType;
};

////////////// 7.0�е�Լ������ת��1.1�е�������
class AFX_EXT_CLS ResTypeWay
{
public:
	ResTypeWay();
	virtual ~ResTypeWay(){};
	int m_iNodeNumer;
	bool m_bResX;
	bool m_bResY;
	bool m_bResZ;
	bool m_bResRX;
	bool m_bResRY;
	bool m_bResRZ;
	int m_iNodeType;
	int m_iBitTag;  //��ȡGLif�ļ���λ��־( 1:��������ϵ  2:��Ԫ����ϵ 3:�ڹ涨����������)

};

class AFX_EXT_CLS OldDisplacement // PSA1.1�е�λ�ƽṹ(����,��,����,����λ��)
{
public:
	OldDisplacement();
	virtual ~OldDisplacement(){};
	int m_nDisplacementNodeNum;
	double m_dbAngleDisplacementX;
	double m_dbAngleDisplacementY;
	double m_dbAngleDisplacementZ;
	double m_dbLinearDisplacementX;
	double m_dbLinearDisplacementY;
	double m_dbLinearDisplacementZ;
};

//�����,����PSA1.1�����õ�����
class AFX_EXT_CLS OldColdTightenToMouth 
{
public:
	OldColdTightenToMouth();
	virtual ~OldColdTightenToMouth(){};
	double	m_dbBMColdTightenValue;
	double m_dbX;
	double m_dbY;
	double m_dbZ;
	int	m_nColdTightenNodeNum;
};

//����Ӧ����ǿϵ��
class AFX_EXT_CLS OldStressStrongthenCoef
{
public:
	OldStressStrongthenCoef();
	virtual ~OldStressStrongthenCoef(){};
	int m_nStressCoefNodeNum;
	double m_dbStrongthenCoef;
};

//����PSA1.1���������õ����ݽṹ
class AFX_EXT_CLS OldThreeOpenPoint  
{
public:
	OldThreeOpenPoint();
	virtual ~OldThreeOpenPoint(){};
	int	m_nThreeOpenPointNodeNum;
	int m_nThreeOpenType;
	double m_dbCrotch;//�ȼ�ѹ��ͨ��������뾶
	double m_dbPadThk;//������
	double m_dbFtgRo;//�ܽ�����ǿ�ε���뾶	
	double m_dbSif; //������Ӧ����ǿϵ��
};

class AFX_EXT_CLS FilepesData
{
public:
	FilepesData();
	virtual ~FilepesData(){};
	int m_nFormNode;
	int m_mToNode;
	double dbPipeDensity;
	double dbFluidDensity;
	double dbInsulationDensity;
	double dbInsulationDensity2;
	double dbInsulationDensity3; 
	double dbPipeInsulationThickness;
	double dbPipeInsulationThickness2;
	double dbPipeInsulationThickness3;
	float m_fSlopeMeetSectNodeCount;//�������б�Ӷν���
};

class AFX_EXT_CLS PipeFeature
{
public:
	PipeFeature();
	virtual ~PipeFeature(){};
	CString m_strMaterialName;//������
	int m_iCodeType; //Ӧ������
	double m_dbPipeOuterDiameter;//�ܵ��⾶
	double m_dbPipeWallThickness; //�ں�
	double m_dbPipeWallThicknessReduce;//���ӱں񸺹���
	double m_dbPipeWallThicknessAdd;//���ӱں�������
	double m_dbPipeCorrosionAllowance;//���ӵĸ�ʴ����ֵ
	double m_dbPipeInsulationThickness; //���º��1
	double m_dbPipeInsulationThickness2; //���º��2
	double m_dbPipeInsulationThickness3; //���º��3
	double m_dbPipeDensity;//�ܵ��ܶ�
	double m_dbFluidDensity;//�����ܶ�
	double m_dbInsulationDensity;//�����ܶ�1
	double m_dbInsulationDensity2;//�����ܶ�2
	double m_dbInsulationDensity3;//�����ܶ�3
	double m_dbPoissonRation; //���ɱ�
	BOOL m_bIsGap; //�Ƿ��к���
	double m_dDn; //ֱ�� PDMS->PSA�ӿ�������
	double m_dInsWeight; //�������� C5.3���õ�
};

//�������ɺ���(AutoPSA1.1�е�)
class AFX_EXT_CLS OldGivenSpringLoad  
{
public:
	OldGivenSpringLoad();
	virtual ~OldGivenSpringLoad(){};
	int	m_nGivenSpringLoadNodeNum;
	double	m_dbGivenSpringVerticalLoad;
};

//����żȻ����(AutoPSA1.1�е�)
class AFX_EXT_CLS OldAccidentalLoad  
{
public:
	OldAccidentalLoad();
	virtual ~OldAccidentalLoad(){};
	double m_dbAccidentalLoadMomentZ;
	double m_dbAccidentalLoadMomentY;
	double m_dbAccidentalLoadMomentX;
	double m_dbAccidentalLoadForceZ;
	double m_dbAccidentalLoadForceY;
	double m_dbAccidentalLoadForceX;
	int m_nAccidentalLoadNodeNum;
};

//���Ӻ���(AutoPSA1.1�е�)
class AFX_EXT_CLS OldAdditionalLoad  
{
public:
	OldAdditionalLoad();
	virtual ~OldAdditionalLoad(){};
	int m_nAdditionalLoadNodeNum;
	int m_nAdditionalLoadConditionNum; //������
	double m_dbAdditionalLoadMomentZ;
	double m_dbAdditionalLoadMomentY;
	double m_dbAdditionalLoadMomentX;
	double m_dbAdditionalLoadForceZ;
	double m_dbAdditionalLoadForceY;
	double m_dbAdditionalLoadForceX;
};


//ֱ��Ԫ
class AFX_EXT_CLS StraightUnit  
{
public:
	StraightUnit();
	virtual ~StraightUnit(){}
	BOOL m_bBend;
	CBendData m_BendData;
};
//��ͨ����
class AFX_EXT_CLS ThreeOrSifsOpenData
{
public:
	ThreeOrSifsOpenData();
	virtual ~ThreeOrSifsOpenData(){};
	int m_iThreeNodeNumer;//��ͨ���
	int m_iThreeType;//��ͨ����(�����е�ID��=���ǽ�������ֵ+1)
	double m_dbSifi;//ƽ���ڵ�ϵ��
	double m_dbSifo;//ƽ�����ϵ��
	double m_dbWeld;//���ӱ�־
	double m_dbFillet;//�޼�
	double m_dbPadThk;//������
	double m_dbFtgRo;//�ܽ�����ǿ�ε���뾶
	double m_dbCrotch;//�ȼ�ѹ��ͨ��������뾶
	double m_dbWeldId;//���ӱ�־
	double m_dbB1;
	double m_dbB2;
};
//��ͨ
class AFX_EXT_CLS ThreeOrSifsOpen
{
public:
	ThreeOrSifsOpen();
	virtual ~ThreeOrSifsOpen(){};
	BOOL m_bThreeOrSifsOpen;
	ThreeOrSifsOpenData m_ThreeOrSifsOpenData[2];
};

//�����ͨ��ѽ�û����õ�ŵ�Ӧ����ǿϵ��(�ڻ�õ��Ӧ����ǿϵ����)
class AFX_EXT_CLS ThreeOrSifsCoef
{
public:
	ThreeOrSifsCoef();
	virtual ~ThreeOrSifsCoef(){}
	int m_iNodeNumer;
	double m_dbStressCoefi; //ƽ����
	double m_dbStressCoefo; //ƽ����
};
class  HighDisplay
{
public:
	CForcmntData  m_forcemnt;
	CDispData m_displacement;
	float m_allowstress;
	float m_factstress;
	int id[2];//corlor
	HighDisplay(){id[0]=id[1]=-1;}
};



class AFX_EXT_CLS ResTypeSever  
{
public:
	ResTypeSever(){};
	virtual ~ResTypeSever(){};
public:
	bool IsAngleRes(int iResId);
	bool IsNotLineRes( int iResId );
	bool IsXRes( int iResId );
	bool IsYRes( int iResId );
	bool IsZRes( int iResId );
};	
#endif