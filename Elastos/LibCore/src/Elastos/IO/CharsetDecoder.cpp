
#include "CharsetDecoder.h"

const Int32 CharsetDecoder::INIT;
const Int32 CharsetDecoder::ONGOING;
const Int32 CharsetDecoder::END;
const Int32 CharsetDecoder::FLUSH;

CharsetDecoder::CharsetDecoder(
    /* [in] */ ICharset* pCharset,
    /* [in] */ Float averageCharsPerByte,
    /* [in] */ Float maxCharsPerByte)
{
    // E_NOT_IMPLEMENTED
}

ECode CharsetDecoder::AverageCharsPerByte(
    /* [out] */ Float* pAverage)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Charset(
    /* [out] */ ICharset** ppCharset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Decode(
    /* [in] */ IByteBuffer* pByteBuffer,
    /* [out] */ ICharBuffer** ppCharBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::DecodeEx(
    /* [in] */ IByteBuffer* pByteBuffer,
    /* [out] */ ICharBuffer** ppCharBuffer,
    /* [in] */ Boolean endOfInput,
    /* [out] */ ICoderResult** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::DecodeLoop(
    /* [in] */ IByteBuffer* pByteBuffer,
    /* [out] */ ICharBuffer** ppCharBuffer,
    /* [out] */ ICoderResult** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::DetectedCharset(
    /* [out] */ ICharset** ppCharset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Flush(
    /* [out] */ ICharBuffer** ppCharBuffer,
    /* [out] */ ICoderResult** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::IsAutoDetecting(
    /* [out] */ Boolean* pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::IsCharsetDetected(
    /* [out] */ Boolean* pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::MalformedInputAction(
    /* [out] */ ICodingErrorAction** ppCodingErrorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::MaxCharsPerByte(
    /* [out] */ Float* pMaxNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::OnMalformedInput(
    /* [in] */ ICodingErrorAction* pNewAction,
    /* [out] */ ICharsetDecoder** ppCharsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::OnUnmappableCharacter(
    /* [in] */ ICodingErrorAction* pNewAction,
    /* [out] */ ICharsetDecoder** ppCharsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Replacement(
    /* [out] */ String* pReplacement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::ReplaceWith(
    /* [in] */ const String& newReplacement,
    /* [out] */ ICharsetDecoder** ppCharsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Reset(
    /* [out] */ ICharsetDecoder** ppCharsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::UnmappableCharacterAction(
    /* [out] */ ICodingErrorAction** ppCodingErrorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::CheckCoderResult(
        /* [in] */ ICoderResult* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::AllocateMore(
        /* [in] */ ICharBuffer* output)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::ImplFlush(
        /* [in] */ ICharBuffer* outInput,
        /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::ImplReplaceWith(
        /* [in] */ String newReplacement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::ImplReset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}