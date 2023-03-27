#include "ubxthread.h"

#ifdef _DEBUG
#include <QDebug>
#endif

UBXThread::UBXThread() :
m_thread(NULL),
    m_bRun(false),
    m_bStopView(false)
{

}
UBXThread::~UBXThread()
{
    if(m_thread)
    {
        delete m_thread;
        m_thread = NULL;
    }
}

bool UBXThread::StartThread()
{
   if(!m_thread)
   {
       m_thread = new UBXThread();
//       connect(UBXThread, &UBXThread::resultReady, this, &MyObject::handleResults);
//       connect(UBXThread, &UBXThread::finished, m_thread, &QObject::deleteLater);
//       workerThread->start();
   }
   return true;
}

void UBXThread::run()
{

//    m_bRun = true;
//    while (!GetFileLoadStatus())
//    {
//        sleep(10);
//    }

//    while(true)
//    {
//        if(m_bStopView)
//        {
//             qDebug()<<"Exit 3d view"<<endl;
//             break;
//        }
//        else
//        {
//             glutMainLoopEvent();
//             sleep(100);
//        }
//    }
}

void UBXThread::ThreadMainLoop()
{

//    glutMainLoopEvent();
}
