
/*
* Copyright (c) 2017, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：MemoryReportFilter.cpp
* 创建时间：2017/6/7
* 文件标识：
* 文件摘要：内存检测报告过滤器，过滤非"*.cpp"格式的报告
*
* 当前版本：1.0.0.0
* 作    者：jianjie
* 完成时间：2017/6/7 11:21
* 版本摘要：
*/

//#include "stdafx.h"
#include "MemoryReportFilter.h"

#ifdef Q_OS_WIN

_CRT_REPORT_HOOK prevHook;


// This function is called several times for each memory leak.  
// Each time a part of the error message is supplied.  
// This holds number of subsequent detail messages after  
// a leak was reported  
int ReportingHook(int reportType, char* userMessage, int* retVal)
{
    const int   numFollowupDebugMsgParts    = 2;
    static bool ignoreMessage               = false;
    static int  debugMsgPartsCount          = 0;

    // check if the memory leak reporting starts  
    if (    (strncmp(userMessage,"Detected memory leaks!/n", 10) == 0)
        ||  ignoreMessage)
    {
        // check if the memory leak reporting ends  
        if (strncmp(userMessage,"Object dump complete./n", 10) == 0)
        {
            _CrtSetReportHook(prevHook);
            ignoreMessage = false;
        } 
        else
        {
            ignoreMessage = true;
        }

        // something from our own code?  
        if(strstr(userMessage, ".cpp") == NULL)
        {
            if(debugMsgPartsCount++ < numFollowupDebugMsgParts)
            {
                // give it back to _CrtDbgReport() to be printed to the console  
                return FALSE;
            }
            else 
            {
                return TRUE;  // ignore it
            }
        } 
        else
        {
            debugMsgPartsCount = 0;
            // give it back to _CrtDbgReport() to be printed to the console  
            return FALSE;  
        }  
    } 
    else
    {
        // give it back to _CrtDbgReport() to be printed to the console  
        return FALSE;
    }
}

//change the report function to only report memory leaks from program code
void setFilterDebugHook(void)
{
    
    prevHook = _CrtSetReportHook(ReportingHook);
}

#endif