
#include "content/CContentProviderOperation.h"
#include "content/CContentProviderResult.h"
#include "content/ContentUris.h"
#include "content/CContentValues.h"
#include "ext/frameworkdef.h"
#include "text/TextUtils.h"
#include <Logger.h>

using namespace Elastos::Utility::Logging;

#define FAIL_WithGoto(ec) do { \
                                if (FAILED(ec)) {   \
                                    goto EXIT;      \
                                }                   \
                            } while (0)

CString CContentProviderOperation::TAG = "CContentProviderOperation";

CContentProviderOperation::CContentProviderOperation()
    : mType(0)
    , mExpectedCount(0)
    , mSelectionArgsBackReferences(NULL)
    , mYieldAllowed(FALSE)
{}

CContentProviderOperation::~CContentProviderOperation()
{
    if (NULL != mSelectionArgsBackReferences) {
        mSelectionArgsBackReferences->Clear();
        delete mSelectionArgsBackReferences;
    }
}

ECode CContentProviderOperation::GetUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    *uri = mUri;
    return NOERROR;
}

ECode CContentProviderOperation::IsYieldAllowed(
    /* [out] */ Boolean* isAllowed)
{
    VALIDATE_NOT_NULL(isAllowed);
    *isAllowed = mYieldAllowed;
    return NOERROR;
}

ECode CContentProviderOperation::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CContentProviderOperation::IsWriteOperation(
    /* [out] */ Boolean* isWrite)
{
    VALIDATE_NOT_NULL(isWrite);
    *isWrite = mType == ContentProviderOperation_TYPE_DELETE || mType == ContentProviderOperation_TYPE_INSERT
            || mType == ContentProviderOperation_TYPE_UPDATE;
    return NOERROR;
}

ECode CContentProviderOperation::IsReadOperation(
    /* [out] */ Boolean* isRead)
{
    VALIDATE_NOT_NULL(isRead);
    *isRead = mType == ContentProviderOperation_TYPE_ASSERT;
    return NOERROR;
}

ECode CContentProviderOperation::Apply(
    /* [in] */ IContentProvider* provider,
    /* [in] */ ArrayOf<IContentProviderResult*>* backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out] */ IContentProviderResult** providerResult)
{
    VALIDATE_NOT_NULL(provider);
    VALIDATE_NOT_NULL(providerResult);
    AutoPtr<IContentValues> values;
    FAIL_RETURN(ResolveValueBackReferences(backRefs, numBackRefs, (IContentValues**)&values));
    AutoFree<ArrayOf<String> > selectionArgs;
    FAIL_RETURN(ResolveSelectionArgsBackReferences(backRefs, numBackRefs, (ArrayOf<String>**)&selectionArgs));

    if (mType == ContentProviderOperation_TYPE_INSERT) {
        AutoPtr<IUri> newUri;
        FAIL_RETURN(provider->Insert(mUri, values, (IUri**)&newUri));
        if (NULL == newUri) {
            //throw new OperationApplicationException("insert failed");
            return E_OPERATION_APPLICATION_EXCEPTION;
        }
        FAIL_RETURN(CContentProviderResult::New(newUri, providerResult));
        return NOERROR;
    }

    Int32 numRows;

    if (mType == ContentProviderOperation_TYPE_DELETE) {
        FAIL_RETURN(provider->Delete(mUri, mSelection, *selectionArgs, &numRows));
    } else if (mType == ContentProviderOperation_TYPE_UPDATE) {
        FAIL_RETURN(provider->Update(mUri, values, mSelection, *selectionArgs, &numRows));
    } else if (mType == ContentProviderOperation_TYPE_ASSERT) {
        // Assert that all rows match expected values
        AutoFree<ArrayOf<String> > projection;
        if (NULL != values) {
            // Build projection map from expected values
            AutoPtr<IObjectStringMap> objStringMap;
            AutoPtr<IObjectContainer> objContainer;
            FAIL_RETURN(values->ValueSet((IObjectStringMap**)&objStringMap));

            if (NULL != objStringMap) {
                FAIL_RETURN(objStringMap->GetAllItems((ArrayOf<String>**)&projection, (IObjectContainer**)&objContainer));
            }

        }

        AutoPtr<ICursor> cursor;
        String sortOrder;
        FAIL_RETURN(provider->Query(mUri, *projection, mSelection, *selectionArgs, sortOrder, (ICursor**)&cursor));
        ECode ec;
        ec = cursor->GetCount(&numRows);
        FAIL_WithGoto(ec);

        if (NULL != projection) {
            Boolean hasNext;
            while ((ec = cursor->MoveToNext(&hasNext), hasNext)) {
                FAIL_WithGoto(ec);
                for (Int32 i = 0; i < projection->GetLength(); i++) {
                    String cursorValue;
                    ec = cursor->GetString(i, &cursorValue);
                    FAIL_WithGoto(ec);
                    String expectedValue;
                    ec = values->GetString((*projection)[i], &expectedValue);
                    FAIL_WithGoto(ec);
                    AutoPtr<ICharSequence> charSeq;
                    AutoPtr<ICharSequence> charSeq2;
                    ec = CStringWrapper::New(cursorValue, (ICharSequence**) &charSeq);
                    FAIL_WithGoto(ec);
                    ec = CStringWrapper::New(expectedValue, (ICharSequence**) &charSeq2);
                    FAIL_WithGoto(ec);

                    if (!TextUtils::Equals(charSeq, charSeq2)) {
                        // Throw exception when expected values don't match
                        String str;
                        ec = ToString(&str);
                        FAIL_WithGoto(ec);
                        Logger::E(TAG, str);
                        //throw new OperationApplicationException("Found value " + cursorValue
                        //        + " when expected " + expectedValue + " for column "
                        //        + projection[i]);
                        ec = E_OPERATION_APPLICATION_EXCEPTION;
                        FAIL_WithGoto(ec);
                    }
                }
            }
        }

        EXIT:
        FAIL_RETURN(cursor->Close());
        FAIL_RETURN(ec);
    } else {
        String str;
        FAIL_RETURN(ToString(&str));
        Logger::E(TAG, str);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mExpectedCount != 0 && mExpectedCount != numRows) {
        String str;
        FAIL_RETURN(ToString(&str));
        Logger::E(TAG, str);
        //throw new OperationApplicationException("wrong number of rows: " + numRows);
        return E_OPERATION_APPLICATION_EXCEPTION;
    }

    FAIL_RETURN(CContentProviderResult::New(numRows, providerResult));
    return NOERROR;
}

ECode CContentProviderOperation::ResolveValueBackReferences(
    /* [in] */ ArrayOf<IContentProviderResult*>* backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out] */ IContentValues** contentValues)
{
    VALIDATE_NOT_NULL(backRefs);
    VALIDATE_NOT_NULL(contentValues);

    if (NULL == mValuesBackReferences) {
        *contentValues = mValues;
        return NOERROR;
    }

    if (NULL == mValues) {
        FAIL_RETURN(CContentValues::New(contentValues));
    } else {
        FAIL_RETURN(CContentValues::New(mValues, contentValues));
    }

    AutoPtr<IObjectStringMap> objStringMap;
    AutoPtr<IObjectContainer> objContainer;
    FAIL_RETURN(mValuesBackReferences->ValueSet((IObjectStringMap**)&objStringMap));

    if (NULL != objStringMap) {
        AutoFree<ArrayOf<String> > keyArray;
        FAIL_RETURN(objStringMap->GetAllItems((ArrayOf<String>**)&keyArray, (IObjectContainer**)&objContainer));
        Int32 len = keyArray->GetLength();
        for (Int32 i = 0; i < len; i++) {
            String key = (*keyArray)[i];
            Int32 backRefIndex;
            FAIL_RETURN(mValuesBackReferences->GetInt32(key, &backRefIndex));
            if (0 == backRefIndex) {
                String str;
                FAIL_RETURN(ToString(&str));
                Logger::E(TAG, str);
                // throw new IllegalArgumentException("values backref " + key + " is not an integer");
                return E_OPERATION_APPLICATION_EXCEPTION;
            }
            Int64 backRefValue;
            FAIL_RETURN(BackRefToValue(backRefs, numBackRefs, backRefIndex, &backRefValue));
            FAIL_RETURN((*contentValues)->PutInt64(key, backRefValue));
        }
    }

    return NOERROR;
}

ECode CContentProviderOperation::ResolveSelectionArgsBackReferences(
    /* [in] */ ArrayOf<IContentProviderResult*>* backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out, callee] */ ArrayOf<String>** stringArray)
{
    VALIDATE_NOT_NULL(backRefs);
    VALIDATE_NOT_NULL(stringArray);

    if (NULL == mSelectionArgsBackReferences) {
        *stringArray = mSelectionArgs;
        return NOERROR;
    }

    //System.arraycopy(mSelectionArgs, 0, newArgs, 0, mSelectionArgs.length);
    (*stringArray)->Copy(mSelectionArgs);
    HashMap<Int32, Int32>::Iterator iter = mSelectionArgsBackReferences->Begin();

    for (; iter != mSelectionArgsBackReferences->End(); ++iter) {
        const Int32 selectionArgIndex = iter->mFirst;
        const Int32 backRefIndex = iter->mSecond;
        Int64 backRefValue;
        FAIL_RETURN(BackRefToValue(backRefs, numBackRefs, backRefIndex, &backRefValue));
        (**stringArray)[selectionArgIndex] = String::FromInt64(backRefValue);
    }

    return NOERROR;
}

ECode CContentProviderOperation::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    String str1("mType: ");
    String strType;
    String strUri("null");
    String strExpectedCount;
    String strYieldAllowed;
    String strValues("null");
    String strValuesBackReferences("null");
    String strSelectionArgsBackReferences("null");
    strType = String::FromInt32(mType);

    if (NULL != mUri) {
        FAIL_RETURN(mUri->ToString(&strUri));
    }

    strExpectedCount = String::FromInt32(mExpectedCount);
    strYieldAllowed = mYieldAllowed ? String("true") : String("false");

    if (NULL != mValues) {
        FAIL_RETURN(mValues->ToString(&strValues));
    }

    if (NULL != mValuesBackReferences) {
        FAIL_RETURN(mValuesBackReferences->ToString(&strValuesBackReferences));
    }

    if (NULL != mSelectionArgsBackReferences) {
        if (mSelectionArgsBackReferences->IsEmpty()) {
            strSelectionArgsBackReferences.SetTo("{}");
        } else {
            String tmpStr("");
            tmpStr.Append("{");
            String key;
            String value;
            HashMap<Int32, Int32>::Iterator iter = mSelectionArgsBackReferences->Begin();
            for (; iter != mSelectionArgsBackReferences->End(); ++iter) {
                key = String::FromInt32(iter->mFirst);
                value = String::FromInt32(iter->mSecond);
                tmpStr.Append(key);
                tmpStr.Append("=");
                tmpStr.Append(value);
                tmpStr.Append(", ");
            }

            Int32 len = tmpStr.GetLength();
            strSelectionArgsBackReferences = tmpStr.Substring(0, len - 2);
            strSelectionArgsBackReferences.Append("}");
        }
    }

    str1.Append(strType);
    str1.Append(", mUri: ");
    str1.Append(strUri);
    str1.Append(", mSelection: ");

    if (mSelection.IsNull()) {
        str1.Append("null");
    } else {
        str1.Append(mSelection);
    }
    
    str1.Append(", mExpectedCount: ");
    str1.Append(strExpectedCount);
    str1.Append(", mYieldAllowed: ");
    str1.Append(strYieldAllowed);
    str1.Append(", mValues: ");
    str1.Append(strValues);
    str1.Append(", mValuesBackReferences: ");
    str1.Append(strValuesBackReferences);
    str1.Append(", mSelectionArgsBackReferences: ");
    str1.Append(strSelectionArgsBackReferences);
    *result = str1;
    return NOERROR;
}

ECode CContentProviderOperation::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mType);
    //TODO
    //Uri.writeToParcel(dest, mUri); 

    if (NULL != mValues) {
        dest->WriteInt32(1);
        //mValues.writeToParcel(dest, 0);
    } else {
        dest->WriteInt32(0);
    }

    if (!mSelection.IsNull()) {
        dest->WriteInt32(1);
        dest->WriteString(mSelection);
    } else {
        dest->WriteInt32(0);
    }

    if (NULL != mSelectionArgs) {
        dest->WriteInt32(1);
        dest->WriteArrayOfString(*mSelectionArgs);
    } else {
        dest->WriteInt32(0);
    }

    if (mExpectedCount != 0) {
        dest->WriteInt32(1);
        dest->WriteInt32(mExpectedCount);
    } else {
        dest->WriteInt32(0);
    }

    if (NULL != mValuesBackReferences) {
        dest->WriteInt32(1);
        //mValuesBackReferences.writeToParcel(dest, 0);
    } else {
        dest->WriteInt32(0);
    }

    if (NULL != mSelectionArgsBackReferences) {
        dest->WriteInt32(1);
        dest->WriteInt32(mSelectionArgsBackReferences->GetSize());
        HashMap<Int32, Int32>::Iterator iter = mSelectionArgsBackReferences->Begin();
        for (; iter != mSelectionArgsBackReferences->End(); ++iter) {
            dest->WriteInt32(iter->mFirst);
            dest->WriteInt32(iter->mSecond);
        }
    } else {
        dest->WriteInt32(0);
    }

    dest->WriteInt32(mYieldAllowed ? 1 : 0);
    return NOERROR;
}

ECode CContentProviderOperation::constructor(
    /* [in] */ IContentProviderOperationBuilder* operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder);
    Boolean hasNext;
    Int32 key;
    Int32 value;
    AutoPtr<IObjectContainer> selectionArgsBackRef;
    AutoPtr<IObjectEnumerator> ObjEnumerator;
    AutoPtr<IIntegerMapWrapper> wrapper;
    FAIL_RETURN(operationBuilder->GetType(&mType));
    FAIL_RETURN(operationBuilder->GetUri((IUri**)&mUri));
    FAIL_RETURN(operationBuilder->GetValues((IContentValues**)&mValues));
    FAIL_RETURN(operationBuilder->GetSelection(&mSelection));
    FAIL_RETURN(operationBuilder->GetSelectionArgs((ArrayOf<String>**)&mSelectionArgs));
    FAIL_RETURN(operationBuilder->GetExpectedCount(&mExpectedCount));
    FAIL_RETURN(operationBuilder->GetSelectionArgsBackReferences((IObjectContainer**)&selectionArgsBackRef));

    if (NULL != selectionArgsBackRef) {
        FAIL_RETURN(selectionArgsBackRef->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator));
        mSelectionArgsBackReferences = new HashMap<Int32, Int32>();

        while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
            FAIL_RETURN(ObjEnumerator->Current((IInterface**) &wrapper));
            FAIL_RETURN(wrapper->GetKey(&key));
            FAIL_RETURN(wrapper->GetValue(&value));
            (*mSelectionArgsBackReferences)[key] = value;
        }

    }

    FAIL_RETURN(operationBuilder->GetValuesBackReferences((IContentValues**)&mValuesBackReferences));
    FAIL_RETURN(operationBuilder->GetYieldAllowed(&mYieldAllowed));
    return NOERROR;
}

ECode CContentProviderOperation::constructor(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    Int32 tmpInt;
    source->ReadInt32(&mType);
    //TODO
    //mUri = Uri.CREATOR.createFromParcel(source);
    //mValues = (source->ReadInt32(&tmpInt),tmpInt) != 0 ? ContentValues.CREATOR.createFromParcel(source) : NULL;
    if ((source->ReadInt32(&tmpInt), tmpInt) != 0) {
        source->ReadString(&mSelection);
    }

    if ((source->ReadInt32(&tmpInt), tmpInt) != 0) {
        source->ReadArrayOfString((Handle32*) &mSelectionArgs);
    }

    if ((source->ReadInt32(&tmpInt), tmpInt) != 0) {
        source->ReadInt32(&mExpectedCount);
    }

    if ((source->ReadInt32(&tmpInt), tmpInt) != 0) {
        //ContentValues.CREATOR.createFromParcel(source);
    }

    if ((source->ReadInt32(&tmpInt), tmpInt) != 0) {
        mSelectionArgsBackReferences = new HashMap<Int32, Int32>();
    }

    if (NULL != mSelectionArgsBackReferences) {
        source->ReadInt32(&tmpInt);
        const Int32 count = tmpInt;
        Int32 key;
        Int32 value;
        for (Int32 i = 0; i < count; i++) {
            source->ReadInt32(&key);
            source->ReadInt32(&value);
            (*mSelectionArgsBackReferences)[key] = value;
        }
    }

    mYieldAllowed = (source->ReadInt32(&tmpInt), tmpInt) != 0;
    return NOERROR;
}

ECode CContentProviderOperation::BackRefToValue(
    /* [in] */ ArrayOf<IContentProviderResult*>* backRefs,
    /* [in] */ const Int32 numBackRefs,
    /* [in] */ const Int32 backRefIndex,
    /* [out] */ Int64* backRefValue)
{
    VALIDATE_NOT_NULL(backRefs);
    VALIDATE_NOT_NULL(backRefValue);

    if (backRefIndex >= numBackRefs) {
        String str;
        FAIL_RETURN(ToString(&str));
        Logger::E(TAG, str);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr<IContentProviderResult> backRef = (*backRefs)[backRefIndex];
    Int64 tmpBackRefValue;
    AutoPtr<IUri> uri;
    FAIL_RETURN(backRef->GetUri((IUri**)&uri));

    if (NULL != uri) {
        tmpBackRefValue = ContentUris::ParseId(uri);
    } else {
        FAIL_RETURN(backRef->GetCount((Int32*)&tmpBackRefValue));
    }

    *backRefValue = tmpBackRefValue;
    return NOERROR;
}
