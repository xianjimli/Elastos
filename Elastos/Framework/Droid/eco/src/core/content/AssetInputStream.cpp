
#include "content/AssetInputStream.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

AssetInputStream::AssetInputStream(
    /* [in] */ CAssetManager* assetManager,
    /* [in] */ android::Asset* asset)
    : mAssetManager(assetManager)
    , mAsset(asset)
{
    assert(mAsset != NULL);
    mLength = mAssetManager->GetAssetLength(asset);
}

PInterface AssetInputStream::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IAssetInputStream*)this;
    }
    else if (riid == EIID_IAssetInputStream) {
        return (IAssetInputStream*)this;
    }

    return NULL;
}

UInt32 AssetInputStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AssetInputStream::Release()
{
    return ElRefBase::Release();
}

ECode AssetInputStream::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}


ECode AssetInputStream::Available(
    /* [out] */ Int32* bytes)
{
    VALIDATE_NOT_NULL(bytes);

    Int64 len = mAssetManager->GetAssetRemainingLength(mAsset);
    if (len > Math::INT32_MAX_VALUE) {
        *bytes = Math::INT32_MAX_VALUE;
    }
    else {
        *bytes = (Int32)len;
    }
    return NOERROR;
}

ECode AssetInputStream::Close()
{
    Mutex::Autolock lock(mAssetManager->_m_syncLock);

    if (mAsset != NULL) {
        mAssetManager->DestroyAsset(mAsset);
        mAsset = NULL;
//        decRefsLocked(hashCode());
    }
    return NOERROR;
}

ECode AssetInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    mMarkPos = mAssetManager->SeekAsset(mAsset, 0, 0);
    return NOERROR;
}

ECode AssetInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = TRUE;
    return NOERROR;
}

ECode AssetInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mAssetManager->ReadAssetChar(mAsset);
    return NOERROR;
}

ECode AssetInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    Int32 len = mAssetManager->ReadAsset(mAsset, *buffer,
        0, buffer->GetLength());
    *number = len;
    if (len == 0) {
        return E_OUT_OF_STREAM;
    }
    else if (len == -1) {
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode AssetInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    Int32 len = mAssetManager->ReadAsset(mAsset, *buffer,
            offset, length);
    *number = len;
    if (len == 0) {
        return E_OUT_OF_STREAM;
    }
    else if (len == -1) {
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode AssetInputStream::Reset()
{
    mAssetManager->SeekAsset(mAsset, mMarkPos, -1);
    return NOERROR;
}

ECode AssetInputStream::Skip(
    /* [in] */ Int64 length,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    Int64 pos = mAssetManager->SeekAsset(mAsset, 0, 0);
    if ((pos + length) > mLength) {
        length = mLength - pos;
    }
    if (length > 0) {
        mAssetManager->SeekAsset(mAsset, length, 0);
    }
    *number = length;
    return NOERROR;
}

ECode AssetInputStream::GetAssetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = (Int32)mAsset;
    return NOERROR;
}

