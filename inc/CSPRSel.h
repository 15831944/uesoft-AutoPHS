// CSPRSel.h: interface for the CSPRSel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSPRSEL_H__0440E273_C345_4D45_9FC7_4BA215DB05C3__INCLUDED_)
#define AFX_CSPRSEL_H__0440E273_C345_4D45_9FC7_4BA215DB05C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class HDForceDist;
class HDCrude;
class LugDiaOfCSPR;
class CrudeTurnbuckleHDCz;
class CSPRSel  
{
public:
	CSPRSel();
	virtual ~CSPRSel();
	BOOL SelCSPRToTable(float& TmpPaz,float& tmpSelPJG4CSPRFiJ,long &mlfx,long &mlfx0);
private:
	//�õ������������ݱ�����Ҫ������ݼ�
	void GetHDForceDist(HDForceDist *pDist,float tmpCapacity,float tmpDist,int iSumSerialNum); 
	//�������ֵ
	void SetCSPRValue(int iDH,double tmpDist);
	//�õ��������ݱ�������Ҫ������ݼ�
	void GetHDCrude(HDCrude *pHDCrude);
	//�õ���������
	void GetLugDiaOfCSPR(LugDiaOfCSPR *pLug,double dbWeight,CString strCustomID,int iFjmin);
	//���ú�������֮ǰ�Ĳ���ѡ��ֱ��
	void SetAforeCompDia(int iFiJ);
	//��ʵ�ʵ����滻ƥ��ʽ
	void ReplaceCNTB(LugDiaOfCSPR *pLugFik);
	//�õ�ǰһ������Ĳ��볤��
	double GetPrePartInsertLen();
	//�������ݵ�TempCSLen�����
	void SetTempCSLen(HDCrude *pHDCrude,double tmpDist,double tmpSelOpLoad);
	//�������ݵ�ZB�����
	void SetResultZB(HDCrude *pCrude,LugDiaOfCSPR *pLugFik,long& mlfx,long& mlfx0);
	//�õ�ʵ�ʳߴ�
	double GetSigDim(HDCrude *pCrude,CrudeTurnbuckleHDCz *pCrudeTurn,LugDiaOfCSPR *pLugFik);

public:
	const CString& GettrtmpSQL0()
	{
		return m_strtmpSQL0;
	}
public:
	int m_iSEQofSPR; //��ǰ����������ģ���еĺ�������������
	int m_iSEQofPart; //��ǰ����������ģ���еĲ���������
	int m_i;          //��ǰ������������
	int m_iFirstCal; //ѡ�ʹ���
	CString m_strPartID; //ѡ�Ͳ�����ID��     
	CString m_strPrePartID; //ǰһ������ID�� 
	double m_dbSelPJG4CSPRPgz; //ѡ�͵Ĺ������� ��ӦPHS��tmpSelPJG4CSPRPgz
	int m_iSumSerialNum; //������
	CString m_strPtype; //ѡ�Ͳ�����CustomID
	double m_dbSelPJG4CSPRFiJ; //����ɽ���ĸֱ�������غ�ֵ
	int m_iNumPart; //�������ɸ���
	double m_dbtmpSelPJG; //������������غ�,��������ѡ��ʱ���������ܻ��ۼ���ȥ
	CString m_strtmpSQL0; //�ߴ�����ƥ������
	int m_inth; //��ǰ·��
	double m_dbmlRot; //��ת�Ƕ�
private: //�ڲ��õ�
	double m_dbSelPJG4CSPRFiK;

};

#endif // !defined(AFX_CSPRSEL_H__0440E273_C345_4D45_9FC7_4BA215DB05C3__INCLUDED_)
