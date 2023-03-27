/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CCommentWidget.h
* 创建时间：2016/09/24
* 文件标识：
* 文件摘要：动作评论控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/09/24
* 版本摘要：
*/

#ifndef CCOMMENTWIDGET_H
#define CCOMMENTWIDGET_H
#include "ubxuibase_global.h"
#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class CCommentWidget;
}

class UBXUIBASE_EXPORT CCommentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CCommentWidget(QWidget *parent = 0);
    ~CCommentWidget();

    // 设置支持评论最大字符数目
    void setMaxCommentCharNumber(int nNumber);

    /************************************************************************************
    * 名称: setCommentButtonState
    * 功能: 设置评论按钮状态，点击评论后处于非使能状态，防止用户多次点击，
    *          评论完成后恢复使能状态
    * 参数：
    * 返回:   void
    * 时间:   2016/09/26
    * 作者:   hwx
   */
    void setCommentButtonState(bool bEnable);

    /************************************************************************************
    * 名称: setReplyToWho
    * 功能: 添加被回复的对象到评论头
    * 参数：@[in] nCommentId: 被回复的评论id
    * 参数：@[in] strWho: 被回复的对象
    * 返回:   void
    * 时间:   2016/09/26
    * 作者:   hwx
    */
    void setReplyToWho(quint64 nCommentId, const QString &strWho);

    /************************************************************************************
    * 名称: resetUI
    * 功能: 复位界面，清除评论
    * 参数：
    * 返回:   void
    * 时间:   2016/09/26
    * 作者:   hwx
   */
    void resetUI();

    /************************************************************************************
    * 名称: getRealComment
    * 功能: 获取真正的评论内容，去掉回复的对象
    * 参数：[out] 剔除了回复对象的评论，回复对象的格式为@xx:
    * 返回:   评论id，如果回复的是楼主，返回0
    * 时间:   2016/09/26
    * 作者:   hwx
   */
    int getRealComment(QString &strRealComment);

protected:
    // ?????每调用起来
    virtual void keyPressEvent(QKeyEvent * event) override;

signals:
    /************************************************************************************
    * 名称: sigOnComment
    * 功能: 点击评论的信号，将评论内容传给外部控件
    * 参数：@[out] nCommendId: 被评论的评论Id
    * 参数：@[out] strComment: 真正评论内容(不包含回复的对象@xx:)
    * 返回:   void
    * 时间:   2016/09/24
    * 作者:   hwx
   */
    void sigOnComment(quint64 nCommendId, const QString &strComment);

private slots:
    /************************************************************************************
    * 名称: slotOnCommentTextChanged
    * 功能: 评论内容发生变化的槽函数
    * 参数：
    * 返回:   void
    * 时间:   2016/09/24
    * 作者:   hwx
   */
    void slotOnCommentTextChanged();

    /************************************************************************************
    * 名称: slotOnCommentClicked
    * 功能: 点击评论按钮的槽函数
    * 参数：
    * 返回:   void
    * 时间:   2016/09/24
    * 作者:   hwx
   */
    void slotOnCommentClicked();

private:
    /************************************************************************************
    * 名称: hasValidComment
    * 功能: 判断评论框中是否存在有效评论
    * 参数：
    * 返回:   是否存在有效的评论
    * 时间:   2016/09/28
    * 作者:   hwx
   */
    bool hasValidComment();

private:
    int m_nMaxCommentCharNumber;  // 支持的评论最大字符数
    quint64 m_nCommendId;  // 被回复的评论Id, 默认值是0表示回复楼主

    QString m_strReplyHeader; // 评论的头部，如@xx

private:
    Ui::CCommentWidget *ui;
};

#endif // CCOMMENTWIDGET_H
