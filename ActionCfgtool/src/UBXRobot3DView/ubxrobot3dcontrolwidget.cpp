#include "stdafx.h"
#include "ubxrobot3dcontrolwidget.h"
#include "ui_ubxrobot3dcontrolwidget.h"
#include "ubxrobot3dconstants.h"
#include <QMessageBox>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include "configs.h"

QMap<QString, int> UBXRobot3DControlWidget::groupNameID;

UBXRobot3DControlWidget::UBXRobot3DControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBXRobot3DControlWidget),
    m_eReadAngleType(eReadAngleAll),
    m_btnGroup(new QButtonGroup),
    m_pTimerRead(NULL),
    m_bCanRead(true),
    m_bIsReading(false),
    m_pTimerSingleClick(NULL),
    m_bDoubleClicked(false)
{
    m_vecMinAngelList.clear();
    m_vecMaxAngelList.clear();

    ui->setupUi(this);

#ifdef Q_OS_WIN
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    ui->labelIDOperationValue->setText("");
    //将radiobutton加入组，ID对应舵机ID
    m_btnGroup->addButton(ui->btnMotor1, 1);
    m_btnGroup->addButton(ui->btnMotor2, 2);
    m_btnGroup->addButton(ui->btnMotor3, 3);
    m_btnGroup->addButton(ui->btnMotor4, 4);
    m_btnGroup->addButton(ui->btnMotor5, 5);
    m_btnGroup->addButton(ui->btnMotor6, 6);
    m_btnGroup->addButton(ui->btnMotor7, 7);
    m_btnGroup->addButton(ui->btnMotor8, 8);
    m_btnGroup->addButton(ui->btnMotor9, 9);
    m_btnGroup->addButton(ui->btnMotor10, 10);
    m_btnGroup->addButton(ui->btnMotor11, 11);
    m_btnGroup->addButton(ui->btnMotor12, 12);
    m_btnGroup->addButton(ui->btnMotor13, 13);
    m_btnGroup->addButton(ui->btnMotor14, 14);

    groupNameID.insert("right_hand1", 1);   //1号舵机组名
    groupNameID.insert("right_hand2", 2);   //2号舵机组名

    groupNameID.insert("left_hand1", 3);    //3号舵机组名
    groupNameID.insert("left_hand2", 4);    //4号舵机组名

    groupNameID.insert("right_leg1", 5);    //5号舵机组名
    groupNameID.insert("right_leg2", 6);    //6号舵机组名
    groupNameID.insert("right_leg3", 7);    //7号舵机组名

    groupNameID.insert("left_leg1", 8);     //8号舵机组名
    groupNameID.insert("left_leg2", 9);     //9号舵机组名
    groupNameID.insert("left_leg3", 10);    //10号舵机组名

    groupNameID.insert("head1", 11);        //11号舵机组名
    groupNameID.insert("head2", 12);        //12号舵机组名
    groupNameID.insert("head3", 13);        //13号舵机组名

    groupNameID.insert("waist1", 14);       //14号舵机组名


    m_mapNodeRadios.insert(1, ui->btnMotor1);
    m_mapNodeRadios.insert(2, ui->btnMotor2);
    m_mapNodeRadios.insert(3, ui->btnMotor3);
    m_mapNodeRadios.insert(4, ui->btnMotor4);
    m_mapNodeRadios.insert(5, ui->btnMotor5);
    m_mapNodeRadios.insert(6, ui->btnMotor6);
    m_mapNodeRadios.insert(7, ui->btnMotor7);
    m_mapNodeRadios.insert(8, ui->btnMotor8);
    m_mapNodeRadios.insert(9, ui->btnMotor9);
    m_mapNodeRadios.insert(10, ui->btnMotor10);
    m_mapNodeRadios.insert(11, ui->btnMotor11);
    m_mapNodeRadios.insert(12, ui->btnMotor12);
    m_mapNodeRadios.insert(13, ui->btnMotor13);
    m_mapNodeRadios.insert(14, ui->btnMotor14);

    QMap<int, QPushButton*>::const_iterator itor = m_mapNodeRadios.begin();
    for (; itor != m_mapNodeRadios.end(); ++itor)
    {
        QPushButton* pButton = itor.value();
        if (pButton)
        {
            pButton->installEventFilter(this);
        }
    }

    //connect(m_btnGroup,static_cast<void (QButtonGroup:: *)(int, bool)>(&QButtonGroup::buttonToggled),
    //        this, &UBXRobot3DControlWidget::onButtonToggled);

    connect(m_btnGroup, SIGNAL(buttonClicked(int)) , this , SLOT(slotServoIdClicked(int)));

    connect(ui->pushButtonPowerOff , SIGNAL(clicked()) , this , SLOT(slotSingleReadBack()));

    connect(ui->btnHeadReadBack , SIGNAL(clicked()) , this , SLOT(slotHeadReadBack()));

    connect(ui->horizontalSliderAngle, &QSlider::sliderMoved, this, &UBXRobot3DControlWidget::onSliderMoved);

    //connect(ui->horizontalSliderAngle, SIGNAL(valueChanged(int)) , this , SLOT(onSliderAnglevalueChanged(int)));

    ui->horizontalLayout_10->setAlignment(ui->widgetAlpha, Qt::AlignCenter);
    m_pLabelSliderNum = new QLabel(ui->widgetOpContainer);
    m_pLabelSliderNum->setAlignment(Qt::AlignCenter);
    m_pLabelSliderNum->setGeometry(0,0,30,16);
    m_pLabelSliderNum->setObjectName("labelSliderNum");
    m_pLabelSliderNum->hide();

    ui->btnHeadReadBack->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->pushButtonHandReadBack->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->pushButtonReadAll->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->pushButtonLegReadback->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //读取3D模型配置文件
    ReadDefaultAngle(m_vecDefaultMotorDataList);

    m_pTimerSingleClick = new QTimer();
    m_pTimerSingleClick->setSingleShot(true);
    connect(m_pTimerSingleClick, SIGNAL(timeout()) , this , SLOT(slotSingleClicked()));
}

UBXRobot3DControlWidget::~UBXRobot3DControlWidget()
{
    CUtil::ReleaseVector(m_vecDefaultMotorDataList);
    delete ui;
}

void UBXRobot3DControlWidget::UpdateNodeDate(const char *szNodeName, float angle, float offset, TypeAngleChanged nChangeType)
{
    if(false == this->isEnabled())
        return;
    int nGroupID = -1;
    if(groupNameID.contains(szNodeName))
    {
        nGroupID = groupNameID[QString(szNodeName)];
    }

    if(nGroupID >=1 && nGroupID <= Constants::SERVO_COUNT)
    {
        int angleAdjusted = angle - offset;
        //m_mapAngleLabels[nGroupID]->setText(QString::number(angleAdjusted));

        m_bIsReading = false;
        if(nChangeType == TAC_3DModel)
        {
            if(!m_mapNodeRadios[nGroupID]->isChecked())
                m_mapNodeRadios[nGroupID]->setChecked(true);
            if(m_btnGroup->checkedId() == nGroupID) //有选中才设置operation id
            { 
                QString strIDOperationValue = m_btnGroup->checkedButton()->text();
                ui->labelIDOperationValue->setText(strIDOperationValue);

                if(angleAdjusted != ui->horizontalSliderAngle->value())
                {
                    // Add by jianjie 2017/9/18
                    int nMinAngle = m_vecMinAngelList[nGroupID - 1];
                    int nMaxAngle = m_vecMaxAngelList[nGroupID - 1];
                    ui->horizontalSliderAngle->setRange(nMinAngle, nMaxAngle);

                    ui->horizontalSliderAngle->setValue(angleAdjusted);
                    m_pLabelSliderNum->setText(QString::number(angleAdjusted));
                    changeRobotAngle(false, 50);
                }
            }
        }
        else
        {
            if(m_btnGroup->checkedId() == nGroupID/* && angleAdjusted != ui->horizontalSliderAngle->value()*/) //有选中才设置operation id
            {
                if(nChangeType != TAC_ControlView)
                {
                    if(nChangeType == TAC_ReadBack)
                    {
                        m_bIsReading = true;
                    }
                    ui->horizontalSliderAngle->setValue(angleAdjusted);
                    m_pLabelSliderNum->setText(QString::number(angleAdjusted));
                }
                else
                {
                    m_pLabelSliderNum->setText(QString::number(angleAdjusted));
                }
            }
        }
    }

    if(!m_pLabelSliderNum->isVisible() && nChangeType != TAC_RESET && nChangeType != TAC_ReadBack && nChangeType !=TAC_MotionEditerManual)
    {
        m_pLabelSliderNum->show();
    }

    if(m_pLabelSliderNum->isVisible() && m_btnGroup->checkedId() == nGroupID)
    {
        int nSliderWidth = ui->horizontalSliderAngle->width();//这里需要比较刻度条的实际长度，不然会出现问题
        m_pLabelSliderNum->move(ui->horizontalSliderAngle->x() + ui->horizontalSliderAngle->sliderPosition()*nSliderWidth/ui->horizontalSliderAngle->maximum() - m_pLabelSliderNum->width() / 2,
                                 ui->horizontalSliderAngle->y() + ui->horizontalSliderAngle->height());
    }
}

void UBXRobot3DControlWidget::onButtonToggled(int id, bool checked)
{
    emActionLayerType eActionType = ACTION_LAYER_UNKNOWN;
    if(checked)
    {
        emit clickNode(id);

        // 仅用于AlphaMini，动作层切换
        if (id < HAND_MOTOR_COUNT + 1)
        {
            eActionType = ACTION_LAYER_HAND;
        } 
        else if (id > HAND_MOTOR_COUNT + LEG_MOTOR_COUNT)
        {
            eActionType = ACTION_LAYER_HEAD;
        } 
        else
        {
            eActionType = ACTION_LAYER_LEG;
        }

        emit sigShowActionLayer(eActionType);
    }
}

/**************************************************************************
* 函数名: slotServoIdClicked
* 功能: 舵机ID点击响应槽
* 时间: 2017/12/15
* 作者: jianjie
*/
void UBXRobot3DControlWidget::slotServoIdClicked(int nId)
{
    if (m_bDoubleClicked)
    {
        m_bDoubleClicked = false;
    } 
    else
    {
        if (m_pTimerSingleClick)
        {
            m_pTimerSingleClick->start(200);
        }
    }
}

/**************************************************************************
* 函数名: slotSingleClicked
* 功能: 双击定时器超时，响应单击
* 时间: 2017/12/15
* 作者: jianjie
*/
void UBXRobot3DControlWidget::slotSingleClicked()
{
    emActionLayerType eActionType = ACTION_LAYER_UNKNOWN;

    int nId = m_btnGroup->checkedId();
    emit clickNode(nId);

    qDebug() << "clicked!" << "ServoId:" << nId << endl;

    // 仅用于AlphaMini，动作层切换
    if (nId < HAND_MOTOR_COUNT + 1)
    {
        eActionType = ACTION_LAYER_HAND;
    }
    else if (nId > HAND_MOTOR_COUNT + LEG_MOTOR_COUNT)
    {
        eActionType = ACTION_LAYER_HEAD;
    }
    else
    {
        eActionType = ACTION_LAYER_LEG;
    }

    emit sigShowActionLayer(eActionType);
}

//void UBXRobot3DControlWidget::onSliderAnglevalueChanged(int value)
void UBXRobot3DControlWidget::onSliderMoved(int value)
{
    if(ui->horizontalSliderAngle)
    {
        changeRobotAngle(true, 50);
    }
}

void UBXRobot3DControlWidget::on_pushButtonMinus_clicked()
{
    int nNewAngle = ui->horizontalSliderAngle->value()-1;
    if(nNewAngle >= 0)
    {
        ui->horizontalSliderAngle->setValue(nNewAngle);

        QString sTemp;
        sTemp.sprintf("%d", nNewAngle);
        m_pLabelSliderNum->setText(sTemp);

        changeRobotAngle(true, 50);
    }
}

void UBXRobot3DControlWidget::on_pushButtonPlus_clicked()
{
    int nNewAngle = ui->horizontalSliderAngle->value()+1;

    if(nNewAngle <= 240)
    {
        ui->horizontalSliderAngle->setValue(nNewAngle);

        QString sTemp;
        sTemp.sprintf("%d", nNewAngle);
        m_pLabelSliderNum->setText(sTemp);

        changeRobotAngle(true, 50);
    }
}

//void UBXRobot3DControlWidget::onSliderMoved(int value)
//{
//    changeRobotAngle();
//}

//void UBXRobot3DControlWidget::onSliderPressed()
//{
//    changeRobotAngle();
//}

void UBXRobot3DControlWidget::on_pushButtonReset_clicked()
{
//    emit reset();   //20160519万向峰合并代码时注释掉

//    if(m_pConnectPort)
//    {
//        m_pConnectPort->ResetRobot();
//    }


    ResetRobot();
}

/**************************************************************************
* 函数名: on_pushButtonLegReadback_clicked
* 功能: 腿部掉电回读
* 时间: 2017/10/27
* 作者: jianjie
*/
void UBXRobot3DControlWidget::on_pushButtonLegReadback_clicked()
{
    if(!m_bCanRead)
        return;

    m_eReadAngleType = eReadAngleLeg;
    bool bPowerDown = true;             // 是否掉电回读

    for (int i = 1 + HAND_MOTOR_COUNT; i <= HAND_MOTOR_COUNT + LEG_MOTOR_COUNT; ++i)
    {
        m_pEvent->IUBXRobot3DViewEvent_ReadRotation(i , bPowerDown);
    }

    if(NULL == m_pTimerRead)
    {
        m_bCanRead = false;
        m_pTimerRead = new QTimer();
        connect(m_pTimerRead, &QTimer::timeout, this, &UBXRobot3DControlWidget::OnTimeOutRead);
        m_pTimerRead->start(400);
    }
}

/**************************************************************************
* 函数名: on_pushButtonLegReadback_clicked
* 功能: 手部掉电回读
* 时间: 2017/10/27
* 作者: jianjie
*/
void UBXRobot3DControlWidget::on_pushButtonHandReadBack_clicked()
{
    if(!m_bCanRead)
        return;

    m_eReadAngleType = eReadAngleHand;
    bool bPowerDown = true;

    for (int i = 1; i <= HAND_MOTOR_COUNT; ++i)
    {
        m_pEvent->IUBXRobot3DViewEvent_ReadRotation(i , bPowerDown);
    }

    if(NULL == m_pTimerRead)
    {
        m_bCanRead = false;
        m_pTimerRead = new QTimer();
        connect(m_pTimerRead, &QTimer::timeout, this, &UBXRobot3DControlWidget::OnTimeOutRead);
        m_pTimerRead->start(300);
    }
}

/**************************************************************************
* 函数名: on_pushButtonLegReadback_clicked
* 功能: 头部掉电回读
* 时间: 2017/10/27
* 作者: jianjie
*/
void UBXRobot3DControlWidget::slotHeadReadBack()
{
    if(!m_bCanRead)
        return;

    bool bPowerDown = true;

    for (int i = 1 + HAND_MOTOR_COUNT + LEG_MOTOR_COUNT; i <= HAND_MOTOR_COUNT + LEG_MOTOR_COUNT + HEAD_MOTOR_COUNT; ++i)
    {
        m_pEvent->IUBXRobot3DViewEvent_ReadRotation(i);
    }

    if(NULL == m_pTimerRead)
    {
        m_bCanRead = false;
        m_pTimerRead = new QTimer();
        connect(m_pTimerRead, &QTimer::timeout, this, &UBXRobot3DControlWidget::OnTimeOutRead);
        m_pTimerRead->start(300);
    }
}

/**************************************************************************
* 函数名: on_pushButtonLegReadback_clicked
* 功能: 回读所有
* 时间: 2017/10/27
* 作者: jianjie
*/
void UBXRobot3DControlWidget::on_pushButtonReadAll_clicked()
{
    if(!m_bCanRead)
        return;

    m_eReadAngleType = eReadAngleAll;

    vector<MOTOR_DATA*>::iterator item;
    for (item = m_vecDefaultMotorDataList.begin(); item != m_vecDefaultMotorDataList.end(); item++)
    {
        MOTOR_DATA* itemData = *item;
        m_pEvent->IUBXRobot3DViewEvent_ReadRotation(itemData->nID);
    }

    if(NULL == m_pTimerRead)
    {
        m_bCanRead = false;
        m_pTimerRead = new QTimer();
        connect(m_pTimerRead, &QTimer::timeout, this, &UBXRobot3DControlWidget::OnTimeOutRead);
        //全部回读时，禁用时间长一点
        m_pTimerRead->start(500);
    }
}

/**************************************************************************
* 函数名: EnableCtrls
* 功能: 设置控件启用状态
* 参数:
*    @[in ] bEnable: 启用标识
* 返回值: void
* 时间: 2016/01/13 14:40
* 作者: ZDJ
*/
void UBXRobot3DControlWidget::EnableCtrls(bool bEnable)
{
#if 0
    //ui->pushButton_open->setEnabled(bEnable);
    ui->pushButton_addact->setEnabled(bEnable);
    ui->pushButton_removeact->setEnabled(bEnable);
    ui->pushButton_downloadact->setEnabled(bEnable);
    ui->pushButton_udiskmode->setEnabled(bEnable);
    ui->tableWidget_localact->setEnabled(bEnable);
    ui->checkBox_selall->setEnabled(bEnable);
#endif
}

/**************************************************************************
* 函数名: ResetRobot
* 功能: 复位机器人
* 时间: 2016/01/25 17:49
* 作者: ZDJ
*/
void UBXRobot3DControlWidget::ResetRobot()
{
//    int nMotorCount = m_vecDefaultMotorDataList.size();

//#ifdef _DEBUG0
//    print_msg("reset angle:\n");
//#endif
//    MOTOR_DATA* pMotorData = new MOTOR_DATA[nMotorCount];
//    for(size_t i = 0; i < nMotorCount; i++)
//    {
//        MOTOR_DATA* pTempData = m_vecDefaultMotorDataList[i];
//        memcpy(pMotorData + i, pTempData, sizeof(MOTOR_DATA));

//#ifdef _DEBUG0
//        print_msg("%d:%02X ", pMotorData[i].nID, pMotorData[i].nAngle);
//#endif
//    }
//#ifdef _DEBUG0
//    print_msg("\n");
//#endif

//    if(g_pRobotController)
//    {
//        g_pRobotController->Req_RobotReset(pMotorData, nMotorCount, 50, 50);
//    }

//    SAFE_DELETE_ARRAY(pMotorData);

    // Modify by jianjie 2017/10/23
    int         nMotorCount = m_vecDefaultMotorDataList.size();
    MOTOR_DATA* pMotorData = new MOTOR_DATA[nMotorCount];
    int         nTime      = 1000;
    int         nCurveType = 0;

    int i = 0;
    vector<MOTOR_DATA*>::iterator item;
    for (item = m_vecDefaultMotorDataList.begin(); item != m_vecDefaultMotorDataList.end(); ++item)
    {
        MOTOR_DATA* tempData = *item;
        memcpy(pMotorData + i , tempData , sizeof(MOTOR_DATA));
        ++i;

        emit angleChanged(tempData->nID, tempData->nAngle, 1000, TAC_RESET/*TAC_ControlView*/);
    }

    if (m_pEvent)
    {
        m_pEvent->IUBXRobot3DViewEvent_SetAnglesToRobot(pMotorData , nMotorCount , nTime , nCurveType);
    }

    SAFE_DELETE_ARRAY(pMotorData);
}

/**************************************************************************
* 函数名: ReadDefaultAngle
* 功能: 读取舵机默认角度
* 参数:
*    @[out] motorlist: 舵机数据列表
* 返回值: void
* 时间: 2016/01/26 10:15
* 作者: ZDJ
*/
void UBXRobot3DControlWidget::ReadDefaultAngle(vector<MOTOR_DATA*>& motorlist)
{    
    QString sPath = CConfigs::getAppSubDirPath(SUBDIR_ROBOTMODEL) + QDir::separator() + MODEL_XML_NAME;
    QFile file(sPath);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        doc.setContent(&file);
        readModelXml(doc.documentElement(), motorlist);
        file.close();
    }
}

/**************************************************************************
* 函数名: readModelXml
* 功能: 读取3D模型的xml配置文件
* 参数:
*    @[in ] element: dom元素
*    @[out] motorlist: 舵机数据列表
* 返回值: void
* 时间: 2016/01/26 10:15
* 作者: ZDJ
*/
void UBXRobot3DControlWidget::readModelXml(const QDomElement& element, vector<MOTOR_DATA*>& motorlist)
{
    QDomNode node = element.firstChild();
    static MOTOR_DATA* pMotorData = NULL;

    while(!node.isNull())
    {
        QDomElement nodeelem = node.toElement();
        if(nodeelem.tagName() == "Motor")
        {
            pMotorData = new MOTOR_DATA;
            motorlist.push_back(pMotorData);
            readModelXml(nodeelem, motorlist);
        }
        else if(nodeelem.tagName() == "ID")
        {
            QDomNode childnode = node.firstChild();
            if(childnode.nodeType() == QDomNode::TextNode)
            {
                if(pMotorData)
                    pMotorData->nID = childnode.toText().data().toInt();
            }
        }
        else if(nodeelem.tagName() == "InitAngle")
        {
            QDomNode childnode = node.firstChild();
            if(childnode.nodeType() == QDomNode::TextNode)
            {
                if(pMotorData)
                    pMotorData->nAngle = childnode.toText().data().toInt();
            }
        }
        else if (nodeelem.tagName() == "MinAngle")
        {
            QDomNode childnode = node.firstChild();
            if(childnode.nodeType() == QDomNode::TextNode)
            {
                m_vecMinAngelList.push_back(childnode.toText().data().toInt());
            }
        }
        else if (nodeelem.tagName() == "MaxAngle")
        {
            QDomNode childnode = node.firstChild();
            if(childnode.nodeType() == QDomNode::TextNode)
            {
                m_vecMaxAngelList.push_back(childnode.toText().data().toInt());
            }
        }

        node = node.nextSibling();
    }
}

/**************************************************************************
* 函数名: slotSingleReadBack
* 功能: 单个舵机回读响应(掉电回读)
* 时间: 2017/10/27
* 作者: jianjie
*/
void UBXRobot3DControlWidget::slotSingleReadBack()
{
    m_eReadAngleType = eReadAngleSingle;

    int nMotorID = m_btnGroup->checkedId();
    bool bPowerDown = true;
    m_pEvent->IUBXRobot3DViewEvent_ReadRotation(nMotorID , bPowerDown);
}

/**************************************************************************
* 函数名: OnPortOpenChanged
* 功能: 端口改变消息响应
* 时间: 2016/01/28 17:39
* 作者: ZDJ
*/
void UBXRobot3DControlWidget::OnPortOpenChanged(BOOL bOpen)
{
    if(bOpen)
    {
        //端口已连接
        //ui->pushButtonOpenPort->setEnabled(true);
        //ui->pushButtonOpenPort->setText(tr("Disconnect"));
        EnableCtrls(true);
    }
    else
    {
        //端口已关闭
        //ui->pushButtonOpenPort->setEnabled(true);
        //ui->pushButtonOpenPort->setText(tr("Connect To Robot"));
        EnableCtrls(false);
    }
}

void UBXRobot3DControlWidget::OnTimeOutRead()
{
    if(m_pTimerRead)
    {
        delete m_pTimerRead;
        m_pTimerRead = NULL;

        if(!m_bCanRead)
        {
            m_bCanRead = true;
        }
    }
}

void UBXRobot3DControlWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int nSliderWidth = ui->horizontalSliderAngle->width();//这里需要比较刻度条的实际长度，不然会出现问题
    m_pLabelSliderNum->move(ui->horizontalSliderAngle->x() + ui->horizontalSliderAngle->sliderPosition()*nSliderWidth/ui->horizontalSliderAngle->maximum() - m_pLabelSliderNum->width() / 2,
                             ui->horizontalSliderAngle->y() + ui->horizontalSliderAngle->height());
}

void UBXRobot3DControlWidget::changeRobotAngle(bool bEmitSignal, int nTime)
{
    if(m_btnGroup->checkedId() != -1 && ui->horizontalSliderAngle)
    {
        int value = ui->horizontalSliderAngle->value();

        if(!m_bIsReading && m_pEvent)
        {
            int nMotorID = m_btnGroup->checkedId();
            qDebug()<<"slider move "<<value<<endl;
            m_pEvent->IUBXRobot3DViewEvent_SetRotation(nMotorID, value, nTime);
        }
        if(bEmitSignal)
        {
            emit angleChanged(m_btnGroup->checkedId(), value, nTime, TAC_ControlView);
        }
    }
}

void UBXRobot3DControlWidget::timerEvent(QTimerEvent* event)
{
    //int nTimerId = event->timerId();

}

void UBXRobot3DControlWidget::ConnectToMainWidgets(IUBXRobot3DViewEvent* pEvent)
{
    m_pEvent = pEvent;
}


/**************************************************************************
* 函数名: eventFilter
* 功能: 截获舵机编号双击信号，响应双击回读
* 时间: 2017/10/27
* 作者: jianjie
*/
bool UBXRobot3DControlWidget::eventFilter(QObject * obj, QEvent * event)
{
    
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        m_bDoubleClicked = true;
        if (m_pTimerSingleClick)
        {
            m_pTimerSingleClick->stop();
        }
        slotSingleReadBack();
        qDebug() << "double clicked!" << endl;
    }

    return QObject::eventFilter(obj , event);
}
