
#include "CharsetDecoder.h"
#include <cmdef.h>

const Int32 CharsetDecoder::INIT;
const Int32 CharsetDecoder::ONGOING;
const Int32 CharsetDecoder::END;
const Int32 CharsetDecoder::FLUSH;

CharsetDecoder::CharsetDecoder(
    /* [in] */ ICharset* charset,
    /* [in] */ Float averageCharsPerByte,
    /* [in] */ Float maxCharsPerByte)
{
    // E_NOT_IMPLEMENTED
}

ECode CharsetDecoder::AverageCharsPerByte(
    /* [out] */ Float* average)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Charset(
    /* [out] */ ICharset** charset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Decode(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ ICharBuffer** charBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::DecodeEx(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ ICharBuffer** charBuffer,
    /* [in] */ Boolean endOfInput,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::DecodeLoop(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ ICharBuffer** charBuffer,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::DetectedCharset(
    /* [out] */ ICharset** charset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Flush(
    /* [out] */ ICharBuffer** charBuffer,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::IsAutoDetecting(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::IsCharsetDetected(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::MalformedInputAction(
    /* [out] */ ICodingErrorAction** codingErrorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::MaxCharsPerByte(
    /* [out] */ Float* maxNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::OnMalformedInput(
    /* [in] */ ICodingErrorAction* newAction,
    /* [out] */ ICharsetDecoder** decoder)
{
    VALIDATE_NOT_NULL(newAction);
    mMalformAction = newAction;
    ECode ec = ImplOnMalformedInput(newAction);
    if (SUCCEEDED(ec)) {
        *decoder = (ICharsetDecoder*)this;
    }
    return ec;
}

ECode CharsetDecoder::OnUnmappableCharacter(
    /* [in] */ ICodingErrorAction* newAction,
    /* [out] */ ICharsetDecoder** decoder)
{
    VALIDATE_NOT_NULL(newAction)
    mUnmapAction = newAction;
    ECode ec = ImplOnUnmappableCharacter(newAction);
    if (SUCCEEDED(ec)) {
        *decoder = (ICharsetDecoder*)this;
    }
    return ec;
}

ECode CharsetDecoder::Replacement(
    /* [out] */ String* replacement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::ReplaceWith(
    /* [in] */ const String& newReplacement,
    /* [out] */ ICharsetDecoder** charsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::Reset(
    /* [out] */ ICharsetDecoder** charsetDecoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CharsetDecoder::UnmappableCharacterAction(
    /* [out] */ ICodingErrorAction** codingErrorAction)
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
    // default implementation is empty
    return NOERROR;
}

ECode CharsetDecoder::ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction)
{
    // default implementation is empty
    return NOERROR;
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