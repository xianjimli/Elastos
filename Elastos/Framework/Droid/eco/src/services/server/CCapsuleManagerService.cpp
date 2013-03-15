
#include "ext/frameworkext.h"
#include "server/CCapsuleManagerService.h"
#include "utils/Config.h"
#include "utils/ListUtils.h"
#include "utils/Xml.h"
#include "utils/XmlUtils.h"
#include "app/ActivityManagerNative.h"
#include "net/Uri.h"
#include "content/cm/CapsuleManager.h"
#include "content/CapsuleHelper.h"
#include "content/NativeLibraryHelper.h"
#include "os/Environment.h"
#include "os/Process.h"
#include "os/SystemClock.h"
#include "os/SystemProperties.h"
#include "os/FileUtils.h"
#include "os/ServiceManager.h"
#include "os/Build.h"
#include <elastos/AutoFree.h>
#include <Slogger.h>
#include <Logger.h>
#include <StringBuffer.h>
#include <elastos/System.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

#define UNUSED(x) (void)x

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::DefaultContainerConnection

PInterface CCapsuleManagerService::DefaultContainerConnection::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IServiceConnection) {
        return (IServiceConnection*)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::DefaultContainerConnection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::DefaultContainerConnection::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::DefaultContainerConnection::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IServiceConnection*)this) {
        *pIID = EIID_IServiceConnection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CCapsuleManagerService::DefaultContainerConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    if (CCapsuleManagerService::DEBUG_SD_INSTALL) Logger::I(TAG, "onServiceConnected");

    IMediaContainerService* imcs = IMediaContainerService::Probe(service);
    assert(imcs != NULL);

    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(IMediaContainerService*);

    pHandlerFunc = &CCapsuleManagerService::HandleMCSBound;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr(imcs);

    return mOwner->SendMessage(*(Handle32*)&pHandlerFunc, params);
}

ECode CCapsuleManagerService::DefaultContainerConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "onServiceDisconnected");
    return NOERROR;
}

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::HandlerParams

const Int32 CCapsuleManagerService::HandlerParams::MAX_RETRIES;

PInterface CCapsuleManagerService::HandlerParams::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::HandlerParams::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::HandlerParams::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::HandlerParams::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

void CCapsuleManagerService::HandlerParams::StartCopy()
{
//	    try {
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "startCopy");
    mRetry++;
    if (mRetry > MAX_RETRIES) {
        Slogger::W(TAG, "Failed to invoke remote methods on default container service. Giving up");

        void (STDCALL CCapsuleManagerService::*pHandlerFunc)();

        pHandlerFunc = &CCapsuleManagerService::HandleMCSGiveUp;
        mOwner->SendMessage(*(Handle32*)&pHandlerFunc, NULL);
        HandleServiceError();
        return;
    }
    else {
        ECode ec = HandleStartCopy();
        if (SUCCEEDED(ec)) {
            if (DEBUG_SD_INSTALL) Logger::I(TAG, "Posting install MCS_UNBIND");

            void (STDCALL CCapsuleManagerService::*pHandlerFunc)();

            pHandlerFunc = &CCapsuleManagerService::HandleMCSUnbind;
            mOwner->SendMessage(*(Handle32*)&pHandlerFunc, NULL);
        }
        else {
	        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Posting install MCS_RECONNECT");

	        void (STDCALL CCapsuleManagerService::*pHandlerFunc)();

            pHandlerFunc = &CCapsuleManagerService::HandleMCSReconnect;
            mOwner->SendMessage(*(Handle32*)&pHandlerFunc, NULL);
        }
    }
//	    } catch (RemoteException e) {
//	        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Posting install MCS_RECONNECT");
//	        mHandler->SendEmptyMessage(MCS_RECONNECT);
//	    }
    HandleReturnCode();
}

void CCapsuleManagerService::HandlerParams::ServiceError()
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "serviceError");
    HandleServiceError();
    HandleReturnCode();
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::InstallParams

Int32 CCapsuleManagerService::InstallParams::InstallLocationPolicy(
    /* [in] */ ICapsuleInfoLite* capLite,
    /* [in] */ Int32 flags)
{
    assert(capLite);
    String capsuleName;
    capLite->GetCapsuleName(&capsuleName);
    Int32 installLocation;
    capLite->GetInstallLocation(&installLocation);
    Boolean onSd = (flags & CapsuleManager::INSTALL_EXTERNAL) != 0;
    {
        Mutex::Autolock lock(mOwner->mCapsulesLock);

        HashMap<String, CapsuleParser::Capsule*>::Iterator it
                            = mOwner->mCapsules.Find(capsuleName);
        CapsuleParser::Capsule* cap = it->mSecond;
        if (cap != NULL) {
            if ((flags & CapsuleManager::INSTALL_REPLACE_EXISTING) != 0) {
                // Check for updated system application.
                Int32 capFlags;
                cap->mApplicationInfo->GetFlags(&capFlags);
                if ((capFlags & ApplicationInfo_FLAG_SYSTEM) != 0) {
                    if (onSd) {
                        Slogger::W(TAG, "Cannot install update to system app on sdcard");
                        return CapsuleHelper::RECOMMEND_FAILED_INVALID_LOCATION;
                    }
                    return CapsuleHelper::RECOMMEND_INSTALL_INTERNAL;
                }
                else {
                    if (onSd) {
                        // Install flag overrides everything.
                        return CapsuleHelper::RECOMMEND_INSTALL_EXTERNAL;
                    }
                    // If current upgrade specifies particular preference
                    if (installLocation == CapsuleInfo_INSTALL_LOCATION_INTERNAL_ONLY) {
                        // Application explicitly specified internal.
                        return CapsuleHelper::RECOMMEND_INSTALL_INTERNAL;
                    }
                    else if (installLocation == CapsuleInfo_INSTALL_LOCATION_PREFER_EXTERNAL) {
                        // App explictly prefers external. Let policy decide
                    }
                    else {
                        // Prefer previous location
                        if (mOwner->IsExternal(cap)) {
                            return CapsuleHelper::RECOMMEND_INSTALL_EXTERNAL;
                        }
                        return CapsuleHelper::RECOMMEND_INSTALL_INTERNAL;
                    }
                }
            }
            else {
                // Invalid install. Return error code
                return CapsuleHelper::RECOMMEND_FAILED_ALREADY_EXISTS;
            }
        }
    }
    // All the special cases have been taken care of.
    // Return result based on recommended install location.
    if (onSd) {
        return CapsuleHelper::RECOMMEND_INSTALL_EXTERNAL;
    }
    capLite->GetRecommendedInstallLocation(&installLocation);
    return installLocation;
}

ECode CCapsuleManagerService::InstallParams::HandleStartCopy()
{
    Int32 ret = CapsuleManager::INSTALL_SUCCEEDED;
    Boolean fwdLocked = (mFlags & CapsuleManager::INSTALL_FORWARD_LOCK) != 0;
    Boolean onSd = (mFlags & CapsuleManager::INSTALL_EXTERNAL) != 0;
    Boolean onInt = (mFlags & CapsuleManager::INSTALL_INTERNAL) != 0;
    if (onInt && onSd) {
        // Check if both bits are set.
        Slogger::W(TAG, "Conflicting flags specified for installing on both internal and external");
        ret = CapsuleManager::INSTALL_FAILED_INVALID_INSTALL_LOCATION;
    }
    else if (fwdLocked && onSd) {
        // Check for forward locked apps
        Slogger::W(TAG, "Cannot install fwd locked apps on sdcard");
        ret = CapsuleManager::INSTALL_FAILED_INVALID_INSTALL_LOCATION;
    }
    else {
        // Remote call to find out default install location
        mOwner->mContext->GrantUriPermission(
            String(DEFAULT_CONTAINER_CAPSULE),
            mCapsuleURI, Intent_FLAG_GRANT_READ_URI_PERMISSION);
        AutoPtr<ICapsuleInfoLite> capLite;
        mOwner->mContainerService->GetMinimalCapsuleInfo(
            mCapsuleURI, mFlags, (ICapsuleInfoLite**)&capLite);
        mOwner->mContext->RevokeUriPermission(mCapsuleURI,
            Intent_FLAG_GRANT_READ_URI_PERMISSION);

        Int32 loc;
        capLite->GetRecommendedInstallLocation(&loc);
        if (loc == CapsuleHelper::RECOMMEND_FAILED_INVALID_LOCATION){
            ret = CapsuleManager::INSTALL_FAILED_INVALID_INSTALL_LOCATION;
        }
        else if (loc == CapsuleHelper::RECOMMEND_FAILED_ALREADY_EXISTS){
            ret = CapsuleManager::INSTALL_FAILED_ALREADY_EXISTS;
        }
        else if (loc == CapsuleHelper::RECOMMEND_FAILED_INSUFFICIENT_STORAGE){
            ret = CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
        }
        else if (loc == CapsuleHelper::RECOMMEND_FAILED_INVALID_APK) {
            ret = CapsuleManager::INSTALL_FAILED_INVALID_APK;
        }
        else if (loc == CapsuleHelper::RECOMMEND_MEDIA_UNAVAILABLE) {
            ret = CapsuleManager::INSTALL_FAILED_MEDIA_UNAVAILABLE;
        }
        else {
            // Override with defaults if needed.
            loc = InstallLocationPolicy(capLite, mFlags);
            if (!onSd && !onInt) {
                // Override install location with flags
                if (loc == CapsuleHelper::RECOMMEND_INSTALL_EXTERNAL) {
                    // Set the flag to install on external media.
                    mFlags |= CapsuleManager::INSTALL_EXTERNAL;
                    mFlags &= ~CapsuleManager::INSTALL_INTERNAL;
                }
                else {
                    // Make sure the flag for installing on external
                    // media is unset
                    mFlags |= CapsuleManager::INSTALL_INTERNAL;
                    mFlags &= ~CapsuleManager::INSTALL_EXTERNAL;
                }
            }
        }
    }
    // Create the file args now.
    mArgs = mOwner->CreateInstallArgs(this);
    if (ret == CapsuleManager::INSTALL_SUCCEEDED) {
        // Create copy only if we are not in an erroneous state.
        // Remote call to initiate copy using temporary file
        ret = mArgs->CopyCap(mOwner->mContainerService, TRUE);
    }
    mRet = ret;
    return NOERROR;
}

void CCapsuleManagerService::InstallParams::HandleReturnCode()
{
    // If mArgs is null, then MCS couldn't be reached. When it
    // reconnects, it will try again to install. At that point, this
    // will succeed.
    if (mArgs != NULL) {
        mOwner->ProcessPendingInstall(mArgs, mRet);
    }
}

void CCapsuleManagerService::InstallParams::HandleServiceError()
{
    mArgs = mOwner->CreateInstallArgs(this);
    mRet = CapsuleManager::INSTALL_FAILED_INTERNAL_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::MoveParams
CCapsuleManagerService::MoveParams::MoveParams(
    /* [in] */ InstallArgs* srcArgs,
    /* [in] */ ICapsuleMoveObserver* observer,
    /* [in] */ Int32 flags,
    /* [in] */ const String& capsuleName,
    /* [in] */ const String& dataDir,
    /* [in] */ CCapsuleManagerService* owner)
    : HandlerParams(owner)
    , mObserver(observer)
    , mFlags(flags)
    , mCapsuleName(capsuleName)
    , mSrcArgs(srcArgs)
    , mTargetArgs(NULL)
    , mRet(0)
{
    if (srcArgs != NULL) {
        AutoPtr<IFile> file;
        CFile::New(srcArgs->GetCodePath(), (IFile**)&file);
        AutoPtr<IUri> capsuleUri = Uri::FromFile(file);
        mTargetArgs = mOwner->CreateInstallArgs(capsuleUri, flags, capsuleName, dataDir);
    }
}

//@Override
ECode CCapsuleManagerService::MoveParams::HandleStartCopy()
{
    mRet = CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
    // Check for storage space on target medium
    if (!mTargetArgs->CheckFreeStorage(mOwner->mContainerService)) {
        Slogger::W(TAG, "Insufficient storage to install");
        return E_FAIL;
    }
    // Create the file args now.
    mRet = mTargetArgs->CopyCap(mOwner->mContainerService, FALSE);
    mTargetArgs->DoPreInstall(mRet);
//    if (DEBUG_SD_INSTALL) {
//        StringBuilder builder = new StringBuilder();
//        if (srcArgs != null) {
//            builder.append("src: ");
//            builder.append(srcArgs.getCodePath());
//        }
//        if (targetArgs != null) {
//            builder.append(" target : ");
//            builder.append(targetArgs.getCodePath());
//        }
//        Log.i(TAG, builder.toString());
//    }
    return NOERROR;
}

//@Override
void CCapsuleManagerService::MoveParams::HandleReturnCode()
{
    mTargetArgs->DoPostInstall(mRet);
    Int32 currentStatus = CapsuleManager_MOVE_FAILED_INTERNAL_ERROR;
    if (mRet == CapsuleManager::INSTALL_SUCCEEDED) {
        currentStatus = CapsuleManager_MOVE_SUCCEEDED;
    }
    else if (mRet == CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE){
        currentStatus = CapsuleManager_MOVE_FAILED_INSUFFICIENT_STORAGE;
    }
    mOwner->ProcessPendingMove(this, currentStatus);
}

//@Override
void CCapsuleManagerService::MoveParams::HandleServiceError()
{
    mRet = CapsuleManager::INSTALL_FAILED_INTERNAL_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::InstallArgs

PInterface CCapsuleManagerService::InstallArgs::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::InstallArgs::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::InstallArgs::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::InstallArgs::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}


///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::FileInstallArgs

CCapsuleManagerService::FileInstallArgs::FileInstallArgs(
    /* [in] */ const String& fullCodePath,
    /* [in] */ const String& fullResourcePath,
    /* [in] */ const String& nativeLibraryPath,
    /* [in] */ CCapsuleManagerService* owner)
    : InstallArgs(NULL, NULL, 0, String(NULL), owner)
    , mCreated(FALSE)
{
    AutoPtr<IFile> codeFile;
    CFile::New(fullCodePath, (IFile**)&codeFile);
    codeFile->GetParentFile((IFile**)&mInstallDir);
    mCodeFileName = fullCodePath;
    mResourceFileName = fullResourcePath;
    mLibraryPath = nativeLibraryPath;
}

CCapsuleManagerService::FileInstallArgs::FileInstallArgs(
    /* [in] */ IUri* capsuleURI,
    /* [in] */ const String& capName,
    /* [in] */ const String& dataDir,
    /* [in] */ CCapsuleManagerService* owner)
    : InstallArgs(capsuleURI, NULL, 0, String(NULL), owner)
    , mCreated(FALSE)
{
    mInstallDir = IsFwdLocked() ? mOwner->mDrmAppPrivateInstallDir : mOwner->mAppInstallDir;
    String name = mOwner->GetNextCodePath(String(NULL), capName, String(".apk"));
    AutoPtr<IFile> codeFile;
    CFile::New(mInstallDir, name + ".apk", (IFile**)&codeFile);
    codeFile->GetPath(&mCodeFileName);
    mResourceFileName = GetResourcePathFromCodePath();
    AutoPtr<IFile> library;
    CFile::New(dataDir, String(LIB_DIR_NAME), (IFile**)&library);
    library->GetPath(&mLibraryPath);
}

Boolean CCapsuleManagerService::FileInstallArgs::CheckFreeStorage(
    /* [in] */ IMediaContainerService* imcs)
{
//    try {
    mOwner->mContext->GrantUriPermission(
        String(DEFAULT_CONTAINER_CAPSULE),
        mCapsuleURI, Intent_FLAG_GRANT_READ_URI_PERMISSION);
    Boolean ret;
    imcs->CheckFreeStorage(FALSE, mCapsuleURI, &ret);
    mOwner->mContext->RevokeUriPermission(mCapsuleURI,
        Intent_FLAG_GRANT_READ_URI_PERMISSION);
    return ret;
//    } finally {
//        mContext.revokeUriPermission(packageURI, Intent.FLAG_GRANT_READ_URI_PERMISSION);
//    }
}

String CCapsuleManagerService::FileInstallArgs::GetCodePath()
{
    return mCodeFileName;
}

void CCapsuleManagerService::FileInstallArgs::CreateCopyFile()
{
    mInstallDir = IsFwdLocked() ? mOwner->mDrmAppPrivateInstallDir : mOwner->mAppInstallDir;
    mOwner->CreateTempCapsuleFile(mInstallDir)->GetPath(&mCodeFileName);
    mResourceFileName = GetResourcePathFromCodePath();
    mCreated = TRUE;
}

Int32 CCapsuleManagerService::FileInstallArgs::CopyCap(
    /* [in] */ IMediaContainerService* imcs,
    /* [in] */ Boolean temp)
{
    if (temp) {
        // Generate temp file name
        CreateCopyFile();
    }
    // Get a ParcelFileDescriptor to write to the output file
    AutoPtr<IFile> codeFile;
    CFile::New(mCodeFileName, (IFile**)&codeFile);
    Boolean succeeded;
    ECode ec;
    if (!mCreated) {
//        try {
        ec = codeFile->CreateNewFile(&succeeded);
        if (SUCCEEDED(ec) && succeeded) {
            // Set permissions
            if (!SetPermissions()) {
                // Failed setting permissions.
                return CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
            }
        }
        else {
//            Slogger::W(TAG, "Failed to create file " + codeFile);
            return CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
        }
//        } catch (IOException e) {
//           Slog.w(TAG, "Failed to create file " + codeFile);
//           return PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
//        }
    }
    AutoPtr<IParcelFileDescriptor> out;
//    try {
    AutoPtr<IParcelFileDescriptorHelper> helper;
    CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&helper);
    ec = helper->Open(codeFile, ParcelFileDescriptor_MODE_READ_WRITE, (IParcelFileDescriptor**)&out);
    if (FAILED(ec)) {
//        Slog.e(TAG, "Failed to create file descritpor for : " + codeFileName);
        return CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
    }
//    } catch (FileNotFoundException e) {
//        Slog.e(TAG, "Failed to create file descritpor for : " + codeFileName);
//        return PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
//    }
    // Copy the resource now
    Int32 ret = CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
//    try {
    mOwner->mContext->GrantUriPermission(
        String(DEFAULT_CONTAINER_CAPSULE), mCapsuleURI,
        Intent_FLAG_GRANT_READ_URI_PERMISSION);
    imcs->CopyResource(mCapsuleURI, out, &succeeded);
    if (succeeded) {
        ret = CapsuleManager::INSTALL_SUCCEEDED;
    }
    if (out != NULL) out->Close();
    mOwner->mContext->RevokeUriPermission(mCapsuleURI,
        Intent_FLAG_GRANT_READ_URI_PERMISSION);
//    } finally {
//        try { if (out != null) out.close(); } catch (IOException e) {}
//        mContext.revokeUriPermission(packageURI, Intent.FLAG_GRANT_READ_URI_PERMISSION);
//    }
//
    return ret;
}

Int32 CCapsuleManagerService::FileInstallArgs::DoPreInstall(
    /* [in] */ Int32 status)
{
    if (status != CapsuleManager::INSTALL_SUCCEEDED) {
        CleanUp();
    }
    return status;
}

Boolean CCapsuleManagerService::FileInstallArgs::DoRename(
    /* [in] */ Int32 status,
    /* [in] */ const String& capName,
    /* [in] */ const String& oldCodePath)
{
    if (status != CapsuleManager::INSTALL_SUCCEEDED) {
        CleanUp();
        return FALSE;
    }
    else {
        // Rename based on packageName
        AutoPtr<IFile> codeFile;
        CFile::New(GetCodePath(), (IFile**)&codeFile);
        String name = mOwner->GetNextCodePath(oldCodePath, capName, String(".apk"));
        AutoPtr<IFile> desFile;
        CFile::New(mInstallDir, name + ".apk", (IFile**)&desFile);
        Boolean succeeded;
        if (codeFile->RenameTo(desFile, &succeeded), !succeeded) {
            return FALSE;
        }
        // Reset paths since the file has been renamed.
        desFile->GetPath(&mCodeFileName);
        mResourceFileName = GetResourcePathFromCodePath();
        // Set permissions
        if (!SetPermissions()) {
            // Failed setting permissions.
            return FALSE;
        }
        return TRUE;
    }
}

Int32 CCapsuleManagerService::FileInstallArgs::DoPostInstall(
    /* [in] */ Int32 status)
{
    if (status != CapsuleManager::INSTALL_SUCCEEDED) {
        CleanUp();
    }
    return status;
}

String CCapsuleManagerService::FileInstallArgs::GetResourcePath()
{
    return mResourceFileName;
}

String CCapsuleManagerService::FileInstallArgs::GetResourcePathFromCodePath()
{
    String codePath = GetCodePath();
    if ((mFlags & CapsuleManager::INSTALL_FORWARD_LOCK) != 0) {
        String capNameOnly = GetCapName(codePath);
        String resPath;
        mOwner->mAppInstallDir->GetPath(&resPath);
        return resPath + "/" + capNameOnly + ".zip";
    }
    else {
        return codePath;
    }
}

String CCapsuleManagerService::FileInstallArgs::GetNativeLibraryPath()
{
    return mLibraryPath;
}

Boolean CCapsuleManagerService::FileInstallArgs::CleanUp()
{
    Boolean ret = TRUE;
    String sourceDir = GetCodePath();
    String publicSourceDir = GetResourcePath();
    if (!sourceDir.IsNull()) {
        AutoPtr<IFile> sourceFile;
        CFile::New(sourceDir, (IFile**)&sourceFile);
        Boolean result;
        if (sourceFile->Exists(&result), !result) {
//            Slog.w(TAG, "Package source " + sourceDir + " does not exist.");
            ret = FALSE;
        }
        // Delete application's code and resources
        sourceFile->Delete(&result);
    }
    if (!publicSourceDir.IsNull() && !publicSourceDir.Equals(sourceDir)) {
        AutoPtr<IFile> publicSourceFile;
        CFile::New(publicSourceDir, (IFile**)&publicSourceFile);
        Boolean result;
        if (publicSourceFile->Exists(&result), !result) {
//            Slog.w(TAG, "Package public source " + publicSourceFile + " does not exist.");
        }
        else {
            publicSourceFile->Delete(&result);
        }
    }
    return ret;
}

void CCapsuleManagerService::FileInstallArgs::CleanUpResourcesLI()
{
    String sourceDir = GetCodePath();
    if (CleanUp() && mOwner->mInstaller != NULL) {
        //todo: RmDex should rename to RmEco
        Int32 retCode = mOwner->mInstaller->RmDex(sourceDir);
        if (retCode < 0) {
//            Slog.w(TAG, "Couldn't remove dex file for package: "
//                    +  " at location "
//                    + sourceDir + ", retcode=" + retCode);
            // we don't consider this to be a failure of the core package deletion
        }
    }
}

Boolean CCapsuleManagerService::FileInstallArgs::SetPermissions()
{
    // TODO Do this in a more elegant way later on. for now just a hack
    if (!IsFwdLocked()) {
        Int32 filePermissions = FileUtils::IRUSR | FileUtils::IWUSR
            | FileUtils::IRGRP | FileUtils::IROTH;
        Int32 retCode = FileUtils::SetPermissions(GetCodePath(), filePermissions, -1, -1);
        if (retCode != 0) {
//            Slog.e(TAG, "Couldn't set new package file permissions for " +
//                    getCodePath()
//                    + ". The return code was: " + retCode);
            // TODO Define new internal error
            return FALSE;
        }
        return TRUE;
    }
    return TRUE;
}

Boolean CCapsuleManagerService::FileInstallArgs::DoPostDeleteLI(
    /* [in] */ Boolean del)
{
    // XXX err, shouldn't we respect the delete flag?
    CleanUpResourcesLI();
    return TRUE;
}

Boolean CCapsuleManagerService::FileInstallArgs::IsFwdLocked()
{
    return (mFlags & CapsuleManager::INSTALL_FORWARD_LOCK) != 0;
}

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::SdInstallArgs

const char* CCapsuleManagerService::SdInstallArgs::RES_FILE_NAME = "cap.apk";

CCapsuleManagerService::SdInstallArgs::SdInstallArgs(
    /* [in] */ const String& fullCodePath,
    /* [in] */ const String& fullResourcePath,
    /* [in] */ const String& nativeLibraryPath,
    /* [in] */ CCapsuleManagerService* owner)
    : InstallArgs(NULL, NULL, CapsuleManager::INSTALL_EXTERNAL, String(NULL), owner)
{
    // Extract cid from fullCodePath
    Int32 eidx = fullCodePath.LastIndexOf("/");
    String subStr1 = fullCodePath.Substring(0, eidx);
    Int32 sidx = subStr1.LastIndexOf("/");
    mCid = subStr1.Substring(sidx + 1, eidx);
    SetCachePath(subStr1);
}

CCapsuleManagerService::SdInstallArgs::SdInstallArgs(
    /* [in] */ const String& cid,
    /* [in] */ CCapsuleManagerService* owner)
    : InstallArgs(NULL, NULL, CapsuleManager::INSTALL_EXTERNAL, String(NULL), owner)
    , mCid(cid)
{
    SetCachePath(CapsuleHelper::GetSdDir(cid));
}

void CCapsuleManagerService::SdInstallArgs::CreateCopyFile()
{
    mCid = GetTempContainerId();
}

Boolean CCapsuleManagerService::SdInstallArgs::CheckFreeStorage(
    /* [in] */ IMediaContainerService* imcs)
{
//    try {
    mOwner->mContext->GrantUriPermission(
        String(DEFAULT_CONTAINER_CAPSULE), mCapsuleURI,
        Intent_FLAG_GRANT_READ_URI_PERMISSION);
    Boolean ret;
    imcs->CheckFreeStorage(TRUE, mCapsuleURI, &ret);
    mOwner->mContext->RevokeUriPermission(mCapsuleURI,
        Intent_FLAG_GRANT_READ_URI_PERMISSION);
    return ret;
//    } finally {
//        mContext.revokeUriPermission(packageURI, Intent.FLAG_GRANT_READ_URI_PERMISSION);
//    }
}

Int32 CCapsuleManagerService::SdInstallArgs::CopyCap(
    /* [in] */ IMediaContainerService* imcs,
    /* [in] */ Boolean temp)
{
    if (temp) {
        CreateCopyFile();
    }

    String newCachePath;
//    try {
    mOwner->mContext->GrantUriPermission(
        String(DEFAULT_CONTAINER_CAPSULE), mCapsuleURI,
        Intent_FLAG_GRANT_READ_URI_PERMISSION);
    imcs->CopyResourceToContainer(mCapsuleURI, mCid,
        mOwner->GetEncryptKey(), String(RES_FILE_NAME), &newCachePath);
    mOwner->mContext->RevokeUriPermission(mCapsuleURI,
        Intent_FLAG_GRANT_READ_URI_PERMISSION);
//    } finally {
//        mContext.revokeUriPermission(packageURI, Intent.FLAG_GRANT_READ_URI_PERMISSION);
//    }

    if (!newCachePath.IsNull()) {
        SetCachePath(newCachePath);
        return CapsuleManager::INSTALL_SUCCEEDED;
    }
    else {
        return CapsuleManager::INSTALL_FAILED_CONTAINER_ERROR;
    }
}

String CCapsuleManagerService::SdInstallArgs::GetCodePath()
{
    return mCapsulePath;
}

String CCapsuleManagerService::SdInstallArgs::GetResourcePath()
{
    return mCapsulePath;
}

String CCapsuleManagerService::SdInstallArgs::GetNativeLibraryPath()
{
    return mLibraryPath;
}

Int32 CCapsuleManagerService::SdInstallArgs::DoPreInstall(
    /* [in] */ Int32 status)
{
    if (status != CapsuleManager::INSTALL_SUCCEEDED) {
        // Destroy container
        CapsuleHelper::DestroySdDir(mCid);
    }
    else {
        Boolean mounted = CapsuleHelper::IsContainerMounted(mCid);
        if (!mounted) {
            String newCachePath = CapsuleHelper::MountSdDir(mCid, mOwner->GetEncryptKey(),
                    Process::SYSTEM_UID);
            if (!newCachePath.IsNull()) {
                SetCachePath(newCachePath);
            }
            else {
                return CapsuleManager::INSTALL_FAILED_CONTAINER_ERROR;
            }
        }
    }
    return status;
}

Boolean CCapsuleManagerService::SdInstallArgs::DoRename(
    /* [in] */ Int32 status,
    /* [in] */ const String& capName,
    /* [in] */ const String& oldCodePath)
{
    String newCacheId = mOwner->GetNextCodePath(oldCodePath,
        capName, String("/") + String(RES_FILE_NAME));
    String newCachePath(NULL);
    if (CapsuleHelper::IsContainerMounted(mCid)) {
        // Unmount the container
        if (!CapsuleHelper::UnMountSdDir(mCid)) {
//            Slog.i(TAG, "Failed to unmount " + cid + " before renaming");
            return FALSE;
        }
    }
    if (!CapsuleHelper::RenameSdDir(mCid, newCacheId)) {
//        Slog.e(TAG, "Failed to rename " + cid + " to " + newCacheId +
//                " which might be stale. Will try to clean up.");
        // Clean up the stale container and proceed to recreate.
        if (!CapsuleHelper::DestroySdDir(newCacheId)) {
//            Slog.e(TAG, "Very strange. Cannot clean up stale container " + newCacheId);
            return FALSE;
        }
        // Successfully cleaned up stale container. Try to rename again.
        if (!CapsuleHelper::RenameSdDir(mCid, newCacheId)) {
//            Slog.e(TAG, "Failed to rename " + cid + " to " + newCacheId
//                    + " inspite of cleaning it up.");
            return FALSE;
        }
    }
    if (!CapsuleHelper::IsContainerMounted(newCacheId)) {
//        Slog.w(TAG, "Mounting container " + newCacheId);
        newCachePath = CapsuleHelper::MountSdDir(newCacheId,
                mOwner->GetEncryptKey(), Process::SYSTEM_UID);
    }
    else {
        newCachePath = CapsuleHelper::GetSdDir(newCacheId);
    }
    if (newCachePath.IsNull()) {
//        Slog.w(TAG, "Failed to get cache path for  " + newCacheId);
        return FALSE;
    }
//    Log.i(TAG, "Succesfully renamed " + cid +
//            " to " + newCacheId +
//            " at new path: " + newCachePath);
    mCid = newCacheId;
    SetCachePath(newCachePath);
    return TRUE;
}

void CCapsuleManagerService::SdInstallArgs::SetCachePath(
    /* [in] */ String newCachePath)
{
    AutoPtr<IFile> cachePath;
    CFile::New(newCachePath, (IFile**)&cachePath);
    AutoPtr<IFile> library, capsule;
    CFile::New(cachePath, String(LIB_DIR_NAME), (IFile**)&library);
    library->GetPath(&mLibraryPath);
    CFile::New(cachePath, String(RES_FILE_NAME), (IFile**)&capsule);
    capsule->GetPath(&mCapsulePath);
}

Int32 CCapsuleManagerService::SdInstallArgs::DoPostInstall(
    /* [in] */ Int32 status)
{
    if (status != CapsuleManager::INSTALL_SUCCEEDED) {
        CleanUp();
    }
    else {
        Boolean mounted = CapsuleHelper::IsContainerMounted(mCid);
        if (!mounted) {
            CapsuleHelper::MountSdDir(mCid,
                mOwner->GetEncryptKey(), Process::MyUid());
        }
    }
    return status;
}

void CCapsuleManagerService::SdInstallArgs::CleanUp()
{
    // Destroy secure container
    CapsuleHelper::DestroySdDir(mCid);
}

void CCapsuleManagerService::SdInstallArgs::CleanUpResourcesLI()
{
    String sourceFile = GetCodePath();
    // Remove dex file
    if (mOwner->mInstaller != NULL) {
        Int32 retCode = mOwner->mInstaller->RmDex(sourceFile);
        if (retCode < 0) {
//            Slog.w(TAG, "Couldn't remove dex file for package: "
//                    + " at location "
//                    + sourceFile.toString() + ", retcode=" + retCode);
            // we don't consider this to be a failure of the core package deletion
        }
    }
    CleanUp();
}

Boolean CCapsuleManagerService::SdInstallArgs::MatchContainer(
    /* [in] */ String app)
{
    if (mCid.StartWith(app)) {
        return TRUE;
    }
    return FALSE;
}

String CCapsuleManagerService::SdInstallArgs::GetCapsuleName()
{
    Int32 idx = mCid.LastIndexOf("-");
    if (idx == -1) {
        return mCid;
    }
    return mCid.Substring(0, idx);
}

Boolean CCapsuleManagerService::SdInstallArgs::DoPostDeleteLI(
    /* [in] */ Boolean del)
{
    Boolean ret = FALSE;
    Boolean mounted = CapsuleHelper::IsContainerMounted(mCid);
    if (mounted) {
        // Unmount first
        ret = CapsuleHelper::UnMountSdDir(mCid);
    }
    if (ret && del) {
        CleanUpResourcesLI();
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleRemovedInfo

void CCapsuleManagerService::CapsuleRemovedInfo::SendBroadcast(
    /* [in] */ Boolean fullRemove,
    /* [in] */ Boolean replacing)
{
    AutoPtr<IBundle> extras;
    ASSERT_SUCCEEDED(CBundle::New(1, (IBundle**)&extras));
    extras->PutInt32(
        String(Intent_EXTRA_UID), mRemovedUid >= 0 ? mRemovedUid : mUid);
    extras->PutBoolean(String(Intent_EXTRA_DATA_REMOVED), fullRemove);
    if (replacing) {
        extras->PutBoolean(String(Intent_EXTRA_REPLACING), TRUE);
    }
    if (!mRemovedCapsule.IsNull()) {
        mOwner->SendCapsuleBroadcast(
            String(Intent_ACTION_CAPSULE_REMOVED),
            mRemovedCapsule, extras, NULL);
    }
    if (mRemovedUid >= 0) {
        mOwner->SendCapsuleBroadcast(
            String(Intent_ACTION_UID_REMOVED),
            String(NULL), extras, NULL);
    }
}

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::AppDirObserver

CCapsuleManagerService::AppDirObserver::AppDirObserver(
    /* [in] */ const String& path,
    /* [in] */ Int32 mask,
    /* [in] */ Boolean isrom,
    /* [in] */ CCapsuleManagerService* owner)
    : FileObserver(path, mask)
    , mRootDir(path)
    , mIsRom(FALSE)
    , mOwner(owner)
{}

PInterface CCapsuleManagerService::AppDirObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IFileObserver) {
        return (IFileObserver*)this;
    }
    else if (riid == EIID_IInterface) {
        return (IInterface*)(IFileObserver*)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::AppDirObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::AppDirObserver::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::AppDirObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::AppDirObserver::StartWatching()
{
    return FileObserver::StartWatching();
}

ECode CCapsuleManagerService::AppDirObserver::StopWatching()
{
    return FileObserver::StopWatching();
}

ECode CCapsuleManagerService::AppDirObserver::OnEvent(
    /* [in] */ Int32 event,
    /* [in] */ const String& path)
{
    String removedCapsule, addedCapsule;
    Int32 removedUid = -1, addedUid = -1;

    {
        Mutex::Autolock lock(mOwner->mInstallLock);

        String fullPathStr;
        AutoPtr<IFile> fullPath;
        if (!path.IsNull()) {
            CFile::New(mRootDir, path, (IFile**)&fullPath);
            fullPath->GetPath(&fullPathStr);
        }

        if (Config::LOGV) {
            Logger::V(TAG, StringBuffer("File ") + fullPathStr + " changed: "
                + event /*+ Integer::ToHexString(event)*/);
        }

        if (!IsCapsuleFilename(path)) {
            if (Config::LOGV) {
                Logger::V(TAG,
                    StringBuffer("Ignoring change of non-capsule file: ")
                    + fullPathStr);
            }
            return NOERROR;
        }

        // Ignore packages that are being installed or
        // have just been installed.
        if (mOwner->IgnoreCodePath(fullPathStr)) {
            return NOERROR;
        }

        CapsuleParser::Capsule* c = NULL;
        {
            Mutex::Autolock l(mOwner->mCapsulesLock);

            HashMap<String, CapsuleParser::Capsule*>::Iterator it
                = mOwner->mAppDirs.Find(fullPathStr);
            c = it->mSecond;
        }

        if ((event & REMOVE_EVENTS) != 0) {
            if (c != NULL) {
                mOwner->RemoveCapsuleLI(c, TRUE);
                c->mApplicationInfo->GetCapsuleName(&removedCapsule);
                c->mApplicationInfo->GetUid(&removedUid);
            }
        }

        if ((event & ADD_EVENTS) != 0) {
            if (c == NULL) {
                c = mOwner->ScanCapsuleLI(fullPath.Get(),
                        (mIsRom ? CapsuleParser::PARSE_IS_SYSTEM
                                | CapsuleParser::PARSE_IS_SYSTEM_DIR: 0) |
                        CapsuleParser::PARSE_CHATTY |
                        CapsuleParser::PARSE_MUST_BE_APK,
                        SCAN_MONITOR | SCAN_NO_PATHS | SCAN_UPDATE_TIME,
                        System::GetCurrentTimeMillis());
                if (c != NULL) {
                    {
                        Mutex::Autolock l(mOwner->mCapsulesLock);

                        mOwner->UpdatePermissionsLP(c->mCapsuleName, c,
                            c->mPermissions.GetSize() > 0, FALSE, FALSE);
                    }
                    c->mApplicationInfo->GetCapsuleName(&addedCapsule);
                    c->mApplicationInfo->GetUid(&addedUid);
                }
            }
        }

        {
            Mutex::Autolock l(mOwner->mCapsulesLock);
            mOwner->mSettings->WriteLP();
        }
    }

    if (removedCapsule != NULL) {
        AutoPtr<IBundle> extras;
        CBundle::New(1, (IBundle**)&extras);
        extras->PutInt32(String(Intent_EXTRA_UID), removedUid);
        extras->PutBoolean(String(Intent_EXTRA_DATA_REMOVED), FALSE);
        mOwner->SendCapsuleBroadcast(
            String(Intent_ACTION_CAPSULE_REMOVED),
            removedCapsule, extras.Get(), NULL);
    }

    if (!addedCapsule.IsNull()) {
        AutoPtr<IBundle> extras;
        CBundle::New(1, (IBundle**)&extras);
        extras->PutInt32(String(Intent_EXTRA_UID), addedUid);
        mOwner->SendCapsuleBroadcast(
            String(Intent_ACTION_CAPSULE_ADDED),
            addedCapsule, extras, NULL);
    }

    return NOERROR;
}

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::ActivityIntentResolver

CCapsuleManagerService::ActivityIntentResolver::ActivityIntentResolver(
    /* [in] */ CCapsuleManagerService* owner)
    : mActivities(23)
    , mOwner(owner)
{
}

CCapsuleManagerService::ActivityIntentResolver::~ActivityIntentResolver()
{
    mActivities.Clear();
}

List<AutoPtr<IResolveInfo>*>*
CCapsuleManagerService::ActivityIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Boolean defaultOnly)
{
    mFlags = defaultOnly ? CapsuleManager_MATCH_DEFAULT_ONLY : 0;
    return Super::QueryIntent(intent, resolvedType, defaultOnly);
}

List<AutoPtr<IResolveInfo>*>*
CCapsuleManagerService::ActivityIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return Super::QueryIntent(intent, resolvedType,
        (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0);
}

List<AutoPtr<IResolveInfo>*>*
CCapsuleManagerService::ActivityIntentResolver::QueryIntentForCapsule(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<CapsuleParser::Activity*>* capsuleActivities)
{
    if (capsuleActivities == NULL) {
        return NULL;
    }

    mFlags = flags;
    Boolean defaultOnly = (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0;
    //todo: listCut maybe cause memory leak;
    List<List<CapsuleParser::ActivityIntentInfo*>* >* listCut =
            new List<List<CapsuleParser::ActivityIntentInfo*>* >();

    List<CapsuleParser::ActivityIntentInfo*>* intentFilters;
    List<CapsuleParser::Activity*>::Iterator it;
    for (it = capsuleActivities->Begin(); it != capsuleActivities->End(); ++it) {
        intentFilters = (*it)->mIntents;
        if (intentFilters != NULL && intentFilters->Begin() != intentFilters->End()) {
            listCut->PushBack(intentFilters);
        }
    }
    return Super::QueryIntentFromList(intent, resolvedType, defaultOnly, listCut);
}

void CCapsuleManagerService::ActivityIntentResolver::AddActivity(
    /* [in] */ CapsuleParser::Activity* activity,
    /* [in] */ const char* type)
{
    assert(activity);
    assert(type != NULL);

    AutoPtr<IApplicationInfo> appInfo;
    activity->mInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    Boolean systemApp = CCapsuleManagerService::IsSystemApp(appInfo);
    AutoPtr<IComponentName> componentName;
    activity->GetComponentName((IComponentName**)&componentName);
    mActivities[componentName] = activity;
/*
    if (SHOW_INFO || Config::LOGV) {
        String str;
        if (activity->mInfo->mNonLocalizedLabel != NULL) {
            activity->mInfo->mNonLocalizedLabel->ToString(&str);
        }
        else {
            str = activity->mInfo->mName;
        }
        Slogger::V(TAG, StringBuffer("  ") + type + " " + str + ":");
    }
    if (SHOW_INFO || Config::LOGV) {
        Slogger::V(TAG, StringBuffer("    Class=") + activity->mInfo->mName);
    }
*/
    List<CapsuleParser::ActivityIntentInfo*>::Iterator it;
    assert(activity->mIntents != NULL);
    for (it = activity->mIntents->Begin(); it != activity->mIntents->End(); ++it) {
        CapsuleParser::ActivityIntentInfo* intent = *it;
        Int32 pri;
        if (!systemApp && (intent->GetPriority(&pri), pri > 0) &&
                !strcmp("activity", type)) {
            intent->SetPriority(0);
/*
            Logger::W(TAG, StringBuffer("Capsule ")
                + activity->mInfo->mApplicationInfo->mCapsuleName
                + " has activity "
                + activity->mClassName + " with priority > 0, forcing to 0");
*/
        }
//        if (SHOW_INFO || Config::LOGV) {
//            Slogger::V(TAG, "    IntentFilter:");
////	            intent->Dump(new LogPrinter(Slog::VERBOSE, TAG), "      ");
//        }
//        if (FALSE /*!(*it)->DebugCheck()*/) {
//            Slogger::W(TAG, "==> For Activity " + activity->mInfo->mName);
//        }
        AddFilter(intent);
    }
}

void CCapsuleManagerService::ActivityIntentResolver::RemoveActivity(
    /* [in] */ CapsuleParser::Activity* activity,
    /* [in] */ const char* type)
{
    AutoPtr<IComponentName> componentName;
    activity->GetComponentName((IComponentName**)&componentName);
    mActivities.Erase(componentName);
/*
    if (SHOW_INFO || Config::LOGV) {
        String str;
        if (activity->mInfo->mNonLocalizedLabel != NULL) {
            activity->mInfo->mNonLocalizedLabel->ToString(&str);
        }
        else {
            str = activity->mInfo->mName;
        }
        Slogger::V(TAG, StringBuffer("  ") + type + " " + str + ":");
    }
    if (SHOW_INFO || Config::LOGV) {
        Slogger::V(TAG, StringBuffer("    Class=") + activity->mInfo->mName);
    }
*/
    List<CapsuleParser::ActivityIntentInfo*>::Iterator it;
    for (it = activity->mIntents->Begin(); it != activity->mIntents->End(); ++it) {
/*
        if (SHOW_INFO || Config::LOGV) {
            Slogger::V(TAG, "    IntentFilter:");
//            intent.dump(new LogPrinter(Slog.VERBOSE, TAG), "      ");
        }
*/
        RemoveFilter(*it);
    }
}

Boolean CCapsuleManagerService::ActivityIntentResolver::AllowFilterResult(
    /* [in] */ CapsuleParser::ActivityIntentInfo* filter,
    /* [in] */ List<AutoPtr<IResolveInfo>*>* dest)
{
//    ActivityInfo filterAi = filter.activity.info;
//    for (int i=dest.size()-1; i>=0; i--) {
//        ActivityInfo destAi = dest.get(i).activityInfo;
//        if (destAi.name == filterAi.name
//                && destAi.packageName == filterAi.packageName) {
//            return false;
//        }
//    }
    return TRUE;
}

String CCapsuleManagerService::ActivityIntentResolver::CapsuleForFilter(
    /* [in] */ CapsuleParser::ActivityIntentInfo* info)
{
    return info->mActivity->mOwner->mCapsuleName;
}

ECode CCapsuleManagerService::ActivityIntentResolver::NewResult(
    /* [in] */ CapsuleParser::ActivityIntentInfo* info,
    /* [in] */ Int32 match,
    /* [out] */ AutoPtr<IResolveInfo>** res)
{
//    assert(info);
    assert(res != NULL);
/*
    if (!mOwner->mSettings->IsEnabledLP(info->mActivity->mInfo, mFlags)) {
        *res = NULL;
        return NOERROR;
    }
*/
    CapsuleParser::Activity* activity = info->mActivity;
/*
    if (mOwner->mSafeMode && (activity->mInfo->mApplicationInfo->mFlags
        & ApplicationInfo_FLAG_SYSTEM) == 0) {
        *res = NULL;
        return NOERROR;
    }
*/
    AutoPtr<IResolveInfo> rinfo;
    CResolveInfo::New((IResolveInfo**)&rinfo);
    rinfo->SetActivityInfo(CapsuleParser::GenerateActivityInfo(activity, mFlags));
/*
    if ((mFlags & CapsuleManager_GET_RESOLVED_FILTER) != 0) {
//	        rinfo->mFilter = info;
    }
    info->GetPriority(&rinfo->mPriority);
    rinfo->mPreferredOrder = activity->mOwner->mPreferredOrder;
    //System.out.println("Result: " + res.activityInfo.className +
    //                   " = " + res.priority);
    rinfo->mMatch = match;
    rinfo->mIsDefault = info->mHasDefault;
    rinfo->mLabelRes = info->mLabelRes;
    rinfo->mNonLocalizedLabel = info->mNonLocalizedLabel;
    rinfo->mIcon = info->mIcon;
*/
    *res = new AutoPtr<IResolveInfo>();
    **res = rinfo;
    return NOERROR;
}

void CCapsuleManagerService::ActivityIntentResolver::SortResults(
    /* [in] */ List<AutoPtr<IResolveInfo>*>* results)
{
//	    Collections.sort(results, mResolvePrioritySorter);
}

////////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::ServiceIntentResolver

CCapsuleManagerService::ServiceIntentResolver::ServiceIntentResolver(
    /* [in] */ CCapsuleManagerService* owner)
    : mServices(11)
    , mOwner(owner)
{
}

CCapsuleManagerService::ServiceIntentResolver::~ServiceIntentResolver()
{
    mServices.Clear();
}

List<AutoPtr<IResolveInfo>*>*
CCapsuleManagerService::ServiceIntentResolver::QueryIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return Super::QueryIntent(intent, resolvedType,
        (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0);
}

List<AutoPtr<IResolveInfo>*>*
CCapsuleManagerService::ServiceIntentResolver::QueryIntentForCapsule(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ List<CapsuleParser::Service*>* capsuleServices)
{
    if (capsuleServices == NULL) {
        return NULL;
    }
    mFlags = flags;
    Boolean defaultOnly = (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0;
    List<List<CapsuleParser::ServiceIntentInfo*>* >* listCut =
            new List<List<CapsuleParser::ServiceIntentInfo*>* >();

    List<CapsuleParser::ServiceIntentInfo*>* intentFilters;
    List<CapsuleParser::Service*>::Iterator it;
    for (it = capsuleServices->Begin(); it != capsuleServices->End(); ++it) {
        intentFilters = (*it)->mIntents;
        if (intentFilters != NULL && intentFilters->GetSize() > 0) {
            listCut->PushBack(intentFilters);
        }
    }
    return Super::QueryIntentFromList(intent, resolvedType, defaultOnly, listCut);
}

void CCapsuleManagerService::ServiceIntentResolver::AddService(
    /* [in] */ CapsuleParser::Service* service)
{
    AutoPtr<IComponentName> componentName;
    service->GetComponentName((IComponentName**)&componentName);
    mServices[componentName] = service;
    String sName;
    service->mInfo->GetName(&sName);
    if (SHOW_INFO || Config::LOGV) {
        String str;
        AutoPtr<ICharSequence> label;
        service->mInfo->GetNonLocalizedLabel((ICharSequence**)&label);
        if (label != NULL) {
            label->ToString(&str);
        }
        else {
            str = sName;
        }
        Slogger::V(TAG, StringBuffer("  ") + str + ":");
    }
    if (SHOW_INFO || Config::LOGV) {
        Slogger::V(TAG, StringBuffer("    Class=") + sName);
    }
    List<CapsuleParser::ServiceIntentInfo*>::Iterator it;
    for (it = service->mIntents->Begin(); it != service->mIntents->End(); ++it) {
        if (SHOW_INFO || Config::LOGV) {
            Slogger::V(TAG, "    IntentFilter:");
//	            intent.dump(new LogPrinter(Slog.VERBOSE, TAG), "      ");
        }
        if (FALSE /*!intent->DebugCheck()*/) {
            Slogger::W(TAG, StringBuffer("==> For Service ")
                + sName);
        }
        AddFilter(*it);
    }
}

void CCapsuleManagerService::ServiceIntentResolver::RemoveService(
    /* [in] */ CapsuleParser::Service* service)
{
    AutoPtr<IComponentName> componentName;
    service->GetComponentName((IComponentName**)&componentName);
    mServices.Erase(componentName);
    String sName;
    service->mInfo->GetName(&sName);
    if (SHOW_INFO || Config::LOGV) {
        String str;
        AutoPtr<ICharSequence> label;
        service->mInfo->GetNonLocalizedLabel((ICharSequence**)&label);
        if (label != NULL) {
            label->ToString(&str);
        }
        else {
            str = sName;
        }
        Slogger::V(TAG, StringBuffer("  ") + str + ":");
    }
    if (SHOW_INFO || Config::LOGV) {
        Slogger::V(TAG, StringBuffer("    Class=") + sName);
    }
    List<CapsuleParser::ServiceIntentInfo*>::Iterator it;
    for (it = service->mIntents->Begin(); it != service->mIntents->End(); ++it) {
        if (SHOW_INFO || Config::LOGV) {
            Slogger::V(TAG, "    IntentFilter:");
//	            intent->Dump(new LogPrinter(Slog::VERBOSE, TAG), "      ");
        }
        RemoveFilter(*it);
    }
}

//@Override
Boolean CCapsuleManagerService::ServiceIntentResolver::AllowFilterResult(
    /* [in] */ CapsuleParser::ServiceIntentInfo* filter,
    /* [in] */ List<AutoPtr<IResolveInfo>*>* dest)
{
//    ServiceInfo filterSi = filter.service.info;
//    for (int i=dest.size()-1; i>=0; i--) {
//        ServiceInfo destAi = dest.get(i).serviceInfo;
//        if (destAi.name == filterSi.name
//                && destAi.packageName == filterSi.packageName) {
//            return false;
//        }
//    }
    return TRUE;
}

//@Override
String CCapsuleManagerService::ServiceIntentResolver::CapsuleForFilter(
    /* [in] */ CapsuleParser::ServiceIntentInfo* info)
{
    return info->mService->mOwner->mCapsuleName;
}

ECode CCapsuleManagerService::ServiceIntentResolver::NewResult(
    /* [in] */ CapsuleParser::ServiceIntentInfo* info,
    /* [in] */ Int32 match,
    /* [out] */ AutoPtr<IResolveInfo>** res)
{
    assert(info);
    assert(res != NULL);
    // if (!mOwner->mSettings->IsEnabledLP(info->mService->mInfo, mFlags)) {
    //     *res = NULL;
    //     return NOERROR;
    // }
    CapsuleParser::Service* service = info->mService;
    AutoPtr<IApplicationInfo> srvAppInfo;
    service->mInfo->GetApplicationInfo((IApplicationInfo**)&srvAppInfo);
    Int32 srvFlags;
    srvAppInfo->GetFlags(&srvFlags);
    if (mOwner->mSafeMode && (srvFlags & ApplicationInfo_FLAG_SYSTEM) == 0) {
        *res = NULL;
        return NOERROR;
    }
    AutoPtr<IResolveInfo> rinfo;
    CResolveInfo::New((IResolveInfo**)&rinfo);
    rinfo->SetServiceInfo(CapsuleParser::GenerateServiceInfo(service, mFlags));
    if ((mFlags & CapsuleManager_GET_RESOLVED_FILTER) != 0) {
//	        res->mFilter = filter;
    }
    Int32 priority;
    info->GetPriority(&priority);
    rinfo->SetPriority(priority);
    rinfo->SetPreferredOrder(service->mOwner->mPreferredOrder);
    //System.out.println("Result: " + res.activityInfo.className +
    //                   " = " + res.priority);
    rinfo->SetMatch(match);
    rinfo->SetDefault(info->mHasDefault);
    rinfo->SetLabelRes(info->mLabelRes);
    rinfo->SetNonLocalizedLabel(info->mNonLocalizedLabel);
    rinfo->SetIcon(info->mIcon);
    *res = new AutoPtr<IResolveInfo>();
    **res = rinfo;
    return NOERROR;
}

void CCapsuleManagerService::ServiceIntentResolver::SortResults(
    /* [in] */ List<AutoPtr<IResolveInfo>*>* results)
{
//	    Collections.sort(results, mResolvePrioritySorter);
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::BasePermission

const Int32 CCapsuleManagerService::BasePermission::TYPE_NORMAL;
const Int32 CCapsuleManagerService::BasePermission::TYPE_BUILTIN;
const Int32 CCapsuleManagerService::BasePermission::TYPE_DYNAMIC;

String CCapsuleManagerService::BasePermission::ToString()
{
    return String((const char*)(StringBuffer("BasePermission{")
        + this /*Integer.toHexString(System.identityHashCode(this))*/
        + " " + mName + "}"));
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleSignatures

CCapsuleManagerService::CapsuleSignatures::CapsuleSignatures(
    /* [in] */ const CapsuleSignatures* orig)
{
    if (orig != NULL && orig->mSignatures != NULL) {
        Int32 size = orig->mSignatures->GetLength();
        mSignatures = ArrayOf< AutoPtr<ISignature> >::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            (*mSignatures.Get())[i] = (*orig->mSignatures.Get())[i];
        }
    }
}

CCapsuleManagerService::CapsuleSignatures::CapsuleSignatures(
    /* [in] */ const ArrayOf< AutoPtr<ISignature> >& sigs)
{
    AssignSignatures(sigs);
}

CCapsuleManagerService::CapsuleSignatures::CapsuleSignatures()
{
}

CCapsuleManagerService::CapsuleSignatures::~CapsuleSignatures()
{
}

PInterface CCapsuleManagerService::CapsuleSignatures::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::CapsuleSignatures::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::CapsuleSignatures::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::CapsuleSignatures::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

void CCapsuleManagerService::CapsuleSignatures::WriteXml(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ CString tagName,
    /* [in] */ List< AutoPtr<ISignature> >* pastSignatures)
{
    if (mSignatures == NULL) {
        return;
    }
    serializer->WriteStartTag(NULL, tagName);
    serializer->WriteAttribute(NULL, "count",
            String::FromInt32(mSignatures->GetLength()));
    for (Int32 i = 0; i < mSignatures->GetLength(); i++) {
        serializer->WriteStartTag(NULL, "cert");
        ISignature* sig = (*mSignatures.Get())[i];
        Int32 sigHash;
        sig->GetHashCode(&sigHash);
        Int32 j;
        List< AutoPtr<ISignature> >::Iterator it;
        for (it = pastSignatures->Begin(), j = 0;
             it != pastSignatures->End(); ++it, ++j) {
            ISignature* pastSig = *it;
            Int32 pastSigHash;
            pastSig->GetHashCode(&pastSigHash);
            Boolean isEqual;
            pastSig->Equals(sig, &isEqual);
            if (pastSigHash == sigHash && isEqual) {
                serializer->WriteAttribute(NULL, "index", String::FromInt32(j));
                break;
            }
        }
        if (it == pastSignatures->End()) {
            pastSignatures->PushBack(sig);
            serializer->WriteAttribute(NULL, "index", String::FromInt32(j));
            String chars;
            sig->ToCharsString(&chars);
            serializer->WriteAttribute(NULL, "key", chars);
        }
        serializer->WriteEndTag(NULL, "cert");
    }
    serializer->WriteEndTag(NULL, tagName);
}

void CCapsuleManagerService::CapsuleSignatures::ReadXml(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ List< AutoPtr<ISignature> >* pastSignatures)
{
    String countStr;
    parser->GetAttributeValueEx(NULL, "count", &countStr);
    if (countStr.IsNull()) {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: <signatures> has"
//                   + " no count at " + parser.getPositionDescription());
        XmlUtils::SkipCurrentTag(parser);
    }
    Int32 count = countStr.ToInt32();
    //todo: maybe have memory leak
    mSignatures = ArrayOf< AutoPtr<ISignature> >::Alloc(count);
    Int32 pos = 0;

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type, depth;
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("cert")) {
            if (pos < count) {
                String index;
                parser->GetAttributeValueEx(NULL, "index", &index);
                if (!index.IsNull()) {
//                    try {
                        Int32 idx = index.ToInt32();
                        String key;
                        parser->GetAttributeValueEx(NULL, "key", &key);
                        if (key.IsNull()) {
                            if (idx >= 0) {
                                List< AutoPtr<ISignature> >::Iterator it =
                                    pastSignatures->Begin();
                                for (Int32 i = 0; it != pastSignatures->End(), i <= idx; ++it, ++i) {}
                                if (it != pastSignatures->End()) {
                                    ISignature* sig = *it;
                                    if (sig != NULL) {
                                        (*mSignatures)[pos] = sig;
                                        pos++;
                                    }
                                    else {
//                                        reportSettingsProblem(Log.WARN,
//                                                "Error in package manager settings: <cert> "
//                                                   + "index " + index + " is not defined at "
//                                                   + parser.getPositionDescription());
                                    }
                                }
                            }
                            else {
//                                reportSettingsProblem(Log.WARN,
//                                        "Error in package manager settings: <cert> "
//                                           + "index " + index + " is out of bounds at "
//                                           + parser.getPositionDescription());
                            }
                        }
                        else {
                            Int32 size = pastSignatures->GetSize();
                            for (Int32 i = 0; i < idx - size; ++i) {
                                pastSignatures->PushBack(NULL);
                            }
                            AutoPtr<ISignature> sig;
                            CSignature::New(key, (ISignature**)&sig);
                            pastSignatures->PushBack(sig);
                            (*mSignatures)[pos] = sig;
                            pos++;
                        }
//                    } catch (NumberFormatException e) {
//                        reportSettingsProblem(Log.WARN,
//                                "Error in package manager settings: <cert> "
//                                   + "index " + index + " is not a number at "
//                                   + parser.getPositionDescription());
//                    }
                }
                else {
//                    reportSettingsProblem(Log.WARN,
//                            "Error in package manager settings: <cert> has"
//                               + " no index at " + parser.getPositionDescription());
                }
            }
            else {
//                reportSettingsProblem(Log.WARN,
//                        "Error in package manager settings: too "
//                           + "many <cert> tags, expected " + count
//                           + " at " + parser.getPositionDescription());
            }
        }
        else {
//            reportSettingsProblem(Log.WARN,
//                    "Unknown element under <cert>: "
//                    + parser.getName());
        }
        XmlUtils::SkipCurrentTag(parser);
    }

    if (pos < count) {
        // Should never happen -- there is an error in the written
        // settings -- but if it does we don't want to generate
        // a bad array.
        ArrayOf< AutoPtr<ISignature> >* newSigs = ArrayOf< AutoPtr<ISignature> >::Alloc(pos);
        memcpy(newSigs->GetPayload(), mSignatures->GetPayload(), pos * sizeof(AutoPtr<ISignature>));
        ArrayOf< AutoPtr<ISignature> >::Free(mSignatures);
        mSignatures = newSigs;
    }
}

void CCapsuleManagerService::CapsuleSignatures::AssignSignatures(
    /* [in] */ const ArrayOf< AutoPtr<ISignature> >& sigs)
{
    Int32 size = sigs.GetLength();
    if (size == 0) {
        mSignatures = NULL;
        return;
    }
    mSignatures = ArrayOf< AutoPtr<ISignature> >::Alloc(size);
    for (Int32 i = 0; i < size; ++i) {
        (*mSignatures.Get())[i] = sigs[i];
    }
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::PreferredActivity

CCapsuleManagerService::PreferredActivity::PreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ IObjectContainer* set, /*IComponentName*/
    /* [in] */ IComponentName* activity)
    : IntentFilter(filter)
{
    mMatch = match & IntentFilter_MATCH_CATEGORY_MASK;
    mActivity = activity;
    activity->FlattenToShortString(&mShortActivity);
    if (set != NULL) {
//        Int32 N = set->GetLength();
//        ArrayOf<String>* myCapsules = ArrayOf<String>::Alloc(N);
//        ArrayOf<String>* myClasses = ArrayOf<String>::Alloc(N);
//        ArrayOf<String>* myComponents = ArrayOf<String>::Alloc(N);
//        for (Int32 i = 0; i < N; i++) {
//            IComponentName* cn = (*set)[i];
//            if (cn == NULL) {
//                mSetCapsules = NULL;
//                mSetClasses = NULL;
//                mSetComponents = NULL;
//                //todo: memory leak;
//                return;
//            }
//            cn->GetCapsuleName(&(*myCapsules)[i]);
//            cn->GetClassName(&(*myClasses)[i]);
//            cn->FlattenToShortString(&(*myComponents)[i]);
//        }
//        mSetCapsules = myCapsules;
//        mSetClasses = myClasses;
//        mSetComponents = myComponents;
    }
    else {
        mSetCapsules = NULL;
        mSetClasses = NULL;
        mSetComponents = NULL;
    }
}

CCapsuleManagerService::PreferredActivity::PreferredActivity(
    /* [in] */ IXmlPullParser* parser)
{
    parser->GetAttributeValueEx(NULL, "name", &mShortActivity);
    AutoPtr<IComponentNameHelper> helper;
    CComponentNameHelper::AcquireSingleton((IComponentNameHelper**)&helper);
    helper->UnflattenFromString(mShortActivity, (IComponentName**)&mActivity);
    if (mActivity == NULL) {
        mParseError = String("Bad activity name ") + mShortActivity;
    }
    String matchStr;
    parser->GetAttributeValueEx(NULL, "match", &matchStr);
    mMatch = !matchStr.IsNull() ? matchStr.ToInt32()/*Integer.parseInt(matchStr, 16)*/ : 0;
    String setCountStr;
    parser->GetAttributeValueEx(NULL, "set", &setCountStr);
    Int32 setCount = !setCountStr.IsNull() ? setCountStr.ToInt32() : 0;

    ArrayOf<String>* myCapsules = setCount > 0 ? ArrayOf<String>::Alloc(setCount) : NULL;
    ArrayOf<String>* myClasses = setCount > 0 ? ArrayOf<String>::Alloc(setCount) : NULL;
    ArrayOf<String>* myComponents = setCount > 0 ? ArrayOf<String>::Alloc(setCount) : NULL;

    Int32 setPos = 0;

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type, depth;
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        //Log.i(TAG, "Parse outerDepth=" + outerDepth + " depth="
        //        + parser.getDepth() + " tag=" + tagName);
        if (tagName.Equals("set")) {
            String name;
            parser->GetAttributeValueEx(NULL, "name", &name);
            if (name.IsNull()) {
                if (mParseError.IsNull()) {
                    mParseError = "No name in set tag in preferred activity "
                        + mShortActivity;
                }
            }
            else if (setPos >= setCount) {
                if (mParseError.IsNull()) {
                    mParseError = "Too many set tags in preferred activity "
                        + mShortActivity;
                }
            }
            else {
                AutoPtr<IComponentName> cn;
                helper->UnflattenFromString(name, (IComponentName**)&cn);
                if (cn == NULL) {
                    if (mParseError.IsNull()) {
                        mParseError = "Bad set name " + name + " in preferred activity "
                            + mShortActivity;
                    }
                }
                else {
                    cn->GetCapsuleName(&(*myCapsules)[setPos]);
                    cn->GetClassName(&(*myClasses)[setPos]);
                    (*myComponents)[setPos] = name;
                    setPos++;
                }
            }
            XmlUtils::SkipCurrentTag(parser);
        }
        else if (tagName.Equals("filter")) {
            //Log.i(TAG, "Starting to parse filter...");
            IntentFilter::ReadFromXml(parser);
            //Log.i(TAG, "Finished filter: outerDepth=" + outerDepth + " depth="
            //        + parser.getDepth() + " tag=" + parser.getName());
        }
        else {
//            reportSettingsProblem(Log.WARN,
//                    "Unknown element under <preferred-activities>: "
//                    + parser.getName());
            XmlUtils::SkipCurrentTag(parser);
        }
    }

    if (setPos != setCount) {
        if (mParseError.IsNull()) {
            mParseError = StringBuffer("Not enough set tags (expected ") + setCount
                + " but found " + setPos + ") in " + mShortActivity;
        }
    }

    mSetCapsules = myCapsules;
    mSetClasses = myClasses;
    mSetComponents = myComponents;
}

PInterface CCapsuleManagerService::PreferredActivity::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IIntentFilter*)this;
    }
    else if (riid == EIID_IIntentFilter) {
        return (IIntentFilter*)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::PreferredActivity::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::PreferredActivity::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::PreferredActivity::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IIntentFilter*)this) {
        *pIID = EIID_IIntentFilter;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::SetPriority(
    /* [in] */ Int32 priority)
{
    return IntentFilter::SetPriority(priority);
}

ECode CCapsuleManagerService::PreferredActivity::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    *priority = IntentFilter::GetPriority();

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::AddAction(
    /* [in] */ const String& action)
{
    return IntentFilter::AddAction(action);
}

ECode CCapsuleManagerService::PreferredActivity::CountActions(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = IntentFilter::CountActions();

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetAction(
    /* [in] */ Int32 index,
    /* [out] */ String *action)
{
    VALIDATE_NOT_NULL(action);
    *action = IntentFilter::GetAction(index);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::HasAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* hasAction)
{
    VALIDATE_NOT_NULL(hasAction);
    *hasAction = IntentFilter::HasAction(action);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::MatchAction(
    /* [in] */ const String& action,
    /* [out] */ Boolean* isMatched)
{
    VALIDATE_NOT_NULL(isMatched);
    *isMatched = IntentFilter::MatchAction(action);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetActions(
    /* [out, callee] */ ArrayOf<String>** actions)
{
    VALIDATE_NOT_NULL(actions);
    return IntentFilter::GetActions(actions);
}

ECode CCapsuleManagerService::PreferredActivity::AddDataType(
    /* [in] */ const String& type)
{
    return IntentFilter::AddDataType(type);
}

ECode CCapsuleManagerService::PreferredActivity::HasDataType(
    /* [in] */ const String& type,
    /* [out] */ Boolean* hasDataType)
{
    VALIDATE_NOT_NULL(hasDataType);
    *hasDataType = IntentFilter::HasDataType(type);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::CountDataTypes(
    /* [out] */ Int32 *count)
{
    VALIDATE_NOT_NULL(count);
    *count = IntentFilter::CountDataTypes();

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetDataType(
    /* [in] */ Int32 index,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    *type = IntentFilter::GetDataType(index);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetTypes(
    /* [out, callee] */ ArrayOf<String>** types)
{
    VALIDATE_NOT_NULL(types);
    return IntentFilter::GetTypes(types);
}

ECode CCapsuleManagerService::PreferredActivity::AddDataScheme(
    /* [in] */ const String& scheme)
{
    return IntentFilter::AddDataScheme(scheme);
}

ECode CCapsuleManagerService::PreferredActivity::CountDataSchemes(
    /* [out] */ Int32 *count)
{
    VALIDATE_NOT_NULL(count);
    *count = IntentFilter::CountDataSchemes();

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetDataScheme(
    /* [in] */ Int32 index,
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
    *scheme = IntentFilter::GetDataScheme(index);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::HasDataScheme(
    /* [in] */ const String& scheme,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = IntentFilter::HasDataScheme(scheme);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetSchemes(
    /* [out, callee] */ ArrayOf<String>** schemes)
{
    VALIDATE_NOT_NULL(schemes);
    return IntentFilter::GetSchemes(schemes);
}

ECode CCapsuleManagerService::PreferredActivity::AddDataAuthority(
    /* [in] */ const String& host,
    /* [in] */ const String& port)
{
    return IntentFilter::AddDataAuthority(host, port);
}

ECode CCapsuleManagerService::PreferredActivity::CountDataAuthorities(
    /* [out] */ Int32 *count)
{
    VALIDATE_NOT_NULL(count);
    *count = IntentFilter::CountDataAuthorities();

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetDataAuthority(
    /* [in] */ Int32 index,
    /* [out] */ IAuthorityEntry** authority)
{
    VALIDATE_NOT_NULL(authority);

    AutoPtr<IAuthorityEntry> tmp = IntentFilter::GetDataAuthority(index);
    *authority = tmp.Get();
    if (*authority != NULL) (*authority)->AddRef();
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::HasDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasDataAuthority(data);
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::AddDataPath(
	/* [in] */ const String& path,
	/* [in] */ Int32 type)
{
	return IntentFilter::AddDataPath(path, type);
}

ECode CCapsuleManagerService::PreferredActivity::CountDataPaths(
    /* [out] */ Int32 *count)
{
    VALIDATE_NOT_NULL(count);
    *count = IntentFilter::CountDataPaths();

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetDataPath(
    /* [in] */ Int32 index,
    /* [out] */ IPatternMatcher** path)
{
    VALIDATE_NOT_NULL(path);

    AutoPtr<IPatternMatcher> tmp = IntentFilter::GetDataPath(index);
    *path = tmp.Get();
    if (*path != NULL) (*path)->AddRef();
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::HasDataPath(
    /* [in] */ const String& data,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::HasDataPath(data);
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::MatchDataAuthority(
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchDataAuthority(data);
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::MatchData(
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchData(type, scheme, data);
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::AddCategory(
    /* [in] */ const String& category)
{
    return IntentFilter::AddCategory(category);
}

ECode CCapsuleManagerService::PreferredActivity::CountCategories(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = IntentFilter::CountCategories();

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::GetCategory(
    /* [in] */ Int32 index,
    /* [out] */ String* category)
{
    VALIDATE_NOT_NULL(category);
    *category = IntentFilter::GetCategory(index);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::HasCategory(
    /* [in] */ const String& category,
    /* [out] */ Boolean* hasCategory)
{
    VALIDATE_NOT_NULL(hasCategory);
    *hasCategory = IntentFilter::HasCategory(category);

    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::MatchCategories(
    /* [in] */ ArrayOf<String>* categories,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::MatchCategories(categories);
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::Match(
    /* [in] */ const String& action,
    /* [in] */ const String& type,
    /* [in] */ const String& scheme,
    /* [in] */ IUri* data,
    /* [in] */ ArrayOf<String>* categories,
    /* [in] */ CString logTag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::Match(action, type, scheme, data, categories, logTag);
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::MatchEx(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean resolve,
    /* [in] */ CString logTag,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IntentFilter::Match(resolver, intent, resolve, logTag);
    return NOERROR;
}

ECode CCapsuleManagerService::PreferredActivity::WriteToXml(
    /* [in] */ IXmlSerializer* serializer)
{
    const Int32 NS = mSetClasses != NULL ? mSetClasses->GetLength() : 0;
    FAIL_RETURN(serializer->WriteAttribute(NULL, "name", mShortActivity));
    FAIL_RETURN(serializer->WriteAttribute(NULL, "match", String::FromInt32(mMatch)/*Integer.toHexString(mMatch)*/));
    FAIL_RETURN(serializer->WriteAttribute(NULL, "set", String::FromInt32(NS)));
    for (Int32 s = 0; s < NS; s++) {
        FAIL_RETURN(serializer->WriteStartTag(NULL, "set"));
        FAIL_RETURN(serializer->WriteAttribute(NULL, "name", (*mSetComponents)[s]));
        FAIL_RETURN(serializer->WriteEndTag(NULL, "set"));
    }
    FAIL_RETURN(serializer->WriteStartTag(NULL, "filter"));
    FAIL_RETURN(IntentFilter::WriteToXml(serializer));
    return serializer->WriteEndTag(NULL, "filter");
}

ECode CCapsuleManagerService::PreferredActivity::ReadFromXml(
    /* [in] */ IXmlPullParser* parser)
{
    return IntentFilter::ReadFromXml(parser);
}

ECode CCapsuleManagerService::PreferredActivity::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return IntentFilter::ReadFromParcel(source);
}

ECode CCapsuleManagerService::PreferredActivity::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return IntentFilter::WriteToParcel(dest);
}

Boolean CCapsuleManagerService::PreferredActivity::SameSet(
    /* [in] */ IObjectContainer* query,
    /* [in] */ Int32 priority)
{
    if (mSetCapsules == NULL) return FALSE;
    const Int32 NS = mSetCapsules->GetLength();
    Int32 numMatch = 0;
    AutoPtr<IObjectEnumerator> objEnu;
    query->GetObjectEnumerator((IObjectEnumerator**)&objEnu);
    Boolean isSucceeded;
    objEnu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IResolveInfo> ri;
        objEnu->Current((IInterface**)&ri);
        Int32 riPriority;
        ri->GetPriority(&riPriority);
        if (riPriority != priority) {
            objEnu->MoveNext(&isSucceeded);
        }
        AutoPtr<IActivityInfo> ai;
        ri->GetActivityInfo((IActivityInfo**)&ai);
        String aiName, aiCName;
        ai->GetName(&aiName);
        ai->GetCapsuleName(&aiCName);
        Boolean good = FALSE;
        for (Int32 j = 0; j < NS; j++) {
            if ((*mSetCapsules)[j].Equals(aiCName)
                    && (*mSetClasses)[j].Equals(aiName)) {
                numMatch++;
                good = TRUE;
                break;
            }
        }
        if (!good) return FALSE;

    }
    return numMatch == NS;
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::GrantedPermissions

CCapsuleManagerService::GrantedPermissions::GrantedPermissions(
    /* [in] */ Int32 capFlags)
    : mGrantedPermissions(11)
{
    SetFlags(capFlags);
}

CCapsuleManagerService::GrantedPermissions::GrantedPermissions(
    /* [in] */ const GrantedPermissions* base)
    : mGrantedPermissions(11)
{
    assert(base != NULL);
    mCapFlags = base->mCapFlags;

    HashSet<String>::Iterator it;
    for (it = base->mGrantedPermissions.Begin();
         it != base->mGrantedPermissions.End(); it++) {
        mGrantedPermissions.Insert(*it);
    }

    if (base->mGids != NULL) {
        mGids = base->mGids->Clone();
    }
}

CCapsuleManagerService::GrantedPermissions::~GrantedPermissions()
{
    mGrantedPermissions.Clear();
}

PInterface CCapsuleManagerService::GrantedPermissions::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    return NULL;
}

UInt32 CCapsuleManagerService::GrantedPermissions::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCapsuleManagerService::GrantedPermissions::Release()
{
    return ElRefBase::Release();
}

ECode CCapsuleManagerService::GrantedPermissions::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

void CCapsuleManagerService::GrantedPermissions::SetFlags(
    /* [in] */ Int32 capFlags)
{
    mCapFlags = capFlags & (
        ApplicationInfo_FLAG_SYSTEM |
        ApplicationInfo_FLAG_FORWARD_LOCK |
        ApplicationInfo_FLAG_EXTERNAL_STORAGE |
        ApplicationInfo_FLAG_NEVER_ENCRYPT);
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleSettingBase

CCapsuleManagerService::CapsuleSettingBase::CapsuleSettingBase(
    /* [in] */ const String& name,
    /* [in] */ const String& realName,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 cVersionCode,
    /* [in] */ Int32 capFlags)
    : GrantedPermissions(capFlags)
    , mName(name)
    , mRealName(realName)
    , mTimeStamp(0)
    , mFirstInstallTime(0)
    , mLastUpdateTime(0)
    , mVersionCode(0)
    , mUidError(FALSE)
    , mSignatures(new CapsuleSignatures())
    , mPermissionsFixed(FALSE)
    , mHaveGids(FALSE)
    , mDisabledComponents(0)
    , mEnabledComponents(0)
    , mEnabled(CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT)
    , mInstallStatus(CCapsuleManagerService::CAP_INSTALL_COMPLETE)
    , mOrigCapsule(NULL)
{
    Init(codePath, resourcePath, nativeLibraryPathString, cVersionCode);
}

CCapsuleManagerService::CapsuleSettingBase::CapsuleSettingBase(
    /* [in] */ const CapsuleSettingBase* base)
    : GrantedPermissions(base)
    , mName(base->mName)
    , mRealName(base->mRealName)
    , mCodePath(base->mCodePath)
    , mCodePathString(base->mCodePathString)
    , mResourcePath(base->mResourcePath)
    , mResourcePathString(base->mResourcePathString)
    , mNativeLibraryPathString(base->mNativeLibraryPathString)
    , mTimeStamp(base->mTimeStamp)
    , mFirstInstallTime(base->mFirstInstallTime)
    , mLastUpdateTime(base->mLastUpdateTime)
    , mVersionCode(base->mVersionCode)
    , mUidError(base->mUidError)
    , mSignatures(new CapsuleSignatures(base->mSignatures))
    , mPermissionsFixed(base->mPermissionsFixed)
    , mHaveGids(base->mHaveGids)
    , mDisabledComponents(0)
    , mEnabledComponents(0)
    , mEnabled(base->mEnabled)
    , mInstallStatus(base->mInstallStatus)
    , mOrigCapsule(base->mOrigCapsule)
    , mInstallerCapsuleName(base->mInstallerCapsuleName)
{
    HashSet<String>::Iterator it;
    for (it = base->mDisabledComponents.Begin();
         it != base->mDisabledComponents.End(); ++it) {
        mDisabledComponents.Insert(*it);
    }

    for (it = base->mEnabledComponents.Begin();
         it != base->mEnabledComponents.End(); ++it) {
        mEnabledComponents.Insert(*it);
    }
}

CCapsuleManagerService::CapsuleSettingBase::~CapsuleSettingBase()
{
    if (mSignatures != NULL) delete mSignatures;
    mDisabledComponents.Clear();
    mEnabledComponents.Clear();
}

void CCapsuleManagerService::CapsuleSettingBase::Init(
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 cVersionCode)
{
    mCodePath = codePath;
    codePath->ToString(&mCodePathString);
    mResourcePath = resourcePath;
    resourcePath->ToString(&mResourcePathString);
    mNativeLibraryPathString = nativeLibraryPathString;
    mVersionCode = cVersionCode;
}

void CCapsuleManagerService::CapsuleSettingBase::SetInstallerCapsuleName(
    /* [in] */ const String& capsuleName)
{
    mInstallerCapsuleName = capsuleName;
}

String CCapsuleManagerService::CapsuleSettingBase::GetInstallerCapsuleName()
{
    return mInstallerCapsuleName;
}

void CCapsuleManagerService::CapsuleSettingBase::SetInstallStatus(
    /* [in] */ Int32 newStatus)
{
    mInstallStatus = newStatus;
}

Int32 CCapsuleManagerService::CapsuleSettingBase::GetInstallStatus()
{
    return mInstallStatus;
}

void CCapsuleManagerService::CapsuleSettingBase::SetTimeStamp(
    /* [in] */ Int64 newStamp)
{
    mTimeStamp = newStamp;
}

void CCapsuleManagerService::CapsuleSettingBase::CopyFrom(
    /* [in] */ const CapsuleSettingBase* base)
{
    assert(base != NULL);
    mGrantedPermissions = base->mGrantedPermissions;
    mGids = base->mGids != NULL ? base->mGids->Clone() : NULL;

    mTimeStamp = base->mTimeStamp;
    mFirstInstallTime = base->mFirstInstallTime;
    mLastUpdateTime = base->mLastUpdateTime;
    mSignatures = base->mSignatures;
    mPermissionsFixed = base->mPermissionsFixed;
    mHaveGids = base->mHaveGids;
    mDisabledComponents = base->mDisabledComponents;
    mEnabledComponents = base->mEnabledComponents;
    mEnabled = base->mEnabled;
    mInstallStatus = base->mInstallStatus;
}

Boolean CCapsuleManagerService::CapsuleSettingBase::EnableComponentLP(
    /* [in] */ const String& componentClassName)
{
    Boolean changed = FALSE;
    HashSet<String>::Iterator it;

    it = mDisabledComponents.Find(componentClassName);
    if (it != mDisabledComponents.End()) {
        mDisabledComponents.Erase(it);
        changed = TRUE;
    }

    it = mEnabledComponents.Find(componentClassName);
    if (it == mEnabledComponents.End()) {
        mEnabledComponents.Insert(componentClassName);
        changed |= TRUE;
    }

    return changed;
}

Boolean CCapsuleManagerService::CapsuleSettingBase::DisableComponentLP(
    /* [in] */ const String& componentClassName)
{
    Boolean changed = FALSE;
    HashSet<String>::Iterator it;

    it = mEnabledComponents.Find(componentClassName);
    if (it != mEnabledComponents.End()) {
        mEnabledComponents.Erase(it);
        changed = TRUE;
    }

    it = mDisabledComponents.Find(componentClassName);
    if (it == mDisabledComponents.End()) {
        mDisabledComponents.Insert(componentClassName);
        changed |= TRUE;
    }

    return changed;
}

Boolean CCapsuleManagerService::CapsuleSettingBase::RestoreComponentLP(
    /* [in] */ const String& componentClassName)
{
    Boolean changed = FALSE;
    HashSet<String>::Iterator it;

    it = mEnabledComponents.Find(componentClassName);
    if (it != mEnabledComponents.End()) {
        mEnabledComponents.Erase(it);
        changed = TRUE;
    }

    it = mDisabledComponents.Find(componentClassName);
    if (it != mDisabledComponents.End()) {
        mDisabledComponents.Erase(it);
        changed |= TRUE;
    }

    return changed;
}

Int32 CCapsuleManagerService::CapsuleSettingBase::CurrentEnabledStateLP(
    /* [in] */ const String& componentName)
{
    HashSet<String>::Iterator it;

    it = mEnabledComponents.Find(componentName);
    if (it != mEnabledComponents.End()) {
        return CapsuleManager_COMPONENT_ENABLED_STATE_ENABLED;
    }

    it = mDisabledComponents.Find(componentName);
    if (it != mDisabledComponents.End()) {
        return CapsuleManager_COMPONENT_ENABLED_STATE_DISABLED;
    }

    return CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT;
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::CapsuleSetting

// {53EC2E67-63D3-4EE4-BD7B-CCEE871CAD6C}
extern "C" const InterfaceID EIID_CapsuleSetting =
        { 0x53ec2e67, 0x63d3, 0x4ee4, { 0xbd, 0x7b, 0xcc, 0xee, 0x87, 0x1c, 0xad, 0x6c } };

CCapsuleManagerService::CapsuleSetting::CapsuleSetting(
    /* [in] */ const String& name,
    /* [in] */ const String& realName,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 pVersionCode,
    /* [in] */ Int32 capFlags)
    : CapsuleSettingBase(name, realName, codePath,
        resourcePath, nativeLibraryPathString, pVersionCode, capFlags)
    , mUserId(0)
    , mCap(NULL)
    , mSharedUser(NULL)
{}

CCapsuleManagerService::CapsuleSetting::CapsuleSetting(
    /* [in] */ const CapsuleSetting* orig)
    : CapsuleSettingBase(orig)
    , mUserId(orig->mUserId)
    , mCap(orig->mCap)
    , mSharedUser(orig->mSharedUser)
{}

CCapsuleManagerService::CapsuleSetting::~CapsuleSetting()
{}

PInterface CCapsuleManagerService::CapsuleSetting::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_CapsuleSetting) {
        return (PInterface)this;
    }
    else return CapsuleSettingBase::Probe(riid);
}

String CCapsuleManagerService::CapsuleSetting::ToString()
{
    return String((const char*)(StringBuffer("CapsuleSetting{")
        + this /*Integer.toHexString(System.identityHashCode(this))*/
        + " " + mName + "/" + mUserId + "}"));
}

Int32 CCapsuleManagerService::CapsuleSetting::GetHashCode() const
{
    return (Int32)this;
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::SharedUserSetting

// {93FF1F91-2CDD-4EFD-B8D9-330D397764EF}
extern "C" const InterfaceID EIID_SharedUserSetting =
        { 0x93ff1f91, 0x2cdd, 0x4efd, { 0xb8, 0xd9, 0x33, 0xd, 0x39, 0x77, 0x64, 0xef } };

CCapsuleManagerService::SharedUserSetting::SharedUserSetting(
    /* [in] */ const String& name,
    /* [in] */ Int32 capFlags)
    : GrantedPermissions(capFlags)
    , mName(name)
    , mUserId(0)
    , mCapsules(5)
    , mSignatures(new CapsuleSignatures())
{}

CCapsuleManagerService::SharedUserSetting::~SharedUserSetting()
{
    mCapsules.Clear();
}

PInterface CCapsuleManagerService::SharedUserSetting::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_SharedUserSetting) {
        return (PInterface)this;
    }
    else return GrantedPermissions::Probe(riid);
}

String CCapsuleManagerService::SharedUserSetting::ToString()
{
    return String((const char*)(StringBuffer("SharedUserSetting{")
        + this /*Integer.toHexString(System.identityHashCode(this))*/
        + " " + mName + "/" + mUserId + "}"));
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::Settings::SettingsIntentResolver

String CCapsuleManagerService::Settings::SettingsIntentResolver::CapsuleForFilter(
    /* [in] */ PreferredActivity* filter)
{
    String name;
    filter->mActivity->GetCapsuleName(&name);
    return name;
}

///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService::Settings

CCapsuleManagerService::Settings::Settings(
    /* [in] */ CCapsuleManagerService* owner)
    : mCapsules(5)
    , mDisabledSysCapsules(5)
    , mInternalSdkPlatform(0)
    , mExternalSdkPlatform(0)
    , mPreferredActivities(new SettingsIntentResolver())
    , mSharedUsers(5)
    , mUserIds(10)
    , mOtherUserIds(10)
    , mPermissions(5)
    , mPermissionTrees(5)
    , mRenamedCapsules(5)
    , mPendingCapsules(5)
    , mOwner(owner)
{
    AutoPtr<IFile> dataDir = Environment::GetDataDirectory();
    AutoPtr<IFile> systemDir;
    CFile::New(dataDir, String("system"), (IFile**)&systemDir);
    // TODO(oam): This secure dir creation needs to be moved somewhere else (later)
    AutoPtr<IFile> systemSecureDir;
    CFile::New(dataDir, String("secure/system"), (IFile**)&systemSecureDir);
    Boolean isSucceeded = FALSE;
    systemDir->Mkdirs(&isSucceeded);
    systemSecureDir->Mkdirs(&isSucceeded);
    String des;
    systemDir->ToString(&des);
    FileUtils::SetPermissions(des,
        FileUtils::IRWXU | FileUtils::IRWXG
        | FileUtils::IROTH | FileUtils::IXOTH,
        -1, -1);
    systemSecureDir->ToString(&des);
    FileUtils::SetPermissions(des,
        FileUtils::IRWXU | FileUtils::IRWXG
        | FileUtils::IROTH | FileUtils::IXOTH,
        -1, -1);
    CFile::New(systemDir, String("capsules.xml"), (IFile**)&mSettingsFilename);
    CFile::New(systemDir, String("capsules-backup.xml"), (IFile**)&mBackupSettingsFilename);
    CFile::New(systemDir, String("capsules.list"), (IFile**)&mCapsuleListFilename);
}

CCapsuleManagerService::Settings::~Settings()
{
    //todo:
}

AutoPtr<CCapsuleManagerService::CapsuleSetting>
CCapsuleManagerService::Settings::GetCapsuleLP(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ CapsuleSetting* origCapsule,
    /* [in] */ const String& realName,
    /* [in] */ SharedUserSetting* sharedUser,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 capFlags,
    /* [in] */ Boolean create,
    /* [in] */ Boolean add)
{
    return GetCapsuleLP(cap->mCapsuleName, origCapsule, realName, sharedUser, codePath,
        resourcePath, nativeLibraryPathString, cap->mVersionCode, capFlags, create, add);
}

AutoPtr<CCapsuleManagerService::CapsuleSetting>
CCapsuleManagerService::Settings::PeekCapsuleLP(
    /* [in] */ const String& name)
{
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(name);
    if (it != mCapsules.End()) {
        return it->mSecond;
    }
    else return NULL;
    /*
    PackageSetting p = mPackages.get(name);
    if (p != null && p.codePath.getPath().equals(codePath)) {
        return p;
    }
    return null;
    */
}

void CCapsuleManagerService::Settings::SetInstallStatus(
    /* [in] */ String capName,
    /* [in] */ Int32 status)
{
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        CapsuleSetting* p = it->mSecond;
        if(p->GetInstallStatus() != status) {
            p->SetInstallStatus(status);
        }
    }
}

void CCapsuleManagerService::Settings::SetInstallerCapsuleName(
    /* [in] */ String capName,
    /* [in] */ String installerCapName)
{
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        CapsuleSetting* p = it->mSecond;
        p->SetInstallerCapsuleName(installerCapName);
    }
}

String CCapsuleManagerService::Settings::GetInstallerCapsuleName(
    /* [in] */ String capName)
{
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(capName);
    return it == mCapsules.End() ? String(NULL) : it->mSecond->GetInstallerCapsuleName();
}

Int32 CCapsuleManagerService::Settings::GetInstallStatus(
    /* [in] */ String capName)
{
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        CapsuleSetting* p = it->mSecond;
        return p->GetInstallStatus();
    }
    return -1;
}

AutoPtr<CCapsuleManagerService::SharedUserSetting>
CCapsuleManagerService::Settings::GetSharedUserLP(
    /* [in] */ const String& name,
    /* [in] */ Int32 capFlags,
    /* [in] */ Boolean create)
{
    AutoPtr<SharedUserSetting> s;
    HashMap<String, AutoPtr<SharedUserSetting> >::Iterator it = mSharedUsers.Find(name);
    if (it != mSharedUsers.End()) {
        s = it->mSecond;
    }
    if (s == NULL) {
        if (!create) {
            return NULL;
        }
        s = new SharedUserSetting(name, capFlags);
        if (MULTIPLE_APPLICATION_UIDS) {
            s->mUserId = NewUserIdLP(s);
        }
        else {
            s->mUserId = FIRST_APPLICATION_UID;
        }
        Logger::I(TAG, "New shared user " + name + ": id=" + s->mUserId);
        // < 0 means we couldn't assign a userid; fall out and return
        // s, which is currently null
        if (s->mUserId >= 0) {
            mSharedUsers[name] = s;
        }
    }

    return s;
}

Boolean CCapsuleManagerService::Settings::DisableSystemCapsuleLP(
    /* [in] */ String name)
{
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it;

    CapsuleSetting* p = NULL;
    it = mCapsules.Find(name);
    if (it != mCapsules.End()) {
        p = it->mSecond;
    }
    if(p == NULL) {
        Logger::W(TAG, StringBuffer("Capsule:") + name + " is not an installed capsule");
        return FALSE;
    }
    CapsuleSetting* dp = NULL;
    it = mDisabledSysCapsules.Find(name);
    if (it != mDisabledSysCapsules.End()) {
        dp = it->mSecond;
    }
    // always make sure the system package code and resource paths dont change
    if (dp == NULL) {
        if((p->mCap != NULL) && (p->mCap->mApplicationInfo != NULL)) {
            Int32 pFlags;
            p->mCap->mApplicationInfo->GetFlags(&pFlags);
            pFlags |= ApplicationInfo_FLAG_UPDATED_SYSTEM_APP;
            p->mCap->mApplicationInfo->SetFlags(pFlags);
        }
        mDisabledSysCapsules[name] = p;

        // a little trick...  when we install the new package, we don't
        // want to modify the existing PackageSetting for the built-in
        // version.  so at this point we need a new PackageSetting that
        // is okay to muck with.
        AutoPtr<CapsuleSetting> newp = new CapsuleSetting(p);
        ReplaceCapsuleLP(name, newp);
        return TRUE;
    }
    return FALSE;
}

AutoPtr<CCapsuleManagerService::CapsuleSetting>
CCapsuleManagerService::Settings::EnableSystemCapsuleLP(
    /* [in] */ const String& name)
{
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it;

    CapsuleSetting* p = NULL;
    it = mDisabledSysCapsules.Find(name);
    if (it != mDisabledSysCapsules.End()) {
        p = it->mSecond;
    }
    if (p == NULL) {
        Logger::W(TAG, StringBuffer("Capsule:") + name + " is not disabled");
        return NULL;
    }
    // Reset flag in ApplicationInfo object
    if ((p->mCap != NULL) && (p->mCap->mApplicationInfo != NULL)) {
        Int32 pFlags;
        p->mCap->mApplicationInfo->GetFlags(&pFlags);
        pFlags &= ~ApplicationInfo_FLAG_UPDATED_SYSTEM_APP;
        p->mCap->mApplicationInfo->SetFlags(pFlags);
    }
    AutoPtr<CapsuleSetting> ret = AddCapsuleLP(
        name, p->mRealName, p->mCodePath, p->mResourcePath,
        p->mNativeLibraryPathString, p->mUserId, p->mVersionCode, p->mCapFlags);
    mDisabledSysCapsules.Erase(it);
    return ret;
}

AutoPtr<CCapsuleManagerService::CapsuleSetting>
CCapsuleManagerService::Settings::AddCapsuleLP(
    /* [in] */ const String& name,
    /* [in] */ const String& realName,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 vc,
    /* [in] */ Int32 capFlags)
{
    AutoPtr<CapsuleSetting> p;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(name);
    if (it != mCapsules.End()) {
        p = it->mSecond;
    }
    if (p != NULL) {
        if (p->mUserId == uid) {
            return p;
        }
        mOwner->ReportSettingsProblem(6 /*Log::ERROR*/,
            String("Adding duplicate capsule, keeping first: ") + name);
        return NULL;
    }
    p = new CapsuleSetting(name, realName,
        codePath, resourcePath, nativeLibraryPathString, vc, capFlags);
    p->mUserId = uid;
    AutoPtr<ICharSequence> nameWrapper;
    CStringWrapper::New(name, (ICharSequence**)&nameWrapper);
    if (AddUserIdLP(uid, p.Get(), nameWrapper.Get())) {
        mCapsules[name] = p;
        return p;
    }
    return NULL;
}

AutoPtr<CCapsuleManagerService::SharedUserSetting>
CCapsuleManagerService::Settings::AddSharedUserLP(
    /* [in] */ const String& name,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 capFlags)
{
    AutoPtr<SharedUserSetting> s;
    HashMap<String, AutoPtr<SharedUserSetting> >::Iterator it = mSharedUsers.Find(name);
    if (it != mSharedUsers.End()) {
        s = it->mSecond;
    }
    if (s != NULL) {
        if (s->mUserId == uid) {
            return s;
        }
        mOwner->ReportSettingsProblem(6 /*Log::ERROR*/,
            String("Adding duplicate shared user, keeping first: ") + name);
        return NULL;
    }
    s = new SharedUserSetting(name, capFlags);
    s->mUserId = uid;
    AutoPtr<ICharSequence> nameWrapper;
    CStringWrapper::New(name, (ICharSequence**)&nameWrapper);
    if (AddUserIdLP(uid, s.Get(), nameWrapper.Get())) {
        mSharedUsers[name] = s;
        return s;
    }
    return NULL;
}

void CCapsuleManagerService::Settings::TransferPermissions(
    /* [in] */ const String& origCap,
    /* [in] */ const String& newCap)
{
    // Transfer ownership of permissions to the new package.
    for (Int32 i = 0; i < 2; i++) {
        HashMap<String, BasePermission*>& permissions =
                i == 0 ? mPermissionTrees : mPermissions;
        HashMap<String, BasePermission*>::Iterator it;
        for (it = permissions.Begin(); it != permissions.End(); it++) {
            BasePermission* bp = it->mSecond;
            if (origCap.Equals(bp->mSourceCapsule)) {
                if (DEBUG_UPGRADE) {
                    Logger::V(TAG, "Moving permission " + bp->mName
                        + " from cap " + bp->mSourceCapsule
                        + " to " + newCap);
                }
                bp->mSourceCapsule = newCap;
                bp->mCapsuleSetting = NULL;
                bp->mPerm = NULL;
                if (bp->mPendingInfo != NULL) {
                    bp->mPendingInfo->SetCapsuleName(newCap);
                }
                bp->mUid = 0;
                bp->mGids = NULL;
            }
        }
    }
}

AutoPtr<CCapsuleManagerService::CapsuleSetting>
CCapsuleManagerService::Settings::GetCapsuleLP(
    /* [in] */ const String& _name,
    /* [in] */ CapsuleSetting* origCapsule,
    /* [in] */ const String& realName,
    /* [in] */ SharedUserSetting* sharedUser,
    /* [in] */ IFile* codePath,
    /* [in] */ IFile* resourcePath,
    /* [in] */ const String& nativeLibraryPathString,
    /* [in] */ Int32 vc,
    /* [in] */ Int32 capFlags,
    /* [in] */ Boolean create,
    /* [in] */ Boolean add)
{
    String name = _name;
    AutoPtr<CapsuleSetting> c;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(name);
    if (it != mCapsules.End()) {
        c = it->mSecond;
    }
    if (c != NULL) {
        Boolean isEqual = FALSE;
        c->mCodePath->Equals(codePath, &isEqual);
        if (!isEqual) {
            // Check to see if its a disabled system app
            if ((c->mCapFlags & ApplicationInfo_FLAG_SYSTEM) != 0) {
                // This is an updated system app with versions in both system
                // and data partition. Just let the most recent version
                // take precedence.
                String str;
                codePath->ToString(&str);
                Slogger::W(TAG, StringBuffer("Trying to update system app code path from ")
                    + c->mCodePathString + " to " + str);
            }
            else {
                // Just a change in the code path is not an issue, but
                // let's log a message about it.
                Slogger::I(TAG, String("Capsule ") + name
                    + " codePath changed from " + c->mCodePath
                    + " to " + codePath + "; Retaining data and using new");
                /*
                 * Since we've changed paths, we need to prefer the new
                 * native library path over the one stored in the
                 * capsule settings since we might have moved from
                 * internal to external storage or vice versa->
                 */
                c->mNativeLibraryPathString = nativeLibraryPathString;
            }
        }
        if (c->mSharedUser.Get() != sharedUser) {
            mOwner->ReportSettingsProblem(5/*Log::WARN*/,
                StringBuffer("Capsule ") + name + " shared user changed from "
                + (c->mSharedUser != NULL ? c->mSharedUser->mName : String("<nothing>"))
                + " to "
                + (sharedUser != NULL ? sharedUser->mName : String("<nothing>"))
                + "; replacing with new");
            c = NULL;
        }
        else {
            if ((capFlags & ApplicationInfo_FLAG_SYSTEM) != 0) {
                // If what we are scanning is a system package, then
                // make it so, regardless of whether it was previously
                // installed only in the data partition.
                c->mCapFlags |= ApplicationInfo_FLAG_SYSTEM;
            }
        }
    }
    if (c == NULL) {
        // Create a new PackageSettings entry. this can end up here because
        // of code path mismatch or user id mismatch of an updated system partition
        if (!create) {
            return NULL;
        }
        if (origCapsule != NULL) {
            // We are consuming the data from an existing package.
            c = new CapsuleSetting(origCapsule->mName, name,
                codePath, resourcePath, nativeLibraryPathString, vc, capFlags);
            if (DEBUG_UPGRADE) {
                Logger::V(TAG, StringBuffer("Capsule ") + name
                    + " is adopting original capsule " + origCapsule->mName);
            }
            // Note that we will retain the new package's signature so
            // that we can keep its data.
            AutoPtr<CapsuleSignatures> s = c->mSignatures;
            c->CopyFrom(origCapsule);
            c->mSignatures = s;
            c->mSharedUser = origCapsule->mSharedUser;
            c->mUserId = origCapsule->mUserId;
            c->mOrigCapsule = origCapsule;
            mRenamedCapsules[name] = origCapsule->mName;
            name = origCapsule->mName;
            // Update new package state.
            Int64 timeStamp = 0;
            codePath->LastModified(&timeStamp);
            c->SetTimeStamp(timeStamp);
        }
        else {
            c = new CapsuleSetting(name, realName,
                codePath, resourcePath, nativeLibraryPathString, vc, capFlags);
            Int64 timeStamp = 0;
            codePath->LastModified(&timeStamp);
            c->SetTimeStamp(timeStamp);
            c->mSharedUser = sharedUser;
            if (sharedUser != NULL) {
                c->mUserId = sharedUser->mUserId;
            }
            else if (MULTIPLE_APPLICATION_UIDS) {
                // Clone the setting here for disabled system packages
                CapsuleSetting* dis = NULL;
                it = mDisabledSysCapsules.Find(name);
                if (it != mDisabledSysCapsules.End()) {
                    dis = it->mSecond;
                }
                if (dis != NULL) {
                    // For disabled packages a new setting is created
                    // from the existing user id. This still has to be
                    // added to list of user id's
                    // Copy signatures from previous setting
                    if (dis->mSignatures->mSignatures != NULL) {
                        //todo: maybe should call element's AddRef
                        c->mSignatures->mSignatures = dis->mSignatures->mSignatures->Clone();
                    }
                    c->mUserId = dis->mUserId;
                    // Clone permissions
                    c->mGrantedPermissions = dis->mGrantedPermissions;
                    // Clone component info
                    c->mDisabledComponents = dis->mDisabledComponents;
                    c->mEnabledComponents = dis->mEnabledComponents;
                    // Add new setting to list of user ids
                    AutoPtr<ICharSequence> nameWrapper;
                    CStringWrapper::New(name, (ICharSequence**)&nameWrapper);
                    AddUserIdLP(c->mUserId, c.Get(), nameWrapper.Get());
                }
                else {
                    // Assign new user id
                    c->mUserId = NewUserIdLP(c);
                }
            }
            else {
                c->mUserId = FIRST_APPLICATION_UID;
            }
        }
        if (c->mUserId < 0) {
            mOwner->ReportSettingsProblem(5/*Log::WARN*/,
                "Capsule " + name + " could not be assigned a valid uid");
            return NULL;
        }
        if (add) {
            // Finish adding new package by adding it and updating shared
            // user preferences
            AddCapsuleSettingLP(c, name, sharedUser);
        }
    }
    return c;
}

void CCapsuleManagerService::Settings::InsertCapsuleSettingLP(
    /* [in] */ CapsuleSetting* c,
    /* [in] */ CapsuleParser::Capsule* cap)
{
    c->mCap = cap;
    cap->mSetEnabled = c->mEnabled;
    String codePath;
    cap->mApplicationInfo->GetSourceDir(&codePath);
    String resourcePath;
    cap->mApplicationInfo->GetPublicSourceDir(&resourcePath);
    // Update code path if needed
    if (!codePath.EqualsIgnoreCase(c->mCodePathString)) {
        Slogger::W(TAG, StringBuffer("Code path for cap : ") + c->mCap->mCapsuleName
            + " changing from " + c->mCodePathString + " to " + codePath);
        CFile::New(codePath, (IFile**)&c->mCodePath);
        c->mCodePathString = codePath;
    }
    //Update resource path if needed
    if (!resourcePath.EqualsIgnoreCase(c->mResourcePathString)) {
        Slogger::W(TAG, "Resource path for cap : " + c->mCap->mCapsuleName
            + " changing from " + c->mResourcePathString + " to " + resourcePath);
        CFile::New(resourcePath, (IFile**)&c->mResourcePath);
        c->mResourcePathString = resourcePath;
    }
    // Update the native library path if needed
    String nativeLibraryPath;
    cap->mApplicationInfo->GetNativeLibraryDir(&nativeLibraryPath);
    if (!nativeLibraryPath.IsNull()
        && !nativeLibraryPath.EqualsIgnoreCase(c->mNativeLibraryPathString)) {
        c->mNativeLibraryPathString = nativeLibraryPath;
    }
    // Update version code if needed
     if (cap->mVersionCode != c->mVersionCode) {
        c->mVersionCode = cap->mVersionCode;
    }
     // Update signatures if needed.
     if (c->mSignatures->mSignatures == NULL) {
         c->mSignatures->AssignSignatures(*(cap->mSignatures));
     }
     // If this app defines a shared user id initialize
     // the shared user signatures as well.
     if (c->mSharedUser != NULL && c->mSharedUser->mSignatures->mSignatures == NULL) {
         c->mSharedUser->mSignatures->AssignSignatures(*(cap->mSignatures));
     }
     AddCapsuleSettingLP(c, cap->mCapsuleName, c->mSharedUser);
}

void CCapsuleManagerService::Settings::AddCapsuleSettingLP(
    /* [in] */ CapsuleSetting* c,
    /* [in] */ const String& name,
    /* [in] */ SharedUserSetting* sharedUser)
{
    assert(c);

    mCapsules[name] = c;
    if (sharedUser != NULL) {
        if (c->mSharedUser != NULL && c->mSharedUser.Get() != sharedUser) {
            ReportSettingsProblem(6 /*Log::ERROR*/,
                StringBuffer("Capsule ") + c->mName + " was user "
                + c->mSharedUser + " but is now " + sharedUser
                + "; I am not changing its files so it will probably fail!");
            c->mSharedUser->mCapsules.Erase(c);
        }
        else if (c->mUserId != sharedUser->mUserId) {
            ReportSettingsProblem(6 /*Log::ERROR*/,
                "Capsule " + c->mName + " was user id " + c->mUserId
                + " but is now user " + sharedUser
                + " with id " + sharedUser->mUserId
                + "; I am not changing its files so it will probably fail!");
        }

        sharedUser->mCapsules.Insert(c);
        c->mSharedUser = sharedUser;
        c->mUserId = sharedUser->mUserId;
    }
}

ECode CCapsuleManagerService::Settings::UpdateSharedUserPermsLP(
    /* [in] */ CapsuleSetting* deletedPs,
    /* [in] */ ArrayOf<Int32>* globalGids)
{
    if ((deletedPs == NULL) || (deletedPs->mCap == NULL)) {
        Slogger::I(TAG, "Trying to update info for NULL capsule. Just ignoring");
        return E_FAIL;
    }
    // No sharedUserId
    if (deletedPs->mSharedUser == NULL) {
        return E_FAIL;
    }
    SharedUserSetting* sus = deletedPs->mSharedUser;
    // Update permissions
    List<String>::Iterator it;
    for (it = deletedPs->mCap->mRequestedPermissions.Begin();
         it != deletedPs->mCap->mRequestedPermissions.End(); it++) {
        const String& eachPerm = *it;
        Boolean used = FALSE;
        if (sus->mGrantedPermissions.Find(eachPerm) ==
            sus->mGrantedPermissions.End()) {
            continue;
        }
        HashSet<AutoPtr<CapsuleSetting>, CapsuleSetting::HashKey>::Iterator csit;
        for (csit = sus->mCapsules.Begin();
             csit != sus->mCapsules.End(); csit++) {
            CapsuleSetting* cap = *csit;
            if (cap->mCap != NULL &&
                !cap->mCap->mCapsuleName.Equals(deletedPs->mCap->mCapsuleName) &&
                Find(cap->mCap->mRequestedPermissions.Begin(),
                    cap->mCap->mRequestedPermissions.End(), eachPerm)
                    != cap->mCap->mRequestedPermissions.End()) {
                used = TRUE;
                break;
            }
        }
        if (!used) {
            // can safely delete this permission from list
            sus->mGrantedPermissions.Erase(eachPerm);
        }
    }
    // Update gids
    ArrayOf<Int32>* newGids = globalGids;
    HashSet<String>::Iterator gpit;
    for (gpit = sus->mGrantedPermissions.Begin();
         gpit != sus->mGrantedPermissions.End(); gpit++) {
        BasePermission* bp = NULL;
        HashMap<String, BasePermission*>::Iterator pit = mPermissions.Find(*it);
        if (pit != mPermissions.End()) {
            bp = pit->mSecond;
        }
        if (bp != NULL) {
//            newGids = mOwner->AppendInts(newGids, bp->mGids.Get());
        }
    }
    //todo: memory leak
    sus->mGids = newGids->Clone();
    return NOERROR;
}

Int32 CCapsuleManagerService::Settings::RemoveCapsuleLP(
    /* [in] */ const String& name)
{
    AutoPtr<CapsuleSetting> c;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(name);
    if (it != mCapsules.End()) {
        c = it->mSecond;
    }
    if (c != NULL) {
        mCapsules.Erase(it);
        if (c->mSharedUser != NULL) {
            c->mSharedUser->mCapsules.Erase(c);
            if (c->mSharedUser->mCapsules.Begin() == c->mSharedUser->mCapsules.End()) {
                mSharedUsers.Erase(c->mSharedUser->mName);
                RemoveUserIdLP(c->mSharedUser->mUserId);
                return c->mSharedUser->mUserId;
            }
        }
        else {
            RemoveUserIdLP(c->mUserId);
            return c->mUserId;
        }
    }
    return -1;
}

void CCapsuleManagerService::Settings::ReplaceCapsuleLP(
    /* [in] */ const String& name,
    /* [in] */ CapsuleSetting* newp)
{
    CapsuleSetting* c;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(name);
    if (it != mCapsules.End()) {
        c = it->mSecond;
    }
    if (c != NULL) {
        if (c->mSharedUser != NULL) {
            c->mSharedUser->mCapsules.Erase(c);
            c->mSharedUser->mCapsules.Insert(newp);
        }
        else {
            ReplaceUserIdLP(c->mUserId, newp);
        }
    }
    mCapsules[name] = newp;
}

Boolean CCapsuleManagerService::Settings::AddUserIdLP(
    /* [in] */ Int32 uid,
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* name)
{
    if (uid >= FIRST_APPLICATION_UID + MAX_APPLICATION_UIDS) {
        return FALSE;
    }

    if (uid >= FIRST_APPLICATION_UID) {
        Int32 N = mUserIds.GetSize();
        const Int32 index = uid - FIRST_APPLICATION_UID;
        while (index >= N) {
            mUserIds.PushBack(NULL);
            N++;
        }
        if (mUserIds[index] != NULL) {
            ReportSettingsProblem(6 /*Log.ERROR*/,
                    StringBuffer("Adding duplicate user id: ") + uid
                    + " name=" /*+ name*/);
            return FALSE;
        }
        mUserIds[index] = obj;
    }
    else {
        IInterface* obj = NULL;
        HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mOtherUserIds.Find(uid);
        if (it != mOtherUserIds.End()) obj = it->mSecond;
        if (obj != NULL) {
            ReportSettingsProblem(6 /*Log.ERROR */,
                    StringBuffer("Adding duplicate shared id: ") + uid
                    + " name=" /*+ name*/);
            return FALSE;
        }
        mOtherUserIds[uid] = obj;
    }
    return TRUE;
}

AutoPtr<IInterface> CCapsuleManagerService::Settings::GetUserIdLP(
    /* [in] */ Int32 uid)
{
    if (uid >= FIRST_APPLICATION_UID) {
        Int32 N = mUserIds.GetSize();
        const Int32 index = uid - FIRST_APPLICATION_UID;
        return index < N ? mUserIds[index] : NULL;
    }
    else {
        AutoPtr<IInterface> obj;
        HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mOtherUserIds.Find(uid);
        if (it != mOtherUserIds.End()) obj = it->mSecond;
        return obj;
    }
}

HashSet<String>* CCapsuleManagerService::Settings::FindCapsulesWithFlag(
    /* [in] */ Int32 flag)
{
    HashSet<String>* ret = new HashSet<String>();
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it;
    for (it = mCapsules.Begin(); it != mCapsules.End(); it++) {
        CapsuleSetting* cs = it->mSecond;
        // Has to match atleast all the flag bits set on flag
        if ((cs->mCapFlags & flag) == flag) {
            ret->Insert(cs->mName);
        }
    }
    return ret;
}

void CCapsuleManagerService::Settings::RemoveUserIdLP(
    /* [in] */ Int32 uid)
{
    if (uid >= FIRST_APPLICATION_UID) {
        Int32 N = mUserIds.GetSize();
        const Int32 index = uid - FIRST_APPLICATION_UID;
        if (index < N) mUserIds[index] = NULL;
    }
    else {
        mOtherUserIds.Erase(uid);
    }
}

void CCapsuleManagerService::Settings::ReplaceUserIdLP(
    /* [in] */ Int32 uid,
    /* [in] */ IInterface* obj)
{
    if (uid >= FIRST_APPLICATION_UID) {
        Int32 N = mUserIds.GetSize();
        const Int32 index = uid - FIRST_APPLICATION_UID;
        if (index < N) mUserIds[index] = obj;
    }
    else {
        mOtherUserIds[uid] = obj;
    }
}

void CCapsuleManagerService::Settings::WriteLP()
{
    //Debug.startMethodTracing("/data/system/packageprof", 8 * 1024 * 1024);

    // Keep the old settings around until we know the new ones have
    // been successfully written.
    Boolean value;
    if (mSettingsFilename->Exists(&value), value) {
        // Presence of backup settings file indicates that we failed
        // to persist settings earlier. So preserve the older
        // backup for future reference since the current settings
        // might have been corrupted.
        if (mBackupSettingsFilename->Exists(&value), !value) {
            if (mSettingsFilename->RenameTo(mBackupSettingsFilename, &value), !value) {
                Slogger::W(TAG, "Unable to backup package manager settings, current changes will be lost at reboot");
                return;
            }
        }
        else {
            mSettingsFilename->Delete(&value);
            Slogger::W(TAG, "Preserving older settings backup");
        }
    }

    mPastSignatures.Clear();

//    try {
        AutoPtr<IFileOutputStream> fstr;
        CFileOutputStream::New(mSettingsFilename, (IFileOutputStream**)&fstr);
        AutoPtr<IBufferedOutputStream> str;
        CBufferedOutputStream::New(fstr, (IBufferedOutputStream**)&str);

        //XmlSerializer serializer = XmlUtils.serializerInstance();
        AutoPtr<IXmlSerializer> serializer;
//        CFastXmlSerializer::New((IXmlSerializer**)&serializer);
        serializer->SetOutput(str, String("utf-8"));
        serializer->StartDocument(String(NULL), TRUE);
        serializer->SetFeature("http://xmlpull.org/v1/doc/features.html#indent-output", TRUE);

        serializer->WriteStartTag(NULL, "packages");

        serializer->WriteStartTag(NULL, "last-platform-version");
        serializer->WriteAttribute(NULL, "internal", String::FromInt32(mInternalSdkPlatform));
        serializer->WriteAttribute(NULL, "external", String::FromInt32(mExternalSdkPlatform));
        serializer->WriteEndTag(NULL, "last-platform-version");

        serializer->WriteStartTag(NULL, "permission-trees");
        HashMap<String, BasePermission*>::Iterator bpit = mPermissionTrees.Begin();
        for (; bpit != mPermissionTrees.End(); ++bpit) {
            WritePermission(serializer, bpit->mSecond);
        }
        serializer->WriteEndTag(NULL, "permission-trees");

        serializer->WriteStartTag(NULL, "permissions");
        bpit = mPermissions.Begin();
        for (; bpit != mPermissions.End(); ++bpit) {
            WritePermission(serializer, bpit->mSecond);
        }
        serializer->WriteEndTag(NULL, "permissions");

        HashMap<String, AutoPtr<CapsuleSetting> >::Iterator csit = mCapsules.Begin();
        for (; csit != mCapsules.End(); ++csit) {
            WriteCapsule(serializer, csit->mSecond);
        }

        csit = mDisabledSysCapsules.Begin();
        for (; csit != mDisabledSysCapsules.End(); ++csit) {
            WriteDisabledSysCapsule(serializer, csit->mSecond);
        }

        serializer->WriteStartTag(NULL, "preferred-activities");
        Set<PreferredActivity*>* list = mPreferredActivities->FilterSet();
        Set<PreferredActivity*>::Iterator pait = list->Begin();
        for (; pait != list->End(); ++pait) {
            serializer->WriteStartTag(NULL, "item");
            (*pait)->WriteToXml(serializer);
            serializer->WriteEndTag(NULL, "item");
        }
        serializer->WriteEndTag(NULL, "preferred-activities");

        HashMap<String, AutoPtr<SharedUserSetting> >::Iterator susit = mSharedUsers.Begin();
        for (; susit != mSharedUsers.End(); ++susit) {
            SharedUserSetting* usr = susit->mSecond;
            serializer->WriteStartTag(NULL, "shared-user");
            serializer->WriteAttribute(NULL, "name", usr->mName);
            serializer->WriteAttribute(NULL, "userId",
                    String::FromInt32(usr->mUserId));
            usr->mSignatures->WriteXml(serializer, "sigs", &mPastSignatures);
            serializer->WriteStartTag(NULL, "perms");
            HashSet<String>::Iterator sit = usr->mGrantedPermissions.Begin();
            for (; sit != usr->mGrantedPermissions.End(); ++sit) {
                serializer->WriteStartTag(NULL, "item");
                serializer->WriteAttribute(NULL, "name", *sit);
                serializer->WriteEndTag(NULL, "item");
            }
            serializer->WriteEndTag(NULL, "perms");
            serializer->WriteEndTag(NULL, "shared-user");
        }

        List<String>::Iterator sit = mCapsulesToBeCleaned.Begin();
        for (; sit != mCapsulesToBeCleaned.End(); ++sit) {
            serializer->WriteStartTag(NULL, "cleaning-package");
            serializer->WriteAttribute(NULL, "name", *sit);
            serializer->WriteEndTag(NULL, "cleaning-package");
        }

        HashMap<String, String>::Iterator ssit = mRenamedCapsules.Begin();
        for (; ssit != mRenamedCapsules.End(); ++ssit) {
            serializer->WriteStartTag(NULL, "renamed-package");
            serializer->WriteAttribute(NULL, "new", ssit->mFirst);
            serializer->WriteAttribute(NULL, "old", ssit->mSecond);
            serializer->WriteEndTag(NULL, "renamed-package");
        }

        serializer->WriteEndTag(NULL, "packages");

        serializer->EndDocument();

        str->Flush();
        FileUtils::Sync(fstr);
        str->Close();

        // New settings successfully written, old ones are no longer
        // needed.
        mBackupSettingsFilename->Delete(&value);
        String fileStr;
        mSettingsFilename->ToString(&fileStr);
        FileUtils::SetPermissions(fileStr,
                FileUtils::IRUSR|FileUtils::IWUSR
                |FileUtils::IRGRP|FileUtils::IWGRP
                |FileUtils::IROTH,
                -1, -1);

//        // Write package list file now, use a JournaledFile.
//        //
//        File tempFile = new File(mPackageListFilename.toString() + ".tmp");
//        JournaledFile journal = new JournaledFile(mPackageListFilename, tempFile);
//
//        fstr = new FileOutputStream(journal.chooseForWrite());
//        str = new BufferedOutputStream(fstr);
//        try {
//            StringBuilder sb = new StringBuilder();
//            for (PackageSetting pkg : mPackages.values()) {
//                ApplicationInfo ai = pkg.pkg.applicationInfo;
//                String dataPath = ai.dataDir;
//                boolean isDebug  = (ai.flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0;
//
//                // Avoid any application that has a space in its path
//                // or that is handled by the system.
//                if (dataPath.indexOf(" ") >= 0 || ai.uid <= Process.FIRST_APPLICATION_UID)
//                    continue;
//
//                // we store on each line the following information for now:
//                //
//                // pkgName    - package name
//                // userId     - application-specific user id
//                // debugFlag  - 0 or 1 if the package is debuggable.
//                // dataPath   - path to package's data path
//                //
//                // NOTE: We prefer not to expose all ApplicationInfo flags for now.
//                //
//                // DO NOT MODIFY THIS FORMAT UNLESS YOU CAN ALSO MODIFY ITS USERS
//                // FROM NATIVE CODE. AT THE MOMENT, LOOK AT THE FOLLOWING SOURCES:
//                //   system/core/run-as/run-as.c
//                //
//                sb.setLength(0);
//                sb.append(ai.packageName);
//                sb.append(" ");
//                sb.append((int)ai.uid);
//                sb.append(isDebug ? " 1 " : " 0 ");
//                sb.append(dataPath);
//                sb.append("\n");
//                str.write(sb.toString().getBytes());
//            }
//            str.flush();
//            FileUtils.sync(fstr);
//            str.close();
//            journal.commit();
//        }
//        catch (Exception  e) {
//            journal.rollback();
//        }

        mCapsuleListFilename->ToString(&fileStr);
        FileUtils::SetPermissions(fileStr,
                FileUtils::IRUSR|FileUtils::IWUSR
                |FileUtils::IRGRP|FileUtils::IWGRP
                |FileUtils::IROTH,
                -1, -1);

        return;

//    } catch(XmlPullParserException e) {
//        Slog.w(TAG, "Unable to write package manager settings, current changes will be lost at reboot", e);
//    } catch(java.io.IOException e) {
//        Slog.w(TAG, "Unable to write package manager settings, current changes will be lost at reboot", e);
//    }
    // Clean up partially written files
    if (mSettingsFilename->Exists(&value), value) {
        if (mSettingsFilename->Delete(&value), !value) {
            Logger::I(TAG, "Failed to clean up mangled file: " + mSettingsFilename);
        }
    }
    //Debug.stopMethodTracing();
}

void CCapsuleManagerService::Settings::WriteDisabledSysCapsule(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ const CapsuleSetting* cap)
{
    serializer->WriteStartTag(NULL, "updated-package");
    serializer->WriteAttribute(NULL, "name", cap->mName);
    if (!cap->mRealName.IsNull()) {
        serializer->WriteAttribute(NULL, "realName", cap->mRealName);
    }
    serializer->WriteAttribute(NULL, "codePath", cap->mCodePathString);
    serializer->WriteAttribute(NULL, "ft", /*Long.toHexString*/String::FromInt64(cap->mTimeStamp));
    serializer->WriteAttribute(NULL, "it", /*Long.toHexString*/String::FromInt64(cap->mFirstInstallTime));
    serializer->WriteAttribute(NULL, "ut", /*Long.toHexString*/String::FromInt64(cap->mLastUpdateTime));
    serializer->WriteAttribute(NULL, "version", String::FromInt32(cap->mVersionCode));
    if (!cap->mResourcePathString.Equals(cap->mCodePathString)) {
        serializer->WriteAttribute(NULL, "resourcePath", cap->mResourcePathString);
    }
    if (!cap->mNativeLibraryPathString.IsNull()) {
        serializer->WriteAttribute(NULL, "nativeLibraryPath", cap->mNativeLibraryPathString);
    }
    if (cap->mSharedUser == NULL) {
        serializer->WriteAttribute(NULL, "userId",
                String::FromInt32(cap->mUserId));
    }
    else {
        serializer->WriteAttribute(NULL, "sharedUserId",
                String::FromInt32(cap->mUserId));
    }
    serializer->WriteStartTag(NULL, "perms");
    if (cap->mSharedUser == NULL) {
        // If this is a shared user, the permissions will
        // be written there.  We still need to write an
        // empty permissions list so permissionsFixed will
        // be set.
        HashSet<String>::Iterator it = cap->mGrantedPermissions.Begin();
        for (; it != cap->mGrantedPermissions.End(); ++it) {
            BasePermission* bp = NULL;
            HashMap<String, BasePermission*>::Iterator bpit = mPermissions.Find(*it);
            if (bpit != mPermissions.End()) bp = bpit->mSecond;
            if (bp != NULL) {
                // We only need to write signature or system permissions but this wont
                // match the semantics of grantedPermissions. So write all permissions.
                serializer->WriteStartTag(NULL, "item");
                serializer->WriteAttribute(NULL, "name", *it);
                serializer->WriteEndTag(NULL, "item");
            }
        }
    }
    serializer->WriteEndTag(NULL, "perms");
    serializer->WriteEndTag(NULL, "updated-package");
}

void CCapsuleManagerService::Settings::WriteCapsule(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ const CapsuleSetting* cap)
{
    serializer->WriteStartTag(NULL, "package");
    serializer->WriteAttribute(NULL, "name", cap->mName);
    if (!cap->mRealName.IsNull()) {
        serializer->WriteAttribute(NULL, "realName", cap->mRealName);
    }
    serializer->WriteAttribute(NULL, "codePath", cap->mCodePathString);
    if (!cap->mResourcePathString.Equals(cap->mCodePathString)) {
        serializer->WriteAttribute(NULL, "resourcePath", cap->mResourcePathString);
    }
    if (!cap->mNativeLibraryPathString.IsNull()) {
        serializer->WriteAttribute(NULL, "nativeLibraryPath", cap->mNativeLibraryPathString);
    }
    serializer->WriteAttribute(NULL, "flags",
            String::FromInt32(cap->mCapFlags));
    serializer->WriteAttribute(NULL, "ft", /*Long.toHexString*/String::FromInt64(cap->mTimeStamp));
    serializer->WriteAttribute(NULL, "it", /*Long.toHexString*/String::FromInt64(cap->mFirstInstallTime));
    serializer->WriteAttribute(NULL, "ut", /*Long.toHexString*/String::FromInt64(cap->mLastUpdateTime));
    serializer->WriteAttribute(NULL, "version", String::FromInt32(cap->mVersionCode));
    if (cap->mSharedUser == NULL) {
        serializer->WriteAttribute(NULL, "userId",
                String::FromInt32(cap->mUserId));
    }
    else {
        serializer->WriteAttribute(NULL, "sharedUserId",
                String::FromInt32(cap->mUserId));
    }
    if (cap->mUidError) {
        serializer->WriteAttribute(NULL, "uidError", "true");
    }
    if (cap->mEnabled != CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT) {
        serializer->WriteAttribute(NULL, "enabled",
                cap->mEnabled == CapsuleManager_COMPONENT_ENABLED_STATE_ENABLED
                ? "true" : "false");
    }
    if(cap->mInstallStatus == CAP_INSTALL_INCOMPLETE) {
        serializer->WriteAttribute(NULL, "installStatus", "false");
    }
    if (!cap->mInstallerCapsuleName.IsNull()) {
        serializer->WriteAttribute(NULL, "installer", cap->mInstallerCapsuleName);
    }
    cap->mSignatures->WriteXml(serializer, "sigs", &mPastSignatures);
    HashSet<String>::Iterator it;
    if ((cap->mCapFlags & ApplicationInfo_FLAG_SYSTEM) == 0) {
        serializer->WriteStartTag(NULL, "perms");
        if (cap->mSharedUser == NULL) {
            // If this is a shared user, the permissions will
            // be written there.  We still need to write an
            // empty permissions list so permissionsFixed will
            // be set.
            it = cap->mGrantedPermissions.Begin();
            for (; it != cap->mGrantedPermissions.End(); ++it) {
                serializer->WriteStartTag(NULL, "item");
                serializer->WriteAttribute(NULL, "name", *it);
                serializer->WriteEndTag(NULL, "item");
            }
        }
        serializer->WriteEndTag(NULL, "perms");
    }
    it = cap->mDisabledComponents.Begin();
    if (it != cap->mDisabledComponents.End()) {
        serializer->WriteStartTag(NULL, "disabled-components");
        for (; it != cap->mDisabledComponents.End(); ++it) {
            serializer->WriteStartTag(NULL, "item");
            serializer->WriteAttribute(NULL, "name", *it);
            serializer->WriteEndTag(NULL, "item");
        }
        serializer->WriteEndTag(NULL, "disabled-components");

    }
    it = cap->mEnabledComponents.Begin();
    if (it != cap->mEnabledComponents.End()) {
        serializer->WriteStartTag(NULL, "enabled-components");
        for (; it != cap->mEnabledComponents.End(); ++it) {
            serializer->WriteStartTag(NULL, "item");
            serializer->WriteAttribute(NULL, "name", *it);
            serializer->WriteEndTag(NULL, "item");
        }
        serializer->WriteEndTag(NULL, "enabled-components");

    }

    serializer->WriteEndTag(NULL, "package");
}

void CCapsuleManagerService::Settings::WritePermission(
    /* [in] */ IXmlSerializer* serializer,
    /* [in] */ BasePermission* bp)
{
    if (bp->mType != BasePermission::TYPE_BUILTIN
            && !bp->mSourceCapsule.IsNull()) {
        serializer->WriteStartTag(NULL, "item");
        serializer->WriteAttribute(NULL, "name", bp->mName);
        serializer->WriteAttribute(NULL, "package", bp->mSourceCapsule);
        if (bp->mProtectionLevel !=
                PermissionInfo_PROTECTION_NORMAL) {
            serializer->WriteAttribute(NULL, "protection",
                    String::FromInt32(bp->mProtectionLevel));
        }
        if (DEBUG_SETTINGS) Logger::W(TAG,
                StringBuffer("Writing perm: name=") + bp->mName + " type=" + bp->mType);
        if (bp->mType == BasePermission::TYPE_DYNAMIC) {
            IPermissionInfo* pi = bp->mPerm != NULL ? bp->mPerm->mInfo
                    : bp->mPendingInfo;
            if (pi != NULL) {
                serializer->WriteAttribute(NULL, "type", "dynamic");
                Int32 piIcon;
                pi->GetIcon(&piIcon);
                if (piIcon != 0) {
                    serializer->WriteAttribute(NULL, "icon",
                            String::FromInt32(piIcon));
                }
                AutoPtr<ICharSequence> piLabel;
                pi->GetNonLocalizedLabel((ICharSequence**)&piLabel);
                if (piLabel != NULL) {
                    String str;
                    piLabel->ToString(&str);
                    serializer->WriteAttribute(NULL, "label", str);
                }
            }
        }
        serializer->WriteEndTag(NULL, "item");
    }
}

String CCapsuleManagerService::Settings::GetReadMessagesLP()
{
    return String(mReadMessages);
}

List< AutoPtr<CCapsuleManagerService::CapsuleSetting> >*
CCapsuleManagerService::Settings::GetListOfIncompleteInstallCapsules()
{
    List< AutoPtr<CapsuleSetting> >* ret = new List< AutoPtr<CapsuleSetting> >();
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Begin();
    for (; it != mCapsules.End(); ++it) {
        CapsuleSetting* ps = it->mSecond;
        if(ps->GetInstallStatus() == CAP_INSTALL_INCOMPLETE) {
            ret->PushBack(ps);
        }
    }
    return ret;
}

Boolean CCapsuleManagerService::Settings::ReadLP()
{
    AutoPtr<IFileInputStream> str;
    Boolean value;
    if (mBackupSettingsFilename->Exists(&value), value) {
//        try {
        CFileInputStream::New(mBackupSettingsFilename, (IFileInputStream**)&str);
        mReadMessages += "Reading from backup settings file\n";
        ReportSettingsProblem(4 /*Log.INFO*/, "Need to read from backup settings file");
        if (mSettingsFilename->Exists(&value), value) {
            // If both the backup and settings file exist, we
            // ignore the settings since it might have been
            // corrupted.
            Slogger::W(TAG, "Cleaning up settings file " + mSettingsFilename);
            mSettingsFilename->Delete(&value);
        }
//        } catch (java.io.IOException e) {
//            // We'll try for the normal settings file.
//        }
    }

    mPastSignatures.Clear();

//    try {
    if (str == NULL) {
        if (mSettingsFilename->Exists(&value), !value) {
            mReadMessages += "No settings file found\n";
            ReportSettingsProblem(4 /*Log.INFO*/, "No settings file; creating initial state");
            return FALSE;
        }
        CFileInputStream::New(mSettingsFilename, (IFileInputStream**)&str);
    }
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInputEx(str, String(NULL));

    Int32 type;
    while ((parser->Next(&type), type != IXmlPullParser_START_TAG)
            && type != IXmlPullParser_END_DOCUMENT) {
    }

    if (type != IXmlPullParser_START_TAG) {
        mReadMessages += "No start tag found in settings file\n";
        ReportSettingsProblem(5 /*Log.WARN*/, "No start tag found in package manager settings");
        return FALSE;
    }

    Int32 outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("package")) {
            ReadCapsuleLP(parser);
        }
        else if (tagName.Equals("permissions")) {
            ReadPermissionsLP(&mPermissions, parser);
        }
        else if (tagName.Equals("permission-trees")) {
            ReadPermissionsLP(&mPermissionTrees, parser);
        }
        else if (tagName.Equals("shared-user")) {
            ReadSharedUserLP(parser);
        }
        else if (tagName.Equals("preferred-packages")) {
            // no longer used.
        }
        else if (tagName.Equals("preferred-activities")) {
            ReadPreferredActivitiesLP(parser);
        }
        else if(tagName.Equals("updated-package")) {
            ReadDisabledSysCapsuleLP(parser);
        }
        else if (tagName.Equals("cleaning-package")) {
            String name;
            parser->GetAttributeValueEx(NULL, "name", &name);
            if (!name.IsNull()) {
                mCapsulesToBeCleaned.PushBack(name);
            }
        }
        else if (tagName.Equals("renamed-package")) {
            String nname, oname;
            parser->GetAttributeValueEx(NULL, "new", &nname);
            parser->GetAttributeValueEx(NULL, "old", &oname);
            if (!nname.IsNull() && !oname.IsNull()) {
                mRenamedCapsules[nname] = oname;
            }
        }
        else if (tagName.Equals("last-platform-version")) {
            mInternalSdkPlatform = mExternalSdkPlatform = 0;
//            try {
            String internal;
            parser->GetAttributeValueEx(NULL, "internal", &internal);
            if (!internal.IsNull()) {
                mInternalSdkPlatform = internal.ToInt32();
            }
            String external;
            parser->GetAttributeValueEx(NULL, "external", &external);
            if (!external.IsNull()) {
                mExternalSdkPlatform = external.ToInt32();
            }
//            } catch (NumberFormatException e) {
//            }
        }
        else {
//            Slog.w(TAG, "Unknown element under <packages>: "
//                  + parser.getName());
            XmlUtils::SkipCurrentTag(parser);
        }
    }

    str->Close();

//    } catch(XmlPullParserException e) {
//        mReadMessages.append("Error reading: " + e.toString());
//        reportSettingsProblem(Log.ERROR, "Error reading settings: " + e);
//        Slog.e(TAG, "Error reading package manager settings", e);
//    } catch(java.io.IOException e) {
//        mReadMessages.append("Error reading: " + e.toString());
//        reportSettingsProblem(Log.ERROR, "Error reading settings: " + e);
//        Slog.e(TAG, "Error reading package manager settings", e);
//    }

    List< AutoPtr<PendingCapsule> >::Iterator it = mPendingCapsules.Begin();
    for (; it != mPendingCapsules.End(); ++it) {
        const PendingCapsule* pp = *it;
//        Object idObj = getUserIdLP(pp.sharedId);
//        if (idObj != null && idObj instanceof SharedUserSetting) {
//            PackageSetting p = getPackageLP(pp.name, null, pp.realName,
//                    (SharedUserSetting) idObj, pp.codePath, pp.resourcePath,
//                    pp.nativeLibraryPathString, pp.versionCode, pp.pkgFlags, true, true);
//            if (p == null) {
//                reportSettingsProblem(Log.WARN, "Unable to create application package for "
//                        + pp.name);
//                continue;
//            }
//            p.copyFrom(pp);
//        }
//        else if (idObj != null) {
//            String msg = "Bad package setting: package " + pp.name
//                    + " has shared uid " + pp.sharedId
//                    + " that is not a shared uid\n";
//            mReadMessages.append(msg);
//            reportSettingsProblem(Log.ERROR, msg);
//        }
//        else {
//            String msg = "Bad package setting: package " + pp.name
//                    + " has shared uid " + pp.sharedId
//                    + " that is not defined\n";
//            mReadMessages.append(msg);
//            reportSettingsProblem(Log.ERROR, msg);
//        }
    }
    mPendingCapsules.Clear();

//    /*
//     * Make sure all the updated system packages have their shared users
//     * associated with them.
//     */
//    final Iterator<PackageSetting> disabledIt = mDisabledSysPackages.values().iterator();
//    while (disabledIt.hasNext()) {
//        final PackageSetting disabledPs = disabledIt.next();
//        final Object id = getUserIdLP(disabledPs.userId);
//        if (id != null && id instanceof SharedUserSetting) {
//          disabledPs.sharedUser = (SharedUserSetting) id;
//        }
//    }

    mReadMessages += StringBuffer("Read completed successfully: ")
            + mCapsules.GetSize() + " packages, "
            + mSharedUsers.GetSize() + " shared uids\n";

    return TRUE;
}

Int32 CCapsuleManagerService::Settings::ReadInt(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ String ns,
    /* [in] */ String name,
    /* [in] */ Int32 defValue)
{
    String v;
    parser->GetAttributeValueEx(ns, name, &v);
//    try {
    if (v.IsNull()) {
        return defValue;
    }
    return v.ToInt32();
//    } catch (NumberFormatException e) {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: attribute " +
//                name + " has bad integer value " + v + " at "
//                + parser.getPositionDescription());
//    }
//    return defValue;
}

void CCapsuleManagerService::Settings::ReadPermissionsLP(
    /* [in] */ HashMap<String, BasePermission*>* out,
    /* [in] */ IXmlPullParser* parser)
{
//    int outerDepth = parser.getDepth();
//    int type;
//    while ((type=parser.next()) != XmlPullParser.END_DOCUMENT
//           && (type != XmlPullParser.END_TAG
//                   || parser.getDepth() > outerDepth)) {
//        if (type == XmlPullParser.END_TAG
//                || type == XmlPullParser.TEXT) {
//            continue;
//        }
//
//        String tagName = parser.getName();
//        if (tagName.equals("item")) {
//            String name = parser.getAttributeValue(null, "name");
//            String sourcePackage = parser.getAttributeValue(null, "package");
//            String ptype = parser.getAttributeValue(null, "type");
//            if (name != null && sourcePackage != null) {
//                boolean dynamic = "dynamic".equals(ptype);
//                BasePermission bp = new BasePermission(name, sourcePackage,
//                        dynamic
//                        ? BasePermission.TYPE_DYNAMIC
//                        : BasePermission.TYPE_NORMAL);
//                bp.protectionLevel = readInt(parser, null, "protection",
//                        PermissionInfo.PROTECTION_NORMAL);
//                if (dynamic) {
//                    PermissionInfo pi = new PermissionInfo();
//                    pi.packageName = sourcePackage.intern();
//                    pi.name = name.intern();
//                    pi.icon = readInt(parser, null, "icon", 0);
//                    pi.nonLocalizedLabel = parser.getAttributeValue(
//                            null, "label");
//                    pi.protectionLevel = bp.protectionLevel;
//                    bp.pendingInfo = pi;
//                }
//                out.put(bp.name, bp);
//            } else {
//                reportSettingsProblem(Log.WARN,
//                        "Error in package manager settings: permissions has"
//                        + " no name at " + parser.getPositionDescription());
//            }
//        } else {
//            reportSettingsProblem(Log.WARN,
//                    "Unknown element reading permissions: "
//                    + parser.getName() + " at "
//                    + parser.getPositionDescription());
//        }
//        XmlUtils.skipCurrentTag(parser);
//    }
}

void CCapsuleManagerService::Settings::ReadDisabledSysCapsuleLP(
    /* [in] */ IXmlPullParser* parser)
{
    String name, realName, codePathStr;
    String resourcePathStr, nativeLibraryPathStr;
    parser->GetAttributeValueEx(NULL, "name", &name);
    parser->GetAttributeValueEx(NULL, "realName", &realName);
    parser->GetAttributeValueEx(NULL, "codePath", &codePathStr);
    parser->GetAttributeValueEx(NULL, "resourcePath", &resourcePathStr);
    parser->GetAttributeValueEx(NULL, "nativeLibraryPath", &nativeLibraryPathStr);
    if (resourcePathStr.IsNull()) {
        resourcePathStr = codePathStr;
    }
    String version;
    parser->GetAttributeValueEx(NULL, "version", &version);
    Int32 versionCode = 0;
    if (!version.IsNull()) {
//        try {
        versionCode = version.ToInt32();
//        } catch (NumberFormatException e) {
//        }
    }

    Int32 capFlags = 0;
    capFlags |= ApplicationInfo_FLAG_SYSTEM;
    AutoPtr<IFile> file1, file2;
    CFile::New(codePathStr, (IFile**)&file1);
    CFile::New(resourcePathStr, (IFile**)&file2);
    AutoPtr<CapsuleSetting> ps = new CapsuleSetting(name, realName, file1,
            file2, nativeLibraryPathStr, versionCode, capFlags);
    String timeStampStr;
    parser->GetAttributeValueEx(NULL, "ft", &timeStampStr);
    if (!timeStampStr.IsNull()) {
//        try {
        Int64 timeStamp = timeStampStr.ToInt64(); /*Long.parseLong(timeStampStr, 16);*/
        ps->SetTimeStamp(timeStamp);
//        } catch (NumberFormatException e) {
//        }
    }
    else {
        parser->GetAttributeValueEx(NULL, "ts", &timeStampStr);
        if (!timeStampStr.IsNull()) {
//            try {
            Int64 timeStamp = timeStampStr.ToInt64();
            ps->SetTimeStamp(timeStamp);
//            } catch (NumberFormatException e) {
//            }
        }
    }
    parser->GetAttributeValueEx(NULL, "it", &timeStampStr);
    if (!timeStampStr.IsNull()) {
//        try {
        ps->mFirstInstallTime = timeStampStr.ToInt64(); /*Long.parseLong(timeStampStr, 16);*/
//        } catch (NumberFormatException e) {
//        }
    }
    parser->GetAttributeValueEx(NULL, "ut", &timeStampStr);
    if (!timeStampStr.IsNull()) {
//        try {
        ps->mLastUpdateTime = timeStampStr.ToInt64(); /*Long.parseLong(timeStampStr, 16);*/
//        } catch (NumberFormatException e) {
//        }
    }
    String idStr;
    parser->GetAttributeValueEx(NULL, "userId", &idStr);
    ps->mUserId = !idStr.IsNull() ? idStr.ToInt32() : 0;
    if(ps->mUserId <= 0) {
        String sharedIdStr;
        parser->GetAttributeValueEx(NULL, "sharedUserId", &sharedIdStr);
        ps->mUserId = !sharedIdStr.IsNull() ? sharedIdStr.ToInt32() : 0;
    }
    Int32 type, outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("perms")) {
            ReadGrantedPermissionsLP(parser,
                    &ps->mGrantedPermissions);
        }
        else {
//            ReportSettingsProblem(Log.WARN,
//                    "Unknown element under <updated-package>: "
//                    + parser.getName());
            XmlUtils::SkipCurrentTag(parser);
        }
    }
    mDisabledSysCapsules[name] = ps;
}

void CCapsuleManagerService::Settings::ReadCapsuleLP(
    /* [in] */ IXmlPullParser* parser)
{
    String name;
    String realName;
    String idStr;
    String sharedIdStr;
    String codePathStr;
    String resourcePathStr;
    String nativeLibraryPathStr;
    String systemStr;
    String installerCapsuleName;
    String uidError;
    Int32 capFlags = 0;
    Int64 timeStamp = 0;
    Int64 firstInstallTime = 0;
    Int64 lastUpdateTime = 0;
    AutoPtr<CapsuleSettingBase> capsuleSetting;
    String version;
    Int32 versionCode = 0;
//    try {
    parser->GetAttributeValueEx(NULL, "name", &name);
    parser->GetAttributeValueEx(NULL, "realName", &realName);
    parser->GetAttributeValueEx(NULL, "userId", &idStr);
    parser->GetAttributeValueEx(NULL, "uidError", &uidError);
    parser->GetAttributeValueEx(NULL, "sharedUserId", &sharedIdStr);
    parser->GetAttributeValueEx(NULL, "codePath", &codePathStr);
    parser->GetAttributeValueEx(NULL, "resourcePath", &resourcePathStr);
    parser->GetAttributeValueEx(NULL, "nativeLibraryPath", &nativeLibraryPathStr);
    parser->GetAttributeValueEx(NULL, "version", &version);
    if (!version.IsNull()) {
//        try {
        versionCode = version.ToInt32();
//        } catch (NumberFormatException e) {
//        }
    }
    parser->GetAttributeValueEx(NULL, "installer", &installerCapsuleName);

    parser->GetAttributeValueEx(NULL, "flags", &systemStr);
    if (!systemStr.IsNull()) {
//        try {
        capFlags = systemStr.ToInt32();
//        } catch (NumberFormatException e) {
//        }
    }
    else {
        // For backward compatibility
        parser->GetAttributeValueEx(NULL, "system", &systemStr);
        if (!systemStr.IsNull()) {
            capFlags |= CString("true").EqualsIgnoreCase(systemStr) ? ApplicationInfo_FLAG_SYSTEM : 0;
        }
        else {
            // Old settings that don't specify system...  just treat
            // them as system, good enough.
            capFlags |= ApplicationInfo_FLAG_SYSTEM;
        }
    }
    String timeStampStr;
    parser->GetAttributeValueEx(NULL, "ft", &timeStampStr);
    if (!timeStampStr.IsNull()) {
//        try {
        timeStamp = timeStampStr.ToInt64(); /* Long.parseLong(timeStampStr, 16); */
//        } catch (NumberFormatException e) {
//        }
    }
    else {
        parser->GetAttributeValueEx(NULL, "ts", &timeStampStr);
        if (!timeStampStr.IsNull()) {
//            try {
            timeStamp = timeStampStr.ToInt64();
//            } catch (NumberFormatException e) {
//            }
        }
    }
    parser->GetAttributeValueEx(NULL, "it", &timeStampStr);
    if (!timeStampStr.IsNull()) {
//        try {
        firstInstallTime = timeStampStr.ToInt64(); /* Long.parseLong(timeStampStr, 16); */
//        } catch (NumberFormatException e) {
//        }
    }
    parser->GetAttributeValueEx(NULL, "ut", &timeStampStr);
    if (!timeStampStr.IsNull()) {
//        try {
        lastUpdateTime = timeStampStr.ToInt64(); /* Long.parseLong(timeStampStr, 16); */
//        } catch (NumberFormatException e) {
//        }
    }
    if (DEBUG_SETTINGS) Logger::V(TAG, String("Reading package: ") + name
            + " userId=" + idStr + " sharedUserId=" + sharedIdStr);
    Int32 userId = !idStr.IsNull() ? idStr.ToInt32() : 0;
    if (resourcePathStr.IsNull()) {
        resourcePathStr = codePathStr;
    }
//    if (realName != null) {
//        realName = realName.intern();
//    }
    if (name.IsNull()) {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: <package> has no name at "
//                + parser.getPositionDescription());
    }
    else if (codePathStr.IsNull()) {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: <package> has no codePath at "
//                + parser.getPositionDescription());
    }
    else if (userId > 0) {
        AutoPtr<IFile> file1, file2;
        CFile::New(codePathStr, (IFile**)&file1);
        CFile::New(resourcePathStr, (IFile**)&file2);
        capsuleSetting = AddCapsuleLP(name, realName, file1, file2,
                nativeLibraryPathStr, userId, versionCode, capFlags);
        if (DEBUG_SETTINGS) Logger::I(TAG, StringBuffer("Reading package ") + name
                + ": userId=" + userId + " pkg=" /*+ packageSetting*/);
        if (capsuleSetting == NULL) {
//            reportSettingsProblem(Log.ERROR,
//                    "Failure adding uid " + userId
//                    + " while parsing settings at "
//                    + parser.getPositionDescription());
        }
        else {
            capsuleSetting->SetTimeStamp(timeStamp);
            capsuleSetting->mFirstInstallTime = firstInstallTime;
            capsuleSetting->mLastUpdateTime = lastUpdateTime;
        }
    }
    else if (!sharedIdStr.IsNull()) {
        userId = !sharedIdStr.IsNull() ?
                sharedIdStr.ToInt32() : 0;
        if (userId > 0) {
            AutoPtr<IFile> file1, file2;
            CFile::New(codePathStr, (IFile**)&file1);
            CFile::New(resourcePathStr, (IFile**)&file2);
            capsuleSetting = new PendingCapsule(name, realName,
                    file1, file2, nativeLibraryPathStr, userId,
                    versionCode, capFlags);
            capsuleSetting->SetTimeStamp(timeStamp);
            capsuleSetting->mFirstInstallTime = firstInstallTime;
            capsuleSetting->mLastUpdateTime = lastUpdateTime;
            mPendingCapsules.PushBack((PendingCapsule*)capsuleSetting.Get());
            if (DEBUG_SETTINGS) Logger::I(TAG, StringBuffer("Reading package ") + name
                    + ": sharedUserId=" + userId + " pkg=" /*+ packageSetting*/);
        }
        else {
//            reportSettingsProblem(Log.WARN,
//                    "Error in package manager settings: package "
//                    + name + " has bad sharedId " + sharedIdStr
//                    + " at " + parser.getPositionDescription());
        }
    }
    else {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: package "
//                + name + " has bad userId " + idStr + " at "
//                + parser.getPositionDescription());
    }
//    } catch (NumberFormatException e) {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: package "
//                + name + " has bad userId " + idStr + " at "
//                + parser.getPositionDescription());
//    }
    if (capsuleSetting != NULL) {
        capsuleSetting->mUidError = CString("true").Equals(uidError);
        capsuleSetting->mInstallerCapsuleName = installerCapsuleName;
        capsuleSetting->mNativeLibraryPathString = nativeLibraryPathStr;
        String enabledStr;
        parser->GetAttributeValueEx(NULL, "enabled", &enabledStr);
        if (!enabledStr.IsNull()) {
            if (enabledStr.EqualsIgnoreCase("true")) {
                capsuleSetting->mEnabled = CapsuleManager_COMPONENT_ENABLED_STATE_ENABLED;
            }
            else if (enabledStr.EqualsIgnoreCase("false")) {
                capsuleSetting->mEnabled = CapsuleManager_COMPONENT_ENABLED_STATE_DISABLED;
            }
            else if (enabledStr.EqualsIgnoreCase("default")) {
                capsuleSetting->mEnabled = CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT;
            }
            else {
//                reportSettingsProblem(Log.WARN,
//                        "Error in package manager settings: package "
//                        + name + " has bad enabled value: " + idStr
//                        + " at " + parser.getPositionDescription());
            }
        }
        else {
            capsuleSetting->mEnabled = CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT;
        }
        String installStatusStr;
        parser->GetAttributeValueEx(NULL, "installStatus", &installStatusStr);
        if (!installStatusStr.IsNull()) {
            if (installStatusStr.EqualsIgnoreCase("false")) {
                capsuleSetting->mInstallStatus = CAP_INSTALL_INCOMPLETE;
            }
            else {
                capsuleSetting->mInstallStatus = CAP_INSTALL_COMPLETE;
            }
        }

        Int32 type, outerDepth, depth;
        parser->GetDepth(&outerDepth);
        while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
               && (type != IXmlPullParser_END_TAG
                       || (parser->GetDepth(&depth), depth > outerDepth))) {
            if (type == IXmlPullParser_END_TAG
                    || type == IXmlPullParser_TEXT) {
                continue;
            }

            String tagName;
            parser->GetName(&tagName);
            if (tagName.Equals("disabled-components")) {
                ReadDisabledComponentsLP(capsuleSetting, parser);
            }
            else if (tagName.Equals("enabled-components")) {
                ReadEnabledComponentsLP(capsuleSetting, parser);
            }
            else if (tagName.Equals("sigs")) {
                capsuleSetting->mSignatures->ReadXml(parser, &mPastSignatures);
            }
            else if (tagName.Equals("perms")) {
                ReadGrantedPermissionsLP(parser,
                        &capsuleSetting->mGrantedPermissions);
                capsuleSetting->mPermissionsFixed = TRUE;
            }
            else {
//                reportSettingsProblem(Log.WARN,
//                        "Unknown element under <package>: "
//                        + parser.getName());
                XmlUtils::SkipCurrentTag(parser);
            }
        }
    }
    else {
        XmlUtils::SkipCurrentTag(parser);
    }
}

void CCapsuleManagerService::Settings::ReadDisabledComponentsLP(
    /* [in] */ CapsuleSettingBase* capsuleSetting,
    /* [in] */ IXmlPullParser* parser)
{
    Int32 type, outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("item")) {
            String name;
            parser->GetAttributeValueEx(NULL, "name", &name);
            if (!name.IsNull()) {
                capsuleSetting->mDisabledComponents.Insert(name);
            }
            else {
//                reportSettingsProblem(Log.WARN,
//                        "Error in package manager settings: <disabled-components> has"
//                        + " no name at " + parser.getPositionDescription());
            }
        }
        else {
//            reportSettingsProblem(Log.WARN,
//                    "Unknown element under <disabled-components>: "
//                    + parser.getName());
        }
        XmlUtils::SkipCurrentTag(parser);
    }
}

void CCapsuleManagerService::Settings::ReadEnabledComponentsLP(
    /* [in] */ CapsuleSettingBase* capsuleSetting,
    /* [in] */ IXmlPullParser* parser)
{
    Int32 type, outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("item")) {
            String name;
            parser->GetAttributeValueEx(NULL, "name", &name);
            if (!name.IsNull()) {
                capsuleSetting->mEnabledComponents.Insert(name);
            }
            else {
//                reportSettingsProblem(Log.WARN,
//                        "Error in package manager settings: <enabled-components> has"
//                           + " no name at " + parser.getPositionDescription());
            }
        }
        else {
//            reportSettingsProblem(Log.WARN,
//                    "Unknown element under <enabled-components>: "
//                    + parser.getName());
        }
        XmlUtils::SkipCurrentTag(parser);
    }
}

void CCapsuleManagerService::Settings::ReadSharedUserLP(
    /* [in] */ IXmlPullParser* parser)
{
    String name;
    String idStr;
    Int32 capFlags = 0;
    AutoPtr<SharedUserSetting> su;
//    try {
    parser->GetAttributeValueEx(NULL, "name", &name);
    parser->GetAttributeValueEx(NULL, "userId", &idStr);
    Int32 userId = !idStr.IsNull() ? idStr.ToInt32() : 0;
    String sysStr;
    parser->GetAttributeValueEx(NULL, "system", &sysStr);
    if (CString("true").Equals(sysStr)) {
        capFlags |= ApplicationInfo_FLAG_SYSTEM;
    }
    if (name.IsNull()) {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: <shared-user> has no name at "
//                + parser.getPositionDescription());
    }
    else if (userId == 0) {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: shared-user "
//                + name + " has bad userId " + idStr + " at "
//                + parser.getPositionDescription());
    }
    else {
        if ((su = AddSharedUserLP(name, userId, capFlags)) == NULL) {
//            reportSettingsProblem(Log.ERROR,
//                    "Occurred while parsing settings at "
//                    + parser.getPositionDescription());
        }
    }
//    } catch (NumberFormatException e) {
//        reportSettingsProblem(Log.WARN,
//                "Error in package manager settings: package "
//                + name + " has bad userId " + idStr + " at "
//                + parser.getPositionDescription());
//    };

    if (su != NULL) {
        Int32 type, outerDepth, depth;
        parser->GetDepth(&outerDepth);
        while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
               && (type != IXmlPullParser_END_TAG
                       || (parser->GetDepth(&depth), depth > outerDepth))) {
            if (type == IXmlPullParser_END_TAG
                    || type == IXmlPullParser_TEXT) {
                continue;
            }

            String tagName;
            parser->GetName(&tagName);
            if (tagName.Equals("sigs")) {
                su->mSignatures->ReadXml(parser, &mPastSignatures);
            }
            else if (tagName.Equals("perms")) {
                ReadGrantedPermissionsLP(parser, &su->mGrantedPermissions);
            }
            else {
//                reportSettingsProblem(Log.WARN,
//                        "Unknown element under <shared-user>: "
//                        + parser.getName());
                XmlUtils::SkipCurrentTag(parser);
            }
        }

    } else {
        XmlUtils::SkipCurrentTag(parser);
    }
}

void CCapsuleManagerService::Settings::ReadGrantedPermissionsLP(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ HashSet<String>* outPerms)
{
    Int32 type, outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("item")) {
            String name;
            parser->GetAttributeValueEx(NULL, "name", &name);
            if (!name.IsNull()) {
                outPerms->Insert(name);
            }
            else {
//                reportSettingsProblem(Log.WARN,
//                        "Error in package manager settings: <perms> has"
//                           + " no name at " + parser.getPositionDescription());
            }
        }
        else {
//            reportSettingsProblem(Log.WARN,
//                    "Unknown element under <perms>: "
//                    + parser.getName());
        }
        XmlUtils::SkipCurrentTag(parser);
    }
}

void CCapsuleManagerService::Settings::ReadPreferredActivitiesLP(
    /* [in] */ IXmlPullParser* parser)
{
    Int32 type, outerDepth, depth;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
           && (type != IXmlPullParser_END_TAG
                   || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG
                || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (tagName.Equals("item")) {
            AutoPtr<PreferredActivity> pa = new PreferredActivity(parser);
            if (pa->mParseError.IsNull()) {
                mPreferredActivities->AddFilter(pa);
            }
            else {
//                reportSettingsProblem(Log.WARN,
//                        "Error in package manager settings: <preferred-activity> "
//                        + pa.mParseError + " at "
//                        + parser.getPositionDescription());
            }
        }
        else {
//            reportSettingsProblem(Log.WARN,
//                    "Unknown element under <preferred-activities>: "
//                    + parser.getName());
            XmlUtils::SkipCurrentTag(parser);
        }
    }
}

Int32 CCapsuleManagerService::Settings::NewUserIdLP(
    /* [in] */ IInterface* obj)
{
    // Let's be stupidly inefficient for now...
    const Int32 N = mUserIds.GetSize();
    for (Int32 i = 0; i < N; i++) {
        if (mUserIds[i] == NULL) {
            mUserIds[i] = obj;
            return FIRST_APPLICATION_UID + i;
        }
    }

    // None left?
    if (N >= MAX_APPLICATION_UIDS) {
        return -1;
    }

    mUserIds.PushBack(obj);
    return FIRST_APPLICATION_UID + N;
}

AutoPtr<CCapsuleManagerService::CapsuleSetting>
CCapsuleManagerService::Settings::GetDisabledSystemCap(
    /* [in] */ const String& name)
{
    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mDisabledSysCapsules.Find(name);
    return it != mDisabledSysCapsules.End() ? it->mSecond : NULL;
}

Boolean CCapsuleManagerService::Settings::IsEnabledLP(
    /* [in] */ IComponentInfo* componentInfo,
    /* [in] */ Int32 flags)
{
    if ((flags & CapsuleManager_GET_DISABLED_COMPONENTS) != 0) {
        return TRUE;
    }

    String capStr;
    componentInfo->GetCapsuleName(&capStr);
    CapsuleSetting* capsuleSettings = NULL;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it = mCapsules.Find(capStr);
    if (it != mCapsules.End()) {
        capsuleSettings = it->mSecond;
    }
//    if (Config.LOGV) {
//        Log.v(TAG, "isEnabledLock - packageName = " + componentInfo.packageName
//                   + " componentName = " + componentInfo.name);
//        Log.v(TAG, "enabledComponents: "
//                   + Arrays.toString(packageSettings.enabledComponents.toArray()));
//        Log.v(TAG, "disabledComponents: "
//                   + Arrays.toString(packageSettings.disabledComponents.toArray()));
//    }
    if (capsuleSettings == NULL) {
//        if (false) {
//            Log.w(TAG, "WAITING FOR DEBUGGER");
//            Debug.waitForDebugger();
//            Log.i(TAG, "We will crash!");
//        }
        return FALSE;
    }
    Boolean isEnabled;
    if (capsuleSettings->mEnabled == CapsuleManager_COMPONENT_ENABLED_STATE_DISABLED
            || (capsuleSettings->mCap != NULL
                    && (capsuleSettings->mCap->mApplicationInfo->IsEnabled(&isEnabled), !isEnabled)
                    && capsuleSettings->mEnabled == CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT)) {
        return FALSE;
    }
    String name;
    componentInfo->GetName(&name);
    if (capsuleSettings->mEnabledComponents.Find(name) != capsuleSettings->mEnabledComponents.End()) {
        return TRUE;
    }
    if (capsuleSettings->mDisabledComponents.Find(name) != capsuleSettings->mDisabledComponents.End()) {
        return FALSE;
    }
    Boolean enabled;
    componentInfo->IsEnabled(&enabled);
    return enabled;
}


///////////////////////////////////////////////////////////////////////////////
// CCapsuleManagerService

CString CCapsuleManagerService::TAG = "CCapsuleManagerService";
const Boolean CCapsuleManagerService::DEBUG_SETTINGS;
const Boolean CCapsuleManagerService::DEBUG_PREFERRED;
const Boolean CCapsuleManagerService::DEBUG_UPGRADE;
const Boolean CCapsuleManagerService::DEBUG_INSTALL;
const Boolean CCapsuleManagerService::MULTIPLE_APPLICATION_UIDS;
const Int32 CCapsuleManagerService::RADIO_UID;
const Int32 CCapsuleManagerService::LOG_UID;
const Int32 CCapsuleManagerService::NFC_UID;
const Int32 CCapsuleManagerService::FIRST_APPLICATION_UID;
const Int32 CCapsuleManagerService::MAX_APPLICATION_UIDS;
const Boolean CCapsuleManagerService::SHOW_INFO;
const Boolean CCapsuleManagerService::GET_CERTIFICATES;
CString CCapsuleManagerService::SYSTEM_PROPERTY_EFS_ENABLED = "persist.security.efs.enabled";
const Int32 CCapsuleManagerService::REMOVE_EVENTS;
const Int32 CCapsuleManagerService::ADD_EVENTS;
//const int REMOVE_EVENTS =
//	  FileObserver.CLOSE_WRITE | FileObserver.DELETE | FileObserver.MOVED_FROM;
//	  const int ADD_EVENTS =
//	      FileObserver.CLOSE_WRITE /*| FileObserver.CREATE*/ | FileObserver.MOVED_TO;

const Int32 CCapsuleManagerService::OBSERVER_EVENTS;
// Suffix used during package installation when copying/moving
// package apks to install directory.
CString CCapsuleManagerService::INSTALL_CAPSULE_SUFFIX = "-";
const Int32 CCapsuleManagerService::CAP_INSTALL_INCOMPLETE;
const Int32 CCapsuleManagerService::CAP_INSTALL_COMPLETE;
const Int32 CCapsuleManagerService::SCAN_MONITOR;
const Int32 CCapsuleManagerService::SCAN_NO_DEX;
const Int32 CCapsuleManagerService::SCAN_FORCE_DEX;
const Int32 CCapsuleManagerService::SCAN_UPDATE_SIGNATURE;
const Int32 CCapsuleManagerService::SCAN_NEW_INSTALL;
const Int32 CCapsuleManagerService::SCAN_NO_PATHS;
const Int32 CCapsuleManagerService::SCAN_UPDATE_TIME;
const Int32 CCapsuleManagerService::REMOVE_CHATTY;
CString CCapsuleManagerService::DEFAULT_CONTAINER_CAPSULE = "com.android.defcontainer";
//	    const ComponentName DEFAULT_CONTAINER_COMPONENT = new ComponentName(
//	            DEFAULT_CONTAINER_PACKAGE,
//	            "com.android.defcontainer.DefaultContainerService");
CString CCapsuleManagerService::LIB_DIR_NAME = "lib";
CString CCapsuleManagerService::mTempContainerPrefix = "smdl2tmp";
const Int32 CCapsuleManagerService::SEND_PENDING_BROADCAST;
const Int32 CCapsuleManagerService::MCS_BOUND;
const Int32 CCapsuleManagerService::END_COPY;
const Int32 CCapsuleManagerService::INIT_COPY;
const Int32 CCapsuleManagerService::MCS_UNBIND;
const Int32 CCapsuleManagerService::START_CLEANING_CAPSULE;
const Int32 CCapsuleManagerService::FIND_INSTALL_LOC;
const Int32 CCapsuleManagerService::POST_INSTALL;
const Int32 CCapsuleManagerService::MCS_RECONNECT;
const Int32 CCapsuleManagerService::MCS_GIVE_UP;
const Int32 CCapsuleManagerService::UPDATED_MEDIA_STATUS;
const Int32 CCapsuleManagerService::WRITE_SETTINGS;
const Int32 CCapsuleManagerService::DELETE_CAPSULE;
const Int32 CCapsuleManagerService::WRITE_SETTINGS_DELAY;
const Int32 CCapsuleManagerService::BROADCAST_DELAY;
const Int32 CCapsuleManagerService::MAX_CONTAINERS;
//	    private static final Comparator<ResolveInfo> mResolvePrioritySorter =
//	            new Comparator<ResolveInfo>() {
//
//	    private static final Comparator<ProviderInfo> mProviderInitOrderSorter =
//	            new Comparator<ProviderInfo>() {
const Boolean CCapsuleManagerService::DEBUG_SD_INSTALL;
CString CCapsuleManagerService::SD_ENCRYPTION_KEYSTORE_NAME = "AppsOnSD";
CString CCapsuleManagerService::SD_ENCRYPTION_ALGORITHM = "AES";
CString CCapsuleManagerService::DEFAULT_RESOURCES_FILE_NAME = "hello.apk";
const AutoPtr<IComponentName> CCapsuleManagerService::DEFAULT_CONTAINER_COMPONENT;

CCapsuleManagerService::CCapsuleManagerService()
    : mSdkVersion(0)
    , mFactoryTest(FALSE)
    , mNoDexOpt(FALSE)
    , mDefParseFlags(0)
    , mFrameworkInstallObserver(NULL)
    , mSystemInstallObserver(NULL)
    , mVendorInstallObserver(NULL)
    , mAppInstallObserver(NULL)
    , mDrmAppInstallObserver(NULL)
    , mInstaller(NULL)
    , mLastScanError(0)
    , mCapsules(23)
    , mSettings(NULL)
    , mRestoredSettings(FALSE)
    , mSharedLibraries(11)
    , mProvidersByComponent(11)
    , mContentProviders(11)
    , mInstrumentation(11)
    , mTransferedCapsules(11)
    , mProtectedBroadcasts(11)
    , mSystemReady(FALSE)
    , mSafeMode(FALSE)
    , mHasSystemUidErrors(FALSE)
    , mPlatformCapsule(NULL)
    , mNextInstallToken(1)
    , mDefContainerConn(new CCapsuleManagerService::DefaultContainerConnection(this))
    , mMediaMounted(FALSE)
    , mBound(FALSE)
{
    //should move to constructor

    if (DEFAULT_CONTAINER_COMPONENT == NULL) {
        ASSERT_SUCCEEDED(CComponentName::New(
            String(DEFAULT_CONTAINER_CAPSULE),
            String("com.android.defcontainer.DefaultContainerService"),
            (IComponentName**)&DEFAULT_CONTAINER_COMPONENT));
    }

    char cwd[512];

    mActivities = new ActivityIntentResolver(this);
    mReceivers = new ActivityIntentResolver(this);
    mServices = new ServiceIntentResolver(this);
    CActivityInfo::New((IActivityInfo**)&mResolveActivity);
    CResolveInfo::New((IResolveInfo**)&mResolveInfo);
    mOutPermissions = ArrayOf<Int32>::Alloc(3);

    getcwd(cwd, 512);
    ScanDir(String(cwd));
}

CCapsuleManagerService::~CCapsuleManagerService()
{
    delete mSettings;
    //free capsuleInfos
    mCapsules.Clear();
    delete mActivities;
    delete mReceivers;
    mContentProviders.Clear();
    mProvidersByComponent.Clear();
    mInstrumentation.Clear();
}

void CCapsuleManagerService::ScheduleWriteSettingsLocked()
{
//	    if (!mHandler.hasMessages(WRITE_SETTINGS)) {
//	        mHandler.sendEmptyMessageDelayed(WRITE_SETTINGS, WRITE_SETTINGS_DELAY);
//	    }
}

Boolean CCapsuleManagerService::InstallOnSd(
    /* [in] */ Int32 flags)
{
    if (((flags & CapsuleManager::INSTALL_FORWARD_LOCK) != 0) ||
            ((flags & CapsuleManager::INSTALL_INTERNAL) != 0)) {
        return FALSE;
    }
    if ((flags & CapsuleManager::INSTALL_EXTERNAL) != 0) {
        return TRUE;
    }
    return FALSE;
}

void CCapsuleManagerService::SplitString(
    /* [in] */ const String& str,
    /* [in] */ Char32 sep,
    /* [out] */ ArrayOf<String>** outStrs)
{
    Int32 count = 1;
    Int32 i = 0;
    while ((i = str.IndexOf(sep, i)) >= 0) {
        count++;
        i++;
    }

    ArrayOf<String>* res = ArrayOf<String>::Alloc(count);
    i=0;
    count = 0;
    Int32 lastI=0;
    while ((i = str.IndexOf(sep, i)) >= 0) {
        (*res)[count] = str.Substring(lastI, i);
        count++;
        i++;
        lastI = i;
    }
    (*res)[count] = str.Substring(lastI, str.GetLength());
    *outStrs = res;
}

ECode CCapsuleManagerService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean factoryTest)
{
//	    EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_START,
//	            SystemClock.uptimeMillis());

    if (mSdkVersion <= 0) {
        Slogger::W(TAG, "**** ro->mBuild.version->mSdk not set!");
    }

    mContext = context;
    mFactoryTest = factoryTest;
    mNoDexOpt = CString("eng").Equals(SystemProperties::Get("ro.build.type"));
    FAIL_RETURN(CDisplayMetrics::New((IDisplayMetrics**)&mMetrics));
    mSettings = new Settings(this);
    if (!mSettings) return E_OUT_OF_MEMORY_ERROR;
    mSettings->AddSharedUserLP(String("android.uid.system"),
            Process::SYSTEM_UID, ApplicationInfo_FLAG_SYSTEM);
    mSettings->AddSharedUserLP(String("android.uid.phone"),
            MULTIPLE_APPLICATION_UIDS ? RADIO_UID : FIRST_APPLICATION_UID,
            ApplicationInfo_FLAG_SYSTEM);
    mSettings->AddSharedUserLP(String("android.uid.log"),
            MULTIPLE_APPLICATION_UIDS ? LOG_UID : FIRST_APPLICATION_UID,
            ApplicationInfo_FLAG_SYSTEM);
    mSettings->AddSharedUserLP(String("android.uid.nfc"),
            MULTIPLE_APPLICATION_UIDS ? NFC_UID : FIRST_APPLICATION_UID,
            ApplicationInfo_FLAG_SYSTEM);

    String separateProcesses = SystemProperties::Get("debug->mSeparate_processes");
    if (!separateProcesses.IsNull() && separateProcesses.GetLength() > 0) {
        if (separateProcesses.Equals("*")) {
            mDefParseFlags = CapsuleParser::PARSE_IGNORE_PROCESSES;
            mSeparateProcesses = NULL;
            Slogger::W(TAG, "Running with debug->mSeparate_processes: * (ALL)");
        }
        else {
            mDefParseFlags = 0;
            SplitString(separateProcesses, ',', (ArrayOf<String>**)&mSeparateProcesses);
            Slogger::W(TAG, StringBuffer("Running with debug->mSeparate_processes: ")
                + separateProcesses);
        }
    }
    else {
        mDefParseFlags = 0;
        mSeparateProcesses = NULL;
    }

    Installer* installer = new Installer();
    if (!installer) return E_OUT_OF_MEMORY_ERROR;
    // Little hacky thing to check if installd is here, to determine
    // whether we are running on the simulator and thus need to take
    // care of building the /data file structure ourself.
    // (apparently the sim now has a working installer)
    if (installer->Ping() && Process::SupportsProcesses()) {
        mInstaller = installer;
    }
    else {
        mInstaller = NULL;
        delete installer;
    }

    AutoPtr<IWindowManager> wm;
    AutoPtr<IDisplay> d;

    context->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&wm);
    wm->GetDefaultDisplay((IDisplay**)&d);
    d->GetMetrics(mMetrics.Get());

    {
        Mutex::Autolock ilLock(mInstallLock);
        Mutex::Autolock cLock(mCapsulesLock);

        FAIL_RETURN(CApartment::New(FALSE, (IApartment**)&mApartment));
        mApartment->Start(ApartmentAttr_New);

        AutoPtr<IFile> dataDir = Environment::GetDataDirectory();
        FAIL_RETURN(CFile::New(dataDir, String("data"), (IFile**)&mAppDataDir));
        FAIL_RETURN(CFile::New(dataDir, String("secure/data"), (IFile**)&mSecureAppDataDir));
        FAIL_RETURN(CFile::New(dataDir, String("app-private"), (IFile**)&mDrmAppPrivateInstallDir));

        if (mInstaller == NULL) {
            // Make sure these dirs exist, when we are running in
            // the simulator.
            // Make a wide-open directory for random misc stuff.
            AutoPtr<IFile> miscDir;
            FAIL_RETURN(CFile::New(dataDir, String("misc"), (IFile**)&miscDir));
            Boolean succeeded = FALSE;
            miscDir->Mkdirs(&succeeded);
            mAppDataDir->Mkdirs(&succeeded);
            mSecureAppDataDir->Mkdirs(&succeeded);
            mDrmAppPrivateInstallDir->Mkdirs(&succeeded);
        }

        ReadPermissions();

        mRestoredSettings = mSettings->ReadLP();
        Int64 startTime = SystemClock::GetUptimeMillis();

//	        EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_SYSTEM_SCAN_START,
//	                startTime);

        // Set flag to monitor and not change apk file paths when
        // scanning install directories.
        Int32 scanMode = SCAN_MONITOR | SCAN_NO_PATHS;
        if (mNoDexOpt) {
            Slogger::W(TAG, "Running ENG build: no pre-dexopt!");
            scanMode |= SCAN_NO_DEX;
        }

        HashSet<String> libFiles;

        FAIL_RETURN(CFile::New(Environment::GetRootDirectory(), String("framework"), (IFile**)&mFrameworkDir));
        FAIL_RETURN(CFile::New(dataDir, String("dalvik-cache"), (IFile**)&mDalvikCacheDir));

        if (mInstaller != NULL) {
            Boolean didDexOpt = FALSE;

            /**
             * Out of paranoia, ensure that everything in the boot class
             * path has been dexed.
             */
//	            String bootClassPath;
//	            System::GetProperty("java->mBoot.class->mPath", &bootClassPath);
//	            if (!bootClassPath.IsNull()) {
//	                AutoStringArray paths;
//	                SplitString(bootClassPath, ':', (ArrayOf<String>**)&paths);
//	                for (Int32 i=0; i < paths->GetLength(); i++) {
//	                    try {
//	                        if (dalvik->mSystem.DexFile::IsDexOptNeeded(paths[i])) {
//	                            libFiles->Add(paths[i]);
//	                            mInstaller->Dexopt(paths[i], Process::SYSTEM_UID, TRUE);
//	                            didDexOpt = TRUE;
//	                        }
//	                    } catch (FileNotFoundException e) {
//	                        Slogger::W(TAG, "Boot class path not found: " + paths[i]);
//	                    } catch (IOException e) {
//	                        Slogger::W(TAG, "Exception reading boot class path: " + paths[i], e);
//	                    }
//	                }
//	            } else {
//	                Slogger::W(TAG, "No BOOTCLASSPATH found!");
//	            }

//	            /**
//	             * Also ensure all external libraries have had dexopt run on them.
//	             */
//	            if (mSharedLibraries.size() > 0) {
//	                Iterator<String> libs = mSharedLibraries.values().iterator();
//	                while (libs.hasNext()) {
//	                    String lib = libs.next();
//	                    try {
//	                        if (dalvik.system.DexFile.isDexOptNeeded(lib)) {
//	                            libFiles.add(lib);
//	                            mInstaller.dexopt(lib, Process.SYSTEM_UID, true);
//	                            didDexOpt = true;
//	                        }
//	                    } catch (FileNotFoundException e) {
//	                        Slog.w(TAG, "Library not found: " + lib);
//	                    } catch (IOException e) {
//	                        Slog.w(TAG, "Exception reading library: " + lib, e);
//	                    }
//	                }
//	            }

            // Gross hack for now: we know this file doesn't contain any
            // code, so don't dexopt it to avoid the resulting log spew.
            String path;
            mFrameworkDir->GetPath(&path);
            libFiles.Insert(path + "/framework-res.apk");

            /**
             * And there are a number of commands implemented in Java, which
             * we currently need to do the dexopt on so that they can be
             * run from a non-root shell.
             */
//	            String[] frameworkFiles = mFrameworkDir.list();
//	            if (frameworkFiles != null) {
//	                for (int i=0; i<frameworkFiles.length; i++) {
//	                    File libPath = new File(mFrameworkDir, frameworkFiles[i]);
//	                    String path = libPath.getPath();
//	                    // Skip the file if we alrady did it.
//	                    if (libFiles.contains(path)) {
//	                        continue;
//	                    }
//	                    // Skip the file if it is not a type we want to dexopt.
//	                    if (!path.endsWith(".apk") && !path.endsWith(".jar")) {
//	                        continue;
//	                    }
//	                    try {
//	                        if (dalvik.system.DexFile.isDexOptNeeded(path)) {
//	                            mInstaller.dexopt(path, Process.SYSTEM_UID, true);
//	                            didDexOpt = true;
//	                        }
//	                    } catch (FileNotFoundException e) {
//	                        Slog.w(TAG, "Jar not found: " + path);
//	                    } catch (IOException e) {
//	                        Slog.w(TAG, "Exception reading jar: " + path, e);
//	                    }
//	                }
//	            }
//
//	            if (didDexOpt) {
//	                // If we had to do a dexopt of one of the previous
//	                // things, then something on the system has changed.
//	                // Consider this significant, and wipe away all other
//	                // existing dexopt files to ensure we don't leave any
//	                // dangling around.
//	                String[] files = mDalvikCacheDir.list();
//	                if (files != null) {
//	                    for (int i=0; i<files.length; i++) {
//	                        String fn = files[i];
//	                        if (fn.startsWith("data@app@")
//	                                || fn.startsWith("data@app-private@")) {
//	                            Slog.i(TAG, "Pruning dalvik file: " + fn);
//	                            (new File(mDalvikCacheDir, fn)).delete();
//	                        }
//	                    }
//	                }
//	            }
        }

        // Find base frameworks (resource packages without code).
        String path;
        mFrameworkDir->GetPath(&path);
        mFrameworkInstallObserver
            = new AppDirObserver(path, OBSERVER_EVENTS, TRUE, this);
        mFrameworkInstallObserver->StartWatching();
        ScanDirLI(mFrameworkDir, CapsuleParser::PARSE_IS_SYSTEM
            | CapsuleParser::PARSE_IS_SYSTEM_DIR,
            scanMode | SCAN_NO_DEX, 0);

        // Collect all system packages.
        FAIL_RETURN(CFile::New(Environment::GetRootDirectory(),
                String("app"), (IFile**)&mSystemAppDir));
        mSystemAppDir->GetPath(&path);
        mSystemInstallObserver
            = new AppDirObserver(path, OBSERVER_EVENTS, TRUE, this);
        mSystemInstallObserver->StartWatching();
        ScanDirLI(mSystemAppDir, CapsuleParser::PARSE_IS_SYSTEM
            | CapsuleParser::PARSE_IS_SYSTEM_DIR, scanMode, 0);

        // Collect all vendor packages.
        FAIL_RETURN(CFile::New(String("/vendor/app"), (IFile**)&mVendorAppDir));
        mVendorAppDir->GetPath(&path);
        mVendorInstallObserver
            = new AppDirObserver(path, OBSERVER_EVENTS, TRUE, this);
        mVendorInstallObserver->StartWatching();
        ScanDirLI(mVendorAppDir, CapsuleParser::PARSE_IS_SYSTEM
            | CapsuleParser::PARSE_IS_SYSTEM_DIR, scanMode, 0);

        if (mInstaller != NULL) {
            if (DEBUG_UPGRADE) Logger::V(TAG, "Running installd update commands");
            mInstaller->MoveFiles();
        }

        // Prune any system packages that no longer exist.
        HashMap<String, AutoPtr<CapsuleSetting> >::Iterator csit = mSettings->mCapsules.Begin();
        while (csit != mSettings->mCapsules.End()) {
            CapsuleSetting* cs = csit->mSecond;
            if ((cs->mCapFlags & ApplicationInfo_FLAG_SYSTEM) != 0
                && (mCapsules.Find(cs->mName) == mCapsules.End())
                && (mSettings->mDisabledSysCapsules.Find(cs->mName)
                    == mSettings->mDisabledSysCapsules.End())) {
                mSettings->mCapsules.Erase(csit++);
                StringBuffer msg = StringBuffer("System capsule ")
                    + cs->mName + " no Int64er exists; wiping its data";
                ReportSettingsProblem(5 /*Log::WARN*/, (String)(const char*)msg);
                if (mInstaller != NULL) {
                    // XXX how to set useEncryptedFSDir for packages that
                    // are not encrypted?
                    mInstaller->Remove(cs->mName, TRUE);
                }
            }
            else {
                csit++;
            }
        }

        FAIL_RETURN(CFile::New(dataDir, String("app"), (IFile**)&mAppInstallDir));
        if (mInstaller == NULL) {
            // Make sure these dirs exist, when we are running in
            // the simulator.
            Boolean succeeded = FALSE;
            mAppInstallDir->Mkdirs(&succeeded); // ScanDirLI() assumes this dir exists
        }
        //look for any incomplete package installations
        //todo: deleteCapsList memory leak
        List< AutoPtr<CapsuleSetting> >* deleteCapsList = mSettings->GetListOfIncompleteInstallCapsules();
        //clean up list
        List< AutoPtr<CapsuleSetting> >::Iterator dclit;
        for (dclit = deleteCapsList->Begin();
             dclit != deleteCapsList->End(); ++dclit) {
            //clean up here
            CleanupInstallFailedCapsule(*dclit);
        }
        //delete tmp files
        DeleteTempCapsuleFiles();

//	        EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_DATA_SCAN_START,
//	                SystemClock.uptimeMillis());

        mAppInstallDir->GetPath(&path);
        mAppInstallObserver
            = new AppDirObserver(path, OBSERVER_EVENTS, FALSE, this);
        mAppInstallObserver->StartWatching();
        ScanDirLI(mAppInstallDir, 0, scanMode, 0);

        mDrmAppPrivateInstallDir->GetPath(&path);
        mDrmAppInstallObserver
            = new AppDirObserver(path, OBSERVER_EVENTS, FALSE, this);
        mDrmAppInstallObserver->StartWatching();
        ScanDirLI(mDrmAppPrivateInstallDir.Get(), CapsuleParser::PARSE_FORWARD_LOCK,
            scanMode, 0);

//	        EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_SCAN_END,
//	                SystemClock.uptimeMillis());
        Slogger::I(TAG, StringBuffer("Time to scan capsules: ")
                + (Int32)(((SystemClock::GetUptimeMillis() - startTime) / 1000.0f))
                + " seconds");

        // If the platform SDK has changed since the last time we booted,
        // we need to re-grant app permission to catch any new ones that
        // appear.  This is really a hack, and means that apps can in some
        // cases get permissions that the user didn't initially explicitly
        // allow...  it would be nice to have some better way to handle
        // this situation.
        Boolean regrantPermissions = mSettings->mInternalSdkPlatform != mSdkVersion;
        if (regrantPermissions) Slogger::I(TAG, StringBuffer("Platform changed from ")
                + mSettings->mInternalSdkPlatform + " to " + mSdkVersion
                + "; regranting permissions for Int32ernal storage");
        mSettings->mInternalSdkPlatform = mSdkVersion;

        UpdatePermissionsLP(String(NULL), NULL, TRUE, regrantPermissions, regrantPermissions);

        mSettings->WriteLP();

//	        EventLog.writeEvent(EventLogTags.BOOT_PROGRESS_PMS_READY,
//	                SystemClock.uptimeMillis());

        // Now after opening every single application zip, make sure they
        // are all flushed.  Not really needed, but keeps things nice and
        // tidy.
//	        Runtime.getRuntime().gc();
    }
    return NOERROR;
}

void CCapsuleManagerService::CleanupInstallFailedCapsule(
    /* [in] */ CapsuleSetting* cs)
{
    Slogger::I(TAG, StringBuffer("Cleaning up incompletely installed app: ") + cs->mName);
    if (mInstaller != NULL) {
        Boolean useSecureFS = UseEncryptedFilesystemForCapsule(cs->mCap);
        Int32 retCode = mInstaller->Remove(cs->mName, useSecureFS);
        if (retCode < 0) {
            Slogger::W(TAG,
                StringBuffer("Couldn't remove app data directory for capsule: ")
                + cs->mName + ", retcode=" + retCode);
        }
    }
    else {
        //for emulator
        HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(cs->mName);
        assert(it != mCapsules.End());
        CapsuleParser::Capsule* cap = it->mSecond;
        String capDataDir;
        cap->mApplicationInfo->GetDataDir(&capDataDir);
        AutoPtr<IFile> dataDir;
        ASSERT_SUCCEEDED(CFile::New(capDataDir, (IFile**)&dataDir));
        Boolean succeeded = FALSE;
        dataDir->Delete(&succeeded);
    }
    if (cs->mCodePath != NULL) {
        Boolean succeeded = FALSE;
        cs->mCodePath->Delete(&succeeded);
        if (!succeeded) {
            Slogger::W(TAG, StringBuffer("Unable to remove old code file: ")
                + cs->mCodePath);
        }
    }
    if (cs->mResourcePath != NULL) {
        Boolean succeeded = FALSE, same = FALSE;
        cs->mResourcePath->Delete(&succeeded);
        cs->mResourcePath->Equals(cs->mCodePath, &same);
        if (!succeeded && !same) {
            Slogger::W(TAG, StringBuffer("Unable to remove old code file: ")
                + cs->mResourcePath);
        }
    }
    mSettings->RemoveCapsuleLP(cs->mName);
}

void CCapsuleManagerService::ReadPermissions()
{
	// Read permissions from .../etc/permission directory.
	AutoPtr<IFile> libraryDir;
	CFile::New(Environment::GetRootDirectory(),
	        String("etc/permissions"), (IFile**)&libraryDir);
	Boolean isExist = FALSE, isDir = FALSE;
	libraryDir->Exists(&isExist);
	libraryDir->IsDirectory(&isDir);
	if (!isExist || !isDir) {
		Slogger::W(TAG, StringBuffer("No directory ") + libraryDir + ", skipping");
		return;
	}
	Boolean isReadable = FALSE;
	libraryDir->CanRead(&isReadable);
	if (!isReadable) {
		Slogger::W(TAG, StringBuffer("Directory ") + libraryDir + " cannot be read");
		return;
	}

	// Iterate over the files in the directory and scan .xml files
	AutoPtr<IObjectContainer> files;
	AutoPtr<IObjectEnumerator> filesEmu;
	libraryDir->ListFiles((IObjectContainer**)&files);
	files->GetObjectEnumerator((IObjectEnumerator**)&filesEmu);
	Boolean hasNext = FALSE;
	while (filesEmu->MoveNext(&hasNext), hasNext) {
	    AutoPtr<IFile> f;
		filesEmu->Current((IInterface**)&f);

		String path;
        f->GetPath(&path);

		// We'll read platform->mXml last
		if (path.EndWith("etc/permissions/platform.xml")) {
			continue;
		}

		if (!path.EndWith(".xml")) {
			Slogger::I(TAG, StringBuffer("Non-xml file ")
               + f + " in " + libraryDir + " directory, ignoring");
			continue;
		}

        Boolean isReadable = FALSE;
        f->CanRead(&isReadable);
		if (!isReadable) {
			Slogger::W(TAG, StringBuffer("Permissions library file ")
               + f + " cannot be read");
			continue;
		}

		ReadPermissionsFromXml(f);
	}

	// Read permissions from .../etc/permissions/platform.xml last so it will take precedence
	AutoPtr<IFile> permFile;
    CFile::New(Environment::GetRootDirectory(),
            String("etc/permissions/platform.xml"), (IFile**)&permFile);
    ReadPermissionsFromXml(permFile);
}

void CCapsuleManagerService::ReadPermissionsFromXml(
    /* [in] */ IFile* permFile)
{
    AutoPtr<IInputStreamReader> permReader;
    if (FAILED(CFileReader::New(permFile, (IInputStreamReader**)&permReader))) {
        Slogger::W(TAG, StringBuffer("Couldn't find or open permissions file ") + permFile);
        return;
    }

    ECode ec = NOERROR;
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    parser->SetInput(permReader);

    ec = XmlUtils::BeginDocument(parser, "permissions");
    if (FAILED(ec)) goto Exception;

    while (TRUE) {
        XmlUtils::NextElement(parser);
        Int32 type = 0;
        if (parser->GetEventType(&type), type == IXmlPullParser_END_DOCUMENT) {
            break;
        }

        String name;
        parser->GetName(&name);
        if (CString("group").Equals(name)) {
            String gidStr;
            parser->GetAttributeValueEx(NULL, "gid", &gidStr);
            if (!gidStr.IsNull()) {
                Int32 gid = gidStr.ToInt32();
                mGlobalGids = AppendInt(mGlobalGids.Get(), gid);
            }
            else {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<group> without gid at ") + des);
            }

            XmlUtils::SkipCurrentTag(parser);
            continue;
        }
        else if (CString("permission").Equals(name)) {
            String perm;
            parser->GetAttributeValueEx(NULL, "name", &perm);
            if (perm.IsNull()) {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<permission> without name at ") + des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            ReadPermission(parser, perm);

        }
        else if (CString("assign-permission").Equals(name)) {
            String perm;
            parser->GetAttributeValueEx(NULL, "name", &perm);
            if (perm.IsNull()) {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<assign-permission> without name at ") + des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            String uidStr;
            parser->GetAttributeValueEx(NULL, "uid", &uidStr);
            if (uidStr.IsNull()) {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<assign-permission> without uid at ") + des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            Int32 uid = Process::GetUidForName(uidStr);
            if (uid < 0) {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<assign-permission> with unknown uid \"")
                        + uidStr + "\" at " + des);
                XmlUtils::SkipCurrentTag(parser);
                continue;
            }
            HashSet<String>* perms = NULL;
            HashMap<Int32, HashSet<String>* >::Iterator it = mSystemPermissions.Find(uid);
            if (it != mSystemPermissions.End()) {
                perms = it->mSecond;
            }
            if (perms == NULL) {
                perms = new HashSet<String>();
                mSystemPermissions[uid] = perms;
            }
            perms->Insert(perm);
            XmlUtils::SkipCurrentTag(parser);
        }
        else if (CString("library").Equals(name)) {
            String lname;
            parser->GetAttributeValueEx(NULL, "name", &lname);
            String lfile;
            parser->GetAttributeValueEx(NULL, "file", &lfile);
            if (lname.IsNull()) {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<library> without name at ") + des);
            }
            else if (lfile.IsNull()) {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<library> without file at ") + des);
            }
            else {
                //Logger::I(TAG, "Got library " + lname + " in " + lfile);
                mSharedLibraries[lname] = lfile;
            }
            XmlUtils::SkipCurrentTag(parser);
            continue;

        }
        else if (CString("feature").Equals(name)) {
            String fname;
            parser->GetAttributeValueEx(NULL, "name", &fname);
            if (fname.IsNull()) {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<feature> without name at ") + des);
            }
            else {
                //Logger::I(TAG, "Got feature " + fname);
                AutoPtr<IFeatureInfo> fi;
                ec = CFeatureInfo::New((IFeatureInfo**)&fi);
                if (FAILED(ec)) goto Exception;
                fi->SetName(fname);
                mAvailableFeatures[fname] = fi;
            }
            XmlUtils::SkipCurrentTag(parser);
            continue;
        }
        else {
            XmlUtils::SkipCurrentTag(parser);
            continue;
        }
    }

    return;

Exception:
    Slogger::W(TAG, StringBuffer("Got execption parsing permissions.") + ec);
}

void CCapsuleManagerService::ReadPermission(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ const String& name)
{
    BasePermission* bp = NULL;
    HashMap<String, BasePermission*>::Iterator it = mSettings->mPermissions.Find(name);
    if (it != mSettings->mPermissions.End()) {
        bp = it->mSecond;
    }
    if (bp == NULL) {
        bp = new BasePermission(name, String(NULL), BasePermission::TYPE_BUILTIN);
        mSettings->mPermissions[name] = bp;
    }

    Int32 type = 0, outerDepth = 0, depth = 0;
    parser->GetDepth(&outerDepth);
    while ((parser->Next(&type), type != IXmlPullParser_END_DOCUMENT)
       && (type != IXmlPullParser_END_TAG
           || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser_END_TAG || type == IXmlPullParser_TEXT) {
            continue;
        }

        String tagName;
        parser->GetName(&tagName);
        if (CString("group").Equals(tagName)) {
            String gidStr;
            parser->GetAttributeValueEx(NULL, "gid", &gidStr);
            if (!gidStr.IsNull()) {
                Int32 gid = Process::GetGidForName(gidStr);
//                bp->mGids = AppendInt(bp->mGids.Get(), gid);
            } else {
                String des;
                parser->GetPositionDescription(&des);
                Slogger::W(TAG, StringBuffer("<group> without gid at ") + des);
            }
        }
        XmlUtils::SkipCurrentTag(parser);
    }
}

ArrayOf<Int32>* CCapsuleManagerService::AppendInt(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ Int32 val)
{
    if (cur == NULL) {
        cur = ArrayOf<Int32>::Alloc(1);
        if (cur == NULL) return NULL;
        (*cur)[0] = val;
        return cur;
    }

    const Int32 N = cur->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if ((*cur)[i] == val) {
            return cur;
        }
    }

    ArrayOf<Int32>* ret = ArrayOf<Int32>::Alloc(N + 1);
    memcpy(ret->GetPayload(), cur->GetPayload(), N * sizeof(Int32));
    (*ret)[N] = val;
    //free old array
    ArrayOf<Int32>::Free(cur);
    return ret;
}

ArrayOf<Int32>* CCapsuleManagerService::AppendInts(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ ArrayOf<Int32>* add)
{
    if (add == NULL) return cur;
    if (cur == NULL) return add;
    const Int32 N = add->GetLength();
    for (Int32 i=0; i < N; i++) {
        cur = AppendInt(cur, (*add)[i]);
    }
    return cur;
}

ArrayOf<Int32>* CCapsuleManagerService::RemoveInt(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ Int32 val)
{
    if (cur == NULL) {
        return NULL;
    }
    const Int32 N = cur->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if ((*cur)[i] == val) {
            ArrayOf<Int32>* ret = ArrayOf<Int32>::Alloc(N - 1); // new Int32[N-1];
            if (!ret) return NULL;
            if (i > 0) {
                memcpy(ret->GetPayload(), cur->GetPayload(), i * sizeof(Int32));
            }
            if (i < (N - 1)) {
                memcpy(ret->GetPayload() + i,
                    cur->GetPayload() + i + 1, (N - i - 1) * sizeof(Int32));
            }
            //free old array
            ArrayOf<Int32>::Free(cur);
            return ret;
        }
    }
    return cur;
}

ArrayOf<Int32>* CCapsuleManagerService::RemoveInts(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ ArrayOf<Int32>* rem)
{
    if (rem == NULL) return cur;
    if (cur == NULL) return cur;
    const Int32 N = rem->GetLength();
    for (Int32 i = 0; i < N; i++) {
        cur = RemoveInt(cur, (*rem)[i]);
    }
    return cur;
}

AutoPtr<ICapsuleInfo> CCapsuleManagerService::GenerateCapsuleInfo(
    /* [in] */ CapsuleParser::Capsule* c,
    /* [in] */ Int32 flags)
{
    if ((flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0) {
        // The package has been uninstalled but has retained data and resources.
        return CapsuleParser::GenerateCapsuleInfo(c, NULL, flags, 0, 0);
    }
    CapsuleSetting* cs = (CapsuleSetting*)c->mExtras->Probe(EIID_CapsuleSetting);
    if (cs == NULL) {
        return NULL;
    }
    GrantedPermissions* gp = cs->mSharedUser != NULL
        ? (GrantedPermissions*)cs->mSharedUser
        : (GrantedPermissions*)cs;
    return CapsuleParser::GenerateCapsuleInfo(c, gp->mGids, flags,
            cs->mFirstInstallTime, cs->mLastUpdateTime);
}

ECode CCapsuleManagerService::GetCapsuleInfo(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ ICapsuleInfo** capInfo)
{
    VALIDATE_NOT_NULL(capInfo);
    VALIDATE_STRING_NOT_NULL(capsuleName);

    Mutex::Autolock lock(mCapsulesLock);

    HashMap<String, CapsuleParser::Capsule*>::Iterator itor = mCapsules.Find(capsuleName);
    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getPackageInfo ")
            + capsuleName + ": " + (Int32)(*itor).mSecond);
    }
    AutoPtr<ICapsuleInfo> ci;
    if (itor != mCapsules.End()) {
        ci = GenerateCapsuleInfo(itor->mSecond, flags);
        *capInfo = ci;
    }
    if((flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0) {
        ci = GenerateCapsuleInfoFromSettingsLP(capsuleName, flags);
        *capInfo = ci;
    }
    *capInfo = NULL;
    if (*capInfo != NULL) (*capInfo)->AddRef();
    return NOERROR;
}

ECode CCapsuleManagerService::CurrentToCanonicalCapsuleNames(
    /* [in] */ const ArrayOf<String>& names,
    /* [out, callee] */ ArrayOf<String>** pnames)
{
    VALIDATE_NOT_NULL(pnames);

    ArrayOf<String>* out = ArrayOf<String>::Alloc(names.GetLength());
    if (!out) return E_OUT_OF_MEMORY_ERROR;
    {
        Mutex::Autolock lock(mCapsulesLock);
        for (Int32 i = names.GetLength() - 1; i >= 0; i--) {
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator itor =
                    mSettings->mCapsules.Find(names[i]);
            (*out)[i] = itor != mSettings->mCapsules.End() && !itor->mSecond->mRealName.IsNull()
                ? itor->mSecond->mRealName : names[i];
        }
    }
    *pnames = out;
    return NOERROR;
}

ECode CCapsuleManagerService::CanonicalToCurrentCapsuleNames(
    /* [in] */ const ArrayOf<String>& names,
    /* [out, callee] */ ArrayOf<String>** pnames)
{
    VALIDATE_NOT_NULL(pnames);

    ArrayOf<String>* out = ArrayOf<String>::Alloc(names.GetLength());
    if (!out) return E_OUT_OF_MEMORY_ERROR;
    {
        Mutex::Autolock lock(mCapsulesLock);
        for (Int32 i = names.GetLength() - 1; i >= 0; i--) {
            HashMap<String, String>::Iterator itor =
                                mSettings->mRenamedCapsules.Find(names[i]);
            (*out)[i] = itor != mSettings->mRenamedCapsules.End()
                ? itor->mSecond : names[i];
        }
    }
    *pnames = out;
    return NOERROR;
}

ECode CCapsuleManagerService::GetCapsuleUid(
    /* [in] */ const String& capsuleName,
    /* [out] */ Int32* uid)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);
    VALIDATE_NOT_NULL(uid);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, CapsuleParser::Capsule*>::Iterator myItor = mCapsules.Find(capsuleName);
    if(myItor != mCapsules.End()) {
        myItor->mSecond->mApplicationInfo->GetUid(uid);
        return NOERROR;
    }
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator itor = mSettings->mCapsules.Find(capsuleName);
    if( itor == mSettings->mCapsules.End()
        || itor->mSecond == NULL
        || itor->mSecond->mCap == NULL
        || itor->mSecond->mCap->mApplicationInfo == NULL) {
        *uid = -1;
        return NOERROR;
    }
    itor->mSecond->mCap->mApplicationInfo->GetUid(uid);
    return NOERROR;
}

ECode CCapsuleManagerService::GetCapsuleGids(
    /* [in] */ const String& capsuleName,
    /* [out, callee] */ ArrayOf<Int32>** gids)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);
    VALIDATE_NOT_NULL(gids);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, CapsuleParser::Capsule*>::Iterator itor = mCapsules.Find(capsuleName);
    if (Config::LOGV) {
        Logger::V(TAG, StringBuffer("getPackageGids")
            + capsuleName + ": " + (Int32)(*itor).mSecond);
    }
    if (itor != mCapsules.End()) {
        CapsuleSetting* cs = (CapsuleSetting*)(itor->mSecond->mExtras.Get());
        SharedUserSetting* suid = cs->mSharedUser;
        *gids = suid != NULL ? suid->mGids->Clone() : cs->mGids->Clone();
        return NOERROR;
    }
    // stupid thing to indicate an error.
    *gids = ArrayOf<Int32>::Alloc(0);
    return NOERROR;
}

AutoPtr<IPermissionInfo> CCapsuleManagerService::GeneratePermissionInfo(
    /* [in] */ BasePermission* bp,
    /* [in] */ Int32 flags)
{
    assert(bp);

    if (bp->mPerm != NULL) {
        return CapsuleParser::GeneratePermissionInfo(bp->mPerm, flags);
    }
    AutoPtr<IPermissionInfo> pi;
    ASSERT_SUCCEEDED(CPermissionInfo::New((IPermissionInfo**)&pi));
    pi->SetName(bp->mName);
    pi->SetCapsuleName(bp->mSourceCapsule);
    AutoPtr<ICharSequence> label;
    CStringWrapper::New(bp->mName, (ICharSequence**)&label);
    pi->SetNonLocalizedLabel(label);
    pi->SetProtectionLevel(bp->mProtectionLevel);
    return pi;
}

ECode CCapsuleManagerService::GetPermissionInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionInfo** info)
{
    VALIDATE_STRING_NOT_NULL(name);
    VALIDATE_NOT_NULL(info);

    Mutex::Autolock lock(mCapsulesLock);
    HashMap<String, BasePermission*>::Iterator itor =
            mSettings->mPermissions.Find(name);
    if (itor != mSettings->mPermissions.End()) {
        AutoPtr<IPermissionInfo> pi = GeneratePermissionInfo(itor->mSecond, flags);
        *info = pi;
        if (*info != NULL) (*info)->AddRef();
        return NOERROR;
    }
    *info = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::QueryPermissionsByGroup(
    /* [in] */ const String& group,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
    *infos = NULL;

    Mutex::Autolock lock(mCapsulesLock);

    AutoPtr<IObjectContainer> out;
    FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&out));

    HashMap<String, BasePermission*>::Iterator itor;
    for (itor = mSettings->mPermissions.Begin();
        itor != mSettings->mPermissions.End(); ++itor) {

        AutoPtr<IPermissionInfo> info;
        BasePermission* p = itor->mSecond;

        if (group.IsNull()) {
            if (p->mPerm == NULL) {
                info = GeneratePermissionInfo(p, flags);
                out->Add(info);
            }
            else {
                String pGroup;
                p->mPerm->mInfo->GetGroup(&pGroup);
                if (pGroup.IsNull()) {
                    info = GeneratePermissionInfo(p, flags);
                    out->Add(info);
                }
            }
        }
        else {
            String pGroup;
            if (p->mPerm != NULL &&
                    (p->mPerm->mInfo->GetGroup(&pGroup), group.Equals(pGroup))) {
                info = CapsuleParser::GeneratePermissionInfo(p->mPerm, flags);
                out->Add(info.Get());
            }
        }
    }

    Int32 count = out->GetObjectCount(&count);
    if (count > 0) {
        *infos = out.Get();
        (*infos)->AddRef();
        return NOERROR;
    }
    HashMap<String, CapsuleParser::PermissionGroup*>::Iterator it =
        mPermissionGroups.Find(group);
    if (it != mPermissionGroups.End()) {
        *infos = out.Get();
        (*infos)->AddRef();
        return NOERROR;
    }
    *infos = NULL;
    return E_RUNTIME_EXCEPTION;
}

ECode CCapsuleManagerService::GetPermissionGroupInfo(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IPermissionGroupInfo** info)
{
    VALIDATE_STRING_NOT_NULL(name);
    VALIDATE_NOT_NULL(info);

    Mutex::Autolock lock(mCapsulesLock);

    HashMap<String, CapsuleParser::PermissionGroup*>::Iterator itor
            = mPermissionGroups.Find(name);
    AutoPtr<IPermissionGroupInfo> pgi = CapsuleParser::GeneratePermissionGroupInfo(
            itor != mPermissionGroups.End() ? itor->mSecond : NULL, flags);
    *info = pgi;
    INTERFACE_ADDREF(*info);
    return NOERROR;
}

ECode CCapsuleManagerService::GetAllPermissionGroups(
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    Mutex::Autolock lock(mCapsulesLock);

    AutoPtr<IObjectContainer> out;
    FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&out));

    HashMap<String, CapsuleParser::PermissionGroup*>::Iterator itor;
    for (itor = mPermissionGroups.Begin();
        itor != mPermissionGroups.End(); ++itor) {
        AutoPtr<IPermissionGroupInfo> g =
            CapsuleParser::GeneratePermissionGroupInfo(itor->mSecond, flags);
        out->Add(g);
    }

    *infos = out.Get();
    (*infos)->AddRef();
    return NOERROR;
}

AutoPtr<IApplicationInfo> CCapsuleManagerService::GenerateApplicationInfoFromSettingsLP(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags)
{
    CapsuleSetting* cs = NULL;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it
            = mSettings->mCapsules.Find(capsuleName);
    if (it != mSettings->mCapsules.End()) {
        cs = it->mSecond;
    }

    if(cs != NULL) {
        if(cs->mCap == NULL) {
            AutoPtr<ICapsuleInfo> capInfo = GenerateCapsuleInfoFromSettingsLP(
                    capsuleName, flags);
            if(capInfo != NULL) {
                AutoPtr<IApplicationInfo> appInfo;
                capInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                return appInfo;
            }
            return NULL;
        }
        return CapsuleParser::GenerateApplicationInfo(cs->mCap, flags);
    }

    return NULL;
}

AutoPtr<ICapsuleInfo> CCapsuleManagerService::GenerateCapsuleInfoFromSettingsLP(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags)
{
    CapsuleSetting* cs = NULL;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it
            = mSettings->mCapsules.Find(capsuleName);
    if (it != mSettings->mCapsules.End()) {
        cs = it->mSecond;
    }
    if(cs != NULL) {
        if(cs->mCap == NULL) {
            cs->mCap = new CapsuleParser::Capsule(capsuleName);
            cs->mCap->mApplicationInfo->SetCapsuleName(capsuleName);
            cs->mCap->mApplicationInfo->SetFlags(cs->mCapFlags);
            cs->mCap->mApplicationInfo->SetPublicSourceDir(cs->mResourcePathString);
            cs->mCap->mApplicationInfo->SetSourceDir(cs->mCodePathString);
            String dataDir;
            GetDataPathForCapsule(cs->mCap)->GetPath(&dataDir);
            cs->mCap->mApplicationInfo->SetDataDir(dataDir);
            cs->mCap->mApplicationInfo->SetNativeLibraryDir(cs->mNativeLibraryPathString);
            cs->mCap->mSetEnabled = cs->mEnabled;
        }
        return GenerateCapsuleInfo(cs->mCap, flags);
    }
    return NULL;
}

ECode CCapsuleManagerService::GetApplicationInfo(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IApplicationInfo** appInfo)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);
    VALIDATE_NOT_NULL(appInfo);
    *appInfo = NULL;

    Mutex::Autolock lock(mCapsulesLock);

    CapsuleParser::Capsule* c = NULL;
    HashMap<String, CapsuleParser::Capsule*>::Iterator it
             = mCapsules.Find(capsuleName);
    if (it != mCapsules.End()) {
        c = it->mSecond;
    }

    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getApplicationInfo ") + capsuleName + ": " + c);
    }

    if (c != NULL) {
        // Note: isEnabledLP() does not apply here - always return info
        AutoPtr<IApplicationInfo> cinfo = CapsuleParser::GenerateApplicationInfo(c, flags);
        *appInfo = cinfo;
        (*appInfo)->AddRef();
        return NOERROR;
    }

    if (capsuleName.Equals("elastos") ||
        capsuleName.Equals("system")) {
        *appInfo = mElastosApplication;
        (*appInfo)->AddRef();
        return NOERROR;
    }

    if((flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0) {
        AutoPtr<IApplicationInfo> cinfo = GenerateApplicationInfoFromSettingsLP(capsuleName, flags);
        *appInfo = cinfo;
        (*appInfo)->AddRef();
        return NOERROR;
    }

    *appInfo = NULL;
    return E_FAIL;
}

ECode CCapsuleManagerService::FreeStorageAndNotify(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ ICapsuleDataObserver* observer)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*android.Manifest.permission.CLEAR_APP_CACHE*/, String(NULL));
    // Queue up an async operation since clearing cache may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Int64, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleFreeStorageAndNotify;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt64(freeStorageSize);
    params->WriteInt32((Handle32)observer);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CCapsuleManagerService::HandleFreeStorageAndNotify(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ ICapsuleDataObserver* observer)
{
//	    mHandler->RemoveCallbacks(this);
    Int32 retCode = -1;
    if (mInstaller != NULL) {
        retCode = mInstaller->FreeCache(freeStorageSize);
        if (retCode < 0) {
            Slogger::W(TAG, "Couldn't clear application caches");
        }
    } //end if mInstaller
    if (observer != NULL) {
        if (FAILED(observer->OnRemoveCompleted(String(NULL), (retCode >= 0)))) {
            Slogger::W(TAG, "RemoveException when invoking call back");
        }
    }
}

ECode CCapsuleManagerService::FreeStorage(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IIntentSender* pi)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*android.Manifest.permission.CLEAR_APP_CACHE*/, String(NULL));
    // Queue up an async operation since clearing cache may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Int64, IIntentSender*);
    pHandlerFunc = &CCapsuleManagerService::HandleFreeStorage;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt64(freeStorageSize);
    params->WriteInt32((Handle32)pi);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CCapsuleManagerService::HandleFreeStorage(
    /* [in] */ Int64 freeStorageSize,
    /* [in] */ IIntentSender* pi)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Int64, IIntentSender*);
    pHandlerFunc = &CCapsuleManagerService::HandleFreeStorage;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
//	    mHandler->RemoveCallbacks(this);
    Int32 retCode = -1;
    if (mInstaller != NULL) {
        retCode = mInstaller->FreeCache(freeStorageSize);
        if (retCode < 0) {
            Slogger::W(TAG, "Couldn't clear application caches");
        }
    }

    if (pi != NULL) {
        // Callback via pending intent
        Int32 code = (retCode >= 0) ? 1 : 0;
        if (FAILED(pi->SendIntent(NULL, code, NULL, NULL, NULL))) {
            Slogger::I(TAG, "Failed to send pending intent");
        }
    }
}

ECode CCapsuleManagerService::GetActivityInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** ai)
{
    VALIDATE_NOT_NULL(ai);

    Mutex::Autolock lock(mCapsulesLock);

    CapsuleParser::Activity* a = NULL;
    HashMap<AutoPtr<IComponentName>, CapsuleParser::Activity*>::Iterator it =
            mActivities->mActivities.Find(component);
    if (it != mActivities->mActivities.End()) {
        a = it->mSecond;
    }
    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getActivityInfo ") + component + ": " + a);
    }
    if (a != NULL && mSettings->IsEnabledLP(
            (IComponentInfo*)(IActivityInfo*)a->mInfo.Get(), flags)) {
        AutoPtr<IActivityInfo> info = CapsuleParser::GenerateActivityInfo(a, flags);
        *ai = info;
        if (*ai != NULL) (*ai)->AddRef();
        return NOERROR;
    }
    Boolean isEqual = FALSE;
    mResolveComponentName->Equals(component, &isEqual);
    if (isEqual) {
        *ai = (IActivityInfo*)mResolveActivity.Get();
        if (*ai != NULL) (*ai)->AddRef();
        return NOERROR;
    }

    *ai = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CCapsuleManagerService::GetReceiverInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IActivityInfo** ai)
{
    VALIDATE_NOT_NULL(ai);

    Mutex::Autolock lock(mCapsulesLock);

    CapsuleParser::Activity* a = NULL;
    HashMap<AutoPtr<IComponentName>, CapsuleParser::Activity*>::Iterator it =
            mReceivers->mActivities.Find(component);
    if (it != mReceivers->mActivities.End()) {
        a = it->mSecond;
    }
    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getReceiverInfo ") + component + ": " + a);
    }

    if (a != NULL && mSettings->IsEnabledLP(
            (IComponentInfo*)(IActivityInfo*)a->mInfo.Get(), flags)) {
        AutoPtr<IActivityInfo> info = CapsuleParser::GenerateActivityInfo(a, flags);
        *ai = info;
        if (*ai != NULL) (*ai)->AddRef();
        return NOERROR;
    }
    *ai = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CCapsuleManagerService::GetServiceInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IServiceInfo** si)
{
    if (si == NULL) return E_INVALID_ARGUMENT;

    Mutex::Autolock lock(mCapsulesLock);

    CapsuleParser::Service* s = NULL;
    HashMap<AutoPtr<IComponentName>, CapsuleParser::Service*>::Iterator it =
            mServices->mServices.Find(component);
    if (it != mServices->mServices.End()) {
        s = it->mSecond;
    }
    if (Config::LOGV) {
        Slogger::V(TAG, StringBuffer("getServiceInfo ") + component + ": " + s);
    }

    if (s != NULL && TRUE /*mSettings->IsEnabledLP(
            (IComponentInfo*)(IServiceInfo*)s->mInfo.Get(), flags)*/) {
        AutoPtr<IServiceInfo> info = CapsuleParser::GenerateServiceInfo(s, flags);
        *si = info;
        if (*si != NULL) (*si)->AddRef();
        return NOERROR;
    }
    *si = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::GetContentProviderInfo(
    /* [in] */ IComponentName* component,
    /* [in] */ Int32 flags,
    /* [out] */ IContentProviderInfo** pi)
{
    VALIDATE_NOT_NULL(component);
    VALIDATE_NOT_NULL(pi);

    Mutex::Autolock lock(mCapsulesLock);

    CapsuleParser::ContentProvider* p = NULL;
    HashMap<AutoPtr<IComponentName>, CapsuleParser::ContentProvider*>::Iterator it =
            mProvidersByComponent.Find(component);
    if (it != mProvidersByComponent.End()) {
        p = it->mSecond;
    }
    if (Config::LOGV) {
        Logger::V(TAG, StringBuffer("getProviderInfo ") + component + ": " + p);
    }

    if (p != NULL && mSettings->IsEnabledLP(p->mInfo, flags)) {
        AutoPtr<IContentProviderInfo> info = CapsuleParser::GenerateContentProviderInfo(p, flags);
        *pi = info;
        if (*pi != NULL) (*pi)->AddRef();
        return NOERROR;
    }
    *pi = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CCapsuleManagerService::GetSystemSharedLibraryNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);

    Mutex::Autolock lock(mCapsulesLock);

    Int32 size = mSharedLibraries.GetSize();
    if (size > 0) {
        ArrayOf<String>* libs = ArrayOf<String>::Alloc(size);
        if (!libs) {
            return E_OUT_OF_MEMORY_ERROR;
        }

        Int32 i = 0;
        HashMap<String, String>::Iterator it;
        for (it = mSharedLibraries.Begin();
             it != mSharedLibraries.End(); ++it) {
            (*libs)[i++] = it->mFirst;
        }
        *names = libs;
        return NOERROR;
    }
    *names = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::GetSystemAvailableFeatures(
    /* [out] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    Mutex::Autolock lock(mCapsulesLock);

    AutoPtr<IObjectContainer> featSet;
    FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&featSet));

    Int32 size = mAvailableFeatures.GetSize();
    if (size > 0) {
        HashMap<String, AutoPtr<IFeatureInfo> >::Iterator it;
        for (it = mAvailableFeatures.Begin();
             it != mAvailableFeatures.End(); ++it) {
            featSet->Add(it->mSecond);
        }
        AutoPtr<IFeatureInfo> fi;
        FAIL_RETURN(CFeatureInfo::New((IFeatureInfo**)&fi));
        fi->SetReqGlEsVersion(SystemProperties::GetInt32("ro.opengles.version",
                FeatureInfo_GL_ES_VERSION_UNDEFINED));
        featSet->Add(fi);
        *infos = featSet;
        if (*infos != NULL) (*infos)->AddRef();
        return NOERROR;
    }
    *infos = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::HasSystemFeature(
    /* [in] */ const String& name,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mCapsulesLock);

    HashMap<String, AutoPtr<IFeatureInfo> >::Iterator it = mAvailableFeatures.Find(name);
    *result = it != mAvailableFeatures.End();
    return NOERROR;
}


ECode CCapsuleManagerService::CheckPermission(
    /* [in] */ const String& permName,
    /* [in] */ const String& capName,
    /* [out] */ Int32* perm)
{
    VALIDATE_STRING_NOT_NULL(permName);
    VALIDATE_STRING_NOT_NULL(capName);
    VALIDATE_NOT_NULL(perm);

    Mutex::Autolock lock(mCapsulesLock);

    CapsuleParser::Capsule* c = NULL;
    HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        c = it->mSecond;
    }
    if (c != NULL && c->mExtras != NULL) {
        CapsuleSetting* cs = (CapsuleSetting*)c->mExtras->Probe(EIID_CapsuleSetting);
        if (cs->mSharedUser != NULL) {
            if (cs->mSharedUser->mGrantedPermissions.Find(permName)
                 != cs->mSharedUser->mGrantedPermissions.End()) {
                *perm = CapsuleManager_PERMISSION_GRANTED;
                return NOERROR;
            }
        }
        else {
            if (cs->mGrantedPermissions.Find(permName)
                != cs->mGrantedPermissions.End()) {
                *perm = CapsuleManager_PERMISSION_GRANTED;
                return NOERROR;
            }
        }
    }
    *perm = CapsuleManager_PERMISSION_DENIED;
    return NOERROR;
}

ECode CCapsuleManagerService::CheckUidPermission(
    /* [in] */ const String& permName,
    /* [in] */ Int32 uid,
    /* [out] */ Int32* perm)
{
    VALIDATE_NOT_NULL(perm);

    Mutex::Autolock lock(mCapsulesLock);

    //todo: check;
    GrantedPermissions* gp = (GrantedPermissions*)mSettings->GetUserIdLP(uid).Get();
    if (gp != NULL) {
        if (gp->mGrantedPermissions.Find(permName) != gp->mGrantedPermissions.End()) {
            *perm = CapsuleManager_PERMISSION_GRANTED;
            return NOERROR;
        }
    }
    else {
        HashSet<String>* perms = NULL;
        HashMap<Int32, HashSet<String>* >::Iterator it = mSystemPermissions.Find(uid);
        if (it != mSystemPermissions.End()) {
            perms = it->mSecond;
        }
        if (perms != NULL && perms->Find(permName) != perms->End()) {
            *perm = CapsuleManager_PERMISSION_GRANTED;
            return NOERROR;
        }
    }
    *perm = CapsuleManager_PERMISSION_DENIED;
    return NOERROR;
}

CCapsuleManagerService::BasePermission*
CCapsuleManagerService::FindPermissionTreeLP(
    /* [in] */ const String& permName)
{
    assert(!permName.IsNull());

    HashMap<String, BasePermission*>::Iterator it;
    for (it = mSettings->mPermissionTrees.Begin();
         it != mSettings->mPermissionTrees.End(); ++it) {
        BasePermission* bp = it->mSecond;

        if (permName.StartWith(bp->mName) &&
                permName.GetLength() > bp->mName.GetLength() &&
                permName.GetChar(bp->mName.GetLength()) == '.') {
            return bp;
        }
    }
    return NULL;
}

ECode CCapsuleManagerService::CheckPermissionTreeLP(
    /* [in] */ const String& permName,
    /* [out] */ BasePermission** basePerm)
{
    if (!permName.IsNull()) {
        BasePermission* bp = FindPermissionTreeLP(permName);
        if (bp != NULL) {
            if (TRUE /*bp->mUid == Binder::GetCallingUid()*/) {
                if (basePerm) *basePerm = bp;
                return NOERROR;
            }
            Logger::E(TAG, StringBuffer("Calling uid ")
                /*+ Binder::GetCallingUid()*/
                + " is not allowed to add to permission tree "
                + bp->mName + " owned by uid " + bp->mUid);
            return E_SECURITY_EXCEPTION;
        }
    }
    Logger::E(TAG, StringBuffer("No permission tree found for ") + permName);
    return E_SECURITY_EXCEPTION;
}

Boolean CCapsuleManagerService::CompareStrings(
    /* [in] */ ICharSequence* s1,
    /* [in] */ ICharSequence* s2)
{
    if (s1 == NULL) {
        return s2 == NULL;
    }
    if (s2 == NULL) {
        return FALSE;
    }
//    if (s1.getClass() != s2.getClass()) {
//        return false;
//    }
    String str1, str2;
    s1->ToString(&str1);
    s2->ToString(&str2);
    return str1.Equals(str2);
}

Boolean CCapsuleManagerService::CompareStrings(
    /* [in] */ const String& s1,
    /* [in] */ const String& s2)
{
    if (s1.IsNull()) {
        return s2.IsNull();
    }
    if (s2.IsNull()) {
        return FALSE;
    }
    return s1.Equals(s2);
}

Boolean CCapsuleManagerService::ComparePermissionInfos(
    /* [in] */ IPermissionInfo* pi1,
    /* [in] */ IPermissionInfo* pi2)
{
    assert(pi1);
    assert(pi2);

    Int32 pi1Icon, pi2Icon;
    pi1->GetIcon(&pi1Icon);
    pi2->GetIcon(&pi2Icon);
    if (pi1Icon != pi2Icon) return FALSE;

    Int32 pi1Logo, pi2Logo;
    pi1->GetLogo(&pi1Logo);
    pi2->GetLogo(&pi2Logo);
    if (pi1Logo != pi2Logo) return FALSE;

    Int32 pi1Level, pi2Level;
    pi1->GetProtectionLevel(&pi1Level);
    pi2->GetProtectionLevel(&pi2Level);
    if (pi1Level != pi2Level) return FALSE;

    String pi1Name, pi2Name;
    pi1->GetName(&pi1Name);
    pi2->GetName(&pi2Name);
    if (!CompareStrings(pi1Name, pi2Name)) return FALSE;

    AutoPtr<ICharSequence> pi1Label, pi2Label;
    pi1->GetNonLocalizedLabel((ICharSequence**)&pi1Label);
    pi2->GetNonLocalizedLabel((ICharSequence**)&pi2Label);
    if (!CompareStrings(pi1Label, pi2Label)) return FALSE;

    // We'll take care of setting this one.
    String pi1CName, pi2CName;
    pi1->GetCapsuleName(&pi1CName);
    pi2->GetCapsuleName(&pi2CName);
    if (!CompareStrings(pi1CName, pi2CName)) return FALSE;
    // These are not currently stored in settings.
    //if (!CompareStrings(pi1->mGroup, pi2->mGroup)) return FALSE;
    //if (!CompareStrings(pi1->mNonLocalizedDescription, pi2->mNonLocalizedDescription)) return FALSE;
    //if (pi1->mLabelRes != pi2->mLabelRes) return FALSE;
    //if (pi1->mDescriptionRes != pi2->mDescriptionRes) return FALSE;
    return TRUE;
}

ECode CCapsuleManagerService::AddPermissionLocked(
    /* [in] */ IPermissionInfo* info,
    /* [in] */ Boolean async,
    /* [out] */ Boolean* isAdded)
{
    assert(info);
    assert(isAdded);

    Int32 infoLabelRes;
    info->GetLabelRes(&infoLabelRes);
    AutoPtr<ICharSequence> infoLabel;
    info->GetNonLocalizedLabel((ICharSequence**)&infoLabel);
    if (infoLabelRes == 0 && infoLabel == NULL) {
        Logger::E(TAG, "Label must be specified in permission");
        *isAdded = FALSE;
        return E_SECURITY_EXCEPTION;
    }
    String infoName;
    info->GetName(&infoName);
    BasePermission* tree = NULL;
    CheckPermissionTreeLP(infoName, &tree);
    BasePermission* bp = NULL;
    HashMap<String, BasePermission*>::Iterator it =
            mSettings->mPermissions.Find(infoName);
    if (it != mSettings->mPermissions.End()) {
        bp = it->mSecond;
    }
    Boolean added = bp == NULL;
    Boolean changed = TRUE;
    if (added) {
        bp = new BasePermission(
            infoName, tree->mSourceCapsule, BasePermission::TYPE_DYNAMIC);
    }
    else if (bp->mType != BasePermission::TYPE_DYNAMIC) {
        Logger::E(TAG,
            StringBuffer("Not allowed to modify non-dynamic permission ")
            + infoName);
        *isAdded = FALSE;
        return E_SECURITY_EXCEPTION;
    }
    else {
        Int32 infoLevel;
        info->GetProtectionLevel(&infoLevel);
        if (bp->mProtectionLevel == infoLevel
            && bp->mPerm->mOwner == tree->mPerm->mOwner
            && bp->mUid == tree->mUid
            && ComparePermissionInfos(bp->mPerm->mInfo, info)) {
            changed = FALSE;
        }
    }
    info->GetProtectionLevel(&bp->mProtectionLevel);
    AutoPtr<IPermissionInfo> pinfo;
    ASSERT_SUCCEEDED(CPermissionInfo::New(info, (IPermissionInfo**)&pinfo));
    bp->mPerm = new CapsuleParser::Permission(tree->mPerm->mOwner, pinfo);
    String treeCName;
    tree->mPerm->mInfo->GetCapsuleName(&treeCName);
    bp->mPerm->mInfo->SetCapsuleName(treeCName);
    bp->mUid = tree->mUid;
    if (added) {
        mSettings->mPermissions[infoName] = bp;
    }
    if (changed) {
        if (!async) {
            mSettings->WriteLP();
        }
        else {
            ScheduleWriteSettingsLocked();
        }
    }
    *isAdded = added;
    return NOERROR;
}

ECode CCapsuleManagerService::AddPermission(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* isAdded)
{
    VALIDATE_NOT_NULL(info);
    VALIDATE_NOT_NULL(isAdded);
    Mutex::Autolock lock(mCapsulesLock);
    return AddPermissionLocked(info, FALSE, isAdded);
}

ECode CCapsuleManagerService::AddPermissionAsync(
    /* [in] */ IPermissionInfo* info,
    /* [out] */ Boolean* isAdded)
{
    VALIDATE_NOT_NULL(info);
    VALIDATE_NOT_NULL(isAdded);
    Mutex::Autolock lock(mCapsulesLock);
    return AddPermissionLocked(info, TRUE, isAdded);
}

ECode CCapsuleManagerService::RemovePermission(
    /* [in] */ const String& name)
{
    Mutex::Autolock lock(mCapsulesLock);

    CheckPermissionTreeLP(name, NULL);
    BasePermission* bp = NULL;
    HashMap<String, BasePermission*>::Iterator it = mSettings->mPermissions.Find(name);
    if (it != mSettings->mPermissions.End()) {
        bp = it->mSecond;
    }
    if (bp != NULL) {
        if (bp->mType != BasePermission::TYPE_DYNAMIC) {
            Logger::E(TAG, StringBuffer("Not allowed to modify non-dynamic permission ") + name);
            return E_SECURITY_EXCEPTION;
        }
        mSettings->mPermissions.Erase(name);
        mSettings->WriteLP();
    }
    return NOERROR;
}

ECode CCapsuleManagerService::IsProtectedBroadcast(
    /* [in] */ const String& actionName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mCapsulesLock);
    *result = mProtectedBroadcasts.Find(actionName) != mProtectedBroadcasts.End();
    return NOERROR;
}

ECode CCapsuleManagerService::CheckSignatures(
    /* [in] */ const String& cap1,
    /* [in] */ const String& cap2,
    /* [out] */ Int32* sig)
{
    VALIDATE_NOT_NULL(sig);

    Mutex::Autolock Lock(mCapsulesLock);
    HashMap<String, CapsuleParser::Capsule*>::Iterator p1 = mCapsules.Find(cap1);
    HashMap<String, CapsuleParser::Capsule*>::Iterator p2 = mCapsules.Find(cap2);
    if (p1 == mCapsules.End() || p1->mSecond->mExtras == NULL
        || p2 == mCapsules.End() || p2->mSecond->mExtras == NULL) {
        *sig = CapsuleManager_SIGNATURE_UNKNOWN_CAPSULE;
        return NOERROR;
    }
    *sig = CheckSignaturesLP(p1->mSecond->mSignatures, p2->mSecond->mSignatures);
    return NOERROR;
}

ECode CCapsuleManagerService::CheckUidSignatures(
    /* [in] */ Int32 uid1,
    /* [in] */ Int32 uid2,
    /* [out] */ Int32* sig)
{
    VALIDATE_NOT_NULL(sig);

    Mutex::Autolock Lock(mCapsulesLock);

    ArrayOf< AutoPtr<ISignature> >* s1;
    ArrayOf< AutoPtr<ISignature> >* s2;
    AutoPtr<IInterface> obj = mSettings->GetUserIdLP(uid1);
    if (obj != NULL) {
        if (obj->Probe(EIID_SharedUserSetting) != NULL) {
            s1 = ((SharedUserSetting*)obj->Probe(EIID_SharedUserSetting))->mSignatures->mSignatures;
        }
        else if (obj->Probe(EIID_CapsuleSetting) != NULL) {
            s1 = ((CapsuleSetting*)obj->Probe(EIID_CapsuleSetting))->mSignatures->mSignatures;
        }
        else {
            *sig = CapsuleManager_SIGNATURE_UNKNOWN_CAPSULE;
            return NOERROR;
        }
    }
    else {
        *sig = CapsuleManager_SIGNATURE_UNKNOWN_CAPSULE;
        return NOERROR;
    }
    obj = mSettings->GetUserIdLP(uid2);
    if (obj != NULL) {
        if (obj->Probe(EIID_SharedUserSetting) != NULL) {
            s2 = ((SharedUserSetting*)obj->Probe(EIID_SharedUserSetting))->mSignatures->mSignatures;
        }
        else if (obj->Probe(EIID_CapsuleSetting) != NULL) {
            s2 = ((CapsuleSetting*)obj->Probe(EIID_CapsuleSetting))->mSignatures->mSignatures;
        }
        else {
            *sig = CapsuleManager_SIGNATURE_UNKNOWN_CAPSULE;
            return NOERROR;
        }
    }
    else {
        *sig = CapsuleManager_SIGNATURE_UNKNOWN_CAPSULE;
        return NOERROR;
    }
    *sig = CheckSignaturesLP(s1, s2);
    return NOERROR;
}

Int32 CCapsuleManagerService::CheckSignaturesLP(
    /* [in] */ ArrayOf< AutoPtr<ISignature> >* s1,
    /* [in] */ ArrayOf< AutoPtr<ISignature> >* s2)
{
    if (s1 == NULL) {
        return s2 == NULL
                ? CapsuleManager_SIGNATURE_NEITHER_SIGNED
                : CapsuleManager_SIGNATURE_FIRST_NOT_SIGNED;
    }
    if (s2 == NULL) {
        return CapsuleManager_SIGNATURE_SECOND_NOT_SIGNED;
    }

    Boolean isContained = TRUE;
    for (Int32 i = 0; i < s1->GetLength(); i++) {
        Boolean is = FALSE;
        for (Int32 j = 0; j < s2->GetLength(); j++) {
            if ((*s1)[i] == (*s1)[j]) {
                is = TRUE;
                break;
            }
        }
        if (!is) {
            isContained = FALSE;
            break;
        }
    }

    // Make sure s2 contains all signatures in s1.
    return isContained
        ? CapsuleManager_SIGNATURE_MATCH
        : CapsuleManager_SIGNATURE_NO_MATCH;
}

ECode CCapsuleManagerService::GetCapsulesForUid(
    /* [in] */ Int32 uid,
    /* [out, callee] */ ArrayOf<String>** capsules)
{
    VALIDATE_NOT_NULL(capsules);

    Mutex::Autolock Lock(mCapsulesLock);

    AutoPtr<IInterface> obj = mSettings->GetUserIdLP(uid);
    if (obj->Probe(EIID_SharedUserSetting) != NULL) {
        SharedUserSetting* sus = (SharedUserSetting*)obj->Probe(EIID_SharedUserSetting);
        const Int32 N = sus->mCapsules.GetSize();
        ArrayOf<String>* res = ArrayOf<String>::Alloc(N);
        Int32 i = 0;
        HashMap<String, CapsuleParser::Capsule*>::Iterator it;
        for (it = mCapsules.Begin(); it != mCapsules.End(); it++) {
            (*res)[i++] = it->mFirst;
        }
        *capsules = res;
        return NOERROR;
    }
    else if (obj->Probe(EIID_CapsuleSetting) != NULL) {
        CapsuleSetting* ps = (CapsuleSetting*)obj->Probe(EIID_CapsuleSetting);
        ArrayOf<String>* res = ArrayOf<String>::Alloc(1);
        (*res)[0] = ps->mName;
        *capsules = res;
        return NOERROR;
    }
    *capsules = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::GetNameForUid(
    /* [in] */ Int32 uid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    Mutex::Autolock Lock(mCapsulesLock);

    AutoPtr<IInterface> obj = mSettings->GetUserIdLP(uid);
    if (obj->Probe(EIID_SharedUserSetting) != NULL) {
        SharedUserSetting* sus = (SharedUserSetting*)obj->Probe(EIID_SharedUserSetting);
        *name = (const char*)(StringBuffer(sus->mName) + ":" + sus->mUserId);
        return NOERROR;
    }
    else if (obj->Probe(EIID_CapsuleSetting) != NULL) {
        CapsuleSetting* ps = (CapsuleSetting*)obj->Probe(EIID_CapsuleSetting);
        *name = ps->mName;
        return NOERROR;
    }
    *name = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::GetUidForSharedUser(
    /* [in] */ const String& sharedUserName,
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    if (sharedUserName.IsNull()) {
        *uid = -1;
        return NOERROR;
    }

    Mutex::Autolock Lock(mCapsulesLock);

    SharedUserSetting* suid = mSettings->GetSharedUserLP(sharedUserName, 0, FALSE);
    if (suid == NULL) {
        *uid = -1;
        return NOERROR;
    }
    *uid = suid->mUserId;
    return NOERROR;
}

ECode CCapsuleManagerService::ResolveIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** resolveInfo)
{
    VALIDATE_NOT_NULL(resolveInfo);

    AutoPtr<IObjectContainer> query;
    QueryIntentActivities(intent, resolvedType, flags, (IObjectContainer**)&query);
    return ChooseBestActivity(intent, resolvedType, flags, query, resolveInfo);
}

ECode CCapsuleManagerService::ChooseBestActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ IObjectContainer* query,
    /* [out] */ IResolveInfo** resolveInfo)
{
    if (query != NULL) {
        Int32 size;
        query->GetObjectCount(&size);
        if (size == 1) {
            AutoPtr<IObjectEnumerator> objEnu;
            query->GetObjectEnumerator((IObjectEnumerator**)&objEnu);
            Boolean isSucceeded;
            objEnu->MoveNext(&isSucceeded);
            objEnu->Current((IInterface**)resolveInfo);
            return NOERROR;
        }
        else if (size > 1) {
            // If there is more than one activity with the same priority,
            // then let the user decide between them.
            AutoPtr<IObjectEnumerator> objEnu;
            query->GetObjectEnumerator((IObjectEnumerator**)&objEnu);
            AutoPtr<IResolveInfo> ri0, ri1;
            Boolean isSucceeded;
            objEnu->MoveNext(&isSucceeded);
            objEnu->Current((IInterface**)&ri0);
            objEnu->MoveNext(&isSucceeded);
            objEnu->Current((IInterface**)&ri1);
//            if (false) {
//                System.out.println(r0.activityInfo.name +
//                                   "=" + r0.priority + " vs " +
//                                   r1.activityInfo.name +
//                                   "=" + r1.priority);
//            }
            // If the first activity has a higher priority, or a different
            // default, then it is always desireable to pick it.
            Int32 ri0Priority, ri1Priority;
            ri0->GetPriority(&ri0Priority);
            ri1->GetPriority(&ri1Priority);
            Int32 ri0Order, ri1Order;
            ri0->GetPreferredOrder(&ri0Order);
            ri1->GetPreferredOrder(&ri1Order);
            Boolean ri0Default, ri1Default;
            ri0->IsDefault(&ri0Default);
            ri1->IsDefault(&ri1Default);
            if (ri0Priority != ri1Priority
                    || ri0Order != ri1Order
                    || ri0Default != ri1Default) {
                *resolveInfo = ri0;
                if (*resolveInfo != NULL) (*resolveInfo)->AddRef();
                return NOERROR;
            }
            // If we have saved a preference for a preferred activity for
            // this Intent, use that.
            AutoPtr<IResolveInfo> ri = FindPreferredActivity(intent, resolvedType,
                    flags, query, ri0Priority);
            if (ri != NULL) {
                *resolveInfo = ri;
                if (*resolveInfo != NULL) (*resolveInfo)->AddRef();
                return NOERROR;
            }
            *resolveInfo = mResolveInfo;
            if (*resolveInfo != NULL) (*resolveInfo)->AddRef();
            return NOERROR;
        }
    }
    *resolveInfo = NULL;
    return NOERROR;
}

AutoPtr<IResolveInfo> CCapsuleManagerService::FindPreferredActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [in] */ IObjectContainer* query,
    /* [in] */ Int32 priority)
{
    assert(!intent || !query);

    Mutex::Autolock lock(mCapsulesLock);

    if (DEBUG_PREFERRED) intent->AddFlags(Intent_FLAG_DEBUG_LOG_RESOLUTION);
    List<PreferredActivity*>* prefs =
        mSettings->mPreferredActivities->QueryIntent(intent, resolvedType,
            (flags & CapsuleManager_MATCH_DEFAULT_ONLY) != 0);
    if (prefs != NULL && prefs->GetSize() > 0) {
        // First figure out how good the original match set is.
        // We will only allow preferred activities that came
        // from the same match quality.
        Int32 match = 0;
        if (DEBUG_PREFERRED) Slogger::V(TAG, "Figuring out best match...");
        AutoPtr<IObjectEnumerator> objEnu;
        query->GetObjectEnumerator((IObjectEnumerator**)&objEnu);
        Boolean isSucceeded;
        objEnu->MoveNext(&isSucceeded);
        while (isSucceeded) {
            AutoPtr<IResolveInfo> ri;
            objEnu->Current((IInterface**)&ri);
            if (DEBUG_PREFERRED) {
                AutoPtr<IActivityInfo> riAInfo;
                ri->GetActivityInfo((IActivityInfo**)&riAInfo);
                Slogger::V(TAG, StringBuffer("Match for ")
                    + riAInfo.Get()
                    + ": 0x" + /*Integer::ToHexString(match)*/ match);
            }
            Int32 riMatch;
            ri->GetMatch(&riMatch);
            if (riMatch > match) match = riMatch;
            objEnu->MoveNext(&isSucceeded);
        }
        if (DEBUG_PREFERRED) {
            Slogger::V(TAG, StringBuffer("Best match: 0x")
                + /*Integer::ToHexString(match)*/ match);
        }
        match &= IntentFilter_MATCH_CATEGORY_MASK;
        List<PreferredActivity*>::Iterator pit;
        for (pit = prefs->Begin(); pit != prefs->End(); ++pit) {
            PreferredActivity* pa = *pit;
            if (pa->mMatch != match) {
                continue;
            }
            AutoPtr<IActivityInfo> ai;
            GetActivityInfo(pa->mActivity.Get(), flags, (IActivityInfo**)&ai);
            if (DEBUG_PREFERRED) {
                Slogger::V(TAG, "Got preferred activity:");
                if (ai != NULL) {
//	                    ai->Dump(new LogPrinter(Log::VERBOSE, TAG), "  ");
                }
                else {
                    Logger::V(TAG, "  NULL");
                }
            }
            if (ai != NULL) {
                AutoPtr<IApplicationInfo> aiAppInfo;
                ai->GetApplicationInfo((IApplicationInfo**)&aiAppInfo);
                String aiName, aiCName;
                ai->GetName(&aiName);
                aiAppInfo->GetCapsuleName(&aiCName);

                AutoPtr<IObjectEnumerator> objEnu;
                query->GetObjectEnumerator((IObjectEnumerator**)&objEnu);
                Boolean isSucceeded;
                objEnu->MoveNext(&isSucceeded);
                while (isSucceeded) {
                    AutoPtr<IResolveInfo> ri;
                    objEnu->Current((IInterface**)&ri);
                    AutoPtr<IActivityInfo> riAInfo;
                    ri->GetActivityInfo((IActivityInfo**)&riAInfo);
                    AutoPtr<IApplicationInfo> riAppInfo;
                    riAInfo->GetApplicationInfo((IApplicationInfo**)&riAppInfo);
                    String riName, riCName;
                    riAInfo->GetName(&riName);
                    riAppInfo->GetCapsuleName(&riCName);
                    if (!riCName.Equals(aiName)) {
                        objEnu->MoveNext(&isSucceeded);
                    }
                    if (!riName.Equals(aiName)) {
                        objEnu->MoveNext(&isSucceeded);
                    }

                    // Okay we found a previously set preferred app.
                    // If the result set is different from when this
                    // was created, we need to clear it and re-ask the
                    // user their preference.
                    if (!pa->SameSet(query, priority)) {
                        Slogger::I(TAG, StringBuffer("Result set changed, dropping preferred activity for ")
                            + intent + " type " + resolvedType);
	                        mSettings->mPreferredActivities->RemoveFilter(pa);
                        return NULL;
                    }

                    // Yay!
                    return ri;
                }
            }
        }
    }
    return NULL;
}

ECode CCapsuleManagerService::QueryIntentActivities(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    assert(intent != NULL);
    VALIDATE_NOT_NULL(infos);

    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp != NULL) {
        AutoPtr<IObjectContainer> list;
        FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&list));
        AutoPtr<IActivityInfo> ai;
        GetActivityInfo(comp, flags, (IActivityInfo**)&ai);
        if (ai != NULL) {
            AutoPtr<IResolveInfo> ri;
            FAIL_RETURN(CResolveInfo::New((IResolveInfo**)&ri));
            ri->SetActivityInfo(ai);
            list->Add(ri);
        }
        *infos = list.Get();
        INTERFACE_ADDREF(*infos);
        return NOERROR;
    }

    Mutex::Autolock Lock(mCapsulesLock);

    String capName;
    intent->GetCapsule(&capName);
    if (capName.IsNull()) {
        List<AutoPtr<IResolveInfo>*>* items =
            mActivities->QueryIntent(intent, resolvedType, flags);
        if (!items) return E_RUNTIME_EXCEPTION;

        AutoPtr<IObjectContainer> list;
        FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&list));
        List<AutoPtr<IResolveInfo>*>::Iterator it;
        for (it = items->Begin(); it != items->End(); ++it) {
            list->Add((IResolveInfo*)(**it).Get());
        }
        delete items;
        *infos = list.Get();
        INTERFACE_ADDREF(*infos);
        return NOERROR;
    }
    HashMap<String, CapsuleParser::Capsule*>::Iterator cap = mCapsules.Find(capName);
    if (cap != mCapsules.End()) {
        List<AutoPtr<IResolveInfo>*>* items =
            mActivities->QueryIntentForCapsule(
                intent, resolvedType, flags, &cap->mSecond->mActivities);
        if (!items) return E_RUNTIME_EXCEPTION;

        AutoPtr<IObjectContainer> list;
        FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&list));
        List<AutoPtr<IResolveInfo>*>::Iterator it;
        for (it = items->Begin(); it != items->End(); ++it) {
            list->Add((IResolveInfo*)(**it).Get());
        }
        delete items;
        *infos = list.Get();
        INTERFACE_ADDREF(*infos);
        return NOERROR;
    }

    *infos = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::QueryIntentActivityOptions(
    /* [in] */ IComponentName* caller,
    /* [in] */ ArrayOf<IIntent*>* specifics,
    /* [in] */ ArrayOf<String>* specificTypes,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    assert(intent != NULL);
    VALIDATE_NOT_NULL(infos);

    String resultsAction;
    intent->GetAction(&resultsAction);

    AutoPtr<IObjectContainer> _results;
    QueryIntentActivities(
            intent, resolvedType, flags | CapsuleManager_GET_RESOLVED_FILTER,
            (IObjectContainer**)&_results);
    assert(_results != NULL);
    List< AutoPtr<IResolveInfo> > results;
    AutoPtr<IObjectEnumerator> objEnu;
    _results->GetObjectEnumerator((IObjectEnumerator**)&objEnu);
    Boolean isSucceeded;
    objEnu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IResolveInfo> obj;
        objEnu->Current((IInterface**)&obj);
        results.PushBack(obj);
        objEnu->MoveNext(&isSucceeded);
    }
    if (Config::LOGV) Logger::V(TAG, StringBuffer("Query ") + intent + ": " + &results);

    Int32 specificsPos = 0;

    // todo: note that the algorithm used here is O(N^2).  This
    // isn't a problem in our current environment, but if we start running
    // into situations where we have more than 5 or 10 matches then this
    // should probably be changed to something smarter...

    // First we go through and resolve each of the specific items
    // that were supplied, taking care of removing any corresponding
    // duplicate items in the generic resolve list.
    if (specifics != NULL) {
        for (Int32 i = 0; i < specifics->GetLength(); i++) {
            AutoPtr<IIntent> sintent = (*specifics)[i];
            if (sintent == NULL) {
                continue;
            }

            if (Config::LOGV) {
                Logger::V(TAG, StringBuffer("Specific #") + i + ": " + sintent);
            }

            String action;
            sintent->GetAction(&action);
            if (!resultsAction.IsNull() && resultsAction.Equals(action)) {
                // If this action was explicitly requested, then don't
                // remove things that have it.
                action = String(NULL);
            }
            AutoPtr<IComponentName> comp;
            sintent->GetComponent((IComponentName**)&comp);
            AutoPtr<IResolveInfo> ri;
            AutoPtr<IActivityInfo> ai;
            if (comp == NULL) {
                ResolveIntent(sintent,
                    specificTypes!= NULL ? (*specificTypes)[i] : String(NULL),
                    flags, (IResolveInfo**)&ri);
                if (ri == NULL) {
                    continue;
                }
                if (ri.Get() == mResolveInfo.Get()) {
                    // ACK!  Must do something better with this.
                }
                ri->GetActivityInfo((IActivityInfo**)&ai);
                String aiName, aiCName;
                ai->GetName(&aiName);
                AutoPtr<IApplicationInfo> aiAppInfo;
                ai->GetApplicationInfo((IApplicationInfo**)&aiAppInfo);
                aiAppInfo->GetCapsuleName(&aiCName);
                FAIL_RETURN(CComponentName::New(
                    aiCName, aiName, (IComponentName**)&comp));
            }
            else {
                GetActivityInfo(comp, flags, (IActivityInfo**)&ai);
                if (ai == NULL) {
                    continue;
                }
            }

            // Look for any generic query activities that are duplicates
            // of this specific one, and remove them from the results.
            if (Config::LOGV) {
                Logger::V(TAG, StringBuffer("Specific #") + i + ": " + ai);
            }

            List< AutoPtr<IResolveInfo> >::Iterator it = results.Begin();
            for (Int32 k = 0; it != results.End(), k < specificsPos; ++it, ++k);
            while (it != results.End()) {
                IResolveInfo* sri = *it;
                String clsName, capName;
                comp->GetClassName(&clsName);
                comp->GetCapsuleName(&capName);
                AutoPtr<IActivityInfo> sriAInfo;
                sri->GetActivityInfo((IActivityInfo**)&sriAInfo);
                AutoPtr<IApplicationInfo> sriAppInfo;
                sriAInfo->GetApplicationInfo((IApplicationInfo**)&sriAppInfo);
                String sriName, sriCName;
                sriAInfo->GetName(&sriName);
                sriAppInfo->GetCapsuleName(&sriCName);
                AutoPtr<IIntentFilter> sriFilter;
                sri->GetIntentFilter((IIntentFilter**)&sriFilter);
                Boolean isMatched;
                if ((sriName.Equals(clsName) && sriCName.Equals(capName))
                    || (!action.IsNull() && (sriFilter->MatchAction(action, &isMatched), isMatched))) {
                    it = results.Erase(it);
                    if (Config::LOGV) Logger::V(
                        TAG, StringBuffer("Removing duplicate item from ")
                             + " due to specific " + specificsPos);
                    if (ri == NULL) {
                        ri = sri;
                    }
                }
                else {
                    ++it;
                }
            }

            // Add this specific item to its proper place.
            if (ri == NULL) {
                CResolveInfo::New((IResolveInfo**)&ri);
                ri->SetActivityInfo(ai);
            }

            it = results.Begin();
            for (Int32 k = 0; it != results.End(), k < specificsPos; ++it, ++k);
            results.Insert(it, ri);
            ri->SetSpecificIndex(i);
            specificsPos++;
        }
    }

    // Now we go through the remaining generic results and remove any
    // duplicate actions that are found here.
    List< AutoPtr<IResolveInfo> >::Iterator it = results.Begin();
    for (Int32 k = 0; it != results.End(), k < specificsPos; ++it, ++k);
    while (it != --results.End()) {
        IResolveInfo* rii = *it;
        AutoPtr<IIntentFilter> riiFilter;
        rii->GetIntentFilter((IIntentFilter**)&riiFilter);
        if (riiFilter == NULL) {
            ++it;
            continue;
        }

        // Iterate over all of the actions of this result's intent
        // filter...  typically this should be just one.
        AutoStringArray actions;
        riiFilter->GetActions((ArrayOf<String>**)&actions);
        if (actions == NULL) {
            continue;
        }
        for (Int32 i; i < actions->GetLength(); ++i) {
            const String& action = (*actions)[i];
            if (!resultsAction.IsNull() && resultsAction.Equals(action)) {
                // If this action was explicitly requested, then don't
                // remove things that have it.
                continue;
            }
//            for (int j=i+1; j<N; j++) {
//                final ResolveInfo rij = results->Get(j);
//                if (rij->mFilter != NULL && rij->mFilter->HasAction(action)) {
//                    results->Remove(j);
//                    if (Config::LOGV) Logger::V(
//                        TAG, "Removing duplicate item from " + j
//                        + " due to action " + action + " at " + i);
//                    j--;
//                    N--;
//                }
//            }
        }

        // If the caller didn't request filter information, drop it now
        // so we don't have to marshall/unmarshall it.
        if ((flags & CapsuleManager_GET_RESOLVED_FILTER) == 0) {
            rii->SetIntentFilter(NULL);
        }
        ++it;
    }

    // Filter out the caller activity if so requested.
    if (caller != NULL) {
        List< AutoPtr<IResolveInfo> >::Iterator it;
        for (it = results.Begin(); it != results.End(); ++it) {
            AutoPtr<IActivityInfo> ainfo;
            (*it)->GetActivityInfo((IActivityInfo**)&ainfo);
            String capName, clsName;
            caller->GetCapsuleName(&capName);
            caller->GetClassName(&clsName);
            AutoPtr<IApplicationInfo> appInfo;
            ainfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
            String aName, aCName;
            ainfo->GetName(&aName);
            appInfo->GetCapsuleName(&aCName);
            if (capName.Equals(aCName) && clsName.Equals(aName)) {
                results.Erase(it);
                break;
            }
        }
    }

    // If the caller didn't request filter information,
    // drop them now so we don't have to
    // marshall/unmarshall it.
    if ((flags & CapsuleManager_GET_RESOLVED_FILTER) == 0) {
        List< AutoPtr<IResolveInfo> >::Iterator it;
        for (it = results.Begin(); it != results.End(); ++it) {
            (*it)->SetIntentFilter(NULL);
        }
    }

    if (Config::LOGV) {
        Logger::V(TAG, StringBuffer("Result: ") + &results);
    }

    AutoPtr<IObjectContainer> container;
    FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&container));

    for (it = results.Begin(); it != results.End(); ++it) {
        container->Add(*it);
    }

    *infos = container.Get();
    INTERFACE_ADDREF(*infos);
    return NOERROR;
}

ECode CCapsuleManagerService::QueryIntentReceivers(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** receivers)
{
    VALIDATE_NOT_NULL(receivers);

    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp != NULL) {
        CParcelableObjectContainer::New(receivers);
        AutoPtr<IActivityInfo> ai;
        GetReceiverInfo(comp, flags, (IActivityInfo**)&ai);
        if (ai != NULL) {
            AutoPtr<IResolveInfo> ri;
            CResolveInfo::New((IResolveInfo**)&ri);
            ri->SetActivityInfo(ai);
            (*receivers)->Add(ri);
        }
        return NOERROR;
    }

    Mutex::Autolock lock(mCapsulesLock);

    String capName;
    intent->GetCapsule(&capName);
    if (capName.IsNull()) {
        CParcelableObjectContainer::New(receivers);
        List<AutoPtr<IResolveInfo>*>* list = mReceivers->QueryIntent(
                intent, resolvedType, flags);
        List<AutoPtr<IResolveInfo>*>::Iterator it = list->Begin();
        for(; it != list->End(); ++it) {
            (*receivers)->Add((IResolveInfo*)(**it).Get());
        }
        delete list;
        return NOERROR;
    }
    CapsuleParser::Capsule* cap = NULL;
    HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        cap = it->mSecond;
    }
    if (cap != NULL) {
        CParcelableObjectContainer::New(receivers);
        List<AutoPtr<IResolveInfo>*>* list = mReceivers->QueryIntentForCapsule(
                intent, resolvedType, flags, &(cap->mReceivers));
        List<AutoPtr<IResolveInfo>*>::Iterator it = list->Begin();
        for(; it != list->End(); ++it) {
            (*receivers)->Add((IResolveInfo*)(**it).Get());
        }
        delete list;
        return NOERROR;
    }

    *receivers = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::ResolveService(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IResolveInfo** resolveInfo)
{
    VALIDATE_NOT_NULL(resolveInfo);
    *resolveInfo = NULL;

    AutoPtr<IObjectContainer> query;
    QueryIntentServices(intent, resolvedType, flags, (IObjectContainer**)&query);
    if (query != NULL) {
        Int32 count;
        if (query->GetObjectCount(&count), count >= 1) {
            // If there is more than one service with the same priority,
            // just arbitrarily pick the first one.
            AutoPtr<IObjectEnumerator> it;
            query->GetObjectEnumerator((IObjectEnumerator**)&it);
            Boolean succeeded;
            it->MoveNext(&succeeded);
            if (succeeded) it->Current((IInterface**)resolveInfo);
            return NOERROR;
        }
    }
    *resolveInfo = NULL;
    return NOERROR;
}

ECode CCapsuleManagerService::QueryIntentServices(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** services)
{
    AutoPtr<IComponentName> comp;
    intent->GetComponent((IComponentName**)&comp);
    if (comp != NULL) {
        CParcelableObjectContainer::New(services);
        AutoPtr<IServiceInfo> si;
        GetServiceInfo(comp, flags, (IServiceInfo**)&si);
        if (si != NULL) {
            AutoPtr<IResolveInfo> ri;
            CResolveInfo::New((IResolveInfo**)&ri);
            ri->SetServiceInfo(si);
            (*services)->Add(ri);
        }
        return NOERROR;
    }

    Mutex::Autolock lock(mCapsulesLock);

    String capName;
    intent->GetCapsule(&capName);
    if (capName.IsNull()) {
        CParcelableObjectContainer::New(services);
        List<AutoPtr<IResolveInfo>*>* list = mServices->QueryIntent(
                intent, resolvedType, flags);
        List<AutoPtr<IResolveInfo>*>::Iterator it = list->Begin();
        for(; it != list->End(); ++it) {
            (*services)->Add((IResolveInfo*)(**it).Get());
        }
        delete list;
        return NOERROR;
    }
    CapsuleParser::Capsule* cap = NULL;
    HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capName);
    if (it != mCapsules.End()) {
        cap = it->mSecond;
    }
    if (cap != NULL) {
        CParcelableObjectContainer::New(services);
        List<AutoPtr<IResolveInfo>*>* list = mServices->QueryIntentForCapsule(
                intent, resolvedType, flags, &(cap->mServices));
        List<AutoPtr<IResolveInfo>*>::Iterator it = list->Begin();
        for(; it != list->End(); ++it) {
            (*services)->Add((IResolveInfo*)(**it).Get());
        }
        delete list;
        return NOERROR;
    }
    *services = NULL;
    return NOERROR;
}

Int32 arrays_binary_search(
    /* [in] */ const ArrayOf<String>& keys,
    /* [in] */ const String& value)
{
    Int32 lo = 0;
    Int32 hi = keys.GetLength() - 1;

    while (lo <= hi) {
        Int32 mid = (lo + hi) / 2;
        Int32 midValCmp = keys[mid].Compare(value);

        if (midValCmp < 0) {
            lo = mid + 1;
        }
        else if (midValCmp > 0) {
            hi = mid - 1;
        }
        else {
            return mid;  // value found
        }
    }
    return ~lo;  // value not present
}

void arrays_sort(
    /* [in] */ const ArrayOf<String>& keys)
{
    //not implemented
}

Int32 CCapsuleManagerService::GetContinuationPoint(
    /* [in] */ const ArrayOf<String>& keys,
    /* [in] */ const String& key)
{
    Int32 index;
    if (key.IsNull()) {
        index = 0;
    }
    else {
        Int32 insertPoint = arrays_binary_search(keys, key);
        if (insertPoint < 0) {
            index = -insertPoint;
        }
        else {
            index = insertPoint + 1;
        }
    }
    return index;
}

ECode CCapsuleManagerService::GetInstalledCapsules(
    /* [in] */ Int32 flags,
    /* [in] */ const String& lastRead,
    /* [out] */ IParceledListSlice** slice)
{
    VALIDATE_NOT_NULL(slice);

    FAIL_RETURN(CParceledListSlice::New(slice));
    Boolean listUninstalled = (flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0;
    AutoStringArray keys;

    {
        Mutex::Autolock lock(mCapsulesLock);

        if (listUninstalled) {
            keys = ArrayOf<String>::Alloc(mSettings->mCapsules.GetSize());
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it;
            Int32 i = 0;
            for (it = mSettings->mCapsules.Begin();
                 it != mSettings->mCapsules.End(); ++it, ++i) {
                (*keys)[i] = it->mFirst;
            }
        }
        else {
            keys = ArrayOf<String>::Alloc(mCapsules.GetSize());
            HashMap<String, CapsuleParser::Capsule*>::Iterator it;
            Int32 i = 0;
            for (it = mCapsules.Begin(); it != mCapsules.End(); ++it, ++i) {
                (*keys)[i] = it->mFirst;
            }
        }

	    arrays_sort(*keys);
        Int32 i = GetContinuationPoint(*keys, lastRead);
        const Int32 N = keys->GetLength();

        while (i < N) {
            String capsuleName = (*keys)[i++];

            AutoPtr<ICapsuleInfo> ci;
            if (listUninstalled) {
                CapsuleSetting* cs = NULL;
                HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it
                        = mSettings->mCapsules.Find(capsuleName);
                if (it != mSettings->mCapsules.End()) {
                    cs = it->mSecond;
                }
                if (cs != NULL) {
                    ci = GenerateCapsuleInfoFromSettingsLP(cs->mName, flags);
                }
            }
            else {
                CapsuleParser::Capsule* c = NULL;
                HashMap<String, CapsuleParser::Capsule*>::Iterator it
                        = mCapsules.Find(capsuleName);
                if (it != mCapsules.End()) {
                    c = it->mSecond;
                }
                if (c != NULL) {
                    ci = GenerateCapsuleInfo(c, flags);
                }
            }

            if (ci != NULL) {
                Boolean isFull = FALSE;
                (*slice)->Append(IParcelable::Probe(ci), &isFull);
                if (!isFull) break;
            }
        }

        if (i == N) {
            (*slice)->SetLastSlice(TRUE);
        }
    }

    return NOERROR;
}

ECode CCapsuleManagerService::GetInstalledApplications(
    /* [in] */ Int32 flags,
    /* [in] */ const String& lastRead,
    /* [out] */ IParceledListSlice** slice)
{
    VALIDATE_NOT_NULL(slice);

    FAIL_RETURN(CParceledListSlice::New(slice));
    const Boolean listUninstalled = (flags & CapsuleManager_GET_UNINSTALLED_CAPSULES) != 0;
    AutoStringArray keys;

    {
        Mutex::Autolock lock(mCapsulesLock);

        if (listUninstalled) {
            keys = ArrayOf<String>::Alloc(mSettings->mCapsules.GetSize());
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it;
            Int32 i = 0;
            for (it = mSettings->mCapsules.Begin();
                 it != mSettings->mCapsules.End(); ++it, ++i) {
                (*keys)[i] = it->mFirst;
            }
        }
        else {
            keys = ArrayOf<String>::Alloc(mCapsules.GetSize());
            HashMap<String, CapsuleParser::Capsule*>::Iterator it;
            Int32 i = 0;
            for (it = mCapsules.Begin(); it != mCapsules.End(); ++it, ++i) {
                (*keys)[i] = it->mFirst;
            }
        }

        arrays_sort(*keys);
        Int32 i = GetContinuationPoint(*keys, lastRead);
        const Int32 N = keys->GetLength();

        while (i < N) {
            String capsuleName = (*keys)[i++];

            AutoPtr<IApplicationInfo> ai;
            if (listUninstalled) {
                CapsuleSetting* cs = NULL;
                HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it
                        = mSettings->mCapsules.Find(capsuleName);
                if (it != mSettings->mCapsules.End()) {
                    cs = it->mSecond;
                }
                if (cs != NULL) {
                    ai = GenerateApplicationInfoFromSettingsLP(cs->mName, flags);
                }
            }
            else {
                CapsuleParser::Capsule* c = NULL;
                HashMap<String, CapsuleParser::Capsule*>::Iterator it
                        = mCapsules.Find(capsuleName);
                if (it != mCapsules.End()) {
                    c = it->mSecond;
                }
                if (c != NULL) {
                    ai = CapsuleParser::GenerateApplicationInfo(c, flags);
                }
            }

            if (ai != NULL) {
                Boolean isFull = FALSE;
                (*slice)->Append(IParcelable::Probe(ai), &isFull);
                if (!isFull) break;
            }
        }

        if (i == N) {
            (*slice)->SetLastSlice(TRUE);
        }
    }

    return NOERROR;
}

ECode CCapsuleManagerService::GetPersistentApplications(
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    FAIL_RETURN(CParcelableObjectContainer::New(infos));

    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<String, CapsuleParser::Capsule*>::Iterator it;
        for (it = mCapsules.Begin(); it != mCapsules.End(); ++it) {
            CapsuleParser::Capsule* c = it->mSecond;
            if (c->mApplicationInfo != NULL) {
                Int32 cFlags;
                c->mApplicationInfo->GetFlags(&cFlags);
                if ((cFlags & ApplicationInfo_FLAG_PERSISTENT) != 0
                    && (!mSafeMode || IsSystemApp(c))) {
                    AutoPtr<IApplicationInfo> cinfo = CapsuleParser::GenerateApplicationInfo(c, flags);
                    (*infos)->Add(cinfo);
                }
            }
        }
    }

    return NOERROR;
}

ECode CCapsuleManagerService::ResolveContentProvider(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ IContentProviderInfo** info)
{
    VALIDATE_NOT_NULL(info);

    Mutex::Autolock lock(mCapsulesLock);

    CapsuleParser::ContentProvider* provider = NULL;
    HashMap<String, CapsuleParser::ContentProvider*>::Iterator it = mContentProviders.Find(name);
    if (it != mContentProviders.End()) {
        provider = it->mSecond;
    }
    if (provider != NULL) {
        AutoPtr<IApplicationInfo> pAppInfo;
        provider->mInfo->GetApplicationInfo((IApplicationInfo**)&pAppInfo);
        Int32 pFlags;
        pAppInfo->GetFlags(&pFlags);
        if (mSettings->IsEnabledLP(provider->mInfo, flags)
            && (!mSafeMode || (pFlags & ApplicationInfo_FLAG_SYSTEM) != 0)) {
            AutoPtr<IContentProviderInfo> cpi=
                CapsuleParser::GenerateContentProviderInfo(provider, flags);
            *info = cpi;
            INTERFACE_ADDREF(*info);
            return NOERROR;
        }
    }

    *info = NULL;
    return E_DOES_NOT_EXIST;
}

ECode CCapsuleManagerService::QuerySyncProviders(
    /* [in] */ IObjectContainer* outNames,
    /* [in] */ IObjectContainer* outInfo)
{
    Mutex::Autolock lock(mCapsulesLock);

    HashMap<String, CapsuleParser::ContentProvider*>::Iterator it =
            mContentProviders.Begin();

    for (; it != mContentProviders.End(); ++it) {
        CapsuleParser::ContentProvider* p = it->mSecond;
        AutoPtr<IApplicationInfo> pAppInfo;
        p->mInfo->GetApplicationInfo((IApplicationInfo**)&pAppInfo);
        Int32 pFlags;
        pAppInfo->GetFlags(&pFlags);
        if (p->mSyncable && (!mSafeMode || (pFlags
            & ApplicationInfo_FLAG_SYSTEM) != 0)) {
            AutoPtr<ICharSequence> name;
            CStringWrapper::New(it->mFirst, (ICharSequence**)&name);
            outNames->Add(name.Get());
            outInfo->Add(CapsuleParser::GenerateContentProviderInfo(p, 0));
        }
    }
    return NOERROR;
}

ECode CCapsuleManagerService::QueryContentProviders(
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 flags,
    /* [out] */ IObjectContainer** providers)
{
    VALIDATE_NOT_NULL(providers);

    *providers = NULL;
    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<AutoPtr<IComponentName>, CapsuleParser::ContentProvider*>::Iterator it = \
                mProvidersByComponent.Begin();
        for (; it != mProvidersByComponent.End(); ++it) {
            CapsuleParser::ContentProvider* p = it->mSecond;
            String pAuth, pPName;
            p->mInfo->GetAuthority(&pAuth);
            p->mInfo->GetProcessName(&pPName);
            AutoPtr<IApplicationInfo> pAppInfo;
            p->mInfo->GetApplicationInfo((IApplicationInfo**)&pAppInfo);
            Int32 pUid, pFlags;
            pAppInfo->GetUid(&pUid);
            pAppInfo->GetFlags(&pFlags);
            if (!pAuth.IsNull()
                    && (processName.IsNull() ||
                            (pPName.Equals(processName) && pUid == uid))
                        && mSettings->IsEnabledLP(p->mInfo, flags)
                        && (!mSafeMode || (pFlags & ApplicationInfo_FLAG_SYSTEM) != 0)) {
                if (*providers == NULL) {
                    CParcelableObjectContainer::New(providers);
                }
                (*providers)->Add(CapsuleParser::GenerateContentProviderInfo(p, flags));
            }
        }
    }

//    if (finalList != null) {
//        Collections.sort(finalList, mProviderInitOrderSorter);
//    }

    return NOERROR;
}

ECode CCapsuleManagerService::GetInstrumentationInfo(
    /* [in] */ IComponentName* name,
    /* [in] */ Int32 flags,
    /* [out] */ IInstrumentationInfo** instInfo)
{
    VALIDATE_NOT_NULL(instInfo);

    Mutex::Autolock lock(mCapsulesLock);

    CapsuleParser::Instrumentation* i = NULL;
    HashMap<AutoPtr<IComponentName>, CapsuleParser::Instrumentation*>::Iterator it =
            mInstrumentation.Find(name);
    if (it != mInstrumentation.End()) {
        i = it->mSecond;
    }
    AutoPtr<IInstrumentationInfo> info =
            CapsuleParser::GenerateInstrumentationInfo(i, flags);
    *instInfo = info;
    INTERFACE_ADDREF(*instInfo);
    return NOERROR;
}

ECode CCapsuleManagerService::QueryInstrumentation(
    /* [in] */ const String& targetCapsule,
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);

    CParcelableObjectContainer::New(infos);

    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<AutoPtr<IComponentName>, CapsuleParser::Instrumentation*>::Iterator it;
        for (it = mInstrumentation.Begin(); it != mInstrumentation.End(); ++it) {
            CapsuleParser::Instrumentation* i = it->mSecond;
            String iTargetCapsule;
            i->mInfo->GetTargetCapsule(&iTargetCapsule);
            if (targetCapsule.IsNull() || targetCapsule.Equals(iTargetCapsule)) {
                (*infos)->Add(CapsuleParser::GenerateInstrumentationInfo(i, flags));
            }
        }
    }

    return NOERROR;
}

ECode CCapsuleManagerService::ScanDirLI(
    /* [in] */ IFile* dir,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 scanMode,
    /* [in] */ Int64 currentTime)
{
    assert(dir);

    ECode ec = NOERROR;
    AutoStringArray files;
    dir->List((ArrayOf<String>**)&files);
    if (files == NULL) {
        String des;
        dir->ToString(&des);
        Logger::D(TAG, StringBuffer("No files in app dir ") + des);
        return NOERROR;
    }

//    if (FALSE) {
//        String des;
//        dir->ToString(&des);
//        Logger::D(TAG, StringBuffer("Scanning app dir ") + des);
//    }

    Int32 i;
    for (i = 0; i < files->GetLength(); i++) {
        AutoPtr<IFile> file;

        FAIL_RETURN(CFile::New(dir, (*files)[i], (IFile**)&file));

        if (!IsCapsuleFilename((*files)[i])) {
            // Ignore entries which are not apk's
            continue;
        }
        CapsuleParser::Capsule* cap = ScanCapsuleLI(file,
            flags | CapsuleParser::PARSE_MUST_BE_APK, scanMode, currentTime);
        // Don't mess around with apps in system partition.
        if (cap == NULL && (flags & CapsuleParser::PARSE_IS_SYSTEM) == 0 &&
            mLastScanError == CapsuleManager::INSTALL_FAILED_INVALID_APK) {
            // Delete the apk
            String des;
            file->ToString(&des);
            Slogger::D(TAG, StringBuffer("Cleaning up failed install of ") + des);

            Boolean isDeleted = FALSE;
            file->Delete(&isDeleted);
        }
    }
    return NOERROR;
}

AutoPtr<IFile> CCapsuleManagerService::GetSettingsProblemFile()
{
    AutoPtr<IFile> dataDir, systemDir, fname;
    dataDir = Environment::GetDataDirectory();
    CFile::New(dataDir, String("system"), (IFile**)&systemDir);
    CFile::New(systemDir, String("uiderrors.txt"), (IFile**)&fname);
    return fname;
}

void CCapsuleManagerService::ReportSettingsProblem(
    /* [in] */ Int32 priority,
    /* [in] */ CString msg)
{
//	    try {
//	        File fname = getSettingsProblemFile();
//	        FileOutputStream out = new FileOutputStream(fname, true);
//	        PrintWriter pw = new PrintWriter(out);
//	        SimpleDateFormat formatter = new SimpleDateFormat();
//	        String dateString = formatter.format(new Date(System.currentTimeMillis()));
//	        pw.println(dateString + ": " + msg);
//	        pw.close();
//	        FileUtils.setPermissions(
//	                fname.toString(),
//	                FileUtils.S_IRWXU|FileUtils.S_IRWXG|FileUtils.S_IROTH,
//	                -1, -1);
//	    } catch (java.io.IOException e) {
//	    }
//	    Slog.println(priority, TAG, msg);
}

Boolean CCapsuleManagerService::CollectCertificatesLI(
    /* [in] */ CapsuleParser* cp,
    /* [in] */ CapsuleSetting* cs,
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ IFile* srcFile,
    /* [in] */ Int32 parseFlags)
{
    if (GET_CERTIFICATES) {
        Boolean isSame = FALSE;
        cs->mCodePath->Equals(srcFile, &isSame);
        Int64 timeStamp = 0;
        srcFile->LastModified(&timeStamp);
        if (cs != NULL && isSame && cs->mTimeStamp == timeStamp) {
            if (cs->mSignatures->mSignatures != NULL
                && cs->mSignatures->mSignatures->GetLength() != 0) {
                // Optimization: reuse the existing cached certificates
                // if the package appears to be unchanged.
                //todo: reference counting
                cap->mSignatures = cs->mSignatures->mSignatures->Clone();
                return TRUE;
            }

            Slogger::W(TAG, StringBuffer("CapsuleSetting for ")
                + cs->mName
                + " is missing signatures.  Collecting certs again to recover them.");
        }
        else {
            String des;
            srcFile->ToString(&des);
            Logger::I(TAG, StringBuffer(des) + " changed; collecting certs");
        }

        if (!cp->CollectCertificates(cap, parseFlags)) {
            mLastScanError = cp->GetParseError();
            return FALSE;
        }
    }
    return TRUE;
}

/*
 *  Scan a package and return the newly parsed package.
 *  Returns null in case of errors and the error code is stored in mLastScanError
 */
CapsuleParser::Capsule* CCapsuleManagerService::ScanCapsuleLI(
    /* [in] */ IFile* scanFile,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ Int64 currentTime)
{
    assert(scanFile);

    mLastScanError = CapsuleManager::INSTALL_SUCCEEDED;
    String scanPath;
    scanFile->GetPath(&scanPath);
    parseFlags |= mDefParseFlags;
    CapsuleParser cp(scanPath);
    cp.SetSeparateProcesses(*mSeparateProcesses);
    CapsuleParser::Capsule* cap = cp.ParseCapsule(scanFile,
            scanPath, mMetrics, parseFlags);
    if (cap == NULL) {
        mLastScanError = cp.GetParseError();
        return NULL;
    }

    CapsuleSetting* cs = NULL;
    CapsuleSetting* updatedCap = NULL;
    {
        Mutex::Autolock Lock(mCapsulesLock);

        // Look to see if we already know about this package.
        String oldName(NULL);
        HashMap<String, String>::Iterator it = mSettings->mRenamedCapsules.Find(cap->mCapsuleName);
        if (it != mSettings->mRenamedCapsules.End()) {
            oldName = it->mSecond;
        }
        if (cap->mOriginalCapsules != NULL &&
            Find(cap->mOriginalCapsules->Begin(), cap->mOriginalCapsules->End(), oldName)
            != cap->mOriginalCapsules->End()) {
            // This package has been renamed to its original name.  Let's
            // use that.
            cs = mSettings->PeekCapsuleLP(oldName);
        }
        // If there was no original package, see one for the real package name.
        if (cs == NULL) {
            cs = mSettings->PeekCapsuleLP(cap->mCapsuleName);
        }
        // Check to see if this package could be hiding/updating a system
        // package.  Must look for it either under the original or real
        // package name depending on our state.
        HashMap<String, AutoPtr<CapsuleSetting> >::Iterator dit
            = mSettings->mDisabledSysCapsules.Find(
                cs != NULL ? cs->mName : cap->mCapsuleName);
        if (dit != mSettings->mDisabledSysCapsules.End()) {
            updatedCap = dit->mSecond;
        }
    }
    // First check if this is a system package that may involve an update
    if (updatedCap != NULL && (parseFlags & CapsuleParser::PARSE_IS_SYSTEM) != 0) {
        Boolean isSame = FALSE;
        cs->mCodePath->Equals(scanFile, &isSame);
        if (cs != NULL && !isSame) {
            // The path has changed from what was last scanned...  check the
            // version of the new path against what we have stored to determine
            // what to do.
            if (cap->mVersionCode < cs->mVersionCode) {
                // The system package has been updated and the code path does not match
                // Ignore entry. Skip it.
                Logger::I(TAG, StringBuffer("Capsule ")
                    + cs->mName + " at " + scanFile
                    + " ignored: updated version " + cs->mVersionCode
                    + " better than this " + cap->mVersionCode);
                mLastScanError = CapsuleManager::INSTALL_FAILED_DUPLICATE_CAPSULE;
                return NULL;
            }
            else {
                // The current app on the system partion is better than
                // what we have updated to on the data partition; switch
                // back to the system partition version.
                // At this point, its safely assumed that package installation for
                // apps in system partition will go through. If not there won't be a working
                // version of the app
                {
                    Mutex::Autolock lock(mCapsulesLock);

                    // Just remove the loaded entries from package lists.
                    mCapsules.Erase(cs->mName);
                }
                Slogger::W(TAG, StringBuffer("Capsule ")
                    + cs->mName + " at " + scanFile
                    + "reverting from " + cs->mCodePathString
                    + ": new version " + cap->mVersionCode
                    + " better than installed " + cs->mVersionCode);
                //todo: when to delete args
                InstallArgs* args = new FileInstallArgs(cs->mCodePathString,
                        cs->mResourcePathString, cs->mNativeLibraryPathString, this);
                args->CleanUpResourcesLI();
                mSettings->EnableSystemCapsuleLP(cs->mName);
            }
        }
    }
    if (updatedCap != NULL) {
        // An updated system app will not have the PARSE_IS_SYSTEM flag set initially
        parseFlags |= CapsuleParser::PARSE_IS_SYSTEM;
    }
    // Verify certificates against what was last scanned
    if (!CollectCertificatesLI(&cp, cs, cap, scanFile, parseFlags)) {
        Slogger::W(TAG,
            StringBuffer("Failed verifying certificates for capsule:")
            + cap->mCapsuleName);
        return NULL;
    }
    // The apk is forward locked (not public) if its code and resources
    // are kept in different files.
    // TODO grab this value from PackageSettings
    Boolean isSame = FALSE;
    cs->mCodePath->Equals(cs->mResourcePath, &isSame);
    if (cs != NULL && !isSame) {
        parseFlags |= CapsuleParser::PARSE_FORWARD_LOCK;
    }

    String codePath;
    String resPath;
    if ((parseFlags & CapsuleParser::PARSE_FORWARD_LOCK) != 0) {
        if (cs != NULL && !cs->mResourcePathString.IsNull()) {
            resPath = cs->mResourcePathString;
        }
        else {
            // Should not happen at all. Just log an error.
            Slogger::E(TAG, StringBuffer("Resource path not set for cap : ")
                + cap->mCapsuleName);
        }
    }
    else {
        resPath = cap->mScanPath;
    }
    codePath = cap->mScanPath;
    // Set application objects path explicitly.
    SetApplicationInfoPaths(cap, codePath, resPath);
    // Note that we invoke the following method only if we are about to unpack an application
    return ScanCapsuleLI(cap, parseFlags, scanMode | SCAN_UPDATE_SIGNATURE, currentTime);
}

void CCapsuleManagerService::SetApplicationInfoPaths(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ const String& destCodePath,
    /* [in] */ const String& destResPath)
{
    assert(cap);

    cap->mScanPath = destCodePath;
    cap->mPath = destCodePath;
    cap->mApplicationInfo->SetSourceDir(destCodePath);
    cap->mApplicationInfo->SetPublicSourceDir(destResPath);
}

String CCapsuleManagerService::FixProcessName(
    /* [in] */ const String& defProcessName,
    /* [in] */ const String& processName,
    /* [in] */ Int32 uid)
{
    if (processName.IsNull()) {
        return defProcessName;
    }
    return processName;
}

Boolean CCapsuleManagerService::VerifySignaturesLP(
    /* [in] */ CapsuleSetting* capSetting,
    /* [in] */ CapsuleParser::Capsule* cap)
{
    assert(capSetting);
    assert(cap);

    if (capSetting->mSignatures->mSignatures != NULL) {
        // Already existing package. Make sure signatures match
        if (CheckSignaturesLP(capSetting->mSignatures->mSignatures, cap->mSignatures)
            != CapsuleManager_SIGNATURE_MATCH) {
            Slogger::E(TAG, StringBuffer("Capsule ") + cap->mCapsuleName
                + " signatures do not match the previously installed version; ignoring!");
            mLastScanError = CapsuleManager::INSTALL_FAILED_UPDATE_INCOMPATIBLE;
            return FALSE;
        }
    }
    // Check for shared user signatures
    if (capSetting->mSharedUser != NULL
        && capSetting->mSharedUser->mSignatures->mSignatures != NULL) {
        if (CheckSignaturesLP(capSetting->mSharedUser->mSignatures->mSignatures,
            cap->mSignatures) != CapsuleManager_SIGNATURE_MATCH) {
            Slogger::E(TAG, StringBuffer("Capsule ") + cap->mCapsuleName
                + " has no signatures that match those in shared user "
                + capSetting->mSharedUser->mName + "; ignoring!");
            mLastScanError = CapsuleManager::INSTALL_FAILED_SHARED_USER_INCOMPATIBLE;
            return FALSE;
        }
    }
    return TRUE;
}

//Int32 CCapsuleManagerService::PerformDexOptLI(
//    /* [in] */ CapsuleParser::Capsule* cap,
//    /* [in] */ Boolean forceDex)
//{
//	    boolean performed = false;
//	    if ((pkg.applicationInfo.flags&ApplicationInfo.FLAG_HAS_CODE) != 0 && mInstaller != null) {
//	        String path = pkg.mScanPath;
//	        int ret = 0;
//	        try {
//	            if (forceDex || dalvik.system.DexFile.isDexOptNeeded(path)) {
//	                ret = mInstaller.dexopt(path, pkg.applicationInfo.uid,
//	                        !isForwardLocked(pkg));
//	                pkg.mDidDexOpt = true;
//	                performed = true;
//	            }
//	        } catch (FileNotFoundException e) {
//	            Slog.w(TAG, "Apk not found for dexopt: " + path);
//	            ret = -1;
//	        } catch (IOException e) {
//	            Slog.w(TAG, "IOException reading apk: " + path, e);
//	            ret = -1;
//	        } catch (dalvik.system.StaleDexCacheError e) {
//	            Slog.w(TAG, "StaleDexCacheError when reading apk: " + path, e);
//	            ret = -1;
//	        } catch (Exception e) {
//	            Slog.w(TAG, "Exception when doing dexopt : ", e);
//	            ret = -1;
//	        }
//	        if (ret < 0) {
//	            //error from installer
//	            return DEX_OPT_FAILED;
//	        }
//	    }
//
//	    return performed ? DEX_OPT_PERFORMED : DEX_OPT_SKIPPED;
//
//    return -1;
//}

Boolean CCapsuleManagerService::UseEncryptedFilesystemForCapsule(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    Int32 capFlags;
    cap->mApplicationInfo->GetFlags(&capFlags);
    return Environment::IsEncryptedFilesystemEnabled() &&
        ((capFlags & ApplicationInfo_FLAG_NEVER_ENCRYPT) == 0);
}

Boolean CCapsuleManagerService::VerifyCapsuleUpdate(
    /* [in] */ CapsuleSetting* oldCap,
    /* [in] */ CapsuleParser::Capsule* newCap)
{
    assert(oldCap);
    assert(newCap);

    if ((oldCap->mCapFlags & ApplicationInfo_FLAG_SYSTEM) == 0) {
        Slogger::W(TAG, StringBuffer("Unable to update from ")
            + oldCap->mName + " to " + newCap->mCapsuleName
            + ": old capsule not in system partition");
        return FALSE;
    }
    else {
        HashMap<String, CapsuleParser::Capsule*>::Iterator it
            = mCapsules.Find(oldCap->mName);
        if (it != mCapsules.End()) {
            Slogger::W(TAG, StringBuffer("Unable to update from ")
                + oldCap->mName + " to " + newCap->mCapsuleName
                + ": old capsule still exists");
            return FALSE;
        }
    }
    return TRUE;
}

AutoPtr<IFile> CCapsuleManagerService::GetDataPathForCapsule(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    AutoPtr<IFile> path;

    Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule(cap);
    if (useEncryptedFSDir) {
        ASSERT_SUCCEEDED(CFile::New(mSecureAppDataDir, cap->mCapsuleName, (IFile**)&path));
    }
    else {
        ASSERT_SUCCEEDED(CFile::New(mAppDataDir, cap->mCapsuleName, (IFile**)&path));
    }
    return path;
}

CapsuleParser::Capsule* CCapsuleManagerService::ScanCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ Int64 currentTime)
{
    assert(cap);

    AutoPtr<IFile> scanFile;
    ECode ec = CFile::New(cap->mScanPath, (IFile**)&scanFile);
    String capCName, capPName, capSrcDir, capPubSrcDir, capDataDir, capNatLibDir;
    cap->mApplicationInfo->GetCapsuleName(&capCName);
    cap->mApplicationInfo->GetProcessName(&capPName);
    cap->mApplicationInfo->GetSourceDir(&capSrcDir);
    cap->mApplicationInfo->GetPublicSourceDir(&capPubSrcDir);
    cap->mApplicationInfo->GetDataDir(&capDataDir);
    cap->mApplicationInfo->GetNativeLibraryDir(&capNatLibDir);

    if (FAILED(ec) || capSrcDir.IsNull() || capPubSrcDir.IsNull()) {
        // Bail out. The resource and code paths haven't been set.
        Slogger::W(TAG, " Code and resource paths haven't been set correctly");
        mLastScanError = CapsuleManager::INSTALL_FAILED_INVALID_APK;
        return NULL;
    }
    mScanningPath = scanFile;
    if (cap->mCapsuleName.IsNull()) {
        mLastScanError = CapsuleManager::INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME;
        return NULL;
    }

    Int32 capFlags;
    if ((parseFlags & CapsuleParser::PARSE_IS_SYSTEM) != 0) {
        cap->mApplicationInfo->GetFlags(&capFlags);
        capFlags |= ApplicationInfo_FLAG_SYSTEM;
        cap->mApplicationInfo->SetFlags(capFlags);
    }

    if (cap->mCapsuleName.Equals("elastos")) {
        {
            Mutex::Autolock lock(mCapsulesLock);

            if (mElastosApplication != NULL) {
                Slogger::W(TAG, "*************************************************");
                Slogger::W(TAG, "Core android package being redefined.  Skipping.");
                Slogger::W(TAG, StringBuffer(" file=") + (Int32)mScanningPath.Get());
                Slogger::W(TAG, "*************************************************");
                mLastScanError = CapsuleManager::INSTALL_FAILED_DUPLICATE_CAPSULE;
                return NULL;
            }

            // Set up information for our fall-back user intent resolution
            // activity.
            mPlatformCapsule = cap;
            cap->mVersionCode = mSdkVersion;
            mElastosApplication = cap->mApplicationInfo;
            mResolveActivity->SetApplicationInfo(mElastosApplication);
            mResolveActivity->SetName(String("ResolverActivity") /*ResolverActivity.class.getName()*/);
            mResolveActivity->SetCapsuleName(capCName);
            mResolveActivity->SetProcessName(capPName);
            mResolveActivity->SetLaunchMode(ActivityInfo_LAUNCH_MULTIPLE);
            mResolveActivity->SetFlags(ActivityInfo_FLAG_EXCLUDE_FROM_RECENTS);
            mResolveActivity->SetTheme(0x010300a3 /*com.android.internal.R.style.Theme_Dialog_Alert*/);
            mResolveActivity->SetExported(TRUE);
            mResolveActivity->SetEnabled(TRUE);
            mResolveInfo->SetActivityInfo(mResolveActivity);
            mResolveInfo->SetPriority(0);
            mResolveInfo->SetPreferredOrder(0);
            mResolveInfo->SetMatch(0);
            mResolveComponentName = NULL;
            String raName;
            mResolveActivity->GetName(&raName);
            CComponentName::New(capCName, raName,
                    (IComponentName**)&mResolveComponentName);
        }
    }

    if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0 && Config::LOGD) {
        Logger::D(TAG, StringBuffer("Scanning capsule ") + cap->mCapsuleName);
    }

    if (mCapsules.Find(cap->mCapsuleName) != mCapsules.End()
        || mSharedLibraries.Find(cap->mCapsuleName) != mSharedLibraries.End()) {
        Slogger::W(TAG, StringBuffer("Application package ") + cap->mCapsuleName
                + " already installed.  Skipping duplicate.");
        mLastScanError = CapsuleManager::INSTALL_FAILED_DUPLICATE_CAPSULE;
        return NULL;
    }

    // Initialize package source and resource directories
    AutoPtr<IFile> destCodeFile, destResourceFile;
    ASSERT_SUCCEEDED(CFile::New(capSrcDir, (IFile**)&destCodeFile));
    ASSERT_SUCCEEDED(CFile::New(capPubSrcDir, (IFile**)&destResourceFile));

    SharedUserSetting* suid = NULL;
    CapsuleSetting* capSetting = NULL;

    if (!IsSystemApp(cap)) {
        // Only system apps can use these features.
        if (cap->mOriginalCapsules != NULL) {
            cap->mOriginalCapsules->Clear();
            delete cap->mOriginalCapsules;
            cap->mOriginalCapsules = NULL;
        }
        cap->mRealCapsule = NULL;
        if (cap->mAdoptPermissions != NULL) {
            cap->mAdoptPermissions->Clear();
            delete cap->mAdoptPermissions;
            cap->mAdoptPermissions = NULL;
        }
    }

    {
        Mutex::Autolock lock(mCapsulesLock);
        // Check all shared libraries and map to their actual file path.
        if (cap->mUsesLibraries != NULL || cap->mUsesOptionalLibraries != NULL) {
            if (mTmpSharedLibraries == NULL ||
                mTmpSharedLibraries->GetLength() < (Int32)mSharedLibraries.GetSize()) {
                if (mTmpSharedLibraries != NULL) {
                    //todo: free String
                    ArrayOf<String>::Free(mTmpSharedLibraries);
                }
                mTmpSharedLibraries = ArrayOf<String>::Alloc((Int32)mSharedLibraries.GetSize());
            }
            Int32 num = 0;
            if (cap->mUsesLibraries != NULL) {
                List<String>::Iterator it;
                for (it = cap->mUsesLibraries->Begin();
                     it != cap->mUsesLibraries->End(); ++it) {
                    String file;
                    HashMap<String, String>::Iterator it1 = mSharedLibraries.Find(*it);
                    if (it1 != mSharedLibraries.End()) {
                        file = it1->mSecond;
                    }
                    if (file.IsNull()) {
                        Slogger::E(TAG, StringBuffer("Capsule ") + cap->mCapsuleName
                            + " requires unavailable shared library " + *it + "; failing!");
                        mLastScanError = CapsuleManager::INSTALL_FAILED_MISSING_SHARED_LIBRARY;
                        return NULL;
                    }
                    (*mTmpSharedLibraries)[num++] = file;
                }
            }
            if (cap->mUsesOptionalLibraries != NULL) {
                List<String>::Iterator it;
                for (it = cap->mUsesOptionalLibraries->Begin();
                     it != cap->mUsesOptionalLibraries->End(); it++) {
                    String file;
                    HashMap<String, String>::Iterator it1 = mSharedLibraries.Find(*it);
                    if (it1 != mSharedLibraries.End()) {
                        file = it1->mSecond;
                    }
                    if (file.IsNull()) {
                        Slogger::W(TAG, StringBuffer("Capsule ") + cap->mCapsuleName
                            + " desires unavailable shared library " + *it + "; ignoring!");
                    }
                    else {
                        (*mTmpSharedLibraries)[num++] = file;
                    }
                }
            }
            if (num > 0) {
                //TODO: cap->mUsesLibraryFiles maybe is not null
                cap->mUsesLibraryFiles = ArrayOf<String>::Alloc(num);
                for (Int32 i = 0; i < num; i++) {
                    (*cap->mUsesLibraryFiles)[i] = (*mTmpSharedLibraries)[i];
                }
            }
        }

        if (cap->mSharedUserId != NULL) {
            suid = mSettings->GetSharedUserLP(cap->mSharedUserId,
                    capFlags, TRUE);
            if (suid == NULL) {
                Slogger::W(TAG, StringBuffer("Creating application capsule ")
                    + cap->mCapsuleName + " for shared user failed");
                mLastScanError = CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
                return NULL;
            }
            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0 && Config::LOGD) {
                Logger::D(TAG, StringBuffer("Shared UserID ")
                    + cap->mSharedUserId + " (uid="
                    + suid->mUserId + "): capsules=" /*+ suid->mCapsules*/);
            }
        }

//        if (FALSE) {
//            Logger::W(TAG, "WAITING FOR DEBUGGER");
//	            Debug::WaitForDebugger();
//            Logger::I(TAG, StringBuffer("Capsule ")
//                + cap->mCapsuleName + " from original capsules"
//                /*+ cap->mOriginalCapsules*/);
//        }

        // Check if we are renaming from an original package name.
        CapsuleSetting* origCapsule = NULL;
        String realName;
        if (cap->mOriginalCapsules != NULL) {
            // This package may need to be renamed to a previously
            // installed name.  Let's check on that...
            String renamed;
            HashMap<String, String>::Iterator it = mSettings->mRenamedCapsules.Find(cap->mRealCapsule);
            if (it != mSettings->mRenamedCapsules.End()) {
                renamed = it->mSecond;
            }
            if (Find(cap->mOriginalCapsules->Begin(), cap->mOriginalCapsules->End(), renamed)
                != cap->mOriginalCapsules->End()) {
                // This package had originally been installed as the
                // original name, and we have already taken care of
                // transitioning to the new one.  Just update the new
                // one to continue using the old name.
                realName = cap->mRealCapsule;
                if (!cap->mCapsuleName.Equals(renamed)) {
                    // Callers into this function may have already taken
                    // care of renaming the package; only do it here if
                    // it is not already done.
                    cap->SetCapsuleName(renamed);
                }

            }
            else {
                List<String>::ReverseIterator rit;
                for (rit = cap->mOriginalCapsules->RBegin();
                     rit != cap->mOriginalCapsules->REnd(); ++rit) {
                    if ((origCapsule = mSettings->PeekCapsuleLP(*rit)) != NULL) {
                        // We do have the package already installed under its
                        // original name...  should we use it?
                        if (!VerifyCapsuleUpdate(origCapsule, cap)) {
                            // New package is not compatible with original.
                            origCapsule = NULL;
                            continue;
                        }
                        else if (origCapsule->mSharedUser != NULL) {
                            // Make sure uid is compatible between packages.
                            if (!origCapsule->mSharedUser->mName.Equals(cap->mSharedUserId)) {
                                Slogger::W(TAG,
                                    StringBuffer("Unable to migrate data from ")
                                    + origCapsule->mName
                                    + " to " + cap->mCapsuleName + ": old uid "
                                    + origCapsule->mSharedUser->mName
                                    + " differs from " + cap->mSharedUserId);
                                origCapsule = NULL;
                                continue;
                            }
                        }
                        else {
                            if (DEBUG_UPGRADE) {
                                Logger::V(TAG,
                                    StringBuffer("Renaming new capsule ")
                                    + cap->mCapsuleName + " to old name "
                                    + origCapsule->mName);
                            }
                        }
                        break;
                    }
                }
            }
        }

        if (mTransferedCapsules.Find(cap->mCapsuleName)
            != mTransferedCapsules.End()) {
            Slogger::W(TAG, StringBuffer("Capsule ")
                + cap->mCapsuleName
                + " was transferred to another, but its .apk remains");
        }

        // Just create the setting, don't add it yet. For already existing packages
        // the PkgSetting exists already and doesn't have to be created.
        capSetting = mSettings->GetCapsuleLP(
            cap, origCapsule, realName, suid, destCodeFile.Get(),
            destResourceFile.Get(), capNatLibDir, capFlags, TRUE, FALSE);
        if (capSetting == NULL) {
            Slogger::W(TAG, StringBuffer("Creating application capsule ")
                + cap->mCapsuleName + " failed");
            mLastScanError = CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
            return NULL;
        }

        if (capSetting->mOrigCapsule != NULL) {
            // If we are first transitioning from an original package,
            // fix up the new package's name now.  We need to do this after
            // looking up the package under its new name, so getPackageLP
            // can take care of fiddling things correctly.
            cap->SetCapsuleName(origCapsule->mName);

            // File a report about this.
            String msg = String((const char*)(StringBuffer("New capsule ")
                + capSetting->mRealName
                + " renamed to replace old capsule " + capSetting->mName));
            ReportSettingsProblem(5 /*Log::WARN*/, msg);

            // Make a note of it.
            mTransferedCapsules.Insert(origCapsule->mName);

            // No longer need to retain this.
            capSetting->mOrigCapsule = NULL;
        }

        if (!realName.IsNull()) {
            // Make a note of it.
            mTransferedCapsules.Insert(cap->mCapsuleName);
        }

        CapsuleSetting* cs = NULL;
        HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it
                = mSettings->mDisabledSysCapsules.Find(cap->mCapsuleName);
        if (it != mSettings->mDisabledSysCapsules.End()) {
            cs = it->mSecond;
        }
        if (cs != NULL) {
            capFlags |= ApplicationInfo_FLAG_UPDATED_SYSTEM_APP;
            cap->mApplicationInfo->SetFlags(capFlags);
        }

        cap->mApplicationInfo->SetUid(capSetting->mUserId);
        cap->mExtras = capSetting;

        if (!VerifySignaturesLP(capSetting, cap)) {
            if ((parseFlags & CapsuleParser::PARSE_IS_SYSTEM_DIR) == 0) {
                return NULL;
            }
            // The signature has changed, but this package is in the system
            // image...  let's recover!
            //todo: reference counting
            ArrayOf< AutoPtr<ISignature> >::Free(capSetting->mSignatures->mSignatures.Get());
            capSetting->mSignatures->mSignatures = cap->mSignatures->Clone();
            // However...  if this package is part of a shared user, but it
            // doesn't match the signature of the shared user, let's fail.
            // What this means is that you can't change the signatures
            // associated with an overall shared user, which doesn't seem all
            // that unreasonable.
            if (capSetting->mSharedUser != NULL) {
                if (CheckSignaturesLP(capSetting->mSharedUser->mSignatures->mSignatures,
                    cap->mSignatures) != CapsuleManager_SIGNATURE_MATCH) {
                    Logger::W(TAG,
                        StringBuffer("Signature mismatch for shared user : ")
                        + capSetting->mSharedUser);
                    mLastScanError = CapsuleManager::INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES;
                    return NULL;
                }
            }
            // File a report about this.
            String msg = String((const char*)(StringBuffer("System capsule ")
                + cap->mCapsuleName + " signature changed; retaining data."));
            ReportSettingsProblem(5 /*Log::WARN*/, msg);
        }

        // Verify that this new package doesn't have any content providers
        // that conflict with existing packages.  Only do this if the
        // package isn't already installed, since we don't want to break
        // things that are installed.
        if ((scanMode & SCAN_NEW_INSTALL) != 0) {
            List<CapsuleParser::ContentProvider*>::Iterator it;
            for (it = cap->mContentProviders.Begin();
                 it != cap->mContentProviders.End(); ++it) {
                CapsuleParser::ContentProvider* c = *it;
                String cAuth;
                c->mInfo->GetAuthority(&cAuth);
                if (!cAuth.IsNull()) {
                    StringTokenizer token((const char*)cAuth, ";");
                    while (token.HasMoreTokens()) {
                        String name = token.NextToken();
                        HashMap<String, CapsuleParser::ContentProvider*>::Iterator
                            it = mContentProviders.Find(name);
                        if (it != mContentProviders.End()) {
                            CapsuleParser::ContentProvider* other = it->mSecond;
                            String capsuleName("?");
                            if (other) {
                                AutoPtr<IComponentName> cn;
                                other->GetComponentName((IComponentName**)&cn);
                                if (cn != NULL) {
                                    cn->GetCapsuleName(&capsuleName);
                                }
                            }
                            Slogger::W(TAG,
                                StringBuffer("Can't install because provider name ")
                                + name + " (in capsule " + capCName
                                + ") is already used by " + capsuleName);
                            mLastScanError = CapsuleManager::INSTALL_FAILED_CONFLICTING_PROVIDER;
                            return NULL;
                        }

                    }
                }
            }
        }
    }

    String capName = cap->mCapsuleName;

    if (cap->mAdoptPermissions != NULL) {
        // This package wants to adopt ownership of permissions from
        // another package.
        List<String>::ReverseIterator rit;
        for (rit = cap->mAdoptPermissions->RBegin();
             rit != cap->mAdoptPermissions->REnd(); ++rit) {
            String origName = *rit;
            CapsuleSetting* orig = mSettings->PeekCapsuleLP(origName);
            if (orig != NULL) {
                if (VerifyCapsuleUpdate(orig, cap)) {
                    Slogger::I(TAG, StringBuffer("Adopting permissions from ")
                        + origName + " to " + cap->mCapsuleName);
                    mSettings->TransferPermissions(origName, cap->mCapsuleName);
                }
            }
        }
    }

    Int32 capUid;
    cap->mApplicationInfo->GetUid(&capUid);

    Int64 scanFileTime = 0;
    scanFile->LastModified(&scanFileTime);
    Boolean forceDex = (scanMode & SCAN_FORCE_DEX) != 0;
    Boolean scanFileNewer = forceDex || scanFileTime != capSetting->mTimeStamp;
    capPName = FixProcessName(capCName, capPName, capUid);
    cap->mApplicationInfo->SetProcessName(capPName);

    AutoPtr<IFile> dataPath;
    if (mPlatformCapsule.Get() == cap) {
        // The system package is special.
        ASSERT_SUCCEEDED(CFile::New(Environment::GetDataDirectory(),
                String("system"), (IFile**)&dataPath));
        dataPath->GetPath(&capDataDir);
        cap->mApplicationInfo->SetDataDir(capDataDir);
    }
    else {
        // This is a normal package, need to make its data directory.
        Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule(cap);
        dataPath = GetDataPathForCapsule(cap);

        Boolean uidError = FALSE;

        Boolean isExists = FALSE;
        if (dataPath->Exists(&isExists), isExists) {
            (*mOutPermissions)[1] = 0;
            String path;
            dataPath->GetPath(&path);
            FileUtils::GetPermissions(path, mOutPermissions.Get());

            // If we have mismatched owners for the data path, we have a
            // problem (unless we're running in the simulator.)
            if ((*mOutPermissions)[1] != capUid && Process::SupportsProcesses()) {
                Boolean recovered = FALSE;
                if ((parseFlags & CapsuleParser::PARSE_IS_SYSTEM) != 0) {
                    // If this is a system app, we can at least delete its
                    // current data so the application will still work.
                    if (mInstaller != NULL) {
                        Int32 ret = mInstaller->Remove(capName, useEncryptedFSDir);
                        if (ret >= 0) {
                            // Old data gone!
                            String msg = String((const char*)(
                                StringBuffer("System capsule ")
                                + cap->mCapsuleName
                                + " has changed from uid: "
                                + mOutPermissions[1] + " to "
                                + capUid + "; old data erased"));
                            ReportSettingsProblem(5 /*Log::WARN*/, msg);
                            recovered = TRUE;

                            // And now re-install the app.
                            ret = mInstaller->Install(
                                capName, useEncryptedFSDir, capUid, capUid);
                            if (ret == -1) {
                                // Ack should not happen!
                                msg = String((const char*)(
                                    StringBuffer("System capsule ")
                                    + cap->mCapsuleName
                                    + " could not have data directory re-created after delete."));
                                ReportSettingsProblem(5 /*Log::WARN*/, msg);
                                mLastScanError = CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
                                return NULL;
                            }
                        }
                    }
                    if (!recovered) {
                        mHasSystemUidErrors = TRUE;
                    }
                }
                if (!recovered) {
                    capDataDir = String((const char*)(
                        StringBuffer("/mismatched_uid/settings_")
                        + capUid + "/fs_" + mOutPermissions[1]));
                    cap->mApplicationInfo->SetDataDir(capDataDir);
                    capNatLibDir = capDataDir;
                    cap->mApplicationInfo->SetNativeLibraryDir(capNatLibDir);
                    String msg = String((const char*)("Capsule " + cap->mCapsuleName
                            + " has mismatched uid: "
                            + mOutPermissions[1] + " on disk, "
                            + capUid + " in settings"));
                    {
                        Mutex::Autolock lock(mCapsulesLock);

                        mSettings->mReadMessages = msg;
                        mSettings->mReadMessages += "\n";
                        uidError = TRUE;
                        if (!capSetting->mUidError) {
                            ReportSettingsProblem(6 /*Log::ERROR*/, msg);
                        }
                    }
                }
            }
            dataPath->GetPath(&capDataDir);
            cap->mApplicationInfo->SetDataDir(capDataDir);
        }
        else {
            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0 && Config::LOGV) {
                Logger::V(TAG, StringBuffer("Want this data dir: ") + dataPath);
            }
            //invoke installer to do the actual installation
            if (mInstaller != NULL) {
                Int32 ret = mInstaller->Install(
                    capName, useEncryptedFSDir, capUid, capUid);
                if (ret < 0) {
                    // Error from installer
                    mLastScanError = CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
                    return NULL;
                }
            }
            else {
                Boolean isSucceeded = FALSE, isExist = FALSE;
                dataPath->Mkdirs(&isSucceeded);
                if (dataPath->Exists(&isExist), isExist) {
                    String des;
                    dataPath->ToString(&des);
                    FileUtils::SetPermissions(des,
                        FileUtils::IRWXU | FileUtils::IRWXG | FileUtils::IXOTH,
                        capUid, capUid);
                }
            }
            Boolean isExist = FALSE;
            if (dataPath->Exists(&isExist), isExist) {
                dataPath->GetPath(&capDataDir);
                cap->mApplicationInfo->SetDataDir(capDataDir);
            }
            else {
                Slogger::W(TAG,
                    StringBuffer("Unable to create data directory: ") + dataPath);
                capDataDir = NULL;
                cap->mApplicationInfo->SetDataDir(capDataDir);
            }
        }

        /*
         * Set the data dir to the default "/data/data/<package name>/lib"
         * if we got here without anyone telling us different (e.g., apps
         * stored on SD card have their native libraries stored in the ASEC
         * container with the APK).
         *
         * This happens during an upgrade from a package settings file that
         * doesn't have a native library path attribute at all.
         */
        if (capNatLibDir.IsNull() && capDataDir.IsNull()) {
            if (capSetting->mNativeLibraryPathString.IsNull()) {
                AutoPtr<IFile> f;
                String nativeLibraryPath;
                ASSERT_SUCCEEDED(CFile::New(dataPath, String(LIB_DIR_NAME), (IFile**)&f));
                f->GetPath(&nativeLibraryPath);
                capNatLibDir = nativeLibraryPath;
                cap->mApplicationInfo->SetNativeLibraryDir(capNatLibDir);
                capSetting->mNativeLibraryPathString = nativeLibraryPath;
            }
            else {
                capNatLibDir = capSetting->mNativeLibraryPathString;
                cap->mApplicationInfo->SetNativeLibraryDir(capNatLibDir);
            }
        }

        capSetting->mUidError = uidError;
    }

    // If we're running in the simulator, we don't need to unpack anything.
    if (mInstaller != NULL) {
        String path;
        scanFile->GetPath(&path);
        /* Note: We don't want to unpack the native binaries for
         *        system applications, unless they have been updated
         *        (the binaries are already under /system/lib)->
         *        Also, don't unpack libs for apps on the external card
         *        since they should have their libraries in the ASEC
         *        container already->
         *
         *        In other words, we're going to unpack the binaries
         *        only for non-system apps and system app upgrades->
         */
        if (!capNatLibDir.IsNull()) {
            AutoPtr<IFile> nativeLibraryDir;
            String dataPathString;
            String parentPath;

            ASSERT_SUCCEEDED(CFile::New(capNatLibDir, (IFile**)&nativeLibraryDir));
            dataPath->GetPath(&dataPathString);
            nativeLibraryDir->GetParent(&parentPath);

            if (IsSystemApp(cap) && !IsUpdatedSystemApp(cap)) {
                /*
                 * Upgrading from a previous version of the OS sometimes
                 * leaves native libraries in the /data/data/<app>/lib
                 * directory for system apps even when they shouldn't be->
                 * Recent changes in the JNI library search path
                 * necessitates we remove those to match previous behavior->
                 */
                if (NativeLibraryHelper::RemoveNativeBinariesFromDirLI(nativeLibraryDir)) {
                    Logger::I(TAG,
                        StringBuffer("removed obsolete native libraries for system capsule ")
                        + path);
                }
            }
            else if (parentPath.Equals(dataPathString)) {
                /*
                 * If this is an internal application or our
                 * nativeLibraryPath points to our data directory, unpack
                 * the libraries-> The native library path pointing to the
                 * data directory for an application in an ASEC container
                 * can happen for older apps that existed before an OTA to
                 * Gingerbread->
                 */
                Slogger::I(TAG, StringBuffer("Unpacking native libraries for ")
                    + path);
                mInstaller->UnlinkNativeLibraryDirectory(dataPathString);
                NativeLibraryHelper::CopyNativeBinariesLI(scanFile, nativeLibraryDir);
            }
            else {
                Slogger::I(TAG, StringBuffer("Linking native library dir for ")
                    + path);
                mInstaller->LinkNativeLibraryDirectory(dataPathString, capNatLibDir);
            }
        }
        cap->mScanPath = path;

//        if ((scanMode & SCAN_NO_DEX) == 0) {
//            if (PerformDexOptLI(cap, forceDex) == DEX_OPT_FAILED) {
//                mLastScanError = CapsuleManager::INSTALL_FAILED_DEXOPT;
//                return NULL;
//            }
//        }
    }

    if (mFactoryTest && Find(cap->mRequestedPermissions.Begin(), cap->mRequestedPermissions.End(),
        String("")/*android.Manifest.permission.FACTORY_TEST*/) != cap->mRequestedPermissions.End()) {
        capFlags |= ApplicationInfo_FLAG_FACTORY_TEST;
        cap->mApplicationInfo->SetFlags(capFlags);
    }

    // Request the ActivityManager to kill the process(only for existing packages)
    // so that we do not end up in a confused state while the user is still using the older
    // version of the application while the new one gets installed.
    if ((parseFlags & CapsuleManager::INSTALL_REPLACE_EXISTING) != 0) {
        KillApplication(capCName, capUid);
    }

    {
        Mutex::Autolock lock(mCapsulesLock);

        // We don't expect installation to fail beyond this point,
        if ((scanMode & SCAN_MONITOR) != 0) {
            mAppDirs[cap->mPath] = cap;
        }
        // Add the new setting to mSettings
        mSettings->InsertCapsuleSettingLP(capSetting, cap);
        // Add the new setting to mPackages
        mCapsules[capCName] = cap;
        // Make sure we don't accidentally delete its data.
        mSettings->mCapsulesToBeCleaned.Remove(capName);

        // Take care of first install / last update times.
        if (currentTime != 0) {
            if (capSetting->mFirstInstallTime == 0) {
                capSetting->mFirstInstallTime = capSetting->mLastUpdateTime = currentTime;
            }
            else if ((scanMode & SCAN_UPDATE_TIME) != 0) {
                capSetting->mLastUpdateTime = currentTime;
            }
        }
        else if (capSetting->mFirstInstallTime == 0) {
            // We need *something*.  Take time time stamp of the file.
            capSetting->mFirstInstallTime = capSetting->mLastUpdateTime = scanFileTime;
        }
        else if ((parseFlags & CapsuleParser::PARSE_IS_SYSTEM_DIR) != 0) {
            if (scanFileTime != capSetting->mTimeStamp) {
                // A package on the system image has changed; consider this
                // to be an update.
                capSetting->mLastUpdateTime = scanFileTime;
            }
        }

        StringBuffer* r = NULL;
        List<CapsuleParser::ContentProvider*>::Iterator cpit;
        for (cpit = cap->mContentProviders.Begin();
             cpit != cap->mContentProviders.End(); ++cpit) {
            CapsuleParser::ContentProvider* p = *cpit;
            String pName, pCName, pPName, pAuth;
            p->mInfo->GetName(&pName);
            p->mInfo->GetCapsuleName(&pCName);
            p->mInfo->GetProcessName(&pPName);
            pPName = FixProcessName(capPName, pPName, capUid);
            p->mInfo->SetProcessName(pPName);
            AutoPtr<IComponentName> cn;
            CComponentName::New(pCName, pName, (IComponentName**)&cn);
            mProvidersByComponent[cn] = p;
            p->mInfo->GetSyncable(&p->mSyncable);
            p->mInfo->GetAuthority(&pAuth);
            if (!pAuth.IsNull()) {
                StringTokenizer tokenizer(pAuth, String(";"));
                Int32 j = 0;
                while (tokenizer.HasMoreTokens()) {
                    String name = tokenizer.NextToken();
                    if (j == 1 && p->mSyncable) {
                        // We only want the first authority for a provider to possibly be
                        // syncable, so if we already added this provider using a different
                        // authority clear the syncable flag. We copy the provider before
                        // changing it because the mContentProviders object contains a reference
                        // to a provider that we don't want to change.
                        // Only do this for the second authority since the resulting provider
                        // object can be the same for all future authorities for this provider.
                        p = new CapsuleParser::ContentProvider(p);
                        p->mInfo->GetName(&pName);
                        p->mInfo->GetCapsuleName(&pCName);
                        p->mInfo->GetProcessName(&pPName);
                        p->mInfo->GetAuthority(&pAuth);
                        p->mSyncable = FALSE;
                    }
                    if (mContentProviders.Find(name) == mContentProviders.End()) {
                        // TODO: mContentProviders
                        mContentProviders[name] = p;
                        if (pAuth.IsNull()) {
                            pAuth = name;
                            p->mInfo->SetAuthority(pAuth);
                        }
                        else {
                            pAuth = pAuth + ";" + name;
                            p->mInfo->SetAuthority(pAuth);
                        }
                        if ((parseFlags&CapsuleParser::PARSE_CHATTY) != 0 && Config::LOGD) {
                            Boolean pSyncable;
                            p->mInfo->GetSyncable(&pSyncable);
                            Logger::D(TAG, StringBuffer("Registered content provider: ")
                                + name
                                + ", className = " + pName
                                + ", isSyncable = " + pSyncable);
                        }
                    }
                    else {
                        CapsuleParser::ContentProvider* other = NULL;
                        HashMap<String, CapsuleParser::ContentProvider*>::Iterator it
                                = mContentProviders.Find(name);
                        if (it != mContentProviders.End()) {
                            other = it->mSecond;
                        }
                        String capsuleName("?");
                        if (other) {
                            AutoPtr<IComponentName> comName;
                            other->GetComponentName((IComponentName**)&comName);
                            if (comName != NULL) {
                                comName->GetCapsuleName(&capsuleName);
                            }
                        }
                        Slogger::W(TAG,
                            StringBuffer("Skipping provider name ")
                            + name + " (in capsule "
                            + capCName + "): name already used by "
                            + capsuleName);
                    }
                    j++;
                }
            }
            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuffer;
                }
                else {
                    *r += " ";
                }
                *r += pName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) {
                Logger::D(TAG, StringBuffer("  Providers: ") + *r);
            }
        }

        delete r;
        r = NULL;
        List<CapsuleParser::Service*>::Iterator csit;
        for (csit = cap->mServices.Begin();
             csit != cap->mServices.End(); ++csit) {
            CapsuleParser::Service* s = *csit;
            String sPName;
            s->mInfo->GetProcessName(&sPName);
            sPName = FixProcessName(capPName, sPName, capUid);
            s->mInfo->SetProcessName(sPName);
            mServices->AddService(s);
            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuffer;
                }
                else {
                    *r += " ";
                }
                String sName;
                s->mInfo->GetName(&sName);
                *r = sName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) {
                Logger::D(TAG, StringBuffer("  Services: ") + *r);
            }
        }

        delete r;
        r = NULL;
        List<CapsuleParser::Activity*>::Iterator cait;
        for (cait = cap->mReceivers.Begin();
             cait != cap->mReceivers.End(); ++cait) {
            CapsuleParser::Activity* a = *cait;
            String aPName;
            a->mInfo->GetProcessName(&aPName);
            aPName = FixProcessName(capPName, aPName, capUid);
            a->mInfo->SetProcessName(aPName);
            mReceivers->AddActivity(a, String("receiver"));
            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuffer;
                }
                else {
                    *r += " ";
                }
                String aName;
                a->mInfo->GetName(&aName);
                *r += aName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) {
                Logger::D(TAG, StringBuffer("  Receivers: ") + *r);
            }
        }

        delete r;
        r = NULL;
        for (cait = cap->mActivities.Begin();
             cait != cap->mActivities.End(); ++cait) {
            CapsuleParser::Activity* a = *cait;
            String aPName;
            a->mInfo->GetProcessName(&aPName);
            String n = FixProcessName(capPName, aPName, capUid);
            mActivities->AddActivity(a, "activity");
            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuffer;
                }
                else {
                    *r += "";
                }
                String aName;
                a->mInfo->GetName(&aName);
                *r += aName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) {
                Logger::D(TAG, StringBuffer("  Activities: ") + *r);
            }
        }

        delete r;
        r = NULL;
        List<CapsuleParser::PermissionGroup*>::Iterator cpgit;
        for (cpgit = cap->mPermissionGroups.Begin();
             cpgit != cap->mPermissionGroups.End(); ++cpgit) {
            CapsuleParser::PermissionGroup* pg = *cpgit;
            String pgName;
            pg->mInfo->GetName(&pgName);
            CapsuleParser::PermissionGroup* cur = NULL;
            HashMap<String, CapsuleParser::PermissionGroup*>::Iterator it
                = mPermissionGroups.Find(pgName);
            if (it != mPermissionGroups.End()) {
                cur = it->mSecond;
            }
            if (cur == NULL) {
                mPermissionGroups[pgName] = pg;
                if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                    if (r == NULL) {
                        r = new StringBuffer;
                    }
                    else {
                        *r += " ";
                    }
                        *r += pgName;
                }
            }
            else {
                String pgCName, curCName;
                pg->mInfo->GetCapsuleName(&pgCName);
                cur->mInfo->GetCapsuleName(&curCName);
                Slogger::W(TAG, StringBuffer("Permission group ")
                    + pgName + " from capsule "
                    + pgCName + " ignored: original from "
                    + curCName);
                if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                    if (r == NULL) {
                        r = new StringBuffer;
                    }
                    else {
                        *r += " ";
                    }
                    *r += "DUP:";
                    *r += pgName;
                }
            }
        }
        if (r != NULL) {
            if (Config::LOGD) {
                Logger::D(TAG, StringBuffer("  Permission Groups: ") + *r);
            }
        }

        delete r;
        r = NULL;
        List<CapsuleParser::Permission*>::Iterator cpermit;
        for (cpermit = cap->mPermissions.Begin();
             cpermit != cap->mPermissions.End(); ++cpermit) {
            CapsuleParser::Permission* p = *cpermit;
            String pName, pCName, pGroup;
            p->mInfo->GetName(&pName);
            p->mInfo->GetCapsuleName(&pCName);
            p->mInfo->GetGroup(&pGroup);
            HashMap<String, BasePermission*>& permissionMap = p->mTree
                ? mSettings->mPermissionTrees : mSettings->mPermissions;
            HashMap<String, CapsuleParser::PermissionGroup*>::Iterator it
                = mPermissionGroups.Find(pGroup);
            if (it != mPermissionGroups.End()) {
                p->mGroup = it->mSecond;
            }
            if (pGroup.IsNull() || p->mGroup != NULL) {
                BasePermission* bp = NULL;
                HashMap<String, BasePermission*>::Iterator it1 =
                    permissionMap.Find(pName);
                if (it1 != permissionMap.End()) {
                    bp = it1->mSecond;
                }
                if (bp == NULL) {
                    bp = new BasePermission(pName, pCName, BasePermission::TYPE_NORMAL);
                    permissionMap[pName] = bp;
                }
                if (bp->mPerm == NULL) {
                    if (bp->mSourceCapsule.IsNull()
                        || bp->mSourceCapsule.Equals(pCName)) {
                        BasePermission* tree = FindPermissionTreeLP(pName);
                        if (tree == NULL
                            || tree->mSourceCapsule.Equals(pCName)) {
                            bp->mCapsuleSetting = capSetting;
                            bp->mPerm = p;
                            cap->mApplicationInfo->GetUid(&bp->mUid);
                            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                                if (r == NULL) {
                                    r = new StringBuffer;
                                }
                                else {
                                    *r += " ";
                                }
                                *r += pName;
                            }
                        }
                        else {
                            Slogger::W(TAG, StringBuffer("Permission ")
                                + pName + " from capsule "
                                + pCName + " ignored: base tree "
                                + tree->mName + " is from capsule "
                                + tree->mSourceCapsule);
                        }
                    }
                    else {
                        Slogger::W(TAG, StringBuffer("Permission ")
                            + pName + " from capsule "
                            + pCName + " ignored: original from "
                            + bp->mSourceCapsule);
                    }
                }
                else if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                    if (r == NULL) {
                        r = new StringBuffer;
                    }
                    else {
                        *r += " ";
                    }
                    *r += "DUP:";
                    *r += pName;
                }
                if (bp->mPerm == p) {
                    p->mInfo->GetProtectionLevel(&bp->mProtectionLevel);
                }
            }
            else {
                Slogger::W(TAG, StringBuffer("Permission ")
                    + pName + " from capsule "
                    + pCName + " ignored: no group "
                    + p->mGroup);
            }
        }
        if (r != NULL) {
            if (Config::LOGD) {
                Logger::D(TAG, StringBuffer("  Permissions: ") + r);
            }
        }

        delete r;
        r = NULL;
        List<CapsuleParser::Instrumentation*>::Iterator ciit;
        for (ciit = cap->mInstrumentation.Begin();
             ciit != cap->mInstrumentation.End(); ++ciit) {
            CapsuleParser::Instrumentation* a = *ciit;
            a->mInfo->SetCapsuleName(capCName);
            a->mInfo->SetSourceDir(capSrcDir);
            a->mInfo->SetPublicSourceDir(capPubSrcDir);
            a->mInfo->SetDataDir(capDataDir);
            a->mInfo->SetNativeLibraryDir(capNatLibDir);
            AutoPtr<IComponentName> comName;
            a->GetComponentName((IComponentName**)&comName);
            mInstrumentation[comName] = a;
            if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0) {
                if (r == NULL) {
                    r = new StringBuffer;
                }
                else {
                    *r += " ";
                }
                String aName;
                a->mInfo->GetName(&aName);
                *r += aName;
            }
        }
        if (r != NULL) {
            if (Config::LOGD) {
                Logger::D(TAG, StringBuffer("  Instrumentation: ") + r);
            }
        }

        if (cap->mProtectedBroadcasts != NULL) {
            List<String>::Iterator it;
            for (it = cap->mProtectedBroadcasts->Begin();
                 it != cap->mProtectedBroadcasts->End(); ++it) {
                mProtectedBroadcasts.Insert(*it);
            }
        }

        capSetting->SetTimeStamp(scanFileTime);
    }

    return cap;
}

void CCapsuleManagerService::KillApplication(
    /* [in] */ const String& capName,
    /* [in] */ Int32 uid)
{
    // Request the ActivityManager to kill the process(only for existing packages)
    // so that we do not end up in a confused state while the user is still using the older
    // version of the application while the new one gets installed.
    AutoPtr<IActivityManager> am;
    ActivityManagerNative::GetDefault((IActivityManager**)&am);
    if (am != NULL) {
        am->KillApplicationWithUid(capName, uid);
    }
}

AutoPtr<IFile> CCapsuleManagerService::GetNativeBinaryDirForCapsule(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    AutoPtr<IFile> f;
    String nativeLibraryDir;
    cap->mApplicationInfo->GetNativeLibraryDir(&nativeLibraryDir);;
    if (!nativeLibraryDir.IsNull()) {
        CFile::New(nativeLibraryDir, (IFile**)&f);
    }
    else {
        // Fall back for old packages
        String dataDir;
        cap->mApplicationInfo->GetDataDir(&dataDir);
        CFile::New(dataDir,
                String(LIB_DIR_NAME), (IFile**)&f);
    }
    return f;
}

void CCapsuleManagerService::RemoveCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Boolean chatty)
{
    String capCName;
    cap->mApplicationInfo->GetCapsuleName(&capCName);
    if (chatty && Config::LOGD) {
        Logger::D(TAG, StringBuffer("Removing capsule ") + capCName);
    }

    Mutex::Autolock lock(mCapsulesLock);

    ClearCapsulePreferredActivitiesLP(cap->mCapsuleName);

    mCapsules.Erase(capCName);
    if (!cap->mPath.IsNull()) {
        mAppDirs.Erase(cap->mPath);
    }

    List<CapsuleParser::ContentProvider*>::Iterator providerIt;
    StringBuffer* r = NULL;
    for (providerIt = cap->mContentProviders.Begin();
         providerIt != cap->mContentProviders.End(); ++providerIt) {
        CapsuleParser::ContentProvider* p = *providerIt;
        String pCName, pName, pAuth;
        p->mInfo->GetCapsuleName(&pCName);
        p->mInfo->GetName(&pName);
        p->mInfo->GetAuthority(&pAuth);
        AutoPtr<IComponentName> cn;
        CComponentName::New(pCName, pName, (IComponentName**)&cn);
        mProvidersByComponent.Erase(cn);
        if (pAuth.IsNull()) {

            /* The is another ContentProvider with this authority when
             * this app was installed so this authority is NULL,
             * Ignore it as we don't have to unregister the provider->
             */
            continue;
        }

        StringTokenizer* names = new StringTokenizer(pAuth, ";");
        while (names->HasMoreTokens()) {
            String name = names->NextToken();
            HashMap<String, CapsuleParser::ContentProvider*>::Iterator it = mContentProviders.Find(name);
            if (it != mContentProviders.End() && it->mSecond == p) {
                mContentProviders.Erase(name);
                if (chatty && Config::LOGD) {
                    Boolean isSyncable;
                    p->mInfo->GetSyncable(&isSyncable);
                    Logger::D(TAG,
                        StringBuffer("Unregistered content provider: ")
                        + name + ", className = " + pName +
                        ", isSyncable = " + isSyncable);
                }
            }
        }
        if (chatty) {
            if (r == NULL) {
                r = new StringBuffer();
            }
            else {
                *r += " ";
            }
            *r += pName;
        }
    }
    if (r != NULL) {
        if (Config::LOGD) Logger::D(TAG, StringBuffer("  Providers: ") + *r);
        delete r;
    }

    r = NULL;
    List<CapsuleParser::Service*>::Iterator serviceIt;
    for (serviceIt = cap->mServices.Begin();
         serviceIt != cap->mServices.End(); ++serviceIt) {
        CapsuleParser::Service* s = *serviceIt;
        mServices->RemoveService(s);
        if (chatty) {
            if (r == NULL) {
                r = new StringBuffer();
            }
            else {
                *r += " ";
            }
            String sName;
            s->mInfo->GetName(&sName);
            *r += sName;
        }
    }
    if (r != NULL) {
        if (Config::LOGD) Logger::D(TAG, StringBuffer("  Services: ") + *r);
        delete r;
    }

    r = NULL;
    List<CapsuleParser::Activity*>::Iterator receiverIt;
    for (receiverIt = cap->mReceivers.Begin();
         receiverIt != cap->mReceivers.End(); ++receiverIt) {
        CapsuleParser::Activity* a = *receiverIt;
        mReceivers->RemoveActivity(a, "receiver");
        if (chatty) {
            if (r == NULL) {
                r = new StringBuffer();
            }
            else {
                *r += " ";
            }
            String aName;
            a->mInfo->GetName(&aName);
            *r += aName;
        }
    }
    if (r != NULL) {
        if (Config::LOGD) Logger::D(TAG, StringBuffer("  Receivers: ") + *r);
        delete r;
    }

    r = NULL;
    List<CapsuleParser::Activity*>::Iterator activityIt;
    for (activityIt = cap->mActivities.Begin();
         activityIt != cap->mActivities.End(); ++activityIt) {
        CapsuleParser::Activity* a = *activityIt;
        mActivities->RemoveActivity(a, "activity");
        if (chatty) {
            if (r == NULL) {
                r = new StringBuffer();
            }
            else {
                *r += " ";
            }
            String aName;
            a->mInfo->GetName(&aName);
            *r += aName;
        }
    }
    if (r != NULL) {
        if (Config::LOGD) Logger::D(TAG, StringBuffer("  Activities: ") + *r);
        delete r;
    }

    r = NULL;
    List<CapsuleParser::Permission*>::Iterator permissionIt;
    for (permissionIt = cap->mPermissions.Begin();
         permissionIt != cap->mPermissions.End(); ++permissionIt) {
        CapsuleParser::Permission* p = *permissionIt;
        String pName;
        p->mInfo->GetName(&pName);
        Boolean tree = FALSE;
        BasePermission* bp = NULL;
        HashMap<String, BasePermission*>::Iterator it =
                mSettings->mPermissions.Find(pName);
        if (it != mSettings->mPermissions.End()) {
            bp = it->mSecond;
        }
        if (bp == NULL) {
            tree = TRUE;
            it = mSettings->mPermissionTrees.Find(pName);
            if (it != mSettings->mPermissionTrees.End()) {
                bp = it->mSecond;
            }
        }
        if (bp != NULL && bp->mPerm == p) {
            bp->mPerm = NULL;
            if (chatty) {
                if (r == NULL) {
                    r = new StringBuffer();
                }
                else {
                    *r += " ";
                }
                *r += pName;
            }
        }
    }
    if (r != NULL) {
        if (Config::LOGD) Logger::D(TAG, StringBuffer("  Permissions: ") + *r);
        delete r;
    }

    r = NULL;
    List<CapsuleParser::Instrumentation*>::Iterator instrumentationIt;
    for (instrumentationIt = cap->mInstrumentation.Begin();
         instrumentationIt != cap->mInstrumentation.End(); ++instrumentationIt) {
        CapsuleParser::Instrumentation* a = *instrumentationIt;
        AutoPtr<IComponentName> name;
        a->GetComponentName((IComponentName**)&name);
        mInstrumentation.Erase(name);
        if (chatty) {
            if (r == NULL) {
                r = new StringBuffer();
            }
            else {
                *r += " ";
            }
            String aName;
            a->mInfo->GetName(&aName);
            *r += aName;
        }
    }
    if (r != NULL) {
        if (Config::LOGD) Logger::D(TAG, StringBuffer("  Instrumentation: ") + *r);
        delete r;
    }
}

Boolean CCapsuleManagerService::IsCapsuleFilename(
    /* [in] */ const String& name)
{
    return !name.IsNull() && name.EndWith(".cap");
}

Boolean CCapsuleManagerService::HasPermission(
    /* [in] */ CapsuleParser::Capsule* capInfo,
    /* [in] */ const String& perm)
{
    assert(capInfo);

    List<CapsuleParser::Permission*>::ReverseIterator rit;
    for (rit = capInfo->mPermissions.RBegin();
         rit != capInfo->mPermissions.REnd(); ++rit) {
        CapsuleParser::Permission* p = *rit;
        String name;
        if (p && (p->mInfo->GetName(&name), name.Equals(perm))) {
            return TRUE;
        }
    }

    return FALSE;
}

void CCapsuleManagerService::UpdatePermissionsLP(
    /* [in] */ const String& changingCap,
    /* [in] */ CapsuleParser::Capsule* capInfo,
    /* [in] */ Boolean grantPermissions,
    /* [in] */ Boolean replace,
    /* [in] */ Boolean replaceAll)
{
    // Make sure there are no dangling permission trees.
    HashMap<String, BasePermission*>::Iterator pit = mSettings->mPermissionTrees.Begin();
    while (pit != mSettings->mPermissionTrees.End()) {
        BasePermission* bp = pit->mSecond;
        if (bp->mCapsuleSetting == NULL) {
            // We may not yet have parsed the capsule, so just see if
            // we still know about its settings->
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it
                    = mSettings->mCapsules.Find(bp->mSourceCapsule);
            if (it != mSettings->mCapsules.End()) {
                bp->mCapsuleSetting = it->mSecond;
            }
        }
        if (bp->mCapsuleSetting == NULL) {
            Slogger::W(TAG, StringBuffer("Removing dangling permission tree: ")
                + bp->mName + " from capsule " + bp->mSourceCapsule);
            mSettings->mPermissionTrees.Erase(pit++);
        }
        else if (!changingCap.IsNull() && changingCap.Equals(bp->mSourceCapsule)) {
            if (capInfo == NULL || !HasPermission(capInfo, bp->mName)) {
                Slogger::I(TAG, StringBuffer("Removing old permission tree: ")
                    + bp->mName + " from capsule " + bp->mSourceCapsule);
                grantPermissions = TRUE;
                mSettings->mPermissionTrees.Erase(pit++);
            }
            else {
                ++pit;
            }
        }
        else {
            ++pit;
        }
    }

    // Make sure all dynamic permissions have been assigned to a package,
    // and make sure there are no dangling permissions.
    pit = mSettings->mPermissions.Begin();
    while (pit != mSettings->mPermissions.End()) {
        BasePermission* bp = pit->mSecond;
        if (bp->mType == BasePermission::TYPE_DYNAMIC) {
            if (DEBUG_SETTINGS) {
                Logger::V(TAG, StringBuffer("Dynamic permission: name=")
                    + bp->mName + " cap=" + bp->mSourceCapsule
                    + " info=" + bp->mPendingInfo);
            }
            if (bp->mCapsuleSetting == NULL && bp->mPendingInfo != NULL) {
                BasePermission* tree = FindPermissionTreeLP(bp->mName);
                if (tree != NULL) {
                    bp->mCapsuleSetting = tree->mCapsuleSetting;
                    AutoPtr<IPermissionInfo> info;
                    CPermissionInfo::New(bp->mPendingInfo.Get(), (IPermissionInfo**)&info);
                    bp->mPerm = new CapsuleParser::Permission(tree->mPerm->mOwner, info);
                    String cname;
                    tree->mPerm->mInfo->GetCapsuleName(&cname);
                    bp->mPerm->mInfo->SetCapsuleName(cname);
                    bp->mPerm->mInfo->SetName(bp->mName);
                    bp->mUid = tree->mUid;
                }
            }
        }
        if (bp->mCapsuleSetting == NULL) {
            // We may not yet have parsed the capsule, so just see if
            // we still know about its settings->
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it
                    = mSettings->mCapsules.Find(bp->mSourceCapsule);
            if (it != mSettings->mCapsules.End()) {
                bp->mCapsuleSetting = it->mSecond;
            }
        }
        if (bp->mCapsuleSetting == NULL) {
            Slogger::W(TAG, StringBuffer("Removing dangling permission: ")
                + bp->mName + " from capsule " + bp->mSourceCapsule);
            mSettings->mPermissions.Erase(pit++);
        }
        else if (!changingCap.IsNull() && changingCap.Equals(bp->mSourceCapsule)) {
            if (capInfo == NULL || !HasPermission(capInfo, bp->mName)) {
                Slogger::I(TAG, StringBuffer("Removing old permission: ")
                    + bp->mName + " from capsule " + bp->mSourceCapsule);
                grantPermissions = TRUE;
                mSettings->mPermissions.Erase(pit++);
            }
            else {
                pit++;
            }
        }
        else {
            pit++;
        }
    }

    // Now update the permissions for all packages, in particular
    // replace the granted permissions of the system packages.
    if (grantPermissions) {
        HashMap<String, CapsuleParser::Capsule*>::Iterator it;
        for (it = mCapsules.Begin(); it != mCapsules.End(); ++it) {
            CapsuleParser::Capsule* cap = it->mSecond;
            if (cap != capInfo) {
                GrantPermissionsLP(cap, replaceAll);
            }
        }
    }

    if (capInfo != NULL) {
        GrantPermissionsLP(capInfo, replace);
    }
}

void CCapsuleManagerService::GrantPermissionsLP(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Boolean replace)
{
    assert(cap);

    CapsuleSetting* ps = (CapsuleSetting*)cap->mExtras->Probe(EIID_CapsuleSetting);
    if (ps == NULL) {
        return;
    }
    GrantedPermissions* gp = ps->mSharedUser != NULL
        ? (GrantedPermissions*)ps->mSharedUser : (GrantedPermissions*)ps;
    Boolean changedPermission = FALSE;

    if (replace) {
        ps->mPermissionsFixed = FALSE;
        if (gp == (GrantedPermissions*)ps) {
            gp->mGrantedPermissions.Clear();
            //todo: check if gp->mGids is null
            gp->mGids = mGlobalGids->Clone();
        }
    }

    if (gp->mGids == NULL) {
        gp->mGids = mGlobalGids->Clone();
    }

    List<String>::Iterator rpit;
    for (rpit = cap->mRequestedPermissions.Begin();
         rpit != cap->mRequestedPermissions.End(); ++rpit) {
        String name = *rpit;
        BasePermission* bp = NULL;
        HashMap<String, BasePermission*>::Iterator it
                = mSettings->mPermissions.Find(name);
        if (it != mSettings->mPermissions.End()) {
            bp = it->mSecond;
        }
//        if (FALSE) {
//            if (gp != (GrantedPermissions*)ps) {
//                Logger::I(TAG, StringBuffer("Capsule ")
//                    + cap->mCapsuleName + " checking " + name + ": " + bp);
//            }
//        }
        if (bp != NULL && bp->mCapsuleSetting != NULL) {
            String perm = bp->mName;
            Boolean allowed = FALSE;
            Boolean allowedSig = FALSE;
            if (bp->mProtectionLevel == PermissionInfo_PROTECTION_NORMAL
                || bp->mProtectionLevel == PermissionInfo_PROTECTION_DANGEROUS) {
                allowed = TRUE;
            }
            else if (bp->mCapsuleSetting == NULL) {
                // This permission is invalid; skip it.
                allowed = FALSE;
            }
            else if (bp->mProtectionLevel == PermissionInfo_PROTECTION_SIGNATURE
                || bp->mProtectionLevel == PermissionInfo_PROTECTION_SIGNATURE_OR_SYSTEM) {
                allowed = (CheckSignaturesLP(
                    bp->mCapsuleSetting->mSignatures->mSignatures.Get(), cap->mSignatures)
                            == CapsuleManager_SIGNATURE_MATCH)
                        || (CheckSignaturesLP(mPlatformCapsule->mSignatures, cap->mSignatures)
                            == CapsuleManager_SIGNATURE_MATCH);
                if (!allowed && bp->mProtectionLevel
                    == PermissionInfo_PROTECTION_SIGNATURE_OR_SYSTEM) {
                    if (IsSystemApp(cap)) {
                        // For updated system applications, the signatureOrSystem permission
                        // is granted only if it had been defined by the original application->
                        if (IsUpdatedSystemApp(cap)) {
                            CapsuleSetting* sysPs = mSettings->GetDisabledSystemCap(cap->mCapsuleName);
                            GrantedPermissions* origGp = sysPs->mSharedUser != NULL
                                ? (GrantedPermissions*)sysPs->mSharedUser
                                : (GrantedPermissions*)sysPs;
                            if (origGp->mGrantedPermissions.Find(perm)
                                != origGp->mGrantedPermissions.End()) {
                                allowed = TRUE;
                            }
                            else {
                                allowed = FALSE;
                            }
                        }
                        else {
                            allowed = TRUE;
                        }
                    }
                }
                if (allowed) {
                    allowedSig = TRUE;
                }
            }
            else {
                allowed = FALSE;
            }
//            if (FALSE) {
//                if (gp != (GrantedPermissions*)ps) {
//                    Logger::I(TAG, StringBuffer("Capsule ")
//                        + cap->mCapsuleName + " granting " + perm);
//                }
//            }
            if (allowed) {
                if ((ps->mCapFlags & ApplicationInfo_FLAG_SYSTEM) == 0
                    && ps->mPermissionsFixed) {
                    // If this is an existing, non-system capsule, then
                    // we can't add any new permissions to it.
                    if (!allowedSig && gp->mGrantedPermissions.Find(perm)
                            == gp->mGrantedPermissions.End()) {
                        allowed = FALSE;
                        // Except...  if this is a permission that was added
                        // to the platform (note: need to only do this when
                        // updating the platform).
                        const Int32 NP = CapsuleParser::GetNewPermissionsLength();
                        for (Int32 ip = 0; ip < NP; ip++) {
                            const CapsuleParser::NewPermissionInfo* npi
                                        = CapsuleParser::NEW_PERMISSIONS[ip];
                            Int32 capSdkVersion;
                            cap->mApplicationInfo->GetTargetSdkVersion(&capSdkVersion);
                            if (npi->mName.Equals(perm) && capSdkVersion < npi->mSdkVersion) {
                                allowed = TRUE;
                                Logger::I(TAG, StringBuffer("Auto-granting ")
                                    + perm + " to old cap " + cap->mCapsuleName);
                                break;
                            }
                        }
                    }
                }
                if (allowed) {
                    if (gp->mGrantedPermissions.Find(perm) == gp->mGrantedPermissions.End()) {
                        changedPermission = TRUE;
                        gp->mGrantedPermissions.Insert(perm);
                        gp->mGids = AppendInts(gp->mGids, bp->mGids);
                    }
                    else if (!ps->mHaveGids) {
                        gp->mGids = AppendInts(gp->mGids.Get(), bp->mGids.Get());
                    }
                }
                else {
                    Slogger::W(TAG, StringBuffer("Not granting permission ")
                        + perm + " to capsule " + cap->mCapsuleName
                        + " because it was previously installed without");
                }
            }
            else {
                HashSet<String>::Iterator it = gp->mGrantedPermissions.Find(perm);
                if (it != gp->mGrantedPermissions.End()) {
                    gp->mGrantedPermissions.Erase(it);
                    changedPermission = TRUE;
//                    gp->mGids = RemoveInts(gp->mGids, bp->mGids);
                    Int32 capFlags;
                    cap->mApplicationInfo->GetFlags(&capFlags);
                    Slogger::I(TAG, StringBuffer("Un-granting permission ") + perm
                        + " from capsule " + cap->mCapsuleName
                        + " (protectionLevel=" + bp->mProtectionLevel
                        + " flags=0x" + /*Integer::ToHexString(cap->mApplicationInfo->flags)*/
                        + capFlags + ")");
                }
                else {
                    Int32 capFlags;
                    cap->mApplicationInfo->GetFlags(&capFlags);
                    Slogger::W(TAG, StringBuffer("Not granting permission ") + perm
                        + " to capsule " + cap->mCapsuleName
                        + " (protectionLevel=" + bp->mProtectionLevel
                        + " flags=0x" + /*Integer::ToHexString(cap->mApplicationInfo->flags)*/
                        + capFlags + ")");
                }
            }
        }
        else {
            Slogger::W(TAG, StringBuffer("Unknown permission ")
                + name + " in capsule " + cap->mCapsuleName);
        }
    }

    if ((changedPermission || replace) && !ps->mPermissionsFixed &&
        (((ps->mCapFlags & ApplicationInfo_FLAG_SYSTEM) == 0) ||
        ((ps->mCapFlags & ApplicationInfo_FLAG_UPDATED_SYSTEM_APP) != 0))) {
        // This is the first that we have heard about this package, so the
        // permissions we have now selected are fixed until explicitly
        // changed.
        ps->mPermissionsFixed = TRUE;
    }
    ps->mHaveGids = TRUE;
}

void CCapsuleManagerService::SendCapsuleBroadcast(
    /* [in] */ const String& action,
    /* [in] */ const String& cap,
    /* [in] */ IBundle* extras,
    /* [in] */ IIntentReceiver* finishedReceiver)
{
    AutoPtr<IActivityManager> am;
    ActivityManagerNative::GetDefault((IActivityManager**)&am);
    if (am != NULL) {
        AutoPtr<IIntent> intent;
        AutoPtr<IUri> uri;
        if (!cap.IsNull()) {
            Uri::FromParts(String("capsule"), cap, String(NULL), (IUri**)&uri);
        }
        CIntent::New(action, uri, (IIntent**)&intent);
        if (extras != NULL) {
            intent->PutExtras(extras);
        }
        intent->AddFlags(Intent_FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
        Int32 res = 0;
        am->BroadcastIntent(NULL, intent.Get(), String(NULL), finishedReceiver,
            0, String(NULL), NULL, String(NULL),
            finishedReceiver != NULL, FALSE, -1, -1, &res);
    }
}

ECode CCapsuleManagerService::NextCapsuleToClean(
    /* [in] */ const String& lastCapsule,
    /* [out] */ String* nextCapsule)
{
    VALIDATE_NOT_NULL(nextCapsule);

    Mutex::Autolock lock(mCapsulesLock);

    if (!mMediaMounted) {
        // If the external storage is no longer mounted at this point,
        // the caller may not have been able to delete all of this
        // packages files and can not delete any more.  Bail.
        *nextCapsule = NULL;
        return NOERROR;
    }
    if (!lastCapsule.IsNull()) {
        List<String>::Iterator it;
        for (it = mSettings->mCapsulesToBeCleaned.Begin();
            it != mSettings->mCapsulesToBeCleaned.End(); ++it) {
            if ((*it).Equals(lastCapsule)) {
                mSettings->mCapsulesToBeCleaned.Erase(it);
                break;
            }
        }
    }
    *nextCapsule = mSettings->mCapsulesToBeCleaned.Begin()
            != mSettings->mCapsulesToBeCleaned.End()
            ? *mSettings->mCapsulesToBeCleaned.Begin() : String(NULL);
    return NOERROR;
}

void CCapsuleManagerService::ScheduleCapsuleCleaning(
    /* [in] */ const String& capsuleName)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
        /* [in] */ const String& capsuleName);
    pHandlerFunc = &CCapsuleManagerService::HandleStartCleaningCapsule;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(capsuleName);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

void CCapsuleManagerService::StartCleaningCapsules()
{
    {
        Mutex::Autolock lock(mCapsulesLock);

        if (!mMediaMounted) {
            return;
        }
        if (mSettings->mCapsulesToBeCleaned.Begin()
            == mSettings->mCapsulesToBeCleaned.End()) {
            return;
        }
    }
    AutoPtr<IIntent> intent;
    CIntent::New(String(CapsuleManager::ACTION_CLEAN_EXTERNAL_STORAGE), (IIntent**)&intent);
    intent->SetComponent(DEFAULT_CONTAINER_COMPONENT);
    AutoPtr<IActivityManager> am;
    ActivityManagerNative::GetDefault((IActivityManager**)&am);
    if (am != NULL) {
        AutoPtr<IComponentName> name;
        am->StartService(NULL, intent, String(NULL), (IComponentName**)&name);
    }
}

ECode CCapsuleManagerService::InstallCapsuleEx(
    /* [in] */ IUri* capsuleURI,
    /* [in] */ ICapsuleInstallObserver* observer,
    /* [in] */ Int32 flags)
{
    return InstallCapsule(capsuleURI, observer, flags, String(NULL));
}

//todo: should be removed
ECode CCapsuleManagerService::InstallCapsuleEx2(
    /* [in] */ const String& path)
{
    ECode ec;
    String err;
    CapsuleParser p;

    String fname = path + "/capsule.xml";

    CapsuleParser::Capsule* capsule = new CapsuleParser::Capsule();
    if (SUCCEEDED(p.ParseCapsule(fname, capsule, &err))) {
        mCapsules[capsule->mCapsuleName] = capsule;
        ec = NOERROR;
    }
    else {
        if (DEBUG_INSTALL) {
            Slogger::E(TAG, err);
        }
        ec = E_FAIL;
    }

    return ec;
}

ECode CCapsuleManagerService::InstallCapsule(
    /* [in] */ IUri* capsuleURI,
    /* [in] */ ICapsuleInstallObserver* observer,
    /* [in] */ Int32 flags,
    /* [in] */ const String& installerCapsuleName)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*android.Manifest.permission.INSTALL_PACKAGES*/, String(NULL));

    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
        /* [in] */ HandlerParams* params);
    pHandlerFunc = &CCapsuleManagerService::HandleInitCopy;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)new InstallParams(capsuleURI, observer,
            flags, installerCapsuleName, this));

    return SendMessage(*(Handle32*)&pHandlerFunc, params);
}

ECode CCapsuleManagerService::FinishCapsuleInstall(
    /* [in] */ Int32 token)
{
    if (DEBUG_INSTALL) Logger::V(TAG, StringBuffer("BM finishing capsule install for ") + token);

    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
        /* [in] */ Int32 token);
    pHandlerFunc = &CCapsuleManagerService::HandlePostInstall;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32(token);

    return SendMessage(*(Handle32*)&pHandlerFunc, params);
}

void CCapsuleManagerService::ProcessPendingInstall(
    /* [in] */ InstallArgs* args,
    /* [in] */ Int32 currentStatus)
{
    // Queue up an async operation since the package installation may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
        /* [in] */ InstallArgs* args,
        /* [in] */ Int32 currentStatus);
    pHandlerFunc = &CCapsuleManagerService::HandleProcessPendingInstall;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)args);
    params->WriteInt32(currentStatus);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
}

void CCapsuleManagerService::HandleProcessPendingInstall(
    /* [in] */ InstallArgs* args,
    /* [in] */ Int32 currentStatus)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
        /* [in] */ InstallArgs* args,
        /* [in] */ Int32 currentStatus);
    pHandlerFunc = &CCapsuleManagerService::HandleProcessPendingInstall;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

     // Result object to be returned
    CapsuleInstalledInfo* res = new CapsuleInstalledInfo();
    res->mReturnCode = currentStatus;
    res->mUid = -1;
    res->mCap = NULL;
    res->mRemovedInfo = new CapsuleRemovedInfo(this);
    if (res->mReturnCode == CapsuleManager::INSTALL_SUCCEEDED) {
        args->DoPreInstall(res->mReturnCode);
        {
            Mutex::Autolock lock(mInstallLock);
            InstallCapsuleLI(args, TRUE, res);
        }
        args->DoPostInstall(res->mReturnCode);
    }

    // A restore should be performed at this point if (a) the install
    // succeeded, (b) the operation is not an update, and (c) the new
    // package has a backupAgent defined.
    const Boolean update = !res->mRemovedInfo->mRemovedCapsule.IsNull();
    String bakAgtName;
    Boolean doRestore = (!update
            && res->mCap != NULL
            && (res->mCap->mApplicationInfo->GetBackupAgentName(&bakAgtName), !bakAgtName.IsNull()));

    // Set up the post-install work request bookkeeping.  This will be used
    // and cleaned up by the post-install event handling regardless of whether
    // there's a restore pass performed.  Token values are >= 1.
    Int32 token;
    if (mNextInstallToken < 0) mNextInstallToken = 1;
    token = mNextInstallToken++;

    PostInstallData* data = new PostInstallData(args, res);
    mRunningInstalls[token] = data;
    if (DEBUG_INSTALL) Logger::V(TAG, StringBuffer("+ starting restore round-trip ") + token);

    if (res->mReturnCode == CapsuleManager::INSTALL_SUCCEEDED && doRestore) {
        // Pass responsibility to the Backup Manager.  It will perform a
        // restore if appropriate, then pass responsibility back to the
        // Package Manager to run the post-install observer callbacks
        // and broadcasts.

        AutoPtr<IBackupManager> bm = IBackupManager::Probe(
                ServiceManager::GetService(String(Context_BACKUP_SERVICE)));
        if (bm != NULL) {
            if (DEBUG_INSTALL) Logger::V(TAG, StringBuffer("token ") + token
                    + " to BM for possible restore");

            String resCName;
            res->mCap->mApplicationInfo->GetCapsuleName(&resCName);
            if (FAILED(bm->RestoreAtInstall(resCName, token))) {
                Slogger::E(TAG, "Exception trying to enqueue restore");
                doRestore = FALSE;
            }
        }
        else {
            Slogger::E(TAG, "Backup Manager not found!");
            doRestore = FALSE;
        }
    }

    if (!doRestore) {
        // No restore possible, or the Backup Manager was mysteriously not
        // available -- just fire the post-install work request directly.
        if (DEBUG_INSTALL) Logger::V(TAG, StringBuffer("No restore - queue post-install for ") + token);

        void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
        /* [in] */ Int32 token);
        pHandlerFunc = &CCapsuleManagerService::HandlePostInstall;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(token);

        SendMessage(*(Handle32*)&pHandlerFunc, params);
    }
}

CCapsuleManagerService::InstallArgs*
CCapsuleManagerService::CreateInstallArgs(
    /* [in] */ InstallParams* params)
{
    assert(params);

    if (InstallOnSd(params->mFlags)) {
        return new SdInstallArgs(params, this);
    }
    else {
        return new FileInstallArgs(params, this);
    }
}

CCapsuleManagerService::InstallArgs* CCapsuleManagerService::CreateInstallArgs(
    /* [in] */ Int32 flags,
    /* [in] */ const String& fullCodePath,
    /* [in] */ const String& fullResourcePath,
    /* [in] */ const String& nativeLibraryPath)
{
    if (InstallOnSd(flags)) {
        return new SdInstallArgs(fullCodePath, fullResourcePath, nativeLibraryPath, this);
    }
    else {
        return new FileInstallArgs(fullCodePath, fullResourcePath, nativeLibraryPath, this);
    }
}

// Used by package mover
CCapsuleManagerService::InstallArgs* CCapsuleManagerService::CreateInstallArgs(
    /* [in] */ IUri* capsuleURI,
    /* [in] */ Int32 flags,
    /* [in] */ const String& capName,
    /* [in] */ String dataDir)
{
    if (InstallOnSd(flags)) {
        String cid = GetNextCodePath(String(NULL), capName, String("/") + SdInstallArgs::RES_FILE_NAME);
        return new SdInstallArgs(capsuleURI, cid, this);
    }
    else {
        return new FileInstallArgs(capsuleURI, capName, dataDir, this);
    }
}

// Utility method used to create code paths based on package name and available index.
String CCapsuleManagerService::GetNextCodePath(
    /* [in] */ const String& oldCodePath,
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix)
{
    String idxStr = String("");
    Int32 idx = 1;
    // Fall back to default value of idx=1 if prefix is not
    // part of oldCodePath
    if (!oldCodePath.IsNull()) {
        String subStr = oldCodePath;
        // Drop the suffix right away
        if (subStr.EndWith(suffix)) {
            subStr = subStr.Substring(0, subStr.GetLength() - suffix.GetLength());
        }
        // If oldCodePath already contains prefix find out the
        // ending index to either increment or decrement.
        Int32 sidx = subStr.LastIndexOf(prefix);
        if (sidx != -1) {
            subStr = subStr.Substring(sidx + prefix.GetLength());
            if (!subStr.IsNull()) {
                if (subStr.StartWith(INSTALL_CAPSULE_SUFFIX)) {
                    subStr = subStr.Substring(INSTALL_CAPSULE_SUFFIX.GetLength());
                }
//                try {
                idx = subStr.ToInt32();
                if (idx <= 1) {
                    idx++;
                }
                else {
                    idx--;
                }
//                } catch(NumberFormatException e) {
//                }
            }
        }
    }
    idxStr = String(INSTALL_CAPSULE_SUFFIX) + String::FromInt32(idx);
    return prefix + idxStr;
}

// Utility method used to ignore ADD/REMOVE events
// by directory observer.
Boolean CCapsuleManagerService::IgnoreCodePath(
    /* [in] */ const String& fullPathStr)
{
    String capName = GetCapName(fullPathStr);
    Int32 idx = capName.LastIndexOf(INSTALL_CAPSULE_SUFFIX);
    if (idx != -1 && ((idx + 1) < (Int32)capName.GetLength())) {
        // Make sure the package ends with a numeral
        String version = capName.Substring(idx + 1);
        return version.ToInt32() == 0 ? FALSE : TRUE;
    }
    return FALSE;
}

// Utility method that returns the relative package path with respect
// to the installation directory. Like say for /data/data/com.test-1.apk
// string com.test-1 is returned.
String CCapsuleManagerService::GetCapName(
    /* [in] */ const String& codePath)
{
    if (codePath.IsNull()) {
        return String(NULL);
    }
    Int32 sidx = codePath.LastIndexOf("/");
    Int32 eidx = codePath.LastIndexOf(".");
    if (eidx == -1) {
        eidx = codePath.GetLength();
    }
    else if (eidx == 0) {
        Slogger::W(TAG, StringBuffer(" Invalid code path, ")
            + codePath + " Not a valid apk name");
        return String(NULL);
    }
    return codePath.Substring(sidx + 1, eidx - sidx - 1);
}

/*
 * Install a non-existing package.
 */
void CCapsuleManagerService::InstallNewCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ const String& installerCapsuleName,
    /* [in] */ CapsuleInstalledInfo* res)
{
    // Remember this for later, in case we need to rollback this install
    String capName = cap->mCapsuleName;

    Boolean dataDirExists;
    GetDataPathForCapsule(cap)->Exists(&dataDirExists);
    res->mName = capName;
    {
        Mutex::Autolock lock(mCapsulesLock);

        if (mSettings->mRenamedCapsules.Find(capName)
            != mSettings->mRenamedCapsules.End()) {
            // A package with the same name is already installed, though
            // it has been renamed to an older name.  The package we
            // are trying to install should be installed as an update to
            // the existing one, but that has not been requested, so bail.
            Slogger::W(TAG, StringBuffer("Attempt to re-install ") + capName
                    + " without first uninstalling package running as "
                    + mSettings->mRenamedCapsules.Find(capName)->mSecond);
            res->mReturnCode = CapsuleManager::INSTALL_FAILED_ALREADY_EXISTS;
            return;
        }
        if (mCapsules.Find(capName) != mCapsules.End()
            || mAppDirs.Find(cap->mPath) != mAppDirs.End()) {
            // Don't allow installation over an existing package with the same name.
            Slogger::W(TAG, StringBuffer("Attempt to re-install ") + capName
                    + " without first uninstalling.");
            res->mReturnCode = CapsuleManager::INSTALL_FAILED_ALREADY_EXISTS;
            return;
        }
    }
    mLastScanError = CapsuleManager::INSTALL_SUCCEEDED;
    CapsuleParser::Capsule* newCapsule = ScanCapsuleLI(cap, parseFlags, scanMode,
            System::GetCurrentTimeMillis());
    if (newCapsule == NULL) {
        Slogger::W(TAG, StringBuffer("Package couldn't be installed in ") + cap->mPath);
        if ((res->mReturnCode = mLastScanError) == CapsuleManager::INSTALL_SUCCEEDED) {
            res->mReturnCode = CapsuleManager::INSTALL_FAILED_INVALID_APK;
        }
    }
    else {
        UpdateSettingsLI(newCapsule, installerCapsuleName, res);
        // delete the partially installed application. the data directory will have to be
        // restored if it was already existing
        if (res->mReturnCode != CapsuleManager::INSTALL_SUCCEEDED) {
            // remove package from internal structures.  Note that we want deletePackageX to
            // delete the package data and cache directories that it created in
            // scanPackageLocked, unless those directories existed before we even tried to
            // install.
            DeleteCapsuleLI(
                    capName, FALSE,
                    dataDirExists ? CapsuleManager_DONT_DELETE_DATA : 0,
                            res->mRemovedInfo, TRUE);
        }
    }
}

void CCapsuleManagerService::ReplaceCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ const String& installerCapsuleName,
    /* [in] */ CapsuleInstalledInfo* res)
{
    CapsuleParser::Capsule* oldCapsule = NULL;
    String capName = cap->mCapsuleName;
    // First find the old package info and check signatures
    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                mCapsules.Find(capName);
        if (it != mCapsules.End()) {
            oldCapsule = it->mSecond;
        }
        if (CheckSignaturesLP(oldCapsule->mSignatures, cap->mSignatures)
                != CapsuleManager_SIGNATURE_MATCH) {
            res->mReturnCode = CapsuleManager::INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES;
            return;
        }
    }
    Boolean sysCap = IsSystemApp(oldCapsule);
    if (sysCap) {
        ReplaceSystemCapsuleLI(oldCapsule, cap, parseFlags, scanMode, installerCapsuleName, res);
    }
    else {
        ReplaceNonSystemCapsuleLI(oldCapsule, cap, parseFlags, scanMode, installerCapsuleName, res);
    }
}

void CCapsuleManagerService::ReplaceNonSystemCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* deletedCapsule,
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ const String& installerCapsuleName,
    /* [in] */ CapsuleInstalledInfo* res)
{
    CapsuleParser::Capsule* newCapsule = NULL;
    String capName = deletedCapsule->mCapsuleName;
    Boolean deletedCap = TRUE;
    Boolean updatedSettings = FALSE;

    String oldInstallerCapsuleName;
    {
        Mutex::Autolock lock(mCapsulesLock);

        oldInstallerCapsuleName = mSettings->GetInstallerCapsuleName(capName);
    }

    Int64 origUpdateTime;
    if (cap->mExtras != NULL) {
        origUpdateTime = ((CapsuleSetting*)cap->mExtras->Probe(EIID_CapsuleSetting))->mLastUpdateTime;
    }
    else {
        origUpdateTime = 0;
    }

    // First delete the existing package while retaining the data directory
    if (!DeleteCapsuleLI(capName, TRUE, CapsuleManager_DONT_DELETE_DATA,
            res->mRemovedInfo, TRUE)) {
        // If the existing package wasn't successfully deleted
        res->mReturnCode = CapsuleManager::INSTALL_FAILED_REPLACE_COULDNT_DELETE;
        deletedCap = FALSE;
    }
    else {
        // Successfully deleted the old package. Now proceed with re-installation
        mLastScanError = CapsuleManager::INSTALL_SUCCEEDED;
        newCapsule = ScanCapsuleLI(cap, parseFlags, scanMode | SCAN_UPDATE_TIME,
                System::GetCurrentTimeMillis());
        if (newCapsule == NULL) {
            Slogger::W(TAG, "Package couldn't be installed in " + cap->mPath);
            if ((res->mReturnCode = mLastScanError) == CapsuleManager::INSTALL_SUCCEEDED) {
                res->mReturnCode = CapsuleManager::INSTALL_FAILED_INVALID_APK;
            }
        }
        else {
            UpdateSettingsLI(newCapsule, installerCapsuleName, res);
            updatedSettings = TRUE;
        }
    }

    if (res->mReturnCode != CapsuleManager::INSTALL_SUCCEEDED) {
        // remove package from internal structures.  Note that we want deletePackageX to
        // delete the package data and cache directories that it created in
        // scanPackageLocked, unless those directories existed before we even tried to
        // install.
        if(updatedSettings) {
            DeleteCapsuleLI(capName, TRUE,
                    CapsuleManager_DONT_DELETE_DATA,
                    res->mRemovedInfo, TRUE);
        }
        // Since we failed to install the new package we need to restore the old
        // package that we deleted.
        if(deletedCapsule) {
            AutoPtr<IFile> restoreFile;
            CFile::New(deletedCapsule->mPath, (IFile**)&restoreFile);
            if (restoreFile == NULL) {
                Slogger::E(TAG, StringBuffer("Failed allocating storage when restoring pkg : ") + capName);
                return;
            }
            // Parse old package
            Boolean oldOnSd = IsExternal(deletedCapsule);
            Int32 oldParseFlags  = mDefParseFlags | CapsuleParser::PARSE_CHATTY |
                    (IsForwardLocked(deletedCapsule) ? CapsuleParser::PARSE_FORWARD_LOCK : 0) |
                    (oldOnSd ? CapsuleParser::PARSE_ON_SDCARD : 0);
            Int32 oldScanMode = (oldOnSd ? 0 : SCAN_MONITOR) | SCAN_UPDATE_SIGNATURE
                    | SCAN_UPDATE_TIME;
            if (ScanCapsuleLI(restoreFile, oldParseFlags, oldScanMode,
                    origUpdateTime) == NULL) {
                Slogger::E(TAG, StringBuffer("Failed to restore package : ")
                        + capName + " after failed upgrade");
                return;
            }
            // Restore of old package succeeded. Update permissions.
            {
                Mutex::Autolock lock(mCapsulesLock);

                UpdatePermissionsLP(deletedCapsule->mCapsuleName, deletedCapsule,
                        TRUE, FALSE, FALSE);
                mSettings->WriteLP();
            }
            Slogger::I(TAG, StringBuffer("Successfully restored package : ")
                    + capName + " after failed upgrade");
        }
    }
}

void CCapsuleManagerService::ReplaceSystemCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* deletedCapsule,
    /* [in] */ CapsuleParser::Capsule* cap,
    /* [in] */ Int32 parseFlags,
    /* [in] */ Int32 scanMode,
    /* [in] */ const String& installerCapsuleName,
    /* [in] */ CapsuleInstalledInfo* res)
{
    CapsuleParser::Capsule* newCapsule = NULL;
    Boolean updatedSettings = FALSE;
    parseFlags |= CapsuleManager::INSTALL_REPLACE_EXISTING |
            CapsuleParser::PARSE_IS_SYSTEM;
    String capsuleName = deletedCapsule->mCapsuleName;
    res->mReturnCode = CapsuleManager::INSTALL_FAILED_REPLACE_COULDNT_DELETE;
    if (capsuleName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete null packageName.");
        return;
    }
    CapsuleParser::Capsule* oldCap = NULL;
    CapsuleSetting* oldCapSetting = NULL;
    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                mCapsules.Find(capsuleName);
        if (it != mCapsules.End()) {
            oldCap = it->mSecond;
        }
        HashMap<String, AutoPtr<CapsuleSetting> >::Iterator cit =
                mSettings->mCapsules.Find(capsuleName);
        if (cit != mSettings->mCapsules.End()) {
            oldCapSetting = cit->mSecond;
        }
        if((oldCap == NULL) || (oldCap->mApplicationInfo == NULL) ||
                (oldCapSetting == NULL)) {
            Slogger::W(TAG, StringBuffer("Couldn't find package:") + capsuleName + " information");
            return;
        }
    }

    Int32 oldCapUid;
    oldCap->mApplicationInfo->GetUid(&oldCapUid);
    KillApplication(capsuleName, oldCapUid);

    res->mRemovedInfo->mUid = oldCapUid;
    res->mRemovedInfo->mRemovedCapsule = capsuleName;
    // Remove existing system package
    RemoveCapsuleLI(oldCap, TRUE);
    {
        Mutex::Autolock lock(mCapsulesLock);

        if (!mSettings->DisableSystemCapsuleLP(capsuleName) && deletedCapsule != NULL) {
            // We didn't need to disable the .apk as a current system package,
            // which means we are replacing another update that is already
            // installed.  We need to make sure to delete the older one's .apk.
            String srcDir, pubSrcDir, natLibDir;
            deletedCapsule->mApplicationInfo->GetSourceDir(&srcDir);
            deletedCapsule->mApplicationInfo->GetPublicSourceDir(&pubSrcDir);
            deletedCapsule->mApplicationInfo->GetNativeLibraryDir(&natLibDir);
            res->mRemovedInfo->mArgs = CreateInstallArgs(IsExternal(cap)
                    ? CapsuleManager::INSTALL_EXTERNAL : CapsuleManager::INSTALL_INTERNAL,
                    srcDir, pubSrcDir, natLibDir);
        }
        else {
            res->mRemovedInfo->mArgs = NULL;
        }
    }

    // Successfully disabled the old package. Now proceed with re-installation
    mLastScanError = CapsuleManager::INSTALL_SUCCEEDED;
    Int32 capFlags;
    cap->mApplicationInfo->GetFlags(&capFlags);
    capFlags |= ApplicationInfo_FLAG_UPDATED_SYSTEM_APP;
    cap->mApplicationInfo->SetFlags(capFlags);
    newCapsule = ScanCapsuleLI(cap, parseFlags, scanMode, 0);
    if (newCapsule == NULL) {
        Slogger::W(TAG, StringBuffer("Package couldn't be installed in ") + cap->mPath);
        if ((res->mReturnCode = mLastScanError) == CapsuleManager::INSTALL_SUCCEEDED) {
            res->mReturnCode = CapsuleManager::INSTALL_FAILED_INVALID_APK;
        }
    }
    else {
        if (newCapsule->mExtras != NULL) {
            CapsuleSetting* newCapSetting =
                    (CapsuleSetting*)newCapsule->mExtras->Probe(EIID_CapsuleSetting);
            newCapSetting->mFirstInstallTime = oldCapSetting->mFirstInstallTime;
            newCapSetting->mLastUpdateTime = System::GetCurrentTimeMillis();
        }
        UpdateSettingsLI(newCapsule, installerCapsuleName, res);
        updatedSettings = TRUE;
    }

    if (res->mReturnCode != CapsuleManager::INSTALL_SUCCEEDED) {
        // Re installation failed. Restore old information
        // Remove new pkg information
        if (newCapsule != NULL) {
            RemoveCapsuleLI(newCapsule, TRUE);
        }
        // Add back the old system package
        ScanCapsuleLI(oldCap, parseFlags, SCAN_MONITOR | SCAN_UPDATE_SIGNATURE, 0);
        // Restore the old system information in Settings
        {
            Mutex::Autolock lock(mCapsulesLock);

            if (updatedSettings) {
                mSettings->EnableSystemCapsuleLP(capsuleName);
                mSettings->SetInstallerCapsuleName(capsuleName,
                        oldCapSetting->mInstallerCapsuleName);
            }
            mSettings->WriteLP();
        }
    }
}

// Utility method used to move dex files during install.
//private int moveDexFilesLI(PackageParser.Package newPackage) {
//    int retCode;
//    if ((newPackage.applicationInfo.flags&ApplicationInfo.FLAG_HAS_CODE) != 0) {
//        retCode = mInstaller.movedex(newPackage.mScanPath, newPackage.mPath);
//        if (retCode != 0) {
//            if (mNoDexOpt) {
//                /*
//                 * If we're in an engineering build, programs are lazily run
//                 * through dexopt. If the .dex file doesn't exist yet, it
//                 * will be created when the program is run next.
//                 */
//                Slog.i(TAG, "dex file doesn't exist, skipping move: " + newPackage.mPath);
//            } else {
//                Slog.e(TAG, "Couldn't rename dex file: " + newPackage.mPath);
//                return PackageManager.INSTALL_FAILED_INSUFFICIENT_STORAGE;
//            }
//        }
//    }
//    return PackageManager.INSTALL_SUCCEEDED;
//}

void CCapsuleManagerService::UpdateSettingsLI(
    /* [in] */ CapsuleParser::Capsule* newCapsule,
    /* [in] */ const String& installerCapsuleName,
    /* [in] */ CapsuleInstalledInfo* res)
{
    String capName = newCapsule->mCapsuleName;
    {
        Mutex::Autolock lock(mCapsulesLock);

        //write settings. the installStatus will be incomplete at this stage.
        //note that the new package setting would have already been
        //added to mPackages. It hasn't been persisted yet.
        mSettings->SetInstallStatus(capName, CAP_INSTALL_INCOMPLETE);
        mSettings->WriteLP();
    }

//    if ((res->mReturnCode = MoveDexFilesLI(newCapsule))
//            != CapsuleManager::INSTALL_SUCCEEDED) {
//        // Discontinue if moving dex files failed.
//        return;
//    }
    if((res->mReturnCode = SetPermissionsLI(newCapsule))
            != CapsuleManager::INSTALL_SUCCEEDED) {
        if (mInstaller != NULL) {
            mInstaller->RmDex(newCapsule->mScanPath);
        }
        return;
    }
    else {
        Logger::D(TAG, StringBuffer("New package installed in ") + newCapsule->mPath);
    }
    {
        Mutex::Autolock lock(mCapsulesLock);

        UpdatePermissionsLP(newCapsule->mCapsuleName, newCapsule,
                newCapsule->mPermissions.Begin() != newCapsule->mPermissions.End(),
                TRUE, FALSE);
        res->mName = capName;
        newCapsule->mApplicationInfo->GetUid(&res->mUid);
        res->mCap = newCapsule;
        mSettings->SetInstallStatus(capName, CAP_INSTALL_COMPLETE);
        mSettings->SetInstallerCapsuleName(capName, installerCapsuleName);
        res->mReturnCode = CapsuleManager::INSTALL_SUCCEEDED;
        //to update install status
        mSettings->WriteLP();
    }
}

void CCapsuleManagerService::InstallCapsuleLI(
    /* [in] */ InstallArgs* args,
    /* [in] */ Boolean newInstall,
    /* [in] */ CapsuleInstalledInfo* res)
{
    Int32 pFlags = args->mFlags;
    String installerCapsuleName = args->mInstallerCapsuleName;
    AutoPtr<IFile> tmpCapsuleFile;
    CFile::New(args->GetCodePath(), (IFile**)&tmpCapsuleFile);
    Boolean forwardLocked = ((pFlags & CapsuleManager::INSTALL_FORWARD_LOCK) != 0);
    Boolean onSd = ((pFlags & CapsuleManager::INSTALL_EXTERNAL) != 0);
    Boolean replace = FALSE;
    Int32 scanMode = (onSd ? 0 : SCAN_MONITOR) | SCAN_FORCE_DEX | SCAN_UPDATE_SIGNATURE
            | (newInstall ? SCAN_NEW_INSTALL : 0);
    // Result object to be returned
    res->mReturnCode = CapsuleManager::INSTALL_SUCCEEDED;

    // Retrieve PackageSettings and parse package
    Int32 parseFlags = CapsuleParser::PARSE_CHATTY |
        (forwardLocked ? CapsuleParser::PARSE_FORWARD_LOCK : 0) |
        (onSd ? CapsuleParser::PARSE_ON_SDCARD : 0);
    parseFlags |= mDefParseFlags;
    String path;
    tmpCapsuleFile->GetPath(&path);
    CapsuleParser pp(path);
    pp.SetSeparateProcesses(*mSeparateProcesses.Get());
    CapsuleParser::Capsule* cap = pp.ParseCapsule(tmpCapsuleFile,
            String(NULL), mMetrics, parseFlags);
    if (cap == NULL) {
        res->mReturnCode = pp.GetParseError();
        return;
    }
    String capName = res->mName = cap->mCapsuleName;
    Int32 capFlags;
    cap->mApplicationInfo->GetFlags(&capFlags);
    if ((capFlags & ApplicationInfo_FLAG_TEST_ONLY) != 0) {
        if ((pFlags & CapsuleManager::INSTALL_ALLOW_TEST) == 0) {
            res->mReturnCode = CapsuleManager::INSTALL_FAILED_TEST_ONLY;
            return;
        }
    }
    if (GET_CERTIFICATES && !pp.CollectCertificates(cap, parseFlags)) {
        res->mReturnCode = pp.GetParseError();
        return;
    }
    String oldCodePath;
    Boolean systemApp = FALSE;
    {
        Mutex::Autolock lock(mCapsulesLock);

        // Check if installing already existing package
        if ((pFlags & CapsuleManager::INSTALL_REPLACE_EXISTING) != 0) {
            String oldName;
            HashMap<String, String>::Iterator it = mSettings->mRenamedCapsules.Find(capName);
            if (it != mSettings->mRenamedCapsules.End()) {
                oldName = it->mSecond;
            }
            if (cap->mOriginalCapsules != NULL
                    && Find(cap->mOriginalCapsules->Begin(),
                        cap->mOriginalCapsules->End(), oldName)
                        != cap->mOriginalCapsules->End()
                    && mCapsules.Find(oldName) != mCapsules.End()) {
                // This package is derived from an original package,
                // and this device has been updating from that original
                // name.  We must continue using the original name, so
                // rename the new package here.
                cap->SetCapsuleName(oldName);
                capName = cap->mCapsuleName;
                replace = TRUE;
            }
            else if (mCapsules.Find(capName) != mCapsules.End()) {
                // This package, under its official name, already exists
                // on the device; we should replace it.
                replace = TRUE;
            }
        }
        CapsuleSetting* ps = NULL;
        HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it =
                mSettings->mCapsules.Find(capName);
        if (it != mSettings->mCapsules.End()) {
            ps = it->mSecond;
        }
        if (ps != NULL) {
            oldCodePath = ps->mCodePathString;
            if (ps->mCap != NULL && ps->mCap->mApplicationInfo != NULL) {
                Int32 psFlags;
                ps->mCap->mApplicationInfo->GetFlags(&psFlags);
                systemApp = (psFlags & ApplicationInfo_FLAG_SYSTEM) != 0;
            }
        }
    }

    if (systemApp && onSd) {
        // Disable updates to system apps on sdcard
        Slogger::W(TAG, "Cannot install updates to system apps on sdcard");
        res->mReturnCode = CapsuleManager::INSTALL_FAILED_INVALID_INSTALL_LOCATION;
        return;
    }

    if (!args->DoRename(res->mReturnCode, capName, oldCodePath)) {
        res->mReturnCode = CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
        return;
    }
    // Set application objects path explicitly after the rename
    SetApplicationInfoPaths(cap, args->GetCodePath(), args->GetResourcePath());
    cap->mApplicationInfo->SetNativeLibraryDir(args->GetNativeLibraryPath());
    if (replace) {
        ReplaceCapsuleLI(cap, parseFlags, scanMode,
                installerCapsuleName, res);
    }
    else {
        InstallNewCapsuleLI(cap, parseFlags, scanMode,
                installerCapsuleName,res);
    }
}

Int32 CCapsuleManagerService::SetPermissionsLI(
    /* [in] */ CapsuleParser::Capsule* newCapsule)
{
    String capName = newCapsule->mCapsuleName;
    Int32 retCode = 0;
    // TODO Gross hack but fix later. Ideally move this to be a post installation
    // check after alloting uid.
    if (IsForwardLocked(newCapsule)) {
        String pubSrcDir;
        newCapsule->mApplicationInfo->GetPublicSourceDir(&pubSrcDir);
        AutoPtr<IFile> destResourceFile;
        CFile::New(pubSrcDir, (IFile**)&destResourceFile);
        if (FAILED(ExtractPublicFiles(newCapsule, destResourceFile))) {
            Slogger::E(TAG, StringBuffer("Couldn't create a new zip file for the public parts of a")
                    + " forward-locked app.");
            return CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
        }

        Int32 uid;
            newCapsule->mApplicationInfo->GetUid(&uid);
        if (mInstaller != NULL) {
            retCode = mInstaller->SetForwardLockPerm(GetCapName(newCapsule->mPath), uid);
        }
        else {
            const Int32 filePermissions =
                    FileUtils::IRUSR | FileUtils::IWUSR | FileUtils::IRGRP;
            retCode = FileUtils::SetPermissions(newCapsule->mPath, filePermissions, -1, uid);
        }
    }
    else {
        // The permissions on the resource file was set when it was copied for
        // non forward locked apps and apps on sdcard
    }

    if (retCode != 0) {
        Slogger::E(TAG, StringBuffer("Couldn't set new package file permissions for ")
                + newCapsule->mPath + ". The return code was: " + retCode);
        // TODO Define new internal error
        return CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
    }
    return CapsuleManager::INSTALL_SUCCEEDED;
}

Boolean CCapsuleManagerService::IsForwardLocked(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    Int32 capFlags;
    cap->mApplicationInfo->GetFlags(&capFlags);
    return (capFlags & ApplicationInfo_FLAG_FORWARD_LOCK) != 0;
}

Boolean CCapsuleManagerService::IsExternal(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    Int32 capFlags;
    cap->mApplicationInfo->GetFlags(&capFlags);
    return (capFlags & ApplicationInfo_FLAG_EXTERNAL_STORAGE) != 0;
}

Boolean CCapsuleManagerService::IsSystemApp(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    Int32 capFlags;
    cap->mApplicationInfo->GetFlags(&capFlags);
    return (capFlags & ApplicationInfo_FLAG_SYSTEM) != 0;
}

Boolean CCapsuleManagerService::IsSystemApp(
    /* [in] */ IApplicationInfo* info)
{
    Int32 flags;
    info->GetFlags(&flags);
    return (flags & ApplicationInfo_FLAG_SYSTEM) != 0;
}

Boolean CCapsuleManagerService::IsUpdatedSystemApp(
    /* [in] */ CapsuleParser::Capsule* cap)
{
    Int32 capFlags;
    cap->mApplicationInfo->GetFlags(&capFlags);
    return (capFlags & ApplicationInfo_FLAG_UPDATED_SYSTEM_APP) != 0;
}

ECode CCapsuleManagerService::ExtractPublicFiles(
    /* [in] */ CapsuleParser::Capsule* newCapsule,
    /* [in] */ IFile* publicZipFile)
{
    AutoPtr<IFileOutputStream> fstr;
    AutoPtr<IZipOutputStream> publicZipOutStream;
    AutoPtr<IZipFile> privateZip;
    FAIL_RETURN(CFileOutputStream::New(publicZipFile, (IFileOutputStream **)&fstr));
    FAIL_RETURN(CZipOutputStream::New((IFileOutputStream*)fstr, (IZipOutputStream **)&publicZipOutStream));
    FAIL_RETURN(CZipFile::New(newCapsule->mPath, (IZipFile **)&privateZip));

    // Copy manifest, resources.arsc and res directory to public zip

    AutoPtr<IObjectContainer> container;
    FAIL_RETURN(privateZip->GetEntries((IObjectContainer **)&container));
    AutoPtr<IObjectEnumerator> emu;
    FAIL_RETURN(container->GetObjectEnumerator((IObjectEnumerator **)&emu));
    Boolean hasNext;
    while(emu->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        emu->Current((IInterface**)&obj);
        IZipEntry* zipEntry = IZipEntry::Probe(obj);
        String zipEntryName;
        zipEntry->GetName(&zipEntryName);
        if (zipEntryName.Equals("AndroidManifest.xml")
            || zipEntryName.Equals("resources.arsc")
            || zipEntryName.StartWith("res/")) {
            // try {
            CopyZipEntry(zipEntry, privateZip, publicZipOutStream);
           //  } catch (IOException e) {
           //     try {
           //         publicZipOutStream.close();
           //         throw e;
           //     } finally {
           //         publicZipFile.delete();
           //     }
           // }
        }
    }
    publicZipOutStream->Finish();
    publicZipOutStream->Flush();
//    FileUtils.sync(fstr);
    publicZipOutStream->Close();
//    FileUtils.setPermissions(
//            publicZipFile.getAbsolutePath(),
//            FileUtils.S_IRUSR|FileUtils.S_IWUSR|FileUtils.S_IRGRP|FileUtils.S_IROTH,
//            -1, -1);
    return NOERROR;
}

void CCapsuleManagerService::CopyZipEntry(
    /* in */ IZipEntry* zipEntry,
    /* in */ IZipFile* inZipFile,
    /* in */ IZipOutputStream* outZipStream)
{
    ArrayOf_<Byte, 4096> buffer;
    Int32 num;

    Int32 method;
    AutoPtr<IZipEntry> newEntry;
    zipEntry->GetMethod(&method);
    if (method == IZipEntry_STORED) {
        ASSERT_SUCCEEDED(CZipEntry::New(zipEntry, (IZipEntry **)&newEntry));
    }
    else {
        String name;
        zipEntry->GetName(&name);
        ASSERT_SUCCEEDED(CZipEntry::New(name, (IZipEntry **)&newEntry));
    }
    outZipStream->PutNextEntry(newEntry);

    AutoPtr<IInputStream> data;
    inZipFile->GetInputStream(zipEntry, (IInputStream **)&data);
    while(data->ReadBuffer(&buffer, &num), num > 0) {
      outZipStream->WriteBufferEx(0, num, buffer);
    }
    outZipStream->Flush();
}

void CCapsuleManagerService::DeleteTempCapsuleFiles()
{
//	    FilenameFilter filter = new FilenameFilter() {
//	        public boolean accept(File dir, String name) {
//	            return name.startsWith("vmdl") && name.endsWith(".tmp");
//	        }
//	    };
//	    String tmpFilesList[] = mAppInstallDir.list(filter);
//	    if(tmpFilesList == null) {
//	        return;
//	    }
//	    for(int i = 0; i < tmpFilesList.length; i++) {
//	        File tmpFile = new File(mAppInstallDir, tmpFilesList[i]);
//	        tmpFile.delete();
//	    }
}

AutoPtr<IFile> CCapsuleManagerService::CreateTempCapsuleFile(
    /* [in] */ IFile* installDir)
{
    AutoPtr<IFile> tmpCapsuleFile;
//    try {
//        tmpPackageFile = File.createTempFile("vmdl", ".tmp", installDir);
//    } catch (IOException e) {
//        Slog.e(TAG, "Couldn't create temp file for downloaded package file.");
//        return null;
//    }
//    try {
//        FileUtils.setPermissions(
//                tmpPackageFile.getCanonicalPath(), FileUtils.S_IRUSR|FileUtils.S_IWUSR,
//                -1, -1);
//    } catch (IOException e) {
//        Slog.e(TAG, "Trouble getting the canoncical path for a temp file.");
//        return null;
//    }
    return tmpCapsuleFile;
}

ECode CCapsuleManagerService::DeleteCapsule(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDeleteObserver* observer,
    /* [in] */ Int32 flags)
{
    VALIDATE_STRING_NOT_NULL(capsuleName);

    mContext->EnforceCallingOrSelfPermission(
        String("") /*android.Manifest.permission.DELETE_CAPSULES*/, String(NULL));

    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
        const String&, ICapsuleDeleteObserver*, Int32);
    pHandlerFunc = &CCapsuleManagerService::HandleDeleteCapsule;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(capsuleName);
    params->WriteInterfacePtr(observer);
    params->WriteInt32(flags);

    SendMessage(*(Handle32*)&pHandlerFunc, params);
    return NOERROR;
}

void CCapsuleManagerService::HandleDeleteCapsule(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDeleteObserver* observer,
    /* [in] */ Int32 flags)
{
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
        const String&, ICapsuleDeleteObserver*, Int32);
    pHandlerFunc = &CCapsuleManagerService::HandleDeleteCapsule;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    Boolean succeded = DeleteCapsuleX(capsuleName, TRUE, TRUE, flags);
    if (observer != NULL) {
        if (FAILED(observer->CapsuleDeleted(succeded))) {
            Logger::I(TAG, "Observer no longer exists.");
        }
    }
}

/**
 *  This method is an internal method that could be get invoked either
 *  to delete an installed package or to clean up a failed installation.
 *  After deleting an installed package, a broadcast is sent to notify any
 *  listeners that the package has been installed. For cleaning up a failed
 *  installation, the broadcast is not necessary since the package's
 *  installation wouldn't have sent the initial broadcast either
 *  The key steps in deleting a package are
 *  deleting the package information in internal structures like mPackages,
 *  deleting the packages base directories through installd
 *  updating mSettings to reflect current status
 *  persisting settings for later use
 *  sending a broadcast if necessary
 */
Boolean CCapsuleManagerService::DeleteCapsuleX(
    /* [in] */ const String& capsuleName,
    /* [in] */ Boolean sendBroadCast,
    /* [in] */ Boolean deleteCodeAndResources,
    /* [in] */ Int32 flags)
{
    //todo: when to delete info
    CapsuleRemovedInfo* info = new CapsuleRemovedInfo(this);
    Boolean res = FALSE;

//    IDevicePolicyManager dpm = IDevicePolicyManager::Stub::AsInterface(
//            ServiceManager::GetService(Context::DEVICE_POLICY_SERVICE));
//    try {
//        if (dpm != NULL && dpm->CapsuleHasActiveAdmins(capsuleName)) {
//            Slogger::W(TAG, "Not removing capsule " + capsuleName + ": has active device admin");
//            return FALSE;
//        }
//    } catch (RemoteException e) {
//    }

    {
        Mutex::Autolock lock(mInstallLock);
        res = DeleteCapsuleLI(capsuleName, deleteCodeAndResources,
                flags | REMOVE_CHATTY, info, TRUE);
    }

    if (res && sendBroadCast) {
        Boolean systemUpdate = info->mIsRemovedCapsuleSystemUpdate;
        info->SendBroadcast(deleteCodeAndResources, systemUpdate);

        // If the removed capsule was a system update, the old system capsuled
        // was re-enabled; we need to broadcast this information
        if (systemUpdate) {
            AutoPtr<IBundle> extras;
            ASSERT_SUCCEEDED(CBundle::New(1, (IBundle**)&extras));
            extras->PutInt32(String(Intent_EXTRA_UID), info->mRemovedUid >= 0 ? info->mRemovedUid : info->mUid);
            extras->PutBoolean(String(Intent_EXTRA_REPLACING), TRUE);

            SendCapsuleBroadcast(String(Intent_ACTION_CAPSULE_ADDED), capsuleName, extras, NULL);
            SendCapsuleBroadcast(String(Intent_ACTION_CAPSULE_REPLACED), capsuleName, extras, NULL);
        }
    }
    // Delete the resources here after sending the broadcast to let
    // other processes clean up before deleting resources.
    if (info->mArgs != NULL) {
        Mutex::Autolock lock(mInstallLock);
        info->mArgs->DoPostDeleteLI(deleteCodeAndResources);
    }
    return res;
}

/*
 * This method deletes the package from internal data structures. If the DONT_DELETE_DATA
 * flag is not set, the data directory is removed as well.
 * make sure this flag is set for partially installed apps. If not its meaningless to
 * delete a partially installed application.
 */
void CCapsuleManagerService::RemoveCapsuleDataLI(
    /* [in] */ CapsuleParser::Capsule* p,
    /* [in] */ CapsuleRemovedInfo* outInfo,
    /* [in] */ Int32 flags,
    /* [in] */ Boolean writeSettings)
{
    String capsuleName = p->mCapsuleName;
    if (outInfo != NULL) {
        outInfo->mRemovedCapsule = capsuleName;
    }
    RemoveCapsuleLI(p, (flags & REMOVE_CHATTY) != 0);
    // Retrieve object to delete permissions for shared user later on
    CapsuleSetting* deletedCs = NULL;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, AutoPtr<CapsuleSetting> >::Iterator itor
                = mSettings->mCapsules.Find(capsuleName);
        if (itor != mSettings->mCapsules.End()) {
            deletedCs = itor->mSecond;
        }
    }
    if ((flags & CapsuleManager_DONT_DELETE_DATA) == 0) {
        Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule(p);
        if (mInstaller != NULL) {
            Int32 retCode = mInstaller->Remove(capsuleName, useEncryptedFSDir);
            if (retCode < 0) {
                Slogger::W(TAG, StringBuffer("Couldn't remove app data or cache directory for capsule: ")
                           + capsuleName + ", retcode=" + retCode);
                // we don't consider this to be a failure of the core capsule deletion
            }
        }
        else {
            // for simulator
            CapsuleParser::Capsule* cap = NULL;
            HashMap<String, CapsuleParser::Capsule*>::Iterator it
                = mCapsules.Find(capsuleName);
            if (it != mCapsules.End()) {
                cap = it->mSecond;
            }
            assert(cap);
            String capDataDir;
            cap->mApplicationInfo->GetDataDir(&capDataDir);
            AutoPtr<IFile> dataDir;
            ASSERT_SUCCEEDED(CFile::New(capDataDir, (IFile**)&dataDir));
            Boolean isDeleted = FALSE;
            dataDir->Delete(&isDeleted);
        }
        ScheduleCapsuleCleaning(capsuleName);
    }
    {
        Mutex::Autolock lock(mCapsulesLock);
        if (deletedCs != NULL) {
            if ((flags & CapsuleManager_DONT_DELETE_DATA) == 0) {
                if (outInfo != NULL) {
                    outInfo->mRemovedUid = mSettings->RemoveCapsuleLP(capsuleName);
                }
                if (deletedCs != NULL) {
                    UpdatePermissionsLP(deletedCs->mName, NULL, FALSE, FALSE, FALSE);
                    if (deletedCs->mSharedUser != NULL) {
                        // remove permissions associated with capsule
                        mSettings->UpdateSharedUserPermsLP(deletedCs, mGlobalGids);
                    }
                }
            }
            // remove from preferred activities.
            List< AutoPtr<PreferredActivity> > removed;
            Set<PreferredActivity*>* list = mSettings->mPreferredActivities->FilterSet();
            if (list) {
                Set<PreferredActivity*>::Iterator it;
                for (it = list->Begin(); it != list->End(); ++it) {
                    AutoPtr<PreferredActivity> pa = *it;
                    String name;
                    pa->mActivity->GetCapsuleName(&name);
                    if (name.Equals(deletedCs->mName)) {
                        removed.PushBack(pa);
                    }
                }
            }
            List< AutoPtr<PreferredActivity> >::Iterator it;
            for (it = removed.Begin(); it != removed.End(); ++it) {
                AutoPtr<PreferredActivity> pa = *it;
                mSettings->mPreferredActivities->RemoveFilter(pa);
            }
        }
        if (writeSettings) {
            // Save settings now
            mSettings->WriteLP();
        }
    }
}

/*
 * Tries to delete system package.
 */
Boolean CCapsuleManagerService::DeleteSystemCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* c,
    /* [in] */ Int32 flags,
    /* [in] */ CapsuleRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings)
{
    IApplicationInfo* applicationInfo = c->mApplicationInfo;
    //applicable for non-partially installed applications only
    if (applicationInfo == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule ") + c->mCapsuleName + " has no applicationInfo.");
        return FALSE;
    }
    CapsuleSetting* cs = NULL;
    // Confirm if the system capsule has been updated
    // An updated system app can be deleted. This will also have to restore
    // the system cap from system partition
    {
    	Mutex::Autolock lock(mCapsulesLock);
        cs = mSettings->GetDisabledSystemCap(c->mCapsuleName);
    }
    if (cs == NULL) {
        Slogger::W(TAG, StringBuffer("Attempt to delete unknown system capsule ") + c->mCapsuleName);
        return FALSE;
    }
	else {
        Logger::I(TAG, "Deleting system cap from data partition");
    }
    // Delete the updated capsule
    outInfo->mIsRemovedCapsuleSystemUpdate = TRUE;
    if (cs->mVersionCode < c->mVersionCode) {
        // Delete data for downgrades
        flags &= ~CapsuleManager_DONT_DELETE_DATA;
    }
    else {
        // Preserve data by setting flag
        flags |= CapsuleManager_DONT_DELETE_DATA;
    }
    Boolean ret = DeleteInstalledCapsuleLI(c, TRUE, flags, outInfo, writeSettings);
    if (!ret) {
        return FALSE;
    }
    {
		Mutex::Autolock lock(mCapsulesLock);
        // Reinstate the old system capsule
        mSettings->EnableSystemCapsuleLP(c->mCapsuleName);
        // Remove any native libraries from the upgraded capsule.
        String natLibDir;
        c->mApplicationInfo->GetNativeLibraryDir(&natLibDir);
        NativeLibraryHelper::RemoveNativeBinariesLI(natLibDir);
    }
    // Install the system capsule
    CapsuleParser::Capsule* newCap = ScanCapsuleLI(cs->mCodePath,
            CapsuleParser::PARSE_MUST_BE_APK | CapsuleParser::PARSE_IS_SYSTEM,
            SCAN_MONITOR | SCAN_NO_PATHS, 0);
    if (newCap == NULL) {
        Slogger::W(TAG, StringBuffer("Failed to restore system capsule:")
						+ c->mCapsuleName + " with error:" + mLastScanError);
        return FALSE;
    }
    {
		Mutex::Autolock lock(mCapsulesLock);
        UpdatePermissionsLP(newCap->mCapsuleName, newCap, TRUE, TRUE, FALSE);
        if (writeSettings) {
            mSettings->WriteLP();
        }
    }
    return TRUE;
}

Boolean CCapsuleManagerService::DeleteInstalledCapsuleLI(
    /* [in] */ CapsuleParser::Capsule* c,
    /* [in] */ Boolean deleteCodeAndResources,
    /* [in] */ Int32 flags,
    /* [in] */ CapsuleRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings)
{
    IApplicationInfo* appInfo = c->mApplicationInfo;
    if (appInfo == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule ") + c->mCapsuleName + " has no applicationInfo.");
        return FALSE;
    }
    if (outInfo != NULL) {
        appInfo->GetUid(&outInfo->mUid);
    }

    // Delete capsule data from Int32ernal structures and also remove data if flag is set
    RemoveCapsuleDataLI(c, outInfo, flags, writeSettings);

    // Delete application code and resources
    if (deleteCodeAndResources) {
        // TODO can pick up from CapsuleSettings as well
        Int32 installFlags = IsExternal(c) ? CapsuleManager::INSTALL_EXTERNAL : 0;
        installFlags |= IsForwardLocked(c) ? CapsuleManager::INSTALL_FORWARD_LOCK : 0;
        String srcDir, pubSrcDir, natLibDir;
        appInfo->GetSourceDir(&srcDir);
        appInfo->GetPublicSourceDir(&pubSrcDir);
        appInfo->GetNativeLibraryDir(&natLibDir);
        outInfo->mArgs = CreateInstallArgs(installFlags, srcDir, pubSrcDir, natLibDir);
    }
    return TRUE;
}

/*
 * This method handles package deletion in general
 */
Boolean CCapsuleManagerService::DeleteCapsuleLI(
    /* [in] */ const String& capsuleName,
    /* [in] */ Boolean deleteCodeAndResources,
    /* [in] */ Int32 flags,
    /* [in] */ CapsuleRemovedInfo* outInfo,
    /* [in] */ Boolean writeSettings)
{
    if (capsuleName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete NULL capsuleName.");
        return FALSE;
    }
    CapsuleParser::Capsule* p = NULL;
    Boolean dataOnly = FALSE;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                mCapsules.Find(capsuleName);
        if (it != mCapsules.End()) {
            p = it->mSecond;
        }
        if (p == NULL) {
            //this retrieves partially installed apps
            dataOnly = TRUE;
            CapsuleSetting* ps = NULL;
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it =
                    mSettings->mCapsules.Find(capsuleName);
            if (it != mSettings->mCapsules.End()) {
                ps = it->mSecond;
            }
            if (ps == NULL) {
                Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName +"' doesn't exist.");
                return FALSE;
            }
            p = it->mSecond->mCap;
        }
    }
    if (p == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName +"' doesn't exist.");
        return FALSE;
    }

    if (dataOnly) {
        // Delete application data first
        RemoveCapsuleDataLI(p, outInfo, flags, writeSettings);
        return TRUE;
    }
    // At this point the capsule should have ApplicationInfo associated with it
    if (p->mApplicationInfo == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule ") + p->mCapsuleName + " has no applicationInfo.");
        return FALSE;
    }
    Boolean ret = FALSE;
    if (IsSystemApp(p)) {
        Logger::I(TAG, StringBuffer("Removing system capsule:") + p->mCapsuleName);
        // When an updated system application is deleted we delete the existing resources as well and
        // fall back to existing code in system partition
        ret = DeleteSystemCapsuleLI(p, flags, outInfo, writeSettings);
    }
    else {
        Logger::I(TAG, StringBuffer("Removing non-system capsule:") + p->mCapsuleName);
        // Kill application pre-emptively especially for apps on sd.
        Int32 pUid;
        p->mApplicationInfo->GetUid(&pUid);
        KillApplication(capsuleName, pUid);
        ret = DeleteInstalledCapsuleLI(p, deleteCodeAndResources, flags, outInfo, writeSettings);
    }
    return ret;
}

ECode CCapsuleManagerService::ClearApplicationUserData(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*android.Manifest.permission.CLEAR_APP_USER_DATA*/, String(NULL));
    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(const String&, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleClearApplicationUserData;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(capsuleName);
    params->WriteInt32((Handle32)observer);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CCapsuleManagerService::HandleClearApplicationUserData(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
//    mHandler.removeCallbacks(this);
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(const String&, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleClearApplicationUserData;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    Boolean succeeded = FALSE;
    {
        Mutex::Autolock lock(mInstallLock);
        succeeded = ClearApplicationUserDataLI(capsuleName);
    }

//	    if (succeeded) {
//	        // invoke DeviceStorageMonitor's update method to clear any notifications
//	        DeviceStorageMonitorService dsm = (DeviceStorageMonitorService)
//	                ServiceManager::GetService(DeviceStorageMonitorService::SERVICE);
//	        if (dsm != NULL) {
//	            dsm->UpdateMemory();
//	        }
//	    }

    if (observer != NULL) {
        if (FAILED(observer->OnRemoveCompleted(capsuleName, succeeded))) {
            Logger::I(TAG, "Observer no Int64er exists->");
        }
    }
}

Boolean CCapsuleManagerService::ClearApplicationUserDataLI(
    /* [in] */ const String& capsuleName)
{
    if (capsuleName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete NULL capsuleName->");
        return FALSE;
    }
    CapsuleParser::Capsule* c = NULL;
    Boolean dataOnly = FALSE;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                mCapsules.Find(capsuleName);
        if (it != mCapsules.End()) {
            c = it->mSecond;
        }
        if (c == NULL) {
            dataOnly = TRUE;
            CapsuleSetting* cs = NULL;
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator csIt =
                    mSettings->mCapsules.Find(capsuleName);
            if (csIt != mSettings->mCapsules.End()) {
                cs = csIt->mSecond;
            }
            if ((cs == NULL) || (cs->mCap == NULL)) {
                Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName + "' doesn't exist->");
                return FALSE;
            }
            c = cs->mCap;
        }
    }

    Boolean useEncryptedFSDir = FALSE;

    if (!dataOnly) {
        //need to check this only for fully installed applications
        if (c == NULL) {
            Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName + "' doesn't exist->");
            return FALSE;
        }
        if (c->mApplicationInfo == NULL) {
            Slogger::W(TAG, StringBuffer("Capsule ") + capsuleName + " has no applicationInfo->");
            return FALSE;
        }
        useEncryptedFSDir = UseEncryptedFilesystemForCapsule(c);
    }
    if (mInstaller != NULL) {
        Int32 retCode = mInstaller->ClearUserData(capsuleName, useEncryptedFSDir);
        if (retCode < 0) {
            Slogger::W(TAG,
                StringBuffer("Couldn't remove cache files for capsule: ")
                + capsuleName);
            return FALSE;
        }
    }
    return TRUE;
}

ECode CCapsuleManagerService::DeleteApplicationCacheFiles(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*android.Manifest.permission.DELETE_CACHE_FILES*/, String(NULL));
    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(const String&, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleDeleteApplicationCacheFiles;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(capsuleName);
    params->WriteInt32((Handle32)observer);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CCapsuleManagerService::HandleDeleteApplicationCacheFiles(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleDataObserver* observer)
{
//    mHandler.removeCallbacks(this);
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(const String&, ICapsuleDataObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleDeleteApplicationCacheFiles;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    Boolean succeded = FALSE;
    {
        Mutex::Autolock lock(mInstallLock);
        succeded = DeleteApplicationCacheFilesLI(capsuleName);
    }

    if (observer != NULL) {
        if (FAILED(observer->OnRemoveCompleted(capsuleName, succeded))) {
            Logger::I(TAG, "Observer no Int64er exists->");
        }
    }
}

Boolean CCapsuleManagerService::DeleteApplicationCacheFilesLI(
    /* [in] */ const String& capsuleName)
{
    if (capsuleName.IsNull()) {
        Slogger::W(TAG, "Attempt to delete NULL capsuleName->");
        return FALSE;
    }
    CapsuleParser::Capsule* c = NULL;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                mCapsules.Find(capsuleName);
        if (it != mCapsules.End()) {
            c = it->mSecond;
        }
    }
    if (c == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName +"' doesn't exist->");
        return FALSE;
    }
    if (c->mApplicationInfo == NULL) {
        Slogger::W(TAG, StringBuffer("Capsule ") + capsuleName + " has no applicationInfo->");
        return FALSE;
    }
    Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule(c);
    if (mInstaller != NULL) {
        Int32 retCode = mInstaller->DeleteCacheFiles(capsuleName, useEncryptedFSDir);
        if (retCode < 0) {
            Slogger::W(TAG,
                StringBuffer("Couldn't remove cache files for capsule: ")
                + capsuleName);
            return FALSE;
        }
    }
    return TRUE;
}

ECode CCapsuleManagerService::GetCapsuleSizeInfo(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleStatsObserver* observer)
{
    mContext->EnforceCallingOrSelfPermission(
        String("") /*android.Manifest.permission.GET_PACKAGE_SIZE*/, String(NULL));
    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(const String&, ICapsuleStatsObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleGetCapsuleSizeInfo;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(capsuleName);
    params->WriteInt32((Handle32)observer);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CCapsuleManagerService::HandleGetCapsuleSizeInfo(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleStatsObserver* observer)
{
//    mHandler.removeCallbacks(this);
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(const String&, ICapsuleStatsObserver*);
    pHandlerFunc = &CCapsuleManagerService::HandleGetCapsuleSizeInfo;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    AutoPtr<ICapsuleStats> lStats;
    CCapsuleStats::New(capsuleName, (ICapsuleStats**)&lStats);

    Boolean succeded = FALSE;
    {
        Mutex::Autolock lock(mInstallLock);
        succeded = GetCapsuleSizeInfoLI(capsuleName, lStats);
    }

    if (observer != NULL) {
        if (FAILED(observer->OnGetStatsCompleted(lStats, succeded))) {
            Logger::I(TAG, "Observer no Int64er exists->");
        }
    }
}

Boolean CCapsuleManagerService::GetCapsuleSizeInfoLI(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleStats* stats)
{
    if (capsuleName.IsNull()) {
        Slogger::W(TAG, "Attempt to get size of NULL capsuleName->");
        return FALSE;
    }
    CapsuleParser::Capsule* c = NULL;
    Boolean dataOnly = FALSE;
    {
        Mutex::Autolock lock(mCapsulesLock);
        HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                mCapsules.Find(capsuleName);
        if (it != mCapsules.End()) {
            c = it->mSecond;
        }
        if (c == NULL) {
            dataOnly = TRUE;
            CapsuleSetting* cs = NULL;
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator csIt =
                    mSettings->mCapsules.Find(capsuleName);
            if (csIt != mSettings->mCapsules.End()) {
                cs = csIt->mSecond;
            }
            if ((cs == NULL) || (cs->mCap == NULL)) {
                Slogger::W(TAG, StringBuffer("Capsule named '") + capsuleName + "' doesn't exist->");
                return FALSE;
            }
            c = cs->mCap;
        }
    }
    String publicSrcDir;
    if (!dataOnly) {
        if (c->mApplicationInfo == NULL) {
            Slogger::W(TAG, StringBuffer("Capsule ") + capsuleName + " has no applicationInfo->");
            return FALSE;
        }
        if (IsForwardLocked(c)) {
        	c->mApplicationInfo->GetPublicSourceDir(&publicSrcDir);
		}
    }
    Boolean useEncryptedFSDir = UseEncryptedFilesystemForCapsule(c);
    if (mInstaller != NULL) {
        Int32 res = mInstaller->GetSizeInfo(
            capsuleName, c->mPath, publicSrcDir, stats, useEncryptedFSDir);
        return res >= 0 ? TRUE : FALSE;
    }
    return TRUE;
}

ECode CCapsuleManagerService::AddCapsuleToPreferred(
    /* [in] */ const String& capsuleName)
{
    Slogger::W(TAG, "addCapsuleToPreferred: this is now a no-op");
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::RemoveCapsuleFromPreferred(
    /* [in] */ const String& capsuleName)
{
    Slogger::W(TAG, "removeCapsuleFromPreferred: this is now a no-op");
    return E_NOT_IMPLEMENTED;
}

ECode CCapsuleManagerService::GetPreferredCapsules(
    /* [in] */ Int32 flags,
    /* [out, callee] */ IObjectContainer** infos)
{
    VALIDATE_NOT_NULL(infos);
    return CParcelableObjectContainer::New(infos);
}

Int32 CCapsuleManagerService::GetUidTargetSdkVersionLockedLP(
    /* [in] */ Int32 uid)
{
//	    Object obj = mSettings.getUserIdLP(uid);
//	    if (obj instanceof SharedUserSetting) {
//	        SharedUserSetting sus = (SharedUserSetting)obj;
//	        final int N = sus.packages.size();
//	        int vers = Build.VERSION_CODES.CUR_DEVELOPMENT;
//	        Iterator<PackageSetting> it = sus.packages.iterator();
//	        int i=0;
//	        while (it.hasNext()) {
//	            PackageSetting ps = it.next();
//	            if (ps.pkg != null) {
//	                int v = ps.pkg.applicationInfo.targetSdkVersion;
//	                if (v < vers) vers = v;
//	            }
//	        }
//	        return vers;
//	    } else if (obj instanceof PackageSetting) {
//	        PackageSetting ps = (PackageSetting)obj;
//	        if (ps.pkg != null) {
//	            return ps.pkg.applicationInfo.targetSdkVersion;
//	        }
//	    }
//	    return Build.VERSION_CODES.CUR_DEVELOPMENT;
    return -1;
}

ECode CCapsuleManagerService::AddPreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ IObjectContainer* set, /*IComponentName*/
    /* [in] */ IComponentName* activity)
{
    Mutex::Autolock lock(mCapsulesLock);

    Int32 perm = 0;
    mContext->CheckCallingOrSelfPermission(
        String("") /*android.Manifest.permission.SET_PREFERRED_APPLICATIONS*/, &perm);
    if (perm != CapsuleManager_PERMISSION_GRANTED) {
        if (GetUidTargetSdkVersionLockedLP(-1 /*Binder::GetCallingUid()*/)
                < Build::VERSION_CODES::FROYO) {
            Slogger::W(TAG, StringBuffer("Ignoring AddPreferredActivity() from uid ")
                   + 1 /*Binder::GetCallingUid()*/);
            return NOERROR;
        }
        mContext->EnforceCallingOrSelfPermission(
            String("") /*android.Manifest.permission.SET_PREFERRED_APPLICATIONS*/, String(NULL));
    }

    Slogger::I(TAG, StringBuffer("Adding preferred activity ") + activity + ":");

//	    filter->Dump(new LogPrinter(Log::INFO, TAG), "  ");

    AutoPtr<PreferredActivity> pa = new PreferredActivity(filter, match, set, activity);
    if (pa == NULL) return E_OUT_OF_MEMORY_ERROR;
    mSettings->mPreferredActivities->AddFilter(pa);
    ScheduleWriteSettingsLocked();

    return NOERROR;
}

ECode CCapsuleManagerService::ReplacePreferredActivity(
    /* [in] */ IIntentFilter* filter,
    /* [in] */ Int32 match,
    /* [in] */ IObjectContainer* set, /*IComponentName*/
    /* [in] */ IComponentName* activity)
{
    VALIDATE_NOT_NULL(filter);

    Int32 count = 0;
    filter->CountActions(&count);
    if (count != 1) {
        Logger::E(TAG, "replacePreferredActivity expects filter to have only 1 action.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    filter->CountCategories(&count);
    if (count != 1) {
        Logger::E(TAG, "replacePreferredActivity expects filter to have only 1 category.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 countA = 0, countP = 0, countS = 0, countT = 0;
    filter->CountDataAuthorities(&countA);
    filter->CountDataPaths(&countP);
    filter->CountDataSchemes(&countS);
    filter->CountDataTypes(&countT);
    if (countA != 0 || countP != 0 || countS != 0 || countT != 0) {
        Logger::E(TAG,
            "replacePreferredActivity expects filter to have no data authorities, "
            "paths, schemes or types.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    {
        Mutex::Autolock lock(mCapsulesLock);

        Int32 perm = 0;
        mContext->CheckCallingOrSelfPermission(
            String("") /*android.Manifest.permission.SET_PREFERRED_APPLICATIONS*/, &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            if (GetUidTargetSdkVersionLockedLP(-1 /*Binder::GetCallingUid()*/)
                    < Build::VERSION_CODES::FROYO) {
                Slogger::W(TAG,
                    StringBuffer("Ignoring ReplacePreferredActivity() from uid ")
                    + -1 /*Binder::GetCallingUid()*/);
                return NOERROR;
            }
            mContext->EnforceCallingOrSelfPermission(
                String("") /*android.Manifest.permission.SET_PREFERRED_APPLICATIONS*/, String(NULL));
        }

        Set<PreferredActivity*>* pas = mSettings->mPreferredActivities->FilterSet();
        if (pas) {
            String action;
            filter->GetAction(0, &action);
            String category;
            filter->GetCategory(0, &category);
            Set<PreferredActivity*>::Iterator it = pas->Begin();
            while (it != pas->End()) {
                PreferredActivity* pa = *it;
                String a, g;
                pa->GetAction(0, &a);
                pa->GetCategory(0, &g);
                if (a.Equals(action) && g.Equals(category)) {
                    pas->Erase(it++);
                    Logger::I(TAG, StringBuffer("Removed preferred activity ")
                        + pa->mActivity + ":");
//	                    filter->Dump(new LogPrinter(Log::INFO, TAG), "  ");
                }
                else {
                    ++it;
                }
            }
        }
        AddPreferredActivity(filter, match, set, activity);
    }
    return NOERROR;
}

ECode CCapsuleManagerService::ClearCapsulePreferredActivities(
    /* [in] */ const String& capsuleName)
{
    Mutex::Autolock lock(mCapsulesLock);

    Int32 uid = -1 /*Binder::GetCallingUid()*/;
    CapsuleParser::Capsule* cap = NULL;
    HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capsuleName);
    if (it != mCapsules.End()) {
        cap = it->mSecond;
    }
    Int32 capUid;
    if (cap == NULL || (cap->mApplicationInfo->GetUid(&capUid), capUid != uid)) {
        Int32 perm = 0;
        mContext->CheckCallingOrSelfPermission(
            String("") /*android.Manifest.permission.SET_PREFERRED_APPLICATIONS*/, &perm);
        if (perm != CapsuleManager_PERMISSION_GRANTED) {
            if (GetUidTargetSdkVersionLockedLP(-1 /*Binder::GetCallingUid()*/)
                    < Build::VERSION_CODES::FROYO) {
                Slogger::W(TAG,
                    StringBuffer("Ignoring ClearCapsulePreferredActivities() from uid ")
                    + -1 /*Binder::GetCallingUid()*/);
                return NOERROR;
            }
            mContext->EnforceCallingOrSelfPermission(
                String("") /*android.Manifest.permission.SET_PREFERRED_APPLICATIONS*/, String(NULL));
        }
    }

    if (ClearCapsulePreferredActivitiesLP(capsuleName)) {
        ScheduleWriteSettingsLocked();
    }

    return NOERROR;
}

Boolean CCapsuleManagerService::ClearCapsulePreferredActivitiesLP(
    /* [in] */ const String& capsuleName)
{
    Boolean changed = FALSE;
    Set<PreferredActivity*>* set = mSettings->mPreferredActivities->FilterSet();
    Set<PreferredActivity*>::Iterator it = set->Begin();
    while (it != set->End()) {
        PreferredActivity* pa = *it;
        String name;
        pa->mActivity->GetCapsuleName(&name);
        if (name.Equals(capsuleName)) {
            set->Erase(it++);
            changed = TRUE;
        }
        else {
            ++it;
        }
    }
    return changed;
}

ECode CCapsuleManagerService::GetPreferredActivities(
    /* [in] */ IObjectContainer* outFilters,
    /* [in] */ IObjectContainer* outActivities,
    /* [in] */ const String& capsuleName,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    Int32 num = 0;
    {
        Mutex::Autolock lock(mCapsulesLock);
        Set<PreferredActivity*>* set = mSettings->mPreferredActivities->FilterSet();
        Set<PreferredActivity*>::Iterator it;
        for (it = set->Begin(); it != set->End(); ++it) {
            PreferredActivity* pa = *it;
            String name;
            pa->mActivity->GetCapsuleName(&name);
            if (capsuleName.IsNull() || name.Equals(capsuleName)) {
                if (outFilters != NULL) {
                    AutoPtr<IIntentFilter> filter;
                    FAIL_RETURN(CIntentFilter::New(
                        (IIntentFilter*)pa->Probe(EIID_IIntentFilter),
                        (IIntentFilter**)&filter));
                    outFilters->Add(filter);
                }
                if (outActivities != NULL) {
                    outActivities->Add(pa->mActivity);
                }
            }
        }
    }
    *count = num;
    return NOERROR;
}

ECode CCapsuleManagerService::SetApplicationEnabledSetting(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    return SetEnabledSetting(capsuleName, String(NULL), newState, flags);
}

ECode CCapsuleManagerService::SetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    VALIDATE_NOT_NULL(componentName);

    String capName, clsName;
    componentName->GetCapsuleName(&capName);
    componentName->GetClassName(&clsName);
    return SetEnabledSetting(capName, clsName, newState, flags);
}

ECode CCapsuleManagerService::SetEnabledSetting(
    /* [in] */ const String& capsuleName,
    /* [in] */ const String& className,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 flags)
{
    if (!(newState == CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT
          || newState == CapsuleManager_COMPONENT_ENABLED_STATE_ENABLED
          || newState == CapsuleManager_COMPONENT_ENABLED_STATE_DISABLED)) {
        Logger::E(TAG, StringBuffer("Invalid new component state: ") + newState);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    CapsuleSetting* capSetting = NULL;
    const Int32 uid = -1 /*Binder::GetCallingUid()*/;
    Int32 permission = 0;
    mContext->CheckCallingPermission(
        String("") /*android.Manifest.permission.CHANGE_COMPONENT_ENABLED_STATE*/,
        &permission);
    const Boolean allowedByPermission = (permission == CapsuleManager_PERMISSION_GRANTED);
    Boolean sendNow = FALSE;
    Boolean isApp = (className.IsNull());
    String componentName = isApp ? capsuleName : className;
    Int32 capsuleUid = -1;
    //todo: when to delete components
    List<String>* components = NULL;
    {
        Mutex::Autolock lock(mCapsulesLock);

        HashMap<String, AutoPtr<CapsuleSetting> >::Iterator csIt =
                mSettings->mCapsules.Find(capsuleName);
        if (csIt != mSettings->mCapsules.End()) {
                capSetting = csIt->mSecond;
        }
        if (capSetting == NULL) {
            if (className.IsNull()) {
                Logger::E(TAG, StringBuffer("Unknown capsule: ") + capsuleName);
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            Logger::E(TAG, StringBuffer("Unknown component: ") + capsuleName + "/" + className);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (!allowedByPermission && (uid != capSetting->mUserId)) {
            Logger::E(TAG, StringBuffer("Permission Denial: attempt to change component state from pid=")
                + -1 /*Binder::GetCallingPid()*/
                + ", uid=" + uid + ", capsule uid=" + capSetting->mUserId);
            return E_SECURITY_EXCEPTION;
        }
        if (className.IsNull()) {
            // We're dealing with an application/capsule level state change
            if (capSetting->mEnabled == newState) {
                // Nothing to do
                return NOERROR;
            }
            capSetting->mEnabled = newState;
            capSetting->mCap->mSetEnabled = newState;
        }
        else {
            // We're dealing with a component level state change
            switch (newState) {
            case CapsuleManager_COMPONENT_ENABLED_STATE_ENABLED:
                if (!capSetting->EnableComponentLP(className)) {
                    return NOERROR;
                }
                break;
            case CapsuleManager_COMPONENT_ENABLED_STATE_DISABLED:
                if (!capSetting->DisableComponentLP(className)) {
                    return NOERROR;
                }
                break;
            case CapsuleManager_COMPONENT_ENABLED_STATE_DEFAULT:
                if (!capSetting->RestoreComponentLP(className)) {
                    return NOERROR;
                }
                break;
            default:
                Slogger::E(TAG, StringBuffer("Invalid new component state: ") + newState);
                return NOERROR;
            }
        }
        mSettings->WriteLP();
        capsuleUid = capSetting->mUserId;
        HashMap< String, List<String>* >::Iterator pbIt =
                mPendingBroadcasts.Find(capsuleName);
        if (pbIt != mPendingBroadcasts.End()) {
            components = pbIt->mSecond;
        }
        Boolean newCapsule = components == NULL;
        if (newCapsule) {
            components = new List<String>();
        }
        if (Find(components->Begin(), components->End(), componentName)
            == components->End()) {
            components->PushBack(componentName);
        }
        if ((flags & CapsuleManager::DONT_KILL_APP) == 0) {
            sendNow = TRUE;
            // Purge entry from pending broadcast list if another one exists already
            // since we are sending one right away->
            mPendingBroadcasts.Erase(capsuleName);
        }
        else {
            if (newCapsule) {
                mPendingBroadcasts[capsuleName] = components;
            }
//	            if (!mHandler->HasMessages(SEND_PENDING_BROADCAST)) {
//	                // Schedule a message
//	                mHandler->SendEmptyMessageDelayed(SEND_PENDING_BROADCAST, BROADCAST_DELAY);
//	            }
        }
    }

    Int64 callingId = -1 /*Binder::ClearCallingIdentity()*/;
    if (sendNow) {
        SendCapsuleChangedBroadcast(capsuleName,
            (flags & CapsuleManager::DONT_KILL_APP) != 0, *components, capsuleUid);
    }
//	    Binder::RestoreCallingIdentity(callingId);
    return NOERROR;
}

void CCapsuleManagerService::SendCapsuleChangedBroadcast(
    /* [in] */ const String& capsuleName,
    /* [in] */ Boolean killFlag,
    /* [in] */ const List<String>& componentNames,
    /* [in] */ Int32 capsuleUid)
{
    if (FALSE) {
        Logger::V(TAG, StringBuffer("Sending capsule changed: capsule=")
            + capsuleName + " components=" /*+ componentNames*/);
    }
    AutoPtr<IBundle> extras; // = new Bundle(4);
    ASSERT_SUCCEEDED(CBundle::New((IBundle**)&extras));
    extras->PutString(String(Intent_EXTRA_CHANGED_COMPONENT_NAME), componentNames.GetFront());
    //todo: when to delete nameList
    ArrayOf<String>* nameList = ArrayOf<String>::Alloc(componentNames.GetSize());
    assert(nameList != NULL);
    Int32 i = 0;
    List<String>::ConstIterator it;
    for (it = componentNames.Begin(); it != componentNames.End(); ++it) {
        (*nameList)[i++] = *it;
    }
    extras->PutStringArray(String(Intent_EXTRA_CHANGED_COMPONENT_NAME_LIST), nameList);
    extras->PutBoolean(String(Intent_EXTRA_DONT_KILL_APP), killFlag);
    extras->PutInt32(String(Intent_EXTRA_UID), capsuleUid);
    SendCapsuleBroadcast(String(Intent_ACTION_CAPSULE_CHANGED), capsuleName, extras, NULL);
}

ECode CCapsuleManagerService::GetInstallerCapsuleName(
    /* [in] */ const String& capsuleName,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    Mutex::Autolock Lock(mCapsulesLock);

    CapsuleSetting* cap = NULL;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it =
            mSettings->mCapsules.Find(capsuleName);
    if (it != mSettings->mCapsules.End()) {
        cap = it->mSecond;
    }
    if (cap == NULL) {
        Logger::E(TAG, StringBuffer("Unknown capsule: ") + capsuleName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *name = cap->mInstallerCapsuleName;
    return NOERROR;
}

ECode CCapsuleManagerService::GetApplicationEnabledSetting(
    /* [in] */ const String& capsuleName,
    /* [out] */ Int32* setting)
{
    VALIDATE_NOT_NULL(setting);

    Mutex::Autolock Lock(mCapsulesLock);

    CapsuleSetting* cap = NULL;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it =
            mSettings->mCapsules.Find(capsuleName);
    if (it != mSettings->mCapsules.End()) {
        cap = it->mSecond;
    }
    if (cap == NULL) {
        Logger::E(TAG, StringBuffer("Unknown capsule: ") + capsuleName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *setting = cap->mEnabled;
    return NOERROR;
}

ECode CCapsuleManagerService::GetComponentEnabledSetting(
    /* [in] */ IComponentName* componentName,
    /* [out] */ Int32* setting)
{
    VALIDATE_NOT_NULL(componentName);
    VALIDATE_NOT_NULL(setting);

    Mutex::Autolock Lock(mCapsulesLock);

    String capsuleNameStr;
    componentName->GetCapsuleName(&capsuleNameStr);
    CapsuleSetting* cap = NULL;
    HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it =
            mSettings->mCapsules.Find(capsuleNameStr);
    if (it != mSettings->mCapsules.End()) {
        cap = it->mSecond;
    }
    if (cap == NULL) {
        Logger::E(TAG, StringBuffer("Unknown component: ") + componentName);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String classNameStr;
    componentName->GetClassName(&classNameStr);
    return cap->CurrentEnabledStateLP(classNameStr);
}

ECode CCapsuleManagerService::EnterSafeMode()
{
    if (!mSystemReady) {
        mSafeMode = TRUE;
    }
    return NOERROR;
}

ECode CCapsuleManagerService::SystemReady()
{
    mSystemReady = TRUE;

    // Read the compatibilty setting when the system is ready->
    Boolean compatibilityModeEnabled = FALSE;
//	        android.provider.Settings.System.GetInt(
//	            mContext->GetContentResolver(),
//	            android.provider.Settings.System.COMPATIBILITY_MODE, 1) == 1;
    CapsuleParser::SetCompatibilityModeEnabled(compatibilityModeEnabled);
    if (DEBUG_SETTINGS) {
        Logger::D(TAG, StringBuffer("compatibility mode:") + compatibilityModeEnabled);
    }
    return NOERROR;
}

ECode CCapsuleManagerService::IsSafeMode(
    /* [out] */ Boolean* isSafeMode)
{
    VALIDATE_NOT_NULL(isSafeMode);
    *isSafeMode = mSafeMode;
    return NOERROR;
}

ECode CCapsuleManagerService::HasSystemUidErrors(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHasSystemUidErrors;
    return NOERROR;
}

String CCapsuleManagerService::GetEncryptKey()
{
//    try {
//        String sdEncKey = SystemKeyStore.getInstance().retrieveKeyHexString(
//                SD_ENCRYPTION_KEYSTORE_NAME);
//        if (sdEncKey == null) {
//            sdEncKey = SystemKeyStore.getInstance().generateNewKeyHexString(128,
//                    SD_ENCRYPTION_ALGORITHM, SD_ENCRYPTION_KEYSTORE_NAME);
//            if (sdEncKey == null) {
//                Slog.e(TAG, "Failed to create encryption keys");
//                return null;
//            }
//        }
//        return sdEncKey;
//    } catch (NoSuchAlgorithmException nsae) {
//        Slog.e(TAG, "Failed to create encryption keys with exception: " + nsae);
//        return null;
//    } catch (IOException ioe) {
//        Slog.e(TAG, "Failed to retrieve encryption keys with exception: "
//                  + ioe);
//        return null;
//    }
    return String(NULL);
}

String CCapsuleManagerService::GetTempContainerId()
{
//    int tmpIdx = 1;
//    String list[] = PackageHelper.getSecureContainerList();
//    if (list != null) {
//        for (final String name : list) {
//            // Ignore null and non-temporary container entries
//            if (name == null || !name.startsWith(mTempContainerPrefix)) {
//                continue;
//            }
//
//            String subStr = name.substring(mTempContainerPrefix.length());
//            try {
//                int cid = Integer.parseInt(subStr);
//                if (cid >= tmpIdx) {
//                    tmpIdx = cid + 1;
//                }
//            } catch (NumberFormatException e) {
//            }
//        }
//    }
//    return mTempContainerPrefix + tmpIdx;
    return String(NULL);
}

/*
 * Update media status on PackageManager.
 */
ECode CCapsuleManagerService::UpdateExternalMediaStatus(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ Boolean reportStatus)
{
//	    if (Binder::GetCallingUid() != Process::SYSTEM_UID) {
//	       throw new SecurityException("Media status can only be updated by the system");
//	    }
    {
        Mutex::Autolock lock(mCapsulesLock);

        Logger::I(TAG, StringBuffer("Updating external media status from ") +
               (mMediaMounted ? "mounted" : "unmounted") + " to " +
               (mediaStatus ? "mounted" : "unmounted"));
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, StringBuffer("updateExternalMediaStatus:: mediaStatus=")
                + mediaStatus + ", mMediaMounted=" + mMediaMounted);
        }
        if (mediaStatus == mMediaMounted) {
            void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Boolean, Set<SdInstallArgs*>*);
            pHandlerFunc = &CCapsuleManagerService::HandleUpdatedMediaStatus;

            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteBoolean(reportStatus);
            params->WriteInt32(0);

            SendMessage(*(Handle32*)&pHandlerFunc, params);
            return NOERROR;
        }
        mMediaMounted = mediaStatus;
    }
    // Queue up an async operation since the capsule installation may take a little while->
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Boolean, Boolean);
    pHandlerFunc = &CCapsuleManagerService::HandleUpdateExternalMediaStatus;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteBoolean(mediaStatus);
    params->WriteBoolean(reportStatus);
    return mApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CCapsuleManagerService::HandleUpdateExternalMediaStatus(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ Boolean reportStatus)
{
//    mHandler.removeCallbacks(this);

    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Boolean, Boolean);
    pHandlerFunc = &CCapsuleManagerService::HandleUpdateExternalMediaStatus;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    UpdateExternalMediaStatusInner(mediaStatus, reportStatus);
}

/*
 * Collect information of applications on external media, map them
 * against existing containers and update information based on current
 * mount status. Please note that we always have to report status
 * if reportStatus has been set to true especially when unloading packages.
 */
ECode CCapsuleManagerService::UpdateExternalMediaStatusInner(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ Boolean reportStatus)
{
    // Collection of uids
    AutoFree< ArrayOf<Int32> > uidArr;
    // Collection of stale containers
//    HashSet<String> removeCids = new HashSet<String>();
    List<String> removeCids;
    // Collection of capsules on external media with valid containers->
//    HashMap<SdInstallArgs, String> processCids = new HashMap<SdInstallArgs, String>();
    HashMap<SdInstallArgs*, String, HashSd, SdEq> processCids;
    // Get list of secure containers->
    AutoStringArray list;
    FAIL_RETURN(CapsuleHelper::GetSecureContainerList((ArrayOf<String>**)&list));
    if (list == NULL || list->GetLength() == 0) {
        Logger::I(TAG, "No secure containers on sdcard");
    }
    else {
        // Process list of secure containers and categorize them
        // as active or stale based on their capsule internal state->
        AutoFree< ArrayOf<Int32> > uidList = ArrayOf<Int32>::Alloc(list->GetLength());
        if (uidList == NULL) return E_OUT_OF_MEMORY_ERROR;
        Int32 num = 0;
        {
            Mutex::Autolock lock(mCapsulesLock);

            for (Int32 i = 0; i < list->GetLength(); i++) {
                String cid = (*list)[i];
                SdInstallArgs* args = new SdInstallArgs(cid, this);
                if (!args) return E_OUT_OF_MEMORY_ERROR;
                if (DEBUG_SD_INSTALL) {
                    Logger::I(TAG, StringBuffer("Processing container ") + cid);
                }
                String capName = args->GetCapsuleName();
                if (capName.IsNull()) {
                    if (DEBUG_SD_INSTALL) {
                        Logger::I(TAG, StringBuffer("Container : ") + cid + " stale");
                    }
                    removeCids.PushBack(cid);
                    continue;
                }
                if (DEBUG_SD_INSTALL) {
                    Logger::I(TAG, StringBuffer("Looking for cap : ") + capName);
                }
                CapsuleSetting* cs = NULL;
                HashMap<String, AutoPtr<CapsuleSetting> >::Iterator it
                        = mSettings->mCapsules.Find(capName);
                if (it != mSettings->mCapsules.End()) {
                    cs = it->mSecond;
                }
                // The capsule status is changed only if the code path
                // matches between settings and the container id.
                String path = args->GetCodePath();
                if (cs != NULL && !cs->mCodePathString.IsNull()
                    && cs->mCodePathString.Equals(path)) {
                    if (DEBUG_SD_INSTALL) {
                        Logger::I(TAG, StringBuffer("Container : ")
                            + cid + " corresponds to cap : " + capName
                            + " at code path: " + cs->mCodePathString);
                    }
                    // We do have a valid capsule installed on sdcard
//                    processCids->Put(args, cs->mCodePathString);
                    processCids[args] = cs->mCodePathString;
                    Int32 uid = cs->mUserId;
                    if (uid != -1) {
                        (*uidList)[num++] = uid;
                    }
                }
                else {
                    // Stale container on sdcard-> Just delete
                    if (DEBUG_SD_INSTALL) {
                        Logger::I(TAG, StringBuffer("Container : ") + cid + " stale");
                    }
                    removeCids.PushBack(cid);
                }
            }
        }

        if (num > 0) {
            // Sort uid list
//	            Arrays::Sort(uidList, 0, num);
            // Throw away duplicates
            uidArr = ArrayOf<Int32>::Alloc(num);
            if (uidArr == NULL) return E_OUT_OF_MEMORY_ERROR;
            (*uidArr)[0] = (*uidList)[0];
            Int32 di = 0;
            for (Int32 i = 1; i < num; i++) {
                if ((*uidList)[i-1] != (*uidList)[i]) {
                    (*uidArr)[di++] = (*uidList)[i];
                }
            }
        }
    }

    // Process capsules with valid entries->
    if (mediaStatus) {
        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Loading capsules");
        LoadMediaCapsules(processCids, *uidArr, &removeCids);
        StartCleaningCapsules();
    }
    else {
        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Unloading capsules");
        UnloadMediaCapsules(processCids, *uidArr, reportStatus);
    }

    return NOERROR;
}

void CCapsuleManagerService::SendResourcesChangedBroadcast(
    /* [in] */ Boolean mediaStatus,
    /* [in] */ const List<String>& capList,
    /* [in] */ const ArrayOf<Int32>* uidArr,
    /* [in] */ IIntentReceiver* finishedReceiver)
{
    Int32 size = capList.GetSize();
    if (size > 0) {
        AutoStringArray caps = ArrayOf<String>::Alloc(size);
        assert(caps != NULL);
        Int32 i = 0;
        List<String>::ConstIterator it;
        for (it = capList.Begin(); it != capList.End(); ++it) {
            (*caps)[i++] = *it;
        }
        // Send broadcasts here
        AutoPtr<IBundle> extras; // = new Bundle();
        ASSERT_SUCCEEDED(CBundle::New((IBundle**)&extras));
        // TODO: ALEX
        ArrayOf<String>* newCaps = caps.Get();
        assert(newCaps);
        newCaps = newCaps->Clone();
        assert(newCaps);
        extras->PutStringArray(String(Intent_EXTRA_CHANGED_CAPSULE_LIST), newCaps);
        if (uidArr != NULL) {
            // TODO: ALEX
            ArrayOf<Int32>* newUidArr = uidArr->Clone();
            assert(newUidArr);
            extras->PutInt32Array(String(Intent_EXTRA_CHANGED_UID_LIST), newUidArr);
        }
        String action = mediaStatus
            ? String(Intent_ACTION_EXTERNAL_APPLICATIONS_AVAILABLE)
            : String(Intent_ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE);
        SendCapsuleBroadcast(action, String(NULL), extras, finishedReceiver);
    }
}

/*
 * Look at potentially valid container ids from processCids
 * If package information doesn't match the one on record
 * or package scanning fails, the cid is added to list of
 * removeCids. We currently don't delete stale containers.
 */
void CCapsuleManagerService::LoadMediaCapsules(
    /* [in] */ HashMap<SdInstallArgs*, String, HashSd, SdEq>& processCids,
    /* [in] */ const ArrayOf<Int32>& uidArr,
    /* [in] */ List<String>* removeCids)
{
    List<String> capList;
    Boolean doGc = FALSE;
    HashMap<SdInstallArgs*, String, HashSd, SdEq>::Iterator it;
    for (it = processCids.Begin(); it != processCids.End(); ++it) {
        SdInstallArgs* args = it->mFirst;
        String codePath = it->mSecond;
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, StringBuffer("Loading container : ") + args->mCid);
        }
        Int32 retCode = CapsuleManager::INSTALL_FAILED_CONTAINER_ERROR;
        // Make sure there are no container errors first.
        if (args->DoPreInstall(CapsuleManager::INSTALL_SUCCEEDED)
            != CapsuleManager::INSTALL_SUCCEEDED) {
            Slogger::E(TAG, StringBuffer("Failed to mount cid : ")
                + args->mCid + " when installing from sdcard");
           continue;
        }
        // Check code path here.
        if (codePath.IsNull() || !codePath.Equals(args->GetCodePath())) {
            Slogger::E(TAG, StringBuffer("Container ")
                + args->mCid + " cachepath " + args->GetCodePath()
                + " does not match one in settings " + codePath);
           continue;
        }
        // Parse capsule
        Int32 parseFlags = CapsuleParser::PARSE_ON_SDCARD | mDefParseFlags;
        doGc = TRUE;
        {
            Mutex::Autolock lock(mInstallLock);

            AutoPtr<IFile> file;
            ASSERT_SUCCEEDED(CFile::New(codePath, (IFile**)&file));
            CapsuleParser::Capsule* cap = ScanCapsuleLI(file, parseFlags, 0, 0);
            // Scan the capsule
            if (cap != NULL) {
                Mutex::Autolock l(mCapsulesLock);

                retCode = CapsuleManager::INSTALL_SUCCEEDED;
                capList.PushBack(cap->mCapsuleName);
                // Post process args
                args->DoPostInstall(CapsuleManager::INSTALL_SUCCEEDED);
            }
            else {
                Slogger::I(TAG, StringBuffer("Failed to install cap from  ")
                    + codePath + " from sdcard");
            }
        }
        if (retCode != CapsuleManager::INSTALL_SUCCEEDED) {
            // Don't destroy container here-> Wait till gc clears things up.
            removeCids->PushBack(args->mCid);
        }
    }
    {
        Mutex::Autolock lock(mCapsulesLock);

        // If the platform SDK has changed since the last time we booted,
        // we need to re-grant app permission to catch any new ones that
        // appear->  This is really a hack, and means that apps can in some
        // cases get permissions that the user didn't initially explicitly
        // allow->->->  it would be nice to have some better way to handle
        // this situation->
        Boolean regrantPermissions = mSettings->mExternalSdkPlatform != mSdkVersion;
        if (regrantPermissions) {
            Slogger::I(TAG, StringBuffer("Platform changed from ")
                + mSettings->mExternalSdkPlatform + " to " + mSdkVersion
                + "; regranting permissions for external storage");
        }
        mSettings->mExternalSdkPlatform = mSdkVersion;

        // Make sure group IDs have been assigned, and any permission
        // changes in other apps are accounted for
        UpdatePermissionsLP(String(NULL), NULL, TRUE, regrantPermissions, regrantPermissions);
        // Persist settings
        mSettings->WriteLP();
    }
    // Send a broadcast to let everyone know we are done processing
    if (capList.GetSize() > 0) {
       SendResourcesChangedBroadcast(TRUE, capList, &uidArr, NULL);
    }
    // List stale containers and destroy stale temporary containers->
    if (removeCids != NULL) {
        List<String>::Iterator rcIt;
        for (rcIt = removeCids->Begin(); rcIt != removeCids->End(); ++rcIt) {
            String cid = *rcIt;
            if (cid.StartWith(mTempContainerPrefix)) {
                 Logger::I(TAG, StringBuffer("Destroying stale temporary container ") + cid);
                 CapsuleHelper::DestroySdDir(cid);
            }
            else {
                 Logger::W(TAG, StringBuffer("Container ") + cid + " is stale");
            }
        }
    }
}

void CCapsuleManagerService::UnloadAllContainers(
    /* [in] */ Set<SdInstallArgs*>& cidArgs)
{
    // Just unmount all valid containers.
    Set<SdInstallArgs*>::Iterator it;
    for (it = cidArgs.Begin(); it != cidArgs.End(); ++it) {
        SdInstallArgs* arg = *it;
        Mutex::Autolock lock(mInstallLock);
        arg->DoPostDeleteLI(FALSE);
    }
}

/*
 * Unload packages mounted on external media. This involves deleting
 * package data from internal structures, sending broadcasts about
 * diabled packages, gc'ing to free up references, unmounting all
 * secure containers corresponding to packages on external media, and
 * posting a UPDATED_MEDIA_STATUS message if status has been requested.
 * Please note that we always have to post this message if status has
 * been requested no matter what.
 */
ECode CCapsuleManagerService::UnloadMediaCapsules(
    /* [in] */ HashMap<SdInstallArgs*, String, HashSd, SdEq>& processCids,
    /* [in] */ const ArrayOf<Int32>& uidArr,
    /* [in] */ Boolean reportStatus)
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "unloading media capsules");
    List<String> capList;
    List<SdInstallArgs*> failedList;
    //todo: when to delete keys;
    Set<SdInstallArgs*>* keys = new Set<SdInstallArgs*>();
    HashMap<SdInstallArgs*, String, HashSd, SdEq>::Iterator it;
    for (it = processCids.Begin(); it != processCids.End(); ++it) {
        SdInstallArgs* args = it->mFirst;
        keys->Insert(args);
        String cid = args->mCid;
        String capName = args->GetCapsuleName();
        if (DEBUG_SD_INSTALL) {
            Logger::I(TAG, StringBuffer("Trying to unload cap : ") + capName);
        }
        // Delete capsule internally
        CapsuleRemovedInfo* outInfo = new CapsuleRemovedInfo(this);
        assert(outInfo);
        {
            Mutex::Autolock lock(mInstallLock);
            Boolean res = DeleteCapsuleLI(
                capName, FALSE, CapsuleManager_DONT_DELETE_DATA, outInfo, FALSE);
            if (res) {
                capList.PushBack(capName);
            }
            else {
                Slogger::E(TAG,
                    StringBuffer("Failed to delete cap from sdcard : ")
                    + capName);
                failedList.PushBack(args);
            }
        }
    }

    {
        Mutex::Autolock lock(mCapsulesLock);
        // We didn't update the settings after removing each capsule;
        // write them now for all capsules.
        mSettings->WriteLP();
    }

    // We have to absolutely send UPDATED_MEDIA_STATUS only
    // after confirming that all the receivers processed the ordered
    // broadcast when capsules get disabled, force a gc to clean things up.
    // and unload all the containers.
    if (capList.GetSize() > 0) {
        SendResourcesChangedBroadcast(FALSE, capList, &uidArr, NULL /*new IIntentReceiver::Stub() {
            public void PerformReceive(Intent intent, Int32 resultCode, String data, Bundle extras,
                    Boolean ordered, Boolean sticky) throws RemoteException {
                Message msg = mHandler->ObtainMessage(UPDATED_MEDIA_STATUS,
                        reportStatus ? 1 : 0, 1, keys);
                mHandler->SendMessage(msg);
            }
        }*/);
    }
    else {
        void (STDCALL CCapsuleManagerService::*pHandlerFunc)(Boolean, Set<SdInstallArgs*>*);
        pHandlerFunc = &CCapsuleManagerService::HandleUpdatedMediaStatus;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteBoolean(reportStatus);
        params->WriteInt32((Handle32)keys);

        SendMessage(*(Handle32*)&pHandlerFunc, params);
    }
    return NOERROR;
}

ECode CCapsuleManagerService::MoveCapsule(
    /* [in] */ const String& capsuleName,
    /* [in] */ ICapsuleMoveObserver* observer,
    /* [in] */ Int32 flags)
{
    mContext->EnforceCallingOrSelfPermission(
           String("") /*android.Manifest.permission.MOVE_CAPSULE*/, String(NULL));
    Int32 returnCode = CapsuleManager_MOVE_SUCCEEDED;
    Int32 currFlags = 0;
    Int32 newFlags = 0;
    {
        Mutex::Autolock lock(mCapsulesLock);

        CapsuleParser::Capsule* cap = NULL;
        HashMap<String, CapsuleParser::Capsule*>::Iterator it = mCapsules.Find(capsuleName);
        if (it != mCapsules.End()) {
            cap = it->mSecond;
        }
        if (cap == NULL) {
            returnCode =  CapsuleManager_MOVE_FAILED_DOESNT_EXIST;
        }
        else {
            // Disable moving fwd locked apps and system capsules
            if (cap->mApplicationInfo != NULL && IsSystemApp(cap)) {
                Slogger::W(TAG, "Cannot move system application");
                returnCode = CapsuleManager_MOVE_FAILED_SYSTEM_CAPSULE;
            }
            else if (cap->mApplicationInfo != NULL && IsForwardLocked(cap)) {
                Slogger::W(TAG, "Cannot move forward locked app->");
                returnCode = CapsuleManager_MOVE_FAILED_FORWARD_LOCKED;
            }
            else if (cap->mOperationPending) {
                Slogger::W(TAG, "Attempt to move capsule which has pending operations");
                returnCode = CapsuleManager_MOVE_FAILED_OPERATION_PENDING;
            }
            else {
                // Find install location first
                if ((flags & CapsuleManager_MOVE_EXTERNAL_MEDIA) != 0
                    && (flags & CapsuleManager_MOVE_INTERNAL) != 0) {
                    Slogger::W(TAG, "Ambigous flags specified for move location.");
                    returnCode = CapsuleManager_MOVE_FAILED_INVALID_LOCATION;
                }
                else {
                    newFlags = (flags & CapsuleManager_MOVE_EXTERNAL_MEDIA) != 0
                        ? CapsuleManager::INSTALL_EXTERNAL
                        : CapsuleManager::INSTALL_INTERNAL;
                    currFlags = IsExternal(cap)
                        ? CapsuleManager::INSTALL_EXTERNAL
                        : CapsuleManager::INSTALL_INTERNAL;
                    if (newFlags == currFlags) {
                        Slogger::W(TAG, "No move required-> Trying to move to same location");
                        returnCode = CapsuleManager_MOVE_FAILED_INVALID_LOCATION;
                    }
                }
                if (returnCode == CapsuleManager_MOVE_SUCCEEDED) {
                    cap->mOperationPending = TRUE;
                }
            }
        }
        if (returnCode != CapsuleManager_MOVE_SUCCEEDED) {
            //todo: when to delete params;
            MoveParams* params = new MoveParams(
                NULL, observer, 0, capsuleName, String(NULL), this);
            ProcessPendingMove(params, returnCode);
        }
        else {
            void (STDCALL CCapsuleManagerService::*pHandlerFunc)(HandlerParams*);
            pHandlerFunc = &CCapsuleManagerService::HandleInitCopy;

            String capSrcDir, capPubSrcDir, capDataDir, capNatLibDir;
            cap->mApplicationInfo->GetSourceDir(&capSrcDir);
            cap->mApplicationInfo->GetPublicSourceDir(&capPubSrcDir);
            cap->mApplicationInfo->GetDataDir(&capDataDir);
            cap->mApplicationInfo->GetNativeLibraryDir(&capNatLibDir);
            InstallArgs* srcArgs = CreateInstallArgs(currFlags,
                capSrcDir, capPubSrcDir, capNatLibDir);
            MoveParams* mp = new MoveParams(
                srcArgs, observer, newFlags, capsuleName,
                capDataDir, this);

            AutoPtr<IParcel> params;
            CCallbackParcel::New((IParcel**)&params);
            params->WriteInt32((Handle32)mp);

            SendMessage(*(Handle32*)&pHandlerFunc, params);
        }
    }
    return NOERROR;
}

void CCapsuleManagerService::ProcessPendingMove(
    /* [in] */ MoveParams* mp,
    /* [in] */ Int32 currentStatus)
{
    // Queue up an async operation since the package deletion may take a little while.
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(MoveParams*, Int32);
    pHandlerFunc = &CCapsuleManagerService::HandleProcessPendingMove;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)mp);
    params->WriteInt32(currentStatus);
    mApartment->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

void CCapsuleManagerService::HandleProcessPendingMove(
    /* [in] */ MoveParams* mp,
    /* [in] */ Int32 currentStatus)
{
//    mHandler.removeCallbacks(this);
    void (STDCALL CCapsuleManagerService::*pHandlerFunc)(MoveParams*, Int32);
    pHandlerFunc = &CCapsuleManagerService::HandleProcessPendingMove;
    mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);

    Int32 returnCode = currentStatus;
    if (currentStatus == CapsuleManager_MOVE_SUCCEEDED) {
        AutoFree< ArrayOf<Int32> > uidArr;
        List<String>* capList = NULL;
        {
            Mutex::Autolock lock(mCapsulesLock);

            CapsuleParser::Capsule* cap = NULL;
            HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                    mCapsules.Find(mp->mCapsuleName);
            if (it != mCapsules.End()) {
                cap = it->mSecond;
            }
            if (cap == NULL) {
                Slogger::W(TAG, StringBuffer(" Capsule ")
                    + mp->mCapsuleName + " doesn't exist-> Aborting move");
                returnCode = CapsuleManager_MOVE_FAILED_DOESNT_EXIST;
            }
            else {
                String capSrcDir;
                cap->mApplicationInfo->GetSourceDir(&capSrcDir);
                if (!mp->mSrcArgs->GetCodePath().Equals(capSrcDir)) {
                    Slogger::W(TAG, StringBuffer("Capsule ")
                        + mp->mCapsuleName + " code path changed from "
                        + mp->mSrcArgs->GetCodePath() + " to "
                        + capSrcDir + " Aborting move and returning error");
                    returnCode = CapsuleManager_MOVE_FAILED_INTERNAL_ERROR;
                }
                else {
                    uidArr = ArrayOf<Int32>::Alloc(1);
                    cap->mApplicationInfo->GetUid(&(*uidArr)[0]);
                    //todo: when to delete capList;
                    capList = new List<String>();
                    capList->PushBack(mp->mCapsuleName);
                }
            }
        }
        if (returnCode == CapsuleManager_MOVE_SUCCEEDED) {
            // Send resources unavailable broadcast
            SendResourcesChangedBroadcast(FALSE, *capList, uidArr.Get(), NULL);
            // Update capsule code and resource paths
            {
                Mutex::Autolock installLock(mInstallLock);

                {
                    Mutex::Autolock installLock(mCapsulesLock);

                    CapsuleParser::Capsule* cap = NULL;
                    HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                            mCapsules.Find(mp->mCapsuleName);
                    if (it != mCapsules.End()) {
                        cap = it->mSecond;
                    }
                    // Recheck for capsule again.
                    if (cap == NULL) {
                         Slogger::W(TAG, StringBuffer(" Capsule ")
                            + mp->mCapsuleName + " doesn't exist-> Aborting move");
                         returnCode = CapsuleManager_MOVE_FAILED_DOESNT_EXIST;
                    }
                    else {
                        String capSrcDir;
                        cap->mApplicationInfo->GetSourceDir(&capSrcDir);
                        if (!mp->mSrcArgs->GetCodePath().Equals(capSrcDir)) {
                            Slogger::W(TAG, StringBuffer("Capsule ")
                                + mp->mCapsuleName + " code path changed from "
                                + mp->mSrcArgs->GetCodePath() + " to "
                                + capSrcDir + " Aborting move and returning error");
                            returnCode = CapsuleManager_MOVE_FAILED_INTERNAL_ERROR;
                        }
                        else {
                            String oldCodePath = cap->mPath;
                            String newCodePath = mp->mTargetArgs->GetCodePath();
                            String newResPath = mp->mTargetArgs->GetResourcePath();
                            String newNativePath = mp->mTargetArgs->GetNativeLibraryPath();

                            String capDataDir;
                            cap->mApplicationInfo->GetDataDir(&capDataDir);
                            if ((mp->mFlags & CapsuleManager::INSTALL_EXTERNAL) == 0) {
                                if (mInstaller->UnlinkNativeLibraryDirectory(capDataDir) < 0) {
                                    returnCode = CapsuleManager_MOVE_FAILED_INSUFFICIENT_STORAGE;
                                }
                                else {
                                    AutoPtr<IFile> codeFile, nativeFile;
                                    CFile::New(newCodePath, (IFile**)&codeFile);
                                    CFile::New(newNativePath, (IFile**)&nativeFile);
                                    NativeLibraryHelper::CopyNativeBinariesLI(codeFile, nativeFile);
                                }
                            }
                            else {
                                if (mInstaller->LinkNativeLibraryDirectory(capDataDir, newNativePath) < 0) {
                                    returnCode = CapsuleManager_MOVE_FAILED_INSUFFICIENT_STORAGE;
                                }
                            }

                            if (returnCode == CapsuleManager_MOVE_SUCCEEDED) {
                                cap->mPath = newCodePath;
                                // Move dex files around
    //	                            if (MoveDexFilesLI(cap) != CapsuleManager::INSTALL_SUCCEEDED) {
    //	                                // Moving of dex files failed-> Set
    //	                                // error code and abort move->
    //	                                cap->mPath = cap->mScanPath;
    //	                                returnCode = CapsuleManager::MOVE_FAILED_INSUFFICIENT_STORAGE;
    //	                            }
                            }

                            if (returnCode == CapsuleManager_MOVE_SUCCEEDED) {
                                cap->mScanPath = newCodePath;
                                cap->mApplicationInfo->SetSourceDir(newCodePath);
                                cap->mApplicationInfo->SetPublicSourceDir(newResPath);
                                cap->mApplicationInfo->SetNativeLibraryDir(newNativePath);
                                CapsuleSetting* ps = (CapsuleSetting*)cap->mExtras->Probe(EIID_CapsuleSetting);
                                ps->mCodePath = NULL;
                                CFile::New(newCodePath, (IFile**)&ps->mCodePath);
                                ps->mCodePath->GetPath(&ps->mCodePathString);
                                ps->mResourcePath = NULL;
                                CFile::New(newResPath, (IFile**)&ps->mResourcePath);
                                ps->mResourcePath->GetPath(&ps->mResourcePathString);
                                ps->mNativeLibraryPathString = newNativePath;
                                // Set the application info flag correctly.
                                Int32 capFlags;
                                cap->mApplicationInfo->GetFlags(&capFlags);
                                if ((mp->mFlags & CapsuleManager::INSTALL_EXTERNAL) != 0) {
                                    capFlags |= ApplicationInfo_FLAG_EXTERNAL_STORAGE;
                                    cap->mApplicationInfo->SetFlags(capFlags);
                                }
                                else {
                                    capFlags &= ~ApplicationInfo_FLAG_EXTERNAL_STORAGE;
                                    cap->mApplicationInfo->SetFlags(capFlags);
                                }
                                ps->SetFlags(capFlags);
                                mAppDirs.Erase(oldCodePath);
                                mAppDirs[newCodePath] = cap;
                                // Persist settings
                                mSettings->WriteLP();
                            }
                        }
                    }
                }
            }
            // Send resources available broadcast
            SendResourcesChangedBroadcast(TRUE, *capList, uidArr.Get(), NULL);
        }
    }
    if (returnCode != CapsuleManager_MOVE_SUCCEEDED){
        // Clean up failed installation
        if (mp->mTargetArgs != NULL) {
            mp->mTargetArgs->DoPostInstall(CapsuleManager::INSTALL_FAILED_INTERNAL_ERROR);
        }
    }
    else {
        // Delete older code
        Mutex::Autolock lock(mInstallLock);
        mp->mSrcArgs->DoPostDeleteLI(TRUE);
    }

    // Allow more operations on this file if we didn't fail because
    // an operation was already pending for this capsule->
    if (returnCode != CapsuleManager_MOVE_FAILED_OPERATION_PENDING) {
        Mutex::Autolock lock(mCapsulesLock);
        CapsuleParser::Capsule* cap = NULL;
        HashMap<String, CapsuleParser::Capsule*>::Iterator it =
                mCapsules.Find(mp->mCapsuleName);
        if (it !=  mCapsules.End()) {
            cap = it->mSecond;
        }
        if (cap != NULL) {
            cap->mOperationPending = FALSE;
        }
    }

    ICapsuleMoveObserver* observer = mp->mObserver;
    if (observer != NULL) {
       if (FAILED(observer->CapsuleMoved(mp->mCapsuleName, returnCode))) {
           Logger::I(TAG, "Observer no Int64er exists.");
       }
    }
}

ECode CCapsuleManagerService::SetInstallLocation(
    /* [in] */ Int32 loc,
    /* [out] */ Boolean* isSetted)
{
    VALIDATE_NOT_NULL(isSetted);

    mContext->EnforceCallingOrSelfPermission(
        String("") /*android.Manifest.permission.WRITE_SECURE_SETTINGS*/, String(NULL));
    Int32 location = 0;
    GetInstallLocation(&location);
    if (location == loc) {
        *isSetted = TRUE;
        return NOERROR;
    }
    if (loc == CapsuleHelper::APP_INSTALL_AUTO
        || loc == CapsuleHelper::APP_INSTALL_INTERNAL
        || loc == CapsuleHelper::APP_INSTALL_EXTERNAL) {
//	        android.provider.Settings.System.PutInt(mContext.GetContentResolver(),
//	                android.provider.Settings.Secure.DEFAULT_INSTALL_LOCATION, loc);
        *isSetted = TRUE;
        return NOERROR;
    }
    *isSetted = FALSE;
    return NOERROR;
}

ECode CCapsuleManagerService::GetInstallLocation(
    /* [out] */ Int32* loc)
{
//	    return android.provider.Settings.System.getInt(mContext.getContentResolver(),
//	        android.provider.Settings.Secure.DEFAULT_INSTALL_LOCATION, PackageHelper.APP_INSTALL_AUTO);
    return E_NOT_IMPLEMENTED;
}

//todo: this constructor should be removed.
ECode CCapsuleManagerService::constructor()
{
    FAIL_RETURN(CApartment::New(FALSE, (IApartment**)&mApartment));
    mApartment->Start(ApartmentAttr_New);
    mSettings = new Settings(this);
    assert(mSettings != NULL);

    return NOERROR;
}

void CCapsuleManagerService::ScanDir(
    /* [in] */ CString path)
{
    DIR* dir;
    struct dirent* dp;
    struct stat fs;
    char fname[512];
    CapsuleParser::Capsule* capsule;

    dir = opendir(path);
    if (dir == NULL) {
        if (DEBUG_INSTALL) {
            Slogger::E(TAG, "Fail to open directory");
        }
        return;
    }

    do {
        if ((dp = readdir(dir)) != NULL) {
            stat(dp->d_name, &fs);
            if (S_ISDIR(fs.st_mode)) {
                strcpy(fname, path);
                strcat(fname, "/");
                strcat(fname, dp->d_name);
                strcat(fname, "/data/");
                strcat(fname, (const char*)DEFAULT_RESOURCES_FILE_NAME);
                if (!access(fname, R_OK)) {
                    capsule = ScanCapsule(String(fname));
                }
            }
        }
    }while(dp != NULL);

    closedir(dir);
}

CapsuleParser::Capsule* CCapsuleManagerService::ScanCapsule(
    /* [in] */ const String& capfile)
{
    String dir;
    Int32 len;
    CapsuleParser p;
    String err;
    CapsuleParser::Capsule* capsule;

    len = capfile.GetLength();
    dir = capfile.Substring(0, len - strlen(DEFAULT_RESOURCES_FILE_NAME) - 1);
    capsule = new CapsuleParser::Capsule();
    assert(capsule != NULL);
    capsule->mApplicationInfo->SetSourceDir(dir);
    if (SUCCEEDED(p.ParseCapsule(capfile, capsule, &err))) {
        String capCName, capPName;
        capsule->mApplicationInfo->GetCapsuleName(&capCName);
        capsule->mApplicationInfo->GetProcessName(&capPName);
        mCapsules[capCName] = capsule;

#ifdef _HAS_PARSED_MORE // TODO: temporary comment.
        //ContentProvider
        List<CapsuleParser::ContentProvider*>::Iterator
                cit = capsule->mProviders.Begin();
        for (; cit != capsule->mProviders.End(); ++cit) {
            CapsuleParser::ContentProvider* p = *cit;
            p->mInfo->mProcessName = FixProcessName(
                    capsule->mApplicationInfo->mProcessName,
                    (*cit)->mInfo->mProcessName,
                    capsule->mApplicationInfo->mUid);
            String strKey = p->mInfo->mCapsuleName + "." + p->mInfo->mName;
            mProvidersByComponent[strKey] = p;
            p->mSyncable = p->mInfo->mIsSyncable;
            if (!p->mInfo->mAuthority.IsNull()) {
                String authority = p->mInfo->mAuthority;
                p->mInfo->mAuthority = NULL;
                StringTokenizer* names = new StringTokenizer(
                        authority, ";");
                Int32 j = -1;
                while(names->HasMoreTokens(), j++) {
                    String token = names->NextToken();
                    if (j == 1 && p->mSyncable) {
                        // We only want the first authority for a provider to possibly be
                        // syncable, so if we already added this provider using a different
                        // authority clear the syncable flag. We copy the provider before
                        // changing it because the mContentProviders object contains a reference
                        // to a provider that we don't want to change.
                        // Only do this for the second authority since the resulting provider
                        // object can be the same for all future authorities for this provider.
                        p = new CapsuleParser::ContentProvider(p);
                        p->mSyncable = FALSE;
                    }

                    if (mContentProviders[token] == NULL) {
                        mContentProviders[token] = p;
                        if (p->mInfo->mAuthority.IsNull()) {
                            p->mInfo->mAuthority = token;
                        }
                        else {
                            char* auth = (char*)malloc(
                                    p->mInfo->mAuthority.GetLength() + 1 /*";"*/ +
                                    token.GetLength() + 1);
                            strcpy(auth, (const char*)p->mInfo->mAuthority);
                            strcat(auth, ";");
                            strcat(auth, token);
                            String::Free(p->mInfo->mAuthority);
                            p->mInfo->mAuthority = String(auth);
                        }
                        if ((parseFlags & CapsuleParser::PARSE_CHATTY) != 0 && Config::LOGD) {
                            Slogger::D(TAG, StringBuffer("Registered content provider: ")
                                + names[j] + ", className = " + p->mInfo->mName
                                + ", isSyncable = " + p->mInfo->mIsSyncable);
                        }
                    }
//	                    else {
//	                        CapsuleParser::ContentProvider* other = mContentProviders[token];
//	                        UNUSED(other);
//	                        Slogger::W(TAG, StringBuffer("Skipping provider name ")
//	                            + names[j] + " (in capsule "
//	                            + cap->mApplicationInfo->capsuleName +
//	                                    "): name already used by "
//	                                    + ((other != NULL && other->GetComponentName() != NULL)
//	                                            ? other->GetComponentName()->GetCapsuleName() : "?"));
//	                    }
                }
            }
//            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//                if (r == null) {
//                    r = new StringBuilder(256);
//                } else {
//                    r.append(' ');
//                }
//                r.append(p.info.name);
//            }
        }
//        if (r != null) {
//            if (Config.LOGD) Slog.d(TAG, "  Providers: " + r);
//        }

        //BroadcastReceiver
        List<CapsuleParser::Activity*>::Iterator bit1 = capsule->mReceivers.Begin();
        List<CapsuleParser::Activity*>::Iterator bit2 = capsule->mReceivers.End();
//        r = null;
        for (; bit1 != bit2; ++bit1) {
            String actPName;
            (*bit1)->mInfo->GetProcessName(&actPName);
            (*bit1)->mInfo->SetProcessName(FixProcessName(
                    capPName, actPName, -1 /*capsule->mApplicationInfo->mUid*/));
            mReceivers->AddActivity(*bit1, "receiver");
//            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//                if (r == null) {
//                    r = new StringBuilder(256);
//                } else {
//                    r.append(' ');
//                }
//                r.append(a.info.name);
//            }
        }
//        if (r != null) {
//            if (Config.LOGD) Slog.d(TAG, "  Receivers: " + r);
//        }
#endif

        //Activity
        List<CapsuleParser::Activity*>::Iterator ait1 = capsule->mActivities.Begin();
        List<CapsuleParser::Activity*>::Iterator ait2 = capsule->mActivities.End();
//        r = null;
        for (; ait1 != ait2; ++ait1) {
            String actPName;
            (*ait1)->mInfo->GetProcessName(&actPName);
            (*ait1)->mInfo->SetProcessName(FixProcessName(
                    capPName, actPName, -1 /*capsule->mApplicationInfo->mUid*/));
            mActivities->AddActivity(*ait1, "activity");
//            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//                if (r == null) {
//                    r = new StringBuilder(256);
//                } else {
//                    r.append(' ');
//                }
//                r.append(a.info.name);
//            }
        }
//        if (r != null) {
//            if (Config.LOGD) Slog.d(TAG, "  Activities: " + r);
//        }

        //Service
        List<CapsuleParser::Service*>::Iterator sit1 = capsule->mServices.Begin();
        List<CapsuleParser::Service*>::Iterator sit2 = capsule->mServices.End();
//        r = null;
        for (; sit1 != sit2; ++sit1) {
            String serPName;
            (*sit1)->mInfo->GetProcessName(&serPName);
            (*sit1)->mInfo->SetProcessName(FixProcessName(
                    capPName, serPName, -1 /*capsule->mApplicationInfo->mUid*/));
            mServices->AddService(*sit1);
//            if ((parseFlags&PackageParser.PARSE_CHATTY) != 0) {
//                if (r == null) {
//                    r = new StringBuilder(256);
//                } else {
//                    r.append(' ');
//                }
//                r.append(a.info.name);
//            }
        }
//        if (r != null) {
//            if (Config.LOGD) Log.d(TAG, "  Receivers: " + r);
//        }

        return capsule;
    }

    if (DEBUG_INSTALL) {
        Slogger::E(TAG, err);
    }

    return NULL;
}

ECode CCapsuleManagerService::SendMessage(
    /* [in] */ Handle32 pvFunc,
    /* [in] */ IParcel* params)
{
    return mApartment->PostCppCallback((Handle32)this, pvFunc, params, 0);
}

Boolean CCapsuleManagerService::ConnectToService()
{
    if (DEBUG_SD_INSTALL) {
        Logger::I(TAG, StringBuffer("Trying to bind to") + " DefaultContainerService");
    }
    AutoPtr<IIntent> service;
    ECode ec = CIntent::New((IIntent**)&service);
    if (FAILED(ec)) return FALSE;
    service->SetComponent(CCapsuleManagerService::DEFAULT_CONTAINER_COMPONENT.Get());
    Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
    Boolean isSucceeded = FALSE;
    mContext->BindService(service.Get(),
        (IServiceConnection*)mDefContainerConn.Get(),
        Context_BIND_AUTO_CREATE,
        &isSucceeded);
    if (isSucceeded) {
        Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
        mBound = TRUE;
        return TRUE;
    }
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
    return FALSE;
}

void CCapsuleManagerService::DisconnectService()
{
    mContainerService = NULL;
    mBound = FALSE;
    Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
    mContext->UnbindService((IServiceConnection*)mDefContainerConn.Get());
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
}

void CCapsuleManagerService::HandleInitCopy(
    /* [in] */ HandlerParams* params)
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "init_copy");
    Int32 idx = mPendingInstalls.GetSize();
    if (DEBUG_SD_INSTALL) {
        Logger::I(TAG, StringBuffer("idx=") + idx);
    }
    // If a bind was already initiated we dont really
    // need to do anything. The pending install
    // will be processed later on.
    if (!mBound) {
        // If this is the only one pending we might
        // have to bind to the service again.
        if (!ConnectToService()) {
            Slogger::E(TAG, "Failed to bind to media container service");
            params->ServiceError();
            delete params;
            return;
        }
        else {
            // Once we bind to the service, the first
            // pending request will be processed.
            mPendingInstalls.PushBack(params);
        }
    }
    else {
        mPendingInstalls.PushBack(params);
        // Already bound to the service. Just make
        // sure we trigger off processing the first request.
        if (idx == 0) {
            void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
                /* [in] */ IMediaContainerService* mcs);
            pHandlerFunc = &CCapsuleManagerService::HandleMCSBound;
            SendMessage(*(Handle32*)&pHandlerFunc, NULL);
        }
    }
}

void CCapsuleManagerService::HandleMCSBound(
    /* [in] */ IMediaContainerService* mcs)
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "mcs_bound");
    if (mcs != NULL) {
        mContainerService = mcs;
    }
    if (mContainerService == NULL) {
        // Something seriously wrong. Bail out
        Slogger::E(TAG, "Cannot bind to media container service");
        List< AutoPtr<HandlerParams> >::Iterator it;
        for (it = mPendingInstalls.Begin();
             it != mPendingInstalls.End(); ++it) {
            // Indicate service bind error
            (*it)->ServiceError();
        }
        mPendingInstalls.Clear();
    }
    else if (mPendingInstalls.Begin() != mPendingInstalls.End()) {
        HandlerParams* params = mPendingInstalls.GetFront();
        if (params != NULL) {
            params->StartCopy();
        }
    }
    else {
        // Should never happen ideally.
        Slogger::W(TAG, "Empty queue");
    }
}

void CCapsuleManagerService::HandleMCSReconnect()
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "mcs_reconnect");
    if (mPendingInstalls.Begin() != mPendingInstalls.End()) {
        if (mBound) {
            DisconnectService();
        }
        if (!ConnectToService()) {
            Slogger::E(TAG, "Failed to bind to media container service");
            List< AutoPtr<HandlerParams> >::Iterator it;
            for (it = mPendingInstalls.Begin();
                 it != mPendingInstalls.End(); ++it) {
                // Indicate service bind error
                (*it)->ServiceError();
            }
            mPendingInstalls.Clear();
        }
    }
}

void CCapsuleManagerService::HandleMCSUnbind()
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "mcs_unbind");
    // Delete pending install
    if (mPendingInstalls.Begin() != mPendingInstalls.End()) {
        mPendingInstalls.PopFront();
    }
    if (mPendingInstalls.Begin() == mPendingInstalls.End()) {
        if (mBound) {
            DisconnectService();
        }
    }
    else {
        // There are m ore pending requests in queue.
        // Just post MCS_BOUND message to trigger processing
        // of next pending install.
        void (STDCALL CCapsuleManagerService::*pHandlerFunc)(
            /* [in] */ IMediaContainerService* mcs);
        pHandlerFunc = &CCapsuleManagerService::HandleMCSBound;
        SendMessage(*(Handle32*)&pHandlerFunc, NULL);
    }
}

void CCapsuleManagerService::HandleMCSGiveUp()
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "mcs_giveup too many retries");
    mPendingInstalls.PopFront();
}

void CCapsuleManagerService::HandleSendPendingBroadcast()
{
    String* capsules = NULL;
    List<String>** components = NULL;
    Int32 size = 0;
    Int32* uids = NULL;
    Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
    {
        Mutex::Autolock lock(mCapsulesLock);

        size = mPendingBroadcasts.GetSize();
        if (size <= 0) {
            // Nothing to be done. Just return
            return;
        }
        capsules = new String[size];
        components = new List<String>*[size];
        uids = new Int32[size];
        Int32 i = 0;
        HashMap<String, List<String>*>::Iterator it;
        for (it = mPendingBroadcasts.Begin();
             it != mPendingBroadcasts.End(); ++it) {
            capsules[i] = it->mFirst;
            components[i] = it->mSecond;
            HashMap<String, AutoPtr<CapsuleSetting> >::Iterator t =
                mSettings->mCapsules.Find(it->mFirst);
            if (t != mSettings->mCapsules.End()
                && t->mSecond != NULL) {
                uids[i] = t->mSecond->mUserId;
            }
            else {
                uids[i] = -1;
            }
            i++;
        }
        size = i;
        mPendingBroadcasts.Clear();
    }
    // Send broadcasts
    for (Int32 i = 0; i < size; i++) {
        SendCapsuleChangedBroadcast(
            capsules[i], TRUE, *components[i], uids[i]);
    }
    delete uids;
    delete components;
    delete capsules;
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
}

void CCapsuleManagerService::HandleStartCleaningCapsule(
    /* [in] */ const String& capsuleName)
{
    Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
    {
        Mutex::Autolock lock(mCapsulesLock);

        if (Find(mSettings->mCapsulesToBeCleaned.Begin(),
            mSettings->mCapsulesToBeCleaned.End(), capsuleName)
            == mSettings->mCapsulesToBeCleaned.End()) {
            mSettings->mCapsulesToBeCleaned.PushBack(capsuleName);
        }
    }
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
    StartCleaningCapsules();
}

void CCapsuleManagerService::HandlePostInstall(
    /* [in] */ Int32 token)
{
    if (DEBUG_INSTALL) {
        Logger::V(TAG, StringBuffer("Handling post-install for ") + token);
    }
    HashMap<Int32, PostInstallData*>::Iterator it = mRunningInstalls.Find(token);
    PostInstallData* data = it == mRunningInstalls.End() ? NULL : it->mSecond;
    mRunningInstalls.Erase(token);
    Boolean deleteOld = FALSE;

    if (data != NULL) {
        InstallArgs* args = data->mArgs;
        CapsuleInstalledInfo* res = data->mRes;

        if (res->mReturnCode == CapsuleManager::INSTALL_SUCCEEDED) {
            res->mRemovedInfo->SendBroadcast(FALSE, TRUE);
            AutoPtr<IBundle> extras;
            CBundle::New(1, (IBundle**)&extras);
            extras->PutInt32(String(Intent_EXTRA_UID), res->mUid);
            Boolean update = !res->mRemovedInfo->mRemovedCapsule.IsNull();
            if (update) {
                extras->PutBoolean(String(Intent_EXTRA_REPLACING), TRUE);
            }
            String resCName;
            res->mCap->mApplicationInfo->GetCapsuleName(&resCName);
            SendCapsuleBroadcast(String(Intent_ACTION_CAPSULE_ADDED),
                resCName, extras, NULL);
            if (update) {
                SendCapsuleBroadcast(String(Intent_ACTION_CAPSULE_REPLACED),
                    resCName, extras, NULL);
            }
            if (res->mRemovedInfo->mArgs != NULL) {
                // Remove the replaced capsule's older resources safely now
                deleteOld = TRUE;
            }
        }
        // Force a gc to clear up things
//        Runtime->MGetRuntime()->Gc();
        // We delete after a gc for applications  on sdcard.
        if (deleteOld) {
            Mutex::Autolock Lock(mInstallLock);
            res->mRemovedInfo->mArgs->DoPostDeleteLI(TRUE);
        }
        if (args->mObserver != NULL) {
            if (FAILED(args->mObserver->CapsuleInstalled(res->mName, res->mReturnCode))) {
                Slogger::I(TAG, "Observer no Int64er exists.");
            }
        }
    }
    else {
        Slogger::E(TAG, StringBuffer("Bogus post-install token ") + token);
    }
}

void CCapsuleManagerService::HandleUpdatedMediaStatus(
    /* [in] */ Boolean reportStatus,
    /* [in] */ Set<SdInstallArgs*>* keys)
{
    if (DEBUG_SD_INSTALL) Logger::I(TAG, "Got message UPDATED_MEDIA_STATUS");
    if (DEBUG_SD_INSTALL) {
        Logger::I(TAG,
            StringBuffer("reportStatus=") + reportStatus);
    }
//    if (doGc) {
//        // Force a gc to clear up stale containers.
//	                Runtime::GetRuntime().Gc();
//    }
    if (keys != NULL) {
        Set<SdInstallArgs*>* args = keys;
        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Unloading all containers");
        // Unload containers
        UnloadAllContainers(*args);
    }
    if (reportStatus) {
        if (DEBUG_SD_INSTALL) Logger::I(TAG, "Invoking MountService call back");
        if (FAILED(CapsuleHelper::GetMountService()->FinishMediaUpdate())) {
            Logger::E(TAG, "MountService not running?");
        }
    }
}

void CCapsuleManagerService::HandleWriteSettings()
{
    Process::SetThreadPriority(Process::THREAD_PRIORITY_DEFAULT);
    {
        Mutex::Autolock lock(mCapsulesLock);
//        RemoveMessages(WRITE_SETTINGS);
        mSettings->WriteLP();
    }
    Process::SetThreadPriority(Process::THREAD_PRIORITY_BACKGROUND);
}


