#ifndef EDITSONGDLG_H
#define EDITSONGDLG_H

#include "ccustomdialog.h"
#include "cpubfun.h"

QT_BEGIN_NAMESPACE
class CTitleWidget;
QT_END_NAMESPACE

namespace Ui {
class CEditSongDlg;
}

/**
 * @brief 歌曲编辑
 */
class CEditSongDlg : public XDialog
{
    Q_OBJECT

public:
    explicit CEditSongDlg(const QString &playListPath, QWidget *parent = 0);
    ~CEditSongDlg();

public:
    void init ();
    void initVar ();
    void initSongTableWidget ();
    void createTitleWidget ();
    void initWindows ();
    void initAction ();
    void layout();

protected:
    void refreshSongTableWidget ();
    void addRow (int nType, const QString& strFileName, tagMusicInfo* p);
    int  getSelRows (QList<int> &rows); /**< 获取选择的行 */
    void refreshSyncState ();           /**< 刷新同步按钮等待状态 */
    void selectRows (int row, bool bSel);  /**< 选择行 */

public slots:
    void slotUpdate();                  /**< 更新 */
    void slotAdd();                     /**< 添加 */
    void slotEdit();                    /**< 编辑 */
    void slotSave ();                   /**< 保存 */
    void slotBatchDownload ();          /**< 批量下载文件 */
    void slotBatchSync ();              /**< 批量同步至机器人 */
    void slotBatchOpenDir ();           /**< 打开文件夹 */
    void slotBatchDelete ();            /**< 批量删除文件夹 */
    void slotSync ();                   /**< 按钮触发同步 */
    void startSync ();                  /**< 执行同步操作 */
    void slotTableCustomContextMenuRequested(QPoint pos);
    void slotRefreshMusicInfo (int nMusicType, const QString& musicName, tagMusicInfo* pMusic);
    void removeRows(QList<int> rows);   /**< 删除行 */

private slots:
    void updateNumberLabel ();
    bool updateDance();                 /**< 更新歌单 */

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::CEditSongDlg*      ui;
    CTitleWidget*          m_pTitleWidget;
    QTypeMusicInfoMap      m_mapSongInfo;
    QList<int>             m_listUsbSync;       /**< 待同步列表 */
    QString                m_playListPath;
};

#include <QPainter>
#include <QHeaderView>
#include <QStyleOptionButton>
#include <QStyle>
class SCheckBoxHeaderView : public QHeaderView
{
    Q_OBJECT
private:
    bool isChecked;
    int m_checkColIdx;
public:
    SCheckBoxHeaderView( int checkColumnIndex, Qt::Orientation orientation, QWidget * parent = 0) : QHeaderView(orientation, parent) {
        m_checkColIdx = checkColumnIndex;
        isChecked = false;
    }

public:
    void setChecked (bool bCheck) {
        isChecked = bCheck;
    }

signals:
    void checkStausChange(bool);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const {
        painter->save();
        QHeaderView::paintSection(painter, rect, logicalIndex);
        painter->restore();
        if (logicalIndex == m_checkColIdx) {
            QStyleOptionButton option;
            int width = 10;
            for (int i=0; i<logicalIndex; ++i)
                width += sectionSize( i );
            option.rect = QRect(3, 5, 21, 21);
            if (isChecked) {
                option.state = QStyle::State_On;
            } else {
                option.state = QStyle::State_Off;
            }
            this->style()->drawControl(QStyle::CE_CheckBox, &option, painter);
        }
    }
    void mousePressEvent(QMouseEvent *event) {
        if (visualIndexAt(event->pos().x()) == m_checkColIdx) {
            isChecked = !isChecked;
            this->updateSection(m_checkColIdx);
            emit checkStausChange(isChecked);
        }
        QHeaderView::mousePressEvent(event);
    }
};

#endif // EDITSONGDLG_H
