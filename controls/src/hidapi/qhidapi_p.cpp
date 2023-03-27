#include "qhidapi_p.h"
#include "qhidapi.h"
/*
Copyright (C) [year] by Simon Meaden <[simonmeaden@virginmedia.com]>

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

QHidApiPrivate::QHidApiPrivate(ushort vendorId, ushort productId, QHidApi *parent) :
    mVendorId(vendorId),
    mProductId(productId),
    mNextId(1),
    q_ptr(parent) {
    init();
    enumerate(vendorId, productId);
}

QHidApiPrivate::~QHidApiPrivate() {
    exit();
}

/*!
 * \brief Get The Manufacturer String from a HID device.
 *
 * \param id A quint32 device id.
 *
 * \return a QString containing the manufacturers name string, otherwise an empty QString.
 */
QString QHidApiPrivate::manufacturerString(quint32 id) {
    wchar_t buf[MAX_STR];
    hid_device *dev = findId(id);

    int rep = hid_get_manufacturer_string(dev, buf, MAX_STR);

    if (rep != -1) {

        QString result = QString::fromWCharArray(buf);

        return result;
    }

    return QString();
}

/*!
 * \brief Get The Product String from a HID device.
 *
 * \param id A quint32 device id.
 *
 * \return a QString containing the product name string, otherwise an empty QString.
 */
QString QHidApiPrivate::productString(quint32 id) {
    wchar_t buf[MAX_STR];
    hid_device *dev = findId(id);

    int rep = hid_get_product_string(dev, buf, MAX_STR);

    if (rep != -1) {

        QString result = QString::fromWCharArray(buf);

        return result;
    }

    return QString();
}

/*!
 * \brief Get The Serial Number String from a HID device.
 *
 * \param id A quint32 device id.
 *
 * \return a QString containing the Serial number string, otherwise an empty QString.
 */
QString QHidApiPrivate::serialNumberString(quint32 id) {
    wchar_t buf[MAX_STR];
    hid_device *dev = findId(id);

    int rep = hid_get_serial_number_string(dev, buf, MAX_STR);

    if (rep != -1) {

        QString result = QString::fromWCharArray(buf);

        return result;
    }

    return QString();
}

/*!
 * \brief Get a string from a HID device, based on its string index.
 *
 * \param id A quint32 device id.
 * \param index The int index of the string to get.
 *
 * \return a QString containing the indexed string, otherwise an empty QString.
 */
QString QHidApiPrivate::indexedString(quint32 id, int index) {
    wchar_t buf[MAX_STR];
    hid_device *dev = findId(id);

    int rep = hid_get_indexed_string(dev, index, buf, MAX_STR);

    if (rep != -1) {

        QString result = QString::fromWCharArray(buf);

        return result;
    }

    return QString();
}

/*!
 * \brief initialises the library.
 *
 * Initialises the hidapi library. Calling it is not strictly necessary, as it will be called automatically by
 * enumerate() and any of the open_*() functions if it is needed. This function should be called at the
 * beginning of execution however, if there is a chance of HIDAPI handles being opened by different threads simultaneously.
 *
 * \return 0 if successful, otherwise returns -1;
 * \see enumerate()
 */
int QHidApiPrivate::init() {
    int res = hid_init();
    return res;
}

/*!
 * \brief Finalize the HIDAPI library.
 *
 * This function frees all of the static data associated with HIDAPI. It should be called at the end of
 * execution to avoid memory leaks.
 *
 * \return This function returns 0 on success and -1 on error.
 */
int QHidApiPrivate::exit() {
    int res = hid_exit();
    return res;
}

/*!
 * \brief Enumerates the HID Devices.
 *
 * This function returns a list of all the HID devices attached to the system which match
 * vendorId and productId. If vendorId is set to 0 then any vendor matches. If productId
 * is set to 0 then any product matches. If vendorId and productId are both left out, then
 * all HID devices will be returned.
 *
 * \code
 *     enumerate();
 * \endcode
 * will return all devices.
 * \code
 *     enumerate(0xafaf);
 * \endcode
 * will return all devices by the selected manufacturer id.
 *\code
 *     enumerate(0xafaf, 0x0735);
 * \endcode
 * will return the all devices of the specified manufacturer and product id's.
 *
 * \param vendorId - an optional unsigned int vendor id
 * \param productId - an optional unsigned int product id.
 * \return a QList<HidDeviceInfo> containing all relevant devices, or an empty list if no devices match.
 */
QList<QHidDeviceInfo> QHidApiPrivate::enumerate(ushort vendorId, ushort productId) {
    hid_device_info *info = hid_enumerate(vendorId, productId);
    mDeviceInfoList.clear();

    while (info != NULL) {
        QHidDeviceInfo i;
        i.path = QString(info->path);
        i.vendorId = info->vendor_id;
        i.manufacturerString = QString::fromWCharArray(info->manufacturer_string);
        i.productId = info->product_id;
        i.productString = QString::fromWCharArray(info->product_string);
        i.releaseNumber = info->release_number;
        i.serialNumber = QString::fromWCharArray(info->serial_number);
#if defined(Q_OS_WIN32) || defined(Q_OS_MAC)
        i.usagePage = info->usage_page;
        i.usage = info->usage;
#endif
        i.interfaceNumber = info->interface_number;
        mDeviceInfoList.append(i);
        info = info->next;
    }

    hid_free_enumeration(info);

    return mDeviceInfoList;
}

/*!
 * \brief Open a HID device using a Vendor ID (VID), Product ID (PID) and optionally a serial number.
 *
 * Opens a HID device with the supplied VID, PID and optionally serial number string. If serial_number
 * is NULL, the first device with the specified VID and PID is opened. The method returns an id number
 * which should be retained as it is used to identify the device that you wish to access.
 *
 * \param vendorId The Vendor ID (VID) of the device to open.
 * \param productId The Product ID (PID) of the device to open.
 * \param serialNumber The Serial Number of the device to open (Optionally NULL).
 * \return returns an quint32 id number, or 0 if unsuccessful.
 */
quint32 QHidApiPrivate::open(ushort vendorId, ushort productId, QString serialNumber) {

    QList<QMultiMap<ushort, QVariant> > products = mVendorMap.values(vendorId);
    if (products.isEmpty()) {
        return openNewProduct(vendorId, productId, serialNumber);
    } else {
        return openProduct(vendorId, productId, serialNumber);
    }
    return 0;
}

/*!
 * \brief Closes the specified device if it exists, otherwise this command is ignored.
 *
 * \param id - the quint32 id for the device.
 */
void QHidApiPrivate::close(quint32 id) {
    hid_device *dev = findId(id);
    if (dev != NULL) {
        hid_close(dev);
    }
}

hid_device *QHidApiPrivate::findId(quint32 id) {
    if (mIdDeviceMap.contains(id)) {
        hid_device* dev = mIdDeviceMap.value(id);
        return dev;
    }
    return NULL;
}

/*!
 * \brief  Read an Input report from a HID device into a QByteArray.
 *
 * Input reports are returned to the host through the INTERRUPT IN endpoint. The first byte will contain the Report number if the device uses numbered reports.
 * It should be noted that the write function returns data as unsigned char values but a QByteArray stores signed char's. To recover the report as raw unsigned
 * characters you should use reinterpret_cast<uchar*>(bytearray.data()).
 *
 * \param id A quint32 device id.
 *
 * \return Returns the data in a QByteArray. If no packet was available to be read and the handle is in non-blocking mode, Returns the data in a QByteArray.
 */
QByteArray QHidApiPrivate::read(quint32 id) {
    hid_device *device = findId(id);

    if (device != NULL) {
        unsigned char buf[65];

        int rep = hid_read(device, buf, 65);

        if (rep > 0) {
            QByteArray data(reinterpret_cast<char*>(buf), rep);
            return data;
        }
    }

    return QByteArray();
}

/*!
 * \brief  Read an Input report from a HID device into a QByteArray,  with timeout.
 *
 * Input reports are returned to the host through the INTERRUPT IN endpoint.
 * The first byte will contain the Report number if the device uses numbered reports.
 * If no packet was availableto be read within timeout milliseconds and the device
 * is in non-blocking mode.
 *
 * It should be noted that the write function returns data as unsigned char values
 * but a QByteArray stores signed char's. To recover the report as raw unsigned
 * characters you should use reinterpret_cast<uchar*>(bytearray.data()).
 *
 * \param id A quint32 device id.
 * \param timeout timeout in milliseconds or -1 for blocking wait.
 *
 * \return Returns the data in a QByteArray.
 */
QByteArray QHidApiPrivate::read(quint32 id, int timeout) {
    hid_device *device = findId(id);

    if (device != NULL) {
        unsigned char buf[65];

        int rep = hid_read_timeout(device, buf, 65, timeout);

        if (rep > 0) {
            QByteArray data(reinterpret_cast<char*>(buf), rep);
            return data;
        }
    }

    return QByteArray();
}

/*!
 * \brief Get a feature report from a HID device.
 *
 * Set the first byte of the returned data will contain the Report ID of the report to be read. Make sure to allow space for this extra byte in data[].
 * Upon return, the first byte will still contain the Report ID, and the report data will start in data[1].
 *
 * \param id A quint32 device id.
 * \param reportId the reportId.
 *
 * \return Returns the data in a QByteArray. Returns an empty QByteArray on error.

 */
QByteArray QHidApiPrivate::featureReport(quint32 id, uint reportId) {
    hid_device *device = findId(id);

    if (device != NULL) {
        unsigned char buf[65];
        buf[0] = reportId;

        int rep = hid_get_feature_report(device, buf, sizeof(buf));

        if (rep > 0) {
            QByteArray data(reinterpret_cast<char*>(buf), rep);
            return data;
        }
    }

    return QByteArray();
}

/*!
 * \brief  Write an Feature report to a HID device.
 *
 * HID reports have a maximum length of 64 bytes, plus an the initial char being a report ID. For devices which only support a single report, this must be set to 0x0.
 * The remaining bytes contain the report data.
 *
 * Data will send the data on the first OUT endpoint, if one exists. If it does not, it will send the data through the Control Endpoint (Endpoint 0).
 *
 * \param id A quint32 device id.
 * \param the report id
 * \param The data to send, excluding the report number as the first byte.
 * \return the number of bytes written, or -1 on error.
 */
int QHidApiPrivate::sendFeatureReport(quint32 id, quint8 reportId, QByteArray data) {
    if (data.length() > 64) return -1;

    hid_device *device = findId(id);

    if (device != NULL) {
        data.prepend(reportId);

        int rep = hid_send_feature_report(device, reinterpret_cast<uchar*>(data.data()), data.length());

        return rep;
    }

    return -1;
}

/*!
 * \brief  Write an Output report to a HID device.
 *
 * HID reports have a maximum length of 64 bytes, plus an the initial char being a report ID. For devices which only support a single report, this must be set to 0x0.
 * The remaining bytes contain the report data.
 *
 * Data will send the data on the first OUT endpoint, if one exists. If it does not, it will send the data through the Control Endpoint (Endpoint 0).
 *
 * \param id A quint32 device id.
 * \param the report id
 * \param The data to send, excluding the report number as the first byte.
 * \return the number of bytes written, or -1 on error.
 */
int QHidApiPrivate::write(quint32 id, QByteArray data, quint8 reportNumber) {
    if (data.length() > 64) return -1;

    hid_device *device = findId(id);

    if (device != NULL) {
        data.prepend(reportNumber);

        int rep = hid_write(device, reinterpret_cast<uchar*>(data.data()), data.length());

        return rep;
    }

    return -1;
}

/*!
 * \brief  Write an Output report to a HID device.
 *
 * HID reports have a maximum length of 255 bytes, plus an the initial char being a report ID. For devices which only support a single report, this must be set to 0x0.
 * The remaining bytes contain the report data. In this version of write() it is assumed that the initial report character is already prepended to the supplied QByteArray.
 *
 * Data will send the data on the first OUT endpoint, if one exists. If it does not, it will send the data through the Control Endpoint (Endpoint 0).
 *
 * \param id A quint32 device id.
 * \param The data to send, excluding the report number as the first byte.
 *
 * \return the number of bytes written, or -1 on error.
 */
int QHidApiPrivate::write(quint32 id, QByteArray data) {
    if (data.length() > 254) return -1;

    hid_device *device = findId(id);

    if (device != NULL) {
        int rep = hid_write(device, reinterpret_cast<uchar*>(data.data()), data.length());

        return rep;
    }

    return -1;
}

/*!
 * \brief Get a string describing the last error which occurred on the supplied device.
 *
 * \param id A quint32 device id.
 *
 * \return This function returns a string containing the last error which occurred on the device or an empty QString if none has occurred.
 */
QString QHidApiPrivate::error(quint32 id) {
    hid_device *device = findId(id);

    if (device != NULL) {
        QString r;

        const wchar_t *errorString = hid_error(device);
        r = QString::fromWCharArray(errorString);

        if (r == NULL) return QString();

        return r;
    }

    return QString();
}

/*!
 * \brief  Set the device handle to be blocking.
 *
 * In non-blocking mode calls to read() will return immediately with a value of 0 if there is no data to be read. In blocking mode, read() will wait (block)
 * until there is data to read before returning.
 *
 * Blocking can be turned on and off at any time.
 *
 * \param id  A quint32 device id.
 * \return Returns true on success and false on error.
 */
bool QHidApiPrivate::setBlocking(quint32 id) {
    hid_device *device = findId(id);

    int rep = hid_set_nonblocking(device, 1);
    return !!rep;
}

/*!
 * \brief  Set the device handle to be non-blocking.
 *
 * In non-blocking mode calls to read() will return immediately with a value of 0 if there is no data to be read. In blocking mode, read() will wait (block)
 * until there is data to read before returning.
 *
 * Nonblocking can be turned on and off at any time.
 *
 * \param id  A quint32 device id.
 * \return Returns true on success and false on error.
 */
bool QHidApiPrivate::setNonBlocking(quint32 id) {
    hid_device *device = findId(id);

    int rep = hid_set_nonblocking(device, 0);
    return !!rep;
}

/*!
 * \brief Open a HID device by its path name.
 *
 * The path name be determined by calling hid_enumerate(), or a platform-specific path
 * name can be used (eg: /dev/hidraw0 on Linux).
 *
 * \param path - the path to the device
 * \return a quint32 id for the device.
 */
quint32 QHidApiPrivate::open(QString path) {

    quint32 id = 0;

    // have we opened this path before.
    if (mPathMap.contains(path)) {
        id = mPathMap.value(path);
        return id;
    }

    // if not open it.
    hid_device *device = hid_open_path(path.toLocal8Bit().data());

    // sorry doesn't exist
    if (device == NULL) return 0;

    // have we already opened it.
    if (mDeviceIdMap.contains(device)) {
        id = mDeviceIdMap.value(device);
        return id;
    }

    // get the next available id
    id = nextId();

    // and save it away with the device
    mIdDeviceMap.insert(id, device);
    mDeviceIdMap.insert(device, id);
    mPathMap.insert(path, id);

    return id;
}

/*
 * Opens a new product for the supplied vendor/product/serial number.
 * returns the handle id if successful, otherwise  returns 0.
 */
quint32 QHidApiPrivate::openNewProduct(ushort vendorId, ushort productId, QString serialNumber) {
    hid_device *device = NULL;
    quint32 id = 0;
    if (serialNumber.isEmpty()) {
        device = hid_open(vendorId, productId, NULL);
        if (device != NULL) {
            id = nextId();
            QVariant v(id);
            QMultiMap<ushort, QVariant> pmap;
            pmap.insert(productId, v);
            mVendorMap.insert(vendorId, pmap);
            mIdDeviceMap.insert(id, device);
            mDeviceIdMap.insert(device, id);
        }
    } else {
        wchar_t* sn = new wchar_t[serialNumber.length() + 1];
        serialNumber.toWCharArray(sn);
        sn[serialNumber.length()] = 0x0;
        device = hid_open(0x4d8, 0xf1fa, NULL);
//        device = hid_open(vendorId, productId, sn/*, Q_NULLPTR*/);
        if (device != Q_NULLPTR) {
            id = nextId();
            QVariant v(serialNumber);
            QMultiMap<ushort, QVariant> pmap;
            pmap.insert(productId, v);
            mVendorMap.insert(vendorId, pmap);
            mSerDevices.insert(serialNumber, id);
            mIdDeviceMap.insert(id, device);
            mDeviceIdMap.insert(device, id);
        }
    }
    return id;
}

/*
 * Opens a product for the supplied vendor/product/serial number. First checks if we already have
 * this combination open, otherwise opens a new one.
 *
 * returns the handle id if successful, otherwise  returns 0.
 */
quint32 QHidApiPrivate::openProduct(ushort vendorId, ushort productId, QString serialNumber) {
    quint32 id = 0;

    /* First check if we already have this.
     * get list of all products for this vendor and
     * iterate through it to find it.
     */
    QList<QMultiMap<ushort, QVariant> > vmap = mVendorMap.values(vendorId);
    QListIterator<QMultiMap<ushort, QVariant> > vmapIt(vmap);
    while (vmapIt.hasNext()) {

        // iterate through them looking for all with this product id
        QMultiMap<ushort, QVariant> pmap = vmapIt.next();
        QMapIterator<ushort, QVariant> pmapIt(pmap);
        while (pmapIt.hasNext()) {
            pmapIt.next();

            ushort p = pmapIt.key();
            if (p == productId) {
                // either an int id or a QString
                QVariant v = pmapIt.value();
                QVariant::Type t = v.type();
                bool ok;

                if (t == QVariant::UInt) {

                    id = v.toUInt(&ok);
                    if (ok) {
                        if (mIdDeviceMap.contains(id))
                            return id;
                    }

                } else if (t == QVariant::String) {
                    // must have been a string
                    QString sn = v.toString();

                    id = mSerDevices.value(sn, 0);

                    return id;
                }
            }
        }
    }

    // Hah not found so open it if it exists.
    id = openNewProduct(vendorId, productId, serialNumber);

    return id;
}

quint32 QHidApiPrivate::nextId() {
    return mNextId++;
}
