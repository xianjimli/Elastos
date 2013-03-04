
#include <gesture/CResourceGestureLibrary.h>
#include <Elastos.IO.h>
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const CString CResourceGestureLibrary::LOG_TAG = GestureConstants_LOG_TAG;

ECode CResourceGestureLibrary::constructor(
    /* [in] */ IContext * context,
    /* [in] */ Int32 resourceId)
{
    VALIDATE_NOT_NULL(context);
    mContext = context;
    //AuotoPtr  = opertor will call AddRef in the rhs side object,
    //so it need to call Release.
    context->Release();
    mResourceId = resourceId;
    return NOERROR;
}

ECode CResourceGestureLibrary::IsReadOnly(
    /* [out] */ Boolean * readOnly)
{
    VALIDATE_NOT_NULL(readOnly);
    *readOnly = TRUE;
    return NOERROR;
}

ECode CResourceGestureLibrary::Save(
    /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=FALSE;
    return NOERROR;
}

ECode CResourceGestureLibrary::Load(
    /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    Boolean result = FALSE;
    ECode ec = NOERROR;

    IResources* res = NULL;
    ec = mContext->GetResources(&res);
    if (FAILED(ec)) {
        *ret = result;
        return ec;
    }
    IInputStream* in = NULL;
    ec = res->OpenRawResource(mResourceId, &in);
    if (FAILED(ec)) {
        *ret = result;
        res->Release();
        return ec;
    }
    ec = mStore.LoadEx(in, TRUE);
    if (FAILED(ec)) {
        String resourceName;
        res->GetResourceName(mResourceId, &resourceName);
        String log= String("Could not load the gesture library from raw resource ")+
                    resourceName;
        Logger::D(LOG_TAG, CString(log.string()) );

    }
    else {
        result = true;
    }

    *ret = result;
    res->Release();
    in->Release();
    return ec;
}

ECode CResourceGestureLibrary::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    return GestureLibrary::SetOrientationStyle(style);
}

ECode CResourceGestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 * orientationStyle)
{
    VALIDATE_NOT_NULL(orientationStyle);
    return GestureLibrary::GetOrientationStyle(orientationStyle);
}

ECode CResourceGestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    return GestureLibrary::SetSequenceType(type);
}

ECode CResourceGestureLibrary::GetSequenceType(
    /* [out] */ Int32 * sequenceType)
{
    VALIDATE_NOT_NULL(sequenceType);
    return GestureLibrary::GetSequenceType(sequenceType);
}

ECode CResourceGestureLibrary::GetGestureEntries(
    /* [out, callee] */ ArrayOf<String> ** gestureEntries)
{
    VALIDATE_NOT_NULL(gestureEntries);
    return GestureLibrary::GetGestureEntries(gestureEntries);
}

ECode CResourceGestureLibrary::Recognize(
    /* [in] */ IGesture * gesture,
    /* [out] */ IObjectContainer ** arrayList)
{
    VALIDATE_NOT_NULL(gesture);
    VALIDATE_NOT_NULL(arrayList);
    return GestureLibrary::Recognize(gesture, arrayList);
}

ECode CResourceGestureLibrary::AddGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture * gesture)
{
    VALIDATE_NOT_NULL(gesture);
    return GestureLibrary::AddGesture(entryName, gesture);
}

ECode CResourceGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture * gesture)
{
    VALIDATE_NOT_NULL(gesture);
    return GestureLibrary::RemoveGesture(entryName, gesture);
}

ECode CResourceGestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return GestureLibrary::RemoveEntry(entryName);
}

ECode CResourceGestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer ** gestures)
{
    VALIDATE_NOT_NULL(gestures);
    return GestureLibrary::GetGestures(entryName, gestures);
}
