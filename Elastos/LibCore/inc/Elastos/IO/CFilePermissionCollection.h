#ifndef __CFILEPERMISSIONCOLLECTION_H__
#define __CFILEPERMISSIONCOLLECTION_H__

#include "cmdef.h"
#include "_CFilePermissionCollection.h"
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>

CarClass(CFilePermissionCollection)
{
public:
    CFilePermissionCollection();

    ~CFilePermissionCollection();

    CARAPI Add(
        /* [in] */ IPermission* permission);

    CARAPI Elements(
        /* [out] */ IObjectEnumerator** elements);

    CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* result);

private:
    static const Int64 sSerialVersionUID;

    Vector<AutoPtr<IPermission> > mPermissions;
};

#endif