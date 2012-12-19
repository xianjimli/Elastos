
#ifndef __CHARARRAYBUFFER_H__
#define __CHARARRAYBUFFER_H__

#include "ext/frameworkext.h"

/**
 * This is used for {@link Cursor#copyStringToBuffer}
 */
class CharArrayBuffer
{
public:
    ~CharArrayBuffer();

    CARAPI GetData(
        /* [out,callee] */ ArrayOf<Char32>** data);

    CARAPI SetData(
        /* [in] */ ArrayOf<Char32>* data);

protected:
    CharArrayBuffer();

    CARAPI Init(
        /* [in] */ Int32 size);

    CARAPI Init(
        /* [in] */ ArrayOf<Char32>* buf);

public:
    ArrayOf<Char32>* mData;
    Int32 mSizeCopied;
};

#endif //_CHARARRAYBUFFER_H_
