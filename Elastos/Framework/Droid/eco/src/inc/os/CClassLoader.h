
#ifndef __CCLASSLOADER_H__
#define __CCLASSLOADER_H__

#include "_CClassLoader.h"

CarClass(CClassLoader)
{
public:
    CARAPI LoadClass(
        /* [in] */ String className,
        /* [out] */ Handle32* _clazz);
};

#endif //__CCLASSLOADER_H__