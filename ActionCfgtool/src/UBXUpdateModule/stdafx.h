// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#ifndef STDAFX_H
#define STDAFX_H

// TODO: 在此处引用程序需要的其他头文件
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <ctype.h>
using namespace std;

#ifdef Q_OS_WIN
#include <WtsApi32.h>
#include <UserEnv.h>
#include <TlHelp32.h>


#include <setupapi.h>
#include <Dbt.h>
#include <devguid.h>
#include <DbgHelp.h>

#include <ShlObj.h>
#include "StringFunc.h"
#include "Util.h"
#include "Markup.h"
#include "LanguageMangr.h"
//#include "UI_TEXT_ID.h"
#endif

#include "zxLib.h"
using namespace zxLib;

#include "UBXPublic.h"
#include "Util.h"
#include "json/json.h"

#endif
