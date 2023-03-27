#ifndef MIMEMESSAGE_H
#define MIMEMESSAGE_H

#include "mimepart.h"
#include "mimemultipart.h"
#include "emailaddress.h"
#include <QList>

#include "smtpexports.h"

class SMTP_EXPORT MimeMessage : public QObject
{
public:

    enum RecipientType {
        To,                 // primary
        Cc,                 // carbon copy
        Bcc                 // blind carbon copy
    };

    /* [1] Constructors and Destructors */

    MimeMessage(bool createAutoMimeConent = true);
    ~MimeMessage();

    /* [1] --- */


    /* [2] Getters and Setters */

    void setSender(EmailAddress* e);
    void addRecipient(EmailAddress* rcpt, RecipientType type = To);
    void addTo(EmailAddress* rcpt);
    void addCc(EmailAddress* rcpt);
    void addBcc(EmailAddress* rcpt);
    void setSubject(const QString & subject);
    void addPart(MimePart* part);

    void setInReplyTo(const QString& inReplyTo);

    void setHeaderEncoding(MimePart::Encoding);

    const EmailAddress & getSender() const;
    const QList<EmailAddress*> & getRecipients(RecipientType type = To) const;
    const QString & getSubject() const;
    const QList<MimePart*> & getParts() const;

    MimePart& getContent();
    void setContent(MimePart *content);
    /* [2] --- */


    /* [3] Public methods */

    virtual QString toString();

    /* [3] --- */

protected:

    /* [4] Protected members */

    EmailAddress* sender;
    QList<EmailAddress*> recipientsTo, recipientsCc, recipientsBcc;
    QString subject;
    QString mInReplyTo;
    MimePart *content;
    bool autoMimeContentCreated;
    
    MimePart::Encoding hEncoding;

    /* [4] --- */


};

#endif // MIMEMESSAGE_H
