
#include "CCharsetEncoderICU.h"


HashMap<String, ArrayOf<Byte>* >* CCharsetEncoderICU::DEFAULT_REPLACEMENTS;

const Int32 CCharsetEncoderICU::INPUT_OFFSET;
const Int32 CCharsetEncoderICU::OUTPUT_OFFSET;
const Int32 CCharsetEncoderICU::INVALID_CHARS;
const Int32 CCharsetEncoderICU::INPUT_HELD;

CCharsetEncoderICU::CCharsetEncoderICU()
    : mData(ArrayOf<Int32>::Alloc(4))
    , mConverterHandle(0)
    , mInput(NULL)
    , mOutput(NULL)
    , mAllocatedInput(NULL)
    , mAllocatedOutput(NULL)
{
    if (DEFAULT_REPLACEMENTS == NULL)
    {
        DEFAULT_REPLACEMENTS = new HashMap<String, ArrayOf<Byte>* >();
    }
}

CCharsetEncoderICU::~CCharsetEncoderICU()
{
    ArrayOf<Int32>::Free(mData);
}

ECode CCharsetEncoderICU::constructor(
        /* [in] */ ICharset* charset,
        /* [in] */ Float averageBytesPerChar,
        /* [in] */ Float maxBytesPerChar,
        /* [in] */ const ArrayOf<Byte>& replacement,
        /* [in] */ Int64 address)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::NewInstance(
        /* [in] */ ICharset* charset,
        /* [in] */ const String& icuCanonicalName,
        /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::AverageBytesPerChar(
    /* [out] */ Float* averageNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::CanEncode(
    /* [in] */ Char8 c,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::CanEncodeEx(
    /* [in] */ ICharSequence* sequence,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::CanEncodeEx1(
        /* [in] */ Int32 codepoint,
        /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::Charset(
    /* [out] */ ICharset** charset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::Encode(
    /* [in] */ ICharBuffer* charBuffer,
    /* [out] */ IByteBuffer** byteBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::EncodeEx(
    /* [in] */ ICharBuffer* charBuffer,
    /* [out] */ IByteBuffer** byteBuffer,
    /* [in] */ Boolean endOfInput,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::Flush(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::IsLegalReplacement(
    /* [in] */ const ArrayOf<Byte>& replacement,
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::MalformedInputAction(
    /* [out] */ ICodingErrorAction** errorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::MaxBytesPerChar(
    /* [out] */ Float* maxNumber)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::OnMalformedInput(
    /* [in] */ ICodingErrorAction* newAction,
    /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::OnUnmappableCharacter(
    /* [in] */ ICodingErrorAction* newAction,
    /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::Replacement(
    /* [out, callee] */ ArrayOf<Byte>** replace)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::ReplaceWith(
    /* [in] */ const ArrayOf<Byte>& replacement,
    /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::Reset(
    /* [out] */ ICharsetEncoder** encoder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::UnmappableCharacterAction(
    /* [out] */ ICodingErrorAction** errorAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::ImplReplaceWith(
        /* [in] */ const ArrayOf<Byte>& newReplacement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::ImplFlush(
        /* [out] */ IByteBuffer** out,
        /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::ImplReset()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::EncodeLoop(
        /* [in] */ ICharBuffer* charBuffer,
        /* [in, out] */ IByteBuffer* byteBuffer,
        /* [out] */ ICoderResult** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ArrayOf<Byte>* CCharsetEncoderICU::MakeReplacement(
        /* [in] */ String icuCanonicalName,
        /* [in] */ Int64 address)
{
    // TODO: Add your code here
    return NULL;
}

ECode CCharsetEncoderICU::UpdateCallback()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

const Int32 CCharsetEncoderICU::GetArray(
        /* [in] */ IByteBuffer* byteBuffer)
{
    // TODO: Add your code here
    return 0;
}

const Int32 CCharsetEncoderICU::GetArray(
        /* [in] */ ICharBuffer* charBuffer)
{
    // TODO: Add your code here
    return 0;
}

ECode CCharsetEncoderICU::SetPosition(
        /* [in] */ IByteBuffer* byteBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetEncoderICU::SetPosition(
        /* [in] */ ICharBuffer* charBuffer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}