
#ifndef __CWEBICONDATABASE_H__
#define __CWEBICONDATABASE_H__

#include "_CWebIconDatabase.h"

CarClass(CWebIconDatabase)
{
public:
    CARAPI Open(
        /* [in] */ CString path);

    CARAPI Close();

    CARAPI RemoveAllIcons();

    CARAPI RequestIconForPageUrl(
        /* [in] */ CString url,
        /* [in] */ IWebIconDatabaseIconListener * pListener);

    CARAPI BulkRequestIconForPageUrl(
        /* [in] */ IContentResolver * pCr,
        /* [in] */ CString where,
        /* [in] */ IWebIconDatabaseIconListener * pListener);

    CARAPI RetainIconForPageUrl(
        /* [in] */ CString url);

    CARAPI ReleaseIconForPageUrl(
        /* [in] */ CString url);

    CARAPI GetInstance(
        /* [out] */ IWebIconDatabase ** ppInstance);

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBICONDATABASE_H__
