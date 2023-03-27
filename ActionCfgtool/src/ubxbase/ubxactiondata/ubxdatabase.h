#ifndef _UBXBASE_H_
#define _UBXBASE_H_

#include "globaldefs.h"
#include "rawdata.h"
#include "baseblock.h"
#include "portconnectmap.h"
#include <map>
#include <list>
#include <vector>

typedef std::map <unsigned long,void*> ULONG_POINTER_MAP;
typedef std::pair <unsigned long, void*> ULONG_PAIR;

#define UBX_MEMCPY_INC(t, s) memcpy(&t, s, sizeof(t));s+=sizeof(t);
UBXACTIONDATA_EXPORTS int AddVectorData(std::vector<char>* pVerData, char* pData, int nLen);
UBXACTIONDATA_EXPORTS void ModifyVertorData(std::vector<char>& pVerData, int nPos, char* pData, int nLen);
UBXACTIONDATA_EXPORTS int CopyVertorToPointer(std::vector<char>& pVerData, char* pData);
#endif  //_UBXBASE_H_
