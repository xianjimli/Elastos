
#include <gesture/GestureLibrary.h>

ECode GestureLibrary::IsReadOnly(
    /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = FALSE;
    return NOERROR;
}

ECode GestureLibrary::GetLearner(
    /* [out] */ ILearner ** learner)
{
    VALIDATE_NOT_NULL(learner);
    return mStore.GetLearner(learner);
}

ECode GestureLibrary::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    return mStore.SetOrientationStyle(style);
}

ECode GestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 * orientationStyle)
{
    VALIDATE_NOT_NULL(orientationStyle);
    return mStore.GetOrientationStyle(orientationStyle);
}

ECode GestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    mStore.SetSequenceType(type);
    return NOERROR;
}

ECode GestureLibrary::GetSequenceType(
    /* [out] */ Int32 * sequenceType)
{
    VALIDATE_NOT_NULL(sequenceType);
    return mStore.GetSequenceType(sequenceType);
}

//IObjectContainer<String>*
ECode GestureLibrary::GetGestureEntries(
    /* [out, callee] */ ArrayOf<String> **ret)
{
    VALIDATE_NOT_NULL(ret);
    return mStore.GetGestureEntries(ret);
}

//IObjectContainer<IPrediction *>*
ECode GestureLibrary::Recognize(
    /* [in] */ IGesture* gesture,
    /* [out] */ IObjectContainer ** ret)
{
    VALIDATE_NOT_NULL(gesture);
    VALIDATE_NOT_NULL(ret);
    return mStore.Recognize(gesture, ret);
}

ECode GestureLibrary::AddGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture* gesture)
{
    VALIDATE_NOT_NULL(gesture);
    return mStore.AddGesture(entryName, gesture);
}

ECode GestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture* gesture)
{
    VALIDATE_NOT_NULL(gesture);
    return mStore.RemoveGesture(entryName, gesture);
}

ECode GestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return mStore.RemoveEntry(entryName);
}

//IObejctContainer<IGesture*>*
ECode GestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer ** ret)
{
    VALIDATE_NOT_NULL(ret);
    return mStore.GetGestures(entryName, ret);
}

