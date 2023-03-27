#ifndef _RAWDATA_H_
#define _RAWDATA_H_

#include "globaldefs.h"

//#ifndef __MACH__
//typedef wchar_t WCHAR;
//#else
//typedef unsigned short WCHAR;
//#endif

class UBXACTIONDATA_EXPORTS CRawData
{
public:
	virtual bool GetRawData(char** ppData, int &nLen) = 0;
	virtual bool SetRawData(char* pData, int nLen) = 0;
};
#endif  //_RAWDATA_H_
