
#ifndef __CSIGNATURE_H__
#define __CSIGNATURE_H__

#include "_CSignature.h"

CarClass(CSignature)
{
public:
    CSignature();

    ~CSignature();

    /**
     * Encode the Signature as ASCII text.
     */
    CARAPI ToChars(
        /* [out, callee] */ ArrayOf<Char8>** text);

    /**
     * Encode the Signature as ASCII text in to an existing array.
     *
     * @param existingArray Existing char array or null.
     * @param outLen Output parameter for the number of characters written in
     * to the array.
     * @return Returns either <var>existingArray</var> if it was large enough
     * to hold the ASCII representation, or a newly created char[] array if
     * needed.
     */
    CARAPI ToCharsEx(
        /* [in] */ ArrayOf<Char8>* existingArray,
        /* [in] */ ArrayOf<Int32>* outLen,
        /* [out, callee] */ ArrayOf<Char8>** text);

    /**
     * Return the result of {@link #toChars()} as a String.  This result is
     * cached so future calls will return the same String.
     */
    CARAPI ToCharsString(
        /* [out] */ String* str);

    /**
     * @return the contents of this signature as a byte array.
     */
    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI Equals(
        /* [in] */ ISignature* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const ArrayOf<Byte>& signature);

    CARAPI constructor(
        /* [in] */ CString text);

private:
    ArrayOf<Byte>* mSignature;
    Int32 mHashCode;
    Boolean mHaveHashCode;
//    private SoftReference<String> mStringRef;
};

#endif //__CSIGNATURE_H__
