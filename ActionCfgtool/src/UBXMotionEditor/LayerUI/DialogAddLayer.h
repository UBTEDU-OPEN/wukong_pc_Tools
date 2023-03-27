#ifndef DIALOGADDLAYER_H
#define DIALOGADDLAYER_H

#include "motioneditor_global.h"
#include <QDialog>
#include <QCheckBox>

namespace Ui {
class CDialogAddLayer;
}

enum AddLayerItemRoles
{
    LayerType = Qt::UserRole + 1
};

class MOTIONEDITOR_EXPORT CDialogAddLayer : public QDialog
{
    Q_OBJECT

public:
    explicit CDialogAddLayer(QWidget *parent = 0);
    ~CDialogAddLayer();

    /**************************************************************************
    * 函数名：InitLayerList
    * 功能:初始化层列表
    * 参数: wu
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void InitLayerList(int **pLayerID, int &nLayerCount);

    /**************************************************************************
    * 函数名：GetSelectLayer
    * 功能:获取选择的层
    * 参数:
    *    @[in ] ppIDs: 具体选择的层
    *    @[in ] nCount: 选择层的数量
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void GetSelectLayer(int** ppIDs, int &nCount);

    /**************************************************************************
    * 函数名：GetLayerNameByType
    * 功能:根据层的类型获取Layer名字
    * 参数:
    *    @[in ] nType: 层的类型
    * 返回值: QString
    * 时间: 2017/1/16
    * 作者: Joker
    */
    QString GetLayerNameByType(int nType);

    bool IsLayerExist(int nType);

protected:

    /*************
    * 函数名:paintEvent
    * 功能:重绘事件
    * 参数:
    * 返回值:
    * 时间: 2016/06/01
    * 作者: 周志平
    *****************/
    virtual void paintEvent(QPaintEvent *);

private slots:
    void on_pbClose_clicked();

    void on_pbCancel_clicked();

    void on_pbOK_clicked();

    void slotCheckState(int);

private:
    Ui::CDialogAddLayer *ui;
    int* m_pIDs;
    int m_nSelectCount;
    QVector<int> m_vUnUseID;
    QMap <QCheckBox*, int> m_mapCheckBox;

    QCheckBox* findCheckBoxByID(int nLayerID);
};

#endif // DIALOGADDLAYER_H
