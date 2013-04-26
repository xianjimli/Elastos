
#include "content/CContentProviderOperationHelper.h"
#include "ext/frameworkdef.h"
#include "content/CContentProviderOperationBuilder.h"

ECode CContentProviderOperationHelper::NewInsert(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderOperationBuilder** operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder);
    FAIL_RETURN(CContentProviderOperationBuilder::New(ContentProviderOperation_TYPE_INSERT, uri, operationBuilder));
    return NOERROR;
}

ECode CContentProviderOperationHelper::NewUpdate(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderOperationBuilder** operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder);
    FAIL_RETURN(CContentProviderOperationBuilder::New(ContentProviderOperation_TYPE_UPDATE, uri, operationBuilder));
    return NOERROR;
}

ECode CContentProviderOperationHelper::NewDelete(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderOperationBuilder** operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder);
    FAIL_RETURN(CContentProviderOperationBuilder::New(ContentProviderOperation_TYPE_DELETE, uri, operationBuilder));
    return NOERROR;
}

ECode CContentProviderOperationHelper::NewAssertQuery(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderOperationBuilder** operationBuilder)
{
    VALIDATE_NOT_NULL(operationBuilder);
    FAIL_RETURN(CContentProviderOperationBuilder::New(ContentProviderOperation_TYPE_ASSERT, uri, operationBuilder));
    return NOERROR;
}

