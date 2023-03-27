#ifndef MIMEFILE_H
#define MIMEFILE_H

#include "mimepart.h"
#include <QFile>

#include "smtpexports.h"

class SMTP_EXPORT MimeFile : public MimePart
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    MimeFile(const QByteArray& stream, const QString& fileName);
    MimeFile(QFile *f);
    ~MimeFile();

    /* [1] --- */


    /* [2] Getters and Setters */

    /* [2] --- */

protected:

    /* [3] Protected members */

    QFile* file;

    /* [3] --- */


    /* [4] Protected methods */

    virtual void prepare();

    /* [4] --- */

};

#endif // MIMEFILE_H
