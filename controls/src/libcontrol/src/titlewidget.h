
#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H
#include <QWidget>
#include <QPixmap>
#include "libcontrol_global.h"
class QPushButton;
class QLabel;
/**
 * @brief 自定义标题栏控件类
 *
 */
class LIBCONTROLSHARED_EXPORT CTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CTitleWidget(QWidget *parent);
    ~CTitleWidget();
    void setLogo(QPixmap &pixmap);
    void setVerion(QString strTitle);
    void setTitle(QPixmap bgPix);
public:
    void setMaxBtnVisible (bool bVisible);
    void setMinBtnVisible (bool bVisible);
    void setCloseBtnVisible (bool bVisible);
    void setMenuBtnVisible (bool bVisible);
    void setLogoVisible (bool bVisible);
    void setTitleLabelVisible(bool bVisible);

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent * event );
    void paintEvent(QPaintEvent *);

private:
    void initVars();
    void createWidget();
    void initWidget();
    void initAction();
    void translateLanguage();

signals:
    void showSkin();
    void showMax();
    void showMainMenu();

public slots:
    void slotClose();
    void slotShowMin();
    void slotShowMax();

protected:
    void showMaximized ();
    void showNormal ();

private:
    QPixmap             m_logoPixmap;
    QWidget             *m_pParentWidget;
    QPoint              m_pointDragPosition;    /**< 拖动的当前坐标位置 */
    bool                m_bIsMaximized;         /**< 是否已经最大化 */
    bool                m_bPressed;             /**< 鼠标左键是否按下 */
    QLabel              *plblLogo;              /**< 显示logo */
    QLabel              *plblVersionTitle;      /**< 标题 */
    QPushButton         *pbtnChangeSkin;        /**< 换肤 */
    QPushButton         *pbtnMainMenu;          /**< 主菜单 */
    QPushButton         *pbtnMin;               /**< 最小化 */
    QPushButton         *pbtnMax;               /**< 最大化 */
    QPushButton         *pbtnClose;             /**< 关闭 */
    QRect               m_orginRect;
};

#endif // TITLEWIDGET_H
