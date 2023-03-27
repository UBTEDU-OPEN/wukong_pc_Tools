/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：RobotCmd.h
* 创建时间：2015/06/08 11:40
* 文件标识：
* 文件摘要：机器人操作命令类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/06/08 11:40
* 版本摘要：
*/
#ifndef ROBOTCMD_H
#define ROBOTCMD_H

#include "global.h"
#include "type.h"

// jianjie
class Q_DECL_EXPORT CRobotCmd
{
public:
	CRobotCmd(void);
    virtual ~CRobotCmd(void);

	/**************************************************************************
	* 函数名: GetCmd_ReadHardwareVer
	* 功能: 获取读取硬件版本命令数据
	* 参数:
	*    @[out] nDataLenRet:返回的数据长度
	* 返回值: 命令数据
	* 时间: 2015/06/16 16:14
	* 作者: ZDJ
	*/
	byte* GetCmd_ReadHardwareVer(int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_ReadSoftwareVer
	* 功能: 获取读取软件版本命令数据
	* 参数:
	*    @[out] nDataLenRet:返回的数据长度
	* 返回值: 命令数据
	* 时间: 2015/06/16 16:14
	* 作者: ZDJ
	*/
	byte* GetCmd_ReadSoftwareVer(int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_DownloadBinPrepare
	* 功能: 获取开始下载bin文件的命令数据
	* 参数:
	*    @[in ] szBinSrcPath: bin源文件路径
	*    @[in ] sBinDestPath: bin目标文件路径
	*    @[in ] bUTF8: 是否是UTF8目标文件路径
	*    @[out] nDataLenRet: 返回的命令数据长度
	* 返回值: 命令数据，需在外释放
	* 时间: 2015/06/16 16:14
	* 作者: ZDJ
	*/
    byte* GetCmd_DownloadBinPrepare(const char *sBinSrcPath, const char *sBinDestPath, BOOL bUTF8, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_DownloadBinIng
	* 功能: 获取正在下载bin文件的命令数据
	* 参数:
	*    @[in ] curdata: 当前bin文件数据
	*    @[in ] curframe: 当前传输帧
	*    @[out] nDataLenRet: 返回的命令数据长度
	* 返回值: 命令数据，需在外释放
	* 时间: 2015/06/16 16:14
	* 作者: ZDJ
	*/
	byte* GetCmd_DownloadBinIng(const byte* curdata, const int curframe, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_DownloadBinEnd
	* 功能: 获取结束下载bin文件的命令数据
	* 参数:
	*    @[in ] curdata: 当前bin文件数据
	*    @[in ] cursize: 当前传输数据长度
	*    @[out] nDataLenRet: 返回的命令数据长度
	* 返回值: 命令数据，需在外释放
	* 时间: 2015/06/16 16:14
	* 作者: ZDJ
	*/
	byte* GetCmd_DownloadBinEnd(const byte* curdata, const int cursize, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorGetOffset
	* 功能: 获取舵机偏移量回读指令
	* 参数: 
	*    @[in ] nMotorID: 舵机ID
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 偏移量回读指令
	* 时间: 2015/09/08 13:15
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorGetOffset(int nMotorID, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorSetOffset
	* 功能: 获取舵机偏移量设置指令
	* 参数: 
	*    @[in ] nMotorID: 舵机ID
	*    @[in ] nOffset: 偏移值
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 偏移量设置指令
	* 时间: 2015/09/08 13:31
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorSetOffset(int nMotorID, int nOffset, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorVersion
	* 功能: 获取舵机读版本指令
	* 参数: 
	*    @[in ] nMotorID: 舵机ID
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 读版本指令
	* 时间: 2015/09/08 13:31
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorVersion(int nMotorID, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorGetID
	* 功能: 获取舵机读ID指令
	* 参数: 
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 读ID指令
	* 时间: 2015/09/08 13:15
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorGetID(int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorSetID
	* 功能: 获取舵机写ID指令
	* 参数: 
	*    @[in ] nOldMotorID: 旧舵机ID
	*    @[in ] nNewMotorID: 新舵机ID
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 写ID指令
	* 时间: 2015/09/08 13:31
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorSetID(int nOldMotorID, int nNewMotorID, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorUpgradePrepare
	* 功能: 获取进入舵机升级模式指令
	* 参数: 
	*    @[in ] nMotorID: 舵机ID
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 舵机升级模式指令
	* 时间: 2015/09/08 13:31
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorUpgradePrepare(int nMotorID, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorUpgradeIng
	* 功能: 获取正在升级舵机指令
	* 参数: 
	*    @[in ] nMotorID: 舵机ID
	*    @[in ] nCurPage: 当前帧
	*    @[in ] pUserData: 用户数据
	*    @[in ] nUserDataLen: 用户数据长度
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 正在升级舵机指令
	* 时间: 2015/09/08 13:31
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorUpgradeIng(int nMotorID, int nCurPage, byte* pUserData, int nUserDataLen, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorUpgradeEnd
	* 功能: 获取结束升级舵机指令
	* 参数: 
	*    @[in ] nMotorID: 舵机ID
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 结束升级舵机指令
	* 时间: 2015/09/08 13:31
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorUpgradeEnd(int nMotorID, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorMove
	* 功能: 舵机运行命令
	* 参数: 
	*    @[in ] nMotorID: 舵机ID
    *    @[in ] nAngle: 角度
    *    @[in ] nTime: 时间
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 舵机运行命令数据
	* 时间: 2015/09/15 13:17
	* 作者: ZDJ
	*/
    byte* GetCmd_MotorMove(int nMotorID, int nAngle, int nTime, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_MotorGetState
	* 功能: 获取舵机状态
	* 参数: 
	*    @[in ] nMotorID: 舵机ID
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 舵机运行命令数据
	* 时间: 2015/10/29 14:37
	* 作者: ZDJ
	*/
	byte* GetCmd_MotorGetState(int nMotorID, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_RobotDebug
	* 功能: 动作调试
	* 参数: 
	*    @[in ] pMotorBuf: 舵机数据缓冲区
	*    @[in ] nMotorCount: 舵机数量
	*    @[in ] nRunTime: 运行时间
	*    @[in ] nAllTime: 总时间
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据
	* 时间: 2015/10/29 14:44
	* 作者: ZDJ
	*/
	byte* GetCmd_RobotDebug(MOTOR_DATA* pMotorBuf, int nMotorCount, int nRunTime, int nAllTime, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_SwitchMode
	* 功能: 获取切换模式命令数据
	* 参数: 
	*    @[in ] nMode: 模式
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据，无需在外释放
	* 时间: 2015/10/19 13:06
	* 作者: ZDJ
	*/
	byte* GetCmd_SwitchMode(int nMode, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_ConnectRobot
	* 功能: 连接机器人
	* 参数: 
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据，无需在外释放
	* 时间: 2015/10/29 9:18
	* 作者: ZDJ
	*/
	byte* GetCmd_ConnectRobot(int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_DisconnectRobot
	* 功能: 断开机器人
	* 参数: 
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据，无需在外释放
	* 时间: 2015/10/29 9:18
	* 作者: ZDJ
	*/
	byte* GetCmd_DisconnectRobot(int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_RobotReset
	* 功能: 机器人复位
	* 参数: 
	*    @[in ] pMotorBuf: 舵机数据缓冲区
	*    @[in ] nMotorCount: 舵机数量
	*    @[in ] nRunTime: 运行时间
	*    @[in ] nAllTime: 总时间
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据
	* 时间: 2015/10/29 14:44
	* 作者: ZDJ
	*/
	byte* GetCmd_RobotReset(MOTOR_DATA* pMotorBuf, int nMotorCount, int nRunTime, int nAllTime, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_RobotReadSN
	* 功能: 读机器人序列号
	* 参数: 
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据，无需在外释放
	* 时间: 2015/10/29 9:18
	* 作者: ZDJ
	*/
	byte* GetCmd_RobotReadSN(int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_RobotWriteSN
	* 功能: 写机器人序列号
	* 参数: 
	*    @[in ] pszSN: 序列号数据
	*    @[in ] nSNlen: 序列号数据长度
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据，无需在外释放
	* 时间: 2015/10/29 9:18
	* 作者: ZDJ
	*/
	byte* GetCmd_RobotWriteSN(char* pszSN, int nSNlen, int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_RobotReadUID
	* 功能: 读机器人UID
	* 参数: 
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据，无需在外释放
	* 时间: 2015/10/29 9:18
	* 作者: ZDJ
	*/
	byte* GetCmd_RobotReadUID(int& nDataLenRet);

	/**************************************************************************
	* 函数名: GetCmd_RobotCheckUTF8
	* 功能: 检查是否支持UTF8
	* 参数: 
	*    @[out] nDataLenRet: 指令数据长度
	* 返回值: 命令数据，无需在外释放
	* 时间: 2015/10/29 9:18
	* 作者: ZDJ
	*/
	byte* GetCmd_RobotCheckUTF8(int& nDataLenRet);

    /**************************************************************************
    * 函数名: GetCmd_MotorGetAngle
    * 功能: 获取舵机角度命令
    * 参数:
    *    @[in ] nMotorID: 舵机ID
    *    @[out] nDataLenRet: 指令数据长度
    * 返回值: 命令数据，无需在外释放
    * 时间: 2015/10/29 9:18
    * 作者: ZDJ
    */
    byte* GetCmd_MotorGetAngle(int nMotorID, int& nDataLenRet);

};

#endif
