#ifndef _PORTCONNETMAP_H_
#define _PORTCONNETMAP_H_

#include "globaldefs.h"
#include "rawdata.h"

class UBXACTIONDATA_EXPORTS CPortConnetMap : public CRawData
{
public:
    CPortConnetMap(void);
    virtual ~CPortConnetMap(void);

//protected:
    int m_nOutputItemID;			// 该层的资源ID，如果为OnStart点，则为-1
    int	m_nOutputIndex;				// 输出脚的序号
    int m_nInputItemID;				// 输入脚块的资源ID，如果是为本块的输入脚，则为-1
    int m_nInputIndex;				// 输入脚序号

public:
    virtual bool GetRawData(char** ppData, int &nLen);
    virtual bool SetRawData(char* pData, int nLen);
};
#endif  //_PORTCONNETMAP_H_
