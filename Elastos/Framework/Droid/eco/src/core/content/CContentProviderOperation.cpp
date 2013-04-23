
#include "content/CContentProviderOperation.h"

CString CContentProviderOperation::TAG = "CContentProviderOperation";

CContentProviderOperation::CContentProviderOperation()
{}

CContentProviderOperation::~CContentProviderOperation()
{}

ECode CContentProviderOperation::GetUri(
    /* [out] */ IUri** uri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::IsYieldAllowed(
    /* [out] */ Boolean* isAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::GetType(
    /* [out] */ Int32* type)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::IsWriteOperation(
    /* [out] */ Boolean* isWrite)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::IsReadOperation(
    /* [out] */ Boolean* isRead)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::Apply(
    /* [in] */ IContentProvider* provider,
    /* [in] */ const ArrayOf<IContentProviderResult*>& backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out] */ IContentProviderResult** providerResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::ResolveValueBackReferences(
    /* [in] */ const ArrayOf<IContentProviderResult*>& backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out] */ IContentValues** contentValues)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::ResolveSelectionArgsBackReferences(
    /* [in] */ const ArrayOf<IContentProviderResult*>& backRefs,
    /* [in] */ Int32 numBackRefs,
    /* [out, callee] */ ArrayOf<String>** stringArray)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::ToString(
    /* [out] */ String* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::constructor(
    /* [in] */ IContentProviderOperationBuilder* operationBuilder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CContentProviderOperation::constructor(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}
