
#include "content/CTheme.h"
#include "content/CTypedArray.h"
#include "content/XmlBlock.h"

CTheme::CTheme()
    : mTheme(NULL)
{
}

ECode CTheme::ApplyStyle(
    /* [in] */ Int32 resid,
    /* [in] */ Boolean force)
{
    CAssetManager::ApplyThemeStyle(mTheme, resid, force);
    return NOERROR;
}

ECode CTheme::SetTo(
    /* [in] */ ITheme* other)
{
    CAssetManager::CopyTheme(mTheme, ((CTheme*)other)->mTheme);
    return NOERROR;
}

ECode CTheme::ObtainStyledAttributes(
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);

    Int32 len = attrs.GetLength();
    AutoPtr<ITypedArray> _array;
    AutoPtr<CTypedArray> array;
    ASSERT_SUCCEEDED(mResources->GetCachedStyledAttributes(len, (ITypedArray**)&_array));
    array = (CTypedArray*)_array.Get();
    array->mRsrcs = attrs.Clone();
    CAssetManager::ApplyStyle(mTheme, 0, 0, NULL, attrs,
            *(array->mData), *(array->mIndices));
    *styles = _array.Get();
    (*styles)->AddRef();
    return NOERROR;
}

ECode CTheme::ObtainStyledAttributesEx(
    /* [in] */ Int32 resid,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);

    Int32 len = attrs.GetLength();
    AutoPtr<ITypedArray> _array;
    AutoPtr<CTypedArray> array;
    ASSERT_SUCCEEDED(mResources->GetCachedStyledAttributes(len, (ITypedArray**)&_array));
    array = (CTypedArray*)_array.Get();
    array->mRsrcs = attrs.Clone();

    CAssetManager::ApplyStyle(mTheme, 0, resid, NULL, attrs,
            *(array->mData), *(array->mIndices));
//    if (false) {
//        int[] data = array.mData;
//
//        System.out.println("**********************************************************");
//        System.out.println("**********************************************************");
//        System.out.println("**********************************************************");
//        System.out.println("Attributes:");
//        String s = "  Attrs:";
//        int i;
//        for (i=0; i<attrs.length; i++) {
//            s = s + " 0x" + Integer.toHexString(attrs[i]);
//        }
//        System.out.println(s);
//        s = "  Found:";
//        TypedValue value = new TypedValue();
//        for (i=0; i<attrs.length; i++) {
//            int d = i*AssetManager.STYLE_NUM_ENTRIES;
//            value.type = data[d+AssetManager.STYLE_TYPE];
//            value.data = data[d+AssetManager.STYLE_DATA];
//            value.assetCookie = data[d+AssetManager.STYLE_ASSET_COOKIE];
//            value.resourceId = data[d+AssetManager.STYLE_RESOURCE_ID];
//            s = s + " 0x" + Integer.toHexString(attrs[i])
//                + "=" + value;
//        }
//        System.out.println(s);
//    }
    *styles = _array.Get();
    (*styles)->AddRef();
    return NOERROR;
}

ECode CTheme::ObtainStyledAttributesEx2(
    /* [in] */ IAttributeSet* set,
    /* [in] */ const ArrayOf<Int32>& attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);

    Int32 len = attrs.GetLength();
    AutoPtr<ITypedArray> _array;
    AutoPtr<CTypedArray> array;
    ASSERT_SUCCEEDED(mResources->GetCachedStyledAttributes(len, (ITypedArray**)&_array));
    array = (CTypedArray*)_array.Get();

    // XXX note that for now we only work with compiled XML files.
    // To support generic XML files we will need to manually parse
    // out the attributes from the XML file (applying type information
    // contained in the resources and such).
    XmlBlock::Parser* parser = (XmlBlock::Parser*)set;
    CAssetManager::ApplyStyle(
        mTheme, defStyleAttr, defStyleRes,
        parser != NULL ? parser->mParseState : NULL, attrs,
                *(array->mData), *(array->mIndices));

    array->mRsrcs = attrs.Clone();
    array->mXml = parser;

//    if (false) {
//        int[] data = array.mData;
//
//        System.out.println("Attributes:");
//        String s = "  Attrs:";
//        int i;
//        for (i=0; i<set.getAttributeCount(); i++) {
//            s = s + " " + set.getAttributeName(i);
//            int id = set.getAttributeNameResource(i);
//            if (id != 0) {
//                s = s + "(0x" + Integer.toHexString(id) + ")";
//            }
//            s = s + "=" + set.getAttributeValue(i);
//        }
//        System.out.println(s);
//        s = "  Found:";
//        TypedValue value = new TypedValue();
//        for (i=0; i<attrs.length; i++) {
//            int d = i*AssetManager.STYLE_NUM_ENTRIES;
//            value.type = data[d+AssetManager.STYLE_TYPE];
//            value.data = data[d+AssetManager.STYLE_DATA];
//            value.assetCookie = data[d+AssetManager.STYLE_ASSET_COOKIE];
//            value.resourceId = data[d+AssetManager.STYLE_RESOURCE_ID];
//            s = s + " 0x" + Integer.toHexString(attrs[i])
//                + "=" + value;
//        }
//        System.out.println(s);
//    }
    *styles = _array.Get();
    (*styles)->AddRef();
    return NOERROR;
}

ECode CTheme::ResolveAttribute(
    /* [in] */ Int32 resid,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs,
    /* [out] */ Boolean* isFounded)
{
    VALIDATE_NOT_NULL(isFounded);

    *isFounded = mAssets->GetThemeValue(mTheme, resid, outValue, resolveRefs);
//    if (false) {
//        System.out.println(
//            "resolveAttribute #" + Integer.toHexString(resid)
//            + " got=" + got + ", type=0x" + Integer.toHexString(outValue.type)
//            + ", data=0x" + Integer.toHexString(outValue.data));
//    }
    return NOERROR;
}

ECode CTheme::Dump(
    /* [in] */ Int32 priority,
    /* [in] */ const String& tag,
    /* [in] */ const String& prefix)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CTheme::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IAssetManager* assets)
{
    mResources = (CResources*)res;
    mAssets = (CAssetManager*)assets;
    mTheme = mAssets->CreateTheme();
    return NOERROR;
}

