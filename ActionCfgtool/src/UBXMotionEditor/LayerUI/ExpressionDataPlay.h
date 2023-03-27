
#ifndef EXPRESSION_DATA_PLAY_H
#define EXPRESSION_DATA_PLAY_H

//#include <QThread>
#include "motioneditor_global.h"
//#include "IGroupDataPlay.h"
#include "LayerUI/ILayerUI.h"
//#include "ActionBlockDataManager.h"
#include "GroupDataPlayThread.h"
#include "UpdateTimeLineProcess.h"

#define TYPE_GIF    0
#define TYPE_JSON   1

class MOTIONEDITOR_EXPORT CExpressionPlayProcess : public QThread /*, public QObject*/
{
    Q_OBJECT
public:
    CExpressionPlayProcess(IUILayerManager* pLayerManager);
    ~CExpressionPlayProcess();

    void PlayProcess(QString strFileName , int nFrameNum);

    void playExpress(QString strExpressName, int nStartTime, int nBlockRunTime);

    void pauseExpress();

    void resumeExpress();

    void stopExpress();

    void run();

private:
    bool m_bStop;
    bool m_bPause;

    QString m_strFileName;

    int m_nStartTime;       // ms
    int m_nTotalTime;       // ms

    void parseJsonFile(QString strFilePath, int& nFps, int& nTotalFrames);

signals:
    void sigExpressionPlay(QString strFileName , int nFrameNum);
};



class CExpressionDataPlay : public CGroupDataPlayThread , public CActionBlockProcess
{
public:
    CExpressionDataPlay(IUILayerManager* pLayerManager);
    ~CExpressionDataPlay();


private:
    CExpressionPlayProcess* m_pExpressionProcess;

    QString m_strFileName;

public:
    virtual void StartPlay(int nEndTime, void* PlayHandle, int nBlockIndex = 0 , int nTimeOffset = 1 );

    virtual void ProcessPlayActionBlock(CActionBlockData* pActionBlock, int nRunTime, int nTimeScale, bool bEnd = false);

    virtual void OnStopPlay();
    virtual void StopPlay();
    virtual void PausePlay();
    virtual void ResumePlay();
};

#endif