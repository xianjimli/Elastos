
#include "content/cm/CapsuleManager.h"


const Int32 CapsuleManager::INSTALL_FORWARD_LOCK;
const Int32 CapsuleManager::INSTALL_REPLACE_EXISTING;
const Int32 CapsuleManager::INSTALL_ALLOW_TEST;
const Int32 CapsuleManager::INSTALL_EXTERNAL;
const Int32 CapsuleManager::INSTALL_INTERNAL;
const Int32 CapsuleManager::DONT_KILL_APP;
const Int32 CapsuleManager::INSTALL_SUCCEEDED;
const Int32 CapsuleManager::INSTALL_FAILED_ALREADY_EXISTS;
const Int32 CapsuleManager::INSTALL_FAILED_INVALID_APK;
const Int32 CapsuleManager::INSTALL_FAILED_INVALID_URI;
const Int32 CapsuleManager::INSTALL_FAILED_INSUFFICIENT_STORAGE;
const Int32 CapsuleManager::INSTALL_FAILED_DUPLICATE_CAPSULE;
const Int32 CapsuleManager::INSTALL_FAILED_NO_SHARED_USER;
const Int32 CapsuleManager::INSTALL_FAILED_UPDATE_INCOMPATIBLE;
const Int32 CapsuleManager::INSTALL_FAILED_SHARED_USER_INCOMPATIBLE;
const Int32 CapsuleManager::INSTALL_FAILED_MISSING_SHARED_LIBRARY;
const Int32 CapsuleManager::INSTALL_FAILED_REPLACE_COULDNT_DELETE;
const Int32 CapsuleManager::INSTALL_FAILED_DEXOPT;
const Int32 CapsuleManager::INSTALL_FAILED_OLDER_SDK;
const Int32 CapsuleManager::INSTALL_FAILED_CONFLICTING_PROVIDER;
const Int32 CapsuleManager::INSTALL_FAILED_NEWER_SDK;
const Int32 CapsuleManager::INSTALL_FAILED_TEST_ONLY;
const Int32 CapsuleManager::INSTALL_FAILED_CPU_ABI_INCOMPATIBLE;
const Int32 CapsuleManager::INSTALL_FAILED_MISSING_FEATURE;
const Int32 CapsuleManager::INSTALL_FAILED_CONTAINER_ERROR;
const Int32 CapsuleManager::INSTALL_FAILED_INVALID_INSTALL_LOCATION;
const Int32 CapsuleManager::INSTALL_FAILED_MEDIA_UNAVAILABLE;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_NOT_APK;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_BAD_MANIFEST;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_NO_CERTIFICATES;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_CERTIFICATE_ENCODING;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_BAD_SHARED_USER_ID;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_MALFORMED;
const Int32 CapsuleManager::INSTALL_PARSE_FAILED_MANIFEST_EMPTY;
const Int32 CapsuleManager::INSTALL_FAILED_INTERNAL_ERROR;
const char* CapsuleManager::ACTION_CLEAN_EXTERNAL_STORAGE = "android.content.pm.CLEAN_EXTERNAL_STORAGE";

ECode CapsuleManager::GetCapsuleArchiveInfo(
    /* [in] */ const String& archiveFilePath,
    /* [in] */ Int32 flags,
    /* [out] */ ICapsuleInfo** info)
{
    assert(0);
    // PackageParser packageParser = new PackageParser(archiveFilePath);
    // DisplayMetrics metrics = new DisplayMetrics();
    // metrics.setToDefaults();
    // final File sourceFile = new File(archiveFilePath);
    // PackageParser.Package pkg = packageParser.parsePackage(
    //         sourceFile, archiveFilePath, metrics, 0);
    // if (pkg == null) {
    //     return null;
    // }
    // return PackageParser.generatePackageInfo(pkg, null, flags, 0, 0);
    return E_NOT_IMPLEMENTED;
}
