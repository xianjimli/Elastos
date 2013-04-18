
#include "CCoderResult.h"
#include "cmdef.h"
#include <elastos/Mutex.h>

AutoPtr<ICoderResult> CreateCoderResult(
    /* [in] */ Int32 type,
    /* [in] */ Int32 length)
{
    AutoPtr<CCoderResult> p;
    CCoderResult::NewByFriend((CCoderResult**)&p);
    p->mType = type;
    p->mLength = length;

    return (ICoderResult*)p.Get();
}

const AutoPtr<ICoderResult> CCoderResult::UNDERFLOW = CreateCoderResult(TYPE_UNDERFLOW, 0);
const AutoPtr<ICoderResult> CCoderResult::OVERFLOW = CreateCoderResult(TYPE_OVERFLOW, 0);
Mutex CCoderResult::mMalformedMutex;
Mutex CCoderResult::mUnmappableMutex;

ECode CCoderResult::GetUNDERFLOW(
    /* [out] */ ICoderResult** result){
    VALIDATE_NOT_NULL(result);
    *result = UNDERFLOW.Get();
    return NOERROR;
}

ECode CCoderResult::GetOVERFLOW(
    /* [out] */ ICoderResult** result){
    VALIDATE_NOT_NULL(result);
    *result = OVERFLOW.Get();
    return NOERROR;
}

ECode CCoderResult::MalformedForLength(
    /* [in] */ Int32 length,
    /* [out] */ ICoderResult** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mMalformedMutex);

    if (length > 0) {
    // TODO:
    //     Integer key = Integer.valueOf(length);
    //     synchronized (_malformedErrors) {
    //         CoderResult r = _malformedErrors.get(key);
    //         if (null == r) {
    //             r = new CoderResult(TYPE_MALFORMED_INPUT, length);
    //             _malformedErrors.put(key, r);
    //         }
    //         return r;
    //     }
    }
    // throw new IllegalArgumentException("Length must be greater than 0; was " + length);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CCoderResult::UnmappableForLength(
    /* [in] */ Int32 length,
    /* [out] */ ICoderResult** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mUnmappableMutex);

    if (length > 0) {
    // TODO:
    //     Integer key = Integer.valueOf(length);
    //     synchronized (_unmappableErrors) {
    //         CoderResult r = _unmappableErrors.get(key);
    //         if (null == r) {
    //             r = new CoderResult(TYPE_UNMAPPABLE_CHAR, length);
    //             _unmappableErrors.put(key, r);
    //         }
    //         return r;
    //     }
    }
    // throw new IllegalArgumentException("Length must be greater than 0; was " + length);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CCoderResult::IsUnderflow(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType == TYPE_UNDERFLOW ? TRUE : FALSE;
    return NOERROR;
}

ECode CCoderResult::IsError(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (mType == TYPE_MALFORMED_INPUT ? TRUE : FALSE)
        || (mType == TYPE_UNMAPPABLE_CHAR ? TRUE : FALSE);
    return NOERROR;
}

ECode CCoderResult::IsMalformed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType == TYPE_MALFORMED_INPUT ? TRUE : FALSE;
    return NOERROR;
}

ECode CCoderResult::IsOverflow(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType == TYPE_OVERFLOW ? TRUE : FALSE;
    return NOERROR;
}

ECode CCoderResult::IsUnmappable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType == TYPE_UNMAPPABLE_CHAR ? TRUE : FALSE;
    return NOERROR;
}

ECode CCoderResult::Length(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);

    if (mType == TYPE_MALFORMED_INPUT || mType == TYPE_UNMAPPABLE_CHAR) {
        *length = mLength;
        return NOERROR;
    }
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCoderResult::ThrowException()
{
    switch (mType) {
        case TYPE_UNDERFLOW:
            return E_BUFFER_UNDER_FLOW_EXCEPTION;
        case TYPE_OVERFLOW:
            return E_BUFFER_OVER_FLOW_EXCEPTION;
        case TYPE_UNMAPPABLE_CHAR:
            return E_UNMAPPABLE_CHARACTER_EXCEPTION;
        case TYPE_MALFORMED_INPUT:
            return E_MALFORMED_INPUT_EXCEPTION;
    }

    return E_CHARACTER_CODING_EXCEPTION;
}

ECode CCoderResult::ToString(
    /* [out] */ String* toString)
{
    VALIDATE_NOT_NULL(toString);

    String dsc;
    switch (mType) {
        case TYPE_UNDERFLOW: {
            dsc = String("UNDERFLOW error");
            break;
        }
        case TYPE_OVERFLOW: {
            dsc = String("OVERFLOW error");
            break;
        }
        case TYPE_UNMAPPABLE_CHAR: {
            dsc = String("Unmappable-character error with erroneous input length ");
            dsc.Append(String::FromInt32(mLength));
            break;
        }
        case TYPE_MALFORMED_INPUT: {
            dsc = String("Malformed-input error with erroneous input length ");
            dsc.Append(String::FromInt32(mLength));
            break;
        }
        default: {
            dsc = String("");            
            break;
        }
    }

    *toString = __FILE__;
    toString->Append(__FUNCTION__);
    toString->Append("[");
    toString->Append(dsc);
    toString->Append("]");

    return NOERROR;
}
