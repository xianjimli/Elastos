
#ifndef __CRESOURCEGESTURELIBRARY_H__
#define __CRESOURCEGESTURELIBRARY_H__

#include "_CResourceGestureLibrary.h"
#include <gesture/GestureLibrary.h>
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
using namespace Elastos;
using namespace Elastos::Core;

CarClass(CResourceGestureLibrary)
, public GestureLibrary
{
public:
    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 resourceId);

    CARAPI IsReadOnly(
        /* [out] */ Boolean * readOnly);

    CARAPI Save(
        /* [out] */ Boolean * ret);

    CARAPI Load(
        /* [out] */ Boolean * ret);

    CARAPI SetOrientationStyle(
        /* [in] */ Int32 style);

    CARAPI GetOrientationStyle(
        /* [out] */ Int32 * orientationStyle);

    CARAPI SetSequenceType(
        /* [in] */ Int32 type);

    CARAPI GetSequenceType(
        /* [out] */ Int32 * sequenceType);

    CARAPI GetGestureEntries(
        /* [out, callee] */ ArrayOf<String> ** gestureEntries);

    CARAPI Recognize(
        /* [in] */ IGesture * gesture,
        /* [out] */ IObjectContainer ** arrayList);

    CARAPI AddGesture(
        /* [in] */ const String& entryName,
        /* [in] */ IGesture * gesture);

    CARAPI RemoveGesture(
        /* [in] */ const String& entryName,
        /* [in] */ IGesture * gesture);

    CARAPI RemoveEntry(
        /* [in] */ const String& entryName);

    CARAPI GetGestures(
        /* [in] */ const String& entryName,
        /* [out] */ IObjectContainer ** gestures);

private:
    static const CString LOG_TAG;

    AutoPtr<IContext> mContext;

    Int32 mResourceId;
};

#endif // __CRESOURCEGESTURELIBRARY_H__
