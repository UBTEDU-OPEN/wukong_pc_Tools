#include "UpdateTimeLineProcess.h"

CUpdateTimeLineProcess::CUpdateTimeLineProcess(IUILayerManager* pLayerManager, int nGroupStartTime)
{
    QObject::connect(this, &CUpdateTimeLineProcess::SigMotionPositionChanged, (CUILayerManager*)pLayerManager->GetWidget(), &CUILayerManager::slotSelectLineChanged);
    m_nGroupTimeCount = nGroupStartTime;
}

CUpdateTimeLineProcess::~CUpdateTimeLineProcess(void)
{
}

void CUpdateTimeLineProcess::OnUpdateTimeLine(quint64 nPosition)
{
    m_nGroupTimeCount += nPosition;
    emit SigMotionPositionChanged(m_nGroupTimeCount);
}
