
#include <gesture/CFileGestureLibrary.h>
#include "ext/frameworkext.h"
#include <Elastos.IO.h>
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const CString CFileGestureLibrary::LOG_TAG = GestureConstants_LOG_TAG;

ECode CFileGestureLibrary::constructor(
    /* [in] */ IFile * path)
{
    mPath = path;
    return NOERROR;
}

ECode CFileGestureLibrary::IsReadOnly(
    /* [out] */ Boolean * readOnly)
{
    return mPath->CanWrite(readOnly);
}

ECode CFileGestureLibrary::Save(
    /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    if (!mStore.HasChanged(ret)) {
        return NOERROR;
    }
    ECode ec = NOERROR;
    IFile* file = mPath;
    String path;
    file->GetAbsolutePath(&path);
    String log = String("Could not save the gesture library in ")+path;
    CString clog= log.string();

    IFile* parentFile = NULL;
    ec = file->GetParentFile(&parentFile);
    if (parentFile == NULL)
    {
        Logger::D(LOG_TAG,clog);
        *ret = FALSE;
        return ec;
    }

    Boolean isExist;
    ec = parentFile->Exists(&isExist);
    if (!isExist) {
        Boolean succeeded;
        ec = parentFile->Mkdirs(&succeeded);
        if (!succeeded) {
            Logger::D(LOG_TAG, clog);
            *ret = FALSE;
            return ec;
        }
    }

//    Boolean result = FALSE;

    //noinspection ResultOfMethodCallIgnored
    Boolean succeeded;
    ec = file->CreateNewFile(&succeeded);
    if (!succeeded) {
        Logger::D(LOG_TAG, clog);
        *ret = FALSE;
         return ec;
    }
    IFileOutputStream *fos = NULL;
    ec = CFileOutputStream::New(file, &fos);
    if (FAILED(ec))
    {
        Logger::D(LOG_TAG, clog);
        *ret = FALSE;
        return ec;
    }
    ec = mStore.SaveEx((IOutputStream *)fos, TRUE);
    if (FAILED(ec))
    {
        Logger::D(LOG_TAG, clog);
        *ret = FALSE;
        return ec;
    }

    *ret = TRUE;
    return NOERROR;

}

ECode CFileGestureLibrary::Load(
    /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    Boolean result = FALSE;
    IFile* file = mPath;
    ECode ec = NOERROR;
    String path;
    file->GetAbsolutePath(&path);
    String log = String("Could not load the gesture library in ")+path;
    CString clog= log.string();
    Boolean isExist;
    Boolean isCanRead;
    ec = file->Exists(&isExist);
    ec = file->CanRead(&isCanRead);

    if (isExist && isCanRead) {
        IFileInputStream * fis = NULL;
        ec = CFileInputStream::New(file, &fis);
        if (FAILED(ec))
        {
            Logger::D(LOG_TAG, clog);
            *ret=result;
            return ec;
        }
        ec = mStore.LoadEx((IFileInputStream*)fis, TRUE);
        if (FAILED(ec))
        {
            Logger::D(LOG_TAG, clog);
            *ret=result;
            return ec;
        }
        result = true;

    }
    Logger::D(LOG_TAG, clog);
    *ret=result;
    return ec;
}

ECode CFileGestureLibrary::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    return GestureLibrary::SetOrientationStyle(style);
}

ECode CFileGestureLibrary::GetOrientationStyle(
    /* [out] */ Int32 * orientationStyle)
{
    return GestureLibrary::GetOrientationStyle(orientationStyle);
}

ECode CFileGestureLibrary::SetSequenceType(
    /* [in] */ Int32 type)
{
    return GestureLibrary::SetSequenceType(type);
}

ECode CFileGestureLibrary::GetSequenceType(
    /* [out] */ Int32 * sequenceType)
{
    return GestureLibrary::GetSequenceType(sequenceType);
}

ECode CFileGestureLibrary::GetGestureEntries(
    /* [out, callee] */ ArrayOf<String> ** gestureEntries)
{
    return GestureLibrary::GetGestureEntries(gestureEntries);
}

ECode CFileGestureLibrary::Recognize(
    /* [in] */ IGesture * gesture,
    /* [out] */ IObjectContainer ** arrayList)
{
    return GestureLibrary::Recognize(gesture, arrayList);
}

ECode CFileGestureLibrary::AddGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture * gesture)
{
    return GestureLibrary::AddGesture(entryName, gesture);
}

ECode CFileGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture * gesture)
{
    return GestureLibrary::RemoveGesture(entryName, gesture);
}

ECode CFileGestureLibrary::RemoveEntry(
    /* [in] */ const String& entryName)
{
    return GestureLibrary::RemoveEntry(entryName);
}

ECode CFileGestureLibrary::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer ** gestures)
{
    return GestureLibrary::GetGestures(entryName, gestures);
}
