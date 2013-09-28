// SelePartPHSData.h: interface for the SelePartPHSData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELEPARTPHSDATA_H__B822D4B6_8ABE_4B45_83AC_57F0A520C070__INCLUDED_)
#define AFX_SELEPARTPHSDATA_H__B822D4B6_8ABE_4B45_83AC_57F0A520C070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//����REST��ȫ�ֱ���:
class AnalysePHSPara  
{
public:
	float GetDblHangerRodDist();//modPHScal::DblHangerRodDist˫�����˼��
	long GetglClassID();//modPHScal::glClassID
	float Gett0();//modPHScal::t0�����¶�,C

	void SetDblHangerRodDist(float DblHangerRodDist);//modPHScal::DblHangerRodDist˫�����˼��
	void SetglClassID(long glClassID);//modPHScal::glClassID
	void Sett0(float t0);//modPHScal::t0�����¶�,C
	
	AnalysePHSPara();
	virtual ~AnalysePHSPara();

private:
	float m_DblHangerRodDist;//modPHScal::DblHangerRodDist˫�����˼��
	long m_glClassID;//modPHScal::glClassID
	float m_t0;//modPHScal::t0�����¶�,C

};

#endif // !defined(AFX_SELEPARTPHSDATA_H__B822D4B6_8ABE_4B45_83AC_57F0A520C070__INCLUDED_)
