
#ifndef __CASSETMANAGER_H__
#define __CASSETMANAGER_H__

#include <utils/AssetManager.h>
#include <utils/ResourceTypes.h>
#include "_CAssetManager.h"
#include "ext/frameworkext.h"
#include "content/StringBlock.h"
#include <content/XmlBlock.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>

using namespace Elastos::Core::Threading;

CarClass(CAssetManager)
{
    friend class AssetInputStream;
    friend class Typeface;

public:
    static const Int32 STYLE_NUM_ENTRIES = 6;
    static const Int32 STYLE_TYPE = 0;
    static const Int32 STYLE_DATA = 1;
    static const Int32 STYLE_ASSET_COOKIE = 2;
    static const Int32 STYLE_RESOURCE_ID = 3;
    static const Int32 STYLE_CHANGING_CONFIGURATIONS = 4;
    static const Int32 STYLE_DENSITY = 5;

public:
    CAssetManager();

    virtual ~CAssetManager();

public:
    CARAPI EnsureStringBlocks();

    /**
     * Return a global shared asset manager that provides access to only
     * system assets (no application assets).
     * {@hide}
     */
    static CARAPI_(CAssetManager*) GetSystem();

    CARAPI Close();

    CARAPI GetResourceText(
        /* [in] */ Int32 ident,
        /* [out] */ ICharSequence** text);

    CARAPI GetResourceBagText(
        /* [in] */ Int32 ident,
        /* [in] */ Int32 bagEntryId,
        /* [out] */ ICharSequence** text);

    CARAPI GetResourceStringArray(
        /* [in] */ Int32 id,
        /* [out] */ ArrayOf<String>** texts);

    /*package*/ CARAPI_(Boolean) GetResourceValue(
        /* [in] */ Int32 ident,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolveRefs);

    /*package*/ CARAPI GetResourceTextArray(
        /* [in] */ Int32 id,
        /* [out, callee] */ ArrayOf<ICharSequence*>** textArray);

    CARAPI Open(
        /* [in] */ const String& fileName,
        /* [out] */ IInputStream** stream);

    CARAPI OpenEx(
        /* [in] */ const String& fileName,
        /* [in] */ Int32 accessMode,
        /* [out] */ IInputStream** stream);

    CARAPI OpenFd(
        /* [in] */ const String& fileName,
        /* [out] */ IAssetFileDescriptor** fd);

    CARAPI List(
        /* [in] */ const String& path,
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI OpenNonAsset(
        /* [in] */ const String& fileName,
        /* [out] */ IInputStream** stream);

    CARAPI OpenNonAssetEx(
        /* [in] */ const String& fileName,
        /* [in] */ Int32 accessMode,
        /* [out] */ IInputStream** stream);

    CARAPI OpenNonAssetEx2(
        /* [in] */ Int32 cookie,
        /* [in] */ const String& fileName,
        /* [out] */ IInputStream** stream);

    CARAPI OpenNonAssetEx3(
        /* [in] */ Int32 cookie,
        /* [in] */ const String& fileName,
        /* [in] */ Int32 accessMode,
        /* [out] */ IInputStream** stream);

    CARAPI OpenNonAssetFd(
        /* [in] */ const String& fileName,
        /* [out] */ IAssetFileDescriptor** fd);

    CARAPI OpenNonAssetFdEx(
        /* [in] */ Int32 cookie,
        /* [in] */ const String& fileName,
        /* [out] */ IAssetFileDescriptor** fd);

    CARAPI OpenXmlResourceParser(
        /* [in] */ const String& fileName,
        /* [out] */ IXmlResourceParser** parser);

    CARAPI OpenXmlResourceParserEx(
        /* [in] */ Int32 cookie,
        /* [in] */ const String& fileName,
        /* [out] */ IXmlResourceParser** parser);

    CARAPI AddAssetPath(
        /* [in] */ const String& path,
        /* [out] */ Int32* cookie);

    CARAPI AddAssetPaths(
        /* [in] */ const ArrayOf<String>& paths,
        /* [out, callee] */ ArrayOf<Int32>** cookies);

    CARAPI IsUpToDate(
        /* [out] */ Boolean* isUpdated);

    CARAPI SetLocale(
        /* [in] */ const String& locale);

    CARAPI GetLocales(
        /* [out, callee] */ ArrayOf<String>** locales);

    CARAPI SetConfiguration(
        /* [in] */ Int32 mcc,
        /* [in] */ Int32 mnc,
        /* [in] */ const String& locale,
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 touchscreen,
        /* [in] */ Int32 density,
        /* [in] */ Int32 keyboard,
        /* [in] */ Int32 keyboardHidden,
        /* [in] */ Int32 navigation,
        /* [in] */ Int32 screenWidth,
        /* [in] */ Int32 screenHeight,
        /* [in] */ Int32 screenLayout,
        /* [in] */ Int32 uiMode,
        /* [in] */ Int32 majorVersion);

    CARAPI_(Int32) GetResourceIdentifier(
        /* [in] */ const char* type,
        /* [in] */ const char* name,
        /* [in] */ const char* defCapsule);

    CARAPI GetResourceName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    CARAPI GetResourceCapsuleName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    CARAPI GetResourceTypeName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    CARAPI GetResourceEntryName(
        /* [in] */ Int32 resid,
        /* [out] */ String* name);

    CARAPI_(Int32) GetArraySize(
        /* [in] */ Int32 resource);

    CARAPI_(Int32) RetrieveArray(
        /* [in] */ Int32 resource,
        /* [in, out] */ ArrayOf<Int32>* outValues);

    CARAPI GetCookieName(
        /* [in] */ Int32 cookie,
        /* [out] */ String* name);

    CARAPI GetGlobalAssetCount(
        /* [out] */ Int32* count);

    CARAPI GetAssetAllocations(
        /* [out] */ String* allocations);

    CARAPI GetGlobalAssetManagerCount(
        /* [out] */ Int32* count);

    CARAPI GetArrayIntResource(
        /* [in] */ Int32 arrayRes,
        /* [out] */ ArrayOf<Int32>** ints);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Boolean isSystem);

public:
    /*package*/ CARAPI_(Boolean) GetThemeValue(
        /* [in] */ android::ResTable::Theme* theme,
        /* [in] */ Int32 ident,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolveRefs);

    /*package*/ CARAPI GetPooledString(
        /* [in] */ Int32 block,
        /* [in] */ Int32 id,
        /* [out] */ ICharSequence** csq);

    /*package*/ CARAPI_(XmlBlock*) OpenXmlBlockAsset(
        /* [in] */ const String& fileName);

    /*package*/ CARAPI_(XmlBlock*) OpenXmlBlockAsset(
        /* [in] */ Int32 cookie,
        /* [in] */ const String& fileName);

    /*package*/ CARAPI_(void) XmlBlockGone(
        /* [in] */ Int32 id);

    /*package*/ CARAPI_(android::ResTable::Theme*) CreateTheme();

    /*package*/ CARAPI_(void) ReleaseTheme(
        /* [in] */ Int32 theme);

    /*package*/ static CARAPI_(Boolean) ApplyStyle(
        /* [in] */ android::ResTable::Theme* theme,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [in] */ android::ResXMLParser* xmlParser,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [in] */ const ArrayOf<Int32>& outValues,
        /* [in] */ const ArrayOf<Int32>& outIndices);

    /*package*/ Boolean RetrieveAttributes(
        /* [in] */ android::ResXMLParser* xmlParser,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [in] */ const ArrayOf<Int32>& outValues,
        /* [in] */ const ArrayOf<Int32>& outIndices);

    /*package*/ static CARAPI_(void) ApplyThemeStyle(
        /* [in] */ android::ResTable::Theme* theme,
        /* [in] */ Int32 styleRes,
        /* [in] */ Boolean force);

    /*package*/ static CARAPI_(void) CopyTheme(
        /* [in] */ android::ResTable::Theme* dest,
        /* [in] */ android::ResTable::Theme* source);

    /*package*/ static CARAPI_(Int32) LoadThemeAttributeValue(
        /* [in] */ android::ResTable::Theme* theme,
        /* [in] */ Int32 ident,
        /* [in] */ ITypedValue* outValue,
        /* [in] */ Boolean resolve);

    /*package*/ static CARAPI_(void) DumpTheme(
        /* [in] */ Int32 theme,
        /* [in] */ Int32 priority,
        /* [in] */ const String& tag,
        /* [in] */ const String& prefix);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

protected:
    CARAPI_(void) Finalize();

private:
    CARAPI MakeStringBlocks(
        /* [in] */ Boolean copyFromSystem);

    static CARAPI EnsureSystemAssets();

    CARAPI_(android::Asset*) OpenAsset(
        /* [in] */ const String& fileName,
        /* [in] */ Int32 accessMode);

    ECode ReturnParcelFileDescriptor(
        /* [in] */ android::Asset* a,
        /* [in, out] */ ArrayOf<Int64>* outOffsets,
        /* [out] */ IParcelFileDescriptor** pfd);

    CARAPI OpenAssetFd(
        /* [in] */ const String& fileName,
        /* [in, out] */ ArrayOf<Int64>* outOffsets,
        /* [out] */ IParcelFileDescriptor** pfd);

    CARAPI OpenNonAssetNative(
        /* [in] */ Int32 cookie,
        /* [in] */ const String& fileName,
        /* [in] */ Int32 mode,
        /* [out] */ android::Asset** asset);

    CARAPI OpenNonAssetFdNative(
        /* [in] */ Int32 cookie,
        /* [in] */ const String& fileName,
        /* [in] */ ArrayOf<Int64>* outOffsets,
        /* [out] */ IParcelFileDescriptor** fd);

    CARAPI_(void) DestroyAsset(
        /* [in] */ android::Asset* asset);

    CARAPI_(Int32) ReadAssetChar(
        /* [in] */ android::Asset* asset);

    CARAPI_(Int32) ReadAsset(
        /* [in] */ android::Asset* asset,
        /* [in] */ const ArrayOf<Byte>& b,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

    CARAPI_(Int64) SeekAsset(
        /* [in] */ android::Asset* asset,
        /* [in] */ Int64 offset,
        /* [in] */ Int32 whence);

    CARAPI_(Int64) GetAssetLength(
        /* [in] */ android::Asset* asset);

    CARAPI_(Int64) GetAssetRemainingLength(
        /* [in] */ android::Asset* asset);

    CARAPI_(Int32) LoadResourceValue(
        /* [in] */ Int32 ident,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolve);

    CARAPI_(Int32) LoadResourceBagValue(
        /* [in] */ Int32 ident,
        /* [in] */ Int32 bagEntryId,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolve);

    CARAPI_(Int32) GetStringBlockCount();

    CARAPI_(const android::ResStringPool*) GetNativeStringBlock(
        /* [in] */ Int32 block);

    CARAPI_(android::ResTable::Theme*) NewTheme();

    CARAPI_(void) DeleteTheme(
        /* [in] */ Int32 theme);

    CARAPI_(android::ResXMLTree*) OpenXmlAssetNative(
        /* [in] */ Int32 cookie,
        /* [in] */ const String& fileName);

    CARAPI GetArrayStringResource(
        /* [in] */ Int32 arrayResId,
        /* [out] */ ArrayOf<String>** res);

    CARAPI GetArrayStringInfo(
        /* [in] */ Int32 arrayResId,
        /* [out] */ ArrayOf<Int32>** res);

    CARAPI Init();

    CARAPI_(void) Destroy();

    CARAPI_(void) IncRefsLocked(
        /* [in] */ Int32 id);

    CARAPI_(void) DecRefsLocked(
        /* [in] */ Int32 id);

private:
    static const char* TAG;
    static const Boolean localLOGV = FALSE; // Config.LOGV || false;
    static const Boolean DEBUG_REFS = FALSE;

    static Mutex sSyncLock;
    static AutoPtr<CAssetManager> sSystem;

    AutoPtr<ITypedValue> mValue;
    ArrayOf<Int64>* mOffsets;

    // For communication with native code.
    android::AssetManager* mNative;
    Int32 mNObject;  // used by the NDK

    Elastos::List<StringBlock*>* mStringBlocks;

    Int32 mNumRefs;
    Boolean mOpen;
    HashMap<Int32, ECode> mRefStacks;
};

#endif // __CASSETMANAGER_H__
