
#include "CharsetEncoder.h"
#include <cmdef.h>
#include <elastos/AutoFree.h>
#include <elastos/AutoPtr.h>
#include "CharBuffer.h"
#include "ByteBuffer.h"

const Int32 CharsetEncoder::INIT;
const Int32 CharsetEncoder::ONGOING;
const Int32 CharsetEncoder::END;
const Int32 CharsetEncoder::FLUSH;

CharsetEncoder::CharsetEncoder()
{
}

ECode CharsetEncoder::Init(
    /* [in] */ ICharset* cs,
    /* [in] */ Float averageBytesPerChar,
    /* [in] */ Float maxBytesPerChar)
{
    ArrayOf<Byte>* replacement = ArrayOf<Byte>::Alloc(1);
    (*replacement)[0] = (Byte)'?';
    ECode res = Init(cs, averageBytesPerChar, maxBytesPerChar, *replacement);
    ArrayOf<Byte>::Free(replacement);

    return res;
}

ECode CharsetEncoder::Init(
    /* [in] */ ICharset* cs,
    /* [in] */ Float averageBytesPerChar,
    /* [in] */ Float maxBytesPerChar,
    /* [in] */ const ArrayOf<Byte>& replacement)
{
    if (averageBytesPerChar <= 0 || maxBytesPerChar <= 0) {
        // throw new IllegalArgumentException("averageBytesPerChar and maxBytesPerChar must both be positive");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (averageBytesPerChar > maxBytesPerChar) {
        // throw new IllegalArgumentException("averageBytesPerChar is greater than maxBytesPerChar");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mCs = cs;
    mAverBytes = averageBytesPerChar;
    mMaxBytes = maxBytesPerChar;
    mStatus = INIT;

    AutoPtr<ICodingErrorAction> action;
    CCodingErrorAction::New((ICodingErrorAction** )&action);
    assert(action != NULL);
    AutoPtr<ICodingErrorAction> report;
    FAIL_RETURN(action->GetREPORT((ICodingErrorAction **)&report));

    mMalformAction = report;
    mUnmapAction = report;

    // TODO:
    ECode ec = E_RUNTIME_EXCEPTION;
    // AutoPtr<IharsetEncoderICU> encoder = (IharsetEncoderICU*)this->Probe(EIID_IharsetEncoderICU);
    // if (encoder != NULL) {
    //     // The RI enforces unnecessary restrictions on the replacement bytes. We trust ICU to
    //     // know what it's doing. This lets us support EUC-JP, SCSU, and Shift_JIS.
    //     ec = UncheckedReplaceWith(replacement);
    // } else {
    //     ec = ReplaceWith(replacement);
    // }

    return ec;
}

CharsetEncoder::~CharsetEncoder()
{
    if (mReplace != NULL)
    {
        ArrayOf<Byte>::Free(mReplace);
    }
}

ECode CharsetEncoder::AverageBytesPerChar(
    /* [out] */ Float* averageNumber)
{
    VALIDATE_NOT_NULL(averageNumber);
    *averageNumber = mAverBytes;
    return NOERROR;
}

ECode CharsetEncoder::CanEncode(
    /* [in] */ Char8 c,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoFree< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(1);
    (*chars)[0] = c;
    AutoPtr<ICharBuffer> charBuffer;
    ECode ec = CharBuffer::Wrap(chars.Get(), (ICharBuffer**)&charBuffer);
    if (SUCCEEDED(ec))
    {
        ec = ImplCanEncode(charBuffer.Get(), result);
    }

    return ec;
}

ECode CharsetEncoder::CanEncodeEx(
    /* [in] */ ICharSequence* sequence,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(sequence);
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICharBuffer> cb;
    AutoPtr<ICharBuffer> charBuffer = (ICharBuffer*)sequence->Probe(EIID_ICharBuffer);
    if (charBuffer != NULL) {
        ((ICharBuffer*) sequence)->Duplicate((ICharBuffer**)&cb);
    } else {
        CharBuffer::WrapEx2(sequence, (ICharBuffer**)&cb);
    }

    return ImplCanEncode(cb.Get(), result);
}

ECode CharsetEncoder::Charset(
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = mCs;
    return NOERROR;
}

ECode CharsetEncoder::Encode(
    /* [in] */ ICharBuffer* charBuffer,
    /* [out] */ IByteBuffer** byteBuffer)
{
    VALIDATE_NOT_NULL(charBuffer);
    VALIDATE_NOT_NULL(byteBuffer);

    Int32 remaining;
    FAIL_RETURN(charBuffer->Remaining(&remaining));
    if (remaining == 0) {
        return ByteBuffer::Allocate(0, byteBuffer);
    }
    AutoPtr<ICharsetEncoder> encoder;
    FAIL_RETURN(Reset((ICharsetEncoder**)&encoder));

    FAIL_RETURN(charBuffer->Remaining(&remaining));
    Int32 length = (Int32) (remaining * mAverBytes);
    AutoPtr<IByteBuffer> output;
    FAIL_RETURN(ByteBuffer::Allocate(length, (IByteBuffer**)&output));
    AutoPtr<ICoderResult> result;
    AutoPtr<ICoderResultHelper> helper;
    AutoPtr<ICoderResult> UNDERFLOW;
    AutoPtr<ICoderResult> OVERFLOW;
    CCoderResultHelper::AcquireSingleton((ICoderResultHelper**)&helper);
    helper->GetUNDERFLOW((ICoderResult**)&UNDERFLOW);
    helper->GetOVERFLOW((ICoderResult**)&OVERFLOW);

    while (TRUE) {
        FAIL_RETURN(EncodeEx(charBuffer, output.Get(), FALSE, (ICoderResult**)&result));
        if (result.Get() == UNDERFLOW) {
            break;
        } else if (result.Get() == UNDERFLOW) {
            FAIL_RETURN(AllocateMore(output.Get(), (IByteBuffer**)&output));
            continue;
        }
        FAIL_RETURN(CheckCoderResult(result.Get()));
    }
    FAIL_RETURN(EncodeEx(charBuffer, output.Get(), TRUE, (ICoderResult**)&result));
    FAIL_RETURN(CheckCoderResult(result.Get()));

    while (TRUE) {
        FAIL_RETURN(Flush(output.Get(), (ICoderResult**)&result));
        if (result == UNDERFLOW) {
            FAIL_RETURN(output->Flip());
            break;
        } else if (result == OVERFLOW) {
            FAIL_RETURN(AllocateMore(output.Get(), (IByteBuffer**)&output));
            continue;
        }
        FAIL_RETURN(CheckCoderResult(result.Get()));
        FAIL_RETURN(output->Flip());
        Boolean res;
        FAIL_RETURN(result->IsMalformed(&res));
        if (res) {
            // throw new MalformedInputException(result.length());
            return E_MALFORMED_INPUT_EXCEPTION;
        }
        FAIL_RETURN(result->IsUnmappable(&res));
        if (res) {
            // throw new UnmappableCharacterException(result.length());
            return E_UNMAPPABLE_CHARACTER_EXCEPTION;
        }
        break;
    }
    mStatus = FLUSH;
    *byteBuffer = output;
    return NOERROR;
}

ECode CharsetEncoder::EncodeEx(
    /* [in] */ ICharBuffer* charBuffer,
    /* [in, out] */ IByteBuffer* byteBuffer,
    /* [in] */ Boolean endOfInput,
    /* [out] */ ICoderResult** result)
{
    VALIDATE_NOT_NULL(charBuffer);
    VALIDATE_NOT_NULL(byteBuffer);
    VALIDATE_NOT_NULL(result);

    if ((mStatus == FLUSH) || (!endOfInput && mStatus == END)) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<ICoderResultHelper> helper;
    AutoPtr<ICoderResult> UNDERFLOW;
    AutoPtr<ICoderResult> OVERFLOW;
    CCoderResultHelper::AcquireSingleton((ICoderResultHelper**)&helper);
    helper->GetUNDERFLOW((ICoderResult**)&UNDERFLOW);
    helper->GetOVERFLOW((ICoderResult**)&OVERFLOW);

    AutoPtr<ICoderResult> res;
    while (TRUE) {
        FAIL_RETURN(EncodeLoop(charBuffer, byteBuffer, (ICoderResult**)&res));
        if (res == UNDERFLOW) {
            mStatus = endOfInput ? END : ONGOING;
            if (endOfInput) {
                Int32 remaining;
                charBuffer->Remaining(&remaining);
                if (remaining > 0) {
                    FAIL_RETURN(helper->MalformedForLength(remaining, (ICoderResult**)&res));
                } else {
                    *result = res;
                    return NOERROR;
                }
            } else {
                *result = res;
                return NOERROR;
            }
        } else if (res == OVERFLOW) {
            mStatus = endOfInput ? END : ONGOING;
            *result = res;
            return NOERROR;
        }
        ICodingErrorAction* action = mMalformAction;
        Boolean enable;
        FAIL_RETURN(res->IsUnmappable(&enable));
        if (enable) {
            action = mUnmapAction;
        }
        // If the action is IGNORE or REPLACE, we should continue
        // encoding.

        AutoPtr<ICodingErrorAction> helper;
        CCodingErrorAction::New((ICodingErrorAction** )&helper);
        AutoPtr<ICodingErrorAction> REPLACE;
        AutoPtr<ICodingErrorAction> IGNORE;
        FAIL_RETURN(helper->GetREPLACE((ICodingErrorAction **)&REPLACE));
        FAIL_RETURN(helper->GetIGNORE((ICodingErrorAction **)&IGNORE));

        if (action == REPLACE) {
            Int32 remaining;
            FAIL_RETURN(byteBuffer->Remaining(&remaining));
            if (remaining < mReplace->GetLength()) {
                *result = OVERFLOW;
                return NOERROR;
            }
            byteBuffer->PutBytes(*mReplace);
        } else {
            if (action != IGNORE) {
                *result = res;
                return NOERROR;
            }
        }
        Int32 pos;
        FAIL_RETURN(charBuffer->GetPosition(&pos));
        Int32 len;
        res->Length(&len);
        FAIL_RETURN(charBuffer->SetPosition(pos + len));
    }

    return E_RUNTIME_EXCEPTION;
}

ECode CharsetEncoder::Flush(
    /* [in, out] */ IByteBuffer* byteBuffer,
    /* [out] */ ICoderResult** result)
{
    VALIDATE_NOT_NULL(byteBuffer);
    VALIDATE_NOT_NULL(result);

    if (mStatus != END && mStatus != INIT) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    ICoderResult* res;
    FAIL_RETURN(ImplFlush(byteBuffer, &res));

    AutoPtr<ICoderResultHelper> helper;
    AutoPtr<ICoderResult> UNDERFLOW;
    CCoderResultHelper::AcquireSingleton((ICoderResultHelper**)&helper);
    helper->GetUNDERFLOW((ICoderResult**)&UNDERFLOW);

    if (res == UNDERFLOW) {
        mStatus = FLUSH;
    }

    *result = res;
    return NOERROR;
}

ECode CharsetEncoder::IsLegalReplacement(
    /* [in] */ const ArrayOf<Byte>& replacement,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mDecoder == NULL) {
        mCs->NewDecoder((ICharsetDecoder**)&mDecoder);

        AutoPtr<ICodingErrorAction> helper;
        CCodingErrorAction::New((ICodingErrorAction** )&helper);
        AutoPtr<ICodingErrorAction> REPORT;
        AutoPtr<ICharsetDecoder> decoder;
        FAIL_RETURN(helper->GetREPORT((ICodingErrorAction **)&REPORT));

        mDecoder->OnMalformedInput(REPORT.Get(), (ICharsetDecoder**)&decoder);
        mDecoder->OnUnmappableCharacter(REPORT.Get(), (ICharsetDecoder**)&decoder);
    }
    AutoPtr<IByteBuffer> in;
    ArrayOf<Byte>* bytes = replacement.Clone();
    FAIL_RETURN(ByteBuffer::Wrap(bytes, (IByteBuffer**)&in));
    ArrayOf<Byte>::Free(bytes);
    AutoPtr<ICharBuffer> out;
    Float maxNum;
    mDecoder->MaxCharsPerByte(&maxNum);
    FAIL_RETURN(CharBuffer::Allocate((Int32) (replacement.GetLength() * maxNum), (ICharBuffer**)&out));
    AutoPtr<ICoderResult> res;
    FAIL_RETURN(mDecoder->DecodeEx(in.Get(), (ICharBuffer**)&out, TRUE, (ICoderResult**)&res));

    Boolean err;
    res->IsError(&err);
    *result = !err;

    return NOERROR;
}

ECode CharsetEncoder::MalformedInputAction(
    /* [out] */ ICodingErrorAction** errorAction)
{
    VALIDATE_NOT_NULL(errorAction);

    *errorAction = mMalformAction.Get();
    return NOERROR;
}

ECode CharsetEncoder::MaxBytesPerChar(
    /* [out] */ Float* maxNumber)
{
    VALIDATE_NOT_NULL(maxNumber);

    *maxNumber = mMaxBytes;
    return NOERROR;
}

ECode CharsetEncoder::OnMalformedInput(
    /* [in] */ ICodingErrorAction* newAction,
    /* [out] */ ICharsetEncoder** encoder)
{
    VALIDATE_NOT_NULL(newAction);

    mMalformAction = newAction;
    ECode ec = ImplOnMalformedInput(newAction);
    if (SUCCEEDED(ec)) {
        *encoder = (ICharsetEncoder*)this;
    }
    return ec;
}

ECode CharsetEncoder::OnUnmappableCharacter(
    /* [in] */ ICodingErrorAction* newAction,
    /* [out] */ ICharsetEncoder** encoder)
{
    VALIDATE_NOT_NULL(newAction);

    mUnmapAction = newAction;
    ECode ec = ImplOnUnmappableCharacter(newAction);
    if (SUCCEEDED(ec)) {
        *encoder = (ICharsetEncoder*)this;
    }
    return ec;
}

ECode CharsetEncoder::Replacement(
    /* [out, callee] */ ArrayOf<Byte>** replace)
{
    VALIDATE_NOT_NULL(replace);

    *replace = mReplace->Clone();
    return NOERROR;
}

ECode CharsetEncoder::ReplaceWith(
    /* [in] */ const ArrayOf<Byte>& replacement,
    /* [out] */ ICharsetEncoder** encoder)
{
    VALIDATE_NOT_NULL(encoder);

    if (replacement.GetLength() == 0 || mMaxBytes < replacement.GetLength()) {
        // throw new IllegalArgumentException("bad replacement length: " + replacement.length);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean result;
    FAIL_RETURN(IsLegalReplacement(replacement, &result));
    if (!result) {
        // throw new IllegalArgumentException("bad replacement: " + Arrays.toString(replacement));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(UncheckedReplaceWith(replacement));

    *encoder = (ICharsetEncoder*)this;
    return NOERROR;
}

ECode CharsetEncoder::Reset(
    /* [out] */ ICharsetEncoder** encoder)
{
    VALIDATE_NOT_NULL(encoder);

    mStatus = INIT;
    FAIL_RETURN(ImplReset());
    *encoder = (ICharsetEncoder*)this;
    return NOERROR;
}

ECode CharsetEncoder::UnmappableCharacterAction(
    /* [out] */ ICodingErrorAction** errorAction)
{
    VALIDATE_NOT_NULL(errorAction);

    *errorAction = mUnmapAction;
    return NOERROR;
}

ECode CharsetEncoder::ImplCanEncode(
    /* [in] */ ICharBuffer* cb,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(cb);
    VALIDATE_NOT_NULL(result);

    if (mStatus == FLUSH) {
        mStatus = INIT;
    }
    if (mStatus != INIT) {
        // throw new IllegalStateException("encoding already in progress");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    ICodingErrorAction* malformBak = mMalformAction;
    ICodingErrorAction* unmapBak = mUnmapAction;
    AutoPtr<ICodingErrorAction> helper;
    CCodingErrorAction::New((ICodingErrorAction** )&helper);
    AutoPtr<ICodingErrorAction> REPORT;
    FAIL_RETURN(helper->GetREPORT((ICodingErrorAction **)&REPORT));

    AutoPtr<ICharsetEncoder> encoder;
    FAIL_RETURN(OnMalformedInput(REPORT.Get(), (ICharsetEncoder**)&encoder));
    FAIL_RETURN(OnUnmappableCharacter(REPORT.Get(), (ICharsetEncoder**)&encoder));
    *result = TRUE;

    AutoPtr<IByteBuffer> bb;
    if(FAILED(Encode(cb, (IByteBuffer**)&bb))) {
        *result = FALSE;
    }
    FAIL_RETURN(OnMalformedInput(malformBak, (ICharsetEncoder**)&encoder));
    FAIL_RETURN(OnUnmappableCharacter(unmapBak, (ICharsetEncoder**)&encoder));
    FAIL_RETURN(Reset((ICharsetEncoder**)&encoder));
    return NOERROR;
}

ECode CharsetEncoder::CheckCoderResult(
    /* [in] */ ICoderResult* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICodingErrorAction> helper;
    CCodingErrorAction::New((ICodingErrorAction** )&helper);
    AutoPtr<ICodingErrorAction> REPORT;
    FAIL_RETURN(helper->GetREPORT((ICodingErrorAction **)&REPORT));

    Boolean isEnable;
    if (mMalformAction == REPORT && (result->IsMalformed(&isEnable), isEnable)) {
        // throw new MalformedInputException(result.length());
        return E_MALFORMED_INPUT_EXCEPTION;
    } else if (mUnmapAction == REPORT && (result->IsUnmappable(&isEnable), isEnable)) {
        // throw new UnmappableCharacterException(result.length());
        return E_UNMAPPABLE_CHARACTER_EXCEPTION;
    }
    return NOERROR;
}

ECode CharsetEncoder::AllocateMore(
    /* [in, out] */ IByteBuffer* output,
    /* [out] */ IByteBuffer** byteBuffer)
{
    VALIDATE_NOT_NULL(output);
    VALIDATE_NOT_NULL(byteBuffer);

    Int32 cap;
    FAIL_RETURN(output->Capacity(&cap));
    if (cap == 0) {
        FAIL_RETURN(ByteBuffer::Allocate(1, byteBuffer));
        return NOERROR;
    }
    FAIL_RETURN(ByteBuffer::Allocate(cap * 2, byteBuffer));
    FAIL_RETURN(output->Flip());
    FAIL_RETURN((*byteBuffer)->PutByteBuffer(output));
    return NOERROR;
}

ECode CharsetEncoder::ImplFlush(
    /* [in] */ IByteBuffer* outInput,
    /* [out] */  ICoderResult** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICoderResultHelper> helper;
    AutoPtr<ICoderResult> UNDERFLOW;
    CCoderResultHelper::AcquireSingleton((ICoderResultHelper**)&helper);
    helper->GetUNDERFLOW((ICoderResult**)&UNDERFLOW);

    *result = UNDERFLOW;
    return NOERROR;
}

ECode CharsetEncoder::ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction)
{
    // default implementation is empty
    return NOERROR;
}
ECode CharsetEncoder::ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction)
{
    // default implementation is empty
    return NOERROR;
}

ECode CharsetEncoder::ImplReplaceWith(
        /* [in] */ ArrayOf<Byte>& newReplacement)
{
    // default implementation is empty
    return NOERROR;
}

ECode CharsetEncoder::ImplReset()
{
    // default implementation is empty
    return NOERROR;
}

ECode CharsetEncoder::UncheckedReplaceWith(
        /* [in] */ const ArrayOf<Byte>& replacement)
{
    // It seems like a bug, but the RI doesn't clone, and we have tests that check we don't.
    if (mReplace != NULL)
    {
        ArrayOf<Byte>::Free(mReplace);
    }

    mReplace = replacement.Clone();
    return ImplReplaceWith(*mReplace);
}
