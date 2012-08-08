
#include <utils/Errors.h>
#include <utils/Asset.h>
#include "content/CAssetManager.h"
#include "content/AssetInputStream.h"
#include "content/CAssetFileDescriptor.h"
#include "content/XmlBlock.h"
#include "os/CParcelFileDescriptor.h"
#include "utils/CTypedValue.h"
#include <ext/frameworkerr.h>
#include <ext/frameworkdef.h>
#include <Logger.h>
#include <StringBuffer.h>
#include <assert.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

#define CTYPEDVALUE(x) ((CTypedValue*)x.Get())

#define DEBUG_STYLES(x) //x

const Int32 CAssetManager::STYLE_NUM_ENTRIES;
const Int32 CAssetManager::STYLE_TYPE;
const Int32 CAssetManager::STYLE_DATA;
const Int32 CAssetManager::STYLE_ASSET_COOKIE;
const Int32 CAssetManager::STYLE_RESOURCE_ID;
const Int32 CAssetManager::STYLE_CHANGING_CONFIGURATIONS;
const Int32 CAssetManager::STYLE_DENSITY;
const char* CAssetManager::TAG = "CAssetManager";
const Boolean CAssetManager::localLOGV;
const Boolean CAssetManager::DEBUG_REFS;

Mutex CAssetManager::sSyncLock;
AutoPtr<CAssetManager> CAssetManager::sSystem;

CAssetManager::CAssetManager()
    : mNative(0)
    , mNObject(0)
    , mNumRefs(1)
    , mOpen(TRUE)
{
    CTypedValue::New((ITypedValue**)&mValue);
    mOffsets = ArrayOf<Int64>::Alloc(2);
    assert(mValue.Get());
    assert(mOffsets);
}

CAssetManager::~CAssetManager()
{
    ArrayOf<Int64>::Free(mOffsets);
}

ECode CAssetManager::constructor()
{
    Mutex::Autolock lock(_m_syncLock);

    if (DEBUG_REFS) {
        mNumRefs = 0;
        Int32 code;
        this->GetHashCode(&code);
        IncRefsLocked(code);
    }
    ECode ec = Init();
    if (FAILED(ec)) return ec;
    if (localLOGV) {
        Logger::V(TAG, StringBuffer("New asset manager: ") + (Int32)this);
    }
    return EnsureSystemAssets();
}

ECode CAssetManager::EnsureSystemAssets()
{
    Mutex::Autolock lock(sSyncLock);

    if (sSystem == NULL) {
        AutoPtr<CAssetManager> system;
        ECode ec = CAssetManager::NewByFriend(TRUE, (CAssetManager**)&system);
        if (FAILED(ec)) return ec;
        system->MakeStringBlocks(FALSE);
        sSystem = system;
    }
    return NOERROR;
}

ECode CAssetManager::constructor(
    /* [in] */ Boolean isSystem)
{
    if (DEBUG_REFS) {
        Mutex::Autolock lock(_m_syncLock);

        mNumRefs = 0;
        Int32 code;
        this->GetHashCode(&code);
        IncRefsLocked(code);
    }
    ECode ec = Init();
    if (localLOGV) {
        Logger::V(TAG, "New asset manager: " + (Int32)this);
    }
    return ec;
}

CAssetManager* CAssetManager::GetSystem()
{
    EnsureSystemAssets();
    return sSystem.Get();
}

ECode CAssetManager::Close()
{
    Mutex::Autolock lock(_m_syncLock);

    //System.out.println("Release: num=" + mNumRefs
    //                   + ", released=" + mReleased);
    if (mOpen) {
        mOpen = FALSE;
        Int32 code;
        this->GetHashCode(&code);
        DecRefsLocked(code);
    }
    return NOERROR;
}

ECode CAssetManager::GetResourceText(
    /* [in] */ Int32 ident,
    /* [out] */ ICharSequence** text)
{
    if (!text) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(_m_syncLock);

    Int32 block = LoadResourceValue(ident, mValue.Get(), TRUE);
    if (block >= 0) {
        if (CTYPEDVALUE(mValue)->mType == TypedValue_TYPE_STRING) {
            return (*mStringBlocks)[block]->Get(CTYPEDVALUE(mValue)->mData, text);
        }
        mValue->CoerceToString(text);
        return NOERROR;
    }
    *text = NULL;
    return NOERROR;
}

ECode CAssetManager::GetResourceBagText(
    /* [in] */ Int32 ident,
    /* [in] */ Int32 bagEntryId,
    /* [out] */ ICharSequence** text)
{
    if (!text) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(_m_syncLock);

    Int32 block = LoadResourceBagValue(ident, bagEntryId, mValue.Get(), TRUE);
    if (block >= 0) {
        if (CTYPEDVALUE(mValue)->mType == TypedValue_TYPE_STRING) {
            return (*mStringBlocks)[block]->Get(CTYPEDVALUE(mValue)->mData, text);
        }
        mValue->CoerceToString(text);
        return NOERROR;
    }
    *text = NULL;
    return NOERROR;
}

ECode CAssetManager::GetResourceStringArray(
    /* [in] */ Int32 id,
    /* [out] */ ArrayOf<String>** texts)
{
    return GetArrayStringResource(id, texts);
}

Boolean CAssetManager::GetResourceValue(
    /* [in] */ Int32 ident,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs)
{
    assert(outValue);

    Int32 block = LoadResourceValue(ident, outValue, resolveRefs);
    if (block >= 0) {
        if (((CTypedValue*)outValue)->mType != TypedValue_TYPE_STRING) {
            return TRUE;
        }
        (*mStringBlocks)[block]->Get(
                ((CTypedValue*)outValue)->mData,
                (ICharSequence**)&(((CTypedValue*)outValue)->mString));
        return TRUE;
    }
    return FALSE;
}

ECode CAssetManager::GetResourceTextArray(
    /* [in] */ Int32 id,
    /* [out, callee] */ ArrayOf<ICharSequence*>** textArray)
{
//    ArrayOf<Int32>* rawInfoArray = GetArrayStringInfo(id);
//    Int32 rawInfoArrayLen = rawInfoArray->GetLength();
////	    const Int32 infoArrayLen = rawInfoArrayLen / 2;
//    Int32 block;
//    Int32 index;
//    // TODO: must support two dimension array here.
////	    CharSequence[] retArray = new CharSequence[infoArrayLen];
//    for (Int32 i = 0, j = 0; i < rawInfoArrayLen; i = i + 2, j++) {
//        block = (*rawInfoArray)[i];
//        index = (*rawInfoArray)[i + 1];
////	        retArray[j] = index >= 0 ? mStringBlocks[block].get(index) : null;
//    }
////	    return retArray;
//    return NULL;
    return E_NOT_IMPLEMENTED;
}

Boolean CAssetManager::GetThemeValue(
    /* [in] */ android::ResTable::Theme* theme,
    /* [in] */ Int32 ident,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs)
{
    assert(outValue);

    Int32 block = LoadThemeAttributeValue(theme, ident, outValue, resolveRefs);
    if (block < 0) return FALSE;

    if (((CTypedValue*)outValue)->mType != TypedValue_TYPE_STRING) {
        return TRUE;
    }
    Elastos::List<StringBlock*>* blocks = mStringBlocks;
    if (blocks == NULL) {
        EnsureStringBlocks();
    }
    ECode ec = (*blocks)[block]->Get(
        ((CTypedValue*)outValue)->mData,
        (ICharSequence**)&((CTypedValue*)outValue)->mString);
    if (FAILED(ec)) return FALSE;
    return TRUE;
}

ECode CAssetManager::EnsureStringBlocks()
{
    if (mStringBlocks == NULL) {
        Mutex::Autolock lock(_m_syncLock);

        if (mStringBlocks == NULL) {
            MakeStringBlocks(TRUE);
        }
    }
    return NOERROR;
}

ECode CAssetManager::MakeStringBlocks(
    /* [in] */ Boolean copyFromSystem)
{
    Int32 sysNum = copyFromSystem ? sSystem->mStringBlocks->GetSize() : 0;
    Int32 num = GetStringBlockCount();

    mStringBlocks = new Elastos::List<StringBlock*>(num);
    if (!mStringBlocks) return E_OUT_OF_MEMORY;

    if (localLOGV) {
        Logger::V(TAG, StringBuffer("Making string blocks for ") + (Int32)this + ": " + num);
    }

    Elastos::List<StringBlock*>::Iterator it1 = mStringBlocks->Begin();
    Elastos::List<StringBlock*>::Iterator it2;
    if (copyFromSystem) {
        it2 = sSystem->mStringBlocks->Begin();
    }
    for (Int32 i = 0; i < num; i++, it1++) {
        if (i < sysNum) {
            *it1 = *it2++;
        } else {
            *it1 = new StringBlock(GetNativeStringBlock(i), TRUE);
            if (!*it1) {
                delete mStringBlocks;
                mStringBlocks = NULL;
                return E_OUT_OF_MEMORY;
            }
        }
    }

    return NOERROR;
}

ECode CAssetManager::GetPooledString(
    /* [in] */ Int32 block,
    /* [in] */ Int32 id,
    /* [out] */ ICharSequence** csq)
{
    assert(csq);

    //System.out.println("Get pooled: block=" + block
    //                   + ", id=#" + Integer.toHexString(id)
    //                   + ", blocks=" + mStringBlocks);
    return (*mStringBlocks)[block-1]->Get(id, csq);
}

ECode CAssetManager::Open(
    /* [in] */ const String& fileName,
    /* [out] */ IInputStream** stream)
{
    return OpenEx(fileName, AssetManager_ACCESS_STREAMING, stream);
}

ECode CAssetManager::OpenEx(
    /* [in] */ const String& fileName,
    /* [in] */ Int32 accessMode,
    /* [out] */ IInputStream** stream)
{
    if (!stream) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(_m_syncLock);

    if (!mOpen) {
        Logger::E(TAG, "Assetmanager has been closed");
        *stream = NULL;
        return E_RUNTIME_EXCEPTION;
    }

    android::Asset* asset = OpenAsset(fileName, accessMode);
    if (asset == NULL) {
        Logger::E(TAG, StringBuffer("Asset file: ") + (const char*)fileName);
        *stream = NULL;
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    *stream = new AssetInputStream(this, asset);
    if (*stream == NULL) {
        DestroyAsset(asset);
        return E_OUT_OF_MEMORY_ERROR;
    }
    IncRefsLocked((Int32)*stream);
    (*stream)->AddRef();
    return NOERROR;
}

ECode CAssetManager::OpenFd(
    /* [in] */ const String& fileName,
    /* [out] */ IAssetFileDescriptor** fd)
{
    if (!fd) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(_m_syncLock);

    if (!mOpen) {
        Logger::E(TAG, "Assetmanager has been closed");
        *fd = NULL;
        return E_RUNTIME_EXCEPTION;
    }

    ECode ec = NOERROR;
    AutoPtr<IParcelFileDescriptor> pfd;

    ec = OpenAssetFd(fileName, mOffsets, (IParcelFileDescriptor**)&pfd);
    if (FAILED(ec)) {
        Logger::E(TAG, StringBuffer("Asset file: ") + fileName);
        *fd = NULL;
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    return CAssetFileDescriptor::New(pfd, (*mOffsets)[0], (*mOffsets)[1], fd);
}

ECode CAssetManager::List(
    /* [in] */ const String& path,
    /* [out, callee] */ ArrayOf<String>** names)
{
    if (!names) {
        return E_INVALID_ARGUMENT;
    }

    if (path.IsNull()) {
        Logger::E(TAG, "fileName");
        *names = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    android::AssetDir* dir = mNative->openDir((const char*)path);

    if (dir == NULL) {
        Logger::E(TAG, (const char*)path);
        *names = NULL;
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    size_t N = dir->getFileCount();

    *names = ArrayOf<String>::Alloc(N);
    if (!*names) {
        delete dir;
        return E_OUT_OF_MEMORY;
    }

    for (size_t i = 0; i < N; i++) {
        const android::String8& name = dir->getFileName(i);
        (**names)[i] = (const char*)name;
        if ((**names)[i].IsNull()) {
            delete dir;
            //todo: maybe exist memory leak
            ArrayOf<String>::Free(*names);
            *names = NULL;
            return E_OUT_OF_MEMORY;
        }
    }

    delete dir;
    return NOERROR;
}

ECode CAssetManager::OpenNonAsset(
    /* [in] */ const String& fileName,
    /* [out] */ IInputStream** stream)
{
    return OpenNonAssetEx3(0, fileName, AssetManager_ACCESS_STREAMING, stream);
}

ECode CAssetManager::OpenNonAssetEx(
    /* [in] */ const String& fileName,
    /* [in] */ Int32 accessMode,
    /* [out] */ IInputStream** stream)
{
    return OpenNonAssetEx3(0, fileName, accessMode, stream);
}

ECode CAssetManager::OpenNonAssetEx2(
    /* [in] */ Int32 cookie,
    /* [in] */ const String& fileName,
    /* [out] */ IInputStream** stream)
{
    return OpenNonAssetEx3(cookie, fileName, AssetManager_ACCESS_STREAMING, stream);
}

ECode CAssetManager::OpenNonAssetEx3(
    /* [in] */ Int32 cookie,
    /* [in] */ const String& fileName,
    /* [in] */ Int32 accessMode,
    /* [out] */ IInputStream** stream)
{
    if (!stream) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(_m_syncLock);

    if (!mOpen) {
        Logger::E(TAG, "Assetmanager has been closed");
        *stream = NULL;
        return E_RUNTIME_EXCEPTION;
    }

    ECode ec = NOERROR;
    android::Asset* asset = NULL;

    ec = OpenNonAssetNative(cookie, fileName, accessMode, &asset);
    if (FAILED(ec)) {
        Logger::E(TAG, StringBuffer("Asset absolute file: ") + (const char*)fileName);
        *stream = NULL;
        return ec;
    }

    AutoPtr<IAssetInputStream> res = new AssetInputStream(this, asset);
    if (res == NULL) {
        DestroyAsset(asset);
        return E_OUT_OF_MEMORY_ERROR;
    }

    *stream = (IInputStream*)res.Get();
    (*stream)->AddRef();
    IncRefsLocked((Int32)*stream);
    return NOERROR;
}

ECode CAssetManager::OpenNonAssetFd(
    /* [in] */ const String& fileName,
    /* [out] */ IAssetFileDescriptor** fd)
{
    return OpenNonAssetFdEx(0, fileName, fd);
}

ECode CAssetManager::OpenNonAssetFdEx(
    /* [in] */ Int32 cookie,
    /* [in] */ const String& fileName,
    /* [out] */ IAssetFileDescriptor** fd)
{
    if (!fd) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(_m_syncLock);

    if (!mOpen) {
        Logger::E(TAG, "Assetmanager has been closed");
        return E_RUNTIME_EXCEPTION;
    }

    ECode ec = NOERROR;
    AutoPtr<IParcelFileDescriptor> pfd;
    ec = OpenNonAssetFdNative(cookie, fileName, mOffsets, (IParcelFileDescriptor**)&pfd);
    if (FAILED(ec)) {
        Logger::E(TAG, StringBuffer("Asset absolute file: ") + (const char*)fileName);
        return E_DOES_NOT_EXIST;
    }

    return CAssetFileDescriptor::New(pfd.Get(), (*mOffsets)[0], (*mOffsets)[1], fd);
}

ECode CAssetManager::OpenXmlResourceParser(
    /* [in] */ const String& fileName,
    /* [out] */ IXmlResourceParser** parser)
{
    return OpenXmlResourceParserEx(0, fileName, parser);
}

ECode CAssetManager::OpenXmlResourceParserEx(
    /* [in] */ Int32 cookie,
    /* [in] */ const String& fileName,
    /* [out] */ IXmlResourceParser** parser)
{
    VALIDATE_NOT_NULL(parser);

    XmlBlock* block = OpenXmlBlockAsset(cookie, fileName);
    if (!block) {
        *parser = NULL;
        return E_OUT_OF_MEMORY;
    }

    AutoPtr<IXmlResourceParser> rp;
    block->NewParser((IXmlResourceParser**)&rp);
    block->Close();
    if (!rp) {
        delete block;
        *parser = NULL;
        return E_OUT_OF_MEMORY;
    }

    *parser = rp.Get();
    (*parser)->AddRef();
    return NOERROR;
}

XmlBlock* CAssetManager::OpenXmlBlockAsset(
    /* [in] */ const String& fileName)
{
    return OpenXmlBlockAsset(0, fileName);
}

XmlBlock* CAssetManager::OpenXmlBlockAsset(
    /* [in] */ Int32 cookie,
    /* [in] */ const String& fileName)
{
    Mutex::Autolock lock(_m_syncLock);

    if (!mOpen) {
        Logger::E(TAG, "Assetmanager has been closed");
        // E_RUNTIME_EXCEPTION
        return NULL;
    }
    android::ResXMLTree* xmlBlock = OpenXmlAssetNative(cookie, fileName);
    if (xmlBlock != NULL) {
        XmlBlock* block = new XmlBlock((IAssetManager*)this, xmlBlock);
        Int32 code;
        block->GetHashCode(&code);
        IncRefsLocked(code);
        return block;
    }
    Logger::E(TAG, StringBuffer("Asset XML file: ") + fileName);
    // E_FILE_NOT_FOUND_EXCEPTION
    return NULL;
}

void CAssetManager::XmlBlockGone(
    /* [in] */ Int32 id)
{
    Mutex::Autolock lock(_m_syncLock);

    DecRefsLocked(id);
}

android::ResTable::Theme* CAssetManager::CreateTheme()
{
    Mutex::Autolock lock(_m_syncLock);

    if (!mOpen) {
//	        throw new RuntimeException("Assetmanager has been closed");
        return NULL;
    }
    android::ResTable::Theme* res = NewTheme();
    IncRefsLocked((Int32)res);
    return res;
}

void CAssetManager::ReleaseTheme(
    /* [in] */ Int32 theme)
{
    Mutex::Autolock lock(_m_syncLock);

    DeleteTheme(theme);
    DecRefsLocked(theme);
}

void CAssetManager::Finalize()
{
//	    try {
//	        if (DEBUG_REFS && mNumRefs != 0) {
//	            Log.w(TAG, "AssetManager " + this
//	                    + " finalized with non-zero refs: " + mNumRefs);
//	            if (mRefStacks != null) {
//	                for (RuntimeException e : mRefStacks.values()) {
//	                    Log.w(TAG, "Reference from here", e);
//	                }
//	            }
//	        }
//	        destroy();
//	    } finally {
//	        super.finalize();
//	    }
}

ECode CAssetManager::AddAssetPath(
    /* [in] */ const String& path,
    /* [out] */ Int32* cookie)
{
    if (!cookie) {
        return E_INVALID_ARGUMENT;
    }

    if (path.IsNull()) {
        Logger::E(TAG, "path");
        *cookie = 0;
        return E_NULL_POINTER_EXCEPTION;
    }

    void* c = NULL;
    bool res = mNative->addAssetPath(android::String8((const char*)path), &c);

    *cookie = (res) ? (Int32)c : 0;
    return NOERROR;
}

ECode CAssetManager::AddAssetPaths(
    /* [in] */ const ArrayOf<String>& paths,
    /* [out, callee] */ ArrayOf<Int32>** cookies)
{
    if (!cookies) {
        return E_INVALID_ARGUMENT;
    }

    if (paths.GetLength() == 0) {
        *cookies = NULL;
        return E_INVALID_ARGUMENT;
    }

    ECode ec = NOERROR;
    ArrayOf<Int32>* cs = ArrayOf<Int32>::Alloc(paths.GetLength());
    if (!cs) {
        *cookies = NULL;
        return E_OUT_OF_MEMORY;
    }

    for (Int32 i = 0; i < paths.GetLength(); i++) {
        ec = AddAssetPath(paths[i], &(*cs)[i]);
        if (FAILED(ec)) {
            ArrayOf<Int32>::Free(cs);
            *cookies = NULL;
            return ec;
        }
    }

    *cookies = cs;
    return NOERROR;
}

ECode CAssetManager::IsUpToDate(
    /* [out] */ Boolean* isUpdated)
{
    if (!isUpdated) {
        return E_INVALID_ARGUMENT;
    }
    *isUpdated = mNative->isUpToDate() ? TRUE : FALSE;
    return NOERROR;
}

ECode CAssetManager::SetLocale(
    /* [in] */ const String& locale)
{
    if (locale.IsNull()) {
        return E_INVALID_ARGUMENT;
    }
    mNative->setLocale((const char*)locale);
    return NOERROR;
}

ECode CAssetManager::GetLocales(
    /* [out, callee] */ ArrayOf<String>** locales)
{
    if (!locales) {
        return E_INVALID_ARGUMENT;
    }

    android::Vector<android::String8> locs;
    mNative->getLocales(&locs);
    const int N = locs.size();

    *locales = ArrayOf<String>::Alloc(N);
    if (!*locales) {
        return E_OUT_OF_MEMORY;
    }

    for (Int32 i = 0; i < N; i++) {
        (**locales)[i] = (const char*)locs[i].string();
        if ((**locales)[i].IsNull()) {
            //todo: maybe exist memory leak
            ArrayOf<String>::Free(*locales);
            *locales = NULL;
            return E_OUT_OF_MEMORY;
        }
    }

    return NOERROR;
}

ECode CAssetManager::SetConfiguration(
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
    /* [in] */ Int32 majorVersion)
{
    android::ResTable_config config;
    memset(&config, 0, sizeof(config));

    const char* locale8 = !locale.IsNull() ? (const char*)locale : NULL;

    config.mcc = (uint16_t)mcc;
    config.mnc = (uint16_t)mnc;
    config.orientation = (uint8_t)orientation;
    config.touchscreen = (uint8_t)touchscreen;
    config.density = (uint16_t)density;
    config.keyboard = (uint8_t)keyboard;
    config.inputFlags = (uint8_t)keyboardHidden;
    config.navigation = (uint8_t)navigation;
    config.screenWidth = (uint16_t)screenWidth;
    config.screenHeight = (uint16_t)screenHeight;
    config.screenLayout = (uint8_t)screenLayout;
    config.uiMode = (uint8_t)uiMode;
    config.sdkVersion = (uint16_t)majorVersion;
    config.minorVersion = 0;
    mNative->setConfiguration(config, locale8);
    return NOERROR;
}

Int32 CAssetManager::GetResourceIdentifier(
    /* [in] */ const char* type,
    /* [in] */ const char* name,
    /* [in] */ const char* defCapsule)
{
    if (name == NULL) {
        Logger::E(TAG, "name");
        return 0;
    }

    const android::String16 n16(name);
    const char16_t* name16 = (const char16_t*)n16;
    size_t nameLen = n16.size();
    const android::String16 t16(type);
    const char16_t* defType16 = (const char16_t*)t16;
    size_t defTypeLen = t16.size();
    const android::String16 c16(defCapsule);
    const char16_t* defCapsule16 = (const char16_t*)c16;
    size_t defCapsuleLen = c16.size();

    return (Int32)mNative->getResources().identifierForName(
        name16, nameLen, defType16, defTypeLen, defCapsule16, defCapsuleLen);
}

ECode CAssetManager::GetResourceName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    android::ResTable::resource_name n;
    if (!mNative->getResources().getResourceName(resid, &n)) {
        *name = NULL;
        return E_FILE_NOT_FOUND;
    }

    android::String16 str16;
    if (n.package != NULL) {
        str16.setTo(n.package, n.packageLen);
    }
    if (n.type != NULL) {
        if (str16.size() > 0) {
            char16_t div = ':';
            str16.append(&div, 1);
        }
        str16.append(n.type, n.typeLen);
    }
    if (n.name != NULL) {
        if (str16.size() > 0) {
            char16_t div = '/';
            str16.append(&div, 1);
        }
        str16.append(n.name, n.nameLen);
    }

    android::String8 str8(str16);
    *name = str8.string();
    return (*name).IsNull() ? E_OUT_OF_MEMORY : NOERROR;
}

ECode CAssetManager::GetResourceCapsuleName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    android::ResTable::resource_name n;
    if (!mNative->getResources().getResourceName(resid, &n)
        || n.package == NULL) {
        *name = NULL;
        return E_DOES_NOT_EXIST;
    }

    android::String8 str8(n.package);
    *name = str8.string();
    return (*name).IsNull() ? E_OUT_OF_MEMORY : NOERROR;
}

ECode CAssetManager::GetResourceTypeName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    android::ResTable::resource_name n;
    if (!mNative->getResources().getResourceName(resid, &n)
        || n.type == NULL) {
        *name = NULL;
        return E_DOES_NOT_EXIST;
    }

    android::String8 str8(n.type);
    *name = str8.string();
    return (*name).IsNull() ? E_OUT_OF_MEMORY : NOERROR;
}

ECode CAssetManager::GetResourceEntryName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    android::ResTable::resource_name n;
    if (!mNative->getResources().getResourceName(resid, &n)
        || n.name == NULL) {
        *name = NULL;
        return E_DOES_NOT_EXIST;
    }

    android::String8 str8(n.name);
    *name = str8.string();
    return (*name).IsNull() ? E_OUT_OF_MEMORY : NOERROR;
}

android::Asset* CAssetManager::OpenAsset(
    /* [in] */ const String& fileName,
    /* [in] */ Int32 mode)
{
    Logger::V(TAG, StringBuffer("openAsset in ") + (Int32)mNative);

    if (fileName.IsNull()) {
        Logger::E(TAG, "fileName");
        return NULL;
    }

    if (mode != android::Asset::ACCESS_UNKNOWN && mode != android::Asset::ACCESS_RANDOM
        && mode != android::Asset::ACCESS_STREAMING && mode != android::Asset::ACCESS_BUFFER) {
        Logger::E(TAG, "Bad access mode");
        return NULL;
    }

    android::Asset* a = mNative->open((const char *)fileName, (android::Asset::AccessMode)mode);

    if (a == NULL) {
        Logger::E(TAG, (const char *)fileName);
        return NULL;
    }

    //printf("Created Asset Stream: %p\n", a);

    return a;
}

ECode CAssetManager::OpenAssetFd(
    /* [in] */ const String& fileName,
    /* [in, out] */ ArrayOf<Int64>* outOffsets,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    if (fileName.IsNull()) {
        Logger::E(TAG, "fileName");
        *pfd = NULL;
        return E_FAIL;
    }

    android::Asset* a = mNative->open((const char *)fileName, android::Asset::ACCESS_RANDOM);

    if (a == NULL) {
        Logger::E(TAG, (const char *)fileName);
        *pfd = NULL;
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    //printf("Created Asset Stream: %p\n", a);

    return ReturnParcelFileDescriptor(a, outOffsets, pfd);
}

ECode CAssetManager::OpenNonAssetNative(
    /* [in] */ Int32 cookie,
    /* [in] */ const String& fileName,
    /* [in] */ Int32 mode,
    /* [out] */ android::Asset** asset)
{
    assert(asset != NULL);

    if (fileName.IsNull()) {
        Logger::E(TAG, "fileName");
        *asset = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    if (mode != android::Asset::ACCESS_UNKNOWN && mode != android::Asset::ACCESS_RANDOM
        && mode != android::Asset::ACCESS_STREAMING && mode != android::Asset::ACCESS_BUFFER) {
        Logger::E(TAG, "Bad access mode");
        *asset = NULL;
        return E_INVALID_ARGUMENT;
    }

    android::Asset* a = cookie
        ? mNative->openNonAsset((void*)cookie, (const char*)fileName, (android::Asset::AccessMode)mode)
        : mNative->openNonAsset((const char*)fileName, (android::Asset::AccessMode)mode);

    if (a == NULL) {
        Logger::E(TAG, (const char*)fileName);
        *asset = NULL;
        return E_FILE_NOT_FOUND;
    }

    //printf("Created Asset Stream: %p\n", a);

    *asset = a;
    return NOERROR;
}

ECode CAssetManager::OpenNonAssetFdNative(
    /* [in] */ Int32 cookie,
    /* [in] */ const String& fileName,
    /* [in] */ ArrayOf<Int64>* outOffsets,
    /* [out] */ IParcelFileDescriptor** fd)
{
    assert(outOffsets != NULL);
    assert(fd != NULL);

    if (fileName.IsNull()) {
        Logger::E(TAG, "fileName");
        *fd = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    android::Asset* a = cookie
        ? mNative->openNonAsset((void*)cookie, (const char*)fileName, android::Asset::ACCESS_RANDOM)
        : mNative->openNonAsset((const char*)fileName, android::Asset::ACCESS_RANDOM);

    if (a == NULL) {
        Logger::E(TAG, (const char*)fileName);
        *fd = NULL;
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    //printf("Created Asset Stream: %p\n", a);

    return ReturnParcelFileDescriptor(a, outOffsets, fd);
}

void CAssetManager::DestroyAsset(
    /* [in] */ android::Asset* asset)
{
    assert(asset);

    delete asset;
}

Int32 CAssetManager::ReadAssetChar(
    /* [in] */ android::Asset* asset)
{
    assert(asset);

    uint8_t b;
    ssize_t res = asset->read(&b, 1);
    return res == 1 ? (Int32)b : -1;
}

Int32 CAssetManager::ReadAsset(
    /* [in] */ android::Asset* asset,
    /* [in] */ const ArrayOf<Byte>& b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    assert(asset);

    if (len == 0) {
        return 0;
    }

    Int32 bLen = b.GetLength();
    if (off < 0 || off >= bLen || len < 0 || len > bLen || (off + len) > bLen) {
//	        jniThrowException(env, "java/lang/IndexOutOfBoundsException", "");
        return -1;
    }

    Byte* bt = b.GetPayload();
    ssize_t res = asset->read(bt + off, len);

    if (res > 0) return res;

    if (res < 0) {
//	        jniThrowException(env, "java/io/IOException", "");
    }
    return -1;
}

Int64 CAssetManager::SeekAsset(
    /* [in] */ android::Asset* asset,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 whence)
{
    assert(asset);

    return asset->seek(
        offset, (whence > 0) ? SEEK_END : (whence < 0 ? SEEK_SET : SEEK_CUR));
}

Int64 CAssetManager::GetAssetLength(
    /* [in] */ android::Asset* asset)
{
    assert(asset);

    return asset->getLength();
}

Int64 CAssetManager::GetAssetRemainingLength(
    /* [in] */ android::Asset* asset)
{
    assert(asset);

    return asset->getLength();
}

Int32 CAssetManager::LoadResourceValue(
    /* [in] */ Int32 ident,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolve)
{
    const android::ResTable& res(mNative->getResources());

    android::Res_value value;
    android::ResTable_config config;
    uint32_t typeSpecFlags;
    ssize_t block = res.getResource((uint32_t)ident, &value, false, &typeSpecFlags, &config);
#if THROW_ON_BAD_ID
    if (block == android::BAD_INDEX) {
        Logger::E(TAG, "Bad resource!");
        return 0;
    }
#endif
    uint32_t ref = ident;
    if (resolve) {
        block = res.resolveReference(&value, block, &ref);
#if THROW_ON_BAD_ID
        if (block == android::BAD_INDEX) {
            Logger::E(TAG, "Bad resource!");
            return 0;
        }
#endif
    }
    if (block >= 0) {
        ((CTypedValue*)outValue)->mType = (Int32)value.dataType;
        ((CTypedValue*)outValue)->mAssetCookie = (Int32)res.getTableCookie(block);
        ((CTypedValue*)outValue)->mData = (Int32)value.data;
        ((CTypedValue*)outValue)->mString = NULL;
        ((CTypedValue*)outValue)->mResourceId = (Int32)ref;
        ((CTypedValue*)outValue)->mChangingConfigurations = (Int32)typeSpecFlags;
        ((CTypedValue*)outValue)->mDensity = (Int32)config.density;
    }
    return block;
}

Int32 CAssetManager::LoadResourceBagValue(
    /* [in] */ Int32 ident,
    /* [in] */ Int32 bagEntryId,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolve)
{
    assert(outValue);

    const android::ResTable& res(mNative->getResources());

    // Now lock down the resource object and start pulling stuff from it.
    res.lock();

    ssize_t block = -1;
    android::Res_value value;

    const android::ResTable::bag_entry* entry = NULL;
    uint32_t typeSpecFlags;
    ssize_t entryCount = res.getBagLocked((uint32_t)ident, &entry, &typeSpecFlags);

    for (ssize_t i=0; i<entryCount; i++) {
        if (((uint32_t)bagEntryId) == entry->map.name.ident) {
            block = entry->stringBlock;
            value = entry->map.value;
        }
        entry++;
    }

    res.unlock();

    if (block < 0) {
        return block;
    }

    uint32_t ref = (uint32_t)ident;
    if (resolve) {
        block = res.resolveReference(&value, block, &ref, &typeSpecFlags);
#if THROW_ON_BAD_ID
        if (block == android::BAD_INDEX) {
            Logger::E(TAG, "Bad resource!");
            return 0;
        }
#endif
    }
    if (block >= 0) {
        ((CTypedValue*)outValue)->mType = (Int32)value.dataType;
        ((CTypedValue*)outValue)->mAssetCookie = (Int32)res.getTableCookie(block);
        ((CTypedValue*)outValue)->mData = (Int32)value.data;
        ((CTypedValue*)outValue)->mString = NULL;
        ((CTypedValue*)outValue)->mResourceId = (Int32)ref;
        ((CTypedValue*)outValue)->mChangingConfigurations = (Int32)typeSpecFlags;
    }
    return block;
}

Boolean CAssetManager::ApplyStyle(
    /* [in] */ android::ResTable::Theme* theme,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [in] */ android::ResXMLParser* xmlParser,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ const ArrayOf<Int32>& outValues,
    /* [in] */ const ArrayOf<Int32>& outIndices)
{
    assert(theme);

//	    DEBUG_STYLES(LOGI("APPLY STYLE: theme=0x%x defStyleAttr=0x%x defStyleRes=0x%x xml=0x%x",
//	        themeToken, defStyleAttr, defStyleRes, xmlParserToken));

    const android::ResTable& res = theme->getResTable();
    android::ResTable_config config;
    android::Res_value value;

    const Int32 NI = attrs.GetLength();
    const Int32 NV = outValues.GetLength();
    if (NV < (NI * STYLE_NUM_ENTRIES)) {
//	        jniThrowException(env, "java/lang/IndexOutOfBoundsException", "out values too small");
        return FALSE;
    }

//	    jint* src = (jint*)env->GetPrimitiveArrayCritical(attrs, 0);
    Int32* src = attrs.GetPayload();
    if (src == NULL) {
//	        jniThrowException(env, "java/lang/OutOfMemoryError", "");
        return FALSE;
    }

    Int32* baseDest = outValues.GetPayload();
    Int32* dest = baseDest;
    if (dest == NULL) {
//	        jniThrowException(env, "java/lang/OutOfMemoryError", "");
        return FALSE;
    }

    Int32* indices = NULL;
    Int32 indicesIdx = 0;
    if (outIndices.GetLength() > NI) {
        indices = outIndices.GetPayload();
    }

    // Load default style from attribute, if specified...
    uint32_t defStyleBagTypeSetFlags = 0;
    if (defStyleAttr != 0) {
        android::Res_value value;
        if (theme->getAttribute(defStyleAttr, &value, &defStyleBagTypeSetFlags) >= 0) {
            if (value.dataType == android::Res_value::TYPE_REFERENCE) {
                defStyleRes = value.data;
            }
        }
    }

    // Retrieve the style class associated with the current XML tag.
    Int32 style = 0;
    uint32_t styleBagTypeSetFlags = 0;
    if (xmlParser != NULL) {
        ssize_t idx = xmlParser->indexOfStyle();
        if (idx >= 0 && xmlParser->getAttributeValue(idx, &value) >= 0) {
            if (value.dataType == value.TYPE_ATTRIBUTE) {
                if (theme->getAttribute(value.data, &value, &styleBagTypeSetFlags) < 0) {
                    value.dataType = android::Res_value::TYPE_NULL;
                }
            }
            if (value.dataType == value.TYPE_REFERENCE) {
                style = value.data;
            }
        }
    }

    // Now lock down the resource object and start pulling stuff from it.
    res.lock();

    // Retrieve the default style bag, if requested.
    const android::ResTable::bag_entry* defStyleEnt = NULL;
    uint32_t defStyleTypeSetFlags = 0;
    ssize_t bagOff = defStyleRes != 0
            ? res.getBagLocked(defStyleRes, &defStyleEnt, &defStyleTypeSetFlags) : -1;
    defStyleTypeSetFlags |= defStyleBagTypeSetFlags;
    const android::ResTable::bag_entry* endDefStyleEnt = defStyleEnt +
        (bagOff >= 0 ? bagOff : 0);

    // Retrieve the style class bag, if requested.
    const android::ResTable::bag_entry* styleEnt = NULL;
    uint32_t styleTypeSetFlags = 0;
    bagOff = style != 0 ? res.getBagLocked(style, &styleEnt, &styleTypeSetFlags) : -1;
    styleTypeSetFlags |= styleBagTypeSetFlags;
    const android::ResTable::bag_entry* endStyleEnt = styleEnt +
        (bagOff >= 0 ? bagOff : 0);

    // Retrieve the XML attributes, if requested.
    const Int32 NX = xmlParser ? xmlParser->getAttributeCount() : 0;
    Int32 ix = 0;
    uint32_t curXmlAttr = xmlParser ? xmlParser->getAttributeNameResID(ix) : 0;

    static const ssize_t kXmlBlock = 0x10000000;

    // Now iterate through all of the attributes that the client has requested,
    // filling in each with whatever data we can find.
    ssize_t block = 0;
    uint32_t typeSetFlags;
    for (Int32 ii = 0; ii < NI; ii++) {
        const uint32_t curIdent = (uint32_t)src[ii];

        DEBUG_STYLES(Logger::I(TAG, StringBuffer("RETRIEVING ATTR 0x%08x...") + (Int32)curIdent));

        // Try to find a value for this attribute...  we prioritize values
        // coming from, first XML attributes, then XML style, then default
        // style, and finally the theme.
        value.dataType = android::Res_value::TYPE_NULL;
        value.data = 0;
        typeSetFlags = 0;
        config.density = 0;

        // Skip through XML attributes until the end or the next possible match.
        while (ix < NX && curIdent > curXmlAttr) {
            ix++;
            curXmlAttr = xmlParser->getAttributeNameResID(ix);
        }
        // Retrieve the current XML attribute if it matches, and step to next.
        if (ix < NX && curIdent == curXmlAttr) {
            block = kXmlBlock;
            xmlParser->getAttributeValue(ix, &value);
            ix++;
            curXmlAttr = xmlParser->getAttributeNameResID(ix);
//	            DEBUG_STYLES(LOGI("-> From XML: type=0x%x, data=0x%08x",
//	                    value.dataType, value.data));
        }

        // Skip through the style values until the end or the next possible match.
        while (styleEnt < endStyleEnt && curIdent > styleEnt->map.name.ident) {
            styleEnt++;
        }
        // Retrieve the current style attribute if it matches, and step to next.
        if (styleEnt < endStyleEnt && curIdent == styleEnt->map.name.ident) {
            if (value.dataType == android::Res_value::TYPE_NULL) {
                block = styleEnt->stringBlock;
                typeSetFlags = styleTypeSetFlags;
                value = styleEnt->map.value;
//	                DEBUG_STYLES(LOGI("-> From style: type=0x%x, data=0x%08x",
//	                        value.dataType, value.data));
            }
            styleEnt++;
        }

        // Skip through the default style values until the end or the next possible match.
        while (defStyleEnt < endDefStyleEnt && curIdent > defStyleEnt->map.name.ident) {
            defStyleEnt++;
        }
        // Retrieve the current default style attribute if it matches, and step to next.
        if (defStyleEnt < endDefStyleEnt && curIdent == defStyleEnt->map.name.ident) {
            if (value.dataType == android::Res_value::TYPE_NULL) {
                block = defStyleEnt->stringBlock;
                typeSetFlags = defStyleTypeSetFlags;
                value = defStyleEnt->map.value;
//	                DEBUG_STYLES(LOGI("-> From def style: type=0x%x, data=0x%08x",
//	                        value.dataType, value.data));
            }
            defStyleEnt++;
        }

        uint32_t resid = 0;
        if (value.dataType != android::Res_value::TYPE_NULL) {
            // Take care of resolving the found resource to its final value.
            ssize_t newBlock = theme->resolveAttributeReference(&value, block,
                    &resid, &typeSetFlags, &config);
            if (newBlock >= 0) block = newBlock;
//	            DEBUG_STYLES(LOGI("-> Resolved attr: type=0x%x, data=0x%08x",
//	                    value.dataType, value.data));
        }
        else {
            // If we still don't have a value for this attribute, try to find
            // it in the theme!
            ssize_t newBlock = theme->getAttribute(curIdent, &value, &typeSetFlags);
            if (newBlock >= 0) {
//	                DEBUG_STYLES(LOGI("-> From theme: type=0x%x, data=0x%08x",
//	                        value.dataType, value.data));
                newBlock = res.resolveReference(&value, block, &resid,
                        &typeSetFlags, &config);
#if THROW_ON_BAD_ID
                if (newBlock == android::BAD_INDEX) {
//	                    jniThrowException(env, "java/lang/IllegalStateException", "Bad resource!");
                    return FALSE;
                }
#endif
                if (newBlock >= 0) block = newBlock;
//	                DEBUG_STYLES(LOGI("-> Resolved theme: type=0x%x, data=0x%08x",
//	                        value.dataType, value.data));
            }
        }

        // Deal with the special @null value -- it turns back to TYPE_NULL.
        if (value.dataType == android::Res_value::TYPE_REFERENCE && value.data == 0) {
//	            DEBUG_STYLES(LOGI("-> Setting to @null!"));
            value.dataType = android::Res_value::TYPE_NULL;
        }

//	        DEBUG_STYLES(LOGI("Attribute 0x%08x: type=0x%x, data=0x%08x",
//	                curIdent, value.dataType, value.data));

        // Write the final value back to Java.
        dest[STYLE_TYPE] = (Int32)value.dataType;
        dest[STYLE_DATA] = (Int32)value.data;
        dest[STYLE_ASSET_COOKIE] =
            block != kXmlBlock ? (Int32)res.getTableCookie(block) : -1;
        dest[STYLE_RESOURCE_ID] = (Int32)resid;
        dest[STYLE_CHANGING_CONFIGURATIONS] = (Int32)typeSetFlags;
        dest[STYLE_DENSITY] = (Int32)config.density;

        if (indices != NULL && value.dataType != android::Res_value::TYPE_NULL) {
            indicesIdx++;
            indices[indicesIdx] = (Int32)ii;
        }

        dest += STYLE_NUM_ENTRIES;
    }

    res.unlock();

    if (indices != NULL) {
        indices[0] = indicesIdx;
    }

    return TRUE;
}

Boolean CAssetManager::RetrieveAttributes(
    /* [in] */ android::ResXMLParser* xmlParser,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ const ArrayOf<Int32>& outValues,
    /* [in] */ const ArrayOf<Int32>& outIndices)
{
    assert(xmlParser);

    android::AssetManager* am = mNative;
    assert(am);

    const android::ResTable& res(am->getResources());
    android::ResTable_config config;
    android::Res_value value;

    const Int32 NI = attrs.GetLength();
    const Int32 NV = outValues.GetLength();
    if (NV < (NI*STYLE_NUM_ENTRIES)) {
//	        jniThrowException(env, "java/lang/IndexOutOfBoundsException", "out values too small");
        return FALSE;
    }

    Int32* src = attrs.GetPayload();
    if (src == NULL) {
//	        jniThrowException(env, "java/lang/OutOfMemoryError", "");
        return FALSE;
    }

    Int32* baseDest = outValues.GetPayload();
    Int32* dest = baseDest;
    if (dest == NULL) {
//	        jniThrowException(env, "java/lang/OutOfMemoryError", "");
        return FALSE;
    }

    Int32* indices = NULL;
    Int32 indicesIdx = 0;
    if (outIndices.GetLength() > NI) {
        indices = outIndices.GetPayload();
    }

    // Now lock down the resource object and start pulling stuff from it.
    res.lock();

    // Retrieve the XML attributes, if requested.
    const Int32 NX = xmlParser->getAttributeCount();
    Int32 ix=0;
    uint32_t curXmlAttr = xmlParser->getAttributeNameResID(ix);

    static const ssize_t kXmlBlock = 0x10000000;

    // Now iterate through all of the attributes that the client has requested,
    // filling in each with whatever data we can find.
    ssize_t block = 0;
    uint32_t typeSetFlags;
    for (Int32 ii=0; ii<NI; ii++) {
        const uint32_t curIdent = (uint32_t)src[ii];

        // Try to find a value for this attribute...
        value.dataType = android::Res_value::TYPE_NULL;
        value.data = 0;
        typeSetFlags = 0;
        config.density = 0;

        // Skip through XML attributes until the end or the next possible match.
        while (ix < NX && curIdent > curXmlAttr) {
            ix++;
            curXmlAttr = xmlParser->getAttributeNameResID(ix);
        }
        // Retrieve the current XML attribute if it matches, and step to next.
        if (ix < NX && curIdent == curXmlAttr) {
            block = kXmlBlock;
            xmlParser->getAttributeValue(ix, &value);
            ix++;
            curXmlAttr = xmlParser->getAttributeNameResID(ix);
        }

        //printf("Attribute 0x%08x: type=0x%x, data=0x%08x\n", curIdent, value.dataType, value.data);
        uint32_t resid = 0;
        if (value.dataType != android::Res_value::TYPE_NULL) {
            // Take care of resolving the found resource to its final value.
            //printf("Resolving attribute reference\n");
            ssize_t newBlock = res.resolveReference(&value, block, &resid,
                    &typeSetFlags, &config);
#if THROW_ON_BAD_ID
            if (newBlock == android::BAD_INDEX) {
//	                jniThrowException(env, "java/lang/IllegalStateException", "Bad resource!");
                return JNI_FALSE;
            }
#endif
            if (newBlock >= 0) block = newBlock;
        }

        // Deal with the special @null value -- it turns back to TYPE_NULL.
        if (value.dataType == android::Res_value::TYPE_REFERENCE && value.data == 0) {
            value.dataType = android::Res_value::TYPE_NULL;
        }

        //printf("Attribute 0x%08x: final type=0x%x, data=0x%08x\n", curIdent, value.dataType, value.data);

        // Write the final value back to Java.
        dest[STYLE_TYPE] = (Int32)value.dataType;
        dest[STYLE_DATA] = (Int32)value.data;
        dest[STYLE_ASSET_COOKIE] =
            block != kXmlBlock ? (Int32)res.getTableCookie(block) : -1;
        dest[STYLE_RESOURCE_ID] = (Int32)resid;
        dest[STYLE_CHANGING_CONFIGURATIONS] = (Int32)typeSetFlags;
        dest[STYLE_DENSITY] = (Int32)config.density;

        if (indices != NULL && value.dataType != android::Res_value::TYPE_NULL) {
            indicesIdx++;
            indices[indicesIdx] = (Int32)ii;
        }

        dest += STYLE_NUM_ENTRIES;
    }

    res.unlock();

    if (indices != NULL) {
        indices[0] = indicesIdx;
    }

    return TRUE;
}

Int32 CAssetManager::GetArraySize(
    /* [in] */ Int32 resource)
{
    const android::ResTable& res(mNative->getResources());

    res.lock();
    const android::ResTable::bag_entry* defStyleEnt = NULL;
    ssize_t bagOff = res.getBagLocked(resource, &defStyleEnt);
    res.unlock();

    return bagOff;
}

Int32 CAssetManager::RetrieveArray(
    /* [in] */ Int32 resource,
    /* [in, out] */ ArrayOf<Int32>* outValues)
{
    if (outValues == NULL) {
        Logger::E(TAG, "out values");
        return 0;
    }

    const android::ResTable& res(mNative->getResources());
    android::ResTable_config config;
    android::Res_value value;
    ssize_t block;

    const Int32 NV = outValues->GetLength();

    // Now lock down the resource object and start pulling stuff from it.
    res.lock();

    const android::ResTable::bag_entry* arrayEnt = NULL;
    uint32_t arrayTypeSetFlags = 0;
    ssize_t bagOff = res.getBagLocked(resource, &arrayEnt, &arrayTypeSetFlags);
    const android::ResTable::bag_entry* endArrayEnt = arrayEnt +
        (bagOff >= 0 ? bagOff : 0);

    Int32 i = 0;
    uint32_t typeSetFlags;
    while (i < NV && arrayEnt < endArrayEnt) {
        block = arrayEnt->stringBlock;
        typeSetFlags = arrayTypeSetFlags;
        config.density = 0;
        value = arrayEnt->map.value;

        uint32_t resid = 0;
        if (value.dataType != android::Res_value::TYPE_NULL) {
            // Take care of resolving the found resource to its final value.
            //printf("Resolving attribute reference\n");
            ssize_t newBlock = res.resolveReference(&value, block, &resid,
                    &typeSetFlags, &config);
#if THROW_ON_BAD_ID
            if (newBlock == android::BAD_INDEX) {
                Logger::E(TAG, "Bad resource!");
                return 0;
            }
#endif
            if (newBlock >= 0) block = newBlock;
        }

        // Deal with the special @null value -- it turns back to TYPE_NULL.
        if (value.dataType == android::Res_value::TYPE_REFERENCE && value.data == 0) {
            value.dataType = android::Res_value::TYPE_NULL;
        }

        //printf("Attribute 0x%08x: final type=0x%x, data=0x%08x\n", curIdent, value.dataType, value.data);

        // Write the final value back to Java.
        (*outValues)[i + STYLE_TYPE] = (Int32)value.dataType;
        (*outValues)[i + STYLE_DATA] = (Int32)value.data;
        (*outValues)[i + STYLE_ASSET_COOKIE] = (Int32)res.getTableCookie(block);
        (*outValues)[i + STYLE_RESOURCE_ID] = (Int32)resid;
        (*outValues)[i + STYLE_CHANGING_CONFIGURATIONS] = (Int32)typeSetFlags;
        (*outValues)[i + STYLE_DENSITY] = (Int32)config.density;
        i+= STYLE_NUM_ENTRIES;
        arrayEnt++;
    }

    i /= STYLE_NUM_ENTRIES;

    res.unlock();

    return i;
}

Int32 CAssetManager::GetStringBlockCount()
{
    return mNative->getResources().getTableCount();
}

const android::ResStringPool* CAssetManager::GetNativeStringBlock(
    /* [in] */ Int32 block)
{
    return mNative->getResources().getTableStringBlock(block);
}

ECode CAssetManager::GetCookieName(
    /* [in] */ Int32 cookie,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    android::String8 n(mNative->getAssetPath((void*)cookie));
    if (n.length() == 0) {
        Logger::E(TAG, "Empty cookie name");
        *name = NULL;
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *name = n.string();
    return (*name).IsNull() ? E_OUT_OF_MEMORY : NOERROR;
}

ECode CAssetManager::GetGlobalAssetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = android::Asset::getGlobalCount();
    return NOERROR;
}

ECode CAssetManager::GetAssetAllocations(
    /* [out] */ String* allocations)
{
    VALIDATE_NOT_NULL(allocations);

    android::String8 alloc = android::Asset::getAssetAllocations();
    if (alloc.length() <= 0) {
        *allocations = NULL;
        return E_DOES_NOT_EXIST;
    }

    *allocations = alloc.string();
    return (*allocations).IsNull() ? E_OUT_OF_MEMORY : NOERROR;
}

ECode CAssetManager::GetGlobalAssetManagerCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = android::AssetManager::getGlobalCount();
    return NOERROR;
}

android::ResTable::Theme* CAssetManager::NewTheme()
{
    return new android::ResTable::Theme(mNative->getResources());
}

void CAssetManager::DeleteTheme(
    /* [in] */ Int32 theme)
{
    delete (android::ResTable::Theme*)theme;
}

void CAssetManager::ApplyThemeStyle(
    /* [in] */ android::ResTable::Theme* theme,
    /* [in] */ Int32 styleRes,
    /* [in] */ Boolean force)
{
    assert(theme);
    theme->applyStyle(styleRes, force ? true : false);
}

void CAssetManager::CopyTheme(
    /* [in] */ android::ResTable::Theme* dest,
    /* [in] */ android::ResTable::Theme* source)
{
    assert(dest);
    assert(source);
    dest->setTo(*source);
}

Int32 CAssetManager::LoadThemeAttributeValue(
    /* [in] */ android::ResTable::Theme* theme,
    /* [in] */ Int32 ident,
    /* [in] */ ITypedValue* outValue,
    /* [in] */ Boolean resolve)
{
    assert(theme);
    assert(outValue);
//	    android::ResTable::Theme* theme = (ResTable::Theme*)themeInt;
    const android::ResTable& res(theme->getResTable());

    android::Res_value value;
    // XXX value could be different in different configs!
    uint32_t typeSpecFlags = 0;
    ssize_t block = theme->getAttribute(
        ident, &value, &typeSpecFlags);
    uint32_t ref = 0;
    if (resolve) {
        block = res.resolveReference(&value, block, &ref, &typeSpecFlags);
#if THROW_ON_BAD_ID
        if (block == android::BAD_INDEX) {
//	            jniThrowException(env, "java/lang/IllegalStateException", "Bad resource!");
            return 0;
        }
#endif
    }
    if (block >= 0) {
        ((CTypedValue*)outValue)->mType = (Int32)value.dataType;
        ((CTypedValue*)outValue)->mAssetCookie = (Int32)res.getTableCookie(block);
        ((CTypedValue*)outValue)->mData = (Int32)value.data;
        ((CTypedValue*)outValue)->mString = NULL;
        ((CTypedValue*)outValue)->mResourceId = (Int32)ref;
        ((CTypedValue*)outValue)->mChangingConfigurations = (Int32)typeSpecFlags;
    }
    return 0;
}

void CAssetManager::DumpTheme(
    /* [in] */ Int32 theme,
    /* [in] */ Int32 priority,
    /* [in] */ const String& tag,
    /* [in] */ const String& prefix)
{
    android::ResTable::Theme* andTheme = (android::ResTable::Theme*)theme;

    assert(!tag.IsNull());

    const char* prefix8 = NULL;
    if (!prefix.IsNull()) {
        prefix8 = (const char*)prefix;
    }

    // XXX Need to use params.
    andTheme->dumpToLog();
}

android::ResXMLTree* CAssetManager::OpenXmlAssetNative(
    /* [in] */ Int32 cookie,
    /* [in] */ const String& fileName)
{
    Logger::V(TAG, StringBuffer("openXmlAsset in native object ") + mNative);

    if (fileName.IsNull()) {
//        jniThrowException(env, "java/lang/NullPointerException", "fileName");
        return NULL;
    }

    const char* fileName8 = (const char*)fileName;
    android::Asset* a = cookie
        ? mNative->openNonAsset((void*)cookie, fileName8, android::Asset::ACCESS_BUFFER)
        : mNative->openNonAsset(fileName8, android::Asset::ACCESS_BUFFER);

    if (a == NULL) {
//        jniThrowException(env, "java/io/FileNotFoundException", fileName8);
        return NULL;
    }

    android::ResXMLTree* block = new android::ResXMLTree();
    android::status_t err = block->setTo(a->getBuffer(true), a->getLength(), true);
    a->close();
    delete a;

    if (err != android::NO_ERROR) {
//        jniThrowException(env, "java/io/FileNotFoundException", "Corrupt XML binary file");
        return NULL;
    }

    return block;
}

ECode CAssetManager::GetArrayStringResource(
    /* [in] */ Int32 arrayResId,
    /* [out] */ ArrayOf<String>** array)
{
    assert(array);

    const android::ResTable& res(mNative->getResources());
    const android::ResTable::bag_entry* startOfBag;
    const ssize_t N = res.lockBag(arrayResId, &startOfBag);
    if (N < 0) {
        *array = NULL;
        return E_FAIL;
    }

    *array = ArrayOf<String>::Alloc(N);
    if (!*array) return E_OUT_OF_MEMORY;

    android::Res_value value;
    const android::ResTable::bag_entry* bag = startOfBag;
    size_t strLen = 0;
    for (size_t i=0; ((ssize_t)i) < N; i++, bag++) {
        value = bag->map.value;
        String str;

        // Take care of resolving the found resource to its final value.
        ssize_t block = res.resolveReference(&value, bag->stringBlock, NULL);
#if THROW_ON_BAD_ID
        if (block == android::BAD_INDEX) {
            Logger::E(TAG, "Bad resource!");
            //todo: maybe exist memory leak
            ArrayOf<String>::Free(*array);
            *array = NULL;
            return E_FILE_NOT_FOUND;
        }
#endif
        if (value.dataType == android::Res_value::TYPE_STRING) {
            const android::ResStringPool* pool = res.getTableStringBlock(block);
            const char* str8 = pool->string8At(value.data, &strLen);
            if (str8 != NULL) {
                str = str8;
            }
            else {
                const char16_t* str16 = pool->stringAt(value.data, &strLen);
                // BUGBUG: support copy str16 here.
//	                str = env->NewString(str16, strLen);
                android::String8 s8(str16);
                str = s8.string();
            }
            if (str.IsNull()) {
                //todo: maybe exist memory leak
                ArrayOf<String>::Free(*array);
                *array = NULL;
                return E_OUT_OF_MEMORY;
            }

            // If one of our NewString{UTF} calls failed due to memory, an
            // exception will be pending.
            (**array)[i] = str;

            // str is not NULL at that point, otherwise ExceptionCheck would have been true.
            // If we have a large amount of strings in our array, we might
            // overflow the local reference table of the VM.
        }
    }
    res.unlockBag(startOfBag);
    return NOERROR;
}

ECode CAssetManager::GetArrayStringInfo(
    /* [in] */ Int32 arrayResId,
    /* [out] */ ArrayOf<Int32>** array)
{
    const android::ResTable& res(mNative->getResources());

    const android::ResTable::bag_entry* startOfBag;
    const ssize_t N = res.lockBag(arrayResId, &startOfBag);
    if (N < 0) {
        *array = NULL;
        return E_FAIL;
    }

    *array = ArrayOf<Int32>::Alloc(N * 2);
    if (!*array) return E_OUT_OF_MEMORY;

    android::Res_value value;
    const android::ResTable::bag_entry* bag = startOfBag;
    for (size_t i = 0, j = 0; ((ssize_t)i) < N; i++, bag++) {
        uint32_t stringIndex = -1;
        ssize_t stringBlock = 0;
        value = bag->map.value;

        // Take care of resolving the found resource to its final value.
        stringBlock = res.resolveReference(&value, bag->stringBlock, NULL);
        if (value.dataType == android::Res_value::TYPE_STRING) {
            stringIndex = value.data;
        }

#if THROW_ON_BAD_ID
        if (stringBlock == android::BAD_INDEX) {
            Logger::E(TAG, "Bad resource!");
            ArrayOf<Int32>::Free(*array);
            *array = NULL;
            return E_FILE_NOT_FOUND;
        }
#endif

        //todo: It might be faster to allocate a C array to contain
        //      the blocknums and indices, put them in there and then
        //      do just one SetIntArrayRegion()
        (**array)[j] = (Int32)stringBlock;
        (**array)[j + 1] = (Int32)stringIndex;
        j = j + 2;
    }
    res.unlockBag(startOfBag);
    return NOERROR;
}

ECode CAssetManager::GetArrayIntResource(
    /* [in] */ Int32 arrayRes,
    /* [out] */ ArrayOf<Int32>** array)
{
    assert(array);

    const android::ResTable& res(mNative->getResources());

    const android::ResTable::bag_entry* startOfBag;
    const ssize_t N = res.lockBag(arrayRes, &startOfBag);
    if (N < 0) {
        *array = NULL;
        return E_FILE_NOT_FOUND;
    }

    *array = ArrayOf<Int32>::Alloc(N);
    if (!*array) {
        return E_OUT_OF_MEMORY;
    }

    android::Res_value value;
    const android::ResTable::bag_entry* bag = startOfBag;
    for (size_t i = 0; ((ssize_t)i) < N; i++, bag++) {
        value = bag->map.value;

#if THROW_ON_BAD_ID
        // Take care of resolving the found resource to its final value.
        ssize_t block = res.resolveReference(&value, bag->stringBlock, NULL);
        if (block == android::BAD_INDEX) {
            Logger::E(TAG, "Bad resource!");
            ArrayOf<Int32>::Free(array);
            *array = NULL;
            return E_ILLEGAL_STATE_EXCEPTION;
        }
#endif
        if (value.dataType >= android::Res_value::TYPE_FIRST_INT
                && value.dataType <= android::Res_value::TYPE_LAST_INT) {
            (**array)[i] = (Int32)value.data;
        }
    }

    res.unlockBag(startOfBag);
    return NOERROR;
}

ECode CAssetManager::Init()
{
    mNative = new android::AssetManager();
    if (mNative == NULL) {
        return E_OUT_OF_MEMORY;
    }

    mNative->addDefaultAssets();

    Logger::V(TAG, StringBuffer("Created AssetManager ") + (Int32)mNative + " for Java object");

    return NOERROR;
}

void CAssetManager::Destroy()
{
    Logger::V(TAG, StringBuffer("Destroying AssetManager %p for Java object %p\n") + (Int32)mNative);
    if (mNative != NULL) {
        delete mNative;
        mNative = NULL;
    }
}

void CAssetManager::IncRefsLocked(
    /* [in] */ Int32 id)
{
    if (DEBUG_REFS) {
        Int32 code;
        this->GetHashCode(&code);
        mRefStacks[code] = E_RUNTIME_EXCEPTION;
    }
    mNumRefs++;
}

void CAssetManager::DecRefsLocked(
    /* [in] */ Int32 id)
{
    if (DEBUG_REFS) {
        mRefStacks.Erase(id);
    }
    mNumRefs--;
    //System.out.println("Dec streams: mNumRefs=" + mNumRefs
    //                   + " mReleased=" + mReleased);
    if (mNumRefs == 0) {
        Destroy();
    }
}

ECode CAssetManager::ReturnParcelFileDescriptor(
    /* [in] */ android::Asset* a,
    /* [in, out] */ ArrayOf<Int64>* outOffsets,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    assert(pfd != NULL);

    off_t startOffset, length;
    int fd = a->openFileDescriptor(&startOffset, &length);
    delete a;

    if (fd < 0) {
        Logger::E(TAG, "This file can not be opened as a file descriptor; it is probably compressed");
        *pfd = NULL;
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    if (outOffsets == NULL) {
        close(fd);
        *pfd = NULL;
        return E_FAIL;
    }

    (*outOffsets)[0] = startOffset;
    (*outOffsets)[1] = length;

    AutoPtr<IFileDescriptor> fileDesc;
    ECode ec = CFileDescriptor::New((IFileDescriptor**)&fileDesc);
    if (FAILED(ec)) {
        close(fd);
        *pfd = NULL;
        return ec;
    }
    fileDesc->SetDescriptor(fd);

    return CParcelFileDescriptor::New(fileDesc.Get(), pfd);
}

ECode CAssetManager::GetHashCode(
    /* [out] */ Int32* code)
{
    assert(code);

    *code = (Int32)this;
    return NOERROR;
}

