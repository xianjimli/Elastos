
#ifndef __CCHARARRAYBUFFER_H__
#define __CCHARARRAYBUFFER_H__

#include "_CCharArrayBuffer.h"
#include "database/CharArrayBuffer.h"
CarClass(CCharArrayBuffer), public CharArrayBuffer
{
public:
    CARAPI GetData(
        /* [out, callee] */ ArrayOf<Char8> ** ppData);

    CARAPI SetData(
        /* [in] */ const ArrayOf<Char8> & data);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 size);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Char8> & buf);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCHARARRAYBUFFER_H__
