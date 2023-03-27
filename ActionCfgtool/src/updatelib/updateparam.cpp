#include "updateparam.h"

UpdateParam::UpdateParam()
{
    m_strModuleName         = "";
    m_strVersionName        = "";
    m_isIncremental         = false;
    m_strPackageUrl         = "";
    m_strPackageMd5         = "";
    m_nPackageSize          = 0;
    m_strIncrementUrl       = "";
    m_strIncrementMd5       = "";
    m_nIncrementSize        = 0;
    m_isForced              = false;
    m_strReleaseNote        = "";
    m_nReleaseTime          = 0;
}

UpdateParam::~UpdateParam ()
{

}
