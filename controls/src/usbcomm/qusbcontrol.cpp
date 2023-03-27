#include "qusbcontrol.h"

QUsbControl::QUsbControl()
{
    m_pHidApi = Q_NULLPTR;
    m_nCurHidDevId = 0;
}

QUsbControl::~QUsbControl ()
{
    close ();
}

QUsbControl* QUsbControl::getInstance()
{
    static QUsbControl control;
    return &control;
}

bool QUsbControl::open ()
{
    if ( isOpen() )
        close ();
    m_pHidApi = new QHidApi;
    m_nCurHidDevId = m_pHidApi->open(VENDORID, PRODUCTID);
    if ( m_nCurHidDevId == 0 ){
        return false;
    }
    return true;
}

bool QUsbControl::close ()
{
    if ( m_pHidApi ){
        m_pHidApi->close(m_nCurHidDevId);
        m_nCurHidDevId = 0;
        delete m_pHidApi;
        m_pHidApi = NULL;
    }
    return true;
}

bool QUsbControl::isOpen ()
{
    return (m_nCurHidDevId != 0 ? true : false);
}

bool QUsbControl::write (const QByteArray& data)
{
    if ( m_pHidApi ) {
        if ( m_pHidApi->write(m_nCurHidDevId, data) > 0 ){
            return true;
        }
    }
    return false;
}
