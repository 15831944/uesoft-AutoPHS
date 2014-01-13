// OutputPDSFileTr.cpp: implementation of the OutputPDSFileTr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutputPDSFileTr.h"
#include "user.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OutputPDSFileTr::OutputPDSFileTr()
{
	m_pRs.CreateInstance( __uuidof(Recordset) );
}

OutputPDSFileTr::~OutputPDSFileTr()
{
	if ( m_pRs->GetState() == adStateOpen )
		m_pRs->Close();
}

//iVolumeID:����; mPHSNumberArray:���������֧���ܺ�  mGNumArray:�������֧���ܵĸ�������
//strFileName:��һ��֧���ܵ����·��������
void OutputPDSFileTr::OutPutText( int iVolumeID,CArray<int,int>& mPHSNumberArray,CArray<int,int>& mGNumArray,const CString& strFileName )
{
	//�ļ�·��
	CString strFilePath,strError;
	int iIndex = strFileName.ReverseFind( _T('\\') );
	if ( iIndex == -1 )
		return;
	strFilePath = strFileName.Left( iIndex+1 );

	_RecordsetPtr pRs,pOutCfgRs,pDataRs; //��¼�� ������ѯ��ļ�¼
	pRs.CreateInstance( __uuidof(Recordset) );//��¼���ĳ�ʼ��
	pOutCfgRs.CreateInstance( __uuidof(Recordset) );//
	pDataRs.CreateInstance( __uuidof(Recordset) );

	CString strFieldName,strOutName,strTableName,strMDBName;
	CString strPreTableNane,strPreMdbName;
	
	CString strSQL;
	int iSize = mPHSNumberArray.GetSize();//����ĸ���
	int iPHSNumber,iGNum;
	CString strCustomID,strBH,strOutValue,strStrand;
	try
	{
		//���֧����
		for ( int i=0; i<iSize; i++ )
		{
			iPHSNumber = mPHSNumberArray.GetAt(i);
			//�ļ���
			CStdioFile stdFile;
			CString strPHSFileName;
			if ( i== 0 )
				strPHSFileName = strFileName;
			else
			{
				strPHSFileName.Format( _T("%d"),iPHSNumber );
				strPHSFileName = strFilePath + strPHSFileName + _T(".txt");
			}
			BOOL bRt = stdFile.Open( strPHSFileName, CStdioFile::modeCreate|CStdioFile::modeWrite );
			if ( !bRt )
			{
				strError.Format("���ļ�%sʧ��",strPHSFileName);
				AfxMessageBox( strError );
				continue;
			}

			
			iGNum = mGNumArray.GetAt(i);
			strSQL.Format( _T("SELECT * FROM ZB where VolumeID=%d AND ZDJH = %d AND IsSAPart = FALSE ORDER BY recno "),iVolumeID, iPHSNumber );
			//�򿪼�¼�� ��һ��������sql��� �ڶ������������ݿ����� 
			pRs->Open( _variant_t(strSQL),(IDispatch*)::conPRJDB ,adOpenKeyset,adLockOptimistic,adCmdText  );
			
			//�õ�ID��
			CString strID;
			int iParalleledNum = 0;
			if ( !pRs->adoEOF )
			{
				strID = vtos( pRs->GetCollect( _variant_t("ID") ) );
				strCustomID = vtos( pRs->GetCollect( _variant_t("CustomID") ) );
				if ( strID == strCustomID )
					strStrand = _T("-XB");
				else
				    strStrand = _T("-HD");
				iParalleledNum = GetParalleledNum( strID );
				/*�ļ��ֿ�������Ҫ������
				if ( i != 0 )
					stdFile.WriteString(_T("\n"));
					*/

				//�õ�֧�����ͣ���֧�����ǵ���,�ܲ����������
				CString strType;
				double dPB=0.0, dRB = 0.0,dDW = 0.0;
				GetZDJInfo( iVolumeID,iPHSNumber,strType,dPB,dRB,dDW );
				if ( iParalleledNum == 2 )
					strOutValue.Format( _T("ZDJINFO%s,%d,SLG%s(1)PB=%g(2)RB=%g(3)DW=%g\n"),strStrand,iPHSNumber,strType,dPB,dRB,dDW );
				else
					strOutValue.Format( _T("ZDJINFO%s,%d,DLG%s(1)PB=%g(2)RB=%g(3)DW=%g\n"),strStrand,iPHSNumber,strType,dPB,dRB,dDW );
				stdFile.WriteString(strOutValue);
			}


			CString strParllText; //������֧����������ı�
			strOutValue.Empty();
			BOOL bFirst = TRUE;
			//��ʼ���Ҫ�����ÿ�����
			while ( !pRs->adoEOF )
			{
				strCustomID = vtos( pRs->GetCollect( _variant_t("CustomID") ) );
				strBH = vtos( pRs->GetCollect( _variant_t("BH") ) );
				strOutValue = GetCustomIDOutText( iVolumeID,iPHSNumber,strBH,strCustomID,pRs );
				
				pRs->MoveNext();

				if ( iParalleledNum == 2) //˫�����ǹܲ�
				{
					if ( bFirst ) //�ܲ�
					{
						strOutValue = _T("ZDJELE") + strOutValue;
					}
					else ////�����˫��,��¼���ı�,�ܲ�����Ҫ
					{
						if ( pRs->adoEOF )
						{
							if ( iGNum == 2 ) //��������
							{
								strParllText = strParllText + _T("ZDJELER") + strOutValue;
							}
						}
						else
						{
							strParllText = strParllText +  _T("ZDJELER") + strOutValue;
						}
						strOutValue = _T("ZDJELEL") + strOutValue;
					}	
				}
				else //����֧����
				{
					strOutValue = _T("ZDJELEL") + strOutValue;
				}
				stdFile.WriteString(strOutValue);
				bFirst = FALSE;

			}
			pRs->Close();
			if ( iParalleledNum == 2 ) //˫��
			{
				stdFile.WriteString(strParllText);
			}
			stdFile.Close();

		}
	}
	catch (_com_error e)
	{
		AfxMessageBox( e.Description() );
	}
	if ( pRs->GetState() == adStateOpen )
		pRs->Close();
	if ( pOutCfgRs->GetState() == adStateOpen )
		pOutCfgRs->Close();
	if ( pDataRs->GetState() == adStateOpen )
		pDataRs->Close();
	AfxMessageBox( _T("�����ļ��ɹ�(��֧���ܵı��Ϊ�ļ���)") );
	
}

//�õ�֧�������� ����ֵ strType 1:��֧�� 2:����  dPB:�ܲ����  dRB:�������
BOOL OutputPDSFileTr::GetZDJInfo( int iVolumeID,int iPHSNumber,CString& strType,double& dPB,double& dRB,double& dDW ) 
{
	BOOL bRtVlaue = FALSE;
	CString strSQL;
	_RecordsetPtr pRs; //��¼�� ������ѯ��ļ�¼
	pRs.CreateInstance( __uuidof(Recordset) );//��¼���ĳ�ʼ��

	try
	{
		strSQL.Format( _T("SELECT * FROM ZA where VolumeID=%d AND ZDJH = %d"),iVolumeID, iPHSNumber );
		//�򿪼�¼�� ��һ��������sql��� �ڶ������������ݿ����� 
		pRs->Open( _variant_t(strSQL),(IDispatch*)::conPRJDB ,adOpenKeyset,adLockOptimistic,adCmdText  );
		if ( !pRs->adoEOF )
		{
			dPB = vtof( pRs->GetCollect( _variant_t( _T("DH1") )) ); //�ܲ�
			dRB = vtof( pRs->GetCollect( _variant_t( _T("GH1") )) ); //����
			dDW = vtof( pRs->GetCollect( _variant_t( _T("DJ1") )) ); //�⾶
			if ( dRB > dPB ) //����
				strType = _T("DJ");
			else
				strType = _T("ZJ");
			bRtVlaue = TRUE;
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox( e.Description() );
	}
	if ( pRs->GetState() == adStateOpen )
		pRs->Close();
	return bRtVlaue;

}

int OutputPDSFileTr::GetParalleledNum( const CString& strID ) //�õ�֧���ܵĲ����� 
{
	CString strSql;
	strSql.Format( _T("SELECT * FROM CrudePAidenCN Where ID = '%s'"),strID );
	_RecordsetPtr pRs; //��¼�� ������ѯ��ļ�¼
	pRs.CreateInstance( __uuidof(Recordset) );//��¼���ĳ�ʼ��
	int iRt =0 ;
	try
	{
		pRs->Open( _variant_t(strSql),(IDispatch*)::conZDJCrude ,adOpenKeyset,adLockOptimistic,adCmdText );
		if ( !pRs->adoEOF )
		{
			iRt = vtoi( pRs->GetCollect( _variant_t("ParalleledNum") ) );
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox( e.Description() );
	}
	if ( pRs->GetState() == adStateOpen )
		pRs->Close();
	return iRt;
}

//�õ����벿��������ı�
//iPHSNumber:֧�мܺ�,  strBH:�㲿���ı��ֵ  strCustomID:�겿���Ĵ���
//pZBRs: ��ǰ���������Ӽ�¼,��Ϊ��Щֵ��Ҫ��ZB��ǰ������¼��ȡ
CString OutputPDSFileTr::GetCustomIDOutText( int iVolumeID,int iPHSNumber,const CString& strBH, const CString& strCustomID,_RecordsetPtr pZBRs ) 
{
	CString strSQL;
	_RecordsetPtr pOutCfgRs;
	pOutCfgRs.CreateInstance( __uuidof(Recordset) );
	///	strSQL.Format(_T("select BH,SIZE2 from ZB inner join crudePA on ZB.CustomID=crudePA.CustomID"););
	strSQL.Format( _T("SELECT * FROM OutPutPDSCfg WHERE CustomID = '%s' ORDER BY OutSEQ"),strCustomID );
	pOutCfgRs->Open( _variant_t(strSQL),(IDispatch*)::conPHSConfig ,adOpenKeyset,adLockOptimistic,adCmdText );
	int iIndex = 0;
	CString strText,strTmp;
	strText.Format( _T(",%d"),iPHSNumber );
				
	CString strFieldName,strOutName,strTableName,strMDBName,strOutValue;
	CString strValue1,strValue2,strMdb1,strMdb2,strTableName1,strTableName2,strFieldName1,strFieldName2;
	CString strMidKey1,strMidKey2;
	int iMid = 0;
	if ( pOutCfgRs->adoEOF )
	{
		CString strError;
		strError.Format( _T("OutPutPDScfg����û������%s�������Ϣ������"),strCustomID );
		AfxMessageBox( strError );
	}
	while( !pOutCfgRs->adoEOF )
	{
		iIndex ++;
		strFieldName = vtos( pOutCfgRs->GetCollect( _variant_t("FieldName") ) );
		strOutName = vtos( pOutCfgRs->GetCollect( _variant_t("OutName") ) );
		strTableName = vtos( pOutCfgRs->GetCollect( _variant_t("FieldTableName") ) );
		strMDBName = vtos( pOutCfgRs->GetCollect( _variant_t("FieldMDBName") ) );
					
		//����ͬ������ֵ
		if ( GetStrAtCountEx( strMDBName ) > 1 )
		{
			strTableName1 = GetStrAtEx(strTableName,1);
			strFieldName1 = GetStrAtEx(strFieldName,1);
			strMdb1 = GetStrAtEx(strMDBName,1);
			strValue1 = GetValue( iVolumeID,iPHSNumber,strBH,strTableName1,strFieldName1,strMdb1,pZBRs );
						
			strTableName2 = GetStrAtEx(strTableName,2);
			strFieldName2 = GetStrAtEx(strFieldName,2);
			strMdb2 = GetStrAtEx(strMDBName,2);
			strValue2 = GetValue( iVolumeID,iPHSNumber,strBH,strTableName2,strFieldName2,strMdb2,pZBRs);
						
			strOutValue.Format( _T("%g"),atof(strValue1) - atof(strValue2) );
		}
		else
		{
			//�����ֵ��ȡ���ַ�ֵ
			if ( strFieldName.Find(_T("����ͺ�")) != -1 )
			{
				strOutValue = strCustomID;
			}
			else if ( strFieldName.Find(_T("���ַ�����")) != -1 )
			{
				strFieldName1 = GetStrAtEx( strFieldName,1 );
				strOutValue = GetValue( iVolumeID,iPHSNumber,strBH,strTableName,strFieldName1,strMDBName,pZBRs );
				strMidKey1 =  GetStrAtEx( strFieldName,3 ); //��һ���ָ��ַ�
				strMidKey2 =  GetStrAtEx( strFieldName,4 ); //�ڶ����ָ��ַ�
				iMid = atoi( strMidKey2 );
				strOutValue.Replace( strMidKey1,_T(",") );
				strOutValue = GetStrAtEx( strOutValue,iMid ); //ȡ�ڼ����ַ�ֵ
			}
			else if ( strFieldName.Find(_T("˫�ַ�����")) != -1 )
			{
				strFieldName1 = GetStrAtEx( strFieldName,1 );
				strOutValue = GetValue( iVolumeID,iPHSNumber,strBH,strTableName,strFieldName1,strMDBName,pZBRs );
				strMidKey1 =  GetStrAtEx( strFieldName,3 ); //��һ���ָ��ַ�
				strMidKey2 =  GetStrAtEx( strFieldName,4 ); //�ڶ����ָ��ַ�
				strOutValue.Replace( strMidKey1,_T(",") );
				if( !strMidKey2.IsEmpty() )
					strOutValue.Replace( strMidKey2,_T(",") );
				strOutValue = GetStrAtEx( strOutValue,2 );
			}
			else
				strOutValue = GetValue( iVolumeID,iPHSNumber,strBH,strTableName,strFieldName,strMDBName,pZBRs );
		}
		
		if ( strOutName == _T("bh") ) //���
		{
			strOutValue.TrimLeft();
			strOutValue.TrimRight();
			strOutValue.Replace( _T(" "),_T("-") );
			strOutValue.Replace( _T("��"),_T("") );
		}
		strTmp.Format( _T("(%d)%s=%s"),iIndex,strOutName,strOutValue );
		strText = strText + strTmp;
					
		pOutCfgRs->MoveNext();
	}
	pOutCfgRs->Close();

	if ( !strText.IsEmpty() )
		strText = strText + _T("\n");
	return strText;
	

}


/*
CString OutputPDSFileTr::GetStrandStr( const CString& strCustomID) //�õ����ݿ��׼�ַ�
{
	_RecordsetPtr pOutCfgRs;
	pOutCfgRs.CreateInstance( __uuidof(Recordset) );
	strSQL.Format( _T("SELECT * FROM OutPutPDSCfg WHERE CustomID = '%s' ORDER BY OutSEQ"),strCustomID );
	try
	{
		pOutCfgRs->Open( _variant_t(strSQL),(IDispatch*)::conPHSConfig ,adOpenKeyset,adLockOptimistic,adCmdText );
	}
	catch( _com_error e )
	{
		AfxMessageBox( e.Description() );
	}
	if ( pOutCfgRs->GetState() == adStateOpen )
		pOutCfgRs->Close();


}
*/


//�õ������㲿���Ŵ�������ֶε�ֵ
CString OutputPDSFileTr::GetValue( int iVolumeID,int iPHSNumber, const CString& strBH,const CString& strTableName,const CString& strFieldName,const CString& strmdbName,_RecordsetPtr pZBRs )
{
	//�����������ݿ�
	CString strRt;
	if ( strmdbName == _T("AllPrjDB.mdb") && strTableName == _T("ZB"))
	{
		strRt = vtos( pZBRs->GetCollect( _variant_t( strFieldName ) ) ); //ZB���еļ�¼,���������ļ���ֻ��ZB��
		return strRt;
	}
	else if ( strmdbName == _T("AllPrjDB.mdb") && strTableName == _T("ZA") )
	{
		if ( m_pRs->GetState() == adStateOpen )
			m_pRs->Close();
		CString strSql;
		strSql.Format( _T("SELECT * FROM %s WHERE VolumeID = %d AND ZDJH = %d"),strTableName,iVolumeID,iPHSNumber );
		m_pRs->Open( _variant_t(strSql),(IDispatch*)::conPRJDB ,adOpenKeyset,adLockOptimistic,adCmdText );

		m_strBH = strBH;
		m_strTableName = strTableName;
		m_strFieldName = strFieldName;
		m_strMdbName = strmdbName;
	}
	else if ( strBH != m_strBH || strTableName != m_strTableName || strFieldName != m_strFieldName || strmdbName != m_strMdbName )
	{
		if ( m_pRs->GetState() == adStateOpen )
			m_pRs->Close();
		CString strSql;
		strSql.Format( _T("SELECT * FROM %s WHERE BH = '%s'"),strTableName,strBH );
		m_pRs->Open( _variant_t(strSql),(IDispatch*)::conZDJCrude ,adOpenKeyset,adLockOptimistic,adCmdText );
		
		m_strBH = strBH;
		m_strTableName = strTableName;
		m_strFieldName = strFieldName;
		m_strMdbName = strmdbName;
	}
	
	if ( !m_pRs->adoEOF )
		strRt = vtos( m_pRs->GetCollect( _variant_t( strFieldName ) ) );
	else
	{
		CString strError;
		strError.Format( _T("�ڱ�%s��û���ұ�BH=%s�ļ�¼����鿴�����Ƿ���ȷ"),strTableName,strBH );
		AfxMessageBox( strError );
	}

	return strRt;
}



