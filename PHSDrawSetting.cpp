// PHSDrawSetting.cpp: implementation of the PHSDrawSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PHSDrawSetting.h"
#include "modPHScal.h"
#include "edibacad.h"
#include "EDIBgbl.h"

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

PHSDrawSetting theDrawSetting;

PHSDrawSetting& GetCurDrawSetting()
{
	return theDrawSetting;
}

PHSDrawSetting::PHSDrawSetting()
{

}

PHSDrawSetting::~PHSDrawSetting()
{
	
}

BOOL PHSDrawSetting::InitSetting()
{
	return TRUE;
}

BOOL PHSDrawSetting::SaveSetting()
{
	SetRegValue(_T("Settings"), _T("IsACAD"), modPHScal::gIsACAD);
	SetRegValue(_T("Settings"), _T("DrawSIZE_A3A4"), modPHScal::giDrawSIZE_A3A4);
	SetRegValue(_T("Settings"), _T("PhsColor"), modPHScal::gsPhsColor);
	SetRegValue(_T("Settings"), _T("DWGPreMethod"), modPHScal::glDWGPreMethod);
	SetRegValue(_T("Settings"), _T("DWGPre"), modPHScal::gsDWGPre);
	SetRegValue(_T("Settings"), _T("SEQyOffset"), modPHScal::gnSEQyOffset);
	SetRegValue(_T("Settings"), _T("NotPrompt_ACAD_SHIFT"), modPHScal::gbNotPrompt_ACAD_SHIFT);
	SetRegValue(_T("Settings"), _T("NotPrompt_Catlog_OVERWRITE"), modPHScal::gbNotPrompt_Catlog_OVERWRITE);
	SetRegValue(_T("Settings"), _T("AutoSaveDrawing"), modPHScal::gbAutoSaveDrawing);
	SetRegValue(_T("Settings"), _T("PhsOutINFOTableType"), modPHScal::giPhsOutINFOTableType);
	SetRegValue(_T("Settings"), _T("DrawNoPreMethod"), modPHScal::gbDrawNoPreMethod);
	SetRegValue(_T("Settings"), _T("DrawNoStartNumber"), modPHScal::glDrawNoStartNumber);
	SetRegValue(_T("Settings"), _T("DwgVersion"), EDIBgbl::strDwgVersion);
	SetRegValue(_T("Settings"), _T("bTBhq"), modPHScal::gbTBhq);	
	SetRegValue(_T("Settings"), _T("bNoFenQu"), modPHScal::gbNoFenQu);
	SetRegValue(_T("Settings"), _T("bNoWaiKuang"), modPHScal::gbNoWaiKuang);
	SetRegValue(_T("Settings"), _T("TbOS"), EDIBgbl::gbTbOS);
	SetRegValue(_T("Settings"), _T("Draw_BOM"), modPHScal::gbDraw_BOM);
	SetRegValue(_T("Settings"), _T("Draw_OVERWRITE"), modPHScal::gbDrawOVERWRITE);
	SetRegValue(_T("Settings"), _T("AnnotateCSPR"), modPHScal::gbAnnotateCSPR);
	SetRegValue(_T("Settings"), _T("LocationDimension"),EDIBAcad::g_bLocationDimension);
	SetRegValue(_T("Settings"), _T("PrintSA_Value"),EDIBAcad::g_bPrintSA_Value);
	SetRegValue(_T("Settings"), _T("CustomPicNo"),EDIBAcad::g_bCustomPicNo);
	SetRegValue(_T("Settings"), _T("InsertLength"),EDIBAcad::g_bInsertLength);
	SetRegValue(_T("Settings"), _T("ColdHeatState"),EDIBAcad::g_bColdHeatState);
	SetRegValue(_T("Settings"), _T("HzAbs"),EDIBAcad::g_bHzabs);
	SetRegValue(_T("Settings"), _T("DWForBJBW"),EDIBAcad::g_bDWForBJBW);
	SetRegValue(_T("Settings"), _T("DimNumber"),EDIBAcad::g_bDimNumber);
	SetRegValue(_T("Settings"), _T("OrientationPicture"),modPHScal::gbOrientationPicture);
	SetRegValue(_T("Settings"), _T("DrawWeldSymbol"), modPHScal::gbDrawWeldSymbol);
	SetRegValue(_T("Settings"), _T("AutoWeldViewInsertPoint"), modPHScal::gbAutoWeldViewInsertPoint);
	SetRegValue(_T("Settings"), _T("DrawWeldViewDim"), modPHScal::gbDrawWeldViewDim);
	SetRegValue(_T("Settings"), _T("WELDAutoDirectAndPos"), modPHScal::gbWeldAutoDirectAndPos);
	SetRegValue(_T("Settings"), _T("DLOADBJBW"),EDIBAcad::g_bDLOADBJBW);
	SetRegValue(_T("Settings"), _T("DrawNameWithoutNo"),EDIBAcad::g_bDrawNameWithoutNo);
	SetRegValue(_T("Settings"), _T("NotDrawSingleWeight"),EDIBAcad::g_bNotDrawSingleWeight);
	SetRegValue(_T("Settings"), _T("SumBomSprFactory"),	EDIBAcad::g_bSumBomSprFactory);
	SetRegValue(_T("Settings"), _T("LJMX_BJBW"),EDIBAcad::g_bLJMX_BJBW);
	SetRegValue(_T("Settings"), _T("TagUsingCircle"),EDIBAcad::g_bTagUsingCircle);
	SetRegValue(_T("Settings"), _T("BGForBJBW"),EDIBAcad::g_bBGForBJBW);
	SetRegValue(_T("Settings"), _T("CircleDiameter"),modPHScal::gfSeqCircleDiameter);
	SetRegValue(_T("Settings"), _T("SeqTxtHeight"),modPHScal::gfSeqTxtHeight);
	SetRegValue(_T("Settings"), _T("SumBomSprFactory"),EDIBAcad::g_bDrawSumWeight);
	SetRegValue(_T("Settings"), _T("BomSprFactory"),EDIBAcad::g_bBomSprFactory);
	SetRegValue(_T("Settings"), _T("InstallElevation"),EDIBAcad::g_bInstallElevation);
	SetRegValue(_T("Settings"), _T("GroupWare"),EDIBAcad::g_bGroupWare);
	SetRegValue(_T("Settings"), _T("strGroupWareName"),EDIBAcad::g_strGroupWareName);
	SetRegValue(_T("Settings"), _T("SetTKWidth"),    EDIBAcad::g_fSetTKWidth);
	SetRegValue(_T("Settings"), _T("SetTableWidth"), EDIBAcad::g_fSetTableWidth);
	SetRegValue(_T("Settings"), _T("OvertopDim"), EDIBAcad::g_fOvertopDim);
	SetRegValue(_T("Settings"), _T("DistanceOffset"), EDIBAcad::g_fDistanceOffset);

	return TRUE;
}

//����ƽ̨����
INT PHSDrawSetting::GetOperatingStation()
{
	return modPHScal::gIsACAD;
}

void PHSDrawSetting::SetOperationgStation(INT nStation)
{
	modPHScal::gIsACAD = nStation;
}

//ͼ���ߴ�
INT PHSDrawSetting::GetFrameSize()
{
	return modPHScal::giDrawSIZE_A3A4;
}

void PHSDrawSetting::SetFrameSize(INT nSize)
{
	modPHScal::giDrawSIZE_A3A4 = nSize;
}

//������ɫ
CString PHSDrawSetting::GetDrawColor()
{
	return modPHScal::gsPhsColor;
}

void PHSDrawSetting::SetDrawColor(const CString& strColor)
{
	modPHScal::gsPhsColor = strColor;
}

//ǰ׺ģʽ
LONG PHSDrawSetting::GetGraphicFilePrefixMode()
{
	return modPHScal::glDWGPreMethod;
}

void PHSDrawSetting::SetGraphicFilePrefixMode(LONG lMode)
{
	modPHScal::glDWGPreMethod = lMode;
}

//ǰ׺�ַ���
CString PHSDrawSetting::GetGraphicFilePrefix()
{
	return modPHScal::gsDWGPre;
}

void PHSDrawSetting::SetGraphicFilePrefix(const CString& strPrefix)
{
	modPHScal::gsDWGPre = strPrefix;
}

//���֮��ľ���
INT PHSDrawSetting::GetMinDistanceBetweenSerialNum()
{
	return modPHScal::gnSEQyOffset;
}

void PHSDrawSetting::SetMinDistanceBetweenSerialNum(INT nDistance)
{
	modPHScal::gnSEQyOffset = nDistance;
}

//������ʾ-AutoCAD�л�
BOOL PHSDrawSetting::IsNotClewSwitchAutoCAD()
{
	return modPHScal::gbNotPrompt_ACAD_SHIFT;
}

void PHSDrawSetting::SetNotClewSwitchAutoCAD(BOOL bState)
{
	modPHScal::gbNotPrompt_ACAD_SHIFT = bState;
}

//������ʾ-Ŀ¼����
BOOL PHSDrawSetting::IsNotClewCoverCatalog()
{
	return modPHScal::gbNotPrompt_Catlog_OVERWRITE;
}

void PHSDrawSetting::SetNotClewCoverCatalog(BOOL bState)
{
	modPHScal::gbNotPrompt_Catlog_OVERWRITE = bState;
}

//�Զ����浱ǰ֧����ͼֽ
BOOL PHSDrawSetting::IsAutoSave()
{
	return modPHScal::gbAutoSaveDrawing;
}

void PHSDrawSetting::SetIsAutoSave(BOOL bState)
{
	modPHScal::gbAutoSaveDrawing = bState;
}

//֧���������Ϣ����
INT PHSDrawSetting::GetPHSOutputInfoStyle()
{
	return modPHScal::giPhsOutINFOTableType;
}

void PHSDrawSetting::SetPHSOutputInfoStyle(INT nStyle)
{
	modPHScal::giPhsOutINFOTableType = nStyle;
}

//֧���ܺŴ����̺;���
BOOL PHSDrawSetting::HasEnginIDAndVolumeID()
{
	return modPHScal::gbDrawNoPreMethod;
}

void PHSDrawSetting::SetHasEnginIDAndVolumeID(BOOL bState)
{
	modPHScal::gbDrawNoPreMethod = bState;
}

//֧������ʼͼ��
LONG PHSDrawSetting::GetPHSStartDrawNum()
{
	return modPHScal::glDrawNoStartNumber;
}

void PHSDrawSetting::SetPHSStartDrawNum(LONG nNum)
{
	modPHScal::glDrawNoStartNumber = nNum;
}

//ͼ�ΰ汾��
CString PHSDrawSetting::GetDWGVersion()
{
	return EDIBgbl::strDwgVersion;
}

void PHSDrawSetting::SetDWGVersion(const CString& strVersion)
{
	EDIBgbl::strDwgVersion = strVersion;
}

//������ǩ��
BOOL PHSDrawSetting::IsNoDrawHuiQian()
{
	return modPHScal::gbTBhq;
}

void PHSDrawSetting::SetNoDrawHuiQian(BOOL bState)
{
	modPHScal::gbTBhq = bState;
}

//����ͼ�����
BOOL PHSDrawSetting::IsNoDrawFrameSubeara()
{
	return modPHScal::gbNoFenQu;
}

void PHSDrawSetting::SetNoDrawFrameSubeara(BOOL bState)
{
	modPHScal::gbNoFenQu = bState;
}

//����ͼ�����
BOOL PHSDrawSetting::IsNoDrawOutFrame()
{
	return modPHScal::gbNoWaiKuang;
}

void PHSDrawSetting::SetNoDrawOutFrame(BOOL bState)
{
	modPHScal::gbNoWaiKuang = bState;
}

//ͼ�����CAD��־
BOOL PHSDrawSetting::HasCADSign()
{
	return  EDIBgbl::gbTbOS;
}

void PHSDrawSetting::SetHasCADSign(BOOL bState)
{
	 EDIBgbl::gbTbOS = bState;
}

//��Ҫ�����ϸ��
BOOL PHSDrawSetting::HasPartList()
{
	return modPHScal::gbDraw_BOM;
}

void PHSDrawSetting::SetHasPartList(BOOL bState)
{
	modPHScal::gbDraw_BOM = bState;
}

//�Զ���������ͼ��
BOOL PHSDrawSetting::IsCoverDraw()
{
	return modPHScal::gbDrawOVERWRITE;
}

void PHSDrawSetting::SetCoverDraw(BOOL bState)
{
	modPHScal::gbDrawOVERWRITE = bState;
}

//������ע���Լ�
BOOL PHSDrawSetting::IsClearLabelSpring()
{
	return modPHScal::gbAnnotateCSPR;
}

void PHSDrawSetting::SetClearLabelSpring(BOOL bState)
{
	modPHScal::gbAnnotateCSPR = bState;
}

//��ע�ܲ���λ�ߴ�
BOOL PHSDrawSetting::IsLabelOrientationOfPA()
{
	return EDIBAcad::g_bLocationDimension;
}

void PHSDrawSetting::SetLabelOrientationOfPA(BOOL bState)
{
	EDIBAcad::g_bLocationDimension = bState;
}

//��ע������λ�ߴ�
BOOL PHSDrawSetting::IsLabelOrientationOfSA()
{
	return EDIBAcad::g_bPrintSA_Value;
}

void PHSDrawSetting::SetLabelOrientationOfSA(BOOL bState)
{
	EDIBAcad::g_bPrintSA_Value = bState;
}

//ͼ���ֹ�����
BOOL PHSDrawSetting::IsManuInputDrawNum()
{
	return EDIBAcad::g_bCustomPicNo;
}

void PHSDrawSetting::SetManuInputDrawNum(BOOL bState)
{
	EDIBAcad::g_bCustomPicNo = bState;
}

//��ע���볤��
BOOL PHSDrawSetting::IsLabelInsertLength()
{
	return EDIBAcad::g_bInsertLength;
}

void PHSDrawSetting::SetLabelInsertLength(BOOL bState)
{
	EDIBAcad::g_bInsertLength = bState;
}

//��ע"��̬"��"��̬"���
BOOL PHSDrawSetting::IsLabelElevationOfColdAndHot()
{
	return EDIBAcad::g_bColdHeatState;
}

void PHSDrawSetting::SetLabelElevationOfColdAndHot(BOOL bState)
{
	EDIBAcad::g_bColdHeatState = bState;
}

//�غ�λ�Ʊ����غ��þ���ֵ��ʾ
BOOL PHSDrawSetting::IsAbsValue()
{
	return EDIBAcad::g_bHzabs;
}

void PHSDrawSetting::SetAbsValue(BOOL bState)
{
	EDIBAcad::g_bHzabs = bState;
}

//����ע����̬���Ӷ�λ�ߴ�
BOOL PHSDrawSetting::IsNoLabelOrientationOfPillar()
{
	return EDIBAcad::g_bDWForBJBW;
}

void PHSDrawSetting::SetNoLabelOrientationOfPillar(BOOL bState)
{
	EDIBAcad::g_bDWForBJBW = bState;
}

//��ע�ظ�����
BOOL PHSDrawSetting::IsLabelSamePartNum()
{
	return EDIBAcad::g_bDimNumber;
}

void PHSDrawSetting::SetLabelSamePartNum(BOOL bState)
{
	EDIBAcad::g_bDimNumber = bState;
}

//����ƽ�涨λͼ
BOOL PHSDrawSetting::IsNoDrawOrientationOfFlat()
{
	return modPHScal::gbOrientationPicture;
}

void PHSDrawSetting::SetNoDrawOrientationOfFlat(BOOL bState)
{
	modPHScal::gbOrientationPicture = bState;
}

//���ƺ������
BOOL PHSDrawSetting::IsDrawWelding()
{
	return modPHScal::gbDrawWeldSymbol;
}

void PHSDrawSetting::SetDrawWelding(BOOL bState)
{
	modPHScal::gbDrawWeldSymbol = bState;
}

//�����Զ�ȷ��������ͼ����λ��
BOOL PHSDrawSetting::IsAutoFixPositionOfWelding()
{
	return modPHScal::gbAutoWeldViewInsertPoint;
}

void PHSDrawSetting::SetAutoFixPositionOfWelding(BOOL bState)
{
	modPHScal::gbAutoWeldViewInsertPoint = bState;
}

//���ƺ�����ͼ��ע
BOOL PHSDrawSetting::IsDrawViewOfWelding()
{
	return modPHScal::gbDrawWeldViewDim;
}

void PHSDrawSetting::SetDrawViewOfWelding(BOOL bState)
{
	modPHScal::gbDrawWeldViewDim = bState;
}

//�Զ�ȷ��������ŷ����λ��
BOOL PHSDrawSetting::IsAutoFixPositionAndDirOfSignOfWelding()
{
	return modPHScal::gbWeldAutoDirectAndPos;
}

void PHSDrawSetting::SetAutoFixPositionAndDirOfSingOfWelding(BOOL bState)
{
	modPHScal::gbWeldAutoDirectAndPos = bState;
}

//��������λ�Ʊ��ʽ
BOOL PHSDrawSetting::IsBaWeiTableFormate()
{
	return EDIBAcad::g_bDLOADBJBW;
}

void PHSDrawSetting::SetBaWeiTableFormate(BOOL bState)
{
	EDIBAcad::g_bDLOADBJBW = bState;
}

//ͼ������֧���ܺ�
BOOL PHSDrawSetting::IsNoSupportIDInDraw()
{
	return EDIBAcad::g_bDrawNameWithoutNo;
}

void PHSDrawSetting::SetNoSupportIDInDraw(BOOL bState)
{
	EDIBAcad::g_bDrawNameWithoutNo = bState;
}

//�����Ϊ1���浥��
BOOL PHSDrawSetting::IsNoDrawUnitWeightIfNumOne()
{
	return EDIBAcad::g_bNotDrawSingleWeight;
}

void PHSDrawSetting::SetNoDrawUnitWeightIfNumOne(BOOL bState)
{
	EDIBAcad::g_bNotDrawSingleWeight = bState;
}

//���ϻ��ܱ��б�ע���ɳ���
BOOL PHSDrawSetting::IsDrawManufactoryOfSpringInMaterial()
{
	return EDIBAcad::g_bSumBomSprFactory;
}

void PHSDrawSetting::SetDrawManufactoryOfSpringInMaterial(BOOL bState)
{
	EDIBAcad::g_bSumBomSprFactory = bState;
}

//���������ϸ��
BOOL PHSDrawSetting::IsBaWeiPartList()
{
	return EDIBAcad::g_bLJMX_BJBW;
}

void PHSDrawSetting::setBaWeiPartList(BOOL bState)
{
	EDIBAcad::g_bLJMX_BJBW = bState;
}

//�����ԲȦ��ʾ
BOOL PHSDrawSetting::IsCircleDrawSerialNum()
{
	return EDIBAcad::g_bTagUsingCircle;
}

void PHSDrawSetting::SetCircleDrawSerialNum(BOOL bState)
{
	EDIBAcad::g_bTagUsingCircle = bState;
}

//������ʽ���
BOOL PHSDrawSetting::IsBaWeiElevation()
{
	return EDIBAcad::g_bBGForBJBW;
}

void PHSDrawSetting::SetBaWeiElevation(BOOL bState)
{
	EDIBAcad::g_bBGForBJBW = bState;
}

//���ԲȦֱ��(mm)
DOUBLE PHSDrawSetting::GetDiameterOfCircleOfSerialNum()
{
	return modPHScal::gfSeqCircleDiameter;
}

void PHSDrawSetting::SetDiameterOfCircleOfSerialNum(DOUBLE dbDiameter)
{
	if(dbDiameter <= 10.0)
		modPHScal::gfSeqCircleDiameter = dbDiameter;
	else
		modPHScal::gfSeqCircleDiameter = 6.0;

	
	if(this->IsCircleDrawSerialNum())
	{
		if(this->GetMinDistanceBetweenSerialNum() <= modPHScal::gfSeqCircleDiameter)
		{
			this->SetMinDistanceBetweenSerialNum(modPHScal::gfSeqCircleDiameter+2);
		}
		else if(this->GetMinDistanceBetweenSerialNum() >= this->GetDiameterOfCircleOfSerialNum() + 6)
		{
			this->SetMinDistanceBetweenSerialNum(modPHScal::gfSeqCircleDiameter+6);
		}
	}
	else
	{
		if(this->GetMinDistanceBetweenSerialNum() < 6)
		{
			this->SetMinDistanceBetweenSerialNum(6);
		}
		else if(this->GetMinDistanceBetweenSerialNum() > 20)
		{
			this->SetMinDistanceBetweenSerialNum(10);
		}
	}
}

//������ָ߶�(mm)
DOUBLE PHSDrawSetting::GetHeightOfCharacterofSerialNum()
{
	return modPHScal::gfSeqTxtHeight;
}

void PHSDrawSetting::SetHeightOfCharaterOfSerialNum(DOUBLE dbHeight)
{
	modPHScal::gfSeqTxtHeight = dbHeight;
}

//�����ϸ���Ϸ�������
BOOL PHSDrawSetting::IsDrawWeightOnPartList()
{
	return EDIBAcad::g_bDrawSumWeight;
}

void PHSDrawSetting::SetDrawWeightOnPartList(BOOL bState)
{
	EDIBAcad::g_bDrawSumWeight = bState;
}

//�����ϸ���б�ע���ɳ���
BOOL PHSDrawSetting::IsDrawManufactoryOfSpringInPartList()
{
	return EDIBAcad::g_bBomSprFactory;
}

void PHSDrawSetting::SetDrawmanufactoryOfSpringInPartList(BOOL bState)
{
	EDIBAcad::g_bBomSprFactory = bState;
}

//ע��"��װ"��߶���
BOOL PHSDrawSetting::IsDrawAssembleCharactor()
{
	return EDIBAcad::g_bInstallElevation;
}

void PHSDrawSetting::SetDrawAssembleCharactor(BOOL bState)
{
	EDIBAcad::g_bInstallElevation = bState;
}

//���ϱ��������ϸ���еġ������д��
CString PHSDrawSetting::GetNameOfComponent()
{
	return EDIBAcad::g_strGroupWareName;
}

void PHSDrawSetting::SetNameOfComponent(const CString& strName)
{
	EDIBAcad::g_strGroupWareName = strName;
}

//���ϱ��������ϸ���еġ��������ʾ��ID
INT PHSDrawSetting::GetNameIDOfComponent()
{
	return EDIBAcad::g_bGroupWare;
}

void PHSDrawSetting::SetNameIDOfcomponent(INT nID)
{
	EDIBAcad::g_bGroupWare = nID;
}

//ͼ�����Ϳ������
DOUBLE PHSDrawSetting::GetLineWidthOfFrame()
{
	return EDIBAcad::g_fSetTKWidth;
}

void PHSDrawSetting::SetLineWidthOfFrame(DOUBLE dbWidth)
{
	EDIBAcad::g_fSetTKWidth = dbWidth;
}

//������ο������
DOUBLE PHSDrawSetting::GetLineWidthOfTable()
{
	return EDIBAcad::g_fSetTableWidth;
}

void PHSDrawSetting::SetLineWidthOfTable(DOUBLE dbWidth)
{
	EDIBAcad::g_fSetTableWidth = dbWidth;
}

//�ߴ��ע�����ߴ��߷�Χ
DOUBLE PHSDrawSetting::GetBeyondScopeOfLabel()
{
	return EDIBAcad::g_fOvertopDim;
}

void PHSDrawSetting::SetBeyondScopeOfLabel(DOUBLE dbScope)
{
	EDIBAcad::g_fOvertopDim = dbScope;
}

//�ߴ��ע���ƫ����
DOUBLE PHSDrawSetting::GetExcursionOfLabel()
{
	return EDIBAcad::g_fDistanceOffset;
}

void PHSDrawSetting::SetExcursionOfLabel(DOUBLE dbExcursion)
{
	EDIBAcad::g_fDistanceOffset = dbExcursion;
}
