
#include "CCharsetICU.h"
#include "cmdef.h"
#include "NativeConverter.h"
#include "elastos/AutoPtr.h"

ECode CCharsetICU::constructor(
    /* [in] */ const String& canonicalName,
    /* [in] */ const String& icuCanonName,
    /* [in] */ const ArrayOf<String>& aliases)
{
    mIcuCanonicalName = icuCanonName;
    return Charset::Init(canonicalName, aliases);
}

ECode CCharsetICU::NewDecoder(
    /* [out] */ ICharsetDecoder** charsetDecoder)
{
    // TODO:
    // return CharsetDecoderICU.newInstance(this, icuCanonicalName);
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetICU::NewEncoder(
    /* [out] */ ICharsetEncoder** charsetEncoder)
{
    // TODO:
    // return CharsetEncoderICU.newInstance(this, icuCanonicalName);
    return E_NOT_IMPLEMENTED;
}

ECode CCharsetICU::Contains(
    /* [in] */ ICharset* charset,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (charset == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    
    if (ICharset::Probe(charset) != NULL) {
        *result = TRUE;
        return NOERROR;
    }

    String thisName;
    String charsetName;

    Charset::Name(&thisName);
    charset->Name(&charsetName);

    return NativeConverter::Contains(thisName, charsetName, result);
}

ECode CCharsetICU::Name(
    /* [out] */ String* name)
{
    return Charset::Name(name);
}

ECode CCharsetICU::Aliases(
    /* [out, callee] */ IObjectContainer** aliases)
{
    ECode res;
    Set<String>* strings;
    res = Charset::Aliases(&strings);
    AutoPtr<IObjectContainer> result;
    if (strings != NULL)
    {
        CObjectContainer::New((IObjectContainer**)&result);
        // TODO:
        // result->Add(strings);

        *aliases = result;
    } else {
        *aliases = NULL;
    }
    return res;
}

ECode CCharsetICU::DisplayName(
    /**out**/ String* name)
{
    return Charset::DisplayName(name);
}

ECode CCharsetICU::DisplayNameEx(
        /* [in] */ ILocale* l,
        /* [out] */ String* name)
{
    return Charset::DisplayNameEx(l, name);
}

ECode CCharsetICU::IsRegistered(
    /* [out] */ Boolean* result)
{
    return Charset::IsRegistered(result);
}

ECode CCharsetICU::CanEncode(
    /* [out] */ Boolean* result)
{
    return Charset::CanEncode(result);
}

ECode CCharsetICU::Encode(
    /* [in] */ ICharBuffer* buffer,
    /* [out] */ IByteBuffer** byteBuffer)
{
    return Charset::Encode(buffer, byteBuffer);
}

ECode CCharsetICU::EncodeEx(
        /* [in] */ const String& s,
        /* [out] */ IByteBuffer** byteBuffer)
{
    return Charset::EncodeEx(s, byteBuffer);
}

ECode CCharsetICU::Decode(
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ ICharBuffer** charBuffer)
{
    return Charset::Decode(buffer, charBuffer);
}

ECode CCharsetICU::CompareTo(
    /* [in] */ ICharset* charset,
    /* [out] */ Int32* result)
{
    return Charset::CompareTo(charset, result);
}
