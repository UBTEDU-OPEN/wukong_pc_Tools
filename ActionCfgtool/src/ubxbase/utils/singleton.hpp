/*
* Copyright (c) 2017, ????????????
* All rights reserved.
*
* ?????stlhelper.h
* ?????2015/10/18 14:39
* ?????
* ?????
*
* ?????1.0.0.0
* ?    ??leo
* ?????2017/6/7 14:39
* ?????
*/
#include "singleton.h"

#ifndef NULL
#define NULL 0
#endif

namespace utils
{
    template <class T>
    T * SingletonT<T>::instance()
    {
        if (NULL == s_pInstance)
        {
            s_pInstance = new T();
        }
        return s_pInstance;
    }

    template <class T>
    void SingletonT<T>::release()
    {
        if (NULL != s_pInstance)
        {
            delete s_pInstance;
            s_pInstance = NULL;
        }
    }

#define IMPL_SINGLETON_CLASS(subClass)  \
    namespace utils \
    {   \
        template<> subClass * SingletonT<subClass>::s_pInstance = NULL;   \
    }\
    template class utils::SingletonT<subClass>
}
