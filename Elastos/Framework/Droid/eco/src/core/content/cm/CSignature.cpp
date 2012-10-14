
#include "ext/frameworkdef.h"
#include "content/cm/CSignature.h"


CSignature::CSignature()
    : mHaveHashCode(FALSE)
{}

CSignature::~CSignature()
{
    ArrayOf<Byte>::Free(mSignature);
}

ECode CSignature::ToChars(
    /* [out, callee] */ ArrayOf<Char8>** text)
{
    return ToCharsEx(NULL, NULL, text);
}

ECode CSignature::ToCharsEx(
    /* [in] */ ArrayOf<Char8>* existingArray,
    /* [in] */ ArrayOf<Int32>* outLen,
    /* [out, callee] */ ArrayOf<Char8>** text)
{
    VALIDATE_NOT_NULL(text);

    ArrayOf<Byte>* sig = mSignature;
    const Int32 N = sig->GetLength();
    const Int32 N2 = N * 2;
    *text = existingArray == NULL || N2 > existingArray->GetLength()
            ? ArrayOf<Char8>::Alloc(N2) : existingArray->Clone();
    for (Int32 j=0; j < N; j++) {
        //todo: check, UTF-8 encoding, not UTF-16
        Byte v = (*sig)[j];
        Int32 d = (v >> 4) & 0xf;
        (**text)[j * 2] = (char)(d >= 10 ? ('a' + d - 10) : ('0' + d));
        d = v & 0xf;
        (**text)[j * 2 + 1] = (char)(d >= 10 ? ('a' + d - 10) : ('0' + d));
    }
    if (outLen != NULL) (*outLen)[0] = N;
    return NOERROR;
}

ECode CSignature::ToCharsString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

//    String str = mStringRef == null ? null : mStringRef.get();
//    if (str != null) {
//        return str;
//    }
//    str = new String(toChars());
//    mStringRef = new SoftReference<String>(str);
//    return str;
    ArrayOf<Char8>* text;
    ToChars(&text);
    *str = text->GetPayload();
    ArrayOf<Char8>::Free(text);
    return NOERROR;
}

ECode CSignature::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);

    *bytes = ArrayOf<Byte>::Alloc(mSignature->GetLength());
    memcpy((*bytes)->GetPayload(), mSignature->GetPayload(), mSignature->GetLength());
    return NOERROR;
}

ECode CSignature::Equals(
    /* [in] */ ISignature* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);

//    try {
    if (obj != NULL) {
        CSignature* other = (CSignature*)obj;
        if (mSignature->GetLength() == other->mSignature->GetLength()) {
            *isEqual = memcmp(mSignature->GetPayload(),
                    other->mSignature->GetPayload(), mSignature->GetLength()) == 0;
            return NOERROR;
        }
    }
//    } catch (ClassCastException e) {
//    }
    *isEqual = FALSE;
    return NOERROR;
}

ECode CSignature::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    if (mHaveHashCode) {
        *hashCode = mHashCode;
        return NOERROR;
    }
//    mHashCode = Arrays.hashCode(mSignature);
    mHaveHashCode = TRUE;
    *hashCode = mHashCode;
    return NOERROR;;
}

ECode CSignature::ReadFromParcel(
    /* [in] */ IParcel* source)
{
//    mSignature = source.createByteArray();
    return E_NOT_IMPLEMENTED;
}

ECode CSignature::WriteToParcel(
    /* [in] */ IParcel* dest)
{
//    dest.writeByteArray(mSignature);
    return E_NOT_IMPLEMENTED;
}

ECode CSignature::constructor(
    /* [in] */ const ArrayOf<Byte>& signature)
{
    mSignature = signature.Clone();
    return NOERROR;
}

ECode CSignature::constructor(
    /* [in] */ CString text)
{
    const Int32 N = text.GetCharCount() / 2;
    ArrayOf<Byte>* sig = ArrayOf<Byte>::Alloc(N);
    for (Int32 i = 0; i < N; i++) {
        //todo: check, UTF-8 encoding, not UTF-16
        Char32 c = text.GetChar(i * 2);
        Byte b = (Byte)(
                (c >= 'a' ? (c - 'a' + 10) : (c - '0')) << 4);
        c = text.GetChar(i * 2 + 1);
        b |= (Byte)(c >= 'a' ? (c - 'a' + 10) : (c - '0'));
        (*sig)[i] = b;
    }
    mSignature = sig;
    return NOERROR;
}
