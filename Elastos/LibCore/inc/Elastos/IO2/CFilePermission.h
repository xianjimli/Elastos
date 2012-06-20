#ifndef __CFILEPERMISSION_H__
#define __CFILEPERMISSION_H__

#include "_CFilePermission.h"
#include "FilePermission.h"

using namespace Elastos;

CarClass(CFilePermission), public FilePermission
{
public:
    CFilePermission();

    ~CFilePermission();

    CARAPI constructor(
        /* [in] */ String path,
        /* [in] */ String actions);

    CARAPI GetActions(
        /* [out] */ String* actions);

    CARAPI Equals(
        /* [in] */ IObject* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI Implies(
//        /* [in] */ IPermission* p,
        /* [out] */ Boolean* isimplied);

    CARAPI NewPermissionCollection();
//        /* [out] */ PermissionCollection* object);

    CARAPI HashCode(
		/* [out] */ Int32* hasCode);

private:    
};

#endif // __CFILEPERMISSION_H__