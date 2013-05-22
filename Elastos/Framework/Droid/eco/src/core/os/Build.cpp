
#include "os/SystemProperties.h"
#include "os/Build.h"

const Int32 Build::VERSION::SDK_INT = SystemProperties::GetInt32("ro.build.version.sdk", 0);
const String Build::VERSION::CODENAME = Build::GetString("ro.build.version.codename");

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

const char* Build::UNKNOWN = "unknown";
const String Build::ID = GetString("ro.build.id");
const String Build::DISPLAY = GetString("ro.build.display.id");
const String Build::PRODUCT = GetString("ro.product.name");
const String Build::DEVICE = GetString("ro.product.device");
const String Build::BOARD = GetString("ro.product.board");
const String Build::CPU_ABI = GetString("ro.product.cpu.abi");
const String Build::CPU_ABI2 = GetString("ro.product.cpu.abi2");
const String Build::MANUFACTURER = GetString("ro.product.manufacturer");
const String Build::BRAND = GetString("ro.product.brand");
const String Build::MODEL = GetString("ro.product.model");
const String Build::BOOTLOADER = GetString("ro.bootloader");
const String Build::RADIO = GetString("gsm.version.baseband");
const String Build::HARDWARE = GetString("ro.hardware");
const String Build::SERIAL = GetString("ro.serialno");
const String Build::TYPE = GetString("ro.build.type");
const String Build::TAGS = GetString("ro.build.tags");
const String Build::FINGERPRINT = GetString("ro.build.fingerprint");
const Int64 Build::TIME = GetLong("ro.build.date.utc") * 1000;
const String Build::USER = GetString("ro.build.user");
const String Build::HOST = GetString("ro.build.host");

String Build::GetString(
    /* [in] */ const char* property)
{
    return String(NULL);
}

Int64 Build::GetLong(
    /* [in] */ const char* property)
{
    return 0;
}
