// PSAResult.h: interface for the PSAResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSARESULT_H__9E822B28_CD4F_4C74_8F0B_7C0AA28C63AF__INCLUDED_)
#define AFX_PSARESULT_H__9E822B28_CD4F_4C74_8F0B_7C0AA28C63AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//��������Ӧ�������Ľ�������еĺ��ء�λ�ơ��ܾ���
class PSAResultPara  
{
public:
	//�̶�֧�ܼ���ģ������ϵ������̬���ܵ���,kgf �Ѿ������������ת��(start)
	float pxlc;//modPHScal::pxlc;
	float pylc;//modPHScal::pylc;
	float pzlc;//modPHScal::pzlc;
	float pxrc;//modPHScal::pxrc;
	float pyrc;//modPHScal::pyrc;
	float pzrc;//modPHScal::pzrc;
	float mxlc;//modPHScal::mxlc;
	float mylc;//modPHScal::mylc;
	float mzlc;//modPHScal::mzlc;
	float mxrc;//modPHScal::mxrc;
	float myrc;//modPHScal::myrc;
	float mzrc;//modPHScal::mzrc;
	//�̶�֧�ܼ���ģ������ϵ������̬���ܵ���,kgf �Ѿ������������ת��(end)
	float	t0;;//�¶�
	
	PSAResultPara();
	virtual ~PSAResultPara();

};

#endif // !defined(AFX_PSARESULT_H__9E822B28_CD4F_4C74_8F0B_7C0AA28C63AF__INCLUDED_)
