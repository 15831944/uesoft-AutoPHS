#if !defined MODREGISTRY_H
#define MODREGISTRY_H

extern float	GetRegKey(LPCTSTR pszKey, LPCTSTR pszName,const float Default=0.0);
extern long		GetRegKey(LPCTSTR pszKey, LPCTSTR pszName,const long Default=0);
extern CString	GetRegKey(LPCTSTR pszKey, LPCTSTR pszName,const CString Default="");
extern CString	GetRegKey(LPCTSTR pSoftShareKey,LPCTSTR pszKey, LPCTSTR pszName,const CString Default="");//20071026
extern CString	GetUSER_RegKey(LPCTSTR pzsPath, LPCTSTR pszKey, LPCTSTR pszName,const CString Default="");
extern bool		GetRegKey(LPCTSTR pszKey, LPCTSTR pszName,const bool Default=false);
extern int		GetRegKey(LPCTSTR pszKey, LPCTSTR pszName,const int Default);

extern const	_TCHAR szCAESARIIKey [];//CAESARIIע���ؼ���
extern const	_TCHAR szAutoPSAKey [];//AutoPSAע���ؼ���
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName,const int vValue);
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName,const CString vValue);
extern void     SetRegValue(LPCTSTR pSoftShareKey,LPCTSTR pszKey, LPCTSTR pszName,const CString vValue);//20071016
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName,const float vValue);
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName,const bool vValue);
extern void		SetRegValue(LPCTSTR pszKey, LPCTSTR pszName, const double vValue);
extern bool UEGetValue(HKEY KeyRoot,CString keyName,CString ValueName,CString& DefValue);
extern int UEGetSettingInstallNumber(CString key,int  defValue=-1);
extern void UESaveSettingInstallNumber(CString key,int  defValue);
extern bool RegSetValueEx(HKEY KeyRoot,CString keyName,CString ValueName,CString regValue);
extern void AutoReg();

extern  const _TCHAR szSoftwareKey[];//����ע���λ�ã�
#endif