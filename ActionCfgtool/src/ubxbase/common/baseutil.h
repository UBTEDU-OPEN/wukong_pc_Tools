#ifndef __BASE_UTILS_H_
#define __BASE_UTILS_H_

#define SAFE_FREE(p) {if(p){ free(p); p = NULL;}}
#define SAFE_DELETE(p) {if(p){ delete p; p = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p){ delete [] p; p = NULL;}}
#define SAFE_FILE_CLOSE(p) {if(p){ fclose(p); p = NULL;}}
#define SAFE_CLOSE(p) {if(p){ p->Close(); p = NULL;}}
#define NULLSTR ""


#define STRUCT_CONSTRUCTOR_DEFAULT(T)         \
T()                                           \
{                                             \
	memset(this, 0, sizeof(T));               \
}

#define STRUCT_CONSTRUCTOR_COPY(T)            \
T(const T& obj)                               \
{                                             \
	if (this != &obj)                         \
	{                                         \
		memcpy(this, &obj, sizeof(T));        \
	}                                         \
}


#define STRUCT_OPERATOR_ASSIGN(T)             \
T& operator = (const T& obj)                  \
{                                             \
	if (this != &obj)                         \
	{                                         \
		memcpy(this, &obj, sizeof(T));        \
	}                                         \
	return *this;                             \
}

#define STRUCT_CONSTRUCTOR(T)                 \
T()                                           \
{                                             \
	memset(this, 0, sizeof(T));               \
}                                             \
T(const T& obj)                               \
{                                             \
	if (this != &obj)                         \
	{                                         \
		memcpy(this, &obj, sizeof(T));        \
	}                                         \
}                                             \
T& operator = (const T& obj)                  \
{                                             \
	if (this != &obj)                         \
	{                                         \
		memcpy(this, &obj, sizeof(T));        \
	}                                         \
	return *this;                             \
}


#define UBX_MEMCPY_INC(t, s) memcpy(&t, s, sizeof(t));s+=sizeof(t);

#define TRUE  1
#define FALSE 0
#define NULL  0

#define IN
#define OUT

#ifdef Q_OS_WIN
#define SELECTANY __declspec(selectany)
#else
#define SELECTANY
#endif

#define SAFE_DELETE(p)       if(p){ delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){ delete [] p; p = NULL;}
#define SAFE_FREE(p)         if(p){ free(p); p = NULL;}
#define SAFE_CLOSEHANDLE(p)  if(p){ CloseHandle(p); p = NULL;}

//拷贝内存并移动源数据指针
#define MEMCPY_OFFSETSRC(dest,src,size) {memcpy(dest, src, size); src += size;}

//拷贝内存并移动目标数据指针
#define MEMCPY_OFFSETDEST(dest,src,size) {memcpy(dest, src, size); dest += size;}

//拷贝内存并移动源数据指针和目标数据指针
#define MEMCPY_OFFSETBOTH(dest,src,size) {memcpy(dest, src, size); src += size; dest += size;}

//将符号转为字符串
#define GETSTR(s) #s

#define STRING_LEN_32    32
#define STRING_LEN_64    64
#define STRING_LEN_128   128
#define STRING_LEN_256   256
#define STRING_LEN_512   512
#define STRING_LEN_1024  1024

#ifdef Q_OS_WIN
#define STDCALL __stdcall
#else
#define STDCALL
#define CALLBACK
#endif

#define STRUCT_INIT(T)\
T()\
{\
    memset(this, 0, sizeof(T));\
}\
T(const T& obj)\
{\
    if(this != &obj)\
        memcpy(this, &obj, sizeof(T));\
}\
T& operator = (const T& obj)\
{\
    if(this != &obj)\
        memcpy(this, &obj, sizeof(T));\
    return *this;\
}

#define STRUCT_CONSTRUCT_DEFAULT(T)\
T()\
{\
    memset(this, 0, sizeof(T));\
}

#define STRUCT_CONSTRUCT_COPY(T)\
T(const T& obj)\
{\
    if(this != &obj)\
        memcpy(this, &obj, sizeof(T));\
}

#define STRUCT_OPERATOR_ASSIGN(T)\
T& operator = (const T& obj)\
{\
    if(this != &obj)\
        memcpy(this, &obj, sizeof(T));\
    return *this;\
}

#define ZXPROPERTY(NAME, TYPE, VARIATE)\
    public:                   \
        TYPE Get##NAME()      \
        {					  \
            return VARIATE;   \
        }					  \
        void Set##NAME(TYPE var)\
        {					  \
            VARIATE = var;    \
        }

#define ZXPROPERTY_GET(NAME, TYPE, VARIATE)\
    public:                   \
        TYPE Get##NAME()      \
        {					  \
            return VARIATE;   \
        }

#define ZXPROPERTY_SET(NAME, TYPE, VARIATE)\
    public:                     \
        void Set##NAME(TYPE var)\
        {					    \
            VARIATE = var;      \
        }


#define ZXPROPERTY_REF(NAME, TYPE, VARIATE)\
    public:                   \
        TYPE& Get##NAME()     \
        {					  \
            return VARIATE;   \
        }					  \
        void Set##NAME(const TYPE& var)\
        {					  \
            VARIATE = var;    \
        }

#define ZXPROPERTY_REF_GET(NAME, TYPE, VARIATE)\
    public:                   \
        TYPE& Get##NAME()     \
        {					  \
            return VARIATE;   \
        }

#define ZXPROPERTY_REF_SET(NAME, TYPE, VARIATE)\
    public:                   \
        void Set##NAME(const TYPE& var)\
        {					  \
            VARIATE = var;    \
        }

#endif