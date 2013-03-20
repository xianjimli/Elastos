#include "ICUCollationKey.h"

PInterface ICUCollationKey::Probe(
            /* [in]  */ REIID riid)
{
    return NULL;
}

ECode ICUCollationKey::Init(
        /* [in] */ String source,
        /* [in] */ ArrayOf<Byte>* bytes)
{
    CollationKey::Init(source);
    mBytes = bytes->Clone();
    return NOERROR;
}

ICUCollationKey::~ICUCollationKey()
{
    if (mBytes != NULL) {
        ArrayOf<Byte>::Free(mBytes);
    }
}

ECode ICUCollationKey::CompareTo(
    /* [in] */ ICollationKey * other,
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);
    // Get the bytes from the other collation key.
    ArrayOf<Byte>* rhsBytes;
    if (other->Probe(EIID_ICollationKey) != NULL) {
        rhsBytes = mBytes->Clone();
    } else {
        other->ToByteArray(&rhsBytes);
    }

    if (mBytes == NULL || mBytes->GetLength() == 0) {
        if (rhsBytes == NULL || rhsBytes->GetLength() == 0) {
            *result = 0;
            return NOERROR;
        }
        *result = -1;
        return NOERROR;
    } else {
        if (rhsBytes == NULL || rhsBytes->GetLength() == 0) {
            *result = 1;
            return NOERROR;
        }
    }

    Int32 count = Math::Min(mBytes->GetLength(), rhsBytes->GetLength());
    for (Int32 i = 0; i < count; ++i) {
        Int32 s = (*mBytes)[i] & 0xFF;
        Int32 t = (*rhsBytes)[i] & 0xFF;
        if (s < t) {
            *result = -1;
            return NOERROR;
        }
        if (s > t) {
            *result = 1;
            return NOERROR;
        }
    }
    if (mBytes->GetLength() < rhsBytes->GetLength()) {
        *result = -1;
        return NOERROR;
    }
    if (mBytes->GetLength() > rhsBytes->GetLength()) {
        *result = 1;
        return NOERROR;
    }
    *result = 0;
    return NOERROR;
}

ECode ICUCollationKey::Equals(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    if (object == (IInterface*)this) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_ICollationKey) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    Int32 i;
    CompareTo((ICollationKey*) object, &i);
    *result = (i == 0) ? TRUE : FALSE;
    return NOERROR;
}

ECode ICUCollationKey::HashCode(
        /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);
    if (hashCode == 0) {
        if (mBytes != NULL && mBytes->GetLength() != 0) {
            Int32 len = mBytes->GetLength();
            Int32 inc = ((len - 32) / 32) + 1;
            for (Int32 i = 0; i < len;) {
                hashCode = (hashCode * 37) + (*mBytes)[i];
                i += inc;
            }
        }
        if (hashCode == 0) {
            hashCode = 1;
        }
    }
    *value = hashCode;
    return NOERROR;
}

ECode ICUCollationKey::ToByteArray(
        /* [out, callee] */ ArrayOf<Byte> ** array)
{
    if (mBytes != NULL && mBytes->GetLength() == 0) {
        *array = NULL;
        return NOERROR;
    }
    *array = mBytes->Clone();
    return NOERROR;
}