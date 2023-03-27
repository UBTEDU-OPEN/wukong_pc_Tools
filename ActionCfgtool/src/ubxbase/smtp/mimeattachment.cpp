#include "mimeattachment.h"
#include <QFileInfo>


MimeAttachment::MimeAttachment(QFile *file)
    : MimeFile(file)
{
}
MimeAttachment::MimeAttachment(const QByteArray& stream, const QString& fileName): MimeFile(stream, fileName)
{

}

MimeAttachment::~MimeAttachment()
{
}

void MimeAttachment::prepare()
{
    this->header += "Content-disposition: attachment\r\n";
    MimeFile::prepare();
}
