#include "mimefile.h"
#include <QFileInfo>


MimeFile::MimeFile(QFile *file)
{
    this->file = file;
    this->cType = "application/octet-stream";
    this->cName = QFileInfo(*file).fileName();
    this->cEncoding = Base64;
}

MimeFile::MimeFile(const QByteArray& stream, const QString& fileName)
{
    this->cEncoding = Base64;
    this->cType = "application/octet-stream";
    this->file = 0;
    this->cName = fileName;
    this->content = stream;
}

MimeFile::~MimeFile()
{
  if (file)
    delete file;
}

void MimeFile::prepare()
{
  if (this->file)
  {
    file->open(QIODevice::ReadOnly);
    this->content = file->readAll();
    file->close();
  }
    MimePart::prepare();
}
