#ifndef ADDPLAYDLG_H
#define ADDPLAYDLG_H

#include <ccustomdialog.h>
#include <QMap>
#include "pubdef.h"

namespace Ui {
class CAddPlayDlg;
}

class CTitleWidget;
class CAddPlayDlg : public XDialog
{
    Q_OBJECT

public:
    explicit CAddPlayDlg(QWidget *parent = 0);
    ~CAddPlayDlg();

public:
    void init ();
    void initVar ();
    void createTitleWidget ();
    void initAction ();
    void layout();

public:
    void setTitleText (const QString& strTitle);
    bool isUniqueFileName (const QString& strFileName);

public slots:
    void slotConfirm();
    void slotClose ();

public:
    void setFileName (const QString& strFileName);
    QString getFileName () const { return m_strFileName; }

    void setDisplayName ( const QString& strDisplayName);
    QString getDisplayName () const { return m_strDisplayName; }

    void setMusicTime ( int nTime );
    int getMusicTime () const { return m_nTime; }

    void setMusicTimeVisiable(bool bVisiable);
    void setSongs (QTypeMusicInfoMap mapSong) { m_mapSong = mapSong; }
    void setDlgType (int nType) { m_nDlgType = nType; }

public:
    enum TYPE_DLG{
        TYPE_EDIT_DLG = 1,
        TYPE_ADD_DLG = 2
    };

protected:
    QTypeMusicInfoMap   m_mapSong;
    QString             m_strFileName;
    QString             m_strDisplayName;
    int                 m_nTime;
    int                 m_nDlgType;

protected:
    CTitleWidget*         m_pTitleWidget;

private:
    Ui::CAddPlayDlg *ui;
};

#endif // ADDPLAYDLG_H
