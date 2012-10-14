
#include "os/Environment.h"
#include "os/SystemProperties.h"
#include "os/ServiceManager.h"
#ifdef _FRAMEWORK_CORE
#include "content/CResourcesFactory.h"
#elif defined(_FRAMEWORK_SERVER)
#include "Elastos.Framework.Core.h"
#endif
#include <elastos/System.h>

using namespace Elastos::Core;

AutoPtr<IFile> Environment::ROOT_DIRECTORY;
const CString Environment::SYSTEM_PROPERTY_EFS_ENABLED = "persist.security.efs.enabled";
AutoPtr<IMountService> Environment::mMntSvc;
AutoPtr<IFile> Environment::DATA_DIRECTORY;
AutoPtr<IFile> Environment::SECURE_DATA_DIRECTORY;
AutoPtr<IFile> Environment::EXTERNAL_STORAGE_DIRECTORY;
AutoPtr<IFile> Environment::EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY;
AutoPtr<IFile> Environment::EXTERNAL_STORAGE_ELASTOS_MEDIA_DIRECTORY;
AutoPtr<IFile> Environment::DOWNLOAD_CACHE_DIRECTORY;

const String Environment::DIRECTORY_MUSIC = String("Music");
const String Environment::DIRECTORY_PODCASTS = String("Podcasts");
const String Environment::DIRECTORY_RINGTONES = String("Ringtones");
const String Environment::DIRECTORY_ALARMS = String("Alarms");
const String Environment::DIRECTORY_NOTIFICATIONS = String("Notifications");
const String Environment::DIRECTORY_PICTURES = String("Pictures");
const String Environment::DIRECTORY_MOVIES = String("Movies");
const String Environment::DIRECTORY_DOWNLOADS = String("Download");
const String Environment::DIRECTORY_DCIM = String("DCIM");
const String Environment::MEDIA_REMOVED = String("removed");
const String Environment::MEDIA_UNMOUNTED = String("unmounted");
const String Environment::MEDIA_CHECKING = String("checking");
const String Environment::MEDIA_NOFS = String("nofs");
const String Environment::MEDIA_MOUNTED = String("mounted");
const String Environment::MEDIA_MOUNTED_READ_ONLY = String("mounted_ro");
const String Environment::MEDIA_SHARED = String("shared");
const String Environment::MEDIA_BAD_REMOVAL = String("bad_removal");
const String Environment::MEDIA_UNMOUNTABLE = String("unmountable");

Boolean Environment::sIsInitilized = FALSE;

AutoPtr<IFile> Environment::GetRootDirectory()
{
    Init();
    return ROOT_DIRECTORY;
}

AutoPtr<IFile> Environment::GetSystemSecureDirectory()
{
    Init();
    AutoPtr<IFile> file;
    if (IsEncryptedFilesystemEnabled()) {
        CFile::New(SECURE_DATA_DIRECTORY, String("system"), (IFile**)&file);
    }
    else {
        CFile::New(DATA_DIRECTORY, String("system"), (IFile**)&file);
    }
    return file;
}

AutoPtr<IFile> Environment::GetSecureDataDirectory()
{
    Init();
    if (IsEncryptedFilesystemEnabled()) {
        return SECURE_DATA_DIRECTORY;
    }
    else {
        return DATA_DIRECTORY;
    }
}

Boolean Environment::IsEncryptedFilesystemEnabled()
{
    return SystemProperties::GetBoolean(SYSTEM_PROPERTY_EFS_ENABLED, FALSE);
}

AutoPtr<IFile> Environment::GetDataDirectory()
{
    Init();
    return DATA_DIRECTORY;
}

AutoPtr<IFile> Environment::GetExternalStorageDirectory()
{
    Init();
    return EXTERNAL_STORAGE_DIRECTORY;
}

AutoPtr<IFile> Environment::GetExternalStoragePublicDirectory(
    /* [in] */ const String& type)
{
    Init();
    AutoPtr<IFile> file;
    CFile::New(GetExternalStorageDirectory(), type, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStorageElastosDataDir()
{
    Init();
    return EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY;
}

AutoPtr<IFile> Environment::GetExternalStorageAppDataDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY, capsuleName, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStorageAppMediaDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_MEDIA_DIRECTORY, capsuleName, (IFile**)&file);
    return file;
}

AutoPtr<IFile> Environment::GetExternalStorageAppFilesDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file1, file2;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY, capsuleName, (IFile**)&file1);
    CFile::New(file1, String("files"), (IFile**)&file2);
    return file2;
}

AutoPtr<IFile> Environment::GetExternalStorageAppCacheDirectory(
    /* [in] */ const String& capsuleName)
{
    Init();
    AutoPtr<IFile> file1, file2;
    CFile::New(EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY, capsuleName, (IFile**)&file1);
    CFile::New(file1, String("cache"), (IFile**)&file2);
    return file2;
}

AutoPtr<IFile> Environment::GetDownloadCacheDirectory()
{
    Init();
    return DOWNLOAD_CACHE_DIRECTORY;
}

String Environment::GetExternalStorageState()
{
    Init();
//    try {
    if (mMntSvc == NULL) {
        mMntSvc = IMountService::Probe(ServiceManager::GetService(String("mount")));
    }

    String str, val;
    GetExternalStorageDirectory()->ToString(&str);
    ECode ec = mMntSvc->GetVolumeState(str, &val);
    if (SUCCEEDED(ec)) {
        return val;
    }
    else {
        return MEDIA_REMOVED;
    }

//    } catch (Exception rex) {
//        return Environment.MEDIA_REMOVED;
//    }
}

Boolean Environment::IsExternalStorageRemovable()
{
    AutoPtr<IResourcesFactory> rf;
    CResourcesFactory::AcquireSingleton((IResourcesFactory**)&rf);
    AutoPtr<IResources> res;
    rf->GetSystem((IResources**)&res);
    Boolean val;
    res->GetBoolean(0x010d0002,/*com.android.internal.R.bool.config_externalStorageRemovable*/
            &val);
    return val;
}

AutoPtr<IFile> Environment::GetDirectory(
    /* [in] */ const String& variableName,
    /* [in] */ const String& defaultPath)
{
    String path;
    System::GetEnv(variableName, &path);
    AutoPtr<IFile> file;
    if (path.IsNull()) {
        CFile::New(defaultPath, (IFile**)&file);
    }
    else {
        CFile::New(path, (IFile**)&file);
    }
    return file;
}

void Environment::Init()
{
    if (!sIsInitilized) {
        ROOT_DIRECTORY = GetDirectory(String("ELASTOS_ROOT"), String("/system"));
        DATA_DIRECTORY = GetDirectory(String("ELASTOS_DATA"), String("/data"));
        SECURE_DATA_DIRECTORY = GetDirectory(String("ELASTOS_SECURE_DATA"), String("/data/secure"));
        EXTERNAL_STORAGE_DIRECTORY = GetDirectory(String("EXTERNAL_STORAGE"), String("/sdcard"));

        AutoPtr<IFile> file1, file2, file3;
        file1 = GetDirectory(String("EXTERNAL_STORAGE"), String("/sdcard"));
        CFile::New(file1, String("Elastos"), (IFile**)&file2);
        CFile::New(file2, String("data"), (IFile**)&EXTERNAL_STORAGE_ELASTOS_DATA_DIRECTORY);

        file1 = GetDirectory(String("EXTERNAL_STORAGE"), String("/sdcard"));
        CFile::New(file1, String("Elastos"), (IFile**)&file3);
        CFile::New(file3, String("media"), (IFile**)&EXTERNAL_STORAGE_ELASTOS_MEDIA_DIRECTORY);

        DOWNLOAD_CACHE_DIRECTORY = GetDirectory(String("DOWNLOAD_CACHE"), String("/cache"));

        sIsInitilized = TRUE;
    }
}
