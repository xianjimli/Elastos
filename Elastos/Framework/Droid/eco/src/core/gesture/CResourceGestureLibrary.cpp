
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
    mContext = context;
    mResourceId = resourceId;
    return NOERROR;
}

ECode CResourceGestureLibrary::IsReadOnly(
    /* [out] */ Boolean * readOnly)
{
    *readOnly = TRUE;
    return NOERROR;
}

ECode CResourceGestureLibrary::Save(
    /* [out] */ Boolean * ret)
{
    *ret=FALSE;
    return NOERROR;
}

ECode CResourceGestureLibrary::Load(
    /* [out] */ Boolean * ret)
{
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
    return GestureLibrary::GetSequenceType(sequenceType);
}

ECode CResourceGestureLibrary::GetGestureEntries(
    /* [out, callee] */ ArrayOf<String> ** gestureEntries)
{
    return GestureLibrary::GetGestureEntries(gestureEntries);
}

ECode CResourceGestureLibrary::Recognize(
    /* [in] */ IGesture * gesture,
    /* [out] */ IObjectContainer ** arrayList)
{
    return GestureLibrary::Recognize(gesture, arrayList);
}

ECode CResourceGestureLibrary::AddGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture * gesture)
{
    return GestureLibrary::AddGesture(entryName, gesture);
}

ECode CResourceGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture * gesture)
{
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
    return GestureLibrary::GetGestures(entryName, gestures);
}
