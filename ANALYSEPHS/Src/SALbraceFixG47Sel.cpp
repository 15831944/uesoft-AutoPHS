// SALbraceFixG47Sel.cpp: implementation of the SALbraceFixG47Sel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SALbraceFixG47Sel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SALbraceFixG47Sel::SALbraceFixG47Sel()
{

}

SALbraceFixG47Sel::~SALbraceFixG47Sel()
{

}

//	case iSALbraceFixG47: Cphs(3538,4391)
//	case iSALbraceFixG48: //����֧��              
bool SALbraceFixG47Sel::SelSALbraceFixG47(CString &Ptype)
{
	bool flag=0;
	try
	{
		if((Ptype[i]!=_T("SJ8")))
		{
			//��ʽ�б����ĵ�λ:mvBWidth,cm;PAfixH,mm->cm; modPHScal::gdw,mm->cm; ��������,kgf.m->kgf.cm; ������,kgf->kgf.
			//ǿ�ȼ���:����Ժ�̶�֧�ܸ���G47(ClassID=iSALbraceFixG47=4),G48(ClassID=iSALbraceFixG48=10)
			if( modPHScal::glClassID == iSALbraceFixG47 )
			{
				mvBWidth = 30;
			}
			else 
			{
				vTmp=rsza->GetCollect(_T("A01"));
				mvBWidth = vtof(vTmp)/ 10;
			}
			bool bLongVertPipe;//�ǳ���ֱ�ܵ��ϵĸ��Ե���
			bLongVertPipe=vtob(rsza->GetCollect(_T("ifLongVertPipe")));
			//��ʼѭ�����Һ��ʵĹ̶�֧��G47��G48
			//��۸����Ա���F1ToWx1
			SQLx = _T("SELECT * FROM SSteelPropertyCS WHERE F1toWx1 is not null ORDER BY ID");
			if(rsTmp.IsOpen())
				rsTmp.Close();
			rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;	
			rsTmp.Open(dbOpenSnapshot,SQLx);
			//���������������Ժ֧�����ֲ�1983���117ҳ����,��"GDW1>="��Ϊ"GDW1=". lgb and pfg20050927
			SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1= ") + ftos(modPHScal::gdw) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
			if(rsX.IsOpen())
				rsX.Close();
			rsX.Open(dbOpenSnapshot,SQLx);
			if( rsX.IsEOF() && rsX.IsBOF() )
				;
			else
			{
				while (!rsX.IsEOF())
				{
					rsX.GetFieldValue(_T("P1"),vTmp1);
					rsX.GetFieldValue(_T("PmaxSF"),vTmp3);
					sngSAfixP=vtof(vTmp3);
					if(rsTmp.FindFirst(_T("BH=\'")+vtos(vTmp1)+_T("\'")))
					{
						rsTmp.GetFieldValue(_T("F1ToWx1"),vTmp2);
						F1ToWx1=vtof(vTmp2);
						sngSAfixPl = 1.3 * modPHScal::pylc / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * modPHScal::pxlc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * modPHScal::pzlc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 1 / 4 * F1ToWx1) * modPHScal::mxlc * 100 + modPHScal::mylc * 100 / mvBWidth + 2.6 / mvBWidth * modPHScal::mzlc * 100;
						sngSAfixPr = 1.3 * modPHScal::pyrc / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * modPHScal::pxrc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * modPHScal::pzrc + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 1 / 4 * F1ToWx1) * modPHScal::mxrc * 100 + modPHScal::myrc * 100 / mvBWidth + 2.6 / mvBWidth * modPHScal::mzrc * 100;
						//��������Ժ֧�����ֲ�1983���116ҳ���ͣ������ء����ķ�������⡣
						if(1)
						{
							
							sngSAfixPl = 1.3 * fabs(modPHScal::pylc) / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * fabs(modPHScal::pxlc) + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * fabs(modPHScal::pzlc) + (1.95/ (modPHScal::gdw/10) + 1 / 4 * F1ToWx1) * fabs(modPHScal::mxlc) * 100 + fabs(modPHScal::mylc) * 100 / mvBWidth + 2.6 / mvBWidth * fabs(modPHScal::mzlc) * 100;
							sngSAfixPr = 1.3 * fabs(modPHScal::pyrc) / iNumPart * (bLongVertPipe ? iNumPart : 1) - 1 / mvBWidth * (2.6 * modPHScal::PAfixH / 10 - 1.25 * modPHScal::gdw / 10) * fabs(modPHScal::pxrc) + (1.95 * modPHScal::PAfixH / modPHScal::gdw + 0.5 + 1 / 4 * F1ToWx1 * modPHScal::PAfixH / 10) * fabs(modPHScal::pzrc) + (1.95/ (modPHScal::gdw/10) + 1 / 4 * F1ToWx1) * fabs(modPHScal::mxrc) * 100 + fabs(modPHScal::myrc) * 100 / mvBWidth + 2.6 / mvBWidth * fabs(modPHScal::mzrc) * 100;							
						}
						//���������<0,ȡ�����ֵ��Ϊ������ by ligb on 2004.09.11
						if(sngSAfixPl<0) sngSAfixPl=fabs(sngSAfixPl);
						if(sngSAfixPr<0) sngSAfixPr=fabs(sngSAfixPr);

						if( (sngSAfixP > sngSAfixPl) && (sngSAfixP > sngSAfixPr) )
						{
							//��̬����̬��������������
							SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + ftos(modPHScal::gdw) + _T(" AND PmaxSF>=")
								+ ftos(sngSAfixP)+ _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
							break;
						}
						else
						{
							//������
							rsX.MoveNext();
						}
					}
				}
				if(rsX.IsEOF())
				{
					//���������������Ժ֧�����ֲ�1983���117ҳ����,��"GDW1>="��Ϊ"GDW1=".lgb and pfg20050927
					SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1= ") + ftos(modPHScal::gdw) + _T(" AND PmaxSF>=")
						+ ftos((sngSAfixPl>sngSAfixPr)?sngSAfixPl:sngSAfixPr)+ _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
				}
			}
		}
		else
		{
			//��ʼǿ�ȼ���:����Ժ�̶�֧�ܸ���˫�����Ǽ�SJ8(ClassID=iSALbraceFixG47=4)
			
			//�ܲ����������Ĳ��ϣ���ϸ�µ������ǰ��ղ��Ϲ淶ѡ�����ͬ�������ֱ�ѡ�񣻴˴��򵥵�ȡĬ��ֵ��
			SQLx.Format(_T("SELECT * FROM SpecificationOfMaterial WHERE ClassIndex=%d AND ID=\'default\' AND tmin<=%g AND %g<tmax ORDER BY tmin,tmax,SEQ"),modPHScal::giClassIndex,modPHScal::gnConnectTJ,modPHScal::gnConnectTJ);
			if(rs3.IsOpen())
				rs3.Close();
			rs3.Open(dbOpenSnapshot,SQLx);
			if(rs3.IsEOF() && rs3.IsBOF())
			{
				//��%s��%s����ѡ��淶��û��%s�ֶ�ֵΪĬ��ֵ%s�ļ�¼(�ǹܲ��������ѡ�����)
				sTmp.Format(IDS_NoDefaultInCustomIDInSpecificationOfMaterial,rs3.m_pDatabase->GetName(),_T("SpecificationOfMaterial"),_T("ID"),_T("default"));
				throw sTmp;
			}
			else
			{
				rs3.GetFieldValue(_T("Material"),vTmp);
				m_strMaterial=vtos(vTmp);
			}
			
			//����Ӧ��
			float Sigmat=0.0,Sigmat100=0.0,Sigma0=0.0;
			modPHScal::StressOfMaterial(m_strMaterial,modPHScal::t0,Sigmat);
			modPHScal::StressOfMaterial(m_strMaterial,100,Sigmat100);
			modPHScal::StressOfMaterial(m_strMaterial,20,Sigma0);
			//�򿪹̶�֧�ܼ��㹫ʽ��������
			SQLx = _T("SELECT * FROM CalFormulaOfFixPASA WHERE CustomID=\'") + Ptype[i] + _T("\' ORDER BY CustomID,SEQ");
			if(rsCal.IsOpen())
				rsCal.Close();
			rsCal.m_pDatabase=&EDIBgbl::dbSACal;	//20071103 "dbSORT" ��Ϊ "dbSACal"
			rsCal.Open(dbOpenSnapshot,SQLx);
			if(rsCal.IsEOF() && rsCal.IsBOF())
			{
				//�����ݿ�%s�еĸ���ǿ�ȼ��㹫ʽ��%sû���κμ�¼��
				sTmp.Format(IDS_NullTableCalFormulaOfFixPASA,EDIBgbl::dbSACal.GetName(),_T("CalFormulaOfFixPASA"));//20071103 "dbSORT" ��Ϊ "dbSACal"
				throw sTmp;
			}
			//�򿪸���ǿ�ȼ��㹫ʽ��������
			SQLx = _T("SELECT * FROM tmpCalFixPhs");
			if(rsTmp1.IsOpen())
				rsTmp1.Close();
			rsTmp1.m_pDatabase=&EDIBgbl::dbPRJ;	
			//rsTmp1��Ҫ�޸Ĺ���
			rsTmp1.Open(dbOpenDynaset,SQLx);
			//�򿪲۸����Ա�����
			SQLx = _T("SELECT * FROM SSteelPropertyCS ORDER BY ID");
			if(rsTmp.IsOpen())
				rsTmp.Close();
			rsTmp.m_pDatabase=&modPHScal::dbZDJcrude;	
			rsTmp.Open(dbOpenSnapshot,SQLx);
			//�򿪽Ǹ����Ա�����
			SQLx = _T("SELECT * FROM SSteelPropertyLS ORDER BY ID");
			if(rs1.IsOpen())
				rs1.Close();
			rs1.m_pDatabase=&modPHScal::dbZDJcrude;	
			rs1.Open(dbOpenSnapshot,SQLx);
			//ȡ�������е����㶨λ���������м�¼,��PmaxSF�����������
			SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + ftos(modPHScal::gdw) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' ORDER BY GDW1,PmaxSF");
			if(rsX.IsOpen())
				rsX.Close();
			rsX.Open(dbOpenSnapshot,SQLx);
			if( rsX.IsEOF() && rsX.IsBOF())
				;
			else
			{				
				CString strP1,strP2,strP3;
				int n,dh;
				float bb,PL3,K;
				int i4,i5;
				float mfpx,mfpy,mfpz;//���������̬����kgf
				float mfmx,mfmy,mfmz;//���������̬���أ�kgf.m
				CString F1sa,Wyh1sa,Wzh1sa;
				CString F2sa,Wyh2sa,Wzh2sa;
				CString SIGMA1sa,SIGMA1saMax,TAO1sa,TAO1saMax,TAOx1sa,TAOy1sa,TAOz1sa,lamda1;
				CString SIGMA2sa,SIGMA2saMax,TAO2sa,TAO2saMax,TAOx2sa,TAOy2sa,TAOz2sa,lamda2;
				float mfSIGMA1sa,mfSIGMA1saMax,mfTAO1sa,mfTAO1saMax,mfLamda1,mfFi1;
				float mfSIGMA2sa,mfSIGMA2saMax,mfTAO2sa,mfTAO2saMax,mfLamda2,mfFi2;
				_RecordsetPtr rs2;
				
				//modPHScal::PAfixH=320.0;//����ɲ��Ի���Ժ����
				
				rsTmp1.Edit();
				//rsTmp1.SetFieldValue(_T("CustomIDsa"),COleVariant(Ptype(i)));
				rsTmp1.SetFieldValue(_T("CustomIDsa"),COleVariant(sPartID));
				rsTmp1.SetFieldValue(_T("H"),COleVariant(modPHScal::PAfixH));
				rsTmp1.SetFieldValue(_T("gdw2"),COleVariant(modPHScal::gdw));
				rsTmp1.SetFieldValue(_T("tj"),COleVariant(modPHScal::t0));
				//����Ӧ����λkgf/cm2->kgf/mm2
				rsTmp1.SetFieldValue(_T("SIGMAjt"),COleVariant(Sigmat/100.0));
				rsTmp1.SetFieldValue(_T("SIGMAj20"),COleVariant(Sigma0/100.0));
				rsTmp1.SetFieldValue(_T("SIGMAj100"),COleVariant(Sigmat100/100.0));
				rsTmp1.Update();
				//����һ��ӿ���£�������ֹ������
				
				//��ù���3
				rsX.GetFieldValue(_T("P3"),vTmp);
				strP3=vtos(vTmp);	
				rsTmp1.Edit();
				rsTmp1.SetFieldValue(_T("P3"),vTmp);
				//�˴�ֻ��Ӧ�й���׼�����ʻ�ʱҪ�޸Ĵ���
				n=strP3.Find((_T("x")));
				//����3�ȿ�bb,unit:mm
				bb=_tcstod(strP3.Mid(1,n-1),NULL);
				rsTmp1.SetFieldValue(_T("bb"),COleVariant(bb));
				//������͸�P1
				rsX.GetFieldValue(_T("P1"),vTmp);
				strP1=vtos(vTmp);
				rsTmp1.SetFieldValue(_T("P1"),vTmp);
				//��ù���2�͸�P2
				rsX.GetFieldValue(_T("P2"),vTmp);
				strP2=vtos(vTmp);
				rsTmp1.SetFieldValue(_T("P2"),vTmp);
				//˫�۸���Ե������PL3,mm
				rsX.GetFieldValue(_T("PL3"),vTmp);
				PL3=vtof(vTmp);
				//rsX�е�PL3��CHAR(20),��rsTmp1�е�PL3��REAL,�ɼ������ܹ��Զ�ת����
				rsTmp1.SetFieldValue(_T("PL3"),vTmp);
				//�۸ֺ���߶�K,mm
				rsX.GetFieldValue(_T("size2"),vTmp);
				K=vtof(vTmp);
				rsTmp1.SetFieldValue(_T("K"),vTmp);
				rsTmp1.Update();
				//���DH,mm
				rsX.GetFieldValue(_T("dh"),vTmp);
				dh=vtoi(vTmp);				
				
				//�ȼ�����̬
				
				bool bEOF=false;
				bool bErr=false;//����һ�����������㣬��Ϊtrue
				do
				{
					if(!rsTmp.FindFirst(_T("BH=\'")+strP1+_T("\'")))
					{
						//�ڲ۸����Ա�%s��û�ҵ��۸��ͺ�%s
						sTmp.Format(GetResStr(IDS_NotFoundThisCSNo),_T("SSteelPropertyCS"),strP1);
						throw sTmp;
					}
					//����1�۸ֹ��Ծ�Ix,cm4->mm4
					rsTmp.GetFieldValue(_T("Ix"),vTmp);
					float J1y=vtof(vTmp)*10000.0;
					rsTmp1.Edit();
					rsTmp1.SetFieldValue(_T("J1y"),COleVariant(J1y));
					//����1�۸ֹ��Ծ�Iy,cm4->mm4
					rsTmp.GetFieldValue(_T("Iy"),vTmp);
					float J1z=vtof(vTmp)*10000.0;
					rsTmp1.SetFieldValue(_T("J1z"),COleVariant(J1z));
					//����1�۸���С���Ծ�,cm4->mm4
					rsTmp1.SetFieldValue(_T("Imin1"),COleVariant(J1z>J1y?J1y:J1z));
					//����1�۸ֿ����Wx,cm3->mm3
					rsTmp.GetFieldValue(_T("Wx"),vTmp);
					float W1y=vtof(vTmp)*1000.0;
					rsTmp1.SetFieldValue(_T("W1y"),COleVariant(W1y));
					//����1�۸ֿ����Wy,cm3->mm3
					rsTmp.GetFieldValue(_T("Wy"),vTmp);
					float W1z=vtof(vTmp)*1000.0;
					rsTmp1.SetFieldValue(_T("W1z"),COleVariant(W1z));
					//����1�۸ֽ����S,cm2->mm2
					rsTmp.GetFieldValue(_T("S"),vTmp);
					float F1=vtof(vTmp)*100.0;
					rsTmp1.SetFieldValue(_T("F1"),COleVariant(F1));
					//����1�۸ָ߶�h,mm
					rsTmp.GetFieldValue(_T("h"),vTmp);
					float h1=vtof(vTmp);
					rsTmp1.SetFieldValue(_T("h1"),vTmp);
					//����1�۸��ȿ�b,mm
					rsTmp.GetFieldValue(_T("b"),vTmp);
					float b1=vtof(vTmp);
					rsTmp1.SetFieldValue(_T("b1"),vTmp);
					rsTmp1.Update();
					
					if(!rsTmp.FindFirst(_T("BH=\'")+strP2+_T("\'")))
					{
						//�ڲ۸����Ա�%s��û�ҵ��۸��ͺ�%s
						sTmp.Format(GetResStr(IDS_NotFoundThisCSNo),_T("SSteelPropertyCS"),strP2);
						throw sTmp;
					}
					//����2�۸ֹ��Ծ�Ix,cm4->mm4
					rsTmp.GetFieldValue(_T("Ix"),vTmp);
					float J2y=vtof(vTmp)*10000.0;
					rsTmp1.Edit();
					rsTmp1.SetFieldValue(_T("J2y"),COleVariant(J2y));
					//����2�۸ֹ��Ծ�Iy,cm4->mm4
					rsTmp.GetFieldValue(_T("Iy"),vTmp);
					float J2z=vtof(vTmp)*10000.0;
					rsTmp1.SetFieldValue(_T("J2z"),COleVariant(J2z));
					//����1�۸���С���Ծ�,cm4->mm4
					rsTmp1.SetFieldValue(_T("Imin2"),COleVariant(J2z>J2y?J2y:J2z));
					//����2�۸ֿ����Wx,cm3->mm3
					rsTmp.GetFieldValue(_T("Wx"),vTmp);
					float W2y=vtof(vTmp)*1000.0;
					rsTmp1.SetFieldValue(_T("W2y"),COleVariant(W2y));
					//����2�۸ֿ����Wy,cm3->mm3
					rsTmp.GetFieldValue(_T("Wy"),vTmp);
					float W2z=vtof(vTmp)*1000.0;
					rsTmp1.SetFieldValue(_T("W2z"),COleVariant(W2z));
					//����2�۸ֽ����S,cm2->mm2
					rsTmp.GetFieldValue(_T("S"),vTmp);
					float F2=vtof(vTmp)*100.0;
					rsTmp1.SetFieldValue(_T("F2"),COleVariant(F2));
					//����2�۸ָ߶�h,mm
					rsTmp.GetFieldValue(_T("h"),vTmp);
					float h2=vtof(vTmp);
					rsTmp1.SetFieldValue(_T("h2"),COleVariant(h2));
					//����2�۸��ȿ�b,mm
					rsTmp.GetFieldValue(_T("b"),vTmp);
					float b2=vtof(vTmp);
					rsTmp1.SetFieldValue(_T("b2"),COleVariant(b2));
					
					//˫�۸־��վ���g,mm
					float g=PL3-2*h1;
					rsTmp1.SetFieldValue(_T("g"),COleVariant(g));
					//˫�۸����ľ�B1,mm
					float B1=PL3-h1;
					//��ΪrsTmp1�д����ֶ�b1,��Jet���ݿ��ֶ����ǲ����ִ�Сд�ģ��ʻ�B1->Bx,������Ӧ�Ĺ�ʽB1�Ѿ��滻��
					rsTmp1.SetFieldValue(_T("Bx"),COleVariant(B1));
					//��������׼������
					rsTmp1.Update();					
					
					//��̬/��̬����
					for(i4=1;i4<=2;i4++)
					{
						if(i4==2)
						{
							mfpx=modPHScal::pxrc;
							mfpy=modPHScal::pyrc;
							mfpz=modPHScal::pzrc;
							mfmx=modPHScal::mxrc;
							mfmy=modPHScal::myrc;
							mfmz=modPHScal::mzrc;
						}
						else
						{
							mfpx=modPHScal::pxlc;
							mfpy=modPHScal::pylc;
							mfpz=modPHScal::pzlc;
							mfmx=modPHScal::mxlc;
							mfmy=modPHScal::mylc;
							mfmz=modPHScal::mzlc;
						}
						
						rsTmp1.Edit();
						rsTmp1.SetFieldValue(_T("Px"),COleVariant(mfpx));
						rsTmp1.SetFieldValue(_T("Py"),COleVariant(mfpy));
						rsTmp1.SetFieldValue(_T("Pz"),COleVariant(mfpz));
						rsTmp1.SetFieldValue(_T("Mx"),COleVariant(mfmx));
						rsTmp1.SetFieldValue(_T("My"),COleVariant(mfmy));
						rsTmp1.SetFieldValue(_T("Mz"),COleVariant(mfmz));
						rsTmp1.Update();
						
						//����1�͹���2����Ŀ����,mm3
						//float Wyh1=0.233*K*(2*h1+g)*(2*h1+g)+1.4*K*(bb+b1)*(h1+0.5*g);
						//float Wzh1=0.233*K*((b1+bb)*(b1+bb)+b1*b1)+0.35*K*(2*h1+g)*(bb+b1);
						//float Wyh2=(1.4*K*h2*h2*h2+4.2*K*h2*B1*B1)/3/(B1+h2)+0.7*K*b2*2*(B1*B1+h2*h2)/(B1+h2);
						//float Wzh2=K*b2*(0.467*b2+1.4*h2);
						
						if(!rsCal.FindFirst(_T("SEQ=1")))
						{
							//����%s��%s�������ļ��㹫ʽû�ҵ���
							sTmp.Format(IDS_SACalFormulaXNotFound,Ptype[i],_T("1"));
							throw sTmp;
						}
						else
						{
							if(i4==2)
							{//�������ݣ�ֻ����һ�Σ��ӿ��ٶ�
								//rsCal.GetFieldValue(_T("F1saFormula"),vTmp);
								//F1sa=vtos(vTmp);
								//rsCal.GetFieldValue(_T("F2saFormula"),vTmp);
								//F2sa=vtos(vTmp);
								rsCal.GetFieldValue(_T("Wyh1saFormula"),vTmp);
								Wyh1sa=vtos(vTmp);
								rsCal.GetFieldValue(_T("Wzh1saFormula"),vTmp);
								Wzh1sa=vtos(vTmp);
								rsCal.GetFieldValue(_T("Wyh2saFormula"),vTmp);
								Wyh2sa=vtos(vTmp);
								rsCal.GetFieldValue(_T("Wzh2saFormula"),vTmp);
								Wzh2sa=vtos(vTmp);
								
								//Ӧ���оݿ��Ȼ�ã������Ժ����
								rsCal.GetFieldValue(_T("SIGMA1saMaxFormula"),vTmp);
								SIGMA1saMax=vtos(vTmp);
								rsCal.GetFieldValue(_T("TAO1saMaxFormula"),vTmp);
								TAO1saMax=vtos(vTmp);
								rsCal.GetFieldValue(_T("SIGMA2saMaxFormula"),vTmp);
								SIGMA2saMax=vtos(vTmp);
								rsCal.GetFieldValue(_T("TAO2saMaxFormula"),vTmp);
								TAO2saMax=vtos(vTmp);
								rsCal.GetFieldValue(_T("lamda1Formula"),vTmp);
								lamda1=vtos(vTmp);
								rsCal.GetFieldValue(_T("lamda2Formula"),vTmp);
								lamda2=vtos(vTmp);
								
								//��ʼ���㼸�Ρ���������
								SQLx=_T("SELECT ")+ Wyh1sa+_T(" AS Wyh1sac, ")+ Wyh2sa+_T(" AS Wyh2sac, ") + Wzh1sa+_T(" AS Wzh1sac, ")+ Wzh2sa+_T(" AS Wzh2sac, ")+SIGMA1saMax+_T(" AS SIGMA1saMaxc, ")+TAO1saMax+_T(" AS TAO1saMaxc, ")+SIGMA2saMax+_T(" AS SIGMA2saMaxc, ")+TAO2saMax+_T(" AS TAO2saMaxc, ")+lamda1+_T(" AS lamda1c, ")+lamda2+_T(" AS lamda2c FROM tmpCalFixPhs");
								if(rsTmp2.IsOpen())
									rsTmp2.Close();
								rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
								rsTmp2.Open(dbOpenDynaset,SQLx);
								
								rsTmp1.Edit();
								rsTmp2.GetFieldValue(_T("Wyh1sac"),vTmp);
								rsTmp1.SetFieldValue(_T("Wyh1"),vTmp);
								
								rsTmp2.GetFieldValue(_T("Wzh1sac"),vTmp);
								rsTmp1.SetFieldValue(_T("Wzh1"),vTmp);
								
								rsTmp2.GetFieldValue(_T("Wyh2sac"),vTmp);
								rsTmp1.SetFieldValue(_T("Wyh2"),vTmp);
								
								rsTmp2.GetFieldValue(_T("Wzh2sac"),vTmp);
								rsTmp1.SetFieldValue(_T("Wzh2"),vTmp);							
								
								rsTmp2.GetFieldValue(_T("TAO1saMaxc"),vTmp);
								rsTmp1.SetFieldValue(_T("TAO1saMax"),vTmp);
								mfTAO1saMax=vtof(vTmp);
								rsTmp2.GetFieldValue(_T("SIGMA1saMaxc"),vTmp);
								rsTmp1.SetFieldValue(_T("SIGMA1saMax"),vTmp);
								mfSIGMA1saMax=vtof(vTmp);
								
								rsTmp2.GetFieldValue(_T("TAO2saMaxc"),vTmp);
								rsTmp1.SetFieldValue(_T("TAO2saMax"),vTmp);
								mfTAO2saMax=vtof(vTmp);
								rsTmp2.GetFieldValue(_T("SIGMA2saMaxc"),vTmp);
								rsTmp1.SetFieldValue(_T("SIGMA2saMax"),vTmp);
								mfSIGMA2saMax=vtof(vTmp);
								
								rsTmp2.GetFieldValue(_T("lamda1c"),vTmp);
								rsTmp1.SetFieldValue(_T("lamda1"),vTmp);
								mfLamda1=vtof(vTmp);
								rsTmp2.GetFieldValue(_T("lamda2c"),vTmp);
								rsTmp1.SetFieldValue(_T("lamda2"),vTmp);
								mfLamda2=vtof(vTmp);
								
								rsTmp1.Update();
								float mf1=0.0,mf2=0,mfl1=0,mfl2=0;
								SQLx=_T("SELECT Lamda,Fi FROM SteadyDecreaseCoef WHERE Material=\'") + m_strMaterial + _T("\'");
								if(rs2.IsOpen())
									rs2.Close();
								rs2.m_pDatabase=&EDIBgbl::dbSACal;//20071103 "dbSORT" ��Ϊ "dbSACal"
								rs2.Open(dbOpenSnapshot,SQLx);
								if(rs2.IsEOF() && rs2.IsBOF())
								{
									//��%s���ȶ��ۼ�ϵ����%s��û��%s���ֲ��ϡ�
									sTmp.Format(IDS_NoMaterialInSteadyDecreaseCoef,rs2.m_pDatabase->GetName(),_T("SteadyDecreaseCoef"),m_strMaterial);
									throw sTmp;
								}
								else
								{
									if(!rs2.FindFirst(_T("Lamda>")+ftos(mfLamda1)))
									{
										//�����ϸ����%s����%s��%s�������ֵ��
										sTmp.Format(IDS_LamdaOverMaxValue,ftos(mfLamda1),modPHScal::dbZDJcrude.GetName(),_T("SteadyDecreaseCoef"));
										throw sTmp;
									}
									else
									{
										rs2.GetFieldValue(_T("Lamda"),vTmp);
										mfl2=vtof(vTmp);
										rs2.GetFieldValue(_T("Fi"),vTmp);
										mf2=vtof(vTmp);
										rs2.MovePrev();
										if(rs2.IsBOF())
										{
											//һ�㲻����,��Ϊ������ֵ��0��ʼ
										}
										else
										{
											//��ֵ��Fi1
											rs2.GetFieldValue(_T("Lamda"),vTmp);
											mfl1=vtof(vTmp);
											rs2.GetFieldValue(_T("Fi"),vTmp);
											mf1=vtof(vTmp);
											mfFi1=(mf2-mf1)/(mfl2-mfl1)*(mfLamda1-mfl1)+mf1;
											rsTmp1.Edit();
											rsTmp1.SetFieldValue(_T("Fi1"),COleVariant(mfFi1));																					
											rsTmp1.Update();
										}
									}
									//�󹹼�2�ȶ��ۼ�ϵ��Fi2
									if(!rs2.FindFirst(_T("Lamda>")+ftos(mfLamda2)))
									{
										//�����ϸ����%s����%s��%s�������ֵ��
										sTmp.Format(IDS_LamdaOverMaxValue,ftos(mfLamda2),modPHScal::dbZDJcrude.GetName(),_T("SteadyDecreaseCoef"));
										throw sTmp;
									}
									else
									{
										rs2.GetFieldValue(_T("Lamda"),vTmp);
										mfl2=vtof(vTmp);
										rs2.GetFieldValue(_T("Fi"),vTmp);
										mf2=vtof(vTmp);
										rs2.MovePrev();
										if(rs2.IsBOF())
										{
											//һ�㲻����,��Ϊ������ֵ��0��ʼ
										}
										else
										{
											//��ֵ��Fi1
											rs2.GetFieldValue(_T("Lamda"),vTmp);
											mfl1=vtof(vTmp);
											rs2.GetFieldValue(_T("Fi"),vTmp);
											mf1=vtof(vTmp);
											mfFi2=(mf2-mf1)/(mfl2-mfl1)*(mfLamda2-mfl1)+mf1;
											rsTmp1.Edit();
											rsTmp1.SetFieldValue(_T("Fi2"),COleVariant(mfFi2));																					
											rsTmp1.Update();
										}
									}
								}
							}
						}
						
						//ȡPy1=1.5*(Py+/-fabs(1000*mx+pz*modPHScal::PAfixH)/B1),�Ծ���ֵ�ϴ�����ΪУ�˹�����unit,mm
						//��ʼ��̬����
						float Py1r1=1.5*(mfpy+fabs(1000*mfmx+mfpz*modPHScal::PAfixH)/B1);
						float Py1r2=1.5*(mfpy-fabs(1000*mfmx+mfpz*modPHScal::PAfixH)/B1);
						float Py1r=((fabs(Py1r1)>fabs(Py1r2))?Py1r1:Py1r2);
						
						rsTmp1.Edit();
						rsTmp1.SetFieldValue(_T("Py1"),COleVariant(Py1r1));
						rsTmp1.SetFieldValue(_T("Py2"),COleVariant(Py1r2));
						rsTmp1.Update();
						
						if( (mfpx>=0 && 0>=Py1r) || (0>=mfpx && mfpx>=1.732*Py1r))
						{
							i5=1;
						}
						else if( (mfpx>=1.732*Py1r) && (Py1r>0))
						{
							i5=2;
						}
						if( (Py1r>=0 && 0>mfpx) || (1.732*Py1r>=mfpx && mfpx>=0))
						{
							i5=3;
						}
						else if( (mfpx<=1.732*Py1r) && (Py1r<0))
						{
							i5=4;
						}
						
						if(!rsCal.FindFirst(_T("SEQ=")+ltos(i5)))
						{
							//����%s��%s�������ļ��㹫ʽû�ҵ���
							sTmp.Format(IDS_SACalFormulaXNotFound,Ptype[i],_T("1"));
							throw sTmp;
						}
						else
						{
							rsCal.GetFieldValue(_T("SIGMA1saFormula"),vTmp);
							SIGMA1sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOx1saFormula"),vTmp);
							TAOx1sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOy1saFormula"),vTmp);
							TAOy1sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOz1saFormula"),vTmp);
							TAOz1sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAO1saFormula"),vTmp);
							TAO1sa=vtos(vTmp);
							
							rsCal.GetFieldValue(_T("SIGMA2saFormula"),vTmp);
							SIGMA2sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOx2saFormula"),vTmp);
							TAOx2sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOy2saFormula"),vTmp);
							TAOy2sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAOz2saFormula"),vTmp);
							TAOz2sa=vtos(vTmp);
							rsCal.GetFieldValue(_T("TAO2saFormula"),vTmp);
							TAO2sa=vtos(vTmp);
							
							//ѹ�˵ĳ���ϵ��u1,u2:����=2,��֧=0.5							
							rsCal.GetFieldValue(_T("u1"),vTmp);
							rsTmp1.Edit();
							rsTmp1.SetFieldValue(_T("u1"),vTmp);
							rsCal.GetFieldValue(_T("u2"),vTmp);
							rsTmp1.SetFieldValue(_T("u2"),vTmp);
							rsTmp1.Update();
							
							//��ʼ������Ӧ��sigma����Ӧ��tao
							SQLx=_T("SELECT ")+ TAOx1sa+_T(" AS TAOx1sac, ")+ TAOy1sa+_T(" AS TAOy1sac, ")+ TAOz1sa+_T(" AS TAOz1sac,")
								+ TAOx2sa+_T(" AS TAOx2sac, ")+ TAOy2sa+_T(" AS TAOy2sac, ")+ TAOz2sa+_T(" AS TAOz2sac FROM tmpCalFixPhs");
							if(rsTmp2.IsOpen())
								rsTmp2.Close();
							rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
							rsTmp2.Open(dbOpenDynaset,SQLx);
							
							rsTmp1.Edit();
							rsTmp2.GetFieldValue(_T("TAOx1sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOx1sa"),vTmp);
							rsTmp2.GetFieldValue(_T("TAOy1sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOy1sa"),vTmp);
							rsTmp2.GetFieldValue(_T("TAOz1sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOz1sa"),vTmp);
							
							rsTmp2.GetFieldValue(_T("TAOx2sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOx2sa"),vTmp);
							rsTmp2.GetFieldValue(_T("TAOy2sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOy2sa"),vTmp);
							rsTmp2.GetFieldValue(_T("TAOz2sac"),vTmp);
							rsTmp1.SetFieldValue(_T("TAOz2sa"),vTmp);
							rsTmp1.Update();
							
							SQLx=_T("SELECT ")+ TAO1sa+_T(" AS TAO1sac,")+ TAO2sa+_T(" AS TAO2sac,")+SIGMA1sa+_T(" AS SIGMA1sac,")+SIGMA2sa+_T(" AS SIGMA2sac FROM tmpCalFixPhs");
							if(rsTmp2.IsOpen())
								rsTmp2.Close();
							rsTmp2.m_pDatabase=&EDIBgbl::dbPRJ;
							rsTmp2.Open(dbOpenDynaset,SQLx);
							
							rsTmp2.GetFieldValue(_T("SIGMA1sac"),vTmp);
							mfSIGMA1sa=vtof(vTmp);
							rsTmp1.Edit();
							rsTmp1.SetFieldValue(_T("SIGMA1sa"),vTmp);
							
							rsTmp2.GetFieldValue(_T("SIGMA2sac"),vTmp);
							mfSIGMA2sa=vtof(vTmp);
							rsTmp1.SetFieldValue(_T("SIGMA2sa"),vTmp);
							
							rsTmp2.GetFieldValue(_T("TAO1sac"),vTmp);
							mfTAO1sa=vtof(vTmp);							
							rsTmp1.SetFieldValue(_T("TAO1sa"),vTmp);
							
							rsTmp2.GetFieldValue(_T("TAO2sac"),vTmp);
							mfTAO2sa=vtof(vTmp);
							rsTmp1.SetFieldValue(_T("TAO2sa"),vTmp);
							rsTmp1.Update();
							
							//����1(���͸�P1)ǿ��У��(�۸ֲ���)
							//unit,kgf/mm2
							if(mfSIGMA1sa>mfSIGMA1saMax)
							{
								if(bEOF)
								{
									//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
									sTmp.Format(GetResStr(IDS_SSteelStressOver),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(mfSIGMA1sa),ftos(mfSIGMA1saMax));
									throw sTmp;
								}
								else
									bErr=true;							
							}
							//����1(���͸�P1)ǿ��У��(���첿��)
							//����1����ϳ�Ӧ��
							if(mfTAO1sa>mfTAO1saMax)
							{
								if(bEOF)
								{
									//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
									sTmp.Format(GetResStr(IDS_SSteelStressOver1),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(mfTAO1sa),ftos(mfTAO1saMax));
									throw sTmp;
								}
								else
									bErr=true;							
							}
							
							//����2(б�Ų۸�P2)ǿ��У��(�۸ֲ���)
							//unit,kgf/mm2
							if(mfSIGMA2sa>mfSIGMA2saMax)
							{
								if(bEOF)
								{
									//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
									sTmp.Format(GetResStr(IDS_SSteelStressOver),ltos(modPHScal::zdjh),_T("2"),strP2,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(mfSIGMA2sa),ftos(mfSIGMA2saMax));
									throw sTmp;
								}
								else
									bErr=true;							
							}
							//����2(б�Ų۸�P2)ǿ��У��(���첿��)
							//����2����ϳ�Ӧ��
							if(mfTAO1sa>mfTAO1saMax)
							{
								if(bEOF)
								{
									//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
									sTmp.Format(GetResStr(IDS_SSteelStressOver1),ltos(modPHScal::zdjh),_T("2"),strP2,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(mfTAO2sa),ftos(mfTAO2saMax));
									throw sTmp;
								}
								else
									bErr=true;							
							}
						}						
					}
					
					if(!bErr /*&& (dh>=36)*/) //ע�͵��Ĳ��ֿɲ��Ի���Ժ�����ֲ�����
					{
						//û���ޣ��ҵ�
						break;
					}
					else
					{
						rsX.MoveNext();
						if(rsX.IsEOF())
						{
							//�Ѿ�����¼β������ѭ��һ�Σ��Ա㱨��
							bEOF=true;
							continue;
						}
						else
							//���¿�ʼ֮ǰ�����ó��ޱ�־
							bErr=false;
						
						//��ù���3
						rsX.GetFieldValue(_T("P3"),vTmp);
						strP3=vtos(vTmp);	
						rsTmp1.Edit();
						rsTmp1.SetFieldValue(_T("P3"),vTmp);
						//�˴�ֻ��Ӧ�й���׼�����ʻ�ʱҪ�޸Ĵ���
						n=strP3.Find((_T("x")));
						//����3�ȿ�bb,unit:mm
						bb=_tcstod(strP3.Mid(1,n-1),NULL);
						rsTmp1.SetFieldValue(_T("bb"),COleVariant(bb));
						//������͸�P1
						rsX.GetFieldValue(_T("P1"),vTmp);
						strP1=vtos(vTmp);
						rsTmp1.SetFieldValue(_T("P1"),vTmp);
						//��ù���2�͸�P2
						rsX.GetFieldValue(_T("P2"),vTmp);
						strP2=vtos(vTmp);
						rsTmp1.SetFieldValue(_T("P2"),vTmp);
						//˫�۸���Ե������PL3,mm
						rsX.GetFieldValue(_T("PL3"),vTmp);
						PL3=vtof(vTmp);
						//rsX�е�PL3��CHAR(20),��rsTmp1�е�PL3��REAL,�ɼ������ܹ��Զ�ת����
						rsTmp1.SetFieldValue(_T("PL3"),vTmp);
						//�۸ֺ���߶�K,mm
						rsX.GetFieldValue(_T("size2"),vTmp);
						K=vtof(vTmp);
						rsTmp1.SetFieldValue(_T("K"),vTmp);
						rsTmp1.Update();
						//���DH,mm
						rsX.GetFieldValue(_T("dh"),vTmp);
						dh=vtoi(vTmp);				
					}
					
					/*
					//����1�͹���2����Ŀ����,mm3
					float Wyh1=0.233*K*(2*h1+g)*(2*h1+g)+1.4*K*(bb+b1)*(h1+0.5*g);
					float Wzh1=0.233*K*((b1+bb)*(b1+bb)+b1*b1)+0.35*K*(2*h1+g)*(bb+b1);
					float Wyh2=(1.4*K*h2*h2*h2+4.2*K*h2*B1*B1)/3/(B1+h2)+0.7*K*b2*2*(B1*B1+h2*h2)/(B1+h2);
					float Wzh2=K*b2*(0.467*b2+1.4*h2);
					
					  //ȡPy1=1.5*(Py+/-fabs(1000*mx+pz*modPHScal::PAfixH)/B1),�Ծ���ֵ�ϴ�����ΪУ�˹�����unit,mm
					  //��ʼ��̬����
					  float Py1r1=1.5*(modPHScal::pyrc+fabs(1000*modPHScal::mxrc+modPHScal::pzrc*modPHScal::PAfixH)/B1);
					  float Py1r2=1.5*(modPHScal::pyrc-fabs(1000*modPHScal::mxrc+modPHScal::pzrc*modPHScal::PAfixH)/B1);
					  float Py1r=((fabs(Py1r1)>fabs(Py1r2))?Py1r1:Py1r2);
					  
						if( (modPHScal::pxrc>=0 && 0>=Py1r) || (0>=modPHScal::pxrc && modPHScal::pxrc>=1.732*Py1r))
						{
						//����1(���͸�P1)ǿ��У��(�۸ֲ���)
						float sigma1r=(modPHScal::pxrc-1.732*Py1r)/2/F1+fabs(1000*modPHScal::myrc-modPHScal::pzrc*modPHScal::gdw)*(B1+h1)*J1y/4/(J1y+0.25*F1*B1*B1)/(J1y+0.65*J2y)+fabs(1000.0*modPHScal::mzrc-modPHScal::pxrc*modPHScal::PAfixH)*J1z/4/W1z/(J1z+0.65*J2z);//unit,kgf/mm2
						//float sigma1r=(px-1.732*Py1)/2/F1+abs(1000*my-pz*gdw)*(B1+h1)*J1y/4/(J1y+0.25*F1*B1*B1)/(J1y+0.65*J2y)+fabs(1000.0*mz-px*H)*J1z/4/W1z/(J1z+0.65*J2z);//unit,kgf/mm2
						if(sigma1r>Sigma0)
						{
						if(bEOF)
						{
						//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
						sTmp.Format(GetResStr(IDS_SSteelStressOver),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(sigma1r),ftos(Sigma0));
						throw sTmp;
						}
						else
						bErr=true;							
						}
						//����1(���͸�P1)ǿ��У��(���첿��)
						//float TAOx1r=(px-1.732*Py1)/1.4/K/(PL3+2*b1+bb)+abs(1000*my-pz*gdw)*J1y/Wyh1/(J1y+0.65*J2y)+(1000*mz-px*H)*J1z/2/Wzh1/(J1z+0.65*J2z);//unit,kgf/mm2
						//float TAOy1r=3*abs(1000.0*mz-px*H)*J1z/2.8/gdw/(J1z+0.65*J2z)/(bb+2*b1)/K;//unit,kgf/mm2
						//float TAOz1r=pz/2.8/K/(PL3-h1);//unit,kgf/mm2
						float TAOx1r=(modPHScal::pxrc-1.732*Py1r)/1.4/K/(PL3+2*b1+bb)+fabs(1000*modPHScal::myrc-modPHScal::pzrc*modPHScal::gdw)*J1y/Wyh1/(J1y+0.65*J2y)+(1000*modPHScal::mzrc-modPHScal::pxrc*modPHScal::PAfixH)*J1z/2/Wzh1/(J1z+0.65*J2z);//unit,kgf/mm2
						float TAOy1r=3*fabs(1000.0*modPHScal::mzrc-modPHScal::pxrc*modPHScal::PAfixH)*J1z/2.8/modPHScal::gdw/(J1z+0.65*J2z)/(bb+2*b1)/K;//unit,kgf/mm2
						float TAOz1r=modPHScal::pzrc/2.8/K/(PL3-h1);//unit,kgf/mm2
						//����1����ϳ�Ӧ��
						float TAO1r=sqrt(TAOx1r*TAOx1r+TAOy1r*TAOy1r+TAOz1r*TAOz1r);//unit,kgf/mm2
						//׼��
						if(TAO1r>0.6*Sigma0)
						{							
						if(bEOF)
						{
						//%s��֧���ܹ���%s�۸�%s������̬��Ӧ��ֵ%s��������ֵ%s
						sTmp.Format(GetResStr(IDS_SSteelStressOver1),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(TAO1r),ftos(0.6*Sigma0));
						throw sTmp;
						}
						else
						bErr=true;							
						}
						}
						
						  float Py1l1=1.5*(modPHScal::pylc+fabs(1000*modPHScal::mxlc+modPHScal::pzlc*modPHScal::PAfixH)/B1);
						  float Py1l2=1.5*(modPHScal::pylc-fabs(1000*modPHScal::mxlc+modPHScal::pzlc*modPHScal::PAfixH)/B1);
						  float Py1l=((fabs(Py1l1)>fabs(Py1l2))?Py1l1:Py1l2);
						  
							if( (modPHScal::pxlc>=0 && 0>=Py1l) || (0>=modPHScal::pxlc && modPHScal::pxlc>=1.732*Py1l))
							{
							//����1(���͸�P1)ǿ��У��(�۸ֲ���)
							float sigma1l=(modPHScal::pxlc-1.732*Py1l)/2/F1+fabs(1000*modPHScal::mylc-modPHScal::pzlc*modPHScal::gdw)*(B1+h1)*J1y/4/(J1y+0.25*F1*B1*B1)/(J1y+0.65*J2y)+fabs(1000.0*modPHScal::mzlc-modPHScal::pxlc*modPHScal::PAfixH)*J1z/4/W1z/(J1z+0.65*J2z);//unit,kgf/mm2
							if(sigma1l>Sigma0)
							{
							if(bEOF)
							{
							//%s�Ź���%s�۸�%s��̬��Ӧ��ֵ%s��������ֵ%s
							sTmp.Format(GetResStr(IDS_SSteelStressOver),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(sigma1l),ftos(Sigma0));
							throw sTmp;
							}
							else
							bErr=true;							
							}
							//����1(���͸�P1)ǿ��У��(���첿��)
							float TAOx1l=(modPHScal::pxlc-1.732*Py1l)/1.4/K/(PL3+2*b1+bb)+fabs(1000*modPHScal::mylc-modPHScal::pzlc*modPHScal::gdw)*J1y/Wyh1/(J1y+0.65*J2y)+(1000*modPHScal::mzlc-modPHScal::pxlc*modPHScal::PAfixH)*J1z/2/Wzh1/(J1z+0.65*J2z);//unit,kgf/mm2
							float TAOy1l=3*fabs(1000.0*modPHScal::mzlc-modPHScal::pxlc*modPHScal::PAfixH)*J1z/2.8/modPHScal::gdw/(J1z+0.65*J2z)/(bb+2*b1)/K;//unit,kgf/mm2
							float TAOz1l=modPHScal::pzlc/2.8/K/(PL3-h1);//unit,kgf/mm2
							//����1����ϳ�Ӧ��
							float TAO1l=sqrt(TAOx1l*TAOx1l+TAOy1l*TAOy1l+TAOz1l*TAOz1l);//unit,kgf/mm2
							//׼��
							if(TAO1l>0.6*Sigma0)
							{							
							if(bEOF)
							{
							//%s��֧���ܹ���%s�۸�%s������̬��Ӧ��ֵ%s��������ֵ%s
							sTmp.Format(GetResStr(IDS_SSteelStressOver1),ltos(modPHScal::zdjh),_T("1"),strP1,GetResStr(IDS_frmtxsr_OptCoolStatus),ftos(TAO1l),ftos(0.6*Sigma0));
							throw sTmp;
							}
							else
							bErr=true;							
							}
							}
							
							  if(!bErr && (dh>36))
							  {
							  //û���ޣ��ҵ�
							  break;
							  }
							  else
							  {
							  rsX.MoveNext();
							  if(rsX.IsEOF())
							  {
							  //�Ѿ�����¼β������ѭ��һ�Σ��Ա㱨��
							  bEOF=true;
							  continue;
							  }
							  else
							  //���¿�ʼ֮ǰ�����ó��ޱ�־
							  bErr=false;
							  
								//��ù���3
								rsX.GetFieldValue(_T("P3"),vTmp);
								strP3=vtos(vTmp);
								//�˴�ֻ��Ӧ�й���׼�����ʻ�ʱҪ�޸Ĵ���
								n=strP3.Find((_T("x")));
								//����3�ȿ�bb,unit:mm
								bb=atof(strP3.Mid(1,n-1));
								//������͸�P1
								rsX.GetFieldValue(_T("P1"),vTmp);
								strP1=vtos(vTmp);
								//��ù���2�͸�P2
								rsX.GetFieldValue(_T("P2"),vTmp);
								strP2=vtos(vTmp);
								//˫�۸���Ե������PL3,mm
								rsX.GetFieldValue(_T("PL3"),vTmp);
								PL3=vtof(vTmp);
								//�۸ֺ���߶�K,mm
								rsX.GetFieldValue(_T("size2"),vTmp);
								K=vtof(vTmp);
								//���DH,mm
								rsX.GetFieldValue(_T("dh"),vTmp);
								dh=vtoi(vTmp);
								}
*/
				}while(true);//��¼β��ѭ��һ��,�Ա㱨��
				
				//����һ������У�˵����ݿ��ļ�����Ӧ��Excel�ļ�
				EDIBgbl::ChangeColumnsToRows(EDIBgbl::dbPRJ,_T("tmpCalFixPhs"),_T("CheckCalFixPhs"));
				EDIBDB::OutPutTable( _T("CheckCalFixPhs"), basDirectory::ProjectDBDir+_T("CheckCalculation.xls"), _T("Excel 5.0;"), EDIBgbl::dbPRJ,  _T("CheckCalFixPhs"), _T("Excel 5.0;"));
				
				SQLx = CString(_T("SELECT * FROM ")) + modPHScal::tbnSA + _T(" WHERE GDW1>= ") + ftos(modPHScal::gdw) + _T(" AND trim(CustomID)=\'") + Ptype[i] + _T("\' AND dh=") + ltos(dh) + _T(" ORDER BY GDW1,PmaxSF");
			}
			//����ǿ�ȼ���:����Ժ�̶�֧�ܸ���˫�����Ǽ�SJ8(ClassID=iSALbraceFixG47=4)
		}			
	}
	catch(_com_error e)
	{
	}
	return flag;
}
