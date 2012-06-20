
#ifndef __CNOTUSED_H__
#define __CNOTUSED_H__

#include "_CNotUsed.h"

CarClass(CNotUsed)
{
public:
    CARAPI Array(
        /* [out] */ IInterface** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI Capacity(
        /* [out] */ Int32* cap);

    CARAPI Clear();

    CARAPI Flip();

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI HasRemaining(
        /* [out] */ Boolean* hasRemaining);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    CARAPI Limit(
        /* [out] */ Int32* limit);

    CARAPI LimitEx(
        /* [in] */ Int32 newLimit);

    CARAPI Mark();

    CARAPI Position(
        /* [out] */ Int32* pos);

    CARAPI PositionEx(
        /* [in] */ Int32 newPosition);

    CARAPI Remaining(
        /* [out] */ Int32* remaining);

    CARAPI Reset();

    CARAPI Rewind();

private:
    // TODO: Add your private member variables here.
};

#endif // __CNOTUSED_H__

