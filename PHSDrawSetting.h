// PHSDrawSetting.h: interface for the PHSDrawSetting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHSDRAWSETTING_H__05888D89_1268_4AF4_BD69_90424CD88136__INCLUDED_)
#define AFX_PHSDRAWSETTING_H__05888D89_1268_4AF4_BD69_90424CD88136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PHSSetting.h"

class __declspec( dllexport ) PHSDrawSetting : public PHSSetting  
{
public:
	PHSDrawSetting();
	virtual ~PHSDrawSetting();

public:
	virtual BOOL InitSetting();
	virtual BOOL SaveSetting();

public:
	//����ƽ̨����
	INT GetOperatingStation();
	void SetOperationgStation(INT nStation);

	//ͼ���ߴ�
	INT GetFrameSize();
	void SetFrameSize(INT nSize);

	//������ɫ
	CString GetDrawColor();
	void SetDrawColor(const CString& strColor);

	//ǰ׺ģʽ
	LONG GetGraphicFilePrefixMode();
	void SetGraphicFilePrefixMode(LONG lMode);

	//ǰ׺�ַ���
	CString GetGraphicFilePrefix();
	void SetGraphicFilePrefix(const CString& strPrefix);

	//���֮��ľ���
	INT GetMinDistanceBetweenSerialNum();
	void SetMinDistanceBetweenSerialNum(INT nDistance);

	//������ʾ-AutoCAD�л�
	BOOL IsNotClewSwitchAutoCAD();
	void SetNotClewSwitchAutoCAD(BOOL bState);

	//������ʾ-Ŀ¼����
	BOOL IsNotClewCoverCatalog();
	void SetNotClewCoverCatalog(BOOL bState);

	//�Զ����浱ǰ֧����ͼֽ
	BOOL IsAutoSave();
	void SetIsAutoSave(BOOL bState);

	//֧���������Ϣ����
	INT GetPHSOutputInfoStyle();
	void SetPHSOutputInfoStyle(INT nStyle);

	//֧���ܺŴ����̺;���
	BOOL HasEnginIDAndVolumeID();
	void SetHasEnginIDAndVolumeID(BOOL bState);

	//֧������ʼͼ��
	LONG GetPHSStartDrawNum();
	void SetPHSStartDrawNum(LONG nNum);

	//ͼ�ΰ汾��
	CString GetDWGVersion();
	void SetDWGVersion(const CString& strVersion);

	//������ǩ��
	BOOL IsNoDrawHuiQian();
	void SetNoDrawHuiQian(BOOL bState);

	//����ͼ�����
	BOOL IsNoDrawFrameSubeara();
	void SetNoDrawFrameSubeara(BOOL bState);

	//����ͼ�����
	BOOL IsNoDrawOutFrame();
	void SetNoDrawOutFrame(BOOL bState);

	//ͼ�����CAD��־
	BOOL HasCADSign();
	void SetHasCADSign(BOOL bState);

	//��Ҫ�����ϸ��
	BOOL HasPartList();
	void SetHasPartList(BOOL bState);

	//�Զ���������ͼ��
	BOOL IsCoverDraw();
	void SetCoverDraw(BOOL bState);

	//������ע���Լ�
	BOOL IsClearLabelSpring();
	void SetClearLabelSpring(BOOL bState);

	//��ע�ܲ���λ�ߴ�
	BOOL IsLabelOrientationOfPA();
	void SetLabelOrientationOfPA(BOOL bState);
	
	//��ע������λ�ߴ�
	BOOL IsLabelOrientationOfSA();
	void SetLabelOrientationOfSA(BOOL bState);

	//ͼ���ֹ�����
	BOOL IsManuInputDrawNum();
	void SetManuInputDrawNum(BOOL bState);

	//��ע���볤��
	BOOL IsLabelInsertLength();
	void SetLabelInsertLength(BOOL bState);

	//��ע"��̬"��"��̬"���
	BOOL IsLabelElevationOfColdAndHot();
	void SetLabelElevationOfColdAndHot(BOOL bState);

	//�غ�λ�Ʊ����غ��þ���ֵ��ʾ
	BOOL IsAbsValue();
	void SetAbsValue(BOOL bState);

	//����ע����̬���Ӷ�λ�ߴ�
	BOOL IsNoLabelOrientationOfPillar();
	void SetNoLabelOrientationOfPillar(BOOL bState);

	//��ע�ظ�����
	BOOL IsLabelSamePartNum();
	void SetLabelSamePartNum(BOOL bState);

	//����ƽ�涨λͼ
	BOOL IsNoDrawOrientationOfFlat();
	void SetNoDrawOrientationOfFlat(BOOL bState);

	//���ƺ������
	BOOL IsDrawWelding();
	void SetDrawWelding(BOOL bState);

	//�����Զ�ȷ��������ͼ����λ��
	BOOL IsAutoFixPositionOfWelding();
	void SetAutoFixPositionOfWelding(BOOL bState);

	//���ƺ�����ͼ��ע
	BOOL IsDrawViewOfWelding();
	void SetDrawViewOfWelding(BOOL bState);

	//�Զ�ȷ��������ŷ����λ��
	BOOL IsAutoFixPositionAndDirOfSignOfWelding();
	void SetAutoFixPositionAndDirOfSingOfWelding(BOOL bState);

	//��������λ�Ʊ��ʽ
	BOOL IsBaWeiTableFormate();
	void SetBaWeiTableFormate(BOOL bState);

	//ͼ������֧���ܺ�
	BOOL IsNoSupportIDInDraw();
	void SetNoSupportIDInDraw(BOOL bState);

	//�����Ϊ1���浥��
	BOOL IsNoDrawUnitWeightIfNumOne();
	void SetNoDrawUnitWeightIfNumOne(BOOL bState);

	//���ϻ��ܱ��б�ע���ɳ���
	BOOL IsDrawManufactoryOfSpringInMaterial();
	void SetDrawManufactoryOfSpringInMaterial(BOOL bState);

	//���������ϸ��
	BOOL IsBaWeiPartList();
	void setBaWeiPartList(BOOL bState);

	//�����ԲȦ��ʾ
	BOOL IsCircleDrawSerialNum();
	void SetCircleDrawSerialNum(BOOL bState);

	//������ʽ���
	BOOL IsBaWeiElevation();
	void SetBaWeiElevation(BOOL bState);

	//���ԲȦֱ��(mm)
	DOUBLE GetDiameterOfCircleOfSerialNum();
	void SetDiameterOfCircleOfSerialNum(DOUBLE dbDiameter);

	//������ָ߶�(mm)
	DOUBLE GetHeightOfCharacterofSerialNum();
	void SetHeightOfCharaterOfSerialNum(DOUBLE dbHeight);

	//�����ϸ���Ϸ�������
	BOOL IsDrawWeightOnPartList();
	void SetDrawWeightOnPartList(BOOL bState);

	//�����ϸ���б�ע���ɳ���
	BOOL IsDrawManufactoryOfSpringInPartList();
	void SetDrawmanufactoryOfSpringInPartList(BOOL bState);

	//ע��"��װ"��߶���
	BOOL IsDrawAssembleCharactor();
	void SetDrawAssembleCharactor(BOOL bState);

	//���ϱ��������ϸ���еġ��������ʾ��
	CString GetNameOfComponent();
	void SetNameOfComponent(const CString& strName);

	//���ϱ��������ϸ���еġ��������ʾ��ID
	INT GetNameIDOfComponent();
	void SetNameIDOfcomponent(INT nID);

	//ͼ�����Ϳ������
	DOUBLE GetLineWidthOfFrame();
	void SetLineWidthOfFrame(DOUBLE dbWidth);

	//������ο������
	DOUBLE GetLineWidthOfTable();
	void SetLineWidthOfTable(DOUBLE dbWidth);

	//�ߴ��ע�����ߴ��߷�Χ
	DOUBLE GetBeyondScopeOfLabel();
	void SetBeyondScopeOfLabel(DOUBLE dbScope);

	////�ߴ��ע���ƫ����
	DOUBLE GetExcursionOfLabel();
	void SetExcursionOfLabel(DOUBLE dbExcursion);
};

_declspec(dllexport) PHSDrawSetting& GetCurDrawSetting();
#endif // !defined(AFX_PHSDRAWSETTING_H__05888D89_1268_4AF4_BD69_90424CD88136__INCLUDED_)
