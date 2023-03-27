#ifndef CCUSTOMDIALOG_H
#define CCUSTOMDIALOG_H

#include <QDialog>
#include "libcontrol_global.h"

#include <QStyledItemDelegate>
class NoFocusDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter* painter,
               const QStyleOptionViewItem & option,
               const QModelIndex &index) const
    {
        QStyleOptionViewItem itemOption(option);
        if (itemOption.state & QStyle::State_HasFocus)
        {
            itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
        }
        QStyledItemDelegate::paint(painter, itemOption, index);
    }
};

QT_BEGIN_NAMESPACE
class QTableWidget;
QT_END_NAMESPACE

class LIBCONTROLSHARED_EXPORT XDialog : public QDialog
{
    Q_OBJECT
public:
    XDialog(QWidget *parent = 0);
    virtual ~XDialog();

signals:
    void    signal_show_max_min_state (bool isMaxWindow);       /**< 最大化与最小化切换时触发该信号 */

public:
    void    setStyle(const QString& strStylePath);
    void    setTableStyle (QTableWidget *pTableWidget, int nRowHeight = 35, int nHeaderHeight = 40, bool canEdit = false);
    void    clearContentsTable (QTableWidget *pTableWidget);
    void    setMoveWindowEnable (bool canMove) { m_canMove = canMove; }
    bool    canMoveWindow () { return m_canMove; }

protected:
    void    drawShadow(QPainter &painter);

protected:
    virtual void    paintEvent(QPaintEvent *event);
    virtual void    mousePressEvent(QMouseEvent * event);
    virtual void    mouseMoveEvent(QMouseEvent * event);
    virtual void    mouseReleaseEvent(QMouseEvent * event);
    virtual void    mouseDoubleClickEvent(QMouseEvent * event);

protected:
    bool                m_isMaxWindow;          /**< 是否已经最大化 */
    bool                m_bPressed;             /**< 鼠标是否按下 */
    QPoint              m_ptPress;              /**< 鼠标按下去的点 */
    bool                m_canMove;              /**< 是否支持移动窗口 */
};

#endif // CCUSTOMDIALOG_H
