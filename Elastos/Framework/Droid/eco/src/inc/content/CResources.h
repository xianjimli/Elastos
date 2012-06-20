
#ifndef __CRESOURCES_H__
#define __CRESOURCES_H__

#include "_CResources.h"
#include "ext/frameworkdef.h"
#include "content/XmlBlock.h"
#include "content/CAssetManager.h"
#include "content/CConfiguration.h"
#include "content/PluralRules.h"
#include "utils/CDisplayMetrics.h"
#include "graphics/Drawable.h"
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::System::Threading;

CarClass(CResources)
{
    friend class CResourcesFactory;
    friend class CTheme;

public:
    CResources();

    virtual ~CResources();

public:
    CARAPI GetText(
        /* [in] */ Int32 id,
        /* [out] */ ICharSequence** seq);

    CARAPI GetQuantityText(
        /* [in] */ Int32 id,
        /* [in] */ Int32 quantity,
        /* [out] */ ICharSequence** seq);

    CARAPI GetString(
        /* [in] */ Int32 id,
        /* [out] */ String* str);

    CARAPI GetQuantityString(
        /* [in] */ Int32 id,
        /* [in] */ Int32 quantity,
        /* [out] */ String* str);

    CARAPI GetTextEx(
        /* [in] */ Int32 id,
        /* [in] */ ICharSequence* def,
        /* [out] */ ICharSequence** seq);

    CARAPI GetTextArray(
        /* [in] */ Int32 id,
        /* [out, callee] */ ArrayOf<String>** seqs);

    CARAPI GetStringArray(
        /* [in] */ Int32 id,
        /* [out, callee] */ ArrayOf<String>** strs);

    CARAPI GetIntArray(
        /* [in] */ Int32 id,
        /* [out, callee] */ ArrayOf<Int32>** ints);

    CARAPI ObtainTypedArray(
        /* [in] */ Int32 id,
        /* [out] */ ITypedArray** array);

    CARAPI GetDimension(
        /* [in] */ Int32 id,
        /* [out] */ Float* dim);

    CARAPI GetDimensionPixelOffset(
        /* [in] */ Int32 id,
        /* [out] */ Int32* offset);

    CARAPI GetDimensionPixelSize(
        /* [in] */ Int32 id,
        /* [out] */ Int32* size);

    CARAPI GetFraction(
        /* [in] */ Int32 id,
        /* [in] */ Int32 base,
        /* [in] */ Int32 pbase,
        /* [out] */ Float* fraction);

    CARAPI GetDrawable(
        /* [in] */ Int32 id,
        /* [out] */ IDrawable** drawable);

    CARAPI GetMovie(
        /* [in] */ Int32 id,
        /* [out] */ IMovie** movie);

    CARAPI GetColor(
        /* [in] */ Int32 id,
        /* [out] */ Int32* color);

    CARAPI GetColorStateList(
        /* [in] */ Int32 id,
        /* [out] */ IColorStateList** list);

    CARAPI GetBoolean(
        /* [in] */ Int32 id,
        /* [out] */ Boolean* b);

    CARAPI GetInteger(
        /* [in] */ Int32 id,
        /* [out] */ Int32* i);

    CARAPI GetLayout(
        /* [in] */ Int32 id,
        /* [out] */ IXmlResourceParser** parser);

    CARAPI GetAnimation(
        /* [in] */ Int32 id,
        /* [out] */ IXmlResourceParser** animation);

    CARAPI GetXml(
        /* [in] */ Int32 id,
        /* [out] */ IXmlResourceParser** xml);

    CARAPI OpenRawResource(
        /* [in] */ Int32 id,
        /* [out] */ IInputStream** res);

    CARAPI OpenRawResourceEx(
        /* [in] */ Int32 id,
        /* [in] */ ITypedValue* value,
        /* [out] */ IInputStream** res);

    CARAPI OpenRawResourceFd(
        /* [in] */ Int32 id,
        /* [out] */ IAssetFileDescriptor** des);

    CARAPI GetValue(
        /* [in] */ Int32 id,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolveRefs);

    CARAPI GetValueEx(
        /* [in] */ String name,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolveRefs);

    CARAPI NewTheme(
        /* [out] */ ITheme** theme);

    CARAPI ObtainAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** array);

    CARAPI UpdateConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ IDisplayMetrics* metrics);

    CARAPI GetDisplayMetrics(
        /* [out] */ IDisplayMetrics** metrics);

    CARAPI GetConfiguration(
        /* [out] */ IConfiguration** config);

    CARAPI GetCompatibilityInfo(
        /* [out] */ ICompatibilityInfo** ci);

    CARAPI SetCompatibilityInfo(
        /* [in] */ ICompatibilityInfo* ci);

    CARAPI GetIdentifier(
        /* [in] */ String name,
        /* [in] */ String defType,
        /* [in] */ String defCapsule,
        /* [out] */ Int32* id);

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

    CARAPI ParseBundleExtras(
        /* [in] */ IXmlResourceParser* parser,
        /* [in, out] */ IBundle* outBundle);

    CARAPI ParseBundleExtra(
        /* [in] */ String tagName,
        /* [in] */ IAttributeSet* attrs,
        /* [in, out] */ IBundle* outBundle);

    CARAPI GetAssets(
        /* [out] */ IAssetManager** manager);

    CARAPI FlushLayoutCache();

    CARAPI StartPreloading();

    CARAPI FinishPreloading();

    CARAPI GetDefaultDisplay(
        /* [in] */ IDisplay* defaultDisplay,
        /* [out] */ IDisplay** display);

    CARAPI constructor(
        /* [in] */ IAssetManager* assets,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ IConfiguration* config);

    CARAPI constructor(
        /* [in] */ IAssetManager* assets,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ IConfiguration* config,
        /* [in] */ ICompatibilityInfo* compInfo);

    CARAPI constructor();

public: /*package*/
    CARAPI LoadDrawable(
        /* [in] */ ITypedValue* value,
        /* [in] */ Int32 id,
        /* [out] */ IDrawable** drawable);

    CARAPI LoadColorStateList(
        /* [in] */ ITypedValue* value,
        /* [in] */ Int32 id,
        /* [out] */ IColorStateList** list);

    CARAPI LoadXmlResourceParser(
        /* [in] */ Int32 id,
        /* [in] */ String type,
        /* [out] */ IXmlResourceParser** parser);

    CARAPI LoadXmlResourceParser(
        /* [in] */ String file,
        /* [in] */ Int32 id,
        /* [in] */ Int32 assetCookie,
        /* [in] */ String type,
        /* [out] */ IXmlResourceParser** parser);

private:
    CARAPI_(PluralRules*) GetPluralRule();

    CARAPI GetCachedDrawable(
        /* [in] */ Int64 key,
        /* [out] */ IDrawable** drawable);

    CARAPI GetCachedColorStateList(
        /* [in] */ Int32 key,
        /* [out] */ IColorStateList** csl);

    CARAPI GetCachedStyledAttributes(
        /* [in] */ Int32 len,
        /* [out] */ ITypedArray** attrs);

public: /*package*/
    static const String TAG;

    AutoPtr<ITypedValue> mTmpValue;
    Mutex mTmpValueLock;

    AutoPtr<ITypedArray> mCachedStyledAttributes;

    AutoPtr<CAssetManager> mAssets;

    AutoPtr<CDisplayMetrics> mMetrics;

private:
    static const Boolean DEBUG_LOAD = FALSE;
    static const Boolean DEBUG_CONFIG = FALSE;
    static const Boolean TRACE_FOR_PRELOAD = FALSE;
    // TODO:
    static const Int32 sSdkVersion = 0; // Build.VERSION.SDK_INT + ("REL".equals(Build.VERSION.CODENAME) ? 0 : 1);

    static Mutex mSyncLock;
    static AutoPtr<IResources> mSystem;

    // Information about preloaded resources.  Note that they are not
    // protected by a lock, because while preloading in zygote we are all
    // single-threaded, and after that these are immutable.
//	    private static final LongSparseArray<Drawable.ConstantState> sPreloadedDrawables
//	            = new LongSparseArray<Drawable.ConstantState>();
    static HashMap<Int64, AutoPtr<IDrawableConstantState> >* sPreloadedDrawables;
//	    private static final SparseArray<ColorStateList> mPreloadedColorStateLists
//	            = new SparseArray<ColorStateList>();
    static HashMap<Int32, AutoPtr<IColorStateList> >* mPreloadedColorStateLists;
    static Boolean mPreloaded;

    // These are protected by the mTmpValue lock.
    HashMap<Int64, IDrawableConstantState*> mDrawableCache;
    HashMap<Int32, IColorStateList*> mColorStateListCache;
    Boolean mPreloading;

    Int32 mLastCachedXmlBlockIndex;
    Int32 mCachedXmlBlockIds[4];
    Mutex mCachedXmlBlockIdsLock;
    XmlBlock* mCachedXmlBlocks[4];

    AutoPtr<CConfiguration> mConfiguration;

    PluralRules* mPluralRule;

    AutoPtr<ICompatibilityInfo> mCompatibilityInfo;
    AutoPtr<IDisplay> mDefaultDisplay;
};

#endif // __CRESOURCES_H__

