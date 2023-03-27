#ifndef QHIDAPI_H
#define QHIDAPI_H

/*
Copyright (C) [year] by Simon Meaden <[simonmeaden@virginmedia.com]>

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <QObject>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QByteArray>
#include <QList>

#include "qhidapi_global.h"
#include "qhiddeviceinfo.h"

class QHidApiPrivate;

class QHIDAPISHARED_EXPORT QHidApi : public QObject {

    Q_OBJECT

public:
    QHidApi(ushort vendorId, QObject *parent=0);
    QHidApi(ushort vendorId, ushort productId, QObject *parent=0);
    QHidApi(QObject *parent=0);
    ~QHidApi();

    QList<QHidDeviceInfo> enumerate(ushort vendorId=0x0, ushort productId=0x0);

    quint32 open(ushort vendor_id, ushort product_id, QString serial_number=QString());
    quint32 open(QString path);
    void close(quint32 deviceId);
    QByteArray read(quint32 deviceId);
    QByteArray read(quint32 id, int timeout);
    int write(quint32 id, QByteArray data, quint8 reportId);
    int write(quint32 id, QByteArray data);
    bool setBlocking(quint32 id);
    bool setNonBlocking(quint32 id);
    QByteArray featureReport(quint32 id, uint reportId);
    int sendFeatureReport(quint32 id, quint8 reportId, QByteArray data);
    QString manufacturerString(quint32 deviceId);
    QString productString(quint32 id);
    QString serialNumberString(quint32 id);
    QString indexedString(quint32 id, int index);
    QString error(quint32 id);

private:
    QHidApiPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QHidApi)
};

#endif // QHIDAPI_H
