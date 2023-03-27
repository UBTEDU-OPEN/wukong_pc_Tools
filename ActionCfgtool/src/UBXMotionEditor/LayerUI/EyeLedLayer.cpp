#include "EyeLedLayer.h"

CEyeLedLayer::CEyeLedLayer(void)
{
}

CEyeLedLayer::~CEyeLedLayer()
{

}

//bool CEyeLedLayer::Connect(QWidget* managerHandle)
//{
//    CLayerCommonImpl::Connect(managerHandle);
//    return true;
//}

//bool CEyeLedLayer::ReleaseConnection()
//{
//    CLayerCommonImpl::ReleaseConnection();

//    return true;
//}

//bool CEyeLedLayer::GetData(char** ppData, int& nLen)
//{
//    CLayerCommonImpl::GetData(ppData, nLen);

//    return true;
//}

//bool CEyeLedLayer::SetData(char* pData, int nLen)
//{
//    CLayerCommonImpl::SetData(pData, nLen);

//    return true;
//}

// 取得类型，具体见宏定义
emActionLayerType CEyeLedLayer::GetType()
{
    return ACTION_LAYER_EYE;
}

QString CEyeLedLayer::GetLayerName()
{
    QString strName = QObject::tr("Eye");
    return strName;
}

void CEyeLedLayer::CreateOptionHandler()
{
    if (m_pOptionHandler)
    {
        m_pOptionHandler->ReleaseConnection();
        delete m_pOptionHandler;
        m_pOptionHandler = NULL;
    }

    CreateEyesLayOptionHandler(&m_pOptionHandler);
    m_pOptionHandler->Connect(m_ManagerHandle, this);
}
