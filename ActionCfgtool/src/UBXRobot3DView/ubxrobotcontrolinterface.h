#ifndef UBXROBOTCONTROLINTERFACE
#define UBXROBOTCONTROLINTERFACE

class IUBXRobotViewControlNotify
{
public:
    virtual void Set3DViewRotationManual(int nDir, float fRot, const char* pNode, int nTime = 200) = 0;
    virtual void SetRotation(float x, float y, float z, float w, char* pNode) = 0;
    virtual void OnSetRotation(int nID, int nAngle, int iTime) = 0;
    virtual bool IsComConnectted() = 0;
    virtual bool IsRobotDebuging() = 0;
    virtual void LostPowerForRobot(int nID) = 0;
    virtual void OnReadRotation(int nID) = 0;
    virtual void ResetAllAngle() = 0;
    /************************************
     * 名称: UpdateNodeDate
     * 功能: 更新控制界面舵机节点数据
     * 访问权限:    public
     * 参数: [in]const char* szNodeName 舵机节点名
     * 参数: [in]float angle 舵机当前角度
     * 参数: [in]float offset 舵机偏移（根据angle和offset将3D坐标的角度转为0-180°）
     * 参数: [in]TypeAngleChanged nChangeType 引起角度改变的原因
     * 返回:   void
     * 时间:   2015/12/28
     * 作者:   hels
     ************************************/
     virtual void UpdateNodeDate(const char* szNodeName, float angle, float offset, TypeAngleChanged nChangeType) = 0;

};

class IUBXRobotControl
{
public:
    virtual bool Control_GetMotorDatas(char** pData, int& nLen, emActionLayerType eActionType) = 0;
    virtual void Control_SetMotorDatas(char* pData, int nLen, int runTime) = 0;
    virtual int Control_GetMotorAngles(char* pData, int nDataLen, int nMotorID) = 0;
    virtual void Control_SetViewMotorDatasManual(char* pData, int nLen, int runTime) = 0;
    virtual void Control_SetMotorAngle(int nID, int nAngle) = 0;
    virtual void Control_SetMotorPowerState(int nMotorID, int nState) = 0;
    virtual void Control_SetAllMotorPowerState(int nState) = 0;
    //virtual void Control_EnableControl(bool bEnable) = 0;
};

#endif // UBXROBOTCONTROLINTERFACE

