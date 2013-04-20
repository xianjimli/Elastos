
#ifndef __CCHAR32_H__
#define __CCHAR32_H__


#include "_CChar32.h"

CarClass(CChar32)
{
public:
    CARAPI constructor(
        /* [in] */ Char32 value);

    CARAPI GetValue(
        /* [out] */ Char32* value);

private:
    Char32 mValue;
};


#endif //__CCHAR32_H__
