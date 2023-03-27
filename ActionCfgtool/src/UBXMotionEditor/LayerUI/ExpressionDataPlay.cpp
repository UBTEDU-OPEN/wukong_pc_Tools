
#include "ExpressionDataPlay.h"
#include "configs.h"

#include <QDir>
#include <QMovie>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>


CExpressionPlayProcess::CExpressionPlayProcess(IUILayerManager* pLayerManager)
    : m_bStop(false)
    , m_bPause(false)
    , m_nStartTime(0)
    , m_nTotalTime(0)
{
    connect(this, SIGNAL(sigExpressionPlay(QString, int)), 
        (CUILayerManager*)pLayerManager->GetWidget(), SLOT(slotExpressionPlay(QString, int)));
}

CExpressionPlayProcess::~CExpressionPlayProcess()
{
    //
}

void CExpressionPlayProcess::PlayProcess(QString strFileName , int nFrameNum)
{
    emit sigExpressionPlay(strFileName, nFrameNum);

    qDebug() << "frame num:" << nFrameNum << endl;
}


void CExpressionPlayProcess::playExpress(QString strFileName, int nStartTime, int nBlockRunTime)
{
    m_strFileName = strFileName;
    m_nStartTime = nStartTime;
    m_nTotalTime = nBlockRunTime;

    m_bPause = false;
    m_bStop = false;

    wait();

    start();
}


void CExpressionPlayProcess::run()
{
    QString strFilePath = CConfigs::getUserDirPath(USERDIR_EXPRESS) + QDir::separator() + m_strFileName;
    int nCurTime = 0;
    int nCurFrame = 0;

    if (m_strFileName.contains(".gif"))
    {
        QMovie* pMovie = new QMovie(strFilePath);
        int nTotalFrames = pMovie->frameCount();

        int nFrameTime = m_nTotalTime * 1000 / nTotalFrames;  // us

        nCurFrame = nTotalFrames * m_nStartTime / m_nTotalTime;

        // 0号帧已有时间
        pMovie->jumpToFrame(nCurFrame);
        //for (int i = 0; i < nTotalFrames; ++i)
        //{
        //    nCurTime += pMovie->nextFrameDelay();
        //    pMovie->jumpToNextFrame();
        //    if (nCurTime >= m_nStartTime)
        //    {
        //        nCurFrame = i;
        //        break;
        //    }
        //}

        while (1)
        {
            if (nCurFrame >= nTotalFrames)
            {
                break;
            }
            if (m_bStop)
            {
                break;
            }
            if (m_bPause)
            {
                continue;
            }

            PlayProcess(m_strFileName, nCurFrame);
            //msleep(pMovie->nextFrameDelay());
            usleep(nFrameTime);
            pMovie->jumpToNextFrame();
            ++nCurFrame;
        }

        delete pMovie;
    }
    else if (m_strFileName.contains(".json"))
    {
        int nFps = 1;
        int nTotalFrames = 0;
        parseJsonFile(strFilePath, nFps, nTotalFrames);
        float fFrameTime = (float)1000 / nFps;
        for (int i = 0; i < nTotalFrames; ++i)
        {
            nCurTime = i * fFrameTime;
            if (nCurTime >= m_nStartTime)
            {
                nCurFrame = i;
                break;
            }
        }

        while (1)
        {
            if (nCurFrame >= nTotalFrames)
            {
                break;
            }
            if (m_bStop)
            {
                break;
            }
            if (m_bPause)
            {
                continue;
            }

            PlayProcess(m_strFileName, nCurFrame);
            usleep(fFrameTime * 1000);
            ++nCurFrame;
        }
    }

    m_bPause = false;
    m_bStop = false;
}


void CExpressionPlayProcess::pauseExpress()
{
    m_bPause = true;
}


void CExpressionPlayProcess::resumeExpress()
{
    m_bPause = false;
}


void CExpressionPlayProcess::stopExpress()
{
    m_bStop = true;
}


void CExpressionPlayProcess::parseJsonFile(QString strFilePath, int& nFps, int& nTotalFrames)
{
    QByteArray byteArray;
    QFile file(strFilePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    byteArray = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);
    if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
    {
        if (document.isObject())
        {
            QJsonObject object = document.object();
            if (object.contains("op"))
            {
                QJsonValue value = object.value("op");
                if (value.isDouble())
                {
                    nTotalFrames = value.toInt();
                }
            }
            if (object.contains("fr"))
            {
                QJsonValue value = object.value("fr");
                if (value.isDouble())
                {
                    nFps = value.toInt();
                }
            }
        }
    }
}





CExpressionDataPlay::CExpressionDataPlay(IUILayerManager* pLayerManager)
{
    m_pExpressionProcess = new CExpressionPlayProcess(pLayerManager);
}


CExpressionDataPlay::~CExpressionDataPlay()
{
    if (m_pExpressionProcess)
    {
        delete m_pExpressionProcess;
        m_pExpressionProcess = NULL;
    }
}


void CExpressionDataPlay::StartPlay(int nEndTime, void* PlayHandle, int nBlockIndex /* = 0  */, int nTimeOffset /* = 1 */)
{
    SetProcessHander(this);
    CGroupDataPlayThread::StartPlay(nEndTime, NULL, nBlockIndex, nTimeOffset);
}


void CExpressionDataPlay::ProcessPlayActionBlock(CActionBlockData* pActionBlock, int nRunTime, int nTimeScale, bool bEnd)
{
    if (NULL == pActionBlock)
    {
        return;
    }

    WCHAR* pwchName = pActionBlock->GetDescription();
    QString strFileName = QString::fromStdWString(pwchName);

    int nTotalTime = pActionBlock->GetRunTime() * nTimeScale;
    int nStartTime = nTotalTime - nRunTime;

    if (m_pExpressionProcess)
    {
        m_pExpressionProcess->playExpress(strFileName, nStartTime, nTotalTime);
    }
}


void CExpressionDataPlay::OnStopPlay()
{
    if (m_pExpressionProcess)
    {
        m_pExpressionProcess->stopExpress();
    }
}


void CExpressionDataPlay::StopPlay()
{
    if (m_pExpressionProcess)
    {
        m_pExpressionProcess->stopExpress();
    }
}


void CExpressionDataPlay::PausePlay()
{
    if (m_pExpressionProcess)
    {
        m_pExpressionProcess->pauseExpress();
    }
}


void CExpressionDataPlay::ResumePlay()
{
    if (m_pExpressionProcess)
    {
        m_pExpressionProcess->resumeExpress();
    }
}


