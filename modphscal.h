// modPHScal.h
//

#if !defined(AFX_MODPHSCAL_H__66EB6429_712C_11D5_8752_00D009619765__INCLUDED_)
#define AFX_MODPHSCAL_H__66EB6429_712C_11D5_8752_00D009619765__INCLUDED_

#include "resource.h"
#include "FrmStatus.h"
#include "CadPoint.h"
#include "MObject.h"

struct SelphsINFO
{
	long	SampleID;
   CString PictureClipData;
   CString FREQUENCE;
   CString BmpName;
};
enum PhsSAfxIndex{
   //������������
   iPX,
   iOZ,
   iOX,
   iPZ
};
enum PhsOutInfoTableTypeIndex{
   iDisplacementLoadSep,
   iDisplacementLoadCombine,
   iDisplacementLoadUser
};

enum PictureClipDataSPCIndex{
   //����������ڽ���������Ϊ����/�ܲ�/���Ӽ�/�͸�
   iSA = 0,
   iPA, //1,
   iConnectPART ,//2
   iSectionSteel , //3
   iAttachment ,//4
   iBoltsNuts,  //5
	iOtherPART //6
};

enum PictureClipDataClassID
{
//�����˳��Ŵ�0��ʼ��
//��ֵ���뱣����ZDJCrude.mdb���е�CrudeXXXXid���е�ClassID�ֶ�ֵһ�¡�
   iSAh =0    ,      //ֱ�ӵ�,0
   iSACantilever , //����,1
   iSAbeam     ,   //��֧��2
   iSALbrace   ,   //���Ǽ�3
   iSALbraceFixG47 , //����֧��G47,4
   iG51   ,        //�������۵�5
   iG52_55     ,   //������˨��֧��6
   iG56       ,    //��������֧��7
   iG57       ,    //��������֧��8
   iGCement ,//9,��������/��/֧��/etc.,����Ҫ�������ݿ�
   iSALbraceFixG48 ,   //����֧��G48,10
   iG100  ,  //11,�û��Զ������
   iGx12   , //10,�����ṹ����
   iGx13   , //10,�����ṹ����
   iGx14   , //10,�����ṹ����
   iGx15   , //10,�����ṹ����
   iGx16   , //10,�����ṹ����
   iGx17   , //10,�����ṹ����
   iGx18   , //10,�����ṹ����
   iPALX   , //19,�ܲ�,���ιܵ���(��)��
   iPAh    , //20,����
   iPAfixZ1, //Z1��(�ܼй̶�֧��)
   iPAfixZ2, //Z2��(���ӹ̶�֧��)
   iPASS   , //23����֧��
   iPAGS   , //24����֧��
   iPAs    , //25����֧��
   iPAD4LA ,    //26,ˮƽԲ�ιܵ��޵�庸�ӵ���(�̷�ú�۹ܵ�LA)
   iPAD11LD ,   //27,��ֱԲ�ιܵ��޵�庸�ӵ���(�̷�ú�۹ܵ�LD)
   iPADLR   ,   //28,ˮƽ���ιܵ���������(�̷�ú�۹ܵ�LR)
   iPAD4LQ  ,   //29,ˮƽ���ιܵ����۸ֵ����˺��ӵ���(�̷�ú�۹ܵ�LQ)
   iROD     ,//30,��������
   iConnected , //31,���Ӽ�
   iL15   , //32,�ְ�,���Ӽ��ṹ����
   iXL1  ,  //33,��������,��Ϊ30,������һ��
   //iConnx33  ,  //32,���Ӽ��ṹ����
   iConnx34  ,  //32,���Ӽ��ṹ����
   iConnx35  ,  //32,���Ӽ��ṹ����
   iConnx36   , //32,���Ӽ��ṹ����
   iConnx37  ,  //32,���Ӽ��ṹ����
   iConnx38 ,   //32,���Ӽ��ṹ����
   iConnx39  ,  //39,���Ӽ��ṹ����
   iAttached ,  //40,����
   iNuts     ,  //41,��ĸ
   iBolts    ,  //42,��˨
   iAttx43   ,  //43,�����ṹ����
   iAttx44    , //44,�����ṹ����
   iAttx45    , //45,�����ṹ����
   iSPR      ,  //46,����
   iCSPR     ,  //47,��������
   iSPR48    ,  //48,���ɽṹ����
   iSPR49     , //49,���ɽṹ����
   iSTEELc    , //50,�۸�       [20a
   iSTEELangle,    //51,�Ǹ�    <75x8
   iSTEELH     ,   //52,���ָ�  I32
   iSTEELround ,   //Բ��53 %%C64
   iSTEELflat  ,   //���54 -120x10
   iSTEELPlate ,   //�ְ�55 -100x100x8
   iSTEELUnknown,		//56δ֪���͸�
   iSTEELUnknown1,  //57δ֪���͸�
   iSTEELUnknown2,  //58δ֪���͸�
   iSTEELPipe,  //59�ֹܦ�108x4.5
   iSABoltsNutsUnknown,  //60δ֪�ĸ�����˨��ĸ����
   iBoltsNutsUnknown,    //61δ֪����˨��ĸ
   iSPRPV,    //62,����Ժ���𵯻�TH6
   iPAXZ1_XZ3=200,//200,��λ֧��XZ1-XZ3

   iPALU=80,//80,�̷�ú�۹ܵ���ֱ���ιܵ�֧��LU
   iPALV=81,//81,�̷�ú�۹ܵ��ŷ۷���������֧��LV
   iPALW=82,//82,�̷�ú�۹ܵ����������֧��LW
};

enum AttachIncludedFlag{
   iAttachIncluded,
   iAttachNotIncluded
};
//������϶�����������ĳ���,�μ�frmphsSamp�е�ChkConditionIndexö��.
//��������Cphs���н����б�SQL����,�Ա�ѡ������һ�������Ŀ������
enum gcChkCondition{
   iPJG = 1,
   iTJ = 2,
   iGDW1 = 4,
   iCNT = 8,
   iDW = 16,
   iOnlyPAh = 32
};

//����ѡ��Ĺ���
enum giSO{
   lngSOAuto = 0,
   lngSOSerialNumMinSEQmin ,//ѡ��������С���ɺ���С�ĵ������
   lngSOWeight, //ѡ��������С�ĵ���
   lngSOSpringNoMin, //ѡ���������С�ĵ���
   lngSODifToMidLine,  //ѡ�񵯻ɹ���λ��������ߵĵ���
   lngSOCoefOfVarLoad   //ѡ����ر仯ϵ�����ĵ���
};

//����8��ö���ͳ�����ʾ֧��������,0/1/.../7=���ɵ���/.../֧��,���sort.mdb��PhsTypeName��
enum ZdjTxNameIndex{
   iZdjSPRh,
   iZdjCSPRh,
   iZdjRigidH,
   iZdjGuide,
   iZdjSlide,
   iZDJfix,
   iZdjSprGuide,
   iZdjSupport
};

enum ZdjClassTypeIndex{
   iPhsSPR,
   iPhsCSPR,
   iPhsRigid
};

struct  SprInfo  //ѡ��ĵ�������
{
   int SerialNum;
   int SprNo;
   int DH;  //= SerialNum*100+SprNo
   float Hpcmprs;	//��ѹ��ֵmm
   float sizeC;	//T5�������ľ�
   float H0;	//�������ɸ߶�mm
   float haz;	//������Χ�ڰ�װ������haz=��װ������hazTotal-Ԥѹ��ֵHpcmprs
	float hgz;	//������Χ�ڹ���������hgz=����������hgzTotal-Ԥѹ��ֵHpcmprs
   /*float hazTotal;	//ȫ���η�Χ�ڰ�װ������hazTotal=Paz*K=haz+Hpcmprs
	float hgzTotal;	//ȫ���η�Χ�ڹ���������hgzTotal=Pgz*K=hgz+Hpcmprs
   float Her;	//��װ�߶�Her=H0-Paz*K=H0-hazTotal
	float Hop;	//�����߶�Hop=H0-Pgz*K=H0-hgzTotal
   float DELTAaz;		//������Χ�ڰ�װѹ��ֵDELTAaz=DELTAaz1-Hpcmprs=hazTotal-Hcmprs=(haz+Hpcmprs)-Hpcmprs=haz
   float DELTAgz;		//������Χ�ڹ���ѹ��ֵDELTAgz=DELTAgz1-Hpcmprs=hgzTotal-Hcmprs=(hgz+Hpcmprs)-Hpcmprs=hgz
   float DELTAaz1;	//ȫ��Χ�ڰ�װѹ��ֵDELTAaz1=H0-Her=H0-(H0-hazTotal)=hazTotal
   float DELTAgz1;	//ȫ��Χ�ڹ���ѹ��ֵDELTAgz1=H0-Hop=H0-(H0-hgzTotal)=hgzTotal
	*/
   //У�˺���������С���λ�Ƶ�λ��ֵ(���ݱ���gbCheckMinMaxDispByActualDisp��ֵ��ȷ����ʵ����λ��HeatDisp���ǲ��÷Ŵ�20%�ļ���ֵ)
	//gbCheckMinMaxDispByActualDisp=true,CheckDisp=HeatDisp
	//gbCheckMinMaxDispByActualDisp=false,CheckDisp=1.2*HeatDisp or HeatDisp+15mm
	float SumDisp; //�����������������ļ���ѡ����λ�ƣ�һ���ʵ����λ��HeatDisp��20%����Ϊ�������ɺͰ�����˾˵Ҫ��ѡ����λ�Ƽ��㰲װ�߶ȣ������Ӵ��
	short SumDistID; //�����������������ļ���ѡ����λ�ƴ��ţ�ֻ��2007����Դ�������ɹ������Ҫ����ֶ�ֵ
	float CheckDisp;   
   float HeatDisp;   //����������������λ��
   float OpLoad;   //�������������Ĺ�������
   float InstallLoad;   //�������������İ�װ����
   float FiJ ;   //�����������������ֱ��
   float FiK  ;   //���������������˨��ֱ��
	CString CustomID;
	CString BH;		//BH,���ڲ������ֱ����T5/LHG����������CLggʱ����Ϊ���ǵ��ɽ���ĸֱ�������ڼ�����ɺ����ȷ��������GetBoltsNutsAndAttachmentsCLgg������,������GetPhsBHandSizes�н����
	CString BHFormat;
	CString CLgg;
};

struct SprDirectionChar   //���ɼ�λ�Ʒ��������ַ������ڵ��Լ�����ţ�
{
	CString CSPR_UP;
   CString CSPR_Down;
   CString SPR_UP;
   CString SPR_Down;
};
extern float vf7850;
class modPHScal
{
private:
	static float fltTmp;
public:	
	static void UpdatePipeDimHeight(); // ����Z8��ܲ��ı�ߣ���̬����̬��
	static int FindPSAType(CString strPSAType, _RecordsetPtr rs);
	static int SpringTypeCompare(CString strType);
	//LFX 2005.6.30 �������ѡ��
	static bool gbDrawWeldSymbol;   //�Ƿ���ƺ������  
	static bool gbAutoWeldViewInsertPoint;//�Ƿ��Զ�ȷ��������ͼ�����
	static bool gbDrawWeldViewDim;  //�Ƿ������ͼ��ע
	static bool gbWeldAutoDirectAndPos;  //�Ƿ��Զ�Ϊ�������ȷ������λ�úͷ���
	//LFX 2005.6.30 �������ѡ��
	static float gmiDiameter;//���濿���ܲ��ĵ�һ������ֱ������1-3�μ���֮��
	static float giDiameter;//��������ֱ������3�μ���֮��
	static int SumLugNum();
	static float gfSeqCircleDiameter;
	static float gfSeqTxtHeight;
	static CString g_strDHTGG;
	static BOOL g_bDHT;
	static BOOL bInstalledAndFirstRun;
	static void DrawUserDesc(int& iNo ,CCadPoint basePt, CMObject& oSpace, CString strTextStyle );
	static float g_fLHGWeight;
	static void WndDataCheck(CWnd* pCtrl, CString strCaption);
	static int iAlignLoad;
	static int iAlignDim;
	static int iAlignLjmx;
	static float fFWLoad;
	static CString strBigFontLoad;
	static CString strSHXFontLoad;

	static CString strBigFontLjmx;
	static CString strSHXFontLjmx;
	static CString strBigFontDim;
	static CString strSHXFontDim;
	static float	fFWDim;
	static float    fFWLjmx;
	static bool bE_FormulaPlusHalf;//�������ɸ߶�ֵ�Ƿ��ʵ����λ�Ƶ�һ��
	static int iMaxRodDia;
	static CString gsPSAOutFilePath;//ѡ���Ӧ���������ļ�Ŀ¼
	static int g_iPSAFilterIndex;//ѡ���Ӧ���������ļ����͹���������
	static CString gsSelectProductMdb;//ѡ��Ĳ�Ʒ���ݿ�Ŀ¼
	static double gdZJOverValue;
	
	static bool gbAutoSPRRodDiaDecrease;
	static bool gbSPRBHLOADBYInstallLoad;
	static bool gbSPRBHLOADUNITBYNEWTON;
	static bool gbPhsIsSPR;
	static int giMinRodLen;
	static float fDiameter;
	static bool gbRodDiaByMax;
	static BOOL gbNoFenQu;
	static BOOL gbTBhq;
	static BOOL gbNoWaiKuang;
	static bool gbOrientationPicture;//pfg20050531
	static float DblHangerRodDist;
	static int giClassIndex;
	static void CreatePhsStructureNameAndREFatStart();
	static void CreatePhsStructureNameAndREFIndbPRJ();
	static bool CreateTmpCustomIDForPART();
	static int giSAG100No;
	static float gfPSThickness;
	static float gfPSWidth;
	static float gfPSLength;
	static void MakeTmpCalFixPhs();
	static void StressOfMaterial(CString Material,float temperature,float& Sigmat,int iMODE=0,float& Density=vf7850);
	static BOOL CalOffset(_RecordsetPtr rsZA);
	static int SprInfoIndex;
	static bool tbExists(_ConnectionPtr db,_ConnectionPtr db1, CString &tbn, CString s1, CString s2,CString s3);
	// LFX �� bool bWarn �Դ˱����ȷ���ڱ�δ�ҵ�ʱ�Ƿ��������  2005.2.21
	static bool HStbExists(_ConnectionPtr db,_ConnectionPtr db1, CString &tbn, CString s1, CString s2,CString s3,bool bWarn = false);  
	static _variant_t sFindAnyTableField(_ConnectionPtr  db, CString  strSourceTable, CString  strSourceFLD, CString  strDestFLD, CString  strSourceFLDvalue);
// 	static bool tbExists(_ConnectionPtr db,_ConnectionPtr db1,CString& tbn,CString s1,CString s2,CString s3);
	static CString GetPhsSAfx(int  SAfx);
	static _ConnectionPtr dbZDJcrude;
//	static int gbTbOS;
//	static long glCurRecordNo;
    static void InitZdjTxName();
    static void InitPhsSAfx();
    static int GetPhsSAfx(CString  SAfx);
    static void GetHeatDisplacement(_RecordsetPtr  rstbl);
    static int iBOMStartNo(int SN=10);
    static _variant_t sFindBlkPosFLD(CString  sFLD, CString  dFLD, CString  sID);
    static _variant_t sFindFLD(CString  strSourceFLD, CString  strDestFLD, CString strSourceFLDvalue);
//    static _variant_t sFindAnyTableField(_ConnectionPtr  db, CString  strSourceTable, CString  strSourceFLD, CString  strDestFLD, CString  strSourceFLDvalue);
    static CString sFindID(CString  CustomID);
    static CString sFindCustomID(CString  ID);
    static CString sFindTBN(CString  PartCustomID);
    static void AutoSEQforPictureClipData();
    static void InitZDJID();
    static void GetRecentWorkSPEC();
    static void SaveRecentWorkSPEC();
    static bool bPAIsHanger();
    static bool blnSelAndSaveSPEC();
    static bool blnSelphsSPEC(bool  mbSPECchanged);
    static void ImportDataFromZdjCrudeXXXX(CString  strFN, bool  bReplacement, bool & mbSPECchanged);
    static void ImportDataFromZdjCrude(CString  strFN, bool  bReplacement, bool & mbSPECchanged);
    static void SetSPRPosValue();
    static long CSLength(CString  sBHFormat, CString  LengthSQL, CString& sBH);
    static bool CheckFieldIfNull(_RecordsetPtr rs);
    static /*Object*/void GetZAdataRs(CWnd*  frm);
    static long lngPreCal();
    static CString GetSaveAsDwgFileName(long zdjh,_RecordsetPtr rs=NULL);
    static void DrawPhs(_RecordsetPtr rsza);
    static void RecordAppend(_RecordsetPtr rs, CString FldName, _variant_t fldValue);
    static void MakeTBDtmpzdj();
    static void MakeTmpCSLen();
    static void MakeZDJ_ZD(long zdjh=10000);
    static _variant_t PhsDrawNameGet(_variant_t ZDJNo);
    static void PhsMLmake(_variant_t ZDJNo);
    static void PhsDisplacementLoadINFOMake(LPCTSTR lpszTextStyle=_T("hz"),int iAlign=0,int iZDLOADtype=20);//TzDLOAD=20
    static void PhsYLBMake(long zdjh=10000);
    static void CreateTmpIDCustomIDTable();
    static void CreateTmpConnectTable();
    static void CreateTmpSPRPropertyTable(int /*Optional*/ SprMaxSerialNum);
    static void CreateTmpSPRPropertyTableUnfinish(int /*Optional*/ SprMaxSerialNum);
    static void SetSpringPhsInfo(_RecordsetPtr rsTmpSelSpring, _RecordsetPtr rsTZA, bool  bLineSPR,CString& mZdjType);
    static void AutoSelSpringNo(_RecordsetPtr rss,float fpgz ,float fpaz,float fyr,float fSPR_CHZBH,float& fTotalInstallCompressValue,
						long lSelSampleID, int nRepZdjType,int iSumDistOfSpring,bool BHintRepZdjType,CString strSpringOrder,
						CString strZdjType,bool bAutoApplyChzbh,CString strbnSPRINGproperty,CString strbnDiscSPRINGpropertyMaxDist);
    static void AutoSelConstantSpringNo(_RecordsetPtr rss,float fpgz,float fyr,bool bCSPRneedSpecialDesign,float fUseConstantSpringMinSurplusDist,
		                long lSelSampleID,CString strZdjType,int& iWholeSprNum,bool bAutoPASA,int nSumSerialNum,CString strbnHDproperty,CString strbnDisplacementSerial);
    static void SetPhsTK3();
    static void CalSpringParallelNum(CString& PipeA=CString(""));
    static void PreCalCrudeData(_RecordsetPtr rstbl,int nth = 1);
    static float sngSpringWeight(CString  strZdjType, _RecordsetPtr rstbl, float /*Optional*/ mvDist=-9999, float& /*Optional*/ sngTotalHeight=fltTmp);
    static float sngSpringWeightNormal(CString  strZdjType, _RecordsetPtr rstbl, float mvDist=-9999, float sngTotalHeight=-9999);
    static long CalRigidZDJType(_RecordsetPtr rstbl);
    static void CalfxSA(_RecordsetPtr rstbl);
    static void CalfxPA(_RecordsetPtr rstbl);
    static void CalDG(_RecordsetPtr rstbl);
    static void CalUnitX(_RecordsetPtr rstbl);
    static float CalUnitCovertCoef(_RecordsetPtr rstbl, int /*Optional*/ iUNIT=0);
    static void UpdateTZB4zdjh(CString FieldName,_variant_t FieldValue);
    static void VB_Cal(_RecordsetPtr  rs, long  zdjh,CFrmStatus &frmStatus,int nth = 1);
    static bool CalRodLength(_RecordsetPtr  rstbl, long  zdjh);
    static void SetBasePoint();
    static void ReadAutoPHSiniValue();
    static void InitializeCrudeData(_RecordsetPtr rs, bool  bFrmTxsrLoaded);
    static void CalPAfixZ1Z2(CString /*ByVal*/ dn, float /*ByVal*/ tmpSelPJG, float /*ByVal*/ tmpT0, int /*ByVal*/ mvariNumPart, _RecordsetPtr rsX);
    /*
     * ������connect���ݱ�ʱʹ�õ��ֶ�����,
     * Ĭ���ֶ���ID,��ʱֻ��Ҫ��connect���������û������ƥ��׼��,��������С
     * �û���ʱҲ�ɸ�ΪCustomID,
     * ���������͵���������ƥ��������ݱ�connect,�������ܴ�.
     */

	static SprInfo sSprInfo[16];
	static SprDirectionChar gSprDirectionChar;
    static const _TCHAR*	gcsCustomID;
    static const _TCHAR*	gcsID;
    static bool	gbStartedOnceAPHS;
    static CString	Unit_Force;
    static CString	Unit_Displacement;
    static CString	Unit_Momental;
    static _variant_t	gBookmark;
    static CString	Ax;
    static int	gintParallelNum;
    static int	gSumDistOfSpring;
    static CString	df1;
    static CString	gf1;
    static CString	df;
    static CString	gf;
    static CString	fx;
    static float	haz;
    static CString	dn;
    static CString	IDdn;
    static float	dj;//���⾶
    static float	dwB;
    static float	t0;
    static float	sngPEL;
    static CString	gn;
    static CString	IDgn;
    static float	sngSEL;
    static long	zdjh;
    static CString	dg;
    static float	gdw;
    static float	gdw1;

    static float	gdwx;
    static float	gdwz;
    static float	yr;
    static float	yr1;
    static float	xr;
    static float	zr;
    static float	yl;
    static float	xl;
    static float	zl;
    static float	dyr;
    static float	dxr;
    static float	dzr;
    static float	dyl;
    static float	dxl;
    static float	dzl;
    static float	dxa0;
    static float	dza0;
    static float	dxa;
    static float	dza;
    static float	gpzx;
    static float	gpzz;
    static float	pjg;
    static float	pgz;
    static float	paz;

	//�̶�֧�ܼ���ģ������ϵ������̬���ܵ���,kgf
	static float pxlc;
	static float pylc;
	static float pzlc;
	static float pxrc;
	static float pyrc;
	static float pzrc;
	static float mxlc;
	static float mylc;
	static float mzlc;
	static float mxrc;
	static float myrc;
	static float mzrc;
	static float PAfixH;

    static int	giNumRod;
    static float	Lspan;
    static float	WidthB;
    static float	WidthA;
    static CString	gstrMaterial;
    static CString	gsPhsArrangementDwgNo;
    static CString	gsPhsScheduleDwgNo;
    static CString	clb;
    static float	SpringRigid;
    static float	SpringLMax;
    static long	iSelSampleID;
	static bool gbSumRodByRoundSteel;
    /*
     * ����������������õģ�������function bFrmLoaded("FrmBmpSe")ȡ��
     * ��Ҫ����frmBmpSe��һ��װ��ʱmovePos������Ҫ����
     */
    static bool	pbFrmLoadedBmp;
    static int	SelInputMethod;
    static CString	CSZdjType;
    static CString	ZdjType;
    static bool	gbPhsIsCSPR;
    /*
     * ȫ���ַ�������TRIM�����Ŀո񣬻���COleVariant���ԡ�
     */
    static CString	gsOldPhsPASel;
    static CString	gsOldPhsPARTSel;
    static CString	gsOldPhsSASel;
    static CString	gsOldPhsSPRINGSel;
    static CString	gsOldPhsConstantSpringSel;
    static CString	gsOldPhsBoltsNutsSel;
   	//������֧���ܱ�׼�汾���Ƶ��������硰�����������Ժ��ˮ�ܵ�֧���ܸ���(English)��,
	//����Ӧ��������Ȼ�ǡ������������Ժ��ˮ�ܵ�֧���ܸ�����,������Ҫд����װͼ�ϣ��Ա㰲װʱ���Һ���߶ȡ�
	static CString	gsPhsPASel;//ѡ��Ĺܲ��淶����
    static CString	gsPhsPARTSel;//ѡ������Ӽ��淶����
    static CString	gsPhsSASel;//ѡ��ĸ����淶����
    static CString	gsPhsSPRINGSel;//ѡ��ĵ��ɹ淶����
    static CString	gsPhsConstantSpringSel;//ѡ��ĺ������ɹ淶����
    static CString	gsPhsBoltsNutsSel;//ѡ�����˨��ĸ�淶����

	//������֧���ܱ�׼�����ƣ��硰�����������Ժ��ˮ�ܵ�֧���ܸ�����
    static CString	gsPhsDescPASel;//ѡ��Ĺܲ��淶����
    static CString	gsPhsDescPARTSel;//ѡ������Ӽ��淶����	
    static CString	gsPhsDescSASel;//ѡ��ĸ����淶����
    static CString	gsPhsDescSPRINGSel;//ѡ��ĵ��ɹ淶����
    static CString	gsPhsDescConstantSpringSel;//ѡ��ĺ������ɹ淶����
    static CString	gsPhsDescBoltsNutsSel;//ѡ�����˨��ĸ�淶����

    static CString	gsCSPRmanufactory;//�����������쳧
    static CString	gsSPRmanufactory;//�������쳧
    static CString	gsCSPRUnitOfLoad;//�������ɺ��ص�λ
    static CString	gsSPRUnitOfLoad;//���ɺ��ص�λ
    static CString	tbnTZB;//֧���ܽ�����ݱ�����(һ����ZB)
    static CString	tbnBoltsNuts;//��˨��ĸ������
    static CString	tbnBoltsNutsID;//��˨��ĸID������
    static CString	tbnAttachment;//����������
    static CString	tbnAttachmentID;//����ID������
    static CString	tbnSABoltsNuts;//������˨��ĸ������
    static CString	tbnSABoltsNutsID;//������˨��ĸID������
    static CString	tbnBoltsSurplusLength;//��˨����ԣ��������
    static CString	tbnPA;//�ܲ�������
    static CString	tbnSA;//����������
    static CString	tbnPAfix;//�̶�֧�ܹܲ�������
    static CString	tbnPART;//���Ӽ�������
    static CString	tbnPAID;//�ܲ�ID������
    static CString	tbnPARTID;//���Ӽ�ID������
    static CString	tbnSAID;//����ID������
    static CString	tbnSectionSteel;//�͸ֱ�����
    static CString	tbnSectionSteelID;//�͸�ID������
    static CString	tbnLugInfo;//������Ϣ������
    static CString	tbnLugLenCal;//���ȼ��������
    static CString	tbnCONNECT;//���ұ�׼�������ӹ����һ����Ҫ������Ӧ����Ժ���������Դ���ĸ�����
    static CString	tbnHDCrude;//���ԭʼ���ݱ�����
    static CString	tbnHDproperty;//����������ݱ�����
    static CString	tbnHDid;//���ID��
    static CString	tbnDisplacementSerial;//���λ��ϵ�б�
    static CString	tbnCSPRDiameterSerial;//���ֱ��ϵ�б�
    static CString	tbnSPRDiameterSerial;//����ֱ��ϵ�б�
    static CString	tbnLugDiaOfCSPR;//�������ֱ����
    static CString	tbnSPRINGCrude;//����ԭʼ���ݱ�
    static CString	tbnSPRINGL5Crude;//�����ɽ���ĸ��
    static CString	tbnCSPRINGL5Crude;//���������ɽ���ĸ��
    static CString	tbnSPRINGproperty;//�����������ݱ�
    static CString	tbnDiscSPRINGpropertyMaxDist;//�������λ���������ݱ�
    static CString	tbnSPRINGid;//����ID��

    static CString	tbnAllowStress;//����Ӧ����

    static bool	gbH1IncludeB;//�������ݱ��и߶�ֵH1�������ɽ���ĸ��Ե�ߴ�B
    static bool	gH1Pos;//�������ݱ��и߶�ֵH1���ɽ���ĸ��λ��(�ϡ��С���)
    static bool	gH1distPos;
    static bool	gbCH1IncludeB;
    static bool	gbCH1IncludeT;
    static int	gCH1Pos;
    static int	gCH1distPos;
    static float	gsngTotalInstallCompressValue;
    static bool	gbByForce;
    static float	gUseConstantSpringMinSurplusDist;
    static float	gUseConstantSpringPercentSurplusDist;
    static float	gUseSpringMinDist;
    static float	gnSPR_CHZBH;//���ɺ��ر仯ϵ��
    static float	gsngRatioOfPjg2Pgz;
    static float	gnConnectTJ;
    static int	gnRodLenRoundVal;
    static int	gnRodInsertLenRoundVal;
	
    static bool	gbConnectHoleDimMatch;
    static bool	gbSPRAutoLugDia;
    static bool	gbCSPRAutoLugDia;
    static bool	gbAutoApplyChzbh;
    static bool	gbOutputSA;
    static bool	gbCalSAbyConservativenessMethod;
    static bool	gbCalSAbyPJG;
    static bool	gbCalPARTbyMaxLoad;
    static bool	gbAddPartWeight2PMax;
    static bool	gbAddPartWeight2CSPRPgz;
    static bool	gbAddPartWeight2SPRPgz;//2007.07.30
	static bool gbPAForceZero;//2007.09.25
	static bool gbMaxPgzPaz;//2007.09.03
    static bool	gbMinMaxDispByActualDisp;
    static bool	gbCalcCSPRHeight_BySelDisp;
    static bool	gbCalCSPRLugDiaByPjg;
    static bool	gbCSPRneedSpecialDesign;
    static bool	gbCSPRCanSpecialDesign;
    static bool	gbAutoAlignCSPRtoSA;
	static bool gbPipeClampBolt;
    /*
     * ���ܵ�֧��������p202 12.1.2(4)�롶�������糧��ˮ�ܵ���Ƽ����涨(DL/T 5054-1996)��p67
     * 7.5.4��ָ����
     * ����ֱ��DN<=50,d>=10mm;DN>=65,d>=12mm����Щ�涨�����е�֧���ܱ�׼ì�ܡ�
     * ����������Ժ��ˮ�ܵ�֧�����ֲ� ���ӵ���D4.76S�׾�13mm,ֻ����L8.10(d=10mm)���ӡ������涨ì�ܡ�
     * ��ˣ����ﲻ�ܰ��������涨������ֱ�����������ӳߴ��齫ʧ�ܡ�
     */
    static bool	gbLimitLugMinDia;//��������ֱ��
    static bool	gbSumAllVolume;//�������в���
    static bool	gbAutoPASA;//�Զ�ѡ��ܲ�����
    static bool	gbAutoApplyCSPRLugDia;//�Զ���Ӧ�������ֱ��
    static bool	gbNotPrompt_ACAD_SHIFT;//����ʾ�л���AutoCAD
    static int	gIsACAD;//ѡ����AutoCAD��ͼ
    static bool	gbNotPrompt_Catlog_OVERWRITE;//����ʾĿ¼����
    static int	giDrawSIZE_A3A4;//��װͼͼ��ʹ��A3/A4ͼ��0/1/2=�Զ�/A4/A3
    static int	giPhsOutINFOTableType;//֧���������Ϣ������ 
    static CString	gsPhsColor;//֧����ͼ����ɫ��0/1/2=white/green/auto
    static bool	gbAnnotateCSPR;//������ע���Լ����
    static bool	gbDraw_BOM;//Ҫ���������ϸ��
    static bool	gbDrawOVERWRITE;
    static bool	gbWorksWithBoltsNuts;//�����Դ������Ӽ����ӵ���˨��ĸ
    static bool	gbWorksWithSABoltsNuts;//�����Դ���������ӵ���˨��ĸ
    static float	gnDW_delta;//���ҹܾ���Χ����,0~7(%)
    static int	giAllowedMaxSerialNumPerSPR;//ÿ�����������������
    static int	giAllowedMaxSerialNumSPRs;//�������(�������)�����������
    static int	giSpringOrder;
    static CString	gstrSpringOrder;//����ѡ��Ĺ�������ֵ
    static CString	gstrSpringOrderSQL;//����ѡ��Ĺ���
    static bool	AutoOrdinateDrawing;//�Զ�������װͼ����λ�ã�ʹ���׼�հ״����롣
    static long	gnSEQyOffset;//���֮�����С��ֱ����
    static bool	gbDrawNoPreMethod;//֧����ͼ��ǰ׺����ʽ
    static bool	gbAutoSaveDrawing;//�Զ����浱ǰ֧����ͼֽ
    static long	glDrawNoStartNumber;//֧������ʼͼ��
    static long	glDWGPreMethod;//֧����ͼ���ļ���ǰ׺����ʽ
    static CString	gsDWGPre;//֧����ͼ���ļ���ǰ׺��
    static CString	gsDwgFN;//֧����ͼ���ļ���
    static int	giZDJHinDwg;//��ǰ����֧����
    static bool	blResizable;
    static CString	gsPhsTypeRigid;//����֧����
    static CString	gsPhsTypeSPR;  //����֧����
    static CString	gsPhsTypeCSPR; //��������֧����
    static int	giWholeSprNum;     //�ܵ�����
    static int	giSumSerialNum;	   //�ܴ�����
    static int	glNumSA;			//��������
    static int	iCSnum;				//����(Ԫ����)���۸���
    static CString	gsPartType;		//�������
    static CString	gsOldPartType;	//
    static CString	gsaSPRPos;		//��������λ��
    static bool	gbStopOLE;
    static bool	gbCalAllPhs;		//����ȫ��֧����
    static bool	gbStopCalAllPHS;	//ֹͣ����ȫ��֧����
    /*
     * pt1x,pt1y--ע�Ϳ����½�X,Y����
     * pt2x,pt2y--�����ϸ�����½�X,Y����
     * pt3x,pt3y--����λ�Ʊ����Ͻ�X,Y����
     */
    static double	VX_pt0x;
    static double	VX_pt0y;
    static double	VZ_pt0x;
    static float	pt1x;
    static float	pt1y;
    static float	pt2x;
    static float	pt2y;
    static float	pt3x;
    static float	pt3y;
    static float	Pt0X;
    static float	giDimOffset;	//�ߴ�ƫ��
    static float	giAttDxOffsetX;	//����Dxƫ��X
    static float	giAttDzOffsetX; //����Dzƫ��X
    static float	giAttDOffsetY;  //����Dƫ��Y
    /*
     * �����������飺�ַ���
     */
    static CString	iPhsSAfx[4];
    /*
     * ����������������ֱ�����飬���ڱ��������˵���Сֱ�����Ա�׼ȷѡ�����˺��������������
     * ��һ������GetphsBHandSizes���ҳ����������Сֱ���������Ѿ��Ͼ�ȷ��
     * �ڶ�������GetphsBHandSizes���������������Ҫ/��Ҫ�����ݱ���gbAutoApplyCSPRLugDia���ƣ���Ӧ�������ֱ����
     */
    static int	*PtypeDiameter;
	static int  PtypeIndexForDiameter;
    static int	* giCSPRindex;
	static int giCSPRindexSize;
    static int	glIDIndex;
    static long	glClassID;
    static long	glPAid;
    static long	glSAid;
    static CString	gsPhsName;//֧����������������
    static CString	gsPhsEnglishName;//֧��������Ӣ������
    static CString	ZdjTxName[20];//֧����������Ӣ�Ļ������
    //�Ƿ�ȡ��Ӧ�������������ĵ��ɽ��
	static int iRepZdjType;
	static bool bHintRepZdjType;//��AutoPHS����ĵ��ɺ�ȡ��Ӧ������AutoPSA����ĵ��ɺ�

	static int iNumCSVal;//�����۸�����ѡ������  0:�Զ�;1:��;2:˫;3:�����Զ�;

	static BOOL g_bPipeClampBoltDiameterChanged;//ָʾС���ش�ܾ��ܼ���˨�Ѿ�������
protected:

private:
	void MakeLISEGA_ConstantSPR_DH();

};
extern int giUPxyz;//���ϵ�������
extern bool gbHotStatus2Zero;//��̬������㵯��
//extern float gfDiameter;//��������ֱ������3�μ���֮��
#endif 
