// PHSCaculateSetting.cpp: implementation of the PHSCaculateSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PHSCaculateSetting.h"
#include <afxdao.h>
#include "modPHScal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
#pragma warning (disable:4800)

extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName,const int vValue);
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName,const CString vValue);
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName,const float vValue);
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName,const bool vValue);
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName, const double vValue);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PHSCaculateSetting theCaculateSetting;

PHSCaculateSetting& GetCurCaculateSetting()
{
	return theCaculateSetting;
}

PHSCaculateSetting::PHSCaculateSetting()
{
	this->m_dbRubCoefficient = 0.0;
}

PHSCaculateSetting::~PHSCaculateSetting()
{
//	this->SaveSetting();
}

BOOL PHSCaculateSetting::InitSetting()
{
	return TRUE;
}

BOOL PHSCaculateSetting::SaveSetting()
{
    SetRegValue(_T("Settings"),	_T("CalcCSPRHeight_BySelDisp"),modPHScal::gbCalcCSPRHeight_BySelDisp);
	SetRegValue(_T("Settings"), _T("MinRodLen"), modPHScal::giMinRodLen);
	SetRegValue(_T("Settings"), _T("UseSpringMinDist"), modPHScal::gUseSpringMinDist);
	SetRegValue(_T("Settings"), _T("SPR_Chzbh"), modPHScal::gnSPR_CHZBH);
	SetRegValue(_T("Settings"), _T("Pjg2Pgz"), modPHScal::gsngRatioOfPjg2Pgz);
	SetRegValue(_T("Settings"), _T("ConnectTJ"), modPHScal::gnConnectTJ);
	SetRegValue(_T("Settings"), _T("RodDiaByMax"), modPHScal::gbRodDiaByMax);
	SetRegValue(_T("Settings"), _T("PipeClampBolt"), modPHScal::gbPipeClampBolt);
	SetRegValue(_T("Settings"), _T("RodLenRoundVal"), modPHScal::gnRodLenRoundVal);
	SetRegValue(_T("Settings"), _T("RodInsertLenRoundVal"), modPHScal::gnRodInsertLenRoundVal);
	SetRegValue(_T("Settings"), _T("ConnectHoleDimMatch"), modPHScal::gbConnectHoleDimMatch);
	SetRegValue(_T("Settings"), _T("SPRAutoLugDia"), modPHScal::gbSPRAutoLugDia);
	SetRegValue(_T("Settings"), _T("AutoApplyChzbh"), modPHScal::gbAutoApplyChzbh);
	SetRegValue(_T("Settings"), _T("AutoApplyCSPRLugDia"), modPHScal::gbAutoApplyCSPRLugDia);
	SetRegValue(_T("Settings"), _T("OutputSA"), modPHScal::gbOutputSA);
	SetRegValue(_T("Settings"), _T("AutoPASA"), modPHScal::gbAutoPASA);
	SetRegValue(_T("Settings"),	_T("HintRepZdjType"),modPHScal::bHintRepZdjType);
	SetRegValue(_T("Settings"),	_T("NumCSVal"),modPHScal::iNumCSVal);
	SetRegValue(_T("Settings"),	_T("AutoSPRRodDiaDecrease"),modPHScal::gbAutoSPRRodDiaDecrease);
	SetRegValue(_T("Settings"),	_T("SPRBHLOADBYInstallLoad"),modPHScal::gbSPRBHLOADBYInstallLoad);
	SetRegValue(_T("Settings"),	_T("SPRBHLOADUNITBYNEWTON"),modPHScal::gbSPRBHLOADUNITBYNEWTON);
	SetRegValue(_T("Settings"),	_T("HotStatus2Zero"),gbHotStatus2Zero);
	SetRegValue(_T("Settings"), _T("SumRodByRoundSteel"), modPHScal::gbSumRodByRoundSteel);
	SetRegValue(_T("Settings"), _T("DHT"), modPHScal::g_bDHT);
	SetRegValue(_T("Settings"), _T("DHTGG"), modPHScal::g_strDHTGG);
	SetRegValue(_T("Settings"), _T("DW_delta"), modPHScal::gnDW_delta);
	SetRegValue(_T("Settings"), _T("Friction-free"), this->m_dbRubCoefficient);
	SetRegValue(_T("Settings"), _T("LimitLugMinDia"), modPHScal::gbLimitLugMinDia);
	SetRegValue(_T("Settings"), _T("CSPRCanSpecialDesign"), modPHScal::gbCSPRCanSpecialDesign);
	SetRegValue(_T("Settings"), _T("AutoAlignCSPRtoSA"), modPHScal::gbAutoAlignCSPRtoSA);
	SetRegValue(_T("Settings"), _T("CalCSPRLugDiaByPjg"), modPHScal::gbCalCSPRLugDiaByPjg);
	SetRegValue(_T("Settings"), _T("CalSAbyConservativenessMethod"), modPHScal::gbCalSAbyConservativenessMethod);
	SetRegValue(_T("Settings"), _T("CalSAbyPJG"), modPHScal::gbCalSAbyPJG);
	SetRegValue(_T("Settings"), _T("AddPartWeight2PMax"), modPHScal::gbAddPartWeight2PMax);
	SetRegValue(_T("Settings"), _T("AddPartWeight2SPRPgz"), modPHScal::gbAddPartWeight2SPRPgz);//2007.07.30
	SetRegValue(_T("Settings"),_T("PAForceZero"),modPHScal::gbPAForceZero);//2007.09.25
	SetRegValue(_T("Settings"), _T("MaxPgzPaz"), modPHScal::gbMaxPgzPaz);//2007.09.03

	SetRegValue(_T("Settings"), _T("AddPartWeight2CSPRPgz"), modPHScal::gbAddPartWeight2CSPRPgz);
	SetRegValue(_T("Settings"), _T("MinMaxDispByActualDisp"), modPHScal::gbMinMaxDispByActualDisp);
	SetRegValue(_T("Settings"), _T("SumAllVolume"), modPHScal::gbSumAllVolume);
	SetRegValue(_T("Settings"), _T("WorksWithSABoltsNuts"), modPHScal::gbWorksWithSABoltsNuts);
	SetRegValue(_T("Settings"), _T("WorksWithBoltsNuts"), modPHScal::gbWorksWithBoltsNuts);
	SetRegValue(_T("Settings"), _T("strSpringOrderSQL"), modPHScal::gstrSpringOrderSQL);
	SetRegValue(_T("Settings"), _T("SpringOrder"), modPHScal::giSpringOrder);
	SetRegValue(_T("Settings"), _T("strSpringOrder"), modPHScal::gstrSpringOrder);

	
	return TRUE;
}

BOOL PHSCaculateSetting::IsDesignBaseMaxWorkLoadOrAssembleLoad()
{
	return FALSE;
}

void PHSCaculateSetting::SetDesignBaseMaxWorkLoadOrAssembleLoad(BOOL bState)
{
}

//�����غɱ仯ϵ��
DOUBLE PHSCaculateSetting::GetSpringLoadChangeCoefficient()
{
	return modPHScal::gnSPR_CHZBH;
}

void PHSCaculateSetting::SetSpringLoadChangeCoefficient(DOUBLE dbCoefficient)
{
	modPHScal::gnSPR_CHZBH = static_cast<float>(dbCoefficient);
}

//�ṹ�غɺ͹����غ�֮��
DOUBLE PHSCaculateSetting::GetStructureLoadWorkLoadRatio()
{
	return modPHScal::gsngRatioOfPjg2Pgz;
}

void PHSCaculateSetting::SetStructureLoadWorkLoadRatio(DOUBLE dbRatio)
{
	modPHScal::gsngRatioOfPjg2Pgz = static_cast<float>(dbRatio);
}

//����ʹ�õ��ɵ���С��λ��ֵ
DOUBLE PHSCaculateSetting::GetMaxHotDisplacementOfRigiditySupport()
{
	return modPHScal::gUseSpringMinDist;
}

void PHSCaculateSetting::SetMaxHotDisplacementOfRigiditySupport(DOUBLE dbMaxDis)
{
	modPHScal::gUseSpringMinDist = static_cast<float>(dbMaxDis);
}

//��������������������¶�(C,GBJ19)
DOUBLE PHSCaculateSetting::GetCaculateTemperature()
{
	return modPHScal::gnConnectTJ;
}

void PHSCaculateSetting::SetCaculateTemperature(DOUBLE dbTemp)
{
	modPHScal::gnConnectTJ = static_cast<float>(dbTemp);
}

//���ҹܾ�����Χϵ��
DOUBLE PHSCaculateSetting::GetPipeDiameterEnlargeCoefficient()
{
	return modPHScal::gnDW_delta;
}

void PHSCaculateSetting::SetPipeDiameterEnlargeCoefficient(DOUBLE dbCoefficient)
{
	modPHScal::gnDW_delta = static_cast<float>(dbCoefficient);
}

//���˳���Բ��ֵ
DOUBLE PHSCaculateSetting::GetRodLengthRoundValue()
{
	return modPHScal::gnRodLenRoundVal;
}

void PHSCaculateSetting::SetRodLengthRoundValue(DOUBLE dbValue)
{
	modPHScal::gnRodLenRoundVal = static_cast<int>(dbValue);
	if (modPHScal::gnRodLenRoundVal<1)
	{
		modPHScal::gnRodLenRoundVal=100;
	}
}

//�Ǳ�֧���ܵĵ�������С����
DOUBLE PHSCaculateSetting::GetMinRodLengthOfUnregularSupport()
{
	return modPHScal::giMinRodLen;
}

void PHSCaculateSetting::SetMinRodLengthOfUnregularSupport(DOUBLE dbLength)
{
	modPHScal::giMinRodLen = static_cast<int>(dbLength);
}

//���˲��볤��Բ��ֵ
DOUBLE PHSCaculateSetting::GetRodInsertLengthRoundValue()
{
	return modPHScal::gnRodInsertLenRoundVal;
}
 
void PHSCaculateSetting::SetRodInsertLengthRoundValue(DOUBLE dbValue)
{
	modPHScal::gnRodInsertLenRoundVal = static_cast<int>(dbValue);
	if (modPHScal::gnRodInsertLenRoundVal<1)
	{
		modPHScal::gnRodInsertLenRoundVal=1;
	}
}

//���ɹ���к��ص�λΪţ��N
BOOL PHSCaculateSetting::IsNewtonUnitOfSpringLoad()
{
	return modPHScal::gbSPRBHLOADUNITBYNEWTON;
}

void PHSCaculateSetting::SetUnitOfSpringLoadIsNewton(BOOL bState)
{
	modPHScal::gbSPRBHLOADUNITBYNEWTON = bState;
}

//Ħ��ϵ��
DOUBLE PHSCaculateSetting::GetRubCoefficient()
{
	return this->m_dbRubCoefficient;
}

void PHSCaculateSetting::SetRubCoefficient(DOUBLE dbCoefficient)
{
	this->m_dbRubCoefficient = dbCoefficient;
}

//���ɹ���к����ð�װ����
BOOL PHSCaculateSetting::IsBaseAssembleLoadDesignSpring()
{
	return modPHScal::gbSPRBHLOADBYInstallLoad;
}

void PHSCaculateSetting::SetBaseAssembleLoadDesignSpring(BOOL bState)
{
	modPHScal::gbSPRBHLOADBYInstallLoad = bState;
}

//��̬����
BOOL PHSCaculateSetting::IsZeroLoadOfHot()
{
	return gbHotStatus2Zero;
}

void PHSCaculateSetting::SetLoadOfHotZero(BOOL bState)
{
	gbHotStatus2Zero = bState;
}

//�������˿��Զ���С
BOOL PHSCaculateSetting::IsRodHoleOfSpringReduce()
{
	return modPHScal::gbAutoSPRRodDiaDecrease;
}

void PHSCaculateSetting::SetRodHoleOfSpringReduce(BOOL bState)
{
	modPHScal::gbAutoSPRRodDiaDecrease = bState;
}

//�������˿��Զ�����
BOOL PHSCaculateSetting::IsRodHoleOfSpringEnlarge()
{
	return modPHScal::gbSPRAutoLugDia;
}

void PHSCaculateSetting::SetRodHoleOfSpringEnglarge(BOOL bState)
{
	modPHScal::gbSPRAutoLugDia = bState;
}

//��ʾ�Ƿ��滻Ӧ���������������ĵ��ɺ�
BOOL PHSCaculateSetting::IsClewToChangeSpringNo()
{
	return modPHScal::bHintRepZdjType;
}

void PHSCaculateSetting::SetClewToChangeSpringNo(BOOL bState)
{
	modPHScal::bHintRepZdjType = bState;
}

//����Ӧ�Ż�ѡ�񵯻�
BOOL PHSCaculateSetting::IsAutoOptimizeSpring()
{
	return modPHScal::gbAutoApplyChzbh;
}

void PHSCaculateSetting::SetAutoOptimizeSpring(BOOL bState)
{
	modPHScal::gbAutoApplyChzbh = bState;
}

//���ӿס��˳ߴ��Զ����
BOOL PHSCaculateSetting::IsAutoCheckHoleAndRodSize()
{
	return modPHScal::gbConnectHoleDimMatch;
}

void PHSCaculateSetting::SetAutoCheckHoleAndRodSize(BOOL bState)
{
	modPHScal::gbConnectHoleDimMatch = bState;
}

//���Ƶ���ֱ��
BOOL PHSCaculateSetting::IsRestrictDiameterOfRod()
{
	return modPHScal::gbLimitLugMinDia;
}

void PHSCaculateSetting::SetRestirctDiameterOfRod(BOOL bState)
{
	modPHScal::gbLimitLugMinDia = bState;
}

//��ѡ�ܲ�����
BOOL PHSCaculateSetting::IsAutoChooseSAAndPA()
{
	return modPHScal::gbAutoPASA;
}

void PHSCaculateSetting::SetAutoChooseSAAndPA(BOOL bState)
{
	modPHScal::gbAutoPASA = bState;
}

//��������������
BOOL PHSCaculateSetting::IsSpecialDesignConstSpring()
{
	return modPHScal::gbCSPRCanSpecialDesign;
}

void PHSCaculateSetting::SetSpecialDesignConstSpring(BOOL bState)
{
	modPHScal::gbCSPRCanSpecialDesign = bState;
}

//����ֱ��һ��
BOOL PHSCaculateSetting::IsSameDiameterOfRod()
{
	return modPHScal::gbRodDiaByMax;
}

void PHSCaculateSetting::SetSameDiamterOfRod(BOOL bState)
{
	modPHScal::gbRodDiaByMax = bState;
}

//�ܼ�������˨�ɻ�С
BOOL PHSCaculateSetting::IsChangeBoltOfClamp()
{
	return modPHScal::gbPipeClampBolt;
}

void PHSCaculateSetting::SetChangeBoltOfClamp(BOOL bState)
{
	modPHScal::gbPipeClampBolt = bState;
}

//�Զ���������������������һ��
BOOL PHSCaculateSetting::IsSameDirectionOfConstSpringAndSA()
{
	return modPHScal::gbAutoAlignCSPRtoSA;
}

void PHSCaculateSetting::SetSameDirectionOfConstSpringAndSA(BOOL bState)
{
	modPHScal::gbAutoAlignCSPRtoSA = bState;
}

//���ṹ����ѡ�������������ֱ��
BOOL PHSCaculateSetting::IsBaseStructureLoadDesignRodOfCSpring()
{
	return modPHScal::gbCalCSPRLugDiaByPjg;
}

void PHSCaculateSetting::SetBaseStructureLoadDesignRodOfCSpring(BOOL bState)
{
	modPHScal::gbCalCSPRLugDiaByPjg = bState;
}

//�����ط�ʽѡ��˫���㵥�����͸�
BOOL PHSCaculateSetting::IsGuardDesignProfileBar()
{
	return modPHScal::gbCalSAbyConservativenessMethod;
}

void PHSCaculateSetting::SetGuardDesignProfileBar(BOOL bState)
{
	modPHScal::gbCalSAbyConservativenessMethod = bState;
}

//���ṹ����ѡ�����
BOOL PHSCaculateSetting::IsBaseStructureLoadDesignSA()
{
	return modPHScal::gbCalSAbyPJG;
}

void PHSCaculateSetting::SetBaseStructureLoadDesignSA(BOOL bState)
{
	modPHScal::gbCalSAbyPJG = bState;
}

//����������������
BOOL PHSCaculateSetting::IsOutputPartTableOfSA()
{
	return modPHScal::gbOutputSA;
}

void PHSCaculateSetting::SetOutputPartTableOfSA(BOOL bState)
{
	modPHScal::gbOutputSA = bState;
}

//�ۼ��㲿���������������
BOOL PHSCaculateSetting::IsAccumulateWeightOfPart()
{
	return modPHScal::gbAddPartWeight2PMax;
}

void PHSCaculateSetting::SetAccumulateWeightOfPart(BOOL bState)
{
	modPHScal::gbAddPartWeight2PMax = bState;
}

//�ۼ��㲿�������������������
BOOL PHSCaculateSetting::IsAccumulateWeightOfPartForCSpring()
{
	return modPHScal::gbAddPartWeight2CSPRPgz;
}

void PHSCaculateSetting::SetAccumulateWeightOfPartForCSpring(BOOL bState)
{
	modPHScal::gbAddPartWeight2CSPRPgz = bState;
}

//���˻���ΪԲ��
BOOL PHSCaculateSetting::IsRodAsRoundSteel()
{
	return modPHScal::gbSumRodByRoundSteel;
}

void PHSCaculateSetting::SetRodAsRoundSteel(BOOL bState)
{
	modPHScal::gbSumRodByRoundSteel = bState;
}

//����ʵ����λ��У�˺����λ����С���ֵ
BOOL PHSCaculateSetting::IsActualHotDisplacementCheckCSpring()
{
	return modPHScal::gbMinMaxDispByActualDisp;
}

void PHSCaculateSetting::SetActualHotDisplacementCheckCSpring(BOOL bState)
{
	modPHScal::gbMinMaxDispByActualDisp = bState;
}

//�������о��
BOOL PHSCaculateSetting::IsCollectAllVolumes()
{
	return modPHScal::gbSumAllVolume;
}

void PHSCaculateSetting::SetCollectAllVolumes(BOOL bState)
{
	modPHScal::gbSumAllVolume = bState;
}

//��ѡ����λ�Ƽ�������װ�߶�
BOOL PHSCaculateSetting::IsBaseHotDispCaculateCSpringHeight()
{
	return modPHScal::gbCalcCSPRHeight_BySelDisp;
}

void PHSCaculateSetting::SetBaseHotDispCaculateCSpringHeight(BOOL bState)
{
	modPHScal::gbCalcCSPRHeight_BySelDisp = bState;
}

//���ܵ纸��
BOOL PHSCaculateSetting::IsCollectWelding()
{
	return modPHScal::g_bDHT;
}

void PHSCaculateSetting::SetCollectWelding(BOOL bState)
{
	modPHScal::g_bDHT = bState;
}

//�纸�����
CString PHSCaculateSetting::GetCollectWeldingSpec()
{
	return modPHScal::g_strDHTGG;
}

void PHSCaculateSetting::SetCollectWeldingSpec(const CString& strSpec)
{
	modPHScal::g_strDHTGG = strSpec;
}

//�����Դ���������ӵ���˨��ĸ
BOOL PHSCaculateSetting::HasBoltNutForSAManufacturer()
{
	return modPHScal::gbWorksWithSABoltsNuts;
}

void PHSCaculateSetting::SetHasboltNutForSAManufacturer(BOOL bState)
{
	modPHScal::gbWorksWithSABoltsNuts = bState;
}

//�����Դ������Ӽ����ӵ���˨��ĸ
BOOL PHSCaculateSetting::HasBoltNutForConnectManufacturer()
{
	return modPHScal::gbWorksWithBoltsNuts;
}

void PHSCaculateSetting::SetHasBoltNutForConnectManufacturer(BOOL bState)
{
	modPHScal::gbWorksWithBoltsNuts = bState;
}

CString PHSCaculateSetting::GetSpringDesignRegular()
{
	return modPHScal::gstrSpringOrderSQL;
}

void PHSCaculateSetting::SetSpringDesignRegular(const CString& strRegular)
{
	modPHScal::gstrSpringOrderSQL = strRegular;
}

//�����͸�����
INT PHSCaculateSetting::GetProfileBarNumOfSA()
{
	return modPHScal::iNumCSVal;
}

void PHSCaculateSetting::SetProfileBarNumOfSA(INT nNum)
{
	modPHScal::iNumCSVal = nNum;
}

//����ֱ���Զ���Ӧ�������ֱ��
BOOL PHSCaculateSetting::IsFitRodDiameterOfCSpring()
{
	return modPHScal::gbAutoApplyCSPRLugDia;
}

void PHSCaculateSetting::SetFitRodDiameterOfCSpring(BOOL bState)
{
	modPHScal::gbAutoApplyCSPRLugDia = bState;
}

//����˳�������
CString PHSCaculateSetting::GetSpringDesignRegularName()
{
	return modPHScal::gstrSpringOrder;
}

void PHSCaculateSetting::SetSpringDesignRegularName(const CString& strRegularName)
{
	modPHScal::gstrSpringOrder = strRegularName;
}

INT PHSCaculateSetting::GetSpringDesignRegularNum()
{
	return modPHScal::giSpringOrder;
}

void PHSCaculateSetting::SetSpringDesignRegularNum(INT nNum)
{
	modPHScal::giSpringOrder = nNum;
}