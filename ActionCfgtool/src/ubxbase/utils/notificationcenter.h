#ifndef _NOTIFYCATIONCENTER_H_
#define _NOTIFYCATIONCENTER_H_
#pragma once

#include "utilsdef.h"
//#include"Ref.h"
#include <iostream>
#include<vector>
#include <list>
#include<functional>
#include "singleton.h"

using namespace std;
using namespace std::placeholders;

class UTILS_EXPORTS NotificationObserver
{
public:
    NotificationObserver(std::function<void(int)> _func, string _msg)
    {
        m_msg = _msg;
        m_func = _func;
    }

    ~NotificationObserver()
    {
    }
    string getName()
    {
        return m_msg;
    }

    void observerCallBack(int ref)
    {
        if (m_func) {
            m_func(ref);
        }
    }
    function<void(int)> getFunction()
    {
        return m_func;
    }
    //重载运算符  
    bool operator ==(NotificationObserver*  _observe)const
    {
        return m_msg == _observe->m_msg;
    }
private:
    string  m_msg;
    std::function<void(int)>  m_func;
};

class UTILS_EXPORTS NotificationCenter : public utils::SingletonT<NotificationCenter>
{
public:
    NotificationCenter();
    virtual ~NotificationCenter();
    //添加观察者  
    //订阅消息的函数，名字标签  
    void addObserver(std::function<void(int)> _func, string _msg);
    //移除观察者  
    void removeObserver(string _msg);
    //清空观察者  
    void removeAllObservers();
    //判断该观察者是否已经添加过了  
    bool observerExisted(function<void(int)>  _func, string _msg);
    //发送消息  
    void postNotification(string _msg);
    //带参数的发送消息  
    void postNotification(string _msg, int _ref);
private:
    vector<NotificationObserver*> m_array;
};
#endif
