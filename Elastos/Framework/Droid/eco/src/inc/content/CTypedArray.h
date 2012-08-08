
#ifndef __CTYPEDARRAY_H__
#define __CTYPEDARRAY_H__

#include "_CTypedArray.h"
#include <elastos/AutoPtr.h>
#include <content/XmlBlock.h>

CarClass(CTypedArray)
{
public:
    CTypedArray();

    virtual ~CTypedArray();

public:
    CARAPI Length(
        /* [out] */ Int32* len);

    CARAPI GetIndexCount(
        /* [out] */ Int32* count);

    CARAPI GetIndex(
        /* [in] */ Int32 at,
        /* [out] */ Int32* index);

    CARAPI GetResources(
        /* [out] */ IResources** res);

    CARAPI GetText(
        /* [in] */ Int32 index,
        /* [out] */ ICharSequence** csq);

    CARAPI GetString(
        /* [in] */ Int32 index,
        /* [out] */ String* str);

    CARAPI GetNonResourceString(
        /* [in] */ Int32 index,
        /* [out] */ String* str);

    CARAPI GetNonConfigurationString(
        /* [in] */ Int32 index,
        /* [in] */ Int32 allowedChangingConfigs,
        /* [out] */ String* str);

    CARAPI GetBoolean(
        /* [in] */ Int32 index,
        /* [in] */ Boolean defValue,
        /* [out] */ Boolean* value);

    CARAPI GetInt32(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* value);

    CARAPI GetFloat(
        /* [in] */ Int32 index,
        /* [in] */ Float defValue,
        /* [out] */ Float* value);

    CARAPI GetColor(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* value);

    CARAPI GetColorStateList(
        /* [in] */ Int32 index,
        /* [out] */ IColorStateList** list);

    CARAPI GetInteger(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* value);

    CARAPI GetDimension(
        /* [in] */ Int32 index,
        /* [in] */ Float defValue,
        /* [out] */ Float* value);

    CARAPI GetDimensionPixelOffset(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* value);

    CARAPI GetDimensionPixelSize(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* value);

    CARAPI GetLayoutDimension(
        /* [in] */ Int32 index,
        /* [in] */ const String& name,
        /* [out] */ Int32* dimension);

    CARAPI GetLayoutDimensionEx(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* dimension);

    CARAPI GetFraction(
        /* [in] */ Int32 index,
        /* [in] */ Int32 base,
        /* [in] */ Int32 pbase,
        /* [in] */ Float defValue,
        /* [out] */ Float* value);

    CARAPI GetResourceId(
        /* [in] */ Int32 index,
        /* [in] */ Int32 defValue,
        /* [out] */ Int32* value);

    CARAPI GetDrawable(
        /* [in] */ Int32 index,
        /* [out] */ IDrawable** drawable);

    CARAPI GetTextArray(
        /* [in] */ Int32 index,
        /* [out, callee] */ ArrayOf<ICharSequence*>** array);

    CARAPI GetValue(
        /* [in] */ Int32 index,
        /* [in, out] */ ITypedValue* outValue,
        /* [out] */ Boolean* value);

    CARAPI HasValue(
        /* [in] */ Int32 index,
        /* [out] */ Boolean* hasValue);

    CARAPI PeekValue(
        /* [in] */ Int32 index,
        /* [out] */ ITypedValue** value);

    CARAPI GetPositionDescription(
        /* [out] */ String* des);

    CARAPI Recycle();

    CARAPI GetDescription(
        /* [out] */ String* des);

    CARAPI constructor(
        /* [in] */ IResources* resources,
        /* [in] */ const ArrayOf<Int32> & data,
        /* [in] */ const ArrayOf<Int32> & indices,
        /* [in] */ Int32 len);

public:
    /*package*/ AutoPtr<XmlBlock::Parser> mXml;
    /*package*/ ArrayOf<Int32>* mRsrcs;
    /*package*/ ArrayOf<Int32>* mData;
    /*package*/ ArrayOf<Int32>* mIndices;
    /*package*/ Int32           mLength;

private:
    CARAPI_(Boolean) GetValueAt(
        /* [in] */ Int32 index,
        /* [in, out] */ ITypedValue* outValue);

    CARAPI LoadStringValueAt(
        /* [in] */ Int32 index,
        /* [out] */ ICharSequence** csq);

private:
    AutoPtr<IResources>     mResources;
    AutoPtr<ITypedValue>    mValue;
};

#endif // __CTYPEDARRAY_H__
