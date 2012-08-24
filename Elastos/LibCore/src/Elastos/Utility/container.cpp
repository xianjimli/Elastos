
#include "container.h"

ELAPI SimpleContainerCurrent(SimpleContainer *container, DLinkNode **node)
{
    assert(NULL != node);

    if (NULL == container->mCurrent || &(container->mHead) == container->mCurrent) {
        return E_INVALID_OPERATION;
    }
    *node = container->mCurrent;
    return NOERROR;
}

ELAPI SimpleContainerMoveNext(SimpleContainer *container)
{
    if (NULL == container->mCurrent) {
        return E_INVALID_OPERATION;
    }
    if (container->mCurrent == container->mHead.Prev()) {
        container->mCurrent = NULL;
        return S_FALSE;
    }
    container->mCurrent = container->mCurrent->Next();
    return NOERROR;
}

ELAPI SimpleContainerReset(SimpleContainer *container)
{
    container->mCurrent = &(container->mHead);
    return NOERROR;
}

ELAPI SimpleContainerAdd(SimpleContainer *container, DLinkNode *node)
{
    container->mHead.InsertPrev(node);
    return NOERROR;
}

ELAPI SimpleContainerRemove(SimpleContainer *container, DLinkNode *node)
{
    if (container->mCurrent == node) {
        container->mCurrent = container->mCurrent->Prev();
    }
    node->Detach();
    return NOERROR;
}


ELAPI ObjectContainerDispose(ObjectContainer *container)
{
    ObjectNode *node;

    node = (ObjectNode *)container->mHead.Next();
    while (&(container->mHead) != (DLinkNode *)node) {
        node->Detach();
        delete node;
        node = (ObjectNode *)container->mHead.Next();
    }
    container->mCurrent = NULL;
    return NOERROR;
}

ELAPI ObjectContainerCurrent(ObjectContainer *container, PInterface *object)
{
    _ELASTOS ECode ec;
    ObjectNode *node;

    ec = SimpleContainerCurrent((SimpleContainer*)container, (DLinkNode **)&node);
    if (NOERROR == ec) *object = node->mObject;
    return ec;
}

ELAPI ObjectContainerAdd(ObjectContainer *container, PInterface object)
{
    ObjectNode *node;

    node = new ObjectNode(object);
    if (NULL == node) return E_OUT_OF_MEMORY;

    return SimpleContainerAdd((SimpleContainer*)container, node);
}

ELAPI ObjectContainerRemove(ObjectContainer *container, PInterface object)
{
    ObjectNode *node;

    ForEachDLinkNode(ObjectNode *, node, &(container->mHead)) {
        if (node->mObject == object) {
            SimpleContainerRemove((SimpleContainer*)container, node);
            delete node;
            return NOERROR;
        }
    }
    return S_FALSE;
}
