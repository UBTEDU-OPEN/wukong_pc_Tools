#include "stdafx.h"
#include <QMouseEvent>
#include <QDir>
#include "ubxrobot3dwidget.h"
#include "FbxPlayer.h"
#include "ubxrobot3ddisplay.h"
#include "ubxrobot3dconstants.h"
#include "configs.h"
#include <QDebug>
UBXRobot3DWidget::UBXRobot3DWidget(QWidget *parent, IUBXRobot3DViewEvent *pEvent, IUBXRobotViewControlNotify* pNotify)
    : QGLWidget(parent),
      m_pIUBXRobot3DViewEvent(pEvent),
      m_pNotify(pNotify),
      m_bEnableShow(false)
{
    m_pDisplay = NULL;

#ifdef Q_OS_WIN
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    this->setMaximumWidth(Constants::MAX_WIDTH);
    this->setMaximumHeight(Constants::MAX_HEIGHT);
    this->setMinimumWidth(Constants::MIN_WIDTH);
    this->setMinimumHeight(Constants::MIN_HEIGHT);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    connect(this, &UBXRobot3DWidget::SigUpdateNodeDate, this, &UBXRobot3DWidget::UpdateNodeDate);
    this->hide();
}

void UBXRobot3DWidget::initializeGLContext()
{
    QString strFilePathName = CConfigs::getAppSubDirPath(SUBDIR_ROBOTMODEL) + QDir::separator() + MODEL_DATA_NAME;
    QByteArray baFBX = strFilePathName.toUtf8();
    char *szFBXFileName =baFBX.data();
    qDebug() << strFilePathName;

    QString strXmlPathName = CConfigs::getAppSubDirPath(SUBDIR_ROBOTMODEL) + QDir::separator() + MODEL_XML_NAME;
    QByteArray baXml = strXmlPathName.toUtf8();
    char *szXmlFileName =baXml.data();
    qDebug() << strXmlPathName;

    m_pDisplay = new UBXRobot3DDisplay(context());

    this->makeCurrent();
    int ratio = windowHandle()->devicePixelRatio();
    InitFbxPlay(szFBXFileName,szXmlFileName,this, m_pDisplay, ratio);
    doneCurrent();
    context()->moveToThread(&m_thread);
    connect(this, &UBXRobot3DWidget::nextFrame, m_pDisplay, &UBXRobot3DDisplay::onNextFrame, Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::angleChanged, m_pDisplay, &UBXRobot3DDisplay::onAngleChanged, Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::resized, m_pDisplay, &UBXRobot3DDisplay::onSize,Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::mouseEvent, m_pDisplay, &UBXRobot3DDisplay::onMouseEvent, Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::mouseMotion, m_pDisplay, &UBXRobot3DDisplay::onMouseMotion, Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::mouseWheel, m_pDisplay, &UBXRobot3DDisplay::onMouseWheel, Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::clickNode, m_pDisplay, &UBXRobot3DDisplay::onClickNode, Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::reset, m_pDisplay, &UBXRobot3DDisplay::onReset, Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::setPlayState, m_pDisplay, &UBXRobot3DDisplay::onSetModelPlayState, Qt::QueuedConnection);
    connect(this, &UBXRobot3DWidget::updateData, m_pDisplay, &UBXRobot3DDisplay::onUpdateData);
    connect(m_pDisplay, &UBXRobot3DDisplay::SigFirstFrameFinished, this, &UBXRobot3DWidget::SigFirstFrameFinished);
    connect(m_pDisplay, &UBXRobot3DDisplay::SigFirstFrameFinished, this, &UBXRobot3DWidget::OnFirstFrameFinished);

    connect(this, &UBXRobot3DWidget::sigSoftSimulate, m_pDisplay, &UBXRobot3DDisplay::slotSoftSimulate, Qt::DirectConnection);

    m_pDisplay->moveToThread(&m_thread);
    m_thread.start();
    //startTimer(33);
}


UBXRobot3DWidget::~UBXRobot3DWidget()
{
    m_thread.quit();
    m_thread.wait();
    if(m_pDisplay)
    {
        m_pDisplay->deleteLater();
        m_pDisplay = NULL;
    }
}

bool UBXRobot3DWidget::Control_GetMotorDatas(char **pData, int &nLen, emActionLayerType eActionType)
{
    if(NULL == m_pDisplay)
        return false;

    // 注意手部舵机号：1-4；腿部：5-10；头部：11-14
    int nMotorsCount    = m_pDisplay->getMotorCount();
    int nStartMotorID   = 1;

    switch (eActionType)
    {
    case ACTION_LAYER_HAND:
        {
            nMotorsCount    = HAND_MOTOR_COUNT;
            nStartMotorID   = 1;
        }
    	break;
    case ACTION_LAYER_LEG:
        {
            nMotorsCount    = LEG_MOTOR_COUNT;
            nStartMotorID   = 1 + HAND_MOTOR_COUNT;
        }
        break;
    case ACTION_LAYER_HEAD:
        {
            nMotorsCount    = HEAD_MOTOR_COUNT;
            nStartMotorID   = 1 + HAND_MOTOR_COUNT + LEG_MOTOR_COUNT;
        }
        break;
    default:
        break;
    }

    //数据大小 + 每个舵机的数据 + 运动曲线类型(mini增加运动曲线)
    int len = sizeof(int) + nMotorsCount * sizeof(MOTOR_DATA) + sizeof(int);
    *pData = new char[len];
    char* p = *pData;
    if(p == NULL)
    {
        *pData = NULL;
        nLen = 0;
        return false;
    }
    //将数据大小保存在最前面的一个int
    memcpy(p, &len, sizeof(len));
    p += sizeof(len);

    MOTOR_DATA* pInfo = (MOTOR_DATA*)p;
    QMap<int, int> items = m_pDisplay->getMotorInfo();
    int i=0;

    QMap<int, int>::const_iterator itor = items.find(nStartMotorID);
    while (i < nMotorsCount && itor != items.end())
    {
        pInfo[i].nID    = itor.key();
        pInfo[i].nAngle = itor.value();
        ++i;
        ++itor;
    }

    int nCurveModel = 0;
    memcpy(p + nMotorsCount * sizeof(MOTOR_DATA) , &nCurveModel , sizeof(int));

    //QMapIterator<int, int> it(items);
    //while(it.hasNext())
    //{
    //    it.next();
    //    pInfo[i].nID = it.key();
    //    pInfo[i].nAngle = it.value();
    //    i++;
    //}
    nLen = len;
    return true;
}

void UBXRobot3DWidget::Control_SetMotorDatas(char *pData, int nLen, int runTime)
{
    if(pData == NULL || nLen <= 4)
    {
        return;
    }
    // 前int个字节是数据长度，因此+sizeof(nLen)跳过，即是数据开始的地址
    MOTOR_DATA* pMotor = (MOTOR_DATA*)(pData + sizeof(nLen));
    int nCount = (nLen - sizeof(nLen)) / sizeof(MOTOR_DATA);

    // Add by jianjie 2017/10/19 解析曲线类型，最后4个字节
    int nCurveType = 0;
    memcpy(&nCurveType , pData + sizeof(nLen) + nCount * sizeof(MOTOR_DATA) , sizeof(nCurveType));

    // 仿真数据
    emit sigSoftSimulate(pMotor , nCount , runTime , nCurveType);

    // Delete by jianjie 2017/11/10
    //机器人运动
    //if(g_pRobotController && g_pRobotController->GetPortOpenState())
    //{
    //    g_pRobotController->Req_RobotDebug(pMotor, nCount, runTime, runTime);
    //}
}

int UBXRobot3DWidget::Control_GetMotorAngles(char *pData, int nDataLen, int nMotorID)
{
    if(pData == NULL || nDataLen <= 4)
    {
        return -1;
    }

    MOTOR_DATA* pMotor = (MOTOR_DATA*)(pData + sizeof(nDataLen));
    int nCount = (nDataLen - sizeof(nDataLen)) / sizeof(MOTOR_DATA);
    for(int i = 0; i < nCount; i++)
    {
        if(pMotor[i].nID == nMotorID)
        {
            return pMotor[i].nAngle;
        }
    }
    return -1;
}

void UBXRobot3DWidget::Control_SetViewMotorDatasManual(char *pData, int nLen, int runTime)
{
    if(pData == NULL || nLen <= 4)
    {
        return;
    }

    MOTOR_DATA* pMotor = (MOTOR_DATA*)(pData + sizeof(nLen));
    int nCount = (nLen - sizeof(nLen)) / sizeof(MOTOR_DATA);
    for(int i = 0; i < nCount; i++)
    {
        emit angleChanged(pMotor[i].nID, pMotor[i].nAngle, runTime, TAC_MotionEditerManual);
    }
}

void UBXRobot3DWidget::Control_SetMotorAngle(int nID, int nAngle)
{
    if(m_mapPowerState.contains(nID))
    {
        m_mapPowerState[nID] = POWERLOST;
        emit angleChanged(nID, nAngle, 200, TAC_MotionEditor);
    }
}

void UBXRobot3DWidget::Control_SetMotorPowerState(int nMotorID, int nState)
{
    m_mapPowerState[nMotorID] = nState;
}

void UBXRobot3DWidget::Control_SetAllMotorPowerState(int nState)
{
    for(int i = 0; i < GetMotorCount(); ++i)
    {
        m_mapPowerState[i + 1] = nState;
    }
}


int UBXRobot3DWidget::GetMotorCount()
{
    if(m_pDisplay)
    {
        return m_pDisplay->getMotorCount();
    }
    return 0;
}

void UBXRobot3DWidget::UpdateNodeDate(const char *szNodeName, float angle, float offset, TypeAngleChanged nChangeType)
{
    if(m_pNotify)
        m_pNotify->UpdateNodeDate(szNodeName, angle, offset, nChangeType);
}


void UBXRobot3DWidget::initializeGL()
{
}

void UBXRobot3DWidget::resizeGL(int width, int height)
{
}

void UBXRobot3DWidget::paintGL()
{
}

void UBXRobot3DWidget::paintEvent(QPaintEvent *event)
{
}

void UBXRobot3DWidget::resizeEvent(QResizeEvent *)
{
    emit resized(windowHandle()->devicePixelRatio() * width(), windowHandle()->devicePixelRatio() * height());
}

void UBXRobot3DWidget::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void UBXRobot3DWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if(m_pDisplay && this->isVisible())
    {
        emit nextFrame();
    }
}

void UBXRobot3DWidget::mousePressEvent(QMouseEvent *event)
{
    QMouseEvent *glevent=new QMouseEvent(event->type(),
                                         mapFromGlobal(event->globalPos()),
                                         event->button(),
                                         event->buttons(),
                                         event->modifiers());
    emit mouseEvent(glevent);
}

void UBXRobot3DWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QMouseEvent *glevent=new QMouseEvent(event->type(),
                                         mapFromGlobal(event->globalPos()),
                                         event->button(),
                                         event->buttons(),
                                         event->modifiers());
    emit mouseEvent(glevent);
}

void UBXRobot3DWidget::mouseMoveEvent(QMouseEvent *event)
{
    QMouseEvent *glevent=new QMouseEvent(event->type(),
                                         mapFromGlobal(event->globalPos()),
                                         event->button(),
                                         event->buttons(),
                                         event->modifiers());
    emit mouseMotion(glevent);
}

void UBXRobot3DWidget::wheelEvent(QWheelEvent *event)
{
    QWheelEvent *glevent = new QWheelEvent(event->pos(),
                                           mapFromGlobal(event->globalPos()),
                                           event->pixelDelta(),
                                           event->angleDelta(),
                                           event->delta(),
                                           event->orientation(),
                                           event->buttons(),
                                           event->modifiers(),
                                           event->phase(),
                                           event->source());
    emit mouseWheel(glevent);
}

void UBXRobot3DWidget::OnFirstFrameFinished()
{
    if(!this->isVisible() && m_bEnableShow)
    {
        this->show();
    }
}

void UBXRobot3DWidget::showEvent(QShowEvent *event)
{
    QGLWidget::showEvent(event);

    if(m_pDisplay && (event->type() == QEvent::Show) && this->isVisible())
    {
        m_pDisplay->setVisible(true);
    }
}

void UBXRobot3DWidget::hideEvent(QHideEvent *event)
{
    QGLWidget::hideEvent(event);

    if(m_pDisplay && (event->type() == QEvent::Hide))
    {
        m_pDisplay->setVisible(false);
    }
}
