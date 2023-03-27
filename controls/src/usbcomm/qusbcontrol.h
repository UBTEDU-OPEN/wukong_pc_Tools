#ifndef QUSBCONTROL_H
#define QUSBCONTROL_H
#include <QString>
#include "qhidapi.h"

/**< usb 定义 */
#define VENDORID   0x0483       /**< 厂商标识 */
#define PRODUCTID  0x5750       /**< 产品标识 */

#define UsbControl QUsbControl::getInstance()

class QUsbControl
{
public:
    QUsbControl ();
    virtual ~QUsbControl ();
    static QUsbControl* getInstance();

public:
    /**
     * @brief 打开hid dev
     * @return
     */
    bool    open ();

    /**
     * @brief 关闭hid dev
     * @return
     */
    bool    close ();

    /**
     * @brief hid dev 是否打开
     * @return
     */
    bool    isOpen ();

    bool write (const QByteArray& data);

private:
    QHidApi*  m_pHidApi;
    quint32   m_nCurHidDevId;
};

#endif // QUSBCONTROL_H
