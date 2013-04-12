
#ifndef __CHARSETENCODER_H__
#define __CHARSETENCODER_H__

#include <elastos.h>
#include <Elastos.IO.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class CharsetEncoder
{
public:
    CharsetEncoder(
        /* [in] */ ICharset* cs,
        /* [in] */ Float averageBytesPerChar,
        /* [in] */ Float maxBytesPerChar);

    CharsetEncoder(
        /* [in] */ ICharset* cs,
        /* [in] */ Float averageBytesPerChar,
        /* [in] */ Float maxBytesPerChar,
        /* [in] */ const ArrayOf<Byte>& replacement);

    CARAPI AverageBytesPerChar(
        /* [out] */ Float* averageNumber);

    CARAPI CanEncode(
        /* [in] */ Char8 c,
        /* [out] */ Boolean* result);

    CARAPI CanEncodeEx(
        /* [in] */ ICharSequence* pSequence,
        /* [out] */ Boolean* result);

    CARAPI Charset(
        /* [out] */ ICharset** charset);

    CARAPI Encode(
        /* [in] */ ICharBuffer* charBuffer,
        /* [out] */ IByteBuffer** byteBuffer);

    CARAPI EncodeEx(
        /* [in] */ ICharBuffer* charBuffer,
        /* [out] */ IByteBuffer** byteBuffer,
        /* [in] */ Boolean endOfInput,
        /* [out] */ ICoderResult** result);

    virtual CARAPI EncodeLoop(
        /* [in] */ ICharBuffer* charBuffer,
        /* [out] */ IByteBuffer** byteBuffer,
        /* [out] */ ICoderResult** result) = 0;

    CARAPI Flush(
        /* [out] */ IByteBuffer** byteBuffer,
        /* [out] */ ICoderResult** result);

    CARAPI IsLegalReplacement(
        /* [in] */ const ArrayOf<Byte>& replacement,
        /* [out] */ Boolean* result);

    CARAPI MalformedInputAction(
        /* [out] */ ICodingErrorAction** errorAction);

    CARAPI MaxBytesPerChar(
        /* [out] */ Float* maxNumber);

    CARAPI OnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction,
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI OnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction,
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI Replacement(
        /* [out, callee] */ ArrayOf<Byte>** replace);

    CARAPI ReplaceWith(
        /* [in] */ const ArrayOf<Byte>& replacement,
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI Reset(
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI UnmappableCharacterAction(
        /* [out] */ ICodingErrorAction** errorAction);

private:
    // implementation of canEncode
    CARAPI ImplCanEncode(
        /* [in] */ ICharBuffer* cb,
        /* [out] */ Boolean* result);

    /*
     * checks the result whether it needs to throw CharacterCodingException.
     */
    CARAPI CheckCoderResult(
        /* [in] */ ICoderResult* result);

    // allocate more spaces to the given ByteBuffer
    CARAPI AllocateMore(
        /* [in] */ IByteBuffer* output,
        /* [out] */ IByteBuffer** byteBuffer);

    /**
     * Flushes this encoder. The default implementation does nothing and always
     * returns <code>CoderResult.UNDERFLOW</code>; this method can be
     * overridden if needed.
     *
     * @param out
     *            the output buffer.
     * @return <code>CoderResult.UNDERFLOW</code> or
     *         <code>CoderResult.OVERFLOW</code>.
     */
    CARAPI ImplFlush(
        /* [in] */ IByteBuffer* outInput,
        /* [out] */  ICoderResult** result);

    /**
     * Notifies that this encoder's <code>CodingErrorAction</code> specified
     * for malformed input error has been changed. The default implementation
     * does nothing; this method can be overridden if needed.
     *
     * @param newAction
     *            the new action.
     */
    CARAPI ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction);

        /**
     * Notifies that this encoder's <code>CodingErrorAction</code> specified
     * for unmappable character error has been changed. The default
     * implementation does nothing; this method can be overridden if needed.
     *
     * @param newAction
     *            the new action.
     */
    CARAPI ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction);

    /**
     * Notifies that this encoder's replacement has been changed. The default
     * implementation does nothing; this method can be overridden if needed.
     *
     * @param newReplacement
     *            the new replacement string.
     */
    CARAPI ImplReplaceWith(
        /* [in] */ ArrayOf<Byte>& newReplacement);

    /**
     * Resets this encoder's charset related state. The default implementation
     * does nothing; this method can be overridden if needed.
     */
    CARAPI ImplReset();

    CARAPI UncheckedReplaceWith(
        /* [in] */ ArrayOf<Byte>& replacement);

private:
    /*
     * internal status consts
     */
    static const Int32 INIT = 0;

    static const Int32 ONGOING = 1;

    static const Int32 END = 2;

    static const Int32 FLUSH = 3;

    // the Charset which creates this encoder
    AutoPtr<ICharset> mCs;

    // average bytes per character created by this encoder
    Float mAverBytes;

    // maximum bytes per character can be created by this encoder
    Float mMaxBytes;

    // replacement byte array
    ArrayOf<Byte>* mReplace;

    // internal status
    Int32 mStatus;

    // action for malformed input
    AutoPtr<ICodingErrorAction> mMalformAction;

    // action for unmapped char input
    AutoPtr<ICodingErrorAction> mUnmapAction;

    // decoder instance for this encoder's charset, used for replacement value
    // checking
    AutoPtr<ICharsetDecoder> mDecoder;
};

#endif // __CHARSETENCODER_H__
