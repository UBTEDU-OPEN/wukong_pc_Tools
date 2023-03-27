//#include <boost/format.hpp>
//#include <boost/lexical_cast.hpp>
#include "formatfunc.h"
#include <stdio.h>
#include <locale>
#include <codecvt>

#ifdef _WIN32
#include <Windows.h>
#include <stringapiset.h>
#endif

namespace utils
{
    std::string get_dir(const std::string &fileName)
    {
        size_t idx = fileName.rfind('/');
        if (std::string::npos == idx)
        {
            idx = fileName.rfind('\\');
        }
        if (std::string::npos != idx)
        {
            return fileName.substr(0, idx);
        }
        return fileName;
    }

//    std::string dirString(const boost::filesystem::path &path)
//    {
//#if defined BOOST_WINDOWS_API
//        return wsToString(path.c_str());
//#else
//        return path.c_str();
//#endif // BOOST_WINDOWS_API
//    }

    std::string ws2s(std::wstring str)
    {
        if (0 == str.compare(L"")) {
            return "";
        }

#ifdef _WIN32
        int nLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
        if (nLen <= 0) return std::string("");
        char* pszDst = new char[nLen];
        if (NULL == pszDst) return std::string("");
        WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pszDst, nLen, NULL, NULL);
        pszDst[nLen -1] = 0;
        std::string strTemp(pszDst);
        delete [] pszDst;
        return strTemp;
#else
        size_t nLen = wcstombs(NULL, str.c_str(), 0) + 1;
        if(nLen <= 0) return string("");
        char* pszDst = new char[nLen];
        if (NULL == pszDst) return string("");
        memset(pszDst, 0, nLen);
        nLen = wcstombs(pszDst, str.c_str(), nLen);
        if(nLen <= 0)
        {
            delete [] pszDst;
            return string("");
        }
        string strTemp(pszDst);
        delete [] pszDst;
        return strTemp;

#endif // WIN32
    }

    std::wstring s2ws(std::string str)
    {
#ifdef _WIN32

        int nLen = str.length();
        int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, 0, 0);
        if(nSize <= 0) return NULL;
        wchar_t *pwszDst = new wchar_t[nSize+1];
        if( NULL == pwszDst) return NULL;
        MultiByteToWideChar(CP_ACP, 0,(LPCSTR)str.c_str(), nLen, pwszDst, nSize);
        pwszDst[nSize] = 0;
        if( pwszDst[0] == 0xFEFF) // skip Oxfeff
            for(int i = 0; i < nSize; i ++)
                pwszDst[i] = pwszDst[i+1];
        std::wstring wchar_tString(pwszDst);
        delete [] pwszDst;
        return wchar_tString;

#else
        int nLen = str.length();
        int nSize = mbstowcs(NULL, str.c_str(), 0) + 1;
        if(nSize <= 0) return NULL;
        wchar_t *pwszDst = new wchar_t[nSize];
        if( NULL == pwszDst) return NULL;
        wmemset(pwszDst, 0, nSize);
        nSize = mbstowcs(pwszDst, str.c_str(), nLen);
        if(nSize <= 0) return NULL;
        if( pwszDst[0] == 0xFEFF) // skip Oxfeff
            for(int i = 0; i < nSize; i ++)
                pwszDst[i] = pwszDst[i+1];
        wstring wchar_tString(pwszDst);
        delete [] pwszDst;
        return wchar_tString;

#endif // WIN32
    }

    int ansi2wc(const char* pszAnsiStr, int nAnsiLen, wchar_t *pWCharStr)
    {
        int ret = -1;
        if (pszAnsiStr == NULL || pWCharStr == NULL || nAnsiLen < 1)
            return ret;
#ifdef _WIN32
        ret = MultiByteToWideChar(CP_ACP, 0, pszAnsiStr,
            strlen(pszAnsiStr) + 1, pWCharStr, nAnsiLen / sizeof(pWCharStr[0]));
#else
        ret = mbstowcs(pWCharStr, pszAnsiStr, nAnsiLen);
#endif
        return ret;
    }

    int wc2ansi(const wchar_t *pWCharStr, int nWCharLen, char* pszAnsiStr)
    {
        int ret = -1;
        if (pszAnsiStr == NULL || pWCharStr == NULL || nWCharLen < 1)
            return ret;
#ifdef _WIN32
        ret = WideCharToMultiByte(CP_ACP, 0, pWCharStr, -1,
            pszAnsiStr, nWCharLen, NULL, NULL);
#else
        ret = wcstombs(pszAnsiStr, pWCharStr, nWCharLen);
#endif
        return ret;
    }
    /**************************************************************************
    * 函数名:   wc2s
    * 功能:
    * 参数:
    *....@[in]  const WCHAR * pwstr
    *....@[in]  int len
    *....@[out] UTILS_EXPORTS std::string
    * 返回值:
    * 时间:     2017/09/21 19:57
    * 作者:   Leo
    */
    std::string wc2s(const wchar_t *pwstr, const int &len)
    {
        std::string str_ret = "";
        if (pwstr == nullptr || len < 1)
            return str_ret;
        int result_len = 0;
        char *psz = new char[len];
        memset(psz, 0, len);
#ifdef _WIN32
        result_len = WideCharToMultiByte(CP_ACP, 0, pwstr, -1,
            psz, len, NULL, NULL);
#else
        result_len = wcstombs(psz, pwstr, len);

#endif
        str_ret = psz;
        //str_ret.copy(psz, len);
        delete[] psz;
        return str_ret;
    }

    const std::string unicode2ansi(const std::wstring& src)
    {
#ifndef __MACH__
        std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>
            cvt_ansi(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
        return cvt_ansi.to_bytes(src);
#else
        return "";
#endif
    }

    const std::wstring ansi2unicode(const std::string& src)
    {
#ifndef __MACH__
        std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>
            cvt_ansi(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
        return cvt_ansi.from_bytes(src);
#else
        return L"";
#endif
    }

    const std::string ws2utf8(const std::wstring &src)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(src);
    }

    const std::wstring utf8_2_ws(const std::string &src)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
        return conv.from_bytes(src);
    }

    const std::string utf8_2_ansi(const std::string &src)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
        std::wstring ws = std::move(conv.from_bytes(src));
        return unicode2ansi(ws);
    }

    const std::string ansi2utf8(const std::string &src)
    {
        const std::wstring ws = std::move(ansi2unicode(src));
        return ws2utf8(ws);
    }

//    std::string formatIntNumber( int nNum, int nLevel /*= 0*/, FFormatNumberFlag flag /*= FNF_NONE*/ )
//    {
//        if (isInvalidValue<int>(nNum))
//        {
//            return "";
//        }
//
//        std::string strValue = boost::lexical_cast<string>(nNum);
//        while (strValue.size() < (size_t)nLevel)
//        {
//            strValue = "0" + strValue;
//        }
//
//        if (flag & FNF_SEGMENTED)
//        {
//            for (int i = static_cast<int>(strValue.size() - 3); i > 0; i -= 3)
//            {
//                strValue = strValue.substr(0, i) + "," + strValue.substr(i, strValue.size() - i);
//            }
//        }
//
//        return strValue;
//    }
//
//    std::string formatDoubleNumber(const long double d, FFormatNumberFlag flag /*= FNF_NONE*/)
//    {
//        long double dPrice = d;
//        if (utils::isZero(dPrice, 1e-6))
//        {
//            dPrice = 0.000;
//        }
//
//        int nPrecision = flag & FNF_PRECISION_MASK;
//        
//        std::string str;
//        if (utils::isLess(abs(dPrice), 1e36))
//        {
//            std::string strFormat = "%." + boost::lexical_cast<string>(nPrecision)+"f";
//            if (flag & FNF_PERCENTAGE)
//            {
//                str = (boost::format(strFormat) % (dPrice * 100.0)).str() + "%";
//            }
//            else
//            {
//                if (utils::isLess(abs(dPrice), 1e4) || !(flag & FNF_AUTO_TRIM))
//                {
//                    str = (boost::format(strFormat) % (dPrice)).str();
//                }
//                else if (utils::isLess(abs(dPrice), 1e8))
//                {
//                    str = (boost::format(strFormat) % (dPrice / 1e4)).str() + "万";
//                }
//                else
//                {
//                    str = (boost::format(strFormat) % (dPrice / 1e8)).str() + "亿";
//                }
//            }
//        }
//        else
//        {
//            str = "";
//        }
//
//        if (!str.empty() && flag & FNF_SEGMENTED)
//        {
//            int idx = str.find('.');
//            int i = idx - 3;
//            while (i > 0 && str[i - 1] >= '0' && str[i - 1] <= '9')
//            {
//                str.insert(i, ",");
//                i -= 3;
//            }
//        }
//
//        return str;
//    }
//
//    std::string formatLongLongNumber(long long longNum, int nLevel /*= 0*/, FFormatNumberFlag flag /*= FNF_NONE*/)
//    {
//        if (isInvalidValue<long long>(longNum))
//        {
//            return "";
//        }
//
//        std::string strValue = boost::lexical_cast<std::string>(longNum);
//        while (strValue.size() < (size_t)nLevel)
//        {
//            strValue = "0" + strValue;
//        }
//
//        if (flag & FNF_SEGMENTED)
//        {
//            for (int i = static_cast<int>(strValue.size() - 3); i > 0; i -= 3)
//            {
//                strValue = strValue.substr(0, i) + "," + strValue.substr(i, strValue.size() - i);
//            }
//        }
//
//        return strValue;
//    }
//
//    bool numBelowWanToChinese(int nNumBelowWan, std::string & str, ERMBWriteFlag flag = RMB_NOMAL)
//    {
//        static std::string s_nomalUnit[] = { "零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十" };
//        static std::string s_upperUnit[] = { "零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖", "拾" };
//
//        int nThousand = nNumBelowWan / 1000;
//        nNumBelowWan %= 1000;
//        int nHundred = nNumBelowWan / 100;
//        nNumBelowWan %= 100;
//        int nTen = nNumBelowWan / 10;
//        int nUnit = nNumBelowWan % 10;
//
//        std::stringstream ss;
//
//        bool bEnableZero = false;
//        if (nThousand > 0)
//        {
//            ss << (RMB_NOMAL == flag ? s_nomalUnit[nThousand] : s_upperUnit[nThousand]);
//            ss << (RMB_NOMAL == flag ? "千" : "仟");
//            bEnableZero = true;
//        }
//
//        bool bExpectingZero = false;
//        if (nHundred > 0)
//        {
//            ss << (RMB_NOMAL == flag ? s_nomalUnit[nHundred] : s_upperUnit[nHundred]);
//            ss << (RMB_NOMAL == flag ? "百" : "佰");
//            bEnableZero = true;
//        }
//        else if (bEnableZero)
//        {
//            bExpectingZero = true;
//        }
//
//        if (nTen > 0)
//        {
//            if (bExpectingZero)
//            {
//                ss << "零";
//                bExpectingZero = false;
//            }
//            ss << (RMB_NOMAL == flag ? s_nomalUnit[nTen] : s_upperUnit[nTen]);
//            ss << (RMB_NOMAL == flag ? "十" : "拾");
//            bEnableZero = true;
//        }
//        else if (bEnableZero)
//        {
//            bExpectingZero = true;
//        }
//
//        if (nUnit > 0)
//        {
//            if (bExpectingZero)
//            {
//                ss << "零";
//                bExpectingZero = false;
//            }
//            ss << (RMB_NOMAL == flag ? s_nomalUnit[nUnit] : s_upperUnit[nUnit]);
//        }
//
//        str = ss.str();
//        return nThousand > 0;
//    }
//
//    std::string numToChinese(int nNum)
//    {
//        if (isInvalidValue<int>(nNum))
//        {
//            return "";
//        }
//        if (0 == nNum)
//        {
//            return "零";
//        }
//
//        int nYi = int(nNum / 1e8);
//        int nYiLeft = nNum % (int)1e8;
//
//        int nWan = int(nYiLeft / 1e4);
//        int nWanLeft = nYiLeft % (int)1e4;
//
//        std::stringstream ss;
//
//        bool bEnableZero = false;
//        if (nYi > 0)
//        {
//            std::string strYi;
//            numBelowWanToChinese(nYi, strYi);
//            ss << strYi << "亿";
//            bEnableZero = true;
//        }
//
//        bool bExpectingZero = false;
//        if (nWan > 0)
//        {
//            std::string strWan;
//            bool bFull = numBelowWanToChinese(nWan, strWan);
//            if (bEnableZero && !bFull)
//            {
//                ss << "零";
//            }
//            ss << strWan << "万";
//            bEnableZero = true;
//        }
//        else if (bEnableZero)
//        {
//            bExpectingZero = true;
//        }
//
//        if (nWanLeft > 0)
//        {
//            std::string strUnit;
//            bool bFull = numBelowWanToChinese(nWanLeft, strUnit);
//            if (bExpectingZero || (bEnableZero && !bFull))
//            {
//                ss << "零";
//                bExpectingZero = false;
//            }
//            ss << strUnit;
//        } 
//        return ss.str();
//    }
//    
//    std::string doubleToChinese(double dNum, double dDecNum, EUnitNameFlag flag)
//    {
//        if (isInvalidValue<double>(dNum))
//        {
//            return "";
//        }
//        if (0 == dNum)
//        {
//            return "零";
//        }
//
//        int nYi = int(dNum / 1e8);
//        int nYiLeft =  (int)(dNum - nYi * 1e8);
//
//        double dDecimals = (dNum - nYi * 1e8 - nYiLeft) * (int)pow(10, dDecNum);
//        int nDecimal = (int)(dDecimals + 0.5);
//
//        std::stringstream ss;
//
//        if ((int)dDecNum > 6)
//        {
//            ss << "最多保留6位小数";
//        }
//        else
//        {
//            if (nYi > 0)
//            {
//                std::string strYi;
//                numBelowWanToChinese(nYi, strYi);
//                ss << strYi << "亿";
//            }
//            
//            if (nYiLeft > 0)
//            {
//                ss << numToChinese(nYiLeft);
//            }
//
//            int nDecimals[6] = {0};
//            int nDebNum = (int)dDecNum;
//
//            bool bDecimalsZero[6] = {true, true, true, true, true, true};
//            bool bDecimalZero = true;
//
//            std::string sDecimal;
//            std::string sDecimals[] = { "点", "点零", "点零零", "点零零零", "点零零零零", "点零零零零零" };
//
//            for (int i = 0; i < nDebNum; i ++)
//            { 
//                nDecimals[i] = nDecimal / (int)pow(10, dDecNum-1);
//                nDecimal = nDecimal % (int)pow(10, dDecNum-1);
//                dDecNum -= 1;
//                if (nDecimals[i] > 0)
//                {
//                    numBelowWanToChinese(nDecimals[i], sDecimal);
//                    if (bDecimalZero)
//                    {
//                        ss << sDecimals[i] << sDecimal;
//                    }
//                    else
//                    {
//                        for (int n = i-1; n > 0; n --)
//                        {
//                            if (bDecimalsZero[n])
//                            {
//                                ss << "零";
//                            }
//                            else
//                            {
//                                break;
//                            }
//                        }
//                        ss << sDecimal;
//                    }
//                    bDecimalsZero[i] = false;
//                }
//
//                bDecimalZero = bDecimalZero && bDecimalsZero[i];
//            }
//
//            ss << (UNIT_NAME_FEN == flag ? "份" : "元");
//        }
//
//        return ss.str();
//    }
}
