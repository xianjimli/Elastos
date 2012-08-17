
#include "content/CResources.h"
#include "content/CompatibilityInfo.h"
#include "content/CCompatibilityInfo.h"
#include "content/CAssetManager.h"
#include "content/CTypedArray.h"
#include "content/CTheme.h"
#include "content/CColorStateList.h"
#include "content/XmlBlock.h"
#include "utils/CDisplayMetrics.h"
#include "utils/CTypedValue.h"
#include "view/CDisplay.h"
#include "graphics/CMovie.h"
#include "graphics/drawable/CColorDrawable.h"
#include <Logger.h>
#include <elastos/Locale.h>
#include <StringBuffer.h>
#include <assert.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

#define CTYPEDARRAY(x) ((CTypedArray*)x.Get())
#define CTYPEDVALUE(x) ((CTypedValue*)x.Get())

const char* CResources::TAG = "CResources";
const Boolean CResources::DEBUG_LOAD;
const Boolean CResources::DEBUG_CONFIG;
const Boolean CResources::TRACE_FOR_PRELOAD;
const Int32 CResources::sSdkVersion;

Mutex CResources::mSyncLock;
AutoPtr<IResources> CResources::mSystem;
HashMap<Int64, AutoPtr<IDrawableConstantState> >* CResources::sPreloadedDrawables =
    new HashMap<Int64, AutoPtr<IDrawableConstantState> >;
HashMap<Int32, AutoPtr<IColorStateList> >* CResources::mPreloadedColorStateLists =
    new HashMap<Int32, AutoPtr<IColorStateList> >;
Boolean CResources::mPreloaded = FALSE;

CResources::CResources()
    : mPreloading(FALSE)
    , mLastCachedXmlBlockIndex(-1)
    , mPluralRule(NULL)
{
    ASSERT_SUCCEEDED(CTypedValue::New((ITypedValue**)&mTmpValue));

    for (Int32 i = 0; i < 4; i++) {
        mCachedXmlBlockIds[i] = 0;
    }
}

CResources::~CResources()
{
    for (Int32 i = 0; i < 4; i++) {
        delete mCachedXmlBlocks[i];
    }
}

ECode CResources::GetText(
    /* [in] */ Int32 id,
    /* [out] */ ICharSequence** seq)
{
    if (!seq) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = mAssets->GetResourceText(id, seq);
    if (SUCCEEDED(ec)) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("String resource ID #0x") + id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetQuantityText(
    /* [in] */ Int32 id,
    /* [in] */ Int32 quantity,
    /* [out] */ ICharSequence** csq)
{
    if (!csq) {
        return E_INVALID_ARGUMENT;
    }

    PluralRules* rule = GetPluralRule();
    mAssets->GetResourceBagText(id, rule->AttrForNumber(quantity), csq);
    if (*csq != NULL) {
        return NOERROR;
    }
    mAssets->GetResourceBagText(id, PluralRules::ID_OTHER, csq);
    if (*csq != NULL) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Plural resource ID #0x") + id
        + " quantity=" + quantity
        + " item=" + PluralRules::StringForQuantity(rule->QuantityForNumber(quantity)));
    return E_NOT_FOUND_EXCEPTION;
}

PluralRules* CResources::GetPluralRule()
{
    Mutex::Autolock lock(mSyncLock);
    if (mPluralRule == NULL) {
        mPluralRule = PluralRules::RuleForLocale(mConfiguration->mLocale);
    }
    return mPluralRule;
}

ECode CResources::GetString(
    /* [in] */ Int32 id,
    /* [out] */ String* str)
{
    if (!str) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<ICharSequence> res;
    ECode ec = GetText(id, (ICharSequence**)&res);
    if (SUCCEEDED(ec) && res != NULL) {
        res->ToString(str);
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("String resource ID #0x") + id);
    *str = NULL;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetQuantityString(
    /* [in] */ Int32 id,
    /* [in] */ Int32 quantity,
    /* [out] */ String* str)
{
    if (!str) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<ICharSequence> res;
    ECode ec = GetQuantityText(id, quantity, (ICharSequence**)&res);
    if (SUCCEEDED(ec) && res != NULL) {
        res->ToString(str);
//        return String.format(mConfiguration.locale, raw, formatArgs);
        return NOERROR;
    }

    *str = NULL;
    return ec;
}

ECode CResources::GetTextEx(
    /* [in] */ Int32 id,
    /* [in] */ ICharSequence* def,
    /* [out] */ ICharSequence** csq)
{
    if (!csq) {
        return E_INVALID_ARGUMENT;
    }

    if (id != 0) {
        mAssets->GetResourceText(id, csq);
    }
    else *csq = NULL;
    if (!*csq) {
        *csq = def;
        if (*csq) (*csq)->AddRef();
    }
    return NOERROR;
}

ECode CResources::GetTextArray(
    /* [in] */ Int32 id,
    /* [out, callee] */ ArrayOf<ICharSequence*>** seqs)
{
    if (!seqs) {
        return E_INVALID_ARGUMENT;
    }

    mAssets->GetResourceTextArray(id, seqs);
    if (*seqs) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Text array resource ID #0x") + id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetStringArray(
    /* [in] */ Int32 id,
    /* [out, callee] */ ArrayOf<String>** strs)
{
    if (!strs) {
        return E_INVALID_ARGUMENT;
    }

    mAssets->GetResourceStringArray(id, strs);
    if (*strs) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("String array resource ID #0x") + id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetIntArray(
    /* [in] */ Int32 id,
    /* [out, callee] */ ArrayOf<Int32>** ints)
{
    if (!ints) {
        return E_INVALID_ARGUMENT;
    }

    mAssets->GetArrayIntResource(id, ints);
    if (*ints) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Int array resource ID #0x") + id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::ObtainTypedArray(
    /* [in] */ Int32 id,
    /* [out] */ ITypedArray** array)
{
    if (!array) {
        return E_INVALID_ARGUMENT;
    }

    Int32 len = mAssets->GetArraySize(id);
    if (len < 0) {
        Logger::E(TAG, StringBuffer("Array resource ID #0x") + id);
        *array = NULL;
        return E_NOT_FOUND_EXCEPTION;
    }

    assert(SUCCEEDED(GetCachedStyledAttributes(len, array)));
    ((CTypedArray*)*array)->mLength = mAssets->RetrieveArray(
        id, ((CTypedArray*)*array)->mData);
    (*((CTypedArray*)*array)->mIndices)[0] = 0;

    return NOERROR;
}

ECode CResources::GetDimension(
    /* [in] */ Int32 id,
    /* [out] */ Float* dim)
{
    if (!dim) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    if (CTYPEDVALUE(mTmpValue)->mType == TypedValue_TYPE_DIMENSION) {
        *dim = CTypedValue::ComplexToDimension(CTYPEDVALUE(mTmpValue)->mData,
                (IDisplayMetrics*)mMetrics.Get());
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Resource ID #0x") + id
        + " type #0x" + CTYPEDVALUE(mTmpValue)->mType + " is not valid");
    *dim = 0.0f;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetDimensionPixelOffset(
    /* [in] */ Int32 id,
    /* [out] */ Int32* offset)
{
    if (!offset) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    if (CTYPEDVALUE(mTmpValue)->mType == TypedValue_TYPE_DIMENSION) {
        *offset = CTypedValue::ComplexToDimensionPixelOffset(
                CTYPEDVALUE(mTmpValue)->mData,
                (IDisplayMetrics*)mMetrics.Get());
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Resource ID #0x") + id
        + " type #0x" + CTYPEDVALUE(mTmpValue)->mType + " is not valid");
    *offset = 0;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetDimensionPixelSize(
    /* [in] */ Int32 id,
    /* [out] */ Int32* size)
{
    if (!size) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    if (CTYPEDVALUE(mTmpValue)->mType == TypedValue_TYPE_DIMENSION) {
        *size = CTypedValue::ComplexToDimensionPixelSize(
                CTYPEDVALUE(mTmpValue)->mData,
                (IDisplayMetrics*)mMetrics.Get());
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Resource ID #0x") + id
        + " type #0x" + CTYPEDVALUE(mTmpValue)->mType + " is not valid");
    *size = 0;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetFraction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 base,
    /* [in] */ Int32 pbase,
    /* [out] */ Float* fraction)
{
    if (!fraction) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    if (CTYPEDVALUE(mTmpValue)->mType == TypedValue_TYPE_FRACTION) {
        *fraction = CTypedValue::ComplexToFraction(CTYPEDVALUE(mTmpValue)->mData,
                base, pbase);
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Resource ID #0x") + id
        + CTYPEDVALUE(mTmpValue)->mType + " is not valid");
    *fraction = 0.0f;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetDrawable(
    /* [in] */ Int32 id,
    /* [out] */ IDrawable** drawable)
{
    if (!drawable) {
        return E_INVALID_ARGUMENT;
    }

    {
        Mutex::Autolock lock(mTmpValueLock);

        GetValue(id, mTmpValue.Get(), TRUE);
    }
    return LoadDrawable(mTmpValue.Get(), id, drawable);
}

ECode CResources::GetMovie(
    /* [in] */ Int32 id,
    /* [out] */ IMovie** movie)
{
    if (!movie) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<IInputStream> is;
    OpenRawResource(id, (IInputStream**)&is);
    ECode ec = CMovie::DecodeStream(is, movie);
    is->Close(); // don't care, since the return value is valid

    return ec;
}

ECode CResources::GetColor(
    /* [in] */ Int32 id,
    /* [out] */ Int32* color)
{
    if (!color) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    if (CTYPEDVALUE(mTmpValue)->mType >= TypedValue_TYPE_FIRST_INT
            && CTYPEDVALUE(mTmpValue)->mType <= TypedValue_TYPE_LAST_INT) {
        *color = CTYPEDVALUE(mTmpValue)->mData;
        return NOERROR;
    } else if (CTYPEDVALUE(mTmpValue)->mType == TypedValue_TYPE_STRING) {
        AutoPtr<IColorStateList> csl;
        LoadColorStateList(mTmpValue.Get(), id, (IColorStateList**)&csl);
        return csl->GetDefaultColor(color);
    }

    Logger::E(TAG, StringBuffer() + id
        + " type #0x" + CTYPEDVALUE(mTmpValue)->mType + " is not valid");
    *color = 0;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetColorStateList(
    /* [in] */ Int32 id,
    /* [out] */ IColorStateList** list)
{
    if (!list) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    return LoadColorStateList(mTmpValue.Get(), id, list);
}

ECode CResources::GetBoolean(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* b)
{
    if (!b) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    if (CTYPEDVALUE(mTmpValue)->mType >= TypedValue_TYPE_FIRST_INT
        && CTYPEDVALUE(mTmpValue)->mType <= TypedValue_TYPE_LAST_INT) {
        *b = CTYPEDVALUE(mTmpValue)->mData != 0;
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Resource ID #0x") + id
        + " type #0x" + CTYPEDVALUE(mTmpValue)->mType + " is not valid");
    *b = FALSE;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetInteger(
    /* [in] */ Int32 id,
    /* [out] */ Int32* i)
{
    if (!i) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    if (CTYPEDVALUE(mTmpValue)->mType >= TypedValue_TYPE_FIRST_INT
        && CTYPEDVALUE(mTmpValue)->mType <= TypedValue_TYPE_LAST_INT) {
        *i = CTYPEDVALUE(mTmpValue)->mData;
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Resource ID #0x") + id
        + " type #0x" + CTYPEDVALUE(mTmpValue)->mType + " is not valid");
    *i = 0;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetLayout(
    /* [in] */ Int32 id,
    /* [out] */ IXmlResourceParser** parser)
{
    if (!parser) {
        return E_INVALID_ARGUMENT;
    }

    return LoadXmlResourceParser(id, "layout", parser);
}

ECode CResources::GetAnimation(
    /* [in] */ Int32 id,
    /* [out] */ IXmlResourceParser** animation)
{
    if (!animation) {
        return E_INVALID_ARGUMENT;
    }

    return LoadXmlResourceParser(id, "anim", animation);
}

ECode CResources::GetXml(
    /* [in] */ Int32 id,
    /* [out] */ IXmlResourceParser** xml)
{
    if (!xml) {
        return E_INVALID_ARGUMENT;
    }

    return LoadXmlResourceParser(id, "xml", xml);
}

ECode CResources::OpenRawResource(
    /* [in] */ Int32 id,
    /* [out] */ IInputStream** res)
{
    Mutex::Autolock lock(mTmpValueLock);

    return OpenRawResourceEx(id, mTmpValue.Get(), res);
}

ECode CResources::OpenRawResourceEx(
    /* [in] */ Int32 id,
    /* [in] */ ITypedValue* value,
    /* [out] */ IInputStream** res)
{
    if (!value) {
        return E_INVALID_ARGUMENT;
    }

    GetValue(id, value, TRUE);

//    try {
    String str;
    ((CTypedValue*)value)->mString->ToString(&str);
    return mAssets->OpenNonAssetEx3(
            ((CTypedValue*)value)->mAssetCookie,
            str, AssetManager_ACCESS_STREAMING, res);
//    } catch (Exception e) {
//        NotFoundException rnf = new NotFoundException("File " + value.string.toString() +
//                " from drawable resource ID #0x" + Integer.toHexString(id));
//        rnf.initCause(e);
//        throw rnf;
//    }
}

ECode CResources::OpenRawResourceFd(
    /* [in] */ Int32 id,
    /* [out] */ IAssetFileDescriptor** des)
{
    if (!des) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);

//    try {
    String str;
    CTYPEDVALUE(mTmpValue)->mString->ToString(&str);
    return mAssets->OpenNonAssetFdEx(
            CTYPEDVALUE(mTmpValue)->mAssetCookie, str, des);
//    } catch (Exception e) {
//        NotFoundException rnf = new NotFoundException(
//            "File " + value.string.toString()
//            + " from drawable resource ID #0x"
//            + Integer.toHexString(id));
//        rnf.initCause(e);
//        throw rnf;
//    }
}

ECode CResources::GetValue(
    /* [in] */ Int32 id,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs)
{
    if (!outValue) {
        return E_INVALID_ARGUMENT;
    }

    Boolean found = mAssets->GetResourceValue(
            id, outValue, resolveRefs);
    if (found) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Resource ID #0x") + id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetValueEx(
    /* [in] */ const String& name,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs)
{
    Int32 id = 0;
    GetIdentifier(name, String("string"), String(NULL), &id);
    if (id != 0) {
        return GetValue(id, outValue, resolveRefs);
    }
    Logger::E(TAG, StringBuffer("String resource name ") + name);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::NewTheme(
    /* [out] */ ITheme** theme)
{
    if (!theme) {
        return E_INVALID_ARGUMENT;
    }

    return CTheme::New((IResources*)this, (IAssetManager*)mAssets.Get(), theme);
}

ECode CResources::ObtainAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** array)
{
    VALIDATE_NOT_NULL(set);
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    Int32 len = attrs.GetLength();
    FAIL_RETURN(GetCachedStyledAttributes(len, array));

    // XXX note that for now we only work with compiled XML files.
    // To support generic XML files we will need to manually parse
    // out the attributes from the XML file (applying type information
    // contained in the resources and such).
    AutoPtr<CTypedArray> a = (CTypedArray*)*array;
    AutoPtr<XmlBlock::Parser> parser = (XmlBlock::Parser*)set;
    mAssets->RetrieveAttributes(
            parser->mParseState, attrs, *a->mData, *a->mIndices);

    a->mRsrcs = attrs.Clone();
    a->mXml = parser;

    return NOERROR;
}

ECode CResources::UpdateConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ IDisplayMetrics* metrics)
{
    {
        Mutex::Autolock lock(mTmpValueLock);

        Int32 configChanges = 0xfffffff;
        if (config != NULL) {
            mConfiguration->UpdateFrom(config, &configChanges);
        }
        if (mConfiguration->mLocale == NULL) {
            mConfiguration->mLocale = Locale::GetDefault();
        }
        if (metrics != NULL) {
            mMetrics->SetTo(metrics);
            mMetrics->UpdateMetrics(mCompatibilityInfo.Get(),
                mConfiguration->mOrientation, mConfiguration->mScreenLayout);
        }
        mMetrics->mScaledDensity = mMetrics->mDensity * mConfiguration->mFontScale;

        StringBuffer locale;
        if (mConfiguration->mLocale != NULL) {
            locale = mConfiguration->mLocale->GetLanguage();
            if (!mConfiguration->mLocale->GetCountry().IsNull()) {
                locale += "-" + mConfiguration->mLocale->GetCountry();
            }
        }
        Int32 width, height;
        if (mMetrics->mWidthPixels >= mMetrics->mHeightPixels) {
            width = mMetrics->mWidthPixels;
            height = mMetrics->mHeightPixels;
        } else {
            //noinspection SuspiciousNameCombination
            width = mMetrics->mHeightPixels;
            //noinspection SuspiciousNameCombination
            height = mMetrics->mWidthPixels;
        }
        Int32 keyboardHidden = mConfiguration->mKeyboardHidden;
        if (keyboardHidden == Configuration_KEYBOARDHIDDEN_NO
                && mConfiguration->mHardKeyboardHidden
                        == Configuration_HARDKEYBOARDHIDDEN_YES) {
            keyboardHidden = Configuration_KEYBOARDHIDDEN_SOFT;
        }
        mAssets->SetConfiguration(mConfiguration->mMcc, mConfiguration->mMnc,
                String((const char *)locale), mConfiguration->mOrientation,
                mConfiguration->mTouchscreen,
                (Int32)(mMetrics->mDensity * 160), mConfiguration->mKeyboard,
                keyboardHidden, mConfiguration->mNavigation, width, height,
                mConfiguration->mScreenLayout, mConfiguration->mUiMode, sSdkVersion);
//	        int N = mDrawableCache.size();
        if (DEBUG_CONFIG) {
            Logger::D(TAG, StringBuffer("Cleaning up drawables config changes: 0x") + configChanges);
        }
//	        for (int i=0; i < N; i++) {
//	            WeakReference<Drawable.ConstantState> ref = mDrawableCache.valueAt(i);
//	            if (ref != null) {
//	                Drawable.ConstantState cs = ref.get();
//	                if (cs != null) {
//	                    if (Configuration.needNewResources(
//	                            configChanges, cs.getChangingConfigurations())) {
//	                        if (DEBUG_CONFIG) {
//	                            Log.d(TAG, "FLUSHING #0x"
//	                                    + Long.toHexString(mDrawableCache.keyAt(i))
//	                                    + " / " + cs + " with changes: 0x"
//	                                    + Integer.toHexString(cs.getChangingConfigurations()));
//	                        }
//	                        mDrawableCache.setValueAt(i, null);
//	                    } else if (DEBUG_CONFIG) {
//	                        Log.d(TAG, "(Keeping #0x"
//	                                + Long.toHexString(mDrawableCache.keyAt(i))
//	                                + " / " + cs + " with changes: 0x"
//	                                + Integer.toHexString(cs.getChangingConfigurations())
//	                                + ")");
//	                    }
//	                }
//	            }
//	        }
//	        mDrawableCache.clear();
//	        mColorStateListCache.clear();
        FlushLayoutCache();
    }
    {
        Mutex::Autolock lock(mSyncLock);

        if (mPluralRule != NULL) {
            assert(config);
            mPluralRule = PluralRules::RuleForLocale(((CConfiguration*)config)->mLocale);
        }
    }
    return NOERROR;
}

ECode CResources::GetDisplayMetrics(
    /* [out] */ IDisplayMetrics** metrics)
{
    if (!metrics) {
        return E_INVALID_ARGUMENT;
    }

    *metrics = (IDisplayMetrics*)mMetrics.Get();
    if (*metrics) (*metrics)->AddRef();
    return NOERROR;
}

ECode CResources::GetConfiguration(
    /* [out] */ IConfiguration** config)
{
    if (!config) {
        return E_INVALID_ARGUMENT;
    }

    *config = (IConfiguration*)mConfiguration.Get();
    if (*config) (*config)->AddRef();
    return NOERROR;
}

ECode CResources::GetCompatibilityInfo(
    /* [out] */ ICompatibilityInfo** ci)
{
    if (!ci) {
        return E_INVALID_ARGUMENT;
    }

    *ci = mCompatibilityInfo.Get();
    if (*ci) (*ci)->AddRef();
    return NOERROR;
}

ECode CResources::SetCompatibilityInfo(
    /* [in] */ ICompatibilityInfo* ci)
{
    if (!ci) {
        return E_INVALID_ARGUMENT;
    }

    mCompatibilityInfo = ci;
    return UpdateConfiguration(mConfiguration.Get(), mMetrics.Get());
}

ECode CResources::GetIdentifier(
    /* [in] */ const String& name,
    /* [in] */ const String& defType,
    /* [in] */ const String& defPackage,
    /* [out] */ Int32* id)
{
    if (!id) {
        return E_INVALID_ARGUMENT;
    }
//	    try {
//	        return Integer.parseInt(name);
//	    } catch (Exception e) {
//	        // Ignore
//	    }
    *id = mAssets->GetResourceIdentifier(name, defType, defPackage);
    return NOERROR;
}

ECode CResources::GetResourceName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    mAssets->GetResourceName(resid, name);
    if (!name->IsNull()) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Unable to find resource ID #0x") + resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetResourceCapsuleName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    mAssets->GetResourceCapsuleName(resid, name);
    if (!name->IsNull()) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Unable to find resource ID #0x") + resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetResourceTypeName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    mAssets->GetResourceTypeName(resid, name);
    if (!name->IsNull()) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Unable to find resource ID #0x") + resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetResourceEntryName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    if (!name) {
        return E_INVALID_ARGUMENT;
    }

    mAssets->GetResourceEntryName(resid, name);
    if (!name->IsNull()) {
        return NOERROR;
    }

    Logger::E(TAG, StringBuffer("Unable to find resource ID #0x") + resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::ParseBundleExtras(
    /* [in] */ IXmlResourceParser* parser,
    /* [in, out] */ IBundle* outBundle)
{
//	    int outerDepth = parser.getDepth();
//	    int type;
//	    while ((type=parser.next()) != XmlPullParser.END_DOCUMENT
//	           && (type != XmlPullParser.END_TAG || parser.getDepth() > outerDepth)) {
//	        if (type == XmlPullParser.END_TAG || type == XmlPullParser.TEXT) {
//	            continue;
//	        }
//
//	        String nodeName = parser.getName();
//	        if (nodeName.equals("extra")) {
//	            parseBundleExtra("extra", parser, outBundle);
//	            XmlUtils.skipCurrentTag(parser);
//
//	        } else {
//	            XmlUtils.skipCurrentTag(parser);
//	        }
//	    }
    return E_NOT_IMPLEMENTED;
}

ECode CResources::ParseBundleExtra(
    /* [in] */ const String& tagName,
    /* [in] */ IAttributeSet* attrs,
    /* [in, out] */ IBundle* outBundle)
{
//	    TypedArray sa = obtainAttributes(attrs,
//	            com.android.internal.R.styleable.Extra);
//
//	    String name = sa.getString(
//	            com.android.internal.R.styleable.Extra_name);
//	    if (name == null) {
//	        sa.recycle();
//	        throw new XmlPullParserException("<" + tagName
//	                + "> requires an android:name attribute at "
//	                + attrs.getPositionDescription());
//	    }
//
//	    TypedValue v = sa.peekValue(
//	            com.android.internal.R.styleable.Extra_value);
//	    if (v != null) {
//	        if (v.type == TypedValue.TYPE_STRING) {
//	            CharSequence cs = v.coerceToString();
//	            outBundle.putCharSequence(name, cs);
//	        } else if (v.type == TypedValue.TYPE_INT_BOOLEAN) {
//	            outBundle.putBoolean(name, v.data != 0);
//	        } else if (v.type >= TypedValue.TYPE_FIRST_INT
//	                && v.type <= TypedValue.TYPE_LAST_INT) {
//	            outBundle.putInt(name, v.data);
//	        } else if (v.type == TypedValue.TYPE_FLOAT) {
//	            outBundle.putFloat(name, v.getFloat());
//	        } else {
//	            sa.recycle();
//	            throw new XmlPullParserException("<" + tagName
//	                    + "> only supports string, integer, float, color, and boolean at "
//	                    + attrs.getPositionDescription());
//	        }
//	    } else {
//	        sa.recycle();
//	        throw new XmlPullParserException("<" + tagName
//	                + "> requires an android:value or android:resource attribute at "
//	                + attrs.getPositionDescription());
//	    }
//
//	    sa.recycle();
    return E_NOT_IMPLEMENTED;
}

ECode CResources::GetAssets(
    /* [out] */ IAssetManager** manager)
{
    if (!manager) {
        return E_INVALID_ARGUMENT;
    }

    *manager = (IAssetManager*)mAssets.Get();
    if (*manager) (*manager)->AddRef();
    return NOERROR;
}

ECode CResources::FlushLayoutCache()
{
//	    synchronized (mCachedXmlBlockIds) {
//	        // First see if this block is in our cache.
//	        final int num = mCachedXmlBlockIds.length;
//	        for (int i=0; i<num; i++) {
//	            mCachedXmlBlockIds[i] = -0;
//	            XmlBlock oldBlock = mCachedXmlBlocks[i];
//	            if (oldBlock != null) {
//	                oldBlock.close();
//	            }
//	            mCachedXmlBlocks[i] = null;
//	        }
//	    }
    return E_NOT_IMPLEMENTED;
}

ECode CResources::StartPreloading()
{
    Mutex::Autolock lock(mSyncLock);

    if (mPreloaded) {
        Logger::E(TAG, "Resources already preloaded");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mPreloaded = TRUE;
    mPreloading = TRUE;
    return NOERROR;
}

ECode CResources::FinishPreloading()
{
    if (mPreloading) {
        mPreloading = FALSE;
        FlushLayoutCache();
    }
    return NOERROR;
}

ECode CResources::LoadDrawable(
    /* [in] */ ITypedValue* value,
    /* [in] */ Int32 id,
    /* [out] */ IDrawable** drawable)
{
    if (TRACE_FOR_PRELOAD) {
        // Log only framework resources
//	        if ((id >>> 24) == 0x1) {
//	            final String name = GetResourceName(id);
//	            if (name != null) android.util.Log.d("PreloadDrawable", name);
//	        }
    }

    Int64 key = (((Int64)((CTypedValue*)value)->mAssetCookie) << 32) | ((CTypedValue*)value)->mData;
    GetCachedDrawable(key, drawable);

    if (*drawable != NULL) {
        return NOERROR;
    }

    HashMap<Int64, AutoPtr<IDrawableConstantState> >::Iterator it =
            sPreloadedDrawables->Find(key);
    AutoPtr<IDrawableConstantState> cs =
            it != sPreloadedDrawables->End()? it->mSecond : NULL;
    if (cs != NULL) {
        cs->NewDrawableEx((IResources*)this, drawable);
    }
    else {
        if (((CTypedValue*)value)->mType >= TypedValue_TYPE_FIRST_COLOR_INT &&
                ((CTypedValue*)value)->mType <= TypedValue_TYPE_LAST_COLOR_INT) {
            CColorDrawable::New(((CTypedValue*)value)->mData, (IColorDrawable**)drawable);
        }

        if (*drawable == NULL) {
            if (((CTypedValue*)value)->mString == NULL) {
                Logger::E(TAG, StringBuffer("Resource is not a Drawable (color or path): ") + (Int32)value);
                *drawable = NULL;
                return E_NOT_FOUND_EXCEPTION;
            }

            String file;
            ((CTypedValue*)value)->mString->ToString(&file);

            if (DEBUG_LOAD) {
                Logger::V(TAG, StringBuffer("Loading drawable for cookie ")
                    + ((CTypedValue*)value)->mAssetCookie + ": " + file);
            }

            if (file.EndWith(".xml")) {
//	                try {
                 AutoPtr<IXmlResourceParser> rp;
                 LoadXmlResourceParser(file, id, ((CTypedValue*)value)->mAssetCookie,
                        "drawable", (IXmlResourceParser**)&rp);
                 Drawable::CreateFromXml(this, rp, drawable);
                 rp->Close();
//	                } catch (Exception e) {
//	                    NotFoundException rnf = new NotFoundException(
//	                        "File " + file + " from drawable resource ID #0x"
//	                        + Integer.toHexString(id));
//	                    rnf.initCause(e);
//	                    throw rnf;
//	                }
//
            }
            else {
//	                try {
                AutoPtr<IInputStream> is;
                ASSERT_SUCCEEDED(mAssets->OpenNonAssetEx3(
                    ((CTypedValue*)value)->mAssetCookie, file,
                    AssetManager_ACCESS_STREAMING, (IInputStream**)&is));
    //                System.out.println("Opened file " + file + ": " + is);
                Drawable::CreateFromResourceStream((IResources*)this, value, is,
                        file, NULL, drawable);
                is->Close();
    //                System.out.println("Created stream: " + dr);
//	                } catch (Exception e) {
//	                    NotFoundException rnf = new NotFoundException(
//	                        "File " + file + " from drawable resource ID #0x"
//	                        + Integer.toHexString(id));
//	                    rnf.initCause(e);
//	                    throw rnf;
//	                }
            }
        }
    }

    if (*drawable != NULL) {
        AutoPtr<IDrawableConstantState> cs2;
        (*drawable)->SetChangingConfigurations(((CTypedValue*)value)->mChangingConfigurations);
        (*drawable)->GetConstantState((IDrawableConstantState**)&cs2);
        if (cs2 != NULL) {
            if (mPreloading) {
                (*sPreloadedDrawables)[key] = cs2;
            }
            else {
                Mutex::Autolock lock(mTmpValueLock);
                //Log.i(TAG, "Saving cached drawable @ #" +
                //        Integer.toHexString(key.intValue())
                //        + " in " + this + ": " + cs2);
                mDrawableCache[key] = cs2;
            }
        }
    }

    return NOERROR;
}

ECode CResources::GetCachedDrawable(
    /* [in] */ Int64 key,
    /* [out] */ IDrawable** drawable)
{
    Mutex::Autolock lock(mTmpValueLock);

    HashMap<Int64, AutoPtr<IDrawableConstantState> >::Iterator it = mDrawableCache.Find(key);
    if (it != mDrawableCache.End()) {
        IDrawableConstantState* wr = it->mSecond;
        if (wr != NULL) {   // we have the key
//	        Drawable.ConstantState entry = wr.get();
//	        if (entry != null) {
//	            //Log.i(TAG, "Returning cached drawable @ #" +
//	            //        Integer.toHexString(((Integer)key).intValue())
//	            //        + " in " + this + ": " + entry);
//	            return entry.newDrawable(this);
//	        }
//	        else {  // our entry has been purged
//	            mDrawableCache.delete(key);
//	        }
            wr->NewDrawableEx((IResources*)this, drawable);
            return NOERROR;
        }
    }
    *drawable = NULL;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::LoadColorStateList(
    /* [in] */ ITypedValue* value,
    /* [in] */ Int32 id,
    /* [out] */ IColorStateList** csl)
{
//	    if (TRACE_FOR_PRELOAD) {
//	        // Log only framework resources
//	        if ((id >>> 24) == 0x1) {
//	            final String name = getResourceName(id);
//	            if (name != null) android.util.Log.d("PreloadColorStateList", name);
//	        }
//	    }

    Int32 key = (((CTypedValue*)value)->mAssetCookie << 24) | ((CTypedValue*)value)->mData;

    if (((CTypedValue*)value)->mType >= TypedValue_TYPE_FIRST_COLOR_INT &&
            ((CTypedValue*)value)->mType <= TypedValue_TYPE_LAST_COLOR_INT) {

        HashMap<Int32, AutoPtr<IColorStateList> >::Iterator it =
                mPreloadedColorStateLists->Find(key);
        *csl = it != mPreloadedColorStateLists->End()? it->mSecond : NULL;
        if (*csl != NULL) {
            (*csl)->AddRef();
            return NOERROR;
        }

        assert(SUCCEEDED(CColorStateList::ValueOf(((CTypedValue*)value)->mData, csl)));
        if (mPreloading) {
            (*mPreloadedColorStateLists)[key] = *csl;
        }

        return NOERROR;
    }

    GetCachedColorStateList(key, csl);
    if (*csl != NULL) {
        return NOERROR;
    }

    HashMap<Int32, AutoPtr<IColorStateList> >::Iterator it =
                mPreloadedColorStateLists->Find(key);
    *csl = it != mPreloadedColorStateLists->End()? it->mSecond : NULL;
    if (*csl != NULL) {
        (*csl)->AddRef();
        return NOERROR;
    }

    if (((CTypedValue*)value)->mString == NULL) {
        Logger::E(TAG, StringBuffer("Resource is not a ColorStateList (color or path): ") + (Int32)value);
        *csl = NULL;
        return E_NOT_FOUND_EXCEPTION;
    }

    String file;
    ((CTypedValue*)value)->mString->ToString(&file);

//	    if (file.endsWith(".xml")) {
//	        try {
//	            XmlResourceParser rp = loadXmlResourceParser(
//	                    file, id, value.assetCookie, "colorstatelist");
//	            csl = ColorStateList.createFromXml(this, rp);
//	            rp.close();
//	        } catch (Exception e) {
//	            NotFoundException rnf = new NotFoundException(
//	                "File " + file + " from color state list resource ID #0x"
//	                + Integer.toHexString(id));
//	            rnf.initCause(e);
//	            throw rnf;
//	        }
//	    } else {
//	        throw new NotFoundException(
//	                "File " + file + " from drawable resource ID #0x"
//	                + Integer.toHexString(id) + ": .xml extension required");
//	    }

    if (*csl != NULL) {
        if (mPreloading) {
            (*mPreloadedColorStateLists)[key] = *csl;
        } else {
            Mutex::Autolock lock(mTmpValueLock);

            //Log.i(TAG, "Saving cached color state list @ #" +
            //        Integer.toHexString(key.intValue())
            //        + " in " + this + ": " + csl);
            mColorStateListCache[key] = *csl;
        }
    }

    return NOERROR;
}

ECode CResources::GetCachedColorStateList(
    /* [in] */ Int32 key,
    /* [out] */ IColorStateList** csl)
{
    Mutex::Autolock lock(mTmpValueLock);

    HashMap<Int32, AutoPtr<IColorStateList> >::Iterator it = mColorStateListCache.Find(key);
    *csl = it != mColorStateListCache.End()? it->mSecond : NULL;
    if (*csl != NULL) {   // we have the key
//	        ColorStateList entry = wr.get();
//	        if (entry != null) {
//	            //Log.i(TAG, "Returning cached color state list @ #" +
//	            //        Integer.toHexString(((Integer)key).intValue())
//	            //        + " in " + this + ": " + entry);
//	            return entry;
//	        }
//	        else {  // our entry has been purged
//	            mColorStateListCache.delete(key);
//	        }
        (*csl)->AddRef();
        return NOERROR;
    }
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::LoadXmlResourceParser(
    /* [in] */ Int32 id,
    /* [in] */ const char* type,
    /* [out] */ IXmlResourceParser** parser)
{
    Mutex::Autolock lock(mTmpValueLock);

    GetValue(id, mTmpValue.Get(), TRUE);
    if (CTYPEDVALUE(mTmpValue)->mType == TypedValue_TYPE_STRING) {
        String str;
        CTYPEDVALUE(mTmpValue)->mString->ToString(&str);
        return LoadXmlResourceParser(str, id,
                CTYPEDVALUE(mTmpValue)->mAssetCookie, type, parser);
    }
//    throw new NotFoundException(
//            "Resource ID #0x" + Integer.toHexString(id) + " type #0x"
//            + Integer.toHexString(value.type) + " is not valid");
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::LoadXmlResourceParser(
    /* [in] */ const String& file,
    /* [in] */ Int32 id,
    /* [in] */ Int32 assetCookie,
    /* [in] */ const char* type,
    /* [out] */ IXmlResourceParser** parser)
{
    if (id != 0) {
//    try {
        // These may be compiled...
        Mutex::Autolock lock(mCachedXmlBlockIdsLock);

        // First see if this block is in our cache.
        Int32 num = 4/*mCachedXmlBlockIds.length*/;
        for (Int32 i = 0; i < num; i++) {
            if (mCachedXmlBlockIds[i] == id) {
                //System.out.println("**** REUSING XML BLOCK!  id="
                //                   + id + ", index=" + i);
                return mCachedXmlBlocks[i]->NewParser(parser);
            }
        }

        // Not in the cache, create a new block and put it at
        // the next slot in the cache.
        XmlBlock* block = mAssets->OpenXmlBlockAsset(
                assetCookie, file);
        if (block != NULL) {
            Int32 pos = mLastCachedXmlBlockIndex + 1;
            if (pos >= num) pos = 0;
            mLastCachedXmlBlockIndex = pos;
            XmlBlock* oldBlock = mCachedXmlBlocks[pos];
            if (oldBlock != NULL) {
                oldBlock->Close();
                //todo: should be deleted here?
                delete oldBlock;
            }
            mCachedXmlBlockIds[pos] = id;
            mCachedXmlBlocks[pos] = block;
            //System.out.println("**** CACHING NEW XML BLOCK!  id="
            //                   + id + ", index=" + pos);
            return block->NewParser(parser);
        }
//    } catch (Exception e) {
//        NotFoundException rnf = new NotFoundException(
//                "File " + file + " from xml type " + type + " resource ID #0x"
//                + Integer.toHexString(id));
//        rnf.initCause(e);
//        throw rnf;
//    }
    }

//    throw new NotFoundException(
//        "File " + file + " from xml type " + type + " resource ID #0x"
//        + Integer.toHexString(id));
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetDefaultDisplay(
    /* [in] */ IDisplay* defaultDisplay,
    /* [out] */ IDisplay** display)
{
    if (!defaultDisplay || display) {
        return E_INVALID_ARGUMENT;
    }

    if (mDefaultDisplay == NULL) {
        Boolean isRequired = FALSE;
        Boolean isSupportScreen = FALSE;
        mCompatibilityInfo->IsScalingRequired(&isRequired);
        mCompatibilityInfo->SupportsScreen(&isSupportScreen);
        if (!isRequired && isSupportScreen) {
            // the app supports the display. just use the default one.
            mDefaultDisplay = defaultDisplay;
        } else {
            // display needs adjustment.
            Int32 id = 0;
            defaultDisplay->GetDisplayId(&id);
            CDisplay::CreateMetricsBasedDisplay(id, mMetrics.Get(), (IDisplay**)&mDefaultDisplay);
        }
    }
    *display = mDefaultDisplay.Get();
    (*display)->AddRef();
    return NOERROR;
}

ECode CResources::GetCachedStyledAttributes(
    /* [in] */ Int32 len,
    /* [out] */ ITypedArray** array)
{
    Mutex::Autolock lock(mTmpValueLock);

    AutoPtr<ITypedArray> attrs = mCachedStyledAttributes.Get();
    if (attrs != NULL) {
        mCachedStyledAttributes = NULL;

        CTYPEDARRAY(attrs)->mLength = len;
        Int32 fullLen = len * CAssetManager::STYLE_NUM_ENTRIES;
        if (CTYPEDARRAY(attrs)->mData->GetLength() >= fullLen) {
            *array = attrs.Get();
            (*array)->AddRef();
            return NOERROR;
        }
        CTYPEDARRAY(attrs)->mData = ArrayOf<Int32>::Alloc(fullLen);
        CTYPEDARRAY(attrs)->mIndices = ArrayOf<Int32>::Alloc(1 + len);
        *array = attrs.Get();
        (*array)->AddRef();
        return NOERROR;
    }
    ArrayOf<Int32>* data = ArrayOf<Int32>::Alloc(len * CAssetManager::STYLE_NUM_ENTRIES);
    ArrayOf<Int32>* indices = ArrayOf<Int32>::Alloc(1 + len);
    CTypedArray::New((IResources*)this, *data, *indices, len, (ITypedArray**)&attrs);
    ArrayOf<Int32>::Free(indices);
    ArrayOf<Int32>::Free(data);
    *array = attrs.Get();
    (*array)->AddRef();
    return NOERROR;
}

ECode CResources::constructor(
    /* [in] */ IAssetManager* assets,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ IConfiguration* config)
{
    return constructor(assets, metrics, config, NULL);
}

ECode CResources::constructor(
    /* [in] */ IAssetManager* assets,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ IConfiguration* config,
    /* [in] */ ICompatibilityInfo* compInfo)
{
    if (!assets) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = NOERROR;
    mAssets = (CAssetManager*)assets;

    ec = CDisplayMetrics::NewByFriend((CDisplayMetrics**)&mMetrics);
    if (FAILED(ec)) return ec;

    mMetrics->SetToDefaults();

    if (compInfo == NULL) {
        ec = CompatibilityInfo::GetDefaultCompatibilityInfo(
            (ICompatibilityInfo**)&mCompatibilityInfo);
        if (FAILED(ec)) return ec;
    } else {
        mCompatibilityInfo = compInfo;
    }

    ec = CConfiguration::NewByFriend((CConfiguration**)&mConfiguration);
    if (FAILED(ec)) return ec;

    UpdateConfiguration(config, metrics);
    assets->EnsureStringBlocks();

    return ec;
}

ECode CResources::constructor()
{
    mAssets = CAssetManager::GetSystem();

    // NOTE: Intentionally leaving this uninitialized (all values set
    // to zero), so that anyone who tries to do something that requires
    // metrics will get a very wrong value.
    ECode ec = CConfiguration::NewByFriend((CConfiguration**)&mConfiguration);
    if (FAILED(ec)) return ec;

    mConfiguration->SetToDefaults();

    ec = CDisplayMetrics::NewByFriend((CDisplayMetrics**)&mMetrics);
    if (FAILED(ec)) return ec;

    mMetrics->SetToDefaults();
    UpdateConfiguration(NULL, NULL);
    mAssets->EnsureStringBlocks();

    return CompatibilityInfo::GetDefaultCompatibilityInfo(
        (ICompatibilityInfo**)&mCompatibilityInfo);
}

