
#include "cmdef.h"
#include "ICUCollationKey.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

// {278E167C-AAE5-4e71-9F92-21837A760086}
// extern "C" const InterfaceID EIID_ICUCollationKey =
//     { 0x278e167c, 0xaae5, 0x4e71, { 0x9f, 0x92, 0x21, 0x83, 0x7a, 0x76, 0x0, 0x86 } };

ICUCollationKey::ICUCollationKey(
    /* [in] */ const String& source,
    /* [in] */ const ArrayOf<Byte>& bytes)
{
    CollationKey::Init(source);
    mBytes = bytes.Clone();
}

ICUCollationKey::~ICUCollationKey()
{
    if (mBytes != NULL) {
        ArrayOf<Byte>::Free(mBytes);
    }
}

PInterface ICUCollationKey::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ICollationKey*)this;
    }
    else if (riid == EIID_ICollationKey) {
        return (ICollationKey*)this;
    }
    // else if (riid == EIID_ICUCollationKey) {
    //     return reinterpret_cast<PInterface>((ICUCollationKey*)this);
    // }
    return NULL;
}

UInt32 ICUCollationKey::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ICUCollationKey::Release()
{
    return ElRefBase::Release();
}

ECode ICUCollationKey::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* IID)
{
    VALIDATE_NOT_NULL(IID);

    if (object == (IInterface*)(ICollationKey*)this) {
        *IID = EIID_ICollationKey;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ICUCollationKey::CompareTo(
    /* [in] */ ICollationKey* other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // Get the bytes from the other collation key.
    ArrayOf<Byte>* rhsBytes;
    if (other != NULL) {
        other->GetByteArray(&rhsBytes);
    }

    if (mBytes == NULL || mBytes->GetLength() == 0) {
        if (rhsBytes == NULL || rhsBytes->GetLength() == 0) {
            *result = 0;
            return NOERROR;
        }
        *result = -1;
        return NOERROR;
    }
    else {
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

ECode ICUCollationKey::GetSourceString(
    /* [out] */ String* sourceString)
{
    return CollationKey::GetSourceString(sourceString);
}

ECode ICUCollationKey::GetByteArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    if (mBytes != NULL && mBytes->GetLength() == 0) {
        *array = NULL;
        return NOERROR;
    }
    *array = mBytes->Clone();
    return NOERROR;
}