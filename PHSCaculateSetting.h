// PHSCaculateSetting.h: interface for the PHSCaculateSetting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHSCACULATESETTING_H__DE1AB612_DF0C_474E_A95B_1A440A17C1B0__INCLUDED_)
#define AFX_PHSCACULATESETTING_H__DE1AB612_DF0C_474E_A95B_1A440A17C1B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PHSSetting.h"

class __declspec( dllexport ) PHSCaculateSetting : public PHSSetting  
{
public:
	PHSCaculateSetting(); 
	virtual ~PHSCaculateSetting();

public:
	virtual BOOL InitSetting();
	virtual BOOL SaveSetting();

public:
	BOOL IsDesignBaseMaxWorkLoadOrAssembleLoad();
	void SetDesignBaseMaxWorkLoadOrAssembleLoad(BOOL bState);

	//�����غɱ仯ϵ��
	DOUBLE GetSpringLoadChangeCoefficient();
	void SetSpringLoadChangeCoefficient(DOUBLE dbCoefficient);
	
	//�ṹ�غɺ͹����غ�֮��
	DOUBLE GetStructureLoadWorkLoadRatio();
	void SetStructureLoadWorkLoadRatio(DOUBLE dbRatio);
	
	//����ʹ�õ��ɵ���С��λ��ֵ
	DOUBLE GetMaxHotDisplacementOfRigiditySupport();
	void SetMaxHotDisplacementOfRigiditySupport(DOUBLE dbMaxDis);
	
	//��������������������¶�(C,GBJ19)
	DOUBLE GetCaculateTemperature();
	void SetCaculateTemperature(DOUBLE dbTemp);

	//���ҹܾ�����Χϵ��
	DOUBLE GetPipeDiameterEnlargeCoefficient();
	void SetPipeDiameterEnlargeCoefficient(DOUBLE dbCoefficient);
	
	//���˳���Բ��ֵ
	DOUBLE GetRodLengthRoundValue();
	void SetRodLengthRoundValue(DOUBLE dbValue);
	
	//�Ǳ�֧���ܵĵ�������С����
	DOUBLE GetMinRodLengthOfUnregularSupport();
	void SetMinRodLengthOfUnregularSupport(DOUBLE dbLength);
	
	//���˲��볤��Բ��ֵ
	DOUBLE GetRodInsertLengthRoundValue();
	void SetRodInsertLengthRoundValue(DOUBLE dbValue);
	
	//���ɹ���к��ص�λΪţ��N
	BOOL IsNewtonUnitOfSpringLoad();
	void SetUnitOfSpringLoadIsNewton(BOOL bState);
	DOUBLE GetRubCoefficient();
	void SetRubCoefficient(DOUBLE dbCoefficient);

	//���ɹ���к����ð�װ����
	BOOL IsBaseAssembleLoadDesignSpring();
	void SetBaseAssembleLoadDesignSpring(BOOL bState);

	//��̬����
	BOOL IsZeroLoadOfHot();
	void SetLoadOfHotZero(BOOL bState);
	
	//�������˿��Զ���С
	BOOL IsRodHoleOfSpringReduce();
	void SetRodHoleOfSpringReduce(BOOL bState);
	
	//�������˿��Զ�����
	BOOL IsRodHoleOfSpringEnlarge();
	void SetRodHoleOfSpringEnglarge(BOOL bState);
	
	//��ʾ�Ƿ��滻Ӧ���������������ĵ��ɺ�
	BOOL IsClewToChangeSpringNo();
	void SetClewToChangeSpringNo(BOOL bState);

	//����Ӧ�Ż�ѡ�񵯻�
	BOOL IsAutoOptimizeSpring();
	void SetAutoOptimizeSpring(BOOL bState);

	//���ӿס��˳ߴ��Զ����
	BOOL IsAutoCheckHoleAndRodSize();
	void SetAutoCheckHoleAndRodSize(BOOL bState);
	
	//���Ƶ���ֱ��
	BOOL IsRestrictDiameterOfRod();
	void SetRestirctDiameterOfRod(BOOL bState);

	//��ѡ�ܲ�����
	BOOL IsAutoChooseSAAndPA();
	void SetAutoChooseSAAndPA(BOOL bState);

	//��������������
	BOOL IsSpecialDesignConstSpring();
	void SetSpecialDesignConstSpring(BOOL bState);

	//����ֱ��һ��
	BOOL IsSameDiameterOfRod();
	void SetSameDiamterOfRod(BOOL bState);

	//�ܼ�������˨�ɻ�С
	BOOL IsChangeBoltOfClamp();
	void SetChangeBoltOfClamp(BOOL bState);

	//�Զ���������������������һ��
	BOOL IsSameDirectionOfConstSpringAndSA();
	void SetSameDirectionOfConstSpringAndSA(BOOL bState);

	//���ṹ����ѡ�������������ֱ��
	BOOL IsBaseStructureLoadDesignRodOfCSpring();
	void SetBaseStructureLoadDesignRodOfCSpring(BOOL bState);

	//�����ط�ʽѡ��˫���㵥�����͸�
	BOOL IsGuardDesignProfileBar();
	void SetGuardDesignProfileBar(BOOL bState);

	//����ֱ���Զ���Ӧ�������ֱ��
	BOOL IsFitRodDiameterOfCSpring();
	void SetFitRodDiameterOfCSpring(BOOL bState);

	//���ṹ����ѡ�����
	BOOL IsBaseStructureLoadDesignSA();
	void SetBaseStructureLoadDesignSA(BOOL bState);

	//����������������
	BOOL IsOutputPartTableOfSA();
	void SetOutputPartTableOfSA(BOOL bState);

	//�ۼ��㲿���������������
	BOOL IsAccumulateWeightOfPart();
	void SetAccumulateWeightOfPart(BOOL bState);

	//�ۼ��㲿�������������������
	BOOL IsAccumulateWeightOfPartForCSpring();
	void SetAccumulateWeightOfPartForCSpring(BOOL bState);

	//���˻���ΪԲ��
	BOOL IsRodAsRoundSteel();
	void SetRodAsRoundSteel(BOOL bState);

	//����ʵ����λ��У�˺����λ����С���ֵ
	BOOL IsActualHotDisplacementCheckCSpring();
	void SetActualHotDisplacementCheckCSpring(BOOL bState);

	//�������о��
	BOOL IsCollectAllVolumes();
	void SetCollectAllVolumes(BOOL bState);

	//��ѡ����λ�Ƽ�������װ�߶�
	BOOL IsBaseHotDispCaculateCSpringHeight();
	void SetBaseHotDispCaculateCSpringHeight(BOOL bState);

	//���ܵ纸��
	BOOL IsCollectWelding();
	void SetCollectWelding(BOOL bState);

	//�纸�����
	CString GetCollectWeldingSpec();
	void SetCollectWeldingSpec(const CString& strSpec);

	//�����Դ���������ӵ���˨��ĸ
	BOOL HasBoltNutForSAManufacturer();
	void SetHasboltNutForSAManufacturer(BOOL bState);

	//�����Դ������Ӽ����ӵ���˨��ĸ
	BOOL HasBoltNutForConnectManufacturer();
	void SetHasBoltNutForConnectManufacturer(BOOL bState);

	INT GetSpringDesignRegularNum();
	void SetSpringDesignRegularNum(INT nNum);

	//����˳�������
	CString GetSpringDesignRegularName();
	void SetSpringDesignRegularName(const CString& strRegularName);

	//����˳�����
	CString GetSpringDesignRegular();
	void SetSpringDesignRegular(const CString& strRegular);

	////�����͸�����
	INT GetProfileBarNumOfSA();
	void SetProfileBarNumOfSA(INT nNum);

private:
	DOUBLE m_dbRubCoefficient;
};

__declspec( dllexport ) PHSCaculateSetting& GetCurCaculateSetting();
#endif // !defined(AFX_PHSCACULATESETTING_H__DE1AB612_DF0C_474E_A95B_1A440A17C1B0__INCLUDED_)
