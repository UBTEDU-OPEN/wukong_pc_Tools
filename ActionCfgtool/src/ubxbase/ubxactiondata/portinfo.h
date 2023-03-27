#ifndef _PORTINFO_H_
#define _PORTINFO_H_

#include <string>
#include "globaldefs.h"
#include "RawData.h"

#define DIR_INPUT	false
#define DIR_OUTPUT	true

//// OnStart
//#define PORT_TYPE_START		0
//// OnCancel
//#define PORT_TYPE_CANCEL	1
//// OnStop
//#define PORT_TYPE_STOP		2
//// GENERAL
//#define PORT_TYPE_GENERAL	3

enum {
    //OnStart
    PORT_TYPE_START,
    //OnCancel
    PORT_TYPE_CANCEL,
    //OnStop
    PORT_TYPE_STOP,
    //General
    PORT_TYPE_GENERAL,
    //Logic
    //Next
    PORT_TYPE_NEXT,
    //Loop
    PORT_TYPE_LOOP,
    //Default
    PORT_TYPE_DEFAULT,
    //true false
    PORT_TYPE_TRUE,
    PORT_TYPE_FALSE,
};

#define MAX_PORTINFO_SIZE	20


class UBXACTIONDATA_EXPORTS CPortInfo : public CRawData
{
public:
    CPortInfo(void);
    virtual ~CPortInfo(void);

public:
    virtual bool GetRawData(char** ppData, int &nLen);
    virtual bool SetRawData(char* pData, int nLen);

    void SetIndex(int nIndex);
    int GetIndex();
    void SetDirection(bool bDirection);
    bool GetDirection();
    void SetPortType(int nPortType);
    int GetPortType();
    void SetCaption(WCHAR* pstrCaption);
    WCHAR * GetCaption();

    std::string GetCaptionByType(const int type) const;
protected:
    int		m_nIndex;			// 连接点的序号
    bool	m_bDirection;		// 方向标志， TRUE 输出， false 输入
    int		m_nDataType;		// 端口的数据类型0，为数字。1为字符串，2混合类型
    int		m_nPortType;		// 端口类型：0:OnStart: 1: OnCancel: 2: OnStop; 3: general。主要用于绘制用，以表示不同的符号
    char*	m_pDataStore;		// 用于保存输入输出传入或者传出的内容
    int		m_nDataStoreLen;	// 保存的长度
    bool	m_bMuiltiOutput;	// 用于作为输出口用时，是否可以多次输出，比如FOR循环。如果为FALSE，则功能块执行结束调用OnStop接口

    WCHAR	m_strCaption[MAX_PORTINFO_SIZE];	// 端口名字
};
#endif  //_PORTINFO_H_
