// PHSBasepointSetting.h: interface for the PHSBasepointSetting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHSBASEPOINTSETTING_H__ADC24A88_1B25_4AAE_9FF5_C8FB950137B6__INCLUDED_)
#define AFX_PHSBASEPOINTSETTING_H__ADC24A88_1B25_4AAE_9FF5_C8FB950137B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PHSSetting.h"

class __declspec( dllexport ) PHSBasepointSetting : public PHSSetting  
{
public:
	enum{
		eSUPPORTLIST,
			eSUPPORTNOLIST,
			eSINGLEHANGERLIST,
			eSINGLEHANGERNOLIST,
			eDOUBLEHANGERLIST,
			eDOUBLEHANGERNOLIST};
public:		
	enum{
		eCURPOSITION,
			eDEFAULTPOSITION,
			eOLDPOSITION};
		
public:
	PHSBasepointSetting();
	virtual ~PHSBasepointSetting();

public:
	virtual BOOL InitSetting();
	virtual BOOL SaveSetting();

public:
	//ͼ��ߴ�����
	CString GetFrameName();
	void SetFrameName(const CString& strName);

	//��������,0��ʾ֧��,1��ʾ����,2��ʾ˫��
	INT GetBasePointStyle();
	void SetBasePointStyle(INT nStyle);
	void SetBasePointStyle(INT nSupportStyle, BOOL HasPartList);

	//����ͼX����
	LONG GetCenterXOfLeftView();
	void SetCenterXOfLeftView(LONG lX);

	//����ͼY����
	LONG GetCenterYOfLeftView();
	void SetCenterYOfLeftView(LONG lY);

	//����ͼX����
	LONG GetCenterXOfRightView();
	void SetCenterXOfRightView(LONG lX);

	//����ͼY����
	LONG GetCenterYOfRightView();
	void SetCenterYOfRightView(LONG lY);

	//ע�Ϳ����½�X����
	LONG GetRightBottomXOfNote();
	void SetRightBottomXOfNote(LONG lX);

	//ע�Ϳ����½�Y����
	LONG GetRightBottomYOfNote();
	void SetRightBottomYOfNote(LONG lY);

	//����嵥���½�X����
	LONG GetRightBottomXOfPartList();
	void SetRightBottomXOfPartList(LONG lX);

	//����嵥���½�Y����
	LONG GetRightBottomYOfPartList();
	void SetRightBottomYOfPartList(LONG lY);

	//�غ�λ�Ʊ����Ͻ�X����
	LONG GetRightTopXOfLoadAndDisp();
	void SetRightTopXOfLoadAndDisp(LONG lX);

	//�غ�λ�Ʊ����Ͻ�Y����
	LONG GetRightTopYOfLoadAndDisp();
	void SetRightTopYOfLoadAndDisp(LONG lY);

	LONG GetDimOffset();
	void SetDimOffset(LONG lOffset);

	LONG GetAttDOffsetX();
	void SetAttDOffsetX(LONG lX);

	LONG GetAttDOffsetY();
	void SetAttDOffsetY(LONG lY);

	LONG GetPt0X();
	void SetPt0X(LONG lX);

private:
	INT m_nStyle;
};

#endif // !defined(AFX_PHSBASEPOINTSETTING_H__ADC24A88_1B25_4AAE_9FF5_C8FB950137B6__INCLUDED_)
