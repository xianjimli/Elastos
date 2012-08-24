
#include "cmdef.h"
#include "CBlockGuard.h"

PInterface CBlockGuard::LaxPolicy::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IPolicy*)this;
    }
    else if (riid == EIID_IPolicy) {
        return (IPolicy*)this;
    }
    return NULL;
}

UInt32 CBlockGuard::LaxPolicy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CBlockGuard::LaxPolicy::Release()
{
    return ElRefBase::Release();
}

ECode CBlockGuard::LaxPolicy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IPolicy*)this) {
        *pIID = EIID_IPolicy;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

/**
 * Called on disk writes.
 */
ECode CBlockGuard::LaxPolicy::OnWriteToDisk()
{
    return NOERROR;
}

/**
 * Called on disk writes.
 */
ECode CBlockGuard::LaxPolicy::OnReadFromDisk()
{
    return NOERROR;
}

/**
 * Called on network operations.
 */
ECode CBlockGuard::LaxPolicy::OnNetwork()
{
    return NOERROR;
}

/**
 * Returns the policy bitmask, for shipping over Binder calls
 * to remote threads/processes and reinstantiating the policy
 * there.  The bits in the mask are from the DISALLOW_* and
 * PENALTY_* constants.
 */
ECode CBlockGuard::LaxPolicy::GetPolicyMask(
    /* [out] */ Int32* mask)
{
    return NOERROR;
}

/**
 * Get the current thread's policy.
 *
 * @return the current thread's policy.  Never returns null.
 *     Will return the LAX_POLICY instance if nothing else is set.
 */
ECode CBlockGuard::GetThreadPolicy(
    /* [out] */ IPolicy** policy)
{
//    return threadPolicy.get();
    VALIDATE_NOT_NULL(policy);
    *policy = new LaxPolicy();
    if (*policy == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    else {
        (*policy)->AddRef();
    }
    return NOERROR;
}

/**
 * Sets the current thread's block guard policy.
 *
 * @param policy policy to set.  May not be null.  Use the public LAX_POLICY
 *   if you want to unset the active policy.
 */
ECode CBlockGuard::SetThreadPolicy(
    /* [in] */ IPolicy* policy)
{
//    if (policy == null) {
//        throw new NullPointerException("policy == null");
//    }
//    threadPolicy.set(policy);
    return E_NOT_IMPLEMENTED;
}
