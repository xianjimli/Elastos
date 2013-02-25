
#ifndef __CGESTURELIBRARIES_H__
#define __CGESTURELIBRARIES_H__

#include <gesture/CGestureStore.h>
#include "ext/frameworkdef.h"
#include <elastos.h>
#include <gesture/Learner.h>

//public abstract class GestureLibrary
class GestureLibrary
{
public:
    virtual CARAPI Save(
        /* [out] */ Boolean * ret) = 0;

    virtual CARAPI Load(
        /* [out] */ Boolean * ret) = 0;

    CARAPI IsReadOnly(
        /* [out] */ Boolean * ret);

    /** @hide */
    CARAPI GetLearner(
        /* [out] */ ILearner ** learner);

    CARAPI SetOrientationStyle(
        /* [in] */ Int32 style);

    CARAPI GetOrientationStyle(
        /* [out] */ Int32 * orientationStyle);

    CARAPI SetSequenceType(
        /* [in] */ Int32 type);

    CARAPI GetSequenceType(
        /* [out] */ Int32 * sequenceType);;

    //public Set<String> getGestureEntries()
    //IObjectContainer<String> *
    CARAPI GetGestureEntries(
        /* [out, callee] */ ArrayOf<String> **ret);

    //public ArrayList<Prediction> recognize(Gesture gesture)
    CARAPI Recognize(
        /* [in] */ IGesture* gesture,
        /* [out] */ IObjectContainer **ret);

    CARAPI AddGesture(
        /* [in] */ const String& entryName,
        /* [in] */ IGesture* gesture);

    CARAPI RemoveGesture(
        /* [in] */ const String& entryName,
        /* [in] */ IGesture* gesture);

    CARAPI RemoveEntry(
        /* [in] */ const String& entryName);

    //IObjectContainer<IGesture *> *
    CARAPI GetGestures(
        /* [in] */ const String& entryName,
        /* [out] */ IObjectContainer ** ret);

protected:
    CGestureStore mStore;

};

#endif //__CGESTURELIBRARIES_H__
