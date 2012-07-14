
#ifndef __ASSETINPUTSTREAM_H__
#define __ASSETINPUTSTREAM_H__

#include <utils/AssetManager.h>
#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include "content/CAssetManager.h"
#include <elastos/AutoPtr.h>

class AssetInputStream : public ElRefBase, public IAssetInputStream
{
public:
    AssetInputStream(
        /* [in] */ CAssetManager* assetManager,
        /* [in] */ android::Asset* asset);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Available(
        /* [out] */ Int32* bytes);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI GetAssetInt32(
        /* [out] */ Int32* value);

private:
    AutoPtr<CAssetManager> mAssetManager;
    android::Asset* mAsset;
    Int64 mLength;
    Int64 mMarkPos;
};

#endif // __CASSETINPUTSTREAM_H__
