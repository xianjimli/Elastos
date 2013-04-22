
#ifndef __CACCOUNTMANAGERHELPER_H__
#define __CACCOUNTMANAGERHELPER_H__

#include "_CAccountManagerHelper.h"

CarClass(CAccountManagerHelper)
{
public:
    /**
     * Gets an AccountManager instance associated with a Context.
     * The {@link Context} will be used as long as the AccountManager is
     * active, so make sure to use a {@link Context} whose lifetime is
     * commensurate with any listeners registered to
     * {@link #addOnAccountsUpdatedListener} or similar methods.
     *
     * <p>It is safe to call this method from the main thread.
     *
     * <p>No permission is required to call this method.
     *
     * @param context The {@link Context} to use when necessary
     * @return An {@link AccountManager} instance
     */
    CARAPI Get(
        /* [in] */ IContext* context,
        /* [out] */ ILocalAccountManager** accountManager);
};

#endif //__CACCOUNTMANAGERHELPER_H__
