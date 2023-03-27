#include "notificationcenter.h"
#include "singleton.hpp"

IMPL_SINGLETON_CLASS(NotificationCenter);

NotificationCenter::NotificationCenter()
{
    m_array.clear();
}


NotificationCenter::~NotificationCenter()
{
}

/**************************************************************************
* 函数名:   addObserver
* 功能:
* 参数:
*....@[in]  std::function<void
*....@[in]  int
*....@[in]  > _func
*....@[in]  string _msg
*....@[out] void
* 返回值:
* 时间:     2017/09/25 14:47
* 作者:   Leo
*/
void NotificationCenter::addObserver(std::function<void(int)> _func, string _msg)
{
    if (this->observerExisted(_func, _msg)) {
        return;
    }
    NotificationObserver *observe = new NotificationObserver(_func, _msg);
    m_array.push_back(observe);
}

/**************************************************************************
* 函数名:   removeObserver
* 功能:
* 参数:
*....@[in]  string _msg
*....@[out] void
* 返回值:
* 时间:     2017/09/25 14:52
* 作者:   Leo
*/
void NotificationCenter::removeObserver(string _msg)
{
    auto itor = m_array.begin();
    int i = 0;
    for (itor = m_array.begin(); itor != m_array.end();)
    {
        if (((*itor)->getName() == _msg)) {
            delete *itor;
            itor = m_array.erase(itor);
        } else {
            ++i;
            ++itor;
        }
    }
}

/**************************************************************************
* 函数名:   removeAllObservers
* 功能:
* 参数:
*....@[out] void
* 返回值:
* 时间:     2017/09/25 14:52
* 作者:   Leo
*/
void NotificationCenter::removeAllObservers()
{
    vector<NotificationObserver*>::iterator itor;
    itor = m_array.begin();
    for (itor = m_array.begin(); itor != m_array.end(); ) {
        delete *itor;
        ++itor;
    }
    m_array.clear();
}

/**************************************************************************
* 函数名:   observerExisted
* 功能:
* 参数:
*....@[in]  function<void
*....@[in]  int
*....@[in]  > _func
*....@[in]  string _msg
*....@[out] bool
* 返回值:
* 时间:     2017/09/25 14:52
* 作者:   Leo
*/
bool NotificationCenter::observerExisted(function<void(int)> _func, string _msg)
{
    NotificationObserver *obj = nullptr;
    NotificationObserver* _observer = new NotificationObserver(_func, _msg);
    bool _existed = false;
    auto it = m_array.begin();
    for (; it != m_array.end(); ++it) {
        obj = *it;
        if (!obj) {
            continue;
        }
        if (obj == _observer) {
            _existed = true;
            break;
        }
    }
    delete _observer;
    return _existed;
}

/**************************************************************************
* 函数名:   postNotification
* 功能:
* 参数:
*....@[in]  string _msg
*....@[out] void
* 返回值:
* 时间:     2017/09/25 14:52
* 作者:   Leo
*/
void NotificationCenter::postNotification(string _msg)
{
    int _ref = 0;
    postNotification(_msg, _ref);
}

/**************************************************************************
* 函数名:   postNotification
* 功能:
* 参数:
*....@[in]  string _msg
*....@[in]  int _ref
*....@[out] void
* 返回值:
* 时间:     2017/09/25 14:52
* 作者:   Leo
*/
void NotificationCenter::postNotification(string _msg, int _ref)
{
    for (auto sp : m_array) {
        if (sp->getName() == _msg) {
            sp->observerCallBack(_ref);
        }
    }
}
