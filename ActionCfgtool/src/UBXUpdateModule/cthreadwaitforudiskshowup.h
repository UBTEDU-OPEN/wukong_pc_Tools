#ifndef CTHREADWAITFORUDISKSHOWUP_H
#define CTHREADWAITFORUDISKSHOWUP_H

#include <QThread>

class CThreadWaitForUdiskShowup : public QThread
{
        Q_OBJECT

public:
    CThreadWaitForUdiskShowup(QObject *pParent = NULL);
    ~CThreadWaitForUdiskShowup();

signals:
    void sigIsUdiskShowUp(bool bShowup);

protected:
    virtual void run() override;
};

#endif // CTHREADWAITFORUDISKSHOWUP_H
