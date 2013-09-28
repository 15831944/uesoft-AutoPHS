// ModEncrypt.h
//

#if !defined(ModEncrypt_h)
#define ModEncrypt_h

#include "resource.h"


class ModEncrypt
{
public:
	static DWORD GetSaveDogNO();
	static DWORD GetNHDogNo();
	static void EncryptLogout();
	//static long gLngSavedMicroDogID;
	//static long gIngVersionID;
    static bool CombineEncryptedFile();
    static bool SplitEncryptedFile();
    static void MakeEncrypt();
    static long MakeEncryptFile(CString  p, CString  CompanyName);
    static void EncryptCompanyName(LPCSTR  strPassWord, bool  bGetPassWord);
    static void VerifyEncryptFile(/*CString &strPassWord*/);
    static void VerifyID();
    static bool CheckLegalUser();
    static void CheckInstallDate();
    /*
     * �����ļ�����//
     */
    static const char*	FilejmBASE0;
    static const char*	FilejmBASE1;
    static const char*	FilejmCompanyName0;
    static const char*	FilejmCompanyName1;
    static const char*	FilejmCombinedXYZ;
    static const char*	FilePsw;
    static const char*	FileIns;
    static const char*	FileOutDate;
    static const char*	FileFlag;
    static const char*	FileWinCom;
    static const char*	FileWinDir;
    /*
     * �Ϸ��û�
     */
    static bool	gbLegalUser;
    static bool	gbLegalStandardUser;//�Ϸ���׼���û�
    static bool	gbLegalLearningUser;//�Ϸ�ѧϰ���û�
    static bool	gbLegalDemoUser;//�Ϸ���ʾ���û�
    static CString	gstrLegalCompanyName;
    static long	glngUseDemoVersionDays;//��ʾ��ʵ��ʹ������
    static long	glngLimitedDemoVersionDays;//��ʾ��ʹ����������

    static DWORD	gLngVersionID;//�汾��־(��ʾ��/ѧϰ��/��׼��=1234/1235/1236)
    static long	glngMicroDogID;//��������ˮ�ű�־(ÿ������������Ψһ��)

    static long	gLngSavedMicroDogID;//����ļ�������ˮ�ű�־(�ڼ����ļ��У�ÿ������������Ψһ��)
    
	static CString	gstrDBZdjCrudePassWord;//�ϲ���֧���ܱ�׼ԭʼ���ݿ�����
    static CString	gstrDBProductPassWord;//֧���ܳ����ṩ�Ĳ�Ʒԭʼ���ݿ�����
    static const char*	conStrDBPasswordSuffix;

protected:


private:


};

#endif /* ModEncrypt_h */
