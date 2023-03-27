#ifndef MIMEATTACHMENT_H
#define MIMEATTACHMENT_H

#include <QFile>
#include "mimepart.h"
#include "mimefile.h"

#include "smtpexports.h"

class SMTP_EXPORT MimeAttachment : public MimeFile
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    MimeAttachment(QFile* file);
    MimeAttachment(const QByteArray& stream, const QString& fileName);

    ~MimeAttachment();

    /* [1] --- */

protected:

    /* [2] Protected methods */

    virtual void prepare();

    /* [2] --- */
};

#endif // MIMEATTACHMENT_H
