#ifndef _FLOWCHARTBLOCK_H_
#define _FLOWCHARTBLOCK_H_

#include "globaldefs.h"
#include "baseblock.h"
#include "portconnectmap.h"
#include "processhandle.h"
#include <list>

class UBXACTIONDATA_EXPORTS CFlowChartBlock : public CProcessHandle, CBaseBlock
{
public:
	CFlowChartBlock(void);
	~CFlowChartBlock(void);

protected:
    std::list<CBaseBlock*> m_BlockList;
    std::list<CPortConnetMap*> m_ConnectList;

public:
	virtual void OnOutPut(char* pData, int nLen);
};
#endif  //_FLOWCHARTBLOCK_H_
