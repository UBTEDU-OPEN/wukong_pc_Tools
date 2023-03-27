#include "EarLedLayer.h"

CEarLedLayer::CEarLedLayer(void)
{

}

CEarLedLayer::~CEarLedLayer(void)
{
}

//bool CEarLedLayer::Connect(QWidget* managerHandle)
//{

//    CLayerCommonImpl::Connect(managerHandle);

//    return true;
//}

//bool CEarLedLayer::ReleaseConnection()
//{
//    CLayerCommonImpl::ReleaseConnection();

//    return true;
//}

//bool CEarLedLayer::GetData(char** ppData, int& nLen)
//{
//    CLayerCommonImpl::GetData(ppData, nLen);

//    return true;
//}

//bool CEarLedLayer::SetData(char* pData, int nLen)
//{
//    CLayerCommonImpl::SetData(pData, nLen);

//    return true;
//}

// 取得类型，具体见宏定义
emActionLayerType CEarLedLayer::GetType()
{
    return ACTION_LAYER_EAR;
}

QString CEarLedLayer::GetLayerName()
{
    QString strName = QObject::tr("Ear");
    return strName;
}

void CEarLedLayer::CreateOptionHandler()
{
    if (m_pOptionHandler)
    {
        m_pOptionHandler->ReleaseConnection();
        delete m_pOptionHandler;
        m_pOptionHandler = NULL;
    }

    CreateEarsLayOptionHandler(&m_pOptionHandler);
    m_pOptionHandler->Connect(m_ManagerHandle, this);
}


