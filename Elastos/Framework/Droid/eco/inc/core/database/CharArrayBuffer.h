
#ifndef __CHARARRAYBUFFER_H__
#define __CHARARRAYBUFFER_H__

#include "ext/frameworkext.h"

class CharArrayBuffer
{
public:
    CharArrayBuffer();

    ~CharArrayBuffer();

    CARAPI Init(
        /* [in] */ Int32 size);

    CARAPI Init(
        /* [in] */ const ArrayOf<Char8>& buf);

    CARAPI GetData(
        /* [out,callee] */ ArrayOf<Char8>** data);

    CARAPI SetData(
        /* [in] */ const ArrayOf<Char8>& data);

public:
    ArrayOf<Char8>* data;

    Int32 sizeCopied;
};
#endif //_CHARARRAYBUFFER_H_