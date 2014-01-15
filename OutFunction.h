#ifndef OUTFUNCTION_H
#define OUTFUNCTION_H

#include <windef.h>
#include "PipeUnitData.h"
//CStringW ֻ����VS2002���ϵĻ����вſ����á�
#if _MSC_VER > 1200
typedef CList<CStringW,CStringW> strMatNameList;
#endif // _MSC_VER > 1200

typedef CList<CPipeUnitData *,CPipeUnitData *> mPipeUnitList;
struct MaxTable
{
  CString m_Value;
  INT m_Node;
  CString m_Name; 
  MaxTable(){m_Name="";m_Node=0;m_Value="";}
};



class __declspec(dllexport) CaseTable// ����+�ַ���
{
public:
 CaseTable()
 {m_tableName=m_returnField=m_dependField="";
  m_NO=NULL;m_str=NULL;m_library=NULL;
  m_stat=0;
 }
 CaseTable(_ConnectionPtr,CString,CString,CString);
 VOID SetDataLibrary(_ConnectionPtr);
 VOID DesignTable(CString,CString,CString);
 VOID refresh();
 int GetNumericField(int);
 CString GetStringField(int);
 ~ CaseTable ();
protected:
	VOID SetCaseTable();
private:
	 _ConnectionPtr m_dataLibrary;
	 CString m_dependField;
	 CString m_tableName;
     CString m_returnField;
	 CString *m_str;
	 int *m_NO;
	 int m_stat;
	 BOOL m_library;
};




 extern "C" __declspec(dllexport) int GetAllowStressCase(_ConnectionPtr&,int**);///����������Ӧ���Ĺ���
 extern "C" __declspec(dllexport) BOOL GetStressForCase (_ConnectionPtr&,mPipeUnitList *,int);///���������ع�ϵ�Ķ�Ӧ�ڵ��Ӧ��������Ӧ��
 __declspec(dllexport) MaxTable GetMaxWithTable(_ConnectionPtr&,CString ,int,CString ="LCASE_NUM",
														  CString ="OUTPUT_STRESSES");//����������Table�����ֵ

 //__declspec(dllexport)VOID OperateCaseTable(CaseTable**,INT symbol);

 //0 ��Z  1 ��-Z  2��X  3��-X  4��Y  5��-Y
 typedef enum
 {
	 CsUp = 0,
		 CsDown = 1,
		 CsEast = 2,
		 CsWest = 3,
		 CsNorth = 4,
		CsSouth = 5
}CSType;

 int GetCaseTable(_ConnectionPtr&,CString**,int **,CString="CASE",CString ="LCASE_NUM",
														  CString ="OUTPUT_RESTRAINTS");//���ع������
extern "C" __declspec(dllexport) int ReadPSAFileToListNoTransfer(mPipeUnitList *m_pPipeUnitList,CPipeGlobalData *m_PipeGlobalData,CString cFileName,_ConnectionPtr pRefInfoCon);//û�н���λת��Ϊ��׼��λ
extern "C" __declspec(dllexport) int ReadPSAFileToList(mPipeUnitList *m_pPipeUnitList,CPipeGlobalData *m_PipeGlobalData,CString cFileName,_ConnectionPtr pRefInfoCon);
extern "C" __declspec(dllexport) void freeList(mPipeUnitList *pPipeUnitList);

extern "C" __declspec(dllexport) int WriteDataToPSAFile(mPipeUnitList *pPipeUnitList,CPipeGlobalData *pPipeGlobalData,CString strFileName,CString strmainVersion,CString strsubVersion,_ConnectionPtr pMaterialCon);//2007.04.26 lyf

//�����������룬ֻ����VS2002���ϵı��뻷���в��ô˽ӿ�
#if _MSC_VER > 1200
//֧�ֿ��ַ��Ľӿڣ�ΪPDMS8.0����
extern "C" __declspec(dllexport) void WriteDataToPSAFileW(mPipeUnitList *pPipeUnitList,CPipeGlobalData *pPipeGlobalData,CStringW strFileName,CStringW strmainVersion,CStringW strsubVersion,_ConnectionPtr pMaterialCon , strMatNameList &MatNameList);//2009.04.27 liuc
#endif // _MSC_VER > 1200

//��λת��
extern "C" __declspec(dllexport) void TransferData(mPipeUnitList *pPipeUnitList,CPipeGlobalData *pPipeGlobalData,_ConnectionPtr &m_UnitsCon);

//�õ���λ�ı���,��ǩ��ʾ
extern "C" __declspec(dllexport) void GetUnitScaleTip(CPipeGlobalData * pGlobalData,_ConnectionPtr &m_UnitsCon,int iUnitsCode);

//���������
extern "C" __declspec(dllexport) void SetUnitCoordinate(mPipeUnitList *pPipeUnitList, CPipeGlobalData *PipeGlobalData);

extern "C" __declspec(dllexport) int WriteListToCmpFile(mPipeUnitList *pPipeUnitList,CPipeGlobalData *pPipeGlobalData,CString strFileName,_ConnectionPtr m_RefInfoCon,_ConnectionPtr pMaterialConn);
//����ת��
extern "C" __declspec(dllexport) void ChangeCoordinate(_ConnectionPtr &RefInfoCon,mPipeUnitList *pPipeUnitList,CPipeGlobalData *PipeGlobalData,int axisFrom,int axisTo);

extern "C" __declspec(dllexport) void ExChangeCoordinate(_ConnectionPtr &RefInfoCon,mPipeUnitList *pPipeUnitList,CPipeGlobalData *PipeGlobalData,CSType XCs,CSType YCs,CSType ZCs);

//�õ���ϵ�г��ֹ������й�������(����:H����;���,ˮѹ,��,�¶�,ѹ��,��/����,������,λ��)
extern "C" __declspec(dllexport) void GetAllCaseLoadCode(CArray<CString,CString> &CaseLoadArray,mPipeUnitList *pPipeUnitList);

//���������еĴ�������
extern "C" __declspec(dllexport) void CheckDataError(CArray<CString,CString> &pErrArray,CArray<CString,CString> &pWarningArray,mPipeUnitList *pPipeList,CPipeGlobalData *pPipeGlobalData,_ConnectionPtr pRefInfoCon,_ConnectionPtr pMaterialCon,CString strFileName);

//д�����������ļ��������
extern "C" __declspec(dllexport) int WritStaOutFileToTable(CString strFileName,_ConnectionPtr &pRefInfoCon,_ConnectionPtr &pSpringCon);

//��PSA1.1���ļ�����
extern "C" __declspec(dllexport) int ReadInFileToList(mPipeUnitList *pPipeList,CPipeGlobalData *pGlobalData,CString strFileName,_ConnectionPtr pRefInfoCon,_ConnectionPtr pMaterialCon,CArray<CString,CString> &pArray);
//дCII�ļ�
extern "C" __declspec(dllexport) int WriteListToCiiFile(mPipeUnitList *pPipeUnitList,CPipeGlobalData const *pGlobalData,CString strFileName,_ConnectionPtr pRefInfoCon,_ConnectionPtr pMaterialCon,_ConnectionPtr pSpringCon);
extern "C" __declspec(dllexport) int WriteListToCiiFileEx(mPipeUnitList *pPipeUnitList,CPipeGlobalData const *pGlobalData,CString strFileName,_ConnectionPtr pRefInfoCon,_ConnectionPtr pMaterialCon,_ConnectionPtr pSpringCon);

//д.in�ļ�
extern "C" __declspec(dllexport) int WriteListToInFile(int nOutUnitsType,int nSpringType,int nCodeType,int nCoefProofS,int nCoefProofB,
							 float fStressReduceRatio,float fAllowStressRatio5,float fAllowStressRatio6,
							 int nMaxSeriesNum,CString	strLoadChangeRatio,double dbRigidityType,double dbRigidityTimes,
							 float fEAffectRatio,int nOutPutStress,int nAddWaterWeight,
							 bool bIGK5,bool bIGK6,bool bIGK7,bool bIGK8,bool bIGK9,bool bIGK10,int nBendFlag,double dbWeldFactor,
							 mPipeUnitList *pPipeUnitList,CPipeGlobalData *PipeGlobalData,CString strFileName,_ConnectionPtr pRefInfoCon);

//���ҵ�ǰ��Ԫ�Ĳ�������
extern "C" __declspec(dllexport) void FindUnitMaterialFeature(CPipeUnitData *pPipeUnitData,CPipeGlobalData* pGlobalData,_ConnectionPtr pMaterialCon);

//�����е�Ԫ�ϲ���ת�����²���
extern "C" __declspec(dllexport) void ConversionNewMaterial(mPipeUnitList *pUnitList,_ConnectionPtr pMaterialCon,CArray<CString,CString> *pInfoArray=NULL);

//�������е�Ԫ���ܶȣ����ɱȵ�����
extern "C" __declspec(dllexport) void SetUnitDensityPoissOther(mPipeUnitList *pUnitList,_ConnectionPtr pMaterialCon);

//�õ������ܶ�
extern "C" __declspec(dllexport) void FindMaterialDensityPoissOther(CString strMaterialName,_ConnectionPtr pMaterialCon,double &dbPipeDensity,double &dbPoisson);

//��������Žڵ�Լ�����ݵĵ�Ԫ(��һ����Ԫ����������һ�������֤,��PSA1.1�е�176����)
//pPipeUnitData:��ǰ��Ԫ
//pRtPipeUnitData:����ֵ��ŵĵ�Ԫ; iIndex:���λ��
extern "C" __declspec(dllexport) void GetSaveNodeResTypeUnit(mPipeUnitList *pUnitList,CPipeUnitData *pPipeUnitData,CPipeUnitData *&pRtPipeUnitData,int &iIndex);


//�ϲ��䵥Ԫ(����.in��.psa�ļ���PDARX����.psa�ļ�) iIWD: 1:ʱֱ��Ԫ�ĳ��Ȳ�������ܰ� iMaxNodeNumr:����
extern "C" __declspec(dllexport) void UniteBendUnit(mPipeUnitList *pUnitList,int iIWD,int &iMaxNodeNumr);

//�õ���Ԫ0�ȴ��ĵ��(��ǰ��Ԫ��ĩ���Բ���1��)
extern "C" __declspec(dllexport) int GetBend0DegreeNode(mPipeUnitList * m_pUnitList,CPipeUnitData *pPipeUnitData);

//д֧�������ݵ�PHS(֧����������)��
extern "C" __declspec(dllexport) int WritePHSDataToTable(_ConnectionPtr pRefInfoCon,_Recordset *rsResult,CString strDataFileName,long imaxZdjh,long iFileNameID,long iSelVlmID,double dbRatioOfPjg2Pgz );

//ת��GLif1.2ToGlif3.x�ļ���ʽ
extern "C" __declspec(dllexport) int GLIFv12ToGLIF3x(CString strOldFileName,CString strNewFileName);

//CII4.5�ļ�ת����PSA�ļ���ʽ
extern "C" __declspec(dllexport) int CIIToPSAFile(CString strCiiFileName,CString strPSAFileName,
						_ConnectionPtr pRefInfo,_ConnectionPtr pSpringInfo,_ConnectionPtr pMaterialInfo,
						CArray<CString,CString> *pArray,CString strMainVerSion,CString strsubVersion);

extern "C" __declspec(dllexport) int ResultDataDistill(mPipeUnitList *pPipeUnitList,CPipeGlobalData *pPipeGlobalData,int iCase,_ConnectionPtr pConResult,double dbDispScale);
//�ı�λ�ƷŴ����, ����:�ڴ�ṹ��һ�������Ľ��λ���������
extern "C" __declspec(dllexport) int SetResultDispScale(mPipeUnitList *pPipeUnitList,CPipeGlobalData *pPipeGlobalData,int iCase,_ConnectionPtr pConResult,double dbNewDispScale,double dbOldDispScale);

//�õ��뵥Ԫ�����ĵ�Ԫ
//i=1:��������������ĵ�Ԫ; 2:��ĩ�������ĵ�Ԫ
__declspec( dllexport ) CPipeUnitData *GetConnectUnit(mPipeUnitList *pPipeList,CPipeUnitData *pPipeUnitData,int i);

//�õ��䵥Ԫ�ĽǶ�
__declspec( dllexport ) double GetBendUnitAngle(mPipeUnitList *pPipeList,CPipeUnitData *pPipeUnitData);

#endif
