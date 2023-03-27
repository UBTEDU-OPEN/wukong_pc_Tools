#ifndef CUPDATETIMELINEPROCESS_H
#define CUPDATETIMELINEPROCESS_H

#include "motioneditor_global.h"
#include <QObject>
#include "UILayerManager.h"

class MOTIONEDITOR_EXPORT CUpdateTimeLineProcess: public QObject
{    
    Q_OBJECT
public:
    CUpdateTimeLineProcess(IUILayerManager* pLayerManager, int nGroupStartTime);
    ~CUpdateTimeLineProcess(void);
private:
    int m_nGroupTimeCount;     //用于累加统计当前仿真块的时间位置
//  int m_nGroupStartTime;     //仿真组的开始时间
public:
    void OnUpdateTimeLine(quint64 nPosition);
signals:
    void SigMotionPositionChanged(quint64 nPosition);

};

#endif // CUPDATETIMELINEPROCESS_H
