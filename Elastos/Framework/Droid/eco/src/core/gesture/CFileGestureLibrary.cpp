
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
    VALIDATE_NOT_NULL(path);
    mPath = path;
    return NOERROR;
}

ECode CFileGestureLibrary::IsReadOnly(
    /* [out] */ Boolean * readOnly)
{
    VALIDATE_NOT_NULL(readOnly);
    return mPath->CanWrite(readOnly);
}

ECode CFileGestureLibrary::Save(
    /* [out] */ Boolean * ret)
{
    VALIDATE_NOT_NULL(ret);
    mStore.HasChanged(ret);
    if (!(*ret)) {
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


    Boolean isExist = FALSE;
    ec = parentFile->Exists(&isExist);
    if (!isExist) {
        Boolean succeeded;
        ec = parentFile->Mkdirs(&succeeded);
        if (!succeeded) {
            Logger::D(LOG_TAG, clog);
            *ret = FALSE;
            parentFile->Release();
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
        parentFile->Release();
         return ec;
    }
    IFileOutputStream *fos = NULL;
    ec = CFileOutputStream::New(file, &fos);
    if (FAILED(ec))
    {
        Logger::D(LOG_TAG, clog);
        *ret = FALSE;
        parentFile->Release();
        return ec;
    }
    ec = mStore.SaveEx((IOutputStream *)fos, TRUE);
    if (FAILED(ec))
    {
        Logger::D(LOG_TAG, clog);
        *ret = FALSE;
        parentFile->Release();
        fos->Release();
        return ec;
    }

    *ret = TRUE;
    parentFile->Release();
    fos->Release();
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
    Boolean isExist = FALSE;
    Boolean isCanRead = FALSE;
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
            fis->Release();
            return ec;
        }
        result = TRUE;
        fis->Release();
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
    VALIDATE_NOT_NULL(orientationStyle);
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
    VALIDATE_NOT_NULL(sequenceType);
    return GestureLibrary::GetSequenceType(sequenceType);
}

ECode CFileGestureLibrary::GetGestureEntries(
    /* [out, callee] */ ArrayOf<String> ** gestureEntries)
{
    VALIDATE_NOT_NULL(gestureEntries);
    return GestureLibrary::GetGestureEntries(gestureEntries);
}

ECode CFileGestureLibrary::Recognize(
    /* [in] */ IGesture * gesture,
    /* [out] */ IObjectContainer ** arrayList)
{
    VALIDATE_NOT_NULL(arrayList);
    return GestureLibrary::Recognize(gesture, arrayList);
}

ECode CFileGestureLibrary::AddGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture * gesture)
{
    VALIDATE_NOT_NULL(gesture);
    return GestureLibrary::AddGesture(entryName, gesture);
}

ECode CFileGestureLibrary::RemoveGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture * gesture)
{
    VALIDATE_NOT_NULL(gesture);
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
    VALIDATE_NOT_NULL(gestures);
    return GestureLibrary::GetGestures(entryName, gestures);
}
