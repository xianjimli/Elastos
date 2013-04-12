
#include "CharsetEncoder.h"

const Int32 CharsetEncoder::INIT;
const Int32 CharsetEncoder::ONGOING;
const Int32 CharsetEncoder::END;
const Int32 CharsetEncoder::FLUSH;

CharsetEncoder::CharsetEncoder(
    /* [in] */ ICharset* cs,
    /* [in] */ Float averageBytesPerChar,
    /* [in] */ Float maxBytesPerChar)
{
    // E_NOT_IMPLEMENTED
}

CharsetEncoder::CharsetEncoder(
    /* [in] */ ICharset* cs,
    /* [in] */ Float averageBytesPerChar,
    /* [in] */ Float maxBytesPerChar,
    /* [in] */ const ArrayOf<Byte>& replacement)
{
    // E_NOT_IMPLEMENTED
}

ECode CharsetEncoder::AverageBytesPerChar(
    /* [out] */ Float* averageNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::CanEncode(
    /* [in] */ Char8 c,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::CanEncodeEx(
    /* [in] */ ICharSequence* sequence,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::Charset(
    /* [out] */ ICharset** charset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::Encode(
    /* [in] */ ICharBuffer* charBuffer,
    /* [out] */ IByteBuffer** byteBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::EncodeEx(
    /* [in] */ ICharBuffer* charBuffer,
    /* [out] */ IByteBuffer** byteBuffer,
    /* [in] */ Boolean endOfInput,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::Flush(
    /* [out] */ IByteBuffer** byteBuffer,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::IsLegalReplacement(
    /* [in] */ const ArrayOf<Byte>& replacement,
    /* [out] */ Boolean* pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::MalformedInputAction(
    /* [out] */ ICodingErrorAction** errorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::MaxBytesPerChar(
    /* [out] */ Float* maxNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::OnMalformedInput(
    /* [in] */ ICodingErrorAction* newAction,
    /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::OnUnmappableCharacter(
    /* [in] */ ICodingErrorAction* newAction,
    /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::Replacement(
    /* [out, callee] */ ArrayOf<Byte>** replace)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::ReplaceWith(
    /* [in] */ const ArrayOf<Byte>& replacement,
    /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::Reset(
    /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::UnmappableCharacterAction(
    /* [out] */ ICodingErrorAction** errorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::ImplCanEncode(
    /* [in] */ ICharBuffer* cb,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::CheckCoderResult(
    /* [in] */ ICoderResult* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::AllocateMore(
    /* [in] */ IByteBuffer* output,
    /* [out] */ IByteBuffer** byteBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::ImplFlush(
    /* [in] */ IByteBuffer* outInput,
    /* [out] */  ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
ECode CharsetEncoder::ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::ImplReplaceWith(
        /* [in] */ ArrayOf<Byte>& newReplacement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::ImplReset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetEncoder::UncheckedReplaceWith(
        /* [in] */ ArrayOf<Byte>& replacement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
