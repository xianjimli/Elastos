
#ifndef __NATIVECONVERTER_H__
#define __NATIVECONVERTER_H__

#include <elastos.h>
#include <Elastos.IO.h>
#include <unicode/ucnv.h>

using namespace Elastos;

class NativeConverter
{
public:
    static CARAPI Decode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Byte>& source,
        /* [in] */ Int32 sourceEnd,
        /* [in] */ const ArrayOf<Char8>& target,
        /* [in] */ Int32 targetEnd,
        /* [in, out] */ ArrayOf<Int32>* data,
        /* [in] */ Boolean flush,
        /* [out] */ Int32* errorCode);

    static CARAPI Encode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Char8>& source,
        /* [in] */ Int32 sourceEnd,
        /* [in] */ const ArrayOf<Byte>& target,
        /* [in] */ Int32 targetEnd,
        /* [in, out] */ ArrayOf<Int32>* data,
        /* [in] */ Boolean flush,
        /* [out] */ Int32* errorCode);

    static CARAPI FlushCharToByte(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Byte>& target,
        /* [in] */ Int32 targetEnd,
        /* [in, out] */ ArrayOf<Int32>* data,
        /* [out] */ Int32* errorCode);

    static CARAPI FlushByteToChar(
        /* [in] */ Int64 converterHandle,
        /* [in] */ const ArrayOf<Char8>& target,
        /* [in] */ Int32 targetEnd,
        /* [in, out] */ ArrayOf<Int32>* data,
        /* [out] */ Int32* errorCode);

    static CARAPI OpenConverter(
        /* [in] */ const String& converterName,
        /* [out] */ Int64* converterHandle);

    static CARAPI CloseConverter(
        /* [in] */ Int64 converterHandle);

    static CARAPI ResetByteToChar(
        /* [in] */ Int64 converterHandle);

    static CARAPI ResetCharToByte(
        /* [in] */ Int64 converterHandle);

    static CARAPI GetSubstitutionBytes(
        /* [in] */ Int64 converterHandle,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    static CARAPI GetMaxBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Int32* max);

    static CARAPI GetMinBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Int32* min);

    static CARAPI GetAveBytesPerChar(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Float* ave);

    static CARAPI GetAveCharsPerByte(
        /* [in] */ Int64 converterHandle,
        /* [out] */ Float* ave);

    static CARAPI Contains(
        /* [in] */ const String& converterName1,
        /* [in] */ const String& converterName2,
        /* [out] */ Boolean* isContains);

    static CARAPI CanEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ Int32 codeUnit,
        /* [out] */ Boolean* canEncode);

    static CARAPI GetAvailableCharsetNames(
        /* [out, callee] */ ArrayOf<String>** names);

    static CARAPI CharsetForName(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset);

    static CARAPI TranslateCodingErrorAction(
        /* [in] */ ICodingErrorAction* action,
        /* [out] */ Int32* errorCode);

    static CARAPI SetCallbackDecode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ Int32 onMalformedInput,
        /* [in] */ Int32 onUnmappableInput,
        /* [in] */ const char* subChars,
        /* [in] */ Int32 subCharsSize,
        /* [out] */ Int32* errorCode);

    static CARAPI SetCallbackEncode(
        /* [in] */ Int64 converterHandle,
        /* [in] */ Int32 onMalformedInput,
        /* [in] */ Int32 onUnmappableInput,
        /* [in] */ const Byte* subBytes,
        /* [in] */ Int32 subBytesSize,
        /* [out] */ Int32* errorCode);

private:
	static UConverter* ToUConverter(
		/* [in] */ Int64 address);

	/*
	 * If a charset listed in the IANA Charset Registry is supported by an implementation
	 * of the Java platform then its canonical name must be the name listed in the registry.
	 * Many charsets are given more than one name in the registry, in which case the registry
	 * identifies one of the names as MIME-preferred. If a charset has more than one registry
	 * name then its canonical name must be the MIME-preferred name and the other names in
	 * the registry must be valid aliases. If a supported charset is not listed in the IANA
	 * registry then its canonical name must begin with one of the strings "X-" or "x-".
	 */
	static String GetJavaCanonicalName(
		/* [in] */ const char* icuCanonicalName);

    static ArrayOf<String>* GetAliases(
        /* [in] */ const char* icuCanonicalName);

    static const char* GetICUCanonicalName(
        /* [in] */ const char* name);

    static void CHARSET_ENCODER_CALLBACK(
        /* [in] */ const void* rawContext,
        /* [in] */ UConverterFromUnicodeArgs* args,
        /* [in] */ const UChar* codeUnits,
        /* [in] */ int32_t length,
        /* [in] */ UChar32 codePoint,
        /* [in] */ UConverterCallbackReason reason,
        /* [out] */ UErrorCode* status);

    static void EncoderReplaceCallback(
        /* [in] */ const void* rawContext,
        /* [in] */ UConverterFromUnicodeArgs* fromArgs,
        /* [in] */ const UChar*,
        /* [in] */ int32_t,
        /* [in] */ UChar32,
        /* [in] */ UConverterCallbackReason,
        /* [out] */ UErrorCode * err);

    static UConverterFromUCallback GetFromUCallback(
        /* [in] */ int32_t mode);

    static void DecoderIgnoreCallback(
        /* [in] */ const void*,
        /* [in] */ UConverterToUnicodeArgs*,
        /* [in] */ const char*,
        /* [in] */ int32_t,
        /* [in] */ UConverterCallbackReason,
        /* [out] */ UErrorCode* err);

    static void DecoderReplaceCallback(
        /* [in] */ const void* rawContext,
        /* [in] */ UConverterToUnicodeArgs* toArgs,
        /* [in] */ const char*,
        /* [in] */ int32_t,
        /* [in] */ UConverterCallbackReason,
        /* [out] */ UErrorCode* err);

    static UConverterToUCallback GetToUCallback(
    /* [in] */ int32_t mode);

    static void CHARSET_DECODER_CALLBACK(
        /* [in] */ const void* rawContext,
        /* [in] */ UConverterToUnicodeArgs* args,
        /* [in] */ const char* codeUnits,
        /* [in] */ int32_t length,
        /* [in] */ UConverterCallbackReason reason,
        /* [out] */ UErrorCode* status);
};

#endif // __NATIVECONVERTER_H__
