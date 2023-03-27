#ifndef UBXTHREAD_H
#define UBXTHREAD_H

#include <QThread>
#include "qutils_global.h"


class QUTILS_EXPORT UBXThread : public QThread
{
    Q_OBJECT
 public:
    UBXThread();
    ~UBXThread();
    bool StartThread();
  virtual   void run();
//    Q_DECL_OVERRIDE {
//        QString result;
//        emit resultReady(result);
//    }
public slots:
    void ThreadMainLoop();
 private:
    UBXThread *m_thread;
    bool m_bRun;
    bool m_bStopView;
};

#endif // UBXTHREAD_H
