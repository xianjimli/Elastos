
#ifndef __VELOCITYTRACKER_H__
#define __VELOCITYTRACKER_H__

#include "utils/Pools.h"
#include "server/Config.h"

/**
 * Helper for tracking the velocity of touch events, for implementing
 * flinging and other such gestures.  Use {@link #obtain} to retrieve a
 * new instance of the class when you are going to begin tracking, put
 * the motion events you receive into it with {@link #addMovement(MotionEvent)},
 * and when you want to determine the velocity call
 * {@link #computeCurrentVelocity(Int32)} and then {@link #getXVelocity()}
 * and {@link #getXVelocity()}.
 */
class VelocityTracker : public ElRefBase, public IPoolable
{
private:
    static const char* TAG;
    static const Boolean DEBUG = FALSE;
    static const Boolean localLOGV = DEBUG || Config::LOGV;

    static const Int32 NUM_PAST = 10;
    static const Int32 MAX_AGE_MILLISECONDS = 200;
    static const Int32 POINTER_POOL_CAPACITY = 20;

    static const AutoPtr<IPool> sPool;
    static Mutex sPoolLock;

private:
    class VelocityTrackerPoolableManager : public ElRefBase, public IPoolableManager
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI NewInstance(
            /* [out] */ IPoolable** element);

        CARAPI OnAcquired(
            /* [in] */ IPoolable* element);

        CARAPI OnReleased(
            /* [in] */ IPoolable* element);
    };

    class Pointer : public ElRefBase
    {
    public:
        AutoPtr<Pointer> mNext;

        Int32 mId;
        Float mXVelocity;
        Float mYVelocity;

        Float mPastX[NUM_PAST];
        Float mPastY[NUM_PAST];
        Int64 mPastTime[NUM_PAST]; // uses Long.MIN_VALUE as a sentinel

        Int32 mGeneration;
    };

public:
    static CARAPI_(AutoPtr<VelocityTracker>) Obtain();

protected:
    static CARAPI_(AutoPtr<Pointer>) ObtainPointer();

    static CARAPI_(void) ReleasePointer(
        /* [in] */ Pointer* pointer);

    static CARAPI_(void) ReleasePointerList(
        /* [in] */ Pointer* pointer);

private:
    VelocityTracker();

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI_(void) Recycle();

    CARAPI SetNextPoolable(
        /* [in] */ IPoolable* element);

    CARAPI GetNextPoolable(
        /* [out] */ IPoolable** element);

    CARAPI_(void) Clear();

    CARAPI_(void) AddMovement(
        /* [in] */ IMotionEvent* ev);

    CARAPI_(void) ComputeCurrentVelocity(
        /* [in] */ Int32 units);

    CARAPI_(void) ComputeCurrentVelocity(
        /* [in] */ Int32 units,
        /* [in] */ Float maxVelocity);

    CARAPI_(Float) GetXVelocity();

    CARAPI_(Float) GetYVelocity();

    CARAPI_(Float) GetXVelocity(
        /* [in] */ Int32 id);

    CARAPI_(Float) GetYVelocity(
        /* [in] */ Int32 id);

private:
    CARAPI_(AutoPtr<Pointer>) GetPointer(
        /* [in] */ Int32 id);

private:
    static AutoPtr<Pointer> sRecycledPointerListHead;
    static Int32 sRecycledPointerCount;

    AutoPtr<Pointer> mPointerListHead; // sorted by id in increasing order
    Int32 mLastTouchIndex;
    Int32 mGeneration;

    AutoPtr<IPoolable> mNext;
};
#endif //__VELOCITYTRACKER_H__
