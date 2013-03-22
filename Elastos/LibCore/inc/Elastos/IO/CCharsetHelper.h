#ifndef __CCHARSETHELPER_H__
#define __CCHARSETHELPER_H__

#include "_CCharsetHelper.h"

CarClass(CCharsetHelper)
{
public:
    CARAPI DefaultCharset(
        /* [out] */ICharset** charset);
};

#endif //__CCHARSETHELPER_H__