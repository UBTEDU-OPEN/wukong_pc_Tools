#include "stable.h"
#include "TimeLayer.h"
#include "IUILayerManager.h"

CTimeLayer::CTimeLayer(void)
{

}

CTimeLayer::~CTimeLayer(void)
{
}

bool CTimeLayer::Connect(QWidget* managerHandle)
{
    m_ManagerHandle = managerHandle;
    return true;
}

bool CTimeLayer::ReleaseConnection()
{
    return true;
}


bool CTimeLayer::GetData(char** ppData, int& nLen)
{
    if (ppData == NULL)
        return false;

    *ppData = new char[8];
    nLen = 8;

    int nScaleValue = m_pLayerManager->GetTimeScaleValues();
    int nCellCount = m_pLayerManager->GetCellCount();

    char* p = *ppData;
    memcpy(p, &nScaleValue, sizeof(int));
    p += sizeof(int);
    memcpy(p, &nCellCount, sizeof(int));

    return true;
}

bool CTimeLayer::SetData(char* pData, int nLen)
{
    int nScaleValue;
    int nCellCount;

    if (nLen == 4)
    {
        nScaleValue = 20;

        int nTimes = 0;
        memcpy(&nTimes, pData, 4);
        nCellCount = nTimes/20;
    }
    else
    {
        memcpy(&nScaleValue, pData, sizeof(int));
        memcpy(&nCellCount, pData+sizeof(int), sizeof(int));

    }

    m_pLayerManager->SetTimeScaleValues(nScaleValue);
    m_pLayerManager->SetCellCount(nCellCount);

    return true;
}

// 取得类型，具体见宏定义
emActionLayerType CTimeLayer::GetType()
{
    return ACTION_LAYER_TIME;
}

QString CTimeLayer::GetLayerName()
{
    return QObject::tr("Time");
}
