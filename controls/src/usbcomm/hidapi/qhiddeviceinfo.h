#ifndef QHIDDEVICEINFO_H
#define QHIDDEVICEINFO_H
/*
Copyright (C) [year] by Simon Meaden <[simonmeaden@virginmedia.com]>

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <QString>

struct QHidDeviceInfo {
    /** Platform-specific device path */
    QString path;
    /** Device Vendor ID */
    ushort vendorId;
    /** Device Product ID */
    ushort productId;
    /** Serial Number */
    QString serialNumber;
    /** Device Release Number in binary-coded decimal,
            also known as Device Version Number */
    ushort releaseNumber;
    /** Manufacturer String */
    QString manufacturerString;
    /** Product string */
    QString productString;
#if defined(Q_OS_WIN32) || defined(Q_OS_MAC)
    /** Usage Page for this Device/Interface
            (Windows/Mac only). */
    ushort usagePage;
    /** Usage for this Device/Interface
            (Windows/Mac only).*/
    ushort usage;
#endif
    /** The USB interface which this logical device
            represents. Valid on both Linux implementations
            in all cases, and valid on the Windows implementation
            only if the device contains more than one interface. */
    int interfaceNumber;
};

#endif // QHIDDEVICEINFO_H
