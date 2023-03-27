#ifndef QHIDAPI_P_H
#define QHIDAPI_P_H
/*
Copyright (C) [year] by Simon Meaden <[simonmeaden@virginmedia.com]>

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <QObject>
#include <QMap>
#include <QList>
#include <QVariant>

#include "qhiddeviceinfo.h"
#include "hidapi.h"

class QHidApi;

class QHidApiPrivate {
public:
    QHidApiPrivate(ushort vendorId, ushort productId, QHidApi *parent);
    ~QHidApiPrivate();

    QList<QHidDeviceInfo> enumerate(ushort vendorId=0x0, ushort productId=0x0);

    quint32 open(ushort vendor_id, ushort product_id, QString serial_number=QString());
    quint32 open(QString path);
    void close(quint32 id);
    QByteArray read(quint32 id);
    QByteArray read(quint32 id, int timeout);
    int write(quint32 id, QByteArray data, quint8 reportNumber);
    int write(quint32 id, QByteArray data);
    bool setBlocking(quint32 id);
    bool setNonBlocking(quint32 id);
    QByteArray featureReport(quint32 id, uint reportId);
    int sendFeatureReport(quint32 id, quint8 reportId, QByteArray data);
    QString manufacturerString(quint32 id);
    QString productString(quint32 id);
    QString serialNumberString(quint32 id);
    QString indexedString(quint32 id, int index);
    QString error(quint32 id);
    quint32 nextId();
    int init();
    int exit();
    hid_device *findId(quint32 id);
    quint32 openProduct(ushort vendorId, ushort productId, QString serialNumber);
    quint32 openNewProduct(ushort vendorId, ushort productId, QString serialNumber);

    static const int MAX_STR = 255;

    quint32 mVendorId, mProductId;
    quint32 mNextId;
    QList<QHidDeviceInfo> mDeviceInfoList;
    /*
     * map of vendorId -> productId.
     */
    QMultiMap<ushort, QMultiMap<ushort, QVariant> > mVendorMap;
    /*
     * map of serialNumber -> id.
     */
    QMap<QString, quint32> mSerDevices;
    /*
     * map of QString path -> id.
     */
    QMap<QString, quint32> mPathMap;
    /*
     * map of id -> handle.
     */
    QMap<quint32, hid_device*> mIdDeviceMap;
    /*
     * reverse of idDeviceMap. Used to check if we already heve a device opened..
     */
    QMap<hid_device*, quint32> mDeviceIdMap;

private:
    QHidApi *q_ptr;

    Q_DECLARE_PUBLIC(QHidApi)

};

#endif // QHIDAPI_P_H
