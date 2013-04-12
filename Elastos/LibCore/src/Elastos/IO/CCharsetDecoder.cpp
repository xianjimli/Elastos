
#include "CCharsetDecoder.h"


ECode CCharsetDecoder::constructor(
    /* [in] */ ICharset* pCharset,
    /* [in] */ Float averageCharsPerByte,
    /* [in] */ Float maxCharsPerByte)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::AverageCharsPerByte(
    /* [out] */ Float* pAverage)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::Charset(
    /* [out] */ ICharset** ppCharset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::Decode(
    /* [in] */ IByteBuffer* pByteBuffer,
    /* [out] */ ICharBuffer** ppCharBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::DecodeEx(
    /* [in] */ IByteBuffer* pByteBuffer,
    /* [out] */ ICharBuffer** ppCharBuffer,
    /* [in] */ Boolean endOfInput,
    /* [out] */ ICoderResult** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::DecodeLoop(
    /* [in] */ IByteBuffer* pByteBuffer,
    /* [out] */ ICharBuffer** ppCharBuffer,
    /* [out] */ ICoderResult** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::DetectedCharset(
    /* [out] */ ICharset** ppCharset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::Flush(
    /* [out] */ ICharBuffer** ppCharBuffer,
    /* [out] */ ICoderResult** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::ImplFlush(
    /* [out] */ ICharBuffer** ppCharBuffer,
    /* [out] */ ICoderResult** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::ImplOnMalformedInput(
    /* [in] */ ICodingErrorAction* pNewAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::ImplOnUnmappableCharacter(
    /* [in] */ ICodingErrorAction* pNewAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::ImplReplaceWith(
    /* [in] */ const String& newReplacement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::ImplReset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::IsAutoDetecting(
    /* [out] */ Boolean* pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::IsCharsetDetected(
    /* [out] */ Boolean* pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::MalformedInputAction(
    /* [out] */ ICodingErrorAction** ppCodingErrorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::MaxCharsPerByte(
    /* [out] */ Float* pMaxNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::OnMalformedInput(
    /* [in] */ ICodingErrorAction* pNewAction,
    /* [out] */ ICharsetDecoder** ppCharsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::OnUnmappableCharacter(
    /* [in] */ ICodingErrorAction* pNewAction,
    /* [out] */ ICharsetDecoder** ppCharsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::Replacement(
    /* [out] */ String* pReplacement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::ReplaceWith(
    /* [in] */ const String& newReplacement,
    /* [out] */ ICharsetDecoder** ppCharsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::Reset(
    /* [out] */ ICharsetDecoder** ppCharsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetDecoder::UnmappableCharacterAction(
    /* [out] */ ICodingErrorAction** ppCodingErrorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
