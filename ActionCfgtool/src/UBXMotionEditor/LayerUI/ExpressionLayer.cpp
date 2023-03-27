
#include "ExpressionLayer.h"



CExpressionLayer::CExpressionLayer()
{
}


CExpressionLayer::~CExpressionLayer()
{
}


emActionLayerType CExpressionLayer::GetType()
{
    return ACTION_LAYER_EXPRESSION;
}


QString CExpressionLayer::GetLayerName()
{
    QString strName = QObject::tr("Expression");

    return strName;
}


void CExpressionLayer::CreateOptionHandler()
{
    if ( m_pOptionHandler )
        m_pOptionHandler->Connect(m_ManagerHandle, this);
}
