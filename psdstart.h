// PSDstart.h
//

#if !defined(AFX_PSDSTART_H__66EB6429_712C_11D5_8752_00D009619765__INCLUDED_)
#define AFX_PSDSTART_H__66EB6429_712C_11D5_8752_00D009619765__INCLUDED_


/*
 * ���ڴ�ģ�����Ҫ��Դ�����ṩ�������ߣ������е���ʾ���ֲ��ŵ���Դ�ļ��и����㡣
 */

#include "resource.h"
#include "time.h"

class PSDstart
{
public:

    static void ConvertAllDb(_variant_t dVer, _variant_t Pwd);
    static void tmpT();
    static void tmpSub2DB();
    static void tmpTelmdb2BitWareDefaultDBF();
    static bool Start();
    static CTime	sTime;
    static CTime	eTime;
protected:


private:


};

#endif /* PSDstart_h */
