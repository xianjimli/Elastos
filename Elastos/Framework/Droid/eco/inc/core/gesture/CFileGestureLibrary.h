
#ifndef __CFILEGESTURELIBRARY_H__
#define __CFILEGESTURELIBRARY_H__

#include "_CFileGestureLibrary.h"
//#include "_Elastos.Framework.Core.h"
#include <gesture/GestureLibrary.h>
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core;

CarClass(CFileGestureLibrary)
, public GestureLibrary
{
public:
    CARAPI constructor(
        /* [in] */ IFile * path);

    CARAPI Save(
        /* [out] */ Boolean * ret);

    CARAPI Load(
        /* [out] */ Boolean * ret);

    CARAPI IsReadOnly(
        /* [out] */ Boolean * readOnly);

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
    IFile* mPath;

    static const CString LOG_TAG; // = GestureConstants_LOG_TAG;

};

#endif // __CFILEGESTURELIBRARY_H__
