
#include <ConsoleTypes.h>
#include <ConsoleApi.h>
#include <linknode.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <Elastos.Core.h>


using namespace Elastos;
using namespace Elastos::Core::Threading;

struct ConsoleExitRoutineNode : public SLinkNode
{
    ConsoleExitRoutineNode(
        /* [in] */ void* target,
        /* [in] */ void* func)
        :mDelegate(EventHandler::Make(target, func))
    {}

    EventHandler mDelegate;
};

class StaticObject : public SingleLinkNode
{
public:
    StaticObject(
        /* [in] */ IInterface* obj)
        :mObject(obj)
    { }

    ~StaticObject()
    {}

    AutoPtr<IInterface> mObject;
};

class ConsoleExitContext
{
public:
    ConsoleExitContext()
    {
        mExitRoutineList.Initialize();
        mObjectList.Initialize();
    }

    ~ConsoleExitContext()
    {
        //BUGBUG: Debug here while main thread is aborted by sub-thread.
        //assert(m_exitRoutineList.IsEmpty() && m_staticObjectList.IsEmpty());
    }

    void DoAtExit()
    {
        // Call process exit routines and delete the routine nodes
        {
            Mutex::Autolock lock(&mExitRoutineListLock);
            void* target;
            void* func;
            SLinkNode* node = mExitRoutineList.Next();
            while (node != &mExitRoutineList) {
                target = ((ConsoleExitRoutineNode*)node)->mDelegate.GetThisPtr();
                func = ((ConsoleExitRoutineNode*)node)->mDelegate.GetFuncPtr();
                node->Detach(&mExitRoutineList);
                delete node;
                mExitRoutineListLock.Unlock();

                ((ConsoleExitRoutine)func)(target);

                mExitRoutineListLock.Lock();
                node = mExitRoutineList.Next();
            }
        }

        // Clean up process static objects
        {
            Mutex::Autolock lock(mObjectListLock);
            SLinkNode* node = mObjectList.Next();
            while (node != &mObjectList) {
                node->Detach(&mObjectList);
                delete (StaticObject*)node;
                node = mObjectList.Next();
            }
        }
    }

    CARAPI AddRoutine(
        /* [in] */ ConsoleExitRoutine routine, PVoid userData)
    {
        assert(routine != NULL);

        ConsoleExitRoutineNode* node = new ConsoleExitRoutineNode(
                userData, *(void **)&routine);
        if (!node) return E_OUT_OF_MEMORY_ERROR;

        Mutex::Autolock lock(&mExitRoutineListLock);
        mExitRoutineList.InsertNext(node);

        return NOERROR;
    }

    CARAPI AddObject(
        /* [in] */ IInterface* object)
    {
        assert(object != NULL);

        StaticObject* node = new StaticObject(object);
        if (!node) return E_OUT_OF_MEMORY_ERROR;

        Mutex::Autolock lock(&mObjectListLock);
        mObjectList.InsertNext(node);

        return NOERROR;
    }

private:
    SLinkNode mExitRoutineList;
    Mutex mExitRoutineListLock;
    SLinkNode mObjectList;
    Mutex mObjectListLock;
};

ConsoleExitContext sConsoleExitContext;
IInterface* sConsoleCallbackContext = NULL;

ELAPI_(void) _Console_Init()
{
    _Impl_CallbackSink_InitCallbackContext(&sConsoleCallbackContext);
    pthread_setspecific(TL_CALLBACK_SLOT, sConsoleCallbackContext);
}

ELAPI _Console_Enter()
{
    return _Impl_CallbackSink_TryToHandleEvents(sConsoleCallbackContext);
}

ELAPI_(void) _Console_Exit()
{
    sConsoleExitContext.DoAtExit();
}

ELAPI _Console_RequestToFinish()
{
    return _Impl_CallbackSink_RequestToFinish(sConsoleCallbackContext, CallbackContextFinish_ASAP);
}

ELAPI _Console_ReleaseAtExit(
    /* [in] */ IInterface* object)
{
    return sConsoleExitContext.AddObject(object);
}