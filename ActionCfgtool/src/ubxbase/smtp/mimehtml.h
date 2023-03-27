#ifndef MIMEHTML_H
#define MIMEHTML_H

#include "mimetext.h"

#include "smtpexports.h"

class SMTP_EXPORT MimeHtml : public MimeText
{
    Q_OBJECT
public:

    /* [1] Constructors and Destructors */

    MimeHtml(const QString &html = "");
    ~MimeHtml();

    /* [1] --- */


    /* [2] Getters and Setters */

    void setHtml(const QString & html);

    const QString& getHtml() const;

    /* [2] --- */

protected:

    /* [3] Protected members */

    /* [3] --- */


    /* [4] Protected methods */

    virtual void prepare();

    /* [4] --- */
};

#endif // MIMEHTML_H
