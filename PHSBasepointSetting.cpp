// PHSBasepointSetting.cpp: implementation of the PHSBasepointSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PHSBasepointSetting.h"
#include "modphscal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PHSBasepointSetting::PHSBasepointSetting()
{
	m_nStyle = -1;
}

PHSBasepointSetting::~PHSBasepointSetting()
{
	
}

BOOL PHSBasepointSetting::InitSetting()
{
	return TRUE;
}

BOOL PHSBasepointSetting::SaveSetting()
{
	return TRUE;
}

//ͼ��ߴ�����
CString PHSBasepointSetting::GetFrameName()
{
	return modPHScal::Ax;
}

void PHSBasepointSetting::SetFrameName(const CString& strName)
{
	modPHScal::Ax = strName;
}

//��������,0��ʾ֧��,1��ʾ����,2��ʾ˫��
INT PHSBasepointSetting::GetBasePointStyle()
{
	return m_nStyle;
}

void PHSBasepointSetting::SetBasePointStyle(INT nStyle)
{
	m_nStyle = nStyle;
}

//����ͼX����
LONG PHSBasepointSetting::GetCenterXOfLeftView()
{
	return modPHScal::VX_pt0x;
}

void PHSBasepointSetting::SetCenterXOfLeftView(LONG lX)
{
	modPHScal::VX_pt0x = lX;
}

//����ͼY����
LONG PHSBasepointSetting::GetCenterYOfLeftView()
{
	return modPHScal::VX_pt0y;
}

void PHSBasepointSetting::SetCenterYOfLeftView(LONG lY)
{
	modPHScal::VX_pt0y = lY;
}

//����ͼX����
LONG PHSBasepointSetting::GetCenterXOfRightView()
{
	return modPHScal::VZ_pt0x;
}

void PHSBasepointSetting::SetCenterXOfRightView(LONG lX)
{
	modPHScal::VZ_pt0x = lX;
}

//����ͼY����
LONG PHSBasepointSetting::GetCenterYOfRightView()
{
	return modPHScal::VX_pt0y;
}

void PHSBasepointSetting::SetCenterYOfRightView(LONG lY)
{
	modPHScal::VX_pt0y = lY;
}

//ע�Ϳ����½�X����
LONG PHSBasepointSetting::GetRightBottomXOfNote()
{
	return modPHScal::pt1x;
}

void PHSBasepointSetting::SetRightBottomXOfNote(LONG lX)
{
	modPHScal::pt1x = lX;
}

//ע�Ϳ����½�Y����
LONG PHSBasepointSetting::GetRightBottomYOfNote()
{
	return modPHScal::pt1y;
}

void PHSBasepointSetting::SetRightBottomYOfNote(LONG lY)
{
	modPHScal::pt1y = lY;
}

//����嵥���½�X����
LONG PHSBasepointSetting::GetRightBottomXOfPartList()
{
	return modPHScal::pt2x;
}

void PHSBasepointSetting::SetRightBottomXOfPartList(LONG lX)
{
	modPHScal::pt2x = lX;
}

//����嵥���½�Y����
LONG PHSBasepointSetting::GetRightBottomYOfPartList()
{
	return modPHScal::pt2y;
}

void PHSBasepointSetting::SetRightBottomYOfPartList(LONG lY)
{
	modPHScal::pt2y = lY;
}

//�غ�λ�Ʊ����Ͻ�X����
LONG PHSBasepointSetting::GetRightTopXOfLoadAndDisp()
{
	return modPHScal::pt3x;
}

void PHSBasepointSetting::SetRightTopXOfLoadAndDisp(LONG lX)
{
	modPHScal::pt3x = lX;
}

//�غ�λ�Ʊ����Ͻ�Y����
LONG PHSBasepointSetting::GetRightTopYOfLoadAndDisp()
{
	return modPHScal::pt3y;
}

void PHSBasepointSetting::SetRightTopYOfLoadAndDisp(LONG lY)
{
	modPHScal::pt3y = lY;
}

LONG PHSBasepointSetting::GetDimOffset()
{
	return modPHScal::giDimOffset;
}

void PHSBasepointSetting::SetDimOffset(LONG lOffset)
{
	modPHScal::giDimOffset = lOffset;
}

LONG PHSBasepointSetting::GetAttDOffsetX()
{
	return modPHScal::giAttDxOffsetX;
}

void PHSBasepointSetting::SetAttDOffsetX(LONG lX)
{
	modPHScal::giAttDxOffsetX = lX;
	modPHScal::giAttDzOffsetX = modPHScal::giAttDxOffsetX;
}

LONG PHSBasepointSetting::GetAttDOffsetY()
{
	return modPHScal::giAttDOffsetY;
}

void PHSBasepointSetting::SetAttDOffsetY(LONG lY)
{
	modPHScal::giAttDOffsetY = lY;
}


LONG PHSBasepointSetting::GetPt0X()
{
	return modPHScal::Pt0X;
}

void PHSBasepointSetting::SetPt0X(LONG lX)
{
	modPHScal::Pt0X = lX;
}


void PHSBasepointSetting::SetBasePointStyle(INT nSupportStyle, BOOL HasPartList)
{
	INT nStyle = 0;

	if(HasPartList)
	{
		switch(nSupportStyle)
		{
		case 0:
			nStyle = PHSBasepointSetting::eSUPPORTLIST;
			break;
		case 1:
			nStyle = PHSBasepointSetting::eSINGLEHANGERLIST;
			break;
		case 2:
			nStyle = PHSBasepointSetting::eDOUBLEHANGERLIST;
			break;
		}
	}
	else
	{
		switch(nSupportStyle)
		{
		case 0:
			nStyle = PHSBasepointSetting::eSUPPORTNOLIST;
			break;
		case 1:
			nStyle = PHSBasepointSetting::eSINGLEHANGERNOLIST;
			break;
		case 2:
			nStyle = PHSBasepointSetting::eDOUBLEHANGERNOLIST;
			break;
		}
	}
	m_nStyle = nStyle;
}