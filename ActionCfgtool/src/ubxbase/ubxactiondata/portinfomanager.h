#ifndef _PORTINFOMANAGER_H_
#define _PORTINFOMANAGER_H_

#include "portinfo.h"
#include "globaldefs.h"
#include <vector>
#include <memory>
using std::vector;
using std::shared_ptr;

class UBXACTIONDATA_EXPORTS CPortInfoManager :public CRawData
{
public:
    CPortInfoManager(void);
     ~CPortInfoManager(void);

protected:
    std::vector<CPortInfo *> m_vecInput;

public:
    int GetUnUsePortID();

public:
    // 添加PortInfo到列表
    bool AddPortInfoToList(CPortInfo* pInfo, bool bChangeIndex = false);
    bool DeletePortInfoFromList(std::string const &caption);
    bool ModifyPortInfoFromList(std::string const & caption, std::string const & waitModifyStr);
    bool DeletePortInfoByType(int nPortType);
    CPortInfo* FindPortByCaption(std::string const & caption);
    // 清除列表
    void ClearPortInotList(void);

    // 获取PORT的个数
    int GetCount();
    // 获取CPortInfo
    CPortInfo* GetAt(int i);

    // CRawData 接口实现
    virtual bool GetRawData(char** ppData, int &nLen);
    virtual bool SetRawData(char* pData, int nLen);
};
#endif  //_PORTINFOMANAGER_H_
