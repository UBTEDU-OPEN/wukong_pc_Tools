#ifndef _BASE_UTILS_FORMATFUNC_H
#define _BASE_UTILS_FORMATFUNC_H

#include "utilsdef.h"
#include <string>
//#include <boost/filesystem/path.hpp>
#include <limits>
#include <vector>
//#include "type.h"
#ifdef __MACH__
#include <stdlib.h>
#endif
using std::string;
using std::wstring;
namespace utils
{
    template <typename T>
    inline T getInvalidValue()
    {
        return (std::numeric_limits<T>::max)();
    }

    template <>
    inline bool getInvalidValue()
    {
        return false;
    }

    template <typename T>
    inline bool isInvalidValue(T t)
    {
        return t == getInvalidValue<T>();
    }

    template <>
    inline std::vector<int> getInvalidValue()
    {
        std::vector<int> vei(5, 0);
        return vei;
    }

    template <>
    inline std::vector<double> getInvalidValue()
    {
        std::vector<double> vei(5, 0);
        return vei;
    }
    template <>
    inline const std::vector<double>& getInvalidValue()
    {
        static std::vector<double> vei(5, 0);
        return vei;
    }

    template <typename T>
    inline void safeDeletePtr(T *&p)
    {
        if (NULL != p)
        {
            delete p;
            p = NULL;
        }
    }

    template <typename T>
    inline void safeDeleteArr(T *&p)
    {
        if (NULL != p)
        {
            delete [] p;
            p = NULL;
        }
    }

    template <typename TContainer>
    inline void safeClearContainer(TContainer *pContainer)
    {
        typedef typename TContainer::iterator TContainerItr;
        if (NULL != pContainer)
        {
            for (TContainerItr it = pContainer->begin();
                it != pContainer->end(); ++it)
            {
                delete (*it);
            }
            pContainer->clear();
        }
    }
    
    template<typename T>
    T stdMax(const T& a,const T& b)
    {
        return a<b?b:a;
    }

    template<typename T>
    T stdMin(const T& a,const T& b)
    {
        return a>b?b:a;
    }

    template <typename T>
    inline void limitValue(T &dst, const T &v1, const T &v2)
    {
        T realMin = stdMin<T>(v1, v2);
        T realMax = stdMax<T>(v1, v2);
        dst = stdMax<T>(dst, realMin);
        dst = stdMin<T>(dst, realMax);
    }

    inline std::string str_s(const char * const sz)
    {
        return (NULL != sz) ? sz : "";
    }

    inline int atoi_s(const char * const sz)
    {
        return (NULL != sz) ? atoi(sz) : getInvalidValue<int>();
    }

    inline double atof_s(const char * const sz)
    {
        return (NULL != sz) ? atof(sz) : getInvalidValue<double>();
    }

    UTILS_EXPORTS std::string get_dir(const std::string &fileName);

    //UTILS_EXPORTS std::string dirString(const boost::filesystem::path &path);

    UTILS_EXPORTS std::string ws2s(std::wstring str);
    UTILS_EXPORTS std::wstring s2ws(std::string str);
    UTILS_EXPORTS int ansi2wc(const char* pszAnsiStr, int nAnsiLen, wchar_t *pWCharStr);
    UTILS_EXPORTS int wc2ansi(const wchar_t *pWCharStr, int nWCharLen, char* pszAnsiStr);
    UTILS_EXPORTS std::string wc2s(const wchar_t *pwstr, const int &len);
    UTILS_EXPORTS const std::string unicode2ansi(const std::wstring &src);
    UTILS_EXPORTS const std::wstring ansi2unicode(const std::string &src);
    UTILS_EXPORTS const std::string ws2utf8(const std::wstring &src);
    UTILS_EXPORTS const std::wstring utf8_2_ws(const std::string &src);
    UTILS_EXPORTS const std::string utf8_2_ansi(const std::string &src);
    UTILS_EXPORTS const std::string ansi2utf8(const std::string &src);
    //enum EFormatNumberFlag
    //{
    //    FNF_NONE            = 0x00000000,
    //    
    //    FNF_PRECISION_1     = 0x00000001,
    //    FNF_PRECISION_2     = 0x00000002,
    //    FNF_PRECISION_3     = 0x00000003,
    //    FNF_PRECISION_4     = 0x00000004,
    //    FNF_PRECISION_5     = 0x00000005,
    //    FNF_PRECISION_6     = 0x00000006,
    //    FNF_PRECISION_MASK  = 0x0000FFFF,

    //    FNF_PERCENTAGE      = 0x00010000,
    //    FNF_SEGMENTED       = 0x00020000,
    //    FNF_AUTO_TRIM       = 0x00040000,
    //    FNF_TENTHOUSAND_SEGMENTED   = 0x00080000,
    //};

    //enum ERMBWriteFlag
    //{
    //    RMB_NOMAL = 0,
    //    RMB_UPPER,
    //};

    //enum EUnitNameFlag
    //{
    //    UNIT_NAME_FEN = 0,
    //    UNIT_NAME_YUAN,
    //};

    //UTILS_EXPORTS(FFormatNumberFlag, EFormatNumberFlag)

    //UTILS_EXPORTS string formatIntNumber(int nNum, int nLevel = 0, FFormatNumberFlag flag = FNF_NONE);
    //UTILS_EXPORTS string formatDoubleNumber(const long double dPrice, FFormatNumberFlag flag = FNF_PRECISION_2);
    //UTILS_EXPORTS string formatLongLongNumber(long long longNum, int nLevel = 0, FFormatNumberFlag flag = FNF_NONE);

    //UTILS_EXPORTS string numToChinese(int nNum);
    //UTILS_EXPORTS string doubleToChinese(double dNum, double dDecNum, EUnitNameFlag flag = UNIT_NAME_FEN);
}

//TT_DECLARE_OPERATORS_FOR_FLAGS(utils::FFormatNumberFlag)

#define     XT_VALID(value, type, def)  (!utils::isInvalidValue<type>((value)) ? (value) : (def))

#endif
