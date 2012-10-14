
#include "view/VelocityTracker.h"
#include <elastos/Math.h>
#include <StringBuffer.h>
#include <Logger.h>
#include <stdio.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const char* VelocityTracker::TAG = "VelocityTracker";
const Boolean VelocityTracker::DEBUG;
const Boolean VelocityTracker::localLOGV;
const Int32 VelocityTracker::NUM_PAST;
const Int32 VelocityTracker::MAX_AGE_MILLISECONDS;
const Int32 VelocityTracker::POINTER_POOL_CAPACITY;
const AutoPtr<IPool> VelocityTracker::sPool = Pools::AcquireSynchronizedPool(
                Pools::AcquireFinitePool(new VelocityTrackerPoolableManager(), 2));

Mutex VelocityTracker::sPoolLock;
AutoPtr<VelocityTracker::Pointer> VelocityTracker::sRecycledPointerListHead;
Int32 VelocityTracker::sRecycledPointerCount = 0;

PInterface VelocityTracker::VelocityTrackerPoolableManager::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPoolableManager) {
        return (IPoolableManager*)this;
    }

    return NULL;
}

UInt32 VelocityTracker::VelocityTrackerPoolableManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 VelocityTracker::VelocityTrackerPoolableManager::Release()
{
    return ElRefBase::Release();
}

ECode VelocityTracker::VelocityTrackerPoolableManager::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IPoolableManager*)this) {
        *pIID = EIID_IPoolableManager;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode VelocityTracker::VelocityTrackerPoolableManager::NewInstance(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    *element = new VelocityTracker();
    if (*element == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    (*element)->AddRef();

    return NOERROR;
}

ECode VelocityTracker::VelocityTrackerPoolableManager::OnAcquired(
    /* [in] */ IPoolable* element)
{
    return NOERROR;
}

ECode VelocityTracker::VelocityTrackerPoolableManager::OnReleased(
    /* [in] */ IPoolable* element)
{
    ((VelocityTracker*)element)->Clear();

    return NOERROR;
}

VelocityTracker::VelocityTracker()
{
    Clear();
}

PInterface VelocityTracker::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPoolable) {
        return (IPoolable*)this;
    }

    return NULL;
}

UInt32 VelocityTracker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 VelocityTracker::Release()
{
    return ElRefBase::Release();
}

ECode VelocityTracker::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IPoolable*)this) {
        *pIID = EIID_IPoolable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

/**
 * Retrieve a new VelocityTracker object to watch the velocity of a
 * motion.  Be sure to call {@link #recycle} when done.  You should
 * generally only maintain an active object while tracking a movement,
 * so that the VelocityTracker can be re-used elsewhere.
 *
 * @return Returns a new VelocityTracker.
 */
AutoPtr<VelocityTracker> VelocityTracker::Obtain()
{
    AutoPtr<IPoolable> element;
    sPool->Acquire((IPoolable**)&element);

    return (VelocityTracker*)element.Get();
}

/**
 * Return a VelocityTracker object back to be re-used by others.  You must
 * not touch the object after calling this function.
 */
void VelocityTracker::Recycle()
{
    sPool->ReleaseElement((IPoolable*)this);
}

/**
 * @hide
 */
ECode VelocityTracker::SetNextPoolable(
    /* [in] */ IPoolable* element)
{
    mNext = element;

    return NOERROR;
}

/**
 * @hide
 */
ECode VelocityTracker::GetNextPoolable(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    *element = mNext;
    if (*element) {
        (*element)->AddRef();
    }

    return NOERROR;
}

/**
 * Reset the velocity tracker back to its initial state.
 */
void VelocityTracker::Clear()
{
    ReleasePointerList(mPointerListHead);

    mPointerListHead = NULL;
    mLastTouchIndex = 0;
}

/**
 * Add a user's movement to the tracker.  You should call this for the
 * initial {@link MotionEvent#ACTION_DOWN}, the following
 * {@link MotionEvent#ACTION_MOVE} events that you receive, and the
 * final {@link MotionEvent#ACTION_UP}.  You can, however, call this
 * for whichever events you desire.
 *
 * @param ev The MotionEvent you received and would like to track.
 */
void VelocityTracker::AddMovement(
    /* [in] */ IMotionEvent* ev)
{
    Int32 historySize;
    ev->GetHistorySize(&historySize);
    Int32 pointerCount;
    ev->GetPointerCount(&pointerCount);
    Int32 lastTouchIndex = mLastTouchIndex;
    Int32 nextTouchIndex = (lastTouchIndex + 1) % NUM_PAST;
    Int32 finalTouchIndex = (nextTouchIndex + historySize) % NUM_PAST;
    Int32 generation = mGeneration++;

    mLastTouchIndex = finalTouchIndex;

    // Update pointer data.
    AutoPtr<Pointer> previousPointer;
    for (Int32 i = 0; i < pointerCount; i++) {
        Int32 pointerId;
        ev->GetPointerId(i, &pointerId);
        // Find the pointer data for this pointer id.
        // This loop is optimized for the common case where pointer ids in the event
        // are in sorted order.  However, we check for this case explicitly and
        // perform a full linear scan from the start if needed.
        AutoPtr<Pointer> nextPointer;
        if (previousPointer == NULL || pointerId < previousPointer->mId) {
            previousPointer = NULL;
            nextPointer = mPointerListHead;
        }
        else {
            nextPointer = previousPointer->mNext;
        }

        AutoPtr<Pointer> pointer;
        for (;;) {
            if (nextPointer != NULL) {
                Int32 nextPointerId = nextPointer->mId;
                if (nextPointerId == pointerId) {
                    pointer = nextPointer;
                    break;
                }
                if (nextPointerId < pointerId) {
                    nextPointer = nextPointer->mNext;
                    continue;
                }
            }

            // Pointer went down.  Add it to the list.
            // Write a sentinel at the end of the pastTime trace so we will be able to
            // tell when the trace started.
            pointer = ObtainPointer();
            pointer->mId = pointerId;
            pointer->mPastTime[lastTouchIndex] = Math::INT64_MAX_VALUE;
            pointer->mNext = nextPointer;
            if (previousPointer == NULL) {
                mPointerListHead = pointer;
            }
            else {
                previousPointer->mNext = pointer;
            }
            break;
        }

        pointer->mGeneration = generation;
        previousPointer = pointer;

        Float* pastX = pointer->mPastX;
        Float* pastY = pointer->mPastY;
        Int64* pastTime = pointer->mPastTime;

        for (Int32 j = 0; j < historySize; j++) {
            Int32 touchIndex = (nextTouchIndex + j) % NUM_PAST;
            ev->GetHistoricalXEx(i, j, pastX + touchIndex);
            ev->GetHistoricalYEx(i, j, pastY + touchIndex);
            ev->GetHistoricalEventTime(j, pastTime + touchIndex);
        }
        ev->GetXEx(i, pastX + finalTouchIndex);
        ev->GetYEx(i, pastY + finalTouchIndex);
        ev->GetEventTime(pastTime + finalTouchIndex);
    }

    // Find removed pointers.
    previousPointer = NULL;
    for (AutoPtr<Pointer> pointer = mPointerListHead; pointer != NULL; ) {
        AutoPtr<Pointer> nextPointer = pointer->mNext;
        if (pointer->mGeneration != generation) {
            // Pointer went up.  Remove it from the list.
            if (previousPointer == NULL) {
                mPointerListHead = nextPointer;
            }
            else {
                previousPointer->mNext = nextPointer;
            }
            ReleasePointer(pointer);
        }
        else {
            previousPointer = pointer;
        }
        pointer = nextPointer;
    }
}

/**
 * Equivalent to invoking {@link #computeCurrentVelocity(Int32, Float)} with a maximum
 * velocity of Float.MAX_VALUE.
 *
 * @see #computeCurrentVelocity(Int32, Float)
 */
void VelocityTracker::ComputeCurrentVelocity(
    /* [in] */ Int32 units)
{
    ComputeCurrentVelocity(units, Math::FLOAT_MAX_VALUE);
}

/**
 * Compute the current velocity based on the points that have been
 * collected.  Only call this when you actually want to retrieve velocity
 * information, as it is relatively expensive.  You can then retrieve
 * the velocity with {@link #getXVelocity()} and
 * {@link #getYVelocity()}.
 *
 * @param units The units you would like the velocity in.  A value of 1
 * provides pixels per millisecond, 1000 provides pixels per second, etc.
 * @param maxVelocity The maximum velocity that can be computed by this method.
 * This value must be declared in the same unit as the units parameter. This value
 * must be positive.
 */
void VelocityTracker::ComputeCurrentVelocity(
    /* [in] */ Int32 units,
    /* [in] */ Float maxVelocity)
{
    Int32 lastTouchIndex = mLastTouchIndex;

    for (AutoPtr<Pointer> pointer = mPointerListHead; pointer != NULL;
        pointer = pointer->mNext) {
        Int64* pastTime = pointer->mPastTime;

        // Search backwards in time for oldest acceptable time.
        // Stop at the beginning of the trace as indicated by the sentinel time Long.MIN_VALUE.
        Int32 oldestTouchIndex = lastTouchIndex;
        Int32 numTouches = 1;
        Int64 minTime = pastTime[lastTouchIndex] - MAX_AGE_MILLISECONDS;
        while (numTouches < NUM_PAST) {
            Int32 nextOldestTouchIndex = (oldestTouchIndex + NUM_PAST - 1) % NUM_PAST;
            Int64 nextOldestTime = pastTime[nextOldestTouchIndex];
            if (nextOldestTime < minTime) { // also handles end of trace sentinel
                break;
            }
            oldestTouchIndex = nextOldestTouchIndex;
            numTouches += 1;
        }

        // If we have a lot of samples, skip the last received sample since it is
        // probably pretty noisy compared to the sum of all of the traces already acquired.
        if (numTouches > 3) {
            numTouches -= 1;
        }

        // Kind-of stupid.
        Float* pastX = pointer->mPastX;
        Float* pastY = pointer->mPastY;

        Float oldestX = pastX[oldestTouchIndex];
        Float oldestY = pastY[oldestTouchIndex];
        Int64 oldestTime = pastTime[oldestTouchIndex];

        Float accumX = 0;
        Float accumY = 0;

        for (Int32 i = 1; i < numTouches; i++) {
            Int32 touchIndex = (oldestTouchIndex + i) % NUM_PAST;
            Int32 duration = (Int32)(pastTime[touchIndex] - oldestTime);

            if (duration == 0) {
                continue;
            }

            Float delta = pastX[touchIndex] - oldestX;
            Float velocity = (delta / duration) * units; // pixels/frame.
            accumX = (accumX == 0) ? velocity : (accumX + velocity) * .5f;

            delta = pastY[touchIndex] - oldestY;
            velocity = (delta / duration) * units; // pixels/frame.
            accumY = (accumY == 0) ? velocity : (accumY + velocity) * .5f;
        }

        if (accumX < -maxVelocity) {
            accumX = - maxVelocity;
        }
        else if (accumX > maxVelocity) {
            accumX = maxVelocity;
        }

        if (accumY < -maxVelocity) {
            accumY = - maxVelocity;
        }
        else if (accumY > maxVelocity) {
            accumY = maxVelocity;
        }

        pointer->mXVelocity = accumX;
        pointer->mYVelocity = accumY;

        if (localLOGV) {
            Logger::V(TAG, StringBuffer("Pointer ") + pointer->mId
                + ": Y velocity=" + Int32(accumX) +" X velocity=" + Int32(accumY)
                + " N=" + numTouches);
        }
    }
}

/**
 * Retrieve the last computed X velocity.  You must first call
 * {@link #computeCurrentVelocity(Int32)} before calling this function.
 *
 * @return The previously computed X velocity.
 */
Float VelocityTracker::GetXVelocity()
{
    AutoPtr<Pointer> pointer = GetPointer(0);
    return pointer != NULL ? pointer->mXVelocity : 0;
}

/**
 * Retrieve the last computed Y velocity.  You must first call
 * {@link #computeCurrentVelocity(Int32)} before calling this function.
 *
 * @return The previously computed Y velocity.
 */
Float VelocityTracker::GetYVelocity()
{
    AutoPtr<Pointer> pointer = GetPointer(0);
    return pointer != NULL ? pointer->mYVelocity : 0;
}

/**
 * Retrieve the last computed X velocity.  You must first call
 * {@link #computeCurrentVelocity(Int32)} before calling this function.
 *
 * @param id Which pointer's velocity to return.
 * @return The previously computed X velocity.
 */
Float VelocityTracker::GetXVelocity(
    /* [in] */ Int32 id)
{
    AutoPtr<Pointer> pointer = GetPointer(id);
    return pointer != NULL ? pointer->mXVelocity : 0;
}

/**
 * Retrieve the last computed Y velocity.  You must first call
 * {@link #computeCurrentVelocity(Int32)} before calling this function.
 *
 * @param id Which pointer's velocity to return.
 * @return The previously computed Y velocity.
 */
Float VelocityTracker::GetYVelocity(
    /* [in] */ Int32 id)
{
    AutoPtr<Pointer> pointer = GetPointer(id);
    return pointer != NULL ? pointer->mYVelocity : 0;
}

AutoPtr<VelocityTracker::Pointer> VelocityTracker::GetPointer(
    /* [in] */ Int32 id)
{
    for (AutoPtr<Pointer> pointer = mPointerListHead;
        pointer != NULL; pointer = pointer->mNext) {
        if (pointer->mId == id) {
            return pointer;
        }
    }
    return NULL;
}

AutoPtr<VelocityTracker::Pointer> VelocityTracker::ObtainPointer()
{
    {
        Mutex::Autolock lock(sPoolLock);

        if (sRecycledPointerCount != 0) {
            AutoPtr<Pointer> element = sRecycledPointerListHead;
            sRecycledPointerCount -= 1;
            sRecycledPointerListHead = element->mNext;
            element->mNext = NULL;
            return element;
        }
    }

    return new Pointer();
}

void VelocityTracker::VelocityTracker::ReleasePointer(
    /* [in] */ Pointer* pointer)
{
    Mutex::Autolock lock(sPoolLock);

    if (sRecycledPointerCount < POINTER_POOL_CAPACITY) {
        pointer->mNext = sRecycledPointerListHead;
        sRecycledPointerCount += 1;
        sRecycledPointerListHead = pointer;
    }
}

void VelocityTracker::ReleasePointerList(
    /* [in] */ Pointer* pointer)
{
    if (pointer != NULL) {
        Mutex::Autolock lock(sPoolLock);
        Int32 count = sRecycledPointerCount;
        if (count >= POINTER_POOL_CAPACITY) {
            return;
        }

        AutoPtr<Pointer> tail = pointer;
        for (;;) {
            count += 1;
            if (count >= POINTER_POOL_CAPACITY) {
                break;
            }

            AutoPtr<Pointer> next = tail->mNext;
            if (next == NULL) {
                break;
            }
            tail = next;
        }

        tail->mNext = sRecycledPointerListHead;
        sRecycledPointerCount = count;
        sRecycledPointerListHead = pointer;
    }
}
