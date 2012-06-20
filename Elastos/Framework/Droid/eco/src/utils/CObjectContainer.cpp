
#include <elatypes.h>
#include <elapi.h>
#include "utils/CObjectContainer.h"

CObjectContainer::CObjectContainer() :
        mCount(0), mState(0)
{}

CObjectContainer::~CObjectContainer()
{
    this->Dispose();
}

ECode CObjectContainer::Add(
    /* [in] */ IInterface *pObject)
{
    ObjectNode *pNode;

    SelfLock();

    if (NULL == pObject) return NOERROR;

    pNode = new ObjectNode(pObject);
    if (NULL == pNode) {
        SelfUnlock();
        return E_OUT_OF_MEMORY;
    }
    pObject->AddRef();

    SimpleContainer::Add(pNode);
    mCount++;
    mState++;

    SelfUnlock();

    return NOERROR;
}

ECode CObjectContainer::Remove(
    /* [in] */ IInterface *pObject)
{
    ObjectNode *pNode;

    SelfLock();

    ForEachDLinkNode(ObjectNode *, pNode, &mHead) {
        if (pNode->mObject == pObject) {
            SimpleContainer::Remove(pNode);
            mCount--;
            mState++;
            pNode->mObject->Release();
            delete pNode;

            SelfUnlock();
            return NOERROR;
        }
    }

    SelfUnlock();

    return S_FALSE;
}

ECode CObjectContainer::GetObjectCount(
    /* [out] */ Int32 *pCount)
{
    if (NULL == pCount) return E_INVALID_ARGUMENT;

    *pCount = mCount;

    return NOERROR;
}

ECode CObjectContainer::GetObjectEnumerator(
    /* [in] */ IObjectEnumerator **ppEnumerator)
{
    if (NULL == ppEnumerator) return E_INVALID_ARGUMENT;

    CObjectEnumerator *pEnum;

    pEnum = new CObjectEnumerator(
                    (ObjectNode *)&mHead,
                    &_m_syncLock,
                    &mState);
    if (NULL == pEnum) return E_OUT_OF_MEMORY;

    *ppEnumerator = (IObjectEnumerator *)pEnum;
    (*ppEnumerator)->AddRef();

    return NOERROR;
}

ECode CObjectContainer::Dispose()
{
    ObjectNode *pNode;

    SelfLock();

    mCount = 0;
    mState++;

    pNode = (ObjectNode *)mHead.Next();
    while (&mHead != (DLinkNode *)pNode) {
        pNode->Detach();
        pNode->mObject->Release();
        delete pNode;
        pNode = (ObjectNode *)mHead.Next();
    }
    mCurrent = NULL;

    SelfUnlock();

    return NOERROR;
}

ECode CObjectContainer::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    SelfLock();
    source->ReadInt32(&mCount);
    for (Int32 i = 0; i < mCount; ++i) {
        IInterface* obj;
        source->ReadInterfacePtrPtr((Handle32*)&obj);
        ObjectNode* node = new ObjectNode(obj);
        SimpleContainer::Add(node);
        mState++;
    }
    SelfUnlock();

    return NOERROR;
}

ECode CObjectContainer::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    ObjectNode *pNode;

    SelfLock();
    dest->WriteInt32(mCount);
    ForEachDLinkNode(ObjectNode *, pNode, &mHead) {
        dest->WriteInterfacePtr(pNode->mObject);
    }

    SelfUnlock();

    return NOERROR;
}
