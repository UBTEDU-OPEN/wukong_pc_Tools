/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CVideoCapture
* 创建时间：20160830
* 文件标识：
* 文件摘要：video capture img of video file
*
* 当前版本：1.0.0.0
* 作    者：asu
*/
#include "videocapture.h"
#include <QFile>
#include <QImage>
#include <QTimer>
#include <QPainter>
#include <QTransform>
#include <QVideoSurfaceFormat>

const int VIDEO_CAPTURE_INTERNAL = 500; // 视频捕获时播放器暂停到播放的时间间隔

CVideoItem::CVideoItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , imageFormat(QImage::Format_Invalid)
    , framePainted(false)
{

}

CVideoItem::~CVideoItem()
{

}

QRectF CVideoItem::boundingRect() const
{
    return QRectF(QPointF(0,0), surfaceFormat().sizeHint());
}

QList<QVideoFrame::PixelFormat> CVideoItem::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle)
    {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    }
    else
    {
        return QList<QVideoFrame::PixelFormat>();
    }
}

void CVideoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

bool CVideoItem::start(const QVideoSurfaceFormat &format)
{
    if (isFormatSupported(format))
    {
        imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
        imageSize = format.frameSize();
        framePainted = true;

        QAbstractVideoSurface::start(format);
        prepareGeometryChange();

        return true;
    }
    else
    {
        return false;
    }
}

void CVideoItem::stop()
{
    currentFrame = QVideoFrame();
    framePainted = false;

    QAbstractVideoSurface::stop();
}

bool CVideoItem::present(const QVideoFrame &frame)
{
    if (!framePainted)
    {
        if (!QAbstractVideoSurface::isActive())
        {
            setError(StoppedError);
        }

        return false;
    }
    else
    {
        currentFrame = frame;
        emit sigVideoFrame(currentFrame);

        return true;
    }
}

CVideoCapture::CVideoCapture(QObject *parent)
    : QObject(parent)
    ,m_pEventLoop(NULL)
    ,m_pVideoItem(NULL)
    ,m_pVideoWidget(NULL)
    ,m_pVideoProbe(NULL)
    ,m_nCaptureFrame(0)
    ,m_nCurrentFrame(0)
    ,m_bOkSetProbeToMediaPlayer(false)
    ,m_bCaptureDone(false)
{
    m_pEventLoop = new QEventLoop();    
    m_pVideoProbe = new QVideoProbe;

    m_bOkSetProbeToMediaPlayer = false;
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
    // 使用QVideoProbe方式，在mac没有videoFrameProbed信号导致无法
    // 捕获视频帧，此外，使用QVideoProbe方式捕获到的视频帧图片，在hwx
    // 电脑上经过map或获取的图片数据大小没有问题，但是在周志平和测试人员
    // 电脑上经过map后获取的图片数据只有Y部分，没有U/V部分，导致数据转换
    // 时崩溃，因此统一采用GraphicsView方式输出视频内容，捕获帧数据，该
    // 方式在mac和windows上都没有问题
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//    m_bOkSetProbeToMediaPlayer = m_pVideoProbe->setSource(&m_mediaPlayer);//wind is ok, but mac is not Ok
    if(m_bOkSetProbeToMediaPlayer)
    {
        m_pVideoWidget = new QVideoWidget;
        m_pVideoWidget->hide();

        connect(m_pVideoProbe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(processFrame(QVideoFrame)));
        m_mediaPlayer.setVideoOutput(m_pVideoWidget);
    }
    else
    {
        m_pVideoItem = new CVideoItem;

        connect(m_pVideoItem, &CVideoItem::sigVideoFrame, this, &CVideoCapture::processFrame);
        m_mediaPlayer.setVideoOutput(m_pVideoItem);
    }

    connect(&m_mediaPlayer, &QMediaPlayer::audioAvailableChanged, this, &CVideoCapture::slotOnVideoAutioAvailableChanged);
}

CVideoCapture::~CVideoCapture()
{
    delete m_pEventLoop;
    delete m_pVideoWidget;
    delete m_pVideoProbe;

    if(m_bOkSetProbeToMediaPlayer) {
        delete m_pVideoItem;//m_mediaPlayer own it
    }
}

bool CVideoCapture::captureFrame(const QString &strVideoPath,
                                 const QString &strImgPath,
                                 const int nCaptureFrame,
                                 const int nOuttimeMsec)
{
    if(strVideoPath.isEmpty() || strImgPath.isEmpty() || (nCaptureFrame < 0) || (nOuttimeMsec <= 0))
    {
        emit sigCaptureFrame(false);
        return false;
    }

    m_bCaptureDone = false;
    m_nCurrentFrame = 0;
    m_strVideoPath = strVideoPath;
    m_strImgPath = strImgPath;
    m_nCaptureFrame = nCaptureFrame;    
    m_pVideoProbe->flush();

    if(QFile::exists(m_strImgPath))
    {
        QFile::remove(m_strImgPath);
    }

    m_mediaPlayer.stop();

    if(QFile::exists(m_strVideoPath))  // 本地视频文件
    {
        m_mediaPlayer.setMedia(QUrl::fromLocalFile(m_strVideoPath));
        m_mediaPlayer.play();//
    }
    else // 网络视频文件
    {
        m_mediaPlayer.setMedia(QUrl(m_strVideoPath));
        m_mediaPlayer.play();//

    }

    /////////////////////////////////////////////////////////////
    // 直接设置静音，在Mac系统下无效, Qt:Bug: https://bugreports.qt.io/browse/QTBUG-48154
    // 目前在audioAvailableChanged(bool available)的槽函数中将播放器设置为静音
    m_mediaPlayer.setMuted(true);
    /////////////////////////////////////////////////////////////

    QTimer::singleShot(nOuttimeMsec, this, &CVideoCapture::onTimeout);
    QTimer::singleShot(VIDEO_CAPTURE_INTERNAL, this, &CVideoCapture::slotOnTryCaptureTime);

    m_pEventLoop->exec();

    return QFile::exists(m_strImgPath);
}

bool CVideoCapture::processFrame(QVideoFrame frame)
{
    if(m_nCurrentFrame > m_nCaptureFrame)
    {
        m_bCaptureDone = true;
        m_mediaPlayer.stop();
        emit sigCaptureFrame(false);

        m_pEventLoop->quit();

        return false;
    }

    else if(frame.isValid() && (m_nCurrentFrame == m_nCaptureFrame))
    {
        QVideoFrame cloneFrame(frame);
        bool bOk = cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
        if(bOk)
        {
            int nWidth = cloneFrame.width();
            int nHeight = cloneFrame.height();
            uchar *pImageData = NULL;
            QVideoFrame::PixelFormat pixelFormat = cloneFrame.pixelFormat();
            QImage::Format format = QVideoFrame::imageFormatFromPixelFormat(pixelFormat);
            if (pixelFormat == QVideoFrame::Format_NV12) // MP4等视频格式返回QVideoFrame::Format_NV12类型
            {
                uchar *pYUV420 = NULL;
                int nYUV420Len = 0;
                if (!convertNV12ToYUV420P(cloneFrame.bits(),nWidth, nHeight, pYUV420, nYUV420Len)) // NV12格式转YUV420格式
                {
                    return false;
                }

                uchar *pRgb32 = NULL;
                int nRgb32Len = 0;
                if (!convertYUV420ToRGB32(pYUV420, nWidth, nHeight, pRgb32, nRgb32Len))// YUV420格式转RGB32格式
                {
                    return false;
                }
                format = QImage::Format_RGB32;
                pImageData = pRgb32;
            }
            else
            {
                if(QImage::Format_Invalid == format)
                {
                    format = QImage::Format_Indexed8;
                }
                pImageData = cloneFrame.bits();
            }

            QImage image(pImageData,
                         nWidth,
                         nHeight,
                         format);

            QPixmap pixmap = QPixmap::fromImage(image);

            if(QFile::exists(m_strImgPath))
            {
                QFile::remove(m_strImgPath);
            }
            pixmap.save(m_strImgPath);
            cloneFrame.unmap();            

            m_bCaptureDone = true;
            m_mediaPlayer.stop();
            emit sigCaptureFrame(true);
            m_pEventLoop->quit();
        }
    }

    ++m_nCurrentFrame;

    return true;
}

void CVideoCapture::onTimeout()
{
    //AlphaRobotLog::getInstance()->printError("Capture video frame timeout!");
    m_bCaptureDone = true;
    m_mediaPlayer.stop();
    emit sigCaptureFrame(false);
    m_pEventLoop->quit();
}

void CVideoCapture::slotOnVideoAutioAvailableChanged(bool bAvailable)
{
    m_mediaPlayer.setMuted(bAvailable);
}

void CVideoCapture::slotOnTryCaptureTime()
{
    // 如果已经超时，播放器已经停止，此时就不要再启动了
    if (!m_bCaptureDone)
    {
        m_mediaPlayer.pause();
        m_mediaPlayer.play();
        QTimer::singleShot(VIDEO_CAPTURE_INTERNAL, this, &CVideoCapture::slotOnTryCaptureTime);
    }
}

bool CVideoCapture::convertYUV420ToRGB32(const uchar *yuv, int width, int height, uchar *&rgb, int &realLen)
{
    if(width < 1 || height < 1 || yuv == NULL )
        return false;
    const long len = width * height;
    unsigned char * yData = (unsigned char *)yuv;   //!y分量起始地址
    unsigned char * uData = &yData[len];			//!u分量起始地址
    unsigned char * vData = &uData[len>>2];			//!v分量起始地址


    const int nRgbLen = width *height *4;			//!RGB32 数据长度
    uchar *pRgb = new uchar[nRgbLen];				//!分配RGB32内存
    memset(pRgb,0,nRgbLen);

// 	uchar *pA = rgb+3;								//!alp 指针
// 	uchar *pR = rgb+2;								//!R起始指针
// 	uchar *pG = rgb+1;								//!G起始指针
// 	uchar *pB = rgb;								//!B起始指针

    int bgr[3];
    int yIdx,uIdx,vIdx,idx;
    for(int i = 0 ; i < height ; i++ )
    {
        for(int j = 0;j < width; j++ )
        {
            //!YUV420格式 每4个Y 共用一个UV。
            yIdx = i * width + j;				//!Y起始索引		* * * * * * * *
            uIdx = (i/2) * (width/2) + (j/2);	//!U起始索引		 @   @   @   @
            vIdx = uIdx;						//!V起始索引		* * * * * * * *

// 			bgr[0] = (int)(yData[yIdx] + 1.732446 * (uData[vIdx] - 128));		//b分量
// 			bgr[1] = (int)(yData[yIdx] - 0.698001 * (uData[uIdx] - 128) - 0.703125 * (vData[vIdx] - 128)); //g分量
// 			bgr[2] = (int)(yData[yIdx] + 1.370705 * (vData[uIdx] - 128));		//!r分量

            bgr[0] = (int)(yData[yIdx] + 1.779 * (uData[vIdx] - 128));		//b分量
            bgr[1] = (int)(yData[yIdx] - 0.3455 * (uData[uIdx] - 128) - 0.7169 * (vData[vIdx] - 128)); //g分量
            bgr[2] = (int)(yData[yIdx] + 1.4075 * (vData[uIdx] - 128));		//!r分量


// 			*pB = (uchar)bgr[0]< 0? 0:bgr[0]>255? 255:bgr[0];
// 			*pG = (uchar)bgr[1]<0 ? 0:bgr[1]>255? 255:bgr[1];
// 			*pR = (uchar)bgr[2]<0 ? 0:bgr[2]>255? 255:bgr[2];
// 			*pA = 255;

// 			pB += 4;
// 			pG += 4;
// 			pR += 4;
// 			pA += 4;

            //!RGB赋值 每个rgb占用32位，4个字节
            for(int k = 0;k < 3;k++)
            {
                idx = (i * width + j) * 4 + k ;		//!RGB数据索引	RGB32数据存储格式 [b][g][r][alph] 4个字节为一个RGB32数据
                if(bgr[k] >= 0 && bgr[k] <=255)
                    pRgb[idx] = bgr[k];
                else
                    pRgb[idx] = (bgr[k] < 0)? 0:255;
            }
        }
    }
    rgb = pRgb;
    realLen = nRgbLen;
    return true;
}

bool CVideoCapture::convertNV12ToYUV420P(uchar *pNV12, int width, int height, uchar *&pOutYuv420p, int &outLen)
{
    if(NULL == pNV12)
    {
        return false;
    }

    //!获取NV12个分量起始地址
    const int nLen = width * height;
    uchar *pY12=pNV12;
    uchar *pU12=pNV12 + nLen;
    uchar *pV12=pNV12 + nLen + 1;

    //!分配YUV420P空间
    outLen = nLen * 3/2;
    uchar *pYUV420p = new uchar[nLen * 3/2];
    if(NULL == pYUV420p)
    {
        return false;
    }
    memset(pYUV420p,0,nLen*3/2);

    //!获取YUV420P各分量起始地址
    uchar *pY=pYUV420p;
    uchar *pU=pY+nLen;
    uchar *pV=pU + (nLen>>2);

    QString strLog = "";
    strLog = QString("width: %1, height: %2, nLen: %3, outLen: %4, height/2: %5, width/2: %6, nLen>>2: %7").arg(width).arg(height).arg(nLen).arg(outLen).arg(height/2).arg(width/2).arg(nLen>>2);
    //AlphaRobotLog::getInstance()->printDebug(strLog);

    //!Y分量赋值 Y分量大小width*height U分量大小width/2*height/2 V分量大小width/2*height/2
    memcpy(pY, pY12, nLen);
    for(unsigned int nh = 0 ; nh < height/2 ; nh++ )
    {
        for(unsigned int nw = 0 ; nw < width/2 ; nw++ )
        {
//            AlphaRobotLog::getInstance()->printDebug(QString("nh: %1, nw: %2").arg(nh).arg(nw));
            *pU = *pU12;	//!U分量赋值
//            AlphaRobotLog::getInstance()->printDebug(QString("hahahhah: it is ok").arg(nh).arg(nw));
            *pV = *pV12;	//!V分量赋值
            pU +=1;			//!U分量地址++
            pV +=1;			//!V分量地址++
            pU12 += 2;		//!NV12 U,V分量地址交替排序，所以各分量地址+2
            pV12 += 2;
        }
    }
    pOutYuv420p = pYUV420p;
    return true;
}
