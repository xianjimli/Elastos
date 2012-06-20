
#include "os/Build.h"

const Int32 Build::VERSION::SDK_INT = SystemProperties::GetInt32("ro.build.version.sdk", 0);
const String Build::VERSION::CODENAME = (const char*)Build::GetString("ro.build.version.codename");

const Int32 Build::VERSION_CODES::CUR_DEVELOPMENT;
const Int32 Build::VERSION_CODES::BASE;
const Int32 Build::VERSION_CODES::BASE_1_1;
const Int32 Build::VERSION_CODES::CUPCAKE;
const Int32 Build::VERSION_CODES::DONUT;
const Int32 Build::VERSION_CODES::ECLAIR;
const Int32 Build::VERSION_CODES::ECLAIR_0_1;
const Int32 Build::VERSION_CODES::ECLAIR_MR1;
const Int32 Build::VERSION_CODES::FROYO;
const Int32 Build::VERSION_CODES::GINGERBREAD;
const Int32 Build::VERSION_CODES::GINGERBREAD_MR1;

const String Build::UNKNOWN = "unknown";
const AutoString Build::ID = (const char*)GetString("ro.build.id");
const AutoString Build::DISPLAY = (const char*)GetString("ro.build.display.id");
const AutoString Build::PRODUCT = (const char*)GetString("ro.product.name");
const AutoString Build::DEVICE = (const char*)GetString("ro.product.device");
const AutoString Build::BOARD = (const char*)GetString("ro.product.board");
const AutoString Build::CPU_ABI = (const char*)GetString("ro.product.cpu.abi");
const AutoString Build::CPU_ABI2 = (const char*)GetString("ro.product.cpu.abi2");
const AutoString Build::MANUFACTURER = (const char*)GetString("ro.product.manufacturer");
const AutoString Build::BRAND = (const char*)GetString("ro.product.brand");
const AutoString Build::MODEL = (const char*)GetString("ro.product.model");
const AutoString Build::BOOTLOADER = (const char*)GetString("ro.bootloader");
const AutoString Build::RADIO = (const char*)GetString("gsm.version.baseband");
const AutoString Build::HARDWARE = (const char*)GetString("ro.hardware");
const AutoString Build::SERIAL = (const char*)GetString("ro.serialno");
const AutoString Build::TYPE = (const char*)GetString("ro.build.type");
const AutoString Build::TAGS = (const char*)GetString("ro.build.tags");
const AutoString Build::FINGERPRINT = (const char*)GetString("ro.build.fingerprint");
const Int64 Build::TIME = GetLong("ro.build.date.utc") * 1000;
const AutoString Build::USER = (const char*)GetString("ro.build.user");
const AutoString Build::HOST = (const char*)GetString("ro.build.host");

String Build::GetString(
    /* [in] */ String property)
{
    return NULL;
}

Int64 Build::GetLong(
    /* [in] */ String property)
{
    return 0;
}
