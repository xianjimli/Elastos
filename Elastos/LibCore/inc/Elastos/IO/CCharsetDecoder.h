
#ifndef __CCHARSETDECODER_H__
#define __CCHARSETDECODER_H__

#include "_CCharsetDecoder.h"

CarClass(CCharsetDecoder)
{
public:
    CARAPI constructor(
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

    CARAPI DetectedCharset(
        /* [out] */ ICharset** ppCharset);

    CARAPI Flush(
        /* [out] */ ICharBuffer** ppCharBuffer,
        /* [out] */ ICoderResult** ppResult);

    CARAPI ImplFlush(
        /* [out] */ ICharBuffer** ppCharBuffer,
        /* [out] */ ICoderResult** ppResult);

    CARAPI ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* pNewAction);

    CARAPI ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* pNewAction);

    CARAPI ImplReplaceWith(
        /* [in] */ const String& newReplacement);

    CARAPI ImplReset();

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
    // TODO: Add your private member variables here.
};

#endif // __CCHARSETDECODER_H__
