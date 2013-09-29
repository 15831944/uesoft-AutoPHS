
#include "stdafx.h"
#include "IPAHSel.h"
#include "SelParameter.h"
#include "SupportCompData.h"
#include "CrudePA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
����:�����ݿ��в��Һ��ʵĲ�����������(iPAXZ1_XZ3��iPAh)(iPAs��iPASS��iPAGS�ĵ�һ��ѡ��)
*/
void IPAHSel::GetSearchComp(SupportCompData *pObj,ParaBase* pPara)
{
	CrudePA *pCrudePA = static_cast<CrudePA *> (pObj);
	IPAHParaC *pParaPA = static_cast<IPAHParaC *> (pPara);
	CString strSql,strTemp;
	for (int k = 0 ;k<= (int)pParaPA->GetDWdelta();k++)
	{
		double dbDw = pParaPA->GetDj() * (1 - k * 0.01);
		double dbDw1 = pParaPA->GetDj() * (1 + k * 0.01);
		//ֱ������
		strSql.Format(_T("SELECT * FROM [%s]  WHERE %s >= %g AND [%s]<= %g"),
			pCrudePA->GetTableName(),pCrudePA->GetDWField(),dbDw,
			pCrudePA->GetDWField(),dbDw1);
		
		strTemp.Format(_T("AND %s >= %g"),
			pCrudePA->GetPmaxField(),(float)(pParaPA->GetSelPJG() /pParaPA->GetiNumPart() ));
		strSql += strTemp;
		
		//�¶�����
		strTemp.Format( _T(" AND %s>=%g AND trim(%s)=\'%s\'"),
			pCrudePA->GetTJField(),pParaPA->GetT0(),
			pCrudePA->GetCustomIDField(),pParaPA->GetCustomID() );
		strSql += strTemp;

		//����
		strTemp.Format(_T("ORDER BY %s,%s,%s,%s"),
			pCrudePA->GetTJField(),pCrudePA->GetDWField(),
			pCrudePA->GetPmaxField(),pCrudePA->GetWeightField() );

		strSql += strTemp;
		pCrudePA->Initialize(strSql);
		
	}
}