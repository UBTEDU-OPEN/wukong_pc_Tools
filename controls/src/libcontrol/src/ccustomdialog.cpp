#include "ccustomdialog.h"
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QTableWidget>
#include <QHeaderView>

XDialog::XDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(QMargins(SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH));

    m_bPressed      = false;
    m_isMaxWindow   = false;
    m_canMove       = false;
}

XDialog::~XDialog()
{

}

void XDialog::drawShadow(QPainter &painter)
{
    //绘制左上角、左下角、右上角、右下角、上、下、左、右边框
    QList<QPixmap> pixmaps;
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Left.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Right.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Top.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Bottom.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Left_Top.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Right_Top.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Left_Bottom.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Right_Bottom.png"));

    painter.drawPixmap(0, 0, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[4]);
    painter.drawPixmap(this->width()-SHADOW_WIDTH, 0, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[5]);
    painter.drawPixmap(0,this->height()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[6]);
    painter.drawPixmap(this->width()-SHADOW_WIDTH, this->height()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[7]);
    painter.drawPixmap(0, SHADOW_WIDTH, SHADOW_WIDTH, this->height()-2*SHADOW_WIDTH, pixmaps[0].scaled(SHADOW_WIDTH, this->height()-2*SHADOW_WIDTH));
    painter.drawPixmap(this->width()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, this->height()-2*SHADOW_WIDTH, pixmaps[1].scaled(SHADOW_WIDTH, this->height()- 2*SHADOW_WIDTH));
    painter.drawPixmap(SHADOW_WIDTH, 0, this->width()-2*SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[2].scaled(this->width()-2*SHADOW_WIDTH, SHADOW_WIDTH));
    painter.drawPixmap(SHADOW_WIDTH, this->height()-SHADOW_WIDTH, this->width()-2*SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[3].scaled(this->width()-2*SHADOW_WIDTH, SHADOW_WIDTH));
}

void XDialog::setStyle(const QString& strStylePath)
{
    QFile file(strStylePath);
    if ( file.open(QFile::ReadOnly) ) {
        QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
        file.close();
    }
}

void XDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    this->drawShadow(painter);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width()-2*SHADOW_WIDTH, this->height()-2*SHADOW_WIDTH));
}

void XDialog::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        m_bPressed = true;
        m_ptPress = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void XDialog::mouseMoveEvent(QMouseEvent * event)
{
    if ( m_bPressed && !m_isMaxWindow && m_canMove ) {
        this->move(event->globalPos() - m_ptPress);//移动窗口
        event->accept();
    }
}

void XDialog::mouseReleaseEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    m_bPressed = false;
    m_ptPress = QPoint(0,0);
}

void XDialog::mouseDoubleClickEvent(QMouseEvent * event)
{
    if ( event->buttons() == Qt::LeftButton ){
        m_isMaxWindow = !m_isMaxWindow;
        emit signal_show_max_min_state (m_isMaxWindow);
    }
}

void XDialog::setTableStyle (QTableWidget *pTableWidget, int nRowHeight/* = 35*/, int nHeaderHeight/* = 40*/, bool canEdit/* = false*/)
{
    // 鼠标点击的选项不会出现虚框
    pTableWidget->setItemDelegate(new NoFocusDelegate());
    // 设置表格为选择整行
    pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置水平表头显示，竖向表头隐藏
    pTableWidget->verticalHeader()->setVisible(false);
    pTableWidget->horizontalHeader()->setVisible(true);
    pTableWidget->horizontalHeader()->setStretchLastSection(true);// 设置充满表宽度
    pTableWidget->horizontalHeader()->setFixedHeight(nHeaderHeight); //设置表头的高度
    pTableWidget->verticalHeader()->setDefaultSectionSize(nRowHeight);  //设置行高
    pTableWidget->horizontalHeader()->setSectionsClickable(false);  // 设置表头不可点击（默认点击后进行排序）
    pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);  //设置为可以选中多个目标
    if ( !canEdit ){
        pTableWidget->setEditTriggers (QAbstractItemView::NoEditTriggers);   // 设置整个表格不可以编辑
    }
    pTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    // 设置表格和表格头的qss
    pTableWidget->setStyleSheet(QString("QTableWidget {"
                                        "border:none;"
                                        "border:1px solid #828790;"
                                        "selection-background-color: rgb(144, 144, 214);"
                                        "font-size: 14px;"
                                        "}"));

    pTableWidget->horizontalHeader()->setStyleSheet(QString("QHeaderView {"
                                                            "background-color: #f9f9f9;"
                                                            "border-bottom:1px solid #d7d7d7;"
                                                            "font-size: 18px;"
                                                            "color: #797979;"
                                                            "}"
                                                            "QHeaderView::section {"
                                                            "background-color: #f0f5f8;"
                                                            "padding-left: 4px;"
                                                            "border: none;"
                                                            "border-right: 1px solid #b8cbd2;"
                                                            "border-bottom: 1px solid #b8cbd2;"
                                                            "padding: 10px;"
                                                            "}"));
}

void XDialog::clearContentsTable (QTableWidget* pTableWidget)
{
    pTableWidget->clearContents();
    pTableWidget->setRowCount (0);
}
