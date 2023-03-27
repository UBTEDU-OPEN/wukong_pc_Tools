#ifndef MIMEINLINEFILE_H
#define MIMEINLINEFILE_H

#include "mimefile.h"

#include "smtpexports.h"

class SMTP_EXPORT MimeInlineFile : public MimeFile
{
public:

    /* [1] Constructors and Destructors */

    MimeInlineFile(QFile *f);
    ~MimeInlineFile();

    /* [1] --- */


    /* [2] Getters and Setters */

    /* [2] --- */

protected:

    /* [3] Protected members */

    /* [3] --- */


    /* [4] Protected methods */

    virtual void prepare();

    /* [4] --- */
};

#endif // MIMEINLINEFILE_H
