
#include "content/CContentProviderOperationBuilder.h"
#include "ext/frameworkdef.h"
#include "content/CIntegerMapWrapper.h"
#include "content/CContentProviderOperation.h"
#include "utils/CParcelableObjectContainer.h"
#include "content/CContentValues.h"


CContentProviderOperationBuilder::CContentProviderOperationBuilder()
{}

CContentProviderOperationBuilder::~CContentProviderOperationBuilder()
{
    ArrayOf<String>::Free(mSelectionArgs);

    if(NULL != mSelectionArgsBackReferences){
        mSelectionArgsBackReferences->Clear();
        delete mSelectionArgsBackReferences;
    }
}

ECode CContentProviderOperationBuilder::Build(
    /* [out] */ IContentProviderOperation** providerOperation)
{
    VALIDATE_NOT_NULL(providerOperation);
    Int32 size;

    if (mType == ContentProviderOperation_TYPE_UPDATE) {
        if ((NULL == mValues || (mValues->GetSize(&size), size) == 0)
                && (NULL == mValuesBackReferences || (mValuesBackReferences->GetSize(&size), size) == 0)) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    if (mType == ContentProviderOperation_TYPE_ASSERT) {
        if ((NULL == mValues || (mValues->GetSize(&size), size) == 0)
                && (NULL == mValuesBackReferences || (mValuesBackReferences->GetSize(&size), size) == 0) && (mExpectedCount == 0)) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    FAIL_RETURN(CContentProviderOperation::New((IContentProviderOperationBuilder*)this, providerOperation));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueBackReferences(
    /* [in] */ IContentValues* backReferences)
{
    VALIDATE_NOT_NULL(backReferences);

    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mValuesBackReferences = backReferences;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueBackReference(
    /* [in] */ const String& key,
    /* [in] */ Int32 previousResult)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (NULL == mValuesBackReferences) {
        FAIL_RETURN(CContentValues::New((IContentValues**)&mValuesBackReferences));
    }

    FAIL_RETURN(mValuesBackReferences->PutInt32(key, previousResult));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithSelectionBackReference(
    /* [in] */ Int32 selectionArgIndex,
    /* [in] */ Int32 previousResult)
{
    if (mType != ContentProviderOperation_TYPE_UPDATE && mType != ContentProviderOperation_TYPE_DELETE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (NULL == mSelectionArgsBackReferences) {
        mSelectionArgsBackReferences = new HashMap<Int32, Int32>();
    }

    (*mSelectionArgsBackReferences)[selectionArgIndex] = previousResult;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValues(
    /* [in] */ IContentValues* values)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutAll(values));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValue(
    /* [in] */ const String& key,
    /* [in] */ const String& stringValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (stringValue.IsNull()) {
        FAIL_RETURN(mValues->PutNull(key));
    } else {
        FAIL_RETURN(mValues->PutString(key, stringValue));
    }

    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueEx(
    /* [in] */ const String& key,
    /* [in] */ Byte byteValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutByte(key, byteValue));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueEx2(
    /* [in] */ const String& key,
    /* [in] */ Int16 shortValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutInt16(key, shortValue));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueEx3(
    /* [in] */ const String& key,
    /* [in] */ Int32 intValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutInt32(key, intValue));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueEx4(
    /* [in] */ const String& key,
    /* [in] */ Int64 longValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutInt64(key, longValue));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueEx5(
    /* [in] */ const String& key,
    /* [in] */ Float floatValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutFloat(key, floatValue));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueEx6(
    /* [in] */ const String& key,
    /* [in] */ Double doubleValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutDouble(key, doubleValue));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueEx7(
    /* [in] */ const String& key,
    /* [in] */ Boolean booleanValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutBoolean(key, booleanValue));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithValueEx8(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<Byte>* arrayValue)
{
    if (mType != ContentProviderOperation_TYPE_INSERT && mType != ContentProviderOperation_TYPE_UPDATE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(mValues->PutByteArray(key, arrayValue));
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithSelection(
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs)
{
    if (mType != ContentProviderOperation_TYPE_UPDATE && mType != ContentProviderOperation_TYPE_DELETE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mSelection = selection;
    if (NULL == selectionArgs) {
        mSelectionArgs = NULL;
    } else {
        mSelectionArgs = ArrayOf<String>::Alloc(selectionArgs->GetLength());
        //System.arraycopy(selectionArgs, 0, mSelectionArgs, 0, selectionArgs.length);
        mSelectionArgs->Copy(selectionArgs);
    }

    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithExpectedCount(
    /* [in] */ Int32 count)
{
    if (mType != ContentProviderOperation_TYPE_UPDATE && mType != ContentProviderOperation_TYPE_DELETE
            && mType != ContentProviderOperation_TYPE_ASSERT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mExpectedCount = count;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::WithYieldAllowed(
    /* [in] */ Boolean yieldAllowed)
{
    mYieldAllowed = yieldAllowed;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = mUri;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetSelection(
    /* [out] */ String* selection)
{
    VALIDATE_NOT_NULL(selection);
    *selection = mSelection;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetSelectionArgs(
    /* [out, callee] */ ArrayOf<String>** selectionArgs)
{
    VALIDATE_NOT_NULL(selectionArgs);
    *selectionArgs = mSelectionArgs;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetValues(
    /* [out] */ IContentValues** values)
{
    VALIDATE_NOT_NULL(values);
    *values = mValues;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetExpectedCount(
    /* [out] */ Int32* expectedCount)
{
    VALIDATE_NOT_NULL(expectedCount);
    *expectedCount = mExpectedCount;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetValuesBackReferences(
    /* [out] */ IContentValues** valuesBackReferences)
{
    VALIDATE_NOT_NULL(valuesBackReferences);
    *valuesBackReferences = mValuesBackReferences;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetSelectionArgsBackReferences(
    /* [out] */ IObjectContainer** selectionArgsBackRef)
{
    VALIDATE_NOT_NULL(selectionArgsBackRef);
    FAIL_RETURN(CParcelableObjectContainer::New(selectionArgsBackRef));

    if (NULL != mSelectionArgsBackReferences) {
        HashMap<Int32, Int32>::Iterator iter = mSelectionArgsBackReferences->Begin();
        for (; iter != mSelectionArgsBackReferences->End(); ++iter) {
            AutoPtr<IIntegerMapWrapper> wrapper;
            FAIL_RETURN(CIntegerMapWrapper::New(iter->mFirst, iter->mSecond, (IIntegerMapWrapper**)&wrapper));
            (*selectionArgsBackRef)->Add(wrapper.Get());
        }
    }

    return NOERROR;
}

ECode CContentProviderOperationBuilder::GetYieldAllowed(
    /* [out] */ Boolean* yieldAllowed)
{
    VALIDATE_NOT_NULL(yieldAllowed);
    *yieldAllowed = mYieldAllowed;
    return NOERROR;
}

ECode CContentProviderOperationBuilder::constructor(
    /* [in] */ Int32 type,
    /* [in] */ IUri* uri)
{
    VALIDATE_NOT_NULL(uri);
    mType = type;
    mUri = uri;
    return NOERROR;
}
