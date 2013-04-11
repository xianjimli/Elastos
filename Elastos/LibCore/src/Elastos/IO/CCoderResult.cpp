
#include "CCoderResult.h"

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

ECode CCoderResult::MalformedForLength(
    /* [in] */ Int32 length,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::UnmappableForLength(
    /* [in] */ Int32 length,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::IsUnderflow(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::IsError(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::IsMalformed(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::IsOverflow(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::IsUnmappable(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::Length(
    /* [out] */ Int32* length)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::ThrowException()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCoderResult::ToString(
    /* [out] */ String* toString)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

