/*
* Copyright (c) 2017, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：stlhelper.h
* 创建时间：2015/10/18 14:39
* 文件标识：
* 文件摘要：
* 
* 当前版本：1.0.0.0
* 作    者：leo
* 完成时间：2017/6/7 14:39
* 版本摘要：
*/
#ifndef UBX_BASE_UTILS_TTSINGLETONT_H
#define UBX_BASE_UTILS_TTSINGLETONT_H

namespace utils
{
    template <class T>
    class SingletonT
    {
    public:
        static T *instance();
        static void release();
    protected:
        static T *s_pInstance;
    };
}

#endif
