﻿#include "quotedprintable.h"

QString QuotedPrintable::encode(const QByteArray &input)
{
    QString output;

    char byte;
    const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (int i = 0; i < input.length() ; ++i)
    {
        byte = input[i];

        if ((byte == 0x20) || ((byte >= 33) && (byte <= 126) && (byte != 61)))
        {
            output.append(byte);
        }
        else
        {
            output.append('=');
            output.append(hex[((byte >> 4) & 0x0F)]);
            output.append(hex[(byte & 0x0F)]);
        }
    }

    return output;
}


QByteArray QuotedPrintable::decode(const QString &input)
{
    //                    0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?  @  A   B   C   D   E   F
    const int hexVal[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};

    QByteArray output;

    for (int i = 0; i < input.length(); ++i)
    {
        if (input.at(i).toLatin1() == '=')
        {
            output.append((hexVal[input.at(i + 1).toLatin1() - '0'] << 4) + hexVal[input.at(i + 2).toLatin1() - '0']);
            i += 2;
        }
        else
        {
            output.append(input.at(i).toLatin1());
        }
    }

    return output;
}
