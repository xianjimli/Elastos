
#ifndef __CCODERRESULT_H__
#define __CCODERRESULT_H__

#include "_CCoderResult.h"
#include <elastos/AutoPtr.h>

CarClass(CCoderResult)
{
public:
    CARAPI MalformedForLength(
        /* [in] */ Int32 length,
        /* [out] */ ICoderResult** result);

    CARAPI UnmappableForLength(
        /* [in] */ Int32 length,
        /* [out] */ ICoderResult** result);

    CARAPI IsUnderflow(
        /* [out] */ Boolean* result);

    CARAPI IsError(
        /* [out] */ Boolean* result);

    CARAPI IsMalformed(
        /* [out] */ Boolean* result);

    CARAPI IsOverflow(
        /* [out] */ Boolean* result);

    CARAPI IsUnmappable(
        /* [out] */ Boolean* result);

    CARAPI Length(
        /* [out] */ Int32* length);

    CARAPI ThrowException();

    CARAPI ToString(
        /* [out] */ String* toString);

private:
    friend AutoPtr<ICoderResult> CreateCoderResult(
        /* [in] */ Int32 type,
        /* [in] */ Int32 length);

public:
    static const AutoPtr<ICoderResult> UNDERFLOW;

    static const AutoPtr<ICoderResult> OVERFLOW;

private:
    static const Int32 TYPE_UNDERFLOW = 1;

    static const Int32 TYPE_OVERFLOW = 2;

    static const Int32 TYPE_MALFORMED_INPUT = 3;

    static const Int32 TYPE_UNMAPPABLE_CHAR = 4;

    // TODO:
    // static WeakHashMap<Integer, CoderResult> _malformedErrors = new WeakHashMap<Integer, CoderResult>();

    // static WeakHashMap<Integer, CoderResult> _unmappableErrors = new WeakHashMap<Integer, CoderResult>();

    Int32 mType;

    Int32 mLength;
};

#endif // __CCODERRESULT_H__
