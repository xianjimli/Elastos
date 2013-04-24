
#ifndef __CNATIVECONVERTER_H__
#define __CNATIVECONVERTER_H__

#include "_CNativeConverter.h"

CarClass(CNativeConverter)
{
public:
    CARAPI Decode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Byte>& input,
        /* [in] */ Int32 inEnd,
        /* [in] */ const ArrayOf<Char8>& output,
        /* [in] */ Int32 outEnd,
        /* [in, out] */ ArrayOf<Int32>* data,
        /* [in] */ Boolean flush,
        /* [out] */ Int32* errorCode);

    CARAPI Encode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Char8>& input,
        /* [in] */ Int32 inEnd,
        /* [in] */ const ArrayOf<Byte>& output,
        /* [in] */ Int32 outEnd,
        /* [in, out] */ ArrayOf<Int32>* data,
        /* [in] */ Boolean flush,
        /* [out] */ Int32* errorCode);

    CARAPI FlushCharToByte(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Byte>& output,
        /* [in] */ Int32 outEnd,
        /* [in, out] */ ArrayOf<Int32>* data,
        /* [out] */ Int32* errorCode);

    CARAPI FlushByteToChar(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Char8>& output,
        /* [in] */ Int32 outEnd,
        /* [in, out] */ ArrayOf<Int32>* data,
        /* [out] */ Int32* errorCode);

    CARAPI OpenConverter(
        /* [in] */ const String& encoding,
        /* [out] */ Int64* converterHandle);

    CARAPI CloseConverter(
        /* [in] */ Int64 converterHandle);

    CARAPI ResetByteToChar(
        /* [in] */ Int64 converterHandle);

    CARAPI ResetCharToByte(
        /* [in] */ Int64 converterHandle);

    CARAPI GetSubstitutionBytes(
        /* [in] */ Int64 converterHandle,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI GetMaxBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Int32* max);

    CARAPI GetMinBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Int32* min);

    CARAPI GetAveBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Float* ave);

    CARAPI GetAveCharsPerByte(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Float* ave);

    CARAPI Contains(
        /* [in] */ const String& converterName1,
        /* [in] */ const String& converterName2,
        /* [out] */ Boolean* isContains);

    CARAPI CanEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ Int32 codeUnit,
        /* [out] */ Boolean* canEncode);

    CARAPI GetAvailableCharsetNames(
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI CharsetForName(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset);

    CARAPI SetCallbackDecode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ ICharsetDecoder* decoder,
        /* [out] */ Int32* errorCode);

    CARAPI SetCallbackEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ ICharsetEncoder* encoder,
        /* [out] */ Int32* errorCode);
};

#endif // __CNATIVECONVERTER_H__
