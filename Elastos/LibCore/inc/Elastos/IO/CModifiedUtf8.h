
#ifndef __CMODIFIEDUTF8_H__
#define __CMODIFIEDUTF8_H__

#include "_CModifiedUtf8.h"

CarClass(CModifiedUtf8)
{
public:
    CARAPI Decode(
        /* [in] */ const ArrayOf<Byte>& bytes,
        /* [out, callee] */ ArrayOf<Char8>** outChars,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 utfSize,
        /* [out] */ String* string);
};

#endif // __CMODIFIEDUTF8_H__
