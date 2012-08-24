#include <elastos.h>
#include <container.h>
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class CObjectEnumerator :
    public ElRefBase,
    public IObject,
    public IObjectEnumerator
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Aggregate(
        /* [in] */ AggregateType type,
        /* [in] */ PInterface pObj);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObj);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI Current(
        /* [out] */ PInterface *ppObj);

    CARAPI MoveNext(
        /* [out] */ Boolean *pSucceeded);

    CARAPI Reset();

    CObjectEnumerator(
        ObjectNode* pHeader,
        Mutex* pLock,
        Int32* pState)
    {
        mHeader = mCurrent = pHeader;
        mLock = pLock;
        mState = pState;
        mOrgState = *pState;
    };

    virtual ~CObjectEnumerator() {};

private:
    ObjectNode*         mHeader;
    ObjectNode*         mCurrent;
    Mutex*              mLock;
    Int32*              mState;
    Int32               mOrgState;
};
