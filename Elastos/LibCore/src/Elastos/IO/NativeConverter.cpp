
#include "NativeConverter.h"
#include "UniquePtr.h"
#include <elastos/AutoPtr.h>
#include <cmdef.h>
#include <unicode/ucnv_cb.h>
#include <unicode/ustring.h>
#include <unicode/uniset.h>
#include <stdio.h>

#define NativeConverter_REPORT 0
#define NativeConverter_IGNORE 1
#define NativeConverter_REPLACE 2

struct DecoderCallbackContext {
    int length;
    UChar subUChars[256];
    UConverterToUCallback onUnmappableInput;
    UConverterToUCallback onMalformedInput;
};

struct EncoderCallbackContext {
    int length;
    char subBytes[256];
    UConverterFromUCallback onUnmappableInput;
    UConverterFromUCallback onMalformedInput;
};

struct UConverterDeleter {
    void operator()(UConverter* p) const {
        ucnv_close(p);
    }
};

typedef UniquePtr<UConverter, UConverterDeleter> UniqueUConverter;

ECode NativeConverter::Decode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Byte>& source,
    /* [in] */ Int32 sourceEnd,
    /* [in] */ const ArrayOf<Char8>& target,
    /* [in] */ Int32 targetEnd,
    /* [in, out] */ ArrayOf<Int32>* data,
    /* [in] */ Boolean flush,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(data);
    VALIDATE_NOT_NULL(errorCode);

    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv == NULL) {
        *errorCode = U_ILLEGAL_ARGUMENT_ERROR;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Do the conversion.
    Int32* sourceOffset = &((*data)[0]);
    Int32* targetOffset =  &((*data)[1]);
    const char* mySource = reinterpret_cast<const char*>(source.GetPayload() + *sourceOffset);
    const char* mySourceLimit = reinterpret_cast<const char*>(source.GetPayload() + sourceEnd);
    UChar* cTarget = (UChar*)(target.GetPayload() + *targetOffset);
    const UChar* cTargetLimit = (UChar*)(target.GetPayload() + targetEnd);
    UErrorCode errorCodeL = U_ZERO_ERROR;
    ucnv_toUnicode(cnv, &cTarget, cTargetLimit, &mySource, mySourceLimit, NULL, flush, &errorCodeL);
    *sourceOffset = mySource - reinterpret_cast<const char*>(source.GetPayload()) - *sourceOffset;
    *targetOffset = (Int32)cTarget - (Int32)target.GetPayload() - *targetOffset;

    // Check how much more input is necessary to complete what's in the converter's internal buffer.
    UErrorCode minorErrorCode = U_ZERO_ERROR;
    Int32 pending = ucnv_toUCountPending(cnv, &minorErrorCode);
    (*data)[3] = pending;

    // If there was an error, count the problematic bytes.
    if (errorCodeL == U_ILLEGAL_CHAR_FOUND || errorCodeL == U_INVALID_CHAR_FOUND) {
        int8_t len = 32;
        char invalidChars[32] = {'\0'};
        ucnv_getInvalidChars(cnv, invalidChars, &len, &minorErrorCode);
        if (U_SUCCESS(minorErrorCode)) {
            (*data)[2] = len;
        }
    }

    *errorCode = errorCodeL;
    return NOERROR;
}

ECode NativeConverter::Encode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Char8>& source,
    /* [in] */ Int32 sourceEnd,
    /* [in] */ const ArrayOf<Byte>& target,
    /* [in] */ Int32 targetEnd,
    /* [in, out] */ ArrayOf<Int32>* data,
    /* [in] */ Boolean flush,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(data);
    VALIDATE_NOT_NULL(errorCode);

    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv == NULL) {
        *errorCode = U_ILLEGAL_ARGUMENT_ERROR;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Do the conversion.
    Int32* sourceOffset = &((*data)[0]);
    Int32* targetOffset = &((*data)[1]);
    const UChar* mySource = (UChar*)(source.GetPayload() + *sourceOffset);
    const UChar* mySourceLimit= (UChar*)(source.GetPayload() + sourceEnd);
    char* cTarget = reinterpret_cast<char*>(target.GetPayload() + *targetOffset);
    const char* cTargetLimit = reinterpret_cast<const char*>(target.GetPayload() + targetEnd);
    UErrorCode errorCodeL = U_ZERO_ERROR;
    ucnv_fromUnicode(cnv , &cTarget, cTargetLimit, &mySource, mySourceLimit, NULL, (UBool) flush, &errorCodeL);
    *sourceOffset = ((Int32)mySource - (Int32)source.GetPayload()) - *sourceOffset;
    *targetOffset = (reinterpret_cast<byte*>(cTarget) - target.GetPayload()) - *targetOffset;

    // Check how much more input is necessary to complete what's in the converter's internal buffer.
    UErrorCode minorErrorCode = U_ZERO_ERROR;
    int32_t pending = ucnv_fromUCountPending(cnv, &minorErrorCode);
    if (U_SUCCESS(minorErrorCode)) {
        (*data)[3] = pending;
    }

    // If there was an error, count the problematic characters.
    if (errorCodeL == U_ILLEGAL_CHAR_FOUND || errorCodeL == U_INVALID_CHAR_FOUND) {
        int8_t len = 32;
        UChar invalidUChars[32];
        ucnv_getInvalidUChars(cnv, invalidUChars, &len, &minorErrorCode);
        if (U_SUCCESS(minorErrorCode)) {
            (*data)[2] = len;
        }
    }

    *errorCode = errorCodeL;
    return NOERROR;
}

ECode NativeConverter::FlushCharToByte(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Byte>& target,
    /* [in] */ Int32 targetEnd,
    /* [in, out] */ ArrayOf<Int32>* data,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(data);
    VALIDATE_NOT_NULL(errorCode);

    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv == NULL) {
        return U_ILLEGAL_ARGUMENT_ERROR;
    }

    char source = '\0';
    Int32* targetOffset = &((*data)[1]);
    const UChar* mySource = (UChar*)&source;
    const UChar* mySourceLimit= (UChar*)&source;
    char* cTarget = reinterpret_cast<char*>(target.GetPayload() + *targetOffset);
    const char* cTargetLimit = reinterpret_cast<char*>(target.GetPayload() + targetEnd);
    UErrorCode errorCodeL = U_ZERO_ERROR;
    ucnv_fromUnicode(cnv, &cTarget, cTargetLimit, &mySource, mySourceLimit, NULL, TRUE, &errorCodeL);
    *targetOffset = reinterpret_cast<Byte*>(cTarget) - target.GetPayload() - *targetOffset;
    *errorCode = errorCodeL;

    return NOERROR;
}

ECode NativeConverter::FlushByteToChar(
    /* [in] */ Int64 converterHandle,
    /* [in] */ const ArrayOf<Char8>& target,
    /* [in] */ Int32 targetEnd,
    /* [in, out] */ ArrayOf<Int32>* data,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(data);
    VALIDATE_NOT_NULL(errorCode);

    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv == NULL) {
        *errorCode = U_ILLEGAL_ARGUMENT_ERROR;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    char source = '\0';
    Int32* targetOffset = &((*data)[1]);
    const char* mySource = &source;
    const char* mySourceLimit = &source;
    UChar* cTarget = (UChar*)(target.GetPayload() + *targetOffset);
    const UChar* cTargetLimit = (UChar*)(target.GetPayload() + targetEnd);
    UErrorCode errorCodeL = U_ZERO_ERROR;
    ucnv_toUnicode(cnv, &cTarget, cTargetLimit, &mySource, mySourceLimit, NULL, TRUE, &errorCodeL);
    *targetOffset = (Int32)cTarget - (Int32)target.GetPayload() - *targetOffset;
    *errorCode = errorCodeL;

    return NOERROR;
}

ECode NativeConverter::OpenConverter(
    /* [in] */ const String& converterName,
    /* [out] */ Int64* converterHandle)
{
    VALIDATE_NOT_NULL(converterHandle);

    if (converterName.IsNull()) {
        *converterHandle = 0;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    UErrorCode errorCode = U_ZERO_ERROR;
    UConverter* cnv = ucnv_open(converterName.string(), &errorCode);
    FAIL_RETURN(CheckErrorCode(errorCode));
    *converterHandle = reinterpret_cast<uintptr_t>(cnv);

    return NOERROR;
}

ECode NativeConverter::CloseConverter(
    /* [in] */ Int64 converterHandle)
{
    ucnv_close(ToUConverter(converterHandle));
    return NOERROR;
}

ECode NativeConverter::ResetByteToChar(
    /* [in] */ Int64 converterHandle)
{
    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv) {
        ucnv_resetToUnicode(cnv);
    }
    return NOERROR;
}

ECode NativeConverter::ResetCharToByte(
    /* [in] */ Int64 converterHandle)
{
    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv) {
        ucnv_resetFromUnicode(cnv);
    }
    return NOERROR;
}

ECode NativeConverter::GetSubstitutionBytes(
    /* [in] */ Int64 converterHandle,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv == NULL) {
        *bytes = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    UErrorCode status = U_ZERO_ERROR;
    char subBytes[10];
    int8_t len = sizeof(subBytes);
    ucnv_getSubstChars(cnv, subBytes, &len, &status);
    if (!U_SUCCESS(status)) {
        *bytes = ArrayOf<Byte>::Alloc(0);
        return NOERROR;
    }
    *bytes = ArrayOf<Byte>::Alloc(len);
    memcpy((*bytes)->GetPayload(), reinterpret_cast<Byte*>(subBytes), len * sizeof(Byte));

    return NOERROR;
}

ECode NativeConverter::GetMaxBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Int32* max)
{
    VALIDATE_NOT_NULL(max);

    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv != NULL){
        *max = ucnv_getMaxCharSize(cnv);
    } else {
        *max = -1;
    }
    return NOERROR;
}

ECode NativeConverter::GetMinBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Int32* min)
{
    VALIDATE_NOT_NULL(min);

    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv != NULL){
        *min = ucnv_getMinCharSize(cnv);
    } else {
        *min = -1;
    }
    return NOERROR;
}

ECode NativeConverter::GetAveBytesPerChar(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Float* ave)
{
    VALIDATE_NOT_NULL(ave);

    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv != NULL){
        *ave = (ucnv_getMaxCharSize(cnv) + ucnv_getMinCharSize(cnv)) / 2.0;
    } else {
        *ave = -1;
    }
    return NOERROR;
}

ECode NativeConverter::GetAveCharsPerByte(
    /* [in] */ Int64 converterHandle,
    /* [out] */ Float* ave)
{
    VALIDATE_NOT_NULL(ave);

    Int32 max;
    FAIL_RETURN(GetMaxBytesPerChar(converterHandle, &max));
    *ave = 1 / (Float)max;
    return NOERROR;
}

ECode NativeConverter::Contains(
    /* [in] */ const String& converterName1,
    /* [in] */ const String& converterName2,
    /* [out] */ Boolean* isContains)
{
    VALIDATE_NOT_NULL(isContains);

    UErrorCode errorCode = U_ZERO_ERROR;
    UniqueUConverter converter1(ucnv_open(converterName1.string(), &errorCode));
    UnicodeSet set1;
    ucnv_getUnicodeSet(converter1.get(), set1.toUSet(), UCNV_ROUNDTRIP_SET, &errorCode);

    UniqueUConverter converter2(ucnv_open(converterName2.string(), &errorCode));
    UnicodeSet set2;
    ucnv_getUnicodeSet(converter2.get(), set2.toUSet(), UCNV_ROUNDTRIP_SET, &errorCode);

    *isContains = U_SUCCESS(errorCode) && set1.containsAll(set2);
    return NOERROR;
}

ECode NativeConverter::CanEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ Int32 codeUnit,
    /* [out] */ Boolean* canEncode)
{
    VALIDATE_NOT_NULL(canEncode);

    UErrorCode errorCode = U_ZERO_ERROR;
    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv == NULL) {
        *canEncode = FALSE;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    UChar srcBuffer[3];
    const UChar* src = &srcBuffer[0];
    const UChar* srcLimit = (codeUnit < 0x10000) ? &src[1] : &src[2];

    char dstBuffer[5];
    char* dst = &dstBuffer[0];
    const char* dstLimit = &dstBuffer[4];

    int i = 0;
    UTF_APPEND_CHAR(&srcBuffer[0], i, 2, codeUnit);

    ucnv_fromUnicode(cnv, &dst, dstLimit, &src, srcLimit, NULL, TRUE, &errorCode);
    *canEncode = U_SUCCESS(errorCode);

    return NOERROR;
}

ECode NativeConverter::GetAvailableCharsetNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);

    int32_t num = ucnv_countAvailable();
    *names = ArrayOf<String>::Alloc(num);
    for (int i = 0; i < num; ++i) {
        const char* name = ucnv_getAvailableName(i);
        (**names)[i] = GetJavaCanonicalName(name);
    }
    return NOERROR;
}

ECode NativeConverter::CharsetForName(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);

    // Get ICU's canonical name for this charset.
    const char* icuCanonicalName = GetICUCanonicalName(charsetName.string());
    if (icuCanonicalName == NULL) {
        *charset = NULL;
        return NOERROR;
    }
    // Get Java's canonical name for this charset.
    String javaCanonicalName = GetJavaCanonicalName(icuCanonicalName);

    // Check that this charset is supported.
    // ICU doesn't offer any "isSupported", so we just open and immediately close.
    // We ignore the UErrorCode because ucnv_open returning NULL is all the information we need.
    UErrorCode dummy = U_ZERO_ERROR;
    UniqueUConverter cnv(ucnv_open(icuCanonicalName, &dummy));
    if (cnv.get() == NULL) {
        *charset = NULL;
        return NOERROR;
    }
    cnv.reset();

    // Get the aliases for this charset.
    ArrayOf<String>* aliases = GetAliases(icuCanonicalName);

    // Construct the CharsetICU object.
    AutoPtr<ICharset> result;
    // TODO:
    // CCharsetICU::New(javaCanonicalName, String(icuCanonicalName), *aliases, (ICharset**)&result);
    *charset = result;

    return NOERROR;
}

ECode NativeConverter::TranslateCodingErrorAction(
    /* [in] */ ICodingErrorAction* action,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(action);
    VALIDATE_NOT_NULL(errorCode);

    AutoPtr<ICodingErrorAction> helper;
    AutoPtr<ICodingErrorAction> IGNORE;
    AutoPtr<ICodingErrorAction> REPLACE;
    AutoPtr<ICodingErrorAction> REPORT;
    CCodingErrorAction::New((ICodingErrorAction**)&helper);
    FAIL_RETURN(helper->GetIGNORE((ICodingErrorAction **)&IGNORE));
    FAIL_RETURN(helper->GetREPLACE((ICodingErrorAction **)&REPLACE));
    FAIL_RETURN(helper->GetREPORT((ICodingErrorAction **)&REPORT));

    if (action == REPORT) {
        *errorCode = 0;
        return NOERROR;
    } else if (action == IGNORE) {
        *errorCode = 1;
        return NOERROR;
    } else if (action == REPLACE) {
        *errorCode = 2;
        return NOERROR;
    }

    return E_ASSERTION_ERROR;
}

ECode NativeConverter::SetCallbackDecode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ Int32 onMalformedInput,
    /* [in] */ Int32 onUnmappableInput,
    /* [in] */ const char* subChars,
    /* [in] */ Int32 subCharsSize,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(subChars);

    UConverter* cnv = ToUConverter(converterHandle);
    if (cnv == NULL) {
        *errorCode = U_ILLEGAL_ARGUMENT_ERROR;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    UConverterToUCallback toUOldAction;
    const void* toUOldContext;
    ucnv_getToUCallBack(cnv, &toUOldAction, &toUOldContext);

    /* toUOldContext can only be DecodeCallbackContext since
     * the converter created is private data for the decoder
     * and callbacks can only be set via this method!
     */
    DecoderCallbackContext* toUNewContext = NULL;
    UConverterToUCallback toUNewAction = NULL;
    if (toUOldContext == NULL) {
        toUNewContext = new DecoderCallbackContext;
        toUNewAction = CHARSET_DECODER_CALLBACK;
    } else {
        toUNewContext = const_cast<DecoderCallbackContext*>(
                reinterpret_cast<const DecoderCallbackContext*>(toUOldContext));
        toUNewAction = toUOldAction;
        toUOldAction = NULL;
        toUOldContext = NULL;
    }
    toUNewContext->onMalformedInput = GetToUCallback(onMalformedInput);
    toUNewContext->onUnmappableInput = GetToUCallback(onUnmappableInput);
    toUNewContext->length = subCharsSize;
    u_strncpy(toUNewContext->subUChars, (const UChar*)subChars, subCharsSize);
    UErrorCode errorCodeL = U_ZERO_ERROR;
    ucnv_setToUCallBack(cnv, toUNewAction, toUNewContext, &toUOldAction, &toUOldContext,
            &errorCodeL);
    *errorCode = errorCodeL;

    return NOERROR;
}

ECode NativeConverter::SetCallbackEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ Int32 onMalformedInput,
    /* [in] */ Int32 onUnmappableInput,
    /* [in] */ const Byte* subBytes,
    /* [in] */ Int32 subBytesSize,
    /* [out] */ Int32* errorCode)
{
    VALIDATE_NOT_NULL(subBytes);

    UConverter* cnv = ToUConverter(converterHandle);
    if (!cnv) {
        *errorCode = U_ILLEGAL_ARGUMENT_ERROR;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    UConverterFromUCallback fromUOldAction = NULL;
    const void* fromUOldContext = NULL;
    ucnv_getFromUCallBack(cnv, &fromUOldAction, const_cast<const void**>(&fromUOldContext));

    /* fromUOldContext can only be DecodeCallbackContext since
     * the converter created is private data for the decoder
     * and callbacks can only be set via this method!
     */
    EncoderCallbackContext* fromUNewContext=NULL;
    UConverterFromUCallback fromUNewAction=NULL;
    if (fromUOldContext == NULL) {
        fromUNewContext = new EncoderCallbackContext;
        fromUNewAction = CHARSET_ENCODER_CALLBACK;
    } else {
        fromUNewContext = const_cast<EncoderCallbackContext*>(
                reinterpret_cast<const EncoderCallbackContext*>(fromUOldContext));
        fromUNewAction = fromUOldAction;
        fromUOldAction = NULL;
        fromUOldContext = NULL;
    }
    fromUNewContext->onMalformedInput = GetFromUCallback(onMalformedInput);
    fromUNewContext->onUnmappableInput = GetFromUCallback(onUnmappableInput);
    fromUNewContext->length = subBytesSize;
    memcpy(fromUNewContext->subBytes, subBytes, subBytesSize);
    UErrorCode errorCodeL = U_ZERO_ERROR;
    ucnv_setFromUCallBack(cnv, fromUNewAction, fromUNewContext, &fromUOldAction, &fromUOldContext,
            &errorCodeL);
    *errorCode = errorCodeL;

    return NOERROR;
}

UConverter* NativeConverter::ToUConverter(
    /* [in] */ Int64 converterHandle)
{
    return reinterpret_cast<UConverter*>(static_cast<uintptr_t>(converterHandle));
}


String NativeConverter::GetJavaCanonicalName(
    /* [in] */ const char* icuCanonicalName)
{
    UErrorCode status = U_ZERO_ERROR;

    // Check to see if this is a well-known MIME or IANA name.
    const char* cName = NULL;
    if ((cName = ucnv_getStandardName(icuCanonicalName, "MIME", &status)) != NULL) {
        return String(cName);
    } else if ((cName = ucnv_getStandardName(icuCanonicalName, "IANA", &status)) != NULL) {
        return String(cName);
    }

    // Check to see if an alias already exists with "x-" prefix, if yes then
    // make that the canonical name.
    int32_t aliasCount = ucnv_countAliases(icuCanonicalName, &status);
    for (int i = 0; i < aliasCount; ++i) {
        const char* name = ucnv_getAlias(icuCanonicalName, i, &status);
        if (name != NULL && name[0] == 'x' && name[1] == '-') {
            return String(name);
        }
    }

    // As a last resort, prepend "x-" to any alias and make that the canonical name.
    status = U_ZERO_ERROR;
    const char* name = ucnv_getStandardName(icuCanonicalName, "UTR22", &status);
    if (name == NULL && strchr(icuCanonicalName, ',') != NULL) {
        name = ucnv_getAlias(icuCanonicalName, 1, &status);
    }
    // If there is no UTR22 canonical name then just return the original name.
    if (name == NULL) {
        name = icuCanonicalName;
    }

    UniquePtr<char[]> result(new char[2 + strlen(name) + 1]);
    strcpy(&result[0], "x-");
    strcat(&result[0], name);
    return String(&result[0]);
}

ArrayOf<String>* NativeConverter::GetAliases(
    /* [in] */ const char* icuCanonicalName)
{
    // Get an upper bound on the number of aliases...
    const char* myEncName = icuCanonicalName;
    UErrorCode error = U_ZERO_ERROR;
    int32_t aliasCount = ucnv_countAliases(myEncName, &error);
    if (aliasCount == 0 && myEncName[0] == 'x' && myEncName[1] == '-') {
        myEncName = myEncName + 2;
        aliasCount = ucnv_countAliases(myEncName, &error);
    }
    if (!U_SUCCESS(error)) {
        return NULL;
    }

    // Collect the aliases we want...
    const char* aliasArray[aliasCount];
    int actualAliasCount = 0;
    for(int i = 0; i < aliasCount; ++i) {
        const char* name = ucnv_getAlias(myEncName, (uint16_t) i, &error);
        if (!U_SUCCESS(error)) {
            return NULL;
        }
        // TODO: why do we ignore these ones?
        if (strchr(name, '+') == 0 && strchr(name, ',') == 0) {
            aliasArray[actualAliasCount++]= name;
        }
    }

    // Convert our C++ char*[] into a Java String[]...
    ArrayOf<String>* result = ArrayOf<String>::Alloc(actualAliasCount);
    for (int i = 0; i < actualAliasCount; ++i) {
        (*result)[i] = String(aliasArray[i]);
    }
    return result;
}

const char* NativeConverter::GetICUCanonicalName(
    /* [in] */ const char* name)
{
    UErrorCode error = U_ZERO_ERROR;
    const char* canonicalName = NULL;
    if ((canonicalName = ucnv_getCanonicalName(name, "MIME", &error)) != NULL) {
        return canonicalName;
    } else if((canonicalName = ucnv_getCanonicalName(name, "IANA", &error)) != NULL) {
        return canonicalName;
    } else if((canonicalName = ucnv_getCanonicalName(name, "", &error)) != NULL) {
        return canonicalName;
    } else if((canonicalName =  ucnv_getAlias(name, 0, &error)) != NULL) {
        /* we have some aliases in the form x-blah .. match those first */
        return canonicalName;
    } else if (strstr(name, "x-") == name) {
        /* check if the converter can be opened with the name given */
        error = U_ZERO_ERROR;
        UniqueUConverter cnv(ucnv_open(name + 2, &error));
        if (cnv.get() != NULL) {
            return name + 2;
        }
    }
    return NULL;
}

void NativeConverter::CHARSET_ENCODER_CALLBACK(
    /* [in] */ const void* rawContext,
    /* [in] */ UConverterFromUnicodeArgs* args,
    /* [in] */ const UChar* codeUnits,
    /* [in] */ int32_t length,
    /* [in] */ UChar32 codePoint,
    /* [in] */ UConverterCallbackReason reason,
    /* [out] */ UErrorCode* status)
{
    if (!rawContext) {
        return;
    }
    const EncoderCallbackContext* ctx = reinterpret_cast<const EncoderCallbackContext*>(rawContext);
    switch(reason) {
    case UCNV_UNASSIGNED:
        ctx->onUnmappableInput(ctx, args, codeUnits, length, codePoint, reason, status);
        return;
    case UCNV_ILLEGAL:
    case UCNV_IRREGULAR:
        ctx->onMalformedInput(ctx, args, codeUnits, length, codePoint, reason, status);
        return;
    case UCNV_CLOSE:
        delete ctx;
        return;
    default:
        *status = U_ILLEGAL_ARGUMENT_ERROR;
        return;
    }
}

void NativeConverter::EncoderReplaceCallback(
    /* [in] */ const void* rawContext,
    /* [in] */ UConverterFromUnicodeArgs* fromArgs,
    /* [in] */ const UChar*,
    /* [in] */ int32_t,
    /* [in] */ UChar32,
    /* [in] */ UConverterCallbackReason,
    /* [out] */ UErrorCode * err)
{
    if (rawContext == NULL) {
        return;
    }
    const EncoderCallbackContext* context = reinterpret_cast<const EncoderCallbackContext*>(rawContext);
    *err = U_ZERO_ERROR;
    ucnv_cbFromUWriteBytes(fromArgs, context->subBytes, context->length, 0, err);
}

UConverterFromUCallback NativeConverter::GetFromUCallback(
    /* [in] */ int32_t mode)
{
    switch(mode) {
    case NativeConverter_REPORT:
        return UCNV_FROM_U_CALLBACK_STOP;
    case NativeConverter_IGNORE:
        return UCNV_FROM_U_CALLBACK_SKIP;
    case NativeConverter_REPLACE:
        return EncoderReplaceCallback;
    }
    assert(FALSE);
    return NULL;
}

void NativeConverter::DecoderIgnoreCallback(
    /* [in] */ const void*,
    /* [in] */ UConverterToUnicodeArgs*,
    /* [in] */ const char*,
    /* [in] */ int32_t,
    /* [in] */ UConverterCallbackReason,
    /* [out] */ UErrorCode* err)
{
    // The icu4c UCNV_FROM_U_CALLBACK_SKIP callback requires that the context is NULL, which is
    // never true for us.
    *err = U_ZERO_ERROR;
}

void NativeConverter::DecoderReplaceCallback(
    /* [in] */ const void* rawContext,
    /* [in] */ UConverterToUnicodeArgs* toArgs,
    /* [in] */ const char*,
    /* [in] */ int32_t,
    /* [in] */ UConverterCallbackReason,
    /* [out] */ UErrorCode* err)
{
    if (!rawContext) {
        return;
    }
    const DecoderCallbackContext* context = reinterpret_cast<const DecoderCallbackContext*>(rawContext);
    *err = U_ZERO_ERROR;
    ucnv_cbToUWriteUChars(toArgs,context->subUChars, context->length, 0, err);
}

UConverterToUCallback NativeConverter::GetToUCallback(
    /* [in] */ int32_t mode)
{
    switch (mode) {
    case NativeConverter_IGNORE: return DecoderIgnoreCallback;
    case NativeConverter_REPLACE: return DecoderReplaceCallback;
    case NativeConverter_REPORT: return UCNV_TO_U_CALLBACK_STOP;
    }
    assert(FALSE);
    return NULL;
}

void NativeConverter::CHARSET_DECODER_CALLBACK(
    /* [in] */ const void* rawContext,
    /* [in] */ UConverterToUnicodeArgs* args,
    /* [in] */ const char* codeUnits,
    /* [in] */ int32_t length,
    /* [in] */ UConverterCallbackReason reason,
    /* [out] */ UErrorCode* status)
{
    if (!rawContext) {
        return;
    }
    const DecoderCallbackContext* ctx = reinterpret_cast<const DecoderCallbackContext*>(rawContext);
    switch(reason) {
    case UCNV_UNASSIGNED:
        ctx->onUnmappableInput(ctx, args, codeUnits, length, reason, status);
        return;
    case UCNV_ILLEGAL:
    case UCNV_IRREGULAR:
        ctx->onMalformedInput(ctx, args, codeUnits, length, reason, status);
        return;
    case UCNV_CLOSE:
        delete ctx;
        return;
    default:
        *status = U_ILLEGAL_ARGUMENT_ERROR;
        return;
    }
}

ECode NativeConverter::CheckErrorCode(
        /* [in] */ UErrorCode errorCode)
{
    // const char* message = u_errorName(errorCode);
    if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
        return NOERROR;
    }

    switch (errorCode) {
    case U_ILLEGAL_ARGUMENT_ERROR:
        // return jniThrowException(env, "java/lang/IllegalArgumentException", message);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    case U_INDEX_OUTOFBOUNDS_ERROR:
    case U_BUFFER_OVERFLOW_ERROR:
        // return jniThrowException(env, "java/lang/ArrayIndexOutOfBoundsException", message);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    case U_UNSUPPORTED_ERROR:
        // return jniThrowException(env, "java/lang/UnsupportedOperationException", message);
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    default:
        // return jniThrowRuntimeException(env, message);
        return E_RUNTIME_EXCEPTION;
    }

    return E_RUNTIME_EXCEPTION;
}
