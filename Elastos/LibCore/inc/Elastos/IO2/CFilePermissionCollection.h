#ifndef __CFILEPERMISSIONCOLLECTION_H__
#define __CFILEPERMISSIONCOLLECTION_H__

#include "_CFilePermissionCollection.h"
#include "FilePermissionCollection.h"

CarClass(CFilePermissionCollection), public FilePermissionCollection
{
public:
    CFilePermissionCollection();

    ~CFilePermissionCollection();

    CARAPI Add();
//   /* [in] */ IPermission *permission);

    CARAPI Elements();
//       /* [out] */ Enumeration<Permission> elements);

    CARAPI Implies(
//        /* [in] */ IPermission *permission,
        /* [out] */ Boolean* IsImplies);

    CARAPI constructor();
};

#endif