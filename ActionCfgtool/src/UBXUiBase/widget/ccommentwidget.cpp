/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CCommentWidget.cpp
* 创建时间：2016/09/24
* 文件标识：
* 文件摘要：动作评论控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/09/24
* 版本摘要：
*/

#include <QTextBlock>

#include "ccommentwidget.h"
#include "ui_ccommentwidget.h"
#include "ubxwidgetconst.h"

CCommentWidget::CCommentWidget(QWidget *parent) :
    QWidget(parent),
    m_nMaxCommentCharNumber(UBXUiBaseConstants::COMMENT_MAX_CHARACTOR_NUMBER),
    ui(new Ui::CCommentWidget),
    m_nCommendId(0)
{
    ui->setupUi(this);

    ui->textEditComment->setPlaceholderText(tr("Please input the comment"));

    connect(ui->pushButtonComment, &QPushButton::clicked, this, &CCommentWidget::slotOnCommentClicked);
    connect(ui->textEditComment, &QTextEdit::textChanged, this, &CCommentWidget::slotOnCommentTextChanged);

    resetUI();
}

CCommentWidget::~CCommentWidget()
{
    delete ui;
}

void CCommentWidget::setMaxCommentCharNumber(int nNumber)
{
    if (nNumber <= 0)
    {
        return;
    }

    m_nMaxCommentCharNumber = nNumber;
}

/************************************************************************************
* 名称: setReplyToWho
* 功能: 添加被回复的对象到评论头
* 参数：@[in] nCommentId: 被回复的评论id
* 参数：@[in] strWho: 被回复的对象
* 返回:   void
* 时间:   2016/09/26
* 作者:   hwx
*/
void CCommentWidget::setReplyToWho(quint64 nCommentId, const QString &strWho)
{
    if (strWho.isEmpty())
    {
        return;
    }

    resetUI();

    m_nCommendId = nCommentId;
    m_strReplyHeader = QString("@%1:").arg(strWho);

    QFont previousFont;
    previousFont = ui->textEditComment->font(); // 记录历史信息

    QFont fontReplyToWho;
    fontReplyToWho.setPointSize(UBXUiBaseConstants::FONT_POINT_SIZE_10);
    ui->textEditComment->setFont(fontReplyToWho);
    ui->textEditComment->setTextColor(QColor(UBXUiBaseConstants::UBX_COLOR_DARK_GREY)); // #858585
    ui->textEditComment->insertPlainText(m_strReplyHeader);

    ui->textEditComment->setFont(previousFont); // 恢复原来的字体格式

    ui->textEditComment->setFocus();
}

/************************************************************************************
* 名称: resetUI
* 功能: 复位界面，清除评论
* 参数：
* 返回:   void
* 时间:   2016/09/26
* 作者:   hwx
*/
void CCommentWidget::resetUI()
{
    m_strReplyHeader = "";
    m_nCommendId = 0;
    ui->textEditComment->clear();
    ui->textEditComment->setFocus();
    setCommentButtonState(false);

    ui->labelInputNumber->setText(QString("%1/%2").arg(0).arg(m_nMaxCommentCharNumber));
}

/************************************************************************************
* 名称: getRealComment
* 功能: 获取真正的评论内容，去掉回复的对象
* 参数：[out] 剔除了回复对象的评论，回复对象的格式为@xx:
* 返回:   评论id，如果回复的是楼主，返回0
* 时间:   2016/09/26
* 作者:   hwx
*/
int CCommentWidget::getRealComment(QString &strRealComment)
{
     int nReplyId = 0;
    QString strCommentWithReplyToWho = ui->textEditComment->toPlainText().trimmed();
    if (strCommentWithReplyToWho.isEmpty())
    {
        return nReplyId;
    }

    QString strReplyHeader = m_strReplyHeader.trimmed(); // 评论头部，如@xx
    strRealComment = strCommentWithReplyToWho;
    if (strCommentWithReplyToWho.startsWith(strReplyHeader))
    {
        strRealComment = strCommentWithReplyToWho.right(strCommentWithReplyToWho.size() - strReplyHeader.size());
        nReplyId = m_nCommendId;
    }

    return nReplyId;
}

void CCommentWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        slotOnCommentClicked();
    }

    QWidget::keyPressEvent(event);
}

/************************************************************************************
* 名称: setCommentButtonState
* 功能: 设置评论按钮状态，点击评论后处于非使能状态，防止用户多次点击，
*          评论完成后恢复使能状态
* 参数：
* 返回:   void
* 时间:   2016/09/26
* 作者:   hwx
*/
void CCommentWidget::setCommentButtonState(bool bEnable)
{
    ui->pushButtonComment->setEnabled(bEnable);
}

/************************************************************************************
* 名称: hasValidComment
* 功能: 判断评论框中是否存在有效评论
* 参数：
* 返回:   是否存在有效的评论
* 时间:   2016/09/28
* 作者:   hwx
*/
bool CCommentWidget::hasValidComment()
{
    QString strRealComment  = "";
    int nRepliedCommentId = getRealComment(strRealComment);
    if (strRealComment.isEmpty())
    {
        return false;
    }

    return true;
}

/************************************************************************************
* 名称: slotOnCommentTextChanged
* 功能: 评论内容发生变化的槽函数
* 参数：
* 返回:   void
* 时间:   2016/09/24
* 作者:   hwx
*/
void CCommentWidget::slotOnCommentTextChanged()
{
    // 如果没有评论内容，则评论按钮不可用
    if (!hasValidComment())
    {
        setCommentButtonState(false);
    }
    else
    {
         setCommentButtonState(true);
    }

    QString strRealComment = "";
    int nRepliedCommentId = getRealComment(strRealComment);

    int nTextLength = strRealComment.count();
    if(nTextLength > m_nMaxCommentCharNumber)
    {
        int nPosition = ui->textEditComment->textCursor().position();
        QTextCursor textCursor = ui->textEditComment->textCursor();
        strRealComment.remove(nPosition - (nTextLength - m_nMaxCommentCharNumber), nTextLength - m_nMaxCommentCharNumber);
        ui->textEditComment->setText(m_strReplyHeader + strRealComment);
        textCursor.setPosition(nPosition - (nTextLength - m_nMaxCommentCharNumber));
        ui->textEditComment->setTextCursor(textCursor);
    }

    if (nTextLength > m_nMaxCommentCharNumber)
    {
        nTextLength = m_nMaxCommentCharNumber;
    }

    ui->labelInputNumber->setText(QString("%1/%2").arg(nTextLength).arg(m_nMaxCommentCharNumber));
}

/************************************************************************************
* 名称: slotOnCommentClicked
* 功能: 点击评论按钮的槽函数
* 参数：
* 返回:   void
* 时间:   2016/09/24
* 作者:   hwx
*/
void CCommentWidget::slotOnCommentClicked()
{
    if (!hasValidComment())
    {
        return;
    }

    QString strRealComment  = "";
    int nRepliedCommentId = getRealComment(strRealComment);
    if (strRealComment.isEmpty())
    {
        return;
    }

    setCommentButtonState(false);

    emit sigOnComment(nRepliedCommentId, strRealComment);
}
