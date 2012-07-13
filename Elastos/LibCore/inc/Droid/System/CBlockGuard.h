
#ifndef __CBLOCKGUARD_H__
#define __CBLOCKGUARD_H__

#include "_CBlockGuard.h"
#include <elastos/ElRefBase.h>

CarClass(CBlockGuard)
{
public:
    /**
     * The default, permissive policy that doesn't prevent any operations.
     */
    class LaxPolicy : public ElRefBase, public IPolicy
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        /**
         * Called on disk writes.
         */
        CARAPI OnWriteToDisk();

        /**
         * Called on disk writes.
         */
        CARAPI OnReadFromDisk();

        /**
         * Called on network operations.
         */
        CARAPI OnNetwork();

        /**
         * Returns the policy bitmask, for shipping over Binder calls
         * to remote threads/processes and reinstantiating the policy
         * there.  The bits in the mask are from the DISALLOW_* and
         * PENALTY_* constants.
         */
        CARAPI GetPolicyMask(
            /* [out] */ Int32* mask);
    };

public:
    /**
     * Get the current thread's policy.
     *
     * @return the current thread's policy.  Never returns null.
     *     Will return the LAX_POLICY instance if nothing else is set.
     */
    CARAPI GetThreadPolicy(
        /* [out] */ IPolicy** policy);

    /**
     * Sets the current thread's block guard policy.
     *
     * @param policy policy to set.  May not be null.  Use the public LAX_POLICY
     *   if you want to unset the active policy.
     */
    CARAPI SetThreadPolicy(
        /* [in] */ IPolicy* policy);
};

#endif //__CBLOCKGUARD_H__