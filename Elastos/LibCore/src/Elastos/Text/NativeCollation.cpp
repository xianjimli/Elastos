
#include "NativeCollation.h"
#include <Elastos.Utility.h>
#include "ElStringByteSink.h"
#include <unicode/ucol.h>
//#include "UniquePtr.h"


NativeCollation::NativeCollation() {
}

/**
* Checks if an error has occurred, throwing a suitable exception if so.
* @param env JNI environment
* @param errorCode code to determine if it is an error
* @return 0 if errorCode is not an error, 1 if errorCode is an error, but the
*         creation of the exception to be thrown fails
 * @exception thrown if errorCode represents an error
*/
static ECode icu4_error(UErrorCode errorCode)
{
//    const char* message = u_errorName(errorCode);
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

Int32 NativeCollation::OpenCollator(
        /* [in] */ String locale)
{
    if (locale.string() == NULL) {
        return 0;
    } 
    UErrorCode status = U_ZERO_ERROR;
    UCollator* c = ucol_open(locale.string(), &status);
    icu4_error(status);
    return static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
}

Int32 NativeCollation::OpenCollatorFromRules(
    /* [in] */ String rules,
    /* [in] */ Int32 normalizationmode,
    /* [in] */ Int32 collationstrength)
{
    UErrorCode status = U_ZERO_ERROR;
    //UCollator* c = ucol_openRules(rules.string().getBuffer(), rules.GetLength(),
    //        UColAttributeValue(normalizationmode), UCollationStrength(collationstrength), NULL, &status);
    icu4_error(status);
    //return static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
    return 0;
}

static UCollator* toCollator(
    /* [in] */ Int32 address) {
    return reinterpret_cast<UCollator*>(static_cast<uintptr_t>(address));
}

void NativeCollation::CloseCollator(
    /* [in] */ Int32 collatoraddress)
{
    ucol_close(toCollator(collatoraddress));
}

Int32 NativeCollation::Compare(
    /* [in] */ Int32 collatoraddress, 
    /* [in] */ String source,
    /* [in] */ String target)
{
    return ucol_strcoll(toCollator(collatoraddress),
            (const UChar*)(source.string()), source.GetLength(),
            (const UChar*)(target.string()), target.GetLength());
}

Int32 NativeCollation::GetNormalization(
    /* [in] */ Int32 collatoraddress)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 result = ucol_getAttribute(toCollator(collatoraddress), UCOL_NORMALIZATION_MODE, &status);
    icu4_error(status);
    return result;
}

void NativeCollation::SetNormalization(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ Int32 normalizationmode)
{
    UErrorCode status = U_ZERO_ERROR;
    ucol_setAttribute(toCollator(collatoraddress), UCOL_NORMALIZATION_MODE, UColAttributeValue(normalizationmode), &status);
    icu4_error(status);
}

String NativeCollation::GetRules(
    /* [in] */ Int32 collatoraddress)
{
    int32_t length = 0;
    const UChar* rules = ucol_getRules(toCollator(collatoraddress), &length);
    return String((const char*)rules, length);
}

ArrayOf<Byte>* NativeCollation::GetSortKey(
    /* [in] */ Int32 collatoraddress, 
    /* [in] */ String source)
{
/*    const UCollator* collator  = toCollator(collatoraddress);
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
    return result;*/
    return NULL;
}

void NativeCollation::SetAttribute(
    /* [in] */ Int32 collatoraddress, 
    /* [in] */ Int32 type, 
    /* [in] */ Int32 value)
{
    UErrorCode status = U_ZERO_ERROR;
    ucol_setAttribute(toCollator(collatoraddress), (UColAttribute)type, (UColAttributeValue)value, &status);
    icu4_error(status);
}

Int32 NativeCollation::GetAttribute(
    /* [in] */ Int32 collatoraddress, 
    /* [in] */ Int32 type)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 result = ucol_getAttribute(toCollator(collatoraddress), (UColAttribute) type, &status);
    icu4_error(status);
    return result;
}

Int32 NativeCollation::SafeClone(
    /* [in] */ Int32 collatoraddress)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 bufferSize = U_COL_SAFECLONE_BUFFERSIZE;
    UCollator* c = ucol_safeClone(toCollator(collatoraddress), NULL, &bufferSize, &status);
    icu4_error(status);
    return static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
}

Int32 NativeCollation::GetCollationElementIterator(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ String source)
{
/*    UErrorCode status = U_ZERO_ERROR;
    UCollationElements* result = ucol_openElements(toCollator(collatoraddress),
            (const UChar*)(source.string()), source.GetLength(), &status);
    icu4_error(status);
    return static_cast<Int32>(reinterpret_cast<uintptr_t>(result));*/
    return 0;
}

//static UCollationElements* toCollationElements(
//    /* [in] */ Int32 address) {
//    return reinterpret_cast<UCollationElements*>(static_cast<uintptr_t>(address));
//}

void NativeCollation::CloseElements(
    /* [in] */ Int32 address)
{
    //ucol_closeElements(toCollationElements(address));
}

void NativeCollation::Reset(
    /* [in] */ Int32 address)
{
    //ucol_reset(toCollationElements(address));
}


Int32 NativeCollation::Next(
    /* [in] */ Int32 address)
{
//    UErrorCode status = U_ZERO_ERROR;
//    Int32 result = ucol_next(toCollationElements(address), &status);
//    icu4_error(status);
//    return result;
    return 0;
}


Int32 NativeCollation::Previous(
    /* [in] */ Int32 address)
{
//    UErrorCode status = U_ZERO_ERROR;
//    Int32 result = ucol_previous(toCollationElements(address), &status);
//    icu4_error(status);
//    return result;
    return 0;
}

Int32 NativeCollation::GetMaxExpansion(
    /* [in] */ Int32 address, 
    /* [in] */ Int32 order)
{
    //return ucol_getMaxExpansion(toCollationElements(address), order);
    return 0;
}

void NativeCollation::SetText(
    /* [in] */ Int32 address, 
    /* [in] */ String source)
{
//    UErrorCode status = U_ZERO_ERROR;
//    ucol_setText(toCollationElements(address),
//            (const UChar*)(source.string()), source.GetLength(), &status);
//    icu4_error(status);
}

Int32 NativeCollation::GetOffset(
    /* [in] */ Int32 address)
{
    //return ucol_getOffset(toCollationElements(address));
    return 0;
}

void NativeCollation::SetOffset(
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset)
{
    UErrorCode status = U_ZERO_ERROR;
    //ucol_setOffset(toCollationElements(address), offset, &status);
    icu4_error(status);
}