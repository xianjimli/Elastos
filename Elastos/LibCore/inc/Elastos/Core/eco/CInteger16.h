
#ifndef __CINTEGER16_H__
#define __CINTEGER16_H__

#include "_CInteger16.h"

CarClass(CInteger16)
{
public:
    CARAPI constructor(
        /* [in] */ Int16 value);

    CARAPI GetValue(
        /* [out] */ Int16* value);

private:
    Int16 mValue;
};

#endif //__CINTEGER16_H__
