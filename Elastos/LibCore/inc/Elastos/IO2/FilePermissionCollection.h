#ifndef __FILEPERMISSIONCOLLECTION_H__
#define __FILEPERMISSIONCOLLECTION_H__

#include <elastos.h>
#include <Elastos.System.h>
#include "Elastos.IO2_server.h"

class FilePermissionCollection
{
public:
    FilePermissionCollection();

    virtual ~FilePermissionCollection();

    /**
     * Add a permission object to the permission collection.
     *
     * @param permission
     *            the FilePermission object to add to the collection.
     * @throws IllegalArgumentException
     *             if {@code permission} is not an instance of
     *             {@code FilePermission}.
     * @throws IllegalStateException
     *             if this collection is read-only.
     * @see java.security.PermissionCollection#add(java.security.Permission)
     */
    CARAPI Add();
//        /* [in] */ IPermission *permission);
        
    /**
     * Returns an enumeration for the collection of permissions.
     *
     * @return a permission enumeration for this permission collection.
     * @see java.security.PermissionCollection#elements()
     */
    CARAPI Elements();
//       /* [out] */ Enumeration<Permission> elements);

    /**
     * Indicates whether this permissions collection implies a specific
     * {@code permission}.
     *
     * @param permission
     *            the permission to check.
     * @see java.security.PermissionCollection#implies(java.security.Permission)
     */
    CARAPI Implies(
//        /* [in] */ IPermission *permission,
        /* [out] */ Boolean* IsImplies);

    CARAPI Init();
    
};

#endif //__FILEPERMISSIONCOLLECTION_H__