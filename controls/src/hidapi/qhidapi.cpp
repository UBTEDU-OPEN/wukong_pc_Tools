#include "qhidapi.h"
#include "qhidapi_p.h"

/*
Copyright (C) 2013 - 2016 by Simon Meaden <[simonmeaden@virginmedia.com]>

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*!
 * \class QHidApi
 * \brief \c QHidApi is a Qt wrapper class for the hidapi library from Signal 11. <http://www.signal11.us/oss/hidapi/>
 *
 * This wrapper class allows the use of QString, quint32 etc rather than the C style wchar_t* and unsigned int.
 *
 * The class maintains a list of one or more hid devices, any of which can be accessed using the id returned when
 * the device is opened. If you have lost the device id, the reopen the device using the original vendow/product id's
 * or the device path and QHidApi will return the original id.
 *
 * You can use enumerate to generate a list of available devices. The vendor and product id's can then be used to \c open() the devices.
 *
 * HIDAPI is a multi-platform library which allows an application to interface with USB and Bluetooth HID-Class devices on Windows,
 * Linux, and Mac OS X. While it can be used to communicate with standard HID devices like keyboards, mice, and Joysticks, it is
 * most useful when used with custom (Vendor-Defined) HID devices. Many devices do this in order to not require a custom driver
 * to be written for each platform. HIDAPI is easy to integrate with the client application, just requiring a single source file
 * to be dropped into the application. On Windows, HIDAPI can optionally be built into a DLL.
 *
 * Programs which use HIDAPI are driverless, meaning they do not require the use of a custom driver for each device on each platform.
 */


/*!  Default constructor of QHidApi class.
 *
 * \param parent the parent object.
 */
QHidApi::QHidApi(QObject *parent) :
    QObject(parent),
    d_ptr(new QHidApiPrivate(0x0, 0x0, this)) {
}

/*!
 * \brief Constructor of QHidApi class with a specified vendor.
 *
 *
 * \param vendorId the specified vendor id.
 * \param parent the parent object.
 */
QHidApi::QHidApi(ushort vendorId, QObject *parent) :
    QObject(parent),
    d_ptr(new QHidApiPrivate(vendorId, 0x0, this)) {

}

/*!
 * \brief Constructor of QHidApi class with a specified vendor. and product.
 *
 * \param vendorId the specified vendor id.
 * \param product the specified product id.
 * \param parent the parent object.
 */
QHidApi::QHidApi(ushort vendorId, ushort productId, QObject *parent) :
    QObject(parent),
    d_ptr(new QHidApiPrivate(vendorId, productId, this)) {
}

QHidApi::~QHidApi() {
}

/*!
 * \brief Get The Manufacturer String from a HID device.
 *
 * \param id A quint32 device id.
 *
 * \return a QString containing the manufacturers name string, otherwise an empty QString.
 */
QString QHidApi::manufacturerString(quint32 deviceId) {
    return d_ptr->manufacturerString(deviceId);
}

/*!
 * \brief Get The Product String from a HID device.
 *
 * \param id A quint32 device id.
 *
 * \return a QString containing the product name string, otherwise an empty QString.
 */
QString QHidApi::productString(quint32 deviceId) {
    return d_ptr->productString(deviceId);
}

/*!
 * \brief Get The Serial Number String from a HID device.
 *
 * \param id A quint32 device id.
 *
 * \return a QString containing the Serial number string, otherwise an empty QString.
 */
QString QHidApi::serialNumberString(quint32 deviceId) {
    return d_ptr->serialNumberString(deviceId);
}

/*!
 * \brief Get a string from a HID device, based on its string index.
 *
 * \param id A quint32 device id.
 * \param index The int index of the string to get.
 *
 * \return a QString containing the indexed string, otherwise an empty QString.
 */
QString QHidApi::indexedString(quint32 deviceId, int index) {
    return d_ptr->indexedString(deviceId, index);
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
QList<QHidDeviceInfo> QHidApi::enumerate(ushort vendorId, ushort productId) {
    return d_ptr->enumerate(vendorId, productId);
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
quint32 QHidApi::open(ushort vendorId, ushort productId, QString serialNumber) {
    return d_ptr->open(vendorId, productId, serialNumber);
}

/*!
 * \brief Closes the specified device if it exists, otherwise this command is ignored.
 *
 * \param id - the quint32 id for the device.
 */
void QHidApi::close(quint32 deviceId) {
    return d_ptr->close(deviceId);
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
QByteArray QHidApi::read(quint32 deviceId) {
    return d_ptr->read(deviceId);
}

/*!
 * \brief  Read an Input report from a HID device into a QByteArray,  with timeout.
 *
 * Input reports are returned to the host through the INTERRUPT IN endpoint. The first byte will contain the Report number if the device uses numbered reports.
 * It should be noted that the write function returns data as unsigned char values but a QByteArray stores signed char's. To recover the report as raw unsigned
 * characters you should use reinterpret_cast<uchar*>(bytearray.data()).
 *
 * \param id A quint32 device id.
 * \param timeout timeout in milliseconds or -1 for blocking wait.
 *
 * \return Returns the data in a QByteArray. If no packet was availableto be read within timeout milliseconds and the device is in non-blocking mode,
 */
QByteArray QHidApi::read(quint32 deviceId, int timeout) {
    return d_ptr->read(deviceId, timeout);
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
QByteArray QHidApi::featureReport(quint32 deviceId, uint reportId) {
    return d_ptr->featureReport(deviceId, reportId);
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
int QHidApi::sendFeatureReport(quint32 deviceId, quint8 reportId, QByteArray data) {
    return d_ptr->sendFeatureReport(deviceId, reportId, data);
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
int QHidApi::write(quint32 deviceId, QByteArray data, quint8 reportId) {
    return d_ptr->write(deviceId, data, reportId);
}

/*!
 * \brief  Write an Output report to a HID device.
 *
 * HID reports have a maximum length of 64 bytes, plus an the initial char being a report ID. For devices which only support a single report, this must be set to 0x0.
 * The remaining bytes contain the report data. In this version of write() it is assumed that the initial report character is already prepended to the supplied QByteArray.
 *
 * Data will send the data on the first OUT endpoint, if one exists. If it does not, it will send the data through the Control Endpoint (Endpoint 0).
 *
 * \param id A quint32 device id.
 * \param The data to send, excluding the report number as the first byte.
 *
 * \return the number of bytes written, or -1 on error.
 */
int QHidApi::write(quint32 deviceId, QByteArray data) {
    return d_ptr->write(deviceId, data);
}

/*!
 * \brief Get a string describing the last error which occurred on the supplied device.
 *
 * \param id A quint32 device id.
 *
 * \return This function returns a string containing the last error which occurred on the device or an empty QString if none has occurred.
 */
QString QHidApi::error(quint32 deviceId) {
    return d_ptr->error(deviceId);
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
bool QHidApi::setBlocking(quint32 deviceid) {
    return d_ptr->setBlocking(deviceid);
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
bool QHidApi::setNonBlocking(quint32 deviceId) {
    return d_ptr->setNonBlocking(deviceId);
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
quint32 QHidApi::open(QString path) {
    return d_ptr->open(path);
}

