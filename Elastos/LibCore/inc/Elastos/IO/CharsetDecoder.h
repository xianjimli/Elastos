
#ifndef __CHARSETDECODER_H__
#define __CHARSETDECODER_H__

#include <elastos.h>
#include <Elastos.IO.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

class CharsetDecoder
{
public:
    CharsetDecoder(
        /* [in] */ ICharset* pCharset,
        /* [in] */ Float averageCharsPerByte,
        /* [in] */ Float maxCharsPerByte);
            
    CARAPI AverageCharsPerByte(
        /* [out] */ Float* pAverage);

    CARAPI Charset(
        /* [out] */ ICharset** ppCharset);

    CARAPI Decode(
        /* [in] */ IByteBuffer* pByteBuffer,
        /* [out] */ ICharBuffer** ppCharBuffer);

    CARAPI DecodeEx(
        /* [in] */ IByteBuffer* pByteBuffer,
        /* [out] */ ICharBuffer** ppCharBuffer,
        /* [in] */ Boolean endOfInput,
        /* [out] */ ICoderResult** ppResult);

    CARAPI DecodeLoop(
        /* [in] */ IByteBuffer* pByteBuffer,
        /* [out] */ ICharBuffer** ppCharBuffer,
        /* [out] */ ICoderResult** ppResult);

    virtual CARAPI DecodeLoop(
        /* [in] */ IByteBuffer* byteBuffer,
        /* [in] */ ICharBuffer* charBuffer,
        /* [out] */ ICoderResult** result) = 0;

    CARAPI DetectedCharset(
        /* [out] */ ICharset** ppCharset);

    CARAPI Flush(
        /* [out] */ ICharBuffer** ppCharBuffer,
        /* [out] */ ICoderResult** ppResult);

    CARAPI IsAutoDetecting(
        /* [out] */ Boolean* pResult);

    CARAPI IsCharsetDetected(
        /* [out] */ Boolean* pResult);

    CARAPI MalformedInputAction(
        /* [out] */ ICodingErrorAction** ppCodingErrorAction);

    CARAPI MaxCharsPerByte(
        /* [out] */ Float* pMaxNumber);

    CARAPI OnMalformedInput(
        /* [in] */ ICodingErrorAction* pNewAction,
        /* [out] */ ICharsetDecoder** ppCharsetDecoder);

    CARAPI OnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* pNewAction,
        /* [out] */ ICharsetDecoder** ppCharsetDecoder);

    CARAPI Replacement(
        /* [out] */ String* pReplacement);

    CARAPI ReplaceWith(
        /* [in] */ const String& newReplacement,
        /* [out] */ ICharsetDecoder** ppCharsetDecoder);

    CARAPI Reset(
        /* [out] */ ICharsetDecoder** ppCharsetDecoder);

    CARAPI UnmappableCharacterAction(
        /* [out] */ ICodingErrorAction** ppCodingErrorAction);

private:
    /*
     * checks the result whether it needs to throw CharacterCodingException.
     */
    CARAPI CheckCoderResult(
        /* [in] */ ICoderResult* result);

    /*
     * original output is full and doesn't have remaining. allocate more space
     * to new CharBuffer and return it, the contents in the given buffer will be
     * copied into the new buffer.
     */
    CARAPI AllocateMore(
        /* [in] */ ICharBuffer* output);

    /**
     * Flushes this decoder. The default implementation does nothing and always
     * returns <code>CoderResult.UNDERFLOW</code>; this method can be
     * overridden if needed.
     *
     * @param out
     *            the output buffer.
     * @return <code>CoderResult.UNDERFLOW</code> or
     *         <code>CoderResult.OVERFLOW</code>.
     */
    CARAPI ImplFlush(
        /* [in] */ ICharBuffer* outInput,
        /* [out] */ ICoderResult** result);

    /**
     * Notifies that this decoder's <code>CodingErrorAction</code> specified
     * for malformed input error has been changed. The default implementation
     * does nothing; this method can be overridden if needed.
     *
     * @param newAction
     *            the new action.
     */
    CARAPI ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction);

    /**
     * Notifies that this decoder's <code>CodingErrorAction</code> specified
     * for unmappable character error has been changed. The default
     * implementation does nothing; this method can be overridden if needed.
     *
     * @param newAction
     *            the new action.
     */
    CARAPI ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction);

    /**
     * Notifies that this decoder's replacement has been changed. The default
     * implementation does nothing; this method can be overridden if needed.
     *
     * @param newReplacement
     *            the new replacement string.
     */
    CARAPI ImplReplaceWith(
        /* [in] */ String newReplacement);

    /**
     * Reset this decoder's charset related state. The default implementation
     * does nothing; this method can be overridden if needed.
     */
    CARAPI ImplReset();

private:
    /*
     * internal status consts
     */
    static const int INIT = 0;

    static const int ONGOING = 1;

    static const int END = 2;

    static const int FLUSH = 3;

    // average number of chars for one byte
    Float mAverChars;

    // maximum number of chars for one byte
    Float mMaxChars;

    // charset for this decoder
    AutoPtr<ICharset> mCs;

    // specify the action if malformed input error encountered
    AutoPtr<ICodingErrorAction> mMalformAction;

    // specify the action if unmappable character error encountered
    AutoPtr<ICodingErrorAction> mUnmapAction;

    // the replacement string
    String mReplace;

    // the current status
    Int32 mStatus;
};

#endif // __CHARSETDECODER_H__
