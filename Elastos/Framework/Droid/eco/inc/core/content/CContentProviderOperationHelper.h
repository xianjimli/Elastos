
#ifndef __CCONTENTPROVIDEROPERATIONHELPER_H__
#define __CCONTENTPROVIDEROPERATIONHELPER_H__

#include "_CContentProviderOperationHelper.h"

CarClass(CContentProviderOperationHelper)
{
public:
    /**
     * Create a {@link Builder} suitable for building an insert {@link ContentProviderOperation}.
     * @param uri The {@link Uri} that is the target of the insert.
     * @return a {@link Builder}
     */
    CARAPI NewInsert(
        /* [in] */ IUri* uri,
        /* [out] */ IContentProviderOperationBuilder** operationBuilder);

    /**
     * Create a {@link Builder} suitable for building an update {@link ContentProviderOperation}.
     * @param uri The {@link Uri} that is the target of the update.
     * @return a {@link Builder}
     */
    CARAPI NewUpdate(
        /* [in] */ IUri* uri,
        /* [out] */ IContentProviderOperationBuilder** operationBuilder);

    /**
     * Create a {@link Builder} suitable for building a delete {@link ContentProviderOperation}.
     * @param uri The {@link Uri} that is the target of the delete.
     * @return a {@link Builder}
     */
    CARAPI NewDelete(
        /* [in] */ IUri* uri,
        /* [out] */ IContentProviderOperationBuilder** operationBuilder);

    /**
     * Create a {@link Builder} suitable for building a
     * {@link ContentProviderOperation} to assert a set of values as provided
     * through {@link Builder#withValues(ContentValues)}.
     */
    CARAPI NewAssertQuery(
        /* [in] */ IUri* uri,
        /* [out] */ IContentProviderOperationBuilder** operationBuilder);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCONTENTPROVIDEROPERATIONHELPER_H__
