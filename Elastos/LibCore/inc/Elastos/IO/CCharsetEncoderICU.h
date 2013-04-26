
#ifndef __CCHARSETENCODERICU_H__
#define __CCHARSETENCODERICU_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "_CCharsetEncoderICU.h"
#include "CharsetEncoder.h"
#include "elastos/HashMap.h"
#include "elastos/AutoFree.h"

CarClass(CCharsetEncoderICU), public CharsetEncoder
{
public:
    CCharsetEncoderICU();

    ~CCharsetEncoderICU();

    static CARAPI NewInstance(
        /* [in] */ ICharset* charset,
        /* [in] */ const String& icuCanonicalName,
        /* [out] */ ICharsetEncoder** encoder);

    CARAPI AverageBytesPerChar(
        /* [out] */ Float* averageNumber);

    CARAPI CanEncode(
        /* [in] */ Char8 c,
        /* [out] */ Boolean* result);

    CARAPI CanEncodeEx(
        /* [in] */ ICharSequence* sequence,
        /* [out] */ Boolean* result);

    CARAPI CanEncodeEx1(
        /* [in] */ Int32 codepoint,
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

    CARAPI Flush(
        /* [in] */ IByteBuffer* byteBuffer,
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

protected:
    CARAPI ImplReplaceWith(
        /* [in] */ const ArrayOf<Byte>& newReplacement);

    CARAPI ImplOnMalformedInput(
        /* [in] */ ICodingErrorAction* newAction);

    CARAPI ImplOnUnmappableCharacter(
        /* [in] */ ICodingErrorAction* newAction);

    CARAPI ImplFlush(
        /* [out] */ IByteBuffer** out,
        /* [out] */ ICoderResult** result);

    CARAPI ImplReset();

    CARAPI EncodeLoop(
        /* [in] */ ICharBuffer* charBuffer,
        /* [in, out] */ IByteBuffer* byteBuffer,
        /* [out] */ ICoderResult** result);

private:
    CARAPI constructor(
        /* [in] */ ICharset* charset,
        /* [in] */ Float averageBytesPerChar,
        /* [in] */ Float maxBytesPerChar,
        /* [in] */ const ArrayOf<Byte>& replacement,
        /* [in] */ Int64 address);

    static ArrayOf<Byte>* MakeReplacement(
        /* [in] */ String icuCanonicalName,
        /* [in] */ Int64 address);

    CARAPI UpdateCallback();

    const Int32 GetArray(
        /* [in] */ IByteBuffer* byteBuffer);

    const Int32 GetArray(
        /* [in] */ ICharBuffer* charBuffer);

    CARAPI SetPosition(
        /* [in] */ IByteBuffer* byteBuffer);

    CARAPI SetPosition(
        /* [in] */ ICharBuffer* charBuffer);

private:
    static HashMap<String, ArrayOf<Byte>* >* DEFAULT_REPLACEMENTS;

    static const Int32 INPUT_OFFSET = 0;
    static const Int32 OUTPUT_OFFSET = 1;
    static const Int32 INVALID_CHARS = 2;
    static const Int32 INPUT_HELD = 3;

    /*
     * data[INPUT_OFFSET]   = on input contains the start of input and on output the number of input chars consumed
     * data[OUTPUT_OFFSET]  = on input contains the start of output and on output the number of output bytes written
     * data[INVALID_CHARS]  = number of invalid chars
     * data[INPUT_HELD]     = number of input chars held in the converter's state
     */
    ArrayOf<Int32>* mData;

    /* handle to the ICU converter that is opened */
    Int64 mConverterHandle;

    AutoFree< ArrayOf<Char8> > mInput;
    AutoFree< ArrayOf<Byte> > mOutput;

    // BEGIN android-added
    AutoFree< ArrayOf<Char8> > mAllocatedInput;
    AutoFree< ArrayOf<Byte> > mAllocatedOutput;
    // END android-added

    // These instance variables are
    // always assigned in the methods
    // before being used. This class
    // inhrently multithread unsafe
    // so we dont have to worry about
    // synchronization
    Int32 mInEnd;
    Int32 mOutEnd;
    Int32 mEc;
    Int32 mSavedInputHeldLen;
};

#endif // __CCHARSETENCODERICU_H__
