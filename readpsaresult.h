

#ifndef READPSARESULT_H
#define READPSARESULT_H
extern const _TCHAR* strSprNWEPDI;// = "�����������Ժ��ˮ�ܵ�֧�����ֲᵯ��";
extern const _TCHAR* strSprECEPDI;// = "�����������Ժ��ˮ�ܵ�֧�����ֲᵯ��";
extern const _TCHAR* strSprGB10182;// = "�������ɳ�Բ���ε���";


typedef struct tagSECT
{
	int iDWIndex;
	int iTJIndex;
	int iElementNums;//��֧�ڵ�Ԫ��
}SECT;
typedef struct tagAddElementInfo
{
	int iBeginNodeNum;
	int iAddedNums;
}AddEI;

typedef struct tag_PhsInfo
{
	long iNode;
	long lPhsType;
}_PhsInfo;

typedef struct _tagVector{
	float x,y,z;
}tagVector;

class PhsInfo
{
public:
	PhsInfo(){memset(this,0,sizeof(PhsInfo));}
	int iCalNodeNumber;//����ڵ��
	long lZDJType;
	float fDW;
	float fTJ;
 
	float fPGZ;
	float fPJG;
	float fPAZ;


	float fLX;
	float fLY;
	float fLZ;
	float fQX;
	float fQY;
	float fQZ;

	float fPX;
	float fPY;
	float fPZ;
	float fMX;
	float fMY;
	float fMZ;

	float fCoolLX;
	float fCoolLY;
	float fCoolLZ;
	float fCoolQX;
	float fCoolQY;
	float fCoolQZ;

	float fCoolPX;
	float fCoolPY;
	float fCoolPZ;
	float fCoolMX;
	float fCoolMY;
	float fCoolMZ;

};


class CNodeUpElevation
{
public :
	CNodeUpElevation(){}
	~CNodeUpElevation(){}
	int NodeNum;
	float UpElevation;
	
};



void UpDataZY();
extern void ReadYljsResult(_Recordset* rs);
extern void ReadResult_ZHDYF30(_Recordset* rsResult ,CString SourceDataFileName,long maxZdjh,long FileNameID);
extern void ReadResult_GLIF12(_Recordset* rsResult ,CString SourceDataFileName,long maxZdjh,long FileNameID);
extern void ReadResult_GLIF31(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID);
extern void ReadResult_GLIF31New(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID);
extern void ReadResult_SWEDPSA(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID);
extern void ReadResult_CAESARII42(_Recordset* rsResult, CString SourceDataFileName,long maxZdjh,long FileNameID);
extern void ReadPSAData(_Recordset* rsResult ,CString SourceDataFileName,long maxZdjh,long FileNameID);
extern CString Mid(CString& strTmp,int nStart,int nCount=-1);
double GetParam(CString & strText);
extern _ConnectionPtr gpconCaesar;//CAESARԴ�����ļ�������
extern void ImportDisplacements(_RecordsetPtr rsData,long node, CString strLGKname,CString strJOBNAME_P,int flag);//pfg20050617
//extern void importUnitsForces(_RecordsetPtr &rsData,long &node,CString m_strJOBNAME_P,CString PSA_OutDataFUnit,CString PSA_OutDataMUnit,CString m_strRGKname,int flagR);//pfg20050616
extern double UnitsToUeUnits(_RecordsetPtr rsData,CString SourceUnits,CString UeUnits,double &dOffset);//pfg20050620
extern void ImportDiameter(_RecordsetPtr &rsData,long &node,CString strJOBNAME_A);
extern void ImportHanger(_RecordsetPtr &rsData,long &node,CString strJOBNAME_P);//pfg20050621���뵯������
extern void ConversionTypeCaesarToPhs(_RecordsetPtr &rsData,long &node,CString strJOBNAME_P,CString strGKname,long &iType);//pfg20050624���ӣ�long &iType
#endif