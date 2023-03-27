#ifndef UBTLIB_H
#define UBTLIB_H
#include <string>

/**************************************************************************
* 函数名: Req_PackageCmdData
* 功能: 请求打包命令数据
* 参数:
*    @[in ] nCmdId: 命令ID
*    @[in ] pDataBuf: 数据缓冲区地址
*    @[in ] nDataLen: 数据缓冲区长度
*    @[out] MYDATA*: 分包数据列表
* 返回值: 成功返回帧数据缓冲区长度，失败返回-1
* 时间: 2018/03/08 17:34
* 作者: Joker
*/
int Req_PackageCmdData(int nCmdId, unsigned char *pDataBuf, int nDataLen, unsigned short nDelayTime, unsigned char *&pOutData);

/**************************************************************************
* 函数名: UBXPackageFrame
* 功能: 打包帧数据
* 参数:
*    @[in ] pDataBuf: 用户数据缓冲区地址
*    @[in ] nDataLen: 用户数据缓冲区长度
*    @[in ] nCmdId: 命令ID
*    @[in ] nFrameCount: 帧数量
*
*
*    @[in ] nFrameIndex: 帧序号，从1开始
*    @[out] pOutData: 帧数据缓冲区地址，需在外释放
* 返回值: 成功返回帧数据缓冲区长度，失败返回-1
* 时间: 2016/07/08 17:34
* 作者: ZDJ
*/
int PackageFrame(unsigned char* pDataBuf, int nDataLen, unsigned short nCmdId, short nFrameIndex, unsigned char*& pOutData);

std::string getHeaderXUBTSignV3(std::string deviceId);
std::string getUpdateHeaderXUBTSignV3(std::string deviceId);

std::string GetBasicParam(int nType, std::string strVersion);

/**************************************************************************
* 函数名: CreateHtsDataFromAes
* 功能: 从aes生成hts格式数据
* 参数:
*    @[in ] sAesPath: aes文件路径
*    @[out] pOutData: hts数据缓冲区
*    @[out] nOutLen: hts数据缓冲区长度
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2015/08/14 13:20
* 作者: ZDJ
*/
bool CreateHtsDataFromAes(const std::string &sAesPath, char*& pOutData, int& nOutLen);


/**************************************************************************
* 函数名: CreateHtsDataFromAes
* 功能: 从aes生成hts格式数据
* 参数:
*    @[in ] pInAesFileData: aes文件数据
*    @[in ] lInLen: aes文件数据长度
*    @[out] pOutData: hts数据缓冲区
*    @[out] nOutLen: hts数据缓冲区长度
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2016/06/24
* 作者: suhaiwu
*/
bool CreateHtsDataFromAes(char* pInAesFileData, const long long lInLen, char*& pOutData, int& nOutLen);

/**************************************************************************
* 函数名: CreateHtsDataFromAes
* 功能: 从aes生成hts格式数据
* 参数:
*    @[in ] sAesPath: aes文件路径
*    @[in ] nActType: 动作类型
*    @[in ] nDocType: 文档类型
*    @[in ] sActDesc: 动作描述信息
*    @[out] pOutData: hts数据缓冲区
*    @[out] nOutLen: hts数据缓冲区长度
* 返回值: 成功返回true，失败返回false
* 时间: 2015/08/14 13:20
* 作者: ZDJ
*/
bool CreateHtsDataFromAes(const std::string& sAesPath,
        int nActType,
        int nDocType,
        const std::string& sActDesc,
        char*& pOutData,
                          int& nOutLen);


/**************************************************************************
* 函数名: ParseHeader
* 功能: 解析文件头
* 参数:
*    @[in ] pAesData: aes数据
*    @[in ] nDataLen: aes数据长度
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2015/08/20 13:50
* 作者: ZDJ
*/
bool ParseHeader(unsigned char* pAesData, int nDataLen, void* pMyTabData);

#endif // UBTLIB_H
