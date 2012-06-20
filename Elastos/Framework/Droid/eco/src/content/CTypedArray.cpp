
#include "content/CTypedArray.h"
#include <ext/frameworkdef.h>
#include <content/CResources.h>
#include <content/CAssetManager.h>
#include <utils/CTypedValue.h>
#include <utils/XmlUtils.h>
#include "utils/AutoString.h"
#include <elastos/AutoFree.h>
#include <elastos/Math.h>
#include <StringBuffer.h>
#include <Logger.h>
#include <stdlib.h>

using namespace Elastos::System;
using namespace Elastos::Utility::Logging;

#define CRESOURCES(x) ((CResources*)x.Get())
#define CASSETMANAGER(x) ((CAssetManager*)x.Get())
#define CTYPEDVALUE(x) ((CTypedValue*)x.Get())
#define CTYPEDARRAY(x) ((CTypedArray*)x.Get())

CTypedArray::CTypedArray()
    : mXml(NULL)
    , mRsrcs(NULL)
    , mData(NULL)
    , mIndices(NULL)
    , mLength(0)
{
    ASSERT_SUCCEEDED(CTypedValue::New((ITypedValue**)&mValue));
}

CTypedArray::~CTypedArray()
{
}

ECode CTypedArray::Length(
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len);
    *len = mLength;
    return NOERROR;
}

ECode CTypedArray::GetIndexCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = (*mIndices)[0];
    return NOERROR;
}

ECode CTypedArray::GetIndex(
    /* [in] */ Int32 at,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = (*mIndices)[1+at];
    return NOERROR;
}

ECode CTypedArray::GetResources(
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(res);
    assert(mResources != NULL);

    *res = mResources.Get();
    (*res)->AddRef();
    return NOERROR;
}

ECode CTypedArray::GetText(
    /* [in] */ Int32 index,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *csq = NULL;
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_STRING) {
        return LoadStringValueAt(index, csq);
    }

    AutoPtr<ITypedValue> v = mValue;
    if (GetValueAt(index, v.Get())) {
        Logger::W(CResources::TAG, StringBuffer("Converting to string: ") + (Int32)v.Get());
        return v->CoerceToString(csq);
    }
    Logger::W(CResources::TAG, "getString of bad type: " + type);
    *csq = NULL;
    return NOERROR;
}

ECode CTypedArray::GetString(
    /* [in] */ Int32 index,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    ECode ec = NOERROR;
    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *str = NULL;
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_STRING) {
        AutoPtr<ICharSequence> csq;
        ec = LoadStringValueAt(index, (ICharSequence**)&csq);
        if (FAILED(ec)) {
            *str = NULL;
            return ec;
        }
        return csq->ToString(str);
    }

    AutoPtr<ITypedValue> v = mValue;
    if (GetValueAt(index, v.Get())) {
        Logger::W(CResources::TAG, StringBuffer("Converting to string: ") + (Int32)v.Get());
        AutoPtr<ICharSequence> csq;
        ec = v->CoerceToString((ICharSequence**)&csq);
        if (FAILED(ec)) {
            *str = NULL;
            return ec;
        }
        return csq->ToString(str);
    }
    Logger::W(CResources::TAG, StringBuffer("getString of bad type: ") + type);
    *str = NULL;
    return NOERROR;
}

ECode CTypedArray::GetNonResourceString(
    /* [in] */ Int32 index,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_STRING) {
        const Int32 cookie = (*data)[index+CAssetManager::STYLE_ASSET_COOKIE];
        if (cookie < 0) {
            AutoPtr<ICharSequence> csq;
            ECode ec = mXml->GetPooledString(
                (*data)[index+CAssetManager::STYLE_DATA],
                (ICharSequence**)&csq);
            if (FAILED(ec)) return ec;
            return csq->ToString(str);
        }
    }
    *str = NULL;
    return NOERROR;
}

ECode CTypedArray::GetNonConfigurationString(
    /* [in] */ Int32 index,
    /* [in] */ Int32 allowedChangingConfigs,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    ECode ec = NOERROR;
    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];

    if (((*data)[index + CAssetManager::STYLE_CHANGING_CONFIGURATIONS]
        & ~allowedChangingConfigs) != 0) {
        *str = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (type == TypedValue_TYPE_NULL) {
        *str = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else if (type == TypedValue_TYPE_STRING) {
        AutoPtr<ICharSequence> csq;
        ec = LoadStringValueAt(index, (ICharSequence**)&csq);
        if (FAILED(ec)) {
            *str = NULL;
            return ec;
        }
        return csq->ToString(str);
    }

    AutoPtr<ITypedValue> v = mValue;
    if (GetValueAt(index, v.Get())) {
        Logger::W(CResources::TAG, StringBuffer("Converting to string: ") + (Int32)v.Get());
        AutoPtr<ICharSequence> csq;
        ec = v->CoerceToString((ICharSequence**)&csq);
        if (FAILED(ec)) {
            *str = NULL;
            return ec;
        }
        return csq->ToString(str);
    }
    Logger::W(CResources::TAG, StringBuffer("getString of bad type: 0x") + type);
    *str = NULL;
    return NOERROR;
}

ECode CTypedArray::GetBoolean(
    /* [in] */ Int32 index,
    /* [in] */ Boolean defValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type >= TypedValue_TYPE_FIRST_INT
        && type <= TypedValue_TYPE_LAST_INT) {
        *value = (*data)[index+CAssetManager::STYLE_DATA] != 0;
        return NOERROR;
    }

    AutoPtr<ITypedValue> v = mValue;
    if (GetValueAt(index, v.Get())) {
        Logger::W(CResources::TAG, StringBuffer("Converting to boolean: ") + (Int32)v.Get());
        AutoPtr<ICharSequence> csq;
        v->CoerceToString((ICharSequence**)&csq);
        *value = XmlUtils::ConvertValueToBoolean(csq, defValue);
        return NOERROR;
    }
    Logger::W(CResources::TAG, StringBuffer("getBoolean of bad type: 0x") + type);
    *value = defValue;
    return NOERROR;
}

ECode CTypedArray::GetInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type >= TypedValue_TYPE_FIRST_INT
        && type <= TypedValue_TYPE_LAST_INT) {
        *value = (*data)[index+CAssetManager::STYLE_DATA];
        return NOERROR;
    }

    AutoPtr<ITypedValue> v = mValue;
    if (GetValueAt(index, v.Get())) {
        Logger::W(CResources::TAG, StringBuffer("Converting to int: ") + (Int32)v.Get());
        AutoPtr<ICharSequence> csq;
        v->CoerceToString((ICharSequence**)&csq);
        *value = XmlUtils::ConvertValueToInt32(csq, defValue);
        return NOERROR;
    }
    Logger::W(CResources::TAG, StringBuffer("getInt of bad type: 0x") + type);
    *value = defValue;
    return NOERROR;
}

ECode CTypedArray::GetFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float defValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    ECode ec = NOERROR;
    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_FLOAT) {
        *value = Math::Int32BitsToFloat((*data)[index+CAssetManager::STYLE_DATA]);
        return NOERROR;
    }
    else if (type >= TypedValue_TYPE_FIRST_INT
        && type <= TypedValue_TYPE_LAST_INT) {
        *value = (Float)(*data)[index+CAssetManager::STYLE_DATA];
        return NOERROR;
    }

    AutoPtr<ITypedValue> v = mValue;
    if (GetValueAt(index, v.Get())) {
        Logger::W(CResources::TAG, StringBuffer("Converting to float: ") + (Int32)v.Get());
        AutoPtr<ICharSequence> csq;
        ec = v->CoerceToString((ICharSequence**)&csq);
        if (SUCCEEDED(ec)) {
            AutoString str;
            csq->ToString(&str);
            *value = atof((const char*)str);
            return NOERROR;
        }
    }
    Logger::W(CResources::TAG, StringBuffer("getFloat of bad type: 0x") + type);
    *value = defValue;
    return NOERROR;
}

ECode CTypedArray::GetColor(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type >= TypedValue_TYPE_FIRST_INT
        && type <= TypedValue_TYPE_LAST_INT) {
        *value = (*data)[index+CAssetManager::STYLE_DATA];
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_STRING) {
        const AutoPtr<ITypedValue> v = mValue;
        if (GetValueAt(index, v.Get())) {
            AutoPtr<IColorStateList> csl;
            CRESOURCES(mResources)->LoadColorStateList(
                    v, CTYPEDVALUE(v)->mResourceId, (IColorStateList**)&csl);
            return csl->GetDefaultColor(value);
        }
        *value = defValue;
        return NOERROR;
    }

    Logger::E(CResources::TAG, StringBuffer("Can't convert to color: type=0x") + type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetColorStateList(
    /* [in] */ Int32 index,
    /* [out] */ IColorStateList** list)
{
    VALIDATE_NOT_NULL(list);

    const AutoPtr<ITypedValue> value = mValue;
    if (GetValueAt(index*CAssetManager::STYLE_NUM_ENTRIES, value.Get())) {
        return CRESOURCES(mResources)->LoadColorStateList(value.Get(), CTYPEDVALUE(value)->mResourceId, list);
    }
    *list = NULL;
    return NOERROR;
}

ECode CTypedArray::GetInteger(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type >= TypedValue_TYPE_FIRST_INT
        && type <= TypedValue_TYPE_LAST_INT) {
        *value = (*data)[index+CAssetManager::STYLE_DATA];
        return NOERROR;
    }

    Logger::E(CResources::TAG, StringBuffer("Can't convert to integer: type=0x") + type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetDimension(
    /* [in] */ Int32 index,
    /* [in] */ Float defValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_DIMENSION) {
        *value = CTypedValue::ComplexToDimension(
            (*data)[index+CAssetManager::STYLE_DATA], CRESOURCES(mResources)->mMetrics);
        return NOERROR;
    }

    Logger::E(CResources::TAG, StringBuffer("Can't convert to dimension: type=0x") + type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetDimensionPixelOffset(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_DIMENSION) {
        *value = CTypedValue::ComplexToDimensionPixelOffset(
            (*data)[index+CAssetManager::STYLE_DATA], CRESOURCES(mResources)->mMetrics);
        return NOERROR;
    }

    Logger::E(CResources::TAG, StringBuffer("Can't convert to dimension: type=0x") + type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetDimensionPixelSize(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_DIMENSION) {
        *value = CTypedValue::ComplexToDimensionPixelSize(
            (*data)[index+CAssetManager::STYLE_DATA], CRESOURCES(mResources)->mMetrics);
        return NOERROR;
    }

    Logger::E(CResources::TAG, StringBuffer("Can't convert to dimension: type=0x") + type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetLayoutDimension(
    /* [in] */ Int32 index,
    /* [in] */ String name,
    /* [out] */ Int32* dimension)
{
    VALIDATE_NOT_NULL(dimension);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type >= TypedValue_TYPE_FIRST_INT
            && type <= TypedValue_TYPE_LAST_INT) {
        *dimension = (*data)[index + CAssetManager::STYLE_DATA];
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_DIMENSION) {
        *dimension = CTypedValue::ComplexToDimensionPixelSize(
            (*data)[index + CAssetManager::STYLE_DATA], CRESOURCES(mResources)->mMetrics);
        return NOERROR;
    }

    String des;
    GetPositionDescription(&des);
    Logger::E(CResources::TAG, StringBuffer("") + des + ": You must supply a " + name + " attribute.");
    String::Free(des);
    return E_RUNTIME_EXCEPTION;
}

ECode CTypedArray::GetLayoutDimensionEx(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* dimension)
{
    VALIDATE_NOT_NULL(dimension);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type >= TypedValue_TYPE_FIRST_INT
            && type <= TypedValue_TYPE_LAST_INT) {
        *dimension = (*data)[index+CAssetManager::STYLE_DATA];
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_DIMENSION) {
        *dimension = CTypedValue::ComplexToDimensionPixelSize(
            (*data)[index+CAssetManager::STYLE_DATA], CRESOURCES(mResources)->mMetrics);
        return NOERROR;
    }

    *dimension = defValue;
    return NOERROR;
}

ECode CTypedArray::GetFraction(
    /* [in] */ Int32 index,
    /* [in] */ Int32 base,
    /* [in] */ Int32 pbase,
    /* [in] */ Float defValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == TypedValue_TYPE_FRACTION) {
        *value = CTypedValue::ComplexToFraction(
            (*data)[index+CAssetManager::STYLE_DATA], base, pbase);
        return NOERROR;
    }

    Logger::E(CResources::TAG, StringBuffer("Can't convert to fraction: type=0x") + type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetResourceId(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    if ((*data)[index + CAssetManager::STYLE_TYPE] != TypedValue_TYPE_NULL) {
        const Int32 resid = (*data)[index + CAssetManager::STYLE_RESOURCE_ID];
        if (resid != 0) {
            *value = resid;
            return NOERROR;
        }
    }
    *value = defValue;
    return NOERROR;
}

ECode CTypedArray::GetDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    const AutoPtr<ITypedValue> value = mValue;
    if (GetValueAt(index*CAssetManager::STYLE_NUM_ENTRIES, value.Get())) {
//	        if (FALSE) {
//	            System.out.println("******************************************************************");
//	            System.out.println("Got drawable resource: type="
//	                               + value.type
//	                               + " str=" + value.string
//	                               + " int=0x" + Integer.toHexString(value.data)
//	                               + " cookie=" + value.assetCookie);
//	            System.out.println("******************************************************************");
//	        }
        return CRESOURCES(mResources)->LoadDrawable(value.Get(), CTYPEDVALUE(value)->mResourceId, drawable);
    }
    *drawable = NULL;
    return NOERROR;
}

ECode CTypedArray::GetTextArray(
    /* [in] */ Int32 index,
    /* [out, callee] */ ArrayOf<String>** array)
{
    const AutoPtr<ITypedValue> value = mValue;
    if (GetValueAt(index*CAssetManager::STYLE_NUM_ENTRIES, value.Get())) {
//	        if (false) {
//	            System.out.println("******************************************************************");
//	            System.out.println("Got drawable resource: type="
//	                               + value.type
//	                               + " str=" + value.string
//	                               + " int=0x" + Integer.toHexString(value.data)
//	                               + " cookie=" + value.assetCookie);
//	            System.out.println("******************************************************************");
//	        }
        return mResources->GetTextArray(CTYPEDVALUE(value)->mResourceId, array);
    }
    *array = NULL;
    return NOERROR;
}

ECode CTypedArray::GetValue(
    /* [in] */ Int32 index,
    /* [in, out] */ ITypedValue* outValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(outValue);
    VALIDATE_NOT_NULL(value);

    *value = GetValueAt(index*CAssetManager::STYLE_NUM_ENTRIES, outValue);
    return NOERROR;
}

ECode CTypedArray::HasValue(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* hasValue)
{
    VALIDATE_NOT_NULL(hasValue);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    const ArrayOf<Int32>* data = mData;
    const Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    *hasValue = type != TypedValue_TYPE_NULL;
    return NOERROR;
}

ECode CTypedArray::PeekValue(
    /* [in] */ Int32 index,
    /* [out] */ ITypedValue** value)
{
    VALIDATE_NOT_NULL(value);

    const AutoPtr<ITypedValue> v = mValue;
    if (GetValueAt(index*CAssetManager::STYLE_NUM_ENTRIES, v.Get())) {
        *value = v;
        (*value)->AddRef();
        return NOERROR;
    }
    *value = NULL;
    return NOERROR;
}

ECode CTypedArray::GetPositionDescription(
    /* [out] */ String* des)
{
    VALIDATE_NOT_NULL(des);

    if (mXml != NULL) {
        return mXml->GetPositionDescription(des);
    }
    *des = String::Duplicate("<internal>");
    return NOERROR;
}

ECode CTypedArray::Recycle()
{
    Mutex::Autolock lock(CRESOURCES(mResources)->mTmpValueLock);
    AutoPtr<ITypedArray> cached = CRESOURCES(mResources)->mCachedStyledAttributes;
    if (cached == NULL || CTYPEDARRAY(cached)->mData->GetLength() < mData->GetLength()) {
        mXml = NULL;
        CRESOURCES(mResources)->mCachedStyledAttributes = (ITypedArray*)this;
    }
    return NOERROR;
}

Boolean CTypedArray::GetValueAt(
    /* [in] */ Int32 index,
    /* [in, out] */ ITypedValue* outValue)
{
    assert(outValue);

    ArrayOf<Int32>* data = mData;
    Int32 type = (*data)[index+CAssetManager::STYLE_TYPE];
    if (type == TypedValue_TYPE_NULL) {
        return FALSE;
    }
    ((CTypedValue*)outValue)->mType = type;
    ((CTypedValue*)outValue)->mData = (*data)[index+CAssetManager::STYLE_DATA];
    ((CTypedValue*)outValue)->mAssetCookie = (*data)[index+CAssetManager::STYLE_ASSET_COOKIE];
    ((CTypedValue*)outValue)->mResourceId = (*data)[index+CAssetManager::STYLE_RESOURCE_ID];
    ((CTypedValue*)outValue)->mChangingConfigurations = (*data)[index+CAssetManager::STYLE_CHANGING_CONFIGURATIONS];
    ((CTypedValue*)outValue)->mDensity = (*data)[index+CAssetManager::STYLE_DENSITY];
    if (type == TypedValue_TYPE_STRING) {
        return SUCCEEDED(LoadStringValueAt(index,
                (ICharSequence**)&((CTypedValue*)outValue)->mString)) ? TRUE : FALSE;
    }
    else {
        ((CTypedValue*)outValue)->mString = NULL;
    }
    return TRUE;
}

ECode CTypedArray::LoadStringValueAt(
    /* [in] */ Int32 index,
    /* [out] */ ICharSequence** csq)
{
    assert(csq);

    ArrayOf<Int32>* data = mData;
    Int32 cookie = (*data)[index + CAssetManager::STYLE_ASSET_COOKIE];
    if (cookie < 0) {
        if (mXml != NULL) {
            return mXml->GetPooledString((*data)[index + CAssetManager::STYLE_DATA], csq);
        }
        *csq = NULL;
        return NOERROR;
    }
    //System.out.println("Getting pooled from: " + v);
    return CASSETMANAGER(CRESOURCES(mResources)->mAssets)->GetPooledString(
        cookie, (*data)[index+CAssetManager::STYLE_DATA], csq);
}

ECode CTypedArray::GetDescription(
    /* [out] */ String* des)
{
//	    return Arrays.toString(mData);
    return E_NOT_IMPLEMENTED;
}

ECode CTypedArray::constructor(
    /* [in] */ IResources* resources,
    /* [in] */ const ArrayOf<Int32> & data,
    /* [in] */ const ArrayOf<Int32> & indices,
    /* [in] */ Int32 len)
{
    VALIDATE_NOT_NULL(resources);

    mResources = resources;
    mData = data.Clone();
    mIndices = indices.Clone();
    mLength = len;
    return NOERROR;
}

