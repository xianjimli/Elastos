
#include "cmdef.h"
#include "NativeCollation.h"
#include "ElStringByteSink.h"
#include <Elastos.Utility.h>
#include <unicode/ucol.h>
#include <unicode/ucoleitr.h>
#include "UniquePtr.h"

NativeCollation::NativeCollation() {
}

static UCollator* toCollator(
    /* [in] */ Int32 address)
{
    return reinterpret_cast<UCollator*>(static_cast<uintptr_t>(address));
}

static UCollationElements* toCollationElements(
   /* [in] */ Int32 address)
{
   return reinterpret_cast<UCollationElements*>(static_cast<uintptr_t>(address));
}

/**
* Checks if an error has occurred, throwing a suitable exception if so.
* @param env JNI environment
* @param errorCode code to determine if it is an error
* @return 0 if errorCode is not an error, 1 if errorCode is an error, but the
*         creation of the exception to be thrown fails
 * @exception thrown if errorCode represents an error
*/
static ECode icu4_error(
    /* [in] */ UErrorCode errorCode)
{
    if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
        return NOERROR;
    }

    switch (errorCode) {
        case U_ILLEGAL_ARGUMENT_ERROR:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case U_INDEX_OUTOFBOUNDS_ERROR:
        case U_BUFFER_OVERFLOW_ERROR:
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        case U_UNSUPPORTED_ERROR:
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        default:
            return E_RUNTIME_EXCEPTION;
    }
}

void NativeCollation::CloseCollator(
    /* [in] */ Int32 collatoraddress)
{
    ucol_close(toCollator(collatoraddress));
}

void NativeCollation::CloseElements(
    /* [in] */ Int32 address)
{
    ucol_closeElements(toCollationElements(address));
}

Int32 NativeCollation::Compare(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ String source,
    /* [in] */ String target)
{
    return ucol_strcoll(toCollator(collatoraddress),
            UnicodeString::fromUTF8(source.string()).getBuffer(),
            UnicodeString::fromUTF8(source.string()).length(),
            UnicodeString::fromUTF8(target.string()).getBuffer(),
            UnicodeString::fromUTF8(target.string()).length());
}

ECode NativeCollation::GetAttribute(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ Int32 type,
    /* [out] */ Int32* result)
{
    UErrorCode status = U_ZERO_ERROR;
    *result = ucol_getAttribute(toCollator(collatoraddress),
            (UColAttribute)type, &status);
    return icu4_error(status);
}

ECode NativeCollation::GetCollationElementIterator(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ String source,
    /* [out] */ Int32* coleitr)
{
    UErrorCode status = U_ZERO_ERROR;
    UCollationElements* result = ucol_openElements(toCollator(collatoraddress),
            UnicodeString::fromUTF8(source.string()).getBuffer(),
            UnicodeString::fromUTF8(source.string()).length(), &status);
    FAIL_RETURN(icu4_error(status));
    *coleitr = static_cast<Int32>(reinterpret_cast<uintptr_t>(result));
    return NOERROR;
}

Int32 NativeCollation::GetMaxExpansion(
    /* [in] */ Int32 address,
    /* [in] */ Int32 order)
{
    return ucol_getMaxExpansion(toCollationElements(address), order);
}

ECode NativeCollation::GetNormalization(
    /* [in] */ Int32 collatoraddress,
    /* [out] */ Int32* result)
{
    UErrorCode status = U_ZERO_ERROR;
    *result = ucol_getAttribute(toCollator(collatoraddress), UCOL_NORMALIZATION_MODE, &status);
    return icu4_error(status);
}

ECode NativeCollation::SetNormalization(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ Int32 normalizationmode)
{
    UErrorCode status = U_ZERO_ERROR;
    ucol_setAttribute(toCollator(collatoraddress), UCOL_NORMALIZATION_MODE, UColAttributeValue(normalizationmode), &status);
    return icu4_error(status);
}

Int32 NativeCollation::GetOffset(
    /* [in] */ Int32 address)
{
    return ucol_getOffset(toCollationElements(address));
}

String NativeCollation::GetRules(
    /* [in] */ Int32 collatoraddress)
{
    int32_t length = 0;
    UnicodeString rules = ucol_getRules(toCollator(collatoraddress), &length);
    String s("");
    ElStringByteSink sink(&s);
    rules.toUTF8(sink);
    return s;
}

ArrayOf<Byte>* NativeCollation::GetSortKey(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ String source)
{
    const UCollator* collator  = toCollator(collatoraddress);
    uint8_t byteArray[128 * 2];
    UniquePtr<uint8_t[]> largerByteArray;
    uint8_t* usedByteArray = byteArray;
    const UChar* chars = (const UChar*)(source.string());
    size_t charCount = source.GetLength();
    size_t byteArraySize = ucol_getSortKey(collator, chars, charCount, usedByteArray, sizeof(byteArray) - 1);
    if (byteArraySize > sizeof(byteArray) - 1) {
        // didn't fit, try again with a larger buffer.
        largerByteArray.reset(new uint8_t[byteArraySize + 1]);
        usedByteArray = largerByteArray.get();
        byteArraySize = ucol_getSortKey(collator, chars, charCount, usedByteArray, byteArraySize);
    }
    if (byteArraySize == 0) {
        return NULL;
    }
    ArrayOf<Byte> * result = ArrayOf<Byte>::Alloc(byteArraySize);
    //env->SetByteArrayRegion(result, 0, byteArraySize, reinterpret_cast<jbyte*>(usedByteArray));
    for (size_t i = 0; i < byteArraySize; ++i) {
        (*result)[i] = usedByteArray[i];
    }
    return result;
}

ECode NativeCollation::Next(
    /* [in] */ Int32 address,
    /* [out] */ Int32* result)
{
    UErrorCode status = U_ZERO_ERROR;
    *result = ucol_next(toCollationElements(address), &status);
    return icu4_error(status);
}

ECode NativeCollation::OpenCollator(
    /* [in] */ String locale,
    /* [out] */ Int32* address)
{
    if (locale.IsNull()) {
        return 0;
    }
    UErrorCode status = U_ZERO_ERROR;
    UCollator* c = ucol_open(locale.string(), &status);
    FAIL_RETURN(icu4_error(status));
    *address = static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
    return NOERROR;
}

ECode NativeCollation::OpenCollatorFromRules(
    /* [in] */ String rules,
    /* [in] */ Int32 normalizationmode,
    /* [in] */ Int32 collationstrength,
    /* [out] */ Int32* address)
{
    UErrorCode status = U_ZERO_ERROR;
    UCollator* c = ucol_openRules(UnicodeString::fromUTF8(rules.string()).getBuffer(),
            UnicodeString::fromUTF8(rules.string()).length(),
            UColAttributeValue(normalizationmode),
            UCollationStrength(collationstrength),NULL, &status);
    FAIL_RETURN(icu4_error(status));
    *address = static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
    return NOERROR;
}

ECode NativeCollation::Previous(
    /* [in] */ Int32 address,
    /* [out] */ Int32* result)
{
    UErrorCode status = U_ZERO_ERROR;
    *result = ucol_previous(toCollationElements(address), &status);
    return icu4_error(status);
}

void NativeCollation::Reset(
    /* [in] */ Int32 address)
{
    ucol_reset(toCollationElements(address));
}

ECode NativeCollation::SafeClone(
    /* [in] */ Int32 collatoraddress,
    /* [out] */ Int32* address)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 bufferSize = U_COL_SAFECLONE_BUFFERSIZE;
    UCollator* c = ucol_safeClone(toCollator(collatoraddress), NULL, &bufferSize, &status);
    FAIL_RETURN(icu4_error(status));
    *address = static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
    return NOERROR;
}

ECode NativeCollation::SetAttribute(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ Int32 type,
    /* [in] */ Int32 value)
{
    UErrorCode status = U_ZERO_ERROR;
    ucol_setAttribute(toCollator(collatoraddress), (UColAttribute)type, (UColAttributeValue)value, &status);
    return icu4_error(status);
}

ECode NativeCollation::SetOffset(
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset)
{
    UErrorCode status = U_ZERO_ERROR;
    ucol_setOffset(toCollationElements(address), offset, &status);
    return icu4_error(status);
}

ECode NativeCollation::SetText(
    /* [in] */ Int32 address,
    /* [in] */ String source)
{
   UErrorCode status = U_ZERO_ERROR;
   ucol_setText(toCollationElements(address),
            UnicodeString::fromUTF8(source.string()).getBuffer(),
            UnicodeString::fromUTF8(source.string()).length(), &status);
   return icu4_error(status);
}
