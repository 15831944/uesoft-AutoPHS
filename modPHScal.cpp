// modPHScal.cpp
//
#include "stdafx.h"
//#include <float.h>
#include "modPHScal.h"
#include "modRegistry.h"
#include "basDirectory.h"
#include "EDIBgbl.h"
#include "Cphs.h"

#include "ModEncrypt.h"
#include "user.h"
#include "EDIBAcad.h"
#include "EDIBDB.h"
#include "PhsData.h"
#include <math.h>
#include "FrmTxsr.h"
#include "PagTxsrOption.h"
#include "frmStatus.h"
#include "FrmPhsSamp.h"
#include "InputBox.h"
#include "MessageBoxEx.h"

#include "fstream.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int g_itest = 0;

//LFX 2005.6.30 ��  �������ѡ��
bool modPHScal::gbDrawWeldSymbol = TRUE; 
bool modPHScal::gbAutoWeldViewInsertPoint = TRUE; 
bool modPHScal::gbDrawWeldViewDim = TRUE; 
bool modPHScal::gbWeldAutoDirectAndPos = TRUE;
//END LFX 2005.6.30 ��  �������ѡ��

float modPHScal::gmiDiameter=0;//���濿���ܲ��ĵ�һ������ֱ������1-3�μ���֮��
float modPHScal::giDiameter=0;//��������ֱ������3�μ���֮��

float modPHScal::gfSeqCircleDiameter=6.0;//��ԲȦ��ע�ߴ�ʱ,Բ��ֱ��
float modPHScal::gfSeqTxtHeight=5.5;//��ע������ʱ,������ֵĸ߶�
BOOL modPHScal::g_bDHT=FALSE;//�ڲ��ϻ��ܱ���CLcl�ֶ�д�ϡ��纸����,
CString modPHScal::g_strDHTGG="";//�ڲ��ϻ��ܱ���CLcl�ֶ�д�ϡ��纸����ʱ���纸���Ĺ������ַ���������CLgg�ֶ�
BOOL modPHScal::bInstalledAndFirstRun=FALSE;//��һ�ΰ�װ�ҵ�һ�����б�־,�����������ݿ�
float modPHScal::g_fLHGWeight=0;//�·����LHG������
BOOL modPHScal::gbNoFenQu=FALSE;//ͼ����򲻷���
BOOL modPHScal::gbTBhq=TRUE;//ͼ����л�ǩ��
BOOL modPHScal::gbNoWaiKuang=FALSE;//��Ҫͼ�����
bool modPHScal::gbOrientationPicture=false;//pfg20050531
int modPHScal::iRepZdjType=-1;//��AutoPHS����ĵ��ɺ�ȡ��Ӧ������AutoPSA����ĵ��ɺ�
bool modPHScal::bHintRepZdjType=false;//��ʾ��AutoPHS����ĵ��ɺ�ȡ��Ӧ������AutoPSA����ĵ��ɺ�
int giUPxyz;//���ϵ�������

bool gbHotStatus2Zero;//��̬������㵯��
float modPHScal::DblHangerRodDist=0;//˫�����˼��
float modPHScal::fltTmp=-9999;
SprInfo modPHScal::sSprInfo[16];//������Ϣ����
int modPHScal::SprInfoIndex=16;//��������ţ���һ��֧���������е�������µ�������ΪT1.209��T1.109,��T1.209���=0
const _TCHAR*	modPHScal::gcsCustomID=_T("CustomID");
const _TCHAR*	modPHScal::gcsID=_T("ID");
_ConnectionPtr modPHScal::dbZDJcrude;//֧����ԭʼ���ݿ�����
bool modPHScal::gbStartedOnceAPHS = false;//AutoPHS������
CString modPHScal::Unit_Force = CString();//����λ
CString modPHScal::Unit_Displacement = CString();//λ�Ƶ�λ
CString modPHScal::Unit_Momental = CString();//���ص�λ
_variant_t modPHScal::gBookmark = _variant_t();
CString modPHScal::Ax = CString();//ͼֽ��С,A3,A4
int modPHScal::gintParallelNum = 0;//���ɲ������������˲�����
int modPHScal::gSumDistOfSpring = 0;//������λ��
SprDirectionChar modPHScal::gSprDirectionChar;//���ɷ����ַ��������ϡ��������¡�����UP������Down��
CString modPHScal::df1 = CString();//�ܲ�����
CString modPHScal::gf1 = CString();//��������
CString modPHScal::df = CString();//
CString modPHScal::gf = CString();
CString modPHScal::fx = CString();
float modPHScal::haz = 0;//���ɰ�װѹ��ֵ
CString modPHScal::dn = CString();//�ܲ�����CustomID
CString modPHScal::IDdn = CString();//�ܲ�ͨ�ô���ID
float modPHScal::dj = 0;//�ܾ�,mm
float modPHScal::dwB = 0;//���ιܵ��⾶�����mm
float modPHScal::t0 = 0;//�����¶�,C
float modPHScal::sngPEL = 0;//�ܲ����,m
CString modPHScal::gn = CString();//��������CustomID
CString modPHScal::IDgn = CString();//����ͨ�ô���ID
float modPHScal::sngSEL = 0;//�������,m
long modPHScal::zdjh = (long) 0;//֧���ܱ��
CString modPHScal::dg = CString();//�ܲ�������ƫװ��־=D��G
float modPHScal::gdw1 = 0;//����֧���ܺ������õ㵽����ľ��룬mm
float modPHScal::gdw = 0;//gdw1����ƫװ�������ֵ��mm

float modPHScal::gdwx = 0;//������λxֵ,mm
float modPHScal::gdwz = 0;//������λzֵ,mm
float modPHScal::yr = 0;//֧������̬λ�ƣ�mm
float modPHScal::yr1 = 0;//����y����λ��
float modPHScal::xr = 0;
float modPHScal::zr = 0;
float modPHScal::yl = 0;
float modPHScal::xl = 0;
float modPHScal::zl = 0;
float modPHScal::dyr = 0;
float modPHScal::dxr = 0;
float modPHScal::dzr = 0;
float modPHScal::dyl = 0;
float modPHScal::dxl = 0;
float modPHScal::dzl = 0;
float modPHScal::dxa0 = 0;//�ܲ���λxֵ,mm
float modPHScal::dza0 = 0;//�ܲ���λzֵ,mm
float modPHScal::dxa = 0;//dxa0ƫװ������ֵ,mm
float modPHScal::dza = 0;//dza0ƫװ������ֵ,mm
float modPHScal::gpzx = 0;//����ƫװxֵ,mm
float modPHScal::gpzz = 0;//����ƫװzֵ,mm
float modPHScal::pjg = 0;//�ṹ����,kgf
float modPHScal::pgz = 0;//��������,kgf
float modPHScal::paz = 0;//��װ����,kgf
	//���ڹ̶�֧��Z2/Z2A��ְ����ĳ������Ϣ
	float modPHScal::gfPSLength=0;
	float modPHScal::gfPSWidth=0;
	float modPHScal::gfPSThickness=0;
	int modPHScal::giSAG100No=1;
	//�̶�֧�ܼ���ģ������ϵ������̬���ܵ���,kgf
	float modPHScal::pxlc=0;
	float modPHScal::pylc=0;
	float modPHScal::pzlc=0;
	float modPHScal::pxrc=0;
	float modPHScal::pyrc=0;
	float modPHScal::pzrc=0;
	float modPHScal::mxlc=0;
	float modPHScal::mylc=0;
	float modPHScal::mzlc=0;
	float modPHScal::mxrc=0;
	float modPHScal::myrc=0;
	float modPHScal::mzrc=0;
	float modPHScal::PAfixH=0;//�̶�֧�ܹܲ��߶�H,cm

int modPHScal::iMaxRodDia = 0;
int modPHScal::giNumRod = 0;
float modPHScal::Lspan = 0;
float modPHScal::WidthB = 0;
float modPHScal::WidthA = 0;
CString modPHScal::gstrMaterial = CString();
CString modPHScal::gsPhsArrangementDwgNo = CString();
CString modPHScal::gsPhsScheduleDwgNo = CString();
CString modPHScal::clb = CString();
float modPHScal::SpringRigid = 0;
float modPHScal::SpringLMax = 0;
long modPHScal::iSelSampleID = (long) 0;
bool modPHScal::pbFrmLoadedBmp = false;
int modPHScal::SelInputMethod = 0;
CString modPHScal::CSZdjType = CString();
CString modPHScal::ZdjType = CString();
int modPHScal::giMinRodLen=300;
bool modPHScal::gbPhsIsCSPR = false;
bool modPHScal::gbPhsIsSPR = false;
BOOL modPHScal::g_bPipeClampBoltDiameterChanged = TRUE;

CString modPHScal::gsOldPhsPASel = CString();
CString modPHScal::gsOldPhsPARTSel = CString();
CString modPHScal::gsOldPhsSASel = CString();
CString modPHScal::gsOldPhsSPRINGSel = CString();
CString modPHScal::gsOldPhsConstantSpringSel = CString();
CString modPHScal::gsOldPhsBoltsNutsSel = CString();
CString modPHScal::gsPhsPASel = CString();
CString modPHScal::gsPhsPARTSel = CString();
CString modPHScal::gsPhsSASel = CString();
CString modPHScal::gsPhsSPRINGSel = CString();
CString modPHScal::gsPhsConstantSpringSel = CString();
CString modPHScal::gsPhsBoltsNutsSel = CString();
CString modPHScal::gsPhsDescPASel = CString();
CString modPHScal::gsPhsDescPARTSel = CString();
CString modPHScal::gsPhsDescSASel = CString();
CString modPHScal::gsPhsDescSPRINGSel = CString();
CString modPHScal::gsPhsDescConstantSpringSel = CString();
CString modPHScal::gsPhsDescBoltsNutsSel = CString();
CString modPHScal::gsCSPRmanufactory = CString();
CString modPHScal::gsSPRmanufactory = CString();
CString modPHScal::gsCSPRUnitOfLoad = CString();
CString modPHScal::gsSPRUnitOfLoad = CString();
CString modPHScal::tbnTZB = CString();
CString modPHScal::tbnBoltsNuts = CString();
CString modPHScal::tbnBoltsNutsID = CString();
CString modPHScal::tbnAttachment = CString();
CString modPHScal::tbnAttachmentID = CString();
CString modPHScal::tbnSABoltsNuts = CString();
CString modPHScal::tbnSABoltsNutsID = CString();
CString modPHScal::tbnBoltsSurplusLength = CString();
CString modPHScal::tbnPA = CString();
CString modPHScal::tbnSA = CString();
CString modPHScal::tbnPAfix = CString();
CString modPHScal::tbnPART = CString();
CString modPHScal::tbnPAID = CString();
CString modPHScal::tbnPARTID = CString();
CString modPHScal::tbnSAID = CString();
CString modPHScal::tbnSectionSteel = CString();
CString modPHScal::tbnSectionSteelID = CString();
CString modPHScal::tbnLugInfo = CString();
CString modPHScal::tbnLugLenCal = CString();
CString modPHScal::tbnCONNECT = CString();//���ұ�׼�������ӹ����һ����Ҫ������Ӧ����Ժ���������Դ���ĸ�����
CString modPHScal::tbnAllowStress = CString();
CString modPHScal::tbnHDCrude = CString();
CString modPHScal::tbnHDproperty = CString();
CString modPHScal::tbnHDid = CString();
CString modPHScal::tbnDisplacementSerial = CString();
CString modPHScal::tbnCSPRDiameterSerial = CString();
CString modPHScal::tbnSPRDiameterSerial = CString();
CString modPHScal::tbnLugDiaOfCSPR = CString();
CString modPHScal::tbnSPRINGCrude = CString();
CString modPHScal::tbnSPRINGL5Crude = CString();
CString modPHScal::tbnCSPRINGL5Crude = CString();
CString modPHScal::tbnSPRINGproperty = CString();
CString modPHScal::tbnDiscSPRINGpropertyMaxDist = CString();
CString modPHScal::tbnSPRINGid = CString();
bool modPHScal::gbH1IncludeB = false;
bool modPHScal::gH1Pos = false;
bool modPHScal::gH1distPos = false;
bool modPHScal::gbCH1IncludeB = false;
bool modPHScal::gbCH1IncludeT = false;
int modPHScal::gCH1Pos = 0;
int modPHScal::gCH1distPos = 0;
bool modPHScal::bE_FormulaPlusHalf = 0;
CString modPHScal::strBigFontLjmx;//�����ϸ������
CString modPHScal::strBigFontDim;	//�ߴ�����
CString modPHScal::strSHXFontLjmx;//�����ϸ������
CString modPHScal::strSHXFontDim;	//�ߴ�����
float modPHScal::fFWDim;	//�ߴ�����������
float modPHScal::fFWLjmx;	//�����ϸ��������
float modPHScal::gsngTotalInstallCompressValue = 0;
bool modPHScal::gbByForce = false;
float modPHScal::gUseConstantSpringMinSurplusDist = 0;
float modPHScal::gUseConstantSpringPercentSurplusDist = 0;
float modPHScal::gUseSpringMinDist = 0;
float modPHScal::gnSPR_CHZBH = 0;
float modPHScal::gsngRatioOfPjg2Pgz = 0;
float modPHScal::gnConnectTJ = 0;
	//��ȡ�㲿��ѡ�ı�׼������ClassIndex,
	//Observation�ֶ�ָ���˲���ѡ���������ClassIndex,���ݸ��������¶ȡ��㲿������ID��
	//���Դ�sort.mdb����SpecificationOfMaterial��鵽��û��ѡ����ϵ��㲿�����õĲ��ϡ�
	//����: ClassIndex=0,Ϊ��ˮ�ܵ�֧�����㲿�����õĲ��ϣ��¹ܹ�DL/T5054-1996��;ClassIndex=1,Ϊ�̷�ú�۹ܵ�֧�����㲿�����õĲ��ϣ����������DL/T5121-2000����
int modPHScal::giClassIndex=0;
int modPHScal::gnRodLenRoundVal = 100;
int modPHScal::gnRodInsertLenRoundVal = 1;
//����ֱ�������ֵ����
bool modPHScal::gbRodDiaByMax=true;
bool modPHScal::gbPipeClampBolt = false;

bool modPHScal::gbConnectHoleDimMatch = false;
bool modPHScal::gbSPRAutoLugDia = false;
bool modPHScal::gbCSPRAutoLugDia = false;
bool modPHScal::gbAutoApplyChzbh = false;
bool modPHScal::gbOutputSA = false;
bool modPHScal::gbCalSAbyConservativenessMethod = false;
bool modPHScal::gbCalSAbyPJG = false;
bool modPHScal::gbCalPARTbyMaxLoad = false;
bool modPHScal::gbAddPartWeight2PMax = false;
bool modPHScal::gbAddPartWeight2CSPRPgz = true;
bool modPHScal::gbAddPartWeight2SPRPgz = false;//2007.07.30
bool modPHScal::gbPAForceZero=false;//2007.09.25
bool modPHScal::gbMaxPgzPaz=false;//2007.09.03
//bool modPHScal::gbMinMaxDispByActualDisp = true;
bool modPHScal::gbMinMaxDispByActualDisp = false;	
//bool modPHScal::gbCalcCSPRHeight_BySelDisp = false;
bool modPHScal::gbCalcCSPRHeight_BySelDisp=true;
bool modPHScal::gbCalCSPRLugDiaByPjg = false;
bool modPHScal::gbCSPRneedSpecialDesign = false;
bool modPHScal::gbCSPRCanSpecialDesign = false;
bool modPHScal::gbAutoAlignCSPRtoSA = false;


bool modPHScal::gbLimitLugMinDia = false;
bool modPHScal::gbSumAllVolume = false;
bool modPHScal::gbAutoPASA = false;
bool modPHScal::gbAutoApplyCSPRLugDia = false;
bool modPHScal::gbNotPrompt_ACAD_SHIFT = false;
int modPHScal::gIsACAD = 0;
bool modPHScal::gbNotPrompt_Catlog_OVERWRITE = false;
int modPHScal::giDrawSIZE_A3A4 = 0;
int modPHScal::giPhsOutINFOTableType = 0;
CString modPHScal::gsPhsColor = CString();
bool modPHScal::gbAnnotateCSPR = false;
bool modPHScal::gbAutoSPRRodDiaDecrease=false;
bool modPHScal::gbSPRBHLOADBYInstallLoad=false;
bool modPHScal::gbSPRBHLOADUNITBYNEWTON=false;
bool modPHScal::gbDraw_BOM = false;
bool modPHScal::gbDrawOVERWRITE = false;
bool modPHScal::gbWorksWithBoltsNuts = false;
bool modPHScal::gbWorksWithSABoltsNuts = false;
float modPHScal::gnDW_delta = 0;
int modPHScal::giAllowedMaxSerialNumPerSPR = 0;
int modPHScal::giAllowedMaxSerialNumSPRs = 0;
int modPHScal::giSpringOrder = 0;
CString modPHScal::gstrSpringOrder = CString();
CString modPHScal::gstrSpringOrderSQL = CString();
bool modPHScal::AutoOrdinateDrawing = false;
long modPHScal::gnSEQyOffset = (long) 8;
bool modPHScal::gbDrawNoPreMethod = false;
bool modPHScal::gbAutoSaveDrawing = false;
long modPHScal::glDrawNoStartNumber = (long) 0;
long modPHScal::glDWGPreMethod = (long) 0;
CString modPHScal::gsDWGPre = CString();
CString modPHScal::gsDwgFN = CString();
int modPHScal::giZDJHinDwg = 0;
bool modPHScal::blResizable = false;
CString modPHScal::gsPhsTypeRigid = CString();
CString modPHScal::gsPhsTypeSPR = CString();
CString modPHScal::gsPhsTypeCSPR = CString();
int modPHScal::giWholeSprNum = 0;
int modPHScal::giSumSerialNum = 0;
int modPHScal::glNumSA = 0;
int modPHScal::iCSnum = 0;
CString modPHScal::gsPartType = CString();
CString modPHScal::gsOldPartType = CString();
CString modPHScal::gsaSPRPos = CString();
bool modPHScal::gbStopOLE = false;
bool modPHScal::gbCalAllPhs = false;
bool modPHScal::gbStopCalAllPHS = false;
double modPHScal::VX_pt0x = 0.0;
double modPHScal::VX_pt0y = 0.0;
double modPHScal::VZ_pt0x = 0.0;
float modPHScal::pt1x = 0;
float modPHScal::pt1y = 0;
float modPHScal::pt2x = 0;
float modPHScal::pt2y = 0;
float modPHScal::pt3x = 0;
float modPHScal::pt3y = 0;
float modPHScal::Pt0X = 0;
float modPHScal::giDimOffset = 0;
float modPHScal::giAttDxOffsetX = 0;
float modPHScal::giAttDzOffsetX = 0;
float modPHScal::giAttDOffsetY = 0;
CString modPHScal::iPhsSAfx[4];
int* modPHScal::PtypeDiameter = NULL;//��getphsBHandSizes�У�������˵�ֱ�����������������Ա����е��������Ӽ���Լ����
int modPHScal::PtypeIndexForDiameter=0;
int* modPHScal::giCSPRindex = NULL;
int modPHScal::giCSPRindexSize=0;
int modPHScal::glIDIndex = 0;
long modPHScal::glClassID = (long) 0;
long modPHScal::glPAid = (long) 0;
long modPHScal::glSAid = (long) 0;
float vf7850=7850.0;
CString modPHScal::gsPhsName = CString();
CString modPHScal::gsPhsEnglishName = CString();
CString modPHScal::ZdjTxName[20];
bool modPHScal::gbSumRodByRoundSteel=0;
CString modPHScal::gsPSAOutFilePath;
int modPHScal::g_iPSAFilterIndex=1;
CString modPHScal::gsSelectProductMdb;
int modPHScal::iNumCSVal=0;
float modPHScal::fDiameter=0;//

int modPHScal::iAlignLoad=0;
int modPHScal::iAlignDim=0;
int modPHScal::iAlignLjmx=0;
float modPHScal::fFWLoad=1.0f;
CString modPHScal::strBigFontLoad=_T("");
CString modPHScal::strSHXFontLoad=_T("");
double modPHScal::gdZJOverValue = 30.0;//֧��ƫװ��Ҫ����λ��ֵ modify 2008.10.16

void modPHScal::InitZdjTxName()
{
 //�˹��̱���λ���޸�����GetPhsAssembleName����֮ǰ����Ϊ����Ҫʹ��gsPhsTypeSPR�ȱ�����
   //��ʼ���֧����ͼ�����ƣ�����ͼ�����
	_ConnectionPtr db=NULL;
	try{
	_RecordsetPtr rs;
	db.CreateInstance(__uuidof(Connection));
	_variant_t tmpvar;
	db->Open(_bstr_t(::dbConnectionString+basDirectory::ProjectDBDir + _T("PHScode.mdb")),_T(""),_T(""),0);//20071018"sort.mdb"��Ϊ"PHScode.mdb"
   rs=db->Execute(_T("SELECT * FROM PhsTypeName"),&tmpvar,adCmdText);
	while(!rs->adoEOF)
	{
		tmpvar=rs->GetCollect(_T("Index"));
		if(tmpvar.vt!=VT_NULL)
		{
			switch((long)tmpvar)
			{
			case iZdjSPRh:
            ZdjTxName[iZdjSPRh] = Trim(vtos(rs->GetCollect(_T("PhsTypeName"))));
			break;
			case iZdjCSPRh:
            ZdjTxName[iZdjCSPRh] = Trim(vtos(rs->GetCollect(_T("PhsTypeName"))));
			break;
			case iZdjRigidH:
            ZdjTxName[iZdjRigidH] = Trim(vtos(rs->GetCollect(_T("PhsTypeName"))));
			break;
			case iZdjGuide:
            ZdjTxName[iZdjGuide] = Trim(vtos(rs->GetCollect(_T("PhsTypeName"))));
			break;
			case iZdjSlide:
            ZdjTxName[iZdjSlide] = Trim(vtos(rs->GetCollect(_T("PhsTypeName"))));
			break;
			case iZDJfix:
            ZdjTxName[iZDJfix] = Trim(vtos(rs->GetCollect(_T("PhsTypeName"))));
			break;
			case iZdjSprGuide:
            ZdjTxName[iZdjSprGuide] = Trim(vtos(rs->GetCollect(_T("PhsTypeName"))));
			break;
			default:
            ZdjTxName[iZdjSupport] = Trim(vtos(rs->GetCollect(_T("PhsTypeName"))));
			}
		}
		rs->MoveNext();
	}
   
   //��ʼ���֧����������ƣ�����ģ�����ƹ���
	rs->Close();
	rs=NULL;
	rs=db->Execute(_T("SELECT * FROM PhsClassType"),&tmpvar,adCmdText);
   while(!rs->adoEOF)
	{
		tmpvar=rs->GetCollect(_T("Index"));
		if(tmpvar.vt!=VT_NULL)
		{
			switch((long)tmpvar)
			{
			case iPhsSPR:
            gsPhsTypeSPR = Trim(vtos(rs->GetCollect(_T("PhsClassType"))));
			break;
			case iPhsCSPR:
            gsPhsTypeCSPR = Trim(vtos(rs->GetCollect(_T("PhsClassType"))));
			break;
			case iPhsRigid:
            gsPhsTypeRigid = Trim(vtos(rs->GetCollect(_T("PhsClassType"))));
			break;
			}
		}
		rs->MoveNext();
   }
   rs->Close();
   rs=NULL;
   db->Close();
   db=NULL;
	}
	catch(_com_error &e)
	{
		ShowMessage(e.Description());
	}
}

void modPHScal::InitPhsSAfx()
{
	iPhsSAfx[iPX] = _T("PX");
   iPhsSAfx[iOZ] = _T("OZ");
   iPhsSAfx[iOX] = _T("OX");
   iPhsSAfx[iPZ] = _T("PZ");
}

int modPHScal::GetPhsSAfx(CString  SAfx)
{
 //��ø�������,Key��ֵ���ַ���
	SAfx.MakeUpper();
	if(SAfx==_T("X") || SAfx==_T("Z"))
		SAfx=CString(_T("P"))+SAfx;
     else if(SAfx.Left(1)==_T("-"))
		 SAfx.SetAt(0,_T('0'));
	 for(int i=0;i<4;i++)
	 {
		 if(iPhsSAfx[i]==SAfx)
			 return i;
	 }
	 return -1;
}

void modPHScal::GetHeatDisplacement(_RecordsetPtr /*ByVal*/ rstbl)
{
}

int modPHScal::iBOMStartNo(int /*Optional*/ SN)
{
	CInputBox dlg;
	dlg.m_strTitle.LoadString(IDS_PleaseInputBOMstartDrawNo);
	dlg.m_strWndText.LoadString(IDS_FirstPageBOMstartDrawNo);
	dlg.m_strValue.Format(_T("%d"),SN);
	dlg.DoModal();
	while(dlg.m_strValue==_T("") || dlg.m_strValue.GetAt(0) < _T('0') || dlg.m_strValue.GetAt(0) > _T('9'))
		dlg.DoModal();
   return _ttoi(dlg.m_strValue);
}

_variant_t modPHScal::sFindBlkPosFLD(CString  sFLD, CString  dFLD, CString  sID)
{
	//����sFLD�ֶε�ֵΪID�ļ�¼,����dFLD�ֶε�ֵ
	_variant_t vtmp;
	vtmp.ChangeType(VT_NULL);
   try
   {
	   _RecordsetPtr rs;
	   rs.CreateInstance(__uuidof(Recordset));
	   sFLD.TrimLeft();sFLD.TrimRight();
	   sID.TrimLeft();sID.TrimRight();
	   dFLD.TrimLeft();dFLD.TrimRight();
	   CString SQLx;
	   SQLx+=_T("SELECT * FROM PhsBlkDimPos WHERE (");
		SQLx+=sFLD;
		SQLx+=_T(")=\'");
		SQLx+=sID;
		SQLx+=_T("\'");
		rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
   if(rs->BOF && rs->adoEOF)
   {
	   ShowMessage(GetResStr(IDS_NosFLDinPhsBlkDimPos));
	   return vtmp;
   }
   rs->get_Collect((_variant_t)dFLD,&vtmp);
   return vtmp;
   }
   catch (_com_error &e)
   {
	   CString strMsg;
	   strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
	   AfxMessageBox(strMsg);
	}

	   return vtmp;
}

_variant_t modPHScal::sFindFLD(CString /*ByVal*/ strSourceFLD, CString /*ByVal*/ strDestFLD, CString strSourceFLDvalue)
{
//����strSourceFLD�ֶε�ֵΪstrSourceFLDvalue�ļ�¼,����strDestFLD�ֶε�ֵ
//����:����ʹ��rs��findfirst��������ʹ��SQL��ñ������һ����Access����ʱ�����ִ�Сд�����CustomID����Ψһ��
	try{
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
   CString sTmp;
   _variant_t var;
   strDestFLD.TrimLeft();strDestFLD.TrimRight();
   strSourceFLD.TrimLeft();strSourceFLD.TrimRight();
   if(strSourceFLDvalue.Left(1)!=(_T("\'")))
	   strSourceFLDvalue=_T("\'")+strSourceFLDvalue;
   if(strSourceFLDvalue.Right(1)!=(_T("\'")))
	   strSourceFLDvalue=strSourceFLDvalue+_T("\'");
   sTmp=CString(_T("SELECT "))+strDestFLD +_T(" FROM PictureClipData WHERE (")+strSourceFLD+_T(") = ")+strSourceFLDvalue;
   //rs=EDIBgbl::dbPRJ->Execute(_bstr_t(sTmp),NULL,adCmdText);
   rs->Open((_bstr_t)sTmp, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
	   adOpenStatic, adLockOptimistic, adCmdText); 
   if(rs->adoEOF)
         return _variant_t((long)0);
     else
     {
		rs->get_Collect((_variant_t)strDestFLD,&var);
		 return var;
     }
	 rs->Close();
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	return _variant_t((long)0);
}
}
/*
_variant_t modPHScal::sFindAnyTableField(_ConnectionPtr  db, CString  strSourceTable, CString  strSourceFLD, CString  strDestFLD, CString  strSourceFLDvalue)
{
	_variant_t ret;
	ret.ChangeType(VT_NULL);
	_variant_t tmpvar;
	if(db==NULL || strSourceTable == _T(""))
		return ret;
	CString sTmp;
	try{
		sTmp=_T("Select ")+strDestFLD+_T(" from ")+strSourceTable+_T(" where ")+strSourceFLD+_T(" = ")+strSourceFLDvalue;
		_RecordsetPtr rs=db->Execute(_bstr_t(sTmp),&tmpvar,adCmdText);
		if(rs->adoEOF)
			return ret;
		ret=rs->GetCollect(_variant_t(strDestFLD));
		rs->Close();
		return ret;
	}
	catch(_com_error &e)
	{
		return ret;
	}
}
*/
_variant_t modPHScal::sFindAnyTableField(_ConnectionPtr  db, CString  strSourceTable, CString  strSourceFLD, 
										  CString  strDestFLD, CString  strSourceFLDvalue)
{
	_variant_t ret;
	ret.ChangeType(VT_NULL);
	_variant_t tmpvar;
	if(strSourceTable == _T(""))
		return ret;
	CString sTmp;
	try{
		sTmp=_T("Select ")+strDestFLD+_T(" from ")+strSourceTable+_T(" where ")+strSourceFLD+_T(" = ")+strSourceFLDvalue;
		//_RecordsetPtr rs=db->Execute(_bstr_t(CString(_T("select * from "))+strSourceTable),&tmpvar,adCmdText);
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		rs->Open((_bstr_t)sTmp, _variant_t((IDispatch*)db,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
		if(rs->adoEOF)
			return ret;
		rs->get_Collect((_variant_t)strDestFLD,&tmpvar);
		rs->Close();
		ret=tmpvar;
		return ret;
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
		return ret;
	}
}
CString modPHScal::sFindID(CString CustomID)
{
//����CustomID����ID
	try
	{
	CustomID.TrimLeft();CustomID.TrimRight();
	CString ret;
   if (CustomID == _T(""))
      return  _T("");
   else
   {
	   _RecordsetPtr rs;
	   rs.CreateInstance(__uuidof(Recordset));
	  _variant_t tmpvar;
	 // _bstr_t bs=(_bstr_t)CString(_T("SELECT * FROM PictureClipData WHERE (CustomID)=\'")+CustomID+_T("\'"));
      //rs = EDIBgbl::dbPRJ->Execute(_bstr_t(CString(_T("SELECT * FROM PictureClipData WHERE (CustomID)=\'")+CustomID+_T("\'"))),
	///	  &tmpvar,adCmdText);
	  if(CustomID.Left(1)!=(_T("\'")))
		  CustomID=_T("\'")+CustomID;
	  if(CustomID.Right(1)!=(_T("\'")))
		  CustomID+=_T("\'");
	  CString sTmp;
	  sTmp = _T("SELECT * FROM PictureClipData WHERE (CustomID)=")+CustomID;
	  rs->Open((_bstr_t)sTmp, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		  adOpenStatic, adLockOptimistic, adCmdText); 
     if(rs->BOF && rs->adoEOF)
         //MsgBox ResolveResString(iUE_NoRecordFLDEqvThisValueInPictureClipData, _T("|1"), _T("CustomID"), _T("|2"), CustomID), vbMsgBoxSetForeground
         ret = _T("");
      else
      {
		  rs->get_Collect((_variant_t)_T("id"),&tmpvar);
		  ret=vtos(tmpvar);
		  //ret =(char*)(_bstr_t)rs->GetCollect(_variant_t(_T("id")));
		  ret.TrimLeft();ret.TrimRight();
	  }
	  rs->Close();
   return ret;
   }
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
		return _T("");
	}
}

CString modPHScal::sFindCustomID(CString ID)
{
//����ID����CustomID
	try
	{
	ID.TrimLeft();
	ID.TrimRight();
	if(ID==_T(""))
		return _T("");
   if(ID.Left(1)!=_T("\'"))
	   ID=_T("\'")+ID;
   if(ID.Right(1)!=_T("\'"))
	   ID=ID+_T("\'");
	CString SQLx=CString(_T("select * From PictureClipData Where Trim(ID)="))+ID;
	_RecordsetPtr rs;
	rs.CreateInstance(__uuidof(Recordset));
	rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenStatic, adLockOptimistic, adCmdText); 
	if(rs->adoEOF || rs->BOF)
	{
		rs->Close();
		/*CString sTmp;
		sTmp.Format(GetResStr(IDS_NoRecordFLDEqvThisValueInPictureClipData),_T("ID"),ID);
		ShowMessage(sTmp);*/
		return _T("");
	}
	_variant_t v;
	rs->get_Collect((_variant_t)_T("CustomID"),&v);
	rs->Close();
	return vtos(v);
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	catch(...)
	{
	}
	return _T("");
}


CString modPHScal::sFindTBN(CString /*ByVal*/ PartCustomID)
{
	_RecordsetPtr rs;
	rs.CreateInstance(__uuidof(Recordset));
//	int i;
   //iSA=0,iCSPR=7
   //Debug.Print PartCustomID
	_variant_t tmpvar;
	PartCustomID.TrimLeft();PartCustomID.TrimRight();
	glClassID = -1;
      glIDIndex = -1;
	if(PartCustomID==_T(""))
	{
		glClassID = -1;
      glIDIndex = -1;
		return _T("");
	}
	CString SQLx;
	SQLx = _T("SELECT * FROM PictureClipData WHERE  (CustomID)=\'")+PartCustomID+_T("\'");
	rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
		adOpenStatic, adLockOptimistic, adCmdText); 
   if(rs->BOF && rs->adoEOF )
   {
      //MsgBox ResolveResString(iUE_NoRecordFLDEqvThisValueInPictureClipData,_T("|1"),_T("CustomID"), _T("|2"), PartCustomID), vbMsgBoxSetForeground
      
      glClassID = -1;
      glIDIndex = -1;
	  rs->Close();
	  return _T("");
	}
   else
   {
	   rs->get_Collect((_variant_t)_T("ClassID"),&tmpvar);
	   if(tmpvar.vt!=VT_NULL)
			glClassID = vtoi( tmpvar);
	   rs->get_Collect((_variant_t)_T("Index"),&tmpvar);
	   if(tmpvar.vt!=VT_NULL)
      glIDIndex = vtoi(tmpvar);
	   rs->Close();
		switch(glClassID)
		{
		case iSAh:
		case iSACantilever:
		case iSAbeam:
		case iSALbrace:
		case iSALbraceFixG47:
		case iSALbraceFixG48:
		case iG51:
		case iG52_55:
		case iG56:
		case iG100:
		case iGCement:
             return tbnSA;
				break;
		case iPAfixZ1:
		case iPAfixZ2:
            return tbnPAfix;
			break;
		case iPAh:
		case iPAs:
		case iPASS:
		case iPAGS:
		case iPAD4LA:
		case iPADLR:
		case iPAD11LD:
		case iPAD4LQ:
		case iPALX:
		case iPAXZ1_XZ3:
         	return tbnPA;
			break;
		case iROD:
		case iConnected:
		case iL15:
		case iXL1:
            //���Ӽ����ݱ�
            return tbnPART;
			break;
		case iAttached:
            //�������ݱ�
            return tbnAttachment;
			break;
		case iBolts:
		case iNuts:
            //���ӱ������˨��ĸ������Ժ��CustomID,����˵��ID�������ʹ��������׼����������˨ʱ��Ҫ�Ƚ��б任��
            //�任������Cphs.GetphsBHandSizes��ʵ�֡�
            return tbnBoltsNuts;
			break;
		case iSPR:
            return tbnSPRINGCrude;
			break;
		case iCSPR:
            return tbnHDCrude;
			break;
        default:
            return tbnSA;
		}
   }
}

void modPHScal::AutoSEQforPictureClipData()
{
}

void modPHScal::InitZDJID()
{
}

void modPHScal::GetRecentWorkSPEC()
{
   //��ȡ�������֧���ܹ淶
	CString s=_T("");
	gsPhsPASel = Trim(GetRegKey(_T("RecentWork"), user::gstrAppName + _T("_phsPASel"),s));
	gsPhsPARTSel =Trim(GetRegKey(_T("RecentWork"), user::gstrAppName + _T("_phsPARTSel"), s));
	gsPhsSASel = Trim(GetRegKey(_T("RecentWork"), user::gstrAppName + _T("_phsSASel"), s));
	gsPhsSPRINGSel = Trim(GetRegKey(_T("RecentWork"), user::gstrAppName + _T("_phsSpringSel"), s));
	gsPhsConstantSpringSel = Trim(GetRegKey(_T("RecentWork"), user::gstrAppName + _T("_phsConstantSpringSel"), s));
	gsPhsBoltsNutsSel = Trim(GetRegKey(_T("RecentWork"), user::gstrAppName + _T("_phsBoltsNutsSel"), s));

}

void modPHScal::SaveRecentWorkSPEC()
{
   //�����������֧���ܹ淶
	::SetRegValue(_T("RecentWork"), user::gstrAppName + _T("_phsPASel"), gsPhsPASel);
   ::SetRegValue(_T("RecentWork"), user::gstrAppName + _T("_phsPARTSel"), gsPhsPARTSel);
   ::SetRegValue(_T("RecentWork"), user::gstrAppName + _T("_phsSASel"), gsPhsSASel);
   ::SetRegValue(_T("RecentWork"), user::gstrAppName + _T("_phsSpringSel"), gsPhsSPRINGSel);
   ::SetRegValue(_T("RecentWork"), user::gstrAppName + _T("_phsConstantSpringSel"), gsPhsConstantSpringSel);
   ::SetRegValue(_T("RecentWork"), user::gstrAppName + _T("_phsBoltsNutsSel"), gsPhsBoltsNutsSel);

}

bool  modPHScal::bPAIsHanger()
{
 //Ŀ�ģ��ж�֧�����Ƿ��ǵ���
   //���������ߴ��ڹܲ���ߣ����ǵ���;������֧�ܡ�
	try
	{
		sngSEL=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("GH1")));
		sngPEL=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("DH1")));
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	catch(...)
	{
	}
   if(sngSEL > sngPEL)
		return true;
	else return false;
}

bool modPHScal::blnSelAndSaveSPEC()
{
	bool ret=false;
	try
	{
		//��������ʱ�����뾭��CreateTmpCustomIDForPART(), blnSelPhsSPEC()��������һ�Σ���Ϊ��һЩ������Ҫ�ڸú����л�ã�������Щ������=0�����������		
		if (EDIBgbl::gbSelSPECset)
			ret =true;
		else
		{
			GetRecentWorkSPEC();
			if (blnSelphsSPEC(true))
			{
				SaveRecentWorkSPEC();
				ret = true;
			}
			else
				ret = false;
			EDIBgbl::gbSelSPECset = ret;
		}
	}
	catch(_com_error &e)
	{
		ret=false;
		ShowMessage(e.Description());
	}
	return ret;
}


bool modPHScal::blnSelphsSPEC(bool /*ByVal*/ mbSPECchanged)
{
	//Ŀ�ģ����ݹ淶ѡ��ȷ����˨��ĸ�����ɡ�������ܲ��������㲿���ı�����
	//���أ��淶ѡ��ɹ�������true;��������False
	//RemAuthor:LEE Guobin
	//����:LEE Guobin
	//LFX 2005.2.24 ע ���������ڵ��õڸ�HStbExists��������ĩβ�����һ��bWarn���ھ����Ƿ���ʾ����
	bool bWarn = gbStartedOnceAPHS;   //LFX 2005.2.24 ��,�����������������ѡ��淶ʱ��ʱ����
	bool ret=FALSE;
	bool bSPECchanged=false;
	_variant_t tmpvar,vTmp;
	CString strTmp;
	int inttbExist=0;//0��ʾ��һ�ν���ѭ����1��ʾ����ڣ�2��ʾ������
	_ConnectionPtr db,db1;
	db.CreateInstance(__uuidof(Connection));
	db1.CreateInstance(__uuidof(Connection));
	_RecordsetPtr rsX;
	rsX.CreateInstance(__uuidof(Recordset));
	//add by lfx 2005.4.20
	CString strStandardConnect;   //��׼���Ӽ����ݱ���
	strStandardConnect.Format("CONNECTNW");
	//END add by lfx 2005.4.20
	try
	{
		//����淶�����µ����ݣ�����ѡ��淶�����仯(����������ʱ)�����������ɹ淶���ݡ�
		//����ʱ�����ġ�
		if( mbSPECchanged )
		{
		/*if((gsPhsConstantSpringSel == _T("")) 
		|| (gsPhsSPRINGSel == _T("")) 
		|| (gsPhsPASel == _T("")) 
		|| (gsPhsPARTSel == _T("")) 
		|| (gsPhsSASel == _T("")) 
		|| (gsPhsBoltsNutsSel == _T("")) )
		return false;
		else
			bSPECchanged=true;*/
			bSPECchanged=true;
		}
		else
		{
			if(gbStartedOnceAPHS)
			{
				if((gsPhsConstantSpringSel == gsOldPhsConstantSpringSel) 
					&& (gsPhsSPRINGSel == gsOldPhsSPRINGSel) 
					&& (gsPhsPASel == gsOldPhsPASel) 
					&& (gsPhsPARTSel == gsOldPhsPARTSel) 
					&& (gsPhsSASel == gsOldPhsSASel) 
					&& (gsPhsBoltsNutsSel == gsOldPhsBoltsNutsSel))
				{
					bSPECchanged=false;
					return TRUE;
				}
				else
				{
					bSPECchanged=true;					
				}
			}
			else
			{
				bSPECchanged=false;				
			}
		}

		long  lngErrNum ;
		CString sTmp;
		//���̽ϳ�
		AfxGetApp()->BeginWaitCursor();
		sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoPA));
		frmStatus.m_Label1 = sTmp;
		frmStatus.m_Label2 = _T("");
		frmStatus.SetWindowText(GetResStr(IDS_InitializeAutoPHS));
		frmStatus.UpdateData(false);
		frmStatus.ShowWindow(SW_SHOW);

		CString strConnect;
		strConnect.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=%s"),
			basDirectory::ProjectDBDir + _T("zdjcrude.mdb"), ModEncrypt::gstrDBZdjCrudePassWord);
		db->Open((_bstr_t)strConnect, "", "", adModeUnknown);

		strConnect="Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=" + basDirectory::ProjectDBDir+_T("sort.mdb");
		db1->Open((_bstr_t)strConnect, "", "", adConnectUnspecified);
		//���ڻ�ȡ�ܲ���Ϣ...
		frmStatus.UpdateStatus(1.0 /9.0,true);
		frmStatus.UpdateData(false);
		frmStatus.UpdateWindow();
		//֧���ܹܲ�ԭʼ���ݱ�����
		CString SQLx;
		SQLx = _T("SELECT * FROM phsManuPA ORDER BY [Observation],[standard] ASC");
		rsX->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
		gsPhsPASel.TrimLeft();gsPhsPASel.TrimRight();
		if( rsX->BOF && rsX->adoEOF )
		{
			lngErrNum = IDS_NotFoundThisStandardInSortMdb;
			sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuPA"),gsPhsPASel);
			ShowMessage(sTmp);
			goto errH;
		}
		if(	gsPhsPASel != _T("") )
		{
			_variant_t vTmp;
			strTmp = (_T("standard=\'")+gsPhsPASel+_T("\'"));
			if( rsX->adoEOF)
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
		}
		else
		{
			rsX->MoveFirst();
			inttbExist=2;
		}
		//�����жϹܲ�ԭʼ���ݱ������ݿ����Ƿ����
		while((inttbExist!=1)&&(!rsX->adoEOF))
		{
			rsX->get_Collect((_variant_t)_T("standard"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				gsPhsPASel=_T("");
			else
			{
				gsPhsPASel=vtos(tmpvar);
				gsPhsPASel.TrimLeft();gsPhsPASel.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("crudePA"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnPA=_T("");
			else
			{
				tbnPA=vtos(tmpvar);
				tbnPA.TrimLeft();tbnPA.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("crudePAfix"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnPAfix=_T("");
			else{
				tbnPAfix=vtos(tmpvar);
				tbnPAfix.TrimLeft();tbnPAfix.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("tbnPAid"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnPAID=_T("");
			else{
				tbnPAID=vtos(tmpvar);
				tbnPAID.TrimLeft();tbnPAID.TrimRight();
			}
			if(!HStbExists(db,db1,tbnPA,_T("phsManuPA"),_T("crudePA"),gsPhsPASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else				//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}

			if(!HStbExists(db,db1,tbnPAfix,_T("phsManuPA"),_T("crudePAfix"),gsPhsPASel,bWarn))
			{
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			if(!HStbExists(db,db1,tbnPAID,_T("phsManuPA"),_T("crudePAid"),gsPhsPASel,bWarn))
			{
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			inttbExist=1;
		}
		if(inttbExist!=1)	//û�ҵ�ԭ���ݱ�
			goto errH;
		inttbExist=0;		//������������Ϊ��ֵ������������ʹ��
		rsX->get_Collect((_variant_t)_T("standardDesc"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			gsPhsDescPASel=_T("");
		else
		{
			gsPhsDescPASel=vtos(tmpvar);
			gsPhsDescPASel.TrimLeft();gsPhsDescPASel.TrimRight();
		}
		//��ȡ�㲿��ѡ�ı�׼������ClassIndex,
		//Observation�ֶ�ָ���˲���ѡ���������ClassIndex,���ݸ��������¶ȡ��㲿������ID��
		//���Դ�sort.mdb����SpecificationOfMaterial��鵽��û��ѡ����ϵ��㲿�����õĲ��ϡ�
		//����: ClassIndex=0,Ϊ��ˮ�ܵ�֧�����㲿�����õĲ��ϣ��¹ܹ�DL/T5054-1996��;ClassIndex=1,Ϊ�̷�ú�۹ܵ�֧�����㲿�����õĲ��ϣ����������DL/T5121-2000����
		rsX->get_Collect((_variant_t)_T("Observation"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			//Ĭ�ϰ��¹ܹ�DL/T5054-1996 p25 4.3.3 p67 7.5.2ѡ��
			giClassIndex=0;
		else
			giClassIndex=vtoi(tmpvar);

		//����ɾ��tbnPAfix����tbnPA��ͬ����Ŀ����ֹCreateTmpCustomIDForPART�����в��������ظ�����
		sTmp=_T("DELETE FROM [") + tbnPAfix + _T("] WHERE CustomID IN ( SELECT DISTINCT CustomID FROM [") + tbnPA + _T("]) ");
		db->Execute((_bstr_t)sTmp, NULL, adCmdText);
		
		//���ڻ�ȡ���Ӽ���Ϣ...
		sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoPART));
		frmStatus.m_Label1 = sTmp;
		frmStatus.UpdateStatus(2.0 /9.0,true);
		frmStatus.UpdateData(false);
		frmStatus.UpdateWindow();
		//֧�������Ӽ�ԭʼ���ݱ�����
		if(rsX->State == adStateOpen)
			rsX->Close();
		sTmp = _T("SELECT * FROM phsManuPART ORDER BY [Observation],[standard] ASC");
		rsX->Open((_bstr_t)sTmp,_variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
		gsPhsPARTSel.TrimLeft();gsPhsPARTSel.TrimRight(); 
		if( rsX->BOF && rsX->adoEOF )
		{
			lngErrNum = IDS_NotFoundThisStandardInSortMdb;
			sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuPART"),gsPhsPARTSel);
			ShowMessage(sTmp);
			goto errH;
		}
		if(gsPhsPARTSel != _T("") )
		{
			HRESULT hr = S_OK;
			rsX->MoveFirst();
			strTmp = (_T("standard=\'")+gsPhsPARTSel+_T("\'"));
			hr = rsX->Find((_bstr_t)strTmp, 0, adSearchForward);
			if( rsX->adoEOF)
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
		}
		else
		{
			rsX->MoveFirst();
			inttbExist=2;
		}
		//�����ж����Ӽ�ԭʼ���ݱ������ݿ����Ƿ����
		while((inttbExist!=1)&&(!rsX->adoEOF))
		{
			rsX->get_Collect((_variant_t)_T("standard"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				gsPhsPARTSel=_T("");
			else
			{
				gsPhsPARTSel=vtos(tmpvar);
				gsPhsPARTSel.TrimLeft();gsPhsPARTSel.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("crudePART"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnPART=_T("");
			else
			{
				tbnPART=vtos(tmpvar);
				tbnPART.TrimLeft();tbnPART.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("tbnPARTid"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnPARTID=_T("");
			else
			{
				tbnPARTID=vtos(tmpvar);
				tbnPARTID.TrimLeft();tbnPARTID.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("LugInfo"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnLugInfo=_T("");
			else
			{
				tbnLugInfo=vtos(tmpvar);
				tbnLugInfo.TrimLeft();tbnLugInfo.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("LugLenCal"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnLugLenCal=_T("");
			else
			{
				tbnLugLenCal=vtos(tmpvar);
				tbnLugLenCal.TrimLeft();tbnLugLenCal.TrimRight();
			}
			//add by ligb/lfx on 2005.04.20
			//����Ժ���˺ͻ�����˿�����Դ��˱���ĸ�����������ӹ���������Ժ��������׼��һ����
			//�������ӱ��������������
			rsX->get_Collect((_variant_t)_T("tbnCONNECT"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnCONNECT = strStandardConnect;
			else
			{
				tbnCONNECT=vtos(tmpvar);
				tbnCONNECT.TrimLeft();tbnCONNECT.TrimRight();
			}
			//END add by ligb/lfx on 2005.04.20

			if(!HStbExists(db,db1,tbnLugInfo,_T("phsManuPART"),_T("LugInfo"),gsPhsPARTSel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			if(!HStbExists(db,db1,tbnLugLenCal,_T("phsManuPART"),_T("LugLenCal"),gsPhsPARTSel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			if(!HStbExists(db,db1,tbnPART,_T("phsManuPART"),_T("crudePART"),gsPhsPARTSel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}

			if(!HStbExists(db,db1,tbnPARTID,_T("phsManuPART"),_T("tbnPARTid"),gsPhsPARTSel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			inttbExist=1;
		}
		if(inttbExist!=1)
			goto errH;
		inttbExist=0;
		rsX->get_Collect((_variant_t)_T("standardDesc"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			gsPhsDescPARTSel=_T("");
		else
		{
			gsPhsDescPARTSel=vtos(tmpvar);
			gsPhsDescPARTSel.TrimLeft();gsPhsDescPARTSel.TrimRight();
		}
		
		//���ڻ�ȡ������Ϣ...
		sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoSA));
		frmStatus.m_Label1 = sTmp;
		frmStatus.UpdateStatus(3.0 /9.0,true);
		frmStatus.UpdateData(false);
		frmStatus.UpdateWindow();
		//֧���ܸ���ԭʼ���ݱ�����
		if(rsX->State == adStateOpen)
			rsX->Close();
		sTmp = _T("SELECT * FROM phsManuSA ORDER BY [Observation],[standard] ASC");
		rsX->Open((_bstr_t)sTmp,_variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
		gsPhsSASel.TrimLeft();gsPhsSASel.TrimRight(); 
		if( rsX->BOF && rsX->adoEOF )
		{
			lngErrNum = IDS_NotFoundThisStandardInSortMdb;
			sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuSA"),gsPhsSASel);
			ShowMessage(sTmp);
			goto errH;
		}
		if(gsPhsSASel != _T("") )
		{
			HRESULT hr = S_OK;
			rsX->MoveFirst();
			strTmp = _T("standard=\'")+gsPhsSASel+_T("\'");
			hr = rsX->Find((_bstr_t)strTmp, 0, adSearchForward);
			if( rsX->adoEOF)
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
		}
		else
		{
			rsX->MoveFirst();
			inttbExist=2;
		}
		BOOL bCrudeSA=FALSE,btbnSAid=FALSE,bCrudeSS=FALSE,tCrudeSSID=FALSE;
		while((inttbExist!=1)&&(!rsX->adoEOF))
		{
			rsX->get_Collect((_variant_t)_T("standard"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				gsPhsSASel=_T("");
			else
			{
				gsPhsSASel=vtos(tmpvar);
				gsPhsSASel.TrimLeft();gsPhsSASel.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("crudeSA"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnSA=_T("");
			else
			{
				tbnSA=vtos(tmpvar);
				tbnSA.TrimLeft();tbnSA.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("tbnSAid"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnSAID=_T("");
			else
			{
				tbnSAID=vtos(tmpvar);
				tbnSAID.TrimLeft();tbnSAID.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("crudeSectionSteel"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnSectionSteel=_T("");
			else
			{
				tbnSectionSteel=vtos(tmpvar);
				tbnSectionSteel.TrimLeft();tbnSectionSteel.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("crudeSectionSteelID"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnSectionSteelID=_T("");
			else
			{
				tbnSectionSteelID=vtos(tmpvar);
				tbnSectionSteelID.TrimLeft();tbnSectionSteelID.TrimRight();
			}
			//������˨��ĸ���ݱ�
			rsX->get_Collect((_variant_t)_T("crudeSABoltsNuts"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnSABoltsNuts=_T("");
			else
			{
				tbnSABoltsNuts=vtos(tmpvar);
				tbnSABoltsNuts.TrimLeft();tbnSABoltsNuts.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("crudeAttachmentID"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnSABoltsNutsID=_T("");
			else
			{
				tbnSABoltsNutsID=vtos(tmpvar);
				tbnSABoltsNutsID.TrimLeft();tbnSABoltsNutsID.TrimRight();
			}
			//�����������ݱ�
			rsX->get_Collect((_variant_t)_T("crudeAttachment"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnAttachment=_T("");
			else
			{
				tbnAttachment=vtos(tmpvar);
				tbnAttachment.TrimLeft();tbnAttachment.TrimRight();
			}
			rsX->get_Collect((_variant_t)_T("crudeAttachmentID"),&tmpvar);
			if(tmpvar.vt==VT_NULL)
				tbnAttachmentID=_T("");
			else
			{
				tbnAttachmentID=vtos(tmpvar);
				tbnAttachmentID.TrimLeft();tbnAttachmentID.TrimRight();
			}
			//�����жϸ���ʹ�õĸ����������ݿ����Ƿ����
			if(!HStbExists(db,db1,tbnAttachment,_T("phsManuSA"),_T("crudeAttachment"),gsPhsSASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			if(!HStbExists(db,db1,tbnAttachmentID,_T("phsManuSA"),_T("crudeAttachmentID"),gsPhsSASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			//�����жϸ���ʹ�õ���˨��ĸ�������ݿ����Ƿ����
			if(!HStbExists(db,db1,tbnSABoltsNuts,_T("phsManuSA"),_T("crudeSABoltsNuts"),gsPhsSASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			if(!HStbExists(db,db1,tbnSABoltsNutsID,_T("phsManuSA"),_T("crudeSABoltsNutsID"),gsPhsSASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			//�����жϸ���ԭʼ���ݱ������ݿ����Ƿ����
			if(!HStbExists(db,db1,tbnSA,_T("phsManuSA"),_T("crudeSA"),gsPhsSASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			if(!HStbExists(db,db1,tbnSAID,_T("phsManuSA"),_T("tbnSAID"),gsPhsSASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			//�����жϸ���ʹ�õ��͸ֱ������ݿ����Ƿ����
			if(!HStbExists(db,db1,tbnSectionSteel,_T("phsManuSA"),_T("crudeSectionSteel"),gsPhsSASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			if(!HStbExists(db,db1,tbnSectionSteelID,_T("phsManuSA"),_T("crudeSectionSteelID"),gsPhsSASel,bWarn))
			{	
				if(inttbExist==0)	//��һ���Ƶ���һ����¼��
				{
					rsX->MoveFirst();
					inttbExist=2;
				}
				else					//���ǵ�һ�Σ������һ����¼
					rsX->MoveNext();
				continue;
			}
			inttbExist=1;
	}
	if(inttbExist!=1)
		goto errH;
	inttbExist=0;
	rsX->get_Collect((_variant_t)_T("standardDesc"),&tmpvar);
	if(tmpvar.vt==VT_NULL)
		gsPhsDescSASel=_T("");
	else
	{
		gsPhsDescSASel=vtos(tmpvar);
		gsPhsDescSASel.TrimLeft();gsPhsDescSASel.TrimRight();
	}
	//���ڻ�ȡ��˨��ĸ��Ϣ...
	sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoBoltsNuts));
	frmStatus.m_Label1 = sTmp;
	frmStatus.UpdateStatus(4.0 /9.0,true);
	frmStatus.UpdateData(false);
	frmStatus.UpdateWindow();
	//��˨��ĸԭʼ���ݱ�����
	if(rsX->State == adStateOpen)
		rsX->Close();
	sTmp = _T("SELECT * FROM phsManuBoltsNuts ORDER BY [Observation],[standard] ASC");
	rsX->Open((_bstr_t)sTmp,_variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
		adOpenStatic, adLockOptimistic, adCmdText); 
	gsPhsBoltsNutsSel.TrimLeft();gsPhsBoltsNutsSel.TrimRight(); 
	if( rsX->BOF && rsX->adoEOF )
	{
		lngErrNum = IDS_NotFoundThisStandardInSortMdb;
		sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuBoltsNuts"),gsPhsBoltsNutsSel);
		ShowMessage(sTmp);
		goto errH;
	}
	if(gsPhsBoltsNutsSel != _T("") )
	{
		HRESULT hr = S_OK;
		rsX->MoveFirst();
		strTmp = _T("standard=\'")+gsPhsBoltsNutsSel+_T("\'");
		hr = rsX->Find((_bstr_t)strTmp, 0, adSearchForward);
		if( rsX->adoEOF)
		{
			rsX->MoveFirst();
			inttbExist=2;
		}
	}
	else
	{
		rsX->MoveFirst();
		inttbExist=2;
	}
	while((inttbExist!=1)&&(!rsX->adoEOF))
	{
		rsX->get_Collect((_variant_t)_T("standard"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			gsPhsBoltsNutsSel=_T("");
		else
		{
			gsPhsBoltsNutsSel=vtos(tmpvar);
			gsPhsBoltsNutsSel.TrimLeft();gsPhsBoltsNutsSel.TrimRight();
		}
		rsX->get_Collect((_variant_t)_T("crudeBoltsNuts"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnBoltsNuts=_T("");
		else
		{
			tbnBoltsNuts=vtos(tmpvar);
			tbnBoltsNuts.TrimLeft();tbnBoltsNuts.TrimRight();
		}
		rsX->get_Collect((_variant_t)_T("crudeBoltsNutsID"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnBoltsNutsID=_T("");
		else
		{
			tbnBoltsNutsID=vtos(tmpvar);
			tbnBoltsNutsID.TrimLeft();tbnBoltsNutsID.TrimRight();
		}
		rsX->get_Collect((_variant_t)_T("BoltsSurplusLength"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnBoltsSurplusLength=_T("");
		else
		{
			tbnBoltsSurplusLength=vtos(tmpvar);
			tbnBoltsSurplusLength.TrimLeft();tbnBoltsSurplusLength.TrimRight();
		}
		//�����ж���˨��ĸ�������ݿ����Ƿ����
		if(!HStbExists(db,db1,tbnBoltsNuts,_T("phsManuBoltsNuts"),_T("crudeBoltsNuts"),gsPhsBoltsNutsSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		if(!HStbExists(db,db1,tbnBoltsNutsID,_T("phsManuBoltsNuts"),_T("crudeBoltsNutsID"),gsPhsBoltsNutsSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		if(!HStbExists(db,db1,tbnBoltsSurplusLength,_T("phsManuBoltsNuts"),_T("BoltsSurplusLength"),gsPhsBoltsNutsSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		inttbExist=1;
	}
	if(inttbExist!=1)
		goto errH;
	inttbExist=0;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	//���ڻ�ȡ������Ϣ...
	sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoSPRING));
	frmStatus.m_Label1 = sTmp;
	frmStatus.UpdateStatus(5.0 /9.0,true);
	frmStatus.UpdateData(false);
	frmStatus.UpdateWindow();
	//����ԭʼ����
	if(rsX->State == adStateOpen)
		rsX->Close();
	sTmp = _T("SELECT * FROM phsManuSPRING ORDER BY [Observation],[standard] ASC");
	rsX->Open((_bstr_t)sTmp,_variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
		adOpenDynamic, adLockOptimistic, adCmdText); 
	gsPhsSPRINGSel.TrimLeft();gsPhsSPRINGSel.TrimRight(); 
	if( rsX->BOF && rsX->adoEOF ){
		lngErrNum = IDS_NotFoundThisStandardInSortMdb;
		sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuBoltsNuts"),gsPhsBoltsNutsSel);
		ShowMessage(sTmp);
		goto errH;
	}
	if(gsPhsSPRINGSel != _T("") )
	{
		  HRESULT hr = S_OK;
		  rsX->MoveFirst();
		  strTmp = _T("standard=\'")+gsPhsSPRINGSel+_T("\'");
		  hr = rsX->Find((_bstr_t)strTmp, 0, adSearchForward);
		  if( rsX->adoEOF)
		{
			rsX->MoveFirst();
			inttbExist=2;
		}
	}
	else
	{
		rsX->MoveFirst();
		inttbExist=2;
	}
	while((inttbExist!=1)&&(!rsX->adoEOF))
	{
		rsX->get_Collect((_variant_t)_T("standard"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			gsPhsSPRINGSel=_T("");
		else{
			gsPhsSPRINGSel=vtos(tmpvar);
			gsPhsSPRINGSel.TrimLeft();gsPhsSPRINGSel.TrimRight();
		}
		rsX->get_Collect((_variant_t)_T("Spring_property"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnSPRINGproperty=_T("");
		else{
			tbnSPRINGproperty=vtos(tmpvar);
			tbnSPRINGproperty.TrimLeft();tbnSPRINGproperty.TrimRight();
		}
		rsX->get_Collect((_variant_t)_T("Spring_propertyMaxDist"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnDiscSPRINGpropertyMaxDist=_T("");
		else{
			tbnDiscSPRINGpropertyMaxDist=vtos(tmpvar);
			tbnDiscSPRINGpropertyMaxDist.TrimLeft();tbnDiscSPRINGpropertyMaxDist.TrimRight();
		}
		rsX->get_Collect((_variant_t)_T("Spring_id"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnSPRINGid=_T("");
		else{
			tbnSPRINGid=vtos(tmpvar);
			tbnSPRINGid.TrimLeft();tbnSPRINGid.TrimRight();
		}
		rsX->get_Collect((_variant_t)_T("Spring_Crude"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnSPRINGCrude=_T("");
		else{
			tbnSPRINGCrude=vtos(tmpvar);
			tbnSPRINGCrude.TrimLeft();tbnSPRINGCrude.TrimRight();
		}
		rsX->get_Collect((_variant_t)_T("CrudeTurnbuckle"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnSPRINGL5Crude=_T("");
		else{
			tbnSPRINGL5Crude=vtos(tmpvar);
			tbnSPRINGL5Crude.TrimLeft();tbnSPRINGL5Crude.TrimRight();
		}
		//�����жϵ���ԭʼ���ݱ������ݿ����Ƿ����
		rsX->get_Collect((_variant_t)_T("DiameterSerial"),&tmpvar);
		if(tmpvar.vt==VT_NULL)
			tbnSPRDiameterSerial=_T("");
		else{
			tbnSPRDiameterSerial=vtos(tmpvar);
			tbnSPRDiameterSerial.TrimLeft();tbnSPRDiameterSerial.TrimRight();
		}
		if(!HStbExists(db,db1,tbnSPRDiameterSerial,_T("phsManuSPRING"),_T("DiameterSerial"),gsPhsSPRINGSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		if(!HStbExists(db,db1,tbnSPRINGCrude,_T("phsManuSPRING"),_T("Spring_Crude"),gsPhsSPRINGSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		if(!HStbExists(db,db1,tbnSPRINGL5Crude,_T("phsManuSPRING"),_T("CrudeTurnbuckle"),gsPhsSPRINGSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		if(!HStbExists(db,db1,tbnSPRINGid,_T("phsManuSPRING"),_T("Spring_id"),gsPhsSPRINGSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		if(!HStbExists(db,db1,tbnSPRINGproperty,_T("phsManuSPRING"),_T("Spring_property"),gsPhsSPRINGSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		if(!HStbExists(db,db1,tbnDiscSPRINGpropertyMaxDist,_T("phsManuSPRING"),_T("Spring_propertyMaxDist"),gsPhsSPRINGSel,bWarn))
		{	
			if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			{
				rsX->MoveFirst();
				inttbExist=2;
			}
			else					//���ǵ�һ�Σ������һ����¼
				rsX->MoveNext();
			continue;
		}
		inttbExist=1;
	}
	if(inttbExist!=1)
		goto errH;
	inttbExist=0;
	   rsX->get_Collect((_variant_t)_T("SerialNumPerSPR"),&tmpvar);
	   if(tmpvar.vt==VT_NULL)
		   giAllowedMaxSerialNumPerSPR=0;
	   else{
		   giAllowedMaxSerialNumPerSPR=vtoi(tmpvar);
		   //   giAllowedMaxSerialNumPerSPR.TrimLeft();giAllowedMaxSerialNumPerSPR.TrimRight();
	   }
	   rsX->get_Collect((_variant_t)_T("SerialNumSPRs"),&tmpvar);
	   if(tmpvar.vt==VT_NULL)
		   giAllowedMaxSerialNumSPRs=0;
	   else{
		   giAllowedMaxSerialNumSPRs=vtoi(tmpvar);
		   //giAllowedMaxSerialNumSPRs.TrimLeft();giAllowedMaxSerialNumSPRs.TrimRight();
	   }
	   //�������˹淶����������֮��
	   if((bSPECchanged || gbStartedOnceAPHS) && !(gsPhsSPRINGSel == gsOldPhsSPRINGSel))
		   CreateTmpSPRPropertyTable(giAllowedMaxSerialNumSPRs);

	   //��õ��ɻ�������ɱ�Ź��ĵ�λ��
	   //�����жϺ�������/���ɹ��������λ�������ֶ�ֵ�Ƿ����
	   rsX->get_Collect((_variant_t)_T("Unit_Force"),&tmpvar);
	   if(tmpvar.vt==VT_NULL)
	   {
		   lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		   sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuSPRING"),gsPhsSPRINGSel ,_T("Unit_Force"));
		   ShowMessage(sTmp);
		   goto errH;
	   }
	   else{
		   sTmp=vtos(tmpvar);
		   sTmp.TrimLeft();sTmp.TrimRight();
		   if(sTmp.Left(1)==_T("N"))
		   {
			   if(!modPHScal::gbSPRBHLOADUNITBYNEWTON)
			   {
				   gsSPRUnitOfLoad = _T("kgf");
				   rsX->put_Collect((_variant_t)_T("Unit_Force"),STR_VAR("kgf"));rsX->Update();
				   //���û�е��룬ֻ�޸�ģ����µ�ԭʼ���ݿ⣻
				   //�޸ķ�����
				   rsX->get_Collect((_variant_t)_T("Folder"),&tmpvar);
				   
				   if(tmpvar.vt==VT_NULL||vtos(tmpvar)=="")
					   ;
				   else
				   {
						//����е��룬��Ҫ�޸ĵ���·���µ�ԭʼ���ݿ⣻
				   }
			   }
			   else
			   {
					gsSPRUnitOfLoad = _T("N");
			   }

		   }
		   else if(sTmp.Left(3)==_T("daN"))
			   gsSPRUnitOfLoad = _T("daN");
		   else if(sTmp.Left(3)==_T("kgf"))
		   {
			   if(modPHScal::gbSPRBHLOADUNITBYNEWTON)
			   {
				   gsSPRUnitOfLoad = _T("N");
				   rsX->put_Collect((_variant_t)_T("Unit_Force"),_variant_t("N"));
				   rsX->Update();
				   //���û�е��룬ֻ�޸�ģ����µ�ԭʼ���ݿ⣻
				   //�޸ķ�����
				   rsX->get_Collect((_variant_t)_T("Folder"),&tmpvar);
				   
				   if(tmpvar.vt==VT_NULL||vtos(tmpvar)=="")
					   ;
				   else
				   {
						//����е��룬��Ҫ�޸ĵ���·���µ�ԭʼ���ݿ⣻
				   }
			   }
			   else
			   {
					gsSPRUnitOfLoad = _T("kgf");
			   }
		   }
		   else  
		   {//û���κκ��ص�λ��ʶ���ַ�
			   sTmp.Format(GetResStr(IDS_NotFoundUnitStringInBHFormat),EDIBgbl::GetDBName(db1),_T("phsManuSPRING"),GetResStr(IDS_SPR),vtos(tmpvar));
			   ShowMessage(sTmp);
			   gsSPRUnitOfLoad = _T("daN");
		   }
	   }
	   rsX->get_Collect((_variant_t)_T("DOWNchar"),&tmpvar);
	   if(tmpvar.vt==VT_NULL)
		   gSprDirectionChar.SPR_Down=_T("");
	   else
	   {
		   gSprDirectionChar.SPR_Down=vtos(tmpvar);
		   gSprDirectionChar.SPR_Down.TrimLeft();gSprDirectionChar.SPR_Down.TrimRight();
	   }
	   rsX->get_Collect((_variant_t)_T("UPchar"),&tmpvar);
	   if(tmpvar.vt==VT_NULL)
		   gSprDirectionChar.SPR_UP=_T("");
	   else
	   {
		   gSprDirectionChar.SPR_UP=vtos(tmpvar);
		   gSprDirectionChar.SPR_UP.TrimLeft();gSprDirectionChar.SPR_UP.TrimRight();
	   }
	   //�����жϵ���/���ɷ��������ֶ�ֵ�Ƿ����
	   if (gSprDirectionChar.SPR_Down.GetLength()<=0 || gSprDirectionChar.SPR_Down == _T("") )
	   {
		   gSprDirectionChar.SPR_Down = _T("");
		   lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		   sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuSPRING"),gsPhsSPRINGSel,_T("DOWNchar"));
		   //ShowMessage(sTmp); //by ligb on 2008.05.10 2007����Դ�������ɷ����ַ�������-�ű�ʾ����λ�����Ѿ�����-�ţ������·������ֻ���ÿձ�ʾ������޸Ĵ�����Ӧ�����
		   //goto errH;
	   }
	   if (gSprDirectionChar.SPR_UP.GetLength()<=0 || gSprDirectionChar.SPR_UP == _T("") )
	   {
		   gSprDirectionChar.SPR_UP = _T("");
		   lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		   sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuSPRING"),gsPhsSPRINGSel,_T("UPchar"));
		   //ShowMessage(sTmp); //by ligb on 2008.05.10 2007����Դ�������ɷ����ַ�������-�ű�ʾ����λ�����Ѿ�����-�ţ������·������ֻ���ÿձ�ʾ������޸Ĵ�����Ӧ�����
		   //goto errH;
	   }
	   
	   //�����жϵ���/���ɰ�װ�ߴ������ֶ�ֵ�Ƿ����
	   rsX->get_Collect((_variant_t)_T("H1IncludeB"), &tmpvar);
	   if( tmpvar.vt==VT_NULL) {
		   gbH1IncludeB = TRUE;
		   lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		   sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuSPRING"), gsPhsSPRINGSel, _T("H1IncludeB"));
		   ShowMessage(sTmp);
		   goto errH;
	   }else{
		   gbH1IncludeB = tmpvar.boolVal;
	   }

	   rsX->get_Collect((_variant_t)_T("H1Pos"),&tmpvar);
	   if(tmpvar.vt==VT_NULL){
		   gH1Pos = 0;
		   lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		   sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuSPRING"), gsPhsSPRINGSel, _T("H1Pos"));
		   ShowMessage(sTmp);
		   goto errH;
	   }else{
		   gH1Pos = vtoi(tmpvar);
	   }

	   rsX->get_Collect((_variant_t)_T("H1distPos"),&tmpvar);
	   if(tmpvar.vt==VT_NULL){
		   gH1distPos = 0;
		   lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		   sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuSPRING"), gsPhsSPRINGSel, _T("H1distPos"));
		   ShowMessage(sTmp);
		   goto errH;
	   }else{
		   gH1distPos =vtoi(tmpvar);
	   }
	   rsX->get_Collect((_variant_t)_T("manufactory"),&tmpvar);
	   if(tmpvar.vt==VT_NULL)
		   gsSPRmanufactory=_T("");
	   else{
		   gsSPRmanufactory=vtos(tmpvar);
		   gsSPRmanufactory.TrimLeft();gsSPRmanufactory.TrimRight();
	   }

	   //���ڻ�ȡ����������Ϣ...
	   sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoCSPRING));
	   frmStatus.m_Label1 = sTmp;
	   frmStatus.UpdateStatus(6.0 /9.0,true);
	   frmStatus.UpdateData(false);
	   frmStatus.ShowWindow(SW_SHOW);
	   frmStatus.UpdateWindow();
	   //��������/����ԭʼ����
	   if(rsX->State == adStateOpen)
		   rsX->Close();
	   SQLx = _T("SELECT * FROM phsManuConstantSPRING ORDER BY [Observation],[standard] ASC");
	   rsX->Open(_variant_t(SQLx),(IDispatch*)EDIBgbl::dbSORT,
		   adOpenStatic, adLockOptimistic,adCmdText);
	   gsPhsConstantSpringSel.TrimLeft();gsPhsConstantSpringSel.TrimRight(); 
	   /*if( rsX->BOF && rsX->adoEOF ){
	   lngErrNum = IDS_NotFoundThisStandardInSortMdb;
	   sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuBoltsNuts"),gsPhsBoltsNutsSel);
	   ShowMessage(sTmp);
	   goto errH;
         }*/
	   if(gsPhsConstantSpringSel != _T("") ){
		   HRESULT hr = S_OK;
		   rsX->MoveFirst();
		   strTmp = _T("standard=\'")+gsPhsConstantSpringSel+_T("\'");
		   hr = rsX->Find((_bstr_t)strTmp, 0, adSearchForward);
		   if( rsX->adoEOF)
		   {			
			   rsX->MoveFirst();
				inttbExist=2;
		   }
	   }
	   else
	   {
		   rsX->MoveFirst();
		   inttbExist=2;
	   }
	   //�����жϺ�������/��ԭʼ���ݱ������ݿ����Ƿ����,
	   //��������ڣ��򽫼�¼ָ���Ƶ���һ����¼�ϣ���ͷ��ʼ���ÿ����¼
	   //������еļ�¼��ԭʼ���ݱ������ݿ��ж�������,�򱨴�
	   //�������ԭʼ���ݱ������ݿ��д��ڵļ�¼,��ѡ��������¼
	   while((inttbExist!=1)&&(!rsX->adoEOF))
	   {
		   rsX->get_Collect((_variant_t)_T("standard"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   gsPhsConstantSpringSel=_T("");
		   else{
			   gsPhsConstantSpringSel=vtos(tmpvar);
			   gsPhsConstantSpringSel.TrimLeft();gsPhsConstantSpringSel.TrimRight();
		   }
		   rsX->get_Collect((_variant_t)_T("manufactory"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   gsCSPRmanufactory=_T("");
		   else{
			   gsCSPRmanufactory=vtos(tmpvar);
			   gsCSPRmanufactory.TrimLeft();gsCSPRmanufactory.TrimRight();
		   }
		   rsX->get_Collect((_variant_t)_T("Spring_Crude"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   tbnHDCrude=_T("");
		   else{
			   tbnHDCrude=vtos(tmpvar);
			   tbnHDCrude.TrimLeft();tbnHDCrude.TrimRight();
		   }
		   rsX->get_Collect((_variant_t)_T("Spring_property"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   tbnHDproperty=_T("");
		   else{
			   tbnHDproperty=vtos(tmpvar);
			   tbnHDproperty.TrimLeft();tbnHDproperty.TrimRight();
		   }
		   rsX->get_Collect((_variant_t)_T("Spring_id"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   tbnHDid=_T("");
		   else{
			   tbnHDid=vtos(tmpvar);
			   tbnHDid.TrimLeft();tbnHDid.TrimRight();
		   }
		   rsX->get_Collect((_variant_t)_T("CrudeTurnbuckle"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   tbnCSPRINGL5Crude=_T("");
		   else{
			   tbnCSPRINGL5Crude=vtos(tmpvar);
			   tbnCSPRINGL5Crude.TrimLeft();tbnCSPRINGL5Crude.TrimRight();
		   }
		   rsX->get_Collect((_variant_t)_T("LugDiaOfCSPR"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   tbnLugDiaOfCSPR=_T("");
		   else{
			   tbnLugDiaOfCSPR=vtos(tmpvar);
			   tbnLugDiaOfCSPR.TrimLeft();tbnLugDiaOfCSPR.TrimRight();
		   }
		   rsX->get_Collect((_variant_t)_T("DisplacementSerial"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   tbnDisplacementSerial=_T("");
		   else{
			   tbnDisplacementSerial=vtos(tmpvar);
			   tbnDisplacementSerial.TrimLeft();tbnDisplacementSerial.TrimRight();
		   }
		   rsX->get_Collect((_variant_t)_T("DiameterSerial"),&tmpvar);
		   if(tmpvar.vt==VT_NULL)
			   tbnCSPRDiameterSerial=_T("");
		   else{
			   tbnCSPRDiameterSerial=vtos(tmpvar);
			   tbnCSPRDiameterSerial.TrimLeft();tbnCSPRDiameterSerial.TrimRight();
		   }


		   if(!HStbExists(db,db1,tbnCSPRDiameterSerial,_T("phsManuConstantSPRING"),_T("DiameterSerial"),gsPhsConstantSpringSel,bWarn))
		   {	
			   if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			   {
				   rsX->MoveFirst();
				   inttbExist=2;
			   }
			   else					//���ǵ�һ�Σ������һ����¼
				   rsX->MoveNext();
			   continue;
		   }
		   if(!HStbExists(db,db1,tbnHDCrude,_T("phsManuConstantSPRING"),_T("Spring_Crude"),gsPhsConstantSpringSel,bWarn))
		   {	
			   if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			   {
				   rsX->MoveFirst();
				   inttbExist=2;
			   }
			   else					//���ǵ�һ�Σ������һ����¼
				   rsX->MoveNext();
			   continue;
		   }
		   if(!HStbExists(db,db1,tbnHDid,_T("phsManuConstantSPRING"),_T("Spring_id"),gsPhsConstantSpringSel,bWarn))
		   {	
			   if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			   {
				   rsX->MoveFirst();
				   inttbExist=2;
			   }
			   else					//���ǵ�һ�Σ������һ����¼
				   rsX->MoveNext();
			   continue;
		   }
		   if(!HStbExists(db,db1,tbnHDproperty,_T("phsManuConstantSPRING"),_T("Spring_property"),gsPhsConstantSpringSel,bWarn))
		   {	
			   if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			   {
				   rsX->MoveFirst();
				   inttbExist=2;
			   }
			   else					//���ǵ�һ�Σ������һ����¼
				   rsX->MoveNext();
			   continue;
		   }
		   if(!HStbExists(db,db1,tbnLugDiaOfCSPR,_T("phsManuConstantSPRING"),_T("Spring_property"),gsPhsConstantSpringSel,bWarn))
		   {	
			   if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			   {
				   rsX->MoveFirst();
				   inttbExist=2;
			   }
			   else					//���ǵ�һ�Σ������һ����¼
				   rsX->MoveNext();
			   continue;
		   }
		   if(!HStbExists(db,db1,tbnDisplacementSerial,_T("phsManuConstantSPRING"),_T("DisplacementSerial"),gsPhsConstantSpringSel,bWarn))
		   {	
			   if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			   {
				   rsX->MoveFirst();
				   inttbExist=2;
			   }
			   else					//���ǵ�һ�Σ������һ����¼
				   rsX->MoveNext();
			   continue;
		   }
		   if(!HStbExists(db,db1,tbnCSPRINGL5Crude,_T("phsManuConstantSPRING"),_T("CrudeTurnbuckle"),gsPhsConstantSpringSel,bWarn))
		   {	
			   if(inttbExist==0)	//��һ���Ƶ���һ����¼��
			   {
				   rsX->MoveFirst();
				   inttbExist=2;
			   }
			   else					//���ǵ�һ�Σ������һ����¼
				   rsX->MoveNext();
			   continue;
		   }
		   inttbExist=1;
	}
	if (inttbExist!=1)
		goto errH;
	inttbExist=0;
	//�����жϺ�������/���ɹ��������λ�������ֶ�ֵ�Ƿ����

	rsX->get_Collect((_variant_t)_T("Unit_Force"),&tmpvar);
	if(tmpvar.vt==VT_NULL)
	{
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuConstantSPRING"),gsPhsConstantSpringSel ,_T("Unit_Force"));
		ShowMessage(sTmp);
		goto errH;
	}
	else{
		sTmp=vtos(tmpvar);
		sTmp.TrimLeft();sTmp.TrimRight();
		if(sTmp.Left(1)==_T("N"))
			gsCSPRUnitOfLoad = _T("N");
		else if(sTmp.Left(3)==_T("daN"))
			gsCSPRUnitOfLoad = _T("daN");
		else if(sTmp.Left(3)==_T("kgf"))
			gsCSPRUnitOfLoad = _T("kgf");
		else
			//û���κκ��ص�λ��ʶ���ַ�
		{
			sTmp.Format(GetResStr(IDS_NotFoundUnitStringInBHFormat),EDIBgbl::GetDBName(db1),_T("phsManuConstantSPRING"),GetResStr(IDS_CSPR),vtos(tmpvar));
			ShowMessage(sTmp);
			gsCSPRUnitOfLoad = _T("daN");
		}

	}

	//���պ���/λ��ѡ��������ɵ�����ֱ��TRUE/FALSE
	//�����жϺ�������/���ɰ�װ�ߴ������ֶ�ֵ�Ƿ����
	rsX->get_Collect((_variant_t)_T("bByForce"), &tmpvar);
	if(tmpvar.vt==VT_NULL ){
		gbByForce = TRUE;
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuConstantSPRING"), gsPhsConstantSpringSel, _T("bByForce"));
		ShowMessage(sTmp);
		goto errH;
	}else{
		gbByForce = tmpvar.boolVal;
	}
	//�����жϺ�������/���ɰ�װ�ߴ������ֶ�ֵ�Ƿ����
	rsX->get_Collect((_variant_t)_T("bE_FormulaPlusHalf"), &tmpvar);
	if(tmpvar.vt==VT_NULL ){
		bE_FormulaPlusHalf = false;
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuConstantSPRING"), gsPhsConstantSpringSel, _T("bE_FormulaPlusHalf"));
		ShowMessage(sTmp);
		goto errH;
	}else{
		bE_FormulaPlusHalf = tmpvar.boolVal;
	}


	rsX->get_Collect((_variant_t)_T("H1IncludeB"), &tmpvar);
	if(tmpvar.vt==VT_NULL )
	{
		gbCH1IncludeB = TRUE;
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuConstantSPRING"), gsPhsConstantSpringSel, _T("H1IncludeB"));
		ShowMessage(sTmp);
		goto errH;
	}
	else
	{
		gbCH1IncludeB = vtob(tmpvar);//tmpvar.boolVal
	}

	rsX->get_Collect((_variant_t)_T("bH1IncludeT"), &tmpvar);
	if(tmpvar.vt==VT_NULL )
	{
		gbCH1IncludeT = TRUE;
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuConstantSPRING"), gsPhsConstantSpringSel, _T("bH1IncludeT"));
		ShowMessage(sTmp);
		goto errH;
	}
	else
	{
		gbCH1IncludeT = vtob(tmpvar);//tmpvar.boolVal
	}

	rsX->get_Collect((_variant_t)_T("H1Pos"), &tmpvar);
	if(tmpvar.vt==VT_NULL )
	{
		gCH1Pos = 0;
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuConstantSPRING"), gsPhsConstantSpringSel, _T("H1Pos"));
		ShowMessage(sTmp);
		goto errH;
	}
	else
	{
		gCH1Pos = vtoi(tmpvar);
	}
	rsX->get_Collect((_variant_t)_T("H1distPos"), &tmpvar);
	if(tmpvar.vt==VT_NULL ){
		gCH1distPos = 0;
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum), EDIBgbl::GetDBName(db1), _T("phsManuConstantSPRING"), gsPhsConstantSpringSel, _T("H1distPos"));
		ShowMessage(sTmp);
		goto errH;
	}else{
		gCH1distPos = vtoi(tmpvar);
	}

	rsX->get_Collect((_variant_t)_T("DOWNchar"),&tmpvar);
	if(tmpvar.vt==VT_NULL)
		gSprDirectionChar.CSPR_Down=_T("");
	else{
		gSprDirectionChar.CSPR_Down=vtos(tmpvar);
		gSprDirectionChar.CSPR_Down.TrimLeft();gSprDirectionChar.CSPR_Down.TrimRight();
	}
	rsX->get_Collect((_variant_t)_T("UPchar"),&tmpvar);
	if(tmpvar.vt==VT_NULL)
		gSprDirectionChar.CSPR_UP=_T("");
	else{
		gSprDirectionChar.CSPR_UP=vtos(tmpvar);
		gSprDirectionChar.CSPR_UP.TrimLeft();gSprDirectionChar.CSPR_UP.TrimRight();
	}
	//�����жϺ�������/�����������ֶ�ֵ�Ƿ����
	if( gSprDirectionChar.CSPR_Down.GetLength()<=0 || gSprDirectionChar.CSPR_Down == _T("") ){
		gSprDirectionChar.CSPR_Down = _T("");
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuConstantSPRING"),gsPhsConstantSpringSel,_T("DOWNchar"));
		//ShowMessage(sTmp);
		//goto errH;
	}
	if( gSprDirectionChar.CSPR_UP.GetLength()<=0 || gSprDirectionChar.CSPR_UP == _T("") ){
		gSprDirectionChar.CSPR_UP = _T("");
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),_T("phsManuConstantSPRING"),gsPhsConstantSpringSel,_T("UPchar"));
		//ShowMessage(sTmp);
		//goto errH;
	}
	
	//Ĭ����λ�Ƽ���ԣ��Ϊ20%
	rsX->get_Collect((_variant_t)_T("PercentSurplusDist"), &tmpvar);
	if( tmpvar.vt==VT_NULL){
		rsX->put_Collect((_variant_t)_T("PercentSurplusDist"),_variant_t((float)0.2));
		//rsX->PutCollect(_T("PercentSurplusDist"),_variant_t((float)0.2));
		
	}
	//Ĭ����λ�Ƽ���ԣ����СֵΪ15mm
	rsX->get_Collect((_variant_t)_T("MinSurplusDist"), &tmpvar);
	if( tmpvar.vt==VT_NULL){
		rsX->put_Collect((_variant_t)_T("MinSurplusDist"),_variant_t((float)15));
		// rsX->PutCollect(_T("MinSurplusDist"),_variant_t((float)15));
	}
	rsX->get_Collect((_variant_t)_T("PercentSurplusDist"),&tmpvar);
	if(tmpvar.vt==VT_NULL)
		gUseConstantSpringPercentSurplusDist=0;
	else{
		gUseConstantSpringPercentSurplusDist=tmpvar.fltVal;
		//   gUseConstantSpringPercentSurplusDist.TrimLeft();gUseConstantSpringPercentSurplusDist.TrimRight();
	}
	rsX->get_Collect((_variant_t)_T("MinSurplusDist"),&tmpvar);
	if(tmpvar.vt==VT_NULL)
		gUseConstantSpringMinSurplusDist=0;
	else{
		gUseConstantSpringMinSurplusDist=tmpvar.fltVal;
		//  gUseConstantSpringMinSurplusDist.TrimLeft();gUseConstantSpringMinSurplusDist.TrimRight();
	}
	
	//���ڻ�ȡ��ʱCustomID��Ϣ...
	sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoTmpCustomIDForPART));
	frmStatus.m_Label1 = sTmp;
	frmStatus.UpdateStatus(7.0 /9.0,true);
	frmStatus.UpdateData(false);
	frmStatus.UpdateWindow();
	//������ǰ��׼CustomID��
	frmStatus.m_Label1 = _T("������ǰ��׼CustomID��");
	frmStatus.UpdateStatus(8.0 /9.0,true);
	frmStatus.UpdateData(false);
	frmStatus.UpdateWindow();
	modPHScal::CreateTmpCustomIDForPART();
	//���ڻ�ȡ��ǰ��׼CustomID��Ϣ...
	sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoCustomID));
	frmStatus.m_Label1 = sTmp;
	frmStatus.UpdateStatus(8.0 /9.0,true);
	frmStatus.UpdateData(false);
	frmStatus.UpdateWindow();
	//��dbprj�н�����ʱID-CustomID���ձ�PictureClipData,
	CreateTmpIDCustomIDTable();
	//��dbprj�н�����ʱ֧��������׼���connect,
	//���ڻ�ȡTmpConnectTable��Ϣ...
	sTmp.Format(IDS_GetDataTableInfo,GetResStr(IDS_InfoTmpConnectTable));
	frmStatus.m_Label1 = sTmp;
	frmStatus.UpdateStatus(9.0 /9.0,true);
	frmStatus.UpdateData(false);
	frmStatus.UpdateWindow();
	CreateTmpConnectTable();
	//���볧�����ݿ��е�CONNECTxx��     LFX 2005.4.19��
	//�˲���������� 	CreateTmpConnectTable(); ���֮��
	CString strDBTableName;
	CString strPRJTableName;
	CString strSQL;
	while (true)
	{
		strDBTableName=tbnCONNECT;
		if (tbnCONNECT == strStandardConnect)
		{
			//�����ǰѡ�����Ӽ�û���Դ����ӱ���ʹ��ϵͳĬ�����ӱ�
			//strSQL.Format("DELETE * FROM CONNECT");
			//EDIBgbl::dbPRJ->Execute((_bstr_t)strSQL);
			//strSQL.Format("INSERT INTO CONNECT SELECT * FROM %s IN '%s'",strStandardConnect,db));
			//EDIBgbl::dbPRJ->Execute((_bstr_t)strSQL);
		}
		else
		{
			//��ǰѡ�����Ӽ��Դ������ӱ���ϵͳ���ӱ�����ѡ�����Ӽ��Դ����ӱ�
			//�� CNTB,CNTE,CNTN �ֶ���ͬ�ļ�¼ɾ��������ѡ�����Ӽ��Դ������ӱ�
			//�еļ�¼���Ƶ�ϵͳ���ӱ���
			strPRJTableName.Format("[connect]");
			if( EDIBgbl::tdfExists(db, strDBTableName) )
			{
				strSQL.Format("DELETE * FROM %s AS A WHERE EXISTS (SELECT * FROM %s AS B IN '%s'\
					where a.cntb = b.cntb and a.cnte = b.cnte and \
					(a.cntn = b.cntn or (a.cntn is null and b.cntn is null)))",
					strPRJTableName,strDBTableName,EDIBgbl::GetDBName(db));
				EDIBgbl::dbPRJ->Execute((_bstr_t)strSQL, NULL, adCmdText);
				
				//���볧������
				strSQL.Format("INSERT INTO %s IN '%s' Select * FROM %s",
					strPRJTableName,EDIBgbl::GetDBName(EDIBgbl::dbPRJ),strDBTableName);
				db->Execute((_bstr_t)strSQL, NULL, adCmdText);
			}
		}
		break;
	}
	//END ���볧�����ݿ��е�CONNECT��     LFX 2005.4.19��			
	//
	

	//�淶�ı䲢��������֮��Ž����¶δ������������ݱ�

	if(bSPECchanged && gbStartedOnceAPHS 
		|| !EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("PhsStructureNAME")) 
		|| !EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("PhsStructureREF"))
		|| modPHScal::bInstalledAndFirstRun )
	{
		//��dbprj�н�����dbsort�ı�PhsStructureREF��PhsStructureName����ͬ�Ŀ���,
		//�û�������dbPrj,����ģ���Ҫͬʱ�޸�dbsort��dbPrj����ֹ����û�ѡ��ͬ�Ĺ淶������ͻ.
		CreatePhsStructureNameAndREFIndbPRJ();
		//��dbPRJ�ı�PhsStructureREF������ɨ�裬
		//����ѡ��Ĺ淶�ͱ�׼���б��ػ����޸�CustomID���¹淶һ�¡�
		Cavphs->CheckAllCustomIDandIDinPhsStructureREF();
		//����ÿ�ν�������Ķ���
		//��dbPRJ�ı�PhsStructureName������ɨ�裬
		//����ѡ��Ĺ淶�ͱ�׼���б��ػ������޸����Ƶ����¹淶��ص�ֵ��

		Cavphs->ChangeNameInphsStructureName();

	}
	if( ! gbStartedOnceAPHS ){
		//������־����ֹ��������ظ�����
		gbStartedOnceAPHS = TRUE;
	}
	
	//����ǰ�ι����淶����ֹ���½���ʱ�ٴμ�����ͬ�淶���˷�ʱ�䡣
	gsOldPhsConstantSpringSel = gsPhsConstantSpringSel;
	gsOldPhsSPRINGSel = gsPhsSPRINGSel;
	gsOldPhsPASel = gsPhsPASel;
	gsOldPhsPARTSel = gsPhsPARTSel;
	gsOldPhsSASel = gsPhsSASel;
	gsOldPhsBoltsNutsSel = gsPhsBoltsNutsSel;
	
	ret = TRUE;
   }
   
   catch (_com_error &e)
   {
	   CString strMsg;
	   strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
	   AfxMessageBox(strMsg);
	   ret = FALSE;
	   //Debug.Print SQLx
	   // ShowMessage(e.Description());
   }
errH:
   if(rsX->State == adStateOpen)
	   rsX->Close();
   rsX.Release();

   if(db->State == adStateOpen)
	   db->Close();
	db.Release();
	
   if(db1->State == adStateOpen)
	   db1->Close();
   db1.Release();

   AfxGetApp()->EndWaitCursor();
   frmStatus.ShowWindow(SW_HIDE);
   return ret;

}


void modPHScal::ImportDataFromZdjCrudeXXXX(CString  strFN, bool  bReplacement, bool & mbSPECchanged)
{
	//Ŀ�ģ��ӳ����ṩ�Ĳ�Ʒ���ݿ��е������ݻ��ڲ�Ʒ���ݿ��м�����ݱ��Ƿ���ȫ
	AfxGetApp()->BeginWaitCursor();

	CString ConnectionString;
	try
	{
		if(modPHScal::dbZDJcrude->State == adStateOpen)
		{
			modPHScal::dbZDJcrude->Close();
		}
		ConnectionString.Format("Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s;Jet OLEDB:Database Password=%s",
			basDirectory::ProjectDBDir+_T("zdjcrude.mdb"), ModEncrypt::gstrDBZdjCrudePassWord);
		modPHScal::dbZDJcrude->Open((_bstr_t)ConnectionString, "", "", adConnectUnspecified);
	}catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
		return;
	}

//	try {
		CString strTbn, strCrudeTbn;
		CMessageBoxEx MsgDlg;
		bool bAllYes=false;
		bool bAllNo=false;
		UINT nDlgID=0;

		long i=0;
		bool bFound=false;
		_ConnectionPtr db;
		db.CreateInstance(__uuidof(Connection));
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rs2;
		rs2.CreateInstance(__uuidof(Recordset));
		strFN.TrimLeft();
		strFN.TrimRight();
		CString strTmp;
		_variant_t vTmp;
		CString SQLx;
		

			
		if(strFN == _T("") )
		{
			throw GetResStr(IDS_NoSelectFileName);
		}
		else
		{
			ConnectionString.Format("Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s;Jet OLEDB:Database Password=%s",
				strFN, ModEncrypt::gstrDBZdjCrudePassWord);
		try {
			db->Open((_bstr_t)ConnectionString, "", "", adConnectUnspecified);
		}
		catch (_com_error &e)
		{
			CString strMsg;
			strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
			AfxMessageBox(strMsg);
			return;
		}
			//ȡ������������
			CString strSql;
			strSql = _T("SELECT * FROM phsManu ORDER BY seq");
		try {
			rs->Open((_bstr_t)strSql, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
		}
		catch (_com_error &e)
		{
			CString strMsg;
			strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
			AfxMessageBox(strMsg);
			return;
		}
			if( rs->adoEOF && rs->BOF )
			{
				strTmp.Format(GetResStr(IDS_NullPhsManuTableInSortMdb),EDIBgbl::GetDBName(EDIBgbl::dbSORT));
				throw strTmp;
			}
			while(!rs->adoEOF)
			{
				rs->get_Collect((_variant_t)_T("TableName"),&vTmp);
				strTbn = vtos(vTmp);
				if( EDIBgbl::tdfExists(db, strTbn) )
				{
					//����ȫ��������
					bFound = true;
					rs->get_Collect((_variant_t)_T("SQL"),&vTmp);
					//����Ҫ�ж��Ƿ����е�ԭʼ���ݱ����ڣ������ڵ�Ҫ�ų�������д�������б�򣬷�ֹ�û�������
					try {
					rs1->Open((_bstr_t)vTmp, _variant_t((IDispatch*)db,true), 
						adOpenStatic, adLockOptimistic, adCmdText); 
					}
					catch (_com_error &e)
					{
						CString strMsg;
						strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
						AfxMessageBox(strMsg);
						return;
					}
					if( rs1->adoEOF || rs1->BOF ){
						strTmp.Format(GetResStr(IDS_NullPhsManuXtableInProductMdb),strTbn);
						ShowMessage(strTmp);
					}
					else
					{
						while(!rs1->adoEOF)
						{
							rs->get_Collect((_variant_t)_T("CrudeDataTableNum"),&vTmp);
							int C=vtoi(vTmp);
							for( i = 1 ;i<= C;i++)
							{
								rs1->get_Collect((_variant_t)i,&vTmp);
								if( vTmp.vt!=VT_NULL)
								{
									strCrudeTbn = vtos(vTmp);
									if( EDIBgbl::tdfExists(db, strCrudeTbn) )
									{
										//������ڳ����ṩ�����ݿ��ԭʼ���ݿ��д���
										if( EDIBgbl::tdfExists(modPHScal::dbZDJcrude, strCrudeTbn) )
										{
											if( bReplacement )
											{
												strTmp.Format(GetResStr(IDS_ExistsThisCrudeDataTableInZDJcrudeOrproductMdb),EDIBgbl::GetDBName(modPHScal::dbZDJcrude),strCrudeTbn);
												MsgDlg.m_strPrompt=strTmp;
												if(! bAllYes)
												{
													nDlgID=IDALLYES;
													if(nDlgID==IDALLYES)
													{
														AfxGetApp()->BeginWaitCursor();
														bAllYes=true;
													}
													else if(nDlgID == IDALLNO)
													{
														bAllNo=true;
														if(rs1->State == adStateOpen) rs1->Close();
														if(rs2->State == adStateOpen) rs2->Close();
														if(rs->State == adStateOpen) rs->Close();
														if(db->State == adStateOpen) db->Close();
														AfxGetApp()->EndWaitCursor();
														return;
													}
												}
												if(bAllYes || nDlgID== IDYES)
												{
													SQLx = _T("drop table ") + strCrudeTbn;
													try {
													dbZDJcrude->Execute((_bstr_t)SQLx, NULL, adCmdText);
													}
													catch (_com_error &e)
													{
														CString strMsg;
														strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
														AfxMessageBox(strMsg);
														return;
													}

													SQLx = "Select * INTO [" + strCrudeTbn + _T("] IN \'") + EDIBgbl::GetDBName(modPHScal::dbZDJcrude) + _T("\'  FROM ") + strCrudeTbn;
													try {
													db->Execute((_bstr_t)SQLx, NULL, adCmdText);
													mbSPECchanged = true;
													}
													catch (_com_error &e)
													{
														CString strMsg;
														strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
														AfxMessageBox(strMsg);
														return;
													}
												}

											}
										}
										else
										{
											if( bReplacement ){

												//ZDJCrude.mdb�в������������Ӳ�Ʒ���е�����
												//ADO���������,���phs.arx
												SQLx = _T("SELECT * INTO [") + strCrudeTbn + _T("] IN \'") + EDIBgbl::GetDBName(dbZDJcrude) + _T("\' FROM ") + strCrudeTbn;
												try {
												db->Execute((_bstr_t) SQLx, NULL, adCmdText);
												}
												catch (_com_error &e)
												{
													// ��ʱ���ڴ˱�
// 													CString strMsg;
// 													strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
// 													AfxMessageBox(strMsg);
// 													return;
												}
												mbSPECchanged = true;
											}
										}
									}
									else
									{
										//������ڳ����ṩ�����ݿ��в�����
										if( EDIBgbl::tdfExists(dbZDJcrude, strCrudeTbn) )
										{
											//�������ԭʼ���ݿ��д���
										}
										else
										{
											//�������ԭʼ���ݿ��в�����
											strTmp.Format(GetResStr(IDS_NoThisCrudeDataTableInProductMdb),EDIBgbl::GetDBName(dbZDJcrude),strCrudeTbn);
											//������ʾ���û����ա�
											//ShowMessage(strTmp);
										}
									}
								}
							}
							//���¹����
							rs1->get_Collect((_variant_t)_T("standard"),&vTmp);
							SQLx=_T("SELECT * FROM ") + strTbn + _T(" WHERE (standard)=\'") +vtos(vTmp) + _T("\'");
							try {
							rs2->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
								adOpenStatic, adLockOptimistic, adCmdText); 
							//����һ��ʱ���潫Ҫ׷�ӵĲ�ѯ
							if( rs2->adoEOF && rs2->BOF ){
							}else{
								SQLx = _T("DELETE * FROM ") + strTbn + _T(" WHERE (standard)=\'") + vtos(vTmp) + _T("\'");
								EDIBgbl::dbSORT->Execute((_bstr_t) SQLx, NULL, adCmdText);
							}							
							rs2->Close();
							EDIBgbl::UpdateDBTable(db,strTbn,EDIBgbl::dbSORT,strTbn);
							SQLx = _T("INSERT INTO [") + strTbn + _T("] IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbSORT) + _T("\' SELECT * FROM ") + strTbn + _T(" WHERE (standard)=\'") +vtos(vTmp) + _T("\'");
							db->Execute((_bstr_t) SQLx, NULL, adCmdText);
							//������Դ���ļ���Ϣ
							SQLx = _T("UPDATE [") + strTbn + _T("] SET Folder=\'") + strFN + _T("\' WHERE (standard)=\'") + vtos(vTmp) + _T("\'");
							EDIBgbl::dbSORT->Execute((_bstr_t) SQLx, NULL, adCmdText);							
							rs1->MoveNext();
							}
							catch (_com_error &e)
							{
								CString strMsg;
								strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
								AfxMessageBox(strMsg);
								return;
							}
						}
					}
					rs1->Close();
				}
				rs->MoveNext();
			}
// 		}catch (_com_error &e)
// 		{
// 			CString strMsg;
// 			strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
// 			AfxMessageBox(strMsg);
// 		}
			
			//ѭ�����
			strTmp.Format(GetResStr(IDS_FinishedImportProductMdb),strFN);			
			ShowMessage(strTmp);
	}
	AfxGetApp()->EndWaitCursor();
}

void modPHScal::ImportDataFromZdjCrude(CString  strFN, bool  bReplacement, bool & mbSPECchanged)
{
	//Ŀ�ģ��ӳ����ṩ�Ĳ�Ʒ���ݿ��е������ݻ��ڲ�Ʒ���ݿ��м�����ݱ��Ƿ���ȫ
	AfxGetApp()->BeginWaitCursor();
	EXECUTE_TIME_BEGIN
	try
	{
		CString strTbn, strCrudeTbn;
		CMessageBoxEx MsgDlg;
		bool bAllYes=false;
		bool bAllNo=false;
		UINT nDlgID=0;

		long i=0;
		bool bFound=false;
		_ConnectionPtr db;
		db.CreateInstance(__uuidof(Connection));
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rs2;
		rs2.CreateInstance(__uuidof(Recordset));
		strFN.TrimLeft();
		strFN.TrimRight();
		CString strTmp;
		_variant_t vTmp;
		CString SQLx;
		if(strFN == _T("") )
		{
			//MsgBox ResolveResString(iUE_NoSelectFileName)
			//Exit Sub
			throw GetResStr(IDS_NoSelectFileName);
		}
		else
		{
			CString ConnectionString;
			ConnectionString.Format("Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s;Jet OLEDB:Database Password=%s",
				strFN, ModEncrypt::gstrDBProductPassWord);
			db->Open((_bstr_t)ConnectionString, "", "", adConnectUnspecified);
			//ȡ������������
			SQLx = _T("SELECT * FROM phsManu ORDER BY seq");
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			if( rs->adoEOF || rs->BOF )
			{
				//MsgBox ResolveResString(iUE_NullPhsManuTableInSortMdb)
				//Exit Sub
				strTmp.Format(GetResStr(IDS_NullPhsManuTableInSortMdb),EDIBgbl::GetDBName(EDIBgbl::dbSORT));
				throw strTmp;
			}
			while(!rs->adoEOF)
			{
				rs->get_Collect((_variant_t)_T("TableName"),&vTmp);
				strTbn = vtos(vTmp);
				if( EDIBgbl::tdfExists(db, strTbn) )
				{
					//����ȫ��������
					bFound = true;
					rs->get_Collect((_variant_t)_T("SQL"),&vTmp);
					//����Ҫ�ж��Ƿ����е�ԭʼ���ݱ����ڣ������ڵ�Ҫ�ų�������д�������б�򣬷�ֹ�û�������
					rs1->Open((_bstr_t)vTmp, _variant_t((IDispatch*)db,true), 
						adOpenStatic, adLockOptimistic, adCmdText); 
					if( rs1->adoEOF || rs1->BOF ){
						strTmp.Format(GetResStr(IDS_NullPhsManuXtableInProductMdb),strTbn);
						ShowMessage(strTmp);
					}
					else
					{
						while(!rs1->adoEOF)
						{
							rs->get_Collect((_variant_t)_T("CrudeDataTableNum"),&vTmp);
							int C=vtoi(vTmp);
							for( i = 1 ;i<= C;i++)
							{
								rs1->get_Collect((_variant_t)i,&vTmp);
								if( vTmp.vt!=VT_NULL)
								{
									strCrudeTbn = vtos(vTmp);
									//if( strCrudeTbn == _T("BoltsSurplusLength") ){
									//	Debug.Print strCrudeTbn
									//}
									if( EDIBgbl::tdfExists(db, strCrudeTbn) )
									{
										//������ڳ����ṩ�����ݿ��ԭʼ���ݿ��д���
										if( EDIBgbl::tdfExists(modPHScal::dbZDJcrude, strCrudeTbn) )
										{
											if( bReplacement )
											{
												strTmp.Format(GetResStr(IDS_ExistsThisCrudeDataTableInZDJcrudeOrproductMdb),EDIBgbl::GetDBName(modPHScal::dbZDJcrude),strCrudeTbn);
												MsgDlg.m_strPrompt=strTmp;
												//if(IDYES == ShowMessage(strTmp, MB_DEFBUTTON1|MB_ICONQUESTION|MB_SYSTEMMODAL|MB_YESNO) )
												if(! bAllYes)
												{
													nDlgID=MsgDlg.DoModal();
													if(nDlgID==IDALLYES)
													{
														AfxGetApp()->BeginWaitCursor();
														bAllYes=true;
													}
													else if(nDlgID == IDALLNO)
													{
														bAllNo=true;
														if(rs1->State == adStateOpen) rs1->Close();
														if(rs2->State == adStateOpen) rs2->Close();
														if(rs->State == adStateOpen) rs->Close();
														if(db->State == adStateOpen) db->Close();
														AfxGetApp()->EndWaitCursor();
														return;
													}
												}
												if(bAllYes || nDlgID== IDYES)
												{
													SQLx = _T("drop table ") + strCrudeTbn;
													dbZDJcrude->Execute((_bstr_t)SQLx, NULL, adCmdText);

													SQLx = "Select * INTO [" + strCrudeTbn + _T("] IN \'") + EDIBgbl::GetDBName(modPHScal::dbZDJcrude) + _T("\'  FROM ") + strCrudeTbn;
													db->Execute((_bstr_t)SQLx, NULL, adCmdText);
													mbSPECchanged = true;
												}

											}
										}
										else
										{
											if( bReplacement ){
												//ZDJCrude.mdb�в������������Ӳ�Ʒ���е�����
												//ADO���������,���phs.arx
												SQLx = _T("SELECT * INTO [") + strCrudeTbn + _T("] IN \'") + EDIBgbl::GetDBName(dbZDJcrude) + _T("\' FROM ") + strCrudeTbn;
												db->Execute((_bstr_t) SQLx, NULL, adCmdText);
												mbSPECchanged = true;
											}
										}
									}
									else
									{
										//������ڳ����ṩ�����ݿ��в�����
										if( EDIBgbl::tdfExists(dbZDJcrude, strCrudeTbn) )
										{
											//�������ԭʼ���ݿ��д���
										}
										else
										{
											//�������ԭʼ���ݿ��в�����
											strTmp.Format(GetResStr(IDS_NoThisCrudeDataTableInProductMdb),EDIBgbl::GetDBName(dbZDJcrude),strCrudeTbn);
											//������ʾ���û����ա�
											//ShowMessage(strTmp);
										}
									}
								}
							}
							//���¹����
							rs1->get_Collect((_variant_t)_T("standard"),&vTmp);
							SQLx=_T("SELECT * FROM ") + strTbn + _T(" WHERE (standard)=\'") +vtos(vTmp) + _T("\'");
							rs2->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbSORT,true), 
								adOpenStatic, adLockOptimistic, adCmdText); 
							if( rs2->adoEOF || rs2->BOF ){
							}else{
								SQLx = _T("DELETE * FROM ") + strTbn + _T(" WHERE (standard)=\'") + vtos(vTmp) + _T("\'");
								EDIBgbl::dbSORT->Execute((_bstr_t) SQLx, NULL, adCmdText);
							}							
							//alter strTbn table structure before inserting records
							//at first close rs2,otherwise to produce a error,because strTbn is still opened.
							rs2->Close();
							EDIBgbl::UpdateDBTable(db,strTbn,EDIBgbl::dbSORT,strTbn);
							SQLx = _T("INSERT INTO [") + strTbn + _T("] IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbSORT) + _T("\' SELECT * FROM ") + strTbn + _T(" WHERE (standard)=\'") +vtos(vTmp) + _T("\'");
							db->Execute((_bstr_t) SQLx, NULL, adCmdText);
							//������Դ���ļ���Ϣ
							SQLx = _T("UPDATE [") + strTbn + _T("] SET Folder=\'") + strFN + _T("\' WHERE (standard)=\'") + vtos(vTmp) + _T("\'");
							EDIBgbl::dbSORT->Execute((_bstr_t) SQLx, NULL, adCmdText);							
							rs1->MoveNext();
						}
					}
					rs1->Close();
				}
				rs->MoveNext();
			}
			//ѭ�����
			strTmp.Format(GetResStr(IDS_FinishedImportProductMdb),strFN);			
			ShowMessage(strTmp);
		}
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	EXECUTE_TIME_END
	AfxGetApp()->EndWaitCursor();
}

void modPHScal::SetSPRPosValue()
{
}

long modPHScal::CSLength(CString /*ByVal*/ sBHFormat, CString /*ByVal*/ LengthSQL, CString & sBH)
{
	//Ŀ�ģ���������۸���������(ClassID)����.
	//����:sBHformat--��Ÿ�ʽ��LengthSQL--���ȼ�����ʽ��sBH--��õĹ������
	long ret=0;
	try
	{
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		//CSLength = 0;
		//����͸ֳ���(���ֶ�sngLenȷ��)
		CString SQL1;
		CString sTmp;
		_variant_t vTmp;
		SQL1 = _T("SELECT ") + LengthSQL + _T(" AS sngLen FROM tmpCSLen");
		rs->Open((_bstr_t)SQL1, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
		
		if(rs->BOF && rs->adoEOF)
		{
			sTmp.Format(GetResStr(IDS_NoRecordInXtableInXmdb),EDIBgbl::GetDBName(EDIBgbl::dbPRJ),_T("tmpCSLen"),SQL1);
			throw sTmp;
		}
		else
		{
			rs->MoveFirst();
			rs->get_Collect((_variant_t)_T("sngLen"),&vTmp);
			ret=vtoi(vTmp);
			CString sTmp1;
			sTmp1.Format(_T("%g"),vtof(vTmp));

			SQL1 = _T("UPDATE tmpCSLen SET L1=") + sTmp1;
			EDIBgbl::dbPRJ->Execute((_bstr_t)SQL1, NULL, adCmdText );
			//���CLgg��BH
			SQL1 = _T("SELECT (") + sBHFormat + _T(") AS sBH FROM tmpCSLen");
			rs->Close();
			rs->Open((_bstr_t)SQL1, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			
			if(rs->BOF && rs->adoEOF)
			{
				sTmp.Format(GetResStr(IDS_NoRecordAtTmpCSLenTableInWorkprjMdb),EDIBgbl::GetDBName(EDIBgbl::dbPRJ),SQL1);
				ShowMessage(sTmp);
			}
			else
			{
				rs->MoveFirst();
				//��������ʽ���ظ�sBH����
				rs->get_Collect((_variant_t)_T("sBH"),&vTmp);
				sBH =vtos(vTmp);
			}
		}
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	return ret;
}

	bool modPHScal::CheckFieldIfNull(_RecordsetPtr rs)
{
	return true;
}

/*Object*/void modPHScal::GetZAdataRs(CWnd* /*ByVal*/ frm)
{
}

long modPHScal::lngPreCal()
{
	try{
   //PreCalCrudeData(FrmTxsr.m_pViewTxsr->m_ActiveRs);
   
   SetBasePoint();
   if( IDdn != _T(""))
      Cavphs->PA = IDdn;
   else
      throw GetResStr(IDS_NullPA);
   
   if( IDgn != _T(""))
      Cavphs->SA = IDgn;
   else
      throw GetResStr(IDS_NullSA);
   
   
   Cavphs->iLIST = mnuiPASALimited;
   if( iSelSampleID <= 0 )
      iSelSampleID = Cavphs->GetPhsStructSampleID();
   else
      //iSelSampleID = Cavphs->GetPhsStructSampleID()
   ;
   CString sTmp1,sTmp2;
   if( iSelSampleID == -1)
   {
	   sTmp1.Format(_T("%d"),iSelSampleID);
	   sTmp2.Format(GetResStr(IDS_SelSampleIDisIllegal),sTmp1);
      throw sTmp2;
	}
   else
   
      return Cavphs->SavephsStructTorsTmpREF();
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	return 0;
}
}

CString modPHScal::GetSaveAsDwgFileName(long zdjh,_RecordsetPtr rs)
{
	//Ŀ�ģ���ô����ļ���
	CString sTmp;


	CString sDwgFileName;
	if( EDIBAcad::g_bCustomPicNo )
	{		
		CString sSQL;
		sSQL.Format("SELECT CustomDwgName FROM ZA WHERE [VolumeID]=%d AND [ZDJH]=%d",EDIBgbl::SelVlmID,zdjh);
		if( rs != NULL )
		{
			if( rs->adoEOF && rs->BOF ) 
			{
			}
			else
			{
				if( vtos(rs->GetCollect("CustomDwgName")).IsEmpty() )
				{
					rs->PutCollect("CustomDwgName",(long)modPHScal::zdjh);
					EDIBAcad::g_strCustomPicNoFileName = ltos(modPHScal::zdjh);
				}
				else
				{
					EDIBAcad::g_strCustomPicNoFileName = vtos(rs->GetCollect("CustomDwgName"));
				}

				sDwgFileName = EDIBAcad::g_strCustomPicNoFileName;
			}
		}

	}
	else
	{
		if( glDWGPreMethod == 0)
			sDwgFileName = EDIBAcad::GetDrawIDAndName(zdjh + glDrawNoStartNumber - 1, sTmp);
		else
			sDwgFileName.Format(_T("%s%d"),gsDWGPre,zdjh + glDrawNoStartNumber - 1);

		EDIBAcad::g_strCustomPicNoFileName=sDwgFileName;
	}


	if( strstr(sDwgFileName,".dwg") != NULL )
	{
		gsDwgFN = basDirectory::ProjectDWGDir + sDwgFileName;
	}
	else
	{
		gsDwgFN = basDirectory::ProjectDWGDir + sDwgFileName + _T(".dwg");
	}
	return gsDwgFN;
}

void modPHScal::DrawPhs(_RecordsetPtr rsza)
{
	try
	{
		static long iDwg=0;
		//long i;
		//dim rsza As Recordset
		//Set rsza = Crs.rsDS  //GetZAdataRs(frm)
		//Ԥ������
		AfxGetApp()->BeginWaitCursor();
		PreCalCrudeData(rsza);
		::SetRegValue(_T("Settings"),_T("PhsCurSANo"),modPHScal::giSAG100No);
		::SetRegValue(_T("Settings"),_T("PhsCurSATbn"),modPHScal::tbnSA);

		//SetWindowNoTop PhsData.hwnd
     
		if(!gbNotPrompt_ACAD_SHIFT)
		{
		  if(MessageBox(NULL,GetResStr(IDS_atFirstGoBackACADsaveDwgThenDraw), GetResStr(IDS_SaveDwg) ,MB_YESNO|MB_DEFBUTTON2|MB_SYSTEMMODAL) ==IDYES)
			  EDIBAcad::SetAcadTop();
		}

		SetPhsTK3();         //����3��ͼ��ߴ缰ͼ������Ϣ

		//֧���Զ�ȡ������ϵͳͼ��:����ܡ���ͼ����Ʊ�
		EDIBAcad::DrawTKphs((Ax == _T("A3")? 3 : 4), 0, 0, modPHScal::gbTBhq, (Ax == _T("A3") ? EDIBgbl::gbTbOS : 0), (long)gbNoFenQu, modPHScal::gbNoWaiKuang,1);


		Cavphs->GetMaterial();
		Cavphs->GetphsSEQ(rsza);   //������
		//�����������ɹ�!
		//SQLx = _T("SELECT * FROM [") & TBNSelPrjspec & Btype(TZB) & _T("] WHERE zdjh=") + ltos(zdjh) + _T(" AND (VolumeID)=\'") + EDIBgbl::SelVlmID + _T("\'")
		//SQL1 = _T("SELECT * FROM [") & TBNSelPrjspec & Btype(TZF) & _T("] WHERE zdjh=") + ltos(zdjh) + _T(" AND (VolumeID)=\'") + EDIBgbl::SelVlmID + _T("\'")  & _T(" ORDER BY recno")
		//SQLx = SQLx & _T(" UNION ALL ") & SQL1

		Cavphs->GetPhsSAELandPAdxdydz();  //���������ߺ͹ܲ���λ�ߴ�
		Cavphs->GetPhsBlkIDandCrd(rsza);
		_RecordsetPtr rsTmpZB;
		rsTmpZB.CreateInstance(__uuidof(Recordset));
		CString SQLx = CString(_T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(modPHScal::zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND IsSAPart<>-1 order by recno");
		rsTmpZB->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockOptimistic, adCmdText); 

		EDIBAcad::DrawPhsAssemble(rsTmpZB, iViewXZ) ; //����֧������װͼ


		gsDwgFN = GetSaveAsDwgFileName(modPHScal::zdjh,rsza);

		rsza->PutCollect(_T("DwgName"),_variant_t(gsDwgFN));
		CString sTmp;
		sTmp.Format(_T("Drawing%d"),(iDwg % 4) + 1);
		if( FileExists(gsDwgFN))
		{
		  if( gbDrawOVERWRITE)
		  {
			  SetRegValue("AutoCAD General", sTmp, gsDwgFN);
			 iDwg++;
		  }
		}
		else
		{
			SetRegValue("AutoCAD General", sTmp, gsDwgFN);
			iDwg++;
		}
  
		//���������ϸ��
		if( gbDraw_BOM )
		{
			EDIBDB::MakeTmp2ToBom();
			_RecordsetPtr tmprs;
			tmprs.CreateInstance(__uuidof(Recordset));
			SQLx = _T("SELECT * FROM TMP2");
			tmprs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			CCadPoint p0(pt2x,pt2y,0);
			if( EDIBAcad::g_bLJMX_BJBW )
			{
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TLJ_BJBW, atan((double)1.0) * 0, tmprs, 100, 1.0f,_T("bom"),_T("bom"),_T("%g"),modPHScal::iAlignLjmx);
			}
			else
			{
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TLJ, atan((double)1.0) * 0, tmprs, 100, 1.0f,_T("bom"),_T("bom"),_T("%g"),modPHScal::iAlignLjmx);
			}
		}

		//���ƺ���λ�Ʊ�(�����������ϸ������bom)
		if(EDIBAcad::g_bDLOADBJBW)
		{
			PhsDisplacementLoadINFOMake((_T("Load")),modPHScal::iAlignLoad,EDIBgbl::TzDLOADNEW);
		}
		else
		{
			PhsDisplacementLoadINFOMake((_T("Load")),modPHScal::iAlignLoad,EDIBgbl::TzDLOAD);
		}

	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	AfxGetApp()->EndWaitCursor();
}

void modPHScal::MakeTmpCSLen()
{
//����GetphsBHandSizes��������һ��֧����֮ǰ��������dbPRJ�н�����tmpCSLen
	CString strExecute;
   if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("tmpCSLen")))
   {
      //�����ȽϷ��㣬���Ұ汾����ʱ�����Ժá�
	   strExecute = _T("DROP TABLE tmpCSLen");
		EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);
   }
   strExecute = _T("CREATE TABLE tmpCSLen (ID char(20),CustomID char(50),SprNo SHORT,SerialNum SHORT,P2 char(20),P3 char(20),P4 char(20),P5 char(20),P6 char(20),P7 char(20),P8 char(20),P9 char(20),t0 REAL,BH CHAR (80),SelOpLoad REAL,OpLoad REAL,InstallLoad REAL,SumDist long,SumDistID short,HeatDist long,DistFX CHAR (50),LugDiameter long,sizeH REAL,sizeC REAL,Length long, GDW1 REAL,sumL REAL,L1 REAL,A REAL,Lspan REAL, H REAL,B REAL,B1 REAL,A1 REAL,B0 REAL,M REAL,C REAL, CHDist REAL, CHheight REAL,CHLegheight REAL,BWIDTH  REAL,size2 REAL,PL1 REAL,SpecialDesign char(50),UnitForce char(50),Dw REAL,P1 char(50),DW1 REAL,S REAL,K REAL,K2 REAL,temperature REAL)");
   EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

   strExecute = _T("INSERT INTO tmpCSLen (sizeH) VALUES (0)");
   EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText );
}



void modPHScal::MakeZDJ_ZD(long zdjh)
{
//Ŀ��:����֧����һ����ṹ��
//����:֧���ܺš����ȱʡ֧���ܺţ��������ǰ�������һ������Ŀ������ֻ�����ǰ֧����һ�������ݡ�
   try
	{
//	bool Found ;
      //ɾ����ǰ�������֧���ܺ�Ϊzdjh��һ��������
		CString strExecute;
		strExecute = _T("DELETE * FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + 
			(zdjh==0 ? _T("") : CString(_T(" AND zdjh=")) + ltos(zdjh));
     EDIBgbl::dbPRJDB->Execute((_bstr_t)strExecute, NULL, adCmdText);
	}
   catch (_com_error &e)
   {
	   CString strMsg;
	   strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
	   AfxMessageBox(strMsg);
	}
}

void modPHScal::PhsMLmake(_variant_t ZDJNo)
{
   //Ŀ�ģ���ӻ��޸�zdjh=zdjno֧������װͼͼֽ��Ŀ¼��Ϣ��
	try
	{
   
	   CString tbn;
	   int i ;
	   _RecordsetPtr rsX;
	   rsX.CreateInstance(__uuidof(Recordset));
	   tbn = EDIBgbl::Btype[EDIBgbl::TML];
	   CString SQL1x=_T("");
	   if(!EDIBgbl::tdfExists(EDIBgbl::dbPRJDB, tbn))
	   {
		   //copy ml table structure from f4511s.mdb in templatedir into ml of allprjdb.mdb in projectdbdir
		   SQL1x = _T("SELECT * INTO [") + tbn + _T("] FROM [") + tbn + _T("] IN \'") + basDirectory::TemplateDir + _T("F4511s.mdb\' WHERE DrawNo=\'\'"); 
		   EDIBgbl::dbPRJDB->Execute((_bstr_t) SQL1x, NULL, adCmdText);
	   }
   
	   CString sTmpDrawNo, sTmpDrawNa,sTmp;
	   sTmpDrawNo = EDIBAcad::GetDrawIDAndName((zdjh + glDrawNoStartNumber - 1), sTmpDrawNa);
		CString sZdjh;
	   sZdjh.Format(_T("%d"),zdjh);
	   if (sTmpDrawNo != _T("") && sTmpDrawNa != _T(""))
	   {
		  //��ʼ����Ŀ¼��Ϣ
		   SQL1x = _T("SELECT * FROM [") + tbn + _T("] where VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND NOT IsNull(DrawNo) AND NOT IsNull(DrawNa) AND NOT (DrawNo)=\'\' AND NOT (DrawNa)=\'\' AND zdjh=") + sZdjh + _T(" ORDER BY SEQ");
		   rsX->Open((_bstr_t)SQL1x, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			   adOpenDynamic, adLockOptimistic, adCmdText); 
		  if( rsX->BOF&& rsX->adoEOF)// Then
		  {
			 //MsgBox ResolveResString(iUE_NoRecordInTMLtableInAllPrjDbMdb, _T("|1"), dbPRJDB.Name, _T("|2"), tbn, _T("|3"), SQL1x), vbMsgBoxSetForeground + vbSystemModal
			 //��ʼ���Ŀ¼��Ϣ
			 rsX->AddNew();
			 rsX->put_Collect((_variant_t)_T("SEQ"),_variant_t((long)(zdjh + glDrawNoStartNumber - 1)));
			 rsX->put_Collect((_variant_t)_T("Pages"),_variant_t((long)1));
			 rsX->put_Collect((_variant_t)_T("VolumeID"),_variant_t(EDIBgbl::SelVlmID));
			 rsX->put_Collect((_variant_t)_T("DrawNo"),STR_VAR(sTmpDrawNo));
			 rsX->put_Collect((_variant_t)_T("DrawNa") ,STR_VAR( sTmpDrawNa));
			 rsX->put_Collect((_variant_t)_T("zdjh"),_variant_t((long)zdjh));
				
			 rsX->Update();
		  }
		  else
		  {
			 //Ŀ¼��Ϣ�Ѿ�����
			 if( gbNotPrompt_Catlog_OVERWRITE )
				i = IDYES;
			 else
			 {
				//�����ֹAutoCAD��ס��ʾ����
				EDIBAcad::SetAcadNoTop();
				sTmp.Format(GetResStr(IDS_ExistsDrawNameAndDrawID),sTmpDrawNa,sTmpDrawNo);
				ShowMessage(sTmp,MB_YESNO|MB_DEFBUTTON2|MB_SYSTEMMODAL|MB_TOPMOST);
				EDIBAcad::SetAcadTop();
			 }
			 if (i == IDYES)
			 {
				rsX->put_Collect((_variant_t)_T("SEQ"),_variant_t((long)(zdjh + glDrawNoStartNumber - 1)));
				rsX->put_Collect((_variant_t)_T("Pages"),_variant_t((long)1));
				rsX->put_Collect((_variant_t)_T("VolumeID"),_variant_t(EDIBgbl::SelVlmID));
				rsX->put_Collect((_variant_t)_T("DrawNo"),STR_VAR( sTmpDrawNo));
				rsX->put_Collect((_variant_t)_T("DrawNa"),STR_VAR(sTmpDrawNa));

				rsX->Update();
			 }
		  }
		  rsX->Close();
	   }
	   }
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

void modPHScal::PhsDisplacementLoadINFOMake(LPCTSTR lpszTextStyle,int iAlign,int iZDLOADtype)
{
	//����:���ɹܵ�λ�Ʊ�֧���ܺ��ر�(����Ժ��ʽ����λ�ƺ��غϲ���(����Ժ��ʽ)tmpDisplacementLoad
	try
	{
		_Recordset* rsza=FrmTxsr.m_pViewTxsr->m_ActiveRs;
		EDIBAcad::DeleteAllEntitiesInLayers(1,_T("DLOAD"));
		CCadPoint p0;
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rsText;
		rsText.CreateInstance(__uuidof(Recordset));
		CString tmpStr;
		CString SQLx;
		CMObject objEnt;
		//����λ�ƻ���ϵ��
		float UnitF=(Unit_Force == _T("kgf") ? EDIBgbl::kgf2N : 1);

		EDIBgbl::g_bOnlyUserDesc  = GetRegKey(_T("Settings"),_T("OnlyUserDesc"),FALSE);
		EDIBgbl::g_bOnlySysDesc	  = GetRegKey(_T("Settings"),_T("OnlySysDesc"),TRUE);
		EDIBgbl::g_bCustomDesc	  = GetRegKey(_T("Settings"),_T("CustomDesc"),FALSE);
		
		//���ѡ���ˡ��Զ���ע�͡���Ӧ�ö������Զ�������		
		EDIBgbl::g_bPhsManual=  GetRegKey(_T("Settings"),_T("Custom_PhsManual"), EDIBgbl::g_bPhsManual);
		EDIBgbl::g_bPhsPicture= GetRegKey(_T("Settings"),_T("Custom_PhsPicture"),EDIBgbl::g_bPhsPicture);
		EDIBgbl::g_bPhsSetNum=  GetRegKey(_T("Settings"),_T("Custom_PhsSetNum"), EDIBgbl::g_bPhsSetNum);
		EDIBgbl::g_bUserDesc=   GetRegKey(_T("Settings"),_T("Custom_UserDesc"),  EDIBgbl::g_bUserDesc);	

		CString strExecute;
		if(giPhsOutINFOTableType == iDisplacementLoadSep)
		{
			//����Ժ��ʽ
			//�ܵ�λ�Ʊ�tmpPipeDisplacement����ر�ֿ�
			if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("tmpPipeDisplacement")))
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE tmpPipeDisplacement"), NULL, adCmdText);
			EDIBgbl::dbPRJ->Execute((_bstr_t) _T("CREATE TABLE tmpPipeDisplacement (duty char(10),design REAL,cool REAL,heat REAL,Tj char(20))"), NULL, adCmdText);

			strExecute = _T("INSERT INTO tmpPipeDisplacement (duty,design,cool,heat,Tj) VALUES (\'x\',0,") +vtos(rsza->GetCollect(_T("xl1"))) + _T(",") + vtos(rsza->GetCollect(_T("xr1"))) + _T(",\'") + ftos(t0) + _T("\')");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);

			strExecute = _T("INSERT INTO tmpPipeDisplacement (duty,design,cool,heat) VALUES (\'y\',0,") + vtos(rsza->GetCollect(_T("yl1"))) + _T(",") + vtos(rsza->GetCollect(_T("yr1"))) + _T(")");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText );

			strExecute = _T("INSERT INTO tmpPipeDisplacement (duty,design,cool,heat) VALUES (\'z\',0,") + vtos(rsza->GetCollect(_T("zl1")) )+ _T(",") + vtos(rsza->GetCollect(_T("zr1"))) + _T(")");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText );
			//֧���ܺ��ر�tmpPhsLoad
			if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("tmpPhsLoad")))
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE tmpPhsLoad"), NULL, adCmdText);
			
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE tmpPhsLoad (duty char(10),design REAL,cool REAL,heat REAL,HydraulicPressure REAL,SafetyValveAction REAL,Tj char(20))"), NULL, adCmdText);

			strExecute = _T("INSERT INTO tmpPhsLoad (duty,design,cool,heat,Tj) VALUES (\'Px\',0,");
			strExecute += (rsza->GetCollect(_T("pxl1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("pxl1"))))));
			strExecute += _T(","); 
			strExecute += (rsza->GetCollect(_T("pxr1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("pxr1")))))); 
			strExecute += _T(",\'"); 
			strExecute += ftos(t0); 
			strExecute += _T("\')");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);
			//pfg20050422ԭ����
			/*	EDIBgbl::dbPRJ->Execute((_bstr_t) CString(_T("INSERT INTO tmpPhsLoad (duty,design,cool,heat) VALUES (\'Py\',")) + 
				(rsza->GetCollect(_T("pjg1")).vt==VT_NULL ? CString(_T("0")) : ftos(fabs(vtof(rsza->GetCollect(_T("pjg1"))) * UnitF))) 
				+ CString(_T(","))
				+ (rsza->GetCollect(_T("paz1")).vt==VT_NULL? CString(_T("0")) : ftos(fabs(vtof(rsza->GetCollect(_T("paz1"))) * UnitF))) 
				+ CString(_T(",")) 
				+ (rsza->GetCollect(_T("pgz1")).vt==VT_NULL ? _T("0") : ftos(vtof(rsza->GetCollect(_T("pgz1"))) * UnitF)) + CString(_T(")")));*/
			//pfg20050422ԭ����

			//pfg20050422
			strExecute =  CString(_T("INSERT INTO tmpPhsLoad (duty,design,cool,heat) VALUES (\'Py\',"));
			strExecute += (rsza->GetCollect(_T("pjg1")).vt==VT_NULL ? _T("0") : ftos((vtof(rsza->GetCollect(_T("pjg1"))) * UnitF)));
			strExecute += _T(","); 
			strExecute += (rsza->GetCollect(_T("paz1")).vt==VT_NULL ? _T("0") : ftos((vtof(rsza->GetCollect(_T("paz1"))) * UnitF))); 
			strExecute += _T(",\'"); 
			strExecute += (rsza->GetCollect(_T("pgz1")).vt==VT_NULL ? _T("0") : ftos((vtof(rsza->GetCollect(_T("pgz1"))) * UnitF))); 
			strExecute += _T(",\'"); 
			strExecute += ftos(t0); 
			strExecute += _T("\')");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText);
			//pfg20050422

			strExecute = _T("INSERT INTO tmpPhsLoad (duty,design,cool,heat) VALUES (\'Pz\',0,");
			strExecute += (rsza->GetCollect(_T("pzl1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("pzl1"))) * UnitF)));
			strExecute += _T(","); 
			strExecute += (rsza->GetCollect(_T("pzr1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("pzr1"))) * UnitF))); 
			strExecute += _T(")");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText );

			strExecute = _T("INSERT INTO tmpPhsLoad (duty,design,cool,heat) VALUES (\'Mx\',0,");
			strExecute += (rsza->GetCollect(_T("mxl1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("mxl1"))))));
			strExecute += _T(","); 
			strExecute += (rsza->GetCollect(_T("mxr1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("mxr1")))))); 
			strExecute += _T(")");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText );

			strExecute = _T("INSERT INTO tmpPhsLoad (duty,design,cool,heat) VALUES (\'My\',0,");
			strExecute += (rsza->GetCollect(_T("myl1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("myl1"))))));
			strExecute += _T(","); 
			strExecute += (rsza->GetCollect(_T("myr1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("myr1")))))); 
			strExecute += _T(")");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText );

			strExecute = _T("INSERT INTO tmpPhsLoad (duty,design,cool,heat) VALUES (\'Mz\',0,");
			strExecute += (rsza->GetCollect(_T("mzl1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("mzl1"))))));
			strExecute += _T(","); 
			strExecute += (rsza->GetCollect(_T("mzr1")).vt==VT_NULL ? _T("0") : ftos(fabs(vtof(rsza->GetCollect(_T("mzr1")))))); 
			strExecute += _T(")");
			EDIBgbl::dbPRJ->Execute((_bstr_t)strExecute, NULL, adCmdText );
			
			//��ʼ���Ʊ��
			p0.SetPoint(pt3x, pt3y);
			rsText->Open((_bstr_t)_T("SELECT * FROM tmpPipeDisplacement"), _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			EDIBAcad::DrawTableACAD(p0, EDIBgbl::TzDIST, atan((double)1.0) * 0, rsText, 100, 1.0f , (_T("DLOAD")),lpszTextStyle,(_T("%.f")),iAlign);
			rsText->Close();


			//����֧���ܺ��ر�Ļ���
			SQLx = _T("SELECT * FROM tableINFO WHERE ID=") + ltos(EDIBgbl::TzDIST);
			rs1->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbTable,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			CString tmpStr;
			if(rs1->BOF && rs1->adoEOF)
			{
				tmpStr.Format(GetResStr(IDS_NoRecordInXtableInXmdb),EDIBgbl::GetDBName(EDIBgbl::dbSORT),_T("tableINFO"),SQLx);
				throw tmpStr;
			}
			rs1->MoveFirst();
			p0.SetX(pt3x);


			//������7mm���Ծ���������Bom��ײ,���ɸı�ȫ�ֱ���pt3y��ֵ
			//pt3y = pt3y + 7
			_variant_t vTmp;
			rs1->get_Collect((_variant_t)_T("CADh0"),&vTmp);
			if(vTmp.vt==VT_NULL)
				p0.SetY(pt3y - 17);
			else
				p0.SetY(pt3y - fabs(vtof(vTmp)));
			rs1->get_Collect((_variant_t)_T("CADRowHeight"),&vTmp);
			if(vTmp.vt==VT_NULL)
				p0.SetY(p0[1] - 7 * 3);
			else
				p0.SetY(p0[1] - vtof(vTmp) * 3);
			p0.SetY(p0[1] - 14 + 7);
			p0.SetZ(0.0);
			rsText->Open((_bstr_t)_T("SELECT * FROM tmpPhsLoad"), _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			EDIBAcad::DrawTableACAD(p0, EDIBgbl::TzLOAD, atan((double)1.0) * 0, rsText, 100, 1.0f , _T("DLOAD"),lpszTextStyle,(_T("%.f")),iAlign);
			
			
			//дע��
			int iNo=1;
			p0.SetX(pt3x - (Ax = _T("A3"), 305, 290));
			p0.SetY(pt3y - 23);	
			
			if( EDIBgbl::g_bCustomDesc || EDIBgbl::g_bOnlySysDesc )
			{
				if( EDIBgbl::g_bCustomDesc )
				{
					if( EDIBgbl::g_bPhsPicture )
					{
						//1.֧���ܲ���ͼ��x1,֧����һ�����x2
						tmpStr.Format(IDS_MsgBox_60674,iNo,gsPhsArrangementDwgNo,gsPhsScheduleDwgNo);
						iNo++;
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}
				}
				else 
				{
					//1.֧���ܲ���ͼ��x1,֧����һ�����x2
					tmpStr.Format(IDS_MsgBox_60674,iNo,gsPhsArrangementDwgNo,gsPhsScheduleDwgNo);
					iNo++;
					objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
					objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                    EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
					p0.SetY(p0[1] - 7);
				}

				
				if( EDIBgbl::g_bCustomDesc )
				{
					if( EDIBgbl::g_bPhsSetNum )
					{
						//2.���ڹ��̰�ͼ����x��
						tmpStr.Format(IDS_MsgBox_60676,iNo,vtoi(rsza->GetCollect(_T("num"))));
						iNo++;
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}
				}
				else 
				{
					//2.���ڹ��̰�ͼ����x��
					tmpStr.Format(IDS_MsgBox_60676,iNo,vtoi(rsza->GetCollect(_T("num"))));
					iNo++;
					objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
					objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                    EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
					p0.SetY(p0[1] - 7);
				}

				CString strDg=vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dg1")));
				strDg.MakeUpper();
				float fPzx,fPzz;
				fPzx=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("pzx")));
				fPzz=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("pzz")));
				if(fabs(fPzx-0.0f) > 0.0001 || fabs(fPzz-0.0f) > 0.0001 )
				{
					//sys �������������ۺ�ʱֻҪ�о����
					if(strDg==_T("D"))
					{
						//%d. �ܲ��ߴ��Ѱ���ƫװֵ
						tmpStr.Format(IDS_PZ_D,iNo++);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}
					else if(strDg==_T("G"))
					{
						//%d. �����ߴ��Ѱ���ƫװֵ
						tmpStr.Format(IDS_PZ_G,iNo++);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					} 
				}

				//sys �������������ۺ�ʱֻҪ�о����
				//4.��֧��������ֱ�����������ȫ���ṹ�������
				if( vtob(rsza->GetCollect(_T("ifLongVertPipe"))) )
				{
					tmpStr.Format(IDS_SINGLEROD_CARRYALLPJG,iNo++);
					objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
					objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                    EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
					p0.SetY(p0[1] - 7);
				}			
				
				//���ʹ��ϵͳ��������û��Զ������
				CCadPoint oldP0=p0;
				if( EDIBgbl::g_bCustomDesc )
				{
					if( EDIBgbl::g_bPhsManual )
					{
						//5.����Ҫ�󼰺���߶ȼ������ֲ�:
						tmpStr.Format(IDS_WeldingDemandAsFollow,iNo++);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						
						p0.SetX(p0[0] + 43);
						//�ܲ�:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(IDS_WeldingDemandSeeManual,GetResStr(IDS_FrmSelSpecification_Option1_PA),gsPhsDescPASel);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
						//���Ӽ�:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PART),gsPhsDescPARTSel);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
						//����:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_SA),gsPhsDescSASel);				
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}
				}
				else 
				{
					//5.����Ҫ�󼰺���߶ȼ������ֲ�:
					tmpStr.Format(IDS_WeldingDemandAsFollow,iNo++);
					objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
					objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                    EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
					
					p0.SetX(p0[0] + 43);
					//�ܲ�:����Ҫ�󼰺���߶ȼ�xx
					tmpStr.Format(IDS_WeldingDemandSeeManual,GetResStr(IDS_FrmSelSpecification_Option1_PA),gsPhsDescPASel);
					objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
					objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                    EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
					p0.SetY(p0[1] - 7);
					//���Ӽ�:����Ҫ�󼰺���߶ȼ�xx
					tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PART),gsPhsDescPARTSel);
					objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
					objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                    EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
					p0.SetY(p0[1] - 7);
					//����:����Ҫ�󼰺���߶ȼ�xx
					tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_SA),gsPhsDescSASel);
					
					objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
					objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                    EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
					p0.SetY(p0[1] - 7);
				}

				//�û��Զ����ע����Ϣ
				if( EDIBgbl::g_bCustomDesc )
				{
					if( EDIBgbl::g_bUserDesc )
					{
						oldP0.SetY(p0[1]);
						DrawUserDesc( iNo, oldP0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
					}

				}
				else
				{
					if( EDIBgbl::g_bOnlyUserDesc )
					{
						oldP0.SetY(p0[1]);
						DrawUserDesc( iNo, oldP0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
					}
				}
			}
			else if ( EDIBgbl::g_bOnlyUserDesc )//�û��Զ����ע����Ϣ
			{
				DrawUserDesc(iNo, p0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
			}
		}
		else
		{
			//����Ժ��ʽ
			//λ�ƺ��غϲ���tmpDisplacementLoad
			if(!bPAIsHanger() && /*!(giUPxyz==1 && yr==0 || giUPxyz==2 && yr==0 || giUPxyz==3 && yr==0 ) &&*/ ZdjType.GetLength()<4 )
			{
				//����Ƿǵ���֧�ܣ�����ר�ŵ���ϸ����λ�Ʊ��ʽ��
				//��������/����/��λ��/��λ�ƹ�12�����ݣ�2�����ϣ���̬/��̬���ֹ���
				if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("tmpPipeDisplacement")))
					EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE tmpPipeDisplacement"), NULL, adCmdText);
				EDIBgbl::dbPRJ->Execute((_bstr_t) _T("CREATE TABLE tmpPipeDisplacement (Jsdbh1 LONG,duty char(20),px REAL,py REAL,pz REAL,mx REAL,my REAL,mz REAL,dx REAL,dy REAL,dz REAL,rx REAL,ry REAL,rz REAL,haz REAL,ZDbz char(20))"), NULL, adCmdText);
				//�ֹ����㣬����ڵ��Ϊ�գ���ʱ��jsdbh1�ֶθ�ֵ���ޡ������򣬸��ֶ���ֵ��INSERT INTO�������
				/*//pfg20050416 ���� ԭ����
				SQLx=GetResStr(IDS_INSERT_INTO_tmpPipeDisplacement2)
					+ (vtoi(rsza->GetCollect(_T("jsdbh1")))==0?"��":vtos(rsza->GetCollect(_T("jsdbh1"))) ) + _T(",") 
					+ ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pxr1"))))) + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pyr1")))))  + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pzr1"))))) + _T(",") 
					+ ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("mxr1"))))) + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("myr1")))))  + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("mzr1"))))) + _T(",") 
					+ vtos(rsza->GetCollect(_T("xr1"))) + _T(",") + vtos(rsza->GetCollect(_T("yr1")))  + _T(",") + vtos(rsza->GetCollect(_T("zr1")))  + _T(",")
					+ vtos(rsza->GetCollect(_T("rxr1"))) + _T(",") + vtos(rsza->GetCollect(_T("ryr1")))  + _T(",") + vtos(rsza->GetCollect(_T("rzr1"))) + _T(",") 
					+ vtos(rsza->GetCollect(_T("haz"))) + _T(",\'tj=") + ftos(t0) + _T("%%dC\')");
					*///pfg20050416


				//pfg20050416 �˹���������Ϊ�β��ܻ�����Ϊû������֧���ܽڵ��,0��ʾû��֧���ܺ�
				SQLx=GetResStr(IDS_INSERT_INTO_tmpPipeDisplacement2)
					+ (vtoi(rsza->GetCollect(_T("jsdbh1")))==0?"-10000":vtos(rsza->GetCollect(_T("jsdbh1"))) ) + _T(",") 
					+ ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pxr1"))))) + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pyr1")))))  + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pzr1"))))) + _T(",") 
					+ ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("mxr1"))))) + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("myr1")))))  + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("mzr1"))))) + _T(",") 
					+ vtos(rsza->GetCollect(_T("xr1"))) + _T(",") + vtos(rsza->GetCollect(_T("yr1")))  + _T(",") + vtos(rsza->GetCollect(_T("zr1")))  + _T(",")
					+ vtos(rsza->GetCollect(_T("rxr1"))) + _T(",") + vtos(rsza->GetCollect(_T("ryr1")))  + _T(",") + vtos(rsza->GetCollect(_T("rzr1"))) + _T(",") 
					+ vtos(rsza->GetCollect(_T("haz"))) + _T(",\'tj=") + ftos(t0) + _T("%%dC\')");
				//pfg20050416				

				EDIBgbl::dbPRJ->Execute((_bstr_t) SQLx, NULL, adCmdText);
				SQLx=GetResStr(IDS_INSERT_INTO_tmpPipeDisplacement)  
					
					+ ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pxl1"))))) + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pyl1")))))  + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("pzl1"))))) + _T(",") 
					+ ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("mxl1"))))) + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("myl1")))))  + _T(",") + ftos(fabs(UnitF*vtof(rsza->GetCollect(_T("mzl1"))))) + _T(",") 
					+ vtos(rsza->GetCollect(_T("xl1"))) + _T(",") + vtos(rsza->GetCollect(_T("yl1")))  + _T(",") + vtos(rsza->GetCollect(_T("zl1")))  + _T(",")
					+ vtos(rsza->GetCollect(_T("rxl1"))) + _T(",") + vtos(rsza->GetCollect(_T("ryl1")))  + _T(",") + vtos(rsza->GetCollect(_T("rzl1"))) + _T(",") 
					+ vtos(rsza->GetCollect(_T("haz"))) + _T(",\'t0=20%%dC\')");
				EDIBgbl::dbPRJ->Execute((_bstr_t) SQLx, NULL, adCmdText);
				
				//CString SQLx;
				//��ʼ���Ʊ��
				p0.SetPoint(pt3x, pt3y);
				SQLx = _T("SELECT * FROM tmpPipeDisplacement");
				rsText->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
					adOpenStatic, adLockOptimistic, adCmdText); 
				EDIBAcad::DrawTableACAD(p0, EDIBgbl::TzDLOADS, atan((double)1.0) * 0, rsText, 100, 1.0f , _T("DLOAD"),lpszTextStyle,(_T("%.f")),iAlign);
				rsText->Close();
				
				
				
				//дע��
				int iNo=1;
				p0.SetX(pt3x - (Ax == _T("A3") ? 193 : 178));
				p0.SetY(p0[1] - 8 * 7 + 3);

				//�����Ҫ���ϵͳ�������Ϣ
				if( EDIBgbl::g_bCustomDesc || EDIBgbl::g_bOnlySysDesc )
				{
					if( EDIBgbl::g_bCustomDesc )
					{
						if( EDIBgbl::g_bPhsPicture )
						{
							//1.֧���ܲ���ͼ��x1,֧����һ�����x2
							tmpStr.Format(IDS_MsgBox_60674,iNo,gsPhsArrangementDwgNo,gsPhsScheduleDwgNo);
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
					}
					else 
					{
						//1.֧���ܲ���ͼ��x1,֧����һ�����x2
						tmpStr.Format(IDS_MsgBox_60674,iNo,gsPhsArrangementDwgNo,gsPhsScheduleDwgNo);
						iNo++;
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}

					if( EDIBgbl::g_bCustomDesc )
					{
						if( EDIBgbl::g_bPhsSetNum )
						{
							//2.���ڹ��̰�ͼ����x��
							tmpStr.Format(IDS_MsgBox_60676,iNo,vtoi(rsza->GetCollect(_T("num"))));
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
					}
					else 
					{
						//2.���ڹ��̰�ͼ����x��
						tmpStr.Format(IDS_MsgBox_60676,iNo,vtoi(rsza->GetCollect(_T("num"))));
						iNo++;
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}


					//3.����װͼ����|1��֧����ģ��(AutoPHS����)
					CString strDg=vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dg1")));
					strDg.MakeUpper();
					float fPzx,fPzz;
					fPzx=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("pzx")));
					fPzz=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("pzz")));
					if(fabs(fPzx-0.0f) > 0.0001 || fabs(fPzz-0.0f) > 0.0001 )
					{
						//sys
						if(strDg==_T("D"))
						{
							//%d.�ܲ��ߴ��Ѱ���ƫװֵ��
							tmpStr.Format(IDS_PZ_D,iNo);
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
						else if(strDg==_T("G"))
						{
							//%d.�����ߴ��Ѱ���ƫװֵ��
							tmpStr.Format(IDS_PZ_G,iNo);
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
					}

					//sys
					//4.��֧��������ֱ�����������ȫ���ṹ�������
					if( vtob(rsza->GetCollect(_T("ifLongVertPipe"))) )
					{
						tmpStr.Format(IDS_SINGLEROD_CARRYALLPJG,iNo++);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}			

					CCadPoint oldP0=p0;
					if( EDIBgbl::g_bCustomDesc )
					{
						if( EDIBgbl::g_bPhsManual )
						{
							//5.����Ҫ�󼰺���߶ȼ������ֲ�:
							tmpStr.Format(IDS_WeldingDemandAsFollow,iNo);
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));

							p0.SetX(p0[0] + 43);
							//�ܲ�:����Ҫ�󼰺���߶ȼ�xx
							tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PA),gsPhsDescPASel);
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
							//���Ӽ�:����Ҫ�󼰺���߶ȼ�xx
							tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PART),gsPhsDescPARTSel);
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
							//����:����Ҫ�󼰺���߶ȼ�xx
							tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_SA),gsPhsDescSASel);
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
					}
					else
					{
						//5.����Ҫ�󼰺���߶ȼ������ֲ�:
						tmpStr.Format(IDS_WeldingDemandAsFollow,iNo);
						iNo++;
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));

						p0.SetX(p0[0] + 43);
						//�ܲ�:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PA),gsPhsDescPASel);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
						//���Ӽ�:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PART),gsPhsDescPARTSel);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
						//����:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_SA),gsPhsDescSASel);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}

					//�û��Զ����ע����Ϣ
					if( EDIBgbl::g_bCustomDesc )
					{
						if( EDIBgbl::g_bUserDesc )
						{
							oldP0.SetY(p0[1]);
							DrawUserDesc( iNo, oldP0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
						}
					}
					else
					{
						if( EDIBgbl::g_bOnlyUserDesc )
						{
							oldP0.SetY(p0[1]);
							DrawUserDesc( iNo, oldP0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
						}
					}
					}
					else if ( EDIBgbl::g_bOnlyUserDesc )//�û��Զ����ע����Ϣ
					{
						DrawUserDesc(iNo, p0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
					}
			}
			else
			{
				//����Ժλ�ƺ��ر��ʽ������paz/pgz/pjg/����λ�Ƶ����ݣ�һ�С�
				if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("tmpDisplacementLoad")) )
					EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE tmpDisplacementLoad"), NULL, adCmdText);
				MakeZDJ_ZD(zdjh);
				PhsYLBMake(zdjh);
				SQLx = _T("SELECT * INTO tmpDisplacementLoad IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' FROM [") + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(modPHScal::zdjh);
				EDIBgbl::dbPRJDB->Execute((_bstr_t)SQLx, NULL, adCmdText);
				//���±�ע�ֶ�Ϊ�¶�ֵ

				if(EDIBAcad::g_bHzabs)
					SQLx = _T("UPDATE tmpDisplacementLoad SET ZDbz=\'tj=") + ftos(t0) + _T("%%dC\' , pgz1 = abs(pgz1), pjg1 = abs(pjg1)  WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh);
				else
					SQLx = _T("UPDATE tmpDisplacementLoad SET ZDbz=\'tj=") + ftos(t0) + _T("%%dC\' , pgz1 = pgz1, pjg1 = pjg1  WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh);

				EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);

				//��ʼ���Ʊ��
				p0.SetPoint(pt3x,pt3y,0);
				SQLx = _T("SELECT * FROM tmpDisplacementLoad WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh);
				rsText->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
					adOpenStatic, adLockOptimistic, adCmdText); 
				//pfg20050519����ѹ��ֵ
				if(modPHScal::giPhsOutINFOTableType==3)
				{
					iZDLOADtype=35;
				}
				//pfg20050519
				
				EDIBAcad::DrawTableACAD(p0, iZDLOADtype, atan((double)1.0) * 0, rsText, 100, 1.0f ,  _T("DLOAD"),lpszTextStyle,(_T("%.f")),iAlign);
				
				
				
				
				//дע��
				int iNo=1;
				p0.SetX(pt3x - (Ax == _T("A3") ? 193 : 178));
				p0.SetY(p0[1] - 7 * 7 + 3);


				//�����Ҫ���ϵͳ�������Ϣ
				if( EDIBgbl::g_bCustomDesc || EDIBgbl::g_bOnlySysDesc )
				{
					if( EDIBgbl::g_bCustomDesc )
					{
						if( EDIBgbl::g_bPhsPicture )
						{
							//1.֧���ܲ���ͼ��x1,֧����һ�����x2
							tmpStr.Format(IDS_MsgBox_60674,iNo,gsPhsArrangementDwgNo,gsPhsScheduleDwgNo);
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
					}
					else 
					{
						//1.֧���ܲ���ͼ��x1,֧����һ�����x2
						tmpStr.Format(IDS_MsgBox_60674,iNo,gsPhsArrangementDwgNo,gsPhsScheduleDwgNo);
						iNo++;
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}

					if( EDIBgbl::g_bCustomDesc )
					{
						if( EDIBgbl::g_bPhsSetNum )
						{
							//2.���ڹ��̰�ͼ����x��
							tmpStr.Format(IDS_MsgBox_60676,iNo,vtoi(rsza->GetCollect(_T("num"))));
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
					}
					else
					{
						//2.���ڹ��̰�ͼ����x��
						tmpStr.Format(IDS_MsgBox_60676,iNo,vtoi(rsza->GetCollect(_T("num"))));
						iNo++;
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}
					
					CString strDg=vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dg1")));
					strDg.MakeUpper();
					float fPzx,fPzz;
					fPzx=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("pzx")));
					fPzz=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("pzz")));
					if(fabs(fPzx-0.0f) > 0.0001 || fabs(fPzz-0.0f) > 0.0001 )
					{
						//sys
						if(strDg==_T("D"))
						{
							//%d.�ܲ��ߴ��Ѱ���ƫװֵ��
							tmpStr.Format(IDS_PZ_D,iNo);
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
						else if(strDg==_T("G"))
						{
							//%d.�����ߴ��Ѱ���ƫװֵ��
							tmpStr.Format(IDS_PZ_G,iNo);
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
					}
					//sys
					//4.��֧��������ֱ�����������ȫ���ṹ�������
					if( vtob(rsza->GetCollect(_T("ifLongVertPipe"))) )
					{
						tmpStr.Format(IDS_SINGLEROD_CARRYALLPJG,iNo++);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}			


					CCadPoint oldP0=p0;
					if( EDIBgbl::g_bCustomDesc )
					{
						if( EDIBgbl::g_bPhsManual )
						{
							//5.����Ҫ�󼰺���߶ȼ������ֲ�:
							tmpStr.Format(IDS_WeldingDemandAsFollow,iNo);
							iNo++;
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetX(p0[0] + 43);
							//�ܲ�:����Ҫ�󼰺���߶ȼ�xx
							tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PA),gsPhsDescPASel);
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
							//���Ӽ�:����Ҫ�󼰺���߶ȼ�xx
							tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PART),gsPhsDescPARTSel);
							
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
							//����:����Ҫ�󼰺���߶ȼ�xx
							tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_SA),gsPhsDescSASel);
							objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
							objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                            EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
							p0.SetY(p0[1] - 7);
						}
					}
					else
					{
						//5.����Ҫ�󼰺���߶ȼ������ֲ�:
						tmpStr.Format(IDS_WeldingDemandAsFollow,iNo);
						iNo++;
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetX(p0[0] + 43);
						//�ܲ�:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PA),gsPhsDescPASel);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
						//���Ӽ�:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_PART),gsPhsDescPARTSel);
						
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
						//����:����Ҫ�󼰺���߶ȼ�xx
						tmpStr.Format(GetResStr(IDS_WeldingDemandSeeManual),GetResStr(IDS_FrmSelSpecification_Option1_SA),gsPhsDescSASel);
						objEnt=EDIBAcad::MoSpace.Invoke(_T("AddText"),3,&_variant_t(tmpStr),(LPVARIANT)p0,&_variant_t((double)4.0)).pdispVal;
						objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM"))));
                        EDIBAcad::ApplyTextStyleProperty(objEnt, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")),_T("ע��"));
						p0.SetY(p0[1] - 7);
					}
					
					//�û��Զ����ע����Ϣ
					if( EDIBgbl::g_bCustomDesc )
					{
						if( EDIBgbl::g_bUserDesc )
						{
							oldP0.SetY(p0[1]);
							DrawUserDesc( iNo, oldP0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
						}
					}
					else
					{
						if( EDIBgbl::g_bOnlyUserDesc )
						{
							oldP0.SetY(p0[1]);
							DrawUserDesc( iNo, oldP0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
						}
					}
				}
				else if ( EDIBgbl::g_bOnlyUserDesc )//�û��Զ����ע����Ϣ
				{
					DrawUserDesc(iNo, p0, EDIBAcad::MoSpace, EDIBAcad::GetTextStyleName(_T("ע��"), _T("BOM")) );
				}
			}
		}
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

void modPHScal::PhsYLBMake(long zdjh)
{
	//Ŀ��:����֧���ܺ�=zdjh��֧����һ����
	//��ǰ������makezdj_zd���̣����֧���ܺ�=zdjh��֧����һ����
	//����:֧����һ�����浽TZD��
	_RecordsetPtr rs;
	rs.CreateInstance(__uuidof(Recordset));
	_RecordsetPtr rs1;
	rs1.CreateInstance(__uuidof(Recordset));
	_variant_t Bmk ;
	_Recordset* rsza=FrmTxsr.m_pViewTxsr->m_ActiveRs;
	CString strDrawName;
	CString strHgz,strHaz;
	CString SQLx,sTmp;
	float UnitF=(Unit_Force == _T("kgf") ? EDIBgbl::kgf2N : 1);
	bool b1st=false;
	SQLx=CString( _T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZD] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) +(zdjh==0 ? _T("") : CString(_T(" AND zdjh=")) + ltos(zdjh));
	
	try
	{
		rs1->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockOptimistic, adCmdText); 
		if(zdjh!=0)
		{
			//��������Ժ��ʽ��֧������ϸ��
			PreCalCrudeData(rsza);
			rs1->AddNew();
			_variant_t vTmp;
			CString tmpStr;
			if(gbDrawNoPreMethod)
				tmpStr=EDIBAcad::GetDrawIDAndName((zdjh + glDrawNoStartNumber - 1), strDrawName);
			else
				tmpStr = EDIBAcad::GetDrawIDAndName((zdjh + glDrawNoStartNumber - 1), strDrawName);
			if( sngSEL < sngPEL )
				rs1->put_Collect((_variant_t)_T("HSid"),STR_VAR(GetResStr(IDS_BRANCH)));
			else				
				rs1->put_Collect((_variant_t)_T("HSid"),STR_VAR(GetResStr(IDS_SUSPEND)));
			
			rs1->put_Collect((_variant_t)_T("ZDID"),STR_VAR(tmpStr));
			rs1->put_Collect((_variant_t)_T("VolumeID"),_variant_t(EDIBgbl::SelVlmID));
			rs1->put_Collect((_variant_t)_T("PLName"),STR_VAR(EDIBgbl::SelVlmName));
			rs1->put_Collect((_variant_t)_T("PLN"),STR_VAR(EDIBgbl::SelJcdm));//ʯ����ҵ�ã����߱�ţ���ʱ�þ��Ŵ���
			rs1->put_Collect((_variant_t)_T("ConDwg"),STR_VAR(EDIBgbl::VolumeNo));
			rs1->put_Collect((_variant_t)_T("TagNo"),STR_VAR(EDIBgbl::VolumeNo));
			rs1->put_Collect((_variant_t)_T("PhsNo"),STR_VAR(EDIBgbl::PhsNo));
			rs1->put_Collect((_variant_t)_T("zdjh"),rsza->GetCollect(_T("ZDJH")));
			
			rs1->put_Collect((_variant_t)_T("ZDmc"),STR_VAR(gsPhsName));         //д�롰֧�������ơ�
			CString strGG;//д�롰�ܾ���
			if( EDIBgbl::bSymbol )
				strGG = _T("%%C") + vtos(rsza->GetCollect("dj1"));
			else
				strGG = vtos(rsza->GetCollect("dj1"));
			rs1->put_Collect((_variant_t)_T("ZDgg"),STR_VAR(strGG));
			rs1->put_Collect((_variant_t)_T("MCAndGG"),STR_VAR(gsPhsName+"   "+strGG));//д�롰���Ƽ����
			rs1->put_Collect((_variant_t)_T("BornSA"),STR_VAR(vtos(rsza->GetCollect("BornSA"))));  //д�롰�����ص㡱
			rs1->put_Collect((_variant_t)_T("BornSAEngName"),STR_VAR(vtos(rsza->GetCollect("BornSAEngName"))));  //д�롰�����ص㡱
			rs1->put_Collect((_variant_t)_T("BornSAEngChina"),STR_VAR(vtos(rsza->GetCollect("BornSAEngChina"))));  //д�롰�����ص㡱
			rs1->put_Collect((_variant_t)_T("ZDnum"),rsza->GetCollect(_T("Num")));
			if(EDIBAcad::g_bHzabs)
				rs1->put_Collect((_variant_t)_T("paz1"),_variant_t(fabs(vtof(rsza->GetCollect(_T("paz1"))) * (float)UnitF)));
			else
				rs1->put_Collect((_variant_t)_T("paz1"),_variant_t(vtof(rsza->GetCollect(_T("paz1"))) * (float)UnitF));
			rs1->put_Collect((_variant_t)_T("pgz1"),_variant_t(vtof(rsza->GetCollect(_T("pgz1"))) * (float)UnitF));
			rs1->put_Collect((_variant_t)_T("pjg1"),_variant_t(vtof(rsza->GetCollect(_T("pjg1"))) * (float)UnitF));
			rs1->put_Collect((_variant_t)_T("xr1"),rsza->GetCollect(_T("xr1")));
			vTmp=rsza->GetCollect(_T("yr1"));
			float fyr1=(int(vtof(vTmp)*10))/10.0;
			rs1->put_Collect((_variant_t)_T("yr1"),_variant_t(fyr1));
			rs1->put_Collect((_variant_t)_T("zr1"),rsza->GetCollect(_T("zr1")));
			rs1->put_Collect((_variant_t)_T("xl1"),rsza->GetCollect(_T("xl1")));
			rs1->put_Collect((_variant_t)_T("yl1"),rsza->GetCollect(_T("yl1")));
			rs1->put_Collect((_variant_t)_T("zl1"),rsza->GetCollect(_T("zl1")));
			rs1->put_Collect((_variant_t)_T("jsdbh1"),rsza->GetCollect(_T("jsdbh1")));
			rs1->put_Collect((_variant_t)_T("temp"),rsza->GetCollect(_T("T01")));
			rs1->put_Collect((_variant_t)_T("InstallPEL"),rsza->GetCollect(_T("dh1")));
			rs1->put_Collect((_variant_t)_T("SEL"),rsza->GetCollect(_T("gh1")));
			rs1->Update();
			rs1->MoveLast();

			UpdatePipeDimHeight();
			//���ڴ���ܲ�
			SQLx=CString( _T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND Index=") + ltos(iPA);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockOptimistic, adCmdText); 
			
			if( !rs->BOF && !rs->adoEOF ){
			
				
				while( !rs->adoEOF){
					if( b1st ){
						//(vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))
						rs1->put_Collect((_variant_t)_T("PAbh"), STR_VAR(vtos(GetFields(rs1,_T("PAbh")))+ _T(";") + (vtoi(GetFields(rs,_T("CLnum"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("PAweight"), _variant_t(vtof(GetFields(rs1,_T("PAweight"))) +vtof(GetFields(rs,_T("CLzz")))));
					}else{
						rs1->put_Collect((_variant_t)_T("PAbh"),STR_VAR((vtoi(GetFields(rs,_T("CLnum"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("PAweight"),_variant_t(vtof(GetFields(rs,_T("CLzz")))));
						//�ǵ�һ�������ñ�־��
						b1st = true;
					}
					rs1->Update();
					rs->MoveNext();					
				}
				
			}
			//��λ��־
			b1st = false;
			if(rs->State == adStateOpen)
				rs->Close();
			//���ڴ���������ɻ򵯻�
			SQLx=CString( _T("SELECT ClassID,GDW1,CLmc,CLgg,CLid,CLcl,CLdw,CLdz,sum(CLnum) as CLnum1,sum(CLzz) as CLzz1 FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND (ClassID=") + ltos(iCSPR) + _T(" OR ClassID=") + ltos(iSPR) + _T(") GROUP BY CLmc,CLgg,CLid,CLcl,CLdw,CLdz,CLnum,CLzz,GDW1,ClassID");
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockOptimistic, adCmdText); 
			if( !rs->BOF && !rs->adoEOF )
			{
				int iSEQofSPR=0;
				while( !rs->adoEOF)
				{
					if( b1st )
					{						
						rs1->put_Collect((_variant_t)_T("SPRbh"), STR_VAR(vtos(GetFields(rs1,_T("SPRbh"))) + _T(";") + (vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("SPRweight"), _variant_t(vtof(GetFields(rs1,_T("SPRweight"))) +vtof(GetFields(rs,_T("CLzz1")))));
						//���ڵ��ɣ�����GetPhsBHandSizes�Ѿ�����԰�װѹ��ֵд��TZB��GDW1�ֶΣ��Ա��ں���PhsYLBmake���һ����ʹ��
						if( vtoi(GetFields(rs,_T("ClassID"))) == iSPR ) 
						{								
							sTmp.Format(_T("%g"),int((sSprInfo[iSEQofSPR].haz + sSprInfo[iSEQofSPR].Hpcmprs)*10)/10.0);
							rs1->put_Collect((_variant_t)_T("hazTotal"),STR_VAR(vtos(GetFields(rs1,_T("hazTotal"))) + _T(";") + sTmp));
							sTmp.Format(_T("%g"),int((sSprInfo[iSEQofSPR].haz - sSprInfo[iSEQofSPR].HeatDisp + sSprInfo[iSEQofSPR].Hpcmprs)*10)/10.0);
							rs1->put_Collect((_variant_t)_T("hgzTotal"),STR_VAR(vtos(GetFields(rs1,_T("hgzTotal"))) + _T(";") + sTmp));
							strHaz.Format(_T("%g"),int(sSprInfo[iSEQofSPR].haz*10)/(double)10.0);
							strHgz.Format(_T("%g"),int((sSprInfo[iSEQofSPR].haz - sSprInfo[iSEQofSPR].HeatDisp)*10)/10.0);
							rs1->put_Collect((_variant_t)_T("Haz"),STR_VAR(vtos(GetFields(rs1,_T("Haz"))) + _T(";") + strHaz));
							rs1->put_Collect((_variant_t)_T("Hgz"),STR_VAR(vtos(GetFields(rs1,_T("Hgz"))) + _T(";") + strHgz));
							rs1->put_Collect((_variant_t)_T("SPRpcmprs"), STR_VAR(vtos(GetFields(rs1,_T("SPRpcmprs"))) + _T(";") + ltos((long)modPHScal::sSprInfo[iSEQofSPR].Hpcmprs)));
							rs1->put_Collect((_variant_t)_T("Diameter"), STR_VAR(vtos(GetFields(rs1,_T("Diameter"))) + _T(";") + ltos((long)modPHScal::sSprInfo[iSEQofSPR].FiJ)));
							rs1->put_Collect((_variant_t)_T("SpringNo"), STR_VAR(vtos(GetFields(rs1,_T("SpringNo"))) + _T(";") + ltos((long)modPHScal::sSprInfo[iSEQofSPR].DH)));
							rs1->put_Collect((_variant_t)_T("BH"), STR_VAR(vtos(GetFields(rs1,_T("BH"))) + _T(";") + modPHScal::sSprInfo[iSEQofSPR].BH));
						}
					}
					else
					{						
						rs1->put_Collect((_variant_t)_T("SPRbh"),STR_VAR((vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("SPRweight"),_variant_t(vtof(GetFields(rs,_T("CLzz1")))));
						//���ڵ��ɣ�����GetPhsBHandSizes�Ѿ�����԰�װѹ��ֵд��TZB��GDW1�ֶΣ��Ա��ں���PhsYLBmake���һ����ʹ��
						if( vtoi(GetFields(rs,_T("ClassID"))) == iSPR ) 
						{
							sTmp.Format(_T("%g"),int((sSprInfo[iSEQofSPR].haz + sSprInfo[iSEQofSPR].Hpcmprs)*10)/10.0);
							rs1->put_Collect((_variant_t)_T("hazTotal"),STR_VAR(sTmp));	
							sTmp.Format(_T("%g"),int((sSprInfo[iSEQofSPR].haz - sSprInfo[iSEQofSPR].HeatDisp + sSprInfo[iSEQofSPR].Hpcmprs)*10)/10.0);
							rs1->put_Collect((_variant_t)_T("hgzTotal"),STR_VAR(sTmp));	
							strHaz.Format(_T("%g"),int(sSprInfo[iSEQofSPR].haz*10)/(double)10.0);
							strHgz.Format(_T("%g"),int((sSprInfo[iSEQofSPR].haz - sSprInfo[iSEQofSPR].HeatDisp)*10)/10.0);
							rs1->put_Collect((_variant_t)_T("Haz"),STR_VAR(strHaz));							
							rs1->put_Collect((_variant_t)_T("Hgz"),STR_VAR(strHgz));							
							rs1->put_Collect((_variant_t)_T("SPRpcmprs"),STR_VAR(ltos((long)modPHScal::sSprInfo[iSEQofSPR].Hpcmprs)));
							rs1->put_Collect((_variant_t)_T("Diameter"),STR_VAR(ltos((long)modPHScal::sSprInfo[iSEQofSPR].FiJ)));
							rs1->put_Collect((_variant_t)_T("SpringNo"),STR_VAR(ltos((long)modPHScal::sSprInfo[iSEQofSPR].DH)));
							rs1->put_Collect((_variant_t)_T("BH"),STR_VAR(modPHScal::sSprInfo[iSEQofSPR].BH));
						}
						//�ǵ�һ�������ñ�־��
						b1st = true;
					}
					rs1->Update();
					rs->MoveNext();
					iSEQofSPR++;
				}				
			}
			//��λ��־
			b1st = false;
			rs->Close();      
      
			//���ڴ�������
			SQLx=CString( _T("SELECT CLmc,CLgg,CLid,CLcl,CLdw,CLdz,sum(CLnum) as CLnum1,sum(CLzz) as CLzz1 FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND ClassID=") + ltos(iROD) + _T(" GROUP BY CLmc,CLgg,CLid,CLcl,CLdw,CLdz,CLnum,CLzz");
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockOptimistic, adCmdText); 
			if( !rs->BOF && !rs->adoEOF ){
				
				while( !rs->adoEOF){
					//��ѭ����ֹһ��ʱ��rs1.EDIT()����������ÿ�������ֶ�֮ǰʹ�á�������ʾ��Edit������
					if( b1st ){
						rs1->put_Collect((_variant_t)_T("RODDiaLen"), STR_VAR(vtos(GetFields(rs1,_T("RODDiaLen"))) + _T(";") + (vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("RODweight"),_variant_t(vtof(GetFields(rs1,_T("RODweight"))) +vtof(GetFields(rs,_T("CLzz1")))));
					}else{
						rs1->put_Collect((_variant_t)_T("RODDiaLen"),STR_VAR((vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("RODweight"),_variant_t(vtof(GetFields(rs,_T("CLzz1")))));
						//�ǵ�һ�������ñ�־��
						b1st = true;
					}
					rs1->Update();
					rs->MoveNext();
				}
				
			}
			//��λ��־
			b1st = false;
			rs->Close();
      
      
			//���ڴ���ǵ������Ӽ������������ˣ�
			SQLx=CString( _T("SELECT CLmc,CLgg,CLid,CLcl,CLdw,CLdz,sum(CLnum) as CLnum1,sum(CLzz) as CLzz1 FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND NOT IsNull(Index) AND Index=") + ltos(iConnectPART) + _T(" AND ClassID<>") + ltos(iROD) + _T(" AND ClassID<>") + ltos(iCSPR) + _T(" AND ClassID<>") + ltos(iSPR) + _T(" AND IsSAPart<>-1 GROUP BY CLmc,CLgg,CLid,CLcl,CLdw,CLdz,CLnum,CLzz");
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockOptimistic, adCmdText); 
			if( !rs->BOF && !rs->adoEOF ){
				while( !rs->adoEOF){
					//��ѭ����ֹһ��ʱ��rs1.EDIT()����������ÿ�������ֶ�֮ǰʹ�á�������ʾ��Edit������
					if( b1st ){
						rs1->put_Collect((_variant_t)_T("CntPart"), STR_VAR(vtos(GetFields(rs1,_T("CntPart"))) + _T(";") + (vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("CntPartweight"),_variant_t(vtof(GetFields(rs1,_T("CntPartweight")))+vtof(GetFields(rs,_T("CLzz1")))));
					}else{
						rs1->put_Collect((_variant_t)_T("CntPart"),STR_VAR((vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						//�ǵ�һ�������ñ�־��
						rs1->put_Collect((_variant_t)_T("CntPartweight"),_variant_t(vtof(GetFields(rs,_T("CLzz1")))));
						b1st = true;
					}
					rs1->Update();
					rs->MoveNext();
				}				
			}
			//��λ��־
			b1st = false;
			rs->Close();
          
          
			//���ڴ���Ǹ����е���ĸ,��˨
			//��������ϸһ�����и�����û��ͳ��"F3"(�����Ѿ�ͳ��),�������������.
			//ԭ����SQLx=CString( _T("SELECT CLmc,CLgg,CLid,CLcl,CLdw,CLdz,sum(CLnum) as CLnum1,sum(CLzz) as CLzz1 FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND (ClassID=") + ltos(iBolts) + _T(" OR ClassID=") + ltos(iNuts) + _T(") AND Not IsNull(SEQ) GROUP BY CLmc,CLgg,CLid,CLcl,CLdw,CLdz,CLnum,CLzz");
			SQLx=CString( _T("SELECT CLmc,CLgg,CLid,CLcl,CLdw,CLdz,sum(CLnum) as CLnum1,sum(CLzz) as CLzz1 FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND (ClassID=") + ltos(iBolts) + _T(" OR ClassID=") + ltos(iNuts) +_T("OR (ID='F3' AND IsSAPart <> -1 )")+ _T(") AND Not IsNull(SEQ) GROUP BY CLmc,CLgg,CLid,CLcl,CLdw,CLdz,CLnum,CLzz");
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockOptimistic, adCmdText); 
			if( !rs->BOF && !rs->adoEOF ){
				
				while( !rs->adoEOF){
					if( b1st ){
						rs1->put_Collect((_variant_t)_T("Attachment"), STR_VAR(vtos(GetFields(rs1,_T("Attachment"))) + _T(";") + (vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("Attachmentweight"),_variant_t(vtof(GetFields(rs1,_T("Attachmentweight")))+vtof(GetFields(rs,_T("CLzz1")))));
						rs1->put_Collect((_variant_t)_T("Attachment1"), STR_VAR(vtos(GetFields(rs1,_T("Attachment1"))) + _T(";") + (vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("Attachmentweight1"),_variant_t(vtof(GetFields(rs1,_T("Attachmentweight1")))+vtof(GetFields(rs,_T("CLzz1")))));
					}else{
						rs1->put_Collect((_variant_t)_T("Attachment"),STR_VAR((vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						//�ǵ�һ�������ñ�־��
						rs1->put_Collect((_variant_t)_T("Attachmentweight"),_variant_t(vtof(GetFields(rs,_T("CLzz1")))));
						rs1->put_Collect((_variant_t)_T("Attachment1"),STR_VAR((vtoi(GetFields(rs,_T("CLnum1"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum1"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						//�ǵ�һ�������ñ�־��
						rs1->put_Collect((_variant_t)_T("Attachmentweight1"),_variant_t(vtof(GetFields(rs,_T("CLzz1")))));
						b1st = true;
					}
					rs1->Update();
					rs->MoveNext();
				}
				
			}
			rs->Close();
			//��λ��־
			//��Ҫ������ĸ����������ӵ���˨��ĸ�ַ������ʹر����濪�ء�
			//b1st = false
      
			//SELECT CLmc,CLgg,CLid,CLcl,CLdw,CLdz,sum(CLnum) as CLnum1,sum(CLzz) as CLzz1 FROM TMP2 GROUP BY CLmc,CLgg,CLid,CLcl,CLdw,CLdz,CLnum,CLzz
			//���ڴ����������(�����͸�)
			SQLx=CString( _T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND IsSAPart=-1 AND (ClassID=") + ltos(iAttached) + _T(" OR ClassID=") + ltos(iBolts )+ _T(" OR ClassID=") + ltos(iNuts) + _T(")");
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockOptimistic, adCmdText); 
			if(!( rs->BOF && rs->adoEOF || (vtoi(rsza->GetCollect(_T("bNotSumSA")))!=0) )){
			
				//ֻѡ�����������Ҫͳ�Ƶ�֧����
				
				while( !rs->adoEOF){
					if( b1st ){
						rs1->put_Collect((_variant_t)_T("Attachment"), STR_VAR(vtos(GetFields(rs1,_T("Attachment"))) + _T(";") + (vtoi(GetFields(rs,_T("CLnum"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("Attachmentweight"),_variant_t(vtof(GetFields(rs1,_T("Attachmentweight")))+vtof(GetFields(rs,_T("CLzz")))));
					}else{
						rs1->put_Collect((_variant_t)_T("Attachment"),STR_VAR((vtoi(GetFields(rs,_T("CLnum"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg")))));
						rs1->put_Collect((_variant_t)_T("Attachmentweight"),_variant_t(vtof(GetFields(rs,_T("CLzz")))));
						//�ǵ�һ�������ñ�־��
						b1st = true;
					}
					rs1->Update();
					rs->MoveNext();
				}
				
			}
			//��λ��־
			b1st = false;
			rs->Close();
      
			//���ڴ�������͸�
			SQLx=CString( _T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND IsSAPart=-1 AND ClassID<>") + ltos(iBolts) + _T(" AND ClassID<>") + ltos(iNuts) + _T(" AND ClassID<>") + ltos(iAttached);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockOptimistic, adCmdText); 
			if(!( rs->BOF && rs->adoEOF || (vtoi(rsza->GetCollect(_T("bNotSumSA")))!=0) ))
			{							
				//ֻѡ�����������Ҫͳ�Ƶ�֧����
				while( !rs->adoEOF)
				{
					if( b1st )
					{
						if( vtoi(GetFields(rs,_T("CLnum"))) ==1  )
							rs1->put_Collect((_variant_t)_T("SAssAndLen"),STR_VAR(vtos(GetFields(rs1,_T("SAssAndLen"))) + _T(";") +vtos(GetFields(rs,_T("CLgg"))) + _T(",L=") +vtos(GetFields(rs,_T("L1")))));
						else
							rs1->put_Collect((_variant_t)_T("SAssAndLen"), STR_VAR(vtos(GetFields(rs1,_T("SAssAndLen"))) + _T(";") +vtos(GetFields(rs,_T("CLnum"))) + _T("x(") +vtos(GetFields(rs,_T("CLgg"))) + _T(",L=") +vtos(GetFields(rs,_T("L1"))) + _T(")")));
						
					}
					else
					{
						if( vtoi(GetFields(rs,_T("CLnum"))) ==1  )
							rs1->put_Collect((_variant_t)_T("SAssAndLen"),STR_VAR(vtos(GetFields(rs,_T("CLgg"))) + _T(",L=") +vtos(GetFields(rs,_T("L1")))));
						else
							rs1->put_Collect((_variant_t)_T("SAssAndLen"),STR_VAR(vtos(GetFields(rs,_T("CLnum"))) + _T("x(") +vtos(GetFields(rs,_T("CLgg"))) + _T(",L=") +vtos(GetFields(rs,_T("L1"))) + _T(")")));
						
						//�ǵ�һ�������ñ�־��
						b1st = true;
					}
					rs1->Update();
					rs->MoveNext();
				}
				//�������������,˫������֧����һ������û�г�2,ԭ���Ǳ�ʾ���������Ĳ���.
				if(modPHScal::glNumSA==2)
				{
					rs1->put_Collect((_variant_t)_T("SAssAndLen"),STR_VAR(_T("2x( ")+vtos(GetFields(rs1,_T("SAssAndLen")))+_T(" )")));
					rs1->Update();
				}
			}
			//��λ��־
			b1st = false;
			rs->Close();
      
			//���ڴ����������
			SQLx=CString( _T("SELECT * FROM [")) + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND zdjh=") + ltos(zdjh) + _T(" AND Index=") + ltos(iSA) + _T(" AND ClassID<>") + ltos(iGCement);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenDynamic, adLockOptimistic, adCmdText); 
			if( !rs->BOF && !rs->adoEOF ){
				
				while( !rs->adoEOF){
					rs1->put_Collect((_variant_t)_T("XBL1"),_variant_t(vtof(GetFields(rs,_T("XBL1")))));//���۳��ȣ��������� Add by luorijin 2008.10.30
					if( b1st ){
						rs1->put_Collect((_variant_t)_T("SAbh"), STR_VAR((vtos(GetFields(rs1,_T("SAbh"))) + _T(";") + (vtoi(GetFields(rs,_T("CLnum"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg"))))));
						rs1->put_Collect((_variant_t)_T("SAweight"), _variant_t(vtof(GetFields(rs1,_T("SAweight"))) +vtof(GetFields(rs,_T("CLzz")))));
					}else{
						rs1->put_Collect((_variant_t)_T("SAbh"),STR_VAR(((vtoi(GetFields(rs,_T("CLnum"))) ==1 ? _T("") : vtos(GetFields(rs,_T("CLnum"))) + _T("x")) + vtos(GetFields(rs,_T("CLgg"))))));
						rs1->put_Collect((_variant_t)_T("SAweight"),_variant_t(vtof(GetFields(rs,_T("CLzz")))));
						//�ǵ�һ�������ñ�־��
						b1st = true;
					}
					rs1->Update();
					rs->MoveNext();
				}
				
			}
			//��λ��־
			b1st = false;
			rs->Close();
			rs1->Close();
			
		}
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}


void modPHScal::CreateTmpIDCustomIDTable()
{
   //Ŀ��:��dbPRJ�н���ID-CustomID���ձ�PictureClipData;
	//ͬʱ��dbPRJ�н�����ѡ��淶���Ӧ��ģ���PhsStructureName/REF,
	//������Name/REF��ṹ��sort.mdb�е�һ����ֻ�ǹ��˵��˲����ʵ�ǰ��׼(�ܲ�/����)��ģ�塣
	//������Ŀ������һ���Եĳ�ʱ�䣬������frmPhsSamp�Ի���ģ����˵��ٶȡ�
	
	//���̿��ܷ�ʱ�ϳ�
   //Screen.MousePointer = vbHourglass
	FILE* fp = fopen("CreateTmpIDCustomIDTable.txt", "w");

	AfxGetApp()->BeginWaitCursor();
	try
	{
		//��������PictureClipData��,���ɶ�������ṩ��ID-CustomID�����
		//Dim rs As Recordset, tdf As TableDef, FD As Field, bLineSPR As Boolean
		//Dim i As Integer, j As Integer
		//�����б�PictureClipData���Ƿ����ø�Cavphs->rsID
		//�����б���Ƿ��Ѿ��򿪡�����򿪣����ȹرն���Cavphs->rsID
		if (Cavphs->rsID->State == adStateOpen)
			Cavphs->rsID->Close();
   
		_variant_t tmpvar;
		if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("PictureClipData")))
		{
			fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE * FROM PictureClipData"), NULL, adCmdText);
			EDIBgbl::SQLx = CString(_T("INSERT INTO PictureClipData IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
				_T("\' SELECT * FROM [") + tbnPAID + _T("]");
			dbZDJcrude->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
		}
		else
		{
			fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
			EDIBgbl::SQLx = CString(_T("SELECT * INTO PictureClipData IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
				_T("\' FROM [") + tbnPAID + _T("]");
			dbZDJcrude->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
		}
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}

	try
	{
		fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
	   EDIBgbl::SQLx = CString(_T("INSERT INTO PictureClipData IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
		   _T("\' SELECT * FROM [") + tbnPARTID + _T("]");
	   dbZDJcrude->Execute((_bstr_t) EDIBgbl::SQLx, NULL, adCmdText);

	   fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
		EDIBgbl::SQLx = CString(_T("INSERT INTO PictureClipData IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
		   _T("\' SELECT * FROM [") + tbnSAID + _T("]");
	   dbZDJcrude->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);

	   fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
	   EDIBgbl::SQLx = CString(_T("INSERT INTO PictureClipData IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
		   _T("\' SELECT * FROM [") + tbnAttachmentID + _T("]");
	   dbZDJcrude->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText );
	   //MsgBox dbPRJ.RecordsAffected
			fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
	   EDIBgbl::SQLx = CString(_T("INSERT INTO PictureClipData IN \'") )+ EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
		   _T("\' SELECT * FROM [") + tbnBoltsNutsID + _T("]");
	   dbZDJcrude->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText );
		
	   //���Լ���ID���ֶο��ܶ�����InstallHeightOfUP,InstallHeightOfDown
	   fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
	   EDIBgbl::SQLx = CString(_T("INSERT INTO PictureClipData IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
		   _T("\' SELECT Index,SEQ,ID,CustomID,ClassID,BmpName,Description,ChineseDescription,ParalleledNum,BHFormat FROM [") + tbnHDid + _T("]");
	   dbZDJcrude->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText );
	   fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
	   EDIBgbl::SQLx = CString(_T("INSERT INTO PictureClipData IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ)+ 
		   _T("\' SELECT Index,SEQ,ID,CustomID,ClassID,BmpName,Description,ChineseDescription,ParalleledNum,BHFormat FROM [") + tbnSPRINGid + _T("]");
	   dbZDJcrude->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText );
	   fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
	   EDIBgbl::SQLx = CString(_T("INSERT INTO PictureClipData IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
		   _T("\' SELECT * FROM [") + tbnSectionSteelID + _T("]");
	   dbZDJcrude->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText );   
	
		//��һ��ID-CustomID���ձ���գ��Ƿ���Լӿ������ٶȡ�
	   fprintf(fp, "%s:%d\n", __FILE__, __LINE__);
		EDIBgbl::SQLx = _T("SELECT * FROM PictureClipData");
		if(Cavphs->rsID->State)
			Cavphs->rsID->Close();
		Cavphs->rsID->Open((_bstr_t)EDIBgbl::SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			adOpenDynamic, adLockOptimistic, adCmdText); 
		Cavphs->brsIDStatus=TRUE;
		//�˺�һ��Ӧ�ü��ID���޸�ģ������
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	fclose(fp);
   AfxGetApp()->EndWaitCursor();
}

void modPHScal::CreateTmpConnectTable()
{
   //Ŀ�ģ���������connect��,����connectPASA/connectCSPR/connectSPR������������ɡ�����ÿ���û������ò��ụ����š�
   //Dim rs As Recordset, FD As Field, bLineSPR As Boolean
   //Dim i As Integer, j As Integer
   //On Error Resume Next
   //����ʱ����ܽϳ�
	AfxGetApp()->BeginWaitCursor();
	_variant_t tmpvar;
	if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("connect")))
	{
		//���Cphs.rsConnect����򿪣��ر������Ա�ɾ����
		Cavphs->CloseRecordsets();
		try 
		{
			EDIBgbl::dbPRJ->Execute((_bstr_t) _T("DROP TABLE [connect]"), NULL, adCmdText);
		} catch (...)
		{
		}
	}
    //20071018(start) "dbSORT" ��Ϊ "dbPHScode"
	EDIBgbl::SQLx = _T("SELECT * INTO [connect] FROM connectPASA IN \'");
		EDIBgbl::SQLx += EDIBgbl::GetDBName(EDIBgbl::dbPHScode);
		EDIBgbl::SQLx += _T("\'");
	EDIBgbl::dbPRJ->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
   EDIBgbl::SQLx = _T("INSERT INTO [connect] SELECT * FROM connectCSPR IN \'");
		EDIBgbl::SQLx += EDIBgbl::GetDBName(EDIBgbl::dbPHScode);
		EDIBgbl::SQLx += _T("\'");
	EDIBgbl::dbPRJ->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
   EDIBgbl::SQLx = _T("INSERT INTO [connect] SELECT * FROM connectSPR IN \'");
		EDIBgbl::SQLx += EDIBgbl::GetDBName(EDIBgbl::dbPHScode);
		EDIBgbl::SQLx += _T("\'");

   EDIBgbl::dbPRJ->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
	Cavphs->InitListRs(); 
   AfxGetApp()->EndWaitCursor();//20071018(end) "dbSORT" ��Ϊ "dbPHScode"
}

void modPHScal::CreateTmpSPRPropertyTable(int /*Optional*/ SprMaxSerialNum)
{
//Ŀ��:�������ɺ���λ�����Ա�
//����:���Ե���,���������Ա�����һ��������Ե��ɽṹ��ͬ�ı�,
//     �Ա�����Ժͷ����Ե���ʹ��ͬһ�����ҵ����ͺ��㷨
//����:SprMaxSerialNum-�ܵ��������������Ƭ����Բ���ε���һ��=4�����ɿ���=5
	try
	{
			FieldPtr sFD;
			CString sTD1;

			_RecordsetPtr rs;
			rs.CreateInstance(__uuidof(Recordset));
			_RecordsetPtr rs1;
			rs1.CreateInstance(__uuidof(Recordset));

			bool bLineSPR=false;//Ĭ��Ϊ����(�����Ե���)
			bool bFinish =false;//д�����ĳ�����ŵ��غ�λ����������
			LONG i=0 , j =0;
			CString SQLx=_T("");
			_variant_t v;
   
		   //���̿��ܽϳ�
		   AfxGetApp()->BeginWaitCursor();
   
		   SQLx = _T("SELECT * FROM [") + tbnSPRINGproperty + _T("]");
		   rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
			   adOpenDynamic, adLockOptimistic, adCmdText); 

		   rs1->Open((_bstr_t)_T("select * from tmpSPRProperty"), _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			   adOpenDynamic, adLockOptimistic, adCmdText); 

		   //�����ֶ�����ȷ���Ƿ������Ե������Ա�
		   //���Ե����иն��ֶ�G
		   
		   int fc=rs->Fields->Count; 
		   for (i=0;i<fc;i++)
		   {
			   rs->Fields->get_Item((_variant_t)i, &sFD);
			   CString strName;
			   strName = (LPTSTR)sFD->Name;
			   if(strName ==_T("G") || strName ==_T("g"))
			   {
				   bLineSPR=true;
				   break;
			   }
		   }


		   //��ʱ�������Ա�����dbprj��(����),�ӿ�����ٶ�
			if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("tmpSPRProperty")))		   
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE * FROM tmpSPRProperty"), NULL, adCmdText);
			else
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE tmpSPRProperty (SerialNum SHORT,seq SHORT,Dist SINGLE,Pgz SINGLE,Pgzmax SINGLE,PreCmprs SINGLE,Hopmax SINGLE)"), NULL, adCmdText);

			long n=0,m=0;//��¼����
   
		   if( bLineSPR) 
			{
				while (!rs->adoEOF)
				{
					//��ȡ��ֵ����¼����
					rs->get_Collect((_variant_t)_T("HopMax"),&v);
					long Hopmax=vtof(v);
					n=n+(Hopmax+1)*SprMaxSerialNum;
					rs->MoveNext();
				}
		   }

		   if( bLineSPR) 
			{
			   frmStatus.m_Label1 = GetResStr(IDS_CreatingSPRpropertyTable);
			   frmStatus.m_Label2 = _T("");
			   frmStatus.SetWindowText(GetResStr(IDS_InitializeAutoPHS));
			   frmStatus.UpdateData(false);
			   frmStatus.ShowWindow(SW_SHOW);
			   rs->MoveFirst();
				while (!rs->adoEOF)
				{
					//��ȡ��ֵ���ӿ��ٶ�
					rs->get_Collect((_variant_t)_T("HopMax"),&v);
					long Hopmax=vtof(v);
					rs->get_Collect((_variant_t)_T("PreCmprs"),&v);
					float PreCmprs=vtof(v);
					rs->get_Collect((_variant_t)_T("G"),&v);
					float G=vtof(v);
					rs->get_Collect((_variant_t)_T("SEQ"),&v);
					CString SEQ=vtos(v);
					long SEQ2 = vtoi(v);
					 //����С�Ĺ��ſ�ʼ
					 //����С�Ĵ�����j=1��ʼ

//					EXECUTE_TIME_BEGIN
					
					 for ( j = 1;j<= SprMaxSerialNum;j++)
					 {
						//����С��λ��i=0��ʼ
						 
						for (i = 0;i<= Hopmax;i++)
						{
							rs1->AddNew();
							rs1->put_Collect((_variant_t)_T("SerialNum"), (_variant_t)j);
							rs1->put_Collect((_variant_t)_T("seq"), (_variant_t)(SEQ2));
							rs1->put_Collect((_variant_t)_T("Dist"), (_variant_t)(i * j));
							rs1->put_Collect((_variant_t)_T("Pgz"), (_variant_t)((i +PreCmprs) * G));
							rs1->put_Collect((_variant_t)_T("Pgzmax"), (_variant_t)((Hopmax + PreCmprs) *G));
							rs1->put_Collect((_variant_t)_T("Hopmax"), (_variant_t)(Hopmax * j));
							rs1->put_Collect((_variant_t)_T("PreCmprs"), (_variant_t)(PreCmprs * j));
							rs1->Update();
/*
							SQLx=_T("INSERT INTO tmpSPRProperty (SerialNum,seq,Dist,Pgz,Pgzmax,Hopmax,PreCmprs) VALUES (") + ltos(j) + _T(",") + SEQ +  _T(",") + ltos(i * j) + _T(",") + ftos((i +PreCmprs) * G) +  _T(",") + ftos((Hopmax + PreCmprs) *G) + _T(",") + ftos(Hopmax * j )+ _T(",") + ftos(PreCmprs * j) + _T(")");
							EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx);
*/
							m++;
							frmStatus.UpdateStatus((float)m /(float) n,true);
							frmStatus.UpdateWindow();
						}
					 }

//					EXECUTE_TIME_END

					 rs->MoveNext();
				}
				frmStatus.ShowWindow(SW_HIDE);
			}
			else
			{
			  //�����Ե������Ա�����ɣ������ڳ��Ҳ�Ʒ�����ĸ�ʽ���˴���Խ���ʯ���豸���ĵ���������ʽ����
				SQLx = _T("INSERT INTO tmpSPRProperty IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' SELECT SerialNum,seq,Dist,Pgz,Pgzmax,Hopmax,PreCmprs FROM [") + tbnSPRINGproperty + _T("]");
				dbZDJcrude->Execute((_bstr_t)SQLx, NULL, adCmdText);
		   }
		   
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	AfxGetApp()->EndWaitCursor();
}

void modPHScal::CreateTmpSPRPropertyTableUnfinish(int /*Optional*/ SprMaxSerialNum)
{
}

void modPHScal::SetSpringPhsInfo(_RecordsetPtr rsTmpSelSpring, _RecordsetPtr rsTZA, bool  bLineSPR,CString& mZdjType)
{
 //Ŀ�ģ�����ԭʼ���ݱ����йص���֧���ܵļ���������
	try
	{
	int iSumSerialNum,signOfPaz0;
	float mvHaz;
	float sngInstallLoad,sngOpLoad;
	CString sTmp;
   if(rsTmpSelSpring==NULL || rsTZA==NULL)
	{
		sTmp=GetResStr(IDS_NullRsOrRss);
		throw sTmp;
	}
	_variant_t v1,v2,v3;
	rsTmpSelSpring->get_Collect((_variant_t)_T("SerialNum"), &v1);
	rsTmpSelSpring->get_Collect((_variant_t)_T("Installload"),&v2);
	rsTmpSelSpring->get_Collect((_variant_t)_T("Opload"),&v3);
      iSumSerialNum = vtoi(v1);
      sngInstallLoad = vtof(v2);
      sngOpLoad = vtof(v3);
      //����֧�����͸�ʽ
	  sTmp.Format(_T("%d"),iSumSerialNum);
	  mZdjType=sTmp;
	  sTmp.Format(_T("%d"),gintParallelNum);
	  mZdjType+=sTmp;
	  rsTmpSelSpring->get_Collect((_variant_t)_T("SEQ"),&v1);
	  sTmp.Format(_T("%02d"),vtoi(v1));
	  mZdjType+=sTmp;
      //���������糧��ˮ�ܵ���Ƽ����涨(������,DL/T5054-1996)��p66
      //��7.4.5.1����̬����ܵ�
      //(1)�����߶�Hop=�������ɸ߶�H0-���ɹ���λ��Dop
      //(2)��װ�߶�Her=�����߶�Hop-��λ��yr
      //(3)���ɾ���ѹ��ֵHabs=���ɰ�װѹ��ֵHaz+����Ԥѹ��ֵHprecmprs=���ɹ���λ��Dop
      //�����ۣ�
      //���ɰ�װѹ��ֵHaz=���ɹ���λ��Dop-����Ԥѹ��ֵHprecmprs
      //��sub AutoSelSpringNo
	  rsTmpSelSpring->get_Collect((_variant_t)_T("OpDist"),&v1);
      mvHaz =vtof(v1) + yr;
      
      
      rsTZA->PutCollect(_T("type"),_variant_t( mZdjType));
      sTmp.Format(_T("%d"),mvHaz);
	  rsTZA->PutCollect(_T("haz"),_variant_t(mvHaz));
	  _variant_t tmpvar;
	  int Sgn=1;
     //֧���ܵİ�װ����=�������ɵİ�װ����*���ɲ�����
	 if(gbHotStatus2Zero)
	 {
		if(pgz<0)
		  Sgn=-1;
		else if(pgz==0)
		  Sgn=0;
		if(vtoi(rsTZA->GetCollect(_T("paz0")))==0)
			rsTZA->PutCollect(_T("paz0"),_variant_t((float)(Sgn * sngInstallLoad * gintParallelNum * (Unit_Force == _T("kgf") ? 1 : EDIBgbl::kgf2N))));
		rsTZA->PutCollect(_T("paz1"),_variant_t((float)(Sgn * sngInstallLoad * gintParallelNum * (Unit_Force == _T("kgf") ? 1 : EDIBgbl::kgf2N))));
		signOfPaz0 = Sgn;
	 }
	 else
	 {
		if(paz<0)
		  Sgn=-1;
		else if(paz==0)
		  Sgn=0;
		if(vtoi(rsTZA->GetCollect(_T("paz0")))==0)
			rsTZA->PutCollect(_T("paz0"),_variant_t((float)(Sgn * sngInstallLoad * gintParallelNum * (Unit_Force == _T("kgf") ? 1 : EDIBgbl::kgf2N))));
		rsTZA->PutCollect(_T("pgz1"),_variant_t((float)(Sgn * sngOpLoad * gintParallelNum * (Unit_Force == _T("kgf") ? 1 : EDIBgbl::kgf2N))));
		signOfPaz0 = Sgn;
	 }

	  rsTmpSelSpring->get_Collect((_variant_t)_T("CoefOfVarLoad"),&v1);
      rsTZA->PutCollect(_T("CoefOfVarLoad"),_variant_t(vtof(v1)));
      //rsTZA->Update();
      CString SQLx;
	  sTmp.Format(_T("%d"),_ttoi(mZdjType.Mid(2,2)));
      SQLx = _T("SELECT * FROM [") + tbnSPRINGproperty + _T("] WHERE seq=") + sTmp;
	  _RecordsetPtr rs1;
	  rs1.CreateInstance(__uuidof(Recordset));
	  rs1->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
		  adOpenStatic, adLockOptimistic, adCmdText); 
      if(rs1->BOF && rs1->adoEOF)
         FrmTxsr.m_pViewTxsr->m_PagItem->m_SPRrigid=_T("");
      else
	  {
         if( bLineSPR )
		 {
            rs1->MoveLast();
			rs1->get_Collect((_variant_t)_T("G"),&v1);
			FrmTxsr.m_pViewTxsr->m_PagItem->m_SPRrigid.Format(_T("%d%s%.2fkgf/mm"),iSumSerialNum,mZdjType.Mid(2,2)+GetResStr(IDS_MsgBox_60684),vtof(v1)/iSumSerialNum);
         }
		else
			 FrmTxsr.m_pViewTxsr->m_PagItem->m_SPRrigid=_T("");
	  }
	  FrmTxsr.m_pViewTxsr->UpdateData(false);
	  FrmTxsr.m_pViewTxsr->m_PagItem->UpdateData(false);
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

void modPHScal::AutoSelSpringNo(_RecordsetPtr rss,float fpgz,float fpaz,float fyr,float fSPR_CHZBH,	float& fTotalInstallCompressValue,
								long lSelSampleID,int nRepZdjType,int iSumDistOfSpring,bool BHintRepZdjType,CString strSpringOrder,
								CString strZdjType,bool bAutoApplyChzbh,CString strbnSPRINGproperty,CString strbnDiscSPRINGpropertyMaxDist)
{
   //Ŀ��:���㲢��ѡ������/�����Ե��ɺ�
   //���룺rss--��ǰ���̵�ǰ����֧����ԭʼ���ݼ�¼��
   //��������ɱ��(����������)��֧�������ͣ�����������
   //������ѡ��ĵ�����Ϣ����PrjDirĿ¼��workprj.mdb����tmpSelectedSpring�����䰴���ر仯ϵ�����������򡣷���Ҫ��ĺ��ر仯ϵ����������С�ĵ�����ѡ��
	try{
	   /*if(!ModEncrypt::gbLegalUser)
		{
			CTime tm=CTime::GetCurrentTime();
			if(tm.GetSecond()% 8 !=0)
			{
				//�Ƿ��û���ֻ��1/3��ʱ����Լ���
				ShowMessage(GetResStr(IDS_DemoVersionOutDate));
				return;
			}
		}*/
	   CString strErrDesc;

	   //����ԭ���ĵ������͡�
	   CString strOldZdjType;

		LONG i=0 ,j=0;
		float deltaZ1 =0, Pmax=0 , Tmpyr=0 , TmpPgz=0 , TmpPaz=0 , mvCoefOfVarLoad=0 , mvHaz=0 ;
//		int iSumSerialNum;
		float paz0=0, mvF=0 ;
		CString FD;
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rs2;
		rs2.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rsw;
		rsw.CreateInstance(__uuidof(Recordset));
		//FD As Field, rs As Recordset, rs1 As Recordset, rs2 As Recordset, rsw As Recordset
		bool bLineSPR =false;//Ĭ��Ϊ����
		float mvDn=0 , mvFn=0 , mvD =0, mvSN =0, mvCoef=0 , mvChzbh=0 ;
		float mvMaxDistActual=0; //����������ʵ�����λ��,mm
		float mvMaxDistTheoretical=0; //���������������λ��,mm
		bool bFoundSPR=0;
		float sngWeight=0,sngHeight=0,sngDifPSAinstallLoad=0;
	   CString sTmpSPRPROPERTY = _T("tmpSPRProperty");
	   CString sTmpSelectedSpring = _T("tmpSelectedSpring");
	   CString SQLx,sTmp,sTmp2;
	   //��ʼ�����У�һ·�������ܵİ�װѹ��ֵ֮��
	   fTotalInstallCompressValue = 0;

		strOldZdjType=vtos(rss->GetCollect(_T("psaTYPE")));
	   //��֤sTmpSelectedSpring����
	   if( EDIBgbl::tdfExists(EDIBgbl::dbPRJ, sTmpSelectedSpring) )
		{
			SQLx = _T("DROP TABLE [") + sTmpSelectedSpring + _T("]");
		  EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
	   }
	   //����SelectedSpring��ṹ��sort.mdb�е�workprj.mdb
	   if( ! EDIBgbl::tdfExists(EDIBgbl::dbPRJ, sTmpSelectedSpring.Mid(3)) )
		{
		  //MsgBox ResolveResString(iUE_NotExistsSelectedSpringTableInWorkPrjMdb)
		  //Err.Raise iUE_NotExistsSelectedSpringTableInWorkPrjMdb, , ResolveResString(iUE_NotExistsSelectedSpringTableInWorkPrjMdb, _T("|1"), ProjectDir)
		  SQLx = _T("SELECT * INTO [") + sTmpSelectedSpring.Mid(3) + _T("] IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' FROM [") + sTmpSelectedSpring.Mid(3) + _T("]");
		  EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071101 "dbSORT" ��Ϊ "dbPHScode"
	   }
	   SQLx = _T("DELETE * FROM [") + sTmpSelectedSpring.Mid(3) + _T("]");
	   EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
	   SQLx = _T("SELECT * INTO [") + sTmpSelectedSpring + _T("] FROM [") + sTmpSelectedSpring.Mid(3) + _T("]");
	   EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
	   //���ر仯ϵ��mvChzbhӦ�þ�����Ӧ�������������ʱһ��,Ϊ��,�Ȼ�ô�ֵ(����еĻ�)��
	   //����AutoPHS41����ɫ�㷨֮һ�������Ʋ⣬�⽫�������ٵ��ɴ����Թ�ϵӦ��������ɵ�ƫ�
	   if( bAutoApplyChzbh )
	   {
		  if( ! IsNull(rss->GetCollect(_T("Paz0"))) && vtof(rss->GetCollect(_T("Paz0"))) != 0 )
		  {
			 mvChzbh = fabs(1 - fabs(vtof(rss->GetCollect(_T("Paz0"))) / (fpgz*gintParallelNum)));
			 paz0 = vtof(rss->GetCollect(_T("paz0"))) * (Unit_Force == _T("kgf") ? 1 : 1 / EDIBgbl::kgf2N);
		  }
		  else
		  {
			 mvChzbh = fSPR_CHZBH;
			 paz0 = 0;
		  }
	   }
	   else
	   {
		  //����Ӧ���ر仯���عر�ʱ
		  mvChzbh = fSPR_CHZBH;
	   }
   
	   //�ж��Ƿ�Ϊ���Ե���?
	   SQLx = _T("SELECT * FROM [") + strbnSPRINGproperty + _T("]");
	   rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
		   adOpenStatic, adLockOptimistic, adCmdText); 
		int CF=rs->Fields->Count;
		FieldPtr fldInfo;
		for(i=0;i<CF;i++)
		{
			rs->Fields->get_Item((_variant_t)i, &fldInfo);
			CString strName;
			strName = (LPTSTR)fldInfo->Name;
			if(strName==_T("G"))
			{
			 //���Ե����иն��ֶ�_T("G")
				bLineSPR=true;
				break;
			}
		}
		rs->Close();
	   i = Cavphs->GetPhsStructFromSampleID(lSelSampleID);
		//dn=Cavphs->PA;
	   dn = vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")));
	   //i = CheckFieldifNull(rss)
	   //Calculate paralleling spring numbers
	   CalSpringParallelNum(dn); //���㲢����
	   //MsgBox iParallelNum
	   //���ɹ�������=fpgz/iParallelNum
	   
// 	   if(gbHotStatus2Zero)
// 	   {
// 		   TmpPgz = fpgz / iParallelNum;
// 		   TmpPaz = fpaz / iParallelNum;
		   TmpPgz = fpgz ;
		   TmpPaz = fpaz ;
		   TmpPgz = fabs(TmpPgz);
		   TmpPaz = fabs(TmpPaz);
// 	   }
// 	   else
// 	   {
// // 		   //TmpPaz = fpaz / iParallelNum;
// 		   TmpPaz = fpaz ;
// // 		   //������̬����Ĺ�������
// // 		   //TmpPgz = fpgz / iParallelNum;
//  		   TmpPgz =fpgz ;
// 		   TmpPgz = fabs(TmpPgz);
// 		   TmpPaz = fabs(TmpPaz);
// 	   }
	   //if( rss(_T("ifLongVertPipe")) ){
		  //�����,����ֱ�ܵ��ϲ��ĸ��Ե��ܣ����ذ�ȫ�����ܿ���;
		  //��Ϊ�ǵ��ɣ���Ҫע�͵�
		  //TmpPgz = TmpPgz * 2
	   //}
	   
	   Tmpyr = fyr;           //fyr ��λ:mm
   
	   //��ʼ����ѡ��
		  //����/�����Ե���ѡ��
		  //ѡ������������Ҫ��ĸ��ֵ��ɺţ����������⽫���ɴ�Ns-Ne��һ�����ɺű�
	   if(gbHotStatus2Zero)
	   {
		   //��̬���㣬�ӹ������ؿ�ʼ��
			sTmp.Format(_T("%g"),TmpPgz);
	   }
	   else
	   {
		   //��̬���㣬�Ӱ�װ���ؿ�ʼ��
		   sTmp.Format(_T("%g"),TmpPaz);
	   }
		  SQLx = _T("SELECT DISTINCT seq FROM [") + sTmpSPRPROPERTY + _T("] WHERE Pgz>=") + sTmp + _T(" ORDER BY seq");
		  //û�б�Ҫ�������жϺ�������������Ϊ�����Ĳ������п����б���Ƿ���С���س����������ػ�װ���ء�
		  //if( bLineSPR ){
			 //SQLx = _T("SELECT seq FROM [") + TBNSPRINGproperty + _T("] WHERE Pgzmin<=") + TmpPgz + _T(" AND Pgzmax>=") + TmpPgz + _T(" ORDER BY seq")
		  //}else{
			 //SQLx = _T("SELECT seq FROM [") + strbnDiscSPRINGpropertyMaxDist + _T("] WHERE Pgzmin<=") + TmpPgz + _T(" AND Pgzmax>=") + TmpPgz + _T(" ORDER BY seq")
		  //}
		  rs1->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			  adOpenStatic, adLockOptimistic, adCmdText); 
		  if( rs1->BOF && rs1->adoEOF ){
			 //�Ҳ������������ĵ��ɺ�
			  sTmp.Format(GetResStr(IDS_NotFoundDiscSPRNoInsTmpSPRPROPERTY),SQLx);
			  throw sTmp;
		  }
		  else
		  {//9
			 //�����й����������������ĵ��������ѡ���ȴ���С�ſ�ʼ
			 //�úŵ��ɵĵ�ǰ����Ƭ��Ϊj,
			 //���絯�ɺ�12����ǰ����Ƭ��Ϊ3�������ͺž���312
				   
				   CString seq,Sj;
			 while(!rs1->BOF)
			 {//8
				//ȷ�����ɴ���Ƭ��,����ʯ������������Ϊ��1��5
				for( j = 1 ;j<= giAllowedMaxSerialNumSPRs;j++)
				{//7
				   //ͨ�����Ӵ�������ʹ�úŵ�������λ�Ƽ����ر仯ϵ��Ҫ��
					_variant_t v,v2;
					rs1->get_Collect((_variant_t)_T("seq"),&v);
					seq.Format(_T("%d"),vtoi(v));
					Sj.Format(_T("%d"),j);
				   //�ñ�ֻ��Ҫǰ��������Լ�����Դ���ġ�ע��ñ���ZdjCrude.mdb��
					if( bLineSPR )//ȡ�������λ��(start)
					{//1 //�Ƿ������Ե���(start)
					  //���Ե��ɣ������Ե��ɱ�
					  SQLx = _T("SELECT * FROM [") + strbnSPRINGproperty + _T("] WHERE seq=") + seq + _T(" ORDER BY Seq");
					}//1
					else
					{//1
					  //�����Ե��ɣ�������Ե��ɱ�
					  SQLx = _T("SELECT * FROM [") + strbnDiscSPRINGpropertyMaxDist + _T("] WHERE seq=") + seq;// + _T(" ORDER BY Seq,SerialNum,Dist");
					}//1 //�Ƿ������Ե���(end)
				   if(rs2->State == adStateOpen)
					   rs2->Close();
				   rs2->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
					   adOpenStatic, adLockOptimistic, adCmdText); 
				   if(rs2->adoEOF && rs2->BOF)
				   {//1
					  //MsgBox iUE_NotFoundDiscSPRNoInsTmpSPRPROPERTY
					  //Exit Sub
					   sTmp.Format(GetResStr(IDS_NotFoundDiscSPRNoInsTmpSPRPROPERTY),SQLx);
					  throw sTmp;
					   //Err.Raise iUE_NotFoundDiscSPRNoInsTmpSPRPROPERTY, , ResolveResString(iUE_NotFoundDiscSPRNoInsTmpSPRPROPERTY, _T("|1"), SQLx)
				   }//1
				   else
				   {//1					  
					  //rs2.MoveFirst //��Ϊ��ǰ�����������ִ�����ֲ���
					   rs2->get_Collect((_variant_t)(_T("Theoretical")+Sj),&v);
					   rs2->get_Collect((_variant_t)Sj,&v2);
					  if(v.vt==VT_NULL || v2.vt==VT_NULL)
						  break;
					  //�������λ��
					  mvMaxDistTheoretical = vtof(v);
				   }//1//ȡ�������λ��(end)
				   //ѡ��ͬһ�ŵ��ɵĺ���λ�Ʊ���λ��ֵ���򡣸ñ��SerialNum�ֶμ�¼�úŵ��ɵĵ�ǰ������
				   SQLx = _T("SELECT * FROM [") + sTmpSPRPROPERTY + _T("] WHERE seq=") + seq + _T(" AND SerialNum=") + Sj + _T(" AND NOT ISNULL(pgz) ORDER BY Seq,SerialNum,Dist");
				   if(rs->State == adStateOpen)
					   rs->Close();
				   rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
					   adOpenStatic, adLockOptimistic, adCmdText); 
					if(!rs->adoEOF && !rs->BOF)
				   {//6
					  rs->MoveLast();
					  //���һ����¼��λ��ֵ������ǵ�ǰ������ʱ����ʵ�ʹ���λ�Ʒ�Χ���Ͻ�
					  rs->get_Collect((_variant_t)_T("dist"),&v);
					  mvMaxDistActual = vtof(v);
					  //�����ǰ���������λ��������Ҫ����λ��ֵ
						rs2->get_Collect((_variant_t)Sj,&v);
					  if( vtof(v) >= fabs(fyr) )
					  {//5
						  //�ҳ�������������Ҫ��ʱ����ǰ���ɵ�λ��ֵ
						  if(gbHotStatus2Zero)
							  sTmp.Format(_T("%g"),TmpPgz);
						  else
							  sTmp.Format(_T("%g"),TmpPaz);					 
						  _variant_t vTmp;
						  rs->Find((_bstr_t)(_T("Pgz>=") +sTmp), 0, adSearchForward);
						  if( !rs->adoEOF)
						  {//4
							//�ҵ�������Ҫ��ֵ��ȷ���������ش��ĵ���λ��
							//��¼��ǰλ��ֵmvDn������Ӧ�ڵ�ǰrs(_T("seq"))�ŵ��ɴ���rs(_T("SerialNum"))Ƭʱ��λ��ֵ
							rs->get_Collect((_variant_t)_T("dist"),&v);
							mvDn =vtof(v); 
							//��¼��ǰ����ֵ
							rs->get_Collect((_variant_t)_T("pgz"),&v);
							mvFn =vtof(v);
							if( mvDn > 0 )
							{//1
							   //�������ش��ĵ���λ��>0,��ǰ��¼
							   //���ǵ�һ����¼,�����ƶ���ǰһ����¼,�Ա��ֵ
							   rs->MovePrevious();
							   rs->get_Collect((_variant_t)_T("dist"),&v);
							   rs->get_Collect((_variant_t)_T("pgz"),&v2);
							   if(gbHotStatus2Zero)
								   mvD = (mvDn - vtof(v)) / (mvFn - vtof(v2)) * (TmpPgz - vtof(v2)) + vtof(v);
							   else
								   mvD = (mvDn - vtof(v)) / (mvFn - vtof(v2)) * (TmpPaz - vtof(v2)) + vtof(v);
							}//1
							else
							{//1
							   //�������ش��ĵ���λ��������0,˵���ǵ�һ����¼
							   mvD = mvDn;
							}//1
							if(gbHotStatus2Zero && ((fyr > 0 && (mvD + fyr) > mvMaxDistActual) || (fyr < 0 && (mvD + fyr) < 0) ) || !gbHotStatus2Zero && ((fyr < 0 && (mvD - fyr) > mvMaxDistActual) || (fyr > 0 && (mvD - fyr) < 0) ) )
							{//1
								////��̬����ʱ:
							   //���ʱ���ϵ���λ������ֵ,���²�,�������ֵ;
							   //���µ���λ���Ǹ�ֵ,���ϲ�,С����Сֵ0;
							   //˵����ʱ��λ��̫�󣬳����úŵ��ɹ���λ������Χ
							   //MsgBox rs.Fields(_T("seq")) + _T(",dist=") +GetFields(rs,_T("dist"))
								////��̬����ʱ:
							   //���ʱ���µ���λ���Ǹ�ֵ,���²�,�������ֵ;
							   //���ϵ���λ������ֵ,���ϲ�,С����Сֵ0;
							   //˵����ʱ��λ��̫�󣬳����úŵ��ɹ���λ������Χ
							   //MsgBox rs.Fields(_T("seq")) + _T(",dist=") +GetFields(rs,_T("dist"))
							}//1
							else
							{//3
							   //��λ���ڵ��ɹ�����Χ֮��,
							   //����λ�Ʋ����װ����
								if(gbHotStatus2Zero)
									sTmp.Format(_T("%g"),(mvD + fyr));
								else
									sTmp.Format(_T("%g"),(mvD - fyr));
                           
								rs->Find((_bstr_t)(_T("Dist>=") +sTmp), 0, adSearchForward);
								if( !rs->adoEOF)
								{//2
								  //��Ҫ��ֵ��ȷ����װ����
								   rs->get_Collect((_variant_t)_T("dist"),&v);
									mvDn =vtof(v); 
									rs->get_Collect((_variant_t)_T("pgz"),&v);
									mvFn =vtof(v);
									if( mvDn > 0 )
									{//1
									 //���ǵ�һ����¼,�����ƶ���ǰһ����¼,�Ա��ֵ
										rs->MovePrevious();
										rs->get_Collect((_variant_t)_T("dist"),&v);
										rs->get_Collect((_variant_t)_T("pgz"),&v2);
										if(gbHotStatus2Zero)
											mvF = (mvFn - vtof(v2)) / (mvDn - vtof(v)) * (mvD + fyr - vtof(v)) + vtof(v2);
										else
											mvF = (mvFn - vtof(v2)) / (mvDn - vtof(v)) * (mvD - fyr - vtof(v)) + vtof(v2);

									}//1
									else
									{//1
									 //�������ش��ĵ���λ��������0,˵���ǵ�һ����¼
										mvF = mvFn;
									}//1
									//���ɺ��ر仯ϵ��mvCoefOfVarLoad
									if(gbHotStatus2Zero)
										mvCoefOfVarLoad = fabs(1 - mvF / TmpPgz);
									else
										mvCoefOfVarLoad = fabs(1 - mvF / TmpPaz);
								  //����(��װ)����ѹ��ֵ=���ɰ�װѹ��ֵ+����Ԥѹ��ֵ
								  //���ɹ���ѹ��ֵ=mvD
								  //���ɰ�װѹ��ֵmvDaz=mvD+fyr=mvHaz
								  //mvD���ڵ�ǰ������=j���Ե�ǰԤѹ��ֵ��Ϊ0λ�ƣ�������ĵ�ǰ���ɵĹ�������ԣ�ѹ��ֵ��
								if(gbHotStatus2Zero)
									mvHaz = mvD + fyr;
								else
									mvHaz = mvD - fyr;
								  //mvHaz = mvD / j + fyr
								  //Debug.Print rs.Fields(_T("seq")), mvHaz, mvF
								  //��������
									rs->get_Collect((_variant_t)_T("seq"),&v);
									sTmp.Format(_T("%2d"),vtoi(v));
								  sngWeight = sngSpringWeight(Sj + _T("1") + sTmp, rss, 1, sngHeight);
								  //�¾�ż������INSERT INTO������
				        		  //sngWeight = sngSpringWeightNormal(j + _T("1") + Format(rs(_T("seq")), _T("00")), rss, 1, sngHeight)
								  //MsgBox sngHeight
								  if( paz0 == 0 )
								  {//1
									 sngDifPSAinstallLoad = 1;
								  }//1
								  else
								  {//1
									 sngDifPSAinstallLoad = fabs(1 - fabs(mvF / paz0));
								  }//1
								  //����SelectedSpring���Ա����ʱѡ�񵯻�
								  short nSEQ;//���ɺ�
								  rs->get_Collect((_variant_t)_T("seq"),&v);
								  nSEQ=vtoi(v);

								  float mPreCmprs;//Ԥѹ��ֵ,mm
								  rs->get_Collect((_variant_t)_T("PreCmprs"),&v);
								  mPreCmprs=vtof(v);
								  								  								  
								  float mOpDist;//����λ��,mm
								  if(gbHotStatus2Zero)
									  mOpDist=mvD;
								  else
									  mOpDist=mvHaz;
								  
								  float mHaz;//��װλ��,mm
								  if(gbHotStatus2Zero)
									  mHaz=mvHaz;
								  else
									  mHaz=mvD;

								  float mDifToMidLine;//����״̬ƫ�����ߵ�ֵ,һ��<0.5�ķǸ���
 								  rs2->get_Collect((_variant_t)Sj,&v);
								  mDifToMidLine=fabs(0.5 - mOpDist/vtof(v));
								 
								 float mOpLoad;//��������,kgf
								 if(gbHotStatus2Zero)
									mOpLoad=TmpPgz;
								 else
									mOpLoad=mvF;

								 float mInstallLoad;//��װ����,kgf
								 if(gbHotStatus2Zero)
									mInstallLoad=mvF;
								 else
									mInstallLoad=TmpPaz;
								 
								 //Ӧ���������paz0
								
								  SQLx.Format("INSERT INTO [%s] (SerialNum,SEQ,SpringTypeNo,CoefOfVarLoad,PreCmprs,MaxDist,        OpDist,Haz,DifToMidLine,   Weight,Height,difPSAinstallLoad,        OpLoad,InstallLoad,paz0)\
									                     VALUES (%s,      %d,  \'%s%02d\',        %g,            %g,      %g,      %g,   %g,  %g,             %g,     %g,    %g,                      %g,    %g,         %g)",\
									         sTmpSelectedSpring, Sj,   nSEQ,    Sj,nSEQ,   mvCoefOfVarLoad,mPreCmprs,mvMaxDistActual,mOpDist,mHaz,mDifToMidLine,sngWeight,sngHeight,sngDifPSAinstallLoad,mOpLoad,mInstallLoad,paz0);
								EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
							   }//2
							   else
							   {//1
								  //û�ҵ�TmpPgzƥ��ֵ
							   }//1
							}//3
						 }//4
					  }//5
				   }//6
				}//7
				rs1->MoveNext();
			 }//8
		  }//9
   
		  rs->Close();
		  rs2->Close();
		  //��ʼ����ѡ���ĵ���ѡ�����ӽ�����й�����Ҫ�ĵ���
		   SQLx = _T("SELECT * FROM [") + sTmpSelectedSpring + _T("]  WHERE CoefOfVarLoad<=");
		   sTmp.Format(_T("%g"),mvChzbh);
		   SQLx+= sTmp + _T(" ") + gstrSpringOrderSQL;
		   rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
			   adOpenStatic, adLockOptimistic, adCmdText); 
		   if( rs->BOF && rs->adoEOF )
		   {//�Ǻ�������(start)
			  //ѡ�񲻵���Ҫ�ĵ��ɣ���ѡ���������
			  //MsgBox ResolveResString(iUE_OverAllowedMaxSerialNumSPRs)
			  CalSpringParallelNum();
			  AutoSelConstantSpringNo(rss,fpgz,fyr,gbCSPRneedSpecialDesign,gUseConstantSpringMinSurplusDist,lSelSampleID,
									strZdjType,giWholeSprNum,gbAutoPASA,giSumSerialNum,tbnHDproperty,tbnDisplacementSerial);
			  bFoundSPR = false;
		   }//�Ǻ�������(end)
		   else
		   {//2	//���Ǻ�������(start)
			  //if( strSpringOrder = _T("�ֹ�") ){
			  if( strSpringOrder == GetResStr(IDS_ManualSelSpring) )
			  {//1
			  }//1
			  else
			  {//1
				 //���㵯�ɺ��ر仯ϵ��<=����ĵ��ɺ��ر仯ϵ��,��������
				  _variant_t v;
				 rs->MoveFirst();
				 if(strOldZdjType!="")
				 {
					FindPSAType(strOldZdjType, rs);//pfg20051101 Ӧ�������֧���������Ƿ����
				 }
				 rs->get_Collect((_variant_t)_T("MaxDist"),&v);
				 iSumDistOfSpring = vtoi(v);//�����λ�ƣ����ڱ���С�
				 //���õ��ɼ�������Ϣ
				 SetSpringPhsInfo(rs, rss, bLineSPR,strZdjType);
				 rs->get_Collect((_variant_t)_T("OpDist"),&v);
				if(gbHotStatus2Zero)
					fTotalInstallCompressValue = vtof(v) + fyr;
				else
					fTotalInstallCompressValue = vtof(v) + fyr;
				 sngSpringWeightNormal(strZdjType, rss, fTotalInstallCompressValue);
				 //tmp = sngSpringWeight(strZdjType, rss, fTotalInstallCompressValue)
			  }//1
		   }//2	//���Ǻ�������(end)

		 // nRepZdjType=-1����ѡ��nRepZdjType=1���ǣ� nRepZdjType=2����
		 if(BHintRepZdjType && (nRepZdjType==-1 || nRepZdjType==2) && strOldZdjType!=_T("") && strOldZdjType!=strZdjType && strOldZdjType.GetLength()==4)
		 {
			 if(nRepZdjType==-1)
			 {
					//ֻ�е����ϵ��ɶ�����ѡ��ʱ���Ż���ֵ�һ����ʾ
					//AutoPHS����ĵ�����Ӧ����������GLIF����Ľ����һ�£��Ұ���Ӧ�������������Ľ��
					rs->Close();
				   SQLx = _T("SELECT * FROM [") + sTmpSelectedSpring + _T("]  WHERE CoefOfVarLoad<=");
				   sTmp.Format(_T("%g"),mvChzbh);
				   SQLx+= sTmp + _T(" AND SerialNum=") + strOldZdjType.Left(1) + _T(" AND SEQ=") + strOldZdjType.Right(2);
				   rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
					   adOpenStatic, adLockOptimistic, adCmdText); 
				   //ShowMessage(rs.GetRecordCount());
				   if( rs->BOF && rs->adoEOF )
				   {
					  //ѡ�񲻵���Ҫ��GLIF���㵯�ɣ���ѡ��AutoPHS���㵯��
						rs->Close();
						SQLx = _T("SELECT * FROM [") + sTmpSelectedSpring + _T("]  WHERE CoefOfVarLoad<=");
						sTmp.Format(_T("%g"),mvChzbh);
						SQLx+= sTmp + _T(" AND SerialNum=") + strZdjType.Left(1) + _T(" AND SEQ=") + strZdjType.Right(2);
						rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
							adOpenStatic, adLockOptimistic, adCmdText); 
						//ShowMessage(rs.GetRecordCount());
						if( rs->BOF && rs->adoEOF )
						{
							//ѡ�񲻵���Ҫ��GLIF����ĵ��ɣ�Ҳѡ�񲻵�AutoPHS����ĵ���	
						  //ֻ��ѡ���������
						  //MsgBox ResolveResString(iUE_OverAllowedMaxSerialNumSPRs)
						  CalSpringParallelNum();
						  AutoSelConstantSpringNo(rss,fpgz/gintParallelNum,fyr,gbCSPRneedSpecialDesign,gUseConstantSpringMinSurplusDist,lSelSampleID,
												strZdjType,giWholeSprNum,gbAutoPASA,giSumSerialNum,tbnHDproperty,tbnDisplacementSerial);
						  bFoundSPR = false;
						}
						else
						{
						  //if( strSpringOrder = _T("�ֹ�") ){
						  if( strSpringOrder == GetResStr(IDS_ManualSelSpring) )
						  {
						  }
						  else
						  {
								//ֻ��ѡ��AutoPHS����ĵ���
								nRepZdjType=1;
								//ǰ���Ѿ�ѡ����һ��
						  }
						}
				   }
				   else
				   {
					  //����ѡ����Ҫ��GLIF���㵯�ɣ��ٿ��Ƿ���ѡ��AutoPHS���㵯��
						rs->Close();
						SQLx = _T("SELECT * FROM [") + sTmpSelectedSpring + _T("]  WHERE CoefOfVarLoad<=");
						sTmp.Format(_T("%g"),mvChzbh);
						SQLx+= sTmp + _T(" AND SerialNum=") + strZdjType.Left(1) + _T(" AND SEQ=") + strZdjType.Right(2);
						rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
							adOpenStatic, adLockOptimistic, adCmdText); 
						//ShowMessage(rs.GetRecordCount());
						if( rs->BOF && rs->adoEOF )
						{
							//ѡ����Ҫ��GLIF���㵯�ɣ�ѡ�񲻵�AutoPHS���㵯��	
						  //ֻ��ѡ��GLIF���㵯��,ִ�д���
							nRepZdjType=2;
						}
						else
						{
						  //if( strSpringOrder = _T("�ֹ�") ){
						  if( strSpringOrder == GetResStr(IDS_ManualSelSpring) )
						  {
						  }
						  else
						  {
								//ѡ����Ҫ��GLIF���㵯�ɣ�Ҳѡ��AutoPHS���㵯��
							  //������ʾ
								CString sMessage;
								sMessage.Format(GetResStr(IDS_YESNOCALSPR),strZdjType,strOldZdjType);
								if(ShowMessage(sMessage,MB_YESNO) == IDNO)
									nRepZdjType=2;
								else
									nRepZdjType=1;
						  }
						}
				   }
			 }
			 if(nRepZdjType==2)
			 {
					//AutoPHS����ĵ�����Ӧ����������GLIF����Ľ����һ�£��Ұ���Ӧ�������������Ľ��
					strZdjType=strOldZdjType;
					rs->Close();
				   SQLx = _T("SELECT * FROM [") + sTmpSelectedSpring + _T("]  WHERE CoefOfVarLoad<=");
				   sTmp.Format(_T("%g"),mvChzbh);
				   SQLx+= sTmp + _T(" AND SerialNum=") + strOldZdjType.Left(1) + _T(" AND SEQ=") + strOldZdjType.Right(2);
				   rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
					   adOpenStatic, adLockOptimistic, adCmdText); 
				   //ShowMessage(rs.GetRecordCount());
				   if( rs->BOF && rs->adoEOF )
				   {
					  //ѡ�񲻵���Ҫ�ĵ��ɣ���ѡ���������
					  //MsgBox ResolveResString(iUE_OverAllowedMaxSerialNumSPRs)
					 CalSpringParallelNum();
					 AutoSelConstantSpringNo(rss,fpgz/gintParallelNum,fyr,gbCSPRneedSpecialDesign,gUseConstantSpringMinSurplusDist,lSelSampleID,
											strZdjType,giWholeSprNum,gbAutoPASA,giSumSerialNum,tbnHDproperty,tbnDisplacementSerial);
					  bFoundSPR = false;
				   }
				   else
				   {
					  //if( strSpringOrder = _T("�ֹ�") ){
					  if( strSpringOrder == GetResStr(IDS_ManualSelSpring) )
					  {
					  }
					  else
					  {
							//���㵯�ɺ��ر仯ϵ��<=����ĵ��ɺ��ر仯ϵ��,��������
							_variant_t v;
							rs->MoveFirst();
							rs->get_Collect((_variant_t)_T("MaxDist"),&v);
							gSumDistOfSpring = vtoi(v);//�����λ�ƣ����ڱ���С�
							//���õ��ɼ�������Ϣ
							SetSpringPhsInfo(rs, rss, bLineSPR,strZdjType);
							rs->get_Collect((_variant_t)_T("OpDist"),&v);
							if(gbHotStatus2Zero)
								fTotalInstallCompressValue = vtof(v) + fyr;
							else
								fTotalInstallCompressValue = vtof(v) + fyr;
							sngSpringWeightNormal(strZdjType, rss, fTotalInstallCompressValue);
							//tmp = sngSpringWeight(strZdjType, rss, fTotalInstallCompressValue)
					  }
				   }
			 }
		 }
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	FrmTxsr.m_pViewTxsr->UpdateData(false);
}

void modPHScal::AutoSelConstantSpringNo(_RecordsetPtr rss,float fpgz,float fyr,bool bCSPRneedSpecialDesign,float fUseConstantSpringMinSurplusDist,long lSelSampleID,
										CString strZdjType,int& iWholeSprNum,bool bAutoPASA,int nSumSerialNum,CString strbnHDproperty,CString strbnDisplacementSerial)
{	
	try
	{
		int i=0;
		long iSumSerialNum=0;
		float Tmpyr=0.0, TmpPgz=0.0, tmpCapacity=0.0;
		float Cmin=0 , Cmax=0 ;  //��һ��/���һ�������غ�����
		float C1=0, C2=0;
		CString sTmp1,sTmp2,sTmp,SQLx;
		_variant_t vTmp;
		float tmpDist=0 ;
		bool bFoundCSPR=false;

		_ConnectionPtr db;
		db.CreateInstance(__uuidof(Connection));
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr rs1;
		rs1.CreateInstance(__uuidof(Recordset));
		long lngMaxSerialCSPR=0;

		//���ú����Ҫ������Ʊ�־Ϊ��
		bCSPRneedSpecialDesign = false;
		 CalSpringParallelNum();  //���㲢����			 
		//���ɹ�������=fpgz/gintParallelNum
		//If rss(_T("ifLongVertPipe")) Then
		//�����,����ֱ�ܵ��ϲ��ĸ��Ե��ܣ����ذ�ȫ�����ܿ���;
		//��Ϊ�ǵ��ɣ���Ҫע�͵�

		//TmpPgz = fabs(fpgz) / gintParallelNum;
		TmpPgz = fabs(fpgz) ;
		Tmpyr = fyr ;          //fyr ��λ:mm
		if (fabs(Tmpyr) * gUseConstantSpringPercentSurplusDist < fUseConstantSpringMinSurplusDist )
			Tmpyr = fabs(Tmpyr) + fUseConstantSpringMinSurplusDist;
		else
			Tmpyr = fabs(Tmpyr) * (1 + gUseConstantSpringPercentSurplusDist);
		
		//ʹ��ѭ����ȷ����Ҫ�ĺ������ɴ�����,�����������
		lSelSampleID = vtoi(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("iSelSampleID")));

		//����ĺ��������˵��ɵ�CustomID��
		lngMaxSerialCSPR = Cavphs->GetPhsIsCSPR(iCSPR, lSelSampleID);
		//��ʾ�����ı���
		if( lngMaxSerialCSPR == 0 )
		{
			sTmp1.Format(_T("%d"),lSelSampleID);
			sTmp2.Format( GetResStr(IDS_PleaseSelectCSPRSampleID), sTmp1);
			FrmTxsr.m_pViewTxsr->m_LabelMsgbox.SetWindowText(sTmp2);
			FrmTxsr.m_pViewTxsr->m_LabelMsgbox.ShowWindow(SW_SHOW);

			//��ʼ�Զ�ѡ���������
			strZdjType = _T("9999");
			iWholeSprNum = 1;
			bAutoPASA = false;

			lSelSampleID = Cavphs->GetphsStructIDsemiAuto();
			if( lSelSampleID == -1)
			{
				strZdjType = _T("9999");
				iWholeSprNum = 2;
				bAutoPASA = false;
				lSelSampleID = Cavphs->GetphsStructIDsemiAuto();
				if( lSelSampleID ==-1)
				{
					strZdjType = _T("9999");
					iWholeSprNum = 1;
					bAutoPASA = true;
					lSelSampleID = Cavphs->GetphsStructIDsemiAuto();
					if( lSelSampleID == -1 )
					{
						strZdjType = _T("9999");
						iWholeSprNum = 2;
						bAutoPASA = true;
						lSelSampleID = Cavphs->GetphsStructIDsemiAuto();
						if( lSelSampleID == -1)
						//ʼ��ѡ�񲻵����ʵĺ��
							return;
					}
					else
						lngMaxSerialCSPR = Cavphs->GetPhsIsCSPR(iCSPR, lSelSampleID);
				}
				else
					lngMaxSerialCSPR = Cavphs->GetPhsIsCSPR(iCSPR, lSelSampleID);
			}
			else
				lngMaxSerialCSPR = Cavphs->GetPhsIsCSPR(iCSPR, lSelSampleID);
		}
		else
		{
			FrmTxsr.m_pViewTxsr->m_LabelMsgbox.SetWindowText(_T(""));
			lngMaxSerialCSPR = Cavphs->GetPhsIsCSPR(iCSPR, lSelSampleID);
		}

		SQLx = _T("SELECT * FROM [") + strbnHDproperty + _T("] ORDER BY Capacity");
		rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
		if( rs->BOF && rs->adoEOF)
		{
			//����غ�������Ϊ�գ�������
			sTmp1.Format(GetResStr(IDS_NullTableInXMdb),EDIBgbl::GetDBName(dbZDJcrude), strbnHDproperty);
			throw sTmp1;
		}
		rs->get_Collect((_variant_t)_T("Capacity"),&vTmp);
		Cmin =vtof(vTmp);
		rs->MoveLast();
		rs->get_Collect((_variant_t)_T("Capacity"),&vTmp);
		Cmax =vtof(vTmp);
		rs->MoveFirst();
		for( iSumSerialNum =1 ;iSumSerialNum<= lngMaxSerialCSPR;iSumSerialNum++)
		{
			tmpDist = Tmpyr / iSumSerialNum;
			sTmp1.Format(_T("%d"),(long)tmpDist);
			CString m_strFilter=CString(_T(" DisplacementSerial>=")) + sTmp1;
			SQLx = _T("SELECT * FROM [") + strbnDisplacementSerial + _T("]") + _T(" WHERE ") + m_strFilter + _T(" ORDER BY DisplacementSerial");
			rs1->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			if( rs1->BOF && rs1->adoEOF);
			//λ�Ƴ���ϵ�����ֵ
			else
			{
				rs1->get_Collect((_variant_t)_T("DisplacementSerial"),&vTmp);
				tmpDist =vtof(vTmp);
				//�غ�������λΪkN*mm,��N*m,λ�Ƶ�λΪmm,���ص�λΪkgf(��daN=10N)
				tmpCapacity = TmpPgz * tmpDist * EDIBgbl::kgf2N / 1000;
				if( tmpCapacity < Cmin)
				{
				//������������С�Ļ�С
					sTmp1.Format(GetResStr(IDS_CapacityLitterCmin),_T("\%g"),_T("\%g"));
					sTmp2.Format(sTmp1,tmpCapacity, Cmin);
					throw sTmp2;
				}
				else
				{
					if( tmpCapacity > Cmax);
					//�������������Ļ���
					//��������1������ѭ��
					else
					{
					//������������
						sTmp1.Format(_T("%g"),tmpCapacity);
						_variant_t vTmp;
						rs->Find((_bstr_t)(_T("Capacity>=")+sTmp1), 0, adSearchForward);
						if( !rs->adoEOF)
						{
							//�ҵ���
							rs->get_Collect((_variant_t)_T("minDist"),&vTmp);
							if (tmpDist < vtof(vTmp))
							{
								//����Сλ��С��Ӧ���õ���
								sTmp1.Format(GetResStr(IDS_ShouldUseSpring),_T("\%g"),_T("\%g"));
								sTmp2.Format(sTmp1,tmpDist,vtof(vTmp));
								throw sTmp2;
							}
							rs->get_Collect((_variant_t)_T("maxDist"),&vTmp);
							if( tmpDist > vtof(vTmp)) 
							{
								//�����λ�ƻ���
								if( iSumSerialNum >= lngMaxSerialCSPR)
								{
									//����ģ���еĴ�����
									//If gbCSPRCanSpecialDesign Then
									//�����Ҫ������ƣ��������������ʹ�ã�,
									//ֻҪһ������Ϳ�����
									//��������λ����Ϊ��������
									bCSPRneedSpecialDesign = true;
									bFoundCSPR = true;
									break;
								}
							}
							//�������Ҫ������ƣ������������ʹ�ã�
							//����ѭ��
							//С��ģ���еĴ�����
							//����ѭ��		
							else
							{
								//����
								//��λ�ƿ�����Ϊ��������
								bFoundCSPR = true;
								break;
							}
						}
					}
				}
			}
		}

		if(!bFoundCSPR)
		{
			FrmTxsr.m_pViewTxsr->m_PagItem->m_ConstantSPRNo= _T("");
			//�������ﵽ���ֵ����û�к��ʵĺ�������
			throw GetResStr(IDS_NotFoundConstantSPRInTBNHDproperty);
		}
		else
		{
			//�ҵ����ʵĺ��
			//���ͽ�ԭ��ѡ����
			//��¼��ǰ�غ�����
			rs->get_Collect((_variant_t)_T("Capacity"),&vTmp);
			C1 = vtof(vTmp);

			//��¼ǰһ���غ�����
			rs->MovePrevious();
			rs->get_Collect((_variant_t)_T("Capacity"),&vTmp);
			C2 = vtof(vTmp);
			if( fabs(C1 - tmpCapacity) >= fabs(C2 - tmpCapacity))
				;
				//�غ��������ӽ�C2
			else
				//�غ��������ӽ�C1
				//�ص������ӽ��ļ�¼
				rs->MoveNext();

			strZdjType = _T("9999");
			rs->get_Collect((_variant_t)_T("DH"),&vTmp);
			sTmp.Format(_T("%d%d%2d"),iSumSerialNum,gintParallelNum,vtoi(vTmp));
            //sTmp.Format(_T("%d%d"),iSumSerialNum,vtoi(vTmp)) ;
			rss->PutCollect(_T("type"),_variant_t(_T("9999")));
			rss->PutCollect(_T("ConstantSPRNo"),_variant_t(sTmp));
			//�������ֵ
			iWholeSprNum = iSumSerialNum;
			//���ں������ɣ�������=��������
			nSumSerialNum = iSumSerialNum;
			rs->get_Collect((_variant_t)_T("DH"),&vTmp);
			for(i = 0;i<iSumSerialNum;i++)
			{

				sSprInfo[i].SerialNum = 1;
				sSprInfo[i].DH =vtoi(vTmp);
				sSprInfo[i].haz = fyr / iSumSerialNum * sSprInfo[i].SerialNum;
				sSprInfo[i].HeatDisp = sSprInfo[i].haz;
				sSprInfo[i].CheckDisp = 0;

			}
			FrmTxsr.m_pViewTxsr->m_PagItem->m_ConstantSPRNo.Format(GetResStr(IDS_ConstantSpringTypeIsXXXX),vtos((_variant_t)rss->GetCollect(_T("ConstantSPRNo"))));

		}
		
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

void modPHScal::SetPhsTK3()
{
//Ŀ��:��ͼ������ΪA3ͼ��.
//����:���е���Ϣ�������
	try
	{
		EDIBDB::DrawID = EDIBAcad::GetDrawIDAndName(zdjh + glDrawNoStartNumber - 1, EDIBDB::DrawName);
		if( EDIBDB::PrjName == CString(_T("")) || EDIBDB::DrawName == CString(_T("")) || EDIBDB::DrawID == CString(_T("")) || EDIBDB::DwgScale == CString(_T("")) )
		  EDIBDB::DrawInfoGet();
		
		EDIBDB::GetTBsize();

		EDIBDB::PrjName = EDIBgbl::SelPrjName;

		if(gbDrawNoPreMethod)
		  EDIBDB::DrawID = EDIBAcad::GetDrawIDAndName(zdjh + glDrawNoStartNumber - 1, EDIBDB::DrawName);
		else
		  EDIBDB::DrawID = EDIBAcad::GetDrawIDAndName(zdjh + glDrawNoStartNumber - 1, EDIBDB::DrawName);
		
		if( EDIBDB::DwgScale = 0)
			EDIBDB::DwgScale = 100;
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

void modPHScal::CalSpringParallelNum(CString& PipeA)
{
	//'Ŀ��:���㵯�ɲ�����
//����:gintParallelNum=���ɲ�����
  
	_RecordsetPtr rstbl=::FrmTxsr.m_pViewTxsr->m_ActiveRs;
	try{
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_variant_t v;
		_variant_t tmpvar;
		if(PipeA==_T(""))
		{
			tmpvar=rstbl->GetCollect(_variant_t(_T("dn1")));
			if(tmpvar.vt==VT_NULL)
				PipeA=dn;
			else
			{
				PipeA=vtos(tmpvar);
				PipeA.TrimLeft();PipeA.TrimRight();
				if(PipeA.GetLength()<=0)
					PipeA=dn;
			}
		}
	   //����ܲ���ߺ͸�����ߣ�����setBasepoint����ȷ��ͼ����С
		sngPEL = rstbl->GetCollect(_variant_t(_T("dh1"))).vt==VT_NULL ? 0 : (float)rstbl->GetCollect(_variant_t(_T("dh1")));
	   sngSEL =rstbl->GetCollect(_variant_t(_T("gh1"))).vt==VT_NULL ? 0 : (float)rstbl->GetCollect(_variant_t(_T("gh1")));

		  if (PipeA != _T(""))
		  {
			  CString SQLx;
			  SQLx = _T("SELECT * FROM PictureClipData WHERE (CustomID)=\'") + PipeA + _T("\'");
			  rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJ,true), 
				  adOpenStatic, adLockOptimistic, adCmdText); 
			 if (rs->BOF && rs->adoEOF)
			 {
				//Err.Raise iUE_NoRecordFLDEqvThisValueInPictureClipData, , ResolveResString(iUE_NoRecordFLDEqvThisValueInPictureClipData, _T("|1"), _T("CustomID"), _T("|2"), PipeA)
				 if(rstbl->GetCollect(_variant_t(_T("ParallelingNum"))).vt==VT_NULL)
				   gintParallelNum = 1;
				else
				{
				   if ((float)rstbl->GetCollect(_variant_t(_T("ParallelingNum"))) == 0)
					  gintParallelNum = 1;
				   else
					{
					  gintParallelNum =(long)rstbl->GetCollect(_variant_t(_T("ParallelingNum")));
						if (gintParallelNum>2) gintParallelNum=2;
					}
				}
			 }
			 else
			 {
				 rs->get_Collect((_variant_t)_T("ParalleledNum"),&v);
				if(v.vt==VT_NULL ||vtoi(v)<=0)
					gintParallelNum = 1;
				else
					  gintParallelNum = vtoi(v);
			 }
			 rs->Close();
		  }
		  else
		  {
			gintParallelNum = 1;
		  }

	   //�����ֶ�ֵ
			rstbl->PutCollect(_variant_t(_T("ParallelingNum")),_variant_t((long)gintParallelNum));
	}
	catch(_com_error &e)
	{
		gintParallelNum = 1;
		ShowMessage(e.Description());
	}
}

void modPHScal::PreCalCrudeData(_RecordsetPtr rstbl,int nth)
{
//*******************************
// ��  ��: ����ԭʼ����,��ó�ʼֵ
// ��  ��: ԭʼ����
// ��  ��: ������ԭʼ����
//*******************************
	if(rstbl==NULL || rstbl->State==adStateClosed)
		return;

	//2007.09.03
	float fTmpPgz=0.0;
	float fTmpPaz=0.0;

   _RecordsetPtr rs;
   rs.CreateInstance(__uuidof(Recordset));
  bool bTmp=false;
  long lngErrNum=0;
  _variant_t tmpvar;
  _variant_t v;
  CString sTmp,sTmp2,SQLx,tbn;
  float UTmp=0.0;
  try{
	   //���ɼ��ڴ˴���������ʼ��ͣ���ڵ�1����¼����
	  modPHScal::zdjh=vtoi(rstbl->GetCollect(_T("zdjh")));

	  //2007.09.03
	  fTmpPaz=vtof(rstbl->GetCollect(_T("paz1")));
	  fTmpPgz=vtof(rstbl->GetCollect(_T("pgz1")));

	   if(!IsNull(rstbl->GetCollect(_T("type"))) )
	   { 
		   modPHScal::ZdjType = Trim(vtos(rstbl->GetCollect(_T("type"))));
	   }
		if( vtoi(rstbl->GetCollect(_T("UPxyz"))) < 1 || vtoi(rstbl->GetCollect(_T("UPxyz"))) >3 )
		{
		  rstbl->PutCollect(_T("UPxyz"),_variant_t((long)2));
	   }
		giUPxyz=vtoi(rstbl->GetCollect(_T("UPxyz")));
	   if( IsNull(rstbl->GetCollect(_T("dj1"))) || rstbl->GetCollect(_T("dj1")).fltVal <= 0 )
	   {
		  rstbl->PutCollect(_T("dj1"),_variant_t((float)76));
	   }
	   modPHScal::dj = rstbl->GetCollect(_T("dj1")).fltVal;
	   //���ιܵ���ֱ����ܾ�B
	   if( IsNull(rstbl->GetCollect(_T("dwB"))) || rstbl->GetCollect(_T("dwB")).fltVal <= 0 )
	   {
		  rstbl->PutCollect(_T("dwB"),_variant_t((float)800));
	   }
	   modPHScal::dwB = rstbl->GetCollect(_T("dwB")).fltVal;
	   //dxa1/dza1��ֵ������ʾ��λ�ߴ��עʱҪ�ڹܲ��Ҳࡣ
	   if( IsNull(rstbl->GetCollect(_T("t01"))) )
	   {
		  rstbl->PutCollect(_T("t01"),_variant_t((float)0));
	   }
	   modPHScal::t0 = rstbl->GetCollect(_T("t01")).fltVal;
	   if( IsNull(rstbl->GetCollect(_T("dxa1"))) )
	   {
		  //����x����λĬ��ֵ
		 rstbl->PutCollect(_T("dxa1"),_variant_t((float)3200));
	   }
	   //ȡ����ֵ��ʹ���עΪ�ߴ�ʱ��ʾΪ��ֵ
	   modPHScal::dxa0 = rstbl->GetCollect(_T("dxa1")).fltVal;
	   //ʹdxa��ó�ֵ
	   modPHScal::dxr = dxa0;
	   if( IsNull(rstbl->GetCollect(_T("dza1"))) )
	   {
		  //����z����λĬ��ֵ
		   rstbl->PutCollect(_T("dza1"),_variant_t((float)4500));
	   }
	   //ȡ����ֵ��ʹ���עΪ�ߴ�ʱ��ʾΪ��ֵ
	   modPHScal::dza0 = rstbl->GetCollect(_T("dza1")).fltVal;
	   //ʹdza��ó�ֵ
	   modPHScal::dzr = dza0;
	   if( IsNull(rstbl->GetCollect(_T("dh1"))) )
	   {
		  //���ùܲ����Ĭ��ֵ
		  rstbl->PutCollect(_T("dh1"),_variant_t((float)8));
	   }
	   modPHScal::sngPEL = rstbl->GetCollect(_T("dh1")).fltVal;
	   if( IsNull(rstbl->GetCollect(_T("iCSnum"))) )
	   {
		  modPHScal::iCSnum = 0;
	   }
	   else
	   {
		  modPHScal::iCSnum = vtoi(rstbl->GetCollect(_T("iCSnum")));
	   }
	   if( IsNull(rstbl->GetCollect  (_T("gdw1"))) || rstbl->GetCollect(_T("gdw1")).fltVal <= 0 )
	   {
		  rstbl->PutCollect(_T("gdw1"),_variant_t((float)0));
	   }
	   modPHScal::gdw1 = rstbl->GetCollect(_T("gdw1")).fltVal;  //����Lֵ
	   //0/1=�������ò۸�/���������ò۸�
	   if( IsNull(rstbl->GetCollect(_T("Gnum"))) || vtoi(rstbl->GetCollect(_T("Gnum")) )<= 0 )
	   {
		  rstbl->PutCollect(_T("Gnum"),_variant_t((short)1));
	   }
	   modPHScal::glNumSA = vtoi(rstbl->GetCollect(_T("Gnum")));
   
	   //��λ��
	   modPHScal::CalUnitX(rstbl);
   
	   //ȷ���ܲ�����
	   modPHScal::CalfxPA (rstbl);
	   //ȷ����������
	   modPHScal::CalfxSA (rstbl);
	   if( IsNull(rstbl->GetCollect(_T("Num"))) || vtoi(rstbl->GetCollect(_T("Num"))) <= 0 )
	   {
		  rstbl->PutCollect(_T("Num"),_variant_t((short)1));
	   }
	   if( IsNull(rstbl->GetCollect(_T("gh1"))) )
	   {
		  rstbl->PutCollect(_T("gh1"),_variant_t((float)12.6));
	   }
	   modPHScal::sngSEL = rstbl->GetCollect(_T("gh1")).fltVal;  //�������
	   if( IsNull(rstbl->GetCollect(_T("haz"))) || rstbl->GetCollect(_T("haz")).fltVal <= 0 )
	   {
		  rstbl->PutCollect(_T("haz"),_variant_t((long)0));
	   }
	   if( IsNull(rstbl->GetCollect(_T("xl1"))) )
	   {
		  rstbl->PutCollect(_T("xl1"),_variant_t((float)0));
	   }
	   modPHScal::xl = rstbl->GetCollect(_T("xl1")).fltVal;
	   if( IsNull(rstbl->GetCollect(_T("yl1"))) )
	   {
		  rstbl->PutCollect(_T("yl1"),_variant_t((float)0));
	   }
	   modPHScal::yl = rstbl->GetCollect(_T("yl1")).fltVal;
	   if( IsNull(rstbl->GetCollect(_T("zl1"))) )
	   {
		  rstbl->PutCollect(_T("zl1"),_variant_t((float) 0));
	   }
	   modPHScal::zl = rstbl->GetCollect(_T("zl1")).fltVal;
	   if( IsNull(rstbl->GetCollect(_T("xr1"))) )
	   {
		  rstbl->PutCollect(_T("xr1"),_variant_t((float)0));
	   }
	   modPHScal::xr = rstbl->GetCollect(_T("xr1")).fltVal;
	   if( IsNull(rstbl->GetCollect(_T("yr1"))) )
	   {
		  rstbl->PutCollect(_T("yr1"),_variant_t((float) 0));
	   }
	   modPHScal::yr1 = rstbl->GetCollect(_T("yr1")).fltVal;
	   //��ֱ������λ��
	   if( giUPxyz == 1 )
	   {
		  modPHScal::yr = rstbl->GetCollect(_T("xr1")).fltVal;
	   }
	   else if( giUPxyz == 2 )
	   {
		  modPHScal::yr = rstbl->GetCollect(_T("yr1")).fltVal;
	   }
	   else if( giUPxyz == 3 )
	   {
		  modPHScal::yr = rstbl->GetCollect(_T("zr1")).fltVal;
	   }
	   if( IsNull(rstbl->GetCollect(_T("zr1"))) )
	   {
		  rstbl->PutCollect(_T("zr1"),_variant_t((float)0));
	   }
	   modPHScal::zr = rstbl->GetCollect(_T("zr1"));
	   if( (IsNull(rstbl->GetCollect(_T("pjg1"))) || rstbl->GetCollect(_T("pjg1")).fltVal == 0) )
	   {
		  if( (! IsNull(rstbl->GetCollect(_T("pgz1"))) && rstbl->GetCollect(_T("pgz1")).fltVal != 0) )
		  {
		  }
		  else
		  {
			 if( IsNull(rstbl->GetCollect(_T("pgz1"))) || rstbl->GetCollect(_T("pgz1")).fltVal == 0 )
			 {
				rstbl->PutCollect(_T("pgz1"),_variant_t((float) 100));
			 }
		  }
		  //�ṹ����=gsngRatioOfPjg2Pgz����������
		  if( IsNull(rstbl->GetCollect(_T("pjg1"))) || rstbl->GetCollect(_T("pjg1")).fltVal == 0 )
		  {
			  rstbl->PutCollect(_T("pjg1"), _variant_t((float)(modPHScal::gsngRatioOfPjg2Pgz * rstbl->GetCollect(_T("pgz1")).fltVal)));
			  //2007.09.03
			  if( gbMaxPgzPaz && fabs(fTmpPaz)>fabs(fTmpPgz) )
			  {
				  rstbl->PutCollect(_T("pjg1"), _variant_t((float)(modPHScal::gsngRatioOfPjg2Pgz * fTmpPaz)));
			  }
			  
		  }
	   }
	   else
	   {
		  if( (!IsNull(rstbl->GetCollect(_T("pgz1"))) && rstbl->GetCollect(_T("pgz1")).fltVal != 0) )
		  {
		  }
		  else
		  {
			 if( IsNull(rstbl->GetCollect(_T("pgz1"))) || rstbl->GetCollect(_T("pgz1")).fltVal == 0 )
			 {
				 rstbl->PutCollect(_T("pgz1"),_variant_t((float)( rstbl->GetCollect(_T("pjg1")).fltVal / modPHScal::gsngRatioOfPjg2Pgz)));
				  
				 //2007.09.03
				  if( gbMaxPgzPaz && fabs(fTmpPaz)>fabs(fTmpPgz) )
				  {
					  rstbl->PutCollect(_T("pjg1"), _variant_t((float)(modPHScal::gsngRatioOfPjg2Pgz * fTmpPaz)));
				  }
				  
			 }
		  }
	   }
	   if( fabs(rstbl->GetCollect(_T("pjg1")).fltVal)+0.01 < modPHScal::gsngRatioOfPjg2Pgz * fabs(rstbl->GetCollect(_T("pgz1")).fltVal) )//����"+0.01"pfg2006.12.28
	   {
		  if( IsNull(rstbl->GetCollect(_T("PSAver"))) || Trim(vtos(rstbl->GetCollect(_T("PSAver")))) == _T("") )
		  {
		  //��Ӧ����������õĽṹ����ֵ��������к���У�ˡ�
				sTmp.Format(GetResStr(IDS_StructureLoadLessthanOperatingLoad),_T("\%g"),_T("\%g"),_T("\%g"));
				sTmp2.Format(sTmp,rstbl->GetCollect(_T("pjg1")).fltVal,rstbl->GetCollect(_T("pgz1")).fltVal,modPHScal::gsngRatioOfPjg2Pgz);
				ShowMessage(sTmp2,MB_TOPMOST|MB_OK);
				rstbl->PutCollect(_T("pjg1"),_variant_t((float)(modPHScal::gsngRatioOfPjg2Pgz*rstbl->GetCollect(_T("pgz1")).fltVal)));

				 //2007.09.03
				  if( gbMaxPgzPaz && fabs(fTmpPaz)>fabs(fTmpPgz) )
				  {
					  rstbl->PutCollect(_T("pjg1"), _variant_t((float)(modPHScal::gsngRatioOfPjg2Pgz * fTmpPaz)));
				  }
         
		  }
	   }
	   UTmp=CalUnitCovertCoef(rstbl);
	   modPHScal::pjg = rstbl->GetCollect(_T("pjg1")).fltVal * UTmp;
	   modPHScal::pgz = rstbl->GetCollect(_T("pgz1")).fltVal * UTmp;
   
		//����Ļ�ȡ�ṹ����λ���ҹܲ�����dn1/gn1֮ǰ
	   tmpvar=rstbl->GetCollect(_T("iSelSampleID"));
	   if( tmpvar.vt==VT_NULL || tmpvar.fltVal <= 0 )
	   {
		  iSelSampleID = Cavphs->GetPhsStructSampleID();
		  if( iSelSampleID <= 0 )
		  {
			  sTmp.Format(_T("%d"),iSelSampleID);
				sTmp2.Format(GetResStr(IDS_SelSampleIDisIllegal),sTmp);
				ShowMessage(sTmp2);
				goto errH;
		  }
		  else
			 rstbl->PutCollect(_T("iSelSampleID"),_variant_t((long) iSelSampleID));
	   }
	   else
		  iSelSampleID = vtoi(rstbl->GetCollect(_T("iSelSampleID")));
		Cavphs->GetPhsStructFromSampleID(iSelSampleID);
		//������δ������λ���¶�֮ǰ
	   //�¾�CalSAPA����Ҫ�õ�dj,t0,pjg�Ȳ���
		//��ȡ�ܲ���������ͨ�ô���,������ɹ�,���˳�
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_variant_t v;
		float tmppjg=fabs(modPHScal::pjg);
	   //��ȡ�ܲ���������ͨ�ô���
	   if(vtos(rstbl->GetCollect(_T("dn1")))==_T(""))
		{
		   SQLx = _T("SELECT DISTINCT CustomID FROM PictureClipData IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + 
					_T("\' WHERE CustomID IN (SELECT DISTINCT CustomID FROM [") + modPHScal::tbnPA + _T("] IN \"\" [\; DATABASE=") 
								+ EDIBgbl::GetDBName(modPHScal::dbZDJcrude) + _T(" ;PWD=") + ModEncrypt::gstrDBZdjCrudePassWord + _T("] WHERE (Pmax >=") 
								+ ftos(tmppjg) + _T(" AND PictureClipData.ClassID<>") + ltos(iPAfixZ1) + _T(" AND PictureClipData.ClassID<>") + ltos(iPAfixZ2) 
								+ _T(" OR PictureClipData.ClassID=") + ltos(iPAfixZ1) + _T(" OR PictureClipData.ClassID=") + ltos(iPAfixZ2) + _T(") AND (Dw >= ") + ftos(modPHScal::dj* (1 - modPHScal::gnDW_delta * 0.01))
								+ _T(" AND Dw <= ") + ftos(modPHScal::dj* (1 + modPHScal::gnDW_delta * 0.01)) 
								+ _T(" AND Dw>0 OR Dw IS NULL OR Dw=0) AND tj >= ") + ftos(modPHScal::t0) + _T(")");
			if(rs->State)
				rs->Close();

			_CommandPtr cmd;
			cmd.CreateInstance(__uuidof(Command));
			cmd->ActiveConnection = modPHScal::dbZDJcrude;
			cmd->CommandText = (_bstr_t)SQLx;
			cmd->CommandType = adCmdText;
			
			AfxMessageBox(SQLx);
//			rs = cmd->Execute(NULL, NULL, adCmdText);
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
				adOpenKeyset, adLockOptimistic, adCmdText); 
			if(rs->adoEOF && rs->BOF)
			{
				//��ǰ��׼�Ҳ����ܲ�
			}
			else
			{
				rs->get_Collect((_variant_t)_T("CustomID"),&v);
				dn=vtos(v);
				rstbl->PutCollect(_T("dn1"),_variant_t(dn));
			}
		}	   
		else
		{
			//�ܲ�/����/���Ӽ����ִ�Сд
			dn = vtos(rstbl->GetCollect(_T("dn1")));
		}
	   //ȷ���ܲ�ͨ�ô��ţ��Ա������Ӧ���ֱ�׼��֧�������ͣ�
	   //����ͨ����
	   IDdn = sFindID(dn);
	   if(IDdn != _T("") )
		  UpdateTZB4zdjh(_T("IDdn1"), _variant_t(IDdn));
	   else
	   {
		  IDdn = vtos(rstbl->GetCollect(_T("IDdn1")));
		  if( Trim(IDdn) != _T(""))
		  {
			 //���ı�淶�򳧼ҵ�ʱ����Ϊԭʼ�����е�dn�ֶλ�gn�ֶ�δ����Ӧ�仯�������ֲ��Ҳ���CustomID�Ĵ���
			 //�˴���ͼ�������ִ���
			 dn = sFindCustomID(IDdn);
			 UpdateTZB4zdjh(_T("dn1"), _variant_t(dn));
		  }
		  else
		  {
			 IDdn = _T("");
			 //MsgBox _T("δ�ҵ�ͨ�ùܲ�����!�����¶���ͨ�ô���֮��������")
			 throw GetResStr(IDS_NotFoundGeneralPAID);
		  }
	   }
	   
	   //��ȡ������������ͨ�ô���
	   if(vtos(rstbl->GetCollect(_T("gn1")))==_T(""))
		{		   
		   int Gnum;			
			Gnum= (modPHScal::glNumSA!=0 ? modPHScal::glNumSA : 1);
			SQLx = _T("SELECT CustomID FROM PictureClipData IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'")
								+ _T(" WHERE EXISTS ( Select CustomID FROM [") + modPHScal::tbnSA + _T("] IN \"\" [; DATABASE=") 
								+ EDIBgbl::GetDBName(modPHScal::dbZDJcrude) + _T(" ;PWD=") + ModEncrypt::gstrDBZdjCrudePassWord + _T("] WHERE PictureClipData.CustomID = CustomID AND (PictureClipData.ClassID= ") + ltos(iG100) + _T(" OR PMAXH >=") 
								+ ftos(tmppjg / Gnum * (vtob(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("ifLongVertPipe"))) ? Gnum : 1))
								+ _T(" AND (( PictureClipData.ClassID = ")
								+ ltos(iSACantilever) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbrace) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbraceFixG47) + _T(" OR PictureClipData.ClassID = ") + ltos(iSALbraceFixG48) + _T(" OR PictureClipData.ClassID = ") + ltos(iG51) + _T(" OR PictureClipData.ClassID = ") + ltos(iG56) + _T(" OR PictureClipData.ClassID = ") + ltos(iG57)  + _T(") AND GDW1 >=")
								+ ftos(modPHScal::gdw1) + _T(" OR ( PictureClipData.ClassID =") + ltos(iSAbeam) +_T(" OR PictureClipData.ClassID =") + ltos(iG52_55) + _T(" ) AND GDW1 >=")
								+ _T(" IIF(")+ ftos(modPHScal::gdw1+(modPHScal::glNumSA==1 ? modPHScal::DblHangerRodDist : 0)/2) +_T(" > L1/2 , L1 - ") + ftos(modPHScal::gdw1+(modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2)
								+ _T(" , ")+ ftos(modPHScal::gdw1+(modPHScal::glNumSA == 1 ? modPHScal::DblHangerRodDist : 0) / 2) + _T(") AND L1> ") + ftos(modPHScal::Lspan)
								+ _T("  OR PictureClipData.ClassID= ") + ltos(iGCement) + _T(")) )");
			if(rs->State == adStateOpen)
				rs->Close();
			rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)modPHScal::dbZDJcrude,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			if(rs->adoEOF && rs->BOF)
			{
				//��ǰ��׼�Ҳ�������
			}
			else
			{
				rs->get_Collect((_variant_t)_T("CustomID"),&v);
				gn=vtos(v);
				rstbl->PutCollect(_T("gn1"),_variant_t(gn));
			}
		}	   
		else
		{
			//�ܲ�/����/���Ӽ����ִ�Сд
			gn = vtos(rstbl->GetCollect(_T("gn1")));
		}
	   //ȷ������ͨ�ô��ţ��Ա������Ӧ���ֱ�׼��֧�������ͣ�
	   //����ͨ����
	   IDgn = sFindID(gn);
	   if(IDgn != _T("") )
		  UpdateTZB4zdjh(_T("IDgn1"), _variant_t(IDgn));
	   else
	   {
		  IDgn = vtos(rstbl->GetCollect(_T("IDgn1")));
		  if( Trim(IDgn) != _T(""))
		  {
			 //���ı�淶�򳧼ҵ�ʱ����Ϊԭʼ�����е�gn�ֶλ�gn�ֶ�δ����Ӧ�仯�������ֲ��Ҳ���CustomID�Ĵ���
			 //�˴���ͼ�������ִ���
			 gn = sFindCustomID(IDgn);
			 UpdateTZB4zdjh(_T("gn1"), _variant_t(gn));
		  }
		  else
		  {
			 IDgn = _T("");
		  }
	   }


	   if( fabs(yr) > gUseSpringMinDist )
	   {
		  if( IsNull(rstbl->GetCollect(_T("paz1"))) || rstbl->GetCollect(_T("paz1")).fltVal == 0 )
		  {
			rstbl->PutCollect(_T("paz1"),_variant_t((float)((1 - gnSPR_CHZBH) * rstbl->GetCollect(_T("pgz1")).fltVal)));
		  }
		  paz = rstbl->GetCollect(_T("paz1")).fltVal * UTmp;
      
		  if( pgz != 0.0 )
		  {
		  }
		  else
		  {
			  sTmp2.Format(_T("%d"),zdjh);
			  sTmp.Format(GetResStr(IDS_ZDJNoXOperatingLoadIsZero),sTmp2);
			  ShowMessage(sTmp);
			  goto errH;
		  }
	   }
	   else
	   {
		  //�������֧�������ͺ�����
		  lngErrNum = CalRigidZDJType(rstbl);
		  //���ڸ���֧���ܰ�װ���ز�һ�����ڹ�������
	   }
	   //�ٴμ���֧���ܵ�����
	   lngErrNum = CalRigidZDJType(rstbl);
	   sTmp=vtos(rstbl->GetCollect(_T("type")));
	   if( sTmp== _T("5000") || sTmp == _T("9999") )
	   {
		  if( IsNull(rstbl->GetCollect(_T("paz1"))) || rstbl->GetCollect(_T("paz1")).fltVal!= rstbl->GetCollect(_T("pgz1")).fltVal )
		  {
			 rstbl->PutCollect(_T("paz1"),rstbl->GetCollect(_T("pgz1")));
		  }
	   }
	   else
	   {
		  tmpvar=rstbl->GetCollect(_T("type"));
		  if( ! tmpvar.vt!=VT_NULL )
		  {
			  sTmp=vtos(tmpvar);
			  sTmp.TrimLeft();sTmp.TrimRight();
			 if( sTmp.GetLength()< 4 )
			 {
				//���ǵ���֧����
				 //���ڸ���֧���ܰ�װ���ز�һ�����ڹ�������
			 }
		  }
	   }
	   tmpvar=rstbl->GetCollect(_T("t01"));
	   if( tmpvar.vt==VT_NULL )//|| (float)tmpvar == 0 )
	   {
		  rstbl->PutCollect(_T("t01"),_variant_t((float)0.0));
	   }
   
	   //ͼ���ļ�ǰ׺��ʽ
	   if( glDWGPreMethod == 0 )
	   {
		   clb = EDIBAcad::GetDrawIDAndName(zdjh + glDrawNoStartNumber - 1, EDIBDB::DrawName);
	   }
	   else
	   {
		   sTmp.Format(_T("%2d"),zdjh + glDrawNoStartNumber - 1);
			clb = gsDWGPre+sTmp;
	   }
   
		tmpvar=rstbl->GetCollect(_T("dg1"));
	   if( tmpvar.vt==VT_NULL || vtos(tmpvar)==_T(""))
	   {
		  rstbl->PutCollect(_T("dg1"),_T("G"));//: rstbl.Update
	   }
      
	   //�ܲ������ƫװ
	   //�����������
		tbn = sFindTBN(dn);
		ZdjType=modPHScal::ZdjType;
		if( tbn == _T(""))
		{
			sTmp.Format(GetResStr(IDS_CantHandlePA),vtos(rstbl->GetCollect(_T("dn1"))));
			return;//throw sTmp;//ԭ����"throw sTmp;" ��Ϊ: "return;" pfg2006.12.20
		}
		else
		{
			if( glClassID == iPAh || glClassID == iPAs)
			{
				if( fabs(yr) > gUseSpringMinDist )
				{
					if( ZdjType == _T("9999") );
						//����
					else
					{
						if (Trim(ZdjType).GetLength() == 4 )
						{
							//���ɵ���
							//���봫�ݰ�װѹ��ֵ�����������򵯻�����İ�װѹ��ֵ=0
							sngSpringWeightNormal(ZdjType, rstbl, vtof(rstbl->GetCollect(_T("haz"))));
						}
					}
				}
				else
				{
					//����֧����
					giWholeSprNum = 0;
					ZdjType=_T("");
				}					
			}
			else
			{
				//�̶�֧�ܻ����ǵ���֧��
				giWholeSprNum = 0;
				ZdjType=_T("");
			}
		}
    
	   tmpvar=rstbl->GetCollect(_T("LC1"));
	   if( tmpvar.vt==VT_NULL || tmpvar.fltVal <= 0.0 )
	   {
		  rstbl->PutCollect(_T("LC1"),_variant_t((float) 1800));
	   }
	   Lspan = rstbl->GetCollect(_T("LC1")).fltVal;  //����L1ֵ=���
	   tmpvar=rstbl->GetCollect(_T("b01"));
	   if( tmpvar.vt==VT_NULL || tmpvar.fltVal <= 0.0 )
	   {
		  rstbl->PutCollect(_T("b01"),_variant_t((float) 300));
	   }
	   WidthB = rstbl->GetCollect(_T("b01")).fltVal;   //�������ӿ��Bֵ
	   tmpvar=rstbl->GetCollect(_T("A01"));
	   if( tmpvar.vt==VT_NULL || tmpvar.fltVal <= 0 )
		  rstbl->PutCollect(_T("A01"),_variant_t((float) 300));
   
	   WidthA = rstbl->GetCollect(_T("A01")).fltVal;   //�������ӿ��Aֵ�����߶�H(mm)
   
		//����ƫװ,��Ϊ���е�gdw1Ҫ���������
		CalOffset(rstbl);
		//gdw��������λ��ƫװ��������GetPhsBHandSizes�и���ƫװ��ĸ�����λֵ���
   }
   
	catch(_com_error &e)
	{
		ShowMessage(e.Description());
	}
	catch(CString &err)
	{
		ShowMessage(err);
	}
errH: ;
}

float modPHScal::sngSpringWeight(CString /*ByVal*/ strZdjType, _RecordsetPtr rstbl, float /*Optional*/ mvDist, float& /*Optional*/ sngTotalHeight)
{
//Ŀ��:���㴮����ΪiSerialNum�ĵ������������Ա������ѡ��
//���γ��򲻴ӵ���ԭʼ���ݱ��鵯�ɴ������Ƿ�����
//����:֧��������,4���ַ���strZdjType
//����:����������
		float ret=0;
		CString sTmp;
		CString strSprNo;
		CString SQLx;
		float sngWeight=0.0,sngHeight=0.0;
		int iSumSerialNum=0;
		int i=0;
//		float haz0,haz1;
		_RecordsetPtr rs;
		rs.CreateInstance(__uuidof(Recordset));
		_variant_t v;
	try
	{	
		//��ȡ���ɺţ�����֧���ܺ�Ϊ1208,�򵯻ɺ�strSprNo=08.
	   strSprNo = strZdjType.Mid(2);
   
	   iSumSerialNum = _ttoi(strZdjType.Left(1));
	   giSumSerialNum = iSumSerialNum;
	   if( (iSumSerialNum % giAllowedMaxSerialNumPerSPR) != 0 ){
		  giWholeSprNum = iSumSerialNum / giAllowedMaxSerialNumPerSPR + 1;
	   }else{
		  giWholeSprNum = iSumSerialNum / giAllowedMaxSerialNumPerSPR;
	   }
	   //Rsx(_T("PreCmprs"))=���ɵ�Ԥѹ��ֵ
	   //ÿ�����ɰ�װʱ�ľ���ѹ��ֵhaz0=K*Pgz+yr/iSumSerialNum
	   //ÿ�����ɰ�װʱ�����ѹ��ֵhaz1=K*Pgz+yr/iSumSerialNum-rsx.fields(_T("PreCmprs"))
	   //K--����ϵ��,mm/kgf;
	   //Pgz--��������,kgf;
	   //yr--��λ��,��������Ϊ+,����Ϊ-,mm.
	   //�ܸ߶�
	   sngTotalHeight = 0;
	   for( i = 0 ;i<giWholeSprNum ;i++)
	   {
		  if( i == giWholeSprNum - 1 && (iSumSerialNum % giAllowedMaxSerialNumPerSPR != 0) )
		  {
			 //���һ�鵯�ɵĴ�����
			 sSprInfo[i].SerialNum = iSumSerialNum % giAllowedMaxSerialNumPerSPR;
		  }
		  else
		  {
			 sSprInfo[i].SerialNum = giAllowedMaxSerialNumPerSPR;
		  }
		  sSprInfo[i].SprNo = _ttoi(strSprNo);
		  sSprInfo[i].DH = sSprInfo[i].SerialNum * 100 + sSprInfo[i].SprNo;
		  sSprInfo[i].haz = mvDist / iSumSerialNum * sSprInfo[i].SerialNum;
		  sSprInfo[i].HeatDisp = yr / iSumSerialNum * sSprInfo[i].SerialNum;
		  //���²�������,�ٶ�����T1������ڽ�����Ϊ����Ƚ��ǿ��еġ�
		  sTmp.Format(_T("%d"),sSprInfo[i].DH);
		  SQLx = _T("SELECT * FROM [") + tbnSPRINGCrude + _T("] WHERE DH=") + sTmp + _T(" And (CustomID)=\'") + sFindCustomID(_T("T1")) + _T("\'");
		  if(rs->State == adStateOpen)
			  rs->Close();
		  rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
			  adOpenStatic, adLockOptimistic, adCmdText); 
		  if( rs->BOF && rs->adoEOF ){
			 sngWeight = 0;
			 sngHeight = 0;
		  }
		  else
		  {
			 rs->MoveFirst();
			rs->get_Collect((_variant_t)_T("weight"),&v);
			 if(v.vt==VT_NULL){
				sngWeight = 0;
				sngHeight = 0;
			 }else{
				 rs->get_Collect((_variant_t)_T("weight"),&v);
				 sngWeight=vtof(v);
				 rs->get_Collect((_variant_t)_T("sizeH"),&v);
				sngHeight = vtof(v);
			 }
		  }
		  //����ͳ�ƺ�*******
		  ret += sngWeight;
		  sngTotalHeight = sngTotalHeight + sngHeight;
	   }
	   return ret;
	  }
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	return ret;
}

float modPHScal::sngSpringWeightNormal(CString /*ByVal*/ strZdjType, _RecordsetPtr rstbl, float mvDist, float sngTotalHeight)
{
	//Ŀ��:���㴮����ΪiSerialNum�ĵ������������Ա������ѡ��
	//���γ���ӵ���ԭʼ���ݱ��鵯�ɴ������Ƿ�����
	//����:֧��������,4���ַ���strZdjType
	//����:����ѡ����ͬ�ĵ��ɣ�����ѡ�����ٵĵ��ɡ�����414=2��214��������<>314��114��514=314��214��
	//����:����������
	 //CString *Ptype=NULL;   //��¼֧���ܽṹ���������������
	 //long *mlPartClassID=NULL;   //��¼����������
	 //long *mlPartIndex=NULL;     //��¼��������
	 float ret=0.0;
	try
	{
		 bool bFound;   //�ҵ����㴮�����ĵ���
		 float sngWeight=0.0 , sngHeight=0.0;
		 long k;  //��ʱ��������Ƭ������
		 long i; //��ʱ�����������������
		 CString mstrCurrentPart;    //��ǰ���ID
   
		 long iSumSerialNum ;CString strSprNo ;float  haz0=0.0 , haz1=0.0;
		 iSumSerialNum=0;
		 _RecordsetPtr rs;
		 rs.CreateInstance(__uuidof(Recordset));
		 _RecordsetPtr rs1;
		 rs1.CreateInstance(__uuidof(Recordset));
		 _RecordsetPtr rsw;
		 rsw.CreateInstance(__uuidof(Recordset));
		 _variant_t v;
		//���浱ǰ֧������Ͻṹ���ݣ��������������ڻ�ͼ��
		 long lngLastPartNo;  //��¼����֧���ܵ������������������
		i=0;
		k=0;
		bFound=false;
		lngLastPartNo=0;
		iSelSampleID = vtoi(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("iSelSampleID")));
		//����ĺ��������˵��ɵ�CustomID��
		giWholeSprNum = Cavphs->GetPhsIsCSPR(iSPR, iSelSampleID);
		//ReDim sSprInfo(giWholeSprNum - 1)
   
		strSprNo = strZdjType.Mid(2);
		CString SQLx,sTmp;
		_variant_t vTmp;
		giSumSerialNum = _ttoi(strZdjType.Left(1));
		if( giSumSerialNum > giAllowedMaxSerialNumSPRs ){
			ShowMessage(GetResStr(IDS_OverAllowedMaxSerialNumSPRs));
		}
		else
		{
			//���ɴ�����������Χ��
			while(true)
			{
				if( i + 1 > giWholeSprNum ){
					sTmp.Format(GetResStr(IDS_NeedSprNumMoreSprNumOfSampleID),ltos(i + 1),ltos(giWholeSprNum));
					//throw sTmp;
					return ret;
				}
				SQLx = _T("SELECT * FROM [") + tbnSPRINGCrude + _T("] WHERE (CustomID)=\'") + sSprInfo[i].CustomID + _T("\' AND (dh mod 100)=") + strSprNo + _T(" ORDER BY DH DESC");
				rs1->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
					adOpenStatic, adLockOptimistic, adCmdText); 
				//if(!rs1->adoEOF && !rs1->BOF)
				//{
				rs1->get_Collect((_variant_t)_T("DH"),&vTmp);
				if( (i == 0? giSumSerialNum: k) > ( vtoi(vTmp) / 100 )){
					//��������Ҫ����
					if( (giSumSerialNum % 2) == 0 ){
						//ż��������ʹ��������ͬ
						k = giSumSerialNum / 2;
					}else{
						//����������ʹ����������
						k = giSumSerialNum - iSumSerialNum;
					}
				}else{
					k = giSumSerialNum - iSumSerialNum;
				}
				SQLx = _T("SELECT * FROM [") + tbnSPRINGCrude + _T("] WHERE (CustomID)=\'") + sSprInfo[i].CustomID + _T("\' AND (dh mod 100)=") + strSprNo + _T(" AND int(dh/100)<=") + ltos(k) + _T(" ORDER BY DH DESC");
				rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
					adOpenStatic, adLockOptimistic, adCmdText); 
				//ֻ�д�ʵ�ʵ����ݿ��в����б�������ɵ��������,���ݱ����ֶ�DH=������ *100+���ɺ�
				if( rs->BOF && rs->adoEOF ){
					//���ݱ�û�����ֵ��ɵ�����
				}
				else
				{
					//���ݱ������ֵ��ɵ�����
					rs->MoveFirst();
					rs->get_Collect((_variant_t)_T("DH"),&vTmp);
					sSprInfo[i].SerialNum = vtoi(vTmp) / 100;
					iSumSerialNum = iSumSerialNum + sSprInfo[i].SerialNum;
					if( iSumSerialNum >= giSumSerialNum ){
						//����������Ҫ��
						giWholeSprNum = i + 1;
						bFound = true;
						break;
					}
					//else{
						//������������Ҫ��
						//����ѭ��
					//}
         
				}
				i = i + 1;
				rs->Close();
				rs1->Close();
			}
			if( bFound )
			{
				//Rsx(_T("PreCmprs"))=���ɵ�Ԥѹ��ֵ
				//ÿ�����ɰ�װʱ�ľ���ѹ��ֵhaz0=K*Pgz+yr/iSumSerialNum
				//ÿ�����ɰ�װʱ�����ѹ��ֵhaz1=K*Pgz+yr/iSumSerialNum-rsx.fields(_T("PreCmprs"))
				//K--����ϵ��,mm/kgf;
				//Pgz--��������,kgf;
				//yr--��λ��,��������Ϊ+,����Ϊ-,mm.
				ret = 0.0;
				//�ܸ߶�
				sngTotalHeight = 0;
				for( i = 0 ;i<giWholeSprNum ;i++)
				{
					sSprInfo[i].SprNo = _ttoi(strSprNo);
					sSprInfo[i].DH = sSprInfo[i].SerialNum * 100 + sSprInfo[i].SprNo;
					sSprInfo[i].haz = mvDist / giSumSerialNum * sSprInfo[i].SerialNum;
					sSprInfo[i].HeatDisp = yr / giSumSerialNum * sSprInfo[i].SerialNum;
					//���²��ҵ���Ԥѹ��ֵ������PhsYLBMake�����������ѹ��ֵ��
				  SQLx = _T("SELECT * FROM [") + tbnSPRINGproperty + _T("] WHERE SEQ=") + strSprNo;// + _T(" ORDER BY seq");
				  if(rs->State == adStateOpen)
					  rs->Close();
				  rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
					  adOpenStatic, adLockOptimistic, adCmdText); 
				  if( rs->BOF && rs->adoEOF )
				  {
					  //û�з������ֵ���,��ʱԤ��Ϊ0
					  sSprInfo[i].Hpcmprs=0;
				  }
				  else
				  {
					  //�������ֵ���
					  rs->get_Collect((_variant_t)_T("PreCmprs"),&v);
					  sSprInfo[i].Hpcmprs=sSprInfo[i].SerialNum*vtoi(v);
				  }
				  
					//���²�������,�ٶ�����T1������ڽ�����Ϊ����Ƚ��ǿ��еġ�
					SQLx = _T("SELECT * FROM [") + tbnSPRINGCrude + _T("] WHERE DH=") + ltos(sSprInfo[i].DH) + _T(" AND (CustomID)=\'") + sSprInfo[i].CustomID + _T("\'");
					rsw->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
						adOpenStatic, adLockOptimistic, adCmdText); 
					if( rsw->BOF && rsw->adoEOF ){
						sngWeight = 0;
						sngHeight = 0;
					}
					else
					{
						rsw->MoveFirst();
						rsw->get_Collect((_variant_t)_T("weight"),&vTmp);
						if( vTmp.vt==VT_NULL){
							sngWeight = 0;
							sngHeight = 0;
						}else{
							rsw->get_Collect((_variant_t)_T("weight"),&vTmp);
							sngWeight = vtof(vTmp);
							rsw->get_Collect((_variant_t)_T("sizeH"),&vTmp);
							sngHeight =vtof(vTmp);
						}
					}
					//����ͳ�ƺ�*******
					ret +=sngWeight;
					sngTotalHeight = sngTotalHeight + sngHeight;
					rsw->Close();
				}
			}
		}
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
	return ret;
}

long modPHScal::CalRigidZDJType(_RecordsetPtr rstbl)
{
//Ŀ��:�������֧���ܵ�typeֵ����������֧���ܵ�ͼ������
//����:�ɹ���0��ZdjType=֧��������ֵ;ʧ�ܣ������롣

	long iNumSPR,iNumCSPR;
   //ʹ��ѭ����ȷ����Ҫ�ĺ������ɴ�����,�����������
   //���浱ǰ֧������Ͻṹ���ݣ��������������ڻ�ͼ��
	try
	{
	   iSelSampleID =vtoi(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("iSelSampleID")));
	   //����ĺ��������˵��ɵ�CustomID��
	   iNumSPR = Cavphs->GetPhsIsCSPR(iSPR, iSelSampleID);
	   iNumCSPR = Cavphs->GetPhsIsCSPR(iCSPR, iSelSampleID);
   
	//Public Enum ZdjTxNameIndex
	 //  iZdjSPRh
	  // iZdjCSPRh
	   //iZdjRigidH
	//   iZdjGuide
	 //  iZdjSlide
	  // iZDJfix
	   //iZdjSprGuide
	//End Enum
		CString sTmp; 
		CString tbn;
	//   long lngErrNum;
		tbn = sFindTBN(dn);
		if( tbn == _T(""))
		{
			sTmp.Format(IDS_CantHandlePA,dn);
			//ShowMessage(sTmp); //ԭ���� pfg2006.12.20
			return IDS_CantHandlePA;
		}
		if( sngPEL < sngSEL)
		{
			 //����
			 if( iNumCSPR > 0)
				//��������
				gsPhsName = ZdjTxName[iZdjCSPRh];
			 else
			 {
				 if( iNumSPR > 0 ||(iNumSPR==0&&fabs(modPHScal::yr)>=modPHScal::gUseSpringMinDist&&modPHScal::yr>0))
				   //����
				   gsPhsName = ZdjTxName[iZdjSPRh];
				else
				{
				   //����
					if(glClassID== iPAh)
					{
						//ZdjType = _T("120");
						gsPhsName = ZdjTxName[iZdjRigidH];
						//rstbl->PutCollect(_T("type"),_variant_t(ZdjType));
					}
					else
						//δ����
						gsPhsName = ZdjTxName[iZdjRigidH];				   
				}
            
			 }
		}
		else
		{
			 //֧��
			 if( iNumCSPR > 0 )
			 {
				 //��������
				 gsPhsName = ZdjTxName[iZdjCSPRh];
			 }
			 else
			 {
				//�Ǻ�������
				if( iNumSPR > 0||(iNumSPR==0&&fabs(modPHScal::yr)>=modPHScal::gUseSpringMinDist&&modPHScal::yr>0))
				   //����
				   gsPhsName = ZdjTxName[iZdjSprGuide];
				else
				{
				   //����
				   //if( IsNull(rstbl.Fields(_T("type"))) || Len(ZdjType) <> 3 Then
					  switch(glClassID)
					  {
					  case iPAfixZ1:
					  case iPAfixZ2:
							ZdjType = _T("177");
							gsPhsName = ZdjTxName[iZDJfix];
							break;
					  case iPAh:
							ZdjType = _T("120");
							gsPhsName = ZdjTxName[iZdjRigidH];
							break;
					  case iPAs:
							//��������GLif(V1.2~3.x
							if( df.Right(1) == _T("X"))
							   ZdjType = _T("130");
							else
							   ZdjType = _T("110");
                        
							gsPhsName = ZdjTxName[iZDJfix];
							break;
					  default:
							ZdjType = _T("");
							gsPhsName = ZdjTxName[iZdjSupport];
							//Err.Raise iUE_CantHandlePA, , ResolveResString(iUE_CantHandlePA, _T("|1"), dn)
					}
					rstbl->PutCollect(_T("type"),_variant_t(ZdjType));
				}
			}
		} 
		SetRegValue(_T("Settings"),_T("PhsCurZDJName"), gsPhsName);    //zsy 05/1/4
		return 0;
	}
	catch(_com_error &e)
	{
		ShowMessage(e.Description());
		return IDS_CantHandlePA;
	}
}

void modPHScal::CalfxSA(_RecordsetPtr rstbl)
{
//Ŀ�ģ�ȷ����������
   try
   {
	   if(rstbl->GetCollect(_variant_t(_T("gf1"))).vt==VT_NULL)
		   //UpdateTZB4zdjh(_T("gf1"),_T("PX"));
		   rstbl->PutCollect(_variant_t(_T("gf1")),_variant_t(_T("PX")));
	   else
	   {
		   gf1 = vtos(rstbl->GetCollect(_variant_t(_T("gf1"))));
		   gf1.TrimLeft();gf1.TrimRight();
		   gf1.MakeUpper();
		   if(gf1.Left(1)==_T("P") || gf1.Left(1)==_T("O"))
		   {
			   if(gf1.GetLength()!=2)
				   //UpdateTZB4zdjh(_T("gf1"), _T("PX"));
				   rstbl->PutCollect(_variant_t(_T("gf1")),_variant_t(_T("PX")));
			   else if(gf1.Mid(1,1)!=_T("X") && gf1.Mid(1,1)!=_T("Z"))
				   //UpdateTZB4zdjh(_T("gf1"),_T("PX"));
				   rstbl->PutCollect(_variant_t(_T("gf1")),_variant_t(_T("PX")));
		   }
		   else
			   rstbl->PutCollect(_variant_t(_T("gf1")),_variant_t(_T("PX")));
	   }
	   gf1 = vtos(rstbl->GetCollect(_variant_t(_T("gf1"))));
	   gf1.TrimLeft();gf1.TrimRight();
	   gf1.MakeUpper();
	   CalSpringParallelNum();
	   //˫·֧���ܹ���һ������ʱ�����������ܵ�������������ơ�
	   if(rstbl->GetCollect(_variant_t(_T("Gnum"))).vt!=VT_NULL && (long)rstbl->GetCollect(_variant_t(_T("Gnum")))==1 && gintParallelNum==2)
	   {
		   if( df == _T("X"))
		   {
			   if(gf1 != _T("PZ") && gf1 != _T("OZ") )
				   gf1 = _T("PZ");
		   }
		   else
		   {
			   if(gf1 != _T("PX") && gf1 != _T("OX"))
				   gf1 = _T("PX");
		   }
		   //rstbl.Edit: rstbl.Fields(_T("gf1")) = gf1: rstbl.Update
		   rstbl->PutCollect(_variant_t(_T("gf1")),_variant_t(gf1));
		   //UpdateTZB4zdjh(_T("gf1"), gf1);
	   }
	   gf1 = vtos(rstbl->GetCollect(_variant_t(_T("gf1"))));
	   gf1.TrimLeft();gf1.TrimRight();
	   gf1.MakeUpper();     //��������
	   //����gf1��4�ַ���,����ʱ�뷽����ת,
	   //����ΪPX,OZ,OX,PZ
	   //�жϸ�������
	   fx=gf1.Left(1);
	   //fx:_T("P"),_T("O")
	   if(gf1.Right(1)==_T("X"))
		   gf=_T("X");
	   else
		   gf=_T("Z");
   }
   catch(_com_error &e)
   {
	   ShowMessage(e.Description());
   }
}

void modPHScal::CalfxPA(_RecordsetPtr rstbl)
{
//Ŀ�ģ�ȷ���ܲ�����
	CString sTmp;
	if(rstbl->GetCollect(_variant_t(_T("df1"))).vt==VT_NULL)
	{
		rstbl->PutCollect(_variant_t(_T("df1")),_variant_t(_T("X")));
		df=_T("X");
	}
	else
	{
		sTmp=vtos(rstbl->GetCollect(_variant_t(_T("df1"))));
		sTmp.TrimLeft();sTmp.TrimRight();
		sTmp.MakeUpper();
		if(sTmp==_T(""))
		{
			rstbl->PutCollect(_variant_t(_T("df1")),_variant_t(_T("X")));
			df=_T("X");
		}
		else
			df=sTmp;
		//�ܲ�ֻ��2�ַ���,��X || Z
		//ʵ����Z11��Z13Ҳ��OX����
		if(df!=_T("X") && df!=_T("-X") && df!=_T("Z") && df!=_T("-Z"))
		{
			rstbl->PutCollect(_variant_t(_T("df1")),_variant_t(_T("X")));
			df=_T("X");
		}
	}
}

void modPHScal::CalDG(_RecordsetPtr rstbl)
{
}

void modPHScal::CalUnitX(_RecordsetPtr rstbl)
{
 //Ŀ��:���ԭʼ���ݱ��еĵ�λ��
	CString sTmp;
   if (rstbl->GetCollect(_variant_t(_T("Unit_Force"))).vt==VT_NULL)
      rstbl->PutCollect(_variant_t(_T("Unit_Force")),_variant_t(_T("kgf")));
   else
   {
		sTmp=vtos(rstbl->GetCollect(_variant_t(_T("Unit_Force"))));
		sTmp.TrimLeft();sTmp.TrimRight();
		if(sTmp==_T(""))
			rstbl->PutCollect(_variant_t(_T("Unit_Force")),_variant_t(_T("kgf")));
		else
		{
			if(sTmp!= _T("kgf") && sTmp!= _T("N"))
			{
				rstbl->PutCollect(_variant_t(_T("Unit_Force")),_variant_t(_T("kgf")));
			}
         //MsgBox ResolveResString(iUE_xUnitIsIllegal, _T("|1"), ResolveResString(iUE_UNIT_FORCE), _T("|2"), LCase(Trim(rstbl.Fields(_T("Unit_Force"))))), vbMsgBoxSetForeground
		}
   }
   if (rstbl->GetCollect(_variant_t(_T("Unit_Displacement"))).vt==VT_NULL)
      rstbl->PutCollect(_variant_t(_T("Unit_Displacement")),_variant_t(_T("mm")));
   if(true)
   {
		sTmp=vtos(rstbl->GetCollect(_variant_t(_T("Unit_Displacement"))));
		sTmp.TrimLeft();sTmp.TrimRight();
		sTmp.MakeLower();
		if(sTmp==_T(""))
			rstbl->PutCollect(_variant_t(_T("Unit_Displacement")),_variant_t(_T("mm")));
		else
		{
			if(sTmp!= _T("mm"))
			{
				rstbl->PutCollect(_variant_t(_T("Unit_Displacement")),_variant_t(_T("mm")));
			}
         //MsgBox ResolveResString(iUE_xUnitIsIllegal, _T("|1"), ResolveResString(iUE_UNIT_FORCE), _T("|2"), LCase(Trim(rstbl.Fields(_T("Unit_Force"))))), vbMsgBoxSetForeground
		}
   }
   if (rstbl->GetCollect(_variant_t(_T("Unit_Momental"))).vt==VT_NULL)
      rstbl->PutCollect(_variant_t(_T("Unit_Momental")),_variant_t(_T("kgf.m")));
   if(true)
   {
		sTmp=vtos(rstbl->GetCollect(_variant_t(_T("Unit_Momental"))));
		sTmp.TrimLeft();sTmp.TrimRight();
		if(sTmp==_T(""))
			rstbl->PutCollect(_variant_t(_T("Unit_Momental")),_variant_t(_T("kgf.m")));
		else
		{
			if(sTmp!= _T("kgf.m") && sTmp!= _T("N*m") && sTmp!= _T("kgf*m") && sTmp!= _T("N.m"))
			{
				rstbl->PutCollect(_variant_t(_T("Unit_Momental")),_variant_t(_T("kgf.m")));
			}
         //MsgBox ResolveResString(iUE_xUnitIsIllegal, _T("|1"), ResolveResString(iUE_UNIT_FORCE), _T("|2"), LCase(Trim(rstbl.Fields(_T("Unit_Force"))))), vbMsgBoxSetForeground
		}
   }
   //�����λ�Ʊ�����δ��ֵ���򸳳�ʼֵ
   if (Unit_Force == _T("")) Unit_Force =vtos(rstbl->GetCollect(_variant_t(_T("Unit_Force"))));
   if (Unit_Displacement == _T("")) Unit_Displacement = vtos( rstbl->GetCollect(_variant_t(_T("Unit_Displacement"))));
   if (Unit_Momental == _T("")) Unit_Momental =vtos(rstbl->GetCollect(_variant_t(_T("Unit_Momental"))));
   //MsgBox Err.Description, vbMsgBoxSetForeground
}

float modPHScal::CalUnitCovertCoef(_RecordsetPtr rstbl, int iUNIT)
{
//Ŀ�ģ����㵥λת��ϵ����Ĭ��iUNIT=0����ת��
   //��λ
   CalUnitX( rstbl);
   CString sTmp;
   _variant_t tmpvar;
   switch(iUNIT)
   {
   case 0:
         //����λ����
	   sTmp=vtos(rstbl->GetCollect(_T("Unit_Force")));
	   sTmp.TrimLeft();sTmp.TrimRight();sTmp.MakeLower();
         if(sTmp== _T("kgf") )
            return 1;
         else
			 return 1 / EDIBgbl::kgf2N;
         
   case 1:
         //���ȵ�λ����
	   sTmp=vtos(rstbl->GetCollect(_T("Unit_Displacement")));
	   sTmp.TrimLeft();sTmp.TrimRight();sTmp.MakeLower();
         if (sTmp== _T("mm") )
            return 1;
         else
            return (float)EDIBgbl::cm2mm;
         
   case 2:
         //���ص�λ����
	   sTmp=vtos(rstbl->GetCollect(_T("Unit_Momental")));
	   sTmp.TrimLeft();sTmp.TrimRight();sTmp.MakeLower();
         if(sTmp==_T("kgf.m") )
            return  1;
         else
			 return 1 / EDIBgbl::kgf2N;
        
   }
	return 0;
}

void modPHScal::UpdateTZB4zdjh(CString FieldName,_variant_t FieldValue)
{
//Ŀ��:����TZB��
   //On Error GoTo errH
	try
	{
		CString sTmp,sFieldValue;
		sTmp.Format(_T("%d"),zdjh);
		FieldName.TrimLeft();FieldName.TrimRight();
		if(FrmTxsr.m_pViewTxsr->m_ActiveRs!=NULL && FrmTxsr.m_pViewTxsr->m_ActiveRs->State!=adStateClosed)
		{
			FrmTxsr.m_pViewTxsr->m_ActiveRs->PutCollect(_variant_t(FieldName),FieldValue);
			//FrmTxsr.m_pViewTxsr->m_ActiveRs->Update();
		}
		else
		{
			if(FieldValue.vt==VT_BSTR)
			{
				sFieldValue=_T("\'");
				sFieldValue+=vtos(FieldValue);
				sFieldValue+=_T("\'");
			}
			else sFieldValue=vtos(FieldValue);
			EDIBgbl::SQLx = CString(_T("UPDATE [")) + EDIBgbl::Btype[EDIBgbl::TZA] + _T("] SET ") 
				+ FieldName + _T("= ") +  sFieldValue +_T(" WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) 
				+ _T(" AND zdjh=") + sTmp;
			EDIBgbl::dbPRJDB->Execute((_bstr_t)EDIBgbl::SQLx, NULL, adCmdText);
		}
		//_variant_t tmpvar;
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

void modPHScal::VB_Cal(_RecordsetPtr rs, long zdjh,CFrmStatus &frmStatus,int nth)
{
	float fPgz,fPaz;//�������أ���װ����
	iRepZdjType=-1;
	//ÿ�μ���֮ǰ���뽫��ǰ֧�����������ֱ����Ϊ��Сֵ0����ֹ��������֧�������ֱ�����š�
	modPHScal::iMaxRodDia=0;
	modPHScal::g_fLHGWeight=0;
	int inthCount = 0;//·��������
	try
	{
		modPHScal::g_bPipeClampBoltDiameterChanged = FALSE;
		CString strSQL;
		CString strTmp;
		strSQL.Format(_T("SELECT COUNT(*) FROM [z1] WHERE VolumeID=%d AND zdjh=%d AND nth<>1"),
			EDIBgbl::SelVlmID,zdjh);
		_RecordsetPtr rsZ1=::conPRJDB->Execute(_bstr_t(strSQL),NULL,adCmdText);
		_RecordsetPtr rsTmpZB;
		rsTmpZB.CreateInstance(__uuidof(Recordset));
		inthCount = vtoi(rsZ1->GetCollect((long)0)); 
		if(inthCount >0)
		{
		}
		rsZ1->Close();
		if(inthCount == 0)
		{
			//����ʹ��AutoPHS���Ƶ�֧���ܣ�Ϊ������Z1���е�·����¼
			strSQL.Format((_T("SELECT count(*) FROM [Z1] WHERE VolumeID=%d AND ZDJH=%d AND [nth]=1")),
							EDIBgbl::SelVlmID,zdjh);
			rsZ1=::conPRJDB->Execute(_bstr_t(strSQL),NULL,adCmdText);
			if(vtoi(rsZ1->GetCollect((long)0)) <=0)
			{
				strSQL.Format((_T("INSERT INTO [Z1] (VolumeID,ZDJH,nth) VALUES(%d,%d,%d)")),
								EDIBgbl::SelVlmID,zdjh,1);
				conPRJDB->Execute(_bstr_t(strSQL),NULL,adCmdText);
			}

			strSQL.Format("SELECT * FROM [ZB] WHERE VolumeID = %d AND ZDJH = %d AND NTH = %d ",EDIBgbl::SelVlmID,modPHScal::zdjh,nth);
			rsTmpZB->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			if(rsTmpZB->BOF && rsTmpZB->adoEOF)
			{
			  Cavphs->SavephsAllStructToTZB(zdjh,nth);
			}
			rsTmpZB->Close();
		}

		if(inthCount > 0)
		{
			strSQL.Format("SELECT * FROM [ZB] WHERE VolumeID = %d AND ZDJH = %d AND NTH = %d ",EDIBgbl::SelVlmID ,modPHScal::zdjh, nth );
			rsTmpZB->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			if(rsTmpZB->BOF && rsTmpZB->adoEOF)
			{
				Cavphs->SavephsAllStructToTZB (zdjh,nth);
			}
			rsTmpZB->Close();
		}

		FrmTxsr.m_pViewTxsr->m_PagOption->UpdateData();

		short iCalSuccess=0;
		int iNumCSPR,iNumSPR,iNumSPROrCSPR,lngLastPartNo,i;
		static long iPreviousZDJH=-1;//ǰ�μ����֧���ܺ�
		static bool bFirst=false;//��ǰ֧���ܵ�һ�μ���
		bool bFoundLHG;//�����·���� 
		bFoundLHG=false;
	    //����Ӧ����
	    modPHScal::tbnAllowStress = _T("MechanicalOfMaterialSIGMAt");
		//�����ɽ���ĸֱ�����Զ���С���������ɽ���ĸֱ������С����ֱ��
		modPHScal::gbAutoSPRRodDiaDecrease=false;

		Cavphs->InitListRs();
		PreCalCrudeData(rs,nth);
		//PreCalCrudeData����λ���¾�֮ǰ������ pgz ��ֵ��lngPreCal �в�����Ҫ���� PreCalCrudeData
		
		  if (fabs(yr) > gUseSpringMinDist && pgz != 0 )
		  {
			 // dn = vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")));
	    	 // CalSpringParallelNum(dn); //���㲢����
			  CalSpringParallelNum(vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")))); //���㲢����
		      AutoSelSpringNo(rs,pgz/gintParallelNum,paz/gintParallelNum,yr,gnSPR_CHZBH,gsngTotalInstallCompressValue,iSelSampleID,
			     iRepZdjType,gSumDistOfSpring,bHintRepZdjType,gstrSpringOrder,ZdjType,gbAutoApplyChzbh,tbnSPRINGproperty,tbnDiscSPRINGpropertyMaxDist);
		  }
		  else			  
			iPreviousZDJH = zdjh;
   
	   //������̽��ƻ�sSprInfo()ȫ�����飬����������Ϣ���ƻ�����˱�����ڿ�ͷ��
	   CString strDeleteUserMaterial;
	   _RecordsetPtr rsTZB;
	   rsTZB.CreateInstance(__uuidof(Recordset));
	   strSQL=_T("SELECT zdjh FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND ZDJH=") + ltos(modPHScal::zdjh) + _T(" AND bUserAdd is not null AND bUserAdd=-1");
	   rsTZB->Open((_bstr_t)strSQL, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
		   adOpenDynamic, adLockOptimistic, adCmdText); 
		if(!(rsTZB->BOF && rsTZB->adoEOF))
		{
			if(AfxMessageBox(IDS_YESNODELETEUSERMATERIAL,MB_YESNO)==IDNO)
				strDeleteUserMaterial=_T(" AND ( bUserAdd is null OR bUserAdd=0) "); 
			else
			{
				while(!rsTZB->adoEOF)
				{
					rsTZB->Delete(adAffectCurrent);
					if(rsTZB->adoEOF )
					{	
						rsTZB->MoveLast();					
					}
				}
			}

			
		}
		rsTZB->Close();
		//�������ɻ������������,��Ҫ�����Ƿ��й�������L9/L10,�Ա�ȷ��ƫװֵ:
		//iScrollPartType=0/1/2(�޹�������/ֻ��L9/ֻ��L10)
	   iNumCSPR = Cavphs->GetPhsIsCSPR(iCSPR, iSelSampleID, lngLastPartNo);
		if(iNumCSPR>0)
		{
			iNumSPROrCSPR=iNumCSPR;
			gbPhsIsCSPR = true;
			gbPhsIsSPR = false;
		}
		else
		{
			iNumSPR = Cavphs->GetPhsIsCSPR(iSPR, iSelSampleID, lngLastPartNo);
			if(iNumSPR>0)
			{
				iNumSPROrCSPR=iNumSPR;
				gbPhsIsCSPR = false;
				gbPhsIsSPR = true;
			}
			else
			{
				iNumSPROrCSPR=0;
				gbPhsIsCSPR = false;
				gbPhsIsSPR = false;
			}
		}

	   if(PtypeDiameter!=NULL)
	   {
		   delete [] PtypeDiameter;
			PtypeDiameter=NULL;
	   }
	   PtypeIndexForDiameter=0;
	   PtypeDiameter=new int[lngLastPartNo];
	   g_itest = (int)PtypeDiameter;
	   PtypeIndexForDiameter=lngLastPartNo;
	   for(i=0 ; i<lngLastPartNo ; i++)
		   PtypeDiameter[i]=0;//������ֱ��Ϊ0����getphsBHandSizes�У�������˵�ֱ�����������������Ա����е��������Ӽ���Լ����
	   CString sTmp2,sTmp;
	   //����Ԥ�ȴ���


	   if(lngPreCal() == -1)
	   {
		   if( fabs(yr) > gUseSpringMinDist && pgz != 0 )
		   {
			  //dn = vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")));
	    	 // CalSpringParallelNum(dn); //���㲢����
			   CalSpringParallelNum(vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")))); //���㲢����
			   AutoSelSpringNo(rs,pgz/gintParallelNum,paz/gintParallelNum,yr,gnSPR_CHZBH,gsngTotalInstallCompressValue,iSelSampleID,
			       iRepZdjType,gSumDistOfSpring,bHintRepZdjType,gstrSpringOrder,ZdjType,gbAutoApplyChzbh,tbnSPRINGproperty,tbnDiscSPRINGpropertyMaxDist);
		   }

		  if( iSelSampleID > 0 )
		  {
			 //����GetphsBHandSizes��������һ��֧����֮ǰ��������dbPRJ�н�����tmpCSLen
			 MakeTmpCSLen();
			 //����GetphsBHandSizes��������һ��֧����֮ǰ��������dbPRJ�н�����tmpCalFixPhs
			 Cavphs->iChkCondition = iPJG + iTJ + iGDW1 + iCNT + iDW;   //���ü�������
         
			 iSelSampleID = vtoi(rs->GetCollect(_T("iSelSampleID")));
         			 if( iNumSPROrCSPR > 0 )
			 {
				giWholeSprNum = iNumSPROrCSPR;
				for( i = 0; i<SprInfoIndex;i++)
				{
				   if( sFindID(sSprInfo[i].CustomID) ==_T("LHG"))
					  bFoundLHG = true;
				}
				if(giCSPRindex!=NULL)
				{
					delete [] giCSPRindex;
					giCSPRindex=NULL;
				}
				giCSPRindex=new int[giWholeSprNum];				
				if( bFoundLHG)
				{
				   //��һ������ҳ��������ֱ�������ҽ�����ֱ������ȫ������PtypeDiameter()
				   iCalSuccess = Cavphs->GetphsBHandSizes(frmStatus,1, 3, nth,fPaz,fPgz);
				   //�ڶ�������ҳ���������ֱ����һ�����������⣬��Ҫ�ܵ�PtypeDiameter()��ֵԼ��
				   //����Ҫ�ѹܲ���������ۼӵ����ɼ�����أ���˵��ɿ���Ҫ����ѡ
				  // dn = vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")));
				   
	    	       CalSpringParallelNum(vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")))); //���㲢����
				   AutoSelSpringNo(rs,fPgz,fPaz,yr,gnSPR_CHZBH,gsngTotalInstallCompressValue,iSelSampleID,
					   iRepZdjType,gSumDistOfSpring,bHintRepZdjType,gstrSpringOrder,ZdjType,gbAutoApplyChzbh,tbnSPRINGproperty,tbnDiscSPRINGpropertyMaxDist);
				   iCalSuccess = Cavphs->GetphsBHandSizes(frmStatus,2, 3, nth,fPaz,fPgz);
				   //����������ҳ��·�������ɽ���ĸֱ������ֱ��=֮�����ӵ�����ֱ��
				   iCalSuccess = Cavphs->GetphsBHandSizes(frmStatus,3, 3, nth,fPaz,fPgz);
				}
				else
				{
				   //��һ������ҳ��������ֱ�������ҽ�����ֱ������ȫ������PtypeDiameter()
				   iCalSuccess = Cavphs->GetphsBHandSizes(frmStatus,1, 2, nth,fPaz,fPgz);
				   //�ڶ�������ҳ���������ֱ����һ�����������⣬��Ҫ�ܵ�PtypeDiameter()��ֵԼ��
				   //����Ҫ�ѹܲ���������ۼӵ����ɼ�����أ���˵��ɿ���Ҫ����ѡ
				   	//dn = vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")));
				   
	    	        CalSpringParallelNum(vtos(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("dn1")))); //���㲢����
				    AutoSelSpringNo(rs,fPgz,fPaz,yr,gnSPR_CHZBH,gsngTotalInstallCompressValue,iSelSampleID,
					   iRepZdjType,gSumDistOfSpring,bHintRepZdjType,gstrSpringOrder,ZdjType,gbAutoApplyChzbh,tbnSPRINGproperty,tbnDiscSPRINGpropertyMaxDist);
				   iCalSuccess = Cavphs->GetphsBHandSizes(frmStatus,2, 2, nth,fPaz,fPgz);
				}
				if(giCSPRindex!=NULL)
				{
					delete [] giCSPRindex;
					giCSPRindex=NULL;
				}
			 }
			 else
			 {
				//ֱ�������1��
				//�ڶ�������ҳ���������ֱ����һ�����������⣬��Ҫ�ܵ�PtypeDiameter()��ֵԼ��
				iCalSuccess = Cavphs->GetphsBHandSizes(frmStatus,1, 1, nth,fPaz,fPgz);
			 }
			 if( iCalSuccess!=false)
			 {
				if( bPAIsHanger())
				{
				   //����ǵ���
				   //������˨��ĸ���ţ���Ϊ��Щ��˨��ĸ���������ֱ��ȷ��֮ǰ�޷�ѡ��
				   Cavphs->GetBoltsNutsAndAttachmentsCLgg();
				   //�������˳���
				   if( CalRodLength(rs, zdjh))
				   {
					  MakeZDJ_ZD( zdjh);
					  PhsYLBMake(zdjh);   //����ĳ��֧���ܵ�һ��������
					  Cavphs->GetMaterial();
					  Cavphs->GetphsSEQ(rs);
					  //����ܲ���λֵ,�����ڼ�����sizeH�ֶ�֮��
					  Cavphs->GetPhsSAELandPAdxdydz();
					  CalOffset(rs);
					  frmStatus.UpdateStatus(1,true);
					  frmStatus.UpdateWindow();
					  _variant_t vSuccess;
					  vSuccess.ChangeType(VT_BOOL);
					  vSuccess.boolVal=(bool)iCalSuccess;
					  rs->PutCollect(_T("bCalSuccess"),vSuccess);
                  
					  //�ɹ�!  
					  if (! gbCalAllPhs)   
					  {
						  sTmp.Format(GetResStr(IDS_ZDJHphsCalculating),_T("\%d"),GetResStr(IDS_Success));
						  sTmp2.Format(sTmp,zdjh);
						  frmStatus.MessageBox(sTmp2);
					  }
				   }
				}
				else
				{
				   //�����֧��
					//���û�����ĸ������<=�ܲ���ߣ�������֧�ܴ����������ʱѡ���˵���ģ�壬����˨��ĸ���Ϲ��δ����������GetphsSEQ��������������ʾ��
				   //������˨��ĸ���ţ���Ϊ��Щ��˨��ĸ���������ֱ��ȷ��֮ǰ�޷�ѡ��
				   Cavphs->GetBoltsNutsAndAttachmentsCLgg();
				   MakeZDJ_ZD(zdjh);
				   PhsYLBMake(zdjh);   //����ĳ��֧���ܵ�һ��������
				   Cavphs->GetMaterial();
				   Cavphs->GetphsSEQ(rs);
				   //����ܲ���λֵ,�����ڼ�����sizeH�ֶ�֮��
				   Cavphs->GetPhsSAELandPAdxdydz();
				   frmStatus.UpdateStatus(1,true);
				   frmStatus.UpdateWindow();
				   _variant_t vSuccess;
					  vSuccess.ChangeType(VT_BOOL);
					  vSuccess.boolVal=(bool)iCalSuccess;
					  rs->PutCollect(_T("bCalSuccess"),vSuccess);
				   //�ɹ�!
				   if(!gbCalAllPhs)
				   {
					   sTmp.Format(GetResStr(IDS_ZDJHphsCalculating),_T("\%d"),GetResStr(IDS_Success));
						  sTmp2.Format(sTmp,zdjh);
						  frmStatus.MessageBox(sTmp2);
				   }
				}
			 }
			 else
			 {
				frmStatus.UpdateStatus(1,true);
				frmStatus.UpdateWindow();
				rs->PutCollect(_T("bCalSuccess"),_variant_t(false));
				//ʧ��!
				sTmp.Format(GetResStr(IDS_ZDJHphsCalculating),_T("\%d"),GetResStr(IDS_Failure));
				sTmp2.Format(sTmp,zdjh);
				frmStatus.MessageBox(sTmp2);
			 }
		  }
	   }
	   else
	   {
		   frmStatus.UpdateStatus(1,true);
		   frmStatus.UpdateWindow();
			rs->PutCollect(_T("bCalSuccess"),_variant_t(false));
    		//ʧ��!
			frmStatus.MessageBox(GetResStr(IDS_PreCalculationFailed));
			sTmp.Format(GetResStr(IDS_ZDJHphsCalculating),_T("\%d"),GetResStr(IDS_Failure));
			  sTmp2.Format(sTmp,zdjh);
			  frmStatus.MessageBox(sTmp2);
	   }
	   rs->PutCollect(_T("IsCal"),rs->GetCollect(_T("bCalSuccess")));
	   PhsMLmake(zdjh);    //��֧����ͼ��\ͼ��д��Ŀ¼��
   }
   catch (_com_error &e)
   {
	   CString strMsg;
	   strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
	   AfxMessageBox(strMsg);
		frmStatus.MessageBox(e.Description());
   }

   if ( g_itest != (int)PtypeDiameter )
	   AfxMessageBox ( "hello" );
   if(PtypeDiameter!=NULL)
   {
	   delete [] PtypeDiameter;
		PtypeDiameter=NULL;
   }
   iRepZdjType=-1;
}

bool modPHScal::CalRodLength(_RecordsetPtr  rstbl, long  zdjh)
{
//Ŀ�ģ��Զ�����ͷ������˳���
//ԭ��1������ʹ����󳤶ȵ����ˣ�Ҳ���Ǿ���ʹ����������;
      //2����������ȷ��ʱ��ʹÿ�ڵĳ��Ⱦ����ӽ�ƽ�����ȡ�
      //3���������������Ժ֧�����ֲ���ƹ�������˳���β��ȡ����
//���룺tbnLugInfo--���˳�����ƹ�������Ʊ�������ѡ��淶ʱ��ֵ
//���أ�ÿ�����˵ĳ���д��������ݱ�TZB��sizeDIM�ֶΣ�ͬʱ���㵥�ء�
	bool ret=false;
	CString *Ptype=NULL;   //��¼֧���ܽṹ���������������
	long *mlPartClassID=NULL;    //��¼����������
	long *mlPartIndex=NULL;     //��¼��������
	long *mlPartDia=NULL;     //��¼�����ֱ��
	long *mlPartRecno=NULL;     //��¼����ļ�¼��
	float (*mvActualLugExt)[2]=NULL;      //��¼�����ʵ�����˳��Ȳ���ֵ����ֵ������СֵAllowedMinLugExt��С�����ֵAllowedMaxLugExt
	float (*AllowedMinLugExt)[2]=NULL;    //��¼���Ӽ������������С����ֵ,���˲�������Ӽ��ĳ��Ȳ���С�ڴ�ֵ;���ڴ���������ܲ�������,ȡ���ݿ�����˨��ĸ��ԣ��,�ڶ�ά0��ʾ�����϶ˣ�1��ʾ�¶�
	float (*AllowedMaxLugExt)[2]=NULL;   //��¼���Ӽ������������󳤶�ֵ,���˲�������Ӽ��ĳ��Ȳ��ô��ڴ�ֵ����������ƥ���������ֵ������ȥ��L8����һ�����ռ�ݵĿռ��ʣ�µ����ɲ���ֵ��
	long *PreviousPartInsertLen=NULL;   //ǰһ��������һ��������뱾���(L8)�ĳ���ֵ

	try
	{
		_variant_t vTmp1,vTmp2,vTmp3;
		 _RecordsetPtr rs;
		 rs.CreateInstance(__uuidof(Recordset));
		 _RecordsetPtr rsTZB;
		 rsTZB.CreateInstance(__uuidof(Recordset));
		 _RecordsetPtr rsTmp;
		 rsTmp.CreateInstance(__uuidof(Recordset));

		 CString tbn1, sBHFormat , sBH;
		 float sngLugAvgLen=0.0 , mvL=0.0 , sngTmpSumLen=0.0;
		 float iDifRodLen=0.0 ; //���˳���Բ��ǰ��Բ����Ĳ�ֵmm
		 long i=0, k=0, n =0, iNumRod=0 ;
		 long RodLen[36];
		 memset(RodLen,0,sizeof(RodLen));
		 float MinLength=0.0, MaxLength=0.0;
		 float IncrementStart1=0.0, IncrementLength1=0.0;
		 float IncrementStart2=0.0, IncrementLength2=0.0;
		 bool bLengthChanged=0;
		 CString mstrCurrentPart;    //��ǰ���CustomID
//		 long lngLastPartNo;  //��¼����֧���ܵ������������������
//		 long lngCurrentPartNo;  //��¼��ǰ��������������������
//		 long lngAttachedPartNo;   //��¼��ǰ�����ǰһ�����֮��ĸ�����
		 CString sLugPASA;  //�����������ʽ��־
		 CString tmpSQLlugLen;
		 giDiameter=0.0;   //����ֱ��
		 float lngLastRodPos=0; //���һ�����˵�˳���
		 float iTotalDeltaRodLen=0.0;    //�ܵ������������ڳ��ȣ�=�����ܵ�ʵ�ʳ���-�����ܵı�ע�ߴ�)
		 float sngDim=0.0;   //��������
		 float tempT3=0.0 ; //�Ϸ�����T3�߶�
		 float tempT3A=0.0 ;   //�·�����T3A�߶�
		 float tempMinLength =0;  //�����������������С���ȣ�ÿ�������ܲ�һ����
		 float iRodMinLen=0.0;    //����������С�ܳ��ȣ�����Բ������>=��ֵ��
		 float iRodMaxLen=0.0;   //������������ܳ��ȣ�����Բ������<=��ֵ��
		 float iSumRodLength=0.0; //���˼����ܳ���
		 float iSumRodLength1=0.0;//֧���ܵĸ�����ߺ͹ܲ����֮����ܳ���(����������)//pfg2005.12.21
		 float fTmp;//��ʱ����
		 float mfNutsHeight;//��¼������ĸ�ĺ��
		CString SQLx,sTmp,sTmp1;
		_variant_t vTmp;


		//ע���¾��б�����IsSAPart<>-1����������NOT IsNull(seq)���Ա��ں����Ӧһ�£���Ϊ����Ҳ����û�б�ţ����ø�������������Ҫ��������ж�
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND NOT IsNull(ClassID) AND ClassID<>") + ltos(iAttached) + _T(" AND ClassID<>") + ltos(iBolts) + _T(" AND ClassID<>") + ltos(iNuts) + _T(" AND IsSAPart<>-1 ORDER BY recno");
		rsTZB->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenDynamic, adLockOptimistic, adCmdText); 
		if( rsTZB->BOF && rsTZB->adoEOF )
		{
			sTmp.Format(GetResStr(IDS_NoThisZDJHResultInTBNSelPrjspecTZB),EDIBgbl::GetDBName(EDIBgbl::dbPRJDB), EDIBgbl::Btype[EDIBgbl::TZB],SQLx);
			throw sTmp;
		}
		else 
		{
			rsTZB->MoveLast();
			int iTZBRcount = rsTZB->RecordCount;
			i = iTZBRcount;
			rsTZB->MoveFirst();//�ӿ��ٶ�
			//������
			//��һ������±�i=0			
			Ptype=new CString [i];
			mlPartClassID=new long [i];
			mlPartIndex=new long [i];
			mlPartRecno=new long [i];
			mlPartDia=new long [i];
			mvActualLugExt=new float [i][2];
			AllowedMinLugExt=new float [i][2];
			AllowedMaxLugExt=new float [i][2];
			PreviousPartInsertLen=new long [i];

			memset(mlPartClassID,0,sizeof(long)*i);
			memset(mlPartIndex,-1,sizeof(long)*i);//��-1����ֹ�͸���iSA=0����
			memset(mlPartRecno,0,sizeof(long)*i);
			memset(mlPartDia,0,sizeof(long)*i);
			memset(PreviousPartInsertLen,0,sizeof(long)*i);
			memset(mvActualLugExt,0,sizeof(float)*i*2);
			memset(AllowedMaxLugExt,0,sizeof(float)*i*2);
			memset(AllowedMinLugExt,0,sizeof(float)*i*2);


			for( i = 0 ;i< iTZBRcount;i++)
			{//4 ZA���е�ÿһ����¼����ѭ��ȡֵ(start)
				rsTZB->get_Collect((_variant_t)_T("ID"),&vTmp);
				Ptype[i] =vtos(vTmp);

				rsTZB->get_Collect((_variant_t)_T("ClassID"),&vTmp);
				mlPartClassID[i] =vtoi(vTmp);

				rsTZB->get_Collect((_variant_t)_T("recno"),&vTmp);
				mlPartRecno[i]=vtoi(vTmp);

				mlPartIndex[i] = vtoi(sFindFLD(_T("ID"), _T("Index"), Ptype[i]));
				rsTZB->get_Collect((_variant_t)_T("sizeC"),&vTmp);
				mlPartDia[i] = vtoi(vTmp);

				if( mlPartClassID[i] == iROD )
				{//3
					SQLx.Format(_T("SELECT SizeH,sizeHH FROM [%s] WHERE size2 = %d AND CustomID = \'%s\'"),modPHScal::tbnBoltsNuts,mlPartDia[i],modPHScal::sFindCustomID("F2"));	
					rsTmp->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
						adOpenDynamic, adLockOptimistic, adCmdText); 
					if(rsTmp->BOF && rsTmp->adoEOF )
					{//1
						sTmp.Format(GetResStr(IDS_NoRecordInXtableInXmdb),EDIBgbl::GetDBName(modPHScal::dbZDJcrude), modPHScal::tbnBoltsNuts,SQLx);
						throw sTmp;
//						mfNutsHeight = 0;
					}//1
					else
					{//2
						rsTmp->get_Collect((_variant_t)"SizeH",&vTmp);
						mfNutsHeight=vtof(vTmp);
						if(mfNutsHeight == 0)
						{//1
							rsTmp->get_Collect((_variant_t)"sizeHH",&vTmp);
							mfNutsHeight = vtof(vTmp);
						}//1
					}//2

					rsTmp->Close();
					//���Ƶ���
					if(Ptype[i]==_T("L2"))
					{//1
						//�����϶˲��볤��=L2�����Ƴ���
						rsTZB->get_Collect((_variant_t)_T("AllowedMinLugExt"),&vTmp);
						AllowedMinLugExt[i][0]= vtof(vTmp);
						AllowedMaxLugExt[i][0] = vtof(vTmp);
						//�����¶˲��볤��=�����Ƴ���
						rsTZB->get_Collect((_variant_t)_T("AllowedMaxLugExt"),&vTmp);
						AllowedMinLugExt[i][1]= vtof(vTmp)-mfNutsHeight;
						AllowedMaxLugExt[i][1] = vtof(vTmp)-mfNutsHeight;						
					}//1
					else
					{//1
						//�����϶˲��볤��=L2�����Ƴ��Ȼ�L1�����Ƴ���
						rsTZB->get_Collect((_variant_t)_T("AllowedMinLugExt"),&vTmp);
						AllowedMinLugExt[i][0]= vtof(vTmp)-mfNutsHeight;
						AllowedMaxLugExt[i][0] = vtof(vTmp)-mfNutsHeight;
						//�����¶˲��볤��=�����Ƴ���
						rsTZB->get_Collect((_variant_t)_T("AllowedMaxLugExt"),&vTmp);
						AllowedMinLugExt[i][1]= vtof(vTmp)-mfNutsHeight;
						AllowedMaxLugExt[i][1] = vtof(vTmp)-mfNutsHeight;						
					}//1
				}//3
				else
				{//2
					//�������Ӽ���Ԥ��ȡ����ֵ
					if( Ptype[i] == _T("LHA") ||Ptype[i] == _T("LHB") || Ptype[i] == _T("PHA") || Ptype[i] == _T("PHB"))
					{//1
						rsTZB->get_Collect((_variant_t)_T("AllowedMinLugExtUp"),&vTmp);
						AllowedMinLugExt[i][0]= vtof(vTmp);
						rsTZB->get_Collect((_variant_t)_T("AllowedMinLugExt"),&vTmp);
						AllowedMinLugExt[i][1]= vtof(vTmp);
						
						rsTZB->get_Collect((_variant_t)_T("AllowedMaxLugExtUp"),&vTmp);
						AllowedMaxLugExt[i][0] = vtof(vTmp);
						rsTZB->get_Collect((_variant_t)_T("AllowedMaxLugExt"),&vTmp);
						AllowedMaxLugExt[i][1] = vtof(vTmp);
					}//1
					else
					{//1						
						rsTZB->get_Collect((_variant_t)_T("AllowedMinLugExt"),&vTmp);
						AllowedMinLugExt[i][0]= vtof(vTmp);
						AllowedMinLugExt[i][1]= vtof(vTmp);
						
						rsTZB->get_Collect((_variant_t)_T("AllowedMaxLugExt"),&vTmp);
						AllowedMaxLugExt[i][0] = vtof(vTmp);
						AllowedMaxLugExt[i][1] = vtof(vTmp);
					}//1
				}//2
				rsTZB->MoveNext();
			}//4 ZA���е�ÿһ����¼����ѭ��ȡֵ(end)
						
			//�ڴ�֮ǰ�����Ȼ����һ�������ClassID��Index
			//�����ڵ�һ��ѭ��֮����ΪҪ��ȡ��һ�������ClassID��Index
			bool bHasPA=false,bHasSA=false,bHasT3=false,bHasT3A=false;
			int iRodPosHasPA,iRodPosHasSA;
			rsTZB->MoveFirst();
			giNumRod = 0;
			rsTZB->MoveFirst();
			for( i = 0 ;i< iTZBRcount;i++)
			{
				rsTZB->get_Collect((_variant_t)_T("ID"),&vTmp);
				Ptype[i] =vtos(vTmp);

				//�޸���2003.06.09��ԭΪ 
				//if(mlPartClassID[i] == iROD)
				if( mlPartClassID[i] == iROD || mlPartClassID[i] == iXL1)
				{ //3 ������(start)
					//������,��������1
					giNumRod++;
					//��ס���һ�����˵�λ�ã��Ա��������������ʱ�õ������˳���
					lngLastRodPos = i;
					//�������ֱ��ֵ(��ǰͨ��GetPhsBHandSizes������ʱ������sizeC�ֶ�)���Ա����ڻ��Ժ�ʹ��
					//rsTZB->get_Collect((_variant_t)_T("sizeC"),vTmp);
					//iDiameter = vtof(vTmp);
					//�ڹ���GetBoltsNutsAndAttachmentsCLgg�У��ֶ�C��������������ܲ������������·��������ӵ�����������������С����(����Ժ),��������ԺC���ܴ����������Ƴ���
					//֧�����㲿���߶Ⱥͣ���������ܲ������������·����ɵȵĳ�ͷԣ����
					//�����·���ͷԣ��
					if( Ptype[i-1] == _T("T3A") || mlPartIndex[i-1]==iPA)
					{//2
						//�ܲ����·����ɣ����¶�
						rsTZB->get_Collect((_variant_t)_T("C"),&vTmp);
						fTmp=vtof(vTmp);
						//�����¶˲��볤��
						//�ܵ����Ƴ�������
						//����ֵȡ��Сֵ
						if(fTmp < AllowedMaxLugExt[i][1])
						{//1
							AllowedMinLugExt[i-1][0]= fTmp;
							AllowedMaxLugExt[i-1][0]= AllowedMaxLugExt[i][1];
							AllowedMinLugExt[i-1][1]= fTmp;
							AllowedMaxLugExt[i-1][1]= AllowedMaxLugExt[i][1];
							//iSumRodLength -=fTmp;
						}//1
						else
						{//1
							AllowedMinLugExt[i-1][0]= AllowedMinLugExt[i][1];
							AllowedMaxLugExt[i-1][0]= AllowedMaxLugExt[i][1];
							AllowedMinLugExt[i-1][1]= AllowedMinLugExt[i][1];
							AllowedMaxLugExt[i-1][1]= AllowedMaxLugExt[i][1];
							//iSumRodLength -=AllowedMinLugExt[i-1][0];
						}//1
					}//2

					if((mlPartClassID[i - 1] == iCSPR && (Ptype[i - 1] != _T("ZHB") || Ptype[i - 1] != _T("LHA") || Ptype[i - 1] != _T("PHA")) ) || ( mlPartClassID[i - 1] == iSPR && Ptype[i - 1] == _T("T3A"))||Ptype[i - 1].Left (2) == _T("L8") ||Ptype[i - 1].Left (2) == _T("L7"))
					{//2
						if( AllowedMaxLugExt[i - 1][0] > AllowedMaxLugExt[i][1] )
						{//1
							AllowedMaxLugExt[i - 1][0] = AllowedMaxLugExt[i][1];
							//AllowedMaxLugExt[i - 1][1] = AllowedMaxLugExt[i][1];//����2005.11.16 ���ܡ�pfg
						}//1
					}//2
					if((mlPartClassID[i + 1] == iCSPR && Ptype[i + 1] != _T("ZHB")) || ( mlPartClassID[i + 1] == iSPR && Ptype[i + 1] == _T("T3"))||Ptype[i + 1].Left (2) == _T("L8") ||Ptype[i + 1].Left (2) == _T("L7"))
					{//2
						if( AllowedMaxLugExt[i + 1][1] > AllowedMaxLugExt[i][0] )
						{//1
							//AllowedMaxLugExt[i + 1][0] = AllowedMaxLugExt[i][0];//����2005.11.16 ���ܡ�pfg
							AllowedMaxLugExt[i + 1][1] = AllowedMaxLugExt[i][0];
						}//1
					}//2

					//�����Ϸ���ͷԣ��
					if( Ptype[i+1] == _T("T3") || mlPartIndex[i+1]==iSA)
					{//2
						//�������Ϸ����ɣ��ӳߴ��϶�
						rsTZB->get_Collect((_variant_t)_T("C"),&vTmp);
						fTmp=vtof(vTmp);
						//�����϶˲��볤��
						//�ܵ����Ƴ�������
						if(fTmp < AllowedMaxLugExt[i][1])
						{//1
							AllowedMinLugExt[i+1][0] = fTmp;
							AllowedMaxLugExt[i+1][0] = AllowedMaxLugExt[i][0];
							AllowedMinLugExt[i+1][1] = fTmp;
							AllowedMaxLugExt[i+1][1] = AllowedMaxLugExt[i][0];
							//iSumRodLength -=fTmp;
						}//1
						else
						{//1
							AllowedMinLugExt[i+1][0] = AllowedMinLugExt[i][0];
							AllowedMaxLugExt[i+1][0] = AllowedMaxLugExt[i][0];
							AllowedMinLugExt[i+1][1] = AllowedMinLugExt[i][0];
							AllowedMaxLugExt[i+1][1] = AllowedMaxLugExt[i][0];
							//iSumRodLength -=AllowedMinLugExt[i+1][0];
						}//1
					}//2
					//L5,L6,L7,L8
					if( Ptype[i-1] == _T("L5") || Ptype[i-1] == _T("L6") )
					{//2
						//������˿���¶˲���ֵ�ܵ�ǰһ�������϶������Ƴ�������
						//�ܵ����Ƴ�������
						if(AllowedMaxLugExt[i-2][0] < AllowedMaxLugExt[i-1][1])
						{//1
							//�������϶����Ƴ���<������˿�¶�����������ֵ
							//AllowedMinLugExt[i-1][1] = AllowedMinLugExt[i-2][0];
							if( AllowedMaxLugExt[i-2][0] <= 0 )
								;
							else
								AllowedMaxLugExt[i-1][1] = AllowedMaxLugExt[i-2][0];
						}//1
						if(AllowedMaxLugExt[i][1] < AllowedMaxLugExt[i-1][0])
						{//1
							//�������¶����Ƴ���<������˿�϶�����������ֵ
							//AllowedMinLugExt[i-1][0] = AllowedMinLugExt[i][1];
							if( AllowedMaxLugExt[i][1] <= 0 )
								;
							else
								AllowedMaxLugExt[i-1][0] = AllowedMaxLugExt[i][1];
						}//1
					}//2
				}//3 ������(end)
				else 
				{//6 //��������(start)
					rsTZB->get_Collect((_variant_t)_T("sizeDIM"),&vTmp);
					if( Ptype[i] == _T("T3A") )
						tempT3A = fabs(vtof(vTmp));
					else if( Ptype[i] == _T("T3") )
						tempT3 = fabs(vtof(vTmp));
					if( Ptype[i] == _T("T3A") || mlPartIndex[i]==iPA)
					{//3
						//�Ѿ�����
						//����ܲ����·��������������ӣ����˳����ǿɵ�����
						//��һ�����i=0
						if((i+1)<iTZBRcount)
						{
							if(mlPartClassID[i+1] == iROD)
							{
								bHasPA=true;
								iRodPosHasPA=i;
							}
						}
					}//3
					else
					{//5
						if( Ptype[i] == _T("T3") || mlPartIndex[i]==iSA)
						{//4
							//�Ѿ�����
							//����������Ϸ��������������ӣ����˳����ǿɵ�����
							if((i-1)>=0)
							{//3
								if(mlPartClassID[i-1] == iROD)
								{
									bHasSA=true;
									iRodPosHasSA=i;
								}
								else if(mlPartClassID[i-1] == iCSPR)
								{//2
									//
									if(Ptype[i-1]==_T("PHD1"))
									{
										//�����·�������ܼ�������߶�
										rsTZB->get_Collect((_variant_t)_T("sizeDIM"),&vTmp);//pfg?
										iSumRodLength+=vtof(vTmp);
									}
									else
									{
									}
								}//2
							}//3
						}//4
						else
						{
							//�Ѿ���ò���ֵ��
						}
					}//5
					rsTZB->get_Collect((_variant_t)_T("PreviousPartInsertLen"),&vTmp);
					PreviousPartInsertLen[i] = vtoi(vTmp);
					//֧�����㲿���߶Ⱥͣ�����������ܲ������������·����ɵȵ���С������ȣ������������Ƽ��ڵ����˲��볤�ȣ�
					//������sizeH�ֶ��ڴ�֮ǰ����=0����sizeDIM�ֶα���=�����߶ȣ����˳��Ȳ��ܼ�����ȷ��
					rsTZB->get_Collect((_variant_t)_T("sizeH"),&vTmp);
					iSumRodLength+=vtof(vTmp);
					//iSumRodLength += abs(vtof(vTmp));
				}//6	//��������(end)
				rsTZB->MoveNext();
			}
      
			//���˳���=(�������-�ܲ����)*1000-�����㲿���߶�֮��+���˳��ȵ���ֵ
			//��ʱ��õ������ܳ�����һ����Сֵ����������ô����������˿�������������ܳ��ȶ�����>=��ֵ
			//��ʱ�������ܳ��ȿ��������·����ɻ�ܲ��������ӵ�ԣ��������û�п������ƽ�ͷ���볤�ȡ�
			iSumRodLength = (sngSEL - sngPEL) * 1000 - iSumRodLength;
			iSumRodLength1=iSumRodLength;//pfg2005.12.21
			//������Сֵ��ʼֵ׼���ۼ�
			iRodMinLen = iSumRodLength;
			iRodMaxLen = iSumRodLength;
      
			//���¿�ʼ�������������Ӽ����볤�ȣ������������ܳ���
			rsTZB->MoveFirst();
			for( i = 0 ;i<iTZBRcount;i++)
			{
				//if( Ptype[i] == _T("L8") )
				//2003.05.23����ΰ�޸ģ�ԭΪ�Ͼ�
				if( Ptype[i].Left(2) == _T("L8") )
				{//5 //��ǰ��L8(start)
					if( i + 1 <= iTZBRcount - 1 )
					{//4
						if( Ptype[i + 1] == _T("L7") || Ptype[i + 1] == _T("G12") || Ptype[i + 1] == _T("L7Dd") || Ptype[i + 1] == _T("G12Lc") )
						{//3
							//��ǰ��L8,��һ����L7��G12
							//����L8�Ĳ��볤��,�ó����ں�һ�������PreviousPartInsertLen�ֶ�
							AllowedMaxLugExt[i][0] -= PreviousPartInsertLen[i + 1];
							if( AllowedMaxLugExt[i][0] < AllowedMinLugExt[i][0] )
							{
								//�������������볤��<��Сֵ
								if(Ptype[i] != _T("L8Dd"))
								{
									sTmp.Format(GetResStr(IDS_PreviousPartInsertLenOverAllowedValue),ltos(PreviousPartInsertLen[i+1]),ftos(AllowedMaxLugExt[i][0]-AllowedMinLugExt[i][0]));
									ShowMessage(sTmp);
								}

								//2007.09.10
								iRodMaxLen += AllowedMaxLugExt[i][0];
								iRodMinLen += AllowedMinLugExt[i][0];
							}
							else
							{
								iRodMaxLen += AllowedMaxLugExt[i][0];
								iRodMinLen += AllowedMinLugExt[i][0];
							}
						}//3
						else 
						{//2
							//��ǰ��L8,��һ������L7��G12
							//����L8�Ĳ��볤��,�ó�����ǰһ�������PreviousPartInsertLen�ֶ�
							AllowedMaxLugExt[i][0] -=PreviousPartInsertLen[i - 1];	
							if( AllowedMaxLugExt[i][0] < AllowedMinLugExt[i][0] )
							{
								//�������������볤��<��Сֵ
								sTmp.Format(GetResStr(IDS_PreviousPartInsertLenOverAllowedValue),ltos(PreviousPartInsertLen[i+1]),ftos(AllowedMaxLugExt[i][0]-AllowedMinLugExt[i][0]));
								ShowMessage(sTmp);
								
								//2007.09.10
								iRodMaxLen += AllowedMaxLugExt[i][0];
								iRodMinLen += AllowedMinLugExt[i][0];
							}
							else 
							{
								iRodMaxLen += AllowedMaxLugExt[i][0];
								iRodMinLen += AllowedMinLugExt[i][0];
							}
						}//2
					}//4					
				}//5 //��ǰ��L8(end)
				else
				{ //��ǰ����L8(start)
					if( Ptype[i].Left(2)==_T("L5") || Ptype[i].Left(2)==_T("L6") || Ptype[i].Left(2)==_T("L7") )
					{//2 //��ǰ����L8,��L5��L6��L7(start)
						if( Ptype[i] == _T("L7") || Ptype[i] == _T("L7Dd") )
						{
							iRodMaxLen = iRodMaxLen + AllowedMaxLugExt[i][0];
							iRodMinLen = iRodMinLen + AllowedMinLugExt[i][0];
						}
						else
						{
							//������˿�����ƽ�ͷ��plus�¶κ��϶�
							iRodMaxLen = iRodMaxLen + AllowedMaxLugExt[i][1] + AllowedMaxLugExt[i][0] ;
							iRodMinLen = iRodMinLen + AllowedMinLugExt[i][1] + AllowedMinLugExt[i][0] ;
						}
					}//2 //��ǰ����L8,��L5��L6��L7(end)
					else 
					{
						//��ǰ����L8,����L5��L6��L7
						if( mlPartClassID[i] == iSPR )
						{
							//��ǰ����L8,����L5��L6��L7,�ǵ���
							if( Ptype[i] != _T("T3") && Ptype[i] != _T("T3A") && Ptype[i] != _T("T4") )
							{
								//��ǰ����L8,����L5��L6��L7,�ǵ��ɣ��Ҳ������·����ɡ�֧�ܵ���
								if( mlPartClassID[i - 1] == iROD )
								{
									SQLx = _T("SELECT * FROM [") + tbnSPRINGL5Crude + _T("] WHERE size2=") + ltos(mlPartDia[i - 1]);
									rsTmp->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
										adOpenStatic, adLockOptimistic, adCmdText); 

									if( rsTmp->BOF && rsTmp->adoEOF )
									{
										sTmp.Format(GetResStr(IDS_NotFoundThisDia),EDIBgbl::GetDBName(dbZDJcrude),tbnSPRINGL5Crude,ltos(mlPartDia[i-1]));
										ShowMessage(sTmp);
									}
									else
									{
										//�뿴���ݿⶨ��˵�������ʯ���豸����������p54
										rsTmp->get_Collect((_variant_t)_T("size4"),&vTmp);

										AllowedMinLugExt[i][0] = vtof(vTmp);
										rsTmp->get_Collect((_variant_t)_T("size5"),&vTmp1);
										rsTmp->get_Collect((_variant_t)_T("size3"),&vTmp2);
										rsTmp->get_Collect((_variant_t)_T("size2"),&vTmp3);
										AllowedMaxLugExt[i][0] =(vTmp1.vt==VT_NULL ? (vtof(vTmp2) - vtof(vTmp3)) / 2 : vtof(vTmp1));
										//���볤�ȱ���<=�����϶����Ƴ���
										if(AllowedMaxLugExt[i-1][0]<AllowedMaxLugExt[i][0])	AllowedMaxLugExt[i][0]=AllowedMaxLugExt[i-1][0];
										//��Ϊ���ɻ�������ɵ��ɽ���ĸֻ����һ�˿ɲ������ˣ��ʿɲ��볤�ȼ���sngDim��
										iRodMaxLen +=AllowedMaxLugExt[i][0];
										iRodMinLen +=AllowedMinLugExt[i][0];
									}
									rsTmp->Close();
								}
								else 
								{
									if( mlPartClassID[i + 1] == iROD )
									{
										//�������ɽ���ĸ�ĵ��Լ�
										SQLx = _T("SELECT * FROM [") + tbnSPRINGL5Crude + _T("] WHERE size2=") + ltos(mlPartDia[i + 1]);
										rsTmp->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
											adOpenStatic, adLockOptimistic, adCmdText); 
										if( rsTmp->BOF && rsTmp->adoEOF )
										{
											sTmp.Format(GetResStr(IDS_NotFoundThisDia),  EDIBgbl::GetDBName(dbZDJcrude), tbnSPRINGL5Crude,ltos(mlPartDia[i + 1])); ShowMessage(sTmp);
										}
										else 
										{
											//�뿴���ݿⶨ��˵����PHSdbManual.DOC����ʯ���豸����������p54
											rsTmp->get_Collect((_variant_t)_T("size4"),&vTmp);
											AllowedMinLugExt[i][0] = vtof(vTmp);
											rsTmp->get_Collect((_variant_t)_T("size5"),&vTmp1);
											rsTmp->get_Collect((_variant_t)_T("size3"),&vTmp2);
											rsTmp->get_Collect((_variant_t)_T("size2"),&vTmp3);
											AllowedMaxLugExt[i][0] =(vTmp1.vt==VT_NULL ? (vtof(vTmp2) - vtof(vTmp3)) / 2 : vtof(vTmp1));
											//���볤�ȱ���<=�����¶����Ƴ���
											if(AllowedMaxLugExt[i+1][1]<AllowedMaxLugExt[i][0])	AllowedMaxLugExt[i][0]=AllowedMaxLugExt[i+1][1];
											//��Ϊ���ɻ�������ɵ��ɽ���ĸֻ����һ�˿ɲ������ˣ��ʿɲ��볤�ȼ���sngDim��
											iRodMaxLen +=AllowedMaxLugExt[i][0];
											iRodMinLen +=AllowedMinLugExt[i][0];
										}
										rsTmp->Close();
									}
								}
							}
							else
							{
								//��ǰ����L8,����L5��L6��L7,�ǵ��ɣ������·����ɡ�֧�ܵ���
								iRodMaxLen +=AllowedMaxLugExt[i][0];
								iRodMinLen +=AllowedMinLugExt[i][0];
							}
						}
						else 
						{
							//��ǰ����L8,����L5��L6��L7,���ǵ���
							if( mlPartClassID[i] == iCSPR && Ptype[i] != _T("ZHB") )
							{
								//��ǰ����L8,����L5��L6��L7,���ǵ��ɣ��Ǻ������ɣ�������ZHB
								if( Ptype[i].Find(_T("HA"))==1 || Ptype[i].Find(_T("HB"))==1 )
								{
									//PHA,LHA,PHB,LHB
									//�������Ӻ�������˱���������һ�γ���PP,��getphsBHandSizes����������A�ֶ���
									//�������䵼�³ߴ������󣬻ָ�ԭ״
									//iRodMaxLen = iRodMaxLen + AllowedMaxLugExt(i) + IIf(IsNull(rsTZB.Fields(_T("A"))), 0, rsTZB.Fields(_T("A")))
									//iRodMinLen = iRodMinLen + AllowedMinLugExt(i) + IIf(IsNull(rsTZB.Fields(_T("A"))), 0, rsTZB.Fields(_T("A")))
									
									//����ΰ����2003.06.03ԭΪ������ʽ
									//iRodMaxLen +=AllowedMaxLugExt[i][0];
									//iRodMinLen +=AllowedMinLugExt[i][0];
									iRodMaxLen +=AllowedMaxLugExt[i][0]+AllowedMaxLugExt[i][1];
									iRodMinLen +=AllowedMinLugExt[i][0]+AllowedMinLugExt[i][1];
								}
								else 
								{
									//��Ϊ���ɻ�������ɵ��ɽ���ĸֻ����һ�˿ɲ������ˣ��ʿɲ��볤�ȡ�
									//��GetPhsBHandSizes���Ѿ����AllowedMaxLugExt,AllowedMinLugExt
									//iRodMaxLen +=AllowedMaxLugExt[i][0];//pfg2005.12.27ԭ����
									//iRodMinLen +=AllowedMinLugExt[i][0];//pfg2005.12.27ԭ����
									iRodMaxLen +=AllowedMaxLugExt[i][1];//pfg2005.12.27
									iRodMinLen +=AllowedMinLugExt[i][1];//pfg2005.12.27

								}
							}
							else 
							{
								//��ǰ����L8,����L5��L6��L7,���ǵ��ɣ�����(�������ɣ�������ZHB)
								//�ܲ�������������
								if(mlPartClassID[i]==iROD)
								{
									//���˵�����γ������������Ӽ�/�ܲ�/������ͳ��;
								}
								else//mlPartIndex[i-1]==iPA;
								{									
									iRodMaxLen +=AllowedMaxLugExt[i][0];
									iRodMinLen +=AllowedMinLugExt[i][0];
								}
							}
						}
					}
				}
				rsTZB->MoveNext();
			}


			//���¿�ʼ��Ϊ���������������������Ӽ��ĳ���
			//��Ϊ��������Ѿ���GetBoltsNutsAndAttachmentsCLgg����
      
 			//���˳���=(�������-�ܲ����)*1000-�����㲿���߶�֮��+���˳��ȵ���ֵ
			//��ʱ�����ܳ���iSumRodLength��Ȼ��һ����Сֵ����������ô����������˿�������������ܳ��ȶ�����>=��ֵ
			//�����ܳ��ȿ�����iSumRodLength~iSumRodLength+iTotalDeltaRodLen֮�������
			//����Բ������ܳ��ȱ���λ��������䡣
			//iSumRodLength = iSumRodLength + iTotalDeltaRodLen
      
			//�������˳��ȵİ�װ�͵���ԣ�ȣ�����ȡ����������������п��������������˳���Բ������
			//1��L5~L8�������ɡ���������������L5�����пɵ������ȣ�������һ��������С�߶�ֵ��һ���������߶�ֵ��
			//2���ɴ˿ɼ�������˳��ȵ����ֵ����Сֵ
			//3�����˵ĳ��ȴ����ֵ��ʼԲ����������ӽ�������
			//4��������˵����ֵԲ����<=��Сֵ
			
			//iTotalDeltaRodLen���������С�Ĳ�ֵ�����ڸ�����Ŀɵ�����϶֮�ͣ�
			//iDifRodLen=����Բ��ǰ����󳤶�-����Բ����ĳ��ȣ�
			//Բ�����������˳��ȱ���󳤶�ҪС�����Ҫ���¼��������ڸ�����ļ�϶������ĳ��ȣ���ά�����˵�ʵ�ʳ������߲ì�ܡ�
			//���㷽���ǣ����ո������϶��С����iDifRodLen������(������iTotalDeltaRodLen)���䵽����϶��			
			bLengthChanged = false;
			float fLenPlus=0;//�ӵ��������·����ɶ˵ĳ�������ֵ
			
			//�����Ϸ��·��ɵ���ʱ��iTotalDeltaRodLen1
			float iTotalDeltaRodLen1=0.0,iDifRodLen1=0.0;
			//���Բ��2�Σ��ʺ�������
			bool bRoundFA=false;
      
			do
			{
				iTotalDeltaRodLen = iRodMaxLen - iRodMinLen;
				iSumRodLength = iRodMaxLen;
				if( iTotalDeltaRodLen == 0 )
				{
					if(bHasPA || bHasSA)
						//�����µ�������ʱ�����ر���
						;
					else
						ShowMessage(GetResStr(IDS_totalDeltaLugLenEQV0));
				}

				if( (long)iRodMinLen % modPHScal::gnRodLenRoundVal == 0 )
				{//2
					//��Сֵ��������
					if( int(iRodMaxLen / modPHScal::gnRodLenRoundVal) * modPHScal::gnRodLenRoundVal >= int(iRodMinLen / modPHScal::gnRodLenRoundVal) * modPHScal::gnRodLenRoundVal )
					{
						//����Բ���ɹ�
						iDifRodLen = iSumRodLength - int(iRodMaxLen / modPHScal::gnRodLenRoundVal) * modPHScal::gnRodLenRoundVal;
						iSumRodLength = int(iRodMaxLen / modPHScal::gnRodLenRoundVal) * modPHScal::gnRodLenRoundVal;
						break;
					}
					else 
					{
						//�����ܵ����!!!
						//����Բ�����ɹ�
					}
				}//2
				else
				{//2
					//��Сֵ��������
					if( int(iRodMaxLen / modPHScal::gnRodLenRoundVal) * modPHScal::gnRodLenRoundVal > int(iRodMinLen / modPHScal::gnRodLenRoundVal) * modPHScal::gnRodLenRoundVal )
					//if( iTotalDeltaRodLen >= modPHScal::gnRodLenRoundVal )
					{
						//Բ���ɹ�
						iDifRodLen = iSumRodLength - int(iRodMaxLen / modPHScal::gnRodLenRoundVal) * modPHScal::gnRodLenRoundVal;
						iSumRodLength = int(iRodMaxLen / modPHScal::gnRodLenRoundVal) * modPHScal::gnRodLenRoundVal;
						break;
					}
					else 
					{
						//����Բ�����ɹ�
						//���β���Բ���ɹ�,ȡ�����С��ƽ��ֵ
						iDifRodLen = iSumRodLength - int((iRodMaxLen + iRodMinLen) / 2);
						iSumRodLength = int((iRodMaxLen + iRodMinLen) / 2);
						sTmp.Format(GetResStr(IDS_CannotRoundSumLugLength),ftos(iRodMaxLen), ftos(iRodMinLen),ftos(iSumRodLength));
						ShowMessage(sTmp);
						break;
					}
				}//2
			}while(true);
      
            
			//���Ȱ������˳����Ƿ���Ե������򣬲��ܵ������ȵ�����������ǰ�档
			//��������󣬳��ȷ������������ͨ���ɵ������ƽ�ͷ������
			int  bA , bB; //A,B�˿ɵ�������
			iNumRod = 0;
		
			rsTZB->MoveFirst();
			for( i = 0 ;i< iTZBRcount;i++)
			{//4
				bA = 1;
				bB = 1;
				if( mlPartClassID[i] == iROD )
				{//3
					iNumRod ++;
					if( i - 1 >= 0 )
					{
						if( mlPartIndex[i - 1] == iPA || Ptype[i-1] == _T("T3A") || Ptype[i-1] == _T("L6") )
						{
							bA = 0;
						}
					}
					if( i + 1 < iTZBRcount )
					{
						if( mlPartIndex[i + 1] == iSA || Ptype[i+1] == _T("T3") ||  Ptype[i+1] == _T("L6") )
						{
							bB = 0;
						}
					}
					rsTZB->put_Collect((_variant_t)_T("size"),_variant_t((float)(bA + bB)));
					rsTZB->Update();
				}//3
				else 
				{
					rsTZB->put_Collect((_variant_t)_T("size"),_variant_t(3.0));
					rsTZB->Update();
				}
				rsTZB->MoveNext();
			}//4
      
			//Բ��������˳��Ȳ�ֵ���ɵ�����϶��С�������䵽���볤����
			//���Ұ����˷����Ϊ10�ı���ȡ��
			if(gnRodInsertLenRoundVal>1) 
			{
				CString sTmp;
				sTmp.Format(IDS_SetPoleInsertLenRoundVal,gnRodInsertLenRoundVal,gnRodInsertLenRoundVal);
				AfxMessageBox(sTmp);
			}
			iNumRod = 0;
			k = 0;//ʣ���ܲ��볤��
			n = 0;//��ǰ����ĳ��������볤��
			long  ns, ne; //�¶˷����ܳ���=ns(��ӦZB���ֶ�A),�϶˷����ܳ���=ne(��ӦZB���ֶ�B)
			//ע���¾��б�����IsSAPart<>-1����������NOT IsNull(seq)����Ϊ����Ҳ����û�б�ţ����ø�������������Ҫ��������ж�
			
			SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND NOT IsNull(ClassID) AND ClassID<>") + ltos(iAttached) + _T(" AND ClassID<>") + ltos(iBolts) + _T(" AND ClassID<>") + ltos(iNuts) + _T(" AND IsSAPart<>-1 AND (bUserAdd IS NULL OR bUserAdd=0) ORDER BY size");
			rsTmp->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
			long delta = 0;
			while(!rsTmp->BOF)
			{//9
				for( i = 0 ;i< iTZBRcount;i++)
				{
					rsTmp->get_Collect((_variant_t)_T("recno"),&vTmp);
					if( mlPartRecno[i] == vtoi(vTmp) )
						break;
				}
				_variant_t vTmp;
				rs->Find((_bstr_t)(_T("recno=") + ltos(mlPartRecno[i])), 0, adSearchForward);
				if( mlPartClassID[i] == iROD )
				{//8 //��ǰ������(start)
					iNumRod ++;
					//���ͷβ���ȣ��Ա㱣��ֵ��
					ns = 0;
					ne = 0;
					//�����Ϸ����ɣ������������ϡ��¶�ͬʱ����

					//����ΰ�޸���2003.06.03,ԭΪ��ʽ
					//if( Ptype[i - 1] == _T("T3A") || mlPartIndex[i - 1] == iPA || Ptype[i-1] == _T("L5") || Ptype[i-1] == _T("L6") || Ptype[i-1] == _T("L7") || Ptype[i-1] == _T("L8") || Ptype[i-1] == _T("L7Dd") || Ptype[i-1] == _T("L8Dd") || Ptype[i-1] == _T("T5") || Ptype[i-1] == _T("LHG") )
					//��������HA��HB��������˲���������ɣ�����Ҫ��ȷ�Ļ����������Ptype[i - 1].Right (2) == _T("HA") || Ptype[i - 1].Right (2) == _T("HB")����
					//T1,T2������ǰ��ӹܲ���������ˣ��������Ptype[i -1] == _T("T1") || Ptype[i -1] == _T("T2") ���������ܱ�֤���뵯�ɵ����˳�������ȷ��
					if(Ptype[i -1] == _T("T1") || Ptype[i -1] == _T("T2") || Ptype[i - 1].Right (2) == _T("HA") || Ptype[i - 1].Right (2) == _T("HB") || Ptype[i - 1] == _T("T3A") || mlPartIndex[i - 1] == iPA || Ptype[i-1] == _T("L5") || Ptype[i-1] == _T("L6") || Ptype[i-1] == _T("L7") || Ptype[i-1] == _T("L8") || Ptype[i-1] == _T("L7Dd") || Ptype[i-1] == _T("L8Dd") || Ptype[i-1] == _T("T5") || Ptype[i-1] == _T("LHG") )
					{//4 ��ǰ������ǰ�����Ӽ�ΪT1,T2,HA,HB,T3A,iPA,L5,L6,L7,L8,L7Dd,L8Dd,T5,LHG (start)
						if(false/* Ptype[i - 1] == _T("L6") */)
						{//1
							//2003.05.22�����ע�ͣ�����L6���ݼ�¼��������Ա���ã�����size4<size5(Ӧ��size4=size5)���������˳��ȼ���������ֵ2*(size5-size4)��
							//����취���ǰ�L6����L5��������ȷ���������<=2.
							n = AllowedMinLugExt[i-1][0];
							//����Բ�����˲���ֵ
						}//1
						else 
						{//3
							if( iTotalDeltaRodLen == 0 )
							{//1
								n = 0;
							}//1
							else
							{//1
								if( Ptype[i - 1] == _T("L5")||Ptype[i - 1] == _T("L6") )
									//��2003.05.22�����ע�ͣ��˴�L6 ������������
									n = int(0.5 + AllowedMaxLugExt[i-1][0] - (AllowedMaxLugExt[i-1][0] - AllowedMinLugExt[i-1][0]) * iDifRodLen / iTotalDeltaRodLen);
								else
									n = int(0.5 + AllowedMaxLugExt[i-1][0] - (AllowedMaxLugExt[i-1][0] - AllowedMinLugExt[i-1][0]) * iDifRodLen / iTotalDeltaRodLen);
							}//1
							if( n < AllowedMinLugExt[i-1][0] )
							{//1
								n = AllowedMinLugExt[i-1][0];
							}//1
							if((int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal<=AllowedMaxLugExt[i-1][0])
							{//1
								//������˲��볤������Բ���󲻳�������������ֵ��
								//���������Բ��
								n=(int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal;//Բ�����˲���ֵ��5��10,etc.
							}//1
							else
							{//2
								if(int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal>=AllowedMinLugExt[i-1][0])
								{//1
									//������˲��볤������Բ����С��������С����ֵ
									n=int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal;//Բ�����˲���ֵ��5��10,etc.
								}//1
								else
								{//1
									//�޷�Բ����������
								}//1
							}//2
							k = k + AllowedMaxLugExt[i-1][0] - n;
						}//3
						rsTZB->put_Collect((_variant_t)_T("A"),_variant_t((float)n));
						rsTZB->Update();
					}//4 ��ǰ������ǰ�����Ӽ�ΪT1,T2,HA,HB,T3A,iPA,L5,L6,L7,L8,L7Dd,L8Dd,T5,LHG (end)	

					if( Ptype[i + 1] == _T("T3") || mlPartIndex[i + 1] == iSA || Ptype[i+1] == _T("L5") || Ptype[i+1] == _T("L6") || Ptype[i+1] == _T("L7") || Ptype[i+1] == _T("L8") || Ptype[i+1] == _T("L7Dd") || Ptype[i+1] == _T("L8Dd") || Ptype[i+1] == _T("T1") || Ptype[i+1] == _T("T2") || Ptype[i+1] == _T("T6") || mlPartClassID[i+1] == iCSPR && Ptype[i+1] != _T("ZHB") )
					{//7
						if( false /*Ptype[i + 1] == _T("L6")*/ )
						{//1
							//��2003.05.22�����ע�ͣ��˴�L6 ������������
							n = AllowedMinLugExt[i+1][0];
						}//1
						else 
						{//6
							if( iNumRod < giNumRod )
							{//5
								if( iTotalDeltaRodLen == 0 )
								{//1
									n = 0;
								}//1
								else
								{//4
									if( Ptype[i + 1] == _T("L5") || Ptype[i + 1] == _T("L6")||mlPartClassID[i+1] == iCSPR && Ptype[i+1] != _T("ZHB"))//pfg2005.12.23������mlPartClassID[i+1] == iCSPR && Ptype[i+1] != _T("ZHB")
									{//3
										//��2003.05.22�����ע�ͣ��˴�L6 ������������
										n = int(0.5 + AllowedMaxLugExt[i+1][1] - (AllowedMaxLugExt[i+1][1] - AllowedMinLugExt[i+1][1]) * iDifRodLen / iTotalDeltaRodLen);
										if( n < AllowedMinLugExt[i+1][1] )
										{//1
											n = AllowedMinLugExt[i+1][1];
										}//1
										if((int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal<=AllowedMaxLugExt[i+1][1])
										{//1
											//������˲��볤������Բ���󲻳�������������ֵ��
											//���������Բ��
											n=(int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal;//Բ�����˲���ֵ��5��10,etc.
										}//1
										else
										{//2
											if(int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal>=AllowedMinLugExt[i+1][1])
											{//1
												//������˲��볤������Բ����С��������С����ֵ
												n=int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal;//Բ�����˲���ֵ��5��10,etc.
											}//1
											else
											{//1
												//�޷�Բ����������
											}//1
										}//2

									}//3
									else
									{//3
										n = int(0.5 + AllowedMaxLugExt[i+1][0] - (AllowedMaxLugExt[i+1][0] - AllowedMinLugExt[i+1][0]) * iDifRodLen / iTotalDeltaRodLen);
										if( n < AllowedMinLugExt[i+1][0] )
										{//1
											n = AllowedMinLugExt[i+1][0];
										}//1

										if((int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal<=AllowedMaxLugExt[i+1][0])
										{//1
											//������˲��볤������Բ���󲻳�������������ֵ��
											//���������Բ��
											n=(int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal;//Բ�����˲���ֵ��5��10,etc.
										}//1
										else
										{//2
											if(int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal>=AllowedMinLugExt[i+1][0])
											{//1
												//������˲��볤������Բ����С��������С����ֵ
												n=int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal;//Բ�����˲���ֵ��5��10,etc.
											}//1
											else
											{//1
												//�޷�Բ����������
											}//1
										}//2
									}//3
								}//4
							}//5
							else
							{//4
								//���һ����
								if( Ptype[i + 1] == _T("L5") || Ptype[i + 1] == _T("L6")||mlPartClassID[i+1] == iCSPR && Ptype[i+1] != _T("ZHB"))//������mlPartClassID[i+1] == iCSPR && Ptype[i+1] != _T("ZHB")
								{//3
									//��2003.05.22�����ע�ͣ��˴�L6 ������������
									n = AllowedMaxLugExt[i+1][1] - (iDifRodLen - k);
									if( n > AllowedMaxLugExt[i+1][1] )
									{//2
										//������һ�������¶˲���ֵ����������������ֵ��������������ƶ�����֮��Ĳ�ֵ
										//ͬʱ��֤�����϶˲���ֵ����������������ֵ
										delta = n - AllowedMaxLugExt[i+1][1];
										n = AllowedMaxLugExt[i+1][1];
									
										_variant_t tmp;
										rsTZB->get_Collect((_variant_t)_T("B"), &tmp);
										float b = vtof(tmp);

										if( b + delta <= AllowedMaxLugExt[i+1][0])
										{//1
											rsTZB->put_Collect((_variant_t)_T("B"),_variant_t((float)(b+delta))); 
											rsTZB->Update();
										}//1
										else
										{//1
											CString sTmp;
											sTmp.Format(IDS_OverL5AllowedMaxRodExtOfxEnd,_T("Down"),n,(int)AllowedMaxLugExt[i+1][1]);
											AfxMessageBox(sTmp);
										}//1
									}//2
									if((int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal<=AllowedMaxLugExt[i+1][1])
									{//1
										//������˲��볤������Բ���󲻳�������������ֵ��
										//���������Բ��
									   //n=(int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal;//ԭ���� pfg2005.12.23Բ�����˲���ֵ��5��10,etc.
									}//1
									else
									{//2
										if(int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal>=AllowedMinLugExt[i+1][1])
										{//1
											//������˲��볤������Բ����С��������С����ֵ
											//n=int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal;//ԭ����pfg2005.12.23Բ�����˲���ֵ��5��10,etc.
										}//1
										else
										{//1
											//�޷�Բ����������
										}//1
									}//2
								}//3
								else
								{//3
									n = AllowedMaxLugExt[i+1][0] - (iDifRodLen - k);
									if( n > AllowedMaxLugExt[i+1][0] )
									{//2
										delta = n - AllowedMaxLugExt[i+1][0];
										n = AllowedMaxLugExt[i+1][0];
									
										_variant_t tmp;
										rsTZB->get_Collect((_variant_t)_T("B"), &tmp);
										float a = vtof(tmp);

										if( a + delta <= AllowedMaxLugExt[i+1][1])
										{//1
											rsTZB->put_Collect((_variant_t)_T("B"),_variant_t((float)(a+delta))); 
											rsTZB->Update();
										}//1
										else
										{//1
											CString sTmp;
											sTmp.Format(IDS_OverL5AllowedMaxRodExtOfxEnd,_T("Down"),n,(int)AllowedMaxLugExt[i+1][1]);
											AfxMessageBox(sTmp);
										}//1	
									}//2
									if( n < AllowedMinLugExt[i+1][0] )
									{//1
										//Ӧ�ñ��������˳��Ȳ���ֵС����С����ֵ
										n = AllowedMinLugExt[i+1][0];
									}//1
									if((int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal<=AllowedMaxLugExt[i+1][0])
									{//1
										//������˲��볤������Բ���󲻳�������������ֵ��
										//���������Բ��
										//n=(int(n/gnRodInsertLenRoundVal)+1)*gnRodInsertLenRoundVal;//ԭ����pfg2005.12.23Բ�����˲���ֵ��5��10,etc.
									
									}//1
									else
									{//2
										if(int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal>=AllowedMinLugExt[i+1][0])
										{//1
											//������˲��볤������Բ����С��������С����ֵ
											//n=int(n/gnRodInsertLenRoundVal)*gnRodInsertLenRoundVal;//ԭ����pfg2005.12.23Բ�����˲���ֵ��5��10,etc.
										}//1
										else
										{//1
											//�޷�Բ����������
										}//1
									}//2
								}//3
							}//4
							//pfg2005.12.23 ���ִ���취�ܲ��ò��Ϸ�һ���˼ά��ʽ����Ҳû�취��Ϊ�����޸�һ��
							//���úܶ�ʱ�䣬�������˲������С����û�кܺõؽ��
							if(mlPartClassID[i+1] == iCSPR && Ptype[i+1] != _T("ZHB")||Ptype[i + 1] == _T("L5"))//pfg2005.12.23//pfg2005.12.26����Ptype[i + 1] == _T("L5")
							{
								k = k + AllowedMaxLugExt[i+1][1] - n;//pfg2005.12.23
							}
							else
							{
								k = k + AllowedMaxLugExt[i+1][0] - n;
							}
						}//6
						rsTZB->put_Collect((_variant_t)_T("B"),_variant_t((float)n)); rsTZB->Update();
					}//7
				}//8
				rsTmp->MoveNext();
			}//9
			rsTmp->Close();   

		
		//���㲢�ҷ���ÿ��������С���ȣ�Ȼ����С�����������ȴ�����С�����������ˡ�
		//�Ƚ���С���ȱ�����size�ֶ�
   
		rsTZB->MoveFirst();
		iNumRod = 1;
		for( i = 0 ;i< iTZBRcount;i++)
		{
			rsTZB->get_Collect((_variant_t)_T("CustomID"),&vTmp);
			tbn1 = sFindTBN(vtos(vTmp));
			if( mlPartClassID[i] == iROD )
			{//5 ��ǰ������(start)
				//�����������
				SQLx = _T("SELECT * FROM [") + tbnLugInfo + _T("] WHERE (CustomID)=\'") +vtos(vTmp) + _T("\'");
				rs->Open((_bstr_t)SQLx, dbZDJcrude.GetInterfacePtr(), 
					adOpenDynamic, adLockOptimistic, adCmdText); 
				if( rs->BOF && rs->adoEOF )
				{
					sTmp.Format(GetResStr(IDS_NoLugCustomIDInLugINFO), EDIBgbl::GetDBName(dbZDJcrude),tbnLugInfo,SQLx);
					throw sTmp;
				}
				rs->MoveFirst();
				//������С����
				rs->get_Collect((_variant_t)_T("minLength"),&vTmp);
				MinLength = vtof(vTmp);
				//������󳤶�
				rs->get_Collect((_variant_t)_T("maxLength"),&vTmp);
				MaxLength = vtof(vTmp);
				//���������������1������������С����200��ʼ����Ϊ100,
				//ֱ��1000��������Ϊ500��ֱ����󳤶�2000ֹ��
				//��IncrementStart1=200,IncrementLength1=100
				//��IncrementStart2=1000,IncrementLength2=500
				rs->get_Collect((_variant_t)_T("IncrementStart1"),&vTmp);
				if( vtof(vTmp) <= MinLength )
				{
					IncrementStart1 = MinLength;
				}
				else
				{
					IncrementStart1 = vtof(vTmp);
				}
				rs->get_Collect((_variant_t)_T("IncrementLength1"),&vTmp);
				IncrementLength1 = vtof(vTmp);
				//���������������2�������1000��ʼ����Ϊ500��
				rs->get_Collect((_variant_t)_T("IncrementStart2"),&vTmp);
				if( vtof(vTmp) <= MinLength )
				{
					IncrementStart2 = MaxLength;
				}
				else
				{
					IncrementStart2 =vtof(vTmp);
				}
				rs->get_Collect((_variant_t)_T("IncrementLength2"),&vTmp);
				if( vtof(vTmp) >= MaxLength - MinLength )
				{
					IncrementLength2 = 0;
				}
				else 
				{
					IncrementLength2 = vtof(vTmp);
				}
				if( IncrementStart2 == MaxLength )
				{
					IncrementLength2 = 0;
				}
         
				//�ó�ʼֵtempMinLength=0
				tempMinLength = 0;
				if( i >= 1 )
				{//4
					if( Ptype[i - 1] == _T("T3A") )
					{//1
						//ǰһ�����·�����
						rsTZB->get_Collect((_variant_t)_T("A"),&vTmp);
						tempMinLength = fabs(tempT3A) + vtof(vTmp);
						/*if( mlPartIndex[i - 2] = iPA && i - 2 >= 0 ){
							//ǰһ�����·����ɣ���ǰһ���ǹܲ�
							//���ϲ۸ֵĸ߶�
						}else {
							//ǰһ�����·����ɣ���ǰһ�����ǹܲ�
							//������
						}*/
					}//1
					else {//1
						//ǰһ�������·�����
						/*if( mlPartIndex[i - 1] == iPA ){
							//ǰһ�������·�����,�ǹܲ�
						}else {
							//ǰһ�������·�����,���ǹܲ�
							if( Ptype(i - 1) Like _T("L[5-8]") ){
								//ǰһ�������·�����,���ǹܲ������������Ӽ�
							}else {
							}
						}*/
						rsTZB->get_Collect((_variant_t)_T("A"),&vTmp);
						tempMinLength = vtof(vTmp);
					}//1
            
					if( i + 1 <= iTZBRcount - 1 )
					{//3
						if( Ptype[i + 1] == _T("T3") )
						{//2
							//��һ�����Ϸ�����
							if( mlPartIndex[i + 2] == iSA && i + 2 <= iTZBRcount - 1 )
							{//1
								//��һ�����Ϸ�����,�ٺ�һ���Ǹ���
								rsTZB->get_Collect((_variant_t)_T("B"),&vTmp);
								tempMinLength = tempMinLength + fabs(tempT3) + vtof(vTmp) + Cavphs->SACSHeight;
							}//1
							//else {
								//��һ�����Ϸ�����,�ٺ�һ�����Ǹ���
                     
							//}
						}//2
						else
						{//2
							//��һ�������Ϸ�����
							if( mlPartIndex[i + 1] == iSA )
							{//1
								//��һ�������Ϸ�����,��һ���Ǹ���
								rsTZB->get_Collect((_variant_t)_T("C"),&vTmp);
								tempMinLength = tempMinLength + vtof(vTmp) + Cavphs->SACSHeight;
							}//1
							else
							{//1
								//��һ�������Ϸ�����,��һ�����Ǹ���
								/*if( Ptype(i + 1) Like _T("L[5-8]") ){
									//��һ�������Ϸ�����,���Ǹ��������������Ӽ�
                        
								}else {
									//��һ�������Ϸ�����,���Ǹ����������������Ӽ�
                        
								}*/
								rsTZB->get_Collect((_variant_t)_T("C"),&vTmp);
								tempMinLength = tempMinLength + vtof(vTmp);
							}//1
						}//2
					}//3
				}//4
				tempMinLength = (tempMinLength > MinLength ? tempMinLength : MinLength);
				//��С�ߴ���ʱ����������size�ֶ�
				rsTZB->put_Collect((_variant_t)_T("size"),_variant_t((float)tempMinLength)); rsTZB->Update();
				rs->Close();
			}//5��ǰ������(end)
			rsTZB->MoveNext();
		}     
		//���㲢�ҷ���ÿ�����˳���
		SQLx = _T("SELECT * FROM [") + EDIBgbl::Btype[EDIBgbl::TZB] + _T("] WHERE zdjh=") + ltos(zdjh) + _T(" AND VolumeID=") + ltos(EDIBgbl::SelVlmID) + _T(" AND NOT IsNull(ClassID) AND ( ClassID=") + ltos(iROD) + _T(" OR ClassID=") + ltos(iXL1) + _T(") AND NOT IsNull(SEQ) ORDER BY C DESC");
		rsTmp->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPRJDB,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
		rsTmp->MoveLast();
		rsTmp->MoveFirst();
		iNumRod = 1;
		for( i = 0 ;i< rsTmp->RecordCount ;i++)
		{
				//�����������
				rsTmp->get_Collect((_variant_t)_T("CustomID"),&vTmp);
				SQLx = _T("SELECT * FROM [") + tbnLugInfo + _T("] WHERE (CustomID)=\'") + vtos(vTmp) + _T("\'");
				rsTmp->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
					adOpenDynamic, adLockOptimistic, adCmdText); 
				if( rs->BOF && rs->adoEOF )
				{
					sTmp.Format(GetResStr(IDS_NoLugCustomIDInLugINFO),EDIBgbl::GetDBName(dbZDJcrude),tbnLugInfo,SQLx);
					throw sTmp;
				}
				rs->MoveFirst();
				//������С����
				rs->get_Collect((_variant_t)_T("minLength"),&vTmp);
				MinLength = vtof(vTmp);
				rs->get_Collect((_variant_t)_T("maxLength"),&vTmp);
				//������󳤶�
				MaxLength =vtof(vTmp);
				//���������������1������������С����200��ʼ����Ϊ100,
				//ֱ��1000��������Ϊ500��ֱ����󳤶�2000ֹ��
				//��IncrementStart1=200,IncrementLength1=100
				//��IncrementStart2=1000,IncrementLength2=500
				rs->get_Collect((_variant_t)_T("IncrementStart1"),&vTmp);
				if( vtof(vTmp)<= MinLength )
				{
					IncrementStart1 = MinLength;
				}
				else
				{
					IncrementStart1 = vtof(vTmp);
				}
				rs->get_Collect((_variant_t)_T("IncrementLength1"),&vTmp);
				IncrementLength1 = vtof(vTmp);
				//���������������2�������1000��ʼ����Ϊ500��
				rs->get_Collect((_variant_t)_T("IncrementStart2"),&vTmp);
				if(  vtof(vTmp)<= MinLength ){
					IncrementStart2 = MaxLength;
				}else {
					IncrementStart2 = vtof(vTmp);
				}
				rs->get_Collect((_variant_t)_T("IncrementLength2"),&vTmp);
				if( vtof(vTmp) >= MaxLength - MinLength ){
					IncrementLength2 = 0;
				}else {
					IncrementLength2 = vtof(vTmp);
				}
				if( IncrementStart2 == MaxLength ){ //2007.09.14 "="��Ϊ"=="
					IncrementLength2 = 0;
				}
         
				//ReDim Preserve RodLen(iNumRod)
				//����ʹ����󳤶ȵ����ˣ�ͬʱ
				//ʹÿ�ڵĳ��Ⱦ����ӽ�ƽ�����ȡ�
				sngLugAvgLen = (iSumRodLength - sngTmpSumLen) / (giNumRod - iNumRod + 1);
				//��סÿ�����˵���С������
				rsTmp->get_Collect((_variant_t)_T("size"),&vTmp);
				tempMinLength = vtof(vTmp);
				if( iNumRod != giNumRod )
				{
				//ʹÿ�ڵĳ��Ⱦ����ӽ�ƽ�����ȡ�
				//��Ҫһ��ѭ�����ԣ�����С���ȿ�ʼֱ��������ƽ������
         
					while( RodLen[iNumRod] < sngLugAvgLen || RodLen[iNumRod] < tempMinLength)
					{//4
						if( RodLen[iNumRod] < MinLength )
						{//1
							RodLen[iNumRod] = MinLength;
						}//1
						else 
						{//3
							if( RodLen[iNumRod] > MaxLength )
							{//1
								sTmp.Format(GetResStr(IDS_LugLengthBiggerMaxLength), ftos(RodLen[iNumRod]),ftos(MaxLength));
								throw sTmp;
							}//1
							else 
							{//2
								if( RodLen[iNumRod] >= IncrementStart2 )//2007.09.14 ">"��Ϊ">="
								{//1
									//�����һ����Ҫ����ƽ�����ȣ�����Ҫ������С�����ȣ����˳���
									if( RodLen[iNumRod] + IncrementLength2 / (giNumRod - iNumRod + 1) > sngLugAvgLen && RodLen[iNumRod] >= tempMinLength )
										break;
									RodLen[iNumRod] = RodLen[iNumRod] + IncrementLength2;
									//2007.09.14(start)
									if( RodLen[iNumRod] >=(long)MaxLength )
									{
										break;
									}
									//2007.09.14(end)
								}//1
								else 
								{//1
									//�����һ����Ҫ����ƽ�����ȣ�����Ҫ������С�����ȣ����˳���
									if( RodLen[iNumRod] + IncrementLength1 / (giNumRod - iNumRod + 1) > sngLugAvgLen && RodLen[iNumRod] >= tempMinLength )
										break;
									RodLen[iNumRod] = RodLen[iNumRod] + IncrementLength1;
								}//1
							}//2
						}//3
					}//4
					sngTmpSumLen = sngTmpSumLen + RodLen[iNumRod];
				}
				else 
				{
					//���һ�ڳ���=ʣ��
					RodLen[iNumRod] = iSumRodLength - sngTmpSumLen;
					if( RodLen[iNumRod] < MinLength )
					{
						//���˳���̫�̻���˽���̫��
						if( RodLen[iNumRod] >= giMinRodLen )
						{
							//������˳���>=100
							RodLen[iNumRod] = RodLen[iNumRod];
						}
						else
						{
							sTmp.Format(GetResStr(IDS_LugLengthLessthanMinLength),ltos(sngLugAvgLen),ltos(MinLength));
							throw sTmp;
						}
					}
					else 
					{
						if( RodLen[iNumRod] > MaxLength )
						{
							//���˳���̫������˽���̫��
							sTmp.Format(GetResStr(IDS_LugLengthBiggerMaxLength),ltos(sngLugAvgLen),ltos(MaxLength));
							throw sTmp;
						}
					}
				}

				//������˹��->sBH
				rsTmp->get_Collect((_variant_t)_T("CustomID"),&vTmp);
				sBHFormat = vtos(sFindFLD(_T("CustomID"), _T("BHformat"), vtos(vTmp)));
				rsTmp->get_Collect((_variant_t)_T("CLgg"),&vTmp);
				sBH = vtos(vTmp);
				SQLx = _T("UPDATE tmpCSLen SET BH=\'") + vtos(vTmp) + _T("\'");
				EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
				_variant_t vBH = vTmp;

				SQLx = _T("UPDATE tmpCSLen SET L1=") + ftos(RodLen[iNumRod]);
				EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
				mvL = CSLength(sBHFormat, _T("L1"), sBH);
				rsTmp->get_Collect((_variant_t)_T("recno"),&vTmp);
				_variant_t vTmp3;
				_variant_t vTmp;
				rsTZB->Find((_bstr_t)(_T("recno=") + vtos(vTmp)), 0, adSearchForward);
				if(!rsTZB->adoEOF)
				{
					rsTZB->get_Collect((_variant_t)_T("GDW1"),&vTmp);
					rsTZB->get_Collect((_variant_t)_T("CLnum"),&vTmp1);
					rsTmp->get_Collect((_variant_t)_T("B"),&vTmp2);
					rsTmp->get_Collect((_variant_t)_T("A"),&vTmp3);
					rsTZB->put_Collect((_variant_t)_T("BH"),vBH);
					
					//2007.09.12(start)
					if( iNumRod<giNumRod )
					{
						iSumRodLength1-=RodLen[iNumRod]-vtof(vTmp3)-vtof(vTmp2);
					}
					else if( iNumRod==giNumRod )
					{
						if(iSumRodLength1 != RodLen[iNumRod] - vtof(vTmp3)-vtof(vTmp2))
						{
							RodLen[iNumRod] = (int)iSumRodLength1+vtof(vTmp3)+vtof(vTmp2);
							//��������Բ��ʱ��β��Ϊ9ʱ����װͼ����1���ף�β��Ϊ1ʱ��1����
							//�������´��� modify by ligb luorijin   2008.10.16
							//iSumRodLength1��Float�ͣ�ȡ��֮��ͻ�����˵�ʵ�ʳ��ȵ�С������ȥ������������Բ��ֵ���Ǳ�ʵ��ֵ��1
							if (RodLen[iNumRod]%10 == 9||iSumRodLength1 - (int)iSumRodLength1 >= 0.1)
							{
								RodLen[iNumRod] = RodLen[iNumRod] + 1;
							}
							else if(RodLen[iNumRod]%10 == 1)
							{
								RodLen[iNumRod] = RodLen[iNumRod]-1;
							}
							
						}
						SQLx = _T("UPDATE tmpCSLen SET L1=") + ftos(RodLen[iNumRod]);
						EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx, NULL, adCmdText);
						mvL = CSLength(sBHFormat, _T("L1"), sBH);
					}
					//2007.09.12(end)

					rsTZB->put_Collect((_variant_t)_T("sizeH"),_variant_t((double)RodLen[iNumRod]));
					//��ע�ߴ�=ʵ�ʳ���-���˵�������
					rsTZB->put_Collect((_variant_t)_T("sizeDIM"),_variant_t((long)(RodLen[iNumRod] - vtof(vTmp3)-vtof(vTmp2))));
					//������Ϊ����������嵥��. ����/m��Cphs.GetPhsBHandSizes����������rsTzb.Fields(_T("GDW1"))
					//gbConnectHoleDimMatch
					rsTZB->put_Collect((_variant_t)_T("L1"),_variant_t((float)RodLen[iNumRod]));
					rsTZB->put_Collect((_variant_t)_T("CLdz"),_variant_t((float)(RodLen[iNumRod] / 1000.0 * vtof(vTmp))));					
					rsTZB->put_Collect((_variant_t)_T("CLzz"),_variant_t((float)(RodLen[iNumRod] / 1000.0 * vtof(vTmp) * vtof(vTmp1))));
					rsTZB->put_Collect((_variant_t)_T("CLgg"),STR_VAR(sBH));
					rsTZB->Update();
				}
				//Debug.Print RodLen(iNumRod)
				iNumRod++;
				rs->Close();
				rsTmp->MoveNext();
			}
			rsTmp->Close();
		}
		ret=true;
	}
	catch(CString e)
	{
		ShowMessage(e);

	}
	catch(CException e)
	{
		AfxMessageBox(_T("Other general error!"));
	}
	if(Ptype!=NULL)
		delete [] Ptype;
   if(mlPartClassID!=NULL)
		delete [] mlPartClassID;
   if(mlPartIndex!=NULL)
		delete [] mlPartIndex;
   if(mlPartDia!=NULL)
		delete [] mlPartDia;
   if(mlPartRecno!=NULL)
		delete [] mlPartRecno;
   if(mvActualLugExt!=NULL)
		delete [] mvActualLugExt;
   if(AllowedMinLugExt!=NULL)
		delete [] AllowedMinLugExt;
   if(AllowedMaxLugExt!=NULL)
		delete [] AllowedMaxLugExt;
   if(PreviousPartInsertLen!=NULL)
		delete [] PreviousPartInsertLen;
	return ret;
}

void modPHScal::SetBasePoint()
{
	try
	{
	   CalSpringParallelNum();
	   CString FDp, FDp1;
	   _RecordsetPtr rs;
	   rs.CreateInstance(__uuidof(Recordset));
	   CString SQLx;
	   SQLx = _T("SELECT * FROM phsDrawPos");
	   rs->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbPHScode,true), 
		   adOpenKeyset, adLockOptimistic, adCmdText); 
	   if (giDrawSIZE_A3A4 == 2)
	   {
		  //ͼ���Զ�
		  if( sngSEL <= sngPEL)
			 //֧�ܣ�A4
			 Ax = _T("A4");
		  else
			 //���ܣ�A3
			 Ax = _T("A3");
	   }
	   else
	   {
		  if( giDrawSIZE_A3A4 == 1)
			 //ǿ��ʹ��A4
			 Ax = _T("A4");
		  else
			 //ǿ��ʹ��A3
			 Ax = _T("A3");
	   }
	   if( bPAIsHanger() )
	   {
		  //����
		  if( gintParallelNum == 1)
			 //����
			 FDp1 = _T("1");
		  else if( gintParallelNum == 2)
				//˫��
				FDp1 = _T("2");
			 //Else
				//δ֪
			 //End If
	   }
	   else
	   {
		  //֧��
		  FDp1 = _T("S");
	   }
   
	   FDp = FDp1 + _T("_") + (gbDraw_BOM? _T("bom"): _T(""));
	   FDp.MakeUpper();
	   FDp.TrimLeft();
	   FDp.TrimRight();
	   _variant_t vTmp;

	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("VX_pt0x")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   VX_pt0x=vtof(vTmp);
	   }
   
	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("VX_pt0y")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {	   
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
			VX_pt0y=vtof(vTmp);
	   }
   
	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("VZ_pt0x")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   VZ_pt0x=vtof(vTmp);
	   }
   
	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("pt1x")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   pt1x=vtof(vTmp);
	   }
   
	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("pt1y")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   pt1y=vtof(vTmp);
	   }

	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("pt2x")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   pt2x=vtof(vTmp);
	   }
   
	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("pt2y")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   pt2y=vtof(vTmp);
	   }

	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("pt3x")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   pt3x=vtof(vTmp);
	   }
   
	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp) + (_T("pt3y")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   pt3y=vtof(vTmp);
	   }

	   rs->Find((_bstr_t)(_T("((name))=\'") + (FDp1) + _T("_") + (_T("pt0x")) + _T("\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   Pt0X=vtof(vTmp);
	   }

	   rs->Find((_bstr_t)(_T("((name))=\'GIDIMOFFSET\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   giDimOffset=vtof(vTmp);
	   }

	   rs->Find((_bstr_t)(_T("((name))=\'GIATTDOFFSETX\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   giAttDxOffsetX=vtof(vTmp);
	   }
	   giAttDzOffsetX = giAttDxOffsetX;

	   rs->Find((_bstr_t)(_T("((name))=\'GIATTDOFFSETY\'")), 0, adSearchForward);
	   if(!rs->adoEOF)
	   {
		   rs->get_Collect((_variant_t)(Ax+_T("Pos")),&vTmp);
		   giAttDOffsetY=vtof(vTmp);
	   }
	 }
	 catch (_com_error &e)
	 {
		 CString strMsg;
		 strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		 AfxMessageBox(strMsg);
	 }
}

void modPHScal::ReadAutoPHSiniValue()
{
	gbSumRodByRoundSteel = GetRegKey((_T("Settings")), _T("SumRodByRoundSteel"), 0);

	iAlignLoad=GetRegKey((_T("Settings")),(_T("CADAlignLoad")),0);
	iAlignDim=GetRegKey((_T("Settings")),(_T("CADAlignDim")),0);
	iAlignLjmx=GetRegKey((_T("Settings")),(_T("CADAlignLjmx")),0);
	strBigFontLoad=GetRegKey((_T("Settings")),(_T("CADBigFontLoad")),CString((_T("hzfs"))));
	strSHXFontLoad=GetRegKey((_T("Settings")),(_T("CADSHXFontLoad")),CString((_T("hzfs"))));
	fFWLoad=GetRegKey((_T("Settings")),(_T("CADFontWidthFactorLoad")),1.0f);

	strBigFontLjmx=GetRegKey((_T("Settings")),(_T("CADBigFontLjmx")),CString((_T("hzfs"))));
	strBigFontDim=GetRegKey((_T("Settings")),(_T("CADBigFontDim")),CString((_T("hzfs"))));
	fFWDim=GetRegKey((_T("Settings")),(_T("CADFontWidthFactorDim")),1.0f);
	strSHXFontLjmx=GetRegKey((_T("Settings")),(_T("CADSHXFontLjmx")),CString((_T("txt"))));
	strSHXFontDim=GetRegKey((_T("Settings")),(_T("CADSHXFontDim")),CString((_T("txt"))));
	fFWLjmx=GetRegKey((_T("Settings")),(_T("CADFontWidthFactorLjmx")),1.0f);

	gUseSpringMinDist = GetRegKey((_T("Settings")), _T("UseSpringMinDist"),float(0));
	gnSPR_CHZBH = GetRegKey((_T("Settings")), _T("SPR_Chzbh"), (float)0.35);
	gnSEQyOffset = GetRegKey((_T("Settings")), _T("SEQyOffset"), 8);
	gsngRatioOfPjg2Pgz = GetRegKey((_T("Settings")), _T("Pjg2Pgz"), (float)1.4);
	gnConnectTJ = GetRegKey((_T("Settings")), _T("ConnectTJ"), (float)200);
	gnRodLenRoundVal = GetRegKey((_T("Settings")), _T("RodLenRoundVal"), (int)100);
	gnRodInsertLenRoundVal = GetRegKey((_T("Settings")), _T("RodInsertLenRoundVal"), (int)1);
	if (gnRodLenRoundVal<1)
	{
		gnRodLenRoundVal=100;
		SetRegValue(_T("Settings"), _T("RodLenRoundVal"), gnRodLenRoundVal);
	}
	if (gnRodInsertLenRoundVal<1)
	{
		gnRodInsertLenRoundVal=1;
		SetRegValue(_T("Settings"), _T("RodInsertLenRoundVal"), gnRodInsertLenRoundVal);
	}
	
	gbConnectHoleDimMatch = GetRegKey((_T("Settings")), _T("ConnectHoleDimMatch"), -1);
	gbPipeClampBolt = GetRegKey((_T("Settings")), _T("PipeClampBolt"), -1);//Modified by Shuli Luo
	gbSPRAutoLugDia = GetRegKey((_T("Settings")), _T("SPRAutoLugDia"), -1);
	gbCSPRAutoLugDia = GetRegKey((_T("Settings")), _T("CSPRAutoLugDia"), 0);
	gbAutoSPRRodDiaDecrease=GetRegKey((_T("Settings")), _T("AutoSPRRodDiaDecrease"), 0);
	gbSPRBHLOADBYInstallLoad=GetRegKey((_T("Settings")), _T("SPRBHLOADBYInstallLoad"), 0);
	gbSPRBHLOADUNITBYNEWTON=GetRegKey((_T("Settings")), _T("SPRBHLOADUNITBYNEWTON"), 0);
	gbAutoApplyCSPRLugDia = GetRegKey((_T("Settings")), _T("AutoApplyCSPRLugDia"), -1);
	AutoOrdinateDrawing = GetRegKey((_T("Settings")), _T("AutoTZ"), true);
	gbStopOLE = GetRegKey(_T("Settings"), _T("StopOLE"), -1);
	gbAutoApplyChzbh = GetRegKey((_T("Settings")), _T("AutoApplyChzbh"), 0);
	gbOutputSA = GetRegKey((_T("Settings")),_T("OutputSA"), -1);
	gbAutoPASA = GetRegKey((_T("Settings")), _T("AutoPASA"), 0);
	gbCSPRCanSpecialDesign = GetRegKey((_T("Settings")), _T("CSPRCanSpecialDesign"), -1);
	gbLimitLugMinDia = GetRegKey((_T("Settings")), _T("LimitLugMinDia"), 0);
	gbRodDiaByMax = GetRegKey((_T("Settings")), _T("RodDiaByMax"), 1);
   //int gbTbOS;
	gbAutoAlignCSPRtoSA = GetRegKey((_T("Settings")), _T("AutoAlignCSPRtoSA"), -1);
	gbSumAllVolume = GetRegKey((_T("Settings")), _T("SumAllVolume"), -1);//
	EDIBgbl::gbTbOS = (bool)GetRegKey((_T("Settings")), _T("TbOS"), -1);
	gbCalSAbyConservativenessMethod = GetRegKey((_T("Settings")), _T("CalSAbyConservativenessMethod"), -1);
	gbCalSAbyPJG = GetRegKey((_T("Settings")), _T("CalSAbyPJG"), 0);
	gbCalCSPRLugDiaByPjg = GetRegKey((_T("Settings")), _T("CalCSPRLugDiaByPjg"), -1);
   
	giMinRodLen=GetRegKey((_T("Settings")), _T("MinRodLen"), 300);
	//gbCalPARTbyMaxLoad = GetRegKey((_T("Settings")), _T("CalPARTbyMaxLoad"), 0);
	gbCalPARTbyMaxLoad = FALSE;
	gbAddPartWeight2PMax = GetRegKey((_T("Settings")), _T("AddPartWeight2PMax"), -1);
	gbAddPartWeight2CSPRPgz = GetRegKey((_T("Settings")), _T("AddPartWeight2CSPRPgz"), -1);
	gbAddPartWeight2SPRPgz = GetRegKey((_T("Settings")), _T("AddPartWeight2SPRPgz"), 0);//2007.07.30
	gbPAForceZero=GetRegKey((_T("Settings")),_T("PAForceZero"),0);//2007.09.25
	gbMaxPgzPaz = GetRegKey((_T("Settings")), _T("MaxPgzPaz"), 0);//2007.09.03
//	gbMinMaxDispByActualDisp = GetRegKey((_T("Settings")), _T("MinMaxDispByActualDisp"), -1);
	gbMinMaxDispByActualDisp = GetRegKey((_T("Settings")), _T("MinMaxDispByActualDisp"), 0);
//	gbCalcCSPRHeight_BySelDisp = GetRegKey((_T("Settings")), _T("CalcCSPRHeight_BySelDisp"), 0);
	gbCalcCSPRHeight_BySelDisp = GetRegKey((_T("Settings")), _T("CalcCSPRHeight_BySelDisp"), -1);
	gbNotPrompt_ACAD_SHIFT = GetRegKey((_T("Settings")), _T("NotPrompt_ACAD_SHIFT"), -1);
	gIsACAD = GetRegKey((_T("Settings")), _T("IsACAD"), 0);
	gbNotPrompt_Catlog_OVERWRITE = GetRegKey((_T("Settings")), _T("NotPrompt_Catlog_OVERWRITE"), -1);
	giPhsOutINFOTableType = GetRegKey((_T("Settings")), _T("PhsOutINFOTableType"), 1);
	giDrawSIZE_A3A4 = GetRegKey((_T("Settings")), _T("DrawSIZE_A3A4"), 2);
	giSpringOrder = GetRegKey((_T("Settings")), _T("SpringOrder"), (int)lngSOSerialNumMinSEQmin);
	gstrSpringOrder = GetRegKey(_T("Settings"), _T("strSpringOrder"), CString(_T("")));
	gstrSpringOrderSQL = GetRegKey((_T("Settings")), _T("strSpringOrderSQL"), CString(_T("")));
	gsPhsColor = GetRegKey((_T("Settings")), _T("PhsColor"),CString( _T("White")));
	gbDraw_BOM = GetRegKey((_T("Settings")), _T("Draw_BOM"), true);
	gbAnnotateCSPR = GetRegKey((_T("Settings")), _T("AnnotateCSPR"), -1);
	gbDrawOVERWRITE = GetRegKey((_T("Settings")), _T("Draw_OVERWRITE"), -1);
	gbWorksWithBoltsNuts = GetRegKey((_T("Settings")), _T("WorksWithBoltsNuts"), -1);
	gbWorksWithSABoltsNuts = GetRegKey((_T("Settings")), _T("WorksWithSABoltsNuts"), -1);
	gnDW_delta = GetRegKey((_T("Settings")), _T("DW_Delta"), (float)7);
   
	gsPhsArrangementDwgNo = GetRegKey((_T("Settings")), _T("phsArrangementDwgNo"), CString(_T("F4452S-J0501-02")));
	gsPhsScheduleDwgNo = GetRegKey((_T("Settings")), _T("phsScheduleDwgNo"), CString(_T("F4452S-J0501-03")));
	EDIBgbl::glCurRecordNo = (long)GetRegKey((_T("Settings")), _T("ZDJH"), 1);
	gbDrawNoPreMethod = GetRegKey((_T("Settings")), _T("DrawNoPreMethod"), 1);
	gbAutoSaveDrawing = GetRegKey((_T("Settings")), _T("AutoSaveDrawing"),0);
	glDrawNoStartNumber = GetRegKey((_T("Settings")), _T("DrawNoStartNumber"), 5);
	EDIBgbl::strDwgVersion = GetRegKey((_T("Settings")), _T("DwgVersion"), CString( _T("")));
	EDIBgbl::MaxLGLong = GetRegKey((_T("Settings")), _T("MaxLGLong"), 110);//pfg20050922

	glDWGPreMethod = GetRegKey((_T("Settings")), _T("DWGPreMethod"), 0);
	gsDWGPre = GetRegKey((_T("Settings")), _T("DWGPre"),CString( _T("")));
	bHintRepZdjType=GetRegKey((_T("Settings")),_T("HintRepZdjType"),false);
	gbNoFenQu=GetRegKey((_T("Settings")),_T("bNoFenQu"),false);
	gbTBhq=GetRegKey((_T("Settings")),_T("bTBhq"),true);
	gbNoWaiKuang=GetRegKey((_T("Settings")),_T("bNoWaiKuang"),false);
	//pfg20050531
	gbOrientationPicture=GetRegKey((_T("Settings")),_T("OrientationPicture"),false);
	
	iNumCSVal=GetRegKey((_T("Settings")),_T("NumCSVal"),0);

	gsPSAOutFilePath = GetRegKey((_T("Settings")), _T("PSAOutFilePath"),basDirectory::TemplateDir);
	g_iPSAFilterIndex = GetRegKey((_T("Settings")), _T("PSAOutFilterIndex"),(int)1);
	gsSelectProductMdb = GetRegKey((_T("Settings")), _T("SelectProductMdb"),basDirectory::TemplateDir);

	gbHotStatus2Zero = GetRegKey((_T("Settings")), _T("HotStatus2Zero"),-1);

	EDIBAcad::g_bBomSprFactory     = GetRegKey((_T("Settings")), _T("BomSprFactory"),0);
	EDIBAcad::g_bSumBomSprFactory  = GetRegKey((_T("Settings")), _T("SumBomSprFactory"),0);
	EDIBAcad::g_bLocationDimension = GetRegKey((_T("Settings")), _T("LocationDimension"),-1);
	EDIBAcad::g_bDLOADBJBW = GetRegKey((_T("Settings")), _T("DLOADBJBW"),0);
	EDIBAcad::g_bDrawNameWithoutNo = GetRegKey((_T("Settings")), _T("DrawNameWithoutNo"),0);

	EDIBAcad::g_bDrawSumWeight  = GetRegKey((_T("Settings")), _T("Draw_SumWeight"),-1);
	EDIBAcad::g_bGroupWare  = GetRegKey((_T("Settings")), _T("GroupWare"),0);

	CString str = CString();
	EDIBAcad::g_strGroupWareName = GetRegKey((_T("Settings")), _T("strGroupWareName"),str);

	EDIBAcad::g_bTagUsingCircle  = GetRegKey((_T("Settings")), _T("TagUsingCircle"),0);
	EDIBAcad::g_bCustomPicNo  = GetRegKey((_T("Settings")), _T("CustomPicNo"),0);

	EDIBAcad::g_bLJMX_BJBW  = GetRegKey((_T("Settings")), _T("LJMX_BJBW"),0);
	EDIBAcad::g_bBGForBJBW  = GetRegKey((_T("Settings")), _T("BGForBJBW"),0);
	EDIBAcad::g_bDWForBJBW  = GetRegKey((_T("Settings")), _T("DWForBJBW"),0);//������ʽ�Ķ�λ

	modPHScal::gfSeqCircleDiameter  = GetRegKey((_T("Settings")), _T("CircleDiameter"),6.0f);
	modPHScal::gfSeqTxtHeight = GetRegKey((_T("Settings")), _T("SeqTxtHeight"),5.5f);

	modPHScal::g_bDHT = GetRegKey((_T("Settings")), _T("DHT"),-1);
	modPHScal::g_strDHTGG = GetRegKey((_T("Settings")), _T("DHTGG"),CString(""));
	EDIBAcad::g_bNotDrawSingleWeight  = GetRegKey((_T("Settings")), _T("NotDrawSingleWeight"),-1);
	
	EDIBAcad::g_bInsertLength = GetRegKey((_T("Settings")),_T("InsertLength"),-1);

	EDIBAcad::g_bPrintSA_Value= GetRegKey((_T("Settings")),_T("PrintSA_Value"),-1);

	EDIBAcad::g_bInstallElevation = GetRegKey((_T("Settings")),_T("InstallElevation"),-1);
	EDIBAcad::g_bColdHeatState  = GetRegKey((_T("Settings")),_T("ColdHeatState"),-1);
	EDIBAcad::g_bHzabs  = GetRegKey((_T("Settings")),_T("HzAbs"),0);
	EDIBgbl::bSymbol = GetRegKey(_T("Settings"),_T("DiaSymbol"),1);
	
	EDIBAcad::g_fDistanceOffset  = GetRegKey((_T("Settings")),_T("DistanceOffset"),0.0f);
	EDIBAcad::g_fOvertopDim  = GetRegKey((_T("Settings")),_T("OvertopDim"),2.0f);
	EDIBAcad::g_bDimNumber = GetRegKey((_T("Settings")),_T("DimNumber"),1);

	EDIBAcad::g_fSetTKWidth = GetRegKey((_T("Settings")),_T("SetTKWidth"),0.3f);
	EDIBAcad::g_fSetTableWidth = GetRegKey((_T("Settings")),_T("SetTableWidth"),0.0f);

	//LFX 2005.6.30 �� �������ѡ��
	modPHScal::gbDrawWeldSymbol = GetRegKey((_T("Settings")),_T("DrawWeldSymbol"),-1);
	modPHScal::gbAutoWeldViewInsertPoint = GetRegKey((_T("Settings")),_T("AutoWeldViewInsertPoint"),-1);
	modPHScal::gbDrawWeldViewDim = GetRegKey((_T("Settings")),_T("DrawWeldViewDim"),-1);
	modPHScal::gbWeldAutoDirectAndPos = GetRegKey((_T("Settings")),_T("WeldAutoDirectAndPos"),-1);
	//END lfx 2005.6.30
}

void modPHScal::InitializeCrudeData(_RecordsetPtr rs, bool /*ByVal*/ bFrmTxsrLoaded)
{ 
	//long	 mZdjh;//�ڲ�֧���ܺ�
   //rs.Bookmark = rs.Bookmark
	if(rs==NULL)
		return;
	try{
		if (!rs->adoEOF && !rs->BOF)
		{
			zdjh = rs->GetCollect(_T("zdjh")).operator long();
		}
		//�����¾��������ջ�ռ�
		//rs.FindFirst _T("zdjh=") + zdjh
		_variant_t v1;
		v1=rs->GetCollect(_T("dn1"));
		CString stmp=vtos(v1);
		stmp.TrimLeft();
		stmp.TrimRight();
		if(stmp.GetLength()>0)
			dn = stmp;
      
		GetHeatDisplacement( rs);
		if(vtoi(rs->GetCollect(_T("iSelSampleID")))<=0)
		{
			iSelSampleID=Cavphs->GetPhsStructSampleID();
		}
		else
			iSelSampleID=rs->GetCollect(_T("iSelSampleID")).operator long();
			
		//�¾����˹ѭ��
		//iSelSampleID = Cavphs->GetPhsStructSampleID()
		//���÷����ʼֵ���Ա���û�м���ʱ�ܹ�ѡ�������ܲ�����
		if( df = _T(""))  df = _T("X");
		if( fx = _T("") ) fx = _T("P");
		if( gf = _T("") ) gf = _T("X");
   
		if( bFrmTxsrLoaded)
		{
			Cavphs->TZBResultObj = &FrmTxsr.m_pViewTxsr->m_LstPhsStructure;
			Cavphs->GetPhsStructFromTZB(zdjh);
		}
		clb.Format(_T("%2d"),zdjh+glDrawNoStartNumber-1);
	}
	catch(_com_error &e)
	{
		ReportError(e.Description(),__FILE__,__LINE__);
	}
}

void modPHScal::CalPAfixZ1Z2(CString dn, float tmpSelPJG, float tmpT0, int mvariNumPart, _RecordsetPtr rsX)
{
//Ŀ�ģ�����̶�֧��Z1��Z2
//ԭ��:�˹��̼��뵽GetPhsBHandSizes�У�����VB6.0���̳���64K��ʹ���б���ֹͣ��
//��ֻ�ܽ����GetPhsBHandSizes�Ƴ���
//�����Make���ɱ��������ͨ����
   //����һ��������ڼ���̶�֧�ܵĵ�����sngSAfixP
//    float sngSAfixP  ; //�̶�֧�ܵĵ�����sngSAfixP
//    float sngSAfixPl ; //��̬�̶�֧�ܵĵ�����sngSAfixPl
//    float sngSAfixPr ; //��̬�̶�֧�ܵĵ�����sngSAfixPr
//    float PAfixH  ; //�̶�֧�ܹܲ��߶�H,cm
//    float F1ToWx1 ;    //������͸ֺ��йص��������ݣ�������zdjcrudeXBDLY�Ĳ۸����Ա�SSteelPropertyCS��F1ToWx1�ֶ�
    float PAfixC1  ;   //�̶�֧��C1ֵ,mm
//    float mvBWidth  ;    //�̶�֧�ܸ������۸ּ��,cm,G47=������chdist�ֶ�ֵ��G48=TZA��A01�ֶ�ֵ
    float pnl, pnr ;  //�̶�֧��Z1�ܲ���˨��/��̬���ܵ�����Z2�ϳ���,kgf
    float ml, mr    ; //�̶�֧��Z2�ĺϳ�����,kgf.m
    float pxl, pyl, pzl, pxr,pyr, pzr;   //�̶�֧������̬���ܵ���,kgf
    float mxl, myl, mzl, mxr, myr, mzr;
    float Sigma0, Sigmat,Sigma100;
    float wk, wkl, wkr1, wkr2;  
    float sngCoef;   //�̶�֧�ܵ�ϵ��
    long k ;
	 //CString sXYZ;	//��¼����ϵת��ǰ�����귽��

    _Recordset* rsza;
	_RecordsetPtr rsTmp;
	rsTmp.CreateInstance(__uuidof(Recordset));
  
	rsza=FrmTxsr.m_pViewTxsr->m_ActiveRs;
	if(rsX->State == adStateOpen)
		rsX->Close();
	CString SQLx,sTmp,sTmp2,sTmp3;
	_variant_t vTmp,vTmp2,vTmp3;
	switch(glClassID)
	{
	case iPAfixZ1:
	   for( k = 0 ;k<= gnDW_delta;k++)
	   {
		SQLx.Format(_T("SELECT * FROM %s WHERE Dw>= %g AND Dw<= %g AND Pmax>=%g AND Tj>=%g AND (CustomID)=\'%s\' ORDER BY Dw,tj,Weight"),
			tbnPAfix,
			dj * (1 - k * 0.01) ,
			dj * (1 + k * 0.01),
			tmpSelPJG / mvariNumPart,
			tmpT0 ,
			dn);
			if(rsX->State == adStateOpen)
				rsX->Close();
			rsX->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
				adOpenStatic, adLockOptimistic, adCmdText); 
         if( rsX->BOF && rsX->adoEOF )
			{
         }
			else
			{
             //�ҵ�������k���磬�Ա��˳�ѭ��
             k=gnDW_delta+1;
         }
      }

		if(dj > 273)
		{
			sngCoef = 1;
		}
		else 
		{
			if(dj>=219)
			{
				sngCoef = 0.95;
			}
			else
			{
				if(dj>= 168 ) 
				{
					sngCoef = 0.83;
				}
				else
				{
					//if(dj< 168) 
					sngCoef = 0.7;					
				}
			}
		}

		if(rsX->State == adStateOpen)
			rsX->Close();
		rsX->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
       if( rsX->BOF && rsX->adoEOF )
	   {
		   sTmp.Format(GetResStr(IDS_NoThisZ1orZ2inCrudePAfix),dn);
			throw sTmp;
       }
	   else
	   {
          rsX->MoveFirst();
			rsX->get_Collect((_variant_t)_T("size5"),&vTmp);
          PAfixC1 = vtof(vTmp);
          
          if( vtos(rsza->GetCollect(_T("Unit_Force"))) == _T("N") )
		  {
             pxl = vtof(rsza->GetCollect(_T("pxl1"))) / EDIBgbl::kgf2N;
             pyl = vtof(rsza->GetCollect(_T("pyl1"))) / EDIBgbl::kgf2N;
             pzl = vtof(rsza->GetCollect(_T("pzl1"))) / EDIBgbl::kgf2N;
             pxr = vtof(rsza->GetCollect(_T("pxr1"))) / EDIBgbl::kgf2N;
             pyr = vtof(rsza->GetCollect(_T("pyr1"))) / EDIBgbl::kgf2N;
             pzr = vtof(rsza->GetCollect(_T("pzr1"))) / EDIBgbl::kgf2N;
          }
		  else
		  {
             pxl = vtof(rsza->GetCollect(_T("pxl1")));
             pyl = vtof(rsza->GetCollect(_T("pyl1")));
             pzl = vtof(rsza->GetCollect(_T("pzl1")));
             pxr = vtof(rsza->GetCollect(_T("pxr1")));
             pyr = vtof(rsza->GetCollect(_T("pyr1")));
             pzr = vtof(rsza->GetCollect(_T("pzr1")));
          }
          if( vtos(rsza->GetCollect(_T("Unit_Momental"))) == _T("N.m") )
		  {
             mxl = vtof(rsza->GetCollect(_T("mxl1"))) / EDIBgbl::kgf2N;
             myl = vtof(rsza->GetCollect(_T("myl1"))) / EDIBgbl::kgf2N;
             mzl = vtof(rsza->GetCollect(_T("mzl1"))) / EDIBgbl::kgf2N;
             mxr = vtof(rsza->GetCollect(_T("mxr1"))) / EDIBgbl::kgf2N;
             myr = vtof(rsza->GetCollect(_T("myr1"))) / EDIBgbl::kgf2N;
             mzr = vtof(rsza->GetCollect(_T("mzr1"))) / EDIBgbl::kgf2N;
          }
		  else
		  {
             mxl = vtof(rsza->GetCollect(_T("mxl1")));
             myl = vtof(rsza->GetCollect(_T("myl1")));
             mzl = vtof(rsza->GetCollect(_T("mzl1")));
             mxr = vtof(rsza->GetCollect(_T("mxr1")));
             myr = vtof(rsza->GetCollect(_T("myr1")));
             mzr = vtof(rsza->GetCollect(_T("mzr1")));
          }
          
		  //ʵ������ϵ����/����ת������������ϵ
		  if(giUPxyz==1)
		  {
			  //ʵ������ϵx������
			  if(df==_T("Z"))
			  {
				  //�ܲ�����ƽ��z��
				  pxlc=pzl;
				  pylc=-pxl;
				  pzlc=-pyl;
				  pxrc=pzr;
				  pyrc=-pxr;
				  pzrc=-pyr;

				  mxlc=mzl;
				  mylc=-mxl;
				  mzlc=-myl;
				  mxrc=mzr;
				  myrc=-mxr;
				  mzrc=-myr;
			  }
			  else
			  {
				  //�ܲ�����ƽ��y��
				  pxlc=pyl;
				  pylc=-pxl;
				  pzlc=pzl;
				  pxrc=pyr;
				  pyrc=-pxr;
				  pzrc=pzr;

				  mxlc=myl;
				  mylc=-mxl;
				  mzlc=mzl;
				  mxrc=myr;
				  myrc=-mxr;
				  mzrc=mzr;
			  }
		  }
		  else if(giUPxyz==2)
		  {
			  //ʵ������ϵy������
			  if(df==_T("Z"))
			  {
				  //�ܲ�����ƽ��z��,���ñ任����ϵ
				  pxlc=pzl;
				  pylc=-pyl;
				  pzlc=pxl;
				  pxrc=pzr;
				  pyrc=-pyr;
				  pzrc=pxr;

				  mxlc=mzl;
				  mylc=-myl;
				  mzlc=mxl;
				  mxrc=mzr;
				  myrc=-myr;
				  mzrc=mxr;
			  }
			  else
			  {
				  //�ܲ�����ƽ��x��
				  pxlc=-pxl;
				  pylc=-pyl;
				  pzlc=pzl;
				  pxrc=-pxr;
				  pyrc=-pyr;
				  pzrc=pzr;

				  mxlc=-mxl;
				  mylc=-myl;
				  mzlc=mzl;
				  mxrc=-mxr;
				  myrc=-myr;
				  mzrc=mzr;
			  }
		  }
		  else if(giUPxyz==3)
		  {
			  //ʵ������ϵz������
			  if(df==_T("Z"))
			  {
				  //�ܲ�����ƽ��y��,���ñ任����ϵ
				  pxlc=-pyl;
				  pylc=-pzl;
				  pzlc=pxl;
				  pxrc=-pyr;
				  pyrc=-pzr;
				  pzrc=pxr;

				  mxlc=-myl;
				  mylc=-mzl;
				  mzlc=mxl;
				  mxrc=-myr;
				  myrc=-mzr;
				  mzrc=mxr;
			  }
			  else
			  {
				  //�ܲ�����ƽ��x��
				  pxlc=-pxl;
				  pylc=-pzl;
				  pzlc=-pyl;
				  pxrc=-pxr;
				  pyrc=-pzr;
				  pzrc=-pyr;

				  mxlc=-mxl;
				  mylc=-mzl;
				  mzlc=-myl;
				  mxrc=-mxr;
				  myrc=-mzr;
				  mzrc=-myr;
			  }
		  }

		  //��Py����ʱ��Pyֵ�����ǡ�������Ժ֧�����ֲ�P44��by ligb on 2005.08.08
          if( pylc > 0 )
		  {
             pnl = pylc * (1 + dj / PAfixC1) / 4 + 1000 * mzlc / (2 * PAfixC1) / sngCoef;
          }
		  else
		  {
             pnl = 1000 * mzlc / (2 * PAfixC1) / sngCoef;
          }

          if( pyrc > 0 )
		  {
             pnr = pyrc * (1 + dj / PAfixC1) / 4 + 1000 * mzrc / (2 * PAfixC1) / sngCoef;
          }
		  else
		  {
             pnr = 1000 * mzrc / (2 * PAfixC1) / sngCoef;
          }

          rsX->get_Collect((_variant_t)_T("pmax"),&vTmp);
          if( fabs(pnl) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_FixSupportBoltsNutsLoadOverAllowedMaxLoad),GetResStr(IDS_frmtxsr_OptCoolStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,pnl, vtof(vTmp));
			  throw sTmp;
          }
          if( fabs(pnr) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_FixSupportBoltsNutsLoadOverAllowedMaxLoad),GetResStr(IDS_frmtxsr_OptHeatStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,pnr, vtof(vTmp));
			  throw sTmp;
          }
          rsX->get_Collect((_variant_t)_T("px0"),&vTmp);
          if( fabs(pxlc) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_XYZdirectionLoadTooLarger),_T("\%d"),_T("X"),GetResStr(IDS_frmtxsr_OptCoolStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,zdjh,pxlc, vtof(vTmp));
			  throw sTmp;
          }
          if( fabs(pxrc) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_XYZdirectionLoadTooLarger),_T("\%d"),_T("X"),GetResStr(IDS_frmtxsr_OptHeatStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,zdjh,pxrc, vtof(vTmp));
			  throw sTmp;
          }
          
          rsX->get_Collect((_variant_t)_T("pz0"),&vTmp);
          if( fabs(pzlc) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_XYZdirectionLoadTooLarger),_T("\%d"),_T("Z"),GetResStr(IDS_frmtxsr_OptCoolStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,zdjh,pzlc, vtof(vTmp));
			  throw sTmp;
          }
          if( fabs(pzrc) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_XYZdirectionLoadTooLarger),_T("\%d"),_T("Z"),GetResStr(IDS_frmtxsr_OptHeatStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,zdjh,pzrc, vtof(vTmp));
			  throw sTmp;
          }
          
          rsX->get_Collect((_variant_t)_T("mx0"),&vTmp);
          if( fabs(mxlc) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_XYZdirectionLoadTooLarger),_T("\%d"),_T("X"),GetResStr(IDS_frmtxsr_OptCoolStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,zdjh,mxlc, vtof(vTmp));
			  throw sTmp;
          }
          if( fabs(mxrc) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_XYZdirectionLoadTooLarger),_T("\%d"),_T("X"),GetResStr(IDS_frmtxsr_OptHeatStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,zdjh,mxrc, vtof(vTmp));
			  throw sTmp;
          }
          
          rsX->get_Collect((_variant_t)_T("my0"),&vTmp);
          if( fabs(mylc) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_XYZdirectionLoadTooLarger),_T("\%d"),_T("Y"),GetResStr(IDS_frmtxsr_OptCoolStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,zdjh,mylc, vtof(vTmp));
			  throw sTmp;
          }
          if( fabs(myrc) > vtof(vTmp) )
		  {
			  sTmp2.Format(GetResStr(IDS_XYZdirectionLoadTooLarger),_T("\%d"),_T("Y"),GetResStr(IDS_frmtxsr_OptHeatStatus),_T("\%g"),_T("\%g"));
			  sTmp.Format(sTmp2,zdjh,myrc, vtof(vTmp));
			  throw sTmp;
          }
          
		  rsX->get_Collect((_variant_t)_T("sizeH"),&vTmp);
          modPHScal::PAfixH = vtof(vTmp);
       }
	   break;
    case iPAfixZ2:
       if( !IsNull(rsza->GetCollect(_T("dcl1"))) )  gstrMaterial= vtos(rsza->GetCollect(_T("dcl1")));
		 sTmp=gstrMaterial;
		 sTmp.MakeLower();sTmp.TrimLeft();sTmp.TrimRight();
       if( sTmp==_T("st45.8"))
	   {
          gstrMaterial = _T("st45.8");
       }
	   else if(sTmp!=_T("20"))
	   {
          //MsgBox _T("ԭʼ������û���������ܲ���,�����ܲ������벻��!")
		   sTmp.Format(GetResStr(IDS_NullMaterialFieldOrNoThisMaterial),gstrMaterial);
		   ShowMessage(sTmp);
          gstrMaterial = _T("20");
       }
       for (k = 0 ;k<= gnDW_delta;k++)
	   {
			 //���򰴹ܾ�dw���¶�tj���ܲ����쿹���size7���ܲ�����߶�size4����������߶�size5��
			 //���򰴹ܾ�dw���¶�tj��֧���⾶sizeC���ܲ�����߶�size4����������߶�size5��
			 //���������ߵȼۣ�������Ӧ�Ը��á��绪��Ժ���ṩsize7�ֶ�ֵ���������㡣
          SQLx.Format(_T("SELECT * FROM %s WHERE Dw>= %g AND Dw<= %g AND Tj>=%g AND (Material)=\'%s\' AND (CustomID)=\'%s\' ORDER BY Dw,tj,sizeC,size4,size5,Weight"),
				tbnPAfix,
				dj * (1 - k * 0.01),
				dj * (1 + k * 0.01),
				tmpT0,
				gstrMaterial,
				dn);
          if(rsX->State == adStateOpen)
				rsX->Close();
		  rsX->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
			  adOpenStatic, adLockOptimistic, adCmdText); 
          if( rsX->BOF && rsX->adoEOF )
			{
          }
		  else
		  {
             //�ҵ����˳�ѭ��
             k=gnDW_delta+1;
          }
       }
       if(dj > 273) 
		   sngCoef = 1;
       else if(dj>=219 && dj<= 273) 
		   sngCoef = 0.95;
       else if(dj>= 168 && dj<=  219) 
		   sngCoef = 0.83;
       if(dj< 168) 
		   sngCoef = 0.7;
	  if(rsX->State == adStateOpen)
		  rsX->Close();
	  rsX->Open((_bstr_t)SQLx, _variant_t((IDispatch*)dbZDJcrude,true), 
		  adOpenStatic, adLockOptimistic, adCmdText); 
       if( rsX->BOF && rsX->adoEOF )
	   {
		   sTmp.Format(GetResStr(IDS_NoThisZ1orZ2inCrudePAfix),dn);
			throw sTmp;
       }
	   else
	   {
          rsX->MoveFirst();
          
          if( vtos(rsza->GetCollect(_T("Unit_Force"))) == _T("N") )
		  {
             pxl = vtof(rsza->GetCollect(_T("pxl1"))) / EDIBgbl::kgf2N;
             pyl = vtof(rsza->GetCollect(_T("pyl1"))) / EDIBgbl::kgf2N;
             pzl = vtof(rsza->GetCollect(_T("pzl1"))) / EDIBgbl::kgf2N;
             pxr = vtof(rsza->GetCollect(_T("pxr1"))) / EDIBgbl::kgf2N;
             pyr = vtof(rsza->GetCollect(_T("pyr1"))) / EDIBgbl::kgf2N;
             pzr = vtof(rsza->GetCollect(_T("pzr1"))) / EDIBgbl::kgf2N;
          }
		  else
		  {
             pxl = vtof(rsza->GetCollect(_T("pxl1")));
             pyl = vtof(rsza->GetCollect(_T("pyl1")));
             pzl = vtof(rsza->GetCollect(_T("pzl1")));
             pxr = vtof(rsza->GetCollect(_T("pxr1")));
             pyr = vtof(rsza->GetCollect(_T("pyr1")));
             pzr = vtof(rsza->GetCollect(_T("pzr1")));
          }
          if( vtos(rsza->GetCollect(_T("Unit_Momental"))) == _T("N.m") )
		  {
             mxl = vtof(rsza->GetCollect(_T("mxl1"))) / EDIBgbl::kgf2N;
             myl = vtof(rsza->GetCollect(_T("myl1"))) / EDIBgbl::kgf2N;
             mzl = vtof(rsza->GetCollect(_T("mzl1"))) / EDIBgbl::kgf2N;
             mxr = vtof(rsza->GetCollect(_T("mxr1"))) / EDIBgbl::kgf2N;
             myr = vtof(rsza->GetCollect(_T("myr1"))) / EDIBgbl::kgf2N;
             mzr = vtof(rsza->GetCollect(_T("mzr1"))) / EDIBgbl::kgf2N;
          }
		  else
		  {
             mxl = vtof(rsza->GetCollect(_T("mxl1")));
             myl = vtof(rsza->GetCollect(_T("myl1")));
             mzl = vtof(rsza->GetCollect(_T("mzl1")));
             mxr = vtof(rsza->GetCollect(_T("mxr1")));
             myr = vtof(rsza->GetCollect(_T("myr1")));
             mzr = vtof(rsza->GetCollect(_T("mzr1")));
          }
          
		  //ʵ������ϵ����/����ת������������ϵ
		  //��ʼ������ԺZ2/Z2A����ϵ�任
		  if(dn==_T("Z2") || dn==_T("Z2A"))
		  {
			  if(giUPxyz==1)
			  {
				  //ʵ������ϵx������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��z��
					  pxlc=pzl;
					  pylc=-pxl;
					  pzlc=-pyl;
					  pxrc=pzr;
					  pyrc=-pxr;
					  pzrc=-pyr;

					  mxlc=mzl;
					  mylc=-mxl;
					  mzlc=-myl;
					  mxrc=mzr;
					  myrc=-mxr;
					  mzrc=-myr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��y��
					  pxlc=pyl;
					  pylc=-pxl;
					  pzlc=pzl;
					  pxrc=pyr;
					  pyrc=-pxr;
					  pzrc=pzr;

					  mxlc=myl;
					  mylc=-mxl;
					  mzlc=mzl;
					  mxrc=myr;
					  myrc=-mxr;
					  mzrc=mzr;
				  }
			  }
			  else if(giUPxyz==2)
			  {
				  //ʵ������ϵy������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��z��,���ñ任����ϵ
					  pxlc=pzl;
					  pylc=-pyl;
					  pzlc=pxl;
					  pxrc=pzr;
					  pyrc=-pyr;
					  pzrc=pxr;

					  mxlc=mzl;
					  mylc=-myl;
					  mzlc=mxl;
					  mxrc=mzr;
					  myrc=-myr;
					  mzrc=mxr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��x��
					  pxlc=-pxl;
					  pylc=-pyl;
					  pzlc=pzl;
					  pxrc=-pxr;
					  pyrc=-pyr;
					  pzrc=pzr;

					  mxlc=-mxl;
					  mylc=-myl;
					  mzlc=mzl;
					  mxrc=-mxr;
					  myrc=-myr;
					  mzrc=mzr;
				  }
			  }
			  else if(giUPxyz==3)
			  {
				  //ʵ������ϵz������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��y��,���ñ任����ϵ
					  pxlc=-pyl;
					  pylc=-pzl;
					  pzlc=pxl;
					  pxrc=-pyr;
					  pyrc=-pzr;
					  pzrc=pxr;

					  mxlc=-myl;
					  mylc=-mzl;
					  mzlc=mxl;
					  mxrc=-myr;
					  myrc=-mzr;
					  mzrc=mxr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��x��
					  pxlc=-pxl;
					  pylc=-pzl;
					  pzlc=-pyl;
					  pxrc=-pxr;
					  pyrc=-pzr;
					  pzrc=-pyr;

					  mxlc=-mxl;
					  mylc=-mzl;
					  mzlc=-myl;
					  mxrc=-mxr;
					  myrc=-mzr;
					  mzrc=-myr;
				  }
			  }
			  //��������Ժ����ϵ�任

			  //��ʼ����ԺZ2/Z2A���Ҽ���
				 //�䡢��̬�ϳ�����,��λ:kgf
				 pnl = sqrt(pxlc * pxlc + pzlc * pzlc);
				 pnr = sqrt(pxrc * pxrc + pzrc * pzrc);
				 //�䡢��̬�ϳ�������,��λ:kgf.m
				 ml = sqrt(mxlc * mxlc + mylc * mylc + mzlc * mzlc);
				 mr = sqrt(mxrc * mxrc + myrc * myrc + mzrc * mzrc);

				 //��̬�����洢��pmax�ֶ�
				//��̬�����ش洢��mx0�ֶ�
				 //��̬�����ش洢��my0�ֶ�
				rsX->get_Collect((_variant_t)_T("sizeH"),&vTmp);//unit=mm
				modPHScal::PAfixH =vtof(vTmp);
				rsX->get_Collect((_variant_t)_T("size4"),&vTmp);//unit=mm
				modPHScal::gfPSThickness=vtof(vTmp);//�ְ���>=K,mm
				FrmTxsr.m_pViewTxsr->m_PagOption->m_fPSThickness=modPHScal::gfPSThickness;
				FrmTxsr.m_pViewTxsr->m_PagOption->UpdateData(FALSE);
				rsX->get_Collect((_variant_t)_T("Pmax"),&vTmp);//unit=kgf
				rsX->get_Collect((_variant_t)_T("mx0"),&vTmp2);//unit=kgf.m
				rsX->get_Collect((_variant_t)_T("my0"),&vTmp3);//unit=kgf.m
				if( pnl <= vtof(vTmp) && pnr <= vtof(vTmp) && ml <= vtof(vTmp2) && mr <= vtof(vTmp3) )
				{
					 //��������������㺸�쿹���						
				 }
			  else
			  {
				  //��������Ӧ��
					modPHScal::StressOfMaterial(gstrMaterial,modPHScal::t0,Sigmat);
					modPHScal::StressOfMaterial(gstrMaterial,100,Sigma100);
					modPHScal::StressOfMaterial(gstrMaterial,20,Sigma0);
					//��̬ʱ���Ϻ���(�ܲ�����)�����wkr1,�º���(��������)�����wkr2:unit=(cm)3
					wkr2 = (pnr * modPHScal::PAfixH/10 + mr*100) / (0.85 * Sigmat);
					wkr1 = (pnr * (dj/10) / 2 + mr*100) / (0.85 * Sigmat);
					//��̬ʱ���Ϻ���(�ܲ�����)�����wkl1,�º���(��������)�����wkl2
					//����Ժ�ֲ���Ϊ��ֻ̬Ҫ�����º��켴�ɡ���ΪmodPHScal::PAfixH=Dw/2+h>dj/2
					//wkl2 = (pnl * modPHScal::PAfixH/10 + ml*100) / (0.85 * Sigma0);
					//wkr1 = (pnl * dj/10 / 2 + ml*100) / (0.85 * Sigma0);
					wkl = (pnl * modPHScal::PAfixH/10 + ml*100) / (0.85 * Sigma0);

					//���쿹���wk������size7�ֶ�
					rsX->get_Collect((_variant_t)_T("size7"),&vTmp);
					wk=vtof(vTmp);
					if( wkr2 > wkr1 )
					{
						if( wkr2 > wkl )
						{
							//��̬����������Ҫ�Ŀ�������
							//wk=wkr2;
							_variant_t vTmp;
							rsX->Find((_bstr_t)(_T("size7>=")+ftos(wkr2)), 0, adSearchForward);
							if(!rsX->adoEOF)
							{
								//%s��֧����%s��̬ʱ���º���(��������)��Ҫ�Ŀ����%s>����ֵ%s
								sTmp.Format(GetResStr(IDS_SAWeldingAgainstMomental),ltos(zdjh),GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(wkr2),ftos(wk));
								throw sTmp;                
							}
						}
						else
						{
							if(wkr1<wkl)
							{
								//��̬����������Ҫ�Ŀ�������
								//wk=wkl;
								_variant_t vTmp;
								rsX->Find((_bstr_t)(_T("size7>=")+ftos(wkl)), 0, adSearchForward);
								if(!rsX->adoEOF)
								{
									//%s��֧����%s��̬ʱ���º���(��������)��Ҫ�Ŀ����%s>����ֵ%s
									sTmp.Format(GetResStr(IDS_SAWeldingAgainstMomental),ltos(zdjh),GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(wkl),ftos(wk));
									throw sTmp;                
								}
							}
							else
								//������wkr2>wkr1 and wkr2<=wkl and wkl<=wkr1
								;		
						}
					}
					else
					{
						if( wkr2 > wkl )
						{
							//��̬�ܲ�������Ҫ�Ŀ�������
							//wk=wkr1;
							_variant_t vTmp;
							rsX->Find((_bstr_t)(_T("size7>=")+ftos(wkr1)), 0, adSearchForward);
							if(!rsX->adoEOF)
							{
								//%s��֧����%s��̬ʱ���Ϻ���(�ܲ�����)��Ҫ�Ŀ����%s>����ֵ%s
								sTmp.Format(GetResStr(IDS_PAWeldingAgainstMomental),ltos(zdjh),GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(wkr1),ftos(wk));
								throw sTmp;                
							}
						}
						else
						{
							if(wkr1<wkl)
							{
								//wkr2<=wkr1 and wkr2<=wkl and wkr1<wkl
								//��̬����������Ҫ�Ŀ�������
								//wk=wkl;
								_variant_t vTmp;
								rsX->Find((_bstr_t)(_T("size7>=")+ftos(wkl)), 0, adSearchForward);
								if(!rsX->adoEOF)
								{
									//%s��֧����%s��̬ʱ���º���(��������)��Ҫ�Ŀ����%s>����ֵ%s
									sTmp.Format(GetResStr(IDS_SAWeldingAgainstMomental),ltos(zdjh),GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(wkl),ftos(wk));
									throw sTmp;                
								}
							}
							else
							{
								//wkr2<=wkr1 and wkr2<=wkl and wkr1>=wkl
								//��̬�ܲ�������Ҫ�Ŀ�������
								_variant_t vTmp;
								rsX->Find((_bstr_t)(_T("size7>=")+ftos(wkr1)), 0, adSearchForward);
								if(!rsX->adoEOF)
								{
									//%s��֧����%s��̬ʱ���Ϻ���(�ܲ�����)��Ҫ�Ŀ����%s>����ֵ%s
									sTmp.Format(GetResStr(IDS_PAWeldingAgainstMomental),ltos(zdjh),GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(wkr1),ftos(wk));
									throw sTmp;                
								}
							}
						}
					}
				}
			  //��������ԺZ2/Z2A���Ҽ���
			}
			//��������ԺZ2/Z2A����ϵ�任�ͼ���

			else if(dn==_T("SZ5"))//�̶�֧��SZ5������Ժ����ϵ����
			{
				//��ʼ����ԺSZ5����任
			  if(giUPxyz==1)
			  {
				  //ʵ������ϵx������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��z��
					  pxlc=-pyl;
					  pylc=pxl;
					  pzlc=pzl;
					  pxrc=-pyr;
					  pyrc=pxr;
					  pzrc=pzr;

					  mxlc=-myl;
					  mylc=mxl;
					  mzlc=mzl;
					  mxrc=-myr;
					  myrc=mxr;
					  mzrc=mzr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��y��
					  pxlc=pzl;
					  pylc=pxl;
					  pzlc=-pyl;
					  pxrc=pzr;
					  pyrc=pxr;
					  pzrc=-pyr;

					  mxlc=mzl;
					  mylc=mxl;
					  mzlc=-myl;
					  mxrc=mzr;
					  myrc=mxr;
					  mzrc=-myr;
				  }
			  }
			  else if(giUPxyz==2)
			  {
				  //ʵ������ϵy������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��z��,���ñ任����ϵ
					  pxlc=pxl;
					  pylc=pyl;
					  pzlc=pzl;
					  pxrc=pxr;
					  pyrc=pyr;
					  pzrc=pzr;

					  mxlc=mxl;
					  mylc=myl;
					  mzlc=mzl;
					  mxrc=mxr;
					  myrc=myr;
					  mzrc=mzr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��x��
					  pxlc=pzl;
					  pylc=pyl;
					  pzlc=pxl;
					  pxrc=pzr;
					  pyrc=pyr;
					  pzrc=pxr;

					  mxlc=mzl;
					  mylc=myl;
					  mzlc=mxl;
					  mxrc=mzr;
					  myrc=myr;
					  mzrc=mxr;					 
				  }
			  }
			  else if(giUPxyz==3)
			  {
				  //ʵ������ϵz������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��y��,���ñ任����ϵ
					  pxlc=pxl;
					  pylc=pzl;
					  pzlc=-pyl;
					  pxrc=pxr;
					  pyrc=pzr;
					  pzrc=-pyr;

					  mxlc=mxl;
					  mylc=mzl;
					  mzlc=-myl;
					  mxrc=mxr;
					  myrc=mzr;
					  mzrc=-myr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��x��
					  pxlc=-pyl;
					  pylc=pzl;
					  pzlc=pxl;
					  pxrc=-pyr;
					  pyrc=pzr;
					  pzrc=pxr;

					  mxlc=-myl;
					  mylc=mzl;
					  mzlc=mxl;
					  mxrc=-myr;
					  myrc=mzr;
					  mzrc=mxr;
				  }
			  }
			  //��������ԺSZ5����任

			  //��ʼ����ԺSZ5���Ҽ���
				modPHScal::StressOfMaterial(gstrMaterial,modPHScal::t0,Sigmat);
				modPHScal::StressOfMaterial(gstrMaterial,100,Sigma100);
				modPHScal::StressOfMaterial(gstrMaterial,20,Sigma0);

					//����һ���ѯ
					//֧���⾶dw,mm
					rsX->get_Collect((_variant_t)_T("sizeC"),&vTmp);
					float dw=vtof(vTmp);
					//֧�ܱں�S,mm
					rsX->get_Collect((_variant_t)_T("size3"),&vTmp);
					float S=vtof(vTmp);
					//�Ϻ���(�ܲ��ຸ��)k1,mm
					rsX->get_Collect((_variant_t)_T("size4"),&vTmp);
					float k1=vtof(vTmp);
					//�º���(�����ຸ��)k2,mm
					rsX->get_Collect((_variant_t)_T("size5"),&vTmp);
					float k2=vtof(vTmp);			
					//�߶�H,mm
					rsX->get_Collect((_variant_t)_T("sizeH"),&vTmp);
					modPHScal::PAfixH=vtof(vTmp);			

					rsX->get_Collect((_variant_t)_T("size4"),&vTmp);//unit=mm
					modPHScal::gfPSThickness=vtof(vTmp);//�ְ���>=K,mm
					FrmTxsr.m_pViewTxsr->m_PagOption->m_fPSThickness=modPHScal::gfPSThickness;
					FrmTxsr.m_pViewTxsr->m_PagOption->UpdateData(FALSE);
			  
					bool bEOF=false;//û��β
					bool bErr=false;//���ޱ�־
				do
				{
				  //�ܲ��ຸ�쿹���W1=(pow(dw,4)-(dw-1.4*k1)*(dw-1.4*k1))*atan((double)1.0)*4/32/dw :unit,mm3
				  float W1=(pow(dw,4)-(dw-1.4*k1)*(dw-1.4*k1))*atan((double)1.0)*4/32/dw;
				  //�ܲ��ຸ�쿹Ť��Wp1=2*W1 :unit,mm3
				  float Wp1=2*W1;
				  //�ܲ��ຸ��Ľ����F1=(dw*dw-(dw-1.4*k1)*(dw-1.4*k1))*atan((double)1.0): unit,mm2
				  float F1=(dw*dw-(dw-1.4*k1)*(dw-1.4*k1))*atan((double)1.0);

				  //�̶�֧��SZ5�ܲ��ຸ��ļ��㹫ʽTAO1=sqrt(TAOy1*TAOy1+TAOxz1*TAOxz1)<=0.6*SIGMAjt
				  float TAOy1=1000*sqrt(mxrc*mxrc+mzrc*mzrc)/W1+fabs(pyrc)/F1; //kgf/mm2
				  float TAOxz1=sqrt(pxrc*pxrc+pzrc*pzrc)/F1+fabs(1000*myrc)/Wp1;//kgf/mm2
				  float TAO1=sqrt(TAOy1*TAOy1+TAOxz1*TAOxz1);
				  if(TAO1>0.6*Sigmat/100 )
				  {
					  if(bEOF)
					  {
							//%s��֧����%s��̬ʱ���Ϻ���(�ܲ�����)�ϳ�Ӧ��%s>����ֵ%s
							sTmp.Format(GetResStr(IDS_PAWeldingStress),ltos(zdjh),GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(TAO1),ftos(0.6*Sigmat/100));
							throw sTmp;                
					  }
					  else
							bErr=true;
				  }

				  TAOy1=1000*sqrt(mxlc*mxlc+mzlc*mzlc)/W1+fabs(pylc)/F1; //kgf/mm2
				  TAOxz1=sqrt(pxlc*pxlc+pzlc*pzlc)/F1+fabs(1000*mylc)/Wp1;//kgf/mm2
				  TAO1=sqrt(TAOy1*TAOy1+TAOxz1*TAOxz1);
				  if(TAO1>0.6*Sigma0/100 )
				  {
					  if(bEOF)
					  {
							//%s��֧����%s��̬ʱ���Ϻ���(�ܲ�����)�ϳ�Ӧ��%s>����ֵ%s
							sTmp.Format(GetResStr(IDS_PAWeldingStress),ltos(zdjh),GetResStr(IDS_frmtxsr_OptCoolStatus),ftos(TAO1),ftos(0.6*Sigma0/100));
							throw sTmp;
					  }
					  else
						  bErr=true;
				  }

				  //�����ຸ�쿹���W2=(pow(dw+1.4*k2,4)-pow(dw,4))*atan((double)1.0)*4/32/(dw+1.4*k2) :unit,mm3
				  float pi=atan((double)1.0)*4;
				  float W2=(pow(dw+1.4*k2,4)-pow(dw,4))*atan((double)1.0)*4/32/(dw+1.4*k2);
				  //�����ຸ�쿹Ť��Wp2=2*W2 :unit,mm3
				  float Wp2=2*W2;
				  //�����ຸ��Ľ����F2=((dw+1.4*k2)*(dw+1.4*k2)-dw*dw)*atan((double)1.0): unit,mm2
				  float F2=((dw+1.4*k2)*(dw+1.4*k2)-dw*dw)*atan((double)1.0);
				  //�̶�֧��SZ5�����ຸ��ļ��㹫ʽTAO2=sqrt(TAOy2*TAOy2+TAOxz2*TAOxz2)<=0.6*SIGMAj20
				  float TAOy2=sqrt((1000*mxrc+pzrc*modPHScal::PAfixH)*(1000*mxrc+pzrc*modPHScal::PAfixH)+(1000*mzrc-pzrc*modPHScal::PAfixH)*(1000*mzrc-pzrc*modPHScal::PAfixH))/W2+fabs(pyrc)/F2; //kgf/mm2
				  float TAOxz2=sqrt(pxrc*pxrc+pzrc*pzrc)/F2+fabs(1000*myrc)/Wp2;//kgf/mm2
				  float TAO2=sqrt(TAOy2*TAOy2+TAOxz2*TAOxz2);
				  if(TAO2>0.6*Sigma0/100 )
				  {
					  if(bEOF)
					  {
							//%s��֧����%s��̬ʱ���º���(��������)�ϳ�Ӧ��%s>����ֵ%s
							sTmp.Format(GetResStr(IDS_SAWeldingStress),ltos(zdjh),GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(TAO2),ftos(0.6*Sigma0/100));
							throw sTmp;
					  }
					  else
						  bErr=true;
				  }

				  TAOy2=sqrt((1000*mxlc+pzlc*modPHScal::PAfixH)*(1000*mxlc+pzlc*modPHScal::PAfixH)+(1000*mzlc-pzlc*modPHScal::PAfixH)*(1000*mzlc-pzlc*modPHScal::PAfixH))/W2+fabs(pylc)/F2; //kgf/mm2
				  TAOxz2=sqrt(pxlc*pxlc+pzlc*pzlc)/F2+fabs(1000*mylc)/Wp2;//kgf/mm2
				  TAO2=sqrt(TAOy2*TAOy2+TAOxz2*TAOxz2);
				  if(TAO2>0.6*Sigma0/100)
				  {
					  if(bEOF)
					  {
							//%s��֧����%s��̬ʱ���º���(��������)�ϳ�Ӧ��%s>����ֵ%s
							sTmp.Format(GetResStr(IDS_SAWeldingStress),ltos(zdjh),GetResStr(IDS_frmtxsr_OptHeatStatus),ftos(TAO2),ftos(0.6*Sigma0/100));
							throw sTmp;                
					  }
					  else
						  bErr=true;
				  }
				  
				  if(!bErr)
					  //û���ޣ��ҵ�
					  break;
				  else
				  {
						rsX->MoveNext();
						if(rsX->adoEOF)
						{
							//�Ѿ�����¼β������ѭ��һ�Σ��Ա㱨��
							bEOF=true;
							continue;
						}
						else
							//���¿�ʼ֮ǰ�����ó��ޱ�־
							bErr=false;

						//֧���⾶dw,mm
						rsX->get_Collect((_variant_t)_T("sizeC"),&vTmp);
						float dw=vtof(vTmp);
						//֧�ܱں�S,mm
						rsX->get_Collect((_variant_t)_T("size3"),&vTmp);
						float S=vtof(vTmp);
						//�Ϻ���(�ܲ��ຸ��)k1,mm
						rsX->get_Collect((_variant_t)_T("size4"),&vTmp);
						float k1=vtof(vTmp);
						//�º���(�����ຸ��)k2,mm
						rsX->get_Collect((_variant_t)_T("size5"),&vTmp);
						float k2=vtof(vTmp);			
						//�߶�H,mm
						rsX->get_Collect((_variant_t)_T("sizeH"),&vTmp);
						modPHScal::PAfixH=vtof(vTmp);	
						
						rsX->get_Collect((_variant_t)_T("size4"),&vTmp);//unit=mm
						modPHScal::gfPSThickness=vtof(vTmp);//�ְ���>=K,mm
						FrmTxsr.m_pViewTxsr->m_PagOption->m_fPSThickness=modPHScal::gfPSThickness;
						FrmTxsr.m_pViewTxsr->m_PagOption->UpdateData(FALSE);
				  }
				}while(true);

				//��������ԺSZ5���Ҽ���
			}//��������ԺSZ5����任�Ͳ��Ҽ���
			  

		  //�ܲ��Ѿ��������,���濪ʼ�̶�֧�ܸ���G47/G48��SJ8�ļ��㣺
		//����������ģ��GetPhsBHandSizes������ɡ�
		//��������ǰ��������ϵת����������������ϵ��
			//��ʼ����G47/G48�̶�֧����Ҫ�ĵ�����PmaxG47,kgf
		//��ʼ������ԺG47/G48����ϵ�任
		  if(gn==_T("G47") || gn==_T("G48"))
		  {
			  if(giUPxyz==1)
			  {
				  //ʵ������ϵx������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��z��
					  pxlc=pzl;
					  pylc=-pxl;
					  pzlc=-pyl;
					  pxrc=pzr;
					  pyrc=-pxr;
					  pzrc=-pyr;

					  mxlc=mzl;
					  mylc=-mxl;
					  mzlc=-myl;
					  mxrc=mzr;
					  myrc=-mxr;
					  mzrc=-myr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��y��
					  pxlc=pyl;
					  pylc=-pxl;
					  pzlc=pzl;
					  pxrc=pyr;
					  pyrc=-pxr;
					  pzrc=pzr;

					  mxlc=myl;
					  mylc=-mxl;
					  mzlc=mzl;
					  mxrc=myr;
					  myrc=-mxr;
					  mzrc=mzr;
				  }
			  }
			  else if(giUPxyz==2)
			  {
				  //ʵ������ϵy������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��z��,���ñ任����ϵ
					  pxlc=pzl;
					  pylc=-pyl;
					  pzlc=pxl;
					  pxrc=pzr;
					  pyrc=-pyr;
					  pzrc=pxr;

					  mxlc=mzl;
					  mylc=-myl;
					  mzlc=mxl;
					  mxrc=mzr;
					  myrc=-myr;
					  mzrc=mxr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��x��
					  pxlc=-pxl;
					  pylc=-pyl;
					  pzlc=pzl;
					  pxrc=-pxr;
					  pyrc=-pyr;
					  pzrc=pzr;

					  mxlc=-mxl;
					  mylc=-myl;
					  mzlc=mzl;
					  mxrc=-mxr;
					  myrc=-myr;
					  mzrc=mzr;
				  }
			  }
			  else if(giUPxyz==3)
			  {
				  //ʵ������ϵz������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��y��,���ñ任����ϵ
					  pxlc=-pyl;
					  pylc=-pzl;
					  pzlc=pxl;
					  pxrc=-pyr;
					  pyrc=-pzr;
					  pzrc=pxr;

					  mxlc=-myl;
					  mylc=-mzl;
					  mzlc=mxl;
					  mxrc=-myr;
					  myrc=-mzr;
					  mzrc=mxr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��x��
					  pxlc=-pxl;
					  pylc=-pzl;
					  pzlc=-pyl;
					  pxrc=-pxr;
					  pyrc=-pzr;
					  pzrc=-pyr;

					  mxlc=-mxl;
					  mylc=-mzl;
					  mzlc=-myl;
					  mxrc=-mxr;
					  myrc=-mzr;
					  mzrc=-myr;
				  }
			  }
			  //��������ԺG47/G48����ϵ�任

			else if(gn==_T("SJ8"))//�̶�֧�ܸ���SJ8������Ժ����ϵ����
			{
				//��ʼ����ԺSJ8����任
			  if(giUPxyz==1)
			  {
				  //ʵ������ϵx������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��z��
					  pxlc=-pyl;
					  pylc=pxl;
					  pzlc=pzl;
					  pxrc=-pyr;
					  pyrc=pxr;
					  pzrc=pzr;

					  mxlc=-myl;
					  mylc=mxl;
					  mzlc=mzl;
					  mxrc=-myr;
					  myrc=mxr;
					  mzrc=mzr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��y��
					  pxlc=pzl;
					  pylc=pxl;
					  pzlc=-pyl;
					  pxrc=pzr;
					  pyrc=pxr;
					  pzrc=-pyr;

					  mxlc=mzl;
					  mylc=mxl;
					  mzlc=-myl;
					  mxrc=mzr;
					  myrc=mxr;
					  mzrc=-myr;
				  }
			  }
			  else if(giUPxyz==2)
			  {
				  //ʵ������ϵy������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��z��,���ñ任����ϵ
					  pxlc=pxl;
					  pylc=pyl;
					  pzlc=pzl;
					  pxrc=pxr;
					  pyrc=pyr;
					  pzrc=pzr;

					  mxlc=mxl;
					  mylc=myl;
					  mzlc=mzl;
					  mxrc=mxr;
					  myrc=myr;
					  mzrc=mzr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��x��
					  pxlc=pzl;
					  pylc=pyl;
					  pzlc=pxl;
					  pxrc=pzr;
					  pyrc=pyr;
					  pzrc=pxr;

					  mxlc=mzl;
					  mylc=myl;
					  mzlc=mxl;
					  mxrc=mzr;
					  myrc=myr;
					  mzrc=mxr;					 
				  }
			  }
			  else if(giUPxyz==3)
			  {
				  //ʵ������ϵz������
				  if(df==_T("Z"))
				  {
					  //�ܲ�����ƽ��y��,���ñ任����ϵ
					  pxlc=pxl;
					  pylc=pzl;
					  pzlc=-pyl;
					  pxrc=pxr;
					  pyrc=pzr;
					  pzrc=-pyr;

					  mxlc=mxl;
					  mylc=mzl;
					  mzlc=-myl;
					  mxrc=mxr;
					  myrc=mzr;
					  mzrc=-myr;
				  }
				  else
				  {
					  //�ܲ�����ƽ��x��
					  pxlc=-pyl;
					  pylc=pzl;
					  pzlc=pxl;
					  pxrc=-pyr;
					  pyrc=pzr;
					  pzrc=pxr;

					  mxlc=-myl;
					  mylc=mzl;
					  mzlc=mxl;
					  mxrc=-myr;
					  myrc=mzr;
					  mzrc=mxr;
				  }
			  }
			  //��������ԺSJ8����ϵ�任
			 }			 
			}		
       }
		 break;
   }
}


CString modPHScal::GetPhsSAfx(int SAfx)
{
	if(SAfx<0 || SAfx>3)
		return _T("");
	return iPhsSAfx[SAfx];
}

bool modPHScal::tbExists(_ConnectionPtr db,_ConnectionPtr db1, CString &tbn, CString s1, CString s2,CString s3)
{
	CString sTmp;
	UINT lngErrNum;
	if( tbn.GetLength()<=0 || tbn == _T("") ){
      tbn = _T("");
      lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
	  sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),s1,s3,s2);
	ShowMessage(sTmp);
		return FALSE;
   }else
	{
		if( !EDIBgbl::tdfExists(db, tbn) )
	  {
			lngErrNum = IDS_NoThisXCrudeDataTableInZDJcrudeMdb;
			sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db),tbn);
			ShowMessage(sTmp);
			return FALSE;
      }
   }
   return TRUE;
}
bool modPHScal::HStbExists(_ConnectionPtr db/*Zdjcrude.mdb*/,_ConnectionPtr db1/*Sort.mdb*/, CString &tbn, 
						   CString s1, CString s2,CString s3,bool bWarn)
{
	CString sTmp;
	UINT lngErrNum;
	if( tbn.GetLength()<=0 || tbn == _T("") )
	{
		tbn = _T("");
		lngErrNum = IDS_NullXfieldInphsManuXOfsortMdb;
		sTmp.Format(GetResStr(lngErrNum),EDIBgbl::GetDBName(db1),s1,s3,s2);
		ShowMessage(sTmp);
		return FALSE;
	}
	else
	{
		if( !EDIBgbl::tdfExists(db,tbn) )
		{
			if (bWarn)
			{
				CString strWarn;
				strWarn.Format("����:     %s ���ݿ��� %s ��δ���ҵ�,\n(���������ݿ� %s �� %s ��� %s �ֶ���)\
					\n�������ݿ�Դ�ļ������д�!!! ���޸���ȷ���ٵ������ݿ�Ȼ��������ѡ��淶!\
					\nע��:  ��ѡ�Ĺ淶����������,ϵͳ���Զ��ӵ�ǰ���ù淶��\
					\nѡ��һ�����ʵĹ淶,���в���,����ѡ��������ȷ�Ĺ淶!!!",
					basDirectory::ProjectDBDir+_T("zdjcrude.mdb"),
					tbn,basDirectory::ProjectDBDir+_T("SORT"),s1,s2);
				AfxMessageBox(strWarn);
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL modPHScal::CalOffset(_RecordsetPtr rsZA)
{
	//Ŀ��:����ƫװֵ����Ϊ��2�ε��ã�����д�ɺ�����
	//�ܲ�������ƫװֵ���㣨�������ԡ��������������ܵ�֧��������P60��
	BOOL bResult = TRUE;
	if( rsZA == NULL )
		return FALSE;
	else
	{
		if( rsZA->adoEOF && rsZA->BOF )
			return FALSE;
	}
	CString sTmp;
	if( giUPxyz == 1 ){
		//x������,����ͼˮƽ������Ϊy��������
		gpzx = yl + 0.5 * yr1;
		gpzz = zl + 0.5 * zr;
		//���߻���������ʽӦ�ò�����λ�ƣ������£�
		//gpzx = 0.5 * yr1
		//gpzz = 0.5 * zr
	}else if( giUPxyz == 2 ){
		//y������,����ͼˮƽ������Ϊx��������
		gpzx = xl + 0.5 * xr;
		gpzz = zl + 0.5 * zr;
		//���߻���������ʽӦ�ò�����λ�ƣ������£�
		//gpzx = 0.5 * xr
		//gpzz = 0.5 * zr
	}else if( giUPxyz == 3 ){
		//z������,����ͼˮƽ������Ϊx��������
		gpzx = xl + 0.5 * xr;
		gpzz = yl + 0.5 * yr1;
		//���߻���������ʽӦ�ò�����λ�ƣ������£�
		//gpzx = 0.5 * xr
		//gpzz = -0.5 * yr1
	}
	//����ƫб��A>4��ʱƫװ,sin(A)=a/l=sqr((xr)^2+(zr)^2)/(sngSEL-sngPEL)
	//tan(A)^2=Sin(A)^2/(1-Sin(A)^2)
	float alfa=0;   //��ƫװ����̬ƫת�ǣ��Ƕ�
	float alfa1=0; //ƫװ�����̬ƫת�ǣ��Ƕ�
	float mvSinA=0;    //ƫת��alfa������
	float alfaLimit=0;  //����/����֧����ƫת������ֵ:����4��(1/15)������3��(1/20)��
	//gdw1:�û�����ĸ�����λֵmm
	//gdw:����ƫװ�����ĸ�����λֵmm


	if( fabs(yr) > gUseSpringMinDist ){
		alfaLimit = 4;
	}else{
		alfaLimit = 3;
	}

	if( sngSEL > sngPEL )
	{
		if( giUPxyz == 1 ){
			//mvSinA = sqrt(yr1 *yr1 + zr * zr) / fabs(sngSEL * 1000.0 - dyr);//pfg2005.12.28ԭ����
			mvSinA = sqrt(yr1 *yr1 + zr * zr) / fabs(sngSEL * 1000.0 - sngPEL*1000);//pfg2005.12.28
		}else if( giUPxyz == 2 ){
			//mvSinA = sqrt(xr * xr + zr * zr) / fabs(sngSEL * 1000.0 - dyr);//pfg2005.12.28ԭ����
			mvSinA = sqrt(xr * xr + zr * zr) / fabs(sngSEL * 1000.0 - sngPEL*1000);//pfg2005.12.28
		}else if( giUPxyz == 3 ){
			//mvSinA = sqrt(yr1 * yr1 + xr * xr) / fabs(sngSEL * 1000.0 - dyr);//pfg2005.12.28ԭ����
			mvSinA = sqrt(yr1 * yr1 + xr * xr) / fabs(sngSEL * 1000.0 - sngPEL*1000);//pfg2005.12.28
		}
	}
	
	if( sngPEL >= sngSEL )
	{//3 //֧��(start)
		//֧��
		if( dg == _T("D") )
		{//2
			//�ܲ�ƫװp
			if( df == _T("Z") )
			{//1
				//����ͼ�ܲ�������Z����
				//ֻ��������ͼ��ӳƫװ
				dxa = dxa0 - gpzx;
				dza = dza0;
			}//1
			else
			{//1
				//ֻ��������ͼ��ӳƫװ
				dxa = dxa0;
				dza = dza0 - gpzz;
			}//1
				gdw = gdw1;
				gdwx = dxa0;
				gdwz = dza0;	
			}//2
			else
			{//1
				//����ƫװ
				/*
				if( gf == _T("X") )
					gdw = gdw1 + gpzx;
				else
					gdw = gdw1 + gpzz;
				
				//�����Թܲ���װ̬��ƫװֵ
				dxa = dxa0; //����������λ�û���.
				dza = dza0;
				gdwx = dxa + gpzx;//20071127 "-" ��Ϊ "+"
				gdwz = dza + gpzz;//20071127 "-" ��Ϊ "+"
				*/
				//�����Թܲ���װ̬��ƫװֵ
				dxa = dxa0; //����������λ�û���.
				dza = dza0;
				//֧��ƫװԭΪ���������¸��ݿͻ�������һ�����ϵ���λ�Ƴ�������ֵ(��ѡ�����ṩ�ͻ�����)ʱƫװ
				//������ƫװ�㷨�У�1.����Y�����ϵ�ʱ��װͼ����ϸ����ȷ  2.����X�����ϵ�ʱ��װͼ�������ϸ����ȷ
				//3.����Z�����ϵ�ʱ��װͼ��ȷ����ϸ����󣨷���
				//modify by ligb luorijin  2008.10.16

				if ( gf == _T("X") )
				{
					if ( 1 == giUPxyz )
					{
						gpzx = gpzx*(fabs(yr1)>=gdZJOverValue ? 1 : 0);
						gpzz = gpzz*(fabs(zr)>=gdZJOverValue ? 1 : 0);					
						gdw = gdw1 - Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzx;
						gdwx = dxa - gpzx;
						gdwz = dza + gpzz;

					}
					else if ( 2 == giUPxyz )
					{
						gpzx = gpzx*(fabs(xr)>=gdZJOverValue ? 1 : 0);
						gpzz = gpzz*(fabs(zr)>=gdZJOverValue ? 1 : 0);
						gdw = gdw1 + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzx;
						gdwx = dxa + gpzx;
						gdwz = dza + gpzz;
					}
					else if ( 3 == giUPxyz )
					{
						gpzx = gpzx*(fabs(xr)>=gdZJOverValue ? 1 : 0);
						gpzz = gpzz*(fabs(yr1)>=gdZJOverValue ? 1 : 0);
						gdwx = dxa + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzx;
						gdw = gdw1 + gpzx;
						gdwz = dza - gpzz;
					}
				}
				else
				{
					//��Բ�ͬ��������ͼ��ע�Ĵ���
					if ( 1 == giUPxyz )
					{
						gpzx = gpzx*(fabs(yr1)>=gdZJOverValue ? 1 : 0);
						gpzz = gpzz*(fabs(zr)>=gdZJOverValue ? 1 : 0);
						gdw = gdw1 + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzz;
						gdwx = dxa - gpzx;
						gdwz = dza + gpzz;
					}
					else if ( 2 == giUPxyz )
					{
						gpzx = gpzx*(fabs(xr)>=gdZJOverValue ? 1 : 0);
						gpzz = gpzz*(fabs(zr)>=gdZJOverValue ? 1 : 0);
						gdw = gdw1 + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzz;
						gdwx = dxa + gpzx;
						gdwz = dza + gpzz;	
					}
					else if ( 3 == giUPxyz )
					{
						gpzx = gpzx*(fabs(xr)>=gdZJOverValue ? 1 : 0);
						gpzz = gpzz*(fabs(yr1)>=gdZJOverValue ? 1 : 0);
						gdw = gdw1 - Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzz;
						gdwx = dxa + gpzx;
						gdwz = dza - gpzz;
					}
				}

			}//1
			rsZA->PutCollect((_T("pzx")),(float)gpzx);
			rsZA->PutCollect((_T("pzz")),(float)gpzz);
		}//3 //֧��(end)
		else
		{//4//����(start)
			alfa = atan(sqrt(mvSinA * mvSinA / (1.0 - mvSinA * mvSinA))) / atan((double)1.0) / 4.0 * 180.0;
			//����ƫװ��ܲ�ƫװ
			if( alfa > alfaLimit )
			{//3
				//��Ҫƫװ
				alfa = atan(sqrt(mvSinA * mvSinA / (1.0 - mvSinA * mvSinA))) / atan((double)1.0) / 4.0 * 180.0;
				if( giUPxyz == 1 )
					//x������,����ͼˮƽ������Ϊy��������
					mvSinA = sqrt((yr1 - gpzx) * (yr1 - gpzx) + (zr - gpzz) * (zr - gpzz)) / fabs(sngSEL * 1000.0 - dyr);
				else if( giUPxyz == 2 )
					//y������,����ͼˮƽ������Ϊx��������
					mvSinA = sqrt((xr - gpzx)* (xr - gpzx) + (zr - gpzz)*(zr - gpzz)) / fabs(sngSEL * 1000.0 - dyr);
				else if( giUPxyz == 3 )
					//z������,����ͼˮƽ������Ϊx��������
					mvSinA = sqrt((xr - gpzx) * (xr - gpzx) + (yr1 - gpzz) * (yr1 - gpzz)) / fabs(sngSEL * 1000.0 - dyr);
				
         
				alfa1 = atan(sqrt(mvSinA * mvSinA / (1.0 - mvSinA * mvSinA))) / atan((double)1.0) / 4.0 * 180.0;
				if( alfa1 > alfaLimit )
				{//1
					//���˳���̫�̣���ʹƫװ��ƫת����Ȼ̫��
					sTmp.Format(IDS_TooShortFromSAToPA,ftos(alfa1),ftos(alfaLimit));
					ShowMessage(sTmp);
					bResult = FALSE;
				}//1
				if( dg == _T("D") )
				{//2
					//�ܲ�ƫװ
					if( df == _T("Z") )
					{//1
						//����ͼ�ܲ�������Z����
						//ֻ��������ͼ��ӳƫװ
						dxa = dxa0 - gpzx;
					}//1
					else
					{//1
						//ֻ��������ͼ��ӳƫװ
						dza = dza0 - gpzz;
					}//1
					gdw = gdw1;
					gdwx = dxa0;
					gdwz = dza0;

				}//2
				else
				{//2
					//����ƫװ
					/*
					if( gf == _T("X") )
					{//1
						gdw = gdw1 + Sgn(dxa0 * (fx == _T("P") ? 1 : -1)) * gpzx;
						dxa=dxa0;
						dza=dza0;
						gdwx = dxa + gpzx;
						gdwz = dza + gpzz;
					}//1
					else
					{//1
						gdw = gdw1 + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzz;
						dxa=dxa0;
						dza=dza0;
						gdwx = dxa + gpzx;
						gdwz = dza + gpzz;
					}//1
					*/
					//������ƫװ�㷨�У�1.����Y�����ϵ�ʱ��װͼ����ϸ����ȷ  2.����X�����ϵ�ʱ��װͼ�������ϸ����ȷ
					//3.����Z�����ϵ�ʱ��װͼ��ȷ����ϸ����󣨷���
					//modify by ligb luorijin  2008.10.16
					dxa=dxa0;
					dza=dza0;
					if (gf == _T("X"))
					{
						if ( 1 == giUPxyz )
						{
							gdw = gdw1 - Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzx;
							gdwx = dxa - gpzx;
							gdwz = dza + gpzz;
						}
						else if ( 2 == giUPxyz )
						{
							gdw = gdw1 + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzx;
							gdwx = dxa + gpzx;
							gdwz = dza + gpzz;				
							
						}
						else if ( 3 == giUPxyz )
						{
							gdw = gdw1 + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzx;
							gdwx = dxa + gpzx;
							gdwz = dza - gpzz;
						}
					}
					else
					{
						if ( 1 == giUPxyz )
						{
							gdw = gdw1 + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzz;
							gdwx = dxa - gpzx;
							gdwz = dza + gpzz;
						}
						else if ( 2 == giUPxyz )
						{
							gdw = gdw1 + Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzz;
							gdwx = dxa + gpzx;
							gdwz = dza + gpzz;				
							
						}
						else if ( 3 == giUPxyz )
						{
							gdw = gdw1 - Sgn(dza0 * (fx == _T("P") ? 1 : -1)) * gpzz;
							gdwx = dxa + gpzx;
							gdwz = dza - gpzz;
						}
					}

				}//2

			}//3
			else
			{//1
				//����Ҫƫװ
				gpzx = 0;//��ƫװʱ����ϸ���е�ƫװֵӦ��Ϊ0;
				gpzz = 0;
				gdw = gdw1;
				gdwx = dxa0;
				gdwz = dza0;
				dxa=dxa0;
				dza=dza0;
			}//1
			rsZA->PutCollect((_T("pzx")),(float)modPHScal::gpzx);
			rsZA->PutCollect((_T("pzz")),(float)modPHScal::gpzz);
		}//4//����(end)
		//�����ܲ�������ƫװֵ����
		return bResult;
}

void modPHScal::StressOfMaterial(CString Material,float temperature,float& Sigmat,int iMODE,float& Density)
{
	CString SQLx,sTmp,strTbn,strFD;
	_variant_t vTmp;
	float t1,t2,Sigma1,Sigma2;

	_RecordsetPtr rsTmp;
	rsTmp.CreateInstance(__uuidof(Recordset));
	Sigmat=0.0;
	Density=0.0;
	float Coef;
	if(iMODE==0)
	{
		//���Ҳ�����ĳ�¶�temperature�µ�����Ӧ��sigma,����ֵ��λunit:kgf/cm2,
		//���ݱ��е�λMPa(N/mm2)
		//��������Ӧ��Sigmat,MPa->kgf/cm2
		if(modPHScal::tbnAllowStress==_T("")) modPHScal::tbnAllowStress=_T("MechanicalOfMaterialSIGMAt");
		strTbn=modPHScal::tbnAllowStress;
		strFD=_T("SIGMA");
		Coef=100.0/EDIBgbl::kgf2N;	
	}
	else if(iMODE==1)
	{
		//���ҵ���ģ��Et,kN/mm,
		strTbn=_T("MechanicalOfMaterialEt");
		strFD=_T("Et");
		Coef=1.0;	
	}
	else if(iMODE==2)
	{
		//������������ϵ��Alphat,unit:10e-6/C
		strTbn=_T("MechanicalOfMaterialALPHAt");
		strFD=_T("ALPHAt");
		Coef=1.0;	
	}
	else return;

	try
	{
		SQLx = _T("SELECT * FROM [") + strTbn + _T("] WHERE Trim(material)=\'") + Trim(Material) + _T("\' ORDER BY t");
		rsTmp->Open((_bstr_t)SQLx, _variant_t((IDispatch*)EDIBgbl::dbMaterial,true), 
			adOpenStatic, adLockOptimistic, adCmdText); 
		if(rsTmp->adoEOF && rsTmp->BOF)
			;
		else
		{
			//���Ҽ����¶�t0ʱ������Ӧ��ֵSigma1
			SQLx.Format(_T("t>=%g"),temperature);
			_variant_t vTmp;
			rsTmp->Find((_bstr_t)SQLx, 0, adSearchForward);
			if(!rsTmp->adoEOF)
			{
				rsTmp->get_Collect((_variant_t)strFD,&vTmp);
				Sigma1 = vtof(vTmp);
				rsTmp->get_Collect((_variant_t)_T("t"),&vTmp);
				t1 = vtof(vTmp);
				if(iMODE==1)
				{
					rsTmp->get_Collect((_variant_t)_T("Density"),&vTmp);
					Density=vtof(vTmp);
				}
				//�ҵ��¶�ֵ,ǰ��һ����¼�����¶�ֵ�϶��ȼ����¶ȵ͡�
				rsTmp->MovePrevious();
				if(rsTmp->adoEOF)
				{
					if(t1!=temperature)
					{
						//�����ܳ����������
						//�ڲ�������Ӧ����%s��û�ҵ�>=%sC���¶�ֵ��
						sTmp.Format(GetResStr(IDS_NotFoundTemperatureValueInTableAllowStress),_T("\%d"),modPHScal::tbnAllowStress,temperature);
						throw sTmp;
					}
					else
					{
						Sigmat = Sigma1 *Coef;
					}
				}
				else
				{
					rsTmp->get_Collect((_variant_t)strFD,&vTmp);
					Sigma2 = vtof(vTmp);
					rsTmp->get_Collect((_variant_t)_T("t"),&vTmp);
					t2 = vtof(vTmp);
					Sigmat = Sigma2 + (temperature - t2) * (Sigma1 - Sigma2) / (t1 - t2);
					Sigmat = Sigmat *Coef;
					if(iMODE==1)
					{
						rsTmp->get_Collect((_variant_t)_T("Density"),&vTmp);
						Density=vtof(vTmp);
					}
				}
			}
			else
			{
				//�ڲ�������Ӧ����%s��û�ҵ�>=%sC���¶�ֵ��
				//sTmp.Format(GetResStr(IDS_NotFoundTemperatureValueInTableAllowStress),_T("\%d"),modPHScal::tbnAllowStress,temperature);
				//throw sTmp;
				//ȡ����¶��µ�ֵ��Ϊ��ѯֵ
				rsTmp->MoveLast();
				rsTmp->get_Collect((_variant_t)strFD,&vTmp);
				Sigmat = vtof(vTmp) *Coef;
				if(iMODE==1)
				{
					rsTmp->get_Collect((_variant_t)_T("Density"),&vTmp);
					Density=vtof(vTmp);
				}
			}
		}
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

void modPHScal::MakeTmpCalFixPhs()
{
//����GetphsBHandSizes��������һ��֧����֮ǰ��������dbPRJ�н�����tmpCalFixPhs
	//����Ĵ��������ΪSQL���̫�������³���ֻ�е����̵�һ�����Ⱥ���ܳɹ���
	/*
	CString SQLx;
	try
	{
		if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("tmpCalFixPhs")))
			//dbPRJ.Execute _T("DELETE * FROM tmpCalFixPhs")
			//�����ȽϷ��㣬���Ұ汾����ʱ�����Ժá�
			;
			//EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DROP TABLE tmpCalFixPhs"));
		//Else
			//������,������ʱ��tmpCalFixPhs,׼������̶�֧�ܣ�˫�����Ǽ�SJ8
		//End If		
		SQLx=_T("CREATE TABLE tmpCalFixPhs (ID char(20),CustomID char(50),BH char(50),tj REAL,H REAL,Px REAL,Py REAL,Pz REAL,Mx REAL,My REAL,Mz REAL,P1 char(20),P2 char(20),P3 char(20),dw1 REAL,s REAL,K REAL,K1 REAL,K2 REAL,W1pa REAL,Wp1pa REAL,F1pa REAL,W2pa REAL,Wp2pa REAL,F2pa REAL,Py1 REAL,Py2 REAL,SIGMAjt REAL,SIGMAj20 REAL,SIGMAj100 REAL,J1y REAL,J2y REAL,J1z REAL,J2z REAL,W1y REAL,W2y REAL,W1z REAL,W2z REAL,Wyh1 REAL,Wyh2 REAL,Wzh1 REAL,Wzh2 REAL,SIGMA1sa,TAO1sa REAL,TAOx1sa REAL,TAOy1sa REAL,TAOz1sa REAL)");
			
			_T(",SIGMA2sa,TAO2sa REAL,TAOx2sa REAL,TAOy2sa REAL,TAOz2sa REAL,")
			_T("F1sa REAL,F2sa REAL,h1 REAL,h2 REAL,b1 REAL,b2 REAL,B1 REAL,bb REAL,PL3 REAL,gdw2 REAL)");
			//�Ͼ�gdw2�ֶδ洢��ƫװ�������ĸ�����λֵgdw
			
		EDIBgbl::dbPRJ->Execute((_bstr_t)SQLx);
		EDIBgbl::dbPRJ->Execute((_bstr_t)"ALTER TABLE ADD COLUMN );
		EDIBgbl::dbPRJ->Execute((_bstr_t) _T("INSERT INTO tmpCalFixPhs (H) VALUES (0)"));
	}
	catch (_com_error &e)
	{
	CString strMsg;
	strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
	AfxMessageBox(strMsg);
	}
	*/
}

bool modPHScal::CreateTmpCustomIDForPART()
{
	// Ŀ��:Ϊ�ӿ���˿���ģ����ٶȣ�������������ʱ���洢PA/PART/SA/SPR/CSPR��CustomID,
	// ÿ��ֻҪ���⼸��������ȷ������Ƿ�����ڵ�ǰ��׼
	FILE* fp;
	fp = fopen("CreateTmpCustomIDForPART.txt", "w");

	CString strSQL;
	HRESULT hr = S_OK;
	try
	{
		if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ ,_T("tmpCustomIDPA")))
		{
			fprintf(fp, "%d: dbPRJ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE FROM tmpCustomIDPA"), NULL, adCmdText);			
			strSQL=_T("INSERT INTO tmpCustomIDPA IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnPA+_T("]");
			fprintf(fp, "%d: dbZDJcrude->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		else
		{
			//��Ҫ����Ψһ���������ܳ�ͻ
			strSQL = _T("CREATE TABLE tmpCustomIDPA (CustomID Char(20) CONSTRAINT index1 UNIQUE)");
			fprintf(fp, "%d: dbPRJ->Execute\n", __LINE__);
			hr = EDIBgbl::dbPRJ->Execute((_bstr_t)strSQL, NULL, adCmdText);
			strSQL=_T("INSERT INTO tmpCustomIDPA IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnPA+_T("]");
			fprintf(fp, "%d: dbZDJcrude->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
	}
	catch(...)
	{
		fprintf(fp, "...");
		fclose(fp);
		return false;
	}
	try
	{
		//�����̶�֧�����ݱ�tbnPAfixʱ�����Բ��ܳ��ֹܲ����ݱ�tbnPA���Ѿ����ֵ�CustomID���������.
		if(modPHScal::tbnPAfix!=modPHScal::tbnPA)
		{
			//strSQL=_T("INSERT INTO tmpCustomIDPA IN \'") + EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnPAfix+_T("] WHERE CustomID NOT IN ( SELECT DISTINCT CustomID FROM [") + modPHScal::tbnPA + _T("] ) ");
			strSQL=_T("INSERT INTO tmpCustomIDPA IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnPAfix+_T("] ");
			fprintf(fp, "%d: dbZDJcrude->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}
	}
	catch(...)
	{
		fprintf(fp, "...");
		fclose(fp);
		return false;
	}

	try
	{
		if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ ,_T("tmpCustomIDSA")))
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE FROM tmpCustomIDSA"), NULL, adCmdText);			
			strSQL=_T("INSERT INTO tmpCustomIDSA IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnSA+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}
		else
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE tmpCustomIDSA (CustomID Char(20) CONSTRAINT index1 UNIQUE)"), NULL, adCmdText);
			strSQL=_T("INSERT INTO tmpCustomIDSA IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnSA+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}
	}
	catch(...)
	{
		fprintf(fp, "...");
		fclose(fp);
		return false;
	}

	try
	{
		if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ ,_T("tmpCustomIDPART")))
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE FROM tmpCustomIDPART"), NULL, adCmdText);			
			strSQL=_T("INSERT INTO tmpCustomIDPART IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [") + modPHScal::tbnPART+_T("]");
			fprintf(fp, "%d: ->Execute: %s\n", __LINE__, strSQL);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}		
		else
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE tmpCustomIDPART (CustomID Char(20) CONSTRAINT index1 UNIQUE)"), NULL, adCmdText);
			strSQL=_T("INSERT INTO tmpCustomIDPART IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [") + modPHScal::tbnPART+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}
	}
	catch(...)
	{
		fprintf(fp, "...");
		fclose(fp);
		return false;
	}

	try
	{
		if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ ,_T("tmpCustomIDSPR")))
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE FROM tmpCustomIDSPR"), NULL, adCmdText);			
			strSQL=_T("INSERT INTO tmpCustomIDSPR IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnSPRINGCrude+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}		
		else
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE tmpCustomIDSPR (CustomID Char(20) CONSTRAINT index1 UNIQUE)"), NULL, adCmdText);
			strSQL=_T("INSERT INTO tmpCustomIDSPR IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnSPRINGCrude+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}
	}
	catch(...)
	{
		fprintf(fp, "...");
		fclose(fp);
		return false;
	}

	try
	{
		if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ ,_T("tmpCustomIDCSPR")))
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE FROM tmpCustomIDCSPR"), NULL, adCmdText);			
			strSQL=_T("INSERT INTO tmpCustomIDCSPR IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnHDCrude+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}		
		else
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE tmpCustomIDCSPR (CustomID Char(20) CONSTRAINT index1 UNIQUE)"), NULL, adCmdText);
			strSQL=_T("INSERT INTO tmpCustomIDCSPR IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID FROM [")+ modPHScal::tbnHDCrude+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}
	}
	catch(...)
	{
		fprintf(fp, "...\n");
		fclose(fp);
		return false;
	}

	try
	{
		//add by pfg,ligb on 2005.02.18
		//Ϊ�����ͼ�����������LoadGDWItem2ComboGDW1()�������ٶȣ�
		//��һ����ʱ��tmpCustomID_GDW1����ΪtbnSA�������ݱ����7000�����ϼ�¼��
		//����ʱ��tmpCustomID_GDW1ֻ��300�����Ҽ�¼���ٶȿ����7000/300=23�����ϡ�
		if(EDIBgbl::tdfExists(EDIBgbl::dbPRJ ,_T("tmpCustomIDGDW1")))
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE FROM tmpCustomIDGDW1"), NULL, adCmdText);			
			strSQL=_T("INSERT INTO tmpCustomIDGDW1 IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID,GDW1 FROM [")+ modPHScal::tbnSA+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}		
		else
		{
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			EDIBgbl::dbPRJ->Execute((_bstr_t)_T("CREATE TABLE tmpCustomIDGDW1 (CustomID Char(20),GDW1 single)"), NULL, adCmdText);
			strSQL=_T("INSERT INTO tmpCustomIDGDW1 IN \'") + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\'  SELECT DISTINCT CustomID,GDW1 FROM [")+ modPHScal::tbnSA+_T("]");
			fprintf(fp, "%d: ->Execute\n", __LINE__);
			modPHScal::dbZDJcrude->Execute((_bstr_t)strSQL, NULL, adCmdText);		
		}
	}
	catch(_com_error e)
	{
		fprintf(fp, "%d: _com_error: %s\n", __LINE__, (LPCSTR)e.Description());
		fclose(fp);
		return false;
	}
	fclose(fp);

	return true;
}

void modPHScal::CreatePhsStructureNameAndREFIndbPRJ()
{
	CString SQLx;
		//�ڴ�֮ǰ�����ƺ�ID��Ӧ�ñ����
		//�ȸ���PhsStructureNAME��sort.mdb->dbPRJ,��ΪJetһ��ֻ������1���ⲿ��
		if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("PhsStructureNAME")))
		{
			//���������Ա�������
			try
			{
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE FROM PhsStructureNAME"), NULL, adCmdText | adExecuteNoRecords);
			}
			catch (_com_error &e)
			{
				CString strMsg;
				strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
				AfxMessageBox(strMsg);
			}
			SQLx = CString(_T("INSERT INTO PhsStructureNAME IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' SELECT * FROM PhsStructureNAME ");
			try
			{
				EDIBgbl::dbPRJ->Close();
				EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText | adExecuteNoRecords);//20071018 "dbSORT" ��Ϊ "dbPHScode"

				EDIBgbl::dbPRJ->Open((_bstr_t)("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + 
					basDirectory::ProjectDir+_T("WorkPrj.mdb")), "", "", adModeUnknown);

			}
			catch (_com_error &e)
			{
				CString strMsg;
				strMsg.Format("%s:%d %s, \nSQL=%s", __FILE__, __LINE__, (LPSTR)e.Description(), SQLx);
				AfxMessageBox(strMsg);
			}
		}
		else
		{
			//�����������ڡ�
			SQLx = CString(_T("SELECT * INTO PhsStructureNAME IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' FROM PhsStructureNAME ");
			try
			{
				EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071018 "dbSORT" ��Ϊ "dbPHScode"
			}
			catch (_com_error &e)
			{
				CString strMsg;
				strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
				AfxMessageBox(strMsg);
			}
		}
		//�ȸ���PhsStructureREF��sort.mdb->dbPRJ,��ΪJetһ��ֻ������1���ⲿ��
		if (EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("PhsStructureREF")))
		{
			try
			{
				EDIBgbl::dbPRJ->Execute((_bstr_t)_T("DELETE FROM PhsStructureREF"), NULL, adCmdText);		
			}
			catch (_com_error &e)
			{
				CString strMsg;
				strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
				AfxMessageBox(strMsg);
			}
			SQLx = CString(_T("INSERT INTO PhsStructureREF IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' SELECT * FROM PhsStructureREF ");
			try
			{
				EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071018 "dbSORT" ��Ϊ "dbPHScode"
			}
			catch (_com_error &e)
			{
				CString strMsg;
				strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
//				AfxMessageBox(strMsg);
			}
		}
		else
		{
			//�����������ڡ�
			SQLx = CString(_T("SELECT * INTO PhsStructureREF IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' FROM PhsStructureREF ");
			try
			{
				EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071018 "dbSORT" ��Ϊ "dbPHScode"
			}
			catch (_com_error &e)
			{
				CString strMsg;
				strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
				AfxMessageBox(strMsg);
			}
		}
}

void modPHScal::CreatePhsStructureNameAndREFatStart()
{
	//Ŀ��:��dbPRJ������ģ���ʱ,��dbSORT����������.
	CString SQLx;
	try
	{
		//�ڴ�֮ǰ�����ƺ�ID��Ӧ�ñ����
		//�ȸ���PhsStructureNAME��sort.mdb->dbPRJ,��ΪJetһ��ֻ������1���ⲿ��
		if (!EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("PhsStructureNAME")))
		{
			SQLx = CString(_T("SELECT * INTO PhsStructureNAME IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' FROM PhsStructureNAME ");
			EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071101 "dbSORT" ��Ϊ "dbPHScode"
		}
		//�ȸ���PhsStructureREF��sort.mdb->dbPRJ,��ΪJetһ��ֻ������1���ⲿ��
		if (!EDIBgbl::tdfExists(EDIBgbl::dbPRJ, _T("PhsStructureREF")))
		{
			SQLx = CString(_T("SELECT * INTO PhsStructureREF IN \'")) + EDIBgbl::GetDBName(EDIBgbl::dbPRJ) + _T("\' FROM PhsStructureREF ");
			EDIBgbl::dbPHScode->Execute((_bstr_t)SQLx, NULL, adCmdText);//20071101 "dbSORT" ��Ϊ "dbPHScode"
		}
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

void modPHScal::WndDataCheck(CWnd *pCtrl, CString strCaption)
{
	ASSERT( pCtrl != NULL );
	ASSERT( IsWindow(pCtrl->GetSafeHwnd()) );
	HWND hCtrl = pCtrl->GetSafeHwnd();

	TCHAR szBuffer[20];
	if( IsWindow(hCtrl) )
	{
		::GetWindowText(hCtrl, szBuffer, 20);
		CString str(szBuffer);
		if( str.IsEmpty() )
		{
			CString strInfo;
			strInfo.Format("����[%s]����������",strCaption);
			AfxMessageBox(strInfo);
		}
	}
}

void modPHScal::DrawUserDesc(int& iNo,CCadPoint basePt, CMObject &oSpace, CString strTextStyle)
{
	CMObject objEnt;
	CString strReadDesc;
	CFile fileDescription;
	CFileException ex;

	try
	{
		basePt.SetY(basePt[1]+4.13);
		user::AddDirSep( basDirectory::ProjectDir);
		if( !fileDescription.Open(basDirectory::ProjectDir+"UserDesc.txt",CFile::modeRead,&ex) )
		{
			ex.ReportError();
			ex.Delete();
			return;
		}
		UINT nFileLength=fileDescription.GetLength();

		//һ�ζ�����������
		int iEnd;
		if( nFileLength>0 )
		{
			iEnd = fileDescription.Read(strReadDesc.GetBufferSetLength(nFileLength),nFileLength);
		}
		else
			return;

		strReadDesc.TrimLeft();
		strReadDesc.TrimRight();

		bool bHasFH=false;
		int iPosA;
		int iPos =  0;//������յķֺ�λ��
		int nRChangeLine=0;//�س����з���
		CString strDesc;
		while( !strReadDesc.IsEmpty() )
		{
			iPosA = strReadDesc.Find(";");
			if( iPosA!=-1 )//ֻ�д��ڷֺŵ�������ܼ���
			{
				iPos = iPosA;
				strDesc = strReadDesc.Mid(0,++iPos);
				strReadDesc = strReadDesc.Mid(iPos);

				strDesc.TrimRight();
				strReadDesc.TrimLeft();

				strDesc.Remove(';');

				nRChangeLine=strDesc.Replace("\r\n","\\P ");

				CString strText;
				strText.Format("%d.%s",iNo++,strDesc);

				//CMObject tmpObj=EDIBAcad::objAcadDoc.GetPropertyByName(_T("Text"));
				//int iWidth = vtoi(tmpObj.GetPropertyByName("Width"));
				//���ܻ���� 'MText' ����������� 'Width' ���Զ����еĶ���
				objEnt=oSpace.Invoke(_T("AddMText"),3, (LPVARIANT)basePt, &_variant_t((double)190.0), &_variant_t(strText)).pdispVal;
				objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(strTextStyle));
				objEnt.PutPropertyByName(_T("Height"),&_variant_t((double)4.0));
				
				if( nRChangeLine )
					basePt.SetY(basePt[1] - 7*(nRChangeLine+1));
				else
					basePt.SetY(basePt[1] - 7);
			}
			else
			{
				CString strText;
				strText.Format("%d.%s",iNo++,strReadDesc);

				//���ܻ���� 'MText' ����������� 'Width' ���Զ����еĶ���
				objEnt=oSpace.Invoke(_T("AddMText"),3, (LPVARIANT)basePt, &_variant_t((double)190.0), &_variant_t(strText)).pdispVal;
				objEnt.PutPropertyByName(_T("StyleName"),&_variant_t(strTextStyle));
				objEnt.PutPropertyByName(_T("Height"),&_variant_t((double)4.0));
				
				if( nRChangeLine )
					basePt.SetY(basePt[1] - 7*(nRChangeLine+1));
				else
					basePt.SetY(basePt[1] - 7);
				break;
			}
		}
		fileDescription.Close();
	}
	catch (_com_error &e)
	{
		CString strMsg;
		strMsg.Format("%s:%d %s", __FILE__, __LINE__, (LPSTR)e.Description());
		AfxMessageBox(strMsg);
	}
}

int modPHScal::SumLugNum()
{  int n,i;
   float gh1,dh1,fRodLen;
   _RecordsetPtr rs;
   rs.CreateInstance(__uuidof(Recordset));
   CString strSQL;
   _variant_t vTmp;
   gh1=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("GH1")));
   dh1=vtof(FrmTxsr.m_pViewTxsr->m_ActiveRs->GetCollect(_T("DH1")));
//   strSQL.Format(_T("SELECT * FROM [%s] WHERE ID='L1'"
  strSQL = _T("SELECT * FROM [") + tbnLugInfo + _T("] WHERE ID=\'L1\'");
   rs->Open((_bstr_t)strSQL, _variant_t((IDispatch*)dbZDJcrude,true), 
	   adOpenKeyset, adLockOptimistic, adCmdText); 
   if(!(rs->BOF && rs->adoEOF))
   {	rs->MoveFirst();
	    rs->get_Collect((_variant_t)_T("maxLength"),&vTmp);
	    fRodLen= vtof(vTmp);
		if((i=int((gh1-dh1)*1000)%int(fRodLen))==0)
			n=(gh1-dh1)*1000/fRodLen;
		else n=(gh1-dh1)*1000/fRodLen+1;
		rs->Close();
   }
   else
   { rs->Close();
     return 0;
   }

   return n;
}

//by ligb on 2005.01.25
//LISEGA�������ɱ�����ɺ���
//�����벽��:
//1.�򿪿�ZDJcrudeLISEGA.MDB,��������:��LISEGALoad����ϵ�б����ÿһ������ֵLoadValueѭ����
//  �г̷�Χ����ѭ����LISEGALoad����ϵ�б����TravelRangeMin�ֶε�TravelRangeMax�ֶΣ�
//  �����г̷�Χ���Ŵ�TravelSeriesValue�г�ϵ�б��ö�Ӧ��TravelMaxValueֵ��
//  �Ѻ���ֵLoadValueѭ�����г�TravelMaxValueֵ��ˣ����д��
//  HDforceDist_LISEGA_new�����������capacity�ֶΣ���CSPRNo�ֶ�����TravelRange�ֶ�
//  ��ϳ�LISEGACSPRNo�ֶΡ�TravelMaxValue->MaxDist,MinDist->MinDist��
//  manufactory="LISEGA"
//2.��capacity�ֶ�ֵ��������HDforceDist_LISEGA_new����DH�ֶ�˳����1-73��
//3.�����γ���һ��LISEGA�������ɵ��غ�������������ͨ�ı����һ���ֶ�LISEGACSPRNo��
//4.�Ժ�Ժ���ߴ����ݱ�HDcrude_LISEGA����ѭ���ñ�ÿ����¼��ȡBH��2��3��4λ����µ�
//  �����г�ʶ���룬��ʶ���뿪ͷ�ַ�Ϊ6(����֧��)��7(�ŷ�����)��9(�ᵣ����)ʱ������ʶ���뿪ͷ�ַ�Ϊ1����HDforceDist_LISEGA_new��
//  LISEGACSPRNo�ֶ�ֵ�����ʶ������ȵļ�¼��ȡ
//  HDforceDist_LISEGA_new��DH�ֶ�ֵ,д�����ߴ����ݱ�HDcrude_LISEGA��DH�ֶΡ�
//5.�깤��
void modPHScal::MakeLISEGA_ConstantSPR_DH()
{//LISEGA�������ɱ�����ɺ��������ü��ϡ��˺�������פ����DBConvert.dsp��Ŀ�С�ligb on 2005.11.21


}
//pfg20051028 ���ݵ�ǰ֧���ܵ��������õĲ������͸��ݵ�
//ǰģ��������õĲ��������бȽ�
//����:0 ��ʾ��Ȼ��ߵ�ǰ֧��������Ϊ�ջ��߲���ȶ��û�Ը�����֧��������
//����:1 ��ʾ��������û���Ը�����֧��������
//ע�⣺��û��д��!!!
int modPHScal::SpringTypeCompare(CString strType)
{
	CString str=_T("");
	CString strMessage=_T("");
	int flag=0;
	int n;
	if(strType==""||strType.GetLength()<=3||strType.GetLength()>4)
	{
		return 0;
	}
	str=strType.Mid(0,1);
	if(str>="5")
	{//��������(start)
		strMessage=_T("��ʾ:\n��ǰ֧��������Ϊ��������,���������õĲ��Ǻ������ɡ�");
		strMessage+=_T("\n\n�ü������õ�֧���������滻��ǰ��֧���������밴 '��';");
		strMessage+=_T("\n\n����,�밴 '��'(�޸ĳ�ѡ�ĵ��ɺ��ر仯ϵ����֧����ģ���,����)!");
		if(AfxMessageBox(strMessage,MB_YESNO,1)==IDNO)
		{
			flag=1;	
		}
	}//��������(start)
	else
	{//���Ǻ�������(start)
		str=strType.Mid(1,1);
		if(str>="0"&&str<="9")
		{
			n=atoi(str);
		}
		if(gintParallelNum==n)
		{
		}
	}//���Ǻ�������(end)
	return flag;
}

//author: pfg20051101
//function: ����Ӧ���������õ�֧���������ڷ���������֧�������ͼ��в���
//return:  �ҵ�����1; ���ҵ�����0;   
int modPHScal::FindPSAType(CString strPSAType, _RecordsetPtr rs)
{
	int flag=0;
	CString strZDJType=_T("");
	_variant_t var;
	CString str=_T("");
	rs->MoveFirst();
	int iSumSerialNum;
	while(!rs->adoEOF)
	{//ѭ��(start)
		rs->get_Collect((_variant_t)_T("SerialNum"),&var);
		iSumSerialNum=vtoi(var);
		str.Format("%d",iSumSerialNum);
		strZDJType=str;
		str.Format("%d",gintParallelNum);
		strZDJType+=str;
		rs->get_Collect((_variant_t)_T("SEQ"),&var);
		str.Format("%02d",vtoi(var));
		strZDJType+=str;
		if(strZDJType==strPSAType)
		{
			flag=1;//Ӧ�������֧�������ʹ���
			break;
		}
		rs->MoveNext();
	}//ѭ��(end)
	if(!flag)
	{
		rs->MoveFirst();//û���ҵ�Ӧ��ԭ�����㷨ѡ���һ��
	}
	return flag;
}

// ����Z8��ܲ��ı�ߣ���̬����̬��.��Ӹ���֧�ܼ�����������SELֵ 20090707
void modPHScal::UpdatePipeDimHeight()
{
	CString strSQL=_T("");
	try
	{
		strSQL.Format("Update Z8 set Design_DH=%f,DH1=%f,work_DH=%f,SEL=%f  where VolumeID=%d and zdjh=%d",
			modPHScal::sngPEL, modPHScal::dyl / 1000, modPHScal::dyr / 1000, modPHScal::sngSEL, EDIBgbl::SelVlmID,
			modPHScal::zdjh );
		EDIBgbl::dbPRJDB->Execute((_bstr_t) strSQL, NULL, adCmdText );
	}
	catch(_com_error e)
	{
	}
}
